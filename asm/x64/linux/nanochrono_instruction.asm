default rel
section .text

; NanoChronometer x64 instruction-family microbench kernels.
; NASM syntax. x64 uses .asm for both Linux (elf64) and Windows (win64).
; These symbols are called only after the C runtime dispatcher checks CPUID.

%macro ARG_MAP 0
%ifidn __OUTPUT_FORMAT__,win64
%define A1  rcx
%define A1d ecx
%define A2  rdx
%define A2d edx
%define A3  r8
%define A3d r8d
%define A4  r9
%define A4d r9d
%else
%define A1  rdi
%define A1d edi
%define A2  rsi
%define A2d esi
%define A3  rdx
%define A3d edx
%define A4  rcx
%define A4d ecx
%endif
%endmacro
ARG_MAP

; uint64_t nc_asm_aesenc_128_loop(uint32_t blocks)
global nc_asm_aesenc_128_loop
nc_asm_aesenc_128_loop:
    mov ecx, A1d
    test ecx, ecx
    jz .aesenc_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.aesenc_loop:
    aesenc xmm0, xmm1
    dec ecx
    jnz .aesenc_loop
.aesenc_done:
    movq rax, xmm0
    ret

; uint64_t nc_asm_aesdec_128_loop(uint32_t blocks)
global nc_asm_aesdec_128_loop
nc_asm_aesdec_128_loop:
    mov ecx, A1d
    test ecx, ecx
    jz .aesdec_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.aesdec_loop:
    aesdec xmm0, xmm1
    dec ecx
    jnz .aesdec_loop
.aesdec_done:
    movq rax, xmm0
    ret

; uint64_t nc_asm_pclmul_loop(uint32_t blocks)
global nc_asm_pclmul_loop
nc_asm_pclmul_loop:
    mov ecx, A1d
    test ecx, ecx
    jz .pclmul_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.pclmul_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .pclmul_loop
.pclmul_done:
    movq rax, xmm0
    ret

; uint64_t nc_asm_sha256rnds2_loop(uint32_t blocks)
; NASM uses the two-operand SHA syntax. XMM0 is the implicit third operand.
global nc_asm_sha256rnds2_loop
nc_asm_sha256rnds2_loop:
    mov ecx, A1d
    test ecx, ecx
    jz .sha_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.sha_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .sha_loop
.sha_done:
    movq rax, xmm1
    ret

; uint64_t nc_asm_crc32_loop(const void *ptr, size_t bytes)
global nc_asm_crc32_loop
nc_asm_crc32_loop:
    mov r8, A1
    mov r9, A2
    xor eax, eax
    test r8, r8
    jz .crc_done
.crc_loop:
    test r9, r9
    jz .crc_done
    crc32 rax, byte [r8]
    inc r8
    dec r9
    jnz .crc_loop
.crc_done:
    ret

; uint64_t nc_asm_vector_add_u64_loop(const void *a, const void *b, void *out_buf, size_t count)
; count is number of uint64_t elements, not bytes.
global nc_asm_vector_add_u64_loop
nc_asm_vector_add_u64_loop:
    mov r10, A1
    mov r11, A2
    mov r8,  A3
    mov r9,  A4
    xor rax, rax
    test r10, r10
    jz .add_done
    test r11, r11
    jz .add_done
    test r8, r8
    jz .add_done
.add_vec_loop:
    cmp r9, 2
    jb .add_tail
    movdqu xmm0, [r10]
    movdqu xmm1, [r11]
    paddq xmm0, xmm1
    movdqu [r8], xmm0
    movq rcx, xmm0
    xor rax, rcx
    psrldq xmm0, 8
    movq rcx, xmm0
    xor rax, rcx
    add r10, 16
    add r11, 16
    add r8, 16
    sub r9, 2
    jmp .add_vec_loop
.add_tail:
    test r9, r9
    jz .add_done
    mov rcx, [r10]
    add rcx, [r11]
    mov [r8], rcx
    xor rax, rcx
.add_done:
    ret

; uint64_t nc_asm_vector_xor_loop(const void *a, const void *b, void *out_buf, size_t bytes)
global nc_asm_vector_xor_loop
nc_asm_vector_xor_loop:
    mov r10, A1
    mov r11, A2
    mov r8,  A3
    mov r9,  A4
    xor rax, rax
    test r10, r10
    jz .xor_done
    test r11, r11
    jz .xor_done
    test r8, r8
    jz .xor_done
.xor_vec_loop:
    cmp r9, 16
    jb .xor_tail
    movdqu xmm0, [r10]
    movdqu xmm1, [r11]
    pxor xmm0, xmm1
    movdqu [r8], xmm0
    movq rcx, xmm0
    xor rax, rcx
    psrldq xmm0, 8
    movq rcx, xmm0
    xor rax, rcx
    add r10, 16
    add r11, 16
    add r8, 16
    sub r9, 16
    jmp .xor_vec_loop
.xor_tail:
    test r9, r9
    jz .xor_done
    mov cl, [r10]
    xor cl, [r11]
    mov [r8], cl
    movzx rcx, cl
    xor rax, rcx
    inc r10
    inc r11
    inc r8
    dec r9
    jmp .xor_tail
.xor_done:
    ret

; uint64_t nc_asm_branch_probe_loop(const uint8_t *pattern, size_t count)
global nc_asm_branch_probe_loop
nc_asm_branch_probe_loop:
    mov r8, A1
    mov r9, A2
    xor rax, rax
    test r8, r8
    jz .br_done
.br_loop:
    test r9, r9
    jz .br_done
    cmp byte [r8], 0
    je .br_zero
    add rax, 3
    jmp .br_next
.br_zero:
    add rax, 1
.br_next:
    inc r8
    dec r9
    jnz .br_loop
.br_done:
    ret
