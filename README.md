# Shoes Chaos Box

Shoes Chaos Box is a portable, wearable performance prop that responds to a removable foot trigger with a randomized combination of face imagery, captions, tiny video/animation loops, LED effects, and short audio cues. The immediate target is a reliable Tech Week demo with an intentionally improvised, visibly handmade aesthetic; the same content system is presented as the portable stand-in for a pressure-pad welcome mat at home.

## Current target: Tech Week wearable

- The whole assembly pins or clips to a lapel or suit pocket and should look purposefully slapped together rather than productized.
- A small outward-facing screen sits in the middle and shows images, text, and short pre-rendered GIF-like loops.
- The owned Adafruit Circuit Playground Express sits behind or around the screen as a circular LED halo, washing light onto the shirt and emphasizing each scene.
- The wearable also carries the ESP32, audio board, speaker, and local visual storage.
- A USB power bank rides in an inside jacket pocket, with a short power cable routed inside the jacket.
- A removable foot trigger launches scenes. The owner believes an existing Bluetooth music pedal may fill this role; its model and protocol must be verified. A detachable wired switch remains the reliable fallback.
- The wearable includes a local mute/emergency-stop button.
- If the wired fallback is used, cable strain relief and a breakaway-friendly detachable connection are required so a snag does not pull the unit off the wearer.

The default display candidate is the owned 1.28-inch, 240 x 240 GC9A01 round TFT because it suits face-first comedy and has the best resolution of the confirmed displays. The owned ST7735S rectangular TFT remains the alternate for caption-heavy layouts. Both should be bench-tested before the enclosure is finalized.

## Trigger behavior

Each pedal press launches a short scene selected within the current mode. A scene may combine:

1. A still image or pre-rendered animation frames
2. A visual treatment such as alert, glitch, monochrome, neon, or VHS
3. A caption
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
