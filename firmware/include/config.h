#pragma once

// Shared SPI bus. The visual microSD module will use these same three pins.
constexpr int PIN_SPI_SCLK = 18;
constexpr int PIN_SPI_MISO = 19;
constexpr int PIN_SPI_MOSI = 23;

// GC9A01 round TFT.
constexpr int PIN_TFT_CS = 27;
constexpr int PIN_TFT_DC = 25;
constexpr int PIN_TFT_RST = 33;
constexpr int PIN_TFT_BACKLIGHT = 32;

// Local controls. Each button connects its GPIO pin to GND when pressed.
constexpr int PIN_TRIGGER_BUTTON = 13;
constexpr int PIN_MUTE_BUTTON = 14;

// Reserved for later phases.
constexpr int PIN_VISUAL_SD_CS = 26;
constexpr int PIN_DFPLAYER_RX = 16;
constexpr int PIN_DFPLAYER_TX = 17;

constexpr unsigned long BUTTON_DEBOUNCE_MS = 35;
constexpr unsigned long TEST_SCENE_DURATION_MS = 2200;

