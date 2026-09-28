# OMG Shoes wearable firmware

This is the current combined ESP-IDF build for the wearable device:

- ESP32 DevKit (38-pin)
- GC9A01 240 x 240 circular display
- HW-125 visual microSD reader on the shared SPI bus
- DFPlayer Mini plus speaker over UART
- STRICH SPT-10 Bluetooth pedal in Modes 1, 4, and 5
- local forward-scene and previous-scene buttons
- GPIO21 halo trigger to the Circuit Playground

## Runtime behavior

- The rest screen and QR screen (`https://reedverde.com/sf`) are compiled into
  ESP32 flash and do not depend on either SD card.
- The left pedal or local scene button selects from a randomized alternating
  deck. Every outside-source scene is followed by a Shoes-family moment. The
  four Curb variants S003/S028/S029/S030 run twice per 48-play cycle; each
  other outside-source scene runs once. The 12 separators include the ten
  original Kelly moments plus Kling Kelly and bacteria-rave scenes.
- The blue local button and left pedal step forward through the randomized scene
  order. The green local button steps backward through that exact history; a
  following blue-button or left-pedal press returns forward to the scene just
  left behind.
- The complete deck and its next-scene position are saved in ESP32 NVS after
  every normal selection. Opening QR or Arc Core leaves the position unchanged,
  and an ESP32 restart resumes the saved deck at the next scene.
- In the normal scene-control mode (pedal Mode 5), successive right-pedal
  presses cycle **QR → silent Arc Core loop → Shoes Off, Dirtbag home screen**.
  The next press starts again at QR. During a normal scene, right interrupts
  playback and opens QR; the following press starts Arc Core. Left still plays
  normal scenes from home and can leave Arc Core to play a scene.
- In pedal Mode 4, either pedal starts the silent `ARCCORE` special from Card B.
  Its 5.04-second startup is followed by 30 seconds of slow blue powered-on
  pulsing, then the full 35.04-second sequence repeats. Either pedal stops it.
  A dedicated long-sync pulse on GPIO21 starts the matching Circuit Playground
  fast gold-chase, gold-ignition, and blue-breathing halo sequence without using one
  of the 32 normal scene IDs.
  The command is re-sent at each complete 35.04-second Arc display loop so the
  lighting stays synchronized and automatically recovers a missed start.
- In pedal Mode 1, Page Up/left raises DFPlayer volume and Page Down/right
  lowers it. Each press moves one step on the 0-30 scale, shows the new number
  for 1.2 seconds, and saves it in ESP32 NVS so it survives a reboot. The
  initial default remains 18. Up/Down Arrow mode is accepted as an alias.
- Left-pedal triggers during a ten-second lockout are discarded instead of
  banked. The blue and green board buttons bypass the remaining cooldown after
  playback so manual forward/back navigation stays responsive.
- GPIO21 sends the S001-S032 scene number to the Circuit Playground over the
  existing A1 wire, then remains high for the scene. The halo uses the matching
  three-color palette and supplies its two-second afterglow after GPIO21 drops.
- Normal scenes retain their existing frame counts and timelines. The current
  pack mixes dense original animations with eight-frame newer scenes; it is
  not an all-eight-frame pack. Arc Core uses all 48 supported frames: 16
  startup frames and 32 powered-on pulse frames.
  Each complete 240 x 240 RGB565 frame is loaded into RAM while the previous
  image remains on the LCD, then sent to the display at the configured 32 MHz
  rate. Direct file reads replace buffered stdio's small SD transactions.
  No frames are silently skipped or removed by this repair.

## Card assignments

The performance media is held in a separate local staging workspace, not in
this repository. New builders can use the [public synthetic starter pack](../../docs/GETTING_STARTED.md).

- **Card A - DFPlayer:** `mp3/0001.mp3` through `mp3/0029.mp3`
- **Card B - HW-125 visual reader:** `SCENES.CSV` and
  `SCENES/S001` through `SCENES/S032`, plus the named silent special at
  `SPECIAL/ARCCORE`

Scene `S001` maps to audio `0001`, and so on through `S029`/`0029`. Kling
scenes S030, S031, and S032 reuse audio 0003, 0020, and 0023 respectively.

## No-visual-card fallback

If Card B is absent or its scene index cannot be mounted, a six-second master
version of **S018 - "Oh my God, shoes"** plays from six RGB565 keyframes compiled
into ESP32 flash. It uses DFPlayer track `0018`, correcting the earlier fallback
that incorrectly requested track 1.

The visual fallback, opening screen, and QR are fully cardless. Spoken audio is
not: the physical speaker is connected to the DFPlayer amplifier, and the
DFPlayer can decode only from its own Card A. Therefore the S018 fallback is
silent if Card A is physically removed.

## Build and upload

The 4 MB flash uses the included 3 MB application partition. Current verified
build usage is 60.7% flash and 12.1% fixed RAM. Playback also allocates two
57,600-byte DMA-capable half-frame buffers before Bluetooth starts. Both
halves are loaded from the SD card before either half is drawn, preventing the
visible top-to-bottom image peel without requiring one oversized contiguous
allocation.

```sh
/tmp/shoes-pio/bin/pio run
/tmp/shoes-pio/bin/pio run -t upload --upload-port /dev/cu.usbserial-0001
```

The visual card is initialized at 4 MHz and then switched to 16 MHz. The
September 27 throughput test measured roughly 95 ms per RAW read at 16 MHz,
versus 372 ms with the former stdio reader at 4 MHz. Display transfer takes
roughly 35 ms. A 20 MHz test immediately failed; do not assume a faster rate
works on this wiring.

A scene summary reports shown/expected frames, elapsed time, maximum read and
draw times, and worst frame-start lateness. It is printed after playback so
logging does not consume frame time. Some 50–100 ms holds remain shorter than
the read-plus-draw cost; on-time scene completion is not a claim of zero
per-frame jitter or measured acoustic synchronization. The DFPlayer still
uses the existing fixed 120 ms startup allowance.

See `tests/README.md` for the optional full-card validation. The normal build
has no automatic playback test. Arc Core, the pedal modes, buttons, volume,
QR interruption, and the current media files remain in place.
