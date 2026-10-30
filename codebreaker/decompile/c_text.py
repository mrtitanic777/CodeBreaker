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
import os
import re
import struct

from codebreaker.decompile import ctree
from codebreaker.translate import l2
from codebreaker.translate.backends import c as cback


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
_LIBM = ("sqrt", "sin", "cos", "tan", "atan2", "pow", "ldexp", "log", "floor", "fmod")    # declared in it


_LOCAL = re.compile(r"(?m)^  ([A-Za-z_][\w ]*?(?:\s*\*+\s*|\s))([a-z]+Stack_([0-9a-f]+))(?: \[(\d+)\])?;[ \t]*$")


_SIZEOF = cback.SIZEOF                                       # (bytes of each of the decompiler's scalar types)


def _keep_frame(t, sizes=None):
    """Keep a function's stack locals where the original had them, relative to one another.

    The original often builds a small structure in its frame and passes its address
    (`mov [ebp-18h], hwnd / mov [ebp-14h], id / lea edx, [ebp-18h] / push edx`). The core
    recovers the fields as separate locals; a C compiler then places them wherever it likes, and
    the callee, reading through the pointer, finds something else. When any stack local's address
    can escape, the locals are declared as the fields of one frame structure at their original
    offsets, so every address taken still points at what the original put there. `sizes`: more
    types' sizes -- the structures recovered for the function (_struct_sizes)."""
    lines = _declaration_lines(t)                        # (`return x;` / `goto x;` are not declarations)
    locs = [m for m in _LOCAL.finditer(t) if m.group(0) in lines]
    if not locs:
        return t
    names = [m.group(2) for m in locs]
    src = ctree.Source(t)
    # (the stack pointer at entry, which the core writes `register0x00000010` when no local stands
    # for the place -- MSVC's SEH frame saving ESP, a buffer reached from ESP: the frame says where)
    esp = _ENTRY_ESP in t
    if not (esp or any(m.group(4) for m in locs) or any(src.unary(n) and src.unary(n)[0] == "&"
                                                       for n in src.find("identifier") if src.at(n) in names)):
        return t
    items = []
    for m in locs:
        ty = " ".join(m.group(1).split())
        size = 4 if ty.endswith("*") else _SIZEOF.get(ty) or (sizes or {}).get(ty)
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
            return "/*codebreaker:frame*/"                # (a comment: the parser reads past it; it stopped at a NUL)
        return ""
    t = _LOCAL.sub(lambda m: strip(m) if m.group(0) in lines else m.group(0), t)
    wanted = set(names)

    def fields(src):
        for n in src.find("identifier"):
            if src.at(n) in wanted:
                src.replace(n, "_f." + src.at(n))
    t = ctree.rewrite(t, fields).replace("/*codebreaker:frame*/", decl)
    # the frame structure ends where the original's locals end: at the stack pointer on entry
    return t.replace(_ENTRY_ESP, "((int4)&_f + (int4)sizeof(_f))") if esp else t


_ENTRY_ESP = "register0x00000010"


_PTR_DECL = re.compile(r"(?m)^  [A-Za-z_][\w ]*?\*+\s*(\w+)(?: \[\d+\])?;[ \t]*$")


_COMPARISONS = ("<=", ">=", "<", ">", "==", "!=")


def _null_is_zero(t):
    """`x == (int4 *)0x0` is `x == 0` whatever x is; written with the cast, it does not compile
    when x is a number."""
    def fix(src):
        for n in src.find("binary_expression"):
            if n.child_by_field_name("operator").type in ("==", "!="):
                r = n.child_by_field_name("right")
                if r.type == "cast_expression" and _null_pointer(src, r):
                    src.replace(r, "0")
    return ctree.rewrite(t, fix)


def _null_pointer(src, cast):
    """`(T *)0x0`: a pointer type, the value 0x0"""
    ty, value = cast.child_by_field_name("type"), cast.child_by_field_name("value")
    star = ty.child_by_field_name("declarator")
    return value.type == "number_literal" and src.at(value) == "0x0" and star is not None and \
        star.type == "abstract_pointer_declarator"


_CAST_PTR = "(cast to a pointer)"


def _compare_as_numbers(t):
    """The core compares a pointer with a plain number freely (`if (pcVar2 < (uint4)a1)`); C does
    not allow it. Where exactly one side is a pointer variable, compare its value as a number."""
    lines = _declaration_lines(t)
    ptrs = {m.group(1) for m in _PTR_DECL.finditer(t) if m.group(0) in lines}
    # (and the parameters declared pointers: `a1 == (uint4)psVar1`, a1 a structure's pointer)
    head = t.split("{", 1)[0]
    if "(" in head:
        ptrs.update(re.findall(r"\*+\s*(\w+)\s*(?=[,)])", head[head.index("("):]))
    for ln in t.split("\n"):
        if "struct {" in ln and "} _f;" in ln:
            ptrs.update(re.findall(r"\*+\s*(\w+)(?:\[\d+\])?;", ln))
    if not ptrs and "*)" not in t:
        return t

    def operand(src, n):
        """A variable -- itself, a field of the frame record `_f.x`, or it cast to a number: the
        name it is a pointer by ('' when cast); None when it is not such an operand"""
        if n.type == "cast_expression" and src.at(n.child_by_field_name("type")) in ("int4", "uint4"):
            v = n.child_by_field_name("value")
            return "" if v.type != "cast_expression" and operand(src, v) is not None else None
        if n.type == "cast_expression" and src.at(n.child_by_field_name("type")).replace(" ", "").endswith("*"):
            return _CAST_PTR                                 # (`(s_f_p *)(q + n)`: a pointer, as cast)
        if n.type == "identifier":
            return src.at(n)
        if n.type == "field_expression" and src.at(n.child_by_field_name("argument")) == "_f" and \
                src.at(n.child_by_field_name("operator")) == ".":
            return src.at(n.child_by_field_name("field"))
        return None

    def fix(src):
        for n in src.find("binary_expression"):
            if n.child_by_field_name("operator").type not in _COMPARISONS:
                continue
            sides = [n.child_by_field_name("left"), n.child_by_field_name("right")]
            names = [operand(src, s) for s in sides]
            if None in names:
                continue
            if _CAST_PTR in names and "" not in names:
                continue                                     # (a pointer cast only against a number cast:
                                                             # a global's type is not seen here)
            ptr = [x in ptrs or x == _CAST_PTR for x in names]
            if ptr[0] != ptr[1]:
                for s, is_ptr in zip(sides, ptr):
                    if is_ptr:
                        src.insert(s.start_byte, "(uint4)")
    return ctree.rewrite(t, fix)


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

    def fit(src):
        for c in src.find("call_expression", under=src.body()):
            f = c.child_by_field_name("function")
            if f.type == "identifier" and src.at(f) in takes and \
                    len(c.child_by_field_name("arguments").named_children) != takes[src.at(f)]:
                src.replace(f, "((int4 (*)())%s)" % src.at(f))
    return {n: ctree.rewrite(out[n], fit) for n in order} if takes else out


def _called(text):
    """The names of the functions the text calls (`f(...)`; not a cast or a declaration the grammar
    read as a call)."""
    src = ctree.Source(text)
    return {src.at(c.child_by_field_name("function")) for c in src.find("call_expression")
            if c.child_by_field_name("function").type == "identifier" and src.is_call(c)}


def _called_through(text):
    """Globals the text calls through: `(*xRam0040c000)(...)`."""
    src = ctree.Source(text)
    out = set()
    for c in src.find("call_expression"):
        f = c.child_by_field_name("function")
        if f.type == "parenthesized_expression" and f.named_child_count == 1:
            p = f.named_children[0]
            if p.type == "pointer_expression" and src.at(p.child_by_field_name("operator")) == "*" and \
                    p.child_by_field_name("argument").type == "identifier" and \
                    _RAM.fullmatch(src.at(p.child_by_field_name("argument"))):
                out.add(src.at(p.child_by_field_name("argument")))
    return out


def _globals(text):
    """(prefix, address) of every global the text names by its address (`iRam0040c000`)."""
    src = ctree.Source(text)
    return {m.groups() for m in (_RAM.fullmatch(src.at(n)) for n in src.find("identifier")) if m}


def _without_notes(text):
    """The text without the notes the decompiler put before the function (`/* WARNING: ... */`)."""
    src = ctree.Source(text)
    end = 0
    for c in src.root.children:
        if c.type != "comment" or not src.at(c).startswith("/*"):
            break
        end = c.end_byte
    if not end:
        return text
    return src.data[end:].decode("utf-8").lstrip()


# A structure the decompiler recovered from a variable's field accesses (CB_STRUCTS), as its facts
# give it (decompiler.split_notes): [name, bytes, [[offset, bytes, type], ...]] -- a type is one
# word, a pointer's ending in `*` (or `^`, a 16-bit far pointer)
_C_WORDS = {"char", "short", "int", "long", "float", "double", "void", "code"}


def _struct_sizes(structs):
    """{structure name: its bytes} of the structures recovered"""
    out = {}
    for name, size, _fields in structs:
        out.setdefault(name, size)
    return out


def _struct_decls(structs, header, far16=False):
    """The C declaring the structures recovered -- every name first (`typedef struct
    s_f_p s_f_p;`, so one may point at another), then each at its exact layout: its fields at
    their offsets with filler between, the whole its full size, packed (the 32-bit header packs
    everything; a 16-bit structure says so itself). A field type the header does not know is
    the bytes it takes."""
    found = {}
    for name, size, fields in structs:
        if name not in found:
            found[name] = (size, [tuple(f) for f in fields])
    if not found:
        return []
    known = set(re.findall(r"typedef [^;()]*?\b(\w+);", header)) | _C_WORDS | set(found)
    lines = ["typedef struct %s %s;" % (n, n) for n in found]
    for name, (size, fields) in found.items():
        body, at = [], 0
        for off, width, tok in sorted(fields):
            if off < at or off + width > size:              # (overlapping or outside: left as bytes)
                continue
            if off > at:
                body.append("uint1 _pad_%x[%d];" % (at, off - at))
            base = tok.rstrip("*^")
            stars = tok[len(base):]
            if base not in known:
                base = "xunknown%d" % width if not stars and width in (1, 2, 4, 8) else "xunknown1"
            if not stars and base in found:                  # (a structure held whole: its bytes)
                base = "xunknown1" if width not in (1, 2, 4, 8) else "xunknown%d" % width
            ptr = "".join(" __far *" if c == "^" else " *" for c in stars)
            body.append("%s%s f_%x;" % (base, ptr, off))
            at = off + width
        if size > at:
            body.append("uint1 _pad_%x[%d];" % (at, size - at))
        lines.append("%sstruct %s { %s };" % ("_Packed " if far16 else "", name, " ".join(body)))
    return lines


