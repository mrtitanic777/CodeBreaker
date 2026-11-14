#!/usr/bin/env python3
"""Regression tests for bugs the original Python carried (found while it was ported to typed C++, and fixed
in the Python and in the port together).

    python tests/test_port_fixes.py
"""
import os
import sys
import types
import warnings

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
warnings.simplefilter("ignore")

RESULTS = []


class Skip(Exception):
    """a case that cannot run in this environment, reported as skipped with its reason (never counted as passed)"""


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "ok", ""))
    except Skip as e:
        RESULTS.append((fn.__name__, "skip", str(e)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def test_java_rules_findings_of_one_line_come_in_name_order():
    """java.rules: where one constructor or line matches several names of a set (virtual-call-in-ctor,
    map-read-insert, leak-drop-no-delete) the findings come in the sorted order of the names, not in the
    set's hash order (which changed from run to run)."""
    from codebreaker.java import rules
    plat = types.SimpleNamespace(override_pairs=None)
    header = "class Foo {\npublic:\n\tFoo();\n\tvirtual void zeta();\n\tvirtual void alpha();\n\tvirtual void mid();\n};\n"
    got = [f[4].split("calls virtual ")[1].split("()")[0] for f in rules.rule_virtual_call_in_ctor(
        "Foo.cpp", "Foo::Foo()\n{\n\tzeta();\n\talpha();\n\tmid();\n}\n", header, plat)]
    assert got == ["alpha", "mid", "zeta"], got
    maps = "std::map<int,int> zz;\nstd::map<int,int> mm;\nstd::map<int,int> aa;\nint f() {\n\treturn zz[1] + aa[2] + mm[3];\n}\n"
    got = [f[4].split("[")[0] for f in rules.rule_map_read_insert("M.cpp", maps, None, plat)]
    assert got == ["aa"], got                                  # (one finding a line: the first by name)
    owners = "std::vector<Foo*> zs;\nstd::vector<Foo*> as;\nstd::vector<Foo*> ms;\nvoid f() {\n\tzs.clear(); as.clear(); ms.pop_back();\n}\n"
    got = [f[4].split(".")[0] for f in rules.rule_leak_drop_no_delete("L.cpp", owners, None, plat)]
    assert got == ["as"], got


def test_segdiff_compile_group_leaves_no_work_folder():
    """source.ne_asm.segdiff.compile_group: the DOSBox work folder msvc152.compile_c leaves to its caller is
    removed (one folder per compile was left in the temporary folder). Skipped where MSVC 1.52c / DOSBox-X are
    not set up."""
    import tempfile
    from codebreaker.build import msvc152
    from codebreaker.source.ne_asm import segdiff
    if not os.path.exists(msvc152.DBX) or not os.path.exists(os.path.join(msvc152.MSVC, "BIN", "CL.EXE")):
        return
    tmp = tempfile.mkdtemp(prefix="cb_segleak_")
    old = tempfile.tempdir
    tempfile.tempdir = tmp
    try:
        found = segdiff.compile_group("int add2(int a, int b) { return a + b; }\n")
        assert "_add2" in found, sorted(found)
        assert os.listdir(tmp) == [], os.listdir(tmp)
    finally:
        tempfile.tempdir = old


def test_origin_notes_calls_to_compiler_helpers():
    """analyze.origin: a call to one of the C compiler's helpers (named: __aFlmul, __aNchkstk ...) is noted in the
    function's compiler notes, near or far (the lookup was `imported.get(<address>)` in a dict keyed by import
    symbols, so it never found anything)."""
    import struct
    from codebreaker import platforms
    from codebreaker.analyze import origin
    from codebreaker.decompile import describe
    md = platforms.decoder(16, detail=True)
    base = describe.address(1, 0)
    code = bytearray(b"\x55\x8b\xec")                                         # push bp / mov bp, sp
    code += b"\xe8" + struct.pack("<h", 0x20 - (len(code) + 3))                # call near 0x20
    code += b"\x9a" + struct.pack("<HH", 0x30, base >> 16)                     # call far selector:0x30
    code += b"\x9a" + struct.pack("<HH", 0x40, base >> 16)                     # call far selector:0x40 (not a helper)
    code += b"\x5d\xcb"                                                       # pop bp / retf
    code += bytes(0x20 - len(code)) + b"\xcb" + bytes(0x0F) + b"\xcb" + bytes(0x20)
    names = {base: "caller", base + 0x20: "__aNlmul", base + 0x30: "__aFuldiv", base + 0x40: "other"}
    o = origin.Origin(base, "caller")
    origin._judge(o, md, bytes(code), 0, 0x20, names, {0, 0x20, 0x30, 0x40}, False, base)
    assert o.compiler == ["calls __aNlmul", "calls __aFuldiv"], o.compiler


def test_native_dosexec_loads_the_dll_once_from_two_threads():
    """exact.emu.native_dosexec: the first use of the native executive from two threads loads (and initialises)
    the DLL once; _load() once ran its whole body in both, leaving two entries in _LIB."""
    import threading
    import time
    from codebreaker.exact.emu import native_dosexec as nd
    saved = list(nd._LIB), list(nd._WHY)
    calls = []

    class Fn:                                             # a DLL entry point: callable, counts its calls
        def __init__(self, value):
            self.value, self.call_count = value, 0

        def __call__(self, *args):
            self.call_count += 1
            return self.value

    class Lib:                                            # the loaded DLL: any other entry point answers 0
        def __init__(self):
            self.cbd_version, self.cbd_init = Fn(1), Fn(0)

        def __getattr__(self, name):
            fn = Fn(0)
            setattr(self, name, fn)
            return fn

    lib = Lib()

    def slow_load(path):
        calls.append(path)
        time.sleep(0.3)                                   # (the other thread comes in while this one is loading)
        return lib

    nd._LIB.clear()
    nd._WHY.clear()
    got = []
    arrived, ready = [0], threading.Condition()

    class Gate:                                           # both threads start their first use together
        def wait(self):
            with ready:
                arrived[0] += 1
                ready.notify_all()
                ready.wait_for(lambda: arrived[0] >= 2, timeout=10)

    gate = Gate()

    def first_use():
        gate.wait()
        got.append(nd._load())

    real_path, real_load = nd.dll_path, nd.settings.load_native
    try:
        nd.dll_path = lambda *a, **k: __file__
        nd.settings.load_native = slow_load
        try:
            threads = [threading.Thread(target=first_use) for _ in range(2)]
            for t in threads:
                t.start()
            for t in threads:
                t.join()
        finally:
            nd.dll_path, nd.settings.load_native = real_path, real_load
        assert len(calls) == 1 and lib.cbd_init.call_count == 1, (len(calls), lib.cbd_init.call_count)
        assert got == [lib, lib] and len(nd._LIB) == 1, (got, nd._LIB, nd._WHY)
    finally:
        nd._LIB[:] = saved[0]
        nd._WHY[:] = saved[1]


def main():
    for fn in (test_java_rules_findings_of_one_line_come_in_name_order,
               test_segdiff_compile_group_leaves_no_work_folder,
               test_origin_notes_calls_to_compiler_helpers,
               test_native_dosexec_loads_the_dll_once_from_two_threads):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for r in RESULTS if r[1] == "FAIL")
    print("%d passed, %d skipped, %d failed" % (sum(1 for r in RESULTS if r[1] == "ok"),
                                              sum(1 for r in RESULTS if r[1] == "skip"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
