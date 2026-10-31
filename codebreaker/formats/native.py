r"""native.py -- the file-format readers in C++ (native/formats: formats.dll), through ctypes.

A thin, temporary shim: each reader's C++ (native/formats/<reader>/) answers the questions the
Python reader answered -- PEFile.exports(), NEFile.segments(), ... -- and this module hands its
answers back as the very Python objects the callers already use, so no caller changes. Until a
reader's Python is deleted (docs/NATIVE.md, step 4), it stays beside it as the fallback.

    native.enabled("pe")                   the switch for one reader (CB_NATIVE_FORMATS)
    native.call("pe.exports", data, *args) one question: the answer, or the exception the
                                           Python reader would raise
    native.PEFile(path)                    a pe.PEFile whose every reader is the C++ one

The switch: CB_NATIVE_FORMATS (default below; 0 = the Python readers), or per reader
CB_NATIVE_FORMATS_PE=0/1 and so on, which wins over it. Without the built DLL the Python readers
run (why_not() says why). A question the C++ leaves to Python (an int past 64 bits, a branch that
runs the emulator) is answered by the Python reader, as if the switch were off for that call.

Answers and arguments cross as pickle streams (native/formats/cbformats.h): Python reads them at
C speed, and the DLL depends on no Python version.
Build: native/formats/CMakeLists.txt (cmake + ninja in a vcvars64 shell).
"""
import ctypes
import inspect as _inspect
import os
import pickle
import struct

from codebreaker.base import settings

DEFAULT = "0"             # CB_NATIVE_FORMATS when it is not set (docs/NATIVE.md: on once gated)
READERS_ON = ()           # readers gated identical and on by default (each may still be set off)
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
        value = "1" if reader in READERS_ON else DEFAULT
    return value not in ("", "0") and _load() is not None


def questions():
    lib = _load()
    return lib.cbf_questions().decode("ascii").split() if lib else []


def _exception(kind, message):
    if kind == "struct.error":
        return struct.error(message)
    if kind == "UnicodeDecodeError":
        return UnicodeDecodeError("ascii", b"", 0, 1, message)
    return _BUILTIN.get(kind, RuntimeError)(message)


_BUILTIN = {e.__name__: e for e in (ValueError, IndexError, KeyError, TypeError, MemoryError, ZeroDivisionError,
                                    OverflowError, AttributeError, OSError, RuntimeError)}


def call(question, data, *args):
    """Ask the C++ one question about `data` (bytes, bytearray or memoryview; None for none)."""
    lib = _LIB[0] if _LIB else _load()
    if data is None:
        buf, size = None, 0
    elif isinstance(data, bytes):
        buf, size = data, len(data)
    elif isinstance(data, bytearray) and data:
        buf, size = ctypes.addressof((ctypes.c_char * len(data)).from_buffer(data)), len(data)
    else:
        data = bytes(data)
        buf, size = data, len(data)
    blob = pickle.dumps(args, 4)
    out, n = ctypes.c_void_p(), ctypes.c_size_t()
    status = lib.cbf_call(question.encode("ascii"), buf, size, blob, len(blob), ctypes.byref(out), ctypes.byref(n))
    raw = ctypes.string_at(out.value, n.value) if n.value else b""
    if status == _OK:
        return pickle.loads(raw)
    if status == _RAISED:
        kind, message = pickle.loads(raw)
        raise _exception(kind, message)
    if status == _FALLBACK:
        raise Fallback(raw.decode("utf-8", "replace"))
    raise RuntimeError("formats.dll answers no question %r: rebuild native/formats" % question)


def _asker(question, python, *, data="data"):
    """A method that asks `question` about self.<data> with the call's arguments, the Python
    reader's own `python` answering what the C++ leaves to it. Keyword arguments are bound to
    the Python method's signature, so a call reads as it did."""
    sig = _inspect.signature(python)

    def method(self, *args, **kw):
        if kw:
            bound = sig.bind(self, *args, **kw)
            bound.apply_defaults()
            args = bound.args[1:]
        try:
            return call(question, getattr(self, data), *args)
        except Fallback:
            return python(self, *args)
    method.__name__, method.__doc__, method.__qualname__ = python.__name__, python.__doc__, python.__qualname__
    return method


def _function(question, python, data_arg=0):
    """A module-level function answered by the C++ (`data_arg`: which argument is the bytes, or
    None when it takes none)."""
    sig = _inspect.signature(python)

    def function(*args, **kw):
        if kw:
            bound = sig.bind(*args, **kw)
            bound.apply_defaults()
            args = bound.args
        try:
            if data_arg is None:
                return call(question, None, *args)
            return call(question, args[data_arg], *(args[:data_arg] + args[data_arg + 1:]))
        except Fallback:
            return python(*args)
    function.__name__, function.__doc__ = python.__name__, python.__doc__
    return function


# ---- PE: formats/pe.py ------------------------------------------------------------------------
from codebreaker.formats import pe as _pe      # noqa: E402

_PE_METHODS = ("machine_name", "is_dll", "_rva_to_off", "code_bytes", "module_name", "export_addrs", "exports",
               "imports", "code_sections", "base_relocs", "build_meta", "debug_codeview", "_u", "_text",
               "file_offset", "optional_header", "directory", "export_table", "import_table", "resource_tree",
               "reloc_blocks", "tls", "debug_entries", "certificates", "clr", "rich")


class PEFile(_pe.PEFile):
    """pe.PEFile with every reader in C++ (the same attributes; methods ask formats.dll)."""

    def __init__(self, path):
        self.path = path
        with open(path, "rb") as f:
            self.data = f.read()
        try:
            self.__dict__.update(call("pe.parse", self.data, path))
        except Fallback:
            self._py_init()

    def _py_init(self):
        d = self.data
        if d[:2] != b"MZ":
            raise ValueError(f"{self.path}: no MZ header")
        self.e_lfanew = struct.unpack("<I", d[0x3C:0x40])[0]
        if d[self.e_lfanew:self.e_lfanew + 4] != b"PE\0\0":
            raise ValueError(f"{self.path}: not a PE image")
        _pe.PEFile._parse(self)


for _name in _PE_METHODS:
    setattr(PEFile, _name, _asker("pe." + _name, getattr(_pe.PEFile, _name)))

is_dotnet = _function("pe.is_dotnet", _pe._is_dotnet, data_arg=None)
