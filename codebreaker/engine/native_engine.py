r"""native_engine.py -- the engine and the store in C++ (native/engine: cbengine.dll), through ctypes.

The shim (docs/NATIVE.md step 6): the program database (facts.py's Database), the engine's
core (engine.py's Run and Context: which facts are current, the inputs recorded, what is written,
kept or remade, the order and the worker threads), the function store (store/functions.py), the
knowledge base's database (knowledge/store.py, reads.py's digests) and the verdict vocabulary
(verify/verdicts.py) are C++; this module only hands Python's calls across, and calls back the
Python that remains:

    a step written in Python        run for the contexts the C++ run gives it (make)
    a step's version                engine.Step.version(): a digest of its source (inspect)
    a knowledge piece's hash now    reads.digest: its kind brought up to date by Python first
    what a value IS                 facts.encode / decode (bytes, text, JSON, a pickle)

There is no switch and no fallback any more: the Python engine and store it replaced are gone (the
milestone of 2026-11-04). Without the built DLL opening a database, a store or a knowledge base
raises, with why_not()'s reason. Build: native/engine/CMakeLists.txt.
"""
import contextvars
import ctypes
import itertools
import json
import os
import sqlite3
import struct
import sys
import threading
import weakref

from codebreaker.base import settings

_LIB = []                 # [the DLL, or None when it cannot be used here]
_WHY = []

_STEP = ctypes.CFUNCTYPE(ctypes.c_int64, ctypes.c_char_p)
_VERSION = ctypes.CFUNCTYPE(ctypes.c_int64, ctypes.c_char_p, ctypes.c_void_p)
_MAKE = ctypes.CFUNCTYPE(ctypes.c_int64, ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int32,
                         ctypes.POINTER(ctypes.c_void_p), ctypes.c_int64, ctypes.c_int64, ctypes.c_int32)
_DIGEST = ctypes.CFUNCTYPE(ctypes.c_int64, ctypes.c_char_p, ctypes.c_void_p)
_LOG = ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_char_p)

_P, _I64, _I = ctypes.c_char_p, ctypes.c_int64, ctypes.c_int
_VP, _OUT, _N = ctypes.c_void_p, ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_int64)
_SIGS = {
    "cbe_abi": ((), _I), "cbe_error": ((), _P), "cbe_error_py": ((), _I64), "cbe_error_code": ((), _I),
    "cbe_free": ((_VP,), None), "cbe_sink_put": ((_VP, _P, _I64), None),
    "cbe_set_callbacks": ((_STEP, _VERSION, _MAKE, _DIGEST, _LOG), None),
    "cbe_step_register": ((_P, _P, _P, _I64, _I, _I), _I), "cbe_step_unregister": ((_P,), _I),
    "cbe_db_open": ((_P, _OUT), _I), "cbe_db_release": ((_VP,), None), "cbe_db_close": ((_VP,), _I),
    "cbe_db_row": ((_VP, _P, _P, _OUT, _N), _I), "cbe_db_rows": ((_VP, _P, _OUT, _N), _I),
    "cbe_db_kinds": ((_VP, _OUT, _N), _I), "cbe_db_blob": ((_VP, _P, _OUT, _N), _I),
    "cbe_db_about": ((_VP, _P, _OUT, _N, ctypes.POINTER(_I)), _I), "cbe_db_set_about": ((_VP, _P, _P), _I),
    "cbe_db_write": ((_VP, _P, _P, _P, _P, _I64, _P, _P, _P, _I64, ctypes.c_double, _P), _I),
    "cbe_db_write_raw": ((_VP, _P, _P, _P, _P, _P, _I64, _P, _P, _P, ctypes.c_double), _I),
    "cbe_db_remake": ((_VP, _P, _P, _P, _P, _I64), _I), "cbe_db_forget": ((_VP, _P, _P, _N), _I),
    "cbe_db_drop_other_versions": ((_VP, _P, _P, _N), _I), "cbe_db_collect": ((_VP,), _I),
    "cbe_db_cleaned": ((_VP, _I, _P, ctypes.POINTER(_I)), _I),
    "cbe_digest": ((_P, _P, _I64, _P), _I), "cbe_sha": ((_P, _P), _I),
    "cbe_databases": ((_P, _OUT, _N), _I), "cbe_hold": ((_I64, _P, _OUT, _N), _I),
    "cbe_run_new": ((_VP, _I, _OUT), _I), "cbe_run_free": ((_VP,), None), "cbe_run_set_log": ((_VP, _I), _I),
    "cbe_run_current": ((_VP, _P, _P, ctypes.POINTER(_I), _P), _I),
    "cbe_run_many": ((_VP, _P, _P, _I64, _I64, _I, _I64), _I),
    "cbe_run_stale": ((_VP, _P, _P, _I64, _OUT, _N), _I), "cbe_run_counts": ((_VP, _OUT, _N), _I),
    "cbe_ctx_need": ((_VP, _P, _P), _I), "cbe_ctx_need_many": ((_VP, _P, _P, _I64, _I64, _I, _I64), _I),
    "cbe_ctx_peek": ((_VP, _P, _P, ctypes.POINTER(_I)), _I), "cbe_ctx_knowledge": ((_VP, _P, _I64), _I),
    "cbe_ctx_result": ((_VP, _P, _P, _I64), _I), "cbe_ctx_unkept": ((_VP,), _I),
    "cbe_ctx_inputs": ((_VP, _OUT, _N), _I), "cbe_ctx_key": ((_VP, _OUT, _N), _I),
    "cbe_store_open": ((_P, _P, _I, _OUT), _I), "cbe_store_call": ((_VP, _P, _P, _I64, _OUT, _N), _I),
    "cbe_store_free": ((_VP,), None),
    "cbe_kb_open": ((_P, _OUT), _I), "cbe_kb_call": ((_VP, _P, _P, _I64, _OUT, _N), _I),
    "cbe_kb_release": ((_VP,), None), "cbe_kb_close": ((_VP,), _I),
    "cbe_verdicts_call": ((_P, _P, _I64, _OUT, _N), _I),
}


