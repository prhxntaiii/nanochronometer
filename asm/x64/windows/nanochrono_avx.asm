; =============================================================================
;  nanochrono_avx.asm  -  TSC + SIMD utility backend (AVX / SSE2)
;  Windows x64 ABI  |  NASM syntax  |  bits 64
;
;  Requires: SSE2, AVX (VZEROALL / VZEROUPPER / VMOVDQU 256-bit)
;  Functions tagged [AVX2] need nc_avx2_check() == 1.
;
;  Serialisation strategy:
;    START  -> LFENCE  (prevents RDTSC from being reordered before loads)
;    END    -> RDTSCP + LFENCE  (waits + blocks subsequent reorder)
;    FULL   -> MFENCE + VZEROALL/VZEROUPPER + LFENCE  (drain everything)
;
;  Register policy (Windows x64 ABI):
;    Volatile  :  RAX RCX RDX R8 R9 R10 R11 | XMM0-5  YMM0-5
;    Preserved :  RBX RSI RDI RBP R12-R15   | XMM6-15 YMM6-15 (uppers)
;    Spill rule:  ONLY push RBX when CPUID is required (trashes EBX).
;                 All other scratch uses R10/R11 -> zero spills.
;
;  Cache-line alignment:
;    Every exported entry point -> align 32 (half a 64-byte cache line).
;    Hot inner loops            -> align 16.
;
;  SIMD instructions present in this file:
;    VMOVDQU    - 256-bit unaligned load/store                  (AVX)
;    VMOVDQA    - 256-bit aligned   load/store                  (AVX)
;    VMOVNTDQ   - 256-bit non-temporal (streaming) store        (AVX)
;    VPXOR      - 256-bit packed XOR                            (AVX)
;    VEXTRACTF128 - extract 128-bit lane from YMM               (AVX)
;    PXOR       - 128-bit XOR                                   (SSE2)
;    PADDQ      - 128-bit packed 64-bit add                     (SSE2)
;    MOVDQU     - 128-bit unaligned load/store                  (SSE2)
;    PSHUFD     - 128-bit shuffle doublewords                   (SSE2)
;    PREFETCHT0 / PREFETCHT1 / PREFETCHT2 / PREFETCHNTA
;    PAUSE      - spin-wait hint
;    MFENCE / LFENCE / SFENCE
;    VZEROALL / VZEROUPPER
;    RDTSC / RDTSCP
; =============================================================================

bits 64
default rel
section .text

%macro TSC64 0
    shl  rdx, 32
    or   rax, rdx
%endmacro

; =============================================================================
;  GROUP 1 - Core TSC reads
; =============================================================================

align 32
global nc_avx_tsc_start
nc_avx_tsc_start:
    lfence
    rdtsc
    TSC64
    ret

align 32
global nc_avx_tsc_end
nc_avx_tsc_end:
    rdtscp
    TSC64
    lfence
    ret

align 32
global nc_avx_tsc_raw
nc_avx_tsc_raw:
    rdtsc
    TSC64
    ret

align 32
global nc_avx_tsc_full_start
nc_avx_tsc_full_start:
    vzeroall
    lfence
    rdtsc
    TSC64
    ret

align 32
global nc_avx_tsc_full_end
nc_avx_tsc_full_end:
    rdtscp
    TSC64
    lfence
    mov     r10, rax
    vzeroall
    mov     rax, r10
    ret

align 32
global nc_avx_tsc_upper_start
nc_avx_tsc_upper_start:
    vzeroupper
    lfence
    rdtsc
    TSC64
    ret

align 32
global nc_avx_tsc_upper_end
nc_avx_tsc_upper_end:
    rdtscp
    TSC64
    lfence
    mov     r10, rax
    vzeroupper
    mov     rax, r10
    ret

align 32
global nc_avx_tsc_sfence_start
nc_avx_tsc_sfence_start:
    sfence
    lfence
    rdtsc
    TSC64
    ret

align 32
global nc_avx_tsc_sfence_end
nc_avx_tsc_sfence_end:
    rdtscp
    TSC64
    lfence
    sfence
    ret

align 32
global nc_avx_tsc_mfence_start
nc_avx_tsc_mfence_start:
    mfence
    lfence
    rdtsc
    TSC64
    ret

align 32
global nc_avx_tsc_mfence_end
nc_avx_tsc_mfence_end:
    rdtscp
    TSC64
    mfence
    lfence
    ret

; =============================================================================
;  GROUP 2 - RDTSCP auxiliary / pairing
; =============================================================================

align 32
global nc_avx_tsc_aux
nc_avx_tsc_aux:
    rdtscp
    mov     eax, ecx
    ret

