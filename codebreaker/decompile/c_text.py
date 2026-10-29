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


_SIZEOF = {"int1": 1, "uint1": 1, "xunknown1": 1, "char": 1, "bool": 1, "undefined": 1, "undefined1": 1,
           "int2": 2, "uint2": 2, "xunknown2": 2, "undefined2": 2,
           "int4": 4, "uint4": 4, "xunknown4": 4, "undefined4": 4, "float4": 4,
           "int8": 8, "uint8": 8, "xunknown8": 8, "undefined8": 8, "float8": 8, "float10": 10}


def _keep_frame(t, sizes=None):
    """Keep a function's stack locals where the original had them, relative to one another.

    The original often builds a small structure in its frame and passes its address
    (`mov [ebp-18h], hwnd / mov [ebp-14h], id / lea edx, [ebp-18h] / push edx`). The core
    recovers the fields as separate locals; a C compiler then places them wherever it likes, and
    the callee, reading through the pointer, finds something else. When any stack local's address
    can escape, the locals are declared as the fields of one frame structure at their original
    offsets, so every address taken still points at what the original put there. `sizes`: more
    types' sizes -- the structures recovered for the function (`/*@S` notes)."""
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


# The decompiler's note for a structure it recovered from a variable's field accesses (CB_STRUCTS):
#   /*@S <name> <bytes> <offset>:<bytes>:<type> ...*/  -- offsets in hex; a type is one word, a
# pointer's ending in `*` (or `^`, a 16-bit far pointer)
_STRUCT_NOTE = re.compile(r"/\*@S (\w+) (\d+)((?: [0-9a-f]+:\d+:[\w*^]+)*) \*/")
_C_WORDS = {"char", "short", "int", "long", "float", "double", "void", "code"}


def _struct_sizes(notes):
    """{structure name: its bytes} from `/*@S` notes"""
    out = {}
    for note in notes:
        mm = _STRUCT_NOTE.fullmatch(note.strip())
        if mm:
            out.setdefault(mm.group(1), int(mm.group(2)))
    return out


def _struct_decls(notes, header, far16=False):
    """The C declaring the structures of the `/*@S` notes -- every name first (`typedef struct
    s_f_p s_f_p;`, so one may point at another), then each at its exact layout: its fields at
    their offsets with filler between, the whole its full size, packed (the 32-bit header packs
    everything; a 16-bit structure says so itself). A field type the header does not know is
    the bytes it takes."""
    found = {}
    for note in notes:
        mm = _STRUCT_NOTE.fullmatch(note.strip())
        if mm and mm.group(1) not in found:
            found[mm.group(1)] = (int(mm.group(2)), [(int(o, 16), int(z), t) for o, z, t in
                                                     (f.split(":") for f in mm.group(3).split())])
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
    out, order, by_va, _err = decompiler.raw(module_path, funcs, outdir)
    imports = dict(B.import_map(module_path))
    # imports by number, under the names the description gave them: declared and linked too
    numbered = describe.BY_NUMBER.get(module_path) or {}
    imports.update({n: stem for n, (stem, _number, _pop) in numbered.items() if n not in imports})
    sizes, in_ecx, in_edx = {}, set(), set()
    own = {}                                  # function -> address of a global -> bytes of its uses there
    struct_notes = []                         # the structures the decompiler recovered (`/*@S`)
    for n in order:
        body = []
        for ln in out[n].split("\n"):
            m = re.match(r"/\*@G ([0-9a-f]+) (\d+)(?: \d+)? \*/$", ln)
            if ln.startswith("/*@S "):
                struct_notes.append(ln)
            elif m:
                sizes.setdefault(int(m.group(1), 16), set()).add(int(m.group(2)))
                own.setdefault(n, {}).setdefault(int(m.group(1), 16), set()).add(int(m.group(2)))
            else:
                body.append(ln)
        # (a note the core put before the function -- "WARNING: Removing unreachable block" --
        # is not part of it: left in, it became the function's declaration and broke the header)
        t = _without_notes("\n".join(body).strip())
        # `x._1_2_` is the core's spelling of "2 bytes of x starting at byte 1"
        t = _pieces(t, _BY_SIZE, "uint4")
        t = _compare_as_numbers(_keep_frame(_null_is_zero(t), _struct_sizes(struct_notes)))
        # a function's address stored in a pointer the core typed loosely
        t = _code_pointers(t, set(order))
        t = _struct_gaps(t)                      # an offset of a recovered structure with no field
        # every function returns a value in EAX whether or not the core saw it used
        if t.startswith("void") and t[4:5].isspace():
            t = "int" + t[4:]
        # the compiler used here has no word for "first argument in ECX": the function is
        # declared plainly, and a #pragma in the header says where its first argument is
        if "__thiscall" in t.split("{", 1)[0]:
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


class Module(object):
    """A decompiled module: `header` (shared text), `functions` {address: (name, text)},
    `order` (names by address), `imports` (the import names its functions call)."""

    def __init__(self, header, texts, order, by_va, imports, in_ecx=(), in_edx=()):
        self.header, self.texts, self.order, self.by_va, self.imports = header, texts, order, by_va, imports
        self.in_ecx = set(in_ecx)                # names of functions that take their first argument in ECX
        self.in_edx = set(in_edx)                # ... and their second in EDX
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

_FACT = re.compile(r"(?m)^/\*@([GP]) (\S+) (\d+)(?: (\d+))? \*/\n?")
_WIDE = ("xunknown4", "int4", "uint4", "undefined4", "float4")


class Module16(object):
    pass


def moved_to_l2(raw):
    """The passes the C back end did on the tree, for a function whose C it wrote from L2
    (CB_C_FROM_L2; decompiler.c_from_l2 marks it): left out here (docs/IR_UNIFICATION.md step 3).
    One it declined keeps the printed C, and every pass."""
    from codebreaker.decompile import decompiler
    if decompiler.FROM_L2 not in raw:
        return ()
    from codebreaker.translate.backends import c as cback
    return cback.MOVED


def data_address(text):
    """Is `text` a constant address in the data segment: a number of one word, not 0? A far
    pointer set to one is DS and it (LZEXPAND's LZDONE counts down the word at DS:32h through
    one); cast to a far pointer, a word gets segment 0 and the write goes to 0000:0032 -- and
    left near it does as well: OpenWatcom folds the constant near pointer made far to
    `xor ax, ax / mov es, ax / dec word ptr es:[bx]`. Written with DS itself,
    MK_FP(codebreaker_ds(), 0x32), as the original. (0 stays a far null; a number past a word
    carries its own segment.)"""
    m = re.fullmatch(r"(0x[0-9a-fA-F]+|\d+)[uUlL]*", text.strip())
    return bool(m) and 0 < int(m.group(1), 0) <= 0xFFFF


def _far(text, far):
    """Declare the four-byte pointers `far` and make what is assigned to them far too (but a data
    address, which stays near: data_address)."""
    if not far:
        return text

    def star(src, dcl):
        """`*name` (a pointer declarator of one of `far`) -> `__far *name`"""
        m = re.match(r"(\*+)\s*(\w+)", src.at(dcl))
        if m and m.group(2) in far and src.declared_name(dcl) == m.group(2):
            src.replace((dcl.start_byte, dcl.start_byte + m.end()), "__far %s%s" % (m.group(1), m.group(2)))

    def fix(src):
        for n in src.find("parameter_declaration"):          # parameters
            dcl = n.child_by_field_name("declarator")
            if dcl is not None and dcl.type == "pointer_declarator":
                star(src, dcl)
        for n in src.find("declaration", under=src.body()):  # locals, one to a line
            ds = n.children_by_field_name("declarator")
            if len(ds) == 1 and ds[0].type == "pointer_declarator" and src.at(n) == src.line(n).strip():
                star(src, ds[0])
        for n in src.find("assignment_expression"):        # `name = (type *)expr` -> `name = (type __far *)expr`
            left, right = n.child_by_field_name("left"), n.child_by_field_name("right")
            while left.type == "pointer_expression" and src.at(left.child_by_field_name("operator")) == "*":
                left = left.child_by_field_name("argument")       # (`*pp = ...`: pp is declared T __far **)
            if src.at(n.child_by_field_name("operator")) != "=" or left.type != "identifier" or src.at(left) not in far:
                continue
            cast = src.leading(right, "cast_expression")       # (the cast the right side starts with)
            if cast is not None:
                ty = cast.child_by_field_name("type")
                value = cast.child_by_field_name("value")
                m = re.fullmatch(r"([A-Za-z_][\w ]*?) ?(\*+)", src.at(ty))
                if m and data_address(src.at(value)):
                    src.replace(cast, "(%s __far %s)MK_FP(codebreaker_ds(), %s)" % (m.group(1), m.group(2), src.at(value)))
                elif m:
                    src.replace(ty, "%s __far %s" % (m.group(1), m.group(2)))
    return _far_from_stack(ctree.rewrite(text, fix), far)


def _global_pointer_steps(text):
    """`pxRam1160012e + 2` -- arithmetic on a pointer the core keeps in an unnamed global -- is
    in bytes: the original reads [[12Eh]+2] (USER's DisableOEMLayer and ExitWindows), while the
    global's own type points at words and C would step 4. Stepped as bytes; a plain `*p` keeps
    its type."""
    return re.sub(r"(?<![\w*)])(p+[a-z]*Ram[0-9a-f]{8}) \+ ", r"(uint1 *)\1 + ", text)


def _own_segment_calls(text, seg):
    """`(*MK_FP(0x1000, *(xunknown2 *)0x18))()` -- a near call through a word, `call word ptr
    [18h]`, as the decompiler prints it: the target is in the caller's own code segment (CS, which
    it takes as the segment's number times 1000h). Made a far call to that constant, it faulted:
    MSMOUS31's Disable took a GP at Setup's restart, after the file copy. A near call, as the
    original makes. (CS as the decompiler has it is the segment's place in the description, its
    made-up selector: 1000h for segment 1, 1040h for segment 9 -- KEYBOARD's OemToAnsi, `cs:
    xlat` from a table beside its code -- not the number times 1000h, which is that only for 1.)"""
    if "MK_FP(" not in text:
        return text
    from codebreaker.decompile import describe
    own = describe.selector(seg)
    src = ctree.Source(text)
    for n in src.find("call_expression"):
        fn = n.child_by_field_name("function")
        while fn is not None and fn.type == "parenthesized_expression" and fn.named_child_count == 1:
            fn = fn.named_children[0]
        if fn is None or fn.type != "pointer_expression":
            continue
        target = fn.child_by_field_name("argument")
        while target is not None and target.type == "parenthesized_expression" and target.named_child_count == 1:
            target = target.named_children[0]
        if target is None or target.type != "call_expression" or src.at(target.child_by_field_name("function")) != "MK_FP":
            continue
        args = target.child_by_field_name("arguments").named_children
        if len(args) == 2 and args[0].type == "number_literal" and int(src.at(args[0]), 0) == own:
            src.replace(fn, "(*(code __near *)%s)" % src.at(args[1]))
    # and a read of the code segment's own bytes (`mov ax, cs:[si]`, a table beside the code):
    # through CS as it runs, not the constant (no selector of that number exists)
    return re.sub(r"\bMK_FP\(0x%x," % own, "MK_FP(codebreaker_cs(),", src.text())


def _near_calls(text):
    """A call through a word -- `call word ptr [18h]`, the decompiler's `(*(code *)*(xunknown2
    *)0x18)()` -- is a near call into the function's own code segment. `code *` is a far
    pointer in the model the 16-bit C is built in; a word made into one gets segment 0, and
    the call faults: MSMOUS31's Disable (`call word ptr [18h]` to the hardware's own disable)
    took a GP at Setup's restart, after the file copy. Called near, as the original does."""
    return re.sub(r"\(\*\(code \*\)(?=\*\((?:xunknown2|int2|uint2|undefined2) \*\))", "(*(code __near *)", text)


