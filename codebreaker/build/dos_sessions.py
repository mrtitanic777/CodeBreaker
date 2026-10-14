r"""
dosworker.py — a POOL of persistent, warm DOSBox+MASM sessions.

The DOSBox BOOT (~1.5-2 s) is the dominant per-assemble cost. `masmlift` amortised
it by batching many functions into one boot; this eliminates the RE-boot entirely:
each worker boots DOSBox ONCE and then loops, assembling batch after batch without
restarting. A batch that took a fresh boot now costs ~0.3 s.

How the warm session works (validated):
  * a SINGLE small mount (C:) holds MASM.EXE + the work files — one small mount so
    DOSBox's `RESCAN` reliably sees host-created files (a big mount does not);
  * a DOS dispatch loop polls: `rescan` -> if C:\GO exists -> `call C:\WORK.BAT`
    (the MASM commands the host just wrote) -> write C:\ACK -> loop;
  * the host drops the .ASM + WORK.BAT + GO, waits for ACK, reads the .OBJ/.LST.

A pool hands out warm workers to the conductor's fan-out (one worker per concurrent
chunk). Falls back cleanly: a dead worker is dropped and a fresh one boots.
"""
import os
import shutil
import subprocess
import tempfile
import threading
import time

# WAIT.COM: BIOS wait (INT 15h/86h) ~50ms, then exit. A bounded busy-wait — used
# to build a short LOW-LATENCY response window before the worker falls back to the
# CPU-cheap (but 1s-coarse) CHOICE throttle.
_WAIT_COM = bytes([0xB4, 0x86, 0xB9, 0x00, 0x00, 0xBA, 0x50, 0xC3,
                   0xCD, 0x15, 0xCD, 0x20])     # mov ah,86h;cx=0;dx=C350h;int15h;int20h

# Hybrid idle loop: a few fast `wait`+rescan checks (~200ms window that catches a
# back-to-back next batch almost instantly), and only THEN a 1s CHOICE keyboard-
# wait that DOSBox idle-throttles to ~7% of a core. A bare `goto loop` busy-spins
# a full core (a pool of those pegs the machine and STARVES MASM); a pure 1s
# CHOICE throttles but adds ~1s latency to every assemble. The hybrid gives both:
# low latency when busy, low CPU when genuinely idle.
_POLL = "rescan >nul\r\nif exist C:\\GO goto work\r\nwait\r\n"
_WORKER_BAT = ("echo hi > C:\\BOOTED\r\n"
               ":loop\r\n"
               + _POLL + _POLL + _POLL + _POLL
               + "choice /C:YN /T:Y,1 /N >nul\r\n"
               "goto loop\r\n"
               ":work\r\n"
               "del C:\\GO\r\n"
               "rescan >nul\r\n"
               "call C:\\WORK.BAT\r\n"
               "echo done > C:\\ACK\r\n"
               "goto loop\r\n")

_CONF = ('[sdl]\r\nautolock=false\r\n'
         '[cpu]\r\ncputype=386\r\ncore=normal\r\ncycles=max\r\n'
         '[dos]\r\nver=6.22\r\n'
         '[autoexec]\r\nmount c "%s"\r\nset TMP=C:\\\r\nc:\r\ncall WORKER.BAT\r\n')


