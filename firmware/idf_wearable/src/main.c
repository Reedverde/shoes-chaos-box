#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_hidh.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "esp_hid_gap.h"

#define PIN_SPI_SCLK 14
#define PIN_SPI_MOSI 27
#define PIN_TFT_CS 26
#define PIN_TFT_DC 25
#define PIN_TFT_RST 33
#define PIN_TFT_BACKLIGHT 32
#define PIN_TRIGGER_BUTTON 13
#define PIN_HALO_TRIGGER 21

#define SCREEN_W 240
#define SCREEN_H 240
#define SCENE_DURATION_MS 2200
#define SCAN_DURATION_SECONDS 8

#define RGB565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))
#define BLACK RGB565(0, 0, 0)
#define WHITE RGB565(255, 255, 255)
#define RED RGB565(255, 0, 0)
#define YELLOW RGB565(255, 255, 0)
#define CYAN RGB565(0, 255, 255)
#define BLUE RGB565(0, 80, 255)
#define NAVY RGB565(0, 0, 120)
#define PURPLE RGB565(120, 0, 120)
#define DARK_GREEN RGB565(0, 100, 30)

typedef enum {
    TRIGGER_LOCAL_BUTTON,
    TRIGGER_PEDAL_LEFT,
    TRIGGER_PEDAL_RIGHT,
} trigger_source_t;

typedef struct {
    const char *top;
    const char *middle;
    const char *bottom;
    uint16_t background;
    uint16_t foreground;
} scene_t;

static const char *TAG = "SHOES";
static const char *PEDAL_NAME = "Bluetooth Music Pedal";
static const scene_t SCENES[] = {
    {"TAKE OFF", "THE", "SHOES", RED, WHITE},
    {"FOOTWEAR", "DETECTED", "!", NAVY, YELLOW},
    {"BOLD", "SHOE", "CHOICE", PURPLE, WHITE},
    {"CHOOSE", "BAREFOOT", "PEACE", DARK_GREEN, WHITE},
};

static spi_device_handle_t display_spi;
static QueueHandle_t trigger_queue;
static volatile bool pedal_connected;
static volatile bool scan_running;

static void lcd_write(bool data_mode, const uint8_t *data, size_t length)
{
    gpio_set_level(PIN_TFT_DC, data_mode);
    spi_transaction_t transaction = {
        .length = length * 8,
        .tx_buffer = data,
    };
    ESP_ERROR_CHECK(spi_device_polling_transmit(display_spi, &transaction));
}

static void lcd_command(uint8_t command)
{
    lcd_write(false, &command, 1);
}

static void lcd_data(const uint8_t *data, size_t length)
{
    lcd_write(true, data, length);
}

static void lcd_command_data(uint8_t command, const uint8_t *data, size_t length)
{
    lcd_command(command);
    if (length) lcd_data(data, length);
}

