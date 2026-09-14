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
    "win95_commands": ["decompile", "disasm", "ne_scale", "pe_scale", "le_scale",
                       "mz_scale", "origin_triage", "oracle_equiv", "autobuild95",
                       "classify32", "..."],
    "typed_routes": {
        "GET /api/health": "liveness",
        "GET /api/capabilities": "this document",
        "GET /api/win95/decompile?module=<name|path>": "native decompiler -> C",
        "GET /api/win95/disasm?module=<name|path>": "native disassembler -> listing",
        "GET /api/win95/list": "wt --target win95 list",
        "GET /api/pe/parse?module=<name|path>": "full PE parse (headers, dirs, sections, exports, imports, resources, ...)",
        "GET /api/pe/hex?module=<name|path>&off=<n>&len=<n>": "raw hex dump window",
    },
}

_WEBUI = os.path.join(os.path.dirname(os.path.abspath(__file__)), "webui.html")
_FALLBACK = ("<!doctype html><title>CodeBreaker API</title><h1>CodeBreaker API</h1>"
             "<p>webui.html missing. POST /api/run {argv:[...]}, GET /api/capabilities.</p>")


def index_html():
    try:
        with open(_WEBUI, encoding="utf-8") as f:
            return f.read()
    except OSError:
        return _FALLBACK


_CORPUS = os.environ.get("WT_DATA_ROOT") or \
    r"C:\Users\mrtit\Documents\GitHub\Windows95\design\corpus\files"
_mod_index = None


def _resolve_module(m):
    """A module name (kernel32.dll) or a full path -> an absolute file path. Prefers
    the installed WINDOWS\\SYSTEM copy when a name matches more than once."""
    if not m:
        return None
    if os.path.isfile(m):
        return m
    global _mod_index
    if _mod_index is None:
        _mod_index = {}
        for dp, _, fs in os.walk(_CORPUS):
            for f in fs:
                k = f.lower()
                prefer = "system" in dp.lower().replace("\\", "/").split("/")
                if k not in _mod_index or prefer:
                    _mod_index[k] = os.path.join(dp, f)
    return _mod_index.get(m.lower())


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
            return self._send(200, index_html(), "text/html; charset=utf-8")
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
        if u.path == "/api/pe/parse":
            m = (q.get("module") or [None])[0]
            p = _resolve_module(m)
            if not p:
                return self._send(404, json.dumps({"error": "module not found", "module": m}))
            from src.formats import peinfo
            return self._send(200, json.dumps(peinfo.parse(p)))
        if u.path == "/api/pe/hex":
            m = (q.get("module") or [None])[0]
            p = _resolve_module(m)
            if not p:
                return self._send(404, json.dumps({"error": "module not found", "module": m}))
            from src.formats import peinfo
            off = int((q.get("off") or ["0"])[0], 0)
            ln = min(int((q.get("len") or ["512"])[0], 0), 65536)
            return self._send(200, json.dumps({"module": m, "offset": off,
                                               "dump": peinfo.hexdump(p, off, ln)}))
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
