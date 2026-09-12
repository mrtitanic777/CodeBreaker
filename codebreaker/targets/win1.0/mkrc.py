#!/usr/bin/env python3
"""
mkrc.py — reconstruct an app's RESOURCE SCRIPT (.RC) from the shipped binary.

The resources are the half of a Windows program that is not code: menus,
dialogs, strings, accelerators, icons, cursors, bitmaps. They survive in the NE
completely and unambiguously — a menu resource literally pairs a command id with
the text the user saw — so this is transcription, not inference, and the .RC it
writes is checkable: recompile it with the period RC and the bytes should come
back.

What is emitted
  MENU / STRINGTABLE / DIALOG / ACCELERATORS  -> RC source, in the 1985 dialect
        (BEGIN/END, `POPUP`, `MENUITEM`, `CONTROL`), taken from Microsoft's own
        CARDFILE.RC as the model for what RC 1.0 accepts.
  ICON / CURSOR / BITMAP / raw                -> written out as binary files
        next to the .RC and referenced by name.

The icon/cursor format was settled against the SDK's own CARDFILE.ICO: the file
and the bound resource are byte-identical except for byte 1, which is 0x00 in
the file and 0x01 once bound. So the file is written back as `01 00 || rest`.
Resource lengths in the table are padded to the alignment unit, so trailing
bytes beyond the real payload are trimmed for the image formats.

Usage:  python mkrc.py CARDFILE -o build16/CARDLK
        python mkrc.py CARDFILE            (print the .RC to stdout)
"""
import os
import struct
import sys

import rsrc
from codebreaker.disasm.disasm import open_ne

# Windows 1.0 virtual-key names worth spelling out in an accelerator table.
VK = {0x08: "VK_BACK", 0x09: "VK_TAB", 0x0D: "VK_RETURN", 0x1B: "VK_ESCAPE",
      0x20: "VK_SPACE", 0x21: "VK_PRIOR", 0x22: "VK_NEXT", 0x23: "VK_END",
      0x24: "VK_HOME", 0x25: "VK_LEFT", 0x26: "VK_UP", 0x27: "VK_RIGHT",
      0x28: "VK_DOWN", 0x2D: "VK_INSERT", 0x2E: "VK_DELETE",
      0x70: "VK_F1", 0x71: "VK_F2", 0x72: "VK_F3", 0x73: "VK_F4",
      0x74: "VK_F5", 0x75: "VK_F6", 0x76: "VK_F7", 0x77: "VK_F8",
      0x78: "VK_F9", 0x79: "VK_F10"}

MF_GRAYED, MF_DISABLED, MF_CHECKED = 0x01, 0x02, 0x08
MF_POPUP, MF_MENUBARBREAK, MF_END = 0x10, 0x40, 0x80


def rcstr(s):
    """A string literal for RC 1.0. Its escaping is NOT C's, verified against
    RC V1.00 and the shipped binaries:
      - an embedded quote is DOUBLED (""), NOT backslash-escaped. `\\"` is read
        as a literal backslash followed by the closing quote (SPOOLER's
        "Spooler=No" string broke on it).
      - a backslash is doubled (\\\\) — this one IS like C, and round-trips
        (CONTROL's "A:\\" binds to a single backslash).
      - octal (\\042) is NOT understood — it mangles.
    """
    return '"%s"' % s.replace("\\", "\\\\").replace('"', '""')


def resid(r):
    """Resource id as RC wants it: a number stays a number, a name is quoted."""
    i = r["id"]
    return i[1:] if isinstance(i, str) and i.startswith("#") else str(i)


