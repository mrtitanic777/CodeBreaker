#!/usr/bin/env python3
r"""msvc2reg.py -- the UNIFIED register-dataflow interpreter for the MSVC 2.0 (win95) inverse.

The 32-bit sibling of msc6reg.py. A small abstract interpreter walks a function's instruction
stream once, tracking each register's symbolic value (`env: reg -> Value`), and emits a
statement-list node.  This is the composable core the pattern-matchers could not be: because
one walk threads register state through calls, loads, ALU ops and callee-saved parks, features
COMBINE -- a void call followed by a return that uses the *next* call's result, an arg reloaded
after a call, a value parked in esi across a call, etc., all fall out of the same walk instead
of needing a bespoke template each.

Output node = {nargs, stmts, ret, saved, leave} in the vocabulary msvc2model._emit_seq already
emits, so every recovery stays self-verified (emit(node) == bytes) -- the interpreter only
proposes structure; byte-identity is still the sole acceptance test.

This first cut handles STRAIGHT-LINE bodies (no branches): frame [+ callee-saved push] ; a run
of statements (calls whose result is dead -> void-call statements) ; a tail return whose value
is the live `eax` (a call result, optionally combined with one arg/const, or a stack expr).
Control flow and multi-park register allocation compose onto this next (see docs/MSVC2_DATAFLOW).
"""
import re

try:
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    _MD = Cs(CS_ARCH_X86, CS_MODE_32)
except Exception:
    _MD = None

_CALLER_SAVED = ("eax", "ecx", "edx")
_CALLEE_SAVED = ("esi", "edi", "ebx")
_ALU = {"add": "+", "sub": "-", "and": "&", "or": "|", "xor": "^"}


def _argk_ebp(opstr):
    """arg index from `[ebp + N]` (framed), else None."""
    m = re.search(r"\[ebp \+ (0x[0-9a-f]+|\d+)\]", opstr)
    if not m:
        return None
    off = int(m.group(1), 0)
    return (off - 4) // 4 if off >= 8 and (off - 4) % 4 == 0 else None


def _sconst(s):
    try:
        v = int(s, 0)
    except ValueError:
        return None
    return v - 0x100000000 if v > 0x7fffffff else v


