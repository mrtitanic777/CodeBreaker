"""nand.py -- the flash file system of an Xbox 360 NAND dump (16 MB, small-block, with spare bytes).

    from codebreaker.formats.xbox import nand
    fs = nand.Flash(open(path, "rb").read())
    fs.files                        {name: (first block, size, time)}
    fs.read(name)                   a file's bytes
    nand.is_nand(path)

A raw dump is pages of 0x200 data bytes and 0x10 spare bytes, 32 pages to a block (0x4000 data
bytes). A block's spare bytes give its id (bytes 2 and 1) and type (byte 12, low six bits; $30 is
the file system's root). Blocks are found by id, not position (bad blocks are remapped). A root
block's even pages hold the allocation table (a big-endian 16-bit next-block number per block:
$1FFF ends a file, $1FFB reserved, $1FFE free), its odd pages sixteen 0x20-byte directory
entries: a name (22 bytes, zero padded), the first block (16 bits), the size and a time (32 bits
each). Older copies of the root stay in the flash; the one with the highest sequence number
(spare bytes 0, 3, 4 and 6) is current. Layout read from a dashboard 17559 console's dump.
"""
import os
import struct

PAGE, SPARE, PAGES_PER_BLOCK = 0x200, 0x10, 32
BLOCK = PAGE * PAGES_PER_BLOCK
ROOT_TYPE = 0x30
END = 0x1FFF


def is_nand(path):
    """A raw dump with spare bytes: its size a whole number of pages-with-spares, starting with the
    flash header's magic ($FF4F) and Microsoft's copyright."""
    try:
        size = os.path.getsize(path)
        with open(path, "rb") as f:
            head = f.read(0x40)
    except OSError:
        return False
    return size % (PAGE + SPARE) == 0 and size >= 0x1000000 and head[:2] == b"\xFF\x4F" and b"Microsoft" in head


class Flash(object):
    def __init__(self, raw):
        if len(raw) % (PAGE + SPARE):
            raise ValueError("not a raw NAND dump with spare bytes (size %#x)" % len(raw))
        self.raw = raw
        blocks = len(raw) // (PAGE + SPARE) // PAGES_PER_BLOCK
        self.by_id, roots = {}, []
        for b in range(blocks):
            s = self._spare(b)
            if s == b"\xFF" * SPARE:
                continue
            if s[12] & 0x3F == ROOT_TYPE:
                roots.append((s[0] << 24 | s[3] << 16 | s[4] << 8 | s[6], b))
            else:
                self.by_id.setdefault(s[2] << 8 | s[1], b)
        if not roots:
            raise ValueError("no flash file system root block")
        self.root = max(roots)[1]
        self.alloc, self.files = [], {}
        for i in range(PAGES_PER_BLOCK):
            p = self._page(self.root * PAGES_PER_BLOCK + i)
            if i % 2 == 0:
                self.alloc += struct.unpack(">256H", p)
                continue
            for o in range(0, PAGE, 0x20):
                e = p[o:o + 0x20]
                if e[0] in (0, 0xFF):
                    continue
                name = e[:22].rstrip(b"\0").decode("ascii", "replace")
                start, size, time = struct.unpack(">HII", e[22:])
                self.files[name] = (start, size, time)

    def _page(self, p):
        o = p * (PAGE + SPARE)
        return self.raw[o:o + PAGE]

    def _spare(self, block):
        o = block * PAGES_PER_BLOCK * (PAGE + SPARE) + PAGE
        return self.raw[o:o + SPARE]

    def chain(self, name):
        start, size, _t = self.files[name]
        out, blk = [], start
        while blk != END and len(out) * BLOCK < size:
            if blk >= len(self.alloc) or blk >= 0x1FF0:
                raise ValueError("%s: a bad block number in its chain (%#x)" % (name, blk))
            out.append(blk)
            blk = self.alloc[blk]
        if len(out) * BLOCK < size:
            raise ValueError("%s: its chain is short (%d blocks for %#x bytes)" % (name, len(out), size))
        return out

    def read(self, name):
        data = bytearray()
        for blk in self.chain(name):
            phys = self.by_id.get(blk, blk)
            for i in range(PAGES_PER_BLOCK):
                data += self._page(phys * PAGES_PER_BLOCK + i)
        return bytes(data[:self.files[name][1]])
