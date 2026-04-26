default rel
section .text
%macro ARG_MAP 0
%ifidn __OUTPUT_FORMAT__,win64
%define A1 rcx
%define A1d ecx
%define A2 rdx
%define A3 r8
%else
%define A1 rdi
%define A1d edi
%define A2 rsi
%define A3 rdx
%endif
%endmacro
ARG_MAP

global nc_x64_mb_sisd_000
nc_x64_mb_sisd_000:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_000_done
.L_nc_x64_mb_sisd_000_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 3
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_000_loop
.L_nc_x64_mb_sisd_000_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_001
nc_x64_mb_sisd_001:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_001_done
.L_nc_x64_mb_sisd_001_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 4
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_001_loop
.L_nc_x64_mb_sisd_001_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_002
nc_x64_mb_sisd_002:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_002_done
.L_nc_x64_mb_sisd_002_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 5
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_002_loop
.L_nc_x64_mb_sisd_002_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_003
nc_x64_mb_sisd_003:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_003_done
.L_nc_x64_mb_sisd_003_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 6
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_003_loop
.L_nc_x64_mb_sisd_003_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_004
nc_x64_mb_sisd_004:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_004_done
.L_nc_x64_mb_sisd_004_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 7
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_004_loop
.L_nc_x64_mb_sisd_004_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_005
nc_x64_mb_sisd_005:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_005_done
.L_nc_x64_mb_sisd_005_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 8
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_005_loop
.L_nc_x64_mb_sisd_005_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_006
nc_x64_mb_sisd_006:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_006_done
.L_nc_x64_mb_sisd_006_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 9
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_006_loop
.L_nc_x64_mb_sisd_006_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_007
nc_x64_mb_sisd_007:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_007_done
.L_nc_x64_mb_sisd_007_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 10
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_007_loop
.L_nc_x64_mb_sisd_007_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_008
nc_x64_mb_sisd_008:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_008_done
.L_nc_x64_mb_sisd_008_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 11
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_008_loop
.L_nc_x64_mb_sisd_008_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_009
nc_x64_mb_sisd_009:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_009_done
.L_nc_x64_mb_sisd_009_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 12
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_009_loop
.L_nc_x64_mb_sisd_009_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_010
nc_x64_mb_sisd_010:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_010_done
.L_nc_x64_mb_sisd_010_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 13
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_010_loop
.L_nc_x64_mb_sisd_010_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_011
nc_x64_mb_sisd_011:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_011_done
.L_nc_x64_mb_sisd_011_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 14
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_011_loop
.L_nc_x64_mb_sisd_011_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_012
nc_x64_mb_sisd_012:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_012_done
.L_nc_x64_mb_sisd_012_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 15
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_012_loop
.L_nc_x64_mb_sisd_012_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_013
nc_x64_mb_sisd_013:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_013_done
.L_nc_x64_mb_sisd_013_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 16
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_013_loop
.L_nc_x64_mb_sisd_013_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_014
nc_x64_mb_sisd_014:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_014_done
.L_nc_x64_mb_sisd_014_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 17
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_014_loop
.L_nc_x64_mb_sisd_014_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_015
nc_x64_mb_sisd_015:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_015_done
.L_nc_x64_mb_sisd_015_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 18
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_015_loop
.L_nc_x64_mb_sisd_015_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_016
nc_x64_mb_sisd_016:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_016_done
.L_nc_x64_mb_sisd_016_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 19
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_016_loop
.L_nc_x64_mb_sisd_016_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_017
nc_x64_mb_sisd_017:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_017_done
.L_nc_x64_mb_sisd_017_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 20
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_017_loop
.L_nc_x64_mb_sisd_017_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_018
nc_x64_mb_sisd_018:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_018_done
.L_nc_x64_mb_sisd_018_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 21
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_018_loop
.L_nc_x64_mb_sisd_018_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_019
nc_x64_mb_sisd_019:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_019_done
.L_nc_x64_mb_sisd_019_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 22
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_019_loop
.L_nc_x64_mb_sisd_019_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_020
nc_x64_mb_sisd_020:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_020_done
.L_nc_x64_mb_sisd_020_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 23
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_020_loop
.L_nc_x64_mb_sisd_020_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_021
nc_x64_mb_sisd_021:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_021_done
.L_nc_x64_mb_sisd_021_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 24
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_021_loop
.L_nc_x64_mb_sisd_021_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_022
nc_x64_mb_sisd_022:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_022_done
.L_nc_x64_mb_sisd_022_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 25
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_022_loop
.L_nc_x64_mb_sisd_022_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_023
nc_x64_mb_sisd_023:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_023_done
.L_nc_x64_mb_sisd_023_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 26
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_023_loop
.L_nc_x64_mb_sisd_023_done:
    mov rax, r8
    ret

