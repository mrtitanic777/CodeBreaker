"""Original Xbox XBE executable reader — the OG counterpart to xex.py.

Parses the XBE header, certificate (title, title id, region, ratings, media),
section table, and decrypts the XOR-obfuscated entry point and kernel-thunk
address by trying the retail / debug / chihiro keys and keeping whichever lands
inside the image. (OG Xbox XISO discs are already read by `gdf` — its base-offset
scan includes the 0x18300000 redump base.)

XBE header (little-endian):
  0x104 base addr   0x10C size of image   0x118 cert addr(VA)   0x11C section count
  0x120 section headers(VA)   0x128 entry(^key)   0x148 kernel thunk(^key)
Certificate: 0x08 title id  0x0C title name(40 UTF-16LE)  0xA0 region  0xA4 ratings
Section header (0x38): flags, VA, vsize, raw addr, raw size, name VA.
"""

import struct
import sys

MAGIC = b"XBEH"
ENTRY_KEYS = {"retail": 0xA8FC57AB, "debug": 0x94859D4B, "chihiro": 0x40B5C16E}
THUNK_KEYS = {"retail": 0x5B6D40B6, "debug": 0xEFB1F152, "chihiro": 0x2290059D}
REGION = {0x1: "North America", 0x2: "Japan", 0x4: "Rest of World",
          0x80000000: "Manufacturing/Debug"}
MEDIA = {0x1: "HDD", 0x2: "DVD-X2", 0x4: "DVD-CD", 0x8: "CD", 0x10: "DVD-5-RO",
         0x20: "DVD-9-RO", 0x40: "DVD-5-RW", 0x80: "DVD-9-RW", 0x100: "Dongle",
         0x200: "Media Board", 0x40000000: "Nonsecure Hard Disk",
         0x80000000: "Nonsecure Mode"}


class XBE:
    def __init__(self, data):
        self.d = data
        if data[:4] != MAGIC:
            raise ValueError("not an XBE (magic %r)" % data[:4])
        self.base = struct.unpack_from("<I", data, 0x104)[0]
        self.size_of_image = struct.unpack_from("<I", data, 0x10C)[0]
        self.cert_addr = struct.unpack_from("<I", data, 0x118)[0]
        self.section_count = struct.unpack_from("<I", data, 0x11C)[0]
        self.section_hdr_addr = struct.unpack_from("<I", data, 0x120)[0]
        self.init_flags = struct.unpack_from("<I", data, 0x124)[0]
        self._entry_raw = struct.unpack_from("<I", data, 0x128)[0]
        self._thunk_raw = struct.unpack_from("<I", data, 0x148)[0]
        self._parse_cert()

    def _off(self, va):
        return va - self.base

    def _wstr(self, off, wchars):
        return self.d[off:off + wchars * 2].decode("utf-16-le", "replace").split("\x00", 1)[0]

    def _parse_cert(self):
        c = self._off(self.cert_addr)
        self.cert_size = struct.unpack_from("<I", self.d, c)[0]
        self.title_id = struct.unpack_from("<I", self.d, c + 0x08)[0]
        self.title_name = self._wstr(c + 0x0C, 40)
        self.allowed_media = struct.unpack_from("<I", self.d, c + 0x9C)[0]
        self.region = struct.unpack_from("<I", self.d, c + 0xA0)[0]
        self.ratings = struct.unpack_from("<I", self.d, c + 0xA4)[0]
        self.disk_number = struct.unpack_from("<I", self.d, c + 0xA8)[0]
        self.version = struct.unpack_from("<I", self.d, c + 0xAC)[0]

    def title_id_str(self):
        """Title id -> 'AA-nnn' (two publisher letters + 3-digit game number)."""
        pub = (self.title_id >> 16) & 0xFFFF
        num = self.title_id & 0xFFFF
        a, b = (pub >> 8) & 0xFF, pub & 0xFF
        pubs = ("%c%c" % (a, b)) if 0x20 <= a < 0x7F and 0x20 <= b < 0x7F else "%04X" % pub
        return "%s-%03d" % (pubs, num)

    def _decrypt(self, raw, keys):
        for name, k in keys.items():
            v = raw ^ k
            if self.base <= v < self.base + self.size_of_image + 0x100000:
                return v, name
        return raw ^ keys["retail"], "unknown"

    def entry_point(self):
        return self._decrypt(self._entry_raw, ENTRY_KEYS)

    def kernel_thunk(self):
        return self._decrypt(self._thunk_raw, THUNK_KEYS)

    def sections(self):
        out = []
        for i in range(self.section_count):
            o = self._off(self.section_hdr_addr) + i * 0x38
            flags, va, vsize, raw_addr, raw_size, name_va = struct.unpack_from("<IIIIII", self.d, o)
            name = ""
            if name_va:
                no = self._off(name_va)
                end = self.d.find(b"\x00", no)
                name = self.d[no:end].decode("latin-1", "replace")
            out.append({"name": name, "flags": flags, "vaddr": va, "vsize": vsize,
                        "raw_addr": raw_addr, "raw_size": raw_size})
        return out

    def region_str(self):
        return "|".join(n for m, n in REGION.items() if self.region & m) or "0x%X" % self.region

    def media_str(self):
        return "|".join(n for m, n in MEDIA.items() if self.allowed_media & m) or "0x%X" % self.allowed_media


DESCRIPTION = "read an Original Xbox XBE executable: header/certificate/sections + entry decrypt"


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: xbe {info|sections} <default.xbe>"); return
    op = a[0] if a[0] in ("info", "sections") else "info"
    path = a[1] if a[0] in ("info", "sections") else a[0]
    x = XBE(open(path, "rb").read())
    if op == "sections":
        for s in x.sections():
            print("  %-12s va=0x%08X vsize=%-8d raw=0x%06X size=%-8d flags=0x%X" % (
                s["name"], s["vaddr"], s["vsize"], s["raw_addr"], s["raw_size"], s["flags"]))
        print("total: %d sections" % x.section_count)
        return
    ep, epk = x.entry_point()
    kt, ktk = x.kernel_thunk()
    print("magic        : XBEH")
    print("title        : %r" % x.title_name)
    print("title id     : 0x%08X  (%s)" % (x.title_id, x.title_id_str()))
    print("base / size  : 0x%08X / %d bytes" % (x.base, x.size_of_image))
    print("entry point  : 0x%08X  (%s key)" % (ep, epk))
    print("kernel thunk : 0x%08X  (%s key)" % (kt, ktk))
    print("region       : %s" % x.region_str())
    print("media        : %s" % x.media_str())
    print("ratings      : 0x%X   version: %d   disk: %d" % (x.ratings, x.version, x.disk_number))
    print("sections     : %d" % x.section_count)


if __name__ == "__main__":
    main()
