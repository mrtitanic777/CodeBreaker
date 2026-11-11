#!/usr/bin/env python3
"""Tests for the port: recovered C of before the standard made C a compiler of today builds, the meaning kept.
Run:  python tests/test_port.py     (exit code 0 = all passed)

The rewriting needs no compiler; the check that a port builds and runs is SKIPPED where no gcc or clang is found."""
import json
import os
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.port import pre_ansi                                                   # noqa: E402
from codebreaker.port import __main__ as port_main                                      # noqa: E402

PROFILE = json.load(open(os.path.join(ROOT, "codebreaker", "port", "runtimes", "dos_c_early.json")))
RESULTS = []

# a program as the C of 1984 had it: old-style definitions, a member used on a char *, a name buffer whose
# next member is the frame's next variable, a 16-bit rotate, a long printed, the library's own stdio among it
TREE = {
    "app.h": "struct name {\n\tchar n_text[8];\n\tlong n_pos;\n};\nlong g_total;\n",
    "app.c": ('#include "stdio.h"\n#include "app.h"\n\n'
              "/* a word rotated left by n, in 16 bits */\nrot(w, n)\nregister unsigned w, n;\n{\n"
              "\treturn (w >> (16 - n)) & ~(0177777 << n) | w << n;\n}\n\n"
              "/* where the name is: put after it */\nfind(s)\nchar *s;\n{\n\ts->n_pos = 0x12345L;\n}\n\n"
              "main(argc, argv)\nchar **argv;\n{\n\tlong pos;\n\tchar text[8];\n\tfind(text);\n"
              "\tg_total = pos;\n\tfprintf(stdout, \"%x %lx\\r\\n\", rot(0xC001, 2), g_total);\n\treturn 0;\n}\n"),
    "stdio.c": "FILE *fopen(name, mode)\nchar *name, *mode;\n{\n\treturn 0;\n}\n",
    "stdio.h": "#define EOF (-1)\n",
}


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


def ported():
    p = pre_ansi.Port(dict(TREE), PROFILE)
    out = p.modernize()
    out.update({k: v for k, v in p.headers().items() if v})
    return p, out


def test_runtime_left_out():
    p, out = ported()
    assert "stdio.c" in p.left_out and "stdio.h" in p.left_out, p.left_out
    assert "stdio.c" not in out


def test_prototypes_and_widths():
    p, out = ported()
    assert "int16_t rot(register uint16_t w, register uint16_t n)" in out["app.c"], out["app.c"]
    assert "int16_t rot(uint16_t w, uint16_t n);" in out["proto.h"], out["proto.h"]
    assert "void find(char *s)" in out["app.c"]
    assert "int32_t\tn_pos;" in out["app.h"] or "int32_t n_pos;" in out["app.h"], out["app.h"]
    assert "extern int32_t g_total;" in out["app.h"] and "int32_t g_total;" in out["data.c"]


def test_formats():
    p, out = ported()
    assert '"%X %lX\\r\\n", (uint16_t)rot(0xC001, 2), (long)g_total' in out["app.c"], out["app.c"]


def test_frame_kept_side_by_side():
    p, out = ported()
    assert "frame.text" in out["app.c"] and "frame.pos" in out["app.c"], out["app.c"]
    body = out["app.c"][out["app.c"].index("struct {"):]
    assert body.index("char text[8];") < body.index("int32_t pos;")


def _widening_port(files, fns):
    """a Port holding only what the widening passes read: the texts and the functions (name, ret, params)"""
    p = pre_ansi.Port({}, PROFILE)
    p.files = dict(files)
    p.fns = {}
    for name, ret, params in fns:
        f = pre_ansi.Fn(name, ret, params, False, 1)
        f.body, f.implicit = "", set()
        p.fns[name] = f
    return p


def test_widening_passes_on_a_definition_that_never_closes():
    # a truncated file: the braces of the definition never close, and the passes indexed past its last line
    import collections
    text = "void f(int16_t *q, intptr_t p)\n{\n    *(int *)q = p;\n"
    out = pre_ansi.whole_word_stores(text, collections.defaultdict(list), "a.c")
    assert "*(intptr_t *)q = p;" in out, out
    p = _widening_port({"a.c": text}, [("f", "", [("q", "int16_t *q"), ("p", "intptr_t p")])])
    files = {"a.c": text}
    assert pre_ansi.passed_on(p, files) is False
    assert pre_ansi.stored_wide(p, files) is False


def test_stored_wide_keeps_the_declarations_it_changed():
    # pop() is mended in a.c; its declaration in b.c is changed with it, and b.c's own function (other) is mended
    # after: its text was the one from before, written back over the declaration
    a_c = ('#include "s.h"\nvoid push(intptr_t n)\n{\n    struct stk *s;\n    s->v = n;\n}\n'
           "int16_t pop(void)\n{\n    int16_t v;\n    struct stk *s;\n    v = s->v;\n    return v;\n}\n")
    b_c = ('#include "s.h"\nint16_t pop(void);\nint16_t other(void)\n{\n    int16_t w;\n    struct stk *s;\n'
           "    w = s->v;\n    return w;\n}\n")
    files = {"a.c": a_c, "b.c": b_c, "s.h": "struct stk { int16_t v; };\n"}
    p = _widening_port(files, [("push", "", [("n", "intptr_t n")]), ("pop", "int16_t", []), ("other", "int16_t", [])])
    out = dict(files)
    assert pre_ansi.stored_wide(p, out) is True
    assert "intptr_t pop(void)" in out["a.c"] and "intptr_t pop(void);" in out["b.c"], out["b.c"]
    assert "intptr_t other(void)" in out["b.c"] and "intptr_t w;" in out["b.c"], out["b.c"]
    assert "intptr_t v;" in out["s.h"], out["s.h"]


