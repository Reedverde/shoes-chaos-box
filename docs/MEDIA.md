# Media, formats, and reuse

The performance uses excerpts and references from third-party movies, television, music, memes, and Kling-generated scenes. This repository's visibility does not grant rights to those works. The downloadable starter ZIP contains only newly generated geometric test images and tones; it does not distribute the performance media. Some **existing firmware headers embed fallback artwork**. They are separate from the starter and also need a rights review before redistribution of firmware binaries.

No repository-wide code license has been selected. Do not describe the repository as licensed open source or add a license on the owner's behalf. Builders should supply media they are permitted to use and obtain any necessary permissions before publicly distributing derivative builds. This document records boundaries, not a legal clearance.

## Card B visual format

`SCENES.CSV` begins with:

```csv
id,audio_id,duration_ms,frames_csv,lockout_ms,halo_tail_ms
S001,1,6000,SCENES/S001/FRAMES.CSV,10000,2000
```

Keep all 32 entries in ascending S001–S032 order with unique IDs. Firmware and the five-bit halo protocol use that ordered catalog. The playlist is a different object and is not imported. S030/S031/S032 reuse audio 3/20/23.

Each scene has `FRAMES.CSV`:

```csv
bmp,raw,duration_ms
F001.BMP,F001.RAW,6000
```

Use up to 48 frames per scene. Each RAW is exactly 240×240×2 = **115,200 bytes**, RGB565 in display byte order (high byte first), row-major. The matching BMP fallback is 240×240, uncompressed 24-bit. Keep short 8.3 filenames. Frame holds must add up to the scene duration. Firmware buffers two 57,600-byte halves and transfers the raw bytes directly to the screen.

A declared short hold does not make storage faster. The tested board reads a full image in roughly 95 ms at 16 MHz plus about 35 ms display time; profile your own wiring, card, and module. Do not use 20 MHz on the assumption that faster is better: that failed on this bench.

## Card A audio format

Use `mp3/0001.mp3` through `0029.mp3`. The active DFPlayer command addresses files in the MP3 folder by number. Check all actual media pairings, start latency, speaker output, and volume. Serial command success does not prove audible sound. The starter uses quiet mono MP3 tones encoded at 22,050 Hz / 64 kbps for six seconds.

## Rebuild the public fixtures

```sh
python3 tools/build_starter_pack.py --ffmpeg /path/to/ffmpeg
python3 tools/validate_starter_pack.py
```

The ZIP includes checksums for every payload file. The external manifest checksums the ZIP itself. MP3 bytes may differ across FFmpeg versions, so commit a regenerated ZIP and its manifest together. The new starter's file validation is not physical device validation. It intentionally omits Arc Core and all third-party performance clips.
