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
declare are taken from them. The headers are read once; the table is kept in the state folder
and made again when a header folder changes.
"""
import json
import os
import re

from codebreaker.base import corpus, settings
from codebreaker.build import watcom

REV = 6
_CONVENTIONS = {"WINAPI": "stdcall", "APIENTRY": "stdcall", "PASCAL": "stdcall", "CALLBACK": "stdcall",
                "STDAPICALLTYPE": "stdcall", "STDCALL": "stdcall", "__stdcall": "stdcall", "_stdcall": "stdcall",
                "STDAPI": "stdcall", "NTAPI": "stdcall", "API": "stdcall", "__pascal": "stdcall",
                "_pascal": "stdcall", "WINAPIV": "cdecl", "CDECL": "cdecl", "cdecl": "cdecl",
                "__cdecl": "cdecl", "_cdecl": "cdecl", "STDAPIVCALLTYPE": "cdecl"}
_NOT_A_TYPE = re.compile(r"^(extern|EXTERN_C\w*|C|DECLSPEC_\w+|__declspec|[A-Z][A-Z0-9_]*API|\w+_LINKAGE|_CRTIMP|static|"
                         r"inline|__inline|dllimport|dllexport|_loadds|__loadds|EXPORTDBG)$")
_TYPE_WORDS = {"int", "long", "short", "char", "unsigned", "signed", "void", "float", "double", "const",
               "volatile", "far", "near", "struct", "union", "enum", "huge", "_far", "__far", "_near", "__near"}
_COM = ("STDAPI", "WINOLEAPI", "WINOLEAUTAPI")            # these markers stand for a result of HRESULT too
_COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)
_DIRECTIVE = re.compile(r"(?m)^[ \t]*#(?:[^\n]*\\\n)*[^\n]*$")
_CANDIDATE = re.compile(r"\b([A-Za-z_]\w*)\s*\(")
_table = None


def _split(params):
    """Parameters at the top level of a list (a function-pointer parameter has commas of its own)."""
    out, depth, cur = [], 0, ""
    for ch in params:
        depth += ch == "("
        depth -= ch == ")"
        if ch == "," and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += ch
    return [p.strip() for p in out + [cur] if p.strip()]


def _parameter(text):
    """[type, name] -- the name is "" when the header gives none."""
    text = re.sub(r"\s+", " ", text).strip()
    m = re.match(r"^(.*[\w*\]])\s+([A-Za-z_]\w*)((?:\s*\[\w*\])*)$", text)
    if m and m.group(2).lower() not in _TYPE_WORDS and not m.group(2).isupper() \
            and any(w.lower() not in ("const", "volatile", "far", "near") for w in re.findall(r"\w+", m.group(1))):
        return [(m.group(1) + m.group(3)).strip(), m.group(2)]
    return [text, ""]


def read_header(path):
    """{name: prototype} for the API declarations of one header: `<result> <CONVENTION> name(...);`"""
    try:
        with open(path, encoding="latin1") as f:
            text = _DIRECTIVE.sub(" ", _COMMENT.sub(" ", f.read()))
    except OSError:
        return {}
    out, header = {}, os.path.basename(path).lower()
    for m in _CANDIDATE.finditer(text):
        name = m.group(1)
        before = text[max(0, m.start() - 200):m.start()]
        words = re.findall(r"[A-Za-z_]\w*|\*|[;{}()=,]", before)
        if not words or name in _CONVENTIONS:
            continue
        marker = words[-1]
        convention = _CONVENTIONS.get(marker) or ("stdcall" if re.fullmatch(r"[A-Z][A-Z0-9]*API", marker) else
                                                  "cdecl" if re.fullmatch(r"[A-Z][A-Z0-9]*APIV", marker) else None)
        if not convention:
            continue
        # the result: the words between the end of the previous declaration and the marker
        result = []
        for w in reversed(words[:-1]):
            if w in ";{}()=,":
                break
            result.append(w)
        result = [w for w in reversed(result) if not _NOT_A_TYPE.match(w) and w not in _CONVENTIONS
                  and w.upper() not in ("FAR", "NEAR")]
        depth, j = 0, m.end() - 1
        while j < len(text):
            depth += text[j] == "("
            depth -= text[j] == ")"
            if depth == 0:
                break
            j += 1
        if depth or not re.match(r"\s*;", text[j + 1:j + 12]):
            continue                                         # not a declaration (a macro's use, a definition)
        inside = re.sub(r"\s+", " ", text[m.end():j]).strip()
        params = [] if inside.lower() in ("", "void") else [_parameter(p) for p in _split(inside)]
        if "typedef" in result:
            continue
        returns = " ".join(result) or ("HRESULT" if marker in _COM else "void")
        out.setdefault(name, {"returns": returns, "convention": convention, "marker": marker,
                              "parameters": params, "header": header})
    # COM's way of writing a result that is not HRESULT: STDAPI_(type) name(...)
    for m in re.finditer(r"\b(?:%s)_\(\s*([^)]*?)\s*\)\s+([A-Za-z_]\w*)\s*\(([^;{}]*?)\)\s*;" % "|".join(_COM), text):
        inside = re.sub(r"\s+", " ", m.group(3)).strip()
        out.setdefault(m.group(2), {
            "returns": m.group(1), "convention": "stdcall", "marker": "STDAPI",
            "parameters": [] if inside.lower() in ("", "void") else [_parameter(p) for p in _split(inside)],
            "header": header})
    return out


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
    parts = [str(REV)]
    for bits in sorted(folders):
        for d in folders[bits]:
            names = [fn for fn in os.listdir(d) if fn.lower().endswith(".h")]
            parts.append("%s:%d:%d" % (d, len(names), int(os.path.getmtime(d))))
    return "|".join(parts)


def _build(folders):
    table = {"32": {}, "16": {}}
    for bits, dirs in folders.items():
        for rank, d in enumerate(dirs):
            for fn in sorted(os.listdir(d)):
                if not fn.lower().endswith(".h") or "win386" in fn.lower():
                    continue                                 # (win386.h: the compiler's own 32-bit covers)
                for name, p in read_header(os.path.join(d, fn)).items():
                    known = table[bits].get(name)
                    if known is None:
                        table[bits][name] = p
                    elif rank and len(known["parameters"]) == len(p["parameters"]) \
                            and not any(nm for _t, nm in known["parameters"]):
                        for mine, theirs in zip(known["parameters"], p["parameters"]):
                            mine[1] = theirs[1]              # Microsoft's header names the parameters
    return table


def table():
    """{"32": {name: prototype}, "16": {...}, "spelling": {NAME: name}}"""
    global _table
    if _table is None:
        folders = _folders()
        stamp = _stamp(folders)
        cache = settings.state("prototypes.json")
        try:
            with open(cache, encoding="utf-8") as f:
                saved = json.load(f)
        except (OSError, ValueError):
            saved = {}
        if saved.get("stamp") != stamp:
            saved = _build(folders)
            saved["stamp"] = stamp
            tmp = "%s.%d.tmp" % (cache, os.getpid())
            with open(tmp, "w", encoding="utf-8") as f:
                json.dump(saved, f, sort_keys=True)
            os.replace(tmp, cache)
        spelling = {}
        for bits in ("32", "16"):                            # the 16-bit spelling wins: that is where it is needed
            for name in saved[bits]:
                spelling[name.upper()] = name
        saved["spelling"] = spelling
        _table = saved
    return _table


def of(name, bits=32):
    """The prototype of a Windows function, or None. Capitals do not matter."""
    t = table()
    found = t[str(bits)].get(name)
    if found is None:
        found = t[str(bits)].get(t["spelling"].get((name or "").upper(), ""))
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
