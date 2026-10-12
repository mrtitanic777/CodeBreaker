#!/usr/bin/env python3
"""oracle_teeth.py -- does the verifier reject wrong code?

Every "verified" figure rests on the verifier. This tests the verifier itself: take bodies it
passes, break each one on purpose in several small ways, rebuild, and check that it now FAILS.
A broken body that still passes is a hole in the test, not a property of the body.

    python tools/oracle_teeth.py kernel32.dll            # 40 verified bodies of one module
    python tools/oracle_teeth.py comctl32.dll 80 --show   # more bodies; print each survivor

Mutations (each applied alone, to the first place it fits):
    const      an integer constant is changed by one
    swap-args  the first two arguments of a call are exchanged
    cond       a comparison is inverted (== <-> !=, < <-> >=)
    drop-call  a call is removed (its result becomes 0)
    ret        the returned value is changed by one

Some survivors are legitimate: the mutated code is not reached by any input the verifier uses, or
the change has no observable effect. The kill rate is therefore a lower bound on the verifier's
strength; what matters is the trend and which KIND of mutation survives.
"""
import os
import re
import sys
import tempfile
from collections import Counter

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

_CALL = re.compile(r"\b([A-Za-z_]\w*)\s*\(([^()]*)\)")
_KW = {"if", "while", "for", "switch", "return", "sizeof"}


def _body_span(c):
    """(start, end) of the function body text, after the local declarations."""
    i = c.find("{")
    j = c.rfind("}")
    return (i + 1, j) if 0 <= i < j else (0, len(c))


def _statements(c):
    """Offsets of the body proper: skip the declaration block so types are not mutated."""
    s, e = _body_span(c)
    m = None
    for m in re.finditer(r"(?m)^\s*(?:unsigned|signed|int|char|short|long|DWORD|WORD|BYTE|struct)\b[^;=(]*;\s*$",
                         c[s:e]):
        pass
    return (s + m.end() if m else s), e


def m_const(c):
    s, e = _statements(c)
    for m in re.finditer(r"(?<![\w.])(0x[0-9A-Fa-f]+|\d+)(?![\w.])", c[s:e]):
        ln_start = c.rfind("\n", 0, s + m.start()) + 1
        line = c[ln_start:c.find("\n", s + m.start())]
        if line.strip().startswith(("LABEL_", "goto", "//")):
            continue
        v = int(m.group(1), 0)
        new = hex(v + 1) if m.group(1).lower().startswith("0x") else str(v + 1)
        return c[:s + m.start()] + new + c[s + m.end():]
    return None


def m_swap_args(c):
    s, e = _statements(c)
    for m in _CALL.finditer(c[s:e]):
        args = [a.strip() for a in m.group(2).split(",")]
        if m.group(1) in _KW or len(args) < 2 or args[0] == args[1]:
            continue
        args[0], args[1] = args[1], args[0]
        return c[:s + m.start(2)] + ", ".join(args) + c[s + m.end(2):]
    return None


def m_cond(c):
    s, e = _statements(c)
    m = re.search(r"\s(==|!=|<=|>=|<|>)\s", c[s:e])
    if not m:
        return None
    flip = {"==": "!=", "!=": "==", "<": ">=", ">=": "<", ">": "<=", "<=": ">"}[m.group(1)]
    return c[:s + m.start(1)] + flip + c[s + m.end(1):]


def m_drop_call(c):
    s, e = _statements(c)
    for m in re.finditer(r"(?m)^(\s*)(\w+\s*=\s*)?([A-Za-z_]\w*)\s*\([^;{}]*\);\s*$", c[s:e]):
        if m.group(3) in _KW:
            continue
        repl = m.group(1) + (m.group(2) + "0;" if m.group(2) else ";")
        return c[:s + m.start()] + repl + c[s + m.end():]
    return None


def m_ret(c):
    s, e = _statements(c)
    m = re.search(r"\breturn\s+([^;]+);", c[s:e])
    if not m:
        return None
    return c[:s + m.start(1)] + "(" + m.group(1) + ") + 1" + c[s + m.end(1):]


MUTATIONS = (("const", m_const), ("swap-args", m_swap_args), ("cond", m_cond),
             ("drop-call", m_drop_call), ("ret", m_ret))


def measure(module, limit=40, show=False, log=print):
    """Returns (Counter killed-by-kind, Counter tried-by-kind, [survivors])."""
    from src.core import corpus
    from src.formats.pe import open_pe
    from src.targets.win95 import bodyverify as B, verify95 as V
    from src.targets.win95.verification import pipeline
    path = corpus.retail(module)
    if not path:
        raise SystemExit("no retail module %r in the corpus" % module)
    pipeline._arm()
    imports = B.import_map(path)
    vecs, more = V._vectors(4, "pe"), V._confirm_vectors(4, "pe")
    work = tempfile.mkdtemp(prefix="teeth_")

    def judge(va, c, fn, tag):
        # Built the way the pipeline builds it: with the real bodies of the callees it reaches.
        src = {fn: c}
        support = pipeline.support_bodies(path, src, imports, [fn])
        dll = None
        for d, exports in B._batch_or_bisect([(g, src[g]) for g in [fn] + support],
                                             os.path.join(work, tag), imports=imports):
            if d and fn in exports:
                dll = d
        if not dll:
            return None
        pb = open_pe(dll)
        addr = pb.export_addrs().get(fn)
        if not addr:
            return None
        return V.compare(lambda v: V._fp_pe(path, va, v),
                         lambda v: V._fp_pe(dll, pb.image_base + addr, v, rebuilt=path), vecs, more,
                         insns=V.insn_count(path, va),
                         directed=lambda cov, rch: V.directed_vectors(path, va, cov, reached=rch))[0]

    killed, tried, survivors = Counter(), Counter(), []
    bodies = 0
    for n, va in enumerate(pipeline.functions("pe", path)):
        if bodies >= limit:
            break
        try:
            fn, c = B.synth(path, va, 4, fast=True)
        except Exception:
            continue
        if not fn or judge(va, c, fn, "%d_base" % n) != "equivalent":
            continue                     # only bodies the verifier passes (built alone) are probes
        bodies += 1
        for kind, mutate in MUTATIONS:
            mc = mutate(c)
            if not mc or mc == c:
                continue
            v = judge(va, mc, fn, "%d_%s" % (n, kind))
            if v is None:
                continue                 # the mutant did not build: not a test of the verifier
            tried[kind] += 1
            if v in pipeline.GOOD:
                survivors.append((fn, kind, v))
                if show:
                    log("  SURVIVED %-10s %s (%s)" % (kind, fn, v))
            else:
                killed[kind] += 1
    return killed, tried, survivors, bodies


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    module = argv[0]
    nums = [a for a in argv[1:] if a.isdigit()]
    killed, tried, survivors, bodies = measure(module, int(nums[0]) if nums else 40, "--show" in argv)
    total = sum(tried.values())
    if not total:
        print("%s: no verified bodies to mutate" % module)
        return 1
    print("%s: %d verified bodies, %d mutants built, %d rejected (%.1f%%)" % (
        module, bodies, total, sum(killed.values()), 100.0 * sum(killed.values()) / total))
    for kind, _f in MUTATIONS:
        if tried[kind]:
            print("  %-10s %4d of %4d rejected (%.0f%%)" % (kind, killed[kind], tried[kind],
                                                         100.0 * killed[kind] / tried[kind]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
