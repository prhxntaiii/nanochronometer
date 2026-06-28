#!/bin/sh
# load.sh — build (if needed) and load the nanochrono_pmu kernel module.
# Requires root (sudo) for insmod/modprobe.
# After loading, /dev/nanochrono_pmu is available to all users (mode 0666).
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
MODULE="nanochrono_pmu"
KO="${SCRIPT_DIR}/${MODULE}.ko"

# Build if the .ko is missing or sources are newer.
if [ ! -f "$KO" ] || [ "${SCRIPT_DIR}/${MODULE}.c" -nt "$KO" ]; then
    echo "[nanochrono_pmu] Building kernel module..."
    make -C "$SCRIPT_DIR"
fi

# Unload stale instance if present.
if lsmod | grep -q "^${MODULE}"; then
    echo "[nanochrono_pmu] Removing existing module..."
    sudo rmmod "$MODULE"
fi

echo "[nanochrono_pmu] Loading ${KO}..."
sudo insmod "$KO"

# Verify device node.
if [ -c "/dev/${MODULE}" ]; then
    echo "[nanochrono_pmu] /dev/${MODULE} ready ($(stat -c '%a' "/dev/${MODULE}") permissions)"
else
    echo "[nanochrono_pmu] WARNING: /dev/${MODULE} not found; check dmesg"
    exit 1
fi

# On ARM64: call ENABLE_UACCESS so subsequent user-space reads need no ioctl.
if uname -m | grep -q "aarch64"; then
    echo "[nanochrono_pmu] ARM64: enabling EL0 counter access via ioctl..."
    # A small C one-liner to call ENABLE_UACCESS (requires CAP_SYS_ADMIN).
    # If nanochrono_cli supports --enable-pmu-uaccess, use that instead.
    : # Module init already sets CNTKCTL_EL1; no extra step needed.
fi

echo "[nanochrono_pmu] Done."
