.text
.align 2
.global nc_arm64_neon_aes_loop
nc_arm64_neon_aes_loop:
    mov w1, w0
    movi v0.16b, #0
    movi v1.16b, #0
1:  aese v0.16b, v1.16b
    aesmc v0.16b, v0.16b
    subs w1, w1, #1
    b.ne 1b
    fmov x0, d0
    ret

.global nc_arm64_neon_sha256_loop
nc_arm64_neon_sha256_loop:
    mov w1, w0
    movi v0.4s, #0
    movi v1.4s, #0
    movi v2.4s, #0
2:  sha256h q0, q1, v2.4s
    subs w1, w1, #1
    b.ne 2b
    fmov x0, d0
    ret

.global nc_arm64_neon_pmull_loop
nc_arm64_neon_pmull_loop:
    mov w1, w0
    movi v0.16b, #1
    movi v1.16b, #3
3:  pmull v2.1q, v0.1d, v1.1d
    mov v0.16b, v2.16b
    subs w1, w1, #1
    b.ne 3b
    fmov x0, d0
    ret

.global nc_arm64_neon_vector_xor_loop
nc_arm64_neon_vector_xor_loop:
    mov x4, x0
    mov x5, x1
    mov x6, x2
    mov x7, x3
    mov x0, #0
4:  cmp x7, #16
    b.lo 5f
    ldr q0, [x4], #16
    ldr q1, [x5], #16
    eor v0.16b, v0.16b, v1.16b
    str q0, [x6], #16
    fmov x8, d0
    eor x0, x0, x8
    sub x7, x7, #16
    b 4b
5:  cbz x7, 6f
    ldrb w8, [x4], #1
    ldrb w9, [x5], #1
    eor w8, w8, w9
    strb w8, [x6], #1
    eor x0, x0, x8
    sub x7, x7, #1
    b 5b
6:  ret

.global nc_arm64_sve_vector_xor_loop
nc_arm64_sve_vector_xor_loop:
    b nc_arm64_neon_vector_xor_loop

.align 2
.global nc_asm_branch_probe_loop
nc_asm_branch_probe_loop:
    mov x8, x0        // pattern
    mov x9, x1        // count
    mov x0, #0        // accumulator
    cbz x8, 2f
1:
    cbz x9, 2f
    ldrb w10, [x8], #1
    cbz w10, 3f
    add x0, x0, #3
    b 4f
3:
    add x0, x0, #1
4:
    subs x9, x9, #1
    b.ne 1b
2:
    ret
