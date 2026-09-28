---
status: assembled_under_test
last_reviewed: 2026-09-27
---

# Current breadboard record

This supersedes the column-0 layout in `WIRING_FINAL_POCKET_BREADBOARD.md`.
Reed confirmed that the ESP32 starts at A1. These are the latest session
connection records, not a new photograph or electrical measurement. Leave the
working wiring in place during timing tests.

## Power and shared ground

| ESP pickup | Signal | Destination |
|---|---|---|
| A1 | VIN / 5 V | 5 V red rail |
| A6 | GND | Shared ground rail |
| A19 | 3V3 | Separate 3.3 V red rail |

Reed installed a ground-to-ground rail link and repaired an unbridged ground
section; both buttons subsequently worked. Retain those working ground links.
Rails can be split halfway along the board. The two positive rails carry
**different voltages** and must remain separate. Earlier reported readings were
4.77 V and 3.27 V; these are not fresh audit measurements.

**Column 20 is now a signal junction, not ground.** A–E20 carries SCK from
A8; F–J20 carries MOSI from A9. The two five-hole groups are separated by the
center trench. Do not follow the old A20 ground-breakout instruction.

## Recorded signal connections

| Module connection | ESP signal / pickup |
|---|---|
| Storage CS, F34 (accessible H34) | GPIO5 / J10 |
| Storage SCK, F35 (accessible H35) | GPIO14 / A8, via A–E20 |
| Storage MOSI, F36 (accessible H36) | GPIO27 / A9, via F–J20 |
| Storage MISO, F37 (accessible H37) | GPIO19 / J12; user confirmed continuity |
| Storage VCC, F38 | 5 V; user confirmed continuity to A1 |
| Storage GND, F39 | Shared ground |
| Display GND, J43 (accessible F43) | Shared ground |
| Display VCC, J44 (accessible F44) | 3.3 V rail |
| Display SCL, J45 (accessible F45) | GPIO14 / A8, via A–E20 |
| Display SDA, J46 (accessible F46) | GPIO27 / A9, via F–J20 |
| Display RST, J47 (accessible F47) | GPIO33 / A12 |
| Display DC, J48 (accessible F48) | GPIO25 / A11 |
| Display CS, J49 (accessible F49) | GPIO26 / A10 |
| Display BL, J50 (accessible F50) | GPIO32 / A13 |
| DFPlayer VCC, H21 | 5 V rail |
| DFPlayer RX, H22 | GPIO17 / J9 through 1 kΩ at C48–C50 |
| DFPlayer TX, H23 | GPIO16 / J8 |
| DFPlayer SPK1 / SPK2, H26 / H28 | The two speaker leads |
| DFPlayer GND, H27 | Shared ground |
| Circuit Playground A1 | GPIO21 / J14, via channel 51 |
| Circuit Playground GND | Shared ground, via channel 52 |

The storage module lies over A–E32–41; do not prescribe hidden jumper holes.
The DFPlayer starts C/H21 with its card entry toward the higher numbers.
Button locations were reported D/G56–58 and D/G61–62. Color labels changed in
conversation; firmware is unambiguous: GPIO13 advances and GPIO22 goes back.
Check physical label numbering before relocating anything: the board's two
printed scales run in opposite directions.

The working speaker was reported 4Ω. Software cannot measure its impedance or
verify acoustic output. Power both controllers by USB for the bench; they share
ground and the halo signal, not their positive supply rails.
