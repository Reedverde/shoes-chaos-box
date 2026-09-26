# Hardware Inventory

The current installed system is summarized below. Detailed ownership is in
[GEAR_HAVE.md](GEAR_HAVE.md), purchases in [PURCHASE_LIST.md](PURCHASE_LIST.md),
and physical work in [TODO.md](TODO.md).

## Pocket electronics

- Classic ESP32 DevKit (USB-C/CP2102)
- Full-size breadboard
- DFPlayer Mini and Card A audio card
- HW-125/HiLetgo SPI reader and Card B visual card
- GPIO13 scene button and GPIO22 QR button
- 1 kΩ ESP32-TX-to-DFPlayer-RX resistor
- Smatree DP20S USB power bank

## Lapel electronics

- GC9A01 round display
- Circuit Playground Express behind a diffuser
- One 4-ohm, 3-watt speaker
- Independent clipped Circuit Playground battery pack

## Frozen ESP32 pin map

| Function | ESP32 pin |
|---|---:|
| Display/SD SCLK | GPIO14 |
| Display/SD MOSI | GPIO27 |
| Display CS | GPIO26 |
| Display DC | GPIO25 |
| Display reset | GPIO33 |
| Display backlight | GPIO32 |
| Visual SD MISO | GPIO19 |
| Visual SD CS | GPIO5 |
| Local scene button | GPIO13 |
| Local QR button | GPIO22 |
| Halo scene code | GPIO21 |
| DFPlayer ESP-RX | GPIO16 |
| DFPlayer ESP-TX | GPIO17 through 1 kΩ to player RX |

## Wearable cables

- 8-pin display lead: 3V3, GND, GPIO14, GPIO27, GPIO26, GPIO25, GPIO33, GPIO32
- 2-pin halo lead: GPIO21/A1 and shared GND
- 2-pin speaker lead: DFPlayer SPK1 and SPK2

The visual reader stays in the pocket and is reoriented flat; its six existing
connections do not need to cross to the lapel.
