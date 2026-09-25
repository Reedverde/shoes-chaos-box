# Gear Master

This is the authoritative configuration and status map. It prevents “recommended,” “probably owned,” and “confirmed owned” from being treated as the same thing.

## Status definitions

- **HAVE:** Confirmed by the owner or clearly visible in supplied photos.
- **NEED:** Required for the selected build and not confirmed owned.
- **CONFIRM:** May already be available; check before buying.
- **CONDITIONAL:** Needed only if the preferred approach fails a compatibility or reliability test.
- **OPTIONAL:** Helpful for a later refinement but not required for the selected behavior.
- **DEFER:** Belongs to the post–Tech Week entryway version.
- **OUT:** Available or discussed but intentionally excluded from the current architecture.

## Current build configuration

| System | Selected item | Status | Notes |
|---|---|---|---|
| Controller | Classic ESP32 38-pin DevKit-style board with CP2102 and USB-C | HAVE | USB-C physically confirmed by owner; use visible labels and verify flash size in firmware |
| Primary display | GC9A01 1.28-inch 240 x 240 SPI TFT | HAVE | Default, pending real-asset comparison |
| Alternate display | ST7735S rectangular color TFT | HAVE | Better candidate for stacked captions |
| LED halo | Adafruit Circuit Playground Express, product 3333 | HAVE | Mount behind/around center display; synchronize its ten NeoPixels with scenes |
| Audio player | DFPlayer Mini | ORDERED | One Amazon-listed DFRobot DFR0299; verify branding and operation on arrival |
| Speaker | MakerHawk 4-ohm, 3W compact speakers | ORDERED | Two ordered; one installed plus one spare; confirm enclosure loudness |
| Visual storage | HiLetgo SPI microSD modules | ORDERED | Five ordered; level-shifted module requires 5V VCC; bench-test with ESP32 |
| Audio storage | Bliksem 8 GB Class 10 microSD card | ORDERED | One of two ordered cards; FAT32; dedicated to DFPlayer |
| Visual storage card | Bliksem 8 GB Class 10 microSD card | ORDERED | One of two ordered cards; FAT32; dedicated to visuals |
| UART series resistor | 1 kΩ, 1/4W, ±1% metal-film resistor | HAVE | Use one from confirmed Xinwei assortment between ESP32 TX and DFPlayer RX |
| Preferred trigger | STRICH SPT-10 Bluetooth music/page-turn pedal | HAVE / VERIFIED | Classic Bluetooth HID works directly with ESP32 in Mode 5; left=Space (`0x2C`), right=Enter (`0x28`); both launch scenes |
| On-device trigger | Owned momentary pushbutton | HAVE | Required wearable backup; launches the same scene path as the pedal |
| Wired fallback trigger | Normally open momentary foot switch | CONDITIONAL | Buy/build only if the Bluetooth pedal is unsuitable; dry contact, not expression |
| Wired trigger connection | Detachable 1/4-inch TS/TRS path | CONDITIONAL | Panel jack/cable plus strain relief if fallback is used |
| Local safety control | Second owned momentary pushbutton | HAVE | Separate mute and emergency-stop behavior |
| Power | Smatree DP20S USB-C battery pack, 5V/2A, 5000mAh / 18.5Wh | HAVE | Pocket power; test auto-shutoff, runtime, and combined load under the 2A ceiling |
| Mounting | Purposefully rough lapel/suit-pocket assembly | CONFIRM | Visible boards/wires are acceptable; sharp edges and exposed conductors are not |
| Home trigger | Low-voltage pressure pad beneath welcome mat | DEFER | Future primary home input and current demonstration premise |
| Entry sensor | Normally closed magnetic door contact | OPTIONAL/DEFER | May supplement the mat |
| Presence sensor | HC-SR501 PIR | HAVE/DEFER | Already owned; may supplement the mat |

## Buying rules

1. Check `GEAR_HAVE.md` before ordering.
2. Resolve every **CONFIRM** line before buying it.
3. Buy Tier 1 items first; defer enclosure purchases until display, speaker, connectors, and boards are measured together.
4. Prefer named manufacturer or specialist suppliers for the DFPlayer and storage breakout; counterfeit or incompatible look-alikes cost more time than they save.
5. Record purchases and exact model numbers in `GEAR_HAVE.md`, then remove them from the active section of `GEAR_NEED.md`.

## Configuration boundaries

- Tech Week is a manually triggered wearable; the pedal substitutes for the pressure welcome mat described in the demonstration story.
- The intentionally rough appearance is cosmetic, not permission for unsafe power, exposed conductors, sharp edges, or weak mounting.
- The DFPlayer card stores audio; the SPI breakout card stores images/animation frames.
- The unit must work offline.
- Power remains a standard USB power bank for the event.
- Existing TP4056 boards, radios, relay, RFID reader, and environmental sensor are not part of the current build.
