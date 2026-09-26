# Purchase List

Updated: 2026-09-26

The core electronics are already owned and working. Do **not** order another
ESP32, display, Circuit Playground, DFPlayer, speaker, SD reader, SD card,
resistor kit, battery pack, relay, or capacitor for the present build.

## Buy only if these are not already in the parts box

| Priority | Item | Quantity | Exact purpose |
|---|---|---:|---|
| Required | Keyed locking 8-conductor inline pigtail, 26-28 AWG, flexible, approximately 35-45 cm | 1 pair | Removable display lead |
| Required | Keyed locking 2-conductor inline pigtail, flexible, approximately 35-45 cm | 2 pairs | One pair for SPK1/SPK2; one for GPIO21/A1 + GND |
| Required | Six-conductor short flexible pigtail or right-angle 2.54 mm header solution | 1 | Lay the HW-125 visual SD reader flat beside the breadboard |
| Required | Small heat-shrink assortment | 1 | Insulate solder joints and label lead ends |
| Required | Small reusable cable ties or narrow hook-and-loop ties | 1 pack | Strain relief and service loops |
| Required | Shallow nonconductive pocket tray/case or lightweight backing sheet | 1 | Prevent the breadboard underside from contacting metal or fabric |
| Required | Nylon standoffs, screws, and nuts sized to the GC9A01/Circuit Playground holes | 1 small kit | Lapel sandwich spacing and diffuser clearance |
| Confirm | 26-28 AWG stranded silicone hookup wire in several colors | 1 kit | Extend pigtails and keep pin identities obvious |
| Confirm | Locking badge pins or a pocket-edge clip plus lower stabilizer | 2 attachment points | Stop the lapel badge rotating |
| Confirm | Thin translucent diffuser material | 1 small sheet | Spread halo light around the circular screen |

## Do not buy yet

- A 1000 uF capacitor: add one only if the final wearable shows measured
  brownouts or audio-peak resets.
- Perfboard: the breadboard is intentionally staying in the pocket for the first
  wearable. Buy perfboard only for a later permanent rebuild.
- A 12-pin combined connector: separate 8 + 2 + 2 leads are easier to service
  and keep the DFPlayer bridge output away from display power/signal wiring.
- Another battery: the DP20S plus the existing clipped Circuit Playground pack
  is the selected first-build power architecture.
- Wired foot pedal hardware: Bluetooth plus the local button already provide
  primary and fallback triggering.

## Connector buying rule

Buy pre-crimped male/female pigtail pairs unless the matching crimp tool is
already owned. Use three visibly different connector colors or label both ends
`DISPLAY`, `SPEAKER`, and `HALO`; never make the two 2-pin plugs interchangeable.
