"""sm83.py -- the Game Boy's CPU (Sharp SM83, "GBZ80"): the table, a disassembler, an assembler.

    from codebreaker.decompile import sm83
    sm83.decode(code, at, pc)       Insn(pattern, size, values, target, flow) or None
    sm83.text(insn, labels)         "LD A,(HL+)" / "JR NZ,loop" / "LDH ($FF44),A"
    sm83.assemble(line, pc, labels) the bytes of one instruction line

The SM83 runs most of the Z80's 8080 subset with a different map: no IX/IY, no ED page, no
alternate registers, no parity flag, only the conditions NZ Z NC C; and its own instructions in
the slots that frees: LD (nn),SP, STOP, LD (HL+)/(HL-), LDH (n) and (C) for the $FF00 page, ADD
SP,e, LD HL,SP+e, RETI at $D9, and SWAP in place of SLL on the CB page. Eleven opcodes do not
exist ($D3 $DB $DD $E3 $E4 $EB $EC $ED $F4 $FC $FD). The table is generated from the opcode
fields as decompile/z80.py's is, and serves the disassembler, the assembler and the
interpreter (emulate/sm83.py).

Syntax is RGBDS's (the Game Boy's usual assembler): `LD A,(HL+)`, `LDH ($FF44),A` -- an LDH
operand is written as the full address, $FF00 + n --, `LD HL,SP+$05`, `STOP`. Operands taken as
values: n (byte), nn (word), e (relative jump, written as its target), s (signed byte of ADD SP
and LD HL,SP+), h (LDH's byte, written $FFxx).
"""
import re

from codebreaker.decompile.z80 import value, _split_operands, _balanced

R = ["B", "C", "D", "E", "H", "L", "(HL)", "A"]
RP = ["BC", "DE", "HL", "SP"]
RP2 = ["BC", "DE", "HL", "AF"]
CC = ["NZ", "Z", "NC", "C"]
ALU = ["ADD A,", "ADC A,", "SUB ", "SBC A,", "AND ", "XOR ", "OR ", "CP "]
ROT = ["RLC", "RRC", "RL", "RR", "SLA", "SRA", "SWAP", "SRL"]
ILLEGAL = {0xD3, 0xDB, 0xDD, 0xE3, 0xE4, 0xEB, 0xEC, 0xED, 0xF4, 0xFC, 0xFD}


def _main(op):
    x, y, z, p, q = op >> 6, (op >> 3) & 7, op & 7, (op >> 4) & 3, (op >> 3) & 1
    if op in ILLEGAL:
        return None
    if x == 0:
        if z == 0:
            return ["NOP", "LD (nn),SP", "STOP", "JR e", "JR NZ,e", "JR Z,e", "JR NC,e", "JR C,e"][y]
        if z == 1:
            return "ADD HL,%s" % RP[p] if q else "LD %s,nn" % RP[p]
        if z == 2:
            return [["LD (BC),A", "LD (DE),A", "LD (HL+),A", "LD (HL-),A"],
                    ["LD A,(BC)", "LD A,(DE)", "LD A,(HL+)", "LD A,(HL-)"]][q][p]
        if z == 3:
            return ("DEC %s" if q else "INC %s") % RP[p]
        if z == 4:
            return "INC %s" % R[y]
        if z == 5:
            return "DEC %s" % R[y]
        if z == 6:
            return "LD %s,n" % R[y]
        return ["RLCA", "RRCA", "RLA", "RRA", "DAA", "CPL", "SCF", "CCF"][y]
    if x == 1:
        return "HALT" if op == 0x76 else "LD %s,%s" % (R[y], R[z])
    if x == 2:
        return ALU[y] + R[z]
    if z == 0:
        return ["RET NZ", "RET Z", "RET NC", "RET C", "LDH (h),A", "ADD SP,s", "LDH A,(h)", "LD HL,SP+s"][y]
    if z == 1:
        return "POP %s" % RP2[p] if not q else ["RET", "RETI", "JP HL", "LD SP,HL"][p]
    if z == 2:
        return ["JP NZ,nn", "JP Z,nn", "JP NC,nn", "JP C,nn", "LD (C),A", "LD (nn),A", "LD A,(C)", "LD A,(nn)"][y]
    if z == 3:
        return {0: "JP nn", 6: "DI", 7: "EI"}.get(y)
    if z == 4:
        return "CALL %s,nn" % CC[y] if y < 4 else None
    if z == 5:
        return "PUSH %s" % RP2[p] if not q else ("CALL nn" if p == 0 else None)
    if z == 6:
        return ALU[y] + "n"
    return "RST $%02X" % (y * 8)


def _cb(op):
    x, y, z = op >> 6, (op >> 3) & 7, op & 7
    if x == 0:
        return "%s %s" % (ROT[y], R[z])
    return "%s %d,%s" % (["", "BIT", "RES", "SET"][x], y, R[z])


TABLE = {}
for _op in range(256):
    if _op != 0xCB:
        TABLE[(b"", _op)] = _main(_op)
    TABLE[(b"\xCB", _op)] = _cb(_op)
_PLACE = {"n": 1, "nn": 2, "e": 1, "s": 1, "h": 1}


class Insn(object):
    __slots__ = ("pattern", "size", "values", "target", "flow", "pc", "bytes")

    def __init__(self, pattern, size, values, target, flow, pc, raw):
        self.pattern, self.size, self.values, self.target = pattern, size, values, target
        self.flow, self.pc, self.bytes = flow, pc, raw

    @property
    def text(self):
        return text(self)

    def __repr__(self):
        return "Insn(%s)" % text(self)


