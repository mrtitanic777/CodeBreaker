#!/usr/bin/env python3
"""
opc8086.py — the authoritative map of which opcodes are legal 8086, harvested
from gzili's 8086-disassembler opcode table (refs/opcodes.inc).

WHY THIS MATTERS FOR A 1985 RECONSTRUCTION

Windows 1.01 was built for the 8086/8088. It contains NO 80186, 80286 or 80386
instructions — the machines did not exist in the install base. So an opcode byte
that decodes as a 186+ instruction, appearing where we think code is, is a
tell: that region is almost certainly DATA that a linear sweep ran through, not
code. objdump (our decoder) targets i386 and will happily decode `enter`,
`push imm`, `bound`, `0F`-prefixed instructions and 32-bit forms that cannot be
in this binary. This table is the ruler that catches them.

The table is transcribed from the primary 256-entry opcode map in
refs/opcodes.inc (each row is tagged with its byte, `; 00`..`; FF`). Rows the
author marked "(not used)" — 0F and the handful of genuinely-undefined 8086
encodings — are recorded as ILLEGAL. Everything else is a legal 8086 opcode,
with its mnemonic and the operand shapes the author assigned.

This module is data, not a decoder: it says WHICH bytes are legal 8086 and what
they mean, which is exactly what a differential check against objdump needs.
"""
import os
import re

HERE = os.path.dirname(os.path.abspath(__file__))
from _wtroot import data_root as _data_root
ROOT = _data_root(HERE)   # DATA root (build16/ reconstructed/ refs/ files/); tools live at HERE
INC = os.path.join(ROOT, "refs", "opcodes.inc")

# mnemonics reachable only through a ModRM group byte (tExtOpc / tOpc* rows).
# The primary row names a placeholder like insExtOpcF6_F7; the real mnemonic is
# one of these, chosen by the reg field. We keep the group's whole legal set so
# a group opcode is never flagged illegal on the strength of its placeholder.
GROUP_SETS = {
    "insOpc80_83": {"ADD", "OR", "ADC", "SBB", "AND", "SUB", "XOR", "CMP"},
    "insExtOpcF6_F7": {"TEST", "NOT", "NEG", "MUL", "IMUL", "DIV", "IDIV"},
    "insExtOpcFE_FF": {"INC", "DEC", "CALL", "JMP", "PUSH"},
    "insOpcD0_D3": {"ROL", "ROR", "RCL", "RCR", "SHL", "SHR", "SAR"},
}


def _load():
    """-> {byte: dict(mnem, ops, legal, group)} for all 256 primary opcodes.

    Indexed by ARRAY POSITION, not by the `; XX` comment. The comments are the
    author's hand annotations and contain typos (0x85 is labelled twice); the
    real opcode is the slot's position from the 0x00 row. Each `instrItem` is
    one slot; each `db ... N dup (0)` is N unused slots (e.g. 60-6F = 16). The
    233 rows + 23 dup slots sum to exactly 256, so position IS the opcode."""
    table = {b: {"mnem": None, "ops": (), "legal": False, "group": None}
             for b in range(256)}
    if not os.path.exists(INC):
        return table
    row = re.compile(
        r"instrItem<\s*(?:ins|op)(\w+)\s*,\s*t(\w+)\s*,\s*op(\w+)\s*,\s*op(\w+)\s*>")
    dup = re.compile(r"db\s*\(size\s+instrItem\)\s*\*\s*(\d+)\s*dup\s*\(0\)")
    # only lines inside the primary table (the first is tagged `; 00`)
    lines = open(INC, encoding="utf-8", errors="replace").read().splitlines()
    start = next(i for i, l in enumerate(lines)
                 if row.search(l) and re.search(r";\s*00\b", l))
    pos = 0
    for line in lines[start:]:
        if pos >= 256:
            break
        m = dup.search(line)
        if m:
            pos += int(m.group(1))                 # N unused slots
            continue
        m = row.search(line)
        if m:
            mnem, typ, op1, op2 = m.groups()
            grp = ("ins" + mnem) if (typ == "ExtOpc"
                                     or mnem.startswith(("ExtOpc", "Opc"))) else None
            table[pos] = {"mnem": mnem.upper(), "ops": (op1, op2),
                          "legal": typ not in ("SegOvr",) or True,
                          "group": grp, "type": typ}
            table[pos]["legal"] = True
            pos += 1
    return table


TABLE = _load()

# 186/286/386 opcodes objdump WILL decode but that CANNOT be genuine 1985 code.
# Two families:
#   - a leading 0x0F byte (all 286/386 two-byte opcodes; illegal-decoded as a
#     one-byte POP CS on a real 8086, which the author correctly marks unused)
#   - the 80186 one-byte additions at these primary slots.
POST8086 = {
    0x0F: "two-byte opcode (286/386) — 8086 has no 0F-prefixed instructions",
    0x60: "PUSHA (186)", 0x61: "POPA (186)",
    0x62: "BOUND (186)", 0x63: "ARPL (286)",
    0x64: "FS segment prefix (386)", 0x65: "GS segment prefix (386)",
    0x66: "operand-size prefix (386)", 0x67: "address-size prefix (386)",
    0x68: "PUSH imm16 (186)", 0x69: "IMUL r,rm,imm16 (186)",
    0x6A: "PUSH imm8 (186)", 0x6B: "IMUL r,rm,imm8 (186)",
    0x6C: "INSB (186)", 0x6D: "INSW (186)",
    0x6E: "OUTSB (186)", 0x6F: "OUTSW (186)",
    0xC8: "ENTER (186)", 0xC9: "LEAVE (186)",
}

# objdump mnemonics that betray a post-8086 decode regardless of opcode byte
# (covers 32-bit register forms and the 0F set by name).
POST8086_MNEM = re.compile(
    r"\b("
    r"pusha|popa|bound|arpl|enter|leave|ins[bwd]?|outs[bwd]?|"
    r"movsx|movzx|seto|setno|setb|setae|sete|setne|setbe|seta|sets|setns|"
    r"setp|setnp|setl|setge|setle|setg|"
    r"shld|shrd|bsf|bsr|bt|bts|btr|btc|cpuid|"
    r"e[abcds][xip]|[re][abcds][xip]"          # 32-bit registers (eax, esp, ...)
    r")\b", re.I)


def is_legal_8086(byte):
    """Is this primary opcode byte a legal 8086 instruction start?"""
    return TABLE.get(byte, {}).get("legal", False) and byte not in POST8086


def mnemonic(byte):
    e = TABLE.get(byte)
    return e["mnem"] if e and e["legal"] else None


def post8086_reason(byte, text=""):
    """Why this opcode/decode cannot be genuine 1985 8086 code, or None."""
    if byte in POST8086:
        return POST8086[byte]
    if text and POST8086_MNEM.search(text):
        return "post-8086 mnemonic/operand in objdump text"
    if byte in TABLE and not TABLE[byte]["legal"]:
        return "opcode byte undefined on 8086"
    return None


if __name__ == "__main__":
    legal = sum(1 for b in range(256) if TABLE[b]["legal"])
    print("refs/opcodes.inc -> %d legal 8086 primary opcodes, %d undefined"
          % (legal, 256 - legal))
    undf = [b for b in range(256) if not TABLE[b]["legal"]]
    print("undefined/unused bytes:",
          " ".join("%02X" % b for b in undf))
    print("186+ bytes flagged as anachronistic:",
          " ".join("%02X" % b for b in sorted(POST8086)))
