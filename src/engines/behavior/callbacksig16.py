#!/usr/bin/env python3
r"""callbacksig16.py -- type a 16-bit function's OWN signature when it's a Win16 callback.

We recover authored names (MAINWINDOWPROC, ABOUTDLGPROC) but emit the function as `WORD f(void)`.
A `*PROC`-named function -- or one that behaves like a callback (a WndProc calls DefWindowProc, a
DlgProc calls EndDialog) -- IS a standard Win16 callback with a KNOWN signature: WndProc is
`LRESULT CALLBACK f(HWND, UINT, WPARAM, LPARAM)`, DlgProc is `BOOL CALLBACK f(...)`, hook procs
`LRESULT CALLBACK f(int, WPARAM, LPARAM)`. This classifies the callback kind from the recovered
name + the recovered call list, and gives the real signature -- so the function reads as what the
author declared. Reliable: driven by recovered names and observed behavior, not by drifty disasm.
"""
import re

# name-suffix rules (checked in order); the authored name comes from the NE name tables
_NAME_RULES = [
    (re.compile(r"(WND|WINDOW)PROC$", re.I), "wndproc"),
    (re.compile(r"(DLG|DIALOG)(PROC|FN)$", re.I), "dlgproc"),
    (re.compile(r"(MDICHILD|FRAME)PROC$", re.I), "wndproc"),
    (re.compile(r"HOOK(PROC)?$", re.I), "hookproc"),
    (re.compile(r"ENUM\w*PROC$", re.I), "enumproc"),
    (re.compile(r"(SUBCLASS)PROC$", re.I), "wndproc"),
]
# behavioral hints: an API a callback of that kind characteristically calls
_CALL_HINTS = {
    "DEFWINDOWPROC": "wndproc", "DEFMDICHILDPROC": "wndproc", "DEFFRAMEPROC": "wndproc",
    "DEFDLGPROC": "dlgproc", "ENDDIALOG": "dlgproc", "CALLNEXTHOOKEX": "hookproc",
    "DEFHOOKPROC": "hookproc",
}
_SIG = {
    "wndproc": ("LRESULT", ["HWND hwnd", "UINT msg", "WPARAM wParam", "LPARAM lParam"]),
    "dlgproc": ("BOOL", ["HWND hDlg", "UINT msg", "WPARAM wParam", "LPARAM lParam"]),
    "hookproc": ("LRESULT", ["int code", "WPARAM wParam", "LPARAM lParam"]),
    "enumproc": ("BOOL", ["HWND hwnd", "LPARAM lParam"]),
}
_PARAM_MAP = {                                                # a0.. -> the callback's named params
    "wndproc": ["hwnd", "msg", "wParam", "lParam"],
    "dlgproc": ["hDlg", "msg", "wParam", "lParam"],
    "hookproc": ["code", "wParam", "lParam"],
    "enumproc": ["hwnd", "lParam"],
}


def classify(name, calls=()):
    """Callback kind ('wndproc'/'dlgproc'/'hookproc'/'enumproc') for a function, or None."""
    if name:
        for rx, kind in _NAME_RULES:
            if rx.search(name):
                return kind
    seen = {c.split(".")[-1].upper() for c in (calls or [])}   # behavioral fallback
    for api, kind in _CALL_HINTS.items():
        if api in seen:
            return kind
    return None


def signature(kind):
    """(ret_type, [param decls], [param names]) for a callback kind, or None."""
    if kind not in _SIG:
        return None
    ret, params = _SIG[kind]
    return ret, params, _PARAM_MAP[kind]


def apply(name, kind, body):
    """Rewrite a reconstructed body's a0..aN to the callback's named params, and return (ret, sig)."""
    ret, params, pnames = signature(kind)
    for i, pn in enumerate(pnames):
        body = re.sub(r"\ba%d\b" % i, pn, body)
    proper = re.sub(r"[^A-Za-z0-9_]", "", name) if name else "CallbackProc"
    return ret, "%s CALLBACK %s(%s)" % (ret, proper, ", ".join(params)), body
