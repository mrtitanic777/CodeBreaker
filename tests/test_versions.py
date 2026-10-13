#!/usr/bin/env python3
"""Tests for more than one product: the plural corpus, what Windows 1.x executables need of the
16-bit reader, raster fonts, and matching the functions of two builds of a module.
Run:  python tests/test_versions.py     (exit code 0 = all passed)

Checks that need a product's binaries are SKIPPED when that product is not on this machine."""
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.base import corpus, settings          # noqa: E402
from codebreaker.compare import versions               # noqa: E402
from codebreaker.formats import fonts, ne              # noqa: E402

RESULTS = []


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


def _need(*names):
    for name in names:
        if name not in corpus.names() or not os.path.isdir(corpus.root(name)):
            raise Skip("no %s binaries here" % name)


# --- the plural corpus -------------------------------------------------------------------------
def test_default_product_is_untouched():
    default = settings.get("target")
    assert corpus.current() == default and default in corpus.names()
    before = corpus.root()
    assert corpus.root(default) == before
    assert settings.corpora()[default] == settings.get("corpus")
    with corpus.selected(default):
        assert corpus.root() == before
    assert corpus.current() == default and corpus.ENV not in os.environ


def test_selecting_a_product():
    _need("win3.1")
    default, home = corpus.current(), corpus.root()
    k = corpus.retail("kernel32.dll")
    with corpus.selected("win3.1"):
        assert corpus.current() == "win3.1" and os.environ[corpus.ENV] == "win3.1"   # workers inherit it
        assert corpus.root() == settings.corpora()["win3.1"] != home
        u = corpus.retail("user.exe")
        assert u and corpus.owner(u) == "win3.1" and corpus.lane(u) == "ne"
        assert corpus.retail("kernel32.dll") is None                 # 3.1 has no such module
        assert "products" in corpus.design_dir()                     # nothing is written beside its binaries
    assert corpus.current() == default and corpus.root() == home and corpus.retail("kernel32.dll") == k
    assert corpus.take_option(["a", "--corpus", "win3.1", "b"]) == ["a", "b"] and corpus.current() == "win3.1"
    corpus.use(None)
    assert corpus.current() == default
    try:
        corpus.use("no-such-product")
        raise AssertionError("an unknown product was accepted")
    except ValueError:
        pass


# --- Windows 1.x ---------------------------------------------------------------------------------
def test_boot_image_modules():
    _need("win1.0")
    image = os.path.join(corpus.root("win1.0"), "WIN100.BIN")
    if not os.path.isfile(image):
        raise Skip("no boot image")
    mods = ne.open_boot_image(image)
    assert {"KERNEL", "GDI", "USER", "SYSTEM", "KEYBOARD", "MOUSE", "DISPLAY", "SOUND", "COMM"} <= set(mods)
    names = dict((o, n) for n, o in mods["KERNEL"].nonresident_names()[1:])
    assert names[19] == "GLOBALUNLOCK" and len(names) == 100       # the names are in the overlay
    gdi = mods["GDI"]
    where = {bool(s["flags"] & ne.PRELOAD) for s in gdi.segments()}
    assert where == {True, False}                                   # spread over both files
    for s in gdi.segments():                                        # every code segment starts as code does
        if s["kind"] == "CODE" and s["length"] > 16:
            assert s["file_off"] + s["length"] <= len(gdi.data)
    # written out as an ordinary file, it reads back the same
    tmp = os.path.join(tempfile.mkdtemp(), "gdi.exe")
    with open(tmp, "wb") as f:
        f.write(ne.standalone(gdi))
    again = ne.NEFile(tmp)
    assert again.entries() == gdi.entries() and again.nonresident_names() == gdi.nonresident_names()
    for x, y in zip(gdi.segments(), again.segments()):
        assert gdi.data[x["file_off"]:x["file_off"] + x["length"]] == again.data[y["file_off"]:y["file_off"] + y["length"]]
        assert gdi.relocations(x) == again.relocations(y)
    with corpus.selected("win1.0"):                                 # and the corpus lists it as a module
        p = corpus.retail("gdi.exe")
        assert p and corpus.lane(p) == "ne" and any(os.path.basename(m) == "gdi.exe" for m in corpus.modules("ne"))


