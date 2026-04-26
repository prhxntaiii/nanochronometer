default rel
section .text
global nc_sse4_touch
nc_sse4_touch: ret
global nc_sse4_tsc_start
nc_sse4_tsc_start: lfence
    rdtsc
    shl rdx,32
    or rax,rdx
    ret
global nc_sse4_tsc_end
nc_sse4_tsc_end: rdtscp
    shl rdx,32
    or rax,rdx
    lfence
    ret
global nc_sse4_tsc_overhead
nc_sse4_tsc_overhead:
    call nc_sse4_tsc_start
    mov r8,rax
    call nc_sse4_tsc_end
    sub rax,r8
    ret

