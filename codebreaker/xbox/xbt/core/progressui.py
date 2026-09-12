#!/usr/bin/env python3
r"""
progressui.py — a pixel-faithful Win95-style progress dialog for XboxTranspiler.

The classic Win95 "Downloading…" look is kept on purpose (it's the workbench's
signature); only the wording is themed for the Xbox 360 / PowerPC work this tool
now does — recompiling functions, not reconstructing Win95 modules.

Fully owner-drawn with GDI so it matches the classic dialog EXACTLY on any
Windows: WS_POPUP window, hand-painted 3D frame, blue gradient title bar, MS Sans
Serif text, a live percentage, a sunken trough filled with individual DICED blue
blocks (red on error), an expandable Details pane streaming each item's verdict,
and a 3D Cancel button. Runs its own message loop on a daemon thread; all update
calls are thread-safe. Also does the Win9x completion ding (MessageBeep) and the
completion sound.

    python progressui.py --selftest      # pop the dialog and step through it

Windows only; import raises elsewhere so callers fall back to stderr progress.
"""
import sys
import threading
import time

if sys.platform != "win32":
    raise ImportError("progressui requires Windows")

import ctypes as C
from ctypes import wintypes as W

user32 = C.WinDLL("user32", use_last_error=True)
kernel32 = C.WinDLL("kernel32")
gdi32 = C.WinDLL("gdi32")
shell32 = C.WinDLL("shell32")
try:
    winmm = C.WinDLL("winmm")                  # MCI: play the completion .mp3
    winmm.mciSendStringW.argtypes = [W.LPCWSTR, W.LPWSTR, W.UINT, W.HANDLE]
    winmm.mciSendStringW.restype = W.UINT
except OSError:
    winmm = None


def _asset(name):
    """Find a bundled asset (icon/sound) in _MEIPASS, next to this file, or cwd."""
    import os
    cands = []
    base = getattr(sys, "_MEIPASS", None)
    if base:
        cands.append(os.path.join(base, "assets", name))
    here = os.path.dirname(os.path.abspath(__file__))
    cands += [os.path.join(here, "assets", name), os.path.join(os.getcwd(), "assets", name)]
    for c in cands:
        if os.path.exists(c):
            return c
    return None


def _sound_path():
    return _asset("windowsding.mp3") or (
        r"C:\Users\mrtit\Downloads\windowsding.mp3"
        if __import__("os").path.exists(r"C:\Users\mrtit\Downloads\windowsding.mp3") else None)
try:
    uxtheme = C.WinDLL("uxtheme")
    uxtheme.SetWindowTheme.argtypes = [W.HWND, W.LPCWSTR, W.LPCWSTR]
    uxtheme.SetWindowTheme.restype = C.c_long
except OSError:
    uxtheme = None

LRESULT = C.c_ssize_t
WNDPROC = C.WINFUNCTYPE(LRESULT, W.HWND, W.UINT, W.WPARAM, W.LPARAM)


def RGB(r, g, b):
    return r | (g << 8) | (b << 16)


FACE = RGB(212, 208, 200)
BLOCK = RGB(0, 0, 160)             # diced progress blocks (classic blue)
BLOCK_ERR = RGB(168, 0, 0)         # red on error
WHITE = RGB(255, 255, 255)
BLACK = RGB(0, 0, 0)
CAP_TEXT = RGB(255, 255, 255)

WM_DESTROY, WM_CLOSE, WM_PAINT, WM_ERASEBKGND, WM_SETFONT = 0x2, 0x10, 0xF, 0x14, 0x30
WM_LBUTTONDOWN, WM_NCLBUTTONDOWN, WM_SETICON, WM_TIMER = 0x201, 0xA1, 0x80, 0x113
GIF_TIMER, GIF_MS = 1, 90
EM_SETSEL, EM_REPLACESEL = 0xB1, 0xC2
IMAGE_ICON, LR_LOADFROMFILE, DI_NORMAL = 1, 0x10, 0x3
ICON_SMALL, ICON_BIG = 0, 1
STATE_ICONS = {"work": "w2k_defragment.ico", "stop": "w2k_stop.ico",
               "warn": "w2k_warning.ico", "ok": "w98_check.ico"}
