# Right-pedal display cycle — September 27, 2026

In pedal Mode 5 (Space/Enter scene controls):

| Press from home | Result |
|---|---|
| First right press | QR code |
| Second right press | Silent Arc Core animation, looping |
| Third right press | Shoes Off, Dirtbag home image |
| Next right press | QR again |

Right during a normal scene stops playback and opens QR. From there the cycle
continues to Arc Core and home. Left plays normal scenes from home; left during
Arc Core leaves the loop and plays a scene. Mode 4's existing direct Arc Core
toggle is retained. Volume modes are unchanged.

The right press that stops Arc Core is consumed once and does not get queued
again as a new QR request. Returning home clears the earlier scene cooldown.
An unavailable Arc Core asset returns to home without getting stuck in QR.

Validation: the actual application control loop passes simulated tests for two
complete cycles, interruption of a normal scene, missing Arc media, the legacy
Mode 4 toggle, and left-pedal exit. The ESP32 build passes and was uploaded with
hash verification. Normal startup retains the 16 MHz card setting and 32-scene
catalog. The prior audiovisual timing repair is unchanged. Physical pedal-cycle
confirmation is pending reconnection/user presses.

Run the control test from `firmware/idf_wearable`:

```sh
python3 tests/host/test_display_cycle.py
```

The pre-change application and new binary are saved in
the owner’s local `right-pedal-cycle-2026-09-27` archive with SHA-256
checksums and the source diff. That archive is not included in this repository.
