#!/usr/bin/env python3
r"""symdb.py -- unified symbol/type database over the extracted period-SDK + beta-CodeView data.

Loads the JSON DBs produced by sdksyms.py (SDK import libs + headers) and the Build 311 CodeView
name harvest, and answers the questions the decompiler/naming pass asks:
  * signature(name) -> "RET name(t1, t2, ...)"   (from SDK headers, 3551 prototypes
                                                   incl. the STDAPI/WINOLEAPI OLE/COM cohort)
  * arity(name)     -> int argument count         (from SDK .lib @N + headers)
  * struct(name)    -> [(type, field), ...]        (1126 SDK struct layouts)
  * known(name)     -> which DB knows this name    ('sdk' | 'codeview' | None)
All HYPOTHESIS-tier (SDK/beta != OSR2): use to annotate/type, cross-check by structure, never to
override a byte-verified fact. DBs live in the win95 reference tree (gitignored, tool-consumed)."""
import json
import os

_REF = ("docs", "reference")
_CACHE = {}


def _find(fname):
    """Locate a reference JSON across the repo/data split. Resolves against the active data corpus
    (paths.data_root / WT_DATA_ROOT) and the cwd -- no in-repo corpus copy."""
    from codebreaker.core import paths
    env = os.environ.get("WT_DATA_ROOT")
    roots = []
    if env:
        roots += [env, os.path.join(env, "win95")]
    try:
        roots.append(paths.data_root())
    except Exception:
        pass
    roots += [os.getcwd(), os.path.join(os.getcwd(), "win95")]
    for r in roots:
        p = os.path.join(r, *_REF, fname)
        if os.path.exists(p):
            return p
    return None


def _load():
    if _CACHE:
        return _CACHE
    sdk = {}
    p = _find("sdk_symbols.json")
    if p:
        try:
            sdk = json.load(open(p, encoding="utf-8"))
        except (OSError, ValueError):
            sdk = {}
    cv = {}
    p = _find("build311_codeview_syms.json")
    if p:
        try:
            cv = json.load(open(p, encoding="utf-8"))
        except (OSError, ValueError):
            cv = {}
    # flatten: name -> arity (from lib @N across all modules), and the codeview name set
    arities = {}
    for dll, syms in sdk.get("32", {}).items():
        for name, a in syms.items():
            if isinstance(a, int) and name not in arities:
                arities[name] = a
    cvnames = set()
    for _mod, names in cv.items():
        cvnames.update(names)
    proto = sdk.get("proto", {})
    arity_lib = arities
    # 16-bit OMF IMPDEF map carries ordinals: {MODULE: {name: ordinal_or_expname}}. Invert to
    # {MODULE: {ordinal: name}} so an ordinal-only export can be named (the 32-bit COFF libs are
    # by-name, so they contribute no ordinal map -- 32-bit exports are named from the PE table).
    ord16 = {}
    for mod, syms in sdk.get("16", {}).items():
        inv = {}
        for name, ov in syms.items():
            if isinstance(ov, int):
                inv.setdefault(ov, name)
        if inv:
            ord16[mod.upper()] = inv
    # case-insensitive index: 16-bit export names are UPPERCASE, SDK names are proper-case
    ci = {k.lower(): k for k in proto}
    ci_arity = {k.lower(): k for k in arity_lib}
    ci_cv = {n.lower() for n in cvnames}
    _CACHE.update(proto=proto, struct=sdk.get("struct", {}), arity_lib=arity_lib,
                  cvnames=cvnames, ci=ci, ci_arity=ci_arity, ci_cv=ci_cv, ord16=ord16)
    return _CACHE


def _canon(name):
    return _load()["ci"].get(name.lower())        # canonical proto name for any-case input


def signature(name):
    """'RET name(t1, t2, ...)' from the SDK header prototype (case-insensitive), or None."""
    db = _load()
    c = db["ci"].get(name.lower())
    if not c:
        return None
    p = db["proto"][c]
    return "%s %s(%s)" % (p["ret"], c, ", ".join(p["params"]) or "void")


def arity(name):
    """Argument count from the SDK header prototype (preferred) or the .lib @N decoration."""
    db = _load()
    c = db["ci"].get(name.lower())
    if c is not None:
        return len(db["proto"][c]["params"])
    ca = db["ci_arity"].get(name.lower())
    return db["arity_lib"].get(ca) if ca else None


_MOD_ALIAS = {"KRNL386": "KERNEL", "KRNL286": "KERNEL"}   # file name -> SDK import-lib module


def ordinal_names(module):
    """{ordinal: name} for a module's exports, from the SDK's 16-bit OMF ordinal map. Keyed by
    the import-library module name (KERNEL/USER/GDI/...); accepts a file name (GDI.EXE, KRNL386)
    and normalizes it. Empty for 32-bit modules (the COFF libs are by-name -> no ordinal map)."""
    base = os.path.basename(module).upper()
    for ext in (".EXE", ".DLL", ".DRV", ".386", ".VXD"):
        if base.endswith(ext):
            base = base[:-len(ext)]
            break
    base = _MOD_ALIAS.get(base, base)
    return _load()["ord16"].get(base, {})


def struct(name):
    return _load()["struct"].get(name)


def known(name):
    db = _load()
    if name in db["proto"] or name in db["arity_lib"]:
        return "sdk"
    if name in db["cvnames"]:
        return "codeview"
    return None


def stats():
    db = _load()
    return {"prototypes": len(db["proto"]), "structs": len(db["struct"]),
            "lib_arities": len(db["arity_lib"]), "codeview_names": len(db["cvnames"])}


def main():
    import sys
    print("symdb:", stats())
    for n in sys.argv[1:] or ("CreateFileA", "GetDC", "SendMessageA", "AttApplyCharFormat"):
        print("  %-18s known=%s  arity=%s  sig=%s" % (n, known(n), arity(n), signature(n)))


if __name__ == "__main__":
    main()
