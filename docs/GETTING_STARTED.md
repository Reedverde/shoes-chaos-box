# Start a similar build

This is a documented proof of concept, not a finished kit. Start on a bench and test each module before arranging it as a wearable. The public starter pack gives you synthetic screen/tone files; it does not reproduce Reed's performance show.

## 1. Explore the software first

Clone this repository, install Node.js 22+ and Python 3, and run the Cloudflare service as described in the [root README](../README.md). The downloadable ZIP is also committed at [shoes-starter-v1.zip](../cloudflare/chaos-director/public/downloads/shoes-starter-v1.zip). Verify its SHA-256 against [the manifest](../cloudflare/chaos-director/public/downloads/starter-manifest.json).

## 2. Identify the hardware

The active build uses an ESP32 DevKit (38-pin variant), GC9A01 240×240 round SPI display, six-pin HW-125-style microSD adapter, DFPlayer Mini, 4-ohm speaker, two momentary buttons, and Adafruit Circuit Playground Express (SAMD21). The STRICH SPT-10 Bluetooth pedal is optional for bench tests. Exact module variants and pin labels matter: a breadboard coordinate alone is not a universal pinout.

| Signal | ESP32 GPIO / connection |
|---|---|
| Shared screen + visual SD clock | GPIO14 |
| Shared screen + visual SD MOSI | GPIO27 |
| Visual SD MISO | GPIO19 |
| Visual SD CS | GPIO5 |
| Screen CS / DC / reset / backlight | GPIO26 / 25 / 33 / 32 |
| DFPlayer TX → ESP receive | GPIO16 |
| ESP transmit → DFPlayer RX | GPIO17 through 1 kΩ |
| Forward / previous local buttons | GPIO13 / GPIO22 to ground when pressed |
| Halo signal | GPIO21 → Circuit Playground A1, plus common ground |
| Speaker | DFPlayer SPK1 and SPK2; neither lead is ground |

Use [the active bench record](../firmware/BENCH_REWIRE_PROGRESS.md) and [firmware wiring notes](../firmware/idf_wearable/README.md) for power, exact module labels, and continuity checks. The old column-0 layout is historical; the current ESP starts at A1. Column 20 is used for SPI signals, not a ground bus. Screen logic is 3.3 V. The bench has separate 5 V and 3.3 V supplies/rails with common ground. Confirm your SD module's regulator and voltage requirements before copying its VCC connection. Keep the separate halo battery-positive rail isolated. Do not rewire while powered.

## 3. Prepare two spare cards

Back up any existing cards. Use FAT32 cards compatible with your modules. Unzip the starter pack and copy the **contents** of each folder to the corresponding card's root:

```text
Card A (DFPlayer)         Card B (visual SD reader)
mp3/0001.mp3             SCENES.CSV
...                      SCENES/S001/FRAMES.CSV
mp3/0029.mp3              SCENES/S001/F001.RAW
                         SCENES/S001/F001.BMP
                         ... through S032
```

All 32 IDs remain in order. Each starter scene holds a geometric screen for six seconds with a quiet tone track. The increasing bar identifies its slot. S030, S031, and S032 reuse tracks 3, 20, and 23. Arc Core files are absent; the firmware returns home when they cannot be loaded. Home/QR/fallback assets are embedded in the firmware and are separate from the starter pack.

Do not overwrite this starter `SCENES.CSV` with the performance catalog download: that index expects the longer, matching performance media. Never rename `playlist.csv` to `SCENES.CSV`. Current firmware does not import playlist order.

## 4. Build the two active firmwares

Install PlatformIO Core using its [official instructions](https://docs.platformio.org/en/latest/core/installation/index.html). From the repository root:

```sh
pio run -d firmware/idf_wearable
pio run -d firmware/circuit_playground_halo
```

The ESP32 platform is pinned to espressif32 6.10.0 (ESP-IDF 5.4.0). The halo
platform is pinned to atmelsam 9.0.0 with Adafruit Circuit Playground 1.12.0,
matching the installed build dependencies reviewed here.

`firmware/idf_wearable` is the active ESP-IDF application. `firmware/circuit_playground_halo` is the separate Arduino/SAMD21 LED application. `firmware/src` and `firmware/idf_pedal_probe` are earlier experiments; do not flash those expecting the current experience.

Read each firmware README before uploading. Select the actual device port and correct board. Save your current build first. Build success does not prove wiring or media correctness. Ordinary builds must **not** define `SHOES_TIMING_BENCH`; that flag starts a special media-specific hardware regression run.

## 5. Test in small steps

Verify power and ground continuity with power disconnected, then measure powered rails before connecting the modules. Test screen and SD first, then DFPlayer/speaker at low volume, then buttons, halo, and pedal. Follow the [demo controls](DEMO.md) and [acceptance checklist](../TODO.md). A continuity beep verifies a connection, not the selected GPIO or program behavior.

The new public starter pack passes structural checks and audio decoding; physical playback of it has not yet been verified. The recorded 32-scene timing run used Reed's performance media, not these fixtures. Keep those two validation claims separate.

## Checks you can run without a device

```sh
(cd cloudflare/chaos-director && npm ci && npm test)
python3 tools/validate_starter_pack.py
python3 firmware/idf_wearable/tests/host/test_display_cycle.py
c++ -std=c++17 -Ifirmware/circuit_playground_halo/tests/host firmware/circuit_playground_halo/tests/host/test.cpp -o /tmp/shoes-halo-test
/tmp/shoes-halo-test
```

The control and halo tests compile the actual application logic with simulated hardware boundaries. They do not test Bluetooth radio transport or physical LED output. The [hardware benchmark](../firmware/idf_wearable/tests/README.md) is optional and tied to a specific private media fixture.
