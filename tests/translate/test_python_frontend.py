"""The Python front end: snippets -> L2 -> C++17 that compiles and gives Python's results; and the refusals.

The behaviour test translates one small module of the Python semantics that bite (floor division and
modulo signs, negative indices and clamped slices, dict insertion order, struct, int.from_bytes, list
aliasing, records, classes with a virtual method, sorting, short-circuit any/all, exceptions), builds
it with MSVC (skipped where there is none) and compares every run_* function's result with Python's.
"""
import json
import os
import shutil
import subprocess
import sys
import textwrap

import pytest

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, ROOT)

from codebreaker.translate import ir, l2                                   # noqa: E402
from codebreaker.translate.frontends import python as pyfe                 # noqa: E402
from codebreaker.translate.backends import cpp17                           # noqa: E402

MOD = 'snip'

SEMANTICS = textwrap.dedent('''
    import struct

    def divs(a, b):
        return a // b, a % b

    def run_divs():
        return [divs(-7, 2), divs(7, -2), divs(-7, -2), divs(7, 2), divs(-1, 3), divs(0, -5)]

    def shifts(a, n):
        return a >> n

    def run_shifts():
        return [shifts(-9, 1), shifts(-1, 70), shifts(9, 2), shifts(-256, 4)]

    def run_index():
        b = b"ABCDEFG"
        s = "h\\xe9llo w\\u00f6rld"
        return (b[-1], b[0], b[1:3].decode("latin1"), b[-3:].decode("latin1"), b[5:100].decode("latin1"),
                b[4:2].decode("latin1"), s[-1], s[1:4], s[:-6], s[::2], len(s), b[::-1].decode("latin1"))

    def run_index_error():
        b = b"AB"
        return b[2]

    def run_dict():
        d = {}
        d["z"] = 1
        d["a"] = 2
        d["m"] = 3
        d["a"] = 4
        out = []
        for k in d:
            out.append(k)
        d.pop("z")
        d["z"] = 9
        return (out, list(d.keys()), list(d.values()), len(d), d.get("q", -1))

    def run_struct():
        data = struct.pack("<HhI", 0xBEEF, -2, 0x12345678) + b"\\xff\\xfe"
        a, b, c = struct.unpack_from("<HhI", data, 0)
        d, = struct.unpack_from(">H", data, 8)
        e = struct.unpack_from("<h", data, -2)[0]
        return (a, b, c, d, e, len(data), data.hex())

    def run_from_bytes():
        b = b"\\x01\\x02\\xff\\xff"
        return [int.from_bytes(b[0:2], "little"), int.from_bytes(b[0:2], "big"),
                int.from_bytes(b[2:4], "little", signed=True), int.from_bytes(b[2:4], "little")]

    def run_alias():
        a = [1, 2]
        b = a
        b.append(3)
        c = list(a)
        c.append(4)
        return (a, b, c)

    def segment(i, flags):
        return dict(idx=i + 1, kind="DATA" if flags & 1 else "CODE", flags=flags)

    def run_records():
        segs = [segment(i, f) for i, f in enumerate([0, 1, 0x101])]
        names = [s["kind"] for s in segs if s["flags"] & 0x100 == 0]
        return (segs, names)

    class Shape:
        def __init__(self, w):
            self.w = w

        def area(self):
            return self.w * self.w

        def describe(self):
            return "%s:%d" % (self.name(), self.area())

        def name(self):
            return "square"

    class Rect(Shape):
        def __init__(self, w, h):
            Shape.__init__(self, w)
            self.h = h

        def area(self):
            return self.w * self.h

        def name(self):
            return "rect"

    def run_classes():
        shapes = [Shape(3), Rect(2, 5)]
        return [s.describe() for s in shapes]

    def run_sort():
        words = ["pear", "Fig", "apple", "fig", "Pear", "kiwi"]
        by_len = sorted(words, key=lambda w: len(w))
        return (sorted(words), by_len, sorted(words, key=lambda w: w.lower(), reverse=True))

    def probe(xs, i):
        return xs[i] > 0

    def run_any_all():
        xs = [3, 0, -1]
        return (any(x < 0 for x in xs), all(x > 0 for x in xs), any(probe(xs, i) for i in range(5)))

    def check(n):
        if n < 0:
            raise ValueError("negative: %d" % n)
        return n * 2

    def run_raise():
        out = []
        for n in [1, -1, 2]:
            try:
                out.append(check(n))
            except ValueError:
                out.append(-100)
        return out

    def run_format():
        n = 255
        name = "NE"
        return (f"0x{n:x}", f"{n:08X}", f"{name!r}", "%-5s|%3d|%#x" % (name, 7, 26), str(-3), repr(b"a\\x00'"),
                "a,b,,c".split(","), "-".join(["x", "y"]), "  pad ".strip(), "MiXeD".upper(), "Abc".find("c"))

    def run_counter():
        total = 0
        i = 0
        while i < 100:
            total += i
            i += 3
        return (total, i)
''')

