#!/usr/bin/env python3
r"""datarec.py -- string / data recovery: resolve a referenced address to its literal.

Reconstructed calls carry raw constant operands (`push 0x40a1b0`). When such a constant points into
the module's data at a printable C string, this renders it as the literal (`"Cannot open %s"`),
which is what makes the output readable. ASCII and UTF-16LE strings are recognized; a data pointer
that isn't a string is reported as `&data_<va>` so the reference is still named, not lost.
"""
import os

_SEC_CACHE = {}


def _sections(module):
    """[(lo, hi, bytes)] for every section of the module, cached per resolved path."""
    from codebreaker.base import registry as cores
    path = cores.get("target").resolve(module)
    if path in _SEC_CACHE:
        return _SEC_CACHE[path]
    from codebreaker.formats.pe import PEFile
    pe = PEFile(path)
    secs = []
    for s in pe.sections:
        lo = pe.image_base + s["vaddr"]
        raw = pe.data[s["rawptr"]:s["rawptr"] + s["rawsize"]]
        secs.append((lo, lo + max(s["rawsize"], s.get("vsize", 0)), raw, lo))
    _SEC_CACHE[path] = secs
    return secs


def read_va(module, va, n):
    for lo, hi, raw, base in _sections(module):
        if lo <= va < hi:
            off = va - base
            return raw[off:off + n]
    return b""


def _printable_ascii(b):
    s = bytearray()
    for c in b:
        if c == 0:
            return bytes(s) if len(s) >= 4 else None
        if 0x20 <= c < 0x7f or c in (9, 10, 13):
            s.append(c)
        else:
            return None
    return None


def _printable_utf16(b):
    s = bytearray()
    for i in range(0, len(b) - 1, 2):
        lo, hi = b[i], b[i + 1]
        if lo == 0 and hi == 0:
            return bytes(s) if len(s) >= 4 else None
        if hi == 0 and (0x20 <= lo < 0x7f or lo in (9, 10, 13)):
            s.append(lo)
        else:
            return None
    return None


def string_at(module, va):
    """Return the C string at va (decoded, un-prefixed), or None."""
    raw = read_va(module, va, 260)
    if not raw:
        return None
    a = _printable_ascii(raw)
    if a is not None:
        return ("A", a.decode("latin1"))
    w = _printable_utf16(raw)
    if w is not None:
        return ("W", w.decode("latin1"))
    return None


def _esc(s):
    return s.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n").replace("\t", "\\t").replace("\r", "\\r")


def render(module, v):
    """A readable literal for value v if it points at data, else None. Strings -> "..."; other
    in-section data pointers -> &data_<va>."""
    if v is None or v < 0x1000:
        return None
    try:
        st = string_at(module, v)
    except Exception:
        return None
    if st:
        kind, text = st
        text = _esc(text)
        if len(text) > 60:
            text = text[:60] + "..."
        return ('L"%s"' % text) if kind == "W" else ('"%s"' % text)
    # a non-string pointer into a data section: still name it
    try:
        for lo, hi, _raw, _b in _sections(module):
            if lo <= v < hi:
                return "&data_%x" % v
    except Exception:
        pass
    return None
