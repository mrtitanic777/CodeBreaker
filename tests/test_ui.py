#!/usr/bin/env python3
"""Tests for what shows the work: the inspector, the coverage ledger, the progress reading, the
query layer and the terminal interface. Run:  python tests/test_ui.py     (exit code 0 = all passed)

They use three small corpus modules (version.dll, lzexpand.dll, vkd.vxd) and a function store of
their own in a temporary folder -- the real store is never opened. Checks that need the corpus
are SKIPPED when it is not present."""
import csv
import io
import json
import os
import sqlite3
import sys
import tempfile
from contextlib import redirect_stdout

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)
TEMP = tempfile.mkdtemp(prefix="cbui")
os.environ["CB_FUNCTIONS_DB"] = os.path.join(TEMP, "functions.sqlite")      # before anything opens a store

from codebreaker.base import corpus                          # noqa: E402
from codebreaker.coverage import ledger                      # noqa: E402
from codebreaker.formats import inspect                      # noqa: E402
from codebreaker.store import functions as F                 # noqa: E402
from codebreaker.ui import progress, service                 # noqa: E402

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
    if not os.path.isdir(corpus.root()):
        raise Skip("retail corpus not present")
    path = corpus.retail(name)
    if not path:
        raise Skip("%s is not in the corpus" % name)
    return path


# --- inspector -------------------------------------------------------------------------------
def test_describe_32bit():
    info = inspect.describe(_module("version.dll"))
    assert info["ok"] and info["format"] == "PE" and not info["notes"], info["notes"]
    s = info["summary"]
    assert s["type"] == "DLL" and s["machine"] == "i386" and s["module"].lower() == "version.dll"
    assert s["linked"].startswith("199"), s["linked"]              # a real link date, not a file pointer
    names = {x["name"]: x for x in info["exports"]["symbols"]}
    assert info["exports"]["count"] == len(names) and "GetFileVersionInfoA" in names
    first = names["GetFileVersionInfoA"]
    assert first["hint"] >= 0 and first["forward"] == "" and first["address"] > s["image_base"]
    assert any(m["module"].upper().startswith("KERNEL32") and m["count"] and not m["delay"] for m in info["imports"])
    assert all(x["name"] or x["ordinal"] >= 0 for m in info["imports"] for x in m["symbols"])
    assert [x["name"] for x in info["sections"]][0] == ".text" and info["sections"][0]["code"]
    assert s["code_bytes"] == sum(x["file_size"] for x in info["sections"] if x["code"])
    assert len(info["directories"]) == 16 and info["relocations"]["blocks"] > 0
    version = [t for t in info["resources"]["types"] if t["type"] == "VERSION"]
    assert version and version[0]["items"][0]["preview"] == "text"
    json.dumps(info)                                               # plain data all the way down


def test_describe_16bit_driver_and_dos():
    info = inspect.describe(_module("lzexpand.dll"))
    assert info["ok"] and info["format"] == "NE" and info["summary"]["module"] == "LZEXPAND"
    assert info["summary"]["type"] == "DLL" and info["summary"]["description"]
    assert any(x["name"] == "LZOPENFILE" and x["ordinal"] > 0 and ":" in x["address"] for x in info["exports"]["symbols"])
    assert [m["module"] for m in info["imports"]][0] == "KERNEL" and info["imports"][0]["count"] > 0
    assert sum(1 for x in info["sections"] if x["code"]) >= 1 and info["sections"][-1]["flags"]
    drv = inspect.describe(_module("vkd.vxd"))
    assert drv["ok"] and drv["format"] == "LE" and drv["summary"]["module"] == "VKD" and drv["driver"]["name"] == "VKD"
    assert drv["summary"]["code_bytes"] > 0 and all("READ" in x["flags"] for x in drv["sections"])
    dos = inspect.describe(_module("xcopy.exe"))
    assert dos["ok"] and dos["format"] == "MZ" and dos["summary"]["type"] == "DOS program"
    assert dos["headers"]["header_size"] + dos["headers"]["load_size"] <= dos["size"]
    json.dumps([drv, dos])


def test_describe_never_raises():
    whole = open(_module("version.dll"), "rb").read()
    cases = {"text.dll": b"not an executable", "empty.dll": b"", "cut.dll": whole[:0x180], "cut2.dll": whole[:0x1300],
             "scrambled.dll": whole[:0x200] + bytes(reversed(whole[0x200:]))}
    for name, data in cases.items():
        path = os.path.join(TEMP, name)
        with open(path, "wb") as f:
            f.write(data)
        info = inspect.describe(path)
        json.dumps(info)
        inspect.render(info)
        assert isinstance(info["sections"], list) and "symbols" in info["exports"], name
    assert inspect.describe(os.path.join(TEMP, "text.dll"))["format"] is None
    assert not inspect.describe(os.path.join(TEMP, "no such file"))["ok"]


