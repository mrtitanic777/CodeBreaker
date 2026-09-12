#!/usr/bin/env python3
r"""l2pipe.py -- the Layer-2 reconstruction conductor (32-bit PE).

Where conduct.py drives a module to byte-exact ASM (Track A, done), l2pipe drives
it to READABLE SOURCE, unattended:

  1. classify32  -- route every function (the unified type router)
  2. emit_c      -- clean C for the mechanical C classes, confirmed byte-exact by
                    the cc32 (MSVC 2.0 /O1) oracle
  3. reconcile_ml-- byte-exact ASM for everything else: asmrecon32's emitter +
                    native ML 6.11c, with a first-diverging-row db fallback that
                    guarantees byte-identity (worst case: all db). This is the
                    coverage FLOOR -- every function ends up buildable + exact.
  4. ledger + coverage report; --emit writes the clean C + a per-function ledger.

Whole-module, resumable-in-spirit, no DOSBox. Run under `wt --target win95` or
standalone; point it at a module, or loop it over a tier.

    python -m engine.targets.win95.l2pipe gdi32.dll [--limit N] [--emit DIR]
"""
import argparse
import json
import os
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))  # target dir on path for -m

import pe32util as H
import classify32
import emit_c
import emit_cpp
import cc32
import asmrecon32
from codebreaker.formats.pe_seg import PeSeg

_SG = {}


def _seg(module):
    if module not in _SG:
        _SG[module] = PeSeg(module)
    return _SG[module]


def _assemble_unit(items, labels=None):
    """Stitch per-function compile units into ONE clean translation unit: the
    shared prelude written once, target decls de-duplicated (preserving first-seen
    order), then each function body under a labelled comment."""
    decls, seen = [], set()
    blocks = []
    for nm, cs in items:
        ds, body = emit_c.split_unit(cs)
        for d in ds:
            if d not in seen:
                seen.add(d); decls.append(d)
        blocks.append((nm, body))
    out = [emit_c.WIN.rstrip("\n")]
    if decls:
        out.append("\n/* internal (sub_<rva>) and imported call targets */\n" + "\n".join(decls))
    out.append("")
    for nm, body in blocks:
        tag = ("  [%s]" % labels[nm]) if labels and nm in labels else ""
        out.append("/* ---- %s%s ---- */\n%s" % (nm, tag, body))
    return "\n".join(out) + "\n"


def reconcile_ml(sg, start, end, max_passes=60):
    """Byte-exact ASM for [start,end) via asmrecon32.emit + native ML, db-ing the
    first diverging row each pass. Returns (asm, ok, ndb, ni)."""
    import ml32
    rows = sorted(o for o, _ in sg.rows if start <= o < end)
    shipped = sg.code[start:end]
    smask = {m - start for m in sg.mask if start <= m < end}
    force_db = set()
    asm = ""
    for _ in range(max_passes):
        asm = asmrecon32.wrap32(asmrecon32.emit_function32(sg, start, end, force_db))
        obj, log = ml32.assemble(asm)
        if obj is None:
            if len(force_db) >= len(rows):
                return asm, False, len(force_db), len(rows)
            force_db = set(rows); continue
        code, mask = ml32.obj_code(obj)
        um = mask | smask
        n = min(len(code), len(shipped))
        oc = cc32.masked(code[:n], {m for m in um if m < n})
        sc = cc32.masked(shipped[:n], {m for m in um if m < n})
        if len(code) == len(shipped) and oc == sc:
            return asm, True, len(force_db), len(rows)
        bad = next((max(o for o in rows if o <= start + k) for k in range(n) if oc[k] != sc[k]),
                   rows[-1] if rows else None)
        if bad is None or bad in force_db:
            if len(force_db) >= len(rows):
                return asm, False, len(force_db), len(rows)
            force_db = set(rows); continue
        force_db.add(bad)
    return asm, False, len(force_db), len(rows)


