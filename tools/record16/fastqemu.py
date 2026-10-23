"""Imported by the Setup drivers: with QEMU_FAST=1, every QEMU they start is the 64-bit one with
Hyper-V acceleration (WHPX) -- 2.4x faster to the License page (68 s against 161 s, 2026-10-22)."""
import os
import subprocess
FAST = r"C:\Program Files\qemu\qemu-system-x86_64.exe"
if os.environ.get("QEMU_FAST") == "1":
    _real = subprocess.Popen

    def _popen(args, *a, **k):
        if args and str(args[0]).lower().endswith("qemu-system-i386.exe"):
            args = [FAST] + list(args[1:]) + ["-accel", "whpx"]
        return _real(args, *a, **k)
    subprocess.Popen = _popen
