#!/usr/bin/env python3
r"""studio/app.py -- CodeBreaker Studio: the native, pixel-faithful Win95 workbench.

Owner-drawn Win32/GDI, the SAME recipe as progressui.py: a WS_POPUP window with a
double-buffered hand-painted 3D frame (EDGE_RAISED), the classic 48-band blue
GRADIENT title bar, MS Sans Serif text, sunken property-sheet panels, comctl32
buttons (DrawFrameControl), and the DICED blue-block progress trough. No browser,
no toolkit. Drop a binary on the LEFT; the MIDDLE runs the reverse-engineering
pipeline in the diced trough, writing everything it produces into a per-file
project folder; the RIGHT holds buttons that open that folder and each artifact.
Theme swatches recolor the gradient/face (95 / XP / Vista / 7).

    wt ui
    python app.py --selftest      # create + destroy the window (plumbing check)
"""
import os
import sys
import threading

_HERE = os.path.dirname(os.path.abspath(__file__))
_ROOT = os.path.dirname(os.path.dirname(_HERE))
if _ROOT not in sys.path:
    sys.path.insert(0, _ROOT)


def main(argv=None):
    argv = argv or []
    if sys.platform != "win32":
        print("CodeBreaker Studio is a native Windows UI. On this platform use the"
              " terminal:\n  wt --target <era> <cmd>  |  wt diff  |  wt recompile")
        return
    _run("--selftest" in argv)


