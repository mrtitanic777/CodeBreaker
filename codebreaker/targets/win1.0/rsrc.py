#!/usr/bin/env python3
"""
rsrc.py — decode MENU, STRINGTABLE, DIALOG and ACCELERATOR resources.

Why this is evidence and not decoration: a menu resource pairs a **command id**
with the **text the user sees**. That is the one place in a Windows binary where
an integer constant is written down next to its meaning by the original authors.
Every `cmp ax, 0x####` in a WM_COMMAND handler can then be resolved to a menu
item without guessing.

The same is true of a dialog template: it lists each control's class, id and
caption, so a dialog procedure's `GetDlgItem(h, 0x0402)` becomes readable.

Nothing here is inferred. The text is Microsoft's, byte for byte.

Usage:
    python rsrc.py APP                 # menus + strings + accelerators
    python rsrc.py APP --dialogs
    python rsrc.py APP --dialog 19
"""
import sys, struct
from codebreaker.disasm_engine.disasm import open_ne


def _sz(d, p):
    """NUL-terminated string at p -> (text, next_offset)."""
    e = d.index(b"\0", p)
    return d[p:e].decode("cp437", "replace"), e + 1


def menu(d):
    """Walk a Windows 1.0 MENU resource. Returns [(depth, id, text, flags)].

    *** THE 1.0 MENU FORMAT IS NOT THE LATER ONE ***

    Two differences, both proven from MW_MENU's bytes and both fatal if assumed
    away:

      1. There is **no 4-byte MENUHEADER**. The resource begins with the first
         item at offset 0. Skipping four bytes eats the first item's flags and
         half its id.
      2. **`flags` is a BYTE, not a WORD.** The layout is

             popup   :  BYTE flags(|0x10);  char text[]
             item    :  BYTE flags;  WORD id;  char text[]
             separator: BYTE flags;  WORD id == 0;  char text[] == ""

         Reading flags as a WORD shifts everything by one and silently
         decapitates every caption — "Open..." reads back as "pen...", which
         looks like a text-encoding problem rather than a structural one.

    Checked against MW_MENU: File > New/Open.../Save.../-/Print.../Change
    Printer.../Repaginate... yields ids 0x1000..0x1005 consecutively, with
    MF_END (0x80) set on exactly the last item of each popup.
    """
    # The menu BAR is an unwritten enclosing level: the top-level popups sit
    # inside it but nothing in the byte stream opens it. Without a sentinel for
    # it, closing the File popup empties the stack and the walk stops after one
    # menu — which is exactly what it did.
    out, p, depth, stack = [], 0, 0, [False]
    while p < len(d):
        flags = d[p]; p += 1
        if flags & 0x10:                       # MF_POPUP: no id field
            txt, p = _sz(d, p)
            out.append((depth, None, txt, flags))
            # A popup can itself carry MF_END — "this is the last item of the
            # menu that CONTAINS me". That bit fires only once its own children
            # are exhausted, so it has to be remembered, not acted on now.
            stack.append(bool(flags & 0x80))
            depth += 1
            continue
        ident = struct.unpack_from("<H", d, p)[0]; p += 2
        txt, p = _sz(d, p)
        out.append((depth, ident, txt, flags))
        if flags & 0x80:                       # MF_END closes the current popup
            closed_last = True
            while stack and closed_last:
                closed_last = stack.pop()
                depth -= 1
            if not stack:
                break
    return out


MF = [(0x01, "GRAYED"), (0x02, "DISABLED"), (0x08, "CHECKED"),
      (0x10, "POPUP"), (0x20, "MENUBREAK"), (0x40, "MENUBARBREAK"),
      (0x80, "END")]


def strings(ne, res):
    """STRINGTABLE block id N holds string ids (N-1)*16 .. (N-1)*16+15."""
    out = []
    for r in res:
        if r["type"] != "STRING" or not r["id"].startswith("#"):
            continue
        blk = int(r["id"][1:])
        d = ne.data[r["off"]:r["off"] + r["length"]]
        p, i = 0, 0
        while p < len(d) and i < 16:
            n = d[p]; p += 1
            if n:
                out.append(((blk - 1) * 16 + i, d[p:p + n].decode("cp437", "replace")))
            p += n
            i += 1
    return out


