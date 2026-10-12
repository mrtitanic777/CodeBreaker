#!/usr/bin/env python3
"""
progress.py — one shared progress context for the whole workbench.

Any long operation reports here. It drives TWO sinks:
  * an in-process native meter (progressui) when a UI is enabled (`--ui`), and
  * a shared LIVE FILE (reports/live_progress.json) that a *separate* process
    can mirror — which is how double-clicking CodeBreaker.exe gives a
    persistent "Ready" window that lights up whenever any run is working, even
    a headless one in another terminal.

`parallel.tmap`/`pmap` drive it automatically (AUTO mode), so every fan-out in
every tool lights up with no extra code; the conductor takes over (MANAGED) for
a richer view. `monitor()` is the consumer side: the Ready window.
"""
import json
import os
from src.core import cores
import sys
import time

_enabled = False
_managed = False
_win = None            # own ProgressWindow | False | None
_t0 = None
_session_t0 = None     # session stopwatch start — set once, spans all modules

LIVE = os.path.join("reports", "live_progress.json")
_state = {"active": False, "caption": "CodeBreaker", "status": "Ready",
          "done": 0, "total": 1, "error": False, "details": [],
          "done_event": 0.0, "summary": "", "ui_state": "work", "next": "",
          "bytes": "", "left": ""}


def enable():
    global _enabled
    _enabled = True


def want_two_tier():          # kept for API compatibility (single bar now)
    pass


def active():
    return _enabled and sys.platform == "win32"


def manage():
    global _managed
    _managed = True


def _write_live():
    try:
        os.makedirs("reports", exist_ok=True)
        st = dict(_state); st["ts"] = time.time()
        tmp = LIVE + ".%d.tmp" % os.getpid()
        with open(tmp, "w") as f:
            json.dump(st, f)
        os.replace(tmp, LIVE)
    except Exception:
        pass


def _get():
    global _win
    if not active():
        return None
    # ONLY the main process may own a window. If any multiprocessing child ever
    # reaches here, it must not spawn its own dialog (that was the duplicate-
    # window bug — closing an extra window killed the shared task).
    import multiprocessing
    if multiprocessing.current_process().name != "MainProcess":
        return None
    if _win is None:
        try:
            progressui = cores.get('progressui')   # an era may override the meter
            _win = progressui.ProgressWindow()
            if not _win.start():
                _win = False
        except Exception:
            _win = False
    return _win or None


_samples = []          # [(time, done)] over a trailing window
_ema_rate = None       # smoothed items/sec
RATE_WINDOW = 20.0     # seconds of history the rate is measured over
RATE_ALPHA = 0.25      # EMA smoothing (lower = steadier)


