# Gear Need

Links are references, not proof of purchase. Availability and revisions can change; verify electrical details before ordering. Move an item to `GEAR_HAVE.md` as soon as it is physically confirmed.

## Order now — travel-critical package

The current trip manifest has the outbound flight on **Monday, October 5, 2026**, with check-in opening Sunday, October 4. October 3 is the hotel cancellation-policy date, not the recorded departure date. Even so, place the electronics orders immediately so there is time for bench testing and one replacement cycle before travel.

| Priority | Order | Recommended qty | Why / exact requirement | Current reference |
|---|---|---:|---|---|
| 1 | Authentic DFRobot DFPlayer Mini, SKU DFR0299 | 2 | One build unit plus one cheap travel/build spare; avoid unbranded clones | [Mouser DFR0299](https://www.mouser.com/en/ProductDetail/DFRobot/DFR0299?mgh=1&qs=Zcin8yvlhnPSNhqM2hweWw%3D%3D) |
| 1 | 40 mm speaker, 4 ohms, rated 3 W or higher | 2 | One build unit plus spare; connect to DFPlayer SPK1/SPK2 | [Adafruit product 3968](https://www.adafruit.com/product/3968) |
| 1 | 3.3 V microSD SPI breakout | 2 | One build unit plus spare for the visual library; product 4682 works with ESP32 logic | [Adafruit product 4682](https://www.adafruit.com/product/4682) |
| 1 | 16 GB or 32 GB name-brand microSD cards | 3 | Audio card, visual card, and ready-to-swap travel spare; all must be FAT32-capable | Buy locally or from a reputable retailer |
| 1 | 1 kΩ resistors | 5 or small assortment | DFRobot recommends one in the ESP32-TX-to-DFPlayer-RX path | Buy locally or add to electronics order |

Current public listings checked September 23, 2026 showed the authentic DFR0299 available through Mouser, and Adafruit products 3968 and 4682 in stock. Stock and delivery promises can change at checkout; use an expedited option that arrives no later than September 28 if practical.

### Check at home today; add to the order if missing

- Short, flexible USB-C cable that fits the Smatree DP20S and the confirmed ESP32 power connector
- Solderable perfboard, male header, stranded hookup wire, and small plug connectors
- Heat-shrink tubing, electrical insulation, cable ties, and adhesive cable anchors
- Soldering iron, solder, wire cutters/strippers, and a multimeter
- Stiff but lightweight backing material, large safety pins or locking clips, and fabric-safe load spreading
- USB microSD reader compatible with the computer

Do not delay the display/LED prototype for these deliveries. The ESP32, round display, Circuit Playground, breadboard, buttons, and battery are already sufficient to build the silent trigger-and-visual path first.

## Tier 1 — required for the bench MVP

| Item | Qty | Requirement | Reference |
|---|---:|---|---|
| DFPlayer Mini | 1 installed + 1 spare | Authentic DFR0299 board, UART control, direct compact-speaker output, microSD support | [Mouser DFR0299](https://www.mouser.com/en/ProductDetail/DFRobot/DFR0299?mgh=1&qs=Zcin8yvlhnPSNhqM2hweWw%3D%3D) |
| Compact speaker | 1 installed + 1 spare | 4 ohms; small enough for wearable; rated for at least 3 W | [Adafruit 40 mm 4-ohm speaker](https://www.adafruit.com/product/3968) |
| Visual microSD breakout | 1 installed + 1 spare | SPI, compatible with 3.3V ESP32 logic | [Adafruit Micro SD SPI/SDIO breakout](https://www.adafruit.com/product/4682) |
| microSD cards | 2 installed + 1 spare | Separate audio and visual cards; use 16–32 GB name-brand cards that can be formatted FAT32 | Buy from a reputable local/electronics retailer |
| UART series resistor | 1 installed + spares | 1 kΩ between ESP32 TX and DFPlayer RX per manufacturer guidance | Buy locally or add to electronics order |

The DFPlayer manufacturer documents UART control, a 3W mono amplifier, FAT16/FAT32 cards up to 32 GB, and a 1 kΩ series resistor in the controller-to-player serial path. The linked Adafruit speaker is currently sold as a 4-ohm 5W revision and is described as suitable where 3W or less is required.

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
