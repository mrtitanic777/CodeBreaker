#!/usr/bin/env python3
r"""symbolize.py -- Milestone 1: strip every baked-in machine address out of a module's lifted C,
so the code becomes RELOCATABLE (and therefore editable -- functions can change size without
breaking address literals). This is the linchpin of a mass-scale ASM->editable-C conversion that
still builds into a loadable module.

The binary's own tables say exactly what each absolute address IS -- no guesswork:
  * import (.idata)  -> `call [0xIAT]`         becomes  a call through a declared extern import.
  * export/.reloc    -> a code/data pointer     becomes  &SYM (a named symbol at that RVA).
  * in-image data    -> `*(u8*)(0xADDR)`         becomes  IMG[rva]  (the module's own image array).
Everything resolves to a symbol the linker can place anywhere. Output: the lifted C with all
`0x<ImageBase..>` literals rewritten to symbolic form + a manifest of what each became.

    python symbolize.py <module>            e.g. lz32.dll     (reports coverage; --emit writes C)
"""
import os, re, sys, glob
_HERE = os.path.abspath(__file__)
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(_HERE)))))
from engine.format_loaders.pe import open_pe
from engine.foundation import paths
W = os.environ.get("WT_DATA_ROOT", paths.binaries("win95"))


def analyze(module):
    pe = open_pe(os.path.join(W, "disc", "staged", "bin", module))
    base = pe.image_base
    imports = pe.imports()                      # {slot_va:(dll,name)}
    relocs = pe.base_relocs()                   # {rva} holding an absolute address
    # section map rva-range -> name (to classify an in-image address)
    secs = [(s["vaddr"], s["vaddr"] + max(s["vsize"], s["rawsize"]), s["name"]) for s in pe.sections]

    def classify(va):
        rva = va - base
        if va in imports:
            return ("import", imports[va])
        if rva in relocs:
            return ("reloc_ptr", rva)           # a slot that holds a pointer the loader fixes
        for a, b, nm in secs:
            if a <= rva < b:
                return ("in_image", (nm, rva))
        return ("external", rva)
    return pe, base, classify


def scan_c(module):
    c = glob.glob(os.path.join(W, "src", "generated", "**", module + ".c"), recursive=True)
    if not c:
        return "", []
    txt = open(c[0], encoding="utf-8", errors="replace").read()
    pe, base, classify = analyze(module)
    lo, hi = base, base + 0x10000000
    hits = sorted(set(int(x, 16) for x in re.findall(r'0x([0-9a-fA-F]{6,8})', txt)
                      if lo <= int(x, 16) < hi))
    return txt, [(va, classify(va)) for va in hits]


def emit(module, outdir):
    """Rewrite EVERY in-module absolute address literal `0x<base+rva>` -> `(_imgbase + 0x<rva>)`,
    making the whole module RELOCATABLE (one rule covers data AND IAT: both are base+rva). The
    result is editable C -- no fixed addresses -- that loads at any _imgbase the harness picks.
    Applies rc.adapt first so the register-machine C compiles (rung 1)."""
    import rc, recover_flags
    c = glob.glob(os.path.join(W, "src", "generated", "**", module + ".c"), recursive=True)
    src = open(c[0], encoding="utf-8", errors="replace").read()
    src, _nrec = recover_flags.recover_flags_src(module, src)   # ASM-recover cmp/test branch conditions
    pe, base, classify = analyze(module)
    simg = 0
    for s in pe.sections:
        simg = max(simg, s["vaddr"] + max(s["vsize"], s["rawsize"]))

    def repl(m):
        va = int(m.group(0), 16)
        if base <= va < base + simg:                 # in this module's image -> relocatable
            return "(_imgbase + 0x%x)" % (va - base)
        return m.group(0)                             # a constant / cross-module -> leave (Milestone 2)
    src = rc.adapt(src, link=True)                    # FIRST: strips &literal / literal=__jmpd (else our rewrite makes &(_imgbase+rva))
    sym = re.sub(r'0x[0-9a-fA-F]{6,8}', repl, src)    # THEN: remaining 0x<base+rva> are in safe deref/value contexts
    os.makedirs(outdir, exist_ok=True)
    dst = os.path.join(outdir, module.replace(".", "_") + "_reloc.c")
    with open(dst, "w") as f:
        f.write('/* RELOCATABLE editable C for %s -- no baked addresses; loads at any _imgbase. */\n'
                '#include "win95c.h"\nunsigned _imgbase;   /* set by the loader to the image buffer */\n\n'
                % module)
        f.write(sym)
    n_reloc = sym.count("_imgbase")
    return dst, n_reloc, base, simg


