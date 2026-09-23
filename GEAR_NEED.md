# Gear Need

Links are references, not proof of purchase. Availability and revisions can change; verify electrical details before ordering. Move an item to `GEAR_HAVE.md` as soon as it is physically confirmed.

## Tier 1 — required for the bench MVP

| Item | Qty | Requirement | Reference |
|---|---:|---|---|
| DFPlayer Mini | 1 | DFR0299-class board, UART control, direct compact-speaker output, microSD support | [DFRobot DFPlayer Mini](https://www.dfrobot.com/product-1121.html) |
| Compact speaker | 1 | 4 ohms; small enough for wearable; rated safely for the DFPlayer output | [Adafruit 40 mm 4-ohm speaker](https://www.adafruit.com/product/3968) |
| Visual microSD breakout | 1 | SPI, compatible with 3.3V ESP32 logic | [Adafruit Micro SD SPI/SDIO breakout](https://www.adafruit.com/product/4682) |
| microSD cards | 2 | Separate audio and visual cards; use 4–32 GB cards that can be formatted FAT32 | Buy from a reputable local/electronics retailer |
| Momentary foot switch | 1 | Normally open dry-contact foot switch; 1/4-inch plug; not a continuous expression pedal | [Nektar NP-1 product family](https://nektartech.com/np-1_np-2_nx-p/) |

The DFPlayer manufacturer documents UART control, a 3W mono amplifier, and FAT16/FAT32 cards up to 32 GB. The linked Adafruit speaker is currently sold as a 4-ohm 5W revision and is described as suitable where 3W or less is required.

## Confirm before buying

| Item | What to verify |
|---|---|
| USB power bank | Already owned? Stable USB output? Does it remain awake at the device’s idle load? |
| Short USB cable | Correct connector for the exact ESP32; flexible enough for inside-jacket routing |
| 1/4-inch connection hardware | Does the chosen pedal already have a captive cable? Select matching panel jack/cable or extension |
| Strain relief and cable clips | Fit the chosen enclosure and garment routing |
| Wearable enclosure | Wait until boards, speaker, jack, and display are measured together |
| Clip, pins, or backing plate | Must spread load and prevent rotation on the jacket |
| Perfboard/solderable prototype board | Needed only after the breadboard circuit is stable |
| Hookup wire, headers/connectors, heat-shrink | Check existing supplies before ordering |
| Spare USB cable and spare cards | Strongly recommended for travel/demo recovery |

## Tier 2 — polish after MVP

- Speaker grille or acoustically open front treatment
- Panel-mounted power connection if the ESP32 USB port is inaccessible
- Second local button for mode advance, if one-button gestures prove awkward
- Status LED, only if the display does not provide enough feedback
- Backup hand trigger using an owned pushbutton

## Deferred — post–Tech Week entryway version

| Item | Requirement |
|---|---|
| Magnetic door contact | Normally closed wired reed/contact sensor |
| USB wall supply | Quality regulated 5V/2A supply plus appropriate cable |
| Fixed enclosure/mounting | Designed after door/PIR placement testing |

## Do not buy for the current build

- Another ESP32 or Arduino
- Another display
- Another PIR sensor
- A relay or radio module
- Lithium cells or another charger board
- A native-app development kit

