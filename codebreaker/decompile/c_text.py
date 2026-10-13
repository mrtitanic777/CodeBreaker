"""c_text.py -- make the decompiler's output into C a compiler accepts and that means what the
original meant.

    compilable32(module_path, addresses, workdir)   a 32-bit module: (header, texts, order, by_va, imports)
    compilable16(module_path, workdir)              a 16-bit module: an object with the same, plus what
                                                    16-bit code needs said (near functions, wide results)
    compilable_powerpc(module_path, workdir)        a PowerPC image: the same, plus its imports and
                                                    how many arguments each is called with

The decompiler prints C for reading. To be compiled and to behave as the original it needs its
base types defined, every function and import declared with its real convention, its globals
bound to where the original keeps them, and its stack locals kept where the original had them
whenever an address to one can escape. 16-bit code needs more said: which pointers are far, how
arguments lie on the stack, what the segment registers hold. Those rules are here, per kind;
what the two kinds share (the byte-piece notation, how a function is declared) is shared.
"""
import re


# The printed text gives a function in the module's default convention (stdcall) without saying
# so, and spells the others out. Compile with stdcall as the default (-ecd; -ecs is __syscall) so
# the text means what it says.
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


def _more_helpers(have):
    """The joins, pieces and widenings the core writes for every pair of sizes up to 8 bytes
    (CONCAT35, SUB86, ZEXT38 ...), beyond the ones spelled out above; and the processor's
    floating-point operations it writes by name. A helper that was missing was an undefined
    function: the call went nowhere and the function calling it was counted as wrong."""
    def kind(n, signed=False):
        return ("int%d" if signed else "uint%d") % (1 if n == 1 else 2 if n == 2 else 4 if n <= 4 else 8)

    def mask(n):
        return "0x%sULL" % ("FF" * n)
    out = []
    for a in range(1, 8):
        for b in range(1, 9 - a):
            if "CONCAT%d%d(" % (a, b) not in have:
                out.append("#define CONCAT%d%d(a,b) ((%s)((((uint8)(a) & %s) << %d) | ((uint8)(b) & %s)))" % (
                    a, b, kind(a + b), mask(a), 8 * b, mask(b)))
    for a in range(2, 9):
        for b in range(1, a):
            if "SUB%d%d(" % (a, b) not in have:
                out.append("#define SUB%d%d(x,n) ((%s)((((uint8)(x) & %s) >> (8 * (n))) & %s))" % (
                    a, b, kind(b), mask(a), mask(b)))
            if "ZEXT%d%d(" % (b, a) not in have:
                out.append("#define ZEXT%d%d(x) ((%s)((uint8)(x) & %s))" % (b, a, kind(a), mask(b)))
            if "SEXT%d%d(" % (b, a) not in have and b in (1, 2, 4):
                out.append("#define SEXT%d%d(x) ((%s)(%s)(x))" % (b, a, kind(a, True), kind(b, True)))
    out += ["double sqrt(double); double sin(double); double cos(double); double tan(double);",
            "double atan2(double, double); double pow(double, double); double ldexp(double, int);",
            "double log(double); double floor(double); double fmod(double, double);",
            # (to the nearest whole number, halves to the even one -- written out, so that a
            # module that never rounds does not have the mathematics library linked into it)
            "#define ROUND(x) (((double)(x) - floor((double)(x)) > 0.5 || ((double)(x) - floor((double)(x)) == 0.5 && "
            "fmod(floor((double)(x)), 2.0) != 0)) ? floor((double)(x)) + 1 : floor((double)(x)))",
            "#define SQRT(x) sqrt((double)(x))", "#define ABS(x) ((x) < 0 ? -(x) : (x))",
            "#define NAN(x) ((x) != (x))",
            "#define fsin(x) sin((double)(x))", "#define fcos(x) cos((double)(x))", "#define fptan(x) tan((double)(x))",
            "#define fpatan(a,b) atan2((double)(a), (double)(b))", "#define f2xm1(x) (pow(2.0, (double)(x)) - 1.0)",
            "#define fscale(a,b) ldexp((double)(a), (int)(b))", "#define log2(x) (log((double)(x)) / 0.6931471805599453)"]
    return "\n".join(out) + "\n"


_X86_HELPERS = _more_helpers(_HELPERS)


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


