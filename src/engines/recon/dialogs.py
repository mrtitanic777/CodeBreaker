#!/usr/bin/env python3
r"""dialogs.py -- decode RT_DIALOG / RT_MENU binary templates back to .rc source.

Dialogs and menus are a huge slice of a Windows app's authored source, and they are stored in
the PE as binary templates (DLGTEMPLATE / DLGTEMPLATEEX, MENU / MENUEX) -- not code, so no
behavioral engine reaches them, and resources.py only counted them. This walks those templates
and re-emits the `.rc` `DIALOG`/`DIALOGEX` and `MENU` blocks: the dialog's rect + style + font,
each control (its class resolved to BUTTON/EDIT/STATIC/... , id, rect, caption), and the menu's
nested POPUP / MENUITEM tree. The result is source a period `rc.exe` would accept.
"""
import struct

# atom -> predefined dialog control class (the 0xFFFF-prefixed ordinals in a DLGITEMTEMPLATE)
_CTLCLASS = {0x80: "BUTTON", 0x81: "EDIT", 0x82: "STATIC", 0x83: "LISTBOX",
             0x84: "SCROLLBAR", 0x85: "COMBOBOX"}

# common dialog / window style bits, for a readable style comment
_DS = {0x0001: "DS_ABSALIGN", 0x0002: "DS_SYSMODAL", 0x0040: "DS_SETFONT",
       0x0080: "DS_MODALFRAME", 0x0800: "DS_CENTER", 0x0004: "DS_LOCALEDIT"}
_WS = {0x80000000: "WS_POPUP", 0x40000000: "WS_CHILD", 0x10000000: "WS_VISIBLE",
       0x00C00000: "WS_CAPTION", 0x00080000: "WS_SYSMENU", 0x00040000: "WS_THICKFRAME",
       0x00020000: "WS_MINIMIZEBOX", 0x00010000: "WS_MAXIMIZEBOX", 0x00800000: "WS_BORDER",
       0x08000000: "WS_DISABLED", 0x00200000: "WS_VSCROLL", 0x00100000: "WS_HSCROLL"}
_MF_POPUP, _MF_END = 0x0010, 0x0080


def _esc(s):
    return s.replace("\\", "\\\\").replace('"', '""')


def _sz_or_ord(d, p):
    """A name that is either a NUL-terminated wide string or an ordinal. -> (value, new_p)."""
    if p + 2 > len(d):
        return "", p
    w = struct.unpack_from("<H", d, p)[0]
    if w == 0x0000:
        return "", p + 2                       # empty / none
    if w == 0xFFFF:
        return struct.unpack_from("<H", d, p + 2)[0], p + 4   # an ordinal (int)
    s = p
    while p + 1 < len(d) and d[p:p + 2] != b"\x00\x00":
        p += 2
    return d[s:p].decode("utf-16le", "replace"), p + 2


def _align4(p):
    return (p + 3) & ~3


def _styles(style, table):
    return [name for bit, name in table.items() if style & bit == bit]


def decode_dialog(d, name):
    """Decode one DLGTEMPLATE / DLGTEMPLATEEX blob -> a .rc DIALOG(EX) block string, or None."""
    if len(d) < 18:
        return None
    ver, sig = struct.unpack_from("<HH", d, 0)
    ex = (ver == 1 and sig == 0xFFFF)
    return _decode_ex(d, name) if ex else _decode_std(d, name)


def _decode_std(d, name):
    style, exstyle = struct.unpack_from("<II", d, 0)
    cdit, x, y, cx, cy = struct.unpack_from("<HHHHH", d, 8)
    p = 18
    _menu, p = _sz_or_ord(d, p)
    _cls, p = _sz_or_ord(d, p)
    title, p = _sz_or_ord(d, p)
    font = None
    if style & 0x40:                                          # DS_SETFONT
        pt = struct.unpack_from("<H", d, p)[0]; p += 2
        face, p = _sz_or_ord(d, p)
        font = (pt, face)
    head = 'DIALOG %d, %d, %d, %d' % (x, y, cx, cy)
    return _emit_dialog(name, head, style, title, font, d, p, cdit, ex=False)


