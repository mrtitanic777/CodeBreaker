#!/usr/bin/env python3
r"""compose.py -- the reconstruction COMPOSITION LAYER (win95 32-bit PE).

Three composition patterns over the existing engines (systems #3-#5), built on the
introspection substrate. They REUSE emit_c / emit_cpp / the asm floor -- no
reconstruction logic is reimplemented here; this is the wiring that makes the
pipeline descriptor-driven and honest-by-construction.

  * REFLECTED DELEGATION (`delegate`) -- route a function to the engine that fits
    it BY INTROSPECTING it (its descriptor's shape/convention), not by a static
    per-route table. Reflect on the target, then delegate: a `method` shape goes to
    emit_cpp, a c-route leaf/forwarder/const/wrapper to emit_c, everything else to
    the byte-exact asm floor.

  * DELEGATED DECORATORS (`@recorded`) -- wrap a delegate handler so every attempt
    automatically carries the cross-cutting concerns the pipeline always wants:
    timing, a uniform Result, and a provenance record (which engine, why, the
    signature source). The verify step already lives inside the emitters (the cc32
    byte-check); the decorator captures its verdict uniformly.

  * LINKED FACADES (`reconstruct`, `reconstruct_module`) -- one composed call that
    links introspect -> sigfactory -> reflected-delegate -> decorated attempt ->
    a single Result, so a caller never wires those stages by hand.

    python compose.py MODULE.dll[:RVA] [--limit N]
"""
import functools
import os
import sys
import time
from dataclasses import dataclass, field

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import pe32util as H
import introspect
import sigfactory
import emit_c
import emit_cpp

# The ENRICHMENT layer: the C++/MFC/typing/naming engines that recover what the body pass doesn't
# (classes, layouts, maps, EH, real names, globals, annotations). Optional; guarded everywhere.
try:
    import enrich95
except Exception:
    enrich95 = None

# The DYNAMIC lane (behavioral synthesis), for the static->dynamic fallback. Imported as a
# package so this stays the ONE place the two reconstruction philosophies are fused.
try:
    from src.engines.behavior import recon as behavrecon
except Exception:                                         # behavior engine optional (needs unicorn)
    behavrecon = None


@dataclass
class Result:
    module: str
    rva: int
    name: str
    shape: str
    delegate: str                     # emit_c | emit_cpp | floor
    reason: str                       # why the delegate was chosen (reflected)
    signature: str                    # sigfactory decl (readability)
    sig_source: str                   # sdk | inferred
    status: str                       # clean-c | clean-cpp | draft | floor | none
    source: str | None = None         # emitted C/C++ (when clean)
    verify: str = ""                  # byte-exact | draft | deferred
    note: str | None = None
    elapsed_ms: float = 0.0
    provenance: dict = field(default_factory=dict)
    display_name: str | None = None   # a real recovered name (demangle / rtti class method), if any


# --- reflected delegation ---------------------------------------------------

# c-route shapes emit_c can model; a 'method' is emit_cpp's; the rest are floor.
_EMIT_C_SHAPES = {"const", "forwarder", "wrapper", "leaf"}


def delegate(desc):
    """Reflected delegation: choose the reconstruction engine for a function from
    its descriptor. Returns (engine, reason)."""
    if desc.shape == "method" or desc.thiscall:
        return "emit_cpp", "thiscall method (this in ecx) -> C++ member modeler"
    if desc.route == "c" and desc.shape in _EMIT_C_SHAPES:
        return "emit_c", "c-route %s -> C free-function modeler" % desc.shape
    if desc.route == "c":
        return "emit_c", "c-route %s" % desc.cat
    return "floor", "%s/%s -> byte-exact asm floor" % (desc.route, desc.shape)


# --- delegated decorators ---------------------------------------------------

def recorded(engine_name):
    """Decorator factory: wrap a single-function handler `(module, desc, sig) ->
    (status, source, verify, note)` so it returns a fully-populated Result with
    timing + provenance. The composable form of the pipeline's attempt+record step."""
    def deco(fn):
        @functools.wraps(fn)
        def wrapper(module, desc, sig, reason):
            t0 = time.time()
            status, source, verify, note = fn(module, desc, sig)
            return Result(
                module=module, rva=desc.rva, name=sig.name, shape=desc.shape,
                delegate=engine_name, reason=reason, signature=sig.render_decl(),
                sig_source=sig.source, status=status, source=source, verify=verify,
                note=note, elapsed_ms=round((time.time() - t0) * 1000, 1),
                provenance=dict(convention=desc.convention, argc=desc.argc,
                                signedness=desc.signedness, cat=desc.cat))
        return wrapper
    return deco


@recorded("emit_c")
def _via_emit_c(module, desc, sig):
    row = _row(desc)
    st, src, _sym, _fl, note = emit_c.emit_best(module, row)
    if st == "byte-exact":
        return "clean-c", src, "byte-exact", None
    if st == "draft":
        return "draft", src, "draft", note
    return "floor", None, "deferred", "no clean-C candidate; covered on the asm floor"


