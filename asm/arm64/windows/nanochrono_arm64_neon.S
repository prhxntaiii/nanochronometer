.text
.align 2
.global nc_arm64_neon_touch
nc_arm64_neon_touch:
    ret
.global nc_arm64_neon_counter
nc_arm64_neon_counter:
    mrs x0, cntvct_el0
    ret
.global nc_arm64_neon_overhead
nc_arm64_neon_overhead:
    mrs x1, cntvct_el0
    mrs x0, cntvct_el0
    sub x0, x0, x1
    ret


// -----------------------------------------------------------------------------
// NanoChronometer low-level SIMD side-channel timing probes (arm64 neon).
// Defensive local-buffer probes only. Crypto remains delegated to external libs.
// -----------------------------------------------------------------------------
.global nc_arm64_neon_sc_counter
nc_arm64_neon_sc_counter:
    isb
    mrs x0, cntvct_el0
    ret

.global nc_arm64_neon_sc_load_ticks
nc_arm64_neon_sc_load_ticks:
    cbnz x0, .load_ok
    mov x0, #0
    ret
.load_ok:
    mov x10, x0
    isb
    mrs x9, cntvct_el0
    ldr x11, [x10]
    isb
    mrs x0, cntvct_el0
    sub x0, x0, x9
    ret

.global nc_arm64_neon_sc_store_ticks
nc_arm64_neon_sc_store_ticks:
    cbnz x0, .store_ok
    mov x0, #0
    ret
.store_ok:
    mov x10, x0
    mov x11, x1
    isb
    mrs x9, cntvct_el0
    str x11, [x10]
    dsb sy
    isb
    mrs x0, cntvct_el0
    sub x0, x0, x9
    ret

.global nc_arm64_neon_sc_vector_load_ticks
nc_arm64_neon_sc_vector_load_ticks:
    cbnz x0, .vload_ok
    mov x0, #0
    ret
.vload_ok:
    mov x10, x0
    isb
    mrs x9, cntvct_el0
    ldr q0, [x10]
    eor v0.16b, v0.16b, v0.16b
    isb
    mrs x0, cntvct_el0
    sub x0, x0, x9
    ret

.global nc_arm64_neon_sc_vector_xor_ticks
nc_arm64_neon_sc_vector_xor_ticks:
    cbz x0, .vx_bad
    cbz x1, .vx_bad
    cbz x2, .vx_bad
    mov x10, x0
    mov x11, x1
    mov x12, x2
    isb
    mrs x9, cntvct_el0
    cmp x3, #16
    b.lo .vx_bad
    ldr q0, [x10]
    ldr q1, [x11]
    eor v0.16b, v0.16b, v1.16b
    str q0, [x12]
    isb
    mrs x0, cntvct_el0
    sub x0, x0, x9
    ret
.vx_bad:
    mov x0, #0
    ret

.global nc_arm64_neon_sc_barrier_ticks
nc_arm64_neon_sc_barrier_ticks:
    cbnz x0, .bar_count_ok
    mov x0, #1
.bar_count_ok:
    mov x10, x0
    isb
    mrs x9, cntvct_el0
.bar_loop:
    dsb sy
    isb
    subs x10, x10, #1
    b.ne .bar_loop
    mrs x0, cntvct_el0
    sub x0, x0, x9
    ret
