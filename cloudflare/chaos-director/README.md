# Chaos Director: content packs for Shoes Off, Dirtbag!

A Cloudflare Worker with Static Assets that lets people download an original two-card starter pack and explore the project's performance catalog. The wearable plays offline; this service runs before a demo, not during it.

**Live:** [Content library](https://shoes-chaos-director.reed-5c2.workers.dev/) · [Starter ZIP](https://shoes-chaos-director.reed-5c2.workers.dev/downloads/shoes-starter-v1.zip) · [Service status](https://shoes-chaos-director.reed-5c2.workers.dev/api/health).

Published September 28, 2026 through Cloudflare's official MCP/API connection. The live catalog, playlist routes, download headers, ZIP checksum, and manifest passed `tools/check_service.py`. The versioned [starter ZIP](public/downloads/shoes-starter-v1.zip) is also available in GitHub. Physical playback of the new synthetic starter remains untested.

## What you get

- A ~473 KB ZIP with 32 original geometric test screens, 29 quiet MP3 tone tracks, card indexes, instructions and checksums.
- A 32-scene performance catalog, including Kling scene provenance and audio reuse mappings. Performance clips are not hosted here.
- Seeded playlist previews: Event Chaos (48 plays), Clean Demo (10), Emerald Preview (6). “Clean” is a selection label, not a content rating; review actual media for the audience.
- Clear separation between a device catalog and a playlist. The current firmware creates its own order and does not import cloud playlists.

Flue is **not** used in this implementation. See [Cloudflare and Flue explained](../../docs/CLOUDFLARE.md) for deployment and an optional future integration path.

## Run it

Node.js 22 or later:

```sh
npm ci
npm test
npm run dev
```

Open the local URL Wrangler prints. For your own deployment, log in with `npx wrangler login`, check `npx wrangler deploy --dry-run`, then `npm run deploy`.

## API

| Route | Response |
|---|---|
| `/api/health` | Version and catalog count |
| `/api/downloads` | Starter manifest, download URL, SHA-256 and validation status |
| `/downloads/shoes-starter-v1.zip` | Actual two-card synthetic media pack |
| `/downloads/starter-manifest.json` | Download checksum and metadata |
| `/api/catalog` | 32 performance scene metadata entries |
| `/api/catalog/scenes.csv` | Ordered performance device catalog; requires matching private media |
| `/api/packs` | Available preview arrangements |
| `/api/packs/event-chaos?seed=demo-night` | Schema v2 planning JSON; `deviceImport: false` |
| `/api/packs/event-chaos/playlist.csv?seed=demo-night` | Position/scene ID planning CSV |
| `/api/packs/:id/scenes.csv` | HTTP 410, retired unsafe export with migration links |

API routes accept GET only. Seeds are limited to 128 characters. Public API responses allow cross-origin reads; there are no public writes, uploads, credentials, or model calls. Preview palettes are illustrative metadata, not the actual firmware's light configuration.

## Maintain it

`npm ci` uses the committed lockfile. Static downloads and their manifest ship with the Worker through the `assets` configuration in `wrangler.jsonc`. There is no R2 dependency. See [the deployment guide](../../docs/CLOUDFLARE.md) for live verification and rollback, and [media formats](../../docs/MEDIA.md) for adding a pack.
