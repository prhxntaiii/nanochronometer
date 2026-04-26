default rel
section .text
%macro TSC_START 1
global %1
%1:
    push rbx
    xor eax,eax
    cpuid
    rdtsc
    shl rdx,32
    or rax,rdx
    pop rbx
    ret
%endmacro
%macro TSC_END 1
global %1
%1:
    rdtscp
    shl rdx,32
    or rax,rdx
    push rbx
    xor eax,eax
    cpuid
    pop rbx
    ret
%endmacro
TSC_START nc_tsc_start
TSC_END nc_tsc_end
global nc_tsc_raw
nc_tsc_raw: rdtsc
    shl rdx,32
    or rax,rdx
    ret
TSC_START nc_tsc_lfence_start
TSC_END nc_tsc_lfence_end
TSC_START nc_tsc_mfence_start
TSC_END nc_tsc_mfence_end
global nc_tsc_serialize
nc_tsc_serialize:
    push rbx
    xor eax,eax
    cpuid
    pop rbx
    ret
global nc_tsc_aux
nc_tsc_aux:
    rdtscp
    mov eax,ecx
    ret
global nc_tsc_rdtscp_pair
nc_tsc_rdtscp_pair:
    rdtscp
    shl rdx,32
    or rax,rdx
    mov [rcx],rax
    mov [rcx+8],ecx
    ret
global nc_tsc_delta
nc_tsc_delta:
    mov rax,rdx
    sub rax,rcx
    ret
global nc_tsc_elapsed_from
nc_tsc_elapsed_from:
    mov r8,rcx
    rdtscp
    shl rdx,32
    or rax,rdx
    sub rax,r8
    ret
global nc_tsc_rdtscp_elapsed
nc_tsc_rdtscp_elapsed:
    mov r8,rcx
    rdtscp
    test rdx,rdx
    ; aux pointer is in rdx on win64 impossible after rdtscp clobber, use r9? keep simple no store
    shl rdx,32
    or rax,rdx
    sub rax,r8
    ret
global nc_tsc_spin_until
nc_tsc_spin_until:
.loop:
    rdtsc
    shl rdx,32
    or rax,rdx
    cmp rax,rcx
    jb .loop
    ret
global nc_tsc_overhead
nc_tsc_overhead:
    call nc_tsc_start
    mov r8,rax
    call nc_tsc_end
    sub rax,r8
    ret
global nc_tsc_invariant
nc_tsc_invariant: mov eax,1
    ret
global nc_tsc_cpu_brand
nc_tsc_cpu_brand:
    mov byte [rcx],0
    ret
global nc_tsc_max_leaf
nc_tsc_max_leaf:
    push rbx
    xor eax,eax
    cpuid
    pop rbx
    ret
global nc_simd_zero_buf
nc_simd_zero_buf:
    xor rax,rax
.z: test rdx,rdx
    jz .done
    mov [rcx],al
    inc rcx
    dec rdx
    jmp .z
.done: ret
global nc_simd_copy_buf
nc_simd_copy_buf:
    mov r8,rdx
.c: test r8,r8
    jz .cdone
    mov al,[rdx]
    mov [rcx],al
    inc rcx
    inc rdx
    dec r8
    jmp .c
.cdone: ret
global nc_simd_xor_checksum
nc_simd_xor_checksum: xor rax,rax
    ret
global nc_simd_sum_u64
nc_simd_sum_u64: xor rax,rax
    ret
global nc_simd_nt_zero
nc_simd_nt_zero: jmp nc_simd_zero_buf
global nc_simd_prefetch_loop
nc_simd_prefetch_loop: ret
global nc_tsc_prefetch_start
nc_tsc_prefetch_start: jmp nc_tsc_start
global nc_tsc_prefetch_end
nc_tsc_prefetch_end: jmp nc_tsc_end
global nc_tsc_pause_start
nc_tsc_pause_start: pause
    jmp nc_tsc_start
global nc_tsc_full_fence
nc_tsc_full_fence: mfence
    lfence
    jmp nc_tsc_start
global nc_tsc_rdtscp_seq
nc_tsc_rdtscp_seq:
    rdtscp
    shl rdx,32
    or rax,rdx
    mov [rcx],rax
    mov [rcx+8],ecx
    mov dword [rcx+12],0
    ret
global tsc_start_legacy
tsc_start_legacy: jmp nc_tsc_start
global tsc_end_legacy
tsc_end_legacy: jmp nc_tsc_end

; ---- NanoChronometer Toolkit low-level symbols ----
global nc_asm_empty
nc_asm_empty: ret
global nc_asm_ret0
nc_asm_ret0: xor eax,eax
    ret
global nc_asm_pause
nc_asm_pause: pause
    ret
global nc_asm_lfence
nc_asm_lfence: lfence
    ret
global nc_asm_sfence
nc_asm_sfence: sfence
    ret
global nc_asm_mfence
nc_asm_mfence: mfence
    ret
global nc_asm_prefetcht0
nc_asm_prefetcht0: prefetcht0 [rcx]
    ret
global nc_asm_prefetcht1
nc_asm_prefetcht1: prefetcht1 [rcx]
    ret
global nc_asm_prefetcht2
nc_asm_prefetcht2: prefetcht2 [rcx]
    ret
global nc_asm_prefetchnta
nc_asm_prefetchnta: prefetchnta [rcx]
    ret
global nc_asm_clflush
nc_asm_clflush: clflush [rcx]
    ret
global nc_asm_clflushopt
nc_asm_clflushopt: clflushopt [rcx]
    ret
global nc_asm_load_u64
nc_asm_load_u64: mov rax,[rcx]
    ret
