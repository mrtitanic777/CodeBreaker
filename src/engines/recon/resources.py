#!/usr/bin/env python3
r"""resources.py -- reconstruct a PE's resources (.rc source) from its .rsrc section.

A large fraction of a Windows app's "source" is resources -- dialogs, menus, string tables,
accelerators, version info -- not code, and no other engine touches them. This walks the PE
resource directory tree (type/name/language) and reconstructs what it can back to .rc:
  * STRINGTABLE   -- fully decoded (RT_STRING bundles of 16 UTF-16 strings)
  * VERSIONINFO   -- VS_VERSION_INFO fully decoded: the fixed VS_FIXEDFILEINFO version numbers
                     + the StringFileInfo key/value block (CompanyName, ProductName, ...)
  * BITMAP/ICON/CURSOR -- extracted back to real .bmp/.ico/.cur files (RT_BITMAP is a headerless
                     DIB -> prepend BITMAPFILEHEADER; GROUP_ICON/GROUP_CURSOR + their RT_ICON/
                     RT_CURSOR members -> reassembled .ico/.cur), when an out dir is given
  * every other type (DIALOG/MENU/ACCELERATOR/...) is inventoried with id + size
so the resource surface is enumerated and the text resources become real source.
"""
import os
import struct

_RT = {1: "CURSOR", 2: "BITMAP", 3: "ICON", 4: "MENU", 5: "DIALOG", 6: "STRING",
       7: "FONTDIR", 8: "FONT", 9: "ACCELERATOR", 10: "RCDATA", 11: "MESSAGETABLE",
       12: "GROUP_CURSOR", 14: "GROUP_ICON", 16: "VERSION", 24: "MANIFEST"}


def _dir_entries(data, base, off):
    """Parse one IMAGE_RESOURCE_DIRECTORY at base+off -> [(id_or_name, entry_offset, is_dir)]."""
    p = base + off
    nnamed, nid = struct.unpack_from("<HH", data, p + 12)
    out = []
    p += 16
    for _ in range(nnamed + nid):
        name, eoff = struct.unpack_from("<II", data, p); p += 8
        ident = ("#%d" % (name & 0x7FFFFFFF)) if not (name & 0x80000000) else _name_str(data, base, name)
        out.append((name, ident, eoff & 0x7FFFFFFF, bool(eoff & 0x80000000)))
    return out


def _name_str(data, base, name):
    p = base + (name & 0x7FFFFFFF)
    n = struct.unpack_from("<H", data, p)[0]
    return data[p + 2:p + 2 + n * 2].decode("utf-16le", "replace")


def _esc(s):
    return s.replace("\\", "\\\\").replace('"', '""').replace("\n", "\\n").replace("\r", "").replace("\t", "\\t")


