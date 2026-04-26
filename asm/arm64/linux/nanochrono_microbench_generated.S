.text
.align 2
.global nc_arm64_mb_sisd_000
.type nc_arm64_mb_sisd_000,%function
nc_arm64_mb_sisd_000:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_000_done
.L_nc_arm64_mb_sisd_000_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #1
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_000_loop
.L_nc_arm64_mb_sisd_000_done:
    ret

.global nc_arm64_mb_sisd_001
.type nc_arm64_mb_sisd_001,%function
nc_arm64_mb_sisd_001:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_001_done
.L_nc_arm64_mb_sisd_001_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #2
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_001_loop
.L_nc_arm64_mb_sisd_001_done:
    ret

.global nc_arm64_mb_sisd_002
.type nc_arm64_mb_sisd_002,%function
nc_arm64_mb_sisd_002:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_002_done
.L_nc_arm64_mb_sisd_002_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_002_loop
.L_nc_arm64_mb_sisd_002_done:
    ret

.global nc_arm64_mb_sisd_003
.type nc_arm64_mb_sisd_003,%function
nc_arm64_mb_sisd_003:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_003_done
.L_nc_arm64_mb_sisd_003_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_003_loop
.L_nc_arm64_mb_sisd_003_done:
    ret

.global nc_arm64_mb_sisd_004
.type nc_arm64_mb_sisd_004,%function
nc_arm64_mb_sisd_004:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_004_done
.L_nc_arm64_mb_sisd_004_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #5
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_004_loop
.L_nc_arm64_mb_sisd_004_done:
    ret

.global nc_arm64_mb_sisd_005
.type nc_arm64_mb_sisd_005,%function
nc_arm64_mb_sisd_005:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_005_done
.L_nc_arm64_mb_sisd_005_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #6
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_005_loop
.L_nc_arm64_mb_sisd_005_done:
    ret

.global nc_arm64_mb_sisd_006
.type nc_arm64_mb_sisd_006,%function
nc_arm64_mb_sisd_006:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_006_done
.L_nc_arm64_mb_sisd_006_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #7
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_006_loop
.L_nc_arm64_mb_sisd_006_done:
    ret

.global nc_arm64_mb_sisd_007
.type nc_arm64_mb_sisd_007,%function
nc_arm64_mb_sisd_007:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_007_done
.L_nc_arm64_mb_sisd_007_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #8
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_007_loop
.L_nc_arm64_mb_sisd_007_done:
    ret

.global nc_arm64_mb_sisd_008
.type nc_arm64_mb_sisd_008,%function
nc_arm64_mb_sisd_008:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_008_done
.L_nc_arm64_mb_sisd_008_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #9
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_008_loop
.L_nc_arm64_mb_sisd_008_done:
    ret

.global nc_arm64_mb_sisd_009
.type nc_arm64_mb_sisd_009,%function
nc_arm64_mb_sisd_009:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_009_done
.L_nc_arm64_mb_sisd_009_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #10
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_009_loop
.L_nc_arm64_mb_sisd_009_done:
    ret

.global nc_arm64_mb_sisd_010
.type nc_arm64_mb_sisd_010,%function
nc_arm64_mb_sisd_010:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_010_done
.L_nc_arm64_mb_sisd_010_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #11
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_010_loop
.L_nc_arm64_mb_sisd_010_done:
    ret

.global nc_arm64_mb_sisd_011
.type nc_arm64_mb_sisd_011,%function
nc_arm64_mb_sisd_011:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_011_done
.L_nc_arm64_mb_sisd_011_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #12
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_011_loop
.L_nc_arm64_mb_sisd_011_done:
    ret

.global nc_arm64_mb_sisd_012
.type nc_arm64_mb_sisd_012,%function
nc_arm64_mb_sisd_012:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_012_done
.L_nc_arm64_mb_sisd_012_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #13
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_012_loop
.L_nc_arm64_mb_sisd_012_done:
    ret

.global nc_arm64_mb_sisd_013
.type nc_arm64_mb_sisd_013,%function
nc_arm64_mb_sisd_013:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_013_done
.L_nc_arm64_mb_sisd_013_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #14
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_013_loop
.L_nc_arm64_mb_sisd_013_done:
    ret

.global nc_arm64_mb_sisd_014
.type nc_arm64_mb_sisd_014,%function
nc_arm64_mb_sisd_014:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_014_done
.L_nc_arm64_mb_sisd_014_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #15
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_014_loop
.L_nc_arm64_mb_sisd_014_done:
    ret

.global nc_arm64_mb_sisd_015
.type nc_arm64_mb_sisd_015,%function
nc_arm64_mb_sisd_015:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_015_done
.L_nc_arm64_mb_sisd_015_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #16
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_015_loop
.L_nc_arm64_mb_sisd_015_done:
    ret

.global nc_arm64_mb_sisd_016
.type nc_arm64_mb_sisd_016,%function
nc_arm64_mb_sisd_016:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_016_done
.L_nc_arm64_mb_sisd_016_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #1
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_016_loop
.L_nc_arm64_mb_sisd_016_done:
    ret

.global nc_arm64_mb_sisd_017
.type nc_arm64_mb_sisd_017,%function
nc_arm64_mb_sisd_017:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_017_done
.L_nc_arm64_mb_sisd_017_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #2
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_017_loop
.L_nc_arm64_mb_sisd_017_done:
    ret

.global nc_arm64_mb_sisd_018
.type nc_arm64_mb_sisd_018,%function
nc_arm64_mb_sisd_018:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_018_done
.L_nc_arm64_mb_sisd_018_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_018_loop
.L_nc_arm64_mb_sisd_018_done:
    ret

.global nc_arm64_mb_sisd_019
.type nc_arm64_mb_sisd_019,%function
nc_arm64_mb_sisd_019:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_019_done
.L_nc_arm64_mb_sisd_019_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_019_loop
.L_nc_arm64_mb_sisd_019_done:
    ret

.global nc_arm64_mb_sisd_020
.type nc_arm64_mb_sisd_020,%function
nc_arm64_mb_sisd_020:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_020_done
.L_nc_arm64_mb_sisd_020_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #5
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_020_loop
.L_nc_arm64_mb_sisd_020_done:
    ret

.global nc_arm64_mb_sisd_021
.type nc_arm64_mb_sisd_021,%function
nc_arm64_mb_sisd_021:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_021_done
.L_nc_arm64_mb_sisd_021_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #6
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_021_loop
.L_nc_arm64_mb_sisd_021_done:
    ret

.global nc_arm64_mb_sisd_022
.type nc_arm64_mb_sisd_022,%function
nc_arm64_mb_sisd_022:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_022_done
.L_nc_arm64_mb_sisd_022_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #7
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_022_loop
.L_nc_arm64_mb_sisd_022_done:
    ret

.global nc_arm64_mb_sisd_023
.type nc_arm64_mb_sisd_023,%function
nc_arm64_mb_sisd_023:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_023_done
.L_nc_arm64_mb_sisd_023_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #8
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_023_loop
.L_nc_arm64_mb_sisd_023_done:
    ret

.global nc_arm64_mb_sisd_024
.type nc_arm64_mb_sisd_024,%function
nc_arm64_mb_sisd_024:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_024_done
.L_nc_arm64_mb_sisd_024_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #9
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_024_loop
.L_nc_arm64_mb_sisd_024_done:
    ret

.global nc_arm64_mb_sisd_025
.type nc_arm64_mb_sisd_025,%function
nc_arm64_mb_sisd_025:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_025_done
.L_nc_arm64_mb_sisd_025_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #10
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_025_loop
.L_nc_arm64_mb_sisd_025_done:
    ret

.global nc_arm64_mb_sisd_026
.type nc_arm64_mb_sisd_026,%function
nc_arm64_mb_sisd_026:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_026_done
.L_nc_arm64_mb_sisd_026_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #11
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_026_loop
.L_nc_arm64_mb_sisd_026_done:
    ret

.global nc_arm64_mb_sisd_027
.type nc_arm64_mb_sisd_027,%function
nc_arm64_mb_sisd_027:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_027_done
.L_nc_arm64_mb_sisd_027_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #12
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_027_loop
.L_nc_arm64_mb_sisd_027_done:
    ret

.global nc_arm64_mb_sisd_028
.type nc_arm64_mb_sisd_028,%function
nc_arm64_mb_sisd_028:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_028_done
.L_nc_arm64_mb_sisd_028_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #13
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_028_loop
.L_nc_arm64_mb_sisd_028_done:
    ret

.global nc_arm64_mb_sisd_029
.type nc_arm64_mb_sisd_029,%function
nc_arm64_mb_sisd_029:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_029_done
.L_nc_arm64_mb_sisd_029_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #14
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_029_loop
.L_nc_arm64_mb_sisd_029_done:
    ret

.global nc_arm64_mb_sisd_030
.type nc_arm64_mb_sisd_030,%function
nc_arm64_mb_sisd_030:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_030_done
.L_nc_arm64_mb_sisd_030_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #15
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_030_loop
.L_nc_arm64_mb_sisd_030_done:
    ret

.global nc_arm64_mb_sisd_031
.type nc_arm64_mb_sisd_031,%function
nc_arm64_mb_sisd_031:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_031_done
.L_nc_arm64_mb_sisd_031_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #16
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_031_loop
.L_nc_arm64_mb_sisd_031_done:
    ret

.global nc_arm64_mb_sisd_032
.type nc_arm64_mb_sisd_032,%function
nc_arm64_mb_sisd_032:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_032_done
.L_nc_arm64_mb_sisd_032_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #1
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_032_loop
.L_nc_arm64_mb_sisd_032_done:
    ret

.global nc_arm64_mb_sisd_033
.type nc_arm64_mb_sisd_033,%function
nc_arm64_mb_sisd_033:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_033_done
.L_nc_arm64_mb_sisd_033_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #2
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_033_loop
.L_nc_arm64_mb_sisd_033_done:
    ret

.global nc_arm64_mb_sisd_034
.type nc_arm64_mb_sisd_034,%function
nc_arm64_mb_sisd_034:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_034_done
.L_nc_arm64_mb_sisd_034_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_034_loop
.L_nc_arm64_mb_sisd_034_done:
    ret

.global nc_arm64_mb_sisd_035
.type nc_arm64_mb_sisd_035,%function
nc_arm64_mb_sisd_035:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_035_done
.L_nc_arm64_mb_sisd_035_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_035_loop
.L_nc_arm64_mb_sisd_035_done:
    ret

.global nc_arm64_mb_sisd_036
.type nc_arm64_mb_sisd_036,%function
nc_arm64_mb_sisd_036:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_036_done
.L_nc_arm64_mb_sisd_036_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #5
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_036_loop
.L_nc_arm64_mb_sisd_036_done:
    ret

.global nc_arm64_mb_sisd_037
.type nc_arm64_mb_sisd_037,%function
nc_arm64_mb_sisd_037:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_037_done
.L_nc_arm64_mb_sisd_037_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #6
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_037_loop
.L_nc_arm64_mb_sisd_037_done:
    ret

.global nc_arm64_mb_sisd_038
.type nc_arm64_mb_sisd_038,%function
nc_arm64_mb_sisd_038:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_038_done
.L_nc_arm64_mb_sisd_038_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #7
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_038_loop
.L_nc_arm64_mb_sisd_038_done:
    ret

.global nc_arm64_mb_sisd_039
.type nc_arm64_mb_sisd_039,%function
nc_arm64_mb_sisd_039:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_039_done
.L_nc_arm64_mb_sisd_039_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #8
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_039_loop
.L_nc_arm64_mb_sisd_039_done:
    ret

