"""inline_asm.py -- a 16-bit function as Microsoft C 8 source whose body is one _asm block.

    from codebreaker.decompile import inline_asm
    text, why = inline_asm.whole_body(name, code, start, end, fixups, name_at, result="int2")

A function analyze/origin finds holding an _asm block (INT n, IN/OUT, CLI/STI, the carry -- inside
the BP frame C 8 gives every function with one) was written as C with assembly in it. Its first
form is the whole body as the block: the instructions between the frame C 8 makes and the return
it makes, written back in the inline assembler's words. C 8 then builds the frame round them --
`push bp / mov bp, sp` or `enter n, 0`, `leave`, the return its convention says -- so whether the
text is the function is the compiler's to say, byte for byte (verify/authored/match16.py: the "asm"
way of saying). Measured with C 8.00c (2026-10-28): SUWIN's 2:7E3C (`mov bx, h / mov ah, 68h /
int 21h`) and 8:0538 (INT 2Fh, then C round a second block) come out as the original's bytes.

What the block keeps as the original has it: places in the frame as [bp+k] / [bp-k] (the frame is
declared as many bytes as the original's), the module's data as ds:[n]. What it names: jumps
within the function as labels, calls (a far call's fix-up, a near call's target) by the name the
module's header declares. A function this cannot say (a jump out of it, a return in its middle,
a fix-up it cannot name) is left as the decompiler wrote it: -> (None, why).
"""
import re

import capstone.x86 as X
from codebreaker import platforms

_STRING = {"movsb", "movsw", "movsd", "lodsb", "lodsw", "lodsd", "stosb", "stosw", "stosd", "scasb", "scasw",
           "scasd", "cmpsb", "cmpsw", "cmpsd", "insb", "insw", "outsb", "outsw"}
_REGS32 = {X.X86_REG_EAX, X.X86_REG_EBX, X.X86_REG_ECX, X.X86_REG_EDX, X.X86_REG_ESI, X.X86_REG_EDI,
           X.X86_REG_EBP, X.X86_REG_ESP}
_JUMPS = {"jmp", "je", "jne", "jb", "jae", "jbe", "ja", "jl", "jge", "jle", "jg", "js", "jns", "jo", "jno",
          "jp", "jnp", "jcxz", "loop", "loope", "loopne"}


def _386(i):
    """Is it an instruction the inline assembler does not take -- the 386's, which /G3 makes: a
    32-bit register, an operand-size prefix, a near conditional jump?"""
    regs = [op.reg for op in i.operands if op.type == X.X86_OP_REG] +         [r for op in i.operands if op.type == X.X86_OP_MEM for r in (op.mem.base, op.mem.index)]
    return any(r in _REGS32 for r in regs) or i.bytes[:1] == b"f" or         (i.bytes[:1] == b"" and 0x80 <= i.bytes[1] <= 0x8f)


def _prologue(ins):
    """-> (instructions it takes, frame bytes, loads DS) -- the frame C 8 makes for the function."""
    k, frame, loadds = 0, 0, False
    if ins and ins[0].mnemonic == "enter":
        frame = ins[0].operands[0].imm
        k = 1
    elif len(ins) > 1 and (ins[0].mnemonic, ins[0].op_str, ins[1].mnemonic, ins[1].op_str) == \
            ("push", "bp", "mov", "bp, sp"):
        k = 2
        if len(ins) > 2 and ins[2].mnemonic == "sub" and ins[2].op_str.startswith("sp, "):
            frame = ins[2].operands[1].imm
            k = 3
    else:
        return None
    # __loadds: push ds / mov ax, DGROUP / mov ds, ax (DGROUP a fix-up)
    if len(ins) > k + 2 and ins[k].op_str == "ds" and ins[k].mnemonic == "push" and \
            ins[k + 1].mnemonic == "mov" and ins[k + 1].op_str.startswith("ax, ") and \
            (ins[k + 2].mnemonic, ins[k + 2].op_str) == ("mov", "ds, ax"):
        k += 3
        loadds = True
    return k, frame, loadds


def _epilogue(ins, loadds):
    """-> (instructions it takes, far, bytes it removes) or None."""
    if not ins or ins[-1].mnemonic not in ("ret", "retf"):
        return None
    far = ins[-1].mnemonic == "retf"
    pop = ins[-1].operands[0].imm if ins[-1].operands else 0
    k = 1
    if len(ins) > k and ins[-1 - k].mnemonic == "leave":
        k += 1
    elif len(ins) > k + 1 and (ins[-1 - k].mnemonic, ins[-1 - k].op_str) == ("pop", "bp"):
        k += 1
        if (ins[-1 - k].mnemonic, ins[-1 - k].op_str) == ("mov", "sp, bp"):
            k += 1
    else:
        return None
    if loadds and (ins[-1 - k].mnemonic, ins[-1 - k].op_str) == ("pop", "ds"):
        k += 1
    return k, far, pop


