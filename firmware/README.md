# Firmware

This directory contains the ESP32 firmware for Shoes Chaos Box. The first milestone is deliberately small: prove the owned 38-pin ESP32, GC9A01 round screen, and one local trigger button before adding storage, audio, LEDs, or Bluetooth.

## Milestone 1 behavior

1. Boot to the circular `SHOES CHAOS BOX` idle screen.
2. Press the local trigger button.
3. Show one of four test reminder scenes for 2.2 seconds.
4. Return to idle and wait for the next press.

Each press advances to the next test scene. Repeated input is ignored while a scene is active.

## Build

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

