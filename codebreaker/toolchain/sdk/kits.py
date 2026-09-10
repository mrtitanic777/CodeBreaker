#!/usr/bin/env python3
r"""kits.py -- the SDK/DDK/CRT kit registry.

Discovers every SDK, DDK, and CRT in the toolchain tree (Win16 SDK, Win32 SDK, the
NT DDK, OLE, MFC), catalogs each kit's include + library directories, and builds a
symbol->header index so that a reconstructed skeleton can automatically #include the
right headers and be built with the right /I include paths and import libraries.

This is what lets the reconstruction pipeline attach the AUTHENTIC types, prototypes,
constants, structs, and libraries to a skeleton .c whose function bodies the tool
(msc6dec) + the refine loop fill in over time.

    from codebreaker.toolchain.sdk import kits
    reg = kits.Registry.discover()          # find all kits under the toolchain root
    reg.kit("win16")                         # the Win16 SDK kit (include/lib dirs)
    reg.header_for("OleDelete")              # -> "ole.h"  (which header declares it)
    reg.apidb(16)                            # merged apidb for all 16-bit kits
"""
import os, re, sys
from codebreaker.toolchain.sdk import apidb as _apidb


def _toolchain_root():
    try:
        from codebreaker.core import config
        r = config.load_config().get("msc6_toolchain")
        if r and os.path.isdir(r):
            return r
    except Exception:
        pass
    from codebreaker.toolchain.sdk import w95paths
    return w95paths.toolchain_root()


# kit definitions: (key, human name, arch bits, [include subpaths], [lib subpaths], is_crt)
# subpaths are relative to the toolchain root; only those that exist are kept.
_KITDEFS = [
    ("win16", "Win16 SDK + CRT (MSVC 1.52c)", 16,
     [r"compilers\msvc-1.52c\INCLUDE", r"compilers\msvc-1.52c\MFC\INCLUDE"],
     [r"compilers\msvc-1.52c\LIB", r"compilers\msvc-1.52c\MFC\LIB"], True),
    ("win16-ole", "Win16 OLE2", 16,
     [r"compilers\msvc-1.52c\OLE2\SAMPLES\INCLUDE"], [r"compilers\msvc-1.52c\OLE2\SAMPLES\LIB"], False),
    ("win32", "Win32 SDK + CRT (MSVC 2.0)", 32,
     [r"compilers\msvc-2.0\INCLUDE", r"compilers\msvc-2.0\MFC\INCLUDE"],
     [r"compilers\msvc-2.0\LIB", r"compilers\msvc-2.0\MFC\LIB"], True),
    ("win32-sdk", "Win32 SDK (NT 3.1)", 32,
     [r"sdks\nt31-win32-sdk\MSTOOLS\H", r"sdks\nt31-win32-sdk\MSTOOLS\OLE20\H"],
     [r"sdks\nt31-win32-sdk\MSTOOLS\LIB"], False),
    ("nt-ddk", "NT 3.51 DDK", 32,
     [r"sdks\nt351-ddk\DDK\INC"], [r"sdks\nt351-ddk\DDK\LIB"], False),
]


class Kit:
    def __init__(s, key, name, bits, include, lib, crt):
        s.key = key; s.name = name; s.bits = bits
        s.include = include; s.lib = lib; s.crt = crt
        s._db = None; s._hdr = None

    def headers(s):
        for d in s.include:
            for dp, _, fns in os.walk(d):
                for fn in fns:
                    if fn.lower().endswith((".h", ".hpp", ".inc")):
                        yield os.path.join(dp, fn)

    def apidb(s):
        """Merged apidb over all this kit's headers (cached)."""
        if s._db is None:
            db = _apidb.ApiDb()
            for h in s.headers():
                try:
                    db.add_header(open(h, encoding="latin-1").read())
                except OSError:
                    pass
            s._db = db
        return s._db

    def header_index(s):
        """symbol(upper) -> header basename that declares it (function / typedef / #define /
        struct). Lets a skeleton pick the right #include per symbol used."""
        if s._hdr is None:
            idx = {}
            for h in s.headers():
                base = os.path.basename(h)
                try:
                    txt = open(h, encoding="latin-1").read()
                except OSError:
                    continue
                for m in _apidb._PROTO.finditer(txt):
                    idx.setdefault(m.group("name").upper(), base)
                for m in re.finditer(r"\btypedef\s+[^;{}]*?\b([A-Za-z_]\w*)\s*;", txt):
                    idx.setdefault(m.group(1).upper(), base)
                for m in re.finditer(r"\b(?:struct|union|enum)\s+([A-Za-z_]\w*)", txt):
                    idx.setdefault(m.group(1).upper(), base)
                for m in re.finditer(r"^\s*#\s*define\s+([A-Za-z_]\w*)", txt, re.M):
                    idx.setdefault(m.group(1).upper(), base)
            s._hdr = idx
        return s._hdr

    def describe(s):
        return {"key": s.key, "name": s.name, "bits": s.bits, "crt": s.crt,
                "include": s.include, "lib": s.lib,
                "n_headers": sum(1 for _ in s.headers())}


class Registry:
    def __init__(s, kits):
        s.kits = kits

    @classmethod
    def discover(cls, root=None):
        root = root or _toolchain_root()
        kits = []
        for key, name, bits, inc, lib, crt in _KITDEFS:
            inc_ok = [os.path.join(root, p) for p in inc if os.path.isdir(os.path.join(root, p))]
            lib_ok = [os.path.join(root, p) for p in lib if os.path.isdir(os.path.join(root, p))]
            if inc_ok:
                kits.append(Kit(key, name, bits, inc_ok, lib_ok, crt))
        return cls(kits)

    def kit(s, key):
        return next((k for k in s.kits if k.key == key), None)

    def for_bits(s, bits):
        return [k for k in s.kits if k.bits == bits]

    def apidb(s, bits=16):
        """Merged apidb across all kits of the given arch (16 or 32)."""
        db = _apidb.ApiDb()
        for k in s.for_bits(bits):
            kd = k.apidb()
            db.funcs.update(kd.funcs); db.typedefs.update(kd.typedefs); db.consts.update(kd.consts)
        return db

    def header_for(s, symbol, bits=16):
        """The header basename that declares `symbol`, across the kits of this arch."""
        for k in s.for_bits(bits):
            h = k.header_index().get(symbol.upper())
            if h:
                return h
        return None

    def include_dirs(s, bits=16):
        return [d for k in s.for_bits(bits) for d in k.include]

    def lib_dirs(s, bits=16):
        return [d for k in s.for_bits(bits) for d in k.lib]


if __name__ == "__main__":
    reg = Registry.discover()
    print("discovered %d kits:" % len(reg.kits))
    for k in reg.kits:
        d = k.describe()
        print("  %-10s %-32s %d-bit  %d headers  %d libdirs%s"
              % (d["key"], d["name"], d["bits"], d["n_headers"], len(d["lib"]),
                 "  [CRT]" if d["crt"] else ""))
    print("\nsymbol -> header (16-bit):")
    for sym in ("OleDelete", "GlobalAlloc", "lstrlen", "CreateWindow", "OFSTRUCT"):
        print("  %-16s %s" % (sym, reg.header_for(sym, 16)))
