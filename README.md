# Shoes Chaos Box

Shoes Chaos Box is a portable, wearable performance prop that responds to a foot-switch press with a randomized combination of face imagery, captions, visual treatments, and short audio cues. The immediate target is a polished Tech Week demo; the same electronics and content system can later become an automatic entryway reminder.

## Current target: Tech Week wearable

- A small lapel/chest enclosure holds the ESP32, display, audio board, speaker, and visual-storage module.
- A USB power bank rides in an inside jacket pocket, with a short power cable routed inside the jacket.
- A rugged momentary foot switch connects through a detachable 1/4-inch cable.
- The wearable includes a local mute/emergency-stop button.
- Cable strain relief and a breakaway-friendly detachable connection are required so a snag does not pull the unit off the wearer.

The default display candidate is the owned 1.28-inch, 240 x 240 GC9A01 round TFT because it suits face-first comedy and has the best resolution of the confirmed displays. The owned ST7735S rectangular TFT remains the alternate for caption-heavy layouts. Both should be bench-tested before the enclosure is finalized.

## Trigger behavior

Each foot press launches a short scene selected within the current mode. A scene may combine:

1. A still image or pre-rendered animation frames
2. A visual treatment such as alert, glitch, monochrome, neon, or VHS
3. A caption
4. A short audio cue
5. An optional follow-up frame

Planned modes are Classic, Chaos, Roast, Clean Demo, and Silent. The first reliable milestone is one press producing one image, one caption, and one audio cue; randomization comes after that path is stable.

## Later entryway version

After Tech Week, retain the foot switch as a manual trigger and add a normally closed magnetic door contact plus an owned HC-SR501 PIR sensor. The door contact detects an opening and the PIR confirms activity, followed by a cooldown to reduce false or repeated alerts.

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

