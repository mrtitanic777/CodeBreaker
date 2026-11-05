"""native_dosexec.py -- dos.run() answered by the C++ executive (native/dosexec: cbdos.dll), through ctypes.

The executive (PSP, MCB arena, program load, INT 20h/21h over the MemFS or the disk, EXEC, the
console and clock) and its DPMI host run in C++ on the same unicorn.dll Python loaded; Python
only hands the run's arguments and a MemFS's files over and turns the result back into the same
dos.Result (and the MemFS changed the same way).  A run with Python hooks of its own (instrument,
instrument_when, driver), a DPMI host object given by the caller, or a file system that is not a
dos.MemFS stays on the Python executive.

The switch: CB_NATIVE_DOSEXEC (default below; 0 = the Python executive, as also without the DLL).
Build: native/dosexec/CMakeLists.txt (cmake + ninja in a vcvars64 shell).
"""
import collections
import ctypes
import os

from codebreaker.base import settings

DEFAULT = "1"             # CB_NATIVE_DOSEXEC when it is not set (identical on the gate: docs/NATIVE.md)
_LIB = []                 # [the DLL, or None when it cannot be used here]
_WHY = []
_RAISE = {1: RuntimeError, 4: AssertionError, 5: ValueError, 8: RuntimeError}


class _Args(ctypes.Structure):
    _fields_ = [("exe", ctypes.c_wchar_p), ("workdir", ctypes.c_wchar_p),
                ("cmdline", ctypes.c_char_p), ("ncmdline", ctypes.c_int64),
                ("stdin_data", ctypes.c_char_p), ("nstdin", ctypes.c_int64),
                ("env", ctypes.c_char_p), ("nenv", ctypes.c_int64),
                ("max_blocks", ctypes.c_int64), ("trace_calls", ctypes.c_int32), ("psp", ctypes.c_int32),
                ("top", ctypes.c_int32), ("fast", ctypes.c_int32), ("dpmi", ctypes.c_int32),
                ("debug", ctypes.c_int32), ("drive", ctypes.c_int32),
                ("fast_limit", ctypes.c_double), ("dpmi_limit", ctypes.c_double), ("fs", ctypes.c_void_p)]


class _Result(ctypes.Structure):
    _fields_ = [("status", ctypes.c_int32), ("err_no", ctypes.c_int32), ("message", ctypes.c_char_p),
                ("filename", ctypes.c_wchar_p), ("has_exit", ctypes.c_int32), ("exit", ctypes.c_int32),
                ("console", ctypes.c_void_p), ("nconsole", ctypes.c_int64),
                ("blocks", ctypes.POINTER(ctypes.c_uint64)), ("nblocks", ctypes.c_int64),
                ("steps", ctypes.c_int64), ("load_seg", ctypes.c_int32),
                ("calls", ctypes.POINTER(ctypes.c_uint64)), ("ncalls", ctypes.c_int64)]


def dll_path():
    """Where cbdos.dll is: the `dosexec_bin` setting (default native/dosexec/build/bin)."""
    return os.path.join(settings.get("dosexec_bin"), "cbdos.dll")


def _load():
    if _LIB:
        return _LIB[0]
    lib = None
    try:
        path = dll_path()
        if not os.path.isfile(path):
            raise OSError("not built: %s (native/dosexec/CMakeLists.txt)" % path)
        from unicorn.unicorn_py3.unicorn import uclib                      # the DLL Python loaded
        lib = settings.load_native(path)
        if lib.cbd_version() != 1:
            raise OSError("cbdos.dll is another version: rebuild native/dosexec")
        lib.cbd_init.argtypes = (ctypes.c_wchar_p, ctypes.c_char_p, ctypes.c_int32)
        err = ctypes.create_string_buffer(256)
        if lib.cbd_init(os.path.abspath(uclib._name), err, 256):
            raise OSError(err.value.decode("ascii", "replace"))
        lib.cbd_fs_new.restype = ctypes.c_void_p
        lib.cbd_fs_free.argtypes = (ctypes.c_void_p,)
        lib.cbd_fs_hide.argtypes = lib.cbd_fs_gone.argtypes = (ctypes.c_void_p, ctypes.c_wchar_p)
        lib.cbd_fs_put.argtypes = (ctypes.c_void_p, ctypes.c_wchar_p, ctypes.c_char_p, ctypes.c_int64)
        lib.cbd_fs_changed.argtypes = lib.cbd_fs_ngone.argtypes = (ctypes.c_void_p,)
        lib.cbd_fs_changed.restype = lib.cbd_fs_ngone.restype = ctypes.c_int64
        lib.cbd_fs_entry.argtypes = (ctypes.c_void_p, ctypes.c_int64, ctypes.POINTER(ctypes.c_wchar_p),
                                     ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_int64))
        lib.cbd_fs_gone_at.argtypes = (ctypes.c_void_p, ctypes.c_int64)
        lib.cbd_fs_gone_at.restype = ctypes.c_wchar_p
        lib.cbd_run.argtypes = (ctypes.POINTER(_Args),)
        lib.cbd_run.restype = ctypes.POINTER(_Result)
        lib.cbd_free.argtypes = (ctypes.POINTER(_Result),)
    except Exception as e:                       # (no DLL, another version, no unicorn: the Python executive)
        _WHY.append(str(e))
        lib = None
    _LIB.append(lib)
    return lib


