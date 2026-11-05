r"""native.py -- the file-format readers in C++ (native/formats: formats.dll), through ctypes.

The readers of codebreaker/formats/ whose Python is gone (pe, ne, le, dos, omf, xex: docs/NATIVE.md,
step 4) are their APIs only -- each def a signature and a docstring -- answered by formats.dll:
their answers come back as the very Python objects the callers use, so no caller changed.

    native.call("pe.exports", data, *args) one question: the answer, or the exception the
                                           Python reader raised
    @native.ask("ne.segments", where)      a method answered about self.data (where(self): the
                                           object's place, passed first)
    @native.answered("dos.read", ...)      a module function answered

Answers and arguments cross as pickle streams (native/formats/cbformats.h): Python reads them at
C speed, and the DLL depends on no Python version. Without the built DLL the readers raise the
OSError native.required() gives (why_not() says why).
Build: native/formats/CMakeLists.txt (cmake + ninja in a vcvars64 shell).
"""
import ctypes
import functools
import inspect as _inspect
import io
import os
import pickle
import struct

from codebreaker.base import settings

_VERSION = 1
_LIB = []
_WHY = []
_OK, _RAISED, _FALLBACK, _NO_SUCH = 0, 1, 2, 3


class Fallback(Exception):
    """The C++ leaves this question to its caller (omf.read: an a.out object, which exact/aout.py reads)."""


def dll_path():
    return os.path.join(settings.get("formats_bin"), "formats.dll")


def _load():
    if _LIB:
        return _LIB[0]
    lib = None
    try:
        path = dll_path()
        if not os.path.isfile(path):
            raise OSError("not built: %s (native/formats/CMakeLists.txt)" % path)
        lib = settings.load_native(path)
        lib.cbf_version.restype = ctypes.c_int
        if lib.cbf_version() != _VERSION:
            raise OSError("formats.dll is another version: rebuild native/formats")
        lib.cbf_call.restype = ctypes.c_int
        lib.cbf_call.argtypes = (ctypes.c_char_p, ctypes.c_void_p, ctypes.c_size_t, ctypes.c_char_p, ctypes.c_size_t,
                                 ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_size_t))
        lib.cbf_questions.restype = ctypes.c_char_p
        lib.cbf_unicorn.argtypes = (ctypes.c_wchar_p, ctypes.c_char_p, ctypes.c_size_t)
        uc = _unicorn_dll()
        if uc:                                   # emulated reads (a packed program's unpacker) run on it
            lib.cbf_unicorn(uc, ctypes.create_string_buffer(256), 256)
    except OSError as e:
        _WHY.append(str(e))
        lib = None
    _LIB.append(lib)
    return lib


def _unicorn_dll():
    """The unicorn.dll Python's `unicorn` package carries, found without importing it."""
    import importlib.util
    spec = importlib.util.find_spec("unicorn")
    for where in (spec.submodule_search_locations or []) if spec else []:
        path = os.path.join(where, "lib", "unicorn.dll")
        if os.path.isfile(path):
            return os.path.abspath(path)
    return None


def capstone_dll():
    """The capstone.dll Python's `capstone` package carries (formats.dll decodes with that very one)."""
    import importlib.util
    spec = importlib.util.find_spec("capstone")
    for where in (spec.submodule_search_locations or []) if spec else []:
        path = os.path.join(where, "lib", "capstone.dll")
        if os.path.isfile(path):
            return os.path.abspath(path)
    raise OSError("capstone.dll not found: is the `capstone` package installed?")


def why_not():
    _load()
    return _WHY[0] if _WHY else None


def questions():
    lib = _load()
    return lib.cbf_questions().decode("ascii").split() if lib else []


_BUILTIN = {e.__name__: e for e in (ValueError, IndexError, KeyError, TypeError, MemoryError, ZeroDivisionError,
                                    OverflowError, AttributeError, OSError, RuntimeError, StopIteration,
                                    FileNotFoundError, PermissionError, RecursionError)}
RAISES = {}               # a reader's own exception classes by name (cab: "CabError"), filled by its module


