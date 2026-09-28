import assert from "node:assert/strict";
import test from "node:test";
import { readFileSync } from "node:fs";
import { catalog } from "../src/catalog.js";
import { buildPack, catalogToCsv, playlistToCsv } from "../src/packs.js";
import { route } from "../src/index.js";
const get = path => route(new Request(`https://example.test${path}`));

test("event deck keeps alternation, repeat counts and every outside scene across seeds", () => {
  for (const seed of ["alcatraz","demo-night","0","😀","different"]) {
    const pack=buildPack("event-chaos",seed);
    assert.equal(pack.sceneCount,48);
    pack.scenes.forEach((s,i)=>assert.equal(s.family,i%2===0?"outside":"shoes"));
    for (const scene of catalog.filter(s=>s.family==="outside")) {
      assert.equal(pack.scenes.filter(s=>s.id===scene.id).length,["S003","S028","S029","S030"].includes(scene.id)?2:1);
    }
    assert.deepEqual(pack.scenes,buildPack("event-chaos",seed).scenes);
  }
});

test("canonical export preserves the firmware's 32 positional halo IDs", () => {
  const rows=catalogToCsv().trim().split("\n");
  assert.equal(rows.length,33);
  assert.equal(rows[0],"id,audio_id,duration_ms,frames_csv,lockout_ms,halo_tail_ms");
  rows.slice(1).forEach((row,index)=>{
    const [id,audio,duration,path,lockout,tail]=row.split(",");
    assert.equal(id,`S${String(index+1).padStart(3,"0")}`);
    assert.equal(Number(audio),index<29?index+1:[3,20,23][index-29]);
    assert.ok(Number(duration)>0);assert.equal(path,`SCENES/${id}/FRAMES.CSV`);
    assert.equal(Number(lockout),10000);assert.equal(Number(tail),2000);
    assert.ok(row.length<128 && path.length<=31);
  });
});

test("playlist preview is explicitly not a device catalog", async () => {
  const pack=buildPack("event-chaos","test");
  assert.equal(pack.deviceImport,false);assert.equal(pack.kind,"playlist-preview");
  assert.equal(pack.schemaVersion,2);
  const csv=playlistToCsv(pack);
  assert.ok(csv.startsWith("position,scene_id\n"));assert.equal(csv.trim().split("\n").length,49);
  const retired=get("/api/packs/event-chaos/scenes.csv");
  assert.equal(retired.status,410);assert.equal((await retired.json()).catalog,"/api/catalog/scenes.csv");
  const download=get("/api/packs/event-chaos/playlist.csv?seed=test");
  assert.equal(download.status,200);assert.equal(await download.text(),csv);
  assert.match(download.headers.get("content-disposition"),/playlist.csv/);
});

test("API limits and errors are explicit", async () => {
  assert.equal(get("/api/packs/missing").status,404);
  assert.equal(get("/unknown").status,404);
  assert.equal(get("/api/packs/event-chaos?seed="+"a".repeat(129)).status,400);
  const post=route(new Request("https://example.test/api/packs",{method:"POST"}));
  assert.equal(post.status,405);assert.equal(post.headers.get("allow"),"GET");
  assert.equal((await get("/api/health").json()).sceneCount,32);
  assert.equal(await get("/api/catalog/scenes.csv").text(),catalogToCsv());
});

test("homepage escapes user seed and separates starter from performance clips", async () => {
  const html=await get('/?seed=%22%3E%3Cscript%3Eevil%3C%2Fscript%3E').text();
  assert.ok(!html.includes('<script>evil</script>'));
  assert.match(html,/&lt;script&gt;/);
  assert.match(html,/shoes-starter-v1.zip/);
  assert.match(html,/does not import these playlists/);
});

test("download manifest describes the committed ZIP", async () => {
  const {createHash}=await import("node:crypto");
  const {packs}=await get("/api/downloads").json();
  const data=readFileSync(new URL('../public'+packs[0].download,import.meta.url));
  assert.equal(data.length,packs[0].bytes);
  assert.equal(createHash('sha256').update(data).digest('hex'),packs[0].sha256);
});
