# Shoes Off, Dirtbag!

Shoes Off, Dirtbag! is a portable wearable performance prop that responds to a
Bluetooth foot trigger with randomized circular visuals, synchronized dialogue,
and a scene-aware LED halo. The current bench build plays 27 mapped scenes from
two microSD cards and has cardless opening, QR, and visual-fallback assets.

## Current target: Tech Week wearable

- The whole assembly pins or clips to a lapel or suit pocket and should look purposefully slapped together rather than productized.
- A small outward-facing screen sits in the middle and shows images, text, and short pre-rendered GIF-like loops.
- The owned Adafruit Circuit Playground Express sits behind or around the screen as a circular LED halo, washing light onto the shirt and emphasizing each scene.
- The ESP32, audio board, two cards, buttons, and full breadboard stay together
  as a serviceable pocket unit. The screen, halo, and speaker connect through
  separate 8-, 2-, and 2-conductor leads.
- The confirmed Smatree DP20S USB-C battery pack rides in an inside jacket pocket, with a short power lead routed inside the jacket. It is labeled 5V/2A output and 5000mAh / 18.5Wh.
- The confirmed STRICH SPT-10 pedal uses left/Space for scenes and right/Enter
  for the QR toggle. In Mode 1, Page Up/Down controls saved audio volume.
- The synchronized firmware assigns all 27 scenes their own halo palette. The
  ten original Shoes scenes use distinct diffuser-aware club-light profiles.
- The wearable includes a dedicated local scene-trigger button so the demo still works if the Bluetooth pedal is missing, disconnected, or inconvenient.
- A separate local mute/emergency-stop control remains available during playback.
- If the wired fallback is used, cable strain relief and a breakaway-friendly detachable connection are required so a snag does not pull the unit off the wearer.

The selected display is the owned 1.28-inch, 240 x 240 GC9A01 round TFT. The
ST7735S is no longer part of the active wearable design.

## Deployment target: Hack Alcatraz

The first real outing is **Hack Alcatraz with Cloudflare and Kling AI** on Monday, October 5, 2026. The event runs 5:00–9:00 p.m.; requested arrival is 5:30 p.m., boarding is 5:45 p.m., and the cruise is 6:00–9:00 p.m. The event calls for fun, simple hacks rather than pitch decks and uses rapid 1–2 minute demos before networking on the boat.

The wearable therefore needs at least five hours of ready-to-demo battery life, reliable offline operation, a fast reset between short demonstrations, and a physical design that tolerates movement and unreliable reception on the Bay.

### Optional event-host integrations

- Use Kling AI during content production to turn selected stills or original artwork into tiny reaction loops, then export display-sized frames to the visual microSD card.
- Use Cloudflare Flue as a hosted “Chaos Director” that creates scene manifests by combining captions, media IDs, audio IDs, timing, and LED patterns.
- Keep the generated pack local on the wearable. Flue and Kling enhance what is prepared before the cruise; neither may become a live network dependency.

## Trigger behavior

Each pedal or local-button press launches one short, reusable scene selected within the current mode. The same interaction can be repeated for the wearer or any nearby participant; every press can produce a different combination. A scene may combine:

1. A still image, shoe close-up, or pre-rendered animation frames
2. A visual treatment such as alert, glitch, monochrome, neon, or VHS
3. A caption rendered on top of or alongside the image
4. A short audio cue
5. A Circuit Playground LED pattern or color chase
6. An optional follow-up frame

Planned modes are Classic, Chaos, Roast, Clean Demo, and Silent. The first reliable milestone is one press producing one image, one caption, and one audio cue; randomization comes after that path is stable.

## Demo premise and later home version

The presentation story is: at home, a pressure pad beneath the welcome mat detects the step into the house and chooses a fresh reminder; during the traveling demo, the removable pedal stands in for that mat. A future home build can implement the mat pressure sensor directly, optionally supported by a door contact and an owned HC-SR501 PIR sensor to distinguish a real arrival from other activity.

## Documentation map

- `firmware/idf_wearable/` — verified ESP-IDF screen, button, Bluetooth-pedal, and halo-trigger firmware
- `firmware/circuit_playground_halo/` — verified Arduino/PlatformIO firmware for the Circuit Playground Express LED halo
- `firmware/WIRING_MILESTONE_2.md` — exact synchronized halo bench wiring and power-safety notes
- [PROJECT_STATE.md](PROJECT_STATE.md) — current decisions, scope, risks, and next actions
- [BUILD_PLAN.md](BUILD_PLAN.md) — phased hardware and firmware plan
- [CONTENT_PLAN.md](CONTENT_PLAN.md) — modes, assets, naming, and scene rules
- [HARDWARE_INVENTORY.md](HARDWARE_INVENTORY.md) — consolidated hardware record
- [GEAR_MASTER.md](GEAR_MASTER.md) — authoritative gear taxonomy and configuration
- [GEAR_HAVE.md](GEAR_HAVE.md) — confirmed owned equipment
- [GEAR_NEED.md](GEAR_NEED.md) — purchases and unresolved checks, with links
- [TODO.md](TODO.md) — ordered physical-build and validation checklist
- [PURCHASE_LIST.md](PURCHASE_LIST.md) — reduced connector/mounting order list
- [CHANGELOG.md](CHANGELOG.md) — documentation history

## Project rules

- GitHub is the source of truth for plans and inventory.
- “Have” means confirmed by the owner or visible in the supplied photos; assumptions remain in “Need / confirm.”
- Build the smallest reliable demo first, then add variety.
- Do not depend on Wi-Fi or streaming during a performance.
- Use only audio and visual assets that the project is permitted to reproduce and perform.
