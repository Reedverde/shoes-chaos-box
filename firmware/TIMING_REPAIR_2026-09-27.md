# Timing repair — September 27, 2026

The repair passed the full-catalog test on the connected hardware. The source audit is in
the earlier local audit (summarized in [the publication audit](../docs/AUDIT.md)). This addendum distinguishes
measured repair results from that earlier audit's hypotheses and estimates.

## Measured cause and repair

A complete 240×240 RGB565 image is 115,200 bytes. The original buffered file
reader at 4 MHz took an average 372.414 ms per image in S021, plus 35.008 ms to
draw. That is incompatible with S021's 100–300 ms frame holds. Merely labeling
this operation “prefetch” cannot hide a read longer than the available hold.

| Reader | Actual SD clock | Mean read | Mean draw | Integrity result |
|---|---:|---:|---:|---|
| Previous buffered stdio | 4 MHz | 372.414 ms | 35.008 ms | Baseline, 36 frames |
| Direct file reads | 4 MHz | 276.506 ms | 35.012 ms | 36 matching hashes |
| Direct file reads | 8 MHz | 155.542 ms | 35.005 ms | 36 matching hashes |
| Direct file reads | 16 MHz | 95.111 ms | 35.004 ms | 36 matching hashes |
| Direct file reads | 20 MHz | Failed on first image | — | SD error 0x108 |

The repair uses direct reads and switches to 16 MHz after initializing the
card at 4 MHz. It does not use 20 MHz. Reads still validate exact frame size
and handle short reads. The same two half-frame buffers hold a complete image
before drawing. No original frames or media files were removed or changed.

The earlier theoretical 8 MHz lower bound omitted filesystem/protocol overhead.
The physical test measured 155.542 ms at that rate. A schedule simulation using
these actual per-frame timings predicts 6.75 seconds for S021 at 8 MHz versus
6.00 at 16 MHz. This is a simulation, not a recorded 8 MHz audiovisual test.

Per-frame serial logging was removed from playback. A single summary after each
scene reports frame count, elapsed time, worst read/draw duration, and worst
frame-start lateness. The normal build excludes the automatic benchmark.

## Physical media check

All **1,071 RAW images** on Card B matched the staging fixture at 16 MHz:
1,055 frames in S001–S032 plus 16 in the Arc Core special. This includes the
newer eight-frame S008 Woody correction. The initial audit counted 1,083 normal
frames before that 28-frame reduction was introduced separately; this repair
preserved that existing correction and did not change the pack.

These are full-byte FNV-1a comparisons, suitable for detecting transfer/content
regressions, not cryptographic authentication. The physical audio card was not
mounted or rehashed during this repair. Earlier project records describe its
separate synchronization; serial playback commands cannot verify acoustic output.

## LED repair

- Build every LED pattern in memory and transmit it once. The previous use of
  the convenience pixel setter transmitted partially cleared/redrawn states.
- Separate persistent spatial rotation from scene-local phrase/beat time.
  S031 now returns to its cyan opening each time instead of getting stuck on
  the final phrase after the global counter advances.
- Calculate position from elapsed time and interpolate between LED positions.
  Late rendering catches up; ring position is retained between scene starts.
- Keep the current palettes, brightness limits, and two-second afterglow.
  Retained 45/25 ms effect tempos are not described as an exact recreation of
  the earliest prototype.

The real source passed host checks for phrase restart, phase preservation,
last-to-first LED interpolation, delayed-loop catch-up, timer wraparound, and
one strip transmission per frame across all 32 scenes. Both firmware builds
compiled successfully.

