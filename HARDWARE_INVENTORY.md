# Hardware Inventory

This file is the consolidated engineering view. Procurement status is normalized in `GEAR_MASTER.md`, with confirmed items in `GEAR_HAVE.md` and outstanding items in `GEAR_NEED.md`.

## Confirmed owned

| Category | Item | Planned use |
|---|---|---|
| Controller | ESP32 development board | Main controller; exact model/pinout still to verify |
| Prototyping | MB-102-style breadboard kit, power module, jumper wires | Bench build |
| Display | 1.28-inch GC9A01 240 x 240 round RGB TFT, 4-wire SPI | Default Tech Week candidate; photo label identifies Shenzhen Esida Electronics |
| Display | 1.8-inch 128 x 160 RGB TFT, ST7735S-class SPI board with K1/K2/K3 buttons | Caption-heavy alternate; PCB labels include GND, VCC, SCL, SDA, RES, DC, CS, and BLK |
| Display | 0.91-inch 128 x 32 white I2C OLED | Status/debug or future use |
| Display | 0.96-inch blue OLED | Status/debug or future use |
| Input | Pushbuttons and mini slide switches | Mute, emergency stop, mode/test |
| Sensor | Several HC-SR501 PIR motion sensors | Post-event entry confirmation |
| Power | TP4056 lithium charging boards | Future rechargeable experiments; not Tech Week power |
| Power | Four-AAA battery holder with switch and two-pin lead | Future use; not the Tech Week power plan |
| Power | Smatree DP20S cylindrical USB-C battery pack, 5V/2A input/output, 5000mAh at 3.7V / 18.5Wh | Confirmed Tech Week pocket power; must pass idle and peak-load tests |
| Interface | Logic-level converter boards | Available if a verified interface requires them |
| Lighting | Adafruit Circuit Playground Express, product 3333 | Circular ten-NeoPixel halo behind/around center display; animated shirt backlight |
| Other | 5V relay module | Not needed for current build |
| Other | CC1101 radio modules | Not needed for current build |
| Other | NRF24L01+PA+LNA 2.4 GHz module with SMA antenna | Not needed for current build |
| Other | BME280 environmental sensor | Not needed for current build |
| Other | RC522 RFID/NFC reader | Not needed for current build |

## Ordered for Tech Week but not yet received

| Priority | Item | Ordered quantity | Purpose / receipt check |
|---|---|---:|---|
| Tier 1 | Amazon-listed DFRobot DFPlayer Mini DFR0299 | 1 | Local audio playback over UART; verify authenticity and operation |
| Tier 1 | MakerHawk 4-ohm, 3W speaker | 2 | Audible cue; one installed and one spare |
| Tier 1 | Bliksem 8 GB Class 10 microSD card | 2 | Separate FAT32 audio and visual libraries; capacity-test both |
| Tier 1 | HiLetgo level-shifted SPI microSD module | 5 | Visual storage; confirm 5V VCC and ESP32 logic operation |

## Still required or unresolved for Tech Week

| Priority | Item | Purpose |
|---|---|---|
| Confirm | Existing Bluetooth music pedal | Preferred removable scene trigger if its protocol is ESP32-compatible |
| Conditional | Normally open momentary wired foot switch | Reliable fallback only if Bluetooth pedal is missing or incompatible |
| Confirm | Short USB-C power lead and safe 5V distribution | Connect confirmed DP20S pack to the wearable electronics |
| Conditional | 1/4-inch panel jack, matching cable/plug, strain relief | Detachable stage connection only for the wired fallback |
| Confirm | Compact wearable enclosure and secure mounting hardware | Finished lapel/chest unit |
| Build supply | Perfboard or solderable prototype board, wire, headers/connectors, heat-shrink | Reliable integration after breadboard proof |

## Deferred entryway hardware

| Item | Status |
|---|---|
| Low-voltage pressure pad/mat switch | Primary future trigger beneath welcome mat |
| Normally closed magnetic reed door/contact sensor | Optional arrival confirmation after Tech Week |
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
