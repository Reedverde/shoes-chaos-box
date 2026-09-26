#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver/gpio.h"
#include "driver/sdspi_host.h"
#include "driver/spi_master.h"
#include "driver/uart.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_hidh.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_random.h"
#include "esp_rom_sys.h"
#include "esp_vfs_fat.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "sdmmc_cmd.h"

#include "esp_hid_gap.h"
#include "embedded_display_assets.h"
#include "embedded_master_f001.h"
#include "embedded_master_f007.h"
#include "embedded_master_f013.h"
#include "embedded_master_f019.h"
#include "embedded_master_f025.h"
#include "embedded_master_f031.h"

#if __has_include("local_face_asset.h")
#include "local_face_asset.h"
#define HAS_LOCAL_FACE_ASSET 1
#else
#define HAS_LOCAL_FACE_ASSET 0
#endif

#define PIN_SPI_SCLK 14
#define PIN_SPI_MOSI 27
#define PIN_TFT_CS 26
#define PIN_TFT_DC 25
#define PIN_TFT_RST 33
#define PIN_TFT_BACKLIGHT 32
#define PIN_TRIGGER_BUTTON 13
#define PIN_QR_BUTTON 22
#define PIN_HALO_TRIGGER 21
#define PIN_DFPLAYER_RX 16
#define PIN_DFPLAYER_TX 17
#define DFPLAYER_UART UART_NUM_2
#define PIN_VISUAL_SD_MISO 19
#define PIN_VISUAL_SD_CS 5

#define SCREEN_W 240
#define SCREEN_H 240
#define TRIGGER_LOCKOUT_MS 10000
#define SCAN_DURATION_SECONDS 8
#define MEDIA_SCENE_MAX 27
#define MEDIA_FRAME_MAX 48
#define NON_SHOES_SCENE_COUNT 17
#define SHOES_SCENE_FIRST 17
#define SHOES_SCENE_COUNT 10
#define CURB_SCENE_INDEX 2
#define PLAYBACK_ORDER_MAX 36
#define RAW_FRAME_BYTES (SCREEN_W * SCREEN_H * 2)
#define RAW_HALF_FRAME_BYTES (RAW_FRAME_BYTES / 2)
#define RAW_ROWS_PER_CHUNK 16
#define RAW_TRANSFER_BYTES (SCREEN_W * 2 * RAW_ROWS_PER_CHUNK)
#define DISPLAY_SPI_HZ (32 * 1000 * 1000)
#define DFPLAYER_STARTUP_MS 120
#define MASTER_SCENE_AUDIO_ID 18
#define MASTER_SCENE_DURATION_MS 6000
#define VOLUME_DEFAULT 18
#define VOLUME_MIN 0
#define VOLUME_MAX 30
#define VOLUME_OVERLAY_MS 1200
#define HALO_SYNC_HIGH_US 60000
#define HALO_SYNC_LOW_US 30000
#define HALO_BIT_ZERO_US 10000
#define HALO_BIT_ONE_US 30000
#define HALO_BIT_GAP_US 15000

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
    TRIGGER_VOLUME_UP,
    TRIGGER_VOLUME_DOWN,
} trigger_source_t;

typedef struct {
    const char *top;
    const char *middle;
    const char *bottom;
    uint16_t background;
    uint16_t foreground;
} scene_t;

typedef struct {
    char id[5];
    uint16_t audio_id;
    uint32_t duration_ms;
    char frames_csv[32];
    uint32_t lockout_ms;
    uint16_t halo_tail_ms;
} media_scene_t;

typedef struct {
    char bmp[13];
    char raw[13];
    uint32_t end_ms;
} media_frame_t;

static const char *TAG = "SHOES";
static const char *PEDAL_NAME = "Bluetooth Music Pedal";
static const scene_t SCENES[] = {
    {"OH MY GOD", "", "SHOES", PURPLE, WHITE},
    {"LETS GET", "SOME", "SHOES", NAVY, YELLOW},
    {"THESE SHOES", "", "RULE", DARK_GREEN, WHITE},
    {"THESE SHOES", "", "SUCK", RED, YELLOW},
};

static spi_device_handle_t display_spi;
static QueueHandle_t trigger_queue;
static volatile bool trigger_locked;
static volatile bool pedal_connected;
static volatile bool scan_running;
static bool visual_sd_ready;
static sdmmc_card_t *visual_sd_card;
static media_scene_t media_scenes[MEDIA_SCENE_MAX];
static size_t media_scene_count;
// The LCD retains displayed pixels. Two half-frame buffers together stage one
// complete frame: both halves are read from the slow 4 MHz SD card before
// either is transferred to the display, avoiding a visible top-down peel and
// avoiding one contiguous 115,200-byte allocation.
static uint8_t *raw_frame_buffers[2];
static uint8_t playback_order[PLAYBACK_ORDER_MAX];
static size_t playback_order_count;
static size_t playback_order_cursor;
static uint8_t current_volume = VOLUME_DEFAULT;

typedef struct {
    const uint8_t *start;
    size_t length;
} embedded_raw_frame_t;

static const embedded_raw_frame_t MASTER_SCENE_FRAMES[] = {
    {master_f001, sizeof master_f001},
    {master_f007, sizeof master_f007},
    {master_f013, sizeof master_f013},
    {master_f019, sizeof master_f019},
    {master_f025, sizeof master_f025},
    {master_f031, sizeof master_f031},
};

static bool draw_sd_bmp(const char *path);
static bool draw_embedded_bmp(const uint8_t *start, const uint8_t *end);
static size_t load_media_scene_index(void);