def _operands(i, fixups, name_at, seg):
    """The instruction's operands in the inline assembler's words, or None."""
    s = i.op_str
    if i.mnemonic.split()[-1] in _STRING:
        return ""
    if i.mnemonic in ("les", "lds") and re.search(r"(^|[ ,])ptr ", s):
        s = re.sub(r"(^|[ ,] ?)ptr ", r"\g<1>dword ptr ", s, count=1)
    ops = i.operands
    # a direct memory operand: the module's data (DS: the inline assembler takes [n] for an immediate)
    for op in ops:
        if op.type == X.X86_OP_MEM and op.mem.base == 0 and op.mem.index == 0 and op.mem.segment == 0:
            disp = op.mem.disp & 0xFFFF
            s = s.replace("[0x%x]" % disp, "ds:[0x%x]" % disp)
    return s


def whole_body(name, code, start, end, fixups, name_at, seg, result="int2"):
    """The function at code[start:end] as a C function whose body is one _asm block.
    fixups: {offset in the segment: target name or None}; name_at(seg, off) -> a function's name.
    -> (C text, None) or (None, why)"""
    md = platforms.decoder(16, detail=True)
    ins = []
    for i in md.disasm(bytes(code[start:end]), start):
        ins.append(i)
        if i.mnemonic in ("ret", "retf"):
            break
    if not ins or ins[-1].mnemonic not in ("ret", "retf"):
        return None, "no return found"
    pro = _prologue(ins)
    if pro is None:
        return None, "no frame of C 8's at its start"
    took, frame, loadds = pro
    epi = _epilogue(ins, loadds)
    if epi is None:
        return None, "no frame of C 8's at its end"
    left, far, pop = epi
    body = ins[took:len(ins) - left]
    first, last = ins[0].address, ins[-1].address + ins[-1].size
    targets = set()
    for i in body:
        if _386(i):
            # (the inline assembler takes the 286's instructions: a 32-bit register, an operand-size
            # prefix, a near conditional jump are /G3's code -- C round a block, not the block)
            return None, "386 code, which the compiler made (%04x %s %s)" % (i.address, i.mnemonic, i.op_str)
        if i.mnemonic in _JUMPS:
            if not i.operands or i.operands[0].type != X.X86_OP_IMM:
                return None, "a jump through a register or memory (%04x)" % i.address
            to = i.operands[0].imm & 0xFFFF
            if not first <= to < last:
                return None, "a jump out of the function (%04x)" % i.address
            targets.add(to)
        if i.mnemonic in ("ret", "retf", "iret"):
            return None, "a return in its middle (%04x)" % i.address
    lines = []
    for i in body:
        if i.address in targets:
            lines.append("L_%04x:" % i.address)
        mn = i.mnemonic.split()[-1]                           # (capstone says "rep stosw")
        site = [a for a in range(i.address, i.address + i.size) if a in fixups]
        if mn in _JUMPS:
            lines.append("        %s L_%04x" % (mn, i.operands[0].imm & 0xFFFF))
            continue
        if mn == "lcall" and site:
            target = fixups[site[0]]
            if ("segment", site[0]) in fixups and len(i.operands) == 2:
                # (a selector-only fix-up: the offset is in the instruction -- a far call within the
                # module, its target named by the segment the fix-up gives and that offset)
                target = fixups.get(("segment", site[0]))
                target = name_at(target, i.operands[1].imm & 0xFFFF) if target else None
            if not target:
                return None, "a far call it cannot name (%04x)" % i.address
            lines.append("        call %s" % target)
            continue
        if mn == "call" and i.operands and i.operands[0].type == X.X86_OP_IMM:
            target = name_at(seg, i.operands[0].imm & 0xFFFF)
            if not target:
                return None, "a near call it cannot name (%04x)" % i.address
            lines.append("        call near ptr %s" % target)
            continue
        if site:
            return None, "a fix-up in %s (%04x)" % (mn, i.address)
        if mn in ("lcall", "ljmp"):
            return None, "a far %s without a fix-up (%04x)" % (mn, i.address)
        ops = _operands(i, fixups, name_at, seg)
        prefix = ""
        if i.prefix[0] in (0xF2, 0xF3) and mn in _STRING:
            prefix = {0xF3: "rep " if mn[:4] in ("movs", "stos", "lods", "ins", "outs") else "repe ",
                      0xF2: "repne "}[i.prefix[0]]
        lines.append("        %s%s%s" % (prefix, mn, (" " + ops) if ops else ""))
    for to in sorted(t for t in targets if t >= (body[-1].address + body[-1].size if body else first)):
        lines.append("L_%04x:" % to)                         # (a jump to the frame's end: the block's)
    if pop:
        params = ["a%d" % (k + 1) for k in range(pop // 2)]
        conv = "__pascal"
    else:
        params, conv = [], "__cdecl"
    head = "%s %s %s%s %s(%s)" % (result, "__far" if far else "__near", conv, " __loadds" if loadds else "", name,
                                  ", ".join(params))
    out = [head]
    out += ["int2 %s;" % p for p in params]
    out.append("{")
    if frame:
        out.append("  char codebreaker_frame[%d];" % frame)
    out.append("  _asm {")
    out += lines
    out.append("  }")
    out.append("}")
    return "\n".join(out), None