_NULL = re.compile(r"([=!]=) \((?:[A-Za-z_][\w ]*?) ?\*+\)0x0\b")


def _null_is_zero(t):
    """`x == (int4 *)0x0` is `x == 0` whatever x is; written with the cast, it does not compile
    when x is a number."""
    return _NULL.sub(lambda m: m.group(1) + " 0", t)


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


def _fit_calls(out, order):
    """A call that passes another number of arguments than the function is declared with is made
    through a pointer with the argument list left open. The decompiler recovers each call on the
    evidence at that call, and sometimes sees fewer or more arguments than the definition has;
    the compiler refuses such a call outright, and the whole calling function was lost. Written
    this way the call compiles as it was recovered, and whether that is right is for the judge."""
    takes = {}
    for n in order:
        head = out[n].split("{", 1)[0]
        if "__cdecl" in head or "(" not in head:
            continue
        inside = head[head.index("(") + 1:head.rindex(")")].strip() if ")" in head else ""
        takes[n] = 0 if inside in ("", "void") else inside.count(",") + 1
    if not takes:
        return out
    call = re.compile(r"(?<![\w.>])(%s)\(" % "|".join(re.escape(n) for n in sorted(takes, key=len, reverse=True)))
    fitted = {}
    for n in order:
        head, brace, body = out[n].partition("{")
        pieces, at = [], 0
        for m in call.finditer(body):
            depth, i, commas, empty = 1, m.end(), 0, True
            while i < len(body) and depth:
                c = body[i]
                if c in "([":
                    depth += 1
                elif c in ")]":
                    depth -= 1
                elif c == "," and depth == 1:
                    commas += 1
                if depth and not c.isspace():
                    empty = False
                i += 1
            given = 0 if empty else commas + 1
            if depth == 0 and given != takes[m.group(1)]:
                pieces.append(body[at:m.start()] + "((int4 (*)())%s)(" % m.group(1))
                at = m.end()
        fitted[n] = head + brace + "".join(pieces) + body[at:] if pieces else out[n]
    return fitted


