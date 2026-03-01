; =============================================================================
;  nanochrono_legacy.asm  -  TSC + SIMD utility backend (SSE2, no AVX)
;  Windows x64 ABI  |  NASM syntax  |  bits 64
;
;  Requires: SSE2  (all x86-64 CPUs support SSE2)
;  No AVX required.  Safe to call even when OS does not support XSAVE.
;
;  Serialisation strategy:
;    START  -> CPUID (full retire barrier) + RDTSC
;    END    -> RDTSCP + CPUID (drain ROB)
;    LIGHT  -> LFENCE / MFENCE variants for lower overhead
;
;  Register policy (Windows x64 ABI):
;    Volatile  :  RAX RCX RDX R8 R9 R10 R11 | XMM0-5
;    Preserved :  RBX RSI RDI RBP R12-R15   | XMM6-15
;    Spill rule:  ONLY push RBX when CPUID is required.
;                 All other scratch uses R8/R10/R11 -> zero spills.
;
;  Cache-line alignment:
;    Every exported entry point  -> align 32
;    Hot inner loops             -> align 16
;
;  SIMD instructions present in this file:
;    MOVDQU   - 128-bit unaligned load/store              (SSE2)
;    MOVDQA   - 128-bit aligned   load/store              (SSE2)
;    MOVNTDQ  - 128-bit non-temporal (streaming) store    (SSE2)
;    MOVNTPD  - 128-bit non-temporal store (packed double)(SSE2)
;    PXOR     - 128-bit packed XOR                        (SSE2)
;    PADDQ    - 128-bit packed 64-bit add                 (SSE2)
;    PSHUFD   - 128-bit shuffle doublewords               (SSE2)
;    MOVQ     - 64-bit MMX/XMM move                      (SSE2)
;    PREFETCHT0 / PREFETCHT1 / PREFETCHT2 / PREFETCHNTA
;    PAUSE    - spin-wait hint
;    MFENCE / LFENCE / SFENCE
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
;  GROUP 1 - Core TSC reads (CPUID-serialised)
; =============================================================================

; nc_tsc_start  ->  uint64_t  (CPUID full retire barrier + RDTSC)
align 32
global nc_tsc_start
nc_tsc_start:
    push    rbx
    xor     eax, eax
    cpuid
    rdtsc
    TSC64
    pop     rbx
    ret

; nc_tsc_end  ->  uint64_t  (RDTSCP + CPUID drain)
align 32
global nc_tsc_end
nc_tsc_end:
    push    rbx
    rdtscp
    TSC64
    mov     r10, rax                ; save TSC over CPUID
    xor     eax, eax
    cpuid
    mov     rax, r10
    pop     rbx
    ret

; nc_tsc_raw  ->  uint64_t  (bare RDTSC, no serialisation)
align 32
global nc_tsc_raw
nc_tsc_raw:
    rdtsc
    TSC64
    ret

; nc_tsc_lfence_start  ->  uint64_t  (LFENCE + RDTSC)
align 32
global nc_tsc_lfence_start
nc_tsc_lfence_start:
    lfence
    rdtsc
    TSC64
    ret

; nc_tsc_lfence_end  ->  uint64_t  (RDTSCP + LFENCE)
align 32
global nc_tsc_lfence_end
nc_tsc_lfence_end:
    rdtscp
    TSC64
    lfence
    ret

; nc_tsc_mfence_start  ->  uint64_t  (MFENCE + RDTSC)
align 32
global nc_tsc_mfence_start
nc_tsc_mfence_start:
    mfence
    rdtsc
    TSC64
    ret

; nc_tsc_mfence_end  ->  uint64_t  (RDTSCP + MFENCE)
align 32
global nc_tsc_mfence_end
nc_tsc_mfence_end:
    rdtscp
    TSC64
    mfence
    ret

; nc_tsc_serialize  ->  void  (CPUID barrier, no timestamp)
align 32
global nc_tsc_serialize
nc_tsc_serialize:
    push    rbx
    xor     eax, eax
    cpuid
    pop     rbx
    ret

; =============================================================================
;  GROUP 2 - RDTSCP auxiliary / pairing
; =============================================================================

; nc_tsc_aux  ->  uint32_t  (IA32_TSC_AUX = logical core/socket ID)
align 32
global nc_tsc_aux
nc_tsc_aux:
    rdtscp
    mov     eax, ecx
    ret

; nc_tsc_rdtscp_pair  ->  void
;   RCX = ptr { uint64_t tsc; uint32_t aux; uint32_t _pad; }
;   R10 = ptr save, R11 = aux  (both volatile -> no spill)
align 32
global nc_tsc_rdtscp_pair
nc_tsc_rdtscp_pair:
    mov     r10, rcx
    rdtscp
    mov     r11d, ecx
    TSC64
    mov     [r10],   rax
    mov     [r10+8], r11d
    ret

; =============================================================================
;  GROUP 3 - Delta / elapsed
; =============================================================================

