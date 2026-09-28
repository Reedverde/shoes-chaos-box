#!/usr/bin/env python3
"""Check the public ZIP against the device's catalog, frame, and audio contract."""
import csv, hashlib, io, json, pathlib, struct, zipfile
ROOT = pathlib.Path(__file__).resolve().parents[1]
p = ROOT/'cloudflare/chaos-director/public/downloads'
m = json.loads((p/'starter-manifest.json').read_text())
data = (p/m['filename']).read_bytes()
assert len(data)==m['bytes'] and hashlib.sha256(data).hexdigest()==m['sha256']
with zipfile.ZipFile(io.BytesIO(data)) as z:
    names=z.namelist()
    assert len(names)==len(set(names))
    assert all(not n.startswith('/') and '..' not in pathlib.PurePosixPath(n).parts for n in names)
    rows=list(csv.DictReader(io.StringIO(z.read('Card-B-visual/SCENES.CSV').decode())))
    assert [r['id'] for r in rows]==[f'S{i:03}' for i in range(1,33)]
    for i,r in enumerate(rows,1):
        aid=i if i<=29 else {30:3,31:20,32:23}[i]
        assert int(r['audio_id'])==aid
        assert int(r['lockout_ms'])==10000 and int(r['halo_tail_ms'])==2000
        assert len(r['frames_csv'])<=31
        frames=list(csv.DictReader(io.StringIO(z.read('Card-B-visual/'+r['frames_csv']).decode())))
        assert 0<len(frames)<=48
        assert sum(int(f['duration_ms']) for f in frames)==int(r['duration_ms'])==6000
        for f in frames:
            assert int(f['duration_ms'])>0
            prefix='Card-B-visual/SCENES/'+r['id']+'/'
            raw=z.read(prefix+f['raw']);bmp=z.read(prefix+f['bmp'])
            assert len(raw)==115200
            assert bmp[:2]==b'BM' and struct.unpack_from('<ii',bmp,18)==(240,240)
            assert struct.unpack_from('<H',bmp,28)[0]==24
            assert struct.unpack_from('<I',bmp,30)[0]==0
            # Verify RAW byte order and every pixel against its BMP counterpart.
            for y in range(240):
                for x in range(240):
                    b,g,red=bmp[54+((239-y)*240+x)*3:57+((239-y)*240+x)*3]
                    value=((red>>3)<<11)|((g>>2)<<5)|(b>>3)
                    assert struct.unpack_from('>H',raw,(y*240+x)*2)[0]==value
        assert len(z.read(f'Card-A-audio/mp3/{aid:04}.mp3'))>1000
    audio=[n for n in names if n.endswith('.mp3')]
    assert len(audio)==29
    hashes=z.read('SHA256SUMS').decode().splitlines()
    assert len(hashes)==len(names)-1
    for row in hashes:
        digest,name=row.split('  ',1)
        assert hashlib.sha256(z.read(name)).hexdigest()==digest,name
print('PASS: ZIP hash, 32 ordered scenes, 29 audio files, all RAW/BMP pixels, timelines and payload hashes')
