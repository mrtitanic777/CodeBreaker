#!/usr/bin/env python3
r"""
bootchain.py -- Windows 3.1 boot-chain reconstruction readiness (NE).

Walks the modules Windows 3.1 loads to reach the Program Manager desktop, in boot
order, and reports for each whether it is reconstructed byte-identical (whole-file
link-ready). A module is boot-ready when its per-function reconstruction is
COMPLETE (conduct VERDICT) and it whole-file links byte-for-byte to the shipped
binary (linkfile) -- i.e. substituting our rebuilt file into a 3.1 install yields
a bit-identical, bootable system. This is the win95 bootchain.py, retargeted to
3.1's 16-bit NE core (no VMM32/W3 -- WIN386.EXE 386-enhanced mode is a separate
track; standard-mode reaches the desktop through the modules below).

    python bootchain.py                    # fast readiness (ledger + .asm on disk)
    python bootchain.py --verify           # ALSO whole-file link each (slow, exact)
    python bootchain.py --verify --save staged/win31   # + save byte-identical files
"""
import os
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)

# Boot stages in load order to the Program Manager desktop. External = not in our
# binary corpus (DOS real-mode / the tiny WIN.COM launcher), noted honestly rather
# than silently skipped. The display driver family is represented by VGA.DRV.
STAGES = [
    ("real-mode DOS",   ["IO.SYS", "MSDOS.SYS", "COMMAND.COM"]),
    ("Windows loader",  ["WIN.COM"]),
    ("16-bit core",     ["KRNL386.EXE", "GDI.EXE", "USER.EXE"]),
    ("boot drivers",    ["SYSTEM.DRV", "VGA.DRV", "KEYBOARD.DRV", "MOUSE.DRV",
                         "COMM.DRV", "MMSOUND.DRV"]),
    ("shell",           ["PROGMAN.EXE"]),
]


def _done(module):
    """{fmt, asm, complete} from .asm on disk + the conductor's report, or None if
    the module is not in our corpus (external DOS/loader). Tries both the full
    filename and the extensionless stem for naming (krnl386 vs krnl386.exe)."""
    import target, linkfile
    try:
        path = target.resolve(module)
    except Exception:
        return None
    if not path or not os.path.exists(path):
        return None
    base = os.path.basename(path)
    adir = linkfile._asm_dir(base)
    nasm = len([f for f in os.listdir(adir) if f.endswith(".asm")]) if adir else 0
    complete = False
    for cand in (base.lower(), base.rsplit(".", 1)[0].lower()):
        rt = os.path.join("reports", "conduct", cand + ".report.txt")
        if os.path.exists(rt) and "VERDICT: COMPLETE" in open(rt, encoding="latin1").read():
            complete = True
            break
    try:
        fmt = target.format_of(path)
    except Exception:
        fmt = "?"
    return {"fmt": fmt, "asm": nasm, "complete": complete}


def main():
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    verify = "--verify" in sys.argv
    savedir = None
    if "--save" in sys.argv:
        savedir = sys.argv[sys.argv.index("--save") + 1]
        os.makedirs(savedir, exist_ok=True)

    ready = total = external = linked = 0
    print("Windows 3.1 boot chain -- reconstruction readiness")
    print("=" * 66)
    for stage, mods in STAGES:
        print("\n%s:" % stage)
        for m in mods:
            total += 1
            d = _done(m)
            if d is None:
                external += 1
                print("  %-14s  external (DOS/loader -- not in the binary corpus)" % m)
                continue
            tag = "COMPLETE" if d["complete"] else \
                ("partial (%d .asm)" % d["asm"] if d["asm"] else "NOT reconstructed")
            extra = ""
            if verify and d["complete"]:
                try:
                    import linkfile
                    lr = linkfile.link_module(m)
                    if lr["match"]:
                        linked += 1
                        extra = "  -> LINK byte-identical (%d funcs)" % lr["placed"]
                        if savedir:
                            import target
                            fn = os.path.basename(target.resolve(m)).lower()
                            open(os.path.join(savedir, fn), "wb").write(lr["rebuilt"])
                    elif not lr.get("all_placed", True):
                        extra = "  -> LINK INCOMPLETE (%d/%d placed)" % (lr["placed"], lr["n_funcs"])
                    else:
                        extra = "  -> LINK DIFF %d (@0x%X)" % (lr["n_diff"], lr["first_diff"])
                except Exception as e:
                    extra = "  -> link err %s" % str(e)[:40]
            if d["complete"]:
                ready += 1
            print("  %-14s  [%s] %-22s %d funcs%s" % (m, d["fmt"], tag, d["asm"], extra))

    print("\n" + "=" * 66)
    inscope = total - external
    print("boot-chain modules: %d  (%d external DOS/loader)" % (total, external))
    print("in-corpus: %d   reconstructed byte-exact (COMPLETE): %d  (%.0f%%)"
          % (inscope, ready, 100.0 * ready / inscope if inscope else 0))
    if verify:
        print("whole-file BYTE-IDENTICAL links: %d / %d in-corpus" % (linked, inscope))
        if savedir:
            print("saved byte-identical rebuilds to: %s" % os.path.abspath(savedir))
    print("=" * 66)


if __name__ == "__main__":
    main()
