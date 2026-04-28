.text
.align 2

// -----------------------------------------------------------------------------
// NanoChronometer ARM64 SME2 timing/probe kernels.
//
// SME2 adds ZT0. The explicit SME2 operation used here is ZERO {ZT0}; it is
// encoded with .inst 0xc0480001 to avoid requiring every Android NDK/binutils
// assembler to know the SME2 mnemonic. Runtime must check HWCAP2_SME2 or the
// Windows PF_ARM_SME2_INSTRUCTIONS_AVAILABLE feature before dispatch.
// -----------------------------------------------------------------------------

#define NC_ARM64_SME2_ZERO_ZT0 .inst 0xc0480001

.global nc_arm64_sme2_touch
nc_arm64_sme2_touch:
    smstart za
    NC_ARM64_SME2_ZERO_ZT0
    smstop za
    ret

.global nc_arm64_sme2_counter
nc_arm64_sme2_counter:
    mrs x0, cntvct_el0
    ret

.global nc_arm64_sme2_overhead
nc_arm64_sme2_overhead:
    mrs x1, cntvct_el0
    mrs x0, cntvct_el0
    sub x0, x0, x1
    ret

.global nc_arm64_sme2_sc_counter
nc_arm64_sme2_sc_counter:
    isb
    mrs x0, cntvct_el0
    ret

.global nc_arm64_sme2_sc_load_ticks
nc_arm64_sme2_sc_load_ticks:
    cbnz x0, .sme2_load_ok
    mov x0, #0
    ret
.sme2_load_ok:
    mov x10, x0
    isb
    mrs x9, cntvct_el0
    ldr x11, [x10]
    isb
    mrs x0, cntvct_el0
    sub x0, x0, x9
    ret

.global nc_arm64_sme2_sc_store_ticks
nc_arm64_sme2_sc_store_ticks:
    cbnz x0, .sme2_store_ok
    mov x0, #0
    ret
.sme2_store_ok:
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

.global nc_arm64_sme2_sc_vector_load_ticks
nc_arm64_sme2_sc_vector_load_ticks:
    cbnz x0, .sme2_vload_ok
    mov x0, #0
    ret
.sme2_vload_ok:
    mov x10, x0
    isb
    mrs x9, cntvct_el0
    smstart za
    NC_ARM64_SME2_ZERO_ZT0
    ldr q0, [x10]
    eor v0.16b, v0.16b, v0.16b
    smstop za
    isb
    mrs x0, cntvct_el0
    sub x0, x0, x9
    ret

.global nc_arm64_sme2_sc_vector_xor_ticks
nc_arm64_sme2_sc_vector_xor_ticks:
    cbz x0, .sme2_vx_bad
    cbz x1, .sme2_vx_bad
    cbz x2, .sme2_vx_bad
    cmp x3, #16
    b.lo .sme2_vx_bad
    mov x10, x0
    mov x11, x1
    mov x12, x2
    isb
    mrs x9, cntvct_el0
    smstart za
    NC_ARM64_SME2_ZERO_ZT0
    ldr q0, [x10]
    ldr q1, [x11]
    eor v0.16b, v0.16b, v1.16b
    str q0, [x12]
    smstop za
    isb
    mrs x0, cntvct_el0
    sub x0, x0, x9
    ret
.sme2_vx_bad:
    mov x0, #0
    ret

.global nc_arm64_sme2_sc_barrier_ticks
nc_arm64_sme2_sc_barrier_ticks:
    cbnz x0, .sme2_bar_count_ok
    mov x0, #1
.sme2_bar_count_ok:
    mov x10, x0
    isb
    mrs x9, cntvct_el0
.sme2_bar_loop:
    smstart za
    NC_ARM64_SME2_ZERO_ZT0
    smstop za
    dsb sy
    isb
    subs x10, x10, #1
    b.ne .sme2_bar_loop
    mrs x0, cntvct_el0
    sub x0, x0, x9
    ret
