r"""native_shadow.py -- the path explorer's symbolic shadow of an x86 run in C++ (native/tracer:
cbtrace.dll, shadow.cc), through ctypes.

    sh = native_shadow.Shadow(16 | 32 | 33, lifter, inputs)   # Shadow16 / Shadow32 / ShadowLE

The same object emulate/shadow.py's x86 shadows were: enter(), step(), after_call(), written(),
forget(), segbase(); and after the run path, trail, vars, lost, pins, steps, off -- read back from
the DLL as the same values (the conditions as emulate/symbolic.py's tuples). A recorder in the same
DLL (emulator.trace_function, windows16.trace_function16, driver.run) is handed `handle` and steps
it itself, so a shadowed run calls no Python per instruction; Python is asked only for the p-code
of an instruction not lifted before in the process (pcode.Lifter.line).

`inputs`: [(low, high)] -- the memory bytes that are inputs of the run until written.

The switch: CB_NATIVE_SHADOW (see DEFAULT); 0, or without the DLL, the Python shadow in
emulate/shadow.py runs instead.
"""
import ctypes
import os
import struct

from codebreaker.emulate import native_trace     # the loaded DLL

DEFAULT = "1"             # CB_NATIVE_SHADOW when not set (gated identical: docs/NATIVE.md)
LIFT = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.POINTER(ctypes.c_uint8), ctypes.c_int32, ctypes.c_uint64,
                        ctypes.c_void_p, ctypes.c_int32)
_FLAGS = {"CF": 0, "PF": 2, "AF": 4, "ZF": 6, "SF": 7, "TF": 8, "IF": 9, "DF": 10, "OF": 11}
_OPS = ("var", "const", "add", "sub", "and", "or", "xor", "mul", "udiv", "urem", "sdiv", "srem", "neg",
        "not", "shl", "lshr", "ashr", "zext", "sext", "extract", "concat", "ite", "popcount", "bool",
        "eq", "ult", "ule", "slt", "sle", "bnot", "band", "bor", "bxor", "nz", "true")
_KINDS = ("jump", "inner", "pin")
_VKINDS = ("arg", "reg", "mem", "ret", "carry")
_NONE = 0xFFFFFFFFFFFFFFFF


class _Config(ctypes.Structure):
    _fields_ = [("kind", ctypes.c_int32), ("mode", ctypes.c_char_p), ("nregs", ctypes.c_int32),
                ("reg_names", ctypes.POINTER(ctypes.c_char_p)),
                ("reg_off", ctypes.POINTER(ctypes.c_int64)), ("reg_size", ctypes.POINTER(ctypes.c_int64)),
                ("reg_uc", ctypes.POINTER(ctypes.c_int32)), ("reg_flag", ctypes.POINTER(ctypes.c_int32)),
                ("eflags_id", ctypes.c_int32),
                ("inputs", ctypes.POINTER(ctypes.c_uint64)), ("ninputs", ctypes.c_int64),
                ("gdt_addr", ctypes.c_uint64), ("lift", LIFT), ("real_mode", ctypes.c_int32)]


class _Result(ctypes.Structure):
    _fields_ = [("nodes", ctypes.c_void_p), ("nnodes", ctypes.c_int64),
                ("path", ctypes.c_void_p), ("npath", ctypes.c_int64),
                ("trail", ctypes.c_void_p), ("ntrail", ctypes.c_int64),
                ("vars", ctypes.c_void_p), ("nvars", ctypes.c_int64),
                ("lost", ctypes.c_void_p), ("nlost", ctypes.c_int64),
                ("strings", ctypes.c_void_p), ("strings_len", ctypes.c_int64), ("nstrings", ctypes.c_int64),
                ("pins", ctypes.c_int64), ("steps", ctypes.c_int64), ("off", ctypes.c_int64)]


_READY = []


