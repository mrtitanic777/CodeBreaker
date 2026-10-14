r"""progress.py -- watch a long run from outside it.

    python -m codebreaker progress                 one status line, rewritten every few seconds
    python -m codebreaker progress --once          print the status once and stop
    python -m codebreaker progress --window        the same in a small window
    python -m codebreaker progress --interval 10   seconds between readings (default 3)
    python -m codebreaker progress --log FILE      also show the last line of a run's output file

    from codebreaker.ui import progress
    progress.snapshot()        # what the line, the window and the web console all show

A sweep or a module run is not asked to report anything: every result it produces goes into
the function store as it is made, so the store is the progress. A reading says, per kind of
module that can be reconstructed, how many modules of the corpus are measured under the verifier
settings in force and how their functions stand (from the coverage ledger); which module results
were last written for and how long ago; how many results arrived in the last ten minutes, and
from that a rough time to the end of the corpus; and how many of the runner's worker processes
are in the middle of an item (jobs/pool.py keeps a note per worker in the temporary folder).
The store is opened read-only, so watching cannot disturb the run.
"""
import glob
import os
import sys
import tempfile
import time

from codebreaker.coverage import ledger
from codebreaker.store import functions as F

WINDOW = 600                       # seconds of results the rate is taken over


def _busy_workers():
    """Worker processes of the job runner that are in the middle of an item right now."""
    busy = 0
    for note in glob.glob(os.path.join(tempfile.gettempdir(), "cbjobs*", "*", "*.cur")):
        try:
            busy += bool(os.path.getsize(note)) and time.time() - os.path.getmtime(note) < 3600
        except OSError:
            pass
    return busy


def _last_line(path):
    try:
        with open(path, "rb") as f:
            f.seek(max(0, os.path.getsize(path) - 4096))
            lines = [ln for ln in f.read().decode("utf-8", "replace").splitlines() if ln.strip()]
        return lines[-1].strip() if lines else ""
    except OSError:
        return ""


def snapshot(log=None):
    """One reading: {'time', 'kinds': [{kind, name, modules, measured, functions, <the store's
    classes>}], 'recent': [{module, kind, results, age}], 'rate' (results per minute over the last
    ten minutes), 'idle' (seconds since the last result, None when there are none), 'workers',
    'eta' (seconds, None when nothing is arriving), 'active', 'log'}."""
    now = time.time()
    out = {"time": time.strftime("%H:%M:%S"), "kinds": [], "recent": [], "rate": 0.0, "idle": None,
           "workers": _busy_workers(), "eta": None, "active": False, "log": _last_line(log) if log else "",
           "store": F.default_path()}
    fs = ledger.open_store()
    led = ledger.measure(fs)
    for r in led["kinds"]:
        if led["profiles"].get(r["kind"]):
            out["kinds"].append({k: r[k] for k in ("kind", "name", "modules", "measured", "functions") + F.ORDER})
    if fs is None:
        return out
    try:
        q = ("FROM verdict v JOIN candidate c ON c.id=v.candidate JOIN function f ON f.id=c.function "
             "JOIN module m ON m.sha=f.module ")
        for name, kind, n, last in fs.db.execute(
                "SELECT m.name, m.kind, COUNT(*), MAX(v.ts) " + q + "GROUP BY m.sha ORDER BY MAX(v.ts) DESC LIMIT 8"):
            out["recent"].append({"module": name, "kind": ledger.TITLE.get(kind, kind), "results": n,
                                  "age": max(0, int(now - last))})
        fresh = fs.db.execute("SELECT COUNT(*) " + q + "WHERE v.ts > ?", (now - WINDOW,)).fetchone()[0]
    finally:
        fs.close()
    out["rate"] = round(fresh * 60.0 / WINDOW, 1)
    if out["recent"]:
        out["idle"] = out["recent"][0]["age"]
    out["active"] = bool(out["workers"]) or (out["idle"] is not None and out["idle"] < 120)
    done = sum(k["measured"] for k in out["kinds"])
    left = sum(k["modules"] - k["measured"] for k in out["kinds"])
    functions = sum(k["functions"] for k in out["kinds"])
    if fresh and done and left:                                # as many functions a module as so far
        out["eta"] = int(left * (functions / float(done)) / (fresh / float(WINDOW)))
    return out


