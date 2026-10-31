"""native_l2.py -- the L2 core and the C back end in C++ (native/l2: cbl2.dll), through ctypes.

A thin shim, kept only until each caller's own turn to move (docs/NATIVE.md step 5): the callers of
codebreaker/translate/backends/c.py and of l2.py's round trip keep calling them, and with the switch
on the work is done by the DLL -- the same text, the same facts, the same exceptions (Decline,
KeyError, ValueError where the Python raised them).

    native_l2.enabled()                      CB_NATIVE_L2 is not 0 and the DLL loads
    native_l2.call("function", doc, moved, far, facts)
    native_l2.install(namespace)             (backends/c.py: its entry points dispatched by the switch)

Values cross as JSON; what JSON has no form for goes tagged (a tuple {"$t": [...]}, a set {"$s": [...]},
a dict of other keys {"$d": [[k, v], ...]}, a float that is not finite {"$f": "nan"}).

The switch: CB_NATIVE_L2 (default 1 since the gate, docs/NATIVE.md; 0 = the Python). Without the DLL
the Python runs (why_not() says why). Build: native/l2/CMakeLists.txt (cmake + ninja, vcvars64).
"""
import ctypes
import functools
import json
import math
import os

from codebreaker.base import settings

DEFAULT = "0"             # CB_NATIVE_L2 when it is not set
_LIB = []
_WHY = []


def dll_path():
    """Where cbl2.dll is: the `l2_bin` setting (default native/l2/build/bin)."""
    return os.path.join(settings.get("l2_bin") or os.path.join(settings.ROOT, "native", "l2", "build", "bin"), "cbl2.dll")


def _load():
    if _LIB:
        return _LIB[0]
    lib = None
    try:
        path = dll_path()
        if not os.path.isfile(path):
            raise OSError("not built: %s (native/l2/CMakeLists.txt)" % path)
        lib = ctypes.CDLL(path)
        lib.l2_version.restype = ctypes.c_int
        if lib.l2_version() != 1:
            raise OSError("cbl2.dll is another version: rebuild native/l2")
        lib.l2_call.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_size_t, ctypes.POINTER(ctypes.c_void_p),
                                ctypes.POINTER(ctypes.c_size_t))
        lib.l2_call.restype = ctypes.c_int
        lib.l2_free.argtypes = (ctypes.c_void_p,)
        lib.l2_free.restype = None
    except Exception as e:                                   # (no DLL: the Python runs)
        _WHY.append("%s: %s" % (type(e).__name__, e))
        lib = None
    _LIB.append(lib)
    return lib


def enabled():
    """The native writer for this process: CB_NATIVE_L2 is not 0 and the DLL loads."""
    if os.environ.get("CB_NATIVE_L2", DEFAULT) in ("", "0"):
        return False
    return _load() is not None


def why_not():
    _load()
    return _WHY[0] if _WHY else None


# ---------------------------------------------------------------------------------------------- values
def _enc(v):
    if v is None or isinstance(v, (bool, str, int)):
        return v
    if isinstance(v, float):
        return v if math.isfinite(v) else {"$f": repr(v)}
    if isinstance(v, list):
        return [_enc(x) for x in v]
    if isinstance(v, tuple):
        return {"$t": [_enc(x) for x in v]}
    if isinstance(v, (set, frozenset)):
        return {"$s": [_enc(x) for x in v]}
    if isinstance(v, dict):
        if all(type(k) is str and not k.startswith("$") for k in v):
            return {k: _enc(x) for k, x in v.items()}
        return {"$d": [[_enc(k), _enc(x)] for k, x in v.items()]}
    raise TypeError("no L2 shim form for %r" % type(v).__name__)


def _dec(d):
    if len(d) == 1:
        k = next(iter(d))
        if k == "$t":
            return tuple(d[k])
        if k == "$s":
            return set(d[k])
        if k == "$d":
            return {(kk if not isinstance(kk, list) else tuple(kk)): vv for kk, vv in d[k]}
        if k == "$f":
            return float(d[k])
    return d


class NativeError(RuntimeError):
    """An error inside the DLL that the Python has no exception for."""


def _raise(err):
    t, msg = err.get("type"), err.get("message", "")
    if t == "Decline":
        from codebreaker.translate.backends import c as cback
        raise cback.Decline(msg)
    exc = {"KeyError": KeyError, "ValueError": ValueError, "TypeError": TypeError, "IndexError": IndexError,
           "AttributeError": AttributeError}.get(t)
    if exc is KeyError:
        raise KeyError(msg)
    if exc is not None:
        raise exc(msg)
    raise NativeError("%s: %s" % (t, msg))