; nc_avx_tsc_rdtscp_pair  ->  void
;   RCX = ptr { uint64_t tsc; uint32_t aux; uint32_t _pad; }
;   R10 = ptr save, R11 = aux  (both volatile -> no spill)
align 32
global nc_avx_tsc_rdtscp_pair
nc_avx_tsc_rdtscp_pair:
    mov     r10, rcx
    rdtscp
    mov     r11d, ecx
    TSC64
    lfence
    mov     [r10],   rax
    mov     [r10+8], r11d
    ret

; =============================================================================
;  GROUP 3 - Delta / elapsed
; =============================================================================

; nc_avx_tsc_delta  ->  uint64_t (end-start, 0 on underflow)
align 32
global nc_avx_tsc_delta
nc_avx_tsc_delta:
    mov     rax, rdx
    sub     rax, rcx
    jnc     .ok
    xor     eax, eax
.ok:
    ret

; nc_avx_tsc_elapsed_from  ->  uint64_t
;   RCX = start -> (RDTSCP now) - start   [R10 = start copy, no spill]
align 32
global nc_avx_tsc_elapsed_from
nc_avx_tsc_elapsed_from:
    mov     r10, rcx
    rdtscp
    TSC64
    lfence
    sub     rax, r10
    ret

; =============================================================================
;  GROUP 4 - Spin
; =============================================================================

align 32
global nc_avx_spin_until
nc_avx_spin_until:
align 16
.loop:
    rdtsc
    TSC64
    cmp     rax, rcx
    jae     .done
    pause
    jmp     .loop
.done:
    ret

; =============================================================================
;  GROUP 5 - Overhead / invariant
; =============================================================================

; nc_avx_tsc_overhead  ->  uint64_t  (best-of-16 back-to-back delta)
;   Temporaries: R8=t0, R10=counter, R11=best   -> all volatile, no spill
align 32
global nc_avx_tsc_overhead
nc_avx_tsc_overhead:
    mov     r10, 16
    mov     r11, -1
align 16
.loop:
    lfence
    rdtsc
    TSC64
    mov     r8, rax
    rdtscp
    TSC64
    lfence
    sub     rax, r8
    cmp     rax, r11
    cmovb   r11, rax
    dec     r10
    jnz     .loop
    mov     rax, r11
    ret

; nc_avx_tsc_invariant  ->  int  (CPUID 80000007H EDX bit8)
align 32
global nc_avx_tsc_invariant
nc_avx_tsc_invariant:
    push    rbx
    mov     eax, 0x80000007
    xor     ecx, ecx
    cpuid
    shr     edx, 8
    and     edx, 1
    mov     eax, edx
    pop     rbx
    ret

; =============================================================================
;  GROUP 6 - YMM state
; =============================================================================

align 32
global nc_avx_vzeroall
nc_avx_vzeroall:
    vzeroall
    ret

align 32
global nc_avx_warmup
nc_avx_warmup:
    vzeroall
    times 32 nop
    lfence
    ret

; =============================================================================
;  GROUP 7 - SIMD memory  (VMOVDQU 256-bit)
; =============================================================================

; nc_avx_simd_zero_buf  ->  void
;   RCX = dst (any alignment), RDX = bytes (truncated to multiple of 32)
;   Stores 256-bit zeros via VMOVDQU.  YMM0 volatile -> no save.
align 32
global nc_avx_simd_zero_buf
nc_avx_simd_zero_buf:
    vxorps  ymm0, ymm0, ymm0 ; zero (vxorps=AVX1)
    test    rdx, rdx
    jz      .done
    and     rdx, ~31
    jz      .done
align 16
.loop:
    vmovdqu  [rcx], ymm0
    add      rcx, 32
    sub      rdx, 32
    jnz      .loop
.done:
    vzeroupper
    ret

; nc_avx_simd_copy_buf  ->  void
;   RCX = dst, RDX = src, R8 = bytes (truncated to multiple of 32)
;   256-bit VMOVDQU load + store.
align 32
global nc_avx_simd_copy_buf
nc_avx_simd_copy_buf:
    test    r8, r8
    jz      .done
    and     r8, ~31
    jz      .done
align 16
.loop:
    vmovdqu  ymm0, [rdx]
    vmovdqu  [rcx], ymm0
    add      rcx, 32
    add      rdx, 32
    sub      r8,  32
    jnz      .loop
.done:
    vzeroupper
    ret

