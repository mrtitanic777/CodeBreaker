#!/usr/bin/env python3
"""parallel.py -- how much of the machine CodeBreaker takes: the budget every runner asks for.

    from codebreaker.base import parallel
    n = parallel.workers()                     # how many things run side by side (settings.json "workers")
    with parallel.slots(want=6, mb_each=256, label="decompile") as n:
        ... run n at once ...                  # the machine's slots, shared by every CodeBreaker process
    parallel.throttle(cores=4)                 # this process below normal priority, on four cores
    parallel.wait_for_memory("verify")         # before a heavy job: while memory is short, wait

The work itself is run by jobs/pool.py (threads, worker processes, a subprocess each); this
holds what they all ask before they start: how many, and whether the machine has room.
"""
import os


def cpu_count():
    return os.cpu_count() or 4


def workers():
    """How many things run side by side, everywhere: the one number every part of the workbench
    asks for (compiles, judging, sweeps). settings.json "workers", or CB_JOBS; otherwise half the
    processors, at most 6 -- so that two parts running at once do not take the whole machine."""
    from codebreaker.base import settings
    n = os.environ.get("CB_JOBS") or settings.get("workers")
    try:
        n = int(n)
    except (TypeError, ValueError):
        n = 0
    return n if n > 0 else max(1, min(6, cpu_count() // 2))


# ---------------------------------------------------------------------------
# Background work: below normal priority, on a bounded number of cores.
# ---------------------------------------------------------------------------
def throttle(cores=None, below_normal=True):
    """Lower this process's priority and/or pin it to the first `cores` CPUs, so a background
    sweep leaves the machine usable. Child processes inherit both. Windows only; a no-op (False)
    elsewhere or on failure."""
    if os.name != "nt":
        return False
    try:
        import ctypes
        k = ctypes.windll.kernel32
        # OpenProcess on our own pid: the GetCurrentProcess pseudo-handle overflows a c_void_p.
        h = k.OpenProcess(0x1F0FFF, False, os.getpid())
        if not h:
            return False
        if below_normal:
            k.SetPriorityClass(h, 0x00004000)                # BELOW_NORMAL_PRIORITY_CLASS
        if cores:
            k.SetProcessAffinityMask(h, (1 << min(cores, cpu_count())) - 1)
        k.CloseHandle(h)
        return True
    except Exception:
        return False


def available_gb():
    """the memory the system can still give out without paging, in GB (None where not known)"""
    if os.name == "nt":
        try:
            import ctypes

            class MS(ctypes.Structure):
                _fields_ = [("dwLength", ctypes.c_ulong), ("dwMemoryLoad", ctypes.c_ulong),
                            ("ullTotalPhys", ctypes.c_ulonglong), ("ullAvailPhys", ctypes.c_ulonglong),
                            ("ullTotalPageFile", ctypes.c_ulonglong), ("ullAvailPageFile", ctypes.c_ulonglong),
                            ("ullTotalVirtual", ctypes.c_ulonglong), ("ullAvailVirtual", ctypes.c_ulonglong),
                            ("ullAvailExtendedVirtual", ctypes.c_ulonglong)]
            m = MS()
            m.dwLength = ctypes.sizeof(MS)
            if ctypes.windll.kernel32.GlobalMemoryStatusEx(ctypes.byref(m)):
                return min(m.ullAvailPhys, m.ullAvailPageFile) / 2 ** 30   # (RAM free, and commit left)
        except Exception:
            return None
    try:
        with open("/proc/meminfo") as f:
            for l in f:
                if l.startswith("MemAvailable:"):
                    return int(l.split()[1]) / 2 ** 20
    except OSError:
        pass
    return None


def min_free_gb():
    """settings.json "min_free_gb" (or CB_MIN_FREE_GB): below this much free memory no new heavy job starts"""
    from codebreaker.base import settings
    v = os.environ.get("CB_MIN_FREE_GB") or settings.get("min_free_gb")
    try:
        return float(v)
    except (TypeError, ValueError):
        return 6.0


def wait_for_memory(label="", poll=2.0, limit=600):
    """before a heavy job: wait while the system has less than min_free_gb free - the machine first, the work
    after. Returns the seconds waited."""
    import time
    need, waited = min_free_gb(), 0.0
    while True:
        a = available_gb()
        if a is None or a >= need or waited >= limit:
            return waited
        if waited == 0:
            import sys
            print("(waiting: %.1f GB free, %.1f wanted%s)" % (a, need, " - " + label if label else ""),
                  file=sys.stderr, flush=True)
        time.sleep(poll)
        waited += poll


def total_gb():
    """the machine's memory in GB (None where not known)"""
    if os.name == "nt":
        try:
            import ctypes
            m = ctypes.c_ulonglong(0)
            if ctypes.windll.kernel32.GetPhysicallyInstalledSystemMemory(ctypes.byref(m)):
                return m.value / 2 ** 20                          # (KB)
        except Exception:
            return None
    try:
        return os.sysconf("SC_PAGE_SIZE") * os.sysconf("SC_PHYS_PAGES") / 2 ** 30
    except (ValueError, OSError, AttributeError):
        return None


# ---------------------------------------------------------------------------
# The governor: one budget of heavy jobs for the whole machine.
#
# Each part used to decide on its own how much to run at once, so they multiplied: a sweep taking
# two binaries apart, each splitting its decompiler six ways, asked for twelve at once. Now a
# heavy job holds a slot while it runs -- slots are the machine's `workers()`, shared by every
# CodeBreaker process (a locked file each: the system lets go of it when its process ends, even
# killed) -- and it is given no more than the free memory carries, at `mb_each` a piece, past the
# reserve the system keeps. A small machine runs one at a time; nobody waits for ever.
# ---------------------------------------------------------------------------
class _Slot:
    def __init__(self, path):
        self.path, self.f = path, None

    def take(self):
        f = open(self.path, "a+b")
        try:
            if os.name == "nt":
                import msvcrt
                f.seek(0)
                msvcrt.locking(f.fileno(), msvcrt.LK_NBLCK, 1)
            else:
                import fcntl
                fcntl.flock(f.fileno(), fcntl.LOCK_EX | fcntl.LOCK_NB)
        except OSError:
            f.close()
            return False
        self.f = f
        return True

    def give_back(self):
        if self.f is None:
            return
        try:
            if os.name == "nt":
                import msvcrt
                self.f.seek(0)
                msvcrt.locking(self.f.fileno(), msvcrt.LK_UNLCK, 1)
        except OSError:
            pass
        self.f.close()
        self.f = None


def reserve_gb():
    """memory the governor leaves to the system: min_free_gb, but no more than a quarter of the machine"""
    t = total_gb()
    return min(min_free_gb(), t / 4) if t else min_free_gb()


def idle_cores(sample=0.25):
    """processors nothing is using, measured over `sample` seconds (None where not known): what
    other programs keep busy -- a build beside us -- is not ours to take"""
    if os.name == "nt":
        try:
            import ctypes
            import time
            FT = ctypes.c_ulonglong

            def times():
                i, k, u = FT(), FT(), FT()
                ctypes.windll.kernel32.GetSystemTimes(ctypes.byref(i), ctypes.byref(k), ctypes.byref(u))
                return i.value, k.value + u.value                # (kernel time includes idle)
            i0, t0 = times()
            time.sleep(sample)
            i1, t1 = times()
            return cpu_count() * (i1 - i0) / (t1 - t0) if t1 > t0 else None
        except Exception:
            return None
    try:
        return max(0.0, cpu_count() - os.getloadavg()[0])
    except (OSError, AttributeError):
        return None


def memory_allows(mb_each):
    """how many pieces of `mb_each` MB the free memory carries past the reserve (None: not known)"""
    a = available_gb()
    if a is None:
        return None
    return int(max(0.0, a - reserve_gb()) * 1024 // max(1, mb_each))


class slots:
    """    with parallel.slots(want=6, mb_each=256, label="decompile") as n:
            ... run n at once ...
    Up to `want` of the machine's slots, as many as are free and the memory carries -- at least
    one: with none free it waits for one (up to `limit` seconds, then runs on one regardless)."""

    def __init__(self, want, mb_each=256, label="", limit=600, poll=0.5):
        self.want, self.mb_each, self.label, self.limit, self.poll = max(1, int(want)), mb_each, label, limit, poll
        self.held = []

    def __enter__(self):
        import time
        from codebreaker.base import settings
        cap = workers()
        folder = settings.state("slots", "")
        mem = memory_allows(self.mb_each)
        want = min(self.want, cap, max(1, mem) if mem is not None else cap)
        if want > 1:
            idle = idle_cores()                                  # (and no more than the processors free)
            if idle is not None:
                want = min(want, max(1, int(idle + 0.5)))
        waited = 0.0
        while True:
            for i in range(cap):
                if len(self.held) >= want:
                    break
                s = _Slot(os.path.join(folder, "slot-%d" % i))
                if s.take():
                    self.held.append(s)
            if self.held or waited >= self.limit:
                return max(1, len(self.held))
            if waited == 0:
                import sys
                print("(waiting for a free slot%s)" % (" - " + self.label if self.label else ""), file=sys.stderr, flush=True)
            time.sleep(self.poll)
            waited += self.poll

    def __exit__(self, *exc):
        for s in self.held:
            s.give_back()
        self.held = []
        return False
