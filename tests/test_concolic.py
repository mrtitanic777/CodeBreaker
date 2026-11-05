#!/usr/bin/env python3
"""Tests for the path-exploring judge: the expressions (emulate/symbolic.py), the explorer
(verify/concolic.py) and the shadow of a run (emulate/shadow.py, on the decompiler's p-code).
Run:  python tests/test_concolic.py     (exit code 0 = all passed)

The explorer's tests are self-contained (a made-up function over z3); the shadow's run real
16-bit and 32-bit code in Unicorn and PowerPC code in CodeBreaker's interpreter, and are skipped
when z3 or the lifting pcodecheck (with the mode each needs) is absent."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

RESULTS = []


class Skip(Exception):
    pass


def _z3():
    try:
        import z3  # noqa: F401
    except ImportError:
        raise Skip("needs z3 (pip install z3-solver)")


def _lifter():
    _z3()
    from codebreaker.emulate import pcode
    lifter = pcode.lifter()
    if lifter is None:
        raise Skip("needs pcodecheck with its lift mode (native/decompiler, target pcodecheck)")
    return lifter


# --- the expressions ----------------------------------------------------------------------------
def test_the_folded_expressions_mean_what_they_were_made_from():
    """A part of a joined value is that part; a boolean tested for being 1 is its condition."""
    _z3()
    import z3
    from codebreaker.emulate import symbolic as S
    x, y = S.var("x", 16), S.var("y", 16)
    whole = S.concat(S.extract(x, 8, 8), S.extract(x, 0, 8))
    assert whole is x or whole == x, whole                  # (the two bytes of x, in order: x)
    j = S.concat(x, y)
    assert S.extract(j, 16, 16) is x and S.extract(j, 0, 16) is y
    c = S.compare("ult", x, y)
    assert S.nonzero(S.to_bool(c)) is c
    assert S.compare("eq", S.to_bool(c), S.const(0, 8)) == S.negate(c)
    memo = {}
    s = z3.Solver()                                          # (and z3 agrees: no x, y tell them apart)
    s.add(S.z3_of(S.extract(S.concat(S.extract(x, 4, 8), S.extract(y, 0, 8)), 8, 8), memo)
          != z3.Extract(11, 4, z3.BitVec("x", 16)))
    assert s.check() == z3.unsat
    assert S.inputs(S.binary("add", x, S.zext(S.extract(y, 0, 8), 16)), {}) == frozenset(("x", "y"))


# --- the explorer, on a made-up function ---------------------------------------------------------
def _made_up(loop=False):
    """run(v) for a function of one word x: x == 0x1234 -> path A; x < 0x100 -> B; else C (or, with
    `loop`, a loop running x times). Its conditions are expressions over x, as a shadow gives them."""
    from codebreaker.emulate import symbolic as S
    X = S.var("x", 16)

    def run(v):
        x = v["x"]
        path, trail = [], []

        def jump(site, cond, held):
            path.append((site, held, cond, "jump"))
            trail.append((site, held))
        if loop:
            i = 0
            while True:
                held = i >= x
                jump(0x10, S.compare("ule", X, S.const(i, 16)), held)
                if held or i > 200:
                    break
                i += 1
        else:
            jump(0x10, S.compare("eq", X, S.const(0x1234, 16)), x == 0x1234)
            if x != 0x1234:
                jump(0x20, S.compare("ult", X, S.const(0x100, 16)), x < 0x100)
        return {"trail": trail, "path": path, "vars": {"x": ("arg", 0, 16, x)}, "lost": set()}

    def derive(v, values, names):
        return {"x": values["x"]} if "x" in values and values["x"] != v["x"] else None
    return run, derive


def test_every_path_of_a_function_is_found_and_said_to_be_all():
    """From one input, the explorer turns each condition and asks z3: three paths, one of them
    behind a constant no input chosen without looking would hit -- and then no more are possible."""
    _z3()
    from codebreaker.verify import concolic
    run, derive = _made_up()
    got = concolic.explore(run, [{"x": 5}], derive, budget=16, keep=16)
    assert got.paths == 3, got.summary()
    assert any(v["x"] == 0x1234 for v in got.kept), got.kept
    assert got.complete, (got.summary(), got.lost, got.unknown, got.missed)
    assert got.runs <= 4, got.runs                           # (the seed and one run per new path)


def test_seeds_that_went_the_same_way_are_not_run_again():
    """Seeds whose paths are known from their runs: one per path is run with a shadow."""
    _z3()
    from codebreaker.verify import concolic
    run, derive = _made_up()
    seeds = [{"x": 5}, {"x": 6}, {"x": 7}, {"x": 0x500}]
    known = {i: run(v)["trail"] for i, v in enumerate(seeds)}
    got = concolic.explore(run, seeds, derive, budget=16, keep=16, known=known)
    assert got.paths == 3 and got.complete, got.summary()
    assert got.runs == 3, got.runs                           # (x=5 and x=0x500 for their paths, and 0x1234)


def test_a_loop_an_input_counts_is_sampled_not_all():
    """A loop as long as an input says has more paths than any budget: never 'all'."""
    _z3()
    from codebreaker.verify import concolic
    run, derive = _made_up(loop=True)
    got = concolic.explore(run, [{"x": 2}], derive, budget=6, keep=16)
    assert got.paths >= 4 and not got.complete, got.summary()
    assert got.runs == 6


# --- the shadow of a real run --------------------------------------------------------------------
CODE = bytes.fromhex(
    "a10001"        # mov ax, [0100h]          ; a word of memory the input sets
    "3d3412"        # cmp ax, 1234h
    "7505"          # jne +5
    "bb0100"        # mov bx, 1
    "eb03"          # jmp +3
    "bb0200"        # mov bx, 2
    "90" "90")      # nop, nop                 ; (the run stops before the last)


def _snippet(lifter):
    """run(v), derive for CODE run in Unicorn (real mode, everything at 0), with its shadow."""
    import unicorn as U
    from unicorn import x86_const as X
    from codebreaker.emulate import shadow

    def run(v):
        uc = U.Uc(U.UC_ARCH_X86, U.UC_MODE_16)
        uc.mem_map(0, 0x10000)
        uc.mem_write(0x1000, CODE)
        uc.mem_write(0x100, v["word"].to_bytes(2, "little"))
        uc.reg_write(X.UC_X86_REG_SP, 0x8000)
        sh = shadow.Shadow16(lifter, shadow.real_mode, [(0x100, 0x102)])
        sh.enter([])
        uc.hook_add(U.UC_HOOK_CODE, lambda u, a, _s, _d: sh.step(u, a, 0))
        uc.emu_start(0x1000, 0x1000 + len(CODE) - 1)
        v["bx"] = uc.reg_read(X.UC_X86_REG_BX)
        return {"trail": sh.trail, "path": sh.path, "vars": sh.vars, "lost": set(sh.lost)}

    def derive(v, values, names):
        word = v["word"]
        for name, value in values.items():
            kind, where, _bits, _was = names[name]
            if kind == "mem":
                word = (word & ~(0xFF << (8 * (where - 0x100)))) | (value << (8 * (where - 0x100)))
        return {"word": word} if word != v["word"] else None
    return run, derive


def test_the_shadow_gives_a_jump_s_condition_over_the_memory_it_compared():
    """mov ax, [100h] / cmp ax, 1234h / jne: the jump's condition, through the specification's
    p-code of the three instructions, is over the two input bytes -- and z3's answer takes the
    other side."""
    lifter = _lifter()
    from codebreaker.verify import concolic
    run, derive = _snippet(lifter)
    o = run({"word": 7})
    assert [k for _s, _t, _c, k in o["path"]] == ["jump"], o["path"]
    assert o["trail"] == [(0x1000 + 6, True)], o["trail"]     # (jne taken: 7 is not 1234h)
    assert set(o["vars"]) == {"m100", "m101"}, o["vars"]
    got = concolic.explore(run, [{"word": 7}], derive, budget=8, keep=8)
    assert [v["word"] for v in got.kept] == [0x1234], got.kept
    assert got.kept[0]["bx"] == 1
    assert got.paths == 2 and got.complete, got.summary()


def test_a_run_with_its_shadow_is_the_same_run():
    """The shadow only watches: LZEXPAND's functions run with it and without it end alike, make the
    same calls and the same writes, in as many steps -- also where the shadow meets a selector no
    descriptor names (LZCOPY, an unmodelled call answering 2: it once stopped the run there)."""
    lifter = _lifter()
    from codebreaker.base import corpus
    from codebreaker.emulate import shadow
    from codebreaker.emulate import windows16 as emu
    from codebreaker.formats.ne import NEFile
    path = corpus.retail("lzexpand.dll")
    if not path:
        raise Skip("needs lzexpand.dll in the corpus")
    nef = NEFile(path)
    entries = [e for e in nef.entries() if e.get("kind") != "UNUSED" and e.get("seg")][:8]
    inputs = [{"args": [0] * 8}, {"args": [1, 0x40, emu.SAFE_ARG, 2, 0x80, emu.SAFE_ARG, 3, 0x100]},
              {"args": [0] * 8, "stub_ret": 2, "mem": ((("D", 50), bytes(6)),)}]
    ds = emu.segment_base(nef.h.get("autodata_seg") or 1)

    def seen(r):
        return (r.get("exit"), r.get("insn_count"), [h.get("api") for h in r.get("imports_hit") or ()],
                [(w["addr"], w["size"], w["value"]) for w in r.get("mem_writes") or ()], r.get("ret32"))
    for e in entries:
        for v in inputs:
            runs = []
            for watched in (False, True):
                sh = shadow.Shadow16(lifter, emu.selector_base, [(ds, ds + 0x10000)]) if watched else None
                runs.append(seen(emu.probe(path, seg=e["seg"], offset=e["offset"], args=v["args"], near=False,
                                           stub_ret=v.get("stub_ret"), mem_init=v.get("mem"), ports=True, shadow=sh)))
            assert runs[0] == runs[1], ("%d:%04x" % (e["seg"], e["offset"]), v, runs[0][:3], runs[1][:3])


def test_the_lifter_gives_the_specification_s_p_code():
    """One instruction lifted: cmp ax, [bp+6] reads memory through segment(SS, ...), sets the flags."""
    lifter = _lifter()
    ins = lifter.lift(bytes.fromhex("3b4606") + bytes(13), 0x10)
    assert ins.length == 3
    ops = [o[0] for o in ins.ops]
    assert "CALLOTHER" in ops and "LOAD" in ops and "INT_SBORROW" in ops, ops
    assert any(o[0] == "CALLOTHER" and o[2][0] == "segment" for o in ins.ops)
    jcc = lifter.lift(bytes.fromhex("7405") + bytes(14), 0x20)
    assert jcc.ops[-1][0] == "CBRANCH" and jcc.ops[-1][2][0] == ("m", 0x27, 4), jcc.ops


# --- the 32-bit and PowerPC shadows (one Shadow over the platform's view) ------------------------
def _lifter_for(mode):
    _z3()
    from codebreaker.emulate import pcode
    lifter = pcode.lifter(mode)
    if lifter is None:
        raise Skip("needs pcodecheck with its %s-lift mode (native/decompiler, target pcodecheck)" % mode)
    return lifter


def _lz32():
    from codebreaker.base import corpus
    from codebreaker.formats.pe import open_pe
    path = corpus.retail("lz32.dll")
    if not path:
        raise Skip("needs lz32.dll in the corpus")
    pe = open_pe(path)
    return path, [(name, pe.image_base + rva) for _o, name, rva in pe.exports()]


def test_a_32_bit_run_with_its_shadow_is_the_same_run():
    """The 32-bit shadow only watches: LZ32's exports traced with it (one instruction at a time)
    and without it (the fast tracer) give the judge the same fingerprint."""
    _lifter_for("x86-32-flat")
    from codebreaker.verify.lanes import win32
    os.environ["CB_NO_FP_CACHE"] = "1"
    path, exports = _lz32()
    seeds = list(win32._vectors(4, "pe"))[1:] + list(win32._confirm_vectors(4, "pe"))[:3]
    from codebreaker.emulate import pcode, shadow
    from codebreaker.emulate import emulator as tracer
    lifter = pcode.lifter("x86-32-flat")
    for name, va in exports[:8]:
        for v in seeds:
            runs = []
            for watched in (False, True):
                sh = shadow.Shadow32(lifter, [(0x200000, 0x210000)]) if watched else None
                r = win32.P.emulator.run(path, func=va, args=v["args"], mem_init=v.get("mem_init"),
                                         stub_externals=True, stub_ret=int(v.get("stub_ret", 0)), reads=False,
                                         stop_wild=True, quiet_stack=True, reuse=True, fast=True, lean=True,
                                         pads=win32._PADS, shadow=sh, **win32._stub_options(path))
                runs.append((r.exit, r.steps, r.results["EAX"], [tuple(c[:2]) for c in r.calls],
                             win32._effects(r, None), r.stack, r.kept(), sorted(r.path)))
            assert runs[0] == runs[1], (name, v.get("args"), runs[0][:4], runs[1][:4])
            assert tracer._MACHINES                                  # (and the kept machines put back)


def test_the_32_bit_explorer_finds_every_path_of_lzopenfile():
    """LZOpenFileA through the x86-32-flat p-code: its paths found, said to be all, and each input
    kept takes one no seed took."""
    _lifter_for("x86-32-flat")
    from codebreaker.verify.lanes import win32
    os.environ["CB_NO_FP_CACHE"] = "1"
    path, exports = _lz32()
    va = dict(exports)["LZOpenFileA"]
    seeds = list(win32._vectors(4, "pe")) + list(win32._confirm_vectors(4, "pe"))
    kept, got = win32.explore_vectors(path, va, seeds)
    assert got.complete and got.paths >= 4 and kept, got.summary()
    trails = {tuple(win32._branch_run(path, va, v)["branches"]) for v in seeds}
    for v in kept:
        assert tuple(win32._branch_run(path, va, v)["branches"]) not in trails


# cmpwi r3, 5 / beq +12 / li r3, 1 / blr / li r3, 2 / blr -- and a call out (bl) whose result is compared
_PPC = bytes.fromhex("2C030005" "4182000C" "38600001" "4E800020" "38600002" "4E800020")
_PPC_CALL = bytes.fromhex("7C0802A6" "F8010010" "48000101" "E8010010" "7C0803A6" "2C030007" "41820008"
                          "4E800020" "38600009" "4E800020")


def _ppc_run(code, a0, stub_ret=0, watched=True, externs=None):
    from codebreaker.emulate import pcode, powerpc, shadow
    base = 0x82000000
    sh = None
    if watched:
        sh = shadow.ShadowPPC(pcode.lifter("ppc-be-64"),
                              lambda b: powerpc.ARGS_BASE <= b < powerpc.ARGS_BASE + 0x10000, span=(base, base + len(code)))
        sh.enter([("r3", a0)], bits=64)
    r = powerpc.run([(base, code)], base, [a0], span=(base, base + len(code)), stub_ret=stub_ret, shadow=sh,
                    externs=externs)
    return r, sh


def test_the_powerpc_shadow_gives_a_branch_s_condition_over_an_argument():
    """cmpwi r3, 5 / beq: the branch's condition, through ppc_64_xenon_be's p-code, is over a0 (r3),
    and z3's answer takes the other side; a call out's result is the input `ret`, and a condition
    over it is solved too. With the shadow or without, the same run."""
    _lifter_for("ppc-be-64")
    from codebreaker.verify import concolic

    def explore(code, externs=None):
        def run(v):
            r, sh = _ppc_run(code, v["a0"], v.get("ret", 0), externs=externs)
            v["got"] = r["ret"]
            return {"trail": sh.trail, "path": sh.path, "vars": sh.vars, "lost": set(sh.lost)}

        def derive(v, values, names):
            w = {"a0": v["a0"], "ret": v.get("ret", 0)}
            for name, value in values.items():
                w["a0" if names[name][0] == "arg" else "ret"] = value
            return w if (w["a0"], w["ret"]) != (v["a0"], v.get("ret", 0)) else None
        return concolic.explore(run, [{"a0": 0}], derive, budget=8, keep=8)
    got = explore(_PPC)
    assert [v["a0"] & 0xFFFFFFFF for v in got.kept] == [5] and got.kept[0]["got"] == 2, got.kept
    assert got.paths == 2 and got.complete, got.summary()
    externs = {0x82000108: "outside"}                       # (bl +0x100 from 0x82000008)
    got = explore(_PPC_CALL, externs)
    assert [v["ret"] & 0xFFFFFFFF for v in got.kept] == [7] and got.kept[0]["got"] == 9, got.kept
    for a0, ret in ((0, 0), (5, 7), (-1, 3)):
        plain, _ = _ppc_run(_PPC_CALL, a0, ret, watched=False, externs=externs)
        watched, _ = _ppc_run(_PPC_CALL, a0, ret, externs=externs)
        assert {k: plain[k] for k in ("exit", "ret", "steps", "calls", "writes", "saved")} == \
            {k: watched[k] for k in ("exit", "ret", "steps", "calls", "writes", "saved")}


def test_the_lifter_names_the_powerpc_registers():
    """ppc-be-64-lift: r0-r31, LR, CTR, cr0-cr7, XER's bits, f0-f31 and XER named; addi's p-code
    adds to r3's eight bytes, a conditional branch's target is its address."""
    lifter = _lifter_for("ppc-be-64")
    assert {"r3", "LR", "CTR", "cr0", "xer_ca", "f1", "XER"} <= set(lifter.regs), sorted(lifter.regs)[:8]
    ins = lifter.lift(bytes.fromhex("38630004"), 0x82001000)
    assert ins.length == 4 and ins.ops == (("INT_ADD", ("r", lifter.regs["r3"][0], 8),
                                            (("r", lifter.regs["r3"][0], 8), ("c", 4, 8))),), ins.ops
    bc = lifter.lift(bytes.fromhex("4182000c"), 0x82001004)
    assert bc.ops[-1][0] == "CBRANCH" and bc.ops[-1][2][0] == ("m", 0x82001010, 4), bc.ops


def main():
    tests = [(n, f) for n, f in sorted(globals().items()) if n.startswith("test_") and callable(f)]
    failed = 0
    for name, fn in tests:
        try:
            fn()
            RESULTS.append(("PASS", name))
        except Skip as e:
            RESULTS.append(("SKIP", "%s (%s)" % (name, e)))
        except Exception as e:
            import traceback
            traceback.print_exc()
            RESULTS.append(("FAIL", "%s: %s" % (name, e)))
            failed += 1
    for status, what in RESULTS:
        print("%s  %s" % (status, what))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
