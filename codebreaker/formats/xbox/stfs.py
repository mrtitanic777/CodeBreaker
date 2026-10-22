#!/usr/bin/env python3
"""STFS — Xbox 360 secure content package reader (CON / LIVE / PIRS).

A clean-room Python implementation of the STFS container format, written to the
public/documented format algorithm (Free60 and community references). It gives
CodeBreaker native read access to Xbox 360 content packages — what the GPL C#
tools (Le Fluffie / the X360 library) do for reading — as original code, so the
capability lives in the tool without any GPL source.

STFS stores a small filesystem in fixed 0x1000-byte blocks with hash blocks
interleaved among the data: one level-0 hash block per 0xAA (170) data blocks,
one level-1 per 0x70E4, optionally a level-2. `_cbdbn` turns a logical data-block
index into its real ("backing") position past those hash blocks; the hash entry
for each block records the *next* block, forming a chain. Packages come in two
hash layouts ("female"/"male", a header bit) which shift the table spacing, and
each level uses one of two shadow tables selected by a status bit in the table
above it.

Resigning: a CON package carries a console certificate (from a keyvault) and an
RSA signature of its header. The signature is standard PKCS#1 v1.5 RSA-SHA1 over
header[0x22C:0x344] using the console's 1024-bit private key, stored byte-reversed
at 0x1AC; the certificate (0x1A8 bytes) is copied verbatim from the keyvault. Both
were reverse-engineered and confirmed byte-exact against genuine console-signed
packages, so `resign` binds a package to a console you own using its KV_dec.bin.
A LIVE/PIRS package (Microsoft-signed -- unforgeable) is converted to CON and
console-signed the same way tools like Velocity do. See docs/STFS.md.

Commands:
    stfs info    <package>            header + content metadata
    stfs list    <package>            the contained file listing
    stfs extract <package> [outdir]   extract every contained file
    stfs resign  <package> <KV> [out] resign CON / convert LIVE|PIRS -> console CON
"""
DESCRIPTION = "read/resign Xbox 360 STFS packages (CON/LIVE/PIRS)"

import hashlib
import os
import struct
import sys

BLOCK = 0x1000
MAGICS = (b"CON ", b"LIVE", b"PIRS")


def _u24_le(d, o):
    return d[o] | (d[o + 1] << 8) | (d[o + 2] << 16)


