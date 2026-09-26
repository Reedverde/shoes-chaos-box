# OMG Shoes wearable firmware

This is the current combined ESP-IDF build for the wearable device:

- ESP32 DevKit (38-pin)
- GC9A01 240 x 240 circular display
- HW-125 visual microSD reader on the shared SPI bus
- DFPlayer Mini plus speaker over UART
- STRICH SPT-10 Bluetooth pedal in Mode 5
- local scene and QR buttons
- GPIO21 halo trigger to the Circuit Playground

## Runtime behavior

- The rest screen and QR screen (`https://reedverde.com/sf`) are compiled into
  ESP32 flash and do not depend on either SD card.
- The left pedal or local scene button selects from a randomized alternating
  deck. Every S001-S017 outside-source scene is followed by one of the actual
  Kelly/*Shoes* moments from S018-S027. Curb Your Enthusiasm (S003) runs twice
  per 36-play cycle; each other outside-source scene runs once. The ten Kelly
  moments are shuffled as separator decks so they stay varied.
- The right pedal or local QR button toggles the QR screen. QR has priority and
  can interrupt a playing scene.
- In pedal Mode 1, Page Up/left raises DFPlayer volume and Page Down/right
  lowers it. Each press moves one step on the 0-30 scale, shows the new number
  for 1.2 seconds, and saves it in ESP32 NVS so it survives a reboot. The
  initial default remains 18. Up/Down Arrow mode is accepted as an alias.
- Triggers during a ten-second lockout are discarded instead of banked.
- GPIO21 sends the S001-S027 scene number to the Circuit Playground over the
  existing A1 wire, then remains high for the scene. The halo uses the matching
  three-color palette and supplies its two-second afterglow after GPIO21 drops.
- Each scene uses eight intentionally spaced sprite-like keyframes. The next
  complete 240 x 240 RGB565 frame is prefetched into RAM while the current
  image remains on the LCD, then sent to the display in a fast 32 MHz burst.
  This hides the 4 MHz SD read and removes the old top-to-bottom peel effect.

## Card assignments

The prepared runtime images are in
`../../staging_shoes_assets/runtime-card-v4-prefetch`.

- **Card A - DFPlayer:** `mp3/0001.mp3` through `mp3/0027.mp3`
- **Card B - HW-125 visual reader:** `SCENES.CSV` and
  `SCENES/S001` through `SCENES/S027`

Scene `S001` maps to audio `0001`, and so on through `S027`/`0027`.

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

The visual SD clock remains at the hardware-proven 4 MHz because the current
breadboard/HW-125 wiring became unreliable at higher rates.
