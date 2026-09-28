#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>

namespace {

constexpr uint8_t PIN_SCENE_TRIGGER = A1;
constexpr uint8_t PIXEL_COUNT = 10;
constexpr uint8_t SCENE_COUNT = 32;
constexpr uint8_t IDLE_BRIGHTNESS = 12;
constexpr uint8_t EFFECT_BRIGHTNESS = 48;
constexpr uint8_t SONG_BRIGHTNESS = 88;
constexpr uint16_t AFTERGLOW_MS = 2000;
// Effect tempos retained from the pre-repair firmware; rendering has its own
// faster cadence below so a missed render does not slow the animation clock.
constexpr uint16_t CALM_FRAME_INTERVAL_MS = 45;
constexpr uint16_t SONG_FRAME_INTERVAL_MS = 25;
constexpr uint16_t SYNC_MIN_MS = 45;
constexpr uint16_t SYNC_MAX_MS = 85;
constexpr uint16_t ARC_SYNC_MIN_MS = 105;
constexpr uint16_t ARC_SYNC_MAX_MS = 145;
constexpr uint16_t BIT_ONE_THRESHOLD_MS = 19;
constexpr uint32_t ARC_STARTUP_MS = 5040;
constexpr uint32_t ARC_PULSE_MS = 30000;
constexpr uint32_t ARC_CYCLE_MS = ARC_STARTUP_MS + ARC_PULSE_MS;
constexpr uint16_t ARC_BOOT_RECOVERY_MS = 750;
constexpr uint16_t STATUS_INTERVAL_MS = 5000;

struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

// Palettes S001-S032. They follow the dominant colors of the actual circular
// scene exports rather than rotating through unrelated generic colors.
constexpr Color SCENE_PALETTES[SCENE_COUNT][3] = {
    {{236, 244, 255}, {160, 205, 255}, {112, 112, 112}},  // S001 Damn Daniel
    {{224,   0,  48}, {  0, 170,  80}, {  0,  70,  36}},  // S002 Wizard of Oz
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
    {{255, 225,  40}, { 72, 190, 255}, {210, 245, 255}},  // S016 SpongeBob
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
    {{160, 205, 255}, {255, 255, 255}, {112, 112, 112}},  // S028 Larry refuses
    {{255, 208,  91}, {160, 205, 255}, {255, 255, 255}},  // S029 Larry gets chilly
    {{246, 211,  45}, {  0, 213, 245}, {238,   0, 168}},  // S030 Kling Curb
    {{  0, 213, 245}, {238,   0, 168}, {201, 255,   0}},  // S031 Kling Kelly
    {{164, 255,   0}, {238,   0, 168}, {  0, 213, 245}},  // S032 bacteria rave
};

enum class DecodeState : uint8_t {
  WaitForSync,
  ReadBits,
  Armed,
  ArcArmed,
  Active,
};

DecodeState decodeState = DecodeState::WaitForSync;
bool previousInput = false;
bool sceneActive = false;
bool arcCoreActive = false;
bool effectVisible = false;
bool bootArcRecoveryPending = false;
uint8_t scenePalette = 17;
uint8_t decodedScene = 0;
uint8_t decodedBitCount = 0;
// Phrase timing starts at each scene; spatial rotation survives scene changes.
uint32_t animationStep = 0;
uint32_t sceneStartedAt = 0;
uint32_t rotationUpdatedAt = 0;
uint32_t rotationQ16 = 0;
Color framePixels[PIXEL_COUNT] = {};

void setFramePixel(uint8_t pixel, uint8_t red, uint8_t green, uint8_t blue) {
  if (pixel < PIXEL_COUNT) framePixels[pixel] = {red, green, blue};
}

void presentFrame(bool rotate);
uint32_t pulseStartedAt = 0;
uint32_t lastFrameAt = 0;
uint32_t afterglowUntil = 0;
uint32_t bootHighStartedAt = 0;
uint32_t lastStatusAt = 0;

void clearPixels() {
  for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
    setFramePixel(pixel, 0, 0, 0);
  }
}

void drawIdle() {
  CircuitPlayground.setBrightness(IDLE_BRIGHTNESS);
  clearPixels();
  setFramePixel(0, 0, 120, 255);
  setFramePixel(5, 0, 120, 255);
  presentFrame(false);

}

bool isShoesSongScene() {
  return (scenePalette >= 17 && scenePalette <= 26) ||
         scenePalette == 30 || scenePalette == 31;
}

