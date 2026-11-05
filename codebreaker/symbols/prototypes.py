"""prototypes.py -- how the Windows headers declare a function: result, convention, parameters.

    from codebreaker.symbols import prototypes
    prototypes.of("GetDlgItem")             {"returns": "HWND", "convention": "stdcall",
                                             "parameters": [["HWND", "hDlg"], ["int", "nIDDlgItem"]],
                                             "header": "winuser.h"}
    prototypes.of("GETDLGITEM", bits=16)    the 16-bit declaration (found whatever the capitals)
    prototypes.declaration("LZRead", 16)    "int WINAPI LZRead(HFILE, void FAR *, int)"
    prototypes.spelling("GETDLGITEM")       "GetDlgItem"    (16-bit name tables are all capitals)

symbols/api_arguments.py answers one question about an import -- how many bytes of arguments it
removes -- from the exporting module's code. This is the rest of the declaration, for reading:
what the function returns and what each parameter is.

Read from the headers reconstructions are compiled against (OpenWatcom's h/nt and h/win), so a
declaration given here is one the compiler accepts. Those headers leave parameters unnamed;
where Microsoft's own headers of the time are at hand (the corpus's reference folder) and
declare the same number of parameters, their parameter names are added, and functions only they
declare are taken from them. The headers are read once; the table is kept in the knowledge base
(platforms win32 and win16, kind "prototype": knowledge/) and made again when a header folder
changes.

Reading the headers is C++ (native/symbols/prototypes.cc, cbsymbols.dll); this is its shim.
"""
import hashlib
import os

from codebreaker.base import corpus, settings
from codebreaker.build import watcom
from codebreaker.symbols import native

REV = 6
_table = None


def _folders():
    """{"32": [folders], "16": [folders]} -- the compiler's headers first, then Microsoft's."""
    ow = watcom.openwatcom_root()
    ref = settings.get("reference") or os.path.join(corpus.design_dir(), "reference")
    want = {"32": [os.path.join(ow, "h", "nt") if ow else "",
                   os.path.join(ref, "WIN32SDK", "MSTOOLS", "INCLUDE"),
                   os.path.join(ref, "Microsoft Windows 95 DDK", "INC32")],
            "16": [os.path.join(ow, "h", "win") if ow else "",
                   os.path.join(ref, "Microsoft Windows 95 DDK", "INC16")]}
    return {bits: [d for d in dirs if d and os.path.isdir(d)] for bits, dirs in want.items()}


def _stamp(folders):
    with open(native.dll_path(), "rb") as f:                 # the reader is part of what the table is made from
        parts = [str(REV), hashlib.sha1(f.read()).hexdigest()[:16]]
    for bits in sorted(folders):
        for d in folders[bits]:
            names = [fn for fn in os.listdir(d) if fn.lower().endswith(".h")]
            parts.append("%s:%d:%d" % (d, len(names), int(os.path.getmtime(d))))
    return "|".join(parts)


def _build(folders):
    """{"32": {name: prototype}, "16": {...}}: every header of every folder, the first folder's
    declaration kept, a later folder's naming the parameters a first one left unnamed."""
    spec = "".join("%s\t%s\n" % (bits, d) for bits in folders for d in folders[bits])
    return native.ask("prototypes.build", spec)


def table():
    """{"32": {name: prototype}, "16": {...}, "spelling": {NAME: name}}"""
    global _table
    if _table is None:
        from codebreaker.knowledge import store
        folders = _folders()
        stamp = _stamp(folders)
        kb = store.open()
        built = {}

        def made(bits):
            def make():
                if not built:
                    built.update(_build(folders))
                return built[bits]
            return make
        source = "the headers: " + "; ".join(d for bits in sorted(folders) for d in folders[bits])
        saved = {"32": kb.ensure("win32", "prototype", stamp, made("32"), source, store.DERIVED),
                 "16": kb.ensure("win16", "prototype", stamp, made("16"), source, store.DERIVED)}
        spelling = {}
        for bits in ("32", "16"):                            # the 16-bit spelling wins: that is where it is needed
            for name in saved[bits]:
                spelling[name.upper()] = name
        saved["spelling"] = spelling
        _table = saved
    return _table


def of(name, bits=32):
    """The prototype of a Windows function, or None. Capitals do not matter."""
    from codebreaker.knowledge import reads
    t = table()
    platform = "win16" if str(bits) == "16" else "win32"
    found = t[str(bits)].get(name)
    reads.record(platform, "prototype", name)            # (the step asking is made from it: knowledge/reads.py)
    if found is None:
        spelled = t["spelling"].get((name or "").upper(), "")
        if spelled and spelled != name:
            reads.record(platform, "prototype", spelled)
        found = t[str(bits)].get(spelled)
    return found


def spelling(name):
    """The name as the headers write it (`GETDLGITEM` -> `GetDlgItem`, `_WSPRINTF` -> `wsprintf`);
    unchanged when they do not."""
    if not name or not name.isupper():
        return name
    known = table()["spelling"]
    return known.get(name) or (known.get(name[1:]) if name.startswith("_") else None) or name


def declaration(name, bits=32):
    """The declaration as one line of C, or None."""
    p = of(name, bits)
    if not p:
        return None
    params = ", ".join((t + " " + n).strip() for t, n in p["parameters"]) or "void"
    real = table()["spelling"].get(name.upper(), name) if name not in table()[str(bits)] else name
    return "%s %s %s(%s)" % (p["returns"], p["marker"], real, params)
