#!/usr/bin/env python3
"""
sdkcheck.py — audit every recovered name against the 1.01 SDK's own WINDOWS.H.

The SDK header (refs/sdk101/d06/WINDOWS.H) is not a cross-check like winevdm:
it is the header this binary was compiled against, so its prototypes are the
ground truth for the PUBLIC API. That makes a mechanical audit possible:

    for every ordinal ORDINALS.md names, does WINDOWS.H declare that name,
    and does its argument list match the `retf N` the binary actually uses?

A PASCAL callee cleans its own stack, so `retf N` states the argument bytes
exactly; the prototype states the same thing in types. They must agree, and
where they do not, one of the two is wrong about that ordinal.

Argument sizes: a word-sized type (int, WORD, HWND, HANDLE, BOOL, short) is one
push; a long/pointer type (LONG, DWORD, LPSTR, FARPROC, LPRECT...) is two.

    python sdkcheck.py            audit every named ordinal
    python sdkcheck.py --missing  list SDK APIs the project has NOT yet found
    python sdkcheck.py NAME       show one prototype

Argument counts come from ghidra_export.json (Ghidra sets them from each
function's RETF); run the Ghidra pipeline first if it is stale.
"""
import json, os, re, sys
import trace

HDR = "refs/sdk101/d06/WINDOWS.H"
EXPORT = "ghidra_export.json"

# Two pushes, per WINDOWS.H's own typedefs: every FAR pointer (the LP* family
# and FARPROC), every 32-bit scalar, and POINT — which is a struct of two ints,
# passed as one DWORD. Getting POINT and FARPROC wrong is what made PtInRect
# and DialogBox look like they disagreed with the binary when they do not.
DWORD_TYPES = {"long", "LONG", "DWORD", "COLORREF", "POINT"}


def parse_header(path=HDR):
    """{name: (return type, [argument types])} for every FAR PASCAL prototype."""
    src = open(path, encoding="latin-1", errors="replace").read()
    src = re.sub(r"/\*.*?\*/", " ", src, flags=re.S)
    protos = {}
    for m in re.finditer(r"([A-Za-z_][\w \t\*]*?)\bFAR\s+PASCAL\s+(\w+)\s*\(([^)]*)\)",
                         src):
        ret, name, args = m.group(1).strip(), m.group(2), m.group(3).strip()
        if not name or name.startswith("*"):
            continue
        parts = [a.strip() for a in args.split(",") if a.strip()]
        if parts in ([], ["void"]):
            parts = []
        protos[name] = (ret, parts)
    return protos


def words(argtype):
    """How many pushes this argument costs."""
    t = argtype.strip()
    head = t.split()[0] if t.split() else t
    if (head in DWORD_TYPES or head.startswith("LP") or head.endswith("PROC")
            or "*" in t or "FAR" in t):
        return 2
    return 1


def main():
    if not os.path.exists(HDR):
        print(f"{HDR} not found — extract the SDK first:\n"
              f"  7z x 'refs/Microsoft Windows 1.01  SDK.7z' -orefs/sdk101\n"
              f"  python fat12.py 'refs/sdk101/.../DISK06.IMG' refs/sdk101/d06")
        sys.exit(1)
    protos = parse_header()
    args = [a for a in sys.argv[1:] if not a.startswith("--")]

    if args:
        for nm in args:
            if nm in protos:
                r, p = protos[nm]
                print(f"{r} FAR PASCAL {nm}({', '.join(p) or 'void'})   "
                      f"= {sum(words(x) for x in p)} pushes")
            else:
                near = [k for k in protos if nm.lower() in k.lower()][:6]
                print(f"{nm}: not declared in WINDOWS.H" +
                      (f"  (near: {near})" if near else ""))
        return

    ordinals = trace.load_ordinals("ORDINALS.md")
    named = {k: v for k, v in ordinals.items()
             if v and re.match(r"^[A-Za-z_]\w*$", v)}

    if "--missing" in sys.argv:
        have = set(named.values())
        missing = sorted(k for k in protos if k not in have)
        print(f"# {len(missing)} of the {len(protos)} APIs WINDOWS.H declares "
              f"are not yet named in ORDINALS.md")
        for k in missing:
            r, p = protos[k]
            print(f"  {k:28s} {sum(words(x) for x in p)} pushes  "
                  f"({', '.join(p) or 'void'})")
        return

    ghp = {}
    if os.path.exists(EXPORT):
        for f in json.load(open(EXPORT, encoding="utf-8")):
            ghp.setdefault(f["name"], f["params"])

    ok = mismatch = undeclared = unknown = 0
    print(f"{'ordinal':12s} {'name':26s} {'binary':>7s} {'SDK':>5s}")
    for key, nm in sorted(named.items()):
        if nm not in protos:
            undeclared += 1
            continue
        want = sum(words(x) for x in protos[nm][1])
        got = ghp.get(nm)
        # Ghidra records 0 arguments for the shared-prologue bodies, which own
        # no `retf N` of their own. That is "not known", not "takes none", so it
        # must not be reported as a disagreement.
        if not got:
            unknown += 1
            continue
        if got == want:
            ok += 1
        else:
            mismatch += 1
            print(f"{key:12s} {nm:26s} {got:7d} {want:5d}   <-- DISAGREES")
    print(f"\n{ok} names agree with WINDOWS.H on argument count")
    print(f"{mismatch} disagree (each is a real question about that ordinal)")
    print(f"{undeclared} are not declared in WINDOWS.H — internal ordinals, or "
          f"names this project coined")
    print(f"{unknown} have no RETF of their own (shared prologue) so the "
          f"binary states no count")


if __name__ == "__main__":
    main()
