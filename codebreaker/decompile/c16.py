"""c16.py -- make a decompiled 16-bit module into C that a 16-bit compiler accepts and that means
what the original meant.

    from codebreaker.decompile import c16
    m = c16.compilable(module_path)
    m.header, m.texts {name: C}, m.order, m.by_va {address: name}, m.where {address: (seg, off)}
    m.near            names of the near functions
    m.imports         {C name: (module, ordinal)} for the import functions its code calls
    m.wide            names of functions that return four bytes (DX:AX)

What 16-bit code needs said that 32-bit code does not:

  * Two sizes of pointer. The decompiler reports which locals and parameters are four-byte (far)
    pointers; they are declared `__far`, the rest stay near (an offset in the data segment).
  * Stack layout. The decompiler numbers a function's parameters from the return address
    outward; a compiler's `__stdcall` pushes them in that same order and has the callee remove
    them, so its conventions are mapped onto `__far __stdcall` / `__near __stdcall`. (Windows'
    own `__pascal` lays the same arguments out in the opposite textual order; the bytes on the
    stack are what must match, and they do.)
  * Globals are offsets in the module's data segment, used through DS as in the original.
  * A function printed without a convention has the module's default one (near, the callee
    removes its arguments); the compiler is told that default (CFLAGS in build/module16.py).
"""
import re

HEADER = """typedef signed char int1; typedef short int2; typedef long int4;
typedef unsigned char uint1; typedef unsigned short uint2; typedef unsigned long uint4;
typedef unsigned char xunknown1; typedef unsigned short xunknown2; typedef unsigned long xunknown4;
typedef unsigned char undefined; typedef unsigned char undefined1; typedef unsigned short undefined2;
typedef unsigned long undefined4; typedef unsigned char bool; typedef unsigned long float4;
typedef int2 code();
#define true 1
#define false 0
#define __stdcall16far __far __stdcall
#define __stdcall16near __near __stdcall
#define __cdecl16far __far __cdecl
#define __cdecl16near __near __cdecl
#define CONCAT11(a,b) ((uint2)(((uint2)(uint1)(a) << 8) | (uint1)(b)))
#define CONCAT22(a,b) ((uint4)(((uint4)(uint2)(a) << 16) | (uint2)(b)))
#define CONCAT13(a,b) ((uint4)(((uint4)(uint1)(a) << 24) | ((uint4)(b) & 0xFFFFFFUL)))
#define CONCAT31(a,b) ((uint4)(((uint4)(a) << 8) | (uint1)(b)))
#define CONCAT12(a,b) ((uint4)(((uint4)(uint1)(a) << 16) | (uint2)(b)))
#define CONCAT21(a,b) ((uint4)(((uint4)(uint2)(a) << 8) | (uint1)(b)))
#define SUB21(x,n) ((uint1)((uint2)(x) >> ((n) * 8)))
#define SUB41(x,n) ((uint1)((uint4)(x) >> ((n) * 8)))
#define SUB42(x,n) ((uint2)((uint4)(x) >> ((n) * 8)))
#define ZEXT12(x) ((uint2)(uint1)(x))
#define ZEXT14(x) ((uint4)(uint1)(x))
#define ZEXT24(x) ((uint4)(uint2)(x))
#define SEXT12(x) ((int2)(int1)(x))
#define SEXT14(x) ((int4)(int1)(x))
#define SEXT24(x) ((int4)(int2)(x))
#define CARRY1(a,b) ((uint1)((uint1)(a) + (uint1)(b)) < (uint1)(a))
#define CARRY2(a,b) ((uint2)((uint2)(a) + (uint2)(b)) < (uint2)(a))
#define CARRY4(a,b) ((uint4)((uint4)(a) + (uint4)(b)) < (uint4)(a))
#define SBORROW1(a,b) ((int1)(((int1)(a) ^ (int1)(b)) & ((int1)(a) ^ (int1)((a) - (b)))) < 0)
#define SBORROW2(a,b) ((int2)(((int2)(a) ^ (int2)(b)) & ((int2)(a) ^ (int2)((a) - (b)))) < 0)
#define SBORROW4(a,b) ((int4)(((int4)(a) ^ (int4)(b)) & ((int4)(a) ^ (int4)((a) - (b)))) < 0)
#define SCARRY1(a,b) ((int1)(((int1)(a) ^ (int1)((a) + (b))) & ((int1)(b) ^ (int1)((a) + (b)))) < 0)
#define SCARRY2(a,b) ((int2)(((int2)(a) ^ (int2)((a) + (b))) & ((int2)(b) ^ (int2)((a) + (b)))) < 0)
#define SCARRY4(a,b) ((int4)(((int4)(a) ^ (int4)((a) + (b))) & ((int4)(b) ^ (int4)((a) + (b)))) < 0)
"""

_FACT = re.compile(r"(?m)^/\*@([GP]) (\S+) (\d+) \*/\n?")
_WIDE = ("xunknown4", "int4", "uint4", "undefined4", "float4")


class Module16(object):
    pass