def compilable32(module_path, funcs, outdir):
    """-> (preamble, {name: text}, [names], {va: name}, [import names used]) for a whole-module build:
    the core's output with its base types defined, its globals bound to the original's addresses,
    and every function and import declared before use with its real convention."""
    from codebreaker.symbols import api_arguments as apiarity
    from codebreaker.build import watcom as B
    from codebreaker.decompile import decompiler
    from codebreaker.decompile import describe
    out, order, by_va, _err = decompiler.raw(module_path, funcs, outdir)
    imports = dict(B.import_map(module_path))
    # imports by number, under the names the description gave them: declared and linked too
    numbered = describe.BY_NUMBER.get(module_path) or {}
    imports.update({n: stem for n, (stem, _number, _pop) in numbered.items() if n not in imports})
    sizes, in_ecx = {}, set()
    for n in order:
        body = []
        for ln in out[n].split("\n"):
            m = re.match(r"/\*@G ([0-9a-f]+) (\d+) \*/$", ln)
            if m:
                sizes.setdefault(int(m.group(1), 16), set()).add(int(m.group(2)))
            else:
                body.append(ln)
        # (a note the core put before the function -- "WARNING: Removing unreachable block" --
        # is not part of it: left in, it became the function's declaration and broke the header)
        t = _LEADING.sub("", "\n".join(body).strip())
        # `x._1_2_` is the core's spelling of "2 bytes of x starting at byte 1"
        t = re.sub(r"\b([A-Za-z_]\w*)\._(\d+)_(\d+)_",
                   lambda mm: "(*(%s *)((char *)&%s + %s))" % (_BY_SIZE.get(int(mm.group(3)), "uint4"),
                                                               mm.group(1), mm.group(2)), t)
        t = _compare_as_numbers(_keep_frame(_null_is_zero(t)))
        # a function's address stored in a pointer the core typed loosely
        t = re.sub(r"(?m)(= )((?:%s);)$" % "|".join(re.escape(x) for x in order), r"\1(code *)\2", t)
        # every function returns a value in EAX whether or not the core saw it used
        t = re.sub(r"\Avoid(\s)", r"int\1", t)
        # the compiler used here has no word for "first argument in ECX": the function is
        # declared plainly, and a #pragma in the header says where its first argument is
        if "__thiscall" in t.split("{", 1)[0]:
            head, brace, body = t.partition("{")
            t = head.replace("__thiscall ", "").replace("__thiscall", "") + brace + body
            in_ecx.add(n)
        out[n] = t
    text = "\n".join(out.values())
    macros = []
    # a global the code calls through holds a function pointer, whatever else it is compared with
    called_through = set(re.findall(r"\(\*([a-z]+Ram[0-9a-f]{8})\)\s*\(", text))
    # A global the decompiler named as a number but reads through (`*iRam..`, `iRam..[n]`) is
    # read through as a pointer THERE; it stays a number everywhere else, where it is added to
    # and compared as one. (Declaring it a pointer would change what `iRam.. + 4` means.)
    kind = {"i": "int4", "u": "uint4", "x": "xunknown4"}

    def through(m):
        name = m.group(2) if m.lastindex == 2 else m.group(1)
        if name in called_through or name[0] not in kind:
            return m.group(0)
        if m.lastindex == 2:
            return "%s*(%s *)%s" % (m.group(1), kind[name[0]], name)
        return "((%s *)%s)[" % (kind[name[0]], name)
    for n in order:
        t = re.sub(r"((?<![\w)\]]))\*([a-z]+Ram[0-9a-f]{8})\b", through, out[n])
        out[n] = re.sub(r"\b([a-z]+Ram[0-9a-f]{8})\[", through, t)
    text = "\n".join(out.values())
    for pre, addr in sorted(set(re.findall(r"\b([a-z]+)Ram([0-9a-f]{8})\b", text))):
        szs = sizes.get(int(addr, 16)) or {4}
        size = min(szs) if len(szs) > 1 and pre[0] in "cb" else max(s for s in szs if s <= 8) if any(s <= 8 for s in szs) else 4
        ty = "code *" if (pre == "pc" or "%sRam%s" % (pre, addr) in called_through) else (
            "int%d" % size if pre[0] == "i" else _BY_SIZE.get(size, "uint4"))
        if pre[0] == "p" and len(pre) > 1 and ty != "code *":
            ty = _ram_type(pre, size, False)             # a pointer, by the decompiler's own naming (piRam: int *)
        macros.append("#define %sRam%s (*(%s *)0x%s)" % (pre, addr, ty, addr))
        if re.search(r"(?<![A-Za-z0-9])_%sRam%s\b" % (pre, addr), text):
            # (the core's name for the same place read at another width: `_uRam0040c000`)
            macros.append("#define _%sRam%s (*(%s *)0x%s)" % (pre, addr, ty, addr))
    called = set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", text))
    used = sorted(n for n in called if n in imports and n not in out)
    decls = []
    for n in used:
        pop = numbered[n][2] if n in numbered else apiarity.stack_bytes(imports[n], n)
        decls.append("int %s %s();" % ("__cdecl" if pop == 0 else "__stdcall", n))
    for n in order:                                    # each function, declared before any use
        head = " ".join(out[n].split("{", 1)[0].split())
        if not head.endswith(")"):
            continue
        if "__cdecl" in head:
            # The caller removes the arguments, so nothing in the binary fixes how many there
            # are: the decompiler recovers each call on its own evidence and callers need not
            # agree with the definition. Declared with the list left open, each call compiles as
            # it was recovered. (A function that removes its own arguments -- the default here --
            # states their size in its return, and keeps its full declaration.)
            head = head[:head.index("(")] + "()"
        decls.append(head + ";")
        if n in in_ecx:
            decls.append('#pragma aux %s "_*" parm routine [ecx] value [eax] modify [eax ecx edx];' % n)
    out = _fit_calls(out, order)
    # the core's names for values of sizes C has no type for (5 bytes of a 64-bit product, 10 of a
    # floating-point register): each is given the next type that holds it. One left undeclared
    # in a function's heading broke the header, and with it every function of the module.
    odd = []
    for name in sorted(set(re.findall(r"\b(unk(?:byte|int|uint)\d+|u?int[3567]|xunknown[3567])\b",
                                      text + "\n".join(decls)))):
        if re.search(r"\b%s;" % name, _TYPES):
            continue
        size = int(re.search(r"\d+$", name).group(0))
        signed = name.startswith(("unkint", "int"))
        odd.append("typedef %s %s;" % ("long double" if size > 8 else ("long long" if size > 4 else "int") if signed
                                       else "unsigned long long" if size > 4 else "unsigned int", name))
    preamble = _TYPES + "\n".join(odd) + "\n" + _HELPERS + _X86_HELPERS + "\n".join(decls) + "\n" + "\n".join(macros) + "\n"
    from codebreaker.decompile import struct_text
    preamble, out = struct_text.apply(module_path, preamble, out, by_va)       # `p->field` for `*(T *)(p + N)`
    return preamble, out, order, by_va, used, in_ecx


