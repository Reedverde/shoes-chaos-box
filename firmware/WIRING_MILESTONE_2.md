# Wiring Milestone 2 — synchronized LED halo

This extends the verified Milestone 1 screen/button/pedal build with the Adafruit Circuit Playground Express LED halo.

## Functional connections

| ESP32 | Circuit Playground Express | Purpose |
|---|---|---|
| GPIO21 | A1 | One-wire scene ID plus active-scene signal |
| GND | GND | Shared signal reference |

For the exact verified breadboard placement, the ESP32 occupied B/I across columns 1–19 with its USB connector on the left:

- `J14` connected GPIO21 to Circuit Playground `A1`.
- `J19` connected ESP32 GND to Circuit Playground `GND`.

## Bench power rule

- Keep the ESP32 and Circuit Playground connected to their own USB cables during this proof.
- Do not connect the two boards' 3.3V, 5V, or VOUT power rails together.
- The shared ground is required for the GPIO21-to-A1 trigger to work reliably.

## Wearable lead

For the first wearable, replace the two loose alligator/Dupont connections with
one keyed 2-conductor lead: GPIO21/A1 and shared GND. Circuit Playground remains
on its own clipped battery. Do not add a positive-power conductor to this lead.

## Verified behavior

At idle, two opposite Circuit Playground pixels glow cyan. An accepted
local-button or Bluetooth-pedal trigger sends the scene ID over GPIO21/A1 and
then holds the line high. The Circuit Playground runs that scene's matching
scene-aware animation, fades for two seconds, and returns to cyan idle. The
S018-S027 source now uses ten different diffuser-aware profiles; final physical
flash and review are still pending.
