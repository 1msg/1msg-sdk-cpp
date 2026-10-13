import { execFileSync } from "node:child_process";
import { relative, resolve } from "node:path";

// Native SDKs currently have no authored TypeScript. New TS must gain a strict program before it can enter the repository.
const requested = process.argv.slice(2);
const files = requested.length
  ? requested
  : execFileSync(
      "git",
      ["ls-files", "--cached", "--others", "--exclude-standard", "-z"],
      { encoding: "utf8" },
    ).split("\0");
/** Normalize lint-staged absolute paths before excluding the sealed vendor; repository-authored TS remains forbidden until a program exists. */
const unsupported = files.filter((file) => {
  const path = relative(process.cwd(), resolve(file)).replaceAll("\\", "/");
  return (
    /\.(?:ts|tsx|mts|cts)$/.test(path) && !path.startsWith("tools/quality/")
  );
});
for (const file of unsupported)
  console.error(
    `${file}: TypeScript requires a configured strict program; preserve the native SDK profile and explicitly configure authored TS tooling first.`,
  );
if (unsupported.length) process.exitCode = 1;
