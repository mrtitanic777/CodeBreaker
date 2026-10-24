"""runtime16.py -- which of a 16-bit module's functions are the compiler's run-time library.

    from codebreaker.verify import runtime16
    runtime16.identify(originals_by_name)   -> {function: "MDLLCEW.LIB:__aFlmul", ...}

A Windows module built with Microsoft C 8.00c (Visual C++ 1.52c) was linked with that compiler's
own libraries -- for a medium-model DLL, MDLLCEW / MDLLCAW / MNOCRTW (and LIBW, the import
library). A function of the module that is one of the library's, byte for byte outside the places
the linker filled in, is not the module's code: it needs no reconstruction (the library is linked),
and matching it is no measure of the C. Each routine of each library is set beside the start of
each function (invert/library_match reads the libraries); the longest routine that fits names it.

settings: msvc152 (the compiler's folder; its LIB folder is read).
"""
import os

LIBRARIES = ("MDLLCEW.LIB", "MDLLCAW.LIB", "MNOCRTW.LIB", "MNOCRTDW.LIB", "MLIBCEW.LIB", "MLIBCAW.LIB",
             "SDLLCEW.LIB", "SNOCRTW.LIB", "LDLLCEW.LIB", "LNOCRTW.LIB", "LIBW.LIB")
_routines = None


def routines(names=LIBRARIES):
    """[(library:name, bytes, mask)] of the libraries' public routines, longest first"""
    global _routines
    if _routines is None:
        from codebreaker.base import settings
        from codebreaker.invert.library_match import iter_modules, module_functions
        home = settings.get("msvc152") or ""
        out = []
        for lib in names:
            path = os.path.join(home, "LIB", lib)
            if not os.path.isfile(path):
                continue
            data = open(path, "rb").read()
            for start in iter_modules(data):
                try:
                    fns = module_functions(data, start)
                except Exception:
                    continue                         # (a module the reader does not take: skipped)
                for nm, code, mask in fns:
                    if len(code) >= 6 and sum(1 for b in mask if not b) >= 4:
                        out.append(("%s:%s" % (lib, nm), code, mask))
        out.sort(key=lambda r: -len(r[1]))
        _routines = out
    return _routines


def identify(originals):
    """{function: library routine} for those of `originals` {name: (bytes, mask)} that are one"""
    out = {}
    by_start = {}
    for name, code, mask in routines():
        by_start.setdefault(code[:2], []).append((name, code, mask))
    for fn, (code, omask) in originals.items():
        code = bytes(code)
        for name, lc, lm in by_start.get(code[:2], ()):
            if len(lc) > len(code) or len(lc) + 16 < len(code):
                continue                             # (a routine much shorter than the function: not it)
            if all(lm[i] or (i < len(omask) and omask[i]) or lc[i] == code[i] for i in range(len(lc))):
                out[fn] = name
                break
    return out
