# Build Plan

## Architecture

```text
Bluetooth music pedal (preferred if compatible) -----------+
detachable wired foot switch (fallback) --------------------+--> ESP32
local manual-trigger button --------------------------------+
local mute / emergency button ------------------------------+     |-- SPI --> center TFT display
                                                                 |-- SPI --> visual microSD
                                                                 |-- UART -> DFPlayer Mini -> 4 ohm speaker
                                                                 `-- trigger/sync -> Circuit Playground LED halo

Smatree DP20S USB-C pack, 5V/2A (inside jacket pocket) ----------> shared 5V power distribution
```

The display and visual microSD may share the SPI bus with separate chip-select lines. Exact GPIOs must be chosen only after the specific ESP32 board and both display pinouts are verified.

## Phase 0 — Confirm and label

- Controller identified from photo as a classic 38-pin ESP32 DevKit-style board with CP2102 USB-UART and Micro-USB; confirm it accepts a basic firmware upload and record detected flash size.
- Confirm their operating voltages and pin labels.
- Find and identify the Bluetooth music pedal and document whether it emits BLE MIDI, BLE keyboard/page-turn keys, or another protocol.
- Select a short USB-C-to-Micro-USB lead and safe 5V distribution method for the confirmed Smatree DP20S pack; do not route the full system load through an unverified ESP32 regulator path.
- Inventory cards, connectors, buttons, and rough mounting materials.
- Label all confirmed components; do not assume similar-looking modules share a pinout.

Controller pin guardrails for the first wiring draft:

- Reserve GPIO18/19/23 for the shared SPI bus unless a library constraint requires otherwise.
- Reserve GPIO16/17 as the preferred hardware UART pair for the DFPlayer.
- Do not use the exposed CLK/CMD/SD0/SD1/SD2/SD3 pins; they are tied to the ESP32 module's flash interface.
- Treat GPIO34/35/36/39 as input-only.
- Avoid GPIO0/2/5/12/15 for the first peripheral assignments because their levels can affect boot behavior.
- Freeze chip-select, display-control, button, and Circuit Playground communication pins only after the display and microSD modules are on the bench together.

**Exit:** Exact boards and missing purchases are known.

## Phase 1 — Display proof

- Power the ESP32 from USB on the bench.
- Wire the GC9A01 and render a solid-color test, text, then one 240 x 240 face asset.
- Repeat a representative caption layout on the ST7735S.
- Choose the screen by readability, comic effect, refresh speed, wiring, and enclosure fit.

**Exit:** One display is selected and can render the idle screen plus one scene reliably.

## Phase 1B — LED halo proof

- Place the Circuit Playground Express behind the selected center display without permanent attachment.
- Confirm that enough of its ten NeoPixels remain visible to produce a ring and shirt backlight.
- Prototype idle, trigger, alert, spin/chase, and fade patterns at conservative brightness.
- Decide whether the ESP32 sends a simple trigger line or serial scene/color data to the Circuit Playground.
- Power-test the display, audio, and LED halo together from the DP20S; stay below its labeled 5V/2A output and avoid sustained full-white NeoPixel output.

**Exit:** One scene synchronizes the screen with a visible LED sweep without brownouts, uncomfortable glare, or excessive heat.

## Phase 2 — Audio proof

- Format a 4–32 GB microSD card as FAT32 for DFPlayer compatibility.
- Load a numbered test cue.
- Connect the DFPlayer Mini to an ESP32 UART and the speaker to the module’s speaker output according to the manufacturer pinout.
- Test startup, play, stop, volume, and repeated triggering.
- Keep audio short and locally stored; do not stream during the demo.

**Exit:** One command produces one clean audio cue without resetting the ESP32.

## Phase 3 — Controls

- Use an owned pushbutton as the initial bench trigger.
- If the Bluetooth pedal is compatible, pair it and map its event to the same trigger interface used by the button.
- Build the detachable wired dry-contact switch only as the dependable fallback; use an internal or external pull-up.
- Add software debounce/reconnect handling and a scene-active lockout.
- Add the wearable mute/emergency-stop button.
- Define controls:
  - Bluetooth or wired foot trigger: launch a scene
  - Dedicated wearable trigger button: launch the same scene path without a pedal
  - Mute/stop button short press: mute/unmute
  - Mute/stop button long press: emergency stop / return to idle
  - Mode changes may initially be performed at boot or through a second owned button if needed

**Exit:** Deliberate presses trigger once; bounce, held presses, reconnects, and accidental repeats do not. At least one removable trigger works without touching the wearable.

## Phase 4 — Visual storage and scene engine

- Add the separate SPI microSD breakout with its own chip-select line.
- Establish the content folders described in `CONTENT_PLAN.md`.
- Load a manifest into memory at boot.
- Randomize within the current mode while avoiding immediate repeats.
- Pre-render animation frames instead of decoding complex GIFs live.
- Fail gracefully if either card is absent: show an error screen and preserve mute/stop behavior.

**Exit:** At least five scenes can be selected and played reliably from local storage.

## Phase 4B — Event-host integrations (stretch)

- Use Kling AI to create a small number of short reaction loops from cleared source images or original artwork.
- Convert the results into the exact frame format and dimensions selected in Phase 1; do not decode or stream Kling output at runtime.
- Build a minimal Cloudflare Flue “Chaos Director” that accepts a theme or tone and produces a validated scene manifest using only known local asset IDs.
- Export/download the manifest and content pack before boarding.
- Demonstrate the Cloudflare/Kling provenance if useful, but keep a known-good hand-curated pack as the default and fallback.

**Exit:** At least one Kling-assisted local loop and one Flue-generated scene manifest run on the device without network access.

## Phase 5 — Wearable integration

- Mock the layout with cardboard, exposed backing, tape, clips, visible wire, and other intentionally rough materials before committing to mounting.
- Center the outward-facing screen over/within the Circuit Playground LED ring, with the LEDs able to wash the shirt.
- Preserve the purposefully slapped-together look; do not hide every board, fastener, or wire.
- Mount the ESP32, DFPlayer, and SD breakout so cards and USB remain serviceable.
- Put the cylindrical DP20S power bank in an inside pocket and restrain it from rolling; route a short USB-C power lead inside the jacket.
- If using the wired fallback, use a panel-mounted 1/4-inch jack or equally robust detachable connector and add strain relief plus a slack loop.
- Ensure no sharp edges, exposed conductors, hot components, or rigid loads press into the wearer.
- Keep roughness theatrical only: insulation, load spreading, secure pins/clips, and emergency control remain non-negotiable.

**Exit:** The unit can be worn, moved, muted, disconnected, and removed safely.

## Phase 6 — Reliability and rehearsal

- Run 100 trigger cycles and record failures.
- Test at expected volume and ambient noise.
- Confirm the DP20S stays awake during a 30-minute idle/run test and remains stable during worst-case screen + audio + LED peaks.
- Run a five-hour event-profile soak test: mostly idle/low-brightness operation with repeated 1–2 minute demonstrations and no charging.
- Walk, sit, turn, and step away while wearing the full system.
- Perform a controlled cable snag/pull test with the wearable supported.
- Pack spare cards, USB cable, and a fallback manual trigger button.
- Rehearse Clean Demo mode first; enable Chaos/Roast only when appropriate.
- Rehearse the Hack Alcatraz format: explain the welcome-mat premise, trigger two or three randomized scenes, and finish inside two minutes.
- Pack the wearable fully charged and self-contained because reception and access to power may be unreliable on the boat.

**Exit:** Repeatable offline boat demo, five-hour measured battery endurance, and a documented trigger/power fallback.

## Phase 7 — Home welcome-mat version

- Put a suitable low-voltage pressure pad beneath the welcome mat as the primary arrival trigger.
- Treat the wearable pedal and welcome-mat pressure pad as interchangeable inputs to the same scene engine.
- Optionally add a normally closed magnetic door contact and one owned HC-SR501 PIR sensor to confirm an arrival.
- Trigger only after the chosen combination of mat/door/presence events occurs within a defined window.
- Add a 2–5 minute cooldown and retain the foot switch as manual test/trigger control.
- Begin with USB wall power; battery charging hardware remains optional.
- A local ESP32 web page may later expose volume, mute, sensitivity, cooldown, and test controls.