def test_builds_and_runs():
    cc = port_main.find(port_main.load_compilers()["gcc"]["find"]) or \
        port_main.find(port_main.load_compilers()["clang"]["find"])
    if not cc:
        raise Skip("no gcc or clang")
    src, dst = tempfile.mkdtemp(), tempfile.mkdtemp()
    try:
        for n, t in TREE.items():
            open(os.path.join(src, n), "w", newline="\n").write(t)
        name = "clang" if "clang" in os.path.basename(cc) else "gcc"
        rc = port_main.main([src, os.path.join(dst, "out"), "--compiler", name, "--cc", cc])
        assert rc == 0, open(os.path.join(dst, "out", "PORT.md")).read()[-1500:]
        exe = os.path.join(dst, "out", "program.exe" if os.name == "nt" else "program")
        r = subprocess.run([exe], capture_output=True, env=port_main.env_for(cc))
        assert r.stdout == b"7 12345\r\n", r.stdout          # (0xC001 rotated in 16 bits: 0x0007; the name's
    finally:                                                 #  pos put in the frame's next variable)
        shutil.rmtree(src, ignore_errors=True)
        shutil.rmtree(dst, ignore_errors=True)


def test_octal_constant_in_a_pass():
    """a number written with a leading 0 is octal in C: malloc(010) is 8 bytes - the pass that reads it no longer
    raises ValueError (int("010", 0) did) but reads the 8"""
    files = {"a.h": "struct node { int a; int b; int c; int d; };\n",
             "a.c": '#include "a.h"\nchar *pool_alloc()\n{\n\treturn malloc(010);\n}\n'
                    "void g()\n{\n\tstruct node *p;\n\tp = (struct node *)pool_alloc();\n}\n"}
    p = pre_ansi.Port(dict(files), PROFILE)
    p.fixed_allocators()
    assert "malloc(sizeof(struct node))" in p.files["a.c"], p.files["a.c"]
    assert pre_ansi.c_int("010") == 8 and pre_ansi.c_int("0x10") == 16 and pre_ansi.c_int("-5") == -5


SIZES_TABLE = (
    "struct recfmt { char *name; int n; };\n"
    "struct psym { int a; int b; int c; int d; int e; int f; int g; int h; int i; int j; int k; int l; int m; int o; int p;"
    " int q; };\n"
    'struct recfmt g_2096[] = { { "tkn", 22 }, { "tuk", 32 } }\n;\n'
    "int other[2][2] = { { 1, 22 }, { 3, 32 } };\n"
    "char *f03f02(n, z, pool)\nint n, z, pool;\n{\n\treturn 0;\n}\n"
    "f()\n{\n\tstruct psym *p;\n\tint n, kind;\n\tn = g_2096[kind].n;\n\tp = (struct psym *)f03f02(32, 0, 1);\n"
    "\tp = (struct psym *)f03f02(n, 0, 1);\n}\n")


def test_size_table_ends_at_its_own_brace():
    """a table of old byte counts written `}` and `;` apart: its rows were those up to the next `};` of the file - the
    rows of the array that came after it were changed too (22 an offsetof, 32 a sizeof in `other`)"""
    p = pre_ansi.Port({"t.c": SIZES_TABLE}, PROFILE)
    p.size_tables()
    out = p.files["t.c"]
    assert '{ "tkn", offsetof(struct psym, l) }, { "tuk", sizeof(struct psym) }' in out, out
    assert "int other[2][2] = { { 1, 22 }, { 3, 32 } };" in out, out
    # a table that never closes has no rows at all
    q = pre_ansi.Port({"t.c": SIZES_TABLE.replace(" }\n;\n", "\n")}, PROFILE)
    q.size_tables()
    assert "offsetof" not in q.files["t.c"] and "sizeof" not in q.files["t.c"], q.files["t.c"]


if __name__ == "__main__":
    for t in (test_runtime_left_out, test_prototypes_and_widths, test_formats, test_frame_kept_side_by_side,
              test_octal_constant_in_a_pass, test_size_table_ends_at_its_own_brace,
              test_widening_passes_on_a_definition_that_never_closes, test_stored_wide_keeps_the_declarations_it_changed,
              test_builds_and_runs):
        check(t)
    for n, v, why in RESULTS:
        print("%-32s %s %s" % (n, v, why))
    print("%d passed, %d failed" % (sum(v != "FAIL" for n, v, w in RESULTS), sum(v == "FAIL" for n, v, w in RESULTS)))
    sys.exit(1 if any(v == "FAIL" for n, v, w in RESULTS) else 0)
