# DumpXex.py - Ghidra headless post-script for the xexkit pipeline.
#
# Runs after XEXLoaderWV has imported and auto-analysis has completed. Writes a
# set of grep-able / machine-readable artifacts describing the program into an
# output directory.
#
# Invoked by ghidra_pipeline.py as:
#     -postScript DumpXex.py <OUTDIR> [decompile]
#
#   OUTDIR      where to write the artifacts (created if missing)
#   decompile   optional flag: also emit full decompilation (slow)
#
# This is Jython (Python 2.7) running inside Ghidra - keep it 2.x compatible.
#@category XEX

import json
import os
import re

from ghidra.program.model.symbol import SymbolType
from ghidra.util.task import ConsoleTaskMonitor

# Import stubs created by XEXLoaderWV are 1 byte; real PPC code is >= 4 bytes
# per instruction, so a sub-4-byte function body is always an import thunk.
# Unresolved imports are named "<lib>.xex_ord_<n>" / "<lib>.exe_ord_<n>".
_ORD_RE = re.compile(r"^(.+\.(?:xex|exe))_ord_(\d+)$")

try:
    from ghidra.program.util import DefinedDataIterator
    _HAVE_DDI = True
except ImportError:
    _HAVE_DDI = False


def _args():
    a = list(getScriptArgs())
    if not a:
        # Fall back to a sibling folder if no outdir was passed.
        a = [os.path.join(os.path.expanduser("~"), "xex_dump")]
    outdir = a[0]
    do_decompile = "decompile" in a[1:]
    return outdir, do_decompile


def write(path, text):
    f = open(path, "w")
    try:
        f.write(text)
    finally:
        f.close()


def is_import_stub(fn):
    return fn.getBody().getNumAddresses() < 4


def dump_functions(prog, outdir):
    """All functions, real code only (import stubs are excluded -> imports.txt)."""
    fm = prog.getFunctionManager()
    rows = ["address,name,size,is_thunk,is_import,calling_convention"]
    n = 0
    for fn in fm.getFunctions(True):
        imp = is_import_stub(fn)
        rows.append("%08X,%s,%d,%s,%s,%s" % (
            fn.getEntryPoint().getOffset(),
            fn.getName().replace(",", "_"),
            fn.getBody().getNumAddresses(),
            fn.isThunk(),
            imp,
            fn.getCallingConventionName(),
        ))
        if not imp:
            n += 1
    write(os.path.join(outdir, "functions.csv"), "\n".join(rows) + "\n")
    return n


def dump_imports(prog, outdir):
    """Imported APIs, recovered from the loader's import-stub functions.

    XEXLoaderWV resolves most Xbox import ordinals to real kernel/XAM/XUI API
    names (e.g. XamShowMessageBox); the rest stay as "<lib>.xex_ord_<n>", which
    tells us the source library directly.
    """
    fm = prog.getFunctionManager()
    by_lib = {}                 # lib -> set(api names) for unresolved-by-ordinal
    resolved = []               # API names where the ordinal was resolved
    count = 0
    for fn in fm.getFunctions(True):
        if not is_import_stub(fn):
            continue
        count += 1
        name = fn.getName()
        addr = fn.getEntryPoint().getOffset()
        m = _ORD_RE.match(name)
        if m:
            by_lib.setdefault(m.group(1), []).append((int(m.group(2)), addr))
        else:
            resolved.append((name, addr))

    lines = ["# %d import stubs (%d resolved to API names, %d unresolved ordinals)"
             % (count, len(resolved), sum(len(v) for v in by_lib.values())), ""]
    if resolved:
        lines.append("## resolved APIs")
        for name, addr in sorted(resolved):
            lines.append("%08X  %s" % (addr, name))
        lines.append("")
    for lib in sorted(by_lib):
        rows = sorted(set(by_lib[lib]))
        lines.append("## %s  (%d unresolved ordinals)" % (lib, len(rows)))
        for ordn, addr in rows:
            lines.append("%08X  %s_ord_%d" % (addr, lib, ordn))
        lines.append("")
    write(os.path.join(outdir, "imports.txt"), "\n".join(lines) + "\n")

    summary_libs = dict((lib, len(set(v))) for lib, v in by_lib.items())
    if resolved:
        summary_libs["(resolved)"] = len(resolved)
    return count, summary_libs


