"""m6502.py -- the 6502's instructions: the table, a disassembler, an assembler.

    from codebreaker.decompile import m6502
    m6502.decode(code, at, pc)      Insn(mnemonic, mode, operand, size, target) or None
    m6502.text(insn, labels)        "LDA $0200,X" / "BNE loop" (ca65 syntax)
    m6502.assemble(line, pc, labels)   the bytes of one ca65 instruction line

The 151 documented opcodes of the NMOS 6502 -- the NES's 2A03 runs the same set (its decimal
mode is wired off). One table serves the disassembler, the assembler that rebuilds the source
the NES lane writes (source/nes_writer.py), and the interpreter (emulate/m6502.py).

Addressing modes, and their operand size:

    imp  (none)        acc  A            imm  #$nn        zp   $nn         zpx  $nn,X
    zpy  $nn,Y         abs  $nnnn        abx  $nnnn,X     aby  $nnnn,Y     ind  ($nnnn)
    izx  ($nn,X)       izy  ($nn),Y      rel  a branch: one signed byte, from the next instruction
"""
import re

SIZE = {"imp": 1, "acc": 1, "imm": 2, "zp": 2, "zpx": 2, "zpy": 2, "izx": 2, "izy": 2, "rel": 2,
        "abs": 3, "abx": 3, "aby": 3, "ind": 3}

# mnemonic -> {mode: opcode}
_GROUPS = {
    "ADC": {"imm": 0x69, "zp": 0x65, "zpx": 0x75, "abs": 0x6D, "abx": 0x7D, "aby": 0x79, "izx": 0x61, "izy": 0x71},
    "AND": {"imm": 0x29, "zp": 0x25, "zpx": 0x35, "abs": 0x2D, "abx": 0x3D, "aby": 0x39, "izx": 0x21, "izy": 0x31},
    "ASL": {"acc": 0x0A, "zp": 0x06, "zpx": 0x16, "abs": 0x0E, "abx": 0x1E},
    "BCC": {"rel": 0x90}, "BCS": {"rel": 0xB0}, "BEQ": {"rel": 0xF0}, "BMI": {"rel": 0x30},
    "BNE": {"rel": 0xD0}, "BPL": {"rel": 0x10}, "BVC": {"rel": 0x50}, "BVS": {"rel": 0x70},
    "BIT": {"zp": 0x24, "abs": 0x2C},
    "BRK": {"imp": 0x00},
    "CLC": {"imp": 0x18}, "CLD": {"imp": 0xD8}, "CLI": {"imp": 0x58}, "CLV": {"imp": 0xB8},
    "CMP": {"imm": 0xC9, "zp": 0xC5, "zpx": 0xD5, "abs": 0xCD, "abx": 0xDD, "aby": 0xD9, "izx": 0xC1, "izy": 0xD1},
    "CPX": {"imm": 0xE0, "zp": 0xE4, "abs": 0xEC},
    "CPY": {"imm": 0xC0, "zp": 0xC4, "abs": 0xCC},
    "DEC": {"zp": 0xC6, "zpx": 0xD6, "abs": 0xCE, "abx": 0xDE},
    "DEX": {"imp": 0xCA}, "DEY": {"imp": 0x88},
    "EOR": {"imm": 0x49, "zp": 0x45, "zpx": 0x55, "abs": 0x4D, "abx": 0x5D, "aby": 0x59, "izx": 0x41, "izy": 0x51},
    "INC": {"zp": 0xE6, "zpx": 0xF6, "abs": 0xEE, "abx": 0xFE},
    "INX": {"imp": 0xE8}, "INY": {"imp": 0xC8},
    "JMP": {"abs": 0x4C, "ind": 0x6C},
    "JSR": {"abs": 0x20},
    "LDA": {"imm": 0xA9, "zp": 0xA5, "zpx": 0xB5, "abs": 0xAD, "abx": 0xBD, "aby": 0xB9, "izx": 0xA1, "izy": 0xB1},
    "LDX": {"imm": 0xA2, "zp": 0xA6, "zpy": 0xB6, "abs": 0xAE, "aby": 0xBE},
    "LDY": {"imm": 0xA0, "zp": 0xA4, "zpx": 0xB4, "abs": 0xAC, "abx": 0xBC},
    "LSR": {"acc": 0x4A, "zp": 0x46, "zpx": 0x56, "abs": 0x4E, "abx": 0x5E},
    "NOP": {"imp": 0xEA},
    "ORA": {"imm": 0x09, "zp": 0x05, "zpx": 0x15, "abs": 0x0D, "abx": 0x1D, "aby": 0x19, "izx": 0x01, "izy": 0x11},
    "PHA": {"imp": 0x48}, "PHP": {"imp": 0x08}, "PLA": {"imp": 0x68}, "PLP": {"imp": 0x28},
    "ROL": {"acc": 0x2A, "zp": 0x26, "zpx": 0x36, "abs": 0x2E, "abx": 0x3E},
    "ROR": {"acc": 0x6A, "zp": 0x66, "zpx": 0x76, "abs": 0x6E, "abx": 0x7E},
    "RTI": {"imp": 0x40}, "RTS": {"imp": 0x60},
    "SBC": {"imm": 0xE9, "zp": 0xE5, "zpx": 0xF5, "abs": 0xED, "abx": 0xFD, "aby": 0xF9, "izx": 0xE1, "izy": 0xF1},
    "SEC": {"imp": 0x38}, "SED": {"imp": 0xF8}, "SEI": {"imp": 0x78},
    "STA": {"zp": 0x85, "zpx": 0x95, "abs": 0x8D, "abx": 0x9D, "aby": 0x99, "izx": 0x81, "izy": 0x91},
    "STX": {"zp": 0x86, "zpy": 0x96, "abs": 0x8E},
    "STY": {"zp": 0x84, "zpx": 0x94, "abs": 0x8C},
    "TAX": {"imp": 0xAA}, "TAY": {"imp": 0xA8}, "TSX": {"imp": 0xBA}, "TXA": {"imp": 0x8A},
    "TXS": {"imp": 0x9A}, "TYA": {"imp": 0x98},
}
OPCODES = {op: (m, mode) for m, modes in _GROUPS.items() for mode, op in modes.items()}
BRANCHES = {"BCC", "BCS", "BEQ", "BMI", "BNE", "BPL", "BVC", "BVS"}
ENDS = {"JMP", "RTS", "RTI", "BRK"}               # flow does not fall through