; nc_avx_simd_xor_checksum  ->  uint64_t   [requires AVX2 for vpxor ymm loop]
;   RCX = ptr, RDX = bytes (truncated to multiple of 32)
;   256-bit VPXOR accumulator folded to 64 bits via VEXTRACTF128 + PXOR.
align 32
global nc_avx_simd_xor_checksum
nc_avx_simd_xor_checksum:
    vxorps   ymm0, ymm0, ymm0 ; zero (vxorps=AVX1)
    test     rdx, rdx
    jz       .reduce
    and      rdx, ~31
    jz       .reduce
align 16
.loop:
    vmovdqu      ymm1, [rcx]
    vpxor        ymm0, ymm0, ymm1
    add          rcx, 32
    sub          rdx, 32
    jnz          .loop
.reduce:
    vextractf128 xmm1, ymm0, 1      ; upper 128 -> xmm1
    pxor         xmm0, xmm1         ; fold: XOR upper into lower  (SSE2)
    pshufd       xmm1, xmm0, 0x4E  ; swap 64-bit halves
    pxor         xmm0, xmm1         ; fold -> 64-bit result
    movq         rax,  xmm0
    vzeroupper
    ret

; nc_avx_simd_sum_u64  ->  uint64_t
;   RCX = uint64_t*, RDX = count  ->  RAX = wrapping sum
;   VMOVDQU 256-bit loads + VEXTRACTF128 + PADDQ (no AVX2 required).
align 32
global nc_avx_simd_sum_u64
nc_avx_simd_sum_u64:
    pxor    xmm0, xmm0
    pxor    xmm1, xmm1
    test    rdx, rdx
    jz      .reduce
    mov     r10, rdx
    shr     r10, 2
    jz      .tail
align 16
.loop4:
    vmovdqu      ymm2, [rcx]
    vextractf128 xmm3, ymm2, 0
    vextractf128 xmm4, ymm2, 1
    paddq        xmm0, xmm3
    paddq        xmm1, xmm4
    add          rcx, 32
    dec          r10
    jnz          .loop4
    paddq        xmm0, xmm1
.tail:
    and     rdx, 3
    jz      .reduce
align 16
.scalar:
    movq    xmm2, [rcx]
    paddq   xmm0, xmm2
    add     rcx, 8
    dec     rdx
    jnz     .scalar
.reduce:
    pshufd  xmm1, xmm0, 0x4E
    paddq   xmm0, xmm1
    movq    rax, xmm0
    vzeroupper
    ret

; nc_avx_simd_nt_zero  ->  void
;   RCX = dst (32-byte ALIGNED), RDX = bytes (multiple of 32)
;   Non-temporal (streaming) stores via VMOVNTDQ -> bypasses cache.
;   Caller must issue SFENCE after all NT stores are complete.
align 32
global nc_avx_simd_nt_zero
nc_avx_simd_nt_zero:
    vxorps  ymm0, ymm0, ymm0 ; zero (vxorps=AVX1)
    test    rdx, rdx
    jz      .done
    and     rdx, ~31
    jz      .done
align 16
.loop:
    vmovntdq  [rcx], ymm0
    add       rcx, 32
    sub       rdx, 32
    jnz       .loop
    sfence
.done:
    vzeroupper
    ret

; nc_avx_simd_prefetch_loop  ->  void
;   RCX = ptr, RDX = bytes, R8 = stride  (prefetches cache lines)
;   Issues PREFETCHT0 every R8 bytes across the range [RCX, RCX+RDX).
align 32
global nc_avx_simd_prefetch_loop
nc_avx_simd_prefetch_loop:
    test    rdx, rdx
    jz      .done
    test    r8, r8
    jz      .done
align 16
.loop:
    prefetcht0 [rcx]
    add     rcx, r8
    sub     rdx, r8
    ja      .loop
.done:
    ret

; =============================================================================
;  GROUP 8 - Prefetch-enhanced TSC reads
; =============================================================================

; nc_avx_prefetch_tsc_start  ->  uint64_t
;   RCX = optional ptr to prefetch (NULL = skip).
;   PREFETCHT0 [RCX] -> LFENCE -> RDTSC.
align 32
global nc_avx_prefetch_tsc_start
nc_avx_prefetch_tsc_start:
    test    rcx, rcx
    jz      .no_pf
    prefetcht0 [rcx]
.no_pf:
    lfence
    rdtsc
    TSC64
    ret

; nc_avx_prefetch_tsc_end  ->  uint64_t
;   RDTSCP + LFENCE, then optional PREFETCHT1 on RCX.
;   R10 = TSC preserve (volatile, no spill).
align 32
global nc_avx_prefetch_tsc_end
nc_avx_prefetch_tsc_end:
    rdtscp
    TSC64
    lfence
    mov     r10, rax
    test    rcx, rcx
    jz      .no_pf
    prefetcht1 [rcx]
