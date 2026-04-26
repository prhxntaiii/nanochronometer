default rel
section .text
global nc_avx512_vnni_touch
nc_avx512_vnni_touch: ret
global nc_avx512_vnni_tsc_start
nc_avx512_vnni_tsc_start: lfence
    rdtsc
    shl rdx,32
    or rax,rdx
    ret
global nc_avx512_vnni_tsc_end
nc_avx512_vnni_tsc_end: rdtscp
    shl rdx,32
    or rax,rdx
    lfence
    ret
global nc_avx512_vnni_tsc_overhead
nc_avx512_vnni_tsc_overhead:
    call nc_avx512_vnni_tsc_start
    mov r8,rax
    call nc_avx512_vnni_tsc_end
    sub rax,r8
    ret



; -----------------------------------------------------------------------------
; NanoChronometer low-level SIMD side-channel timing probes (avx512_vnni).
; Defensive local-buffer probes only. Crypto remains delegated to external libs.
; -----------------------------------------------------------------------------
global nc_avx512_vnni_sc_counter
nc_avx512_vnni_sc_counter:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    ret

global nc_avx512_vnni_sc_load_cycles
nc_avx512_vnni_sc_load_cycles:
    mov r10, rdi
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

global nc_avx512_vnni_sc_store_cycles
nc_avx512_vnni_sc_store_cycles:
    mov r10, rdi
    mov r11, rsi
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

global nc_avx512_vnni_sc_vector_load_cycles
nc_avx512_vnni_sc_vector_load_cycles:
    mov r10, rdi
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
    vpxord zmm0, zmm0, zmm0
    mov eax, -1
    vpbroadcastd zmm1, eax
    vpdpbusd zmm0, zmm1, [r10]
    vzeroupper
    lfence
    rdtscp
    shl rdx, 32
    or rax, rdx
    lfence
    sub rax, r8
    ret

global nc_avx512_vnni_sc_vector_xor_cycles
nc_avx512_vnni_sc_vector_xor_cycles:
    mov r10, rdi
    mov r11, rsi
    mov r9, rdx
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
    vpxord zmm0, zmm0, zmm0
    mov eax, -1
    vpbroadcastd zmm1, eax
    vpdpbusd zmm0, zmm1, [r11]
    vmovdqu64 [r9], zmm0
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

global nc_avx512_vnni_sc_barrier_cycles
nc_avx512_vnni_sc_barrier_cycles:
    mov ecx, edi
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
