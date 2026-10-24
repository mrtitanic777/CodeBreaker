#!/usr/bin/env python3
"""detect.py -- the ONE place that says what kind of executable a file is.

Every MZ-family executable starts with a DOS header whose field at 0x3C (e_lfanew, 4 bytes)
points at the "new" header; its two-byte signature decides the format. This check was written
out separately in each era's target module, the corpus sweeps, the native-decompiler router and
others, with small disagreements (some read e_lfanew as 2 bytes, which mis-locates the header
of any file whose DOS stub is 64 KB or more). Dependency-free, like the rest of formats/.

    signature(path)   -> b"PE" | b"NE" | b"LE" | b"LX" | b"" (MZ, no known new header) | None (not MZ)
    format_of(path)   -> "PE" | "NE" | "LE" | "LX" | "MZ" | None
    lane(path)        -> "pe" | "ne" | "le" | "dos" | None      (the decompiler lane)
    identify(path)    -> [{format, detail, family, command}]: what ANY file is (executables,
                         Xbox containers and assets, disc and disk images, objects, bytecode)
"""
import os
import struct

_KNOWN = (b"PE", b"NE", b"LE", b"LX")


def signature_of_bytes(head, read_at=None):
    """Signature from the first 0x40 bytes. `read_at(offset, n)` fetches the new-header bytes
    when they lie beyond `head`; without it only `head` is consulted."""
    if len(head) < 0x40 or head[:2] != b"MZ":
        return None
    off = struct.unpack_from("<I", head, 0x3C)[0]
    if off + 2 <= len(head):
        sig = head[off:off + 2]
    elif read_at is not None and 0 < off < 0x10000000:
        sig = read_at(off, 2)
    else:
        sig = b""
    return sig if sig in _KNOWN else b""


def signature(path):
    """New-header signature of the file at `path` (see module docstring), or None when the file
    is unreadable or not an MZ executable."""
    try:
        with open(path, "rb") as f:
            head = f.read(0x40)

            def read_at(off, n):
                f.seek(off)
                return f.read(n)
            return signature_of_bytes(head, read_at)
    except OSError:
        return None


def format_of(path):
    """'PE' | 'NE' | 'LE' | 'LX' | 'MZ' (plain DOS executable) | None (not an MZ executable)."""
    sig = signature(path)
    if sig is None:
        return None
    return sig.decode("ascii") if sig else "MZ"


_LANE = {b"PE": "pe", b"NE": "ne", b"LE": "le", b"LX": "le", b"": "dos"}


def lane(path):
    """The decompiler lane for a file: 'pe', 'ne', 'le' (VxD; LE and LX), 'dos', or None."""
    sig = signature(path)
    return None if sig is None else _LANE[sig]


def dos_header(data):
    """The DOS (MZ) header every one of these formats starts with, by field name, with the size of
    the program it loads and its relocation entries; None when `data` is not an MZ executable.
    For a plain DOS program this is the whole description of the file."""
    if len(data) < 0x1C or data[:2] != b"MZ":
        return None
    (last, pages, nrel, hdr, minalloc, maxalloc, ss, sp, checksum, ip, cs, reltab,
     overlay) = struct.unpack_from("<13H", data, 2)
    image = max(0, (pages * 512 - ((512 - last) if last else 0)) - hdr * 16)
    relocs = []
    for i in range(nrel):
        if reltab + i * 4 + 4 > len(data):
            break
        off, seg = struct.unpack_from("<HH", data, reltab + i * 4)
        relocs.append((seg, off))
    return {"bytes_in_last_page": last, "pages": pages, "relocations": nrel, "header_paragraphs": hdr,
            "header_size": hdr * 16, "min_alloc": minalloc, "max_alloc": maxalloc, "ss": ss, "sp": sp,
            "checksum": checksum, "ip": ip, "cs": cs, "relocation_table": reltab, "overlay": overlay,
            "new_header": struct.unpack_from("<I", data, 0x3C)[0] if len(data) >= 0x40 else 0,
            "load_size": min(image, max(0, len(data) - hdr * 16)), "relocation_entries": relocs}


# ---- what any file is ----------------------------------------------------------------------------
# STFS content type (big-endian, header offset 0x344)
_CONTENT = {0x00000001: "saved game", 0x00000002: "marketplace / DLC content", 0x00000003: "publisher",
            0x00001000: "Xbox 360 title (installer)", 0x00002000: "IPTV DVR", 0x00004000: "Xbox 360 title",
            0x00005000: "original Xbox title", 0x00007000: "Games on Demand", 0x00009000: "avatar item",
            0x00010000: "profile", 0x00020000: "gamer picture", 0x00030000: "theme", 0x00040000: "cache file",
            0x00050000: "storage download", 0x00070000: "installer", 0x000B0000: "community game"}
