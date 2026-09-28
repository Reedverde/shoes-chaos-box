# Publication handoff

Updated September 28, 2026 (America/New_York).

| Item | Status / link |
|---|---|
| GitHub source | [shoes-chaos-box](https://github.com/Reedverde/shoes-chaos-box), packaged source commit `008f8f8` |
| Proof-of-concept release | [poc-2026-09-27](https://github.com/Reedverde/shoes-chaos-box/releases/tag/poc-2026-09-27), prerelease with starter ZIP and checksum manifest |
| Automated checks | [Successful run for the package](https://github.com/Reedverde/shoes-chaos-box/actions/runs/36374736647) |
| Project story | [reedverde.com/shoes-off-dirtbag/](https://reedverde.com/shoes-off-dirtbag/), published and verified with the new 32-scene copy and starter/audit links |
| SF feature | [reedverde.com/sf/](https://reedverde.com/sf/), published and verified with the new bench status |
| Cloudflare content site | [Live content library](https://shoes-chaos-director.reed-5c2.workers.dev/) — production API, catalog, playlist and download checks passed |
| Cloudflare starter download | [shoes-starter-v1.zip](https://shoes-chaos-director.reed-5c2.workers.dev/downloads/shoes-starter-v1.zip), verified against the [served manifest](https://shoes-chaos-director.reed-5c2.workers.dev/downloads/starter-manifest.json) |

The story website is hosted by Lovable, separate from the Cloudflare Worker. Its publication ID is `df62796f-dfaa-4ba3-890e-3fdf2d747211`, from website source `4b17d881d6be8c2e6573d39e36dbe5ca40edafd2`. Both public routes returned HTTP 200 with unique new text; the story retains noindex,follow and its canonical URL.

## Cloudflare deployment record

The official Cloudflare MCP connection published `shoes-chaos-director` to Reed's account, with Static Assets and observability enabled. Deployment ID: `899f87ca5e7f4ec995c78d463e52c413`; published September 28, 2026 at 16:20 UTC. The existing unrelated Worker was not changed. No R2 bucket, paid model, database, or custom-domain route was added.

`python3 tools/check_service.py https://shoes-chaos-director.reed-5c2.workers.dev` passed against production: health, 32-entry catalog, 48-play preview, retired export returning 410, ZIP attachment headers, and matching manifest/SHA-256. The live browser homepage was also inspected. The service checker now sends an identifying User-Agent because this environment's default Python request received HTTP 403.

Future updates can use the documented [Wrangler deployment and rollback workflow](CLOUDFLARE.md). Flue is not installed; this deployment is a Worker plus static downloads.

The original performance cards and connected hardware were not changed during this packaging task. The public synthetic starter has not been played on physical hardware. [Wearable and acceptance work](../TODO.md) remains separate from software publication.
