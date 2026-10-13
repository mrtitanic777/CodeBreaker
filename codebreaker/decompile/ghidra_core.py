#!/usr/bin/env python3
r"""ghidra_core.py -- the embedded Ghidra decompiler core (native/ghidra_core), fed CodeBreaker's facts.

`cbdecomp` is the Ghidra decompiler core linked directly, with no Java and no Ghidra application.
Left alone it guesses what it cannot see from one function. This module tells it what CodeBreaker
has read from the binaries:

  * every function's start and name, and the bytes of arguments it removes on return (its own
    `ret N`) -- which fixes its calling convention and argument count;
  * every import-table slot, the function it holds, and the same for that function (from the DLL
    that exports it, checked against the Win32 headers -- apiarity).

    facts(module_path)        -> path of the facts file for a module (written on demand)
    decompile(module_path)    -> ({name: C text}, [names in address order], {va: name})
"""
import os
import re
import subprocess

from codebreaker.base import paths

REV = 13                                    # bump when what is fed to / taken from the core changes

# The core prints a function in the module's default convention (stdcall) without saying so, and
# spells the others out. Compile with stdcall as the default (-ecd; -ecs is __syscall) so the text
# means what it says.
CFLAGS = ["-bt=nt", "-zq", "-w0", "-s", "-zastd=c99", "-ecd"]

_TYPES = """#pragma pack(1)
typedef signed char int1; typedef short int2; typedef int int4; typedef long long int8;
typedef unsigned char uint1; typedef unsigned short uint2; typedef unsigned int uint4;
typedef unsigned long long uint8;
typedef unsigned char xunknown1; typedef unsigned short xunknown2; typedef unsigned int xunknown4;
typedef unsigned long long xunknown8;
typedef unsigned char undefined; typedef unsigned char undefined1; typedef unsigned short undefined2;
typedef unsigned int undefined4; typedef unsigned long long undefined8;
typedef unsigned char bool; typedef float float4; typedef double float8; typedef long double float10;
typedef int code();
typedef unsigned int unkbyte3; typedef unsigned int unkint3; typedef unsigned int unkuint3;
#define true 1
#define false 0
"""

# The core's own operators for things C has no syntax for: joining and splitting values by
# bytes, widening, and the carry/overflow conditions of an addition or subtraction.
_HELPERS = """
#define CONCAT11(a,b) ((uint2)(((uint2)(uint1)(a) << 8) | (uint1)(b)))
#define CONCAT12(a,b) ((uint4)(((uint4)(uint1)(a) << 16) | (uint2)(b)))
#define CONCAT13(a,b) ((uint4)(((uint4)(uint1)(a) << 24) | ((uint4)(b) & 0xFFFFFF)))
#define CONCAT21(a,b) ((uint4)(((uint4)(uint2)(a) << 8) | (uint1)(b)))
#define CONCAT22(a,b) ((uint4)(((uint4)(uint2)(a) << 16) | (uint2)(b)))
#define CONCAT31(a,b) ((uint4)(((uint4)(a) << 8) | (uint1)(b)))
#define CONCAT44(a,b) ((uint8)(((uint8)(uint4)(a) << 32) | (uint4)(b)))
#define SUB21(x,n) ((uint1)((uint2)(x) >> (8 * (n))))
#define SUB41(x,n) ((uint1)((uint4)(x) >> (8 * (n))))
#define SUB42(x,n) ((uint2)((uint4)(x) >> (8 * (n))))
#define SUB43(x,n) ((uint4)((uint4)(x) >> (8 * (n))) & 0xFFFFFF)
#define SUB81(x,n) ((uint1)((uint8)(x) >> (8 * (n))))
#define SUB82(x,n) ((uint2)((uint8)(x) >> (8 * (n))))
#define SUB84(x,n) ((uint4)((uint8)(x) >> (8 * (n))))
#define ZEXT12(x) ((uint2)(uint1)(x))
#define ZEXT14(x) ((uint4)(uint1)(x))
#define ZEXT24(x) ((uint4)(uint2)(x))
#define ZEXT48(x) ((uint8)(uint4)(x))
#define SEXT12(x) ((int2)(int1)(x))
#define SEXT14(x) ((int4)(int1)(x))
#define SEXT24(x) ((int4)(int2)(x))
#define SEXT48(x) ((int8)(int4)(x))
#define CARRY1(a,b) ((uint1)(a) > (uint1)(0xFF - (uint1)(b)))
#define CARRY2(a,b) ((uint2)(a) > (uint2)(0xFFFF - (uint2)(b)))
#define CARRY4(a,b) ((uint4)(a) > 0xFFFFFFFFu - (uint4)(b))
#define SCARRY4(a,b) ((((int4)(a) ^ (int4)((uint4)(a) + (uint4)(b))) & ((int4)(b) ^ (int4)((uint4)(a) + (uint4)(b)))) < 0)
#define SBORROW4(a,b) ((((int4)(a) ^ (int4)(b)) & ((int4)(a) ^ (int4)((uint4)(a) - (uint4)(b)))) < 0)
#define SCARRY2(a,b) ((int2)(((int2)(a) ^ (int2)((a) + (b))) & ((int2)(b) ^ (int2)((a) + (b)))) < 0)
#define SBORROW2(a,b) ((int2)(((int2)(a) ^ (int2)(b)) & ((int2)(a) ^ (int2)((a) - (b)))) < 0)
#define SCARRY1(a,b) ((int1)(((int1)(a) ^ (int1)((a) + (b))) & ((int1)(b) ^ (int1)((a) + (b)))) < 0)
#define SBORROW1(a,b) ((int1)(((int1)(a) ^ (int1)(b)) & ((int1)(a) ^ (int1)((a) - (b)))) < 0)
"""
_BY_SIZE = {1: "uint1", 2: "uint2", 4: "uint4", 8: "uint8"}