global nc_x64_mb_sisd_024
nc_x64_mb_sisd_024:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sisd_024_done
.L_nc_x64_mb_sisd_024_loop:
    mov rax, r8
    add rax, rcx
    imul rax, rax, 27
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_sisd_024_loop
.L_nc_x64_mb_sisd_024_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_000
nc_x64_mb_rdtsc_000:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_000_done
.L_nc_x64_mb_rdtsc_000_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_000_loop
.L_nc_x64_mb_rdtsc_000_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_001
nc_x64_mb_rdtsc_001:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_001_done
.L_nc_x64_mb_rdtsc_001_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_001_loop
.L_nc_x64_mb_rdtsc_001_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_002
nc_x64_mb_rdtsc_002:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_002_done
.L_nc_x64_mb_rdtsc_002_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_002_loop
.L_nc_x64_mb_rdtsc_002_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_003
nc_x64_mb_rdtsc_003:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_003_done
.L_nc_x64_mb_rdtsc_003_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_003_loop
.L_nc_x64_mb_rdtsc_003_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_004
nc_x64_mb_rdtsc_004:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_004_done
.L_nc_x64_mb_rdtsc_004_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_004_loop
.L_nc_x64_mb_rdtsc_004_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_005
nc_x64_mb_rdtsc_005:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_005_done
.L_nc_x64_mb_rdtsc_005_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_005_loop
.L_nc_x64_mb_rdtsc_005_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_006
nc_x64_mb_rdtsc_006:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_006_done
.L_nc_x64_mb_rdtsc_006_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_006_loop
.L_nc_x64_mb_rdtsc_006_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_007
nc_x64_mb_rdtsc_007:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_007_done
.L_nc_x64_mb_rdtsc_007_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_007_loop
.L_nc_x64_mb_rdtsc_007_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_008
nc_x64_mb_rdtsc_008:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_008_done
.L_nc_x64_mb_rdtsc_008_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_008_loop
.L_nc_x64_mb_rdtsc_008_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_009
nc_x64_mb_rdtsc_009:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_009_done
.L_nc_x64_mb_rdtsc_009_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_009_loop
.L_nc_x64_mb_rdtsc_009_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_010
nc_x64_mb_rdtsc_010:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_010_done
.L_nc_x64_mb_rdtsc_010_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_010_loop
.L_nc_x64_mb_rdtsc_010_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_011
nc_x64_mb_rdtsc_011:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_011_done
.L_nc_x64_mb_rdtsc_011_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_011_loop
.L_nc_x64_mb_rdtsc_011_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_012
nc_x64_mb_rdtsc_012:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_012_done
.L_nc_x64_mb_rdtsc_012_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_012_loop
.L_nc_x64_mb_rdtsc_012_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_013
nc_x64_mb_rdtsc_013:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_013_done
.L_nc_x64_mb_rdtsc_013_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_013_loop
.L_nc_x64_mb_rdtsc_013_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_014
nc_x64_mb_rdtsc_014:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_014_done
.L_nc_x64_mb_rdtsc_014_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_014_loop
.L_nc_x64_mb_rdtsc_014_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_015
nc_x64_mb_rdtsc_015:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_015_done
.L_nc_x64_mb_rdtsc_015_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_015_loop
.L_nc_x64_mb_rdtsc_015_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_016
nc_x64_mb_rdtsc_016:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_016_done
.L_nc_x64_mb_rdtsc_016_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_016_loop
.L_nc_x64_mb_rdtsc_016_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_017
nc_x64_mb_rdtsc_017:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_017_done
.L_nc_x64_mb_rdtsc_017_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_017_loop
.L_nc_x64_mb_rdtsc_017_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_018
nc_x64_mb_rdtsc_018:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_018_done
.L_nc_x64_mb_rdtsc_018_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_018_loop
.L_nc_x64_mb_rdtsc_018_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_019
nc_x64_mb_rdtsc_019:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_019_done
.L_nc_x64_mb_rdtsc_019_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_019_loop
.L_nc_x64_mb_rdtsc_019_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_020
nc_x64_mb_rdtsc_020:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_020_done
.L_nc_x64_mb_rdtsc_020_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_020_loop
.L_nc_x64_mb_rdtsc_020_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_021
nc_x64_mb_rdtsc_021:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_021_done
.L_nc_x64_mb_rdtsc_021_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_021_loop
.L_nc_x64_mb_rdtsc_021_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_022
nc_x64_mb_rdtsc_022:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_022_done
.L_nc_x64_mb_rdtsc_022_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_022_loop
.L_nc_x64_mb_rdtsc_022_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_023
nc_x64_mb_rdtsc_023:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_023_done
.L_nc_x64_mb_rdtsc_023_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_023_loop
.L_nc_x64_mb_rdtsc_023_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtsc_024
nc_x64_mb_rdtsc_024:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtsc_024_done
.L_nc_x64_mb_rdtsc_024_loop:
    lfence
    rdtsc
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtsc_024_loop
.L_nc_x64_mb_rdtsc_024_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_000
nc_x64_mb_rdtscp_000:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_000_done
.L_nc_x64_mb_rdtscp_000_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_000_loop
.L_nc_x64_mb_rdtscp_000_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_001
nc_x64_mb_rdtscp_001:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_001_done
.L_nc_x64_mb_rdtscp_001_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_001_loop
.L_nc_x64_mb_rdtscp_001_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_002
nc_x64_mb_rdtscp_002:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_002_done
.L_nc_x64_mb_rdtscp_002_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_002_loop
.L_nc_x64_mb_rdtscp_002_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_003
nc_x64_mb_rdtscp_003:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_003_done
.L_nc_x64_mb_rdtscp_003_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_003_loop
.L_nc_x64_mb_rdtscp_003_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_004
nc_x64_mb_rdtscp_004:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_004_done
.L_nc_x64_mb_rdtscp_004_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_004_loop
.L_nc_x64_mb_rdtscp_004_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_005
nc_x64_mb_rdtscp_005:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_005_done
.L_nc_x64_mb_rdtscp_005_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_005_loop
.L_nc_x64_mb_rdtscp_005_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_006
nc_x64_mb_rdtscp_006:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_006_done
.L_nc_x64_mb_rdtscp_006_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_006_loop
.L_nc_x64_mb_rdtscp_006_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_007
nc_x64_mb_rdtscp_007:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_007_done
.L_nc_x64_mb_rdtscp_007_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_007_loop
.L_nc_x64_mb_rdtscp_007_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_008
nc_x64_mb_rdtscp_008:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_008_done
.L_nc_x64_mb_rdtscp_008_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_008_loop
.L_nc_x64_mb_rdtscp_008_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_009
nc_x64_mb_rdtscp_009:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_009_done
.L_nc_x64_mb_rdtscp_009_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_009_loop
.L_nc_x64_mb_rdtscp_009_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_010
nc_x64_mb_rdtscp_010:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_010_done
.L_nc_x64_mb_rdtscp_010_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_010_loop
.L_nc_x64_mb_rdtscp_010_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_011
nc_x64_mb_rdtscp_011:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_011_done
.L_nc_x64_mb_rdtscp_011_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_011_loop
.L_nc_x64_mb_rdtscp_011_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_012
nc_x64_mb_rdtscp_012:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_012_done
.L_nc_x64_mb_rdtscp_012_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_012_loop
.L_nc_x64_mb_rdtscp_012_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_013
nc_x64_mb_rdtscp_013:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_013_done
.L_nc_x64_mb_rdtscp_013_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_013_loop
.L_nc_x64_mb_rdtscp_013_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_014
nc_x64_mb_rdtscp_014:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_014_done
.L_nc_x64_mb_rdtscp_014_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_014_loop
.L_nc_x64_mb_rdtscp_014_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_015
nc_x64_mb_rdtscp_015:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_015_done
.L_nc_x64_mb_rdtscp_015_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_015_loop
.L_nc_x64_mb_rdtscp_015_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_016
nc_x64_mb_rdtscp_016:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_016_done
.L_nc_x64_mb_rdtscp_016_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_016_loop
.L_nc_x64_mb_rdtscp_016_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_017
nc_x64_mb_rdtscp_017:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_017_done
.L_nc_x64_mb_rdtscp_017_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_017_loop
.L_nc_x64_mb_rdtscp_017_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_018
nc_x64_mb_rdtscp_018:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_018_done
.L_nc_x64_mb_rdtscp_018_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_018_loop
.L_nc_x64_mb_rdtscp_018_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_019
nc_x64_mb_rdtscp_019:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_019_done
.L_nc_x64_mb_rdtscp_019_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_019_loop
.L_nc_x64_mb_rdtscp_019_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_020
nc_x64_mb_rdtscp_020:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_020_done
.L_nc_x64_mb_rdtscp_020_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_020_loop
.L_nc_x64_mb_rdtscp_020_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_021
nc_x64_mb_rdtscp_021:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_021_done
.L_nc_x64_mb_rdtscp_021_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_021_loop
.L_nc_x64_mb_rdtscp_021_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_022
nc_x64_mb_rdtscp_022:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_022_done
.L_nc_x64_mb_rdtscp_022_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_022_loop
.L_nc_x64_mb_rdtscp_022_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_023
nc_x64_mb_rdtscp_023:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_023_done
.L_nc_x64_mb_rdtscp_023_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_023_loop
.L_nc_x64_mb_rdtscp_023_done:
    mov rax, r8
    ret

