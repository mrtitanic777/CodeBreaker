"""z80.py -- the Z80's instructions: the table, a disassembler, an assembler.

    from codebreaker.decompile import z80
    z80.decode(code, at, pc)        Insn(text, size, target, flow, operands) or None
    z80.text(insn, labels)          "LD A,(IX+5)" / "JR NZ,loop" / "CALL sub_0150"
    z80.assemble(line, pc, labels)  the bytes of one instruction line

Every opcode of the Z80, documented and not: the unprefixed page, CB (rotates and bits), ED
(the rest), and the DD/FD pages that put IX/IY (and their halves IXH IXL IYH IYL) in place of HL
(H L), with DDCB/FDCB for the indexed bit operations. The table is generated from the way the
CPU decodes an opcode (its fields x y z p q, as in "Decoding Z80 opcodes", z80.info), so the
disassembler, the assembler and the interpreter (emulate/z80.py) read one description.

Syntax is Zilog's, as assemblers for the Master System, Game Gear, MSX and Spectrum take it
(WLA-DX, sjasmplus, pasmo): `LD A,(IX+5)`, `JP (HL)`, `EX AF,AF'`, `RST $38`, `IM 1`. Numbers
are written `$1F`. Operands an assembler may take as a value:

    n   a byte          nn  a word         d   an index displacement, -128..127
    e   a relative jump: a signed byte from the next instruction (written as its target)
"""
import re

R = ["B", "C", "D", "E", "H", "L", "(HL)", "A"]
RP = ["BC", "DE", "HL", "SP"]
RP2 = ["BC", "DE", "HL", "AF"]
CC = ["NZ", "Z", "NC", "C", "PO", "PE", "P", "M"]
ALU = ["ADD A,", "ADC A,", "SUB ", "SBC A,", "AND ", "XOR ", "OR ", "CP "]
ROT = ["RLC", "RRC", "RL", "RR", "SLA", "SRA", "SLL", "SRL"]
IM = ["0", "0", "1", "2", "0", "0", "1", "2"]
BLOCK = {(4, 0): "LDI", (4, 1): "CPI", (4, 2): "INI", (4, 3): "OUTI",
         (5, 0): "LDD", (5, 1): "CPD", (5, 2): "IND", (5, 3): "OUTD",
         (6, 0): "LDIR", (6, 1): "CPIR", (6, 2): "INIR", (6, 3): "OTIR",
         (7, 0): "LDDR", (7, 1): "CPDR", (7, 2): "INDR", (7, 3): "OTDR"}


def _main(op):
    """The unprefixed opcode `op` as a pattern ("LD B,n", "JR NZ,e", ...)."""
    x, y, z, p, q = op >> 6, (op >> 3) & 7, op & 7, (op >> 4) & 3, (op >> 3) & 1
    if x == 0:
        if z == 0:
            return ["NOP", "EX AF,AF'", "DJNZ e", "JR e", "JR NZ,e", "JR Z,e", "JR NC,e", "JR C,e"][y]
        if z == 1:
            return "ADD HL,%s" % RP[p] if q else "LD %s,nn" % RP[p]
        if z == 2:
            return [["LD (BC),A", "LD (DE),A", "LD (nn),HL", "LD (nn),A"],
                    ["LD A,(BC)", "LD A,(DE)", "LD HL,(nn)", "LD A,(nn)"]][q][p]
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
        return "RET %s" % CC[y]
    if z == 1:
        return "POP %s" % RP2[p] if not q else ["RET", "EXX", "JP (HL)", "LD SP,HL"][p]
    if z == 2:
        return "JP %s,nn" % CC[y]
    if z == 3:
        return ["JP nn", None, "OUT (n),A", "IN A,(n)", "EX (SP),HL", "EX DE,HL", "DI", "EI"][y]
    if z == 4:
        return "CALL %s,nn" % CC[y]
    if z == 5:
        return "PUSH %s" % RP2[p] if not q else ["CALL nn", None, None, None][p]
    if z == 6:
        return ALU[y] + "n"
    return "RST $%02X" % (y * 8)


def _cb(op, target="(HL)"):
    x, y, z = op >> 6, (op >> 3) & 7, op & 7
    reg = R[z] if target == "(HL)" else target
    if x == 0:
        return "%s %s" % (ROT[y], reg)
    return "%s %d,%s" % (["", "BIT", "RES", "SET"][x], y, reg)


