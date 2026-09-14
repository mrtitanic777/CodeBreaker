#!/usr/bin/env python3
"""apiserver.py -- a localhost HTTP API exposing ALL of CodeBreaker's capabilities,
the way XEXMagic serves its engine over localhost.

Every `wt` command is callable through one endpoint (POST /api/run {"argv":[...]}),
so the whole toolset -- every subsystem and every per-era target command -- is
reachable over HTTP without a per-command wrapper. Typed convenience routes wrap
the common native-engine operations, and a small UI lets you drive it from a
browser. Binds 127.0.0.1 by default; commands run as `python wt.py <argv>`
subprocesses (argv passed as a list -- no shell, no injection).

    wt serve [--port 8756] [--host 127.0.0.1]
"""
import argparse
import json
import os
import subprocess
import sys
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse, parse_qs

from src.core import paths

WT = os.path.join(paths.repo_root(), "wt.py")

# Discovery: the wt subsystems + the native-engine convenience routes. `wt <sub>
# list` / `wt --target <era> list` enumerate deeper; run them via /api/run.
CAPABILITIES = {
    "run_any": {"POST /api/run": {"body": {"argv": ["--target", "win95", "decompile", "LZEXPAND"]},
                                  "note": "runs any wt command; returns {argv,exit,stdout,stderr}"}},
    "subsystems": ["recompile", "behavior", "inverse", "msc6", "xbox", "diff", "backbone", "sdk"],
    "targets": ["win1.0..win98 (use --target <era>)"],
    "win95_commands": ["decompile", "disasm", "ne_scale", "origin_triage", "oracle_equiv",
                       "autobuild95", "classify32", "..."],
    "typed_routes": {
        "GET /api/health": "liveness",
        "GET /api/capabilities": "this document",
        "GET /api/win95/decompile?module=<name|path>": "native decompiler -> C",
        "GET /api/win95/disasm?module=<name|path>": "native disassembler -> listing",
        "GET /api/win95/list": "wt --target win95 list",
    },
}

INDEX = """<!doctype html><meta charset=utf-8><title>CodeBreaker API</title>
<style>body{font:14px system-ui;margin:2rem;max-width:60rem}h1{font-size:1.3rem}
input,button,select{font:inherit;padding:.3rem}textarea,pre{width:100%;box-sizing:border-box}
pre{background:#111;color:#ddd;padding:.6rem;overflow:auto;max-height:28rem}code{background:#eee;padding:0 .2rem}</style>
<h1>CodeBreaker &mdash; localhost API</h1>
<p>Run any <code>wt</code> command. Args, space-separated:</p>
<p><input id=a size=70 value="--target win95 disasm LZEXPAND"> <button onclick=run()>Run</button></p>
<pre id=o>ready.</pre>
<p>Endpoints: <code>POST /api/run</code> {argv:[...]}, <code>GET /api/capabilities</code>,
<code>GET /api/win95/decompile?module=</code>, <code>GET /api/win95/disasm?module=</code></p>
<script>
async function run(){
 const argv=document.getElementById('a').value.trim().split(/\\s+/);
 document.getElementById('o').textContent='running...';
 const r=await fetch('/api/run',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({argv})});
 const j=await r.json();
 document.getElementById('o').textContent='exit '+j.exit+'\\n\\n'+(j.stdout||'')+(j.stderr?('\\n[stderr]\\n'+j.stderr):'');
}
</script>"""


def run_wt(argv, timeout=900):
    r = subprocess.run([sys.executable, WT] + list(argv),
                       capture_output=True, text=True, timeout=timeout)
    return {"argv": list(argv), "exit": r.returncode, "stdout": r.stdout, "stderr": r.stderr}


class Handler(BaseHTTPRequestHandler):
    def log_message(self, *a):
        pass

    def _send(self, code, body, ctype="application/json"):
        b = body if isinstance(body, bytes) else body.encode("utf-8", "replace")
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(b)))
        self.end_headers()
        self.wfile.write(b)

    def do_GET(self):
        u = urlparse(self.path); q = parse_qs(u.query)
        if u.path == "/":
            return self._send(200, INDEX, "text/html; charset=utf-8")
        if u.path == "/api/health":
            return self._send(200, json.dumps({"ok": True}))
        if u.path == "/api/capabilities":
            return self._send(200, json.dumps(CAPABILITIES, indent=1))
        if u.path == "/api/win95/list":
            return self._send(200, json.dumps(run_wt(["--target", "win95", "list"])))
        if u.path in ("/api/win95/decompile", "/api/win95/disasm"):
            m = (q.get("module") or [None])[0]
            if not m:
                return self._send(400, json.dumps({"error": "module= required"}))
            cmd = "decompile" if u.path.endswith("decompile") else "disasm"
            return self._send(200, json.dumps(run_wt(["--target", "win95", cmd, m])))
        return self._send(404, json.dumps({"error": "not found", "path": u.path}))

    def do_POST(self):
        u = urlparse(self.path)
        if u.path == "/api/run":
            try:
                n = int(self.headers.get("Content-Length", 0) or 0)
                data = json.loads(self.rfile.read(n) or b"{}")
            except (ValueError, json.JSONDecodeError):
                return self._send(400, json.dumps({"error": "invalid JSON body"}))
            argv = data.get("argv")
            if not isinstance(argv, list) or not all(isinstance(x, str) for x in argv):
                return self._send(400, json.dumps({"error": "argv must be a list of strings"}))
            try:
                return self._send(200, json.dumps(run_wt(argv, int(data.get("timeout", 900)))))
            except subprocess.TimeoutExpired:
                return self._send(504, json.dumps({"error": "timeout", "argv": argv}))
        return self._send(404, json.dumps({"error": "not found", "path": u.path}))


def main(argv):
    ap = argparse.ArgumentParser(prog="serve", description=__doc__.splitlines()[0])
    ap.add_argument("--port", type=int, default=8756)
    ap.add_argument("--host", default="127.0.0.1", help="bind address (default localhost only)")
    a = ap.parse_args(argv)
    srv = ThreadingHTTPServer((a.host, a.port), Handler)
    print("CodeBreaker API -> http://%s:%d   (UI at /, POST /api/run {argv:[...]})" % (a.host, a.port),
          flush=True)
    try:
        srv.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        srv.server_close()


if __name__ == "__main__":
    main(sys.argv[1:])
