# Optional hardware regression test

Production builds do not run this test. Build explicitly with
`PLATFORMIO_BUILD_FLAGS=-DSHOES_TIMING_BENCH` to enable it.

The benchmark first reads 1,071 RAW files (1,055 catalog frames plus 16 Arc Core
frames) and compares FNV-1a hashes against the September 27 local staging pack.
It does not write the SD card. A read/hash failure stops the test and restores
the SD clock to 4 MHz. After a successful check it plays every catalog scene,
with audio and halo triggers, recording completion and worst read/draw/lateness
measurements after each scene. It then returns to the ordinary application.

`bench_expected.h` is a fixture for this specific pack, including the newer
8-frame Woody correction. It must be deliberately regenerated when assets
change. These are regression checksums, not cryptographic authentication.

Use the ordinary release build after testing so power-on does not rerun the
complete test. The release still emits one summary after each played scene;
there is no serial log for every frame.

## Right-pedal control regression

Run `python3 tests/host/test_display_cycle.py`. It compiles the actual
`app_task` control loop with simulated queue/GPIO/media boundaries and checks
two complete right-pedal cycles, right interruption of a normal scene, failed
Arc loading, the existing Mode 4 toggle, and left-pedal exit into a scene.
It does not simulate Bluetooth electrical/radio transport or the display.
