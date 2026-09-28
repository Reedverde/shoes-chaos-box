import { catalog } from "./catalog.js";
import { buildPack, packDefinitions, catalogToCsv, playlistToCsv } from "./packs.js";
import starter from "../public/downloads/starter-manifest.json" with { type: "json" };

const repo = "https://github.com/Reedverde/shoes-chaos-box";
const headers = { "cache-control": "no-store", "access-control-allow-origin": "*", "x-content-type-options": "nosniff" };
const json = (value, status = 200) => new Response(JSON.stringify(value, null, 2), {
  status, headers: { ...headers, "content-type": "application/json; charset=utf-8" }
});
const csv = (value, name) => new Response(value, {headers: {...headers, "content-type":"text/csv; charset=utf-8", "content-disposition":`attachment; filename="${name}"`}});
const escape = (value) => String(value).replace(/[&<>"']/g, ch => ({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"}[ch]));

function home(url) {
  const selected = packDefinitions.find(p => p.id === url.searchParams.get("pack")) || packDefinitions[0];
  const seed = (url.searchParams.get("seed") || "demo-night").slice(0,128);
  const pack = buildPack(selected.id, seed);
  const query = `?seed=${encodeURIComponent(seed)}`;
  return new Response(`<!doctype html>
<html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="description" content="Build your own Shoes Off, Dirtbag prototype. Download original test media, explore scene playlists, and follow Reed's hardware build.">
<title>Shoes Off, Dirtbag! | Content packs</title><link rel="icon" href="data:,">
<style>
:root{color-scheme:dark;--bg:#121315;--card:#1d1f22;--ink:#f5f5f0;--muted:#babebf;--lime:#daff7b;font-family:system-ui,sans-serif;background:var(--bg);color:var(--ink)}
*{box-sizing:border-box}body{margin:0}main,nav,footer{max-width:1120px;margin:auto;padding:24px}nav{display:flex;justify-content:space-between;gap:20px;border-bottom:1px solid #414549}nav a{font-size:.9rem}a{color:var(--lime);text-underline-offset:4px}a:focus-visible,button:focus-visible,input:focus-visible,select:focus-visible{outline:3px solid #fff;outline-offset:5px}.hero{padding:64px 0 44px;display:grid;grid-template-columns:1.4fr 1fr;gap:48px;align-items:center}h1{font-size:clamp(2.8rem,7vw,5.8rem);line-height:1;letter-spacing:-.055em;margin:0 0 24px}h2{font-size:clamp(1.7rem,4vw,2.6rem);line-height:1.1;letter-spacing:-.03em}h3{font-size:1.15rem}p{line-height:1.65;color:var(--muted);max-width:72ch}.lead{font-size:1.15rem}.badge{display:grid;place-content:center;aspect-ratio:1;border-radius:50%;border:12px solid var(--lime);text-align:center;font-weight:950;font-size:clamp(1.8rem,4vw,3rem);transform:rotate(-6deg);box-shadow:inset 0 0 0 10px var(--bg),inset 0 0 0 12px #676f54}.badge small{font-size:1rem;margin-top:20px;font-weight:500;color:var(--muted)}.actions{display:flex;gap:12px;flex-wrap:wrap;margin:24px 0}.button,button{display:inline-block;background:var(--lime);color:#14180b;border:0;border-radius:10px;padding:13px 18px;text-decoration:none;font:inherit;font-weight:750;cursor:pointer}.secondary{background:transparent;border:1px solid #69715b;color:var(--lime)}section{padding:36px 0;border-top:1px solid #414549}.grid{display:grid;grid-template-columns:repeat(3,1fr);gap:18px}.card{background:var(--card);border:1px solid #414549;border-radius:16px;padding:24px}.card h3{margin-top:0}.number{font-size:2.2rem;color:var(--lime);font-weight:750}.note{font-size:.9rem}.form{display:flex;gap:16px;flex-wrap:wrap;align-items:end}label{display:grid;gap:8px;flex:1;min-width:200px}select,input{width:100%;background:#121315;border:1px solid #777d7c;color:white;border-radius:8px;padding:12px;font:inherit}.playlist{padding-left:28px;columns:2;column-gap:40px}.playlist li{break-inside:avoid;padding:6px 0;line-height:1.4}.playlist span{font-size:.8rem;color:var(--muted)}code{overflow-wrap:anywhere}footer{border-top:1px solid #414549;color:var(--muted);font-size:.9rem;padding-bottom:40px}@media(max-width:700px){.hero{grid-template-columns:1fr;padding-top:36px;gap:28px}.badge{max-width:240px;justify-self:center;width:100%}.grid{grid-template-columns:1fr}.playlist{columns:1}nav{flex-wrap:wrap}}
</style></head><body><nav><strong>Shoes Off, Dirtbag!</strong><a href="${repo}">Code &amp; build notes ↗</a></nav>
<main><div class="hero"><div><h1>A silly idea.<br>A working build.<br>Your turn.</h1><p class="lead">Reed’s shoe-removal reminder plays a short scene with a round screen, sound, and a ring of light. This Cloudflare site is where you can download a starter pack and explore how the content fits together.</p><div class="actions"><a class="button" href="#download">Get the starter pack</a><a class="button secondary" href="https://reedverde.com/shoes-off-dirtbag/">Read the story</a></div><p class="note">Working bench proof of concept. Wearable assembly and battery testing are next.</p></div><div class="badge" aria-label="Shoes off, dirtbag. A project by Reed.">SHOES OFF,<br>DIRTBAG!<small>A project by Reed Verdesoto</small></div></div>
<section><div class="grid"><div><div class="number">32 scenes</div><p>The performance catalog, including three Kling AI loops.</p></div><div><div class="number">1,055 frames</div><p>All normal scene frames displayed in the recorded bench timing test.</p></div><div><div class="number">Offline playback</div><p>Cloudflare prepares and distributes files. Two cards carry them on the device.</p></div></div></section>
<section id="download"><h2>Start with shapes and sound</h2><p>A complete two-card starter download: 32 original geometric test screens, 29 quiet tone tracks, matching indexes, and setup instructions. It helps you check a similar build without sourcing the performance clips.</p><div class="actions"><a class="button" href="${starter.download}" download>Download starter ZIP · ${Math.ceil(starter.bytes/1024)} KB</a><a class="button secondary" href="${repo}/blob/main/docs/GETTING_STARTED.md">Build and card setup</a></div><p class="note">Version 1 · File structure and checksums validated. Physical playback of this new starter pack is still pending. It does not contain the movie, music, or Kling performance media, or Arc Core. Use spare cards and keep your existing pack backed up.</p><details><summary>Verify the download</summary><p>SHA-256: <code>${starter.sha256}</code></p><a href="/downloads/starter-manifest.json">Download manifest</a></details></section>
<section id="playlists"><h2>Explore a performance playlist</h2><p>Choose an arrangement and a seed. The same seed gives the same order. These previews reference Reed’s performance catalog; they do not include its media. The current firmware makes its own randomized order and does not import these playlists.</p><form class="form" method="get" action="/#playlists"><label>Arrangement<select name="pack">${packDefinitions.map(p=>`<option value="${p.id}" ${p.id===selected.id?'selected':''}>${p.name}</option>`).join('')}</select></label><label>Seed<input name="seed" maxlength="128" value="${escape(seed)}" required></label><button type="submit">Preview order</button></form><h3>${escape(pack.name)} · ${pack.sceneCount} plays</h3><p>${escape(pack.description)}</p><div class="actions"><a href="/api/packs/${pack.id}${query}">Playlist JSON</a><a href="/api/packs/${pack.id}/playlist.csv${query}">Playlist CSV</a></div><details><summary>Show all ${pack.sceneCount} plays</summary><ol class="playlist">${pack.scenes.map(s=>`<li>${escape(s.title)} <span>${s.id} · ${(s.durationMs/1000).toFixed(2)}s</span></li>`).join('')}</ol></details><p class="note">Playlist CSV is for planning. Do not put it on the card as SCENES.CSV. <a href="/api/catalog/scenes.csv">The performance catalog index</a> retains all 32 IDs in firmware order and requires the matching private media.</p></section>
<section><h2>How the parts work together</h2><div class="grid"><article class="card"><h3>Prepare</h3><p>Make your visuals and audio, or start with these test files. Cloudflare serves versioned downloads and generates playlist previews.</p></article><article class="card"><h3>Load</h3><p>Copy visual files to Card B and audio to Card A. Follow the exact wiring and two-firmware setup in GitHub.</p></article><article class="card"><h3>Play</h3><p>The ESP32 runs the display and audio. The Circuit Playground runs the halo. A pedal or local button starts the scene. Wi-Fi stays out of the loop.</p></article></div></section>
<section><h2>See the decisions and the evidence</h2><p>A timing problem made the visuals fall behind the audio. Measurements led to faster card reads and complete LED updates. The recorded full-catalog run showed every frame, with 31 scenes ending 10 ms over target and one 58 ms over. Frame-level jitter remains; wearable testing comes next.</p><div class="actions"><a href="${repo}/blob/main/docs/AUDIT.md">Audit findings</a><a href="${repo}/blob/main/firmware/TIMING_REPAIR_2026-09-27.md">Timing evidence</a><a href="${repo}/blob/main/docs/CLOUDFLARE.md">Cloudflare and Flue explained</a></div></section>
</main><footer>Built by Reed Verdesoto. Hosted on Cloudflare Workers with Static Assets. Flue is an optional future extension, not a running dependency. <a href="/api/health">Service status</a> · <a href="${repo}/blob/main/docs/MEDIA.md">Media and reuse notes</a></footer></body></html>`,{headers:{...headers,"content-type":"text/html; charset=utf-8"}});
}

