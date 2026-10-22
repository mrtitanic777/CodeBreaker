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
        self.size_of_headers = struct.unpack_from("<I", data, 0x108)[0]
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
    """The bytes of the PE image for an XBE (see the module's notes)."""
    base, size = x.base, x.size_of_image
    sections = [s for s in x.sections() if s["vsize"] or s["raw_size"]]
    entry, _k = x.entry_point()
    thunk, _k = x.kernel_thunk()
    image = bytearray(max(size, max((s["vaddr"] - base + max(s["vsize"], s["raw_size"]) for s in sections), default=0)))
    head = min(len(x.d), x.size_of_headers, len(image))
    image[:head] = x.d[:head]                                     # (overwritten below by the PE headers)
    for s in sections:
        raw = x.d[s["raw_addr"]:s["raw_addr"] + min(s["raw_size"], s["vsize"] or s["raw_size"])]
        at = s["vaddr"] - base
        image[at:at + len(raw)] = raw
    # the import directory: one descriptor for the kernel, its name, in a section of its own
    imp = (len(image) + 0xFFF) & ~0xFFF
    name = b"xboxkrnl.exe\0"
    thunk_rva = thunk - base
    directory = struct.pack("<5I", thunk_rva, 0, 0, imp + 40, thunk_rva) + bytes(20) + name
    count = 0
    while thunk_rva + 4 * count + 4 <= len(image) and struct.unpack_from("<I", image, thunk_rva + 4 * count)[0]:
        count += 1
    image += bytes(imp - len(image)) + directory
    image += bytes((-len(image)) % 0x1000)
    names = [s["name"][:8].encode("latin-1", "replace") for s in sections] + [b".cbimp"]
    spans = [(s["vaddr"] - base, max(s["vsize"], s["raw_size"]), s["flags"]) for s in sections]
    spans.append((imp, len(directory), 0))
    nsec = len(spans)
    headers = 0x40 + 4 + 20 + 224 + 40 * nsec
    if sections and headers > min(sp[0] for sp in spans):
        raise ValueError("the XBE's first section starts too low for PE headers")
    pe = bytearray(headers)
    pe[0:2] = b"MZ"
    struct.pack_into("<I", pe, 0x3C, 0x40)
    pe[0x40:0x44] = b"PE\0\0"
    struct.pack_into("<HHIIIHH", pe, 0x44, 0x014C, nsec, 0, 0, 0, 224, 0x0102)
    o = 0x58
    struct.pack_into("<HBBIIIIIIIIIHHHHHHIIIIHHIIIIII", pe, o, 0x10B, 0, 0, 0, 0, 0, entry - base, 0, 0,
                     base, 0x20, 0x20, 4, 0, 0, 0, 4, 0, 0, len(image), headers, 0, 14, 0,
                     0x100000, 0x1000, 0x100000, 0x1000, 0, 16)
    struct.pack_into("<II", pe, o + 96 + 8, imp, len(directory))                       # import directory
    struct.pack_into("<II", pe, o + 96 + 12 * 8, thunk_rva, 4 * (count + 1))           # import address table
    for i, ((rva, vsize, flags), nm) in enumerate(zip(spans, names)):
        chars = 0x40000040 | (0x20000020 if flags & 0x4 else 0) | (0x80000000 if flags & 0x1 else 0)
        if nm == b".cbimp":
            chars = 0x40000040
        struct.pack_into("<8sIIIIIIHHI", pe, 0x58 + 224 + 40 * i, nm, vsize, rva, vsize, rva, 0, 0, 0, 0, chars)
    image[:headers] = pe
    return bytes(image)


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
