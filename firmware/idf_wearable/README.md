# Combined wearable firmware

This is the verified ESP-IDF bench baseline for the Shoes Chaos Box. It combines:

- GC9A01 240 x 240 round display
- local GPIO13 trigger button
- STRICH SPT-10 classic Bluetooth HID pedal in Mode 5
- four temporary text/color scenes

Left pedal (Space, HID `0x2C`), right pedal (Enter, HID `0x28`), and the local button all feed the same scene queue. The firmware scans again when the pedal is unavailable or disconnected, while the local button remains usable.

Build and upload from a filesystem path without spaces:

```sh
pio run
pio run --target upload --upload-port /dev/cu.usbserial-0001
```

The original Arduino display proof remains one directory above this project. `idf_pedal_probe/` is the isolated Bluetooth diagnostic build.