global nc_x64_mb_rdtscp_024
nc_x64_mb_rdtscp_024:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_rdtscp_024_done
.L_nc_x64_mb_rdtscp_024_loop:
    rdtscp
    lfence
    shl rdx, 32
    or rax, rdx
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_rdtscp_024_loop
.L_nc_x64_mb_rdtscp_024_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_000
nc_x64_mb_lfence_000:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_000_done
.L_nc_x64_mb_lfence_000_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_000_loop
.L_nc_x64_mb_lfence_000_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_001
nc_x64_mb_lfence_001:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_001_done
.L_nc_x64_mb_lfence_001_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_001_loop
.L_nc_x64_mb_lfence_001_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_002
nc_x64_mb_lfence_002:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_002_done
.L_nc_x64_mb_lfence_002_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_002_loop
.L_nc_x64_mb_lfence_002_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_003
nc_x64_mb_lfence_003:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_003_done
.L_nc_x64_mb_lfence_003_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_003_loop
.L_nc_x64_mb_lfence_003_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_004
nc_x64_mb_lfence_004:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_004_done
.L_nc_x64_mb_lfence_004_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_004_loop
.L_nc_x64_mb_lfence_004_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_005
nc_x64_mb_lfence_005:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_005_done
.L_nc_x64_mb_lfence_005_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_005_loop
.L_nc_x64_mb_lfence_005_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_006
nc_x64_mb_lfence_006:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_006_done
.L_nc_x64_mb_lfence_006_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_006_loop
.L_nc_x64_mb_lfence_006_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_007
nc_x64_mb_lfence_007:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_007_done
.L_nc_x64_mb_lfence_007_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_007_loop
.L_nc_x64_mb_lfence_007_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_008
nc_x64_mb_lfence_008:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_008_done
.L_nc_x64_mb_lfence_008_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_008_loop
.L_nc_x64_mb_lfence_008_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_009
nc_x64_mb_lfence_009:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_009_done
.L_nc_x64_mb_lfence_009_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_009_loop
.L_nc_x64_mb_lfence_009_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_010
nc_x64_mb_lfence_010:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_010_done
.L_nc_x64_mb_lfence_010_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_010_loop
.L_nc_x64_mb_lfence_010_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_011
nc_x64_mb_lfence_011:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_011_done
.L_nc_x64_mb_lfence_011_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_011_loop
.L_nc_x64_mb_lfence_011_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_012
nc_x64_mb_lfence_012:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_012_done
.L_nc_x64_mb_lfence_012_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_012_loop
.L_nc_x64_mb_lfence_012_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_013
nc_x64_mb_lfence_013:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_013_done
.L_nc_x64_mb_lfence_013_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_013_loop
.L_nc_x64_mb_lfence_013_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_014
nc_x64_mb_lfence_014:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_014_done
.L_nc_x64_mb_lfence_014_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_014_loop
.L_nc_x64_mb_lfence_014_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_015
nc_x64_mb_lfence_015:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_015_done
.L_nc_x64_mb_lfence_015_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_015_loop
.L_nc_x64_mb_lfence_015_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_016
nc_x64_mb_lfence_016:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_016_done
.L_nc_x64_mb_lfence_016_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_016_loop
.L_nc_x64_mb_lfence_016_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_017
nc_x64_mb_lfence_017:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_017_done
.L_nc_x64_mb_lfence_017_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_017_loop
.L_nc_x64_mb_lfence_017_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_018
nc_x64_mb_lfence_018:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_018_done
.L_nc_x64_mb_lfence_018_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_018_loop
.L_nc_x64_mb_lfence_018_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_019
nc_x64_mb_lfence_019:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_019_done
.L_nc_x64_mb_lfence_019_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_019_loop
.L_nc_x64_mb_lfence_019_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_020
nc_x64_mb_lfence_020:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_020_done
.L_nc_x64_mb_lfence_020_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_020_loop
.L_nc_x64_mb_lfence_020_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_021
nc_x64_mb_lfence_021:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_021_done
.L_nc_x64_mb_lfence_021_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_021_loop
.L_nc_x64_mb_lfence_021_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_022
nc_x64_mb_lfence_022:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_022_done
.L_nc_x64_mb_lfence_022_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_022_loop
.L_nc_x64_mb_lfence_022_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_023
nc_x64_mb_lfence_023:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_023_done
.L_nc_x64_mb_lfence_023_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_023_loop
.L_nc_x64_mb_lfence_023_done:
    mov rax, r8
    ret