.no_pf:
    mov     rax, r10
    ret

; nc_avx_tsc_pause_start  ->  uint64_t
;   16 x PAUSE to drain the OOO pipeline -> LFENCE -> RDTSC.
align 32
global nc_avx_tsc_pause_start
nc_avx_tsc_pause_start:
    times 16 pause
    lfence
    rdtsc
    TSC64
    ret

; nc_avx_tsc_full_fence  ->  uint64_t
;   MFENCE + VZEROUPPER + LFENCE + RDTSC  (maximum isolation start fence)
align 32
global nc_avx_tsc_full_fence
nc_avx_tsc_full_fence:
    mfence
    vzeroupper
    lfence
    rdtsc
    TSC64
    ret

; =============================================================================
;  GROUP 9 - Sequential RDTSCP stamp (for lock-free ring-buffer use)
; =============================================================================

; nc_avx_tsc_rdtscp_seq  ->  void
;   RCX = ptr { uint64_t tsc; uint32_t aux; uint32_t seq; }
;   Stores tsc+aux from RDTSCP, increments seq in-place.
align 32
global nc_avx_tsc_rdtscp_seq
nc_avx_tsc_rdtscp_seq:
    mov     r10, rcx
    rdtscp
    mov     r11d, ecx
    TSC64
    lfence
    mov     [r10],    rax
    mov     [r10+8],  r11d
    mov     r8d, dword [r10+12]
    inc     r8d
    mov     dword [r10+12], r8d
    ret

; =============================================================================
;  GROUP 10 - CPU feature detection
; =============================================================================

; nc_avx_check  ->  int  (1 = AVX + OSXSAVE present, 0 = not)
align 32
global nc_avx_check
nc_avx_check:
    push    rbx
    mov     eax, 1
    xor     ecx, ecx
    cpuid
    ; need ECX bit27 (OSXSAVE) and bit28 (AVX) both set.
    ; Do NOT use PF/parity here. If both bits are zero, PF is set and
    ; XGETBV would run without OSXSAVE, causing #UD before the GUI opens.
    mov     eax, ecx
    and     eax, (1 << 27) | (1 << 28)
    cmp     eax, (1 << 27) | (1 << 28)
    jne     .no
    xor     ecx, ecx
    xgetbv
    and     eax, 0x6
    cmp     eax, 0x6
    jne     .no
    mov     eax, 1
    pop     rbx
    ret
.no:
    xor     eax, eax
    pop     rbx
    ret

; nc_avx2_check  ->  int  (1 = AVX2 present, 0 = not; implies AVX)
align 32
global nc_avx2_check
nc_avx2_check:
    push    rbx
    xor     eax, eax
    xor     ecx, ecx
    cpuid
    cmp     eax, 7
    jb      .no
    mov     eax, 7
    xor     ecx, ecx
    cpuid                           ; EBX bit5 = AVX2
    mov     eax, ebx
    shr     eax, 5
    and     eax, 1
    pop     rbx
    ret
.no:
    xor     eax, eax
    pop     rbx
    ret

; =============================================================================
;  Backward-compat aliases
; =============================================================================

global tsc_start_simd
align 16
tsc_start_simd:
    jmp nc_avx_tsc_start

global tsc_end_simd
align 16
tsc_end_simd:
    jmp nc_avx_tsc_end



; -----------------------------------------------------------------------------
; NanoChronometer low-level SIMD side-channel timing probes (avx).
; Defensive local-buffer probes only. Crypto remains delegated to external libs.
; -----------------------------------------------------------------------------
global nc_avx_sc_counter
nc_avx_sc_counter:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    ret

global nc_avx_sc_load_cycles
nc_avx_sc_load_cycles:
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

global nc_avx_sc_store_cycles
nc_avx_sc_store_cycles:
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

global nc_avx_sc_vector_load_cycles
nc_avx_sc_vector_load_cycles:
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
    vmovups ymm0, [r10]
    vxorps ymm0, ymm0, ymm0
    vzeroupper
    lfence
    rdtscp
    shl rdx, 32
    or rax, rdx
    lfence
    sub rax, r8
    ret

global nc_avx_sc_vector_xor_cycles
nc_avx_sc_vector_xor_cycles:
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
    vmovups ymm0, [r10]
    vxorps ymm0, ymm0, [r11]
    vmovups [r9], ymm0
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

global nc_avx_sc_barrier_cycles
nc_avx_sc_barrier_cycles:
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
