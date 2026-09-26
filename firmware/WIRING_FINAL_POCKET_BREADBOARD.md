# Final Pocket Breadboard Wiring

This is the target layout for the first wearable build. It keeps the working
full-size breadboard in the pocket while making the display, speaker, and halo
assemblies detachable.

## Orientation

- Breadboard numbers increase from left to right.
- The ESP32 straddles the center trench at **columns 0–18**, with its USB-C
  connector at column 0.
- In the diagrams, `A–E` is the upper half and `F–J` is the lower half.
- Module pin labels, not wire colors, are authoritative.

```text
                 accessible top / far end

 top red rail    +5 V ================================================
 top blue rail   GND  ================================================

                 0        18 20   23--28    32-------39   44/46  51/53
 A–E side       [   ESP32   ][G] [VISUAL SD] [ DFPLAYER ] [SCENE] [ QR ]
                 | straddles | |   lays flat   straddles    buttons
 ----------------+-----------+-+-------------------------------------- center trench
 F–J side       [   ESP32   ][G]             [ DFPLAYER ]

 bottom red rail +3.3 V ==============================================
 bottom blue     GND  ================================================

 [G] at column 20 is the dedicated ground breakout channel.
```

The positions after the ESP32 allow a little service space. A module may move
one or two columns for mechanical clearance without changing its electrical
mapping.

## 1. Install the ESP32 at columns 0–18

Place the two ESP32 header rows in `B0–B18` and `I0–I18`, USB-C toward column
0. This is the current verified firmware pin map after shifting the old
columns 1–19 layout left by one.

### Upper header, B row

| Column | ESP32 pin | Use |
|---:|---|---|
| 0 | VIN / 5V | Feed top +5 V rail |
| 4 | GPIO13 | Scene button |
| 5 | GND | Ground source |
| 7 | GPIO14 | Shared SPI clock |
| 8 | GPIO27 | Shared SPI MOSI |
| 9 | GPIO26 | Display CS |
| 10 | GPIO25 | Display DC |
| 11 | GPIO33 | Display reset |
| 12 | GPIO32 | Display backlight |
| 18 | 3V3 | Feed bottom +3.3 V rail |

All unlisted upper-header pins remain unused by this build.

### Lower header, I row

| Column | ESP32 pin | Use |
|---:|---|---|
| 7 | GPIO16 / RX2 | From DFPlayer TX |
| 8 | GPIO17 / TX2 | Through 1 kOhm resistor to DFPlayer RX |
| 9 | GPIO5 | Visual SD CS |
| 11 | GPIO19 | Visual SD MISO |
| 12 | GND | Alternate ground source |
| 13 | GPIO21 | Circuit Playground halo signal |
| 16 | GPIO22 | QR button |
| 18 | GND | Alternate ground source |

Do not use the ESP32 flash pins marked `CLK`, `D0`, `D1`, `D2`, `D3`, or
`CMD`.

## 2. Establish the power rails before adding modules

With all power disconnected:

1. Connect ESP32 `VIN/5V` at B0 to the **top red rail**.
2. Connect ESP32 `3V3` at B18 to the **bottom red rail**.
3. Connect ESP32 `GND` at B5 to the **top blue rail**.
4. Connect the top blue rail to the bottom blue rail with one ground jumper.
5. If either rail is split in the middle, bridge each half to the **same-colored,
   same-voltage** half and verify continuity with a meter.

Never connect the two red rails together. The top rail is 5 V; the bottom rail
is 3.3 V.

## 3. Make the column-20 ground breakout

Column 20 is reserved exclusively for ground:

1. Jumper the top blue ground rail to A20.
2. Jumper E20 across the center trench to F20.
3. Jumper J20 to the bottom blue ground rail.

This makes `A20–E20` and `F20–J20` one common ground point. Future ground
connections can branch from an open hole in column 20 or either blue rail.
Never place a signal or positive-power wire in column 20.

