"""ordinals.py -- the name of a function that is imported by number.

    from codebreaker.symbols import ordinals
    ordinals.name("USER", 91)            "GetDlgItem"
    ordinals.name("MFC30", 800)          "??1CString@@QAE@XZ"     (demangle.readable() makes it a declaration)
    ordinals.name("KERNEL", 99999)       None
    ordinals.source("USER", 91)          "user.exe"               where the name was read
    ordinals.table("COMMDLG")            {ordinal: name}

The 16-bit system modules, the MFC libraries and parts of SHELL32, COMCTL32 and KERNEL32 are
called by number: the caller's file holds `USER.91`, not a name. Two places know the name:

 1. the module that exports the function -- its own name tables say which name has which number.
    This is read from the corpus and is the same binary the call reaches;
 2. for exports the module itself leaves unnamed (every MFC function, the undocumented system
    ones), the definition file or import library the module's maker shipped. Those are read once
    into ordinal_names.json beside this file (`python -m codebreaker names --ordinals FILE...`),
    each entry kept only where the corpus module really has an export with that number.

16-bit name tables are in capitals; a name is given as the Windows headers spell it when they
declare it (symbols/prototypes.py). How many bytes of arguments a 16-bit import takes is
symbols/api_arguments.py's business and is not repeated here.
"""
import json
import os
import re
import struct

from codebreaker.base import corpus, settings
from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import open_pe
from codebreaker.symbols import api_arguments, demangle, prototypes
from codebreaker.symbols.libraries import coff

_TABLE_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "ordinal_names.json")
# Exports no definition file or import library names, named from documentation instead, for
# people to read (documented()). name() does not use them: the decompiler's names come from
# files only, and these would change what it writes.
_DOCUMENTED = {
    "KERNEL32": ({17: "CommonUnimpStub", 20: "DosFileHandleToWin32Handle", 21: "Win32HandleToDosFileHandle",
                  22: "DisposeLZ32Handle", 41: "ThunkInitLSF", 89: "FT_PrologPrime", 90: "QT_ThunkPrime"},
                 "Geoff Chappell, Ordinal-Only Exports In KERNEL32 4.0 (names per the Windows Me symbol file)"),
}
_tables = {}                    # MODULE -> ({ordinal: name}, where it was read) from the corpus
_shipped = None                 # ordinal_names.json
_by_module_name = None          # 16-bit module name -> file, for modules whose file is named otherwise


def _stem(module):
    return api_arguments._stem(module)


def _named_modules():
    """{MODULE NAME: path} of the corpus's 16-bit modules, by the name each gives itself (a file
    called mnmnet.dll can be the module MNMNET_). Read once and kept in the state folder."""
    global _by_module_name
    if _by_module_name is None:
        cache = settings.state("module_names16.json")
        try:
            with open(cache, encoding="utf-8") as f:
                saved = json.load(f)
        except (OSError, ValueError):
            saved = {}
        if saved.get("root") != corpus.root():
            found = {}
            for path in corpus.modules("ne", exts=corpus._EXTS):
                try:
                    found.setdefault(NEFile(path).module_name().upper(), path)
                except Exception:
                    pass
            saved = {"root": corpus.root(), "modules": found}
            with open(cache, "w", encoding="utf-8") as f:
                json.dump(saved, f, sort_keys=True)
        _by_module_name = saved["modules"]
    return _by_module_name


def _from_corpus(stem):
    """({ordinal: name}, file name) read from the module that exports them, or ({}, "")."""
    if stem not in _tables:
        names, where = {}, ""
        ne = api_arguments._module(stem)
        if not ne and _named_modules().get(stem):
            try:
                ne = (NEFile(_named_modules()[stem]),)
            except Exception:
                ne = None
        if ne:
            nef = ne[0]
            for nm, o in nef.nonresident_names()[1:] + nef.resident_names()[1:]:
                names[o] = nm
            where = os.path.basename(nef.path).lower()
        else:
            for ext in api_arguments._EXTS:
                path = corpus.retail(stem.lower() + ext)
                if path and corpus.lane(path) == "pe":
                    names = {o: nm for o, nm, _rva in open_pe(path).exports() if nm}
                    where = os.path.basename(path).lower()
                    break
        _tables[stem] = (names, where)
    return _tables[stem]


def _from_file(stem):
    global _shipped
    if _shipped is None:
        try:
            with open(_TABLE_FILE, encoding="utf-8") as f:
                _shipped = json.load(f)
        except (OSError, ValueError):
            _shipped = {}
    entry = _shipped.get(stem) or {}
    return {int(o): nm for o, nm in (entry.get("names") or {}).items()}, entry.get("from", "")


def linker_name(module, ordinal):
    """The export's name exactly as the tables hold it (decorated, capitals), or None."""
    stem = _stem(module)
    return _from_corpus(stem)[0].get(int(ordinal)) or _from_file(stem)[0].get(int(ordinal))


def name(module, ordinal):
    """The export's name: as the headers spell it when they know it, else as the tables hold it."""
    raw = linker_name(module, ordinal)
    return prototypes.spelling(raw) if raw and not raw.startswith("?") else raw