def _far_from_stack(body, far):
    """A far pointer set from a near one into the frame: the original made it with SS (`push ss;
    pop es; rep movsw` -- USER's WM_WINDOWPOSCHANGED copies each WINDOWPOS into its frame so).
    The decompiler keeps the destination far but not the ES it was given; C makes a far pointer
    of a near one with DS, and the 14 bytes went to the frame's offset in the data segment --
    over the code of whatever lay there. Given SS, as the original."""
    src = ctree.Source(body)
    sets = []                                            # (variable, the assignment's right side) of `v = ...;` statements
    for st in src.find("expression_statement"):
        a = st.named_children[0] if st.named_child_count == 1 else None
        line = src.line(st)
        if a is not None and a.type == "assignment_expression" and src.at(a.child_by_field_name("operator")) == "=" \
                and a.child_by_field_name("left").type == "identifier" \
                and line[:1].isspace() and line.lstrip().startswith(src.at(st)):     # (a statement of its own line)
            sets.append((src.at(a.child_by_field_name("left")), a.child_by_field_name("right")))
    # (an address in the frame: an array local, or & of a local -- not the value a local holds)
    arrays = set(re.findall(r"(?m)^\s+[\w ]+?\**\s*(\w*Stack_[0-9a-f]+) \[\d+\];", body))

    def origin(n):
        """(the variable a pointer is set from, whether its address was taken): `x`, `&x`,
        `(T *)x`, `(T *)&x`, each `+ n` -- or (None, False)"""
        if n.type == "binary_expression" and n.child_by_field_name("operator").type == "+" and \
                re.fullmatch(r"[\w()]+", src.at(n.child_by_field_name("right"))) and \
                src.at(n)[len(src.at(n.child_by_field_name("left"))):].startswith(" + "):
            n = n.child_by_field_name("left")
        if n.type == "cast_expression" and re.fullmatch(r"[\w ]+\*", src.at(n.child_by_field_name("type"))):
            n = n.child_by_field_name("value")
        amp = False
        if n.type == "pointer_expression" and src.at(n.child_by_field_name("operator")) == "&":
            n, amp = n.child_by_field_name("argument"), True
        return (src.at(n), amp) if n.type == "identifier" else (None, False)

    def in_frame(found, stack):
        name, amp = found
        return name is not None and bool((amp and re.fullmatch(r"\w*Stack_[0-9a-f]+", name)) or name in arrays or
                                         (not amp and name in stack))
    stack = set()
    for _ in range(3):                                   # (pointers set from those, in turn)
        for v, rhs in sets:
            if v not in far and in_frame(origin(rhs), stack):
                stack.add(v)
    # (a pointer set from anything else too is not known to be in the frame -- nor, then, one set
    # from it: dropped until none is left to drop, so the answer does not depend on the order a
    # set is gone through, which changes from run to run)
    dropped = True
    while dropped:
        dropped = False
        for v in sorted(stack):
            if any(not in_frame(origin(rhs), stack) for w, rhs in sets if w == v):
                stack.discard(v)
                dropped = True
    if not stack:
        return body
    for v, rhs in sets:
        if v in far and rhs.type == "identifier" and src.at(rhs) in stack:
            src.replace(rhs, "MK_FP(codebreaker_ss(), %s)" % src.at(rhs))
    return src.text()


def _frame_through_ss(body):
    """A buffer in the frame reached through its address as a number: `*(xunknown2 *)((int2)
    axStack_a8 + iVar1) = 0x5c` -- KEYBOARD's NEWTABLEEX puts the `\\` after the system directory
    so, `mov ss:[bx], ax`. The decompiler keeps the offset but not the SS it went with, and a near
    pointer made of a number is read through DS: the byte went to the data segment. Written with
    SS, as the original: `*(xunknown2 *)MK_FP(codebreaker_ss(),(int2)axStack_a8 + iVar1)`, which
    the far passes after make `*(xunknown2 __far *)((uint1 __far *)MK_FP(...))`."""
    if "Stack_" not in body:
        return body
    arrays = set(re.findall(r"(?m)^\s+[\w ]+?\**\s*(\w*Stack_[0-9a-f]+) \[\d+\];", body))
    src = ctree.Source(body)

    def in_frame(n):
        """`(int2)axStack_a8` or `(int2)&iStack_6`: a frame address as a number"""
        if n.type != "cast_expression" or src.at(n.child_by_field_name("type")) not in ("int2", "uint2"):
            return False
        v = n.child_by_field_name("value")
        if v.type == "pointer_expression" and src.at(v.child_by_field_name("operator")) == "&":
            v = v.child_by_field_name("argument")
            return v.type == "identifier" and re.fullmatch(r"\w*Stack_[0-9a-f]+", src.at(v)) is not None
        return v.type == "identifier" and src.at(v) in arrays

    for n in src.find("pointer_expression"):
        if src.at(n.child_by_field_name("operator")) != "*":
            continue
        cast = n.child_by_field_name("argument")
        if cast.type != "cast_expression":
            continue
        ty = re.fullmatch(r"([\w ]+?) ?\*", src.at(cast.child_by_field_name("type")))
        sum_ = cast.child_by_field_name("value")
        if ty is None or sum_.type != "parenthesized_expression" or sum_.named_child_count != 1:
            continue
        sum_ = sum_.named_children[0]
        if sum_.type != "binary_expression" or sum_.child_by_field_name("operator").type != "+" or \
                not (in_frame(sum_.child_by_field_name("left")) or in_frame(sum_.child_by_field_name("right"))):
            continue
        src.replace(cast, "(%s *)MK_FP(codebreaker_ss(),%s)" % (ty.group(1), src.at(sum_)))   # (made far by _far_casts)
    return src.text()


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


def _joined_arguments(text, widths=None):
    """`_a1` is the decompiler's spelling of "a1 and the argument after it, as one four-byte
    value" (a far pointer or a long passed as two words). Arguments lie next to each other on
    the stack, lowest first, so that value is the four bytes starting at a1."""
    # read through directly (`*_a1`): it is a far pointer; the decompiler gives no type for what it
    # points at -- a word in nearly every case, a byte where the original reads one through it
    # (`widths`, from its instructions: USER's sub_42_038E compares `byte ptr es:[bx]` with FFh,
    # the mark of an icon's resource number in a dialog item; read as a word it never matched,
    # an extra 0 went into the template, and every item after the icon was read askew -- the
    # icon message boxes failed with a divide error)
    widths = widths or {}

    def through(name):
        return "*(*(%s __far * *)&%s)" % ("char" if widths.get(name) == 1 else "int2", name)

    def fix(src):
        for n in src.find("identifier"):
            m = re.fullmatch(r"_(a\d+|param_\d+)", src.at(n))
            if not m:
                continue
            op = src.unary(n)
            if op and op[0] == "*":
                src.replace((op[1], n.end_byte), through(m.group(1)))
            else:
                src.replace(n, "(*(uint4 *)&%s)" % m.group(1))
    return ctree.rewrite(text, fix)


def _far_argument_widths(code, start, end, far_fn):
    """{a<n>: bytes} -- how wide the original reads through the far pointer it takes as
    argument n: `les reg, [bp+N]`, then the first `es:[reg...]` through it."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
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


def _segment_registers(text):
    """`unaff_SS`, `unaff_DS`, `unaff_CS` are the decompiler's names for "whatever that segment
    register held when the function was entered" -- used when it passes the address of a local
    as a far pointer (offset, SS) or a string constant as (offset, CS). Left as plain locals
    they would be garbage; they are given the register's value."""
    def fix(src):
        for n in src.find("declaration"):
            ds = n.children_by_field_name("declarator")
            if len(ds) == 1 and ds[0].type == "identifier" and src.at(n.child_by_field_name("type")).isidentifier():
                m = re.fullmatch(r"unaff_(SS|DS|CS)", src.at(ds[0]))
                if m:
                    src.insert(ds[0].end_byte, " = codebreaker_%s()" % m.group(1).lower())
    return ctree.rewrite(text, fix)


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
    src = ctree.Source(text)
    params = src.parameters()
    if params is None or ",".join(params).strip() in ("", "void"):
        return text
    at, where = (4 if near else 6), {}
    for prm in params:
        words = prm.split()
        name = words[-1].lstrip("*")
        wide = "*" in prm and "__far" in prm or words[0] in _WIDE
        where[at] = (name, words[0], wide)
        at += 4 if wide else 2
    segment_of = {}
    for off in sorted(loads):
        if off not in where or off + 2 not in where:
            continue
        (name, kind, wide), (segment, kind2, wide2) = where[off], where[off + 2]
        if wide or wide2 or kind not in ("int2", "uint2", "xunknown2") or kind2 not in ("int2", "uint2", "xunknown2"):
            continue
        segment_of.setdefault(name, segment)
    if not segment_of:
        return text
    for cast in src.find("cast_expression", under=src.body()):
        op = src.unary(cast)
        ty = re.fullmatch(r"([A-Za-z_]\w*) \*", src.at(cast.child_by_field_name("type")))
        if not op or op[0] != "*" or not ty:
            continue
        v = cast.child_by_field_name("value")
        if v.type == "identifier" and src.at(v) in segment_of:              # *(T *)name
            name, rest = src.at(v), ""
        elif v.type == "parenthesized_expression" and v.named_child_count == 1 and \
                v.named_children[0].type == "binary_expression" and \
                v.named_children[0].child_by_field_name("operator").type == "+" and \
                src.at(v.named_children[0].child_by_field_name("left")) in segment_of and \
                "(" not in src.at(v)[1:-1] and ")" not in src.at(v)[1:-1]:      # *(T *)(name + offset)
            b = v.named_children[0]
            name = src.at(b.child_by_field_name("left"))
            rest = " + " + src.at(v)[1:-1].split("+", 1)[1].strip() if src.at(v)[1:-1].startswith(name + " + ") else None
            if rest is None:
                continue
        else:
            continue
        src.replace((op[1], cast.end_byte), "*(%s __far *)CODEBREAKER_FAR(%s, %s%s)" % (ty.group(1), segment_of[name], name, rest))
    return src.text()


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


_DEFINITION = re.compile(r"^([^\n#{};]*?\b(\w+))\(([^)]*)\)\n((?:[^{;\n]+;\n)*)\{", re.M)
_WIDE_TYPES = re.compile(r"\b(u?int4|xunknown4|long|unkbyte3|unkint3|unkuint3|int3|float4)\b")


def _words(ty):
    """the words a value of C type `ty` takes on the stack -- 1, 2 -- or None"""
    ty = " ".join(ty.split())
    if "*" in ty:
        return 2 if "__far" in ty else 1
    if _WIDE_TYPES.search(ty):
        return 2
    if re.search(r"\b(u?int2|xunknown2|int|unsigned|short|code|u?int1|char|xunknown1|bool)\b", ty):
        return 1
    return None


def _parameters(text):
    """(head before the parameter list, [(name, type)]) of a definition as the decompiler writes it, or None"""
    m = _DEFINITION.search(text)
    if not m:
        return None
    decl = {}
    for d in m.group(4).strip().splitlines():
        d = d.strip().rstrip(";")
        nm = re.findall(r"\w+", d)
        if nm:
            decl[nm[-1]] = d[:d.rindex(nm[-1])].strip()
    names = [p.strip() for p in m.group(3).split(",") if p.strip()]
    head = []                                        # (the head may run over lines: `int2 __stdcall16far` / `name(`)
    for line in reversed(text[:m.end(2)].split("\n")):
        s = line.strip()
        if head and (not s or s.endswith((";", "}", "*/")) or s.startswith(("#", "extern ", "/*"))):
            break
        head.insert(0, s)
    return " ".join(head), [(p, decl.get(p, "int")) for p in names]


def _argument_words(arg, types):
    """the words an argument expression takes on the stack, as far as it can be told, else None"""
    a = arg.strip()
    while a.startswith("(") and _arguments_at(a, 0)[1] == len(a):
        a = a[1:-1].strip()                          # (the whole in brackets)
    if re.fullmatch(r"-?(0x[0-9a-fA-F]+|\d+)", a):
        v = int(a, 0)
        return 1 if -0x8000 <= v <= 0xFFFF else 2
    if re.fullmatch(r"-?(0x[0-9a-fA-F]+|\d+)[lL]", a):
        return 2
    cast = re.match(r"\(\s*([A-Za-z_][\w ]*\**)\s*\)", a)
    if cast and _arguments_at(a, 0)[1] == cast.end():
        return _words(cast.group(1))
    if re.fullmatch(r"[A-Za-z_]\w*", a):
        if a in ("unaff_SS", "unaff_DS", "unaff_ES", "unaff_CS"):
            return 1
        return _words(types[a]) if a in types else None
    if a.startswith("&") or a.startswith('"'):
        return 1                                     # (a near address: the medium model's data is near)
    return None


