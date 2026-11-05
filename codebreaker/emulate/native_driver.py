r"""native_driver.py -- driver.py run()'s hooks in C++ (native/tracer: cbtrace.dll), through ctypes.

A run of a VxD entry point used to call back into Python on every instruction and every memory
access. Here those hooks are C++: emulate/driver.run still lays the regions out, sets the
registers, starts the run (uc.emu_start to _RET) and reads the result; this module attaches the
DLL's driver hooks to the same Unicorn engine (uc._uch), in driver.run's order, and turns what
they recorded back into the same rec dict. During a run Python is asked one thing: which register
a service answers in (driver_services + the knowledge base), once per distinct service.

    run = native_driver.Run(uc, ...)   # the hooks attached (None when the DLL cannot be used here)
    uc.emu_start(entry, RET, count=cap + 1)
    run.collect(rec)                   # the rec fields the Python hooks would have left
    run.detach()

The Python hooks this replaced are gone (docs/NATIVE.md, step 3): without the DLL a run raises
(require()). Build: native/tracer/CMakeLists.txt (cmake + ninja in a vcvars64 shell; cbtrace.dll).
"""
import ctypes
import struct

from codebreaker.emulate import native_trace     # the loaded DLL, the register ids, dll_path

_CALL_WORDS, _PORT_WORDS, _MEM_WORDS, _LAZY_WORDS = 15, 4, 8, 5

ANSWERS = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_int32, ctypes.c_int32, ctypes.POINTER(ctypes.c_int32))
_P64 = ctypes.POINTER(ctypes.c_uint64)


class _Config(ctypes.Structure):
    _fields_ = [("uch", ctypes.c_void_p), ("reg_ids", ctypes.c_int32 * 10),
                ("ins_in", ctypes.c_int32), ("ins_out", ctypes.c_int32),
                ("vm_addr", ctypes.c_uint64), ("ret", ctypes.c_uint64),
                ("scratch_lo", ctypes.c_uint64), ("scratch_hi", ctypes.c_uint64),
                ("spans", _P64), ("nspans", ctypes.c_int64),
                ("cap", ctypes.c_int64), ("service_eax", ctypes.c_int64), ("service_carry", ctypes.c_int64),
                ("gov_n", ctypes.c_int64), ("gov_ceil", ctypes.c_int64), ("answers", ANSWERS),
                ("shadow", ctypes.c_void_p)]


class _Result(ctypes.Structure):
    _fields_ = [(n, ctypes.c_int64) for n in (
                    "exit_code", "exit_int", "insns", "truncated", "governed", "outside",
                    "memops", "writes_seen", "stuck", "gov_n", "hook_error", "uc_err")] + [
                ("cover", ctypes.c_void_p), ("ncover", ctypes.c_int64),
                ("calls", ctypes.c_void_p), ("ncalls", ctypes.c_int64),
                ("ports", ctypes.c_void_p), ("nports", ctypes.c_int64),
                ("reads", ctypes.c_void_p), ("nreads", ctypes.c_int64),
                ("writes", ctypes.c_void_p), ("nwrites", ctypes.c_int64),
                ("lazy", ctypes.c_void_p), ("nlazy", ctypes.c_int64)]


_READY = []          # [bound] once the driver entry points are prepared on the DLL


def _lib():
    """The cbtrace.dll native_trace loaded, with the driver entry points bound, or None."""
    lib = native_trace._load()
    if lib is None:
        return None
    if not _READY:
        lib.cbt_drv_begin.argtypes = (ctypes.POINTER(_Config),)
        lib.cbt_drv_begin.restype = ctypes.c_void_p
        lib.cbt_drv_attach.argtypes = (ctypes.c_void_p,)
        lib.cbt_drv_attach.restype = ctypes.c_int
        lib.cbt_drv_detach.argtypes = (ctypes.c_void_p,)
        lib.cbt_drv_detach.restype = ctypes.c_int
        lib.cbt_drv_free.argtypes = (ctypes.c_void_p,)
        lib.cbt_drv_free.restype = None
        lib.cbt_drv_state.argtypes = (ctypes.c_void_p, ctypes.POINTER(ctypes.c_int64))
        lib.cbt_drv_state.restype = None
        lib.cbt_drv_result_get.argtypes = (ctypes.c_void_p,)
        lib.cbt_drv_result_get.restype = ctypes.POINTER(_Result)
        _READY.append(True)
    return lib


def enabled():
    """The driver recorder for this process: the DLL loads here (the Python hooks it replaced are gone;
    require() raises with why_not() when it does not)."""
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


# driver.REGISTERS and emulator._RET (exit computed in driver.run after the run).
_EXITS = {1: "wild", 2: "exception: service number unreadable"}


