#!/usr/bin/env python3
"""Tests for the MCP server (codebreaker/ui/mcp_server.py): its tools called through a real MCP
client, in process. Run:  python tests/test_mcp.py     (exit code 0 = all passed)

They use a function store of their own in a temporary folder -- the real store is never opened --
and version.dll from the corpus (checks that need it are SKIPPED when the corpus is not present).
SKIPPED as a whole when the mcp package is not installed."""
import asyncio
import json
import os
import sys
import tempfile
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)
TEMP = tempfile.mkdtemp(prefix="cbmcp")
os.environ["CB_FUNCTIONS_DB"] = os.path.join(TEMP, "functions.sqlite")      # before anything opens a store

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
                 "run_command", "jobs", "job", "job_file", "stop_job", "name_function"):
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


def main():
    for fn in (test_every_tool_is_offered, test_reading_a_module, test_wrong_names_are_said, test_symbols,
               test_a_job_runs_and_reports, test_naming_writes_only_this_store, test_cut_and_slow):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
