#!/usr/bin/env python3
r"""
matchbatch.py — unattended matching-decompilation at corpus scale.

For each conducted function, classify its shape into a known structural pattern
(far-pascal thin wrapper, guarded wrapper, const-return, register getter/setter),
auto-generate a small set of compilable C candidates, compile each under the
recovered retail pin (/AM /Os /G2 /Gs), segdiff vs the shipped bytes, and PROMOTE
any that byte-match. Functions with no matching pattern go to a ranked residual
queue for permute/manual work. Parallel across cores; resumable ledger.

This does NOT try to decompile arbitrary code (the unsolved core of decompilation)
— it drives the SIMPLE MAJORITY (Windows API is wrapper/guard/getter-heavy) to
provably-byte-exact C automatically, and reports exactly what's left.

    python matchbatch.py --module WINTUTOR         # one module
    python matchbatch.py --tier apps -j 8          # a tier
    python matchbatch.py                           # whole corpus
"""
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, ROOT)
sys.path.insert(0, os.environ.get("WT_DATA_ROOT", os.getcwd()))
# engine (segdiff lives per-target; ne lives in engine)
_ENG = os.path.join(os.path.dirname(ROOT), "engine")
if os.path.isdir(_ENG):
    sys.path.insert(0, _ENG)

PIN = "/AM /Os /G2 /Gs /c"
LEDGER_DIR = os.path.join("reports", "matchdecomp")


# ---- asm parsing: emitted .asm -> logical instruction stream -----------------
def parse_func(path):
    """Return (seq, retf_n, calls) where seq is a list of normalized logical
    instructions (real mnemonics + db-comment mnemonics), retf_n the byte count
    the epilogue cleans, calls the imported targets in order."""
    try:
        L = open(path, encoding="latin1").read().splitlines()
    except OSError:
        return None
    try:
        s = next(i for i, l in enumerate(L) if "segment" in l and "_TEXT" in l)
        e = next(i for i, l in enumerate(L) if "_TEXT" in l and "ends" in l)
    except StopIteration:
        return None
    seq, calls, retf_n = [], [], 0
    for l in L[s + 1:e]:
        raw = l.rstrip("\n")
        if not raw.strip() or raw.strip().startswith(("_TEXT",)):
            continue
        if raw.strip().endswith(":"):                       # label
            seq.append(("label", raw.strip()[:-1]))
            continue
        code = raw.split(";", 1)[0].strip()
        comment = raw.split(";", 1)[1].strip() if ";" in raw else ""
        if code.startswith(("db ", "dw ")):
            # logical instruction is in the comment: "OFFSET mnem ops [-> import]"
            m = comment.split(None, 1)
            logical = m[1] if len(m) > 1 else ""
        else:
            logical = code
        logical = logical.strip()
        if not logical or logical.split()[0] in ("assume", "public", "extrn", "org", "even"):
            continue                                          # assembler directive, not code
        imp = None
        if "->" in logical:
            imp = logical.split("->", 1)[1].strip().split()[0]
            logical = logical.split("->", 1)[0].strip()
        mn = logical.split()[0] if logical else ""
        if mn in ("ret", "retf"):
            m = re.search(r"(0x[0-9a-f]+|[0-9a-f]+h|\d+)", logical)
            if m:
                v = m.group(1).rstrip("h")
                retf_n = int(v, 16) if re.search("[a-f]", v) or logical.count("h") else int(v, 16)
            seq.append(("ret", logical))
            continue
        if mn == "call":
            calls.append(imp or "?")
        seq.append((mn, logical))
    return seq, retf_n, calls


def _mnems(seq):
    return [m for m, _ in seq if m != "label"]


# ---- pattern -> candidate C --------------------------------------------------
def _pushes(seq):
    """Ordered list of pushed operands between the frame and the call:
    ('arg', bp_off) for [bp+N], ('imm', value) for an immediate."""
    out = []
    for mn, txt in seq:
        if mn != "push":
            continue
        mb = re.search(r"\[bp\+([0-9a-fx]+)h?\]", txt)
        if mb:
            out.append(("arg", int(mb.group(1).rstrip("h"), 16)))
            continue
        mi = re.search(r"push\s+([0-9][0-9a-f]*)h?$", txt)   # imm starts with a digit
        if mi:
            out.append(("imm", int(mi.group(1).rstrip("h"), 16)))
        else:
            out.append(("other", txt))                        # register/segment push
    return out


