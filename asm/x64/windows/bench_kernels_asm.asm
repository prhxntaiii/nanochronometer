; =============================================================================
; bench_kernels_asm.asm - scalar legacy + MMX benchmark kernels
; Windows x64 ABI, NASM syntax
; =============================================================================

bits 64
default rel
section .text

; uint64_t nc_bench_kernel_asm_legacy(size_t loops)
; RCX = loops
align 32
global nc_bench_kernel_asm_legacy
nc_bench_kernel_asm_legacy:
    mov     r8, rcx
    mov     rax, 0x0123456789ABCDEF
    mov     rdx, 0x9E3779B97F4A7C15
    mov     r10, 0xD1B54A32D192ED03
    test    r8, r8
    jz      .done_scalar
.loop_scalar:
    add     rax, rdx
    rol     rax, 13
    xor     rax, r10
    imul    rax, rax, 0x100000001B3
    add     rdx, rax
    ror     r10, 7
    xor     r10, rdx
    dec     r8
    jnz     .loop_scalar
.done_scalar:
    xor     rax, rdx
    xor     rax, r10
    ret

; uint64_t nc_bench_kernel_mmx(size_t loops)
; RCX = loops
; Uses only legacy MMX integer SIMD and calls EMMS before return.
align 32
global nc_bench_kernel_mmx
nc_bench_kernel_mmx:
    mov     r8, rcx
    mov     rax, 0x0123456789ABCDEF
    mov     rdx, 0x9E3779B97F4A7C15
    movq    mm0, rax
    movq    mm1, rdx
    pxor    mm2, mm2
    test    r8, r8
    jz      .done_mmx
.loop_mmx:
    paddd   mm0, mm1
    pxor    mm2, mm0
    psllq   mm1, 1
    paddd   mm1, mm2
    psrlq   mm2, 3
    pxor    mm0, mm2
    dec     r8
    jnz     .loop_mmx
.done_mmx:
    pxor    mm0, mm1
    pxor    mm0, mm2
    movq    rax, mm0
    emms
    ret