def test_fonts():
    _need("win1.0", "win3.1")
    with corpus.selected("win1.0"):
        old = fonts.fonts_in(corpus.retail("helva.fon"))
        stroke = fonts.fonts_in(corpus.retail("roman.fon"))
    assert old and old[0].version == 0x0100 and old[0].name == "Helv" and old[0].kind == "proportional"
    rows = old[0].glyph("W")
    assert len(rows) == old[0].height and any("#" in r for r in rows) and len(rows[0]) == old[0].widths[ord("W")]
    assert stroke and stroke[0].vector
    with corpus.selected("win3.1"):
        new = fonts.fonts_in(corpus.retail("sserife.fon"))
    assert new and new[0].version == 0x0200 and new[0].name == "MS Sans Serif"
    assert any("#" in r for r in new[0].glyph("W")) and not any("#" in r for r in new[0].glyph(" "))


# --- two builds of a module ----------------------------------------------------------------------
def test_a_module_against_itself():
    path = corpus.retail("version.dll")
    if not path:
        raise Skip("no version.dll")
    c = versions.compare(path, path)
    n = c.counts()
    assert n["identical"] == len(c.a.order) > 20 and not (n["changed"] or n["added"] or n["removed"])
    assert all(p["from"] == p["to"] for p in c.pairs)               # each function found itself


def test_function_spans_are_the_carved_functions():
    from codebreaker.formats import pe_functions
    path = corpus.retail("version.dll")
    if not path:
        raise Skip("no version.dll")
    carved = pe_functions.PeCarve(path).funcs
    assert pe_functions.function_spans(path) == {va: (f["end"], f["name"]) for va, f in carved.items()}


def test_two_products_32bit():
    _need("win98")
    old = corpus.retail("version.dll")
    with corpus.selected("win98"):
        new = corpus.retail("version.dll")
    if not old or not new:
        raise Skip("no version.dll")
    c = versions.compare(old, new)
    n = c.counts()
    assert n["identical"] >= 30 and n["identical"] + n["changed"] + n["added"] == len(c.b.order)
    by_name = {p["from_name"]: p for p in c.pairs}
    assert by_name["GetFileVersionInfoA"]["to_name"] == "GetFileVersionInfoA"
    assert set(c.a.functions) >= {p["from"] for p in c.pairs}        # keyed as the store keys them
    data = c.as_json()
    assert data["from"]["corpus"] == settings.get("target") and data["to"]["corpus"] == "win98"
    changed = [p for p in data["pairs"] if p["status"] == "changed"]
    assert all(p["from"] in data["listings"]["from"] and p["ops"] for p in changed)
    assert data["code"]["identical"] + data["code"]["changed"] + data["code"]["added"] == data["code"]["to_bytes"]


def test_two_products_16bit():
    _need("win3.1")
    with corpus.selected("win3.1"):
        old = corpus.retail("lzexpand.dll")
    new = next((p for p in corpus.candidates("lzexpand.dll") if os.path.getsize(p) > 20000), None)
    if not old or not new:
        raise Skip("no lzexpand.dll in both")
    c = versions.compare(old, new)
    names = {p["from_name"]: p["to_name"] for p in c.pairs if c.a.functions[p["from"]].named}
    assert names.get("LZOPENFILE") == "LZOPENFILE" and len(c.pairs) >= 20
    assert all(":" in k for k in c.a.order)                          # segment:offset


def main():
    for fn in (test_default_product_is_untouched, test_selecting_a_product, test_boot_image_modules,
               test_fonts, test_a_module_against_itself, test_function_spans_are_the_carved_functions,
               test_two_products_32bit, test_two_products_16bit):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "ok"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