def _flow(pattern):
    m = pattern.split(" ")[0]
    if m in ("JP", "JR"):
        if pattern == "JP HL":
            return "indirect"
        return "branch" if "," in pattern else "jump"
    if m == "CALL":
        return "ccall" if "," in pattern else "call"
    if m == "RST":
        return "call"
    if m == "RET":
        return "cret" if " " in pattern else "ret"
    if m == "RETI":
        return "ret"
    if m in ("HALT", "STOP"):
        return "halt"
    return ""


def _fields(pattern):
    return re.findall(r"\b(nn|n|e|s|h)\b", pattern)


def decode(code, at, pc):
    """The instruction at code[at:] (it runs at address `pc`), None if code ends inside it or the
    opcode does not exist."""
    if at >= len(code):
        return None
    prefix, i = b"", at
    if code[i] == 0xCB:
        prefix, i = b"\xCB", i + 1
        if i >= len(code):
            return None
    pattern = TABLE.get((prefix, code[i]))
    if pattern is None:
        return None
    i += 1
    if pattern == "STOP":                  # STOP is two bytes ($10 $00)
        i += 1
    values = []
    for f in _fields(pattern):
        size = _PLACE[f]
        if i + size > len(code):
            return None
        v = code[i] if size == 1 else code[i] | code[i + 1] << 8
        if f in ("e", "s") and v >= 128:
            v -= 256
        if f == "h":
            v += 0xFF00
        values.append(v)
        i += size
    if i > len(code):
        return None
    size = i - at
    flow = _flow(pattern)
    target = None
    if "e" in _fields(pattern):
        target = (pc + size + values[-1]) & 0xFFFF
        values[-1] = target
    elif pattern.startswith("RST"):
        target = int(pattern.split("$")[1], 16)
    elif flow in ("jump", "branch", "call", "ccall"):
        target = values[-1]
    return Insn(pattern, size, values, target, flow, pc, bytes(code[at:i]))


def text(insn, labels=None):
    labels = labels or {}
    if insn.pattern == "STOP" and insn.bytes[1:2] not in (b"\x00", b""):
        return "DB $10,$%02X" % insn.bytes[1]     # STOP with a second byte assemblers do not write
    out, k = [], 0
    for part in re.split(r"(\bnn\b|\bn\b|\be\b|\bs\b|\bh\b)", insn.pattern):
        if part in _PLACE:
            v = insn.values[k]
            k += 1
            if part == "e" or (part == "nn" and v in labels):
                out.append(labels.get(v, "$%04X" % v))
            elif part in ("nn", "h"):
                out.append(labels.get(v, "$%04X" % v) if part == "h" and v in labels else "$%04X" % v)
            elif part == "s":
                out.append("$%02X" % v if v >= 0 else "-$%02X" % -v)
            else:
                out.append("$%02X" % v)
        else:
            out.append(part)
    return "".join(out).replace("SP+-", "SP-")


_ASM = None
_WORDS = set(R + RP + RP2 + CC + ["(BC)", "(DE)", "(C)", "(HL+)", "(HL-)", "(HLI)", "(HLD)", "HL"])


def assemble(line, pc=0, labels=None):
    """The bytes of one instruction (RGBDS syntax)."""
    global _ASM
    if _ASM is None:
        _ASM = {}
        for (prefix, op), pattern in TABLE.items():
            if pattern:
                _ASM.setdefault(pattern, prefix + bytes([op]))
    labels = {} if labels is None else labels
    mnemonic, _, rest = line.strip().partition(" ")
    mnemonic = mnemonic.upper()
    ops = _split_operands(rest) if rest.strip() else []
    shape, exprs = [], []
    for o in ops:
        u = re.sub(r"\s+", "", o).upper().replace("(HLI)", "(HL+)").replace("(HLD)", "(HL-)")
        if u in _WORDS:
            shape.append(u)
            continue
        m = re.match(r"^SP([+-].+)$", re.sub(r"\s+", "", o), re.I)
        if m and mnemonic == "LD":
            shape.append("SP+?")
            exprs.append(m.group(1))
            continue
        if mnemonic == "RST":
            shape.append("$%02X" % value(o, labels, pc))
            continue
        if mnemonic in ("BIT", "SET", "RES") and not shape:
            shape.append(str(value(o, labels, pc)))
            continue
        if o.startswith("(") and o.endswith(")") and _balanced(o[1:-1]):
            shape.append("(?)")
            exprs.append(o[1:-1])
        else:
            shape.append("?")
            exprs.append(o)
    count = sum(s.count("?") for s in shape)
    for kinds in _choices(count):
        k = iter(kinds)
        pattern = mnemonic + (" " + ",".join(s.replace("?", next(k)) if "?" in s else s for s in shape) if shape else "")
        if pattern in _ASM:
            break
    else:
        raise ValueError("not an instruction: %r" % line)
    code = bytearray(_ASM[pattern])
    if pattern == "STOP":
        code.append(0)
    fields = _fields(pattern)
    size = len(code) + sum(_PLACE[f] for f in fields)
    for f, e in zip(fields, exprs):
        v = value(e, labels, pc)
        if f == "e":
            v -= pc + size
            if not -128 <= v <= 127:
                raise ValueError("relative jump out of range: %r" % line)
            code.append(v & 0xFF)
        elif f == "h":
            if not 0xFF00 <= v <= 0xFFFF:
                raise ValueError("LDH address outside $FF00-$FFFF: %r" % line)
            code.append(v & 0xFF)
        elif f in ("n", "s"):
            code.append(v & 0xFF)
        else:
            code += bytes([v & 0xFF, (v >> 8) & 0xFF])
    return bytes(code)


def _choices(count):
    if count == 0:
        yield []
        return
    for first in ("n", "nn", "e", "s", "h"):
        for rest in _choices(count - 1):
            yield [first] + rest