; nc_tsc_delta  ->  uint64_t  (end-start, 0 on underflow)
align 32
global nc_tsc_delta
nc_tsc_delta:
    mov     rax, rdx
    sub     rax, rcx
    jnc     .ok
    xor     eax, eax
.ok:
    ret

; nc_tsc_elapsed_from  ->  uint64_t  (RDTSCP now - start)
align 32
global nc_tsc_elapsed_from
nc_tsc_elapsed_from:
    mov     r10, rcx
    rdtscp
    TSC64
    lfence
    sub     rax, r10
    ret

; nc_tsc_rdtscp_elapsed  ->  uint64_t
;   RCX=start, RDX=aux_out (uint32_t* or NULL)  ->  RAX = elapsed, *RDX = aux
align 32
global nc_tsc_rdtscp_elapsed
nc_tsc_rdtscp_elapsed:
    mov     r10, rcx
    mov     r9,  rdx
    rdtscp
    mov     r11d, ecx
    TSC64
    lfence
    sub     rax, r10
    test    r9, r9
    jz      .no_aux
    mov     dword [r9], r11d
.no_aux:
    ret

; =============================================================================
;  GROUP 4 - Spin
; =============================================================================

align 32
global nc_tsc_spin_until
nc_tsc_spin_until:
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
;  GROUP 5 - Overhead / invariant / CPUID utilities
; =============================================================================

; nc_tsc_overhead  ->  uint64_t  (best-of-16 LFENCE+RDTSC back-to-back delta)
align 32
global nc_tsc_overhead
nc_tsc_overhead:
    mov     r10, 16
    mov     r11, -1
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
    cmp     rax, r11
    cmovb   r11, rax
    dec     r10
    jnz     .loop
    mov     rax, r11
    ret

; nc_tsc_invariant  ->  int  (CPUID 80000007H EDX bit8)
align 32
global nc_tsc_invariant
nc_tsc_invariant:
    push    rbx
    mov     eax, 0x80000007
    xor     ecx, ecx
    cpuid
    shr     edx, 8
    and     edx, 1
    mov     eax, edx
    pop     rbx
    ret

; nc_tsc_cpu_brand  ->  void  (RCX = 49-byte buffer)
align 32
global nc_tsc_cpu_brand
nc_tsc_cpu_brand:
    push    rbx
    push    r12
    mov     r12, rcx
    mov     eax, 0x80000002
    xor     ecx, ecx
    cpuid
    mov     dword [r12+ 0], eax
    mov     dword [r12+ 4], ebx
    mov     dword [r12+ 8], ecx
    mov     dword [r12+12], edx
    mov     eax, 0x80000003
    xor     ecx, ecx
    cpuid
    mov     dword [r12+16], eax
    mov     dword [r12+20], ebx
    mov     dword [r12+24], ecx
    mov     dword [r12+28], edx
    mov     eax, 0x80000004
    xor     ecx, ecx
    cpuid
    mov     dword [r12+32], eax
    mov     dword [r12+36], ebx
    mov     dword [r12+40], ecx
    mov     dword [r12+44], edx
    mov     byte  [r12+48], 0
    pop     r12
    pop     rbx
    ret

; nc_tsc_max_leaf  ->  uint32_t  (CPUID leaf 0 max standard leaf)
align 32
global nc_tsc_max_leaf
nc_tsc_max_leaf:
    push    rbx
    xor     eax, eax
    xor     ecx, ecx
    cpuid
    pop     rbx
    ret

; =============================================================================
;  GROUP 6 - SSE2 SIMD memory utilities  (128-bit MOVDQU)
; =============================================================================

; nc_simd_zero_buf  ->  void
;   RCX = dst (any alignment), RDX = bytes (truncated to multiple of 16)
;   Zeros memory using 128-bit MOVDQU unaligned stores.
align 32
global nc_simd_zero_buf
nc_simd_zero_buf:
    pxor    xmm0, xmm0
    test    rdx, rdx
    jz      .done
    and     rdx, ~15
    jz      .done
align 16
.loop:
    movdqu  [rcx], xmm0
    add     rcx, 16
    sub     rdx, 16
    jnz     .loop
.done:
    ret

; nc_simd_copy_buf  ->  void
;   RCX = dst, RDX = src, R8 = bytes (truncated to multiple of 16)
;   128-bit MOVDQU unaligned load + store.
align 32
global nc_simd_copy_buf
nc_simd_copy_buf:
    test    r8, r8
    jz      .done
    and     r8, ~15
    jz      .done
align 16
.loop:
    movdqu  xmm0, [rdx]
    movdqu  [rcx], xmm0
    add     rcx, 16
    add     rdx, 16
    sub     r8,  16
    jnz     .loop
.done:
    ret