def _lib():
    lib = native_trace._load()
    if lib is None or not hasattr(lib, "cbt_sh_new"):
        return None
    if not _READY:
        V, U64, I32, I64, P = ctypes.c_void_p, ctypes.c_uint64, ctypes.c_int32, ctypes.c_int64, ctypes.c_char_p
        lib.cbt_sh_new.argtypes, lib.cbt_sh_new.restype = (ctypes.POINTER(_Config),), V
        lib.cbt_sh_free.argtypes, lib.cbt_sh_free.restype = (V,), None
        lib.cbt_sh_enter.argtypes, lib.cbt_sh_enter.restype = (V, P, I32, I64, P, U64, I32, I32, U64, I64), ctypes.c_int
        lib.cbt_sh_step.argtypes, lib.cbt_sh_step.restype = (V, V, U64, U64), None
        lib.cbt_sh_after_call.argtypes = (V, V, I32, U64, I32, U64, I64, P, I32, U64)
        lib.cbt_sh_after_call.restype = None
        lib.cbt_sh_written.argtypes, lib.cbt_sh_written.restype = (V, U64, U64), None
        lib.cbt_sh_forget.argtypes, lib.cbt_sh_forget.restype = (V, P), None
        lib.cbt_sh_segbase.argtypes, lib.cbt_sh_segbase.restype = (V, V, U64), U64
        lib.cbt_sh_export.argtypes, lib.cbt_sh_export.restype = (V,), ctypes.POINTER(_Result)
        _READY.append(True)
    return lib


def enabled():
    """The C++ shadow for this process: CB_NATIVE_SHADOW is not 0 and the DLL has it."""
    if os.environ.get("CB_NATIVE_SHADOW", DEFAULT) == "0" or os.environ.get("WT_DBG"):
        return False
    return _lib() is not None


def handle_of(shadow):
    """The DLL's handle of a shadow a recorder can step itself, or None (a Python shadow)."""
    if not isinstance(shadow, Shadow):
        return None
    shadow._got = None                    # (the recorder steps it from here on)
    return shadow.handle


def _uch(uc):
    h = uc._uch
    return h.value if hasattr(h, "value") else h


def _words(ptr, n, k):
    if not n:
        return []
    return list(struct.iter_unpack("<%dQ" % k, ctypes.string_at(ptr, n * 8 * k)))