def _fmt_eta(sec):
    sec = int(max(0, sec))
    if sec >= 3600:
        return "%d:%02d:%02d" % (sec // 3600, (sec % 3600) // 60, sec % 60)
    return "%d:%02d" % (sec // 60, sec % 60)


def op(title, total):
    global _t0, _samples, _ema_rate, _session_t0
    _t0 = time.time()
    if _session_t0 is None:                    # start the session stopwatch ONCE
        _session_t0 = _t0
    _samples = [(_t0, 0)]
    _ema_rate = None
    _state.update(active=True, caption=title, status=title, done=0,
                  total=max(1, total), error=False, done_event=0.0, summary="",
                  ui_state="work", clock=_session_t0)
    _write_live()
    w = _get()
    if w:
        w.set_caption(title); w.set_total(max(1, total)); w.step(0); w.set_working(True)
        w.set_clock(_session_t0)


def bar(done, total):
    global _ema_rate
    total = max(1, total)
    _state["done"], _state["total"] = done, total
    # Rate over a trailing WINDOW, then EMA-smoothed — steady and reflects
    # CURRENT speed (so the ETA doesn't swing when tiny and big functions
    # alternate), instead of a since-start average.
    now = time.time()
    _samples.append((now, done))
    while len(_samples) > 2 and now - _samples[0][0] > RATE_WINDOW:
        _samples.pop(0)
    span = now - _samples[0][0]
    ddone = done - _samples[0][1]
    # rate when we have enough history; the elapsed SESSION stopwatch (rendered by
    # the window from `clock`) always counts up — no ETA, no "estimating".
    if span >= 4.0 and ddone > 0:
        inst = ddone / span
        _ema_rate = inst if _ema_rate is None else RATE_ALPHA * inst + (1 - RATE_ALPHA) * _ema_rate
        _state["status"] = ("%s   %d/%d   %.1f/s"
                            % (_state["caption"], done, total, _ema_rate))
    else:
        _state["status"] = "%s   %d/%d" % (_state["caption"], done, total)
    _write_live()
    w = _get()
    if w:
        w.set_total(total); w.step(done); w.set_status(_state["status"])


def overall(done, total):
    bar(done, total)


def sub(done, total):         # single bar now: no-op mirror
    pass


def status(msg):
    _state["status"] = msg; _write_live()
    w = _get()
    if w:
        w.set_status(msg)


def caption(msg):
    _state["caption"] = msg; _write_live()
    w = _get()
    if w:
        w.set_caption(msg)


def next_module(name):
    """Show the module that runs after the current one (batch/tier mode)."""
    _state["next"] = str(name or ""); _write_live()
    w = _get()
    if w:
        w.set_next(name)


def modules_left(n):
    """Tally of modules still to do in a batch/tier (shown beside Next)."""
    s = ("%d modules left" % n) if n and n > 0 else ""
    _state["left"] = s
    _write_live()
    w = _get()
    if w:
        w.set_left(s)


def bytes_done(done, total=0):
    """Running count of file bytes reconstructed so far (shown above the bar)."""
    s = ("%s / %s bytes" % (f"{int(done):,}", f"{int(total):,}")) if total \
        else ("%s bytes" % f"{int(done):,}")
    _state["bytes"] = s
    _write_live()
    w = _get()
    if w:
        w.set_bytes(s)


def detail(line):
    d = _state["details"]
    d.append(str(line))
    del d[:-40]
    _write_live()
    w = _get()
    if w:
        w.add_detail(line)


def error(flag=True):
    _state["error"] = bool(flag)
    _state["ui_state"] = "warn" if flag else "work"
    _write_live()
    w = _get()
    if w:
        w.set_error(flag)


def set_ui_state(key):
    """Switch the state icon (work / warn / stop) without touching the red bar."""
    _state["ui_state"] = key
    _write_live()
    w = _get()
    if w:
        w.set_state(key)


def warn():
    set_ui_state("warn")


def cancelled():
    w = _get()
    return bool(w and w.cancelled())


def auto_op(title, total):
    if not _managed:
        op(title, total)


def auto_bar(done, total):
    if not _managed:
        bar(done, total)


def finish(summary, beep=True, **_ignored):
    global _session_t0
    _session_t0 = None                         # next session restarts the stopwatch
    _state.update(active=False, status=summary, summary=summary,
                  done_event=time.time(), next="", bytes="", left="")
    _write_live()
    w = _get()
    if w:
        w.set_working(False)
        w.set_status(summary)
        if beep:
            w.beep()
        # Leave the window OPEN showing the result; only the top-right X closes
        # it. Block here until the user closes it, then let the process exit.
        while w.alive():
            time.sleep(0.15)
    close()


def close():
    global _win, _enabled, _session_t0
    _session_t0 = None
    if _win and _win is not False:
        try:
            _win.close()
        except Exception:
            pass
    _win = None
    _enabled = False


# ---------------------------------------------------------------------------
# CONSUMER side — the double-click "Ready" window that mirrors the live file
# ---------------------------------------------------------------------------
def monitor(sound=True):
    """Open the persistent window and mirror reports/live_progress.json until
    the window is closed. Shows 'Ready' when nothing is running; reflects any
    CodeBreaker run's progress live; dings + shows the summary on completion."""
    if sys.platform != "win32":
        print("progress UI requires Windows"); return False
    progressui = cores.get('progressui')   # an era may override the meter
    p = progressui.ProgressWindow(caption="CodeBreaker")
    if not p.start():
        return False
    p.set_status("Ready"); p.set_total(1); p.step(0)
    last_done_event = 0.0
    while p.alive():
        st = None
        try:
            if os.path.exists(LIVE):
                st = json.load(open(LIVE))
        except Exception:
            st = None
        now = time.time()
        fresh = bool(st) and (now - st.get("ts", 0) < 12)
        if fresh and st.get("active"):
            p.set_caption(st.get("caption", "CodeBreaker"))
            p.set_status(st.get("status", "Working..."))
            p.set_total(max(1, st.get("total", 1))); p.step(st.get("done", 0))
            p.set_error(bool(st.get("error")))
            p.set_state(st.get("ui_state", "work"))
            p.set_next(st.get("next", ""))
            p.set_bytes(st.get("bytes", ""))
            p.set_left(st.get("left", ""))
            p.set_clock(st.get("clock"))
            p.set_working(True)
        elif st and st.get("done_event", 0) > last_done_event:
            last_done_event = st["done_event"]
            p.set_working(False)
            p.set_next(""); p.set_bytes(""); p.set_left("")
            p.set_caption(st.get("caption", "CodeBreaker"))
            p.set_status(st.get("summary", "Done"))
            p.set_error(bool(st.get("error")))
            p.set_state(st.get("ui_state", "work"))
            p.set_total(max(1, st.get("total", 1))); p.step(st.get("total", 1))
            if sound:
                p.beep()
        elif not fresh:
            p.set_working(False)
            p.set_next(""); p.set_bytes(""); p.set_left("")
            p.set_caption("CodeBreaker"); p.set_status("Ready")
            p.set_total(1); p.step(0); p.set_error(False); p.set_state("work")
        time.sleep(0.25)
    return True
