#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>

namespace {

constexpr uint8_t PIN_SCENE_TRIGGER = A1;
constexpr uint8_t PIXEL_COUNT = 10;
constexpr uint8_t SCENE_COUNT = 27;
constexpr uint8_t IDLE_BRIGHTNESS = 12;
constexpr uint8_t EFFECT_BRIGHTNESS = 48;
constexpr uint8_t SONG_BRIGHTNESS = 88;
constexpr uint16_t AFTERGLOW_MS = 2000;
constexpr uint16_t CALM_FRAME_INTERVAL_MS = 90;
constexpr uint16_t SONG_FRAME_INTERVAL_MS = 42;
constexpr uint16_t SYNC_MIN_MS = 45;
constexpr uint16_t SYNC_MAX_MS = 85;
constexpr uint16_t BIT_ONE_THRESHOLD_MS = 19;

struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

// Palettes S001-S027. They follow the dominant colors of the actual circular
// scene exports rather than rotating through unrelated generic colors.
constexpr Color SCENE_PALETTES[SCENE_COUNT][3] = {
    {{236, 244, 255}, {160, 205, 255}, {112, 112, 112}},  // S001 Damn Daniel
    {{249,  56,  84}, {255, 208,  91}, {255, 255, 255}},  // S002 Wizard of Oz
    {{255, 208,  91}, {144, 112,  80}, {255, 244, 210}},  // S003 Curb
    {{ 87, 170, 255}, {255, 255, 255}, {255, 195,  67}},  // S004 Austin Powers
    {{177, 156, 255}, {144,  80, 112}, {255, 255, 255}},  // S005 Silence/Lambs
    {{140, 224, 180}, { 40, 112,  72}, {255, 255, 255}},  // S006 Forrest Gump
    {{173, 198, 213}, { 72,  96, 112}, {255, 255, 255}},  // S007 Shawshank
    {{255, 195,  67}, { 48,  80, 112}, {220,  64,  48}},  // S008 Toy Story
    {{112, 237, 255}, {255,  71, 170}, {255, 255, 255}},  // S009 Back/Future
    {{237, 237, 255}, {176, 176, 192}, { 48,  96, 180}},  // S010 Michael Jackson
    {{255,  84,  84}, {255, 255, 255}, { 32,  32,  32}},  // S011 Michael Jordan
    {{156, 220, 255}, {208, 112, 144}, {255, 255, 255}},  // S012 Cinderella
    {{226, 101, 103}, {255, 232, 192}, { 72, 112, 176}},  // S013 Mister Rogers
    {{255, 124, 213}, {255, 190, 230}, {255, 255, 255}},  // S014 Barbie
    {{127, 228, 179}, { 32, 128,  80}, {255, 255, 255}},  // S015 Get Smart
    {{255, 235, 100}, { 80, 176, 208}, {112,  72,  32}},  // S016 SpongeBob
    {{238, 231, 213}, {176, 144,  96}, {255, 208,  91}},  // S017 Chaplin
    {{255,  71, 170}, {255, 255, 255}, {224, 112,  48}},  // S018 Oh my God
    {{255,  71, 170}, {255, 255, 255}, {240, 144, 112}},  // S019 Get some shoes
    {{255, 144, 112}, {255,  71, 170}, {255, 208,  91}},  // S020 Rule/suck
    {{255, 176, 112}, {112,  80,  48}, {255,  71, 170}},  // S021 Shoes suck
    {{255,  71, 170}, {255, 255, 255}, {112,  80,  48}},  // S022 Too many shoes
    {{240,  16, 240}, { 16, 144, 240}, {255, 176, 240}},  // S023 Let's party
    {{255, 240, 176}, {208, 176, 144}, {255, 208, 112}},  // S024 $300
    {{255, 144,  80}, {255, 240, 112}, {208,  80,  48}},  // S025 Let's get 'em
    {{255, 240, 144}, {255, 176, 112}, {112,  80,  48}},  // S026 Runs small
    {{255, 255, 255}, {208, 240,  16}, {255,  71, 170}},  // S027 Those are mine
};

enum class DecodeState : uint8_t {
  WaitForSync,
  ReadBits,
  Armed,
  Active,
};

DecodeState decodeState = DecodeState::WaitForSync;
bool previousInput = false;
bool sceneActive = false;
bool effectVisible = false;
uint8_t scenePalette = 17;
uint8_t decodedScene = 0;
uint8_t decodedBitCount = 0;
uint8_t animationStep = 0;
uint32_t pulseStartedAt = 0;
uint32_t lastFrameAt = 0;
uint32_t afterglowUntil = 0;

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

bool isShoesSongScene() {
  return scenePalette >= 17;  // S018-S027 are excerpts from the Shoes video.
}

Color scaleColor(const Color &color, uint8_t amount) {
  return {
      static_cast<uint8_t>((static_cast<uint16_t>(color.red) * amount) / 255),
      static_cast<uint8_t>((static_cast<uint16_t>(color.green) * amount) / 255),
      static_cast<uint8_t>((static_cast<uint16_t>(color.blue) * amount) / 255),
  };
}

Color blendColor(const Color &from, const Color &to, uint8_t amount) {
  const uint16_t inverse = 255 - amount;
  return {
      static_cast<uint8_t>((from.red * inverse + to.red * amount) / 255),
      static_cast<uint8_t>((from.green * inverse + to.green * amount) / 255),
      static_cast<uint8_t>((from.blue * inverse + to.blue * amount) / 255),
  };
}

void fillPixels(const Color &color) {
  for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
    CircuitPlayground.setPixelColor(pixel, color.red, color.green, color.blue);
  }
}

