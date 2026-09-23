# Content Plan

## Creative direction

The device should feel like a tiny randomized performance character, not a single repeating sound button. Every trigger launches a short “scene” assembled from compatible visual, caption, audio, and effect assets.

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

- 15–25 still images
- 5–10 animations, stored as short numbered frame sequences
- 30–50 captions
- 10–20 short audio cues
- 8–10 looks: normal, monochrome, posterized, neon, glitch, VHS, red alert, pixelated, zoom burst, and comic halftone

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
SCENE_001,CLEAN,IMG_004,CAP_012,AUDIO_003,LOOK_ALERT,3500
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
- Do not distribute ripped video/audio in this repository.
- Use an original performance, licensed excerpt, or other cleared audio inspired by the desired comic timing.
- Obtain permission before using identifiable face images, especially for public performance or publication.
- Keep Clean Demo content free of personal attacks, private references, and unexpected high-volume cues.

## Scene-selection rules

1. Select only assets allowed in the current mode.
2. Avoid repeating the last image, caption, or audio cue when alternatives exist.
3. Respect audio/animation duration so the scene ends cleanly.
4. Give emergency stop and mute priority over all playback.
5. Return to the idle screen after each scene.
6. Log or expose a debug scene ID during development so failures are reproducible.

