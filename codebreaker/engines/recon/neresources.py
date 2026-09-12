#!/usr/bin/env python3
r"""neresources.py -- reconstruct a 16-bit NE module's resources to .rc (the Win16 twin of resources).

resources.py/dialogs.py read the PE .rsrc format; the 360 NE modules keep resources in the NE
header's own resource table, with different on-disk layouts -- ASCII (not UTF-16) text, byte-length-
prefixed strings, 16-bit dialog/menu templates. So no 16-bit app's UI was being recovered. This
decodes the NE resource types back to source: STRINGTABLE (Pascal-string bundles), MENU (16-bit
ASCII item tree), DIALOG (16-bit DLGTEMPLATE with its controls), ACCELERATORS, and extracts
BITMAP/ICON/CURSOR to real files -- recovering the entire authored UI of a Win16 program.
"""
import os
import struct

from codebreaker.engines.recon import resources as _pe   # reuse _esc / image extraction / VK names

_CTLCLASS = {0x80: "BUTTON", 0x81: "EDIT", 0x82: "STATIC", 0x83: "LISTBOX",
             0x84: "SCROLLBAR", 0x85: "COMBOBOX"}
_MF_POPUP, _MF_END = 0x0010, 0x0080


def _asciiz(d, p):
    e = d.find(b"\x00", p)
    if e < 0:
        e = len(d)
    return d[p:e].decode("latin1", "replace"), e + 1


def _sz_or_ord(d, p):
    """16-bit name: 0xFF + WORD ordinal, or a NUL-terminated ASCII string, or empty."""
    if p >= len(d):
        return "", p
    if d[p] == 0xFF:
        return struct.unpack_from("<H", d, p + 1)[0], p + 3
    if d[p] == 0x00:
        return "", p + 1
    return _asciiz(d, p)


def _decode_string(blob, bundle_id, out):
    """NE STRINGTABLE: 16 byte-length-prefixed ASCII strings; id=(bundle-1)*16+i."""
    p, i = 0, 0
    while p < len(blob) and i < 16:
        n = blob[p]; p += 1
        if n:
            out[(bundle_id - 1) * 16 + i] = blob[p:p + n].decode("latin1", "replace"); p += n
        i += 1


def _decode_menu(blob, name):
    out = ["%s MENU" % _resname(name), "BEGIN"]
    p = 4                                                    # skip version + header-size words
    _menu_items(blob, p, out, 1)
    out.append("END\n")
    return "\n".join(out)


def _menu_items(d, p, out, depth):
    ind = "    " * depth
    while p + 2 <= len(d):
        flags = struct.unpack_from("<H", d, p)[0]; p += 2
        if flags & _MF_POPUP:
            text, p = _asciiz(d, p)
            out.append('%sPOPUP "%s"' % (ind, _pe._esc(text)))
            out.append("%sBEGIN" % ind)
            p = _menu_items(d, p, out, depth + 1)
            out.append("%sEND" % ind)
        else:
            if p + 2 > len(d):
                break
            mid = struct.unpack_from("<H", d, p)[0]; p += 2
            text, p = _asciiz(d, p)
            if not text and mid == 0 and not (flags & _MF_END):
                out.append("%sMENUITEM SEPARATOR" % ind)
            else:
                out.append('%sMENUITEM "%s", %d' % (ind, _pe._esc(text), mid))
        if flags & _MF_END:
            break
    return p