def reconstruct(module, out_dir=None):
    from src.core import cores
    from src.formats.pe import PEFile
    path = cores.get("target").resolve(module)
    pe = PEFile(path)
    if len(pe.datadirs) < 3 or not pe.datadirs[2][0]:
        return {"ok": False, "reason": "no resource directory"}
    res_rva = pe.datadirs[2][0]
    base = pe._rva_to_off(res_rva)
    if base is None:
        return {"ok": False, "reason": "resource dir not in a section"}
    d = pe.data
    inv = {}                      # type name -> [(id, size)]
    strings = {}                  # string id -> text
    blobs = {}                    # type name -> {resource id: raw bytes}  (for binary resources)
    versions = []                 # decoded VS_VERSION_INFO dicts
    for tname, tid, toff, tdir in _dir_entries(d, base, 0):
        typ = _RT.get(tname & 0x7FFFFFFF, tid) if not (tname & 0x80000000) else tid
        if not tdir:
            continue
        for nn, nid, noff, ndir in _dir_entries(d, base, toff):
            if not ndir:
                continue
            for _ln, _lid, loff, ldir in _dir_entries(d, base, noff):
                if ldir:
                    continue
                data_rva, size = struct.unpack_from("<II", d, base + loff)
                foff = pe._rva_to_off(data_rva)
                inv.setdefault(typ, []).append((nid, size))
                if foff is None:
                    continue
                if typ == "STRING":
                    _decode_stringtable(d, foff, size, nn & 0x7FFFFFFF, strings)
                elif typ in ("BITMAP", "ICON", "CURSOR", "GROUP_ICON", "GROUP_CURSOR"):
                    # key by the numeric resource id where possible: GROUP_ICON entries
                    # reference their member RT_ICON images by integer id.
                    key = int(nid[1:]) if isinstance(nid, str) and nid[1:].isdigit() else nid
                    blobs.setdefault(typ, {})[key] = bytes(d[foff:foff + size])
                elif typ == "VERSION":
                    v = _decode_version(bytes(d[foff:foff + size]))
                    if v:
                        versions.append((nid, v))
                elif typ in ("DIALOG", "MENU", "ACCELERATOR", "MESSAGETABLE"):
                    blobs.setdefault(typ, {}).setdefault(nid, bytes(d[foff:foff + size]))
    # emit .rc
    lines = ["// Reconstructed resources -- %s\n" % module]
    if strings:
        lines.append("STRINGTABLE\nBEGIN")
        for sid in sorted(strings):
            lines.append('    %-6d "%s"' % (sid, _esc(strings[sid])))
        lines.append("END\n")
    for nid, v in versions:
        lines.append(_emit_versioninfo(nid, v))
    ndlg = nmenu = 0
    from src.engines.recon import dialogs
    for nid, blob in sorted(blobs.get("DIALOG", {}).items(), key=lambda kv: str(kv[0])):
        try:
            rc = dialogs.decode_dialog(blob, nid)
        except Exception:
            rc = None
        if rc:
            lines.append(rc); ndlg += 1
    for nid, blob in sorted(blobs.get("MENU", {}).items(), key=lambda kv: str(kv[0])):
        try:
            rc = dialogs.decode_menu(blob, nid)
        except Exception:
            rc = None
        if rc:
            lines.append(rc); nmenu += 1
    naccel = 0
    for nid, blob in sorted(blobs.get("ACCELERATOR", {}).items(), key=lambda kv: str(kv[0])):
        try:
            rc = _decode_accelerators(blob, nid)
        except Exception:
            rc = None
        if rc:
            lines.append(rc); naccel += 1
    messages = {}
    for blob in blobs.get("MESSAGETABLE", {}).values():
        try:
            _decode_messagetable(blob, messages)
        except Exception:
            pass
    if messages:
        lines.append("\n// MESSAGETABLE -- %d messages" % len(messages))
        for mid in sorted(messages):
            lines.append("//   0x%08X  %s" % (mid, _esc(messages[mid]).strip()))
    files = _extract_images(blobs, out_dir) if out_dir else {}
    lines.append("// Resource inventory:")
    for typ in sorted(inv, key=str):
        lines.append("//   %-12s x%d  (%d bytes)" % (typ, len(inv[typ]), sum(s for _i, s in inv[typ])))
    return {"ok": True, "rc": "\n".join(lines), "strings": len(strings),
            "versions": [v for _i, v in versions], "extracted": files,
            "dialogs": ndlg, "menus": nmenu, "accelerators": naccel, "messages": len(messages),
            "types": {t: len(v) for t, v in inv.items()}}


def _decode_stringtable(d, foff, size, bundle_id, out):
    """RT_STRING bundle: 16 consecutive length-prefixed UTF-16 strings; id=(bundle-1)*16+i."""
    p, end, i = foff, foff + size, 0
    while p < end and i < 16:
        n = struct.unpack_from("<H", d, p)[0]; p += 2
        if n:
            s = d[p:p + n * 2].decode("utf-16le", "replace"); p += n * 2
            out[(bundle_id - 1) * 16 + i] = s
        i += 1


# ------------------------------------------------------------------ VS_VERSION_INFO
def _wstr(d, p):
    """Read a NUL-terminated UTF-16LE string at p; return (text, offset-past-terminator)."""
    s = p
    while p + 1 < len(d) and d[p:p + 2] != b"\x00\x00":
        p += 2
    return d[s:p].decode("utf-16le", "replace"), p + 2


def _align4(p):
    return (p + 3) & ~3


def _ver_node(d, p, end):
    """Parse one version node -> (key, value_bytes, wType, children_span, node_end, value_start)."""
    if p + 6 > end:
        return None
    wLength, wValueLength, wType = struct.unpack_from("<HHH", d, p)
    if wLength == 0 or p + wLength > end + 3:
        return None
    node_end = p + wLength
    key, q = _wstr(d, p + 6)
    q = _align4(q)
    # wValueLength is in WORDs for text values (wType==1), in bytes for binary (wType==0)
    vlen = wValueLength * 2 if wType == 1 else wValueLength
    val = d[q:q + vlen]
    children = _align4(q + vlen)
    return key, val, wType, children, node_end, q