def prototypes(texts, order):
    """{name: a declaration with its parameters} for each function every call to which, in all of
    `texts`, passes as many words in each place as its definition takes there -- the source had
    prototypes, and with none Microsoft C gives other code for the call (it keeps what it passes
    in a register variable, and removes the arguments itself before a frame's `leave`). A function
    a call disagrees with keeps the open declaration (see declaration). Each parameter is declared
    with the type its old-style definition gives it, as it is -- `short`, not the `int` it is
    promoted to: both Microsoft C 8 (silent; C4028 for the promoted one) and OpenWatcom (E1129 for
    the promoted one) take it so."""
    params = {n: _parameters(texts[n]) for n in order}
    want = {n: [_words(t) for _p, t in ps[1]] for n, ps in params.items() if ps and "..." not in ps[0]}
    ok = {n: all(w is not None for w in ws) for n, ws in want.items()}
    for caller in order:
        t = texts[caller]
        info = params.get(caller)
        types = dict(info[1]) if info else {}
        body = t[t.find("{"):] if "{" in t else t
        for line in body.splitlines():                   # (the function's own variables)
            mm = re.match(r"^\s+([A-Za-z_][\w ]*?[\w*])\s*\**\s*(\w+)(\s*\[[^\]]*\])?\s*;\s*$", line)
            if mm and mm.group(1) not in ("return", "goto"):
                star = line.count("*")
                types[mm.group(2)] = mm.group(1) + (" *" * star if star else "") + (" *" if mm.group(3) else "")
        for mm in re.finditer(r"\b(\w+)\s*\(", body):
            callee = mm.group(1)
            if not ok.get(callee):
                continue
            args, _end = _arguments_at(body, mm.end() - 1)
            if len(args) != len(want[callee]) or \
                    any(_argument_words(a, types) != w for a, w in zip(args, want[callee])):
                ok[callee] = False
            elif "__stdcall" in params[callee][0] and \
                    any(re.search(r"\b(unaff_[SD]S|codebreaker_[sd]s\(\))|>>\s*0x10", a) for a in args):
                ok[callee] = False                   # (a far pointer passed as halves: build/msc6.py's
                                                     #  pascal_order makes it one argument -- not two words)
    out = {}
    for n, good in ok.items():
        if not good:
            continue
        head, ps = params[n]
        listed = ", ".join("%s %s" % (t, p) for p, t in ps)
        out[n] = " ".join(head.split()) + "(" + (listed or "void") + ");"
    return out


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
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
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
    # (and the C written from L2 instead, CB_C_FROM_L2: other C, by the back end's own source)
    k = cache.key(body, _source_stamp(sys.modules[__name__], decompiler, describe, api_arguments),
                  *([os.environ["CB_C_FROM_L2"], _source_stamp(cback)] if os.environ.get("CB_C_FROM_L2") else []),
                  cache.tool(decompiler._exe()), decompiler._language_stamp(),
                  (settings.get("struct_recovery") or "off").strip().lower(),
                  os.environ.get("CB_SELECTOR_STRUCTS", ""),       # (_selector_structs' off switch)
                  os.environ.get("CB_CALLER_ARGS", ""),            # (describe's direct-call arguments)
                  os.environ.get("CB_FASTCALL", ""),               # (describe's _fastcall functions)
                  os.environ.get("CB_PROTOTYPES", ""),             # (prototypes where the calls agree)
                  os.environ.get("CB_CALLER_RESULTS", ""))         # (results the callers read DX of)
    had = cache.get("compilable16", k)
    if had is not None:
        m, description = pickle.loads(had)
        folder = outdir or os.path.dirname(m.description.path)
        os.makedirs(folder, exist_ok=True)
        m.description.path = os.path.join(folder, "description.txt")
        with open(m.description.path, "w", encoding="utf-8", newline="\n") as f:
            f.write(description)
        return m
    m = _compilable16(module_path, outdir)
    with open(m.description.path, encoding="utf-8") as f:
        cache.put("compilable16", k, pickle.dumps((m, f.read())))
    return m


def _before_name(text, name, word):
    """`word` written before the function's name in its heading (`__export F(`)."""
    src = ctree.Source(text)
    n = src.function_name()
    if n is None or src.at(n) != name:
        return text
    src.insert(n.start_byte, word)
    return src.text()


def _far_before_name(text, name):
    """Is the function declared `__far name(`?"""
    src = ctree.Source(text)
    n = src.function_name()
    return n is not None and src.at(n) == name and \
        src.data[:n.start_byte].decode("utf-8").rstrip().endswith("__far")


def _far_result(text, name):
    """The `*` of the result in the heading made `__far *` (the function returns a far pointer)."""
    src = ctree.Source(text)
    n = src.function_name()
    if n is None or src.at(n) != name:
        return text
    at = n.start_byte
    while at > 0 and (chr(src.data[at - 1]).isspace() or chr(src.data[at - 1]).isalnum() or src.data[at - 1:at] == b"_"):
        at -= 1                                          # (back over the words between: `* __stdcall16far name`)
    if src.data[at - 1:at] != b"*":
        return text
    src.replace((at - 1, at), "__far *")
    return src.text()


def _byte_result(text):
    """(the type, where what follows it starts) when the function's result is a byte type, else None"""
    src = ctree.Source(text)
    for f in src.find("function_definition"):
        ty = f.child_by_field_name("type")
        dcl = f.child_by_field_name("declarator")
        if src.at(ty) in ("bool", "char", "int1", "uint1", "xunknown1", "undefined1", "undefined") and \
                dcl is not None and dcl.type != "pointer_declarator" and not src.data[:ty.start_byte].strip():
            end = ty.end_byte
            while end < len(src.data) and chr(src.data[end]).isspace():
                end += 1
            return src.at(ty), len(src.data[:end].decode("utf-8"))
        break
    return None


def _one_name_for_globals(text):
    """`_iRam0040c000` -> `iRam0040c000`: the decompiler's second name for a global (read at
    another width) is the global."""
    src = ctree.Source(text)
    for n in src.find("identifier"):
        if re.fullmatch(r"_[a-z]+Ram[0-9a-f]{8}", src.at(n)):
            src.replace(n, src.at(n)[1:])
    return src.text()


def _callee_pops(text):
    """Calls through pointers whose callee removes the arguments: `(code *)` -> `(code_callee_pops *)`."""
    src = ctree.Source(text)
    for c in src.find("cast_expression"):
        ty = c.child_by_field_name("type")
        m = re.fullmatch(r"code (__far |__near )?\*", src.at(ty))
        if m:
            src.replace(ty, "code_callee_pops %s*" % (m.group(1) or ""))
    return src.text()


def _identifiers(text):
    src = ctree.Source(text)
    return [src.at(n) for n in src.find("identifier")]


