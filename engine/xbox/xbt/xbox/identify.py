"""Format auto-identifier — drop any file, learn what it is.

Recognises the Xbox / Xbox 360 formats this toolbox handles by magic bytes and a
few structural markers, and points at the command that works with each. Pure
detection; reads only the header (and one probe at the XGD media offset), never
the whole file.
"""

import struct
import sys

DESCRIPTION = "identify an Xbox/Xbox 360 file by format (magic + structure) and name the tool for it"

# STFS content-type codes -> friendly names (offset 0x344, big-endian)
_CONTENT_TYPES = {
    0x00000001: "Saved Game", 0x00000002: "Marketplace/DLC content",
    0x00000003: "Publisher", 0x00001000: "Xbox 360 Title (installer)",
    0x00002000: "IPTV DVR", 0x00004000: "Xbox 360 Title", 0x00005000: "Xbox Original",
    0x00007000: "Games on Demand", 0x00009000: "Avatar Item",
    0x00010000: "Profile", 0x00020000: "Gamer Picture", 0x00030000: "Theme",
    0x00040000: "Cache File", 0x00050000: "Storage Download",
    0x00070000: "Installer", 0x000B0000: "Community Game",
}

# COFF machine ids
_MACHINE = {0x01F2: "PowerPC big-endian (Xbox 360)", 0x014C: "x86",
            0x8664: "x86-64", 0x01F0: "PowerPC little-endian"}

# 4-byte magics at offset 0 -> (format, tool)
_MAGIC4 = {
    b"XEX2": ("Xbox 360 executable (XEX2)", "xex"),
    b"XEX1": ("Xbox 360 executable (XEX1, early kit)", "xex"),
    b"XBEH": ("Original Xbox executable (XBE)", "xbe"),
    b"FATX": ("FATX filesystem partition", "fatx"),
    b"CISO": ("Compressed ISO (CSO / CISO)", "ciso"),
    b"CCIM": ("Compressed ISO (CCI)", "ciso"),
    b"XPR0": ("Xbox resource bundle (XPR0, original Xbox)", "xpr"),
    b"XPR2": ("Xbox 360 resource bundle (XPR2)", "xpr"),
    b"XDBF": ("Xbox 360 database file (XDBF: GPD/SPA)", "xdbf"),
    b"DDS ": ("DirectDraw Surface texture (DDS)", "texture"),
    b"WBND": ("XACT wave bank (XWB, big-endian)", "xwb"),
    b"DNBW": ("XACT wave bank (XWB, little-endian)", "xwb"),
    b"RIFF": ("RIFF container (WAV / XMA2)", "xma"),
}
_STFS = {b"CON ": "CON (console-signed)", b"LIVE": "LIVE (Xbox Live-signed)",
         b"PIRS": "PIRS (Microsoft-signed)"}


def identify(data):
    """Return a list of (format, detail, tool) matches (usually one). `data` may
    be a path, bytes, or a file-like header. Reads only what it needs."""
    if isinstance(data, str):
        with open(data, "rb") as f:
            head = f.read(0x40)
            f.seek(0x10000)
            xgd = f.read(20)
    else:
        head = bytes(data[:0x40])
        xgd = bytes(data[0x10000:0x10000 + 20]) if len(data) >= 0x10000 + 20 else b""

    m4 = head[:4]
    out = []

    if m4 in _STFS:
        ct = struct.unpack_from(">I", head, 0x344)[0] if len(head) >= 0x348 else None
        # header is longer than 0x40; re-read the content-type field if needed
        if isinstance(data, str):
            with open(data, "rb") as f:
                f.seek(0x344)
                ctb = f.read(4)
            ct = struct.unpack(">I", ctb)[0] if len(ctb) == 4 else None
        elif len(data) >= 0x348:
            ct = struct.unpack_from(">I", data, 0x344)[0]
        name = _CONTENT_TYPES.get(ct, "content type 0x%X" % ct if ct is not None else "?")
        out.append(("STFS package", "%s -> %s" % (_STFS[m4], name), "stfs"))
        return out

    if m4 in _MAGIC4:
        fmt, tool = _MAGIC4[m4]
        detail = ""
        if m4 in (b"XPR0", b"XPR2"):
            detail = "resource bundle"
        out.append((fmt, detail, tool))
        return out

    if head[:8] == b"MComprHD":
        ver = struct.unpack_from(">I", head, 12)[0] if len(head) >= 16 else 0
        out.append(("CHD (MAME/redump compressed disc)", "version %d" % ver, "chd"))
        return out

    if head[:3] == b"BIK":
        out.append(("Bink video (RAD Game Tools)", "codec '%s'" % chr(head[3]), "bink"))
        return out

    # ZArchive: two 32-bit magics 0x169F52D6 / 0x61BF3A01 in the header
    if len(head) >= 8:
        w0, w1 = struct.unpack_from(">II", head, 0)
        if 0x169F52D6 in (w0, w1) or 0x61BF3A01 in (w0, w1):
            out.append(("ZArchive (ZAR)", "zstd disc archive", "zar"))
            return out

    # XGD disc image: MICROSOFT*XBOX*MEDIA at the volume descriptor
    if xgd == b"MICROSOFT*XBOX*MEDIA":
        out.append(("Xbox/Xbox 360 disc image (XISO / XGD, XDVDFS)", "media @0x10000", "gdf"))
        return out

    # Xbox 360 NAND flash image: bootloader header 0xFF 0x4F at offset 0
    if head[:2] == b"\xFF\x4F":
        # CB (2BL) offset lives at 0x08 (big-endian) in the flash header
        cb = struct.unpack_from(">I", head, 8)[0] if len(head) >= 12 else 0
        out.append(("Xbox 360 NAND flash image", "CB bootloader @0x%X" % cb, "nand"))
        return out

    # PE / COFF (check machine word)
    if head[:2] == b"MZ":
        out.append(("PE / DOS executable (MZ)", "Windows/DOS image", "pedump"))
        return out
    if len(head) >= 20:
        machine, nsec = struct.unpack_from("<HH", head, 0)
        if machine in _MACHINE and 0 < nsec < 200:
            tool = "ppcobj" if machine == 0x01F2 else "pedump"
            out.append(("COFF object", "machine: %s, %d sections" % (_MACHINE[machine], nsec), tool))
            return out

    return out


