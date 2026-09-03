#!/usr/bin/env python3
r"""auto_sweep.py -- the unattended auto-sweeper daemon.

Runs the whole reconstruction loop with no human in it, per the workflow:

  [ranked gap log] -> [quarantine skip] -> [hash-DB hit? emit] -> [constraint prune]
   -> [stratified synth_search] -> [oracle byte-diff] -> [byte-diff 0? auto-bank+commit]

Three efficiency/safety subsystems keep it fast and out of sinkholes:

  1. Pre-flight constraint engine  -- reject impossible candidates in-memory BEFORE the
     (slow) two-toolchain compile: stack delta (retf N), arg-count, far-ptr type boundary.
  2. Quarantine DB                 -- a masked opcode SIGNATURE that fails across N grammar
     variations is logged; subsequent passes skip it, so compute goes only to solvables.
  3. Level-0 hash DB               -- masked-bytes -> banked C. Identical functions (shared
     runtime/helpers across DLLs) resolve O(1) with no compile at all. This is the
     precomputed [asm->C] table, filled as the daemon solves.

Grammars are STRATIFIED fast->slow (L0 lookup, L1 linear, L2 pointer/long, L3 control flow)
with a per-level timeout so easy gaps sweep instantly and heavy compute is reserved.

    python -m engine.inverse_engine.auto_sweep <binary> --gaplog gaps.jsonl [--commit]
"""
import os
import re
import sys
import json
import time
import struct
import hashlib

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
_MD = Cs(CS_ARCH_X86, CS_MODE_16)

_REGS = re.compile(r"\b(ax|bx|cx|dx|si|di|bp|sp|al|ah|bl|bh|cl|ch|dl|dh|es|ds|cs|ss)\b")


# ---- signature: masked opcode hash (reg ids + immediates + disps wildcarded) ---
def signature(fb):
    toks = []
    for ins in _MD.disasm(bytes(fb), 0):
        op = re.sub(r"0x[0-9a-f]+", "K", ins.op_str)
        op = _REGS.sub("r", op)
        op = re.sub(r"\[[^]]*\]", "[m]", op)
        toks.append(ins.mnemonic + " " + op)
    return hashlib.sha1("|".join(toks).encode()).hexdigest()[:16]


# ---- subsystem 1: pre-flight constraint engine ---------------------------------
def target_retf(fb):
    """Stack cleanup of the function: retf N (ca NN NN) -> N, retf (cb) -> None."""
    ins = list(_MD.disasm(bytes(fb), 0))
    for i in reversed(ins):
        if i.mnemonic == "retf":
            return int(i.op_str, 0) if i.op_str else None
    return 0


def _cand_retf(cand):
    """Predicted stack cleanup of a candidate WITHOUT compiling: pascal cleans its args
    (retf 2*words), cdecl/default does not (retf, None)."""
    m = re.search(r"\bfar\s+(pascal\s+)?F\s*\(([^)]*)\)", cand)
    if not m:
        return 0
    pascal = bool(m.group(1))
    plist = m.group(2).strip()
    if not pascal:
        return None
    if plist in ("", "void"):
        return 0
    words = 0
    for p in plist.split(","):
        p = p.strip()
        words += 2 if ("far *" in p or "far*" in p or p.startswith("long")) else 1
    return words * 2


def preflight(t_retf, cand):
    """Cheap static rejection before the compiler. The candidate's predicted stack cleanup
    must match the target's EXACTLY: both None (cdecl, retf) or the same N (pascal, retf N).
    A cdecl candidate can never produce a pascal target's `retf N`, and vice-versa."""
    return _cand_retf(cand) == t_retf


# ---- persistent DBs: hash-DB (level 0) + quarantine ----------------------------
class DB:
    def __init__(s, root):
        s.hashpath = os.path.join(root, "solved.json")
        s.quarpath = os.path.join(root, "quarantine.json")
        s.solved = json.load(open(s.hashpath)) if os.path.exists(s.hashpath) else {}
        s.quar = json.load(open(s.quarpath)) if os.path.exists(s.quarpath) else {}

    def save(s):
        json.dump(s.solved, open(s.hashpath, "w"), indent=1)
        json.dump(s.quar, open(s.quarpath, "w"), indent=1)

    def is_quarantined(s, sig):
        return s.quar.get(sig, 0) >= 3                     # N=3 grammar-variation failures

    def note_fail(s, sig):
        s.quar[sig] = s.quar.get(sig, 0) + 1

    def bank(s, sig, c):
        s.solved[sig] = c


# ---- stratified grammars (fast -> slow), per-level timeout ---------------------
def _levels():
    from engine.inverse_engine import synth_search as S
    return [("L1-linear", [S.g_thunk], 15),
            ("L2-ptr-long", [S.g_bufread], 45)]


