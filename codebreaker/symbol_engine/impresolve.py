#!/usr/bin/env python3
r"""impresolve.py -- IMPORT ordinal -> API name resolver.

The naming tools (names.py / genordinals.py) answer "what does THIS module export?".
This answers the other half a decompiler needs: "a call goes to MODULE.ordinal -- what API
is that?". It maps an imported (module, ordinal) to the real API name by reading the
EXPORTING module's own export table (ground truth, the name Microsoft shipped) -- so a
live-carve decompile (Studio / il over capstone output) can turn `call KERNEL.17` into
`GlobalFree` instead of an opaque ordinal.

Resolution is by evidence only: the name comes from the exporting binary's export table
(names.export_names). Nothing is invented -- an ordinal with no export-table name resolves to
`MODULE.ord<N>` (honest, not guessed). win95 KERNEL32/USER32/... export by name; the 16-bit
KRNL386/USER/GDI export by name in their NE tables too, so both halves resolve from ground truth.

    wt --target win95 impresolve KERNEL.17 USER.1 GDI.27   # resolve one or more refs
    wt --target win95 impresolve --selftest                # prove against known ground truth

Programmatic:  from codebreaker.symbol_engine import impresolve
               impresolve.resolve("KERNEL", 17)  ->  "GLOBALFREE"
               impresolve.resolve_ref("USER.116") ->  "USER.POSTAPPMESSAGE" style "MOD.NAME"
"""
import sys

# Import-side module names (as NE import tables spell them) -> the file that EXPORTS them.
# win95's 16-bit kernel ships as KRNL386.EXE but is imported as "KERNEL".
_ALIAS = {
    "KERNEL": "KRNL386.EXE", "SYSTEM": "SYSTEM.DRV",
    "USER": "USER.EXE", "GDI": "GDI.EXE",
    "KEYBOARD": "KEYBOARD.DRV", "SOUND": "SOUND.DRV", "SHELL": "SHELL.DLL",
    "COMMDLG": "COMMDLG.DLL", "TOOLHELP": "TOOLHELP.DLL", "DDEML": "DDEML.DLL",
    # 32-bit: imported name == file name (with .DLL)
}

_cache = {}


def _module_file(dll):
    """Map an imported module name to its exporting file name."""
    u = dll.upper()
    if u in _ALIAS:
        return _ALIAS[u]
    if "." in u:                                  # already a filename (KERNEL32.DLL)
        return u
    return u + ".DLL"                             # 32-bit convention: KERNEL32 -> KERNEL32.DLL


def exporter_map(dll):
    """{ordinal: name} for the module that exports `dll`, from its own export table. Cached.
    Empty dict if the exporting binary can't be found/parsed."""
    key = dll.upper()
    if key in _cache:
        return _cache[key]
    from codebreaker.symbol_engine import names
    out = {}
    for cand in (_module_file(dll), dll.upper(), dll.upper() + ".EXE", dll.upper() + ".DLL"):
        try:
            _ne, exp = names.export_names(cand)
        except Exception:
            continue
        if exp:
            out = {o: v[0] for o, v in exp.items()}
            break
    _cache[key] = out
    return out


def resolve(dll, ordinal):
    """Import (module, ordinal) -> API name from the exporting module's export table, or
    None if that ordinal isn't a named export (caller renders MODULE.ord<N>)."""
    return exporter_map(dll).get(int(ordinal))


def signature(name):
    """SDK header prototype for an API name (typed), via the unified symbol DB, or None."""
    try:
        from codebreaker.symbol_engine import symdb
        return symdb.signature(name)
    except Exception:
        return None


def resolve_typed(dll, ordinal):
    """Import (module, ordinal) -> (api_name, sdk_signature_or_None). Names the ordinal from the
    exporting module's export table (ground truth), then attaches the SDK prototype (hypothesis)."""
    nm = resolve(dll, ordinal)
    return nm, (signature(nm) if nm else None)


def resolve_ref(ref):
    """Resolve a 'MODULE.ordinal' string to 'MODULE.NAME' (or 'MODULE.ord<N>' if unnamed)."""
    if "." not in ref:
        return ref
    dll, rest = ref.rsplit(".", 1)
    try:
        o = int(rest, 0)
    except ValueError:
        return ref                                # already a name
    nm = resolve(dll, o)
    return "%s.%s" % (dll.upper(), nm) if nm else "%s.ord%d" % (dll.upper(), o)


# --- ground-truth self-test -------------------------------------------------
_SELFTEST = [
    ("KERNEL", 17, "GLOBALFREE"),
    ("KERNEL", 69, "FINDATOM"),
    ("USER", 17, "GETCURSORPOS"),
    ("USER", 77, "OFFSETRECT"),
    ("GDI", 91, "GETTEXTEXTENT"),
    ("KERNEL32", 103, "AllocConsole"),
    ("USER32", 1, "ActivateKeyboardLayout"),
    ("USER32", 5, "AppendMenuA"),
]


def selftest():
    ok = tot = 0
    for dll, o, expect in _SELFTEST:
        got = resolve(dll, o)
        tot += 1
        good = got == expect
        ok += good
        print("  %-10s.%-4d -> %-24s %s" % (dll, o, got, "OK" if good else "MISS (want %s)" % expect))
    print("\nimpresolve self-test: %d/%d resolved to ground-truth API names" % (ok, tot))
    return ok == tot


def main():
    args = sys.argv[1:]
    if not args or "--selftest" in args:
        sys.exit(0 if selftest() else 1)
    for ref in args:
        print("%-16s -> %s" % (ref, resolve_ref(ref)))


if __name__ == "__main__":
    main()