void drawCalmSceneFrame(uint8_t brightness) {
  CircuitPlayground.setBrightness(brightness);
  for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
    const Color color = SCENE_PALETTES[scenePalette]
                                      [(pixel + animationStep) % 3];
    CircuitPlayground.setPixelColor(pixel, color.red, color.green, color.blue);
  }
  CircuitPlayground.strip.show();
  animationStep = (animationStep + 1) % 30;
}

void drawSongSceneFrame(uint8_t brightness) {
  // Give each Shoes excerpt its own coherent lighting identity. Across the ten
  // excerpts the show includes chase, blink, fade, roll, strobe, and hard-cut
  // treatments, without changing visual language halfway through one clip.
  constexpr Color neonGreen = {16, 255, 72};
  constexpr Color neonMagenta = {255, 0, 190};
  constexpr Color electricBlue = {0, 96, 255};
  const uint8_t step = animationStep % 120;
  const uint8_t profile = scenePalette - 17;
  clearPixels();
  CircuitPlayground.setBrightness(brightness);

  if (profile == 0) {
    // S018: a wide green wedge chases clockwise and fades at its edges.
    const uint8_t runner = (step / 2) % PIXEL_COUNT;
    for (uint8_t width = 0; width < 5; ++width) {
      const uint8_t levels[5] = {70, 170, 255, 170, 70};
      const Color color = scaleColor(neonGreen, levels[width]);
      const uint8_t pixel = (runner + width) % PIXEL_COUNT;
      CircuitPlayground.setPixelColor(pixel, color.red, color.green, color.blue);
    }
  } else if (profile == 1) {
    // S019: forceful magenta blink that falls away between hits.
    const uint8_t beat = step % 12;
    uint8_t level = 0;
    if (beat < 3) level = 255;
    else if (beat < 8) level = static_cast<uint8_t>(220 - (beat - 3) * 42);
    fillPixels(scaleColor(neonMagenta, level));
  } else if (profile == 2) {
    // S020: clean blue and magenta half-rings rotate around each other.
    const uint8_t shift = (step / 2) % PIXEL_COUNT;
    for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
      const Color color = ((pixel + shift) % PIXEL_COUNT < 5) ? electricBlue
                                                               : neonMagenta;
      CircuitPlayground.setPixelColor(pixel, color.red, color.green,
                                      color.blue);
    }
  } else if (profile == 3) {
    // S021: two broad scene-color halves breathe and slowly trade places.
    const uint8_t breath = step % 40;
    const uint8_t triangle = breath < 20 ? breath : 39 - breath;
    const uint8_t level = static_cast<uint8_t>(55 + triangle * 10);
    const uint8_t shift = (step / 8) % PIXEL_COUNT;
    for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
      const uint8_t index = ((pixel + shift) % PIXEL_COUNT < 5) ? 0 : 2;
      const Color color = scaleColor(SCENE_PALETTES[scenePalette][index], level);
      CircuitPlayground.setPixelColor(pixel, color.red, color.green, color.blue);
    }
  } else if (profile == 4) {
    // S022: rotating green/blue halves punctuated by a full magenta hit.
    const uint8_t beat = step % 24;
    if (beat < 3) {
      fillPixels(neonMagenta);
    } else {
      const uint8_t shift = (step / 3) % PIXEL_COUNT;
      for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
        const Color color = ((pixel + shift) % PIXEL_COUNT < 5) ? neonGreen
                                                                 : electricBlue;
        CircuitPlayground.setPixelColor(pixel, color.red, color.green, color.blue);
      }
    }
  } else if (profile == 5) {
    // S023 "Let's party": rolling neon colors with brief white strobes.
    for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
      const uint8_t roll = (pixel * 25 + step * 13) % 255;
      Color color;
      if (roll < 85) color = blendColor(neonGreen, neonMagenta, roll * 3);
      else if (roll < 170) {
        color = blendColor(neonMagenta, electricBlue, (roll - 85) * 3);
      } else color = blendColor(electricBlue, neonGreen, (roll - 170) * 3);
      CircuitPlayground.setPixelColor(pixel, color.red, color.green, color.blue);
    }
    if (step % 20 < 2) fillPixels({255, 255, 255});
  } else if (profile == 6) {
    // S024: a wide warm-gold wedge circles through darkness.
    const Color gold = SCENE_PALETTES[scenePalette][2];
    const uint8_t runner = (step / 3) % PIXEL_COUNT;
    for (uint8_t width = 0; width < 5; ++width) {
      const uint8_t levels[5] = {65, 155, 255, 155, 65};
      const Color color = scaleColor(gold, levels[width]);
      const uint8_t pixel = (runner + width) % PIXEL_COUNT;
      CircuitPlayground.setPixelColor(pixel, color.red, color.green, color.blue);
    }
  } else if (profile == 7) {
    // S025: unapologetic rapid full-ring color cuts with black punctuation.
    const uint8_t cut = (step / 3) % 7;
    if (cut == 0) fillPixels(neonMagenta);
    else if (cut == 1) fillPixels(SCENE_PALETTES[scenePalette][0]);
    else if (cut == 2) fillPixels(electricBlue);
    else if (cut == 3) fillPixels(SCENE_PALETTES[scenePalette][1]);
    else if (cut == 4) fillPixels(neonGreen);
    else if (cut == 5) fillPixels(SCENE_PALETTES[scenePalette][2]);
  } else if (profile == 8) {
    // S026: the whole halo smoothly rolls through its three scene colors.
    const uint8_t phase = step % 60;
    Color color;
    if (phase < 20) {
      color = blendColor(SCENE_PALETTES[scenePalette][0],
                         SCENE_PALETTES[scenePalette][1], phase * 13);
    } else if (phase < 40) {
      color = blendColor(SCENE_PALETTES[scenePalette][1],
                         SCENE_PALETTES[scenePalette][2], (phase - 20) * 13);
    } else {
      color = blendColor(SCENE_PALETTES[scenePalette][2],
                         SCENE_PALETTES[scenePalette][0], (phase - 40) * 13);
    }
    fillPixels(color);
  } else {
    // S027: rotating lime/magenta halves with synchronized full-ring blinks.
    const uint8_t beat = step % 30;
    if (beat < 4) {
      fillPixels(beat < 2 ? neonMagenta : neonGreen);
    } else {
      const uint8_t shift = (step / 2) % PIXEL_COUNT;
      for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
        const Color color = ((pixel + shift) % PIXEL_COUNT < 5) ? neonMagenta
                                                                 : neonGreen;
        CircuitPlayground.setPixelColor(pixel, color.red, color.green,
                                        color.blue);
      }
    }
  }

  CircuitPlayground.strip.show();
  animationStep = (animationStep + 1) % 120;
}

