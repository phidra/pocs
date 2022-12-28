// Snowpack Configuration File
// See all supported options: https://www.snowpack.dev/reference/configuration

/** @type {import("snowpack").SnowpackUserConfig } */
module.exports = {
  root: "src",
  mount: {
    /* ... */
  },
  plugins: [
    '@snowpack/plugin-typescript'
  ],
  packageOptions: {
    types: true,
  },
  devOptions: {
    open: "none",
  },
  buildOptions: {
    out: "dist",
  },
};