@recorded("emit_cpp")
def _via_emit_cpp(module, desc, sig):
    row = _row(desc)
    st, src, _tag, _fl, note = emit_cpp.emit_best(module, row)
    if st == "byte-exact":
        return "clean-cpp", src, "byte-exact", None
    if st == "draft":
        return "draft", src, "draft", note
    return "floor", None, "deferred", "no clean-C++ candidate; covered on the asm floor"


@recorded("floor")
def _via_floor(module, desc, sig):
    return "floor", None, "deferred", "%s/%s: byte-exact asm floor (reconcile_ml)" % (desc.route, desc.shape)


_HANDLERS = {"emit_c": _via_emit_c, "emit_cpp": _via_emit_cpp, "floor": _via_floor}


def _row(desc):
    """The row dict the emitters' emit_best/emit_module expect, from a descriptor."""
    return dict(rva=desc.rva, name=desc.name, size=desc.size, cat=desc.cat,
                route=desc.route, retbytes=desc.retbytes)


# --- static -> dynamic fusion (system C) ------------------------------------

def _behavioral(module, rva, desc, sig):
    """The DYNAMIC-lane fallback: when the static lane floors a function, try faithful
    behavioral synthesis (recon's per-function router). Returns a Result or None. The
    behavior tracer addresses by section VA, so pass image_base + rva."""
    if behavrecon is None:
        return None
    try:
        va = H.image_base(module) + rva
        e = behavrecon.reconstruct_one(module, va)
    except Exception:
        return None
    if not e.get("method"):
        return None
    return Result(module=module, rva=rva, name=sig.name, shape=desc.shape,
                  delegate="behavior", reason="static floored -> faithful behavioral synthesis",
                  signature=sig.render_decl(), sig_source=sig.source,
                  status="behav-" + e["method"], source=e.get("c"),
                  verify="verified" if e.get("verified") else "unverified", note=e.get("summary"),
                  provenance=dict(behavior_method=e["method"], convention=desc.convention))


# --- linked facades ---------------------------------------------------------

def reconstruct(module, rva, fuse=True):
    """The linked facade: introspect -> signature -> reflected-delegate -> decorated
    static attempt; if that FLOORS and fuse=True, fall back to faithful behavioral
    synthesis. One call, best-available readable source (system C: the fused lanes)."""
    desc = introspect.describe(module, rva)
    sig = sigfactory.signature(module, rva, desc)
    engine, reason = delegate(desc)
    res = _HANDLERS[engine](module, desc, sig, reason)
    if fuse and res.status == "floor":
        b = _behavioral(module, rva, desc, sig)
        if b is not None:
            res = b
    _enrich(module, [res], annotate_limit=1)
    return res


def reconstruct_module(module, limit=None, fuse=True, behav_limit=30, annotate_limit=40):
    """Module facade: reconstruct every named function. Uses the batched emit_module
    for the emit_c/emit_cpp buckets (one compile per chunk, not per function), then
    packages uniform Results -- the composition layer at corpus speed."""
    ds = introspect.describe_module(module)
    if limit:
        ds = ds[:limit]
    routed = {}                                           # rva -> (engine, reason, desc, sig)
    for d in ds:
        engine, reason = delegate(d)
        routed[d.rva] = (engine, reason, d, sigfactory.signature(module, d.rva, d))
    cbatch = emit_c.emit_module(module, [_row(d) for _e, _r, d, _s in routed.values()
                                         if _e == "emit_c"])
    cppbatch = emit_cpp.emit_module(module, [_row(d) for _e, _r, d, _s in routed.values()
                                             if _e == "emit_cpp"])
    out = []
    for rva, (engine, reason, d, sig) in routed.items():
        if engine == "emit_c":
            st, src, _sym, _fl, note = cbatch.get(rva, (None, None, None, None, None))
            status, verify = ({"byte-exact": ("clean-c", "byte-exact"),
                               "draft": ("draft", "draft")}.get(st, ("floor", "deferred")))
        elif engine == "emit_cpp":
            st, src, _tag, _fl, note = cppbatch.get(rva, (None, None, None, None, None))
            status, verify = ({"byte-exact": ("clean-cpp", "byte-exact"),
                               "draft": ("draft", "draft")}.get(st, ("floor", "deferred")))
        else:
            st, src, note, status, verify = None, None, "asm floor", "floor", "deferred"
        out.append(Result(module=module, rva=rva, name=sig.name, shape=d.shape,
                           delegate=engine, reason=reason, signature=sig.render_decl(),
                           sig_source=sig.source, status=status,
                           source=src if status in ("clean-c", "clean-cpp") else None,
                           verify=verify, note=note,
                           provenance=dict(convention=d.convention, argc=d.argc,
                                           signedness=d.signedness, cat=d.cat)))
    # FUSION: the static lane floored these; try faithful behavioral synthesis, bounded
    # (each behavioral attempt emulates the function many times). Replace the Result in place.
    if fuse and behavrecon is not None:
        floored = [r for r in out if r.status == "floor"]
        by_rva = {r.rva: i for i, r in enumerate(out)}
        for r in floored[:behav_limit]:
            _e, _rn, d, sig = routed[r.rva]
            b = _behavioral(module, r.rva, d, sig)
            if b is not None:
                out[by_rva[r.rva]] = b
    # ENRICHMENT: real names (rtti/demangle), recovered-global rewriting, and per-function
    # annotations (types/strings/enums). Additive + guarded; never rewrites a verified body's bytes.
    _enrich(module, out, annotate_limit=annotate_limit)
    return out


