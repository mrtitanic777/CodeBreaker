#!/usr/bin/env python3
"""Tests for building a module whole: its resources as source and back, its exports and imports,
its data as C, and the linked image checked against the original.
Run:  python tests/test_whole_module.py     (exit code 0 = all passed)

Everything here needs the corpus; the builds also need OpenWatcom and the native decompiler.
Checks are SKIPPED when what they need is not present."""
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.base import corpus                          # noqa: E402
from codebreaker.build import data, definitions, module, watcom    # noqa: E402
from codebreaker.build import resources as rebuild           # noqa: E402
from codebreaker.formats import resources                    # noqa: E402

RESULTS = []


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


def _module(name):
    path = corpus.retail(name) if os.path.isdir(corpus.root()) else None
    if not path:
        raise Skip("%s is not in the corpus" % name)
    return path


def _need_compiler():
    if not (watcom.ow_tool("wrc") and watcom.ow_tool("wlink") and watcom.ow_tool("wcc")):
        raise Skip("OpenWatcom is not set up")


def _need_decompiler():
    from codebreaker.decompile import decompiler
    if not decompiler.available():
        raise Skip("the native decompiler is not built")


# --- reading and decoding resources ----------------------------------------------------------
def test_resources_of_a_32_bit_module():
    found = resources.read(_module("version.dll"))
    assert [(r.type, r.name, r.language) for r in found] == [(resources.VERSION, 1, 0x409)], found
    fixed, table, translations = resources.version(found[0].data, True)
    assert fixed[0] == 0xFEEF04BD and translations == [0x409, 1252]
    assert dict(table["040904E4"])["InternalName"] == "VERSION"


def test_resources_of_a_16_bit_module():
    found = resources.read(_module("commdlg.dll"))
    kinds = {resources.type_name(r.type) for r in found}
    assert {"DIALOG", "STRING", "GROUP_ICON", "ICON", "BITMAP", "VERSION"} <= kinds, kinds
    assert all(r.language is None for r in found)
    dialogs = [resources.dialog(r.data, False) for r in found if r.type == resources.DIALOG]
    assert dialogs and all(d["controls"] for d in dialogs)
    assert any(c["class"] == 0x80 and c["text"] == "OK" for d in dialogs for c in d["controls"])
    texts = [t for r in found if r.type == resources.STRING for t in resources.strings(r.data, False)]
    assert any(t for t in texts)
    group = next(r for r in found if r.type == resources.GROUP_ICON)
    members = {r.name: r.data for r in found if r.type == resources.ICON}
    ico = resources.icon_file(group.data, members.__getitem__)
    assert ico[:4] == b"\0\0\1\0" and len(ico) > 22
    bmp = resources.bitmap_file(next(r.data for r in found if r.type == resources.BITMAP))
    assert bmp[:2] == b"BM" and int.from_bytes(bmp[2:6], "little") == len(bmp)


def test_type_library():
    found = [r for r in resources.read(_module("stdole2.tlb")) if r.type == "TYPELIB"]
    library = resources.type_library(found[0].data)
    assert library["format"] == "MSFT" and library["name"] == "stdole", library
    assert ("interface", "IUnknown", "00000000-0000-0000-C000-000000000046") in library["types"]
    assert "interface IDispatch;" in resources.type_library_outline(library)
    assert resources.type_library(b"not a type library") is None


def test_padding_is_not_a_difference():
    assert resources.same_data(b"abc", b"abc\0\0") and not resources.same_data(b"abc", b"abd\0")


# --- resources as source and back ------------------------------------------------------------
def test_string_literals():
    assert rebuild.quoted('a"b\\c\n', 0) == '"a""b\\\\c\\n"'
    assert rebuild.quoted("caf\xe9", rebuild.WIDE) == '"caf\\351"'
    for text, mode in (("x\0y", 0), ("ا", rebuild.UNICODE)):
        try:
            rebuild.quoted(text, mode, counted=True)
            raise AssertionError("%r has no source form" % text)
        except rebuild.NoSourceForm:
            pass


def _round_trip(name, text_at_least):
    _need_compiler()
    r = rebuild.rebuild(_module(name), tempfile.mkdtemp())
    assert r.same, r.line()
    assert r.identical + r.equivalent == r.total and not r.extra, r.line()
    assert r.as_text >= text_at_least, r.line()
    assert os.path.isfile(r.rc) and os.path.isfile(r.res) and os.path.isfile(r.link_res)
    return r