def dll_path():
    """Where cbengine.dll is: the `engine_bin` setting (default native/engine/build/bin)."""
    return os.path.join(settings.get("engine_bin"), "cbengine.dll")


def _load():
    if _LIB:
        return _LIB[0]
    lib = None
    try:
        path = dll_path()
        if not os.path.isfile(path):
            raise OSError("not built: %s (native/engine/CMakeLists.txt)" % path)
        lib = settings.load_native(path)
        for name, (args, res) in _SIGS.items():
            fn = getattr(lib, name)
            fn.argtypes, fn.restype = args, res
        if lib.cbe_abi() != 1:
            raise OSError("cbengine.dll is another version: rebuild native/engine")
        lib.cbe_set_callbacks(_CB[0], _CB[1], _CB[2], _CB[3], _CB[4])
        _LIB.append(lib)
        from codebreaker.engine import engine
        for s in list(engine.STEPS.values()):                  # (steps declared before the DLL was loaded)
            declared(s)
    except (OSError, AttributeError) as e:
        _WHY.append(str(e))
        if not _LIB:
            _LIB.append(None)
    return _LIB[0]


def enabled():
    """The DLL loads in this process."""
    return _load() is not None


def _need():
    """The DLL, or the reason it cannot be had, raised."""
    lib = _load()
    if lib is None:
        raise RuntimeError("the engine and store are native (native/engine, cbengine.dll): %s -- run tools\build_native.cmd in this checkout" % why_not())
    return lib


def why_not():
    return _WHY[0] if _WHY else None


# ---- row buffers -----------------------------------------------------------------------------------
_Q = struct.Struct("<q")
_D = struct.Struct("<d")


def pack(values):
    out = bytearray()
    for v in values:
        if v is None:
            out.append(0)
        elif isinstance(v, bool) or isinstance(v, int):
            out.append(1)
            out += _Q.pack(int(v))
        elif isinstance(v, float):
            out.append(2)
            out += _D.pack(v)
        elif isinstance(v, str):
            b = v.encode("utf-8", "surrogatepass")
            out.append(3)
            out += _Q.pack(len(b))
            out += b
        elif isinstance(v, (bytes, bytearray, memoryview)):
            b = bytes(v)
            out.append(4)
            out += _Q.pack(len(b))
            out += b
        else:
            raise sqlite3.InterfaceError("Error binding parameter - type '%s' is not supported" % type(v).__name__)
    return bytes(out)


def unpack(data):
    out, i, n = [], 0, len(data)
    while i < n:
        t = data[i]
        i += 1
        if t == 0:
            out.append(None)
        elif t == 1:
            out.append(_Q.unpack_from(data, i)[0])
            i += 8
        elif t == 2:
            out.append(_D.unpack_from(data, i)[0])
            i += 8
        else:
            k = _Q.unpack_from(data, i)[0]
            i += 8
            b = data[i:i + k]
            i += k
            out.append(b.decode("utf-8", "surrogatepass") if t == 3 else bytes(b))
    return out


def _rows(data):
    """A store's or knowledge base's answer: [tuple] of its rows."""
    f = unpack(data)
    if not f:
        return []
    n = f[0]
    if not n:
        return []
    return [tuple(f[i:i + n]) for i in range(1, len(f), n)]


def _b(s):
    return s.encode("utf-8", "surrogatepass") if s is not None else None


# ---- errors ----------------------------------------------------------------------------------------
_ERRS = {}                # number -> an exception a callback raised, until it is raised again here
_NEXT = itertools.count(1)
_ERRS_LOCK = threading.Lock()


