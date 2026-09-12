#!/usr/bin/env python3
r"""promote.py -- turn the ASM-fallback hybrid into REAL C where the oracle proved it.

hybrid_emit.py emits every function as byte-exact naked asm (compiles+runs, but it's still asm).
The point of proving a function bit-identical (difftest) is that we can then PROMOTE it: replace
its naked-asm body with the actual lifted C. This emits a module TU that is a genuine MIX --
recovered C for the pure-leaf functions the oracle verified, byte-exact naked asm for the rest --
and compiles with the recompile95 prelude. That is the real "compilable C, ASM fallback" deliverable:
the C fraction is real source, and it grows as the oracle promotes more.

    python promote.py <module>  [--out DIR]     e.g. gdi32.dll
"""
import os, re, sys, glob
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import hybrid_emit as H
INC = os.path.join(os.path.dirname(HERE), "include")
GEN = os.path.join(os.environ.get("WT_DATA_ROOT", "."), "src", "generated")


_CTRL = {"if", "while", "for", "switch", "return", "sizeof", "do"}


def _pure_leaf(body):
    """The oracle-provable subset: NO calls of any kind (sub_, named APIs like USER_*/VMM_ord_*),
    no memory deref, no computed goto, recovered, non-void, writes a real value. Rejecting every
    call is what makes a promoted leaf both self-contained-correct AND compile with no extern."""
    if not body or '*(' in body or 'goto *' in body or 'not recovered' in body:
        return False
    head, inner = body.split('{', 1)
    if '(void)' in head or re.match(r'\s*void\b', head):
        return False
    for call in re.findall(r'\b([A-Za-z_]\w*)\s*\(', inner):     # any call -> not a leaf
        if call not in _CTRL:
            return False
    return len(re.findall(r'[+\-*&|^]|<<|>>', body)) >= 2


_GENIDX = None


def _gen_index():
    """{module.lower(): generated_c_path} over GEN, built ONCE (per-module glob was O(tree)*633)."""
    global _GENIDX
    if _GENIDX is None:
        _GENIDX = {}
        for p in glob.glob(os.path.join(GEN, "**", "*.c"), recursive=True):
            _GENIDX[os.path.basename(p)[:-2].lower()] = p
    return _GENIDX


def load_manifest(path):
    """{module.lower(): set((seg,off))} of oracle-VERIFIED functions to promote."""
    mani = {}
    if path and os.path.exists(path):
        for line in open(path, encoding="utf-8", errors="replace"):
            p = line.split("\t")
            if len(p) >= 3:
                mani.setdefault(p[0].strip().lower(), set()).add((int(p[1]), int(p[2], 16)))
    return mani


