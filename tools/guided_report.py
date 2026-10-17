"""How much more of a weakly verified function its inputs run, with branch-guided inputs
(verify/guided.py) added to the fixed and directed ones.

    python tools/guided_report.py [--store FILE] [--kind pe|powerpc] [--sample N] [--seed S]

Takes N functions the store judged equivalent with the limit `low-coverage` (under half of the
original's instructions ran on any input), runs the original on the inputs the verifier gave it
before (fixed, confirm, directed) and then on those plus the guided ones, and reports the share
of each function's instructions that ran, and how many functions reach half (and so would no
longer be weak on that count). Only the original is run: coverage is the original's alone."""
import argparse
import os
import random
import sqlite3
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)


def sample(store, kind, n, seed):
    db = sqlite3.connect("file:%s?mode=ro" % store.replace("\\", "/"), uri=True)
    rows = db.execute("""SELECT DISTINCT m.path, f.addr FROM verdict v
        JOIN candidate c ON c.id = v.candidate JOIN function f ON f.id = c.function
        JOIN module m ON m.sha = f.module
        WHERE m.kind = ? AND v.verdict = 'equivalent' AND v.limits LIKE '%low-coverage%'""", (kind,)).fetchall()
    rows = sorted(r for r in rows if r[0] and os.path.isfile(r[0]))
    random.Random(seed).shuffle(rows)
    return [(p, int(a, 16) if isinstance(a, str) else int(a)) for p, a in rows[:n]], len(rows)


def _takes_ecx(path, va):
    """Whether the function reads ECX before it writes it (its first argument is there: a member
    function -- the store does not record the convention, the judge has it from the decompiler)."""
    import capstone
    from codebreaker.emulate import emulator as T
    off, va0, code, end, _sec = T._resolve_func(path, va)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    for k, ins in enumerate(md.disasm(bytes(code[off:min(end, off + 96)]), va0 + off)):
        read, written = ins.regs_access()
        names_r = {ins.reg_name(r) for r in read}
        names_w = {ins.reg_name(r) for r in written}
        if names_r & {"ecx", "cx", "cl"}:
            return True
        if names_w & {"ecx", "cx", "cl"} or ins.mnemonic in ("call", "ret", "jmp") or k > 24:
            return False
    return False


def pe_coverage(path, va):
    """(instructions, ran before, ran after, inputs kept, seconds) for one PE function: the inputs
    the judge gives it (verify/win32.py judge), then those and the guided ones."""
    from codebreaker.verify import win32 as W
    insns = W.insn_count(path, va)
    if not insns:
        return None
    shape = W.in_ecx if _takes_ecx(path, va) else None

    def fp(v):
        return W._fp_pe(path, va, shape(v) if shape else v)

    def cov(vs):
        got = set()
        for v in vs:
            f = fp(v)
            if "err" not in f:
                got |= set(f.get("cov") or ())
        return got
    seeds = list(W._vectors(4, "pe")) + list(W._confirm_vectors(4, "pe"))
    before = cov(seeds)
    reach = set()
    for v in seeds:
        reach |= set(fp(v).get("reach") or ())
    directed = W.directed_vectors(path, va, before, reached=reach, shape=shape)
    before |= cov(directed)
    t = time.time()
    g = W.guided_vectors(path, va, seeds + list(directed), shape)
    after = before | cov(g)
    return insns, len(before), len(after), len(g), time.time() - t


def main(argv):
    from codebreaker.store import functions as F
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--store", default=F.default_path())
    ap.add_argument("--kind", default="pe", choices=("pe",))
    ap.add_argument("--sample", type=int, default=150)
    ap.add_argument("--seed", type=int, default=1)
    a = ap.parse_args(argv)
    funcs, total = sample(a.store, a.kind, a.sample, a.seed)
    print("%d weak (low-coverage) %s functions in the store; sampling %d" % (total, a.kind, len(funcs)))
    rows, spent = [], 0.0
    for k, (path, va) in enumerate(funcs):
        try:
            r = pe_coverage(path, va)
        except Exception as e:
            print("  %-14s %08x  error %s" % (os.path.basename(path), va, type(e).__name__))
            continue
        if not r:
            continue
        insns, b, af, kept, sec = r
        spent += sec
        rows.append(r)
        print("  %3d %-14s %08x  %4d insns  %3d%% -> %3d%%  (+%d inputs, %.1fs)" % (
            k, os.path.basename(path), va, insns, 100 * b // insns, 100 * af // insns, kept, sec))
    if not rows:
        return 1
    half_b = sum(1 for i, b, _a, _k, _s in rows if 2 * b >= i)
    half_a = sum(1 for i, _b, af, _k, _s in rows if 2 * af >= i)
    mean_b = sum(100.0 * b / i for i, b, _a, _k, _s in rows) / len(rows)
    mean_a = sum(100.0 * af / i for i, _b, af, _k, _s in rows) / len(rows)
    print("\n%d functions: mean coverage %.1f%% -> %.1f%%; at least half run: %d -> %d (%.0f%% of the "
          "sample no longer weak on coverage); guided search %.2fs a function" % (
              len(rows), mean_b, mean_a, half_b, half_a, 100.0 * (half_a - half_b) / len(rows), spent / len(rows)))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