def _clock(seconds):
    if seconds is None:
        return "-"
    h, rest = divmod(int(seconds), 3600)
    return "%dh %02dm" % (h, rest // 60) if h else "%dm %02ds" % (rest // 60, rest % 60)


def line(s):
    """A reading as one line of text."""
    parts = []
    for k in s["kinds"]:
        parts.append("%s %d/%d modules, %d functions (%s)" % (
            k["name"], k["measured"], k["modules"], k["functions"],
            ", ".join("%s %d" % (F.TITLE[c], k[c]) for c in F.ORDER if k[c]) or "nothing yet"))
    if s["recent"]:
        parts.append("last: %s %s ago" % (s["recent"][0]["module"], _clock(s["idle"])))
    parts.append("%s results/min" % s["rate"] if s["rate"] else "idle" if not s["active"] else "working")
    if s["workers"]:
        parts.append("%d workers busy" % s["workers"])
    if s["eta"] is not None:
        parts.append("about %s left" % _clock(s["eta"]))
    if s["log"]:
        parts.append(s["log"][:80])
    return "[%s] %s" % (s["time"], " | ".join(parts) or "no results yet")


def _window(interval, log, once):
    """The reading in a small window (standard-library tkinter), refreshed every `interval` seconds."""
    import tkinter as tk
    from tkinter import ttk
    root = tk.Tk()
    root.title("CodeBreaker -- progress")
    root.minsize(520, 300)
    frame = ttk.Frame(root, padding=12)
    frame.pack(fill="both", expand=True)
    head = ttk.Label(frame, text="reading the store...", font=("Segoe UI", 10, "bold"))
    head.pack(anchor="w")
    rows = ttk.Frame(frame)
    rows.pack(fill="x", pady=(8, 4))
    bars = {}
    status = ttk.Label(frame, text="", wraplength=640, justify="left")
    status.pack(anchor="w", pady=(4, 4))
    recent = tk.Listbox(frame, height=8, activestyle="none", font=("Consolas", 9))
    recent.pack(fill="both", expand=True)

    def refresh():
        try:
            s = snapshot(log)
        except Exception as e:                                # the store was mid-write: read again
            status.config(text="could not read the store this time (%s)" % type(e).__name__)
            s = None
        if s:
            head.config(text=("Working" if s["active"] else "Idle") + "   " + s["time"])
            for k in s["kinds"]:
                if k["kind"] not in bars:
                    label = ttk.Label(rows, text="")
                    bar = ttk.Progressbar(rows, maximum=max(1, k["modules"]), length=360)
                    label.grid(row=len(bars) * 2, column=0, sticky="w")
                    bar.grid(row=len(bars) * 2 + 1, column=0, sticky="we", pady=(0, 6))
                    bars[k["kind"]] = (label, bar)
                label, bar = bars[k["kind"]]
                label.config(text="%s: %d of %d modules measured; %d functions: %s" % (
                    k["name"], k["measured"], k["modules"], k["functions"],
                    ", ".join("%s %d" % (F.TITLE[c], k[c]) for c in F.ORDER if k[c]) or "nothing yet"))
                bar.config(value=k["measured"])
            status.config(text="%s results a minute; %d workers busy; about %s left%s" % (
                s["rate"], s["workers"], _clock(s["eta"]), ("\n" + s["log"]) if s["log"] else ""))
            recent.delete(0, "end")
            for r in s["recent"]:
                recent.insert("end", "%-16s %-7s %6d results   %s ago" % (r["module"], r["kind"], r["results"], _clock(r["age"])))
        if once:
            root.after(400, root.destroy)
        else:
            root.after(int(interval * 1000), refresh)

    root.after(50, refresh)
    root.mainloop()
    return 0


def main(argv):
    """python -m codebreaker progress [--once] [--window] [--interval SECONDS] [--log FILE]

    Show how a running sweep or module run is getting on, read from the function store."""
    def option(name, default=None):
        return argv[argv.index(name) + 1] if name in argv and argv.index(name) + 1 < len(argv) else default

    interval, log, once = max(1.0, float(option("--interval", "3"))), option("--log"), "--once" in argv
    if "--window" in argv:
        try:
            return _window(interval, log, once)
        except ImportError:
            print("no window toolkit (tkinter) in this Python; showing the line instead")
    width = 0
    try:
        while True:
            text = line(snapshot(log))
            if once or not sys.stdout.isatty():
                print(text, flush=True)
            else:
                sys.stdout.write("\r" + text[:200].ljust(width))
                sys.stdout.flush()
                width = len(text[:200])
            if once:
                return 0
            time.sleep(interval)
    except KeyboardInterrupt:
        print()
        return 0