class Insn(object):
    __slots__ = ("at", "op", "mnemonic", "mode", "operand", "size", "target")

    def __init__(self, at, op, mnemonic, mode, operand, size, target):
        self.at, self.op, self.mnemonic, self.mode = at, op, mnemonic, mode
        self.operand, self.size, self.target = operand, size, target

    def __repr__(self):
        return "<%04X %s>" % (self.at, text(self))


def decode(code, offset, pc):
    """The instruction at code[offset], which sits at address `pc`; None for an undocumented
    opcode or one cut off by the end of `code`."""
    if offset >= len(code) or code[offset] not in OPCODES:
        return None
    op = code[offset]
    mnemonic, mode = OPCODES[op]
    size = SIZE[mode]
    if offset + size > len(code):
        return None
    operand = code[offset + 1] if size == 2 else (code[offset + 1] | code[offset + 2] << 8) if size == 3 else None
    target = None
    if mode == "rel":
        target = (pc + 2 + (operand - 256 if operand & 0x80 else operand)) & 0xFFFF
    elif mnemonic in ("JMP", "JSR") and mode == "abs":
        target = operand
    return Insn(pc, op, mnemonic, mode, operand, size, target)


def text(insn, labels=None):
    """The instruction in ca65 syntax; an address that has a label is written as the label.
    (A zero-page address is written with two hex digits, an absolute one with four: ca65 picks
    the instruction's size from how the address is written, so the bytes come back the same.)"""
    labels = labels or {}
    m, mode, v = insn.mnemonic, insn.mode, insn.operand

    def addr(value, width):
        name = labels.get(value)
        if name:
            return ("a:" + name) if width == 4 and value < 0x100 else name
        return "$%0*X" % (width, value)
    if mode == "imp":
        return m
    if mode == "acc":
        return m + " a"
    if mode == "imm":
        return "%s #$%02X" % (m, v)
    if mode == "rel":
        return "%s %s" % (m, addr(insn.target, 4))
    form = {"zp": "%s", "zpx": "%s,x", "zpy": "%s,y", "abs": "%s", "abx": "%s,x", "aby": "%s,y",
            "ind": "(%s)", "izx": "(%s,x)", "izy": "(%s),y"}[mode]
    return "%s %s" % (m, form % addr(v, 2 if mode in ("zp", "zpx", "zpy", "izx", "izy") else 4))


_LINE = re.compile(r"^\s*([A-Za-z]{3})(?:\s+(.*?))?\s*(?:;.*)?$")


def assemble(line, pc, labels=None):
    """The bytes of one instruction written as text() writes it. Labels are resolved from
    `labels` ({name: address}); `a:` forces an absolute address."""
    labels = labels or {}
    m = _LINE.match(line)
    if not m:
        raise ValueError("not an instruction: %r" % line)
    mnemonic, arg = m.group(1).upper(), (m.group(2) or "").strip()
    modes = _GROUPS.get(mnemonic)
    if modes is None:
        raise ValueError("unknown instruction %s" % mnemonic)

    def value(s):
        s = s.strip()
        wide = s.startswith("a:")
        s = s[2:] if wide else s
        if s.startswith("$"):
            return int(s[1:], 16), wide or len(s) > 3
        if s in labels:
            return labels[s], wide or labels[s] > 0xFF
        return int(s, 0), wide or int(s, 0) > 0xFF
    if not arg:
        mode, operand = "imp", None
    elif arg.lower() == "a":
        mode, operand = "acc", None
    elif arg.startswith("#"):
        mode, operand = "imm", value(arg[1:].replace("<", "").replace(">", ""))[0]
    elif mnemonic in BRANCHES:
        target = value(arg)[0]
        d = target - (pc + 2)
        if not -128 <= d <= 127:
            raise ValueError("branch out of reach: %s" % line)
        mode, operand = "rel", d & 0xFF
    else:
        low = arg.lower().replace(" ", "")
        if low.startswith("(") and low.endswith(",x)"):
            mode, operand = "izx", value(arg[1:arg.lower().rindex(",x")])[0]
        elif low.startswith("(") and low.endswith("),y"):
            mode, operand = "izy", value(arg[1:arg.index(")")])[0]
        elif low.startswith("("):
            mode, operand = "ind", value(arg[1:arg.rindex(")")])[0]
        else:
            index = low[-2:] if low.endswith((",x", ",y")) else ""
            v, wide = value(arg[:-2] if index else arg)
            if not wide and ({"": "zp", ",x": "zpx", ",y": "zpy"}[index]) in modes:
                mode = {"": "zp", ",x": "zpx", ",y": "zpy"}[index]
            else:
                mode = {"": "abs", ",x": "abx", ",y": "aby"}[index]
            operand = v
    if mode not in modes:
        raise ValueError("%s has no %s form" % (mnemonic, mode))
    size = SIZE[mode]
    out = bytes([modes[mode]])
    if size == 2:
        out += bytes([operand & 0xFF])
    elif size == 3:
        out += bytes([operand & 0xFF, operand >> 8 & 0xFF])
    return out
