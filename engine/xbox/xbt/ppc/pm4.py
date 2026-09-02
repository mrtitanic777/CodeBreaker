"""Xenos PM4 command-buffer parser — decode the Xbox 360 GPU command stream.

The Xenos GPU is driven by an AMD-style PM4 packet ring: the CPU writes packets
(register writes, draws, constant loads, indirect buffers) that the command
processor executes. This decodes a captured command buffer / ring dump into a
readable packet listing, which pairs with the shader disassembler for GPU RE.

Packet types (header word): 0 = register write, 2 = NOP/pad, 3 = typed opcode.
Opcode/register facts are the documented Xenos PM4 set. Words are big-endian by
default (Xenon); pass little-endian for a host-captured stream.
"""

import struct
import sys

DESCRIPTION = "decode Xbox 360 Xenos GPU PM4 command buffers (register writes / draws / loads)"

# Type-3 IT opcodes (documented Xenos PM4).
_TYPE3 = {
    0x00: "ME_INIT", 0x10: "NOP", 0x12: "INDIRECT_BUFFER", 0x13: "INDIRECT_BUFFER_PFD",
    0x15: "WAIT_FOR_IDLE", 0x16: "WAIT_REG_MEM", 0x21: "WAIT_REG_MEM", 0x22: "REG_RMW",
    0x23: "COND_WRITE", 0x24: "EVENT_WRITE", 0x25: "EVENT_WRITE_SHD",
    0x26: "EVENT_WRITE_CFL", 0x28: "EVENT_WRITE_ZPD", 0x29: "DRAW_INDX_2",
    0x2A: "DRAW_INDX", 0x2C: "VIZ_QUERY", 0x2D: "SET_STATE", 0x2F: "DRAW_INDX_2",
    0x33: "IM_LOAD", 0x34: "IM_LOAD_IMMEDIATE", 0x36: "SET_CONSTANT",
    0x37: "LOAD_ALU_CONSTANT", 0x39: "SET_CONSTANT2", 0x3B: "INVALIDATE_STATE",
    0x3C: "SET_SHADER_CONSTANTS", 0x3F: "MEM_WRITE", 0x40: "MEM_WRITE_CNTR",
    0x43: "COND_EXEC", 0x44: "COND_WRITE", 0x46: "EVENT_WRITE", 0x50: "ME_INIT",
    0x55: "SET_BIN_MASK", 0x56: "SET_BIN_SELECT", 0x57: "WAIT_REG_EQ",
    0x58: "WAIT_REG_GTE", 0x5C: "CONTEXT_UPDATE",
}


class Packet:
    def __init__(self, offset, ptype, header, count, name, body):
        self.offset = offset
        self.type = ptype
        self.header = header
        self.count = count          # body dword count
        self.name = name
        self.body = body            # tuple of body dwords

    def __repr__(self):
        return "+0x%04X  T%d %-20s count=%-3d %s" % (
            self.offset, self.type, self.name, self.count,
            " ".join("%08X" % w for w in self.body[:6]) + (" …" if len(self.body) > 6 else ""))


def parse(data, big_endian=True):
    """Decode a PM4 command buffer into a list of Packet."""
    endian = ">" if big_endian else "<"
    n = len(data) // 4
    words = list(struct.unpack(endian + "I" * n, data[:n * 4]))
    out = []
    i = 0
    while i < len(words):
        w = words[i]
        off = i * 4
        ptype = (w >> 30) & 3
        if ptype == 0:                              # register write
            count = ((w >> 16) & 0x3FFF) + 1
            base = w & 0x7FFF
            body = tuple(words[i + 1:i + 1 + count])
            out.append(Packet(off, 0, w, count, "REG_WRITE base=0x%04X" % base, body))
            i += 1 + count
        elif ptype == 2:                            # NOP / pad
            out.append(Packet(off, 2, w, 0, "NOP", ()))
            i += 1
        elif ptype == 3:                            # typed opcode
            opcode = (w >> 8) & 0x7F
            count = ((w >> 16) & 0x3FFF) + 1
            body = tuple(words[i + 1:i + 1 + count])
            out.append(Packet(off, 3, w, count, _TYPE3.get(opcode, "OP_0x%02X" % opcode), body))
            i += 1 + count
        else:                                       # type 1 (unused) — stop
            out.append(Packet(off, 1, w, 0, "TYPE1?", ()))
            i += 1
    return out


def summary(packets):
    counts = {}
    draws = 0
    for p in packets:
        counts[p.name.split()[0]] = counts.get(p.name.split()[0], 0) + 1
        if "DRAW" in p.name:
            draws += 1
    return {"packets": len(packets), "draws": draws, "by_type": counts}


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: pm4 <cmdbuf.bin> [--le] [--limit N] [--summary]")
        return
    data = open(a[0], "rb").read()
    pkts = parse(data, big_endian="--le" not in a)
    if "--summary" in a:
        s = summary(pkts)
        print("PM4: %d packets, %d draw(s)" % (s["packets"], s["draws"]))
        for name, n in sorted(s["by_type"].items(), key=lambda kv: -kv[1]):
            print("  %-20s %d" % (name, n))
        return
    limit = int(a[a.index("--limit") + 1]) if "--limit" in a else 200
    for p in pkts[:limit]:
        print(p)
    if len(pkts) > limit:
        print("  ... (%d more packets)" % (len(pkts) - limit))


if __name__ == "__main__":
    main()
