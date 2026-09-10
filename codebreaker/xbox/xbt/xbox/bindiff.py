"""Binary diff / patch — emit a compact, distributable patch between two builds
and apply it to reproduce the target.

Made for distributing XEX mods and title updates: diff two builds (or their
decompressed basefiles) and ship a small patch instead of the whole image. The
matcher is content-based (16-byte block index + greedy extension, like
rsync/xdelta), so it survives insertions/deletions, not just aligned edits.

Patch format (little-endian):
  magic 'XPCH'  u8 version(=1)  u64 target_size
  ops until target_size bytes produced:
    COPY  0x00  varint src_offset  varint length     (bytes from the source)
    ADD   0x01  varint length      length literal bytes

Commands:
  bindiff make  <old> <new> <patch>     create a patch
  bindiff apply <old> <patch> <new>     apply a patch
  bindiff info  <patch>                 op summary + size stats
"""

import struct
import sys

MAGIC = b"XPCH"
BLK = 16
COPY, ADD = 0x00, 0x01


def _wv(out, v):                                         # varint encode
    while True:
        b = v & 0x7F
        v >>= 7
        out.append(b | (0x80 if v else 0))
        if not v:
            break


def _rv(data, o):                                        # varint decode -> (val, next)
    v = s = 0
    while True:
        b = data[o]; o += 1
        v |= (b & 0x7F) << s
        if not (b & 0x80):
            return v, o
        s += 7


def make_patch(src: bytes, dst: bytes, max_candidates: int = 64) -> bytes:
    index = {}
    for i in range(0, len(src) - BLK + 1):
        index.setdefault(src[i:i + BLK], []).append(i)
    ops = bytearray()
    lit = bytearray()

    def flush_lit():
        if lit:
            ops.append(ADD)
            _wv(ops, len(lit))
            ops.extend(lit)
            lit.clear()

    i = 0
    n = len(dst)
    while i < n:
        best_len = 0
        best_off = 0
        if i + BLK <= n:
            for so in index.get(dst[i:i + BLK], ())[:max_candidates]:
                l = 0
                lim = min(n - i, len(src) - so)
                while l < lim and dst[i + l] == src[so + l]:
                    l += 1
                if l > best_len:
                    best_len, best_off = l, so
        if best_len >= BLK:
            flush_lit()
            ops.append(COPY)
            _wv(ops, best_off)
            _wv(ops, best_len)
            i += best_len
        else:
            lit.append(dst[i])
            i += 1
    flush_lit()
    head = bytearray(MAGIC)
    head.append(1)
    head += struct.pack("<Q", n)
    return bytes(head + ops)


def apply_patch(src: bytes, patch: bytes) -> bytes:
    if patch[:4] != MAGIC:
        raise ValueError("not an XPCH patch")
    target_size = struct.unpack_from("<Q", patch, 5)[0]
    o = 13
    out = bytearray()
    while len(out) < target_size and o < len(patch):
        op = patch[o]; o += 1
        if op == COPY:
            so, o = _rv(patch, o)
            ln, o = _rv(patch, o)
            out += src[so:so + ln]
        elif op == ADD:
            ln, o = _rv(patch, o)
            out += patch[o:o + ln]
            o += ln
        else:
            raise ValueError("bad opcode 0x%X" % op)
    return bytes(out[:target_size])


def patch_stats(patch: bytes) -> dict:
    target = struct.unpack_from("<Q", patch, 5)[0]
    o, copies, adds, addbytes, copybytes = 13, 0, 0, 0, 0
    while o < len(patch):
        op = patch[o]; o += 1
        if op == COPY:
            _so, o = _rv(patch, o); ln, o = _rv(patch, o)
            copies += 1; copybytes += ln
        else:
            ln, o = _rv(patch, o); o += ln
            adds += 1; addbytes += ln
    return {"target_size": target, "patch_size": len(patch), "copies": copies,
            "adds": adds, "copied_bytes": copybytes, "added_bytes": addbytes}


DESCRIPTION = "binary diff/patch between two builds (XEX images or any file): make/apply"


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print(__doc__.split("Commands:")[1].rstrip()); return
    op = a[0]
    if op == "make":
        p = make_patch(open(a[1], "rb").read(), open(a[2], "rb").read())
        open(a[3], "wb").write(p)
        s = patch_stats(p)
        print("patch -> %s  (%d bytes for a %d-byte target; %d copy / %d add ops, %d literal bytes)" % (
            a[3], s["patch_size"], s["target_size"], s["copies"], s["adds"], s["added_bytes"]))
        return
    if op == "apply":
        out = apply_patch(open(a[1], "rb").read(), open(a[2], "rb").read())
        open(a[3], "wb").write(out)
        print("applied -> %s (%d bytes)" % (a[3], len(out))); return
    if op == "info":
        s = patch_stats(open(a[1], "rb").read())
        for k, v in s.items():
            print("  %-14s %s" % (k, v))
        return
    print("unknown op: %s" % op)


if __name__ == "__main__":
    main()
