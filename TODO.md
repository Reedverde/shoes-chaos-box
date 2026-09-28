# Active Todo List

Updated: 2026-09-27

## Do next — preserve the working electronics

- [x] Mount Card A on the computer and replace `mp3/0001.mp3` with the corrected
  complete Damn Daniel catchphrase/laugh export from runtime-card-v4-prefetch.
- [x] Replace Card A `mp3/0029.mp3` with the corrected 13.01-second file from
  `staging_shoes_assets/runtime-card-v5-kling/audio/mp3/0029.mp3`; confirm
  “Gail, get the coats” and the final “psychotic” line play completely.
- [x] Replace Card B S008 with the bright, unobstructed eight-frame Woody scene
  and checksum-compare both mounted cards to runtime-card-v5 staging.
- [x] Store the silent Arc Core special on Card B under trigger key `ARCCORE`,
  outside the normal S001–S032 rotation.
- [x] Expand Arc Core from 8 frames at 630 ms to 16 frames at 315 ms, bind
  either Bluetooth pedal in Mode 4 to start/stop it, and flash the ESP32.
- [x] Build the extended 35.04-second Arc Core media: 5.04-second startup plus
  30 seconds of source-frame blue pulsing, using all 48 supported frames.
- [x] Reconnect Card B and copy/checksum the extended Arc Core media: 97 special
  files, 48 frames, and the 35.04-second timeline match staging.
- [x] Add and install the synchronized Arc Core halo: seven accelerating gold
  rotations, a full gold ignition glow, direct transition to blue, and four
  dramatic breathing pulses.
- [x] Rebuild the Arc Core display pulse from the user's two scrub marks: use
  the actual Kling frames from 3.68 seconds (calm) to 3.10 seconds (bright),
  play them backward and forward four times, and stage the Card B package.
- [x] Install the scrub-mark Arc Core package on Card B and verify all 97
  scene files plus `SPECIAL.CSV` against staging by checksum.
- [x] Repair the missed Arc halo start after controller restarts: re-send the
  special command at every complete display loop, recover from an already-high
  Arc line at halo boot, install both firmwares, and confirm the live
  `HALO_ARCCORE` active/stop trace.
- [x] Preserve normal scene progress through QR, Arc Core, and ESP32 restarts
  by saving the full randomized deck and next-scene cursor in NVS after every
  normal scene selection; test the overlay cursor behavior and install it.
- [x] Double-press Circuit Playground RESET until `CPLAYBOOT` appears.
- [x] Flash the built diffuser-optimized halo firmware and verify checksum.
- [ ] Trigger S018-S027 and approve each profile through the real diffuser.
- [ ] Photograph and label every current breadboard connection before moving a
  single wire.
- [ ] Mark the two cards permanently: **A AUDIO / DFPLAYER** and
  **B VISUAL / HW-125**.
- [ ] Cold-boot with both cards installed and verify screen, buttons, speaker,
  pedal, QR, and halo.
- [x] Replace Card B with `staging_shoes_assets/runtime-card-v5-kling/visual`;
  Card A stays unchanged because the Kling scenes reuse existing tracks.
- [x] Flash the 32-scene ESP32 firmware and verify all flash regions by digest.
- [ ] Verify S030/0003, S031/0020, S032/0023, and the revised Arc Core loop on the
  actual screen after reinstalling both cards.

## Convert the bench build into the wearable

- [ ] Preserve and photograph the current A1-start layout in
  `firmware/BENCH_REWIRE_PROGRESS.md` before mechanical conversion. The older
  column-0 plan is superseded; column 20 now carries SPI signals, not ground.
- [ ] Establish separate +5 V and +3.3 V red rails and common blue ground rails;
  meter-test them before attaching any module.
- [ ] Move the display off the breadboard onto one keyed 8-conductor lead:
  3V3, GND, GPIO14, GPIO27, GPIO26, GPIO25, GPIO33, GPIO32.
- [ ] Put the speaker on its own keyed 2-conductor lead from DFPlayer SPK1/SPK2.
- [ ] Put the Circuit Playground control on its own keyed 2-conductor lead:
  GPIO21/A1 plus shared GND. Keep Circuit Playground battery-positive isolated.
- [ ] Lay the six-pin HW-125 visual SD board flat on a short secured pigtail.
  Preserve VCC, GND, GPIO14/SCLK, GPIO27/MOSI, GPIO19/MISO, and GPIO5/CS.
- [ ] Move the blue/green local buttons to the accessible end of the breadboard
  without changing GPIO13 forward and GPIO22 previous-scene assignments.
- [ ] Use the breadboard power rails for organized distribution only after their
  split points and continuity are checked with a meter.
- [ ] Secure the breadboard in a shallow nonconductive pocket tray; keep USB-C,
  cards, and buttons accessible.
- [ ] Build the screen/diffuser/Circuit Playground lapel sandwich with a rigid
  backing and two-point garment attachment.
- [ ] Add strain relief at both ends of all three lapel leads.

## Functional verification

- [ ] Put the pedal in Mode 4; confirm either pedal starts the silent Arc Core
  loop, the gold chase completes seven accelerating rotations, the complete
  ring blooms gold, the dramatic 30-second blue pulse follows, the full cycle
  repeats cleanly, and either
  pedal stops it.
- [ ] Left pedal in Mode 5 launches one scene; right pedal cycles QR → Arc Core
  → home. Verify two complete cycles and right-pedal interruption of a scene.
- [ ] Mode 1 Page Up/Down changes volume, displays the number, and survives reboot.
- [ ] Local forward and previous buttons each work for 25 consecutive presses;
  verify blue, blue, green, blue returns to the same second scene.
- [ ] Run all 32 scenes and confirm each visual/audio pairing, including all
  four Curb variants and all three Kling AI-generated visuals.
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

- [ ] Add a wired dedicated Arc Core button only if a separate physical control
  is still wanted after the Mode 4 pedal test.

- [ ] Replace the breadboard with solderable perfboard only after the event build
  is reliable and the frozen wiring has been measured.
- [ ] Add a wired foot-switch fallback only if Bluetooth reliability testing
  shows a real need.
- [ ] Build the welcome-mat pressure-pad version after the event.