class Run:
    """One driver run's native hooks on `uc`. `spans` the image's [(lo, hi)]; `scratch` the
    scratch region (lo, hi); `reg_ids` the Unicorn ids of eax,ebx,ecx,edx,esi,edi,ebp,esp,eip,
    eflags; `answers(device, service)` -> the place in REGISTERS the service answers in (or -1).
    `gov` emulator._GOV. Raises nothing; `enabled()` gates its use."""

    def __init__(self, uc, *, reg_ids, ins_in, ins_out, vm_addr, ret, scratch, spans, cap,
                 service_eax, service_carry, gov, answers, shadow=None):
        lib = self.lib = _lib()
        self.uc, self.gov, self.answers_fn, self.exc = uc, gov, answers, None
        self._answers = ANSWERS(self._resolve)
        self._spans = _u64s([x for lo, hi in spans for x in (lo, hi)])
        c = self.config = _Config()
        c.uch = uc._uch.value if hasattr(uc._uch, "value") else uc._uch
        c.reg_ids = (ctypes.c_int32 * 10)(*reg_ids)
        c.ins_in, c.ins_out = int(ins_in), int(ins_out)
        c.vm_addr, c.ret = int(vm_addr), int(ret)
        c.scratch_lo, c.scratch_hi = int(scratch[0]), int(scratch[1])
        c.spans, c.nspans = self._spans[0], len(spans)
        c.cap = int(cap)
        c.service_eax, c.service_carry = int(service_eax) & 0xFFFFFFFF, int(service_carry)
        c.gov_n, c.gov_ceil = int(gov["n"]), int(gov["ceil"] or 0)
        c.answers = self._answers
        c.shadow = shadow
        self.handle = lib.cbt_drv_begin(ctypes.byref(c))
        if not self.handle:
            raise RuntimeError("cbtrace: a driver run could not be set up")
        e = lib.cbt_drv_attach(self.handle)
        if e:
            import unicorn
            raise unicorn.UcError(e)
        self._state = (ctypes.c_int64 * 3)()

    def _resolve(self, device, service, out):
        """Which register the service answers in (driver.answers_in), or -1."""
        try:
            out[0] = int(self.answers_fn(device, service))
            return 0
        except BaseException as e:
            if self.exc is None:
                self.exc = e
            return 1

    def raised(self):
        """The exception the resolver raised, if any (driver.run has no try around the hook, so a
        resolver that raised comes out of emu_start as a UcError; this is the original)."""
        self.lib.cbt_drv_state(self.handle, self._state)
        self.gov["n"] = self._state[2]
        e, self.exc = self.exc, None
        return e

    def collect(self, rec):
        """The rec fields the Python hooks would have left (driver.run's rec), as data."""
        r = self.lib.cbt_drv_result_get(self.handle).contents
        self.gov["n"] = r.gov_n
        rec["insns"] = r.insns
        rec["truncated"] = bool(r.truncated)
        if r.governed:
            rec["governed"] = True
        rec["outside"] = r.outside
        if r.exit_code == 3:
            rec["exit"] = "int:%02x" % r.exit_int
        elif r.exit_code in _EXITS:
            rec["exit"] = _EXITS[r.exit_code]
        rec["cover"] = set((ctypes.c_uint64 * r.ncover).from_address(r.cover)) if r.ncover else set()
        calls = []
        for w in _words(r.calls, r.ncalls, "<%dQ" % _CALL_WORDS):
            first = None if w[0] else w[1]
            regs = tuple(w[4:4 + w[3]]) if w[3] else ()
            words = tuple(w[11:11 + w[10]]) if w[10] else ()
            calls.append((first, w[2]) + (regs, words))
        rec["calls"] = calls
        rec["ports"] = [(("in", "out")[w[0]], w[1], w[2], w[3])
                        for w in _words(r.ports, r.nports, "<%dQ" % _PORT_WORDS)]
        rec["mem_reads"] = [{"pc": w[0], "addr": w[1], "size": w[2], "value": None if w[4] else w[3], "n": w[5]}
                            for w in _words(r.reads, r.nreads, "<%dQ" % _MEM_WORDS)]
        rec["mem_writes"] = [{"pc": w[0], "addr": w[1], "size": w[2], "value": w[3], "n": w[5], "old": w[7]}
                             for w in _words(r.writes, r.nwrites, "<%dQ" % _MEM_WORDS)]
        rec["lazy_maps"] = [{"pc": w[0], "addr": w[1], "page": w[2], "access": ("read", "write", "fetch", "?")[w[3]],
                             "size": w[4]} for w in _words(r.lazy, r.nlazy, "<%dQ" % _LAZY_WORDS)]
        if r.memops >= 0:
            rec["memops"] = r.memops
        if r.writes_seen >= 0:
            rec["writes_seen"] = r.writes_seen
        if r.stuck >= 0:
            rec["_stuck"] = r.stuck

    def detach(self):
        if self.lib.cbt_drv_detach(self.handle):
            raise RuntimeError("cbtrace: a driver hook could not be taken off")

    def __del__(self):
        h, self.handle = getattr(self, "handle", None), None
        if h:
            self.lib.cbt_drv_free(h)


def require():
    """The recorder is C++ only (the Python hooks it replaced are gone): raise when the DLL cannot
    be used here, saying why."""
    if not enabled():
        raise RuntimeError("the run recorder is native (native/tracer, cbtrace.dll): %s" % why_not())
