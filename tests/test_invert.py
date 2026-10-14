#!/usr/bin/env python3
"""Tests for the compiler models: reading a function back into the source its compiler was given.
Run:  python tests/test_invert.py     (exit code 0 = all passed)

The models read machine code without help; these checks need no compiler. The ones that put a
model's answer to the real compiler are SKIPPED where that compiler is not set up."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.base import corpus                                                   # noqa: E402
from codebreaker.invert import (compilers, early_c, early_c_frame, flow16, known_differences, msc6,    # noqa: E402
                                msc6_registers, msvc2, run)

RESULTS = []
# int far pascal F(p0) { register v0 = G(p0); if (!v0) return 0; H(v0); return v0; }   as Microsoft C 6 compiles it
CALLS = bytes.fromhex("1e589045558bec1e8ed883ec0256ff76069a000000008bf00bf675042bc0eb08569a000000008bc65e"
                      "4d4d8be51f5d4dca020090")
ADD = bytes.fromhex("558bec8b46040346065dc3")              # f(a, b) { return a + b; }   as the C compiler of 1983-84 does


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def test_early_c_reads_a_function():
    for recover in (early_c.recover, early_c.recover_cfg):   # the line-by-line reader and the one that knows loops
        size, params, statements, kind, local = recover(ADD)
        assert (size, params, statements, kind) == (0, ["a", "b"], ["return (a + b);"], "int")
        text = early_c.to_c("f", size, params, statements, kind, local)
        assert "f(a, b)" in text and "int a, b;" in text and "return (a + b);" in text
    assert early_c.build_cxmap(b"", []) == {}                # no library given: no helper is named


def test_early_c_frame_model():
    assert early_c_frame.layout([], [("int", "a", 1), ("int", "b", 1)])[1] == 4       # two ints: four bytes of frame
    offsets = {"a": -2, "b": -4}
    assert early_c_frame.apply_copy_swaps(offsets, []) == {"a": "a", "b": "b"}       # no copy: each stays where it is
    assert early_c_frame.apply_copy_swaps(offsets, [("a", "b")]) == {"a": "b", "b": "a"}


def test_flow_of_a_straight_function():
    import capstone
    ins = list(capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16).disasm(ADD, 0))
    blocks, _index, entry = flow16.build_cfg(ins, 0, len(ADD))
    assert len(blocks) == 1 and flow16.structure(blocks, entry) is not None


def test_msc6_reads_calls_and_a_branch():
    text, _notes = msc6.invert(CALLS, imports={0: "G", 1: "H"})
    assert "v0 = G(p0);" in text and "if (v0 == 0)" in text and "H(v0);" in text and "return v0;" in text


def test_msc6_declines_what_it_does_not_cover():
    text, why = msc6.invert(b"\x90" * 12)
    assert text is None and why                                                # a reason, never a guess
    assert msc6_registers.invert_loop(b"\x90" * 12)[0] is None


def test_msvc2_reads_constants():
    assert msvc2.invert(bytes.fromhex("33c0c3"))["c"] == "int __stdcall f(void) { return 0; }"
    assert msvc2.invert(bytes.fromhex("b834120000c3"))["c"] == "int __stdcall f(void) { return 4660; }"


def test_known_differences_table():
    assert known_differences.KB and all(p["cause"] and p["fix"] for p in known_differences.KB)
    assert known_differences.diagnose("nothing the table knows") in (None, [])


def test_every_model_has_a_command():
    import importlib
    for name, module in run.TOOLS.items():
        assert importlib.import_module(module), name
    assert set(run.CHECKS) <= set(run.TOOLS)


def test_against_the_real_compiler():
    if not compilers.available():
        raise Skip("Microsoft C 6 is not set up")
    built = compilers.mc6_compile(msc6.invert(CALLS, imports={0: "G", 1: "H"})[0], "/AM /G2 /Gsw /Ox")
    assert len(built) == len(CALLS)                          # the model's text gives a function of the same size back


def test_a_module():
    if not compilers.available() or not corpus.retail("lzexpand.dll"):
        raise Skip("Microsoft C 6 or lzexpand.dll is not here")
    results = run.module(corpus.retail("lzexpand.dll"))
    assert sum(v[0] == "identical" for v in results.values()) >= 3
    assert all(v[1] for v in results.values())               # a text, or the reason there is none


def test_object_file_functions_and_the_early_compiler():
    from codebreaker.invert import early_c_check, omf
    try:
        obj = compilers.mc_compile("f(a, b)\nint a, b;\n{\n    return (a + b);\n}\n")
    except RuntimeError as e:
        raise Skip(str(e))
    functions = omf.functions(obj)
    assert len(functions) == 1
    code, fixed = next(iter(functions.values()))
    assert all(0 <= i < len(code) for i in fixed)
    text, same, _note = early_c_check.reconstruct(code, 0, len(code))
    assert same is True and "return (a + b);" in text
    assert early_c_check.external_branches(bytes.fromhex("e81000c3"))[1:3] == b"\1\1"   # a call out of the function


def test_dos_tools():
    from codebreaker.build import dos_tools
    if not dos_tools.emulator():
        raise Skip("DOSBox-X is not set up")
    ok, made, _work = dos_tools.run({}, ["echo hi > OUT.TXT"], ["OUT.TXT"])
    assert ok and made["OUT.TXT"].strip() == b"hi"
    if not dos_tools.masm_root():
        raise Skip("no MASM set up")
    r = dos_tools.masm("CODE SEGMENT\nASSUME CS:CODE\nstart: mov ax, 1\n ret\nCODE ENDS\nEND\n")
    assert r.get("obj", b"")[:1] == b"\x80"                                   # an object file


def test_routing_thunks_and_grammars():
    from codebreaker.invert import grammars, segment_thunk
    assert run.compiler_of(CALLS) == "msc6"
    assert run.compiler_of(bytes.fromhex("8cd89045558bec1e8ed8") + CALLS[10:]) == "msvc152"
    assert segment_thunk.verify(CALLS) is None                          # a real function is not a thunk
    texts = [t for g in grammars.GRAMMARS for t in g({"nargs": 2})]
    assert texts and all("F(" in t for t in texts)


def test_16bit_assembly_source():
    """KRNL386's code at 0x4a4..0x4fa as cMacros source that MASM assembles back to the same bytes."""
    import os
    from codebreaker.build import dos_tools
    from codebreaker.formats import ne
    from codebreaker.source.ne_asm import idiom, segment
    krnl = corpus.retail("krnl386.exe")
    if not krnl or not dos_tools.emulator():
        raise Skip("no krnl386.exe or no DOSBox-X")
    nef = ne.NEFile(krnl)
    seg = next(segment.Seg(nef, i, {}) for i, s in enumerate(nef.segments(), 1) if i == 1 and s.get("kind") == "CODE")
    r = idiom.verify_range(seg, 0x4a4, 0x4fa)
    assert r["ok"] and r["live"] == r["n_code"] and ("cBegin" in r["asm"] or "cProc" in r["asm"]), {k: v for k, v in r.items() if k != "asm"}
    assert os.path.basename(krnl).lower() == "krnl386.exe"


def test_debug_build_names():
    import os
    from codebreaker.source.ne_asm import segment
    from codebreaker.symbols import debug_build
    krnl, dbg = corpus.retail("krnl386.exe"), corpus.reference("WIN32SDK", "SCT", "DEBUG16", "I386")
    if not krnl or not os.path.isfile(os.path.join(dbg, "KRNL386.MAP")):
        raise Skip("no debug build of KRNL386 here")
    names, (agree, disagree) = debug_build.recover(krnl, os.path.join(dbg, "KRNL386.EXE"),
                                                   os.path.join(dbg, "KRNL386.MAP"), segment.load_ordinals("-"), None)
    assert disagree == 0 and agree >= 5 and len(names) >= 40


def main():
    for fn in (test_early_c_reads_a_function, test_early_c_frame_model, test_flow_of_a_straight_function,
               test_msc6_reads_calls_and_a_branch, test_msc6_declines_what_it_does_not_cover, test_msvc2_reads_constants,
               test_known_differences_table, test_every_model_has_a_command, test_against_the_real_compiler,
               test_a_module, test_object_file_functions_and_the_early_compiler, test_dos_tools,
               test_routing_thunks_and_grammars, test_16bit_assembly_source, test_debug_build_names):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "ok"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