def test_strings_bytes_and_resources():
    path = _module("version.dll")
    rows = inspect.strings(path)
    assert any(r["text"] == "KERNEL32.dll" and r["category"] == "module" and r["address"] for r in rows), len(rows)
    assert any(r["kind"] == "utf16" for r in rows) and rows == sorted(rows, key=lambda r: r["offset"])
    dump = inspect.hexdump(path, 0, 32).splitlines()
    assert len(dump) == 2 and dump[0].startswith("00000000  4D 5A") and dump[1].startswith("00000010")
    item = [t for t in inspect.describe(path)["resources"]["types"] if t["type"] == "VERSION"][0]["items"][0]
    body = inspect.resource_body(path, item["offset"], item["size"], item["type_id"], item["wide"])
    assert body["kind"] == "text" and "FileVersion" in body["text"] and "Microsoft" in body["text"], body
    assert inspect.resource_body(path, 0, 64, 99)["kind"] == "hex"
    assert inspect.resource_body(path, 0, 2, 2)["kind"] == "hex"       # too short to be a picture: shown as bytes


def test_info_command():
    path = _module("version.dll")
    for argv, want in (([path], "exports"), ([path, "--section", "exports"], "GetFileVersionInfoA"),
                       ([path, "--section", "hex", "--length", "16"], "4D 5A"),
                       ([path, "--section", "strings", "--min", "8"], "KERNEL32.dll"), ([path, "--json"], '"summary"')):
        out = io.StringIO()
        with redirect_stdout(out):
            code = inspect.main(argv)
        assert code == 0 and want in out.getvalue(), (argv[1:], out.getvalue()[:200])
    with redirect_stdout(io.StringIO()):
        assert inspect.main(["no_such_module_zz.dll"]) == 2 and inspect.main([path, "--section", "nonsense"]) == 2


# --- coverage ledger -------------------------------------------------------------------------
def _fill_store():
    """A store of our own holding version.dll with one function of each outcome."""
    path = _module("version.dll")
    digest = ledger.profiles()["pe"]
    if not digest:
        raise Skip("the verifier settings cannot be worked out here")
    sha = corpus.sha(path)
    fs = F.open()
    if not fs.addresses(sha):
        addrs = sorted("%08x" % x["address"] for x in inspect.describe(path)["exports"]["symbols"][:6])
        outcome = [("equivalent", "coverage=90"), ("equivalent", "coverage=20,low-coverage"), ("divergent", ""),
                   ("inconclusive", ""), ("no-compile", "")]
        fs.add_module(sha, "version.dll", path, "pe")
        fs.add_functions(sha, addrs)
        fs.record_many(sha, "decompiler", "1", digest, [
            dict(addr=a, sig="s", name="f%d" % i, source="int f%d(void) { return %d; }" % (i, i), verdict=v,
                 limits=lim, reason="ret-differs" if v == "divergent" else "")
            for i, (a, (v, lim)) in enumerate(zip(addrs, outcome))])
    fs.close()
    return path, sha, digest


def test_ledger_agrees_with_the_store():
    path, sha, digest = _fill_store()
    led = ledger.measure(module="version.dll")
    row = led["modules"][0]
    fs = F.open()
    tally = fs.tally(digest, sha)
    fs.close()
    assert {k: row[k] for k in F.ORDER} == tally, (row, tally)       # the store's own classes, not a second count
    assert tally["verified"] == tally["weak"] == tally["unverified"] == tally["not-judged"] == 1
    assert row["functions"] == 6 and row["decompiled"] == 5 and row["compiled"] == 4 and row["no-source"] == 1
    assert row["in_store"] and not row["measured"] and row["judged"] == 5
    assert 0 < row["bytes_verified"] < row["bytes_known"] <= row["code_bytes"] == 2048, row
    whole = ledger.measure()
    kinds = {k["kind"]: k for k in whole["kinds"]}
    assert kinds["pe"]["in_store"] == 1 and kinds["pe"]["verified"] == 1 and kinds["ne"]["functions"] == 0
    assert kinds["pe"]["modules"] == len(corpus.modules("pe")) and kinds["le"]["modules"] > 0 and kinds["dos"]["modules"] > 0
    assert whole["total"]["modules"] == len(whole["modules"]) == sum(k["modules"] for k in whole["kinds"])
    assert whole["total"]["code_bytes"] == sum(m["code_bytes"] for m in whole["modules"]) > 1000000
    text = "\n".join(ledger.render(whole))
    assert "version.dll" in text and F.TITLE["weak"] in text and "32-bit" in text
    out = os.path.join(TEMP, "coverage.csv")
    assert ledger.write_csv(whole, out) == len(whole["modules"])
    rows = list(csv.DictReader(open(out, encoding="utf-8")))
    mine = [r for r in rows if r["name"] == "version.dll"][0]
    assert mine["verified"] == "1" and mine["kind"] == "32-bit" and mine["state"] == "part measured"