_GAP_FIELD = re.compile(r"field_0x([0-9a-f]+)")


def _struct_gaps(text, distance=None):
    """`&p->field_0x1` -- the core's name for an offset of a recovered structure that holds no
    field of it (`p` stepped a byte along, `p + 4` read where nothing else is): the bytes at that
    offset, `((uint1 *)p + 0x1)`, and `p->field_0x1` the byte there. With `.` the same from the
    structure's own address: `&p[1].field_0x4` (past its end) is `((uint1 *)&(p[1]) + 0x4)`.
    `distance` (16-bit): {variable: "far" or "near"} -- the byte pointer keeps the variable's
    distance (`uint1 __far *` for a far one); where the base is no variable of known distance the
    offset is left as it is."""
    if "field_0x" not in text:
        return text
    for _ in range(4):                                   # (one inside another: the outer first)
        src = ctree.Source(text)
        taken = []
        for f in src.find("field_expression"):
            name = f.child_by_field_name("field")
            mm = _GAP_FIELD.fullmatch(src.at(name)) if name is not None else None
            op = f.child_by_field_name("operator")
            if not mm or op is None or src.at(op) not in ("->", ".") or \
                    any(s <= f.start_byte and f.end_byte <= e for s, e in taken):
                continue
            arg = src.at(f.child_by_field_name("argument"))
            ptr = "uint1 *"
            if distance is not None:
                root = re.match(r"\(*(\w+)", arg)
                d = distance.get(root.group(1)) if root else None
                cast = re.match(r"\(+\s*\w+\s*(__far\s*)?\*\s*\)", arg)    # (a cast says its own distance)
                if cast:
                    d = "far" if cast.group(1) else "near"
                if d is None:
                    continue                                 # (a base of unknown distance: as it was)
                ptr = "uint1 __far *" if d == "far" else "uint1 *"
            if src.at(op) == ".":
                arg = "&(%s)" % arg
            up = f.parent
            if up is not None and up.type == "pointer_expression" and src.at(up.child_by_field_name("operator")) == "&":
                src.replace(up, "((%s)(%s) + 0x%s)" % (ptr, arg, mm.group(1)))
                taken.append((up.start_byte, up.end_byte))
            else:
                src.replace(f, "(*((%s)(%s) + 0x%s))" % (ptr, arg, mm.group(1)))
                taken.append((f.start_byte, f.end_byte))
        if not taken:
            break
        text = src.text()
    return text


def _code_pointers(text, functions):
    """`p = sub_401000;` -- a function's address stored in a pointer the core typed loosely:
    `p = (code *)sub_401000;` (where the statement ends its line)."""
    src = ctree.Source(text)
    for n in src.find("assignment_expression", "init_declarator"):
        v = n.child_by_field_name("right" if n.type == "assignment_expression" else "value")
        if v is not None and v.type == "identifier" and src.at(v) in functions and \
                src.data[v.end_byte:v.end_byte + 1] == b";" and src.data[v.end_byte + 1:v.end_byte + 2] in (b"\n", b"\r", b"") \
                and src.data[v.start_byte - 2:v.start_byte] == b"= ":
            src.insert(v.start_byte, "(code *)")
    return src.text()


def _numbers(src, under=None):
    """The number literals of the text (not inside strings or comments)."""
    return src.find("number_literal", under=under)


def compilable32(module_path, funcs, outdir):
    """-> (preamble, {name: text}, [names], {va: name}, [import names used]) for a whole-module build:
    the core's output with its base types defined, its globals bound to the original's addresses,
    and every function and import declared before use with its real convention."""
    from codebreaker.symbols import api_arguments as apiarity
    from codebreaker.build import watcom as B
    from codebreaker.decompile import decompiler
    from codebreaker.decompile import describe
    # (the L2 documents kept, and each function's C written from its own here: with the module's
    # facts, the back end's passes of the 32-bit path among them -- backends/c.FLAT)
    got = decompiler.raw(module_path, funcs, outdir, l2=True, write=False)
    out, order, by_va, _err = got
    imports = dict(B.import_map(module_path))
    # imports by number, under the names the description gave them: declared and linked too
    numbered = describe.BY_NUMBER.get(module_path) or {}
    imports.update({n: stem for n, (stem, _number, _pop) in numbered.items() if n not in imports})
    sizes, in_ecx, in_edx = {}, set(), set()
    own = {}                                  # function -> address of a global -> bytes of its uses there
    struct_notes = []                         # the structures the decompiler recovered (its facts)
    for n in order:
        facts = got.notes.get(n) or {}
        struct_notes += facts.get("structs") or []
        for va, size, _elem in facts.get("globals") or ():
            sizes.setdefault(va, set()).add(size)
            own.setdefault(n, {}).setdefault(va, set()).add(size)
        written = {}                              # (what the back end found of the function as it wrote it)
        if facts.get("l2") is not None:
            out[n] = decompiler.c_from_l2(out[n], facts, None, written)
        # (a comment the core put before the function -- "WARNING: Removing unreachable block" --
        # is not part of it: left in, it became the function's declaration and broke the header)
        t = _without_notes(out[n].strip())
        if not facts.get("from_l2"):
            # (C the back end declined -- an Unsupported node -- is the printed C, and has those
            # of its passes the 32-bit path shares done here: `x._1_2_` is the core's spelling
            # of "2 bytes of x starting at byte 1")
            t = _null_is_zero(_pieces(t, _BY_SIZE, "uint4"))
        t = _compare_as_numbers(_keep_frame(t, _struct_sizes(struct_notes)))
        # a function's address stored in a pointer the core typed loosely
        t = _code_pointers(t, set(order))
        t = _struct_gaps(t)                      # an offset of a recovered structure with no field
        # every function returns a value in EAX whether or not the core saw it used; the compiler
        # used here has no word for "first argument in ECX": the function is declared plainly, and
        # a #pragma in the header says where its first argument is (both the back end's for C
        # written from L2: backends/c._result_shown, _register_arguments)
        if written.get("registers") == "ecx":
            in_ecx.add(n)
        elif written.get("registers") == "edx":
            in_edx.add(n)
        elif facts.get("from_l2"):
            pass
        elif t.startswith("void") and t[4:5].isspace():
            t = "int" + t[4:]
        if facts.get("from_l2"):
            pass
        elif "__thiscall" in t.split("{", 1)[0]:
            head, brace, body = t.partition("{")
            t = head.replace("__thiscall ", "").replace("__thiscall", "") + brace + body
            in_ecx.add(n)
        elif "__fastcall" in t.split("{", 1)[0]:             # ... or its first two in ECX and EDX
            head, brace, body = t.partition("{")
            t = head.replace("__fastcall ", "").replace("__fastcall", "") + brace + body
            in_edx.add(n)
        out[n] = t
    text = "\n".join(out.values())
    macros = []
    underscored = {"_" + "%sRam%s" % g for t in out.values() for g in
                   (m2.groups() for m2 in (re.fullmatch(r"_([a-z]+)Ram([0-9a-f]{8})", n2) for n2 in _identifiers(t)) if m2)}
    # a global the code calls through holds a function pointer, whatever else it is compared with
    called_through = set().union(*(_called_through(t) for t in out.values())) if out else set()
    # A global the decompiler named as a number but reads through (`*iRam..`, `iRam..[n]`) is
    # read through as a pointer THERE; it stays a number everywhere else, where it is added to
    # and compared as one. (Declaring it a pointer would change what `iRam.. + 4` means.)
    kind = {"i": "int4", "u": "uint4", "x": "xunknown4"}

    def through(src):
        for n in src.find("identifier"):
            name = src.at(n)
            if not _RAM.fullmatch(name) or name in called_through or name[0] not in kind:
                continue
            up = n.parent
            if up.type == "pointer_expression" and src.at(up.child_by_field_name("operator")) == "*":
                src.replace(n, "(%s *)%s" % (kind[name[0]], name))
            elif up.type == "subscript_expression" and up.child_by_field_name("argument") == n:
                src.replace(n, "((%s *)%s)" % (kind[name[0]], name))
    for n in order:
        out[n] = ctree.rewrite(out[n], through)
    text = "\n".join(out.values())
    for pre, addr in sorted(set().union(*(_globals(t) for t in out.values())) if out else ()):
        szs = sizes.get(int(addr, 16)) or {4}
        size = min(szs) if len(szs) > 1 and pre[0] in "cb" else max(s for s in szs if s <= 8) if any(s <= 8 for s in szs) else 4
        # `pc` is the decompiler's prefix for a pointer to char; a global is a code pointer
        # when something calls through it
        ty = "code *" if "%sRam%s" % (pre, addr) in called_through else "char *" if pre == "pc" else (
            "int%d" % size if pre[0] == "i" else _BY_SIZE.get(size, "uint4"))
        if pre[0] == "p" and len(pre) > 1 and ty != "code *":
            ty = _ram_type(pre, size, False)             # a pointer, by the decompiler's own naming (piRam: int *)
        macros.append("#define %sRam%s (*(%s *)0x%s)" % (pre, addr, ty, addr))
        if ("_%sRam%s" % (pre, addr)) in underscored:
            # (the core's name for the same place read at another width: `_uRam0040c000`)
            macros.append("#define _%sRam%s (*(%s *)0x%s)" % (pre, addr, ty, addr))
        # A function that uses the place at one width other than the declared one (a byte
        # stored where other functions read a word) uses it at its own width: the declared
        # width would store, or read, the bytes beside it too.
        if ty in _BY_SIZE.values() or re.fullmatch(r"int\d", ty):
            name = "%sRam%s" % (pre, addr)
            for n in order:
                mine = own.get(n, {}).get(int(addr, 16))
                if mine and len(mine) == 1 and min(mine) != size and min(mine) in _BY_SIZE:
                    w = min(mine)
                    alias = "%s_%d" % (name, w)
                    out[n] = re.sub(r"(?<![A-Za-z0-9_])%s\b" % name, alias, out[n])
                    m = "#define %s (*(%s *)0x%s)" % (alias, "int%d" % w if pre[0] == "i" else _BY_SIZE[w], addr)
                    if m not in macros:
                        macros.append(m)
    text = "\n".join(out.values())
    # a pointer-typed global compared with a number the decompiler cast to an integer
    # (`(uint4)pcVar3 != pcRam004094b0`, `(uint4)piVar3 == piRambff4b328`) is compared as the
    # same integer, whatever it points to
    pointers = [m.group(1) for m in re.finditer(r"#define (\w+Ram[0-9a-f]{8}) \(\*\([^()]*\* \*\)", "\n".join(macros))]
    if pointers:
        pset = set(pointers)

        def same_cast(src):
            for b in src.find("binary_expression"):
                if b.child_by_field_name("operator").type not in _COMPARISONS:
                    continue
                left, right = b.child_by_field_name("left"), b.child_by_field_name("right")
                for g, other in ((right, left), (left, right)):
                    if g.type == "identifier" and src.at(g) in pset and other.type == "cast_expression":
                        ty = src.at(other.child_by_field_name("type"))
                        if re.fullmatch(r"u?int\d", ty) and re.fullmatch(r"[\w.>\-]+", src.at(other.child_by_field_name("value"))):
                            src.insert(g.start_byte, "(%s)" % ty)
                            break
        for n in order:
            out[n] = ctree.rewrite(out[n], same_cast)
        text = "\n".join(out.values())
    called = set().union(*(_called(t) for t in out.values())) if out else set()
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
        elif n in in_edx:
            decls.append('#pragma aux %s "_*" parm routine [ecx] [edx] value [eax] modify [eax ecx edx];' % n)
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
    # A module that is itself a C runtime (crtdll, msvcrt, mfc40, opengl32 ...) has functions of
    # its own called sqrt, ldexp, floor: its declarations are the ones that hold, and the
    # helpers' library prototypes of the same names step aside (a clash broke the whole header).
    own = {d.split("(")[0].split()[-1].lstrip("*") for d in decls if "(" in d}
    helpers = re.sub(r"double (%s)\([^)]*\); ?" % "|".join(_LIBM), lambda m: "" if m.group(1) in own else m.group(0),
                     _X86_HELPERS) if own & set(_LIBM) else _X86_HELPERS
    structs = _struct_decls(struct_notes, _TYPES + "\n".join(odd))
    preamble = (_TYPES + "\n".join(odd) + "\n" + "".join(t + "\n" for t in structs) + _HELPERS + helpers +
                "\n".join(decls) + "\n" + "\n".join(macros) + "\n")
    from codebreaker.decompile import struct_text
    preamble, out = struct_text.apply(module_path, preamble, out, by_va)       # `p->field` for `*(T *)(p + N)`
    return preamble, out, order, by_va, used, in_ecx, in_edx