bool isKlingScene() {
  return scenePalette >= 29 && scenePalette <= 31;
}

bool isWizardOfOzScene() {
  return scenePalette == 1;
}

bool isSpongeBobScene() {
  return scenePalette == 15;
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
    setFramePixel(pixel, color.red, color.green, color.blue);
  }
}

void drawCalmSceneFrame(uint8_t brightness) {
  CircuitPlayground.setBrightness(brightness);
  for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
    const Color color = SCENE_PALETTES[scenePalette]
                                      [pixel % 3];
    setFramePixel(pixel, color.red, color.green, color.blue);
  }

}

void drawWizardOfOzFrame(uint8_t brightness) {
  // Emerald City glow under a ruby-slipper highlight. Spatial movement is
  // applied once in presentFrame using elapsed time.
  const Color ruby = SCENE_PALETTES[scenePalette][0];
  const Color emerald = SCENE_PALETTES[scenePalette][1];
  CircuitPlayground.setBrightness(brightness);
  fillPixels(scaleColor(emerald, 92));

  const uint8_t runner = 0;
  const Color rubyTail = scaleColor(ruby, 70);
  const Color rubyGlow = scaleColor(ruby, 150);
  setFramePixel((runner + PIXEL_COUNT - 1) % PIXEL_COUNT,
                                  rubyTail.red, rubyTail.green, rubyTail.blue);
  setFramePixel(runner, ruby.red, ruby.green, ruby.blue);
  setFramePixel((runner + 1) % PIXEL_COUNT,
                                  rubyGlow.red, rubyGlow.green, rubyGlow.blue);

}

void drawSpongeBobFrame(uint8_t brightness) {
  // Ocean-blue field with two drifting SpongeBob-yellow bubble highlights.
  // Keeping most of the ring blue prevents the diffuser from blending the two
  // colors into an indistinct green wash.
  const Color yellow = SCENE_PALETTES[scenePalette][0];
  const Color water = SCENE_PALETTES[scenePalette][1];
  CircuitPlayground.setBrightness(brightness);
  fillPixels(scaleColor(water, 125));

  const uint8_t first = 0;
  const uint8_t second = (first + 5) % PIXEL_COUNT;
  const Color yellowGlow = scaleColor(yellow, 120);
  setFramePixel(first, yellow.red, yellow.green, yellow.blue);
  setFramePixel((first + 1) % PIXEL_COUNT,
                                  yellowGlow.red, yellowGlow.green,
                                  yellowGlow.blue);
  setFramePixel(second, yellow.red, yellow.green, yellow.blue);
  setFramePixel((second + 1) % PIXEL_COUNT,
                                  yellowGlow.red, yellowGlow.green,
                                  yellowGlow.blue);

}