global nc_x64_mb_lfence_024
nc_x64_mb_lfence_024:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_lfence_024_done
.L_nc_x64_mb_lfence_024_loop:
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_lfence_024_loop
.L_nc_x64_mb_lfence_024_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_000
nc_x64_mb_mfence_000:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_000_done
.L_nc_x64_mb_mfence_000_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_000_loop
.L_nc_x64_mb_mfence_000_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_001
nc_x64_mb_mfence_001:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_001_done
.L_nc_x64_mb_mfence_001_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_001_loop
.L_nc_x64_mb_mfence_001_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_002
nc_x64_mb_mfence_002:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_002_done
.L_nc_x64_mb_mfence_002_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_002_loop
.L_nc_x64_mb_mfence_002_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_003
nc_x64_mb_mfence_003:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_003_done
.L_nc_x64_mb_mfence_003_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_003_loop
.L_nc_x64_mb_mfence_003_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_004
nc_x64_mb_mfence_004:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_004_done
.L_nc_x64_mb_mfence_004_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_004_loop
.L_nc_x64_mb_mfence_004_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_005
nc_x64_mb_mfence_005:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_005_done
.L_nc_x64_mb_mfence_005_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_005_loop
.L_nc_x64_mb_mfence_005_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_006
nc_x64_mb_mfence_006:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_006_done
.L_nc_x64_mb_mfence_006_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_006_loop
.L_nc_x64_mb_mfence_006_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_007
nc_x64_mb_mfence_007:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_007_done
.L_nc_x64_mb_mfence_007_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_007_loop
.L_nc_x64_mb_mfence_007_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_008
nc_x64_mb_mfence_008:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_008_done
.L_nc_x64_mb_mfence_008_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_008_loop
.L_nc_x64_mb_mfence_008_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_009
nc_x64_mb_mfence_009:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_009_done
.L_nc_x64_mb_mfence_009_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_009_loop
.L_nc_x64_mb_mfence_009_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_010
nc_x64_mb_mfence_010:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_010_done
.L_nc_x64_mb_mfence_010_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_010_loop
.L_nc_x64_mb_mfence_010_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_011
nc_x64_mb_mfence_011:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_011_done
.L_nc_x64_mb_mfence_011_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_011_loop
.L_nc_x64_mb_mfence_011_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_012
nc_x64_mb_mfence_012:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_012_done
.L_nc_x64_mb_mfence_012_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_012_loop
.L_nc_x64_mb_mfence_012_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_013
nc_x64_mb_mfence_013:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_013_done
.L_nc_x64_mb_mfence_013_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_013_loop
.L_nc_x64_mb_mfence_013_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_014
nc_x64_mb_mfence_014:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_014_done
.L_nc_x64_mb_mfence_014_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_014_loop
.L_nc_x64_mb_mfence_014_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_015
nc_x64_mb_mfence_015:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_015_done
.L_nc_x64_mb_mfence_015_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_015_loop
.L_nc_x64_mb_mfence_015_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_016
nc_x64_mb_mfence_016:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_016_done
.L_nc_x64_mb_mfence_016_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_016_loop
.L_nc_x64_mb_mfence_016_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_017
nc_x64_mb_mfence_017:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_017_done
.L_nc_x64_mb_mfence_017_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_017_loop
.L_nc_x64_mb_mfence_017_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_018
nc_x64_mb_mfence_018:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_018_done
.L_nc_x64_mb_mfence_018_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_018_loop
.L_nc_x64_mb_mfence_018_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_019
nc_x64_mb_mfence_019:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_019_done
.L_nc_x64_mb_mfence_019_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_019_loop
.L_nc_x64_mb_mfence_019_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_020
nc_x64_mb_mfence_020:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_020_done
.L_nc_x64_mb_mfence_020_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_020_loop
.L_nc_x64_mb_mfence_020_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_021
nc_x64_mb_mfence_021:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_021_done
.L_nc_x64_mb_mfence_021_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_021_loop
.L_nc_x64_mb_mfence_021_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_022
nc_x64_mb_mfence_022:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_022_done
.L_nc_x64_mb_mfence_022_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_022_loop
.L_nc_x64_mb_mfence_022_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_023
nc_x64_mb_mfence_023:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_023_done
.L_nc_x64_mb_mfence_023_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_023_loop
.L_nc_x64_mb_mfence_023_done:
    mov rax, r8
    ret

global nc_x64_mb_mfence_024
nc_x64_mb_mfence_024:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_mfence_024_done
.L_nc_x64_mb_mfence_024_loop:
    mfence
    lfence
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_mfence_024_loop
.L_nc_x64_mb_mfence_024_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_000
nc_x64_mb_prefetch_000:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_000_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_000_scalar
.L_nc_x64_mb_prefetch_000_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_000_loop
    jmp .L_nc_x64_mb_prefetch_000_done
.L_nc_x64_mb_prefetch_000_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_000_scalar
.L_nc_x64_mb_prefetch_000_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_001
nc_x64_mb_prefetch_001:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_001_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_001_scalar
.L_nc_x64_mb_prefetch_001_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_001_loop
    jmp .L_nc_x64_mb_prefetch_001_done
.L_nc_x64_mb_prefetch_001_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_001_scalar
.L_nc_x64_mb_prefetch_001_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_002
nc_x64_mb_prefetch_002:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_002_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_002_scalar
.L_nc_x64_mb_prefetch_002_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_002_loop
    jmp .L_nc_x64_mb_prefetch_002_done
.L_nc_x64_mb_prefetch_002_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_002_scalar
.L_nc_x64_mb_prefetch_002_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_003
nc_x64_mb_prefetch_003:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_003_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_003_scalar
.L_nc_x64_mb_prefetch_003_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_003_loop
    jmp .L_nc_x64_mb_prefetch_003_done
.L_nc_x64_mb_prefetch_003_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_003_scalar
.L_nc_x64_mb_prefetch_003_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_004
nc_x64_mb_prefetch_004:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_004_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_004_scalar
.L_nc_x64_mb_prefetch_004_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_004_loop
    jmp .L_nc_x64_mb_prefetch_004_done
.L_nc_x64_mb_prefetch_004_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_004_scalar
.L_nc_x64_mb_prefetch_004_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_005
nc_x64_mb_prefetch_005:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_005_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_005_scalar
.L_nc_x64_mb_prefetch_005_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_005_loop
    jmp .L_nc_x64_mb_prefetch_005_done
.L_nc_x64_mb_prefetch_005_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_005_scalar
.L_nc_x64_mb_prefetch_005_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_006
nc_x64_mb_prefetch_006:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_006_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_006_scalar
.L_nc_x64_mb_prefetch_006_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_006_loop
    jmp .L_nc_x64_mb_prefetch_006_done
.L_nc_x64_mb_prefetch_006_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_006_scalar
.L_nc_x64_mb_prefetch_006_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_007
nc_x64_mb_prefetch_007:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_007_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_007_scalar
.L_nc_x64_mb_prefetch_007_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_007_loop
    jmp .L_nc_x64_mb_prefetch_007_done
.L_nc_x64_mb_prefetch_007_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_007_scalar
.L_nc_x64_mb_prefetch_007_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_008
nc_x64_mb_prefetch_008:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_008_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_008_scalar
.L_nc_x64_mb_prefetch_008_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_008_loop
    jmp .L_nc_x64_mb_prefetch_008_done
.L_nc_x64_mb_prefetch_008_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_008_scalar
.L_nc_x64_mb_prefetch_008_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_009
nc_x64_mb_prefetch_009:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_009_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_009_scalar
.L_nc_x64_mb_prefetch_009_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_009_loop
    jmp .L_nc_x64_mb_prefetch_009_done
.L_nc_x64_mb_prefetch_009_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_009_scalar
.L_nc_x64_mb_prefetch_009_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_010
nc_x64_mb_prefetch_010:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_010_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_010_scalar
.L_nc_x64_mb_prefetch_010_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_010_loop
    jmp .L_nc_x64_mb_prefetch_010_done
.L_nc_x64_mb_prefetch_010_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_010_scalar
.L_nc_x64_mb_prefetch_010_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_011
nc_x64_mb_prefetch_011:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_011_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_011_scalar
.L_nc_x64_mb_prefetch_011_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_011_loop
    jmp .L_nc_x64_mb_prefetch_011_done
