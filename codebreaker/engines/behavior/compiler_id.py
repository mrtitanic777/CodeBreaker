#!/usr/bin/env python3
r"""compiler_id.py -- fingerprint the compiler + version that built a module.

Knowing the exact toolchain sharpens everything upstream: it says which CRT/library signature set
libsig should apply, which calling-convention defaults to assume, and which name-mangling scheme to
expect. Three signals, most to least reliable: the PE optional header's linker version (a direct
MSVC-generation marker), the "Rich" header MSVC stamps between the DOS stub and the PE header
(the exact @comp.id of every toolchain component that touched the object), and vendor/CRT strings
in the image. Combined into a single best-effort identification.
"""
import struct

# major linker version -> the MSVC generation that shipped it (coarse; Win95 era is links 2-6)
_LINK = {2: "MSVC 1.0 / 2.x (16/32)", 3: "MSVC 2.x", 4: "MSVC 4.x", 5: "Visual C++ 5.0",
         6: "Visual C++ 6.0", 7: "Visual C++ .NET (2002/03)", 8: "Visual C++ 2005",
         9: "Visual C++ 2008", 10: "Visual C++ 2010"}
# Rich header product-id -> component (subset of the well-known @comp.id product ids)
_PROD = {0x00: "import/old", 0x01: "import", 0x06: "cvtres", 0x0A: "C (MSVC2)", 0x0B: "C++ (MSVC2)",
         0x0F: "linker 5.x", 0x15: "C (VC6)", 0x16: "C++ (VC6)", 0x19: "linker (VC6)",
         0x5D: "C (VC2003)", 0x5E: "C++ (VC2003)", 0x5F: "linker (VC2003)"}


def _opt_header_off(d):
    e_lfanew = struct.unpack_from("<I", d, 0x3C)[0]
    return e_lfanew + 24                                     # PE sig(4) + file header(20)


def _linker_version(d):
    try:
        opt = _opt_header_off(d)
        return d[opt + 2], d[opt + 3]                        # Major, Minor linker version
    except Exception:
        return None


def _rich(d):
    """Decode the MSVC Rich header -> [(product, build, count)], or [] if absent."""
    try:
        e_lfanew = struct.unpack_from("<I", d, 0x3C)[0]
        blob = d[:e_lfanew]
        ri = blob.rfind(b"Rich")
        if ri < 0:
            return []
        key = struct.unpack_from("<I", blob, ri + 4)[0]
        # walk backward in 8-byte records (XOR key) until the "DanS" start marker
        recs, p = [], ri - 8
        while p >= 0x80:
            v0 = struct.unpack_from("<I", blob, p)[0] ^ key
            v1 = struct.unpack_from("<I", blob, p + 4)[0] ^ key
            if v0 == 0x536E6144:                            # 'DanS'
                break
            recs.append((v0 >> 16, v0 & 0xFFFF, v1))        # (product_id, build, count)
            p -= 8
        return list(reversed(recs))
    except Exception:
        return []


def _vendor_strings(d):
    out = []
    for marker, vendor in ((b"Microsoft Visual C++", "MSVC"), (b"Borland", "Borland"),
                           (b"Watcom", "Watcom"), (b"__CxxFrameHandler", "MSVC (EH)"),
                           (b"MSVCRT", "MSVC (msvcrt)"), (b"LIBC", "MSVC (static CRT)")):
        if marker in d:
            out.append(vendor)
    return out


def identify(module):
    """{'linker','compiler','rich','vendors'} best-effort compiler identification."""
    from codebreaker.core import cores
    from codebreaker.formats.pe import PEFile
    pe = PEFile(cores.get("target").resolve(module))
    d = pe.data
    lv = _linker_version(d)
    rich = _rich(d)
    vendors = _vendor_strings(d)
    guess = _LINK.get(lv[0], "unknown") if lv else "unknown"
    rich_named = [(_PROD.get(p, "prod:0x%X" % p), b, c) for p, b, c in rich]
    return {"linker": lv, "compiler": guess, "vendors": vendors,
            "rich": rich_named, "has_rich": bool(rich)}


def suggest_libset(module):
    """A label for which library signature set libsig should apply, from the identification."""
    info = identify(module)
    return info["compiler"]