def _decode_dialog(blob, name):
    """NE 16-bit DLGTEMPLATE -> a .rc DIALOG block."""
    if len(blob) < 13:
        return None
    style = struct.unpack_from("<I", blob, 0)[0]
    cdit = blob[4]
    x, y, cx, cy = struct.unpack_from("<HHHH", blob, 5)
    p = 13
    _menu, p = _sz_or_ord(blob, p)
    _cls, p = _sz_or_ord(blob, p)
    caption, p = _sz_or_ord(blob, p)
    font = None
    if style & 0x40:                                         # DS_SETFONT
        pt = struct.unpack_from("<H", blob, p)[0]; p += 2
        face, p = _asciiz(blob, p)
        font = (pt, face)
    out = ["%s DIALOG %d, %d, %d, %d" % (_resname(name), x, y, cx, cy)]
    if caption:
        out.append('CAPTION "%s"' % _pe._esc(str(caption)))
    out.append("STYLE 0x%08X" % style)
    if font:
        out.append('FONT %d, "%s"' % (font[0], _pe._esc(str(font[1]))))
    out.append("BEGIN")
    for _ in range(cdit):
        if p + 10 > len(blob):
            break
        ix, iy, icx, icy, iid = struct.unpack_from("<HHHHH", blob, p); p += 10
        istyle = struct.unpack_from("<I", blob, p)[0]; p += 4
        if p < len(blob) and blob[p] in _CTLCLASS:          # predefined class atom
            cls = _CTLCLASS[blob[p]]; p += 1
        else:
            cls, p = _asciiz(blob, p)
        text, p = _sz_or_ord(blob, p)
        if p < len(blob):
            extra = blob[p]; p += 1 + extra                 # creation-data byte count
        cap = ('"%s"' % _pe._esc(str(text))) if not isinstance(text, int) else str(text)
        out.append('    CONTROL %s, %d, "%s", 0x%08X, %d, %d, %d, %d'
                   % (cap or '""', iid, cls or "", istyle, ix, iy, icx, icy))
    out.append("END\n")
    return "\n".join(out)


def _decode_accel(blob, name):
    FVIRTKEY, FSHIFT, FCONTROL, FALT, FLAST = 0x01, 0x04, 0x08, 0x10, 0x80
    out, p = ["%s ACCELERATORS" % _resname(name), "BEGIN"], 0
    while p + 5 <= len(blob):
        flags = blob[p]; key, ident = struct.unpack_from("<HH", blob, p + 1); p += 5
        mods = [m for bit, m in ((FSHIFT, "SHIFT"), (FCONTROL, "CONTROL"), (FALT, "ALT")) if flags & bit]
        if flags & FVIRTKEY:
            keytok, typ = _pe._VK.get(key, str(key)), ["VIRTKEY"]
        else:
            keytok, typ = ('"%s"' % (chr(key) if 32 <= key < 127 else "\\x%02x" % key)), ["ASCII"]
        out.append("    %-10s %-8d %s" % (keytok + ",", ident, ", ".join(typ + mods)))
        if flags & FLAST:
            break
    out.append("END\n")
    return "\n".join(out)


def _resname(name):
    return ('"%s"' % name) if isinstance(name, str) and not name.lstrip("#").isdigit() \
        else str(name).lstrip("#")


def _idnum(rid):
    return int(rid[1:]) if isinstance(rid, str) and rid[1:].isdigit() else rid


def reconstruct(module, out_dir=None):
    """Recover a 16-bit NE module's resources as .rc + extracted images. {'ok','rc',...}."""
    from codebreaker.core import cores
    from codebreaker.formats.ne import NEFile
    path = cores.get("target").resolve(module) if not os.path.isabs(str(module)) else module
    try:
        ne = NEFile(path)
        reslist = ne.resources()
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    if not reslist:
        return {"ok": False, "reason": "no NE resource table (not an NE, or none)"}
    d = ne.data
    strings, blobs, inv = {}, {}, {}
    lines = ["// Reconstructed NE resources -- %s\n" % os.path.basename(str(path))]
    for r in reslist:
        typ, rid = r["type"], r["id"]
        blob = d[r["off"]:r["off"] + r["length"]]
        inv[typ] = inv.get(typ, 0) + 1
        if typ == "STRING":
            _decode_string(blob, _idnum(rid) if isinstance(_idnum(rid), int) else 1, strings)
        elif typ in ("BITMAP", "ICON", "CURSOR", "GROUP_ICON", "GROUP_CURSOR"):
            blobs.setdefault(typ, {})[_idnum(rid)] = bytes(blob)
    ndlg = nmenu = naccel = 0
    if strings:
        lines.append("STRINGTABLE\nBEGIN")
        for sid in sorted(strings):
            lines.append('    %-6d "%s"' % (sid, _pe._esc(strings[sid])))
        lines.append("END\n")
    for r in reslist:
        typ, rid, blob = r["type"], r["id"], d[r["off"]:r["off"] + r["length"]]
        try:
            if typ == "MENU":
                lines.append(_decode_menu(blob, rid)); nmenu += 1
            elif typ == "DIALOG":
                out = _decode_dialog(blob, rid)
                if out:
                    lines.append(out); ndlg += 1
            elif typ == "ACCELERATOR":
                lines.append(_decode_accel(blob, rid)); naccel += 1
        except Exception:
            pass
    files = _pe._extract_images(blobs, out_dir) if out_dir else []
    lines.append("// Resource inventory: " + ", ".join("%s x%d" % (t, n) for t, n in sorted(inv.items())))
    return {"ok": True, "rc": "\n".join(lines), "strings": len(strings), "dialogs": ndlg,
            "menus": nmenu, "accelerators": naccel, "extracted": files, "types": inv}


