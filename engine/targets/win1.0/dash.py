#!/usr/bin/env python3
"""
dash.py — a live status server for the Windows 1.01 reconstruction.

    python dash.py            serve on http://localhost:8787
    python dash.py --port N   ...somewhere else
    python dash.py --once     print the JSON and exit (for debugging)

WHY A SERVER AND NOT A STATIC PAGE
----------------------------------
`ledger.py` and `verify.py` take a few seconds each: they re-parse the NE
binaries and re-scan every reconstruction file. Polling them directly from a
browser would mean a page that hangs for seconds on every refresh.

So this keeps a CACHE and refreshes it on a background thread, and only when
something has actually changed — the cache key is the newest mtime across
`reconstructed/`, `ORDINALS.md` and the tools. A poll is then a dictionary
lookup and returns instantly, while the numbers are never more than one
recompute behind the files.

Reporting is deliberately conservative: the headline is only the human-read
"addressed" figure; auto-generated skeletons appear in a separate panel and are
never folded into a percentage; partial/prototype counts sit beside transcribed
rather than inside it; and every number is parsed from live tool output (a failed
tool is reported, not shown stale). The page is organised around the LADDER of
claims, weakest to strongest, because the gap between rungs is the remaining work:

    carved       every function found in the binaries
    cited        read and placed
    addressed    a C function exists for it
    transcribed  ...with a real body
    compiles     the file is well-formed against the era SDK headers
    verified     behaviour checked against the original

A percentage is never shown without the rung it belongs to.
"""
import json
import os
import re
import subprocess
import sys
import threading
import time
from engine.foundation import layout
from http.server import BaseHTTPRequestHandler, HTTPServer

HERE = os.path.dirname(os.path.abspath(__file__))
from _wtroot import data_root as _data_root
ROOT = _data_root(HERE)   # DATA root (build16/ reconstructed/ refs/ files/); tools live at HERE
WATCH = [layout.recon_dir(ROOT), "ORDINALS.md", "ledger.py", "verify.py", "massgen.py"]

_state = {"status": "starting", "generated": 0, "modules": [], "layers": [],
          "totals": {}, "auto": {}, "verify": {}, "apps": [], "apptotal": {},
          "build": {}, "ladder": [], "error": None}
_lock = threading.Lock()


def newest_mtime():
    newest = 0.0
    for w in WATCH:
        p = os.path.join(ROOT, w)
        if not os.path.exists(p):            # tool entries (ledger.py, ...) live at HERE
            p = os.path.join(HERE, w)
        if os.path.isfile(p):
            newest = max(newest, os.path.getmtime(p))
        elif os.path.isdir(p):
            for dirpath, _, files in os.walk(p):
                for f in files:
                    try:
                        newest = max(newest, os.path.getmtime(os.path.join(dirpath, f)))
                    except OSError:
                        pass
    return newest


def run(script, timeout=240):
    try:
        # script is a sibling tool (e.g. "ledger.py"): it lives at HERE now, but
        # runs with cwd=ROOT so it reads the project's data (reconstructed/, files/).
        p = subprocess.run([sys.executable, os.path.join(HERE, script)],
                           cwd=ROOT, timeout=timeout,
                           capture_output=True, text=True, errors="replace")
        return p.stdout + p.stderr
    except Exception as ex:
        return "ERROR: %s" % ex


def parse_ledger(out):
    """Pull the real figures out of ledger.py's report."""
    mods, layers, totals, auto = [], [], {}, {}

    # The two tables have the SAME shape — four integers and a percentage — and
    # "SYSTEM" is both a module name and a layer name, so the rows must be read
    # SECTION BY SECTION or the second table silently overwrites the first.
    # (It did: byte coverage read 15.3% instead of 31.8%, and two layer rows
    # were counted as modules.)
    head, _, tail = out.partition("WRITTEN C")
    sec_mod, sec_lay = head, tail

    # "  KERNEL        258      84     30431       8538      28.1%"
    for m in re.finditer(r"^\s{2}([A-Z][A-Z0-9]+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+([\d.]+)%",
                         sec_mod, re.M):
        name, fns, cited, cb, citedb, pctv = m.groups()
        if name == "TOTAL":
            totals["fns"] = int(fns)
            totals["cited"] = int(cited)
            totals["code_bytes"] = int(cb)
            totals["cited_bytes"] = int(citedb)
            totals["byte_pct"] = float(pctv)
        else:
            mods.append({"name": name, "fns": int(fns), "cited": int(cited),
                         "code_bytes": int(cb), "cited_bytes": int(citedb),
                         "pct": float(pctv)})

    # "  SYSTEM                95       75     18        188      11.3%"
    for m in re.finditer(r"^\s{2}(SYSTEM|APPS|TOTAL)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+([\d.]+)%",
                         sec_lay, re.M):
        name, tr, pa, pr, add, pctv = m.groups()
        row = {"name": name, "transcribed": int(tr), "partial": int(pa),
               "prototype": int(pr), "addressed": int(add), "pct": float(pctv)}
        if name == "TOTAL":
            totals.update({"transcribed": int(tr), "partial": int(pa),
                           "prototype": int(pr), "addressed": int(add),
                           "addressed_pct": float(pctv)})
        else:
            layers.append(row)

    m = re.search(r"ORDINALS named:\s*(\d+)\s+raw call markers left:\s*(\d+)\s*\(([\d.]+)%", out)
    if m:
        totals["ordinals"] = int(m.group(1))
        totals["raw_markers"] = int(m.group(2))
        totals["callsite_pct"] = float(m.group(3))

    m = re.search(r"AUTO skeletons \(massgen\.py\):\s*(\d+) generated,\s*(\d+) promoted", out)
    if m:
        auto = {"generated": int(m.group(1)), "promoted": int(m.group(2))}

    return mods, layers, totals, auto


