# Shoes Chaos Director

Cloudflare Worker that generates deterministic scene packs for the **Shoes
Off, Dirtbag!** ESP32 wearable. It arranges known scene IDs, preserves the
visual/audio mapping, and exports a firmware-compatible `SCENES.CSV`.

The service does **not** host or redistribute the movie, TV, meme, or Shoes
media. Those validated files remain on the two device microSD cards.

## Pack endpoints

- `/api/packs` — available pack definitions
- `/api/packs/event-chaos?seed=demo-night` — 44-play JSON pack
- `/api/packs/event-chaos/scenes.csv?seed=demo-night` — CSV download
- `/api/packs/clean-demo` — ten-play clean demonstration pack
- `/api/packs/emerald-preview` — Wizard-led halo diagnostic pack
- `/api/catalog` — the 29-scene metadata catalog
- `/api/health` — deployment health

`event-chaos` preserves the current performance rule: every outside-source
scene is followed by a Kelly/Shoes scene, and S003, S028, and S029 each appear
twice in the complete run.

## Local use

```sh
npm install
npm test
npm run dev
```

Deploy with `npm run deploy` after authenticating Wrangler. Cloudflare is the
pack-production layer; the wearable remains fully offline during playback.

## Kling AI handoff

Kling AI loops should be rendered and validated by the existing 240-pixel
circular export pipeline before being added to `src/catalog.js`. Record a new
scene ID, source permission, prompt, export settings, matching DFPlayer track,
frame timing, and halo treatment. Do not mark legacy source footage as
Kling-generated.