.L_nc_x64_mb_prefetch_011_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_011_scalar
.L_nc_x64_mb_prefetch_011_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_012
nc_x64_mb_prefetch_012:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_012_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_012_scalar
.L_nc_x64_mb_prefetch_012_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_012_loop
    jmp .L_nc_x64_mb_prefetch_012_done
.L_nc_x64_mb_prefetch_012_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_012_scalar
.L_nc_x64_mb_prefetch_012_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_013
nc_x64_mb_prefetch_013:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_013_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_013_scalar
.L_nc_x64_mb_prefetch_013_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_013_loop
    jmp .L_nc_x64_mb_prefetch_013_done
.L_nc_x64_mb_prefetch_013_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_013_scalar
.L_nc_x64_mb_prefetch_013_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_014
nc_x64_mb_prefetch_014:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_014_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_014_scalar
.L_nc_x64_mb_prefetch_014_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_014_loop
    jmp .L_nc_x64_mb_prefetch_014_done
.L_nc_x64_mb_prefetch_014_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_014_scalar
.L_nc_x64_mb_prefetch_014_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_015
nc_x64_mb_prefetch_015:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_015_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_015_scalar
.L_nc_x64_mb_prefetch_015_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_015_loop
    jmp .L_nc_x64_mb_prefetch_015_done
.L_nc_x64_mb_prefetch_015_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_015_scalar
.L_nc_x64_mb_prefetch_015_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_016
nc_x64_mb_prefetch_016:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_016_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_016_scalar
.L_nc_x64_mb_prefetch_016_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_016_loop
    jmp .L_nc_x64_mb_prefetch_016_done
.L_nc_x64_mb_prefetch_016_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_016_scalar
.L_nc_x64_mb_prefetch_016_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_017
nc_x64_mb_prefetch_017:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_017_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_017_scalar
.L_nc_x64_mb_prefetch_017_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_017_loop
    jmp .L_nc_x64_mb_prefetch_017_done
.L_nc_x64_mb_prefetch_017_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_017_scalar
.L_nc_x64_mb_prefetch_017_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_018
nc_x64_mb_prefetch_018:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_018_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_018_scalar
.L_nc_x64_mb_prefetch_018_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_018_loop
    jmp .L_nc_x64_mb_prefetch_018_done
.L_nc_x64_mb_prefetch_018_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_018_scalar
.L_nc_x64_mb_prefetch_018_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_019
nc_x64_mb_prefetch_019:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_019_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_019_scalar
.L_nc_x64_mb_prefetch_019_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_019_loop
    jmp .L_nc_x64_mb_prefetch_019_done
.L_nc_x64_mb_prefetch_019_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_019_scalar
.L_nc_x64_mb_prefetch_019_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_020
nc_x64_mb_prefetch_020:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_020_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_020_scalar
.L_nc_x64_mb_prefetch_020_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_020_loop
    jmp .L_nc_x64_mb_prefetch_020_done
.L_nc_x64_mb_prefetch_020_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_020_scalar
.L_nc_x64_mb_prefetch_020_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_021
nc_x64_mb_prefetch_021:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_021_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_021_scalar
.L_nc_x64_mb_prefetch_021_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_021_loop
    jmp .L_nc_x64_mb_prefetch_021_done
.L_nc_x64_mb_prefetch_021_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_021_scalar
.L_nc_x64_mb_prefetch_021_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_022
nc_x64_mb_prefetch_022:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_022_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_022_scalar
.L_nc_x64_mb_prefetch_022_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_022_loop
    jmp .L_nc_x64_mb_prefetch_022_done
.L_nc_x64_mb_prefetch_022_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_022_scalar
.L_nc_x64_mb_prefetch_022_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_023
nc_x64_mb_prefetch_023:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_023_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_023_scalar
.L_nc_x64_mb_prefetch_023_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_023_loop
    jmp .L_nc_x64_mb_prefetch_023_done
.L_nc_x64_mb_prefetch_023_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_023_scalar
.L_nc_x64_mb_prefetch_023_done:
    mov rax, r8
    ret

global nc_x64_mb_prefetch_024
nc_x64_mb_prefetch_024:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_prefetch_024_done
    test r10, r10
    jz .L_nc_x64_mb_prefetch_024_scalar
.L_nc_x64_mb_prefetch_024_loop:
    prefetcht0 [r10]
    prefetchnta [r10]
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_024_loop
    jmp .L_nc_x64_mb_prefetch_024_done
.L_nc_x64_mb_prefetch_024_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_prefetch_024_scalar
.L_nc_x64_mb_prefetch_024_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_000
nc_x64_mb_cache_000:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_000_done
    test r10, r10
    jz .L_nc_x64_mb_cache_000_scalar
.L_nc_x64_mb_cache_000_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_000_loop
    jmp .L_nc_x64_mb_cache_000_done
.L_nc_x64_mb_cache_000_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_000_scalar
.L_nc_x64_mb_cache_000_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_001
nc_x64_mb_cache_001:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_001_done
    test r10, r10
    jz .L_nc_x64_mb_cache_001_scalar
.L_nc_x64_mb_cache_001_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_001_loop
    jmp .L_nc_x64_mb_cache_001_done
.L_nc_x64_mb_cache_001_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_001_scalar
.L_nc_x64_mb_cache_001_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_002
nc_x64_mb_cache_002:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_002_done
    test r10, r10
    jz .L_nc_x64_mb_cache_002_scalar
.L_nc_x64_mb_cache_002_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_002_loop
    jmp .L_nc_x64_mb_cache_002_done
.L_nc_x64_mb_cache_002_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_002_scalar
.L_nc_x64_mb_cache_002_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_003
nc_x64_mb_cache_003:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_003_done
    test r10, r10
    jz .L_nc_x64_mb_cache_003_scalar
.L_nc_x64_mb_cache_003_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_003_loop
    jmp .L_nc_x64_mb_cache_003_done
.L_nc_x64_mb_cache_003_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_003_scalar
.L_nc_x64_mb_cache_003_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_004
nc_x64_mb_cache_004:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_004_done
    test r10, r10
    jz .L_nc_x64_mb_cache_004_scalar
.L_nc_x64_mb_cache_004_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_004_loop
    jmp .L_nc_x64_mb_cache_004_done
.L_nc_x64_mb_cache_004_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_004_scalar
.L_nc_x64_mb_cache_004_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_005
nc_x64_mb_cache_005:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_005_done
    test r10, r10
    jz .L_nc_x64_mb_cache_005_scalar
.L_nc_x64_mb_cache_005_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_005_loop
    jmp .L_nc_x64_mb_cache_005_done
