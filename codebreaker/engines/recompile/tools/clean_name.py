#!/usr/bin/env python3
r"""clean_name.py -- the NAMING layer over the deregisterized, typed genuine-source tree. Renames
each function symbol `f_<MOD>_s1_<off>` to its REAL name where we know it EXACTLY: the module's PE
EXPORT table (export name -> that offset). The rename is applied to the WHOLE TU -- C definitions,
C call-sites, and the naked-asm `_f_..` references -- so the module still compiles, but now reads
with genuine Microsoft identifiers (RasEnumEntriesW, not f_RASAPI32_DLL_s1_0a157). Internal-symbol
naming (ms_symbols/.DBG, candidate names) is a later, lower-precision layer; exports are exact.

    (run from win95 data root)  python clean_name.py <build/clean dir>
"""
import os, re, sys, glob, json
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(HERE)))))
from codebreaker.core import cores
cores.set_target(paths.target_dir("win95"))
from codebreaker.formats.pe import open_pe
from codebreaker.core import paths

_IDENT = re.compile(r"^[A-Za-z_]\w*$")
_GEN = os.path.join(os.environ.get("WT_DATA_ROOT", "."), "src", "generated")
# N.1 import thunk: a fn whose body is a single `goto *(*(u32*)(<IAT slot addr>))` (deref of a slot).
_THUNK = re.compile(r"seg1:0x([0-9a-f]+)[^\n]*\*/\s*(?:u32|int|void|short|long)\s+\w+\([^)]*\)\s*\{\s*"
                    r"(goto\s*\*[^;\n]*\*\(u32\s*\*\)[^;\n]*);")


_MATCHED = None


def _matched_names(module, thresh=0.9):
    """N.3 CANDIDATE internal names from the behavioral matcher (reports/matched_names.json:
    {MODULE: {<sym>_<off>: [name, confidence]}}). High-confidence only. These are Microsoft's real
    names (Wine/ReactOS-anchored) matched by API/string/call-graph fingerprint -- authoritative
    names, but NOT offset-proven, so they are flagged in the file header."""
    global _MATCHED
    if _MATCHED is None:
        p = os.path.join(os.environ.get("WT_DATA_ROOT", "."), "reports", "matched_names.json")
        _MATCHED = json.load(open(p)) if os.path.exists(p) else {}
    mod = _MATCHED.get(module) or _MATCHED.get(module.upper()) or {}
    out = {}
    for k, v in mod.items():
        mm = re.match(r".+_([0-9a-f]+)$", k)
        if mm and isinstance(v, list) and len(v) >= 2 and v[1] >= thresh and _IDENT.match(v[0]):
            out[int(mm.group(1), 16)] = v[0]
    return out


def _import_thunks(module, pe, imgbase):
    """{off: import_name} — EXACT: an internal `jmp [IAT slot]` thunk named after the import it
    forwards to (offset -> slot VA from the generated C -> name from pe.imports())."""
    c = glob.glob(os.path.join(_GEN, "**", module + ".c"), recursive=True)
    if not c:
        return {}
    txt = open(c[0], encoding="utf-8", errors="replace").read()
    imp = pe.imports()
    slots = {va: nm for va, (dll, nm) in (imp.items() if isinstance(imp, dict) else imp)}
    out = {}
    for m in _THUNK.finditer(txt):
        off, stmt = int(m.group(1), 16), m.group(2)
        ha = re.search(r"0x([0-9a-f]+)", stmt)
        if not ha:
            continue
        va = int(ha.group(1), 16) + (imgbase if "_imgbase" in stmt else 0)
        nm = slots.get(va)
        if nm and _IDENT.match(nm):
            out[off] = nm
    return out


def name_file(path):
    base = os.path.basename(path)
    m = re.match(r"(.+)_clean\.c$", base)
    if not m:
        return None
    mm = re.match(r"(.+)_([A-Za-z0-9]+)$", m.group(1))   # MFCO40_DLL -> MFCO40.DLL
    module = "%s.%s" % (mm.group(1), mm.group(2)) if mm else m.group(1)
    try:
        pe = open_pe(cores.get("target").resolve(module))
    except Exception:
        return None
    tv = [s for s in pe.sections if s["chars"] & 0x20000000][0]["vaddr"]
    imgbase = pe.image_base
    txt = open(path, encoding="utf-8", errors="replace").read()
    _sm = re.search(r"\bf_([A-Za-z0-9_]+?)_s1_[0-9a-f]+\b", txt)   # sym from CONTENT (case-robust)
    sym = _sm.group(1) if _sm else module.replace(".", "_")
    exp = {}
    _ex = pe.exports()
    for t in (_ex.items() if isinstance(_ex, dict) else _ex):
        if isinstance(t, (list, tuple)) and len(t) == 3 and t[1] and _IDENT.match(t[1]):
            exp.setdefault(t[2] - tv, t[1])              # first (canonical) name wins for an offset
    thunks = _import_thunks(module, pe, imgbase)         # N.1 (exact)
    matched = _matched_names(module)                     # N.3 (candidate, behavioral)
    used = set()
    nexp = nthunk = nmatch = 0

    def _apply(off, nm):
        old = "f_%s_s1_%05x" % (sym, off)
        if old not in txt or nm in used:
            return False
        return True

    def _do(off, nm):
        nonlocal txt
        txt = re.sub(r"\b(_?)" + re.escape("f_%s_s1_%05x" % (sym, off)) + r"\b",
                     lambda g: g.group(1) + nm, txt)
        used.add(nm)

    for off, nm in sorted(exp.items()):                  # N.0 exports (exact) -- win all ties
        if _apply(off, nm):
            _do(off, nm); nexp += 1
    for off, nm in sorted(thunks.items()):               # N.1 import thunks (exact)
        if off not in exp and _apply(off, nm):
            _do(off, nm); nthunk += 1
    for off, nm in sorted(matched.items()):              # N.3 behavioral (candidate)
        if off not in exp and off not in thunks and _apply(off, nm):
            _do(off, nm); nmatch += 1
    if nmatch:                                           # honesty: flag the non-offset-proven names
        txt = ("/* NAMING: exports + import-thunks are EXACT (PE table); %d other internal names are\n"
               "   behaviorally-matched CANDIDATES (Wine/ReactOS-anchored, confidence>=0.9) -- real\n"
               "   Microsoft names but not offset-proven. */\n" % nmatch) + txt
    out = path.replace("_clean.c", "_named.c")
    open(out, "w", encoding="utf-8").write(txt)
    return nexp, nthunk, nmatch, os.path.basename(out)


def main():
    d = sys.argv[1] if len(sys.argv) > 1 else "."
    te = tt = tmz = 0
    for f in sorted(glob.glob(os.path.join(d, "*_clean.c"))):
        r = name_file(f)
        if r:
            nexp, nthunk, nmatch, out = r
            if nexp or nthunk or nmatch:
                print("  %-26s %4d exports + %4d thunks + %4d matched -> %s"
                      % (os.path.basename(f), nexp, nthunk, nmatch, out))
            te += nexp; tt += nthunk; tmz += nmatch
    print("== named %d exports (exact) + %d import-thunks (exact) + %d behavioral matches across the tree =="
          % (te, tt, tmz))


if __name__ == "__main__":
    main()
