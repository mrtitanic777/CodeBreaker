#!/usr/bin/env python3
"""Tests for the CodeBreaker service (codebreaker/ui/serve.py) and its client (ui/service.py):
started on demand, the token, queries and jobs answered by it, one instance per state folder,
the command line's `facts` through it, the MCP server's tools forwarded to it, the in-process
path when it is not there or goes away, and exiting when idle or when its code changes.
Run:  python tests/test_service.py     (exit code 0 = all passed)

Every service runs on a state folder (and a function store) of its own in a temporary folder;
every one started here is stopped before the end. Seconds; no corpus, no toolchain."""
import asyncio
import http.client
import json
import os
import shutil
import subprocess
import sys
import tempfile
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)
TEMP = tempfile.mkdtemp(prefix="cbsvc")
os.environ["CB_FUNCTIONS_DB"] = os.path.join(TEMP, "functions.sqlite")      # before anything opens a store
os.environ["CB_STATE"] = os.path.join(TEMP, "state")
os.environ["CB_CONVERTED"] = os.path.join(TEMP, "Converted")
os.environ.pop("CB_SERVICE", None)
os.environ.pop("CB_SERVICE_PORT", None)

from codebreaker.ui import serve, service           # noqa: E402

RESULTS = []
STARTED = []                                         # Popen of services started here by hand


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "PASS", ""))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


def _env(state):
    env = dict(os.environ, CB_STATE=state, CB_FUNCTIONS_DB=os.path.join(state, "functions.sqlite"))
    env.pop("CB_SERVICE", None)
    return env


def _cb(args, env=None, timeout=120):
    return subprocess.run([sys.executable, "-m", "codebreaker"] + args, cwd=ROOT, env=env or dict(os.environ),
                          capture_output=True, text=True, timeout=timeout)


def _calls():
    c = service.find()
    return c.ping()["calls"] if c else None


# ---- tests -----------------------------------------------------------------------------------
def test_in_process_without_a_service():
    assert service.record() is None and service.find() is None
    assert service.client() is None                                 # not an interface: never started
    assert service.call("jobs.all") == [] and service.functions("no_such_zz.dll") is None
    os.environ["CB_SERVICE"] = "0"
    try:
        assert service.client(start=True) is None                   # off: always here
    finally:
        del os.environ["CB_SERVICE"]


def test_started_on_demand():
    service.START = True
    service._drop()
    c = service.client()
    assert c is not None and c.started and c.pid in service.LAUNCHED, c
    p = c.ping()
    rec = service.record()
    assert p["pid"] == rec["pid"] == c.pid and p["port"] == rec["port"] == serve.fixed_port()
    assert os.path.normcase(p["state"]) == os.path.normcase(os.environ["CB_STATE"]) and not p["stale"]
    assert os.path.isfile(os.path.join(service.folder(), "token"))
    service.START = False


def test_the_token_is_needed():
    c = service.find()
    for headers in ({}, {"X-CodeBreaker-Token": "nope"}):
        conn = http.client.HTTPConnection("127.0.0.1", c.port, timeout=5)
        conn.request("GET", "/ping", headers=headers)
        r = conn.getresponse()
        r.read()
        conn.close()
        assert r.status == 401, (headers, r.status)
    assert c.ping()["pid"] == c.pid


def test_queries_answered_by_it():
    from codebreaker.ui import mcp_server
    before = _calls()
    assert service.call("mcp.commands") == mcp_server.commands()      # its answer is the in-process one
    assert service.call("mcp.demangle", ["_Bar@8"])["_Bar@8"]["stack_bytes"] == 8
    assert service.functions("no_such_zz.dll") is None
    try:
        service.call("mcp.describe", "no-such-module.dll")
        raise AssertionError("no error")
    except ValueError as e:                                           # the kind of failure carried over
        assert "no module" in str(e)
    try:
        service.find().call("no.such.call", {})
        raise AssertionError("no error")
    except LookupError:
        pass
    assert _calls() - before >= 5, (before, _calls())


