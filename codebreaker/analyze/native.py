r"""native.py -- codebreaker/analyze/ in C++ (native/analyze: cbanalyze.dll), through ctypes.

A thin shim. An image is handed to the DLL once (its spans, function starts and limits, the
addresses standing for imports, the pointers the file names) and stays there; questions about it
come back as JSON, so callers keep the very Python values they had.

    h = native.Handle(img)        the image in the DLL (freed with the handle)
    h.ask("image.discover")       the answer as Python values

The Python it replaced is deleted, so without the built DLL these questions fail loudly
(why_not() says why). Build: native/analyze/CMakeLists.txt (tools/build_native.cmd analyze).
"""
import ctypes
import json
import os
import struct

from codebreaker.base import settings

_VERSION = 4
_LIB = []
_WHY = []
_POINTER_KINDS = {"abs32": 0, "far": 1, "selector": 2, "offset": 3, "byte": 4}


def dll_path():
    return os.path.join(settings.get("analyze_bin"), "cbanalyze.dll")


def _load():
    if _LIB:
        return _LIB[0]
    lib = None
    try:
        path = dll_path()
        if not os.path.isfile(path):
            raise OSError("not built: %s (native/analyze/CMakeLists.txt)" % path)
        import capstone                                   # the very capstone.dll Python decodes with
        lib = settings.load_native(path)
        lib.cba_version.restype = ctypes.c_int
        if lib.cba_version() != _VERSION:
            raise OSError("cbanalyze.dll is another version: rebuild native/analyze")
        lib.cba_init.argtypes = (ctypes.c_wchar_p, ctypes.c_char_p, ctypes.c_int)
        err = ctypes.create_string_buffer(256)
        if lib.cba_init(os.path.abspath(capstone._cs._name), err, 256):
            raise OSError(err.value.decode("ascii", "replace"))
        lib.cba_open.restype = ctypes.c_void_p
        lib.cba_open.argtypes = (ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p, ctypes.c_int)
        lib.cba_ask.restype = ctypes.c_int
        lib.cba_ask.argtypes = (ctypes.c_void_p, ctypes.c_char_p, ctypes.c_longlong,
                                ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_size_t))
        lib.cba_names.argtypes = (ctypes.c_void_p, ctypes.c_char_p, ctypes.c_size_t)
        lib.cba_free.argtypes = (ctypes.c_void_p,)
        lib.cba_close.argtypes = (ctypes.c_void_p,)
    except OSError as e:
        _WHY.append(str(e))
        lib = None
    _LIB.append(lib)
    return lib


def why_not():
    _load()
    return _WHY[0] if _WHY else None


def library():
    lib = _load()
    if lib is None:
        raise RuntimeError("cbanalyze.dll: %s" % why_not())
    return lib


def _blob(img):
    """The image as the DLL reads it (little-endian): kind, bits, flat, has pointers; the spans
    (va, length, executable, writable, initialised length, name, bytes); the function starts with their
    limits; the import addresses; the pointers in the file's order (site, kind, has target, target)."""
    out = [struct.pack("<BBBB", ord(img.kind[0]), img.bits, 1 if img.flat else 0, 1 if img.pointers is not None else 0),
           struct.pack("<I", len(img.spans))]
    for va, data, exe, wr, name, init in img.spans:
        raw = (name or "").encode("utf-8", "surrogatepass")
        out.append(struct.pack("<qIBBqI", va, len(data), 1 if exe else 0, 1 if wr else 0, init, len(raw)) + raw)
        out.append(bytes(data))
    starts = sorted(img.names)
    out.append(struct.pack("<I", len(starts)))
    out.append(b"".join(struct.pack("<qq", s, img.limits.get(s, s)) for s in starts))
    out.append(struct.pack("<I", len(img.imports)))
    out.append(b"".join(struct.pack("<q", a) for a in img.imports))
    ptrs = img.pointers or {}
    out.append(struct.pack("<I", len(ptrs)))
    out.append(b"".join(struct.pack("<qBBq", site, _POINTER_KINDS.get(kind, 4), 0 if t is None else 1, t or 0)
                        for site, (kind, t) in ptrs.items()))
    out.append(struct.pack("<qI", img.data_segment or 0, len(img.roots)))
    out.append(b"".join(struct.pack("<q", r) for r in img.roots))
    return b"".join(out)


def _names(img):
    """The functions' names, then the imports' (count; address, length, UTF-8 each)."""
    out = []
    for table in (img.names, img.imports):
        out.append(struct.pack("<I", len(table)))
        for va, name in table.items():
            raw = name.encode("utf-8", "surrogatepass")
            out.append(struct.pack("<qI", va, len(raw)) + raw)
    return b"".join(out)


class Handle(object):
    """An image held by the DLL."""

    def __init__(self, img):
        lib = library()
        blob = _blob(img)
        err = ctypes.create_string_buffer(256)
        self._h = lib.cba_open(blob, len(blob), err, 256)
        if not self._h:
            raise RuntimeError("cbanalyze.dll: %s" % err.value.decode("ascii", "replace"))
        self._lib = lib

    def names(self, img):
        """Hand over the image's names (after discover names the starts it found)."""
        blob = _names(img)
        if self._lib.cba_names(self._h, blob, len(blob)):
            raise RuntimeError("cbanalyze.dll: the names blob is short")

    def ask(self, question, arg=0):
        out, size = ctypes.c_void_p(), ctypes.c_size_t()
        rc = self._lib.cba_ask(self._h, question.encode("ascii"), arg, ctypes.byref(out), ctypes.byref(size))
        if rc < 0:
            raise ValueError("cbanalyze.dll does not know %r" % question)
        if rc == 0:
            return None
        try:
            return json.loads(ctypes.string_at(out.value, size.value).decode("ascii"))
        finally:
            self._lib.cba_free(out)

    def __del__(self):
        h, self._h = getattr(self, "_h", None), None
        if h:
            self._lib.cba_close(h)
