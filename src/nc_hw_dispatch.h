#pragma once
#ifndef NC_HW_DISPATCH_H
#define NC_HW_DISPATCH_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Hardware counter dispatcher — function-pointer table.
 *
 * Each entry represents one concrete timing backend ranked by precision.
 * nc_hw_dispatch_select() iterates the table in priority order and returns
 * the first entry whose check() returns 1.
 *
 * Boolean invariants enforced per entry (De Morgan / absorption):
 *   - check() is a single boolean expression; no nested if-else chains.
 *   - Platform guards are compile-time (#if), availability is runtime.
 *   - Android path is strictly disjoint from the kernel-module path:
 *       cntpct_check : !nc_hw_is_android() && nc_hw_dev_pmu_ok()
 *       cntvct_check : nc_hw_is_android() && nc_hw_cntvct_ok()
 *     Absorption: if one branch is true the other is definitionally false.
 */
typedef struct nc_hw_entry {
    const char *(*name)(void);       /* human-readable backend identifier   */
    int         (*check)(void);      /* 1 = available, 0 = not              */
    uint64_t    (*run)(void);        /* read the counter (no ctx argument)  */
} nc_hw_entry_t;

/* Returns the best available entry, or the fallback (wall monotonic). Never NULL. */
const nc_hw_entry_t *nc_hw_dispatch_select(void);

/* Convenience wrappers used by nanochrono_runtime.c */
const char *nc_hw_selected_name(void);
int         nc_hw_is_android(void);
int         nc_hw_dev_pmu_ok(void);   /* /dev/nanochrono_pmu is readable  */
int         nc_hw_cntvct_ok(void);    /* CNTVCT_EL0 accessible in EL0     */

#ifdef __cplusplus
}
#endif
#endif /* NC_HW_DISPATCH_H */
