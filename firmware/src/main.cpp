#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GC9A01A.h>
#include <Adafruit_GFX.h>

#include "config.h"

namespace {

Adafruit_GC9A01A display(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);

enum class AppState {
  Idle,
  PlayingTestScene,
};

struct TestScene {
  const char* top;
  const char* middle;
  const char* bottom;
  uint16_t background;
  uint16_t foreground;
};

constexpr uint16_t COLOR_NAVY = 0x000F;
constexpr uint16_t COLOR_PURPLE = 0x780F;
constexpr uint16_t COLOR_DARK_GREEN = 0x03E0;

constexpr TestScene TEST_SCENES[] = {
    {"TAKE OFF", "THE", "SHOES", GC9A01A_RED, GC9A01A_WHITE},
    {"FOOTWEAR", "DETECTED", "!", COLOR_NAVY, GC9A01A_YELLOW},
    {"BOLD", "SHOE", "CHOICE", COLOR_PURPLE, GC9A01A_WHITE},
    {"CHOOSE", "BAREFOOT", "PEACE", COLOR_DARK_GREEN, GC9A01A_WHITE},
};

constexpr size_t TEST_SCENE_COUNT = sizeof(TEST_SCENES) / sizeof(TEST_SCENES[0]);

AppState state = AppState::Idle;
size_t nextSceneIndex = 0;
unsigned long sceneStartedAt = 0;

bool lastRawTrigger = HIGH;
bool stableTrigger = HIGH;
unsigned long triggerChangedAt = 0;

void drawCenteredLine(const char* text, int16_t y, uint8_t size, uint16_t color) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.setTextSize(size);
  display.setTextColor(color);
  display.getTextBounds(text, 0, y, &x1, &y1, &width, &height);
  display.setCursor((display.width() - width) / 2, y);
  display.print(text);
}

void drawIdle() {
  display.fillScreen(GC9A01A_BLACK);
  display.drawCircle(120, 120, 112, GC9A01A_CYAN);
  display.drawCircle(120, 120, 108, GC9A01A_BLUE);
  drawCenteredLine("SHOES", 58, 3, GC9A01A_WHITE);
  drawCenteredLine("CHAOS", 96, 3, GC9A01A_CYAN);
  drawCenteredLine("BOX", 134, 3, GC9A01A_WHITE);
  drawCenteredLine("PRESS BUTTON", 188, 1, GC9A01A_YELLOW);
}

void startTestScene(size_t sceneIndex) {
  const TestScene& scene = TEST_SCENES[sceneIndex];

  display.fillScreen(scene.background);
  display.drawCircle(120, 120, 114, scene.foreground);
  drawCenteredLine(scene.top, 60, 2, scene.foreground);
  drawCenteredLine(scene.middle, 104, 3, scene.foreground);
  drawCenteredLine(scene.bottom, 152, 2, scene.foreground);

  Serial.printf("TEST_SCENE_%02u\n", static_cast<unsigned int>(sceneIndex + 1));
  sceneStartedAt = millis();
  state = AppState::PlayingTestScene;
}

bool triggerWasPressed() {
  const bool rawTrigger = digitalRead(PIN_TRIGGER_BUTTON);
  const unsigned long now = millis();

  if (rawTrigger != lastRawTrigger) {
    lastRawTrigger = rawTrigger;
    triggerChangedAt = now;
  }

  if ((now - triggerChangedAt) >= BUTTON_DEBOUNCE_MS && rawTrigger != stableTrigger) {
    stableTrigger = rawTrigger;
    return stableTrigger == LOW;
  }

  return false;
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.println("Shoes Chaos Box display proof starting");

  pinMode(PIN_TRIGGER_BUTTON, INPUT_PULLUP);
  pinMode(PIN_MUTE_BUTTON, INPUT_PULLUP);
  pinMode(PIN_TFT_BACKLIGHT, OUTPUT);
  digitalWrite(PIN_TFT_BACKLIGHT, HIGH);

  SPI.begin(PIN_SPI_SCLK, PIN_SPI_MISO, PIN_SPI_MOSI);
  display.begin();
  display.setRotation(0);
  display.setTextWrap(false);

  drawIdle();
  Serial.println("READY");
}

void loop() {
  const unsigned long now = millis();

  if (state == AppState::Idle && triggerWasPressed()) {
    startTestScene(nextSceneIndex);
    nextSceneIndex = (nextSceneIndex + 1) % TEST_SCENE_COUNT;
  }

  if (state == AppState::PlayingTestScene &&
      (now - sceneStartedAt) >= TEST_SCENE_DURATION_MS) {
    drawIdle();
    state = AppState::Idle;
    Serial.println("READY");
  }
}