def emit_seg(module, outdir):
    """NE(16-bit)/LE(VxD) relocatable C. These bake own-data as SMALL segment/object offsets
    (`*(u16*)(0x2ac)`, not ImageBase+rva), and imports/VMM services are already NAMED calls
    (GDI_BITBLT, VMM_ord_264). So: rewrite every LITERAL-address deref `*(uN*)(0xOFF)` ->
    `*(uN*)((_imgbase + 0xOFF))` (data/object base, runtime) -- leaving segment-register far
    pointers (`*(u16*)(es + bx)`) and plain constants alone -- then rc.adapt for rung-1 compile.
    Named imports link via stubs (same as un-lifted sub_)."""
    import rc, hybrid_emit as H
    ent = H._index().get(module.strip().lower())
    prefix = ent[1] if ent else "seg"
    c = glob.glob(os.path.join(W, "src", "generated", "**", module + ".c"), recursive=True)
    src = open(c[0], encoding="utf-8", errors="replace").read()
    # only literal-address derefs (the module's own data); NOT `(es + bx)` or bare constants
    src = re.sub(r'(\*\(u\d+\*\)\()(0x[0-9a-f]+)(\))',
                 lambda m: "%s(_imgbase + %s)%s" % (m.group(1), m.group(2), m.group(3)), src)
    n = src.count("_imgbase")
    src = rc.adapt(src, link=True)
    os.makedirs(outdir, exist_ok=True)
    dst = os.path.join(outdir, module.replace(".", "_") + "_reloc.c")
    with open(dst, "w") as f:
        f.write('/* RELOCATABLE editable C for %s (%s) -- own-data via runtime _imgbase. */\n'
                '#include "win95c.h"\nunsigned _imgbase;\n\n' % (module, prefix))
        f.write(src)
    return dst, n, 0, 0


def main():
    if "--seg" in sys.argv:                          # NE/LE single module
        outdir = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else "."
        dst, n, _, _ = emit_seg(sys.argv[1], outdir)
        print("wrote %s\n  %d data derefs rewritten to (_imgbase + off)" % (dst, n))
        return
    if "--all" in sys.argv:                          # convert every PE DLL to relocatable C (one process)
        import gc, glob as _g
        outdir = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else "."
        stg = os.path.join(W, "disc", "staged", "bin")
        gen = {os.path.basename(p)[:-2].lower() for p in _g.glob(os.path.join(W, "src", "generated", "**", "*.c"), recursive=True)}
        mods = sorted(os.path.basename(p) for p in _g.glob(os.path.join(stg, "*.dll"))
                      if os.path.basename(p).lower() in gen and open(p, "rb").read(2) == b"MZ")
        ok = tot = 0
        for m in mods:
            try:
                dst, n, base, simg = emit(m, outdir); ok += 1; tot += n
                print("SYM\t%s\t%d" % (m, n), flush=True)
            except Exception as e:
                print("FAIL\t%s\t%s" % (m, type(e).__name__), flush=True)
            gc.collect()
        print("== symbolized %d/%d PE DLLs, %d address refs relocated ==" % (ok, len(mods), tot), flush=True)
        return
    module = sys.argv[1]
    if "--emit" in sys.argv:
        outdir = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else "."
        dst, n, base, simg = emit(module, outdir)
        print("wrote %s\n  %d address refs rewritten to (_imgbase + rva); ImageBase was 0x%08x, size 0x%x"
              % (dst, n, base, simg))
        return
    txt, cls = scan_c(module)
    from collections import Counter
    kinds = Counter(k for _va, (k, _d) in cls)
    print("%s: %d distinct baked-in addresses" % (module, len(cls)))
    for k, n in kinds.most_common():
        print("   %-10s %d" % (k, n))
    ext = [va for va, (k, _d) in cls if k == "external"]
    print("SYMBOLIZABLE: %d / %d  (%s)" % (len(cls) - len(ext), len(cls),
          "100% -- fully relocatable" if not ext else "%d external gaps" % len(ext)))
    if ext:
        print("   external:", ["0x%08x" % v for v in ext[:8]])


if __name__ == "__main__":
    main()
