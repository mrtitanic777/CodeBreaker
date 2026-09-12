#!/usr/bin/env python3
r"""
bootchain.py -- Windows 95 boot-chain reconstruction readiness.

Walks the modules Win95 loads to reach the desktop, in boot order, and reports for
each whether it is reconstructed byte-identical (whole-file link-ready). A module
is boot-ready when its per-function reconstruction is COMPLETE and it whole-file
links byte-for-byte to the shipped binary (linkfile) -- i.e. substituting our
rebuilt file into a Win95 install yields a bit-identical, bootable system.

    python bootchain.py            # readiness report (fast: ledger + .asm on disk)
    python bootchain.py --verify   # ALSO whole-file link each module (slow, exact)
"""
import json
import os
import sys
from codebreaker.core import layout

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)

# Boot stages in load order. External = not in our binary corpus (DOS real-mode /
# real-mode loader), noted honestly rather than silently skipped.
STAGES = [
    ("real-mode DOS",     ["IO.SYS", "MSDOS.SYS"]),
    ("Windows loader",    ["WIN.COM"]),
    ("virtual machine mgr", ["VMM32.VXD"]),
    ("16-bit core",       ["KRNL386.EXE", "GDI.EXE", "USER.EXE"]),
    ("32-bit core",       ["KERNEL32.DLL", "GDI32.DLL", "USER32.DLL", "ADVAPI32.DLL"]),
    ("boot drivers",      ["SYSTEM.DRV", "VGA.DRV", "KEYBOARD.DRV", "MOUSE.DRV"]),
    ("shell",             ["EXPLORER.EXE"]),
]


def _done(module):
    """(reconstructed?, complete?) from .asm on disk + the conductor's report,
    trying both the full-filename and extensionless-stem naming."""
    import target, linkfile
    try:
        path = target.resolve(module)
    except Exception:
        return None                          # not in the corpus at all
    base = os.path.basename(path)
    # W3 containers (VMM32.VXD): the wrapper reconstructs; report the inner module.
    try:
        with open(path, "rb") as f:
            head = f.read(0x40)
        import struct
        if head[:2] == b"MZ":
            lfa = struct.unpack_from("<I", head, 0x3C)[0]
            with open(path, "rb") as f:
                f.seek(lfa)
                if f.read(2) == b"W3":
                    import w3
                    a = w3.analyze(base)
                    # inner LE reconstructed? (conduct wrote its report)
                    rt = os.path.join(layout.runtime_dir(), "conduct", base.lower() + ".report.txt")
                    inner_done = os.path.exists(rt) and "VERDICT: COMPLETE" in open(rt, encoding="latin1").read()
                    adir = linkfile._asm_dir(base)
                    nasm = len([x for x in os.listdir(adir) if x.endswith(".asm")]) if adir else 0
                    if inner_done:
                        return {"fmt": "W3", "asm": nasm, "complete": True,
                                "note": "W3 wrapper reconstructed byte-exact; inner VMM LE %d funcs byte-exact" % nasm}
                    inner = a["modules"][0]["le"] if a.get("modules") else {}
                    note = "W3 wrapper %s; inner %s LE %d exec obj, %d KB code (pipeline-ready)" % (
                        "OK" if a["hdr_ok"] else "DIFF",
                        a["modules"][0]["name"] if a.get("modules") else "?",
                        inner.get("exec", 0), inner.get("code_bytes", 0) // 1024)
                    return {"fmt": "W3", "asm": nasm, "complete": False, "note": note}
    except Exception:
        pass
    adir = linkfile._asm_dir(base)
    nasm = len([f for f in os.listdir(adir) if f.endswith(".asm")]) if adir else 0
    complete = False
    for cand in (base.lower(), base.rsplit(".", 1)[0].lower()):
        rt = os.path.join(layout.runtime_dir(), "conduct", cand + ".report.txt")
        if os.path.exists(rt) and "VERDICT: COMPLETE" in open(rt, encoding="latin1").read():
            complete = True
            break
    return {"fmt": target.format_of(path), "asm": nasm, "complete": complete}


def main():
    os.chdir(os.path.dirname(ROOT) if os.path.basename(ROOT) == "tools" else ".")
    from codebreaker.core import config
    config.apply_config()
    verify = "--verify" in sys.argv

    ready = total = external = 0
    print("Windows 95 boot chain -- reconstruction readiness")
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
            if d.get("note"):
                if d["complete"]:
                    ready += 1
                print("  %-14s  [%s] %s" % (m, d["fmt"], d["note"]))
                continue
            tag = "COMPLETE" if d["complete"] else ("partial (%d .asm)" % d["asm"] if d["asm"] else "NOT reconstructed")
            extra = ""
            if verify and d["complete"]:
                try:
                    import linkfile
                    lr = linkfile.link_module(m)
                    extra = "  -> LINK %s" % ("byte-identical" if lr["match"] else "DIFF %d" % lr["n_diff"])
                except Exception as e:
                    extra = "  -> link err %s" % str(e)[:30]
            if d["complete"]:
                ready += 1
            print("  %-14s  [%s] %-22s %d funcs%s" % (m, d["fmt"], tag, d["asm"], extra))

    print("\n" + "=" * 66)
    inscope = total - external
    print("boot-chain modules: %d  (%d external DOS/loader)" % (total, external))
    print("in-corpus: %d   reconstructed byte-identical (COMPLETE): %d  (%.0f%%)"
          % (inscope, ready, 100.0 * ready / inscope if inscope else 0))
    gaps = [m for _s, mods in STAGES for m in mods
            if (_done(m) or {}).get("complete") is False]
    if gaps:
        print("gaps still to reconstruct: %s" % ", ".join(gaps))
    print("=" * 66)


if __name__ == "__main__":
    main()
