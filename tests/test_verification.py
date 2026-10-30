#!/usr/bin/env python3
"""Tests for the verification pipeline and its results database.
Run:  python tests/test_verification.py     (exit code 0 = all passed)

The database tests are self-contained. The pipeline tests run real verification on two small
retail modules and are skipped when the corpus, OpenWatcom or the decompilers are absent."""
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.base import corpus
from codebreaker.build import watcom as toolchain                      # noqa: E402
from codebreaker.verify.lanes import win32 as pipeline

RESULTS = []


class Skip(Exception):
    pass


def _need(lane):
    from codebreaker.decompile import decompiler
    if not (corpus.retail("version.dll") and decompiler.available() and toolchain.ow_tool("wlink")):
        raise Skip("needs the retail corpus, OpenWatcom and the decompilers")


def test_reconstruct_is_incremental():
    """Results land in the function store, and a second run judges nothing again."""
    _need("pe")
    from codebreaker.store import functions as F
    from codebreaker.verify import reconstruct
    fs = F.open(os.path.join(tempfile.mkdtemp(), "functions.sqlite"))
    p = corpus.retail("version.dll")
    log = []
    sha, digest = reconstruct.run(p, fs=fs, log=log.append)
    first = fs.candidates(sha, digest)
    t = fs.tally(digest, sha)
    assert t["verified"] >= 8 and t["verified"] + t["weak"] >= 24, t     # 8 and 18 today
    assert sum(t.values()) == len(pipeline.functions("pe", p)), t
    assert all(r["reason"] for r in first if r["verdict"] == "divergent")
    assert all(r["source"] for r in first if r["verdict"] != "no-synth")
    log[:] = []
    reconstruct.run(p, fs=fs, log=log.append)
    assert " 0 judged now" in log[0], log
    assert fs.tally(digest, sha) == t
    assert all(r["class"] in F.ORDER for r in fs.best(sha, digest).values())


def test_function_store():
    from codebreaker.store import functions as F
    fs = F.open(os.path.join(tempfile.mkdtemp(), "functions.sqlite"))
    fs.add_module("m1", "A.DLL", "x", "pe")
    fs.add_functions("m1", ["10", "20", "30"])
    fs.record("m1", "10", "core", "1", "s1", "f10", "int f10(){}", "p1", "equivalent", limits="coverage=90")
    fs.record("m1", "10", "native", "1", "s2", "f10", "int f10(void){}", "p1", "divergent", reason="ret-differs")
    fs.record("m1", "20", "native", "1", "s3", "f20", "int f20(void){}", "p1", "equivalent", limits="low-coverage,coverage=10")
    best = fs.best("m1", "p1", prefer=("core", "native"))
    assert best["10"]["engine"] == "core" and best["10"]["class"] == "verified"
    assert best["20"]["class"] == "weak" and best["20"]["weak"] == ["low-coverage"]
    assert fs.tally("p1", "m1") == {"verified": 1, "weak": 1, "unverified": 0, "not-judged": 0,
                                    "no-compile": 0, "no-source": 1}
    # another verifier profile sees nothing judged yet, and the old verdicts are still there
    assert fs.tally("p2", "m1")["no-source"] == 3
    fs.record("m1", "10", "core", "1", "s1", "f10", "int f10(){}", "p2", "divergent", reason="writes-differ")
    assert fs.best("m1", "p1")["10"]["class"] == "verified" and fs.best("m1", "p2")["10"]["class"] == "unverified"
    # a changed source invalidates what was judged for the old one
    fs.record("m1", "10", "core", "2", "s9", "f10", "int f10(){return 1;}", "p2", "equivalent")
    assert "10" not in fs.judged("m1", "p1", "core")
    assert [n for _s, n in fs.modules("p1")] == ["a.dll"]


def test_variant_store():
    """The repair search's many variants per function, kept apart from the behavioural tables."""
    from codebreaker.store import functions as F
    fs = F.open(os.path.join(tempfile.mkdtemp(), "functions.sqlite"))
    fs.add_module("m1", "A.DLL", "x", "pe")
    fs.add_functions("m1", ["10", "20"])
    fs.record_variant("m1", "10", "msvc", "int f(){return 0;}", 14, 46, "different", knobs="", behaves="equivalent")
    fs.record_variant("m1", "10", "msvc", "int f(){int x;return 0;}", 2, 46, "same size", knobs="pad",
                      parent=1, behaves="equivalent")
    fs.record_variant("m1", "10", "msvc", "int f(){return 1;}", 0, 46, "identical", knobs="bad", behaves="divergent")
    fs.record_variant("m1", "20", "msvc", "int g(){}", 0, 8, "identical", knobs="", behaves="equivalent")
    best = fs.best_variant("m1", "msvc")
    # a divergent variant is never chosen even when its bytes are closer
    assert best["10"]["knobs"] == "pad" and best["10"]["distance"] == 2
    assert best["20"]["verdict"] == "identical"
    assert fs.match_tally("m1", "msvc") == (2, 1, 1, 0)   # 2 functions, 1 identical, 1 same size, 0 different


def test_repair_knobs_preserve_meaning():
    """The repair knobs are behaviour-preserving source rewrites, and the distance measure ranks
    a right-length near-match ahead of a wrong-length one and ignores linker-filled bytes."""
    from codebreaker.verify.authored import repair, match
    src = "int f(int a, int b){\n  int x;\n  int y;\n  return a + b;\n}"
    knobs = dict(repair._knobs(src, 16, 4))
    assert any(k.startswith("pad") for k in knobs)       # the original reserves 16, the draft 4
    assert any(k.startswith("order") for k in knobs)     # two locals -> a reordering
    assert "b + a" in next(v for k, v in knobs.items() if k.startswith("swap"))
    assert "char codebreaker_unused[12]" in knobs[next(k for k in knobs if k.startswith("pad"))]
    assert match.distance((b"\x55\x8b\xec\xc3", bytes(4)), (b"\x55\x8b\xec\xc3", bytes(4)))[:2] == ("identical", 0)
    assert match.distance((b"\x55\x8b\xec\x90", bytes(4)), (b"\x55\x8b\xec\xc3", bytes(4)))[:2] == ("same size", 1)
    masked = b"\x00\x01\x01\x01\x01"                      # a 4-byte call displacement, ignored
    assert match.distance((b"\xe8\x11\x22\x33\x44", masked), (b"\xe8\x99\x88\x77\x66", masked))[1] == 0


