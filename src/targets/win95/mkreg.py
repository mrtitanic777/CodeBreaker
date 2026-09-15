#!/usr/bin/env python3
r"""mkreg.py -- generate a minimal valid Windows 95 SYSTEM.DAT registry (CREG format).

Byte layout per the libyal/libcreg spec (Windows 9x Registry File format). A CREG hive is:
  CREG header (32 bytes) | RGKN block (key-navigation tree) | RGDB block(s) (key/value data).

VMM32's loader warns "Registry File was not found / XMS cache problem" and runs degraded without a
valid hive; a well-formed minimal one clears that and enables registry services for the GUI phase.

    wt --target win95 mkreg <out SYSTEM.DAT>
"""
import struct
import sys

RGKN_HDR = 0x20
ENT = 0x1C                    # key-hierarchy entry = 28 bytes


def _hash(name):
    """CREG name hash: uppercase, sum ASCII values < 0x80 ('SOFTWARE' -> 0x26b)."""
    return sum(b for b in name.upper().encode("latin1") if b < 0x80) & 0xFFFFFFFF


def build():
    # standard minimal SYSTEM.DAT top level (root name is empty)
    keys = [("", -1), ("SOFTWARE", 0), ("System", 0), ("Enum", 0), ("Config", 0),
            ("Classes", 1), ("CurrentControlSet", 2)]
    n = len(keys)

    def eoff(i):                                  # entry offset relative to RGKN start
        return RGKN_HDR + i * ENT

    # link first-subkey / next-sibling
    child = [0xFFFFFFFF] * n
    sib = [0xFFFFFFFF] * n
    last = {}
    for i, (_nm, par) in enumerate(keys):
        if par < 0:
            continue
        if par in last:
            sib[last[par]] = eoff(i)
        else:
            child[par] = eoff(i)
        last[par] = i

    # RGKN key-hierarchy entries: unknown(0), hash, ref(-1), parent, child, next, nameID, block#
    ents = b""
    for i, (nm, par) in enumerate(keys):
        paroff = 0xFFFFFFFF if par < 0 else eoff(par)
        ents += struct.pack("<IIIIIIHH", 0, _hash(nm), 0xFFFFFFFF,
                            paroff, child[i], sib[i], i, 0)
    rgkn_used = RGKN_HDR + len(ents)
    rgkn_total = rgkn_used
    rgkn = struct.pack("<4sIIIII", b"RGKN", rgkn_total, RGKN_HDR, len(ents), 0, 0)
    rgkn += b"\x00" * (RGKN_HDR - len(rgkn)) + ents

    # RGDB key-name records: size, index, unk, usedsize, namelen, valcount, reserved, name
    recs = b""
    for i, (nm, _par) in enumerate(keys):
        name = nm.encode("latin1")
        content = struct.pack("<HHIHHI", i, 0, 0, len(name), 0, 0) + name
        rec = struct.pack("<I", 4 + len(content)) + content
        # 'used size' field (offset 8) = actual content size
        rec = rec[:8] + struct.pack("<I", len(rec)) + rec[12:]
        recs += rec
    rgdb_used = 0x20 + len(recs)
    rgdb_total = rgdb_used
    rgdb = struct.pack("<4sIIHHIHH", b"RGDB", rgdb_total, 0, 0x0008, 0, rgdb_used, n, 0)
    rgdb += b"\x00" * (0x20 - len(rgdb)) + recs

    rgdb_off = 0x20 + len(rgkn)
    # CREG header: magic, minor(0), major(1), rgdb offset, checksum(0), block count(1), ...
    creg = struct.pack("<4sHHIIHHHH", b"CREG", 0, 1, rgdb_off, 0, 1, 0, 0, 1)
    creg += b"\x00" * (0x20 - len(creg))
    return creg + rgkn + rgdb


def main(argv):
    out = argv[0] if argv else "SYSTEM.DAT"
    data = build()
    open(out, "wb").write(data)
    print("wrote %s (%d bytes): CREG + RGKN + RGDB, 7 keys (spec-conformant)" % (out, len(data)))


if __name__ == "__main__":
    main(sys.argv[1:])
