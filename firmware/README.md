# Firmware

This directory contains the full Shoes Off, Dirtbag! firmware. The original
Arduino display proof remains in `src/`; the active ESP32 build is
`idf_wearable/`, and the active halo build is `circuit_playground_halo/`.

The active build now includes the GC9A01 display, two local buttons, 27-scene
visual SD runtime, DFPlayer/speaker, classic Bluetooth pedal, saved volume,
right-pedal QR mode, randomized alternating playback, embedded fallback, and
scene-coded halo output. See each subproject README for build commands.

## Historical Milestone 1 behavior

1. Boot to the circular `SHOES CHAOS BOX` idle screen.
2. Press the local trigger button.
3. Show one of four test reminder scenes for 2.2 seconds.
4. Return to idle and wait for the next press.

Each press advances to the next test scene. Repeated input is ignored while a scene is active.

## Historical Arduino proof build

The project uses PlatformIO with the Arduino framework and the generic `esp32dev` board profile.

```sh
pio run
```

Connect the ESP32 with a USB-C data cable, then upload with:

```sh
pio run --target upload
pio device monitor
```

Do not upload until the wiring in `WIRING_MILESTONE_1.md` has been checked against the labels printed on the actual boards.
