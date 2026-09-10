# Ghidra headless post-script: apply a period GDT + function-discovery cross-check +
# decompile-all to one .c.  Pair with AggressivePre.py as the -preScript.
#   ... -preScript AggressivePre.py -postScript DecompilePipeline.py <out.c> [<gdt_path>]
import os
from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor

args = getScriptArgs()
# Positional args after the required out.c are recognized by extension, so the
# caller can pass a GDT and/or a names map in any order:
#   <out.c> [<types.gdt>] [<names.json>]
# names.json is a flat {"0xVA": "name", ...} symbol map applied before decompile,
# so call sites in the emitted C read as real names instead of FUN_xxxx.
outpath = args[0] if len(args) > 0 else (currentProgram.getName() + ".c")
gdt_path = None
names_path = None
for a in args[1:]:
    low = a.lower()
    if low.endswith(".json"):
        names_path = a
    elif low.endswith(".gdt"):
        gdt_path = a
    elif gdt_path is None:
        gdt_path = a                                   # back-compat: bare 2nd arg = gdt

prog = currentProgram
fm = prog.getFunctionManager()
mem = prog.getMemory()
monitor = ConsoleTaskMonitor()

# (0) Apply a period-correct type archive (Win32/DDK/Win16 GDT built by BuildGDT.py).
# Makes its types available for the decompiler and applies function signatures to
# matching named functions (exports/imports) before decompilation.
applied = 0
if gdt_path and os.path.exists(gdt_path):
    try:
        from ghidra.program.model.data import FileDataTypeManager
        from ghidra.app.cmd.function import ApplyFunctionDataTypesCmd
        from ghidra.program.model.symbol import SourceType
        from java.io import File
        arch = FileDataTypeManager.openFileArchive(File(gdt_path), False)
        ApplyFunctionDataTypesCmd([arch], None, SourceType.IMPORTED, False, False).applyTo(prog, monitor)
        applied = 1
    except:
        import sys
        print("GDT_APPLY_ERR= %s" % (sys.exc_info()[1],))


def in_exec(addr):
    b = mem.getBlock(addr)
    return b is not None and b.isExecute() and b.isInitialized()


before = fm.getFunctionCount()
created = 0

# (A) create functions at undefined CALL targets Ghidra's flow analysis left behind
rm = prog.getReferenceManager()
it = rm.getReferenceIterator(prog.getMinAddress())
while it.hasNext():
    ref = it.next()
    if ref.getReferenceType().isCall():
        to = ref.getToAddress()
        if in_exec(to) and fm.getFunctionAt(to) is None and fm.getFunctionContaining(to) is None:
            if createFunction(to, None) is not None:
                created += 1

# (B) prologue scan for UNREFERENCED functions: standard frame + hot-patch stub
for pat in ["\\x55\\x8b\\xec", "\\x8b\\xff\\x55\\x8b\\xec"]:
    hits = findBytes(prog.getMinAddress(), pat, 200000)
    if hits is None:
        continue
    for a in hits:
        if in_exec(a) and fm.getFunctionAt(a) is None and fm.getFunctionContaining(a) is None:
            if createFunction(a, None) is not None:
                created += 1

# (B2) apply a caller-supplied symbol map: name/rename functions before decompile
# so the emitted C uses real names at every call site (a generic rename pass; the
# map is produced outside Ghidra from exports/signatures/xrefs).
renamed = 0
if names_path and os.path.exists(names_path):
    try:
        import json as _json
        from ghidra.program.model.symbol import SourceType
        nm = _json.load(open(names_path))
        for va, name in nm.items():
            addr = toAddr(long(va, 16) if isinstance(va, basestring) else long(va))
            f = fm.getFunctionAt(addr)
            if f is None and in_exec(addr):
                f = createFunction(addr, None)
            if f is not None:
                f.setName(str(name), SourceType.USER_DEFINED)
                renamed += 1
    except:
        import sys as _sys
        print("NAMES_APPLY_ERR= %s" % (_sys.exc_info()[1],))

# (C) decompile everything
decomp = DecompInterface()
decomp.openProgram(prog)
funcs = list(fm.getFunctions(True))
lines = ["// Ghidra pipeline: %s | gdt_applied=%d named=%d before=%d created=%d total=%d"
         % (prog.getName(), applied, renamed, before, created, len(funcs))]
ok = 0
fail = 0
for f in funcs:
    try:
        res = decomp.decompileFunction(f, 60, monitor)
        if res is not None and res.decompileCompleted():
            lines.append(res.getDecompiledFunction().getC())
            ok += 1
        else:
            lines.append("// %s @ %s: FAILED" % (f.getName(), f.getEntryPoint()))
            fail += 1
    except Exception as e:
        lines.append("// %s: EXC %s" % (f.getName(), e))
        fail += 1

# Write UTF-8: decompiled C can contain non-ASCII (e.g. bytes from wide-string
# literals). Jython's default open() uses ASCII and would raise mid-write, leaving
# a 0-byte file, so go through codecs with replacement for anything stray.
import codecs
fh = codecs.open(outpath, "w", "utf-8", "replace")
fh.write((u"// %d ok, %d fail\n" % (ok, fail)) + u"\n\n".join(
    [unicode(x) for x in lines]))
fh.close()
print("PIPELINE_DONE gdt=%d named=%d before=%d created=%d ok=%d fail=%d total=%d"
      % (applied, renamed, before, created, ok, fail, len(funcs)))
