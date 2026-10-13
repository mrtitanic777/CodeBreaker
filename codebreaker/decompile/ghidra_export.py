"""ghidra_export.py -- Ghidra run headless as a decompiler: export a module's C, split it into
functions and adapt it so OpenWatcom compiles it.

A second opinion beside the embedded core (ghidra_core.py), which is the same decompiler fed
our facts and without the Java start-up. Used through codebreaker.decompile.engines ("export").
"""
import hashlib
import os
import re
import subprocess
from codebreaker.base import corpus


LANE = "pe-ghidra"


REV = 7                                    # bump when the adaptation below changes what is compiled


_FIXUPS = ("typedef unsigned int pointer32;\ntypedef unsigned int undefined3;\ntypedef void *pointer;\n"
           "typedef unsigned long long ulonglong;\ntypedef long long longlong;\n"
           "#define true 1\n#define false 0\n")


def export_path(module_path):
    d = os.path.join(corpus.design_dir(), "bank", "ghidra")
    os.makedirs(d, exist_ok=True)
    return os.path.join(d, os.path.basename(module_path).lower() + ".c")


def export(module_path, log=None):
    """Ghidra's C export of the module (cached). Returns the .c path, or None if Ghidra failed."""
    out = export_path(module_path)
    if os.path.exists(out) and os.path.exists(os.path.splitext(out)[0] + ".h"):
        return out
    try:
        from codebreaker.decompile import ghidra_headless as ghidra_decomp
        r = ghidra_decomp.decompile_export_one(module_path, out)
    except Exception as e:                             # Ghidra absent or misconfigured
        if log:
            log("ghidra export failed: %s" % e)
        return None
    if log and isinstance(r, dict) and r.get("skip"):
        log("ghidra export skipped: %s" % r["skip"])
    return out if os.path.exists(out) else None


def parse(c_path):
    """(globals lines, {name: text}, [names in order]) from a Ghidra C export."""
    lines = open(c_path, encoding="utf-8", errors="replace").read().split("\n")
    funcs, order, first, i = {}, [], None, 0
    while i < len(lines):
        ln = lines[i]
        if ln and ln[0] not in " \t/{}#" and ln.rstrip().endswith(")") and "(" in ln and ";" not in ln:
            j = i + 1
            while j < len(lines) and lines[j].strip() == "":
                j += 1
            if j < len(lines) and lines[j].startswith("{"):
                k = j
                while k < len(lines) and lines[k] != "}":
                    k += 1
                m = re.search(r"([A-Za-z_]\w*)\s*\([^()]*\)\s*$", ln) or re.search(r"([A-Za-z_]\w*)\s*\(", ln)
                name = m.group(1)
                if first is None:
                    first = i
                if name not in funcs:
                    funcs[name] = "\n".join(lines[i:k + 1])
                    order.append(name)
                i = k + 1
                continue
        i += 1
    return lines[:first or 0], funcs, order


def adapt(module_path, c_path):
    """-> (preamble, {name: text}, [names], {va: name}, [import names used]).

    The preamble is Ghidra's header made acceptable to a C compiler, its globals bound to the
    original's addresses, and imports declared with the convention they really use."""
    from codebreaker.emulate import api_stack as apiarity
    from codebreaker.formats.pe import open_pe
    from codebreaker.verify import body32 as B
    globs, funcs, order = parse(c_path)
    header = open(os.path.splitext(c_path)[0] + ".h", encoding="utf-8", errors="replace").read()

    def clean(t):
        """Spellings a C compiler does not take: a name Ghidra could not settle
        (`FID_conflict:x`) and the C++-only __thiscall."""
        return re.sub(r"\b__thiscall\b", "", t.replace("FID_conflict:", "FID_conflict_"))
    header = clean(header)
    globs = [clean(g) for g in globs]
    order = [clean(n) for n in order]
    funcs = {clean(n): clean(t) for n, t in funcs.items()}
    # Ghidra names a string after its text, which need not be an identifier
    # (`s_SOFTWARE\Microsoft\..._bfeca530`, `s_%lu%c%03lu_7fe9f17c`). Rename those by address.
    renames = {}
    for i, g in enumerate(globs):
        m = re.match(r"^(string|unicode)\s+(.+_([0-9a-f]{8}))(\[\d*\])?;\s*$", g)
        if m and not re.fullmatch(r"[A-Za-z_]\w*", m.group(2)):
            safe = "%s_%s" % ("s" if m.group(1) == "string" else "u", m.group(3))
            renames[m.group(2)] = safe
            globs[i] = "%s %s%s;" % (m.group(1), safe, m.group(4) or "")
    if renames:
        for old in sorted(renames, key=len, reverse=True):
            for n in funcs:
                if old in funcs[n]:
                    funcs[n] = funcs[n].replace(old, renames[old])
    text = "\n".join(funcs.values())
    hdr = _FIXUPS + header
    hdr = re.sub(r"\{\s*\}\s*;", "{ int _empty; };", hdr)            # C has no empty structs
    # Ghidra declares a function void and then uses its result elsewhere: every one returns int.
    hdr = re.sub(r"(?m)^void\s+(?=(?:__\w+\s+)?[A-Za-z_]\w*\s*\()", "int ", hdr)
    for n in list(funcs):
        funcs[n] = re.sub(r"\Avoid\s+", "int ", funcs[n])
    macros, plain = [], []
    for g in globs:
        m = re.match(r"^(.*?)\b([A-Za-z_]\w*?_([0-9a-f]{8}))(\[\d*\])?;\s*$", g)
        if not m:
            if g.strip() and not g.startswith("#include"):
                plain.append(g)
            continue
        ty, name, addr, arr = m.group(1).strip(), m.group(2), m.group(3), m.group(4)
        if name in funcs:
            continue
        if ty in ("string", "unicode"):                 # text: the name stands for its address
            macros.append("#define %s ((%s *)0x%s)" % (name, "char" if ty == "string" else "unsigned short", addr))
        else:
            macros.append(("#define %s ((%s *)0x%s)" if arr else "#define %s (*(%s *)0x%s)") % (name, ty, addr))
    for a in sorted(set(re.findall(r"\b_DAT_([0-9a-f]{8})\b", text))):  # the same cell at another width
        macros.append("#define _DAT_%s (*(unsigned int *)0x%s)" % (a, a))
    imports = B.import_map(module_path)
    called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", text))
    used = sorted(n for n in called if n in imports and n not in funcs)
    extra = []
    for n in used:
        conv = "__cdecl" if apiarity.stack_bytes(imports[n], n) == 0 else "__stdcall"
        pat = re.compile(r"(?m)^([^\n;(]*?)\b(?:__stdcall\s+|__cdecl\s+)?%s\s*\(([^;]*)\);" % re.escape(n))
        if pat.search(hdr):
            hdr = pat.sub(lambda mm: "%s %s %s(%s);" % (mm.group(1).rstrip(), conv, n, mm.group(2)), hdr, count=1)
        else:
            extra.append("int %s %s();" % (conv, n))
    preamble = "\n".join([hdr] + extra + plain + macros) + "\n"
    pe = open_pe(module_path)
    by_va = {}
    for n in funcs:
        m = re.fullmatch(r"FUN_([0-9a-f]{8})", n)
        if m:
            by_va[int(m.group(1), 16)] = n
    for nm, rva in pe.export_addrs().items():
        if nm in funcs:
            by_va[pe.image_base + rva] = nm
    return preamble, funcs, order, by_va, used
