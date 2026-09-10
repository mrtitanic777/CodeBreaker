#!/usr/bin/env python3
r"""package.py -- turn a converted module into a REAL DROP-IN DLL.

symbolize.py makes the module editable+relocatable C; this packages it into a loadable DLL that
(a) EXPORTS the real Win95 API by name+ordinal (from the byte-exact PE's export table), so other
modules import it as a drop-in, and (b) SELF-RECONSTITUTES on load -- a generated DllMain that
lays the module's own .data image at a runtime buffer, populates the IAT with the real imports,
and sets _imgbase. Output: a folder with the .def, glue, embedded image, and a build command.

    python package.py <module.dll> --out DIR
"""
import os, re, sys, glob
_HERE = os.path.abspath(__file__)
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(_HERE)))))
from codebreaker.format_loaders.pe import open_pe
from codebreaker.foundation import paths
sys.path.insert(0, os.path.dirname(_HERE))
import symbolize
W = os.environ.get("WT_DATA_ROOT", paths.binaries("win95"))


def package(module, outdir):
    os.makedirs(outdir, exist_ok=True)
    pe = open_pe(os.path.join(W, "disc", "staged", "bin", module))
    base = pe.image_base
    tv = [s for s in pe.sections if s["chars"] & 0x20000000][0]["vaddr"]
    # 1) the relocatable editable C (reuses symbolize.emit)
    symbolize.emit(module, outdir)
    # 2) embed the module's .data IMAGE + the IAT import map
    simg = 0
    for s in pe.sections:
        simg = max(simg, s["vaddr"] + max(s["vsize"], s["rawsize"]))
    simg = (simg + 0xFFF) & ~0xFFF
    img = bytearray(simg)
    for s in pe.sections:
        raw = pe.data[s["rawptr"]:s["rawptr"] + s["rawsize"]]
        img[s["vaddr"]:s["vaddr"] + len(raw)] = raw
    imports = pe.imports()                                # {slot_va:(dll,name)}
    sym = module.replace(".", "_")
    with open(os.path.join(outdir, sym + "_pkg.h"), "w") as f:
        f.write("static const unsigned char IMGDATA[]={%s};\n" % ",".join(str(b) for b in img))
        f.write("#define IMGSZ %du\n#define IMGBASE 0x%08xu\n" % (simg, base))
        f.write("typedef struct{unsigned slotrva;const char*dll,*name;}imp_t;\n")
        f.write("static const imp_t IMPS[]={\n")
        for slot_va, (dll, name) in sorted(imports.items()):
            if not name.startswith("#"):
                f.write('  {0x%xu,"%s","%s"},\n' % (slot_va - base, dll, name))
        f.write("};\n#define NIMP (sizeof(IMPS)/sizeof(IMPS[0]))\n")
    # 3) the self-reconstituting DllMain
    with open(os.path.join(outdir, sym + "_glue.c"), "w") as f:
        f.write('#include <windows.h>\n#include "%s_pkg.h"\nextern unsigned _imgbase;\n' % sym)
        f.write("BOOL WINAPI DllMain(HINSTANCE h,DWORD r,LPVOID x){ unsigned i;\n"
                "  if(r==DLL_PROCESS_ATTACH){\n"
                "    unsigned char*b=(unsigned char*)VirtualAlloc(NULL,IMGSZ,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);\n"
                "    memcpy(b,IMGDATA,IMGSZ); _imgbase=(unsigned)(size_t)b;\n"
                "    for(i=0;i<NIMP;i++){HMODULE m=LoadLibraryA(IMPS[i].dll);FARPROC p=m?GetProcAddress(m,IMPS[i].name):0;\n"
                "      if(p)*(void**)(b+IMPS[i].slotrva)=(void*)p;}\n"
                "  } return TRUE; }\n")
    # 4) the export .def: real Win95 name+ordinal -> sub_<off>
    ex = pe.exports()
    items = ex.items() if isinstance(ex, dict) else ex
    lines = ["LIBRARY %s" % module.split(".")[0], "EXPORTS"]
    nexp = 0
    for t in items:
        if not (isinstance(t, (list, tuple)) and len(t) == 3):
            continue
        ordn, name, rva = t
        off = rva - tv
        if name and 0 <= off < 0x100000:
            lines.append("  %s=sub_%x @%d" % (name, off, ordn)); nexp += 1
    open(os.path.join(outdir, sym + ".def"), "w").write("\n".join(lines) + "\n")
    return sym, nexp, len(imports), simg


if __name__ == "__main__":
    mod = sys.argv[1]
    outdir = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else "."
    sym, nexp, nimp, simg = package(mod, outdir)
    print("packaged %s: %d real exports, %d imports wired, %d-byte image" % (mod, nexp, nimp, simg))
    print("  files: %s_reloc.c  %s_glue.c  %s_pkg.h  %s.def" % (sym, sym, sym, sym))