HTCAPTION = 2
WS_POPUP, WS_VISIBLE, WS_CHILD = 0x80000000, 0x10000000, 0x40000000
WS_VSCROLL, WS_BORDER = 0x00200000, 0x00800000
ES_MULTILINE, ES_READONLY, ES_AUTOVSCROLL = 0x4, 0x800, 0x40
SRCCOPY = 0x00CC0020
TRANSPARENT = 1
EDGE_RAISED, EDGE_SUNKEN, BF_RECT = 0x5, 0xA, 0xF
DFC_CAPTION, DFCS_CAPTIONCLOSE = 1, 0
DFC_BUTTON, DFCS_BUTTONPUSH = 4, 0x10
DT_CENTER, DT_VCENTER, DT_SINGLELINE, DT_LEFT, DT_RIGHT = 0x1, 0x4, 0x20, 0x0, 0x2
FW_NORMAL, FW_BOLD, DEFAULT_CHARSET = 400, 700, 1
IDC_ARROW, SWP_NOMOVE, SWP_NOZORDER = 32512, 0x2, 0x4
CANCEL_ID = 100
NIM_ADD, NIM_MODIFY, NIM_DELETE = 0, 1, 2
NIF_MESSAGE, NIF_ICON, NIF_TIP, NIF_INFO = 0x1, 0x2, 0x4, 0x10
IDI_APPLICATION = 32512


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


class BMIH(C.Structure):
    _fields_ = [("biSize", W.DWORD), ("biWidth", C.c_long), ("biHeight", C.c_long),
                ("biPlanes", W.WORD), ("biBitCount", W.WORD), ("biCompression", W.DWORD),
                ("biSizeImage", W.DWORD), ("biXPels", C.c_long), ("biYPels", C.c_long),
                ("biClrUsed", W.DWORD), ("biClrImportant", W.DWORD)]


class NOTIFYICONDATA(C.Structure):
    _fields_ = [("cbSize", W.DWORD), ("hWnd", W.HWND), ("uID", W.UINT),
                ("uFlags", W.UINT), ("uCallbackMessage", W.UINT), ("hIcon", W.HICON),
                ("szTip", W.WCHAR * 128), ("dwState", W.DWORD), ("dwStateMask", W.DWORD),
                ("szInfo", W.WCHAR * 256), ("uVersion", W.UINT),
                ("szInfoTitle", W.WCHAR * 64), ("dwInfoFlags", W.DWORD)]


