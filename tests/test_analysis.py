#!/usr/bin/env python3
"""Tests for the analyses (codebreaker/analyze): the image, the call graph, conventions, globals,
strings, structures, classes, dispatch, resources and the fact table, on three small modules of
the corpus -- notepad.exe (32-bit), lzexpand.dll (16-bit) and vmouse.vxd (a driver).
Run:  python tests/test_analysis.py     (exit code 0 = all passed)

Checks that need the retail corpus are SKIPPED when it is not present. Nothing is run in the
emulator here (that takes minutes); observe.py's own arithmetic is checked on a made-up record."""
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.analyze import (call_graph, classes, conventions, dispatch, facts, globals as global_variables,  # noqa: E402
                                 image, observe, report, resources, strings, structures)
from codebreaker.base import corpus  # noqa: E402

RESULTS = []
_IMAGES = {}


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "PASS", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def _image(name):
    if not os.path.isdir(corpus.root()) or not corpus.retail(name):
        raise Skip("%s is not in the corpus" % name)
    if name not in _IMAGES:
        _IMAGES[name] = image.load(name, work=tempfile.mkdtemp(prefix="cbanalyze"))
    return _IMAGES[name]


# --- the image ---------------------------------------------------------------------------------
def test_image_32():
    img = _image("notepad.exe")
    assert img.kind == "pe" and img.bits == 32 and len(img.functions) > 40
    assert all(img.limits[f] > f for f in img.functions)
    assert any(n.startswith("USER32!") for n in img.imports.values())
    fn = img.function(img.roots[0])
    assert fn.insns and fn.insns[0].addr == fn.start and fn.end <= fn.limit
    assert [i.addr for i in fn.insns] == sorted(i.addr for i in fn.insns)
    covered = sum(i.size for f in img.each() for i in f.insns)
    assert covered > 0.7 * sum(img.limits[f] - f for f in img.functions)      # following the code reaches most of it
    assert img.read(img.spans[0][0], 4) and img.u32(0x10) is None


def test_image_16_and_driver():
    ne = _image("lzexpand.dll")
    assert ne.kind == "ne" and ne.bits == 16 and ne.data_segment and "LZOPENFILE" in ne.exports.values()
    assert any(n.startswith("KERNEL!") and not n.endswith("#") for n in ne.imports.values())
    assert "KERNEL!GLOBALALLOC" in ne.imports.values()                        # ordinals resolved to names
    le = _image("vmouse.vxd")
    assert le.kind == "le" and le.device and le.device["name"].startswith("VMOUSE")
    assert le.discovered and all(le.is_code(f) for f in le.discovered)        # starts the description did not have


def test_arguments_of_a_call():
    img = _image("notepad.exe")
    g = call_graph.build(img)
    found = 0
    for f in g.users.get("USER32!MessageBoxA", ()):
        fn = img.function(f)
        for site, _k, dest in g.calls[f]:
            if img.imports.get(dest) == "USER32!MessageBoxA":
                args = image.arguments(fn, fn.at(site), 4)
                assert 1 <= len(args) <= 4
                found += 1
    assert found


# --- the analyses -------------------------------------------------------------------------------
def test_call_graph():
    img = _image("notepad.exe")
    g = call_graph.build(img)
    assert set(g.calls) == set(img.functions) and set(g.order) == set(img.functions)
    pos = {f: n for n, f in enumerate(g.order)}
    cyclic = {f for c in g.cycles for f in c}
    for f in img.functions:                                                   # callees come before their callers
        for _s, kind, dest in g.calls[f]:
            if kind == "call" and dest not in cyclic and f not in cyclic:
                assert pos[dest] < pos[f], (hex(dest), hex(f))
    assert g.users["USER32!SendMessageA"] and len(g.reachable) > 0.8 * len(img.functions)
    assert all(f in g.callers.get(d, ()) for f in img.functions for _s, k, d in g.calls[f] if k == "call")
    c = call_graph.analyse(img)["counts"]
    assert c["edges"] > 50 and c["import_call_sites"] > 100


