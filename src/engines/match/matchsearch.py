#!/usr/bin/env python3
r"""matchsearch.py -- the byte-exact MATCH ENGINE for the "compile our C" project.

Heuristics-first, diff-guided-search-second, oracle-verified, corpus-scalable. For every
function it mechanically does the loop we used to do by hand:

  1. HEURISTIC RECOVERY  -- read the SHIPPED disassembly and directly infer the semantic
     facts that survive /O1 (the only things that change the bytes): calling convention
     (outer + each callee + fn-ptr, from the ret / post-call `add esp,N`), and a signedness
     signal (idiv/sar/movsx/jl vs div/shr/movzx/jb). No compiles -- just read the truth.
  2. BUILD + VERIFY      -- build the candidate with those facts (raiser.candidate already
     consumes the convention), compile once on the MSVC 2.0 oracle, byte-compare (relocs
     masked).
  3. DIFF-GUIDED SEARCH  -- if it misses, hill-climb the byte distance over the AMBIGUOUS
     knobs via permutebridge (flag matrix x C-variants incl. signedness/convention flips x
     below-C asm-peephole). Bounded, and only for the residual heuristics can't pin down.

WHY IT WORKS: with the EXACT compiler (MSVC 2.0 /O1, proven the exact Win95 toolchain) codegen
is DETERMINISTIC -- the same source-equivalent C yields the same bytes every time. So the only
unknown is our C's SEMANTICS, a finite space, searched against a deterministic oracle. What it
CANNOT reach is what C can't express (register-lane aliasing, instruction scheduling) or what
never builds a candidate (alloca/SEH) -- those are lift/representation limits, not search ones.

    wt --target win95 matchsearch --limit 500          # sample, report rate + how
    wt --target win95 matchsearch --module kernel32    # one module
    wt --target win95 matchsearch --all -j 8 --save    # whole corpus, parallel, save the C
    wt --target win95 matchsearch --show MOD:seg:off    # one function, verbose
"""
import glob
from src.core import cores
import os
import re
import sys
from concurrent.futures import ThreadPoolExecutor

_ENG = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, _ENG)
sys.path.insert(0, os.path.join(os.path.dirname(_ENG), "win95"))

from src.core import layout
matchbatch = cores.get('matchbatch')
from src.engines.match import raiser
from src.ir import il
try:
    from src.engines.match import permutebridge as _pb
except Exception:
    _pb = None
try:
    import capstone
    _MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
except Exception:
    _MD = None

_SIGBODY = re.compile(r"\b\w[\w ]*\**\w+\(([^)]*)\)\s*\n\{(.*)\n\}\s*$", re.S)
# every function in a generated module .c, with its seg:off tag
_FN = re.compile(
    r"/\* \[AUTO\] \w+\s+\w+ seg(\d+):0x([0-9a-f]+)\s+\w+ \*/\n"
    r"[A-Za-z_][\w ]*\**\w+\(([^)]*)\)\n\{(.*?)\n\}", re.S)


# ---------------------------------------------------------------------------
# 1. HEURISTIC RECOVERY -- read semantic facts straight from the shipped bytes
# ---------------------------------------------------------------------------
def _conv_scan(sb):
    """Calling convention read from the DECODED function, not just the last byte.
    A function's `ret` is often NOT the final byte -- the tail may be a `jmp` (tail-call
    or loop-back), padding, or a second return path. Decode and find the actual return
    instruction(s): `ret imm16` (callee pops args) => __stdcall; plain `ret` => __cdecl.
    All returns in one function share a convention; if a `ret N` appears anywhere it is
    stdcall. Falls back to the raiser's last-byte check, then None (pure tail-jmp thunk)."""
    if _MD is not None:
        saw_ret = saw_retn = False
        for ins in _MD.disasm(bytes(sb), 0):
            if ins.mnemonic == "ret":
                if ins.op_str.strip():
                    saw_retn = True
                else:
                    saw_ret = True
            elif ins.mnemonic == "retf":       # far return -- not our 32-bit flat model
                return None
        if saw_retn:
            return "__stdcall"
        if saw_ret:
            return "__cdecl"
    return raiser._conv_from_shipped(sb)