SAM-BA failed after erasing the Circuit Playground application. Recovery used
an application-only UF2 at 0x2000. Readback matched all but the first 256 bytes;
inspection of the installed SAMD core showed that its 1200-baud reset deliberately
erases that startup page. Reinstalling the whole UF2 restored the page. The
running board then acknowledged scene commands over USB. UF2 encoding followed
the [official format specification](https://github.com/microsoft/uf2/blob/master/README.md).

## Preserved behavior and corrected records

Arc Core, the recent Woody media correction, pedal modes, local buttons, volume
persistence, QR behavior, and scene order were preserved. No breadboard wire or
physical card file was modified. The current wiring record now reflects A1 as
the ESP start, separate SPI junctions in column 20, and the repaired shared
ground. The old column-0/ground-20 plan is marked historical.

## Remaining limits

Finishing at the programmed scene duration does not mean every frame transition
hits its deadline. A ~95 ms read plus ~35 ms draw still exceeds a 50–100 ms
hold. The player preserves all frames and catches up during longer holds; small
transition jitter remains measurable. Eliminating it would require a further
change such as lossless compressed/delta media, additional buffering, or faster
reliable hardware, followed by another integrity and timing test.

DFPlayer onset and actual audio finish are not timestamped by the hardware.
The fixed 120 ms startup allowance and the earlier first-frame/halo handshake
ordering remain. Reed observed the automatic test and reported that the scenes were ending
correctly ("they end correct"). This supports practical audiovisual alignment
for the scenes observed, without claiming instrumented acoustic timestamps or
a separate diffuser assessment. The automated catalog test exercises
Bluetooth-enabled firmware but does not reproduce repeated concurrent discovery
or every pedal/button interaction; those belong to final operational testing.

The optional Worker playlist/catalog incompatibility, malformed/oversized CSV
handling, halo packet timeout, and trigger queue priority findings remain in the
audit as separate follow-up work. They were not presented as the cause of this
measured storage bottleneck and were not silently rewritten in this repair.

## Playback results

The physical run completed all **32 scenes**, displaying **1,055/1,055**
catalog frames. All **32 halo scene IDs** were received correctly, in order.
Thirty-one scenes completed 10 ms beyond their programmed duration (the summary
includes sending the DFPlayer stop command). S022 completed 58 ms beyond its
6.050-second target. Worst frame-start lateness across the catalog was 64.362 ms.
No card read failure or device reset occurred in this run.

S020 and S021 each displayed all 36 frames and completed in **6.010 seconds**
against a 6.000-second target. Wizard S002 displayed all 38 frames in 6.260
seconds against 6.250. S029 retained its full 13.010-second program and completed
in 13.020. Reed reported during the test that the scenes were ending correctly.

| Scene | Frames shown | Target (ms) | Measured (ms) | Worst frame-start lateness (ms) |
|---|---:|---:|---:|---:|
| S001 | 36/36 | 6000 | 6010 | 62.075 |
| S002 | 38/38 | 6250 | 6260 | 62.324 |
| S003 | 36/36 | 6000 | 6010 | 61.769 |
| S004 | 36/36 | 6000 | 6010 | 61.912 |
| S005 | 36/36 | 6000 | 6010 | 61.760 |
| S006 | 38/38 | 6250 | 6260 | 61.961 |
| S007 | 36/36 | 6000 | 6010 | 61.707 |
| S008 | 8/8 | 6000 | 6010 | 0.000 |
| S009 | 36/36 | 6000 | 6010 | 63.011 |
| S010 | 36/36 | 6000 | 6010 | 62.667 |
| S011 | 36/36 | 6000 | 6010 | 62.941 |
| S012 | 36/36 | 6000 | 6010 | 62.153 |
| S013 | 36/36 | 6000 | 6010 | 62.146 |
| S014 | 36/36 | 6000 | 6010 | 61.798 |
| S015 | 36/36 | 6000 | 6010 | 63.404 |
| S016 | 36/36 | 6000 | 6010 | 64.009 |
| S017 | 36/36 | 6000 | 6010 | 63.392 |
| S018 | 36/36 | 6000 | 6010 | 63.337 |
| S019 | 36/36 | 6000 | 6010 | 63.447 |
| S020 | 36/36 | 6000 | 6010 | 63.343 |
| S021 | 36/36 | 6000 | 6010 | 63.507 |
| S022 | 37/37 | 6050 | 6108 | 63.495 |
| S023 | 36/36 | 6000 | 6010 | 64.362 |
| S024 | 36/36 | 6000 | 6010 | 64.173 |
| S025 | 36/36 | 6000 | 6010 | 63.818 |
| S026 | 40/40 | 6700 | 6710 | 64.310 |
| S027 | 36/36 | 6000 | 6010 | 63.328 |
| S028 | 35/35 | 7000 | 7010 | 0.000 |
| S029 | 43/43 | 13010 | 13020 | 0.000 |
| S030 | 8/8 | 6000 | 6010 | 0.000 |
| S031 | 8/8 | 6000 | 6010 | 0.000 |
| S032 | 8/8 | 6000 | 6010 | 0.000 |

Public per-scene evidence: [CSV](../docs/validation/scene-results.csv) and [JSON](../docs/validation/scene-results.json). Raw serial logs and recovery binaries remain in the owner’s local workspace.

## Final installed state and recovery

The normal ESP32 application was installed at 0x10000 and its upload hash
verified. Startup confirmed `VISUAL_SD_READY khz=16000`, all 32 catalog scenes,
ordinary button/pedal readiness, and a successful Bluetooth pedal connection.
The release binary contains neither automatic diagnostic entry point. Both
serial captures were closed after verification.

Recovery copies are `esp32-before.bin` (matched against the pre-repair device
application with esptool), `halo-before.bin` (saved prior build), and the new
`esp32-repaired.bin` / `halo-repaired.uf2`. `firmware-sha256.json` records the
binary checksums. The unsuccessful full-flash read is not represented as a
complete ESP32 backup. The ESP32 recovery copy covers the application; the
bootloader, partition table, and NVS were not replaced by this repair.

The full-catalog test used the same repaired playback functions as the release.
Only the optional startup test and its fixture are excluded from the normal
build. Source changes remain in the existing working checkout, preserving
pre-existing Arc Core and media work.

The owner’s local `repair-2026-09-27` archive holds full logs and recovery binaries; it is not a folder in this Git repository. Later Arc Core and control changes are recorded separately in PROJECT_STATE.md and CHANGELOG.md.
