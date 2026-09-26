# Content Plan

## Creative direction

The device should feel like a tiny randomized performance character, not a single repeating sound button. Its physical language is intentionally homemade: a small center screen, an LED ring glowing onto the shirt, visible construction, and a surprising amount of media. Every trigger launches a short “scene” assembled from compatible visual, caption, audio, screen effect, and LED assets.

## Demonstration story

The spoken setup is: “At home, there is a pressure pad beneath the welcome mat. When I step into the house, it reminds me to take off my shoes, but it mixes the reminders up so they stay fresh and funny. Today this removable foot pedal is standing in for the mat.”

The stage/demo mode and future mat installation should call the same scene-selection function; only the physical trigger changes. It is not limited to the owner: a visitor stepping on the future mat, or a participant pressing the boat-demo pedal, receives the same randomized reminder experience.

Each scene follows the same reusable rhythm: trigger, reveal a face or shoe shot on the round screen, add a short caption over or beside it, play the matching sound and LED animation, then reset to idle for the next person. The assets can repeat, but the combinations should stay varied enough that several consecutive demonstrations feel fresh.

## Production tools

- **Kling AI:** optional source for very short image-to-video reaction loops. Export the result into optimized local frame sequences; record the source, prompt, permission, and export settings in the manifest.
- **Cloudflare Flue / Chaos Director:** optional hosted agent that proposes captions and assembles valid combinations of existing image, audio, look, and LED IDs. It must output the same deterministic `scenes.csv` format used by hand-authored packs.
- AI-generated assets and manifests always pass a human review before being copied to the device.

## Modes

| Mode | Behavior |
|---|---|
| Classic | Recognizable shoe-warning energy and signature-style reactions |
| Chaos | Broadest random pool and the strangest combinations |
| Roast | Sharper but still event-appropriate fashion commentary |
| Clean Demo | Curated, short, predictable, and safe for showing new people |
| Silent | Visuals only |

An optional escalation sequence can replace pure randomization: gentle warning, stronger warning, chaotic reaction, then a reset.

## Initial content targets

The initial target has been surpassed. The current validated pack contains 27
five-to-seven-second scenes: 17 outside-source or shoe-icon scenes and ten
moments from the original Shoes video. Runtime Card B uses eight evenly spaced
RGB565 keyframes per scene; Card A contains the matching numbered MP3. S001
maps to `0001.mp3` through S027/`0027.mp3`.

Current playback alternates an S001-S017 scene with an S018-S027 Shoes scene.
S003 Curb appears twice per 36-play deck; every other outside scene appears
once. Shoes separators are shuffled without banking triggers.

- 15–25 still images
- 5–10 animations, stored as short numbered frame sequences
- 30–50 captions
- 10–20 short audio cues
- 8–10 looks: normal, monochrome, posterized, neon, glitch, VHS, red alert, pixelated, zoom burst, and comic halftone
- 6–10 LED patterns: idle pulse, color wipe, chase, alarm spin, flash, sparkle, success fade, and blackout

Start with five fully curated Clean Demo scenes before expanding the pools.

## Asset layout

```text
/content
  /images
    IMG_001.raw
  /animations
    /ANIM_001
      FRAME_001.raw
      FRAME_002.raw
  /audio
    AUDIO_001.mp3
  /led
    led_patterns.csv
  captions.csv
  scenes.csv
  manifest.csv
```

The final bitmap format depends on the selected display library. Store source artwork outside the device export folders; export optimized, display-sized assets for runtime.

## Naming and metadata

- Use uppercase ASCII filenames with fixed-width numbers to avoid card/filesystem surprises.
- Give every asset a stable ID.
- Tag assets by allowed mode, intensity, duration, and compatibility.
- Keep audio numbering compatible with the DFPlayer folder/track convention selected during testing.
- Track content provenance and permission in the source manifest.

Example scene record:

```text
SCENE_001,CLEAN,IMG_004,CAP_012,AUDIO_003,LOOK_ALERT,LED_CHASE_RED,3500
```

## Starter caption bank

### Shoe warnings

- TAKE OFF THE SHOES
- WE HAVE A SHOE SITUATION
- THIS IS A NO-SHOE ZONE
- FOOTWEAR DETECTED
- SHOE EVENT IN PROGRESS
- PLEASE DE-SHOE YOURSELF
- THE SHOES ARE THE PROBLEM
- REMOVE THE EVIDENCE

### Absurd dramatic

- ALERT: UNAUTHORIZED SHOE ENERGY
- THE PROPHECY SPOKE OF THIS
- THIS CAN STILL BE FIXED
- CHOOSE BAREFOOT PEACE
- THE COUNCIL REJECTS THESE SHOES
- THIS IS NOW A FULL-SCALE INCIDENT
- THE FLOOR IS BEGGING FOR MERCY

### Deadpan

- INTERESTING CHOICE.
- BOLD.
- THAT IS CERTAINLY FOOTWEAR.
- WE NOTICED.
- THIS DID NOT HAVE TO HAPPEN.
- LET'S RESET AND TRY AGAIN.
- THE SHOES HAVE OVERSTEPPED.

### Tech Week / booth

- TECH WEEK CANNOT HANDLE THIS
- THIS IS CAUSING SIGNAL LOSS
- PLEASE RESOLVE THE SHOE FEED
- LATENCY INTRODUCED BY FOOTWEAR
- AUDIO IS FINE. IT IS THE SHOES.
- THIS IS NOW A FRONT-OF-HOUSE ISSUE
- THIS IS NOT IN THE RUN OF SHOW

### Meme energy

- THIS IS A SHOE JUMPSCARE
- ABSOLUTELY NOT
- YOU CAN STILL TURN BACK
- THE AUDACITY
- PLEASE EXPLAIN YOURSELF
- WE WILL NOT BE DOING THAT TODAY
- INCREDIBLE. DEEPLY CONCERNING.

## Content safety and rights

- Do not make the device depend on YouTube or network access.
- YouTube is the creative source for desired images, moments, and little clips, but the live device must not depend on streaming.
- Do not commit or distribute ripped video/audio in this repository. Keep source references and permissions in the manifest.
- For any public or distributed version, use licensed/cleared excerpts, original recreations, or other permitted assets with the desired comic timing.
- Obtain permission before using identifiable face images, especially for public performance or publication.
- Keep Clean Demo content free of personal attacks, private references, and unexpected high-volume cues.

## Scene-selection rules

1. Select only assets allowed in the current mode.
2. Avoid repeating the last image, caption, or audio cue when alternatives exist.
3. Respect audio/animation duration so the scene ends cleanly.
4. Give emergency stop and mute priority over all playback.
5. Synchronize the Circuit Playground pattern with the screen/audio scene and cap LED brightness globally.
6. Return to the idle screen and idle LED state after each scene.
7. Log or expose a debug scene ID during development so failures are reproducible.
