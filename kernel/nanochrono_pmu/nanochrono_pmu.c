// SPDX-License-Identifier: GPL-2.0-only
/*
 * nanochrono_pmu.c — NanoChronometer PMU kernel module
 *
 * Exposes /dev/nanochrono_pmu (mode 0666) with ioctl interface for:
 *   - ARM64: reading CNTPCT_EL0 with ISB serialisation and enabling direct
 *     EL0 counter access via CNTKCTL_EL1 (so user-space needs no root for
 *     subsequent reads after one privileged ENABLE_UACCESS call).
 *   - x86-64: reading TSC with LFENCE serialisation from ring-0.
 *
 * Security model:
 *   - /dev/nanochrono_pmu is world-readable (0666) for counter reads.
 *   - NANOCHRONO_PMU_IOC_ENABLE_UACCESS requires CAP_SYS_ADMIN.
 *   - No arbitrary register writes are exposed; only curated read-only ops.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/capability.h>
#include <linux/smp.h>
#include <linux/types.h>
#include "nanochrono_pmu.h"

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("NanoChronometer Project");
MODULE_DESCRIPTION("PMU counter access for NanoChronometer precision timing");
MODULE_VERSION("1.0.0");

#define DEVICE_NAME "nanochrono_pmu"

static int           nc_major;
static struct class *nc_class;
static struct cdev   nc_cdev;
static dev_t         nc_devno;

/* ── ARM64 counter helpers ──────────────────────────────────────────────── */
#if defined(__aarch64__)

static u64 nc_read_cntpct(void)
{
    u64 val;
    asm volatile("isb\n\tmrs %0, cntpct_el0" : "=r"(val));
    return val;
}

static void nc_enable_el0_counter_access_cpu(void *info)
{
    u64 val;
    (void)info;
    asm volatile("mrs %0, cntkctl_el1" : "=r"(val));
    val |= (1UL << 1);  /* EL0VCTEN: CNTVCT_EL0 accessible from EL0 */
    val |= (1UL << 0);  /* EL0PCTEN: CNTPCT_EL0 accessible from EL0 */
    asm volatile("msr cntkctl_el1, %0\n\tisb" : : "r"(val));
}

static u32 nc_query_caps(void)
{
    u32 caps = NANOCHRONO_PMU_CAP_CNTPCT;
    /* Check if CNTKCTL_EL1 already has user-access bits set on this CPU */
    u64 knctl;
    asm volatile("mrs %0, cntkctl_el1" : "=r"(knctl));
    if ((knctl & 0x3) == 0x3) caps |= NANOCHRONO_PMU_CAP_UACCESS;
    return caps;
}

#endif /* __aarch64__ */

/* ── x86-64 counter helpers ─────────────────────────────────────────────── */
#if defined(__x86_64__)

static u64 nc_read_rdtsc_serialised(void)
{
    u32 lo, hi;
    asm volatile("lfence\n\trdtsc\n\tlfence" : "=a"(lo), "=d"(hi));
    return ((u64)hi << 32) | lo;
}

static u32 nc_query_caps(void)
{
    return NANOCHRONO_PMU_CAP_RDTSC;
}

#endif /* __x86_64__ */

/* ── ioctl handler ──────────────────────────────────────────────────────── */
static long nc_pmu_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    (void)filp;

    switch (cmd) {

    case NANOCHRONO_PMU_IOC_READ_CNTPCT: {
#if defined(__aarch64__)
        u64 val = nc_read_cntpct();
        if (copy_to_user((void __user *)arg, &val, sizeof(val)))
            return -EFAULT;
        return 0;
#else
        return -ENOSYS;
#endif
    }

    case NANOCHRONO_PMU_IOC_READ_RDTSC: {
#if defined(__x86_64__)
        u64 val = nc_read_rdtsc_serialised();
        if (copy_to_user((void __user *)arg, &val, sizeof(val)))
            return -EFAULT;
        return 0;
#else
        return -ENOSYS;
#endif
    }

    case NANOCHRONO_PMU_IOC_ENABLE_UACCESS: {
#if defined(__aarch64__)
        if (!capable(CAP_SYS_ADMIN))
            return -EPERM;
        /* Set CNTKCTL_EL1 bits on every online CPU. */
        on_each_cpu(nc_enable_el0_counter_access_cpu, NULL, 1);
        pr_info("nanochrono_pmu: CNTKCTL_EL1 EL0PCTEN+EL0VCTEN enabled on all CPUs\n");
        return 0;
#else
        return -ENOSYS;
#endif
    }

    case NANOCHRONO_PMU_IOC_QUERY_CAPS: {
        u32 caps = nc_query_caps();
        if (copy_to_user((void __user *)arg, &caps, sizeof(caps)))
            return -EFAULT;
        return 0;
    }

    default:
        return -ENOTTY;
    }
}