.L_nc_x64_mb_cache_005_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_005_scalar
.L_nc_x64_mb_cache_005_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_006
nc_x64_mb_cache_006:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_006_done
    test r10, r10
    jz .L_nc_x64_mb_cache_006_scalar
.L_nc_x64_mb_cache_006_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_006_loop
    jmp .L_nc_x64_mb_cache_006_done
.L_nc_x64_mb_cache_006_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_006_scalar
.L_nc_x64_mb_cache_006_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_007
nc_x64_mb_cache_007:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_007_done
    test r10, r10
    jz .L_nc_x64_mb_cache_007_scalar
.L_nc_x64_mb_cache_007_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_007_loop
    jmp .L_nc_x64_mb_cache_007_done
.L_nc_x64_mb_cache_007_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_007_scalar
.L_nc_x64_mb_cache_007_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_008
nc_x64_mb_cache_008:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_008_done
    test r10, r10
    jz .L_nc_x64_mb_cache_008_scalar
.L_nc_x64_mb_cache_008_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_008_loop
    jmp .L_nc_x64_mb_cache_008_done
.L_nc_x64_mb_cache_008_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_008_scalar
.L_nc_x64_mb_cache_008_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_009
nc_x64_mb_cache_009:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_009_done
    test r10, r10
    jz .L_nc_x64_mb_cache_009_scalar
.L_nc_x64_mb_cache_009_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_009_loop
    jmp .L_nc_x64_mb_cache_009_done
.L_nc_x64_mb_cache_009_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_009_scalar
.L_nc_x64_mb_cache_009_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_010
nc_x64_mb_cache_010:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_010_done
    test r10, r10
    jz .L_nc_x64_mb_cache_010_scalar
.L_nc_x64_mb_cache_010_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_010_loop
    jmp .L_nc_x64_mb_cache_010_done
.L_nc_x64_mb_cache_010_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_010_scalar
.L_nc_x64_mb_cache_010_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_011
nc_x64_mb_cache_011:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_011_done
    test r10, r10
    jz .L_nc_x64_mb_cache_011_scalar
.L_nc_x64_mb_cache_011_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_011_loop
    jmp .L_nc_x64_mb_cache_011_done
.L_nc_x64_mb_cache_011_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_011_scalar
.L_nc_x64_mb_cache_011_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_012
nc_x64_mb_cache_012:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_012_done
    test r10, r10
    jz .L_nc_x64_mb_cache_012_scalar
.L_nc_x64_mb_cache_012_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_012_loop
    jmp .L_nc_x64_mb_cache_012_done
.L_nc_x64_mb_cache_012_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_012_scalar
.L_nc_x64_mb_cache_012_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_013
nc_x64_mb_cache_013:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_013_done
    test r10, r10
    jz .L_nc_x64_mb_cache_013_scalar
.L_nc_x64_mb_cache_013_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_013_loop
    jmp .L_nc_x64_mb_cache_013_done
.L_nc_x64_mb_cache_013_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_013_scalar
.L_nc_x64_mb_cache_013_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_014
nc_x64_mb_cache_014:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_014_done
    test r10, r10
    jz .L_nc_x64_mb_cache_014_scalar
.L_nc_x64_mb_cache_014_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_014_loop
    jmp .L_nc_x64_mb_cache_014_done
.L_nc_x64_mb_cache_014_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_014_scalar
.L_nc_x64_mb_cache_014_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_015
nc_x64_mb_cache_015:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_015_done
    test r10, r10
    jz .L_nc_x64_mb_cache_015_scalar
.L_nc_x64_mb_cache_015_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_015_loop
    jmp .L_nc_x64_mb_cache_015_done
.L_nc_x64_mb_cache_015_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_015_scalar
.L_nc_x64_mb_cache_015_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_016
nc_x64_mb_cache_016:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_016_done
    test r10, r10
    jz .L_nc_x64_mb_cache_016_scalar
.L_nc_x64_mb_cache_016_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_016_loop
    jmp .L_nc_x64_mb_cache_016_done
.L_nc_x64_mb_cache_016_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_016_scalar
.L_nc_x64_mb_cache_016_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_017
nc_x64_mb_cache_017:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_017_done
    test r10, r10
    jz .L_nc_x64_mb_cache_017_scalar
.L_nc_x64_mb_cache_017_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_017_loop
    jmp .L_nc_x64_mb_cache_017_done
.L_nc_x64_mb_cache_017_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_017_scalar
.L_nc_x64_mb_cache_017_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_018
nc_x64_mb_cache_018:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_018_done
    test r10, r10
    jz .L_nc_x64_mb_cache_018_scalar
.L_nc_x64_mb_cache_018_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_018_loop
    jmp .L_nc_x64_mb_cache_018_done
.L_nc_x64_mb_cache_018_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_018_scalar
.L_nc_x64_mb_cache_018_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_019
nc_x64_mb_cache_019:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_019_done
    test r10, r10
    jz .L_nc_x64_mb_cache_019_scalar
.L_nc_x64_mb_cache_019_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_019_loop
    jmp .L_nc_x64_mb_cache_019_done
.L_nc_x64_mb_cache_019_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_019_scalar
.L_nc_x64_mb_cache_019_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_020
nc_x64_mb_cache_020:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_020_done
    test r10, r10
    jz .L_nc_x64_mb_cache_020_scalar
.L_nc_x64_mb_cache_020_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_020_loop
    jmp .L_nc_x64_mb_cache_020_done
.L_nc_x64_mb_cache_020_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_020_scalar
.L_nc_x64_mb_cache_020_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_021
nc_x64_mb_cache_021:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_021_done
    test r10, r10
    jz .L_nc_x64_mb_cache_021_scalar
.L_nc_x64_mb_cache_021_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_021_loop
    jmp .L_nc_x64_mb_cache_021_done
.L_nc_x64_mb_cache_021_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_021_scalar
.L_nc_x64_mb_cache_021_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_022
nc_x64_mb_cache_022:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_022_done
    test r10, r10
    jz .L_nc_x64_mb_cache_022_scalar
.L_nc_x64_mb_cache_022_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_022_loop
    jmp .L_nc_x64_mb_cache_022_done
.L_nc_x64_mb_cache_022_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_022_scalar
.L_nc_x64_mb_cache_022_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_023
nc_x64_mb_cache_023:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_023_done
    test r10, r10
    jz .L_nc_x64_mb_cache_023_scalar
.L_nc_x64_mb_cache_023_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_023_loop
    jmp .L_nc_x64_mb_cache_023_done
.L_nc_x64_mb_cache_023_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_023_scalar
.L_nc_x64_mb_cache_023_done:
    mov rax, r8
    ret

global nc_x64_mb_cache_024
nc_x64_mb_cache_024:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_cache_024_done
    test r10, r10
    jz .L_nc_x64_mb_cache_024_scalar
