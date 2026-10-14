r"""server.py -- the local web console: look at any module, and at how the reconstruction stands.

    python -m codebreaker serve                 http://127.0.0.1:8765/ (the next free port if taken)
    python -m codebreaker serve --port N        that port, or fail
    python -m codebreaker serve --open          open the page in the default browser as well

One page (console.html, beside this file; nothing is fetched from anywhere else) over a small
JSON interface. Everything it shows is read from the same places the commands use:

    GET  /api/health                             where the corpus and the store are
    GET  /api/coverage                           the scoreboard: the coverage ledger, whole
    GET  /api/modules                            every corpus module with its ledger numbers
    GET  /api/info?module=M[&section=S]          the inspector (formats/inspect.py)
    GET  /api/strings?module=M[&min=4&find=T&offset=0&limit=500]
    GET  /api/hex?module=M[&offset=0&length=1024]
    GET  /api/resource?module=M&offset=N&size=N&type=T[&wide=0]    a picture, or {kind, text}
    GET  /api/functions?module=M                 every function: class, name, why (the store)
    GET  /api/function?module=M&addr=A           one function: its source and the verdict's detail
    GET  /api/progress                           what `progress` shows
    GET  /api/jobs    GET /api/job?id=J[&offset=N]    GET /api/job/output?id=J
    POST /api/jobs {"action": "decompile" | "measure", "module": M}      start a background job
    POST /api/jobs/stop {"id": J}                stop a job this console started

M is a module name of the corpus or a file path. A job is the same command a person would type
(`python -m codebreaker decompile M`, `... measure M`), run as its own process below normal
priority with its output kept in state/ui_jobs/; nothing else can be started from the page.
The console listens on 127.0.0.1 only and answers only requests addressed to that name, and it
reads the function store read-only (only a `measure` job writes results, as the command does).
"""
import json
import os
import subprocess
import sys
import threading
import time
import urllib.parse
import webbrowser
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

from codebreaker.base import corpus, settings
from codebreaker.coverage import ledger
from codebreaker.formats import inspect
from codebreaker.source import module_writer
from codebreaker.store import functions as F
from codebreaker.ui import progress

PORT = 8765
PAGE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "console.html")
ACTIONS = {"decompile": "decompile it to C (nothing is built or judged)",
           "measure": "decompile, build and judge every function; results go to the store"}


