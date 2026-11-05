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

    to_pe(path)     the program as a 32-bit PE image, for the x86 lane (see below)

The original Xbox runs x86 code, so its programs go down the 32-bit Windows lane once they are
in the shape that lane reads. to_pe writes that image: the same bytes at the same addresses
(each section at its address, the XBE's own header at the base), the entry point, and the
kernel's functions as imports. The kernel thunk table already has the shape of a PE import
address table -- one word per import, the kernel's ordinal with the top bit set, a zero at the
end -- so it becomes the import table of `xboxkrnl.exe`, whose ordinals symbols/ordinal_names.json
names (the kernel's export map). The image is written once per distinct file, named by its
contents.
"""

import hashlib
import os
import sys

from codebreaker.formats import native

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
        self.__dict__.update(native.call("xbe.parse", data))        # (the header and certificate: formats.dll's)

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
        return native.call("xbe.sections", self.d, self.base, self.section_count, self.section_hdr_addr)

    def region_str(self):
        return "|".join(n for m, n in REGION.items() if self.region & m) or "0x%X" % self.region

    def media_str(self):
        return "|".join(n for m, n in MEDIA.items() if self.allowed_media & m) or "0x%X" % self.allowed_media


def to_pe(path):
    """The XBE at `path` as a PE image (written once, kept under state/unpacked/xbe). -> its path"""
    from codebreaker.base import settings
    data = open(path, "rb").read()
    digest = hashlib.sha1(data).hexdigest()[:12]
    out = settings.state("unpacked", "xbe", "%s-%s.exe" % (os.path.splitext(os.path.basename(path))[0].lower(), digest))
    if not os.path.isfile(out):
        with open(out + ".part", "wb") as f:
            f.write(pe_image(XBE(data)))
        os.replace(out + ".part", out)
    return out


def pe_image(x):
    """The bytes of the PE image for an XBE (see the module's notes; formats.dll's)."""
    return native.call("xbe.pe_image", x.d)


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