.L_nc_x64_mb_cache_024_loop:
    clflush [r10]
    mfence
    movzx eax, byte [r10]
    xor r8, rax
    dec ecx
    jnz .L_nc_x64_mb_cache_024_loop
    jmp .L_nc_x64_mb_cache_024_done
.L_nc_x64_mb_cache_024_scalar:
    add r8, rcx
    dec ecx
    jnz .L_nc_x64_mb_cache_024_scalar
.L_nc_x64_mb_cache_024_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_000
nc_x64_mb_sse2_000:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_000_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_000_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_000_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_000_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_001
nc_x64_mb_sse2_001:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_001_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_001_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_001_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_001_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_002
nc_x64_mb_sse2_002:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_002_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_002_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_002_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_002_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_003
nc_x64_mb_sse2_003:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_003_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_003_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_003_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_003_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_004
nc_x64_mb_sse2_004:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_004_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_004_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_004_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_004_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_005
nc_x64_mb_sse2_005:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_005_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_005_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_005_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_005_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_006
nc_x64_mb_sse2_006:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_006_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_006_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_006_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_006_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_007
nc_x64_mb_sse2_007:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_007_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_007_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_007_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_007_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_008
nc_x64_mb_sse2_008:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_008_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_008_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_008_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_008_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_009
nc_x64_mb_sse2_009:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_009_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_009_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_009_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_009_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_010
nc_x64_mb_sse2_010:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_010_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_010_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_010_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_010_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_011
nc_x64_mb_sse2_011:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_011_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_011_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_011_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_011_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_012
nc_x64_mb_sse2_012:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_012_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_012_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_012_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_012_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_013
nc_x64_mb_sse2_013:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_013_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_013_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_013_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_013_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_014
nc_x64_mb_sse2_014:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_014_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_014_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_014_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_014_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_015
nc_x64_mb_sse2_015:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_015_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_015_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_015_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_015_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_016
nc_x64_mb_sse2_016:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_016_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_016_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_016_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_016_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_017
nc_x64_mb_sse2_017:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_017_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_017_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_017_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_017_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_018
nc_x64_mb_sse2_018:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_018_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_018_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_018_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_018_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_019
nc_x64_mb_sse2_019:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_019_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_019_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_019_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_019_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_020
nc_x64_mb_sse2_020:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_020_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_020_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_020_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_020_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_021
nc_x64_mb_sse2_021:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_021_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_021_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_021_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_021_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_022
nc_x64_mb_sse2_022:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_022_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_022_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_022_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_022_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_023
nc_x64_mb_sse2_023:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_023_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_023_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_023_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_023_done:
    mov rax, r8
    ret

global nc_x64_mb_sse2_024
nc_x64_mb_sse2_024:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_sse2_024_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_sse2_024_loop:
    paddq xmm0, xmm1
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_sse2_024_loop
    movq r8, xmm0
.L_nc_x64_mb_sse2_024_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_000
nc_x64_mb_aesni_000:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_000_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_000_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_000_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_000_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_001
nc_x64_mb_aesni_001:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_001_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_001_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_001_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_001_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_002
nc_x64_mb_aesni_002:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_002_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_002_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_002_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_002_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_003
nc_x64_mb_aesni_003:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_003_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_003_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_003_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_003_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_004
nc_x64_mb_aesni_004:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_004_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_004_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_004_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_004_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_005
nc_x64_mb_aesni_005:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_005_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_005_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_005_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_005_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_006
nc_x64_mb_aesni_006:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_006_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_006_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_006_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_006_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_007
nc_x64_mb_aesni_007:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_007_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_007_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_007_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_007_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_008
nc_x64_mb_aesni_008:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_008_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_008_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_008_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_008_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_009
nc_x64_mb_aesni_009:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_009_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_009_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_009_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_009_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_010
nc_x64_mb_aesni_010:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_010_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_010_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_010_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_010_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_011
nc_x64_mb_aesni_011:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_011_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_011_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_011_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_011_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_012
nc_x64_mb_aesni_012:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_012_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_012_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_012_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_012_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_013
nc_x64_mb_aesni_013:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_013_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_013_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_013_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_013_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_014
nc_x64_mb_aesni_014:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_014_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_014_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_014_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_014_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_015
nc_x64_mb_aesni_015:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_015_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_015_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_015_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_015_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_016
nc_x64_mb_aesni_016:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_016_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_016_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_016_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_016_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_017
nc_x64_mb_aesni_017:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_017_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_017_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_017_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_017_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_018
nc_x64_mb_aesni_018:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_018_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_018_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_018_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_018_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_019
nc_x64_mb_aesni_019:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_019_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_019_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_019_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_019_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_020
nc_x64_mb_aesni_020:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_020_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_020_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_020_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_020_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_021
nc_x64_mb_aesni_021:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_021_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_021_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_021_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_021_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_022
nc_x64_mb_aesni_022:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_022_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_022_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_022_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_022_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_023
nc_x64_mb_aesni_023:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_023_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_023_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_023_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_023_done:
    mov rax, r8
    ret

global nc_x64_mb_aesni_024
nc_x64_mb_aesni_024:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_aesni_024_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
.L_nc_x64_mb_aesni_024_loop:
    aesenc xmm0, xmm1
    aesdec xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_aesni_024_loop
    movq r8, xmm0
.L_nc_x64_mb_aesni_024_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_000
nc_x64_mb_pclmul_000:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_000_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_000_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_000_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_000_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_001
nc_x64_mb_pclmul_001:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_001_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_001_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_001_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_001_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_002
nc_x64_mb_pclmul_002:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_002_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_002_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_002_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_002_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_003
nc_x64_mb_pclmul_003:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_003_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_003_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_003_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_003_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_004
nc_x64_mb_pclmul_004:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_004_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_004_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_004_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_004_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_005
nc_x64_mb_pclmul_005:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_005_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_005_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_005_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_005_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_006
nc_x64_mb_pclmul_006:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_006_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_006_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_006_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_006_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_007
nc_x64_mb_pclmul_007:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_007_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_007_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_007_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_007_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_008
nc_x64_mb_pclmul_008:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_008_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_008_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_008_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_008_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_009
nc_x64_mb_pclmul_009:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_009_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_009_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_009_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_009_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_010
nc_x64_mb_pclmul_010:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_010_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_010_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_010_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_010_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_011
nc_x64_mb_pclmul_011:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_011_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_011_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_011_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_011_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_012
nc_x64_mb_pclmul_012:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_012_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_012_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_012_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_012_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_013
nc_x64_mb_pclmul_013:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_013_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_013_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_013_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_013_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_014
nc_x64_mb_pclmul_014:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_014_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_014_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_014_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_014_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_015
nc_x64_mb_pclmul_015:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_015_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_015_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_015_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_015_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_016
nc_x64_mb_pclmul_016:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_016_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_016_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_016_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_016_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_017
nc_x64_mb_pclmul_017:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_017_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_017_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_017_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_017_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_018
nc_x64_mb_pclmul_018:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_018_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_018_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_018_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_018_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_019
nc_x64_mb_pclmul_019:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_019_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_019_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_019_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_019_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_020
nc_x64_mb_pclmul_020:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_020_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_020_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_020_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_020_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_021
nc_x64_mb_pclmul_021:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_021_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_021_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_021_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_021_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_022
nc_x64_mb_pclmul_022:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_022_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_022_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_022_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_022_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_023
nc_x64_mb_pclmul_023:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_023_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_023_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_023_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_023_done:
    mov rax, r8
    ret

