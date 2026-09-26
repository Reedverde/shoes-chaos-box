# Build Plan

Updated after the 2026-09-26 project audit.

## Final first-build architecture

```text
STRICH Bluetooth pedal ------------------------------+
GPIO13 scene button / GPIO22 QR button --------------+--> ESP32 pocket brain
                                                        |-- shared SPI -> display + Card B
                                                        |-- UART -> DFPlayer/Card A
                                                        |-- GPIO21 -> Circuit Playground A1
                                                        `-- Bluetooth -> pedal

Pocket brain -- keyed 8-wire lead ----------------------> lapel display
DFPlayer SPK1/SPK2 -- keyed 2-wire lead ----------------> lapel/pocket speaker
ESP32 GPIO21/GND -- keyed 2-wire lead ------------------> lapel halo controller

DP20S USB pack -----------------------------------------> ESP32 pocket brain
Clipped battery pack -----------------------------------> Circuit Playground
```

The two battery-positive rails are never joined. ESP32 and Circuit Playground
share only signal ground and GPIO21/A1. The speaker is a bridge output and must
use SPK1/SPK2—not system ground.

## Phase 1 — recover the current bench baseline

1. Double-press Circuit Playground RESET until `CPLAYBOOT` appears.
2. Flash `firmware/circuit_playground_halo` and verify the write.
3. Test the ten diffuser-aware S018-S027 profiles.
4. Photograph and label the working breadboard before reorientation.

**Exit:** screen, both local buttons, both pedal functions, audio, both cards,
and halo all work from a cold boot.

## Phase 2 — reorient without redesigning

1. Keep the ESP32 and support electronics on the existing breadboard.
2. Move the GC9A01 off-board to the keyed 8-wire cable.
3. Move speaker and halo control to their separate keyed 2-wire cables.
4. Lay the HW-125 visual reader flat beside the breadboard using a short six-pin
   pigtail; retain the verified 4 MHz SD clock and current pin map.
5. Put the local buttons at the accessible end of the breadboard.
6. Use the breadboard rails only after checking whether either rail is split.

**Exit:** the pocket unit fits its carrier, all cables detach, and every function
still passes without moving wires by hand.

## Phase 3 — make the two physical units

### Pocket unit

- Shallow nonconductive tray around the breadboard
- USB-C and both card slots accessible
- Speaker opening if the speaker remains in the pocket
- Cable anchors before each connector
- Labelled DISPLAY, SPEAKER, and HALO leads

### Lapel unit

- GC9A01 facing outward
- Thin diffuser ring
- Circuit Playground behind the screen
- Nylon standoffs and rigid lightweight backing
- Pocket-edge clip plus lower stabilizer, or two locking pins
- Service loop and strain relief inside the jacket

The 3D printing pen may form cable guides, spacers, or a rough bezel after the
electronics are removed. It is not used around powered boards, as conductor
insulation, or as the sole load-bearing attachment.

**Exit:** wearer can walk, sit, turn, disconnect the lapel unit, and remove the
system without stressing a solder joint or header.

## Phase 4 — functional test

- Validate every S001-S027 visual/audio pair.
- Confirm left pedal scene, right pedal QR, local buttons, and Mode 1 volume.
- Confirm the 36-play alternating deck and ten-second non-banking lockout.
- Confirm Card B removal invokes the flashed S018 visual fallback and track 0018
  still plays when Card A is present.
- Run 100 triggers.

## Phase 5 — event reliability

- 30-minute maximum-use test for reset, heat, and audio distortion
- Ten-minute wear/movement and controlled cable-pull test
- Five-hour event-profile battery test
- DP20S idle auto-shutoff test
- Speaker loudness test in a noisy room
- Pack known-good card backups, USB cable, spare speaker, and spare SD reader

The exact checklist is maintained in [TODO.md](TODO.md).
