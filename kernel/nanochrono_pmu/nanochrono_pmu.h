/*
 * nanochrono_pmu.h — ioctl interface shared between the kernel module and
 * user-space clients.  Include this header in both kernel and user-space code.
 */
#ifndef NANOCHRONO_PMU_H
#define NANOCHRONO_PMU_H

#ifdef __KERNEL__
#  include <linux/ioctl.h>
#  include <linux/types.h>
#else
#  include <stdint.h>
#  include <sys/ioctl.h>
typedef uint64_t __u64;
#endif

#define NANOCHRONO_PMU_IOC_MAGIC  'N'

/*
 * NANOCHRONO_PMU_IOC_READ_CNTPCT
 *   ARM64: read CNTPCT_EL0 (physical counter) with ISB serialisation.
 *   x86-64: ioctl returns 0; use NANOCHRONO_PMU_IOC_READ_RDTSC instead.
 *   Returns the 64-bit counter value via the ioctl arg pointer.
 */
#define NANOCHRONO_PMU_IOC_READ_CNTPCT  _IOR(NANOCHRONO_PMU_IOC_MAGIC, 1, __u64)

/*
 * NANOCHRONO_PMU_IOC_READ_RDTSC
 *   x86-64: read TSC with LFENCE serialisation from ring 0.
 *   ARM64: ioctl returns 0.
 */
#define NANOCHRONO_PMU_IOC_READ_RDTSC   _IOR(NANOCHRONO_PMU_IOC_MAGIC, 2, __u64)

/*
 * NANOCHRONO_PMU_IOC_ENABLE_UACCESS (ARM64 only)
 *   Sets CNTKCTL_EL1.EL0PCTEN and EL0VCTEN on all CPUs so that CNTPCT_EL0
 *   and CNTVCT_EL0 are directly readable from EL0 (user-space) without
 *   going through the kernel module for each read.
 *   Requires CAP_SYS_ADMIN (enforced in the ioctl handler).
 *   After this ioctl succeeds, user-space can read CNTPCT_EL0 directly
 *   and the kernel module is only needed at boot.
 */
#define NANOCHRONO_PMU_IOC_ENABLE_UACCESS  _IO(NANOCHRONO_PMU_IOC_MAGIC, 3)

/*
 * NANOCHRONO_PMU_IOC_QUERY_CAPS
 *   Returns a bitmask of available capabilities.
 */
#define NANOCHRONO_PMU_CAP_CNTPCT    (1u << 0)
#define NANOCHRONO_PMU_CAP_RDTSC     (1u << 1)
#define NANOCHRONO_PMU_CAP_UACCESS   (1u << 2)

#define NANOCHRONO_PMU_IOC_QUERY_CAPS  _IOR(NANOCHRONO_PMU_IOC_MAGIC, 4, uint32_t)

#endif /* NANOCHRONO_PMU_H */