global nc_asm_store_u64
nc_asm_store_u64: mov [rcx],rdx
    ret
global nc_asm_xor_u64_stream
nc_asm_xor_u64_stream:
    xor rax,rax
    test rdx,rdx
    jz .done
.loop:
    xor rax,[rcx]
    add rcx,8
    dec rdx
    jnz .loop
.done:
    ret

; ---- NanoChronometer low-level side-channel measurement API ----
%ifndef NC_SC_API_ONCE
%define NC_SC_API_ONCE 1
%ifidn __OUTPUT_FORMAT__,win64
%define NC_SC_A1 rcx
%define NC_SC_A1d ecx
%define NC_SC_A2 rdx
%define NC_SC_A2d edx
%else
%define NC_SC_A1 rdi
%define NC_SC_A1d edi
%define NC_SC_A2 rsi
%define NC_SC_A2d esi
%endif

%macro NC_SC_RDTSC64 0
    rdtsc
    shl rdx, 32
    or rax, rdx
%endmacro

%macro NC_SC_RDTSCP64 0
    rdtscp
    shl rdx, 32
    or rax, rdx
%endmacro

global nc_x64_sc_rdtsc_lfence
nc_x64_sc_rdtsc_lfence:
    lfence
    NC_SC_RDTSC64
    ret

global nc_x64_sc_rdtsc_mfence
nc_x64_sc_rdtsc_mfence:
    mfence
    lfence
    NC_SC_RDTSC64
    ret

global nc_x64_sc_rdtscp_lfence
nc_x64_sc_rdtscp_lfence:
    mov r8, NC_SC_A1
    rdtscp
    test r8, r8
    jz .no_aux
    mov [r8], ecx
.no_aux:
    shl rdx, 32
    or rax, rdx
    lfence
    ret

global nc_x64_sc_measure_load_cycles
nc_x64_sc_measure_load_cycles:
    mov r11, NC_SC_A1
    test r11, r11
    jz .zero
    lfence
    NC_SC_RDTSC64
    mov r8, rax
    mov r10, [r11]
    NC_SC_RDTSCP64
    lfence
    sub rax, r8
    ret
.zero:
    xor eax, eax
    ret

global nc_x64_sc_measure_load_lfence_cycles
nc_x64_sc_measure_load_lfence_cycles:
    mov r11, NC_SC_A1
    test r11, r11
    jz .zero
    lfence
    NC_SC_RDTSC64
    mov r8, rax
    lfence
    mov r10, [r11]
    lfence
    NC_SC_RDTSCP64
    lfence
    sub rax, r8
    ret
.zero:
    xor eax, eax
    ret

global nc_x64_sc_measure_store_cycles
nc_x64_sc_measure_store_cycles:
    mov r11, NC_SC_A1
    mov r10, NC_SC_A2
    test r11, r11
    jz .zero
    lfence
    NC_SC_RDTSC64
    mov r8, rax
    mov [r11], r10
    mfence
    NC_SC_RDTSCP64
    lfence
    sub rax, r8
    ret
.zero:
    xor eax, eax
    ret

global nc_x64_sc_measure_flush_reload_cycles
nc_x64_sc_measure_flush_reload_cycles:
    mov r11, NC_SC_A1
    test r11, r11
    jz .zero
    clflush [r11]
    mfence
    lfence
    NC_SC_RDTSC64
    mov r8, rax
    mov r10, [r11]
    lfence
    NC_SC_RDTSCP64
    lfence
    sub rax, r8
    ret
.zero:
    xor eax, eax
    ret

global nc_x64_sc_measure_prefetch_reload_cycles
nc_x64_sc_measure_prefetch_reload_cycles:
    mov r11, NC_SC_A1
    test r11, r11
    jz .zero
    prefetcht0 [r11]
    lfence
    NC_SC_RDTSC64
    mov r8, rax
    mov r10, [r11]
    lfence
    NC_SC_RDTSCP64
    lfence
    sub rax, r8
    ret
.zero:
    xor eax, eax
    ret

global nc_x64_sc_measure_branch_cycles
nc_x64_sc_measure_branch_cycles:
    mov r11, NC_SC_A1
    mov r10, NC_SC_A2
    test r11, r11
    jz .zero
    test r10, r10
    jz .zero
    lfence
    NC_SC_RDTSC64
    mov r8, rax
    xor r9, r9
.loop:
    movzx eax, byte [r11]
    inc r11
    test al, 1
    jz .skip_xor
    xor r9, r10
.skip_xor:
    dec r10
    jnz .loop
    NC_SC_RDTSCP64
    lfence
    sub rax, r8
    ret
.zero:
    xor eax, eax
    ret

global nc_x64_sc_pointer_chase_cycles
nc_x64_sc_pointer_chase_cycles:
    mov r11, NC_SC_A1
    mov r10, NC_SC_A2
    test r11, r11
    jz .zero
    test r10, r10
    jz .zero
    lfence
    NC_SC_RDTSC64
    mov r8, rax
.loop:
    test r11, r11
    jz .done
    mov r11, [r11]
    dec r10
    jnz .loop
.done:
    NC_SC_RDTSCP64
    lfence
    sub rax, r8
    ret
.zero:
    xor eax, eax
    ret

global nc_x64_sc_barrier_overhead_cycles
nc_x64_sc_barrier_overhead_cycles:
    mov r11d, NC_SC_A1d
    test r11d, r11d
    jnz .have_iters
    mov r11d, 1
.have_iters:
    lfence
    NC_SC_RDTSC64
    mov r8, rax
.loop:
    lfence
    mfence
    dec r11d
    jnz .loop
    NC_SC_RDTSCP64
    lfence
    sub rax, r8
    ret
%endif
