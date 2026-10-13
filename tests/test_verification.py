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
               test_period_source_gives_the_declarations_an_author_had,
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