def test_a_job_runs_in_it():
    j = service.call("mcp.run_command", "demangle", ["?f@@YAHH@Z"])
    assert j["owner"] == service.OWNER and not service.JOBS.all()    # the service's process, not ours
    end = time.time() + 120
    while time.time() < end:
        got = service.call("mcp.job", j["id"])
        if got["status"] != "running":
            break
        time.sleep(0.3)
    assert got["status"] == "done" and "int __cdecl f(int)" in got["log"], got
    assert any(x["id"] == j["id"] for x in service.call("mcp.jobs")["jobs"])
    service.stop_mine()                                               # (nothing of ours runs: a no-op)


def test_one_instance_per_state_folder():
    first = service.find()
    r = _cb(["serve"], timeout=60)
    assert r.returncode == 0 and "already running" in r.stdout and "pid %d" % first.pid in r.stdout, r.stdout + r.stderr
    r = _cb(["serve", "--status"], timeout=60)
    assert r.returncode == 0 and "pid %d" % first.pid in r.stdout, r.stdout
    assert service.find().pid == first.pid


def test_facts_through_it():
    before = _calls()
    r = _cb(["facts", "--databases"])
    assert r.returncode == 0 and "db_max_gb" in r.stdout, r.stdout + r.stderr
    assert _calls() == before + 1                                     # answered by the service
    local = _cb(["facts", "--databases"], env=dict(os.environ, CB_SERVICE="0"))
    assert local.stdout == r.stdout and _calls() == before + 1       # the same answer, here


def test_mcp_tools_forwarded():
    try:
        from mcp import Client
    except ImportError:
        return                                                         # (the mcp package is not installed)
    from codebreaker.ui import mcp_server

    async def go():
        async with Client(mcp_server.build()) as c:
            r = await c.call_tool("demangle", {"decorated": ["_Bar@8"]})
            bad = await c.call_tool("describe", {"path": "no-such-module.dll"})
            return json.loads(r.content[0].text), bad.is_error, bad.content[0].text
    before = _calls()
    got, is_error, text = asyncio.run(go())
    assert got["_Bar@8"]["stack_bytes"] == 8 and is_error and "no module" in text
    assert _calls() == before + 2


def test_gone_then_in_process():
    c = service.find()
    c.stop()
    assert c.wait_gone() and service.record() is None                 # its record taken away
    assert service.call("mcp.demangle", ["_Bar@8"])["_Bar@8"]["stack_bytes"] == 8   # answered here
    assert service.client() is None


def test_idle_exit():
    state = os.path.join(TEMP, "idle")
    p = subprocess.Popen([sys.executable, "-m", "codebreaker", "serve", "--idle", "0.05"], cwd=ROOT,
                         env=_env(state), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    STARTED.append(p)
    t = time.time()
    p.wait(timeout=60)
    assert p.returncode == 0 and time.time() - t >= 3.0
    assert not os.path.exists(os.path.join(state, "service", "service.json"))


def test_stale_when_the_code_changes():
    state = os.path.join(TEMP, "stale")
    p = subprocess.Popen([sys.executable, "-m", "codebreaker", "serve", "--idle", "10"], cwd=ROOT,
                         env=_env(state), stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, text=True)
    STARTED.append(p)
    end = time.time() + 60
    while time.time() < end and not os.path.exists(os.path.join(state, "service", "service.json")):
        time.sleep(0.2)
    stamp = list(serve.code_stamp())
    probe = os.path.join(serve.PACKAGE, "ui", "_stale_probe.py")    # a new file of the package: its count changes
    with open(probe, "w") as f:
        f.write("")
    try:
        p.wait(timeout=30)
    finally:
        os.remove(probe)
    err = p.stderr.read()
    assert p.returncode == 0 and "code changed" in err and serve.code_stamp() == stamp, err


def main():
    for fn in (test_in_process_without_a_service, test_started_on_demand, test_the_token_is_needed,
               test_queries_answered_by_it, test_a_job_runs_in_it, test_one_instance_per_state_folder,
               test_facts_through_it, test_mcp_tools_forwarded, test_gone_then_in_process, test_idle_exit,
               test_stale_when_the_code_changes):
        check(fn)
    c = service.find()                                                # every service started here, stopped
    if c:
        c.stop()
        c.wait_gone()
    for p in STARTED:
        if p.poll() is None:
            p.terminate()
            p.wait(timeout=30)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    shutil.rmtree(TEMP, ignore_errors=True)
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d failed" % (sum(1 for r in RESULTS if r[1] == "PASS"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
