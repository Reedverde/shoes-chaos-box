# Hardware Inventory

This file is the consolidated engineering view. Procurement status is normalized in `GEAR_MASTER.md`, with confirmed items in `GEAR_HAVE.md` and outstanding items in `GEAR_NEED.md`.

## Confirmed owned

| Category | Item | Planned use |
|---|---|---|
| Controller | ESP32 development board | Main controller; exact model/pinout still to verify |
| Prototyping | MB-102-style breadboard kit, power module, jumper wires | Bench build |
| Display | 1.28-inch GC9A01 240 x 240 SPI round TFT | Default Tech Week candidate |
| Display | ST7735S rectangular color TFT | Caption-heavy alternate |
| Display | Two small monochrome OLED displays | Status/debug or future use |
| Input | Pushbuttons and mini slide switches | Mute, emergency stop, mode/test |
| Sensor | Several HC-SR501 PIR motion sensors | Post-event entry confirmation |
| Power | TP4056 lithium charging boards | Future rechargeable experiments; not Tech Week power |
| Power | Battery holder | Future use; chemistry/fit not confirmed |
| Interface | Logic-level converter boards | Available if a verified interface requires them |
| Other | Adafruit Circuit Playground Express | Spare platform; not in current architecture |
| Other | 5V relay module | Not needed for current build |
| Other | CC1101 radio modules | Not needed for current build |
| Other | NRF24L01/PA/LNA radio module(s) | Not needed for current build |
| Other | BME280 environmental sensor | Not needed for current build |
| Other | RC522 RFID/NFC reader | Not needed for current build |

## Required for Tech Week but not confirmed owned

| Priority | Item | Purpose |
|---|---|---|
| Tier 1 | DFPlayer Mini (DFR0299 or compatible verified board) | Local audio playback over UART |
| Tier 1 | 4-ohm speaker rated for at least the DFPlayer's output | Audible cue |
| Tier 1 | Momentary foot switch | Manual scene trigger |
| Tier 1 | Two microSD cards, 4–32 GB, FAT32-capable | Separate audio and visual libraries |
| Tier 1 | SPI microSD breakout compatible with 3.3V ESP32 logic | Visual storage |
| Confirm | USB power bank and short matching cable | Pocket power |
| Confirm | 1/4-inch panel jack, matching cable/plug, strain relief | Detachable stage connection |
| Confirm | Compact wearable enclosure and secure mounting hardware | Finished lapel/chest unit |
| Build supply | Perfboard or solderable prototype board, wire, headers/connectors, heat-shrink | Reliable integration after breadboard proof |

## Deferred entryway hardware

| Item | Status |
|---|---|
| Normally closed magnetic reed door/contact sensor | Need after Tech Week |
| Quality 5V/2A USB wall adapter and cable | Confirm/need when converting to fixed installation |
| Wall enclosure/mount | Design after trigger logic is validated |

## Explicitly not needed now

- Another microcontroller
- Another PIR sensor
- Another display
- A relay
- Additional radios
- A custom lithium pack or TP4056-based charging system
- A native phone app

