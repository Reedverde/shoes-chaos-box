# Project State

Last updated: 2026-09-22

## Status

**Phase:** Documentation and procurement, before bench prototype  
**Primary milestone:** Wearable Tech Week demo  
**Current form factor:** Lapel/chest unit + inside-pocket USB battery + detachable wired foot switch  
**Source of truth:** This GitHub repository

No hardware assembly, wiring assignment, firmware implementation, enclosure selection, or purchase has yet been recorded as complete.

## Locked decisions

- ESP32 is the main controller.
- The Tech Week version is manually triggered by one momentary foot switch.
- The foot-switch lead should be stage-friendly and detachable, preferably via a 1/4-inch TS/TRS connection.
- The wearable must have strain relief and a local mute/emergency-stop control.
- The owned GC9A01 round 240 x 240 TFT is the default screen candidate; the owned ST7735S rectangular TFT is the fallback/alternate.
- A DFPlayer Mini will handle local audio from its own microSD card and drive a small 4-ohm speaker.
- A separate SPI microSD breakout will hold the larger visual library.
- Power comes from a normal USB power bank in the jacket pocket for the Tech Week build. No custom lithium charging circuit is required.
- The live demo must function without Wi-Fi.
- After Tech Week, add a magnetic door contact and use an owned PIR sensor to convert the same platform into an entryway shoe reminder.

## MVP acceptance criteria

The first prototype is successful when it can:

1. Boot reliably from USB power.
2. Debounce one foot-switch press into exactly one trigger.
3. Show one prepared face image and caption.
4. Play one short audio cue locally.
5. Return to an idle screen after the scene.
6. Stop or mute immediately from the wearable control.
7. Run for a 30-minute handling test without resets, loose connectors, or unsafe cable pull.

## Tech Week target

- 15–25 still images
- 5–10 short pre-rendered animation sequences
- 30–50 captions
- 10–20 short audio cues
- 8–10 reusable visual looks
- Five modes: Classic, Chaos, Roast, Clean Demo, Silent
- A compact, wearable presentation with concealed pocket power and tidy cable routing

Those are content targets, not requirements for the first electronics test.

## Open decisions

- Compare the GC9A01 and ST7735S using real face/caption mockups.
- Confirm the exact ESP32 model and safe GPIO assignments before wiring.
- Confirm whether a suitable USB power bank, short power cable, microSD cards, enclosure materials, 1/4-inch cable, and panel jack are already owned.
- Choose whether the foot-switch cable runs down the inside of a pant leg or along the wearer’s side.
- Measure practical speaker loudness in the Tech Week environment.
- Decide whether the final wearable is clipped, pinned, or mounted with a harness/backing plate.

## Risks and controls

| Risk | Control |
|---|---|
| Cable catches or is stepped on | Detachable jack, strain relief, slack loop, and pull test |
| Accidental repeated triggers | Hardware/software debounce and scene lockout |
| Audio is too quiet | Bench-test speaker orientation and enclosure grille before fabrication |
| Visual playback is slow | Pre-render 240 x 240 frames and benchmark SD reads early |
| Power-bank auto-shutoff | Test the exact bank under idle load before travel |
| ESP32 brownout during audio peaks | Use short power wiring, adequate supply, and local decoupling as needed |
| Demo content creates rights/privacy issues | Use owned, licensed, or original assets and obtain consent for identifiable faces |

## Next actions

1. Confirm or purchase the Tier 1 items in `GEAR_NEED.md`.
2. Breadboard ESP32 + selected display and render one image/caption.
3. Add DFPlayer Mini + speaker and play one locally stored cue.
4. Add and debounce the foot switch and emergency-stop button.
5. Add the visual microSD and benchmark scene load time.
6. Freeze connectors and pin map, then build the wearable enclosure.
7. Load the first curated content pack and rehearse the demo.