def _keep(e):
    with _ERRS_LOCK:
        n = next(_NEXT)
        _ERRS[n] = e
        while len(_ERRS) > 512:                                 # (ones the C++ caught and let go)
            _ERRS.pop(next(iter(_ERRS)))
    return (n << 1) | (1 if isinstance(e, LookupError) else 0)


def _fail(lib, rc):
    msg = (lib.cbe_error() or b"").decode("utf-8", "replace")
    if rc == 3:
        with _ERRS_LOCK:
            e = _ERRS.pop(lib.cbe_error_py(), None)
        if e is not None:
            raise e
        raise RuntimeError("an exception in a Python step, lost: " + msg)
    if rc == 1:
        raise LookupError(msg)
    if rc == 5:
        raise KeyError(msg)
    if rc == 6:
        raise ValueError(msg)
    if rc == 4:
        raise (sqlite3.IntegrityError if lib.cbe_error_code() == 19 else sqlite3.OperationalError)(msg)
    if rc == 7:
        code = lib.cbe_error_code()
        raise (FileNotFoundError if code in (2, 3) else OSError)(msg)
    raise RuntimeError(msg)


def _call(name, *args):
    lib = _need()
    rc = getattr(lib, name)(*args)
    if rc:
        _fail(lib, rc)


def _out(name, *args):
    """A call that hands back a buffer: -> its bytes."""
    lib = _need()
    p, n = ctypes.c_void_p(), ctypes.c_int64()
    rc = getattr(lib, name)(*(args + (ctypes.byref(p), ctypes.byref(n))))
    if rc:
        _fail(lib, rc)
    try:
        return ctypes.string_at(p.value, n.value) if n.value else b""
    finally:
        lib.cbe_free(p)


# ---- the callbacks ---------------------------------------------------------------------------------
_RUNS = weakref.WeakValueDictionary()      # the C++ run's address -> its Run here
_STORES = {}                               # the C++ store's address -> (store, its carry's log)
_HINTS = {}                                # number -> a hint handed to a step (need_many's)
_CONTEXTS = {}                             # number -> the asker's context, for work on the pool's threads
_IDS = itertools.count(1)


def _put(sink, text):
    b = text.encode("utf-8", "surrogatepass")
    _LIB[0].cbe_sink_put(sink, b, len(b))


def _on_step(kind):
    try:
        from codebreaker.engine import engine
        s = engine.STEPS.get(kind.decode("utf-8", "surrogatepass"))
        if s is None:
            s = engine._step_of(kind.decode("utf-8", "surrogatepass"))
        declared(s)
        return 0
    except BaseException as e:
        return _keep(e)


def _on_version(kind, sink):
    try:
        from codebreaker.engine import engine
        _put(sink, engine._step_of(kind.decode("utf-8", "surrogatepass")).version())
        return 0
    except BaseException as e:
        return _keep(e)


def _on_digest(read, sink):
    try:
        from codebreaker.knowledge import reads
        _put(sink, reads.digest(read.decode("utf-8", "surrogatepass")))
        return 0
    except BaseException as e:
        return _keep(e)


def _on_log(who, text):
    try:
        text = text.decode("utf-8", "replace")
        run = _RUNS.get(who)
        if run is not None:
            if run.log:
                run.log(text)
            return
        got = _STORES.get(who)
        if got is not None and got[1] is not None:
            got[1](text[1:])
        elif text[:1] == "1":
            sys.stderr.write(text[1:] + "\n")
    except BaseException:
        pass


def _on_make(run_p, kind, n, ctxs, hint, context, batch):
    try:
        run = _RUNS[run_p]
        kind = kind.decode("utf-8", "surrogatepass")
        ptrs = [ctxs[i] for i in range(n)]
        h = _HINTS.get(hint) if hint else None

        def body():
            from codebreaker.engine import engine
            from codebreaker.knowledge import reads
            s = engine._step_of(kind)
            cs = [Context(run, s, p, h) for p in ptrs]
            if batch:
                with reads.recording() as r:                    # (what the knowledge base was asked: inputs too)
                    values = s.fn({c.key: c for c in cs}, h)
                for c in cs:
                    c.knowledge(r)
                for c in cs:
                    if c.key in values:
                        c._give(values[c.key])
            else:
                c = cs[0]
                with reads.recording() as r:
                    v = s.fn(c, c.key)
                c.knowledge(r)
                c._give(v)

        if context:
            _CONTEXTS[context].copy().run(body)
        else:
            body()
        return 0
    except BaseException as e:
        return _keep(e)


_CB = (_STEP(_on_step), _VERSION(_on_version), _MAKE(_on_make), _DIGEST(_on_digest), _LOG(_on_log))


def declared(s):
    """A step declared (engine.step): the C++ registry knows its kind, what it reads, batch, always."""
    if not _LIB or _LIB[0] is None:
        return
    reads = pack(list(s.reads))
    _call("cbe_step_register", _b(s.kind), _b(s.platform or ""), reads, len(reads), int(bool(s.batch)),
          int(bool(s.always)))


