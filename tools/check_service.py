#!/usr/bin/env python3
"""Smoke-test a deployed or locally running Chaos Director, including its ZIP."""
import argparse, hashlib, json, urllib.request, urllib.error
p=argparse.ArgumentParser();p.add_argument('url');base=p.parse_args().url.rstrip('/')
def get(path):
    request=urllib.request.Request(base+path,headers={'User-Agent':'Mozilla/5.0 (compatible; ShoesChaosBoxServiceCheck/1.0)'})
    with urllib.request.urlopen(request,timeout=30) as response:
        return response.read(),response.headers
health=json.loads(get('/api/health')[0]);assert health['sceneCount']==32 and health['schemaVersion']==2
manifest=json.loads(get('/api/downloads')[0])['packs'][0]
data,headers=get(manifest['download'])
assert len(data)==manifest['bytes'] and hashlib.sha256(data).hexdigest()==manifest['sha256']
assert 'attachment' in headers['Content-Disposition']
assert json.loads(get('/downloads/starter-manifest.json')[0])==manifest
rows=get('/api/catalog/scenes.csv')[0].decode().strip().splitlines()
assert len(rows)==33 and [r.split(',')[0] for r in rows[1:]]==[f'S{i:03}' for i in range(1,33)]
pack=json.loads(get('/api/packs/event-chaos?seed=smoke')[0]);assert pack['sceneCount']==48 and pack['deviceImport'] is False
assert len(get('/api/packs/event-chaos/playlist.csv?seed=smoke')[0].decode().strip().splitlines())==49
try:get('/api/packs/event-chaos/scenes.csv');raise AssertionError('old export still available')
except urllib.error.HTTPError as error:assert error.code==410
assert b'Download starter ZIP' in get('/')[0]
print('PASS:',base,'health, 32-entry catalog, 48-play preview, retired export, starter ZIP and served checksum')
