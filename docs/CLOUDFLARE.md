# Cloudflare, content packs, and Flue

## What Cloudflare does here

The **Chaos Director** is the name of this project's content service. It is a Cloudflare Worker with Static Assets, not a separate Cloudflare product. It serves a small public website, a versioned two-card starter ZIP, a catalog API, and repeatable playlist previews. That is a useful Cloudflare integration: people can discover the project and download files before taking their own device offline.

The static download is deployed alongside the Worker. No R2 bucket, database, paid AI model, secret, or public upload endpoint is needed for this version. Future large, approved media packs could move to R2; that is not implemented today.

See [the service README](../cloudflare/chaos-director/README.md) for the current live link and [the code](../cloudflare/chaos-director/src/index.js).

## Three different outputs

| Output | Purpose | Put on a device? |
|---|---|---|
| Starter ZIP | Original synthetic screens, tones, indexes, instructions, checksums | Yes, on two spare cards after following setup; physical playback pending |
| Performance `SCENES.CSV` | 32 ordered catalog entries for Reed's matching performance media | Only with those matching files |
| Playlist JSON / CSV | A proposed sequence of scene IDs, generated from a seed | No; current firmware builds its own deck |

The old `/api/packs/:id/scenes.csv` route now returns HTTP 410 with migration links. It previously emitted up to 48 playlist rows as if they were a 32-entry device catalog. That could truncate scenes and misalign halo IDs. `/api/catalog/scenes.csv` is the ordered catalog; `/api/packs/:id/playlist.csv` is explicitly a playlist.

Cloud preview colors and effect names are creative metadata. The actual halo profiles live in the Circuit Playground firmware. Changing a JSON palette does not reprogram the lights.

## Run and deploy your copy

Prerequisites: Node.js 22+, npm, and an existing Cloudflare account.

```sh
cd cloudflare/chaos-director
npm ci
npm test
npm run dev
# When ready to publish:
npx wrangler login
npx wrangler deploy --dry-run
npm run deploy
```

Wrangler prints the deployed `workers.dev` URL. Choose your own Worker name in `wrangler.jsonc` if deploying another copy. If your login has multiple accounts, select the intended account explicitly. Keep tokens in your environment or Cloudflare/GitHub secrets, never in this repository. The lockfile pins the toolchain used by `npm ci`.

Run `python3 tools/check_service.py https://YOUR-WORKER.workers.dev` from the
repository root to verify the live API, catalog, downloadable ZIP and checksum.

After deployment, open the homepage, submit a playlist preview, download the ZIP, verify its SHA-256, and request `/api/health`. Confirm `/api/catalog/scenes.csv` contains one header plus S001–S032 in order. The public ZIP and its manifest ship atomically with the service's Static Assets deployment.

For an update, test, review, and redeploy. For recovery, use `npx wrangler deployments list` and `npx wrangler rollback` to select a previously verified Worker version. Recheck the static download and manifest after rollback. See [Cloudflare's deployment documentation](https://developers.cloudflare.com/workers/wrangler/commands/#deploy).

## Where Flue fits

[Flue](https://flueframework.com/docs/guide/cloudflare-target/) is an agent framework that can target Cloudflare. This repository does **not** run a Flue agent and does not claim a Flue integration. The event's [official description](https://www.tech-week.com/calendar/sf/events/hack-alcatraz-with-cloudflare-and-kling-ai-85ba1851-ec51-40cf-904f-edfd4f9a66a8) presents Cloudflare and Kling AI as optional bonus criteria and mentions Flue as a suggested framework, not a required dependency.

A future Flue agent could accept “make a short Wizard-led demo,” use the existing read-only API as a tool, and return a reviewed playlist. A concrete integration path is:

1. Create a separate Flue project using its current [getting-started guide](https://flueframework.com/docs/guide/getting-started/) and Cloudflare target.
2. Expose a bounded tool that GETs `/api/packs` and `/api/catalog` from this service, then accepts only a known pack ID and a seed of at most 128 characters.
3. GET `/api/packs/{id}?seed={encodedSeed}` and return its schema-version-2 `playlist-preview`. Preserve `deviceImport: false` and the content/provenance notes.
4. Require human review of generated suggestions. Do not let model output rewrite device IDs, audio mappings, or the ordered catalog.
5. Deploy that agent separately with its required model/bindings and cost controls, following [Flue's Cloudflare deployment guide](https://flueframework.com/docs/ecosystem/deploy/cloudflare/). Test it before advertising it as live.

No agent is needed to make the current downloads or deterministic generator work. The wearable never calls either service during playback.

## Add another public pack

Keep source media you have permission to distribute. Use the [media format](MEDIA.md), generate a versioned ZIP, include setup instructions and a SHA-256 manifest, and add it under the service's `public/downloads/`. Test that every catalog path exists, duration totals match, and all files decode. Update `/api/downloads` and the download page together. Preserve old versioned URLs when publishing a new pack.

The included reproducible generator is `tools/build_starter_pack.py` (Python 3 and FFmpeg). It creates only original procedural fixtures and never reads the private performance-media folders.
