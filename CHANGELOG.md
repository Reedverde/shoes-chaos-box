# Changelog

All notable documentation and project-direction changes are recorded here.

## 2026-09-23 — First firmware milestone

### Added

- Added a PlatformIO/Arduino firmware project for the confirmed classic ESP32 board and GC9A01 round display.
- Added an idle screen and four button-triggered diagnostic scenes with debounce and automatic return to idle.
- Added the exact Milestone 1 breadboard wiring for the ESP32, display, and one local button.
- Reserved later pins for the visual microSD, DFPlayer UART, and mute button without connecting those devices yet.

### Verified

- Compiled the firmware successfully for the `esp32dev` target; the build used 6.6% of RAM and 21.7% of the available application flash.

## 2026-09-23 — Travel-critical purchase list

### Changed

- Converted `GEAR_NEED.md` into an urgent pre-travel order and then clarified minimum versus optional redundancy: one DFPlayer, one speaker, one visual-storage breakout, and two functionally required microSD cards; any duplicates are optional travel insurance.
- Marked the 1 kΩ UART resistor as verify-owned rather than an automatic purchase.
- Recorded the completed four-item Amazon order as in transit: one DFPlayer, two MakerHawk speakers, five HiLetgo microSD modules, and two Bliksem 8 GB microSD cards.
- Kept ordered items out of `GEAR_HAVE.md` until they arrive and pass label, capacity, and basic-operation checks.
- Identified the owned controller as a classic 38-pin ESP32 DevKit-style board with CP2102 USB-UART; corrected the initially misread connector to USB-C after the owner physically confirmed the matching cable.
- Added ESP32 pin guardrails and retained USB-C for the initial upload and power test.
- Confirmed an owned Xinwei 1/4W, ±1% metal-film assortment containing ten 1 kΩ resistors; removed the resistor purchase uncertainty.
- Reconciled the spoken October 3 departure estimate with the trip manifest: the recorded outbound flight is October 5; October 3 is the hotel cancellation-policy date.

## 2026-09-22 — Wearable concept clarified

### Changed

- Locked the intentionally slapped-together lapel/suit-pocket aesthetic; the build should look improvised while remaining safe and reliable underneath.
- Put the small outward-facing image/text/animation screen at the visual center of the wearable.
- Promoted the owned Adafruit Circuit Playground Express from spare hardware to the circular LED halo behind/around the screen, washing animated light onto the shirt.
- Added many randomized audio/video samples and synchronized LED patterns to the scene concept.
- Made the believed-owned Bluetooth music pedal the first trigger to evaluate and moved the detachable wired foot switch to fallback status.
- Defined the presentation story: a pressure pad beneath the home welcome mat normally triggers randomized shoe reminders; the removable pedal substitutes for it during the traveling demo.
- Changed the future home build’s primary input from door sensing to the welcome-mat pressure pad, with door contact and PIR retained as optional confirmation sensors.
- Confirmed the pocket battery as a Smatree DP20S USB-C pack labeled 5V/2A input/output and 5000mAh at 3.7V / 18.5Wh; purchasing another bank is now conditional on load testing.
- Set Hack Alcatraz on October 5, 2026 as the first deployment target: arrive by 5:30 p.m., board at 5:45, cruise 6:00–9:00, and prepare a rapid 1–2 minute offline demo.
- Replaced the vague all-day power question with a five-hour event-profile endurance requirement on the confirmed DP20S pack.
- Made a dedicated on-device scene-trigger button mandatory alongside the Bluetooth pedal, with a separate mute/emergency-stop control.
- Added Kling AI as an optional short-loop production tool and Cloudflare Flue as the proposed hosted Chaos Director for generating local scene manifests.
- Clarified that the same reusable scene flow serves the owner or any visitor: trigger, face/shoe image, overlaid reminder text, audio and circular LED response, then reset for the next person.
- Kept both host-product integrations outside the critical runtime path so the boat demo remains fully offline.

## 2026-09-22 — Repository baseline

### Added

- Established GitHub as the project source of truth.
- Added the nine core Markdown documents.
- Recorded the confirmed owned hardware from the supplied photos and conversation.
- Added the master gear taxonomy plus separate confirmed-have and need/confirm lists.
- Added referenced purchase candidates for the DFPlayer Mini, compact speaker, visual microSD breakout, and momentary foot switch.
- Added phased build, test, wearable-safety, content, and post-event conversion plans.

### Changed from the original concept

- Prioritized a portable Tech Week demo before the permanent entryway installation.
- Refined the Tech Week form factor from a tabletop box to a wearable lapel/chest unit.
- Moved power to a USB battery bank in an inside jacket pocket.
- Standardized the event trigger on a detachable wired momentary foot switch with strain relief.
- Expanded the device from a single repeated cue into a randomized scene engine with images, captions, looks, animation frames, audio, and modes.
- Added a separate visual microSD card while retaining the DFPlayer’s dedicated audio card.
- Kept the automatic door contact + PIR behavior as the post–Tech Week phase.

### Clarified

- The round GC9A01 is the default display candidate, but the rectangular ST7735S must be compared using real content before final enclosure work.
- Power bank, cards, connectors, enclosure supplies, and other common items are **confirm before buying**, not assumed owned.
- The project will not distribute ripped YouTube media; runtime assets must be original, licensed, or otherwise cleared.
- Rechecked the recoverable component photos and recorded the exact 1.8-inch 128 x 160 TFT format, both OLED variants, four-AAA holder, NRF24L01+PA+LNA configuration, and Adafruit Circuit Playground Express product number.
- The exact ESP32 model remains unconfirmed because its board markings are not visible in the recoverable photos.
