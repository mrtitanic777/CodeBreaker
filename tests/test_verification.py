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
from codebreaker.verify import win32 as pipeline

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
    from codebreaker.verify import repair, match
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
    from codebreaker.verify import repair
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
    from codebreaker.verify import match
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
    from codebreaker.verify import match, repair
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


def test_field_accesses_are_written_as_fields():
    """`*(T *)(p + N)` through an argument known to be a structure becomes `p->f_N`, declared at
    that offset with that type; an access of another type at the same place is left alone."""
    from codebreaker.analyze import image, structures
    from codebreaker.decompile import struct_text
    from codebreaker.verify import match
    texts = {"f": "int4 f(int4 a1,int4 a2)\n{\n  *(int4 *)(a1 + 8) = *(int4 *)a1 + a2;\n"
                  "  return *(uint1 *)(a1 + 8) + *(int4 *)(a1 + 8) + (**(code **)(a1 + 0x10))(a2);\n}",
             "g": "int4 g(int4 a1)\n{\n  return *(int4 *)(a1 + 8);\n}"}
    found = [{"name": "struct_1", "roots": [("arg", 0x1000, 0)]}]
    real = (image.load, structures.recover)
    image.load, structures.recover = (lambda path: None), (lambda img: found)
    try:
        header, out = struct_text.apply("x.dll", "typedef int int4;\n", texts, {0x1000: "f", 0x2000: "g"})
    finally:
        image.load, structures.recover = real
    assert "((struct_1 *)a1)->f_8 = ((struct_1 *)a1)->f_0 + a2;" in out["f"], out["f"]
    assert "*(uint1 *)(a1 + 8) + ((struct_1 *)a1)->f_8 + (*((struct_1 *)a1)->f_10)(a2)" in out["f"], out["f"]
    assert out["g"] == texts["g"]                                   # g's argument is not known to be one
    assert "typedef struct struct_1 {\n  int4 f_0;\n  uint1 _gap_4[4];\n  int4 f_8;\n  uint1 _gap_c[4];\n  code *f_10;\n} struct_1;" in header, header
    near = match.needed_header(header, out["f"])
    assert "int4 f_8;" in near and "struct_1" not in match.needed_header(header, out["g"])


def test_other_shapes_of_the_same_function():
    """An if-chain on one value is offered as a switch, and `r = x;` before the final `return r;`
    as `return x;` -- but not through a variable narrower than what the function returns, nor
    where a branch holds a `break`."""
    from codebreaker.verify import repair
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
    import shutil
    shutil.rmtree(os.path.dirname(os.path.dirname(cache._path(kind, k))), ignore_errors=True)


def main():
    for fn in (test_function_store, test_variant_store, test_repair_knobs_preserve_meaning,
               test_a_verdict_holds_until_what_it_was_for_changes, test_cache_files_results_under_what_made_them,
               test_period_source_gives_the_declarations_an_author_had, test_breaks_change_what_a_function_does,
               test_field_accesses_are_written_as_fields, test_other_shapes_of_the_same_function,
               test_object_file_of_the_16_bit_compiler_is_read, test_a_verdict_is_for_the_compiled_code,
               test_repair_search_with_any_compiler, test_period_compiler_gives_the_original_back,
               test_reconstruct_is_incremental):
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