void drawSceneFrame(uint8_t brightness) {
  if (isShoesSongScene()) drawSongSceneFrame(brightness);
  else drawCalmSceneFrame(brightness);
}

void beginScene(uint32_t now) {
  scenePalette = decodedScene < SCENE_COUNT ? decodedScene : 17;
  sceneActive = true;
  effectVisible = true;
  animationStep = 0;
  lastFrameAt = 0;
  decodeState = DecodeState::Active;
  Serial.print("HALO_SCENE S");
  if (scenePalette + 1 < 10) Serial.print("00");
  else if (scenePalette + 1 < 100) Serial.print('0');
  Serial.println(scenePalette + 1);
  drawSceneFrame(isShoesSongScene() ? SONG_BRIGHTNESS : EFFECT_BRIGHTNESS);
  lastFrameAt = now;
}

void handleFallingEdge(uint32_t now) {
  const uint32_t pulseWidth = now - pulseStartedAt;
  if (decodeState == DecodeState::Active) {
    sceneActive = false;
    afterglowUntil = now + AFTERGLOW_MS;
    decodeState = DecodeState::WaitForSync;
    return;
  }

  if (decodeState == DecodeState::WaitForSync) {
    if (pulseWidth >= SYNC_MIN_MS && pulseWidth <= SYNC_MAX_MS) {
      decodedScene = 0;
      decodedBitCount = 0;
      decodeState = DecodeState::ReadBits;
    }
    return;
  }

  if (decodeState == DecodeState::ReadBits) {
    if (pulseWidth >= BIT_ONE_THRESHOLD_MS) {
      decodedScene |= static_cast<uint8_t>(1U << decodedBitCount);
    }
    ++decodedBitCount;
    if (decodedBitCount == 5) decodeState = DecodeState::Armed;
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  CircuitPlayground.begin();
  pinMode(PIN_SCENE_TRIGGER, INPUT_PULLDOWN);
  drawIdle();
  Serial.println("HALO_READY A1 scene-code v2");
}

void loop() {
  const uint32_t now = millis();
  const bool inputHigh = digitalRead(PIN_SCENE_TRIGGER) == HIGH;

  if (inputHigh != previousInput) {
    if (inputHigh) {
      pulseStartedAt = now;
      if (decodeState == DecodeState::Armed) beginScene(now);
    } else {
      handleFallingEdge(now);
    }
    previousInput = inputHigh;
  }

  const int32_t afterglowRemaining = static_cast<int32_t>(afterglowUntil - now);
  const uint16_t frameInterval = isShoesSongScene()
                                     ? SONG_FRAME_INTERVAL_MS
                                     : CALM_FRAME_INTERVAL_MS;
  if ((sceneActive || afterglowRemaining > 0) &&
      now - lastFrameAt >= frameInterval) {
    const uint8_t sceneBrightness = isShoesSongScene()
                                        ? SONG_BRIGHTNESS
                                        : EFFECT_BRIGHTNESS;
    uint8_t brightness = sceneBrightness;
    if (!sceneActive) {
      brightness = static_cast<uint8_t>(
          (sceneBrightness * afterglowRemaining) / AFTERGLOW_MS);
    }
    drawSceneFrame(brightness);
    lastFrameAt = now;
  } else if (!sceneActive && afterglowRemaining <= 0 && effectVisible) {
    drawIdle();
    effectVisible = false;
  }
  delay(1);
}