.global nc_arm64_mb_sisd_040
.type nc_arm64_mb_sisd_040,%function
nc_arm64_mb_sisd_040:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_040_done
.L_nc_arm64_mb_sisd_040_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #9
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_040_loop
.L_nc_arm64_mb_sisd_040_done:
    ret

.global nc_arm64_mb_sisd_041
.type nc_arm64_mb_sisd_041,%function
nc_arm64_mb_sisd_041:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_041_done
.L_nc_arm64_mb_sisd_041_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #10
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_041_loop
.L_nc_arm64_mb_sisd_041_done:
    ret

.global nc_arm64_mb_sisd_042
.type nc_arm64_mb_sisd_042,%function
nc_arm64_mb_sisd_042:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_042_done
.L_nc_arm64_mb_sisd_042_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #11
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_042_loop
.L_nc_arm64_mb_sisd_042_done:
    ret

.global nc_arm64_mb_sisd_043
.type nc_arm64_mb_sisd_043,%function
nc_arm64_mb_sisd_043:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_043_done
.L_nc_arm64_mb_sisd_043_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #12
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_043_loop
.L_nc_arm64_mb_sisd_043_done:
    ret

.global nc_arm64_mb_sisd_044
.type nc_arm64_mb_sisd_044,%function
nc_arm64_mb_sisd_044:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_044_done
.L_nc_arm64_mb_sisd_044_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #13
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_044_loop
.L_nc_arm64_mb_sisd_044_done:
    ret

.global nc_arm64_mb_sisd_045
.type nc_arm64_mb_sisd_045,%function
nc_arm64_mb_sisd_045:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_045_done
.L_nc_arm64_mb_sisd_045_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #14
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_045_loop
.L_nc_arm64_mb_sisd_045_done:
    ret

.global nc_arm64_mb_sisd_046
.type nc_arm64_mb_sisd_046,%function
nc_arm64_mb_sisd_046:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_046_done
.L_nc_arm64_mb_sisd_046_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #15
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_046_loop
.L_nc_arm64_mb_sisd_046_done:
    ret

.global nc_arm64_mb_sisd_047
.type nc_arm64_mb_sisd_047,%function
nc_arm64_mb_sisd_047:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_047_done
.L_nc_arm64_mb_sisd_047_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #16
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_047_loop
.L_nc_arm64_mb_sisd_047_done:
    ret

.global nc_arm64_mb_sisd_048
.type nc_arm64_mb_sisd_048,%function
nc_arm64_mb_sisd_048:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_048_done
.L_nc_arm64_mb_sisd_048_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #1
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_048_loop
.L_nc_arm64_mb_sisd_048_done:
    ret

.global nc_arm64_mb_sisd_049
.type nc_arm64_mb_sisd_049,%function
nc_arm64_mb_sisd_049:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_sisd_049_done
.L_nc_arm64_mb_sisd_049_loop:
    add x0, x0, x3
    eor x0, x0, x3, lsl #2
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_sisd_049_loop
.L_nc_arm64_mb_sisd_049_done:
    ret

.global nc_arm64_mb_counter_000
.type nc_arm64_mb_counter_000,%function
nc_arm64_mb_counter_000:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_000_done
.L_nc_arm64_mb_counter_000_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_000_loop
.L_nc_arm64_mb_counter_000_done:
    ret

.global nc_arm64_mb_counter_001
.type nc_arm64_mb_counter_001,%function
nc_arm64_mb_counter_001:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_001_done
.L_nc_arm64_mb_counter_001_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_001_loop
.L_nc_arm64_mb_counter_001_done:
    ret

.global nc_arm64_mb_counter_002
.type nc_arm64_mb_counter_002,%function
nc_arm64_mb_counter_002:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_002_done
.L_nc_arm64_mb_counter_002_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_002_loop
.L_nc_arm64_mb_counter_002_done:
    ret

.global nc_arm64_mb_counter_003
.type nc_arm64_mb_counter_003,%function
nc_arm64_mb_counter_003:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_003_done
.L_nc_arm64_mb_counter_003_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_003_loop
.L_nc_arm64_mb_counter_003_done:
    ret

.global nc_arm64_mb_counter_004
.type nc_arm64_mb_counter_004,%function
nc_arm64_mb_counter_004:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_004_done
.L_nc_arm64_mb_counter_004_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_004_loop
.L_nc_arm64_mb_counter_004_done:
    ret

.global nc_arm64_mb_counter_005
.type nc_arm64_mb_counter_005,%function
nc_arm64_mb_counter_005:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_005_done
.L_nc_arm64_mb_counter_005_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_005_loop
.L_nc_arm64_mb_counter_005_done:
    ret

.global nc_arm64_mb_counter_006
.type nc_arm64_mb_counter_006,%function
nc_arm64_mb_counter_006:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_006_done
.L_nc_arm64_mb_counter_006_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_006_loop
.L_nc_arm64_mb_counter_006_done:
    ret

.global nc_arm64_mb_counter_007
.type nc_arm64_mb_counter_007,%function
nc_arm64_mb_counter_007:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_007_done
.L_nc_arm64_mb_counter_007_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_007_loop
.L_nc_arm64_mb_counter_007_done:
    ret

.global nc_arm64_mb_counter_008
.type nc_arm64_mb_counter_008,%function
nc_arm64_mb_counter_008:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_008_done
.L_nc_arm64_mb_counter_008_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_008_loop
.L_nc_arm64_mb_counter_008_done:
    ret

.global nc_arm64_mb_counter_009
.type nc_arm64_mb_counter_009,%function
nc_arm64_mb_counter_009:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_009_done
.L_nc_arm64_mb_counter_009_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_009_loop
.L_nc_arm64_mb_counter_009_done:
    ret

.global nc_arm64_mb_counter_010
.type nc_arm64_mb_counter_010,%function
nc_arm64_mb_counter_010:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_010_done
.L_nc_arm64_mb_counter_010_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_010_loop
.L_nc_arm64_mb_counter_010_done:
    ret

.global nc_arm64_mb_counter_011
.type nc_arm64_mb_counter_011,%function
nc_arm64_mb_counter_011:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_011_done
.L_nc_arm64_mb_counter_011_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_011_loop
.L_nc_arm64_mb_counter_011_done:
    ret

.global nc_arm64_mb_counter_012
.type nc_arm64_mb_counter_012,%function
nc_arm64_mb_counter_012:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_012_done
.L_nc_arm64_mb_counter_012_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_012_loop
.L_nc_arm64_mb_counter_012_done:
    ret

.global nc_arm64_mb_counter_013
.type nc_arm64_mb_counter_013,%function
nc_arm64_mb_counter_013:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_013_done
.L_nc_arm64_mb_counter_013_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_013_loop
.L_nc_arm64_mb_counter_013_done:
    ret

.global nc_arm64_mb_counter_014
.type nc_arm64_mb_counter_014,%function
nc_arm64_mb_counter_014:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_014_done
.L_nc_arm64_mb_counter_014_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_014_loop
.L_nc_arm64_mb_counter_014_done:
    ret

.global nc_arm64_mb_counter_015
.type nc_arm64_mb_counter_015,%function
nc_arm64_mb_counter_015:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_015_done
.L_nc_arm64_mb_counter_015_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_015_loop
.L_nc_arm64_mb_counter_015_done:
    ret

.global nc_arm64_mb_counter_016
.type nc_arm64_mb_counter_016,%function
nc_arm64_mb_counter_016:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_016_done
.L_nc_arm64_mb_counter_016_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_016_loop
.L_nc_arm64_mb_counter_016_done:
    ret

.global nc_arm64_mb_counter_017
.type nc_arm64_mb_counter_017,%function
nc_arm64_mb_counter_017:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_017_done
.L_nc_arm64_mb_counter_017_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_017_loop
.L_nc_arm64_mb_counter_017_done:
    ret

.global nc_arm64_mb_counter_018
.type nc_arm64_mb_counter_018,%function
nc_arm64_mb_counter_018:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_018_done
.L_nc_arm64_mb_counter_018_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_018_loop
.L_nc_arm64_mb_counter_018_done:
    ret

.global nc_arm64_mb_counter_019
.type nc_arm64_mb_counter_019,%function
nc_arm64_mb_counter_019:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_019_done
.L_nc_arm64_mb_counter_019_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_019_loop
.L_nc_arm64_mb_counter_019_done:
    ret

.global nc_arm64_mb_counter_020
.type nc_arm64_mb_counter_020,%function
nc_arm64_mb_counter_020:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_020_done
.L_nc_arm64_mb_counter_020_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_020_loop
.L_nc_arm64_mb_counter_020_done:
    ret

.global nc_arm64_mb_counter_021
.type nc_arm64_mb_counter_021,%function
nc_arm64_mb_counter_021:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_021_done
.L_nc_arm64_mb_counter_021_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_021_loop
.L_nc_arm64_mb_counter_021_done:
    ret

.global nc_arm64_mb_counter_022
.type nc_arm64_mb_counter_022,%function
nc_arm64_mb_counter_022:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_022_done
.L_nc_arm64_mb_counter_022_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_022_loop
.L_nc_arm64_mb_counter_022_done:
    ret

.global nc_arm64_mb_counter_023
.type nc_arm64_mb_counter_023,%function
nc_arm64_mb_counter_023:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_023_done
.L_nc_arm64_mb_counter_023_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_023_loop
.L_nc_arm64_mb_counter_023_done:
    ret

.global nc_arm64_mb_counter_024
.type nc_arm64_mb_counter_024,%function
nc_arm64_mb_counter_024:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_024_done
.L_nc_arm64_mb_counter_024_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_024_loop
.L_nc_arm64_mb_counter_024_done:
    ret

.global nc_arm64_mb_counter_025
.type nc_arm64_mb_counter_025,%function
nc_arm64_mb_counter_025:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_025_done
.L_nc_arm64_mb_counter_025_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_025_loop
.L_nc_arm64_mb_counter_025_done:
    ret

.global nc_arm64_mb_counter_026
.type nc_arm64_mb_counter_026,%function
nc_arm64_mb_counter_026:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_026_done
.L_nc_arm64_mb_counter_026_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_026_loop
.L_nc_arm64_mb_counter_026_done:
    ret

.global nc_arm64_mb_counter_027
.type nc_arm64_mb_counter_027,%function
nc_arm64_mb_counter_027:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_027_done
.L_nc_arm64_mb_counter_027_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_027_loop
.L_nc_arm64_mb_counter_027_done:
    ret

.global nc_arm64_mb_counter_028
.type nc_arm64_mb_counter_028,%function
nc_arm64_mb_counter_028:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_028_done
.L_nc_arm64_mb_counter_028_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_028_loop
.L_nc_arm64_mb_counter_028_done:
    ret

.global nc_arm64_mb_counter_029
.type nc_arm64_mb_counter_029,%function
nc_arm64_mb_counter_029:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_029_done
.L_nc_arm64_mb_counter_029_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_029_loop
.L_nc_arm64_mb_counter_029_done:
    ret

.global nc_arm64_mb_counter_030
.type nc_arm64_mb_counter_030,%function
nc_arm64_mb_counter_030:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_030_done
.L_nc_arm64_mb_counter_030_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_030_loop
.L_nc_arm64_mb_counter_030_done:
    ret

.global nc_arm64_mb_counter_031
.type nc_arm64_mb_counter_031,%function
nc_arm64_mb_counter_031:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_031_done
.L_nc_arm64_mb_counter_031_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_031_loop
.L_nc_arm64_mb_counter_031_done:
    ret

