---
status: in_progress
last_reviewed: 2026-09-26
review_after: 2026-09-27
---

# Current physical breadboard rewire

Reed's latest photo shows the cleared breadboard with only the ESP32 seated,
USB connector pointing left. Reed explicitly confirmed that the ESP32 starts
at **A1**, not A0. Keep this placement for the ongoing guided rewire.

The target document `WIRING_FINAL_POCKET_BREADBOARD.md` assumes a start at
column 0. For the current placement, its A-side power pickup coordinates
therefore shift by one:

| Current pickup | Pin from saved wiring map | Destination |
|---|---|---|
| A1 | VIN / 5V | Top red rail |
| A6 | GND | Top blue rail |
| A19 | 3V3 | Bottom red rail |

Also connect top blue ground to bottom blue ground. Keep the two red rails
separate. Use rail sections beside the ESP32 until rail continuity is checked.

These are the next instructions, not confirmed installed wiring. Power remains
disconnected during rewiring. Pin identities above come from the saved wiring
map plus Reed's confirmed one-hole offset, not a readable close-up of the pin
labels. Do not claim the rail voltages or continuity have been measured.

The printed number scales on opposite edges run in opposite directions;
confirm the physical J-side numbering before giving J-hole instructions.
Peripheral positions and the ground breakout have not yet been installed.
