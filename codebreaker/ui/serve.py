r"""serve.py -- CodeBreaker as a service: one long-running process that owns the databases and the
engine, and answers the terminal interface, the MCP server and the command line's queries.

    python -m codebreaker serve                  start it here (or say where the one running is)
    python -m codebreaker serve --status         the one running: pid, port, jobs, idle time
    python -m codebreaker serve --stop           stop the one running (its jobs with it)
        --idle MINUTES    exit after this long with no call and no job (setting service_idle_minutes,
                          default 20)
        --port N          the port to answer on (setting service_port; default one fixed for this
                          state folder, 20000-39999)

Nobody needs to start it by hand: an interface (ui/terminal.py, ui/mcp_server.py) starts it on
demand through ui/service.py, and every client answers in process when it is not there.

One per checkout. It answers on 127.0.0.1 only, at a port fixed for the state folder (the next
one up when another program holds it), bound exclusively, so a second `serve` cannot bind beside
it: it finds the one running and says so. Its record is state/service/service.json (pid, port,
root, state folder, when started); its token state/service/token, a new secret each start, which
every request must carry (header X-CodeBreaker-Token) -- the state folder is this user's, so only
this user's processes can talk to it. What it prints goes to state/service/serve.log.

The protocol, HTTP+JSON:

    GET  /ping   -> {pid, port, root, state, started, idle_minutes, idle_for, calls, jobs, stale}
    POST /call   {"name": N, "args": {...}, "owner": CLIENT}
                 -> {"value": ...} or {"error": text, "kind": ValueError|OSError|LookupError|...}
                 N is a name of ui/service.CALLS (functions, store.overview, jobs.*, cli) or
                 mcp.<tool>, any tool of the MCP server; `owner` is whose jobs a job started is
    POST /stop   -> {"stopping": true}; it stops its jobs and exits

What it keeps between calls is what a process keeps: the databases open (engine/facts.py), each
step's code digest worked out once (engine.py), binaries' hashes by size and time, the MCP
server's slow answers (the ledger, decompiles), the jobs it started (they are its own processes).

It exits when it has answered no call for the idle time and runs no job, and when its own code
(codebreaker/*.py) or settings.json changed since it started (it is "stale": what it would answer
is the old code's) as soon as no job runs; the next client starts a fresh one. On exit it stops
its jobs and takes its record away.
"""
import hmac
import json
import os
import secrets
import socket
import socketserver
import sys
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

from codebreaker.base import settings
from codebreaker.ui import service

DEFAULT_IDLE_MINUTES = 20
PACKAGE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def code_stamp():
    """What the service's answers are made by: every .py file of the package (how many, the
    newest time, the total size) and settings.json's time."""
    n = newest = size = 0
    for top, dirs, files in os.walk(PACKAGE):
        dirs[:] = [d for d in dirs if d != "__pycache__"]
        for f in files:
            if f.endswith(".py"):
                try:
                    st = os.stat(os.path.join(top, f))
                except OSError:
                    continue
                n, size, newest = n + 1, size + st.st_size, max(newest, st.st_mtime_ns)
    try:
        conf = os.stat(settings.FILE).st_mtime_ns
    except OSError:
        conf = 0
    return [n, newest, size, conf]


def fixed_port():
    """The port of this state folder's service: the setting service_port, else one worked out
    from the folder's path (so two checkouts, or a test's state folder, never meet)."""
    import zlib
    try:
        p = int(settings.get("service_port") or 0)
    except (TypeError, ValueError):
        p = 0
    return p or 20000 + zlib.crc32(os.path.normcase(os.path.abspath(settings.get("state"))).encode()) % 20000


def idle_minutes():
    try:
        return float(settings.get("service_idle_minutes") or DEFAULT_IDLE_MINUTES)
    except (TypeError, ValueError):
        return float(DEFAULT_IDLE_MINUTES)


class Server(ThreadingHTTPServer):
    """127.0.0.1 only, the port held exclusively (Windows lets two sockets share a port without it)."""
    daemon_threads = True
    allow_reuse_address = False

    def server_bind(self):
        if os.name == "nt":
            self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_EXCLUSIVEADDRUSE, 1)
        socketserver.TCPServer.server_bind(self)        # (not HTTPServer's: no name lookup)
        self.server_name, self.server_port = "127.0.0.1", self.server_address[1]