.global nc_arm64_mb_counter_032
.type nc_arm64_mb_counter_032,%function
nc_arm64_mb_counter_032:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_032_done
.L_nc_arm64_mb_counter_032_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_032_loop
.L_nc_arm64_mb_counter_032_done:
    ret

.global nc_arm64_mb_counter_033
.type nc_arm64_mb_counter_033,%function
nc_arm64_mb_counter_033:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_033_done
.L_nc_arm64_mb_counter_033_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_033_loop
.L_nc_arm64_mb_counter_033_done:
    ret

.global nc_arm64_mb_counter_034
.type nc_arm64_mb_counter_034,%function
nc_arm64_mb_counter_034:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_034_done
.L_nc_arm64_mb_counter_034_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_034_loop
.L_nc_arm64_mb_counter_034_done:
    ret

.global nc_arm64_mb_counter_035
.type nc_arm64_mb_counter_035,%function
nc_arm64_mb_counter_035:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_035_done
.L_nc_arm64_mb_counter_035_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_035_loop
.L_nc_arm64_mb_counter_035_done:
    ret

.global nc_arm64_mb_counter_036
.type nc_arm64_mb_counter_036,%function
nc_arm64_mb_counter_036:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_036_done
.L_nc_arm64_mb_counter_036_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_036_loop
.L_nc_arm64_mb_counter_036_done:
    ret

.global nc_arm64_mb_counter_037
.type nc_arm64_mb_counter_037,%function
nc_arm64_mb_counter_037:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_037_done
.L_nc_arm64_mb_counter_037_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_037_loop
.L_nc_arm64_mb_counter_037_done:
    ret

.global nc_arm64_mb_counter_038
.type nc_arm64_mb_counter_038,%function
nc_arm64_mb_counter_038:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_038_done
.L_nc_arm64_mb_counter_038_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_038_loop
.L_nc_arm64_mb_counter_038_done:
    ret

.global nc_arm64_mb_counter_039
.type nc_arm64_mb_counter_039,%function
nc_arm64_mb_counter_039:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_039_done
.L_nc_arm64_mb_counter_039_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_039_loop
.L_nc_arm64_mb_counter_039_done:
    ret

.global nc_arm64_mb_counter_040
.type nc_arm64_mb_counter_040,%function
nc_arm64_mb_counter_040:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_040_done
.L_nc_arm64_mb_counter_040_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_040_loop
.L_nc_arm64_mb_counter_040_done:
    ret

.global nc_arm64_mb_counter_041
.type nc_arm64_mb_counter_041,%function
nc_arm64_mb_counter_041:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_041_done
.L_nc_arm64_mb_counter_041_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_041_loop
.L_nc_arm64_mb_counter_041_done:
    ret

.global nc_arm64_mb_counter_042
.type nc_arm64_mb_counter_042,%function
nc_arm64_mb_counter_042:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_042_done
.L_nc_arm64_mb_counter_042_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_042_loop
.L_nc_arm64_mb_counter_042_done:
    ret

.global nc_arm64_mb_counter_043
.type nc_arm64_mb_counter_043,%function
nc_arm64_mb_counter_043:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_043_done
.L_nc_arm64_mb_counter_043_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_043_loop
.L_nc_arm64_mb_counter_043_done:
    ret

.global nc_arm64_mb_counter_044
.type nc_arm64_mb_counter_044,%function
nc_arm64_mb_counter_044:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_044_done
.L_nc_arm64_mb_counter_044_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_044_loop
.L_nc_arm64_mb_counter_044_done:
    ret

.global nc_arm64_mb_counter_045
.type nc_arm64_mb_counter_045,%function
nc_arm64_mb_counter_045:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_045_done
.L_nc_arm64_mb_counter_045_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_045_loop
.L_nc_arm64_mb_counter_045_done:
    ret

.global nc_arm64_mb_counter_046
.type nc_arm64_mb_counter_046,%function
nc_arm64_mb_counter_046:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_046_done
.L_nc_arm64_mb_counter_046_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_046_loop
.L_nc_arm64_mb_counter_046_done:
    ret

.global nc_arm64_mb_counter_047
.type nc_arm64_mb_counter_047,%function
nc_arm64_mb_counter_047:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_047_done
.L_nc_arm64_mb_counter_047_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_047_loop
.L_nc_arm64_mb_counter_047_done:
    ret

.global nc_arm64_mb_counter_048
.type nc_arm64_mb_counter_048,%function
nc_arm64_mb_counter_048:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_048_done
.L_nc_arm64_mb_counter_048_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_048_loop
.L_nc_arm64_mb_counter_048_done:
    ret

.global nc_arm64_mb_counter_049
.type nc_arm64_mb_counter_049,%function
nc_arm64_mb_counter_049:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_counter_049_done
.L_nc_arm64_mb_counter_049_loop:
    isb
    mrs x4, cntvct_el0
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_counter_049_loop
.L_nc_arm64_mb_counter_049_done:
    ret

.global nc_arm64_mb_barrier_000
.type nc_arm64_mb_barrier_000,%function
nc_arm64_mb_barrier_000:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_000_done
.L_nc_arm64_mb_barrier_000_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_000_loop
.L_nc_arm64_mb_barrier_000_done:
    ret

.global nc_arm64_mb_barrier_001
.type nc_arm64_mb_barrier_001,%function
nc_arm64_mb_barrier_001:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_001_done
.L_nc_arm64_mb_barrier_001_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_001_loop
.L_nc_arm64_mb_barrier_001_done:
    ret

.global nc_arm64_mb_barrier_002
.type nc_arm64_mb_barrier_002,%function
nc_arm64_mb_barrier_002:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_002_done
.L_nc_arm64_mb_barrier_002_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_002_loop
.L_nc_arm64_mb_barrier_002_done:
    ret

.global nc_arm64_mb_barrier_003
.type nc_arm64_mb_barrier_003,%function
nc_arm64_mb_barrier_003:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_003_done
.L_nc_arm64_mb_barrier_003_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_003_loop
.L_nc_arm64_mb_barrier_003_done:
    ret

.global nc_arm64_mb_barrier_004
.type nc_arm64_mb_barrier_004,%function
nc_arm64_mb_barrier_004:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_004_done
.L_nc_arm64_mb_barrier_004_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_004_loop
.L_nc_arm64_mb_barrier_004_done:
    ret

.global nc_arm64_mb_barrier_005
.type nc_arm64_mb_barrier_005,%function
nc_arm64_mb_barrier_005:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_005_done
.L_nc_arm64_mb_barrier_005_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_005_loop
.L_nc_arm64_mb_barrier_005_done:
    ret

.global nc_arm64_mb_barrier_006
.type nc_arm64_mb_barrier_006,%function
nc_arm64_mb_barrier_006:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_006_done
.L_nc_arm64_mb_barrier_006_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_006_loop
.L_nc_arm64_mb_barrier_006_done:
    ret

.global nc_arm64_mb_barrier_007
.type nc_arm64_mb_barrier_007,%function
nc_arm64_mb_barrier_007:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_007_done
.L_nc_arm64_mb_barrier_007_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_007_loop
.L_nc_arm64_mb_barrier_007_done:
    ret

.global nc_arm64_mb_barrier_008
.type nc_arm64_mb_barrier_008,%function
nc_arm64_mb_barrier_008:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_008_done
.L_nc_arm64_mb_barrier_008_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_008_loop
.L_nc_arm64_mb_barrier_008_done:
    ret

.global nc_arm64_mb_barrier_009
.type nc_arm64_mb_barrier_009,%function
nc_arm64_mb_barrier_009:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_009_done
.L_nc_arm64_mb_barrier_009_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_009_loop
.L_nc_arm64_mb_barrier_009_done:
    ret

.global nc_arm64_mb_barrier_010
.type nc_arm64_mb_barrier_010,%function
nc_arm64_mb_barrier_010:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_010_done
.L_nc_arm64_mb_barrier_010_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_010_loop
.L_nc_arm64_mb_barrier_010_done:
    ret

.global nc_arm64_mb_barrier_011
.type nc_arm64_mb_barrier_011,%function
nc_arm64_mb_barrier_011:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_011_done
.L_nc_arm64_mb_barrier_011_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_011_loop
.L_nc_arm64_mb_barrier_011_done:
    ret

.global nc_arm64_mb_barrier_012
.type nc_arm64_mb_barrier_012,%function
nc_arm64_mb_barrier_012:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_012_done
.L_nc_arm64_mb_barrier_012_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_012_loop
.L_nc_arm64_mb_barrier_012_done:
    ret

.global nc_arm64_mb_barrier_013
.type nc_arm64_mb_barrier_013,%function
nc_arm64_mb_barrier_013:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_013_done
.L_nc_arm64_mb_barrier_013_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_013_loop
.L_nc_arm64_mb_barrier_013_done:
    ret

.global nc_arm64_mb_barrier_014
.type nc_arm64_mb_barrier_014,%function
nc_arm64_mb_barrier_014:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_014_done
.L_nc_arm64_mb_barrier_014_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_014_loop
.L_nc_arm64_mb_barrier_014_done:
    ret

.global nc_arm64_mb_barrier_015
.type nc_arm64_mb_barrier_015,%function
nc_arm64_mb_barrier_015:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_015_done
.L_nc_arm64_mb_barrier_015_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_015_loop
.L_nc_arm64_mb_barrier_015_done:
    ret

.global nc_arm64_mb_barrier_016
.type nc_arm64_mb_barrier_016,%function
nc_arm64_mb_barrier_016:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_016_done
.L_nc_arm64_mb_barrier_016_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_016_loop
.L_nc_arm64_mb_barrier_016_done:
    ret

.global nc_arm64_mb_barrier_017
.type nc_arm64_mb_barrier_017,%function
nc_arm64_mb_barrier_017:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_017_done
.L_nc_arm64_mb_barrier_017_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_017_loop
.L_nc_arm64_mb_barrier_017_done:
    ret

.global nc_arm64_mb_barrier_018
.type nc_arm64_mb_barrier_018,%function
nc_arm64_mb_barrier_018:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_018_done
.L_nc_arm64_mb_barrier_018_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_018_loop
.L_nc_arm64_mb_barrier_018_done:
    ret

.global nc_arm64_mb_barrier_019
.type nc_arm64_mb_barrier_019,%function
nc_arm64_mb_barrier_019:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_019_done
.L_nc_arm64_mb_barrier_019_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_019_loop
.L_nc_arm64_mb_barrier_019_done:
    ret

.global nc_arm64_mb_barrier_020
.type nc_arm64_mb_barrier_020,%function
nc_arm64_mb_barrier_020:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_020_done
.L_nc_arm64_mb_barrier_020_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_020_loop
.L_nc_arm64_mb_barrier_020_done:
    ret

.global nc_arm64_mb_barrier_021
.type nc_arm64_mb_barrier_021,%function
nc_arm64_mb_barrier_021:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_021_done
.L_nc_arm64_mb_barrier_021_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_021_loop
.L_nc_arm64_mb_barrier_021_done:
    ret

.global nc_arm64_mb_barrier_022
.type nc_arm64_mb_barrier_022,%function
nc_arm64_mb_barrier_022:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_022_done
.L_nc_arm64_mb_barrier_022_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_022_loop
.L_nc_arm64_mb_barrier_022_done:
    ret

.global nc_arm64_mb_barrier_023
.type nc_arm64_mb_barrier_023,%function
nc_arm64_mb_barrier_023:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_023_done
.L_nc_arm64_mb_barrier_023_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_023_loop
.L_nc_arm64_mb_barrier_023_done:
    ret

