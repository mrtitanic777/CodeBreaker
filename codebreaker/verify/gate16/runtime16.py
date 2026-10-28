"""runtime16.py -- which of a 16-bit module's functions are the compiler's run-time library.

    from codebreaker.verify.gate16 import runtime16
    runtime16.identify(originals_by_name)   -> {function: "MDLLCEW.LIB:__aFlmul", ...}

A Windows module built with Microsoft C 8.00c (Visual C++ 1.52c) was linked with that compiler's
own libraries -- for a medium-model DLL, MDLLCEW / MDLLCAW / MNOCRTW (and LIBW, the import
library). A function of the module that is one of the library's, byte for byte outside the places
the linker filled in, is not the module's code: it needs no reconstruction (the library is linked),
and matching it is no measure of the C. Each routine of each library is set beside the start of
each function (symbols/libraries reads the libraries); the longest routine that fits names it. A
far call in a routine is compared as a far call (only its address left out): a short routine
around one would otherwise fit every small function that makes a call.

settings: msvc152 (the compiler's folder; its LIB folder is read).
"""
import os

LIBRARIES = ("MDLLCEW.LIB", "MDLLCAW.LIB", "MNOCRTW.LIB", "MNOCRTDW.LIB", "MLIBCEW.LIB", "MLIBCAW.LIB",
             "SDLLCEW.LIB", "SNOCRTW.LIB", "LDLLCEW.LIB", "LNOCRTW.LIB", "LIBW.LIB")
_routines = None


def routines(names=LIBRARIES):
    """[Function] of the libraries' public routines (symbols/libraries), each named library:name,
    longest first"""
    global _routines
    if _routines is None:
        from codebreaker.base import settings
        from codebreaker.symbols import libraries
        from codebreaker.symbols.libraries import omf
        home = settings.get("msvc152") or ""
        out = []
        for lib in names:
            path = os.path.join(home, "LIB", lib)
            if not os.path.isfile(path):
                continue
            for f in libraries.functions(path)[1]:
                f = f._replace(name="%s:%s" % (lib, f.name), mask=omf.fields_only(f.mask))   # (a far call stays one)
                if len(f.code) >= 6 and sum(1 for b in f.mask if not b) >= 4:
                    out.append(f)
        out.sort(key=lambda r: -len(r.code))
        _routines = out
    return _routines


def identify(originals):
    """{function: library routine} for those of `originals` {name: (bytes, mask)} that are one"""
    from codebreaker.symbols import libraries
    out = {}
    by_start = {}
    for f in routines():
        by_start.setdefault(f.code[:2], []).append(f)
    for fn, (code, omask) in originals.items():
        code = bytes(code)
        for f in by_start.get(code[:2], ()):
            if len(f.code) + 16 < len(code):
                continue                             # (a routine much shorter than the function: not it)
            if libraries.fits(f, code, omask):
                out[fn] = f.name
                break
    return out
