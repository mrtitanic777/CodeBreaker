#!/usr/bin/env python3
r"""studio/app.py -- CodeBreaker Workbench: the unified, native Win32/GDI front end.

One owner-drawn WS_POPUP window that drives the WHOLE tool -- the same pixel-faithful
Win95 recipe as progressui.py (double-buffered 3D frame, 48-band blue gradient title,
MS Sans Serif, sunken property-sheet panels, DICED blue-block progress trough, 95/XP/
Vista/7 themes), now with:

  * a left NAV tree that exposes every subsystem in plain language,
  * an ANALYZE front door (drop a binary -> engine pipeline) for non-technical users,
  * a generic COMMAND panel that runs any `wt <subsystem> ...` and streams its output
    live into a native read-only EDIT console (this is what unifies the Xbox CLI and the
    CodeBreaker CLI under one UI),
  * a SETTINGS/DOCTOR panel: attach-your-own toolchains, one config, one doctor, with
    "Get it" links to the official downloads.

    wt ui                         # launch the workbench
    python app.py --selftest      # create + destroy the window (plumbing check)
    python app.py --shot out.png  # render the current screen to a PNG (dev/preview)
"""
import os
import sys
import queue
import shlex
import struct
import threading
import subprocess

_HERE = os.path.dirname(os.path.abspath(__file__))
_ROOT = os.path.dirname(os.path.dirname(_HERE))
if _ROOT not in sys.path:
    sys.path.insert(0, _ROOT)

# --------------------------------------------------------------------------- #
# Static model (no ctypes) -- the nav, the command specs, the dependency table.
# --------------------------------------------------------------------------- #
# nav entry kinds:  ("grp", label)  group header
#                   ("act", label, action)   clickable row; action tuples below
#   ("analyze",)                      -> the drop-a-binary front door
#   ("cmd", subsystem, default_args, blurb)   -> generic command runner
#   ("settings",)                     -> attach-your-tools / doctor
NAV = [
    ("act", "Analyze a binary", "binary", ("analyze",)),
    ("grp", "Take apart"),
    ("act", "Disassemble", "disasm", ("cmd", "inverse", "list",
                            "Disassemble to editable, reassembleable assembly.")),
    ("act", "Decompile to C", "cfile", ("cmd", "recompile", "list",
                            "Lift proven-exact assembly into readable, structured C.")),
    ("grp", "Rebuild - byte-exact"),
    ("act", "Inverse (LC / MSC 2.03)", "inverse", ("cmd", "inverse", "list",
                            "Invert the Lattice C / MS C 2.03 codegen to byte-exact C.")),
    ("act", "MSC 6.0a (Win95 DLLs)", "chip", ("cmd", "msc6", "list",
                            "Reconstruct 16-bit NE DLLs to source that re-links to the exact bytes.")),
    ("act", "Recompile", "gear", ("cmd", "recompile", "list",
                            "Take the recovered C the rest of the way: compile -> link.")),
    ("grp", "Compare & explore"),
    ("act", "Compare versions (Diff)", "diff", ("cmd", "diff", "versions",
                            "Fingerprint every function and match it across releases.")),
    ("act", "Xbox 360", "xbox", ("cmd", "xbox", "",
                            "XEX2 / PowerPC reconstruction + package, crypto & forensics tools.")),
    ("grp", "Observe & reconstruct (dynamic)"),
    ("act", "Behavioral engine", "binary", ("cmd", "behavior", "list",
                            "Run the binary as an oracle: trace, experiment, form hypotheses, "
                            "graph dependencies, and synthesize verified C.")),
    ("grp", "Build & set up"),
    ("act", "Toolchain (backbone)", "backbone", ("cmd", "backbone", "list",
                            "One uniform interface over compilers / assemblers / linkers.")),
    ("act", "SDK / DDK ingest", "sdk", ("cmd", "sdk", "list",
                            "Parse a kit's headers/defs into typed signatures.")),
    ("act", "Settings & tools", "settings", ("settings",)),
]

# Pixel-art icons drawn natively (no files): each is a list of (x,y,w,h,palette_key) rects
# on a 16x16 grid. Palette keys resolve in ICON_PAL (built once inside _run).
ICONS = {
    "binary":   [(2,3,12,10,"NV"),(3,4,10,8,"LB"),(5,6,6,1,"NV"),(5,8,6,1,"NV"),(5,10,4,1,"NV")],
    "disasm":   [(3,2,10,12,"BK"),(4,4,7,1,"GN"),(4,6,5,1,"CY"),(4,8,6,1,"CY"),(4,10,4,1,"GN")],
    "cfile":    [(3,2,10,12,"W"),(3,2,10,1,"DG"),(3,13,10,1,"DG"),(3,2,1,12,"DG"),(12,2,1,12,"DG"),
                 (5,6,2,1,"NV"),(5,7,1,3,"NV"),(5,9,2,1,"NV"),(9,6,2,1,"NV"),(10,7,1,3,"NV"),(9,9,2,1,"NV")],
    "inverse":  [(7,2,2,1,"PP"),(6,3,4,1,"PU"),(4,4,8,2,"PP"),(3,6,10,1,"PU"),(4,7,8,2,"PU"),(6,9,4,2,"PU"),(7,11,2,1,"PU")],
    "chip":     [(3,3,10,10,"NV"),(4,4,8,8,"LG"),(5,5,6,3,"NV"),(1,5,2,1,"DG"),(1,8,2,1,"DG"),(1,11,2,1,"DG"),(13,5,2,1,"DG"),(13,8,2,1,"DG"),(13,11,2,1,"DG")],
    "gear":     [(6,1,4,3,"DG"),(6,12,4,3,"DG"),(1,6,3,4,"DG"),(12,6,3,4,"DG"),(3,3,10,10,"DG"),(5,5,6,6,"W"),(7,7,2,2,"DG")],
    "diff":     [(2,3,5,10,"CY"),(3,4,3,2,"W"),(3,7,3,1,"NV"),(9,3,5,10,"RD"),(10,4,3,2,"W"),(10,7,3,1,"K")],
    "xbox":     [(3,3,10,10,"GR"),(4,4,2,1,"W"),(5,5,2,1,"W"),(6,6,2,2,"W"),(8,8,2,1,"W"),(9,9,2,1,"W"),(9,4,2,1,"W"),(8,5,2,1,"W"),(6,8,2,1,"W"),(5,9,2,1,"W")],
    "backbone": [(2,6,6,4,"DG"),(3,7,4,2,"LG"),(8,6,6,4,"DG"),(9,7,4,2,"LG"),(7,7,2,2,"DG")],
    "sdk":      [(2,5,12,8,"YL"),(2,5,12,2,"DG"),(6,3,4,2,"DG"),(2,9,12,1,"YP")],
    "settings": [(6,1,4,3,"DG"),(6,12,4,3,"DG"),(1,6,3,4,"DG"),(12,6,3,4,"DG"),(3,3,10,10,"DG"),(5,5,6,6,"W"),(7,7,2,2,"DG")],
    "folder":   [(1,5,14,9,"YL"),(1,4,7,2,"YL"),(2,6,12,1,"YP")],
    "lens":     [(2,2,9,9,"DG"),(3,3,7,7,"CY"),(4,4,5,5,"W"),(10,10,3,3,"DG"),(11,11,3,3,"K")],
    "stop":     [(4,3,8,8,"RD"),(3,4,10,6,"RD")],
    "floppy":   [(2,2,12,12,"NV"),(4,3,6,4,"LG"),(8,3,1,3,"K"),(4,9,8,5,"W"),(5,10,6,1,"DG")],
}

