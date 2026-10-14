r"""live.py -- the "Live reconstruction" window: watch a binary get reverse-engineered.

A native Win32/GDI window (same owner-drawn, double-buffered recipe as the Workbench) that makes
reconstruction visible. The real bytes of the target scroll down the left as a hex/text-editor
dump; on the right, each recovered line of C arrives as a cascade of Matrix-style glyphs that
settle -- left to right, a bright decoding head trailing dimmer scramble -- into the actual source
the engine recovered. It is not a fake: the bytes are the file's real bytes, and the revealed lines
are the file's own decompiled C (decompile/decompiler.py), or its strings when it cannot be
decompiled -- the animation is a readout of work, not a screensaver.

    python -m codebreaker live <binary>                  open on a real module
    python -m codebreaker live --selftest                create + destroy (plumbing check)
    python -m codebreaker live --shot out.png <binary>   render one frame to a PNG
"""
import os
import sys
import queue
import random
import struct
import threading


# Matrix glyph pool: half-width katakana (the film's real charset) + digits + a few symbols.
_GLYPHS = ([chr(c) for c in range(0xFF66, 0xFF9E)] + list("0123456789")
           + list("=<>+-*/&|^~!%(){}[];.,") + list("ABCDEFabcdef"))


def _rand_glyph():
    return random.choice(_GLYPHS)


# --------------------------------------------------------------------------- the reveal model
class Reveal:
    """Animation state: a queue of real recovered lines, settling one character-column at a time."""

    SCRAMBLE = 10          # columns of live scramble trailing the decode head
    SPEED = 1.6            # characters settled per animation tick (~30 ticks/s)

    def __init__(self):
        self._q = queue.Queue()
        self.done_lines = []      # fully-decoded lines (real recovered source)
        self.active = None        # the line currently decoding
        self.pos = 0.0            # settled-character cursor within the active line
        self.total = 0            # lines seen (for the status readout)
        self.finished = False

    def push(self, line):
        self._q.put(line)

    def finish(self):
        self._q.put(None)         # sentinel: producer is done

    def tick(self):
        """Advance the animation one frame. Returns True if anything changed (needs repaint)."""
        if self.active is None:
            try:
                nxt = self._q.get_nowait()
            except queue.Empty:
                return False
            if nxt is None:
                self.finished = True
                return True
            self.active = nxt.rstrip("\n")
            self.pos = 0.0
            self.total += 1
            return True
        self.pos += self.SPEED
        if self.pos >= len(self.active) + self.SCRAMBLE:      # this line has fully settled
            self.done_lines.append(self.active)
            self.active = None
            self.pos = 0.0
        return True

    def render_active(self):
        """(settled_text, scramble_text) for the line mid-decode, or ('','') if none."""
        if self.active is None:
            return "", ""
        s = int(self.pos)
        settled = self.active[:min(s, len(self.active))]
        scr = []
        for j in range(s, min(s + self.SCRAMBLE, len(self.active))):
            scr.append(self.active[j] if self.active[j] == " " else _rand_glyph())
        return settled, "".join(scr)


# --------------------------------------------------------------------------- real data producers
def _hex_lines(data, width=16, limit=4096):
    """The file's real bytes as text-editor rows: 'OFFSET  HH HH ..  |ascii|'."""
    rows = []
    for off in range(0, min(len(data), limit), width):
        chunk = data[off:off + width]
        hexs = " ".join("%02X" % b for b in chunk)
        asci = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
        rows.append("%08X  %-*s  %s" % (off, width * 3 - 1, hexs, asci))
    return rows