# --------------------------------------------------------------------------
def emit_menu(d, name):
    out = ["%s MENU" % name, "BEGIN"]
    depth = 1
    stack = []
    for lvl, ident, txt, fl in rsrc.menu(d):
        while len(stack) > lvl:
            depth -= 1
            out.append("    " * depth + "END")
            stack.pop()
        ind = "    " * (lvl + 1)
        if ident is None:                       # POPUP
            out.append("%sPOPUP %s" % (ind, rcstr(txt)))
            out.append("%sBEGIN" % ind)
            stack.append(True)
            depth += 1
            continue
        opts = []
        if fl & MF_GRAYED:
            opts.append("GRAYED")
        if fl & MF_DISABLED:
            opts.append("INACTIVE")
        if fl & MF_CHECKED:
            opts.append("CHECKED")
        if fl & MF_MENUBARBREAK:
            opts.append("MENUBARBREAK")
        if ident == 0 and not txt:
            out.append("%sMENUITEM SEPARATOR" % ind)
        else:
            out.append("%sMENUITEM %s, %d%s"
                       % (ind, rcstr(txt), ident,
                          (", " + ", ".join(opts)) if opts else ""))
    while stack:
        depth -= 1
        out.append("    " * depth + "END")
        stack.pop()
    out.append("END")
    return out


def emit_accel(d, name):
    """5 bytes per entry: BYTE flags, WORD key, WORD command; 0x80 ends it.
    Validated against CARDFILE: VK_DELETE->113 and VK_INSERT->115 are exactly
    the Cut/Del and Paste/Ins items its menu names."""
    out = ["%s ACCELERATORS" % name, "BEGIN"]
    p = 0
    while p + 5 <= len(d):
        fl = d[p]
        key, cmd = struct.unpack_from("<HH", d, p + 1)
        p += 5
        opts = ["VIRTKEY"] if fl & 0x01 else []
        if fl & 0x04:
            opts.append("SHIFT")
        if fl & 0x08:
            opts.append("CONTROL")
        if fl & 0x10:
            opts.append("ALT")
        keytxt = VK.get(key, None)
        if keytxt is None:
            keytxt = ("%d" % key) if (fl & 0x01) else rcstr(chr(key & 0xFF))
        out.append("    %-12s %d%s" % (keytxt + ",", cmd,
                                       (", " + ", ".join(opts)) if opts else ""))
        if fl & 0x80:
            break
    out.append("END")
    return out


def emit_strings(pairs):
    out = ["STRINGTABLE", "BEGIN"]
    for ident, txt in pairs:
        out.append("    %-6d %s" % (ident, rcstr(txt)))
    out.append("END")
    return out


def emit_dialog(d, name):
    hdr, items = rsrc.dialog(d)
    x, y, cx, cy = hdr["rect"]
    out = ["%s DIALOG %d, %d, %d, %d" % (name, x, y, cx, cy),
           "STYLE 0x%08lxL" % hdr["style"]]
    if hdr.get("caption"):
        out.append("CAPTION %s" % rcstr(hdr["caption"]))
    out.append("BEGIN")
    for it in items:
        ix, iy, icx, icy = it["rect"]
        out.append('    CONTROL %s, %d, %s, 0x%08lxL, %d, %d, %d, %d'
                   % (_dlg_field(it["text"]), it["id"], _dlg_field(str(it["cls"])),
                      it["style"], ix, iy, icx, icy))
    out.append("END")
    return out


def _dlg_field(v):
    """A dialog CONTROL's text/class field for RC. An ordinal ('#N', from the
    binary's 0xFF+WORD form — e.g. an icon static naming a system bitmap) is
    emitted as a BARE NUMBER so RC re-encodes it as an ordinal, not a literal
    string; anything else is a quoted string."""
    if isinstance(v, str) and v.startswith("#") and v[1:].isdigit():
        return v[1:]
    return rcstr(v)


def write_blob(outdir, fname, data, is_image):
    """Image resources (ICON/CURSOR) are written back as their file form:
    byte 1 = 0 (0x01 once bound), and TRIMMED to the real payload length.

    The NE resource table stores each length rounded UP to the segment
    alignment unit (16 bytes for these apps), so the bytes mkrc reads carry
    trailing pad. RC 1.0 does NOT copy an icon/cursor file blindly — it
    validates the length against the device-dependent header
    (14-byte header + XOR mask + AND mask = 14 + 2*cy*cbWidth) and, when a
    file is longer than that, silently emits a 4-byte stub instead of the
    image. So the pad MUST be stripped or the cursor/icon never binds. The
    trim only fires when the computed payload sits within one alignment unit
    of the blob (i.e. we are removing pad, never real bits); anything that
    does not fit the mono formula is left untouched."""
    if is_image and len(data) > 14:
        cy  = struct.unpack_from("<H", data, 8)[0]
        cbw = struct.unpack_from("<H", data, 10)[0]
        dim = 14 + 2 * cy * cbw
        if 0 < dim <= len(data) and (len(data) - dim) < 16:
            data = data[:dim]
    if is_image and len(data) > 2:
        data = bytes([data[0], 0x00]) + data[2:]
    open(os.path.join(outdir, fname), "wb").write(data)
    return fname