for fn, res, args in [
    (user32.DefWindowProcW, LRESULT, [W.HWND, W.UINT, W.WPARAM, W.LPARAM]),
    (user32.CreateWindowExW, W.HWND, [W.DWORD, W.LPCWSTR, W.LPCWSTR, W.DWORD,
        C.c_int, C.c_int, C.c_int, C.c_int, W.HWND, W.HMENU, W.HINSTANCE, W.LPVOID]),
    (user32.RegisterClassW, W.ATOM, [C.POINTER(WNDCLASS)]),
    (user32.SendMessageW, LRESULT, [W.HWND, W.UINT, W.WPARAM, W.LPARAM]),
    (user32.PostMessageW, W.BOOL, [W.HWND, W.UINT, W.WPARAM, W.LPARAM]),
    (user32.GetMessageW, W.BOOL, [C.POINTER(MSG), W.HWND, W.UINT, W.UINT]),
    (user32.DestroyWindow, W.BOOL, [W.HWND]),
    (user32.LoadCursorW, W.HANDLE, [W.HINSTANCE, W.LPCWSTR]),
    (user32.LoadIconW, W.HICON, [W.HINSTANCE, W.LPCWSTR]),
    (user32.LoadImageW, W.HANDLE, [W.HINSTANCE, W.LPCWSTR, W.UINT, C.c_int, C.c_int, W.UINT]),
    (user32.DrawIconEx, W.BOOL, [W.HDC, C.c_int, C.c_int, W.HICON, C.c_int, C.c_int,
                                 W.UINT, W.HBRUSH, W.UINT]),
    (user32.GetSystemMetrics, C.c_int, [C.c_int]),
    (user32.BeginPaint, W.HDC, [W.HWND, C.POINTER(PAINTSTRUCT)]),
    (user32.EndPaint, W.BOOL, [W.HWND, C.POINTER(PAINTSTRUCT)]),
    (user32.GetClientRect, W.BOOL, [W.HWND, C.POINTER(W.RECT)]),
    (user32.FillRect, C.c_int, [W.HDC, C.POINTER(W.RECT), W.HBRUSH]),
    (user32.DrawEdge, W.BOOL, [W.HDC, C.POINTER(W.RECT), W.UINT, W.UINT]),
    (user32.DrawFrameControl, W.BOOL, [W.HDC, C.POINTER(W.RECT), W.UINT, W.UINT]),
    (user32.DrawTextW, C.c_int, [W.HDC, W.LPCWSTR, C.c_int, C.POINTER(W.RECT), W.UINT]),
    (user32.InvalidateRect, W.BOOL, [W.HWND, C.POINTER(W.RECT), W.BOOL]),
    (user32.ReleaseCapture, W.BOOL, []),
    (user32.SetWindowPos, W.BOOL, [W.HWND, W.HWND, C.c_int, C.c_int, C.c_int, C.c_int, W.UINT]),
    (user32.ShowWindow, W.BOOL, [W.HWND, C.c_int]),
    (user32.MessageBeep, W.BOOL, [W.UINT]),
    (kernel32.GetModuleHandleW, W.HMODULE, [W.LPCWSTR]),
    (gdi32.CreateSolidBrush, W.HBRUSH, [W.COLORREF]),
    (gdi32.CreateFontW, W.HFONT, [C.c_int] * 5 + [W.DWORD] * 8 + [W.LPCWSTR]),
    (gdi32.SelectObject, W.HGDIOBJ, [W.HDC, W.HGDIOBJ]),
    (gdi32.DeleteObject, W.BOOL, [W.HGDIOBJ]),
    (gdi32.SetTextColor, W.COLORREF, [W.HDC, W.COLORREF]),
    (gdi32.SetBkMode, C.c_int, [W.HDC, C.c_int]),
    (gdi32.CreateCompatibleDC, W.HDC, [W.HDC]),
    (gdi32.CreateCompatibleBitmap, W.HBITMAP, [W.HDC, C.c_int, C.c_int]),
    (gdi32.BitBlt, W.BOOL, [W.HDC, C.c_int, C.c_int, C.c_int, C.c_int, W.HDC,
                            C.c_int, C.c_int, W.DWORD]),
    (gdi32.DeleteDC, W.BOOL, [W.HDC]),
    (gdi32.StretchDIBits, C.c_int, [W.HDC, C.c_int, C.c_int, C.c_int, C.c_int,
        C.c_int, C.c_int, C.c_int, C.c_int, W.LPCVOID, C.POINTER(BMIH), W.UINT, W.DWORD]),
    (user32.SetTimer, C.c_void_p, [W.HWND, C.c_void_p, W.UINT, C.c_void_p]),
    (shell32.Shell_NotifyIconW, W.BOOL, [W.DWORD, C.POINTER(NOTIFYICONDATA)]),
]:
    fn.restype, fn.argtypes = res, args


def _rc(l, t, r, b):
    return W.RECT(l, t, r, b)


def _fmt_clock(sec):
    sec = int(max(0, sec))
    h, m, s = sec // 3600, (sec % 3600) // 60, sec % 60
    return "%d:%02d:%02d" % (h, m, s) if h else "%d:%02d" % (m, s)


