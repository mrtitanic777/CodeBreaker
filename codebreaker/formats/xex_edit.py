"""xex_edit.py -- change an Xbox 360 executable: its privilege flags, or its code at an address.

    codebreaker xex-edit privileges <in.xex>                         the privilege bits that are set
    codebreaker xex-edit privileges <in.xex> <+bit|-bit ...> <out>   set (+) or clear (-) bits; `pal50`
                                                                     names bit 10, PAL-50 incompatible
    codebreaker xex-edit nop   <in.xex> <address> [count] <out>      count instructions become nop
    codebreaker xex-edit bytes <in.xex> <address> <hex> <out>        raw bytes over the code
    codebreaker xex-edit asm   <in.xex> <address> "<asm>" <out>      PowerPC assembled there
    codebreaker xex-edit hook  <in.xex> <address> <cave> "<asm>" <out>
                                                                     `b cave` at address; at the cave
                                                                     the asm, the displaced instruction,
                                                                     and `b address+4`
    codebreaker xex-edit asm-only "<asm>" [address]                  the bytes, nothing written

Privileges live in the header's privilege entries (key 0x30000: bits 0-31; 0x30100: bits 32-63),
changed where they stand. A code edit writes the module as an unencrypted XEX holding the whole
image in one block (xex.basic) and changes the image there; that is what the recompiler, the Xbox
360 runtime and emulators read. Several asm lines are separated by `;` or newlines. The header's
hashes and signature are not redone: `codebreaker xex compress` / `machine` re-pack and re-sign.
Only bit 10 is named here: the meaning of the other bits is not settled from a source this
project trusts, so they are given by number.
"""
import os
import struct
import sys
import tempfile

from codebreaker.build import ppc_asm
from codebreaker.formats import xex

PRIVILEGE_KEYS = (0x00030000, 0x00030100)             # bits 0-31, bits 32-63
NAMED_BITS = {"pal50": 10}                             # PAL-50 incompatible
NOP = b"\x60\x00\x00\x00"


def _entries(data):
    if data[:4] != xex.MAGIC:
        raise ValueError("not an XEX2 module")
    count, = struct.unpack_from(">I", data, 0x14)
    return [(0x18 + i * 8,) + struct.unpack_from(">II", data, 0x18 + i * 8) for i in range(count)]


def privileges(data):
    """{bit: True} for each privilege bit set, and the entries' offsets {key: value offset}."""
    bits, where = {}, {}
    for offset, key, value in _entries(data):
        if key in PRIVILEGE_KEYS:
            first = 32 * PRIVILEGE_KEYS.index(key)
            where[key] = offset + 4
            for b in range(32):
                if value >> b & 1:
                    bits[first + b] = True
    return sorted(bits), where


def set_privileges(data, changes):
    """`data` with each (bit, on) change made. A bit in a privilege entry the header lacks stops."""
    out = bytearray(data)
    _bits, where = privileges(data)
    for bit, on in changes:
        key = PRIVILEGE_KEYS[bit // 32]
        if key not in where:
            raise ValueError("the header has no privilege entry for bit %d (key %X)" % (bit, key))
        value, = struct.unpack_from(">I", out, where[key])
        mask = 1 << (bit % 32)
        struct.pack_into(">I", out, where[key], (value | mask) if on else (value & ~mask))
    return bytes(out)


def _parse_bit(token):
    sign, name = token[0], token[1:].lower()
    if sign not in "+-":
        raise ValueError("a change is +bit or -bit: %r" % token)
    bit = NAMED_BITS[name] if name in NAMED_BITS else int(name, 0)
    if not 0 <= bit < 64:
        raise ValueError("privilege bits are 0-63: %r" % token)
    return bit, sign == "+"


class Image:
    """A module written as a basic XEX: its header, then the whole image from `pe_offset`."""

    def __init__(self, path):
        h = xex.header(path)
        fd, tmp = tempfile.mkstemp(suffix=".xex")
        os.close(fd)
        try:
            xex.basic(path, tmp)
            with open(tmp, "rb") as f:
                self.data = bytearray(f.read())
        finally:
            os.remove(tmp)
        self.base = h.base
        self.pe_offset, = struct.unpack_from(">I", self.data, 8)

    def offset(self, address, size):
        off = self.pe_offset + address - self.base
        if address < self.base or off + size > len(self.data):
            raise ValueError("%08X+%X is outside the image (%08X, %X bytes)"
                             % (address, size, self.base, len(self.data) - self.pe_offset))
        return off

    def read(self, address, size):
        off = self.offset(address, size)
        return bytes(self.data[off:off + size])

    def write(self, address, blob):
        off = self.offset(address, len(blob))
        self.data[off:off + len(blob)] = blob

    def save(self, out):
        with open(out, "wb") as f:
            f.write(bytes(self.data))


def _relative_branch(word):
    """A displaced instruction that branches relative to its own address cannot move to a cave."""
    op = word >> 26
    return (op == 18 and not word & 2) or (op == 16 and not word & 2)


def hook(image, address, cave, asm):
    stolen = image.read(address, 4)
    word, = struct.unpack(">I", stolen)
    if _relative_branch(word):
        raise ValueError("the instruction at %08X is a relative branch (%08X): it cannot be moved to the cave"
                         % (address, word))
    body = ppc_asm.assemble(asm, cave)
    back = ppc_asm.assemble_one("b 0x%X" % (address + 4), cave + len(body) + 4)
    image.write(cave, body + stolen + struct.pack(">I", back))
    image.write(address, struct.pack(">I", ppc_asm.assemble_one("b 0x%X" % cave, address)))


def _asm_text(text):
    return text.replace(";", "\n")


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__)
        return 0 if argv else 2
    op, args = argv[0], argv[1:]
    try:
        if op == "asm-only":
            address = int(args[1], 0) if len(args) > 1 else 0
            print(ppc_asm.assemble(_asm_text(args[0]), address).hex())
            return 0
        if op == "privileges":
            with open(args[0], "rb") as f:
                data = f.read()
            if len(args) == 1:
                bits, _where = privileges(data)
                names = {v: k for k, v in NAMED_BITS.items()}
                print("privileges set: " + (", ".join("%d%s" % (b, " (%s)" % names[b] if b in names else "")
                                                      for b in bits) or "none"))
                return 0
            data = set_privileges(data, [_parse_bit(t) for t in args[1:-1]])
            with open(args[-1], "wb") as f:
                f.write(data)
            print("privileges now: %s -> %s" % (", ".join(map(str, privileges(data)[0])) or "none", args[-1]))
            return 0
        if op not in ("nop", "bytes", "asm", "hook"):
            print("unknown operation %r (privileges, nop, bytes, asm, hook, asm-only)" % op)
            return 2
        image, address, out = Image(args[0]), int(args[1], 0), args[-1]
        if op == "nop":
            count = int(args[2], 0) if len(args) > 3 else 1
            image.write(address, NOP * count)
        elif op == "bytes":
            image.write(address, bytes.fromhex(args[2].replace(" ", "")))
        elif op == "asm":
            image.write(address, ppc_asm.assemble(_asm_text(args[2]), address))
        else:
            hook(image, address, int(args[2], 0), _asm_text(args[3]))
        image.save(out)
        print("%s at %08X -> %s" % (op, address, out))
        return 0
    except (ValueError, IndexError, KeyError, ppc_asm.AsmError) as e:
        print("xex-edit: %s" % e)
        return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
