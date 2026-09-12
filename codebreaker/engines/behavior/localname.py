#!/usr/bin/env python3
r"""localname.py -- name local variables from how they are used (local_8 -> i, local_c -> p).

stackframe declares locals as `local_8`, `local_c` -- correct but anonymous. How a slot is USED
tells you what the author called it: a slot incremented each iteration and used as an index is a
loop counter (`i`, then `j`, `k` for nested loops); a slot loaded into a register that is then
dereferenced is a pointer (`p`, `p2`); a slot that only ever holds an API handle return is a handle.
This scans the function's instructions, classifies each local by its usage pattern, and returns a
rename map the emitters apply over their output -- turning mechanical slot names into the names a
reader (and the original author) would use.
"""
import re
from codebreaker.engines.behavior import tracer

_LOCAL = re.compile(r"\[ebp\s*-\s*(0x[0-9a-f]+|\d+)\]")
_INC = re.compile(r"(?:dword|word|byte) ptr \[ebp\s*-\s*(0x[0-9a-f]+|\d+)\]")
_MOV_LOAD = re.compile(r"^(e[a-z]{2}),\s*(?:dword ptr )?\[ebp\s*-\s*(0x[0-9a-f]+|\d+)\]$")
_DEREF = re.compile(r"\[(e[a-z]{2})(?:\s*[+*]|\])")


def _rows(module, va):
    import capstone
    _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
    off = va - va0
    end = funcs[off]["end"] if off in funcs else min((o for o in funcs if o > off), default=off + 0x400)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    return [(i.address, i.mnemonic, i.op_str, i.size) for i in md.disasm(code[off:end], va0 + off)]


def classify(rows):
    """{offset: role} for locals, role in {'counter','pointer'}. Order-stable for naming."""
    counters, pointers = [], []
    seen_c, seen_p = set(), set()
    # a slot incremented in place is a counter
    for _a, mn, ops, _s in rows:
        if mn in ("inc", "add", "sub") and "[ebp -" in ops.replace(" ", " "):
            m = _INC.search(ops)
            if m:
                if mn == "add" and not ops.rstrip().endswith((", 1", ",1", ", 2", ", 4")):
                    continue                                # only unit/stride increments look like counters
                off = int(m.group(1), 0)
                if off not in seen_c:
                    seen_c.add(off); counters.append(off)
    # a slot loaded into a register that is later dereferenced is a pointer
    reg_from = {}                                            # reg -> local offset it was loaded from
    for _a, mn, ops, _s in rows:
        if mn == "mov":
            m = _MOV_LOAD.match(ops.strip())
            if m:
                reg_from[m.group(1)] = int(m.group(2), 0)
                continue
        for m in _DEREF.finditer(ops):
            reg = m.group(1)
            if reg in reg_from:
                off = reg_from[reg]
                if off not in seen_p and off not in seen_c:
                    seen_p.add(off); pointers.append(off)
    roles = {}
    for off in counters:
        roles[off] = "counter"
    for off in pointers:
        if off not in roles:
            roles[off] = "pointer"
    return roles, counters, [o for o in pointers if o not in seen_c]


def suggest(module, va):
    """{'local_<off>': name} rename map from usage, or {} -- names counters i/j/k, pointers p/p2."""
    try:
        rows = _rows(module, va)
    except Exception:
        return {}
    return suggest_from_rows(rows)


def suggest_from_rows(rows):
    roles, counters, pointers = classify(rows)
    names = {}
    ci = ["i", "j", "k", "l", "m"]
    for n, off in enumerate(counters):
        names["local_%x" % off] = ci[n] if n < len(ci) else "i%d" % n
    for n, off in enumerate(pointers):
        names["local_%x" % off] = "p" if n == 0 else "p%d" % (n + 1)
    return names


def apply(src, mapping):
    """Replace whole-word local_<off> tokens in emitted C with their recovered names."""
    if not mapping:
        return src
    def repl(m):
        return mapping.get(m.group(0), m.group(0))
    return re.sub(r"local_[0-9a-f]+", repl, src)