def _ed(op):
    x, y, z, p, q = op >> 6, (op >> 3) & 7, op & 7, (op >> 4) & 3, (op >> 3) & 1
    if x == 1:
        if z == 0:
            return "IN (C)" if y == 6 else "IN %s,(C)" % R[y]
        if z == 1:
            return "OUT (C),0" if y == 6 else "OUT (C),%s" % R[y]
        if z == 2:
            return ("ADC HL,%s" if q else "SBC HL,%s") % RP[p]
        if z == 3:
            return "LD %s,(nn)" % RP[p] if q else "LD (nn),%s" % RP[p]
        if z == 4:
            return "NEG"
        if z == 5:
            return "RETI" if y == 1 else "RETN"
        if z == 6:
            return "IM %s" % IM[y]
        return ["LD I,A", "LD R,A", "LD A,I", "LD A,R", "RRD", "RLD", "NOP", "NOP"][y]
    if x == 2 and z <= 3 and y >= 4:
        return BLOCK[(y, z)]
    return None     # an ED opcode that does nothing (a two-byte NOP)


def _index(pattern, ix):
    """An unprefixed pattern as the DD/FD page has it: HL -> IX, H/L -> IXH/IXL, (HL) -> (IX+d) (and
    then H/L stay H/L), or None when the prefix changes nothing (the opcode runs as unprefixed)."""
    if pattern is None or pattern in ("EX DE,HL", "EXX") or pattern.startswith("RST"):
        return None
    words = re.split(r"([ ,()+])", pattern)
    if pattern == "JP (HL)":                     # a jump to HL's value, not through memory
        return "JP (%s)" % ix
    if "(HL)" in pattern:
        out = pattern.replace("(HL)", "(%s+d)" % ix)
    elif "HL" in words:
        out = "".join(ix if w == "HL" else w for w in words)
    elif "H" in words or "L" in words:
        out = "".join(ix + w if w in ("H", "L") else w for w in words)
    else:
        return None
    return out


# ---- the table: (prefix bytes, opcode) -> pattern ----------------------------------------------

def _build():
    table = {}
    for op in range(256):
        table[(b"\xCB", op)] = _cb(op)
        table[(b"\xED", op)] = _ed(op)
        for prefix, ix in ((b"\xDD", "IX"), (b"\xFD", "IY")):
            if op not in (0xCB, 0xDD, 0xED, 0xFD):
                pat = _index(_main(op), ix)
                if pat:
                    table[(prefix, op)] = pat
            # DDCB d op: the operation on (IX+d); the undocumented forms also copy the result to a register
            z = op & 7
            base = _cb(op, "(%s+d)" % ix)
            if z != 6 and (op >> 6) != 1:
                base += ",%s" % R[z]
            table[(prefix + b"\xCB", op)] = base
        if op not in (0xCB, 0xDD, 0xED, 0xFD):
            table[(b"", op)] = _main(op)
    return table


TABLE = _build()
# placeholder sizes
_PLACE = {"n": 1, "nn": 2, "d": 1, "e": 1}


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
    """How control leaves the instruction: "jump" "branch" "call" "ccall" "ret" "cret" "indirect"
    "halt" or "" (falls through)."""
    m = pattern.split(" ")[0]
    if m in ("JP", "JR"):
        if "(" in pattern:
            return "indirect"
        return "branch" if "," in pattern else "jump"
    if m == "DJNZ":
        return "branch"
    if m == "CALL":
        return "ccall" if "," in pattern else "call"
    if m == "RST":
        return "call"
    if m in ("RET",):
        return "cret" if " " in pattern else "ret"
    if m in ("RETI", "RETN"):
        return "ret"
    if m == "HALT":
        return "halt"
    return ""


def _fields(pattern):
    return re.findall(r"\b(nn|n|d|e)\b", pattern)