def _decode_version(d):
    """Decode a VS_VERSION_INFO blob -> {'fixed': {...}, 'strings': {name: value}}."""
    root = _ver_node(d, 0, len(d))
    if not root or root[0] != "VS_VERSION_INFO":
        return None
    _key, fixed_bytes, _t, cstart, root_end, _vs = root
    out = {"fixed": {}, "strings": {}}
    if len(fixed_bytes) >= 52 and struct.unpack_from("<I", fixed_bytes, 0)[0] == 0xFEEF04BD:
        msf, lsf, msp, lsp = struct.unpack_from("<IIII", fixed_bytes, 8)
        ver = lambda hi, lo: "%d.%d.%d.%d" % (hi >> 16, hi & 0xFFFF, lo >> 16, lo & 0xFFFF)
        out["fixed"]["FileVersion"] = ver(msf, lsf)
        out["fixed"]["ProductVersion"] = ver(msp, lsp)
    # children: StringFileInfo / VarFileInfo
    p = cstart
    while p < root_end:
        child = _ver_node(d, p, root_end)
        if not child:
            break
        ckey, _v, _t2, gcstart, cend, _vs = child
        if ckey == "StringFileInfo":
            _collect_stringfileinfo(d, gcstart, cend, out["strings"])
        np = _align4(cend)
        p = np if np > p else p + 4
    return out


def _collect_stringfileinfo(d, p, end, out):
    """StringFileInfo -> StringTable(s) (one per lang) -> String(key=value) leaves."""
    while p < end:
        tbl = _ver_node(d, p, end)
        if not tbl:
            break
        _langkey, _v, _t, sstart, tend, _vs = tbl
        q = sstart
        while q < tend:
            s = _ver_node(d, q, tend)
            if not s:
                break
            skey, _sval, stype, _gc, send, vstart = s
            if stype == 1 and vstart < send:                   # value: a NUL-terminated wide string
                out[skey] = _wstr(d, vstart)[0]                # (wValueLength is unreliable -- read to NUL)
            nq = _align4(send)
            q = nq if nq > q else q + 4
        np = _align4(tend)
        p = np if np > p else p + 4


def _emit_versioninfo(nid, v):
    out = ["VS_VERSION_INFO VERSIONINFO   // resource %s" % nid]
    fx = v.get("fixed", {})
    if "FileVersion" in fx:
        parts = fx["FileVersion"].split(".")
        out.append(" FILEVERSION %s" % ",".join(parts))
    if "ProductVersion" in fx:
        parts = fx["ProductVersion"].split(".")
        out.append(" PRODUCTVERSION %s" % ",".join(parts))
    out.append("BEGIN\n    BLOCK \"StringFileInfo\"\n    BEGIN\n        BLOCK \"040904b0\"\n        BEGIN")
    for k in sorted(v.get("strings", {})):
        out.append('            VALUE "%s", "%s"' % (k, _esc(v["strings"][k])))
    out.append("        END\n    END\nEND\n")
    return "\n".join(out)


# ------------------------------------------------------------------ image extraction
def _bmp_offbits(dib):
    """Byte offset from the start of a .bmp file to the pixel data, given a headerless DIB."""
    if len(dib) < 4:
        return 14 + len(dib)
    hdr = struct.unpack_from("<I", dib, 0)[0]                   # biSize
    bitcount = struct.unpack_from("<H", dib, 14)[0] if len(dib) >= 16 else 0
    clrused = struct.unpack_from("<I", dib, 32)[0] if len(dib) >= 36 else 0
    if clrused == 0 and 0 < bitcount <= 8:
        clrused = 1 << bitcount
    return 14 + hdr + clrused * 4


def _extract_images(blobs, out_dir):
    """Write .bmp / .ico / .cur files from the collected binary resource blobs."""
    os.makedirs(out_dir, exist_ok=True)
    written = []
    for nid, dib in blobs.get("BITMAP", {}).items():           # RT_BITMAP is a headerless DIB
        offbits = _bmp_offbits(dib)
        hdr = struct.pack("<2sIHHI", b"BM", 14 + len(dib), 0, 0, offbits)
        fn = os.path.join(out_dir, "bitmap_%s.bmp" % nid)
        with open(fn, "wb") as f:
            f.write(hdr + dib)
        written.append(fn)
    written += _assemble_icons(blobs.get("GROUP_ICON", {}), blobs.get("ICON", {}), out_dir, 1, "ico")
    written += _assemble_icons(blobs.get("GROUP_CURSOR", {}), blobs.get("CURSOR", {}), out_dir, 2, "cur")
    return written