def test_repair_search_with_any_compiler():
    """The search works with whatever compiles: here a stand-in whose `bytes` are the order of
    the declarations, so only the authored order is at distance 0."""
    from codebreaker.verify.authored import repair
    authored = "f(a)\nint a;\n{\n\tint x;\n\tregister int y;\n\treturn a;\n}"
    fake = lambda text: (" ".join(text.split()).encode(), bytes(len(" ".join(text.split()))))
    moved = authored.replace("\tint x;\n\tregister int y;", "\tregister int y;\n\tint x;")
    assert moved != authored
    r = repair.repair(fake, moved, fake(authored))
    assert r["distance"] == 0 and r["knobs"].startswith("order"), r
    kinds = set(k.rstrip("0123456789") for k, _s in repair._knobs(authored, 0, None))
    assert kinds == {"order", "register", "regparam"}, kinds


def test_period_compiler_gives_the_original_back():
    """A source file of the reconstructed 1984 linker, compiled by the XENIX C compiler through
    build/period.py, is the linker's own bytes."""
    from codebreaker.build import period
    from codebreaker.verify.authored import match
    if "xenix-c" not in period.names():
        raise Skip("needs the XENIX C compiler (settings: xenix_c)")
    p = period.get("xenix-c")
    units = p.units()
    assert len(units) >= 20, len(units)
    u = next(x for x in units if x.name == "main.c")
    assert match.distance(p.compile(u.source, tempfile.mkdtemp()), u.original)[:2] == ("identical", 0)
    other = next(x for x in units if x.name != "main.c")          # another file's code is not main's
    assert match.distance(p.compile(other.source, tempfile.mkdtemp()), u.original)[0] != "identical"


def test_a_verdict_holds_until_what_it_was_for_changes():
    """A function's signature covers its own text and everything it can reach, and nothing else:
    changing one function re-judges it and its callers, and leaves the rest of the module alone."""
    from codebreaker.verify import reconstruct

    class M(object):
        header = "typedef int x;"
        order = ["a", "b", "c", "d", "e"]

    def sigs(**changed):
        m = M()
        m.texts = dict({"a": "int a(){ return b(); }", "b": "int b(){ return 1; }", "c": "int c(){ return 2; }",
                        "d": "int d(){ return e(); }", "e": "int e(){ return d() + c(); }"}, **changed)
        return reconstruct.signatures(m, "whole")
    was = sigs()
    now = sigs(b="int b(){ return 3; }")
    assert now["b"] != was["b"] and now["a"] != was["a"]                 # b, and a which calls it
    assert all(now[n] == was[n] for n in "cde"), (was, now)
    assert was["d"] == was["e"]                                          # a ring is judged as one
    now = sigs(c="int c(){ return 4; }")
    assert all(now[n] != was[n] for n in "cde") and all(now[n] == was[n] for n in "ab")
    m = M()
    m.texts = {}
    assert reconstruct.signatures(m, "whole") == dict.fromkeys(M.order, "whole")


def test_period_source_gives_the_declarations_an_author_had():
    """For a period compiler an import is called through its slot, a global is a variable, an
    address the module's relocations name is the address of a variable, and a function that
    only hands back what happened to be in EAX returns nothing."""
    from codebreaker.verify.authored import match, repair
    header = "\n".join(["typedef int int4;", "int __stdcall HeapFree();", "#define iRam00401000 (*(int4 *)0x401000)",
                        "#define iRam00900000 (*(int4 *)0x900000)", "int4 f(int4 a1);", "int4 __cdecl g(void);"])
    texts = {"f": "int4 f(int4 a1)\n{\n  g();\n  return HeapFree(iRam00401000,0,a1 -0x400eff90) + 0x402000 + 0x12345;\n}",
             "g": "int4 __cdecl g(void)\n{\n  int4 in_EAX;\n  \n  return in_EAX;\n}"}
    h, t = match.period_source(header, texts, {"HeapFree"}, {0x401000, 0x402000, 0xbff10070})
    assert "__declspec(dllimport) int __stdcall HeapFree();" in h
    assert "extern int4 iRam00401000;" in h and "#define iRam00900000" in h      # only what is relocated
    assert "a1 + (int4)&codebreaker_at_bff10070) + (int4)&codebreaker_at_00402000 + 0x12345" in t["f"], t["f"]
    assert "extern char codebreaker_at_00402000;" in h
    assert t["g"].startswith("void __cdecl g(void)") and "in_EAX" not in t["g"] and "void __cdecl g(void);" in h
    near = match.needed_header(h, t["g"])
    assert "HeapFree" not in near and "typedef int int4;" in near and "g(void);" in near
    src = "int f(int a,int b){\n  if (a < *(int *)(b + 4)) {\n    a = 1;\n  }\n  while (g(a) != 0) a++;\n  if (a & 1 == b) a = 2;\n  return a;\n}"
    tests = [v for k, v in repair._knobs(src, 0, None) if k.startswith("test")]
    assert len(tests) == 1 and "if (*(int *)(b + 4) > a)" in tests[0]            # not a call, not `&`
    framed = "int f(void)\n{\n  struct { int a; uint1 _rest[8]; } _f;\n  return 0;\n}"
    assert "uint1 _rest[4]; } _f;" in dict(repair._knobs(framed, 8, 12))["frame4"]


def test_breaks_change_what_a_function_does():
    """The judge's self-test breaks a function one small way at a time, in its statements only."""
    from codebreaker.verify import teeth
    src = ("int4 f(int4 a1,int4 a2)\n{\n  int4 x[4];\n  int4 y;\n\n  y = a1 + a2;\n  if (y == 3) {\n"
           "    *(int4 *)a1 = g(a1,a2);\n  }\n  return y;\n}")
    made = dict(teeth.breaks(src))
    assert set(made) == {"constant", "comparison", "operator", "store", "arguments"}, set(made)
    assert "int4 x[4];" in made["constant"] and "y == 4" in made["constant"]      # not the declaration
    assert "y != 3" in made["comparison"] and "a1 - a2" in made["operator"]
    assert "g(a2,a1)" in made["arguments"] and "*(int4 *)a1 =" not in made["store"]
    assert all(t.startswith("int4 f(int4 a1,int4 a2)") for t in made.values())


def test_other_shapes_of_the_same_function():
    """An if-chain on one value is offered as a switch, and `r = x;` before the final `return r;`
    as `return x;` -- but not through a variable narrower than what the function returns, nor
    where a branch holds a `break`."""
    from codebreaker.verify.authored import repair
    src = "\n".join([
        "int4 f(int4 a1,int4 a2)", "", "{", "  int4 iVar1;", "  ", "  iVar1 = g(a1);", "  if (iVar1 == 0) {",
        "    iVar1 = 0;", "  }", "  else {", "    if (a2 == 0) {", "      h(a1);", "    }", "    else if (a2 == 1) {",
        "      k(a1);", "    }", "    else {", "      a1 = 3;", "    }", "    iVar1 = a1;", "  }", "  return iVar1;", "}"])
    knobs = dict(repair._knobs(src, 0, None))
    assert "switch (a2) {\ncase 0:\n{\n      h(a1);\n    }\nbreak;\ncase 1:" in knobs["switch0"], knobs["switch0"]
    assert "default:\n{\n      a1 = 3;\n    }\n}" in knobs["switch0"]
    assert "    return 0;\n" in knobs["return"] and "    return a1;\n" in knobs["return"], knobs["return"]
    assert "iVar1 = g(a1);" in knobs["return"]                       # not the last thing done: kept
    narrow = src.replace("  int4 iVar1;", "  uint1 iVar1;")
    assert "return" not in dict(repair._knobs(narrow, 0, None))
    looped = src.replace("      k(a1);", "      break;")
    assert not any(k.startswith("switch") for k, _v in repair._knobs(looped, 0, None))


