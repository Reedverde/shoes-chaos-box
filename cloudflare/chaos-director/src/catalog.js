// Metadata only. Copyrighted frame and audio files remain on the two device cards.
const rows = [
  ["S001", 1, "Damn Daniel", "outside", 6000, true, false, ["#ecf4ff", "#ffffff"], "cool-flash"],
  ["S002", 2, "The Wizard of Oz", "outside", 6250, false, false, ["#00a651", "#d71920"], "ruby-orbit"],
  ["S003", 3, "Curb Your Enthusiasm", "outside", 6000, false, true, ["#f6d32d", "#111111"], "awkward-pulse"],
  ["S004", 4, "Austin Powers", "outside", 6000, false, false, ["#ff7a00", "#7b2cff"], "mod-cut"],
  ["S005", 5, "The Silence of the Lambs", "outside", 6000, false, false, ["#741b1b", "#d9c7a1"], "slow-threat"],
  ["S006", 6, "Forrest Gump", "outside", 6250, false, false, ["#ffffff", "#3d78b8"], "soft-wipe"],
  ["S007", 7, "The Shawshank Redemption", "outside", 6000, false, false, ["#44546a", "#f2c14e"], "hope-rise"],
  ["S008", 8, "Toy Story 3 — Woody", "outside", 6000, false, false, ["#f2c14e", "#8b4513"], "western-chase"],
  ["S009", 9, "Back to the Future Part II", "outside", 6000, false, false, ["#ff5a1f", "#00b7ff"], "time-cut"],
  ["S010", 10, "Michael Jackson — toe stand + logo", "outside", 6000, false, false, ["#ffffff", "#111111"], "spotlight"],
  ["S011", 11, "Michael Jordan + Mars Blackmon", "outside", 6000, false, false, ["#d71920", "#111111"], "court-flash"],
  ["S012", 12, "Cinderella — glass slipper", "outside", 6000, false, false, ["#8ed8ff", "#ffffff"], "sparkle"],
  ["S013", 13, "Mister Rogers — changing shoes", "outside", 6000, false, false, ["#c62828", "#f4c542"], "gentle-fade"],
  ["S014", 14, "Barbie — heel or Birkenstock", "outside", 6000, false, false, ["#ff4fb3", "#ffffff"], "pink-pop"],
  ["S015", 15, "Get Smart — shoe phone", "outside", 6000, false, false, ["#ef3e36", "#f7d038"], "spy-blink"],
  ["S016", 16, "SpongeBob — squeaky boots", "outside", 6000, false, false, ["#75d5ff", "#ffd600"], "bubble-pop"],
  ["S017", 17, "Charlie Chaplin — The Gold Rush", "outside", 6000, false, false, ["#f4e8c1", "#222222"], "silent-flicker"],
  ["S018", 18, "Kelly — Oh my God, shoes", "shoes", 6000, false, false, ["#ff00c8", "#00f5ff"], "club-cut"],
  ["S019", 19, "Kelly — Let’s get some shoes", "shoes", 6000, false, false, ["#7cff00", "#7b2cff"], "chase-fade"],
  ["S020", 20, "Kelly — These shoes rule / suck", "shoes", 6000, false, false, ["#ff3b30", "#00c7ff"], "split-blink"],
  ["S021", 21, "Kelly — These shoes SUCK", "shoes", 6000, false, false, ["#ff00c8", "#ffea00"], "hard-cut"],
  ["S022", 22, "Kelly — Too many shoes / shut up", "shoes", 6050, false, false, ["#00f5ff", "#ff5a1f"], "rotate-pop"],
  ["S023", 23, "Kelly — Let’s party", "shoes", 6000, false, false, ["#ff00c8", "#00ff6a"], "party-mix"],
  ["S024", 24, "Kelly — Three hundred dollars", "shoes", 6000, false, false, ["#ffd600", "#7b2cff"], "money-flash"],
  ["S025", 25, "Kelly — The price / let’s get ’em", "shoes", 6000, false, true, ["#ff5a1f", "#ff00c8"], "retail-rush"],
  ["S026", 26, "Kelly — This style runs small", "shoes", 6700, false, false, ["#00f5ff", "#7cff00"], "size-roll"],
  ["S027", 27, "Kelly — Those shoes are mine", "shoes", 6000, false, true, ["#ff3b30", "#7b2cff"], "claim-flash"],
  ["S028", 28, "Curb — Larry Refuses", "outside", 7000, false, false, ["#f6d32d", "#111111"], "awkward-pulse"],
  ["S029", 29, "Curb — Get the Coats", "outside", 13010, false, false, ["#f6d32d", "#111111"], "awkward-pulse"]
];

export const catalog = rows.map(([id, audioId, title, family, durationMs, mildLanguage, explicit, haloPalette, haloPattern]) => ({
  id,
  audioId,
  title,
  family,
  durationMs,
  mildLanguage,
  explicit,
  haloPalette,
  haloPattern,
  framesCsv: `SCENES/${id}/FRAMES.CSV`,
  lockoutMs: 10000,
  haloTailMs: 2000,
  provenance: "OMG-Shoes-Circle-240-v2"
}));

export const catalogById = new Map(catalog.map((scene) => [scene.id, scene]));
