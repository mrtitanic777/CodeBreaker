#!/usr/bin/env python3
"""
ppc_imports.py — ordinal -> name for Xbox 360 system imports.

The XEX import table gives us libraries + ORDINALS (xboxkrnl.exe#157, xam.xex#652)
but not names. The XDK ships import libraries (COFF archives of "short import"
records) that carry name<->ordinal, so we can turn `sub_8238F094 // ordinal 157`
into `sub_8238F094 // RtlEnterCriticalSection` and, for the tractable ones, into a
real host shim.

    python ppc_imports.py                    # dump xboxkrnl.lib ordinal->name
    python ppc_imports.py <path-to>.lib
"""
import os
import struct
import sys

# where the XDK import libs live (overridable via config `xdk` or env)
def _xdk_root():
    env = os.environ.get("XBOXTRANSPILER_XDK")
    if env:
        return env
    sys.path.insert(0, os.path.join(os.path.dirname(
        os.path.dirname(os.path.abspath(__file__))), "core"))
    try:
        import config
        return config.tool("xdk")
    except Exception:
        return r"C:\Program Files (x86)\Microsoft Xbox 360 SDK"


def lib_path(libname):
    """Map an import library name (xboxkrnl.exe / xam.xex) to its XDK .lib."""
    stem = libname.split(".")[0].lower()
    return os.path.join(_xdk_root(), "lib", "xbox", stem + ".lib")


def _members(data):
    """Yield (name, body) for each COFF-archive member. Always advances, so a
    malformed header ends iteration instead of looping forever."""
    if data[:8] != b"!<arch>\n":
        return
    pos = 8
    n = len(data)
    while pos + 60 <= n:
        hdr = data[pos:pos + 60]
        if hdr[58:60] != b"`\n":            # member-header magic — bail if wrong
            break
        try:
            size = int(hdr[48:58].decode("ascii").strip())
        except ValueError:
            break
        name = hdr[0:16].decode("ascii", "replace").rstrip()
        start = pos + 60
        yield name, data[start:start + size]
        pos = start + size + (size & 1)      # members are 2-byte aligned


def ordinal_names(libname_or_path):
    """{ordinal: name} parsed from a .lib's short-import records. Accepts a
    library name (resolved under the XDK) or a direct .lib path."""
    path = libname_or_path
    if not path.lower().endswith(".lib"):
        path = lib_path(libname_or_path)
    if not os.path.exists(path):
        return {}
    with open(path, "rb") as f:
        data = f.read()
    out = {}
    for _name, body in _members(data):
        # IMPORT_OBJECT_HEADER: Sig1=0x0000, Sig2=0xFFFF marks a short-import
        if len(body) >= 22 and struct.unpack_from("<HH", body, 0) == (0x0000, 0xFFFF):
            ordinal = struct.unpack_from("<H", body, 16)[0]
            imp_name = body[20:].split(b"\x00", 1)[0].decode("latin1")
            if imp_name and ordinal:
                out[ordinal] = imp_name
    return out


# cache per library so a whole-image scaffold parses each .lib once
_CACHE = {}


def name_for(libname, ordinal):
    """Best-effort name for (library, ordinal); '' if unknown."""
    if ordinal is None:
        return ""
    if libname not in _CACHE:
        _CACHE[libname] = ordinal_names(libname)
    return _CACHE[libname].get(ordinal, "")


def main():
    lib = sys.argv[1] if len(sys.argv) > 1 else "xboxkrnl.exe"
    names = ordinal_names(lib)
    print("%s -> %d ordinal->name entries" % (lib, len(names)))
    for o in sorted(names)[:25]:
        print("  %-5d %s" % (o, names[o]))


if __name__ == "__main__":
    main()