def test_object_file_of_the_16_bit_compiler_is_read():
    """A function's bytes come out of an OMF object file with the linker's places marked, and
    the reconstruction's dialect is said the way Microsoft C 6 takes it."""
    from codebreaker.build import msc6

    def record(kind, body):
        return bytes([kind]) + (len(body) + 1).to_bytes(2, "little") + body + b"\0"
    obj = (record(0x80, b"\x03f.c") + record(0xA0, b"\x01\x00\x00" + bytes([0x55, 0x9A, 0, 0, 0, 0, 0x5D, 0xCB]))
           + record(0x9C, bytes([0xCC, 0x01, 0x26, 0x01, 0x01]))            # a far pointer at +1, to an external
           + record(0xA0, b"\x02\x00\x00" + b"data") + record(0x8A, b"\x00"))
    code, mask = msc6.object_code(obj)
    assert code == bytes([0x55, 0x9A, 0, 0, 0, 0, 0x5D, 0xCB]) and mask == bytes([0, 1, 1, 1, 1, 0, 0, 0]), (code, mask)
    header, _text = msc6.dialect("typedef unsigned long long uint8;\nuint2 codebreaker_ss(void);\n"
                                 '#pragma aux codebreaker_ss = "mov ax,ss" value [ax] modify exact [ax];\n'
                                 "#define __stdcall16far __far __stdcall\n", "")
    assert "#define __far _far" in header and "pragma aux" not in header and "long long" not in header
    assert "#define __stdcall16far __far __stdcall" in header
    header, _text = msc6.dialect("extern char __based(__segname(\"S\")) s[];\n", "")
    assert "#define __based _based" in header and "#define __segname _segname" in header


def test_the_16_bit_compiler_s_function_is_cut_out_by_its_name():
    """A header's own functions come first in the code segment: the function judged is found by
    its public name, from there to the next public, with its fix-ups marked."""
    from codebreaker.build import msc6

    def record(kind, body):
        return bytes([kind]) + (len(body) + 1).to_bytes(2, "little") + body + b"\0"
    helper, judged = bytes([0x55, 0x8B, 0xEC, 0x5D, 0xC3]), bytes([0x55, 0x9A, 0, 0, 0, 0, 0x5D, 0xCB])
    obj = (record(0x80, b"\x03f.c")
           + record(0x96, b"\x00\x05_TEXT\x04CODE")                        # names 1 (empty), 2, 3
           + record(0x98, bytes([0x28]) + (len(helper + judged)).to_bytes(2, "little") + b"\x02\x03\x01")
           + record(0x90, b"\x00\x01" + b"\x07_helper\x00\x00\x00" + b"\x07_judged" + (5).to_bytes(2, "little") + b"\x00")
           + record(0x8C, b"\x04_ext\x00")                                  # external 1, the call's target
           + record(0xA0, b"\x01\x00\x00" + helper + judged)
           + record(0x9C, bytes([0xCC, 0x06, 0x26, 0x01, 0x01]))            # a far pointer at +6 (judged's +1)
           + record(0x8A, b"\x00"))
    assert msc6.function_code(obj, "judged") == (judged, bytes([0, 1, 1, 1, 1, 0, 0, 0]))
    assert msc6.function_code(obj, "_HELPER")[0] == helper and msc6.function_code(obj, "none") is None


def test_pascal_calls_are_said_first_pushed_first():
    """OpenWatcom's __stdcall lists arguments right to left; said as Microsoft's _pascal the list is
    turned around, and an offset with its segment register is one far pointer (SUWIN's sub_1_06AA:
    GetClientRect(hwnd, &rc) then Rectangle(hdc, left, top, right, bottom))."""
    from codebreaker.build import msc6
    text = ("int2 __stdcall16near f(param_1,param_2)\n{\n"
            "  USER_33((int2)&aiStack_c[0],unaff_SS,param_1);\n"
            "  return GDI_22(aiStack_c[3],aiStack_c[2],g(a, b),aiStack_c[0],param_2);\n"
            "  x = USER_25(\"a,(b\",0);\n}")
    names = msc6.pascal_names("extern uint4 __far __stdcall GDI_22();\nextern uint4 __far __stdcall USER_33();", text)
    assert names == {"USER_33", "GDI_22", "f"}
    out = msc6.pascal_order(text, names)
    assert "f(param_2, param_1)" in out
    assert "USER_33(param_1, (void __far *)&aiStack_c[0])" in out
    assert "GDI_22(param_2, aiStack_c[0], g(a, b), aiStack_c[2], aiStack_c[3])" in out
    assert 'USER_25("a,(b",0)' in out                          # not a pascal callee: as it was
    # a 4-byte value split into its two words is the value (lstrcpy's LPCSTR in SUWIN's sub_2_30FC)
    assert msc6.pascal_order("KERNEL_88((int2)param_1, (int2)((uint4)param_1 >> 0x10));", {"KERNEL_88"}) == \
        "KERNEL_88(param_1);"


def test_a_constant_near_address_is_a_variable_and_a_batch_s_errors_name_their_functions(tmp_path):
    """`*(int2 *)0x63b2` is said as a data-segment variable (Microsoft C addresses a variable
    directly, a constant pointer through a register); and a source file of several functions that
    the compiler refuses names the functions its errors are in, so the rest stay together."""
    from codebreaker.build import msc6
    decls, text = msc6.ds_globals("  *(xunknown1 *)0x34ee = 1;\n  x = *(int2 *)0x63b2 + *(int2 *)0x63b2;\n"
                                  "  y = *(uint2 __far *)p;\n")
    assert decls == "extern char codebreaker_d34ee[];\nextern char codebreaker_d63b2[];\n"
    assert "(*(xunknown1 *)codebreaker_d34ee) = 1;" in text and "*(uint2 __far *)p" in text
    files, starts = msc6._batch_files([["a", "b"]], {"a": (0, "int f()\n{\n}"), "b": (0, "int g()\n{\n}")}, "B")
    assert files == [("B0", "int f()\n{\n}\n\nint g()\n{\n}")] and starts == [[(3, "a"), (7, "b")]]
    (tmp_path / "B0.TXT").write_text("B0.C\nB0.C(8) : error C2065: 'x' : undeclared identifier\n")
    assert msc6._refused(str(tmp_path / "B0.TXT"), starts[0]) == ["b"]
    (tmp_path / "B1.TXT").write_text("B1.C\nm.h(211) : error C2059: syntax error : '('\n")
    assert msc6._refused(str(tmp_path / "B1.TXT"), starts[0]) == []     # the header's: none told apart


