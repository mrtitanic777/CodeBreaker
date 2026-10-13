#!/usr/bin/env python3
r"""apidb.py -- SDK/DDK API database (Phase 3).

Parses SDK headers (.h) and module-definition files (.def) into an API database:
each exported function -> its return type, ordinal, and a TYPED PARAMETER SIGNATURE
in exactly the form the MSC-6.0a inverse (msc6dec) consumes -- [(name,'int'|'far'|
'long'), ...]. That is the accuracy multiplier of Phase 3: with the real prototype,
msc6dec lays out far/long params correctly and recovers byte-exact C instead of
guessing the signature from `retf n` alone.

No fabrication: a type it cannot classify is reported as kind 'unknown' (surfaced,
never silently mapped), so a human/LLM resolves it rather than the tool guessing.

    from codebreaker.symbols import apidb
    db = apidb.ApiDb.from_paths(["lzexpand.h"], ["lzexpand.def"])
    db.sig("LZOpenFile")      # -> {'ret':'int', 'params':[('lpszFile','far'),...], 'ordinal':2}
"""
import os, re

# base C type spelling (pointer stripped) -> msc6dec param kind, for the 16-bit ABI
_LONG = {"long", "dword", "ulong", "longlong", "lparam", "lresult", "colorref",
         "olestatus", "handle32", "hresult"}
_WORD = {"int", "word", "hfile", "byte", "char", "bool", "short", "uint", "atom",
         "wparam", "wchar", "tchar", "handle", "hlocal", "hglobal", "hwnd", "hdc",
         "hmenu", "hbitmap", "hbrush", "hfont", "hicon", "hcursor", "hpen", "hrgn",
         "hpalette", "hinstance", "hmodule", "htask", "hgdiobj", "hkey", "hmf",
         "hmetafile", "hcolorspace", "hdrvr", "hmmio", "boolean", "olebool", "oleclipformat"}


def classify(ctype, typedefs=None, _depth=0):
    """C type string -> ('int'|'far'|'long'|'void'|'unknown'). Win16 sizes:
    handles/int/WORD/BOOL = 2, long/DWORD/LPARAM/LRESULT/COLORREF = 4, any pointer = 4 (far).
    Resolves typedef'd names through `typedefs` (name -> base spelling)."""
    t = ctype.strip()
    if "*" in t or re.search(r"\bfar\b", t) or re.match(r"(?i)LP[A-Z]?\w*$", t) or \
            re.match(r"(?i)P[A-Z]\w*$", t):
        return "far"
    base = re.sub(r"\b(const|unsigned|signed|struct|far|near|volatile|OLE_CONST)\b", " ", t)
    base = base.replace("*", " ").strip().lower()
    base = base.split()[-1] if base.split() else base
    if base in _LONG:
        return "long"
    if base in _WORD:
        return "int"
    if re.match(r"h[a-z]", base):                     # any H* handle = 2-byte near handle
        return "int"
    if base in ("void",):
        return "void"
    if typedefs and _depth < 8:                        # resolve a typedef'd name
        td = typedefs.get(base.upper())
        if td and td.upper() != base.upper():
            return classify(td, typedefs, _depth + 1)
    return "unknown"


# Win16 calling-convention spellings (WINAPI/CALLBACK/APIENTRY all == far pascal)
_CONV = r"(?:WINAPI|WINAPIV|APIENTRY|CALLBACK|PASCAL|far\s+pascal|FAR\s+PASCAL|" \
        r"far\s+cdecl|FAR\s+cdecl|_far\s+_pascal|__far\s+__pascal|near\s+pascal)"
_PROTO = re.compile(
    r"(?P<ret>[A-Za-z_][\w \t\*]*?)\b" + _CONV +
    r"\s+(?P<name>[A-Za-z_]\w*)\s*\((?P<args>[^;{]*)\)\s*;", re.S)


def _split_args(argstr, typedefs=None):
    argstr = argstr.strip()
    if argstr == "" or argstr.lower() == "void":
        return []
    out = []
    for i, a in enumerate(argstr.split(",")):
        a = a.strip()
        is_ptr = "*" in a
        # a bare type with no parameter name (common in Win16 prototypes: OleDelete(POLEOBJECT))
        if re.match(r"[\w ]*\b[A-Za-z_]\w*$", a) and len(a.split()) == 1:
            out.append(("a%d" % i, classify(a, typedefs))); continue
        m = re.match(r"(?P<type>.+?)(?P<name>[A-Za-z_]\w*)\s*(\[\s*\d*\s*\])?$", a)
        if not m:
            out.append(("a%d" % i, classify(a, typedefs))); continue
        nm = m.group("name"); ty = m.group("type").strip()
        if ty == "":                                     # no type token -> whole arg is the type
            out.append(("a%d" % i, classify(a, typedefs))); continue
        kind = "far" if is_ptr else classify(ty, typedefs)
        out.append((nm, kind))
    return out


