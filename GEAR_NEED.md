# Gear Need

Links are references, not proof of purchase. Availability and revisions can change; verify electrical details before ordering. Move an item to `GEAR_HAVE.md` as soon as it is physically confirmed.

## Ordered — awaiting delivery and inspection

The four-item Amazon order was placed on September 23, 2026. These items remain here—not in `GEAR_HAVE.md`—until they arrive and their labels, quantities, electrical specifications, and basic operation are confirmed.

| Ordered item | Ordered qty | Intended use | Arrival checks | Reference |
|---|---:|---|---|---|
| DFPlayer Mini listing represented as DFRobot DFR0299 | 1 | Dedicated audio player | Confirm DFRobot branding/model, pin labels, and playback from FAT32 card | [Amazon listing](https://www.amazon.com/dp/B089D5NLW1) |
| MakerHawk 4-ohm, 3-watt speaker pack | 1 pack / 2 speakers | One installed speaker plus one spare | Confirm 4-ohm/3-watt label and connector/wire arrangement | [Amazon listing](https://www.amazon.com/dp/B08JCHK7GR) |
| HiLetgo SPI microSD module pack | 1 pack / 5 modules | One visual-storage interface plus spares | Confirm level shifting, 5V VCC requirement, pin labels, and ESP32 operation | [Amazon listing](https://www.amazon.com/dp/B07BJ2P6X6) |
| Bliksem 8 GB Class 10 microSD two-pack | 1 pack / 2 cards | One audio card and one visual card | Capacity-test both cards, format FAT32, label AUDIO and VISUAL, then read/write-test | [Amazon listing](https://www.amazon.com/dp/B0CYZ2WKNF) |

The 1 kΩ resistor is confirmed in the owned Xinwei 1/4W, ±1% metal-film assortment; no resistor purchase is needed.

The ordered speaker and microSD-module multipacks include extra units, so no additional travel spares are needed now.

### Check at home today; add to the order if missing

- Short, flexible USB-C cable that fits the Smatree DP20S and the confirmed ESP32 power connector
- Solderable perfboard, male header, stranded hookup wire, and small plug connectors
- Heat-shrink tubing, electrical insulation, cable ties, and adhesive cable anchors
- Soldering iron, solder, wire cutters/strippers, and a multimeter
- Stiff but lightweight backing material, large safety pins or locking clips, and fabric-safe load spreading
- USB microSD reader compatible with the computer

Do not delay the display/LED prototype for these deliveries. The ESP32, round display, Circuit Playground, breadboard, buttons, and battery are already sufficient to build the silent trigger-and-visual path first.

## Tier 1 — ordered for the bench MVP

| Item | Qty | Requirement | Reference |
|---|---:|---|---|
| DFPlayer Mini | 1 ordered | UART control, direct compact-speaker output, microSD support; verify authenticity on arrival | [Amazon order listing](https://www.amazon.com/dp/B089D5NLW1) |
| Compact speaker | 2 ordered | 4 ohms, 3 W; one installed plus one spare | [Amazon order listing](https://www.amazon.com/dp/B08JCHK7GR) |
| Visual microSD modules | 5 ordered | SPI modules with onboard level shifting; one installed plus spares | [Amazon order listing](https://www.amazon.com/dp/B07BJ2P6X6) |
| microSD cards | 2 ordered | Separate 8 GB Class 10 audio and visual cards; format FAT32 and test capacity | [Amazon order listing](https://www.amazon.com/dp/B0CYZ2WKNF) |
| UART series resistor | 1 confirmed owned | 1 kΩ between ESP32 TX and DFPlayer RX per manufacturer guidance | Xinwei assortment in `GEAR_HAVE.md` |

The DFPlayer manufacturer documents UART control, a 3W mono amplifier, FAT16/FAT32 cards up to 32 GB, and a 1 kΩ series resistor in the controller-to-player serial path. The ordered MakerHawk speakers are listed as 4 ohms and 3 watts, matching that direct-output target.

## Confirm before buying

| Item | What to verify |
|---|---|
| USB-C power lead/distribution | Use the owned Smatree DP20S as the source; choose a short flexible lead and a safe shared 5V distribution method after the ESP32 connector/power path is identified |
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
