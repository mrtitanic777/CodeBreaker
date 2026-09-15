#!/usr/bin/env python3
r"""cabswap.py -- swap our reconstructed modules INTO the Win95 CD cabinets.

The Win95 CD ships its payload in a Quantum-compressed cabinet SET (WIN95_02..28.CAB +
PRECOPY*/MINI.CAB). Quantum is unreproducible, so we cannot rebuild a byte-identical CAB -- but
Win95 SETUP extracts MSZIP cabinets too. This tool, for each cabinet, keeps its EXACT file list
(so Setup's layout still finds every file in the cabinet it expects) and substitutes the bytes of
any file we have reconstructed BYTE-IDENTICAL, then repacks it as a functional MSZIP cabinet with
cabwrite. The result is a CD whose payload is progressively OUR files -- delivered by our Setup.

    wt --target win95 cabswap [--src DIR] [--out DIR]     # swap all cabs; report per-file provenance

--src = the CD tree (default corpus disc/src/WIN95); --out = where the swapped cabs are written
(default design/recon/cd_payload). Non-swapped files are re-packed unchanged (from the extracted
originals) so each cabinet stays complete.
"""
import os
import sys
import shutil
import tempfile
import subprocess

from src.core import paths
from src.targets.win95 import cab as cabmod, cabwrite


def _makecab(entries, out_cab):
    """Repack [(name, bytes)] into a VALID MSZIP cabinet using Windows makecab -- FDI/Setup and 7-Zip
    accept it with zero errors (cabwrite's hand-rolled MSZIP tripped strict readers). Falls back to
    cabwrite if makecab is unavailable."""
    tmp = tempfile.mkdtemp(prefix="mkcab_")
    try:
        for name, data in entries:
            with open(os.path.join(tmp, name), "wb") as f:
                f.write(data)
        cabname = os.path.basename(out_cab)
        ddf = os.path.join(tmp, "_c.ddf")
        with open(ddf, "w") as f:
            f.write(".OPTION EXPLICIT\n.Set CabinetNameTemplate=%s\n.Set DiskDirectory1=%s\n"
                    ".Set Cabinet=on\n.Set Compress=on\n.Set CompressionType=MSZIP\n.Set MaxDiskSize=0\n"
                    % (cabname, tmp))
            for name, _ in entries:
                f.write('"%s" "%s"\n' % (os.path.join(tmp, name), name))
        r = subprocess.run(["makecab.exe", "/F", ddf], cwd=tmp, capture_output=True, text=True)
        produced = os.path.join(tmp, cabname)
        if os.path.isfile(produced):
            shutil.move(produced, out_cab)
            return True
        cabwrite.write_cab(entries, out_cab)         # fallback
        return False
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


def _design_root():
    dr = paths.design_root()
    if os.path.isdir(os.path.join(dr, "corpus")):
        return dr
    sib = os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design")
    return sib if os.path.isdir(os.path.join(sib, "corpus")) else dr


def _our_modules():
    """Map UPPER filename -> path of our reconstructed BYTE-IDENTICAL build (recon _built + base)."""
    root = _design_root()
    recon = os.path.join(root, "recon")
    win95 = os.path.dirname(root)                       # .../Windows95
    m = {}

    def add(name, path):
        if os.path.isfile(path):
            m[name.upper()] = path
    for key, name in (("command", "COMMAND.COM"), ("himem", "HIMEM.SYS"), ("emm386", "EMM386.EXE"),
                      ("ifshlp", "IFSHLP.SYS"), ("setver", "SETVER.EXE"), ("ramdrive", "RAMDRIVE.SYS"),
                      ("mscdex", "MSCDEX.EXE"), ("fdisk", "FDISK.EXE"), ("format", "FORMAT.COM"),
                      ("iosys", "IO.SYS"), ("oemsetup", "OEMSETUP.EXE")):
        add(name, os.path.join(recon, key, "_built", key + ".bin"))
    add("LZ32.DLL", os.path.join(win95, "base", "lz32", "_build", "lz32.dll"))
    add("LZEXPAND.DLL", os.path.join(win95, "base", "lzexpand", "_build", "lzexpand.dll"))
    # the GUI Setup + mini-Windows core (whole-file byte-identical NE, from nerecon --neimage)
    for key, name in (("krnl386", "KRNL386.EXE"), ("gdi", "GDI.EXE"), ("user", "USER.EXE"),
                      ("setupx", "SETUPX.DLL"), ("suwin", "SUWIN.EXE")):
        add(name, os.path.join(recon, "ne", key, "_built", name.split(".")[0].lower()
                               + (".dll" if name.endswith(".DLL") else ".exe")))
    return m


