#!/usr/bin/env python3
r"""mfcord.py -- resolve MFC (and other ordinal-only DLL) imports to real names.

MFC's runtime DLL is exported BY ORDINAL -- MFC250.dll / MFC42.dll expose `@282`, `@312`, ... with
no names in the DLL -- so every MFC call in an app disassembles to `MFC250.dll!#282` instead of a
function. But the mapping is ground truth: the MFC .def file (or its import .lib) lists
`<mangled-name> @ <ordinal>` for every export. This ingests that table and applies it to a module's
ordinal imports, then demangles the names, so thousands of MFC calls across an app become real C++
signatures at once. No fabricated data: the table comes from the period .def / .lib, and an
unknown ordinal is left as `#N`.
"""
import json
import re
import struct

_REG = {}                                                    # {dll_lower: {ordinal: name}}
_DEF = re.compile(r"^\s*(\S+)\s+@\s*(\d+)")


def parse_def(path):
    """Parse an MFC-style .def -> {ordinal: name}. Lines are `<name> @ <ord> [NONAME]`."""
    table, in_exports = {}, False
    with open(path, "r", errors="replace") as f:
        for line in f:
            s = line.strip()
            if not s or s.startswith(";"):
                continue
            up = s.upper()
            if up.startswith("EXPORTS"):
                in_exports = True
                s = s[7:].strip()
            if up.startswith(("LIBRARY", "DESCRIPTION", "HEAPSIZE", "STACKSIZE", "SECTIONS",
                              "CODE", "DATA", "VERSION")):
                continue
            m = _DEF.match(s)
            if m and (in_exports or "@" in s):
                table[int(m.group(2))] = m.group(1)
    return table


def _iter_archive(data):
    """Yield each member (name, bytes) of a COFF `!<arch>` library."""
    if data[:8] != b"!<arch>\n":
        return
    p = 8
    while p + 60 <= len(data):
        hdr = data[p:p + 60]
        try:
            size = int(hdr[48:58].decode("latin1").strip() or "0")
        except ValueError:
            break
        name = hdr[0:16].decode("latin1").rstrip()
        body = data[p + 60:p + 60 + size]
        yield name, body
        p += 60 + size + (size & 1)


def parse_import_lib(path):
    """Parse a COFF import library (short-import objects) -> {ordinal: name} for by-ordinal imports."""
    with open(path, "rb") as f:
        data = f.read()
    table = {}
    for _name, body in _iter_archive(data):
        if len(body) < 20:
            continue
        sig1, sig2 = struct.unpack_from("<HH", body, 0)
        if sig1 != 0 or sig2 != 0xFFFF:                      # IMAGE_IMPORT_OBJECT_HEADER short form
            continue
        ordhint, typ = struct.unpack_from("<HH", body, 16)
        import_type = typ & 0x3
        name_type = (typ >> 2) & 0x7
        strs = body[20:]
        z = strs.find(b"\x00")
        sym = strs[:z].decode("latin1", "replace") if z >= 0 else ""
        if import_type == 1 and ordhint:                     # IMPORT_OBJECT_ORDINAL
            table[ordhint] = sym
    return table


def register(dll, table):
    _REG[dll.lower()] = table


def load_def(path, dll):
    register(dll, parse_def(path))
    return _REG[dll.lower()]


def save_json(path):
    with open(path, "w") as f:
        json.dump(_REG, f)


def load_json(path):
    global _REG
    with open(path) as f:
        _REG = {k: {int(o): n for o, n in v.items()} for k, v in json.load(f).items()}


def _readable(name):
    from codebreaker.engines.symbol import demangle
    return demangle.short(name) or demangle.demangle(name) or name


def name_for(dll, ordinal):
    """Readable name for a (dll, ordinal) import, or None if not in a loaded table."""
    t = _REG.get((dll or "").lower())
    if not t:
        return None
    raw = t.get(ordinal)
    return _readable(raw) if raw else None


def resolve_imports(module):
    """{iat_va: (dll, ordinal, name)} for the module's ordinal imports found in a loaded table."""
    from codebreaker.core import cores
    from codebreaker.formats.pe import PEFile
    pe = PEFile(cores.get("target").resolve(module))
    out = {}
    for iat_va, (dll, nm) in pe.imports().items():
        if nm.startswith("#"):
            try:
                ordn = int(nm[1:])
            except ValueError:
                continue
            resolved = name_for(dll, ordn)
            if resolved:
                out[iat_va] = (dll, ordn, resolved)
    return out