def test_an_api_called_by_number_is_said_through_the_sdk_where_its_arguments_fit():
    """USER_107 is DefWindowProc(HWND, UINT, WPARAM, LPARAM): five words are four parameters, the
    last two one LONG (MAKELONG, low first); a void API whose value is taken, and a call whose
    arguments fit neither way, keep their numbers."""
    from codebreaker.build import msc6
    sdk = ("LRESULT WINAPI DefWindowProc(HWND, UINT, WPARAM, LPARAM);\nvoid    WINAPI MessageBeep(UINT);\n"
           "LPSTR   WINAPI lstrcpy(LPSTR, LPCSTR);\n")
    protos = msc6.sdk_prototypes(sdk)
    assert protos["DefWindowProc"] == ["HWND", "UINT", "WPARAM", "LPARAM"] and protos.void == {"MessageBeep"}
    text = "  x = USER_107(a5, a4, a3, a2, a1);\n  y = USER_104(a1);\n  USER_104(a1);\n  KERNEL_88(a2, a1, b);\n"
    out = msc6.sdk_calls(text, protos)
    assert "x = DefWindowProc(a5, a4, a3, MAKELONG(a1, a2));" in out
    assert "y = USER_104(a1);" in out and "  MessageBeep(a1);" in out and "KERNEL_88(a2, a1, b);" in out


def test_a_temporary_used_once_goes_back_into_its_expression():
    """SUWIN's sub_2_40D8 was LocalAlloc(LMEM_ZEROINIT, lstrlen(s) + 1) in one expression: the
    decompiler's temporaries -- one reassigned at once, a constant used two statements on -- fold
    back; one used twice, or in a loop's condition, stays."""
    from codebreaker.build import msc6
    text = ("int2 __stdcall16far f(a1,a2)\nint2 a1;\nint2 a2;\n{\n  int2 iVar1;\n  int2 unaff_DS = codebreaker_ds();\n"
            "  int2 a2_00;\n  int2 n;\n  int2 w;\n  \n  a2_00 = 0x40;\n  iVar1 = KERNEL_90(a1,a2);\n"
            "  iVar1 = KERNEL_5(iVar1 + 1,a2_00);\n  n = g();\n  h(n, n);\n  w = g();\n  while (w != 0) {\n  }\n"
            "  return iVar1;\n}")
    out = msc6.fold_temporaries(text)
    assert "  iVar1 = KERNEL_5(KERNEL_90(a1,a2) + 1,0x40);\n" in out and "a2_00" not in out
    assert "  n = g();\n  h(n, n);" in out and "  w = g();\n  while (w != 0)" in out


def test_a_pair_of_parameters_used_only_together_is_one_far_pointer_and_a_c_named_import_is_cdecl():
    """sub_2_40D8's (a1, a2) is only ever passed on as a pair -- offset then segment -- so it was one
    far pointer; a2 used alone keeps them apart. An import exported under a C name (_SUFORMATMESSAGEBOX)
    is a cdecl function."""
    from codebreaker.build import msc6
    text = "int2 __stdcall16far f(a1,a2)\nint2 a1;\nint2 a2;\n{\n  x = KERNEL_90(a1,a2);\n  KERNEL_88(b, a1, a2);\n}"
    out = msc6.join_far_parameters(text)
    assert out.startswith("int2 __stdcall16far f(a1)\nvoid __far * a1;\n{") and "KERNEL_90(a1)" in out and "KERNEL_88(b, a1)" in out
    assert msc6.join_far_parameters(text.replace("}", "  y = a2;\n}")) == text.replace("}", "  y = a2;\n}")
    header = "extern uint4 __far __stdcall SETUPX_132();\nextern uint4 __far __stdcall USER_33();\n"
    out = msc6.cdecl_imports(header, sdk="")
    assert "__cdecl SETUPX_132();" in out and "__stdcall USER_33();" in out


def test_the_last_mile_search_rewrites_a_comparison_the_way_a_source_had_it():
    """Ghidra writes `0x19 < iVar1`; SUWIN's sub_3_200C was `iVar1 >= 0x1a` (cmp cx, 1Ah; jl):
    the search's comparison rewrites give both sides of a constant, and either order of operands."""
    import random
    from codebreaker.verify.authored import search16
    assert search16._LEFT.sub(search16._left, "if (0x19 < iVar1) {") == "if (iVar1 >= 0x1a) {"
    assert search16._COMPARE.sub(search16._compare, "x <= 0x19 && y > 3 && z >= -0x2") == \
        "x < 0x1a && y >= 0x4 && z > -0x3"
    text = "int f()\n{\n  int2 a;\n  uint2 b;\n  \n  if (0x19 < a) {\n  }\n}"
    seen = {search16.rewrite(text, random.Random(s)) for s in range(40)}
    assert text.replace("(0x19 < a)", "(a >= 0x1a)") in seen
    assert text.replace("  int2 a;\n  uint2 b;", "  uint2 b;\n  int2 a;") in seen       # declaration order
    assert text.replace("  int2 a;", "  uint2 a;") in seen                               # signedness
    assert search16.rewrite("int f()\n{\n  return 0;\n}", random.Random(0)) is None


def test_the_frame_structure_is_taken_apart_into_the_locals_a_source_declares():
    """SUWIN's sub_2_0A92: two 260-byte buffers (one reached 10 bytes below its member, as the
    decompiler printed it) and a word read through the structure become two buffers and a variable."""
    from codebreaker.build import msc6
    text = ("int2 f()\n{\n  _Packed struct {\n  xunknown1 axStack_200 [250];\n  xunknown1 axStack_106 [260];\n"
            "  } codebreaker_frame;\n  int2 iVar1;\n\n  g((int2)codebreaker_frame.axStack_106);\n"
            "  g((int2)(codebreaker_frame.axStack_200 + -10));\n"
            "  iVar1 = (*(int2 *)((uint1 *)codebreaker_frame.axStack_200 + 0xf8));\n  return iVar1;\n}")
    out = msc6.split_frame(text)
    assert "codebreaker_frame" not in out
    assert "xunknown1 local_106[260];" in out and "xunknown1 local_20a[258];" in out and "int2 local_108;" in out
    assert "g((int2)((xunknown1 *)&local_106));" in out and "g((int2)((xunknown1 *)&local_20a));" in out
    assert "iVar1 = local_108;" in out
    # a frame reached some other way is left as it was
    odd = text.replace("(int2)codebreaker_frame.axStack_106", "(int2)codebreaker_frame.axStack_999")
    assert msc6.split_frame(odd) == odd