# ---- the program database (facts.py) ----------------------------------------------------------------
class _Cleaned(object):
    """The steps whose other versions' facts are gone in this process (facts.Database.cleaned)."""

    def __init__(self, db):
        self.db = db

    def _op(self, op, step=""):
        got = ctypes.c_int()
        _call("cbe_db_cleaned", self.db._h, op, _b(step), ctypes.byref(got))
        return got.value

    def __contains__(self, step):
        return bool(self._op(0, step))

    def add(self, step):
        self._op(1, step)

    def clear(self):
        self._op(2)


class Database(object):
    """facts.Database on the C++ fact store (the same file, the same schema)."""

    def __init__(self, path):
        _need()
        self.path = path
        self.lock = threading.RLock()
        h = ctypes.c_void_p()
        _call("cbe_db_open", _b(path), ctypes.byref(h))
        self._h = h.value
        self._conn = None
        self.cleaned = _Cleaned(self)

    @property
    def conn(self):
        """A sqlite3 connection to the file, for reading by hand and the tests (the engine's own
        connection is the C++ one)."""
        if self._conn is None:
            self._conn = sqlite3.connect(self.path, timeout=30.0, check_same_thread=False)
        return self._conn

    @property
    def native(self):
        return True

    def _row(self, fields):
        from codebreaker.engine import facts
        return facts.Row(self, *fields)

    def row(self, kind, key=""):
        data = _out("cbe_db_row", self._h, _b(kind), _b(key))
        return self._row(unpack(data)) if data else None

    def rows(self, kind=None):
        f = unpack(_out("cbe_db_rows", self._h, _b(kind) if kind else None))
        return [self._row(f[i:i + 12]) for i in range(0, len(f), 12)]

    def kinds(self):
        f = unpack(_out("cbe_db_kinds", self._h))
        return [tuple(f[i:i + 4]) for i in range(0, len(f), 4)]

    def blob(self, h):
        return _out("cbe_db_blob", self._h, _b(h))

    def about(self, name, value=None):
        if value is None:
            p, n, found = ctypes.c_void_p(), ctypes.c_int64(), ctypes.c_int()
            lib = _LIB[0]
            rc = lib.cbe_db_about(self._h, _b(name), ctypes.byref(p), ctypes.byref(n), ctypes.byref(found))
            if rc:
                _fail(lib, rc)
            try:
                return ctypes.string_at(p.value, n.value).decode("utf-8", "surrogatepass") if found.value else None
            finally:
                lib.cbe_free(p)
        _call("cbe_db_set_about", self._h, _b(name), _b(value))
        return value

    def write(self, kind, key, value, step, version="", inputs=(), took=0.0, encoded=None):
        from codebreaker.engine import facts
        tag, data = encoded or facts.encode(value)
        ins = pack([str(x) for i in inputs for x in i])
        h = ctypes.create_string_buffer(64)
        _call("cbe_db_write", self._h, _b(kind), _b(key), _b(tag), data, len(data), _b(step), _b(version or ""),
              ins, len(ins), float(took), h)
        return h.value.decode("ascii")

    def write_raw(self, kind, key, hash, tag, data, step, version, inputs_json, took=0):
        _call("cbe_db_write_raw", self._h, _b(kind), _b(key), _b(hash), _b(tag), data, len(data), _b(step),
              _b(version), _b(inputs_json), float(took))

    def remake(self, kind, key, version, inputs):
        ins = pack([str(x) for i in inputs for x in i])
        _call("cbe_db_remake", self._h, _b(kind), _b(key), _b(version), ins, len(ins))

    def forget(self, kind, key=None):
        n = ctypes.c_int64()
        _call("cbe_db_forget", self._h, _b(kind), _b(key) if key is not None else None, ctypes.byref(n))
        return n.value

    def drop_other_versions(self, step, version):
        n = ctypes.c_int64()
        _call("cbe_db_drop_other_versions", self._h, _b(step), _b(version), ctypes.byref(n))
        return n.value

    def collect(self):
        _call("cbe_db_collect", self._h)

    def close(self):
        from codebreaker.engine import facts
        with facts._LOCK:
            facts._OPEN.pop(self.path, None)
        if self._conn is not None:
            try:
                self._conn.close()
            except sqlite3.Error:
                pass
            self._conn = None
        if self._h:
            _LIB[0].cbe_db_close(self._h)
            _LIB[0].cbe_db_release(self._h)
            self._h = None


def sha(path):
    out = ctypes.create_string_buffer(65)
    _call("cbe_sha", _b(path), out)
    return out.value.decode("ascii")


def databases(folder):
    f = unpack(_out("cbe_databases", _b(folder)))
    return [tuple(f[i:i + 3]) for i in range(0, len(f), 3)]


