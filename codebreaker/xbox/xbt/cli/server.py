"""Headless REST server for XboxTranspiler — drive every command over HTTP for
automation, remote use, or a browser console, without the desktop GUI.

Endpoints (JSON):
  GET  /health            -> {ok, version, commands}
  GET  /commands          -> [{name, desc}, ...]  (the same registry the CLI/GUI use)
  POST /run  {cmd, args}  -> {code, stdout, stderr}   (cmd must be a known command)
  GET  /                  -> a minimal browser console (pick a command, run it)

Security posture — this endpoint can run tools that touch a console and the
filesystem, so it is locked down by default:
  * binds 127.0.0.1 unless you pass --host explicitly,
  * refuses any non-loopback bind unless a --token is set (bearer auth), and
    warns loudly that you are exposing command execution,
  * only runs commands that are in the built-in registry (no arbitrary exec),
  * enforces a per-request timeout.

Stdlib only (http.server) so it works inside the frozen EXE with no wheels.
"""

import json
import os
import subprocess
import sys
import threading
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

DESCRIPTION = "headless REST server: drive every command over HTTP (automation / web console)"

_VERSION = "1.0"


# ── shared with the CLI: the command registry + how to invoke one ──────────
def _commands():
    """[(name, desc)] from the CLI's BUILTINS (single source of truth)."""
    here = os.path.dirname(os.path.abspath(__file__))
    if here not in sys.path:
        sys.path.insert(0, here)
    import xboxtranspiler as xt
    names = {}
    for name, _script, desc in xt.BUILTINS:
        names.setdefault(name, desc)
    return sorted(names.items())


def _dispatch(argv):
    """Command line that runs one XboxTranspiler command (frozen-aware)."""
    if getattr(sys, "frozen", False):
        return [sys.executable] + argv
    entry = os.path.join(os.path.dirname(os.path.abspath(__file__)), "xboxtranspiler.py")
    return [sys.executable, entry] + argv


def run_command(cmd, args, allow, timeout=300):
    """Run one command by name. Returns (code, stdout, stderr). Rejects any name
    not in `allow` (the registry) so the endpoint can't exec arbitrary things."""
    if cmd not in allow:
        return 2, "", "unknown command: %s" % cmd
    argv = [cmd] + [str(a) for a in (args or [])]
    try:
        r = subprocess.run(_dispatch(argv), capture_output=True, text=True,
                           timeout=timeout, encoding="utf-8", errors="replace")
        return r.returncode, r.stdout, r.stderr
    except subprocess.TimeoutExpired:
        return 124, "", "timed out after %ds" % timeout
    except Exception as e:                      # noqa: BLE001
        return 1, "", "%s: %s" % (type(e).__name__, e)


# ── HTTP handler ───────────────────────────────────────────────────────────
_INDEX = """<!doctype html><meta charset=utf-8><title>XboxTranspiler</title>
<style>body{font:14px system-ui;margin:2rem;max-width:60rem}
select,input,button{font:inherit;padding:.3rem}
pre{background:#111;color:#0f0;padding:1rem;overflow:auto;white-space:pre-wrap}</style>
<h1>XboxTranspiler &mdash; headless console</h1>
<p><select id=cmd></select> <input id=args size=50 placeholder="arguments (space separated)">
<button onclick=run()>Run</button></p>
<pre id=out>ready.</pre>
<script>
let tok=new URLSearchParams(location.search).get('token')||'';
fetch('commands').then(r=>r.json()).then(cs=>{
 let s=document.getElementById('cmd');
 for(const c of cs){let o=document.createElement('option');o.value=c.name;
   o.textContent=c.name+' — '+c.desc;s.appendChild(o);}});
function run(){
 let cmd=document.getElementById('cmd').value;
 let args=document.getElementById('args').value.trim();
 args=args? args.split(/\\s+/):[];
 document.getElementById('out').textContent='running '+cmd+'...';
 fetch('run',{method:'POST',headers:{'Content-Type':'application/json',
   'Authorization':tok?'Bearer '+tok:''},body:JSON.stringify({cmd,args})})
 .then(r=>r.json()).then(j=>{document.getElementById('out').textContent=
   '$ '+cmd+' '+args.join(' ')+'  (exit '+j.code+')\\n\\n'+(j.stdout||'')+
   (j.stderr?('\\n[stderr]\\n'+j.stderr):'');})
 .catch(e=>document.getElementById('out').textContent='error: '+e);}
</script>"""


