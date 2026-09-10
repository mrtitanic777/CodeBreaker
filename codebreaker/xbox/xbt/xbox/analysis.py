"""Xbox 360 binary analysis tools — PPC disassembly, function mapping, diffing."""

import struct
from pathlib import Path
from typing import Optional

# Standard function prologue on Xbox 360
PROLOGUE_MFSPR_LR = 0x7D8802A6  # mfspr r12, LR

# SHA-1 init constants (high halves loaded via lis)
SHA1_CONSTANTS = {
    0x6745: "SHA-1 H0 (0x67452301)",
    0xEFCD: "SHA-1 H1 (0xEFCDAB89)",
    0x98BA: "SHA-1 H2 (0x98BADCFE)",
    0x1032: "SHA-1 H3 (0x10325476)",
    0xC3D2: "SHA-1 H4 (0xC3D2E1F0)",
    0x5A82: "SHA-1 K0 (rounds 0-19)",
    0x6ED9: "SHA-1 K1 (rounds 20-39)",
    0x8F1B: "SHA-1 K2 (rounds 40-59)",
    0xCA62: "SHA-1 K3 (rounds 60-79)",
}

def decode_ppc(word: int) -> str:
    """Decode a PPC instruction to mnemonic string."""
    opcode = (word >> 26) & 0x3F
    rd = (word >> 21) & 0x1F
    ra = (word >> 16) & 0x1F
    rb = (word >> 11) & 0x1F
    simm = word & 0xFFFF
    if simm & 0x8000:
        simm -= 0x10000

    if word == 0x4E800020:
        return "blr"
    if word == 0x7D8802A6:
        return "mfspr   r12, LR"
    if word == 0x60000000:
        return "nop"

    if opcode == 14:
        if ra == 0:
            return f"li      r{rd}, {simm}"
        return f"addi    r{rd}, r{ra}, {simm}"
    elif opcode == 15:
        if ra == 0:
            return f"lis     r{rd}, 0x{word & 0xFFFF:04X}"
        return f"addis   r{rd}, r{ra}, 0x{word & 0xFFFF:04X}"
    elif opcode == 18:
        li = word & 0x03FFFFFC
        if li & 0x02000000:
            li -= 0x04000000
        lk = "l" if word & 1 else ""
        aa = "a" if word & 2 else ""
        return f"b{lk}{aa}      0x{li & 0xFFFFFFFF:X}"
    elif opcode == 16:
        bo = rd
        bi = ra
        bd = simm & 0xFFFC
        if bd & 0x8000:
            bd -= 0x10000
        lk = "l" if word & 1 else ""
        return f"bc{lk}     {bo}, {bi}, {bd}"
    elif opcode == 19:
        xo = (word >> 1) & 0x3FF
        if xo == 16:
            return "blr"
        elif xo == 528:
            return "bctr" + ("l" if word & 1 else "")
        elif xo == 150:
            return "isync"
        return f"cr_op   xo={xo}"
    elif opcode == 24:
        if rd == 0 and ra == 0 and (word & 0xFFFF) == 0:
            return "nop"
        return f"ori     r{ra}, r{rd}, 0x{word & 0xFFFF:04X}"
    elif opcode == 28:
        return f"andi.   r{ra}, r{rd}, 0x{word & 0xFFFF:04X}"
    elif opcode == 31:
        xo = (word >> 1) & 0x3FF
        names = {
            266: "add", 40: "subf", 444: "or", 28: "and", 316: "xor",
            339: "mfspr", 467: "mtspr", 26: "cntlzw", 0: "cmp",
            32: "cmpl", 824: "srawi", 792: "sraw",
            536: "srw", 24: "slw",
        }
        # 64-bit multiply
        if xo == 9:
            return f"mulhdu  r{rd}, r{ra}, r{rb}"
        if xo == 233:
            return f"mulld   r{rd}, r{ra}, r{rb}"
        if xo == 489:
            return f"divd    r{rd}, r{ra}, r{rb}"
        name = names.get(xo, f"op31.{xo}")
        return f"{name:<8s}r{rd}, r{ra}, r{rb}"
    elif opcode == 32:
        return f"lwz     r{rd}, {simm}(r{ra})"
    elif opcode == 33:
        return f"lwzu    r{rd}, {simm}(r{ra})"
    elif opcode == 34:
        return f"lbz     r{rd}, {simm}(r{ra})"
    elif opcode == 36:
        return f"stw     r{rd}, {simm}(r{ra})"
    elif opcode == 37:
        return f"stwu    r{rd}, {simm}(r{ra})"
    elif opcode == 38:
        return f"stb     r{rd}, {simm}(r{ra})"
    elif opcode == 40:
        return f"lhz     r{rd}, {simm}(r{ra})"
    elif opcode == 42:
        return f"lha     r{rd}, {simm}(r{ra})"
    elif opcode == 44:
        return f"sth     r{rd}, {simm}(r{ra})"
    elif opcode == 46:
        return f"lmw     r{rd}, {simm}(r{ra})"
    elif opcode == 47:
        return f"stmw    r{rd}, {simm}(r{ra})"
    elif opcode == 58:
        return f"ld      r{rd}, {simm & 0xFFFC}(r{ra})"
    elif opcode == 62:
        return f"std     r{rd}, {simm & 0xFFFC}(r{ra})"
    elif opcode == 11:
        return f"cmpi    cr{rd >> 2}, r{ra}, {simm}"
    elif opcode == 10:
        return f"cmpli   cr{rd >> 2}, r{ra}, {word & 0xFFFF}"
    else:
        return f"op{opcode:<3d}   0x{word:08X}"


