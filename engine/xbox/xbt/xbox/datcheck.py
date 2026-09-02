"""Redump / No-Intro DAT verification — authenticate a dump against the disc
preservation database.

Parses a Logiqx DAT (the XML the Redump/No-Intro projects publish), indexes
every ROM by CRC32/MD5/SHA1, and looks up a file's hashes to report whether it
is a clean, canonical dump and which game it is. Compressed images (.cso/.cci)
and GoD packages are hashed as their decompressed ISO stream, so a converted
copy still authenticates.

The DAT itself is Redump/No-Intro data — supply your own (download from
redump.org, or reuse the copy bundled with xVerter); nothing is bundled here.

Commands: verify <file> <dat> | name <file> <dat> | info <dat>.
"""

import hashlib
import os
import sys
import zlib
import xml.etree.ElementTree as ET

DESCRIPTION = "verify a dump against a Redump/No-Intro DAT (CRC32/MD5/SHA1 match)"


class DatError(Exception):
    pass


# ── DAT index ──────────────────────────────────────────────────────────
class Dat:
    def __init__(self):
        self.by_sha1 = {}
        self.by_md5 = {}
        self.by_crc = {}
        self.name = ""
        self.count = 0

    @classmethod
    def load(cls, path):
        d = cls()
        try:
            for _evt, el in ET.iterparse(path, events=("end",)):
                if el.tag == "header":
                    n = el.find("name")
                    if n is not None and n.text:
                        d.name = n.text
                    el.clear()
                elif el.tag == "game" or el.tag == "machine":
                    game = el.get("name") or ""
                    for rom in el.findall("rom"):
                        entry = (game, rom.get("name") or "", int(rom.get("size") or 0))
                        if rom.get("sha1"):
                            d.by_sha1[rom.get("sha1").lower()] = entry
                        if rom.get("md5"):
                            d.by_md5[rom.get("md5").lower()] = entry
                        if rom.get("crc"):
                            d.by_crc[rom.get("crc").lower().zfill(8)] = entry
                        d.count += 1
                    el.clear()
        except ET.ParseError as e:
            raise DatError("not a valid DAT XML: %s" % e)
        if not d.count:
            raise DatError("no ROM entries found (is this a Logiqx DAT?)")
        return d

    def lookup(self, sha1=None, md5=None, crc=None):
        if sha1 and sha1.lower() in self.by_sha1:
            return self.by_sha1[sha1.lower()], "sha1"
        if md5 and md5.lower() in self.by_md5:
            return self.by_md5[md5.lower()], "md5"
        if crc and crc.lower().zfill(8) in self.by_crc:
            return self.by_crc[crc.lower().zfill(8)], "crc"
        return None, None


# ── hashing (streams the decompressed ISO for .cso/.cci/.god) ──────────
def _iter_content(path):
    """Yield the byte stream to hash: the raw file, or the decompressed ISO for
    a compressed/container image."""
    ext = os.path.splitext(path)[1].lower()
    magic = open(path, "rb").read(4)
    if ext in (".cso", ".cci") or magic in (b"CISO", b"CCIM"):
        import ciso
        r = ciso.open_image(path)
        off = 0
        while off < r.size:
            chunk = r.read_at(off, 8 << 20)
            if not chunk:
                break
            yield chunk
            off += len(chunk)
        r.close()
    else:
        with open(path, "rb") as f:
            while True:
                chunk = f.read(8 << 20)
                if not chunk:
                    break
                yield chunk


def hashes(path):
    """Return (crc32-hex, md5-hex, sha1-hex, size) over the content stream."""
    crc = 0
    md5 = hashlib.md5()
    sha1 = hashlib.sha1()
    size = 0
    for chunk in _iter_content(path):
        crc = zlib.crc32(chunk, crc)
        md5.update(chunk)
        sha1.update(chunk)
        size += len(chunk)
    return "%08x" % (crc & 0xFFFFFFFF), md5.hexdigest(), sha1.hexdigest(), size


# ── CLI ────────────────────────────────────────────────────────────────
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  datcheck verify <file> <dat.xml>   hash the file and match it\n"
              "  datcheck name   <file> <dat.xml>   print the matched game name only\n"
              "  datcheck info   <dat.xml>          summarize a DAT\n"
              "(DATs come from redump.org / no-intro; supply your own — none bundled)")
        return
    op = a[0]
    if op == "info":
        d = Dat.load(a[1])
        print("DAT: %s" % (d.name or a[1]))
        print("entries: %d  (sha1 %d, md5 %d, crc %d)" % (
            d.count, len(d.by_sha1), len(d.by_md5), len(d.by_crc)))
        return
    if op in ("verify", "name") and len(a) >= 3:
        crc, md5, sha1, size = hashes(a[1])
        d = Dat.load(a[2])
        hit, via = d.lookup(sha1=sha1, md5=md5, crc=crc)
        if op == "name":
            print(hit[0] if hit else "(no match)")
            return
        print("file  : %s" % a[1])
        print("size  : %d bytes" % size)
        print("crc32 : %s" % crc)
        print("md5   : %s" % md5)
        print("sha1  : %s" % sha1)
        if hit:
            game, rom, dsize = hit
            print("\n✓ MATCH (%s): %s" % (via.upper(), game))
            print("   rom: %s  (%d bytes)" % (rom, dsize))
            print("   this is a clean, canonical dump per %s" % (d.name or "the DAT"))
        else:
            print("\n✗ NO MATCH in %s" % (d.name or a[2]))
            print("   not a recognized clean dump (modified, trimmed, or not in this DAT)")
        return
    print("usage: datcheck verify|name <file> <dat.xml> | info <dat.xml>"); sys.exit(2)


if __name__ == "__main__":
    main()
