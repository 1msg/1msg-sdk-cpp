import assert from "node:assert/strict";
import { spawnSync } from "node:child_process";
import { readFileSync, rmSync, writeFileSync } from "node:fs";
import { dirname, join, resolve } from "node:path";
import { fileURLToPath } from "node:url";
const root = resolve(dirname(fileURLToPath(import.meta.url)), "../..");
const inventory = JSON.parse(
  readFileSync(join(root, "quality/generator/inventory.json"), "utf8"),
);
// Use a generated directory member so the extra-file fixture exercises the same recursive output scope.
const path = Object.keys(inventory).find((file) => file.startsWith("include/"));
assert.ok(path, "generation inventory cannot be empty");
const original = readFileSync(join(root, path));
const ignored = join(
  dirname(path),
  `.quality-untracked${path.slice(path.lastIndexOf("."))}`,
);
const ignorePath = join(root, ".gitignore");
const originalIgnore = readFileSync(ignorePath);
/** Require the real generation replay to fail specifically on output drift, not a missing tool or configuration. */
function rejectDrift() {
  const result = spawnSync(
    process.execPath,
    ["quality/generator/generate.mjs"],
    { cwd: root, encoding: "utf8", timeout: 120000 },
  );
  assert.notEqual(result.status, 0, "generation accepted modified output");
  assert.match(
    result.stderr + result.stdout,
    /Generated drift/,
    "fixture failed for an unrelated reason",
  );
}
try {
  writeFileSync(
    join(root, path),
    Buffer.concat([
      original,
      Buffer.from("\n// negative output drift fixture\n"),
    ]),
  );
  rejectDrift();
  rmSync(join(root, path));
  rejectDrift();
  writeFileSync(join(root, path), original);
  writeFileSync(
    ignorePath,
    Buffer.concat([originalIgnore, Buffer.from(`\n/${ignored}\n`)]),
  );
  writeFileSync(join(root, ignored), "unexpected ignored generated output");
  rejectDrift();
  console.log(
    "Real regeneration rejects changed, deleted and ignored/untracked output",
  );
} finally {
  writeFileSync(join(root, path), original);
  writeFileSync(ignorePath, originalIgnore);
  rmSync(join(root, ignored), { force: true });
}