def test_ledger_reads_without_writing():
    _path, sha, _digest = _fill_store()
    fs = ledger.open_store()
    try:
        for db, what in ((fs.db, "DELETE FROM module"), (fs.module_db(sha), "DELETE FROM verdict")):
            try:                                   # (the index, and the module's database)
                db.execute(what)
                raise AssertionError("the ledger's store connection accepted a write")
            except sqlite3.OperationalError:
                pass
        assert fs.candidates(sha, _digest)        # (and it reads)
    finally:
        fs.close()
    assert ledger.open_store(os.path.join(TEMP, "none.sqlite")) is None
    with redirect_stdout(io.StringIO()) as out:
        assert ledger.main(["--module", "version.dll"]) == 0 and ledger.main(["--module", "no_such_zz"]) == 2
    assert "returns a different value" not in out.getvalue() and "ret-differs" in out.getvalue()


# --- progress --------------------------------------------------------------------------------
def test_progress_reading():
    _fill_store()
    s = progress.snapshot()
    assert [k["kind"] for k in s["kinds"]][:2] == ["pe", "ne"] and s["kinds"][0]["verified"] == 1
    assert s["recent"][0]["module"] == "version.dll" and s["recent"][0]["results"] == 5 and s["idle"] is not None
    assert s["rate"] > 0 and isinstance(s["workers"], int)
    text = progress.line(s)
    assert "32-bit 0/" in text and "version.dll" in text and F.TITLE["verified"] + " 1" in text, text


# --- the query layer under the interfaces ------------------------------------------------------
def test_service_reads_the_store():
    _fill_store()
    # The test's store, answered in this process: a service running for this checkout (the MCP
    # server starts one) would answer from the real store instead.
    real_find, service.find = service.find, (lambda: None)
    try:
        _service_reads_the_store()
    finally:
        service.find = real_find


def _service_reads_the_store():
    fns = service.functions("version.dll")
    assert len(fns["functions"]) == 6 and fns["counts"]["verified"] == 1 and "unverified" in fns["reasons"]
    good = [f for f in fns["functions"] if f["class"] == "verified"][0]
    one = service.functions("version.dll", good["addr"])["function"]
    assert one["verdict"] == "equivalent" and one["source"].startswith("int f0") and one["title"] == F.TITLE["verified"]
    assert service.functions("no_such_zz.dll") is None
    try:
        service.JOBS.start("erase", "version.dll")
        raise AssertionError("an unknown action was started")
    except ValueError as e:
        assert "no such action" in str(e)
    assert service.JOBS.all() == []                                  # nothing was started


def test_terminal_interface():
    """The terminal interface (ui/terminal.py): its verdict bar and addresses, the store's quick
    overview it opens on, and every screen shown headless -- home, a module, its tabs, help, jobs."""
    import asyncio
    import importlib.util
    if not importlib.util.find_spec("textual"):
        raise Skip("textual is not installed")
    from codebreaker.ui import terminal as T
    b = T.bar({"verified": 3, "weak": 1}, 20)
    assert len(b.plain) == 20 and b.plain == "█" * 20
    assert len(T.bar({}, 12).plain) == 12
    assert T.flat_address("4:015c") == "1018015c" and T.flat_address("401000") == "401000"
    over = F.overview()                                     # (the test's own store)
    assert all({"name", "functions", "latest"} <= set(m) for m in over)
    path = _module("lzexpand.dll")

    async def tour():
        app = T.CodeBreaker(path)
        async with app.run_test(size=(160, 44)) as pilot:
            await pilot.pause(3)
            assert isinstance(app.screen, T.Module), app.screen
            for tab in ("tab-asm", "tab-calls", "tab-verdict", "tab-source"):
                app.screen.query_one(T.TabbedContent).active = tab
                await pilot.pause(0.3)
            await pilot.press("f", "question_mark")
            await pilot.pause(0.5)
            assert isinstance(app.screen, T.Help)
            await pilot.press("escape", "j")
            await pilot.pause(0.5)
            assert isinstance(app.screen, T.Jobs)
            await pilot.press("escape", "escape")
            await pilot.pause(0.5)
            assert isinstance(app.screen, T.Home)
    asyncio.run(tour())


def main():
    for fn in (test_describe_32bit, test_describe_16bit_driver_and_dos, test_describe_never_raises,
               test_strings_bytes_and_resources, test_info_command, test_ledger_agrees_with_the_store,
               test_ledger_reads_without_writing, test_progress_reading, test_service_reads_the_store,
               test_terminal_interface):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
