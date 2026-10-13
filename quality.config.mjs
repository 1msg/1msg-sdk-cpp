export default {
  schemaVersion: 1,
  profile: "D",
  defaultBranch: "main",
  sourceRoots: ["quality", "eslint.config.mjs", "quality.config.mjs"],
  generated: [],
  comments: {
    full: ["quality/**/*.{js,jsx,mjs,cjs}"],
  },
  duplicatePaths: ["quality"],
  duplicateMaxPercentage: 0,
  duplicateFullPaths: ["quality"],
  baselineDir: "quality/baselines",
};
