#!/usr/bin/env python3
r"""rtti.py -- recover C++ class names and inheritance from MSVC RTTI data.

When a C++ module is built with RTTI, the compiler emits, next to each polymorphic class's vtable,
a chain of structures that name the class and its bases: the dword just before the vtable points to
a Complete Object Locator -> a Type Descriptor (carrying the mangled type name, `.?AVCFoo@@`) and a
Class Hierarchy Descriptor -> a Base Class Array naming every base. Reading that chain recovers the
real class NAMES and the inheritance graph -- so the vtables classlayout finds become
`class CFoo : public CObject`, and every polymorphic function gets its true class. This is authored
identity the binary still carries; we only read it.
"""
import struct
from src.engines.symbol import demangle


def _read(pe, va, n):
    off = pe._rva_to_off(va - pe.image_base)
    if off is None or off + n > len(pe.data):
        return None
    return pe.data[off:off + n]


def _cstr(pe, va, maxlen=512):
    off = pe._rva_to_off(va - pe.image_base)
    if off is None:
        return None
    end = pe.data.find(b"\x00", off, off + maxlen)
    return pe.data[off:end].decode("latin1", "replace") if end > off else ""


def _type_name(rtti_name):
    """`.?AVCFoo@@` -> 'class CFoo' ; `.?AUtag@@` -> 'struct tag'. Returns the readable class name."""
    if not rtti_name or not rtti_name.startswith(".?A"):
        return rtti_name
    kind = rtti_name[3]
    kw = {"V": "class ", "U": "struct ", "W": "enum ", "T": "union "}.get(kind, "")
    rest = rtti_name[4:]
    try:
        from src.engines.symbol.demangle import _P
        qn = _P(rest).qualified()[0]
    except Exception:
        qn = rest.rstrip("@")
    return kw + qn


def _class_from_col(pe, col_va):
    """(class_name, [base_names]) from a Complete Object Locator VA, or None."""
    col = _read(pe, col_va, 20)
    if col is None:
        return None
    _sig, _off, _cd, ptd, pchd = struct.unpack("<IIIII", col)
    td = _read(pe, ptd + 8, 4)                               # TypeDescriptor.name at +8
    if td is None:
        return None
    name = _type_name(_cstr(pe, ptd + 8))
    bases = []
    chd = _read(pe, pchd, 16)
    if chd is not None:
        _s, _a, nbase, pbca = struct.unpack("<IIII", chd)
        for k in range(min(nbase, 32)):
            bref = _read(pe, pbca + k * 4, 4)
            if bref is None:
                break
            bcd = int.from_bytes(bref, "little")
            btd = _read(pe, bcd, 4)                          # BaseClassDescriptor.pTypeDescriptor at +0
            if btd is None:
                continue
            btd_va = int.from_bytes(btd, "little")
            bn = _type_name(_cstr(pe, btd_va + 8))
            if bn:
                bases.append(bn)
    return name, bases


def class_of_vtable(pe, vtable_va):
    """The class (name, bases) a vtable belongs to, via the COL at vtable_va-4, or None."""
    ref = _read(pe, vtable_va - 4, 4)
    if ref is None:
        return None
    col_va = int.from_bytes(ref, "little")
    if col_va < pe.image_base or col_va >= pe.image_base + len(pe.data) * 8:
        return None
    return _class_from_col(pe, col_va)


def reconstruct(module):
    """Recover class names + inheritance for every RTTI-carrying vtable. {'ok','classes','c'}."""
    from src.core import cores
    from src.formats.pe import PEFile
    from src.engines.behavior import classlayout
    pe = PEFile(cores.get("target").resolve(module))
    try:
        vts, va0 = classlayout.find_vtables(module)
    except Exception as e:
        return {"ok": False, "reason": "%r" % e}
    classes = []
    for vt_va, slots in vts:
        info = None
        try:
            info = class_of_vtable(pe, vt_va)
        except Exception:
            info = None
        if not info or not info[0]:
            continue
        name, bases = info
        # bases[0] is the class itself in MSVC's array; the rest are its ancestors
        ancestors = [b for b in bases[1:]] if bases else []
        classes.append({"vtable": vt_va, "name": name, "bases": ancestors, "nmethods": len(slots)})
    if not classes:
        return {"ok": False, "reason": "no RTTI found (module built without /GR, or stripped)"}
    def _bare(t):                                            # 'class CBase' -> 'CBase' for a base clause
        for kw in ("class ", "struct ", "union ", "enum "):
            if t.startswith(kw):
                return t[len(kw):]
        return t

    lines = []
    for c in classes:
        inh = (" : " + ", ".join("public %s" % _bare(b) for b in c["bases"])) if c["bases"] else ""
        lines.append("%s%s { /* vtable 0x%08X, %d virtual method(s) */ };"
                     % (c["name"], inh, c["vtable"], c["nmethods"]))
    return {"ok": True, "nclasses": len(classes), "classes": classes, "c": "\n".join(lines)}