void drawSongSceneFrame(uint8_t brightness) {
  // Give each Shoes excerpt its own coherent lighting identity. Across the ten
  // excerpts the show includes chase, blink, fade, roll, strobe, and hard-cut
  // treatments, without changing visual language halfway through one clip.
  constexpr Color neonGreen = {16, 255, 72};
  constexpr Color neonMagenta = {255, 0, 190};
  constexpr Color electricBlue = {0, 96, 255};
  const uint32_t step = animationStep;
  const uint8_t profile = scenePalette - 17;
  clearPixels();
  CircuitPlayground.setBrightness(brightness);

  if (profile == 0) {
    // S018: a wide green wedge chases clockwise and fades at its edges.
    const uint8_t runner = 0;
    for (uint8_t width = 0; width < 5; ++width) {
      const uint8_t levels[5] = {70, 170, 255, 170, 70};
      const Color color = scaleColor(neonGreen, levels[width]);
      const uint8_t pixel = (runner + width) % PIXEL_COUNT;
      setFramePixel(pixel, color.red, color.green, color.blue);
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
    const uint8_t shift = 0;
    for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
      const Color color = ((pixel + shift) % PIXEL_COUNT < 5) ? electricBlue
                                                               : neonMagenta;
      setFramePixel(pixel, color.red, color.green,
                                      color.blue);
    }
  } else if (profile == 3) {
    // S021: two broad scene-color halves breathe and slowly trade places.
    const uint8_t breath = step % 40;
    const uint8_t triangle = breath < 20 ? breath : 39 - breath;
    const uint8_t level = static_cast<uint8_t>(55 + triangle * 10);
    const uint8_t shift = 0;
    for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
      const uint8_t index = ((pixel + shift) % PIXEL_COUNT < 5) ? 0 : 2;
      const Color color = scaleColor(SCENE_PALETTES[scenePalette][index], level);
      setFramePixel(pixel, color.red, color.green, color.blue);
    }
  } else if (profile == 4) {
    // S022: rotating green/blue halves punctuated by a full magenta hit.
    const uint8_t beat = step % 24;
    if (beat < 3) {
      fillPixels(neonMagenta);
    } else {
      const uint8_t shift = 0;
      for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
        const Color color = ((pixel + shift) % PIXEL_COUNT < 5) ? neonGreen
                                                                 : electricBlue;
        setFramePixel(pixel, color.red, color.green, color.blue);
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
      setFramePixel(pixel, color.red, color.green, color.blue);
    }
    if (step % 20 < 2) fillPixels({255, 255, 255});
  } else if (profile == 6) {
    // S024: a wide warm-gold wedge circles through darkness.
    const Color gold = SCENE_PALETTES[scenePalette][2];
    const uint8_t runner = 0;
    for (uint8_t width = 0; width < 5; ++width) {
      const uint8_t levels[5] = {65, 155, 255, 155, 65};
      const Color color = scaleColor(gold, levels[width]);
      const uint8_t pixel = (runner + width) % PIXEL_COUNT;
      setFramePixel(pixel, color.red, color.green, color.blue);
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
      const uint8_t shift = 0;
      for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
        const Color color = ((pixel + shift) % PIXEL_COUNT < 5) ? neonMagenta
                                                                 : neonGreen;
        setFramePixel(pixel, color.red, color.green,
                                        color.blue);
      }
    }
  }


}

void drawKlingSceneFrame(uint8_t brightness) {
  const uint32_t step = animationStep;
  const Color first = SCENE_PALETTES[scenePalette][0];
  const Color second = SCENE_PALETTES[scenePalette][1];
  const Color third = SCENE_PALETTES[scenePalette][2];
  clearPixels();
  CircuitPlayground.setBrightness(brightness);

  if (scenePalette == 29) {
    // S030 confrontation: tense yellow/cyan halves with sharp magenta retorts.
    if (step % 24 < 3) {
      fillPixels(third);
    } else {
      const uint8_t shift = 0;
      for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
        const Color color = ((pixel + shift) % PIXEL_COUNT < 5) ? first : second;
        setFramePixel(pixel, color.red, color.green, color.blue);
      }
    }
  } else if (scenePalette == 30) {
    // S031 rule/suck/rule: cyan approval, magenta rejection, lime comeback.
    const uint32_t age = millis() - sceneStartedAt;
    const uint8_t phrase = age < 2250 ? 0 : (age < 4500 ? 1 : 2);
    const Color phraseColor = phrase == 0 ? first : (phrase == 1 ? second : third);
    const uint8_t pulse = step % 12;
    fillPixels(scaleColor(phraseColor, pulse < 3 ? 255 : 125));
  } else {
    // S032 bacteria rave: rotating lime/cyan halves with full magenta club hits.
    if (step % 18 < 3) {
      fillPixels(second);
    } else {
      const uint8_t shift = 0;
      for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
        const Color color = ((pixel + shift) % PIXEL_COUNT < 5) ? first : third;
        setFramePixel(pixel, color.red, color.green, color.blue);
      }
    }
  }


}

uint8_t smoothPulseLevel(uint32_t age) {
  // Four 7.5-second breathing cycles. Smoothstep softens both ends so the
  // powered-on ring never looks like a blink.
  const uint32_t cycle = age % 7500U;
  const uint32_t distance = cycle <= 3750U ? cycle : 7500U - cycle;
  const uint32_t linear = distance * 255U / 3750U;
  return static_cast<uint8_t>(
      (linear * linear * (765U - 2U * linear)) / (255U * 255U));
}

