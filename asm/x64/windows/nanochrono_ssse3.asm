default rel
section .text
global nc_ssse3_touch
nc_ssse3_touch: ret
global nc_ssse3_tsc_start
nc_ssse3_tsc_start: lfence
    rdtsc
    shl rdx,32
    or rax,rdx
    ret
global nc_ssse3_tsc_end
nc_ssse3_tsc_end: rdtscp
    shl rdx,32
    or rax,rdx
    lfence
    ret
global nc_ssse3_tsc_overhead
nc_ssse3_tsc_overhead:
    call nc_ssse3_tsc_start
    mov r8,rax
    call nc_ssse3_tsc_end
    sub rax,r8
    ret



; -----------------------------------------------------------------------------
; NanoChronometer low-level SIMD side-channel timing probes (ssse3).
; Defensive local-buffer probes only. Crypto remains delegated to external libs.
; -----------------------------------------------------------------------------
global nc_ssse3_sc_counter
nc_ssse3_sc_counter:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    ret

global nc_ssse3_sc_load_cycles
nc_ssse3_sc_load_cycles:
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

global nc_ssse3_sc_store_cycles
nc_ssse3_sc_store_cycles:
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

global nc_ssse3_sc_vector_load_cycles
nc_ssse3_sc_vector_load_cycles:
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
    movdqu xmm0, [r10]
    pxor xmm1, xmm1
    pshufb xmm0, xmm1
    lfence
    rdtscp
    shl rdx, 32
    or rax, rdx
    lfence
    sub rax, r8
    ret

global nc_ssse3_sc_vector_xor_cycles
nc_ssse3_sc_vector_xor_cycles:
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
    movdqu xmm0, [r10]
    movdqu xmm1, [r11]
    pshufb xmm0, xmm1
    movdqu [r9], xmm0
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

global nc_ssse3_sc_barrier_cycles
nc_ssse3_sc_barrier_cycles:
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