def documented(module, ordinal):
    """(name, where) for an export only documentation names, or None."""
    names, where = _DOCUMENTED.get(_stem(module), ({}, ""))
    return (names[int(ordinal)], where) if int(ordinal) in names else None


def source(module, ordinal):
    """Where name() read it: the exporting file's name, or the definition file / library."""
    stem = _stem(module)
    names, where = _from_corpus(stem)
    if int(ordinal) in names:
        return where
    names, where = _from_file(stem)
    return where if int(ordinal) in names else ""


def table(module):
    """{ordinal: name} -- everything known for one module."""
    stem = _stem(module)
    out = dict(_from_file(stem)[0])
    out.update(_from_corpus(stem)[0])
    return {o: (prototypes.spelling(nm) if not nm.startswith("?") else nm) for o, nm in out.items()}


def identifier(module, ordinal):
    """The name as a C identifier, or None when it has none (unknown, or a C++ name)."""
    nm = name(module, ordinal)
    if nm and "@" in nm and not nm.startswith("?"):
        nm = demangle.plain(nm)[0]                              # `_Name@12`, `Name@12`
    return nm if nm and re.fullmatch(r"[A-Za-z_]\w*", nm) else None


def imports16(module_path):
    """{(MODULE, ordinal): name or None} for every function a 16-bit module imports by number."""
    nef = NEFile(module_path)
    refs = nef.module_refs()
    out = {}
    for seg in nef.segments():
        for r in nef.relocations(seg):
            if r["kind"] == "IMPORTORD" and 0 < r["module"] <= len(refs):
                key = (refs[r["module"] - 1].upper(), r["ordinal"])
                if key not in out:
                    out[key] = name(*key)
    return out


def imports32(module_path):
    """{(MODULE, ordinal): name or None} for every function a 32-bit module imports by number."""
    out = {}
    for dll, nm in open_pe(module_path).imports().values():
        if nm.startswith("#") and nm[1:].isdigit():
            key = (_stem(dll), int(nm[1:]))
            if key not in out:
                out[key] = name(*key)
    return out


# ---- reading definition files and import libraries (to make ordinal_names.json) ------------------
_DEF_LINE = re.compile(r"^\s*(\S+?)(?:=\S+)?\s+@\s*(\d+)")


def read_definition_file(path):
    """{MODULE: {ordinal: name}} from a linker definition file (`name @ number [NONAME]`)."""
    module, names = os.path.splitext(os.path.basename(path))[0].upper(), {}
    with open(path, errors="replace") as f:
        for line in f:
            line = line.split(";", 1)[0]
            m = re.match(r"\s*LIBRARY\s+(\w+)", line, re.I)
            if m:
                module = m.group(1).upper()
            m = _DEF_LINE.match(line)
            if m:
                names.setdefault(int(m.group(2)), m.group(1))
    return {module: names} if names else {}


def _coff_import(obj):
    """(dll stem, ordinal, symbol) when a library member imports one function by number."""
    if len(obj) >= 20 and obj[:4] == b"\0\0\xff\xff":           # the short form
        hint, kind = struct.unpack_from("<HH", obj, 16)
        symbol, dll = (obj[20:].split(b"\0") + [b"", b""])[:2]
        if (kind >> 2) & 7 == 0:
            return _stem(dll.decode("latin1")), hint, symbol.decode("latin1")
        return None
    if len(obj) < 20:
        return None
    _machine, nsec, _ts, symptr, nsym = struct.unpack_from("<HHIII", obj, 0)
    ordinal = None
    for i in range(nsec):                                       # the long form: a little object file
        o = 20 + 40 * i
        if obj[o:o + 8].rstrip(b"\0") in (b".idata$5", b".idata$4"):
            rawsize, rawptr = struct.unpack_from("<II", obj, o + 16)
            if rawsize >= 4:
                value = struct.unpack_from("<I", obj, rawptr)[0]
                if value & 0x80000000:
                    ordinal = value & 0xFFFF
    if ordinal is None:
        return None
    strings, symbol, dll, i = symptr + 18 * nsym, None, None, 0
    while i < nsym:
        rec = obj[symptr + 18 * i:symptr + 18 * i + 18]
        if len(rec) < 18:
            break
        if rec[:4] == b"\0\0\0\0":
            o = strings + struct.unpack_from("<I", rec, 4)[0]
            nm = obj[o:obj.find(b"\0", o)].decode("latin1")
        else:
            nm = rec[:8].rstrip(b"\0").decode("latin1")
        if nm.startswith("__imp_"):
            symbol = nm[6:]
        elif nm.startswith("__IMPORT_DESCRIPTOR_"):
            dll = nm[20:]
        i += 1 + rec[17]
    return (_stem(dll), ordinal, symbol) if symbol and dll else None


