# Circuit Playground halo firmware

This program turns the owned Adafruit Circuit Playground Express into the wearable's ten-NeoPixel halo.

Bench wiring:

- ESP32 GPIO21 to Circuit Playground A1
- ESP32 GND to Circuit Playground GND
- Power each board from its own USB cable for this test
- Do not connect the boards' 3.3V or 5V/VOUT pads together

At idle, two dim cyan pixels show that the halo controller is alive. Before
holding A1 high for a scene, the ESP32 sends a short one-wire code containing
the S001-S027 scene number. The Circuit Playground selects that scene's
three-color palette, animates it for the complete scene, fades the same palette
for two seconds, and returns to idle.

The scene-code protocol uses the existing GPIO21/A1 and shared-ground wires;
no additional data wire is required. The 27 palettes are curated to the
dominant colors in the circular media exports.

Movie, television, and meme scenes use a restrained moving palette. The ten
original Shoes-video scenes (S018-S027) each have a coherent, diffuser-aware
club profile rather than sharing one continuous chase. Across those clips the
profiles include wide chasing wedges, five-pixel color halves, full-ring blinks,
breathing fades, rainbow rolls, theatrical gold trails, and fast solid cuts.
Brightness is capped at 88/255 and the existing two-second afterglow remains.

**Physical status:** this source builds successfully. The final diffuser-aware
revision still needs to be reflashed after an interrupted SAM-BA upload: put the
board in `CPLAYBOOT`, upload, and verify before relying on the halo.
