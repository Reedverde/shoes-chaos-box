# Project State

Last audited: 2026-09-26

## Current result

**Shoes Off, Dirtbag!** is a working two-controller bench prototype. The ESP32
drives the round display, two local buttons, visual microSD, DFPlayer/speaker,
and STRICH Bluetooth pedal. The Circuit Playground Express supplies the lapel
halo. The next milestone is mechanical conversion from the working breadboard
into a secure two-unit wearable; a perfboard conversion is not required for the
first event build.

## Verified or implemented

- GC9A01 240 x 240 display, local scene button, local QR button, DFPlayer,
  speaker, visual SD reader, both 8 GB cards, and Bluetooth pedal are connected.
- Card A contains `mp3/0001.mp3` through `0027.mp3`. Card B contains the
  optimized eight-frame exports for scenes S001-S027.
- The left pedal launches a scene; the right pedal toggles the QR screen for
  `https://reedverde.com/sf`.
- Pedal Mode 1 Page Up/Down changes DFPlayer volume from 0-30, shows the value
  for 1.2 seconds, and saves it in NVS. Default volume is 18.
- Scene order alternates an outside-source scene with a Kelly/Shoes scene.
  S003 Curb appears twice per 36-play deck; all other outside scenes appear
  once; the Shoes separators are shuffled.
- Visual frames are read at the hardware-stable 4 MHz SD clock into two
  57,600-byte buffers and sent to the LCD at 32 MHz, eliminating the old
  top-to-bottom peel without the failed 115,200-byte allocation.
- The opening image, QR image, and six-frame S018 master fallback are stored in
  ESP32 flash. Fallback audio is correctly mapped to DFPlayer track 0018 and
  therefore still requires Card A.
- GPIO21 sends a five-bit scene ID plus active-scene state to Circuit Playground
  A1. The halo source has 27 scene palettes and ten diffuser-aware club-light
  profiles for S018-S027.

## Current exception

The newest diffuser-optimized Circuit Playground firmware is built but not yet
verified on the physical board. Its last upload was interrupted after erase and
the board dropped out of USB bootloader mode. Recovery is safe: double-press
RESET until `CPLAYBOOT` appears, upload, then physically test S018-S027. The
ESP32 firmware and media cards are unaffected.

## Frozen wearable architecture

### Pocket unit

- Keep the full-size breadboard for the first wearable.
- ESP32, DFPlayer, Card A, visual SD reader/Card B, and local buttons remain on
  or immediately beside the breadboard.
- Lay the six-pin visual SD reader flat beside the breadboard on a short secured
  pigtail; do not leave the HW-125 standing vertically.
- Put the assembly in a shallow nonconductive pocket tray or case with access to
  USB-C, both cards, and buttons.

### Lapel unit

- GC9A01 display in front.
- Diffuser ring and Circuit Playground behind it.
- Rigid lightweight backing and two-point garment attachment.
- Circuit Playground remains powered by its own clipped battery pack.

### Three serviceable leads

1. **8 conductors — display:** 3V3, GND, SCLK/GPIO14, MOSI/GPIO27,
   CS/GPIO26, DC/GPIO25, RST/GPIO33, BL/GPIO32.
2. **2 conductors — halo control:** ESP32 GPIO21 to Circuit Playground A1,
   plus shared GND. Do not join the two battery-positive rails.
3. **2 conductors — speaker:** DFPlayer SPK1 and SPK2. Neither speaker wire is
   system ground.

This split is preferred over one 12-pin connector: it separates power-sensitive
display wiring, the differential speaker output, and the halo control; it also
makes each lapel component replaceable.

## Acceptance tests still required

- Recover and flash the Circuit Playground; visually approve all ten Shoes
  lighting profiles through the actual diffuser.
- Verify left pedal = scene and right pedal = QR after a cold boot.
- Verify Mode 1 volume up/down and saved volume after reboot.
- Exercise all 27 audio/visual mappings and confirm no missing frames or tracks.
- Test cardless ESP fallback, then reinstall both cards.
- Run 100 triggers, a 30-minute movement/load test, and a five-hour event-profile
  battery test.
- Measure speaker intelligibility from the final pocket/lapel position.

The active task list is in [TODO.md](TODO.md); the reduced procurement list is
in [PURCHASE_LIST.md](PURCHASE_LIST.md).
