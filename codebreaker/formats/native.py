r"""native.py -- the file-format readers in C++ (native/formats: formats.dll), through ctypes.

A thin, temporary shim: each reader's C++ (native/formats/<reader>/) answers the questions the
Python reader answered -- PEFile.exports(), NEFile.segments(), ... -- and hands its answers back
as the very Python objects the callers already use, so no caller changes. Until a reader's
Python is deleted (docs/NATIVE.md, step 4), it stays beside it as the fallback. This module is
the plumbing; native_<reader>.py holds each reader's classes (native_pe.PEFile ...).

    native.enabled("pe")                   the switch for one reader (CB_NATIVE_FORMATS)
    native.call("pe.exports", data, *args) one question: the answer, or the exception the
                                           Python reader would raise
    @native.native_first("ne.embedded", "ne")   a reader's function answered by the C++ when
                                           switched on (f.python is the Python one)

The switch: CB_NATIVE_FORMATS (default: READERS_ON below; 0 = the Python readers), or per reader
CB_NATIVE_FORMATS_PE=0/1 and so on, which wins over it. Without the built DLL the Python readers
run (why_not() says why). A question the C++ leaves to Python (an int past 64 bits, a branch that
runs the emulator) is answered by the Python reader, as if the switch were off for that call.

Answers and arguments cross as pickle streams (native/formats/cbformats.h): Python reads them at
C speed, and the DLL depends on no Python version.
Build: native/formats/CMakeLists.txt (cmake + ninja in a vcvars64 shell).
"""
import ctypes
import functools
import inspect as _inspect
import os
import pickle
import struct

from codebreaker.base import settings

READERS_ON = ()           # readers gated identical and on by default (docs/NATIVE.md); each may be set off
_VERSION = 1
_LIB = []
_WHY = []
_OK, _RAISED, _FALLBACK, _NO_SUCH = 0, 1, 2, 3


class Fallback(Exception):
    """The C++ leaves this question to the Python reader."""


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
        lib = ctypes.CDLL(path)
        lib.cbf_version.restype = ctypes.c_int
        if lib.cbf_version() != _VERSION:
            raise OSError("formats.dll is another version: rebuild native/formats")
        lib.cbf_call.restype = ctypes.c_int
        lib.cbf_call.argtypes = (ctypes.c_char_p, ctypes.c_void_p, ctypes.c_size_t, ctypes.c_char_p, ctypes.c_size_t,
                                 ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_size_t))
        lib.cbf_questions.restype = ctypes.c_char_p
    except OSError as e:
        _WHY.append(str(e))
        lib = None
    _LIB.append(lib)
    return lib


def why_not():
    _load()
    return _WHY[0] if _WHY else None


def enabled(reader):
    """Is `reader` ("pe", "ne", ...) answered by the C++?"""
    value = os.environ.get("CB_NATIVE_FORMATS_" + reader.upper())
    if value is None:
        value = os.environ.get("CB_NATIVE_FORMATS")
    if value is None:
        value = "1" if reader in READERS_ON else "0"
    return value not in ("", "0") and _load() is not None


def questions():
    lib = _load()
    return lib.cbf_questions().decode("ascii").split() if lib else []


_BUILTIN = {e.__name__: e for e in (ValueError, IndexError, KeyError, TypeError, MemoryError, ZeroDivisionError,
                                    OverflowError, AttributeError, OSError, RuntimeError, StopIteration)}


def _exception(kind, message):
    if kind == "struct.error":
        return struct.error(message)
    if kind == "UnicodeDecodeError":
        return UnicodeDecodeError("ascii", b"", 0, 1, message)
    if kind == "UnicodeEncodeError":
        return UnicodeEncodeError("latin-1", "", 0, 1, message)
    if kind == "KeyError":
        return KeyError(message.strip("'"))
    return _BUILTIN.get(kind, RuntimeError)(message)


_QUESTION = {}            # question -> its bytes
_NO_ARGS = pickle.dumps((), 4)


def call(question, data, *args):
    """Ask the C++ one question about `data` (bytes, bytearray or memoryview; None for none)."""
    lib = _LIB[0] if _LIB else _load()
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
    blob = pickle.dumps(args, 4) if args else _NO_ARGS
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


def asker(question, python, where=lambda self: (), data="data", result=None):
    """A method that asks `question` about self.<data>, after the object's place where(self) and
    the call's own arguments; the Python reader's own `python` answers what the C++ leaves.
    `result(self, answer)` turns an answer into what the method returns, when it is not the
    answer itself (an object the instance already holds, say)."""
    sig = _inspect.signature(python)

    def method(self, *args, **kw):
        args = _positional(sig, (self,) + args, kw, 1) if kw else args
        try:
            answer = call(question, getattr(self, data), *where(self), *args)
        except Fallback:
            return python(self, *args)
        return result(self, answer) if result else answer
    method.__name__, method.__doc__, method.__qualname__ = python.__name__, python.__doc__, python.__qualname__
    return method


def native_first(question, reader, data_arg=0, args_of=None, result=None):
    """A reader's module-level function, answered by the C++ while `reader` is switched on.
    `data_arg`: which argument is the bytes (None: none); or `args_of(*args) -> (data, args)`
    for a function that takes an object (or a path); `result(answer, data, *args)` turns the
    answer into the function's value when it is not the answer itself. The Python function stays
    as f.python, and answers whatever the C++ leaves to it; f.cpp asks the C++ whatever the
    switch says (the gate)."""
    def wrap(python):
        sig = _inspect.signature(python)

        def cpp(*args, **kw):
            args = _positional(sig, args, kw)
            try:
                if args_of is not None:
                    data, rest = args_of(*args)
                elif data_arg is None:
                    data, rest = None, args
                else:
                    data, rest = args[data_arg], args[:data_arg] + args[data_arg + 1:]
                answer = call(question, data, *rest)
            except Fallback:
                return python(*args)
            return result(answer, data, *args) if result else answer

        @functools.wraps(python)
        def function(*args, **kw):
            if enabled(reader):
                return cpp(*args, **kw)
            return python(*args, **kw)
        function.python, function.cpp = python, cpp
        return function
    return wrap
