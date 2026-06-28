/**
 * nanochrono_wasi.js — Node.js WASI runner for nanochrono_wasi.wasm
 *
 * Usage:
 *   node nanochrono_wasi.js [--feature timing.monotonic] [--bench timing]
 *
 * The WASI sandbox exposes only monotonic/realtime clocks.
 * All hardware benchmarks are reported as "N/A".
 */

'use strict';

const { WASI } = require('wasi');
const { readFileSync } = require('fs');
const { argv } = require('process');

async function main() {
  const wasmPath = argv[2] || new URL('./nanochrono_wasi.wasm', import.meta?.url || __filename).pathname;

  const wasi = new WASI({
    version: 'preview1',
    args: argv,
    env: process.env,
  });

  let wasmBytes;
  try {
    wasmBytes = readFileSync(wasmPath);
  } catch (e) {
    console.error(`[nanochrono_wasi] Cannot read ${wasmPath}: ${e.message}`);
    console.error('Build first: emcmake cmake -B build_wasm wrappers/wasm && cmake --build build_wasm');
    process.exit(1);
  }

  const { instance } = await WebAssembly.instantiate(wasmBytes, {
    ...wasi.getImportObject(),
  });
  wasi.start(instance);

  const exports = instance.exports;
  const { memory } = exports;

  function cstr(s) {
    const buf = Buffer.from(s + '\0', 'utf8');
    const ptr = exports.__stack_pointer?.value ?? 0x10000;
    const view = new Uint8Array(memory.buffer, ptr, buf.length);
    view.set(buf);
    return ptr;
  }

  const nowNs    = exports.nc_wasm_now_ns;
  const featAvail = exports.nc_wasm_feature_available;
  const version  = exports.nc_wasm_version;

  console.log('NanoChronometer WASI Runner');
  if (version) {
    /* version returns a pointer; decode it */
    const vptr = version();
    const bytes = new Uint8Array(memory.buffer, vptr, 64);
    let vstr = '';
    for (let i = 0; i < 64 && bytes[i]; i++) vstr += String.fromCharCode(bytes[i]);
    console.log('Version:', vstr);
  }

  // Feature probes
  const features = [
    'timing.monotonic', 'timing.realtime',
    'hw.rdtsc', 'hw.cntpct', 'hw.cntvct',
    'crypto.sha256', 'crypto.hmac', 'crypto.rand',
    'simd.aes', 'simd.avx2', 'simd.neon',
  ];

  console.log('\nFeature availability:');
  for (const f of features) {
    const ptr = cstr(f);
    const avail = featAvail ? featAvail(ptr) : 0;
    console.log(`  ${f.padEnd(24)} ${avail ? 'available' : 'N/A'}`);
  }

  if (nowNs) {
    const a = BigInt(nowNs());
    // Busy-wait ~100µs
    let sum = 0n;
    for (let i = 0; i < 1000000; i++) sum += BigInt(i);
    const b = BigInt(nowNs());
    console.log(`\nTiming loop: ${b - a} ns (sum=${sum} to prevent optimisation)`);
  } else {
    console.log('\nnc_wasm_now_ns not exported — check build flags');
  }
}

main().catch(e => { console.error(e); process.exit(1); });