HEADER = """typedef long long int8; typedef unsigned long long uint8; typedef unsigned long long xunknown8;
typedef double float8; typedef long double float10;
typedef unsigned long unkbyte3; typedef long unkint3; typedef unsigned long unkuint3; typedef long int3;
typedef unsigned long uint3; typedef unsigned long long unkbyte6; typedef long double unkbyte10;
typedef signed char int1; typedef short int2; typedef long int4;
typedef unsigned char uint1; typedef unsigned short uint2; typedef unsigned long uint4;
typedef unsigned char xunknown1; typedef unsigned short xunknown2; typedef unsigned long xunknown4;
typedef unsigned char undefined; typedef unsigned char undefined1; typedef unsigned short undefined2;
typedef unsigned long undefined4; typedef unsigned char bool; typedef unsigned long float4;
typedef int2 __cdecl code();                    /* (a convention named: Watcom's own would pass the arguments in registers) */
typedef int2 __stdcall code_callee_pops();      /* a call through a pointer whose callee removes its arguments */
#define true 1
uint2 codebreaker_ss(void);
#pragma aux codebreaker_ss = "mov ax,ss" value [ax] modify exact [ax];
uint2 codebreaker_ds(void);
#pragma aux codebreaker_ds = "mov ax,ds" value [ax] modify exact [ax];
uint2 codebreaker_cs(void);
#pragma aux codebreaker_cs = "mov ax,cs" value [ax] modify exact [ax];
/* the stack pointer as the function was entered (where its return address is): with the
   function's frame (BP pushed, then BP = SP) that is BP+2 */
uint2 codebreaker_entry_sp(void);
#pragma aux codebreaker_entry_sp = "lea ax,[bp+2]" value [ax] modify exact [ax];
/* a word of the caller's stack: the stack segment, at the entry SP plus `off` */
#define codebreaker_stack(T, off) (*(T __based(__segname("_STACK")) *)(codebreaker_entry_sp() + (off)))
#define false 0
#define __stdcall16far __far __stdcall
#define __stdcall16near __near __stdcall
#define __cdecl16far __far __cdecl
#define __cdecl16near __near __cdecl
#ifdef __WATCOMC__
/* Microsoft's _fastcall (describe: a near function reading AX, DX or BX before it writes them):
   its arguments in AX, DX, BX, its result where Watcom puts one too (AL, AX, DX:AX: a `value
   [ax]` here made every long-returning one "invalid return register"), and the registers
   Microsoft C lets a function overwrite. Watcom has no keyword for it: each such function is
   named in a pragma after its declaration. */
#pragma aux codebreaker_fastcall "_*" parm caller [ax] [dx] [bx] modify [ax bx cx dx es];
#define __fastcall16near __near
#endif
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

# ---------------------------------------------------------------------------------------------
# Data kept in a code segment, read through CS
# ---------------------------------------------------------------------------------------------
# KEYBOARD's GetKeyNameText looks a key up in tables beside its own code (9:0261, 9:02E9 ...)
# and reads them through CS: `MK_FP(unaff_CS, pcVar11)` with pcVar11 set to 0x261. The rebuilt
# code segment is laid out by the compiler -- functions rebuilt from C are other sizes, some are
# moved -- so CS:0261 is not the table any more, and in a function judged on its own it is
# whatever the throw-away build has there. The source such a module was written from named the
# table, in the code segment (`static char _based(_segname("_CODE")) ...`): so does this. The
# bytes a function's C reads through CS are the original's, in a structure in its own code
# segment, codebreaker_code_<n> (one member at each place the code starts reading from), and
# each number that is such a place is written as the member's offset.
_CS_RETURNS = ("ret", "retf", "iret", "iretd", "hlt")


def _code_bytes(image, starts):
    """Which bytes of a code segment are its functions' instructions: each function followed
    from its start, every branch it can take. -> bytearray, 1 at a byte of code. (A place only a
    jump table reaches is taken for data -- its bytes copied as data is all that comes of it.)"""
    import capstone
    from codebreaker.decompile.describe import decoder16
    md = decoder16()
    code, seen = bytearray(len(image)), set()
    todo = [s for s in starts if 0 <= s < len(image)]
    while todo:
        a = todo.pop()
        while 0 <= a < len(image) and a not in seen:
            one = next(md.disasm_lite(image[a:a + 16], a), None)
            if one is None:
                break
            _at, size, mn, ops = one
            seen.add(a)
            code[a:a + size] = b"\x01" * size
            if mn in _CS_RETURNS:
                break
            target = re.fullmatch(r"0x([0-9a-f]+)|(\d+)", ops.strip())
            if (mn.startswith("j") or mn.startswith("loop") or mn == "call") and target:
                todo.append(int(target.group(1), 16) if target.group(1) else int(target.group(2)))
            if mn in ("jmp", "ljmp"):
                break
            a += size
    return code


def _data_runs(code):
    """[(start, end)] of the bytes of a code segment that are not instructions"""
    runs, k = [], 0
    while k < len(code):
        if code[k]:
            k += 1
            continue
        s = k
        while k < len(code) and not code[k]:
            k += 1
        runs.append((s, k))
    return runs


def _string_literal(chunk):
    """The bytes as a C string -- its own NUL the last of them -- when they are text (names, NUL
    between them), else None"""
    if len(chunk) < 2 or chunk[-1] != 0 or sum(32 <= c < 127 for c in chunk) * 2 < len(chunk):
        return None
    if any(c and not 32 <= c < 127 and c not in (9, 10, 13) for c in chunk):
        return None
    return '"%s"' % "".join({0: "\\000", 9: "\\t", 10: "\\n", 13: "\\r", 34: '\\"', 92: "\\\\", 63: "\\?"}.get(c) or chr(c)
                            for c in chunk[:-1])


def _rows(items, per):
    """`items` joined by commas, `per` to a line (an initialiser's elements)"""
    if len(items) <= per:
        return ", ".join(items)
    return "\n    " + ",\n    ".join(", ".join(items[k:k + per]) for k in range(0, len(items), per)) + "\n  "


def _code_segment_data(reports, where, d, module_path):
    """Each number in the C that is a place in a code segment's data, read through CS, written as
    that place in the segment's codebreaker_code_<n> (above): from what writing each function
    found (`reports` {function: backends/c._code_segment_reads' facts["code_reads"]}, in address
    order; `where` {function: (segment, offset)}). -> (header declarations, {segment: the C
    defining its structure}, {segment: {"read": places, "left": [why]}}, {function: the facts it
    is written again with: "code_places" {its number's node: the member}})"""
    from codebreaker.build import data as data16
    from codebreaker.formats.ne import NEFile
    found = {name: (r, where[name][0]) for name, r in reports.items() if r and where.get(name)}
    if not found:
        return [], {}, {}, {}
    nef = NEFile(module_path)
    segs = {s["idx"]: s for s in nef.segments()}
    starts = {}
    for (sg, off) in d.where.values():
        starts.setdefault(sg, []).append(off)
    runs, relocated = {}, {}
    for sg in {s for _g, s in found.values()}:
        image = d.images.get(sg, b"")
        runs[sg] = _data_runs(_code_bytes(image, starts.get(sg, ())))
        s = segs.get(sg)
        relocated[sg] = sorted(off for off, _src, _add, _r in data16._places(nef, s, bytes(nef.data[
            s["file_off"]:s["file_off"] + s["length"]]))) if s and s["file_off"] else []
    import bisect

    def run_of(sg, v):
        rs = runs.get(sg, [])
        k = bisect.bisect_right(rs, (v, 0x10000)) - 1
        return rs[k] if k >= 0 and rs[k][0] <= v < rs[k][1] else None
    report = {}                                              # segment -> {"read", "named", "left", "placed"}
    per = {}                                                 # segment -> {name: (src, places, kinds)}
    each_read = {}                                           # segment -> [the places of each read]
    for name, (rep, sg) in found.items():
        inside = lambda v, sg=sg: run_of(sg, v) is not None
        places = {k: (k, v) for k, v in rep["places"].items() if inside(v)}
        kinds = {}
        for kind, (_k, v) in rep["events"]:                 # (in the order the reads found them)
            if not inside(v):
                continue
            if kind == "call":
                kinds[v] = "call"
            else:
                kinds.setdefault(v, kind)
        each = [{v for _k, v in got if inside(v)} for got in rep["each"]]
        unknown, unsafe = rep["unknown"], rep["unsafe"]
        r = report.setdefault(sg, {"read": 0, "named": 0, "left": []})
        r["read"] += rep["reads"]
        if unsafe:                                           # (a table of places it cannot write as places)
            r["left"].append("%s: an offset it reads through CS is a %s" % (name, unsafe[0]))
            continue
        per.setdefault(sg, {})[name] = (None, places, kinds)
        each_read.setdefault(sg, []).extend(each)
        if not places:
            r["left"].append("%s: its offsets through CS come from %s" % (
                name, ", ".join(sorted(set(unknown))) or "nothing it sets to a place in the data"))
    decls, defs, plan = [], {}, {}
    for sg in sorted(per):
        image = d.images.get(sg, b"")
        bases, kinds = set(), {}
        for _src, places, k in per[sg].values():
            bases |= {v for _t, v in places.values()}
            for v, kind in k.items():
                if kinds.get(v) != "call":
                    kinds[v] = kind
        calls = {v for v, k in kinds.items() if k == "call"}
        if calls:                                            # (its addresses would be the original's)
            report[sg]["left"].append("tables of the code's own addresses, called: %s" % ", ".join(
                "%04x" % v for v in sorted(calls)))
        bases -= calls
        # what each table of places holds: words, each a place in the data, up to the first of
        # those places that is after it (the strings it points at, after the table)
        pointers = {}
        for b in sorted(v for v in bases if kinds.get(v) == "pointers"):
            run = run_of(sg, b)
            later = [x for x in bases if x > b]
            limit, at, got = min([run[1]] + later), b, []
            while at + 2 <= limit and at < min([t for t in got if t > b] + [0x10000]):
                v = struct.unpack_from("<H", image, at)[0]
                if not run_of(sg, v) or v in calls:
                    break
                got.append(v)
                at += 2
            if got:
                pointers[b] = got
        # the structure: each run of data a place is in, whole (a read may go back from the place:
        # VGA's sub_5_027E reads from 40h down), a member at each place
        spans = {run_of(sg, v): run_of(sg, v)[0] for v in bases | {t for got in pointers.values() for t in got}}
        if not spans:
            continue
        while True:
            cuts = set()
            targets = {t for got in pointers.values() for t in got}
            for run, lo in spans.items():
                cuts |= {lo, run[1]} | {v for v in bases | targets if lo <= v < run[1]}
                cuts |= {b + 2 * len(got) for b, got in pointers.items() if lo <= b < run[1]}
            # (a table some place points into the middle of is bytes after all)
            torn = [b for b, got in pointers.items() if any(b < c < b + 2 * len(got) for c in cuts)]
            if not torn:
                break
            for b in torn:
                del pointers[b]
        member_at, members = {}, []
        for run, lo in sorted(spans.items()):
            edges = sorted(c for c in cuts if lo <= c <= run[1])
            for a, b in zip(edges, edges[1:]):
                members.append((a, b))
                member_at[a] = "at_%04x" % a
        struct_name = "codebreaker_code_%d" % sg

        def place(v):
            k = max(a for a, _b in members if a <= v)
            return "%s.%s" % (struct_name, member_at[k]) if k == v else "&%s.%s[%d]" % (struct_name, member_at[k], v - k)
        # (a place in a member the loader writes into -- a selector, a far pointer kept there -- is
        # left as it was: the bytes copied would be the made-up values the description gave them)
        fixed = {a for a, b in members if any(a <= r < b for r in relocated.get(sg, ()))}
        if fixed:
            report[sg]["left"].append("data the loader fixes up: %s" % ", ".join("%04x" % a for a in sorted(fixed)))
        ok_place = {v for v in bases if any(a <= v < b and a not in fixed for a, b in members)}
        fields, inits = [], []
        for a, b in members:
            if a in pointers:
                fields.append('  uint1 __based(__segname("_CODE")) *%s[%d];' % (member_at[a], len(pointers[a])))
                inits.append("  {%s}" % _rows([place(v) for v in pointers[a]], 3))
            else:
                fields.append("  uint1 %s[%d];" % (member_at[a], b - a))
                chunk = bytes(image[a:b])
                inits.append("  %s" % (_string_literal(chunk) or "{%s}" % _rows(["0x%02x" % c for c in chunk], 16)))
        where = ", ".join("%04x-%04x" % (lo, run[1]) for run, lo in sorted(spans.items()))
        decls += ["/* the original's bytes of code segment %d (%s) that its C reads through CS */" % (sg, where),
                  "_Packed struct %s {\n%s\n};" % (struct_name, "\n".join(fields)),
                  'extern _Packed struct %s __based(__segname("_CODE")) %s;' % (struct_name, struct_name)]
        defs[sg] = '_Packed struct %s __based(__segname("_CODE")) %s = {\n%s\n};\n' % (
            struct_name, struct_name, ",\n".join(inits))
        for name, (_src, places, _k) in per[sg].items():
            written = {k: "(uint2)%s" % place(v) for k, v in places.values() if v in ok_place}
            if written:
                plan[name] = {"code_places": written}
        report[sg]["placed"] = sorted(ok_place)
        report[sg]["named"] = sum(1 for got in each_read[sg] if got and got <= ok_place)
    return decls, defs, report, plan


_SIZED = {1: "uint1", 2: "uint2", 4: "uint4"}


_SIGNED = {1: "int1", 2: "int2", 4: "int4"}
_ORDERING = ("<", ">", "<=", ">=")


def _pieces(text, sized=None, default="uint2", spell=None):
    """`x._2_2_` is the decompiler's spelling of "2 bytes of x starting at byte 2": the bytes at
    that place, as the type of that size (`spell(x, offset, type)` writes it another way).
    Unsigned, except as one side of an ordering comparison: the core prints its signed compare
    (INT_SLESS) with no cast there, and an unsigned piece made it unsigned -- USER's sub_10_2F28
    `xRam116001bc._2_2_ < a1` took the other branch for a1 = FFFFh. (Against an unsigned other
    side a signed piece of the same size still compares unsigned in C.)"""
    spell = spell or (lambda x, off, ty: "(*(%s *)((char *)&%s + %s))" % (ty, x, off))

    def fix(src):
        for n in src.find("field_expression"):
            m = re.fullmatch(r"_(\d+)_(\d+)_", src.at(n.child_by_field_name("field")))
            if m and src.at(n.child_by_field_name("operator")) == "." and not _has_piece(n.child_by_field_name("argument")):
                table = sized or (_SIGNED if _ordered(src, n) else _SIZED)
                src.replace(n, spell(src.at(n.child_by_field_name("argument")), m.group(1),
                                     table.get(int(m.group(2)), default)))
    before = None
    while before != text:                                # (a piece of a piece: the inner one first)
        before, text = text, ctree.rewrite(text, fix)
    return text


def _ordered(src, n):
    """Is this expression (in parentheses or not) one side of <, >, <= or >=?"""
    up = n.parent
    while up is not None and up.type == "parenthesized_expression":
        up = up.parent
    return up is not None and up.type == "binary_expression" and \
        src.at(up.child_by_field_name("operator")) in _ORDERING


def _has_piece(n):
    todo = [n]
    while todo:
        c = todo.pop()
        f = c.child_by_field_name("field") if c.type == "field_expression" else None
        if f is not None and re.fullmatch(rb"_\d+_\d+_", f.text or b""):
            return True
        todo.extend(c.children)
    return False


def _far_argument_widths(code, start, end, far_fn):
    """{a<n>: bytes} -- how wide the original reads through the far pointer it takes as
    argument n: `les reg, [bp+N]`, then the first `es:[reg...]` through it."""
    import capstone
    from codebreaker.decompile.describe import decoder16
    md = decoder16()
    first = 6 if far_fn else 4                             # (the first argument above the return address)
    out, held = {}, {}
    for ins in md.disasm(bytes(code[start:end]), start):
        mm = re.fullmatch(r"(\w+), ptr \[bp \+ (0x[0-9a-f]+|\d+)\]", ins.op_str) if ins.mnemonic == "les" else None
        if mm:
            n = (int(mm.group(2), 0) - first) // 2 + 1
            if n >= 1:
                held[mm.group(1)] = "a%d" % n
            continue
        for reg, arg in list(held.items()):
            ref = re.search(r"(byte|word|dword) ptr es:\[%s\b" % reg, ins.op_str)
            if ref:
                out.setdefault(arg, {"byte": 1, "word": 2, "dword": 4}[ref.group(1)])
                del held[reg]
            elif re.match(r"\s*%s\s*," % reg, ins.op_str) or ins.mnemonic in ("call", "lcall", "ret", "retf"):
                del held[reg]                               # (the register set again, or gone)
    return out


_FAR_LOAD = re.compile(r"\[bp \+ (0x[0-9a-f]+|\d+)\]")


def _far_loads(code, start, end):
    """The offsets from BP of the argument words a function loads as far pointers: `les bx,
    [bp+6]` takes the word at +6 as an offset and the one above it as its segment."""
    import capstone
    from codebreaker.decompile.describe import decoder16
    md = decoder16()
    found = set()
    for _a, _n, mnemonic, ops in md.disasm_lite(bytes(code[start:end]), start):
        if mnemonic in ("les", "lds"):
            m = _FAR_LOAD.search(ops)
            if m:
                found.add(int(m.group(1), 0))
    return found


def _open_arguments(text):
    """Write the definition in the older style, `f(a, b) int a; int b; { ... }`.

    A definition written with its argument types in the parentheses also fixes how every later
    call must look, and the decompiler recovers each call on its own evidence: a caller that
    passes three words where the definition takes one far pointer and a word is the same bytes on
    the stack, but not the same C. In the older style the definition says what the function
    takes and each call is compiled as written."""
    src = ctree.Source(text)
    plist = src.parameter_list()
    body = src.body()
    if plist is None or body is None or any("(" in p or ")" in p for p in src.parameters()):
        return text
    params = [p for p in src.parameters() if p not in ("", "void")] if src.parameters() != ["void"] else []
    names = []
    for prm in params:
        nm = re.search(r"([A-Za-z_]\w*)\s*(?:\[\d*\])?$", prm)
        if not nm:
            return text
        names.append(nm.group(1))
    decls = "".join("%s;\n" % prm for prm in params)
    before = src.data[:plist.start_byte].decode("utf-8").rstrip()
    return "%s(%s)\n%s%s" % (before, ",".join(names), decls, src.data[body.start_byte:].decode("utf-8"))


def declaration(text):
    """A declaration that fixes the function's convention and result but not its arguments.
    The decompiler works out arguments one call at a time and callers do not always agree with
    the definition; with the argument list left open each call compiles as it was recovered,
    and a call that passes the wrong amount shows up when it is run, not as a compile error
    that hides the whole function."""
    head = " ".join(text.split("{", 1)[0].split())
    return head[:head.index("(")] + "();"


_RAM = re.compile(r"([a-z]+)Ram([0-9a-f]{8})")


def _global_type(prefix, size, elem=None):
    if prefix[0] == "p" and len(prefix) > 1:             # a pointer, by the decompiler's own naming (piRam: int *)
        to = {"i": "int2", "u": "uint2", "c": "char", "b": "bool"}.get(prefix.lstrip("p")[:1], "xunknown2")
        # (px..: a pointer to an unknown of 1, 2 or 4 bytes -- the core says which; `p + 2` counts
        # in those: USER's DisableOEMLayer read [[12Eh]+4] for the original's [[12Eh]+2])
        if elem and len(prefix) - len(prefix.lstrip("p")) == 1:
            letter = prefix.lstrip("p")[:1]
            to = {"x": "xunknown", "i": "int", "u": "uint"}.get(letter, "")
            to = (to + str(elem)) if to else {"c": "char", "b": "bool"}.get(letter, "xunknown%d" % elem)
        stars = "*" * (len(prefix) - len(prefix.lstrip("p")))
        return "%s %s%s" % (to, "__far " if size == 4 else "", stars)
    signed = prefix.startswith("i") or prefix.startswith("c")
    return {1: "int1" if signed else "uint1", 2: "int2" if signed else "uint2", 4: "int4" if signed else "uint4"}.get(
        size, "uint2")


def _loads_dgroup(code, off, dgroup):
    """Whether a function's prologue loads DS with its own data segment: `mov ax, DGROUP`
    then `mov ds, ax`, among its first instructions (before or after `enter`/`push bp`, saves)."""
    import capstone
    from codebreaker.decompile.describe import decoder16
    md = decoder16()
    loaded = False
    for i in md.disasm(bytes(code[off:off + 24]), off):
        if i.mnemonic == "mov" and i.op_str == "ax, 0x%x" % dgroup:
            loaded = True
        elif i.mnemonic == "mov" and i.op_str == "ds, ax":
            return loaded
        elif not (i.mnemonic in ("push", "enter", "inc", "nop", "sub")
                  or (i.mnemonic == "mov" and i.op_str == "bp, sp")):
            return False
    return False


def _selector_sites(module_path):
    """{segment: [(offset, segment whose selector the loader puts there)]} -- the module's own
    selector fix-ups (a selector, or the selector half of a far pointer), chains followed."""
    from codebreaker.formats.ne import NEFile
    nef = NEFile(module_path)
    entries = nef.entries()
    out = {}
    for s in nef.segments():
        if not s["file_off"]:
            continue
        raw = nef.data[s["file_off"]:s["file_off"] + s["length"]]
        for r in nef.relocations(s):
            if r["kind"] != "INTERNAL" or r["src"] not in (2, 3):
                continue
            target = r["seg"]
            if target == 0xFF:                               # a movable segment, through its entry
                e = entries[r["entry"] - 1] if 0 < r["entry"] <= len(entries) else None
                target = e.get("seg") if e else None
            if not target:
                continue
            at, seen = r["offset"], set()
            while at < len(raw) - 1 and at not in seen:
                seen.add(at)
                # (the target of a far call or jump into the segment is not its selector as a
                # value: with one in the function, a plain `push 1000h` became segment 1's)
                opcode = at - (1 if r["src"] == 3 else 3)
                if not (opcode >= 0 and raw[opcode] in (0x9A, 0xEA)):
                    out.setdefault(s["idx"], []).append((at + (2 if r["src"] == 3 else 0), target))
                if r["additive"]:
                    break
                nxt = struct.unpack_from("<H", raw, at)[0]
                if nxt == 0xFFFF:
                    break
                at = nxt
    return out


def _source_stamp(*modules):
    """A digest of the source of the code that makes a result (a change to it makes it again)."""
    from codebreaker.base import cache
    return cache.key(*[open(mod.__file__, "rb").read() for mod in modules])


def generation():
    """The version of what makes a module compilable (compilable16), as the cache keeps its
    results apart (base/cache.py GENERATIONS): the source stamp its keys carry, and the
    decompiler's own generation."""
    import sys
    from codebreaker.decompile import decompiler, describe
    from codebreaker.symbols import api_arguments
    return _source_stamp(sys.modules[__name__], decompiler, describe, api_arguments), decompiler.generation()


def compilable16(module_path, outdir=None):
    """The module made compilable (see _compilable16) -- kept (base/cache.py) by the module's
    bytes, the decompiler and the source of what makes it: the same module built again (a
    hunt builds one many times over) is not decompiled and made again. Its description is
    written where it was asked for, as when it is made."""
    import pickle
    import sys
    from codebreaker.base import cache
    from codebreaker.decompile import decompiler, describe
    from codebreaker.symbols import api_arguments
    with open(module_path, "rb") as f:
        body = f.read()
    from codebreaker.base import settings
    # (the setting struct_recovery too: with structures the C is other C -- without it here, a
    # run with them was handed the C made without; and the reading of the imports' argument
    # sizes, which the description is written with -- a change to it alone handed back C made
    # with the sizes before)
    from codebreaker.translate.backends import c as cback
    from codebreaker.decompile import asm_blocks, inline_asm
    # (and the C back end's source: the C is written from L2)
    k = cache.key(body, _source_stamp(sys.modules[__name__], decompiler, describe, api_arguments, cback),
                  *(["asm blocks", _source_stamp(asm_blocks, inline_asm)] if os.environ.get("CB_ASM_BLOCKS") else []),
                  cache.tool(decompiler._exe()), decompiler._language_stamp(),
                  (settings.get("struct_recovery") or "off").strip().lower(),
                  os.environ.get("CB_SELECTOR_STRUCTS", ""),       # (_selector_structs' off switch)
                  os.environ.get("CB_CODE_DATA", ""),              # (_code_segment_data's off switch)
                  os.environ.get("CB_CALLER_ARGS", ""),            # (describe's direct-call arguments)
                  os.environ.get("CB_FASTCALL", ""),               # (describe's _fastcall functions)
                  os.environ.get("CB_INLINE_HELPERS", ""),         # (describe's helpers made part of their callers)
                  os.environ.get("CB_PROTOTYPES", ""),             # (prototypes where the calls agree)
                  os.environ.get("CB_CALLER_RESULTS", ""))         # (results the callers read DX of)
    had = cache.get("compilable16", k)
    if had is not None:
        m = pickle.loads(had)
        folder = outdir or os.path.dirname(m.description.path)
        os.makedirs(folder, exist_ok=True)
        m.description.write(os.path.join(folder, "description.txt"))     # (from its lines, for the decompiler)
        return m
    m = _compilable16(module_path, outdir)
    cache.put("compilable16", k, pickle.dumps(m))
    return m


def _identifiers(text):
    src = ctree.Source(text)
    return [src.at(n) for n in src.find("identifier")]


_IMAGES16 = {}                   # segment -> its bytes, for _write16 (in each worker: _write16_images)


def _write16_images(images):
    _IMAGES16.clear()
    _IMAGES16.update(images)


_EXPORT_PROLOGUE = (b"\x8c\xd8\x90", b"\x1e\x58\x90")      # mov ax, ds / nop (either encoding)


def module_facts16(code, off, end, distance, model, dgroup, far_return, seg=None):
    """What the module says of a 16-bit function that its C is written with (backends/c.function's
    `facts`): its distance and model in the description; how wide the original reads through each
    far pointer it takes, as a far or a near function ({far: {a<n>: bytes}}); the argument words
    it loads as far pointers; whether it starts with Windows' export prologue (`mov ax, ds; nop`,
    which Windows rewrites to load the module's data segment: the source marked it _export) and
    whether it loads its own data segment whoever calls it (`mov ax, DGROUP / mov ds, ax`: _loadds);
    whether it returns a far pointer; the made-up selector the description gives its own code
    segment (describe.selector), which the decompiler writes for CS; where it stores SS into its
    frame (_ss_store_places)."""
    from codebreaker.decompile import describe
    have = code is not None
    return {"distance": distance, "model": model, "far_return": far_return,
            "selector": describe.selector(seg) if seg is not None else None,
            "widths": {f: _far_argument_widths(code, off, end, f) for f in (True, False)} if have else {},
            "loads": _far_loads(code, off, end) if have else set(),
            "export": have and bytes(code[off:off + 3]) in _EXPORT_PROLOGUE,
            "loadds": have and dgroup is not None and _loads_dgroup(code, off, dgroup),
            "ss_stores": _ss_store_places(code, off, end) if have else []}


def _ss_store_places(code, start, end):
    """Where the function stores SS into its frame, `mov [bp-0Ch], ss` -- the segment half of a
    far pointer it builds there to a local, a store the decompiler drops (backends/c.
    _stack_segment_stores puts it back): [the decompiler's offset of each, from the entry SP]."""
    from codebreaker.decompile.describe import decoder16
    places = []
    for ins in decoder16().disasm(bytes(code[start:end]), start):
        mm = re.fullmatch(r"word ptr \[bp - (0x[0-9a-f]+|\d+)\], ss", ins.op_str) if ins.mnemonic == "mov" else None
        if mm:
            places.append(int(mm.group(1), 0) + 2)
    return places


def _write16(context, item):
    """One function's C written from its L2 document by the C back end (backends/c), with what the
    module says of it (module_facts16) -> (address, text or None where the back end declines, its
    flags in the module, its facts as the back end may have changed them, what its passes found
    for the module: {"interrupts": {(vector, registers)}, "selector_decls": [declaration],
    "selector_shared": {structure: {function: fields}}})."""
    images = context
    va, distance, model, seg, off, end, dgroup, known, by_selector, callee_pops = item[:10]
    from codebreaker.decompile import decompiler
    from codebreaker.translate.backends import c as cback
    far = {n for n, size in ((known or {}).get("pointers") or {}).items() if size == 4}
    facts = module_facts16(_IMAGES16.get(seg), off, end, distance, model, dgroup, "return" in far, seg)
    facts["selector_structs"] = by_selector
    facts["callee_pops"] = bool(callee_pops)
    facts["without_notes"] = True                        # (the module's C: no notes above a function)
    facts.update(item[10] if len(item) > 10 else {})     # (the module's own: "selector_values"; and what it decided of it since: "frame_slots" ...)
    known = dict(known or {})
    text = decompiler.c_from_l2(None, known, images, facts)
    found = {k: facts[k] for k in ("interrupts", "selector_decls", "selector_shared", "frame", "selector_imports",
                                   "code_reads", "names", "heading") if k in facts}
    if not known.get("from_l2"):
        return va, None, set(), known, {}
    return va, text, cback.flags(known["l2"], facts), known, found


def _compilable16(module_path, outdir=None):
    from codebreaker.decompile import decompiler, describe
    texts, order, by_va, d = decompiler.decompile16(module_path, outdir)
    # the module made: one L2 Module (translate/l2.Made), each function a MethodDecl with its C, and
    # what the 16-bit build needs said of the module beside it
    m = l2.Made(os.path.basename(module_path).lower())
    m.notes = d.notes                                        # what the decompiler said of each besides its C
    m.where = {}
    m.near, m.wide, m.novalue = set(), set(), set()
    m.interrupts = set()                                     # (vector, arguments) of the interrupts taken
    m.selector_imports = {}                                  # imports used for their selector
    m.exported_prologue = set()                              # far functions the original starts with Windows' prologue
    sizes = {}                                               # address of a global -> bytes of the access
    struct_notes = []                                        # the structures the decompiler recovered
    selector_structs = []                                    # ... and those a segment holds (_selector_structs)
    by_selector = decompiler._structs(d.path) == "1" and os.environ.get("CB_SELECTOR_STRUCTS") != "0"   # (with structures)
    elems = {}                                               # ... a pointer's -> bytes of what it points at
    ends, starts = {}, {}                                    # where each function's code stops: at the next one
    for va in sorted(d.where):
        starts.setdefault(d.where[va][0], []).append((d.where[va][1], va))
    for seg, lst in starts.items():
        lst.sort()
        for k, (off, va) in enumerate(lst):
            ends[va] = lst[k + 1][0] if k + 1 < len(lst) else len(d.images.get(seg, b""))
    # each function's model as the description gives it: near or far is how it returns, and
    # the C must say it (the decompiler leaves the convention out of a head whose result is a
    # pointer -- and in the medium model an unmarked function is far)
    distance, model_of = {}, {}
    for parts in d.records("F"):
        if len(parts) > 4:
            distance[parts[2]] = "far" if parts[4].endswith("far") else "near" if parts[4].endswith("near") else None
            if parts[4].startswith(("__stdcall16", "__cdecl16")):
                model_of[parts[2]] = parts[4]
    from codebreaker.jobs import pool
    dgroup = describe.selector(d.data_segment) if d.data_segment else None
    images = dict(d.images)
    # (the segments given each worker once, as it starts: sent with every share of the work they
    # were most of what went down the pipes -- a minute of USER's)
    _write16_images(images)                                    # (and here: a short list is done in this process)
    # Each function's C written from its L2 document by the C back end, with what the module says
    # of it (_write16, among worker processes: the back end's passes on the tree, its heading as
    # the description and its instructions give it)
    asm_images = decompiler._segment_images(d.path) if os.environ.get("CB_ASM_BLOCKS") else None
    # the functions whose calls through pointers remove nothing themselves: the callee does (the
    # arguments are listed in stack order, so __stdcall lays them out as the original pushed
    # them; backends/c._callee_pops). Each call is kept by its own place -- the function the
    # description named for it may since have been found to be two: the call belongs to the one
    # that holds it, of every function the module will have (those the decompiler could not make
    # among them)
    sites = [int(parts[1], 16) for parts in d.records("O") if parts[4].startswith("__stdcall")]
    import bisect
    starts_all = sorted(set(by_va) | {int(parts[1], 16) for parts in d.records("F")
                                      if int(parts[1], 16) not in by_va and parts[2] not in texts
                                      and d.where.get(int(parts[1], 16))})
    callee_pops = {}
    for site in sites:
        k = bisect.bisect_right(starts_all, site) - 1
        if k >= 0 and starts_all[k] >> 16 == site >> 16:
            callee_pops[starts_all[k]] = True
    # what the module says that every function is written with: its imports used for their value
    # (describe's selector_imports, the made-up number each is shown as)
    shared = {"selector_values": dict(getattr(d, "selector_imports", {}) or {})}
    items = []
    for va in sorted(by_va):
        name = by_va[va]
        seg, off = d.where[va]
        items.append((va, distance.get(name), model_of.get(name), seg, off, ends.get(va, off), dgroup,
                      d.notes.get(name), by_selector, callee_pops.get(va), shared))
    written = pool.run(_write16, items, context=asm_images, init=_write16_images, initargs=(images,),
                       below_normal=False)
    for item in items:                                       # (a worker that could not start or died
        if written.get(item[0]) is None:                     # -- a calling script with no __main__
            written[item[0]] = _write16(asm_images, item)    # guard -- is done here, as before)
    # A function the decompiler could not make stays a function of the module: a body that says
    # so, which the build replaces by the function's own instructions where it was written in
    # assembly, and otherwise reports as not rebuilt (m.failed). So is one whose C the back end
    # could not write from its L2 -- an Unsupported node, where the core's PrintL2 has no node
    # yet (GDI's sub_22_0B96): the passes the C is made with are the back end's, on the tree.
    m.failed = {}
    flags_of, found_of = {}, {}
    for va, nm in list(by_va.items()):
        _va, text, flags, known, found = written[va]
        m.interrupts |= found.get("interrupts") or set()
        found_of[nm] = found
        d.notes[nm] = known
        if text is None:
            m.failed[nm] = "its C could not be written from L2" if known.get("l2") else "it has no L2"
            del texts[nm], by_va[va]
        else:
            texts[nm], flags_of[nm] = text, flags
    for parts in d.records("F"):
        va, nm = int(parts[1], 16), parts[2]
        if va not in by_va and nm not in texts and d.where.get(va):
            model = parts[4] if len(parts) > 4 and parts[4] != "inline" else "__cdecl16near"
            seg, off = d.where[va]
            facts = module_facts16(images.get(seg), off, ends.get(va, off), None, None, dgroup, False, seg)
            far = "__stdcall16far" in model or "__cdecl16far" in model
            words = [model] + (["__export"] if far and facts["export"] else []) + (["__loadds"] if facts["loadds"] else [])
            texts[nm] = ("xunknown2 %s %s()\n{\n  /* the decompiler could not make this function */\n"
                         "  return 0;\n}" % (" ".join(words), nm))
            head = "xunknown2 %s %s" % (" ".join(words), nm)          # (its heading, as the back end reports one)
            found_of[nm] = {"heading": {"params": (head, []), "types": {}, "calls": [], "declaration": head + "();",
                                        "fastcall": "__fastcall16near" in head}}
            flags_of[nm] = ({"exported_prologue"} if far and facts["export"] else set()) | (set() if far else {"near"})
            by_va[va] = nm
            m.failed.setdefault(nm, "the decompiler could not make it")
    # helpers that work in their caller's frame: its places passed to them -- what the module
    # decides from what writing each function found (backends/c.caller_frame_plan), and the
    # functions it names written again with it
    frames = {}
    for va, nm in by_va.items():
        seg, off = d.where.get(va) or (None, None)
        frames[nm] = seg in d.images and _keeps_frame(d.images[seg], off)
    plan = cback.caller_frame_plan({nm: (found_of.get(nm) or {}).get("frame") for nm in texts}, frames)
    # ... and the module's own segments as values, where the function's code holds a loader fix-up
    # for one (backends/c._segment_values: which function is far is known once each is written)
    near = {nm for nm, fl in flags_of.items() if "near" in fl}
    for nm, extra in cback.segment_plan(sorted(by_va.items()), near, d.where, _selector_sites(module_path),
                                        getattr(d, "code", {}), d.data_segment,
                                        set(getattr(d, "images", {})) - set(getattr(d, "code", {})) - {d.data_segment},
                                        describe.FIRST_SELECTOR).items():
        plan.setdefault(nm, {}).update(extra)
    # ... and the data a function reads through CS from beside its code: by name, in its own code
    # segment (backends/c._code_segment_reads finds the numbers, _code_segment_data lays it out)
    code_decls, code_data, code_reads, code_plan = _code_segment_data(
        {nm: (found_of.get(nm) or {}).get("code_reads") for _va, nm in sorted(by_va.items())},
        {nm: d.where.get(va) for va, nm in by_va.items()}, d, module_path) \
        if os.environ.get("CB_CODE_DATA") != "0" else ([], {}, {}, {})
    for nm, extra in code_plan.items():
        plan.setdefault(nm, {}).update(extra)
    # ... and the structures of global selectors: one for the module where every function's fields
    # agree, else each function's own -- its uses then written by that name (backends/c
    # ._selector_structs, given "selector_renames")
    shared_selectors = {}
    for _va, nm in sorted(by_va.items()):
        for k, fields in ((found_of.get(nm) or {}).get("selector_shared") or {}).items():
            shared_selectors.setdefault(k, {}).update(fields)
    shared_decls, renames = _shared_selector_structs(shared_selectors)
    for nm, names in renames.items():
        plan.setdefault(nm, {})["selector_renames"] = names
    item_of = {by_va[item[0]]: item for item in items if item[0] in by_va}
    for nm, extra in plan.items():
        if nm in item_of:
            _va, text, _flags, _known, _found = _write16(asm_images, item_of[nm][:10] + (dict(shared, **extra),))
            if text is not None:
                texts[nm], found_of[nm] = text, _found
    m.l2 = {n: f["l2"] for n, f in d.notes.items() if f.get("l2") is not None}   # each one's L2 document
    for va in sorted(by_va):
        name = by_va[va]
        text = texts[name].strip()
        found = found_of.get(name) or {}
        known = d.notes.get(name) or {}
        struct_notes += known.get("structs") or []
        for a, size, elem in known.get("globals") or ():
            sizes[a] = max(sizes.get(a, 0), size) if size <= 4 else sizes.get(a, 2)
            if elem in (1, 2, 4):
                elems[a] = elem
        for flag in flags_of.get(name, ()):
            getattr(m, flag).add(name)
        selector_structs += found.get("selector_decls") or []
        m.add(name, text, va)
        m.where[va] = d.where.get(va)
        # (an import's value used bare is the import: backends/c._selector_imports)
        m.selector_imports.update(found.get("selector_imports") or {})
    other_data = set(getattr(d, "images", {})) - set(getattr(d, "code", {})) - {d.data_segment}
    m.code_data, m.code_reads = code_data, code_reads
    # what the functions name and call, as each was written (backends/c._names_used)
    used = [(found_of.get(n) or {}).get("names") or {} for n in m.order]
    called = set().union(*(u.get("called", ()) for u in used)) if used else set()
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
    for sg in other_data:                                    # (a segment's selector used as a value)
        if any(sg in u.get("segments", ()) for u in used):
            m.segments[sg] = d.images[sg]
    # a global the code calls through holds a function pointer -- near or far by its size
    called_through = set().union(*(u.get("called_through", ()) for u in used)) if used else set()
    for prefix, addr in sorted(set().union(*(u.get("globals", ()) for u in used)) if used else ()):
        va = int(addr, 16)
        seg, off = describe.where(va)
        ty = _global_type(prefix, sizes.get(va, 2), elems.get(va))
        if "%sRam%s" % (prefix, addr) in called_through:
            ty = "code __far *" if sizes.get(va, 2) == 4 else "code __near *"
        if seg == d.data_segment or (va >> 16) == 0:
            # (segment 0: an address the core folded to a constant -- a table index after it
            # copied a shared tail, LZEXPAND's sub_11_00AB `cRam00000095` -- an offset through
            # DS like the rest; it was declared nowhere and the C did not compile)
            macros.append("#define %sRam%s (*(%s *)0x%x)" % (prefix, addr, ty, va & 0xFFFF if (va >> 16) == 0 else off))
        elif (va >> 16) == 0x40:
            # through the selector 0040h itself: the BIOS's data, which Windows keeps at that selector
            macros.append("#define %sRam%s (*(%s __far *)MK_FP(0x40, 0x%x))" % (prefix, addr, ty, va & 0xFFFF))
            m.bios_data = True
        elif (va >> 16) in getattr(d, "selector_imports", {}):
            # a place in a segment the module gets as an import (KERNEL's __0040H: the BIOS data):
            # the import's selector, which the loader fills in, and the offset
            imp, how = d.selector_imports[va >> 16]
            m.selector_imports[imp] = how
            macros.append("#define %sRam%s (*(%s __far *)MK_FP(%s(&%s), 0x%x))" % (
                prefix, addr, ty, "FP_SEG" if how == "segment" else "FP_OFF", imp, va & 0xFFFF))
        elif seg in getattr(d, "code", {}) and any(
                m.where.get(v, (None,))[0] == seg and n not in m.near for v, n in m.by_va.items()):
            # in one of the module's code segments (a variable kept beside the code): that
            # segment itself, by the selector of a far function in it -- not a copy of its bytes,
            # which the code there would never see written
            far = min(n for v, n in m.by_va.items() if m.where.get(v, (None,))[0] == seg and n not in m.near)
            macros.append("#define %sRam%s (*(%s __far *)(((uint4)(uint2)((uint4)(void __far *)&%s >> 16) << 16)"
                          " | 0x%x))" % (prefix, addr, ty, far, off))
        elif seg in d.images and off < len(d.images[seg]):
            m.segments[seg] = d.images[seg]
            macros.append("#define %sRam%s (*(%s __far *)(codebreaker_segment_%d + 0x%x))" % (prefix, addr, ty, seg, off))
    # (data.c, compiled with CODEBREAKER_DATA, defines these segments with their pointers in
    # place: the declaration as bytes is for the code)
    decls = ['#ifndef CODEBREAKER_DATA\nextern uint1 __based(__segname("CODEBREAKER_SEGMENT_%d")) codebreaker_segment_%d[];\n#endif'
             % (n, n) for n in sorted(m.segments)]
    if m.selector_imports:                                   # imports used for their value
        decls.append("#define MK_FP(s, o) ((void __far *)(((uint4)(uint2)(s) << 16) | (uint2)(o)))")
        decls.append("#define FP_SEG(p) ((uint2)((uint4)(void __far *)(p) >> 16))")
        decls.append("#define FP_OFF(p) ((uint2)(uint4)(void __far *)(p))")
        decls += ["extern uint1 __far %s;  /* an import: its %s is the value */" % (n, m.selector_imports[n])
                  for n in sorted(m.selector_imports)]
        for n in m.selector_imports:
            mm = re.fullmatch(r"(.+)_(\d+)", n)
            if mm:
                m.imports[n] = (mm.group(1), int(mm.group(2)))
    decls += ["extern uint4 __far __stdcall %s();" % nm for nm in sorted(m.imports) if nm not in m.selector_imports]
    # each function declared: with its parameters where every call agrees with its definition
    # (backends/c.prototypes, from each one's heading and calls as the back end wrote them)
    headings = {n: (found_of.get(n) or {}).get("heading") or {} for n in m.order}
    agreed = cback.prototypes(headings, m.order) if os.environ.get("CB_PROTOTYPES", "1") != "0" else {}
    m.prototyped = set(agreed)
    decls += [agreed.get(n) or headings[n]["declaration"] for n in m.order]
    decls += ["#ifdef __WATCOMC__\n#pragma aux (codebreaker_fastcall) %s;\n#endif" % n for n in m.order
              if headings[n]["fastcall"]]
    intrinsics = [INTRINSICS16[n] for n in sorted(INTRINSICS16) if n in called]
    intrinsics += interrupt_helpers(m.interrupts)
    if (any(u.get("mk_fp") for u in used) or getattr(m, "bios_data", False)) and not m.selector_imports:
        intrinsics.append("#define MK_FP(s, o) ((void __far *)(((uint4)(uint2)(s) << 16) | (uint2)(o)))")
    selector_structs += shared_decls
    if selector_structs:
        intrinsics.append(SEG_PTR)
    m.header = HEADER + "\n" + "\n".join(intrinsics + macros + _struct_decls(struct_notes, HEADER, True) +
                                          selector_structs + code_decls + decls) + "\n"
    m.description = d
    return m


# Software interrupts: the C back end writes each call of int<n> as its helper, with the registers
# set for it (backends/c._interrupts); the header declares the helpers (interrupt_helpers) and the
# build writes each vector's routine (build/module.py, interrupt_routines).
INTERRUPT_REGISTERS = ("ax", "bx", "cx", "dx", "si", "di", "ds", "es")


def _keeps_frame(code, off):
    """Does the function at `off` set up BP as its frame -- Windows' far prologue, `push bp /
    mov bp, sp`, or `enter`?"""
    from codebreaker.decompile import describe
    head = bytes(code[off:off + 3])
    return describe.windows_far_prologue(code, off) or head in (b"\x55\x8b\xec", b"\x55\x89\xe5") or head[:1] == b"\xc8"


def _declaration_lines(text):
    """The lines of the function's body that are a declaration and nothing else, as written --
    what a pattern for `  T name;` may take apart (it took `  return uStack_6;` for one, of type
    `return`)."""
    src = ctree.Source(text)
    return {src.line(n) for n in src.find("declaration", under=src.body()) if src.at(n) == src.line(n).strip()}


def interrupt_helper_name(vector, regs):
    return "int%02x_%s" % (vector, "_".join(regs) if regs else "none")


SEG_PTR = "#define SEG_PTR(T, s) ((T __far *)((uint4)(uint2)(s) << 16))   /* a structure at the start of a segment */"


def _shared_selector_structs(shared):
    """The structures of global selectors (backends/c._selector_structs' `selector_shared`): one
    per global for the whole module -- the fields every function saw, together -- where they agree
    (one type per offset, none overlapping); where they do not, each function's own, as for a
    local selector. -> (declarations, {function: {the module's name: its own}})"""
    out, renames = [], {}
    for name, per in sorted(shared.items()):
        every = {}
        for fields in per.values():
            for off, ty in fields.items():
                if every.setdefault(off, ty) != ty:
                    every = None
                    break
            if every is None:
                break
        body = cback.seg_layout(every) if every is not None else None
        if body is not None:
            out.append("typedef _Packed struct %s { %s } %s;" % (name, " ".join(body), name))
            continue
        for fname, fields in sorted(per.items()):
            own = "seg_%s_%s" % (re.sub(r"\W", "_", fname), name[4:])
            out.append("typedef _Packed struct %s { %s } %s;" % (own, " ".join(cback.seg_layout(fields)), own))
            renames.setdefault(fname, {})[name] = own
    return out, renames


def interrupt_helpers(used):
    """The header's part for the interrupts a module takes: the register record, each vector's
    routine, and a helper per vector and argument count."""
    if not used:
        return []
    out = ["struct codebreaker_registers { uint2 ax, bx, cx, dx, si, di, ds, es; uint1 cf, zf; uint2 set; };",
           "extern struct codebreaker_registers codebreaker_int;   /* as the last interrupt left them */"]
    out += ["#define int_%s codebreaker_int.%s" % (r.upper(), r) for r in INTERRUPT_REGISTERS + ("cf", "zf")]
    for r in ("ax", "bx", "cx", "dx"):                     # and their bytes: AL AH, BL BH ...
        out += ["#define int_%sL (*(uint1 *)&codebreaker_int.%s)" % (r[0].upper(), r),
                "#define int_%sH (*((uint1 *)&codebreaker_int.%s + 1))" % (r[0].upper(), r)]
    for v in sorted({v for v, _k in used}):
        out.append("void __far codebreaker_int%02x(void);              /* INT %02Xh */" % (v, v))
    for v, regs in sorted(used):
        params = ", ".join("uint2 %s" % r for r in regs) or "void"
        sets = " ".join("codebreaker_int.%s = %s;" % (r, r) for r in regs)
        mask = sum(1 << INTERRUPT_REGISTERS.index(r) for r in regs)
        out.append("static uint4 %s(%s) { %s codebreaker_int.set = 0x%02x; codebreaker_int%02x(); "
                   "return (uint4)codebreaker_int.dx << 16 | codebreaker_int.ax; }" % (
                       interrupt_helper_name(v, regs), params, sets, mask, v))
    return out


# The decompiler's names for instructions C has no word for, in a 16-bit module: the port
# instructions (as the instruction itself, inline), the LOCK prefix (one processor and no other
# bus master here: a mark, not an operation) and the population count it writes for the parity
# flag. (`swi`, a software interrupt, is not one of these: what the registers held when it was
# taken is not in the decompiler's text, so no body here could do what the original does.)
INTRINSICS16 = {
    "in": "uint1 in(uint2 port);\n#pragma aux in = 0xEC parm [dx] value [al];",
    "out": "void out(uint2 port, uint1 value);\n#pragma aux out = 0xEE parm [dx] [al];",
    "LOCK": "#define LOCK()    /* the next instruction carries a LOCK prefix */",
    "UNLOCK": "#define UNLOCK()  /* (the end of that instruction) */",
    # LSL: the limit of a selector's segment, 32 bits (lsl eax,eax; edx = its high half)
    "SegmentLimit": "uint4 SegmentLimit(uint2 selector);\n#pragma aux SegmentLimit = 0x66 0x0f 0x03 0xc0 "
                    "0x66 0x8b 0xd0 0x66 0xc1 0xea 0x10 parm [ax] value [dx ax] modify exact [ax dx];",
    "POPCOUNT": "static uint2 POPCOUNT(uint4 x) { uint2 n = 0; while (x) { n += (uint2)(x & 1); x >>= 1; } return n; }",
}


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
_WIDE_ADDRESS = re.compile(r"0xffffffff([89a-f][0-9a-f]{7})")


_HOME = re.compile(r"_?([a-z]+)Stack(000000[0-9a-f]{2})")


def _homed_arguments(t):
    """A caller leaves room on the stack for each of the eight register arguments (eight bytes
    each, from 0x10 above the stack pointer at entry), and a function that needs an argument's
    address stores it there. The decompiler shows that slot as a stack variable of the CALLER's
    frame; it is the argument, and is written as the argument."""
    src = ctree.Source(t)
    params = src.parameters()
    names = [x.split()[-1].lstrip("*") for x in params] if params and ",".join(params).strip() not in ("", "void") else []

    def slot(off):
        k = (off - 0x10) // 8
        return names[k] if 0 <= k < len(names) and off % 8 in (0, 4) else None

    def home(src):
        # the slot's own declaration goes; every other mention becomes the argument
        for n in src.find("declaration", under=src.body()):
            ds = n.children_by_field_name("declarator")
            name = src.declared_name(ds[0]) if len(ds) == 1 else None
            m = _HOME.fullmatch(name or "")
            if m and src.line(n).startswith("  ") and src.at(n) == src.line(n).strip() and slot(int(m.group(2), 16)):
                src.replace(src.line_span(n), "")
        for n in src.find("identifier", under=src.body()):
            m = _HOME.fullmatch(src.at(n))
            if m and slot(int(m.group(2), 16)) and n.parent.type not in ("declaration", "pointer_declarator", "array_declarator"):
                src.replace(n, slot(int(m.group(2), 16)))

    def own_slot(src):                                       # the store into its own slot: `a1 = a1;`
        for st in src.find("expression_statement", under=src.body()):
            a = st.named_children[0] if st.named_child_count == 1 else None
            if a is not None and a.type == "assignment_expression" and src.at(a.child_by_field_name("operator")) == "=" \
                    and a.child_by_field_name("left").type == "identifier" \
                    and src.at(a.child_by_field_name("left")) == src.at(a.child_by_field_name("right")) \
                    and src.line(st).strip() == src.at(st):
                src.replace(src.line_span(st), "")
    return ctree.rewrite(ctree.rewrite(t, home), own_slot)


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
    for t in texts.values() if names else ():
        src = ctree.Source(t)
        for c in src.find("call_expression", under=src.body()):
            f = c.child_by_field_name("function")
            if f.type == "field_expression":                  # (`p->name(...)` counts as a call of name)
                f = f.child_by_field_name("field")
            if src.at(f) in names:
                seen.setdefault(src.at(f), set()).add(len(c.child_by_field_name("arguments").named_children))
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
    got = decompiler.raw(module_path, describe.function_starts(module_path), outdir)
    out, order, by_va, _err = got
    sizes, novalue = {}, set()
    for n in order:
        for va, size, _elem in (got.notes.get(n) or {}).get("globals") or ():
            sizes.setdefault(va, set()).add(size)
        t = out[n].strip()
        src = ctree.Source(t)
        for num in _numbers(src):
            wide = _WIDE_ADDRESS.fullmatch(src.at(num))
            if wide:
                src.replace(num, "0x" + wide.group(1))
        t = src.text()
        t = _struct_gaps(t)                      # an offset of a recovered structure with no field
        t = _pieces(t, _BY_SIZE, "uint4", lambda x, off, ty: "PIECE(%s, %s, %s)" % (x, off, ty))
        t = _compare_as_numbers(t)
        t = _code_pointers(t, set(order))
        t = _homed_arguments(t)
        body = _without_notes(t)
        lead = t[:len(t) - len(body)]
        if body.startswith("void") and not body[4:5].isalnum():
            # every function leaves something in r3 and callers are free to use it
            novalue.add(n)
            t = lead + "uint8" + body[4:]
        out[n] = _open_arguments(t)
    macros = []
    called_through = set().union(*(_called_through(t) for t in out.values())) if out else set()
    for pre, addr in sorted(set().union(*(_globals(t) for t in out.values())) if out else ()):
        szs = sizes.get(int(addr, 16)) or {4}
        size = max((x for x in szs if x <= 8), default=4)
        macros.append("#define %sRam%s (*(%s *)0x%s)" % (
            pre, addr, _ram_type(pre, size, "%sRam%s" % (pre, addr) in called_through), addr))
    # a call to a place in the original the decompiler was not told is a function: called there,
    # in the original, which is in memory beside the rebuilt image when it is judged
    lo_image = min(va for va, _off, _size in img.spans)
    hi_image = max(va + size for va, _off, size in img.spans)
    for addr in sorted({n[len("func_0x"):] for t in out.values() for n in _identifiers(t)
                        if re.fullmatch(r"func_0x[0-9a-f]{8}", n)}):
        if lo_image <= int(addr, 16) < hi_image and "func_0x" + addr not in out:
            macros.append("#define func_0x%s ((code *)0x%s)" % (addr, addr))
    used = sorted({n for t in out.values() for n in _called(t) if re.fullmatch(r"import\d+_ord_\d+", n)})
    decls = ["uint8 %s();" % n for n in used]
    for n in order:
        head = " ".join(_without_notes(out[n]).split("{", 1)[0].split())
        if "(" not in head:
            continue
        decls.append(head[:head.index("(")] + "();")
    # The structures recovered for each function (its facts), declared before the prototypes:
    # a function returning a pointer to one names it in its prototype, which every body includes.
    types = _TYPES.replace("typedef int code();", "typedef uint8 code();") + _HELPERS + _POWERPC
    structs = _struct_decls([st for n in order for st in (got.notes.get(n) or {}).get("structs") or ()], types)
    m = l2.Made(os.path.basename(module_path).lower(),
                types + "".join(x + "\n" for x in structs) + "\n".join(decls) + "\n" + "\n".join(macros) + "\n",
                out, order, by_va, novalue=novalue)
    by_name = {name: va for va, name in img.imports.items()}
    m.externs = {n: by_name[n] for n in used if n in by_name}
    m.argc = call_counts(out, set(m.externs))
    m.image = img
    return m


def interrupt_routines(vectors):
    """Assembly for the routines the interrupt helpers call: codebreaker_int<n> loads the
    registers the helper set (codebreaker_int.set, a bit each in INTERRUPT_REGISTERS' order) from
    codebreaker_int -- the others keep what they hold, DS and ES the caller's -- takes INT n, and
    stores every register and CF and ZF back. -> (assembly text, C text that defines codebreaker_int)"""
    off = {r: 2 * i for i, r in enumerate(INTERRUPT_REGISTERS)}
    bit = {r: 1 << i for i, r in enumerate(INTERRUPT_REGISTERS)}
    out = ["; the software interrupts the module takes, each as the original takes it: the registers",
           "; from codebreaker_int, INT n, the registers and the carry and zero flags back into it",
           ".286", "DGROUP group _DATA", "_DATA segment word public 'DATA'", "        extrn _codebreaker_int:word",
           "_DATA ends", "_TEXT segment word public 'XCODE'", "        assume cs:_TEXT, ds:DGROUP"]
    for v in sorted(vectors):
        name = "_codebreaker_int%02x" % v
        out += ["        public %s" % name, "%s proc far" % name,
                "        push bp", "        push si", "        push di", "        push ds", "        push es",
                "        mov  bp, ds                                   ; DGROUP, to come back to",
                "        push ds                                       ; DS as the caller has it"]
        for k, r in enumerate(("es", "ax", "bx", "cx", "dx", "si", "di", "ds")):
            skip = "L_%02x_%d" % (v, k)
            out += ["        test word ptr _codebreaker_int+18, %d" % bit[r], "        jz   short %s" % skip]
            if r == "ds":                                      # (DS last: it is what the others are read through)
                out += ["        pop  ax", "        push word ptr _codebreaker_int+%d" % off["ds"]]
            else:
                out.append("        mov  %s, word ptr _codebreaker_int+%d" % (r, off[r]))
            out.append("%s:" % skip)
        out += ["        test word ptr _codebreaker_int+18, %d" % bit["ax"], "        jz   short L_%02x_ax" % v,
                "        mov  ax, word ptr _codebreaker_int+%d" % off["ax"], "L_%02x_ax:" % v,
                "        pop  ds",
                "        int  %03Xh" % v,
                "        pushf", "        push ds", "        mov  ds, bp"]
        for r in ("ax", "bx", "cx", "dx", "si", "di", "es"):
            out.append("        mov  word ptr _codebreaker_int+%d, %s" % (off[r], r))
        out += ["        pop  word ptr _codebreaker_int+%d" % off["ds"], "        pop  ax",
                "        mov  byte ptr _codebreaker_int+16, al", "        and  byte ptr _codebreaker_int+16, 1",
                "        shr  ax, 6", "        and  al, 1", "        mov  byte ptr _codebreaker_int+17, al",
                "        pop  es", "        pop  ds", "        pop  di", "        pop  si", "        pop  bp",
                "        retf", "%s endp" % name]
    out += ["_TEXT ends", "end", ""]
    c = '#include "m.h"\nstruct codebreaker_registers codebreaker_int;\n'
    return "\n".join(out), c
