# Milestone 1 Wiring

This wiring proves only the ESP32, round GC9A01 screen, and local trigger button. Leave the Circuit Playground, relay board, breadboard power module, battery holder, DFPlayer, speaker, and microSD module disconnected.

## Before connecting power

- Unplug the ESP32 USB-C cable.
- Place the ESP32 with its pin rows in `B1–B19` and `J1–J19`, components facing up, with USB-C beside row 1.
- Place the screen header in `A25–A32`, with `GND` at row 32.
- Match every connection to the labels printed on the actual boards.
- Use jumper wires, not alligator clips, on the ESP32 and display headers.

## Round screen to ESP32

The display's `SDA` label means SPI MOSI in this module; it is not an I2C connection.

| Wire | Round display | ESP32 label | Purpose |
|---|---|---|---|
| `B32 → A6` | GND | GND | Common ground |
| `B31 → A19` | VCC | 3V3 | Display power |
| `B30 → A8` | SCL | G14 | SPI clock |
| `B29 → A9` | SDA | G27 | SPI data / MOSI |
| `B28 → A12` | RES | G33 | Display reset |
| `B27 → A11` | DC | G25 | Data/command select |
| `B26 → A10` | CS | G26 | Display chip select |
| `B25 → A13` | BLK | G32 | Backlight enable |

## Trigger button to ESP32

Use one large momentary pushbutton. In the photographed bench layout its legs occupy `D36`, `D38`, `G36`, and `G38`.

| Button side | ESP32 label |
|---|---|
| One contact (`A36`) | G13 (`A5`) |
| Opposite contact (`J36`) | GND (`E32`) |

The firmware enables the ESP32's internal pull-up resistor, so no external resistor is required for this button. On a four-legged tactile switch, the two legs on each side are already joined internally; use contacts from opposite sides.

## First power-on expectation

1. Connect the ESP32 to the computer using the confirmed USB-C data cable.
2. Upload the firmware.
3. The display should show `SHOES CHAOS BOX` inside blue rings.
4. Each button press should show one reminder scene and return to idle after 2.2 seconds.

If the display remains blank, disconnect USB before changing any wire. Do not move wires while powered.
