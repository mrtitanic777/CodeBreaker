#!/usr/bin/env python3
"""mzasmrecon.py - byte-exact 16-bit ASM reconstruction for MZ-DOS functions,
emitting the WinTranspiler engine corpus format (sec<seg>_<off5>.asm) and verifying
via the engine's own masmlift (MASM under DOSBox) + objextract.

This is the MZ front-end adapter: it does what win1.0's asmrecon.py does (disassemble
-> MASM -> assemble -> diff -> db-fallback until byte-exact) but sources instructions
from capstone on a flat MZ image instead of trace/NE. Output is consumable by
matchbatch/matchsearch/raiser (the automated C-matching layer) unchanged.

Usage: python mzasmrecon.py <image.bin> <hexoff> <len> [--emit path.asm]
"""
import sys, os, re
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
from codebreaker.engines.recon import masmlift
from codebreaker.formats import objextract

_MD = Cs(CS_ARCH_X86, CS_MODE_16)

BRANCH = {"jmp","call","je","jne","jz","jnz","jl","jle","jg","jge","jb","jbe","ja","jae",
          "jc","jnc","jo","jno","js","jns","jp","jnp","jcxz","loop","loope","loopne","jcxz"}

def masm_hex(m):
    s = m.group(0)[2:]
    return ("0"+s+"h") if s[0] in "abcdef" else (s+"h")

def to_masm(op_str):
    """capstone Intel op_str -> MASM-16 operands (hex 0x1f->1fh, keep ptr/seg forms)."""
    s = re.sub(r"0x[0-9a-fA-F]+", masm_hex, op_str)
    return s

def recon(image, off, length, seg=1):
    code = image[off:off+length]
    insns = list(_MD.disasm(code, off))
    # intra-function branch targets -> labels
    labels = {}
    for ins in insns:
        if ins.mnemonic in BRANCH and ins.op_str.strip().replace("h","").startswith(("0x","0")) or ins.mnemonic in BRANCH:
            m = re.match(r"0x([0-9a-fA-F]+)$", ins.op_str.strip())
            if m:
                t = int(m.group(1),16)
                if off <= t < off+length: labels[t] = "L_%05x" % t
    # precompute forced-db: absolute [imm16] addressing, and branches leaving the function
    # (their displacement is layout-dependent -> db, exactly like asmrecon's external calls).
    def _ext_branch(ins):
        if ins.mnemonic not in BRANCH: return False
        m = re.match(r"0x([0-9a-fA-F]+)$", ins.op_str.strip())
        if not m: return True   # register/indirect call -> db to be safe
        t = int(m.group(1),16)
        return not (off <= t < off+length)
    db_force = set(ins.address for ins in insns if _needs_db(ins) or _ext_branch(ins))
    def build():
        lines = ["        title   mzrecon",
                 "_TEXT   segment byte public 'CODE'","        assume  cs:_TEXT"]
        for ins in insns:
            a = ins.address
            if a in labels: lines.append("%s:" % labels[a])
            bs = " ".join("%02x"%b for b in ins.bytes)
            if a in db_force:
                dbb = ", ".join((("0"+("%02x"%b)) if ("%02x"%b)[0] in "abcdef" else ("%02x"%b))+"h" for b in ins.bytes)
                lines.append("        db      %s ; %05x %s %s" % (dbb, a, ins.mnemonic, ins.op_str))
            else:
                ops = ins.op_str
                m = re.match(r"0x([0-9a-fA-F]+)$", ops.strip())
                if ins.mnemonic in BRANCH and m and int(m.group(1),16) in labels:
                    ops = labels[int(m.group(1),16)]   # intra-function branch -> label (position-independent)
                txt = ("%s %s" % (ins.mnemonic, to_masm(ops))).strip()
                lines.append("        %-32s ; %05x %s" % (txt, a, bs))
        lines += ["_TEXT   ends","        end"]
        return "\n".join(lines)+"\n"
    # assemble/diff/db-fallback loop
    for _ in range(40):
        asm = build()
        try:
            r = masmlift.assemble(asm)
            _nm, got, _mask = objextract.Obj(r["obj"]).code_segment()
        except Exception as e:
            return None, "masm error: %s"%e, asm
        if got == code:
            live = sum(1 for ins in insns if ins.address not in db_force)
            return asm, "EXACT live=%d/%d db=%d"%(live,len(insns),len(insns)-live), asm
        # find first differing offset -> force that instruction to db
        n = min(len(got), len(code)); bad = next((i for i in range(n) if got[i]!=code[i]), n)
        # map bad byte offset (within function) to the instruction covering it
        forced = None
        for ins in insns:
            if ins.address <= off+bad < ins.address+ins.size and ins.address not in db_force:
                db_force.add(ins.address); forced=ins.address; break
        if forced is None:
            return None, "stuck at byte %d (len got=%d exp=%d)"%(bad,len(got),len(code)), asm
    return None, "no convergence", build()

def _needs_db(ins):
    # absolute memory addressing [imm16] (a0-a3 and mod=00,rm=110 forms): MASM encodes the
    # bracketed literal as an immediate, not a memory ref, so these must be db-emitted.
    return bool(re.search(r"\[0x[0-9a-fA-F]+\]", ins.op_str))

if __name__ == "__main__":
    img = open(sys.argv[1],"rb").read(); off=int(sys.argv[2],16); ln=int(sys.argv[3])
    asm, verdict, full = recon(img, off, ln)
    print("VERDICT:", verdict)
    if "--emit" in sys.argv and asm:
        p = sys.argv[sys.argv.index("--emit")+1]; open(p,"w",newline="\n").write(asm); print("wrote",p)
    if "-v" in sys.argv: print(full)
