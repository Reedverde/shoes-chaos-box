# Milestone 1 Wiring

This wiring proves only the ESP32, round GC9A01 screen, and local trigger button. Leave the Circuit Playground, relay board, breadboard power module, battery holder, DFPlayer, speaker, and microSD module disconnected.

## Before connecting power

- Unplug the ESP32 USB-C cable.
- Place the ESP32 across the breadboard center channel with its components facing up.
- Match every connection to the labels printed on the actual boards.
- Use jumper wires, not alligator clips, on the ESP32 and display headers.

## Round screen to ESP32

The display's `SDA` label means SPI MOSI in this module; it is not an I2C connection.

| Round display | ESP32 label | Purpose |
|---|---|---|
| GND | GND | Common ground |
| VCC | 3V3 | Display power |
| SCL | G18 | SPI clock |
| SDA | G23 | SPI data / MOSI |
| RES | G33 | Display reset |
| DC | G25 | Data/command select |
| CS | G27 | Display chip select |
| BLK | G32 | Backlight enable |

## Trigger button to ESP32

Use one large momentary pushbutton.

| Button side | ESP32 label |
|---|---|
| One contact | G13 |
| Opposite contact | GND |

The firmware enables the ESP32's internal pull-up resistor, so no external resistor is required for this button. On a four-legged tactile switch, the two legs on each side are already joined internally; use contacts from opposite sides.

## First power-on expectation

1. Connect the ESP32 to the computer using the confirmed USB-C data cable.
2. Upload the firmware.
3. The display should show `SHOES CHAOS BOX` inside blue rings.
4. Each button press should show one reminder scene and return to idle after 2.2 seconds.

If the display remains blank, disconnect USB before changing any wire. Do not move wires while powered.

