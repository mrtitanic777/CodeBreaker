#!/usr/bin/env python3
r"""datarec16.py -- resolve 16-bit DS offsets to string literals + recover the DGROUP data segment.

Win16 code refers to its strings and static data by a DS-relative offset into DGROUP (the module's
auto-data segment), so a call argument like 0x0A3C is really `&"Cannot open %s"`. This reads the
DGROUP segment and: (a) `resolver(nef)` turns an offset into the string literal it points at (used
by interproc16 to render real string arguments), and (b) `reconstruct` recovers the data segment's
string contents as C declarations -- the 16-bit `datarec`/`datainit`, the module's data surface.
"""


def _dgroup(nef):
    """(bytes, seg_idx) of the module's auto-data (DGROUP) segment, or (b'', None)."""
    segs = nef.segments()
    auto = nef.h.get("autodata_seg") or 0
    s = None
    for seg in segs:
        if seg["idx"] == auto and seg["kind"] == "DATA":
            s = seg; break
    if s is None:
        s = next((seg for seg in segs if seg["kind"] == "DATA"), None)
    if s is None:
        return b"", None
    return nef.data[s["file_off"]:s["file_off"] + s["length"]], s["idx"]


def _esc(s):
    return s.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n").replace("\r", "").replace("\t", "\\t")


def _string_at(blob, off, minlen=3, maxlen=256):
    """The NUL-terminated printable string at blob[off], or None."""
    if not (0 <= off < len(blob)) or not (32 <= blob[off] < 127):
        return None
    end = off
    while end < len(blob) and end - off < maxlen and 32 <= blob[end] < 127:
        end += 1
    if end - off < minlen:
        return None
    if end < len(blob) and blob[end] != 0:            # require a NUL terminator (a real C string)
        return None
    return blob[off:end].decode("latin1")


def resolver(nef):
    """offset -> '"literal"' if the DS offset points at a DGROUP string, else None."""
    blob, _seg = _dgroup(nef)
    if not blob:
        return lambda off: None

    def resolve(off):
        s = _string_at(blob, off)
        return ('"%s"' % _esc(s)) if s is not None else None
    return resolve


def strings(nef):
    """[(offset, text)] for every NUL-terminated printable string in DGROUP (deduped by offset)."""
    blob, _seg = _dgroup(nef)
    out, i = [], 0
    n = len(blob)
    while i < n:
        s = _string_at(blob, i)
        if s is not None:
            out.append((i, s)); i += len(s) + 1
        else:
            i += 1
    return out


def reconstruct(module, path=None):
    """Recover a 16-bit module's DGROUP strings as C data declarations. {'ok','nstrings','c'}."""
    from src.formats.ne import NEFile
    from src.core import cores
    p = path or (module if str(module).lower().endswith((".exe", ".dll", ".drv"))
                 else cores.get("target").resolve(module))
    try:
        nef = NEFile(p)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    ss = strings(nef)
    if not ss:
        return {"ok": False, "reason": "no DGROUP strings (not an NE, or no data segment)"}
    lines = ["// DGROUP data -- %s" % nef.module_name()]
    for off, text in ss:
        lines.append('char s_%04x[] = "%s";' % (off, _esc(text)))
    return {"ok": True, "nstrings": len(ss), "c": "\n".join(lines)}
