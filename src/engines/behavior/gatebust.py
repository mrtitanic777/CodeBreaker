#!/usr/bin/env python3
r"""gatebust.py -- get a bare-oracle fuzz past a handle-validation gate (dynamic cluster).

Most Win32 dispatchers validate a handle/pointer at the front door and bail before the
interesting multi-arm body when the check fails. The bare Unicorn oracle has no live
Win95 heaps to hold a valid handle, so arg-fuzzing stalls at the gate. Two doctrine-
clean ways past it, on the bare oracle (no VM, no live OS):

  STRATEGY 4 -- FUZZ THE INTERNAL WORKER. The export is a thin gate + thunk over an
  internal function that assumes validation already happened and takes the (validated)
  pointer/index as a plain argument. Calling that internal worker DIRECTLY means WE set
  up its entry state, so there is no front-door gate -- and a scratch-buffer pointer is
  a FAITHFUL input (the worker doesn't re-validate, it just uses the pointer). `workers`
  finds those targets from a trace of the export (exactly the edges exectree/fracture
  resolve).

  STRATEGY 2 -- BUST THE GATE. Patch the handle-check's conditional jump in the emulated
  code page (mem_init writes after the code is mapped, so no tracer change is needed).
  The patch is a 1-BYTE CONDITION INVERSION (74<->75, 0F84<->0F85, ...): length- and
  offset-preserving, so it only flips which way the gate goes. `find_gates` greedily
  inverts candidate gate jccs and keeps those that UNLOCK coverage (a 1-branch concolic
  step). A nonzero `stub_ret` additionally makes an external validation call report
  'valid', clearing API-gate checks.

Honesty: busting a gate or feeding a synthetic pointer is a DISCOVERY aid -- it reaches
code the natural inputs can't. What it observes downstream (a deref of a non-real
handle) is not guaranteed faithful; strategy 4 avoids that (the worker's pointer arg is
genuinely how it's called). Both record their traces raw like any observation.

    wt behavior gatebust <module> <func> [--budget N] [--seed S] [--insns N]
"""
import sys

from src.engines.behavior import tracer, fuzz
from src.cli.common import opt as _opt

# A nonzero stub return that also works as a pointer into mapped scratch, so a validation
# call that returns a handle/pointer yields something derefs land inside (not a fault).
_VALID = 0x204800


def _jcc_invert(codebytes):
    """(patch_offset, patch_byte) that inverts the conditional jump at the start of
    `codebytes`, or None if it isn't a jcc. 1-byte, length/offset-preserving."""
    b0 = codebytes[0]
    if b0 == 0x0F and len(codebytes) > 1 and 0x80 <= codebytes[1] <= 0x8F:
        return (1, codebytes[1] ^ 1)                      # 0F 8x  (near jcc)
    if 0x70 <= b0 <= 0x7F:
        return (0, b0 ^ 1)                                # 7x     (short jcc)
    return None


def _codebyte_reader(module):
    """Return (read(va,n), func range helper) reading the module's mapped code bytes."""
    from src.core import cores
    from src.formats.pe import open_pe
    pe = open_pe(cores.get("target").resolve(module))
    va0, code, _sec = pe.code_sections()[0]
    def read(va, n):
        off = va - va0
        return bytes(code[off:off + n]) if 0 <= off < len(code) else b""
    return read, va0


def _cov(rec):
    return len(fuzz._coverage(rec))


def workers(module, export, insn_cap=4000):
    """Strategy 4: internal worker FUNCTIONS to fuzz directly instead of the gated export
    -- the resolved intra-module call/forward targets from a trace of the export, kept
    only when they are real carved function STARTS (a jump-table arm landing mid-function
    is not a standalone entry to fuzz)."""
    starts = {va for _off, va, _sz in tracer.list_functions(module)}
    rec = tracer.trace_function(module, func=export, insn_cap=insn_cap,
                                stub_externals=True, stub_ret=_VALID)
    lo, hi = rec["func_va"], rec["func_end_va"]
    out = []
    for c in rec.get("calls", []):
        rt = c.get("resolved_target")
        if rt is not None and not c.get("external") and not (lo <= rt < hi):
            out.append(rt)                                # an internal callee outside this func
        elif c.get("target") is not None and not c.get("into_func"):
            out.append(c["target"])
    seen, uniq = set(), []
    for t in out:
        if t in starts and t not in seen:                 # only real function entries
            seen.add(t); uniq.append(t)
    return uniq


