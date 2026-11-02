# SDK quality contract

This repository implements profile D of 1MSG-156. `cpp` is the native SDK language; JavaScript rules apply only to authored generator/check tooling. The SDK maintainers own the public generated model/API surface and offline contract fixtures.

`quality/generator/openapi.yaml` is pinned to the upstream revision and input hashes in `provenance.json`. OpenAPI Generator 7.22.0 is downloaded with a verified SHA256. Update the contract and its provenance deliberately, then run `node quality/generator/generate.mjs --write`; never hand-edit generated outputs. Every generated code path is replayed and hashed in a temporary directory. New, removed, ignored and modified generated files must match the inventory. Compatibility repairs belong in `quality/postprocess.mjs` and the corresponding upstream producer.

Use Node 24.19.0 for tooling and `npm --prefix tools/quality ci`. Run `node tools/quality/run.mjs guards`, `node tools/quality/node_modules/eslint/bin/eslint.js . --max-warnings=0`, `node --test tools/quality/test/*.test.mjs`, generation replay, and the native commands in `.github/workflows/quality.yml`. Native contract fixtures use local transports/serialization only; no live API credentials are needed. Comments describe intent, invariants, failure cases and resource ownership on new/changed functions.

The `quality-required` aggregate fails if any required job is failed, skipped or cancelled. Repository branch protection must require that check; a workflow file alone does not configure GitHub protection. No generation/build/publish command may swallow errors.

`tools/quality` is a checksum-sealed versioned vendor preset, validated by its own executable tests and integrity check; Knip analyzes the consumer tooling graph and excludes that nested vendor workspace. `java` is the explicitly provisioned native generator runtime (CI Temurin24), not an npm package; its pinned generator JAR checksum is mandatory. No authored consumer source is excluded.