class STFS:
    def __init__(self, data):
        if data[:4] not in MAGICS:
            raise ValueError("not an STFS package (magic %r)" % data[:4])
        self.d = data
        self.magic = data[:4]

        self.header_size = struct.unpack_from(">I", data, 0x340)[0]
        self.first_hash = (self.header_size + 0xFFF) & 0xFFFFF000

        # STFS volume descriptor
        self.block_sep = data[0x37B]
        self.ft_block_count = struct.unpack_from("<H", data, 0x37C)[0]
        self.ft_block_number = _u24_le(data, 0x37E)
        self.alloc_blocks = struct.unpack_from(">I", data, 0x395)[0]
        self.unalloc_blocks = struct.unpack_from(">I", data, 0x399)[0]

        self.sex = (~self.block_sep) & 1                 # 0 = female, 1 = male
        self.step = [0xAB, 0x718F] if self.sex == 0 else [0xAC, 0x723A]
        self.top_level = 0 if self.alloc_blocks <= 0xAA else (
            1 if self.alloc_blocks <= 0x70E4 else 2)

        # content metadata
        self.content_type = struct.unpack_from(">I", data, 0x344)[0]
        self.title_id = data[0x360:0x364].hex().upper()
        self.profile_id = data[0x371:0x379].hex().upper()
        self.display_name = self._wstr(0x411)
        self.title_name = self._wstr(0x1691)

        self._load_top_table()
        self.files = self._read_file_listing()

    # ------------------------------------------------------------------ util
    def _wstr(self, off, maxlen=0x80):
        return self.d[off:off + maxlen].decode("utf-16-be", "replace").split("\x00", 1)[0]

    # --------------------------------------------------- block address maths
    def _cbdbn(self, b):
        """ComputeBackingDataBlockNumber — data block -> backing block."""
        r = (((b + 0xAA) // 0xAA) << self.sex) + b
        if b < 0xAA:
            return r
        r += ((b + 0x70E4) // 0x70E4) << self.sex
        return r if b < 0x70E4 else r + (1 << self.sex)

    def _block_addr(self, b):
        return (self._cbdbn(b) << 0xC) + self.first_hash

    def _cl0(self, b):
        """ComputeLevel0BackingHashBlockNumber."""
        if b < 0xAA:
            return 0
        num = (b // 0xAA) * self.step[0]
        num += ((b // 0x70E4) + 1) << self.sex
        return num if b // 0x70E4 == 0 else num + (1 << self.sex)

    def _cl1(self, b):
        if b < 0x70E4:
            return self.step[0]
        return (1 << self.sex) + (b // 0x70E4) * self.step[1]

    def _load_top_table(self):
        """Cache the top hash table's per-entry status bytes; `_hash_addr` reads
        their 0x40 bit to choose which shadow table a lower level lives in."""
        true_block = (self._cl0(0) if self.top_level == 0 else
                      self._cl1(0) if self.top_level == 1 else self.step[1])
        base = (true_block << 0xC) + self.first_hash
        addr = base + ((self.block_sep & 2) << 0xB)
        per = [1, 0xAA, 0x70E4][self.top_level]
        cnt = self.alloc_blocks // per
        if self.alloc_blocks > 0x70E4 and self.alloc_blocks % 0x70E4:
            cnt += 1
        elif self.alloc_blocks > 0xAA and self.alloc_blocks % 0xAA:
            cnt += 1
        self.top_entries = []
        for i in range(cnt + 1):
            e = self.d[addr + i * 0x18: addr + i * 0x18 + 0x18]
            if len(e) < 0x18:
                break
            self.top_entries.append(e[0x14])             # status byte

    def _hash_addr(self, b):
        a = (self._cl0(b) << 0xC) + self.first_hash + (b % 0xAA) * 0x18
        if self.top_level == 0:
            a += (self.block_sep & 2) << 0xB
        elif self.top_level == 1:
            a += (self.top_entries[b // 0xAA] & 0x40) << 6
        else:
            l1 = (self.top_entries[b // 0x70E4] & 0x40) << 6
            pos = (self._cl1(b) << 0xC) + self.first_hash + l1 + (b % 0xAA) * 0x18
            a += (self.d[pos + 0x14] & 0x40) << 6
        return a

    def _next_block(self, b):
        a = self._hash_addr(b)
        return (self.d[a + 0x15] << 16) | (self.d[a + 0x16] << 8) | self.d[a + 0x17]

    # ------------------------------------------------------------- file table
    def _read_chain(self, start, nblocks, size):
        out = bytearray()
        cur = start
        for _ in range(nblocks or 1 << 30):
            o = self._block_addr(cur)
            out += self.d[o:o + BLOCK]
            cur = self._next_block(cur)
            if cur == 0xFFFFFF or cur == 0:
                break
        return bytes(out[:size]) if size else bytes(out)

    def _read_file_listing(self):
        files = {}
        cur = self.ft_block_number
        for _ in range(self.ft_block_count):
            block = self.d[self._block_addr(cur):self._block_addr(cur) + BLOCK]
            for i in range(BLOCK // 0x40):
                e = block[i * 0x40:(i + 1) * 0x40]
                nlen = e[0x28] & 0x3F
                if nlen == 0:
                    continue
                name = e[:nlen].decode("latin-1", "ignore")
                blocks = _u24_le(e, 0x29)
                start = _u24_le(e, 0x2F)
                size = struct.unpack_from(">I", e, 0x34)[0]
                if not (e[0x28] & 0x80):                  # not a directory
                    files[name] = (start, blocks, size)
            cur = self._next_block(cur)
            if cur == 0xFFFFFF or cur == 0:
                break
        return files

    def read_file(self, name):
        start, blocks, size = self.files[name]
        return self._read_chain(start, blocks, size)

    def extract_all(self, outdir):
        n = 0
        for name in self.files:
            dest = os.path.join(outdir, name.replace("/", os.sep).replace("\\", os.sep))
            os.makedirs(os.path.dirname(dest) or outdir, exist_ok=True)
            with open(dest, "wb") as f:
                f.write(self.read_file(name))
            n += 1
        return n


# ------------------------------------------------------------- writer (pack)
class _WriteGeom:
    """Block geometry for writing. Packages are written in the 'male' layout
    (block separation 0), which fixes the table spacing at [0xAC, 0x723A]."""
    def __init__(self, alloc_blocks, first_hash):
        self.sex = 1
        self.step = [0xAC, 0x723A]
        self.alloc_blocks = alloc_blocks
        self.first_hash = first_hash
        self.top_level = 0 if alloc_blocks <= 0xAA else 1

    def cbdbn(self, b):
        r = (((b + 0xAA) // 0xAA) << self.sex) + b
        if b < 0xAA:
            return r
        r += ((b + 0x70E4) // 0x70E4) << self.sex
        return r if b < 0x70E4 else r + (1 << self.sex)

    def block_addr(self, b):
        return (self.cbdbn(b) << 0xC) + self.first_hash

    def l0_backing(self, b):
        if b < 0xAA:
            return 0
        return (b // 0xAA) * self.step[0] + 2

    def l1_backing(self):
        return self.step[0]


def _header_hash(buf, first_hash):
    """Master hash @0x32C = SHA1(0x344 .. first hash table)."""
    buf[0x32C:0x32C + 0x14] = hashlib.sha1(bytes(buf[0x344:first_hash])).digest()


def build(folder, out_path, template):
    """Pack a FOLDER of files into a loadable STFS package, cloning the header
    skeleton (console cert + metadata layout) from an existing `template`
    package and rebuilding the payload, hash tree, file table and volume
    descriptor. Clean-room implementation of the documented STFS write flow.

    The package is rehashed here but keeps the template's console signature. It
    loads on emulators / RGH-JTAG as-is; to bind it to a specific console (and
    satisfy a stock retail console), follow with `resign` using that console's
    keyvault.
    """
    tpl = template if isinstance(template, (bytes, bytearray)) else open(template, "rb").read()
    if tpl[:4] not in MAGICS:
        raise ValueError("template is not an STFS package")
    first_hash = (struct.unpack_from(">I", tpl, 0x340)[0] + 0xFFF) & 0xFFFFF000
    header = bytearray(tpl[:first_hash])

    inner = []
    if os.path.isdir(folder):
        for nm in sorted(os.listdir(folder)):
            p = os.path.join(folder, nm)
            if os.path.isfile(p):
                inner.append((nm, open(p, "rb").read()))
    else:
        inner.append((os.path.basename(folder), open(folder, "rb").read()))
    if not inner:
        raise ValueError("no files to pack in %r" % folder)

    # assign data blocks: file table at block 0.., then each file's content
    ft_nblocks = max(1, (len(inner) + 0x3F) // 0x40)
    entries, contents, cur = [], [], ft_nblocks
    for nm, data in inner:
        nb = max(1, (len(data) + 0xFFF) // 0x1000)
        entries.append((nm, cur, nb, len(data)))
        for i in range(nb):
            contents.append(data[i * 0x1000:(i + 1) * 0x1000].ljust(0x1000, b"\x00"))
        cur += nb
    alloc_blocks = cur

    # file-table block(s)
    ftbuf = bytearray(ft_nblocks * 0x1000)
    for i, (nm, start, nb, size) in enumerate(entries):
        b = nm.encode("latin1", "ignore")[:0x28]
        ftbuf[i * 0x40:i * 0x40 + len(b)] = b
        ftbuf[i * 0x40 + 0x28] = len(b)
        struct.pack_into("<I", ftbuf, i * 0x40 + 0x29, nb); ftbuf[i * 0x40 + 0x2B] = 0
        struct.pack_into("<I", ftbuf, i * 0x40 + 0x2C, nb); ftbuf[i * 0x40 + 0x2E] = 0
        struct.pack_into("<I", ftbuf, i * 0x40 + 0x2F, start); ftbuf[i * 0x40 + 0x31] = 0
        struct.pack_into(">H", ftbuf, i * 0x40 + 0x32, 0xFFFF)     # root
        struct.pack_into(">I", ftbuf, i * 0x40 + 0x34, size)
    blocks = [ftbuf[i * 0x1000:(i + 1) * 0x1000] for i in range(ft_nblocks)] + contents

    # next-block chains
    nextb = {}
    for j in range(ft_nblocks):
        nextb[j] = j + 1 if j < ft_nblocks - 1 else 0xFFFFFF
    for nm, start, nb, size in entries:
        for i in range(nb):
            nextb[start + i] = start + i + 1 if i < nb - 1 else 0xFFFFFF

    g = _WriteGeom(alloc_blocks, first_hash)
    max_back = 0
    for b in range(alloc_blocks):
        max_back = max(max_back, g.cbdbn(b), g.l0_backing(b) + 1)
    if g.top_level == 1:
        max_back = max(max_back, g.l1_backing() + 1)
    buf = bytearray(first_hash + (max_back + 2) * 0x1000)
    buf[:len(header)] = header
    buf[:4] = b"CON " if tpl[:4] == b"CON " else tpl[:4]

    for b in range(alloc_blocks):
        off = g.block_addr(b)
        buf[off:off + 0x1000] = blocks[b]

    # L0 hash tables (+ male backup copy)
    ngroups = (alloc_blocks + 0xA9) // 0xAA
    l0_hashes = []
    for grp in range(ngroups):
        base = (g.l0_backing(grp * 0xAA) << 0xC) + first_hash
        for k in range(0xAA):
            b = grp * 0xAA + k
            if b >= alloc_blocks:
                break
            ea = base + k * 0x18
            buf[ea:ea + 0x14] = hashlib.sha1(blocks[b]).digest()
            buf[ea + 0x14] = 0xC0
            nb = nextb.get(b, 0xFFFFFF)
            buf[ea + 0x15] = (nb >> 16) & 0xFF
            buf[ea + 0x16] = (nb >> 8) & 0xFF
            buf[ea + 0x17] = nb & 0xFF
        table = bytes(buf[base:base + 0x1000])
        buf[base + 0x1000:base + 0x2000] = table
        l0_hashes.append(hashlib.sha1(table).digest())

    if g.top_level == 1:
        l1 = (g.l1_backing() << 0xC) + first_hash
        for i, h in enumerate(l0_hashes):
            ea = l1 + i * 0x18
            buf[ea:ea + 0x14] = h
            buf[ea + 0x14] = 0x80
        table = bytes(buf[l1:l1 + 0x1000])
        buf[l1 + 0x1000:l1 + 0x2000] = table
        top_hash = hashlib.sha1(table).digest()
    else:
        top_hash = l0_hashes[0]

    # volume descriptor + sizes (male layout: block separation 0)
    buf[0x379] = 0x24
    buf[0x37B] = 0x00
    struct.pack_into("<H", buf, 0x37C, ft_nblocks)
    buf[0x37E] = buf[0x37F] = buf[0x380] = 0
    buf[0x381:0x381 + 0x14] = top_hash
    struct.pack_into(">I", buf, 0x395, alloc_blocks)
    struct.pack_into(">I", buf, 0x399, 0)
    struct.pack_into(">Q", buf, 0x34C, alloc_blocks * 0x1000)

    _header_hash(buf, first_hash)
    with open(out_path, "wb") as f:
        f.write(bytes(buf))
    return out_path


# --------------------------------------------------------------- resign (CON)
# A decrypted keyvault (KV_dec.bin, 0x4000 bytes) holds the console certificate
# and the console's 1024-bit RSA private key. Offsets are from the documented
# XECRYPT_KEYVAULT layout; the certificate copied into the STFS header is the
# 0x1A8-byte region at KV 0x9C8, and the private key is XECRYPT_RSAPRV_1024 at
# KV 0x298 (cqw, e, reserved, then n/p/q/dp/dq/cr as XeCrypt "BnQw" bignums).
KV_CERT_OFF = 0x9C8
KV_CERT_LEN = 0x1A8
KV_PRV_OFF = 0x298
# PKCS#1 v1.5 DigestInfo prefix for SHA-1
_SHA1_DER = bytes.fromhex("3021300906052b0e03021a05000414")


def _bn(b):
    """Decode a XeCrypt big-number (array of big-endian qwords) to an int."""
    swapped = b"".join(b[i:i + 8][::-1] for i in range(0, len(b), 8))
    return int.from_bytes(swapped, "little")


def _con_sign(region, kv):
    """PKCS#1 v1.5 RSA-SHA1 sign `region` with the KV's console private key.
    Returns the 0x80-byte signature in the byte-reversed form STFS stores."""
    e = struct.unpack_from(">I", kv, KV_PRV_OFF + 4)[0]
    n = _bn(kv[KV_PRV_OFF + 0x10:KV_PRV_OFF + 0x10 + 0x80])
    p = _bn(kv[KV_PRV_OFF + 0x90:KV_PRV_OFF + 0x90 + 0x40])
    q = _bn(kv[KV_PRV_OFF + 0xD0:KV_PRV_OFF + 0xD0 + 0x40])
    d = pow(e, -1, (p - 1) * (q - 1))
    digest_info = _SHA1_DER + hashlib.sha1(region).digest()
    pad = b"\xFF" * (0x80 - len(digest_info) - 3)
    block = b"\x00\x01" + pad + b"\x00" + digest_info
    sig = pow(int.from_bytes(block, "big"), d, n).to_bytes(0x80, "big")
    return sig[::-1], (e, n)


def _con_verify(region, sig, modulus_bnqw, e):
    """Verify a byte-reversed STFS signature against the certificate's public
    modulus (a self-check). The modulus is a XeCrypt BnQw bignum, like the key."""
    n = _bn(modulus_bnqw)
    block = pow(int.from_bytes(sig[::-1], "big"), e, n).to_bytes(0x80, "big")
    digest_info = _SHA1_DER + hashlib.sha1(region).digest()
    return block.endswith(digest_info) and block[:2] == b"\x00\x01"


def resign(package, kv, out_path=None, bind_console=False):
    """Re-sign a package with a console keyvault you own so it will load on that
    console. Copies the console certificate from the KV and RSA-signs the header
    (PKCS#1 v1.5 over SHA1(header[0x22C:0x344])) with the console private key.

    Works on CON packages (saves, profiles, DLC) and CONVERTS Microsoft-signed
    LIVE / PIRS packages to console-signed CON -- a LIVE/PIRS signature cannot be
    forged (that needs Microsoft's key), so the only way to load a modified one is
    to re-sign it as CON, exactly as Velocity / Le Fluffie do. The metadata from
    0x22C on is identical across CON/LIVE/PIRS, so conversion only rewrites the
    0x04..0x22C signature block, the magic, and the license descriptor.

    The certificate copy and the CON signature scheme were verified byte-exact
    against genuine console-signed packages. `bind_console` additionally stamps
    the KV's console id into the content metadata (0x36C) -- off by default so a
    same-console CON resign stays byte-identical to the console's own output.
    Overwrites in place unless `out_path` is given.
    """
    data = bytearray(package if isinstance(package, (bytes, bytearray))
                     else open(package, "rb").read())
    kv = kv if isinstance(kv, (bytes, bytearray)) else open(kv, "rb").read()
    if data[:4] not in MAGICS:
        raise ValueError("not an STFS package (magic %r)" % bytes(data[:4]))
    if len(kv) < 0x4000:
        raise ValueError("keyvault too small (%d bytes); expected a decrypted "
                         "KV_dec.bin of 0x4000 bytes" % len(kv))
    converting = data[:4] != b"CON "
    if converting:
        # LIVE/PIRS -> CON: flip the magic and reset the license descriptor to a
        # single Unrestricted entry (the state console content uses). The cert
        # copy below fills 0x04..0x1AC and the signature fills 0x1AC..0x22C, so
        # the whole Microsoft-signature block is overwritten.
        data[0x00:0x04] = b"CON "
        data[0x22C:0x32C] = b"\x00" * 0x100                # 0x10 license entries
        data[0x22C:0x234] = b"\xFF" * 8                    # license[0] = Unrestricted
    # 1) console certificate -> header 0x04 (also sets cert size 0x1A8 at 0x04)
    data[0x04:0x04 + KV_CERT_LEN] = kv[KV_CERT_OFF:KV_CERT_OFF + KV_CERT_LEN]
    # optional: bind the content to this console (metadata console id @0x36C)
    if bind_console:
        data[0x36C:0x371] = kv[KV_CERT_OFF + 2:KV_CERT_OFF + 7]
    # 2) header hash @0x32C over the current metadata (inside the signed region).
    #    Hash 0x344 up to the first hash table, but never past end-of-file.
    first_hash = (struct.unpack_from(">I", data, 0x340)[0] + 0xFFF) & 0xFFFFF000
    first_hash = min(first_hash, len(data))
    data[0x32C:0x32C + 0x14] = hashlib.sha1(bytes(data[0x344:first_hash])).digest()
    # 3) sign header[0x22C:0x344] with the console private key
    region = bytes(data[0x22C:0x344])
    sig, (e, n) = _con_sign(region, kv)
    data[0x1AC:0x1AC + 0x80] = sig
    # 4) self-check: signature must verify against the certificate's own modulus
    cert_mod = bytes(data[0x2C:0x2C + 0x80])
    cert_exp = struct.unpack_from(">I", data, 0x28)[0]
    if not _con_verify(region, sig, cert_mod, cert_exp):
        raise RuntimeError("internal error: resigned signature failed self-verify")
    dest = out_path or (package if isinstance(package, str) else None)
    if dest:
        with open(dest, "wb") as f:
            f.write(bytes(data))
    return bytes(data)


# ------------------------------------------------------------------------ CLI
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: stfs {info|list|extract} <package> [outdir]")
        print("       stfs pack   <folder> <out.stfs> <template>")
        print("       stfs resign <package> <KV_dec.bin> [out] [--bind]")
        print("              resigns CON, and converts LIVE/PIRS -> CON")
        print("              --bind also stamps the console id into the metadata")
        return
    op = a[0]
    if op == "pack":
        if len(a) < 4:
            print("usage: stfs pack <folder> <out.stfs> <template-package>"); sys.exit(2)
        out = build(a[1], a[2], a[3])
        r = STFS(open(out, "rb").read())
        print("packed %d files -> %s  (reads back: %d files)"
              % (len(r.files), out, len(r.files)))
        return
    if op == "resign":
        bind = "--bind" in a
        a = [x for x in a if x != "--bind"]
        if len(a) < 3:
            print("usage: stfs resign <package> <KV_dec.bin> [out] [--bind]"); sys.exit(2)
        out = a[3] if len(a) > 3 else a[1]
        src_magic = open(a[1], "rb").read(4)
        resign(a[1], a[2], out, bind_console=bind)
        kv = open(a[2], "rb").read()
        cid = kv[KV_CERT_OFF + 2:KV_CERT_OFF + 7].hex().upper()
        print("resigned %s" % os.path.basename(a[1]))
        if src_magic != b"CON ":
            print("  converted  : %s -> CON" % src_magic.decode("latin-1"))
        print("  console id : %s  (from %s)%s"
              % (cid, os.path.basename(a[2]), "  [bound @0x36C]" if bind else ""))
        print("  signature  : verified against console certificate")
        print("  written    : %s" % out)
        return
    if len(a) < 2:
        print("usage: stfs %s <package> [args]" % op); sys.exit(2)
    s = STFS(open(a[1], "rb").read())
    if op == "info":
        print("package     : %s" % os.path.basename(a[1]))
        print("magic       : %s" % s.magic.decode("latin-1"))
        print("title       : %r" % s.title_name)
        print("display name: %r" % s.display_name)
        print("title id    : %s   content type: 0x%X" % (s.title_id, s.content_type))
        print("profile id  : %s" % s.profile_id)
        print("layout      : %s  top-level=%d  alloc_blocks=%d"
              % ("female" if s.sex == 0 else "male", s.top_level, s.alloc_blocks))
        print("files       : %d" % len(s.files))
    elif op == "list":
        for name in sorted(s.files):
            print("  %10d  %s" % (s.files[name][2], name))
        print("total: %d files, %d bytes" % (len(s.files), sum(v[2] for v in s.files.values())))
    elif op == "extract":
        outdir = a[2] if len(a) > 2 else os.path.splitext(a[1])[0] + "_extracted"
        print("extracted %d files -> %s" % (s.extract_all(outdir), outdir))
    else:
        print("unknown op %r (info|list|extract)" % op); sys.exit(2)


if __name__ == "__main__":
    main()
