#!/usr/bin/env python3
r"""neboot.py -- NE boot-integration: build a Win95 Setup CD that installs OUR reconstructed 16-bit
(NE) binaries, so the real OS -- not the isolated oracle -- is the behavioral test for them.

Per-function dynamic verification is viable for PE (32-bit near code is self-contained) but not for
NE: Win16 far functions need a full runtime (initialized DGROUP globals, LibMain, valid far
structures, the task DB) that the oracle can't cheaply stand up -- retail itself faults in isolation.
The genuine environment for the NE core is Win95 itself. This tool substitutes our owlink-built NE
binaries into the install cabinets and lays out a bootable Setup CD; whether Setup completes and the
installed system boots to the desktop IS the NE fidelity gate.

By default our binaries go ONLY into the INSTALLED cabinets (WIN95_*.CAB), leaving the Setup-runtime
cabinets (MINI.CAB, PRECOPY*.CAB) original -- so GUI Setup runs on stock code and reaches the desktop,
and our NE core is exercised on the FIRST BOOT of the installed system (a clean, isolated signal).
`--runtime` also substitutes the Setup-runtime cabinets (Setup then runs on our core too). `--only
mod,mod` restricts substitution to specific NE modules, for bisecting which of our binaries is sound.

    wt --target win95 neboot [--only setupx,gdi] [--runtime] [--out PATH]

Writes the ISO (default: Documents/Win95-VMware/win95-ne.iso) + a manifest of which binaries are ours.
"""
import csv
import os
import shutil
import sys

from src.targets.win95 import authsrc, owlink, disc_build, cab

RUNTIME_CABS = {"MINI.CAB", "PRECOPY1.CAB", "PRECOPY2.CAB"}


def _our_modules(fmts=("NE", "PE")):
    """Every module that links OK in the given formats, from BUILDLINK.csv -> {filename: rebuilt path}.
    Default NE+PE = the whole Win16 core AND the Win32 subsystem, so the disc is entirely ours, not just
    the 16-bit core. The output filename keeps the retail extension (.dll/.exe/.cpl/...) so it matches
    and replaces the media member of the same name."""
    dr = authsrc._dr()
    out = {}
    csvp = os.path.join(dr, "recon", "authored", "BUILDLINK.csv")
    for r in csv.DictReader(open(csvp)):
        if r["fmt"] not in fmts or r["status"] != "OK":
            continue
        retail = owlink._find_binary(r["module"])
        if not retail:
            continue
        ext = os.path.splitext(retail)[1] or (".dll" if r["fmt"] == "PE" else ".exe")
        outfn = r["module"] + (".dll" if retail.lower().endswith(".dll") else
                               ".exe" if retail.lower().endswith(".exe") else ext)
        p = os.path.join(dr, "recon", "authored", "_link", r["module"], r["module"] +
                         (".dll" if retail.lower().endswith(".dll") else ".exe"))
        if os.path.isfile(p) and os.path.getsize(p) > 512:
            out[outfn.lower()] = p
    return out


def _ne_modules():
    return _our_modules(("NE",))


