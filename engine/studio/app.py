#!/usr/bin/env python3
r"""studio/app.py -- CodeBreaker Studio: the native, themed, drag-and-drop workbench.

A Win32/GDI window (same approach as progressui.py -- owner-drawn, no browser, no
extra GUI toolkit). Drop a binary on the LEFT; the MIDDLE animates the reverse-
engineering pipeline; the reconstructed output lands on the RIGHT. Click a theme
swatch to reskin (Windows 95 / XP Luna / Vista / 7).

    wt ui              # launch
    python app.py --selftest   # create + destroy the window (headless plumbing check)

Windows only; on other platforms `main` explains the terminal alternatives.
"""
import os
import sys
import threading

_HERE = os.path.dirname(os.path.abspath(__file__))
_REPO = os.path.dirname(os.path.dirname(_HERE))
if _REPO not in sys.path:
    sys.path.insert(0, _REPO)


def main(argv=None):
    argv = argv or []
    if sys.platform != "win32":
        print("CodeBreaker Studio is a native Windows UI. On this platform, use the"
              " terminal:\n  wt --target <era> <cmd>   |   wt diff   |   wt recompile")
        return
    _run(selftest="--selftest" in argv)


# ------------------------------------------------------------------- Win32 core
def _run(selftest=False):
    import ctypes as C
    from ctypes import wintypes as W
    from engine.studio import pipeline

    user32 = C.WinDLL("user32", use_last_error=True)
    gdi32 = C.WinDLL("gdi32")
    kernel32 = C.WinDLL("kernel32")
    shell32 = C.WinDLL("shell32")
    LRESULT = C.c_ssize_t
    WNDPROC = C.WINFUNCTYPE(LRESULT, W.HWND, W.UINT, W.WPARAM, W.LPARAM)

    def RGB(r, g, b):
        return r | (g << 8) | (b << 16)

    # --- themes: palette per skin -------------------------------------------
    THEMES = [
        ("Windows 95", dict(face=RGB(212, 208, 200), panel=RGB(255, 255, 255),
                            text=RGB(0, 0, 0), dim=RGB(96, 96, 96),
                            t1=RGB(0, 0, 128), t2=RGB(16, 132, 208), tt=RGB(255, 255, 255),
                            accent=RGB(0, 0, 160))),
        ("XP Luna", dict(face=RGB(236, 233, 216), panel=RGB(255, 255, 255),
                         text=RGB(0, 0, 0), dim=RGB(90, 90, 90),
                         t1=RGB(0, 88, 230), t2=RGB(60, 140, 243), tt=RGB(255, 255, 255),
                         accent=RGB(10, 91, 211))),
        ("Vista Aero", dict(face=RGB(40, 52, 68), panel=RGB(24, 32, 44),
                            text=RGB(234, 241, 248), dim=RGB(159, 179, 200),
                            t1=RGB(50, 70, 100), t2=RGB(24, 34, 50), tt=RGB(234, 241, 248),
                            accent=RGB(58, 160, 255))),
        ("Windows 7", dict(face=RGB(240, 246, 252), panel=RGB(255, 255, 255),
                           text=RGB(20, 30, 45), dim=RGB(90, 105, 125),
                           t1=RGB(214, 231, 247), t2=RGB(255, 255, 255), tt=RGB(20, 40, 70),
                           accent=RGB(42, 127, 212))),
    ]

    class S:                       # tiny app state (mutable, shared with the wndproc)
        theme = 0
        stage = -1
        caption = "Drop a binary to begin."
        report = None
        running = False
        swatches = []              # (rect, theme_index) hit-test boxes
        edit = None
        hwnd = None

    STAGES = ["Sniffing the format", "Carving functions", "Disassembling",
              "Lifting to IR", "Recovering control flow", "Emitting readable C"]

    # constants
    (WM_DESTROY, WM_PAINT, WM_ERASEBKGND, WM_SIZE, WM_TIMER, WM_LBUTTONDOWN,
     WM_DROPFILES, WM_SETFONT, WM_SETTEXT, WM_APP) = (0x2, 0xF, 0x14, 0x5, 0x113,
     0x201, 0x233, 0x30, 0xC, 0x8000)
    WS_OVERLAPPEDWINDOW, WS_VISIBLE, WS_CHILD, WS_VSCROLL = 0x00CF0000, 0x10000000, 0x40000000, 0x00200000
    ES_MULTILINE, ES_READONLY, ES_AUTOVSCROLL = 0x4, 0x800, 0x40
    DT_CENTER, DT_VCENTER, DT_SINGLELINE, DT_WORDBREAK, DT_LEFT = 0x1, 0x4, 0x20, 0x10, 0x0
    EDGE_SUNKEN, EDGE_RAISED, BF_RECT = 0xA, 0x5, 0xF
    TRANSPARENT, IDC_ARROW, SW_SHOW = 1, 32512, 5
    FW_NORMAL, FW_BOLD, DEFAULT_CHARSET = 400, 700, 1

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

    for fn, res, args in [
        (user32.DefWindowProcW, LRESULT, [W.HWND, W.UINT, W.WPARAM, W.LPARAM]),
        (user32.CreateWindowExW, W.HWND, [W.DWORD, W.LPCWSTR, W.LPCWSTR, W.DWORD,
            C.c_int, C.c_int, C.c_int, C.c_int, W.HWND, W.HMENU, W.HINSTANCE, W.LPVOID]),
        (user32.RegisterClassW, W.ATOM, [C.POINTER(WNDCLASS)]),
        (user32.SendMessageW, LRESULT, [W.HWND, W.UINT, W.WPARAM, W.LPARAM]),
        (user32.PostMessageW, W.BOOL, [W.HWND, W.UINT, W.WPARAM, W.LPARAM]),
        (user32.GetMessageW, W.BOOL, [C.POINTER(MSG), W.HWND, W.UINT, W.UINT]),
        (user32.TranslateMessage, W.BOOL, [C.POINTER(MSG)]),
        (user32.DispatchMessageW, LRESULT, [C.POINTER(MSG)]),
        (user32.DestroyWindow, W.BOOL, [W.HWND]),
        (user32.PostQuitMessage, None, [C.c_int]),
        (user32.LoadCursorW, W.HANDLE, [W.HINSTANCE, W.LPCWSTR]),
        (user32.BeginPaint, W.HDC, [W.HWND, C.POINTER(PAINTSTRUCT)]),
        (user32.EndPaint, W.BOOL, [W.HWND, C.POINTER(PAINTSTRUCT)]),
        (user32.GetClientRect, W.BOOL, [W.HWND, C.POINTER(W.RECT)]),
        (user32.FillRect, C.c_int, [W.HDC, C.POINTER(W.RECT), W.HBRUSH]),
        (user32.DrawEdge, W.BOOL, [W.HDC, C.POINTER(W.RECT), W.UINT, W.UINT]),
        (user32.DrawTextW, C.c_int, [W.HDC, W.LPCWSTR, C.c_int, C.POINTER(W.RECT), W.UINT]),
        (user32.InvalidateRect, W.BOOL, [W.HWND, C.POINTER(W.RECT), W.BOOL]),
        (user32.ShowWindow, W.BOOL, [W.HWND, C.c_int]),
        (user32.UpdateWindow, W.BOOL, [W.HWND]),
        (user32.SetTimer, C.c_void_p, [W.HWND, C.c_void_p, W.UINT, C.c_void_p]),
        (user32.MoveWindow, W.BOOL, [W.HWND, C.c_int, C.c_int, C.c_int, C.c_int, W.BOOL]),
        (kernel32.GetModuleHandleW, W.HMODULE, [W.LPCWSTR]),
        (gdi32.CreateSolidBrush, W.HBRUSH, [W.COLORREF]),
        (gdi32.CreateFontW, W.HFONT, [C.c_int] * 5 + [W.DWORD] * 8 + [W.LPCWSTR]),
        (gdi32.SelectObject, W.HGDIOBJ, [W.HDC, W.HGDIOBJ]),
        (gdi32.DeleteObject, W.BOOL, [W.HGDIOBJ]),
        (gdi32.SetTextColor, W.COLORREF, [W.HDC, W.COLORREF]),
        (gdi32.SetBkMode, C.c_int, [W.HDC, C.c_int]),
        (gdi32.GradientFill if hasattr(gdi32, "GradientFill") else gdi32.CreateSolidBrush,
         W.BOOL if hasattr(gdi32, "GradientFill") else W.HBRUSH, None),
        (shell32.DragAcceptFiles, None, [W.HWND, W.BOOL]),
        (shell32.DragQueryFileW, W.UINT, [W.HANDLE, W.UINT, W.LPWSTR, W.UINT]),
        (shell32.DragFinish, None, [W.HANDLE]),
    ]:
        if args is not None:
            fn.restype, fn.argtypes = res, args

    hinst = kernel32.GetModuleHandleW(None)
    fontUI = gdi32.CreateFontW(-13, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
                               0, 0, 0, 0, "Tahoma")
    fontBig = gdi32.CreateFontW(-40, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
                                0, 0, 0, 0, "Segoe UI Emoji")
    fontMono = gdi32.CreateFontW(-13, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
                                 0, 0, 0, 0, "Consolas")
    _brushes = {}

    def brush(col):
        if col not in _brushes:
            _brushes[col] = gdi32.CreateSolidBrush(col)
        return _brushes[col]

    def rc(l, t, r, b):
        return W.RECT(l, t, r, b)

    def text(hdc, s, r, col, flags, font=fontUI):
        gdi32.SelectObject(hdc, font)
        gdi32.SetBkMode(hdc, TRANSPARENT)
        gdi32.SetTextColor(hdc, col)
        rr = rc(*r)
        user32.DrawTextW(hdc, s, -1, C.byref(rr), flags)

    def panel(hdc, r, th):
        rr = rc(*r)
        user32.FillRect(hdc, C.byref(rr), brush(th["panel"]))
        user32.DrawEdge(hdc, C.byref(rr), EDGE_SUNKEN, BF_RECT)

    def layout(hwnd):
        cr = W.RECT()
        user32.GetClientRect(hwnd, C.byref(cr))
        w, h = cr.right, cr.bottom
        top = 34                                   # title bar
        bot = h - 40                               # taskbar
        colw = (w - 48) // 3
        left = (12, top + 12, 12 + colw, bot - 12)
        mid = (24 + colw, top + 12, 24 + 2 * colw, bot - 12)
        right = (36 + 2 * colw, top + 12, w - 12, bot - 12)
        return w, h, top, bot, left, mid, right

    def place_edit(hwnd):
        w, h, top, bot, left, mid, right = layout(hwnd)
        if S.edit:
            user32.MoveWindow(S.edit, right[0] + 4, right[1] + 24,
                              right[2] - right[0] - 8, bot - right[1] - 28, True)

    def paint(hwnd):
        ps = PAINTSTRUCT()
        hdc = user32.BeginPaint(hwnd, C.byref(ps))
        th = THEMES[S.theme][1]
        w, h, top, bot, left, mid, right = layout(hwnd)
        full = rc(0, 0, w, h)
        user32.FillRect(hdc, C.byref(full), brush(th["face"]))
        # title bar (two-tone; a real gradient is a later polish)
        tb1 = rc(0, 0, w // 2, top); user32.FillRect(hdc, C.byref(tb1), brush(th["t1"]))
        tb2 = rc(w // 2, 0, w, top); user32.FillRect(hdc, C.byref(tb2), brush(th["t2"]))
        text(hdc, "  🧩  CodeBreaker Studio — Reverse Engineering Workbench",
             (10, 0, w, top), th["tt"], DT_LEFT | DT_VCENTER | DT_SINGLELINE)
        # column headers + panels
        for r, title in ((left, "1 · Drop a binary"), (mid, "2 · Reverse engineering"),
                         (right, "3 · Reconstructed output")):
            text(hdc, title, (r[0], r[1] - 20, r[2], r[1] - 2), th["dim"],
                 DT_LEFT | DT_VCENTER | DT_SINGLELINE)
            panel(hdc, r, th)
        # left: drop zone
        dz = (left[0] + 14, left[1] + 14, left[2] - 14, left[3] - 14)
        dzr = rc(*dz); user32.DrawEdge(hdc, C.byref(dzr), EDGE_RAISED, BF_RECT)
        text(hdc, "📂", (dz[0], dz[1] + 20, dz[2], dz[1] + 90), th["dim"],
             DT_CENTER | DT_SINGLELINE, fontBig)
        fn = S.report["name"] if S.report else "Drop your file here"
        text(hdc, fn, (dz[0] + 8, dz[1] + 96, dz[2] - 8, dz[1] + 150), th["text"],
             DT_CENTER | DT_WORDBREAK)
        text(hdc, ".exe .dll .drv .vxd .386 — a Windows binary",
             (dz[0] + 8, dz[3] - 34, dz[2] - 8, dz[3] - 8), th["dim"],
             DT_CENTER | DT_WORDBREAK)
        # middle: pipeline stages
        y = mid[1] + 20
        for i, name in enumerate(STAGES):
            done = 0 <= S.stage and i < S.stage
            active = i == S.stage
            mark = "✓" if done else ("»" if active else "•")
            col = th["accent"] if (active or done) else th["dim"]
            text(hdc, "%s  %s" % (mark, name), (mid[0] + 14, y, mid[2] - 10, y + 22),
                 col, DT_LEFT | DT_VCENTER | DT_SINGLELINE)
            y += 26
        # progress trough
        tr = (mid[0] + 14, mid[3] - 40, mid[2] - 14, mid[3] - 24)
        trr = rc(*tr); user32.FillRect(hdc, C.byref(trr), brush(th["panel"]))
        user32.DrawEdge(hdc, C.byref(trr), EDGE_SUNKEN, BF_RECT)
        frac = 0 if S.stage < 0 else min(1.0, (S.stage + 1) / (len(STAGES) + 1))
        fill = rc(tr[0] + 2, tr[1] + 2, tr[0] + 2 + int((tr[2] - tr[0] - 4) * frac), tr[3] - 2)
        user32.FillRect(hdc, C.byref(fill), brush(th["accent"]))
        text(hdc, S.caption, (mid[0] + 14, mid[3] - 20, mid[2] - 14, mid[3]),
             th["text"], DT_CENTER | DT_SINGLELINE)
        # taskbar + theme swatches
        tbar = rc(0, bot, w, h); user32.FillRect(hdc, C.byref(tbar), brush(th["face"]))
        S.swatches = []
        sx = w - 4 - len(THEMES) * 30
        for i, (nm, t) in enumerate(THEMES):
            box = (sx + i * 30, bot + 8, sx + i * 30 + 24, bot + 30)
            br = rc(*box); user32.FillRect(hdc, C.byref(br), brush(t["t2"]))
            user32.DrawEdge(hdc, C.byref(br),
                            EDGE_RAISED if i != S.theme else EDGE_SUNKEN, BF_RECT)
            S.swatches.append((box, i))
        text(hdc, "🧩 CodeBreaker  ·  %s theme  ·  drop a file to start" % THEMES[S.theme][0],
             (10, bot + 8, sx - 10, h - 8), th["text"], DT_LEFT | DT_VCENTER | DT_SINGLELINE)
        user32.EndPaint(hwnd, C.byref(ps))

    def set_output(s):
        buf = C.create_unicode_buffer(s)
        user32.SendMessageW(S.edit, WM_SETTEXT, 0, C.addressof(buf))

    def worker(path):
        def prog(i, cap):
            S.stage = i
            S.caption = cap + "…"
            user32.InvalidateRect(S.hwnd, None, False)
        try:
            S.report = pipeline.reverse(path, progress=prog)
        except Exception as e:
            S.report = {"name": os.path.basename(path), "error": str(e)}
        S.running = False
        user32.PostMessageW(S.hwnd, WM_APP, 0, 0)

    def on_done():
        S.stage = len(STAGES)
        r = S.report or {}
        if r.get("error"):
            S.caption = "error"
            set_output("ERROR: " + r["error"])
        else:
            S.caption = "✓ " + r.get("format", "done")
            lines = ["%s   %s   %s bytes" % (r.get("name"), r.get("format"),
                                             format(r.get("size", 0), ",")),
                     "  " + "  ·  ".join(r.get("notes", [])), ""]
            if r.get("structure"):
                lines.append("STRUCTURE")
                lines += ["  %-8s %-12s %s" % (s["k"], s["n"], s["d"]) for s in r["structure"]]
                lines.append("")
            if r.get("exports"):
                lines.append("EXPORTS (%d)" % len(r["exports"]))
                lines.append("  " + ", ".join(e["name"] for e in r["exports"][:40]))
                lines.append("")
            lines.append("DISASSEMBLY")
            lines.append(r.get("disasm", ""))
            set_output("\r\n".join(lines))
        user32.InvalidateRect(S.hwnd, None, False)

    def on_drop(hdrop):
        buf = C.create_unicode_buffer(1024)
        shell32.DragQueryFileW(hdrop, 0, buf, 1024)
        shell32.DragFinish(hdrop)
        path = buf.value
        if not path or S.running:
            return
        S.running = True
        S.report = None
        S.stage = 0
        S.caption = "Sniffing the format…"
        set_output("")
        user32.InvalidateRect(S.hwnd, None, False)
        threading.Thread(target=worker, args=(path,), daemon=True).start()

    def wndproc(hwnd, msg, wp, lp):
        if msg == WM_ERASEBKGND:
            return 1
        if msg == WM_PAINT:
            paint(hwnd); return 0
        if msg == WM_SIZE:
            place_edit(hwnd); user32.InvalidateRect(hwnd, None, False); return 0
        if msg == WM_DROPFILES:
            on_drop(wp); return 0
        if msg == WM_APP:
            on_done(); return 0
        if msg == WM_LBUTTONDOWN:
            x, y = lp & 0xFFFF, (lp >> 16) & 0xFFFF
            for (bx0, by0, bx1, by1), ti in S.swatches:
                if bx0 <= x <= bx1 and by0 <= y <= by1:
                    S.theme = ti; user32.InvalidateRect(hwnd, None, False); return 0
            return 0
        if msg == WM_DESTROY:
            user32.PostQuitMessage(0); return 0
        return user32.DefWindowProcW(hwnd, msg, wp, lp)

    proc = WNDPROC(wndproc)
    cls = WNDCLASS()
    cls.style = 0x0003                          # CS_HREDRAW | CS_VREDRAW
    cls.lpfnWndProc = proc
    cls.hInstance = hinst
    cls.hCursor = user32.LoadCursorW(None, C.c_wchar_p(IDC_ARROW))
    cls.hbrBackground = 0
    cls.lpszClassName = "CodeBreakerStudio"
    user32.RegisterClassW(C.byref(cls))

    S.hwnd = user32.CreateWindowExW(
        0, "CodeBreakerStudio", "CodeBreaker Studio", WS_OVERLAPPEDWINDOW,
        120, 80, 1100, 680, None, None, hinst, None)
    S.edit = user32.CreateWindowExW(
        0, "EDIT", None,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
        0, 0, 10, 10, S.hwnd, None, hinst, None)
    user32.SendMessageW(S.edit, WM_SETFONT, fontMono, 1)
    shell32.DragAcceptFiles(S.hwnd, True)
    place_edit(S.hwnd)
    set_output("Drop a Windows binary on the left to reverse-engineer it.\r\n\r\n"
               "The engine detects the format (MZ/NE/PE/LE), parses its structure,\r\n"
               "disassembles the code, and lifts toward readable C.")

    if selftest:
        user32.DestroyWindow(S.hwnd)
        print("studio: window created + destroyed OK (Win32 plumbing sound)")
        return

    user32.ShowWindow(S.hwnd, SW_SHOW)
    user32.UpdateWindow(S.hwnd)
    msg = MSG()
    while user32.GetMessageW(C.byref(msg), None, 0, 0):
        user32.TranslateMessage(C.byref(msg))
        user32.DispatchMessageW(C.byref(msg))


if __name__ == "__main__":
    main(sys.argv[1:])