static void halo_start_scene(uint8_t scene_index)
{
    // One-wire scene protocol on the existing GPIO21 -> A1 connection:
    // sync pulse, five LSB-first scene bits, then sustained HIGH for the
    // scene. The Circuit Playground starts immediately on the final rise.
    gpio_set_level(PIN_HALO_TRIGGER, 0);
    esp_rom_delay_us(HALO_BIT_GAP_US);
    gpio_set_level(PIN_HALO_TRIGGER, 1);
    esp_rom_delay_us(HALO_SYNC_HIGH_US);
    gpio_set_level(PIN_HALO_TRIGGER, 0);
    esp_rom_delay_us(HALO_SYNC_LOW_US);

    for (uint8_t bit = 0; bit < 5; ++bit) {
        gpio_set_level(PIN_HALO_TRIGGER, 1);
        esp_rom_delay_us((scene_index & (1U << bit)) ?
                         HALO_BIT_ONE_US : HALO_BIT_ZERO_US);
        gpio_set_level(PIN_HALO_TRIGGER, 0);
        esp_rom_delay_us(HALO_BIT_GAP_US);
    }
    gpio_set_level(PIN_HALO_TRIGGER, 1);
    ESP_LOGI(TAG, "HALO_SCENE_CODE S%03u", (unsigned)scene_index + 1);
}

static void halo_stop(void)
{
    gpio_set_level(PIN_HALO_TRIGGER, 0);
}

static void dfplayer_command(uint8_t command, uint16_t parameter)
{
    uint8_t packet[10] = {
        0x7E, 0xFF, 0x06, command, 0x00,
        (uint8_t)(parameter >> 8), (uint8_t)parameter,
        0x00, 0x00, 0xEF,
    };
    int16_t checksum = -(int16_t)(packet[1] + packet[2] + packet[3] +
                                  packet[4] + packet[5] + packet[6]);
    packet[7] = (uint8_t)(checksum >> 8);
    packet[8] = (uint8_t)checksum;
    uart_write_bytes(DFPLAYER_UART, packet, sizeof packet);
    uart_wait_tx_done(DFPLAYER_UART, pdMS_TO_TICKS(100));
}