def _far(text, far):
    """Declare the four-byte pointers `far` and make what is assigned to them far too."""
    if not far:
        return text
    names = "|".join(re.escape(n) for n in sorted(far, key=len, reverse=True))
    star = re.compile(r"(?<![\w*])(\*+)\s*(%s)\b" % names)
    head, brace, body = text.partition("{")
    head = star.sub(r"__far \1\2", head)                      # parameters
    decl = re.compile(r"^\s+[A-Za-z_][\w ]*?\*+\s*(?:%s)(?: \[\d+\])?;\s*$" % names)
    lines = [star.sub(r"__far \1\2", ln) if decl.match(ln) else ln for ln in body.split("\n")]   # locals
    body = "\n".join(lines)
    # `name = (type *)expr` -> `name = (type __far *)expr`
    body = re.sub(r"\b(%s) = \(([A-Za-z_][\w ]*?) ?(\*+)\)" % names, r"\1 = (\2 __far \3)", body)
    return head + brace + body


_SIZED = {1: "uint1", 2: "uint2", 4: "uint4"}


def _pieces(text):
    """`x._2_2_` is the decompiler's spelling of "2 bytes of x starting at byte 2"."""
    return re.sub(r"\b([A-Za-z_]\w*)\._(\d+)_(\d+)_",
                  lambda m: "(*(%s *)((char *)&%s + %s))" % (_SIZED.get(int(m.group(3)), "uint2"),
                                                             m.group(1), m.group(2)), text)


def _open_arguments(text):
    """Write the definition in the older style, `f(a, b) int a; int b; { ... }`.

    A definition written with its argument types in the parentheses also fixes how every later
    call must look, and the decompiler recovers each call on its own evidence: a caller that
    passes three words where the definition takes one far pointer and a word is the same bytes on
    the stack, but not the same C. In the older style the definition says what the function
    takes and each call is compiled as written."""
    head, brace, body = text.partition("{")
    m = re.match(r"(?s)(.*?)\(([^()]*)\)\s*$", head)
    if not m or not brace:
        return text
    params = [x.strip() for x in m.group(2).split(",")] if m.group(2).strip() not in ("", "void") else []
    names = []
    for prm in params:
        nm = re.search(r"([A-Za-z_]\w*)\s*(?:\[\d*\])?$", prm)
        if not nm:
            return text
        names.append(nm.group(1))
    decls = "".join("%s;\n" % prm for prm in params)
    return "%s(%s)\n%s%s%s" % (m.group(1).rstrip(), ",".join(names), decls, brace, body)


def prototype(text):
    """The declaration of the function a text defines (everything before its body)."""
    head = text.split("{", 1)[0]
    head = head[:head.index(")") + 1] if ")" in head else head      # without old-style argument lines
    return " ".join(head.split()) + ";"


def declaration(text):
    """A declaration that fixes the function's convention and result but not its arguments.
    The decompiler works out arguments one call at a time and callers do not always agree with
    the definition; with the argument list left open each call compiles as it was recovered,
    and a call that passes the wrong amount shows up when it is run, not as a compile error
    that hides the whole function."""
    head = " ".join(text.split("{", 1)[0].split())
    return head[:head.index("(")] + "();"


def compilable(module_path, outdir=None):
    from codebreaker.decompile import decompiler
    texts, order, by_va, d = decompiler.decompile16(module_path, outdir)
    m = Module16()
    m.texts, m.order, m.by_va, m.where = {}, [], {}, {}
    m.near, m.wide = set(), set()
    for va in sorted(by_va):
        name = by_va[va]
        raw = texts[name]
        far = {mm.group(2) for mm in _FACT.finditer(raw) if mm.group(1) == "P" and mm.group(3) == "4"}
        text = _pieces(_far(_FACT.sub("", raw).strip(), far))
        if "return" in far:                                  # the function returns a far pointer
            head, brace, body = text.partition("{")
            head = re.sub(r"\*(?=[\s\w]*\b%s\s*\()" % re.escape(name), "__far *", head, count=1)
            text = head + brace + body
            m.wide.add(name)
        head = text.split("{", 1)[0]
        if not ("__stdcall16far" in head or "__cdecl16far" in head):
            m.near.add(name)                                 # near is the default and is not printed
        if head.split()[0] in _WIDE or "*" in head.split(name)[0] and "__far" in head.split(name)[0]:
            m.wide.add(name)
        m.texts[name] = _open_arguments(text)
        m.order.append(name)
        m.by_va[va] = name
        m.where[va] = d.where.get(va)
    body = "\n".join(m.texts.values())
    called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", body))
    m.imports = {}
    for nm in d.imports:
        if nm in called and nm not in m.texts:
            mm = re.fullmatch(r"(.+)_(\d+)", nm)
            if mm:
                m.imports[nm] = (mm.group(1), int(mm.group(2)))
    decls = ["extern uint4 __far __stdcall %s();" % nm for nm in sorted(m.imports)]
    decls += [declaration(m.texts[n]) for n in m.order]
    m.header = HEADER + "\n" + "\n".join(decls) + "\n"
    m.description = d
    return m
