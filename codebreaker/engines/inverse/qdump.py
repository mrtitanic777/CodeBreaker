#!/usr/bin/env python3
"""qdump.py - structural parser for Lattice C 2.12 quad files (<base>.q).

Usage: python qdump.py <file.q>

Record framing (from LC2's reader @0x28c5 and confirmed by tiling every sample):
  each quad record = [opcode:1][length:1][payload: `length` bytes]
The reader reads 2 bytes (opcode,length) then `length` more, then parses up to 3
operands from the payload. A function is a run of records starting at FUNC-BEGIN
(0x23); the module ends at a 0xff terminator, after which come symbol trailer bytes
and the NUL-terminated name pool.

Opcode map (differentially verified via dynamic analysis; see
re/notes/05-quad-format-and-harness.md, 07-quad-framing.md):
"""
import sys, struct

OPC = {
    # unary
    0x0a:"neg", 0x0b:"lnot", 0x0c:"compl",
    # binary (0x10-0x1f)
    0x10:"mul", 0x11:"div", 0x12:"mod", 0x13:"add", 0x14:"sub", 0x15:"shl",
    0x16:"shr", 0x17:"lt", 0x18:"le", 0x19:"gt", 0x1a:"ge", 0x1b:"eq",
    0x1c:"ne", 0x1d:"and", 0x1e:"xor", 0x1f:"or",
    # statements / structure (>=0x23)
    0x23:"FUNC-BEGIN", 0x24:"RETURN", 0x20:"CALL", 0x2c:"CBRANCH",
    0x2e:"LABEL", 0x2f:"LOOP", 0x3f:"LABEL(named)", 0x40:"GOTO",
}

def namepool_start(d):
    i = len(d)
    while i > 0 and (32 <= d[i-1] < 127 or d[i-1] == 0):
        i -= 1
    return i

def fmt_payload(p):
    # operands are packed little-endian words; show a compact view
    return " ".join("%02x"%b for b in p)

def main(path):
    d = open(path, "rb").read()
    n = len(d)
    pool_off = namepool_start(d)
    names = [s.decode('latin1') for s in d[pool_off:].split(b'\x00') if s]
    print(f"== {path} ({n} bytes) ==")
    print(f"0x00 magic={struct.unpack_from('<H',d,0)[0]}  "
          f"0x04 type-size-tbl={[struct.unpack_from('<H',d,4+2*i)[0] for i in range(10)]}")
    print(f"0x4e symbolcount={struct.unpack_from('<H',d,0x4e)[0]}  "
          f"name-pool@0x{pool_off:x}={names}")
    print("\n-- quad records  [off] opcode(len) : payload --")
    o = 0x5c
    while o + 2 <= pool_off:
        op, ln = d[o], d[o+1]
        if op == 0xff:  # module/function terminator (not a quad)
            print(f"  0x{o:03x}: 0xff TERMINATOR (len={ln}); {pool_off-o} trailer bytes to name pool")
            break
        payload = d[o+2:o+2+ln]
        name = OPC.get(op, f"op{op:02x}")
        print(f"  0x{o:03x}: {name:<12}({ln:2d}) : {fmt_payload(payload)}")
        o += 2 + ln
    else:
        print(f"  (reached 0x{o:x}; name pool at 0x{pool_off:x})")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(__doc__ + "\n  " + ", ".join(f"{k:#04x}={v}" for k,v in sorted(OPC.items())))
        sys.exit(1)
    main(sys.argv[1])