static void dfplayer_init(void)
{
    const uart_config_t config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    ESP_ERROR_CHECK(uart_driver_install(DFPLAYER_UART, 256, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(DFPLAYER_UART, &config));
    ESP_ERROR_CHECK(uart_set_pin(DFPLAYER_UART, PIN_DFPLAYER_TX, PIN_DFPLAYER_RX,
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // Give the player and card time to become ready, then select the TF card
    // and use a conservative wearable volume.
    vTaskDelay(pdMS_TO_TICKS(1200));
    dfplayer_command(0x09, 2);
    vTaskDelay(pdMS_TO_TICKS(200));
    dfplayer_command(0x06, current_volume);
    ESP_LOGI(TAG, "DFPLAYER_READY volume=%u", (unsigned)current_volume);
}

static void volume_load(void)
{
    nvs_handle_t handle;
    uint8_t stored = VOLUME_DEFAULT;
    if (nvs_open("shoes", NVS_READONLY, &handle) == ESP_OK) {
        if (nvs_get_u8(handle, "volume", &stored) != ESP_OK || stored > VOLUME_MAX) {
            stored = VOLUME_DEFAULT;
        }
        nvs_close(handle);
    }
    current_volume = stored;
}

static void volume_save(void)
{
    nvs_handle_t handle;
    if (nvs_open("shoes", NVS_READWRITE, &handle) != ESP_OK) return;
    if (nvs_set_u8(handle, "volume", current_volume) == ESP_OK) nvs_commit(handle);
    nvs_close(handle);
}

static void dfplayer_play_track(uint16_t audio_id)
{
    // 0x12 selects /mp3/NNNN.mp3 using the four-digit folder convention.
    dfplayer_command(0x12, audio_id);
    ESP_LOGI(TAG, "DFPLAYER_PLAY /mp3/%04u.mp3", (unsigned)audio_id);
}

static void dfplayer_stop(void)
{
    dfplayer_command(0x16, 0);
}

static void visual_sd_init(void)
{
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI2_HOST;
    // Start conservatively: the wearable prototype uses long breadboard jumpers
    // and an HW-125 level-shifting module. Higher SPI rates can make the card
    // appear completely absent even when its wiring and power are correct.
    host.max_freq_khz = 4000;
    sdspi_device_config_t slot = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot.host_id = SPI2_HOST;
    slot.gpio_cs = PIN_VISUAL_SD_CS;
    esp_vfs_fat_mount_config_t mount = {
        .format_if_mount_failed = false,
        .max_files = 3,
        .allocation_unit_size = 16 * 1024,
    };
    esp_err_t result = esp_vfs_fat_sdspi_mount("/visual", &host, &slot, &mount,
                                               &visual_sd_card);
    visual_sd_ready = result == ESP_OK;
    if (visual_sd_ready) {
        ESP_LOGI(TAG, "VISUAL_SD_READY");
    } else {
        ESP_LOGE(TAG, "VISUAL_SD_FAILED: %s", esp_err_to_name(result));
    }
}

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
        .miso_io_num = PIN_VISUAL_SD_MISO,
        .sclk_io_num = PIN_SPI_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = SCREEN_W * 40 * 2,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus, SPI_DMA_CH_AUTO));
    spi_device_interface_config_t device = {
        .clock_speed_hz = DISPLAY_SPI_HZ,
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
    static const char *G[] = {"01111","10000","10000","10111","10001","10001","01111"};
    static const char *H[] = {"10001","10001","10001","11111","10001","10001","10001"};
    static const char *I[] = {"11111","00100","00100","00100","00100","00100","11111"};
    static const char *J[] = {"00111","00010","00010","00010","10010","10010","01100"};
    static const char *K[] = {"10001","10010","10100","11000","10100","10010","10001"};
    static const char *L[] = {"10000","10000","10000","10000","10000","10000","11111"};
    static const char *M[] = {"10001","11011","10101","10101","10001","10001","10001"};
    static const char *N[] = {"10001","11001","11001","10101","10011","10011","10001"};
    static const char *O[] = {"01110","10001","10001","10001","10001","10001","01110"};
    static const char *P[] = {"11110","10001","10001","11110","10000","10000","10000"};
    static const char *Q[] = {"01110","10001","10001","10001","10101","10010","01101"};
    static const char *R[] = {"11110","10001","10001","11110","10100","10010","10001"};
    static const char *S[] = {"01111","10000","10000","01110","00001","00001","11110"};
    static const char *T[] = {"11111","00100","00100","00100","00100","00100","00100"};
    static const char *U[] = {"10001","10001","10001","10001","10001","10001","01110"};
    static const char *V[] = {"10001","10001","10001","10001","10001","01010","00100"};
    static const char *W[] = {"10001","10001","10001","10101","10101","10101","01010"};
    static const char *X[] = {"10001","10001","01010","00100","01010","10001","10001"};
    static const char *Y[] = {"10001","10001","01010","00100","00100","00100","00100"};
    static const char *Z[] = {"11111","00001","00010","00100","01000","10000","11111"};
    static const char *n0[] = {"01110","10001","10011","10101","11001","10001","01110"};
    static const char *n1[] = {"00100","01100","00100","00100","00100","00100","01110"};
    static const char *n2[] = {"01110","10001","00001","00010","00100","01000","11111"};
    static const char *n3[] = {"11110","00001","00001","01110","00001","00001","11110"};
    static const char *n4[] = {"00010","00110","01010","10010","11111","00010","00010"};
    static const char *n5[] = {"11111","10000","10000","11110","00001","00001","11110"};
    static const char *n6[] = {"01110","10000","10000","11110","10001","10001","01110"};
    static const char *n7[] = {"11111","00001","00010","00100","01000","01000","01000"};
    static const char *n8[] = {"01110","10001","10001","01110","10001","10001","01110"};
    static const char *n9[] = {"01110","10001","10001","01111","00001","00001","01110"};
    switch (c) {
        case '!': return bang; case 'A': return A; case 'B': return B; case 'C': return C;
        case 'D': return D; case 'E': return E; case 'F': return F; case 'G': return G;
        case 'H': return H; case 'I': return I; case 'J': return J; case 'K': return K;
        case 'L': return L; case 'M': return M; case 'N': return N; case 'O': return O;
        case 'P': return P; case 'Q': return Q;
        case 'R': return R; case 'S': return S; case 'T': return T; case 'U': return U;
        case 'V': return V; case 'W': return W; case 'X': return X; case 'Y': return Y;
        case 'Z': return Z;
        case '0': return n0; case '1': return n1; case '2': return n2;
        case '3': return n3; case '4': return n4; case '5': return n5;
        case '6': return n6; case '7': return n7; case '8': return n8;
        case '9': return n9; default: return blank;
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

static void draw_qr_code(void)
{
    // Keep the QR available even when the removable visual card is absent.
    if (draw_embedded_bmp(assets_QR_BMP,
                          assets_QR_BMP + assets_QR_BMP_len)) return;
    if (visual_sd_ready && draw_sd_bmp("/visual/QR.BMP")) return;

    static const char *rows[25] = {
        "1111111010110010101111111", "1000001011001000101000001",
        "1011101010100101001011101", "1011101000101100101011101",
        "1011101010110010101011101", "1000001001010100101000001",
        "1111111010101010101111111", "0000000001110100100000000",
        "1001111111000011110010111", "1101010000000111000111110",
        "1110011010001101001001001", "0001010111011011101001111",
        "0100001010010001001000001", "1001110010101011100010010",
        "1111101011100111011011111", "1000010110001000011101101",
        "1001101011111111111110110", "0000000010110100100010110",
        "1111111011001000101010001", "1000001011010101100010001",
        "1011101010111011111110000", "1011101011101000011000011",
        "1011101000111110010011111", "1000001001100011001110111",
        "1111111011011000111001001",
    };
    const int module = 4;
    const int quiet = 4;
    const int total = (25 + quiet * 2) * module;
    const int origin_x = (SCREEN_W - total) / 2;
    const int origin_y = 54;

    // Keep every visible element inside the safe circular area. The labels
    // hug the QR's required quiet zone without intruding into it.
    fill_rect(0, 0, SCREEN_W, SCREEN_H, WHITE);
    draw_text("REEDVERDE", 18, 2, DARK_GREEN);
    draw_text("INC", 36, 2, DARK_GREEN);
    for (int y = 0; y < 25; ++y) {
        for (int x = 0; x < 25; ++x) {
            if (rows[y][x] == '1') {
                int qx = origin_x + (x + quiet) * module;
                int qy = origin_y + (y + quiet) * module;
                fill_rect(qx, qy, module, module, BLACK);
            }
        }
    }
    draw_text("REED", 190, 2, DARK_GREEN);
    draw_text("VERDESOTO", 208, 2, DARK_GREEN);
}

#if HAS_LOCAL_FACE_ASSET
static void draw_face_sprite(uint8_t sprite)
{
    const int sprite_x = (sprite % 3) * 120;
    const int sprite_y = (sprite / 3) * 120;
    uint8_t row[SCREEN_W * 2];
    lcd_window(0, 0, SCREEN_W - 1, SCREEN_H - 1);
    for (int y = 0; y < SCREEN_H; ++y) {
        int sy = sprite_y + y / 2;
        for (int x = 0; x < SCREEN_W; ++x) {
            int sx = sprite_x + x / 2;
            size_t source = ((size_t)sy * 360 + sx) * 2;
            row[x * 2] = shoes_sprite_rgb565[source];
            row[x * 2 + 1] = shoes_sprite_rgb565[source + 1];
        }
        lcd_data(row, sizeof row);
    }
}

static void play_face_sprite(size_t scene)
{
    static const uint8_t cuts[4][8] = {
        {4, 5, 4, 2, 5, 4, 5, 2},
        {0, 1, 2, 0, 2, 1, 0, 2},
        {3, 4, 3, 5, 4, 3, 4, 5},
        {2, 5, 2, 4, 5, 2, 5, 4},
    };
    static const uint16_t holds[8] = {310, 170, 420, 140, 260, 190, 350, 440};

    for (size_t pass = 0; pass < 2; ++pass) {
        for (size_t frame = 0; frame < 8; ++frame) {
            draw_face_sprite(cuts[scene][frame]);
            vTaskDelay(pdMS_TO_TICKS(holds[frame]));
        }
    }

    draw_face_sprite(cuts[scene][7]);
    fill_rect(0, 8, SCREEN_W, 34, BLACK);
    fill_rect(0, 158, SCREEN_W, 74, BLACK);
    draw_text(SCENES[scene].top, 14, 2, YELLOW);
    if (SCENES[scene].middle[0]) draw_text(SCENES[scene].middle, 164, 3, WHITE);
    draw_text(SCENES[scene].bottom, SCENES[scene].middle[0] ? 198 : 174, 3, YELLOW);
}
#endif

static void draw_idle(void)
{
    // The branded rest state lives in ESP32 flash; the SD card is reserved
    // for the larger scene library.
    if (draw_embedded_bmp(assets_OPENING_BMP,
                          assets_OPENING_BMP + assets_OPENING_BMP_len)) return;
    if (visual_sd_ready && draw_sd_bmp("/visual/OPENING.BMP")) return;

    fill_rect(0, 0, SCREEN_W, SCREEN_H, BLACK);
    draw_circle(120, 120, 112, CYAN);
    draw_circle(120, 120, 108, BLUE);
    draw_text("SHOES", 58, 3, WHITE);
    draw_text("CHAOS", 96, 3, CYAN);
    draw_text("BOX", 134, 3, WHITE);
    draw_text(pedal_connected ? "PEDAL READY" : "BUTTON READY", 188, 1, YELLOW);
}

static void draw_volume_screen(void)
{
    char value[4];
    snprintf(value, sizeof value, "%u", (unsigned)current_volume);
    fill_rect(0, 0, SCREEN_W, SCREEN_H, BLACK);
    draw_circle(120, 120, 114, CYAN);
    draw_circle(120, 120, 109, BLUE);
    draw_text("VOLUME", 52, 3, WHITE);
    draw_text(value, 96, 7, YELLOW);
    draw_text("OF 30", 174, 2, CYAN);
}

static void volume_adjust(int delta)
{
    int next = (int)current_volume + delta;
    if (next < VOLUME_MIN) next = VOLUME_MIN;
    if (next > VOLUME_MAX) next = VOLUME_MAX;
    if ((uint8_t)next != current_volume) {
        current_volume = (uint8_t)next;
        dfplayer_command(0x06, current_volume);
        volume_save();
    }
    ESP_LOGI(TAG, "VOLUME %u", (unsigned)current_volume);
    draw_volume_screen();
}

static uint16_t read_le16(const uint8_t *bytes)
{
    return (uint16_t)bytes[0] | ((uint16_t)bytes[1] << 8);
}

static uint32_t read_le32(const uint8_t *bytes)
{
    return (uint32_t)bytes[0] | ((uint32_t)bytes[1] << 8) |
           ((uint32_t)bytes[2] << 16) | ((uint32_t)bytes[3] << 24);
}

static bool draw_embedded_bmp(const uint8_t *start, const uint8_t *end)
{
    size_t size = (size_t)(end - start);
    if (size < 54 || start[0] != 'B' || start[1] != 'M') return false;

    uint32_t pixel_offset = read_le32(&start[10]);
    int32_t width = (int32_t)read_le32(&start[18]);
    int32_t height = (int32_t)read_le32(&start[22]);
    uint16_t bits = read_le16(&start[28]);
    size_t pixel_bytes = (size_t)SCREEN_W * SCREEN_H * 4;
    if (width != SCREEN_W || (height != SCREEN_H && height != -SCREEN_H) ||
        bits != 32 || pixel_offset > size || pixel_bytes > size - pixel_offset) {
        ESP_LOGE(TAG, "EMBEDDED_BMP_UNSUPPORTED");
        return false;
    }

    uint8_t output[SCREEN_W * 2];
    lcd_window(0, 0, SCREEN_W - 1, SCREEN_H - 1);
    for (int y = 0; y < SCREEN_H; ++y) {
        int source_y = height < 0 ? y : (SCREEN_H - 1 - y);
        const uint8_t *source = start + pixel_offset +
                                (size_t)source_y * SCREEN_W * 4;
        for (int x = 0; x < SCREEN_W; ++x) {
            uint8_t blue = source[x * 4];
            uint8_t green = source[x * 4 + 1];
            uint8_t red = source[x * 4 + 2];
            uint16_t pixel = RGB565(red, green, blue);
            output[x * 2] = pixel >> 8;
            output[x * 2 + 1] = pixel & 0xFF;
        }
        lcd_data(output, sizeof output);
    }
    ESP_LOGI(TAG, "EMBEDDED_BMP_DRAWN");
    return true;
}

static bool draw_sd_bmp(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (!file) return false;

    uint8_t header[54];
    bool valid = fread(header, 1, sizeof header, file) == sizeof header &&
                 header[0] == 'B' && header[1] == 'M';
    uint32_t pixel_offset = valid ? read_le32(&header[10]) : 0;
    int32_t width = valid ? (int32_t)read_le32(&header[18]) : 0;
    int32_t height = valid ? (int32_t)read_le32(&header[22]) : 0;
    uint16_t bits = valid ? read_le16(&header[28]) : 0;
    if (!valid || width != SCREEN_W || (height != SCREEN_H && height != -SCREEN_H) ||
        bits != 32) {
        fclose(file);
        ESP_LOGE(TAG, "BMP_UNSUPPORTED %s", path);
        return false;
    }

    uint8_t source[SCREEN_W * 4];
    uint8_t output[SCREEN_W * 2];
    lcd_window(0, 0, SCREEN_W - 1, SCREEN_H - 1);
    for (int y = 0; y < SCREEN_H; ++y) {
        int source_y = height < 0 ? y : (SCREEN_H - 1 - y);
        if (fseek(file, (long)pixel_offset + source_y * (SCREEN_W * 4), SEEK_SET) != 0 ||
            fread(source, 1, sizeof source, file) != sizeof source) {
            fclose(file);
            ESP_LOGE(TAG, "BMP_READ_FAILED %s", path);
            return false;
        }
        for (int x = 0; x < SCREEN_W; ++x) {
            uint8_t blue = source[x * 4];
            uint8_t green = source[x * 4 + 1];
            uint8_t red = source[x * 4 + 2];
            uint16_t pixel = RGB565(red, green, blue);
            output[x * 2] = pixel >> 8;
            output[x * 2 + 1] = pixel & 0xFF;
        }
        lcd_data(output, sizeof output);
    }
    fclose(file);
    ESP_LOGI(TAG, "BMP_DRAWN %s", path);
    return true;
}

static size_t load_media_scene_index(void)
{
    if (!visual_sd_ready) return 0;
    FILE *file = fopen("/visual/SCENES.CSV", "r");
    if (!file) {
        ESP_LOGE(TAG, "MEDIA_INDEX_MISSING");
        return 0;
    }

    char line[128];
    fgets(line, sizeof line, file);  // Header.
    size_t count = 0;
    while (count < MEDIA_SCENE_MAX && fgets(line, sizeof line, file)) {
        unsigned audio_id = 0;
        unsigned duration_ms = 0;
        unsigned lockout_ms = 0;
        unsigned halo_tail_ms = 0;
        media_scene_t candidate = {0};
        int fields = sscanf(line, "%4[^,],%u,%u,%31[^,],%u,%u",
                            candidate.id, &audio_id, &duration_ms,
                            candidate.frames_csv, &lockout_ms, &halo_tail_ms);
        if (fields != 6 || audio_id == 0 || duration_ms == 0) {
            ESP_LOGW(TAG, "MEDIA_INDEX_BAD_LINE %s", line);
            continue;
        }
        candidate.audio_id = (uint16_t)audio_id;
        candidate.duration_ms = duration_ms;
        candidate.lockout_ms = lockout_ms;
        candidate.halo_tail_ms = (uint16_t)halo_tail_ms;
        media_scenes[count++] = candidate;
    }
    fclose(file);
    ESP_LOGI(TAG, "MEDIA_INDEX_READY scenes=%u", (unsigned)count);
    return count;
}

static size_t load_media_frames(const media_scene_t *scene,
                                media_frame_t *frames)
{
    char path[64];
    snprintf(path, sizeof path, "/visual/%s", scene->frames_csv);
    FILE *file = fopen(path, "r");
    if (!file) {
        ESP_LOGE(TAG, "FRAME_INDEX_MISSING %s", path);
        return 0;
    }

    char line[80];
    fgets(line, sizeof line, file);  // Header.
    uint32_t cumulative_ms = 0;
    size_t count = 0;
    while (count < MEDIA_FRAME_MAX && fgets(line, sizeof line, file)) {
        unsigned duration_ms = 0;
        media_frame_t candidate = {0};
        if (sscanf(line, "%12[^,],%12[^,],%u",
                   candidate.bmp, candidate.raw, &duration_ms) != 3 ||
            duration_ms == 0) {
            ESP_LOGW(TAG, "FRAME_INDEX_BAD_LINE %s", line);
            continue;
        }
        cumulative_ms += duration_ms;
        candidate.end_ms = cumulative_ms;
        frames[count++] = candidate;
    }
    fclose(file);
    ESP_LOGI(TAG, "FRAME_INDEX_READY %s frames=%u timeline=%ums",
             scene->id, (unsigned)count, (unsigned)cumulative_ms);
    return count;
}

static bool load_sd_raw(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (!file) return false;

    size_t first_read = fread(raw_frame_buffers[0], 1,
                              RAW_HALF_FRAME_BYTES, file);
    size_t second_read = fread(raw_frame_buffers[1], 1,
                               RAW_HALF_FRAME_BYTES, file);
    int extra = fgetc(file);
    fclose(file);
    if (first_read != RAW_HALF_FRAME_BYTES ||
        second_read != RAW_HALF_FRAME_BYTES || extra != EOF) {
        ESP_LOGE(TAG, "RAW_READ_FAILED %s bytes=%u expected=%u",
                 path, (unsigned)(first_read + second_read),
                 (unsigned)RAW_FRAME_BYTES);
        return false;
    }
    return true;
}

static void draw_raw_buffer(void)
{
    lcd_window(0, 0, SCREEN_W - 1, SCREEN_H - 1);
    for (size_t half = 0; half < 2; ++half) {
        size_t offset = 0;
        while (offset < RAW_HALF_FRAME_BYTES) {
            size_t chunk = RAW_HALF_FRAME_BYTES - offset;
            if (chunk > RAW_TRANSFER_BYTES) chunk = RAW_TRANSFER_BYTES;
            lcd_data(raw_frame_buffers[half] + offset, chunk);
            offset += chunk;
        }
    }
}

static bool draw_sd_raw(const char *path)
{
    if (!load_sd_raw(path)) return false;
    draw_raw_buffer();
    return true;
}

static bool draw_embedded_raw(const embedded_raw_frame_t *frame)
{
    size_t length = frame->length;
    if (length != RAW_FRAME_BYTES) {
        ESP_LOGE(TAG, "MASTER_RAW_BAD_SIZE got=%u expected=%u",
                 (unsigned)length, (unsigned)RAW_FRAME_BYTES);
        return false;
    }

    lcd_window(0, 0, SCREEN_W - 1, SCREEN_H - 1);
    size_t offset = 0;
    while (offset < length) {
        size_t chunk = length - offset;
        if (chunk > RAW_TRANSFER_BYTES) chunk = RAW_TRANSFER_BYTES;
        lcd_data(frame->start + offset, chunk);
        offset += chunk;
    }
    return true;
}

static bool draw_media_frame(const media_scene_t *scene,
                             const media_frame_t *frame)
{
    char path[64];
    snprintf(path, sizeof path, "/visual/SCENES/%s/%s",
             scene->id, frame->raw);
    if (draw_sd_raw(path)) return true;

    // A validated BMP remains a slower emergency fallback for damaged or
    // incomplete RAW copies.
    snprintf(path, sizeof path, "/visual/SCENES/%s/%s",
             scene->id, frame->bmp);
    return draw_sd_bmp(path);
}

static bool load_media_frame_raw(const media_scene_t *scene,
                                 const media_frame_t *frame)
{
    char path[64];
    snprintf(path, sizeof path, "/visual/SCENES/%s/%s",
             scene->id, frame->raw);
    return load_sd_raw(path);
}

static bool qr_interrupt_pending(void)
{
    if (gpio_get_level(PIN_QR_BUTTON) == 0) return true;
    trigger_source_t source;
    while (xQueueReceive(trigger_queue, &source, 0) == pdTRUE) {
        if (source == TRIGGER_PEDAL_RIGHT) return true;
    }
    return false;
}

static bool play_media_scene(const media_scene_t *scene)
{
    media_frame_t frames[MEDIA_FRAME_MAX];
    size_t frame_count = load_media_frames(scene, frames);
    if (!frame_count || !load_media_frame_raw(scene, &frames[0])) {
        ESP_LOGE(TAG, "SCENE_LOAD_FAILED %s", scene->id);
        return false;
    }

    // Nothing is sent to the LCD until all 115,200 bytes are resident in RAM.
    // This replaces the visible top-to-bottom SD-card paint with one short
    // high-speed SPI burst.
    draw_raw_buffer();
    halo_start_scene((uint8_t)(scene - media_scenes));
    dfplayer_play_track(scene->audio_id);

    TickType_t startup = xTaskGetTickCount();
    while ((xTaskGetTickCount() - startup) < pdMS_TO_TICKS(DFPLAYER_STARTUP_MS)) {
        if (qr_interrupt_pending()) {
            dfplayer_stop();
            halo_stop();
            return true;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    TickType_t scene_started = xTaskGetTickCount();
    size_t current_frame = 0;
    while (current_frame + 1 < frame_count) {
        size_t next_frame = current_frame + 1;

        // Read the next complete image while the LCD continues showing the
        // current one. SD latency is therefore hidden inside the hold time.
        if (!load_media_frame_raw(scene, &frames[next_frame])) {
            ESP_LOGE(TAG, "SCENE_PREFETCH_FAILED %s frame=%u",
                     scene->id, (unsigned)(next_frame + 1));
            break;
        }

        uint32_t next_frame_at_ms = frames[current_frame].end_ms;
        while (pdTICKS_TO_MS(xTaskGetTickCount() - scene_started) < next_frame_at_ms) {
            if (qr_interrupt_pending()) {
                dfplayer_stop();
                halo_stop();
                ESP_LOGI(TAG, "SCENE_INTERRUPTED_QR %s", scene->id);
                return true;
            }
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        draw_raw_buffer();
        current_frame = next_frame;
    }

    while (pdTICKS_TO_MS(xTaskGetTickCount() - scene_started) < scene->duration_ms) {
        if (qr_interrupt_pending()) {
            dfplayer_stop();
            halo_stop();
            ESP_LOGI(TAG, "SCENE_INTERRUPTED_QR %s", scene->id);
            return true;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    dfplayer_stop();
    // The Circuit Playground supplies the configured afterglow after this
    // line drops; keeping it high here as well would double the tail.
    halo_stop();
    ESP_LOGI(TAG, "SCENE_COMPLETE %s shown=%u prefetched=yes halo_tail=%ums",
             scene->id, (unsigned)(current_frame + 1),
             (unsigned)scene->halo_tail_ms);
    return false;
}

static bool play_master_scene(void)
{
    const size_t frame_count = sizeof MASTER_SCENE_FRAMES /
                               sizeof MASTER_SCENE_FRAMES[0];
    if (!draw_embedded_raw(&MASTER_SCENE_FRAMES[0])) return false;

    halo_start_scene(SHOES_SCENE_FIRST);  // Embedded fallback is S018.
    // Track 0018 is the exact audio paired with S018. The visuals are fully
    // flash-resident; sound is heard whenever the DFPlayer's Card A is fitted.
    dfplayer_play_track(MASTER_SCENE_AUDIO_ID);

    TickType_t startup = xTaskGetTickCount();
    while ((xTaskGetTickCount() - startup) < pdMS_TO_TICKS(DFPLAYER_STARTUP_MS)) {
        if (qr_interrupt_pending()) {
            dfplayer_stop();
            halo_stop();
            return true;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    TickType_t scene_started = xTaskGetTickCount();
    size_t current_frame = 0;
    while (true) {
        uint32_t elapsed_ms = pdTICKS_TO_MS(xTaskGetTickCount() - scene_started);
        if (elapsed_ms >= MASTER_SCENE_DURATION_MS) break;
        if (qr_interrupt_pending()) {
            dfplayer_stop();
            halo_stop();
            ESP_LOGI(TAG, "MASTER_SCENE_INTERRUPTED_QR");
            return true;
        }

        size_t target_frame = elapsed_ms / 1000;
        if (target_frame >= frame_count) target_frame = frame_count - 1;
        if (target_frame != current_frame) {
            if (!draw_embedded_raw(&MASTER_SCENE_FRAMES[target_frame])) break;
            current_frame = target_frame;
            continue;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    dfplayer_stop();
    halo_stop();
    ESP_LOGI(TAG, "MASTER_SCENE_COMPLETE frames=%u audio=%u",
             (unsigned)frame_count, (unsigned)MASTER_SCENE_AUDIO_ID);
    return false;
}

static void draw_scene(size_t index)
{
    if (visual_sd_ready && index == 0 && draw_sd_bmp("/visual/TEST.BMP")) return;
#if HAS_LOCAL_FACE_ASSET
    play_face_sprite(index);
    return;
#endif
    const scene_t *scene = &SCENES[index];
    fill_rect(0, 0, SCREEN_W, SCREEN_H, scene->background);
    draw_circle(120, 120, 114, scene->foreground);
    draw_text(scene->top, 60, 2, scene->foreground);
    draw_text(scene->middle, 104, 3, scene->foreground);
    draw_text(scene->bottom, 152, 2, scene->foreground);
}

static void request_trigger(trigger_source_t source)
{
    // The right pedal is the QR control and must remain available while a
    // scene is locked out or while the QR code is already being displayed.
    if (trigger_queue && (source == TRIGGER_PEDAL_RIGHT ||
                          source == TRIGGER_VOLUME_UP ||
                          source == TRIGGER_VOLUME_DOWN ||
                          !trigger_locked)) {
        xQueueOverwrite(trigger_queue, &source);
    }
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
                static uint8_t previous_key;
                uint8_t key = param->input.data[2];
                if (key == 0 || key == previous_key) {
                    previous_key = key;
                    break;
                }
                previous_key = key;
                if (key == 0x2C) {
                    ESP_LOGI(TAG, "TRIGGER pedal-left/space");
                    request_trigger(TRIGGER_PEDAL_LEFT);
                } else if (key == 0x28) {
                    ESP_LOGI(TAG, "TRIGGER pedal-right/enter");
                    request_trigger(TRIGGER_PEDAL_RIGHT);
                } else if (key == 0x4B || key == 0x52) {
                    // Mode 1 Page Up (and Mode 3 Up Arrow for convenience).
                    ESP_LOGI(TAG, "TRIGGER volume-up key=0x%02x", key);
                    request_trigger(TRIGGER_VOLUME_UP);
                } else if (key == 0x4E || key == 0x51) {
                    // Mode 1 Page Down (and Mode 3 Down Arrow for convenience).
                    ESP_LOGI(TAG, "TRIGGER volume-down key=0x%02x", key);
                    request_trigger(TRIGGER_VOLUME_DOWN);
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
    if (!pedal_connected && !scan_running) {
        // Reserve the scan slot before creating the task. Setting this inside
        // pedal_scan_task leaves a scheduler race where app_task can create
        // many scanners during consecutive 20 ms passes.
        scan_running = true;
        if (xTaskCreate(pedal_scan_task, "pedal_scan", 6144, NULL, 2, NULL) != pdPASS) {
            scan_running = false;
            ESP_LOGE(TAG, "PEDAL_SCAN_TASK_CREATE_FAILED");
        }
    }
}

static void shuffle_u8(uint8_t *items, size_t count)
{
    for (size_t i = count; i > 1; --i) {
        size_t swap_with = esp_random() % i;
        uint8_t temporary = items[i - 1];
        items[i - 1] = items[swap_with];
        items[swap_with] = temporary;
    }
}

static bool curb_copies_are_spaced(const uint8_t *items, size_t count)
{
    size_t first = count;
    for (size_t i = 0; i < count; ++i) {
        if (items[i] != CURB_SCENE_INDEX) continue;
        if (first == count) first = i;
        else return i > first + 1;
    }
    return false;
}

static void build_playback_order(void)
{
    playback_order_count = 0;
    playback_order_cursor = 0;

    // A partial or damaged card still gets a safe conventional shuffle.
    if (media_scene_count < MEDIA_SCENE_MAX) {
        for (size_t i = 0; i < media_scene_count; ++i) {
            playback_order[playback_order_count++] = (uint8_t)i;
        }
        shuffle_u8(playback_order, playback_order_count);
        ESP_LOGW(TAG, "SCENE_DECK_PARTIAL count=%u",
                 (unsigned)playback_order_count);
        return;
    }

    // S001-S017 are movie/TV/meme or shoe-icon remix scenes. Curb (S003)
    // appears exactly twice; every other scene in this group appears once.
    uint8_t outside_shoes[NON_SHOES_SCENE_COUNT + 1];
    for (size_t i = 0; i < NON_SHOES_SCENE_COUNT; ++i) {
        outside_shoes[i] = (uint8_t)i;
    }
    outside_shoes[NON_SHOES_SCENE_COUNT] = CURB_SCENE_INDEX;
    for (int attempt = 0; attempt < 16; ++attempt) {
        shuffle_u8(outside_shoes, NON_SHOES_SCENE_COUNT + 1);
        if (curb_copies_are_spaced(outside_shoes,
                                   NON_SHOES_SCENE_COUNT + 1)) break;
    }

    // Build enough randomized S018-S027 separators for all 18 outside scenes.
    // A ten-scene shoe deck is exhausted before reshuffling, and the boundary
    // cannot repeat the same Kelly moment.
    uint8_t shoes_sequence[NON_SHOES_SCENE_COUNT + 1];
    size_t shoes_written = 0;
    int previous_shoes = -1;
    while (shoes_written < NON_SHOES_SCENE_COUNT + 1) {
        uint8_t shoes_deck[SHOES_SCENE_COUNT];
        for (size_t i = 0; i < SHOES_SCENE_COUNT; ++i) {
            shoes_deck[i] = (uint8_t)(SHOES_SCENE_FIRST + i);
        }
        shuffle_u8(shoes_deck, SHOES_SCENE_COUNT);
        if (previous_shoes >= 0 && shoes_deck[0] == (uint8_t)previous_shoes) {
            uint8_t temporary = shoes_deck[0];
            shoes_deck[0] = shoes_deck[1];
            shoes_deck[1] = temporary;
        }
        for (size_t i = 0;
             i < SHOES_SCENE_COUNT && shoes_written < NON_SHOES_SCENE_COUNT + 1;
             ++i) {
            shoes_sequence[shoes_written++] = shoes_deck[i];
            previous_shoes = shoes_deck[i];
        }
    }

    for (size_t i = 0; i < NON_SHOES_SCENE_COUNT + 1; ++i) {
        playback_order[playback_order_count++] = outside_shoes[i];
        playback_order[playback_order_count++] = shoes_sequence[i];
    }
    ESP_LOGI(TAG,
             "SCENE_DECK_ALTERNATING plays=%u outside=18 shoes=18 curb=2 first=%s",
             (unsigned)playback_order_count,
             media_scenes[playback_order[0]].id);
}

static size_t next_random_scene_index(void)
{
    if (playback_order_cursor >= playback_order_count) build_playback_order();
    return playback_order[playback_order_cursor++];
}

static void app_task(void *unused)
{
    bool button_was_down = false;
    bool qr_button_was_down = false;
    bool qr_mode = false;
    bool volume_overlay = false;
    TickType_t volume_overlay_until = 0;
    bool lockout_active = false;
    TickType_t lockout_started = 0;
    uint32_t lockout_ms = TRIGGER_LOCKOUT_MS;
    TickType_t qr_last_toggled = 0;
    draw_idle();

    while (true) {
        bool qr_toggle_requested = false;
        bool qr_button_down = gpio_get_level(PIN_QR_BUTTON) == 0;
        if (qr_button_down && !qr_button_was_down &&
            (xTaskGetTickCount() - qr_last_toggled) >= pdMS_TO_TICKS(250)) {
            qr_toggle_requested = true;
        }
        qr_button_was_down = qr_button_down;

        bool button_down = gpio_get_level(PIN_TRIGGER_BUTTON) == 0;
        if (!qr_mode && button_down && !button_was_down) request_trigger(TRIGGER_LOCAL_BUTTON);
        button_was_down = button_down;

        trigger_source_t source;
        bool scene_requested = false;
        if (xQueueReceive(trigger_queue, &source, 0) == pdTRUE) {
            if (source == TRIGGER_VOLUME_UP || source == TRIGGER_VOLUME_DOWN) {
                volume_adjust(source == TRIGGER_VOLUME_UP ? 1 : -1);
                volume_overlay = true;
                volume_overlay_until = xTaskGetTickCount() +
                                       pdMS_TO_TICKS(VOLUME_OVERLAY_MS);
            } else if (source == TRIGGER_PEDAL_RIGHT) {
                qr_toggle_requested = true;
            } else if (!qr_mode && !lockout_active) {
                scene_requested = true;
            }
        }

        if (qr_toggle_requested &&
            (xTaskGetTickCount() - qr_last_toggled) >= pdMS_TO_TICKS(250)) {
            qr_last_toggled = xTaskGetTickCount();
            qr_mode = !qr_mode;
            volume_overlay = false;
            dfplayer_stop();
            halo_stop();
            xQueueReset(trigger_queue);
            trigger_locked = qr_mode || lockout_active;
            if (qr_mode) draw_qr_code();
            else draw_idle();
        }

        if (scene_requested) {
            volume_overlay = false;
            lockout_active = true;
            trigger_locked = true;
            lockout_started = xTaskGetTickCount();
            bool qr_interrupted = false;

            if (media_scene_count) {
                size_t scene_index = next_random_scene_index();
                const media_scene_t *scene = &media_scenes[scene_index];
                lockout_ms = scene->lockout_ms;
                ESP_LOGI(TAG, "SCENE_%s audio=%u source=%d deck=%u/%u",
                         scene->id, (unsigned)scene->audio_id, source,
                         (unsigned)playback_order_cursor,
                         (unsigned)playback_order_count);
                qr_interrupted = play_media_scene(scene);
            } else {
                // With no visual card, play one coherent six-frame master
                // scene from flash. Its paired S018 audio is track 0018 on
                // the DFPlayer card instead of the old, incorrect track 1.
                lockout_ms = TRIGGER_LOCKOUT_MS;
                ESP_LOGI(TAG, "MASTER_SCENE_S018 source=%d", source);
                qr_interrupted = play_master_scene();
            }

            if (qr_interrupted) {
                qr_mode = true;
                qr_button_was_down = gpio_get_level(PIN_QR_BUTTON) == 0;
                qr_last_toggled = xTaskGetTickCount();
                draw_qr_code();
            } else {
                draw_idle();
            }
            trigger_locked = qr_mode || lockout_active;
        }

        if (volume_overlay &&
            (int32_t)(xTaskGetTickCount() - volume_overlay_until) >= 0) {
            volume_overlay = false;
            if (qr_mode) draw_qr_code();
            else draw_idle();
        }

        if (lockout_active &&
            (xTaskGetTickCount() - lockout_started) >= pdMS_TO_TICKS(lockout_ms)) {
            lockout_active = false;
            xQueueReset(trigger_queue);
            trigger_locked = qr_mode;
            ESP_LOGI(TAG, "TRIGGER_LOCKOUT_COMPLETE");
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
    volume_load();

    // Two half-frame allocations fit the ESP32's fragmented internal DMA
    // regions; one contiguous 115,200-byte allocation does not. Both halves
    // are filled before drawing, so SD reads remain invisible on the LCD.
    for (size_t half = 0; half < 2; ++half) {
        raw_frame_buffers[half] = heap_caps_malloc(
            RAW_HALF_FRAME_BYTES, MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
    }
    if (!raw_frame_buffers[0] || !raw_frame_buffers[1]) {
        ESP_LOGE(TAG, "FRAMEBUFFER_ALLOC_FAILED halves=%p,%p bytes_each=%u",
                 raw_frame_buffers[0], raw_frame_buffers[1],
                 (unsigned)RAW_HALF_FRAME_BYTES);
        ESP_ERROR_CHECK(ESP_ERR_NO_MEM);
    }
    ESP_LOGI(TAG, "FRAMEBUFFER_READY split=2 bytes=%u free_heap=%u",
             (unsigned)RAW_FRAME_BYTES,
             (unsigned)heap_caps_get_free_size(MALLOC_CAP_8BIT));

    gpio_config_t button = {
        .pin_bit_mask = (1ULL << PIN_TRIGGER_BUTTON) | (1ULL << PIN_QR_BUTTON),
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
    trigger_queue = xQueueCreate(1, sizeof(trigger_source_t));
    lcd_init();
    visual_sd_init();
    media_scene_count = load_media_scene_index();
    dfplayer_init();

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
