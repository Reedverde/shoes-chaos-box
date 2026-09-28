# Proof-of-concept audit and handoff

Reviewed September 27, 2026. Scope: repository history and current changes, active ESP32 and halo code, control tests, hardware timing evidence, media/catalog contracts, public documentation, and Cloudflare service. This is a project readiness review, not a certification of a finished wearable or a legal clearance for media.

## Findings and disposition

| Finding | Evidence / consequence | Disposition |
|---|---|---|
| Dense media exceeded SD read throughput | S021 averaged 372.414 ms read + 35.008 ms draw against 100–300 ms holds | Repaired with direct reads at tested 16 MHz; average read 95.111 ms. 20 MHz failed and was rejected. |
| LED renderer sent partial patterns | Convenience pixel calls transmitted during redraw; shared counter confused rotation with scene phrases | Repaired with one completed update per frame, separate phrase timing, elapsed-time rotation and interpolation. Actual-source host tests cover 32 profiles. |
| Cloud export confused playlist and catalog | 48 playlist rows could be truncated by 32-entry firmware; row position controls halo ID | Replaced by canonical ordered catalog plus clearly non-importable playlist export. Old unsafe route returns 410. Regression tests check positional IDs and mappings. |
| Public Cloudflare role was unclear | Documentation implied a Flue agent and packaged performance media, neither present | Real Worker + Static Assets pack library, downloadable synthetic two-card pack, API and setup guide. Flue documented as optional future work. |
| Project entry point was stale | 29-scene language, unflashed claims, QR-only controls, emergency-stop claim, external local paths | New README and getting-started/demo/media guides; current 32-scene state and explicit pending tests. Local-only recovery archives labeled honestly. |
| New builders lacked distributable media | The performance source files are private/local and not a complete public kit | New synthetic starter: 32 original patterns and 29 tone tracks with matching indexes, per-file checksums, ZIP manifest and generator. Structure and decoding checked; physical playback pending. |
| Test results were not portable | Original measurements lived outside the Git repository | Public per-scene CSV/JSON and repair report; CI runs cloud, media-contract, control and halo tests. Raw device logs and recovery binaries remain local. |
| More controls/specials were added after timing repair | Arc Core expanded to 48 frames / 35.04 s, halo resync and boot recovery, persistent scene deck | Preserved latest source and actual-source regression checks; separate validation claims for installed firmware versus physical acceptance. |

## Evidence that is complete

The September 27 hardware test verified all 1,071 then-installed RAW files (1,055 normal frames plus 16 original Arc Core frames). Every normal frame appeared during all 32 scene runs. Thirty-one scenes ended 10 ms over target; S022 ended 58 ms over. Reed observed the scene endings and confirmed they were correct. [Detailed measurements](../firmware/TIMING_REPAIR_2026-09-27.md) and [machine-readable results](validation/scene-results.json) record the limits.

The later Arc Core replacement has 48 frames and a 35.04-second timeline. Its 97 special files plus special index were separately checksum-checked. Project records report both firmwares installed and a live Arc active/stop trace. Those later changes are not part of the earlier 1,071-file count.

The latest control-loop tests cover QR → Arc → home, interruption, missing special media, Mode 4, and preservation of normal scene progress. Halo tests cover all normal profiles, time wrap, continuous rotation, Arc lighting stages and recovery. These are simulated hardware boundaries around the actual application logic, not optical/audio measurements.

## Remaining limitations

| Area | Limit / next check |
|---|---|
| Timing | Up to 64.362 ms frame-start jitter; DFPlayer start latency is not measured from BUSY or acoustic feedback. A slower/replacement card can still cause backlog. |
| Media parser | Firmware still assumes ordered, complete catalog IDs and a maximum of 48 frames; it does not fully reject every malformed or over-budget pack. The cloud export and starter validator enforce the supported contract. |
| Controls | Full physical two-cycle test of the latest right-pedal cycle remains pending. Local buttons are not in-scene emergency stops. One-entry event queue and blocking reads impose response limits. |
| Wearable | Mechanical mounting, cable strain relief, movement/load tests and five-hour battery life are unverified. This remains a bench proof of concept. |
| Lights and speaker | Final diffuser appearance and intelligibility in the final mounting position need approval. |
| Starter download | All payload paths, checksums, RGB565/BMP pixel correspondence, mappings, frame totals and MP3 decoding checked; no physical starter-card test yet. |
| Rights | Private performance clips are not in the public download. Existing embedded firmware artwork needs separate review. No repository-wide license is selected. |
| Flue | Not implemented. Cloudflare Workers + Static Assets is the current integration. The event lists platform use as optional bonus consideration. |

## Keep the working baseline

This packaging work does not reflash the connected hardware or rewrite either physical card. It preserves the already tested timing repair and the latest Arc/control work in Git. Download packs to spare cards for evaluation. Keep recovery firmware and original media in the owner's local archive. See [TODO](../TODO.md) for the physical checks before calling the wearable complete.

## Publication checks

Both active firmware builds compiled successfully with the pinned platform versions. The halo build reports warnings in upstream Adafruit utility code; it completes successfully. The six Cloudflare tests, actual-source control/halo tests, synthetic pack validator, all 29 MP3 decode checks, local served-download checksum check, and Wrangler deployment dry run passed. Newcomer/documentation relative links resolve. The pack library was exercised in a desktop browser and inspected at a 390-pixel phone width.

GitHub CI repeats the portable checks on pushes and pull requests. It does not flash devices or claim a hardware endurance test. Current deployment links and publication status live in the [service README](../cloudflare/chaos-director/README.md).