def _selfcheck():
    cases = [
        (b"XEX2" + b"\x00" * 60, "Xbox 360 executable"),
        (b"FATX" + b"\x00" * 60, "FATX"),
        (b"CISO" + b"\x00" * 60, "Compressed ISO"),
        (b"XPR2" + b"\x00" * 60, "resource bundle"),
        (b"MComprHD" + b"\x00" * 8 + struct.pack(">I", 5), "CHD"),
        (b"BIKi" + b"\x00" * 60, "Bink"),
    ]
    for blob, expect in cases:
        got = identify(blob)
        if not got or expect not in (got[0][0] + got[0][1]):
            return False, "%r -> %r (wanted %r)" % (blob[:4], got, expect)
    # a CON package with a Games-on-Demand content type
    con = bytearray(0x400)
    con[0:4] = b"CON "
    struct.pack_into(">I", con, 0x344, 0x7000)
    g = identify(bytes(con))
    if not g or "Games on Demand" not in g[0][1]:
        return False, "STFS content-type decode failed: %r" % g
    # a NAND flash image (0xFF4F header, CB offset at 0x08)
    nand = b"\xFF\x4F\x00\x00\x00\x00\x00\x00" + struct.pack(">I", 0x8000) + b"\x00" * 16
    n = identify(nand)
    if not n or "NAND" not in n[0][0]:
        return False, "NAND header detect failed: %r" % n
    # a PowerPC COFF object (machine 0x01F2, 1 section)
    obj = struct.pack("<HH", 0x01F2, 1) + b"\x00" * 40
    o = identify(obj)
    if not o or "PowerPC" not in o[0][1]:
        return False, "COFF machine detect failed: %r" % o
    # XGD disc: media magic at 0x10000
    disc = bytearray(0x10000 + 20)
    disc[0x10000:0x10000 + 20] = b"MICROSOFT*XBOX*MEDIA"
    d = identify(bytes(disc))
    if not d or "disc image" not in d[0][0]:
        return False, "XGD disc detect failed: %r" % d
    return True, "8 magics + STFS content-type + COFF machine + XGD media + NAND header"


def main():
    a = sys.argv[1:]
    if a and a[0] == "--selftest":
        ok, msg = _selfcheck()
        print(("PASS: " if ok else "FAIL: ") + msg)
        sys.exit(0 if ok else 1)
    if not a or a[0] in ("-h", "--help"):
        print("usage: identify <file> [<file> ...]   auto-detect Xbox/360 file formats")
        return
    for path in a:
        matches = identify(path)
        if not matches:
            print("%s: unrecognised (no known magic)" % path)
            continue
        for fmt, detail, tool in matches:
            tail = ("  [%s]" % detail) if detail else ""
            print("%s: %s%s   -> try:  %s" % (path, fmt, tail, tool))


if __name__ == "__main__":
    main()