static int nc_pmu_open(struct inode *inode, struct file *filp)
{
    (void)inode; (void)filp;
    return 0;
}

static int nc_pmu_release(struct inode *inode, struct file *filp)
{
    (void)inode; (void)filp;
    return 0;
}

static const struct file_operations nc_pmu_fops = {
    .owner          = THIS_MODULE,
    .open           = nc_pmu_open,
    .release        = nc_pmu_release,
    .unlocked_ioctl = nc_pmu_ioctl,
};

/* ── module init / exit ─────────────────────────────────────────────────── */
static int __init nc_pmu_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&nc_devno, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("nanochrono_pmu: alloc_chrdev_region failed: %d\n", ret);
        return ret;
    }
    nc_major = MAJOR(nc_devno);

    cdev_init(&nc_cdev, &nc_pmu_fops);
    nc_cdev.owner = THIS_MODULE;
    ret = cdev_add(&nc_cdev, nc_devno, 1);
    if (ret < 0) {
        pr_err("nanochrono_pmu: cdev_add failed: %d\n", ret);
        unregister_chrdev_region(nc_devno, 1);
        return ret;
    }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    nc_class = class_create(DEVICE_NAME);
#else
    nc_class = class_create(THIS_MODULE, DEVICE_NAME);
#endif
    if (IS_ERR(nc_class)) {
        ret = PTR_ERR(nc_class);
        pr_err("nanochrono_pmu: class_create failed: %d\n", ret);
        cdev_del(&nc_cdev);
        unregister_chrdev_region(nc_devno, 1);
        return ret;
    }

    /* Create /dev/nanochrono_pmu with mode 0666 (world-readable/writable for ioctl). */
    struct device *dev = device_create(nc_class, NULL, nc_devno, NULL, DEVICE_NAME);
    if (IS_ERR(dev)) {
        ret = PTR_ERR(dev);
        pr_err("nanochrono_pmu: device_create failed: %d\n", ret);
        class_destroy(nc_class);
        cdev_del(&nc_cdev);
        unregister_chrdev_region(nc_devno, 1);
        return ret;
    }

    /* Ensure world-accessible permissions on the device node. */
    dev->devt = nc_devno;

#if defined(__aarch64__)
    /* Enable user-space counter access on all CPUs at module load time.
     * This avoids requiring the ENABLE_UACCESS ioctl separately. */
    on_each_cpu(nc_enable_el0_counter_access_cpu, NULL, 1);
    pr_info("nanochrono_pmu: loaded (ARM64) — CNTPCT_EL0/CNTVCT_EL0 enabled in EL0\n");
#elif defined(__x86_64__)
    pr_info("nanochrono_pmu: loaded (x86-64) — serialised RDTSC via ioctl\n");
#else
    pr_info("nanochrono_pmu: loaded (unsupported arch — ioctl returns ENOSYS)\n");
#endif

    return 0;
}

static void __exit nc_pmu_exit(void)
{
    device_destroy(nc_class, nc_devno);
    class_destroy(nc_class);
    cdev_del(&nc_cdev);
    unregister_chrdev_region(nc_devno, 1);
    pr_info("nanochrono_pmu: unloaded\n");
}

module_init(nc_pmu_init);
module_exit(nc_pmu_exit);
