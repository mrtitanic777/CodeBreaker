#!/usr/bin/env python3
r"""sigrefine.py -- small signature refinements: varargs, boolean params, duplicate folding.

Three low-cost passes that sharpen a recovered signature or the module view:
  * is_variadic -- a function that takes the address of an argument slot and then walks it (`lea
    reg,[ebp+N]; ... add reg,4`) is using va_start; its signature ends in `...`.
  * bool_params -- a parameter used only in a truth test (`cmp aN,0` / `test`), never in arithmetic,
    is a BOOL, not an `unsigned`.
  * fold_identical -- functions with identical code are the same source emitted more than once
    (templates, inlined-then-shared helpers the linker did not COMDAT-fold); grouping them avoids
    reconstructing the same body many times.
"""
import re
import hashlib
from src.engines.behavior import tracer

_LEA_ARG = re.compile(r"^e[a-z]{2},\s*(?:dword ptr )?\[ebp\s*\+\s*(0x[0-9a-f]+|\d+)\]$")


def _rows(module, va):
    import capstone
    _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
    off = va - va0
    end = funcs[off]["end"] if off in funcs else min((o for o in funcs if o > off), default=off + 0x400)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    return [(i.address, i.mnemonic, i.op_str, i.size) for i in md.disasm(code[off:end], va0 + off)]


def is_variadic_rows(rows):
    """True if the function takes &arg-slot then walks it (the va_start / va_arg idiom)."""
    argptr = set()
    for _a, mn, ops, _sz in rows:
        m = _LEA_ARG.match(ops.strip())
        if mn == "lea" and m and int(m.group(1), 0) >= 8:
            argptr.add(ops.split(",")[0].strip())            # reg = &arg slot
        parts = [p.strip() for p in ops.split(",")]
        if mn in ("add", "lea") and parts and parts[0] in argptr and parts[-1] in ("4", "8", "0x4", "0x8"):
            return True                                       # walking the argument list
    return False


def bool_params_rows(rows, arity=6):
    """Arg indices used only as a truth value (cmp/test vs 0), never in arithmetic -> BOOL params."""
    slots = {k: "[ebp + 0x%x]" % (8 + 4 * k) if (8 + 4 * k) >= 0xA else "[ebp + %d]" % (8 + 4 * k)
             for k in range(arity)}
    truth, arith = set(), set()
    for _a, mn, ops, _sz in rows:
        for k, slot in slots.items():
            if slot not in ops and ("[ebp + %d]" % (8 + 4 * k)) not in ops:
                continue
            if mn in ("cmp", "test") and (", 0" in ops or ", 1" in ops or mn == "test"):
                truth.add(k)
            elif mn in ("add", "sub", "imul", "mul", "lea", "or", "and", "xor", "shl", "shr", "push"):
                arith.add(k)
            elif mn == "mov":
                pass                                          # a plain load is neutral
            else:
                arith.add(k)
    return sorted(truth - arith)


def is_variadic(module, va):
    try:
        return is_variadic_rows(_rows(module, va))
    except Exception:
        return False


def bool_params(module, va, arity=6):
    try:
        return bool_params_rows(_rows(module, va), arity)
    except Exception:
        return []


def _mask_code(code):
    """Zero the rel32 operand of E8/E9 (call/jmp) so relocation-differing duplicates still match."""
    b = bytearray(code)
    i = 0
    while i < len(b):
        if b[i] in (0xE8, 0xE9) and i + 5 <= len(b):
            b[i + 1:i + 5] = b"\x00\x00\x00\x00"
            i += 5
        else:
            i += 1
    return bytes(b)


def fold_identical(module):
    """{code_hash: [offsets]} for functions whose (reloc-masked) code is identical (dupes)."""
    _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
    groups = {}
    for off, f in funcs.items():
        h = hashlib.md5(_mask_code(code[off:f["end"]])).hexdigest()
        groups.setdefault(h, []).append(off)
    return {h: sorted(v) for h, v in groups.items() if len(v) > 1}