def dump_exports(prog, outdir):
    st = prog.getSymbolTable()
    lines = []
    it = st.getExternalEntryPointIterator()
    while it.hasNext():
        addr = it.next()
        syms = st.getSymbols(addr)
        name = syms[0].getName() if syms and len(syms) else "?"
        lines.append("%08X %s" % (addr.getOffset(), name))
    lines.sort()
    write(os.path.join(outdir, "exports.txt"), "\n".join(lines) + "\n")
    return len(lines)


def dump_strings(prog, outdir):
    lines = []
    if _HAVE_DDI:
        for data in DefinedDataIterator.definedStrings(prog):
            try:
                val = data.getValue()
            except Exception:
                val = None
            if val is None:
                continue
            s = unicode(val).encode("utf-8").replace("\n", "\\n")
            lines.append("%08X\t%s" % (data.getAddress().getOffset(), s))
    else:
        listing = prog.getListing()
        di = listing.getDefinedData(True)
        while di.hasNext():
            data = di.next()
            if data.hasStringValue():
                s = ("%s" % data.getValue()).replace("\n", "\\n")
                lines.append("%08X\t%s" % (data.getAddress().getOffset(), s))
    write(os.path.join(outdir, "strings.txt"), "\n".join(lines) + "\n")
    return len(lines)


def dump_segments(prog, outdir):
    """Memory block (section) map as the loader mapped it. With XEXLoaderWV this
    is the real XEX layout at the correct image base; the native-PE fallback
    shows the raw PE sections instead."""
    mem = prog.getMemory()
    rows = ["name,start,end,size,r,w,x,initialized"]
    n = 0
    for b in mem.getBlocks():
        rows.append("%s,%08X,%08X,%d,%d,%d,%d,%d" % (
            b.getName().replace(",", "_"),
            b.getStart().getOffset(), b.getEnd().getOffset(), b.getSize(),
            1 if b.isRead() else 0, 1 if b.isWrite() else 0,
            1 if b.isExecute() else 0, 1 if b.isInitialized() else 0))
        n += 1
    write(os.path.join(outdir, "segments.csv"), "\n".join(rows) + "\n")
    return n


def dump_callgraph(prog, outdir):
    """caller,callee edges between real (non-import) functions."""
    fm = prog.getFunctionManager()
    mon = ConsoleTaskMonitor()
    rows = ["caller,callee"]
    edges = 0
    for fn in fm.getFunctions(True):
        if is_import_stub(fn):
            continue
        caller = fn.getEntryPoint().getOffset()
        try:
            callees = fn.getCalledFunctions(mon)
        except Exception:
            callees = []
        for c in callees:
            rows.append("%08X,%08X" % (caller, c.getEntryPoint().getOffset()))
            edges += 1
    write(os.path.join(outdir, "callgraph.csv"), "\n".join(rows) + "\n")
    return edges


def dump_namespaces(prog, outdir):
    """Functions grouped by C++ namespace/class. Whenever the loader's demangler
    named a function Class::method, this recovers the class -> method map that
    stripped RTTI cannot. Empty when names stay as FUN_/sub_."""
    fm = prog.getFunctionManager()
    by_ns = {}
    named = 0
    for fn in fm.getFunctions(True):
        if is_import_stub(fn):
            continue
        ns = fn.getParentNamespace()
        if ns is None:
            continue
        qual = ns.getName(True)          # fully-qualified, e.g. std::vector
        if qual in ("Global", "", None):
            continue
        by_ns.setdefault(qual, []).append(
            (fn.getEntryPoint().getOffset(), fn.getName()))
        named += 1
    lines = ["# %d functions in %d namespaces/classes" % (named, len(by_ns)), ""]
    for ns in sorted(by_ns):
        lines.append("## %s  (%d)" % (ns, len(by_ns[ns])))
        for addr, nm in sorted(by_ns[ns]):
            lines.append("%08X  %s" % (addr, nm))
        lines.append("")
    write(os.path.join(outdir, "namespaces.txt"), "\n".join(lines) + "\n")
    return len(by_ns), named


