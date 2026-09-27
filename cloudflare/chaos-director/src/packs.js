import { catalog, catalogById } from "./catalog.js";

const curbIds = new Set(["S003", "S028", "S029"]);
const shoes = catalog.filter((scene) => scene.family === "shoes");
const outside = catalog.filter((scene) => scene.family === "outside");

export const packDefinitions = [
  {
    id: "event-chaos",
    name: "Event Chaos",
    description: "Full 44-play alternating run. Every outside scene is followed by Shoes; each Curb scene appears twice."
  },
  {
    id: "clean-demo",
    name: "Clean Demo",
    description: "Ten safe, recognizable plays for quick demonstrations."
  },
  {
    id: "emerald-preview",
    name: "Emerald Preview",
    description: "Wizard-led six-play diagnostic pack for the emerald/ruby halo profile."
  }
];

function hashSeed(value) {
  let hash = 2166136261;
  for (const char of String(value)) {
    hash ^= char.charCodeAt(0);
    hash = Math.imul(hash, 16777619);
  }
  return hash >>> 0;
}

function randomFrom(seed) {
  let state = hashSeed(seed);
  return () => {
    state += 0x6d2b79f5;
    let value = state;
    value = Math.imul(value ^ (value >>> 15), value | 1);
    value ^= value + Math.imul(value ^ (value >>> 7), value | 61);
    return ((value ^ (value >>> 14)) >>> 0) / 4294967296;
  };
}

function shuffle(items, random) {
  const result = [...items];
  for (let index = result.length - 1; index > 0; index -= 1) {
    const target = Math.floor(random() * (index + 1));
    [result[index], result[target]] = [result[target], result[index]];
  }
  return result;
}

function shoesDeck(count, random) {
  const result = [];
  let previous = "";
  while (result.length < count) {
    let round = shuffle(shoes, random);
    if (round.length > 1 && round[0].id === previous) {
      [round[0], round[1]] = [round[1], round[0]];
    }
    for (const scene of round) {
      if (result.length === count) break;
      result.push(scene);
      previous = scene.id;
    }
  }
  return result;
}

function interleave(left, right) {
  return left.flatMap((scene, index) => [scene, right[index]]);
}

function scenesFor(packId, random) {
  if (packId === "event-chaos") {
    const doubledCurbs = outside.filter((scene) => curbIds.has(scene.id));
    const outsideDeck = shuffle([...outside, ...doubledCurbs], random);
    return interleave(outsideDeck, shoesDeck(outsideDeck.length, random));
  }

  if (packId === "clean-demo") {
    const outsideIds = ["S002", "S004", "S008", "S014", "S016"];
    const shoesIds = ["S018", "S019", "S023", "S024", "S026"];
    return interleave(outsideIds.map((id) => catalogById.get(id)), shoesIds.map((id) => catalogById.get(id)));
  }

  if (packId === "emerald-preview") {
    const ids = ["S002", "S018", "S016", "S023", "S028", "S019"];
    return ids.map((id) => catalogById.get(id));
  }

  throw new Error(`Unknown pack: ${packId}`);
}

export function buildPack(packId, seed = "shoes-off-dirtbag") {
  const definition = packDefinitions.find((pack) => pack.id === packId);
  if (!definition) throw new Error(`Unknown pack: ${packId}`);
  const scenes = scenesFor(packId, randomFrom(seed));
  return {
    schemaVersion: 1,
    id: packId,
    name: definition.name,
    description: definition.description,
    seed,
    generatedAt: new Date().toISOString(),
    runtime: {
      offline: true,
      visualCard: "Card B",
      audioCard: "Card A",
      mapping: "S001=0001.mp3 through S029=0029.mp3"
    },
    sceneCount: scenes.length,
    scenes
  };
}

export function packToCsv(pack) {
  const header = "id,audio_id,duration_ms,frames_csv,lockout_ms,halo_tail_ms";
  const rows = pack.scenes.map((scene) => [
    scene.id,
    scene.audioId,
    scene.durationMs,
    scene.framesCsv,
    scene.lockoutMs,
    scene.haloTailMs
  ].join(","));
  return `${[header, ...rows].join("\n")}\n`;
}
