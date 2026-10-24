"""wine16.py -- the 16-bit Windows modules' exports as Wine's .spec files type them.

    from codebreaker.symbols import wine16
    wine16.exports("SETUPX")[7]   -> Export(name="IpFindFirstLine", convention="pascal",
                                            args=["word", "str", "str", "ptr"], ret16=False, stub=True)

Wine (LGPL; settings: wine16 -- its `dlls` folder, the *.dll16 / *.exe16 / *.drv16 folders read)
reimplements the 16-bit system modules -- KERNEL, USER, GDI, SETUPX, the drivers -- and declares
every export in a .spec file: its ordinal, convention, the kind of each argument and whether the
result is 16 bits (-ret16) or 32. An export Wine leaves unimplemented (`stub`) often carries its
arguments in a comment (`stub IpFindFirstLine #(word str str ptr)`). Argument kinds, as pushed:

    word, s_word          2 bytes (unsigned, signed)
    long, segptr, segstr  4 bytes (a number; a far pointer passed as such)
    ptr, str              4 bytes, a far pointer (Wine converts it to a flat one)

The order is the declaration's: for pascal the first is pushed first.
"""
import os
import re
from collections import namedtuple

Export = namedtuple("Export", "name convention args ret16 stub")

WIDTH = {"word": 2, "s_word": 2, "long": 4, "segptr": 4, "segstr": 4, "ptr": 4, "str": 4}
POINTER = {"ptr", "str", "segptr", "segstr"}

_LINE = re.compile(r"^\s*(\d+)\s+(pascal|cdecl|varargs|stub|stdcall)\s+((?:-\w+(?:=\S+)?\s+)*)(\w+)\s*(?:\(([^)]*)\))?\s*(?:#\s*\(([^)]*)\))?")
_cache = {}


def _folder():
    from codebreaker.base import settings
    return settings.get("wine16") or ""


def _module(spec_name):
    stem = spec_name.split(".")[0].upper()
    return {"KRNL386": "KERNEL"}.get(stem, stem)


def exports(module):
    """{ordinal: Export} of a 16-bit module (KERNEL, USER, SETUPX, ...), or {} when Wine has none"""
    module = module.upper()
    if module not in _cache:
        _cache.update(_read_all())
        _cache.setdefault(module, {})
    return _cache[module]


def _read_all():
    out = {}
    root = _folder()
    if not root or not os.path.isdir(root):
        return out
    for d in os.listdir(root):
        if not re.search(r"\.(dll|exe|drv)16$", d):
            continue
        for f in os.listdir(os.path.join(root, d)):
            if f.endswith(".spec"):
                out.setdefault(_module(f), {}).update(read_spec(open(os.path.join(root, d, f), encoding="latin1").read()))
    return out


def read_spec(text):
    """{ordinal: Export} of one .spec file's text"""
    out = {}
    for line in text.split("\n"):
        m = _LINE.match(line)
        if not m:
            continue
        ordinal, conv, flags, name, args, commented = m.groups()
        stub = conv == "stub"
        listed = commented if stub else args
        if listed is None:
            kinds = None                             # (nothing said of its arguments)
        else:
            kinds = [a for a in listed.split() if a]
            if any(a not in WIDTH for a in kinds):
                kinds = None                         # (a kind not read here: double, int64, ...)
        out[int(ordinal)] = Export(name, "pascal" if stub else conv, kinds, "-ret16" in (flags or ""), stub)
    return out


def c_type(kind):
    """the reconstruction's type for an argument kind"""
    return "void __far *" if kind in POINTER else {"word": "uint2", "s_word": "int2"}.get(kind, "uint4")
