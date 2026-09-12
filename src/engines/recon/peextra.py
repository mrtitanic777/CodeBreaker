#!/usr/bin/env python3
r"""peextra.py -- three smaller PE-directory recoveries: delay imports, TLS callbacks, RCDATA.

Loose ends the main import/resource engines don't cover:
  * delay-load imports (data directory 13) -- functions bound lazily on first call; recovering them
    completes the import picture (the DLLs/APIs an app uses that don't appear in the normal IAT).
  * TLS callbacks (data directory 9) -- functions the loader runs at thread start/exit; they are
    otherwise unreferenced code, so naming them surfaces entry points a call-graph misses.
  * RT_RCDATA resources -- arbitrary custom binary blobs; classify each (ASCII / UTF-16 text /
    binary) and extract the text ones, so embedded scripts / config / templates become readable.
"""
import struct


def _pe(module):
    from src.core import cores
    from src.formats.pe import PEFile
    return PEFile(cores.get("target").resolve(module))


def _cstr(pe, rva):
    off = pe._rva_to_off(rva)
    if off is None:
        return None
    end = pe.data.find(b"\x00", off, off + 256)
    return pe.data[off:end].decode("latin1", "replace") if end > off else ""


def delay_imports(module):
    """{dll: [name, ...]} of delay-loaded imports, or {}."""
    pe = _pe(module)
    if len(pe.datadirs) < 14 or not pe.datadirs[13][0]:
        return {}
    d = pe.data
    base = pe._rva_to_off(pe.datadirs[13][0])
    if base is None:
        return {}
    out = {}
    i = 0
    while True:
        rec = d[base + i * 32: base + i * 32 + 32]
        if len(rec) < 32:
            break
        attrs, rvaName, _hmod, _iat, rvaINT = struct.unpack_from("<IIIII", rec, 0)
        if rvaName == 0 and rvaINT == 0:
            break
        i += 1
        # v2 descriptors use RVAs (attrs&1); v1 used image VAs -- normalize to RVA
        to_rva = (lambda v: v) if (attrs & 1) else (lambda v: v - pe.image_base if v else 0)
        dll = _cstr(pe, to_rva(rvaName)) or "?"
        names, thunk = [], pe._rva_to_off(to_rva(rvaINT))
        if thunk is not None:
            j = 0
            while True:
                val = struct.unpack_from("<I", d, thunk + j * 4)[0]
                if val == 0:
                    break
                if val & 0x80000000:
                    names.append("#%d" % (val & 0xFFFF))
                else:
                    po = pe._rva_to_off(to_rva(val) if not (attrs & 1) else val)
                    names.append(_cstr(pe, val + 2) if po is not None else "?")
                j += 1
        out[dll] = names
    return out


def tls_callbacks(module):
    """[callback_va, ...] from the PE TLS directory, or []."""
    pe = _pe(module)
    if len(pe.datadirs) < 10 or not pe.datadirs[9][0]:
        return []
    off = pe._rva_to_off(pe.datadirs[9][0])
    if off is None:
        return []
    _start, _end, _idx, cb_va = struct.unpack_from("<IIII", pe.data, off)
    if not cb_va:
        return []
    p = pe._rva_to_off(cb_va - pe.image_base)
    out = []
    while p is not None and p + 4 <= len(pe.data):
        v = struct.unpack_from("<I", pe.data, p)[0]
        if v == 0:
            break
        out.append(v); p += 4
    return out


def rcdata(module):
    """[(id, kind, text_or_len)] for RT_RCDATA resources, classified ascii/utf16/binary."""
    from src.engines.recon import resources
    pe = _pe(module)
    if len(pe.datadirs) < 3 or not pe.datadirs[2][0]:
        return []
    d = pe.data
    base = pe._rva_to_off(pe.datadirs[2][0])
    if base is None:
        return []
    out = []
    for tname, tid, toff, tdir in resources._dir_entries(d, base, 0):
        typ = resources._RT.get(tname & 0x7FFFFFFF, tid) if not (tname & 0x80000000) else tid
        if typ != "RCDATA" or not tdir:
            continue
        for _nn, nid, noff, ndir in resources._dir_entries(d, base, toff):
            if not ndir:
                continue
            for _l, _li, loff, ld in resources._dir_entries(d, base, noff):
                if ld:
                    continue
                rva, size = struct.unpack_from("<II", d, base + loff)
                fo = pe._rva_to_off(rva)
                if fo is None:
                    continue
                blob = d[fo:fo + size]
                out.append((nid,) + _classify(blob))
    return out


def _classify(blob):
    if not blob:
        return ("empty", 0)
    printable = sum(1 for b in blob if 32 <= b < 127 or b in (9, 10, 13))
    if printable >= 0.85 * len(blob):
        return ("ascii", blob.decode("latin1", "replace")[:400])
    if len(blob) >= 4 and blob[1] == 0 and blob[3] == 0:      # UTF-16LE text
        try:
            return ("utf16", blob.decode("utf-16le", "replace")[:400])
        except Exception:
            pass
    return ("binary", len(blob))


def reconstruct(module):
    """Recover delay imports + TLS callbacks + RCDATA classification. {'ok', ...}."""
    try:
        di = delay_imports(module)
        tls = tls_callbacks(module)
        rc = rcdata(module)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    if not (di or tls or rc):
        return {"ok": False, "reason": "no delay imports / TLS callbacks / RCDATA"}
    return {"ok": True, "delay_imports": di, "tls_callbacks": tls,
            "rcdata": [(i, k) for i, k, _v in rc], "ndelay": sum(len(v) for v in di.values()),
            "ntls": len(tls), "nrcdata": len(rc)}
