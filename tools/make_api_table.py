#!/usr/bin/env python3
"""make_api_table.py -- (re)generate codebreaker/symbols/win32_arguments.json from the Win32 headers.

    python tools/make_api_table.py

For every function the headers declare, the bytes of arguments it takes: 4 per parameter of a
prototype whose callee removes them. Left out (so the exporting module's own code decides):
variadic functions, caller-cleans functions, and any function with a parameter passed by value
that is wider than four bytes -- for those the parameter COUNT is not the stack size.

The headers are OpenWatcom's (the ones reconstructions are compiled against). The package only
reads the table; the header parser lives here because it is needed once, not on every run.
"""
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

_BY_VALUE = re.compile(r"\b(POINT|POINTS|SIZE|POINTL|SIZEL|COORD|LARGE_INTEGER|ULARGE_INTEGER|"
                       r"double|DOUBLE|LONGLONG|ULONGLONG|DWORDLONG|FILETIME|CY|DATE|VARIANT)\b(?!\s*\*)")


# calling-convention markers that sit between the return type and the function name in a Win32
# header declaration. Any token of the shape <UPPER>API (WINMMAPI is import-decoration, handled
# separately) is also treated as a convention marker.
_CONV = {"WINAPI", "APIENTRY", "PASCAL", "CALLBACK", "STDAPICALLTYPE", "ACMAPI", "STDCALL",
         "WINGDIAPI", "STDAPI", "cdecl", "__cdecl", "__stdcall", "FAR"}


# import/export decoration + storage-class tokens to strip from a captured return type.
_DECORATION = {"WINMMAPI", "WINCOMMCTRLAPI", "WINVERAPI", "WINCOMMDLGAPI", "DECLSPEC_IMPORT",
               "extern", "EXTERN_C", "__declspec(dllimport)", "WINOLEAPI", "WINOLEAUTAPI",
               "WINADVAPI", "WINBASEAPI", "WINUSERAPI", "WINGDIAPI", "NTAPI", "LWSTDAPI"}


_COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)


_PREPROC = re.compile(r"^[ \t]*#.*$", re.M)                  # #define/#ifdef/#include/... lines


_NAME_PAREN = re.compile(r"\b([A-Za-z_]\w*)\s*\(")


def _strip_comments(text):
    """Drop comments AND preprocessor directive lines -- so a captured return type is the real type
    and not text bleeding out of a preceding #define/#ifdef (a \\ continued macro is dropped whole)."""
    text = _COMMENT.sub(" ", text)
    # remove line-continued preprocessor blocks first (a '#...\' run), then single directive lines
    text = re.sub(r"^[ \t]*#(?:[^\n]*\\\n)+[^\n]*$", " ", text, flags=re.M)
    text = _PREPROC.sub(" ", text)
    return text


def _clean_ret(tokens):
    """Return type from the tokens before the convention marker: drop decoration/storage, keep the
    real type words + any '*'."""
    keep = [t for t in tokens if t not in _DECORATION and not t.startswith("__declspec")
            and not re.match(r"^[A-Z][A-Z0-9_]*API$", t)]   # *API import/export decoration macros
    ret = " ".join(keep).strip()
    ret = re.sub(r"\s+\*", " *", re.sub(r"\s{2,}", " ", ret)).strip()
    return ret or "void"


def _norm_params(s):
    """Collapse a parameter list to one clean line (comments already stripped)."""
    s = re.sub(r"\s+", " ", s).strip()
    return s or "void"


def extract_header(path):
    """-> dict name -> {ret, params, conv, header}. Parses the WINAPI-style declarations in one
    header. Tolerant: a declaration is <ret tokens> <CONV> <name> ( <balanced params> ) ;"""
    try:
        text = _strip_comments(open(path, encoding="latin1", errors="replace").read())
    except OSError:
        return {}
    out = {}
    hdr = os.path.basename(path)
    for m in _NAME_PAREN.finditer(text):
        name = m.group(1)
        if name in _CONV or name.endswith("API") or name in ("if", "for", "while", "sizeof",
                                                              "return", "switch", "defined"):
            continue
        # the token immediately before the name must be a convention marker for this to be a decl
        pre = text[:m.start()].rstrip()
        pretok = re.findall(r"[A-Za-z_]\w*|\*", pre[-160:])
        if not pretok:
            continue
        conv = pretok[-1]
        # convention markers: WINAPI/APIENTRY/ACMAPI + any <UPPER>API, and the varargs forms
        # <UPPER>APIV (VFWAPIV = cdecl varargs, used by ICCompress etc.).
        is_conv = conv in _CONV or (conv.isupper() and (conv.endswith("API") or conv.endswith("APIV")))
        if not is_conv:
            continue
        # return type = tokens before the convention marker, back to the start of this declaration
        # (the previous ';' or '}' or '#').
        seg = re.split(r"[;{}#]", pre)[-1]
        rt = seg[:seg.rfind(conv)] if conv in seg else seg
        ret = _clean_ret(re.findall(r"[A-Za-z_]\w*|\*", rt))
        # balanced parameter list from the '(' the regex matched
        i = m.end() - 1
        depth, j = 0, i
        while j < len(text):
            if text[j] == "(":
                depth += 1
            elif text[j] == ")":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        if depth != 0:
            continue
        params = _norm_params(text[i + 1:j])
        # STDAPI / STDAPI_(type): the COM export macro expands to `EXTERN_C <type> STDAPICALLTYPE`,
        # so the real return is HRESULT (or the STDAPI_ argument), not the empty text before it.
        if conv in ("STDAPI", "STDMETHODIMP"):
            ret, conv = "HRESULT", "WINAPI"
        elif ret in ("", "void") and conv.endswith("API") and not ret:
            ret = "void"
        if name not in out:
            out[name] = {"ret": ret or "void", "params": params, "conv": conv, "header": hdr,
                         "doc": "documented"}
    return out


# header -> which reference dirs to look in.  win32: the 32-bit SDK + DDK 32-bit includes.
# win16: the DDK's 16-bit INC16 (the headers for the actual 16-bit NE DLLs -- VER, COMMDLG,
# SHELL, MMSYSTEM, the very partners the 32-bit flat thunks target).
def _ow_header_dirs():
    """OpenWatcom's own Win32 headers -- the toolchain we actually BUILD with, so prototypes
    extracted here match exactly what wcc386 compiles against (no E1071/E1062/E1051 header-skew)."""
    from codebreaker.build import watcom as toolchain
    ow = toolchain.openwatcom_root()
    return [d for d in (os.path.join(ow, "h", "nt"), os.path.join(ow, "h")) if os.path.isdir(d)]


def main():
    table = {}
    for d in _ow_header_dirs()[:1]:
        for fn in sorted(os.listdir(d)):
            if not fn.lower().endswith(".h"):
                continue
            for name, v in extract_header(os.path.join(d, fn)).items():
                conv, params = (v.get("conv") or ""), str(v.get("params") or "").strip()
                if name in table:
                    continue
                if "..." in params or conv.upper() in ("WINAPIV", "CDECL", "__CDECL", "_CDECL") or _BY_VALUE.search(params):
                    continue
                table[name] = 0 if params in ("", "void", "VOID") else 4 * (params.count(",") + 1)
    out = os.path.join(ROOT, "codebreaker", "symbols", "win32_arguments.json")
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        json.dump(table, f, sort_keys=True, indent=0)
    print("%d functions -> %s" % (len(table), os.path.relpath(out, ROOT)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