def test_resource_round_trip_32_bit():
    r = _round_trip("version.dll", 1)
    assert "VERSIONINFO" in open(r.rc).read() and r.fallback == 0


def test_resource_round_trip_16_bit():
    r = _round_trip("commdlg.dll", 20)
    text = open(r.rc).read()
    assert "DIALOG" in text and "STRINGTABLE" in text and " ICON " in text
    assert r.fallback == 1, r.line()                     # the version block: see build/resources.py


# --- exports and imports ---------------------------------------------------------------------
def test_definitions_32_bit():
    path = _module("version.dll")
    found = definitions.exports(path)
    assert [e.ordinal for e in found] == list(range(1, 15))
    assert {e.name for e in found if e.kind == "data"} == {"VerFThk_ThunkData32", "VerThkSL_ThunkData32"}
    assert ("KERNEL32", "LocalAlloc") in definitions.imports(path) and ("KERNEL32", 17) in definitions.imports(path)
    assert definitions.is_library(path) and definitions.module_name(path) == "VERSION"
    text = definitions.definition_file(path)
    assert "LIBRARY VERSION" in text and "GetFileVersionInfoA" in text and "@1" in text


def test_definitions_16_bit():
    path = _module("lzexpand.dll")
    found = definitions.exports(path)
    assert len(found) == 12, found                       # the entry table's 18 other entries are not exports
    assert {e.name: e.ordinal for e in found}["LZOPENFILE"] == 2
    assert [e.name for e in found if e.resident] == ["WEP", "___EXPORTEDSTUB"]
    assert ("KERNEL", 81) in definitions.imports(path)
    assert definitions.is_library(path) and definitions.description(path) == "Windows file expansion library"
    lines, missing = definitions.export_directives(path, lambda e: "_" + e.name)
    assert "export WEP.11='_WEP' resident" in lines and not missing


# --- data as C -------------------------------------------------------------------------------
def test_data_objects():
    d = data.data16(_module("lzexpand.dll"), lambda where: None)
    assert d.size == 644 and "codebreaker_data" in d.source and not d.unresolved
    d = data.data32(_module("version.dll"), lambda va: None, {})
    assert d.movable and d.parts and d.place(d.parts[0][1] + 4).endswith("+ 0x4)")
    header, lost = data.relocate_header("#define uRam%08x (*(uint4 *)0x%08x)\n" % (d.parts[0][1], d.parts[0][1]), d)
    assert "&codebreaker_data_" in header and not lost
    text, lost = data.relocate_text("int f()\n{\n  return 0x%x + 0x10;\n}\n" % d.parts[0][1], {d.parts[0][1]}, d,
                                    lambda va: None)
    assert "codebreaker_data_" in text and "0x10;" in text and not lost


# --- the whole build -------------------------------------------------------------------------
def _build(name):
    _need_compiler()
    _need_decompiler()
    b = module.build(_module(name), outdir=tempfile.mkdtemp(), log=lambda *a: None)
    assert b.linked, b.error
    assert b.format_same and not b.imports_extra and not b.undefined, b.report()
    assert b.exports_same == b.exports_total and not b.exports_missing, b.report()
    assert b.resources_same, b.report()
    assert not b.data_error and b.data.size, b.report()
    assert b.ran and sum(1 for _e, mine, _o, _s in b.ran if mine == "ret") >= len(b.ran) // 2, b.ran
    assert "linked" in b.report()
    return b


def test_build_32_bit_module():
    b = _build("version.dll")
    assert b.exports_total == 14 and b.with_text == b.functions


def test_build_16_bit_module():
    b = _build("lzexpand.dll")
    assert b.exports_total == 12
    from codebreaker.formats.ne import NEFile
    nef = NEFile(b.image)
    assert nef.h["flags"] & 0x8000 and nef.module_name() == "LZEXPAND"
    assert nef.segments()[nef.h["autodata_seg"] - 1]["kind"] == "DATA"


def main():
    for fn in (test_resources_of_a_32_bit_module, test_resources_of_a_16_bit_module, test_type_library,
               test_padding_is_not_a_difference,
               test_string_literals, test_resource_round_trip_32_bit, test_resource_round_trip_16_bit,
               test_definitions_32_bit, test_definitions_16_bit, test_data_objects,
               test_build_32_bit_module, test_build_16_bit_module):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