## 4. Place the visual SD reader after the ground channel

Place the HW-125 visual reader around columns **23–28**. It must lie flat. Use
a right-angle six-pin header or a short secured six-wire pigtail; do not leave
the reader standing vertically.

Match these connections by the labels printed on the module. The order of the
module's six pins can vary.

| HW-125 label | Connect to |
|---|---|
| VCC | Top +5 V rail |
| GND | Column-20 ground or blue rail |
| SCK / CLK | ESP32 GPIO14, B7 |
| MOSI | ESP32 GPIO27, B8 |
| MISO | ESP32 GPIO19, I11 |
| CS | ESP32 GPIO5, I9 |

Keep the firmware's verified 4 MHz SD clock setting.

## 5. Place the DFPlayer after the visual reader

Place the DFPlayer across the center trench around columns **32–39**, with Card
A's slot and the volume/speaker wiring accessible. Follow the labels printed
on the DFPlayer rather than assuming a pin order from its orientation.

| DFPlayer label | Connect to |
|---|---|
| VCC | Top +5 V rail |
| GND | Column-20 ground or blue rail |
| TX | ESP32 GPIO16 / RX2, I7 |
| RX | 1 kOhm resistor, then ESP32 GPIO17 / TX2, I8 |
| SPK1 | One conductor of dedicated speaker lead |
| SPK2 | Other conductor of dedicated speaker lead |

Do not connect either speaker conductor to ground. `BUSY`, `ADKEY`, `IO`, and
`DAC` pins remain unused.

## 6. Put the buttons at the accessible end

Place each four-leg tactile switch across the center trench so its two sides
cannot be confused.

- **SCENE button:** around columns 44/46. One side to GPIO13 at B4; the
  opposite side to ground.
- **QR button:** around columns 51/53. One side to GPIO22 at I16; the opposite
  side to ground.

The firmware uses internal pull-ups, so the buttons need no external resistor.
Pressing a button must connect its GPIO to ground, not to 3.3 V or 5 V.

## 7. Detachable leads

### Display: one keyed 8-conductor lead

| Lead pin | Display label | ESP32 / rail |
|---:|---|---|
| 1 | VCC | Bottom +3.3 V rail |
| 2 | GND | Ground |
| 3 | CLK / SCL | GPIO14 |
| 4 | DIN / SDA | GPIO27 |
| 5 | CS | GPIO26 |
| 6 | DC | GPIO25 |
| 7 | RST / RES | GPIO33 |
| 8 | BL / BLK | GPIO32 |

Use a keyed connector and label both ends `DISPLAY`. Do not substitute the 5 V
rail for pin 1.

### Halo controller: one keyed 2-conductor lead

- GPIO21 to Circuit Playground A1
- Common ground to Circuit Playground GND

The Circuit Playground's positive battery wire remains isolated and uses its
own clipped battery pack. Do not join its positive supply to either red rail.

### Speaker: one keyed 2-conductor lead

- DFPlayer SPK1
- DFPlayer SPK2

Label this lead `SPEAKER`; neither conductor is ground.

## 8. Power-off inspection and bring-up order

1. Confirm no red rail is connected to a blue rail.
2. Confirm the 5 V and 3.3 V red rails are not connected to one another.
3. Confirm column 20 is ground everywhere and contains no signal wire.
4. Confirm the display receives 3.3 V, not 5 V.
5. Confirm the Circuit Playground battery-positive wire is isolated.
6. Confirm the speaker uses SPK1 and SPK2 only.
7. Insert Card A in the DFPlayer and Card B in the HW-125.
8. Power the ESP32 without the detachable peripherals and measure both rails.
9. Power off; attach the display, then test it.
10. Power off; attach DFPlayer/speaker, then test audio.
11. Power off; attach halo signal/ground, then power its separate battery.
12. Test both local buttons and both pedal sides.

Photograph the completed wiring before closing the pocket enclosure.
