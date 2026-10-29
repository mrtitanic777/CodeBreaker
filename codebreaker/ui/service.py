r"""service.py -- what the interfaces read and start: one query layer under the terminal interface
(ui/terminal.py), the MCP server (ui/mcp_server.py) and the command line's queries, so they never
disagree -- and the client of the service (ui/serve.py) that answers them all from one process.

    call(name, *args, **kw)    a query or a job, by its name in CALLS: answered by the running
                               service, else here, in this process (the same function either way)
    cli(command, argv)         a query command (`facts`) answered by the running service -> its exit
                               code, or None: run it here
    stop_mine()                the jobs this process started, stopped (an interface quitting)
    stored_path(name)          where a module not in the corpus (a Java program's folder) came from
    functions(module[, addr])  what the store holds for a module, or for one function of it
    JOBS                       commands started from an interface, each its own process

A job is the same command a person would type (`python -m codebreaker decompile M`, `... measure M`),
run below normal priority with its output kept in state/ui_jobs/.

Which process answers. CB_SERVICE=0 (or off): always this one -- the in-process path, as before
the service. CB_SERVICE=1 (or on): the service, started when it is not running. Unset: an
interface (the terminal interface, the MCP server: START) starts the service on demand; anything
else (a command such as `facts`, a test) uses it when it runs and answers in process when not.
Inside the service itself (LOCAL) every call is answered there. When the service cannot be
reached or started, or goes away, the call is answered in process: nothing needs it to work.
"""
import contextlib
import http.client
import inspect
import io
import json
import os
import subprocess
import sys
import threading
import time

from codebreaker.base import corpus, settings
from codebreaker.coverage import ledger
from codebreaker.source import module_writer
from codebreaker.store import functions as F

START = False                    # an interface: start the service when it is not running
LOCAL = False                    # this process is the service: answer everything here
OWNER = "pid-%d" % os.getpid()   # whose jobs: this process's, unless the service is asked for a client
_CALL = threading.local()        # in the service: the client a call is answered for

ACTIONS = {"decompile": "decompile it to C (nothing is built or judged)",
           "measure": "decompile, build and judge every function; results go to the store",
           "module": "a folder of Java source: transpile it to C++, compile every class, write the folder"}


def stored_path(name):
    """The path a module not in the corpus (a Java program's folder) was reconstructed from."""
    fs = ledger.open_store()
    row = fs and fs.db.execute("SELECT path FROM module WHERE name=? ORDER BY ts DESC LIMIT 1", (name,)).fetchone()
    return row[0] if row and row[0] and os.path.exists(row[0]) else None


