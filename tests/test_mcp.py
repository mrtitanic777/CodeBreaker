#!/usr/bin/env python3
"""Tests for the MCP server (codebreaker/ui/mcp_server.py): its tools called through a real MCP
client, in process. Run:  python tests/test_mcp.py     (exit code 0 = all passed)

They use a function store, a state folder and a Converted folder of their own in a temporary
folder -- the real ones are never opened -- and version.dll from the corpus (checks that need it are SKIPPED when the corpus is not present).
SKIPPED as a whole when the mcp package is not installed."""
import asyncio
import json
import os
import shutil
import sys
import tempfile
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)
TEMP = tempfile.mkdtemp(prefix="cbmcp")
os.environ["CB_FUNCTIONS_DB"] = os.path.join(TEMP, "functions.sqlite")      # before anything opens a store
os.environ["CB_STATE"] = os.path.join(TEMP, "state")                          # job logs, reach runs, the kept ledger
os.environ["CB_CONVERTED"] = os.path.join(TEMP, "Converted")                  # project folders

from codebreaker.base import corpus                          # noqa: E402

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


def _mcp():
    try:
        from mcp import Client
    except ImportError:
        raise Skip("the mcp package is not installed (pip install mcp)")
    from codebreaker.ui import mcp_server
    return Client, mcp_server


def _module(name):
    if not os.path.isdir(corpus.root()):
        raise Skip("retail corpus not present")
    path = corpus.retail(name)
    if not path:
        raise Skip("%s is not in the corpus" % name)
    return path


def _calls(steps):
    """Run (tool, arguments) calls on a fresh server through a client. -> [(is_error, text)]"""
    Client, mcp_server = _mcp()

    async def go():
        out = []
        async with Client(mcp_server.build()) as c:
            for name, args in steps:
                r = await c.call_tool(name, args)
                out.append((r.is_error, "".join(getattr(x, "text", "") for x in r.content)))
        return out
    return asyncio.run(go())


# ---- tests -----------------------------------------------------------------------------------
def test_every_tool_is_offered():
    Client, mcp_server = _mcp()

    async def go():
        async with Client(mcp_server.build()) as c:
            tools = {t.name: t for t in (await c.list_tools()).tools}
            prompts = [p.name for p in (await c.list_prompts()).prompts]
            templates = [t.uri_template for t in (await c.list_resource_templates()).resource_templates]
            return tools, prompts, templates
    tools, prompts, templates = asyncio.run(go())
    for name in ("describe", "inspect", "strings", "hex_dump", "resource", "modules", "coverage", "progress",
                 "functions", "function", "names", "identify", "demangle", "commands", "command_help",
                 "run_command", "jobs", "job", "job_file", "stop_job", "name_function", "reach_results",
                 "projects", "project", "source_tree", "read_file", "decompile_function", "compare_versions"):
        assert name in tools, name
    assert tools["describe"].annotations.read_only_hint and not tools["run_command"].annotations.read_only_hint
    assert "module" in tools["functions"].input_schema["properties"]
    assert prompts == ["triage"] and templates == ["codebreaker://function/{module}/{addr}"]


def test_reading_a_module():
    path = _module("version.dll")
    got = _calls([("describe", {"path": path}), ("inspect", {"module": path, "section": "exports"}),
                  ("strings", {"module": path, "limit": 2}), ("hex_dump", {"module": path, "length": 16}),
                  ("inspect", {"module": path, "section": "no-such-part"})])
    d = json.loads(got[0][1])
    assert d["platform"] == "32-bit Windows" and d["lane"] == "pe" and d["summary"]["format"] == "PE32"
    assert json.loads(got[1][1])["exports"]["count"] > 5
    s = json.loads(got[2][1])
    assert s["count"] == 2 and s["total"] > 2 and s["next"] == 2
    assert json.loads(got[3][1])["text"].startswith("00000000  4D 5A")
    assert got[4][0] and "no part" in got[4][1]                                 # said, not a crash


def test_wrong_names_are_said():
    got = _calls([("describe", {"path": "no-such-module.dll"}), ("command_help", {"command": "nothing"}),
                  ("run_command", {"command": "serve"}), ("job", {"job_id": "x"})])
    assert all(err for err, _t in got), got
    assert "no module" in got[0][1] and "no command" in got[1][1] and "window or a server" in got[2][1]


