#!/usr/bin/env python3
r"""sdksyms.py -- extract the API name/ordinal database from a period Windows SDK's import
libraries (the Chicago Build 331 SDK). The .lib import records name every export -- INCLUDING
the ordinal-only exports that the shipped DLLs' own export tables leave unnamed (the gap
genordinals.py flagged). Evidence-only: these are the names Microsoft shipped in the SDK, a
strong HYPOTHESIS-tier naming source (SDK build != OSR2; cross-check by ordinal/structure).

  LIB32/*.LIB = COFF archives (!<arch>) of short-import members (IMPORT_OBJECT_HEADER).
  LIB16/*.LIB = OMF libraries; IMPDEF COMENT records (class A0, subtype 01).

    wt --target win95 sdksyms <SDK_DIR>          # build + summarize the DB
"""
import os
import struct
import sys


# --- COFF import library (LIB32) --------------------------------------------
def _coff_lib_syms(data):
    """Decorated export symbols from the archive's first '/' member (System V symbol table:
    u32-BE count, count u32-BE offsets, then count NUL-terminated names)."""
    if data[:8] != b"!<arch>\n":
        return []
    szf = data[56:66].replace(b"\x00", b" ").strip()
    if not szf.isdigit():
        return []
    size = int(szf)
    body = data[8 + 60:8 + 60 + size]
    count = struct.unpack_from(">I", body, 0)[0]
    names = body[4 + 4 * count:].split(b"\x00")
    return [n.decode("latin1", "replace") for n in names if n]


def _undecorate(sym):
    """__imp__Foo@16 / _Foo@16 -> (Foo, arity=4). Returns (clean_name, arity_or_None)."""
    s = sym
    if s.startswith("__imp__"):
        s = s[7:]
    elif s.startswith("__imp_"):
        s = s[6:]
    arity = None
    if "@" in s:
        s, _, tail = s.rpartition("@")
        if tail.isdigit():
            arity = int(tail) // 4
    if s.startswith("_") and not s.startswith("__"):
        s = s[1:]
    return s, arity


# --- OMF import library (LIB16) ---------------------------------------------
def _omf_lib_imports(data):
    """Yield (module, internal_name, ordinal_or_export_name) from OMF IMPDEF records."""
    i = 0
    n = len(data)
    while i + 3 <= n:
        rectype = data[i]
        rlen = struct.unpack_from("<H", data, i + 1)[0]
        rec = data[i + 3:i + 3 + rlen]
        i += 3 + rlen
        if rectype in (0x88, 0x89) and len(rec) >= 4 and rec[1] == 0xA0 and rec[2] == 0x01:  # COMENT IMPDEF
            p = 3
            ordflag = rec[p]; p += 1
            ln = rec[p]; p += 1
            internal = rec[p:p + ln].decode("latin1", "replace"); p += ln
            ln = rec[p]; p += 1
            module = rec[p:p + ln].decode("latin1", "replace"); p += ln
            if ordflag:
                ordv = struct.unpack_from("<H", rec, p)[0]
                yield module.upper(), internal, ordv
            else:
                ln = rec[p]; p += 1
                expname = rec[p:p + ln].decode("latin1", "replace") or internal
                yield module.upper(), internal, expname
        if rectype == 0xF1:                               # LIBEND-ish; keep scanning defensively
            pass


# --- C headers (INC16/INC32): prototypes + struct layouts -------------------
_CONV = r"(?:WINAPI|APIENTRY|PASCAL|CALLBACK|WINAPIV|STDMETHODCALLTYPE)"
_IMP = r"(?:WINBASEAPI|WINUSERAPI|WINGDIAPI|WINADVAPI|WINOLEAPI|WINMMAPI|WINCOMMCTRLAPI|STDAPI|EXTERN_C)?"
_PROTO = None
import re as _re
_PROTO = _re.compile(_IMP + r"\s*([A-Za-z_][\w \t\*]*?)\s+" + _CONV +
                     r"\s+([A-Za-z_]\w*)\s*\(([^;{()]*)\)\s*;", _re.S)


def _ptype(p):
    """'LPCSTR lpFileName' -> 'LPCSTR'  (drop the parameter name; keep the type)."""
    p = p.strip().replace("CONST", "const")
    toks = p.split()
    if len(toks) <= 1:
        return p
    # last token is the param name unless it's all-caps (a bare type like DWORD)
    return " ".join(toks[:-1]) if _re.match(r"[A-Za-z_]\w*$", toks[-1]) else p


