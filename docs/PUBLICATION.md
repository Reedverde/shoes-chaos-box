# Publication handoff

September 27, 2026 (America/New_York); September 28 UTC.

| Item | Status / link |
|---|---|
| GitHub source | [shoes-chaos-box](https://github.com/Reedverde/shoes-chaos-box), packaged source commit `008f8f8` |
| Proof-of-concept release | [poc-2026-09-27](https://github.com/Reedverde/shoes-chaos-box/releases/tag/poc-2026-09-27), prerelease with starter ZIP and checksum manifest |
| Automated checks | [Successful run for the package](https://github.com/Reedverde/shoes-chaos-box/actions/runs/36374736647) |
| Project story | [reedverde.com/shoes-off-dirtbag/](https://reedverde.com/shoes-off-dirtbag/), published and verified with the new 32-scene copy and starter/audit links |
| SF feature | [reedverde.com/sf/](https://reedverde.com/sf/), published and verified with the new bench status |
| Cloudflare content site | Source and assets ready; local browser/API/download checks and deployment dry run passed. **Production deployment blocked on account sign-in.** |

The story website is hosted by Lovable, separate from the Cloudflare Worker. Its publication ID is `df62796f-dfaa-4ba3-890e-3fdf2d747211`, from website source `4b17d881d6be8c2e6573d39e36dbe5ca40edafd2`. Both public routes returned HTTP 200 with unique new text; the story retains noindex,follow and its canonical URL.

## Finish Cloudflare publication

Sign into the intended Cloudflare account using Wrangler, then follow [the deployment guide](CLOUDFLARE.md). Publish the existing `shoes-chaos-director` project with its Static Assets directory. Run `tools/check_service.py` against the URL Wrangler returns. Update the service README and project story with that verified live link. Do not claim Flue is installed; this deployment is a Worker plus static downloads.

The original performance cards and connected hardware were not changed during this packaging task. The public synthetic starter has not been played on physical hardware. [Wearable and acceptance work](../TODO.md) remains separate from software publication.