def _enrich(module, out, annotate_limit=0):
    """Upgrade names, rewrite recovered globals, and annotate -- in place, best-effort."""
    if enrich95 is None:
        return
    try:
        nm = enrich95.name_map(module)
        gm = enrich95.globals_map(module)
    except Exception:
        nm, gm = {}, {}
    annotated = 0
    for r in out:
        try:
            disp = nm.get(r.rva) or enrich95.display_name(r.name)
            if disp and disp != r.name:
                r.display_name = disp
            if r.source and str(r.status).startswith("behav"):
                r.source = enrich95.apply_globals(r.source, gm)
            if r.source and r.status != "floor" and (annotate_limit == 0 or annotated < annotate_limit):
                ann = enrich95.annotate(module, r.rva)
                if ann:
                    r.source = ann + "\n" + r.source
                    annotated += 1
        except Exception:
            continue


def module_preamble(module):
    """The module-scope recovered-source header (classes/layouts/maps/EH/globals/xrefs/toolchain),
    for a writer to place at the top of <module>.fused.c. '' when nothing is recovered / disabled."""
    if enrich95 is None:
        return ""
    try:
        return enrich95.preamble(module)
    except Exception:
        return ""


def main():
    import argparse
    import collections
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("target", help="MODULE.dll or MODULE.dll:RVA")
    ap.add_argument("--limit", type=int)
    ap.add_argument("--behav-limit", type=int, default=30, help="max behavioral fallbacks/module")
    ap.add_argument("--no-fuse", action="store_true", help="static lane only (no behavioral fallback)")
    ap.add_argument("--emit", help="write fused reconstructions to <module>.fused.c")
    a = ap.parse_args()
    H.ensure_target()
    if ":" in a.target:
        mod, rva = a.target.split(":")
        r = reconstruct(mod, int(rva, 0), fuse=not a.no_fuse)
        for k, v in r.__dict__.items():
            if k == "source" and v:
                v = v.split("\n")[-2] if "\n" in v else v
            print("  %-12s %s" % (k, v))
        return
    rs = reconstruct_module(a.target, a.limit, fuse=not a.no_fuse, behav_limit=a.behav_limit)
    byv = collections.Counter(r.status for r in rs)
    static = sum(1 for r in rs if r.status in ("clean-c", "clean-cpp"))
    behav = sum(1 for r in rs if r.status.startswith("behav-"))
    floor = sum(1 for r in rs if r.status == "floor")
    tot = len(rs) or 1
    print("=== fused reconstruct %s : %d functions ===" % (a.target, len(rs)))
    print("  outcomes             :", dict(byv))
    print("  STATIC readable (byte-exact C/C++) : %d (%.0f%%)" % (static, 100.0 * static / tot))
    print("  DYNAMIC readable (behavioral)      : %d (%.0f%%)" % (behav, 100.0 * behav / tot))
    print("  COMBINED readable source           : %d/%d (%.0f%%)" % (static + behav, tot, 100.0 * (static + behav) / tot))
    print("  residue (asm floor)                : %d  <- the true frontier" % floor)
    print("  SDK-typed signatures : %d/%d" % (sum(1 for r in rs if r.sig_source == "sdk"), tot))
    if a.emit:
        os.makedirs(a.emit, exist_ok=True)
        stem = a.target.replace(".", "_")
        with open(os.path.join(a.emit, stem + ".fused.c"), "w", encoding="utf-8") as f:
            f.write("/* %s -- FUSED reconstruction: static byte-exact clean C/C++ where provable,\n"
                    "   faithful behavioral synthesis where the static lane floored. */\n\n" % a.target)
            pre = module_preamble(a.target)
            if pre:
                f.write(pre + "\n")
            for r in rs:
                if r.source:
                    f.write(_fn_header(r) + "%s\n\n" % r.source.rstrip())
        print("  wrote %s.fused.c" % stem)


def _fn_header(r):
    """The per-function comment banner: real recovered name when we have one, else sub_<rva>."""
    nm = r.display_name or ("sub_%x" % r.rva)
    tail = "" if nm == ("sub_%x" % r.rva) else "  (sub_%x)" % r.rva
    return "/* %s%s [%s %s] */\n" % (nm, tail, r.status, r.verify)


if __name__ == "__main__":
    main()