; nc_simd_xor_checksum  ->  uint64_t
;   RCX = ptr, RDX = bytes (truncated to multiple of 16)
;   XOR checksum using 128-bit PXOR + fold with PSHUFD.
align 32
global nc_simd_xor_checksum
nc_simd_xor_checksum:
    pxor    xmm0, xmm0
    test    rdx, rdx
    jz      .reduce
    and     rdx, ~15
    jz      .reduce
align 16
.loop:
    movdqu  xmm1, [rcx]
    pxor    xmm0, xmm1
    add     rcx, 16
    sub     rdx, 16
    jnz     .loop
.reduce:
    pshufd  xmm1, xmm0, 0x4E       ; swap 64-bit halves
    pxor    xmm0, xmm1
    movq    rax, xmm0
    ret

; nc_simd_sum_u64  ->  uint64_t
;   RCX = uint64_t*, RDX = count  ->  RAX = wrapping sum
;   128-bit PADDQ accumulation via MOVDQU loads.
align 32
global nc_simd_sum_u64
nc_simd_sum_u64:
    pxor    xmm0, xmm0
    test    rdx, rdx
    jz      .reduce
    mov     r10, rdx
    shr     r10, 1
    jz      .tail
align 16
.loop2:
    movdqu  xmm1, [rcx]
    paddq   xmm0, xmm1
    add     rcx, 16
    dec     r10
    jnz     .loop2
.tail:
    and     rdx, 1
    jz      .reduce
    movq    xmm1, [rcx]
    paddq   xmm0, xmm1
.reduce:
    pshufd  xmm1, xmm0, 0x4E
    paddq   xmm0, xmm1
    movq    rax, xmm0
    ret

; nc_simd_nt_zero  ->  void
;   RCX = dst (16-byte ALIGNED), RDX = bytes (multiple of 16)
;   Non-temporal stores via MOVNTDQ -> bypasses cache (write-combining).
align 32
global nc_simd_nt_zero
nc_simd_nt_zero:
    pxor    xmm0, xmm0
    test    rdx, rdx
    jz      .done
    and     rdx, ~15
    jz      .done
align 16
.loop:
    movntdq  [rcx], xmm0
    add      rcx, 16
    sub      rdx, 16
    jnz      .loop
    sfence
.done:
    ret

; nc_simd_prefetch_loop  ->  void
;   RCX = ptr, RDX = bytes, R8 = stride
;   Issues PREFETCHT0 every R8 bytes.
align 32
global nc_simd_prefetch_loop
nc_simd_prefetch_loop:
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
;  GROUP 7 - Prefetch-enhanced TSC reads (SSE2 / CPUID serialised)
; =============================================================================

; nc_tsc_prefetch_start  ->  uint64_t
;   RCX = optional ptr to prefetch (NULL = skip).
;   PREFETCHT0 [RCX] -> CPUID -> RDTSC.
align 32
global nc_tsc_prefetch_start
nc_tsc_prefetch_start:
    push    rbx
    test    rcx, rcx
    jz      .no_pf
    prefetcht0 [rcx]
.no_pf:
    xor     eax, eax
    cpuid
    rdtsc
    TSC64
    pop     rbx
    ret

; nc_tsc_prefetch_end  ->  uint64_t
;   RDTSCP + CPUID drain, then optional PREFETCHT1 on RCX.
;   R10 = TSC preserve (volatile, no spill).
align 32
global nc_tsc_prefetch_end
nc_tsc_prefetch_end:
    push    rbx
    rdtscp
    TSC64
    mov     r10, rax
    xor     eax, eax
    cpuid
    mov     rax, r10
    pop     rbx
    test    rcx, rcx
    jz      .done
    prefetcht1 [rcx]
.done:
    ret

; nc_tsc_pause_start  ->  uint64_t
;   16 x PAUSE + CPUID + RDTSC.
align 32
global nc_tsc_pause_start
nc_tsc_pause_start:
    push    rbx
    times 16 pause
    xor     eax, eax
    cpuid
    rdtsc
    TSC64
    pop     rbx
    ret

; nc_tsc_full_fence  ->  uint64_t
;   MFENCE + CPUID + LFENCE + RDTSC  (maximum isolation start fence)
align 32
global nc_tsc_full_fence
nc_tsc_full_fence:
    push    rbx
    mfence
    xor     eax, eax
    cpuid
    lfence
    rdtsc
    TSC64
    pop     rbx
    ret

; =============================================================================
;  GROUP 8 - Sequential RDTSCP stamp
; =============================================================================

; nc_tsc_rdtscp_seq  ->  void
;   RCX = ptr { uint64_t tsc; uint32_t aux; uint32_t seq; }
;   Stores tsc+aux from RDTSCP, increments seq.
align 32
global nc_tsc_rdtscp_seq
nc_tsc_rdtscp_seq:
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
;  Backward-compat aliases
; =============================================================================

global tsc_start_legacy
align 16
tsc_start_legacy:
    jmp nc_tsc_start

global tsc_end_legacy
align 16
tsc_end_legacy:
    jmp nc_tsc_end