_COFF = {0x014C: "x86", 0x8664: "x86-64", 0x01F2: "PowerPC big-endian (Xbox 360)", 0x01F0: "PowerPC",
         0x01C0: "ARM", 0x01C4: "ARM Thumb-2", 0xAA64: "ARM64"}
# magic at offset 0 -> (format, family, command that reads it)
_MAGIC = [
    (b"XEX2", ("Xbox 360 executable (XEX2)", "xbox360", "xex")),
    (b"XEX1", ("Xbox 360 executable (XEX1, early kit)", "xbox360", "xex")),
    (b"XEX%", ("Xbox 360 executable (XEX%, prototype)", "xbox360", "xex")),
    (b"XBEH", ("original Xbox executable (XBE)", "xbox", "info")),
    (b"FATX", ("FATX filesystem (Xbox drive / memory unit)", "xbox", "media")),
    (b"CISO", ("compressed disc image (CSO)", "xbox", "media")),
    (b"CCIM", ("compressed disc image (CCI)", "xbox", "media")),
    (b"XUIZ", ("XUI package (XZP)", "xbox360", "media")),
    (b"XPR0", ("Xbox resource bundle (XPR0)", "xbox", "info")),
    (b"XPR2", ("Xbox 360 resource bundle (XPR2)", "xbox360", "info")),
    (b"XDBF", ("Xbox 360 database (XDBF: GPD / SPA)", "xbox360", "info")),
    (b"WBND", ("XACT wave bank (XWB, big-endian)", "xbox360", "info")),
    (b"DNBW", ("XACT wave bank (XWB, little-endian)", "xbox", "info")),
    (b"XUIB", ("compiled XUI scene (XUR)", "xbox360", "info")),
    (b"MComprHD", ("CHD compressed disc image", "disc", "info")),
    (b"BIK", ("Bink video", "media", "info")),
    (b"DDS ", ("DirectDraw Surface texture (DDS)", "media", "")),
    (b"MSCF", ("Microsoft cabinet (CAB)", "windows", "cab")),
    (b"\x7fELF", ("ELF executable / object", "unix", "")),
    (b"dex\n", ("Android DEX bytecode", "android", "")),
    (b"\xfe\xed\xfa\xce", ("Mach-O (32-bit)", "apple", "")),
    (b"\xfe\xed\xfa\xcf", ("Mach-O (64-bit)", "apple", "")),
    (b"\xce\xfa\xed\xfe", ("Mach-O (32-bit)", "apple", "")),
    (b"\xcf\xfa\xed\xfe", ("Mach-O (64-bit)", "apple", "")),
    (b"NES\x1a", ("NES ROM (iNES)", "nes", "")),
    (b"\xff\x4f", ("Xbox 360 NAND flash image", "xbox360", "")),
    (b"PK\x03\x04", ("ZIP archive (or .jar / .apk / .xlsx)", "archive", "")),
    (b"RIFF", ("RIFF container (WAV / AVI / XMA)", "media", "info")),
    (b"XNBw", ("XNA asset (XNB, Windows)", "xna", "")),
    (b"XNBx", ("XNA asset (XNB, Xbox 360)", "xna", "")),
    (b"XNBm", ("XNA asset (XNB, Windows Phone)", "xna", "")),
    (b"Microsoft C/C++ MSF 7.00", ("program database (PDB)", "windows", "")),
    (b"Microsoft C/C++ program database 2.00", ("program database (PDB 2.0)", "windows", "")),
    (b"\x89PNG", ("PNG image", "media", "")),
    (b"\xff\xd8\xff", ("JPEG image", "media", "")),
    (b"GIF8", ("GIF image", "media", "")),
    (b"OggS", ("Ogg audio / video", "media", "")),
    (b"fLaC", ("FLAC audio", "media", "")),
    (b"ID3", ("MP3 audio", "media", "")),
]


def _match(**kw):
    out = {"format": "", "detail": "", "family": "", "command": ""}
    out.update(kw)
    return out


