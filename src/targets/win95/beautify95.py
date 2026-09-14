#!/usr/bin/env python3
r"""beautify95.py -- give the decompiler output the most readable, authored form the EVIDENCE allows.

Two honest tiers (per design/RECONSTRUCTION_STANDARD.md):

  RECOVERED (real, from evidence in the binary + the shipped SDK/DDK):
    * imported API calls: `KERNEL32_LocalAlloc(...)` -> `LocalAlloc(...)` (the import table names it),
      with the owning header #included so it is declared and typed;
    * exported functions: the real SDK prototype (return type + parameter list + parameter names)
      stamped on the signature (from sdkprotos, the shipped headers);
    * period pseudo-types mapped to real Win32 types (undefined4->DWORD, uint->unsigned, ...).

  INFERRED (labelled, never claimed as recovered): internal function names stay `sub_<addr>` and
    value temporaries stay `vN` -- a stripped binary holds no name for a private function or a local,
    so any prettier name would be a guess. A provenance banner says exactly this.

    wt --target win95 beautify95 <module> [--func NAME]      # print beautified source
"""
import os
import re
import sys

from src.targets.win95 import origintriage as OT

# DLL prefixes the decompiler puts on import calls (<DLL>_<Api>) -> real API name.
_DLLS = ("KERNEL32", "USER32", "GDI32", "ADVAPI32", "SHELL32", "COMDLG32", "COMCTL32", "WINMM",
         "VERSION", "MPR", "OLE32", "OLEAUT32", "RPCRT4", "WSOCK32", "LZ32", "WINSPOOL", "KERNEL",
         "USER", "GDI", "MMSYSTEM", "COMMDLG", "SHELL", "VER", "TOOLHELP")
_APICALL = re.compile(r"\b(" + "|".join(_DLLS) + r")_([A-Za-z_]\w+)\s*\(")

_PROVENANCE = (
    "/* Beautified reconstruction. RECOVERED (from the binary's import/export tables + the shipped\n"
    " * SDK/DDK): imported API names, exported-function prototypes, Win32 types. INFERRED (labelled,\n"
    " * not claimed as recovered): a stripped binary keeps no name for a private function or a local,\n"
    " * so internal callees stay sub_<addr> and temporaries stay vN. */\n")


def _load_protos():
    p32 = OT.__dict__  # noqa -- placeholder to keep import tidy
    from src.targets.win95 import autobuild95 as A
    protos = {}
    for w16 in (True, False):                      # 32-bit wins on name clash
        protos.update(A._load_sdk_protos(win16=w16))
    return protos


def _headers_for(apis, protos):
    """#include lines for the headers the used APIs come from (<windows.h> covers the base ones)."""
    from src.targets.win95 import autobuild95 as A
    used_protos = [protos.get(a) for a in apis if protos.get(a)]
    incs = A._needed_includes(used_protos)
    return ["#include <windows.h>"] + ["#include <%s>" % h for h in incs]


def beautify_text(c_text, protos=None, export_names=None):
    """Beautify one decompiled C blob. Returns (text, used_apis)."""
    protos = protos or _load_protos()
    from src.targets.win95 import raise95
    used_apis = set()

    def _api(m):
        api = m.group(2)
        used_apis.add(api)
        return api + "("
    text = _APICALL.sub(_api, c_text)                    # KERNEL32_LocalAlloc( -> LocalAlloc(
    text = raise95.map_types(text)                        # pseudo-types -> real Win32 types
    # exported functions: stamp the real SDK prototype (return + params + names)
    if export_names:
        for nm in export_names:
            p = protos.get(nm)
            if not p:
                continue
            sig = "%s %s(%s)" % (p.get("ret") or "int", nm, p.get("params") or "void")
            text = re.sub(r"(?m)^[A-Za-z_][\w \*]*?\b(?:__fastcall|__cdecl|__stdcall)?\s*%s\s*\([^;{]*\)"
                          % re.escape(nm), sig, text, count=1)
    return text, used_apis


def beautify_module(module, func=None):
    from src.targets.win95 import autobuild95 as A
    path = module                                        # resolve a name to a corpus path
    if not (os.sep in module or "/" in module):
        root = OT._corpus_root()
        for dp, _d, fs in os.walk(root):
            for x in fs:
                if x.lower() == (module if "." in module else module + ".dll").lower():
                    path = os.path.join(dp, x); break
    _lane, exe = OT._detect(path)
    text = OT._dec(exe, path, "--module") or ""
    exps = set()
    try:
        b = A.load(path)
        exps = {n for _o, n in (b.exports() if hasattr(b, "exports") else [])} if b else set()
        exps |= {n for _o, n in (b.resident_names() if hasattr(b, "resident_names") else [])}
    except Exception:
        pass
    protos = _load_protos()
    if func:
        m = re.search(r"(?ms)^[A-Za-z_][\w \*]*?\b%s\s*\([^;{]*\)\s*\{.*?^\}" % re.escape(func), text)
        text = m.group(0) if m else text
    body, apis = beautify_text(text, protos, export_names=exps)
    return _PROVENANCE + "\n" + "\n".join(_headers_for(apis, protos)) + "\n\n" + body


def main(argv):
    pos = [x for x in argv if not x.startswith("--")]
    if not pos:
        print(__doc__); return
    func = argv[argv.index("--func") + 1] if "--func" in argv else None
    print(beautify_module(pos[0], func=func))


if __name__ == "__main__":
    main(sys.argv[1:])