def main(argv):
    dr = authsrc._dr()
    only = None
    exc = set()
    out_iso = os.path.join(os.path.expanduser("~"), "Documents", "Win95-VMware", "win95-ne.iso")
    runtime = "--runtime" in argv
    if "--only" in argv:
        only = {m.strip().lower() for m in argv[argv.index("--only") + 1].split(",")}
    if "--except" in argv:                                    # substitute all our NE EXCEPT these
        exc = {m.strip().lower() for m in argv[argv.index("--except") + 1].split(",")}
    if "--out" in argv:
        out_iso = argv[argv.index("--out") + 1]

    src = os.path.join(dr, "corpus", "disc", "src", "WIN95")
    if not os.path.isdir(src):
        print("no install source at %s" % src); return 1

    # our binaries -> a flat `our` dir (lowercase names, as disc_build._resolve expects). Default is the
    # WHOLE thing: NE (Win16 core) + PE (Win32 subsystem). `--ne-only` restricts to the 16-bit core.
    fmts = ("NE",) if "--ne-only" in argv else ("NE", "PE")
    ne = _our_modules(fmts)
    if only:
        ne = {fn: p for fn, p in ne.items() if os.path.splitext(fn)[0] in only}
    if exc:
        ne = {fn: p for fn, p in ne.items() if os.path.splitext(fn)[0] not in exc}
    if not ne:
        print("no matching our-built binaries (run owlink --all first)"); return 1
    our_dir = os.path.join(dr, "recon", "authored", "_neboot", "our")
    os.makedirs(our_dir, exist_ok=True)
    for fn, p in ne.items():
        shutil.copy2(p, os.path.join(our_dir, fn))

    # media expansion (all cab members), cached under recon
    media = os.path.join(dr, "recon", "authored", "_neboot", "media")
    if not (os.path.isdir(media) and len(os.listdir(media)) > 500):
        os.makedirs(media, exist_ok=True)
        print("expanding install media (7-Zip) ...")
        cab.extract(src, media)

    # --runtime: the Setup-runtime cabinets (MINI/PRECOPY) carry members that live NOWHERE else (DOSX,
    # MINIKBD, the mini mouse/sound drivers) so the flat media pool lacks them and disc_build's
    # all-or-nothing repack would pass those cabinets through STOCK. Extract each runtime cabinet's
    # ORIGINAL members (7-Zip on the retail cab, which is standard MSZIP/QUANTUM) into the media pool so
    # every member resolves and our binaries actually go in. our_dir still overrides the substituted
    # names, so name collisions with the full-Windows variants don't matter.
    import glob as _glob
    if runtime:
        for cb in _glob.glob(os.path.join(src, "*.[cC][aA][bB]")):
            if os.path.basename(cb).upper() in RUNTIME_CABS:
                try:
                    cab.extract(cb, media, head=cb)
                except Exception as e:                               # noqa: BLE001
                    print("  (could not expand %s: %s)" % (os.path.basename(cb), e))

    # which cabinets get our binaries
    allcabs = {os.path.basename(c).upper() for c in
               _glob.glob(os.path.join(src, "*.[cC][aA][bB]"))}
    subst = None if runtime else (allcabs - RUNTIME_CABS)

    print("== neboot: substituting %d our-NE binaries into %s ==" % (
        len(ne), "ALL cabinets" if runtime else "installed cabinets only (runtime stays original)"))
    print("   binaries:", ", ".join(sorted(ne)))
    out_root = os.path.join(dr, "recon", "authored", "_neboot", "out")
    r = disc_build.build(src, media, out_iso, our=our_dir, boot=None, volid="WIN95",
                         out_root=out_root, subst_cabs=subst,
                         progress=lambda c: (c % 40 == 0 and print("  repacked", c, "cabs", flush=True)))
    # manifest
    man = os.path.join(os.path.dirname(out_iso), "win95-ne.manifest.txt")
    with open(man, "w") as f:
        f.write("NE boot-integration CD\niso: %s\nmode: %s\n" %
                (out_iso, "runtime+installed" if runtime else "installed-only"))
        f.write("our NE binaries substituted (%d):\n" % len(ne))
        for fn in sorted(ne):
            f.write("  %s\n" % fn)
    print("=" * 60)
    print("ISO: %s  (%d bytes)" % (r["iso"], r["iso_bytes"]))
    print("  cabs repacked: %d  files from OUR reconstruction: %d  from media: %d" %
          (r["cabs"], r["files_ours"], r["files_media"]))
    print("  manifest: %s" % man)
    print("\nBoot in VMware: attach win95boot.flp (floppy) + this ISO (CD), install to a blank disk.")
    print("Installed-only mode: Setup runs on stock code; our NE core runs on the FIRST desktop boot.")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
