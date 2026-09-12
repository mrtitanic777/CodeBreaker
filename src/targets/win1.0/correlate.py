#!/usr/bin/env python3
"""
correlate.py — Derive Windows 1.0 API names by cross-referencing many apps.

Independently written apps handle the same window message the same canonical way:
every WM_PAINT handler calls BeginPaint/EndPaint, every WM_DESTROY posts a quit,
mouse-capture is taken on button-down and released on button-up, and so on.

By tallying, across all apps, which system ordinal is called under which message,
the API names fall out of consensus. An ordinal used by N independent apps in a
single message context is almost certainly that message's canonical function.
"""
import glob, os, sys
from collections import defaultdict, Counter
from wndproc import analyze

# Which exported ordinal is each app's WndProc? Nearly all export it @1, but a
# few export additional procs; we scan the obvious window-proc export names.
def wndproc_ordinals(path):
    from src.disasm.disasm import open_ne
    ne = open_ne(path)
    res = ne.resident_names()
    nonres = ne.nonresident_names()
    ords = []
    for n,o in res[1:] + nonres[1:]:
        if "WNDPROC" in n.upper() or n.upper().endswith("PROC"):
            ords.append((o,n))
    return ords or [(1,"@1")]

# Canonical expectation for a handful of messages, used only to *label* the
# consensus winner (the evidence itself is the cross-app agreement).
EXPECT = {
    (0x000f,"USER"): "BeginPaint / EndPaint",
    (0x0002,"USER"): "PostQuitMessage",
    (0x0201,"USER"): "SetCapture",
    (0x0202,"USER"): "ReleaseCapture",
}

def main(directory):
    apps = sorted(glob.glob(os.path.join(directory,"*.EXE")))
    # message -> ordinal -> set(apps)
    ctx = defaultdict(lambda: defaultdict(set))
    per_app = {}
    for path in apps:
        name = os.path.basename(path)
        try:
            for ordv,_ in wndproc_ordinals(path):
                data = analyze(path, ordv, quiet=True)
                for h in data["handlers"]:
                    for call in h["syscalls"]:
                        ctx[h["msg"]][call].add(name)
        except Exception as ex:
            continue

    from wndproc import WM
    print("SYSTEM CALLS BY WINDOW MESSAGE (consensus across apps)")
    print("="*70)
    for msg in sorted(ctx):
        wm = WM.get(msg, f"msg_0x{msg:04x}")
        # rank ordinals by how many distinct apps use them in this message
        ranked = sorted(ctx[msg].items(), key=lambda kv:-len(kv[1]))
        top = [(o,len(a)) for o,a in ranked if len(a)>=2]
        if not top:
            continue
        print(f"\n{wm} (0x{msg:04x}):")
        for ordn,napps in top[:8]:
            print(f"    {ordn:<12} used by {napps} apps")

    print("\n\nHIGH-CONFIDENCE ORDINAL NAMES (>=3 apps agree in one message context)")
    print("="*70)
    derived = {}
    for msg in ctx:
        for ordn,appset in ctx[msg].items():
            if len(appset) >= 3 and ordn.startswith(("USER.","GDI.","KERNEL.")):
                derived.setdefault(ordn, []).append((msg,len(appset)))
    # Attach canonical labels where the message context is unambiguous
    for ordn in sorted(derived):
        best_msg,_ = max(derived[ordn], key=lambda x:x[1])
        mod = ordn.split(".")[0]
        label = EXPECT.get((best_msg,mod),"")
        from wndproc import WM as _WM
        ctxs = ", ".join(f"{_WM.get(m,hex(m))}×{n}" for m,n in sorted(derived[ordn]))
        tag = f"  = {label}" if label else ""
        print(f"    {ordn:<12} [{ctxs}]{tag}")

if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv)>1 else "files/WF")
