#!/usr/bin/env python3
"""Tests for the terminal interface's client of the CodeBreaker service (codebreaker/ui/client.py) against
the native service (`serve`, native/cb/ui/serve.*): started on demand, the token, queries and jobs answered
by it, one instance per state folder, started again when it has gone, exiting when idle or when its program
changes -- and, when it cannot be had at all, the client saying why (Unavailable) instead of answering from
anywhere else.
Run:  python tests/test_service.py     (exit code 0 = all passed)

Needs the native program: CB_EXE, `exe` in settings.json, or the build in state/cxx/codebreaker/build (the
checks are SKIPPED, and say so, without it). Every service runs on a state folder (and a function store) of
its own in a temporary folder; every one started here is stopped before the end. Seconds; no corpus."""
import http.client
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
os.environ["CB_FUNCTIONS_DB"] = os.path.join(TEMP, "functions.sqlite")
os.environ["CB_STATE"] = os.path.join(TEMP, "state")
os.environ["CB_CONVERTED"] = os.path.join(TEMP, "Converted")
os.environ.pop("CB_SERVICE_PORT", None)

from codebreaker.ui import client                    # noqa: E402

RESULTS = []
STARTED = []                                         # Popen of services started here by hand


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


def _exe():
    exe = client.program()
    if not exe:
        raise Skip("the native program is not built (CB_EXE)")
    return exe


def _env(state):
    return dict(os.environ, CB_STATE=state, CB_FUNCTIONS_DB=os.path.join(state, "functions.sqlite"))


def _serve(args, env=None, timeout=120):
    return subprocess.run([_exe(), "serve"] + args, cwd=ROOT, env=env or dict(os.environ),
                          capture_output=True, text=True, timeout=timeout)


def _wait_record(state, seconds=60):
    end = time.time() + seconds
    path = os.path.join(state, "service", "service.json")
    while time.time() < end and not os.path.exists(path):
        time.sleep(0.2)
    return os.path.exists(path)


# ---- tests -----------------------------------------------------------------------------------
def test_unavailable_says_why():
    """No program to start: Unavailable names where it looked. A program that exits at once: its end is said."""
    assert client.record() is None and client.find() is None
    real = client.program
    client.program = lambda: None
    try:
        client._drop()
        try:
            client.connect(force=True)
            raise AssertionError("connected to nothing")
        except client.Unavailable as e:
            assert "was not found" in str(e) and "Looked for it at" in str(e), str(e)
        try:
            client.call("jobs.all")                                   # (the failed start is not repeated at once)
            raise AssertionError("answered by nothing")
        except client.Unavailable:
            pass
        client.program = lambda: os.environ.get("COMSPEC") or "/bin/false"   # a program that is not a service (the shell, run as `<shell> serve`): it stops again (not sys.executable: in the exe that IS the service)
        try:
            client.connect(force=True)
            raise AssertionError("connected to a program that is not a service")
        except client.Unavailable as e:
            assert "stopped again" in str(e) and "serve.log" in str(e), str(e)
    finally:
        client.program = real
        client._drop()
    assert client.find() is None and not client.connected()


def test_started_on_demand():
    _exe()
    client._drop()
    c = client.connect()
    assert c is not None and c.pid, c                                 # (started by us; the program may run the service as a copy of itself, so its pid is not the one we launched)
    p = c.ping()
    rec = client.record()
    assert p["pid"] == rec["pid"] == c.pid and p["port"] == rec["port"]
    assert os.path.normcase(p["state"]) == os.path.normcase(os.environ["CB_STATE"]) and not p["stale"]
    assert os.path.isfile(os.path.join(client.folder(), "token"))
    assert client.connect() is c and client.connected()              # the one client of this process


def test_the_token_is_needed():
    c = client.find()
    assert c is not None
    for headers in ({}, {"X-CodeBreaker-Token": "nope"}):
        conn = http.client.HTTPConnection("127.0.0.1", c.port, timeout=5)
        conn.request("GET", "/ping", headers=headers)
        r = conn.getresponse()
        r.read()
        conn.close()
        assert r.status == 401, (headers, r.status)
    assert c.ping()["pid"] == c.pid