class Jobs(object):
    """Commands started from a user interface, each its own process with its output in a file."""

    def __init__(self):
        self.lock = threading.Lock()
        self.jobs = {}                                           # id -> dict
        self.count = 0

    def start(self, action, module):
        """Start `action` on a module. -> the job, or raises ValueError saying why not."""
        path = corpus.retail(module) if module else None
        if action not in ACTIONS:
            raise ValueError("no such action %r" % action)
        if not path:
            raise ValueError("no module %r in the corpus" % module)
        name = os.path.basename(path).lower()
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
            output = os.path.join(folder, os.path.splitext(name)[0] + ".c") if action == "decompile" else ""
            argv = [sys.executable, "-u", "-m", "codebreaker", action, path] + (["--out", output] if output else [])
            with open(log, "wb") as f:
                proc = subprocess.Popen(argv, cwd=settings.ROOT, stdout=f, stderr=subprocess.STDOUT,
                                        stdin=subprocess.DEVNULL,
                                        creationflags=0x00004000 if os.name == "nt" else 0)   # below normal
            self.jobs[jid] = {"id": jid, "action": action, "module": name, "proc": proc, "log": log,
                              "output": output, "started": time.time(), "ended": None}
            return self.view(self.jobs[jid])

    def view(self, job, offset=None):
        """A job as plain data; with `offset`, also its output from that byte on."""
        code = job["proc"].poll()
        if code is not None and job["ended"] is None:
            job["ended"] = time.time()
        out = {"id": job["id"], "action": job["action"], "module": job["module"], "pid": job["proc"].pid,
               "status": "running" if code is None else "done" if code == 0 else "failed", "exit": code,
               "seconds": int((job["ended"] or time.time()) - job["started"]),
               "started": time.strftime("%H:%M:%S", time.localtime(job["started"])),
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

    def stop_all(self):
        for jid in list(self.jobs):
            self.stop(jid)


JOBS = Jobs()
_memo = {}                                                       # name -> (when, value)
_memo_lock = threading.Lock()


def _ledger(age=4.0):
    """The coverage ledger, worked out at most once every few seconds however many ask."""
    with _memo_lock:
        got = _memo.get("ledger")
        if not got or time.time() - got[0] > age:
            got = (time.time(), ledger.measure())
            _memo["ledger"] = got
        return got[1]


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
    detail and its weak reasons spelled out}."""
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


class Refused(Exception):
    def __init__(self, code, message):
        Exception.__init__(self, message)
        self.code = code


class Handler(BaseHTTPRequestHandler):
    server_version = "CodeBreakerConsole"
    protocol_version = "HTTP/1.1"

    def log_message(self, *args):                                # quiet: the page polls
        pass

    # ---- plumbing ----------------------------------------------------------------------------
    def _send(self, code, body, kind="application/json"):
        data = body if isinstance(body, bytes) else body.encode("utf-8", "replace")
        self.send_response(code)
        self.send_header("Content-Type", kind)
        self.send_header("Content-Length", str(len(data)))
        self.send_header("Cache-Control", "no-store")
        self.send_header("X-Content-Type-Options", "nosniff")
        if kind.startswith("text/html"):
            self.send_header("Content-Security-Policy", "default-src 'self'; style-src 'unsafe-inline'; "
                             "script-src 'unsafe-inline'; img-src 'self' data:; frame-ancestors 'none'")
        self.end_headers()
        self.wfile.write(data)

    def _json(self, value, code=200):
        self._send(code, json.dumps(value))

    def _local(self):
        """Only requests addressed to this machine's own name are answered, so a page on some
        other site cannot reach the console through a name that merely points here."""
        port = self.server.server_address[1]
        allowed = {"127.0.0.1:%d" % port, "localhost:%d" % port}
        if (self.headers.get("Host") or "") not in allowed:
            raise Refused(403, "this console answers only on 127.0.0.1")
        origin = self.headers.get("Origin")
        if origin and urllib.parse.urlparse(origin).netloc not in allowed:
            raise Refused(403, "requests from another site are not accepted")

    def _module(self, q):
        name = (q.get("module") or [""])[0]
        path = corpus.retail(name) if name else None
        if not path:
            raise Refused(404, "no module %r in the corpus" % name)
        return path

    @staticmethod
    def _int(q, name, default, most=None):
        try:
            value = int((q.get(name) or [str(default)])[0], 0)
        except ValueError:
            raise Refused(400, "%s must be a number" % name)
        return max(0, min(value, most) if most is not None else value)

    def _answer(self, work):
        try:
            self._local()
            work()
        except Refused as e:
            self._json({"error": str(e)}, e.code)
        except (BrokenPipeError, ConnectionError):
            pass
        except Exception as e:                                    # say what failed; keep serving
            self._json({"error": "%s: %s" % (type(e).__name__, str(e)[:200])}, 500)

    # ---- reading -----------------------------------------------------------------------------
    def do_GET(self):
        self._answer(self._get)

    def _get(self):
        u = urllib.parse.urlparse(self.path)
        q = urllib.parse.parse_qs(u.query)
        route = u.path
        if route == "/":
            with open(PAGE, "rb") as f:
                return self._send(200, f.read(), "text/html; charset=utf-8")
        if route == "/api/health":
            return self._json({"ok": True, "corpus": corpus.root(), "store": F.default_path(),
                               "store_exists": os.path.isfile(F.default_path()),
                               "actions": ACTIONS, "classes": [{"key": k, "title": F.TITLE[k]} for k in F.ORDER],
                               "kinds": [{"key": k, "title": t} for k, t in ledger.KINDS]})
        if route == "/api/coverage":
            return self._json(_ledger())
        if route == "/api/modules":
            keep = ("name", "kind", "size", "in_store", "measured", "judged") + ledger.COUNTS
            return self._json({"modules": [{k: r[k] for k in keep} for r in _ledger()["modules"]]})
        if route == "/api/progress":
            return self._json(progress.snapshot())
        if route == "/api/jobs":
            return self._json({"jobs": JOBS.all()})
        if route in ("/api/job", "/api/job/output"):
            job = JOBS.get((q.get("id") or [""])[0])
            if not job:
                raise Refused(404, "no such job")
            if route == "/api/job":
                return self._json(JOBS.view(job, self._int(q, "offset", 0)))
            if not (job["output"] and os.path.isfile(job["output"])):
                raise Refused(404, "this job wrote no file")
            with open(job["output"], "rb") as f:
                text = f.read(4 << 20).decode("utf-8", "replace")
            return self._json({"id": job["id"], "module": job["module"], "path": job["output"],
                               "size": os.path.getsize(job["output"]), "text": text})
        if route in ("/api/functions", "/api/function"):
            name = os.path.basename(self._module(q))
            addr = (q.get("addr") or [None])[0] if route == "/api/function" else None
            if route == "/api/function" and not addr:
                raise Refused(400, "addr= is needed")
            found = functions(name, addr)
            if found is None:
                raise Refused(404, "%s is not a module of the corpus or the store" % name)
            return self._json(found)
        path = self._module(q) if route in ("/api/info", "/api/strings", "/api/hex", "/api/resource") else None
        if route == "/api/info":
            info = inspect.describe(path)
            section = (q.get("section") or [""])[0]
            if section and section not in info:
                raise Refused(404, "no part %r" % section)
            return self._json(info[section] if section else info)
        if route == "/api/strings":
            find = (q.get("find") or [""])[0].lower()
            rows = inspect.strings(path, max(2, self._int(q, "min", 4, 64)))
            if find:
                rows = [r for r in rows if find in r["text"].lower()]
            at, n = self._int(q, "offset", 0), self._int(q, "limit", 500, 5000)
            return self._json({"total": len(rows), "offset": at, "rows": rows[at:at + n]})
        if route == "/api/hex":
            at, n = self._int(q, "offset", 0), self._int(q, "length", 1024, 65536)
            return self._json({"offset": at, "length": n, "size": os.path.getsize(path),
                               "text": inspect.hexdump(path, at, n)})
        if route == "/api/resource":
            body = inspect.resource_body(path, self._int(q, "offset", 0), self._int(q, "size", 0),
                                         self._int(q, "type", 0), (q.get("wide") or ["1"])[0] != "0")
            if body["kind"] == "image":
                return self._send(200, body["bytes"], body["content_type"])
            return self._json(body)
        raise Refused(404, "nothing at %s" % route)

    # ---- acting ------------------------------------------------------------------------------
    def do_POST(self):
        self._answer(self._post)

    def _post(self):
        if (self.headers.get("Content-Type") or "").split(";")[0].strip() != "application/json":
            raise Refused(415, "send JSON")
        try:
            body = json.loads(self.rfile.read(min(int(self.headers.get("Content-Length") or 0), 1 << 16)) or b"{}")
        except ValueError:
            raise Refused(400, "the body is not JSON")
        if not isinstance(body, dict):
            raise Refused(400, "the body must be an object")
        route = urllib.parse.urlparse(self.path).path
        if route == "/api/jobs":
            try:
                return self._json(JOBS.start(str(body.get("action") or ""), str(body.get("module") or "")))
            except ValueError as e:
                raise Refused(409 if str(e).startswith("busy") else 400, str(e))
        if route == "/api/jobs/stop":
            if not JOBS.stop(str(body.get("id") or "")):
                raise Refused(404, "no such job")
            return self._json({"ok": True})
        raise Refused(404, "nothing at %s" % route)


class _Server(ThreadingHTTPServer):
    allow_reuse_address = False          # on Windows "reuse" would let two consoles share one port
    daemon_threads = True


def make_server(port=PORT, search=True):
    """A console server bound to 127.0.0.1 -- on `port`, or when `search` on the first free port
    of the twenty from it. Call .serve_forever() on it; .server_address[1] is the port it got."""
    last = None
    for p in range(port, port + (20 if search else 1)):
        try:
            return _Server(("127.0.0.1", p), Handler)
        except OSError as e:
            last = e
    raise SystemExit("no free port from %d: %s" % (port, last))


def main(argv):
    """python -m codebreaker serve [--port N] [--open]

    Serve the web console on 127.0.0.1 until interrupted."""
    port = int(argv[argv.index("--port") + 1]) if "--port" in argv else PORT
    server = make_server(port, search="--port" not in argv)
    url = "http://127.0.0.1:%d/" % server.server_address[1]
    print("CodeBreaker console at %s  (process %d; Ctrl+C stops it)" % (url, os.getpid()), flush=True)
    if "--open" in argv:
        webbrowser.open(url)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        JOBS.stop_all()
        server.server_close()
    return 0