.global nc_arm64_mb_barrier_024
.type nc_arm64_mb_barrier_024,%function
nc_arm64_mb_barrier_024:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_024_done
.L_nc_arm64_mb_barrier_024_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_024_loop
.L_nc_arm64_mb_barrier_024_done:
    ret

.global nc_arm64_mb_barrier_025
.type nc_arm64_mb_barrier_025,%function
nc_arm64_mb_barrier_025:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_025_done
.L_nc_arm64_mb_barrier_025_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_025_loop
.L_nc_arm64_mb_barrier_025_done:
    ret

.global nc_arm64_mb_barrier_026
.type nc_arm64_mb_barrier_026,%function
nc_arm64_mb_barrier_026:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_026_done
.L_nc_arm64_mb_barrier_026_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_026_loop
.L_nc_arm64_mb_barrier_026_done:
    ret

.global nc_arm64_mb_barrier_027
.type nc_arm64_mb_barrier_027,%function
nc_arm64_mb_barrier_027:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_027_done
.L_nc_arm64_mb_barrier_027_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_027_loop
.L_nc_arm64_mb_barrier_027_done:
    ret

.global nc_arm64_mb_barrier_028
.type nc_arm64_mb_barrier_028,%function
nc_arm64_mb_barrier_028:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_028_done
.L_nc_arm64_mb_barrier_028_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_028_loop
.L_nc_arm64_mb_barrier_028_done:
    ret

.global nc_arm64_mb_barrier_029
.type nc_arm64_mb_barrier_029,%function
nc_arm64_mb_barrier_029:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_029_done
.L_nc_arm64_mb_barrier_029_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_029_loop
.L_nc_arm64_mb_barrier_029_done:
    ret

.global nc_arm64_mb_barrier_030
.type nc_arm64_mb_barrier_030,%function
nc_arm64_mb_barrier_030:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_030_done
.L_nc_arm64_mb_barrier_030_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_030_loop
.L_nc_arm64_mb_barrier_030_done:
    ret

.global nc_arm64_mb_barrier_031
.type nc_arm64_mb_barrier_031,%function
nc_arm64_mb_barrier_031:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_031_done
.L_nc_arm64_mb_barrier_031_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_031_loop
.L_nc_arm64_mb_barrier_031_done:
    ret

.global nc_arm64_mb_barrier_032
.type nc_arm64_mb_barrier_032,%function
nc_arm64_mb_barrier_032:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_032_done
.L_nc_arm64_mb_barrier_032_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_032_loop
.L_nc_arm64_mb_barrier_032_done:
    ret

.global nc_arm64_mb_barrier_033
.type nc_arm64_mb_barrier_033,%function
nc_arm64_mb_barrier_033:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_033_done
.L_nc_arm64_mb_barrier_033_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_033_loop
.L_nc_arm64_mb_barrier_033_done:
    ret

.global nc_arm64_mb_barrier_034
.type nc_arm64_mb_barrier_034,%function
nc_arm64_mb_barrier_034:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_034_done
.L_nc_arm64_mb_barrier_034_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_034_loop
.L_nc_arm64_mb_barrier_034_done:
    ret

.global nc_arm64_mb_barrier_035
.type nc_arm64_mb_barrier_035,%function
nc_arm64_mb_barrier_035:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_035_done
.L_nc_arm64_mb_barrier_035_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_035_loop
.L_nc_arm64_mb_barrier_035_done:
    ret

.global nc_arm64_mb_barrier_036
.type nc_arm64_mb_barrier_036,%function
nc_arm64_mb_barrier_036:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_036_done
.L_nc_arm64_mb_barrier_036_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_036_loop
.L_nc_arm64_mb_barrier_036_done:
    ret

.global nc_arm64_mb_barrier_037
.type nc_arm64_mb_barrier_037,%function
nc_arm64_mb_barrier_037:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_037_done
.L_nc_arm64_mb_barrier_037_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_037_loop
.L_nc_arm64_mb_barrier_037_done:
    ret

.global nc_arm64_mb_barrier_038
.type nc_arm64_mb_barrier_038,%function
nc_arm64_mb_barrier_038:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_038_done
.L_nc_arm64_mb_barrier_038_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_038_loop
.L_nc_arm64_mb_barrier_038_done:
    ret

.global nc_arm64_mb_barrier_039
.type nc_arm64_mb_barrier_039,%function
nc_arm64_mb_barrier_039:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_039_done
.L_nc_arm64_mb_barrier_039_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_039_loop
.L_nc_arm64_mb_barrier_039_done:
    ret

.global nc_arm64_mb_barrier_040
.type nc_arm64_mb_barrier_040,%function
nc_arm64_mb_barrier_040:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_040_done
.L_nc_arm64_mb_barrier_040_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_040_loop
.L_nc_arm64_mb_barrier_040_done:
    ret

.global nc_arm64_mb_barrier_041
.type nc_arm64_mb_barrier_041,%function
nc_arm64_mb_barrier_041:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_041_done
.L_nc_arm64_mb_barrier_041_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_041_loop
.L_nc_arm64_mb_barrier_041_done:
    ret

.global nc_arm64_mb_barrier_042
.type nc_arm64_mb_barrier_042,%function
nc_arm64_mb_barrier_042:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_042_done
.L_nc_arm64_mb_barrier_042_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_042_loop
.L_nc_arm64_mb_barrier_042_done:
    ret

.global nc_arm64_mb_barrier_043
.type nc_arm64_mb_barrier_043,%function
nc_arm64_mb_barrier_043:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_043_done
.L_nc_arm64_mb_barrier_043_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_043_loop
.L_nc_arm64_mb_barrier_043_done:
    ret

.global nc_arm64_mb_barrier_044
.type nc_arm64_mb_barrier_044,%function
nc_arm64_mb_barrier_044:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_044_done
.L_nc_arm64_mb_barrier_044_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_044_loop
.L_nc_arm64_mb_barrier_044_done:
    ret

.global nc_arm64_mb_barrier_045
.type nc_arm64_mb_barrier_045,%function
nc_arm64_mb_barrier_045:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_045_done
.L_nc_arm64_mb_barrier_045_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_045_loop
.L_nc_arm64_mb_barrier_045_done:
    ret

.global nc_arm64_mb_barrier_046
.type nc_arm64_mb_barrier_046,%function
nc_arm64_mb_barrier_046:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_046_done
.L_nc_arm64_mb_barrier_046_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_046_loop
.L_nc_arm64_mb_barrier_046_done:
    ret

.global nc_arm64_mb_barrier_047
.type nc_arm64_mb_barrier_047,%function
nc_arm64_mb_barrier_047:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_047_done
.L_nc_arm64_mb_barrier_047_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_047_loop
.L_nc_arm64_mb_barrier_047_done:
    ret

.global nc_arm64_mb_barrier_048
.type nc_arm64_mb_barrier_048,%function
nc_arm64_mb_barrier_048:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_048_done
.L_nc_arm64_mb_barrier_048_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_048_loop
.L_nc_arm64_mb_barrier_048_done:
    ret

.global nc_arm64_mb_barrier_049
.type nc_arm64_mb_barrier_049,%function
nc_arm64_mb_barrier_049:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_barrier_049_done
.L_nc_arm64_mb_barrier_049_loop:
    dmb sy
    dsb sy
    isb
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_barrier_049_loop
.L_nc_arm64_mb_barrier_049_done:
    ret

.global nc_arm64_mb_memory_000
.type nc_arm64_mb_memory_000,%function
nc_arm64_mb_memory_000:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_000_done
    cbz x1, .L_nc_arm64_mb_memory_000_scalar
.L_nc_arm64_mb_memory_000_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_000_loop
    b .L_nc_arm64_mb_memory_000_done
.L_nc_arm64_mb_memory_000_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_000_scalar
.L_nc_arm64_mb_memory_000_done:
    ret

.global nc_arm64_mb_memory_001
.type nc_arm64_mb_memory_001,%function
nc_arm64_mb_memory_001:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_001_done
    cbz x1, .L_nc_arm64_mb_memory_001_scalar
.L_nc_arm64_mb_memory_001_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_001_loop
    b .L_nc_arm64_mb_memory_001_done
.L_nc_arm64_mb_memory_001_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_001_scalar
.L_nc_arm64_mb_memory_001_done:
    ret

.global nc_arm64_mb_memory_002
.type nc_arm64_mb_memory_002,%function
nc_arm64_mb_memory_002:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_002_done
    cbz x1, .L_nc_arm64_mb_memory_002_scalar
.L_nc_arm64_mb_memory_002_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_002_loop
    b .L_nc_arm64_mb_memory_002_done
.L_nc_arm64_mb_memory_002_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_002_scalar
.L_nc_arm64_mb_memory_002_done:
    ret

.global nc_arm64_mb_memory_003
.type nc_arm64_mb_memory_003,%function
nc_arm64_mb_memory_003:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_003_done
    cbz x1, .L_nc_arm64_mb_memory_003_scalar
.L_nc_arm64_mb_memory_003_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_003_loop
    b .L_nc_arm64_mb_memory_003_done
.L_nc_arm64_mb_memory_003_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_003_scalar
.L_nc_arm64_mb_memory_003_done:
    ret

.global nc_arm64_mb_memory_004
.type nc_arm64_mb_memory_004,%function
nc_arm64_mb_memory_004:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_004_done
    cbz x1, .L_nc_arm64_mb_memory_004_scalar
.L_nc_arm64_mb_memory_004_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_004_loop
    b .L_nc_arm64_mb_memory_004_done
.L_nc_arm64_mb_memory_004_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_004_scalar
.L_nc_arm64_mb_memory_004_done:
    ret

.global nc_arm64_mb_memory_005
.type nc_arm64_mb_memory_005,%function
nc_arm64_mb_memory_005:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_005_done
    cbz x1, .L_nc_arm64_mb_memory_005_scalar
.L_nc_arm64_mb_memory_005_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_005_loop
    b .L_nc_arm64_mb_memory_005_done
.L_nc_arm64_mb_memory_005_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_005_scalar
.L_nc_arm64_mb_memory_005_done:
    ret

.global nc_arm64_mb_memory_006
.type nc_arm64_mb_memory_006,%function
nc_arm64_mb_memory_006:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_006_done
    cbz x1, .L_nc_arm64_mb_memory_006_scalar
.L_nc_arm64_mb_memory_006_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_006_loop
    b .L_nc_arm64_mb_memory_006_done
.L_nc_arm64_mb_memory_006_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_006_scalar
.L_nc_arm64_mb_memory_006_done:
    ret

.global nc_arm64_mb_memory_007
.type nc_arm64_mb_memory_007,%function
nc_arm64_mb_memory_007:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_007_done
    cbz x1, .L_nc_arm64_mb_memory_007_scalar
.L_nc_arm64_mb_memory_007_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_007_loop
    b .L_nc_arm64_mb_memory_007_done
.L_nc_arm64_mb_memory_007_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_007_scalar
.L_nc_arm64_mb_memory_007_done:
    ret

.global nc_arm64_mb_memory_008
.type nc_arm64_mb_memory_008,%function
nc_arm64_mb_memory_008:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_008_done
    cbz x1, .L_nc_arm64_mb_memory_008_scalar
.L_nc_arm64_mb_memory_008_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_008_loop
    b .L_nc_arm64_mb_memory_008_done
.L_nc_arm64_mb_memory_008_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_008_scalar
.L_nc_arm64_mb_memory_008_done:
    ret