class Module(object):
    """A decompiled module: `header` (shared text), `functions` {address: (name, text)},
    `order` (names by address), `imports` (the import names its functions call)."""

    def __init__(self, header, texts, order, by_va, imports, in_ecx=()):
        self.header, self.texts, self.order, self.by_va, self.imports = header, texts, order, by_va, imports
        self.in_ecx = set(in_ecx)                # names of functions that take their first argument in ECX
        self.functions = {va: (n, texts[n]) for va, n in by_va.items()}


HEADER = """typedef long long int8; typedef unsigned long long uint8; typedef unsigned long long xunknown8;
typedef double float8; typedef long double float10;
typedef unsigned long unkbyte3; typedef long unkint3; typedef unsigned long unkuint3; typedef long int3;
typedef unsigned long uint3; typedef unsigned long long unkbyte6; typedef long double unkbyte10;
typedef signed char int1; typedef short int2; typedef long int4;
typedef unsigned char uint1; typedef unsigned short uint2; typedef unsigned long uint4;
typedef unsigned char xunknown1; typedef unsigned short xunknown2; typedef unsigned long xunknown4;
typedef unsigned char undefined; typedef unsigned char undefined1; typedef unsigned short undefined2;
typedef unsigned long undefined4; typedef unsigned char bool; typedef unsigned long float4;
typedef int2 code();
#define true 1
uint2 codebreaker_ss(void);
#pragma aux codebreaker_ss = "mov ax,ss" value [ax] modify exact [ax];
uint2 codebreaker_ds(void);
#pragma aux codebreaker_ds = "mov ax,ds" value [ax] modify exact [ax];
uint2 codebreaker_cs(void);
#pragma aux codebreaker_cs = "mov ax,cs" value [ax] modify exact [ax];
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
#define CODEBREAKER_FAR(seg, off) ((void __far *)(((uint4)(uint2)(seg) << 16) | (uint2)(off)))
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


def _joined_arguments(text):
    """`_a1` is the decompiler's spelling of "a1 and the argument after it, as one four-byte
    value" (a far pointer or a long passed as two words). Arguments lie next to each other on
    the stack, lowest first, so that value is the four bytes starting at a1."""
    # read through directly (`*_a1`): it is a far pointer; the decompiler gives no type for what it
    # points at, and a word is what such a read or write is in nearly every case
    text = re.sub(r"\*_(a\d+|param_\d+)\b", r"*(*(int2 __far * *)&\1)", text)
    return re.sub(r"(?<![\w.])_(a\d+|param_\d+)\b", r"(*(uint4 *)&\1)", text)


def _segment_registers(text):
    """`unaff_SS`, `unaff_DS`, `unaff_CS` are the decompiler's names for "whatever that segment
    register held when the function was entered" -- used when it passes the address of a local
    as a far pointer (offset, SS) or a string constant as (offset, CS). Left as plain locals
    they would be garbage; they are given the register's value."""
    return re.sub(r"(?m)^(\s+\w+ )unaff_(SS|DS|CS);\s*$",
                  lambda m: "%sunaff_%s = codebreaker_%s();" % (m.group(1), m.group(2), m.group(2).lower()), text)


_FAR_LOAD = re.compile(r"\[bp \+ (0x[0-9a-f]+|\d+)\]")


