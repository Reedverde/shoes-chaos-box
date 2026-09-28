# A short demo

“Shoes Off, Dirtbag started as a funny reminder to take my shoes off at home. I built the screen, sound, lights, and controls as a small system, then measured and fixed it when the visuals started falling behind. This is the working bench version; the wearable mounting is next.”

Show one scene with the left pedal or local forward button. Explain that the screen and sound live on separate cards and the halo has its own controller. Show the QR with the right pedal, then Arc Core, then home. Open the Cloudflare pack library on a phone if useful: it distributes files before the performance, so the device needs no Wi-Fi on the boat.

## Controls

| Input | Result |
|---|---|
| Mode 5 left pedal (Space) | Play next scene, subject to pedal cooldown |
| Mode 5 right pedal (Enter) | QR → silent Arc Core → home; during a normal scene, interrupt to QR |
| Blue local forward button | Advance current scene order; bypass remaining pedal cooldown after playback |
| Green local previous button | Walk back through the same scene history |
| Mode 4 either pedal | Direct Arc Core start/stop |
| Mode 1 Page Up / Page Down | Saved DFPlayer volume, 0–30 |

Normal scene order and progress persist across restart. The QR and Arc Core overlays do not consume a normal scene. Arc Core is a separate 35.04-second loop: 5.04-second startup, then 30 seconds of blue motion and breathing light. A missing special-media folder returns home.

## Before showing it

Use the installed performance cards for the show, not the new synthetic starter. Cold-boot and check sound, a normal scene, two complete right-pedal cycles, and saved volume. Approve brightness through the final diffuser and test the actual battery and clothing mount. These physical acceptance checks remain pending in [TODO](../TODO.md); host tests cannot replace them.

There is no dedicated local emergency-stop/mute control. Set volume conservatively and keep the power connection accessible. The device is manually triggered; doorway direction detection is a future build, not part of this demo.
