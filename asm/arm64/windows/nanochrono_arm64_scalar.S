.text
.align 2
.global nc_arm64_scalar_counter
nc_arm64_scalar_counter:
    mrs x0, cntvct_el0
    ret
.global nc_arm64_scalar_overhead
nc_arm64_scalar_overhead:
    mrs x1, cntvct_el0
    mrs x0, cntvct_el0
    sub x0, x0, x1
    ret

/* ---- NanoChronometer Toolkit low-level ARM64 scalar symbols ---- */
.global nc_arm64_cntvct
nc_arm64_cntvct:
    isb
    mrs x0, cntvct_el0
    ret
.global nc_arm64_cntfrq
nc_arm64_cntfrq:
    mrs x0, cntfrq_el0
    ret

.global nc_arm64_pmccntr_el0
nc_arm64_pmccntr_el0:
    isb
    mrs x0, pmccntr_el0
    ret

.global nc_arm64_isb
nc_arm64_isb:
    isb
    ret
.global nc_arm64_dmb_sy
nc_arm64_dmb_sy:
    dmb sy
    ret
.global nc_arm64_dsb_sy
nc_arm64_dsb_sy:
    dsb sy
    ret
.global nc_arm64_yield
nc_arm64_yield:
    yield
    ret
.global nc_arm64_load_u64
nc_arm64_load_u64:
    ldr x0, [x0]
    ret
.global nc_arm64_store_u64
nc_arm64_store_u64:
    str x1, [x0]
    ret
.global nc_arm64_xor_u64_stream
nc_arm64_xor_u64_stream:
    mov x2, #0
    cbz x1, 2f
1:
    ldr x3, [x0], #8
    eor x2, x2, x3
    subs x1, x1, #1
    b.ne 1b
2:
    mov x0, x2
    ret

/* ---- NanoChronometer low-level side-channel measurement API ---- */
.global nc_arm64_sc_cntvct_isb
nc_arm64_sc_cntvct_isb:
    isb
    mrs x0, cntvct_el0
    ret

.global nc_arm64_sc_measure_load_ticks
nc_arm64_sc_measure_load_ticks:
    cbz x0, 9f
    mov x9, x0
    isb
    mrs x10, cntvct_el0
    ldr x11, [x9]
    isb
    mrs x12, cntvct_el0
    sub x0, x12, x10
    ret
9:
    mov x0, #0
    ret

.global nc_arm64_sc_measure_load_isb_ticks
nc_arm64_sc_measure_load_isb_ticks:
    cbz x0, 9f
    mov x9, x0
    dsb sy
    isb
    mrs x10, cntvct_el0
    ldr x11, [x9]
    dsb sy
    isb
    mrs x12, cntvct_el0
    sub x0, x12, x10
    ret
9:
    mov x0, #0
    ret

.global nc_arm64_sc_measure_store_ticks
nc_arm64_sc_measure_store_ticks:
    cbz x0, 9f
    mov x9, x0
    mov x11, x1
    dsb sy
    isb
    mrs x10, cntvct_el0
    str x11, [x9]
    dsb sy
    isb
    mrs x12, cntvct_el0
    sub x0, x12, x10
    ret
9:
    mov x0, #0
    ret

.global nc_arm64_sc_measure_prefetch_load_ticks
nc_arm64_sc_measure_prefetch_load_ticks:
    cbz x0, 9f
    mov x9, x0
    prfm pldl1keep, [x9]
    isb
    mrs x10, cntvct_el0
    ldr x11, [x9]
    isb
    mrs x12, cntvct_el0
    sub x0, x12, x10
    ret
9:
    mov x0, #0
    ret

.global nc_arm64_sc_measure_branch_ticks
nc_arm64_sc_measure_branch_ticks:
    cbz x0, 9f
    cbz x1, 9f
    mov x9, x0
    mov x10, x1
    mov x13, #0
    isb
    mrs x11, cntvct_el0
1:
    ldrb w12, [x9], #1
    tbz w12, #0, 2f
    eor x13, x13, x10
2:
    subs x10, x10, #1
    b.ne 1b
    isb
    mrs x14, cntvct_el0
    sub x0, x14, x11
    ret
9:
    mov x0, #0
    ret

.global nc_arm64_sc_pointer_chase_ticks
nc_arm64_sc_pointer_chase_ticks:
    cbz x0, 9f
    cbz x1, 9f
    mov x9, x0
    mov x10, x1
    isb
    mrs x11, cntvct_el0
1:
    cbz x9, 2f
    ldr x9, [x9]
    subs x10, x10, #1
    b.ne 1b
2:
    isb
    mrs x12, cntvct_el0
    sub x0, x12, x11
    ret
9:
    mov x0, #0
    ret

.global nc_arm64_sc_barrier_overhead_ticks
nc_arm64_sc_barrier_overhead_ticks:
    cbnz w0, 1f
    mov w0, #1
1:
    mov w9, w0
    isb
    mrs x10, cntvct_el0
2:
    dmb sy
    dsb sy
    isb
    subs w9, w9, #1
    b.ne 2b
    mrs x11, cntvct_el0
    sub x0, x11, x10
    ret
