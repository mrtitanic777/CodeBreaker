r"""native.py -- codebreaker/symbols/ in C++ (native/symbols: cbsymbols.dll), through ctypes.

A thin shim: each question goes to the DLL with its text as code points and comes back as JSON,
so callers keep the very Python values they had.

    native.ask("demangle.parse", name)    the answer (a dict, a str ...), or None for no answer

The switch: CB_NATIVE_SYMBOLS (default 1). The Python it replaced is deleted, so without the
built DLL these questions fail loudly (why_not() says why).
Build: native/symbols/CMakeLists.txt (cmake + ninja in a vcvars64 shell).
"""
import ctypes
import json
import os

from codebreaker.base import settings

_VERSION = 1
_LIB = []
_WHY = []


def dll_path():
    return os.path.join(settings.get("symbols_bin"), "cbsymbols.dll")


def _load():
    if _LIB:
        return _LIB[0]
    lib = None
    try:
        path = dll_path()
        if not os.path.isfile(path):
            raise OSError("not built: %s (native/symbols/CMakeLists.txt)" % path)
        lib = ctypes.CDLL(path)
        lib.cbs_version.restype = ctypes.c_int
        if lib.cbs_version() != _VERSION:
            raise OSError("cbsymbols.dll is another version: rebuild native/symbols")
        lib.cbs_ask.restype = ctypes.c_int
        lib.cbs_ask.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_size_t,
                                ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_size_t))
        lib.cbs_free.argtypes = (ctypes.c_void_p,)
    except OSError as e:
        _WHY.append(str(e))
        lib = None
    _LIB.append(lib)
    return lib


def why_not():
    _load()
    return _WHY[0] if _WHY else None


def ask(question, text):
    """`question` about `text` (a str): the JSON answer as Python values, or None."""
    lib = _load()
    if lib is None:
        raise RuntimeError("cbsymbols.dll: %s" % why_not())
    raw = text.encode("utf-32-le", "surrogatepass")
    out, size = ctypes.c_void_p(), ctypes.c_size_t()
    rc = lib.cbs_ask(question.encode("ascii"), raw, len(raw) // 4, ctypes.byref(out), ctypes.byref(size))
    if rc < 0:
        raise ValueError("cbsymbols.dll does not know %r" % question)
    if rc == 0:
        return None
    try:
        return json.loads(ctypes.string_at(out.value, size.value).decode("ascii"))
    finally:
        lib.cbs_free(out)
