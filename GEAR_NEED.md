# Gear Need

Links are references, not proof of purchase. Availability and revisions can change; verify electrical details before ordering. Move an item to `GEAR_HAVE.md` as soon as it is physically confirmed.

## Tier 1 — required for the bench MVP

| Item | Qty | Requirement | Reference |
|---|---:|---|---|
| DFPlayer Mini | 1 | DFR0299-class board, UART control, direct compact-speaker output, microSD support | [DFRobot DFPlayer Mini](https://www.dfrobot.com/product-1121.html) |
| Compact speaker | 1 | 4 ohms; small enough for wearable; rated safely for the DFPlayer output | [Adafruit 40 mm 4-ohm speaker](https://www.adafruit.com/product/3968) |
| Visual microSD breakout | 1 | SPI, compatible with 3.3V ESP32 logic | [Adafruit Micro SD SPI/SDIO breakout](https://www.adafruit.com/product/4682) |
| microSD cards | 2 | Separate audio and visual cards; use 4–32 GB cards that can be formatted FAT32 | Buy from a reputable local/electronics retailer |

The DFPlayer manufacturer documents UART control, a 3W mono amplifier, and FAT16/FAT32 cards up to 32 GB. The linked Adafruit speaker is currently sold as a 4-ohm 5W revision and is described as suitable where 3W or less is required.

## Confirm before buying

| Item | What to verify |
|---|---|
| USB-C power lead/distribution | Use the owned Smatree DP20S as the source; choose a short flexible lead and a safe shared 5V distribution method after the ESP32 connector/power path is identified |
| Bluetooth music pedal | Find the believed-owned pedal; record brand/model and whether it emits BLE MIDI, keyboard/page-turn commands, or a proprietary signal |
| Wired momentary pedal | Buy only if the Bluetooth pedal is missing or incompatible; normally open dry-contact example: [Nektar NP-1 family](https://nektartech.com/np-1_np-2_nx-p/) |
| 1/4-inch connection hardware | Needed only for the wired fallback; select a matching panel jack/cable or extension |
| Strain relief and cable clips | Required if the wired fallback is used; fit the mounting and garment routing |
| Rough wearable mounting materials | Confirm safe clips/pins, backing plate, insulation, and load spreading; cardboard/tape/visible wire may be part of the intended look |
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

The USB battery pack itself is already owned; do not buy another unless the DP20S fails its idle-shutoff, runtime, or 5V/2A peak-load test.

## Deferred — post–Tech Week entryway version

| Item | Requirement |
|---|---|
| Magnetic door contact | Normally closed wired reed/contact sensor |
| USB wall supply | Quality regulated 5V/2A supply plus appropriate cable |
| Fixed enclosure/mounting | Designed after door/PIR placement testing |
| Low-voltage pressure pad/mat switch | Primary welcome-mat trigger; select only after floor, mat, cable route, and expected load are measured |

## Do not buy for the current build

- Another ESP32 or Arduino
- Another display
- Another PIR sensor
- A relay or radio module
- Lithium cells or another charger board
- A native-app development kit