.global nc_arm64_mb_memory_009
.type nc_arm64_mb_memory_009,%function
nc_arm64_mb_memory_009:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_009_done
    cbz x1, .L_nc_arm64_mb_memory_009_scalar
.L_nc_arm64_mb_memory_009_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_009_loop
    b .L_nc_arm64_mb_memory_009_done
.L_nc_arm64_mb_memory_009_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_009_scalar
.L_nc_arm64_mb_memory_009_done:
    ret

.global nc_arm64_mb_memory_010
.type nc_arm64_mb_memory_010,%function
nc_arm64_mb_memory_010:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_010_done
    cbz x1, .L_nc_arm64_mb_memory_010_scalar
.L_nc_arm64_mb_memory_010_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_010_loop
    b .L_nc_arm64_mb_memory_010_done
.L_nc_arm64_mb_memory_010_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_010_scalar
.L_nc_arm64_mb_memory_010_done:
    ret

.global nc_arm64_mb_memory_011
.type nc_arm64_mb_memory_011,%function
nc_arm64_mb_memory_011:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_011_done
    cbz x1, .L_nc_arm64_mb_memory_011_scalar
.L_nc_arm64_mb_memory_011_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_011_loop
    b .L_nc_arm64_mb_memory_011_done
.L_nc_arm64_mb_memory_011_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_011_scalar
.L_nc_arm64_mb_memory_011_done:
    ret

.global nc_arm64_mb_memory_012
.type nc_arm64_mb_memory_012,%function
nc_arm64_mb_memory_012:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_012_done
    cbz x1, .L_nc_arm64_mb_memory_012_scalar
.L_nc_arm64_mb_memory_012_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_012_loop
    b .L_nc_arm64_mb_memory_012_done
.L_nc_arm64_mb_memory_012_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_012_scalar
.L_nc_arm64_mb_memory_012_done:
    ret

.global nc_arm64_mb_memory_013
.type nc_arm64_mb_memory_013,%function
nc_arm64_mb_memory_013:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_013_done
    cbz x1, .L_nc_arm64_mb_memory_013_scalar
.L_nc_arm64_mb_memory_013_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_013_loop
    b .L_nc_arm64_mb_memory_013_done
.L_nc_arm64_mb_memory_013_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_013_scalar
.L_nc_arm64_mb_memory_013_done:
    ret

.global nc_arm64_mb_memory_014
.type nc_arm64_mb_memory_014,%function
nc_arm64_mb_memory_014:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_014_done
    cbz x1, .L_nc_arm64_mb_memory_014_scalar
.L_nc_arm64_mb_memory_014_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_014_loop
    b .L_nc_arm64_mb_memory_014_done
.L_nc_arm64_mb_memory_014_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_014_scalar
.L_nc_arm64_mb_memory_014_done:
    ret

.global nc_arm64_mb_memory_015
.type nc_arm64_mb_memory_015,%function
nc_arm64_mb_memory_015:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_015_done
    cbz x1, .L_nc_arm64_mb_memory_015_scalar
.L_nc_arm64_mb_memory_015_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_015_loop
    b .L_nc_arm64_mb_memory_015_done
.L_nc_arm64_mb_memory_015_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_015_scalar
.L_nc_arm64_mb_memory_015_done:
    ret

.global nc_arm64_mb_memory_016
.type nc_arm64_mb_memory_016,%function
nc_arm64_mb_memory_016:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_016_done
    cbz x1, .L_nc_arm64_mb_memory_016_scalar
.L_nc_arm64_mb_memory_016_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_016_loop
    b .L_nc_arm64_mb_memory_016_done
.L_nc_arm64_mb_memory_016_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_016_scalar
.L_nc_arm64_mb_memory_016_done:
    ret

.global nc_arm64_mb_memory_017
.type nc_arm64_mb_memory_017,%function
nc_arm64_mb_memory_017:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_017_done
    cbz x1, .L_nc_arm64_mb_memory_017_scalar
.L_nc_arm64_mb_memory_017_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_017_loop
    b .L_nc_arm64_mb_memory_017_done
.L_nc_arm64_mb_memory_017_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_017_scalar
.L_nc_arm64_mb_memory_017_done:
    ret

.global nc_arm64_mb_memory_018
.type nc_arm64_mb_memory_018,%function
nc_arm64_mb_memory_018:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_018_done
    cbz x1, .L_nc_arm64_mb_memory_018_scalar
.L_nc_arm64_mb_memory_018_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_018_loop
    b .L_nc_arm64_mb_memory_018_done
.L_nc_arm64_mb_memory_018_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_018_scalar
.L_nc_arm64_mb_memory_018_done:
    ret

.global nc_arm64_mb_memory_019
.type nc_arm64_mb_memory_019,%function
nc_arm64_mb_memory_019:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_019_done
    cbz x1, .L_nc_arm64_mb_memory_019_scalar
.L_nc_arm64_mb_memory_019_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_019_loop
    b .L_nc_arm64_mb_memory_019_done
.L_nc_arm64_mb_memory_019_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_019_scalar
.L_nc_arm64_mb_memory_019_done:
    ret

.global nc_arm64_mb_memory_020
.type nc_arm64_mb_memory_020,%function
nc_arm64_mb_memory_020:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_020_done
    cbz x1, .L_nc_arm64_mb_memory_020_scalar
.L_nc_arm64_mb_memory_020_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_020_loop
    b .L_nc_arm64_mb_memory_020_done
.L_nc_arm64_mb_memory_020_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_020_scalar
.L_nc_arm64_mb_memory_020_done:
    ret

.global nc_arm64_mb_memory_021
.type nc_arm64_mb_memory_021,%function
nc_arm64_mb_memory_021:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_021_done
    cbz x1, .L_nc_arm64_mb_memory_021_scalar
.L_nc_arm64_mb_memory_021_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_021_loop
    b .L_nc_arm64_mb_memory_021_done
.L_nc_arm64_mb_memory_021_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_021_scalar
.L_nc_arm64_mb_memory_021_done:
    ret

.global nc_arm64_mb_memory_022
.type nc_arm64_mb_memory_022,%function
nc_arm64_mb_memory_022:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_022_done
    cbz x1, .L_nc_arm64_mb_memory_022_scalar
.L_nc_arm64_mb_memory_022_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_022_loop
    b .L_nc_arm64_mb_memory_022_done
.L_nc_arm64_mb_memory_022_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_022_scalar
.L_nc_arm64_mb_memory_022_done:
    ret

.global nc_arm64_mb_memory_023
.type nc_arm64_mb_memory_023,%function
nc_arm64_mb_memory_023:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_023_done
    cbz x1, .L_nc_arm64_mb_memory_023_scalar
.L_nc_arm64_mb_memory_023_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_023_loop
    b .L_nc_arm64_mb_memory_023_done
.L_nc_arm64_mb_memory_023_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_023_scalar
.L_nc_arm64_mb_memory_023_done:
    ret

.global nc_arm64_mb_memory_024
.type nc_arm64_mb_memory_024,%function
nc_arm64_mb_memory_024:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_024_done
    cbz x1, .L_nc_arm64_mb_memory_024_scalar
.L_nc_arm64_mb_memory_024_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_024_loop
    b .L_nc_arm64_mb_memory_024_done
.L_nc_arm64_mb_memory_024_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_024_scalar
.L_nc_arm64_mb_memory_024_done:
    ret

.global nc_arm64_mb_memory_025
.type nc_arm64_mb_memory_025,%function
nc_arm64_mb_memory_025:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_025_done
    cbz x1, .L_nc_arm64_mb_memory_025_scalar
.L_nc_arm64_mb_memory_025_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_025_loop
    b .L_nc_arm64_mb_memory_025_done
.L_nc_arm64_mb_memory_025_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_025_scalar
.L_nc_arm64_mb_memory_025_done:
    ret

.global nc_arm64_mb_memory_026
.type nc_arm64_mb_memory_026,%function
nc_arm64_mb_memory_026:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_026_done
    cbz x1, .L_nc_arm64_mb_memory_026_scalar
.L_nc_arm64_mb_memory_026_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_026_loop
    b .L_nc_arm64_mb_memory_026_done
.L_nc_arm64_mb_memory_026_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_026_scalar
.L_nc_arm64_mb_memory_026_done:
    ret

.global nc_arm64_mb_memory_027
.type nc_arm64_mb_memory_027,%function
nc_arm64_mb_memory_027:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_027_done
    cbz x1, .L_nc_arm64_mb_memory_027_scalar
.L_nc_arm64_mb_memory_027_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_027_loop
    b .L_nc_arm64_mb_memory_027_done
.L_nc_arm64_mb_memory_027_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_027_scalar
.L_nc_arm64_mb_memory_027_done:
    ret

.global nc_arm64_mb_memory_028
.type nc_arm64_mb_memory_028,%function
nc_arm64_mb_memory_028:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_028_done
    cbz x1, .L_nc_arm64_mb_memory_028_scalar
.L_nc_arm64_mb_memory_028_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_028_loop
    b .L_nc_arm64_mb_memory_028_done
.L_nc_arm64_mb_memory_028_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_028_scalar
.L_nc_arm64_mb_memory_028_done:
    ret

.global nc_arm64_mb_memory_029
.type nc_arm64_mb_memory_029,%function
nc_arm64_mb_memory_029:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_029_done
    cbz x1, .L_nc_arm64_mb_memory_029_scalar
.L_nc_arm64_mb_memory_029_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_029_loop
    b .L_nc_arm64_mb_memory_029_done
.L_nc_arm64_mb_memory_029_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_029_scalar
.L_nc_arm64_mb_memory_029_done:
    ret

.global nc_arm64_mb_memory_030
.type nc_arm64_mb_memory_030,%function
nc_arm64_mb_memory_030:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_030_done
    cbz x1, .L_nc_arm64_mb_memory_030_scalar
.L_nc_arm64_mb_memory_030_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_030_loop
    b .L_nc_arm64_mb_memory_030_done
.L_nc_arm64_mb_memory_030_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_030_scalar
.L_nc_arm64_mb_memory_030_done:
    ret

.global nc_arm64_mb_memory_031
.type nc_arm64_mb_memory_031,%function
nc_arm64_mb_memory_031:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_031_done
    cbz x1, .L_nc_arm64_mb_memory_031_scalar
.L_nc_arm64_mb_memory_031_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_031_loop
    b .L_nc_arm64_mb_memory_031_done
.L_nc_arm64_mb_memory_031_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_031_scalar
.L_nc_arm64_mb_memory_031_done:
    ret

.global nc_arm64_mb_memory_032
.type nc_arm64_mb_memory_032,%function
nc_arm64_mb_memory_032:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_032_done
    cbz x1, .L_nc_arm64_mb_memory_032_scalar
.L_nc_arm64_mb_memory_032_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_032_loop
    b .L_nc_arm64_mb_memory_032_done
.L_nc_arm64_mb_memory_032_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_032_scalar
.L_nc_arm64_mb_memory_032_done:
    ret

.global nc_arm64_mb_memory_033
.type nc_arm64_mb_memory_033,%function
nc_arm64_mb_memory_033:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_033_done
    cbz x1, .L_nc_arm64_mb_memory_033_scalar
.L_nc_arm64_mb_memory_033_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_033_loop
    b .L_nc_arm64_mb_memory_033_done
.L_nc_arm64_mb_memory_033_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_033_scalar
.L_nc_arm64_mb_memory_033_done:
    ret