def enabled():
    if str(os.environ.get("CB_NATIVE_DOSEXEC", DEFAULT)).lower() in ("0", "off", "no", "false"):
        return False
    return _load() is not None


def _raise(r):
    msg = (r.message or b"").decode("utf-8", "replace")
    if r.status == 2:
        from unicorn import UcError
        raise UcError(r.err_no)
    if r.status == 3:
        if r.err_no == 0:
            raise FileNotFoundError(msg)
        if r.err_no < 0:
            raise OSError(None, msg, r.filename or None, -r.err_no)
        if r.filename:
            raise OSError(r.err_no, msg, r.filename)
        raise OSError(r.err_no, msg)
    if r.status == 6:
        import struct
        raise struct.error(msg)
    if r.status == 7:
        import io
        raise io.UnsupportedOperation(msg)
    raise _RAISE.get(r.status, RuntimeError)(msg)


def run(exe, workdir, cmdline, stdin, max_blocks, trace_calls, env, psp, top, fast, fs, dpmi, fast_limit):
    """dos.run's arguments -> dos.Result, or the exception dos.run would have raised."""
    from codebreaker.exact.emu import dos
    lib = _load()
    tail = cmdline.encode("latin1")
    stdin, env = bytes(stdin), bytes(env)
    h = None
    if fs is not None:
        h = lib.cbd_fs_new()
        for p in fs.hide:
            lib.cbd_fs_hide(h, p)
        for k, b in fs.files.items():
            b = bytes(b)
            lib.cbd_fs_put(h, k, b, len(b))
        for k in fs.gone:
            lib.cbd_fs_gone(h, k)
    a = _Args(str(exe), str(workdir), tail, len(tail), stdin, len(stdin), env, len(env), int(max_blocks),
              1 if trace_calls else 0, int(psp or 0), int(top or 0), 1 if fast else 0, 1 if dpmi else 0,
              1 if os.environ.get("DOSRUN_DEBUG") else 0, int(os.environ.get("DOSRUN_DRIVE", "2")),
              float(fast_limit), float(os.environ.get("DOSRUN_DPMI_LIMIT", "300")), h)
    rp = lib.cbd_run(ctypes.byref(a))
    try:
        if h is not None:                        # what the run changed in the MemFS
            key, data, n = ctypes.c_wchar_p(), ctypes.c_void_p(), ctypes.c_int64()
            for i in range(lib.cbd_fs_changed(h)):
                if lib.cbd_fs_entry(h, i, ctypes.byref(key), ctypes.byref(data), ctypes.byref(n)):
                    fs.files[key.value] = bytearray(ctypes.string_at(data.value, n.value) if n.value else b"")
                else:
                    fs.files.pop(key.value, None)
            gone = {lib.cbd_fs_gone_at(h, i) for i in range(lib.cbd_fs_ngone(h))}
            fs.gone.clear()
            fs.gone.update(gone)
        r = rp.contents
        if r.status:
            _raise(r)
        console = ctypes.string_at(r.console, r.nconsole) if r.nconsole else b""
        blocks = set(r.blocks[:r.nblocks]) if r.nblocks else set()
        calls = collections.Counter()
        for i in range(r.ncalls):
            calls[r.calls[2 * i]] = r.calls[2 * i + 1]
        return dos.Result(r.exit if r.has_exit else None, console, blocks, r.load_seg, None, r.steps, calls)
    finally:
        lib.cbd_free(rp)
        if h is not None:
            lib.cbd_fs_free(h)
