#!/usr/bin/env python3
r"""rettype.py -- infer a function's return type (void vs value vs pointer).

The emitters stamp every reconstructed function `unsigned`, but most functions have a knowable
return type. Two signals: the function's own return paths (what last writes eax before `ret` --
nothing => void; `lea` or an allocator's result => a pointer; a call to a typed SDK function =>
that function's return type; an immediate/load => an integer), and its callers (if no caller ever
reads eax after calling it, it is void). This combines them into a single recovered return type,
so a reconstructed function reads `void`, `HANDLE`, or `LPVOID` where the evidence supports it.
"""
import re
from src.engines.behavior import tracer

_EAX = ("eax", "ax", "al")
_ALLOC = ("Alloc", "alloc", "malloc", "calloc", "realloc", "LocalLock", "GlobalLock", "LockResource")
_HANDLE_API = re.compile(r"(CreateFile|CreateWindow|CreateEvent|CreateMutex|CreateThread|"
                         r"CreateSemaphore|OpenProcess|OpenEvent|BeginPaint|GetDC|LoadLibrary|"
                         r"LoadCursor|LoadIcon|LoadBitmap|FindFirstFile|CreateDC|CreateCompatibleDC)")


def _rows(module, va):
    import capstone
    _, va0, code, _s, funcs, _sha, imports = tracer._load(module)
    off = va - va0
    end = funcs[off]["end"] if off in funcs else min((o for o in funcs if o > off), default=off + 0x400)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    return [(i.address, i.mnemonic, i.op_str, i.size) for i in md.disasm(code[off:end], va0 + off)], imports


def _writes_eax(mn, ops):
    if mn == "call":
        return True                                         # the return value convention: eax = f(...)
    dst = ops.split(",")[0].strip()
    return dst in _EAX and mn not in ("cmp", "test", "push")


def infer_from_rows(rows, last_call_name=None):
    """Return-type guess from the body alone. rows: [(addr,mn,ops,sz)]."""
    # walk to the first ret; record the last instruction that wrote eax before it
    last = None
    for a, mn, ops, sz in rows:
        if mn in ("ret", "retn"):
            break
        if _writes_eax(mn, ops):
            last = (mn, ops)
    if last is None:
        return "void"                                       # no return value produced on this path
    mn, ops = last
    if mn == "lea":
        return "void*"                                      # eax = &something
    if mn == "call":
        if last_call_name:
            if any(k in last_call_name for k in _ALLOC):
                return "void*"
            if _HANDLE_API.search(last_call_name):
                return "HANDLE"
        return "int"                                        # eax = a call result of unknown type
    if mn in ("xor", "mov", "or", "and", "add", "sub", "inc", "dec", "sar", "shr", "shl", "movzx", "movsx"):
        if "[" in ops.split(",", 1)[-1]:
            return "int"                                    # loaded a value
        return "int"
    return "int"


def infer(module, va):
    """Recovered return type string ('void' / 'int' / 'void*' / 'HANDLE'), best-effort."""
    try:
        rows, _imp = _rows(module, va)
    except Exception:
        return None
    if not rows:
        return None
    # find the name of the last call before the first ret (for SDK-typed / allocator returns)
    last_call = None
    for a, mn, ops, sz in rows:
        if mn in ("ret", "retn"):
            break
        if mn == "call":
            last_call = ops
    return infer_from_rows(rows, last_call_name=last_call)


def callers_ignore_result(module, va):
    """True if NO caller reads eax after calling `va` before overwriting it -> the callee is void.

    Best-effort over the static call graph; returns False (unknown) if it can't tell."""
    try:
        import capstone
        from src.engines.behavior import callgraph
        _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
        g = callgraph.build(module)
        off = va - va0
        callers = [o for o in g if off in g[o]["calls"]]
        if not callers:
            return False
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        any_use = False
        seen = False
        for co in callers:
            rows = list(md.disasm(code[co:funcs[co]["end"]], va0 + co))
            for i, ins in enumerate(rows):
                if ins.mnemonic == "call" and ins.op_str.endswith("%x" % (va0 + off)):
                    seen = True
                    for j in range(i + 1, min(i + 4, len(rows))):   # a few insns after the call
                        o = rows[j].op_str
                        if rows[j].mnemonic in ("mov", "test", "cmp", "push", "add", "or", "and") \
                                and ("eax" in o.split(",")[-1] or o.startswith(("test eax", "cmp eax"))
                                     or ", eax" in o):
                            any_use = True
                        if _writes_eax(rows[j].mnemonic, rows[j].op_str) and rows[j].mnemonic != "call":
                            break                            # eax redefined without being read
        return seen and not any_use
    except Exception:
        return False
