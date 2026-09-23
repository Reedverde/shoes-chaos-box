# Gear Master

This is the authoritative configuration and status map. It prevents “recommended,” “probably owned,” and “confirmed owned” from being treated as the same thing.

## Status definitions

- **HAVE:** Confirmed by the owner or clearly visible in supplied photos.
- **NEED:** Required for the selected build and not confirmed owned.
- **CONFIRM:** May already be available; check before buying.
- **DEFER:** Belongs to the post–Tech Week entryway version.
- **OUT:** Available or discussed but intentionally excluded from the current architecture.

## Current build configuration

| System | Selected item | Status | Notes |
|---|---|---|---|
| Controller | ESP32 development board | HAVE | Exact model and pin map must be verified |
| Primary display | GC9A01 1.28-inch 240 x 240 SPI TFT | HAVE | Default, pending real-asset comparison |
| Alternate display | ST7735S rectangular color TFT | HAVE | Better candidate for stacked captions |
| Audio player | DFPlayer Mini | NEED | UART control; local audio card |
| Speaker | 4-ohm compact speaker, 3W-class load | NEED | Confirm enclosure loudness |
| Visual storage | 3.3V-compatible SPI microSD breakout | NEED | Separate from DFPlayer card |
| Audio storage | 4–32 GB microSD card | NEED | FAT32; dedicated to DFPlayer |
| Visual storage card | 4–32 GB microSD card | NEED | FAT32; dedicated to visuals |
| Trigger | Normally open momentary foot switch | NEED | Dry-contact switch, not an expression pedal |
| Trigger connection | Detachable 1/4-inch TS/TRS path | CONFIRM | Panel jack/cable plus strain relief |
| Local control | Owned momentary pushbutton | HAVE | Mute and emergency-stop behavior |
| Power | USB power bank in inside jacket pocket | CONFIRM | Test auto-shutoff and runtime |
| Enclosure | Wearable lapel/chest housing | CONFIRM | Choose after breadboard dimensions are final |
| Entry sensor | Normally closed magnetic door contact | DEFER | Post-event automatic version |
| Presence sensor | HC-SR501 PIR | HAVE/DEFER | Already owned; post-event use |

## Buying rules

1. Check `GEAR_HAVE.md` before ordering.
2. Resolve every **CONFIRM** line before buying it.
3. Buy Tier 1 items first; defer enclosure purchases until display, speaker, connectors, and boards are measured together.
4. Prefer named manufacturer or specialist suppliers for the DFPlayer and storage breakout; counterfeit or incompatible look-alikes cost more time than they save.
5. Record purchases and exact model numbers in `GEAR_HAVE.md`, then remove them from the active section of `GEAR_NEED.md`.

## Configuration boundaries

- Tech Week is a manually triggered wearable; it does not need door sensing.
- The DFPlayer card stores audio; the SPI breakout card stores images/animation frames.
- The unit must work offline.
- Power remains a standard USB power bank for the event.
- Existing TP4056 boards, radios, relay, RFID reader, and environmental sensor are not part of the current build.

