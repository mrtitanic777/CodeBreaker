#!/usr/bin/env python3
"""
names.py — recovered Windows 1.0 ordinal -> API name map.

Only high-confidence identifications (see ../ORDINALS.md for the evidence).
Anything not here is left as module.ordinal on purpose — we don't guess.
"""
NAMES = {
    # USER — from cross-app message-context consensus
    ("USER",   6): "PostQuitMessage",
    ("USER",  18): "SetCapture",
    ("USER",  19): "ReleaseCapture",
    ("USER",  39): "BeginPaint",
    ("USER",  40): "EndPaint",
    ("USER",  88): "EndDialog",
    ("USER", 107): "DefWindowProc",
    # GDI
    ("GDI",   69): "DeleteObject",
    # KERNEL — from INT 21h DOS-wrapper fingerprints (single-call, unambiguous)
    ("KERNEL", 25): "FatalExit",
    ("KERNEL", 47): "GetCurrentDirectory",  # DOS 47h
    ("KERNEL", 54): "GetFileAttr",          # DOS 43h
    ("KERNEL", 67): "_llseek",              # DOS 42h
    ("KERNEL", 69): "_lread",               # DOS 3Fh
}

def name(mod, ordinal):
    return NAMES.get((mod, ordinal))

def label(token):
    """'USER.88' -> 'EndDialog (USER.88)' when known, else the token."""
    if "." not in token:
        return token
    mod, _, num = token.partition(".")
    try:
        nm = NAMES.get((mod, int(num)))
    except ValueError:
        nm = None
    return f"{nm} ({token})" if nm else token
