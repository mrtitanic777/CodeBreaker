"""watcom.py -- compiling and linking reconstructed C with OpenWatcom.

The pieces a whole-module build needs: where the tools are and the environment they run in,
the flags a reconstruction is compiled with, how a module's imports are declared and linked
so calls reach the same DLL functions the original calls, and a check that catches calls
whose argument count disagrees with the callee's definition before the compiler does.
"""
import os
import re
import sys
import subprocess
from codebreaker.base import corpus
from codebreaker.base import period_tools as toolchain
from codebreaker.formats.pe import open_pe
from codebreaker.emulate import trace32 as tracer
from codebreaker.recover import interproc
from codebreaker.emulate import provision as envprov
from codebreaker.verify import oracle as verify95
from codebreaker.targets.win95 import origin as OT


def _ow():
    return {"root": toolchain.openwatcom_root(), "wcc": toolchain.ow_cc("pe"),
            "wlink": toolchain.ow_tool("wlink"), "inc": toolchain.ow_include("pe")}


def _scratch(*parts):
    """Build scratch lives in the temp dir, not beside the source (it grew to many thousands of
    files under src/targets/win95 and slowed every search of the repo)."""
    import tempfile
    return os.path.join(tempfile.gettempdir(), "codebreaker", "bodyverify", *parts)


# -ecc makes __cdecl the DEFAULT calling convention. Without it wcc386 passes arguments in registers
# (its __watcall), so a rebuilt function read its parameters from EAX/EDX/EBX/ECX while the oracle --
# like every caller of the original -- puts them on the stack. A rebuilt function that depended on an
# argument therefore never saw it, and failed verification however good the reconstruction was. A
# convention written on a signature (__stdcall, __fastcall) still wins.
_CFLAGS = ["-bt=nt", "-zq", "-w0", "-s", "-zastd=c99", "-ecc"]


# The compiler emits calls to its own runtime helpers for some constructs: __TryInit/__TryUnwind/
# __Except for __try/__except, and 64-bit arithmetic and floating-point helpers. They live in the
# C runtime library; without it every body using such a construct failed to link. Local stubs
# for a body's callees are defined in the object and still take precedence.
_RTL = "library clib3r.lib\n"


# Where a rebuilt DLL is based. The linker's default (0x400000) is exactly where ordinary computed
# values fall -- the sum of two of the oracle's scratch pointers, say -- and a value inside the
# rebuilt image is taken for "an address of this build's own data", which the original cannot
# have: a correct body was then reported as passing a different argument. Up here nothing collides.
IMAGE_BASE = 0x6E000000


_BASE = "option offset=0x%X\n" % IMAGE_BASE


def _ow_env(ow):
    return toolchain.ow_env("pe")


# ---------------------------------------------------------------------------------------------
# Real imports. A rebuilt body must call the retail module's imported functions THROUGH AN
# IMPORT, not through a local stand-in. The oracle records an external call when execution
# lands on an import; a local `int HeapAlloc(){return 0;}` is invisible to it. With stand-ins
# the rebuilt side never called anything, so any body whose retail path reached an API could
# not match -- most of the code that matters. (The 16-bit lane had the same flaw.)
# ---------------------------------------------------------------------------------------------
_IMPORT_MAPS = {}


def import_map(module):
    """{name: dll} for the functions a retail PE imports by name (the first DLL wins)."""
    if module not in _IMPORT_MAPS:
        out = {}
        try:
            for dll, name in open_pe(module).imports().values():
                if name and re.fullmatch(r"[A-Za-z_]\w*", name):
                    out.setdefault(name, os.path.splitext(dll)[0].upper())
        except Exception:
            pass
        _IMPORT_MAPS[module] = out
    return _IMPORT_MAPS[module]


def _omf_externs(data):
    """External symbol names an OMF object refers to (EXTDEF records, 0x8C)."""
    out, i, n = [], 0, len(data)
    while i + 3 <= n:
        rectype = data[i]
        end = i + 3 + (data[i + 1] | (data[i + 2] << 8))
        if end > n:
            break
        if rectype == 0x8C:
            q = i + 3
            while q < end - 1:                           # end-1 is the record checksum
                ln = data[q]
                out.append(data[q + 1:q + 1 + ln].decode("latin1"))
                q += 1 + ln
                q += 2 if (q < end and data[q] & 0x80) else 1     # type index
        i = end
    return out


def _import_lines(obj, names, imports):
    """wlink `import` directives binding each imported name's decorated symbol to DLL.name."""
    ext = _omf_externs(obj)
    out = []
    for n in sorted(names):
        for sym in ext:
            if sym.lstrip("_").split("@")[0] == n:
                out.append("import '%s' '%s'.'%s'\n" % (sym, imports[n], n))
                break
    return "".join(out)


def _param_count(c_text, fname):
    """Number of parameters `fname` is DEFINED with in c_text, or None (not found / `()`)."""
    m = re.search(r"(?m)^[^\n;{}#/]*\b%s\s*\(([^;{}]*)\)\s*\{" % re.escape(fname), c_text)
    if not m:
        return None
    inner = m.group(1).strip()
    if inner == "":
        return None                                   # K&R `()`: takes anything
    return 0 if inner == "void" else inner.count(",") + 1


def _call_arg_counts(c_text, names):
    """[(callee, argument count)] for every call in c_text to one of `names`."""
    out = []
    for m in re.finditer(r"\b([A-Za-z_]\w*)\s*\(", c_text):
        if m.group(1) not in names:
            continue
        i, depth, n, any_tok = m.end(), 1, 0, False
        while i < len(c_text) and depth:
            ch = c_text[i]
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
            elif ch == "," and depth == 1:
                n += 1
            elif not ch.isspace():
                any_tok = True
            i += 1
        out.append((m.group(1), n + 1 if any_tok else 0))
    return out


def arity_mismatches(items):
    """{caller: reason} for bodies that call another body of the batch with a different number of
    arguments than it is defined with. The compiler rejects these, but reports only the first bad
    call of each callee per run -- found here, they are all known at once."""
    defined = {}
    for fn, c in items:
        n = _param_count(c, fn)
        if n is not None:
            defined[fn] = n
    bad = {}
    for fn, c in items:
        body = c[c.find("{"):] if "{" in c else c
        for callee, n in _call_arg_counts(body, defined):
            if callee != fn and n != defined[callee]:
                bad[fn] = "calls %s with %d argument(s); it is defined with %d" % (callee, n, defined[callee])
                break
    return bad
