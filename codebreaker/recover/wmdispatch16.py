#!/usr/bin/env python3
r"""wmdispatch16.py -- name the WM_ messages a 16-bit WndProc/DlgProc dispatches on (conservative).

A window/dialog procedure switches on its message argument (WM_PAINT, WM_COMMAND, WM_DESTROY, ...).
Recovering which messages it handles is real insight, but reading the dispatch needs the compare
sites -- and 16-bit linear disasm DRIFTS. So this byte-scans for the compare encodings against a
known WM_ value (`cmp ax, imm16` = 3D, `cmp r16, imm16` = 81 /7) rather than trusting a full
disassembly. Because small message ids collide with ordinary constants, it only reports a dispatch
when several distinct WM_ values appear AND at least one is a core message -- conservative by design.
"""
import struct

_WM = {
    0x0001: "WM_CREATE", 0x0002: "WM_DESTROY", 0x0003: "WM_MOVE", 0x0005: "WM_SIZE",
    0x0006: "WM_ACTIVATE", 0x0007: "WM_SETFOCUS", 0x0008: "WM_KILLFOCUS", 0x000A: "WM_ENABLE",
    0x000D: "WM_GETTEXT", 0x000E: "WM_GETTEXTLENGTH", 0x000F: "WM_PAINT", 0x0010: "WM_CLOSE",
    0x0011: "WM_QUERYENDSESSION", 0x0012: "WM_QUIT", 0x0014: "WM_ERASEBKGND",
    0x0018: "WM_SHOWWINDOW", 0x001A: "WM_WININICHANGE", 0x0024: "WM_GETMINMAXINFO",
    0x0081: "WM_NCCREATE", 0x0082: "WM_NCDESTROY", 0x0083: "WM_NCCALCSIZE", 0x0084: "WM_NCHITTEST",
    0x0085: "WM_NCPAINT", 0x0086: "WM_NCACTIVATE", 0x0100: "WM_KEYDOWN", 0x0101: "WM_KEYUP",
    0x0102: "WM_CHAR", 0x0110: "WM_INITDIALOG", 0x0111: "WM_COMMAND", 0x0112: "WM_SYSCOMMAND",
    0x0113: "WM_TIMER", 0x0114: "WM_HSCROLL", 0x0115: "WM_VSCROLL", 0x0116: "WM_INITMENU",
    0x0117: "WM_INITMENUPOPUP", 0x011F: "WM_MENUSELECT", 0x0120: "WM_MENUCHAR",
    0x0132: "WM_CTLCOLOR", 0x0200: "WM_MOUSEMOVE", 0x0201: "WM_LBUTTONDOWN", 0x0202: "WM_LBUTTONUP",
    0x0203: "WM_LBUTTONDBLCLK", 0x0204: "WM_RBUTTONDOWN", 0x0205: "WM_RBUTTONUP",
    0x0210: "WM_PARENTNOTIFY", 0x0214: "WM_SIZING", 0x0220: "WM_MDICREATE",
    0x0300: "WM_CUT", 0x0301: "WM_COPY", 0x0302: "WM_PASTE",
}
_CORE = {0x0001, 0x0002, 0x000F, 0x0010, 0x0110, 0x0111, 0x0112, 0x0005, 0x0081}


def messages_in(code):
    """The set of WM_ names the code compares against, or {} unless a real dispatch cluster is seen."""
    found = set()
    n = len(code)
    i = 0
    while i + 3 <= n:
        b = code[i]
        if b == 0x3D:                                        # cmp ax, imm16
            v = code[i + 1] | (code[i + 2] << 8)
            if v in _WM:
                found.add(v)
            i += 3
        elif b == 0x81 and 0xF8 <= code[i + 1] <= 0xFF and i + 4 <= n:   # cmp r16, imm16 (/7 reg-direct)
            v = code[i + 2] | (code[i + 3] << 8)
            if v in _WM:
                found.add(v)
            i += 4
        else:
            i += 1
    if len(found) >= 2 and (found & _CORE):                  # a genuine message dispatch
        return {v: _WM[v] for v in found}
    return {}


def messages_for(nef, seg_idx, lo_off, hi_off):
    """WM_ messages handled by the function in [lo,hi) of a code segment."""
    segs = {s["idx"]: s for s in nef.segments()}
    s = segs.get(seg_idx)
    if not s or s["kind"] != "CODE":
        return {}
    code = nef.data[s["file_off"] + lo_off:s["file_off"] + hi_off]
    return messages_in(code)
