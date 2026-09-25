# Circuit Playground halo firmware

This program turns the owned Adafruit Circuit Playground Express into the wearable's ten-NeoPixel halo.

Bench wiring:

- ESP32 GPIO21 to Circuit Playground A1
- ESP32 GND to Circuit Playground GND
- Power each board from its own USB cable for this test
- Do not connect the boards' 3.3V or 5V/VOUT pads together

At idle, two dim cyan pixels show that the halo controller is alive. A rising signal on A1 runs an orange/purple chase followed by a blue fade, then returns to idle.