def _is_far_export(mn):
    """The standard Win16 far-exported prologue: push ds; pop ax; nop; inc bp; ...
    (in the shipped bytes the DS-load is the linker's loadds thunk). These functions
    compile with /Gw and need the 2-byte export-prologue fixup at match time."""
    return mn[:4] == ["push", "pop", "nop", "inc"]


def candidates(seq, retf_n, calls):
    """Yield compilable C strings whose function symbol is F, for recognized
    shapes. Empty -> unrecognized (residual)."""
    mn = _mnems(seq)
    has_frame = (mn[:2] == ["push", "mov"] or _is_far_export(mn))
    ncall = mn.count("call")
    pushes = _pushes(seq)
    nparam = max(0, (retf_n // 2))                       # word params cleaned by retf
    args = ", ".join("int a%d" % i for i in range(nparam)) or "void"
    # map a push to its C expression
    def pexpr(p):
        if p[0] == "arg":
            idx = (p[1] - 6) // 2                          # bp+6 = a0, bp+8 = a1
            return "a%d" % idx if 0 <= idx < nparam else None
        if p[0] == "imm":
            return str(p[1])
        return None

    # ---- pattern: thin far-pascal wrapper: frame, pushes, ONE call, epilogue --
    if has_frame and ncall == 1 and "cmp" not in mn and "je" not in mn:
        argpushes = [p for p in pushes if p[0] in ("arg", "imm")]  # drop frame push bp etc.
        callargs = [pexpr(p) for p in argpushes]
        if all(a is not None for a in callargs):
            proto = "extern int __far __pascal EXT(%s);" % (", ".join("int" for _ in callargs) or "void")
            # __pascal push order vs source order is a fixed permutation; try both so
            # a pass-through forwarder matches regardless of which way it reads.
            orders = [callargs] if len(callargs) < 2 else [callargs, list(reversed(callargs))]
            for order in orders:
                al = ", ".join(order)
                for body in ("return EXT(%s);" % al, "EXT(%s);" % al):
                    yield "%s\nint __far __pascal F(%s){ %s }\n" % (proto, args, body)
        return

    # ---- pattern: guarded wrapper: frame, cmp arg,0; je; push arg; call -------
    if has_frame and ncall == 1 and "cmp" in mn and "je" in mn:
        # the guard is on the first param; call passes it
        argpushes = [p for p in pushes if p[0] in ("arg", "imm")]
        callargs = [pexpr(p) for p in argpushes]
        if callargs and all(a is not None for a in callargs):
            al = ", ".join(callargs)
            proto = "extern int __far __pascal EXT(%s);" % (", ".join("int" for _ in callargs) or "void")
            guard = callargs[0]
            for body in ("if(%s) EXT(%s);" % (guard, al), "if(%s) return EXT(%s);" % (guard, al)):
                yield "%s\nint __far __pascal F(%s){ %s }\n" % (proto, args, body)
        return

    # ---- pattern: constant return: no call, mov ax,imm; epilogue -------------
    _CONST = r"mov ax,\s*([0-9][0-9a-f]*)h?$"             # imm starts with a digit
    if has_frame and ncall == 0 and any(re.match(_CONST, t) for _, t in seq):
        m = next(t for _, t in seq if re.match(_CONST, t))
        val = int(re.match(_CONST, m).group(1).rstrip("h"), 16)
        yield "int __far __pascal F(%s){ return %d; }\n" % (args, val)
        return
    return


def _match_obj(obj, size, tgt, tmask, far=False):
    """True if a compiled candidate's function F matches the shipped bytes (mask-aware).
    For far-exported functions, apply the linker's export-prologue fixup: the compiler
    emits `mov ax,ds` (8C D8) as the DS-load; the Win16 linker rewrites it to the
    loader-patchable `push ds; pop ax` (1E 58) for exported entries. Same operation,
    fixed 2-byte substitution -- rewriting it lets the compiler codegen be compared."""
    if not obj:
        return False
    from engine.format_loaders import objextract
    try:
        g = objextract.Obj(obj).functions()
    except Exception:
        return False
    if "F" not in g:
        return False
    ob, om = g["F"]
    if len(ob) < size:
        return False
    ob = bytearray(ob)
    if far and ob[:2] == b"\x8c\xd8":
        ob[0:2] = b"\x1e\x58"
    our = bytearray(ob[:size])
    shp = bytearray(tgt)
    for m in tmask:
        if m < size:
            shp[m] = 0
    for m in om:
        if m < size:
            our[m] = 0
    return bytes(our[:size]) == bytes(shp)


def _asm_dir(module):
    from engine.foundation import layout
    stem = module.rsplit(".", 1)[0]
    idx = layout.asm_dir_index()                    # resolve at any depth (component tree)
    for cand in (module.lower(), stem.lower()):
        d = idx.get(cand)
        if d and os.path.isdir(d):
            return d
    return None


def _module_funcs(module):
    for cand in (module.lower(), module.rsplit(".", 1)[0].lower()):
        lp = os.path.join("reports", "conduct", cand + ".json")
        if os.path.exists(lp):
            break
    else:
        return []
    led = json.load(open(lp))
    adir = _asm_dir(module)
    if not adir:
        return []
    out = []
    for key, rec in led.items():
        if rec.get("status") != "exact":
            continue
        seg = int(key.split(":")[0]); off = int(key.split(":")[1], 16)
        p = os.path.join(adir, "seg%d_%04x.asm" % (seg, off))
        if os.path.exists(p):
            out.append((seg, off, rec.get("size", 0), p))
    return out


def _modules(tier=None):
    import target
    if tier:
        paths = target.tiers().get(tier, [])
        return [os.path.basename(p) for p in paths
                if target.format_of(p) in ("NE",)]
    # all conducted modules
    return sorted(os.path.basename(p)[:-5]
                  for p in __import__("glob").glob("reports/conduct/*.json"))


def _ledger_path(module):
    return os.path.join(LEDGER_DIR, module.lower() + ".json")


def run_module(module, jobs):
    """Batched matching-decompilation for one module. Candidates for ALL functions
    are generated first (cheap, no DOSBox), then compiled in batches of COMPILE_BATCH
    per DOSBox boot -- with `jobs` boots running concurrently -- instead of one boot
    per candidate. The DOSBox boot dominates cost, so this is the decisive speedup."""
    from engine.foundation import parallel
    from engine.recon_engine import segdiff
    import target, cc16
    funcs = _module_funcs(module)
    if not funcs:
        return None

    # ---- phase 1: parse + generate candidates (no DOSBox) --------------------
    results = {}                      # (seg,off) -> ledger record
    cand_map = {}                     # stem -> (seg, off, order, csrc)
    func_cands = {}                   # (seg,off) -> [stem,...] in preference order
    func_far = {}                     # (seg,off) -> is far-exported prologue
    items_near, items_far = [], []    # (stem, csrc) to compile, by flag set
    n = 0
    for seg, off, size, path in funcs:
        parsed = parse_func(path)
        if not parsed:
            results[(seg, off)] = {"off": off, "seg": seg, "status": "parse_err"}
            continue
        seqp, retf_n, calls = parsed
        cands = list(candidates(seqp, retf_n, calls))
        if not cands:
            results[(seg, off)] = {"off": off, "seg": seg, "status": "residual",
                                   "shape": "+".join(_mnems(seqp)[:6])}
            continue
        far = _is_far_export(_mnems(seqp))
        func_far[(seg, off)] = far
        func_cands[(seg, off)] = []
        for i, csrc in enumerate(cands):
            stem = "C%d" % n; n += 1
            (items_far if far else items_near).append((stem, csrc))
            cand_map[stem] = (seg, off, i, csrc)
            func_cands[(seg, off)].append(stem)

    # ---- phase 2: batch-compile all candidates (few DOSBox boots) ------------
    # far-exported functions need the /Gw Windows prolog (loadds); near use the pin.
    objs = {}
    B = cc16.COMPILE_BATCH
    for group, flags in ((items_near, PIN), (items_far, PIN + " /Gw")):
        if not group:
            continue
        chunks = [group[i:i + B] for i in range(0, len(group), B)]
        parts = parallel.tmap(lambda ch: cc16.compile_many(ch, flags=flags),
                              chunks, jobs=jobs, label="cc %s" % module)
        for part in parts:
            if part:
                for stem, r in part.items():
                    objs[stem] = r.get("obj")

    # ---- phase 3: match each function against its candidates' objs -----------
    seg_bytes = {}                    # cache shipped segment bytes+mask per segment
    for seg, off, size, path in funcs:
        if (seg, off) in results:     # parse_err / no-candidate residual already set
            continue
        if seg not in seg_bytes:
            seg_bytes[seg] = segdiff.shipped_ne_segment(target.resolve(module), seg)
        sb, sm = seg_bytes[seg]
        tgt = sb[off:off + size]
        tmask = {m - off for m in sm if off <= m < off + size}
        far = func_far.get((seg, off), False)
        hit = None
        for stem in func_cands.get((seg, off), ()):
            if _match_obj(objs.get(stem), size, tgt, tmask, far=far):
                hit = cand_map[stem][3].strip(); break
        if hit is not None:
            results[(seg, off)] = {"off": off, "seg": seg, "status": "matched", "c": hit}
        else:
            seqp = parse_func(path)
            shape = "+".join(_mnems(seqp[0])[:6]) if seqp else ""
            results[(seg, off)] = {"off": off, "seg": seg, "status": "residual",
                                   "shape": shape}

    matched = [r for r in results.values() if r["status"] == "matched"]
    resid = [r for r in results.values() if r["status"] == "residual"]
    os.makedirs(LEDGER_DIR, exist_ok=True)
    json.dump({"%d:%04x" % (r["seg"], r["off"]): r for r in results.values()},
              open(_ledger_path(module), "w"))
    return {"module": module, "total": len(funcs), "matched": len(matched),
            "residual": len(resid)}


def main():
    from engine.foundation import parallel
    jobs = parallel.resolve_jobs(sys.argv, default=6)
    mods = None
    if "--module" in sys.argv:
        mods = [sys.argv[sys.argv.index("--module") + 1]]
    elif "--tier" in sys.argv:
        mods = _modules(sys.argv[sys.argv.index("--tier") + 1])
    else:
        mods = _modules()
    force = "--force" in sys.argv
    if not force:                                      # resume: skip finished modules
        todo = [m for m in mods if not os.path.exists(_ledger_path(m))]
        skipped = len(mods) - len(todo)
        if skipped:
            print("resume: %d/%d modules already done, %d to go (--force to redo)"
                  % (skipped, len(mods), len(todo)))
        mods = todo
    tm = tt = tr = 0
    print("matchbatch: %d module(s), pin %s, -j %d" % (len(mods), PIN, jobs))
    for i, m in enumerate(mods, 1):
        r = run_module(m, jobs)
        if not r:
            continue
        print("  [%d/%d] %-16s %4d/%-4d matched  (%d residual)"
              % (i, len(mods), r["module"], r["matched"], r["total"], r["residual"]),
              flush=True)
        tm += r["matched"]; tt += r["total"]; tr += r["residual"]
    # corpus totals from ALL ledgers on disk (so resumed runs report the true whole)
    gm = gt = gp = 0
    for lp in __import__("glob").glob(os.path.join(LEDGER_DIR, "*.json")):
        for rec in json.load(open(lp)).values():
            gt += 1
            if rec.get("status") == "matched":
                gm += 1
            elif rec.get("status") == "parse_err":
                gp += 1
    print("=" * 60)
    print("this run: matched %d / %d  (%d residual)" % (tm, tt, tr))
    print("CORPUS (all ledgers): AUTO-MATCHED %d / %d functions  (%.2f%%);  "
          "residual %d;  parse_err %d"
          % (gm, gt, 100.0 * gm / max(1, gt), gt - gm - gp, gp))
    print("promoted C in %s/*.json" % LEDGER_DIR)
    print("=" * 60)


if __name__ == "__main__":
    main()