.global nc_arm64_mb_memory_034
.type nc_arm64_mb_memory_034,%function
nc_arm64_mb_memory_034:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_034_done
    cbz x1, .L_nc_arm64_mb_memory_034_scalar
.L_nc_arm64_mb_memory_034_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_034_loop
    b .L_nc_arm64_mb_memory_034_done
.L_nc_arm64_mb_memory_034_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_034_scalar
.L_nc_arm64_mb_memory_034_done:
    ret

.global nc_arm64_mb_memory_035
.type nc_arm64_mb_memory_035,%function
nc_arm64_mb_memory_035:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_035_done
    cbz x1, .L_nc_arm64_mb_memory_035_scalar
.L_nc_arm64_mb_memory_035_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_035_loop
    b .L_nc_arm64_mb_memory_035_done
.L_nc_arm64_mb_memory_035_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_035_scalar
.L_nc_arm64_mb_memory_035_done:
    ret

.global nc_arm64_mb_memory_036
.type nc_arm64_mb_memory_036,%function
nc_arm64_mb_memory_036:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_036_done
    cbz x1, .L_nc_arm64_mb_memory_036_scalar
.L_nc_arm64_mb_memory_036_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_036_loop
    b .L_nc_arm64_mb_memory_036_done
.L_nc_arm64_mb_memory_036_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_036_scalar
.L_nc_arm64_mb_memory_036_done:
    ret

.global nc_arm64_mb_memory_037
.type nc_arm64_mb_memory_037,%function
nc_arm64_mb_memory_037:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_037_done
    cbz x1, .L_nc_arm64_mb_memory_037_scalar
.L_nc_arm64_mb_memory_037_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_037_loop
    b .L_nc_arm64_mb_memory_037_done
.L_nc_arm64_mb_memory_037_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_037_scalar
.L_nc_arm64_mb_memory_037_done:
    ret

.global nc_arm64_mb_memory_038
.type nc_arm64_mb_memory_038,%function
nc_arm64_mb_memory_038:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_038_done
    cbz x1, .L_nc_arm64_mb_memory_038_scalar
.L_nc_arm64_mb_memory_038_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_038_loop
    b .L_nc_arm64_mb_memory_038_done
.L_nc_arm64_mb_memory_038_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_038_scalar
.L_nc_arm64_mb_memory_038_done:
    ret

.global nc_arm64_mb_memory_039
.type nc_arm64_mb_memory_039,%function
nc_arm64_mb_memory_039:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_039_done
    cbz x1, .L_nc_arm64_mb_memory_039_scalar
.L_nc_arm64_mb_memory_039_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_039_loop
    b .L_nc_arm64_mb_memory_039_done
.L_nc_arm64_mb_memory_039_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_039_scalar
.L_nc_arm64_mb_memory_039_done:
    ret

.global nc_arm64_mb_memory_040
.type nc_arm64_mb_memory_040,%function
nc_arm64_mb_memory_040:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_040_done
    cbz x1, .L_nc_arm64_mb_memory_040_scalar
.L_nc_arm64_mb_memory_040_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_040_loop
    b .L_nc_arm64_mb_memory_040_done
.L_nc_arm64_mb_memory_040_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_040_scalar
.L_nc_arm64_mb_memory_040_done:
    ret

.global nc_arm64_mb_memory_041
.type nc_arm64_mb_memory_041,%function
nc_arm64_mb_memory_041:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_041_done
    cbz x1, .L_nc_arm64_mb_memory_041_scalar
.L_nc_arm64_mb_memory_041_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_041_loop
    b .L_nc_arm64_mb_memory_041_done
.L_nc_arm64_mb_memory_041_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_041_scalar
.L_nc_arm64_mb_memory_041_done:
    ret

.global nc_arm64_mb_memory_042
.type nc_arm64_mb_memory_042,%function
nc_arm64_mb_memory_042:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_042_done
    cbz x1, .L_nc_arm64_mb_memory_042_scalar
.L_nc_arm64_mb_memory_042_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_042_loop
    b .L_nc_arm64_mb_memory_042_done
.L_nc_arm64_mb_memory_042_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_042_scalar
.L_nc_arm64_mb_memory_042_done:
    ret

.global nc_arm64_mb_memory_043
.type nc_arm64_mb_memory_043,%function
nc_arm64_mb_memory_043:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_043_done
    cbz x1, .L_nc_arm64_mb_memory_043_scalar
.L_nc_arm64_mb_memory_043_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_043_loop
    b .L_nc_arm64_mb_memory_043_done
.L_nc_arm64_mb_memory_043_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_043_scalar
.L_nc_arm64_mb_memory_043_done:
    ret

.global nc_arm64_mb_memory_044
.type nc_arm64_mb_memory_044,%function
nc_arm64_mb_memory_044:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_044_done
    cbz x1, .L_nc_arm64_mb_memory_044_scalar
.L_nc_arm64_mb_memory_044_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_044_loop
    b .L_nc_arm64_mb_memory_044_done
.L_nc_arm64_mb_memory_044_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_044_scalar
.L_nc_arm64_mb_memory_044_done:
    ret

.global nc_arm64_mb_memory_045
.type nc_arm64_mb_memory_045,%function
nc_arm64_mb_memory_045:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_045_done
    cbz x1, .L_nc_arm64_mb_memory_045_scalar
.L_nc_arm64_mb_memory_045_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_045_loop
    b .L_nc_arm64_mb_memory_045_done
.L_nc_arm64_mb_memory_045_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_045_scalar
.L_nc_arm64_mb_memory_045_done:
    ret

.global nc_arm64_mb_memory_046
.type nc_arm64_mb_memory_046,%function
nc_arm64_mb_memory_046:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_046_done
    cbz x1, .L_nc_arm64_mb_memory_046_scalar
.L_nc_arm64_mb_memory_046_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_046_loop
    b .L_nc_arm64_mb_memory_046_done
.L_nc_arm64_mb_memory_046_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_046_scalar
.L_nc_arm64_mb_memory_046_done:
    ret

.global nc_arm64_mb_memory_047
.type nc_arm64_mb_memory_047,%function
nc_arm64_mb_memory_047:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_047_done
    cbz x1, .L_nc_arm64_mb_memory_047_scalar
.L_nc_arm64_mb_memory_047_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_047_loop
    b .L_nc_arm64_mb_memory_047_done
.L_nc_arm64_mb_memory_047_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_047_scalar
.L_nc_arm64_mb_memory_047_done:
    ret

.global nc_arm64_mb_memory_048
.type nc_arm64_mb_memory_048,%function
nc_arm64_mb_memory_048:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_048_done
    cbz x1, .L_nc_arm64_mb_memory_048_scalar
.L_nc_arm64_mb_memory_048_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_048_loop
    b .L_nc_arm64_mb_memory_048_done
.L_nc_arm64_mb_memory_048_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_048_scalar
.L_nc_arm64_mb_memory_048_done:
    ret

.global nc_arm64_mb_memory_049
.type nc_arm64_mb_memory_049,%function
nc_arm64_mb_memory_049:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_memory_049_done
    cbz x1, .L_nc_arm64_mb_memory_049_scalar
.L_nc_arm64_mb_memory_049_loop:
    ldrb w4, [x1]
    eor x0, x0, x4
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_049_loop
    b .L_nc_arm64_mb_memory_049_done
.L_nc_arm64_mb_memory_049_scalar:
    add x0, x0, x3
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_memory_049_scalar
.L_nc_arm64_mb_memory_049_done:
    ret

.global nc_arm64_mb_neon_000
.type nc_arm64_mb_neon_000,%function
nc_arm64_mb_neon_000:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_000_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_000_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_000_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_000_done:
    ret

.global nc_arm64_mb_neon_001
.type nc_arm64_mb_neon_001,%function
nc_arm64_mb_neon_001:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_001_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_001_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_001_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_001_done:
    ret

.global nc_arm64_mb_neon_002
.type nc_arm64_mb_neon_002,%function
nc_arm64_mb_neon_002:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_002_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_002_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_002_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_002_done:
    ret

.global nc_arm64_mb_neon_003
.type nc_arm64_mb_neon_003,%function
nc_arm64_mb_neon_003:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_003_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_003_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_003_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_003_done:
    ret

.global nc_arm64_mb_neon_004
.type nc_arm64_mb_neon_004,%function
nc_arm64_mb_neon_004:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_004_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_004_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_004_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_004_done:
    ret

.global nc_arm64_mb_neon_005
.type nc_arm64_mb_neon_005,%function
nc_arm64_mb_neon_005:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_005_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_005_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_005_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_005_done:
    ret

.global nc_arm64_mb_neon_006
.type nc_arm64_mb_neon_006,%function
nc_arm64_mb_neon_006:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_006_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_006_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_006_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_006_done:
    ret

.global nc_arm64_mb_neon_007
.type nc_arm64_mb_neon_007,%function
nc_arm64_mb_neon_007:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_007_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_007_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_007_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_007_done:
    ret

.global nc_arm64_mb_neon_008
.type nc_arm64_mb_neon_008,%function
nc_arm64_mb_neon_008:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_008_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_008_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_008_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_008_done:
    ret

.global nc_arm64_mb_neon_009
.type nc_arm64_mb_neon_009,%function
nc_arm64_mb_neon_009:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_009_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_009_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_009_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_009_done:
    ret

.global nc_arm64_mb_neon_010
.type nc_arm64_mb_neon_010,%function
nc_arm64_mb_neon_010:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_010_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_010_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_010_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_010_done:
    ret

.global nc_arm64_mb_neon_011
.type nc_arm64_mb_neon_011,%function
nc_arm64_mb_neon_011:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_011_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_011_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_011_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_011_done:
    ret

.global nc_arm64_mb_neon_012
.type nc_arm64_mb_neon_012,%function
nc_arm64_mb_neon_012:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_012_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_012_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_012_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_012_done:
    ret

.global nc_arm64_mb_neon_013
.type nc_arm64_mb_neon_013,%function
nc_arm64_mb_neon_013:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_013_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_013_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_013_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_013_done:
    ret

.global nc_arm64_mb_neon_014
.type nc_arm64_mb_neon_014,%function
nc_arm64_mb_neon_014:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_014_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_014_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_014_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_014_done:
    ret

.global nc_arm64_mb_neon_015
.type nc_arm64_mb_neon_015,%function
nc_arm64_mb_neon_015:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_015_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_015_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_015_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_015_done:
    ret

.global nc_arm64_mb_neon_016
.type nc_arm64_mb_neon_016,%function
nc_arm64_mb_neon_016:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_016_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_016_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_016_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_016_done:
    ret

.global nc_arm64_mb_neon_017
.type nc_arm64_mb_neon_017,%function
nc_arm64_mb_neon_017:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_017_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_017_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_017_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_017_done:
    ret

.global nc_arm64_mb_neon_018
.type nc_arm64_mb_neon_018,%function
nc_arm64_mb_neon_018:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_018_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_018_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_018_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_018_done:
    ret

.global nc_arm64_mb_neon_019
.type nc_arm64_mb_neon_019,%function
nc_arm64_mb_neon_019:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_019_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_019_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_019_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_019_done:
    ret

.global nc_arm64_mb_neon_020
.type nc_arm64_mb_neon_020,%function
nc_arm64_mb_neon_020:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_020_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_020_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_020_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_020_done:
    ret

.global nc_arm64_mb_neon_021
.type nc_arm64_mb_neon_021,%function
nc_arm64_mb_neon_021:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_021_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_021_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_021_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_021_done:
    ret

.global nc_arm64_mb_neon_022
.type nc_arm64_mb_neon_022,%function
nc_arm64_mb_neon_022:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_022_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_022_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_022_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_022_done:
    ret

