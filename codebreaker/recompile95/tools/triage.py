#!/usr/bin/env python3
r"""triage.py -- classify WHY a module's clean-lifted functions FAIL the differential oracle, so the
boot-ceiling bugs get fixed BY CLASS instead of hunted one boot at a time.

Runs the memory oracle (memdifftest --clean --graph) to get the pass/fail verdict per selected
function, then STATICALLY classifies each FAILURE from its clean lift + original asm into the
dominant failure modes:

  1. unresolved-indirect   -- sub_indirect / goto* / __lifted / "not recovered" in the body
                              (a lifter gap: an unresolved call/jump target -> stub -> wrong).
  2. ebp-less-frame        -- the original has no `push ebp; mov ebp,esp` prologue (FPO); esp-relative
                              locals the lift may mis-slot.
  3. stack-cleanup         -- original `ret N` disagrees with the recovered stack-arg count (callee vs
                              caller cleanup / arg-slot off-by).
  4. cross-call-clobber    -- calls internal helpers AND has recovered register params -> a helper may
                              clobber a register that's live across the call (register threading gap).
  5. nested-pointer        -- dereferences a LOADED pointer (`*(u32*)(*(u32*)..)` / a `->fXX` used as a
                              base address). Usually a TEST limitation (flat buffer can't model the
                              pointer graph), not a real bug -- fixed by the buffer-graph oracle.
  6. unknown               -- none of the above; needs a hand look (real semantic/flag bug).

    (run from the win95 data root)  python triage.py MODULE.DLL
"""
import os, re, sys, subprocess, tempfile
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(HERE)))))
from codebreaker.foundation import cores
cores.set_target(paths.target_dir("win95"))
from codebreaker.format_loaders.pe import open_pe
from codebreaker.format_loaders.pe_seg import PeSeg
from codebreaker.format_loaders.pe_trace import PeCarve
from codebreaker.foundation import layout
from codebreaker.ir_engine import il
from codebreaker.foundation import paths
import clean_graph


def _asm(module, off):
    ap = os.path.join(layout.asm_module_dir(module), "sec1_%05x.asm" % off)
    return open(ap, encoding="utf-8", errors="replace").read() if os.path.isfile(ap) else ""


def _retN(asm):
    m = re.search(r"\bret\s+([0-9a-fx]+)h?\b", asm)
    return int(m.group(1).rstrip("h"), 16) if m else 0


def classify(module, off):
    asm = _asm(module, off)
    try:
        cc = il.lift_asm_function(os.path.join(layout.asm_module_dir(module), "sec1_%05x.asm" % off),
                                  "sub_%x" % off, clean=True, reg_inputs=None)
    except Exception as e:
        return "unknown", "lift failed: %s" % str(e)[:40]
    body = cc.split("{", 1)[-1]
    regp = clean_graph._leading_regs(cc, off)
    if "sub_indirect" in body or "goto *" in body or "__lifted" in body or "not recovered" in body:
        return "unresolved-indirect", "has sub_indirect/goto*/unlifted"
    # ebp-less prologue: no `push ebp` in the first few instructions
    head = "\n".join(asm.splitlines()[:8])
    if "push ebp" not in head and re.search(r"\[esp", asm):
        return "ebp-less-frame", "FPO: no push ebp, esp-relative"
    # nested pointer: a deref whose base is itself a deref, or a field used as a base
    if re.search(r"\*\(u32\*\)\(\s*\*\(", body) or re.search(r"->f[0-9a-f]+\s*\)?\s*\+", body) \
            or re.search(r"\(char\*\)\(\*\(u32\*\)", body):
        return "nested-pointer", "derefs a loaded pointer (buffer-graph needed)"
    # stack cleanup mismatch: ret N but recovered stack-arg count doesn't line up
    N = _retN(asm)
    stackargs = len([p for p in re.findall(r"\barg(\d+)\b", cc.split("{", 1)[0])])
    if N and stackargs and N // 4 != stackargs:
        return "stack-cleanup", "ret %d (=%d args) vs %d recovered" % (N, N // 4, stackargs)
    # cross-call clobber risk: calls a helper and has register params
    if re.search(r"\bsub_[0-9a-f]+\s*\(", body) and regp:
        return "cross-call-clobber", "calls helper(s), reg params %s" % regp
    return "unknown", "no known signal"


def main():
    module = [a for a in sys.argv[1:] if not a.startswith("-")][0]
    tmp = tempfile.mktemp(suffix=".txt")
    print("== running memory oracle on %s (this takes a minute) ==" % module)
    out = subprocess.run([sys.executable, os.path.join(HERE, "memdifftest.py"), module,
                          "--clean", "--graph", "--manifest", tmp],
                         capture_output=True, text=True).stdout
    passed = set()
    if os.path.isfile(tmp):
        for ln in open(tmp, encoding="utf-8", errors="replace"):
            p = ln.strip().split("\t")
            if len(p) == 2:
                try:
                    passed.add(int(p[1], 16))
                except Exception:
                    pass
    failed = []
    for m in re.finditer(r"XX\s+\S+:m_\w+_([0-9a-f]+)", out):
        failed.append(int(m.group(1), 16))
    sm = re.search(r"(\d+)/(\d+) functions fully identical", out)
    print(out.strip().splitlines()[-1] if out.strip() else "(no oracle output)")
    print("== TRIAGE: %d passed, %d failed ==" % (len(passed), len(failed)))
    buckets = {}
    for off in failed:
        cls, why = classify(module, off)
        buckets.setdefault(cls, []).append((off, why))
    order = ["unresolved-indirect", "stack-cleanup", "cross-call-clobber", "ebp-less-frame",
             "nested-pointer", "unknown"]
    for cls in order:
        if cls not in buckets:
            continue
        items = buckets[cls]
        print("\n[%s]  %d fns" % (cls, len(items)))
        for off, why in items[:12]:
            print("   sub_%05x  %s" % (off, why))
        if len(items) > 12:
            print("   ... +%d more" % (len(items) - 12))
    print("\n== SUMMARY: %s ==" % ", ".join("%s=%d" % (c, len(buckets[c])) for c in order if c in buckets))


if __name__ == "__main__":
    main()