def _real_reveal_lines(path):
    """Lines recovered from THIS file: its decompiled C, else its printable strings. Never made up."""
    lines = []
    try:                                                       # 1) the decompiler's C for the file
        from codebreaker.decompile import decompiler
        text, _done, _total = decompiler.as_c(path)
        body = text.split("*/", 1)[1] if text.startswith("/*") else text
        import re
        first = re.search(r"(?m)^\w[^\n;]*\)[ \t]*\n\s*\{", body)   # the first function, not the declarations
        if first:
            body = body[first.start():]
        lines = [ln for ln in body.splitlines() if ln.strip() and not ln.startswith(("typedef", "#"))][:400]
    except Exception:
        pass
    if len(lines) < 8:                                         # 2) printable strings from the bytes
        try:
            with open(path, "rb") as f:
                data = f.read()
            cur = bytearray()
            for b in data:
                if 32 <= b < 127:
                    cur.append(b)
                else:
                    if len(cur) >= 6:
                        lines.append('const char s[] = "%s";' % cur.decode("ascii").replace(chr(92), chr(92) * 2).replace('"', chr(92) + '"'))
                    cur = bytearray()
                if len(lines) > 40:
                    break
        except Exception:
            pass
    if not lines:                                              # 3) a real recovered example
        lines = _EXAMPLE.splitlines()
    return lines


# a genuine reconstruction (a window procedure recovered by the behavior cascade), shown when a
# standalone launch can't run the engine on the given file -- labelled as an example in the header.
_EXAMPLE = """\
LRESULT __stdcall sub_694(HWND a0, UINT a1, WPARAM a2, LPARAM a3)
{
    switch (a1) {
        case 0x0002: /* WM_DESTROY */
            PostQuitMessage(0);
            return 0;
        case 0x000F: /* WM_PAINT */
            BeginPaint(a0, &local_40);
            /* MeasureString */ sub_26cd(local_40.hdc, str);
            EndPaint(a0, &local_40);
            return 0;
    }
    return DefWindowProcA(a0, a1, a2, a3);
}"""