class ApiDb:
    def __init__(self):
        self.funcs = {}      # NAME(upper) -> {'name','ret','params','ordinal','module'}
        self.ordinals = {}   # NAME(upper) -> int
        self.modules = {}    # NAME(upper) -> module
        self.consts = {}     # NAME -> int value (#define constants)
        self.typedefs = {}   # NAME(upper) -> base type spelling (for classify resolution)

    # -- ingestion --------------------------------------------------------------
    def add_header(self, text):
        for m in _PROTO.finditer(text):
            name = m.group("name")                        # store RAW; classify lazily (typedefs
            self.funcs[name.upper()] = {                  # from other files must be seen first)
                "name": name, "ret_raw": m.group("ret"), "args_raw": m.group("args"),
                "ordinal": None, "module": None,
            }
        # typedefs -> base spelling.  `typedef <base> <NAME>;` and DECLARE_HANDLE(NAME).
        for tm in re.finditer(r"\btypedef\s+([^;{}]+?)\s+\**\s*([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*;",
                              text):
            self.typedefs[tm.group(2).upper()] = tm.group(1).strip()
        for hm in re.finditer(r"DECLARE_HANDLE\s*\(\s*([A-Za-z_]\w*)", text):
            self.typedefs[hm.group(1).upper()] = "int"     # a handle is a 2-byte near handle
        # integer #define constants -> value (for typed literals)
        for cm in re.finditer(r"^\s*#\s*define\s+([A-Za-z_]\w*)\s+"
                              r"\(?\s*(-?0[xX][0-9a-fA-F]+|-?\d+)\s*[lLuU]*\s*\)?\s*$",
                              text, re.M):
            try:
                self.consts[cm.group(1)] = int(cm.group(2), 0)
            except ValueError:
                pass
        return self

    def add_dir(self, directory, exts=(".h",)):
        """Ingest every header in a directory tree (the whole SDK/DDK include set)."""
        for dp, _, fns in os.walk(directory):
            for fn in fns:
                if fn.lower().endswith(exts):
                    try:
                        self.add_header(open(os.path.join(dp, fn), encoding="latin-1").read())
                    except OSError:
                        pass
        return self

    def add_def(self, text):
        mod = None
        mm = re.search(r"^\s*LIBRARY\s+(\w+)", text, re.M | re.I)
        if mm:
            mod = mm.group(1)
        in_exports = False
        for ln in text.splitlines():
            s = ln.split(";", 1)[0].strip()
            if re.match(r"(?i)^EXPORTS\b", s):
                in_exports = True; continue
            if not in_exports or not s:
                continue
            em = re.match(r"([A-Za-z_@]\w*)\s*(?:=\s*\w+)?\s*(?:@(\d+))?", s)
            if em:
                nm = em.group(1).upper()
                if em.group(2):
                    self.ordinals[nm] = int(em.group(2))
                if mod:
                    self.modules[nm] = mod
                if nm in self.funcs:
                    self.funcs[nm]["ordinal"] = self.ordinals.get(nm)
                    self.funcs[nm]["module"] = mod
        return self

    @classmethod
    def from_paths(cls, headers=None, defs=None):
        db = cls()
        for h in headers or []:
            db.add_header(open(h, encoding="latin-1").read())
        for d in defs or []:
            db.add_def(open(d, encoding="latin-1").read())
        return db

    # -- queries ----------------------------------------------------------------
    def _resolve(self, f):
        """Lazily classify a stored raw prototype with the full typedef map, so that
        types defined in one header resolve for prototypes in another. Cached."""
        if "params" not in f:
            f["ret"] = classify(f["ret_raw"], self.typedefs)
            f["params"] = _split_args(f["args_raw"], self.typedefs)
        return f

    def sig(self, name):
        f = self.funcs.get(name.upper())
        return self._resolve(f) if f else None

    def params(self, name):
        f = self.sig(name)
        return f["params"] if f else None

    def rtype(self, name):
        f = self.sig(name)
        return f["ret"] if f else None

    def by_ordinal(self, module, ordinal):
        for nm, o in self.ordinals.items():
            if o == ordinal and (module is None or self.modules.get(nm) == module):
                return self.funcs.get(nm) or {"name": nm, "ordinal": o}
        return None

    def unknowns(self):
        """Every param/ret the parser could not classify -- for hand review."""
        bad = []
        for nm in list(self.funcs):
            f = self._resolve(self.funcs[nm])
            if f["ret"] == "unknown":
                bad.append((f["name"], "ret"))
            for pn, pk in f["params"]:
                if pk == "unknown":
                    bad.append((f["name"], pn))
        return bad


def _selftest():
    from codebreaker.targets.win95 import paths as w95paths
    here = w95paths.src("base", "lzexpand")
    db = ApiDb.from_paths([os.path.join(here, "lzexpand.h")],
                          [os.path.join(here, "lzexpand.def")])
    for name in ("LZStart", "LZCopy", "LZOpenFile", "LZSeek", "LZRead", "LZClose"):
        s = db.sig(name)
        print("%-16s ret=%-5s @%-3s params=%s" % (
            name, s["ret"], s["ordinal"], s["params"]))
    u = db.unknowns()
    print("\nunknowns (need review):", u if u else "none")


if __name__ == "__main__":
    _selftest()