def dialog(d):
    """DLGTEMPLATE + DLGITEMTEMPLATEs. Returns (header_dict, [items])."""
    style, = struct.unpack_from("<I", d, 0)
    n = d[4]
    x, y, cx, cy = struct.unpack_from("<4h", d, 5)
    p = 13
    menu_, p = _sz(d, p)
    cls, p = _sz(d, p)
    cap, p = _sz(d, p)
    hdr = dict(style=style, n=n, rect=(x, y, cx, cy), menu=menu_, cls=cls, caption=cap)
    items = []
    CLASSES = {0x80: "BUTTON", 0x81: "EDIT", 0x82: "STATIC",
               0x83: "LISTBOX", 0x84: "SCROLLBAR", 0x85: "COMBOBOX"}
    for _ in range(n):
        if p + 12 > len(d):
            break
        ix, iy, icx, icy, ident = struct.unpack_from("<5h", d, p)
        istyle, = struct.unpack_from("<I", d, p + 10)
        p += 14
        # CLASS field: a byte with 0x80 set is a predefined class id; 0xFF
        # means "class by ordinal" (0xFF + a WORD, no NUL); otherwise a
        # NUL-terminated class-name string.
        icls, ord_cls = _dlg_name(d, p, CLASSES)
        p = ord_cls
        # TEXT field: same rule. A control whose text is an ordinal (0xFF +
        # WORD) is an SS_ICON-style static that names an icon/resource by NUMBER,
        # NOT a string. Reading it as a NUL-terminated string ran past the real
        # 3-byte field and desynchronised every following control (WRITE #10/#21).
        if p < len(d) and d[p] == 0xFF:
            txt = "#%d" % struct.unpack_from("<H", d, p + 1)[0]
            p += 3
        else:
            txt, p = _sz(d, p)
        nextra = d[p] if p < len(d) else 0     # count of creation-data bytes
        p += 1 + nextra                        # skip the count AND its bytes
        items.append(dict(rect=(ix, iy, icx, icy), id=ident, cls=icls,
                          style=istyle, text=txt))
    return hdr, items


def _dlg_name(d, p, classes):
    """Decode a dialog CLASS field at p -> (name, next_offset).
    0x80-set byte = predefined class; 0xFF = ordinal (0xFF+WORD); else string."""
    c = d[p]
    if c == 0xFF:
        return "#%d" % struct.unpack_from("<H", d, p + 1)[0], p + 3
    if c & 0x80:
        return classes.get(c, "0x%02x" % c), p + 1
    return _sz(d, p)


def main():
    app = sys.argv[1].upper()
    ne = open_ne("files/WF/%s.EXE" % app)
    res = ne.resources()

    if "--dialog" in sys.argv or "--dialogs" in sys.argv:
        want = None
        if "--dialog" in sys.argv:
            want = "#" + sys.argv[sys.argv.index("--dialog") + 1]
        for r in res:
            if r["type"] != "DIALOG" or (want and r["id"] != want):
                continue
            d = ne.data[r["off"]:r["off"] + r["length"]]
            try:
                hdr, items = dialog(d)
            except Exception as e:
                print("DIALOG %s: undecodable (%s)" % (r["id"], e)); continue
            print("\nDIALOG %s  %r  style=0x%08x  rect=%s"
                  % (r["id"], hdr["caption"], hdr["style"], hdr["rect"]))
            for it in items:
                print("   id=%-6d %-9s %-28r style=0x%08x %s"
                      % (it["id"], it["cls"], it["text"], it["style"], it["rect"]))
        return

    for r in res:
        if r["type"] == "MENU":
            d = ne.data[r["off"]:r["off"] + r["length"]]
            print("MENU %s" % r["id"])
            for depth, ident, txt, fl in menu(d):
                tags = ",".join(n for b, n in MF if fl & b and n != "END")
                print("   %s%-30s %-18s %s"
                      % ("  " * depth, txt or "(separator)",
                         "" if ident is None else "id=0x%04x (%d)" % (ident, ident),
                         tags))
    s = strings(ne, res)
    if s:
        print("\nSTRINGTABLE (%d strings)" % len(s))
        for i, t in sorted(s):
            print("   %-6d %r" % (i, t))
    for r in res:
        if r["type"] == "ACCELERATOR":
            d = ne.data[r["off"]:r["off"] + r["length"]]
            print("\nACCELERATOR %s" % r["id"])
            p = 0
            while p + 5 <= len(d):
                fl, key, cmd = struct.unpack_from("<BHH", d, p); p += 5
                print("   flags=0x%02x key=0x%04x cmd=%d (0x%04x)" % (fl, key, cmd, cmd))


if __name__ == "__main__":
    main()
