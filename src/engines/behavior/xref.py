#!/usr/bin/env python3
r"""xref.py -- a whole-program cross-reference index (navigation, not reconstruction).

Understanding a binary is as much about navigation as decompilation: which functions call this one,
which functions reference this string or import. This builds those indices for a whole module -- a
reverse call graph (callee -> its callers), a string index (literal text -> the functions that use
it), and an import index (API -> the functions that call it) -- so a reader (or another engine) can
jump straight to "everything that touches this". Built on callgraph + datarec; read-only.
"""
from src.engines.behavior import tracer


def callers_of(module):
    """{callee_off: [caller_off, ...]} -- the reverse of the static call graph."""
    from src.engines.behavior import callgraph
    g = callgraph.build(module)
    rev = {o: [] for o in g}
    for caller, info in g.items():
        for callee in info["calls"]:
            rev.setdefault(callee, []).append(caller)
    return rev


def string_refs(module):
    """{text: [func_off, ...]} -- functions that reference each data-section string literal."""
    import capstone
    import re
    from src.engines.behavior import datarec
    _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    hexre = re.compile(r"0x[0-9a-f]{5,}")
    out = {}
    for off in funcs:
        seen = set()
        for ins in md.disasm(code[off:funcs[off]["end"]], va0 + off):
            for h in hexre.findall(ins.op_str):
                try:
                    v = int(h, 16)
                except ValueError:
                    continue
                if v in seen:
                    continue
                seen.add(v)
                lit = datarec.render(module, v)
                if lit and (lit.startswith('"') or lit.startswith('L"')):
                    text = lit[lit.index('"') + 1:lit.rindex('"')]
                    if len(text) >= 3:
                        out.setdefault(text, [])
                        if off not in out[text]:
                            out[text].append(off)
    return out


def import_refs(module):
    """{api_name: [func_off, ...]} -- functions that call each imported API."""
    import capstone
    _, va0, code, _s, funcs, _sha, imports = tracer._load(module)
    by_iat = {va: nm for va, (dll, nm) in imports.items()}
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = {}
    for off in funcs:
        for ins in md.disasm(code[off:funcs[off]["end"]], va0 + off):
            if ins.mnemonic == "call" and "[" in ins.op_str and "0x" in ins.op_str:
                try:
                    tgt = int(ins.op_str[ins.op_str.index("0x"):].rstrip("]"), 16)
                except ValueError:
                    continue
                nm = by_iat.get(tgt)
                if nm:
                    out.setdefault(nm, [])
                    if off not in out[nm]:
                        out[nm].append(off)
    return out


def build(module):
    """The full xref index: {'callers','strings','imports'}."""
    return {"callers": callers_of(module), "strings": string_refs(module),
            "imports": import_refs(module)}


def report(module, top=20):
    """A human-readable summary of the most-referenced functions / strings / imports."""
    idx = build(module)
    lines = ["// xref index -- %s" % module]
    hot = sorted(idx["callers"].items(), key=lambda kv: -len(kv[1]))[:top]
    lines.append("\n// most-called functions:")
    for off, callers in hot:
        if callers:
            lines.append("//   sub_%x  <- %d caller(s)" % (off, len(callers)))
    lines.append("\n// most-referenced strings:")
    for text, offs in sorted(idx["strings"].items(), key=lambda kv: -len(kv[1]))[:top]:
        lines.append('//   "%s"  <- %d func(s)' % (text[:40], len(offs)))
    lines.append("\n// most-used imports:")
    for nm, offs in sorted(idx["imports"].items(), key=lambda kv: -len(kv[1]))[:top]:
        lines.append("//   %s  <- %d func(s)" % (nm, len(offs)))
    return "\n".join(lines)