global nc_x64_mb_pclmul_024
nc_x64_mb_pclmul_024:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_pclmul_024_done
    mov eax, 0x1f35a7bd
    movq xmm0, rax
    mov eax, 0x9e3779b9
    movq xmm1, rax
.L_nc_x64_mb_pclmul_024_loop:
    pclmulqdq xmm0, xmm1, 0x00
    pxor xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_pclmul_024_loop
    movq r8, xmm0
.L_nc_x64_mb_pclmul_024_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_000
nc_x64_mb_shani_000:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_000_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_000_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_000_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_000_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_001
nc_x64_mb_shani_001:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_001_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_001_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_001_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_001_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_002
nc_x64_mb_shani_002:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_002_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_002_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_002_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_002_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_003
nc_x64_mb_shani_003:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_003_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_003_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_003_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_003_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_004
nc_x64_mb_shani_004:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_004_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_004_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_004_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_004_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_005
nc_x64_mb_shani_005:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_005_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_005_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_005_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_005_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_006
nc_x64_mb_shani_006:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_006_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_006_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_006_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_006_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_007
nc_x64_mb_shani_007:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_007_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_007_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_007_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_007_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_008
nc_x64_mb_shani_008:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_008_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_008_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_008_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_008_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_009
nc_x64_mb_shani_009:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_009_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_009_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_009_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_009_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_010
nc_x64_mb_shani_010:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_010_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_010_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_010_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_010_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_011
nc_x64_mb_shani_011:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_011_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_011_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_011_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_011_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_012
nc_x64_mb_shani_012:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_012_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_012_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_012_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_012_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_013
nc_x64_mb_shani_013:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_013_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_013_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_013_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_013_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_014
nc_x64_mb_shani_014:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_014_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_014_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_014_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_014_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_015
nc_x64_mb_shani_015:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_015_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_015_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_015_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_015_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_016
nc_x64_mb_shani_016:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_016_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_016_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_016_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_016_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_017
nc_x64_mb_shani_017:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_017_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_017_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_017_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_017_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_018
nc_x64_mb_shani_018:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_018_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_018_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_018_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_018_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_019
nc_x64_mb_shani_019:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_019_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_019_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_019_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_019_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_020
nc_x64_mb_shani_020:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_020_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_020_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_020_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_020_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_021
nc_x64_mb_shani_021:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_021_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_021_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_021_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_021_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_022
nc_x64_mb_shani_022:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_022_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_022_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_022_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_022_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_023
nc_x64_mb_shani_023:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_023_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_023_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_023_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_023_done:
    mov rax, r8
    ret

global nc_x64_mb_shani_024
nc_x64_mb_shani_024:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_shani_024_done
    pxor xmm0, xmm0
    pcmpeqd xmm1, xmm1
    movdqa xmm2, xmm1
.L_nc_x64_mb_shani_024_loop:
    sha256rnds2 xmm1, xmm2
    paddd xmm0, xmm1
    dec ecx
    jnz .L_nc_x64_mb_shani_024_loop
    movq r8, xmm0
.L_nc_x64_mb_shani_024_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_000
nc_x64_mb_avx2_000:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_000_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_000_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_000_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_000_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_001
nc_x64_mb_avx2_001:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_001_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_001_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_001_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_001_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_002
nc_x64_mb_avx2_002:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_002_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_002_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_002_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_002_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_003
nc_x64_mb_avx2_003:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_003_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_003_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_003_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_003_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_004
nc_x64_mb_avx2_004:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_004_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_004_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_004_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_004_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_005
nc_x64_mb_avx2_005:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_005_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_005_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_005_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_005_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_006
nc_x64_mb_avx2_006:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_006_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_006_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_006_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_006_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_007
nc_x64_mb_avx2_007:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_007_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_007_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_007_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_007_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_008
nc_x64_mb_avx2_008:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_008_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_008_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_008_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_008_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_009
nc_x64_mb_avx2_009:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_009_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_009_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_009_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_009_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_010
nc_x64_mb_avx2_010:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_010_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_010_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_010_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_010_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_011
nc_x64_mb_avx2_011:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_011_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_011_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_011_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_011_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_012
nc_x64_mb_avx2_012:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_012_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_012_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_012_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_012_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_013
nc_x64_mb_avx2_013:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_013_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_013_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_013_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_013_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_014
nc_x64_mb_avx2_014:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_014_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_014_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_014_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_014_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_015
nc_x64_mb_avx2_015:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_015_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_015_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_015_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_015_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_016
nc_x64_mb_avx2_016:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_016_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_016_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_016_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_016_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_017
nc_x64_mb_avx2_017:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_017_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_017_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_017_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_017_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_018
nc_x64_mb_avx2_018:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_018_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_018_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_018_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_018_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_019
nc_x64_mb_avx2_019:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_019_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_019_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_019_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_019_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_020
nc_x64_mb_avx2_020:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_020_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_020_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_020_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_020_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_021
nc_x64_mb_avx2_021:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_021_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_021_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_021_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_021_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_022
nc_x64_mb_avx2_022:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_022_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_022_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_022_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_022_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_023
nc_x64_mb_avx2_023:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_023_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_023_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_023_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_023_done:
    mov rax, r8
    ret

global nc_x64_mb_avx2_024
nc_x64_mb_avx2_024:
    mov ecx, A1d
    mov r10, A2
    mov r11, A3
    xor r8, r8
    test ecx, ecx
    jz .L_nc_x64_mb_avx2_024_done
    vpxor ymm0, ymm0, ymm0
    vpcmpeqd ymm1, ymm1, ymm1
.L_nc_x64_mb_avx2_024_loop:
    vpaddq ymm0, ymm0, ymm1
    vpxor ymm0, ymm0, ymm1
    dec ecx
    jnz .L_nc_x64_mb_avx2_024_loop
    vmovq r8, xmm0
    vzeroupper
.L_nc_x64_mb_avx2_024_done:
    mov rax, r8
    ret