def find_gates(module, func, insn_cap=4000, max_gates=4):
    """Strategy 2: greedily invert gate jccs to unlock coverage. Returns (patches, cov0,
    cov1): the 1-byte inversion patches kept, and coverage before/after."""
    read, _va0 = _codebyte_reader(module)
    base = tracer.trace_function(module, func=func, insn_cap=insn_cap,
                                 stub_externals=True, stub_ret=_VALID)
    cov0 = fuzz._coverage(base)
    # candidate gate sites: executed conditional jumps on the (short) baseline path
    cands = []
    mn = base.get("mnemonics", {})
    for va in dict.fromkeys(base.get("path", [])):        # unique, order-preserving
        m = mn.get("0x%x" % va, "")
        if m[:1] == "j" and not m.startswith("jmp"):
            cb = read(va, 2)
            inv = _jcc_invert(cb)
            if inv:
                cands.append((va, inv))
    patches, cov = [], set(cov0)
    for _ in range(max_gates):
        best = None
        for va, (poff, pbyte) in cands:
            if any(p[0] == va + poff for p in patches):
                continue
            trial = patches + [(va + poff, bytes([pbyte]))]
            try:
                rec = tracer.trace_function(module, func=func, insn_cap=insn_cap,
                                            mem_init=trial, stub_externals=True, stub_ret=_VALID)
            except Exception:
                continue
            c = fuzz._coverage(rec)
            gain = len(c - cov)
            if gain > 0 and (best is None or gain > best[0]):
                best = (gain, va, (va + poff, bytes([pbyte])), c)
        if best is None:
            break
        patches.append(best[2]); cov |= best[3]
    return patches, cov0, cov


def gatebust(module, func, budget=400, seed=0, insn_cap=4000):
    """Run both strategies and fuzz past the gate. Returns a combined report."""
    # strategy 2: bust the export's own gates, then coverage-fuzz through them
    patches, cov0, cov1 = find_gates(module, func, insn_cap=insn_cap)
    gated = fuzz.fuzz(module, func=func, budget=budget, seed=seed, insn_cap=insn_cap,
                      patches=patches, stub_externals=True, stub_ret=_VALID)
    # strategy 4: fuzz each internal worker directly
    wk = workers(module, func, insn_cap=insn_cap)
    worker_reports = []
    for w in wk[:6]:
        try:
            s = fuzz.fuzz(module, func="0x%x" % w, budget=max(120, budget // 2),
                          seed=seed, insn_cap=insn_cap, stub_externals=True, stub_ret=_VALID)
        except Exception:
            continue
        worker_reports.append({"worker": w, "blocks": s["blocks"],
                               "indirect": s["indirect_targets"], "corpus": len(s["corpus"])})
    return {
        "module": module, "func_va": gated["func_va"],
        "gate_patches": [{"va": p[0], "byte": p[1].hex()} for p in patches],
        "cov_before_gate": len(cov0), "cov_after_gate": len(cov1),
        "gated_fuzz_blocks": gated["blocks"], "gated_indirect": gated["indirect_targets"],
        "workers": worker_reports,
    }


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help") or len(argv) < 2:
        print(main.__doc__); return
    module, func = argv[0], next(a for a in argv[1:] if not a.startswith("-"))
    budget = int(_opt(argv, "--budget", 400))
    seed = int(_opt(argv, "--seed", 0))
    insns = int(_opt(argv, "--insns", 4000))
    r = gatebust(module, func, budget=budget, seed=seed, insn_cap=insns)
    print("gatebust %s  func VA 0x%x" % (r["module"], r["func_va"]))
    print("  [strategy 2] gate patches: %d  ->  coverage %d -> %d (+%d unlocked)"
          % (len(r["gate_patches"]), r["cov_before_gate"], r["cov_after_gate"],
             r["cov_after_gate"] - r["cov_before_gate"]))
    for p in r["gate_patches"]:
        print("      invert jcc @ 0x%x -> %s" % (p["va"], p["byte"]))
    print("  [strategy 2] fuzz through the gate: %d blocks, %d indirect targets"
          % (r["gated_fuzz_blocks"], r["gated_indirect"]))
    print("  [strategy 4] internal workers fuzzed directly: %d" % len(r["workers"]))
    for w in r["workers"]:
        print("      worker 0x%-8x -> %d blocks, %d indirect, %d corpus inputs"
              % (w["worker"], w["blocks"], w["indirect"], w["corpus"]))


if __name__ == "__main__":
    main()