def hold(cap, folder):
    return unpack(_out("cbe_hold", int(cap), _b(folder)))


# ---- a run (engine.py's Run) and the context a step is given ----------------------------------------
def _jobs():
    from codebreaker.engine import engine
    return engine.jobs()


class Run(object):
    """engine.Run on the C++ core: each fact decided current or made again at most once in it."""

    def __init__(self, db, log=None):
        self.db, self.log = db, log
        self.values = {}                     # (kind, key) -> the value made, or read, in this run
        p = ctypes.c_void_p()
        _call("cbe_run_new", db._h, 1 if log else 0, ctypes.byref(p))
        self._p = p.value
        _RUNS[self._p] = self

    def __del__(self):
        try:
            if self._p and _LIB and _LIB[0] is not None:
                _LIB[0].cbe_run_free(self._p)
        except Exception:
            pass

    @property
    def counts(self):
        """{kind: [kept, made]}"""
        f = unpack(_out("cbe_run_counts", self._p))
        return {f[i]: [f[i + 1], f[i + 2]] for i in range(0, len(f), 3)}

    def hash_of(self, kind, key):
        kept, h = ctypes.c_int(), ctypes.create_string_buffer(128)
        _call("cbe_run_current", self._p, _b(kind), _b(key), ctypes.byref(kept), h)
        return h.value.decode("utf-8", "surrogatepass") if kept.value else None

    def value(self, kind, key):
        self.hash_of(kind, key)
        return self._value(kind, key)

    def _value(self, kind, key):
        k = (kind, key)
        try:
            return self.values[k]
        except KeyError:
            pass
        v = self.db.row(kind, key).value()
        self.values[k] = v
        return v

    def stale(self, kind, keys):
        b = pack(list(keys))
        return unpack(_out("cbe_run_stale", self._p, _b(kind), b, len(b)))

    def many(self, kind, keys, hint=None):
        keys = list(dict.fromkeys(keys))
        b = pack(keys)
        with _handed(hint) as (h, c):
            _call("cbe_run_many", self._p, _b(kind), b, len(b), h, _jobs(), c)
        return {k: self._value(kind, k) for k in keys}


class _handed(object):
    """A hint and the asker's context, numbered for the C++ run while a call is in it."""

    def __init__(self, hint):
        self.hint = hint

    def __enter__(self):
        self.h = next(_IDS) if self.hint is not None else 0
        if self.h:
            _HINTS[self.h] = self.hint
        self.c = next(_IDS)
        _CONTEXTS[self.c] = contextvars.copy_context()
        return self.h, self.c

    def __exit__(self, *exc):
        _HINTS.pop(self.h, None)
        _CONTEXTS.pop(self.c, None)
        return False


class Context(object):
    """engine.Context on the C++ core: what a step is given, its reads recorded as it asks."""

    def __init__(self, run, step, p, hint):
        self.run, self.step, self._p, self.hint = run, step, p, hint
        self.db = run.db
        self.key = _out("cbe_ctx_key", p).decode("utf-8", "surrogatepass")

    @property
    def inputs(self):
        f = unpack(_out("cbe_ctx_inputs", self._p))
        return [tuple(f[i:i + 3]) for i in range(0, len(f), 3)]

    def need(self, kind, key=""):
        _call("cbe_ctx_need", self._p, _b(kind), _b(key))
        return self.run._value(kind, key)

    def need_many(self, kind, keys, hint=None):
        keys = list(dict.fromkeys(keys))
        b = pack(keys)
        with _handed(hint) as (h, c):
            _call("cbe_ctx_need_many", self._p, _b(kind), b, len(b), h, _jobs(), c)
        return {k: self.run._value(kind, k) for k in keys}

    def peek(self, kind, key=""):
        found = ctypes.c_int()
        _call("cbe_ctx_peek", self._p, _b(kind), _b(key), ctypes.byref(found))
        return self.run._value(kind, key) if found.value else None

    def file(self, path):
        return self.need("file", os.path.abspath(path))

    def knowledge(self, keys):
        b = pack(list(keys))
        _call("cbe_ctx_knowledge", self._p, b, len(b))

    def previous(self):
        row = self.db.row(self.step.kind, self.key)
        try:
            return row.value() if row is not None else None
        except Exception:
            return None

    def work(self, *parts):
        from codebreaker.engine import facts
        return facts.work(self.db, *parts)

    def _give(self, v):
        from codebreaker.engine import engine, facts
        k = (self.step.kind, self.key)
        if isinstance(v, engine.Unkept):
            self.run.values[k] = v.value
            _call("cbe_ctx_unkept", self._p)
            return
        tag, data = facts.encode(v)
        self.run.values[k] = v
        _call("cbe_ctx_result", self._p, _b(tag), data, len(data))


