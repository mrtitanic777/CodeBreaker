"""native_invert.py -- the compiler models (invert/) answered by C++ (native/invert: cbinvert.dll), through ctypes.

The models run in C++; Python hands their input over as text and turns the JSON answer back
into the same objects (flow16's Blocks and structure tuples), or raises the exception the
Python raised at the same place.  There is no Python fallback: the Python each piece replaced
was deleted once the gate passed (docs/NATIVE.md, "the compiler models").

Build: native/invert/CMakeLists.txt (cmake + ninja in a vcvars64 shell).
"""
import ctypes
import json
import os

from codebreaker.base import settings

_LIB = []
_RAISE = {1: IndexError, 2: ValueError, 3: RuntimeError}


def dll_path():
    """Where cbinvert.dll is: the `invert_bin` setting (default native/invert/build/bin)."""
    return os.path.join(settings.get("invert_bin"), "cbinvert.dll")


def _load():
    if _LIB:
        return _LIB[0]
    path = dll_path()
    if not os.path.isfile(path):
        raise OSError("cbinvert.dll is not built: %s (native/invert/CMakeLists.txt)" % path)
    lib = ctypes.CDLL(path)
    if lib.cbinv_version() != 1:
        raise OSError("cbinvert.dll is another version: rebuild native/invert")
    lib.cbinv_cfg.argtypes = (ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.c_void_p))
    lib.cbinv_structure.argtypes = (ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_void_p))
    lib.cbinv_free.argtypes = (ctypes.c_void_p,)
    _LIB.append(lib)
    return lib


def _call(fn, text, *args):
    p = ctypes.c_void_p()
    status = fn(text.encode("utf-8"), *args, ctypes.byref(p))
    try:
        answer = ctypes.string_at(p.value).decode("utf-8")
    finally:
        _load().cbinv_free(p)
    if status:
        raise _RAISE.get(status, RuntimeError)(answer)
    return answer


def cfg(insns, switch, cmpswitch):
    """flow16.build_cfg's blocks as rows: [lo, hi, term, succ, cond_idx, table, cases, default,
    selreg, selbase].  `switch(ins)`: the SWITCH hook's answer for an indirect jmp (None: none)."""
    rows = ["%d\t%s\t%s" % (i.address, i.mnemonic, i.op_str) for i in insns]
    for i in insns:
        if i.mnemonic == "jmp" and "[" in i.op_str:
            t = switch(i)
            if t is not None:
                rows.append("S\t%d\t%s" % (i.address, ",".join(str(int(x)) for x in t)))
    return json.loads(_call(_load().cbinv_cfg, "\n".join(rows) + "\n", 1 if cmpswitch else 0))["b"]


def structure(blocks, entry, nocap):
    """flow16.structure's tree, as JSON lists (flow16 makes them tuples)."""
    def ints(v):
        return ",".join(str(x) for x in v)
    rows = ["%s\t%d\t%d\t%s\t%s\t%s\t%s" % (
        b.term, b.lo, b.hi, ints(b.succ),
        "-" if b.cases is None else ",".join("%d:%d" % (v, t) for v, t in b.cases),
        "-" if b.default is None else b.default, "-" if b.table is None else ints(b.table)) for b in blocks]
    return json.loads(_call(_load().cbinv_structure, "\n".join(rows) + "\n", int(entry), 1 if nocap else 0))