class DosboxWorker:
    """One persistent DOSBox session running the MASM dispatch loop."""

    def __init__(self, masmroot, dbx, boot_timeout=20):
        self.dbx = dbx
        self.work = tempfile.mkdtemp(prefix="dosw_")
        # colocate ONLY MASM.EXE — the reconstructor's asm uses no INCLUDE, so the
        # mount stays tiny, which is what makes DOSBox's RESCAN reliably see
        # host-created trigger files (a large mount breaks that).
        shutil.copy(os.path.join(masmroot, "BIN", "MASM.EXE"),
                    os.path.join(self.work, "MASM.EXE"))
        open(os.path.join(self.work, "WAIT.COM"), "wb").write(_WAIT_COM)
        open(os.path.join(self.work, "WORKER.BAT"), "w", newline="").write(_WORKER_BAT)
        self.conf = os.path.join(self.work, "dbx.conf")
        open(self.conf, "w", newline="\r\n").write(_CONF % self.work)
        self.proc = subprocess.Popen(
            [dbx, "-conf", self.conf, "-nolog"],
            env=dict(os.environ, SDL_VIDEODRIVER="dummy"),
            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        self._wait_boot(boot_timeout)

    def _p(self, name):
        return os.path.join(self.work, name)

    def _wait_boot(self, timeout):
        t = time.time()
        while time.time() - t < timeout:
            if os.path.exists(self._p("BOOTED")):
                return
            if self.proc.poll() is not None:
                raise RuntimeError("DOSBox worker died during boot")
            time.sleep(0.05)
        raise RuntimeError("DOSBox worker boot timed out")

    def assemble_many(self, items, timeout=None, cancelled=None):
        """Assemble [(stem, asm_src)] in this warm session. Returns
        {'results': {stem: {obj,lst,log}}} or {'aborted': True}/{'dead': True}."""
        for f in os.listdir(self.work):
            if (f.endswith((".ASM", ".OBJ", ".LST", ".LOG"))
                    or f in ("ACK", "GO", "WORK.BAT")):
                try:
                    os.remove(self._p(f))
                except OSError:
                    pass
        for stem, src in items:
            with open(self._p(stem + ".ASM"), "w", newline="\r\n") as fh:
                fh.write(src)
        cmds = "".join("MASM %s.ASM, %s.OBJ, %s.LST, NUL; > %s.LOG\r\n"
                       % (s, s, s, s) for s, _ in items)
        open(self._p("WORK.BAT"), "w", newline="").write(cmds)
        open(self._p("GO"), "w").write("x")             # trigger — written LAST
        if timeout is None:
            timeout = 60 + 3 * len(items)
        t = time.time()
        while time.time() - t < timeout:
            if os.path.exists(self._p("ACK")):
                break
            if cancelled and cancelled():
                self.close()
                return {"aborted": True}
            if self.proc.poll() is not None:
                return {"dead": True}
            time.sleep(0.02)
        else:
            return {"dead": True}                        # hung -> treat as dead
        results = {}
        for stem, _src in items:
            d = {}
            for ext, key in ((".OBJ", "obj"), (".LST", "lst"), (".LOG", "log")):
                p = self._p(stem + ext)
                if os.path.exists(p):
                    d[key] = open(p, "rb").read() if ext == ".OBJ" else \
                        open(p, encoding="latin1").read()
            results[stem] = d
        return {"results": results, "work": self.work}

    def alive(self):
        return self.proc.poll() is None

    def close(self):
        try:
            self.proc.kill()
            self.proc.wait(timeout=3)
        except Exception:
            pass
        shutil.rmtree(self.work, ignore_errors=True)


class WorkerPool:
    """Thread-safe pool of warm workers, capped at `maxsize`."""

    def __init__(self, masmroot, dbx, maxsize):
        self.masmroot = masmroot
        self.dbx = dbx
        self._sem = threading.Semaphore(maxsize)
        self._idle = []
        self._all = []
        self._lock = threading.Lock()

    def lease(self):
        self._sem.acquire()
        with self._lock:
            if self._idle:
                return self._idle.pop()
        try:
            w = DosboxWorker(self.masmroot, self.dbx)
        except Exception:
            self._sem.release()
            raise
        with self._lock:
            self._all.append(w)
        return w

    def release(self, w, keep=True):
        with self._lock:
            if keep and w in self._all and w.alive():
                self._idle.append(w)
            else:
                if w in self._all:
                    self._all.remove(w)
                w.close()
        self._sem.release()

    def close(self):
        with self._lock:
            for w in self._all:
                w.close()
            self._all = []
            self._idle = []