.global nc_arm64_mb_neon_023
.type nc_arm64_mb_neon_023,%function
nc_arm64_mb_neon_023:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_023_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_023_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_023_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_023_done:
    ret

.global nc_arm64_mb_neon_024
.type nc_arm64_mb_neon_024,%function
nc_arm64_mb_neon_024:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_024_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_024_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_024_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_024_done:
    ret

.global nc_arm64_mb_neon_025
.type nc_arm64_mb_neon_025,%function
nc_arm64_mb_neon_025:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_025_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_025_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_025_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_025_done:
    ret

.global nc_arm64_mb_neon_026
.type nc_arm64_mb_neon_026,%function
nc_arm64_mb_neon_026:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_026_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_026_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_026_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_026_done:
    ret

.global nc_arm64_mb_neon_027
.type nc_arm64_mb_neon_027,%function
nc_arm64_mb_neon_027:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_027_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_027_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_027_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_027_done:
    ret

.global nc_arm64_mb_neon_028
.type nc_arm64_mb_neon_028,%function
nc_arm64_mb_neon_028:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_028_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_028_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_028_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_028_done:
    ret

.global nc_arm64_mb_neon_029
.type nc_arm64_mb_neon_029,%function
nc_arm64_mb_neon_029:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_029_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_029_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_029_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_029_done:
    ret

.global nc_arm64_mb_neon_030
.type nc_arm64_mb_neon_030,%function
nc_arm64_mb_neon_030:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_030_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_030_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_030_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_030_done:
    ret

.global nc_arm64_mb_neon_031
.type nc_arm64_mb_neon_031,%function
nc_arm64_mb_neon_031:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_031_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_031_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_031_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_031_done:
    ret

.global nc_arm64_mb_neon_032
.type nc_arm64_mb_neon_032,%function
nc_arm64_mb_neon_032:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_032_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_032_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_032_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_032_done:
    ret

.global nc_arm64_mb_neon_033
.type nc_arm64_mb_neon_033,%function
nc_arm64_mb_neon_033:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_033_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_033_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_033_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_033_done:
    ret

.global nc_arm64_mb_neon_034
.type nc_arm64_mb_neon_034,%function
nc_arm64_mb_neon_034:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_034_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_034_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_034_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_034_done:
    ret

.global nc_arm64_mb_neon_035
.type nc_arm64_mb_neon_035,%function
nc_arm64_mb_neon_035:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_035_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_035_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_035_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_035_done:
    ret

.global nc_arm64_mb_neon_036
.type nc_arm64_mb_neon_036,%function
nc_arm64_mb_neon_036:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_036_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_036_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_036_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_036_done:
    ret

.global nc_arm64_mb_neon_037
.type nc_arm64_mb_neon_037,%function
nc_arm64_mb_neon_037:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_037_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_037_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_037_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_037_done:
    ret

.global nc_arm64_mb_neon_038
.type nc_arm64_mb_neon_038,%function
nc_arm64_mb_neon_038:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_038_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_038_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_038_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_038_done:
    ret

.global nc_arm64_mb_neon_039
.type nc_arm64_mb_neon_039,%function
nc_arm64_mb_neon_039:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_039_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_039_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_039_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_039_done:
    ret

.global nc_arm64_mb_neon_040
.type nc_arm64_mb_neon_040,%function
nc_arm64_mb_neon_040:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_040_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_040_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_040_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_040_done:
    ret

.global nc_arm64_mb_neon_041
.type nc_arm64_mb_neon_041,%function
nc_arm64_mb_neon_041:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_041_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_041_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_041_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_041_done:
    ret

.global nc_arm64_mb_neon_042
.type nc_arm64_mb_neon_042,%function
nc_arm64_mb_neon_042:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_042_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_042_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_042_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_042_done:
    ret

.global nc_arm64_mb_neon_043
.type nc_arm64_mb_neon_043,%function
nc_arm64_mb_neon_043:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_043_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_043_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_043_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_043_done:
    ret

.global nc_arm64_mb_neon_044
.type nc_arm64_mb_neon_044,%function
nc_arm64_mb_neon_044:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_044_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_044_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_044_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_044_done:
    ret

.global nc_arm64_mb_neon_045
.type nc_arm64_mb_neon_045,%function
nc_arm64_mb_neon_045:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_045_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_045_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_045_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_045_done:
    ret

.global nc_arm64_mb_neon_046
.type nc_arm64_mb_neon_046,%function
nc_arm64_mb_neon_046:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_046_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_046_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_046_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_046_done:
    ret

.global nc_arm64_mb_neon_047
.type nc_arm64_mb_neon_047,%function
nc_arm64_mb_neon_047:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_047_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_047_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_047_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_047_done:
    ret

.global nc_arm64_mb_neon_048
.type nc_arm64_mb_neon_048,%function
nc_arm64_mb_neon_048:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_048_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_048_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_048_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_048_done:
    ret

.global nc_arm64_mb_neon_049
.type nc_arm64_mb_neon_049,%function
nc_arm64_mb_neon_049:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_neon_049_done
    movi v0.2d, #0
    mov x16, #1
    dup v1.2d, x16
.L_nc_arm64_mb_neon_049_loop:
    add v0.2d, v0.2d, v1.2d
    eor v0.16b, v0.16b, v1.16b
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_neon_049_loop
    fmov x0, d0
.L_nc_arm64_mb_neon_049_done:
    ret

.global nc_arm64_mb_branch_000
.type nc_arm64_mb_branch_000,%function
nc_arm64_mb_branch_000:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_000_done
.L_nc_arm64_mb_branch_000_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_000_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_000_next
.L_nc_arm64_mb_branch_000_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_000_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_000_loop
.L_nc_arm64_mb_branch_000_done:
    ret

.global nc_arm64_mb_branch_001
.type nc_arm64_mb_branch_001,%function
nc_arm64_mb_branch_001:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_001_done
.L_nc_arm64_mb_branch_001_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_001_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_001_next
.L_nc_arm64_mb_branch_001_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_001_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_001_loop
.L_nc_arm64_mb_branch_001_done:
    ret

.global nc_arm64_mb_branch_002
.type nc_arm64_mb_branch_002,%function
nc_arm64_mb_branch_002:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_002_done
.L_nc_arm64_mb_branch_002_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_002_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_002_next
.L_nc_arm64_mb_branch_002_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_002_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_002_loop
.L_nc_arm64_mb_branch_002_done:
    ret

.global nc_arm64_mb_branch_003
.type nc_arm64_mb_branch_003,%function
nc_arm64_mb_branch_003:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_003_done
.L_nc_arm64_mb_branch_003_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_003_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_003_next
.L_nc_arm64_mb_branch_003_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_003_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_003_loop
.L_nc_arm64_mb_branch_003_done:
    ret

.global nc_arm64_mb_branch_004
.type nc_arm64_mb_branch_004,%function
nc_arm64_mb_branch_004:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_004_done
.L_nc_arm64_mb_branch_004_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_004_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_004_next
.L_nc_arm64_mb_branch_004_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_004_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_004_loop
.L_nc_arm64_mb_branch_004_done:
    ret

.global nc_arm64_mb_branch_005
.type nc_arm64_mb_branch_005,%function
nc_arm64_mb_branch_005:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_005_done
.L_nc_arm64_mb_branch_005_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_005_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_005_next
.L_nc_arm64_mb_branch_005_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_005_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_005_loop
.L_nc_arm64_mb_branch_005_done:
    ret

.global nc_arm64_mb_branch_006
.type nc_arm64_mb_branch_006,%function
nc_arm64_mb_branch_006:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_006_done
.L_nc_arm64_mb_branch_006_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_006_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_006_next
.L_nc_arm64_mb_branch_006_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_006_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_006_loop
.L_nc_arm64_mb_branch_006_done:
    ret

.global nc_arm64_mb_branch_007
.type nc_arm64_mb_branch_007,%function
nc_arm64_mb_branch_007:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_007_done
.L_nc_arm64_mb_branch_007_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_007_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_007_next
.L_nc_arm64_mb_branch_007_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_007_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_007_loop
.L_nc_arm64_mb_branch_007_done:
    ret

.global nc_arm64_mb_branch_008
.type nc_arm64_mb_branch_008,%function
nc_arm64_mb_branch_008:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_008_done
.L_nc_arm64_mb_branch_008_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_008_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_008_next
.L_nc_arm64_mb_branch_008_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_008_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_008_loop
.L_nc_arm64_mb_branch_008_done:
    ret

.global nc_arm64_mb_branch_009
.type nc_arm64_mb_branch_009,%function
nc_arm64_mb_branch_009:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_009_done
.L_nc_arm64_mb_branch_009_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_009_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_009_next
.L_nc_arm64_mb_branch_009_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_009_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_009_loop
.L_nc_arm64_mb_branch_009_done:
    ret

.global nc_arm64_mb_branch_010
.type nc_arm64_mb_branch_010,%function
nc_arm64_mb_branch_010:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_010_done
.L_nc_arm64_mb_branch_010_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_010_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_010_next
.L_nc_arm64_mb_branch_010_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_010_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_010_loop
.L_nc_arm64_mb_branch_010_done:
    ret

.global nc_arm64_mb_branch_011
.type nc_arm64_mb_branch_011,%function
nc_arm64_mb_branch_011:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_011_done
.L_nc_arm64_mb_branch_011_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_011_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_011_next
.L_nc_arm64_mb_branch_011_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_011_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_011_loop
.L_nc_arm64_mb_branch_011_done:
    ret

.global nc_arm64_mb_branch_012
.type nc_arm64_mb_branch_012,%function
nc_arm64_mb_branch_012:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_012_done
.L_nc_arm64_mb_branch_012_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_012_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_012_next
.L_nc_arm64_mb_branch_012_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_012_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_012_loop
.L_nc_arm64_mb_branch_012_done:
    ret

.global nc_arm64_mb_branch_013
.type nc_arm64_mb_branch_013,%function
nc_arm64_mb_branch_013:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_013_done
.L_nc_arm64_mb_branch_013_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_013_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_013_next
.L_nc_arm64_mb_branch_013_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_013_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_013_loop
.L_nc_arm64_mb_branch_013_done:
    ret

.global nc_arm64_mb_branch_014
.type nc_arm64_mb_branch_014,%function
nc_arm64_mb_branch_014:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_014_done
.L_nc_arm64_mb_branch_014_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_014_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_014_next
.L_nc_arm64_mb_branch_014_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_014_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_014_loop
.L_nc_arm64_mb_branch_014_done:
    ret

.global nc_arm64_mb_branch_015
.type nc_arm64_mb_branch_015,%function
nc_arm64_mb_branch_015:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_015_done
.L_nc_arm64_mb_branch_015_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_015_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_015_next
.L_nc_arm64_mb_branch_015_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_015_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_015_loop
.L_nc_arm64_mb_branch_015_done:
    ret

.global nc_arm64_mb_branch_016
.type nc_arm64_mb_branch_016,%function
nc_arm64_mb_branch_016:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_016_done
.L_nc_arm64_mb_branch_016_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_016_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_016_next
.L_nc_arm64_mb_branch_016_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_016_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_016_loop
.L_nc_arm64_mb_branch_016_done:
    ret

.global nc_arm64_mb_branch_017
.type nc_arm64_mb_branch_017,%function
nc_arm64_mb_branch_017:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_017_done
.L_nc_arm64_mb_branch_017_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_017_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_017_next
.L_nc_arm64_mb_branch_017_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_017_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_017_loop
.L_nc_arm64_mb_branch_017_done:
    ret

