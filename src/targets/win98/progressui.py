#!/usr/bin/env python3
r"""
progressui.py — a pixel-faithful Win98 TABBED application dialog for CodeBreaker.

Owner-drawn with GDI so it matches the classic look EXACTLY on any Windows: a
WS_POPUP window with a hand-painted 3D frame, blue gradient title bar, MS Sans
Serif text, a menu bar, and a row of property-sheet TABS (like "Find: All
Files") that switch the panel below:

  * Progress  — the live reconstruction: working gif, status, a sunken trough of
                DICED blue blocks (red on error), %, session stopwatch, next-up
                module + modules-left + byte counter, and a per-segment verdict list.
  * Report    — the full analysis report for the current/last module.
  * Log       — the complete streaming console output (scrollback).
  * Coverage  — per-tier byte-exact / live totals for the whole corpus.

All report/log text is streamed IN HERE (progress.capture_output) instead of a
console — the app is a windowed build with no console window. Runs its own
message loop on a daemon thread; all update calls are thread-safe.

    python progressui.py --demo      # pop the dialog and step through it

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
        r""
        if __import__("os").path.exists(r"") else None)
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
GRAYTX = RGB(96, 96, 96)
CAP_TEXT = RGB(255, 255, 255)

WM_DESTROY, WM_CLOSE, WM_PAINT, WM_ERASEBKGND, WM_SETFONT = 0x2, 0x10, 0xF, 0x14, 0x30
WM_LBUTTONDOWN, WM_NCLBUTTONDOWN, WM_SETICON, WM_TIMER = 0x201, 0xA1, 0x80, 0x113
WM_SETTEXT = 0xC
GIF_TIMER, GIF_MS = 1, 90
EM_SETSEL, EM_REPLACESEL = 0xB1, 0xC2
IMAGE_ICON, LR_LOADFROMFILE, DI_NORMAL = 1, 0x10, 0x3
ICON_SMALL, ICON_BIG = 0, 1
STATE_ICONS = {"work": "w2k_defragment.ico", "stop": "w2k_stop.ico",
               "warn": "w2k_warning.ico", "ok": "w98_check.ico"}
HTCAPTION = 2
WS_POPUP, WS_VISIBLE, WS_CHILD = 0x80000000, 0x10000000, 0x40000000
WS_CLIPCHILDREN = 0x02000000       # parent paint never overdraws child controls (no strobe)
WS_VSCROLL, WS_HSCROLL, WS_BORDER = 0x00200000, 0x00100000, 0x00800000
ES_MULTILINE, ES_READONLY, ES_AUTOVSCROLL = 0x4, 0x800, 0x40
SW_HIDE, SW_SHOW = 0, 5
SRCCOPY = 0x00CC0020
TRANSPARENT = 1
EDGE_RAISED, EDGE_SUNKEN, BF_RECT = 0x5, 0xA, 0xF
BF_TOP, BF_LEFT, BF_RIGHT = 0x2, 0x1, 0x4
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


# --- extra native controls for the merged Tools + Explorer tabs -------------
WS_TABSTOP, ES_AUTOHSCROLL = 0x00010000, 0x80
LBS_NOTIFY, LBS_NOINTEGRALHEIGHT = 0x1, 0x100
CBS_DROPDOWNLIST, CBS_HASSTRINGS = 0x3, 0x200
BS_DEFPUSHBUTTON = 0x1
CB_ADDSTRING, CB_RESETCONTENT, CB_GETCURSEL, CB_SETCURSEL = 0x143, 0x14B, 0x147, 0x14E
LB_ADDSTRING, LB_RESETCONTENT, LB_GETCURSEL, LB_SETCURSEL = 0x180, 0x184, 0x188, 0x186
LB_GETTEXT, LB_GETTEXTLEN = 0x189, 0x18A
WM_COMMAND, WM_GETTEXT, WM_GETTEXTLENGTH, WM_SIZE = 0x111, 0xD, 0xE, 0x5
CBN_SELCHANGE, LBN_SELCHANGE, LBN_DBLCLK, BN_CLICKED = 1, 1, 2, 0
IDC_COMBO, IDC_FUNCS, IDC_CALLEES, IDC_CALLERS, IDC_TOOLS, IDC_ARGS, IDC_RUN = \
    1001, 1002, 1003, 1004, 1005, 1006, 1007
EXPLORER_TABS = {"Code", "Strings", "Memory", "Tables", "CallGraph", "Entry", "Export"}


class ProgressWindow:
    def __init__(self, title="CodeBreaker", caption="Working...", two_tier=False,
                 width=920):
        self.title = title
        self._titletext = caption or title
        self._status = "Starting..."
        self._next = ""                        # next module in a batch (left-aligned)
        self._left = ""                        # modules-left tally (beside Next)
        self._bytes = ""                       # bytes reconstructed so far (right-aligned)
        self._sbar = ""                        # status-bar text (tier progress)
        self._clock_t0 = None                  # session stopwatch start (unix time)
        self.two_tier = False
        self.w = width
        # chrome bands + a tall panel: title / tabs / panel / buttons / status.
        # (No menu bar — File/Edit/… did nothing, so they're gone.)
        self._titleh = 22
        self._menu_h = 0
        self._tab_h = 23
        self._btn_h = 32
        self._status_h = 20
        self.h = 600
        # ONE window: explorer facets first, then the Tools launcher, then the run
        # monitor (Progress/Log/Coverage) — tab order chosen by the user.
        self._tabs = ["Code", "Strings", "Memory", "Tables", "CallGraph", "Entry",
                      "Export", "Tools", "Progress", "Log", "Coverage"]
        self._tab = 0                          # active tab (Code)
        self._logn = 0                         # Log line count (trimming)
        self._working = False
        self._gif = []                         # list of BGRX frame buffers
        self._gif_i = 0
        self._gif_wh = (0, 0)
        self._gif_top = 0
        self._pos, self._total = 0, 100       # overall bar
        self._sub, self._subtotal = 0, 0
        self._error = False
        self._icon_key = "work"               # defragment = reconstructing
        self._icons = {}                       # key -> (small HICON, big HICON)
        self.hwnd = None
        self._detail = self._log = self._cov = None      # Progress-verdicts / Log / Coverage
        self._combo = self._funcs = self._expcode = None  # Explorer controls
        self._callees = self._callers = None
        self._tools_lb = self._args = self._runbtn = None  # Tools launcher controls
        # NB: the Run button is _runbtn, NOT _run — _run is the window-thread method;
        # naming the control self._run shadowed it and gave the thread target=None.
        self._fontm = None                                # monospace (code view)
        self._modnames = []
        self._modules_loaded = False
        self._model = None
        self._funcrows = []
        self._toolrows = []
        self._start_module = None                         # set by open on a module
        self._cancel = False
        self._alive = False
        self._ready = threading.Event()
        self._font = self._fontb = None
        self._nid = None
        self._wndproc = WNDPROC(self._proc)
        self._thread = threading.Thread(target=self._run, daemon=True)
        self._layout()

    def _layout(self):
        w, h = self.w, self.h
        th, mh, tabh = self._titleh, self._menu_h, self._tab_h
        # tab strip
        self._tab_top = th + mh
        self._tab_rects = []
        tx = 8
        for name in self._tabs:
            tw = 30 + 7 * len(name)
            self._tab_rects.append(_rc(tx, self._tab_top, tx + tw, self._tab_top + tabh))
            tx += tw
        # panel (sunken content frame) below the tabs, above the button + status rows
        self._panel_top = th + mh + tabh
        self._panel_bot = h - self._status_h - self._btn_h
        self._r_panel = _rc(8, self._panel_top, w - 8, self._panel_bot)
        # PROGRESS tab: dashboard on top of the panel, verdict list filling the rest
        y = self._panel_top + 6
        self._gif_top = y
        y2 = y + 48 + 4
        self._r_status = _rc(16, y2, w - 16, y2 + 16)
        self._r_pct = _rc(16, y2 + 20, w - 16, y2 + 38)
        self._r_next = _rc(20, y2 + 40, w - 20, y2 + 54)
        self._r_bar1 = _rc(20, y2 + 58, w - 20, y2 + 84)
        self._detail_top = y2 + 90
        # per-tab EDIT geometry
        self._edit_full = (12, self._panel_top + 6, w - 24, self._panel_bot - self._panel_top - 12)
        self._edit_detail = (12, self._detail_top, w - 24, self._panel_bot - self._detail_top - 6)
        # button row (Cancel) + status bar
        self._r_cancel = _rc(w - 8 - 90, h - self._status_h - self._btn_h + 4,
                             w - 12, h - self._status_h - self._btn_h + 4 + 24)
        self._r_status_bar = _rc(6, h - self._status_h - 2, w - 6, h - 4)
        self._r_xbtn = _rc(w - 25, 6, w - 8, 21)
        # Explorer three-pane + Tools controls (positioned; shown per active tab)
        pt, pb, m2 = self._panel_top, self._panel_bot, 8
        if self._combo:
            exp_top = pt + 26
            left_w, right_w = 250, 220
            cx2 = w - right_w - m2
            user32.SetWindowPos(self._combo, None, 12, pt + 2, 340, 300, SWP_NOZORDER)
            user32.SetWindowPos(self._funcs, None, m2, exp_top, left_w, pb - exp_top, SWP_NOZORDER)
            user32.SetWindowPos(self._expcode, None, m2 + left_w + m2, exp_top,
                                cx2 - (m2 + left_w + m2) - m2, pb - exp_top, SWP_NOZORDER)
            ch = (pb - exp_top - m2) // 2
            user32.SetWindowPos(self._callees, None, cx2, exp_top, right_w, ch, SWP_NOZORDER)
            user32.SetWindowPos(self._callers, None, cx2, exp_top + ch + m2, right_w, ch, SWP_NOZORDER)
        if self._tools_lb:
            aby = pb - 30
            user32.SetWindowPos(self._tools_lb, None, m2, pt + 4, w - 2 * m2, aby - pt - 8, SWP_NOZORDER)
            user32.SetWindowPos(self._args, None, m2 + 44, aby, w - 2 * m2 - 150, 22, SWP_NOZORDER)
            user32.SetWindowPos(self._runbtn, None, w - m2 - 96, aby - 2, 96, 26, SWP_NOZORDER)

    # ---- public, thread-safe ----------------------------------------------
    def start(self, timeout=3.0):
        self._thread.start()
        self._ready.wait(timeout)
        return self.hwnd is not None

    def run_blocking(self, start_module=None):
        """Open the window standalone (double-click / `explore`) and pump messages
        on THIS thread until it closes. Same window the conductor drives."""
        self._start_module = start_module
        if start_module:
            self._tab = self._tabs.index("Code")
        self._build()
        if start_module or self._tabs[self._tab] in EXPLORER_TABS:
            threading.Thread(target=self._load_modules, daemon=True).start()
        msg = MSG()
        while user32.GetMessageW(C.byref(msg), None, 0, 0) > 0:
            user32.TranslateMessage(C.byref(msg))
            user32.DispatchMessageW(C.byref(msg))

    def set_caption(self, s):
        self._titletext = str(s); self._invalidate()

    def set_status(self, s):
        self._status = str(s); self._invalidate()

    set_text = set_status

    def set_next(self, s):
        self._next = str(s or ""); self._invalidate()

    def set_left(self, s):
        self._left = str(s or ""); self._invalidate()

    def set_bytes(self, s):
        self._bytes = str(s or ""); self._invalidate()

    def set_status_bar(self, s):
        self._sbar = str(s or ""); self._invalidate()

    def set_clock(self, t0):
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
        self._working = bool(flag)
        if flag and self.hwnd and self._tabs[self._tab] != "Progress":
            self._tab = self._tabs.index("Progress")   # a run started: front the monitor
            self._apply_tab_vis()
        self._invalidate()

    def set_state(self, key):
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

    # ---- tab switching + per-tab content ----------------------------------
    def set_tab(self, i):
        self._tab = max(0, min(len(self._tabs) - 1, int(i)))
        self._apply_tab_vis()
        self._invalidate()

    def _apply_tab_vis(self):
        """Show only the controls belonging to the active tab. Progress also draws
        the owner-drawn dashboard (in _draw); its EDIT is the verdict list."""
        name = self._tabs[self._tab]
        expl = [self._combo, self._funcs, self._expcode, self._callees, self._callers]
        show = {"Progress": [self._detail], "Log": [self._log], "Coverage": [self._cov],
                "Tools": [self._tools_lb, self._args, self._runbtn]}.get(name, [])
        if name in EXPLORER_TABS:
            show = expl
        allc = [self._detail, self._log, self._cov, self._tools_lb, self._args, self._runbtn] + expl
        for c in allc:
            if c:
                user32.ShowWindow(c, SW_SHOW if c in show else SW_HIDE)

    def _place_edit(self, edit, rect_full):
        x, y, w, h = rect_full
        user32.SetWindowPos(edit, None, x, y, w, h, SWP_NOZORDER)

    def _append(self, edit, line, cap_attr=None):
        if not edit:
            return
        if cap_attr is not None:
            n = getattr(self, cap_attr, 0) + 1
            if n > 1400:
                user32.SendMessageW(edit, EM_SETSEL, 0, -1)
                t = C.create_unicode_buffer("[ ...earlier output trimmed... ]\r\n")
                user32.SendMessageW(edit, EM_REPLACESEL, 0, C.addressof(t))
                n = 1
            setattr(self, cap_attr, n)
        ln = user32.SendMessageW(edit, 0xE, 0, 0)          # WM_GETTEXTLENGTH
        user32.SendMessageW(edit, EM_SETSEL, ln, ln)
        buf = C.create_unicode_buffer(str(line).replace("\t", "    ") + "\r\n")
        user32.SendMessageW(edit, EM_REPLACESEL, 0, C.addressof(buf))

    def _settext(self, edit, text):
        if edit:
            buf = C.create_unicode_buffer(str(text).replace("\r\n", "\n").replace("\n", "\r\n"))
            user32.SendMessageW(edit, WM_SETTEXT, 0, C.addressof(buf))

    # ---- Tools launcher (merged tab) --------------------------------------
    def _tool_registry(self):
        try:
            from src.core import config
            reg = [(n, d) for n, _s, d in win98re.BUILTINS if n != "explore"]
            try:
                reg += [(n, d) for n, _p, d in config.load_plugins()]
            except Exception:
                pass
            return reg
        except Exception:
            return [("conduct", "drive a whole module to byte-exact (the conductor)"),
                    ("ledger", "coverage ledger"), ("verify", "check reconstructions"),
                    ("doctor", "dependency check"), ("target", "module inventory + tier map")]

    def _fill_tools(self):
        self._toolrows = self._tool_registry()
        for name, desc in self._toolrows:
            b = C.create_unicode_buffer("%-12s  %s" % (name, desc))
            user32.SendMessageW(self._tools_lb, LB_ADDSTRING, 0, C.addressof(b))

    def _launch_tool(self):
        import os as _os
        i = user32.SendMessageW(self._tools_lb, LB_GETCURSEL, 0, 0)
        if not (0 <= i < len(self._toolrows)):
            self.set_status_bar("select a tool first"); return
        name = self._toolrows[i][0]
        n = user32.SendMessageW(self._args, WM_GETTEXTLENGTH, 0, 0)
        buf = C.create_unicode_buffer(n + 1)
        user32.SendMessageW(self._args, WM_GETTEXT, n + 1, C.addressof(buf))
        args = buf.value.split()
        import subprocess
        try:
            if getattr(sys, "frozen", False):
                cmd = [sys.executable, name] + args
            else:
                cmd = [sys.executable, _os.path.join(_os.path.dirname(_os.path.abspath(__file__)),
                       "win98re.py"), name] + args
            subprocess.Popen(cmd, cwd=_os.getcwd())
            self.set_status_bar("launched: %s %s" % (name, " ".join(args)))
        except Exception as e:
            self.set_status_bar("launch failed: %r" % e)

    # ---- Explorer (merged tabs) -------------------------------------------
    def _load_modules(self):
        if self._modules_loaded:
            return
        self._modules_loaded = True
        import os as _os
        try:
            import target
            names = [_os.path.basename(p) for tier in target.TIER_ORDER
                     for p in target.tiers().get(tier, [])]
        except Exception as e:
            self.set_status_bar("module list error: %r" % e); return
        self._modnames = names
        for n in names:
            b = C.create_unicode_buffer(n)
            user32.SendMessageW(self._combo, CB_ADDSTRING, 0, C.addressof(b))
        pick = 0
        if self._start_module:
            low = [x.lower() for x in names]
            if self._start_module.lower() in low:
                pick = low.index(self._start_module.lower())
        if names:
            user32.SendMessageW(self._combo, CB_SETCURSEL, pick, 0)
            self._load_module(names[pick])

    def _load_module(self, name):
        self.set_status_bar("analyzing %s ..." % name)
        try:
            from src.ir import explore_model
            self._model = explore_model.analyze_module(name)
        except Exception as e:
            self.set_status_bar("analyze error: %r" % e); return
        user32.SendMessageW(self._funcs, LB_RESETCONTENT, 0, 0)
        self._funcrows = list(self._model.get("functions", []))
        mark = {"exact": " ", "partial": "~", "error": "!", "not-reconstructed": "?"}
        for f in self._funcrows:
            b = C.create_unicode_buffer("%s %-28s %8X  %d/%d" % (mark.get(f["status"], "?"),
                f["name"][:28], f["addr"], f["live"], f["total"]))
            user32.SendMessageW(self._funcs, LB_ADDSTRING, 0, C.addressof(b))
        m = self._model
        self.set_status_bar("%s [%s]: %d funcs, %d exact, %d strings, entry 0x%X"
                            % (m["name"], m["format"], m["n_functions"], m["n_exact"],
                               m["n_strings"], m["entry"]["addr"]))
        if self._funcrows:
            user32.SendMessageW(self._funcs, LB_SETCURSEL, 0, 0)
            self._select_func(0)

    def _select_func(self, i):
        if not (0 <= i < len(self._funcrows)):
            return
        f = self._funcrows[i]
        self._settext(self._expcode, self._asm_text(f) or
                      "; no reconstructed .asm on disk for this function")
        self._fill_xrefs(f)

    def _asm_text(self, f):
        import os as _os
        d = _os.path.join("reconstructed", "asm", self._model["name"].lower())
        if not _os.path.isdir(d):
            return None
        want = f.get("off", f["addr"])
        for fn in _os.listdir(d):
            if fn.endswith(".asm") and "_" in fn:
                try:
                    off = int(fn.rsplit("_", 1)[1][:-4], 16)
                except ValueError:
                    continue
                if off == want:
                    return open(_os.path.join(d, fn), encoding="latin1").read()
        return None

    def _fill_xrefs(self, f):
        m = self._model
        cg = (m.get("callgraph") or {}).get(f["addr"]) or {}
        callees = cg.get("callees", f.get("callees", []))
        callers = cg.get("callers", [])
        name_by = {x["addr"]: x["name"] for x in m.get("functions", [])}
        user32.SendMessageW(self._callees, LB_RESETCONTENT, 0, 0)
        for c in callees:
            b = C.create_unicode_buffer(str(c)); user32.SendMessageW(self._callees, LB_ADDSTRING, 0, C.addressof(b))
        user32.SendMessageW(self._callers, LB_RESETCONTENT, 0, 0)
        for a in callers:
            b = C.create_unicode_buffer(name_by.get(a, hex(a)))
            user32.SendMessageW(self._callers, LB_ADDSTRING, 0, C.addressof(b))

    def _lbtext(self, lb, i):
        n = user32.SendMessageW(lb, LB_GETTEXTLEN, i, 0)
        buf = C.create_unicode_buffer(n + 1)
        user32.SendMessageW(lb, LB_GETTEXT, i, C.addressof(buf))
        return buf.value

    def _jump_xref(self, lb):
        i = user32.SendMessageW(lb, LB_GETCURSEL, 0, 0)
        if i < 0 or not self._model:
            return
        text = self._lbtext(lb, i)
        for j, f in enumerate(self._funcrows):
            if f["name"] == text or hex(f["addr"]) == text:
                user32.SendMessageW(self._funcs, LB_SETCURSEL, j, 0)
                self._tab = self._tabs.index("Code"); self._apply_tab_vis()
                self._select_func(j); self._invalidate(); return

    def _render_explorer_tab(self):
        m = self._model
        name = self._tabs[self._tab]
        if not m:
            self._settext(self._expcode, "pick a module from the drop-down"); return
        if name == "Code":
            i = user32.SendMessageW(self._funcs, LB_GETCURSEL, 0, 0)
            self._select_func(i if i >= 0 else 0)
        elif name == "Strings":
            self._settext(self._expcode, "STRINGS (%d)\n%s\n" % (m["n_strings"], "-" * 60) +
                "\n".join("0x%08X  %-5s  %s" % (s["addr"], s["enc"], s["text"][:120])
                          for s in m.get("strings", [])[:4000]))
        elif name == "Memory":
            rows = ["SECTIONS", "-" * 60, "%-10s %-11s %-9s %-9s %-6s funcs" % ("name", "address", "vsize", "raw", "kind")]
            for s in m["sections"]:
                nf = sum(1 for f in m.get("functions", []) if f.get("section") == s["name"])
                rows.append("%-10s 0x%08X  %-9d %-9d %-6s %d" % (s["name"], s["addr"], s["vsize"], s["rawsize"], s["kind"], nf))
            self._settext(self._expcode, "\n".join(rows))
        elif name == "Tables":
            rows = ["IMPORTS (%d)" % len(m["imports"]), "-" * 60]
            rows += ["%-14s %s" % (i.get("dll") or "", i.get("name") or "") for i in m["imports"][:2000]]
            rows += ["", "EXPORTS (%d)" % len(m["exports"]), "-" * 60]
            rows += ["@%-6s %-28s %s" % (e.get("ordinal"), e.get("name") or "",
                     ("0x%X" % e["addr"]) if e.get("addr") is not None else "") for e in m["exports"]]
            rows += ["", "RELOCATIONS: %d (%s)" % (m["relocations"]["count"], m["relocations"]["kind"])]
            self._settext(self._expcode, "\n".join(rows))
        elif name == "CallGraph":
            i = user32.SendMessageW(self._funcs, LB_GETCURSEL, 0, 0)
            f = self._funcrows[i] if 0 <= i < len(self._funcrows) else None
            if not f:
                self._settext(self._expcode, "select a function"); return
            cg = (m.get("callgraph") or {}).get(f["addr"]) or {"callees": f.get("callees", []), "callers": []}
            name_by = {x["addr"]: x["name"] for x in m["functions"]}
            rows = ["%s  0x%X" % (f["name"], f["addr"]), "", "calls (%d):" % len(cg["callees"])]
            rows += ["  -> " + str(c) for c in cg["callees"]]
            rows += ["", "called by (%d):" % len(cg.get("callers", []))]
            rows += ["  <- " + name_by.get(a, hex(a)) for a in cg.get("callers", [])]
            self._settext(self._expcode, "\n".join(rows))
        elif name == "Entry":
            ef = next((f for f in m.get("functions", []) if f["addr"] == m["entry"]["addr"]), None)
            self._settext(self._expcode, "entry point: 0x%X (%s)%s" % (m["entry"]["addr"], m["entry"]["kind"],
                          ("\n\nfunction: %s (%s)" % (ef["name"], ef["status"]) if ef else "\n\n(no carved function at entry)")))
        elif name == "Export":
            import os as _os, json as _json
            outdir = _os.path.join("reports", "explore"); _os.makedirs(outdir, exist_ok=True)
            p = _os.path.join(outdir, m["name"].lower() + ".model.json")
            try:
                _json.dump(m, open(p, "w"), indent=1, default=str)
                self._settext(self._expcode, "Exported the analysis model to:\n\n  %s" % _os.path.abspath(p))
            except Exception as e:
                self._settext(self._expcode, "export failed: %r" % e)

    def add_detail(self, line):
        """A per-segment verdict line — shown in the Progress tab's list."""
        self._append(self._detail, line)

    def add_log(self, line):
        """A raw console/report line — appended to the Log tab (scrollback)."""
        self._append(self._log, line, "_logn")

    def set_report(self, text):
        """No-op: the Report tab was removed (the Log tab already carries the full
        report via the stdout tee). Kept so callers don't need to change."""
        return

    def set_coverage(self, text):
        """Replace the Coverage tab with the whole-corpus byte-exact summary."""
        self._settext(self._cov, text)

    def percent(self):
        return int(100.0 * self._pos / max(1, self._total) + 0.0001)

    def beep(self):
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
        return

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

    def _mkedit(self, hInst, rect_full, visible):
        x, y, w, h = rect_full
        style = (WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY
                 | ES_AUTOVSCROLL | (WS_VISIBLE if visible else 0))
        e = user32.CreateWindowExW(0, "EDIT", None, style, x, y, w, h,
                                   self.hwnd, None, hInst, None)
        user32.SendMessageW(e, WM_SETFONT, self._font, 1)   # keep MS Sans Serif
        if uxtheme:
            uxtheme.SetWindowTheme(e, "", "")
        return e

    def _build(self):
        hInst = kernel32.GetModuleHandleW(None)
        cls = WNDCLASS()
        cls.lpfnWndProc = self._wndproc
        cls.hInstance = hInst
        cls.hCursor = user32.LoadCursorW(None, C.c_wchar_p(IDC_ARROW))
        cls.hbrBackground = None
        cls.lpszClassName = "CodeBreakerProgress95"
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
            0, "CodeBreakerProgress95", self.title,
            WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN,     # clip children => no strobe
            x, y, self.w, self.h, None, None, hInst, None)
        self._alive = True
        self._apply_win_icon()
        # the tab panels — native read-only EDIT children. Progress shows the verdict
        # list (short), Log/Coverage fill the whole panel; only one is visible.
        self._detail = self._mkedit(hInst, self._edit_detail, False)
        self._log = self._mkedit(hInst, self._edit_full, False)
        self._cov = self._mkedit(hInst, self._edit_full, False)
        # --- the merged Explorer + Tools controls ---
        self._fontm = gdi32.CreateFontW(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                                        DEFAULT_CHARSET, 0, 0, 0, 1, "Consolas")

        def mk(cls, style, cid):
            return user32.CreateWindowExW(0, cls, None, WS_CHILD | style, 0, 0, 10, 10,
                                          self.hwnd, cid, hInst, None)
        self._combo = mk("COMBOBOX", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL | WS_TABSTOP, IDC_COMBO)
        self._funcs = mk("LISTBOX", LBS_NOTIFY | WS_VSCROLL | WS_HSCROLL | WS_BORDER | LBS_NOINTEGRALHEIGHT, IDC_FUNCS)
        self._expcode = mk("EDIT", ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | ES_AUTOHSCROLL
                           | WS_VSCROLL | WS_HSCROLL | WS_BORDER, 0)
        self._callees = mk("LISTBOX", LBS_NOTIFY | WS_VSCROLL | WS_BORDER | LBS_NOINTEGRALHEIGHT, IDC_CALLEES)
        self._callers = mk("LISTBOX", LBS_NOTIFY | WS_VSCROLL | WS_BORDER | LBS_NOINTEGRALHEIGHT, IDC_CALLERS)
        self._tools_lb = mk("LISTBOX", LBS_NOTIFY | WS_VSCROLL | WS_BORDER | LBS_NOINTEGRALHEIGHT, IDC_TOOLS)
        self._args = mk("EDIT", ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP, IDC_ARGS)
        self._runbtn = mk("BUTTON", BS_DEFPUSHBUTTON | WS_TABSTOP, IDC_RUN)
        rb = C.create_unicode_buffer("Run"); user32.SendMessageW(self._runbtn, WM_SETTEXT, 0, C.addressof(rb))
        for c in (self._combo, self._funcs, self._callees, self._callers, self._tools_lb, self._args, self._runbtn):
            user32.SendMessageW(c, WM_SETFONT, self._font, 1)
        user32.SendMessageW(self._expcode, WM_SETFONT, self._fontm, 1)
        # NB: no imports here — _build runs on the window's worker thread. Keep the
        # tool/module lists LAZY (filled on first tab entry) so _build stays light.
        self._layout()
        self._apply_tab_vis()
        self._nid = None
        user32.SetTimer(self.hwnd, GIF_TIMER, GIF_MS, None)

    def _load_gif(self):
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
        for f in frames:
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

    def _on_tab(self, i):
        self._tab = max(0, min(len(self._tabs) - 1, i))
        self._apply_tab_vis()
        name = self._tabs[self._tab]
        if name == "Tools" and not getattr(self, "_tools_filled", False):
            self._tools_filled = True
            try:
                self._fill_tools()          # safe now (post-startup, user-triggered)
            except Exception:
                pass
        if name in EXPLORER_TABS:
            if not self._modules_loaded:
                threading.Thread(target=self._load_modules, daemon=True).start()
            else:
                self._render_explorer_tab()
        self._invalidate()

    def _proc(self, hwnd, msg, wp, lp):
        if msg == WM_ERASEBKGND:
            return 1
        if msg == WM_COMMAND:
            cid, note = wp & 0xFFFF, (wp >> 16) & 0xFFFF
            if (cid == IDC_RUN and note == BN_CLICKED) or (cid == IDC_TOOLS and note == LBN_DBLCLK):
                self._launch_tool(); return 0
            if cid == IDC_COMBO and note == CBN_SELCHANGE:
                i = user32.SendMessageW(self._combo, CB_GETCURSEL, 0, 0)
                if 0 <= i < len(self._modnames):
                    threading.Thread(target=self._load_module, args=(self._modnames[i],), daemon=True).start()
                return 0
            if cid == IDC_FUNCS and note == LBN_SELCHANGE:
                self._select_func(user32.SendMessageW(self._funcs, LB_GETCURSEL, 0, 0)); return 0
            if cid == IDC_CALLERS and note == LBN_SELCHANGE:
                self._jump_xref(self._callers); return 0
            if cid == IDC_CALLEES and note == LBN_SELCHANGE:
                self._jump_xref(self._callees); return 0
            return 0
        if msg == WM_TIMER:
            if self._working and self._tabs[self._tab] == "Progress":
                if self._gif:
                    self._gif_i = (self._gif_i + 1) % len(self._gif)
                # only repaint the dashboard band (above the verdict list) — never
                # the whole client, so the log EDIT is left untouched (no strobe).
                user32.InvalidateRect(hwnd, C.byref(_rc(3, self._titleh, self.w - 3,
                                                        self._detail_top)), False)
            return 0
        if msg == WM_PAINT:
            self._paint(hwnd); return 0
        if msg == WM_LBUTTONDOWN:
            x = C.c_short(lp & 0xFFFF).value
            y = C.c_short((lp >> 16) & 0xFFFF).value
            if self._in(self._r_xbtn, x, y):
                self._cancel = True
                user32.DestroyWindow(hwnd)
                return 0
            if self._in(self._r_cancel, x, y):
                self._cancel = True
                self.set_working(False)
                self._status = "Failed - cancelled (didn't finish)"
                self.set_state("stop")
                self._invalidate()
                return 0
            for i, r in enumerate(self._tab_rects):     # switch tabs
                if self._in(r, x, y):
                    self._on_tab(i)
                    return 0
            if y < self._titleh:
                user32.ReleaseCapture()
                user32.SendMessageW(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0)
            return 0
        if msg == WM_CLOSE:
            user32.DestroyWindow(hwnd); return 0
        if msg == WM_DESTROY:
            self._alive = False
            user32.PostQuitMessage(0); return 0
        return user32.DefWindowProcW(hwnd, msg, wp, lp)

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
            self._draw(mem, W_, H_)
            gdi32.BitBlt(hdc, 0, 0, W_, H_, mem, 0, 0, SRCCOPY)
        finally:
            gdi32.SelectObject(mem, old)
            gdi32.DeleteObject(bmp); gdi32.DeleteDC(mem)
            user32.EndPaint(hwnd, C.byref(ps))

    def _draw_bar(self, dc, r, pos, total, white, blockbr):
        bw, gap, pad, edge = 8, 2, 2, 2
        avail = (r.right - r.left) - 2 * (edge + pad)
        n = max(1, (avail + gap) // (bw + gap))
        block_area = n * bw + (n - 1) * gap
        trough_w = block_area + 2 * (edge + pad)
        cx = (r.left + r.right) // 2
        tl = cx - trough_w // 2
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

    def _text(self, dc, s, r, flags, color=BLACK, bold=False):
        gdi32.SelectObject(dc, self._fontb if bold else self._font)
        gdi32.SetTextColor(dc, color)
        user32.DrawTextW(dc, s, -1, C.byref(W.RECT(r.left, r.top, r.right, r.bottom)), flags)

    def _draw(self, dc, W_, H_):
        face = gdi32.CreateSolidBrush(FACE)
        white = gdi32.CreateSolidBrush(WHITE)
        block = gdi32.CreateSolidBrush(BLOCK_ERR if self._error else BLOCK)
        try:
            full = _rc(0, 0, W_, H_)
            user32.FillRect(dc, C.byref(full), face)
            user32.DrawEdge(dc, C.byref(full), EDGE_RAISED, BF_RECT)
            gdi32.SetBkMode(dc, TRANSPARENT)
            th = self._titleh
            # --- title bar (blue gradient) ---
            bands = 48
            for i in range(bands):
                t = i / (bands - 1)
                col = RGB(int(10 + 156 * t), int(36 + 166 * t), int(106 + 134 * t))
                br = gdi32.CreateSolidBrush(col)
                x0 = 3 + (W_ - 6) * i // bands
                x1 = 3 + (W_ - 6) * (i + 1) // bands
                user32.FillRect(dc, C.byref(_rc(x0, 3, x1, 3 + th)), br)
                gdi32.DeleteObject(br)
            sm, _bg = self._icons.get(self._icon_key, (None, None))
            tx = 8
            if sm:
                user32.DrawIconEx(dc, 6, 4, sm, 16, 16, 0, None, DI_NORMAL)
                tx = 28
            self._text(dc, self._titletext, _rc(tx, 3, W_ - 24, 3 + th),
                       DT_LEFT | DT_VCENTER | DT_SINGLELINE, CAP_TEXT, bold=True)
            user32.DrawFrameControl(dc, C.byref(self._r_xbtn), DFC_CAPTION, DFCS_CAPTIONCLOSE)
            # --- tab strip (property-sheet tabs) ---
            for i, r in enumerate(self._tab_rects):
                active = (i == self._tab)
                tr = _rc(r.left, r.top + (0 if active else 2), r.right,
                         r.bottom + (2 if active else 0))
                user32.DrawEdge(dc, C.byref(tr), EDGE_RAISED, BF_TOP | BF_LEFT | BF_RIGHT)
                self._text(dc, self._tabs[i], _rc(r.left, r.top, r.right, r.bottom),
                           DT_CENTER | DT_VCENTER | DT_SINGLELINE, BLACK, bold=active)
            # --- panel frame (sunken) ---
            user32.DrawEdge(dc, C.byref(W.RECT(self._r_panel.left, self._r_panel.top,
                            self._r_panel.right, self._r_panel.bottom)), EDGE_RAISED, BF_RECT)
            # --- Progress tab dashboard (only when active) ---
            if self._tabs[self._tab] == "Progress":
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
                self._text(dc, self._status, self._r_status,
                           DT_CENTER | DT_VCENTER | DT_SINGLELINE, BLACK)
                self._text(dc, "%d%%" % self.percent(), self._r_pct,
                           DT_CENTER | DT_VCENTER | DT_SINGLELINE, BLACK, bold=True)
                if self._clock_t0:
                    self._text(dc, _fmt_clock(time.time() - self._clock_t0), self._r_pct,
                               DT_RIGHT | DT_VCENTER | DT_SINGLELINE, BLACK)
                if self._next or self._left or self._bytes:
                    left_txt = ("Next: " + self._next) if self._next else ""
                    if self._left:
                        left_txt = (left_txt + "   -   " + self._left) if left_txt else self._left
                    if left_txt:
                        self._text(dc, left_txt, self._r_next,
                                   DT_LEFT | DT_VCENTER | DT_SINGLELINE, GRAYTX)
                    if self._bytes:
                        self._text(dc, self._bytes, self._r_next,
                                   DT_RIGHT | DT_VCENTER | DT_SINGLELINE, GRAYTX)
                self._draw_bar(dc, W.RECT(self._r_bar1.left, self._r_bar1.top,
                               self._r_bar1.right, self._r_bar1.bottom), self._pos,
                               self._total, white, block)
            # --- Cancel button ---
            user32.DrawFrameControl(dc, C.byref(W.RECT(self._r_cancel.left, self._r_cancel.top,
                                    self._r_cancel.right, self._r_cancel.bottom)),
                                    DFC_BUTTON, DFCS_BUTTONPUSH)
            self._text(dc, "Cancel", self._r_cancel,
                       DT_CENTER | DT_VCENTER | DT_SINGLELINE, BLACK)
            # --- status bar (sunken) ---
            user32.DrawEdge(dc, C.byref(W.RECT(self._r_status_bar.left, self._r_status_bar.top,
                            self._r_status_bar.right, self._r_status_bar.bottom)),
                            EDGE_SUNKEN, BF_RECT)
            sb = self._sbar or self._status
            self._text(dc, sb, _rc(self._r_status_bar.left + 6, self._r_status_bar.top,
                       self._r_status_bar.right - 6, self._r_status_bar.bottom),
                       DT_LEFT | DT_VCENTER | DT_SINGLELINE, BLACK)
        finally:
            gdi32.DeleteObject(face); gdi32.DeleteObject(white); gdi32.DeleteObject(block)


def open_app(module=None):
    """Open the ONE unified CodeBreaker window (Explorer + Tools + Progress/Log/
    Coverage tabs) standalone, blocking until closed. `module` opens on Code."""
    ProgressWindow().run_blocking(module)


def demo():
    p = ProgressWindow(caption="Reconstructing DEMO.DRV")
    if not p.start():
        print("could not create window"); return False
    p.set_working(True)
    p.set_total(25)
    p.set_clock(time.time())
    p.set_next("KEYBOARD.DRV")
    p.set_left("42 modules left")
    p.set_status_bar("Reconstructing drivers tier - 3 of 5 done")
    p.set_report("=" * 52 + "\nCodeBreaker analysis report - DEMO.DRV\n" + "=" * 52 +
                 "\nmodule format:      NE\nfunctions carved:   25\n  byte-exact:       25"
                 "\n  partial:          0\n  errors:           0\n\n"
                 "VERDICT: COMPLETE - every function reassembles to the shipped bytes")
    p.set_coverage("tier         modules   byte-exact    live%\n" + "-" * 44 +
                   "\ncore              7      100.0%     98.1%\nsystem           41      100.0%     97.4%"
                   "\ndrivers         985      100.0%     96.8%\napps        13/148       99.99%     95.0%")
    for i in range(1, 26):
        if p.cancelled():
            break
        p.set_status("Reconstructing seg1:0x%04x  (%d / 25)" % (0x100 + i * 8, i))
        p.set_bytes("%d / 2,551 bytes" % (i * 100))
        p.step(i)
        p.add_detail("seg1:0x%04x  OK        %d db  (%dB)" % (0x100 + i * 8, i % 4, i * 40))
        p.add_log("conductor: seg1:0x%04x reconstructed (%d db)" % (0x100 + i * 8, i % 4))
        time.sleep(0.12)
    p.set_working(False)
    p.set_state("ok")
    p.set_status("Done - 25/25 byte-exact, 90.9% live")
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
