#!/usr/bin/env python3
r"""imgrecon.py -- the reconstitution-loader DATA LAYER for whole-module 'runs'.

A hybrid_emit.py module compiles+links, but its functions reference their own .data and imports
by ABSOLUTE original-load VA (ImageBase+RVA) -- e.g. `xchg al,[0xbfea2016]`, `call [0xbfea3070]`.
To make a whole module RUN, a 32-bit harness must reconstitute that memory image: reserve the
region at ImageBase, lay down each section's bytes, and populate the IAT slots with real function
pointers. This emits the SPEC the harness needs, as a C header:

  * IMG_BASE / IMG_SIZE                -- VirtualAlloc target
  * img_sections[]  {va, len, bytes}   -- section bytes to memcpy into place
  * img_imports[]   {slot_va, "dll", "name"}  -- IAT slots to fill via GetProcAddress

    python imgrecon.py <module.dll> [--out DIR]     reads the byte-exact PE from disc/staged/bin
"""
import os, sys
W = os.environ.get("WT_DATA_ROOT", paths.binaries("win95"))
_HERE = os.path.abspath(__file__)                            # engine/recompile95/tools/imgrecon.py
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(_HERE)))))  # -> CB root
from codebreaker.format_loaders.pe import open_pe
from codebreaker.foundation import paths


def spec(module, outdir):
    pe = open_pe(os.path.join(W, "disc", "staged", "bin", module))
    base = pe.image_base
    size = 0
    secs = []
    for s in pe.sections:
        va = base + s["vaddr"]
        raw = pe.data[s["rawptr"]:s["rawptr"] + s["rawsize"]]
        end = s["vaddr"] + max(s["vsize"], s["rawsize"])
        size = max(size, end)
        secs.append((va, s["name"], raw))
    size = (size + 0xFFF) & ~0xFFF
    imports = pe.imports()                                   # {slot_va: (dll, name)}
    sym = module.replace(".", "_")
    os.makedirs(outdir, exist_ok=True)
    dst = os.path.join(outdir, sym + "_img.h")
    with open(dst, "w") as f:
        f.write("/* AUTO reconstitution spec for %s (ImageBase 0x%08x). */\n" % (module, base))
        f.write("#define IMG_BASE 0x%08xu\n#define IMG_SIZE 0x%08xu\n\n" % (base, size))
        f.write("typedef struct { unsigned va, len; const unsigned char *bytes; } imgsec_t;\n")
        for i, (va, name, raw) in enumerate(secs):
            f.write("static const unsigned char sec%d[] = {%s};\n"
                    % (i, ",".join(str(b) for b in raw)) if raw else "static const unsigned char sec%d[1]={0};\n" % i)
        f.write("static const imgsec_t img_sections[] = {\n")
        for i, (va, name, raw) in enumerate(secs):
            f.write('  {0x%08xu, %du, sec%d},  /* %s */\n' % (va, len(raw), i, name))
        f.write("};\n#define IMG_NSEC %d\n\n" % len(secs))
        f.write("typedef struct { unsigned slot; const char *dll, *name; } imgimp_t;\n")
        f.write("static const imgimp_t img_imports[] = {\n")
        for slot_va, (dll, name) in sorted(imports.items()):
            f.write('  {0x%08xu, "%s", "%s"},\n' % (slot_va, dll, name))
        f.write("};\n#define IMG_NIMP %d\n" % len(imports))
    return dst, base, size, len(secs), len(imports)


if __name__ == "__main__":
    mod = sys.argv[1]
    outdir = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else "."
    dst, base, size, ns, ni = spec(mod, outdir)
    print("wrote %s\n  ImageBase=0x%08x size=0x%x  sections=%d  imports=%d" % (dst, base, size, ns, ni))