void drawArcCoreFrame() {
  uint32_t age = (millis() - sceneStartedAt) % ARC_CYCLE_MS;
  constexpr Color deepBlue = {0, 34, 105};
  constexpr Color poweredBlue = {0, 112, 255};
  constexpr Color gold = {255, 176, 32};
  constexpr Color paleGold = {255, 226, 128};
  clearPixels();

  if (age < 4200U) {
    // Seven accelerating revolutions build energy around a dim blue base.
    // The faster initial speed makes the rotation obvious immediately.
    fillPixels(scaleColor(deepBlue, 90));
    const uint64_t baseStepsQ16 = static_cast<uint64_t>(age) * 65536U / 170U;
    const uint64_t accelerationQ16 = static_cast<uint64_t>(age) * age *
        65536U * 453U / (4200ULL * 4200ULL * 10ULL);
    rotationQ16 = static_cast<uint32_t>(
        (baseStepsQ16 + accelerationQ16) % (PIXEL_COUNT * 65536ULL));
    const Color tail = scaleColor(gold, 65);
    const Color shoulder = scaleColor(paleGold, 165);
    setFramePixel(0, tail.red, tail.green, tail.blue);
    setFramePixel(1, shoulder.red, shoulder.green, shoulder.blue);
    setFramePixel(2, gold.red, gold.green, gold.blue);
    setFramePixel(3, shoulder.red, shoulder.green, shoulder.blue);
    CircuitPlayground.setBrightness(
        static_cast<uint8_t>(68U + age * 32U / 4200U));
    presentFrame(true);
    return;
  }

  if (age < 4650U) {
    // The chase closes into a complete ring and blooms from gold to pale gold.
    const uint8_t mix = static_cast<uint8_t>((age - 4200U) * 255U / 450U);
    fillPixels(blendColor(gold, paleGold, mix));
    CircuitPlayground.setBrightness(
        static_cast<uint8_t>(100U + (age - 4200U) * 25U / 450U));
    presentFrame(false);
    return;
  }

  if (age < 4850U) {
    // Short, decisive gold ignition glow with no white flash.
    fillPixels(paleGold);
    CircuitPlayground.setBrightness(125);
    presentFrame(false);
    return;
  }

  if (age < ARC_STARTUP_MS) {
    // Cool directly from gold into the blue operating state.
    const uint8_t mix = static_cast<uint8_t>(
        (age - 4850U) * 255U / (ARC_STARTUP_MS - 4850U));
    fillPixels(blendColor(paleGold, poweredBlue, mix));
    CircuitPlayground.setBrightness(105);
    presentFrame(false);
    return;
  }

  const uint8_t pulse = smoothPulseLevel(age - ARC_STARTUP_MS);
  const Color lowBlue = {0, 38, 125};
  const Color highBlue = {25, 195, 255};
  fillPixels(blendColor(lowBlue, highBlue, pulse));
  CircuitPlayground.setBrightness(static_cast<uint8_t>(45U + pulse * 60U / 255U));
  presentFrame(false);
}

// Milliseconds per LED position. Zero means a non-rotating effect.
uint16_t rotationPeriod() {
  if (isWizardOfOzScene()) return 90;
  if (isSpongeBobScene()) return 135;
  if (scenePalette == 29) return 225;
  if (scenePalette == 30) return 0;
  if (scenePalette == 31) return 50;
  if (isShoesSongScene()) {
    switch (scenePalette - 17) {
      case 0: case 2: case 9: return 50;
      case 3: return 200;
      case 4: case 6: return 75;
      default: return 0;
    }
  }
  return 45;
}

void presentFrame(bool rotate) {
  const uint8_t shift = rotate ? rotationQ16 >> 16 : 0;
  const uint8_t fraction = rotate ? (rotationQ16 & 65535U) >> 8 : 0;
  for (uint8_t pixel = 0; pixel < PIXEL_COUNT; ++pixel) {
    const uint8_t source = (pixel + PIXEL_COUNT - shift) % PIXEL_COUNT;
    const Color color = blendColor(framePixels[source],
        framePixels[(source + PIXEL_COUNT - 1) % PIXEL_COUNT], fraction);
    CircuitPlayground.strip.setPixelColor(pixel, color.red, color.green, color.blue);
  }
  CircuitPlayground.strip.show();
}

void drawSceneFrame(uint8_t brightness) {
  if (arcCoreActive) {
    drawArcCoreFrame();
    return;
  }
  const uint32_t now = millis();
  const uint16_t period = rotationPeriod();
  if (period) {
    rotationQ16 = (rotationQ16 + (uint64_t)(now - rotationUpdatedAt) * 65536U / period)
                   % (PIXEL_COUNT * 65536U);
  }
  rotationUpdatedAt = now;
  animationStep = (now - sceneStartedAt) /
      (isShoesSongScene() ? SONG_FRAME_INTERVAL_MS : CALM_FRAME_INTERVAL_MS);
  if (isWizardOfOzScene()) drawWizardOfOzFrame(brightness);
  else if (isSpongeBobScene()) drawSpongeBobFrame(brightness);
  else if (isKlingScene()) drawKlingSceneFrame(brightness);
  else if (isShoesSongScene()) drawSongSceneFrame(brightness);
  else drawCalmSceneFrame(brightness);
  presentFrame(period != 0);
}