# ------------------------------------------------------------------ structured extractors (for xref)
def _blobs_of(nef, typ):
    d = nef.data
    return [(r["id"], d[r["off"]:r["off"] + r["length"]]) for r in nef.resources() if r["type"] == typ]


def string_table(nef):
    """{string id: text} across all STRING resources (bundle-1)*16 + i indexing."""
    out = {}
    for rid, blob in _blobs_of(nef, "STRING"):
        bundle = int(rid[1:]) if isinstance(rid, str) and rid[1:].isdigit() else 1
        _decode_string(blob, bundle, out)
    return out


def _menu_ids_walk(d, p, out):
    while p + 2 <= len(d):
        flags = struct.unpack_from("<H", d, p)[0]; p += 2
        if flags & _MF_POPUP:
            _txt, p = _asciiz(d, p)
            p = _menu_ids_walk(d, p, out)
        else:
            if p + 2 > len(d):
                break
            mid = struct.unpack_from("<H", d, p)[0]; p += 2
            text, p = _asciiz(d, p)
            if text and mid:
                out[mid] = text
        if flags & _MF_END:
            break
    return p


def menu_ids(nef):
    """{command id: menu-item text} across all MENU resources."""
    out = {}
    for _rid, blob in _blobs_of(nef, "MENU"):
        try:
            _menu_ids_walk(blob, 4, out)
        except Exception:
            pass
    return out


def accel_ids(nef):
    """{command id: key} across all ACCELERATOR resources."""
    out = {}
    for _rid, blob in _blobs_of(nef, "ACCELERATOR"):
        p = 0
        while p + 5 <= len(blob):
            flags = blob[p]; key, ident = struct.unpack_from("<HH", blob, p + 1); p += 5
            if ident:
                out[ident] = (_pe._VK.get(key, str(key)) if flags & 0x01 else
                              (chr(key) if 32 <= key < 127 else "0x%x" % key))
            if flags & 0x80:
                break
    return out


def dialog_ctl_ids(nef):
    """{control id: caption} across all DIALOG resources (skips the 0xFFFF static-text sentinel)."""
    out = {}
    for _rid, blob in _blobs_of(nef, "DIALOG"):
        try:
            if len(blob) < 13:
                continue
            style = struct.unpack_from("<I", blob, 0)[0]; cdit = blob[4]
            p = 13
            for _f in ("menu", "class", "caption"):
                _v, p = _sz_or_ord(blob, p)
            if style & 0x40:
                _pt = struct.unpack_from("<H", blob, p)[0]; p += 2
                _face, p = _asciiz(blob, p)
            for _ in range(cdit):
                if p + 10 > len(blob):
                    break
                _x, _y, _cx, _cy, iid = struct.unpack_from("<HHHHH", blob, p); p += 10
                p += 4                                        # style dword
                if p < len(blob) and blob[p] in _CTLCLASS:
                    p += 1
                else:
                    _c, p = _asciiz(blob, p)
                text, p = _sz_or_ord(blob, p)
                if p < len(blob):
                    p += 1 + blob[p]
                if iid and iid != 0xFFFF and not isinstance(text, int) and text:
                    out[iid] = text
        except Exception:
            pass
    return out