static void lcd_init(void)
{
    gpio_config_t outputs = {
        .pin_bit_mask = (1ULL << PIN_TFT_DC) | (1ULL << PIN_TFT_RST) | (1ULL << PIN_TFT_BACKLIGHT),
        .mode = GPIO_MODE_OUTPUT,
    };
    ESP_ERROR_CHECK(gpio_config(&outputs));
    gpio_set_level(PIN_TFT_BACKLIGHT, 0);
    gpio_set_level(PIN_TFT_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(PIN_TFT_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(120));

    spi_bus_config_t bus = {
        .mosi_io_num = PIN_SPI_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = PIN_SPI_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = SCREEN_W * 40 * 2,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus, SPI_DMA_CH_AUTO));
    spi_device_interface_config_t device = {
        .clock_speed_hz = 20 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = PIN_TFT_CS,
        .queue_size = 1,
        .flags = SPI_DEVICE_HALFDUPLEX,
    };
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &device, &display_spi));

    static const uint8_t d_eb[] = {0x14};
    static const uint8_t d_84[] = {0x40};
    static const uint8_t d_ff[] = {0xFF};
    static const uint8_t d_88[] = {0x0A};
    static const uint8_t d_89[] = {0x21};
    static const uint8_t d_00[] = {0x00};
    static const uint8_t d_80[] = {0x80};
    static const uint8_t d_01[] = {0x01};
    static const uint8_t d_b6[] = {0x00, 0x20};
    static const uint8_t d_36[] = {0x08};
    static const uint8_t d_3a[] = {0x05};
    static const uint8_t d_90[] = {0x08, 0x08, 0x08, 0x08};
    static const uint8_t d_bd[] = {0x06};
    static const uint8_t d_bc[] = {0x00};
    static const uint8_t d_f0[] = {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A};
    static const uint8_t d_f1[] = {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F};
    static const uint8_t d_ed[] = {0x1B, 0x0B};
    static const uint8_t d_70[] = {0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08, 0x03};
    static const uint8_t d_62[] = {0x18, 0x0D, 0x71, 0xED, 0x70, 0x70, 0x18, 0x0F, 0x71, 0xEF, 0x70, 0x70};
    static const uint8_t d_63[] = {0x18, 0x11, 0x71, 0xF1, 0x70, 0x70, 0x18, 0x13, 0x71, 0xF3, 0x70, 0x70};
    static const uint8_t d_64[] = {0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07};
    static const uint8_t d_66[] = {0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00, 0x00, 0x00};
    static const uint8_t d_67[] = {0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10, 0x32, 0x98};
    static const uint8_t d_74[] = {0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00};
    static const uint8_t d_98[] = {0x3E, 0x07};

    lcd_command(0xEF); lcd_command_data(0xEB, d_eb, sizeof d_eb);
    lcd_command(0xFE); lcd_command(0xEF);
    lcd_command_data(0x84, d_84, 1); lcd_command_data(0x85, d_ff, 1);
    lcd_command_data(0x86, d_ff, 1); lcd_command_data(0x87, d_ff, 1);
    lcd_command_data(0x88, d_88, 1); lcd_command_data(0x89, d_89, 1);
    lcd_command_data(0x8A, d_00, 1); lcd_command_data(0x8B, d_80, 1);
    lcd_command_data(0x8C, d_01, 1); lcd_command_data(0x8D, d_01, 1);
    lcd_command_data(0x8E, d_ff, 1); lcd_command_data(0x8F, d_ff, 1);
    lcd_command_data(0xB6, d_b6, sizeof d_b6); lcd_command_data(0x36, d_36, 1);
    lcd_command_data(0x3A, d_3a, 1); lcd_command_data(0x90, d_90, sizeof d_90);
    lcd_command_data(0xBD, d_bd, 1); lcd_command_data(0xBC, d_bc, 1);
    lcd_command_data(0xF0, d_f0, sizeof d_f0); lcd_command_data(0xF1, d_f1, sizeof d_f1);
    lcd_command_data(0xF2, d_f0, sizeof d_f0); lcd_command_data(0xF3, d_f1, sizeof d_f1);
    lcd_command_data(0xED, d_ed, sizeof d_ed); lcd_command_data(0x70, d_70, sizeof d_70);
    lcd_command_data(0x62, d_62, sizeof d_62); lcd_command_data(0x63, d_63, sizeof d_63);
    lcd_command_data(0x64, d_64, sizeof d_64); lcd_command_data(0x66, d_66, sizeof d_66);
    lcd_command_data(0x67, d_67, sizeof d_67); lcd_command_data(0x74, d_74, sizeof d_74);
    lcd_command_data(0x98, d_98, sizeof d_98);
    lcd_command(0x35);
    lcd_command(0x21);
    lcd_command(0x11);
    vTaskDelay(pdMS_TO_TICKS(120));
    lcd_command(0x29);
    vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(PIN_TFT_BACKLIGHT, 1);
}

static void lcd_window(int x0, int y0, int x1, int y1)
{
    uint8_t x[] = {(uint8_t)(x0 >> 8), (uint8_t)x0, (uint8_t)(x1 >> 8), (uint8_t)x1};
    uint8_t y[] = {(uint8_t)(y0 >> 8), (uint8_t)y0, (uint8_t)(y1 >> 8), (uint8_t)y1};
    lcd_command_data(0x2A, x, sizeof x);
    lcd_command_data(0x2B, y, sizeof y);
    lcd_command(0x2C);
}

static void fill_rect(int x, int y, int w, int h, uint16_t color)
{
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if (x + w > SCREEN_W) w = SCREEN_W - x;
    if (y + h > SCREEN_H) h = SCREEN_H - y;
    if (w <= 0 || h <= 0) return;

    lcd_window(x, y, x + w - 1, y + h - 1);
    uint8_t pixels[512];
    for (size_t i = 0; i < sizeof pixels; i += 2) {
        pixels[i] = color >> 8;
        pixels[i + 1] = color & 0xFF;
    }
    int remaining = w * h;
    while (remaining > 0) {
        int count = remaining > 256 ? 256 : remaining;
        lcd_data(pixels, count * 2);
        remaining -= count;
    }
}

static void draw_pixel(int x, int y, uint16_t color)
{
    fill_rect(x, y, 1, 1, color);
}

static void draw_circle(int cx, int cy, int radius, uint16_t color)
{
    int x = -radius, y = 0, error = 2 - 2 * radius;
    do {
        draw_pixel(cx - x, cy + y, color); draw_pixel(cx - y, cy - x, color);
        draw_pixel(cx + x, cy - y, color); draw_pixel(cx + y, cy + x, color);
        int previous = error;
        if (previous <= y) error += ++y * 2 + 1;
        if (previous > x || error > y) error += ++x * 2 + 1;
    } while (x < 0);
}