class Jobs(object):
    """Commands started from the terminal interface or the MCP server, each its own process with its output in a file."""

    def __init__(self):
        self.lock = threading.Lock()
        self.jobs = {}                                           # id -> dict
        self.count = 0

    def start(self, action, module):
        """Start `action` on a module. -> the job, or raises ValueError saying why not."""
        path = (module if os.path.isdir(module) else corpus.retail(module) or stored_path(module)) if module else None
        if action not in ACTIONS:
            raise ValueError("no such action %r" % action)
        if not path:
            raise ValueError("no module %r in the corpus" % module)
        name = os.path.basename(os.path.normpath(path)).lower()
        output = os.path.join(settings.state("ui_jobs"), os.path.splitext(name)[0] + ".c") if action == "decompile" else ""
        return self.launch(action, [path] + (["--out", output] if output else []), name, output)

    def launch(self, action, args, name, output="", owner=None):
        """Run `python -m codebreaker <action> <args>` as a job (any command of __main__.COMMANDS).
        -> the job, or raises ValueError("busy: ...") while two jobs run, or a measure would be
        the second. `output` is a file the command is told to write, shown once it is done.
        `owner`: whose job it is (default: the client asking, caller())."""
        with self.lock:
            running = [j for j in self.jobs.values() if j["proc"].poll() is None]
            if len(running) >= 2 or any(j["action"] == "measure" for j in running if action == "measure"):
                raise ValueError("busy: %s is still running" % ", ".join(
                    "%s %s" % (j["action"], j["module"]) for j in running))
            self.count += 1
            jid = "%s-%d" % (time.strftime("%H%M%S"), self.count)
            folder = settings.state("ui_jobs")
            os.makedirs(folder, exist_ok=True)
            log = os.path.join(folder, jid + ".log")
            argv = [sys.executable, "-u", "-m", "codebreaker", action] + list(args)
            with open(log, "wb") as f:
                proc = subprocess.Popen(argv, cwd=settings.ROOT, stdout=f, stderr=subprocess.STDOUT,
                                        stdin=subprocess.DEVNULL,
                                        creationflags=0x00004000 if os.name == "nt" else 0)   # below normal
            self.jobs[jid] = {"id": jid, "action": action, "module": name, "proc": proc, "log": log,
                              "output": output, "started": time.time(), "ended": None,
                              "owner": owner or caller()}
            return self.view(self.jobs[jid])

    def view(self, job, offset=None):
        """A job as plain data; with `offset`, also its output from that byte on."""
        code = job["proc"].poll()
        if code is not None and job["ended"] is None:
            job["ended"] = time.time()
        out = {"id": job["id"], "action": job["action"], "module": job["module"], "pid": job["proc"].pid,
               "status": "running" if code is None else "done" if code == 0 else "failed", "exit": code,
               "seconds": int((job["ended"] or time.time()) - job["started"]),
               "started": time.strftime("%H:%M:%S", time.localtime(job["started"])), "owner": job["owner"],
               "has_output": bool(job["output"]) and code == 0 and os.path.isfile(job["output"])}
        if offset is not None:
            try:
                with open(job["log"], "rb") as f:
                    f.seek(max(0, offset))
                    chunk = f.read(1 << 18)
            except OSError:
                chunk = b""
            out["log"], out["next"] = chunk.decode("utf-8", "replace"), max(0, offset) + len(chunk)
        return out

    def all(self):
        with self.lock:
            return [self.view(j) for j in sorted(self.jobs.values(), key=lambda j: -j["started"])]

    def get(self, jid):
        return self.jobs.get(jid)

    def stop(self, jid):
        job = self.jobs.get(jid)
        if job and job["proc"].poll() is None:
            job["proc"].terminate()                              # only a process started here
        return bool(job)

    def stop_all(self, owner=None):
        """Every job stopped, or with `owner` only that client's."""
        for jid, job in list(self.jobs.items()):
            if owner is None or job["owner"] == owner:
                self.stop(jid)

    def running(self):
        with self.lock:
            return sum(1 for j in self.jobs.values() if j["proc"].poll() is None)


JOBS = Jobs()


def _row(row, addr):
    """One function for the page: the store's best row for it, in words."""
    if not row:
        return {"addr": addr, "name": "", "class": "no-source", "title": F.TITLE["no-source"],
                "why": "no result under the verifier settings in force", "engine": "", "has_source": False}
    return {"addr": addr, "name": row["name"] or "", "class": row["class"], "title": F.TITLE[row["class"]],
            "why": module_writer.describe(row), "engine": row["engine"], "has_source": bool(row["source"])}


def functions(module, addr=None):
    """What the store holds for a module: {module, kind, sha, profile, in_store, counts, reasons,
    functions: [row]}; with `addr`, {..., function: that row with source, verdict, reason, limits,
    detail and its weak reasons spelled out}. (Answered by the service when it runs.)"""
    return call("functions", module, addr)


def _functions(module, addr=None):
    fs = ledger.open_store()
    try:
        led = ledger.measure(fs, module=module)
        if not led["modules"]:
            return None
        m = led["modules"][0]
        digest = led["profiles"].get(m["kind"])
        out = {"module": m["name"], "kind": m["kind"], "kind_title": ledger.TITLE[m["kind"]], "sha": m.get("sha", ""),
               "profile": digest, "in_store": m["in_store"], "measured": m["measured"],
               "counts": {k: m[k] for k in ledger.COUNTS}, "functions": [], "reasons": {}}
        if not m["in_store"]:
            return out
        best = fs.best(m["sha"], digest) if digest else {}
        if addr is not None:
            row = best.get(addr)
            one = _row(row, addr)
            if row:
                one.update(source=row["source"] or "", verdict=row["verdict"], reason=row["reason"] or "",
                           limits=row["limits"] or "", detail=row["detail"] or "", rev=row["rev"] or "",
                           problem=row["problem"] or "",
                           weak=[{"code": c, "text": F.WEAK[c]} for c in row["weak"]])
            out["function"] = one
            return out
        out["functions"] = [_row(best.get(a), a) for a in fs.addresses(m["sha"])]
        if digest:
            out["reasons"] = {k: [[why, n] for why, n in v] for k, v in ledger.reasons(fs, m["sha"], digest).items()}
        return out
    finally:
        if fs:
            fs.close()


# ---- what can be asked ---------------------------------------------------------------------------
QUERIES = {"facts"}              # commands of the command line that the service answers (cli)


def _overview():
    return F.overview()


def _latest(module_sha):
    return F.latest(module_sha)