def _exe():
    from codebreaker.base import settings
    return os.path.join(settings.get("ghidra_core"), "cbdecomp.exe")


def _specs():
    return os.path.join(paths.repo_root(), "native", "ghidra_core", "languages")


def available():
    return os.path.isfile(_exe()) and os.path.isdir(_specs())


def _ident(name):
    return re.sub(r"\W", "_", name)


def function_names(module_path, funcs):
    """{va: name} -- an export's own name, else sub_<rva>. A name also used by an import of this
    module stays with the import (the call has to bind to it)."""
    from codebreaker.formats.pe import open_pe
    from codebreaker.verify import body32 as B
    pe = open_pe(module_path)
    imported = set(B.import_map(module_path))
    by_rva = {}
    for nm, rva in sorted(pe.export_addrs().items()):
        if re.fullmatch(r"[A-Za-z_]\w*", nm) and nm not in imported:
            by_rva.setdefault(rva, nm)
    return {va: by_rva.get(va - pe.image_base, "sub_%06X" % (va - pe.image_base)) for va in funcs}


def facts(module_path, funcs, outdir):
    """Write the facts file for `funcs` (VAs) of a module; return (path, {va: name})."""
    from codebreaker.emulate import api_stack as apiarity
    from codebreaker.formats.pe import open_pe
    pe = open_pe(module_path)
    names = function_names(module_path, funcs)
    mod = apiarity._Mod(module_path)
    lines = []
    for va in funcs:
        try:
            pop = apiarity._scan(mod, va - pe.image_base, 4)
        except Exception:
            pop = None
        lines.append("F\t%x\t%s\t%d" % (va - pe.image_base, names[va], -1 if pop is None else pop))
    for slot, (dll, name) in sorted(pe.imports().items()):
        stem = os.path.splitext(dll)[0].upper()
        if not name or name.startswith("#"):
            lines.append("I\t%x\t%s\t-1" % (slot - pe.image_base, _ident("%s_ord_%s" % (stem, (name or "#0")[1:]))))
            continue
        pop = apiarity.stack_bytes(stem, name)
        lines.append("I\t%x\t%s\t%d" % (slot - pe.image_base, _ident(name), -1 if pop is None else pop))
    os.makedirs(outdir, exist_ok=True)
    path = os.path.join(outdir, "facts.txt")
    with open(path, "w", newline="\n") as f:
        f.write("\n".join(lines) + "\n")
    return path, names