static const char **glyph(char c)
{
    static const char *blank[] = {"00000","00000","00000","00000","00000","00000","00000"};
    static const char *bang[]  = {"00100","00100","00100","00100","00100","00000","00100"};
    static const char *A[] = {"01110","10001","10001","11111","10001","10001","10001"};
    static const char *B[] = {"11110","10001","10001","11110","10001","10001","11110"};
    static const char *C[] = {"01111","10000","10000","10000","10000","10000","01111"};
    static const char *D[] = {"11110","10001","10001","10001","10001","10001","11110"};
    static const char *E[] = {"11111","10000","10000","11110","10000","10000","11111"};
    static const char *F[] = {"11111","10000","10000","11110","10000","10000","10000"};
    static const char *H[] = {"10001","10001","10001","11111","10001","10001","10001"};
    static const char *I[] = {"11111","00100","00100","00100","00100","00100","11111"};
    static const char *K[] = {"10001","10010","10100","11000","10100","10010","10001"};
    static const char *L[] = {"10000","10000","10000","10000","10000","10000","11111"};
    static const char *N[] = {"10001","11001","11001","10101","10011","10011","10001"};
    static const char *O[] = {"01110","10001","10001","10001","10001","10001","01110"};
    static const char *P[] = {"11110","10001","10001","11110","10000","10000","10000"};
    static const char *R[] = {"11110","10001","10001","11110","10100","10010","10001"};
    static const char *S[] = {"01111","10000","10000","01110","00001","00001","11110"};
    static const char *T[] = {"11111","00100","00100","00100","00100","00100","00100"};
    static const char *U[] = {"10001","10001","10001","10001","10001","10001","01110"};
    static const char *W[] = {"10001","10001","10001","10101","10101","10101","01010"};
    static const char *X[] = {"10001","10001","01010","00100","01010","10001","10001"};
    static const char *Y[] = {"10001","10001","01010","00100","00100","00100","00100"};
    switch (c) {
        case '!': return bang; case 'A': return A; case 'B': return B; case 'C': return C;
        case 'D': return D; case 'E': return E; case 'F': return F; case 'H': return H;
        case 'I': return I; case 'K': return K; case 'L': return L; case 'N': return N;
        case 'O': return O; case 'P': return P;
        case 'R': return R; case 'S': return S; case 'T': return T; case 'U': return U;
        case 'W': return W; case 'X': return X; case 'Y': return Y; default: return blank;
    }
}

static void draw_text(const char *text, int y, int scale, uint16_t color)
{
    int length = strlen(text);
    int width = length ? length * 6 * scale - scale : 0;
    int x0 = (SCREEN_W - width) / 2;
    for (int i = 0; i < length; ++i) {
        const char **rows = glyph(text[i]);
        for (int row = 0; row < 7; ++row) {
            for (int col = 0; col < 5; ++col) {
                if (rows[row][col] == '1') fill_rect(x0 + (i * 6 + col) * scale, y + row * scale, scale, scale, color);
            }
        }
    }
}

static void draw_idle(void)
{
    fill_rect(0, 0, SCREEN_W, SCREEN_H, BLACK);
    draw_circle(120, 120, 112, CYAN);
    draw_circle(120, 120, 108, BLUE);
    draw_text("SHOES", 58, 3, WHITE);
    draw_text("CHAOS", 96, 3, CYAN);
    draw_text("BOX", 134, 3, WHITE);
    draw_text(pedal_connected ? "PEDAL READY" : "BUTTON READY", 188, 1, YELLOW);
}

static void draw_scene(size_t index)
{
    const scene_t *scene = &SCENES[index];
    fill_rect(0, 0, SCREEN_W, SCREEN_H, scene->background);
    draw_circle(120, 120, 114, scene->foreground);
    draw_text(scene->top, 60, 2, scene->foreground);
    draw_text(scene->middle, 104, 3, scene->foreground);
    draw_text(scene->bottom, 152, 2, scene->foreground);
}

static void request_trigger(trigger_source_t source)
{
    if (trigger_queue) xQueueSend(trigger_queue, &source, 0);
}