.global nc_arm64_mb_branch_018
.type nc_arm64_mb_branch_018,%function
nc_arm64_mb_branch_018:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_018_done
.L_nc_arm64_mb_branch_018_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_018_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_018_next
.L_nc_arm64_mb_branch_018_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_018_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_018_loop
.L_nc_arm64_mb_branch_018_done:
    ret

.global nc_arm64_mb_branch_019
.type nc_arm64_mb_branch_019,%function
nc_arm64_mb_branch_019:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_019_done
.L_nc_arm64_mb_branch_019_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_019_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_019_next
.L_nc_arm64_mb_branch_019_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_019_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_019_loop
.L_nc_arm64_mb_branch_019_done:
    ret

.global nc_arm64_mb_branch_020
.type nc_arm64_mb_branch_020,%function
nc_arm64_mb_branch_020:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_020_done
.L_nc_arm64_mb_branch_020_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_020_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_020_next
.L_nc_arm64_mb_branch_020_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_020_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_020_loop
.L_nc_arm64_mb_branch_020_done:
    ret

.global nc_arm64_mb_branch_021
.type nc_arm64_mb_branch_021,%function
nc_arm64_mb_branch_021:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_021_done
.L_nc_arm64_mb_branch_021_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_021_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_021_next
.L_nc_arm64_mb_branch_021_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_021_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_021_loop
.L_nc_arm64_mb_branch_021_done:
    ret

.global nc_arm64_mb_branch_022
.type nc_arm64_mb_branch_022,%function
nc_arm64_mb_branch_022:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_022_done
.L_nc_arm64_mb_branch_022_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_022_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_022_next
.L_nc_arm64_mb_branch_022_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_022_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_022_loop
.L_nc_arm64_mb_branch_022_done:
    ret

.global nc_arm64_mb_branch_023
.type nc_arm64_mb_branch_023,%function
nc_arm64_mb_branch_023:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_023_done
.L_nc_arm64_mb_branch_023_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_023_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_023_next
.L_nc_arm64_mb_branch_023_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_023_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_023_loop
.L_nc_arm64_mb_branch_023_done:
    ret

.global nc_arm64_mb_branch_024
.type nc_arm64_mb_branch_024,%function
nc_arm64_mb_branch_024:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_024_done
.L_nc_arm64_mb_branch_024_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_024_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_024_next
.L_nc_arm64_mb_branch_024_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_024_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_024_loop
.L_nc_arm64_mb_branch_024_done:
    ret

.global nc_arm64_mb_branch_025
.type nc_arm64_mb_branch_025,%function
nc_arm64_mb_branch_025:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_025_done
.L_nc_arm64_mb_branch_025_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_025_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_025_next
.L_nc_arm64_mb_branch_025_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_025_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_025_loop
.L_nc_arm64_mb_branch_025_done:
    ret

.global nc_arm64_mb_branch_026
.type nc_arm64_mb_branch_026,%function
nc_arm64_mb_branch_026:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_026_done
.L_nc_arm64_mb_branch_026_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_026_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_026_next
.L_nc_arm64_mb_branch_026_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_026_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_026_loop
.L_nc_arm64_mb_branch_026_done:
    ret

.global nc_arm64_mb_branch_027
.type nc_arm64_mb_branch_027,%function
nc_arm64_mb_branch_027:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_027_done
.L_nc_arm64_mb_branch_027_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_027_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_027_next
.L_nc_arm64_mb_branch_027_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_027_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_027_loop
.L_nc_arm64_mb_branch_027_done:
    ret

.global nc_arm64_mb_branch_028
.type nc_arm64_mb_branch_028,%function
nc_arm64_mb_branch_028:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_028_done
.L_nc_arm64_mb_branch_028_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_028_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_028_next
.L_nc_arm64_mb_branch_028_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_028_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_028_loop
.L_nc_arm64_mb_branch_028_done:
    ret

.global nc_arm64_mb_branch_029
.type nc_arm64_mb_branch_029,%function
nc_arm64_mb_branch_029:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_029_done
.L_nc_arm64_mb_branch_029_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_029_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_029_next
.L_nc_arm64_mb_branch_029_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_029_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_029_loop
.L_nc_arm64_mb_branch_029_done:
    ret

.global nc_arm64_mb_branch_030
.type nc_arm64_mb_branch_030,%function
nc_arm64_mb_branch_030:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_030_done
.L_nc_arm64_mb_branch_030_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_030_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_030_next
.L_nc_arm64_mb_branch_030_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_030_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_030_loop
.L_nc_arm64_mb_branch_030_done:
    ret

.global nc_arm64_mb_branch_031
.type nc_arm64_mb_branch_031,%function
nc_arm64_mb_branch_031:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_031_done
.L_nc_arm64_mb_branch_031_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_031_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_031_next
.L_nc_arm64_mb_branch_031_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_031_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_031_loop
.L_nc_arm64_mb_branch_031_done:
    ret

.global nc_arm64_mb_branch_032
.type nc_arm64_mb_branch_032,%function
nc_arm64_mb_branch_032:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_032_done
.L_nc_arm64_mb_branch_032_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_032_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_032_next
.L_nc_arm64_mb_branch_032_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_032_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_032_loop
.L_nc_arm64_mb_branch_032_done:
    ret

.global nc_arm64_mb_branch_033
.type nc_arm64_mb_branch_033,%function
nc_arm64_mb_branch_033:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_033_done
.L_nc_arm64_mb_branch_033_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_033_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_033_next
.L_nc_arm64_mb_branch_033_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_033_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_033_loop
.L_nc_arm64_mb_branch_033_done:
    ret

.global nc_arm64_mb_branch_034
.type nc_arm64_mb_branch_034,%function
nc_arm64_mb_branch_034:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_034_done
.L_nc_arm64_mb_branch_034_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_034_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_034_next
.L_nc_arm64_mb_branch_034_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_034_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_034_loop
.L_nc_arm64_mb_branch_034_done:
    ret

.global nc_arm64_mb_branch_035
.type nc_arm64_mb_branch_035,%function
nc_arm64_mb_branch_035:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_035_done
.L_nc_arm64_mb_branch_035_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_035_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_035_next
.L_nc_arm64_mb_branch_035_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_035_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_035_loop
.L_nc_arm64_mb_branch_035_done:
    ret

.global nc_arm64_mb_branch_036
.type nc_arm64_mb_branch_036,%function
nc_arm64_mb_branch_036:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_036_done
.L_nc_arm64_mb_branch_036_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_036_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_036_next
.L_nc_arm64_mb_branch_036_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_036_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_036_loop
.L_nc_arm64_mb_branch_036_done:
    ret

.global nc_arm64_mb_branch_037
.type nc_arm64_mb_branch_037,%function
nc_arm64_mb_branch_037:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_037_done
.L_nc_arm64_mb_branch_037_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_037_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_037_next
.L_nc_arm64_mb_branch_037_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_037_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_037_loop
.L_nc_arm64_mb_branch_037_done:
    ret

.global nc_arm64_mb_branch_038
.type nc_arm64_mb_branch_038,%function
nc_arm64_mb_branch_038:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_038_done
.L_nc_arm64_mb_branch_038_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_038_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_038_next
.L_nc_arm64_mb_branch_038_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_038_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_038_loop
.L_nc_arm64_mb_branch_038_done:
    ret

.global nc_arm64_mb_branch_039
.type nc_arm64_mb_branch_039,%function
nc_arm64_mb_branch_039:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_039_done
.L_nc_arm64_mb_branch_039_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_039_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_039_next
.L_nc_arm64_mb_branch_039_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_039_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_039_loop
.L_nc_arm64_mb_branch_039_done:
    ret

.global nc_arm64_mb_branch_040
.type nc_arm64_mb_branch_040,%function
nc_arm64_mb_branch_040:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_040_done
.L_nc_arm64_mb_branch_040_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_040_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_040_next
.L_nc_arm64_mb_branch_040_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_040_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_040_loop
.L_nc_arm64_mb_branch_040_done:
    ret

.global nc_arm64_mb_branch_041
.type nc_arm64_mb_branch_041,%function
nc_arm64_mb_branch_041:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_041_done
.L_nc_arm64_mb_branch_041_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_041_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_041_next
.L_nc_arm64_mb_branch_041_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_041_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_041_loop
.L_nc_arm64_mb_branch_041_done:
    ret

.global nc_arm64_mb_branch_042
.type nc_arm64_mb_branch_042,%function
nc_arm64_mb_branch_042:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_042_done
.L_nc_arm64_mb_branch_042_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_042_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_042_next
.L_nc_arm64_mb_branch_042_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_042_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_042_loop
.L_nc_arm64_mb_branch_042_done:
    ret

.global nc_arm64_mb_branch_043
.type nc_arm64_mb_branch_043,%function
nc_arm64_mb_branch_043:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_043_done
.L_nc_arm64_mb_branch_043_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_043_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_043_next
.L_nc_arm64_mb_branch_043_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_043_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_043_loop
.L_nc_arm64_mb_branch_043_done:
    ret

.global nc_arm64_mb_branch_044
.type nc_arm64_mb_branch_044,%function
nc_arm64_mb_branch_044:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_044_done
.L_nc_arm64_mb_branch_044_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_044_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_044_next
.L_nc_arm64_mb_branch_044_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_044_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_044_loop
.L_nc_arm64_mb_branch_044_done:
    ret

.global nc_arm64_mb_branch_045
.type nc_arm64_mb_branch_045,%function
nc_arm64_mb_branch_045:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_045_done
.L_nc_arm64_mb_branch_045_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_045_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_045_next
.L_nc_arm64_mb_branch_045_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_045_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_045_loop
.L_nc_arm64_mb_branch_045_done:
    ret

.global nc_arm64_mb_branch_046
.type nc_arm64_mb_branch_046,%function
nc_arm64_mb_branch_046:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_046_done
.L_nc_arm64_mb_branch_046_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_046_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_046_next
.L_nc_arm64_mb_branch_046_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_046_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_046_loop
.L_nc_arm64_mb_branch_046_done:
    ret

.global nc_arm64_mb_branch_047
.type nc_arm64_mb_branch_047,%function
nc_arm64_mb_branch_047:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_047_done
.L_nc_arm64_mb_branch_047_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_047_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_047_next
.L_nc_arm64_mb_branch_047_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_047_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_047_loop
.L_nc_arm64_mb_branch_047_done:
    ret

.global nc_arm64_mb_branch_048
.type nc_arm64_mb_branch_048,%function
nc_arm64_mb_branch_048:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_048_done
.L_nc_arm64_mb_branch_048_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_048_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_048_next
.L_nc_arm64_mb_branch_048_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_048_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_048_loop
.L_nc_arm64_mb_branch_048_done:
    ret

.global nc_arm64_mb_branch_049
.type nc_arm64_mb_branch_049,%function
nc_arm64_mb_branch_049:
    mov x3, x0
    mov x0, #0
    cbz x3, .L_nc_arm64_mb_branch_049_done
.L_nc_arm64_mb_branch_049_loop:
    tbz x3, #0, .L_nc_arm64_mb_branch_049_even
    add x0, x0, #3
    b .L_nc_arm64_mb_branch_049_next
.L_nc_arm64_mb_branch_049_even:
    add x0, x0, #1
.L_nc_arm64_mb_branch_049_next:
    subs x3, x3, #1
    b.ne .L_nc_arm64_mb_branch_049_loop
.L_nc_arm64_mb_branch_049_done:
    ret