def shipped_facts(sb):
    """Semantic facts inferred directly from the shipped disassembly (no compiles).
    conv: outer calling convention from the actual return instruction(s). signed: does the
    function use signed instructions (idiv/sar/movsx + signed jcc) -- a search hint."""
    facts = {"conv": _conv_scan(sb), "signed": False, "n_ins": 0}
    if _MD is not None:
        mn = [i.mnemonic for i in _MD.disasm(bytes(sb), 0)]
        facts["n_ins"] = len(mn)
        facts["signed"] = any(m in ("idiv", "sar", "movsx", "cdq", "jl", "jg", "jle", "jge")
                              for m in mn)
    return facts


# ---------------------------------------------------------------------------
# 2/3. per-function match: heuristic candidate -> verify -> diff-guided search
# ---------------------------------------------------------------------------
def _lift_body(ap):
    try:
        c = il.lift_asm_function(ap, "fn", faithful=True)
    except Exception:
        return None
    m = _SIGBODY.search(c)
    return (m.group(1), m.group(2)) if m else None


def match_function(cc32, mod, seg, off, deep=True, verbose=False):
    """Try to produce byte-exact recompilable C for one function. Returns a dict:
    status: match|miss|oos|no-conv|no-asm ; how: the winning lever ; c: the winning source."""
    ap = matchbatch._asm_for(mod, seg, off)
    if not ap:
        return {"status": "no-asm", "mod": mod, "seg": seg, "off": off}
    sb = matchbatch.shipped_bytes(ap)
    facts = shipped_facts(sb)
    if not sb or facts["conv"] is None:
        return {"status": "no-conv", "mod": mod, "seg": seg, "off": off}
    ab = _lift_body(ap)
    if ab is None:
        return {"status": "no-conv", "mod": mod, "seg": seg, "off": off}
    args, body = ab
    # HEURISTIC-FIRST: build the candidate with the recovered convention.
    try:
        cand = raiser.candidate(args, body, want_conv=facts["conv"])
    except Exception:
        cand = None
    if cand is None:
        return {"status": "oos", "mod": mod, "seg": seg, "off": off}
    # VERIFY: fast flag family (raiser._try_match leads with /O1, the Win95 build flag).
    fl = raiser._try_match(cc32, cand, sb)
    if fl:
        return {"status": "match", "how": "heuristic:%s" % fl, "c": cand,
                "mod": mod, "seg": seg, "off": off, "bytes": len(sb)}
    # DIFF-GUIDED SEARCH: permutebridge (flag matrix x C-variants x asm-peephole).
    if deep and _pb is not None:
        try:
            r = _pb.close(cc32, cand, sb)
        except Exception:
            r = {"closed": False}
        if r.get("closed"):
            return {"status": "match", "how": "search:%s" % r.get("tier"),
                    "c": r.get("variant") or cand, "mod": mod, "seg": seg, "off": off,
                    "bytes": len(sb), "dist": 0}
        return {"status": "miss", "mod": mod, "seg": seg, "off": off,
                "dist": r.get("dist"), "c": cand}
    return {"status": "miss", "mod": mod, "seg": seg, "off": off, "c": cand}


# ---------------------------------------------------------------------------
# corpus iteration + scalable driver
# ---------------------------------------------------------------------------
def iter_corpus(module=None, limit=None):
    """Yield (mod, seg, off) for every function in the generated PE modules (or one module).
    candidate() filters domain internally, so no pre-filter here -- the engine sees the
    whole corpus and reports what it can and can't do."""
    # module may be given bare ("kernel32") or full ("kernel32.dll"); match either.
    pat = "%s*.c" % module if module else "*.c"
    files = glob.glob(os.path.join(layout.generated_dir(), "**", pat), recursive=True)
    files = [f for f in files if re.search(r"\.(dll|exe|cpl|ocx|drv|scr)\.c$", f)]
    n = 0
    for f in sorted(files):
        mod = os.path.basename(f)[:-2]
        txt = open(f, encoding="utf-8", errors="replace").read()
        for m in _FN.finditer(txt):
            yield mod, m.group(1), m.group(2)
            n += 1
            if limit and n >= limit:
                return