def test_a_c_function_s_arguments_are_what_its_caller_takes_off():
    """`add sp, 6` after a direct call: six bytes; at /Os `pop bx; pop cx`: four; a pop of DX
    (a result's high word) says nothing."""
    import capstone
    from codebreaker.decompile import describe
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    code = bytes.fromhex("6a5c5250" "9a00000000" "83c406" "9a00000000" "5b59" "9a00000000" "5a")
    ins = list(md.disasm(code, 0))
    assert [describe._caller_cleanup(ins, k) for k, i in enumerate(ins) if i.mnemonic == "lcall"] == [6, 4, 0]   # (used with CB_CALLER_ARGS=1)


def test_a_call_s_arguments_are_what_its_callee_removes_of_what_was_pushed():
    """The call-contract check's count: `GlobalAlloc(0x2042, GlobalSize(h))` pushes the flags, then
    GlobalSize's handle -- six bytes are GlobalAlloc's, two GlobalSize's; a push before a call that
    takes nothing is the next call's (USER's sub_14_041A); fewer pushed than the callee removes,
    from the function's start, is a count that differs."""
    from codebreaker.verify.gate16 import contract16
    removes = {0xE0000000: 2, 0xE0000010: 6, 0xE0000020: 0, 0xE0000030: 4}
    names = {0xE0000000: "GlobalSize", 0xE0000010: "GlobalAlloc", 0xE0000020: "none", 0xE0000030: "two"}

    def calls(code):
        return [(c, p) for c, _t, p, _lv in contract16._calls(bytes.fromhex(code), 1, 0, len(bytes.fromhex(code)),
                                                              lambda key: names[key[1]], lambda key: removes[key[1]])]
    assert calls("684220" "57" "9a000000e0" "52" "50" "9a100000e0" "cb") == [("GlobalSize", 2), ("GlobalAlloc", 6)]
    assert calls("bb0100" "56" "9a200000e0" "50" "9a300000e0" "cb") == [("none", None), ("two", 4)]
    assert calls("bb0100" "50" "9a100000e0" "cb") == [("GlobalAlloc", 2)]


def test_source_files_are_told_by_push_cs_calls_and_an_if_else_can_be_turned_around():
    """SUWIN's sub_2_594A calls sub_2_595C as `push cs; call` -- the same source file, so it and
    everything between them is one file; a function no such call reaches is a file of its own.
    The search's branch rewrite: `if (a == 0) {A} else {B}` -> `if (a != 0) {B} else {A}`."""
    import random
    from codebreaker.verify.authored import match16, search16
    # three functions of segment 2 at 0x10, 0x20, 0x30: the first calls the third near (push cs; call)
    a = bytes([0x0E, 0xE8]) + (0x30 - (0x10 + 4)).to_bytes(2, "little", signed=True) + bytes([0xCB])
    where = {1: (2, 0x10), 2: (2, 0x20), 3: (2, 0x30), 4: (2, 0x40)}
    originals = {1: (a, b"\0" * len(a)), 2: (b"\xcb", b"\0"), 3: (b"\xcb", b"\0"), 4: (b"\xcb", b"\0")}
    address = {"f1": 1, "f2": 2, "f3": 3, "f4": 4}
    assert match16.source_files(["f1", "f2", "f3", "f4"], where, originals, address) == [["f1", "f2", "f3"], ["f4"]]
    text = "int f()\n{\n  int2 a;\n  \n  if (a == 0) {\n    g();\n  }\n  else {\n    h();\n  }\n  return a;\n}"
    seen = {search16.rewrite(text, random.Random(s)) for s in range(60)}
    assert "int f()\n{\n  int2 a;\n  \n  if (a != 0) {\n    h();\n  }\n  else {\n    g();\n  }\n  return a;\n}" in seen


def test_a_cextdef_is_numbered_with_the_other_externals():
    """Microsoft C 8 names some externals through LNAMES (CEXTDEF, 0xBC): a fix-up's external index
    counts them with the EXTDEFs (SETUPX's file of sub_2_1D92 at /O1 stopped the reader)."""
    from codebreaker.formats import omf

    def record(kind, body):
        return bytes([kind]) + (len(body) + 1).to_bytes(2, "little") + body + b"\0"
    code = bytes([0x9A, 0, 0, 0, 0, 0xCB])
    obj = (record(0x80, b"\x03f.c") + record(0x96, b"\x00\x05_TEXT\x04CODE\x04_far")
           + record(0x98, bytes([0x28]) + len(code).to_bytes(2, "little") + b"\x02\x03\x01")
           + record(0xBC, b"\x04\x00")                                      # external 1: name 4, _far
           + record(0x8C, b"\x04_ext\x00")                                  # external 2
           + record(0xA0, b"\x01\x00\x00" + code)
           + record(0x9C, bytes([0xCC, 0x01, 0x26, 0x02, 0x02]))            # a far pointer at +1, to external 2
           + record(0x8A, b"\x00"))
    o = omf.read(obj)
    assert o.ext == ["_far", "_ext"] and o.fix[0][3] == ("ext", "_ext")


def test_wine_s_spec_types_an_import_and_its_calls_are_said_one_argument_to_a_parameter():
    """Wine's .spec: `33 pascal -ret16 GetClientRect(word ptr)`, a stub's arguments in its comment.
    lstrcpy(segptr, str) called with four words gets its pointers joined, high word first; a call
    whose words fit neither way leaves its import without a prototype."""
    from codebreaker.build import msc6
    from codebreaker.symbols import wine16
    spec = wine16.read_spec("33  pascal -ret16 GetClientRect(word ptr) GetClientRect16\n"
                            "7    stub     IpFindFirstLine #(word str str ptr)\n"
                            "88  pascal   lstrcpy(segptr str) lstrcpy16\n1 pascal X(double) X16\n")
    assert spec[33] == wine16.Export("GetClientRect", "pascal", ["word", "ptr"], True, False)
    assert spec[7].args == ["word", "str", "str", "ptr"] and spec[7].stub and spec[1].args is None
    ex = {("USER", 33): spec[33], ("KERNEL", 88): spec[88], ("USER", 1): wine16.Export("Y", "pascal", ["word"], True, False)}
    header = ("extern uint4 __far __stdcall USER_33();\nextern uint4 __far __stdcall KERNEL_88();\n"
              "extern uint4 __far __stdcall USER_1();\n")
    texts = {"f": "  USER_33(param_1, (void __far *)&rc);\n  KERNEL_88(a2, a1, b2, b1);\n  USER_1(1, 2);\n"}
    h, t = msc6.wine_typing(header, texts, lambda m, n: ex.get((m, n)))
    assert "extern uint2 __far __stdcall USER_33(uint2, void __far *);" in h
    assert "extern uint4 __far __stdcall KERNEL_88(void __far *, void __far *);" in h
    assert "extern uint4 __far __stdcall USER_1();" in h
    assert "KERNEL_88((void __far *)(((uint4)(uint2)(a2) << 16) | (uint2)(a1)), " \
           "(void __far *)(((uint4)(uint2)(b2) << 16) | (uint2)(b1)));" in t["f"]
    assert "USER_1(1, 2);" in t["f"]


