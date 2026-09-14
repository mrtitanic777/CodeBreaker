#!/usr/bin/env python3
"""resview.py -- render PE/NE resource BODIES, not just list them: bitmaps and icons
as displayable images, VERSION info and string tables as text, manifests/HTML/RCDATA
as text, everything else as a hex dump. Backs the EXE Explorer's resource previewer.

    from src.formats import resview
    kind = resview.preview_kind(type_id)         # 'image'|'text'|'hex'
    blob = resview.render(path, foff, size, type_id)  # -> {kind, content_type?, bytes?, text?}
"""
import struct

_u16 = lambda d, o: struct.unpack_from("<H", d, o)[0]
_u32 = lambda d, o: struct.unpack_from("<I", d, o)[0]

# resource type ids (Win32 RT_*)
RT_CURSOR, RT_BITMAP, RT_ICON, RT_MENU, RT_DIALOG, RT_STRING = 1, 2, 3, 4, 5, 6
RT_RCDATA, RT_GROUP_CURSOR, RT_GROUP_ICON, RT_VERSION = 10, 12, 14, 16
RT_MANIFEST, RT_HTML = 24, 23

_IMAGE = {RT_BITMAP, RT_ICON, RT_CURSOR}
_TEXT = {RT_VERSION, RT_STRING, RT_MANIFEST, RT_HTML, RT_RCDATA}


def preview_kind(type_id):
    if type_id in _IMAGE:
        return "image"
    if type_id in _TEXT:
        return "text"
    return "hex"


def _get(path, foff, size):
    with open(path, "rb") as f:
        f.seek(foff)
        return f.read(size)


def render(path, foff, size, type_id):
    try:
        data = _get(path, foff, size)
    except OSError as e:
        return {"kind": "hex", "text": "read error: %s" % e}
    if not data:
        return {"kind": "hex", "text": "(empty)"}
    try:
        if type_id == RT_BITMAP:
            return {"kind": "image", "content_type": "image/bmp", "bytes": _bmp(data)}
        if type_id == RT_ICON:
            return {"kind": "image", "content_type": "image/x-icon", "bytes": _ico(data, 1)}
        if type_id == RT_CURSOR:
            return {"kind": "image", "content_type": "image/x-icon", "bytes": _ico(data, 2, cursor=True)}
        if type_id == RT_VERSION:
            return {"kind": "text", "text": _version(data)}
        if type_id == RT_STRING:
            return {"kind": "text", "text": _stringtable(data)}
        if type_id in (RT_MANIFEST, RT_HTML):
            return {"kind": "text", "text": data.decode("utf-8", "replace")}
        if type_id == RT_RCDATA:
            return {"kind": "text", "text": _maybe_text(data)}
    except Exception as e:
        return {"kind": "hex", "text": "decode error: %s\n\n%s" % (e, _hex(data))}
    return {"kind": "hex", "text": _hex(data)}


# ---- BITMAP: a DIB (no file header) -> wrap in a BITMAPFILEHEADER -------------
def _bmp(dib):
    if len(dib) < 4:
        return dib
    bisize = _u32(dib, 0)
    bitcount = _u16(dib, 14) if len(dib) >= 16 else 0
    clr_used = _u32(dib, 32) if len(dib) >= 36 else 0
    if bitcount <= 8:
        pal = clr_used or (1 << bitcount)
        pixoff = 14 + bisize + pal * 4
    else:
        pixoff = 14 + bisize
    fh = b"BM" + struct.pack("<IHHI", 14 + len(dib), 0, 0, pixoff)
    return fh + dib


# ---- ICON/CURSOR: a single image (DIB or PNG) -> wrap in an ICO/CUR container -
def _ico(img, restype, cursor=False):
    if img[:8] == b"\x89PNG\r\n\x1a\n":              # PNG-compressed icon (modern)
        w = h = 0; bitcount = 32
    else:
        bisize = _u32(img, 0)
        w = _u32(img, 4)
        h = _u32(img, 8) // 2                         # DIB height is doubled (XOR+AND masks)
        bitcount = _u16(img, 14)
    hdr = struct.pack("<HHH", 0, restype, 1)         # ICONDIR: reserved, type, count
    entry = struct.pack("<BBBBHHII",
                        w & 0xFF, h & 0xFF, 0, 0,
                        1 if not cursor else 0, bitcount if not cursor else 0,
                        len(img), 6 + 16)
    return hdr + entry + img


# ---- VERSION: VS_VERSIONINFO -> fixed info + StringFileInfo -------------------
def _version(data):
    out = []
    # VS_FIXEDFILEINFO signature 0xFEEF04BD
    sig = data.find(b"\xbd\x04\xef\xfe")
    if sig >= 0 and sig + 52 <= len(data):
        ms = _u32(data, sig + 8); ls = _u32(data, sig + 12)      # file version
        pms = _u32(data, sig + 16); pls = _u32(data, sig + 20)   # product version
        out.append("FileVersion    %d.%d.%d.%d" % (ms >> 16, ms & 0xFFFF, ls >> 16, ls & 0xFFFF))
        out.append("ProductVersion %d.%d.%d.%d" % (pms >> 16, pms & 0xFFFF, pls >> 16, pls & 0xFFFF))
        out.append("")
    # StringFileInfo: pull the UTF-16 key/value pairs heuristically
    keys = ["CompanyName", "FileDescription", "FileVersion", "InternalName",
            "LegalCopyright", "OriginalFilename", "ProductName", "ProductVersion",
            "Comments", "LegalTrademarks", "PrivateBuild", "SpecialBuild"]
    txt = data.decode("utf-16-le", "replace")
    for k in keys:
        i = txt.find(k)
        if i < 0:
            continue
        j = i + len(k)
        while j < len(txt) and txt[j] in "\x00":     # skip padding to value
            j += 1
        end = txt.find("\x00", j)
        val = txt[j:end if end > 0 else j + 80].strip("\x00").strip()
        if val and val != k:
            out.append("%-17s%s" % (k, val))
    return "\n".join(out) if out else "(no version fields decoded)"


# ---- STRING table: 16 UTF-16 length-prefixed strings per block ---------------
def _stringtable(data):
    out = []
    o = 0
    idx = 0
    while o + 2 <= len(data) and idx < 16:
        ln = _u16(data, o); o += 2
        if ln:
            s = data[o:o + ln * 2].decode("utf-16-le", "replace")
            out.append("[%2d] %s" % (idx, s.replace("\r", " ").replace("\n", " ")))
            o += ln * 2
        idx += 1
    return "\n".join(out) if out else "(empty string block)"


def _maybe_text(data):
    printable = sum(1 for b in data[:256] if 9 <= b <= 13 or 32 <= b < 127)
    if printable > len(data[:256]) * 0.85:
        return data.decode("latin1", "replace")
    return _hex(data)


def _hex(data, limit=4096):
    d = data[:limit]
    lines = []
    for i in range(0, len(d), 16):
        c = d[i:i + 16]
        lines.append("%08X  %-47s  %s" % (i, " ".join("%02X" % b for b in c),
                     "".join(chr(b) if 32 <= b < 127 else "." for b in c)))
    if len(data) > limit:
        lines.append("... (%d more bytes)" % (len(data) - limit))
    return "\n".join(lines)
