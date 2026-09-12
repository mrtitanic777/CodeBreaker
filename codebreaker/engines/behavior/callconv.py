#!/usr/bin/env python3
r"""callconv.py -- detect a function's calling convention (stdcall / cdecl / thiscall / fastcall).

The emitters guess arg order and (for a caller) how many arguments a callee takes. Knowing the
convention pins both -- and the callee-clean `ret N` even hands us the argument byte count when no
SDK proto exists. This reads the function statically:
  * `ret N` (N>0)  -> the CALLEE cleans the stack -> stdcall family; N/4 dword args.
  * `ret` / `ret 0`-> the CALLER cleans -> cdecl (the C default; also all varargs).
  * ecx read before it is written -> `this` (thiscall) or a fastcall register arg.
  * ecx AND edx both read-before-write -> two register args -> fastcall.
Convention = combine the two axes. A lone `ret` with an early ecx read is still thiscall for a
member function that takes no stack args, so the register evidence is reported alongside.
"""
from codebreaker.engines.behavior import tracer

# registers that carry arguments under thiscall/fastcall, in order
_REGS8 = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"}


def _fend(funcs, off):
    if off in funcs:
        return funcs[off]["end"]
    nx = sorted(o for o in funcs if o > off)
    return nx[0] if nx else off + 0x400


def _reads_before_writes(rows, reg):
    """True if `reg` (or its sub-registers) is read as a source before it is ever defined."""
    fam = {reg, reg.replace("e", "", 1)}                     # ecx -> {ecx, cx}
    fam.add(reg[1] + "l")                                    # ecx -> cl
    fam.add(reg[1] + "h")
    for ins in rows:
        ops = ins.op_str
        # a write to reg as the destination (first operand) "defines" it
        dst = ops.split(",")[0].strip()
        srcs = ops.split(",")[1:] if "," in ops else []
        if any(r in " ".join(srcs) for r in fam):
            return True                                       # used as a source first
        if dst in fam and ins.mnemonic in ("mov", "lea", "pop", "xor", "sub", "and"):
            return False                                      # defined before any read
    return False


def detect(module, va):
    """{'conv','argbytes','ret_imm','ecx_arg','edx_arg'} or None. `conv` is a best-effort label."""
    try:
        import capstone
        _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
        off = va - va0
        end = _fend(funcs, off)
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        rows = list(md.disasm(code[off:end], va))
    except Exception:
        return None
    if not rows:
        return None
    ret_imm = None
    for ins in rows:
        if ins.mnemonic in ("ret", "retn"):
            try:
                ret_imm = int(ins.op_str, 0) if ins.op_str.strip() else 0
            except ValueError:
                ret_imm = 0
            break                                             # first return is representative
    # register-arg evidence: only meaningful within the first block, so scan a prologue window
    head = rows[:40]
    ecx_arg = _reads_before_writes(head, "ecx")
    edx_arg = ecx_arg and _reads_before_writes(head, "edx")
    callee_clean = bool(ret_imm)
    if edx_arg:
        conv = "fastcall"
    elif ecx_arg and callee_clean:
        conv = "thiscall"
    elif ecx_arg:
        conv = "thiscall"                                    # member fn, no stack args
    elif callee_clean:
        conv = "stdcall"
    elif ret_imm == 0:
        conv = "cdecl"
    else:
        conv = "unknown"
    argbytes = ret_imm if ret_imm else None
    return {"conv": conv, "argbytes": argbytes, "ret_imm": ret_imm,
            "ecx_arg": ecx_arg, "edx_arg": edx_arg,
            "stack_args": (argbytes // 4) if argbytes else None}
