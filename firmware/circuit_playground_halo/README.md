# Circuit Playground halo firmware

This program turns the owned Adafruit Circuit Playground Express into the wearable's ten-NeoPixel halo.

Bench wiring:

- ESP32 GPIO21 to Circuit Playground A1
- ESP32 GND to Circuit Playground GND
- Power each board from its own USB cable for this test
- Do not connect the boards' 3.3V or 5V/VOUT pads together

At the home logo, three broad blue, pink, and yellow groups rotate smoothly
once every 24 seconds, with a gentle six-second pulse at brightness 18–32/255.
At QR, all ten pixels breathe green together every five seconds at brightness
8–20/255, with no chase. Both patterns render at roughly 31 frames per second.
Before
holding A1 high for a scene, the ESP32 sends a short one-wire code containing
the S001-S032 scene number. The Circuit Playground selects that scene's
three-color palette and animates it for the complete scene. The explicit home
command ends any residual scene afterglow when the logo returns.

The scene-code protocol uses the existing GPIO21/A1 and shared-ground wires;
no additional data wire is required. The 32 palettes are curated to the
dominant colors in the circular media exports.

Home and QR use reserved 340 ms and 220 ms high pulses respectively, preceded
by 60 ms low. These commands latch at the falling edge and leave the wire low.
They do not consume normal scene IDs. The ESP32 refreshes the QR command every
five seconds while QR is shown, allowing a late-powered halo to recover. A
fresh sync can replace an incomplete packet; a 100 ms low gap clears an
unfinished packet. Normal scene and Arc active-high signaling is retained.

Arc Core uses a distinct 120 ms sync pulse on that same wire, so it does not
consume or collide with any of the 32 normal scene codes. Its 35.04-second
lighting cycle matches the display media: an immediately visible accelerating
gold chase makes seven revolutions during startup, closes into a bright gold
ring with no white stage, transitions directly to blue at 5.04 seconds, then
performs four slow, high-contrast full-ring blue breathing pulses over the next
30 seconds. Stopping
Arc Core returns the halo directly to idle without a normal-scene afterglow.
If the Circuit Playground restarts while Arc Core is already holding the A1
control line high, it recovers the Arc animation after a 750 ms guard. A serial
status line every five seconds reports input, Arc activity, and decoder state
for hardware diagnosis.

Movie, television, and meme scenes use a restrained moving palette. The ten
original Shoes-video scenes (S018-S027) each have a coherent, diffuser-aware
club profile rather than sharing one continuous chase. Across those clips the
profiles include wide chasing wedges, five-pixel color halves, full-ring blinks,
breathing fades, rainbow rolls, theatrical gold trails, and fast solid cuts.
Brightness is capped at 88/255 and the existing two-second afterglow remains.
Kling scenes S030-S032 add an argument split, phrase-colored verdict pulses,
and a lime/cyan bacteria rave with magenta club hits.

## Timing repair, September 27

Each rendered frame is buffered and transmitted to the LEDs once, avoiding
visible partial clearing/redraw. Scene phrases use time since scene start;
rotation has a separate persistent position so restarting a phrase does not
restart the chase at LED zero. Motion interpolates between LED positions and
renders every 16 ms. The 45/25 ms effect tempos are retained from the firmware
before this repair, not claimed as a recreation of the earliest prototype.

Host regression checks include phrase restart, continuous position, interpolation
across the last/first LED, delayed-loop catch-up, clock wrap, and a single strip
update for every one of the 32 profiles. They also cover Arc Core's long-sync
decode and fast-gold/blue timing:

```sh
c++ -std=c++17 -I tests/host tests/host/test.cpp -o /tmp/halo-test
/tmp/halo-test
python3 tests/host/test_link.py
```

The link test runs the real ESP32 pulse senders against the halo loop. It checks
home/QR colors, brightness limits, motion, every scene ID, Arc transitions,
interrupted packets, and QR refresh after a receiver restart.

The SAM-BA uploader failed on this board. The repair was installed using a UF2
image at application address 0x2000, preserving its bootloader. Application
readback after a 1200-baud reset matched apart from the first 256 bytes, which
this installed SAMD core deliberately erases when entering its bootloader.
The complete UF2 was then reinstalled to restore that startup page. Scene-code
reception is checked separately in the combined playback test.
