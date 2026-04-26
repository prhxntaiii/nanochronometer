; =============================================================================
;  nanochrono_avx_vnni.asm - isolated AVX-VNNI timer backend entry points
;  Windows x64 ABI | NASM syntax | bits 64
;
;  Timer entry points intentionally do NOT execute AVX-VNNI-specific opcodes.
;  They use safe LFENCE/RDTSC/RDTSCP only. Family-specific instructions belong
;  in explicit benchmark kernels after CPUID+XGETBV gating.
; =============================================================================

bits 64
default rel
section .text

%macro TSC64 0
    shl  rdx, 32
    or   rax, rdx
%endmacro

align 32
global nc_avx_vnni_touch
nc_avx_vnni_touch:
    ret

align 32
global nc_avx_vnni_tsc_start
nc_avx_vnni_tsc_start:
    lfence
    rdtsc
    TSC64
    ret

align 32
global nc_avx_vnni_tsc_end
nc_avx_vnni_tsc_end:
    rdtscp
    TSC64
    lfence
    ret

align 32
global nc_avx_vnni_tsc_overhead
nc_avx_vnni_tsc_overhead:
    mov     r10, 16
    mov     r9, -1
align 16
.loop:
    lfence
    rdtsc
    TSC64
    mov     r8, rax
    lfence
    rdtsc
    TSC64
    sub     rax, r8
    cmp     rax, r9
    cmovb   r9, rax
    dec     r10
    jnz     .loop
    mov     rax, r9
    ret

; -----------------------------------------------------------------------------
; Explicit AVX-VNNI benchmark kernel.
; Not used by GUI startup/timer creation. Called only after CPUID.7.1.EAX[4]
; and OS YMM state checks pass.
; -----------------------------------------------------------------------------
align 32
global nc_bench_kernel_avx_vnni
nc_bench_kernel_avx_vnni:
    mov     r8, rcx
    vpxor   ymm0, ymm0, ymm0
    vpcmpeqb ymm1, ymm1, ymm1
    vpcmpeqb ymm2, ymm2, ymm2
    test    r8, r8
    jz      .done
align 16
.loop:
    vpdpbusd ymm0, ymm1, ymm2
    vpxor   ymm1, ymm1, ymm0
    dec     r8
    jnz     .loop
.done:
    vextracti128 xmm1, ymm0, 1
    vpxor   xmm0, xmm0, xmm1
    movq    rax, xmm0
    vzeroupper
    ret



; -----------------------------------------------------------------------------
; NanoChronometer low-level SIMD side-channel timing probes (avx_vnni).
; Defensive local-buffer probes only. Crypto remains delegated to external libs.
; -----------------------------------------------------------------------------
global nc_avx_vnni_sc_counter
nc_avx_vnni_sc_counter:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    ret

global nc_avx_vnni_sc_load_cycles
nc_avx_vnni_sc_load_cycles:
    mov r10, rcx
    test r10, r10
    jnz .load_ok
    xor eax, eax
    ret
.load_ok:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    mov r8, rax
    mov rax, [r10]
    lfence
    rdtscp
    shl rdx, 32
    or rax, rdx
    lfence
    sub rax, r8
    ret

global nc_avx_vnni_sc_store_cycles
nc_avx_vnni_sc_store_cycles:
    mov r10, rcx
    mov r11, rdx
    test r10, r10
    jnz .store_ok
    xor eax, eax
    ret
.store_ok:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    mov r8, rax
    mov [r10], r11
    mfence
    rdtscp
    shl rdx, 32
    or rax, rdx
    lfence
    sub rax, r8
    ret

global nc_avx_vnni_sc_vector_load_cycles
nc_avx_vnni_sc_vector_load_cycles:
    mov r10, rcx
    test r10, r10
    jnz .vload_ok
    xor eax, eax
    ret
.vload_ok:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    mov r8, rax
    vpxor ymm0, ymm0, ymm0
    vpcmpeqb ymm1, ymm1, ymm1
    vpdpbusd ymm0, ymm1, [r10]
    vzeroupper
    lfence
    rdtscp
    shl rdx, 32
    or rax, rdx
    lfence
    sub rax, r8
    ret

global nc_avx_vnni_sc_vector_xor_cycles
nc_avx_vnni_sc_vector_xor_cycles:
    mov r10, rcx
    mov r11, rdx
    mov r9, r8
    test r10, r10
    jz .vx_bad
    test r11, r11
    jz .vx_bad
    test r9, r9
    jz .vx_bad
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    mov r8, rax
    vpxor ymm0, ymm0, ymm0
    vpcmpeqb ymm1, ymm1, ymm1
    vpdpbusd ymm0, ymm1, [r11]
    vmovdqu [r9], ymm0
    vzeroupper
    lfence
    rdtscp
    shl rdx, 32
    or rax, rdx
    lfence
    sub rax, r8
    ret
.vx_bad:
    xor eax, eax
    ret

global nc_avx_vnni_sc_barrier_cycles
nc_avx_vnni_sc_barrier_cycles:
    mov ecx, ecx
    test ecx, ecx
    jnz .bar_count_ok
    mov ecx, 1
.bar_count_ok:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    mov r8, rax
.bar_loop:
    lfence
    dec ecx
    jnz .bar_loop
    rdtscp
    shl rdx, 32
    or rax, rdx
    lfence
    sub rax, r8
    ret