def _compilable16(module_path, outdir=None):
    from codebreaker.decompile import decompiler, describe
    texts, order, by_va, d = decompiler.decompile16(module_path, outdir)
    m = Module16()
    m.texts, m.order, m.by_va, m.where = {}, [], {}, {}
    m.near, m.wide, m.novalue = set(), set(), set()
    m.interrupts = set()                                     # (vector, arguments) of the interrupts taken
    m.selector_imports = {}                                  # imports used for their selector
    m.exported_prologue = set()                              # far functions the original starts with Windows' prologue
    sizes = {}                                               # address of a global -> bytes of the access
    struct_notes = []                                        # the structures the decompiler recovered (`/*@S`)
    selector_structs = []                                    # ... and those a segment holds (_selector_structs)
    shared_selectors = {}                                    # ... a global selector's: the module's
    by_selector = decompiler._structs(d.path) == "1" and os.environ.get("CB_SELECTOR_STRUCTS") != "0"   # (with structures)
    elems = {}                                               # ... a pointer's -> bytes of what it points at
    ends, starts = {}, {}                                    # where each function's code stops: at the next one
    for va in sorted(d.where):
        starts.setdefault(d.where[va][0], []).append((d.where[va][1], va))
    for seg, lst in starts.items():
        lst.sort()
        for k, (off, va) in enumerate(lst):
            ends[va] = lst[k + 1][0] if k + 1 < len(lst) else len(d.images.get(seg, b""))
    # A function the decompiler could not make stays a function of the module: a body that says
    # so, which the build replaces by the function's own instructions where it was written in
    # assembly, and otherwise reports as not rebuilt (m.failed).
    m.failed = {}
    with open(d.path, encoding="utf-8") as f:
        for line in f:
            if line.startswith("F\t"):
                parts = line.rstrip("\n").split("\t")
                va, nm = int(parts[1], 16), parts[2]
                if va not in by_va and nm not in texts and d.where.get(va):
                    model = parts[4] if len(parts) > 4 and parts[4] != "inline" else "__cdecl16near"
                    texts[nm] = ("xunknown2 %s %s()\n{\n  /* the decompiler could not make this function */\n"
                                 "  return 0;\n}" % (model, nm))
                    by_va[va] = nm
                    m.failed[nm] = "the decompiler could not make it"
    # helpers that work in their caller's frame: its places passed to them (_caller_frame_slots)
    frames = {}
    for va, nm in by_va.items():
        seg, off = d.where.get(va) or (None, None)
        frames[nm] = seg in d.images and _keeps_frame(d.images[seg], off)
    texts = _caller_frame_slots(texts, frames)
    # each function's model as the description gives it: near or far is how it returns, and
    # the C must say it (the decompiler leaves the convention out of a head whose result is a
    # pointer -- and in the medium model an unmarked function is far)
    callee_pops = {}                                         # function -> its pointer calls' callee removes the arguments
    with open(d.path, encoding="utf-8") as f:
        for line in f:
            if line.startswith("O\t"):
                parts = line.rstrip("\n").split("\t")
                if parts[4].startswith("__stdcall"):
                    # (kept by the call's own place: the function the description named for it
                    # may since have been found to be two -- the call belongs to the one that
                    # holds it, below)
                    callee_pops[int(parts[1], 16)] = True
    import bisect
    starts_all = sorted(by_va)
    held = {}
    for site in callee_pops:
        k = bisect.bisect_right(starts_all, site) - 1
        if k >= 0 and starts_all[k] >> 16 == site >> 16:
            held[starts_all[k]] = True
    callee_pops = held
    distance, model_of = {}, {}
    with open(d.path, encoding="utf-8") as f:
        for line in f:
            parts = line.rstrip("\n").split("\t")
            if parts[0] == "F" and len(parts) > 4:
                distance[parts[2]] = "far" if parts[4].endswith("far") else "near" if parts[4].endswith("near") else None
                if parts[4].startswith(("__stdcall16", "__cdecl16")):
                    model_of[parts[2]] = parts[4]
    for va in sorted(by_va):
        name = by_va[va]
        raw = texts[name]
        moved = moved_to_l2(raw)
        head0 = raw.split("{", 1)[0]
        named = ctree.Source(raw).function_name()
        if distance.get(name) and "16near" not in head0 and "16far" not in head0 and named is not None and \
                named.start_byte < len(head0.encode("utf-8")) and named.text.decode("utf-8") == name:
            # its whole model, convention too: the decompiler leaves the default one
            # (__stdcall16near) out, and a function with no convention named is compiled with
            # OpenWatcom's own -- arguments in registers, where its callers push them
            words = head0.replace("(", " ").replace(")", " ").replace("*", " ").split()
            marker = model_of.get(name) if not set(words) & {"__stdcall", "__cdecl", "__pascal"} else None
            at = named.start_byte
            raw = (raw.encode("utf-8")[:at] + ("%s " % (marker or "__" + distance[name])).encode("utf-8") +
                   raw.encode("utf-8")[at:]).decode("utf-8")
        far = set()
        struct_notes += [ln for ln in raw.split("\n") if ln.startswith("/*@S ")]
        for mm in _FACT.finditer(raw):
            if mm.group(1) == "P" and mm.group(3) == "4":
                far.add(mm.group(2))
            elif mm.group(1) == "G":
                a = int(mm.group(2), 16)
                sizes[a] = max(sizes.get(a, 0), int(mm.group(3))) if int(mm.group(3)) <= 4 else sizes.get(a, 2)
                if mm.group(4) and int(mm.group(4)) in (1, 2, 4):
                    elems[a] = int(mm.group(4))
        text = _without_notes(_FACT.sub("", raw))            # notes the decompiler put before the function
        seg, off = d.where[va]
        widths = _far_argument_widths(d.images[seg], off, ends.get(va, off),
                                      "far" in text.split("(", 1)[0]) if seg in d.images else {}
        # (_far and _near_calls are the back end's for C written from L2: docs/IR_UNIFICATION.md step 3)
        far_done = (text.strip() if "far_from_stack" in moved else _far_from_stack(text.strip(), far)) \
            if "far" in moved else _far(text.strip(), far)
        text = _null_is_zero(_segment_registers(_joined_arguments(_pieces(_frame_through_ss(far_done)), widths)))
        text = _global_pointer_steps(text if "near_calls" in moved else _near_calls(text))
        if seg in d.images:
            text = _far_arguments(text, _far_loads(d.images[seg], off, ends.get(va, off)), "far" not in text.split("{", 1)[0])
        if "return" in far:                                  # the function returns a far pointer
            text = _far_result(text, name)
            m.wide.add(name)
        head = text.split("{", 1)[0]
        if head.split()[0] == "void":
            # It sets no result -- but its callers may still read AX, and the decompiler shows
            # them doing so. Declared as returning a word, with no `return`, it leaves AX as the
            # original does: whatever was last there.
            m.novalue.add(name)
            text = "xunknown2" + text[4:]
            head = text.split("{", 1)[0]
        byte_result = _byte_result(text)
        if byte_result:
            # A result the decompiler sized as a byte is returned in AX all the same: Windows'
            # BOOL is an int, and callers test the whole of AX (a byte would leave AH as it was)
            text = ("int2 " if byte_result[0] in ("char", "int1") else "uint2 ") + text[byte_result[1]:]
            head = text.split("{", 1)[0]
        if not ("__stdcall16far" in head or "__cdecl16far" in head or _far_before_name(text, name)):
            m.near.add(name)                                 # near is the default and is not printed
        elif seg in d.images and bytes(d.images[seg][off:off + 3]) in (b"\x8c\xd8\x90", b"\x1e\x58\x90"):
            # The original starts `mov ax,ds; nop` -- the prologue Windows rewrites, for an export,
            # to load the module's own data segment: the source marked it _export, and so does
            # this (the compiler then writes that prologue: build/functions.py)
            text = _before_name(text, name, "__export ")
            m.exported_prologue.add(name)
        if seg in d.images and d.data_segment and _loads_dgroup(d.images[seg], off, describe.selector(d.data_segment)):
            # it loads its own data segment, whoever calls it -- `mov ax, DGROUP` / `mov ds, ax`,
            # first or after its frame is set up (the source's _loadds): so does this
            text = _before_name(text, name, "__loadds ")
        if head.split()[0] in _WIDE or "*" in head.split(name)[0] and "__far" in head.split(name)[0]:
            m.wide.add(name)
        text = _one_name_for_globals(text)                  # (a global the decompiler named twice)
        if by_selector:
            text = _selector_structs(text, name, selector_structs, shared_selectors)
            text = _struct_gaps(text, _pointer_distances(text))
        text = _own_segment_calls(text, seg)
        m.texts[name] = _near_to_far(_far_reads(_buffer_fields(_far_concat_casts(_far_locals(_stack_arguments(_entry_sp(_far_casts(_far_addresses(_interrupts(_open_arguments(text), m.interrupts))))))))))
        if seg in d.images:
            m.texts[name] = _stack_segment_stores(m.texts[name], d.images[seg], off, _end_of(d, seg, off))
        if callee_pops.get(va):
            # its calls through pointers remove nothing themselves: the callee does (the arguments
            # are listed in stack order, so __stdcall lays them out as the original pushed them)
            m.texts[name] = _callee_pops(m.texts[name])
        m.order.append(name)
        m.by_va[va] = name
        m.where[va] = d.where.get(va)
    # an import's value used bare (mov es, __0040H; test ax, __WINFLAGS) is the import
    values = getattr(d, "selector_imports", {})

    def _value(number):
        if int(number, 16) not in values:
            return number
        imp, how = values[int(number, 16)]
        m.selector_imports[imp] = how
        return "%s(&%s)" % ("FP_SEG" if how == "segment" else "FP_OFF", imp)
    if values:
        for name in m.texts:
            src = ctree.Source(m.texts[name])
            for num in _numbers(src):
                if re.fullmatch(r"0x[dD]0[0-9a-fA-F]{2}", src.at(num)):
                    new = _value(src.at(num))
                    if new != src.at(num):
                        src.replace(num, new)
            m.texts[name] = src.text()
    # A segment of the module's own as a value -- its selector passed on, pushed, left in AX: the
    # decompiler shows the made-up selector the description gave it. The automatic data segment
    # is DS; a code segment, the selector of a far function in it (the loader fixes that one up).
    own = {}
    for va, n in m.by_va.items():
        sg = (m.where.get(va) or (None,))[0]
        if sg is not None and n not in m.near:
            own.setdefault(sg, n)
    sel_expr = {}
    for sg in set(getattr(d, "code", {})) | ({d.data_segment} if d.data_segment else set()):
        made_up = describe.FIRST_SELECTOR + 8 * (sg - 1)
        if sg == d.data_segment:
            sel_expr[made_up] = "codebreaker_ds()"
        elif sg in own:
            sel_expr[made_up] = "((uint2)((uint4)(void __far *)&%s >> 16))" % own[sg]
    # ... and a data segment besides the automatic one, by the bytes the build carries for it
    # (USER's segment 44 handed to GetProfileString as the far pointer to a string in it)
    other_data = set(getattr(d, "images", {})) - set(getattr(d, "code", {})) - {d.data_segment}
    for sg in other_data:
        sel_expr[describe.FIRST_SELECTOR + 8 * (sg - 1)] = \
            "((uint2)((uint4)(void __far *)codebreaker_segment_%d >> 16))" % sg
    # ... where the function's own code holds a loader fix-up for that segment's selector: a
    # number like 1000h is as often a plain constant
    sites = _selector_sites(module_path)
    starts = {}
    for va, n in m.by_va.items():
        sg_off = m.where.get(va)
        if sg_off:
            starts.setdefault(sg_off[0], []).append(sg_off[1])
    for sg in starts:
        starts[sg].sort()
    at_place = {m.where[va]: n for va, n in m.by_va.items() if m.where.get(va)}
    if sel_expr:
        for va, name in m.by_va.items():
            sg_off = m.where.get(va)
            if not sg_off or name not in m.texts:
                continue
            sg, off = sg_off
            later = [o for o in starts[sg] if o > off]
            end = later[0] if later else 0x10000
            wanted = {describe.FIRST_SELECTOR + 8 * (t - 1) for (o, t) in sites.get(sg, ()) if off <= o < end}
            here = {v: e for v, e in sel_expr.items() if v in wanted}
            if not here:
                continue
            spelled = {"0x%x" % v: v for v in here}
            src = ctree.Source(m.texts[name])
            for num in _numbers(src, under=src.body()):
                if src.at(num) in spelled:
                    src.replace(num, here[spelled[src.at(num)]])
            t = src.text()
            # a far pointer passed whole -- its offset the argument before its selector: a
            # function's place, written by the function's name (USER's sub_1_3514 passes the
            # callback 1:3452 to an enumeration as `0x3452, <segment 1>`: a number, it would
            # still go where the function was if that one moved)
            src = ctree.Source(t)
            by_expr = {expr: (v - describe.FIRST_SELECTOR) // 8 + 1 for v, expr in here.items()}
            for args in src.find("argument_list", under=src.body()):
                items = args.named_children
                for a, b in zip(items, items[1:]):
                    sg_t = by_expr.get(src.at(b).strip())
                    # (an offset of 8000h or more the core writes negative: GetFilePortName's
                    # dialog procedure 1:9D0F was `-0x62f1`, left a number)
                    lit = re.fullmatch(r"(-?)0x([0-9a-f]+)", src.at(a).replace(" ", ""))
                    if sg_t is None or a.type not in ("number_literal", "unary_expression") or not lit:
                        continue
                    fn = at_place.get((sg_t, (-int(lit.group(2), 16) if lit.group(1) else int(lit.group(2), 16)) & 0xFFFF))
                    if fn:
                        src.replace(a, "((uint2)(uint4)(void __far *)&%s)" % fn)
            m.texts[name] = src.text()
    body = "\n".join(m.texts.values())
    called = set().union(*(_called(t) for t in m.texts.values())) if m.texts else set()
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
        if any(re.search(r"\bcodebreaker_segment_%d\b" % sg, t) for t in m.texts.values()):
            m.segments[sg] = d.images[sg]
    # a global the code calls through holds a function pointer -- near or far by its size
    called_through = set().union(*(_called_through(t) for t in m.texts.values())) if m.texts else set()
    for prefix, addr in sorted(set().union(*(_globals(t) for t in m.texts.values())) if m.texts else ()):
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
    agreed = prototypes(m.texts, m.order) if os.environ.get("CB_PROTOTYPES", "1") != "0" else {}
    m.prototyped = set(agreed)
    decls += [agreed.get(n) or declaration(m.texts[n]) for n in m.order]
    decls += ["#ifdef __WATCOMC__\n#pragma aux (codebreaker_fastcall) %s;\n#endif" % n for n in m.order
              if "__fastcall16near" in m.texts[n].split("(", 1)[0]]
    intrinsics = [INTRINSICS16[n] for n in sorted(INTRINSICS16) if n in called]
    intrinsics += interrupt_helpers(m.interrupts)
    if ("MK_FP(" in body or getattr(m, "bios_data", False)) and not m.selector_imports:
        intrinsics.append("#define MK_FP(s, o) ((void __far *)(((uint4)(uint2)(s) << 16) | (uint2)(o)))")
    selector_structs += _shared_selector_structs(shared_selectors, m.texts)
    if selector_structs:
        intrinsics.append(SEG_PTR)
    m.header = HEADER + "\n" + "\n".join(intrinsics + macros + _struct_decls(struct_notes, HEADER, True) +
                                          selector_structs + decls) + "\n"
    m.description = d
    return m


# Software interrupts. The decompiler gives `INT n` as a call to int<n> (languages/x86-16.cspec):
# its arguments are the registers set before it, in the order AX BX CX DX SI DI DS ES, and what it
# hands back besides AX is read as int_<register> (int_CF, int_BX, ...). In the C, a call of int21
# setting AX, CX and DX is int21_ax_cx_dx(ax, cx, dx), a helper that puts them in those registers,
# takes the interrupt (an assembly routine, codebreaker_int21, the build writes: build/module.py,
# interrupt_routines) and returns DX:AX; int_<register> is the register as the interrupt left it.
# A register the code did not set before the interrupt (the decompiler passes it on as in_BX,
# unaff_DS ...) is not an argument: it keeps the value it has -- DS the caller's data segment.
INTERRUPT_REGISTERS = ("ax", "bx", "cx", "dx", "si", "di", "ds", "es")
_INT_CALL_NAME = re.compile(r"int[0-9a-f]{2}")
_INT_RESULT = re.compile(r"int_E?([A-Z][A-Z])(?:_[0-9a-f]+)?")


def _arguments_at(text, open_paren):
    """The arguments of the call whose `(` is at `open_paren`. -> ([argument text], index after `)`)"""
    depth, args, start, i = 0, [], open_paren + 1, open_paren
    while i < len(text):
        c = text[i]
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
            if depth == 0:
                if text[start:i].strip():
                    args.append(text[start:i])
                return args, i + 1
        elif c == "," and depth == 1:
            args.append(text[start:i])
            start = i + 1
        i += 1
    return args, len(text)


def _interrupts(text, used):
    """A function's interrupts and their results as the C helpers take them (see above)."""
    if "int" not in text:
        return text

    def calls(src):
        for n in src.find("call_expression"):
            m = re.fullmatch(r"int([0-9a-f]{2})", src.at(n.child_by_field_name("function")))
            if not m or any(_INT_CALL_NAME.fullmatch(src.at(up.child_by_field_name("function")))
                            for up in src.ancestors(n, "call_expression")):
                continue                                     # (one inside another's arguments: as it was)
            args = src.raw_arguments(n)[:len(INTERRUPT_REGISTERS)]
            regs = tuple(r for r, a in zip(INTERRUPT_REGISTERS, args) if not _passed_on(a, r))
            values = [a for r, a in zip(INTERRUPT_REGISTERS, args) if r in regs]
            used.add((int(m.group(1), 16), regs))
            src.replace(n, "%s(%s)" % (interrupt_helper_name(int(m.group(1), 16), regs), ",".join(values)))
    text = ctree.rewrite(text, calls)

    # the results: no variables of their own -- the registers as the interrupt left them
    def results(src):
        for n in src.find("declaration"):
            ds = n.children_by_field_name("declarator")
            name = src.declared_name(ds[0]) if len(ds) == 1 else None
            if name and _INT_RESULT.fullmatch(name) and src.at(n) == src.line(n).strip():
                src.replace(src.line_span(n), "")
        for n in src.find("identifier"):
            m = _INT_RESULT.fullmatch(src.at(n))
            if m and n.parent.type != "declaration" and not (n.parent.type == "pointer_declarator"):
                src.replace(n, "int_" + m.group(1))
    return ctree.rewrite(text, results)


_CALLER_SLOT = re.compile(r"\*\((\w+) \*\)\(unaff_BP \+ -(0x[0-9a-f]+|\d+)\)")


def _keeps_frame(code, off):
    """Does the function at `off` set up BP as its frame -- Windows' far prologue, `push bp /
    mov bp, sp`, or `enter`?"""
    from codebreaker.decompile import describe
    head = bytes(code[off:off + 3])
    return describe.windows_far_prologue(code, off) or head in (b"\x55\x8b\xec", b"\x55\x89\xe5") or head[:1] == b"\xc8"


def _caller_frame_slots(texts, frames):
    """A helper that works in its caller's frame: it makes no frame of its own and reaches places
    through BP as its caller left it -- KEYBOARD's sub_9_0A62, `xor ax, ax / mov [bp-4], ax /
    mov [bp-6], ax / ret`, which clears two of OEMTOANSI's locals. The decompiler writes
    `*(xunknown2 *)(unaff_BP + -4) = 0`, a BP the C never has, and the caller's locals stay
    unset (its loop compared the pointers against whatever the frame held). Here each such place
    becomes a parameter -- a far pointer, the frame being in SS -- and each call passes the
    address of the caller's own local there: [bp-4] of a frame BP set up is entry SP - 6, the
    caller's xStack_6. Only where every place is below BP, every caller keeps a frame of its own
    (`frames`: function -> bool), and every use of the helper is a plain call of it; otherwise
    the C is left as it was. -> {function: text}"""
    out = dict(texts)
    for name, text in texts.items():
        hm = re.search(r"\b(__cdecl16(?:near|far)) %s\((void)?\)" % re.escape(name), text)
        if not hm or "{" not in text[hm.end():] or not re.search(r"(?m)^\s+int2 unaff_BP;$", text):
            continue
        at = text.index("{", hm.end())
        head, body = text[:at], text[at:]
        uses = len(re.findall(r"\bunaff_BP\b", body))
        slots = {}                                           # BP less -> the type first reached there
        for m in _CALLER_SLOT.finditer(body):
            slots.setdefault(int(m.group(2), 0), m.group(1))
        if not slots or uses != 1 + len(_CALLER_SLOT.findall(body)) or \
                any(t not in _SIZEOF for t in slots.values()):
            continue
        callers = {n: t for n, t in texts.items() if n != name and re.search(r"\b%s\b" % re.escape(name), t)}
        if not callers or any(not frames.get(n) or "unaff_BP" in t or
                              len(re.findall(r"\b%s\b" % re.escape(name), t)) !=
                              len(re.findall(r"\b%s\(\)" % re.escape(name), t)) for n, t in callers.items()):
            continue
        # the caller's local at each place: declared alone, as wide as what the helper puts there
        found, ok = {}, True
        for n, t in callers.items():
            own = {int(m.group(3), 16): (m.group(1), m.group(2)) for m in
                   re.finditer(r"(?m)^\s+([\w ]+?\**) ?(\w*Stack_([0-9a-f]+));$", t)}
            arrays = re.search(r"(?m)^\s+[\w ]+?\**\s*\w*Stack_[0-9a-f]+ \[\d+\];", t)
            for less, ty in slots.items():
                got = own.get(less + 2)
                width = 2 if got and "*" in got[0] else _SIZEOF.get(got[0].strip()) if got else None
                if got is None and arrays or got is not None and width != _SIZEOF[ty]:
                    ok = False
                found[(n, less)] = got[1] if got else None
        if not ok:
            continue
        params = ["%s __far *frame_%x" % (slots[less], less) for less in sorted(slots)]
        new_body = _CALLER_SLOT.sub(lambda m: "*frame_%x" % int(m.group(2), 0) if m.group(1) == slots[int(m.group(2), 0)]
                                    else "*(%s __far *)frame_%x" % (m.group(1), int(m.group(2), 0)), body)
        new_body = re.sub(r"(?m)^\s+int2 unaff_BP;\n", "", new_body, count=1)
        out[name] = head[:hm.start()] + "%s %s(%s)" % (hm.group(1), name, ",".join(params)) + head[hm.end():] + new_body
        for n, t in callers.items():
            t = out[n]
            args = []
            for less in sorted(slots):
                local = found[(n, less)]
                if local is None:                            # (a place the caller's C never reads)
                    local = "xStack_%x" % (less + 2)
                if not re.search(r"(?m)^\s+[\w ]+?\**\s*%s;$" % local, t):
                    own = re.search(r"\b%s\(" % re.escape(n), t)
                    at = t.find("{\n", own.end() if own else 0)
                    t = t[:at] + "{\n  %s %s;\n" % (slots[less], local) + t[at + 2:]
                args.append("(%s __far *)MK_FP(codebreaker_ss(),&%s)" % (slots[less], local))
            out[n] = re.sub(r"\b%s\(\)" % re.escape(name), "%s(%s)" % (name, ",".join(args)), t)
    return out


def _entry_sp(text):
    """The decompiler's name for SP as the function was entered, used as a value (code that keeps
    the stack pointer, or makes an address on the stack from it): codebreaker_entry_sp(). A place
    on the stack it names by that (stack0xffec: entry SP - 0x14, above the function's own frame,
    the caller's) is that address -- unless it is one of the function's own locals (xStack_4:
    entry SP - 4) read through its address: then it is that local, wherever the compiler keeps
    it. (SUWIN's 2:819E keeps GetProcAddress's DX:AX in [bp-4] and calls `lcall [bp-4]`; the C
    read the segment as `*(xunknown2 *)(&stack0xfffa + 2)`, a place in its own frame nothing had
    set, and called into nowhere.)"""
    own = {int(m.group(3), 16): (m.group(1), m.group(2))     # entry SP less -> (type, name)
           for m in re.finditer(r"^\s+(\w+) (\w*Stack_([0-9a-f]+));$", text, re.M)}

    def local(m):
        ty, base, plus = m.group(1), int(m.group(2), 16), m.group(3)
        hit = own.get(0x10000 - base - (int(plus, 0) if plus else 0))
        if not hit:
            return m.group(0)
        return hit[1] if hit[0] == ty else "*(%s *)&%s" % (ty, hit[1])
    if own:
        text = re.sub(r"\*\((\w+) \*\)\(&stack0x([0-9a-f]{4}) \+ (\d+|0x[0-9a-f]+)\)", local, text)
        text = re.sub(r"\*\((\w+) \*\)&stack0x([0-9a-f]{4})()\b", local, text)

    def fix(src):
        for n in src.find("identifier"):
            name = src.at(n)
            if name == "register0x00000010":
                src.replace(n, "codebreaker_entry_sp()")
                continue
            m = re.fullmatch(r"stack0x([0-9a-f]{4})", name)
            op = src.unary(n) if m else None
            if op and op[0] == "&":
                src.replace((op[1], n.end_byte), "((uint1 *)codebreaker_entry_sp() + (int2)0x%s)" % m.group(1))
    return ctree.rewrite(text, fix)


def _stack_arguments(text):
    """Words of the caller's stack the function reads beyond its declared parameters: the
    decompiler declares them as locals (in_stack_0000000c -- which in C would hold nothing) or
    names a piece of them (_uStack000e). Each is that place: the stack segment, at the entry SP
    plus its offset."""
    def place(ty, off):
        return "codebreaker_stack(%s, 0x%x)" % (ty, off)

    def declared_alone(src, n):
        """(type, name) of a declaration of one plain variable alone on its line, else None"""
        ds = n.children_by_field_name("declarator")
        if len(ds) != 1 or ds[0].type != "identifier":
            return None
        line = src.line(n)
        if src.at(n) != line.strip():
            return None
        return line.strip()[:-1].rsplit(None, 1)[0].strip(), src.at(ds[0])

    def arguments(src):
        locals_ = {}
        for n in src.find("declaration", under=src.body()):
            got = declared_alone(src, n)
            m = re.fullmatch(r"in_stack_([0-9a-f]{8})", got[1]) if got else None
            if m and int(m.group(1), 16) < 0x10000 and got[1] not in locals_:
                locals_[got[1]] = (got[0], int(m.group(1), 16))
                src.replace(src.line_span(n), "")
        for n in src.find("identifier"):
            name = src.at(n)
            if name in locals_ and n.parent.type != "declaration":
                src.replace(n, place(*locals_[name]))
    text = ctree.rewrite(text, arguments)

    def pieces(src):
        body = src.body()
        declared = set()
        for n in src.find("declaration", under=body):
            if src.at(n) == src.line(n).strip():
                for dcl in n.children_by_field_name("declarator"):
                    while dcl.type in ("pointer_declarator", "array_declarator"):
                        dcl = dcl.child_by_field_name("declarator")
                    declared.add(src.at(dcl))
        for n in src.find("identifier"):
            m = re.fullmatch(r"_?([a-z]+)Stack([0-9a-f]{4})", src.at(n))
            if m and src.at(n) not in declared:
                ty = {"u": "uint2", "i": "int2", "c": "char", "b": "uint1", "x": "xunknown2"}.get(m.group(1)[:1], "uint2")
                src.replace(n, place(ty, int(m.group(2), 16)))
    return ctree.rewrite(text, pieces)


_SIZE = {"1": 1, "2": 2, "4": 4}


def _elem16(typ):
    """Bytes in one of a 16-bit local's type, as written: `int2`, `uint1`, `char`, `code *`."""
    typ = (typ or "").strip()
    if typ.endswith("*"):
        return 2
    return _SIZE.get(typ[-1:]) or (1 if typ in ("char", "uchar", "byte") else 2)
# (a name of its own: it was `_LOCAL` too, and replaced the 32-bit one above for every pass that
# read it -- _keep_frame among them. A near pointer local is two bytes, its type kept for casts.)
_LOCAL16 = re.compile(r"(?m)^  (\w+?)(\d)?(\s*\*+\s*|\s+)(\w*Stack_([0-9a-f]+))( \[(\d+)\])?;\s*$")






def _near_to_far(text):
    """`(T __far *)(n)` of a number `n` -- an offset in the data segment, as the original reads
    it through DS: C makes a far pointer of a number with segment 0. Through a near pointer it
    gets DS: `(T __far *)(T *)(n)`. (Not where the value is far already: MK_FP, a far cast.)"""
    # (the function's four-byte variables already hold a segment and an offset: left as they are)
    def fix(src):
        wide = set()                                         # (four-byte variables and arrays of them)
        for n in src.find("declaration"):
            if src.at(n.child_by_field_name("type")) in ("int4", "uint4", "xunknown4", "undefined4", "float4"):
                dcl = n.children_by_field_name("declarator")
                dcl = dcl[0] if dcl else None
                while dcl is not None and dcl.type in ("init_declarator", "array_declarator"):
                    dcl = dcl.child_by_field_name("declarator")
                if dcl is not None and dcl.type == "identifier":
                    wide.add(src.at(dcl))
        for n in src.find("cast_expression"):
            far = re.fullmatch(r"(\w+(?: \w+)?) __far \*", src.at(n.child_by_field_name("type")))
            value = n.child_by_field_name("value")
            if not far or value.type != "parenthesized_expression":
                continue
            inner = src.at(value)
            if "__far" in inner or "MK_FP" in inner or "CONCAT" in inner or "&" in inner \
                    or re.search(r"\b(?:int4|uint4|xunknown4|undefined4)\b", inner) \
                    or any(re.search(r"\b%s\b" % re.escape(w), inner) for w in wide) \
                    or re.fullmatch(r"\(\s*0\s*\)", inner):          # (a null pointer stays null)
                continue
            src.insert(value.start_byte, "(%s *)" % far.group(1))
    return ctree.rewrite(text, fix)


def _far_reads(text):
    """`*(T *)((U __far *)MK_FP(s, o))` -- a far place read (or written) through a near cast:
    the segment dropped, DS:o in its stead (a RECT read out of the caller's segment, read out of
    the module's own). What is reached through a far pointer is reached far."""
    def fix(src):
        for cast in src.find("cast_expression"):
            op = src.unary(cast)
            if not op or op[0] != "*":
                continue
            ty = cast.child_by_field_name("type")
            near = re.fullmatch(r"(\w+(?: \w+)?) \*", src.at(ty))
            inner = cast.child_by_field_name("value")
            inner = inner.named_children[0] if inner.type == "parenthesized_expression" and inner.named_child_count == 1 else None
            if near and inner is not None and inner.type == "cast_expression" and \
                    re.fullmatch(r"\w+ __far \*", src.at(inner.child_by_field_name("type"))):
                src.insert(ty.start_byte + len(near.group(1)), " __far")
    return ctree.rewrite(text, fix)


def _declaration_lines(text):
    """The lines of the function's body that are a declaration and nothing else, as written --
    what a pattern for `  T name;` may take apart (it took `  return uStack_6;` for one, of type
    `return`)."""
    src = ctree.Source(text)
    return {src.line(n) for n in src.find("declaration", under=src.body()) if src.at(n) == src.line(n).strip()}


def _is_declarator(n):
    """`n` is the name a declaration declares, not a use of it"""
    up = n.parent
    while up is not None and up.type in ("pointer_declarator", "array_declarator", "init_declarator",
                                         "parenthesized_declarator"):
        if up.type == "array_declarator" and up.child_by_field_name("declarator") != n and \
                up.child_by_field_name("size") is not None and up.child_by_field_name("size").start_byte <= n.start_byte:
            return False                                 # (in an array's size)
        if up.type == "init_declarator" and up.child_by_field_name("value") is not None and \
                up.child_by_field_name("value").start_byte <= n.start_byte:
            return False                                 # (in an initial value)
        n, up = up, up.parent
    return up is not None and up.type in ("declaration", "parameter_declaration")


def _uses(text, name):
    """Where `name` is used (not declared), outside strings and comments: [node], and the source."""
    src = ctree.Source(text)
    return [n for n in src.find("identifier") if src.at(n) == name and not _is_declarator(n)], src


def _rename(text, name, new):
    """Every use of the variable `name` written as `new` (its declaration left as it is)."""
    nodes, src = _uses(text, name)
    for n in nodes:
        src.replace(n, new)
    return src.text()


def _addressed(text, name):
    """Is the address of `name` (or of an element of it) taken: `&name`, `&name[2]`?"""
    nodes, src = _uses(text, name)
    for n in nodes:
        op = src.unary(n)
        if op and op[0] == "&":
            return True
        up = n.parent
        if up is not None and up.type == "subscript_expression" and up.child_by_field_name("argument") == n:
            op = src.unary(up)
            if op and op[0] == "&" and op[1] + 1 == n.start_byte:
                return True
    return False


def _passed(text, name):
    """Is `name` handed over -- right after a `(` or a `,`, through a cast of a word or a pointer
    to one and an `&` at most (`f(x, &name)`, `CONCAT22(s, (int2)name)`)?"""
    nodes, src = _uses(text, name)
    return any(re.search(r"[(,]\s*(?:\(\w+\s*\*?\))?\s*&?\s*\Z", src.data[:n.start_byte].decode("utf-8")) for n in nodes)


def _buffer_fields(text):
    """A local the function never sets, lying past the end of an array local whose address it
    passes on, is a field of that buffer: the callee fills the buffer, the function reads the
    field (the analysis sized the array to reach the next thing it saw used, and made that a
    variable of its own -- which no one writes). It is read where it is, in the buffer, and
    the buffer is made to reach it."""
    head, brace, body = text.partition("{")
    lines = _declaration_lines(text)
    decl = {}
    for mm in _LOCAL16.finditer(body):
        if mm.group(0).split("\n")[0] not in lines:
            continue                                     # (a statement the pattern took for a declaration)
        typ, width, stars, name, off, _a, count = mm.groups()
        stars = (stars or "").strip()
        elem = 2 if stars else _SIZE.get(width or "", 2) if width else _elem16(typ or "")
        size = int(count) * elem if count else elem                  # (bytes)
        decl[name] = (int(off, 16), size, bool(count), mm.group(0),
                      (typ or "") + (width or "") + (" " + stars if stars else ""))
    text = head + brace + body
    # A local whose address is passed on, with locals of its width right after it in the frame:
    # one buffer of them (a RECT copied into four words, its address given to a function that
    # converts it in place -- four variables of their own, only the first where the callee
    # writes). Made one array, as the frame has them.
    by_off = {v[0]: n for n, v in decl.items() if not v[2]}
    for name, (off, size, is_array, line, typ) in sorted(decl.items(), key=lambda kv: -kv[1][0]):
        if is_array or name not in decl or not _addressed(text, name):
            continue
        run, at = [name], off - size
        while at in by_off and decl.get(by_off[at], (0, 0))[1] == size and by_off[at] not in run \
                and decl[by_off[at]][4] == typ \
                and not _addressed(text, by_off[at]):
            run.append(by_off[at])
            at -= size
        if len(run) < 2:
            continue
        arr = "a" + name
        text = _replace_in_body(text, line, "  %s %s [%d];" % (typ or "int2", arr, len(run)))
        for k, n in enumerate(run):
            if k:
                text = _replace_in_body(text, decl[n][3], "")
            text = _rename(text, n, "%s[%d]" % (arr, k))
            decl.pop(n, None)
        decl[arr] = (off, size * len(run), True, "  %s %s [%d];" % (typ or "int2", arr, len(run)), typ)
    arrays = {n: v for n, v in decl.items() if v[2] and _passed(text, n)}
    if not arrays:
        return head + brace + body if text == head + brace + body else text
    # A buffer whose address is passed on is laid out as the original's frame had it: from its
    # place up to the next buffer passed on, or the top of the frame (the saved BP, at 2) -- what
    # the callee reads or fills is as long as the frame made room for (GetObject's 14-byte BITMAP;
    # a WNDCLASS built word by word, then its address given to RegisterClass). Every local in that
    # stretch, read or written, is a field of it, where it is: C would otherwise place them where
    # it likes, and the callee would read past the buffer into whatever is there.
    for owner, (ao, asz, _i, aline, _atyp) in list(arrays.items()):
        others = [v[0] for k, v in arrays.items() if k != owner and v[0] < ao]
        top = max(others + [2])
        need = ao - top
        if asz < need <= 0x400:
            elem = _elem16(_atyp)
            newline = _resized(aline, (need + elem - 1) // elem)
            text = _replace_in_body(text, aline, newline)
            arrays[owner] = (ao, need, True, newline, _atyp)
    for name, (off, size, is_array, line, typ) in sorted(decl.items()):
        if name in arrays:
            continue
        if not _uses(_replace_in_body(text, line, ""), name)[0]:
            continue
        owner = next((a for a, (ao, asz, _i, _l, _t) in arrays.items() if ao > off >= ao - asz), None)
        if owner is None:
            continue
        ao, asz, _i, aline, _atyp = arrays[owner]
        at = ao - off                                        # bytes into the buffer
        text = _replace_in_body(text, line, "")
        # (an array there is the rest of the buffer too: USER's sub_2_03A4 fills 'a'..'z' and
        # 'A'..'Z' as two arrays of 26 and measures the 52 as one string -- two arrays of their
        # own, the compiler put them apart, and every dialog came out the wrong width)
        place = "(%s *)((uint1 *)%s + 0x%x)" % (typ or "int2", owner, at)
        text = _rename(text, name, "(%s)" % place if is_array else "(*%s)" % place)
        if is_array:
            need = at + size
            if need > asz:
                elem = _elem16(_atyp)
                newline = _resized(aline, (need + elem - 1) // elem)
                text = _replace_in_body(text, aline, newline)
                arrays[owner] = (ao, need, True, newline, _atyp)
            continue
        need = at + _elem16(typ)
        if need > asz:
            elem = _elem16(_atyp)
            newline = _resized(aline, (need + elem - 1) // elem)
            text = _replace_in_body(text, aline, newline)
            arrays[owner] = (ao, need, True, newline, _atyp)
    text = _frame_block(text, arrays)
    head, brace, body = text.partition("{")
    while "\n\n\n" in body:
        body = body.replace("\n\n\n", "\n\n")
    return head + brace + body


def _replace_in_body(text, old, new):
    """`old` (a line of the body, as written) replaced by `new`, once."""
    head, brace, body = text.partition("{")
    return head + brace + body.replace(old, new, 1)


def _resized(line, count):
    """An array's declaration with another element count: `  char a [4];` -> `  char a [6];`"""
    left, _sep, right = line.rpartition("[")
    return "%s[%d]%s" % (left, count, right[right.index("]") + 1:]) if _sep and "]" in right else line


def _frame_block(text, arrays):
    """The buffers a function passes on, kept in the order and at the distances the original's
    frame had them: one packed structure. What a callee writes past one buffer then lands in the
    next one, as in the original -- USER's sub_3_1C4E has GetObject write a 50-byte LOGFONT into
    an 18-byte buffer, running on into the 32-byte one after it, which the function fills later
    anyway. Laid out by the compiler instead, the 50 bytes went over the saved DS and the return
    address. {name: (offset below BP, size, _, its declaration, type)}"""
    if len(arrays) < 2:
        return text
    order = sorted(arrays.items(), key=lambda kv: -kv[1][0])          # (lowest address first)
    fields, at = [], None
    for k, (name, (off, size, _i, line, _typ)) in enumerate(order):
        if at is not None and at > off:
            fields.append("    uint1 codebreaker_gap%d [%d];" % (k, at - off))
        elif at is not None and at < off:
            return text                                                 # (overlapping: left alone)
        fields.append("  " + line.strip())
        at = off - size
    for name, (off, size, _i, line, _typ) in order:
        text = _replace_in_body(text, line, "")
    decl = "  _Packed struct {\n%s\n  } codebreaker_frame;\n" % "\n".join(fields)
    # (the declaration first among the locals: after the brace that opened the body)
    head, brace, body = text.partition("{")
    text = head + brace + "\n" + decl + (body[1:] if body.startswith("\n") else body)
    for name in arrays:                                  # (the fields' own declarations keep their names)
        text = _rename(text, name, "codebreaker_frame.%s" % name)
    return text


def _end_of(d, seg, off):
    """Where the function at seg:off ends: the next function's start in its segment. (Kept on the
    description: kept by id() it was given to the next module's description when that one came
    to the same address -- and its functions were cut at the other module's starts.)"""
    ends = d.__dict__.setdefault("_ends", {})
    if seg not in ends:
        ends[seg] = sorted(o for (s, o) in d.where.values() if s == seg)
    starts = ends[seg]
    import bisect
    k = bisect.bisect_right(starts, off)
    return starts[k] if k < len(starts) else len(d.images.get(seg, b""))


def _stack_segment_stores(text, code, start, end):
    """The original stores SS into its frame (`mov [bp-0Ch], ss`) as the segment half of a far
    pointer it builds there to a local -- WNDCLASS.lpszClassName, CBT_CREATEWND.lpcs -- and the
    decompiler drops that store (a copy of the incoming SS, taken for a register saved). Put back
    where the buffer holding that place has it: SS written there just before the first call the
    buffer's address is passed to."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    places = []
    for ins in md.disasm(bytes(code[start:end]), start):
        mm = re.fullmatch(r"word ptr \[bp - (0x[0-9a-f]+|\d+)\], ss", ins.op_str) if ins.mnemonic == "mov" else None
        if mm:
            places.append(int(mm.group(1), 0) + 2)              # (the decompiler's offset: from the entry SP)
    if not places:
        return text
    src = ctree.Source(text)
    arrays = {}
    # (a buffer kept in the frame's layout is a field of codebreaker_frame's packed struct: a
    # field declaration, not a declaration)
    decls = list(src.find("declaration", under=src.body())) + list(src.find("field_declaration", under=src.body()))
    for n in sorted(decls, key=lambda d: d.start_byte):
        ds = n.children_by_field_name("declarator")
        a = ds[0] if len(ds) == 1 else None
        while a is not None and a.type == "pointer_declarator":
            a = a.child_by_field_name("declarator")
        if a is None or a.type != "array_declarator" or not src.line(n).startswith("  "):
            continue
        name, size = src.at(a.child_by_field_name("declarator")), a.child_by_field_name("size")
        m = re.fullmatch(r"\w*Stack_([0-9a-f]+)", name)
        if m and size is not None and size.type == "number_literal" and src.at(size).isdigit():
            typ = src.at(n)[:src.at(n).index(name)].strip()
            arrays.setdefault(name, (int(m.group(1), 16), int(src.at(size)), typ))
    ss = "unaff_SS" if any(src.at(n) == "unaff_SS" for n in src.find("identifier")) else "codebreaker_ss()"
    for place in sorted(set(places)):
        for arr, (ao, count, typ) in arrays.items():
            elem = 2 if "*" in typ else _SIZE.get(typ.strip()[-1:], 2)
            if not ao > place >= ao - count * elem:
                continue
            at = ao - place
            # (a buffer kept in the frame's layout is a field of codebreaker_frame by now: found
            # by its bare name the store was never put in -- USER's eleven RegisterClass callers
            # in segment 3 gave the class name's far pointer no segment)
            ref = "codebreaker_frame." + arr if "codebreaker_frame." + arr in text else arr
            store = "(*(int2 *)((uint1 *)%s + 0x%x)) = %s;" % (ref, at, ss)
            if store in text:
                break
            before = _first_passing(ctree.Source(text), arr)
            if before is not None:
                text = text[:before[0]] + before[1] + store + "\n" + text[before[0]:]
            break
        else:
            text = _far_record(text, place, ss)
    return text


def _first_passing(src, arr):
    """(where its line starts, its indentation) of the first statement with a call whose
    arguments use `arr` -- passed, or anything made from it (`f((int2)*(char **)(arr + 2), ss)`:
    the far pointer kept in the buffer handed on) -- or None. A buffer kept in the frame's layout
    is a field of codebreaker_frame by now, and found by that name too."""
    best = None
    framed = "codebreaker_frame." + arr
    opening = src.text().find("{")
    for c in src.find("call_expression", under=src.body()):
        args = c.child_by_field_name("arguments")
        if not src.is_call(c) or not (any(src.at(n) == arr for n in src.find("identifier", under=args)) or
                                      any(src.at(n) == framed for n in src.find("field_expression", under=args))):
            continue
        st = c                                       # (the statement it is in: after a label, not before it)
        while st.parent is not None and st.parent.type not in ("compound_statement", "labeled_statement"):
            st = st.parent
        if st.start_byte <= opening:
            continue                                 # (not inside the body: a header the parser misread
                                                     # -- SUWIN's sub_4_9324 got the store above it)
        if best is None or st.start_byte < best.start_byte:
            best = st
    if best is None:
        return None
    start = src.line_span(best)[0]
    line = src.line(best)
    return start, line[:len(line) - len(line.lstrip())]


def _far_record(text, place, ss):
    """The far pointer is not in a buffer but in a record of single words the decompiler split
    into separate locals -- USER's CreateWindow builds CBT_CREATEWND {lpcs = ss:&args,
    hwndInsertAfter} at [bp-26h] and passes its address to the CBT hook: the offset half a local
    of its own, the SS half dropped, the next field another local. Laid out by the compiler those
    are anywhere, and the hook read a garbage selector. Kept as one packed record, from the word
    whose address is passed on, through SS, through the words after it with no gap."""
    lines = _declaration_lines(text)

    def scalar(off):
        """(its declaration, its name, 2) of the word-sized local at `off`, else None"""
        for mm in re.finditer(r"(?m)^  ([\w ]*?\w(?:\s*\*+)?)\s*\b(\w*Stack_%x);[ \t]*$" % off, text):
            if mm.group(0) not in lines:
                continue
            typ = mm.group(1)
            size = 2 if "*" in typ else _SIZE.get(typ.strip()[-1:], 0)
            return (mm.group(0), mm.group(2), size) if size == 2 else None
        return None
    first = scalar(place + 2)
    if not first or not _addressed(text, first[1]):
        return text
    words = [first, None]
    off = place - 2
    while scalar(off):
        words.append(scalar(off))
        off -= 2
    rec = "codebreaker_far_%x" % (place + 2)
    fields = []
    for w in words:
        fields.append("    int2 codebreaker_ss;" if w is None else "  " + w[0].strip())
    for w in words:
        if w:
            text = text.replace(w[0] + "\n", "", 1)
    decl = "  _Packed struct {\n%s\n  } %s;\n" % ("\n".join(fields), rec)
    brace = text.find("{\n")
    if brace < 0:
        return text
    text = text[:brace + 2] + decl + text[brace + 2:]
    for w in words:                                      # (the fields' own declarations keep their names)
        if w:
            text = _rename(text, w[1], "%s.%s" % (rec, w[1]))
    # (SS written with the offset: where the offset is set -- a statement of its own line)
    src = ctree.Source(text)
    for st in src.find("expression_statement", under=src.body()):
        a = st.named_children[0] if st.named_child_count == 1 else None
        line = src.line(st)
        if a is not None and a.type == "assignment_expression" and src.at(a.child_by_field_name("operator")) == "=" \
                and src.at(a.child_by_field_name("left")) == "%s.%s" % (rec, first[1]) and line.strip() == src.at(st) \
                and line[:1].isspace():
            end = src.line_span(st)[1]
            end -= 1 if src.data[end - 1:end] == b"\n" else 0
            indent = line[:len(line) - len(line.lstrip())]
            src.insert(end, "\n" + indent + "%s.codebreaker_ss = %s;" % (rec, ss))
            break
    return src.text()


def _far_concat_casts(text):
    """`(T *)CONCAT22(seg, off)` -> `(T __far *)CONCAT22(seg, off)` wherever it stands: four
    bytes, a segment and an offset, cast to a near pointer keep the offset alone and C gives it
    DS. (SETUPX's sub_31_002A passed four far pointers on as `(xunknown1 *)CONCAT22(a4,a3)` once
    its callee's arguments were stated as the far pointers they are.)"""
    if "CONCAT22" not in text:
        return text
    src = ctree.Source(text)
    for cast in src.find("cast_expression"):
        ty, v = cast.child_by_field_name("type"), cast.child_by_field_name("value")
        m = re.fullmatch(r"([\w ]+?) \*", src.at(ty)) if ty is not None else None
        if m and "__far" not in m.group(1) and v is not None and v.type == "call_expression" and \
                src.at(v.child_by_field_name("function")) == "CONCAT22":
            src.replace(ty, "%s __far *" % m.group(1))
    return src.text()


def _far_locals(text):
    """A four-byte local the code reads THROUGH holds a far pointer: it is set from a segment and
    an offset (CONCAT22(seg, off)) and the decompiler casts that to a near pointer, which in C
    would keep the offset alone. Declared as the far pointer it is, to what that cast names."""
    def concat_sets(src, name, any_value=False):
        """`name = (T *)CONCAT22(...)` (with `any_value`, `name = (T *)` anything): [(the cast's
        type node, T)]"""
        out = []
        for a in src.find("assignment_expression"):
            left, right = a.child_by_field_name("left"), a.child_by_field_name("right")
            if src.at(a.child_by_field_name("operator")) != "=" or left.type != "identifier" or src.at(left) != name:
                continue
            cast = src.leading(right, "cast_expression")
            ty = cast.child_by_field_name("type") if cast is not None else None
            m = re.fullmatch(r"([\w ]+?) \*", src.at(ty)) if ty is not None else None
            v = cast.child_by_field_name("value") if m else None
            if v is not None and (any_value or v.type == "call_expression" and
                                  src.at(v.child_by_field_name("function")) == "CONCAT22"):
                out.append((ty, m.group(1)))
        return out

    def far_casts(t, name, any_value=False):
        """`name = (T *)CONCAT22(` -> `name = (T __far *)CONCAT22(` (with `any_value`, any cast set)"""
        src = ctree.Source(t)
        for ty, to in concat_sets(src, name, any_value):
            src.replace(ty, "%s __far *" % to)
        return src.text()
    lines = _declaration_lines(text)
    for mm in list(re.finditer(r"(?m)^([ \t]+)(?:xunknown4|uint4|int4|undefined4) (\w+);[ \t]*$", text)):
        if mm.group(0) not in lines:
            continue
        name = mm.group(2)
        nodes, src = _uses(text, name)
        if not any(src.unary(n) and src.unary(n)[0] == "*" for n in nodes):
            continue                                     # (read through: `*name`, also after a cast)
        sets = concat_sets(src, name, any_value=True)
        to = sets[0][1] if sets else "xunknown1"
        text = text.replace(mm.group(0), "%s%s __far *%s;" % (mm.group(1), to, name), 1)
        # (whatever it is set from: a far pointer a call returns -- `(T *)GlobalLock(h)` -- cast
        # near first kept the offset alone and C gave it DS: USER's sub_43_032F copied from its
        # own data segment instead of the block GlobalLock locked)
        text = far_casts(text, name, any_value=True)
    # `_p` beside `p`: the decompiler's name for the whole four bytes whose low half is the near
    # pointer p -- a far pointer, and p its offset
    lines = _declaration_lines(text)
    for mm in list(re.finditer(r"(?m)^([ \t]+)([\w ]+?) \*(\w+);[ \t]*$", text)):
        if mm.group(0) not in lines:
            continue
        lead, to, name = mm.groups()
        whole = "_" + name
        nodes, src = _uses(text, whole)
        if not nodes or any(_declares(src, n, whole) for n in src.find("declaration")):
            continue
        assigned = _addressed(text, name) or any(
            src.at(a.child_by_field_name("operator")) == "=" and a.child_by_field_name("left").type == "identifier"
            and src.at(a.child_by_field_name("left")) == name for a in src.find("assignment_expression"))
        decl = "%s%s __far *%s;" % (lead, to, whole)
        text = text.replace(mm.group(0), (mm.group(0) + "\n" if assigned else "") + decl, 1)
        text = far_casts(text, whole)
        if not assigned:
            text = _rename(text, name, "((%s *)(uint2)(uint4)%s)" % (to, whole))
    # one pointer local set from another the decompiler typed differently (int2 __far ** from
    # int2 **): what is kept is the value, in the type of the one it goes into
    lines = _declaration_lines(text)
    types = {m2.group(2): m2.group(1).strip() for m2 in
             re.finditer(r"(?m)^[ \t]+([\w ]+?[\w ]*\*+)\s*(\w+);[ \t]*$", text) if m2.group(0) in lines}
    src = ctree.Source(text)
    for st in src.find("expression_statement", under=src.body()):
        a = st.named_children[0] if st.named_child_count == 1 else None
        if a is None or a.type != "assignment_expression" or src.at(a.child_by_field_name("operator")) != "=":
            continue
        left, right = a.child_by_field_name("left"), a.child_by_field_name("right")
        if left.type == "identifier" and right.type == "identifier" and src.line(st).strip() == src.at(st) and \
                src.line(st)[:1].isspace() and src.at(left) in types and src.at(right) in types and \
                types[src.at(left)] != types[src.at(right)]:
            src.insert(right.start_byte, "(%s)" % types[src.at(left)])
    return src.text()


def _declares(src, decl, name):
    """Does declaration node `decl` declare `name`?"""
    return any(src.declared_name(dcl) == name for dcl in decl.children_by_field_name("declarator"))


def _passed_on(arg, reg):
    """Is `arg` the register `reg` itself as the function had it -- not set for the interrupt?"""
    r = reg.upper()
    return arg.strip() in ("in_" + r, "in_E" + r, "unaff_" + r, "unaff_E" + r)


def interrupt_helper_name(vector, regs):
    return "int%02x_%s" % (vector, "_".join(regs) if regs else "none")


def _pointer_distances(text):
    """{name: "far" | "near"} of the function's pointer variables and parameters, by how each is
    declared (`T __far *p` far, `T *p` near) -- after _far has marked the far ones"""
    out = {}
    for m in re.finditer(r"(?<![\w*])[A-Za-z_]\w*\s+(__far\s*)?\*+\s*(\w+)\s*[,;)\[]", text):
        out.setdefault(m.group(2), "far" if m.group(1) else "near")
    return out


SEG_PTR = "#define SEG_PTR(T, s) ((T __far *)((uint4)(uint2)(s) << 16))   /* a structure at the start of a segment */"


def _seg_layout(fields):
    """the fields of a selector's structure, {offset: type}, as a declaration's body; None when
    they overlap"""
    body, at = [], 0
    for off in sorted(fields):
        if off < at:
            return None
        if off > at:
            body.append("uint1 _pad_%x[%d];" % (at, off - at))
        body.append("%s f_%x;" % (fields[off], off))
        at = off + _SIZEOF[fields[off]]
    return body


def _shared_selector_structs(shared, texts):
    """The structures of global selectors (_selector_structs' `shared`): one per global for the
    whole module -- the fields every function saw, together -- where they agree (one type per
    offset, none overlapping); where they do not, each function's own, as for a local selector.
    `texts` {function: text} has the names put right. -> declarations"""
    out = []
    for name, per in sorted(shared.items()):
        every = {}
        for fields in per.values():
            for off, ty in fields.items():
                if every.setdefault(off, ty) != ty:
                    every = None
                    break
            if every is None:
                break
        body = _seg_layout(every) if every is not None else None
        if body is not None:
            out.append("typedef _Packed struct %s { %s } %s;" % (name, " ".join(body), name))
            continue
        for fname, fields in sorted(per.items()):
            own = "seg_%s_%s" % (re.sub(r"\W", "_", fname), name[4:])
            out.append("typedef _Packed struct %s { %s } %s;" % (own, " ".join(_seg_layout(fields)), own))
            texts[fname] = texts[fname].replace("SEG_PTR(%s," % name, "SEG_PTR(%s," % own)
    return out


def _selector_structs(text, fname, decls, shared=None):
    """A segment reached through a selector at constant offsets -- `*MK_FP(xVar2,(int2 *)0x3e)`,
    `*MK_FP(*(xunknown2 *)(a1 + 0x18),(int2 *)0x40)`: a structure that segment holds (a task's
    or a module's database) -- read as one: `SEG_PTR(seg_f_xVar2, xVar2)->f_3e` (SEG_PTR: the segment's start as a far pointer), its fields at
    exactly those offsets with exactly those types (the same place, the same width: the same
    program). Per function, for a selector that is a variable or a plain read (no call in it),
    used at two offsets or more, each always with one type and none overlapping. The structures
    are appended to `decls`, packed. A global selector's (`xRam10000002`, the same segment in
    every function) is the module's, `seg_xRam10000002`: its fields go to `shared` {name:
    {function: fields}}, declared once by _shared_selector_structs. -> the text"""
    if "MK_FP(" not in text:
        return text
    src = ctree.Source(text)
    groups = {}                                         # selector text -> [(node, offset, type)]
    for n in src.find("call_expression"):
        if src.at(n.child_by_field_name("function")) != "MK_FP":
            continue
        args = n.child_by_field_name("arguments").named_children
        if len(args) != 2 or args[1].type != "cast_expression":
            continue
        ty = src.at(args[1].child_by_field_name("type")).replace(" ", "")
        val = args[1].child_by_field_name("value")
        if not ty.endswith("*") or ty.count("*") != 1 or val.type != "number_literal":
            continue
        try:
            off = int(src.at(val), 0)
        except ValueError:
            continue
        sel = " ".join(src.at(args[0]).split())
        if args[0].type not in ("identifier", "pointer_expression", "field_expression"):
            continue
        if any(c.type == "call_expression" for c in src.find("call_expression", under=args[0])):
            continue
        if any(src.at(up.child_by_field_name("function")) == "MK_FP" for up in src.ancestors(n, "call_expression")):
            continue                                     # (one inside another's arguments: a far call
                                                         # through a far pointer -- left as it is)
        groups.setdefault(sel, []).append((n, off, ty[:-1]))
    made = 0
    for sel, uses in groups.items():
        fields = {}
        for _n, off, ty in uses:
            if fields.get(off, ty) != ty:
                fields = None
                break
            fields[off] = ty
        if not fields or len(fields) < 2 or any(_SIZEOF.get(t) is None for t in fields.values()):
            continue
        body = _seg_layout(fields)
        if body is None:
            continue                                     # (overlapping: not one layout)
        made += 1
        if shared is not None and _RAM.fullmatch(sel):
            name = "seg_" + sel                          # (the module's: see _shared_selector_structs)
            shared.setdefault(name, {})[fname] = fields
        else:
            tag = sel if re.fullmatch(r"[A-Za-z_]\w*", sel) else "sel%d" % made
            name = "seg_%s_%s" % (re.sub(r"\W", "_", fname), tag)   # (not s_..: the core's own structures)
            decls.append("typedef _Packed struct %s { %s } %s;" % (name, " ".join(body), name))
        for n, off, _ty in uses:
            ptr = "SEG_PTR(%s, %s)" % (name, src.at(n.child_by_field_name("arguments").named_children[0]))
            up = n.parent
            if up is not None and up.type == "pointer_expression" and src.at(up.child_by_field_name("operator")) == "*":
                src.replace(up, "%s->f_%x" % (ptr, off))
            else:
                src.replace(n, "(&%s->f_%x)" % (ptr, off))     # (parenthesized: `MK_FP(...)[1]` indexes the address)
    return src.text() if made else text


def _far_addresses(text, pointers=None):
    """An address made of a segment the decompiler could not fold to a constant and an offset
    (`MK_FP(seg, (T *)off)`, its printing) as a far pointer to T: `(T __far *)MK_FP(seg, off)`.
    An offset with no cast is a pointer's own arithmetic (`MK_FP(seg, p + 2)`, p an `int2 *`:
    the word two words on) -- the place is a T as the pointer is; only an offset that is no
    pointer at all is taken a byte at a time."""
    if "MK_FP(" not in text and "+ 2))" not in text and ">> 0x10" not in text:
        return text
    src = ctree.Source(text)
    if pointers is None:                                    # (the function's own: `T *name;`)
        pointers = {}
        for n in src.find("declaration"):
            if not src.line(n)[:1].isspace():
                continue                                    # (a parameter's, in the older style: not the function's own)
            ds = n.children_by_field_name("declarator")
            ty = src.at(n.child_by_field_name("type"))
            if len(ds) == 1 and ds[0].type == "pointer_declarator" and ds[0].child_by_field_name("declarator").type == "identifier" \
                    and re.fullmatch(r"[A-Za-z_]\w*(?: [A-Za-z_]\w*)?", ty) and src.at(ds[0]).startswith("*"):
                pointers[src.at(ds[0].child_by_field_name("declarator"))] = ty

    def leftmost(n):
        """what an offset counts from: itself, or the first term of a sum or difference"""
        while True:
            while n.type == "parenthesized_expression" and n.named_child_count == 1:
                n = n.named_children[0]
            if n.type == "binary_expression" and n.child_by_field_name("operator").type in ("+", "-"):
                n = n.child_by_field_name("left")
            else:
                return n

    def pointer_cast(n):
        """T of a cast to `T *`, else None"""
        if n.type != "cast_expression":
            return None
        ty = src.at(n.child_by_field_name("type")).strip()
        return ty[:-1].strip() if ty.endswith("*") and ty.count("*") == 1 else None

    # A far pointer's own halves put back together -- `MK_FP((int2)((uint4)p >> 0x10), (T *)p + 2)`,
    # or the high half kept in a variable first -- is the pointer's own arithmetic: `(T __far *)p + 2`.
    # Written as halves, no compiler gives back the original's `les bx, p` / `es:[bx+2]`.
    far_names = set(re.findall(r"__far\s*\*\s*(\w+)\s*[;,)]", text))
    # (the high half: `(T)((uint4)p >> 0x10)`, the core's `p._2_2_`, or that as the word at &p + 2)
    high = (r"(?:\(\s*\w+\s*\)\s*\(\s*\(\s*uint4\s*\)\s*(\w+)\s*>>\s*0x10\s*\)|(\w+)\._2_2_|"
            r"\(?\s*\*\s*\(\s*u?int2\s*\*\s*\)\s*\(\s*\(\s*char\s*\*\s*\)\s*&\s*(\w+)\s*\+\s*2\s*\)\s*\)?)")
    high_vars = {v: p or q or r for v, p, q, r in re.findall(r"(?m)^\s*(\w+)\s*=\s*" + high + r"\s*;", text)
                 if (p or q or r) in far_names}

    def own_halves(seg, off):
        """the far pointer whose two halves seg and off are, else None"""
        s = src.at(seg).strip()
        m = re.fullmatch(high, s)
        p = (m.group(1) or m.group(2) or m.group(3)) if m else high_vars.get(s)
        if p not in far_names:
            return None
        base = off
        for _ in range(12):                          # (through casts, brackets, a sum's first term)
            if base.type == "parenthesized_expression" and base.named_child_count == 1:
                base = base.named_children[0]
            elif base.type == "cast_expression":
                base = base.child_by_field_name("value")
            elif base.type == "binary_expression" and base.child_by_field_name("operator").type in ("+", "-"):
                base = base.child_by_field_name("left")
            else:
                break
        return p if base is not None and base.type == "identifier" and src.at(base) == p else None

    def far(n):
        """MK_FP(seg, off) as a far pointer to what is there; None for any other node"""
        if n.type != "call_expression" or src.at(n.child_by_field_name("function")) != "MK_FP":
            return None
        args = n.child_by_field_name("arguments").named_children
        if len(args) != 2:
            return None
        seg, off = args
        p = own_halves(seg, off)
        if p is not None:
            target = pointer_cast(off) or pointer_cast(leftmost(off)) or "uint1"
            sum_ = src.render(off.child_by_field_name("value") if pointer_cast(off) else off, far)
            # (the low half counted in bytes -- `(int2)(void *)p + 2` -- is the pointer moved on 2 bytes)
            sum_ = re.sub(r"\(\s*u?int2\s*\)\s*(?:\(\s*void\s*\*\s*\)\s*)?%s\b" % re.escape(p), "(uint1 __far *)%s" % p, sum_)
            sum_ = re.sub(r"\(\s*((?:(?!__far)[\w ])+?)\s*\*\s*\)\s*%s\b" % re.escape(p), r"(\1 __far *)%s" % p, sum_)
            if not re.search(r"__far \*\)\s*%s\b" % re.escape(p), sum_):
                sum_ = re.sub(r"\b%s\b" % re.escape(p), "(uint1 __far *)%s" % p, sum_, count=1)
            return "((%s __far *)(%s))" % ("uint1" if target in ("void", "code") else target, sum_)
        first = leftmost(off)
        if pointer_cast(off):                              # (T *)x: a T there, at x
            target, offset = pointer_cast(off), off.child_by_field_name("value")
        elif pointer_cast(first):                          # (T *)x + n: a T there, the sum as written
            target, offset = pointer_cast(first), off
        else:                                              # p + n, p a T *: as the pointer is
            target, offset = pointers.get(src.at(first) if first.type == "identifier" else None, "uint1"), off
        if target in ("void", "code"):
            target = "uint1"
        up = n.parent
        if up is not None and up.type == "pointer_expression" and up.parent is not None and \
                up.parent.type == "parenthesized_expression" and up.parent.parent is not None and \
                up.parent.parent.type == "call_expression" and up.parent.parent.child_by_field_name("function") == up.parent:
            target = "code"                                # (*MK_FP(seg, off))(...): a call to code there
        return "((%s __far *)MK_FP(%s, %s))" % (target, src.render(seg, far), src.render(offset, far))
    for n in src.find("call_expression"):
        if src.at(n.child_by_field_name("function")) == "MK_FP" and \
                not any(src.at(up.child_by_field_name("function")) == "MK_FP" for up in src.ancestors(n, "call_expression")):
            new = far(n)
            if new is not None:
                src.replace(n, new)
    out = _whole_far_arguments(src.text(), far_names, high, high_vars)
    for v in high_vars:                              # (a far pointer's high half no longer used: gone)
        if len(re.findall(r"\b%s\b" % re.escape(v), out)) == 2:
            out = re.sub(r"(?m)^\s*%s\s*=\s*%s\s*;\n" % (re.escape(v), high), "", out)
            out = re.sub(r"(?m)^\s*\w+\s+%s\s*;\n" % re.escape(v), "", out)
    return out


def _far_casts(text):
    """A far pointer read as another type stays far: `*(int2 *)p`, p a `void __far *`, is
    `*(int2 __far *)p` -- the near cast keeps only its offset, and reads it in DS.
    And a far pointer's own halves put back as it: `CONCAT22(<p's high word>, p)` is `(int4)p`;
    `<p's low word> = <p's offset> + k` -- the pointer moved on in its segment, as a far
    pointer's arithmetic is -- is `p = (uint1 __far *)p + k`."""
    for p in set(re.findall(r"__far\s*\*\s*(\w+)\s*[;,)]", text)):
        text = re.sub(r"\(\s*((?:(?!__far)[\w ])+?)\s*\*\s*\)\s*(?=%s\b)" % re.escape(p), r"(\1 __far *)", text)
        q = re.escape(p)
        hi = (r"(?:\(\s*\*\s*\(\s*u?int2\s*(?:__far\s*)?\*\s*\)\s*\(\s*\(\s*char\s*(?:__far\s*)?\*\s*\)\s*&\s*%s\s*\+\s*2\s*\)\s*\)"
              r"|\(\s*u?int2\s*\)\s*\(\s*\(\s*uint4\s*\)\s*%s\s*>>\s*0x10\s*\)|%s\._2_2_)" % (q, q, q))
        lo = r"(?:\(\s*void\s*__far\s*\*\s*\)\s*)?%s" % q
        text = re.sub(r"CONCAT22\(\s*%s\s*,\s*%s\s*\)" % (hi, lo), "(int4)%s" % p, text)
        low_word = r"\(\s*\*\s*\(\s*u?int2\s*(?:__far\s*)?\*\s*\)\s*\(\s*\(\s*char\s*(?:__far\s*)?\*\s*\)\s*&\s*%s\s*\+\s*0\s*\)\s*\)" % q
        offset = r"\(\s*void\s*(?:__far\s*)?\*\s*\)\s*\(\s*\(\s*u?int2\s*\)\s*(?:\(\s*void\s*__far\s*\*\s*\)\s*)?%s\s*([+-])\s*(0x[0-9a-fA-F]+|\d+)\s*\)" % q
        text = re.sub(r"%s\s*=\s*%s\s*;" % (low_word, offset), r"%s = (uint1 __far *)%s \1 \2;" % (p, p), text)
    return text


def _whole_far_arguments(text, far_names, high, high_vars):
    """A far pointer passed as its two halves, low then high -- `f((int2)(void *)p, p._2_2_)` -- is
    the pointer passed: the same two words, in the same places, whatever the convention (the list
    is in the order the words go on the stack), and what the original pushes (`push es` / `push bx`
    from one `les`). As halves, the callee cannot be declared with the pointer it takes."""
    low = r"\(\s*u?int2\s*\)\s*(?:\(\s*void\s*\*\s*\)\s*)?(\w+)"
    out, i = [], 0
    for mm in re.finditer(r"\b\w+\s*\(", text):
        if mm.start() < i:
            continue
        args, end = _arguments_at(text, mm.end() - 1)
        if len(args) < 2:
            continue
        new, k, changed = [], 0, False
        while k < len(args):
            lo = re.fullmatch(r"\s*" + low + r"\s*", args[k]) if k + 1 < len(args) else None
            hi = re.fullmatch(r"\s*" + high + r"\s*", args[k + 1]) if lo else None
            hp = (hi.group(1) or hi.group(2) or hi.group(3)) if hi else (high_vars.get(args[k + 1].strip()) if lo else None)
            if lo and lo.group(1) in far_names and hp == lo.group(1):
                new.append(" " + lo.group(1) if k else lo.group(1))
                k += 2
                changed = True
            else:
                new.append(args[k])
                k += 1
        if changed:
            out.append(text[i:mm.end()] + ",".join(new) + ")")
            i = end
    out.append(text[i:])
    return "".join(out)


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


class ModulePowerPC(object):
    pass


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
    out, order, by_va, _err = decompiler.raw(module_path, describe.function_starts(module_path), outdir)
    sizes, novalue = {}, set()
    for n in order:
        body = []
        for ln in out[n].split("\n"):
            m = re.match(r"/\*@G ([0-9a-f]+) (\d+)(?: \d+)? \*/$", ln)
            if m:
                sizes.setdefault(int(m.group(1), 16), set()).add(int(m.group(2)))
            elif not ln.startswith("/*@P "):
                body.append(ln)
        t = "\n".join(body).strip()
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
    # The structures recovered for each function (`/*@S` notes), declared before the prototypes:
    # a function returning a pointer to one names it in its prototype, which every body includes.
    types = _TYPES.replace("typedef int code();", "typedef uint8 code();") + _HELPERS + _POWERPC
    notes = [ln for t in out.values() for ln in t.split("\n") if ln.startswith("/*@S ")]
    structs = _struct_decls(notes, types)
    m = ModulePowerPC()
    m.header = types + "".join(x + "\n" for x in structs) + "\n".join(decls) + "\n" + "\n".join(macros) + "\n"
    m.texts, m.order, m.by_va, m.novalue = out, order, by_va, novalue
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