class Shadow(object):
    """One run's shadow in the DLL. kind 16 (a 16-bit run; selectors' bases read from the GDT),
    32 (flat), 33 (a driver's flat ring-0 run: a service call also sets the carry flag to `carry`)."""

    def __init__(self, kind, lifter, inputs, span=None, real_mode=False):
        from unicorn import x86_const as X
        from codebreaker.emulate import windows16
        lib = self.lib = _lib()
        self.lifter, self.span, self.kind = lifter, span, kind
        names = list(lifter.regs)
        self._names = [n.encode() for n in names]
        c = self.config = _Config()
        c.kind, c.mode, c.nregs = kind, lifter.mode.encode(), len(names)
        c.reg_names = (ctypes.c_char_p * max(1, len(names)))(*self._names)
        c.reg_off = (ctypes.c_int64 * max(1, len(names)))(*[lifter.regs[n][0] for n in names])
        c.reg_size = (ctypes.c_int64 * max(1, len(names)))(*[lifter.regs[n][1] for n in names])
        c.reg_uc = (ctypes.c_int32 * max(1, len(names)))(*[-1 if n in _FLAGS else
                                                           getattr(X, "UC_X86_REG_" + n, -1) for n in names])
        c.reg_flag = (ctypes.c_int32 * max(1, len(names)))(*[_FLAGS.get(n, -1) for n in names])
        c.eflags_id = X.UC_X86_REG_EFLAGS
        flat = [int(v) for lo_hi in inputs for v in lo_hi]
        c.inputs, c.ninputs = (ctypes.c_uint64 * max(1, len(flat)))(*flat), len(flat) // 2
        c.gdt_addr = windows16.GDT_ADDR
        c.real_mode = 1 if real_mode else 0
        c.lift = self._lift_cb = LIFT(self._lift)
        self.handle = lib.cbt_sh_new(ctypes.byref(c))
        self._got = None

    def _lift(self, code, n, offset, out, cap):
        try:
            line = self.lifter.line(ctypes.string_at(code, n), offset)
            if line is None:
                return -1
            data = line.encode()
            if len(data) < cap:
                ctypes.memmove(out, data + b"\0", len(data) + 1)
            return len(data)
        except BaseException as e:
            name = type(e).__name__.encode()[:cap - 1]
            ctypes.memmove(out, name + b"\0", len(name) + 1)
            return -2

    def __del__(self):
        h, self.handle = getattr(self, "handle", None), None
        if h:
            self.lib.cbt_sh_free(h)

    # --- what the runner calls ----------------------------------------------------------------
    def enter(self, places, bits=16):
        """As shadow.Shadow.enter: argument k, `bits` wide, is (where, value[, k])."""
        self._got = None
        for n, item in enumerate(places):
            at, value = item[0], item[1]
            k = item[2] if len(item) > 2 else n
            if isinstance(k, int):
                name, vkind, wi, ws = "a%d" % k, 0, k, None
            else:
                name, vkind, wi, ws = k, 1, 0, k.encode()
            if isinstance(at, str):
                off, size = self.lifter.regs[at]
                self.lib.cbt_sh_enter(self.handle, name.encode(), vkind, wi, ws, int(value) & _NONE, bits, 1, off, size)
            else:
                self.lib.cbt_sh_enter(self.handle, name.encode(), vkind, wi, ws, int(value) & _NONE, bits, 0, at, 0)

    def step(self, m, address, cs_base=0):
        self._got = None
        self.lib.cbt_sh_step(self.handle, _uch(m), address, cs_base)

    def after_call(self, m, symbolic_result, value, args_at=None, nargs=0, why=None, word=2, carry=0):
        self._got = None
        self.lib.cbt_sh_after_call(self.handle, _uch(m), 1 if symbolic_result else 0, int(value) & _NONE,
                                   0 if args_at is None else 1, args_at or 0, nargs,
                                   why.encode() if why else None, word, int(carry) & _NONE)

    def written(self, at, size):
        self._got = None
        self.lib.cbt_sh_written(self.handle, at, size)

    def forget(self, names):
        self._got = None
        for n in names:
            self.lib.cbt_sh_forget(self.handle, n.encode())

    def segbase(self, m, sel):
        return self.lib.cbt_sh_segbase(self.handle, _uch(m), sel)

    # --- what the run made ----------------------------------------------------------------------
    def _export(self):
        if self._got is not None:
            return self._got
        r = self.lib.cbt_sh_export(self.handle).contents
        strings = ctypes.string_at(r.strings, r.strings_len).decode().split("\0")[:r.nstrings] if r.strings_len else []
        nodes = []
        for op, bits, k0, k1, k2, lo, vlo, vhi in _words(r.nodes, r.nnodes, 8):
            name = _OPS[op]
            if bits >= 1 << 63:
                bits -= 1 << 64
            if name == "var":
                e = ("var", bits, strings[vlo])
            elif name == "const":
                e = ("const", bits, vlo | (vhi << 64))
            elif name == "extract":
                e = ("extract", bits, nodes[k0], lo - (1 << 64) if lo >= 1 << 63 else lo)
            elif name == "true":
                e = ("true", 0)
            else:
                e = (name, bits) + tuple(nodes[k] for k in (k0, k1, k2) if k != _NONE)
            nodes.append(e)
        path = [(site, bool(taken), nodes[node], _KINDS[kind]) for site, taken, node, kind in _words(r.path, r.npath, 4)]
        trail = [(site, bool(taken)) for site, taken in _words(r.trail, r.ntrail, 2)]
        variables = {}
        for name, kind, tag, where, bits, value, _z in _words(r.vars, r.nvars, 7):
            variables[strings[name]] = (_VKINDS[kind], None if tag == 0 else where if tag == 1 else strings[where],
                                        bits, value)
        lost = {strings[i] for (i,) in _words(r.lost, r.nlost, 1)}
        self._got = (path, trail, variables, lost, r.pins, r.steps, bool(r.off))
        return self._got

    path = property(lambda self: self._export()[0])
    trail = property(lambda self: self._export()[1])
    vars = property(lambda self: self._export()[2])
    lost = property(lambda self: self._export()[3])
    pins = property(lambda self: self._export()[4])
    steps = property(lambda self: self._export()[5])
    off = property(lambda self: self._export()[6])