def _decode_ex(d, name):
    # DLGTEMPLATEEX: dlgVer,sig,helpID(DWORD),exStyle(DWORD),style(DWORD),cDlgItems(WORD),x,y,cx,cy
    exstyle, style = struct.unpack_from("<II", d, 8)
    cdit, x, y, cx, cy = struct.unpack_from("<Hhhhh", d, 16)
    p = 26
    _menu, p = _sz_or_ord(d, p)
    _cls, p = _sz_or_ord(d, p)
    title, p = _sz_or_ord(d, p)
    font = None
    if style & 0x40:                                          # DS_SETFONT / DS_SHELLFONT
        pt, _w, _i, _c = struct.unpack_from("<HHBB", d, p); p += 6
        face, p = _sz_or_ord(d, p)
        font = (pt, face)
    head = 'DIALOGEX %d, %d, %d, %d' % (x, y, cx, cy)
    return _emit_dialog(name, head, style, title, font, d, p, cdit, ex=True)


def _emit_dialog(name, head, style, title, font, d, p, cdit, ex):
    out = ['%s %s' % (_dlgname(name), head)]
    st = _styles(style, {**_DS, **_WS})
    if title:
        out.append('CAPTION "%s"' % _esc(str(title)))
    if st:
        out.append('STYLE ' + " | ".join(st))
    if font:
        out.append('FONT %d, "%s"' % (font[0], _esc(str(font[1]))))
    out.append('BEGIN')
    for _ in range(cdit):
        p = _align4(p)
        item = _dlg_item(d, p, ex)
        if item is None:
            break
        line, p = item
        out.append('    ' + line)
    out.append('END\n')
    return "\n".join(out)


def _dlg_item(d, p, ex):
    if ex:
        # DLGITEMTEMPLATEEX: helpID(DWORD),exStyle,style,x,y,cx,cy,id(DWORD)
        if p + 24 > len(d):
            return None
        style = struct.unpack_from("<I", d, p + 8)[0]
        x, y, cx, cy = struct.unpack_from("<hhhh", d, p + 12)
        cid = struct.unpack_from("<I", d, p + 20)[0]
        p += 24
    else:
        # DLGITEMTEMPLATE: style(DWORD),exStyle(DWORD),x,y,cx,cy,id(WORD)
        if p + 18 > len(d):
            return None
        style = struct.unpack_from("<I", d, p)[0]
        x, y, cx, cy, cid = struct.unpack_from("<hhhhH", d, p + 8)
        p += 18
    cls, p = _sz_or_ord(d, p)
    text, p = _sz_or_ord(d, p)
    extra = struct.unpack_from("<H", d, p)[0]; p += 2          # creation-data byte count
    p += extra
    clsname = _CTLCLASS.get(cls, cls) if isinstance(cls, int) else cls
    cap = ('"%s"' % _esc(str(text))) if not isinstance(text, int) else str(text)
    line = 'CONTROL %s, %d, "%s", 0x%X, %d, %d, %d, %d' % (
        cap or '""', cid if not isinstance(cid, str) else 0, clsname or "", style, x, y, cx, cy)
    return line, p


def _dlgname(name):
    return ('"%s"' % name) if isinstance(name, str) and not name.lstrip("#").isdigit() else str(name).lstrip("#")


# --------------------------------------------------------------------------- MENU
def decode_menu(d, name):
    """Decode an RT_MENU (standard MENU) template -> a .rc MENU block string, or None."""
    if len(d) < 4:
        return None
    ver, hdr = struct.unpack_from("<HH", d, 0)
    if ver != 0:                                              # MENUEX (ver 1) -- not handled here
        return None
    out = ['%s MENU' % _dlgname(name), 'BEGIN']
    p = 4 + hdr
    p, _ = _menu_items(d, p, out, 1)
    out.append('END\n')
    return "\n".join(out)


def _menu_items(d, p, out, depth):
    ind = '    ' * depth
    while p + 2 <= len(d):
        flags = struct.unpack_from("<H", d, p)[0]; p += 2
        if flags & _MF_POPUP:
            text, p = _sz_or_ord(d, p)
            out.append('%sPOPUP "%s"' % (ind, _esc(str(text))))
            out.append('%sBEGIN' % ind)
            p, _ = _menu_items(d, p, out, depth + 1)
            out.append('%sEND' % ind)
        else:
            cid = struct.unpack_from("<H", d, p)[0]; p += 2
            text, p = _sz_or_ord(d, p)
            if text == "" and cid == 0 and not (flags & _MF_END):
                out.append('%sMENUITEM SEPARATOR' % ind)
            else:
                out.append('%sMENUITEM "%s", %d' % (ind, _esc(str(text)), cid))
        if flags & _MF_END:
            break
    return p, None
