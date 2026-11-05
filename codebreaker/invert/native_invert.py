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
_RAISE = {1: IndexError, 2: ValueError, 3: RuntimeError, 4: KeyError, 5: AssertionError, 6: TypeError}


def dll_path():
    """Where cbinvert.dll is: the `invert_bin` setting (default native/invert/build/bin)."""
    return os.path.join(settings.get("invert_bin"), "cbinvert.dll")


def _load():
    if _LIB:
        return _LIB[0]
    path = dll_path()
    if not os.path.isfile(path):
        raise OSError("cbinvert.dll is not built: %s (native/invert/CMakeLists.txt)" % path)
    lib = settings.load_native(path)
    if lib.cbinv_version() != 3:
        raise OSError("cbinvert.dll is another version: rebuild native/invert")
    import capstone                          # the models decode with the very capstone.dll Python's package carries
    cs = os.path.join(os.path.dirname(capstone.__file__), "lib", "capstone.dll")
    lib.cbinv_init.argtypes = (ctypes.c_wchar_p, ctypes.POINTER(ctypes.c_void_p))
    for name in ("cbinv_msc6", "cbinv_msc6_sigs", "cbinv_wf", "cbinv_mask"):
        getattr(lib, name).argtypes = (ctypes.c_char_p, ctypes.POINTER(ctypes.c_void_p))
    lib.cbinv_small.argtypes = (ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.c_void_p))
    lib.cbinv_msvc2.argtypes = (ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.c_void_p))
    lib.cbinv_msc6reg.argtypes = (ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.c_void_p))
    lib.cbinv_thunk.argtypes = (ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.c_void_p))
    lib.cbinv_cfg.argtypes = (ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.c_void_p))
    lib.cbinv_structure.argtypes = (ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_void_p))
    lib.cbinv_free.argtypes = (ctypes.c_void_p,)
    _LIB.append(lib)
    p = ctypes.c_void_p()
    status = lib.cbinv_init(cs, ctypes.byref(p))
    answer = ctypes.string_at(p.value).decode("utf-8")
    lib.cbinv_free(p)
    if status:
        _LIB.clear()
        raise OSError(answer)
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


def msc6(code, imports, syms, rtype, params, nearmap, func_base):
    """msc6.invert: (text, meta) or (None, the reason it declined)."""
    rows = ["C\t" + bytes(code).hex(), "R\t%s" % rtype, "B\t%d" % func_base,
            "P\t%d" % (-1 if params is None else len(params))]
    rows += ["p\t%s\t%s" % (n, t) for n, t in (params or ())]
    rows += ["I\t%d\t%s" % (k, v) for k, v in (imports or {}).items()]
    rows += ["Y\t%d\t%s\t%s" % (k, v[0], v[1]) for k, v in (syms or {}).items()]
    rows += ["N\t%d\t%s" % (k, v) for k, v in (nearmap or {}).items()]
    a = json.loads(_call(_load().cbinv_msc6, "\n".join(rows) + "\n"))
    return (a["t"], a["m"]) if a["t"] is not None else (None, a["w"])


def segment_sigs(segb):
    """msc6_calls.infer_segment_sigs: {offset: [(name, kind), ...]}."""
    return {off: [tuple(p) for p in ps] for off, ps in json.loads(_call(_load().cbinv_msc6_sigs, bytes(segb).hex()))}


def thunk(fb, name="thunk", exact=True):
    """segment_thunk.verify (exact) or parse: None, or the parsed thunk with its MASM text."""
    p = ctypes.c_void_p()
    status = _load().cbinv_thunk(bytes(fb).hex().encode(), name.encode("utf-8"), 1 if exact else 0, ctypes.byref(p))
    try:
        answer = ctypes.string_at(p.value).decode("utf-8")
    finally:
        _load().cbinv_free(p)
    if status:
        raise _RAISE.get(status, RuntimeError)(answer)
    return json.loads(answer)


def candidates(fb):
    """whole_function.candidates: [(template, text), ...]."""
    return [tuple(c) for c in json.loads(_call(_load().cbinv_wf, bytes(fb).hex()))]


def mask(code):
    """msc6_function._mask."""
    return bytes.fromhex(_call(_load().cbinv_mask, bytes(code).hex()))


def small(which, code, syms=None, callee="G"):
    """0: msc6_function.invert_cleanup (syms {off: name}) -> text; 1: msc6_walk.invert (syms {off: (name,
    kind)}) and 2: msc6_templates.invert -> (text, meta) or (None, why)."""
    rows = ["C\t" + bytes(code).hex(), "K\t" + callee]
    for k, v in (syms or {}).items():
        rows.append("Y\t%d\t%s" % (k, v) if which == 0 else "Y\t%d\t%s\t%s" % (k, v[0], v[1]))
    a = json.loads(_call(_load().cbinv_small, "\n".join(rows) + "\n", which))
    if which == 0:
        return a
    return (a["t"], a["m"]) if a["t"] is not None else (None, a["w"])


def msc6_registers(which, code, imports=None):
    """0: msc6_registers.invert_reg, 1: invert_loop -> (text, meta) or (None, why)."""
    rows = ["C\t" + bytes(code).hex()] + ["I\t%d\t%s" % (k, v) for k, v in (imports or {}).items()]
    a = json.loads(_call(_load().cbinv_msc6reg, "\n".join(rows) + "\n", which))
    return (a["t"], a["m"]) if a["t"] is not None else (None, a["w"])


def _enc(x):
    """A model's value as pyval JSON (native/invert/pyval.h): a tuple an array, a list {"L": [..]},
    a dict {"D": [[key, value], ..]}."""
    if isinstance(x, tuple):
        return [_enc(v) for v in x]
    if isinstance(x, list):
        return {"L": [_enc(v) for v in x]}
    if isinstance(x, dict):
        return {"D": [[k, _enc(v)] for k, v in x.items()]}
    return x


def _dec(x):
    if isinstance(x, list):
        return tuple(_dec(v) for v in x)
    if isinstance(x, dict):
        if "L" in x:
            return [_dec(v) for v in x["L"]]
        return {k: _dec(v) for k, v in x["D"]}
    return x


def msvc2_invert(code, mask=None):
    """msvc2.invert: {nargs, expr, c, node} or None."""
    rows = ["C\t" + bytes(code).hex()]
    if mask is not None:
        rows.append("M\t" + ",".join(str(int(i)) for i in mask))
    return _dec(json.loads(_call(_load().cbinv_msvc2, "\n".join(rows) + "\n", 0)))


def msvc2_interp(code, nargs):
    """msvc2_registers.interp: a list of seq nodes, or None."""
    return _dec(json.loads(_call(_load().cbinv_msvc2, "C\t%s\nN\t%d\n" % (bytes(code).hex(), nargs), 1)))


def msvc2_emit(node):
    """msvc2_forward.emit: the bytes, or None."""
    h = json.loads(_call(_load().cbinv_msvc2, json.dumps(_enc(node)), 2))
    return None if h is None else bytes.fromhex(h)


def msvc2_to_c(node):
    """msvc2_forward.to_c."""
    return json.loads(_call(_load().cbinv_msvc2, json.dumps(_enc(node)), 3))


def msvc2_frame_layout(locals_):
    """msvc2_forward.frame_layout: (framesize, {decl_index: ebp_offset})."""
    cum, offs = json.loads(_call(_load().cbinv_msvc2, json.dumps(_enc(list(locals_))), 4))
    return cum, {i: off for i, off in offs}
