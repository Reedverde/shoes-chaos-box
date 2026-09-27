import { catalog } from "./catalog.js";
import { buildPack, packDefinitions, packToCsv } from "./packs.js";

const json = (value, status = 200) => new Response(JSON.stringify(value, null, 2), {
  status,
  headers: {
    "content-type": "application/json; charset=utf-8",
    "cache-control": "no-store",
    "access-control-allow-origin": "*"
  }
});

function home(origin) {
  const cards = packDefinitions.map((pack) => `
    <article>
      <h2>${pack.name}</h2>
      <p>${pack.description}</p>
      <a href="/api/packs/${pack.id}">JSON</a>
      <a href="/api/packs/${pack.id}/scenes.csv">SCENES.CSV</a>
    </article>`).join("");

  return new Response(`<!doctype html>
<html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Shoes Off, Dirtbag! — Chaos Director</title>
<style>
:root{color-scheme:dark;font-family:ui-sans-serif,system-ui;background:#10051c;color:#fff}
body{max-width:900px;margin:auto;padding:48px 24px;background:radial-gradient(circle at top,#5622a8 0,#10051c 52%);min-height:100vh}
h1{font-size:clamp(2.4rem,8vw,5rem);line-height:.88;margin:0 0 20px;text-transform:uppercase;color:#ffef35;text-shadow:5px 5px #ff167d}
.lede{font-size:1.2rem;max-width:680px}section{display:grid;grid-template-columns:repeat(auto-fit,minmax(220px,1fr));gap:16px;margin-top:36px}
article{background:#211033;border:2px solid #ff167d;border-radius:18px;padding:20px;box-shadow:7px 7px 0 #2de2e6}h2{margin-top:0}a{display:inline-block;margin:8px 8px 0 0;color:#10051c;background:#ffef35;padding:8px 11px;border-radius:999px;font-weight:800;text-decoration:none}
footer{margin-top:40px;color:#cdbfe0}code{color:#2de2e6}</style></head>
<body><h1>Chaos<br>Director</h1><p class="lede">Cloudflare-generated, deterministic scene packs for the offline ESP32 wearable. Media stays on the two microSD cards; this service arranges validated scene IDs, audio mappings, timing, and halo metadata.</p>
<section>${cards}</section><footer>Catalog: <a href="/api/catalog">32 scenes</a> · API: <code>${origin}/api/packs/event-chaos?seed=demo-night</code></footer></body></html>`, {
    headers: { "content-type": "text/html; charset=utf-8" }
  });
}

function route(request) {
  const url = new URL(request.url);
  const path = url.pathname.replace(/\/+$/, "") || "/";
  if (request.method !== "GET") return json({ error: "Method not allowed" }, 405);
  if (path === "/") return home(url.origin);
  if (path === "/api/health") return json({ ok: true, service: "shoes-chaos-director", sceneCount: catalog.length });
  if (path === "/api/catalog") return json({ count: catalog.length, scenes: catalog });
  if (path === "/api/packs") return json({ count: packDefinitions.length, packs: packDefinitions });

  const match = path.match(/^\/api\/packs\/([a-z0-9-]+)(\/scenes\.csv)?$/);
  if (match) {
    try {
      const pack = buildPack(match[1], url.searchParams.get("seed") || "shoes-off-dirtbag");
      if (match[2]) {
        return new Response(packToCsv(pack), {
          headers: {
            "content-type": "text/csv; charset=utf-8",
            "content-disposition": `attachment; filename="${pack.id}-SCENES.CSV"`,
            "cache-control": "no-store",
            "access-control-allow-origin": "*"
          }
        });
      }
      return json(pack);
    } catch (error) {
      return json({ error: error.message, available: packDefinitions.map((pack) => pack.id) }, 404);
    }
  }
  return json({ error: "Not found" }, 404);
}

export { route };
export default { fetch: route };
