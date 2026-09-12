#!/usr/bin/env python3
r"""stackstring.py -- reassemble strings built on the stack byte-by-byte.

Some code constructs a string inline instead of referencing a literal -- `mov byte ptr [ebp-8],'H';
mov [ebp-7],'i'; mov [ebp-6],0`, or in dword chunks `mov dword ptr [ebp-8],'lleH'`. datarec never
sees these because there is no pointer to a data-section literal; the bytes only exist as
immediates in the code. This collects the immediate stores to consecutive stack offsets, lays the
bytes out, and recovers the printable runs as string literals -- so a stack-built `"Hello"` reads
as a string instead of a scatter of moves.
"""
import re
from codebreaker.engines.behavior import tracer

_W = {"byte": 1, "word": 2, "dword": 4}
_STORE = re.compile(r"^(byte|word|dword) ptr \[(ebp|esp)\s*([+-])\s*(0x[0-9a-f]+|\d+)\],\s*"
                    r"(0x[0-9a-f]+|\d+)$")


def recover_from_rows(rows, min_len=3):
    """[(base_reg, offset, string)] for stack strings built via immediate stores."""
    mem = {}                                                  # (base_reg, signed_off) -> byte
    for _a, mn, ops, _sz in rows:
        if mn != "mov":
            continue
        m = _STORE.match(ops.strip())
        if not m:
            continue
        w, base, sign, off, imm = m.group(1), m.group(2), m.group(3), m.group(4), m.group(5)
        try:
            o = int(off, 0) * (-1 if sign == "-" else 1)
            v = int(imm, 0)
        except ValueError:
            continue
        for k in range(_W[w]):                                # little-endian bytes of the immediate
            mem[(base, o + k)] = (v >> (8 * k)) & 0xFF
    return _assemble(mem, min_len)


def _assemble(mem, min_len):
    out = []
    keys = sorted(mem)
    i = 0
    while i < len(keys):
        base, off = keys[i]
        run = []
        j = i
        while j < len(keys) and keys[j][0] == base and keys[j][1] == off + (j - i):
            run.append(mem[keys[j]]); j += 1
        # split the run into printable substrings (NUL or non-printable ends one)
        s, start = [], off
        for k, b in enumerate(run):
            if 32 <= b < 127:
                s.append(chr(b))
            else:
                if len(s) >= min_len:
                    out.append((base, start, "".join(s)))
                s, start = [], off + k + 1
        if len(s) >= min_len:
            out.append((base, start, "".join(s)))
        i = j
    return out


def recover(module, va):
    """Recover stack strings in a function. {'ok','strings':[(reg,off,text)], 'c':...}."""
    try:
        import capstone
        _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
        off = va - va0
        end = funcs[off]["end"] if off in funcs else min((o for o in funcs if o > off), default=off + 0x400)
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        rows = [(i.address, i.mnemonic, i.op_str, i.size) for i in md.disasm(code[off:end], va0 + off)]
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    strs = recover_from_rows(rows)
    if not strs:
        return {"ok": False, "reason": "no stack strings"}
    lines = ['    char sz_%s_%x[] = "%s";' % (r, o & 0xFFFF, t.replace('"', '\\"'))
             for r, o, t in strs]
    return {"ok": True, "strings": strs, "c": "\n".join(lines)}