def decode(code, at, pc):
    """The instruction at code[at:] (it runs at address `pc`), or None if code ends inside it."""
    prefix = b""
    i = at
    while i < len(code) and code[i] in (0xDD, 0xFD) and prefix[-1:] != b"\xED":
        if prefix in (b"\xDD", b"\xFD"):
            # a prefix after a prefix: the first one does nothing (a one-byte instruction of its own)
            return Insn("NOP", 1, [], None, "", pc, bytes(code[at:at + 1]))
        prefix = bytes([code[i]])
        i += 1
    if i >= len(code):
        return None
    op = code[i]
    if op in (0xCB, 0xED) and not prefix:
        prefix, i = bytes([op]), i + 1
        if i >= len(code):
            return None
        op = code[i]
    elif op == 0xCB and prefix:
        # DDCB d op: the displacement comes before the opcode
        if i + 2 >= len(code):
            return None
        d, op = code[i + 1], code[i + 2]
        pattern = TABLE[(prefix + b"\xCB", op)]
        values = [d - 256 if d >= 128 else d]
        return Insn(pattern, i + 3 - at, values, None, "", pc, bytes(code[at:i + 3]))
    elif op == 0xED and prefix:
        return Insn("NOP", 1, [], None, "", pc, bytes(code[at:at + 1]))   # DD ED: the DD is ignored
    pattern = TABLE.get((prefix, op))
    if pattern is None and prefix in (b"\xDD", b"\xFD"):
        return Insn("NOP", 1, [], None, "", pc, bytes(code[at:at + 1]))   # DD before an unindexed op
    if pattern is None:
        pattern = "NOP" if prefix == b"\xED" else None
        if pattern is None:
            return None
        return Insn(".db $ED,$%02X" % op, i + 1 - at, [], None, "", pc, bytes(code[at:i + 1]))
    i += 1
    values = []
    for f in _fields(pattern):
        size = _PLACE[f]
        if i + size > len(code):
            return None
        v = code[i] if size == 1 else code[i] | code[i + 1] << 8
        if f in ("d", "e") and v >= 128:
            v -= 256
        values.append(v)
        i += size
    size = i - at
    flow = _flow(pattern)
    target = None
    if "e" in _fields(pattern):
        target = (pc + size + values[-1]) & 0xFFFF
        values[-1] = target
    elif flow in ("jump", "branch", "call", "ccall") and pattern.startswith("RST"):
        target = int(pattern.split("$")[1], 16)
    elif flow in ("jump", "branch", "call", "ccall"):
        target = values[-1]
    return Insn(pattern, size, values, target, flow, pc, bytes(code[at:i]))


def text(insn, labels=None):
    """The instruction as source; `labels` {address: name} names jump targets and absolute words."""
    labels = labels or {}
    out, k = [], 0
    for part in re.split(r"(\bnn\b|\bn\b|\bd\b|\be\b)", insn.pattern):
        if part in _PLACE:
            v = insn.values[k]
            k += 1
            if part == "e" or (part == "nn" and v in labels):
                out.append(labels.get(v, "$%04X" % v))
            elif part == "nn":
                out.append("$%04X" % v)
            elif part == "d":
                out.append(("+$%02X" % v) if v >= 0 else ("-$%02X" % -v))
            else:
                out.append("$%02X" % v)
        else:
            out.append(part)
    s = "".join(out).replace("++", "+").replace("+-", "-")
    return s


# ---- the assembler -------------------------------------------------------------------------------

def _patterns():
    """{normalised pattern: (prefix + opcode bytes, displacement-first?)} for the assembler."""
    out = {}
    for (prefix, op), pattern in TABLE.items():
        if not pattern or pattern.startswith(".db"):
            continue
        key = pattern
        if prefix in (b"\xDD\xCB", b"\xFD\xCB"):
            out.setdefault(key, (prefix, op, True))
        else:
            out.setdefault(key, (prefix + bytes([op]), None, False))
    return out


_ASM = None
_NUMBER = re.compile(r"^(\$[0-9A-Fa-f]+|0x[0-9A-Fa-f]+|[0-9][0-9A-Fa-f]*[hH]|%[01]+|[0-9]+)$")
_WORDS = set(R + RP + RP2 + CC + ["IX", "IY", "IXH", "IXL", "IYH", "IYL", "I", "R", "AF'", "(C)", "(SP)",
                                  "(BC)", "(DE)", "(IX)", "(IY)"])


