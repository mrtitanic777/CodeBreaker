#!/usr/bin/env python3
r"""seh.py -- recover structured exception handling (__try/__except).

A function that installs an SEH frame does it in its prologue: push a handler address, push the
previous `fs:[0]`, and point `fs:[0]` at the new frame. This detects that install and recovers the
handler address, so the reconstructed body can be wrapped `__try { ... } __except(sub_<handler>) {}`
instead of silently dropping the exception structure (the `has_seh` functions).
"""
import re
from codebreaker.emulate import trace32 as tracer


def detect(module, va, scan=24):
    """Return the exception-handler VA if the function installs an SEH frame, else None.

    Prologue idiom: `push <handler>` ... `mov eax, fs:[0]` / `push eax` ... `mov fs:[0], esp`
    (or `mov fs:[0], <reg>`). The handler is the last immediate pushed before fs:[0] is written."""
    try:
        import capstone
        _, va0, code, _s, _f, _sha, _imp = tracer._load(module)
        off = va - va0
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        rows = list(md.disasm(code[off:off + scan * 8], va))
    except Exception:
        return None
    last_push_imm = None
    for i, ins in enumerate(rows[:scan]):
        if ins.mnemonic == "push" and ins.op_str.startswith("0x"):
            try:
                last_push_imm = int(ins.op_str, 16)
            except ValueError:
                pass
        # fs:[0] written -> the SEH frame is now installed
        if ins.mnemonic == "mov" and "fs:[" in ins.op_str.replace(" ", "") \
                and ins.op_str.split(",")[0].replace(" ", "").startswith("dwordptrfs:[0"):
            if last_push_imm and last_push_imm >= va0:
                return last_push_imm
    return None


def wrap(src, handler_va, va0):
    """Wrap a reconstructed body's statements in __try/__except(handler)."""
    lines = src.splitlines()
    try:
        open_i = next(i for i, l in enumerate(lines) if l.rstrip().endswith("{"))
        close_i = max(i for i, l in enumerate(lines) if l.strip() == "}")
    except (StopIteration, ValueError):
        return src
    head = lines[:open_i + 1]
    body = lines[open_i + 1:close_i]
    hname = "sub_%x" % (handler_va - va0 if handler_va >= va0 else handler_va)
    inner = ["    __try {"] + ["    " + b for b in body] + \
            ["    } __except (%s(GetExceptionInformation())) {" % hname, "        /* exception path */", "    }"]
    return "\n".join(head + inner + ["}"])