class Service(object):
    """The running service's state: when it last answered, what it is answering, whether stale."""

    def __init__(self, port, secret, idle):
        self.port, self.secret, self.idle = port, secret, idle
        self.started = self.last = time.time()
        self.busy = self.calls = 0
        self.lock = threading.Lock()
        self.code = code_stamp()
        self.stale = False
        self.why = ""
        self.server = None
        self.done = threading.Event()

    def ping(self):
        with self.lock:
            busy, last = self.busy, self.last
        return {"pid": os.getpid(), "port": self.port, "root": settings.ROOT, "state": settings.get("state"),
                "started": self.started, "idle_minutes": self.idle,
                "idle_for": 0 if busy else round(time.time() - last, 1), "calls": self.calls,
                "busy": busy, "jobs": service.JOBS.running(), "stale": self.stale}

    def answer(self, body):
        """One /call: the value, or the failure in words."""
        name, args = body.get("name") or "", body.get("args") or {}
        with self.lock:
            self.busy += 1
            self.calls += 1
        try:
            fn = service.registered(name)
            with service.answering_for(body.get("owner")):
                return {"value": fn(**args)}
        except Exception as e:                           # noqa: BLE001 -- said to the client
            kind = next((k.__name__ for k in type(e).__mro__ if k.__name__ in ("ValueError", "OSError", "LookupError")),
                        type(e).__name__)
            return {"error": str(e) or type(e).__name__, "kind": kind}
        finally:
            with self.lock:
                self.busy -= 1
                self.last = time.time()

    def stop(self, why):
        if not self.done.is_set():
            self.why = why
            self.done.set()
            threading.Thread(target=self.server.shutdown, daemon=True).start()

    def watch(self):
        """Every second: exit when idle past the time, or stale, with nothing running."""
        checked = 0.0
        while not self.done.wait(1.0):
            now = time.time()
            if now - checked >= 5.0:
                checked = now
                if not self.stale and code_stamp() != self.code:
                    self.stale = True
                    log("the code or the settings changed: exiting once no job runs")
            with self.lock:
                busy = self.busy
            if service.JOBS.running():
                with self.lock:
                    self.last = now                      # (idle counts from the last job's end)
                continue
            if busy:
                continue
            if self.stale:
                self.stop("its code changed")
            elif now - self.last >= self.idle * 60.0:
                self.stop("idle %g minutes" % self.idle)


def handler(svc):
    class Handler(BaseHTTPRequestHandler):
        protocol_version = "HTTP/1.1"

        def log_message(self, fmt, *args):               # (each request is not worth a line)
            pass

        def _send(self, status, value):
            data = json.dumps(value, default=str).encode("utf-8")
            self.send_response(status)
            self.send_header("Content-Type", "application/json")
            self.send_header("Content-Length", str(len(data)))
            self.end_headers()
            self.wfile.write(data)

        def _allowed(self):
            host = (self.headers.get("Host") or "").split(":")[0]
            got = (self.headers.get("X-CodeBreaker-Token") or "").encode()
            if host not in ("127.0.0.1", "localhost") or not hmac.compare_digest(got, svc.secret.encode()):
                self._send(401, {"error": "a token is needed"})
                return False
            return True

        def _body(self):
            n = int(self.headers.get("Content-Length") or 0)
            return json.loads(self.rfile.read(n).decode("utf-8")) if n else {}

        def do_GET(self):
            if not self._allowed():
                return
            if self.path == "/ping":
                self._send(200, svc.ping())
            else:
                self._send(404, {"error": "no %s" % self.path})

        def do_POST(self):
            if not self._allowed():
                return
            try:
                body = self._body()
            except ValueError:
                return self._send(400, {"error": "the body is not JSON"})
            if self.path == "/call":
                self._send(200, svc.answer(body))
            elif self.path == "/stop":
                self._send(200, {"stopping": True})
                svc.stop("asked to stop")
            else:
                self._send(404, {"error": "no %s" % self.path})
    return Handler