# ---- the function store (store/functions.py) ---------------------------------------------------------
_ROW = ("addr", "engine", "rev", "sig", "name", "source", "problem", "verdict", "reason", "limits", "detail",
        "judged_sig")
_VROW = ("addr", "compiler", "source", "parent", "knobs", "distance", "total", "verdict", "behaves")


class FunctionStore(object):
    """functions.FunctionStore on the C++ store (the same files, the same schema)."""

    def __init__(self, path=None, readonly=False):
        from codebreaker.store import functions
        _need()
        self.own_path = bool(path or os.environ.get("CB_FUNCTIONS_DB"))
        self.path = path or functions.default_path()
        self.readonly = readonly
        self.folder = functions.databases_folder(self.path, self.own_path)
        self._db = None
        p = ctypes.c_void_p()
        _call("cbe_store_open", _b(self.path), _b(os.path.abspath(self.folder)), 1 if readonly else 0, ctypes.byref(p))
        self._p = p.value
        _STORES[self._p] = (self, None)
        if not readonly:
            self.carry()

    def _c(self, method, *args):
        b = pack(args)
        return _rows(_out("cbe_store_call", self._p, _b(method), b, len(b)))

    @property
    def db(self):
        """A sqlite3 connection to the store's index, for reading by hand and the tests."""
        if self._db is None:
            from codebreaker.store import functions
            self._db = functions._ro(self.path) if self.readonly else sqlite3.connect(self.path, timeout=60.0)
        return self._db

    def close(self):
        if self._db is not None:
            self._db.close()
            self._db = None
        if self._p:
            _STORES.pop(self._p, None)
            _LIB[0].cbe_store_free(self._p)
            self._p = None

    def database_path(self, module):
        return os.path.join(self.folder, module + ".sqlite")

    def module_db(self, module):
        """A sqlite3 connection to a module's tables (for reading by hand, and the tests)."""
        from codebreaker.store import functions
        p = self.database_path(module)
        if self.readonly:
            return functions._ro(p) if os.path.isfile(p) else None
        self._c("open_module", module)                          # (its tables made, as _m does)
        return sqlite3.connect(p, timeout=60.0)

    def summaries(self):
        out = {}
        for sha, n, ident, judged, last, lp, profiles, latest in self._c("summaries"):
            out[sha] = {"functions": n or 0, "identified": ident or 0, "judged": judged or 0, "last": last,
                        "last_profile": lp, "profiles": json.loads(profiles or "[]"),
                        "latest": json.loads(latest or "{}")}
        return out

    # ---- writing
    def add_module(self, sha, name, path="", kind=""):
        self._c("add_module", sha, name.lower(), path, kind)

    def add_profile(self, digest, settings):
        self._c("add_profile", digest, settings)

    def add_functions(self, module, addrs):
        self._c("add_functions", module, *list(addrs))

    def record_many(self, module, engine, rev, profile, rows):
        flat = []
        for r in rows:
            flat += [r["addr"], r.get("name"), r.get("source"), r.get("sig"), r["verdict"], r.get("reason"),
                     r.get("limits"), r.get("detail"), r.get("problem")]
        self._c("record_many", module, engine, rev, profile, *flat)

    def put_text(self, module, engine, kind, body):
        self._c("put_text", module, engine, kind, body)

    def get_text(self, module, engine, kind):
        return self._c("get_text", module, engine, kind)[0][0]

    def record(self, module, addr, engine, rev, sig, name, source, profile, verdict, **more):
        self.record_many(module, engine, rev, profile,
                         [dict(addr=addr, sig=sig, name=name, source=source, verdict=verdict, **more)])

    # ---- reading
    def candidates(self, module, profile, engine=None):
        return [dict(zip(_ROW, r)) for r in self._c("candidates", module, profile, engine)]

    def stored(self, module, addr):
        out, at = [], {}
        for idx, e, r, n, s, pr, has, p, v, why, lim, car in self._c("stored", module, addr):
            if idx not in at:
                at[idx] = {"engine": e, "rev": r, "name": n, "source": s, "problem": pr, "verdicts": []}
                out.append(at[idx])
            if has:
                at[idx]["verdicts"].append({"profile": p, "verdict": v, "reason": why, "limits": lim,
                                            "carried": bool(car)})
        return out

    def built(self, module, profile):
        return dict(self._c("built", module, profile))

    def set_built(self, module, profile, sigs):
        flat = []
        for addr, sig in sigs.items():
            flat += [addr, sig]
        self._c("set_built", module, profile, *flat)

    def judged(self, module, profile, engine):
        return {r["addr"]: r for r in (dict(zip(_ROW, x)) for x in self._c("judged", module, profile, engine))}

    def best(self, module, profile, prefer=()):
        out = {}
        for x in self._c("best", module, profile, *list(prefer)):
            r = dict(zip(_ROW, x[:12]))
            r["class"], r["weak"] = x[12], x[13].split(",") if x[13] else []
            out[r["addr"]] = r
        return out

    def latest(self, module):
        return dict(self._c("latest", module))

    def addresses(self, module):
        return [r[0] for r in self._c("addresses", module)]

    def tally(self, profile, module, prefer=()):
        return dict(self._c("tally", profile, module, *list(prefer)))

    def set_identified(self, module, found):
        flat = []
        for a, (n, lib) in found.items():
            flat += [a, n, lib]
        self._c("set_identified", module, *flat)

    def identified(self, module):
        return {a: (n, lib) for a, n, lib in self._c("identified", module)}

    def scores(self, profile, module, prefer=()):
        return dict(self._c("scores", profile, module, *list(prefer)))

    # ---- bodies and their names: the knowledge base's
    @property
    def knowledge(self):
        kb = self.__dict__.get("_knowledge")
        if kb is None:
            from codebreaker.knowledge import store as kstore
            kb = kstore.open(kstore.beside(self.path) if self.own_path else None)
            kb.carry_bodies(self.path, "function store " + os.path.normcase(os.path.abspath(self.path)))
            self._knowledge = kb
        return kb

    def set_bodies(self, module, rows):
        got = [r for r in self.module_rows() if r[0] == module]
        if got:
            self.knowledge.add_module(module, got[0][1], got[0][3])
        self.knowledge.set_bodies(module, rows)

    def bodies(self, module):
        return self.knowledge.bodies(module)

    def name_body(self, digest, name, source="named by hand"):
        self.knowledge.name_body(digest, name, source)

    def names_from_bodies(self, module):
        return self.knowledge.names_from_bodies(module)

    def shared_bodies(self):
        return self.knowledge.shared_bodies()

    def modules_with_body(self, digest):
        return self.knowledge.modules_with_body(digest)

    # ---- byte-match variants
    def record_variant(self, module, addr, compiler, source, distance, total, verdict, knobs="", parent=None,
                       behaves=""):
        return self._c("record_variant", module, addr, compiler, source, distance, total, verdict, knobs, parent,
                       behaves)[0][0]

    def variants(self, module, compiler=None):
        return [dict(zip(_VROW, r)) for r in self._c("variants", module, compiler)]

    def best_variant(self, module, compiler):
        return {r[0]: dict(zip(_VROW, r)) for r in self._c("best_variant", module, compiler)}

    def match_tally(self, module, compiler):
        return tuple(self._c("match_tally", module, compiler)[0])

    # ---- across modules: the index
    def modules(self, profile=None):
        return self._c("modules", profile)

    def module_rows(self, kind=None):
        return self._c("module_rows", kind)

    def module_path(self, name):
        got = self._c("module_path", name)
        return got[0][0] if got else None

    def stamps(self):
        return {sha: (s["functions"], s["judged"], s["last"]) for sha, s in self.summaries().items() if s["functions"]}

    def latest_profile(self, kind):
        got = self._c("latest_profile", kind)
        return got[0][0] if got else None

    def recent(self, limit=8):
        return self._c("recent", limit)

    def verdicts_since(self, t):
        return self._c("verdicts_since", t)[0][0]

    def overview(self):
        return [{"sha": sha, "name": name, "kind": kind, "path": p, "functions": n, "identified": ident,
                 "latest": json.loads(latest)} for sha, name, kind, p, n, ident, latest in self._c("overview")]

    # ---- the store from before the databases
    def _about(self, name, value=None):
        if value is None:
            got = self._c("about", name)
            return got[0][0] if got else None
        self._c("set_about", name, str(value))
        return value

    def carry(self, log=None):
        if self.readonly:
            return {}
        _STORES[self._p] = (self, log)
        try:
            return dict(self._c("carry"))
        finally:
            _STORES[self._p] = (self, None)