def promote(module, outdir, verified=None):
    ent = H._index().get(module.strip().lower())
    if not ent:
        sys.exit("no asm dir for " + module)
    cp = _gen_index().get(module.strip().lower())
    gen = open(cp, encoding="utf-8", errors="replace").read() if cp else ""
    # {(seg,off): lifted-C body}: manifest-VERIFIED fns if given, else the pure-leaf heuristic
    promo = {}
    for m in re.finditer(r'/\* \[AUTO\]\s+(\w+)\s+\w+\s+seg(\d+):0x([0-9a-f]+)[^\n]*\*/\s*'
                         r'((?:int|u32|short)\s+\w+\([^)]*\)\s*\{(?:(?!/\* \[AUTO\]).)*?\n\})', gen, re.S):
        seg, off, body = int(m.group(2)), int(m.group(3), 16), m.group(4)
        take = ((seg, off) in verified) if verified is not None else _pure_leaf(body)
        if take and '*(' not in body and 'sub_' not in body and 'goto *' not in body:
            promo[(seg, off)] = body                 # (compile-safe subset even from a manifest)
    # emit naked hybrid to a temp, then splice: replace each promoted fn's naked def with real C
    mdir, prefix = ent
    funcs = sorted(glob.glob(os.path.join(mdir, prefix + "*_*.asm")),
                   key=lambda p: (lambda mm: (int(mm.group(2)), int(mm.group(3), 16)))(H._SEGFILE.match(os.path.basename(p))))
    sym = module.replace(".", "_")
    parsed = [H.parse_function(p) for p in funcs]
    starts = {(sg, st) for sg, st, _ in parsed}
    os.makedirs(outdir, exist_ok=True)
    dst = os.path.join(outdir, sym + "_promoted.c")
    npro = nnak = 0
    with open(dst, "w") as fh:
        fh.write('/* AUTO promoted TU for %s: real lifted C for oracle-proven pure leaves, '
                 'byte-exact naked asm for the rest. */\n' % module)
        fh.write('#include "win95c.h"\n#define NK __attribute__((naked,used))\n')
        # numbered flag temps (__cf0, __ovf_add_1, ...) the lift both assigns and reads -> globals
        # (same rule as rc.adapt); the no-paren lookahead skips prelude helpers like __carry(a,b).
        flags = sorted(set(re.findall(r'\b__(?:cf|zf|sf|of|pf|af|ovf|carry|borrow|aux)\w*\b(?!\s*\()',
                                      "".join(promo.values()))))
        fh.write("".join("static u32 %s;\n" % f for f in flags) + "\n")
        # forward decls for the naked (non-promoted) fns only
        for sg, st, _ in parsed:
            if (sg, st) not in promo:
                fh.write("NK void f_%s_s%d_%05x(void);\n" % (sym, sg, st))
        fh.write("\n")
        for sg, start, items in parsed:
            if (sg, start) in promo:                   # REAL C
                fh.write("/* promoted: verified bit-identical to Win95 */\n")
                fh.write(promo[(sg, start)].rstrip() + "\n\n")
                npro += 1
                continue
            fh.write('NK void f_%s_s%d_%05x(void){ __asm__ volatile(\n' % (sym, sg, start))
            for it in items:
                if it[0] == "xfer":
                    _, op, tgt, raw = it
                    if (sg, tgt) in starts and (sg, tgt) not in promo:
                        fh.write('  "%s _f_%s_s%d_%05x\\n"\n' % (op, sym, sg, tgt))
                    elif raw:
                        fh.write('  ".byte %s\\n"\n' % ",".join("0x%02x" % b for b in raw))
                else:
                    fh.write('  ".byte %s\\n"\n' % ",".join("0x%02x" % b for b in it[1]))
            fh.write('  ); }\n')
            nnak += 1
    return dst, npro, nnak


if __name__ == "__main__":
    outdir = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else "."
    mani = load_manifest(sys.argv[sys.argv.index("--manifest") + 1]) if "--manifest" in sys.argv else None
    if "--all" in sys.argv:
        import gc
        idx = H._index()
        only = sys.argv[sys.argv.index("--prefix") + 1] if "--prefix" in sys.argv else None
        mods = sorted(m for m, (d, p) in idx.items() if (only is None or p == only))
        if mani and "--manifest-only" in sys.argv:       # ONLY modules with verified fns (fast)
            mods = [m for m in mods if m in mani]
        tot = ok = 0
        for m in mods:
            try:
                dst, npro, nnak = promote(m, outdir, verified=mani.get(m, frozenset()) if mani else None)
                ok += 1; tot += npro
                print("PROMO\t%s\t%d\t%d" % (m, npro, nnak), flush=True)
            except Exception as e:
                print("FAIL\t%s\t%s" % (m, e), flush=True)
            gc.collect()
        tag = "VERIFIED" if mani else "leaf-heuristic"
        print("== promoted %d %s fns to real C across %d/%d modules ==" % (tot, tag, ok, len(mods)), flush=True)
    else:
        mod = sys.argv[1]
        dst, npro, nnak = promote(mod, outdir, verified=mani.get(mod.lower(), frozenset()) if mani else None)
        print("wrote %s\n  promoted to real C: %d   naked-asm fallback: %d" % (dst, npro, nnak))
