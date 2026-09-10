#!/usr/bin/env python3
"""
scaffold.py — Generate a C reconstruction skeleton for a Windows 1.0 app.

Combines everything: the NE metadata (module, description, exports, imports),
the decoded window-procedure message switch (wndproc.py), and the recovered API
names (names.py). Emits a period-accurate .c skeleton where the WndProc is a real
message switch and every handler is annotated with the (named) system calls it
makes. Internal functions are left as commented stubs to fill in.

Usage: python3 scaffold.py FILE.EXE > out.c
"""
import sys, os
from codebreaker.format_loaders.ne import NEFile
from wndproc import analyze, WM
from names import label

def wndproc_exports(ne):
    res = ne.resident_names(); nonres = ne.nonresident_names()
    out = []
    for n,o in res[1:] + nonres[1:]:
        if n.upper().endswith("WNDPROC"):
            out.append((o, n))
    return out

def dialog_exports(ne):
    res = ne.resident_names(); nonres = ne.nonresident_names()
    out = []
    for n,o in res[1:] + nonres[1:]:
        u = n.upper()
        if u.endswith("WNDPROC"): continue
        out.append((o, n))
    return out

# arg frame for a WndProc/DlgProc under PASCAL far calls
FRAME = "hWnd=[bp+0e] message=[bp+0c] wParam=[bp+0a] lParam=[bp+06]"

def emit(path):
    ne = NEFile(path)
    res = ne.resident_names(); nonres = ne.nonresident_names()
    module = res[0][0]
    desc = nonres[0][0] if nonres else ""
    exports = sorted(res[1:] + nonres[1:], key=lambda x: x[1])
    imports = ne.module_refs()

    P = print
    P(f"/*")
    P(f" * {module.lower()}.c  —  reconstruction skeleton for {os.path.basename(path)}")
    P(f" *   {desc}")
    P(f" *")
    P(f" * Auto-generated from the binary: NE metadata + decoded WndProc message")
    P(f" * switch + recovered API names. Handlers show the system calls they make;")
    P(f" * internal routines are stubs to fill in. Frame: {FRAME}")
    P(f" *")
    P(f" * imports: {', '.join(imports)}")
    P(f" * exports: {', '.join(n for n,_ in exports)}")
    P(f" */")
    P(f"#include <windows.h>\n")

    wps = wndproc_exports(ne)
    done = set(o for o,_ in wps)

    for ordv, nm in wps:
        try:
            data = analyze(path, ordv, quiet=True)
        except Exception as e:
            P(f"/* {nm}: could not decode ({e}) */\n"); continue
        P(f"/* ===== {nm}  (export @{ordv}) — window procedure ===== */")
        P(f"long FAR PASCAL {nm.title().replace('_','')}(HWND hWnd, unsigned message,")
        P(f"                              WORD wParam, LONG lParam)")
        P("{")
        P("    PAINTSTRUCT ps;\n")
        P("    switch (message) {")
        for h in data["handlers"]:
            calls = ", ".join(label(c) for c in h["syscalls"]) or "(internal only)"
            P(f"    case {h['name']}:               /* @0x{h['offset']:04x} */")
            P(f"        /* calls: {calls}"
              + (f"  +{h['internal']} internal" if h['internal'] else "") + " */")
            P(f"        break;")
        P("    default:")
        P("        return DefWindowProc(hWnd, message, wParam, lParam);")
        P("    }")
        P("    return 0L;")
        P("}\n")

    # remaining exports: dialog procs / callbacks
    for ordv, nm in dialog_exports(ne):
        if ordv in done: continue
        P(f"/* {nm} (export @{ordv}) — dialog/callback procedure (stub) */")
        P(f"BOOL FAR PASCAL {nm.title().replace('_','')}(HWND hDlg, unsigned message,")
        P(f"                             WORD wParam, LONG lParam);\n")

if __name__ == "__main__":
    emit(sys.argv[1])