def value(expr, labels, pc=0):
    """A numeric expression: numbers ($1F 1Fh %0101 31 'c'), labels, $ (here), + - * / and parentheses."""
    expr = expr.strip()
    tokens = re.findall(r"\$[0-9A-Fa-f]+|0x[0-9A-Fa-f]+|[0-9][0-9A-Fa-f]*[hH]\b|%[01]+|[0-9]+|'.'|"
                        r"[A-Za-z_.@][\w.@]*|\$|<<|>>|[-+*/()&|^~<>]", expr)
    py = []
    for t in tokens:
        if t == "$":
            py.append(str(pc))
        elif t.startswith("$"):
            py.append(str(int(t[1:], 16)))
        elif t.lower().startswith("0x"):
            py.append(str(int(t, 16)))
        elif t[0].isdigit() and t[-1] in "hH":
            py.append(str(int(t[:-1], 16)))
        elif t.startswith("%") and len(t) > 1:
            py.append(str(int(t[1:], 2)))
        elif t.startswith("'"):
            py.append(str(ord(t[1])))
        elif t[0].isdigit():
            py.append(t)
        elif re.match(r"[A-Za-z_.@]", t):
            if t not in labels:
                raise KeyError(t)
            py.append(str(labels[t]))
        elif t == "<":
            py.append("<")
        elif t == ">":
            py.append(">")
        else:
            py.append(t.replace("/", "//"))
    if not py:
        raise ValueError(expr)
    if py[0] in ("<", ">"):                      # <x low byte, >x high byte (WLA-DX)
        v = int(eval("".join(py[1:]), {"__builtins__": {}}))
        return (v & 0xFF) if py[0] == "<" else (v >> 8) & 0xFF
    return int(eval("".join(py), {"__builtins__": {}}))


def _split_operands(s):
    out, depth, cur = [], 0, ""
    for c in s:
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
        if c == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += c
    if cur.strip():
        out.append(cur.strip())
    return out


def assemble(line, pc=0, labels=None):
    """The bytes of one instruction (`labels` {name: address}); KeyError for an unknown label,
    ValueError for something that is not an instruction."""
    global _ASM
    if _ASM is None:
        _ASM = _patterns()
    labels = {} if labels is None else labels
    line = line.strip()
    mnemonic, _, rest = line.partition(" ")
    mnemonic = mnemonic.upper()
    ops = _split_operands(rest) if rest.strip() else []
    shape, exprs = [], []
    if mnemonic == "OUT" and [re.sub(r"\s+", "", o).upper() for o in ops] == ["(C)", "0"]:
        return bytes([0xED, 0x71])               # OUT (C),0 (undocumented): 0 is part of the instruction
    for o in ops:
        u = re.sub(r"\s+", "", o).upper()
        if u in _WORDS or u == "AF'":
            shape.append(u)
            continue
        m = re.match(r"^\((I[XY])([+-].*)\)$", re.sub(r"\s+", "", o), re.I)
        if m:
            shape.append("(%s+d)" % m.group(1).upper())
            exprs.append(("d", m.group(2)))
            continue
        if mnemonic == "RST":
            shape.append("$%02X" % value(o, labels, pc))
            continue
        if mnemonic == "IM" or (mnemonic in ("BIT", "SET", "RES") and not shape):
            shape.append(str(value(o, labels, pc)))
            continue
        if o.startswith("(") and o.endswith(")") and _balanced(o[1:-1]):
            shape.append("(?)")
            exprs.append(("?", o[1:-1]))
        else:
            shape.append("?")
            exprs.append(("?", o))
    # try each way the value operands can be read
    for kinds in _choices(len([s for s in shape if "?" in s])):
        k = iter(kinds)
        candidate = []
        for s in shape:
            candidate.append(s.replace("?", next(k)) if "?" in s else s)
        pattern = mnemonic + (" " + ",".join(candidate) if candidate else "")
        if pattern in _ASM:
            break
    else:
        raise ValueError("not an instruction: %r" % line)
    code, cbop, displacement_first = _ASM[pattern]
    fields = _fields(pattern)
    vals = [e for _kind, e in exprs]
    size = len(code) + sum(_PLACE[f] for f in fields) + (1 if displacement_first else 0)
    data = bytearray()
    if displacement_first:
        d = value(vals[0], labels, pc)
        return bytes(code) + bytes([d & 0xFF, cbop])
    data += code
    for f, e in zip(fields, vals):
        v = value(e, labels, pc)
        if f == "e":
            v = v - (pc + size)
            if not -128 <= v <= 127:
                raise ValueError("relative jump out of range: %r" % line)
            data.append(v & 0xFF)
        elif f in ("n", "d"):
            data.append(v & 0xFF)
        else:
            data += bytes([v & 0xFF, (v >> 8) & 0xFF])
    return bytes(data)


def _balanced(s):
    depth = 0
    for c in s:
        depth += c == "("
        depth -= c == ")"
        if depth < 0:
            return False
    return depth == 0


def _choices(count):
    if count == 0:
        yield []
        return
    for first in ("n", "nn", "e", "d"):
        for rest in _choices(count - 1):
            yield [first] + rest