def _far_loads(code, start, end):
    """The offsets from BP of the argument words a function loads as far pointers: `les bx,
    [bp+6]` takes the word at +6 as an offset and the one above it as its segment."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    found = set()
    for _a, _n, mnemonic, ops in md.disasm_lite(bytes(code[start:end]), start):
        if mnemonic in ("les", "lds"):
            m = _FAR_LOAD.search(ops)
            if m:
                found.add(int(m.group(1), 0))
    return found


def _far_arguments(text, loads, near):
    """Read and written THROUGH, an argument the original loads as a far pointer is one -- the
    word after it is its segment. The decompiler shows the two words as two numbers and the
    access as through the first alone, which in C is a pointer into the data segment: the
    rebuilt function then reads its own data where the original reads wherever its caller
    pointed. Arguments lie on the stack lowest first, the first 6 bytes above BP in a far
    function and 4 in a near one."""
    if not loads:
        return text
    head, brace, body = text.partition("{")
    m = re.search(r"\(([^()]*)\)\s*$", head)
    if not m or m.group(1).strip() in ("", "void"):
        return text
    at, where = (4 if near else 6), {}
    params = [x.strip() for x in m.group(1).split(",")]
    for prm in params:
        words = prm.split()
        name = words[-1].lstrip("*")
        wide = "*" in prm and "__far" in prm or words[0] in _WIDE
        where[at] = (name, words[0], wide)
        at += 4 if wide else 2
    for off in sorted(loads):
        if off not in where or off + 2 not in where:
            continue
        (name, kind, wide), (segment, kind2, wide2) = where[off], where[off + 2]
        if wide or wide2 or kind not in ("int2", "uint2", "xunknown2") or kind2 not in ("int2", "uint2", "xunknown2"):
            continue
        n = re.escape(name)
        body = re.sub(r"\*\(([A-Za-z_]\w*) \*\)\(%s \+ ([^()]*)\)" % n,
                      lambda mm: "*(%s __far *)CODEBREAKER_FAR(%s, %s + %s)" % (mm.group(1), segment, name, mm.group(2)), body)
        body = re.sub(r"\*\(([A-Za-z_]\w*) \*\)%s(?![\w(])" % n,
                      lambda mm: "*(%s __far *)CODEBREAKER_FAR(%s, %s)" % (mm.group(1), segment, name), body)
    return head + brace + body


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


def declaration(text):
    """A declaration that fixes the function's convention and result but not its arguments.
    The decompiler works out arguments one call at a time and callers do not always agree with
    the definition; with the argument list left open each call compiles as it was recovered,
    and a call that passes the wrong amount shows up when it is run, not as a compile error
    that hides the whole function."""
    head = " ".join(text.split("{", 1)[0].split())
    return head[:head.index("(")] + "();"


_RAM = re.compile(r"\b([a-z]+)Ram([0-9a-f]{8})\b")
_LEADING = re.compile(r"\A(?:\s*/\*.*?\*/)+\s*", re.S)


def _global_type(prefix, size):
    if prefix[0] == "p" and len(prefix) > 1:             # a pointer, by the decompiler's own naming (piRam: int *)
        to = {"i": "int2", "u": "uint2", "c": "char", "b": "bool"}.get(prefix.lstrip("p")[:1], "xunknown2")
        stars = "*" * (len(prefix) - len(prefix.lstrip("p")))
        return "%s %s%s" % (to, "__far " if size == 4 else "", stars)
    signed = prefix.startswith("i") or prefix.startswith("c")
    return {1: "int1" if signed else "uint1", 2: "int2" if signed else "uint2", 4: "int4" if signed else "uint4"}.get(
        size, "uint2")


def compilable16(module_path, outdir=None):
    from codebreaker.decompile import decompiler, describe
    texts, order, by_va, d = decompiler.decompile16(module_path, outdir)
    m = Module16()
    m.texts, m.order, m.by_va, m.where = {}, [], {}, {}
    m.near, m.wide, m.novalue = set(), set(), set()
    sizes = {}                                               # address of a global -> bytes of the access
    ends, starts = {}, {}                                    # where each function's code stops: at the next one
    for va in sorted(d.where):
        starts.setdefault(d.where[va][0], []).append((d.where[va][1], va))
    for seg, lst in starts.items():
        lst.sort()
        for k, (off, va) in enumerate(lst):
            ends[va] = lst[k + 1][0] if k + 1 < len(lst) else len(d.images.get(seg, b""))
    for va in sorted(by_va):
        name = by_va[va]
        raw = texts[name]
        far = set()
        for mm in _FACT.finditer(raw):
            if mm.group(1) == "P" and mm.group(3) == "4":
                far.add(mm.group(2))
            elif mm.group(1) == "G":
                a = int(mm.group(2), 16)
                sizes[a] = max(sizes.get(a, 0), int(mm.group(3))) if int(mm.group(3)) <= 4 else sizes.get(a, 2)
        text = _LEADING.sub("", _FACT.sub("", raw))          # notes the decompiler put before the function
        text = _null_is_zero(_segment_registers(_joined_arguments(_pieces(_far(text.strip(), far)))))
        seg, off = d.where[va]
        if seg in d.images:
            text = _far_arguments(text, _far_loads(d.images[seg], off, ends.get(va, off)), "far" not in text.split("{", 1)[0])
        if "return" in far:                                  # the function returns a far pointer
            head, brace, body = text.partition("{")
            head = re.sub(r"\*(?=[\s\w]*\b%s\s*\()" % re.escape(name), "__far *", head, count=1)
            text = head + brace + body
            m.wide.add(name)
        head = text.split("{", 1)[0]
        if head.split()[0] == "void":
            # It sets no result -- but its callers may still read AX, and the decompiler shows
            # them doing so. Declared as returning a word, with no `return`, it leaves AX as the
            # original does: whatever was last there.
            m.novalue.add(name)
            text = "xunknown2" + text[4:]
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
    # Globals the decompiler names by address. In the data segment: an offset through DS, as in
    # the original. In another segment (constants kept beside the code, a second data segment):
    # the module carries that segment's bytes, and the global is a place in them.
    macros, m.segments = [], {}
    for prefix, addr in sorted(set(_RAM.findall(body))):
        va = int(addr, 16)
        seg, off = describe.where(va)
        ty = _global_type(prefix, sizes.get(va, 2))
        if seg == d.data_segment:
            macros.append("#define %sRam%s (*(%s *)0x%x)" % (prefix, addr, ty, off))
        elif seg in d.images and off < len(d.images[seg]):
            m.segments[seg] = d.images[seg]
            macros.append("#define %sRam%s (*(%s __far *)(codebreaker_segment_%d + 0x%x))" % (prefix, addr, ty, seg, off))
    decls = ["extern uint1 __far codebreaker_segment_%d[];" % n for n in sorted(m.segments)]
    decls += ["extern uint4 __far __stdcall %s();" % nm for nm in sorted(m.imports)]
    decls += [declaration(m.texts[n]) for n in m.order]
    m.header = HEADER + "\n" + "\n".join(macros + decls) + "\n"
    m.description = d
    return m


# PowerPC ---------------------------------------------------------------------------------------
_POWERPC = """
#define PIECE(x, off, T) (*(T *)((char *)&(x) + sizeof(x) - (off) - sizeof(T)))
/* Instructions the decompiler writes by name rather than as C. */
static uint4 codebreaker_lzcount32(uint4 x) { uint4 n = 0; if (!x) return 32; while (!(x & 0x80000000)) { x <<= 1; n++; } return n; }
static uint4 codebreaker_lzcount64(uint8 x) { uint4 n = 0; if (!x) return 64; while (!(x >> 63)) { x <<= 1; n++; } return n; }
#define LZCOUNT(x) (sizeof(x) == 8 ? codebreaker_lzcount64((uint8)(x)) : codebreaker_lzcount32((uint4)(x)))
#define ABS(x) ((x) < 0 ? -(x) : (x))
double sqrt(double);
#define SQRT(x) sqrt((double)(x))
/* a store that succeeds when nothing else wrote there since the matching load: here nothing else runs */
static uint4 codebreaker_store_conditional(uint4 v, uint4 a, uint4 b) { *(uint4 *)(a + b) = v; return 1; }
#define storeWordConditionalIndexed(v, a, b) codebreaker_store_conditional((uint4)(v), (uint4)(a), (uint4)(b))
static void codebreaker_zero_block(uint4 a, uint4 b) { uint1 *p = (uint1 *)((a + b) & ~31u); int i; for (i = 0; i < 32; i++) p[i] = 0; }
#define dataCacheBlockSetToZero(a, b) codebreaker_zero_block((uint4)(a), (uint4)(b))
/* hints to the processor and waits for it, which change nothing a program can see; and a trap,
   which is taken only when a check the compiler put in fails (a division by zero) */
