#!/usr/bin/env python3
r"""win16proto.py -- the Win16 API prototype database (types the recovered 16-bit calls).

interproc16 recovers which Win16 APIs a function calls and the raw argument words. This adds the
missing half: the authored SIGNATURE of each API, parsed from the period SDK WINDOWS.H
(`int WINAPI MessageBox(HWND, LPCSTR, LPCSTR, UINT);`). With it, a recovered call gets its exact
parameter count (so the argument list is trimmed correctly), its argument TYPES, and -- crucially --
knows which parameters are STRING pointers, so datarec16's DS-offset resolver is applied only where
a parameter really is a string (never turning a numeric flag into a bogus literal).
"""
import re

_REG = {}                                                    # NAME_UPPER -> {"ret", "params": [type]}
_PROTO = re.compile(
    r"([A-Za-z_][\w ]*?[\w*])\s+(?:WINAPI|FAR\s+PASCAL|PASCAL|APIENTRY|CALLBACK)\s+"
    r"([A-Za-z_]\w*)\s*\(([^;{}]*?)\)\s*;")
_STRT = ("LPSTR", "LPCSTR", "LPTSTR", "LPCTSTR", "NPSTR", "PSTR", "LPSZ", "LPSTZ")


def _param_type(p):
    """The type part of a parameter declaration ('HWND hWnd' -> 'HWND', 'LPCSTR' -> 'LPCSTR')."""
    p = p.strip()
    if not p or p.upper() in ("VOID", ""):
        return None
    m = re.match(r"^(.*?)([A-Za-z_]\w*)\s*$", p)              # split off a trailing identifier name
    if m and m.group(1).strip():
        return re.sub(r"\s+", " ", m.group(1).strip())       # 'HWND hWnd' -> 'HWND'
    return re.sub(r"\s+", " ", p)                            # bare type, no name


def parse_header(path):
    """Parse a 16-bit WINDOWS.H -> {NAME_UPPER: {'ret','params'}}."""
    with open(path, "r", errors="replace") as f:
        text = f.read()
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)        # strip block comments
    text = re.sub(r"//[^\n]*", " ", text)                    # and line comments
    out = {}
    for m in _PROTO.finditer(text):
        ret, name, params = m.group(1).strip(), m.group(2), m.group(3).strip()
        plist = [] if params.upper() in ("", "VOID") else [
            t for t in (_param_type(p) for p in params.split(",")) if t]
        out[name.upper()] = {"ret": re.sub(r"\s+", " ", ret), "params": plist, "name": name}
    return out


def load(path):
    _REG.update(parse_header(path))
    return len(_REG)


_LOADED = [False]


def ensure_loaded():
    """Best-effort: locate and parse a 16-bit WINDOWS.H (most WINAPI protos wins). Cached; returns count."""
    if _LOADED[0]:
        return len(_REG)
    _LOADED[0] = True
    import os
    roots = [os.environ.get("WT_DATA_ROOT") or "", "binaries"]
    best, best_n = None, 0
    for root in roots:
        if not root or not os.path.isdir(root):
            continue
        for dp, _dn, fns in os.walk(root):
            if "windows.h" in {f.lower() for f in fns} and "include" in dp.lower():
                p = os.path.join(dp, next(f for f in fns if f.lower() == "windows.h"))
                try:
                    n = sum(1 for _ in _PROTO.finditer(open(p, "r", errors="replace").read()))
                except Exception:
                    n = 0
                if n > best_n:
                    best, best_n = p, n
        if best:
            break
    if best:
        load(best)
    return len(_REG)


def proto(name):
    """The prototype for an API (module-qualified 'USER.MESSAGEBOX' or bare 'MessageBox')."""
    if not _REG:
        ensure_loaded()
    return _REG.get(name.split(".")[-1].upper())


def arity(name):
    p = proto(name)
    return len(p["params"]) if p else None


def is_str_param(name, idx):
    """True if parameter `idx` of the API is a string pointer (LPSTR/LPCSTR/...)."""
    p = proto(name)
    if not p or idx >= len(p["params"]):
        return False
    return any(s in p["params"][idx].upper() for s in _STRT)


def signature(name):
    p = proto(name)
    if not p:
        return None
    return "%s %s(%s)" % (p["ret"], p["name"], ", ".join(p["params"]) or "void")