def call_raw(fn, text):
    """One entry point with its arguments already as the tagged JSON array -> (status, the result's text)."""
    lib = _load()
    out, n = ctypes.c_void_p(), ctypes.c_size_t()
    data = text.encode("utf-8", "surrogatepass")
    status = lib.l2_call(fn.encode("ascii"), data, len(data), ctypes.byref(out), ctypes.byref(n))
    try:
        got = ctypes.string_at(out, n.value).decode("utf-8", "surrogatepass")
    finally:
        lib.l2_free(out)
    return status, got


def call(fn, *args, plain=None, facts=None):
    """An entry point of the DLL with `args` -> its result. `plain`: the index of an argument that is
    plain JSON already (an L2 document: written by json.dumps, which is quicker). `facts`: a dict the
    call changes in place (backends/c.function's): the keys it wrote are put back, also when it raises."""
    parts = []
    for k, a in enumerate(args):
        parts.append(json.dumps(a, separators=(",", ":")) if k == plain else json.dumps(_enc(a), separators=(",", ":")))
    status, got = call_raw(fn, "[" + ",".join(parts) + "]")
    res = json.loads(got, object_hook=_dec)
    if status:
        if facts is not None:
            facts.update(res.get("facts") or {})
        _raise(res)
    return res


# ---------------------------------------------------------------------------------------------- backends/c.py
def _function(doc, moved=(), far=None, facts=None):
    text, out = call("function", doc, list(moved), sorted(far) if isinstance(far, (set, frozenset)) else far, facts,
                     plain=0, facts=facts)
    if facts is not None:
        facts.update(out)
    return text


def _type_text(t, name='', far=False):
    return call("type_text", t, name, far)


def _flags(doc, facts):
    return call("flags", doc, facts, plain=0)


def _module(doc, moved=None):
    from codebreaker.translate.backends import c as cback
    text, asm = call("module", doc, list(cback.MOVED if moved is None else moved), plain=0)
    return text, asm


def _segment_plan(functions, near, where, sites, code, data_segment, other_data, first_selector):
    # (`code` is read only as the set of its keys -- a dict of segments' bytes, or a set)
    return call("segment_plan", functions, near, where, sites, sorted(code), data_segment, other_data, first_selector)


def _plain(name):
    def run(*args):
        return call(name, *args)
    return run


def _doc1(name):
    def run(doc, *rest):
        return call(name, doc, *rest, plain=0)
    return run


# backends/c.py's entry points the native writer has, with how each is called
_ENTRIES = {
    "function": _function, "flags": _flags, "type_text": _type_text, "module": _module,
    "signature": _doc1("signature"), "module_names32": _doc1("module_names32"), "type_names": _doc1("type_names"),
    "parameter_names": _doc1("parameter_names"), "struct_accesses": _doc1("struct_accesses"),
    "expr": _doc1("expr"), "condition": _doc1("condition"), "fragment": _doc1("fragment"),
    "takes32": _plain("takes32"), "prototypes": _plain("prototypes"), "segment_plan": _segment_plan,
    "caller_frame_plan": _plain("caller_frame_plan"), "seg_layout": _plain("seg_layout"),
    "arguments_at": _plain("arguments_at"), "words": _plain("words"), "parameters": _plain("parameters"),
    "argument_words": _plain("argument_words"),
}


def install(namespace):
    """Each of backends/c.py's entry points dispatched by the switch: the native writer where it is on,
    the Python (kept as namespace["_py_<name>"]) where it is off."""
    for name, native in _ENTRIES.items():
        py = namespace[name]
        namespace["_py_" + name] = py

        def dispatch(*args, _py=py, _native=native, **kw):
            if enabled():
                return _native(*args, **kw)
            return _py(*args, **kw)
        functools.update_wrapper(dispatch, py)
        namespace[name] = dispatch


# ---------------------------------------------------------------------------------------------- the L2 core
_SCHEMA = []


def roundtrip_text(doc):
    """json.dumps(l2.to_json(l2.from_json(doc))), from the native core."""
    return call("roundtrip_text", doc, plain=0)


def validate(doc):
    """(valid, why) of `doc` against l2.schema.json, as l2.validate decides."""
    if not _SCHEMA:
        from codebreaker.translate import l2
        with open(l2.SCHEMA, encoding="utf-8") as f:
            call_raw("set_schema", "[" + f.read() + "]")
        _SCHEMA.append(True)
    return tuple(call("validate", doc, plain=0))
