r"""client.py -- the terminal interface's whole link to CodeBreaker: the native service.

The terminal (ui/terminal.py) shows what the CodeBreaker service answers and nothing else. This
file is its client: stdlib only, no other module of the package, so the interface runs with just
codebreaker/ui in front of it.

    call(name, **args)     one call of the service by its name (`functions`, `store.overview`,
                           `store.latest`, `jobs.*`, or `mcp.<tool>`); raises Unavailable when the
                           service cannot be reached or started, ValueError / OSError / LookupError
                           for the failures the service reports, RemoteError for any other
    connect(force=False)   the client of the running service, started when it is not running;
                           Unavailable (with the reason, in words) when that cannot be done
    stop_mine()            the jobs this process started, stopped
    OWNER                  whose jobs these are (the service keeps them per client)

The service is `codebreaker serve` (native/cb/ui/serve.*: HTTP and JSON on 127.0.0.1, the token in
<state>/service/token, its record in <state>/service/service.json). It is started on demand with
the program found by program(): CB_EXE, the setting `exe` of settings.json, the program the
native build leaves in state/cxx/codebreaker/build, or `codebreaker` on the PATH. When no service
answers there is no other way to the answers: the interface says so and offers to try again.
"""
import http.client
import json
import os
import shutil
import subprocess
import threading
import time

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OWNER = "pid-%d" % os.getpid()          # whose jobs: this process's
START_WAIT = 60.0                       # seconds a started service has to answer


class Unavailable(RuntimeError):
    """The service cannot be reached, and could not be started. str() says why and what to do."""


class Gone(Exception):
    """A request found the service gone (not running, or it went away)."""


class RemoteError(RuntimeError):
    """A failure in the service that is not one the callers expect (ValueError, OSError, LookupError)."""


_RAISE = {"ValueError": ValueError, "OSError": OSError, "LookupError": LookupError}


# ---- where things are (what the interface needs of settings.json, no more) ------------------------
_SETTINGS = None


def setting(key):
    """One setting of settings.json (the environment's CB_<KEY> wins); a relative path is the checkout's."""
    global _SETTINGS
    if _SETTINGS is None:
        try:
            with open(os.path.join(ROOT, "settings.json"), encoding="utf-8") as f:
                _SETTINGS = json.load(f)
        except (OSError, ValueError):
            _SETTINGS = {}
    value = os.environ.get("CB_" + key.upper()) or _SETTINGS.get(key) or ("state" if key == "state" else "")
    if value and key in ("state", "exe") and not os.path.isabs(value):
        value = os.path.normpath(os.path.join(ROOT, value))
    return value


def folder():
    """<state>/service: the running service's record (service.json), its token, its log."""
    path = os.path.join(setting("state"), "service")
    os.makedirs(path, exist_ok=True)
    return path


def candidates():
    """The places the program is looked for, in order."""
    tail = "codebreaker.exe" if os.name == "nt" else "codebreaker"
    found = [setting("exe"), os.path.join(ROOT, "state", "cxx", "codebreaker", "build", tail), shutil.which("codebreaker")]
    return [p for p in found if p]


def program():
    """The CodeBreaker program that runs `serve`: the first of candidates() that exists, or None."""
    return next((p for p in candidates() if os.path.isfile(p)), None)


def record():
    """The running service's record {pid, port, root, state, started, ...}, or None."""
    try:
        with open(os.path.join(folder(), "service.json"), encoding="utf-8") as f:
            return json.load(f)
    except (OSError, ValueError):
        return None


def _same(a, b):
    return os.path.normcase(os.path.abspath(a or "")) == os.path.normcase(os.path.abspath(b or ""))


# ---- the wire --------------------------------------------------------------------------------------
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
    """A client of the service running for this state folder, or None. A service whose program
    changed under it (stale) and that runs no job is told to go, and None is returned."""
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
    if not _same(pong.get("state"), setting("state")):
        return None
    if pong.get("stale") and not pong.get("jobs"):
        c.stop()
        c.wait_gone()
        return None
    return c


LAUNCHED = []                    # pids of services this process started (tests stop them)


