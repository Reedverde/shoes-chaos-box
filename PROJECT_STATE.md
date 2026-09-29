# Project State

Last updated: 2026-09-29 (full audit: September 27)

## Current result

**Shoes Off, Dirtbag!** is a working two-controller proof of concept. The ESP32
drives the round display, local forward/previous buttons, visual microSD, DFPlayer/speaker,
and STRICH Bluetooth pedal. The Circuit Playground Express supplies the lapel
halo. Reed confirmed the final physical assembly on September 29 and supplied
a photo; placement in the bag remains. Movement and battery-endurance checks
below are still separate acceptance tests.

September 29 follow-up: Reed reported an occasional missed initial Arc light
start and distorted audio at higher volume. A subsequent live trace confirmed
both the ESP32 Arc command and the halo's immediate acknowledgement. A quieter
audio trial sounded clean; supply/speaker/connection causes remain unisolated.
New home-logo and QR lighting is installed on both controllers and described
in the halo firmware README. Builds and host tests passed; live logs confirm
the home command was received. Final visual approval and the new QR/Arc/home
pedal check remain pending.

## Verified or implemented

- GC9A01 240 x 240 display, local forward and previous-scene buttons, DFPlayer,
  speaker, visual SD reader, both 8 GB cards, and Bluetooth pedal are connected.
- Card A contains `mp3/0001.mp3` through `0029.mp3`. Card B now contains the
  runtime-card-v5 visual exports for scenes S001-S032. Both mounted cards were
  checksum-compared to the v5 staging source after the 2026-09-27 synchronization.
  Card A has the complete 13-second S029/0029 Curb exchange, and Card B has the
  bright eight-frame S008 Woody replacement. S030-S032 reuse audio 0003, 0020,
  and 0023, so Card A needs no additional tracks.
- Card B also contains the silent named special `ARCCORE` at
  `SPECIAL/ARCCORE/FRAMES.CSV`. It is excluded from the random scene deck and
  now uses all 48 supported frames: a 5.04-second startup
  followed by 30 seconds of real source motion moving between calm and bright
  frames. In pedal Mode 4,
  either pedal starts the loop and either pedal stops it on the next click. Its
  revised 97-file, 35.04-second package plus `SPECIAL.CSV` is installed on
  Card B and checksum-matches staging.
- Arc Core sends its own long-sync command over GPIO21/A1 without consuming a
  normal scene code. The Circuit Playground accelerates a gold chase through
  seven revolutions, closes into a bright gold ring, transitions directly to
  blue at 5.04 seconds, then breathes blue four times with a wide intensity
  range during the 30-second powered-on section. The display pulse uses the
  actual Kling source frames between the user's bright 3.10-second and calm
  3.68-second scrub marks, playing them backward and forward instead of
  modifying a frozen final frame.
- Both controller firmwares are installed; the ESP32 flash matches
  the built image and the Circuit Playground accepted and rebooted from UF2.
  Arc Core lighting now re-synchronizes at every 35.04-second visual loop, and
  the halo recovers when it boots while the Arc control line is already high.
  A live hardware trace confirmed `HALO_ARCCORE`, active high hold, and clean
  return to idle on the stop signal.
- The left pedal launches a scene. In Mode 5, successive right-pedal presses
  cycle QR (`https://reedverde.com/sf`) → silent Arc Core loop → Shoes Off,
  Dirtbag home. Right during normal playback interrupts into QR. The existing
  Mode 4 direct Arc Core toggle is also retained.
- The blue local button advances through the current randomized order. The green
  local button walks backward through that same history, so stepping back and
  then pressing blue replays the scene that followed it. Both board buttons
  bypass the remaining pedal cooldown after playback; pedal presses are still
  discarded during the ten-second anti-repeat window.
- The entire 48-play randomized deck and its next-scene cursor are saved in
  ESP32 NVS after every normal scene selection. QR and Arc Core are temporary
  overlays that leave this cursor unchanged, and power cycles resume at the
  next queued scene instead of rebuilding from the beginning.