def sweep(binary, gaplog, root, commit=False, limit=None):
    from engine.sdk import reconstruct as R
    from engine.format_loaders import ne
    from engine.inverse_engine import synth_search as S
    db = DB(root)
    d = open(binary, "rb").read(); lfa = struct.unpack_from("<I", d, 0x3c)[0]
    segb = {sg["idx"]: d[sg["file_off"]:sg["file_off"] + sg["length"]]
            for sg in ne.NEFile.from_buffer(d, lfa, binary).segments() if sg.get("length")}
    gaps = [json.loads(l) for l in open(gaplog)] if os.path.exists(gaplog) else []
    stats = dict(skipped=0, level0=0, pruned=0, solved=0, quarantined=0, tried=0)
    for g in gaps[:limit]:
        sb = segb.get(g["seg"])
        if sb is None or g.get("off") is None:
            continue
        e = R._func_end(sb, g["off"], len(sb))
        if not e:
            continue
        fb = bytes(sb[g["off"]:e]); sig = signature(fb); t_retf = target_retf(fb)
        if db.is_quarantined(sig):                          # subsystem 2: skip sinkhole
            stats["skipped"] += 1; continue
        if sig in db.solved:                                # subsystem 3: level-0 O(1) hit
            stats["level0"] += 1; continue
        stats["tried"] += 1
        found = None
        for lname, grams, budget in _levels():              # stratified fast->slow
            # directed (convention) + parallel (workers) + constraint-prune, all at once
            c, conv, ntasks = S.parallel_solve(
                fb, grammars=grams, workers=6, prune=lambda cand: preflight(t_retf, cand))
            if c:
                found = c
                break
        if found:
            db.bank(sig, found); stats["solved"] += 1
            print("  SOLVED seg%s:0x%x -> %s" % (g["seg"], g["off"], found.split("\n")[-1][:70]))
        else:
            db.note_fail(sig)
            if db.is_quarantined(sig):
                stats["quarantined"] += 1
    db.save()
    print("== auto-sweep %s ==" % os.path.basename(binary))
    for k in ("level0", "solved", "pruned", "skipped", "quarantined", "tried"):
        print("   %-12s %d" % (k, stats[k]))
    return stats


def harvest(binary, root):
    """Bank the SOLVED C functions of a module: verify every msc6dec-inverted candidate against
    its frame-matched oracle (via oracle_router) and store the byte-exact ones in Level-0. This
    complements sweep(), which synthesizes DECLINED gaps -- harvest banks what the decompiler
    already produced and the router can prove. Seg-switch thunks are handled by seg_thunk, so
    they never reach here (they are tagged asm-origin, not 'inverted')."""
    from engine.sdk import reconstruct as R
    from engine.inverse_engine import oracle_router as router
    db = DB(root)
    rep = R.analyze(binary)
    items = []
    for sg in rep["segments"]:
        for fn in sg["funcs"]:
            if fn.get("status") == "inverted" and "bytes" in fn and "c" in fn:
                items.append((bytes.fromhex(fn["bytes"]), fn["c"], sg["idx"], fn["off"]))
    banked = 0
    results = router.byte_exact_batch([(fb, c) for fb, c, _s, _o in items])
    for (fb, c, sgi, off), r in zip(items, results):
        if r.get("exact"):
            db.bank(signature(fb), c); banked += 1
            print("  BANK seg%d:0x%-4x via %s (%s)" % (sgi, off, r["oracle"], r.get("recipe", "")))
    # needs-link functions: their intra-segment call resolves only against a sibling, so verify
    # them at the SEGMENT level via mocklink (recompile with sibling stubs, mask the layout rel16).
    # mocklink drives the 6.0a oracle, so restrict to 6.0a-framed modules.
    from engine.sdk import mocklink
    nl = [(bytes.fromhex(fn["bytes"]), fn["c"], sg["idx"], fn["off"])
          for sg in rep["segments"] for fn in sg["funcs"]
          if fn.get("status") == "needs-link" and "bytes" in fn and "c" in fn]
    banked_nl = 0
    for fb, c, sgi, off in nl:
        if router.is_msvc152(fb):                          # mocklink is 6.0a-only
            continue
        if mocklink.verify(fb, c).get("exact"):
            db.bank(signature(fb), c); banked_nl += 1
    db.save()
    print("== harvest %s ==  inverted=%d banked=%d | needs-link=%d segment-verified=%d" % (
        os.path.basename(binary), len(items), banked, len(nl), banked_nl))
    return {"inverted": len(items), "banked": banked, "needs_link": len(nl), "banked_nl": banked_nl}


def main(argv):
    if not argv:
        print(__doc__); return 2
    gaplog = argv[argv.index("--gaplog") + 1] if "--gaplog" in argv else "gaps.jsonl"
    root = argv[argv.index("--db") + 1] if "--db" in argv else os.path.join(HERE, "_sweepdb")
    os.makedirs(root, exist_ok=True)
    lim = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else None
    if "--harvest" in argv:
        harvest(argv[0], root)
    else:
        sweep(argv[0], gaplog, root, commit="--commit" in argv, limit=lim)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