def interp(code, nargs):
    """Walk a straight-line framed function -> a seq node {nargs, stmts, ret, saved, leave}, or
    None if it steps outside this cut.  The caller emit-verifies the node."""
    if _MD is None:
        return None
    ins = list(_MD.disasm(bytes(code), 0))
    if not ins:
        return None
    i = 0
    if not (len(ins) >= 2 and (ins[0].mnemonic, ins[0].op_str) == ("push", "ebp")
            and (ins[1].mnemonic, ins[1].op_str) == ("mov", "ebp, esp")):
        return None                                          # framed only, this cut
    i = 2
    saved = []
    while i < len(ins) and ins[i].mnemonic == "push" and ins[i].op_str in _CALLEE_SAVED:
        saved.append(ins[i].op_str)
        i += 1

    # epilogue: [pop callee-saved]* ; (pop ebp | leave) ; ret
    hi = len(ins)
    if hi > i and ins[hi - 1].mnemonic in ("ret", "retn"):
        hi -= 1
    else:
        return None
    leave = False
    if hi > i and ins[hi - 1].mnemonic == "leave":
        leave = True
        hi -= 1
    elif hi > i and (ins[hi - 1].mnemonic, ins[hi - 1].op_str) == ("pop", "ebp"):
        hi -= 1
    else:
        return None
    for r in reversed(saved):                                # matching pop order
        if hi > i and (ins[hi - 1].mnemonic, ins[hi - 1].op_str) == ("pop", r):
            hi -= 1
        else:
            return None

    body = ins[i:hi]
    env = {}                    # reg -> Value
    stmts = []
    pending = []                # pushed call args (Values), in push order
    ret = None

    def flush_dead_eax():
        """If eax holds an un-consumed call, it was a statement (its result is discarded)."""
        v = env.get("eax")
        if v is not None and v[0] in ("call", "icall"):
            stmts.append(("voidcall", "__stdcall", v[2]) if v[0] == "call" else ("ivoidcall", v[1]))

    for x in body:
        m, o = x.mnemonic, x.op_str
        if m == "push":
            if "[" in o:
                k = _argk_ebp(o)
                if k is None:
                    return None
                pending.append(("arg", k))
            elif o in env:                                   # push reg -> its tracked value
                pending.append(env[o])
            else:
                c = _sconst(o)
                if c is None:
                    return None
                pending.append(("const", c))
        elif m == "call":
            if "[" in o and not re.search(r"\[(0x[0-9a-f]+|\d+)\]", o):
                return None
            flush_dead_eax()                                 # a prior dead call becomes a statement
            cargs = list(reversed(pending))
            pending = []
            val = ("icall", cargs) if "[" in o else ("call", "__stdcall", cargs)
            env = {k: v for k, v in env.items() if k in _CALLEE_SAVED}   # calls clobber caller-saved
            env["eax"] = val
        elif m == "mov":
            dst, _, src = o.partition(", ")
            if dst not in ("eax", "ecx", "edx", "esi", "edi", "ebx"):
                return None                                  # store to memory -- not this cut
            k = _argk_ebp(o)
            consumes_eax = False
            if k is not None:                                # mov reg,[ebp+argk]
                nv = ("arg", k)
            elif src in ("eax", "ecx", "edx", "esi", "edi", "ebx"):
                nv = env.get(src)                            # reg->reg copy / callee-saved park
                consumes_eax = src == "eax"
            else:
                ml = re.match(r"(?:dword ptr )?\[(eax|ecx|edx|esi|edi|ebx)(?: \+ (0x[0-9a-f]+|\d+))?\]$", src)
                mg = re.match(r"(?:dword ptr )?\[(0x[0-9a-f]+|\d+)\]$", src)
                if ml and env.get(ml.group(1)) is not None:  # mov reg,[base+disp] -> load off a value
                    nv = ("load", env[ml.group(1)], int(ml.group(2), 0) if ml.group(2) else 0)
                    consumes_eax = ml.group(1) == "eax"
                elif mg and dst == "eax":                    # mov eax,[disp32] -> a global
                    nv = ("global",)
                else:
                    c = _sconst(src)
                    nv = ("const", c) if c is not None else None
            if dst == "eax" and not consumes_eax:            # overwriting eax without using it: a
                flush_dead_eax()                             # prior un-consumed call was a statement
            env[dst] = nv
        elif m == "xor" and o in ("eax, eax", "ecx, ecx", "edx, edx"):
            if o == "eax, eax":
                flush_dead_eax()
            env[o.split(",")[0]] = ("const", 0)
        elif m in ("inc", "dec") and o in env:
            env[o] = ("binop", "+" if m == "inc" else "-", env[o], ("const", 1))
        elif m in _ALU and o.startswith("eax"):
            _, _, rhs = o.partition(", ")
            if rhs in env:                                   # op eax, reg (e.g. add eax,esi)
                if env["eax"] is None:
                    return None
                env["eax"] = ("binop", _ALU[m], env["eax"], env[rhs])
            else:
                k = _argk_ebp(o)
                rv = ("arg", k) if k is not None else (lambda c: ("const", c) if c is not None else None)(_sconst(rhs))
                if rv is None or env.get("eax") is None:
                    return None
                env["eax"] = ("binop", _ALU[m], env["eax"], rv)
        else:
            return None                                      # unmodeled instruction -> decline

    if pending or saved:
        return None                                          # esi/edi/ebx parks intertwine with the
    ret = env.get("eax")                                     # schedule -> deferred to the next cut
    # the returned value is the live eax, as a raw Value -- _materialize/_emit_seq reproduce it
    # (call/icall/load/global/deref/binop) and emit-verify is the acceptance test.
    return {"nargs": nargs, "stmts": stmts, "ret": ret, "leave": leave}
