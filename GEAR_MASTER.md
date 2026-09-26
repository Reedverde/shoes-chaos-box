# Gear Master

Audited: 2026-09-26

| System | Selected hardware | Status |
|---|---|---|
| Controller | 38-pin classic ESP32 DevKit | HAVE / WORKING |
| Display | GC9A01 240 x 240 round TFT | HAVE / WORKING |
| Halo | Circuit Playground Express | HAVE / SOURCE READY; FINAL REFLASH PENDING |
| Audio | DFPlayer Mini + one 4-ohm/3W speaker | HAVE / WORKING |
| Visual storage | HW-125/HiLetgo SPI reader + Card B | HAVE / WORKING AT 4 MHz |
| Audio storage | DFPlayer + Card A | HAVE / WORKING |
| Trigger | STRICH SPT-10 + GPIO13 local button | HAVE / WORKING |
| QR control | Right pedal + GPIO22 local button | IMPLEMENTED |
| Main power | Smatree DP20S | HAVE / RUNTIME TEST PENDING |
| Halo power | Existing clipped battery pack | HAVE / SELECTED |
| Pocket structure | Existing breadboard in shallow nonconductive carrier | CARRIER TO BUILD |
| Lapel structure | Screen + diffuser + Circuit Playground + backing | TO BUILD |
| Interconnect | Separate 8-pin display, 2-pin speaker, 2-pin halo leads | TO SOURCE/ASSEMBLE |

The first wearable deliberately retains the breadboard. Perfboard is a later
reliability upgrade, not a blocker. The two positive battery rails remain
isolated; only GPIO21/A1 and ground connect the ESP32 and Circuit Playground.
