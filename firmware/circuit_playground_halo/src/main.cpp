#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>

namespace {

constexpr uint8_t PIN_SCENE_TRIGGER = A1;
constexpr uint8_t PIXEL_COUNT = 10;
constexpr uint8_t IDLE_BRIGHTNESS = 12;
constexpr uint8_t EFFECT_BRIGHTNESS = 48;

bool previousTrigger = false;

void clearPixels() {
  for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
    CircuitPlayground.setPixelColor(pixel, 0, 0, 0);
  }
}

void drawIdle() {
  CircuitPlayground.setBrightness(IDLE_BRIGHTNESS);
  clearPixels();
  CircuitPlayground.setPixelColor(0, 0, 120, 255);
  CircuitPlayground.setPixelColor(5, 0, 120, 255);
  CircuitPlayground.strip.show();
}

void playHaloChase() {
  CircuitPlayground.setBrightness(EFFECT_BRIGHTNESS);

  for (uint8_t step = 0; step < 20; ++step) {
    clearPixels();
    const uint8_t head = step % PIXEL_COUNT;
    const uint8_t tail = (head + PIXEL_COUNT - 1) % PIXEL_COUNT;
    CircuitPlayground.setPixelColor(head, 255, 35, 0);
    CircuitPlayground.setPixelColor(tail, 80, 0, 255);
    CircuitPlayground.strip.show();
    delay(45);
  }

  for (int brightness = EFFECT_BRIGHTNESS; brightness >= 0; brightness -= 4) {
    CircuitPlayground.setBrightness(brightness);
    for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
      CircuitPlayground.setPixelColor(pixel, 0, 120, 255);
    }
    CircuitPlayground.strip.show();
    delay(25);
  }

  drawIdle();
}

}  // namespace

void setup() {
  Serial.begin(115200);
  CircuitPlayground.begin();
  pinMode(PIN_SCENE_TRIGGER, INPUT_PULLDOWN);
  drawIdle();
  Serial.println("HALO_READY A1 trigger");
}

void loop() {
  const bool trigger = digitalRead(PIN_SCENE_TRIGGER) == HIGH;
  if (trigger && !previousTrigger) {
    Serial.println("HALO_TRIGGER");
    playHaloChase();
  }
  previousTrigger = trigger;
  delay(5);
}