def log(text):
    sys.stderr.write("%s  %s\n" % (time.strftime("%Y-%m-%d %I:%M:%S %p"), text))
    sys.stderr.flush()


def _write(path, text):
    tmp = path + ".%d.tmp" % os.getpid()
    with open(tmp, "w", encoding="utf-8") as f:
        f.write(text)
    os.replace(tmp, path)


def _bind(first, secret, idle):
    """The server, on the fixed port or the next one free -- or None when a service of this state
    folder already answers (another `serve` won)."""
    for port in range(first, first + 20):
        try:
            svc = Service(port, secret, idle)
            svc.server = Server(("127.0.0.1", port), handler(svc))
            return svc
        except OSError:
            end = time.time() + (10.0 if port == first else 0.0)
            while True:                                  # (one starting beside us: give it time to say so)
                if service.find():
                    return None
                if time.time() >= end:
                    break
                time.sleep(0.25)
    # (all twenty refused: Windows keeps whole ranges of ports for itself -- Hyper-V's and WinNAT's
    # excluded port ranges, a hundred at a time -- and a folder's port can fall inside one. Then any
    # port the system gives: the clients find it in the record, not by working it out)
    try:
        svc = Service(0, secret, idle)
        svc.server = Server(("127.0.0.1", 0), handler(svc))
        svc.port = svc.server.server_address[1]
        return svc
    except OSError:
        pass
    raise OSError("no port free from %d to %d" % (first, first + 19))


def _say(c):
    p = c.ping()
    return ("CodeBreaker service: pid %d on 127.0.0.1:%d, up %dm, %d calls, %d jobs running, idle %ds%s"
            % (p["pid"], p["port"], (time.time() - p["started"]) // 60, p["calls"], p["jobs"], p["idle_for"],
               " (stale: exits once no job runs)" if p["stale"] else ""))


def main(argv):
    """python -m codebreaker serve [--idle MINUTES] [--port N] | --status | --stop"""
    if "--status" in argv:
        c = service.find()
        print(_say(c) if c else "no CodeBreaker service running for %s" % settings.get("state"))
        return 0 if c else 1
    if "--stop" in argv:
        c = service.find()
        if not c:
            print("no CodeBreaker service running")
            return 0
        c.stop()
        print("stopped" if c.wait_gone() else "asked it to stop; it still answers")
        return 0
    c = service.find()
    if c:
        print("already running -- " + _say(c))
        return 0
    idle = float(argv[argv.index("--idle") + 1]) if "--idle" in argv else idle_minutes()
    first = int(argv[argv.index("--port") + 1]) if "--port" in argv else fixed_port()
    secret = secrets.token_urlsafe(32)
    svc = _bind(first, secret, idle)
    if svc is None:
        print("already running -- " + _say(service.find()))
        return 0
    where = service.folder()
    record = os.path.join(where, "service.json")
    _write(os.path.join(where, "token"), secret)
    _write(record, json.dumps({"pid": os.getpid(), "port": svc.port, "root": settings.ROOT,
                               "state": settings.get("state"), "started": svc.started}, indent=1))
    service.LOCAL = True                                 # every call answered here
    sys.stdout = service.Router(sys.stdout)              # what a call prints stays with that call
    log("CodeBreaker service, pid %d, on 127.0.0.1:%d; exits after %g idle minutes" % (os.getpid(), svc.port, idle))
    threading.Thread(target=svc.watch, daemon=True).start()
    try:
        svc.server.serve_forever(poll_interval=0.5)
    except KeyboardInterrupt:
        svc.why = svc.why or "interrupted"
    finally:
        svc.done.set()
        service.JOBS.stop_all()                          # (its own processes only)
        # the record goes before the socket closes: a client that finds the port gone then finds no record
        # (closed first, a client's next look at the record could come before it was taken away)
        try:
            with open(record, encoding="utf-8") as f:
                mine = json.load(f).get("pid") == os.getpid()
        except (OSError, ValueError):
            mine = False
        if mine:
            for name in ("service.json", "token"):
                try:
                    os.remove(os.path.join(where, name))
                except OSError:
                    pass
        svc.server.server_close()
        log("exited: %s" % (svc.why or "stopped"))
    return 0