def _run(selftest=False):
    import ctypes as C
    from ctypes import wintypes as W
    from engine.studio import pipeline

    user32 = C.WinDLL("user32", use_last_error=True)
    gdi32 = C.WinDLL("gdi32")
    kernel32 = C.WinDLL("kernel32")
    shell32 = C.WinDLL("shell32")
    comdlg32 = C.WinDLL("comdlg32")
    LRESULT = C.c_ssize_t
    WNDPROC = C.WINFUNCTYPE(LRESULT, W.HWND, W.UINT, W.WPARAM, W.LPARAM)

    def RGB(r, g, b): return r | (g << 8) | (b << 16)
    FACE, WHITE, BLACK, GRAY = RGB(212, 208, 200), RGB(255, 255, 255), RGB(0, 0, 0), RGB(96, 96, 96)

    # theme = (name, grad_start, grad_end, caption_text, face, panel, text, dim, block, font)
    THEMES = [
        ("Windows 95", (10, 36, 106), (166, 202, 240), WHITE, FACE, WHITE, BLACK, GRAY, RGB(0, 0, 160), "MS Sans Serif"),
        ("XP Luna",    (0, 84, 227),  (60, 145, 245),  WHITE, RGB(236, 233, 216), WHITE, BLACK, RGB(90, 90, 90), RGB(20, 90, 220), "Tahoma"),
        ("Vista Aero", (60, 84, 120), (18, 26, 40),    RGB(234, 241, 248), RGB(52, 64, 82), RGB(28, 36, 50), RGB(234, 241, 248), RGB(150, 172, 196), RGB(60, 150, 240), "Segoe UI"),
        ("Windows 7",  (210, 229, 247), (255, 255, 255), RGB(20, 40, 70), RGB(238, 244, 251), WHITE, RGB(20, 30, 45), RGB(90, 105, 125), RGB(60, 130, 210), "Segoe UI"),
    ]
    # Shown before a drop; the real stage list is format-driven and comes from the pipeline.
    IDLE_STAGES = ["Reading & sniffing format", "Parsing structure",
                   "Disassembling code", "Carving functions"]

    (WM_DESTROY, WM_PAINT, WM_ERASEBKGND, WM_SIZE, WM_LBUTTONDOWN, WM_NCLBUTTONDOWN,
     WM_DROPFILES, WM_SETFONT, WM_SETTEXT, WM_APP) = (0x2, 0xF, 0x14, 0x5, 0x201, 0xA1,
     0x233, 0x30, 0xC, 0x8000)
    WS_POPUP, WS_VISIBLE, WS_CLIPCHILDREN, WS_CHILD, WS_VSCROLL = 0x80000000, 0x10000000, 0x02000000, 0x40000000, 0x00200000
    ES_MULTILINE, ES_READONLY, ES_AUTOVSCROLL, WS_BORDER = 0x4, 0x800, 0x40, 0x00800000
    DT_CENTER, DT_VCENTER, DT_SINGLELINE, DT_LEFT, DT_WORDBREAK = 0x1, 0x4, 0x20, 0x0, 0x10
    EDGE_RAISED, EDGE_SUNKEN, BF_RECT = 0x5, 0xA, 0xF
    TRANSPARENT, IDC_ARROW, SW_SHOW, HTCAPTION = 1, 32512, 5, 2
    DFC_CAPTION, DFCS_CAPTIONCLOSE, DFC_BUTTON, DFCS_BUTTONPUSH = 1, 0, 4, 0x10
    FW_NORMAL, FW_BOLD, DEFAULT_CHARSET, SRCCOPY = 400, 700, 1, 0x00CC0020
    TITLEH = 20

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
        (kernel32.GetModuleHandleW, W.HMODULE, [W.LPCWSTR]),
        (gdi32.CreateSolidBrush, W.HBRUSH, [W.COLORREF]),
        (gdi32.CreateFontW, W.HFONT, [C.c_int] * 5 + [W.DWORD] * 8 + [W.LPCWSTR]),
        (gdi32.SelectObject, W.HGDIOBJ, [W.HDC, W.HGDIOBJ]),
        (gdi32.DeleteObject, W.BOOL, [W.HGDIOBJ]),
        (gdi32.SetTextColor, W.COLORREF, [W.HDC, W.COLORREF]),
        (gdi32.SetBkMode, C.c_int, [W.HDC, C.c_int]),
        (gdi32.CreateCompatibleDC, W.HDC, [W.HDC]),
        (gdi32.CreateCompatibleBitmap, W.HBITMAP, [W.HDC, C.c_int, C.c_int]),
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
        theme = 0; stage = -1; caption = ""; report = None; running = False; stages = []
        hwnd = None; buttons = []; swatches = []; xbtn = (0, 0, 0, 0); dropzone = (0, 0, 0, 0)

    def th(): return THEMES[S.theme]

    def draw_text(dc, s, r, col, flags, fnt):
        gdi32.SelectObject(dc, fnt); gdi32.SetBkMode(dc, TRANSPARENT); gdi32.SetTextColor(dc, col)
        rr = R(*r); user32.DrawTextW(dc, s, -1, C.byref(rr), flags)

    def diced_bar(dc, r, frac, blockcol):
        """The classic sunken trough of diced blue blocks (from progressui)."""
        bw, gap, pad, edge = 8, 2, 2, 2
        rr = R(*r); user32.DrawEdge(dc, C.byref(rr), EDGE_SUNKEN, BF_RECT)
        user32.FillRect(dc, C.byref(R(r[0] + edge, r[1] + edge, r[2] - edge, r[3] - edge)), brush(WHITE))
        avail = (r[2] - r[0]) - 2 * (edge + pad)
        n = max(1, (avail + gap) // (bw + gap))
        x0, y0, y1 = r[0] + edge + pad, r[1] + edge + pad, r[3] - edge - pad
        for k in range(int(min(1.0, frac) * n + 1e-6)):
            bx = x0 + k * (bw + gap)
            user32.FillRect(dc, C.byref(R(bx, y0, bx + bw, y1)), brush(blockcol))

    def draw(dc, w, h):
        name, gs, ge, cap, face, panel, txt, dim, block, fname = th()
        fN, fB = font(fname), font(fname, bold=True)
        user32.FillRect(dc, C.byref(R(0, 0, w, h)), brush(face))
        user32.DrawEdge(dc, C.byref(R(0, 0, w, h)), EDGE_RAISED, BF_RECT)
        # gradient title bar (48 bands, progressui formula, theme endpoints)
        for i in range(48):
            t = i / 47.0
            col = RGB(int(gs[0] + (ge[0] - gs[0]) * t), int(gs[1] + (ge[1] - gs[1]) * t),
                      int(gs[2] + (ge[2] - gs[2]) * t))
            user32.FillRect(dc, C.byref(R(3 + (w - 6) * i // 48, 3, 3 + (w - 6) * (i + 1) // 48, 3 + TITLEH)), brush(col))
        draw_text(dc, "CodeBreaker Studio  —  Reverse Engineering Workbench",
                  (8, 3, w - 24, 3 + TITLEH), cap, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fB)
        S.xbtn = (w - 20, 5, w - 6, 5 + 14)
        user32.DrawFrameControl(dc, C.byref(R(*S.xbtn)), DFC_CAPTION, DFCS_CAPTIONCLOSE)
        # three sunken panels
        top, bot = 3 + TITLEH + 20, h - 26
        colw = (w - 40) // 3
        left = (10, top, 10 + colw, bot)
        mid = (20 + colw, top, 20 + 2 * colw, bot)
        right = (30 + 2 * colw, top, w - 10, bot)
        for r, title in ((left, "1  Drop a binary"), (mid, "2  Reverse engineering"),
                         (right, "3  Reconstructed output")):
            draw_text(dc, title, (r[0], r[1] - 17, r[2], r[1] - 3), dim, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fB)
            rr = R(*r); user32.DrawEdge(dc, C.byref(rr), EDGE_SUNKEN, BF_RECT)
            user32.FillRect(dc, C.byref(R(r[0] + 2, r[1] + 2, r[2] - 2, r[3] - 2)), brush(panel))
        # LEFT: drop zone (raised inner well) — drop a file OR click to browse
        dz = (left[0] + 12, left[1] + 12, left[2] - 12, left[3] - 12)
        S.dropzone = dz
        user32.DrawEdge(dc, C.byref(R(*dz)), EDGE_RAISED, BF_RECT)
        fn = S.report["name"] if S.report else "Drop your file here"
        draw_text(dc, fn, (dz[0] + 6, (dz[1] + dz[3]) // 2 - 24, dz[2] - 6, (dz[1] + dz[3]) // 2), txt, DT_CENTER | DT_WORDBREAK, fB)
        draw_text(dc, "drop it here — or click to browse\n.exe  .dll  .drv  .vxd  .386",
                  (dz[0] + 6, (dz[1] + dz[3]) // 2 + 4, dz[2] - 6, dz[3] - 8), dim, DT_CENTER | DT_WORDBREAK, fN)
        # MIDDLE: stages + diced trough (the stage list is whatever the pipeline declared)
        stages = S.stages or IDLE_STAGES
        y = mid[1] + 12
        for i, nm in enumerate(stages):
            done, active = (0 <= S.stage and i < S.stage), (i == S.stage)
            mark = "✔" if done else ("►" if active else "·")
            draw_text(dc, "%s  %s" % (mark, nm), (mid[0] + 12, y, mid[2] - 8, y + 20),
                      txt if (done or active) else dim, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fB if active else fN)
            y += 24
        frac = 0.0 if S.stage < 0 else min(1.0, (S.stage + 1) / (len(stages) + 1.0))
        diced_bar(dc, (mid[0] + 12, mid[3] - 44, mid[2] - 12, mid[3] - 26), frac, block)
        draw_text(dc, S.caption or "idle", (mid[0] + 12, mid[3] - 22, mid[2] - 12, mid[3] - 6),
                  txt, DT_CENTER | DT_SINGLELINE, fN)
        # RIGHT: buttons that open the reconstruction folder + its files
        S.buttons = []
        rx0, ry0, rx1 = right[0] + 12, right[1] + 14, right[2] - 12
        r = S.report or {}

        def pbtn(label, y, hh):
            rr = (rx0, y, rx1, y + hh)
            user32.DrawFrameControl(dc, C.byref(R(*rr)), DFC_BUTTON, DFCS_BUTTONPUSH)
            draw_text(dc, label, (rr[0] + 12, rr[1], rr[2] - 8, rr[3]), txt,
                      DT_LEFT | DT_VCENTER | DT_SINGLELINE, fB)
            return rr

        if r.get("error"):
            draw_text(dc, "Error\n\n" + str(r["error"]), (rx0, ry0, rx1, right[3] - 12),
                      txt, DT_LEFT | DT_WORDBREAK, fN)
        elif r.get("outdir"):
            draw_text(dc, "Saved to  \\%s" % os.path.basename(r["outdir"]),
                      (rx0, ry0, rx1, ry0 + 16), dim, DT_LEFT | DT_SINGLELINE, fN)
            by = ry0 + 26
            S.buttons.append((pbtn("Open output folder", by, 30), r["outdir"])); by += 40
            for label, p in r.get("artifacts", []):
                S.buttons.append((pbtn(label, by, 24), p)); by += 28
        else:
            draw_text(dc, "The tool builds a folder holding the original file and everything "
                          "it reconstructs.\n\nButtons here open that folder and each file.",
                      (rx0, ry0, rx1, right[3] - 12), dim, DT_LEFT | DT_WORDBREAK, fN)
        # status bar + theme swatches
        sb = R(3, h - 22, w - 3, h - 3); user32.DrawEdge(dc, C.byref(sb), EDGE_SUNKEN, BF_RECT)
        S.swatches = []
        sx = w - 8 - len(THEMES) * 26
        for i, tdef in enumerate(THEMES):
            box = (sx + i * 26, h - 20, sx + i * 26 + 20, h - 5)
            user32.FillRect(dc, C.byref(R(*box)), brush(RGB(*tdef[2])))
            user32.DrawEdge(dc, C.byref(R(*box)), EDGE_SUNKEN if i == S.theme else EDGE_RAISED, BF_RECT)
            S.swatches.append((box, i))
        draw_text(dc, "CodeBreaker  ·  %s theme  ·  drop a file to start" % name,
                  (10, h - 21, sx - 10, h - 4), txt, DT_LEFT | DT_VCENTER | DT_SINGLELINE, fN)

    def paint(hwnd):
        ps = PAINTSTRUCT(); hdc = user32.BeginPaint(hwnd, C.byref(ps))
        cr = W.RECT(); user32.GetClientRect(hwnd, C.byref(cr)); w, h = cr.right, cr.bottom
        mem = gdi32.CreateCompatibleDC(hdc); bmp = gdi32.CreateCompatibleBitmap(hdc, w, h)
        old = gdi32.SelectObject(mem, bmp)
        try:
            draw(mem, w, h)
            gdi32.BitBlt(hdc, 0, 0, w, h, mem, 0, 0, SRCCOPY)
        finally:
            gdi32.SelectObject(mem, old); gdi32.DeleteObject(bmp); gdi32.DeleteDC(mem)
            user32.EndPaint(hwnd, C.byref(ps))

    def open_path(p):
        """Open a folder or file in Explorer / its default app."""
        try:
            shell32.ShellExecuteW(None, "open", p, None, None, SW_SHOW)
        except Exception:
            pass

    def desktop_dir():
        home = os.path.expanduser("~")
        for d in (os.path.join(home, "Desktop"), os.path.join(home, "OneDrive", "Desktop")):
            if os.path.isdir(d):
                return d
        return home

    def worker(path):
        def prog(kind, *a):
            if kind == "stages":
                S.stages, S.stage = list(a[0]), -1
            elif kind == "stage":
                S.stage, S.caption = a[0], (a[1] if len(a) > 1 else "") + "…"
            user32.InvalidateRect(S.hwnd, None, False)
        outbase = desktop_dir()
        try:
            S.report = pipeline.reverse(path, progress=prog, outbase=outbase)
        except Exception as e:
            S.report = {"name": os.path.basename(path), "error": str(e)}
        S.running = False; user32.PostMessageW(S.hwnd, WM_APP, 0, 0)

    def on_done():
        S.stage = len(S.stages); r = S.report or {}
        S.caption = "error" if r.get("error") else ("done  ·  " + r.get("format", ""))
        user32.InvalidateRect(S.hwnd, None, False)

    def start(path):
        """Kick off the pipeline on `path` (shared by drag-drop and the file browser)."""
        if not path or S.running:
            return
        S.running = True; S.report = None; S.stages = []; S.stage = 0; S.caption = "Reading…"
        user32.InvalidateRect(S.hwnd, None, False)
        threading.Thread(target=worker, args=(path,), daemon=True).start()

    def browse():
        """Native Open-File dialog — returns a chosen path or None."""
        buf = C.create_unicode_buffer(1024)
        flt = C.create_unicode_buffer(
            "Windows binaries\x00*.exe;*.dll;*.drv;*.vxd;*.386;*.ocx;*.cpl;*.mpd;*.sys;*.scr"
            "\x00All files\x00*.*\x00")
        ofn = OPENFILENAMEW()
        ofn.lStructSize = C.sizeof(OPENFILENAMEW)
        ofn.hwndOwner = S.hwnd
        ofn.lpstrFile = C.cast(buf, W.LPWSTR); ofn.nMaxFile = 1024
        ofn.lpstrFilter = C.cast(flt, W.LPCWSTR); ofn.nFilterIndex = 1
        ofn.lpstrTitle = "Select a binary to reverse-engineer"
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER
        return buf.value if comdlg32.GetOpenFileNameW(C.byref(ofn)) else None

    def on_drop(hdrop):
        buf = C.create_unicode_buffer(1024); shell32.DragQueryFileW(hdrop, 0, buf, 1024)
        shell32.DragFinish(hdrop); start(buf.value)

    def wndproc(hwnd, msg, wp, lp):
        if msg == WM_ERASEBKGND:
            return 1
        if msg == WM_PAINT:
            paint(hwnd); return 0
        if msg == WM_SIZE:
            user32.InvalidateRect(hwnd, None, False); return 0
        if msg == WM_DROPFILES:
            on_drop(wp); return 0
        if msg == WM_APP:
            on_done(); return 0
        if msg == WM_LBUTTONDOWN:
            x, y = lp & 0xFFFF, (lp >> 16) & 0xFFFF
            bx = S.xbtn
            if bx[0] <= x <= bx[2] and bx[1] <= y <= bx[3]:
                user32.DestroyWindow(hwnd); return 0
            for (b0, t0, b1, t1), ti in S.swatches:
                if b0 <= x <= b1 and t0 <= y <= t1:
                    S.theme = ti; user32.InvalidateRect(hwnd, None, False); return 0
            for (b0, t0, b1, t1), pth in S.buttons:    # output-folder / file buttons
                if b0 <= x <= b1 and t0 <= y <= t1:
                    open_path(pth); return 0
            dz = S.dropzone                            # click the drop-well -> file browser
            if dz[0] <= x <= dz[2] and dz[1] <= y <= dz[3] and not S.running:
                start(browse()); return 0
            if y <= 3 + TITLEH:                        # drag by the title bar
                user32.ReleaseCapture(); user32.SendMessageW(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0)
            return 0
        if msg == WM_DESTROY:
            user32.PostQuitMessage(0); return 0
        return user32.DefWindowProcW(hwnd, msg, wp, lp)

    proc = WNDPROC(wndproc)
    cls = WNDCLASS(); cls.style = 0x0003; cls.lpfnWndProc = proc; cls.hInstance = hinst
    cls.hCursor = user32.LoadCursorW(None, C.c_wchar_p(IDC_ARROW)); cls.hbrBackground = None
    cls.lpszClassName = "CodeBreakerStudio95"
    user32.RegisterClassW(C.byref(cls))
    sw, sh = user32.GetSystemMetrics(0), user32.GetSystemMetrics(1)
    ww, wh = 1000, 620
    S.hwnd = user32.CreateWindowExW(0, "CodeBreakerStudio95", "CodeBreaker Studio",
        WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN, (sw - ww) // 2, (sh - wh) // 3, ww, wh,
        None, None, hinst, None)
    shell32.DragAcceptFiles(S.hwnd, True)

    if selftest:
        user32.DestroyWindow(S.hwnd)
        print("studio: WS_POPUP window created + destroyed OK (Win32/GDI plumbing sound)")
        return

    user32.ShowWindow(S.hwnd, SW_SHOW); user32.UpdateWindow(S.hwnd)
    msg = MSG()
    while user32.GetMessageW(C.byref(msg), None, 0, 0):
        user32.TranslateMessage(C.byref(msg)); user32.DispatchMessageW(C.byref(msg))


if __name__ == "__main__":
    main(sys.argv[1:])