- Pedal Mode 1 Page Up/Down changes DFPlayer volume from 0-30, shows the value
  for 1.2 seconds, and saves it in NVS. Default volume is 18.
- Scene order alternates an outside-source scene with a Shoes-family scene.
  The four Curb variants S003/S028/S029/S030 appear twice each in the 48-play
  deck; all other outside scenes appear once; the 12 separators are shuffled.
- Visual frames use direct file reads into two 57,600-byte buffers. The card
  initializes at 4 MHz then runs at the tested 16 MHz rate; display transfers
  retain their configured 32 MHz clock. The September 27 repair cut average
  S021 reads from 372 ms to 95 ms. A 20 MHz test failed and is not used.
- The opening image, QR image, and six-frame S018 master fallback are stored in
  ESP32 flash. Fallback audio is correctly mapped to DFPlayer track 0018 and
  therefore still requires Card A.
- GPIO21 sends a five-bit scene ID plus active-scene state to Circuit Playground
  A1. The halo source now uses every available code for 32 scene palettes,
  including dedicated Kling confrontation, verdict, and bacteria-rave effects.

## Timing repair validation — September 27

The connected Card B passed full-byte checksums for 1,071 RAW files, including
1,055 normal scene frames and the then-installed 16 Arc Core frames. The later
48-frame Arc Core replacement added 32 RAW/BMP pairs; all 97 files in its
special folder and `SPECIAL.CSV` were independently checksum-verified. All 32 normal scenes then
played with every frame shown and the halo acknowledged every matching scene ID.
Thirty-one scenes completed within 10 ms of their programmed duration; S022 was
58 ms over. S020/S021 both finished in 6.010 seconds, and Reed confirmed that
the observed screen/audio endings were correct. Individual frame starts still
show up to 64.362 ms jitter; this is not a claim of sample-accurate audio sync.

The LED repair batches each complete pattern into one hardware update and
separates continuous rotation from scene-local phrase timing. Its actual source
passes regression tests for all 32 profiles, wraparound, phrase restart, and
elapsed-time catch-up. Normal ESP32 builds exclude automatic diagnostic playback. The repaired release
was uploaded with hash verification, booted at 16 MHz with 32 scenes, and
reconnected to the Bluetooth pedal.

Arc Core image integrity was checked; its interactive start/stop loop, repeated
pedal/button tests, battery operation, and diffuser approval remain operational
checks. Existing card synchronization records above predate this repair; no
physical media file or breadboard connection was changed during it.

See `firmware/TIMING_REPAIR_2026-09-27.md` for measured results and remaining limits.

## Public project packaging

The repository now includes newcomer setup, an audit, published timing results,
and a synthetic two-card starter pack. Cloudflare uses Workers with Static
Assets for downloads and deterministic playlist previews; Flue is not a running
dependency. Playlist previews are not imported by the firmware. The canonical
32-entry catalog and new starter validator preserve the positional halo IDs.
Public-site deployment status is recorded in [the publication handoff](docs/PUBLICATION.md).
The GitHub package and project story are published. The [Cloudflare library](https://shoes-chaos-director.reed-5c2.workers.dev/)
was published September 28, 2026 through the official MCP/API connection.
Live API, catalog, playlist, ZIP, download headers, and checksum checks passed.

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

- Visually approve the installed lighting profiles through the actual diffuser.
- Verify left pedal = scene and two complete right-pedal QR → Arc Core → home
  cycles after a cold boot.
- Verify Mode 1 volume up/down and saved volume after reboot.
- Exercise all 32 visual mappings and confirm the three reused audio mappings.
- Test cardless ESP fallback, then reinstall both cards.
- Run 100 triggers, a 30-minute movement/load test, and a five-hour event-profile
  battery test.
- Measure speaker intelligibility from the final pocket/lapel position.

The active task list is in [TODO.md](TODO.md); the reduced procurement list is
in [PURCHASE_LIST.md](PURCHASE_LIST.md).
