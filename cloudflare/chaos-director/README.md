# Shoes Chaos Director

Cloudflare Worker that generates deterministic scene packs for the **Shoes
Off, Dirtbag!** ESP32 wearable. It arranges known scene IDs, preserves the
visual/audio mapping, and exports a firmware-compatible `SCENES.CSV`.

The service does **not** host or redistribute the movie, TV, meme, or Shoes
media. Those validated files remain on the two device microSD cards.

## Pack endpoints

- `/api/packs` — available pack definitions
- `/api/packs/event-chaos?seed=demo-night` — 48-play JSON pack
- `/api/packs/event-chaos/scenes.csv?seed=demo-night` — CSV download
- `/api/packs/clean-demo` — ten-play clean demonstration pack
- `/api/packs/emerald-preview` — Wizard-led halo diagnostic pack
- `/api/catalog` — the 32-scene metadata catalog
- `/api/health` — deployment health

`event-chaos` preserves the current performance rule: every outside-source
scene is followed by a Shoes-family scene, and S003, S028, S029, and S030 each
appear twice in the complete run.

## Local use

```sh
npm install
npm test
npm run dev
```

Deploy with `npm run deploy` after authenticating Wrangler. Cloudflare is the
pack-production layer; the wearable remains fully offline during playback.

## Kling AI integration

Three validated Kling AI loops are catalogued as S030-S032. Their generated
media stays on Card B; the Worker exposes metadata, provenance, mappings, and
deterministic packs without hosting source media. They reuse audio 0003, 0020,
and 0023 from Card A.