# The four plain-language action cards on the Analyze front door.
CARDS = [
    ("Show the assembly", "See the exact machine code, laid out to read.", "disasm",
     ("navlabel", "Disassemble")),
    ("Turn it into C", "Readable, structured C you can actually follow.", "cfile",
     ("navlabel", "Decompile to C")),
    ("Rebuild it byte-for-byte", "Recover source that recompiles to the exact bytes.", "chip",
     ("navlabel", "MSC 6.0a (Win95 DLLs)")),
    ("Compare to another version", "Match this against another release, function by function.", "diff",
     ("navlabel", "Compare versions (Diff)")),
]

# dependency rows for the Settings panel: (config_key, display, url, optional, note)
DEPS = [
    ("dosbox_x",      "DOSBox-X",           "https://dosbox-x.com/",                 False, "DOS emulator for the period toolchains"),
    ("msc6_toolchain","MS C 6.0a toolchain","https://winworldpc.com/product/microsoft-c/60", False, "provide your own licensed copy"),
    ("msc203_oracle", "MS C 2.03 oracle",   "https://winworldpc.com/product/microsoft-c/2x", False, "the inverse-engine oracle (MC1/MC2)"),
    ("nasm",          "NASM (nasm-masm)",   "https://www.nasm.us/",                  False, "native --masm assembler; build tools/nasm-masm"),
    ("sevenzip",      "7-Zip",              "https://www.7-zip.org/",                True,  "archive extraction"),
    ("ghidra",        "Ghidra",             "https://ghidra-sre.org/",               True,  "optional external analyzer"),
]


def _load_cfg():
    try:
        from engine.foundation import config
        return config.load_config() or {}
    except Exception:
        return {}


def _wt_argv(rest):
    """argv to invoke the CLI -- the frozen exe re-invokes itself; from source, wt.py."""
    if getattr(sys, "frozen", False):
        return [sys.executable] + rest
    return [sys.executable, os.path.join(_ROOT, "wt.py")] + rest


def _write_png(path, w, h, rgb):
    import zlib

    def chunk(typ, data):
        return (struct.pack(">I", len(data)) + typ + data
                + struct.pack(">I", zlib.crc32(typ + data) & 0xffffffff))
    raw = bytearray()
    stride = w * 3
    for y in range(h):
        raw.append(0)
        raw += rgb[y * stride:(y + 1) * stride]
    out = (b"\x89PNG\r\n\x1a\n"
           + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0))
           + chunk(b"IDAT", zlib.compress(bytes(raw), 6))
           + chunk(b"IEND", b""))
    open(path, "wb").write(out)


def main(argv=None):
    argv = argv or []
    if sys.platform != "win32":
        print("CodeBreaker Workbench is a native Windows UI. On this platform use the"
              " terminal:\n  wt <subsystem> ...  |  wt --target <era> <cmd>  |  wt doctor")
        return
    shot = shot_panel = None
    if "--shot" in argv:
        i = argv.index("--shot")
        shot = argv[i + 1] if i + 1 < len(argv) else "codebreaker.png"
        if i + 2 < len(argv) and not argv[i + 2].startswith("-"):
            shot_panel = argv[i + 2]
    _run(selftest="--selftest" in argv, shot=shot, shot_panel=shot_panel)


