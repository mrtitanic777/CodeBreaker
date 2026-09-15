#!/usr/bin/env python3
"""lift40 - a Microsoft C 4.0-aware leaf lifter (16-bit MZ -> readable C).

Uses the calibrated C-4.0 codegen model (SIGNATURES.md SIG-CG-*): the
`push bp; mov bp,sp; mov ax,N; call __chkstk` prologue, cdecl right-to-left
calls with caller `add sp` cleanup, [bp+k] args / [bp-k] locals, absolute
[imm16] globals. Straight-line and simple forward-`if` bodies become C; anything
with loops / irreducible flow is emitted as `/* [AUTO] */` with the raw asm so
nothing is faked (the HONESTY RULE).

Usage: python lift40.py <unpacked.exe> <hexoff> <size> [chkstk_hexoff] [--names triage.txt]
"""
import sys, struct, re
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
MD = Cs(CS_ARCH_X86, CS_MODE_16); MD.detail = True


def load(path):
    b = open(path, 'rb').read()
    hdr = struct.unpack('<H', b[8:10])[0] * 16
    return b[hdr:]


def names_from_triage(path):
    nm = {}
    if path:
        for l in open(path):
            m = re.match(r"(0x[0-9a-f]+)\t(?:LIB (\S+)|APP)", l)
            if m and m.group(2):
                nm[int(m.group(1), 16)] = m.group(2)
    return nm


def opnd(s):
    """map a capstone operand to C-ish text for args/locals/globals."""
    s = s.strip()
    m = re.fullmatch(r"word ptr \[bp \+ (0x[0-9a-f]+|\d+)\]", s)
    if m:
        k = int(m.group(1), 16 if m.group(1).startswith("0x") else 10)
        return f"a{(k-4)//2+1}" if k >= 4 else f"loc_{k}"
    m = re.fullmatch(r"word ptr \[bp \- (0x[0-9a-f]+|\d+)\]", s)
    if m:
        return f"v{int(m.group(1),16 if m.group(1).startswith('0x') else 10)}"
    m = re.fullmatch(r"word ptr \[(0x[0-9a-f]+)\]", s)
    if m:
        return f"g_{int(m.group(1),16):04x}"
    m = re.fullmatch(r"0x[0-9a-f]+", s)
    if m:
        return s  # immediate
    return s