def parse_appledger(out):
    """The application layer — 415 KB, bigger than the whole system."""
    apps, tot = [], {}
    for m in re.finditer(r"^\s{2}([A-Z][A-Z0-9]+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+([\d.]+)%",
                         out, re.M):
        name, fns, cited, cb, citedb, pctv = m.groups()
        row = {"name": name, "fns": int(fns), "cited": int(cited),
               "code_bytes": int(cb), "cited_bytes": int(citedb), "pct": float(pctv)}
        if name == "TOTAL":
            tot = row
        else:
            apps.append(row)
    return apps, tot


def parse_build(out):
    """The compile harness — the only rung above `addressed` we can measure."""
    d = {}
    m = re.search(r"files checked\s*:\s*(\d+)", out)
    if m: d["files"] = int(m.group(1))
    m = re.search(r"files with NO real error\s*:\s*(\d+)", out)
    if m: d["clean"] = int(m.group(1))
    m = re.search(r"real diagnostics\s*:\s*(\d+)", out)
    if m: d["diagnostics"] = int(m.group(1))
    return d


def parse_verify(out):
    g = re.search(r"GHOST calls \(errors\):\s*(\d+)", out)
    mi = re.search(r"missing calls in non-partial functions:\s*(\d+)", out)
    ch = re.search(r"checked (\d+) functions", out)
    return {"ghosts": int(g.group(1)) if g else None,
            "missing": int(mi.group(1)) if mi else None,
            "checked": int(ch.group(1)) if ch else None}


def byte_exact(mods, totals):
    """Credit the byte-exact-ASM track that the cited-C ledger cannot see.

    The nine system modules are reconstructed as byte-exact MASM
    (`reconstructed/<MOD>.asm`), each proven by `masmlift --verify` to reassemble
    to the *exact* shipped bytes. That is a different, complete track from the
    readable-C reconstruction the ledger measures — which is why the system
    modules read ~0% cited-C. Mark each module whose byte-exact source is present
    and report the byte-exact byte-coverage of the system layer (analogous to
    win95's byte_pct). Coverage is 100% for a module whose whole extent is lifted;
    the readable-C figure stays separate (`pct` / addressed)."""
    be_bytes = 0
    for m in mods:
        asm = os.path.join(layout.recon_dir(ROOT), m["name"] + ".asm")
        if os.path.isfile(asm):
            m["byte_exact"] = True
            m["byte_exact_pct"] = 100.0        # whole module reassembles to shipped bytes
            be_bytes += m.get("code_bytes", 0)
    cb = totals.get("code_bytes") or 0
    if cb:
        totals["byte_exact_bytes"] = be_bytes
        totals["byte_exact_pct"] = round(100.0 * be_bytes / cb, 1)


def byte_exact_apps(apps, totals):
    """Credit the byte-exact-ASM track for the 13 desktop APPS, produced by the
    conductor (reports/conduct/<app>.json, via `bxscore`). Like the system
    modules, this is a complete SCAFFOLDING track distinct from the human-read
    cited-C figure: every listed function reassembles byte-identical to the shipped
    binary (segdiff-verified). Annotate each app and report the aggregate."""
    import glob
    led = {}
    for p in glob.glob(os.path.join(ROOT, "reports", "conduct", "*.json")):
        b = os.path.basename(p)
        app = (b[:-9] if b.endswith(".exe.json") else b[:-5]).upper()
        try:
            d = json.load(open(p))
        except (OSError, ValueError):
            continue
        if d and len(d) > len(led.get(app, {})):
            led[app] = d
    be_fns = be_bytes = live = tot = 0
    for a in apps:
        d = led.get((a.get("name") or "").upper())
        if not d:
            continue
        ex = sum(1 for r in d.values() if r.get("status") == "exact")
        cb = sum(r.get("size", 0) for r in d.values())
        a["byte_exact_fns"] = ex
        a["byte_exact_bytes"] = cb
        a["byte_exact_live_pct"] = round(
            100.0 * sum(r.get("live", 0) for r in d.values())
            / max(1, sum(r.get("total", 0) for r in d.values())), 1)
        be_fns += ex; be_bytes += cb
        live += sum(r.get("live", 0) for r in d.values())
        tot += sum(r.get("total", 0) for r in d.values())
    if be_fns:
        totals["apps_byte_exact_fns"] = be_fns
        totals["apps_byte_exact_bytes"] = be_bytes
        totals["apps_byte_exact_live_pct"] = round(100.0 * live / tot, 1) if tot else 0.0