def build(app, outdir=None):
    ne = open_ne(os.path.join("files", "WF", app + ".EXE"))
    res = ne.resources()
    L = ["/* %s.RC — reconstructed from %s.EXE by mkrc.py." % (app, app),
         " * Menus, dialogs, strings and accelerators are transcribed from the",
         " * binary's own resource data: the text and every command id below is",
         " * Microsoft's, byte for byte. Images are written out beside this file",
         " * and referenced by name. Resource NUMBERS are the binary's. */",
         "",
         "/* RC 1.0 knows no VK_ names of its own — they come from the SDK",
         " * header, which is why Microsoft's own .RC files include it. */",
         "#include <windows.h>",
         ""]
    counts = {}
    blob_n = {}
    strings = []
    for r in res:
        t = r["type"]
        counts[t] = counts.get(t, 0) + 1
        d = ne.data[r["off"]:r["off"] + r["length"]]
        name = resid(r)
        try:
            if t == "MENU":
                L += emit_menu(d, name) + [""]
            elif t == "ACCELERATOR":
                L += emit_accel(d, name) + [""]
            elif t == "DIALOG":
                L += emit_dialog(d, name) + [""]
            elif t in ("ICON", "CURSOR", "BITMAP", "FONT", "DATA"):
                ext = {"ICON": "ico", "CURSOR": "cur", "BITMAP": "bmp",
                       "FONT": "fnt", "DATA": "bin"}[t]
                # DOS 8.3: the basename must be <= 8 chars. `app[:6]+name` blew
                # that (CLOCK + CLOCK = CLOCKCLOCK.ico, 10 chars, and RC then
                # "file not found"). Use a short app prefix + a per-type running
                # index instead — unique, 8.3-safe, and stable across runs.
                blob_n[t] = blob_n.get(t, 0) + 1
                letter = {"ICON": "I", "CURSOR": "C", "BITMAP": "B",
                          "FONT": "F", "DATA": "D"}[t]
                # app[:4] + letter + up to 2 digits <= 7 chars, 8.3-safe even
                # for PAINT's 10 cursors / CONTROL's 11 dialogs.
                fn = "%s%s%d.%s" % (app[:4], letter, blob_n[t], ext)
                if outdir:
                    write_blob(outdir, fn, d, t in ("ICON", "CURSOR"))
                L.append("%s %s %s" % (name, t, fn))
                L.append("")
        except Exception as e:
            L.append("/* %s %s: NOT DECODED (%s) — resource left unreconstructed */"
                     % (t, name, e))
            L.append("")
    try:
        strings = rsrc.strings(ne, res)
    except Exception:
        strings = []
    if strings:
        L += emit_strings(strings) + [""]
    return "\n".join(L), counts, len(strings)


def main():
    if len(sys.argv) < 2:
        print(__doc__); return 1
    app = sys.argv[1].upper()
    outdir = None
    if "-o" in sys.argv:
        outdir = sys.argv[sys.argv.index("-o") + 1]
        os.makedirs(outdir, exist_ok=True)
    text, counts, nstr = build(app, outdir)
    if outdir:
        p = os.path.join(outdir, app[:8] + ".RC")
        # cp437, not the host default: the resource text IS OEM bytes (arrows,
        # box characters) and the 1985 RC reads them as such. This round-trips
        # exactly with the cp437 decode rsrc.py does on the way in.
        open(p, "w", encoding="cp437", errors="replace",
             newline="\r\n").write(text)
        print("%s -> %s   %s, %d strings"
              % (app, p, ", ".join("%s=%d" % kv for kv in sorted(counts.items())),
                 nstr))
    else:
        sys.stdout.buffer.write(text.encode("cp437", "replace"))
    return 0


if __name__ == "__main__":
    sys.exit(main())