def _shipped_db_asm(module, rva):
    """Ultimate byte-exact floor: emit the AUTHORITATIVE shipped bytes (from
    cc32.shipped_pe_function, PeCarve-based) as a raw db block. reconcile_ml
    slices from PeSeg.code, which can disagree with PeCarve on an odd layout
    (the lone gapia32!FInitIME escalation) -- its all-db then emits the wrong
    source. db-of-shipped is byte-exact by construction; ML round-trips it to
    confirm. Returns (asm, ok)."""
    import ml32
    sb, sm = cc32.shipped_pe_function(module, rva)
    body = ["        db      " + ", ".join("0%02xh" % x for x in sb[off:off + 16])
            for off in range(0, len(sb), 16)]
    asm = asmrecon32.wrap32(body)
    obj, _log = ml32.assemble(asm)
    if obj is None:
        return asm, False
    code, mask = ml32.obj_code(obj)
    um = mask | sm
    n = min(len(code), len(sb))
    ok = (len(code) == len(sb)
          and cc32.masked(code[:n], {m for m in um if m < n}) == cc32.masked(sb[:n], {m for m in um if m < n}))
    return asm, ok


def run(module, limit=None, emit=None, floor=True, budget=240):
    """Drive `module` to readable source. With floor=True (default) every
    non-clean-C function is taken to a byte-exact ASM floor via reconcile_ml --
    the 100%-coverage guarantee, but the slow part. With floor=False only the
    clean-C emit runs (classify + emit_best); the remainder is recorded
    'asm-pending' for a later floor pass. Use floor=False for a fast whole-corpus
    clean-C refresh after emitter changes (the ASM floor is emitter-independent)."""
    H.ensure_target()
    rows = [r for r in classify32.classify_module(module) if r["name"]]
    if limit:
        rows = rows[:limit]
    sg = _seg(module) if floor else None
    ledger = []
    cfiles = []
    cppfiles = []
    cppdrafts = []
    drafts = []
    cbatch = emit_c.emit_module(module, [r for r in rows if r["route"] == "c"])  # ONE compile
    # Clean C++ for __thiscall methods (getters/setters/field-address): the coverage
    # emit_c cannot reach (it bails on a live ecx `this`). Batched + gated by the same
    # cc32 oracle, compiled as C++. gen() fires only on method shapes, so passing every
    # row is cheap on C-only modules (empty candidate set -> no compile).
    cppbatch = emit_cpp.emit_module(module, rows)
    t0 = time.time()
    for r in rows:
        rec = dict(name=r["name"], rva=r["rva"], cat=r["cat"], route=r["route"])
        if r["route"] == "c":
            status, csrc, sym, fl, note = cbatch.get(r["rva"], (None, None, None, None, None))
            if status == "byte-exact":
                rec.update(kind="clean-c", verify="byte-exact", flags=fl)
                cfiles.append((r["name"] or "sub_%05x" % r["rva"], csrc)); ledger.append(rec); continue
            if status == "draft":
                rec["draft"] = True
                drafts.append((r["name"] or "sub_%05x" % r["rva"], csrc, note))
        cppst, cppsrc, _tag, cppfl, cppnote = cppbatch.get(r["rva"], (None, None, None, None, None))
        if cppst == "byte-exact":                         # __thiscall method, oracle-confirmed
            rec.update(kind="clean-cpp", verify="byte-exact", flags=cppfl)
            cppfiles.append((r["name"] or "sub_%05x" % r["rva"], cppsrc)); ledger.append(rec); continue
        if cppst == "draft" and not rec.get("draft"):
            rec["cpp_draft"] = True
            cppdrafts.append((r["name"] or "sub_%05x" % r["rva"], cppsrc, cppnote))
        # WATCHDOG: cap the (slow) ASM-floor wall-clock per module. A single huge module
        # (mfc40: ~6700 fns x up to 60 ML passes) must never wedge a corpus sweep -- past the
        # budget, defer the remaining floor work (later targeted pass) instead of hanging.
        over = floor and budget and (time.time() - t0) > budget
        if not floor or over:
            rec.update(kind="asm-pending", verify="deferred" + ("(budget)" if over else ""),
                       was_c=(r["route"] in ("c", "human")))
            ledger.append(rec); continue
        asm, ok, ndb, ni = reconcile_ml(sg, r["rva"], r["rva"] + r["size"])
        if ok:
            rec.update(kind="asm", verify="byte-exact", db=ndb, instrs=ni,
                       was_c=(r["route"] in ("c", "human")))
        else:
            _db_asm, db_ok = _shipped_db_asm(module, r["rva"])   # raw-shipped-bytes floor
            if db_ok:
                rec.update(kind="asm", verify="byte-exact", db=ni or 1, instrs=ni, rawdb=True,
                           was_c=(r["route"] in ("c", "human")))
            else:
                rec.update(kind="escalate", verify="none")
        ledger.append(rec)
    nC = sum(1 for r in ledger if r["kind"] == "clean-c")
    nCpp = sum(1 for r in ledger if r["kind"] == "clean-cpp")
    nA = sum(1 for r in ledger if r["kind"] == "asm" and not r.get("db"))
    nAdb = sum(1 for r in ledger if r["kind"] == "asm" and r.get("db"))
    nE = sum(1 for r in ledger if r["kind"] == "escalate")
    nP = sum(1 for r in ledger if r["kind"] == "asm-pending")
    review = sum(1 for r in ledger if r["kind"] == "asm" and r.get("was_c"))
    covered = nC + nCpp + nA + nAdb
    tot = len(rows)
    print("=== %s : %d functions ===" % (module, tot))
    print("  clean C (byte-exact)      : %d" % nC)
    print("  clean C++ (byte-exact)    : %d  [__thiscall methods]" % nCpp)
    if floor:
        print("  byte-exact ASM (0 db)     : %d" % nA)
        print("  byte-exact ASM (some db)  : %d" % nAdb)
        print("  escalated                 : %d" % nE)
        print("  COVERED (buildable exact) : %d/%d (%.0f%%)" % (covered, tot, 100.0 * covered / max(1, tot)))
    else:
        print("  clean C/C++ rate          : %d/%d (%.0f%%)" % (nC + nCpp, tot, 100.0 * (nC + nCpp) / max(1, tot)))
        print("  ASM floor pending         : %d" % nP)
    print("  clean-C drafts for review : %d (compile + call the shipped target)" % len(drafts))
    print("  clean-C++ drafts (review) : %d" % len(cppdrafts))
    print("  no-draft review worklist  : %d" % (review - len(drafts)))
    if emit:
        os.makedirs(emit, exist_ok=True)
        stem = module.replace(".", "_")
        if cfiles:
            with open(os.path.join(emit, stem + ".auto.c"), "w") as f:
                f.write("/* %s -- auto-reconstructed clean C (l2pipe), each function verified\n"
                        "   byte-exact against retail by the cc32 MSVC 2.0 /O1 oracle.\n"
                        "   Internal targets appear as sub_<rva>; imported ones by name. */\n\n" % module)
                f.write(_assemble_unit(cfiles))
        if cppfiles:
            with open(os.path.join(emit, stem + ".auto.cpp"), "w") as f:
                f.write("/* %s -- auto-reconstructed clean C++ (l2pipe), each __thiscall method\n"
                        "   verified byte-exact against retail by the cc32 MSVC 2.0 /O1 oracle\n"
                        "   (compiled as C++). Methods are unnamed internal functions (MFC exports\n"
                        "   by ordinal); each is emitted on a per-function struct C_<rva> with the\n"
                        "   field accessed via `this`-relative arithmetic. */\n\n" % module)
                for nm, cs in cppfiles:
                    f.write("/* ---- %s ---- */\n%s\n" % (nm, cs.rstrip()))
        if drafts:
            with open(os.path.join(emit, stem + ".review.c"), "w") as f:
                f.write("/* %s -- clean-C DRAFTS (l2pipe). Each compiles and calls the\n"
                        "   shipped target, but is NOT byte-identical -- a starting point for a\n"
                        "   human to confirm/adjust. The function is already covered byte-exact\n"
                        "   on the ASM floor; this is the readability upgrade. */\n\n" % module)
                f.write(_assemble_unit([(nm, cs) for nm, cs, _ in drafts],
                                       labels={nm: note for nm, _, note in drafts}))
        with open(os.path.join(emit, stem + ".l2ledger.json"), "w") as f:
            json.dump(ledger, f, indent=1)
        print("  wrote %s.auto.c (%d) + %s.auto.cpp (%d) + %s.review.c (%d drafts) + ledger"
              % (stem, len(cfiles), stem, len(cppfiles), stem, len(drafts)))
    return ledger


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module")
    ap.add_argument("--limit", type=int)
    ap.add_argument("--emit")
    ap.add_argument("--no-floor", action="store_true",
                    help="clean-C only; skip the (slow, emitter-independent) byte-exact ASM floor")
    ap.add_argument("--budget", type=int, default=240,
                    help="per-module ASM-floor wall-clock cap (s); past it, remaining functions "
                         "are deferred so one huge module can't wedge a sweep (0 = no cap)")
    a = ap.parse_args()
    run(a.module, a.limit, a.emit, floor=not a.no_floor, budget=a.budget)


if __name__ == "__main__":
    main()