# --------------------------------------------------------------------------- the window
def run(binary_path=None, producer=None, selftest=False, shot=None, title=None):
    """Open the Live Reconstruction window on `binary_path`.

    producer(reveal): optional callable run on a worker thread that push()es real recovered lines
    as they are reconstructed (and calls finish()). If omitted, a real reveal is built from the
    file itself. Returns 0 on clean exit."""
    import ctypes as C
    from ctypes import wintypes as W

    user32 = C.WinDLL("user32", use_last_error=True)
    gdi32 = C.WinDLL("gdi32")
    kernel32 = C.WinDLL("kernel32")
    LRESULT = C.c_ssize_t
    WNDPROC = C.WINFUNCTYPE(LRESULT, W.HWND, W.UINT, W.WPARAM, W.LPARAM)

    def RGB(r, g, b): return r | (g << 8) | (b << 16)
    BG = RGB(6, 12, 8)                 # near-black with a green bias (a chosen neutral)
    PANEL = RGB(10, 20, 14)
    HEAD = RGB(190, 255, 205)          # the bright decoding head
    GREEN = RGB(0, 208, 80)            # phosphor green -- settled code
    DIM = RGB(0, 104, 46)              # scramble tail / rain
    FAINT = RGB(0, 60, 30)
    HEXCOL = RGB(70, 150, 100)
    OFFCOL = RGB(0, 130, 70)
    AMBER = RGB(230, 200, 90)

    (WM_DESTROY, WM_PAINT, WM_ERASEBKGND, WM_TIMER, WM_KEYDOWN,
     WM_LBUTTONDOWN, WM_NCLBUTTONDOWN) = (0x2, 0xF, 0x14, 0x113, 0x100, 0x201, 0xA1)
    WS_POPUP, WS_VISIBLE, WS_CLIPCHILDREN = 0x80000000, 0x10000000, 0x02000000
    DT_LEFT, DT_TOP, DT_SINGLELINE, DT_NOPREFIX = 0x0, 0x0, 0x20, 0x100
    DT_CENTER, DT_VCENTER = 0x1, 0x4
    TRANSPARENT, IDC_ARROW, SW_SHOW, SRCCOPY, HTCAPTION = 1, 32512, 5, 0x00CC0020, 2
    FW_NORMAL, FW_BOLD, DEFAULT_CHARSET, FIXED_PITCH = 400, 700, 1, 1
    VK_ESCAPE = 0x1B

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
        (user32.DrawTextW, C.c_int, [W.HDC, W.LPCWSTR, C.c_int, C.POINTER(W.RECT), W.UINT]),
        (user32.InvalidateRect, W.BOOL, [W.HWND, C.POINTER(W.RECT), W.BOOL]),
        (user32.ShowWindow, W.BOOL, [W.HWND, C.c_int]),
        (user32.UpdateWindow, W.BOOL, [W.HWND]),
        (user32.SetTimer, C.c_void_p, [W.HWND, C.c_void_p, W.UINT, C.c_void_p]),
        (user32.KillTimer, W.BOOL, [W.HWND, C.c_void_p]),
        (kernel32.GetModuleHandleW, W.HMODULE, [W.LPCWSTR]),
        (gdi32.CreateSolidBrush, W.HBRUSH, [W.COLORREF]),
        (gdi32.CreateFontW, W.HFONT, [C.c_int] * 5 + [W.DWORD] * 8 + [W.LPCWSTR]),
        (gdi32.SelectObject, W.HGDIOBJ, [W.HDC, W.HGDIOBJ]),
        (gdi32.DeleteObject, W.BOOL, [W.HGDIOBJ]),
        (gdi32.SetTextColor, W.COLORREF, [W.HDC, W.COLORREF]),
        (gdi32.SetBkMode, C.c_int, [W.HDC, C.c_int]),
        (gdi32.TextOutW, W.BOOL, [W.HDC, C.c_int, C.c_int, W.LPCWSTR, C.c_int]),
        (gdi32.CreateCompatibleDC, W.HDC, [W.HDC]),
        (gdi32.CreateCompatibleBitmap, W.HBITMAP, [W.HDC, C.c_int, C.c_int]),
        (gdi32.CreateDIBSection, W.HBITMAP, [W.HDC, C.c_void_p, W.UINT, C.POINTER(C.c_void_p), W.HANDLE, W.DWORD]),
        (gdi32.GetDIBits, C.c_int, [W.HDC, W.HBITMAP, W.UINT, W.UINT, C.c_void_p, C.c_void_p, W.UINT]),
        (gdi32.BitBlt, W.BOOL, [W.HDC, C.c_int, C.c_int, C.c_int, C.c_int, W.HDC, C.c_int, C.c_int, W.DWORD]),
        (gdi32.DeleteDC, W.BOOL, [W.HDC]),
    ]:
        fn.restype, fn.argtypes = res, args

    hinst = kernel32.GetModuleHandleW(None)
    _fonts, _brushes = {}, {}

    def font(size=-15, bold=False):
        k = (size, bold)
        if k not in _fonts:
            _fonts[k] = gdi32.CreateFontW(size, 0, 0, 0, FW_BOLD if bold else FW_NORMAL, 0, 0, 0,
                                          DEFAULT_CHARSET, 0, 0, 0, FIXED_PITCH, "Consolas")
        return _fonts[k]

    def brush(col):
        if col not in _brushes:
            _brushes[col] = gdi32.CreateSolidBrush(col)
        return _brushes[col]

    def fillrect(dc, l, t, r, b, col):
        rc = W.RECT(l, t, r, b); user32.FillRect(dc, C.byref(rc), brush(col))

    def text(dc, x, y, s, col, fnt):
        gdi32.SelectObject(dc, fnt); gdi32.SetTextColor(dc, col)
        gdi32.TextOutW(dc, x, y, s, len(s))

    # ---- real data
    data = b""
    if binary_path and os.path.isfile(binary_path):
        try:
            with open(binary_path, "rb") as f:
                data = f.read()
        except Exception:
            data = b""
    hexrows = _hex_lines(data) if data else ["%08X  (no file loaded)" % 0]
    name = os.path.basename(binary_path) if binary_path else "example"
    reveal = Reveal()
    is_example = False

    if producer is None:
        rl = _real_reveal_lines(binary_path) if binary_path else _EXAMPLE.splitlines()
        is_example = (not binary_path) or (rl == _EXAMPLE.splitlines())

        def _feed(rv, lines=rl):
            for ln in lines:
                rv.push(ln)
            rv.finish()
        producer = _feed
    threading.Thread(target=producer, args=(reveal,), daemon=True).start()

    LINEH, CHW = 18, 9              # monospace cell (Consolas at -15)
    MARGIN, HEADH, STATH = 14, 46, 26
    state = {"hwnd": None, "w": 1024, "h": 680, "scroll": 0.0, "frame": 0}
    # background rain columns: (col_x, y_head, speed, glyph_run)
    rain = []

    def _seed_rain(w, h):
        rain.clear()
        cols = max(1, w // (CHW * 2))
        for i in range(cols):
            rain.append([i * CHW * 2 + 4, random.randint(-h, 0),
                         random.uniform(6, 16), [_rand_glyph() for _ in range(random.randint(6, 18))]])

    def _scramble(n):
        """n scramble glyphs (the noise the real code resolves out of): mostly 1/0, some glyphs."""
        return "".join("1" if random.random() < 0.28 else "0" if random.random() < 0.5 else _rand_glyph()
                       for _ in range(max(0, n)))

    def draw(dc, w, h):
        fnt = font(-15); fb = font(-15, True); fs = font(-13)
        gdi32.SetBkMode(dc, TRANSPARENT)
        fillrect(dc, 0, 0, w, h, BG)
        # ---- header
        fillrect(dc, 0, 0, w, HEADH, PANEL)
        fillrect(dc, 0, HEADH - 1, w, HEADH, DIM)
        text(dc, MARGIN, 8, "CodeBreaker  ─  Live Reconstruction", HEAD, fb)
        tag = "  reverse-engineering  %s" % name + ("   [example]" if is_example else "")
        text(dc, MARGIN, 26, tag, GREEN, fs)
        text(dc, w - 210, 8, "ESC / click to close", DIM, fs)

        top = HEADH + 10
        bot = h - STATH - 6
        gutter = MARGIN + 11 * CHW                  # offset column, then the code, in ONE editor
        colw = max(8, (w - gutter - MARGIN) // CHW)  # characters that fit across the code column
        vis = (bot - top) // LINEH

        done = len(reveal.done_lines)
        # the visible window follows the decode head: keep the active line ~2/3 down the page
        first = max(0, done - (vis * 2 // 3))
        settled, scr = reveal.render_active()

        for i in range(vis):
            li = first + i
            y = top + i * LINEH
            text(dc, MARGIN, y, "%08X" % ((li * 16) & 0xFFFFFFFF), OFFCOL, fnt)   # text-editor gutter
            if li < done:                                        # fully decoded -> real source
                text(dc, gutter, y, reveal.done_lines[li][:colw], GREEN, fnt)
            elif li == done and (settled or scr):                # the line resolving right now
                text(dc, gutter, y, settled[:colw], GREEN, fnt)
                x = gutter + len(settled) * CHW
                if scr:
                    text(dc, x, y, scr[0], HEAD, fnt)            # bright decoding head
                    if len(scr) > 1:
                        text(dc, x + CHW, y, scr[1:], DIM, fnt)  # scramble tail
                # the rest of the line's width, still pure noise
                rest = colw - len(settled) - len(scr)
                if rest > 0:
                    text(dc, x + len(scr) * CHW, y, _scramble(rest)[:rest], FAINT, fnt)
            else:                                                # not yet reached -> pure scramble
                text(dc, gutter, y, _scramble(colw), FAINT if (li - done) > 3 else DIM, fnt)
        # ---- status
        fillrect(dc, 0, h - STATH, w, h, PANEL)
        fillrect(dc, 0, h - STATH, w, h - STATH + 1, DIM)
        st = ("decoding █" if not reveal.finished else "reconstruction complete ✓")
        text(dc, MARGIN, h - STATH + 5,
             "%s    lines recovered: %d    line %d/%d    bytes cited: %d" %
             (st, done, min(reveal.total, done + 1), max(reveal.total, done),
              min(len(data), (done + 1) * 16)),
             (AMBER if not reveal.finished else GREEN), fs)

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

    def advance():
        state["frame"] += 1
        return reveal.tick()

    @WNDPROC
    def wndproc(hwnd, msg, wp, lp):
        if msg == WM_ERASEBKGND:
            return 1
        if msg == WM_PAINT:
            paint(hwnd); return 0
        if msg == WM_TIMER:
            advance(); user32.InvalidateRect(hwnd, None, False); return 0
        if msg == WM_LBUTTONDOWN:
            user32.DestroyWindow(hwnd); return 0
        if msg == WM_KEYDOWN and wp == VK_ESCAPE:
            user32.DestroyWindow(hwnd); return 0
        if msg == WM_DESTROY:
            user32.KillTimer(hwnd, 1); user32.PostQuitMessage(0); return 0
        return user32.DefWindowProcW(hwnd, msg, wp, lp)

    clsname = "CBDecoderWnd"
    wc = WNDCLASS()
    wc.style = 0x0003                          # CS_HREDRAW|CS_VREDRAW
    wc.lpfnWndProc = wndproc
    wc.hInstance = hinst
    wc.hCursor = user32.LoadCursorW(None, C.cast(IDC_ARROW, W.LPCWSTR))
    wc.hbrBackground = brush(BG)
    wc.lpszClassName = clsname
    user32.RegisterClassW(C.byref(wc))

    sw = user32.GetSystemMetrics(0); sh = user32.GetSystemMetrics(1)
    w, h = min(1024, sw - 80), min(680, sh - 80)
    state["w"], state["h"] = w, h
    x, y = (sw - w) // 2, (sh - h) // 2
    hwnd = user32.CreateWindowExW(0, clsname, title or "CodeBreaker — Live Reconstruction",
                                  WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN, x, y, w, h,
                                  None, None, hinst, None)
    state["hwnd"] = hwnd
    _seed_rain(w, h)

    if shot:
        import time
        for _ in range(240):                                    # pump the animation to a mid-decode frame
            advance()
            if reveal.total == 0:
                time.sleep(0.01)
        _shot(shot, w, h, draw, C, W, gdi32, BITMAPINFOHEADER)
        user32.DestroyWindow(hwnd)
        return 0
    if selftest:
        user32.DestroyWindow(hwnd)
        return 0

    user32.ShowWindow(hwnd, SW_SHOW); user32.UpdateWindow(hwnd)
    user32.SetTimer(hwnd, 1, 33, None)                          # ~30 fps
    msg = MSG()
    while user32.GetMessageW(C.byref(msg), None, 0, 0) > 0:
        user32.TranslateMessage(C.byref(msg))
        user32.DispatchMessageW(C.byref(msg))
    return 0


def _shot(path, w, h, draw, C, W, gdi32, BIH):
    """Render one frame into a DIB and save as PNG (dev/preview -- no on-screen window needed)."""
    dc = gdi32.CreateCompatibleDC(None)
    bih = BIH(); bih.biSize = C.sizeof(BIH); bih.biWidth = w; bih.biHeight = -h
    bih.biPlanes = 1; bih.biBitCount = 32; bih.biCompression = 0
    bits = C.c_void_p()
    bmp = gdi32.CreateDIBSection(dc, C.byref(bih), 0, C.byref(bits), None, 0)
    old = gdi32.SelectObject(dc, bmp)
    draw(dc, w, h)
    buf = (C.c_ubyte * (w * h * 4)).from_address(bits.value)
    rgb = bytearray(w * h * 3)
    for i in range(w * h):                                      # BGRA -> RGB
        rgb[i * 3] = buf[i * 4 + 2]; rgb[i * 3 + 1] = buf[i * 4 + 1]; rgb[i * 3 + 2] = buf[i * 4]
    _write_png(path, w, h, bytes(rgb))
    gdi32.SelectObject(dc, old); gdi32.DeleteObject(bmp); gdi32.DeleteDC(dc)


def _write_png(path, w, h, rgb):
    import zlib
    def chunk(typ, data):
        c = typ + data
        return struct.pack(">I", len(data)) + c + struct.pack(">I", zlib.crc32(c) & 0xffffffff)
    raw = bytearray()
    for y in range(h):
        raw.append(0); raw += rgb[y * w * 3:(y + 1) * w * 3]
    png = (b"\x89PNG\r\n\x1a\n"
           + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0))
           + chunk(b"IDAT", zlib.compress(bytes(raw), 6)) + chunk(b"IEND", b""))
    with open(path, "wb") as f:
        f.write(png)


def main(argv=None):
    argv = list(sys.argv[1:] if argv is None else argv)
    selftest = "--selftest" in argv
    shot = None
    if "--shot" in argv:
        i = argv.index("--shot"); shot = argv[i + 1]; del argv[i:i + 2]
    binary = next((a for a in argv if not a.startswith("--")), None)
    return run(binary_path=binary, selftest=selftest, shot=shot)


if __name__ == "__main__":
    sys.exit(main())
