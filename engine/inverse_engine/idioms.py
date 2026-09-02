#!/usr/bin/env python3
r"""idioms.py -- the MSC 6.0a codegen idiom / flag test suite.

A curated set of tiny C snippets compiled under the MSC 6.0a oracle across the flag
matrix, recording the exact _TEXT bytes. This does three jobs:

  1. DOCUMENTS undocumented codegen behavior by example (the `sub sp,2` frame under
     /Ox, the loadds prologue, the sbb/neg boolean, pascal vs cdecl arg cleanup).
  2. Provides the REFERENCE FORMS the per-function flag prober (detect_recipe) anchors
     on -- which flag profile produces which prologue/frame/epilogue.
  3. Seeds the msc6dec rule set and the divergence-pattern KB with ground-truth
     (C construct) -> (byte sequence) mappings, per flag.

    wt inverse idioms                 # print the flag-sensitivity table
    from engine.inverse_engine import idioms; idioms.probe(FLAGS)
"""
import os, sys
_HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(os.path.dirname(_HERE)))
from engine.inverse_engine import msc6_oracle as ora

# the flag axes worth sweeping (medium-model DLL code). /Gsw = loadds; /G2 = 286.
FLAGS = ["/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os", "/AM /G2 /Gsw /Ot",
         "/AM /G2 /Gsw /O1", "/AM /G2 /Gsw /O2", "/AM /G2 /Gsw"]

# (name, C source).  Each F is one function so its _TEXT is the whole snippet.
IDIOMS = [
    ("ret-const",     "int far pascal F(void){ return 1; }"),
    ("ret-param",     "int far pascal F(int a){ return a; }"),
    ("frame-local",   "int far pascal F(void){ int i; i=0; return i; }"),
    ("bool-eq",       "int far pascal F(int a){ return a == 1; }"),
    ("add-params",    "int far pascal F(int a,int b){ return a+b; }"),
    ("far-deref",     "int far pascal F(int far *p){ return *p; }"),
    ("far-field",     "typedef unsigned int WORD; int far pascal F(char far *p){ return *(WORD far*)((char far*)p+4); }"),
    ("cdecl-clean",   "typedef unsigned int WORD; extern WORD far G(); int far pascal F(int a){ return G(a); }"),
    ("loop-count",    "int far pascal F(int n){ int i,s; s=0; for(i=0;i<n;i++) s+=i; return s; }"),
    ("long-shl",      "long far pascal F(long a){ return a << 4; }"),
    ("global-store",  "extern int gW; void far pascal F(void){ gW = -1; }"),
    ("if-else",       "int far pascal F(int a){ if(a) return 1; return 0; }"),
]

_PRO6 = bytes.fromhex("1e589045558bec1e8ed8")     # push ds;pop ax loadds (6.0a)
_EPI6 = bytes.fromhex("4d4d8be51f5d4d")


def _tag(b):
    """A short fingerprint of a snippet's frame/prologue/epilogue shape."""
    t = []
    t.append("pro" if b.startswith(_PRO6) else "PRO?")
    if bytes.fromhex("83ec") in b[:16]:
        t.append("frame")                              # sub sp,N (a stack frame)
    if _EPI6 in b:
        t.append("epi")
    if bytes.fromhex("1bc0f7d8") in b:
        t.append("sbb/neg")                            # boolean idiom
    return ",".join(t)


def probe(flags=None, idioms=None):
    """Compile each idiom under each flag; return {name: {flag: hexbytes}}."""
    flags = flags or FLAGS
    idioms = idioms or IDIOMS
    out = {}
    for name, src in idioms:
        out[name] = {}
        for fl in flags:
            try:
                out[name][fl] = ora.mc6_compile(src, fl).hex()
            except Exception as e:
                out[name][fl] = "ERR"
    return out


def flag_sensitivity(res):
    """For each idiom, how many DISTINCT byte forms across the flag matrix (>1 = the
    construct is flag-sensitive -- the prober must pin the flag before reconstructing)."""
    rows = []
    for name, byflag in res.items():
        forms = {}
        for fl, hx in byflag.items():
            forms.setdefault(hx, []).append(fl.split()[-1])   # group flags by identical output
        rows.append((name, len(forms), forms))
    return rows


if __name__ == "__main__":
    if not ora.available():
        print("oracle not configured"); sys.exit(1)
    res = probe()
    print("MSC 6.0a idiom flag-sensitivity (distinct byte forms across the flag matrix):\n")
    for name, ndistinct, forms in flag_sensitivity(res):
        marker = "  FLAG-SENSITIVE" if ndistinct > 1 else ""
        print("%-14s %d form(s)%s" % (name, ndistinct, marker))
        for hx, fls in forms.items():
            if hx == "ERR":
                print("      ERR under %s" % ",".join(fls)); continue
            print("      [%s] %s" % (",".join(fls), _tag(bytes.fromhex(hx))))
