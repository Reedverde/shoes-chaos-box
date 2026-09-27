# Purchase List

Updated: 2026-09-26

The core electronics are already owned and working. The first wearable keeps
the full breadboard in the pocket and moves only the screen, speaker, and halo
onto detachable leads. Do **not** order another ESP32, display, Circuit
Playground, DFPlayer, speaker, SD reader, SD card, resistor kit, battery pack,
relay, capacitor, perfboard, or project enclosure for the present build.

## Final order for tonight

| Buy | Quantity | Selection rule | Exact purpose |
|---|---:|---|---|
| Prewired locking JST-SM 8-pin male/female pair | 1 | 20 cm leads on each half, 22-26 AWG | One keyed removable display cable; about 40 cm total |
| Prewired locking JST-SM 2-pin male/female pair | 1 | 20 cm leads on each half, 22-26 AWG | Dedicated DFPlayer SPK1/SPK2 cable |
| Prewired locking JST-SM 3-pin male/female pair | 1 | 20 cm leads on each half; use only the two outside wires | Dedicated GPIO21/A1 + GND halo cable that cannot be confused with the speaker plug |
| Adhesive-lined heat-shrink assortment | 1 | Small diameters suitable for 22-28 AWG wire | Insulate and reinforce every solder splice |
| M2/M2.5/M3 nylon standoff/screw/nut assortment | 1 | Mixed 6-20 mm lengths | Space and fasten the screen/Circuit Playground lapel sandwich |

Amazon search shortcuts:

- [8-pin JST-SM prewired pair](https://www.amazon.com/s?k=8+pin+JST+SM+connector+20cm+prewired)
- [2-pin JST-SM prewired pair](https://www.amazon.com/s?k=2+pin+JST+SM+connector+20cm+prewired)
- [3-pin JST-SM prewired pair](https://www.amazon.com/s?k=3+pin+JST+SM+connector+20cm+prewired)
- [Adhesive-lined heat shrink](https://www.amazon.com/s?k=adhesive+lined+heat+shrink+small+diameter)
- [M2 M2.5 M3 nylon standoff kit](https://www.amazon.com/s?k=M2+M2.5+M3+nylon+standoff+kit)

The existing clipped battery pack is the lapel attachment, and the owned
plastic piece is the rigid nonconductive backing. Do not order another backing,
badge mount, enclosure, Dual Lock, or lapel hardware. The daughter's 3D pen may
add small cable guides after assembly, but the nylon fasteners—not printed
plastic—carry the screen and board.

## Buy only if not already owned

- A temperature-controlled soldering iron, electronics solder, flux pen, and
  helping-hands holder. The final detachable leads should not rely on twisted
  wires or loose Dupont joints.
- A basic digital multimeter with continuity mode. The final rail layout must
  be checked before attaching modules.

## Do not buy yet

- A 1000 uF capacitor: add one only if the final wearable shows measured
  brownouts or audio-peak resets.
- Perfboard: the breadboard is intentionally staying in the pocket for the first
  wearable. Buy perfboard only for a later permanent rebuild.
- A 12-pin combined connector: separate 8 + 2 + 2-conductor leads are easier to service
  and keep the DFPlayer bridge output away from display power/signal wiring.
- Another battery: the DP20S plus the existing clipped Circuit Playground pack
  is the selected first-build power architecture.
- Wired foot pedal hardware: Bluetooth plus the local button already provide
  primary and fallback triggering.

## Connector buying rule

Buy prewired male/female pigtail pairs; do not buy loose housings or a crimp
tool. Use the 8-pin plug for `DISPLAY`, the 2-pin plug for `SPEAKER`, and the
3-pin housing with its middle position unused for the two-wire `HALO` lead.
Label both ends. This mechanically prevents the speaker and halo cables from
being exchanged.
