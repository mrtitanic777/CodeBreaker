r"""native_win16.py -- trace_function16's per-instruction and memory hooks in C++ (native/tracer:
cbtrace.dll), through ctypes.

windows16.trace_function16 still lays the NE module out, enters the function, starts the run
(uc.emu_start to SENT_BASE) and reads the result; this module attaches the DLL's 16-bit hooks to
the same Unicorn engine (uc._uch), in trace_function16's order, and turns what the plain code path
and the memory hooks recorded back into the same rec fields. The heavy, rare events stay Python,
called from the native hooks: the import stand-in (import_stub / the NE oracle), a call into
another of the module's functions (answer), and interrupts (int 21h as DOS3Call). The branch log
and the ports' device model stay Python hooks, which trace_function16 attaches itself; the ports'
clock reads the live instruction count this recorder keeps (count()).

The switch: CB_NATIVE_TRACE16=0 runs the Python hooks instead (off until gated, then on). A run
kept beside a shadow, a recorded-system context, or with a prelude stays entirely on the Python
hooks. Build is shared with the 32-bit tracer (native/tracer/CMakeLists.txt).
"""
import ctypes
import os
import struct

from codebreaker.emulate import native_trace     # the loaded DLL, dll_path

_MEM_WORDS, _LAZY_WORDS = 8, 5
_ACCESS = ("read", "write", "fetch", "?")
DEFAULT = "1"             # CB_NATIVE_TRACE16 when not set (gated identical: docs/NATIVE.md)

EVENT = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_int64, ctypes.c_int64)
_P64 = ctypes.POINTER(ctypes.c_uint64)


class _Config(ctypes.Structure):
    _fields_ = [("uch", ctypes.c_void_p), ("eip_id", ctypes.c_int32),
                ("near_fn", ctypes.c_int32), ("fill", ctypes.c_int32),
                ("cap", ctypes.c_int64),
                ("cs_base", ctypes.c_uint64), ("near_ret", ctypes.c_uint64), ("far_ret", ctypes.c_uint64),
                ("sent_lo", ctypes.c_uint64), ("sent_hi", ctypes.c_uint64), ("snap_base", ctypes.c_uint64),
                ("gov_n", ctypes.c_int64), ("gov_ceil", ctypes.c_int64),
                ("count", ctypes.c_void_p),
                ("intercept", _P64), ("nintercept", ctypes.c_int64),
                ("on_sentinel", EVENT), ("on_intercept", EVENT), ("on_intr", EVENT),
                ("shadow", ctypes.c_void_p), ("own_lo", ctypes.c_uint64), ("own_hi", ctypes.c_uint64),
                ("on_own", EVENT)]


class _Result(ctypes.Structure):
    _fields_ = [(n, ctypes.c_int64) for n in (
                    "insn_count", "truncated", "returned", "limit_fault",
                    "memops", "writes_seen", "stuck", "gov_n", "hook_error", "uc_err")] + [
                ("path", ctypes.c_void_p), ("npath", ctypes.c_int64),
                ("reads", ctypes.c_void_p), ("nreads", ctypes.c_int64),
                ("writes", ctypes.c_void_p), ("nwrites", ctypes.c_int64),
                ("lazy", ctypes.c_void_p), ("nlazy", ctypes.c_int64)]


_READY = []


def _lib():
    lib = native_trace._load()
    if lib is None:
        return None
    if not _READY:
        lib.cbt_w16_begin.argtypes = (ctypes.POINTER(_Config),)
        lib.cbt_w16_begin.restype = ctypes.c_void_p
        for name in ("cbt_w16_attach", "cbt_w16_detach"):
            getattr(lib, name).argtypes = (ctypes.c_void_p,)
            getattr(lib, name).restype = ctypes.c_int
        lib.cbt_w16_free.argtypes = (ctypes.c_void_p,)
        lib.cbt_w16_free.restype = None
        lib.cbt_w16_reset.argtypes = (ctypes.c_void_p,)
        lib.cbt_w16_reset.restype = None
        lib.cbt_w16_state.argtypes = (ctypes.c_void_p, ctypes.POINTER(ctypes.c_int64))
        lib.cbt_w16_state.restype = None
        lib.cbt_w16_result_get.argtypes = (ctypes.c_void_p,)
        lib.cbt_w16_result_get.restype = ctypes.POINTER(_Result)
        _READY.append(True)
    return lib


def enabled():
    """Native 16-bit hooks for this process: CB_NATIVE_TRACE16 is not 0 and the DLL loads."""
    if os.environ.get("CB_NATIVE_TRACE16", DEFAULT) == "0" or os.environ.get("WT_DBG"):
        return False
    return _lib() is not None


def why_not():
    return native_trace.why_not()


def _u64s(values):
    values = list(values)
    return (ctypes.c_uint64 * max(len(values), 1))(*values), len(values)