def read_import_library(path):
    """{MODULE: {ordinal: name}} of the functions an import library reaches by number. Reads the
    32-bit form (an archive of import members) and the 16-bit one (import records)."""
    with open(path, "rb") as f:
        data = f.read()
    out = {}
    if data[:8] == b"!<arch>\n":
        for _label, body in coff.members(data):
            hit = _coff_import(body)
            if hit:
                out.setdefault(hit[0], {}).setdefault(hit[1], hit[2])
        return out
    at, page = 0, 1
    while at + 3 <= len(data):                                  # 16-bit: comment records of class A0, kind 1
        kind, size = data[at], struct.unpack_from("<H", data, at + 1)[0]
        body = data[at + 3:at + 3 + size - 1]
        at += 3 + size
        if kind == 0xF0:                                        # the library header gives the page size
            page = size + 3
        elif kind in (0x8A, 0x8B):                              # end of a member: the next starts on a page
            at += (-at) % page
        elif kind == 0xF1:
            break
        elif kind == 0x88 and len(body) > 4 and body[1] == 0xA0 and body[2] == 1:
            by_number, n = body[3], body[4]
            internal = body[5:5 + n].decode("latin1")
            q = 5 + n
            m = body[q]
            module = body[q + 1:q + 1 + m].decode("latin1").upper()
            if by_number and q + 3 + m <= len(body):
                out.setdefault(module, {}).setdefault(struct.unpack_from("<H", body, q + 1 + m)[0], internal)
    return out


_seen = {}


def _has_export(stem, ordinal):
    """Does the corpus module export that number at all? (None: the module is not in the corpus.)"""
    if stem not in _seen:
        numbers = None
        ne = api_arguments._module(stem)
        if ne:
            numbers = set(ne[1])
        elif _named_modules().get(stem):
            numbers = set(NEFile(_named_modules()[stem]).export_ordinals())
        else:
            for ext in api_arguments._EXTS:
                path = corpus.retail(stem.lower() + ext)
                if path and corpus.lane(path) == "pe":
                    numbers = {o for o, _nm, _rva in open_pe(path).exports()}
                    break
        _seen[stem] = numbers
    return None if _seen[stem] is None else ordinal in _seen[stem]


def _agrees(stem, ordinal, nm):
    """Does the corpus module's export with that number take the arguments the name says?
    True / False, or None when the name or the code does not settle it. A table made for
    another release of a module gives itself away here: its names land on other functions."""
    expect = demangle.stack_bytes(nm)
    mod = api_arguments._mod(stem) if expect is not None else None
    rva = mod.ords.get(ordinal) if mod else None
    if not rva:
        return None
    try:
        got = api_arguments._scan(mod, rva, 4)
    except Exception:
        got = None
    return None if got is None else got == expect


def collect(paths, log=print):
    """Rewrite ordinal_names.json from definition files and import libraries (files or folders;
    earlier ones win). Kept: names for numbers the corpus module exports without a name, and
    whole tables of modules the corpus does not hold but its modules import from. Each name is
    checked against the export it would name (see _agrees); one that disagrees is dropped, and
    a table that disagrees for more than one function in twenty is dropped whole."""
    files = []
    for p in paths:
        if os.path.isdir(p):
            for dp, dirs, fns in os.walk(p):
                dirs[:] = sorted(d for d in dirs if d.upper() not in ("ALPHA", "MIPS", "PPC"))
                files += [os.path.join(dp, fn) for fn in sorted(fns)]
        else:
            files.append(p)
    wanted = set()                                              # modules something in the corpus imports by number
    for path in corpus.modules("ne", exts=corpus._EXTS):
        try:
            wanted |= {m for m, _o in imports16(path)}
        except Exception:
            pass
    for path in corpus.modules("pe", exts=corpus._EXTS):
        try:
            wanted |= {m for m, _o in imports32(path)}
        except Exception:
            pass
    out = {}
    for path in files:
        ext = os.path.splitext(path)[1].lower()
        try:
            found = read_definition_file(path) if ext == ".def" else read_import_library(path) if ext == ".lib" else {}
            with open(path, "rb") as f:
                coff = ext == ".lib" and f.read(8) == b"!<arch>\n"
        except Exception:
            found = {}
        for module, names in found.items():
            own = _from_corpus(module)[0]
            keep = {}
            for o, nm in names.items():
                present = _has_export(module, o)
                if o in own or present is False or (present is None and module not in wanted):
                    continue
                keep[o] = nm
            if not keep or module in out:
                continue
            checks = {o: _agrees(module, o, nm) for o, nm in keep.items()}
            if coff:                                            # a 32-bit library holds the linker's symbols
                keep = {o: demangle.plain(nm)[0] for o, nm in keep.items()}
            yes, no = sum(1 for v in checks.values() if v), sum(1 for v in checks.values() if v is False)
            if no * 20 > yes + no:
                log("%-10s NOT USED: %s names %d exports rightly and %d wrongly" % (module, os.path.basename(path), yes, no))
                continue
            out[module] = {"from": os.path.basename(path), "checked": [yes, no],
                           "names": {str(o): nm for o, nm in keep.items() if checks[o] is not False}}
    with open(_TABLE_FILE, "w", encoding="utf-8", newline="\n") as f:
        json.dump(out, f, sort_keys=True, indent=0)
    for module in sorted(out):
        log("%-10s %5d names from %-14s (%d checked against the code, %d dropped)" % (
            module, len(out[module]["names"]), out[module]["from"], *out[module]["checked"]))
    return out