# ---- the knowledge base (knowledge/store.py) -------------------------------------------------------
class KnowledgeBase(object):
    """knowledge.store.KnowledgeBase on the C++ database: values decoded here, as Python's."""

    def __init__(self, path):
        _need()
        self.path = path
        self.lock = threading.RLock()
        p = ctypes.c_void_p()
        _call("cbe_kb_open", _b(path), ctypes.byref(p))
        self._p = p.value
        self._conn = None
        self._tables = {}                    # (platform, kind) -> {key: value}, read once
        self._ensured = {}                   # (platform, kind) -> stamp checked this process

    def _c(self, method, *args):
        b = pack(args)
        return _rows(_out("cbe_kb_call", self._p, _b(method), b, len(b)))

    @property
    def conn(self):
        if self._conn is None:
            self._conn = sqlite3.connect(self.path, timeout=60.0, check_same_thread=False)
        return self._conn

    def close(self):
        if self._conn is not None:
            self._conn.close()
            self._conn = None
        if self._p:
            _call("cbe_kb_close", self._p)
            _LIB[0].cbe_kb_release(self._p)
            self._p = None

    def get(self, platform, kind, key, default=None):
        from codebreaker.engine.facts import decode
        t = self._tables.get((platform, kind))
        if t is not None:
            return t.get(key, default)
        got = self._c("get", platform, kind, key)
        return decode(got[0][0], got[0][1]) if got else default

    def has(self, platform, kind, key):
        t = self._tables.get((platform, kind))
        if t is not None:
            return key in t
        return bool(self._c("has", platform, kind, key)[0][0])

    def put(self, platform, kind, key, value, source="", origin="learned"):
        self.put_many(platform, kind, {key: value}, source, origin)

    def put_many(self, platform, kind, values, source="", origin="learned"):
        from codebreaker.engine.facts import encode
        flat = []
        for key, value in values.items():
            tag, data = encode(value)
            flat += [key, tag, data]
        self._c("put_many", platform, kind, source, origin, *flat)
        t = self._tables.get((platform, kind))
        if t is not None:
            t.update(values)

    def forget(self, platform, kind, key):
        self._c("forget", platform, kind, key)
        self._tables.get((platform, kind), {}).pop(key, None)

    def about(self, platform, kind, key):
        got = self._c("about", platform, kind, key)
        return got[0] if got else None

    def table(self, platform, kind):
        from codebreaker.engine.facts import decode
        t = self._tables.get((platform, kind))
        if t is None:
            t = self._tables[(platform, kind)] = {k: decode(tag, v) for k, tag, v in self._c("table", platform, kind)}
        return t

    def hashes(self, platform, kind):
        return dict(self._c("hashes", platform, kind))

    def hash_of(self, platform, kind, key):
        got = self._c("hash_of", platform, kind, key)
        return got[0][0] if got else None

    def kind_digest(self, platform, kind):
        got = self._c("kind_digest", platform, kind)
        return got[0][0] if got else None

    def digest(self, read):
        """reads.digest's answer from the base (the kind brought up to date by the caller)."""
        return self._c("digest", read)[0][0]

    def stamp(self, platform, kind):
        got = self._c("stamp", platform, kind)
        return got[0][0] if got else None

    def ensure(self, platform, kind, stamp, make, source, origin="authored"):
        from codebreaker.engine.facts import encode
        if self._ensured.get((platform, kind)) == stamp:
            return self.table(platform, kind)
        if self.stamp(platform, kind) != stamp:
            values = make()
            flat = []
            for key, value in values.items():
                tag, data = encode(value)
                flat += [key, tag, data]
            self._c("replace_kind", platform, kind, stamp, source, origin, *flat)
            self._tables.pop((platform, kind), None)
        self._ensured[(platform, kind)] = stamp
        return self.table(platform, kind)

    def kinds(self, platform=None):
        return [[p, k, n, size, json.loads(origins), src]
                for p, k, n, size, origins, src in self._c("kinds", platform)]

    def carried(self, what, mark=None):
        got = self._c("carried", what, mark) if mark is not None else self._c("carried", what)
        return got[0][0] if got else None

    def _read(self, kind, key):
        from codebreaker.knowledge import reads, store
        if reads.active() and os.path.normcase(self.path) == os.path.normcase(os.path.abspath(store.default_path())):
            reads.record("bodies", kind, key)

    def add_module(self, sha, name, platform=""):
        from codebreaker import knowledge
        self._c("add_module", sha, name, knowledge.platform(platform or ""))

    def set_bodies(self, module, rows):
        flat = []
        for a, (h, n, s) in rows.items():
            flat += [a, h, n, s]
        self._c("set_bodies", module, *flat)

    def bodies(self, module):
        return {a: (h, n, s) for a, h, n, s in self._c("bodies", module)}

    def name_body(self, body_id, name, source="named by hand"):
        self._c("name_body", body_id, name, source)

    def name(self, body_id, record=True):
        if record:
            self._read("name", body_id)
        got = self._c("name", body_id)
        return tuple(got[0]) if got else None

    def names_from_bodies(self, module, record=True):
        if record:
            self._read("names", module)
        return {a: (n, s) for a, n, s in self._c("names_from_bodies", module)}

    def shared_bodies(self):
        return tuple(self._c("shared_bodies")[0])

    def modules_with_body(self, digest):
        return self._c("modules_with_body", digest)[0][0]

    def carry_bodies(self, store_db, label):
        if not isinstance(store_db, str):                       # (a sqlite3 connection: its file)
            try:
                store_db = [r[2] for r in store_db.execute("PRAGMA database_list") if r[1] == "main"][0]
            except (sqlite3.Error, IndexError):
                return
        self._c("carry_bodies", store_db, label)


# ---- the verdict vocabulary -----------------------------------------------------------------------
def verdicts_call(method, *args):
    b = pack(args)
    return unpack(_out("cbe_verdicts_call", _b(method), b, len(b)))