def make_handler(token, timeout):
    allow = {n for n, _ in _commands()}

    class Handler(BaseHTTPRequestHandler):
        server_version = "XboxTranspiler/" + _VERSION

        def log_message(self, *a):             # quiet by default
            pass

        def _send(self, code, body, ctype="application/json"):
            data = body if isinstance(body, bytes) else body.encode("utf-8")
            self.send_response(code)
            self.send_header("Content-Type", ctype)
            self.send_header("Content-Length", str(len(data)))
            self.end_headers()
            self.wfile.write(data)

        def _json(self, code, obj):
            self._send(code, json.dumps(obj))

        def _auth_ok(self):
            if not token:
                return True
            hdr = self.headers.get("Authorization", "")
            return hdr == "Bearer " + token

        def do_GET(self):
            path = self.path.split("?", 1)[0]
            if path == "/health":
                return self._json(200, {"ok": True, "version": _VERSION,
                                        "commands": len(allow)})
            if path == "/commands":
                if not self._auth_ok():
                    return self._json(401, {"error": "unauthorized"})
                return self._json(200, [{"name": n, "desc": d} for n, d in _commands()])
            if path in ("/", "/index.html"):
                return self._send(200, _INDEX, "text/html; charset=utf-8")
            return self._json(404, {"error": "not found"})

        def do_POST(self):
            if self.path.split("?", 1)[0] != "/run":
                return self._json(404, {"error": "not found"})
            if not self._auth_ok():
                return self._json(401, {"error": "unauthorized"})
            try:
                n = int(self.headers.get("Content-Length", 0))
                body = json.loads(self.rfile.read(n) or b"{}")
            except Exception:
                return self._json(400, {"error": "bad JSON body"})
            cmd = body.get("cmd", "")
            args = body.get("args", [])
            if not isinstance(args, list):
                return self._json(400, {"error": "args must be a list"})
            code, out, err = run_command(cmd, args, allow, timeout)
            return self._json(200, {"code": code, "stdout": out, "stderr": err})

    return Handler


def serve(host="127.0.0.1", port=8760, token=None, timeout=300):
    """Start the server (blocking). Returns the HTTPServer if start_only."""
    httpd = ThreadingHTTPServer((host, port), make_handler(token, timeout))
    return httpd


def _is_loopback(host):
    return host in ("127.0.0.1", "::1", "localhost")


# ── self-verification ──────────────────────────────────────────────────────
def _selfcheck():
    """Start on an ephemeral port, hit /health, /commands and /run in-process."""
    import urllib.request
    httpd = ThreadingHTTPServer(("127.0.0.1", 0), make_handler(None, 60))
    port = httpd.server_address[1]
    t = threading.Thread(target=httpd.serve_forever, daemon=True)
    t.start()
    try:
        base = "http://127.0.0.1:%d" % port

        def get(p):
            with urllib.request.urlopen(base + p, timeout=30) as r:
                return json.loads(r.read())

        h = get("/health")
        if not h.get("ok") or h.get("commands", 0) < 50:
            return False, "health bad: %r" % h
        cs = get("/commands")
        names = {c["name"] for c in cs}
        if "demangle" not in names or "ppcdecomp" not in names:
            return False, "registry missing expected commands"
        # run a fast, side-effect-free command through /run
        req = urllib.request.Request(
            base + "/run", method="POST",
            data=json.dumps({"cmd": "demangle", "args": ["?foo@@YAHXZ"]}).encode(),
            headers={"Content-Type": "application/json"})
        with urllib.request.urlopen(req, timeout=60) as r:
            res = json.loads(r.read())
        if res["code"] != 0 or "foo" not in res["stdout"]:
            return False, "/run demangle failed: %r" % res
        # unknown command must be rejected, not executed
        req2 = urllib.request.Request(
            base + "/run", method="POST",
            data=json.dumps({"cmd": "rm", "args": ["-rf", "/"]}).encode(),
            headers={"Content-Type": "application/json"})
        with urllib.request.urlopen(req2, timeout=30) as r:
            res2 = json.loads(r.read())
        if res2["code"] != 2 or "unknown command" not in res2["stderr"]:
            return False, "allowlist did not reject unknown command: %r" % res2
        return True, "health + %d commands + /run demangle + allowlist reject" % len(cs)
    finally:
        httpd.shutdown()


def main():
    a = sys.argv[1:]
    if a and a[0] in ("-h", "--help"):
        print("usage: server [--host H] [--port N] [--token T] [--timeout S]\n"
              "  default: 127.0.0.1:8760 (loopback only). A non-loopback --host\n"
              "  REQUIRES --token (bearer auth) — it exposes command execution.\n"
              "  server --selftest")
        return
    if a and a[0] == "--selftest":
        ok, msg = _selfcheck()
        print(("PASS: " if ok else "FAIL: ") + msg)
        sys.exit(0 if ok else 1)

    def opt(flag, default):
        return a[a.index(flag) + 1] if flag in a else default

    host = opt("--host", "127.0.0.1")
    port = int(opt("--port", "8760"))
    token = opt("--token", None)
    timeout = int(opt("--timeout", "300"))
    if not _is_loopback(host) and not token:
        print("REFUSING to bind %s without --token: that would expose command\n"
              "execution to the network. Pass --token <secret> to allow it." % host)
        sys.exit(2)
    if not _is_loopback(host):
        print("WARNING: binding %s exposes command execution to the network.\n"
              "         Clients must send  Authorization: Bearer <token>." % host)
    httpd = serve(host, port, token, timeout)
    url = "http://%s:%d/" % ("localhost" if _is_loopback(host) else host, port)
    print("XboxTranspiler server on %s  (%d commands%s)" %
          (url, len(_commands()), ", token auth" if token else ""))
    print("  GET /health  GET /commands  POST /run {cmd,args}   Ctrl-C to stop")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nstopped.")
        httpd.shutdown()


if __name__ == "__main__":
    main()
