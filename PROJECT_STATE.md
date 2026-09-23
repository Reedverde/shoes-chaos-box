# Project State

Last updated: 2026-09-22

## Status

**Phase:** Documentation and procurement, before bench prototype  
**Primary milestone:** Wearable Tech Week demo  
**Current form factor:** Purposefully rough lapel/suit-pocket wearable + outward screen + Circuit Playground LED halo + pocket battery + removable foot trigger
**First deployment:** Hack Alcatraz, Monday, October 5, 2026, 5:00–9:00 p.m. PT; arrive by 5:30, board at 5:45, cruise 6:00–9:00
**Source of truth:** This GitHub repository

No hardware assembly, wiring assignment, firmware implementation, enclosure selection, or purchase has yet been recorded as complete.

## Locked decisions

- ESP32 is the main controller.
- The wearable should look intentionally slapped together and visibly homemade while remaining electrically and mechanically safe.
- A small screen faces outward from the middle of the assembly and shows images, text, and tiny pre-rendered video/GIF-like loops.
- The Adafruit Circuit Playground Express provides a circular LED halo behind/around the display and casts animated light onto the shirt.
- The photographed main controller is a classic 38-pin ESP32 DevKit-style board with a CP2102 USB-UART bridge and Micro-USB connector. Its ESP32-WROOM-32-class layout has sufficient SPI, UART, Bluetooth, and GPIO resources for the planned build.
- The Tech Week version is manually triggered by a removable pedal. First verify the reportedly owned Bluetooth music pedal; retain a detachable 1/4-inch wired switch as the fallback.
- A wired foot-switch lead must be stage-friendly, strain-relieved, and detachable.
- The wearable must have a dedicated local manual-trigger button independent of the pedal.
- The wearable must also have a local mute/emergency-stop control independent of the trigger button.
- The owned GC9A01 round 240 x 240 TFT is the default screen candidate; the owned ST7735S rectangular TFT is the fallback/alternate.
- A DFPlayer Mini will handle local audio from its own microSD card and drive a small 4-ohm speaker.
- A separate SPI microSD breakout will hold the larger visual library.
- Power comes from the confirmed Smatree DP20S USB-C battery pack in the jacket pocket. Its label specifies 5V/2A input/output and 5000mAh at 3.7V (18.5Wh). No custom lithium charging circuit is required.
- The live demo must function without Wi-Fi.
- The demonstration premise is that the home version uses a pressure pad beneath a welcome mat; the removable pedal substitutes for that sensor during the live demo.
- A later real home installation can use the pressure mat as the primary trigger, with an optional door contact and owned PIR sensor for arrival confirmation.

## Event-host integration direction

- **Kling AI:** candidate production tool for short image-to-video reaction loops. Runtime exports remain local frame sequences.
- **Cloudflare Flue:** candidate framework for a hosted Chaos Director that generates or curates scene manifests and downloadable content packs.
- These integrations are stretch enhancements. The core pedal/button-triggered wearable must be complete and offline-capable without them.

## MVP acceptance criteria

The first prototype is successful when it can:

1. Boot reliably from USB power.
2. Convert one Bluetooth-pedal or local-trigger-button press into exactly one scene, including reconnect and switch-bounce cases.
3. Show one prepared face image or short frame loop plus a caption.
4. Play one short audio cue locally.
5. Run one synchronized Circuit Playground LED effect at a brightness safe for power and comfort.
6. Return to an idle screen/LED state after the scene.
7. Stop or mute immediately from the wearable control.
8. Run for a 30-minute handling test without resets, loose connectors, excessive heat, or unsafe cable pull.
9. Run in ready-to-demo mode for at least five hours on the Smatree DP20S, with repeated short scenes and no recharge.

## Tech Week target

- 15–25 still images
- 5–10 short pre-rendered animation sequences
- 30–50 captions
- 10–20 short audio cues
- 8–10 reusable visual looks
- Five modes: Classic, Chaos, Roast, Clean Demo, Silent
- A compact wearable whose deliberately rough visual construction conceals safe mounting, insulated connections, and dependable power routing

Those are content targets, not requirements for the first electronics test.

## Open decisions

- Compare the GC9A01 and ST7735S using real face/caption mockups.
- Confirm the exact ESP32 model and safe GPIO assignments before wiring.
- Confirm a short USB-C-to-Micro-USB power lead from the Smatree pack, the shared 5V distribution method, mounting materials, and any wired-fallback jack/cable.
- Find and identify the Bluetooth music pedal; determine whether it sends BLE MIDI, keyboard/page-turn commands, or a proprietary protocol.
- Test whether the Circuit Playground can sit behind the selected display while leaving its ten NeoPixels visible enough to create the desired halo.
- Choose whether the foot-switch cable runs down the inside of a pant leg or along the wearer’s side.
- Measure practical speaker loudness in the Tech Week environment.
- Decide whether the final wearable is clipped, pinned, or mounted with a harness/backing plate.

## Risks and controls

| Risk | Control |
|---|---|
| Cable catches or is stepped on | Detachable jack, strain relief, slack loop, and pull test |
| Accidental repeated triggers | Hardware/software debounce and scene lockout |
| Bluetooth pedal is missing, incompatible, or disconnects | Keep an owned pushbutton for bench tests and build a detachable wired fallback |
| Audio is too quiet | Bench-test speaker orientation and enclosure grille before fabrication |
| Visual playback is slow | Pre-render 240 x 240 frames and benchmark SD reads early |
| LED halo causes excess current draw or glare | Cap brightness, avoid sustained full-white output, and test against the actual shirt |
| Combined load exceeds the pack's 5V/2A rating | Measure worst-case current with screen, loud audio, and LED effect active; impose firmware brightness/volume limits |
| Power-bank auto-shutoff | Test the exact bank under idle load before travel |
| Cellular/Wi-Fi reception is poor on the boat | Keep every runtime asset and trigger path local; use no network dependency |
| Movement, wind, or crowd contact disturbs the wearable | Use load-spreading backing, secure clips/pins, restrained pocket battery, and a pre-boarding movement test |
| ESP32 brownout during audio peaks | Use short power wiring, adequate supply, and local decoupling as needed |
| Demo content creates rights/privacy issues | Use owned, licensed, or original assets and obtain consent for identifiable faces |

## Next actions

1. While the four-item electronics order is in transit, breadboard ESP32 + selected display and render one image/caption.
2. Add the Circuit Playground LED halo and synchronize one effect using the owned hardware.
3. On delivery, inspect and test the ordered DFPlayer, speakers, microSD modules, and both 8 GB cards before moving them to `GEAR_HAVE.md`.
4. Add DFPlayer Mini + speaker and play one locally stored cue.
5. Identify/test the Bluetooth pedal; add and debounce the wired fallback only if needed.
6. Add the emergency-stop button and visual microSD; benchmark scene load time.
7. Freeze connectors and pin map, then assemble the intentionally rough-looking wearable safely.
8. Load the first curated content pack and rehearse the pressure-mat story plus pedal demonstration.
9. Rehearse a 1–2 minute Hack Alcatraz version and complete a five-hour battery soak test.
