#!/usr/bin/env python3
r"""globalname.py -- name and type module globals from how they are used (g_count, g_hInstance).

globals.py recovers writes to global state but names each slot by its address. How a global is USED
across the module tells you what it is: a global assigned the result of GetModuleHandle is the
instance handle; one incremented in place is a counter; one whose value is dereferenced is a
pointer. This scans every function's absolute `[0xADDR]` accesses, aggregates them per global,
infers a role + size + type, and hands back a rename map and typed declarations -- so module state
reads `g_hInstance`, `g_count`, `g_pHead` instead of `dword_40A010`.
"""
import re
from codebreaker.engines.behavior import tracer

_W = {"byte": 1, "word": 2, "dword": 4}
_CTYPE = {1: "unsigned char", 2: "unsigned short", 4: "unsigned"}
_ABS = re.compile(r"(byte|word|dword) ptr \[(0x[0-9a-f]+)\]")
_HANDLE_API = re.compile(r"(GetModuleHandle|GetCurrentProcess|GetCurrentThread|LoadLibrary)")


def _funcs(module):
    import capstone
    _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    for off in sorted(funcs):
        rows = [(i.address, i.mnemonic, i.op_str, i.size)
                for i in md.disasm(code[off:funcs[off]["end"]], va0 + off)]
        yield rows


def classify_rows(rows, acc=None):
    """Aggregate global accesses in one function into acc {addr: {'size','ptr','counter','inst'}}."""
    acc = acc if acc is not None else {}
    loaded = {}                                              # reg -> global addr it currently holds
    last_call = None
    for _a, mn, ops, _sz in rows:
        if mn == "call":
            last_call = ops
        for w, addr in _ABS.findall(ops):
            a = int(addr, 0)
            g = acc.setdefault(a, {"size": 0, "ptr": False, "counter": False, "inst": False})
            g["size"] = max(g["size"], _W[w])
            if mn in ("inc", "dec") or (mn in ("add", "sub") and ops.rstrip().endswith((", 1", ",1"))):
                g["counter"] = True
            dst = ops.split(",")[0].strip()
            if mn == "mov" and re.match(r"^e[a-z]{2}$", dst):
                loaded[dst] = a                              # reg now holds the global's value
            if mn == "mov" and ("[%s]" % addr) == dst.replace("dword ptr ", "").strip() \
                    and last_call and _HANDLE_API.search(last_call):
                g["inst"] = True                             # global = GetModuleHandle(...) result
        for m in re.finditer(r"\[(e[a-z]{2})", ops):         # a loaded global reg used as a base -> ptr
            r = m.group(1)
            if r in loaded:
                acc[loaded[r]]["ptr"] = True
    return acc


def suggest(module):
    """{addr: (name, ctype)} for the module's globals, named by role."""
    acc = {}
    try:
        for rows in _funcs(module):
            classify_rows(rows, acc)
    except Exception:
        return {}
    return _name(acc)


def _name(acc):
    out = {}
    nc = np = nf = 0
    for a in sorted(acc):
        g = acc[a]
        if g["inst"]:
            out[a] = ("g_hInstance", "HINSTANCE")
        elif g["ptr"]:
            np += 1; out[a] = ("g_p%d" % np if np > 1 else "g_ptr", "void *")
        elif g["counter"]:
            nc += 1; out[a] = ("g_count%d" % nc if nc > 1 else "g_count",
                               _CTYPE.get(g["size"], "unsigned"))
        else:
            out[a] = ("g_%x" % a, _CTYPE.get(g["size"] or 4, "unsigned"))
    return out


def declarations(namemap):
    """C declarations for the named globals."""
    seen = {}
    for a, (name, ty) in sorted(namemap.items()):
        seen[name] = ty
    return ["%s %s;" % (ty, name) for name, ty in seen.items()]


def apply(src, namemap):
    """Rewrite dword_<addr> / [0x<addr>] references in emitted C to the recovered global names."""
    if not namemap:
        return src
    for a, (name, _ty) in namemap.items():
        src = re.sub(r"\b(?:dword|word|byte)_%x\b" % a, name, src)
        src = src.replace("[0x%x]" % a, name)
    return src
