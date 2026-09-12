#!/usr/bin/env python3
r"""sigfactory.py -- the SIGNATURE FACTORY (win95 32-bit PE).

Manufacture a function's C/C++ signature from evidence, so the emitters stop
typing every parameter `int a0` and guessing conventions. Two evidence sources,
in trust order (never fabricate -- decline to 'int'/inferred when nothing is
known):

  1. SDK PROTOTYPE  -- if the function is a named export the period SDK headers
     describe, take its real return type + typed, named parameters (symbol/symdb,
     3184 prototypes). Ground-truth types.
  2. INFERRED       -- else synthesize from the introspection descriptor: the
     calling convention and arg count are already evidence (ret N / arg slots /
     ecx-as-this), the parameter types default to int, and the function's
     signedness is carried as a hint. Honest scaffolding, not a claim of types.

Built ON introspect.describe; consumed BY the emitters (emit_c/emit_cpp), reflected
delegation, and the reconstruction facades.

    python sigfactory.py MODULE.dll[:RVA]     # one signature, or a sample per source
"""
import os
import re
import sys
from dataclasses import dataclass, field

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import pe32util as H
import introspect

_CONV_KW = {"stdcall": "__stdcall", "cdecl": "__cdecl", "thiscall": "__thiscall",
            "stdcall0": "__stdcall", "unknown": "__stdcall"}

_symdb = None


def _sdb():
    """Lazily load symbol/symdb (the SDK-prototype DB). Best-effort: if it is not
    importable/populated, the factory just uses the inferred path."""
    global _symdb
    if _symdb is None:
        try:
            from src.engines.symbol import symdb
            _symdb = symdb
        except Exception:
            _symdb = False
    return _symdb


@dataclass
class Signature:
    name: str
    ret: str                          # C return type
    convention: str                   # __stdcall | __cdecl | __thiscall
    params: list = field(default_factory=list)   # [(ctype, pname)]  (this NOT included)
    source: str = "inferred"          # sdk | inferred
    signedness: str = "unknown"
    thiscall: bool = False

    def render_params(self):
        return "void" if not self.params else ", ".join("%s %s" % (t, n) for t, n in self.params)

    def render_decl(self):
        """A C declaration line: `RET __conv name(params);`."""
        return "%s %s %s(%s);" % (self.ret, self.convention, self.name, self.render_params())


def _split_proto(proto):
    """Parse 'RET name(t1 a1, t2 a2, ...)' -> (ret, name, [(type,name), ...]) or None.
    Types keep their spelling; a bare type with no name gets a synthesized aN."""
    m = re.match(r"\s*(.+?)\b(\w+)\s*\((.*)\)\s*;?\s*$", proto, re.S)
    if not m:
        return None
    ret, name, args = m.group(1).strip(), m.group(2), m.group(3).strip()
    if args in ("", "void", "VOID"):
        return ret, name, []
    params = []
    for k, a in enumerate(args.split(",")):
        a = a.strip()
        if not a:
            continue
        pm = re.match(r"^(.*?)(\w+)$", a)               # split trailing identifier as the name
        if pm and pm.group(1).strip() and not a.endswith(("*", " ")):
            params.append((pm.group(1).strip(), pm.group(2)))
        else:
            params.append((a.rstrip("*").strip() + ("*" if a.endswith("*") else ""), "a%d" % k))
    return ret, name, params


def signature(module, rva, desc=None):
    """Return a Signature for the function at `rva` (SDK prototype if known, else
    inferred from the descriptor)."""
    d = desc or introspect.describe(module, rva)
    conv = _CONV_KW.get(d.convention, "__stdcall")
    nm = d.name or ("sub_%05x" % d.rva)
    # (1) SDK prototype for a named export
    sdb = _sdb()
    if d.name and sdb:
        try:
            proto = sdb.signature(d.name)
        except Exception:
            proto = None
        if proto:
            parsed = _split_proto(proto)
            if parsed:
                ret, _pn, params = parsed
                return Signature(name=nm, ret=ret, convention=conv, params=params,
                                 source="sdk", signedness=d.signedness, thiscall=d.thiscall)
    # (2) inferred from the descriptor -- convention + argc are evidence; types default to int
    n = max(d.argc, 0)
    params = [("int", "a%d" % i) for i in range(n)]
    ret = "void *" if d.shape == "method" and d.retbytes == 0 and not params else "int"
    return Signature(name=nm, ret=ret, convention=conv, params=params,
                     source="inferred", signedness=d.signedness, thiscall=d.thiscall)


def main():
    H.ensure_target()
    arg = sys.argv[1] if len(sys.argv) > 1 else "kernel32.dll"
    if ":" in arg:
        mod, rva = arg.split(":")
        print(signature(mod, int(rva, 0)).render_decl())
        return
    ds = introspect.describe_module(arg)
    sdk = [d for d in ds if d.name]
    print("=== %s : sample signatures ===" % arg)
    ns = ni = 0
    for d in ds:
        sig = signature(arg, d.rva, d)
        if sig.source == "sdk" and ns < 6:
            print("  [sdk]      %s" % sig.render_decl()); ns += 1
        elif sig.source == "inferred" and ni < 6:
            print("  [inferred] %s  (%s%s)" % (sig.render_decl(), d.shape,
                                               ", " + d.signedness if d.signedness != "unknown" else ""))
            ni += 1
        if ns >= 6 and ni >= 6:
            break
    tot = len(ds)
    nsdk = sum(1 for d in ds if signature(arg, d.rva, d).source == "sdk")
    print("  -- %d/%d functions get an SDK-typed signature; the rest are inferred --" % (nsdk, tot))


if __name__ == "__main__":
    main()