def decompile(module_path, funcs, outdir, timeout=3600):
    """Decompile `funcs` with the embedded core. -> ({name: text}, [names], {va: name}, stderr)."""
    from codebreaker.formats.pe import open_pe
    fpath, names = facts(module_path, funcs, outdir)
    env = dict(os.environ, CB_GHIDRA_SPECS=_specs())
    r = subprocess.run([_exe(), module_path, "--facts", fpath], capture_output=True, text=True,
                       errors="replace", timeout=timeout, env=env)
    base = open_pe(module_path).image_base
    out, order, by_va = {}, [], {}
    for m in re.finditer(r"(?ms)^/\*@@ (\S+) ([0-9a-f]+) \*/\n(.*?)(?=^/\*@@ |\Z)", r.stdout or ""):
        name, rva, text = m.group(1), int(m.group(2), 16), m.group(3).strip()
        if not text or text.startswith("/* decompile failed"):
            continue
        if name not in out:
            out[name] = text
            order.append(name)
            by_va[base + rva] = name
    return out, order, by_va, (r.stderr or "")[-2000:]


_LOCAL = re.compile(r"(?m)^  ([A-Za-z_][\w ]*?(?:\s*\*+\s*|\s))([a-z]+Stack_([0-9a-f]+))(?: \[(\d+)\])?;[ \t]*$")
_SIZEOF = {"int1": 1, "uint1": 1, "xunknown1": 1, "char": 1, "bool": 1, "undefined": 1, "undefined1": 1,
           "int2": 2, "uint2": 2, "xunknown2": 2, "undefined2": 2,
           "int4": 4, "uint4": 4, "xunknown4": 4, "undefined4": 4, "float4": 4,
           "int8": 8, "uint8": 8, "xunknown8": 8, "undefined8": 8, "float8": 8, "float10": 10}


def _keep_frame(t):
    """Keep a function's stack locals where the original had them, relative to one another.

    The original often builds a small structure in its frame and passes its address
    (`mov [ebp-18h], hwnd / mov [ebp-14h], id / lea edx, [ebp-18h] / push edx`). The core
    recovers the fields as separate locals; a C compiler then places them wherever it likes, and
    the callee, reading through the pointer, finds something else. When any stack local's address
    can escape, the locals are declared as the fields of one frame structure at their original
    offsets, so every address taken still points at what the original put there."""
    locs = [m for m in _LOCAL.finditer(t) if m.group(1).strip() not in ("return", "goto")]
    if not locs:
        return t
    names = [m.group(2) for m in locs]
    if not (any(m.group(4) for m in locs) or re.search(r"&\s*(?:%s)\b" % "|".join(names), t)):
        return t
    items = []
    for m in locs:
        ty = " ".join(m.group(1).split())
        size = 4 if ty.endswith("*") else _SIZEOF.get(ty)
        if size is None:
            return t                                       # a type whose size we do not know
        count = int(m.group(4)) if m.group(4) else 1
        items.append((int(m.group(3), 16), size * count, ty, m.group(2), m.group(4)))
    items.sort(reverse=True)                               # largest offset = lowest address, first
    fields, pos = [], None
    for off, size, ty, name, arr in items:
        addr = -off
        if pos is not None:
            gap = addr - pos
            if gap < 0:
                return t                                   # overlapping locals: leave as they are
            if gap:
                fields.append("uint1 _gap_%x[%d];" % (off, gap))
        fields.append("%s %s%s;" % (ty, name, "[%s]" % arr if arr else ""))
        pos = addr + size
    if pos is not None and pos < 0:
        # the original's frame runs on to its return address: a callee handed the address of the
        # last local may fill a whole structure there (a RECT the core saw only two fields of)
        fields.append("uint1 _rest[%d];" % -pos)
    decl = "  struct { %s } _f;   /* locals at their offsets in the original's frame */" % " ".join(fields)
    first = [True]

    def strip(_m):
        if first[0]:
            first[0] = False
            return "\x00FRAME\x00"
        return ""
    t = _LOCAL.sub(lambda m: m.group(0) if m.group(1).strip() in ("return", "goto") else strip(m), t)
    for name in sorted(names, key=len, reverse=True):
        t = re.sub(r"(?<![\w.])%s\b" % re.escape(name), "_f." + name, t)
    return t.replace("\x00FRAME\x00", decl)


_PTR_DECL = re.compile(r"(?m)^  [A-Za-z_][\w ]*?\*+\s*(\w+)(?: \[\d+\])?;[ \t]*$")
_OPERAND = r"(?:\(u?int4\))?(?:_f\.)?[A-Za-z_]\w*"
_COMPARE = re.compile(r"(?<![\w.>)\]*&])(%s) (<=|>=|<|>|==|!=) (%s)(?![\w.(\[]|->)" % (_OPERAND, _OPERAND))