def test_symbols():
    got = _calls([("demangle", {"decorated": ["?AddRef@CFoo@@UAGKXZ", "_Bar@8"]}),
                  ("commands", {}), ("command_help", {"command": "reach"})])
    d = json.loads(got[0][1])
    assert d["?AddRef@CFoo@@UAGKXZ"]["readable"] == "public: virtual unsigned long __stdcall CFoo::AddRef(void)"
    assert d["_Bar@8"]["stack_bytes"] == 8
    c = json.loads(got[1][1])
    assert "reach" in c["runnable"] and "serve" not in c["runnable"] and "mcp" not in c["runnable"]
    assert "--judge" in json.loads(got[2][1])["help"]


def test_a_job_runs_and_reports():
    Client, mcp_server = _mcp()

    async def go():
        async with Client(mcp_server.build()) as c:
            r = await c.call_tool("run_command", {"command": "demangle", "args": ["?f@@YAHH@Z"]})
            jid = json.loads(r.content[0].text)["id"]
            end = time.time() + 120
            while time.time() < end:
                j = json.loads((await c.call_tool("job", {"job_id": jid})).content[0].text)
                if j["status"] != "running":
                    os.remove(mcp_server.server.JOBS.get(jid)["log"])          # leave nothing behind
                    return j
                await asyncio.sleep(0.5)
            return j
    j = asyncio.run(go())
    assert j["status"] == "done" and "int __cdecl f(int)" in j["log"], j


def test_naming_writes_only_this_store():
    path = _module("version.dll")
    got = _calls([("names", {"module": path, "limit": 1})])
    addr = json.loads(got[0][1])["rows"][0]["addr"]
    got = _calls([("name_function", {"module": path, "addr": addr, "name": "TestNamedBody"}),
                  ("names", {"module": path, "find": "TestNamedBody"}),
                  ("name_function", {"module": path, "addr": addr, "name": "bad name;"})])
    assert json.loads(got[0][1])["named"] == "TestNamedBody"
    assert json.loads(got[1][1])["rows"][0]["source"] == "named body (named by hand)"
    assert got[2][0]
    assert os.path.dirname(os.environ["CB_FUNCTIONS_DB"]) == TEMP


def test_cut_and_slow():
    _Client, mcp_server = _mcp()
    big = {"small": 1, "huge": "x" * (mcp_server.MOST + 10)}
    cut = mcp_server._fit(big)
    assert cut["small"] == 1 and "huge" not in cut and cut["_cut"]["parts"]["huge"] > mcp_server.MOST
    slow = mcp_server.Slow()
    value, waited = slow.get("k", lambda: time.sleep(1.5) or 7, wait=0.1)
    assert value is None and waited >= 0                      # not ready: said at once
    value, _ = slow.get("k", lambda: 8, wait=5)                # the first one's value, not a second run
    assert value == 7
    value, _ = slow.get("e", lambda: 1 / 0, wait=5)
    assert "ZeroDivisionError" in value["error"]


def test_what_was_made():
    _Client, mcp_server = _mcp()
    from codebreaker.base import settings
    with open(settings.state("reach", "try.json"), "w", encoding="utf-8") as f:
        json.dump({"when": "now", "inputs": ["x"], "programs": [
            {"path": "a.gb", "platform": "Game Boy", "lane": "gb", "judged": {"tally": {"verified": 3}}},
            {"path": "b.exe", "platform": "32-bit Windows", "lane": "pe"}]}, f)
    folder = os.path.join(os.environ["CB_CONVERTED"], "demo")
    os.makedirs(os.path.join(folder, "src"))
    with open(os.path.join(folder, "project.json"), "w", encoding="utf-8") as f:
        json.dump({"name": "demo", "file": "demo.exe", "kind": "PE executable",
                   "steps": [{"command": "decompile", "outputs": ["src/demo.c"]}]}, f)
    with open(os.path.join(folder, "src", "demo.c"), "wb") as f:
        f.write(b"int main(void) { return 0; } /* caf\xe9 */")              # Windows-1252, as period files are
    outside = os.path.join(tempfile.gettempdir(), "cbmcp_outside.txt")
    with open(outside, "w") as f:
        f.write("no")
    try:
        got = _calls([("reach_results", {}), ("reach_results", {"run": "try", "judged_only": True}),
                      ("projects", {}), ("project", {"name": "demo"}),
                      ("read_file", {"path": os.path.join(folder, "src", "demo.c")}),
                      ("read_file", {"path": outside}), ("project", {"name": "nothing"})])
    finally:
        os.remove(outside)
    assert [r["run"] for r in json.loads(got[0][1])["runs"]] == ["try"]
    r = json.loads(got[1][1])
    assert r["total"] == 1 and r["rows"][0]["path"] == "a.gb" and r["platforms"]["32-bit Windows"]["programs"] == 1
    assert json.loads(got[2][1])["projects"][0]["commands"] == ["decompile"]
    assert {x["file"] for x in json.loads(got[3][1])["files"]} == {"project.json", "src/demo.c"}
    assert json.loads(got[4][1])["text"].endswith("café */")
    assert got[5][0] and "outside" in got[5][1] and got[6][0]


