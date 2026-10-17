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

from codebreaker.port import early_c                                                   # noqa: E402
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
    p = early_c.Port(dict(TREE), PROFILE)
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


if __name__ == "__main__":
    for t in (test_runtime_left_out, test_prototypes_and_widths, test_formats, test_frame_kept_side_by_side,
              test_builds_and_runs):
        check(t)
    for n, v, why in RESULTS:
        print("%-32s %s %s" % (n, v, why))
    sys.exit(1 if any(v == "FAIL" for n, v, w in RESULTS) else 0)