def test_a_run_time_library_routine_is_named_outside_the_linker_s_bytes(monkeypatch):
    """A function that is a library routine byte for byte, outside the places the linker filled in
    (the routine's fix-ups, the original's relocations), is the library's -- __aFlmul in SUWIN."""
    from codebreaker.symbols import libraries
    from codebreaker.verify.gate16 import runtime16
    routine = bytes([0x55, 0x8B, 0xEC, 0x9A, 0, 0, 0, 0, 0x5D, 0xCA, 0x08, 0x00])
    mask = bytes([0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0])
    monkeypatch.setattr(runtime16, "_routines", [libraries.Function("MDLLCEW.LIB:__aFlmul", routine, mask, 0)])
    linked = bytes([0x55, 0x8B, 0xEC, 0x9A, 0x34, 0x12, 0xFF, 0xFF, 0x5D, 0xCA, 0x08, 0x00])
    other = bytes([0x55, 0x8B, 0xEC, 0x9A, 0x34, 0x12, 0xFF, 0xFF, 0x5D, 0xCA, 0x06, 0x00])
    got = runtime16.identify({"f": (linked, b"\0" * 12), "g": (other, b"\0" * 12)})
    assert got == {"f": "MDLLCEW.LIB:__aFlmul"}


def test_a_function_reading_ax_dx_or_bx_before_writing_it_takes_them_as_fastcall_arguments():
    """SUWIN's 7:0000 begins `push bp / mov bp, sp / push ax` -- its char argument kept in its frame:
    Microsoft's _fastcall. `xor ax, ax` writes AX without reading it; cdq writes DX (as EDX)."""
    from codebreaker.decompile import describe
    fastcall = bytes.fromhex("55" "8bec" "50" "a07a6c" "3846fe" "7504" "33c0" "c9" "c3")
    plain = bytes.fromhex("c8080000" "57" "56" "33c0" "6699" "8956fa" "c9" "c3")     # enter; xor ax,ax; cdq; mov [bp-6],dx
    assert describe.register_arguments(fastcall, 0)
    assert not describe.register_arguments(plain, 0)


def test_a_callee_that_gives_back_the_caller_s_ax_returns_no_dx_either():
    """The caller-results rule (CB_CALLER_RESULTS=1) makes a callee four-byte only where it hands
    back a DX of its own. KEYBOARD's 9:0367 saves AX and BX, calls the VxD's entry through [20h]
    and gives both back: no result at all. The same call with AX left as the call set it hands
    the call's DX:AX on; one that sets AX before it pushes it (KRNL386's 2:4C91) saves nothing."""
    from types import SimpleNamespace
    from codebreaker.decompile import describe
    saved = bytes.fromhex("50" "53" "bb3a00" "b81400" "ff1e2000" "5b" "58" "c3")   # push ax/bx ... pop bx/ax; ret
    passed = bytes.fromhex("b81400" "ff1e2000" "c3")                               # mov ax, 14h; lcall [20h]; ret
    made = bytes.fromhex("b80100" "50" "ba0200" "58" "c3")                          # mov ax, 1; push ax; mov dx, 2; pop ax
    code = saved.ljust(0x20, b"\x90") + passed.ljust(0x20, b"\x90") + made
    d = SimpleNamespace(names={describe.address(1, o): "f%x" % o for o in (0, 0x20, 0x40)}, images={1: code})
    own = describe._DxReturns(d)
    assert not own.every_way((1, 0), set())
    assert own.every_way((1, 0x20), set())
    assert own.every_way((1, 0x40), set())


def test_an_exported_function_s_parameters_are_typed_by_wine_s_spec():
    """IpFindFirstLine(word, str, str, ptr), pascal: in stack order a pointer, two strings, a word --
    seven words as decompiled, four parameters as written; a half used alone is FP_OFF / FP_SEG."""
    from codebreaker.build import msc6
    from codebreaker.symbols.wine16 import Export
    text = ("int2 __stdcall16far IPFINDFIRSTLINE(a1,a2,a3,a4,a5,a6,a7)\nint2 a1;\nint2 a2;\nint2 a3;\nint2 a4;\n"
            "int2 a5;\nint2 a6;\nint2 a7;\n{\n  x = g(a1,a2);\n  y = a3 + 1;\n  h(a4, a5,a6);\n  return a7;\n}")
    out = msc6.wine_definition(text, Export("IpFindFirstLine", "pascal", ["word", "str", "str", "ptr"], False, True))
    assert out.startswith("int2 __stdcall16far IPFINDFIRSTLINE(a1,a3,a5,a7)\nvoid __far * a1;\nvoid __far * a3;\n"
                          "void __far * a5;\nint2 a7;\n{")
    assert "x = g(a1);" in out and "y = FP_OFF(a3) + 1;" in out and "h(FP_SEG(a3), a5);" in out
    assert msc6.wine_definition(text, Export("X", "pascal", ["word"], False, False)) == text   # (words that do not add up)


def test_only_visual_c_1_52c_runs_without_the_emulator(monkeypatch):
    """C/C++ 8.00c's CL is a Win32 program and runs on Windows itself; Microsoft C 6.0a is DOS's and
    keeps the emulator; native16 = off keeps the emulator for both."""
    from codebreaker.base import settings
    from codebreaker.build import msc6
    assert msc6._native("msc6") is None
    real = settings.get
    monkeypatch.setattr(settings, "get", lambda k, *a: "off" if k == "native16" else real(k, *a))
    assert msc6._native("msvc152") is None


def test_a_backpatch_record_is_added_into_the_code():
    """Microsoft C 8 at /Od writes `sub sp, 0` and gives the frame's size in a BAKPAT record after
    the function: the reader adds it in (SUWIN's sub_1_06AA is the original's bytes only with it)."""
    from codebreaker.formats import omf

    def record(kind, body):
        return bytes([kind]) + (len(body) + 1).to_bytes(2, "little") + body + b"\0"
    code = bytes([0x55, 0x8B, 0xEC, 0x81, 0xEC, 0, 0, 0x8B, 0xE5, 0x5D, 0xC3])
    obj = (record(0x80, b"\x03f.c") + record(0x96, b"\x00\x05_TEXT\x04CODE")
           + record(0x98, bytes([0x28]) + len(code).to_bytes(2, "little") + b"\x02\x03\x01")
           + record(0xA0, b"\x01\x00\x00" + code)
           + record(0xB2, b"\x01\x01" + (5).to_bytes(2, "little") + (10).to_bytes(2, "little"))
           + record(0x8A, b"\x00"))
    assert omf.read(obj).seg["_TEXT"][3:7] == bytes([0x81, 0xEC, 10, 0])