def _run(selftest=False, shot=None, shot_panel=None):
    import ctypes as C
    from ctypes import wintypes as W
    try:
        from engine.studio import pipeline
    except Exception:
        pipeline = None

    user32 = C.WinDLL("user32", use_last_error=True)
    gdi32 = C.WinDLL("gdi32")
    kernel32 = C.WinDLL("kernel32")
    shell32 = C.WinDLL("shell32")
    comdlg32 = C.WinDLL("comdlg32")
    LRESULT = C.c_ssize_t
    WNDPROC = C.WINFUNCTYPE(LRESULT, W.HWND, W.UINT, W.WPARAM, W.LPARAM)

    def RGB(r, g, b): return r | (g << 8) | (b << 16)
    FACE, WHITE, BLACK, GRAY = RGB(212, 208, 200), RGB(255, 255, 255), RGB(0, 0, 0), RGB(96, 96, 96)
    LGRAY, DGRAY = RGB(223, 223, 223), RGB(128, 128, 128)

    # theme = (name, grad_start, grad_end, caption, face, panel, text, dim, block, font)
    THEMES = [
        ("Windows 95", (10, 36, 106), (166, 202, 240), WHITE, FACE, WHITE, BLACK, GRAY, RGB(0, 0, 160), "MS Sans Serif"),
        ("XP Luna",    (0, 84, 227),  (60, 145, 245),  WHITE, RGB(236, 233, 216), WHITE, BLACK, RGB(90, 90, 90), RGB(20, 90, 220), "Tahoma"),
        ("Vista Aero", (60, 84, 120), (18, 26, 40),    RGB(234, 241, 248), RGB(52, 64, 82), RGB(28, 36, 50), RGB(234, 241, 248), RGB(150, 172, 196), RGB(60, 150, 240), "Segoe UI"),
        ("Windows 7",  (210, 229, 247), (255, 255, 255), RGB(20, 40, 70), RGB(238, 244, 251), WHITE, RGB(20, 30, 45), RGB(90, 105, 125), RGB(60, 130, 210), "Segoe UI"),
    ]
    GREEN, RED, AMBER = RGB(0, 150, 0), RGB(180, 40, 40), RGB(210, 150, 30)

    (WM_DESTROY, WM_PAINT, WM_ERASEBKGND, WM_SIZE, WM_LBUTTONDOWN, WM_NCLBUTTONDOWN,
     WM_DROPFILES, WM_SETFONT, WM_COMMAND) = (0x2, 0xF, 0x14, 0x5, 0x201, 0xA1, 0x233, 0x30, 0x111)
    WM_APP_LINE, WM_APP_DONE, WM_APP_ANALYZED = 0x8000, 0x8001, 0x8002
    WS_POPUP, WS_VISIBLE, WS_CLIPCHILDREN, WS_CHILD = 0x80000000, 0x10000000, 0x02000000, 0x40000000
    WS_VSCROLL, WS_BORDER = 0x00200000, 0x00800000
    ES_MULTILINE, ES_READONLY, ES_AUTOVSCROLL, ES_AUTOHSCROLL = 0x4, 0x800, 0x40, 0x80
    WS_EX_CLIENTEDGE = 0x200
    EM_SETSEL, EM_REPLACESEL, EM_SETReadOnly = 0xB1, 0xC2, 0xCF
    DT_CENTER, DT_VCENTER, DT_SINGLELINE, DT_LEFT, DT_WORDBREAK, DT_END = 0x1, 0x4, 0x20, 0x0, 0x10, 0x8000
    EDGE_RAISED, EDGE_SUNKEN, BF_RECT = 0x5, 0xA, 0xF
    TRANSPARENT, IDC_ARROW, SW_SHOW, SW_HIDE, HTCAPTION = 1, 32512, 5, 0, 2
    DFC_CAPTION, DFCS_CAPTIONCLOSE, DFC_BUTTON, DFCS_BUTTONPUSH = 1, 0, 4, 0x10
    FW_NORMAL, FW_BOLD, DEFAULT_CHARSET, SRCCOPY = 400, 700, 1, 0x00CC0020
    PW_RENDERFULLCONTENT, CREATE_NO_WINDOW = 0x2, 0x08000000
    TITLEH, MENUH, TOOLH, NAVW, STATH = 20, 18, 28, 210, 22

    class WNDCLASS(C.Structure):
        _fields_ = [("style", W.UINT), ("lpfnWndProc", WNDPROC), ("cbClsExtra", C.c_int),
                    ("cbWndExtra", C.c_int), ("hInstance", W.HINSTANCE), ("hIcon", W.HICON),
                    ("hCursor", W.HANDLE), ("hbrBackground", W.HBRUSH),
                    ("lpszMenuName", W.LPCWSTR), ("lpszClassName", W.LPCWSTR)]

    class MSG(C.Structure):
        _fields_ = [("hwnd", W.HWND), ("message", W.UINT), ("wParam", W.WPARAM),
                    ("lParam", W.LPARAM), ("time", W.DWORD), ("pt", W.POINT)]

    class PAINTSTRUCT(C.Structure):
        _fields_ = [("hdc", W.HDC), ("fErase", W.BOOL), ("rcPaint", W.RECT),
                    ("fRestore", W.BOOL), ("fIncUpdate", W.BOOL), ("rgbReserved", C.c_byte * 32)]

    class BITMAPINFOHEADER(C.Structure):
        _fields_ = [("biSize", W.DWORD), ("biWidth", C.c_long), ("biHeight", C.c_long),
                    ("biPlanes", W.WORD), ("biBitCount", W.WORD), ("biCompression", W.DWORD),
                    ("biSizeImage", W.DWORD), ("biXPelsPerMeter", C.c_long),
                    ("biYPelsPerMeter", C.c_long), ("biClrUsed", W.DWORD), ("biClrImportant", W.DWORD)]

    class OPENFILENAMEW(C.Structure):
        _fields_ = [("lStructSize", W.DWORD), ("hwndOwner", W.HWND), ("hInstance", W.HINSTANCE),
                    ("lpstrFilter", W.LPCWSTR), ("lpstrCustomFilter", W.LPWSTR), ("nMaxCustFilter", W.DWORD),
                    ("nFilterIndex", W.DWORD), ("lpstrFile", W.LPWSTR), ("nMaxFile", W.DWORD),
                    ("lpstrFileTitle", W.LPWSTR), ("nMaxFileTitle", W.DWORD), ("lpstrInitialDir", W.LPCWSTR),
                    ("lpstrTitle", W.LPCWSTR), ("Flags", W.DWORD), ("nFileOffset", W.WORD),
                    ("nFileExtension", W.WORD), ("lpstrDefExt", W.LPCWSTR), ("lCustData", W.LPARAM),
                    ("lpfnHook", W.LPVOID), ("lpTemplateName", W.LPCWSTR), ("pvReserved", W.LPVOID),
                    ("dwReserved", W.DWORD), ("FlagsEx", W.DWORD)]
    OFN_HIDEREADONLY, OFN_FILEMUSTEXIST, OFN_PATHMUSTEXIST, OFN_EXPLORER = 0x4, 0x1000, 0x800, 0x80000

    for fn, res, args in [
        (user32.DefWindowProcW, LRESULT, [W.HWND, W.UINT, W.WPARAM, W.LPARAM]),
        (user32.CreateWindowExW, W.HWND, [W.DWORD, W.LPCWSTR, W.LPCWSTR, W.DWORD, C.c_int,
            C.c_int, C.c_int, C.c_int, W.HWND, W.HMENU, W.HINSTANCE, W.LPVOID]),
        (user32.RegisterClassW, W.ATOM, [C.POINTER(WNDCLASS)]),
        (user32.SendMessageW, LRESULT, [W.HWND, W.UINT, W.WPARAM, W.LPARAM]),
        (user32.PostMessageW, W.BOOL, [W.HWND, W.UINT, W.WPARAM, W.LPARAM]),
        (user32.GetMessageW, W.BOOL, [C.POINTER(MSG), W.HWND, W.UINT, W.UINT]),
        (user32.TranslateMessage, W.BOOL, [C.POINTER(MSG)]),
        (user32.DispatchMessageW, LRESULT, [C.POINTER(MSG)]),
        (user32.DestroyWindow, W.BOOL, [W.HWND]),
        (user32.PostQuitMessage, None, [C.c_int]),
        (user32.ReleaseCapture, W.BOOL, []),
        (user32.LoadCursorW, W.HANDLE, [W.HINSTANCE, W.LPCWSTR]),
        (user32.GetSystemMetrics, C.c_int, [C.c_int]),
        (user32.BeginPaint, W.HDC, [W.HWND, C.POINTER(PAINTSTRUCT)]),
        (user32.EndPaint, W.BOOL, [W.HWND, C.POINTER(PAINTSTRUCT)]),
        (user32.GetClientRect, W.BOOL, [W.HWND, C.POINTER(W.RECT)]),
        (user32.FillRect, C.c_int, [W.HDC, C.POINTER(W.RECT), W.HBRUSH]),
        (user32.DrawEdge, W.BOOL, [W.HDC, C.POINTER(W.RECT), W.UINT, W.UINT]),
        (user32.DrawFrameControl, W.BOOL, [W.HDC, C.POINTER(W.RECT), W.UINT, W.UINT]),
        (user32.DrawTextW, C.c_int, [W.HDC, W.LPCWSTR, C.c_int, C.POINTER(W.RECT), W.UINT]),
        (user32.InvalidateRect, W.BOOL, [W.HWND, C.POINTER(W.RECT), W.BOOL]),
        (user32.ShowWindow, W.BOOL, [W.HWND, C.c_int]),
        (user32.UpdateWindow, W.BOOL, [W.HWND]),
        (user32.MoveWindow, W.BOOL, [W.HWND, C.c_int, C.c_int, C.c_int, C.c_int, W.BOOL]),
        (user32.GetWindowTextW, C.c_int, [W.HWND, W.LPWSTR, C.c_int]),
        (user32.GetWindowTextLengthW, C.c_int, [W.HWND]),
        (user32.MessageBoxW, C.c_int, [W.HWND, W.LPCWSTR, W.LPCWSTR, W.UINT]),
        (user32.GetDC, W.HDC, [W.HWND]),
        (user32.ReleaseDC, C.c_int, [W.HWND, W.HDC]),
        (user32.PrintWindow, W.BOOL, [W.HWND, W.HDC, W.UINT]),
        (kernel32.GetModuleHandleW, W.HMODULE, [W.LPCWSTR]),
        (gdi32.CreateSolidBrush, W.HBRUSH, [W.COLORREF]),
        (gdi32.CreateFontW, W.HFONT, [C.c_int] * 5 + [W.DWORD] * 8 + [W.LPCWSTR]),
        (gdi32.SelectObject, W.HGDIOBJ, [W.HDC, W.HGDIOBJ]),
        (gdi32.DeleteObject, W.BOOL, [W.HGDIOBJ]),
        (gdi32.SetTextColor, W.COLORREF, [W.HDC, W.COLORREF]),
        (gdi32.SetBkMode, C.c_int, [W.HDC, C.c_int]),
        (gdi32.CreateCompatibleDC, W.HDC, [W.HDC]),
        (gdi32.CreateCompatibleBitmap, W.HBITMAP, [W.HDC, C.c_int, C.c_int]),
        (gdi32.CreateDIBSection, W.HBITMAP, [W.HDC, C.c_void_p, W.UINT, C.POINTER(C.c_void_p), W.HANDLE, W.DWORD]),
        (gdi32.BitBlt, W.BOOL, [W.HDC, C.c_int, C.c_int, C.c_int, C.c_int, W.HDC, C.c_int, C.c_int, W.DWORD]),
        (gdi32.DeleteDC, W.BOOL, [W.HDC]),
        (shell32.DragAcceptFiles, None, [W.HWND, W.BOOL]),
        (shell32.DragQueryFileW, W.UINT, [W.HANDLE, W.UINT, W.LPWSTR, W.UINT]),
        (shell32.DragFinish, None, [W.HANDLE]),
        (shell32.ShellExecuteW, W.HINSTANCE, [W.HWND, W.LPCWSTR, W.LPCWSTR, W.LPCWSTR, W.LPCWSTR, C.c_int]),
        (comdlg32.GetOpenFileNameW, W.BOOL, [C.POINTER(OPENFILENAMEW)]),
    ]:
        fn.restype, fn.argtypes = res, args

    hinst = kernel32.GetModuleHandleW(None)
    _fonts, _brushes = {}, {}

    def font(name, size=-11, bold=False):
        k = (name, size, bold)
        if k not in _fonts:
            _fonts[k] = gdi32.CreateFontW(size, 0, 0, 0, FW_BOLD if bold else FW_NORMAL,
                                          0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, name)
        return _fonts[k]

    def brush(col):
        if col not in _brushes:
            _brushes[col] = gdi32.CreateSolidBrush(col)
        return _brushes[col]

    def R(l, t, r, b): return W.RECT(l, t, r, b)

    class S:
        theme = 0; panel = "analyze"; nav_sel = 0
        hwnd = None; hConsole = None; hArgs = None
        open_menu = None; menu_x = {}
        hits = []                      # [(x0,y0,x1,y1, action)]
        running = False; status = "Ready"; target = "win95"
        cmd = None                     # active command spec dict
        cmd_input = ""                 # chosen input file for the command
        report = None; stages = []; stage = -1; caption = ""
        cfg = {}
        _q = queue.Queue()

    def th(): return THEMES[S.theme]

    def draw_text(dc, s, r, col, flags, fnt):
        gdi32.SelectObject(dc, fnt); gdi32.SetBkMode(dc, TRANSPARENT); gdi32.SetTextColor(dc, col)
        rr = R(*r); user32.DrawTextW(dc, s, -1, C.byref(rr), flags | 0x800)   # DT_NOPREFIX

    def edge(dc, r, kind):
        rr = R(*r); user32.DrawEdge(dc, C.byref(rr), kind, BF_RECT)

    def fill(dc, r, col):
        user32.FillRect(dc, C.byref(R(*r)), brush(col))

    def hit(r, action):
        S.hits.append((r[0], r[1], r[2], r[3], action))

    def button(dc, r, label, fnt, txt, default=False):
        fill(dc, r, th()[4]); edge(dc, r, EDGE_RAISED)    # theme face + 3D bevel (not system color)
        draw_text(dc, label, (r[0], r[1], r[2], r[3]), txt, DT_CENTER | DT_VCENTER | DT_SINGLELINE, fnt)

    def dot(dc, x, y, col):
        fill(dc, (x, y, x + 9, y + 9), col)
        edge(dc, (x - 1, y - 1, x + 10, y + 10), EDGE_SUNKEN)

    ICON_PAL = {"K": BLACK, "W": WHITE, "LG": RGB(200, 200, 200), "DG": RGB(120, 120, 120),
                "NV": RGB(0, 0, 130), "BL": RGB(48, 72, 180), "LB": RGB(190, 214, 250),
                "CY": RGB(120, 180, 240), "YL": RGB(232, 195, 74), "YP": RGB(255, 242, 192),
                "GR": RGB(16, 124, 16), "GN": RGB(60, 200, 90), "RD": RGB(193, 67, 67),
                "PU": RGB(122, 92, 255), "PP": RGB(200, 190, 250), "BK": RGB(40, 40, 40)}

    def draw_icon(dc, name, x, y, scale=1):
        for (dx, dy, w, h, key) in ICONS.get(name, ()):
            fill(dc, (x + dx * scale, y + dy * scale, x + (dx + w) * scale, y + (dy + h) * scale),
                 ICON_PAL[key])

    def diced_bar(dc, r, frac, blockcol):
        bw, gap, pad, e = 8, 2, 2, 2
        edge(dc, r, EDGE_SUNKEN)
        fill(dc, (r[0] + e, r[1] + e, r[2] - e, r[3] - e), WHITE)
        avail = (r[2] - r[0]) - 2 * (e + pad)
        n = max(1, (avail + gap) // (bw + gap))
        x0, y0, y1 = r[0] + e + pad, r[1] + e + pad, r[3] - e - pad
        for k in range(int(min(1.0, frac) * n + 1e-6)):
            bx = x0 + k * (bw + gap)
            fill(dc, (bx, y0, bx + bw, y1), blockcol)

    def geom(w, h):
        y = 3 + TITLEH
        g = {"title": (3, 3, w - 3, 3 + TITLEH)}
        g["tool"] = (3, y, w - 3, y + TOOLH); y += TOOLH + 3
        g["nav"] = (6, y, 6 + NAVW, h - STATH - 3)
        g["main"] = (6 + NAVW + 4, y, w - 6, h - STATH - 3)
        g["status"] = (3, h - STATH, w - 3, h - 3)
        return g

    # ------------------------------------------------------------------ panels
    def draw_nav(dc, r):
        name, gs, ge, cap, face, panel, txt, dim, block, fname = th()
        fN, fB = font(fname), font(fname, bold=True)
        edge(dc, r, EDGE_SUNKEN); fill(dc, (r[0] + 2, r[1] + 2, r[2] - 2, r[3] - 2), WHITE)
        y = r[1] + 5
        for i, entry in enumerate(NAV):
            if entry[0] == "grp":
                y += 5
                draw_text(dc, entry[1], (r[0] + 8, y, r[2] - 6, y + 15), BLACK,
                          DT_LEFT | DT_VCENTER | DT_SINGLELINE, fB)
                y += 16
            else:
                sel = (i == S.nav_sel)
                row = (r[0] + 4, y, r[2] - 4, y + 20)
                if sel:
                    fill(dc, row, RGB(0, 0, 128))
                draw_icon(dc, entry[2], row[0] + 5, y + 3)
                draw_text(dc, entry[1], (row[0] + 26, y, row[2] - 4, y + 20),
                          WHITE if sel else BLACK, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN)
                hit(row, ("nav", i))
                y += 21

    def draw_analyze(dc, r):
        name, gs, ge, cap, face, panel, txt, dim, block, fname = th()
        fN, fB, fH = font(fname), font(fname, bold=True), font(fname, -15, True)
        x0, x1 = r[0], r[2]
        draw_text(dc, "Take a program apart", (x0, r[1] + 2, x1, r[1] + 22), BLACK,
                  DT_LEFT | DT_SINGLELINE, fH)
        draw_text(dc, "Drop a Windows or Xbox 360 program below. CodeBreaker works out what it is on "
                  "its own, then shows you - in plain terms - what it can do with it. You don't need to "
                  "know the file format.", (x0, r[1] + 24, x1, r[1] + 58), BLACK, DT_LEFT | DT_WORDBREAK, fN)
        dz = (x0, r[1] + 62, x1, r[1] + 160)
        edge(dc, dz, EDGE_SUNKEN); fill(dc, (dz[0] + 2, dz[1] + 2, dz[2] - 2, dz[3] - 2), WHITE)
        rep = S.report or {}
        cx = (dz[0] + dz[2]) // 2
        draw_icon(dc, "floppy", cx - 16, dz[1] + 10, 2)
        title = rep.get("name") or "Drop a program here to take it apart"
        draw_text(dc, title, (dz[0], dz[1] + 46, dz[2], dz[1] + 66), BLACK, DT_CENTER | DT_SINGLELINE, fB)
        draw_text(dc, "or click to browse    .exe  .dll  .drv  .vxd  .386  default.xex",
                  (dz[0], dz[1] + 68, dz[2], dz[1] + 86), dim, DT_CENTER | DT_SINGLELINE, fN)
        hit(dz, ("analyze_browse",))
        y = dz[3] + 8
        # detected bar with tags
        if rep.get("format"):
            bar = (x0, y, x1, y + 24); edge(dc, bar, EDGE_SUNKEN)
            fill(dc, (bar[0] + 2, bar[1] + 2, bar[2] - 2, bar[3] - 2), RGB(239, 239, 224))
            draw_text(dc, "  " + rep.get("name", ""), (bar[0] + 6, bar[1], bar[0] + 170, bar[3]),
                      BLACK, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fB)
            draw_text(dc, "just analyzed", (bar[0] + 172, bar[1], bar[0] + 262, bar[3]),
                      dim, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN)
            tg = rep.get("format", ""); tw = 8 * len(tg) + 14
            fill(dc, (bar[0] + 264, bar[1] + 4, bar[0] + 264 + tw, bar[3] - 4), RGB(0, 0, 130))
            draw_text(dc, tg, (bar[0] + 264, bar[1], bar[0] + 264 + tw, bar[3]), WHITE,
                      DT_CENTER | DT_VCENTER | DT_SINGLELINE, fN)
            y += 30
        # four plain-language action cards (2x2)
        cw = (x1 - x0 - 8) // 2; ch = 54
        for idx, (ct, cd, cic, cact) in enumerate(CARDS):
            bx = x0 + (idx % 2) * (cw + 8); by = y + (idx // 2) * (ch + 8)
            card = (bx, by, bx + cw, by + ch)
            fill(dc, card, face); edge(dc, card, EDGE_RAISED)
            draw_icon(dc, cic, bx + 11, by + 11)
            draw_text(dc, ct, (bx + 34, by + 8, bx + cw - 8, by + 26), BLACK, DT_LEFT | DT_SINGLELINE, fB)
            draw_text(dc, cd, (bx + 34, by + 25, bx + cw - 8, by + ch - 5), BLACK, DT_LEFT | DT_WORDBREAK, fN)
            hit(card, cact)
        y += 2 * (ch + 8) + 4
        # live progress + artifact buttons after an analyze run
        if S.running and S.stages:
            frac = 0.0 if S.stage < 0 else min(1.0, (S.stage + 1) / (len(S.stages) + 1.0))
            diced_bar(dc, (x0, y, x1 - 120, y + 20), frac, block)
            draw_text(dc, S.caption or "working...", (x1 - 112, y, x1, y + 20), dim,
                      DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN); y += 26
        bx = x0
        arts = (rep.get("artifacts") or [])[:3]
        if rep.get("outdir"):
            arts = arts + [("Open output folder", rep["outdir"])]
        for label, p in arts:
            b = (bx, y, bx + 190, y + 22); button(dc, b, label, fN, BLACK); hit(b, ("open", p)); bx += 198

    def draw_command(dc, r):
        name, gs, ge, cap, face, panel, txt, dim, block, fname = th()
        fN, fB, fH = font(fname), font(fname, bold=True), font(fname, -15, True)
        spec = S.cmd or {"sub": "", "blurb": ""}
        x0, x1 = r[0], r[2]
        draw_text(dc, "wt %s" % spec["sub"], (x0, r[1] + 2, x1, r[1] + 22), BLACK, DT_LEFT | DT_SINGLELINE, fH)
        draw_text(dc, spec.get("blurb", ""), (x0, r[1] + 24, x1, r[1] + 44), BLACK, DT_LEFT | DT_WORDBREAK, fN)
        # input file row
        draw_text(dc, "Input", (x0, r[1] + 50, x0 + 52, r[1] + 73), BLACK, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN)
        fld = (x0 + 56, r[1] + 50, x1 - 90, r[1] + 73); edge(dc, fld, EDGE_SUNKEN)
        fill(dc, (fld[0] + 2, fld[1] + 2, fld[2] - 2, fld[3] - 2), WHITE)
        draw_text(dc, S.cmd_input or "(optional - pick a file)", (fld[0] + 6, fld[1], fld[2] - 4, fld[3]),
                  BLACK if S.cmd_input else dim, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN)
        bb = (x1 - 84, r[1] + 50, x1, r[1] + 73); button(dc, bb, "Browse", fN, BLACK); hit(bb, ("cmd_browse",))
        # args label (the child EDIT is positioned by layout() right under here)
        draw_text(dc, "Arguments", (x0, r[1] + 80, x0 + 90, r[1] + 100), BLACK, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN)
        # Run / Stop
        rb = (x1 - 168, r[1] + 106, x1 - 88, r[1] + 129); button(dc, rb, "Run", fB, BLACK); hit(rb, ("run",))
        sb = (x1 - 84, r[1] + 106, x1, r[1] + 129); button(dc, sb, "Stop", fN, BLACK); hit(sb, ("stop",))
        draw_text(dc, "Output", (x0, r[1] + 134, x1, r[1] + 150), dim, DT_LEFT | DT_SINGLELINE, fB)
        # console child EDIT occupies r[1]+152 .. r[3]-2 (see layout)

    def draw_settings(dc, r):
        name, gs, ge, cap, face, panel, txt, dim, block, fname = th()
        fN, fB, fH = font(fname), font(fname, bold=True), font(fname, -15, True)
        x0, x1 = r[0], r[2]
        draw_text(dc, "Attach your tools", (x0, r[1] + 2, x1, r[1] + 22), BLACK, DT_LEFT | DT_SINGLELINE, fH)
        draw_text(dc, "CodeBreaker ships as the tool only - point it at the compilers you have. "
                  "Everything works without these except the byte-for-byte rebuild loop.",
                  (x0, r[1] + 24, x1, r[1] + 56), BLACK, DT_LEFT | DT_WORDBREAK, fN)
        y = r[1] + 62
        for key, disp, url, optional, note in DEPS:
            val = S.cfg.get(key, "")
            ok = bool(val) and (os.path.exists(val) or _which(val))
            col = GREEN if ok else (AMBER if optional else RED)
            dot(dc, x0 + 2, y + 6, col)
            draw_text(dc, disp, (x0 + 18, y, x0 + 170, y + 20), BLACK, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fB)
            st = "configured" if ok else ("optional" if optional else "not set")
            draw_text(dc, st, (x0 + 172, y, x0 + 250, y + 20), dim, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN)
            fld = (x0 + 254, y, x1 - 74, y + 20); edge(dc, fld, EDGE_SUNKEN)
            fill(dc, (fld[0] + 2, fld[1] + 2, fld[2] - 2, fld[3] - 2), WHITE)
            draw_text(dc, val or note, (fld[0] + 5, fld[1], fld[2] - 4, fld[3]),
                      BLACK if val else dim, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN)
            gb = (x1 - 70, y, x1, y + 20); button(dc, gb, "Get it", fN, BLACK); hit(gb, ("url", url))
            y += 26
        y += 4
        db = (x0, y, x0 + 110, y + 23); button(dc, db, "Run Doctor", fB, BLACK); hit(db, ("doctor",))
        cb = (x0 + 120, y, x0 + 230, y + 23); button(dc, cb, "Open config", fN, BLACK)
        hit(cb, ("open", os.path.join(_ROOT, "config.json")))

    def draw(dc, w, h):
        name, gs, ge, cap, face, panel, txt, dim, block, fname = th()
        fN, fB = font(fname), font(fname, bold=True)
        S.hits = []
        fill(dc, (0, 0, w, h), face); edge(dc, (0, 0, w, h), EDGE_RAISED)
        g = geom(w, h)
        # title bar gradient
        for i in range(48):
            t = i / 47.0
            col = RGB(int(gs[0] + (ge[0] - gs[0]) * t), int(gs[1] + (ge[1] - gs[1]) * t),
                      int(gs[2] + (ge[2] - gs[2]) * t))
            fill(dc, (3 + (w - 6) * i // 48, 3, 3 + (w - 6) * (i + 1) // 48, 3 + TITLEH), col)
        draw_text(dc, "CodeBreaker  -  Reverse Engineering Workbench", (10, 3, w - 24, 3 + TITLEH),
                  cap, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fB)
        xb = (w - 20, 6, w - 6, 6 + 14)
        user32.DrawFrameControl(dc, C.byref(R(*xb)), DFC_CAPTION, DFCS_CAPTIONCLOSE); hit(xb, ("close",))
        # toolbar: quick icon buttons (left) + theme swatches (right)
        edge(dc, g["tool"], EDGE_SUNKEN)
        tx = g["tool"][0] + 4; ty = g["tool"][1] + 3; tb = g["tool"][3] - 3
        for label, ic, act in (("Open", "folder", ("analyze_browse",)),
                               ("Analyze", "lens", ("nav", 0)),
                               ("Diff", "diff", ("navlabel", "Compare versions (Diff)")),
                               ("Xbox", "xbox", ("navlabel", "Xbox 360")),
                               ("Stop", "stop", ("stop",))):
            bw = 8 * len(label) + 30; b = (tx, ty, tx + bw, tb)
            button(dc, b, "", fN, BLACK)
            draw_icon(dc, ic, b[0] + 5, (ty + tb) // 2 - 8)
            draw_text(dc, label, (b[0] + 24, b[1], b[2] - 4, b[3]), BLACK,
                      DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN)
            hit(b, act); tx += bw + 3
        sx = g["tool"][2] - len(THEMES) * 22 - 6
        draw_text(dc, "Look:", (sx - 34, ty, sx - 4, tb), BLACK, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN)
        for i, tdef in enumerate(THEMES):
            box = (sx + i * 22, ty + 2, sx + i * 22 + 18, tb - 1)
            fill(dc, box, RGB(*tdef[2])); edge(dc, box, EDGE_SUNKEN if i == S.theme else EDGE_RAISED)
            hit(box, ("theme", i))
        # nav + active panel
        draw_nav(dc, g["nav"])
        m = g["main"]; pad = (m[0] + 10, m[1] + 8, m[2] - 10, m[3] - 8)
        if S.panel == "analyze":
            draw_analyze(dc, pad)
        elif S.panel == "settings":
            draw_settings(dc, pad)
        else:
            draw_command(dc, pad)
        # status bar: message (left) + oracle LED + clickable target (right)
        st = g["status"]; edge(dc, st, EDGE_SUNKEN)
        tgt = (st[2] - 112, st[1], st[2] - 8, st[3])
        draw_text(dc, "Target: %s" % S.target, (tgt[0], tgt[1], tgt[2], tgt[3]), BLACK,
                  DT_CENTER | DT_VCENTER | DT_SINGLELINE, fN)
        edge(dc, (tgt[0], tgt[1] + 2, tgt[2], tgt[3] - 2), EDGE_SUNKEN); hit(tgt, ("target_cycle",))
        oracle_ok = bool(S.cfg.get("msc203_oracle"))
        dot(dc, tgt[0] - 128, st[1] + 6, GREEN if oracle_ok else RED)
        draw_text(dc, "Oracle: %s" % ("connected" if oracle_ok else "not set"),
                  (tgt[0] - 114, st[1], tgt[0] - 8, st[3]), BLACK, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN)
        draw_text(dc, "  " + S.status, (st[0], st[1], tgt[0] - 138, st[3]), BLACK,
                  DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN)

    def paint(hwnd):
        ps = PAINTSTRUCT(); hdc = user32.BeginPaint(hwnd, C.byref(ps))
        cr = W.RECT(); user32.GetClientRect(hwnd, C.byref(cr)); w, h = cr.right, cr.bottom
        mem = gdi32.CreateCompatibleDC(hdc); bmp = gdi32.CreateCompatibleBitmap(hdc, w, h)
        old = gdi32.SelectObject(mem, bmp)
        try:
            draw(mem, w, h); gdi32.BitBlt(hdc, 0, 0, w, h, mem, 0, 0, SRCCOPY)
        finally:
            gdi32.SelectObject(mem, old); gdi32.DeleteObject(bmp); gdi32.DeleteDC(mem)
            user32.EndPaint(hwnd, C.byref(ps))

    # ------------------------------------------------------------- child EDITs
    def layout():
        if not (S.hwnd and S.hConsole):
            return
        cr = W.RECT(); user32.GetClientRect(S.hwnd, C.byref(cr)); w, h = cr.right, cr.bottom
        g = geom(w, h); m = g["main"]; pad = (m[0] + 10, m[1] + 8, m[2] - 10, m[3] - 8)
        if S.panel == "command":
            user32.MoveWindow(S.hArgs, pad[0] + 92, pad[1] + 80, (pad[2] - pad[0]) - 260, 22, True)
            user32.ShowWindow(S.hArgs, SW_SHOW)
            user32.MoveWindow(S.hConsole, pad[0], pad[1] + 152, pad[2] - pad[0], pad[3] - (pad[1] + 152), True)
            user32.ShowWindow(S.hConsole, SW_SHOW)
        elif S.panel == "settings":
            user32.ShowWindow(S.hArgs, SW_HIDE)
            user32.MoveWindow(S.hConsole, pad[0], pad[3] - 96, pad[2] - pad[0], 96, True)
            user32.ShowWindow(S.hConsole, SW_SHOW)
        else:
            user32.ShowWindow(S.hArgs, SW_HIDE); user32.ShowWindow(S.hConsole, SW_HIDE)

    def console_set(text):
        buf = C.create_unicode_buffer(text)
        user32.SendMessageW(S.hConsole, 0x000C, 0, C.addressof(buf))   # WM_SETTEXT

    def console_append(text):
        n = user32.GetWindowTextLengthW(S.hConsole)
        user32.SendMessageW(S.hConsole, EM_SETSEL, n, n)
        buf = C.create_unicode_buffer(text)
        user32.SendMessageW(S.hConsole, EM_REPLACESEL, 0, C.addressof(buf))

    def get_args():
        n = user32.GetWindowTextLengthW(S.hArgs)
        buf = C.create_unicode_buffer(n + 1); user32.GetWindowTextW(S.hArgs, buf, n + 1)
        return buf.value

    def set_args(text):
        buf = C.create_unicode_buffer(text)
        user32.SendMessageW(S.hArgs, 0x000C, 0, C.addressof(buf))      # WM_SETTEXT

    # --------------------------------------------------------------- behaviour
    def open_path(p):
        try:
            shell32.ShellExecuteW(None, "open", p, None, None, SW_SHOW)
        except Exception:
            pass

    def browse(filt=None):
        buf = C.create_unicode_buffer(1024)
        flt = C.create_unicode_buffer(filt or
              "All files\x00*.*\x00Windows binaries\x00*.exe;*.dll;*.drv;*.vxd;*.386\x00")
        ofn = OPENFILENAMEW(); ofn.lStructSize = C.sizeof(OPENFILENAMEW); ofn.hwndOwner = S.hwnd
        ofn.lpstrFile = C.cast(buf, W.LPWSTR); ofn.nMaxFile = 1024
        ofn.lpstrFilter = C.cast(flt, W.LPCWSTR); ofn.nFilterIndex = 1
        ofn.lpstrTitle = "Select a file"
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER
        return buf.value if comdlg32.GetOpenFileNameW(C.byref(ofn)) else None

    def repaint():
        if S.hwnd:
            user32.InvalidateRect(S.hwnd, None, False)

    def desktop_dir():
        home = os.path.expanduser("~")
        for d in (os.path.join(home, "Desktop"), os.path.join(home, "OneDrive", "Desktop")):
            if os.path.isdir(d):
                return d
        return home

    def analyze(path):
        if not path or S.running or pipeline is None:
            return
        S.running = True; S.report = None; S.stages = []; S.stage = 0; S.caption = "Reading..."
        S.status = "Analyzing " + os.path.basename(path); repaint()

        def prog(kind, *a):
            if kind == "stages":
                S.stages, S.stage = list(a[0]), -1
            elif kind == "stage":
                S.stage, S.caption = a[0], (a[1] if len(a) > 1 else "")
            repaint()

        def worker():
            try:
                S.report = pipeline.reverse(path, progress=prog, outbase=desktop_dir())
            except Exception as e:
                S.report = {"name": os.path.basename(path), "error": str(e)}
            S.running = False; user32.PostMessageW(S.hwnd, WM_APP_ANALYZED, 0, 0)
        threading.Thread(target=worker, daemon=True).start()

    def run_command():
        if S.running or not S.cmd:
            return
        rest = [S.cmd["sub"]]
        if S.cmd_input:
            rest.append(S.cmd_input)
        try:
            rest += shlex.split(get_args())
        except Exception:
            rest += get_args().split()
        argv = _wt_argv(rest)
        console_set("$ wt %s\r\n" % " ".join(rest))
        S.running = True; S.status = "Running: wt " + " ".join(rest); repaint()

        def worker():
            try:
                p = subprocess.Popen(argv, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                     cwd=_ROOT, text=True, bufsize=1, creationflags=CREATE_NO_WINDOW)
                for line in iter(p.stdout.readline, ""):
                    S._q.put(line.rstrip("\n"))
                    user32.PostMessageW(S.hwnd, WM_APP_LINE, 0, 0)
                p.wait()
            except Exception as e:
                S._q.put("[error] %s" % e); user32.PostMessageW(S.hwnd, WM_APP_LINE, 0, 0)
            S.running = False; user32.PostMessageW(S.hwnd, WM_APP_DONE, 0, 0)
        threading.Thread(target=worker, daemon=True).start()

    def run_doctor():
        S.panel = "settings"; layout()
        S.cmd = {"sub": "doctor", "blurb": ""}
        # reuse the streaming runner but keep the settings view
        argv = _wt_argv(["doctor"]); console_set("$ wt doctor\r\n")
        S.running = True; repaint()

        def worker():
            try:
                p = subprocess.Popen(argv, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                     cwd=_ROOT, text=True, bufsize=1, creationflags=CREATE_NO_WINDOW)
                for line in iter(p.stdout.readline, ""):
                    S._q.put(line.rstrip("\n")); user32.PostMessageW(S.hwnd, WM_APP_LINE, 0, 0)
                p.wait()
            except Exception as e:
                S._q.put("[error] %s" % e); user32.PostMessageW(S.hwnd, WM_APP_LINE, 0, 0)
            S.running = False; user32.PostMessageW(S.hwnd, WM_APP_DONE, 0, 0)
        threading.Thread(target=worker, daemon=True).start()

    def find_nav(label):
        for i, e in enumerate(NAV):
            if e[0] == "act" and e[1] == label:
                return i
        return None

    def select_nav(i):
        S.nav_sel = i; action = NAV[i][3]
        kind = action[0]
        if kind == "analyze":
            S.panel = "analyze"
        elif kind == "settings":
            S.panel = "settings"; S.cfg = _load_cfg()
        elif kind == "cmd":
            S.panel = "command"
            S.cmd = {"sub": action[1], "blurb": action[3]}; S.cmd_input = ""
            set_args(action[2])
        layout(); repaint()

    def dispatch(action):
        k = action[0]
        if k == "close":
            user32.DestroyWindow(S.hwnd)
        elif k == "nav":
            select_nav(action[1])
        elif k == "navlabel":
            i = find_nav(action[1])
            if i is not None:
                select_nav(i)
        elif k == "theme":
            S.theme = action[1]; repaint()
        elif k == "analyze_browse":
            S.panel = "analyze"; layout(); analyze(browse(
                "Windows binaries\x00*.exe;*.dll;*.drv;*.vxd;*.386;*.ocx;*.cpl;*.sys\x00Xbox 360\x00*.xex\x00All files\x00*.*\x00"))
        elif k == "cmd_browse":
            p = browse()
            if p:
                S.cmd_input = p; repaint()
        elif k == "run":
            run_command()
        elif k == "stop":
            S.status = "Stopped."; S.running = False; repaint()
        elif k == "doctor":
            run_doctor()
        elif k == "url":
            open_path(action[1])
        elif k == "open":
            open_path(action[1])
        elif k == "target_cycle":
            eras = ["win1.0", "win3.1", "win95", "win98"]
            S.target = eras[(eras.index(S.target) + 1) % len(eras)] if S.target in eras else "win95"
            repaint()
        elif k == "clear_console":
            console_set(""); repaint()
        elif k == "about":
            user32.MessageBoxW(S.hwnd, "CodeBreaker Workbench\n\nA deterministic, evidence-only "
                "reverse-engineering workbench.\nNative Win32/GDI.", "About CodeBreaker", 0x40)
        elif k == "target_set":
            S.target = action[1]; repaint()
        elif k == "refresh":
            S.cfg = _load_cfg(); repaint()
        elif k == "noop":
            pass

    def on_click(x, y):
        for hh in reversed(S.hits):                       # last drawn = topmost wins
            if hh[0] <= x <= hh[2] and hh[1] <= y <= hh[3]:
                dispatch(hh[4]); return True
        return False

    def on_drop(hdrop):
        buf = C.create_unicode_buffer(1024); shell32.DragQueryFileW(hdrop, 0, buf, 1024)
        shell32.DragFinish(hdrop); S.panel = "analyze"; S.nav_sel = 0; layout(); analyze(buf.value)

    def drain_queue():
        chunk = []
        try:
            while True:
                chunk.append(S._q.get_nowait())
        except queue.Empty:
            pass
        if chunk:
            console_append("\r\n".join(chunk) + "\r\n")

    def wndproc(hwnd, msg, wp, lp):
        if msg == WM_ERASEBKGND:
            return 1
        if msg == WM_PAINT:
            paint(hwnd); return 0
        if msg == WM_SIZE:
            layout(); user32.InvalidateRect(hwnd, None, False); return 0
        if msg == WM_DROPFILES:
            on_drop(wp); return 0
        if msg == WM_APP_LINE:
            drain_queue(); return 0
        if msg == WM_APP_DONE:
            S.status = "Done."; repaint(); return 0
        if msg == WM_APP_ANALYZED:
            S.stage = len(S.stages); r = S.report or {}
            S.caption = "error" if r.get("error") else ("done  -  " + r.get("format", ""))
            S.status = "Ready"; repaint(); return 0
        if msg == WM_LBUTTONDOWN:
            x, y = lp & 0xFFFF, (lp >> 16) & 0xFFFF
            if on_click(x, y):
                return 0
            if y <= 3 + TITLEH:
                user32.ReleaseCapture(); user32.SendMessageW(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0)
            return 0
        if msg == WM_DESTROY:
            user32.PostQuitMessage(0); return 0
        return user32.DefWindowProcW(hwnd, msg, wp, lp)

    proc = WNDPROC(wndproc)
    cls = WNDCLASS(); cls.style = 0x0003; cls.lpfnWndProc = proc; cls.hInstance = hinst
    cls.hCursor = user32.LoadCursorW(None, C.c_wchar_p(IDC_ARROW)); cls.hbrBackground = None
    cls.lpszClassName = "CodeBreakerWorkbench"
    user32.RegisterClassW(C.byref(cls))
    sw, sh = user32.GetSystemMetrics(0), user32.GetSystemMetrics(1)
    ww, wh = 1000, 640
    S.hwnd = user32.CreateWindowExW(0, "CodeBreakerWorkbench", "CodeBreaker",
        WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN, (sw - ww) // 2, (sh - wh) // 3, ww, wh,
        None, None, hinst, None)
    S.cfg = _load_cfg()
    # child controls: the live console + the arguments field
    S.hConsole = user32.CreateWindowExW(WS_EX_CLIENTEDGE, "EDIT", "",
        WS_CHILD | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL, 0, 0, 10, 10,
        S.hwnd, None, hinst, None)
    S.hArgs = user32.CreateWindowExW(WS_EX_CLIENTEDGE, "EDIT", "",
        WS_CHILD | ES_AUTOHSCROLL, 0, 0, 10, 10, S.hwnd, None, hinst, None)
    cfont = font("Lucida Console", -12); ufont = font("MS Sans Serif", -11)
    user32.SendMessageW(S.hConsole, WM_SETFONT, cfont, 1)
    user32.SendMessageW(S.hArgs, WM_SETFONT, ufont, 1)
    shell32.DragAcceptFiles(S.hwnd, True)
    layout()

    if selftest:
        user32.DestroyWindow(S.hwnd)
        print("workbench: WS_POPUP window + child controls created/destroyed OK (GDI plumbing sound)")
        return

    if shot:
        if shot_panel == "command":
            i = find_nav("MSC 6.0a (Win95 DLLs)")
            if i is not None:
                select_nav(i)
            console_set("$ wt msc6 list\r\n\r\nmsc6-engine commands (engine/msc6) -- byte-exact MSC-6.0a NE reconstruction:\r\n\r\n"
                        "  detect_recipe\r\n  ne_normalize\r\n  skeleton\r\n  tree\r\n\r\n"
                        "(recipe: auto-detect a DLL's compiler flags; tree: file into the Win95 source tree)\r\n")
        elif shot_panel == "settings":
            i = find_nav("Settings & tools")
            if i is not None:
                select_nav(i)
        elif shot_panel and shot_panel.startswith("menu:"):
            S.open_menu = shot_panel.split(":", 1)[1]
        elif shot_panel == "analyze":
            S.report = {"name": "SHELL.DLL", "format": "NE (16-bit)"}
        cr = W.RECT(); user32.GetClientRect(S.hwnd, C.byref(cr))
        w, h = (cr.right or ww), (cr.bottom or wh)
        sdc = user32.GetDC(None); mem = gdi32.CreateCompatibleDC(sdc)
        bmi = BITMAPINFOHEADER(); bmi.biSize = C.sizeof(BITMAPINFOHEADER)
        bmi.biWidth = w; bmi.biHeight = -h; bmi.biPlanes = 1; bmi.biBitCount = 32; bmi.biCompression = 0
        ppv = C.c_void_p()
        dib = gdi32.CreateDIBSection(mem, C.byref(bmi), 0, C.byref(ppv), None, 0)
        old = gdi32.SelectObject(mem, dib)
        draw(mem, w, h)                                   # render owner-drawn content directly (reliable)
        raw = (C.c_char * (w * h * 4)).from_address(ppv.value).raw
        rgb = bytearray(w * h * 3)
        for i in range(w * h):
            rgb[i * 3] = raw[i * 4 + 2]; rgb[i * 3 + 1] = raw[i * 4 + 1]; rgb[i * 3 + 2] = raw[i * 4]
        _write_png(shot, w, h, bytes(rgb))
        gdi32.SelectObject(mem, old); gdi32.DeleteObject(dib); gdi32.DeleteDC(mem); user32.ReleaseDC(None, sdc)
        user32.DestroyWindow(S.hwnd)
        print("workbench: rendered %dx%d -> %s" % (w, h, shot))
        return

    user32.ShowWindow(S.hwnd, SW_SHOW); user32.UpdateWindow(S.hwnd)
    msg = MSG()
    while user32.GetMessageW(C.byref(msg), None, 0, 0):
        user32.TranslateMessage(C.byref(msg)); user32.DispatchMessageW(C.byref(msg))


def _which(cmd):
    import shutil
    try:
        return shutil.which(cmd)
    except Exception:
        return None


if __name__ == "__main__":
    main(sys.argv[1:])