void beginScene(uint32_t now) {
  arcCoreActive = false;
  scenePalette = decodedScene < SCENE_COUNT ? decodedScene : 17;
  sceneStartedAt = now;
  rotationUpdatedAt = now;
  animationStep = 0;
  sceneActive = true;
  effectVisible = true;
  lastFrameAt = 0;
  decodeState = DecodeState::Active;
  Serial.print("HALO_SCENE S");
  if (scenePalette + 1 < 10) Serial.print("00");
  else if (scenePalette + 1 < 100) Serial.print('0');
  Serial.println(scenePalette + 1);
  drawSceneFrame(isShoesSongScene() ? SONG_BRIGHTNESS : EFFECT_BRIGHTNESS);
  lastFrameAt = now;
}

void beginArcCore(uint32_t now) {
  sceneStartedAt = now;
  rotationUpdatedAt = now;
  animationStep = 0;
  sceneActive = true;
  arcCoreActive = true;
  effectVisible = true;
  lastFrameAt = 0;
  decodeState = DecodeState::Active;
  Serial.println("HALO_ARCCORE");
  drawArcCoreFrame();
  lastFrameAt = now;
}

void recoverArcFromHeldBootSignal(uint32_t now, bool inputHigh) {
  if (!bootArcRecoveryPending) return;
  if (!inputHigh) {
    bootArcRecoveryPending = false;
    return;
  }
  if (now - bootHighStartedAt < ARC_BOOT_RECOVERY_MS) return;
  bootArcRecoveryPending = false;
  Serial.println("HALO_ARCCORE_BOOT_RECOVERY");
  beginArcCore(now);
}

void handleFallingEdge(uint32_t now) {
  const uint32_t pulseWidth = now - pulseStartedAt;
  if (decodeState == DecodeState::Active) {
    const bool wasArcCore = arcCoreActive;
    sceneActive = false;
    arcCoreActive = false;
    afterglowUntil = wasArcCore ? now : now + AFTERGLOW_MS;
    decodeState = DecodeState::WaitForSync;
    return;
  }

  if (decodeState == DecodeState::WaitForSync) {
    if (pulseWidth >= ARC_SYNC_MIN_MS && pulseWidth <= ARC_SYNC_MAX_MS) {
      decodeState = DecodeState::ArcArmed;
    } else if (pulseWidth >= SYNC_MIN_MS && pulseWidth <= SYNC_MAX_MS) {
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
  previousInput = digitalRead(PIN_SCENE_TRIGGER) == HIGH;
  if (previousInput) {
    pulseStartedAt = millis();
    bootHighStartedAt = pulseStartedAt;
    bootArcRecoveryPending = true;
  }
  drawIdle();
  Serial.println("HALO_READY A1 scene-code v3 + ARCCORE");
}

void loop() {
  const uint32_t now = millis();
  const bool inputHigh = digitalRead(PIN_SCENE_TRIGGER) == HIGH;

  recoverArcFromHeldBootSignal(now, inputHigh);

  if (inputHigh != previousInput) {
    if (inputHigh) {
      pulseStartedAt = now;
      if (decodeState == DecodeState::Armed) beginScene(now);
      else if (decodeState == DecodeState::ArcArmed) beginArcCore(now);
    } else {
      handleFallingEdge(now);
    }
    previousInput = inputHigh;
  }

  if (now - lastStatusAt >= STATUS_INTERVAL_MS) {
    lastStatusAt = now;
    Serial.print("HALO_STATUS input=");
    Serial.print(inputHigh ? 1 : 0);
    Serial.print(" arc=");
    Serial.print(arcCoreActive ? 1 : 0);
    Serial.print(" state=");
    Serial.println(static_cast<uint8_t>(decodeState));
  }

  const int32_t afterglowRemaining = static_cast<int32_t>(afterglowUntil - now);
  const uint16_t frameInterval = 16; // Smooth rendering, independent of effect tempo.
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