def find_functions(data: bytes, base_addr: int) -> list:
    """Find all functions by scanning for standard prologues.
    
    Returns list of (address, estimated_size, first_instruction) tuples.
    """
    prologues = []
    for i in range(0, len(data) - 4, 4):
        w = struct.unpack(">I", data[i:i+4])[0]
        if w == PROLOGUE_MFSPR_LR:
            prologues.append(base_addr + i)

    functions = []
    for idx, addr in enumerate(prologues):
        if idx + 1 < len(prologues):
            size = prologues[idx + 1] - addr
        else:
            size = base_addr + len(data) - addr
        offset = addr - base_addr
        first = struct.unpack(">I", data[offset:offset + 4])[0]
        functions.append((addr, size, first))

    return functions


def disassemble(data: bytes, base_addr: int, count: int = None) -> list:
    """Disassemble PPC binary data.
    
    Returns list of (address, raw_word, mnemonic) tuples.
    """
    result = []
    limit = len(data) if count is None else min(len(data), count * 4)
    for i in range(0, limit, 4):
        if i + 4 > len(data):
            break
        w = struct.unpack(">I", data[i:i+4])[0]
        mnemonic = decode_ppc(w)
        result.append((base_addr + i, w, mnemonic))
    return result


def find_crypto_constants(data: bytes, base_addr: int) -> list:
    """Scan for crypto constants (SHA-1, etc.) loaded via lis instructions."""
    results = []
    for i in range(0, len(data) - 4, 4):
        w = struct.unpack(">I", data[i:i+4])[0]
        opcode = (w >> 26) & 0x3F
        if opcode == 15:  # lis
            imm = w & 0xFFFF
            if imm in SHA1_CONSTANTS:
                rd = (w >> 21) & 0x1F
                results.append((base_addr + i, rd, imm, SHA1_CONSTANTS[imm]))
    return results


def analyze_prologue(data: bytes, base_addr: int, func_addr: int) -> dict:
    """Analyze a function's prologue to determine calling convention.
    
    Returns dict with stack_size, saved_registers, parameter_registers used.
    """
    offset = func_addr - base_addr
    if offset < 0 or offset + 64 > len(data):
        return {"error": "Address out of range"}

    info = {
        "address": func_addr,
        "stack_size": 0,
        "saves_lr": False,
        "params_checked": [],
        "first_param_use": None,
    }

    # Scan first 16 instructions of function
    for i in range(16):
        if offset + i * 4 + 4 > len(data):
            break
        w = struct.unpack(">I", data[offset + i * 4:offset + i * 4 + 4])[0]
        opcode = (w >> 26) & 0x3F
        rd = (w >> 21) & 0x1F
        ra = (w >> 16) & 0x1F
        simm = w & 0xFFFF
        if simm & 0x8000:
            simm -= 0x10000

        # mfspr r12, LR
        if w == 0x7D8802A6:
            info["saves_lr"] = True

        # stwu r1, offset(r1) — stack allocation
        if opcode == 37 and rd == 1 and ra == 1:
            info["stack_size"] = -simm

        # cmplwi rX, imm — parameter check (rX = r3-r10 are params)
        if opcode == 10 and 3 <= ra <= 10:
            info["params_checked"].append(f"r{ra}")

        # First instruction that reads r3 (first parameter)
        if info["first_param_use"] is None:
            if opcode in (10, 11) and ra == 3:  # compare r3
                info["first_param_use"] = i
            elif opcode == 31 and ra == 3:  # op using r3
                info["first_param_use"] = i

    return info


def diff_binaries(data1: bytes, data2: bytes, base_addr: int) -> list:
    """Diff two binary dumps at the same base address.
    
    Returns list of (offset, addr, old_word, new_word) for differing DWORDs.
    """
    diffs = []
    size = min(len(data1), len(data2))
    for i in range(0, size - 3, 4):
        w1 = struct.unpack(">I", data1[i:i+4])[0]
        w2 = struct.unpack(">I", data2[i:i+4])[0]
        if w1 != w2:
            diffs.append((i, base_addr + i, w1, w2))
    return diffs


def function_xrefs(data: bytes, base_addr: int, target_addr: int) -> list:
    """Find all call sites (bl instructions) that target a specific address."""
    xrefs = []
    for i in range(0, len(data) - 4, 4):
        w = struct.unpack(">I", data[i:i+4])[0]
        opcode = (w >> 26) & 0x3F
        if opcode == 18 and (w & 1):  # bl (branch and link)
            li = w & 0x03FFFFFC
            if li & 0x02000000:
                li -= 0x04000000
            call_from = base_addr + i
            call_to = call_from + li
            if call_to == target_addr:
                xrefs.append(call_from)
    return xrefs


DESCRIPTION = "scan a PowerPC binary: function prologues + embedded crypto constants"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: analyze360 <bin> [base_hex]"); return
    data = open(a[0], "rb").read()
    base = int(a[1], 16) if len(a) > 1 else 0x80000000
    funcs = find_functions(data, base)
    print("functions (prologue scan): %d" % len(funcs))
    for addr, size, first in funcs[:12]:
        print("   0x%08X  ~%d bytes  %s" % (addr, size, first))
    consts = find_crypto_constants(data, base)
    if consts:
        print("crypto constants: %d" % len(consts))
        for c in consts[:12]:
            print("   %s" % (c,))


if __name__ == "__main__":
    main()