REFUSALS = textwrap.dedent('''
    import re

    def untyped(x):
        return x + 1

    def grows(n):
        x = 1
        while x != n:
            x = x * 2
        return x

    def run_grows():
        return grows(64)

    def gen():
        yield 1

    def dyn(o):
        return getattr(o, "name")

    def run_dyn():
        return dyn("x")

    def regex(s):
        return re.match("a+", s) is not None

    def run_regex():
        return regex("aa")

    def mixed(c):
        v = 1 if c else "one"
        return v

    def run_mixed():
        return mixed(True)

    def set_order():
        out = []
        for x in {3, 1, 2}:
            out.append(x)
        return out

    def calls_refused():
        return len(dyn("y"))
''')


def translate(src, policy='strict'):
    prog = pyfe.program_of_sources({MOD: src})
    prog.int_policy = policy
    prog.solve()
    return pyfe.translate(prog, [MOD], int_policy=policy)[MOD]


def test_semantics_translate():
    r = translate(SEMANTICS)
    runs = [q for q in r.translated if q.rsplit('.', 1)[-1].startswith('run_')]
    refused = dict((q.rsplit('.', 1)[-1], i.reason) for q, i in r.refused.items())
    assert not refused, refused
    assert len(runs) == SEMANTICS.count('def run_')
    doc = l2.to_json(r.unit)                                 # the tree is L2
    assert doc['node'] == 'Unit' and ir.unsupported(r.unit) == []
    h, s, spans = cpp17.write_unit(r.unit)
    assert 'int64_t' in h and 'py::list' in h


def test_refusals():
    r = translate(REFUSALS)
    why = dict((q.rsplit('.', 1)[-1], i.reason) for q, i in r.refused.items())
    assert why['untyped'].startswith('untyped:parameter x')
    assert why['grows'].startswith('int-range')
    assert why['gen'] == 'dynamic:generator'
    assert why['dyn'] == 'dynamic:getattr'
    assert why['regex'] == 'stdlib:re.match'
    assert why['mixed'].startswith('conflict')
    assert why['set_order'] == 'dynamic:set iteration order'
    assert why["calls_refused"].startswith("calls-refused")
    # checked ints: the doubling loop is written with overflow-checked arithmetic instead
    r2 = translate(REFUSALS, 'checked')
    assert 'snip.grows' in r2.translated


def test_int_proofs():
    """what intervals prove is written with C++'s own operators; what they do not, with Python's"""
    r = translate(SEMANTICS)
    h, s, spans = cpp17.write_unit(r.unit)
    body = s[s.index('divs('):]
    assert 'py::floordiv(a, b)' in body and 'py::mod(a, b)' in body       # signs unknown: Python's
    assert 'total += i;' in s and 'i += 3;' in s                         # a bounded counter: plain


def _msvc():
    try:
        from codebreaker.recompile.xenon import msvc_env
        env = dict(msvc_env())
    except Exception:
        return None
    cl = shutil.which('cl', path=env.get('Path') or env.get('PATH'))
    return (env, cl) if cl else None


def test_semantics_compile_and_match():
    tools = _msvc()
    if tools is None:
        pytest.skip('no MSVC')
    env, cl = tools
    r = translate(SEMANTICS)
    h, s, spans = cpp17.write_unit(r.unit)
    work = os.path.join(ROOT, 'state', 'work', 'py2cpp', 'test_frontend')
    if os.path.isdir(work):
        shutil.rmtree(work)
    os.makedirs(work)
    env = dict(env, TEMP=work, TMP=work)
    with open(os.path.join(work, 'snip.hpp'), 'w', encoding='utf-8') as f:
        f.write(h)
    runs = sorted(q.rsplit('.', 1)[-1] for q in r.translated if q.rsplit('.', 1)[-1].startswith('run_'))
    main = ['#include "snip.hpp"', '#include "py_json.hpp"', '#include <cstdio>', 'using namespace snip;',
            'int main() {']
    for name in runs:
        main.append('    std::printf("%%s\\n", py::json::attempt([&](std::string& v) { py::json::put(v, %s()); }).c_str());'
                    % name)
    main += ['    return 0;', '}']
    with open(os.path.join(work, 'snip.cpp'), 'w', encoding='utf-8') as f:
        f.write(s.replace('#include "snip.hpp"', '#include "snip.hpp"') + '\n' + '\n'.join(main) + '\n')
    inc = os.path.join(ROOT, 'native', 'ported', 'include')
    c = subprocess.run([cl, '/nologo', '/std:c++17', '/EHsc', '/utf-8', '/permissive-', '/I' + inc, '/I' + work,
                        '/Fe' + os.path.join(work, 'snip.exe'), 'snip.cpp'], cwd=work, env=env,
                       capture_output=True, text=True, errors='replace')
    assert c.returncode == 0, c.stdout[-3000:]
    out = subprocess.run([os.path.join(work, 'snip.exe')], capture_output=True, text=True, encoding='utf-8')
    got = [json.loads(line) for line in out.stdout.splitlines()]
    space = {}
    exec(compile(SEMANTICS, 'snip', 'exec'), space)
    want = []
    for name in runs:
        try:
            v = space[name]()
            want.append(json.loads(json.dumps(v, default=lambda b: b.decode('latin1'))))
        except Exception as e:
            want.append({'error': type(e).__name__})
    for name, a, b in zip(runs, want, got):
        assert a == b, (name, a, b)
    shutil.rmtree(work, ignore_errors=True)
