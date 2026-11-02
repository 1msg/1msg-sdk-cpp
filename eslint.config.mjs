import { staticConfig, globals } from "./tools/quality/eslint.mjs";
export default [
  {
    files: ["**/*.jsx"],
    languageOptions: { parserOptions: { ecmaFeatures: { jsx: true } } },
  },
  {
    ignores: [
      "**/node_modules/**",
      "tools/quality/**",
      "vendor/**",
      ".build/**",
      ".quality-build/**",
    ],
  },
  ...staticConfig({ globals: globals.node }),
];
