# Kling AI Integration

Three six-second scenes generated with Kling AI are integrated into the
32-scene Shoes Off, Dirtbag! runtime. This is the project's explicit use of
Kling AI for the Hack Alcatraz hackathon requirement.

| Runtime | Kling package | Device role | Existing audio | Halo treatment |
| --- | --- | --- | --- | --- |
| S030 | K001 Curb pilot | Outside/Curb variant | 0003.mp3 | Yellow/cyan argument split with magenta retorts |
| S031 | K002 Kelly rule/suck/rule | Shoes separator | 0020.mp3 | Cyan/magenta/lime verdict pulses |
| S032 | K004 bacteria rave | Shoes separator | 0023.mp3 | Lime/cyan rotation with magenta club hits |

The source packages live outside Git under
`staging_shoes_assets/kling-meme-scenes/`. Each package contains the original
Kling video, a captioned circular preview, review frames, eight 240 x 240
RGB565 big-endian device frames, timing metadata, and a manifest. The generated
media remains local because of size and source-media licensing; this repository
tracks the runtime mappings, playback behavior, halo design, and provenance.

The reproducible card build is
`staging_shoes_assets/kling-meme-scenes/build_runtime_v5.py`. Its output,
`staging_shoes_assets/runtime-card-v5-kling`, preserves S001-S029, adds S030-S032,
and reuses existing Card A tracks instead of duplicating audio IDs. The staged
export passed frame-size, BMP-format, duration, and audio-hash checks. Physical
SD throughput, synchronization, and diffuser behavior still require hardware
verification.

K001 remains a cartoon pilot with generic characters. K002 is a stylized
caricature rather than a precise likeness. Those creative limitations are
preserved in the source manifests. Wizard of Oz generation is intentionally
deferred.