def identify(path):
    """What the file at `path` is, from its own bytes. -> [{"format", "detail", "family", "command"}]
    (usually one; empty when nothing is recognised). `command` is the codebreaker command that
    reads it ("" when none does yet). Reads the header and a few fixed offsets, never the file."""
    try:
        size = os.path.getsize(path)
        with open(path, "rb") as f:
            head = f.read(0x400)

            def at(off, n):
                f.seek(off)
                return f.read(n)
            sig = signature_of_bytes(head[:0x40], at) if head[:2] == b"MZ" else None
            if sig is not None:                                  # the MZ family
                if sig == b"PE":
                    e = struct.unpack_from("<I", head, 0x3C)[0]
                    machine = struct.unpack("<H", at(e + 4, 2) or b"\0\0")[0]
                    cpu = _COFF.get(machine, "machine 0x%04X" % machine)
                    fam = "xbox360" if machine == 0x01F2 else "windows"
                    return [_match(format="PE executable", detail=cpu, family=fam, command="info")]
                if sig == b"":                                   # W3 / W4: a bundle of drivers (VMM32)
                    e = struct.unpack_from("<I", head, 0x3C)[0] if len(head) >= 0x40 else 0
                    w = at(e, 2) if 0 < e < size else b""
                    if w in (b"W3", b"W4"):
                        return [_match(format="driver bundle (%s)" % w.decode(), detail="VxDs stored together",
                                       family="windows", command="info")]
                name = {b"NE": "16-bit Windows (NE)", b"LE": "virtual device driver (LE)",
                        b"LX": "OS/2 / VxD (LX)", b"": "DOS program (MZ)"}[sig]
                return [_match(format=name, family="windows" if sig else "dos", command="info")]
            if head[:4] in (b"CON ", b"LIVE", b"PIRS"):
                ct = struct.unpack(">I", at(0x344, 4).rjust(4, b"\0"))[0]
                return [_match(format="STFS package (%s)" % head[:4].decode().strip(),
                               detail=_CONTENT.get(ct, "content type 0x%X" % ct), family="xbox360", command="media")]
            if head[:4] == b"\xca\xfe\xba\xbe":                  # Java class, or a Mach-O universal binary
                n = struct.unpack(">I", head[4:8])[0] if len(head) >= 8 else 0
                if 0 < n < 30:
                    return [_match(format="Mach-O universal binary", detail="%d architectures" % n, family="apple")]
                return [_match(format="Java class file", family="java", command="java")]
            for magic, (fmt, fam, cmd) in _MAGIC:
                if head.startswith(magic):
                    detail = "codec %r" % chr(head[3]) if magic == b"BIK" and len(head) > 3 else ""
                    return [_match(format=fmt, detail=detail, family=fam, command=cmd)]
            for base in (0, 0x10000, 0xFD90000, 0x2080000, 0xFB20000, 0x18300000):
                if base + 0x10000 + 20 <= size and at(base + 0x10000, 20) == b"MICROSOFT*XBOX*MEDIA":
                    return [_match(format="Xbox disc image (XDVDFS)", detail="volume at 0x%X" % base,
                                   family="xbox", command="media")]
            for sector, skip in ((2048, 0), (2352, 16), (2352, 24)):
                if at(16 * sector + skip + 1, 5) == b"CD001":
                    return [_match(format="CD image (ISO-9660)", detail="%d-byte sectors" % sector,
                                   family="disc", command="media")]
            # (55 AA alone is two bytes any file may hold at 510: a FAT boot sector also starts
            # with a jump and gives a sector size; a disk's partition table marks each entry
            # active (80) or not (00))
            if len(head) >= 512 and head[510:512] == b"\x55\xaa" and (
                    head[0] in (0xEB, 0xE9) and struct.unpack_from("<H", head, 11)[0] in (512, 1024, 2048, 4096)
                    or all(head[0x1BE + 16 * i] in (0, 0x80) for i in range(4))
                    and any(head[0x1BE + 16 * i + 4] for i in range(4))):
                kind = "floppy image" if size <= 2949120 else "hard-disk image"
                return [_match(format="disk image (FAT)", detail=kind, family="disc", command="media")]
            if head[:1] in (b"\x80", b"\x82") and len(head) > 3:            # THEADR / LHEADR
                return [_match(format="OMF object", family="dos", command="")]
            if head[:1] == b"\xf0":
                return [_match(format="OMF library", family="dos", command="")]
            if len(head) >= 20:
                machine, nsec = struct.unpack_from("<HH", head, 0)
                if machine in _COFF and 0 < nsec < 200:
                    return [_match(format="COFF object", detail="%s, %d sections" % (_COFF[machine], nsec),
                                   family="xbox360" if machine == 0x01F2 else "windows", command="")]
            if path.lower().endswith(".zar"):
                return [_match(format="ZArchive (ZAR)", family="archive", command="media")]
            if path.lower().endswith(".pck"):
                return [_match(format="Minecraft LCE package (PCK)", family="xbox360", command="media")]
    except OSError:
        return []
    return []