def test_queries_answered_by_it():
    got = client.call("mcp.commands")
    assert "<command>" in got["usage"] and got["runnable"], got
    assert client.call("mcp.demangle", decorated=["_Bar@8"])["_Bar@8"]["stack_bytes"] == 8
    assert client.call("functions", module="no_such_zz.dll") is None
    assert client.call("jobs.all") == [] and client.call("store.overview") == []
    assert client.call("mcp.jobs")["jobs"] == []
    try:
        client.call("mcp.describe", path="no-such-module.dll")
        raise AssertionError("no error")
    except ValueError as e:                                           # the kind of failure carried over
        assert "no module" in str(e)
    try:
        client.call("no.such.call")
        raise AssertionError("no error")
    except LookupError:
        pass


def test_a_job_runs_in_it():
    j = client.call("mcp.run_command", command="serve", args=["--status"])   # (a command every program has: the service says it runs)
    assert j["owner"] == client.OWNER                                 # (the service keeps the jobs per client)
    end = time.time() + 120
    while time.time() < end:
        got = client.call("mcp.job", job_id=j["id"])
        if got["status"] != "running":
            break
        time.sleep(0.3)
    assert got["status"] == "done" and "pid %d" % client.find().pid in got["log"], got
    assert any(x["id"] == j["id"] for x in client.call("mcp.jobs")["jobs"])
    client.stop_mine()                                                # (nothing of ours runs: a no-op)


def test_one_instance_per_state_folder():
    first = client.find()
    r = _serve([], timeout=60)
    assert r.returncode == 0 and "already running" in r.stdout and "pid %d" % first.pid in r.stdout, r.stdout + r.stderr
    r = _serve(["--status"], timeout=60)
    assert r.returncode == 0 and "pid %d" % first.pid in r.stdout, r.stdout
    assert client.find().pid == first.pid


def test_gone_then_started_again():
    c = client.find()
    c.stop()
    assert c.wait_gone() and client.record() is None                  # its record taken away
    assert client.call("mcp.demangle", decorated=["_Bar@8"])["_Bar@8"]["stack_bytes"] == 8   # started again, once
    assert client.find() is not None and client.find().pid != c.pid


def test_idle_exit():
    state = os.path.join(TEMP, "idle")
    p = subprocess.Popen([_exe(), "serve", "--idle", "0.05"], cwd=ROOT, env=_env(state),
                         stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    STARTED.append(p)
    t = time.time()
    p.wait(timeout=60)
    assert p.returncode == 0 and time.time() - t >= 3.0
    assert not os.path.exists(os.path.join(state, "service", "service.json"))


def test_stale_when_the_program_changes():
    """The service runs from a copy of the program; the copy's time changing (a rebuilt program) makes it exit."""
    exe = _exe()
    state = os.path.join(TEMP, "stale")
    copy = os.path.join(os.path.dirname(exe), "stale-%d-program%s" % (os.getpid(), os.path.splitext(exe)[1]))   # (beside the program: its libraries)
    shutil.copy2(exe, copy)
    try:
        p = subprocess.Popen([copy, "serve", "--idle", "10"], cwd=ROOT, env=_env(state),
                             stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, text=True)
        STARTED.append(p)
        assert _wait_record(state), "the service did not start"
        later = time.time() + 120
        os.utime(copy, (later, later))                                # the program "rebuilt"
        p.wait(timeout=60)
        err = p.stderr.read()
        assert p.returncode == 0 and "code changed" in err, err
    finally:
        try:
            os.remove(copy)
        except OSError:
            pass


def main():
    for fn in (test_unavailable_says_why, test_started_on_demand, test_the_token_is_needed, test_queries_answered_by_it,
               test_a_job_runs_in_it, test_one_instance_per_state_folder, test_gone_then_started_again, test_idle_exit,
               test_stale_when_the_program_changes):
        check(fn)
    c = client.find()                                                 # every service started here, stopped
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
    print("%d passed, %d skipped, %d failed" % (sum(1 for r in RESULTS if r[1] == "PASS"),
                                                sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
