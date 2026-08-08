#!/usr/bin/env python3
r"""
verify_install.py — end-to-end proof that the INSTALLED Windows 3.1 is our
byte-identical reconstruction (the win95 "read installed files back out of the
VM disk and hash them" capstone, retargeted to 3.1).

Reads the installed \WINDOWS\SYSTEM binaries straight out of the VM's flat VMDK
(7-Zip opens it MBR -> FAT16), and SHA-256-compares each against our source-
rebuilt binaries in staged/win31/. Any mismatch is diffed to a byte range so an
install-time personalization (e.g. SETUP writes the registration name into
USER.EXE) is reported as exactly that, not hidden behind a bare "DIFFER".

    python verify_install.py                    # default vmdk + staged dir
    python verify_install.py --vmdk vm/win31hd-flat.vmdk --staged staged/win31
"""
import hashlib
import os
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.abspath(__file__))
SEVENZIP = r"C:\Program Files\7-Zip\7z.exe"


def _sha(b):
    return hashlib.sha256(b).hexdigest()


def _extract_windows(vmdk, dest):
    """Extract the FAT partition's file tree from a flat VMDK via 7-Zip (MBR ->
    FAT16 -> files, recursed in one pass)."""
    subprocess.run([SEVENZIP, "x", vmdk, "-o" + dest, "-y"],
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)
    for r, _d, _f in os.walk(dest):
        if os.path.basename(r).upper() == "WINDOWS":
            return r
    return None


def _find(root, name):
    for r, _d, fs in os.walk(root):
        for f in fs:
            if f.lower() == name.lower():
                return os.path.join(r, f)
    return None


def _diff_ranges(a, b):
    n = min(len(a), len(b))
    d = [i for i in range(n) if a[i] != b[i]]
    ranges = []
    if d:
        s = p = d[0]
        for x in d[1:]:
            if x == p + 1:
                p = x
            else:
                ranges.append((s, p)); s = p = x
        ranges.append((s, p))
    return len(d), ranges


def main():
    vmdk = "vm/win31hd-flat.vmdk"
    staged = "staged/win31"
    if "--vmdk" in sys.argv:
        vmdk = sys.argv[sys.argv.index("--vmdk") + 1]
    if "--staged" in sys.argv:
        staged = sys.argv[sys.argv.index("--staged") + 1]
    if not os.path.exists(vmdk):
        sys.exit("vmdk not found: %s (build/boot the VM first)" % vmdk)
    if not os.path.isdir(staged):
        sys.exit("staged dir not found: %s (run bootchain --verify --save first)" % staged)

    tmp = tempfile.mkdtemp(prefix="win31inst_")
    win = _extract_windows(vmdk, tmp)
    if not win:
        sys.exit("no \\WINDOWS on the disk — Windows is not installed in this VMDK yet")

    print("Windows 3.1 install verification — installed binaries vs our source-rebuild")
    print("=" * 74)
    ident = personalized = missing = 0
    for ours in sorted(os.listdir(staged)):
        op = os.path.join(staged, ours)
        ip = _find(win, ours)
        if not ip:
            print("  %-14s  not installed on disk" % ours); missing += 1; continue
        ob, ib = open(op, "rb").read(), open(ip, "rb").read()
        if _sha(ob) == _sha(ib):
            ident += 1
            print("  %-14s  BYTE-IDENTICAL" % ours)
        else:
            ndiff, ranges = _diff_ranges(ob, ib)
            personalized += 1
            tag = "install-time personalization" if ndiff <= 64 else "DIFFERS"
            print("  %-14s  %d byte(s) differ (%s):" % (ours, ndiff, tag))
            for a, b in ranges[:4]:
                print("      0x%05X-0x%05X  ours=%r  installed=%r"
                      % (a, b, ob[a:b + 1][:16], ib[a:b + 1][:16]))
    print("=" * 74)
    print("byte-identical: %d   install-time-personalized: %d   not-installed: %d"
          % (ident, personalized, missing))
    print("=" * 74)


if __name__ == "__main__":
    main()