def _log_tail(path, lines=6):
    try:
        with open(path, "rb") as f:
            f.seek(0, os.SEEK_END)
            f.seek(max(0, f.tell() - 4000))
            text = f.read().decode("utf-8", "replace")
    except OSError:
        return ""
    return "\n".join(text.strip().splitlines()[-lines:])


def launch(wait=None):
    """Start the service (`<program> serve`) as a process of its own, out of this one's console
    and job, and wait until it answers. -> its client. Raises Unavailable, saying why not."""
    exe = program()
    if exe is None:
        raise Unavailable("CodeBreaker's program was not found, so the service cannot be started.\n"
                          "Looked for it at:\n  " + "\n  ".join(candidates() or ["(nowhere: set CB_EXE or `exe` in settings.json)"])
                          + "\nBuild it, or point CB_EXE at it, then retry.")
    log_path = os.path.join(folder(), "serve.log")
    argv = [exe, "serve"]
    kw = dict(cwd=ROOT, stdin=subprocess.DEVNULL)
    with open(log_path, "ab") as log:
        kw.update(stdout=log, stderr=subprocess.STDOUT)
        try:
            if os.name == "nt":
                flags = 0x08000000 | 0x00000200          # no window, a process group of its own
                try:
                    proc = subprocess.Popen(argv, creationflags=flags | 0x01000000, **kw)   # out of our job
                except OSError:
                    proc = subprocess.Popen(argv, creationflags=flags, **kw)
            else:
                proc = subprocess.Popen(argv, start_new_session=True, **kw)
        except OSError as e:
            raise Unavailable("the service could not be started: %s\n(%s serve)" % (e, exe))
    LAUNCHED.append(proc.pid)
    end = time.time() + (START_WAIT if wait is None else wait)
    while time.time() < end:
        c = find()
        if c:
            c.started = c.pid == proc.pid
            return c
        if proc.poll() is not None:                  # it found another one running, or failed
            c = find()
            if c:
                return c
            raise Unavailable("the service started and stopped again (exit %s).\n%s\nIts log: %s"
                              % (proc.returncode, _log_tail(log_path), log_path))
        time.sleep(0.2)
    raise Unavailable("the service was started (%s serve) but did not answer within %d seconds.\nIts log: %s"
                      % (exe, int(START_WAIT if wait is None else wait), log_path))


# ---- the one client of this process ----------------------------------------------------------------
_CLIENT = None
_FAILED = None                   # (when, the Unavailable) of the last attempt that failed
_LOCK = threading.Lock()
RETRY_AFTER = 5.0                # seconds before a failed start is tried again without being asked


def connect(force=False):
    """The service's client, started when it is not running. Raises Unavailable. A failed attempt
    is not repeated for a few seconds (every caller would start the program again), unless
    `force`: a person asking to retry."""
    global _CLIENT, _FAILED
    with _LOCK:
        if _CLIENT is not None and not force:
            return _CLIENT
        if _FAILED and not force and time.time() - _FAILED[0] < RETRY_AFTER:
            raise _FAILED[1]
        try:
            _CLIENT = find() or launch()
        except Unavailable as e:
            _CLIENT, _FAILED = None, (time.time(), e)
            raise
        _FAILED = None
        return _CLIENT


def connected():
    """True when this process holds a client of the service (it may still have gone since)."""
    return _CLIENT is not None


def _drop():
    global _CLIENT, _FAILED
    with _LOCK:
        _CLIENT, _FAILED = None, None


def call(name, /, **args):
    """What the service answers to `name` with `args`. A service that has gone away is looked for
    (and started) once more; Unavailable when it cannot be had."""
    for attempt in range(2):
        c = connect()
        try:
            return c.call(name, args)
        except Gone as e:
            _drop()                                  # gone: look again (start it again) once
            if attempt:
                raise Unavailable("the service went away: %s" % e)
    raise Unavailable("the service cannot be reached")


def stop_mine():
    """The jobs this process started, stopped (in the service). Never starts it for that."""
    c = _CLIENT
    if c is not None:
        try:
            c.call("jobs.stop_all", {"owner": OWNER}, timeout=10.0)
        except (Gone, OSError, RemoteError, ValueError, LookupError):
            pass