def _assemble_icons(groups, members, out_dir, rtype, ext):
    """Reassemble .ico/.cur from a GRPICONDIR and its RT_ICON/RT_CURSOR member images."""
    written = []
    for gid, g in groups.items():
        if len(g) < 6:
            continue
        _res, _typ, count = struct.unpack_from("<HHH", g, 0)
        entries, images, p = [], [], 6
        for _ in range(count):
            if p + 14 > len(g):
                break
            w, h, cc, res, planes, bits, nbytes, wid = struct.unpack_from("<BBBBHHIH", g, p)
            p += 14
            img = members.get(wid)
            if img is None:
                continue
            entries.append((w, h, cc, res, planes, bits, len(img)))
            images.append(img)
        if not images:
            continue
        # ICONDIR + ICONDIRENTRY[count], then image data
        head = struct.pack("<HHH", 0, rtype, len(images))
        offset = 6 + 16 * len(images)
        body = b""
        for (w, h, cc, res, planes, bits, nbytes), img in zip(entries, images):
            body += struct.pack("<BBBBHHII", w, h, cc, res, planes, bits, nbytes, offset)
            offset += nbytes
        fn = os.path.join(out_dir, "%s_%s.%s" % (ext, gid, ext))
        with open(fn, "wb") as f:
            f.write(head + body + b"".join(images))
        written.append(fn)
    return written


# ------------------------------------------------------------------ accelerators / message table
# a handful of virtual-key names; anything else prints as its numeric VK code.
_VK = {0x08: "VK_BACK", 0x09: "VK_TAB", 0x0D: "VK_RETURN", 0x1B: "VK_ESCAPE", 0x20: "VK_SPACE",
       0x21: "VK_PRIOR", 0x22: "VK_NEXT", 0x23: "VK_END", 0x24: "VK_HOME", 0x25: "VK_LEFT",
       0x26: "VK_UP", 0x27: "VK_RIGHT", 0x28: "VK_DOWN", 0x2D: "VK_INSERT", 0x2E: "VK_DELETE",
       0x70: "VK_F1", 0x71: "VK_F2", 0x72: "VK_F3", 0x73: "VK_F4", 0x74: "VK_F5", 0x75: "VK_F6",
       0x76: "VK_F7", 0x77: "VK_F8", 0x78: "VK_F9", 0x79: "VK_F10", 0x7A: "VK_F11", 0x7B: "VK_F12"}


def _resname(name):
    return ('"%s"' % name) if isinstance(name, str) and not name.lstrip("#").isdigit() \
        else str(name).lstrip("#")


def _decode_accelerators(d, name):
    """RT_ACCELERATOR: array of {WORD fFlags, WORD key, WORD id, WORD pad}; last has fFlags&0x80."""
    FVIRTKEY, FSHIFT, FCONTROL, FALT, FLAST = 0x01, 0x04, 0x08, 0x10, 0x80
    out, p = ["%s ACCELERATORS" % _resname(name), "BEGIN"], 0
    while p + 8 <= len(d):
        flags, key, ident, _pad = struct.unpack_from("<HHHH", d, p); p += 8
        mods = [m for bit, m in ((FSHIFT, "SHIFT"), (FCONTROL, "CONTROL"), (FALT, "ALT")) if flags & bit]
        if flags & FVIRTKEY:
            keytok = _VK.get(key, str(key)); typ = ["VIRTKEY"]
        else:
            keytok = '"%s"' % (chr(key) if 32 <= key < 127 else "\\x%02x" % key); typ = ["ASCII"]
        out.append("    %-10s %-8d %s" % (keytok + ",", ident, ", ".join(typ + mods)))
        if flags & FLAST:
            break
    out.append("END\n")
    return "\n".join(out)


def _decode_messagetable(d, out):
    """RT_MESSAGETABLE: MESSAGE_RESOURCE_DATA -> blocks[LowId,HighId,Offset] -> entries."""
    if len(d) < 4:
        return
    nblocks = struct.unpack_from("<I", d, 0)[0]
    for b in range(nblocks):
        base = 4 + b * 12
        if base + 12 > len(d):
            break
        low, high, off = struct.unpack_from("<III", d, base)
        p = off
        for mid in range(low, high + 1):
            if p + 4 > len(d):
                break
            length, flags = struct.unpack_from("<HH", d, p)
            if length < 4 or p + length > len(d):
                break
            raw = d[p + 4:p + length]
            txt = raw.decode("utf-16le", "replace") if flags & 1 else raw.decode("latin1", "replace")
            out[mid] = txt.rstrip("\x00\r\n")
            p += length