def test_one_function_cut_out():
    _Client, mcp_server = _mcp()
    text = ("/* a note */\nxunknown2 __cdecl16near sub_00132(void)\n\n{\n  return 1;\n}\n\n"
            "int2 __cdecl16far sub_1_0116(int2 a)\n{\n  return a;\n}\n\n"
            "int2 __stdcall16far LZINIT(int2 a1)\n{\n  return 0;\n}\n")
    cut = mcp_server._cut_function
    assert cut(text, "132").startswith("/* a note */") and cut(text, "132").endswith("}")
    assert cut(text, "1320") is None and cut(text, "32") is None                 # numbers, not substrings
    assert "return a;" in cut(text, "0001:0116") and cut(text, "2:0116") is None
    assert "return 0;" in cut(text, "lzinit")
    path = _module("version.dll")
    got = _calls([("names", {"module": path, "limit": 1})])
    addr = json.loads(got[0][1])["rows"][0]["addr"]
    got = _calls([("decompile_function", {"module": path, "addr": addr, "wait": 200})])
    d = json.loads(got[0][1])
    assert not got[0][0] and "{" in d["text"] and d["note"].startswith("decompiled now"), got


def test_compare_two_builds():
    path = _module("version.dll")
    got = _calls([("compare_versions", {"module_a": path, "module_b": path, "limit": 2, "wait": 200})])
    d = json.loads(got[0][1])
    assert d["counts"]["changed"] == 0 and d["counts"]["identical"] == d["total"] > 5 and d["count"] == 2


def test_ledger_kept_answers_at_once():
    _Client, mcp_server = _mcp()
    from codebreaker.base import settings
    with open(settings.state("mcp", "ledger.json"), "w", encoding="utf-8") as f:
        json.dump({"time": time.time() - 3600, "ledger": {"total": {"functions": 9}, "kinds": [], "modules": [],
                                                          "profiles": {}}}, f)

    class Quick(mcp_server.Slow):                                # the fresh one never comes in time
        def get(self, key, work, age=600.0, wait=20.0, again=False):
            return mcp_server.Slow.get(self, key, work, age, 0.2, again)
    real, mcp_server._ledger, mcp_server.SLOW = (mcp_server._ledger, lambda: time.sleep(5),
                                                 Quick())
    try:
        t = time.time()
        got = mcp_server.coverage()
        assert got["total"] == {"functions": 9} and "as_of" in got["kept"] and time.time() - t < 3, got
    finally:
        mcp_server._ledger, mcp_server.SLOW = real, mcp_server.Slow()


def test_http_needs_the_token():
    _Client, mcp_server = _mcp()
    seen = []

    async def inner(scope, receive, send):
        seen.append(scope["path"])
        await send({"type": "http.response.start", "status": 200, "headers": []})
        await send({"type": "http.response.body", "body": b"ok"})
    app = mcp_server.Bearer(inner, "s3cret")

    async def ask(header):
        out = []

        async def send(m):
            out.append(m)
        headers = [(b"authorization", header)] if header is not None else []
        await app({"type": "http", "path": "/mcp", "headers": headers}, None, send)
        return out[0]["status"]
    assert asyncio.run(ask(None)) == 401 and asyncio.run(ask(b"Bearer nope")) == 401
    assert asyncio.run(ask(b"Bearer s3cret")) == 200 and seen == ["/mcp"]
    os.environ["CB_MCP_TOKEN"] = "from-env"
    try:
        assert mcp_server.token() == "from-env"
    finally:
        del os.environ["CB_MCP_TOKEN"]
    made = mcp_server.token()
    assert len(made) > 30 and made == mcp_server.token()                       # made once, then kept


def main():
    for fn in (test_every_tool_is_offered, test_reading_a_module, test_wrong_names_are_said, test_symbols,
               test_a_job_runs_and_reports, test_naming_writes_only_this_store, test_cut_and_slow,
               test_what_was_made, test_one_function_cut_out, test_compare_two_builds,
               test_ledger_kept_answers_at_once, test_http_needs_the_token):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    shutil.rmtree(TEMP, ignore_errors=True)                     # the store, state and projects made here
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
