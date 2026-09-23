# Build Plan

## Architecture

```text
momentary foot switch -- detachable 1/4 in cable --+
                                                     |
local mute / emergency button -----------------------+--> ESP32
                                                           |-- SPI --> TFT display
                                                           |-- SPI --> visual microSD
                                                           `-- UART -> DFPlayer Mini -> 4 ohm speaker

USB power bank (inside jacket pocket) ---------------------> regulated USB input/power rail
```

The display and visual microSD may share the SPI bus with separate chip-select lines. Exact GPIOs must be chosen only after the specific ESP32 board and both display pinouts are verified.

## Phase 0 — Confirm and label

- Photograph and identify the exact ESP32, GC9A01, and ST7735S boards.
- Confirm their operating voltages and pin labels.
- Inventory power bank, cables, cards, connectors, buttons, and enclosure materials.
- Label all confirmed components; do not assume similar-looking modules share a pinout.

**Exit:** Exact boards and missing purchases are known.

## Phase 1 — Display proof

- Power the ESP32 from USB on the bench.
- Wire the GC9A01 and render a solid-color test, text, then one 240 x 240 face asset.
- Repeat a representative caption layout on the ST7735S.
- Choose the screen by readability, comic effect, refresh speed, wiring, and enclosure fit.

**Exit:** One display is selected and can render the idle screen plus one scene reliably.

## Phase 2 — Audio proof

- Format a 4–32 GB microSD card as FAT32 for DFPlayer compatibility.
- Load a numbered test cue.
- Connect the DFPlayer Mini to an ESP32 UART and the speaker to the module’s speaker output according to the manufacturer pinout.
- Test startup, play, stop, volume, and repeated triggering.
- Keep audio short and locally stored; do not stream during the demo.

**Exit:** One command produces one clean audio cue without resetting the ESP32.

## Phase 3 — Controls

- Wire the momentary foot switch as a dry-contact input using an internal or external pull-up.
- Add software debounce and a scene-active lockout.
- Add the wearable mute/emergency-stop button.
- Define controls:
  - Foot switch: trigger a scene
  - Short local-button press: mute/unmute
  - Long local-button press: emergency stop / return to idle
  - Mode changes may initially be performed at boot or through a second owned button if needed

**Exit:** Deliberate presses trigger once; bounce, held presses, and accidental repeats do not.

## Phase 4 — Visual storage and scene engine

- Add the separate SPI microSD breakout with its own chip-select line.
- Establish the content folders described in `CONTENT_PLAN.md`.
- Load a manifest into memory at boot.
- Randomize within the current mode while avoiding immediate repeats.
- Pre-render animation frames instead of decoding complex GIFs live.
- Fail gracefully if either card is absent: show an error screen and preserve mute/stop behavior.

**Exit:** At least five scenes can be selected and played reliably from local storage.

## Phase 5 — Wearable integration

- Mock the layout in cardboard before committing to an enclosure.
- Put the screen and speaker grille on the visible face.
- Mount the ESP32, DFPlayer, and SD breakout so cards and USB remain serviceable.
- Put the power bank in an inside pocket; route a short USB cable inside the jacket.
- Use a panel-mounted 1/4-inch jack or equally robust detachable connector for the pedal.
- Add strain relief on both sides of the wearable, plus a slack loop.
- Ensure no sharp edges, exposed conductors, hot components, or rigid loads press into the wearer.

**Exit:** The unit can be worn, moved, muted, disconnected, and removed safely.

## Phase 6 — Reliability and rehearsal

- Run 100 trigger cycles and record failures.
- Test at expected volume and ambient noise.
- Confirm the power bank stays awake during a 30-minute idle/run test.
- Walk, sit, turn, and step away while wearing the full system.
- Perform a controlled cable snag/pull test with the wearable supported.
- Pack spare cards, USB cable, and a fallback manual trigger button.
- Rehearse Clean Demo mode first; enable Chaos/Roast only when appropriate.

**Exit:** Repeatable Tech Week demo with a documented fallback.

## Phase 7 — Post–Tech Week entryway conversion

- Add a normally closed magnetic door contact.
- Add one owned HC-SR501 PIR sensor.
- Trigger only after the door opens and presence/motion is observed within a defined window.
- Add a 2–5 minute cooldown and retain the foot switch as manual test/trigger control.
- Begin with USB wall power; battery charging hardware remains optional.
- A local ESP32 web page may later expose volume, mute, sensitivity, cooldown, and test controls.

