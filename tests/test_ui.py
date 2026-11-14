#!/usr/bin/env python3
"""Tests for what shows the work: the inspector and the terminal interface. Run:  python tests/test_ui.py     (exit code 0 = all passed)

They use three small corpus modules (version.dll, lzexpand.dll, vkd.vxd) and a function store of
their own in a temporary folder -- the real store is never opened. Checks that need the corpus
are SKIPPED when it is not present."""
import io
import json
import os
import sys
import tempfile
from contextlib import redirect_stdout

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)
TEMP = tempfile.mkdtemp(prefix="cbui")
os.environ["CB_FUNCTIONS_DB"] = os.path.join(TEMP, "functions.sqlite")      # before anything opens a store

from codebreaker.base import corpus                          # noqa: E402
from codebreaker.formats import inspect                      # noqa: E402
from codebreaker.store import functions as F                 # noqa: E402
from codebreaker.verify import reconstruct                   # noqa: E402

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


# --- a store of our own ----------------------------------------------------------------------
def _fill_store():
    """A store of our own holding version.dll with one function of each outcome."""
    path = _module("version.dll")
    digest = reconstruct.digests()["pe"]
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


def test_terminal_interface():
    """The terminal interface (ui/terminal.py) against the native service: its verdict bar and
    addresses, the store's overview it opens on, and every screen shown headless -- home, a module,
    its tabs, help, jobs -- then the service gone: ServiceDown says so and `r` brings it back.
    Needs the native program (CB_EXE or the build in state/cxx) and a corpus."""
    import importlib.util
    if not hasattr(sys, "meta_path"):
        raise Skip("run under python: the exe's translated runtime has no asyncio/Textual (async def needs Python's compiler)")
    import asyncio
    if not importlib.util.find_spec("textual"):
        raise Skip("textual is not installed")
    from codebreaker.ui import client
    from codebreaker.ui import terminal as T
    b = T.bar({"verified": 3, "weak": 1}, 20)
    assert len(b.plain) == 20 and b.plain == "\u2588" * 20
    assert len(T.bar({}, 12).plain) == 12
    assert T.flat_address("4:015c") == "1018015c" and T.flat_address("401000") == "401000"
    over = F.overview()                                     # (the test's own store)
    assert all({"name", "functions", "latest"} <= set(m) for m in over)
    path = _module("lzexpand.dll")
    version, _sha, _digest = _fill_store()
    if not client.program():
        raise Skip("the native program is not built (CB_EXE)")
    was_state = os.environ.get("CB_STATE")
    os.environ["CB_STATE"] = os.path.join(TEMP, "state")    # a service of its own, on the test's store (CB_FUNCTIONS_DB)
    client._drop()

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
            assert any(r["name"] == "version.dll" for r in app.screen.rows), app.screen.rows   # the overview, from the service
            app.push_screen(T.Module(version))                                                 # a module the store holds
            for _ in range(40):
                await pilot.pause(0.5)
                if app.screen.rows:
                    break
            # (a row the service has no verdict for under its own verifier settings shows the last one it had: store.latest)
            seen = {r.get("earlier") or r["class"] for r in app.screen.rows}
            assert len(app.screen.rows) == 6 and seen >= {"verified", "weak", "unverified", "not-judged", "no-compile"}, app.screen.rows
    asyncio.run(tour())

    async def gone():
        """No service and no program to start one with: ServiceDown, then the program back and `r`."""
        have = client.program
        c = client.find()
        if c:
            c.stop()
            c.wait_gone()
        client._drop()
        client.program = lambda: None
        try:
            app = T.CodeBreaker(None)
            async with app.run_test(size=(160, 44)) as pilot:
                await pilot.pause(2)
                assert isinstance(app.screen, T.ServiceDown) and "was not found" in app.screen.message, app.screen
                client.program = have
                await pilot.press("r")
                for _ in range(130):
                    await pilot.pause(0.5)
                    if isinstance(app.screen, T.Home):
                        break
                assert isinstance(app.screen, T.Home), app.screen
        finally:
            client.program = have
    try:
        asyncio.run(gone())
    finally:
        c = client.find()
        if c:
            c.stop()
            c.wait_gone()
        client._drop()
        if was_state is None:
            del os.environ["CB_STATE"]
        else:
            os.environ["CB_STATE"] = was_state


def main():
    """`--only NAME ...` runs just those tests (tools/gate.cmd runs test_terminal_interface this way, under python)."""
    every = (test_describe_32bit, test_describe_16bit_driver_and_dos, test_describe_never_raises,
             test_strings_bytes_and_resources, test_info_command, test_terminal_interface)
    only = sys.argv[sys.argv.index("--only") + 1:] if "--only" in sys.argv else None
    for fn in every:
        if only is None or fn.__name__ in only:
            check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
