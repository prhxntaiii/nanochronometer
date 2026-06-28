/**
 * nanochrono.js — Browser/Node.js glue for the NanoChronometer WASM module.
 *
 * Usage (Emscripten-compiled output):
 *   const NC = await NanoChronometer();   // load nanochrono_browser.js
 *   const now = NC.ccall('nc_wasm_now_ns', 'bigint', [], []);
 *   const avail = NC.ccall('nc_wasm_feature_available', 'number', ['string'], ['timing.monotonic']);
 *
 * All hardware features return 0 (not available) — call nc_wasm_feature_available()
 * before using any measurement and report "N/A" when it returns 0.
 */

'use strict';

const NC_STATUS_OK          =  0;
const NC_STATUS_UNSUPPORTED = -1;
const NC_STATUS_BAD_ARG     = -2;
const NC_STATUS_CRYPTO_ERR  = -3;

/**
 * Thin JS wrapper around the Emscripten-compiled WASM module.
 * Wraps the C functions with BigInt and N/A handling.
 */
class NanoChronometer {
  constructor(module) {
    this._m = module;
    this._nowNs   = module.cwrap('nc_wasm_now_ns',           'bigint', []);
    this._start   = module.cwrap('nc_wasm_start',             'bigint', []);
    this._elapsed = module.cwrap('nc_wasm_elapsed_ns',        'bigint', []);
    this._featAvail = module.cwrap('nc_wasm_feature_available','number', ['string']);
    this._version = module.cwrap('nc_wasm_version',           'string', []);
  }

  /** Current monotonic time in nanoseconds. */
  nowNs() { return this._nowNs(); }

  /** Start a measurement; returns start timestamp in ns. */
  start() { return this._start(); }

  /** Nanoseconds elapsed since last start(). */
  elapsedNs() { return this._elapsed(); }

  /** Returns true if a named feature is available in this WASM environment. */
  featureAvailable(name) { return this._featAvail(name) !== 0; }

  /** Library version string. */
  version() { return this._version(); }

  /**
   * Measure a callback's execution time in nanoseconds.
   * Uses WASI monotonic clock — no RDTSC access.
   */
  measureNs(fn) {
    const a = this._nowNs();
    fn();
    const b = this._nowNs();
    return b > a ? b - a : 0n;
  }

  /**
   * Report a benchmark result, printing "N/A" for hardware features
   * that are unavailable in the WASM sandbox.
   */
  report(featureName, measureFn) {
    if (!this.featureAvailable(featureName)) {
      return { status: 'N/A', feature: featureName, cycles: null, ns: null };
    }
    const ns = this.measureNs(measureFn);
    return { status: 'ok', feature: featureName, cycles: null, ns };
  }
}

/**
 * Load the NanoChronometer WASM module.
 * @param {string} wasmPath - path to nanochrono_browser.js (Emscripten output)
 * @returns {Promise<NanoChronometer>}
 */
async function loadNanoChronometer(wasmPath) {
  if (typeof window !== 'undefined') {
    // Browser: load the Emscripten module
    const script = document.createElement('script');
    script.src = wasmPath || 'nanochrono_browser.js';
    await new Promise((res, rej) => {
      script.onload = res; script.onerror = rej;
      document.head.appendChild(script);
    });
    const mod = await window.NanoChronometer();
    return new NanoChronometer(mod);
  } else {
    // Node.js
    const factory = require(wasmPath || './nanochrono_browser.js');
    const mod = await factory();
    return new NanoChronometer(mod);
  }
}

if (typeof module !== 'undefined') {
  module.exports = { NanoChronometer, loadNanoChronometer,
                     NC_STATUS_OK, NC_STATUS_UNSUPPORTED };
}