def main(argv):
    src = os.path.join(_design_root(), "corpus", "disc", "src", "WIN95")
    out = os.path.join(_design_root(), "recon", "cd_payload")
    i = 0
    while i < len(argv):
        if argv[i] == "--src" and i + 1 < len(argv):
            src = argv[i + 1]; i += 1
        elif argv[i] == "--out" and i + 1 < len(argv):
            out = argv[i + 1]; i += 1
        elif argv[i] in ("-h", "--help"):
            print(__doc__); return 0
        i += 1

    ours = _our_modules()
    print("our reconstructed modules available to swap: %s" % ", ".join(sorted(ours)))
    os.makedirs(out, exist_ok=True)

    # extract the whole CD payload tree once (7z handles Quantum + the set spanning)
    exdir = tempfile.mkdtemp(prefix="cabx_")
    seven = cabmod.find_7zip()
    cabs = sorted(f for f in os.listdir(src) if f.lower().endswith(".cab"))
    # extract every cabinet's files into one pool (spanning-safe: extract each; 7z pulls continuations)
    pool = os.path.join(exdir, "pool")
    os.makedirs(pool, exist_ok=True)
    for cab in cabs:
        subprocess.run([seven, "x", "-y", "-o" + pool, os.path.join(src, cab)],
                       capture_output=True, text=True)

    swapped_total = {}
    for cab in cabs:
        info, files = cabmod.read_cab(os.path.join(src, cab))    # this cabinet's own file list
        if not files:
            shutil.copy2(os.path.join(src, cab), os.path.join(out, cab)); continue
        entries = []
        swapped = []
        for cf in files:
            name = cf.name
            srcf = ours.get(name.upper())
            # only swap when OUR file is the SAME module/version as the cabinet's (size identity).
            # Some cabinets carry a DIFFERENT build under the same name -- e.g. MINI.CAB holds the
            # smaller Setup mini-Windows KRNL386/GDI/USER, not the installed Win95 ones -- and swapping
            # our installed build in breaks Setup's mini-Windows ("KERNEL: Unable to initialize heap").
            if srcf and os.path.getsize(srcf) == cf.size:
                data = open(srcf, "rb").read(); swapped.append(name)
            else:
                pf = os.path.join(pool, name)
                if not os.path.isfile(pf):               # spanning/other; keep original cabinet
                    entries = None; break
                data = open(pf, "rb").read()
            entries.append((name, data))
        if entries is None:
            shutil.copy2(os.path.join(src, cab), os.path.join(out, cab))
            print("  %-14s copied unchanged (unextractable member)" % cab)
            continue
        if not swapped:                              # nothing of ours here -> keep the original CAB
            shutil.copy2(os.path.join(src, cab), os.path.join(out, cab))
            print("  %-14s copied unchanged (no swaps)" % cab)
            continue
        _makecab(entries, os.path.join(out, cab))
        for s in swapped:
            swapped_total[s] = cab
        print("  %-14s repacked (%d files)%s" % (cab, len(entries),
              "  SWAPPED: " + ", ".join(swapped) if swapped else ""))
    shutil.rmtree(exdir, ignore_errors=True)
    print("\nswapped %d of our modules into the payload: %s"
          % (len(swapped_total), ", ".join(sorted(swapped_total))))
    print("-> %s" % out)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
