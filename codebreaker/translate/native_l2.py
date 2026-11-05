"""native_l2.py -- the L2 core and the C back end in C++ (native/l2: cbl2.dll), through ctypes.

A thin shim, kept only until each caller's own turn to move (docs/NATIVE.md step 5): the callers of
codebreaker/translate/backends/c.py keep calling it, and the work is done by the DLL -- the same
text, the same facts, the same exceptions (Decline, KeyError, ValueError where the Python raised
them). The Python writer is gone (2026-11-04): c.py's entry points are _ENTRIES below.

    native_l2.enabled()                      the DLL loads
    native_l2.call("function", doc, moved, far, facts)

Values cross as JSON; what JSON has no form for goes tagged (a tuple {"$t": [...]}, a set {"$s": [...]},
a dict of other keys {"$d": [[k, v], ...]}, a float that is not finite {"$f": "nan"}).

There is no switch and no fallback: without the DLL a call raises, and why_not() says why. Build:
native/l2/CMakeLists.txt (cmake + ninja, vcvars64).
"""
import copy
import ctypes
import json
import math
import os

from codebreaker.base import settings

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
    except Exception as e:                                   # (no DLL: a call says why)
        _WHY.append("%s: %s" % (type(e).__name__, e))
        lib = None
    _LIB.append(lib)
    return lib


def enabled():
    """The native writer loads in this process."""
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


_BIG = []                 # [(a large container, its JSON)]: the same value handed over again is not encoded again
_SEP = (",", ":")


def _text(v):
    """`v` as the tagged JSON text. A large set, list or dict equal to one handed over lately (the module's
    sets each function is written with: its functions, the globals called through ...) takes the text it
    had; a set or list of strings alone is written by json itself."""
    t = type(v)
    if t in (set, frozenset, list, dict) and len(v) >= 32:
        n = len(v)
        for obj, number in _BIG:                         # (an equal one first: compared at C's speed)
            if type(obj) is t and len(obj) == n and obj == v:
                return '{"$big":%d}' % number            # (the DLL keeps it: handed over by its number)
    if t in (set, frozenset, list, dict) and len(v) >= 32 and (
            all(type(k) is str and type(x) in (str, int) for k, x in v.items()) if t is dict else
            all(type(x) in (str, int) for x in v)):
        # (of strings and ints alone -- no bool, which == would take for an int -- kept as a copy,
        # so a change to the container later is no stale hit)
        if t is dict:
            text = json.dumps(v, separators=_SEP) if not any(k.startswith("$") for k in v) else json.dumps(_enc(v), separators=_SEP)
        else:
            text = json.dumps(v if t is list else {"$s": list(v)}, separators=_SEP)
        _NEXT[0] += 1
        _BIG.append((t(v), _NEXT[0]))
        del _BIG[:-32]                                   # (the DLL keeps the latest 64)
        return '{"$keep":%d,"$v":%s}' % (_NEXT[0], text)
    if t is dict and all(type(k) is str and not k.startswith("$") for k in v):
        return "{" + ",".join(json.dumps(k) + ":" + _text(x) for k, x in v.items()) + "}"
    return json.dumps(_enc(v), separators=_SEP)


_HELD = {}                # {id: (an L2 document, its number in the DLL)}, oldest first
_KEEP = 2048              # (a module's functions: each document goes to several entry points, loop after loop)
_NEXT = [0]


def _held(doc):
    """An L2 document as an argument: the first time whole, numbered (the DLL keeps the latest few),
    then by its number -- one function's document goes to several entry points in turn. (Documents
    handed to the writer are read, never changed in place: backends/c.py's own rule.)
    -> (the argument's text, a way to make the whole text again or None)"""
    got = _HELD.get(id(doc))
    if got is not None and got[0] is doc:
        return '{"$doc":%d}' % got[1], lambda: _put(doc)
    return _put(doc), None


_TEXTS = {}               # {id: (an L2 document, the JSON text it was read from)}