def dump_data_symbols(prog, outdir):
    """User/loader-defined data symbols (globals, vtables, RTTI) with their type
    and any label. Vtables surface as arrays of pointers named vtbl/vftable."""
    st = prog.getSymbolTable()
    listing = prog.getListing()
    lines = ["address\ttype\tname"]
    n = 0
    di = listing.getDefinedData(True)
    while di.hasNext():
        data = di.next()
        sym = data.getPrimarySymbol()
        if sym is None or sym.getSymbolType() != SymbolType.LABEL:
            continue
        nm = sym.getName()
        if nm.startswith("DAT_") or nm.startswith("s_") or nm.startswith("u_"):
            continue
        tn = data.getDataType().getName() if data.getDataType() else "?"
        lines.append("%08X\t%s\t%s" % (
            data.getAddress().getOffset(), tn.replace("\t", " "),
            nm.replace("\t", " ")))
        n += 1
    write(os.path.join(outdir, "data_symbols.txt"), "\n".join(lines) + "\n")
    return n


def dump_decompilation(prog, outdir):
    from ghidra.app.decompiler import DecompInterface
    ifc = DecompInterface()
    ifc.openProgram(prog)
    mon = ConsoleTaskMonitor()
    fm = prog.getFunctionManager()
    out = open(os.path.join(outdir, "decompiled.c"), "w")
    n = 0
    try:
        for fn in fm.getFunctions(True):
            if fn.isThunk() or fn.isExternal() or is_import_stub(fn):
                continue
            out.write("// ===== %s @ %08X =====\n" % (
                fn.getName(), fn.getEntryPoint().getOffset()))
            try:
                res = ifc.decompileFunction(fn, 45, mon)
                if res and res.getDecompiledFunction():
                    out.write(res.getDecompiledFunction().getC())
                else:
                    out.write("// (decompile failed)\n")
            except Exception as e:
                out.write("// (exception %s)\n" % e)
            out.write("\n")
            n += 1
    finally:
        out.close()
    return n


def main():
    outdir, do_decompile = _args()
    if not os.path.isdir(outdir):
        os.makedirs(outdir)

    prog = currentProgram
    print("[DumpXex] %s -> %s" % (prog.getName(), outdir))

    n_func = dump_functions(prog, outdir)
    n_imp, import_libs = dump_imports(prog, outdir)
    n_exp = dump_exports(prog, outdir)
    n_str = dump_strings(prog, outdir)

    # Extra extractors: each guarded so one failure never aborts the pipeline.
    def _safe(fn, default):
        try:
            return fn()
        except Exception as e:
            print("[DumpXex] %s failed: %s" % (getattr(fn, "__name__", "?"), e))
            return default
    n_seg = _safe(lambda: dump_segments(prog, outdir), 0)
    n_edge = _safe(lambda: dump_callgraph(prog, outdir), 0)
    n_ns, n_ns_fn = _safe(lambda: dump_namespaces(prog, outdir), (0, 0))
    n_data = _safe(lambda: dump_data_symbols(prog, outdir), 0)

    n_dec = dump_decompilation(prog, outdir) if do_decompile else 0

    base = prog.getImageBase()
    summary = {
        "program": prog.getName(),
        "language": str(prog.getLanguageID()),
        "compiler": str(prog.getCompilerSpec().getCompilerSpecID()),
        "image_base": "%08X" % base.getOffset(),
        "entry_point": None,
        "min_address": "%08X" % prog.getMinAddress().getOffset(),
        "max_address": "%08X" % prog.getMaxAddress().getOffset(),
        "functions": n_func,
        "imports": n_imp,
        "import_libs": import_libs,
        "exports": n_exp,
        "strings": n_str,
        "segments": n_seg,
        "callgraph_edges": n_edge,
        "namespaces": n_ns,
        "namespaced_functions": n_ns_fn,
        "data_symbols": n_data,
        "decompiled": n_dec,
    }
    try:
        eps = prog.getSymbolTable().getExternalEntryPointIterator()
        if eps.hasNext():
            summary["entry_point"] = "%08X" % eps.next().getOffset()
    except Exception:
        pass
    write(os.path.join(outdir, "summary.json"), json.dumps(summary, indent=2))

    print("[DumpXex] functions=%d imports=%d exports=%d strings=%d "
          "segments=%d edges=%d namespaces=%d data=%d decompiled=%d"
          % (n_func, n_imp, n_exp, n_str, n_seg, n_edge, n_ns, n_data, n_dec))


main()