def _cli(command, argv):
    """A query command run here, what it prints kept: {exit, out}."""
    if command not in QUERIES:
        raise ValueError("%r is not a query the service answers (%s)" % (command, ", ".join(sorted(QUERIES))))
    import importlib
    from codebreaker import __main__ as M
    module, fixed = M.COMMANDS[command]
    with captured() as out:
        code = importlib.import_module(module).main(fixed + list(argv))
    return {"exit": code or 0, "out": out.getvalue()}


CALLS = {"functions": _functions, "store.overview": _overview, "store.latest": _latest, "cli": _cli,
         "jobs.all": lambda: JOBS.all(), "jobs.running": lambda: JOBS.running(),
         "jobs.stop_all": lambda owner=None: JOBS.stop_all(owner)}


def registered(name):
    """The function a call's name stands for: CALLS, or `mcp.<tool>`, a tool of the MCP server
    (ui/mcp_server.TOOLS). Raises LookupError."""
    fn = CALLS.get(name)
    if fn is None and name.startswith("mcp."):
        from codebreaker.ui import mcp_server
        fn = {f.__name__: f for f in mcp_server.TOOLS}.get(name[4:])
    if fn is None:
        raise LookupError("no call %r" % name)
    return fn


def caller():
    """Whose call this is: in the service, the client it is answered for; else this process."""
    return getattr(_CALL, "owner", None) or OWNER


@contextlib.contextmanager
def answering_for(owner):
    """In the service: the calls made inside are answered for the client `owner`."""
    was = getattr(_CALL, "owner", None)
    _CALL.owner = owner
    try:
        yield
    finally:
        _CALL.owner = was


class Router(object):
    """stdout for a process that answers on threads (the service): what a thread prints inside
    captured() goes to that thread's buffer, anything else to the stream it had."""

    def __init__(self, base):
        self.base, self.local = base, threading.local()

    def write(self, text):
        buf = getattr(self.local, "buf", None)
        return (buf or self.base).write(text)

    def flush(self):
        buf = getattr(self.local, "buf", None)
        (buf or self.base).flush()

    def isatty(self):
        return False

    def __getattr__(self, name):
        return getattr(self.base, name)


@contextlib.contextmanager
def captured():
    """What is printed inside, kept in a StringIO (yielded) instead -- per thread under a Router,
    else by redirecting stdout."""
    out = io.StringIO()
    if isinstance(sys.stdout, Router):
        was = getattr(sys.stdout.local, "buf", None)
        sys.stdout.local.buf = out
        try:
            yield out
        finally:
            sys.stdout.local.buf = was
    else:
        with contextlib.redirect_stdout(out):
            yield out


# ---- the client ----------------------------------------------------------------------------------
class Gone(Exception):
    """The service could not be reached (not running, or it went away)."""


class RemoteError(RuntimeError):
    """A failure in the service that is not one the callers expect (ValueError, OSError, LookupError)."""


_RAISE = {"ValueError": ValueError, "OSError": OSError, "LookupError": LookupError}


def mode():
    """off, on, or "" (an interface starts it, anything else uses it when it runs): CB_SERVICE."""
    v = (os.environ.get("CB_SERVICE") or "").strip().lower()
    return "off" if v in ("0", "off", "no", "false") else "on" if v in ("1", "on", "yes", "true") else ""


def folder():
    """state/service: the running service's record (service.json), its token, its log."""
    return os.path.dirname(settings.state("service", "service.json"))


def record():
    """The running service's record {pid, port, root, state, started, ...}, or None."""
    try:
        with open(os.path.join(folder(), "service.json"), encoding="utf-8") as f:
            return json.load(f)
    except (OSError, ValueError):
        return None


def _same(a, b):
    return os.path.normcase(os.path.abspath(a or "")) == os.path.normcase(os.path.abspath(b or ""))


class Client(object):
    """Talks to the service: HTTP+JSON on 127.0.0.1, every request carrying the token."""

    def __init__(self, port, secret, pid=None):
        self.port, self.secret, self.pid = port, secret, pid
        self.started = False                         # this process started it

    def request(self, method, path, body=None, timeout=None):
        conn = http.client.HTTPConnection("127.0.0.1", self.port, timeout=timeout)
        try:
            data = json.dumps(body).encode("utf-8") if body is not None else None
            try:
                conn.request(method, path, body=data,
                             headers={"X-CodeBreaker-Token": self.secret, "Content-Type": "application/json"})
                r = conn.getresponse()
                text = r.read()
            except TimeoutError:
                raise                                    # busy, not gone
            except (OSError, http.client.HTTPException) as e:
                raise Gone(str(e))
            if r.status == 401:
                raise Gone("the service refused the token")
            return json.loads(text.decode("utf-8"))
        finally:
            conn.close()

    def ping(self, timeout=3.0):
        return self.request("GET", "/ping", timeout=timeout)

    def call(self, name, args, timeout=None):
        got = self.request("POST", "/call", {"name": name, "args": args, "owner": OWNER}, timeout=timeout)
        if "error" in got:
            raise _RAISE.get(got.get("kind"), RemoteError)(got["error"])
        return got.get("value")

    def stop(self):
        try:
            self.request("POST", "/stop", {}, timeout=5.0)
        except (Gone, TimeoutError):
            pass

    def wait_gone(self, seconds=15.0):
        """True once the service no longer answers."""
        end = time.time() + seconds
        while time.time() < end:
            try:
                self.ping(timeout=1.0)
            except (Gone, OSError):
                return True
            time.sleep(0.2)
        return False