def known_text(doc, text):
    """The decompiler's note an L2 document was read from (decompiler.split_notes): handed to the DLL as
    it is, the document is not written out again. (Kept only while the native writer loads.)"""
    if enabled():
        _TEXTS[id(doc)] = (doc, text)
        while len(_TEXTS) > 2 * _KEEP:
            _TEXTS.pop(next(iter(_TEXTS)))


def _put(doc):
    _NEXT[0] += 1
    _HELD.pop(id(doc), None)
    _HELD[id(doc)] = (doc, _NEXT[0])                     # (holding the document: its id is not reused)
    while len(_HELD) > _KEEP:
        _HELD.pop(next(iter(_HELD)))
    known = _TEXTS.pop(id(doc), None)
    text = known[1] if known is not None and known[0] is doc else json.dumps(doc, separators=_SEP)
    return '{"$put":%d,"$v":%s}' % (_NEXT[0], text)


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
    if t == "Cpp17Decline":
        from codebreaker.translate.backends import cpp17
        raise cpp17.Decline(msg)
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
    if lib is None:
        raise RuntimeError("the C back end is native (native/l2, cbl2.dll): %s -- run tools\build_native.cmd in this checkout" % why_not())
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
    def arguments():
        parts, held = [], None
        for k, a in enumerate(args):
            if k == plain and isinstance(a, dict):
                held = _held(a)
                parts.append(held[0])
            else:
                parts.append(json.dumps(a, separators=_SEP) if k == plain else _text(a))
        return parts, held
    parts, held = arguments()
    status, got = call_raw(fn, "[" + ",".join(parts) + "]")
    if status and "no longer kept" in got:              # (the DLL let a large value go: all of them again)
        del _BIG[:]
        parts, held = arguments()
        status, got = call_raw(fn, "[" + ",".join(parts) + "]")
    if status and held is not None and held[1] is not None and "no longer held" in got:
        parts[plain] = held[1]()                         # (the DLL let it go: handed over whole again)
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


_DESCRIBED = {}           # {id: (an L2 document, what describe said of it)} -- the same documents as _HELD
_TAKES = {}               # {a heading: takes32 of it}, from describe


def _described(name):
    """module_names32, type_names, parameter_names, signature of a function's document: all four
    asked of the DLL once (`describe`), each handed out from that -- a module asks them of every
    function, loop after loop, and each crossing cost more than the answer."""
    def run(doc):
        got = _DESCRIBED.get(id(doc))
        if got is None or got[0] is not doc:
            said = call("describe", doc, plain=0)
            got = _DESCRIBED[id(doc)] = (doc, said)
            while len(_DESCRIBED) > _KEEP:
                _DESCRIBED.pop(next(iter(_DESCRIBED)))
            if isinstance(said.get("signature"), str) and "takes32" in said:
                _TAKES[said["signature"]] = said["takes32"]
                if len(_TAKES) > 4 * _KEEP:
                    _TAKES.clear()
        v = got[1][name]
        if isinstance(v, dict) and "$err" in v:
            _raise({"type": v["$err"][0], "message": v["$err"][1]})
        return copy.deepcopy(v) if isinstance(v, (dict, list, set)) else v
    return run


def _takes32(signature):
    if signature in _TAKES:
        return _TAKES[signature]
    return call("takes32", signature)


# backends/c.py's entry points the native writer has, with how each is called
_ENTRIES = {
    "function": _function, "flags": _flags, "type_text": _type_text, "module": _module,
    "signature": _described("signature"), "module_names32": _described("module_names32"),
    "type_names": _described("type_names"), "parameter_names": _described("parameter_names"),
    "struct_accesses": _doc1("struct_accesses"),
    "expr": _doc1("expr"), "condition": _doc1("condition"), "fragment": _doc1("fragment"),
    "takes32": _takes32, "prototypes": _plain("prototypes"), "segment_plan": _segment_plan,
    "caller_frame_plan": _plain("caller_frame_plan"), "seg_layout": _plain("seg_layout"),
    "arguments_at": _plain("arguments_at"), "words": _plain("words"), "parameters": _plain("parameters"),
    "argument_words": _plain("argument_words"),
}


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
