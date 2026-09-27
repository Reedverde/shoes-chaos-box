import assert from "node:assert/strict";
import test from "node:test";
import { catalog } from "../src/catalog.js";
import { buildPack, packToCsv } from "../src/packs.js";
import { route } from "../src/index.js";

test("event pack alternates outside and Shoes scenes", () => {
  const pack = buildPack("event-chaos", "alcatraz");
  assert.equal(pack.sceneCount, 48);
  pack.scenes.forEach((scene, index) => {
    assert.equal(scene.family, index % 2 === 0 ? "outside" : "shoes");
  });
});

test("each Curb scene occurs exactly twice in the event pack", () => {
  const ids = buildPack("event-chaos", "alcatraz").scenes.map((scene) => scene.id);
  for (const id of ["S003", "S028", "S029", "S030"]) {
    assert.equal(ids.filter((candidate) => candidate === id).length, 2);
  }
});

test("seeded packs are deterministic except for their timestamp", () => {
  const first = buildPack("event-chaos", "same-seed").scenes.map((scene) => scene.id);
  const second = buildPack("event-chaos", "same-seed").scenes.map((scene) => scene.id);
  assert.deepEqual(first, second);
});

test("CSV is firmware-compatible and every ID is catalogued", () => {
  const pack = buildPack("clean-demo", "test");
  const csv = packToCsv(pack);
  assert.match(csv, /^id,audio_id,duration_ms,frames_csv,lockout_ms,halo_tail_ms\n/);
  const known = new Set(catalog.map((scene) => scene.id));
  assert.ok(pack.scenes.every((scene) => known.has(scene.id)));
});

test("Worker exposes generated CSV", async () => {
  const response = route(new Request("https://example.test/api/packs/event-chaos/scenes.csv?seed=boat"));
  assert.equal(response.status, 200);
  assert.match(response.headers.get("content-type"), /text\/csv/);
  assert.equal((await response.text()).trim().split("\n").length, 49);
});