def find():
    """A client of the service running for this checkout's state folder, or None. A service whose
    code changed under it (stale) and that runs no job is told to go, and None is returned."""
    rec = record()
    if not rec or not rec.get("port"):
        return None
    try:
        with open(os.path.join(folder(), "token"), encoding="ascii") as f:
            secret = f.read().strip()
    except OSError:
        return None
    c = Client(int(rec["port"]), secret, rec.get("pid"))
    try:
        pong = c.ping()
    except (Gone, OSError):
        return None
    if not _same(pong.get("state"), settings.get("state")):
        return None
    if pong.get("stale") and not pong.get("jobs"):
        c.stop()
        c.wait_gone()
        return None
    return c


LAUNCHED = []                    # pids of services this process started (tests stop them)


def launch(wait=60.0):
    """Start the service (`python -m codebreaker serve`) as a process of its own, out of this
    one's console and job, and wait until it answers. -> its client, or None."""
    log = open(os.path.join(folder(), "serve.log"), "ab")
    argv = [sys.executable, "-m", "codebreaker", "serve"]
    kw = dict(cwd=settings.ROOT, stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT)
    try:
        if os.name == "nt":
            flags = 0x08000000 | 0x00000200          # no window, a process group of its own
            try:
                proc = subprocess.Popen(argv, creationflags=flags | 0x01000000, **kw)   # out of our job
            except OSError:
                proc = subprocess.Popen(argv, creationflags=flags, **kw)
        else:
            proc = subprocess.Popen(argv, start_new_session=True, **kw)
    finally:
        log.close()
    LAUNCHED.append(proc.pid)
    end = time.time() + wait
    while time.time() < end:
        c = find()
        if c:
            c.started = c.pid == proc.pid
            return c
        if proc.poll() is not None:                  # it found another one running, or failed
            return find()
        time.sleep(0.2)
    return None


_CLIENT = None
_LOOKED = 0.0
_LOCK = threading.Lock()


def client(start=None):
    """The service's client, or None to answer in process (see the module's text). Looked for
    at most every five seconds while there is none."""
    global _CLIENT, _LOOKED
    m = mode()
    if LOCAL or m == "off":
        return None
    start = True if m == "on" else START if start is None else start
    with _LOCK:
        if _CLIENT is not None:
            return _CLIENT
        if time.time() - _LOOKED < 5.0:
            return None
        _LOOKED = time.time()
        _CLIENT = find() or (launch() if start else None)
        return _CLIENT


def _drop():
    global _CLIENT, _LOOKED
    with _LOCK:
        _CLIENT, _LOOKED = None, 0.0


def call(what, /, *args, **kw):
    """`what`'s answer (CALLS, or mcp.<tool>): from the service when it runs, else made here.
    (`what` is positional only: a tool may have an argument called `name`.)"""
    fn = registered(what)
    given = dict(inspect.signature(fn).bind(*args, **kw).arguments)
    for _ in range(2):
        c = client()
        if c is None:
            break
        try:
            return c.call(what, given)
        except Gone:
            _drop()                                      # gone: look again (start it again) once
    return fn(**given)


def cli(command, argv):
    """A query command answered by the running service (never started for it): prints what it
    printed. -> its exit code, or None when there is no service: run it here."""
    c = client(start=False)
    if c is None:
        return None
    try:
        got = c.call("cli", {"command": command, "argv": list(argv)})
    except Gone:
        _drop()
        return None
    sys.stdout.write(got["out"])
    return got["exit"]


def stop_mine():
    """The jobs this process started, stopped: in the service, and here."""
    c = _CLIENT
    if c is not None:
        try:
            c.call("jobs.stop_all", {"owner": OWNER}, timeout=10.0)
        except (Gone, OSError, RemoteError):
            pass
    JOBS.stop_all()
