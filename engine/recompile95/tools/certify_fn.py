#!/usr/bin/env python3
r"""certify_fn.py -- the STRONGER, FUNCTIONAL swap gate.

certify.py gated on the pure-leaf oracle + a defect census, but that never checked the EXPORTED
functions Win95 actually calls -- so a "certified" linkinfo.dll still fell to Safe Mode. This gates
on what matters: for every EXPORT, walk its transitive call graph and require that NOTHING in it
carries a lift defect (empty body / (*(fnptr)0) / 'not recovered' / goto* / un-lifted callee).
Only then is the C a faithful translation of everything that export can execute.

  CERTIFIED : every export's whole call graph is defect-free  -> safe to swap.
  PARTIAL   : k/N exports are clean                            -> swap only if Win95 uses only those.
  UNSAFE    : few/no clean exports.

    python certify_fn.py <module> [...]        (--corpus for all; --list to see per-export verdicts)
"""
import os, re, sys, glob
_HERE = os.path.abspath(__file__)
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(_HERE)))))
from engine.format_loaders.pe import open_pe
W = os.environ.get("WT_DATA_ROOT", "C:/Users/mrtit/Documents/GitHub/CodeBreaker/binaries/win95")


def _analyze(module):
    """-> (funcs, defects, calls): funcs=set of defined offsets; defects[off]=reason or None;
    calls[off]=set of callee offsets (sub_<hex>)."""
    c = glob.glob(os.path.join(W, "src", "generated", "**", module + ".c"), recursive=True)
    if not c:
        return None
    txt = open(c[0], encoding="utf-8", errors="replace").read()
    # Apply recover_flags BEFORE classifying: package_ff emits with recover_flags applied, so the
    # certification/`bad` decision must judge the SAME code. Otherwise label-goto branches that
    # recover_flags resolves are still counted as `notrec` defects -> the fn is wrongly wrappered.
    import recover_flags
    txt, _ = recover_flags.recover_flags_src(module, txt)
    funcs, defects, calls = set(), {}, {}
    # NOTE: a struct-arg function emits one or more local `struct NAME { ... };` typedefs
    # BETWEEN the [AUTO] comment and the signature. Allow (and skip) them, else 18% of the
    # corpus (41k fns -- incl. hundreds each in shell32/ole32/comctl32) is wrongly seen as
    # "unlifted", inflating `bad` and defeating certification + clean-C emission.
    for m in re.finditer(r'/\* \[AUTO\]\s+\w+\s+\w+\s+seg\d+:0x([0-9a-f]+)[^\n]*\*/\s*'
                         r'(?:struct\s+\w+\s*\{[^}]*\}\s*;\s*)*'
                         r'(?:u32|int|void|short|long)\s+\w+\([^)]*\)\s*\{((?:(?!/\* \[AUTO\]).)*?)\n\}', txt, re.S):
        off, body = int(m.group(1), 16), m.group(2)
        funcs.add(off)
        d = None
        if body.strip() == "":
            d = "empty"
        elif "(*(fnptr)0)" in body:
            d = "fnptr0"
        elif "not recovered" in body:
            d = "notrec"
        elif "goto *0" in body or "/* external */" in body:
            d = "extgoto"
        defects[off] = d
        calls[off] = set(int(x, 16) for x in re.findall(r'\bsub_([0-9a-f]+)', body))
    return funcs, defects, calls


def certify_exports(module):
    a = _analyze(module)
    if not a:
        return None
    funcs, defects, calls = a
    pe = open_pe(os.path.join(W, "disc", "staged", "bin", module))
    tv = [s for s in pe.sections if s["chars"] & 0x20000000][0]["vaddr"]
    ex = pe.exports()
    items = ex.items() if isinstance(ex, dict) else ex
    verdicts = {}
    for t in items:
        if not (isinstance(t, (list, tuple)) and len(t) == 3):
            continue
        _ord, name, rva = t
        off = rva - tv
        # transitive closure of the export's call graph
        seen, stack, bad = set(), [off], None
        while stack:
            o = stack.pop()
            if o in seen:
                continue
            seen.add(o)
            if o not in funcs:
                bad = "unlifted-callee@%x" % o
                break
            if defects.get(o):
                bad = "%s@%x" % (defects[o], o)
                break
            stack.extend(calls.get(o, ()))
        verdicts[name] = (bad is None, bad, len(seen))
    return verdicts


def main():
    show = "--list" in sys.argv
    if "--corpus" in sys.argv:
        mods = sorted({os.path.basename(p)[:-2] for p in glob.glob(os.path.join(W, "src", "generated", "**", "*.dll.c"), recursive=True)})
    else:
        mods = [a for a in sys.argv[1:] if not a.startswith("-")]
    cert = 0
    for m in mods:
        try:
            v = certify_exports(m)
        except Exception as e:
            print("%-18s ERROR %s" % (m, type(e).__name__)); continue
        if not v:
            continue
        clean = sum(1 for ok, _, _ in v.values() if ok)
        tot = len(v)
        verdict = "CERTIFIED" if clean == tot and tot else ("PARTIAL" if clean else "UNSAFE")
        if verdict == "CERTIFIED":
            cert += 1
        print("%-10s %-18s clean-exports=%d/%d" % (verdict, m, clean, tot))
        if show:
            for name, (ok, bad, n) in sorted(v.items()):
                print("      %-28s %s" % (name, "clean (graph=%d)" % n if ok else "DEFECT %s" % bad))
    if len(mods) > 1:
        print("== fully CERTIFIED (all exports clean): %d / %d ==" % (cert, len(mods)))


if __name__ == "__main__":
    main()
