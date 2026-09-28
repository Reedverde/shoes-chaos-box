#!/usr/bin/env python3
"""Build synthetic two-card fixtures; never reads the private performance media."""
import argparse, csv, hashlib, io, json, math, pathlib, struct, subprocess, tempfile, wave, zipfile
ROOT = pathlib.Path(__file__).resolve().parents[1]
OUT = ROOT / 'cloudflare/chaos-director/public/downloads'
HEADER = 'id,audio_id,duration_ms,frames_csv,lockout_ms,halo_tail_ms\n'

def image(scene):
    raw, pixels = bytearray(), bytearray()
    for y in range(240):
        for x in range(240):
            dx, dy = x - 119.5, y - 119.5
            ring = 56 < math.hypot(dx, dy) < 105
            bar = abs(dy) < 10 and -90 < dx < -90 + scene * 180 / 32
            r, g, b = ((40 + scene * 47 % 180, 60 + scene * 31 % 170, 90 + scene * 17 % 150) if ring else ((230,230,230) if bar else (8,12,20)))
            raw.extend(struct.pack('>H', (r >> 3) << 11 | (g >> 2) << 5 | (b >> 3)))
            pixels.extend((b,g,r))
    # 24-bit BMP, bottom-up, width already divisible by four.
    bmp = b'BM' + struct.pack('<IHHI', 54 + len(pixels), 0, 0, 54)
    bmp += struct.pack('<IiiHHIIiiII',40,240,240,1,24,0,len(pixels),2835,2835,0,0)
    bmp += b''.join(pixels[y*720:(y+1)*720] for y in range(239,-1,-1))
    return bytes(raw), bmp

def build(ffmpeg):
    files = {}
    rows = []
    for i in range(1,33):
        scene = f'S{i:03}'
        audio = i if i <= 29 else {30:3,31:20,32:23}[i]
        rows.append(f'{scene},{audio},6000,SCENES/{scene}/FRAMES.CSV,10000,2000')
        raw,bmp = image(i)
        folder = f'Card-B-visual/SCENES/{scene}/'
        files[folder+'F001.RAW'],files[folder+'F001.BMP'] = raw,bmp
        files[folder+'FRAMES.CSV'] = b'bmp,raw,duration_ms\nF001.BMP,F001.RAW,6000\n'
    files['Card-B-visual/SCENES.CSV'] = (HEADER+'\n'.join(rows)+'\n').encode()
    with tempfile.TemporaryDirectory() as td:
        for track in range(1,30):
            wav,mp3 = pathlib.Path(td)/'tone.wav',pathlib.Path(td)/'tone.mp3'
            with wave.open(str(wav),'wb') as w:
                w.setparams((1,2,22050,0,'NONE','not compressed'))
                samples = bytearray()
                for sample in range(22050*6):
                    t = sample/22050
                    # Quiet short tone once per second with fades; otherwise silence.
                    phase=t%1
                    envelope=max(0,min(1,phase/.02,(.18-phase)/.02))
                    samples.extend(struct.pack('<h',int(2500*envelope*math.sin(2*math.pi*(220+track*12)*t))))
                w.writeframes(samples)
            subprocess.run([ffmpeg,'-hide_banner','-loglevel','error','-y','-i',str(wav),'-map_metadata','-1','-codec:a','libmp3lame','-b:a','64k',str(mp3)],check=True)
            files[f'Card-A-audio/mp3/{track:04}.mp3'] = mp3.read_bytes()
    files['START-HERE.txt'] = b'''Shoes Off, Dirtbag! synthetic starter pack v1
32 original geometric test screens + 29 quiet six-second test tones.
No movie, music, meme, or Kling media is included. These are synthetic test
fixtures, not the performance show or proof of hardware playback validation.

BACK UP your existing cards. Use two spare FAT32 microSD cards.
Copy the CONTENTS of Card-A-audio to the root of the DFPlayer card.
Copy the CONTENTS of Card-B-visual to the root of the visual-reader card.
Keep SCENES.CSV and the SCENES directory at the root, not in a nested folder.
Power off before changing cards. Begin at low volume.
Use the project's ESP-IDF and Circuit Playground firmware and wiring guide.
All 32 scene slots remain in order for the existing halo protocol.
A scene holds one test screen for six seconds while its tone track plays.
The bar length increases with the scene slot. S030/31/32 reuse tracks 3/20/23.
Firmware still randomizes its own play order; playlist.csv is not imported.
Arc Core media is intentionally absent, so its missing-media path returns home.
Firmware's embedded home/QR/fallback images are separate from this download.

Build, wiring, replacement media format and limitations:
https://github.com/Reedverde/shoes-chaos-box/blob/main/docs/GETTING_STARTED.md
'''
    hashes = {name:hashlib.sha256(data).hexdigest() for name,data in sorted(files.items())}
    files['SHA256SUMS'] = ''.join(f'{digest}  {name}\n' for name,digest in hashes.items()).encode()
    OUT.mkdir(parents=True,exist_ok=True)
    dest=OUT/'shoes-starter-v1.zip'
    with zipfile.ZipFile(dest,'w',zipfile.ZIP_DEFLATED,compresslevel=9) as z:
        for name,data in sorted(files.items()):
            info=zipfile.ZipInfo(name,date_time=(2026,9,27,0,0,0))
            info.compress_type=zipfile.ZIP_DEFLATED
            info.external_attr=0o644<<16
            z.writestr(info,data)
    manifest={'id':'synthetic-starter-v1','version':1,'title':'Start with shapes and sound','kind':'device-media','filename':dest.name,'download':'/downloads/'+dest.name,'bytes':dest.stat().st_size,'sha256':hashlib.sha256(dest.read_bytes()).hexdigest(),'sceneCount':32,'audioTracks':29,'media':'Original geometric test screens and synthesized tones. No performance clips.','hardwareValidation':'File structure, dimensions, mappings and hashes checked; this new starter pack has not been played on the physical device.'}
    (OUT/'starter-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print(json.dumps(manifest,indent=2))
if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--ffmpeg',default='ffmpeg');build(p.parse_args().ffmpeg)