static void hidh_callback(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
{
    esp_hidh_event_t event = (esp_hidh_event_t)id;
    esp_hidh_event_data_t *param = event_data;
    switch (event) {
        case ESP_HIDH_OPEN_EVENT:
            if (param->open.status == ESP_OK) {
                pedal_connected = true;
                const uint8_t *bda = esp_hidh_dev_bda_get(param->open.dev);
                ESP_LOGI(TAG, "PEDAL_CONNECTED " ESP_BD_ADDR_STR, ESP_BD_ADDR_HEX(bda));
            } else {
                pedal_connected = false;
                ESP_LOGE(TAG, "PEDAL_CONNECT_FAILED");
            }
            break;
        case ESP_HIDH_INPUT_EVENT:
            if (param->input.usage == ESP_HID_USAGE_KEYBOARD && param->input.length >= 3) {
                uint8_t key = param->input.data[2];
                if (key == 0x2C) {
                    ESP_LOGI(TAG, "TRIGGER pedal-left/space");
                    request_trigger(TRIGGER_PEDAL_LEFT);
                } else if (key == 0x28) {
                    ESP_LOGI(TAG, "TRIGGER pedal-right/enter");
                    request_trigger(TRIGGER_PEDAL_RIGHT);
                }
            }
            break;
        case ESP_HIDH_CLOSE_EVENT:
            pedal_connected = false;
            ESP_LOGW(TAG, "PEDAL_DISCONNECTED");
            break;
        default:
            break;
    }
}

static void pedal_scan_task(void *unused)
{
    scan_running = true;
    while (!pedal_connected) {
        size_t count = 0;
        esp_hid_scan_result_t *results = NULL;
        ESP_LOGI(TAG, "PEDAL_SCAN");
        esp_hid_scan(SCAN_DURATION_SECONDS, &count, &results);
        esp_hid_scan_result_t *match = NULL;
        for (esp_hid_scan_result_t *item = results; item; item = item->next) {
            if (item->transport == ESP_HID_TRANSPORT_BT && item->name && strcmp(item->name, PEDAL_NAME) == 0) {
                match = item;
                break;
            }
        }
        if (match) {
            ESP_LOGI(TAG, "PEDAL_FOUND");
            esp_hidh_dev_open(match->bda, match->transport, match->ble.addr_type);
        }
        esp_hid_scan_results_free(results);
        for (int i = 0; i < 20 && !pedal_connected; ++i) vTaskDelay(pdMS_TO_TICKS(250));
    }
    scan_running = false;
    vTaskDelete(NULL);
}

static void start_scan_if_needed(void)
{
    if (!pedal_connected && !scan_running) xTaskCreate(pedal_scan_task, "pedal_scan", 6144, NULL, 2, NULL);
}

static void app_task(void *unused)
{
    size_t scene_index = 0;
    bool button_was_down = false;
    bool scene_active = false;
    TickType_t scene_started = 0;
    draw_idle();

    while (true) {
        bool button_down = gpio_get_level(PIN_TRIGGER_BUTTON) == 0;
        if (button_down && !button_was_down) request_trigger(TRIGGER_LOCAL_BUTTON);
        button_was_down = button_down;

        trigger_source_t source;
        if (!scene_active && xQueueReceive(trigger_queue, &source, 0) == pdTRUE) {
            ESP_LOGI(TAG, "SCENE_%02u source=%d", (unsigned)(scene_index + 1), source);
            gpio_set_level(PIN_HALO_TRIGGER, 1);
            draw_scene(scene_index);
            gpio_set_level(PIN_HALO_TRIGGER, 0);
            scene_index = (scene_index + 1) % (sizeof SCENES / sizeof SCENES[0]);
            scene_started = xTaskGetTickCount();
            scene_active = true;
        }
        if (scene_active && (xTaskGetTickCount() - scene_started) >= pdMS_TO_TICKS(SCENE_DURATION_MS)) {
            scene_active = false;
            draw_idle();
        }
        start_scan_if_needed();
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void app_main(void)
{
    esp_err_t result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        result = nvs_flash_init();
    }
    ESP_ERROR_CHECK(result);

    gpio_config_t button = {
        .pin_bit_mask = 1ULL << PIN_TRIGGER_BUTTON,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&button));
    gpio_config_t halo_trigger = {
        .pin_bit_mask = 1ULL << PIN_HALO_TRIGGER,
        .mode = GPIO_MODE_OUTPUT,
    };
    ESP_ERROR_CHECK(gpio_config(&halo_trigger));
    gpio_set_level(PIN_HALO_TRIGGER, 0);
    trigger_queue = xQueueCreate(8, sizeof(trigger_source_t));
    lcd_init();

    ESP_ERROR_CHECK(esp_hid_gap_init(HID_HOST_MODE));
    esp_hidh_config_t hid_config = {
        .callback = hidh_callback,
        .event_stack_size = 4096,
        .callback_arg = NULL,
    };
    ESP_ERROR_CHECK(esp_hidh_init(&hid_config));

    xTaskCreate(app_task, "shoes_app", 6144, NULL, 3, NULL);
    ESP_LOGI(TAG, "READY button + classic Bluetooth pedal");
}