class ProgressWindow:
    def __init__(self, title="XboxTranspiler", caption="Working...", two_tier=False,
                 width=420):
        self.title = title
        self._titletext = caption or title
        self._status = "Starting..."
        self._next = ""                        # next module in a batch (left-aligned)
        self._left = ""                        # modules-left tally (beside Next)
        self._bytes = ""                       # bytes reconstructed so far (right-aligned)
        self._clock_t0 = None                  # session stopwatch start (unix time)
        self.two_tier = False                 # single bar (dual bars removed)
        self.w = width
        self.h = 228                           # taller: room for the working gif + next line
        self._exp_h = self.h + 132            # expanded (details) height
        self._working = False
        self._gif = []                         # list of BGRX frame buffers
        self._gif_i = 0
        self._gif_wh = (0, 0)
        self._gif_top = 0
        self._pos, self._total = 0, 100       # overall bar
        self._sub, self._subtotal = 0, 0       # sub bar (two-tier)
        self._error = False
        self._expanded = False
        self._want_expand = False              # cross-thread request to expand Details
        self._icon_key = "work"               # defragment = recompiling
        self._icons = {}                       # key -> (small HICON, big HICON)
        self.hwnd = self._edit = None
        self._cancel = False
        self._alive = False
        self._ready = threading.Event()
        self._font = self._fontb = None
        self._nid = None
        self._wndproc = WNDPROC(self._proc)
        self._thread = threading.Thread(target=self._run, daemon=True)
        self._layout()

    def _layout(self):
        th = 22
        w, h = self.w, self.h
        # top-middle panel: working gif, then status + percentage centered,
        # all ABOVE the bar
        self._gif_top = th + 8
        y = th + 8 + 48 + 4
        self._r_status = _rc(16, y, w - 16, y + 16)
        self._r_pct = _rc(16, y + 20, w - 16, y + 38)
        # next-up module: left-aligned, directly above the bar
        self._r_next = _rc(20, y + 40, w - 20, y + 54)
        self._r_bar1 = _rc(20, y + 58, w - 20, y + 84)
        self._r_bar2 = _rc(0, 0, 0, 0)         # (dual bars removed)
        self._r_details = _rc(20, h - 40, 20 + 88, h - 40 + 26)
        self._r_cancel = _rc(w - 20 - 88, h - 40, w - 20, h - 40 + 26)
        self._r_xbtn = _rc(w - 25, 6, w - 8, 21)
        self._titleh = th

    # ---- public, thread-safe ----------------------------------------------
    def start(self, timeout=3.0):
        self._thread.start()
        self._ready.wait(timeout)
        return self.hwnd is not None

    def set_caption(self, s):
        self._titletext = str(s); self._invalidate()

    def set_status(self, s):
        self._status = str(s); self._invalidate()

    set_text = set_status

    def set_next(self, s):
        """The module that will run after the current one (batch mode)."""
        self._next = str(s or ""); self._invalidate()

    def set_left(self, s):
        """Tally of modules still to do (shown beside Next)."""
        self._left = str(s or ""); self._invalidate()

    def set_bytes(self, s):
        """Running count of code bytes recompiled (right-aligned above bar)."""
        self._bytes = str(s or ""); self._invalidate()

    def set_clock(self, t0):
        """Start (or adopt) the session stopwatch; t0 is a unix timestamp."""
        self._clock_t0 = t0; self._invalidate()

    def set_total(self, n):
        self._total = max(1, int(n)); self._invalidate()

    def step(self, pos):
        self._pos = int(pos); self._invalidate()

    def set_sub(self, pos, total):
        self._sub, self._subtotal = int(pos), int(total); self._invalidate()

    def set_error(self, flag=True):
        self._error = bool(flag)
        self.set_state("warn" if flag else "work")

    def set_working(self, flag=True):
        """Animate the working gif (only while actually recompiling)."""
        self._working = bool(flag)
        self._invalidate()

    def set_state(self, key):
        """Switch the title-bar/taskbar icon: work (defragment), warn, stop."""
        if key in STATE_ICONS:
            self._icon_key = key
            self._apply_win_icon()
            self._invalidate()

    def _apply_win_icon(self):
        if not self.hwnd:
            return
        sm, bg = self._icons.get(self._icon_key, (None, None))
        if bg:
            user32.SendMessageW(self.hwnd, WM_SETICON, ICON_BIG, bg)
        if sm:
            user32.SendMessageW(self.hwnd, WM_SETICON, ICON_SMALL, sm)

    def expand_details(self):
        """Request the Details pane to open (applied on the window thread)."""
        self._want_expand = True

    def add_detail(self, line):
        if self._edit:
            n = user32.SendMessageW(self._edit, 0xE, 0, 0)   # WM_GETTEXTLENGTH
            user32.SendMessageW(self._edit, EM_SETSEL, n, n)
            buf = C.create_unicode_buffer(str(line) + "\r\n")
            user32.SendMessageW(self._edit, EM_REPLACESEL, 0, C.addressof(buf))

    def percent(self):
        return int(100.0 * self._pos / max(1, self._total) + 0.0001)

    def beep(self):
        """Play the completion sound (windowsding.mp3 via MCI); no fallback ding."""
        path = _sound_path()
        if winmm and path:
            try:
                winmm.mciSendStringW("close wtsnd", None, 0, None)
                winmm.mciSendStringW('open "%s" type mpegvideo alias wtsnd' % path,
                                     None, 0, None)
                winmm.mciSendStringW("play wtsnd", None, 0, None)
                return
            except Exception:
                pass

    def balloon(self, title, msg):
        if not (self.hwnd and self._nid):
            return
        self._nid.uFlags = NIF_INFO
        self._nid.szInfoTitle = title[:63]
        self._nid.szInfo = msg[:255]
        self._nid.dwInfoFlags = 1               # NIIF_INFO
        try:
            shell32.Shell_NotifyIconW(NIM_MODIFY, C.byref(self._nid))
        except Exception:
            pass

    def cancelled(self):
        return self._cancel

    def alive(self):
        return self._alive and self.hwnd is not None

    def close(self):
        if self.hwnd:
            user32.PostMessageW(self.hwnd, WM_CLOSE, 0, 0)
        self._thread.join(timeout=2.0)

    def _invalidate(self):
        if self.hwnd:
            user32.InvalidateRect(self.hwnd, None, False)

    # ---- window thread -----------------------------------------------------
    def _run(self):
        try:
            self._build()
        except Exception:
            self._ready.set(); return
        self._ready.set()
        msg = MSG()
        while user32.GetMessageW(C.byref(msg), None, 0, 0) > 0:
            user32.TranslateMessage(C.byref(msg))
            user32.DispatchMessageW(C.byref(msg))

    def _build(self):
        hInst = kernel32.GetModuleHandleW(None)
        cls = WNDCLASS()
        cls.lpfnWndProc = self._wndproc
        cls.hInstance = hInst
        cls.hCursor = user32.LoadCursorW(None, C.c_wchar_p(IDC_ARROW))
        cls.hbrBackground = None
        cls.lpszClassName = "XboxTranspilerProgress95"
        user32.RegisterClassW(C.byref(cls))
        self._font = gdi32.CreateFontW(-11, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                                       DEFAULT_CHARSET, 0, 0, 0, 0, "MS Sans Serif")
        self._fontb = gdi32.CreateFontW(-11, 0, 0, 0, FW_BOLD, 0, 0, 0,
                                        DEFAULT_CHARSET, 0, 0, 0, 0, "MS Sans Serif")
        sw, sh = user32.GetSystemMetrics(0), user32.GetSystemMetrics(1)
        x, y = (sw - self.w) // 2, (sh - self.h) // 3
        for key, fn in STATE_ICONS.items():
            p = _asset(fn)
            sm = bg = None
            if p:
                sm = user32.LoadImageW(None, p, IMAGE_ICON, 16, 16, LR_LOADFROMFILE)
                bg = user32.LoadImageW(None, p, IMAGE_ICON, 48, 48, LR_LOADFROMFILE)
            self._icons[key] = (sm, bg)
        self._load_gif()
        self.hwnd = user32.CreateWindowExW(
            0, "XboxTranspilerProgress95", self.title, WS_POPUP | WS_VISIBLE,
            x, y, self.w, self.h, None, None, hInst, None)
        self._alive = True
        self._apply_win_icon()                 # taskbar/alt-tab icon
        # hidden details EDIT (shown when the Details button expands the window)
        self._edit = user32.CreateWindowExW(
            0, "EDIT", None,
            WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
            16, self.h - 4, self.w - 32, 116, self.hwnd, None, hInst, None)
        user32.SendMessageW(self._edit, WM_SETFONT, self._font, 1)
        if uxtheme:
            uxtheme.SetWindowTheme(self._edit, "", "")
        self._nid = None       # no tray icon / balloon toast (removed by request)
        # always tick: drives the gif AND the session stopwatch repaint
        user32.SetTimer(self.hwnd, GIF_TIMER, GIF_MS, None)

    def _load_gif(self):
        """Decode working.gif -> cropped, downscaled, gray-composited BGRX frames."""
        p = _asset("working.gif")
        if not p:
            return
        try:
            from PIL import Image, ImageSequence
            im = Image.open(p)
            frames = [f.convert("RGBA") for f in ImageSequence.Iterator(im)]
        except Exception:
            return
        bb = None
        for f in frames:                        # union content box across frames
            b = f.getbbox()
            if b:
                bb = b if bb is None else (min(bb[0], b[0]), min(bb[1], b[1]),
                                           max(bb[2], b[2]), max(bb[3], b[3]))
        if not bb:
            return
        face = (FACE & 0xFF, (FACE >> 8) & 0xFF, (FACE >> 16) & 0xFF)
        cw, ch = bb[2] - bb[0], bb[3] - bb[1]
        target = 46
        scale = target / max(cw, ch)
        tw, th = max(1, int(cw * scale)), max(1, int(ch * scale))
        for f in frames:
            crop = f.crop(bb).resize((tw, th))
            canvas = Image.new("RGB", (tw, th), face)
            canvas.paste(crop, (0, 0), crop)
            self._gif.append(canvas.tobytes("raw", "BGRX"))
        self._gif_wh = (tw, th)

    def _proc(self, hwnd, msg, wp, lp):
        if msg == WM_ERASEBKGND:
            return 1
        if msg == WM_TIMER:
            # honour a cross-thread request to open the Details pane (SetWindowPos
            # must run on the window's own thread — do it here, not in the caller)
            if self._want_expand and not self._expanded:
                self._want_expand = False
                self._toggle_details()
            if self._working:
                if self._gif:
                    self._gif_i = (self._gif_i + 1) % len(self._gif)
                # repaint the whole (double-buffered) dialog so the gif animates
                # AND the session stopwatch ticks up ~11x/sec
                user32.InvalidateRect(hwnd, None, False)
            return 0
        if msg == WM_PAINT:
            self._paint(hwnd); return 0
        if msg == WM_LBUTTONDOWN:
            x = C.c_short(lp & 0xFFFF).value
            y = C.c_short((lp >> 16) & 0xFFFF).value
            if self._in(self._r_xbtn, x, y):
                # ONLY the top-right X closes the window (and stops any work).
                self._cancel = True
                user32.DestroyWindow(hwnd)
            elif self._in(self._r_cancel, x, y):
                # Cancel: stop the work at once, mark FAILED (red X), stop the
                # animation — but leave the window open.
                self._cancel = True
                self.set_working(False)
                self._status = "Failed - cancelled (didn't finish)"
                self.set_state("stop")
                self._invalidate()
            elif self._in(self._r_details, x, y):
                self._toggle_details()
            elif y < self._titleh:
                user32.ReleaseCapture()
                user32.SendMessageW(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0)
            return 0
        if msg == WM_CLOSE:
            if self._nid:
                try:
                    shell32.Shell_NotifyIconW(NIM_DELETE, C.byref(self._nid))
                except Exception:
                    pass
            user32.DestroyWindow(hwnd); return 0
        if msg == WM_DESTROY:
            self._alive = False
            user32.PostQuitMessage(0); return 0
        return user32.DefWindowProcW(hwnd, msg, wp, lp)

    def _toggle_details(self):
        self._expanded = not self._expanded
        nh = self._exp_h if self._expanded else self.h
        user32.SetWindowPos(self.hwnd, None, 0, 0, self.w, nh, SWP_NOMOVE | SWP_NOZORDER)
        user32.ShowWindow(self._edit, 1 if self._expanded else 0)
        self._invalidate()

    @staticmethod
    def _in(rc, x, y):
        return rc.left <= x < rc.right and rc.top <= y < rc.bottom

    def _paint(self, hwnd):
        ps = PAINTSTRUCT()
        hdc = user32.BeginPaint(hwnd, C.byref(ps))
        rc = W.RECT(); user32.GetClientRect(hwnd, C.byref(rc))
        W_, H_ = rc.right, rc.bottom
        mem = gdi32.CreateCompatibleDC(hdc)
        bmp = gdi32.CreateCompatibleBitmap(hdc, W_, H_)
        old = gdi32.SelectObject(mem, bmp)
        try:
            self._draw(mem, W_, self.h)          # paint the fixed dialog region
            gdi32.BitBlt(hdc, 0, 0, W_, self.h, mem, 0, 0, SRCCOPY)
        finally:
            gdi32.SelectObject(mem, old)
            gdi32.DeleteObject(bmp); gdi32.DeleteDC(mem)
            user32.EndPaint(hwnd, C.byref(ps))

    def _draw_bar(self, dc, r, pos, total, white, blockbr):
        # All blocks are the SAME size. Compute how many whole blocks fit in the
        # available width, then size the trough to fit exactly that many (with
        # symmetric white margins) and CENTER it within r — instead of stretching
        # the last block. No irregular block, no trailing sliver.
        bw, gap, pad, edge = 8, 2, 2, 2
        avail = (r.right - r.left) - 2 * (edge + pad)
        n = max(1, (avail + gap) // (bw + gap))
        block_area = n * bw + (n - 1) * gap
        trough_w = block_area + 2 * (edge + pad)
        cx = (r.left + r.right) // 2
        tl = cx - trough_w // 2                       # centered trough
        tr = _rc(tl, r.top, tl + trough_w, r.bottom)
        user32.DrawEdge(dc, C.byref(tr), EDGE_SUNKEN, BF_RECT)
        user32.FillRect(dc, C.byref(_rc(tl + edge, r.top + edge,
                                        tl + trough_w - edge, r.bottom - edge)), white)
        x0 = tl + edge + pad
        y0, y1 = r.top + edge + pad, r.bottom - edge - pad
        filled = int(min(1.0, pos / max(1, total)) * n + 1e-6)
        for k in range(filled):
            bx = x0 + k * (bw + gap)
            user32.FillRect(dc, C.byref(_rc(bx, y0, bx + bw, y1)), blockbr)

    def _draw(self, dc, W_, H_):
        face = gdi32.CreateSolidBrush(FACE)
        white = gdi32.CreateSolidBrush(WHITE)
        block = gdi32.CreateSolidBrush(BLOCK_ERR if self._error else BLOCK)
        try:
            full = _rc(0, 0, W_, H_)
            user32.FillRect(dc, C.byref(full), face)
            user32.DrawEdge(dc, C.byref(full), EDGE_RAISED, BF_RECT)
            th = self._titleh
            bands = 48
            for i in range(bands):
                t = i / (bands - 1)
                col = RGB(int(10 + 156 * t), int(36 + 166 * t), int(106 + 134 * t))
                br = gdi32.CreateSolidBrush(col)
                x0 = 3 + (W_ - 6) * i // bands
                x1 = 3 + (W_ - 6) * (i + 1) // bands
                user32.FillRect(dc, C.byref(_rc(x0, 3, x1, 3 + th)), br)
                gdi32.DeleteObject(br)
            gdi32.SetBkMode(dc, TRANSPARENT)
            # state icon at the left of the title bar (defragment/warn/stop)
            sm, _bg = self._icons.get(self._icon_key, (None, None))
            tx = 8
            if sm:
                user32.DrawIconEx(dc, 6, 4, sm, 16, 16, 0, None, DI_NORMAL)
                tx = 28
            gdi32.SelectObject(dc, self._fontb)
            gdi32.SetTextColor(dc, CAP_TEXT)
            tr = _rc(tx, 3, W_ - 24, 3 + th)
            user32.DrawTextW(dc, self._titletext, -1, C.byref(tr),
                             DT_LEFT | DT_VCENTER | DT_SINGLELINE)
            user32.DrawFrameControl(dc, C.byref(self._r_xbtn), DFC_CAPTION, DFCS_CAPTIONCLOSE)

            # centered above the bar: the animated science gif WHILE working;
            # otherwise the outcome icon (ok=green / warn / stop) "filled in".
            if self._working and self._gif:
                gw, gh = self._gif_wh
                gx = (W_ - gw) // 2
                bmi = BMIH(); bmi.biSize = C.sizeof(BMIH); bmi.biWidth = gw
                bmi.biHeight = -gh; bmi.biPlanes = 1; bmi.biBitCount = 32
                buf = self._gif[self._gif_i % len(self._gif)]
                gdi32.StretchDIBits(dc, gx, self._gif_top, gw, gh, 0, 0, gw, gh,
                                    buf, C.byref(bmi), 0, SRCCOPY)
            else:
                _sm, bg = self._icons.get(self._icon_key, (None, None))
                if bg:
                    user32.DrawIconEx(dc, (W_ - 46) // 2, self._gif_top, bg,
                                      46, 46, 0, None, DI_NORMAL)

            # top-middle panel: status line (centered) + percentage (centered,
            # bold), both entirely above the progress bar
            gdi32.SelectObject(dc, self._font)
            gdi32.SetTextColor(dc, BLACK)
            user32.DrawTextW(dc, self._status, -1, C.byref(W.RECT(*self._rc4(self._r_status))),
                             DT_CENTER | DT_VCENTER | DT_SINGLELINE)
            gdi32.SelectObject(dc, self._fontb)
            user32.DrawTextW(dc, "%d%%" % self.percent(), -1,
                             C.byref(W.RECT(*self._rc4(self._r_pct))),
                             DT_CENTER | DT_VCENTER | DT_SINGLELINE)
            # session stopwatch — always counts UP, right-aligned on the pct row
            if self._clock_t0:
                gdi32.SelectObject(dc, self._font)
                gdi32.SetTextColor(dc, BLACK)
                user32.DrawTextW(dc, _fmt_clock(time.time() - self._clock_t0), -1,
                                 C.byref(W.RECT(*self._rc4(self._r_pct))),
                                 DT_RIGHT | DT_VCENTER | DT_SINGLELINE)

            # above the bar: next-up module + modules-left (left), bytes (right)
            if self._next or self._left or self._bytes:
                gdi32.SelectObject(dc, self._font)
                gdi32.SetTextColor(dc, RGB(96, 96, 96))
                nr = W.RECT(*self._rc4(self._r_next))
                left_txt = ("Next: " + self._next) if self._next else ""
                if self._left:
                    left_txt = (left_txt + "   -   " + self._left) if left_txt \
                        else self._left
                if left_txt:
                    user32.DrawTextW(dc, left_txt, -1, C.byref(nr),
                                     DT_LEFT | DT_VCENTER | DT_SINGLELINE)
                if self._bytes:
                    user32.DrawTextW(dc, self._bytes, -1, C.byref(nr),
                                     DT_RIGHT | DT_VCENTER | DT_SINGLELINE)

            self._draw_bar(dc, W.RECT(*self._rc4(self._r_bar1)), self._pos, self._total,
                           white, block)

            for r, label in ((self._r_details, "Details " + ("<<" if self._expanded else ">>")),
                             (self._r_cancel, "Cancel")):
                rr = W.RECT(*self._rc4(r))
                user32.DrawFrameControl(dc, C.byref(rr), DFC_BUTTON, DFCS_BUTTONPUSH)
                gdi32.SelectObject(dc, self._font)
                gdi32.SetTextColor(dc, BLACK)
                user32.DrawTextW(dc, label, -1, C.byref(rr),
                                 DT_CENTER | DT_VCENTER | DT_SINGLELINE)
        finally:
            gdi32.DeleteObject(face); gdi32.DeleteObject(white); gdi32.DeleteObject(block)

    @staticmethod
    def _rc4(r):
        return [r.left, r.top, r.right, r.bottom]


def demo():
    p = ProgressWindow(caption="Recompiling sub_820FBC58")
    if not p.start():
        print("could not create window"); return False
    p.set_working(True)
    p.set_total(25)
    p.set_clock(time.time())                    # showcase the session stopwatch
    p.set_next("sub_82080000")                  # showcase the next-up function
    p.set_left("8,828 functions left")          # showcase the functions-left tally
    for i in range(1, 26):
        if p.cancelled():
            break
        p.set_status("Recompiling 0x%08X  (%d / 25 instrs)" % (0x820FBC58 + i * 4, i))
        p.set_bytes("%d / 320 bytes" % (i * 4))        # showcase the byte counter
        p.step(i)
        p.add_detail("0x%08X  %s" % (0x820FBC58 + i * 4,
                     "translated" if i % 7 else "PPC_UNIMPLEMENTED"))
        time.sleep(0.12)
    p.set_working(False)
    p.set_state("ok")                          # green check fills in
    p.set_status("Done - sub_820FBC58: 24/25 instrs translated (96%)")
    p.beep()
    time.sleep(2.5)
    p.close()
    return True


if __name__ == "__main__":
    if "--selftest" in sys.argv or "--demo" in sys.argv:
        ok = demo()
        print("SELFTEST:", "PASS - progress dialog ran" if ok else "FAIL")
        sys.exit(0 if ok else 1)
    print(__doc__)
