"""conventions.py -- how each function is called and what kind of function it is.

    from codebreaker.analyze import image, conventions
    facts = conventions.functions(image.load("comctl32.dll"))     # {start: fact}
    facts[va]["convention"]   "stdcall" | "cdecl" | "thiscall" | "fastcall" | "register"
                              16-bit: "pascal far" | "pascal near" | "cdecl far" | "cdecl near"
    facts[va]["args"]         number of stack arguments (words in 16-bit code), or None
    facts[va]["arg_source"]   what says so: "return" (`ret N`), "callers" (the bytes callers
                              remove after calling it), "frame" (the argument slots it reads)
    facts[va]["reg_args"]     registers it reads before writing them (ecx: `this`)
    facts[va]["result"]       "none" | "value" | "flag" | "float" | "wide" | None (not known)
    facts[va]["shape"]        "import thunk" | "jump thunk" | "empty" | "constant" | "leaf" | "calls"
    ... frame, locals, saved registers, loops, inline string operations, floating point use,
    whether it takes a variable argument list, a 16-bit function's entry sequence and, when it
    is a window, dialog, hook or enumeration procedure, which.

What each fact rests on is the code itself and its callers:

  * a function that removes its own arguments says how many in its return instruction;
  * one that leaves them to the caller is given away by its callers (`add esp, 8` after the
    call) and by the highest argument slot it reads;
  * a register read before anything wrote it was passed in;
  * a result is a value the callers use, or that the function sets just before returning;
    callers that all ignore it, and no path that sets it, mean there is none.

The facts are worked out in C++ (native/analyze, conventions.cc) over the image cbanalyze.dll
holds; duplicates and the bridge imports are found here.

The decompiler's description (decompile/describe.py) already carries the bytes a function
removes on return. What it cannot yet be told, and this finds: the argument count of a
caller-cleans function, register arguments, and a function that returns nothing.
"""
import hashlib

from codebreaker.analyze import image
from codebreaker.analyze.image import family

_THUNK_APIS = ("THUNKCONNECT32", "THUNKCONNECT16", "C16THKSL01", "C32THKSL01", "THUNKINITLS", "QT_THUNK",
               "FT_THUNK", "FT_PROLOG", "FT_EXIT", "SSONBIGSTACK", "SSCALL", "MAPLS", "UNMAPLS", "MAPSL",
               "GETVDMPOINTER32W", "ALLOCSLCALLBACK", "GETPROCADDRESS32W", "K32THK1632PROLOG", "FT_EXIT0")

def stack_delta(ins):
    """Bytes `ins` adds to what is pushed (negative: removes), or None when it is not plain."""
    mn, ops = ins.mn, ins.ops
    if mn == "push":
        return ops[0][-1] if ops else 4
    if mn == "pop":
        return -(ops[0][-1] if ops else 4)
    if mn in ("add", "sub") and len(ops) == 2 and ops[0][0] == "r" and family(ops[0][1]) == "esp" and ops[1][0] == "i":
        n = image.signed(ops[1][1], ops[1][2])
        return n if mn == "sub" else -n
    return 0 if "esp" not in ins.wr else None


def _signature(img, fn):
    """A digest of a function's code that two copies of the same function share."""
    parts = []
    for ins in fn.insns:
        if ins.target is not None and ins.flow:
            where = ("+%d" % (ins.target - fn.start)) if fn.start <= ins.target < fn.limit else img.label(ins.target)
            parts.append("%s %s" % (ins.mn, where))
        else:
            parts.append(ins.text)
    return hashlib.sha1("\n".join(parts).encode()).hexdigest()


def _functions(img):
    """The facts, worked out in C++ (native/analyze, conventions.cc) over the image the DLL holds."""
    return {f["addr"]: f for f in img._handle().ask("conventions.functions")}


def functions(img):
    """{start: fact} for every function of the image."""
    return img.memo("conventions", _functions)


def duplicates(img, least=6):
    """Groups of functions with the same code (the same source compiled more than once)."""
    groups = {}
    for fn in img.each():
        if len(fn.insns) >= least:
            groups.setdefault(_signature(img, fn), []).append(fn.start)
    return sorted((g for g in groups.values() if len(g) > 1), key=lambda g: g[0])


def thunk_layer(img):
    """The 16/32-bit bridge functions a module imports ([] when it is not such a layer)."""
    names = {n.split("!")[-1].upper() for n in img.imports.values()}
    return sorted(n for n in names if n in _THUNK_APIS)


def analyse(img):
    facts = functions(img)
    dup = duplicates(img)
    bridge = thunk_layer(img)
    rows = list(facts.values())
    n = lambda test: sum(1 for f in rows if test(f))         # noqa: E731
    counts = {"functions": len(rows),
              "convention_known": n(lambda f: f.get("convention")),
              "callee_removes_arguments": n(lambda f: f.get("removes")),
              "caller_removes_arguments": n(lambda f: f.get("removes") == 0),
              "argument_count_known": n(lambda f: f.get("args") is not None),
              "argument_count_from_return": n(lambda f: f.get("arg_source") == "return"),
              "argument_count_from_callers": n(lambda f: f.get("arg_source") == "callers"),
              "argument_count_from_frame": n(lambda f: f.get("arg_source") == "frame"),
              "register_arguments": n(lambda f: f.get("reg_args")),
              "this_in_ecx": n(lambda f: f.get("convention") == "thiscall"),
              "result_known": n(lambda f: f.get("result")),
              "result_none": n(lambda f: f.get("result") == "none"),
              "import_thunks": n(lambda f: f.get("shape") == "import thunk"),
              "jump_thunks": n(lambda f: f.get("shape") == "jump thunk"),
              "tail_calling": n(lambda f: f.get("tail_calls")),
              "empty_or_constant": n(lambda f: f.get("shape") in ("empty", "constant")),
              "leaf": n(lambda f: f.get("shape") == "leaf"),
              "with_loops": n(lambda f: f.get("loops")),
              "inline_string_operations": n(lambda f: f.get("string_ops")),
              "floating_point": n(lambda f: f.get("float_instructions")),
              "variadic": n(lambda f: f.get("variadic")),
              "system_called_procedures": n(lambda f: f.get("procedure")),
              "duplicate_groups": len(dup), "duplicate_functions": sum(len(g) for g in dup),
              "bridge_imports": len(bridge)}
    out = rows + [{"addr": g[0], "kind": "duplicates", "functions": [img.label(f) for f in g]} for g in dup]
    if bridge:
        out.append({"addr": None, "kind": "thunk_layer", "imports": bridge})
    by = {}
    for f in rows:
        by[f.get("convention") or "unknown"] = by.get(f.get("convention") or "unknown", 0) + 1
    text = ["conventions: " + ", ".join("%s %d" % kv for kv in sorted(by.items(), key=lambda kv: -kv[1]))]
    procs = [f for f in rows if f.get("procedure")]
    if procs:
        text.append("system-called procedures: " + ", ".join("%s (%s)" % (f["name"], f["procedure"]) for f in procs[:12]))
    if bridge:
        text.append("16/32-bit bridge imports: " + ", ".join(bridge))
    if dup:
        text.append("identical functions: %d groups, largest %d copies" % (len(dup), max(len(g) for g in dup)))
    return {"counts": counts, "facts": out, "text": text}