def test_conventions():
    img = _image("notepad.exe")
    facts32 = conventions.functions(img)
    stdcall = [f for f in facts32.values() if f.get("convention") == "stdcall"]
    assert len(stdcall) > 20 and all(f["args"] == f["removes"] // 4 and f["arg_source"] == "return" for f in stdcall)
    assert any(f.get("shape") == "import thunk" and "!" in f["thunk_of"] for f in facts32.values())
    procs = [f for f in facts32.values() if f.get("procedure") == "window procedure"]
    assert procs and all(f["removes"] == 16 for f in procs)
    ne = _image("lzexpand.dll")
    facts16 = conventions.functions(ne)
    by_name = {f["name"]: f for f in facts16.values()}
    assert by_name["LZOPENFILE"]["convention"] == "pascal far" and by_name["LZOPENFILE"]["removes"] == 10
    assert by_name["LZCLOSE"]["args"] == 1


def test_globals_and_strings():
    img = _image("notepad.exe")
    var = global_variables.variables(img)
    assert len(var) > 60
    assert any(g["type"].startswith("result of ") for g in var.values())
    assert any(g["initial"] == "uninitialised" for g in var.values())
    assert all(g["size"] in (1, 2, 4, 8, 10) for g in var.values())
    tab = strings.table(img)
    assert tab and all(len(t) >= 2 for t, _k, _r in tab.values())
    fmts, sites, _own = strings.formats(img)
    assert sites >= 1 and any(f["format"] == "%d" and f["arguments"] == ["int"] for f in fmts)
    assert strings.conversions("%s = %ld, %5.2f %c%%") == ["char *", "long", "double", "char"]
    assert strings.conversions("%*d %S", wide=False) == ["int", "int", "wchar_t *"]
    assert strings._assemble({("ebp", -8 + k): (b, k // 4) for k, b in enumerate(b"Hello\0")}) == [("ebp", -8, "Hello", False)]


def test_structures():
    img = _image("lzexpand.dll")
    found = structures.recover(img)
    assert found and all(len(s["fields"]) >= 2 or s["exact"] for s in found)
    big = found[0]
    assert len(big["fields"]) >= 10 and big["size"] >= max(big["fields"])
    text = structures.layout(img, big)
    assert text.startswith("struct struct_1 {") and text.rstrip().endswith("};")
    assert structures.analyse(img)["counts"]["fields"] == sum(len(s["fields"]) for s in found)


def test_dispatch_and_resources():
    img = _image("notepad.exe")
    msgs = dispatch.messages(img)
    names = {dispatch.MESSAGES.get(v) for m in msgs for v in m["messages"]}
    assert {"WM_COMMAND", "WM_INITDIALOG", "WM_CLOSE"} <= names
    sw = dispatch.switches(img)
    assert any(s["table"] for s in sw) and all(s["cases"] for s in sw)
    loads, _controls = resources.uses(img)
    assert any(u["type"] == "STRING" and u["present"] for u in loads)
    assert not any(u["present"] is False for u in loads)                      # notepad loads only what it carries
    le = _image("vmouse.vxd")
    ctl = dispatch.control_messages(le)
    assert ctl and "Device_Init" in ctl["handles"]


def test_classes_none_in_c():
    img = _image("notepad.exe")
    c = classes.analyse(img)["counts"]
    assert c["virtual_tables"] == 0 and c["message_maps"] == 0 and c["cxx_exception_tables"] == 0
    assert classes._decorated(".?AVCWnd@@") == "CWnd" and classes._decorated(".?AVInner@Outer@@") == "Outer::Inner"


def test_observation_arithmetic():
    out = {"fields": {}, "nested": {}, "indirect": [], "passes": [], "exit": None, "removed": None, "returns": None}
    rec = {"exit": "ret", "esp_delta": 12, "ret": observe.BUFFERS + 0x10000,
           "mem_reads": [{"addr": observe.BUFFERS + 8, "size": 4, "value": 0},
                         {"addr": observe.MARK + (1 << 24) + (0x10 << 12) + 4, "size": 2, "value": 0}],
           "mem_writes": [{"addr": observe.BUFFERS + 8, "size": 4, "value": 1}],
           "calls": [{"pc": 0x401000, "indirect": True, "resolved_target": 0x402000, "external": False}],
           "api_calls": [("USER32.DLL!SetWindowTextA", [5, observe.BUFFERS + 0x20])]}
    observe._digest(rec, out)
    assert out["fields"] == {0: {8: [4, 1, 1]}} and out["nested"] == {(1, 0x10): {4: [2, 1, 0]}}
    assert out["removed"] == 8 and out["returns"] == ("argument", 1) and out["indirect"] == [(0x401000, 0x402000)]
    assert out["passes"] == [("USER32.DLL!SetWindowTextA", 1, 0, 0x20, "address")]
    assert observe._after({"mem_writes": [{"addr": 0x1002, "size": 2, "value": 0x4241}]}, 0x1000, b"....") == b"..AB"


def test_report_and_fact_table():
    _image("lzexpand.dll")
    os.environ["CB_FUNCTIONS_DB"] = os.path.join(tempfile.mkdtemp(prefix="cbfacts"), "functions.sqlite")
    try:
        r = report.run("lzexpand.dll", ("call_graph", "conventions", "dispatch"))
        assert set(r["counts"]) == {"call_graph", "conventions", "dispatch"} and "failed" not in str(r["counts"])
        text = report.text(r)
        assert "== call graph" in text and "16-bit Windows module" in text
        fs = facts.open()
        tally = fs.tally(r["sha"])
        assert tally["conventions"]["convention"] == r["functions"]
        img = r["image"]
        va = next(v for v, n in img.exports.items() if n == "LZOPENFILE")
        rows = fs.get(r["sha"], analysis="conventions", addr=facts.key(img, va))
        assert rows and rows[0]["value"]["convention"] == "pascal far"
        report.run("lzexpand.dll", ("conventions",))                         # storing again replaces
        assert fs.tally(r["sha"])["conventions"]["convention"] == r["functions"]
        fs.close()
        try:
            report.run("lzexpand.dll", ("no_such_analysis",))
            raise AssertionError("an unknown analysis was accepted")
        except ValueError:
            pass
    finally:
        del os.environ["CB_FUNCTIONS_DB"]


def main():
    for fn in (test_image_32, test_image_16_and_driver, test_arguments_of_a_call, test_call_graph, test_conventions,
               test_globals_and_strings, test_structures, test_dispatch_and_resources, test_classes_none_in_c,
               test_observation_arithmetic, test_report_and_fact_table):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