def parse_headers(sdk_dir):
    """Return (protos, structs): {name:{ret,params}} and {name:[(type,field)]} from the SDK .h."""
    protos, structs = {}, {}
    for sub in ("INC32", "INC16"):
        d = os.path.join(sdk_dir, sub)
        if not os.path.isdir(d):
            continue
        for fn in os.listdir(d):
            if not fn.upper().endswith(".H"):
                continue
            try:
                txt = open(os.path.join(d, fn), encoding="latin1").read()
            except OSError:
                continue
            txt = _re.sub(r"/\*.*?\*/", " ", txt, flags=_re.S)
            txt = _re.sub(r"//[^\n]*", " ", txt)
            for m in _PROTO.finditer(txt):
                ret = " ".join(m.group(1).split())
                ret = _re.sub(r"^(?:WINBASEAPI|WINUSERAPI|WINGDIAPI|WINADVAPI|WINOLEAPI|WINMMAPI|"
                              r"WINCOMMCTRLAPI|EXTERN_C|extern)\s+", "", ret).strip()
                name = m.group(2)
                params = m.group(3).strip()
                if not ret or name in protos or ret in ("return", "else", "typedef"):
                    continue
                plist = [] if params in ("", "void", "VOID") else [_ptype(x) for x in params.split(",")]
                protos[name] = {"ret": ret, "params": plist}
            for m in _re.finditer(r"typedef\s+struct\s+\w*\s*\{([^{}]*)\}\s*([A-Za-z_]\w*)", txt, _re.S):
                fields = []
                for decl in m.group(1).split(";"):
                    decl = " ".join(decl.split())
                    if not decl:
                        continue
                    t = decl.split()
                    if len(t) >= 2:
                        fields.append((" ".join(t[:-1]), t[-1]))
                if m.group(2) not in structs:
                    structs[m.group(2)] = fields
    return protos, structs


def build(sdk_dir):
    """Return {'32':{dll:{name:arity}}, '16':{module:{name:ord_or_expname}}}."""
    out = {"32": {}, "16": {}}
    d32 = os.path.join(sdk_dir, "LIB32")
    if os.path.isdir(d32):
        for fn in os.listdir(d32):
            if not fn.upper().endswith(".LIB"):
                continue
            dll = fn.rsplit(".", 1)[0].upper()
            try:
                data = open(os.path.join(d32, fn), "rb").read()
            except OSError:
                continue
            for sym in _coff_lib_syms(data):
                name, arity = _undecorate(sym)
                if name and name[0].isalpha() or name.startswith("_"):
                    out["32"].setdefault(dll, {})[name] = arity
    d16 = os.path.join(sdk_dir, "LIB16")
    if os.path.isdir(d16):
        for fn in os.listdir(d16):
            if not fn.upper().endswith(".LIB"):
                continue
            try:
                data = open(os.path.join(d16, fn), "rb").read()
            except OSError:
                continue
            for mod, internal, ordv in _omf_lib_imports(data):
                out["16"].setdefault(mod, {})[internal] = ordv
    return out


def main():
    a = [x for x in sys.argv[1:] if not x.startswith("-")]
    sdk = a[0] if a else r"C:\Users\mrtit\Documents\GitHub\CodeBreaker\binaries\toolchains\archive-stash\Windows Chicago - Build 331 SDK"
    db = build(sdk)
    protos, structs = parse_headers(sdk)
    db["proto"] = protos
    db["struct"] = structs
    print("\n=== headers: %d prototypes, %d structs ===" % (len(protos), len(structs)))
    for n in ("CreateFileA", "GetProcAddress", "SendMessageA", "RegOpenKeyExA"):
        if n in protos:
            p = protos[n]
            print("  %-16s %s (%s)" % (n, p["ret"], ", ".join(p["params"])))
    if "--save" in sys.argv[1:]:
        import json
        out = a[1] if len(a) > 1 else "sdk_symbols.json"
        with open(out, "w", encoding="utf-8") as f:
            json.dump(db, f, indent=0, sort_keys=True)
        print("saved DB ->", out)
    for k, label in (("32", "LIB32 (COFF)"), ("16", "LIB16 (OMF)")):
        tot = sum(len(v) for v in db[k].values())
        byord = sum(1 for m in db[k].values() for o in m.values() if isinstance(o, int))
        print("\n=== %s : %d modules, %d symbols (%d by-ordinal) ===" % (label, len(db[k]), tot, byord))
        for mod in sorted(db[k], key=lambda m: -len(db[k][m]))[:8]:
            items = list(db[k][mod].items())
            sample = ", ".join("%s%s" % (s, "@%s" % o if o is not None else "") for s, o in items[:5])
            print("  %-14s %4d  e.g. %s" % (mod, len(db[k][mod]), sample))
    return db


if __name__ == "__main__":
    main()
