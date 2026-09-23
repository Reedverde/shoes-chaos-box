# Shoes Chaos Box

Shoes Chaos Box is a portable, wearable performance prop that responds to a removable foot trigger with a randomized combination of face imagery, captions, tiny video/animation loops, LED effects, and short audio cues. The immediate target is a reliable Tech Week demo with an intentionally improvised, visibly handmade aesthetic; the same content system is presented as the portable stand-in for a pressure-pad welcome mat at home.

## Current target: Tech Week wearable

- The whole assembly pins or clips to a lapel or suit pocket and should look purposefully slapped together rather than productized.
- A small outward-facing screen sits in the middle and shows images, text, and short pre-rendered GIF-like loops.
- The owned Adafruit Circuit Playground Express sits behind or around the screen as a circular LED halo, washing light onto the shirt and emphasizing each scene.
- The wearable also carries the ESP32, audio board, speaker, and local visual storage.
- The confirmed Smatree DP20S USB-C battery pack rides in an inside jacket pocket, with a short power lead routed inside the jacket. It is labeled 5V/2A output and 5000mAh / 18.5Wh.
- A removable foot trigger launches scenes. The owner believes an existing Bluetooth music pedal may fill this role; its model and protocol must be verified. A detachable wired switch remains the reliable fallback.
- The wearable includes a dedicated local scene-trigger button so the demo still works if the Bluetooth pedal is missing, disconnected, or inconvenient.
- A separate local mute/emergency-stop control remains available during playback.
- If the wired fallback is used, cable strain relief and a breakaway-friendly detachable connection are required so a snag does not pull the unit off the wearer.

The default display candidate is the owned 1.28-inch, 240 x 240 GC9A01 round TFT because it suits face-first comedy and has the best resolution of the confirmed displays. The owned ST7735S rectangular TFT remains the alternate for caption-heavy layouts. Both should be bench-tested before the enclosure is finalized.

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

- [PROJECT_STATE.md](PROJECT_STATE.md) — current decisions, scope, risks, and next actions
- [BUILD_PLAN.md](BUILD_PLAN.md) — phased hardware and firmware plan
- [CONTENT_PLAN.md](CONTENT_PLAN.md) — modes, assets, naming, and scene rules
- [HARDWARE_INVENTORY.md](HARDWARE_INVENTORY.md) — consolidated hardware record
- [GEAR_MASTER.md](GEAR_MASTER.md) — authoritative gear taxonomy and configuration
- [GEAR_HAVE.md](GEAR_HAVE.md) — confirmed owned equipment
- [GEAR_NEED.md](GEAR_NEED.md) — purchases and unresolved checks, with links
- [CHANGELOG.md](CHANGELOG.md) — documentation history

## Project rules

- GitHub is the source of truth for plans and inventory.
- “Have” means confirmed by the owner or visible in the supplied photos; assumptions remain in “Need / confirm.”
- Build the smallest reliable demo first, then add variety.
- Do not depend on Wi-Fi or streaming during a performance.
- Use only audio and visual assets that the project is permitted to reproduce and perform.