function route(request) {
  const url = new URL(request.url);
  const path = url.pathname.replace(/\/+$/, "") || "/";
  if (request.method !== "GET") return new Response(JSON.stringify({error:"Method not allowed"}),{status:405,headers:{...headers,"content-type":"application/json","allow":"GET"}});
  if (url.searchParams.get("seed")?.length > 128) return json({error:"Seed must be at most 128 characters"},400);
  if (path === "/") return home(url);
  if (path === "/api/health") return json({ ok:true,service:"shoes-chaos-director",schemaVersion:2,sceneCount:catalog.length,starterVersion:starter.version });
  if (path === "/api/catalog") return json({count:catalog.length,scenes:catalog,haloMetadata:"Preview only; firmware is authoritative"});
  if (path === "/api/catalog/scenes.csv") return csv(catalogToCsv(),"SCENES.CSV");
  if (path === "/api/downloads") return json({packs:[starter]});
  if (path === "/api/packs") return json({count:packDefinitions.length,packs:packDefinitions});
  const match = path.match(/^\/api\/packs\/([a-z0-9-]+)(\/(?:scenes|playlist)\.csv)?$/);
  if (match) {
    if (!packDefinitions.some(p=>p.id===match[1])) return json({error:"Unknown pack",available:packDefinitions.map(p=>p.id)},404);
    if (match[2] === "/scenes.csv") return json({error:"Playlist-to-device export retired: it changes catalog order and halo mapping.",catalog:"/api/catalog/scenes.csv",playlist:`/api/packs/${match[1]}/playlist.csv`},410);
    const pack = buildPack(match[1],url.searchParams.get("seed") || "shoes-off-dirtbag");
    return match[2] ? csv(playlistToCsv(pack),`${pack.id}-playlist.csv`) : json(pack);
  }
  return json({error:"Not found"},404);
}
export { route };
export default { fetch:route };