def test_a_verdict_is_for_the_compiled_code():
    """The signature of what was built does not change with the source's name, date or notes,
    nor with the header; it does when the function's code, or the code of one it calls, does."""
    from codebreaker.build import functions
    from codebreaker.verify import reconstruct

    def record(kind, body):
        return bytes([kind]) + (len(body) + 1).to_bytes(2, "little") + body + b"\0"

    def obj(name, note, code):
        return record(0x80, bytes([len(name)]) + name) + record(0x88, note) + record(0xA0, b"\x01\x00\x00" + code) + record(0x8A, b"\x00")
    d = tempfile.mkdtemp()

    def digest(*parts):
        with open(os.path.join(d, "x.obj"), "wb") as f:
            f.write(obj(*parts))
        return functions.object_digest(os.path.join(d, "x.obj"))
    assert digest(b"f1.c", b"made monday", b"\x55\xc3") == digest(b"f9.c", b"made friday", b"\x55\xc3")
    assert digest(b"f1.c", b"made monday", b"\x55\xc3") != digest(b"f1.c", b"made monday", b"\x56\xc3")
    assert functions.object_digest(os.path.join(d, "none.obj")) is None

    class M(object):
        order = ["a", "b", "c"]
        texts = {"a": "int a(){ return b(); }", "b": "int b(){ return 1; }", "c": "int c(){ return 2; }"}
    m = M()
    m.header = "one header"
    was = reconstruct.signatures(m, None, own={"a": "A", "b": "B", "c": "C"})
    m.header = "another header"
    m.texts = dict(M.texts, c="int c(){ /* said differently */ return 2; }")
    assert reconstruct.signatures(m, None, own={"a": "A", "b": "B", "c": "C"}) == was
    now = reconstruct.signatures(m, None, own={"a": "A", "b": "B2", "c": "C"})
    assert now["b"] != was["b"] and now["a"] != was["a"] and now["c"] == was["c"]


def test_cache_files_results_under_what_made_them():
    from codebreaker.base import cache
    k = cache.key("tool:1:2", ["-a", "-b"], "text")
    assert k != cache.key("tool:1:3", ["-a", "-b"], "text") and k != cache.key("tool:1:2", ["-a"], "-b", "text")
    kind = "test-%d" % os.getpid()
    assert cache.get(kind, k) is None
    cache.put(kind, k, b"made once")
    assert cache.get(kind, k) == b"made once"
    os.environ["CB_NO_CACHE"] = "1"
    try:
        assert cache.get(kind, k) is None
    finally:
        del os.environ["CB_NO_CACHE"]
    v = cache.key("a value")
    assert cache.value(kind, v) is None
    cache.keep(kind, v, ({"kind": "error"}, (1, 2)))
    assert cache.value(kind, v) == ({"kind": "error"}, (1, 2))
    import shutil
    cache._close(kind)
    shutil.rmtree(os.path.join(cache.root(), kind), ignore_errors=True)


def test_every_kind_keeps_fingerprints_and_writes_calls_alike():
    from codebreaker.verify import compare, lane_common
    kept, runs = {}, []
    was = compare.cache_get, compare.cache_put, os.environ.pop("CB_NO_FP_CACHE", None)
    compare.cache_get, compare.cache_put = kept.get, kept.__setitem__
    try:
        def run():
            runs.append(1)
            return {"exit": "ret", "truncated": len(runs) == 1}
        lane_common.remembered("k", run, keep=lambda fp: not fp["truncated"])        # turned down: made again
        lane_common.remembered("k", run, keep=lambda fp: not fp["truncated"])
        lane_common.remembered("k", run)
        assert len(runs) == 2 and "k" in kept
        lane_common.remembered("e", lambda: {"err": "x"})                              # not an observation
        assert "e" not in kept
        os.environ["CB_NO_FP_CACHE"] = "1"                                              # every kind obeys it
        lane_common.remembered("k", run)
        assert len(runs) == 3
    finally:
        compare.cache_get, compare.cache_put = was[:2]
        os.environ.pop("CB_NO_FP_CACHE", None)
        if was[2] is not None:
            os.environ["CB_NO_FP_CACHE"] = was[2]
    # one normaliser: inputs by position, small constants signed, the frame and the own image by kind
    arg = lane_common.argument
    assert arg(0x1234, [0x99, 0x1234]) == "a1" and arg(0x12, [0x12], above=0xFF) == "12"
    assert arg(0xFFFFFFFE, small=True) == "-2" and arg(0x7000, frame=((0x6000, 0x8000), None)) == "S"
    assert arg(0x10, image=(0, 0x100)) == "P" and arg(0x10, image=lambda v: False) == "10"
    seq = [("A", [1, 2]), ("NOISE", [3]), ("B", None)] * 3
    assert lane_common.calls(seq, str, skip=lambda n: n == "NOISE", cap=4) == ("A(1,2)", "B", "A(1,2)", "B", "...")
    from codebreaker.verify.lanes import win16
    assert win16._call({"api": "KERNEL.X", "arg_bytes": 6, "args_stack": [0x10, 0x1F, 0x1234]},
                       [0x1234], {"SS": 0x1F, "DS": 0x2F}) == "KERNEL.X(stack,SS,a0)"


def test_the_judge_s_build_has_the_original_s_code_segment_where_c_still_reads_it():
    # KEYBOARD's ENABLE reads its segment through CS at offsets from a register: c_text could not
    # name them, so the judge's build puts the original's segment 10 first in the rebuilt one
    import types
    from codebreaker.verify.lanes import win16

    class Nef:
        data = bytes(range(16)) * 4

        def segments(self):
            return [{"idx": 9, "file_off": 0, "length": 8}, {"idx": 10, "file_off": 16, "length": 8}]
    m = types.SimpleNamespace(code_reads={9: {"read": 2, "named": 2, "left": []},
                                          10: {"read": 5, "named": 4, "left": ["ENABLE: ... int_DI"]}},
                              by_va={1: "ENABLE", 2: "VKKEYSCAN"}, where={1: (10, 0), 2: (9, 0)},
                              texts={"ENABLE": "MK_FP(codebreaker_cs(), x)", "VKKEYSCAN": "y"})
    old = os.environ.pop("CB_CODE_DATA", None)
    try:
        assert win16._code_images(m, Nef()) == {10: bytes(range(8))}
        os.environ["CB_CODE_DATA"] = "0"                     # (nothing named: every segment read through CS)
        assert win16._code_images(m, Nef()) == {10: bytes(range(8))}
    finally:
        os.environ.pop("CB_CODE_DATA", None)
        if old is not None:
            os.environ["CB_CODE_DATA"] = old