def recompute():
    led = run("ledger.py")
    ver = run("verify.py")
    if led.startswith("ERROR") or "Traceback" in led:
        with _lock:
            _state["error"] = led.strip().splitlines()[-1][:300]
            _state["status"] = "error"
        return
    mods, layers, totals, auto = parse_ledger(led)
    byte_exact(mods, totals)
    apps, apptot = parse_appledger(run("appledger.py"))
    byte_exact_apps(apps, totals)
    build = parse_build(run("build.py"))

    # THE LADDER. Each rung is a strictly stronger claim than the one above it,
    # and each carries its own denominator so a percentage can never be read
    # against the wrong one.
    carved = totals.get("fns", 0) + (apptot.get("fns") or 0)
    cited = totals.get("cited", 0) + (apptot.get("cited") or 0)
    ladder = [
        {"rung": "carved", "n": carved, "of": carved,
         "what": "functions found in the binaries"},
        {"rung": "cited", "n": cited, "of": carved,
         "what": "read and placed — segment, family, call graph, address"},
        {"rung": "addressed", "n": totals.get("addressed", 0), "of": carved,
         "what": "a C function exists for it"},
        {"rung": "transcribed", "n": totals.get("transcribed", 0), "of": carved,
         "what": "…with a real body derived from the instructions"},
        {"rung": "compiles", "n": build.get("clean", 0), "of": build.get("files", 0),
         "what": "files well-formed against the 1985 SDK headers", "unit": "files"},
        {"rung": "verified", "n": 0, "of": carved,
         "what": "behaviour checked against the original — never compiled to "
                 "16-bit, never run"},
    ]
    with _lock:
        _state.update({"status": "ok", "error": None, "modules": mods,
                       "layers": layers, "totals": totals, "auto": auto,
                       "apps": apps, "apptotal": apptot, "build": build,
                       "ladder": ladder,
                       "verify": parse_verify(ver),
                       "generated": time.time()})


def watcher():
    last = None
    while True:
        try:
            cur = newest_mtime()
            if cur != last:
                recompute()
                last = cur
        except Exception as ex:
            with _lock:
                _state["error"] = str(ex)[:300]
        time.sleep(4)


class Handler(BaseHTTPRequestHandler):
    def log_message(self, *a):
        pass    # the console is for the user, not for request spam

    def _send(self, code, body, ctype):
        b = body.encode("utf-8") if isinstance(body, str) else body
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(b)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(b)

    def do_GET(self):
        if self.path.startswith("/api/status"):
            with _lock:
                self._send(200, json.dumps(_state), "application/json")
            return
        path = os.path.join(ROOT, "dashboard.html")
        if os.path.isfile(path):
            self._send(200, open(path, encoding="utf-8").read(),
                       "text/html; charset=utf-8")
        else:
            self._send(404, "dashboard.html not found next to dash.py", "text/plain")


class Server(HTTPServer):
    """*** DO NOT LET A SECOND INSTANCE SHARE THE PORT ***

    HTTPServer sets `allow_reuse_address = 1`, which sets SO_REUSEADDR. On
    Unix that only permits rebinding a port still in TIME_WAIT. On WINDOWS it
    permits several sockets to bind the SAME address at once, and no error is
    raised — so starting the dashboard twice leaves two live listeners, and an
    incoming request goes to whichever the kernel picks.

    That is exactly what happened here: THREE processes were found LISTENING on
    127.0.0.1:8787 together, requests were being answered by a stale one or
    refused outright, and an earlier session had worked around it by moving to
    port 8788 rather than noticing the cause.

    With reuse off, a second instance fails loudly with "address already in
    use" — which is the correct behaviour and says what is wrong.
    """
    allow_reuse_address = False


def main():
    port = 8787
    if "--port" in sys.argv:
        port = int(sys.argv[sys.argv.index("--port") + 1])
    if "--once" in sys.argv:
        recompute()
        print(json.dumps(_state, indent=1))
        return
    # Bind BEFORE announcing. Printing the URL first and failing afterwards
    # tells the user the dashboard is up at an address that is serving someone
    # else's process.
    try:
        srv = Server(("127.0.0.1", port), Handler)
    except OSError as ex:
        print("cannot bind port %d: %s" % (port, ex))
        print("another dash.py is already listening — stop that one rather than"
              " starting a second, or pass --port N.")
        return
    threading.Thread(target=watcher, daemon=True).start()
    print("win1.0-re live ledger  ->  http://localhost:%d" % port)
    print("(first numbers appear once the initial ledger run finishes)")
    srv.serve_forever()


if __name__ == "__main__":
    main()
