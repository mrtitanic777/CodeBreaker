#!/usr/bin/env python3
r"""ml32.py -- byte-exact 32-bit ASM verification via native MASM 6.11c (ML.EXE).

The period-correct assembler for Win95 hand-asm. ML 6.11c is a Win32 PE (i386)
and runs natively on modern Windows (WOW64) -- no DOSBox, unlike the masmlift
path asmrecon32 uses. `ML /c /coff` emits a COFF object cc32.Obj parses; a
reconstruction is byte-exact when the assembled code, masked at its own
relocation slots (the only bytes whose address legitimately differs from the
linked image), equals the shipped bytes masked at the same slots.
"""
import os
import subprocess
import tempfile

import pe32util as H          # first: arms sys.path + active target for the bare imports
import cc32
import segdiff
from src.core import layout


def _ml():
    for root in (os.getcwd(), os.path.dirname(H.APP_DIR)):
        tc = layout.reference_sub("toolchain", base=root)
        for cand in (os.path.join(tc, "masm611c", "ML.EXE"),):
            if os.path.exists(cand):
                return cand
    for root in (os.getcwd(),):
        cand = os.path.join(root, "docs", "reference", "win95ddk", "MASM611C", "ML.EXE")
        if os.path.exists(cand):
            return cand
    return ""


def assemble(asm_text):
    """ML 6.11c /c /coff -> (coff_obj_bytes | None, log). Run from a temp dir with
    relative filenames -- ML's DOS-style parser rejects '/'-slashed paths."""
    ml = _ml()
    if not ml:
        raise RuntimeError("ML.EXE (MASM 6.11c) not found under reference/toolchain/masm611c")
    with tempfile.TemporaryDirectory(prefix="ml32_") as td:
        with open(os.path.join(td, "t.asm"), "w", newline="\r\n") as f:
            f.write(asm_text)
        r = subprocess.run([ml, "/c", "/coff", "/Fot.obj", "t.asm"],
                           cwd=td, capture_output=True, text=True, timeout=60)
        op = os.path.join(td, "t.obj")
        obj = open(op, "rb").read() if os.path.exists(op) else None
        return obj, (r.stdout or "") + (r.stderr or "")


def obj_code(obj):
    """Whole first code section of the COFF obj: (bytes, reloc-mask-set)."""
    secs = cc32.Obj(obj)._code_sections()
    if not secs:
        return b"", set()
    _idx, code, mask = secs[0]
    return code, set(mask)


def verify_range(asm_text, module, start_rva, end_rva, show=True):
    """Assemble; compare its code to the shipped bytes of [start_rva,end_rva),
    masking (on both sides) the assembled object's own relocation slots."""
    obj, log = assemble(asm_text)
    if obj is None:
        if show:
            print("  ASSEMBLE FAILED:\n" + log[:1200])
        return False, log
    code, mask = obj_code(obj)
    c = H.carve(module); base = c.pe.image_base
    shipped = H.func_bytes(c, base + start_rva, base + end_rva)
    n = min(len(code), len(shipped))
    m = {x for x in mask if x < n}
    exact = (len(code) == len(shipped)) and cc32.masked(code[:n], m) == cc32.masked(shipped[:n], m)
    if show:
        print("  assembled %dB / shipped %dB / masked %d -> %s"
              % (len(code), len(shipped), len(mask), "BYTE-EXACT" if exact else "DIFF"))
    return exact, log