def _save_c(mod, seg, off, csrc):
    d = os.path.join(layout._src_root(), "matched", mod)
    os.makedirs(d, exist_ok=True)
    open(os.path.join(d, "sec%s_%s.c" % (seg, off)), "w", encoding="utf-8").write(csrc)


def run(module=None, limit=None, jobs=1, deep=True, save=False):
    import cc32
    from collections import Counter
    targets = list(iter_corpus(module, limit))
    print("matchsearch: %d functions%s  (deep=%s, jobs=%d, save=%s)"
          % (len(targets), " in " + module if module else "", deep, jobs, save), flush=True)
    stat = Counter()
    how = Counter()
    done = [0]

    def work(t):
        mod, seg, off = t
        r = match_function(cc32, mod, seg, off, deep=deep)
        done[0] += 1
        if done[0] % 200 == 0:
            print("  ...%d/%d  match=%d" % (done[0], len(targets), stat["match"]), flush=True)
        return r

    results = []
    if jobs > 1:
        with ThreadPoolExecutor(max_workers=jobs) as ex:
            for r in ex.map(work, targets):
                results.append(r)
    else:
        results = [work(t) for t in targets]

    # HONESTY split: a match closed by asm-peephole is byte-exact only WITH a named
    # below-C equivalence (leave<->pop-ebp, ...) applied to the compiled output -- our C
    # alone does not reproduce it. Those are TEMPLATE matches, kept OUT of the headline
    # "our C compiles byte-exact" number and reported on their own annotated line.
    cmatch = tmatch = 0
    for r in results:
        stat[r["status"]] += 1
        if r["status"] == "match":
            tier = r["how"].split(":", 1)[1] if ":" in r["how"] else r["how"]
            how[tier] += 1
            if r["how"].endswith("asm-peephole"):
                tmatch += 1
            else:
                cmatch += 1
                if save:
                    _save_c(r["mod"], r["seg"], r["off"], r["c"])

    tot = len(targets)
    inscope = stat["match"] + stat["miss"]
    print("\n" + "=" * 60)
    print("MATCHSEARCH  (%d functions)" % tot)
    print("=" * 60)
    print("  BYTE-EXACT FROM OUR C:  %d  (%.1f%% of all, %.1f%% of in-scope)"
          % (cmatch, 100 * cmatch / max(1, tot), 100 * cmatch / max(1, inscope)))
    if tmatch:
        print("  + template-closed (C + named below-C asm equivalence): %d" % tmatch)
    print("  miss (built, no match): %d" % stat["miss"])
    print("  out-of-scope (no candidate): %d" % stat["oos"])
    print("  no convention / no asm: %d" % (stat["no-conv"] + stat["no-asm"]))
    print("-" * 60)
    print("  closed via:  " + "  ".join("%s=%d" % (k, v) for k, v in how.most_common()))
    if save:
        print("  saved byte-exact-from-C sources to %s/matched/" % layout._src_root())
    return stat


def main():
    a = sys.argv[1:]
    import cc32
    if "--show" in a:
        tgt = a[a.index("--show") + 1]
        mod, seg, off = tgt.split(":")
        r = match_function(cc32, mod, seg, off, deep="--deep" in a, verbose=True)
        print("status:", r["status"], " how:", r.get("how"), " dist:", r.get("dist"))
        if r.get("c"):
            print("--- candidate ---\n" + r["c"])
        return
    def opt(flag, d=None):
        return a[a.index(flag) + 1] if flag in a else d
    module = opt("--module")
    limit = int(opt("--limit")) if "--limit" in a else (None if "--all" in a else 500)
    jobs = int(opt("-j", "1"))
    # DEFAULT is the cheap heuristic + fast-flag path (corpus-scalable). --deep opts into
    # the full permutebridge matrix per miss (~288 compiles each) for targeted runs.
    run(module=module, limit=limit, jobs=jobs, deep="--deep" in a, save="--save" in a)


if __name__ == "__main__":
    main()