def test_the_uart_and_the_interrupt_controller_answer_as_the_chips_do():
    # COMM.DRV waits for an empty transmitter, sets the divisor behind DLAB, tells a 16550A by its
    # FIFO bits, loops on the interrupt identification until nothing is pending, unmasks its IRQ
    from codebreaker.emulate import ports16
    from codebreaker.verify.lanes import win16
    d = ports16.Devices()
    assert d.read(0x3FD) & 0x60 == 0x60 and d.read(0x3FD) & 1         # transmitter empty, data waiting
    d.write(0x3FB, 0x83)                                              # DLAB
    d.write(0x3F8, 0x06), d.write(0x3F9, 0x00)
    assert d.read(0x3F8) == 0x06 and d.read(0x3FB) == 0x83
    d.write(0x3FB, 0x03)
    assert d.read(0x3F8) == ord("A")                                  # (the receiver, not the divisor)
    d.write(0x3FF, 0x5A)
    assert d.read(0x3FF) == 0x5A                                      # scratch
    d.write(0x3FA, 0xC7)
    assert d.read(0x3FA) & 0xC0 == 0xC0                               # a 16550A's FIFOs
    d.write(0x3F9, 0x02)                                              # transmitter interrupt on
    assert d.read(0x3FA) & 0x0F == 0x02 and d.read(0x3FA) & 0x0F == 0x01   # once, then nothing
    assert d.read(0x21) == 0xB8
    d.write(0x21, 0xA8), d.write(0x20, 0x20)
    assert d.read(0x21) == 0xA8
    assert d.read(0x60) == 0 and all(p != 0x60 for _n, _w, p, _v in d.log)  # (not a modelled port)
    assert d.read(0x005) & 0x60 == 0x60                               # (base 0: a port record unset)
    # what was done to them is compared with the calls, in the order it was done
    hits = [{"api": "KERNEL.X", "n": 5}]
    io = [(2, "out", 0x3FB, 0x83), (9, "in", 0x3FD, 0x60)]
    from codebreaker.emulate import windows16
    r = windows16.record({"exit": "ret", "imports_hit": hits, "io": io})
    assert win16._effects(r, lambda h: h["api"]) == ("out(3fb,83)", "KERNEL.X", "in(3fd)")


def test_the_governor_shares_one_budget():
    # slots taken are held until given back, across holders; never more than the machine's workers,
    # never fewer than one
    from codebreaker.base import parallel
    old = os.environ.get("CB_WORKERS")
    os.environ["CB_WORKERS"] = "2"
    try:
        with parallel.slots(want=5, mb_each=1) as a:
            assert 1 <= a <= 2, a
            with parallel.slots(want=5, mb_each=1, limit=0) as b:
                assert b == 1                                  # (none free: it goes on with one, not waiting for ever)
        with parallel.slots(want=2, mb_each=1) as c:
            assert c >= 1
    finally:
        if old is None:
            del os.environ["CB_WORKERS"]
        else:
            os.environ["CB_WORKERS"] = old


def test_an_instruction_is_decoded_once():
    import capstone
    from codebreaker.decompile import describe
    code = bytes.fromhex("5589e58b4604e8100083c4025dc3")
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    for at in (0, 1, 3, 6, 9, 12, 13):
        mine, theirs = describe._one(code, at), next(md.disasm(code[at:at + 16], at), None)
        assert (mine.mnemonic, mine.op_str, mine.size) == (theirs.mnemonic, theirs.op_str, theirs.size)
    assert describe._one(code, 3) is describe._one(code, 3)                # (the same object: kept)
    assert describe._one(bytearray(code), 3) is not describe._one(code, 3)  # (another segment, its own)


def test_chunks_come_back_in_address_order():
    # the decompiler's chunks run side by side; their blocks are put back in address order
    from codebreaker.decompile import decompiler
    parts = ["/*@@ b 10020 */\nB\n\n/*@@ d 10040 */\nD\n", "/*@@ a 10010 */\nA\n\n/*@@ c 10030 */\nC\n"]
    blocks = sorted((int(m.group(1), 16), m.group(0)) for p in parts for m in decompiler._BLOCK.finditer(p))
    assert "".join(b for _v, b in blocks) == "/*@@ a 10010 */\nA\n\n/*@@ b 10020 */\nB\n\n/*@@ c 10030 */\nC\n/*@@ d 10040 */\nD\n"


def test_programs_stored_one_after_another_are_each_found():
    # CodeView 4.1's CV.EXE: a stub, the DOS extender, CodeView -- three MZ programs back to back
    from codebreaker.formats import dos

    def mz(body_paras, extra=b""):
        size = 32 + body_paras * 16                          # (a 32-byte header, then the body)
        h = bytearray(32)
        h[0:2] = b"MZ"
        h[2:4] = (size % 512).to_bytes(2, "little")
        h[4:6] = ((size + 511) // 512).to_bytes(2, "little")
        h[8:10] = (2).to_bytes(2, "little")                  # (header paragraphs)
        return bytes(h) + bytes(body_paras * 16) + extra
    one, two = mz(4), mz(8)
    assert dos.chain(one) == []                              # (one alone: not a chain)
    assert dos.chain(one + two) == [(0, len(one)), (len(one), len(two))]
    assert dos.chain(one + b"\0" * 6 + two)[1] == (len(one) + 6, len(two))   # (zeros to a paragraph between)
    assert dos.chain(one + b"junk" + two) == []              # (something else after it: no chain)


def main():
    for fn in (test_programs_stored_one_after_another_are_each_found, test_the_governor_shares_one_budget, test_an_instruction_is_decoded_once,
               test_chunks_come_back_in_address_order,
               test_the_judge_s_build_has_the_original_s_code_segment_where_c_still_reads_it,
               test_the_uart_and_the_interrupt_controller_answer_as_the_chips_do, test_function_store, test_variant_store, test_repair_knobs_preserve_meaning,
               test_a_verdict_holds_until_what_it_was_for_changes, test_cache_files_results_under_what_made_them,
               test_every_kind_keeps_fingerprints_and_writes_calls_alike,
               test_period_source_gives_the_declarations_an_author_had, test_breaks_change_what_a_function_does,
               test_other_shapes_of_the_same_function,
               test_object_file_of_the_16_bit_compiler_is_read, test_a_verdict_is_for_the_compiled_code,
               test_repair_search_with_any_compiler, test_period_compiler_gives_the_original_back,
               test_reconstruct_is_incremental,
               test_a_callee_that_gives_back_the_caller_s_ax_returns_no_dx_either):
        try:
            fn()
            RESULTS.append((fn.__name__, "PASS", ""))
        except Skip as s:
            RESULTS.append((fn.__name__, "SKIP", str(s)))
        except Exception as e:
            RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, str(e)[:500])))
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