#define sync(...) ((void)0)
#define instructionSynchronize(...) ((void)0)
#define enforceInOrderExecutionIO(...) ((void)0)
#define dataCacheBlockTouch(...) ((void)0)
#define dataCacheBlockTouchForStore(...) ((void)0)
#define dataCacheBlockStore(...) ((void)0)
#define dataCacheBlockFlush(...) ((void)0)
/* a trap: taken when the comparison its first operand selects holds (16 less, 8 greater, 4 equal,
   2 and 1 the same unsigned) -- the check the compiler put before a division. Taken, the program
   stops there; here by a call to nowhere. */
static void codebreaker_trap(uint4 to, int8 a, int8 b, uint8 ua, uint8 ub) {
  if (((to & 16) && a < b) || ((to & 8) && a > b) || ((to & 4) && a == b) || ((to & 2) && ua < ub) || ((to & 1) && ua > ub))
    ((void (*)())0)();
}
#define trapWord(to, a, b) codebreaker_trap((uint4)(to), (int8)(int4)(a), (int8)(int4)(b), (uint8)(uint4)(a), (uint8)(uint4)(b))
#define trapDoubleWord(to, a, b) codebreaker_trap((uint4)(to), (int8)(a), (int8)(b), (uint8)(a), (uint8)(b))
"""
_WIDE_ADDRESS = re.compile(r"\b0xffffffff([89a-f][0-9a-f]{7})\b")
_PIECE = re.compile(r"\b([A-Za-z_]\w*(?:\[[^\[\]]*\])?)\._(\d+)_(\d+)_")


_HOME = re.compile(r"\b_?([a-z]+)Stack(000000[0-9a-f]{2})\b")


class ModulePowerPC(object):
    pass


def _homed_arguments(t):
    """A caller leaves room on the stack for each of the eight register arguments (eight bytes
    each, from 0x10 above the stack pointer at entry), and a function that needs an argument's
    address stores it there. The decompiler shows that slot as a stack variable of the CALLER's
    frame; it is the argument, and is written as the argument."""
    head = t.split("{", 1)[0]
    m = re.search(r"\(([^()]*)\)", head)
    names = [x.strip().split()[-1].lstrip("*") for x in m.group(1).split(",")]         if m and m.group(1).strip() not in ("", "void") else []

    def slot(off):
        k = (off - 0x10) // 8
        return names[k] if 0 <= k < len(names) and off % 8 in (0, 4) else None
    head, brace, body = t.partition("{")
    # the slot's own declaration goes; every other mention becomes the argument
    body = re.sub(r"(?m)^  [A-Za-z_][\w ]*?[ *]_?[a-z]+Stack(000000[0-9a-f]{2})(?: \[\d+\])?;[ \t]*\n",
                  lambda mm: "" if slot(int(mm.group(1), 16)) else mm.group(0), body)
    body = _HOME.sub(lambda mm: slot(int(mm.group(2), 16)) or mm.group(0), body)
    body = re.sub(r"(?m)^\s*(\w+) = \1;[ \t]*\n", "", body)   # the store into its own slot
    return head + brace + body


def _ram_type(prefix, size, called):
    """The C type of a global from the decompiler's name prefix (p = pointer to what follows)."""
    stars = len(prefix) - len(prefix.lstrip("p"))
    base = prefix[stars:] or "x"
    if stars:
        if base[0] == "c":
            return "code " + "*" * stars if called else "char " + "*" * stars
        ty = {"i": "int4", "u": "uint4", "b": "bool", "f": "float4"}.get(base[0], "xunknown4")
        return ty + " " + "*" * stars
    if called:
        return "code *"
    if base[0] == "i":
        return "int%d" % size
    if base[0] == "c":
        return "char"
    return _BY_SIZE.get(size, "uint4")


