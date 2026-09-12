#!/usr/bin/env python3
r"""enrich95.py -- wire the C++ / MFC / typing / naming engines into the PE deploy.

compose's fused lane recovers function BODIES; this adds everything the standalone engines recover
that the body pass doesn't, as ADDITIVE enrichment over a module's results:

  * PREAMBLE (module-scope, once per module): recovered C++ classes + inheritance (rtti), MFC
    CRuntimeClass hierarchies (mfcclass), vtable/COM class layouts (classlayout), MFC message/
    interface/dispatch maps (mfcmap), C++ EH try/catch frames (cxxeh), named globals (globalname),
    a cross-reference summary (xref), and a toolchain-identification banner (compiler_id).
  * NAME MAP: upgrade `sub_xxxx` to real names -- vtable slots to `Class::vftable[i]` (rtti +
    classlayout), mangled exports to readable C++ signatures (demangle), MFC ordinal imports via
    the period .def ground truth (mfcord).
  * PER-FUNCTION ANNOTATIONS (bounded, opt-in): recovered arg types (typeunify), inline stack
    strings (stackstring), recovered enums (enumrec), and synthesized structs (structsynth),
    attached as `/* recovered: ... */` comments -- never rewriting a verified body.

Every engine call is guarded: a failure (or "nothing found") degrades to no enrichment and NEVER
breaks the deploy. Nothing here fabricates -- each block is what a standalone engine actually
recovered, or it is omitted.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

# module-scope recovery engines (behavior group) + naming (symbol group). All optional.
_ENG = {}
for _name in ("rtti", "mfcclass", "mfcmap", "classlayout", "cxxeh", "xref", "compiler_id",
              "globalname", "structsynth", "typeunify", "stackstring", "enumrec", "constprop"):
    try:
        _ENG[_name] = __import__("src.engines.behavior." + _name, fromlist=[_name])
    except Exception:
        _ENG[_name] = None
try:
    from src.engines.symbol import demangle as _demangle
except Exception:
    _demangle = None
try:
    from src.engines.symbol import mfcord as _mfcord
except Exception:
    _mfcord = None


def _bare(t):
    """'class CFoo' -> 'CFoo' (strip the C++ aggregate keyword for use in a qualified name)."""
    for kw in ("class ", "struct ", "union ", "enum "):
        if t.startswith(kw):
            return t[len(kw):]
    return t


def _imgbase(module):
    try:
        import pe32util as H
        return H.image_base(module)
    except Exception:
        return 0


def display_name(name):
    """A readable name: demangle a mangled symbol (`?x@C@@...`), else return it unchanged."""
    if not name:
        return name
    if _demangle is not None and (name.startswith("?") or name.startswith("@")):
        try:
            d = _demangle.short(name)
            if d:
                return d
        except Exception:
            pass
    return name


def name_map(module):
    """{rva: recovered_name} for functions the body pass would leave as sub_xxxx: vtable methods
    named by their owning class (rtti -> classlayout slots)."""
    out = {}
    cl, rt = _ENG.get("classlayout"), _ENG.get("rtti")
    if cl is None or rt is None:
        return out
    try:
        base = _imgbase(module)
        vts, _va0 = cl.find_vtables(module)
        by_vt = {vt: slots for vt, slots in vts}
        r = rt.reconstruct(module)
        if not r.get("ok"):
            return out
        for c in r.get("classes", []):
            slots = by_vt.get(c["vtable"]) or []
            cname = _bare(c["name"])
            for i, va in enumerate(slots):
                rva = va - base if base and va >= base else va
                out.setdefault(rva, "%s::vf%d" % (cname, i))
    except Exception:
        return {}
    return out


def _block(title, text):
    return "/* ===== %s ===== */\n%s" % (title, text.rstrip()) if text and text.strip() else ""


def preamble(module):
    """The module-scope recovered-source header: classes, layouts, maps, EH, globals, xrefs,
    toolchain. Each section is included only if its engine actually recovered something."""
    parts = []
    # toolchain banner
    ci = _ENG.get("compiler_id")
    if ci is not None:
        try:
            info = ci.identify(module)
            lv = info.get("linker") or ""
            parts.append("/* toolchain: compiler=%s linker=%s%s */"
                         % (info.get("compiler", "?"), lv, "  rich" if info.get("has_rich") else ""))
        except Exception:
            pass
    # module-scope reconstructions that render C. Only engines whose output is backed by a real
    # on-disk structure (RTTI typeinfo, linked CRuntimeClass, vtable pointer arrays,
    # __CxxFrameHandler FuncInfo) are included -- they cannot emit a false positive from code bytes.
    # mfcmap is intentionally EXCLUDED here: its interface-map detection byte-scans for IID-shaped
    # runs and misreads x86 prologues as GUIDs, which would pollute the corpus. Re-add once its
    # interface-map scan is gated to the data section with a validity check.
    for key, title in (("rtti", "C++ classes (RTTI)"), ("mfcclass", "MFC CRuntimeClass hierarchy"),
                       ("classlayout", "vtable / COM class layouts"),
                       ("cxxeh", "C++ exception frames")):
        eng = _ENG.get(key)
        if eng is None:
            continue
        try:
            r = eng.reconstruct(module)
            if r.get("ok") and r.get("c"):
                parts.append(_block(title, r["c"]))
        except Exception:
            pass
    # named globals
    gn = _ENG.get("globalname")
    if gn is not None:
        try:
            gm = gn.suggest(module)
            decls = gn.declarations(gm) if gm else []
            if decls:
                parts.append(_block("recovered globals", "\n".join(decls)))
        except Exception:
            pass
    # cross-reference summary
    xr = _ENG.get("xref")
    if xr is not None:
        try:
            rep = xr.report(module)
            if rep and rep.strip():
                parts.append(_block("cross-reference summary", rep))
        except Exception:
            pass
    parts = [p for p in parts if p]
    if not parts:
        return ""
    return ("/* ========================================================================\n"
            "   ENRICHMENT -- module-scope source recovered by the C++/MFC/typing engines\n"
            "   (additive: classes, layouts, maps, EH, globals, xrefs; see enrich95.py)\n"
            "   ======================================================================== */\n"
            + "\n\n".join(parts) + "\n")


def preamble16(module, path=None):
    """The 16-bit (NE) module-scope preamble: resource<->code correlation (resxref16) and recovered
    DGROUP data (datainit16). The per-function enrichment (typed args, strings, callback sigs, WM_
    dispatch) is already applied inside interproc16, so this adds only the module-scope halves."""
    parts = []
    try:
        from src.engines.behavior import resxref16
        rep = resxref16.report(module, path)
        if rep and rep.lstrip().startswith("// resource <-> code"):
            parts.append(rep.rstrip())
    except Exception:
        pass
    try:
        from src.engines.recon import datainit16
        d = datainit16.reconstruct(module, path)
        if d.get("ok") and d.get("c"):
            parts.append("/* ===== recovered DGROUP data (datainit16) ===== */\n" + d["c"].rstrip())
    except Exception:
        pass
    if not parts:
        return ""
    return ("/* ===== ENRICHMENT: 16-bit module-scope recovery (resxref16 + datainit16) ===== */\n"
            + "\n\n".join(parts) + "\n")


def globals_map(module):
    gn = _ENG.get("globalname")
    if gn is None:
        return {}
    try:
        return gn.suggest(module) or {}
    except Exception:
        return {}


def apply_globals(src, gmap):
    """Rewrite dword_<addr> / [0x<addr>] global refs in an emitted body to recovered names."""
    gn = _ENG.get("globalname")
    if gn is None or not gmap or not src:
        return src
    try:
        return gn.apply(src, gmap)
    except Exception:
        return src


def annotate(module, rva, va=None):
    """A short `/* recovered: ... */` annotation for one function from the per-function static
    refiners (types, stack strings, enums, structs). Bounded + guarded; '' when nothing found."""
    base = _imgbase(module)
    if va is None:
        va = (base + rva) if base else rva
    hits = []
    ss = _ENG.get("stackstring")
    if ss is not None:
        try:
            strs = ss.recover(module, va)
            lits = strs.get("strings") if isinstance(strs, dict) else strs
            if lits:
                hits.append("stack strings " + ", ".join('"%s"' % s for s in list(lits)[:4]))
        except Exception:
            pass
    tu = _ENG.get("typeunify")
    if tu is not None:
        try:
            tys = tu.param_types(module, va)
            if tys and any(t not in ("int", "unsigned", None) for t in tys):
                hits.append("arg types (%s)" % ", ".join(str(t) for t in tys))
        except Exception:
            pass
    er = _ENG.get("enumrec")
    if er is not None:
        try:
            en = er.reconstruct(module, va)
            if en and (en.get("c") if isinstance(en, dict) else en):
                hits.append("enum recovered")
        except Exception:
            pass
    if not hits:
        return ""
    return "/* recovered: " + "; ".join(hits) + " */"


def resolve_imports(module):
    """Best-effort: resolve a module's MFC ordinal imports to names (mfcord). Returns a dict or {}.
    Kept separate so the caller can fold it into the import section when present."""
    if _mfcord is None:
        return {}
    try:
        return _mfcord.resolve(module) if hasattr(_mfcord, "resolve") else {}
    except Exception:
        return {}