def _words(ptr, n, fmt):
    if not n:
        return []
    size = struct.calcsize(fmt)
    return list(struct.iter_unpack(fmt, ctypes.string_at(ptr, n * size)))


class Run:
    """One NE function's native hooks on `uc`. `on_sentinel(addr, n)`, `on_intercept(addr, n)` and
    `on_intr(intno, n)` run the Python event handlers (import_stub / answer / the interrupt body);
    each may raise, and the exception comes back out of trace_function16. `gov` emulator._GOV."""

    def __init__(self, uc, *, eip_id, near, fill, cap, cs_base, near_ret, far_ret, sent_lo, sent_hi,
                 snap_base, gov, intercept, on_sentinel, on_intercept, on_intr, shadow=None, own=None,
                 on_own=None):
        lib = self.lib = _lib()
        self.uc, self.gov, self.exc = uc, gov, None
        self._fns = (on_sentinel, on_intercept, on_intr)
        self._count = ctypes.c_int64(0)
        self._inter = _u64s(intercept)
        self._cb = [EVENT(self._wrap(f)) for f in self._fns]
        c = self.config = _Config()
        c.uch = uc._uch.value if hasattr(uc._uch, "value") else uc._uch
        c.eip_id = int(eip_id)
        c.near_fn, c.fill = (1 if near else 0), (1 if fill else 0)
        c.cap = int(cap)
        c.cs_base, c.near_ret, c.far_ret = int(cs_base), int(near_ret), int(far_ret)
        c.sent_lo, c.sent_hi, c.snap_base = int(sent_lo), int(sent_hi), int(snap_base)
        c.gov_n, c.gov_ceil = int(gov["n"]), int(gov["ceil"] or 0)
        c.count = ctypes.cast(ctypes.pointer(self._count), ctypes.c_void_p)
        c.intercept, c.nintercept = self._inter
        c.on_sentinel, c.on_intercept, c.on_intr = self._cb
        c.shadow = shadow
        if own is not None:
            c.own_lo, c.own_hi = int(own[0]), int(own[1])
            self._own_cb = EVENT(self._wrap_own(on_own))
            c.on_own = self._own_cb
        self.handle = lib.cbt_w16_begin(ctypes.byref(c))
        if not self.handle:
            raise RuntimeError("cbtrace: a 16-bit run could not be set up")
        e = lib.cbt_w16_attach(self.handle)
        if e:
            import unicorn
            raise unicorn.UcError(e)
        self._state = (ctypes.c_int64 * 3)()

    def _wrap(self, fn):
        def cb(a, n):
            try:
                fn(a, n)
                return 0
            except BaseException as e:
                if self.exc is None:
                    self.exc = e
                return 1
        return cb

    def _wrap_own(self, fn):
        def cb(a, n):
            try:
                return 1 if fn(a, n) else 0
            except BaseException as e:
                if self.exc is None:
                    self.exc = e
                return 2
        return cb

    def reset(self):
        """A prelude ran: its path, count, reads and writes are dropped (cbt_w16_reset)."""
        self.lib.cbt_w16_reset(self.handle)

    def count(self):
        return self._count.value

    def raised(self):
        self.lib.cbt_w16_state(self.handle, self._state)
        self.gov["n"] = self._state[2]
        e, self.exc = self.exc, None
        return e

    def collect(self, rec):
        r = self.lib.cbt_w16_result_get(self.handle).contents
        self.gov["n"] = r.gov_n
        rec["insn_count"] = r.insn_count
        rec["truncated"] = bool(r.truncated)
        if r.returned:
            rec["returned"] = True
        if r.limit_fault:
            rec["limit_fault"] = True
        rec["path"] = list((ctypes.c_uint64 * r.npath).from_address(r.path)) if r.npath else []
        rec["mem_reads"] = [{"pc": w[0], "addr": w[1], "size": w[2], "value": None if w[4] else w[3], "n": w[5]}
                            for w in _words(r.reads, r.nreads, "<%dQ" % _MEM_WORDS)]
        rec["mem_writes"] = [{"pc": w[0], "addr": w[1], "size": w[2], "value": w[3], "n": w[5], "old": w[7]}
                             for w in _words(r.writes, r.nwrites, "<%dQ" % _MEM_WORDS)]
        rec["lazy_maps"] = [{"pc": w[0], "addr": w[1], "page": w[2], "access": _ACCESS[w[3]], "size": w[4]}
                            for w in _words(r.lazy, r.nlazy, "<%dQ" % _LAZY_WORDS)]
        if r.memops >= 0:
            rec["memops"] = r.memops
        if r.writes_seen >= 0:
            rec["writes_seen"] = r.writes_seen
        if r.stuck >= 0:
            rec["_stuck"] = r.stuck

    def detach(self):
        if self.lib.cbt_w16_detach(self.handle):
            raise RuntimeError("cbtrace: a 16-bit hook could not be taken off")

    def __del__(self):
        h, self.handle = getattr(self, "handle", None), None
        if h:
            self.lib.cbt_w16_free(h)