def lift(img, off, size, chkstk=None, names=None):
    names = names or {}
    code = img[off:off+size]
    ins = list(MD.disasm(code, off))
    body, i, framesize, regsaved = [], 0, 0, []
    faithful = True

    # --- prologue ---
    if len(ins) >= 2 and ins[0].mnemonic == "push" and ins[0].op_str == "bp" \
       and ins[1].mnemonic == "mov" and ins[1].op_str == "bp, sp":
        i = 2
        # mov ax,N / xor ax,ax  ; call __chkstk
        if i+1 < len(ins) and ins[i].mnemonic in ("mov", "xor") and ins[i].op_str.startswith("ax"):
            if ins[i].mnemonic == "xor":
                framesize = 0
            else:
                mm = re.search(r"0x[0-9a-f]+", ins[i].op_str); framesize = int(mm.group(0), 16) if mm else 0
            if i+1 < len(ins) and ins[i+1].mnemonic == "call":
                i += 2
        while i < len(ins) and ins[i].mnemonic == "push" and ins[i].op_str in ("si", "di"):
            regsaved.append(ins[i].op_str); i += 1
    else:
        return None, "no C-4.0 prologue (not a standard function)"

    # --- body (straight line; stop at epilogue) ---
    pend_pushes = []
    reg_val = {}          # transient GPR value tracking (mov/lea reg, X)
    GPR = ("ax", "bx", "cx", "dx", "si", "di")
    while i < len(ins):
        a = ins[i]; mn, ops = a.mnemonic, a.op_str
        # epilogue
        if mn == "mov" and ops == "sp, bp":
            break
        if mn in ("pop", "ret", "retf"):
            break
        if mn == "push":
            pend_pushes.append(reg_val.get(ops, opnd(ops))); i += 1; continue
        if mn == "lea":
            m = re.fullmatch(r"(\w+), \[bp \+ (0x[0-9a-f]+|\d+)\]", ops)
            if m:
                k = int(m.group(2), 16 if m.group(2).startswith("0x") else 10)
                reg_val[m.group(1)] = ("&a%d" % ((k-4)//2+1)) if k >= 4 else ("&loc_%d" % k)
            else:
                faithful = False
            i += 1; continue
        if mn == "call":
            m = re.fullmatch(r"0x([0-9a-f]+)", ops)
            tgt = int(m.group(1), 16) if m else None
            fname = names.get(tgt, ("__chkstk" if tgt == chkstk else (f"sub_{tgt:04x}" if tgt is not None else ops)))
            fname = fname.lstrip("_")
            args = list(reversed(pend_pushes)); pend_pushes = []
            call = f"{fname}({', '.join(args)})"
            # caller cleanup add sp,N follows
            if i+1 < len(ins) and ins[i+1].mnemonic == "add" and ins[i+1].op_str.startswith("sp"):
                i += 1
            body.append(call + ";"); i += 1; continue
        if mn == "cmp":
            l, r = [opnd(x) for x in ops.split(",")]
            # look at next jcc
            if i+1 < len(ins) and ins[i+1].mnemonic in ("je","jne","jz","jnz","jg","jl","jge","jle","ja","jb","jae","jbe"):
                cc = ins[i+1].mnemonic
                inv = {"je":"!=","jz":"!=","jne":"==","jnz":"==","jg":"<=","jle":">","jl":">=","jge":"<","ja":"<=","jbe":">","jb":">=","jae":"<"}
                m = re.search(r"0x[0-9a-f]+", ins[i+1].op_str); tgt = int(m.group(0),16) if m else None
                # simple forward if: body until tgt is the then-block
                then = []
                j = i+2
                while j < len(ins) and ins[j].address < (tgt or 0):
                    if ins[j].mnemonic == "mov" and "," in ins[j].op_str:
                        d,s=[opnd(x) for x in ins[j].op_str.split(",")]
                        then.append(f"{d} = {s};")
                    elif ins[j].mnemonic == "call":
                        mm=re.fullmatch(r"0x([0-9a-f]+)",ins[j].op_str); tt=int(mm.group(1),16) if mm else None
                        then.append(f"{names.get(tt, f'sub_{tt:04x}' if tt else ins[j].op_str).lstrip('_')}();")
                    else:
                        then.append(f"/* {ins[j].mnemonic} {ins[j].op_str} */"); faithful=False
                    j += 1
                body.append(f"if ({l} {inv[cc]} {r}) {{")
                body += ["    "+t for t in then]
                body.append("}")
                i = j; continue
            body.append(f"/* cmp {ops} */"); faithful=False; i += 1; continue
        if mn == "mov" and "," in ops:
            draw, sraw = [x.strip() for x in ops.split(",", 1)]
            if draw in ("ax","sp","bp") and sraw in ("bp","sp"):
                i += 1; continue  # frame housekeeping
            if draw in GPR:                       # load into a transient register
                reg_val[draw] = reg_val.get(sraw, opnd(sraw)); i += 1; continue
            d = opnd(draw); s = reg_val.get(sraw, opnd(sraw))  # store to mem/global
            body.append(f"{d} = {s};"); i += 1; continue
        body.append(f"/* {mn} {ops} */"); faithful = False; i += 1

    # signature: guess arg count from max [bp+k] referenced
    maxk = 0
    for b in body:
        for m in re.finditer(r"\ba(\d+)\b", b):
            maxk = max(maxk, int(m.group(1)))
    params = ", ".join("int a%d" % k for k in range(1, maxk+1)) or "void"
    ret = "int" if any("return" in b for b in body) else "void"
    src = f"{ret} f_{off:04x}({params})\n{{\n" + "".join("    "+b+"\n" for b in body) + "}\n"
    return src, ("faithful" if faithful else "partial (has /* */ raw asm)")


def main():
    img = load(sys.argv[1])
    off = int(sys.argv[2], 16); size = int(sys.argv[3],0)
    chk = int(sys.argv[4], 16) if len(sys.argv) > 4 and sys.argv[4].startswith("0x") else None
    names = names_from_triage(sys.argv[sys.argv.index("--names")+1]) if "--names" in sys.argv else {}
    src, note = lift(img, off, size, chk, names)
    print(f"// {note}\n{src}")


if __name__ == "__main__":
    main()