def _exception(kind, message):
    if kind == "struct.error":
        return struct.error(message)
    if kind in RAISES:
        return RAISES[kind](message)
    if kind == "zlib.error":
        import zlib
        return zlib.error(message)
    if kind == "UnicodeDecodeError":
        return UnicodeDecodeError("ascii", b"", 0, 1, message)
    if kind == "UnicodeEncodeError":
        return UnicodeEncodeError("latin-1", "", 0, 1, message)
    if kind == "KeyError":
        return KeyError(message.strip("'"))
    return _BUILTIN.get(kind, RuntimeError)(message)


_QUESTION = {}            # question -> its bytes
_NO_ARGS = pickle.dumps((), 4)


def _dumps(args):
    """The arguments pickled without a memo: an object passed twice is written twice, so the C++
    reads each value whole where it stands (it keeps no memo of objects still being filled)."""
    f = io.BytesIO()
    p = pickle.Pickler(f, 4)
    p.fast = True
    p.dump(args)
    return f.getvalue()


def call(question, data, *args):
    """Ask the C++ one question about `data` (bytes, bytearray or memoryview; None for none)."""
    lib = _LIB[0] if _LIB else _load()
    if lib is None:
        lib = required()
    q = _QUESTION.get(question)
    if q is None:
        q = _QUESTION[question] = question.encode("ascii")
    if isinstance(data, bytes):
        buf, size = data, len(data)
    elif data is None:
        buf, size = None, 0
    elif isinstance(data, bytearray) and data:
        buf, size = ctypes.addressof((ctypes.c_char * len(data)).from_buffer(data)), len(data)
    else:
        data = bytes(data)
        buf, size = data, len(data)
    blob = _dumps(args) if args else _NO_ARGS
    out, n = ctypes.c_void_p(), ctypes.c_size_t()
    status = lib.cbf_call(q, buf, size, blob, len(blob), ctypes.byref(out), ctypes.byref(n))
    raw = ctypes.string_at(out.value, n.value) if n.value else b""
    if status == _OK:
        return pickle.loads(raw)
    if status == _RAISED:
        kind, message = pickle.loads(raw)
        raise _exception(kind, message)
    if status == _FALLBACK:
        raise Fallback(raw.decode("utf-8", "replace"))
    raise RuntimeError("formats.dll answers no question %r: rebuild native/formats" % question)


def _positional(sig, args, kw, skip=0):
    """A call's arguments as the positional tuple the Python function would have bound."""
    if not kw:
        return args
    bound = sig.bind(*args, **kw)
    bound.apply_defaults()
    return bound.args[skip:]




# ---- readers whose Python is gone: the C++ answers every call -------------------------------------
def required():
    """The loaded DLL, or the OSError saying why a reader has none."""
    lib = _LIB[0] if _LIB else _load()
    if lib is None:
        raise OSError("formats.dll is required: %s" % why_not())
    return lib


def ask(question, where=lambda self: (), data="data", result=None):
    """Decorator: the method (its def a signature and a docstring) answered by the C++ about
    self.<data>, after the object's place where(self) and the call's own arguments;
    `result(self, answer)` turns an answer into what the method returns."""
    def wrap(stub):
        sig = _inspect.signature(stub)

        def method(self, *args, **kw):
            args = _positional(sig, (self,) + args, kw, 1) if kw else args
            answer = call(question, getattr(self, data), *where(self), *args)
            return result(self, answer) if result else answer
        return functools.wraps(stub)(method)
    return wrap


def answered(question, data_arg=0, args_of=None, result=None):
    """Decorator: the module function (its def a signature and a docstring) answered by the C++.
    `data_arg`: which argument is the bytes (None: none); or `args_of(*args) -> (data, args)`;
    `result(answer, data, *args)` turns the answer into the function's value."""
    def wrap(stub):
        sig = _inspect.signature(stub)

        def function(*args, **kw):
            args = _positional(sig, args, kw)
            if args_of is not None:
                data, rest = args_of(*args)
            elif data_arg is None:
                data, rest = None, args
            else:
                data, rest = args[data_arg], args[:data_arg] + args[data_arg + 1:]
            answer = call(question, data, *rest)
            return result(answer, data, *args) if result else answer
        return functools.wraps(stub)(function)
    return wrap
