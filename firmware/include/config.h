#pragma once

// Display SPI pins are grouped on the exposed A-side of the breadboard.
// The GC9A01 is write-only here, so MISO is intentionally unused.
constexpr int PIN_SPI_SCLK = 14;
constexpr int PIN_SPI_MISO = -1;
constexpr int PIN_SPI_MOSI = 27;

// GC9A01 round TFT.
constexpr int PIN_TFT_CS = 26;
constexpr int PIN_TFT_DC = 25;
constexpr int PIN_TFT_RST = 33;
constexpr int PIN_TFT_BACKLIGHT = 32;

// Local controls. Each button connects its GPIO pin to GND when pressed.
constexpr int PIN_TRIGGER_BUTTON = 13;
constexpr int PIN_MUTE_BUTTON = 34;

// Reserved for later phases.
constexpr int PIN_VISUAL_SD_CS = 5;
constexpr int PIN_DFPLAYER_RX = 16;
constexpr int PIN_DFPLAYER_TX = 17;

constexpr unsigned long BUTTON_DEBOUNCE_MS = 35;
constexpr unsigned long TEST_SCENE_DURATION_MS = 2200;