def _compare_as_numbers(t):
    """The core compares a pointer with a plain number freely (`if (pcVar2 < (uint4)a1)`); C does
    not allow it. Where exactly one side is a pointer variable, compare its value as a number."""
    ptrs = set(_PTR_DECL.findall(t))
    for ln in t.split("\n"):
        if "struct {" in ln and "} _f;" in ln:
            ptrs.update(re.findall(r"\*+\s*(\w+)(?:\[\d+\])?;", ln))
    if not ptrs:
        return t

    def is_ptr(x):
        return "(" not in x and x.split(".")[-1] in ptrs

    def fix(m):
        l, op, r = m.groups()
        if is_ptr(l) == is_ptr(r) or "0" in (l, r):
            return m.group(0)
        return "%s %s %s" % ("(uint4)" + l if is_ptr(l) else l, op, "(uint4)" + r if is_ptr(r) else r)
    return _COMPARE.sub(fix, t)


def adapt(module_path, funcs, outdir):
    """-> (preamble, {name: text}, [names], {va: name}, [import names used]) for ghidra_engine:
    the core's output with its base types defined, its globals bound to the original's addresses,
    and every function and import declared before use with its real convention."""
    from codebreaker.emulate import api_stack as apiarity
    from codebreaker.verify import body32 as B
    out, order, by_va, _err = decompile(module_path, funcs, outdir)
    imports = B.import_map(module_path)
    sizes = {}
    for n in order:
        body = []
        for ln in out[n].split("\n"):
            m = re.match(r"/\*@G ([0-9a-f]+) (\d+) \*/$", ln)
            if m:
                sizes.setdefault(int(m.group(1), 16), set()).add(int(m.group(2)))
            else:
                body.append(ln)
        t = "\n".join(body).strip()
        # `x._1_2_` is the core's spelling of "2 bytes of x starting at byte 1"
        t = re.sub(r"\b([A-Za-z_]\w*)\._(\d+)_(\d+)_",
                   lambda mm: "(*(%s *)((char *)&%s + %s))" % (_BY_SIZE.get(int(mm.group(3)), "uint4"),
                                                               mm.group(1), mm.group(2)), t)
        t = _compare_as_numbers(_keep_frame(t))
        # a function's address stored in a pointer the core typed loosely
        t = re.sub(r"(?m)(= )((?:%s);)$" % "|".join(re.escape(x) for x in order), r"\1(code *)\2", t)
        # every function returns a value in EAX whether or not the core saw it used
        out[n] = re.sub(r"\Avoid(\s)", r"int\1", t)
    text = "\n".join(out.values())
    macros = []
    # a global the code calls through holds a function pointer, whatever else it is compared with
    called_through = set(re.findall(r"\(\*([a-z]+Ram[0-9a-f]{8})\)\s*\(", text))
    for pre, addr in sorted(set(re.findall(r"\b([a-z]+)Ram([0-9a-f]{8})\b", text))):
        szs = sizes.get(int(addr, 16)) or {4}
        size = min(szs) if len(szs) > 1 and pre[0] in "cb" else max(s for s in szs if s <= 8) if any(s <= 8 for s in szs) else 4
        ty = "code *" if (pre == "pc" or "%sRam%s" % (pre, addr) in called_through) else (
            "int%d" % size if pre[0] == "i" else _BY_SIZE.get(size, "uint4"))
        macros.append("#define %sRam%s (*(%s *)0x%s)" % (pre, addr, ty, addr))
    called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", text))
    used = sorted(n for n in called if n in imports and n not in out)
    decls = []
    for n in used:
        conv = "__cdecl" if apiarity.stack_bytes(imports[n], n) == 0 else "__stdcall"
        decls.append("int %s %s();" % (conv, n))
    for n in order:                                    # each function, declared before any use
        head = out[n].split("{", 1)[0].strip()
        if head.endswith(")"):
            decls.append(" ".join(head.split()) + ";")
    preamble = _TYPES + _HELPERS + "\n".join(decls) + "\n" + "\n".join(macros) + "\n"
    return preamble, out, order, by_va, used