def call_counts(texts, names):
    """{name: number of arguments} for the functions in `names` that every call in `texts`
    passes the same number of arguments to. Nothing in a PowerPC image states how many arguments
    a function outside it takes; that all its callers set up the same number is the evidence
    there is."""
    seen = {}
    pat = re.compile(r"\b(%s)\s*\(" % "|".join(re.escape(n) for n in sorted(names, key=len, reverse=True))) if names else None
    for t in texts.values() if pat else ():
        body = t.split("{", 1)[-1]
        for m in pat.finditer(body):
            depth, count, i, empty = 1, 1, m.end(), True
            while i < len(body) and depth:
                c = body[i]
                if c in "([":
                    depth += 1
                elif c in ")]":
                    depth -= 1
                elif c == "," and depth == 1:
                    count += 1
                if depth and not c.isspace():
                    empty = False
                i += 1
            seen.setdefault(m.group(1), set()).add(0 if empty else count)
    return {n: next(iter(c)) for n, c in seen.items() if len(c) == 1}


def compilable_powerpc(module_path, outdir):
    """A PowerPC image decompiled and made compilable. -> an object with header, texts, order,
    by_va, externs {name: address in the original}, argc {extern: argument count}, novalue
    (names of functions that return nothing).

    Every function is written in the older style and declared with its argument list left
    open: all arguments travel in registers whatever their number, so callers and definitions
    need not agree for the code to mean the same, and the decompiler recovers each on its own
    evidence. A global is bound to the address the original keeps it at. A value split by bytes
    is taken from the end of its storage (the processor is big-endian). An address the
    decompiler shows widened to 64 bits (registers are 64 bits wide, addresses 32) is written as
    the 32-bit address it is."""
    from codebreaker.decompile import decompiler, describe
    from codebreaker.formats import powerpc_image
    img = powerpc_image.read(module_path)
    out, order, by_va, _err = decompiler.raw(module_path, describe.function_starts(module_path), outdir)
    sizes, novalue = {}, set()
    for n in order:
        body = []
        for ln in out[n].split("\n"):
            m = re.match(r"/\*@G ([0-9a-f]+) (\d+) \*/$", ln)
            if m:
                sizes.setdefault(int(m.group(1), 16), set()).add(int(m.group(2)))
            elif not ln.startswith("/*@P "):
                body.append(ln)
        t = "\n".join(body).strip()
        t = _WIDE_ADDRESS.sub(r"0x\1", t)
        t = _PIECE.sub(lambda mm: "PIECE(%s, %s, %s)" % (mm.group(1), mm.group(2), _BY_SIZE.get(int(mm.group(3)), "uint4")), t)
        t = _compare_as_numbers(t)
        t = re.sub(r"(?m)(= )((?:%s);)$" % "|".join(re.escape(x) for x in order), r"\1(code *)\2", t)
        t = _homed_arguments(t)
        lead = _LEADING.match(t)
        body = t[lead.end():] if lead else t
        if body.startswith("void") and not body[4:5].isalnum():
            # every function leaves something in r3 and callers are free to use it
            novalue.add(n)
            t = (lead.group(0) if lead else "") + "uint8" + body[4:]
        out[n] = _open_arguments(t)
    text = "\n".join(out.values())
    macros = []
    called_through = set(re.findall(r"\(\*([a-z]+Ram[0-9a-f]{8})\)\s*\(", text))
    for pre, addr in sorted(set(_RAM.findall(text))):
        szs = sizes.get(int(addr, 16)) or {4}
        size = max((x for x in szs if x <= 8), default=4)
        macros.append("#define %sRam%s (*(%s *)0x%s)" % (
            pre, addr, _ram_type(pre, size, "%sRam%s" % (pre, addr) in called_through), addr))
    # a call to a place in the original the decompiler was not told is a function: called there,
    # in the original, which is in memory beside the rebuilt image when it is judged
    lo_image = min(va for va, _off, _size in img.spans)
    hi_image = max(va + size for va, _off, size in img.spans)
    for addr in sorted(set(re.findall(r"\bfunc_0x([0-9a-f]{8})\b", text))):
        if lo_image <= int(addr, 16) < hi_image and "func_0x" + addr not in out:
            macros.append("#define func_0x%s ((code *)0x%s)" % (addr, addr))
    used = sorted(set(re.findall(r"\b(import\d+_ord_\d+)\s*\(", text)))
    decls = ["uint8 %s();" % n for n in used]
    for n in order:
        head = " ".join(_LEADING.sub("", out[n]).split("{", 1)[0].split())
        if "(" not in head:
            continue
        decls.append(head[:head.index("(")] + "();")
    m = ModulePowerPC()
    m.header = _TYPES.replace("typedef int code();", "typedef uint8 code();") + _HELPERS + _POWERPC + "\n".join(decls) + "\n" + "\n".join(macros) + "\n"
    m.texts, m.order, m.by_va, m.novalue = out, order, by_va, novalue
    by_name = {name: va for va, name in img.imports.items()}
    m.externs = {n: by_name[n] for n in used if n in by_name}
    m.argc = call_counts(out, set(m.externs))
    m.image = img
    return m
