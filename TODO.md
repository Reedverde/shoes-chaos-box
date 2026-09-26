# Active Todo List

Updated: 2026-09-26

## Do next — preserve the working electronics

- [ ] Mount Card A on the computer and replace `mp3/0001.mp3` with the corrected
  complete Damn Daniel catchphrase/laugh export from runtime-card-v4-prefetch.
- [x] Double-press Circuit Playground RESET until `CPLAYBOOT` appears.
- [x] Flash the built diffuser-optimized halo firmware and verify checksum.
- [ ] Trigger S018-S027 and approve each profile through the real diffuser.
- [ ] Photograph and label every current breadboard connection before moving a
  single wire.
- [ ] Mark the two cards permanently: **A AUDIO / DFPLAYER** and
  **B VISUAL / HW-125**.
- [ ] Cold-boot with both cards installed and verify screen, buttons, speaker,
  pedal, QR, and halo.

## Convert the bench build into the wearable

- [ ] Rebuild to the numbered layout in
  `firmware/WIRING_FINAL_POCKET_BREADBOARD.md`: ESP32 0–18, ground channel 20,
  visual SD 23–28, DFPlayer 32–39, then the two buttons.
- [ ] Establish separate +5 V and +3.3 V red rails and common blue ground rails;
  meter-test them before attaching any module.
- [ ] Move the display off the breadboard onto one keyed 8-conductor lead:
  3V3, GND, GPIO14, GPIO27, GPIO26, GPIO25, GPIO33, GPIO32.
- [ ] Put the speaker on its own keyed 2-conductor lead from DFPlayer SPK1/SPK2.
- [ ] Put the Circuit Playground control on its own keyed 2-conductor lead:
  GPIO21/A1 plus shared GND. Keep Circuit Playground battery-positive isolated.
- [ ] Lay the six-pin HW-125 visual SD board flat on a short secured pigtail.
  Preserve VCC, GND, GPIO14/SCLK, GPIO27/MOSI, GPIO19/MISO, and GPIO5/CS.
- [ ] Move the red/green local buttons to the accessible end of the breadboard
  without changing GPIO13 scene and GPIO22 QR assignments.
- [ ] Use the breadboard power rails for organized distribution only after their
  split points and continuity are checked with a meter.
- [ ] Secure the breadboard in a shallow nonconductive pocket tray; keep USB-C,
  cards, and buttons accessible.
- [ ] Build the screen/diffuser/Circuit Playground lapel sandwich with a rigid
  backing and two-point garment attachment.
- [ ] Add strain relief at both ends of all three lapel leads.

## Functional verification

- [ ] Left pedal in Mode 5 launches one scene; right pedal toggles QR.
- [ ] Mode 1 Page Up/Down changes volume, displays the number, and survives reboot.
- [ ] Local scene and QR buttons each work for 25 consecutive presses.
- [ ] Run all 27 scenes and confirm each visual/audio pairing.
- [ ] Confirm Curb/S003 repeats more often while Shoes scenes separate every
  outside-source scene.
- [ ] Remove Card B and verify the flashed S018 visual fallback; reinstall Card B.
- [ ] Confirm the fallback plays track 0018 when Card A remains installed.
- [ ] Run 100 total triggers without queued accidental presses or resets.

## Wear and power tests

- [ ] Ten-minute fit test: walk, sit, bend, and turn.
- [ ] Gentle cable-pull test with the lapel badge supported.
- [ ] 30-minute worst-case screen + audio + halo test; check heat and resets.
- [ ] Five-hour event-duty battery test.
- [ ] Verify the DP20S does not shut off during idle.
- [ ] Check speaker intelligibility from the final mounting position.
- [ ] Pack USB cable, spare speaker, spare SD reader, and a known-good card backup.

## Later, not blocking the wearable

- [ ] Replace the breadboard with solderable perfboard only after the event build
  is reliable and the frozen wiring has been measured.
- [ ] Add a wired foot-switch fallback only if Bluetooth reliability testing
  shows a real need.
- [ ] Build the welcome-mat pressure-pad version after the event.
