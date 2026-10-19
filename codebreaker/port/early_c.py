"""early_c.py -- C of before the standard made C a compiler of today takes, the meaning kept.

The C that CodeBreaker recovers from a program of the 1980s is what its authors wrote: function definitions in the
old style (`f(a, b) char *a; {`), no prototypes, an int that is 16 bits, a long that is 32, structures whose bytes
are laid out in memory pages and files by literal sizes, a member name used on whatever pointer is at hand, the C
library of the program's own (its stdio, printf, malloc) among the sources. Each of these is mended here, the same
way for every program:

  definitions    the old-style definition becomes a prototyped one; a function that gives no value is void
  prototypes     one header (proto.h) declares every function of the program; the old `extern f();` lines go
  widths         long is int32_t everywhere (what it was); in a structure, and in a pointer cast that reads
                 the bytes of a buffer, int and unsigned are int16_t and uint16_t - the layout the bytes have
  formats        a %ld / %lx of a long, now an int32_t, is %d / %x
  shared data    a variable a header defines (every file had its own, the linker made them one) is declared
                 extern there and defined once, in data.c
  runtime        the program's own C library (the modules that define fopen, malloc, _main...) and its headers
                 are left out: the C library of today stands in; what was no standard call (DOS's handles, the
                 keyboard line) the platform layer gives (platform/dosrt.h)

and then the compiler's own complaints, in a loop: a member used on a pointer to something else is reached
through a cast to the structure that has it; a function whose value is used after all gives an int again; a
function called with more or fewer arguments than it has keeps the old-style definition (its calls unchecked,
as they were). Every change is noted for the report, with the line.
"""
import collections
import os
import functools
import re
import subprocess

KEYWORDS = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern",
            "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static",
            "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"}
STDINT = frozenset("int8_t uint8_t int16_t uint16_t int32_t uint32_t intptr_t uintptr_t size_t".split())
TYPEWORDS = {"int", "char", "long", "short", "unsigned", "signed", "float", "double", "void", "struct", "union",
             "enum", "register", "static", "extern", "auto", "const", "volatile", "FILE", "int16_t", "uint16_t",
             "int32_t", "uint32_t", "int8_t", "uint8_t", "intptr_t", "uintptr_t"}
PRINTF = {"printf", "fprintf", "sprintf"}


# ---------------------------------------------------------------- reading the text

_BLANK_RX = re.compile(r"/\*[\s\S]*?(?:\*/|\Z)|\"(?:\\[\s\S]|[^\"\\\n])*\"?|'(?:\\[\s\S]|[^'\\\n])*'?")


def _blank_one(m):
    t = m.group(0)
    if t.startswith("/*"):                                  # (a comment: its newlines kept)
        return re.sub(r"[^\n]", " ", t)
    q = t[0]
    end = len(t) - 1 if len(t) > 1 and t[-1] == q and not _escaped_end(t) else len(t)
    return q + " " * (end - 1) + t[end:]                    # (a string: inside it spaces, its quotes kept)


def _escaped_end(t):
    """whether the last quote of a string's match is inside it (an escape: \\" ) - never, by the pattern"""
    return False


@functools.lru_cache(maxsize=16384)
def blank(text):
    """the text with what is inside its comments, strings and character constants made spaces (newlines and
    the quotes kept, every offset the same): what is code (a text seen before: as it was - the passes ask often)"""
    return _BLANK_RX.sub(_blank_one, text)


def blank_slow(text):
    """blank, a character at a time (what blank must give: the test of it)"""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            for k in range(i, j):
                if out[k] != "\n":
                    out[k] = " "
            i = j
        elif text[i] in "\"'":
            q, j = text[i], i + 1
            while j < n and text[j] != q and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            for k in range(i + 1, min(j, n)):
                out[k] = " "
            i = j + 1
        else:
            i += 1
    return "".join(out)


def sub_code(text, rx, fn):
    """re.sub of what is code alone (not in strings or comments)"""
    code = blank(text)
    out, last = [], 0
    for m in rx.finditer(code):
        out += [text[last:m.start()], fn(m)]
        last = m.end()
    return "".join(out) + text[last:]


def depths(code_lines):
    """the brace depth at the start of each line"""
    d, out = 0, []
    for l in code_lines:
        out.append(d)
        d += l.count("{") - l.count("}")
    return out


def split_top(s, sep=","):
    parts, d, cur = [], 0, ""
    for c in s:
        if c in "([":
            d += 1
        elif c in ")]":
            d -= 1
        if c == sep and d == 0:
            parts.append(cur)
            cur = ""
        else:
            cur += c
    parts.append(cur)
    return parts


def declarators(decl, types=TYPEWORDS):
    """'register struct s *a, b[3]' -> (base type, [(name, its declarator)]) - or None"""
    parts = split_top(decl.strip().rstrip(";"))
    first = parts[0]
    toks = list(re.finditer(r"[A-Za-z_]\w*", first))
    name = None
    for i, m in enumerate(toks):
        if m.group() in types or (i and toks[i - 1].group() in ("struct", "union", "enum")):
            continue
        name = m
        break
    if name is None:
        return None
    start = name.start()
    while start and first[start - 1] in " \t*(":
        start -= 1
    while start < name.start() and first[start] in " \t":
        start += 1
    base = first[:start].strip()
    out = [(name.group(), first[start:].strip())]
    for p in parts[1:]:
        m = re.search(r"[A-Za-z_]\w*", p)
        if not m:
            return None
        out.append((m.group(), p.strip()))
    return base, out


# ---------------------------------------------------------------- one file

def array_param(t, n):
    """a parameter declared an array - T a[] or T a[][3]: the pointer it is (T *a, T (*a)[3]), which a prototype
    can name before T is complete"""
    m = re.fullmatch(r"(.*?)\b%s\s*\[[^\]]*\]((?:\s*\[[^\]]*\])*)\s*" % re.escape(n), t)
    if not m:
        return t
    rest = m.group(2).strip()
    return "%s(*%s)%s" % (m.group(1), n, rest) if rest else "%s*%s" % (m.group(1), n)


class Fn:
    def __init__(self, name, ret, params, static, line):
        self.name, self.ret, self.params, self.static, self.line = name, ret, params, static, line
        self.void = False
        self.loose = False                                  # (calls it does not match: old-style kept)
        self.bare = 0                                       # (bare returns given a value)

    def proto(self, names=True):
        ps = ", ".join(array_param(t, n) for n, t in self.params) or "void"   # (T a[][3]: the pointer it is)
        ret = "void" if self.void else (self.ret or "int")
        sep = "" if ret.endswith("*") else " "
        if getattr(self, "varargs", None):
            return "%s%s%s%s(%s, ...)" % ("static " if self.static else "", ret, sep, self.name, ps)
        if self.loose:
            return "%s%s%s%s()" % ("static " if self.static else "", ret, sep, self.name)
        return "%s%s%s%s(%s)" % ("static " if self.static else "", ret, sep, self.name, ps)


HEAD = re.compile(r"^(?P<ret>[\w\s*]*?)(?P<name>[A-Za-z_]\w*)\s*\((?P<params>[^()]*)\)\s*$")


def definitions(lines, code):
    """the old-style function definitions of a file: (Fn, first line, the line of its '{', its last line)"""
    out = []
    ds = depths(code)
    i = 0
    while i < len(lines):
        m = HEAD.match(code[i].rstrip()) if ds[i] == 0 and not code[i].lstrip().startswith("#") else None
        if not m or m.group("name") in KEYWORDS or not all(w in TYPEWORDS or w == "*" for w in
                                                            re.findall(r"\w+|\*", m.group("ret"))
                                                            if w not in ("struct", "union")) \
           and not re.fullmatch(r"\s*(?:static\s+)?(?:struct|union)\s+\w+\s*\**\s*", m.group("ret")):
            i += 1
            continue
        params = [p.strip() for p in m.group("params").split(",") if p.strip()]
        if not all(re.fullmatch(r"[A-Za-z_]\w*", p) for p in params) or params == ["void"]:
            i += 1
            continue
        first = i
        if not m.group("ret").strip() and i and ds[i - 1] == 0 and \
           re.fullmatch(r"\s*(?:static\s+)?(?:(?:unsigned|signed|long|short|int|char|void|float|double|struct\s+\w+|"
                        r"union\s+\w+)\s*)+\**\s*|\s*(?:static\s+)?[A-Z_][A-Z0-9_]*\s*\*+\s*", code[i - 1]) and \
                code[i - 1].strip():                        # (or a type of the library's: FILE *)
            first = i - 1                                   # (the type on the line before: char *\nf(a))
        j, decls = i + 1, []
        while j < len(lines) and not code[j].strip().startswith("{"):
            s = code[j].strip()
            if s and not s.endswith(";"):
                break
            if s:
                decls.append(s)
            j += 1
        if j >= len(lines) or not code[j].strip().startswith("{"):
            i += 1
            continue
        types = {}
        for d in decls:
            r = declarators(d)
            if r is None:
                continue
            base, ds2 = r
            if not re.sub(r"\b(?:register|auto|static|extern|const|volatile)\b", "", base).strip():
                base = (base + " int").strip()              # (register op, asg; - old C's int, unsaid)
            for n, dcl in ds2:
                types[n] = (base + " " + dcl).strip()
        ret = m.group("ret").strip() if first == i else code[first].strip()
        static = bool(re.match(r"static\b", ret))
        ret = re.sub(r"^static\s*", "", ret).strip()
        fn = Fn(m.group("name"), ret, [(p, types.get(p, "int " + p)) for p in params], static, i + 1)
        fn.implicit = {p for p in params if p not in types}   # (parameters it never declared: ints by default)
        k, d = j, 0                                         # (the body: to its closing brace)
        while k < len(lines):
            d += code[k].count("{") - code[k].count("}")
            if d == 0:
                break
            k += 1
        out.append((fn, first, j, k))
        i = k + 1
    return out


def widths(text, code, notes, fname, int16=False):
    """long as int32_t; int and unsigned in a structure, and in a pointer cast, as int16_t and uint16_t; with
    int16, every int and unsigned (the arithmetic that rotates a word, or counts on its wrapping round, as it was)"""
    edits = []                                              # (start, end, new)

    def sub(rx, new, lo=0, hi=None, why=None):
        for m in re.finditer(rx, code[lo:hi]):
            a, b = lo + m.start(), lo + m.end()
            if not any(a < e and s < b for s, e, x in edits):
                edits.append((a, b, new))
                if why:
                    notes[why].append("%s:%d" % (fname, code.count("\n", 0, a) + 1))
    for m in re.finditer(r"\b(?:struct|union)\s*\w*\s*\{", code):   # (structures: the bytes' layout)
        d, k = 0, m.end() - 1
        while k < len(code):
            d += {"{": 1, "}": -1}.get(code[k], 0)
            if d == 0:
                break
            k += 1
        sub(r"\bunsigned\s+short(?:\s+int)?\b|\bunsigned(?!\s+(?:char|long|short))(?:\s+int)?\b", "uint16_t",
            m.end(), k, "struct member unsigned -> uint16_t (the structure's bytes as they were)")
        sub(r"(?<!unsigned )(?<!signed )\b(?:short\s+int|short|int)\b(?!\w)", "int16_t", m.end(), k,
            "struct member int -> int16_t (the structure's bytes as they were)")
    sub(r"\(\s*unsigned(?:\s+int)?\s*(?=\*+\s*\))", "(uint16_t ", why="pointer cast (unsigned *) -> (uint16_t *): a word of a buffer")
    sub(r"\(\s*int\s*(?=\*+\s*\))", "(int16_t ", why="pointer cast (int *) -> (int16_t *): a word of a buffer")
    sub(r"\bunsigned\s+long(?:\s+int)?\b|\blong\s+unsigned(?:\s+int)?\b", "uint32_t", why="unsigned long -> uint32_t")
    sub(r"\blong(?:\s+int)?\b", "int32_t", why="long -> int32_t (32 bits, as it was)")
    sub(r"\bunsigned\s+short(?:\s+int)?\b", "uint16_t", why="short -> int16_t")
    sub(r"\bshort(?:\s+int)?\b", "int16_t", why="short -> int16_t")
    if int16:                                               # (every int as wide as it was: a store truncates as
        for m in re.finditer(r"(?m)^.*\bmain\s*\(.*$", code):   #  the 8086's did; main's own are the system's)
            edits.append((m.start(), m.start(), None))
            edits.append((m.start(), m.end(), None))
        sub(r"\bunsigned(?!\s+(?:char|long|short))(?:\s+int)?\b", "uint16_t",
            why="unsigned -> uint16_t (16 bits, as it was)")
        sub(r"(?<!unsigned )(?<!signed )\bint\b", "int16_t", why="int -> int16_t (16 bits, as it was)")
        edits[:] = [e for e in edits if e[2] is not None]
    for a, b, new in sorted(edits, reverse=True):
        text = text[:a] + new + text[b:]
        code = code[:a] + new + code[b:]
    return text, code


def formats(text, code, notes, fname, int16=False, hex_upper=False):
    """the argument of a %ld %lx %lu in printf's family (an int32_t now, or a long of the C library's, as
    ftell's) given as (long): what the format says, on every system"""
    edits = []
    for m in re.finditer(r"\b(?:%s)\s*\(" % "|".join(PRINTF), code):
        i, d, spans, start = m.end(), 1, [], m.end()     # (the call's arguments)
        while i < len(code) and d:
            c = code[i]
            d += (c in "([") - (c in ")]")
            if (c == "," and d == 1) or d == 0:
                spans.append((start, i))
                start = i + 1
            i += 1
        fmt = next((k for k, (a, b) in enumerate(spans) if code[a:b].strip().startswith('"')), None)
        if fmt is None:
            continue
        a, b = spans[fmt]
        lit = re.sub(r"(%[-+ #0]*(?:\d+|\*)?(?:\.\d+)?)[FN](?=l?[spndiouxXc])", r"\1", text[a:b])
        if lit != text[a:b]:                                # (%Fs, %Np: Microsoft's far and near - one memory now)
            notes["%Fs / %Np -> %s / %p (Microsoft's printf: a far or near pointer; one flat memory now)"].append(
                "%s:%d" % (fname, code.count("\n", 0, a) + 1))
        if hex_upper:                                       # (the old printf's %x was in capitals)
            up = re.sub(r"(%[-+ #0]*(?:\d+|\*)?(?:\.\d+)?l?)x", r"\1X", lit)
            if up != lit:
                notes["%x -> %X (the old printf gave its hexadecimal in capitals)"].append(
                    "%s:%d" % (fname, code.count("\n", 0, a) + 1))
            lit = up
        if lit != text[a:b]:
            edits.append((a, b, lit))
        convs = re.findall(r"%[-+ #0]*(?:\d+|\*)?(?:\.\d+)?(l?)([diouxXcsp%])", lit)
        k = fmt
        for lng, conv in convs:
            if conv == "%":
                continue
            k += 1
            if k >= len(spans):
                break
            x, y = spans[k]
            arg = text[x:y].strip()
            lead = text[x:y][:len(text[x:y]) - len(text[x:y].lstrip())]
            simple = arg if re.fullmatch(r"[\w.\->\[\]]+|\w+\([^()]*\)", arg) else "(%s)" % arg
            if lng and not arg.startswith("(long)"):
                edits.append((x, y, lead + "(long)" + simple))
                notes["argument of a %l. format given as (long) (a long is int32_t here)"].append(
                    "%s:%d" % (fname, code.count("\n", 0, x) + 1))
            elif int16 and not lng and conv in "ouxX" and not arg.startswith("(uint16_t)"):
                edits.append((x, y, lead + "(uint16_t)" + simple))  # (16 bits shown, as its printf showed them)
                notes["argument of a %x / %u format given as (uint16_t) (16 bits, as the old printf showed)"].append(
                    "%s:%d" % (fname, code.count("\n", 0, x) + 1))
    for x, y, new in sorted(edits, reverse=True):
        text = text[:x] + new + text[y:]
    return text


POOLS = {}                                              # ({allocator: {(struct, old size)}}: of struct_sizes)
ARGCAST = r"(?:\(\s*(?:(?:struct|union)\s+\w+|\w+)(?:\s+\w+)*\s*\**\s*\)\s*)?"   # (a cast of type words alone)
ALLOC = re.compile(r"\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*(\w*alloc\w*)\s*\(")
CASTCALL = re.compile(r"\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*([A-Za-z_]\w*)\s*\(")


MEMFNS = ("memset", "memcpy", "memmove", "memcmp", "movbytes", "fillb", "far_memcpy", "_fmemcpy", "_fmemset")


GLOBAL_DECL = r"(?m)((?:^|;)\s*(?:extern\s+|static\s+|register\s+)?)(u?int16_t|int|unsigned)\s+((?:\w+\s*,\s*)*)%s\s*((?:,\s*\w+\s*)*);"
LOCAL_DECL = r"(?m)^(\s*(?:register\s+)?)(u?int16_t|int|unsigned)\s+((?:\w+\s*,\s*)*)%s\s*((?:,\s*\w+\s*)*);"


def used_as_number(code, var):
    """whether a variable is computed with as a number somewhere (n /= 2, n * v, n << 1, n & 7, n % k)"""
    v = re.escape(var)
    return bool(re.search(r"\b%s\s*(?:[*/%%^|]=|<<=|>>=|[/%%]|<<|>>|\*(?!\s*\))|&(?!&)|\|(?!\|))" % v, code) or
                re.search(r"(?:[\w)\]]\s*(?:[*/%%]|<<|>>)|[*/%%]=|<<=|>>=)\s*%s\b" % v, code) or
                re.search(r"\(?\s*\b%s\s*(?:[+\-]=?[^;)]*)?\)?\s*(?:[<>]=?)\s*-?\s*(?:[1-9]|0\b)" % v, code) or
                re.search(r"\b%s\s*[+\-]\s*\w+\s*\)\s*[&|]" % v, code) or   # (compared with a number; (off + n) & ~n)
                re.search(r"\[\s*(?:\+\+|--)?\s*%s\s*(?:\+\+|--)?\s*\]" % v, code))   # (an index)


@functools.lru_cache(maxsize=4096)
def function_map(text):
    """the functions of a text, new style and then old: ((name, first line, '{' line, last line), ...) - a text
    seen before: as it was (one parse a text, not one a complaint)"""
    code = blank(text)
    lines, clines = text.split("\n"), code.split("\n")
    return tuple(tuple(d) for d in definitions_any(lines, clines)) + \
        tuple((f.name, s, j, k) for f, s, j, k in definitions(lines, clines))


def enclosing(text, ln):
    """(name, first line, '{' line, last line) of the function line ln (1-based) is in - None"""
    for d in function_map(text):                            # (new style first; an old-style one, kept so)
        if d[1] <= ln - 1 <= d[3]:
            return d
    return None


def declares_local(text, fn, var):
    """whether a function (definitions_any's) declares var itself: a parameter or a local"""
    lines = blank(text).split("\n")[fn[1]:fn[3] + 1]
    return bool(re.search(r"\b(?:u?int16_t|int|unsigned|intptr_t)\s+(?:[^;(){}]*,\s*)?%s\b\s*[;,)=]" % re.escape(var),
                          "\n".join(lines)))


def apply_local_retypes(text, fname, retypes):
    """the variables of one function that hold pointers, declared pointers there alone"""
    mine = [(fn, var, t) for (f, fn, var), t in retypes.items() if f == fname]
    if not mine:
        return text
    lines = text.split("\n")
    code = blank(text).split("\n")
    defs = definitions_any(lines, code) + [(f.name, s, j, k) for f, s, j, k in definitions(lines, code)]
    for d in sorted(defs, key=lambda d: -d[1]):          # (new style and old)
        for fn, var, t in mine:
            if fn != d[0]:
                continue
            body = "\n".join(lines[d[1]:d[3] + 1])
            v = re.escape(var)
            body = re.sub(r"(?m)^(\s*(?:register\s+)?)(?:u?int16_t|int|unsigned)\s+%s\s*;" % v,
                          r"\g<1>%s%s%s;" % (t, "" if t.endswith("*") else " ", var), body)
            body = re.sub(LOCAL_DECL % v, split_decl(var, t), body)
            body = re.sub(r"\((?:u?int16_t|int|unsigned)\s+%s\s*([,)])" % v,       # (a parameter of the prototype)
                          lambda m: "(%s%s%s%s" % (t, "" if t.endswith("*") else " ", var, m.group(1)), body)
            body = re.sub(r",\s*(?:u?int16_t|int|unsigned)\s+%s\s*([,)])" % v,
                          lambda m: ", %s%s%s%s" % (t, "" if t.endswith("*") else " ", var, m.group(1)), body)
            lines[d[1]:d[3] + 1] = body.split("\n")
    return "\n".join(lines)


class split_decl:
    """int16_t a, tag, b;  ->  int16_t a, b; struct tag *tag;  (a re.sub replacement: one name of a list retyped)"""
    def __init__(self, name, typ):
        self.name, self.typ = name, typ

    def __eq__(self, other):
        return isinstance(other, split_decl) and (self.name, self.typ) == (other.name, other.typ)

    def __call__(self, m):
        rest = [x.strip() for x in (m.group(3) + m.group(4)).replace(",", " ").split() if x.strip()]
        lead = m.group(1)
        out = ("%s%s %s; " % (lead, m.group(2), ", ".join(rest))) if rest else lead
        cls = " ".join(w for w in lead.split() if w in ("extern", "static")) if rest else ""   # (its storage: so)
        return out + "%s%s%s%s;" % (cls + " " if cls else "", self.typ, "" if self.typ.endswith("*") else " ",
                                   self.name)


def mem_sizes(text, olds, elems, notes, fname):
    """memset(g_seg_assume, 0, 24): the bytes of a global array given as the number it had - sizeof it now"""
    code = blank(text)
    edits = []
    for m in re.finditer(r"\b(%s)\s*\(" % "|".join(MEMFNS), code):
        i, d, start, args = m.end(), 1, m.end(), []
        while i < len(code) and d:
            d += (code[i] in "([") - (code[i] in ")]")
            if (code[i] == "," and d == 1) or d == 0:
                args.append((start, i))
                start = i + 1
            i += 1
        names = [re.fullmatch(r"\s*&?\s*(\w+)\s*", code[a:b]) for a, b in args]
        names = [x.group(1) for x in names if x and x.group(1) in olds]
        for a, b in args:
            lit = re.fullmatch(r"\s*(0[xX][0-9a-fA-F]+|\d+)\s*", code[a:b])
            if not lit:
                continue
            n = int(lit.group(1), 0) if not re.fullmatch(r"0\d+", lit.group(1)) else int(lit.group(1))
            for nm in names:
                if olds[nm] == n and n > 1:
                    e = elems.get(nm)                       # (as many elements as it had, of today's size: the
                    new = "%d * sizeof(%s[0])" % (n // e, nm) if e and n % e == 0 else "sizeof(%s)" % nm
                    edits.append((a + lit.start(1), a + lit.end(1), new))   #  declaration may say no size)
                    notes["the bytes of a global array given as the number it had: sizeof it now"].append(
                        "%s:%d (%s: %d)" % (fname, code.count("\n", 0, a) + 1, nm, n))
                    break
    for a, b, new in sorted(edits, reverse=True):
        text = text[:a] + new + text[b:]
    return text


def header_offsets(text, old, notes, fname):
    """p = alloc(n + 8); p[i + 8] = c; b = (struct blk *)p - a structure with its bytes after a head (a trailing
    array): the head's old length as the number it was - offsetof(struct blk, its array) now"""
    lines, code = text.split("\n"), blank(text).split("\n")
    for name, s_, j_, k_ in definitions_any(lines, code):
        body = "\n".join(code[s_:k_ + 1])
        heads = [(m.group(1), m.group(2)) for m in
                 re.finditer(r"\(\s*(struct\s+\w+)\s*\*\s*\)\s*(\w+)\b(?!\s*(?:\(|->|\[))", body)]
        for m in re.finditer(r"\b(struct\s+\w+)\s*\*\s*(\w+)\s*[;,]", body):   # (b = p, b a structure's pointer)
            for a_ in re.finditer(r"\b%s\s*=\s*(\w+)\s*;" % re.escape(m.group(2)), body):
                heads.append((m.group(1), a_.group(1)))
        for st_, v in heads:
            st_ = re.sub(r"\s+", " ", st_)
            lay = old.get(st_)
            if not lay:
                continue
            mems = [(x[0], k) for k, x in lay.items() if k != "@size" and "[" in str(x[3])]
            if not mems:
                continue
            off, mem = max(mems)
            if off < 4 or off < max(x[0] for k, x in lay.items() if k != "@size"):
                continue                                    # (its last member, the bytes after the head)
            n_ = r"(?<![\w.])%d(?![\w.])" % off
            what = "offsetof(%s, %s)" % (st_, mem)
            pats = [r"\b%s\s*\[[^\]\n]*" % re.escape(v), r"\b%s\s*=\s*(?:\([^()]*\)\s*)?\w+\s*\([^;\n]*" % re.escape(v),
                    r"\b%s\s*\+\s*" % re.escape(v)]
            for x in range(s_, k_ + 1):
                c, l = code[x], lines[x]
                for pt in pats:
                    for mm in reversed(list(re.finditer(pt + n_, c))):
                        e = mm.end()
                        b_ = e - len(str(off))
                        if c[b_:e] == str(off):
                            l = l[:b_] + what + l[e:]
                            c = c[:b_] + what + c[e:]
                if l != lines[x]:
                    lines[x] = l
                    code[x] = c
                    notes["a structure's head given as the bytes it was (its array after it): offsetof"].append(
                        "%s:%d (%s)" % (fname, x + 1, what))
    return "\n".join(lines)


def prefix_member(lay, n):
    """the member a structure's old layout has at the offset n (not its start, not its end) - where a block of n
    bytes, a prefix of the structure, ended then; or None"""
    if not lay or n <= 0 or n >= lay.get("@size", 0):
        return None
    return next((k for k, v in lay.items() if k != "@size" and v[0] == n), None)


def struct_sizes(text, old, notes, fname, allocs=()):
    """(struct S *)malloc(n + 29): the recovered source gave a structure's size as the number it was (the bytes the
    same) - now sizeof(struct S) and the rest: a structure of today's layout is longer"""
    code = blank(text)
    ptr_of = {}                                             # (p = f04fb5(0, 10); p a struct tcell *: as if the
    for m in re.finditer(r"\b((?:struct|union)\s+\w+)\s*\*\s*(\w+)\s*[;,)=]", code):   # call were cast to it)
        ptr_of.setdefault(m.group(2), re.sub(r"\s+", " ", m.group(1)))
    ins = []
    for m in re.finditer(r"\b(\w+)\s*=\s*([A-Za-z_]\w*)\s*\(([^()]*)\)", code):
        st_ = ptr_of.get(m.group(1))
        if not st_ or st_ not in old or m.group(2) in KEYWORDS or "alloc" in m.group(2):
            continue
        lits = [int(a_.strip(), 0) for a_ in m.group(3).split(",")
                if re.fullmatch(r"\s*(?:0[xX][0-9a-fA-F]+|[1-9]\d*)\s*", a_)]
        tab = [a_ for a_ in m.group(3).split(",") if re.fullmatch(r"\s*[A-Za-z_]\w*\s*", a_) and
               re.search(r"\b%s\s*=\s*\w+\s*\[[^\]]*\]\s*\.\s*\w+\s*;" % re.escape(a_.strip()), code)]
        if old[st_]["@size"] in lits or m.group(2) in allocs and (   # (or, to an allocator, a part's size:
                len([n_ for n_ in lits if prefix_member(old[st_], n_)]) == 1 or   #  s = f(1, 10), s = f(far, sz))
                (len(tab) == 1 and not lits)):
            ins.append((m.start(2), "(%s *)" % st_))        # (its size, or a prefix's: s = f(1, 10), 10 where a
                                                            #  member of it began)
    for m in re.finditer(r"\b(\w+)\s*=\s*([A-Za-z_]\w*)\s*\(\s*([A-Za-z_]\w*)\s*\(([^()]*)\)\s*\)", code):
        st_ = ptr_of.get(m.group(1))                        # (t = f08e60(f0a42b(3, 22)); t a struct pnode *: the
        if not st_ or st_ not in old or m.group(3) in KEYWORDS or "alloc" in m.group(3):   # block made, then
            continue                                        #  filled - the inner call's block of its old size)
        if any(re.fullmatch(r"\s*(?:0[xX][0-9a-fA-F]+|[1-9]\d*)\s*", a_) and int(a_.strip(), 0) == old[st_]["@size"]
               for a_ in m.group(4).split(",")):
            ins.append((m.start(3), "(%s *)" % st_))
    ins.sort()
    for at, cast in reversed(ins):
        text = text[:at] + cast + text[at:]
    code = blank(text)
    pedits = []                                             # ((struct sym **)f(1, 22): a table of pointers by its
    for m in re.finditer(r"\(\s*((?:struct|union)\s+\w+|\w+)\s*\*\s*\*\s*\)\s*([A-Za-z_]\w*)\s*\(", code):   # bytes
        if m.group(2) in KEYWORDS or m.group(2) in MEMFNS:  #  then: two each - its pointers' now)
            continue
        i, d, start, args = m.end(), 1, m.end(), []
        while i < len(code) and d:
            d += (code[i] in "([") - (code[i] in ")]")
            if (code[i] == "," and d == 1) or d == 0:
                args.append((start, i))
                start = i + 1
            i += 1
        nums = [(a_, b_) for a_, b_ in args if re.fullmatch(r"\s*[1-9]\d*\s*", code[a_:b_])]
        if not nums:
            continue
        a_, b_ = nums[-1]
        n_ = int(code[a_:b_])
        if n_ % 2 == 0:
            pedits.append((a_, b_, " %d * sizeof(%s *)" % (n_ // 2, m.group(1))))
            notes["a table of pointers given by its bytes then (two a pointer): as many of today's"].append(
                "%s:%d (%s **: %d)" % (fname, code.count("\n", 0, a_) + 1, m.group(1), n_))
    for a_, b_, new in sorted(pedits, reverse=True):
        text = text[:a_] + new + text[b_:]
    code = blank(text)
    edits = []
    for m in CASTCALL.finditer(code):
        s = re.sub(r"\s+", " ", m.group(1))
        if s not in old:
            continue
        i, d, start = m.end(), 1, m.end()
        args = []
        while i < len(code) and d:
            d += (code[i] in "([") - (code[i] in ")]")
            if (code[i] == "," and d == 1) or d == 0:
                args.append((start, i))
                start = i + 1
            i += 1
        if not args:
            continue
        if not re.search(r"alloc", m.group(2)):             # (a routine of the program's that gives it: one of its
            whole = [(a_, b_) for a_, b_ in args             #  arguments the old size itself)
                     if re.fullmatch(r"\s*(?:0[xX][0-9a-fA-F]+|[1-9]\d*)\s*", code[a_:b_]) and
                     int(code[a_:b_].strip(), 0) == old[s]["@size"]]
            if len(whole) != 1 and m.group(2) not in allocs:
                continue                                    # (a part's size: only to an allocator's call)
            if len(whole) != 1:
                tabled = [(a_, b_) for a_, b_ in args       # (s = f(far, sz), sz = g_11a8[kind].n: a size a table
                          if re.fullmatch(r"\s*[A-Za-z_]\w*\s*", code[a_:b_]) and   #  of old byte counts gives,
                          re.search(r"\b%s\s*=\s*\w+\s*\[[^\]]*\]\s*\.\s*\w+\s*;" %   #  by kind - the structure whole
                                    re.escape(code[a_:b_].strip()), code)]   #  now: no part of it is read past
                if len(tabled) == 1 and not any(re.fullmatch(r"\s*(?:0[xX][0-9a-fA-F]+|[1-9]\d*)\s*", code[a_:b_])
                                                for a_, b_ in args):   #  the kind's own part)
                    a_, b_ = tabled[0]
                    lead = code[a_:b_][:len(code[a_:b_]) - len(code[a_:b_].lstrip())]
                    edits.append((a_, b_, "%ssizeof(%s)" % (lead, s)))
                    notes["a structure's block of a size a table of old byte counts gives (a part of it, by kind): "
                          "the whole structure now"].append("%s:%d (%s: %s)" % (
                              fname, code.count("\n", 0, a_) + 1, s, code[a_:b_].strip()))
                    continue
                a_, b_ = args[-1]                           # (its size the allocator's last argument: a block of
                lits = [x for x in re.finditer(r"(?<![\w.])(0[xX][0-9a-fA-F]+|[1-9]\d*)(?![\w.])", code[a_:b_])
                        if not re.search(r"(?:[=!<>]=|[<>])\s*$", code[a_:b_][:x.start()]) and   # (a
                        not re.match(r"\s*(?:[=!<>]=|[<>])", code[a_:b_][x.end():])]   #  compared one: no size)
                ok = lits and all(int(x.group(1), 0) == old[s]["@size"] or   #  prefix of it - as many bytes as
                                  prefix_member(old[s], int(x.group(1), 0)) for x in lits)   #  its members up to
                if ok and any(prefix_member(old[s], int(x.group(1), 0)) for x in lits):   # the one there, now;
                    for x in reversed(lits):                #  op == 0x38 ? 12 : 32 - each the place, or all)
                        n_ = int(x.group(1), 0)
                        mem = prefix_member(old[s], n_)
                        edits.append((a_ + x.start(), a_ + x.end(),
                                      "offsetof(%s, %s)" % (s, mem) if mem else "sizeof(%s)" % s))
                    cm = re.match(r"\s*\(\s*(?:unsigned\s+|signed\s+)?char\s*\)", code[a_:b_])
                    if cm:                                  # ((char)(...): a byte held the old size - not now)
                        edits.append((a_ + cm.start() + len(code[a_:b_]) - len(code[a_:b_].lstrip()), a_ + cm.end(),
                                      ""))
                    notes["a block of a structure's first members, given as the bytes they were: offsetof the "
                          "member after them"].append("%s:%d (%s: %s)" % (
                              fname, code.count("\n", 0, a_) + 1, s, code[a_:b_].strip()))
                continue
            args = whole
        a, b = args[0] if m.group(2) != "realloc" else args[-1]
        nums = [x for x in re.finditer(r"(?<![\w.])(0[xX][0-9a-fA-F]+|\d+)(?![\w.])", code[a:b])]
        if not nums:
            continue
        x = max(nums, key=lambda x: int(x.group(1), 0) if not re.fullmatch(r"0\d+", x.group(1)) else int(x.group(1)))
        n, size = int(x.group(1), 0) if not re.fullmatch(r"0\d+", x.group(1)) else int(x.group(1)), old[s]["@size"]
        if n < size // 2:
            continue                                        # (a count, not the structure)
        rest = n - size
        new = "sizeof(%s)%s" % (s, (" + %d" % rest) if rest > 0 else (" - %d" % -rest) if rest < 0 else "")
        edits.append((a + x.start(), a + x.end(), new))
        if m.group(2) not in ("malloc", "calloc", "realloc", "_fmalloc") and n == size and re.search("alloc", m.group(2)):
            POOLS.setdefault(m.group(2), set()).add((s, n))  # (the program's own allocator: given this size)
        notes["a structure's size given as the number it had (%s): sizeof and the rest" % "16-bit layout"].append(
            "%s:%d (%s: %d)" % (fname, code.count("\n", 0, a) + 1, s, n))
    for a, b, new in sorted(edits, reverse=True):
        text = text[:a] + new + text[b:]
    code = blank(text)
    pool_edits = {}
    for m in list(re.finditer(r"\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*(\w+)\s*;", code)):
        s, pool = re.sub(r"\s+", " ", m.group(1)), m.group(2)  # (a record carved from a pool: p = (struct S *)pool;
        if s not in old:                                    #  pool += size - its size the old one's)
            continue
        bump = re.search(r"\b%s\s*\+=\s*(\w+)\s*;" % re.escape(pool), code[m.end():m.end() + 400])
        if not bump:
            continue
        var = bump.group(1)
        start = code.rfind("\n{", 0, m.start())
        for a in re.finditer(r"\b%s\s*=\s*(\d+)\s*;" % re.escape(var), code[start:m.start()]):
            n = int(a.group(1))
            if 2 <= n <= old[s]["@size"]:
                x = start + a.start(1)
                pool_edits[x] = (x + len(a.group(1)), "sizeof(%s)" % s)
                notes["a record carved from a pool by the old size of its structure (part of it): the whole "
                      "structure's size now, so that none reaches into the next"].append(
                    "%s:%d (%s: %d)" % (fname, code.count("\n", 0, x) + 1, s, n))
                break
        else:                                               # (size = extra[kind] + n + 0x1c: the old common part,
            offs = {v[0] for k, v in old[s].items() if k != "@size"} | {old[s]["@size"]}   # what the kind had
            for a in re.finditer(r"\b%s\s*=\s*([^;=]+);" % re.escape(var), code[start:m.start()]):
                terms = [t.strip() for t in re.split(r"\+", a.group(1))]
                lits = [t for t in terms if re.fullmatch(r"0[xX][0-9a-fA-F]+|\d+", t)]
                if len(lits) != 1 or int(lits[0], 0) not in offs or int(lits[0], 0) < 2 or "-" in a.group(1):
                    continue
                keep = [t for t in terms if t not in lits and not re.fullmatch(r"\w+\s*\[[^\]]*\]", t)]
                x, e = start + a.start(1), start + a.end(1)
                pool_edits[x] = (e, " + ".join(["sizeof(%s)" % s] + keep))
                notes["a record carved from a pool by the old sizes of its parts (the common part, what its "
                      "kind had past it): the whole structure now, so that none reaches into the next"].append(
                    "%s:%d (%s: %s)" % (fname, code.count("\n", 0, x) + 1, s, a.group(1).strip()))
                break
    for x in sorted(pool_edits, reverse=True):
        e, new = pool_edits[x]
        text = text[:x] + new + text[e:]
    return text


def old_declarations(text, code, known, notes, fname, platform=()):
    """`extern T f(), g();` lines of functions the program defines or the C library has: gone (proto.h and the
    library's headers declare them); a variable on the same line is kept - unless the platform layer declares it
    (what the old runtime's assembler defined, as ipnCore)"""
    lines, clines = text.split("\n"), code.split("\n")
    jl, jc, i = [], [], 0                                   # (extern f(), g(),\n h(); - one declaration: one line)
    while i < len(lines):
        s = clines[i].strip()
        if s.startswith("extern") and not s.endswith(";") and "{" not in s:
            k = i + 1
            while k < len(lines) and k - i < 12 and not clines[k].strip().endswith(";") and "{" not in clines[k]:
                k += 1
            if k < len(lines) and k - i < 12 and "{" not in clines[k] and "#" not in "".join(clines[i:k + 1]):
                jl.append(" ".join([lines[i].rstrip()] + [x.strip() for x in lines[i + 1:k + 1]]))
                jc.append(" ".join([clines[i].rstrip()] + [x.strip() for x in clines[i + 1:k + 1]]))
                i = k + 1
                continue
        jl.append(lines[i])
        jc.append(clines[i])
        i += 1
    lines, clines = jl, jc
    out = []
    for l, c in zip(lines, clines):
        s = c.strip()
        plat = s.startswith("extern") and any(re.search(r"\b%s\b" % re.escape(p), s) for p in platform)
        r = declarators(s) if s.endswith(";") and ("()" in s or plat) and "=" not in s else None
        base = r[0] if r else ""
        if not r or not base or not set(re.sub(r"\b(?:struct|union)\s+\w+", "", base).split()) <= TYPEWORDS:
            out.append(l)
            continue
        base, ds = r
        keep = [d for n, d in ds if not (d.endswith("()") and n in known) and n not in platform]
        if len(keep) == len(ds):
            out.append(l)
            continue
        notes["old declaration removed (proto.h / the C library declare it)"].append(
            "%s:%d" % (fname, len(out) + 1))
        if keep:
            ind = l[:len(l) - len(l.lstrip())]
            out.append("%s%s %s;" % (ind, base, ", ".join(keep)))
    return "\n".join(out)


def header_data(text, code, fname, notes):
    """a header's variables without extern (each file that included it had them; the linker made them one):
    declared extern here, -> (text, [definitions for data.c])"""
    lines, clines = text.split("\n"), code.split("\n")
    ds = depths(clines)
    defs = []
    for i, c in enumerate(clines):
        s = c.strip()
        if ds[i] or not s.endswith(";") or s.startswith(("#", "extern", "typedef", "static")) or "(" in s and \
           not re.search(r"\(\s*\*", s) or "{" in s or "}" in s or "=" in s:
            continue
        r = declarators(s)
        if not r or not r[0]:
            continue
        defs.append(lines[i].strip())
        lines[i] = re.sub(r"^(\s*)", r"\1extern ", lines[i])
        notes["shared variable of a header: extern there, defined once in data.c"].append("%s:%d" % (fname, i + 1))
    return "\n".join(lines), defs


# ---------------------------------------------------------------- the program

class Port:
    def __init__(self, files, profile, members=None):
        """files: {name: text} of the source tree; profile: the old runtime's (runtimes/*.json); members:
        {"struct.member": type} - the port's corrections of members declared otherwise than they are used"""
        self.profile = profile
        self.notes = collections.defaultdict(list)
        files = dict(files)
        self.member_fixes = []
        self.reach = {}                                     # (tables a run read past their ends: sanitize.py)
        self.local_retypes = {}                             # ((file, function, variable): the pointer it holds)
        self.need_headers = {}                              # ({file: headers a mend made it need})
        self.extra_decls = {}                               # ({file: declarations a mend made it need})
        for key, typ in (members or {}).items():
            st, mem = key.split(".", 1)
            for n, t in files.items():
                m = re.search(r"\bstruct\s+%s\s*\{" % re.escape(st), t)
                if not m:
                    continue
                end = t.find("};", m.end())
                body = t[m.end():end]
                new = re.sub(r"(?m)^([ \t]*)[^;{}\n]*?\b%s\s*;" % re.escape(mem),
                              lambda x: "%s%s%s%s;" % (x.group(1), typ, "" if typ.endswith("*") else " ", mem), body,
                              count=1)
                if new != body:
                    files[n] = t[:m.end()] + new + t[end:]
                    self.member_fixes.append("%s: struct %s's %s is %s (as the program uses it)" % (n, st, mem, typ))
        self.left_out = {}                                  # (file: why)
        self.originals = dict(files)                        # (the source as it was: the old layouts, far ones)
        self.asm_protos = {}                                # (the program's assembly routines: their C declarations)
        self.asm_data = []                                  # ((module, definition) of a module of data alone)
        self.aliases = {}                                   # (a name that is an element of another: where, owner's)
        self.files = {}
        for n, t in files.items():
            why = self._runtime(n, t)
            if why:
                self.left_out[n] = why
            else:
                self.files[n] = t
        stand = re.compile(r"/\* SAMEFILE \*/\n([^\n{;]*\([^)]*\))\s*\{\s*\}")
        for n, t in self.files.items():                     # (empty stand-ins of functions of the original's same
            new = stand.sub(lambda m: "extern %s;" % re.sub(r"\(.*\)", "()", m.group(1)).strip(), t)
            if new != t:                                    #  file, made for the byte-exact build: declarations)
                self.files[n] = new
                self.notes["an empty stand-in of a function defined elsewhere (for the byte-exact build): "
                           "declared instead"].append(n)
        self.union_notes = collections.defaultdict(list)
        internals = set(profile.get("runtime_internals", []))
        for n, t in list(self.files.items()):               # (the old library's own structures, copied into the
            new = re.sub(r"(?m)^[ \t]*struct\s+(\w+)\s*\{[^{}]*\}\s*;[^\n]*\n?",   # program's header - FILE's:
                         lambda m: "" if m.group(1) in internals else m.group(0), t)   # today's library's)
            if new != t:
                self.files[n] = new
                self.union_notes["the old library's own structure, copied into the program: today's library's "
                                 "instead"].append(n)
        from codebreaker.port import asm16 as _a16
        _old = _a16.CTypes._layouts(list(self.originals.values()), 2, 4)
        _new = _a16.CTypes._layouts(list(self.files.values()), 8, 8)
        _sizes = {k: (_old[k]["@size"], _new[k]["@size"]) for k in _old if k in _new}
        for n in sorted(self.files):                        # (unions whose views meet at the old places: so still)
            self.files[n] = union_views(self.files[n], self.union_notes, n, _sizes)
        self.anon_tags()
        self.view_headers = set()
        self.struct_views()
        self.word_columns()
        self.twin_structs()
        self.cast_twins()
        self.fnptr_returns()
        self.link_words()
        self.array_tails()
        casts = set()                                       # (struct rfld *)&sym->u.rfl: a structure of its own laid
        for t in self.files.values():                       #  over a view of the union - laid out as the view is
            for m in re.finditer(r"\(\s*struct\s+(\w+)\s*(?:far\s*|near\s*|huge\s*)?\*\s*\)\s*&[^;,)]*?\bu\.(\w+)",
                                 blank(t)):
                casts.add((m.group(1), m.group(2)))
        for tag, view in sorted(casts):
            vbody = None
            for t in self.files.values():
                vm = re.search(r"\bstruct\s*(?:\w+\s*)?\{([^{}]*)\}\s*%s\s*;" % re.escape(view), t)   # (tagged or not)
                if vm and "_old" in vm.group(1):
                    vbody = vm.group(1)
            if not vbody:
                continue
            for n, t in self.files.items():
                sm = re.search(r"\bstruct\s+%s\s*\{[^{}]*\}" % re.escape(tag), t)
                if sm:
                    new = "struct %s {%s}" % (tag, vbody)
                    self.files[n] = t[:sm.start()] + new + t[sm.end():]
                    self.union_notes["a structure laid over a view of a union (by a cast): laid out as the view "
                                     "is"].append("%s (struct %s, u.%s)" % (n, tag, view))
        over = set()                                        # (((struct comsym *)sym)->count: a structure that is
        for t in self.files.values():                       #  another's first bytes and then members of it)
            for m in re.finditer(r"\(\s*\(\s*struct\s+(\w+)\s*(?:far\s*|near\s*|huge\s*)?\*\s*\)\s*(\w+)\s*\)\s*->",
                                 blank(t)):
                d_ = re.search(r"\bstruct\s+(\w+)\s*(?:far\s*|near\s*|huge\s*)?\*\s*%s\b" % re.escape(m.group(2)),
                               blank(t))
                if d_:
                    over.add((m.group(1), "struct " + d_.group(1)))
        if over:
            from codebreaker.port import asm16
            old_l = asm16.CTypes._layouts(list(self.originals.values()), 2, 4)
            new_l = asm16.CTypes._layouts(list(self.files.values()), 8, 8)
            ct_ = asm16.CTypes.__new__(asm16.CTypes)
            ct_.old, ct_.new = old_l, new_l
            for tag, under in sorted(over):
                for n, t in self.files.items():
                    sm = re.search(r"\bstruct\s+%s\s*\{\s*char\s+(\w+)\s*\[\s*(0[xX][0-9a-fA-F]+|\d+)\s*\]\s*;" %
                                   re.escape(tag), blank(t))   # (its comments aside: the places are the text's)
                    if not sm:
                        continue
                    k = int(sm.group(2), 0)
                    f = ct_.field(under, k) if under in old_l else None   # (the member of the other there)
                    best = (under, f[3]) if f and f[1] == k else None
                    if best and best[1] != k:
                        self.files[n] = t[:sm.start(2)] + str(best[1]) + t[sm.end(2):]
                        self.union_notes["a structure that is another's first bytes and then members of it: the "
                                         "bytes as many as the other's now"].append(
                            "%s (struct %s over %s: %d)" % (n, tag, best[0], best[1]))
        fp = re.compile(r"FP_SEG\s*\(\s*(\w+)\s*\)\s*=\s*([^;]+);\s*FP_OFF\s*\(\s*\1\s*\)\s*=\s*([^;]+);")
        for n, t in self.files.items():                     # (a far pointer made of a segment and an offset: the
            new = fp.sub(lambda m: "%s = dos_far(%s, %s);" % (m.group(1), m.group(2).strip(), m.group(3).strip()),
                         t)                                 #  platform layer gives the place, as DOS had it)
            if new != t:
                self.files[n] = new
                self.notes_init = getattr(self, "notes_init", collections.Counter())
        kw = profile.get("keywords_removed")                # (far, near, pascal...: the 8086's segments and
        if kw:                                              #  calling conventions, which a flat memory has not)
            mac = set()                                     # (a macro of those words alone - #define NEAR near
            for t in self.files.values():                   #  pascal: gone too, and the word where it was used)
                for m in re.finditer(r"(?m)^[ \t]*#[ \t]*define[ \t]+(\w+)[ \t]+([\w \t]+?)[ \t]*$", t):
                    if set(m.group(2).split()) <= set(kw):
                        mac.add(m.group(1))
            if mac:
                drx = re.compile(r"(?m)^[ \t]*#[ \t]*define[ \t]+(?:%s)[ \t]+[\w \t]*\n" % "|".join(sorted(mac)))
                for n, t in self.files.items():
                    self.files[n] = drx.sub("", t)
            rx = re.compile(r"(?<!->)(?<!\.)\b(?:%s)\b(?!\s*[,;=\[)])[ \t]*" % "|".join(map(re.escape, list(kw) + sorted(mac))))
            #                 (a qualifier: not a name of its own - char near, x3; p->far)
            for n, t in self.files.items():
                new = sub_code(t, rx, lambda m: "")
                if new != t:
                    self.files[n] = new
                    self.notes_init = getattr(self, "notes_init", collections.Counter())
                    self.notes_init[n] += len(rx.findall(blank(t)))
        self.fns = {}
        self.data = []
        self.line_origin = {}                               # ({file: {line number: (its text before the rules, n)}})
        self.line_fixes = {}                                # ((file, a line's text): the line mended) - the loop's
        self.mended = collections.defaultdict(list)         #  (and its notes), kept from one round to the next
        self.renames = dict(profile.get("platform", {}).get("renames", {}))   # (a function whose name the C
                                                            #  library has: renamed; the platform's own)
        self.decl_rules = []                                # ((pattern, replacement) on every file)
        self.fptr, self.fptr_done = {}, set()               # (a function pointer: the types of what it is given)
        self.param_types = {}                               # ((function, parameter index): its type, the loop's)
        self.int16 = profile.get("int_storage") == 16       # (every int 16 bits; else only where bytes are laid out)
        self.grown = {}                                     # (an array the sanitizer saw reached past its end: size)
        self.retyped = {}                                   # (an int variable that holds a pointer: its type)

    def _runtime(self, name, text):
        p = self.profile
        if name.endswith(".h") and name in p["runtime_headers"]:
            return "the old C library's header: the C library of today has its own"
        if name.endswith(".c"):
            code = blank(text)
            got = [f for f, s, j, k in definitions(text.split("\n"), code.split("\n"))
                   if f.name in p["runtime_defines"]]
            if got:
                return "the old C library (it defines %s): the C library of today stands in" % \
                       ", ".join(f.name for f in got)
        return None

    def agree_globals(self):
        """extern unsigned g; in one file, extern char *g; in another (the old linker did not mind): a pointer in
        every file - and the number that was the top of the segment (0xffff), the top of memory now"""
        if getattr(self, "_agreed", False):
            return
        self._agreed = True
        decl = re.compile(r"(?m)^(\s*extern\s+)((?:unsigned|int|short|unsigned\s+int)\s+)(\w+)\s*;")
        ptrs = {}
        for n, t in self.files.items():
            for m in re.finditer(r"(?m)^\s*extern\s+((?:const\s+)?(?:struct\s+\w+|union\s+\w+|\w+)\s*\*+)\s*(\w+)\s*;",
                                 blank(t)):
                ptrs.setdefault(m.group(2), re.sub(r"\s+", " ", m.group(1)).replace(" *", "*").replace("*", " *", 1))
        for n, t in self.files.items():                     # (and its definition's: FILE *g = 0;)
            for m in re.finditer(r"(?m)^((?:const\s+)?(?:struct\s+\w+|union\s+\w+|(?!static\b|extern\b|return\b)\w+)"
                                 r"\s*\*+)\s*(\w+)\s*(?:=[^;]*)?;", blank(t)):
                ptrs.setdefault(m.group(2), re.sub(r"\s+", " ", m.group(1)).replace(" *", "*").replace("*", " *", 1))
        self.agree_notes = []
        many = re.compile(r"(?m)(?:^|(?<=;))([ \t]*extern\s+)((?:unsigned|int|short|unsigned\s+int)\s+)(\w+(?:\s*,\s*\w+)+)\s*;")
        for n in sorted(self.files):                        # (extern int f, g; - g a pointer: a line of its own)
            t = self.files[n]
            for m in reversed(list(many.finditer(blank(t)))):
                names = [w.strip() for w in m.group(3).split(",")]
                if not any(w in ptrs for w in names):
                    continue
                rest = [w for w in names if w not in ptrs]
                new = ";\n".join(([m.group(1) + m.group(2) + ", ".join(rest)] if rest else []) +
                                   ["%s%s%s" % (m.group(1), m.group(2), w) for w in names if w in ptrs]) + ";"
                t = t[:m.start()] + new + t[m.end():]
            self.files[n] = t
        for n in sorted(self.files):
            t = self.files[n]
            names = [m.group(3) for m in decl.finditer(blank(t)) if m.group(3) in ptrs]
            for name in names:
                typ = ptrs[name]
                t = re.sub(r"(?m)^(\s*extern\s+)(?:unsigned|int|short|unsigned\s+int)\s+%s\s*;" % re.escape(name),
                           lambda m: "%s%s%s;" % (m.group(1), typ + ("" if typ.endswith("*") else " "), name), t)
                t = re.sub(r"\b(%s\s*=\s*)(?:0[xX][fF]{4}|65535)\b" % re.escape(name),
                           lambda m: "%s(%s)UINTPTR_MAX" % (m.group(1), typ), t)
                self.agree_notes.append("%s (%s)" % (n, name))
            self.files[n] = t

    def joins_next(self, var):
        """whether a table's definition is followed in its file by one of the same type (what an index past its
        end reached, in the original)"""
        defn = re.compile(r"(?m)^((?:unsigned\s+|signed\s+)?(?:char|short|int|long|unsigned))\s+%s\s*\[\s*\d*\s*\]\s*="
                          r"[^;]*;[^\n]*\n\1\s+\w+\s*(?:\[\s*\d*\s*\])?\s*=" % re.escape(var))
        return any(defn.search(t) for t in self.files.values())

    def fixed_allocators(self):
        """char *pool_alloc() { ... malloc(36) ... }, its callers (struct node *)pool_alloc(): the program's own
        allocator of blocks of one old size, for structures - blocks of the structures' size now"""
        if getattr(self, "_fixed", False):
            return
        self._fixed = True
        old = self.old_layouts()
        users = collections.defaultdict(set)
        for n, t in self.files.items():
            for m in re.finditer(r"\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*(\w+)\s*\(\s*\)", blank(t)):
                users[m.group(2)].add(re.sub(r"\s+", " ", m.group(1)))
        for n in sorted(self.files):
            t = self.files[n]
            code = blank(t)
            for f, s, j, k in definitions_any(t.split("\n"), code.split("\n")):
                structs = sorted(x for x in users.get(f, ()) if x in old)
                if not structs:
                    continue
                lines = t.split("\n")
                clines = code.split("\n")
                for x in range(j, k + 1):
                    m = re.search(r"\b\w*alloc\w*\s*\(\s*(0[xX][0-9a-fA-F]+|\d+)\s*\)", clines[x])
                    if not m:
                        continue
                    size = int(m.group(1), 0)
                    if size < max(old[st_]["@size"] for st_ in structs):
                        continue                            # (not the structures' blocks)
                    expr = "sizeof(%s)" % structs[0]
                    for st_ in structs[1:]:
                        expr = "(sizeof(%s) > %s ? sizeof(%s) : %s)" % (st_, expr, st_, expr)
                    rest = size - max(old[st_]["@size"] for st_ in structs)
                    if rest:
                        expr = "%s + %d" % (expr, rest)
                    a, b = m.span(1)
                    lines[x] = lines[x][:a] + expr + lines[x][b:]
                    self.notes_init = getattr(self, "notes_init", collections.Counter())
                    self.alloc_notes = getattr(self, "alloc_notes", []) + [
                        "%s:%d (%s: %d for %s)" % (n, x + 1, f, size, ", ".join(structs))]
                t = "\n".join(lines)
            self.files[n] = t

    def array_bounds(self):
        """if (x >= g_16ec) - x a struct xstate * into g_123c[200], g_16ec the variable the old linker put right
        after g_123c (another type's): the comparison was with g_123c's end - (g_123c + 200)"""
        if getattr(self, "_bounds_done", False):
            return
        self._bounds_done = True
        ds = getattr(self, "ds", {}) or {}
        if not ds:
            return
        old = self.old_layouts()
        arrays = {}                                         # (name: (base type, elements, old element size))
        for t in self.originals.values():
            code = blank(t)
            dps = depths(code.split("\n"))
            for i, l in enumerate(code.split("\n")):
                if dps[i] or l.lstrip().startswith(("extern", "#", "typedef")):
                    continue
                m = re.match(r"\s*(?:static\s+)?((?:struct|union)\s+\w+|(?:unsigned\s+)?\w+)\s+(\*?)\s*(\w+)\s*\[\s*(\d+)\s*\]"
                             r"\s*[;=]", l)
                if not m or m.group(3) not in ds:
                    continue
                base = re.sub(r"\s+", " ", m.group(1))
                esz = 2 if m.group(2) else old[base]["@size"] if base in old else OLD_SCALARS.get(base, 0)
                if esz:
                    arrays[m.group(3)] = (base + (" *" if m.group(2) else ""), int(m.group(4)), esz)
        starts = {}
        for nm, at in ds.items():
            starts.setdefault(at, []).append(nm)
        for a, (base, cnt, esz) in sorted(arrays.items()):
            after = [b for b in starts.get(ds[a] + cnt * esz, []) if b != a]
            for b in after:
                bbase = arrays.get(b, (None,))[0]
                if bbase == base:
                    continue                                # (one type: the tables' join does it)
                for n in sorted(self.files):
                    t = self.files[n]
                    code = blank(t)
                    edits = []
                    for m in re.finditer(r"(?<![\w.>])([\w.\->]+)\s*(>=|<=|<|>)\s*%s\b(?!\s*[\[.(])|"
                                         r"(?<![\w.>])%s\b(?!\s*[\[.(])\s*(>=|<=|<|>)\s*([\w.\->]+)" %
                                         (re.escape(b), re.escape(b)), code):
                        v = m.group(1) or m.group(4)
                        last = re.findall(r"\w+", v)[-1]
                        pt = self.pointee_of(t, last) if re.fullmatch(r"\w+", v) else None
                        if pt and re.sub(r"\s+", " ", pt) == base.replace(" *", ""):
                            bs, be = (m.start(), m.end())
                            seg = code[bs:be]
                            k = seg.rfind(b) if m.group(1) else seg.find(b)
                            edits.append((bs + k, bs + k + len(b), "(%s + %d)" % (a, cnt)))
                    for x, y, new in sorted(edits, reverse=True):
                        t = t[:x] + new + t[y:]
                    if edits:
                        self.files[n] = t
                        self.notes["a pointer into a table compared with the variable the old linker put after it: "
                                   "the table's end"].append("%s (%s: %s + %d)" % (n, b, a, cnt))

    def word_stacks(self):
        """int *sp; ... sp[1] = out; fp = (int *)sp[2]; - words that held pointers as well as numbers (a stack of
        frames, a list of results): the words a pointer's width - intptr_t - and every int * and int array they
        are one with (given to each other, compared, passed on to a parameter)"""
        if getattr(self, "_stacks_done", False):
            return
        self._stacks_done = True
        INT = r"(?:register\s+)?(?:unsigned\s+|signed\s+)?(?:int|short|unsigned)"
        ptrs = set()                                        # (names declared pointers anywhere, at the top)
        arrays = {}                                         # (an int array at a file's top: where it is)
        for n, t in self.files.items():
            code = blank(t)
            ds = depths(code.split("\n"))
            for i, l in enumerate(code.split("\n")):
                if ds[i]:
                    continue
                for m in re.finditer(r"\*\s*(\w+)\s*(?=[;,=\[)])", l):
                    if "(" not in l[:m.start()] or "(*" in l:
                        ptrs.add(m.group(1))
                m = re.match(r"\s*(?:extern\s+|static\s+)?%s\s+(\w+)\s*\[[^\]]*\]\s*(?:=[^;]*)?;" % INT, l)
                if m:
                    arrays.setdefault(m.group(1), []).append(n)
        parent = {}

        def find(x):
            while parent.setdefault(x, x) != x:
                parent[x] = parent[parent[x]]
                x = parent[x]
            return x

        def union(a, b):
            parent[find(a)] = find(b)
        seeds, nodes, params = set(), {}, {}               # (node: (file, function or None, name))
        defs = {}
        for n in sorted(self.files):
            if not n.endswith(".c"):
                continue
            t = self.files[n]
            lines, code = t.split("\n"), blank(t).split("\n")
            for f, s_, j_, k_ in definitions(lines, code):
                head = "\n".join(code[s_:j_])
                body = "\n".join(code[j_:k_ + 1])
                names = [pn for pn, pt in f.params]
                mine = {}                                   # (an int * of the function's: its node)
                for d in re.finditer(r"(?m)^\s*%s\s+([^;(){}]*);" % INT, head + "\n" + body):
                    for x in re.finditer(r"(?:^|,)\s*(?:\*\s*(\w+)|(\w+)\s*\[[^\]]*\])\s*(?=,|$)", d.group(1)):
                        mine[x.group(1) or x.group(2)] = (n, f.name, x.group(1) or x.group(2))
                defs[f.name] = (n, names)
                for k, pn in enumerate(names):
                    if pn in mine:
                        params[(f.name, k)] = mine[pn]
                local_ptrs = set(re.findall(r"\*\s*(\w+)\s*[;,=)\[]", head + "\n" +
                                            "\n".join(l for l in body.split("\n") if l.strip().endswith(";") and
                                                       re.match(r"\s*(?:register\s+)?(?:struct|union|char|int|"
                                                                r"unsigned|long|short)\b", l))))

                def node(v):
                    if v in mine:
                        return mine[v]
                    if v in arrays:
                        return ("", None, v)
                    return None
                for v in set(mine) | set(a for a in arrays if re.search(r"\b%s\b" % re.escape(a), body)):
                    nodes[node(v)] = True
                for m in re.finditer(r"(?:\b(\w+)\s*\[[^\]]*\]|\*\s*(\w+)\s*(?:\+\+|--)?)\s*=(?!=)\s*([^;]*);", body):
                    v = node(m.group(1) or m.group(2))
                    e = m.group(3).strip()
                    if v and (re.fullmatch(r"&\s*\w+.*|\".*", e) or
                              (re.fullmatch(r"\w+", e) and (e in local_ptrs or e in mine or e in ptrs))):
                        seeds.add(v)                        # (a pointer kept in a word of it)
                for m in re.finditer(r"\(\s*(?:struct\s+\w+|\w+)\s*\*+\s*\)\s*(\w+)\s*\[", body):
                    if node(m.group(1)):
                        seeds.add(node(m.group(1)))         # (a word of it read as a pointer)
                for m in re.finditer(r"\(\s*(?:struct\s+\w+|\w+)\s*\*\s*\*\s*\)\s*(\w+)\b(?!\s*\[)", body):
                    if node(m.group(1)):
                        seeds.add(node(m.group(1)))         # (it, taken as pointers)
                for m in re.finditer(r"\b(\w+)\s*(?:=(?!=)|[<>]=?|==|!=)\s*(?:\(\s*\w+\s*\*\s*\)\s*)?(\w+)\b"
                                     r"(?!\s*[\[(])", body):
                    a, b = node(m.group(1)), node(m.group(2))
                    if a and b:
                        union(a, b)                         # (one of them given the other, or compared)
                for m in re.finditer(r"\b(\w+)\s*=\s*\(\s*%s\s*\*\s*\)\s*\w+\s*\[" % INT.replace("(?:register\\s+)?", ""),
                                     body):
                    if node(m.group(1)):
                        seeds.add(node(m.group(1)))         # (fp = (int *)sp[2]: a pointer kept in a word)
        for n in sorted(self.files):                        # (a call: its arguments one with the parameters)
            if not n.endswith(".c"):
                continue
            t = self.files[n]
            lines, code = t.split("\n"), blank(t).split("\n")
            for f, s_, j_, k_ in definitions(lines, code):
                body = "\n".join(code[j_:k_ + 1])
                mine = {}
                for d in re.finditer(r"(?m)^\s*%s\s+([^;(){}]*);" % INT, "\n".join(code[s_:k_ + 1])):
                    for x in re.finditer(r"(?:^|,)\s*(?:\*\s*(\w+)|(\w+)\s*\[[^\]]*\])\s*(?=,|$)", d.group(1)):
                        mine[x.group(1) or x.group(2)] = (n, f.name, x.group(1) or x.group(2))
                for g, (gfile, gnames) in defs.items():
                    for c in re.finditer(r"(?<![\w.>])%s\s*\(" % re.escape(g), body):
                        sp_ = call_args(body[c.start():c.start() + 400], g) or []
                        for k, (a, b) in enumerate(sp_):
                            arg = body[c.start() + a:c.start() + b].strip()
                            if (g, k) in params and re.fullmatch(r"\w+", arg):
                                v = mine.get(arg) or (("", None, arg) if arg in arrays else None)
                                if v:
                                    union(v, params[(g, k)])
        wide = {find(x) for x in seeds}
        chosen = {x for x in set(parent) | set(nodes) | seeds if find(x) in wide}
        if not chosen:
            return
        for (n, fname, v) in sorted(chosen, key=lambda x: (x[0], x[1] or "", x[2])):
            if fname is None:                               # (an array: its definition and every extern of it)
                for f in arrays.get(v, []):
                    t = self.files[f]
                    t = re.sub(r"(?m)^(\s*(?:extern\s+|static\s+)?)%s(\s+%s\s*\[)" % (INT, re.escape(v)),
                               r"\1intptr_t\2", t)
                    self.files[f] = t
                self.notes["words that held pointers too (a stack, a list of results): intptr_t"].append(v)
                continue
            t = self.files[n]
            lines, code = t.split("\n"), blank(t).split("\n")
            for f, s_, j_, k_ in definitions(lines, code):
                if f.name != fname:
                    continue
                for x in range(s_, k_ + 1):
                    m = re.match(r"(\s*(?:register\s+)?)(?:unsigned\s+|signed\s+)?(?:int|short|unsigned)(\s+)(.*;.*)$",
                                 lines[x])
                    me_ = r"\*\s*%s|%s\s*\[[^\]]*\]" % (re.escape(v), re.escape(v))   # (int *v, or int v[4])
                    if not m or not any(re.fullmatch(me_, d_.strip())
                                        for d_ in split_top(m.group(3).split(";")[0])):
                        continue
                    decls = [d_.strip() for d_ in split_top(m.group(3).split(";")[0])]
                    rest = m.group(3)[len(m.group(3).split(";")[0]):]
                    keep = [d_ for d_ in decls if not re.fullmatch(me_, d_)]
                    mine_d = next(d_ for d_ in decls if re.fullmatch(me_, d_))
                    base = lines[x][len(m.group(1)):lines[x].index(m.group(2), len(m.group(1)))]
                    new = m.group(1) + "intptr_t %s;" % mine_d
                    if keep:
                        new = m.group(1) + base + " " + ", ".join(keep) + "; intptr_t %s;" % mine_d
                    lines[x] = new + rest[1:]
                    break
                body = "\n".join(lines[j_:k_ + 1])         # (fp = (int *)sp[2]: an intptr_t * taken out)
                body = re.sub(r"(\b%s\s*=\s*\(\s*)(?:unsigned\s+|signed\s+)?(?:int|short|unsigned)(\s*\*\s*\))" %
                              re.escape(v), r"\1intptr_t\2", body)
                lines[j_:k_ + 1] = body.split("\n")
            self.files[n] = "\n".join(lines)
            self.notes["words that held pointers too (a stack, a list of results): intptr_t"].append(
                "%s (%s: %s)" % (n, fname, v))

    def merge_tables(self):
        """a table the program reads past its end at a constant place (t[7] of a t[3]): it went on into the
        variables defined after it in its file - one table again, those names its elements"""
        reach = getattr(self, "reach", {})                  # (and where a run read one past its end: sanitize)
        if getattr(self, "_merged", False) == len(reach) + 1:
            return
        self._merged = len(reach) + 1
        uses = collections.defaultdict(int)
        for n, t in self.files.items():
            for m in re.finditer(r"\b([A-Za-z_]\w*)\s*\[\s*(\d+)\s*\]", blank(t)):
                uses[m.group(1)] = max(uses[m.group(1)], int(m.group(2)))
        for k_, v_ in reach.items():
            uses[k_] = max(uses[k_], v_)
        defn = re.compile(r"^(?P<base>(?:unsigned\s+|signed\s+)?(?:char|short|int|long|unsigned|intptr_t))\s+(?P<name>\w+)"
                          r"\s*(?P<arr>\[\s*\d*\s*\])?\s*(?:=\s*(?P<val>\{[^;{}]*\}|[^;{}]*?)\s*)?;(?P<rest>.*)$")
        def elems(m_):                                      # (an array without initialiser: its size in zeros)
            if m_.group("val") is not None:
                return None
            sz = re.search(r"\d+", m_.group("arr") or "")
            return ["0"] * (int(sz.group(0)) if sz else 1) if (sz or not m_.group("arr")) else None
        for n in sorted(self.files):
            if not n.endswith(".c"):
                continue
            lines = self.files[n].split("\n")
            i = 0
            while i < len(lines):
                m = defn.match(lines[i])
                vals = None
                if m and m.group("arr") and m.group("val") is None:
                    vals = elems(m)
                elif m and m.group("arr"):
                    vals = [v.strip() for v in m.group("val").strip("{} ").split(",") if v.strip()]
                    sz_ = re.search(r"\d+", m.group("arr"))         # (int t[2691] = { 0 }: its 2691, the rest 0)
                    if sz_ and len(vals) < int(sz_.group(0)) and not m.group("val").lstrip().startswith('"'):
                        vals += ["0"] * (int(sz_.group(0)) - len(vals))
                    if m.group("val").lstrip().startswith('"'):   # (a string: its bytes, and the 0 after)
                        vals = None
                        lit = m.group("val").strip()
                        size = re.search(r"\d+", m.group("arr"))
                        size = int(size.group(0)) if size else len(c_string_bytes(lit)) + 1
                        if uses.get(m.group("name"), -1) >= size:
                            self.notes["a string read past its end at a constant place (not joined: "
                                       "check)"].append("%s (%s)" % (n, m.group("name")))
                if not m or not vals or uses.get(m.group("name"), -1) < len(vals):
                    i += 1
                    continue
                base, name = m.group("base"), m.group("name")
                parts = [(name, 0, len(vals), True)]
                j = i + 1
                while j < len(lines):                       # (what follows, of the same type: the table's rest)
                    m2 = defn.match(lines[j])
                    if not m2 or m2.group("base") != base:
                        break
                    if m2.group("val") is None:
                        v2 = elems(m2)
                        if v2 is None:
                            break
                    elif m2.group("arr") and m2.group("val").lstrip().startswith('"'):
                        lit = m2.group("val").strip()               # (a string among them: its bytes as elements)
                        bs = list(c_string_bytes(lit))
                        size = re.search(r"\d+", m2.group("arr"))
                        bs += [0] * ((int(size.group(0)) if size else len(bs) + 1) - len(bs))
                        v2 = [str(x) for x in bs]
                    else:
                        v2 = [v.strip() for v in m2.group("val").strip("{} ").split(",") if v.strip()] \
                            if m2.group("arr") else [m2.group("val").strip()]
                        sz_ = re.search(r"\d+", m2.group("arr") or "")
                        if sz_ and len(v2) < int(sz_.group(0)):
                            v2 += ["0"] * (int(sz_.group(0)) - len(v2))
                    parts.append((m2.group("name"), len(vals), len(v2), bool(m2.group("arr"))))
                    vals += v2
                    j += 1
                    if len(vals) > uses[name] and not any(uses.get(p[0], -1) >= p[2] for p in parts if p[3]):
                        pass
                if len(parts) == 1:
                    i += 1
                    continue
                body = ", ".join(vals)
                if any(v != "0" for v in vals):
                    lines[i:j] = ["%s %s[%d] = { %s };\t/* (one table: %s) */" % (base, name, len(vals), body,
                                                                             ", ".join(p[0] for p in parts))]
                else:                                       # (none given a value: one block of them)
                    lines[i:j] = ["%s %s[%d];\t/* (one table: %s) */" % (base, name, len(vals),
                                                                    ", ".join(p[0] for p in parts))]
                for p in parts[1:]:
                    self.aliases[p[0]] = (("(&%s[%d])" % (name, p[1])) if p[3] else "%s[%d]" % (name, p[1]),
                                          "%s %s[%d];" % (base, name, len(vals)))
                self.notes_init = getattr(self, "notes_init", collections.Counter())
                self.table_notes = getattr(self, "table_notes", []) + ["%s (%s: %s)" % (
                    n, name, ", ".join(p[0] for p in parts[1:]))]
                i += 1
            self.files[n] = "\n".join(lines)

    def modernize(self):
        self.array_bounds()
        self.word_stacks()
        self.merge_tables()
        self.fixed_allocators()
        self.agree_globals()
        self.notes = collections.defaultdict(list, {k: list(v) for k, v in self.mended.items()})
        for x in getattr(self, "agree_notes", []):
            self.notes["a variable another file declares a pointer, declared a number here (the old linker did "
                       "not mind): the pointer; 0xffff, the top of the segment, the top of memory"].append(x)
        for x in getattr(self, "alloc_notes", []):
            self.notes["the program's own allocator of blocks of a structure's old size: the structure's size "
                       "now"].append(x)
        for k_, v_ in self.union_notes.items():
            self.notes[k_] += v_
        for x in self.member_fixes:
            self.notes["member retyped: the source declared it otherwise than the program uses it (--types)"].append(x)
        for x in getattr(self, "table_notes", []):
            self.notes["a table read past its end into the variables after it: one table, their names its "
                       "elements"].append(x)
        for n, c in getattr(self, "notes_init", {}).items():
            self.notes["far / near / pascal and their kind dropped (one flat memory, one way of calling)"].append(
                "%s (%d)" % (n, c))
        self.data = []
        known_lib = set(self.profile["libc"]) | set(self.profile["platform"]["calls"])
        heads = {}
        for n in sorted(self.files):                        # (the definitions first: every name the program has)
            if n.endswith(".c"):
                t = self.files[n]
                for f, s, j, k in definitions(t.split("\n"), blank(t).split("\n")):
                    self.fns.setdefault(f.name, f).file = n
        self.bss_views()
        self.pass_through_params()
        known = known_lib | set(self.fns) | set(self.asm_protos)
        lower = {}                                          # (pascal names were made capitals for the linker: a
        for f in list(self.fns) + list(self.asm_protos):    #  call in capitals and a definition in small letters
            lower.setdefault(f.lower(), f)                  #  were one function)
        for n, t in self.files.items():
            for c in set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", blank(t))):
                d = lower.get(c.lower())
                if d and d != c and c not in known and c not in self.renames:
                    self.renames[c] = d
                    self.notes["a pascal name called in other letters (the linker made both capitals): the "
                               "definition's"].append("%s (%s)" % (n, c))
        for n in sorted(self.files):                        # (the callers' declarations say what a function gives,
            code = blank(self.files[n])                     #  where its definition left it old C's int)
            for m in re.finditer(r"(?m)^\s*(?:extern\s+)?((?:unsigned\s+|signed\s+)?(?:struct\s+\w+|union\s+\w+|\w+)"
                                 r"(?:\s*\*+\s*|\s+))(\w+)\s*\(\s*\)\s*;", code):
                t, name = re.sub(r"\s+", " ", m.group(1)).strip(), m.group(2)
                f = self.fns.get(name)
                if f and not f.ret and t not in ("int", "extern") and not t.startswith(("extern", "static")) \
                   and t.split()[0] in TYPEWORDS | {"struct", "union"}:
                    f.ret, f.ret_set = t.strip(), True
                    self.notes["what a function of old C's int gives, as its callers declared it"].append(
                        "%s (%s: %s)" % (n, name, f.ret))
        out = {}
        for n in sorted(self.files):
            t = self.files[n]
            t = self._preprocessor(t, n)
            t = sub_code(t, re.compile(r"(?m)^([ \t]*(?:register|auto|static)\s+)(?=\**\s*\w+\s*(?:\[[^\]]*\]\s*)?"
                                       r"(?:[,;=]))(?!(?:unsigned|signed|int|char|short|long|float|double|void|"
                                       r"const|volatile|register|static)\b)"),
                         lambda m: m.group(1) + "int ")     # (register i; - old C's int, unsaid)
            code = blank(t)
            macro_names = {a for a, b in re.findall(r"(?m)^\s*#\s*define\s+(\w+)\s+(\w+)\s*$", t) if b in known}
            t = old_declarations(t, code, known | macro_names, self.notes, n,   # (#define yylex f049ab: f049ab's)
                                 list(self.profile["platform"]["calls"]) + list(self.aliases) +
                                 [re.search(r"[A-Za-z_]\w*", w).group(0) for w, d in self.aliases.values()])
            if n.endswith(".h"):
                t, defs = header_data(t, blank(t), n, self.notes)
                self.data += [(n, d) for d in defs]
            out[n] = t
        tags, twice = {}, set()                             # (a structure: the header that defines it - one alone;
        for h, t in out.items():                            #  a tag two headers define is either's: none added)
            if h.endswith(".h"):
                for m in re.finditer(r"\b(struct|union)\s+(\w+)\s*\{", blank(t)):
                    k_ = "%s %s" % (m.group(1), m.group(2))
                    if tags.get(k_, h) != h:
                        twice.add(k_)
                    tags.setdefault(k_, h)
        for k_ in twice:
            tags.pop(k_, None)
        for n, t in list(out.items()):
            if not n.endswith(".c"):
                continue
            code = blank(t)
            own = {"%s %s" % (m.group(1), m.group(2)) for m in re.finditer(r"\b(struct|union)\s+(\w+)\s*\{", code)}
            inc, todo_ = set(), re.findall(r'#\s*include\s*"([^"]+)"', t)   # (and what those include, in turn)
            while todo_:
                h_ = todo_.pop()
                if h_ not in inc:
                    inc.add(h_)
                    todo_ += re.findall(r'#\s*include\s*"([^"]+)"', out.get(h_, ""))
            need = sorted({tags[u] for u in set(re.findall(r"\b(?:struct|union)\s+\w+", code)) - own
                           if u in tags and tags[u] not in inc} |
                          {h_ for h_ in self.need_headers.get(n, ()) if h_ not in inc})
            if need:                                        # (the old build gave it the header some other way:
                ls = t.split("\n")                          #  after its own - what those need comes first)
                at = max(i for i, l in enumerate(ls) if re.match(r"\s*#\s*include\b", l))
                ls[at + 1:at + 1] = ['#include "%s"' % h for h in need]
                t = "\n".join(ls)
                out[n] = t
                self.notes["header of the structures a file uses, which it did not include: included"].append(
                    "%s (%s)" % (n, ", ".join(need)))
            more = sorted(d_ for d_ in self.extra_decls.get(n, ()) if d_ not in t)
            if more:                                        # (a variable a mend reads there: declared)
                ls = out[n].split("\n")
                at = max([i for i, l in enumerate(ls) if re.match(r"\s*#\s*include\b", l)] + [-1])
                ls[at + 1:at + 1] = more
                out[n] = "\n".join(ls)
        for n, t in out.items():
            if n.endswith(".c"):
                out[n] = self._definitions(t, n)
        for n, t in out.items():
            code = blank(t)
            t, code = widths(t, code, self.notes, n, self.int16)
            t = word_unions(t, self.notes, n, set(getattr(self, "old_records", ())))
            t = word_copies(t, self.notes, n, self.copied_members())
            t = stdio_inits(t, self.notes, n)
            t = file_internals(t, self.notes, n)
            t = whole_word_stores(t, self.notes, n)
            t = self.offset_views(t, n)
            t = block_heads(t, self.notes, n, self.ptr2_names())
            t = self.bss_ends(t, n)
            t = formats(t, code, self.notes, n, self.int16, self.profile.get("printf_hex_upper", False))
            t = struct_sizes(t, self.old_layouts_for(n), self.notes, n, self.allocators())
            t = header_offsets(t, self.old_layouts_for(n), self.notes, n)
            t = mem_sizes(t, self.old_var_sizes(), self.old_elem_sizes(), self.notes, n)
            out[n] = t
        for n, t in out.items():                            # (the program's allocators: the old sizes they keep
            for fn_, sizes in POOLS.items():                #  their blocks by, the structures' sizes now)
                f = self.fns.get(fn_)
                if f is None or getattr(f, "file", None) != n:
                    continue
                code = blank(t)
                dm = re.search(r"(?m)^[\w\s*]*\b%s\s*\([^;]*\)\s*$" % re.escape(fn_), code)
                if not dm:
                    continue
                body_end = code.find("\n}", dm.end())
                for st_, old_n in sizes:
                    seg = t[dm.end():body_end]
                    new = re.sub(r"(?<![\w.])%d(?![\w.])" % old_n, "sizeof(%s)" % st_, seg)
                    if new != seg:
                        t = t[:dm.end()] + new + t[body_end:]
                        body_end += len(new) - len(seg)
                        h = tags.get(st_)
                        if h and '#include "%s"' % h not in t:   # (the structure's header, for its size)
                            t = t.replace('#include "port.h"', '#include "port.h"\n#include "%s"' % h, 1)
                            body_end += len('\n#include "%s"' % h)
                        self.notes["the program's own allocator keeps its blocks by a structure's old size: its "
                                   "size now"].append("%s (%s: %d)" % (n, fn_, old_n))
            out[n] = t
        frames(self, out)
        self.loose_calls(out)
        self.array_sizes = {}                               # (each array the program defines: its elements)
        for n, t in list(out.items()) + [("bss.c", "\n".join(self.bss_definitions()))]:
            for rx, new in self.decl_rules:
                t = re.sub(rx, new, t)
            c = blank(t)
            ds_ = depths(c.split("\n"))
            for i, l in enumerate(c.split("\n")):
                if ds_[i] or l.lstrip().startswith(("extern", "#", "typedef")):
                    continue
                m = re.match(r"\s*(?:static\s+)?[\w\s*]*?\b(\w+)\s*\[\s*(\d+)\s*\]\s*(?:=|;)", l)
                if m and "(" not in l.split("[")[0]:
                    self.array_sizes.setdefault(m.group(1), int(m.group(2)))
                    continue
                m = re.match(r"\s*(?:static\s+)?[\w\s*]*?\b(\w+)\s*\[\s*\]\s*=\s*\{", l)
                if m and "(" not in l.split("[")[0]:
                    at = sum(len(x) + 1 for x in c.split("\n")[:i]) + m.end() - 1
                    d, k = 0, at
                    while k < len(c):
                        d += {"{": 1, "}": -1}.get(c[k], 0)
                        if d == 0:
                            break
                        k += 1
                    rows, dd, cur_ = [], 0, ""              # (its rows: commas outside braces)
                    for ch in c[at + 1:k]:
                        dd += (ch in "({[") - (ch in ")}]")
                        if ch == "," and dd == 0:
                            rows.append(cur_)
                            cur_ = ""
                        else:
                            cur_ += ch
                    rows = [x for x in rows + [cur_] if x.strip()]
                    if rows:
                        self.array_sizes.setdefault(m.group(1), len(rows))
        for h in [n for n in out if n.endswith(".h")]:      # (every header of the program's: once - port.h and
            if h in out and "DOSRT_ONCE_" not in out[h]:    #  data.c include some of them too)
                g_ = "DOSRT_ONCE_" + re.sub(r"\W", "_", h).upper()
                out[h] = "#ifndef %s\n#define %s\n%s\n#endif\n" % (g_, g_, out[h].rstrip("\n"))
        for n, t in out.items():
            out[n] = self.finish(n, t)
        self.out = out
        return out

    def loose_calls(self, out):
        """the calls of an old-style definition kept (its calls unchecked, its integer parameters intptr_t): each
        integer given it made an intptr_t - unprototyped, an int 0 leaves the upper half of the 64-bit place what
        it was"""
        loose = {}
        for f in self.fns.values():
            if f.loose:
                loose[self.renames.get(f.name, f.name)] = [
                    bool(re.fullmatch(r"(?:register\s+)?(?:unsigned\s+int|unsigned|short|int|signed|intptr_t)\s+\w+",
                                      t.strip())) for n_, t in f.params]
        if not loose:
            return
        pat = re.compile(r"\b(%s)\s*\(" % "|".join(map(re.escape, sorted(loose))))
        for n, t in out.items():
            if not n.endswith(".c"):
                continue
            lines, code = t.split("\n"), blank(t).split("\n")
            changed = False
            for i, c in enumerate(code):
                if not pat.search(c) or re.match(r"^\s*(?:static\s+)?[\w\s*]*\b\w+\s*\([\w\s,]*\)\s*$", c):
                    continue                                # (the definition's own head)
                l = lines[i]
                for m in reversed(list(pat.finditer(c))):
                    spans = call_args(l[m.start():], m.group(1))
                    if not spans:
                        continue
                    ints = loose[m.group(1)]
                    for k, (a_, b_) in reversed(list(enumerate(spans))):
                        arg = l[m.start() + a_:m.start() + b_]
                        if k >= len(ints) or not ints[k] or not arg.strip() or arg.strip().startswith("(intptr_t)"):
                            continue
                        lead = arg[:len(arg) - len(arg.lstrip())]
                        l = l[:m.start() + a_] + lead + "(intptr_t)(%s)" % arg.strip() + l[m.start() + b_:]
                        changed = True
                if l != lines[i]:
                    lines[i] = l
                    self.notes["an integer given to an old-style definition kept (unchecked calls): made its "
                               "intptr_t"].append("%s:%d" % (n, i + 1))
            if changed:
                out[n] = "\n".join(lines)

    def _globals(self, text):
        """[(line, name, base, shape, initializer or None)] of the one-name definitions at a file's top"""
        code = blank(text).split("\n")
        ds = depths(code)
        out = []
        for i, c in enumerate(code):
            st_ = c.strip()
            if ds[i] or not st_.endswith(";") or st_.startswith(("extern", "typedef", "#")) or \
                    "(" in st_.replace("(*", "") and "=" not in st_:
                continue
            m = re.match(r"([^=]*?)(?:=(.*))?;$", st_)
            if not m:
                continue
            r = declarators(m.group(1).strip())
            if not r or len(r[1]) != 1:
                continue
            nm, dcl = r[1][0]
            base = re.sub(r"\s+", " ", re.sub(r"\bstatic\s+", "", r[0])).strip()
            raw = text.split("\n")[i]
            eq, semi = c.find("="), c.rfind(";")
            out.append((i, nm, base, re.sub(r"\b%s\b" % re.escape(nm), "@", dcl).replace(" ", ""),
                        raw[eq + 1:semi].strip() if m.group(2) is not None else None))
        return out

    def anon_tags(self):
        """struct { char kind, flags; } t; - an anonymous structure given by value (a parameter) that a header's
        member also is: one type, by a tag the header's gets (two anonymous ones are two types)"""
        def norm(b):                                        # (the members, one a declaration: int n, f4; is
            out_ = []                                       #  int n; int f4;)
            for d_ in blank(b).split(";"):
                r_ = declarators(d_.strip()) if d_.strip() else None
                if r_:
                    out_ += ["%s %s" % (re.sub(r"\s+", " ", r_[0]).strip(), dc.replace(" ", "")) for nm, dc in r_[1]]
                elif d_.strip():
                    out_.append(re.sub(r"\s+", "", d_))
            return ";".join(out_)
        named = {}
        for h, t in list(self.files.items()):
            if not h.endswith(".h"):
                continue
            for m in list(re.finditer(r"\bstruct\s*\{([^{}]*)\}\s*(\w+)\s*;", t))[::-1]:
                tag = "%s_%s" % (os.path.splitext(h)[0], m.group(2))
                named.setdefault(norm(m.group(1)), (tag, h))
        for h, t in list(self.files.items()):               # (the header's: tagged)
            if h.endswith(".h"):
                new = re.sub(r"\bstruct\s*\{([^{}]*)\}(\s*)(\w+)\s*;",
                             lambda m: "struct %s {%s}%s%s;" % (named[norm(m.group(1))][0], m.group(1), m.group(2),
                                                                m.group(3)) if norm(m.group(1)) in named and
                             named[norm(m.group(1))][1] == h else m.group(0), t)
                self.files[h] = new
        for n, t in list(self.files.items()):               # (a C file's parameter of the same: the tag)
            if not n.endswith(".c"):
                continue

            def one(m):
                k = norm(m.group(1))
                if k not in named:
                    return m.group(0)
                self.notes["an anonymous structure given by value, a header's member's too: one type, by a "
                           "tag"].append("%s (%s)" % (n, named[k][0]))
                return "struct %s %s;" % (named[k][0], m.group(2))
            t = re.sub(r"\bstruct\s*\{([^{}]*)\}\s*(\w+)\s*;", one, t)   # (a parameter's, a local's)
            own = []

            def two(m):                                     # (register struct { ... } *p; of a definition's
                k = norm(m.group(2))                        #  parameters: a tag of the header's or its own)
                tag = named[k][0] if k in named else "%s_%s" % (os.path.splitext(n)[0], m.group(4))
                if k not in named:
                    own.append("struct %s {%s};" % (tag, m.group(2)))
                self.notes["an anonymous structure of a parameter: a tag (a prototype names it)"].append(
                    "%s (%s)" % (n, tag))
                return "%sstruct %s %s%s;" % (m.group(1), tag, m.group(3), m.group(4))
            t = re.sub(r"(?m)^(\s*(?:register\s+)?)struct\s*\{([^{}]*)\}\s*(\*+)\s*(\w+)\s*;", two, t)
            if own:
                at = max([i for i, l in enumerate(t.split("\n")) if l.startswith("#include")] + [-1]) + 1
                ls = t.split("\n")
                ls[at:at] = own
                t = "\n".join(ls)
            self.files[n] = t

    def array_tails(self):
        """struct pnode { ...; struct pnode *kid[2]; int pa; ... } - kid[k] for k up to 2 (three children), pa never
        named: the word after an array, of its elements' old size, at the place its next element would be, is
        that element - the array one longer, the word gone"""
        from codebreaker.port import asm16
        lays = asm16.CTypes._layouts(list(self.originals.values()), 2, 4)
        allcode = "\n".join(blank(t) for t in self.files.values())
        for n in sorted(self.files):
            t = self.files[n]
            for sm in list(re.finditer(r"\bstruct\s+(\w+)\s*\{([^{}]*)\}", blank(t))):
                tag, lay = sm.group(1), lays.get("struct " + sm.group(1))
                if not lay:
                    continue
                body = t[sm.start(2):sm.end(2)]
                decls = [(m_.start(), m_.end(), m_.group(0)) for m_ in re.finditer(r"[^;{}]+;", body)]
                for (a1, b1, d1), (a2, b2, d2) in zip(decls, decls[1:]):
                    r1 = declarators(blank(d1).strip().rstrip(";"), TYPEWORDS | STDINT)
                    r2 = declarators(blank(d2).strip().rstrip(";"), TYPEWORDS | STDINT)
                    if not r1 or not r2 or len(r1[1]) != 1 or len(r2[1]) != 1:
                        continue
                    (an, ad), (wn, wd) = r1[1][0], r2[1][0]
                    am = re.fullmatch(r"(\**)\s*%s\s*\[\s*(\d+)\s*\]" % re.escape(an), ad.strip())
                    if not am or wd.strip() != wn or an not in lay or wn not in lay:
                        continue
                    count = int(am.group(2))
                    esz = lay[an][1] // max(1, count)
                    if lay[wn][0] != lay[an][0] + lay[an][1] or lay[wn][1] != esz:
                        continue                            # (not where, nor of the size, the next element is)
                    if re.search(r"(?:->|\.)\s*%s\b" % re.escape(wn), allcode) or \
                            re.search(r"(?m)^\s*#\s*define\s+\w+.*\b%s\b" % re.escape(wn), allcode):
                        continue                            # (the word named somewhere: a member of its own)
                    if not re.search(r"(?:->|\.)\s*%s\s*\[\s*(?![0-%d]\s*\])[^\]]+\]" % (re.escape(an), count - 1),
                                     allcode):
                        continue                            # (the array only ever indexed within it)
                    new_d1 = d1.replace("[%s]" % am.group(2), "[%d]" % (count + 1), 1)
                    new_d1 = re.sub(r"\[\s*%d\s*\]" % count, "[%d]" % (count + 1), d1, count=1)
                    body2 = body[:a1] + new_d1 + body[b1:a2] + "\t/* (%s: the word after it, its element %d) */" % (
                        wn, count) + body[b2:]
                    t = t[:sm.start(2)] + body2 + t[sm.end(2):]
                    self.files[n] = t
                    self.notes["a word after an array that its next element was (indexed past it, never named): "
                               "the array one longer"].append("%s (struct %s: %s[%d], %s)" % (n, tag, an, count, wn))
                    break

    def link_words(self):
        """#define NEXT(s) (*(struct isym **)(s)) - NEXT(s) = *pp: a structure's first word its chain's link (an
        int then, the width of a pointer) - its first member that pointer now"""
        macros = {}                                         # (macro: the structure its link points to)
        for t in self.files.values():
            for m in re.finditer(r"(?m)^\s*#\s*define\s+(\w+)\s*\(\s*(\w+)\s*\)\s*\(?\s*\*\s*\(\s*struct\s+(\w+)"
                                 r"\s*\*\s*\*\s*\)\s*\(?\s*\2\s*\)?\s*\)?\s*$", t):
                macros[m.group(1)] = m.group(3)
        uses = {}                                           # (structure: the link's structure)
        for n, t in self.files.items():
            code = blank(t)
            found = [(mm.group(2), x) for x in macros
                     for mm in re.finditer(r"\b(%s)\s*\(\s*(\w+)\s*\)" % re.escape(x), code)]
            found += [(mm.group(2), mm.group(1)) for mm in
                      re.finditer(r"\*\s*\(\s*struct\s+(\w+)\s*\*\s*\*\s*\)\s*(\w+)\b", code)]
            for v, x in found:
                tags = set(re.findall(r"\bstruct\s+(\w+)\s*\*\s*%s\s*[;,=)]" % re.escape(v), code))
                if len(tags) == 1:
                    uses.setdefault(tags.pop(), set()).add(macros.get(x, x))
        from codebreaker.port import asm16
        lays = asm16.CTypes._layouts(list(self.originals.values()), 2, 4)
        sig = lambda lay: [(k, v[0], v[1]) for k, v in lay.items() if k != "@size"]
        for y, xs in list(uses.items()):                   # (struct isym4 { int s0; char kind, s3; int s4; }:
            if len(xs) != 1 or "struct " + y not in lays:   #  an isym's first fields - its link alike)
                continue
            whole = sig(lays["struct " + y])
            for k, lay in lays.items():
                z = k.split()[-1]
                part = sig(lay)
                if k.startswith("struct ") and z != y and z not in uses and part and \
                        len(part) < len(whole) and part == whole[:len(part)]:
                    uses[z] = set(xs)
        for y, xs in uses.items():
            if len(xs) != 1:
                continue
            x = next(iter(xs))
            for n in sorted(self.files):
                t = self.files[n]
                m = re.search(r"\bstruct\s+%s\s*\{\s*((?:unsigned\s+|signed\s+)?(?:int|short|unsigned|int16_t|"
                              r"uint16_t))\s+(\w+)\s*;" % re.escape(y), blank(t))
                if not m:
                    continue
                t = t[:m.start(1)] + "struct %s *%s;" % (x, m.group(2)) + \
                    "\t/* (the chain's link: NEXT) */" + t[m.end():]
                self.files[n] = t
                self.notes["a structure's first word its chain's link (read as a pointer): a pointer"].append(
                    "%s (struct %s: %s)" % (n, y, m.group(2)))

    def fnptr_returns(self):
        """f(t, s, fn) int (*fn)(); ... (*fn)() - and every call gives it a function of a pointer (char *f09581()):
        int was a pointer's width on the old machine, not here - the parameter a function of that pointer"""
        rets = {}                                           # (function: its return type, a pointer one)
        defs = {}                                           # ((function, index): (file, its declaration's span))
        for n, t in self.files.items():
            code = blank(t)
            for m in re.finditer(r"(?m)^(?:static\s+)?((?:struct\s+|unsigned\s+)?\w+)\s*(\*+)\s*(\w+)\s*"
                                 r"\(([\w\s,]*)\)\s*\n((?:[^{;]*;\s*)*)\{", code):
                rets.setdefault(m.group(3), set()).add(m.group(1) + " " + m.group(2))
            for m in re.finditer(r"(?m)^(?:\w+\s+\**\s*|\w+\s*\*+\s*)?(\w+)\s*\(([\w\s,]*)\)\s*\n"
                                 r"((?:[^{;]*;\s*)*)\{", code):
                names = [w.strip() for w in m.group(2).split(",") if w.strip()]
                for d in re.finditer(r"(?m)^[ \t]*(?:register\s+)?(?:int\s+)?\(\s*\*\s*(\w+)\s*\)\s*\(\s*\)\s*;",
                                     m.group(3)):
                    if d.group(1) in names:
                        defs[(m.group(1), names.index(d.group(1)))] = (n, m.start(3) + d.start(), m.start(3) + d.end(),
                                                                        d.group(1))
        rets = {f: next(iter(r)) for f, r in rets.items() if len(r) == 1}
        for (fn, i), (n, a, b, name) in sorted(defs.items(), key=lambda kv: -kv[1][1]):
            given = set()
            for ct in self.files.values():
                code = blank(ct)
                for cm in re.finditer(r"(?<![\w.>])%s\s*\(" % re.escape(fn), code):
                    if re.search(r"(?m)^[^\n]*$", code[:cm.start()]).group().strip() == "" and \
                            re.match(r"[\w\s,]*\)\s*\n", code[cm.end():]):
                        continue                            # (its definition)
                    ln = code[code.rfind("\n", 0, cm.start()) + 1:]
                    ln = ln[:ln.find("\n")] if "\n" in ln else ln
                    sp = call_args(ln, fn)
                    if sp and len(sp) == 1 and not ln[sp[0][0]:sp[0][1]].strip():
                        continue                            # (extern f(), a declaration)
                    if not sp or i >= len(sp):
                        given.add(None)
                        continue
                    arg = ln[sp[i][0]:sp[i][1]].strip().lstrip("&")
                    given.add(rets.get(arg) if re.fullmatch(r"\w+", arg) else None)
            if len(given) != 1 or None in given:
                continue
            rt = given.pop()
            t = self.files[n]
            t = t[:a] + re.sub(r"(?:int\s+)?\(\s*\*\s*%s" % re.escape(name), rt + "(*" + name, t[a:b], 1) + t[b:]
            self.files[n] = t
            self.notes["a function parameter given functions of pointers (int was a pointer's width): a function of "
                       "that pointer"].append("%s (%s of %s: %s)" % (n, name, fn, rt))

    def cast_twins(self):
        """struct opt { char *pat; int o2; ... } here, struct flagdef { char *pat; intptr_t *var; ... } there, the
        old bytes of both alike and the one cast to the other ((struct opt *)g_399a, a struct flagdef table): one
        structure under two names - the copy takes the other's types as the port has them (its names kept)"""
        from codebreaker.port import asm16
        lays = asm16.CTypes._layouts(list(self.originals.values()), 2, 4)
        sig = lambda lay: [(v[0], v[1]) for k, v in lay.items() if k != "@size"]
        where = {}
        for n, t in self.files.items():
            for m in re.finditer(r"\bstruct\s+(\w+)\s*\{", blank(t)):
                where.setdefault(m.group(1), []).append(n)
        def members(n, tag):
            t = self.files[n]
            m = re.search(r"\bstruct\s+%s\s*\{([^{}]*)\}" % re.escape(tag), blank(t))
            if not m:
                return None
            out = []
            for d in blank(t)[m.start(1):m.end(1)].split(";"):
                r = declarators(d.strip(), TYPEWORDS | STDINT) if d.strip() else None
                if r:
                    out += [(nm, re.sub(r"\s+", " ", r[0]).strip(), dcl) for nm, dcl in r[1]]
            return m, out
        params = {}                                         # ((function, index): (file, tag)) of struct T * ones
        glob = {}                                           # (variable: tag) of the files' outer struct ones
        for n, t in self.files.items():
            code = blank(t)
            for m in re.finditer(r"(?m)^(?:\w+\s+\**\s*|\w+\s*\*+\s*)?(\w+)\s*\(([\w\s,]*)\)\s*\n((?:[^{;]*;\s*)*)\{", code):
                names = [w.strip() for w in m.group(2).split(",") if w.strip()]
                for d in re.finditer(r"\bstruct\s+(\w+)\s*\*\s*(\w+)\s*[;,]", m.group(3)):
                    if d.group(2) in names:
                        params[(m.group(1), names.index(d.group(2)))] = (n, d.group(1))
            depth, outer = 0, []
            for c in code:                                  # (outside every function)
                depth += c == "{"
                depth -= c == "}"
                outer.append(c if depth == 0 or c in "{}" else " ")
            for d in re.finditer(r"\bstruct\s+(\w+)\s+\**\s*(\w+)\s*[\[;=,]", "".join(outer)):
                glob.setdefault(d.group(2), set()).add(d.group(1))
        fvars = collections.defaultdict(set)                # (a function: the function pointers given it)
        for t in self.files.values():
            code = blank(t)
            fps = set(re.findall(r"\(\s*\*\s*(\w+)\s*\)\s*\(\s*\)", code))
            for m in re.finditer(r"\(\s*\*\s*(\w+)\s*\)\s*\(\s*\)\s*=\s*(\w+)\s*[;,]", code):
                fvars[m.group(2)].add(m.group(1))
            for m in re.finditer(r"(?<![\w.>])(\w+)\s*=\s*&?\s*(\w+)\s*;", code):
                if m.group(1) in fps:
                    fvars[m.group(2)].add(m.group(1))

        def calls(ct, fn):                                  # (each call of fn - by its pointers too: the line,
            code = blank(ct)                                #  as a call of the name, and its arguments)
            for cm in re.finditer(r"(?<![\w.>])%s\s*\(" % re.escape(fn), code):
                ln = code[code.rfind("\n", 0, cm.start()) + 1:]
                ln = ln[:ln.find("\n")] if "\n" in ln else ln
                yield ln, call_args(ln, fn)
            for v in fvars.get(fn, ()):
                for cm in re.finditer(r"\(\s*\*\s*%s\s*\)\s*\(" % re.escape(v), code):
                    ln = code[code.rfind("\n", 0, cm.start()) + 1:]
                    ln = ln[:ln.find("\n")] if "\n" in ln else ln
                    ln = re.sub(r"\(\s*\*\s*%s\s*\)\s*\(" % re.escape(v), v + "(", ln)
                    yield ln, call_args(ln, v)

        def kinds(fname, var):
            m = re.findall(r"\bstruct\s+(\w+)\s*\*\s*%s\s*[;,=)]" % re.escape(var), blank(self.files[fname]))
            if len(set(m)) == 1:
                return m[0]
            g = glob.get(var, set())
            return next(iter(g)) if len(g) == 1 else None
        for n in sorted(self.files):
            if not n.endswith(".c"):
                continue
            t = self.files[n]
            code = blank(t)
            for xm in list(re.finditer(r"\bstruct\s+(\w+)\s*\{", code)):
                x = xm.group(1)
                if len(where.get(x, [])) != 1:
                    continue
                ys = set()
                for (fn, i), (pf, tag) in params.items():           # (a struct X * parameter given a Y, or
                    for cn, ct in self.files.items():               #  a struct Y * parameter given an X)
                        for ln, sp in calls(ct, fn):
                            if not sp or i >= len(sp):
                                continue
                            arg = ln[sp[i][0]:sp[i][1]].strip()
                            if not re.fullmatch(r"&?\w+", arg):
                                continue
                            at = kinds(cn, arg.lstrip("&"))
                            if tag == x and at and at != x and pf == n:
                                ys.add(at)
                            elif at == x and tag != x and cn == n:
                                ys.add(tag)
                prefix = lambda a_, b_: bool(a_) and bool(b_) and a_[:min(len(a_), len(b_))] == b_[:min(len(a_), len(b_))]   # (one the other's first words)
                ys = {y for y in ys if "struct " + y in lays and "struct " + x in lays and
                      prefix(sig(lays["struct " + y]), sig(lays["struct " + x])) and len(where.get(y, [])) >= 1}
                if len(ys) != 1:
                    continue
                y = ys.pop()
                mx = members(n, x)
                my = members(sorted(where[y], key=lambda w: not w.endswith(".h"))[0], y)
                if not mx or not my:
                    continue
                if all((a[1], a[2].replace(a[0], "@")) == (b[1], b[2].replace(b[0], "@")) for a, b in zip(mx[1], my[1])):
                    continue                                # (alike already)
                ptr = lambda d_: "*" in d_[2] or "*" in d_[1]
                if not any(ptr(a) != ptr(b) for a, b in zip(mx[1], my[1])):
                    continue                                # (no word a pointer in one, a number in the other)
                local_ok = lambda d_: not (re.match(r"(?:struct|union)\s+(\w+)$", d_[1]) and "*" not in d_[2] and
                                           not any(h.endswith(".h") for h in
                                                   where.get(d_[1].split()[-1], [])))
                if not all(local_ok(a) and local_ok(b) for a, b in zip(mx[1], my[1])):
                    continue                                # (a whole structure of one file's own: not moved)

                def merged(mine_, other, me_tag, other_tag):  # (each word: the pointer, of whichever has it)
                    return "\n".join(["    %s %s;" % (
                        (o[1].replace("struct " + other_tag, "struct " + me_tag), re.sub(r"\b%s\b" % re.escape(o[0]),
                                                                                         a[0], o[2]))
                        if ptr(o) and not ptr(a) else (a[1], a[2])) for a, o in zip(mine_, other)] +
                        ["    %s %s;" % (a[1], a[2]) for a in mine_[len(other):]])
                bx = merged(mx[1], my[1], x, y)
                by = merged(my[1], mx[1], y, x)
                if any(ptr(b) and not ptr(a) for a, b in zip(mx[1], my[1])):
                    m = mx[0]
                    t = t[:m.start(1)] + "\t/* (as struct %s is: one structure, two names) */\n%s\n" % (y, bx) + \
                        t[m.end(1):]
                    code = blank(t)
                    self.files[n] = t
                    self.notes["a structure that is another's, under its own name (cast to it, the old bytes alike): "
                               "its members of the other's types"].append("%s (struct %s as struct %s)" % (n, x, y))
                if any(ptr(a) and not ptr(b) for a, b in zip(mx[1], my[1])):
                    for w in where[y]:
                        my_ = members(w, y)
                        if not my_:
                            continue
                        tw = self.files[w]
                        tw = tw[:my_[0].start(1)] + "\t/* (as struct %s is: one structure, two names) */\n%s\n" % (
                            x, merged(my_[1], mx[1], y, x)) + tw[my_[0].end(1):]
                        self.files[w] = tw
                        self.notes["a structure that is another's, under its own name (cast to it, the old bytes "
                                   "alike): its members of the other's types"].append("%s (struct %s as struct %s)" % (
                                       w, y, x))

    def twin_structs(self):
        """struct macro { int f0, f2, f4; char *body; } in a file of its own - another structure's bytes (struct mac:
        its next, name, hash...) with ints where that one has pointers: its members of the other's types, its
        names kept"""
        from codebreaker.port import asm16
        lays = asm16.CTypes._layouts(list(self.originals.values()), 2, 4)
        hdr = {}
        for h, t in self.originals.items():
            if h.endswith(".h"):
                for m in re.finditer(r"\bstruct\s+(\w+)\s*\{", blank(t)):
                    hdr.setdefault("struct " + m.group(1), h)
        sig = lambda lay: [(v[0], v[1]) for k, v in lay.items() if k != "@size"]
        for n, t in list(self.files.items()):
            if not n.endswith(".c"):
                continue
            for m in re.finditer(r"\bstruct\s+(\w+)\s*\{([^{}]*)\}", blank(t)):
                x = "struct " + m.group(1)
                lx = lays.get(x)
                if not lx or x in hdr:
                    continue
                twins = [y for y, ly in lays.items() if y != x and y in hdr and len(sig(ly)) >= 3 and
                         sig(ly) == sig(lx) and re.search(r'#\s*include\s*"%s"' % re.escape(hdr[y]), t)]
                if len(twins) != 1:
                    continue
                y_tag = twins[0].split()[-1]                # (and the file takes one for the other: a function it
                evid = False                                #  declares giving an X that gives a Y, or a cast of a Y)
                for fm in re.finditer(r"\bstruct\s+%s\s*\*\s*(\w+)\s*\(\s*\)" % re.escape(m.group(1)), blank(t)):
                    for n2, t2 in self.originals.items():
                        if n2 != n and re.search(r"(?m)^\s*(?:static\s+)?struct\s+%s\s*\*\s*\n?\s*%s\s*\(" %
                                                 (re.escape(y_tag), re.escape(fm.group(1))), blank(t2)):
                            evid = True
                if not evid:
                    continue
                ly = lays[twins[0]]
                mx = [(k, v) for k, v in lx.items() if k != "@size"]
                my = [(k, v) for k, v in ly.items() if k != "@size"]
                if not any(not vx[4] and vy[4] for (kx, vx), (ky, vy) in zip(mx, my)):
                    continue                                # (no int where the other has a pointer: as it is)
                body = "\n".join("    %s %s;" % (vy[2], re.sub(r"\b%s\b" % re.escape(ky), kx, vy[3]))
                                  for (kx, vx), (ky, vy) in zip(mx, my))
                t = t[:m.start(2)] + "\t/* (as %s is: its bytes) */\n%s\n" % (twins[0], body) + t[m.end(2):]
                self.files[n] = t
                self.notes["a file's own structure that is another's bytes, ints where it has pointers: its "
                           "members of the other's types"].append("%s (%s as %s)" % (n, x, twins[0]))
                break

    def struct_views(self):
        """int top = 20; char *v[20]; - globals one after another that are a structure's members in order, the
        first's place taken (and used as the structure's): one variable of the structure, their names its
        members' (the old linker kept them in that order; today's need not)"""
        from codebreaker.port import asm16
        lays = asm16.CTypes._layouts(list(self.originals.values()), 2, 4)
        shapes = {}
        for st_, lay in lays.items():
            mem = [(k, v) for k, v in lay.items() if k != "@size"]
            if st_.startswith("struct ") and len(mem) >= 2 and any("[" in v[3] for k, v in mem):
                shapes[st_] = [(k, re.sub(r"\s+", " ", v[2]).strip(),
                                re.sub(r"\b%s\b" % re.escape(k), "@", v[3]).replace(" ", "")) for k, v in mem]
        if not shapes:
            return
        alltext = "\n".join(blank(t) for t in self.files.values())
        for n in sorted(self.files):
            if not n.endswith(".c"):
                continue
            t = self.files[n]
            gl = self._globals(t)
            lines = t.split("\n")
            done = set()
            for i in range(len(gl)):
                if i in done or not re.search(r"&\s*%s\b" % re.escape(gl[i][1]), alltext):
                    continue
                for st_, mem in shapes.items():
                    run = gl[i:i + len(mem)]
                    if len(run) != len(mem) or any(run[k][0] - run[k - 1][0] != 1 for k in range(1, len(run))):
                        continue
                    if any((g_[2], g_[3]) != (m_[1], m_[2]) for g_, m_ in zip(run, mem)):
                        continue
                    var = run[0][1] + "_s"
                    inits = [g_[4] if g_[4] is not None else ("{ 0 }" if "[" in g_[3] else "0") for g_ in run]
                    lines[run[0][0]] = "%s %s = { %s };\t/* (%s: one place, as the old linker kept them) */" % (
                        st_, var, ", ".join(inits), ", ".join(g_[1] for g_ in run))
                    for g_ in run[1:]:
                        lines[g_[0]] = ""
                    for g_, m_ in zip(run, mem):
                        self.aliases[g_[1]] = ("(%s.%s)" % (var, m_[0]), "%s %s;" % (st_, var))
                    self.view_vars = getattr(self, "view_vars", set()) | {g_[1] for g_ in run}
                    for h, ht in self.files.items():
                        if h.endswith(".h") and re.search(r"\b%s\s*\{" % re.escape(st_), blank(ht)):
                            self.view_headers.add(h)
                    self.notes["globals one after another that are a structure's members, the first's place "
                               "taken: one variable of it"].append("%s (%s: %s)" % (n, st_, ", ".join(
                                   g_[1] for g_ in run)))
                    done |= set(range(i, i + len(run)))
                    break
            self.files[n] = "\n".join(lines)

    def word_columns(self):
        """a table of structures with an int * member given (int *)&p - a word's place that held a pointer in
        some rows, a number in others: the member intptr_t *, the ints placed in it intptr_t"""
        from codebreaker.port import asm16
        lays = asm16.CTypes._layouts(list(self.originals.values()), 2, 4)
        ptrs, ints = set(), set()
        for t in self.files.values():
            for i, nm, base, shape, init in self._globals(t):
                if "*" in shape or base.startswith(("struct", "union")):
                    ptrs.add(nm)
                elif base in ("int", "unsigned", "short", "unsigned int") and shape == "@":
                    ints.add(nm)
        cols = {}
        for n, t in self.files.items():
            code = blank(t)
            for m in re.finditer(r"\bstruct\s+(\w+)\s+\w+\s*\[[^\]]*\]\s*=\s*\{", code):
                lay = lays.get("struct " + m.group(1))
                if not lay:
                    continue
                mem = [(k, v) for k, v in lay.items() if k != "@size"]
                d, k = 1, m.end()
                while k < len(code) and d:
                    d += {"{": 1, "}": -1}.get(code[k], 0)
                    k += 1
                for row in re.finditer(r"\{([^{}]*)\}", code[m.end():k - 1]):
                    els = [e.strip() for e in split_top(row.group(1))]
                    for c_, e in enumerate(els):
                        x = re.fullmatch(r"\(\s*(?:int|unsigned|short)\s*\*\s*\)\s*&\s*(\w+)", e)
                        if x and x.group(1) in ptrs and c_ < len(mem) and \
                                re.fullmatch(r"(?:int|unsigned|short|unsigned int)", mem[c_][1][2].strip()) and \
                                mem[c_][1][3].replace(" ", "") == "*" + mem[c_][0]:
                            cols.setdefault(m.group(1), set()).add(c_)
        if not cols:
            return
        widen = set()
        for n in list(self.files):
            t = self.files[n]
            code = blank(t)
            edits = []
            for m in re.finditer(r"\bstruct\s+(\w+)\s+\w+\s*\[[^\]]*\]\s*=\s*\{", code):
                if m.group(1) not in cols:
                    continue
                d, k = 1, m.end()
                while k < len(code) and d:
                    d += {"{": 1, "}": -1}.get(code[k], 0)
                    k += 1
                for row in re.finditer(r"\{([^{}]*)\}", code[m.end():k - 1]):
                    base_at = m.end() + row.start(1)
                    pos = 0
                    for c_, e in enumerate(split_top(row.group(1))):
                        at = base_at + pos + (len(e) - len(e.lstrip()))
                        pos += len(e) + 1
                        es = e.strip()
                        if c_ not in cols[m.group(1)]:
                            continue
                        x = re.fullmatch(r"\(\s*(?:int|unsigned|short)\s*\*\s*\)\s*&\s*(\w+)", es)
                        y = re.fullmatch(r"&\s*(\w+)", es)
                        z = re.match(r"\(\s*(?:int|unsigned|short)\s*\*\s*\)", es)
                        if x or (y and y.group(1) in getattr(self, "view_vars", set())):
                            edits.append((at, at + len(es), "(intptr_t *)&%s" % (x or y).group(1)))
                        elif z:                             # ((int *)table: another thing's place - so cast)
                            edits.append((at, at + z.end(), "(intptr_t *)"))
                        elif y and y.group(1) in ints:
                            widen.add(y.group(1))
            for a, b, new in reversed(edits):
                t = t[:a] + new + t[b:]
            self.files[n] = t
        for st_, cs in cols.items():
            lay = lays["struct " + st_]
            mem = [k for k in lay if k != "@size"]
            for n, t in self.files.items():
                sm = re.search(r"\bstruct\s+%s\s*\{([^{}]*)\}" % re.escape(st_), t)
                if not sm:
                    continue
                body = sm.group(1)
                for c_ in cs:
                    body = re.sub(r"(?m)^([ \t]*)(?:int|unsigned|short|unsigned int)\s*\*\s*%s\s*;" % re.escape(mem[c_]),
                                  r"\1intptr_t *%s;\t/* (a word's place - a pointer's in some rows) */" % mem[c_], body)
                self.files[n] = t[:sm.start(1)] + body + t[sm.end(1):]
                self.notes["a table's int * column given pointers' places too (a word was either): intptr_t *, "
                           "the ints placed in it intptr_t"].append("%s (struct %s: %s)" % (
                               n, st_, ", ".join(mem[c_] for c_ in sorted(cs))))
        for n, t in self.files.items():                     # (the ints: intptr_t, wherever declared)
            for v in widen:
                t = re.sub(r"(?m)^([ \t]*(?:extern\s+|static\s+)?)(?:int|unsigned|short|unsigned int)\s+([^;(){}]*)\b%s\b"
                           r"(\s*(?:=[^,;]*)?)\s*([,;])" % re.escape(v), lambda m: self._widen(m, v), t)
            self.files[n] = t
        if widen:
            self.notes["an int whose place a column of pointers' places is given: intptr_t"].append(
                ", ".join(sorted(widen)))

    @staticmethod
    def _widen(m, v):
        """int a, v = 1, b;  ->  intptr_t v = 1; int a, b;"""
        pre, before, init, end = m.group(1), m.group(2), m.group(3), m.group(4)
        rest = before.strip().rstrip(",").strip()
        own = "%sintptr_t %s%s;" % (pre, v, init)
        if end == ";" and not rest:
            return own
        tail = (rest + (", " if end == "," else "")) if rest else ""
        return own + " %sint %s" % (pre.strip() + " " if pre.strip() else "", tail) + ("" if end == "," else ";")

    def bss_ends(self, t, fname):
        """(char *)&g - the place of the variable the old linker put after a char array: the array's end, as the
        program means it (its bytes as they were)"""
        bss = getattr(self, "bss", {}) or {}
        if not bss:
            return t
        names = list(bss)
        chars = set()
        for o in self.originals.values():
            for m in re.finditer(r"\bchar\s+(\w+)\s*\[", blank(o)):
                chars.add(m.group(1))

        def one(m):
            v = m.group(1)
            if v not in bss or v in chars:
                return m.group(0)
            k = names.index(v)
            if k == 0 or names[k - 1] not in chars:
                return m.group(0)
            prev = names[k - 1]
            self.notes["the place of the variable after a char array (the old linker's order): the array's "
                       "end"].append("%s (&%s: %s + %d)" % (fname, v, prev, bss[prev]))
            used.add(prev)
            return "(%s + %d)" % (prev, bss[prev])
        used = set()
        t = sub_code(t, re.compile(r"\(\s*char\s*\*\s*\)\s*&\s*(\w+)\b(?!\s*(?:\[|->|\.))"), one)
        for prev in sorted(used):                           # (the array declared where its end is taken)
            if not re.search(r"\b%s\s*\[" % re.escape(prev), blank(t)):
                t = t.replace('#include "port.h"', '#include "port.h"\nextern char %s[];' % prev, 1)
        return t

    def copied_members(self):
        """the members the program copies from one to another as they are: x.val.i = y->val.i"""
        if not hasattr(self, "_copied"):
            self._copied = set()
            for t in self.originals.values():
                for m in re.finditer(r"\.\s*(\w+)\s*=\s*[\w.\->()\[\] ]*?\.\s*(\w+)\s*;", blank(t)):
                    if m.group(1) == m.group(2):
                        self._copied.add(m.group(1))
        return self._copied

    def ptr2_names(self):
        """the members and variables the program declares char ** (a block's link, its first word)"""
        if not hasattr(self, "_ptr2"):
            self._ptr2 = set()
            for t in self.originals.values():
                for m in re.finditer(r"\bchar\s*\*\s*\*\s*(\w+)\s*[;,=)\[]", blank(t)):
                    self._ptr2.add(m.group(1))
        return self._ptr2

    def pointee_of(self, text, var):
        """the structure a variable of the text points to (struct S *v, S v[]) - or None"""
        c = blank(text)
        m = re.search(r"\b((?:struct|union)\s+\w+)\s*\*\s*(?:\w+\s*,\s*\*?\s*)*%s\b\s*[;,)=\[]" % re.escape(var), c) or \
            re.search(r"\b((?:struct|union)\s+\w+)\s+%s\s*\[" % re.escape(var), c)
        return re.sub(r"\s+", " ", m.group(1)) if m else None

    def pass_through_params(self):
        """a pointer parameter a function only gives on (never through it, never a member of it) that every
        caller gives a pointer of another structure: it is that one's (the recovered source named some pointer -
        the bytes alike, any pointer two bytes)"""
        if getattr(self, "_pass_done", False):
            return
        self._pass_done = True
        texts = {n: t for n, t in self.files.items() if n.endswith(".c")}
        for fn in list(self.fns.values()):
            body = getattr(fn, "body", None) or ""
            if not body:
                src = texts.get(getattr(fn, "file", ""), "")
                d_ = next((d for d in function_map(src) if d[0] == fn.name), None) if src else None
                body = "\n".join(blank(src).split("\n")[d_[2]:d_[3] + 1]) if d_ else ""
            for k, (pn, pt) in enumerate(fn.params):
                m = re.fullmatch(r"\s*(?:register\s+)?((?:struct|union)\s+\w+)\s*\*\s*%s\s*" % re.escape(pn), pt)
                if not m or not body:
                    continue
                uses = list(re.finditer(r"\b%s\b" % re.escape(pn), body))
                if not uses or not all(re.search(r"[(,]\s*(?:\([^()]*\)\s*)?$", body[:u.start()]) and
                                       re.match(r"\s*[,)]", body[u.end():]) for u in uses):
                    continue                                # (used otherwise than given on: its own type)
                given = set()
                for n, t in texts.items():
                    code = blank(t)
                    for c in re.finditer(r"\b%s\s*\(" % re.escape(fn.name), code):
                        if re.match(r"[\w\s*]*\b%s\s*\([^;]*\)\s*$" % re.escape(fn.name),
                                    code[code.rfind("\n", 0, c.start()) + 1:code.find("\n", c.start())]):
                            continue                        # (its definition)
                        sp = call_args(code[c.start():c.start() + 400], fn.name) or []
                        if k >= len(sp):
                            continue
                        a_ = code[c.start() + sp[k][0]:c.start() + sp[k][1]].strip()
                        if not a_ or re.fullmatch(r"0|\(\s*\w+\s*\*\s*\)\s*0", a_):
                            continue                        # (none given; a declaration's f())
                        x = re.fullmatch(r"&?\s*(\w+)(?:\s*\[[^\]]*\])?", a_)
                        given.add(self.pointee_of(t, x.group(1)) if x else None)
                if len(given) == 1 and None not in given and given != {m.group(1)}:
                    y = given.pop()
                    self.param_types[(fn.name, k)] = "%s *%s" % (y, pn)
                    self.mended["a pointer parameter only given on, its callers all giving another structure's: "
                                "that one's (any pointer two bytes then)"].append("%s (%s: %s)" % (fn.name, pn, y))

    def offset_views(self, t, fname):
        """struct f071f5_p { long v; int x; unsigned char c; } *p, given (struct f071f5_p *)s->type - a file's own
        view of another structure (a struct type) by its old places: each member the other's at its place; a long
        over two of its members, given as an argument, the two (the old callee read them off the stack so)"""
        if not hasattr(self, "_views"):
            self._views = {}
            from codebreaker.port import asm16              # (the old layouts of the port's texts: tags it gave
            old = asm16.CTypes._layouts(                    #  anonymous ones known, headers first)
                [t_ for n_, t_ in sorted(self.files.items(), key=lambda x: not x[0].endswith(".h"))], 2, 4)
            ptr_mem = collections.defaultdict(set)          # (a member's name: the structures it points to)
            for st_, lay in old.items():
                for k, v in lay.items():
                    if k != "@size" and v[4] and str(v[2]).startswith(("struct", "union")):
                        ptr_mem[k].add(re.sub(r"\s+", " ", v[2]))
            local = {}
            for n, t_ in self.files.items():
                if n.endswith(".c"):
                    for m in re.finditer(r"\bstruct\s+(\w+)\s*\{", blank(t_)):
                        local.setdefault("struct " + m.group(1), n)
            exp_ = {}                                       # (#define L(s) ((struct lsym *)(s)): its uses the casts)
            for n_, t_ in self.files.items():
                b_ = blank(t_)
                for mm in re.finditer(r"(?m)^\s*#\s*define\s+(\w+)\s*\(\s*(\w+)\s*\)\s*\(\s*\(\s*((?:struct|union)"
                                      r"\s+\w+)\s*\*\s*\)\s*\(?\s*\2\s*\)?\s*\)\s*$", b_):
                    b_ = re.sub(r"\b%s\s*\(\s*(\w+)\s*\)" % re.escape(mm.group(1)),
                                lambda u, x_=mm.group(3): "((%s *)%s)" % (x_, u.group(1)), b_)
                exp_[n_] = b_
            alltext = "\n".join(exp_.values())
            for x, n in local.items():
                lx = old.get(x)
                if not lx:
                    continue
                ys = set()
                for m in re.finditer(r"\(\s*%s\s*\*\s*\)\s*([\w.\->\[\]]+)" % re.escape(x).replace(r"\ ", r"\s+"),
                                     alltext):
                    last = re.findall(r"\w+", m.group(1))
                    ys |= ptr_mem.get(last[-1], set()) if last else set()
                    if last and re.fullmatch(r"\w+", m.group(1)):   # (a variable's: as it is declared)
                        n_ = next((n__ for n__, t__ in exp_.items() if
                                   re.search(r"\(\s*%s\s*\*\s*\)\s*%s\b" % (re.escape(x).replace(r"\ ", r"\s+"),
                                                                           re.escape(m.group(1))), t__)), None)
                        py = None
                        for (fn_, pk), pt in self.param_types.items():
                            if pt.split("*")[-1].strip() == m.group(1) and self.fns.get(fn_) and \
                                    getattr(self.fns[fn_], "file", "") == n_:
                                py = re.sub(r"\s*\*.*", "", pt).strip()
                        py = py or (self.pointee_of(self.files[n_], m.group(1)) if n_ else None)
                        if py:
                            ys = {py}
                for fn in self.fns.values():                # (or what the callers give a parameter of it)
                    for k, (pn, pt) in enumerate(fn.params):
                        if re.search(r"%s\s*\*" % re.escape(x).replace(r"\ ", r"\s+"), pt):
                            for m in re.finditer(r"\b%s\s*\(" % re.escape(fn.name), alltext):
                                sp = call_args(alltext[m.start():m.start() + 400], fn.name) or []
                                if k < len(sp):
                                    a_ = alltext[m.start() + sp[k][0]:m.start() + sp[k][1]].strip()
                                    last = re.findall(r"\w+", a_)
                                    if last and re.fullmatch(r"\w+", a_):   # (a variable: as declared there, or
                                        pre = alltext[:m.start()]           #  as the loop typed a parameter)
                                        caller = next((d for d in reversed(re.findall(
                                            r"(?m)^[\w\s*]*?\b(\w+)\s*\([^;{]*\)\s*$", pre)) if d in self.fns), None)
                                        py = next((re.sub(r"\s*\*.*", "", pt).strip() for (fn_, pk), pt in
                                                   self.param_types.items() if fn_ == caller and
                                                   pt.split("*")[-1].strip() == a_), None)
                                        cf = getattr(self.fns.get(caller), "file", None) if caller else None
                                        py = py or (self.pointee_of(self.files[cf], a_) if cf in self.files else None)
                                        if py:
                                            ys.add(py)
                                            continue
                                    if last and re.fullmatch(r"[\w.\->\[\]]+", a_):
                                        ys |= ptr_mem.get(last[-1], set())
                ys = {y_ for y_ in ys if y_ not in local}   # (the other a header's: two files' own copies of one
                if len(ys) != 1:                            #  structure are each as it is)
                    continue
                y = ys.pop()
                ly = old.get(y)
                if not ly or y == x:
                    continue
                ymem = []                                   # (and the members of a structure in it: c.op)
                for k, v in ly.items():
                    if k == "@size":
                        continue
                    ymem.append((v[0], v[1], k, v))
                    sub = (old.get(re.sub(r"\s+", " ", str(v[2]))) or
                           (getattr(old, "inner", {}).get("%s.%s" % (y, k)) or (None, None))[1]) if not v[4] else None
                    if sub and "[" not in str(v[3]):
                        for k2, v2 in sub.items():
                            if k2 != "@size":
                                ymem.append((v[0] + v2[0], v2[1], "%s.%s" % (k, k2), v2))
                ymem.sort(key=lambda z: (z[0], -z[1]))
                mapping = {}
                for k, v in lx.items():
                    if k == "@size":
                        continue
                    lo, hi = v[0], v[0] + v[1]
                    same = [ym for ym in ymem if ym[0] == lo and ym[1] == v[1]]   # (one member just there: of its
                    if same:                                                       #  type the best)
                        best = next((ym for ym in same if str(ym[3][2]) == str(v[2]) and ym[3][4] == v[4]),
                                    next((ym for ym in same if ym[3][4] == v[4]), same[0]))
                        mapping[k] = [best[2]]
                        continue
                    top = [ym for ym in ymem if "." not in ym[2]]
                    cover = [ym for ym in top if lo <= ym[0] < hi]
                    ct_ = {"char": "char", "unsigned char": "unsigned char", "int": "int16_t", "short": "int16_t",
                           "unsigned": "uint16_t", "unsigned int": "uint16_t", "unsigned short": "uint16_t",
                           "long": "int32_t", "unsigned long": "uint32_t"}.get(re.sub(r"\s+", " ", str(v[2])).strip())
                    inside = [ym for ym in top if ym[0] <= lo and hi <= ym[0] + ym[1] and ym[1] > v[1]]
                    if inside and ct_ and not v[4] and y.startswith("struct "):   # (a union's all at 0: none)
                        mapping[k] = ("pun", inside[0][2], lo - inside[0][0], ct_)   # (a byte of a word of the
                        continue                                                     #  other's: that byte)
                    if not cover or cover[0][0] != lo or cover[-1][0] + cover[-1][1] != hi:
                        mapping = None
                        break
                    if len(cover) > 1 and v[1] <= 2 and ct_ and not v[4] and y.startswith("struct ") and \
                            all(not c[3][4] and "[" not in str(c[3][3]) for c in cover):
                        mapping[k] = ("pun", cover[0][2], 0, ct_)    # (a word over two bytes of the other's: the
                        continue                                     #  word there)
                    mapping[k] = [c[2] for c in cover]
                if mapping:
                    self._views[x] = (y, mapping, n)
        views = {x: v for x, v in self._views.items() if v[2] == fname}
        if not views:
            return t
        for x, (y, mapping, n) in views.items():
            code = blank(t)
            vars_ = set(re.findall(r"%s\s*\*\s*(\w+)" % re.escape(x).replace(r"\ ", r"\s+"), code))
            macs = [mm.group(1) for mm in re.finditer(r"(?m)^\s*#\s*define\s+(\w+)\s*\(\s*(\w+)\s*\)\s*\(\s*\(\s*%s"
                                                      r"\s*\*\s*\)\s*\(?\s*\2\s*\)?\s*\)\s*$" %
                                                      re.escape(x).replace(r"\ ", r"\s+"), code)]
            edits = []
            pats = []
            if vars_:
                pats.append(r"\b(?P<v>%s)\s*->\s*(?P<m>\w+)\b" % "|".join(map(re.escape, sorted(vars_))))
            if macs:                                        # (L(s)->num: the macro's cast, the other's member)
                pats.append(r"\b(?:%s)\s*\(\s*(?P<v>\w+)\s*\)\s*->\s*(?P<m>\w+)\b" % "|".join(map(re.escape, macs)))
            found_ = []
            for p_ in pats:
                found_ += [(m.start(), m.end(), m.group("v"), m.group("m")) for m in re.finditer(p_, code)]
            for st0, en0, var0, mem0 in sorted(found_):
                ms = mapping.get(mem0)
                if not ms:
                    continue
                pre = code[:st0].rstrip()
                post = code[en0:].lstrip()
                as_arg = pre.endswith(("(", ",")) and post.startswith((",", ")"))
                if isinstance(ms, tuple):                   # (a part of a member of the other's, or a word over two)
                    _, base_, off_, ct_ = ms
                    edits.append((st0, en0, "(*(%s *)((char *)&((%s *)%s)->%s + %d))" % (ct_, y, var0, base_, off_)))
                    continue
                if len(ms) == 1:
                    edits.append((st0, en0, "((%s *)%s)->%s" % (y, var0, ms[0])))
                elif as_arg:
                    edits.append((st0, en0, ", ".join("((%s *)%s)->%s" % (y, var0, w) for w in ms)))
            for a, b, new in sorted(edits, reverse=True):
                t = t[:a] + new + t[b:]
            if edits:
                hs_ = [h for h, t_ in self.files.items() if h.endswith(".h") and
                       re.search(r"\b%s\s*\{" % re.escape(y).replace(r"\ ", r"\s+"), blank(t_))]
                h_ = next((h for h in hs_ if os.path.splitext(h)[0] == y.split()[-1]), hs_[0] if hs_ else None)
                if h_:
                    self.need_headers.setdefault(fname, set()).add(h_)   # (the other's members: its header)
                self.notes["a file's own view of another structure by its old places: its members the other's (a "
                           "long over two of them, given, the two)"].append("%s (%s as %s)" % (fname, x, y))
        return t

    def bss_views(self):
        """f(&g_4e52) where f takes a struct type *, g_4e52 a variable of the BSS list and those after it at the
        structure's members' places (g_4e56 its t_code, g_4e58 its t_kind): the old linker's order made them one
        structure - one variable of it, their names its members'"""
        bss = getattr(self, "bss", {}) or {}
        if not bss or getattr(self, "_bss_views_done", False):
            return
        self._bss_views_done = True
        old = self.old_layouts()
        names = list(bss)
        offs, o = {}, 0                                     # (each one's place from the first: the sizes in order)
        for nm in names:
            offs[nm] = o
            o += bss[nm]
        found = {}
        for fn in list(self.fns.values()):
            ks = {k: re.sub(r"\s+", " ", x.group(1)) for k, (pn, pt) in enumerate(fn.params)
                  for x in [re.match(r"\s*(?:register\s+)?((?:struct|union)\s+\w+)\s*\*\s*\w+\s*$", pt)] if x}
            if not ks:
                continue
            for t in self.files.values():
                code = blank(t)
                for m in re.finditer(r"\b%s\s*\(" % re.escape(fn.name), code):
                    sp = call_args(code[m.start():], fn.name) or []
                    for k, st_ in ks.items():
                        if k < len(sp):
                            a = code[m.start() + sp[k][0]:m.start() + sp[k][1]].strip()
                            x = re.fullmatch(r"&\s*(\w+)", a)
                            if x and x.group(1) in bss and st_ in old and bss[x.group(1)] < old[st_]["@size"]:
                                found[x.group(1)] = st_
        for v, st_ in sorted(found.items()):
            lay = old[st_]
            mem = {val[0]: k for k, val in lay.items() if k != "@size"}
            k0 = names.index(v)
            run = []
            for nm in names[k0:]:
                rel = offs[nm] - offs[v]
                if rel >= lay["@size"]:
                    break
                if rel not in mem:
                    run = None
                    break
                run.append((nm, mem[rel]))
            if not run or any(nm in self.aliases for nm, m_ in run):
                continue
            var = v + "_s"
            for nm, m_ in run:
                self.aliases[nm] = ("(%s.%s)" % (var, m_), "%s %s;" % (st_, var))
            self.bss_struct = getattr(self, "bss_struct", {})
            self.bss_struct[var] = (st_, [nm for nm, m_ in run])
            for h, ht in self.files.items():
                if h.endswith(".h") and re.search(r"\b%s\s*\{" % re.escape(st_).replace(r"\ ", r"\s+"), blank(ht)):
                    self.view_headers.add(h)
            self.notes["variables of the BSS list one after another that a routine takes as a structure (its "
                       "members' places): one variable of it"].append("%s (%s: %s)" % (
                           var, st_, ", ".join("%s=.%s" % (nm, m_) for nm, m_ in run)))

    def allocators(self):
        """the program's functions that give blocks of memory: a call of one given a structure's whole old size
        somewhere ((struct inode *)f0a42b(2, 32)) - the evidence that a number given it is a size"""
        if not hasattr(self, "_allocs"):
            old = self.old_layouts()
            out = set()
            for t in self.originals.values():
                code = blank(t)
                ptr_of = {m.group(2): re.sub(r"\s+", " ", m.group(1)) for m in
                          re.finditer(r"\b((?:struct|union)\s+\w+)\s*\*\s*(\w+)\s*[;,)=]", code)}
                for m in re.finditer(r"(?:\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)|\b(\w+)\s*=)\s*"
                                     r"(?:\w+\s*\(\s*)?([A-Za-z_]\w*)\s*\(([^()]*)\)", code):
                    st_ = re.sub(r"\s+", " ", m.group(1)) if m.group(1) else ptr_of.get(m.group(2))
                    if not st_ or st_ not in old or m.group(3) in KEYWORDS:
                        continue
                    if any(re.fullmatch(r"\s*(?:0[xX][0-9a-fA-F]+|[1-9]\d*)\s*", a_) and
                           int(a_.strip(), 0) == old[st_]["@size"] for a_ in m.group(4).split(",")):
                        out.add(m.group(3))
            self._allocs = out
        return self._allocs

    def bss_decls(self):
        """{BSS name: (base type, declarator)} - what the program's externs say of each of its arrays"""
        bss = getattr(self, "bss", {}) or {}
        decl = {}
        for t in [blank(t) for n, t in self.originals.items() if n.endswith((".c", ".h"))]:
            ds = depths(t.split("\n"))
            for i, l in enumerate(t.split("\n")):
                st_ = l.strip()
                if ds[i] or not st_.startswith("extern") or not st_.endswith(";") or "(" in st_.replace("(*", ""):
                    continue
                r = declarators(re.sub(r"^extern\s+", "", st_).rstrip(";"))
                for nm, dcl in (r[1] if r else []):
                    if nm in bss and "[" in dcl:
                        decl.setdefault(nm, (re.sub(r"\s+", " ", r[0]).strip(), dcl))
        return decl

    def join_bss(self, var):
        """a BSS array the program went past the end of into the variable after it (in the old layout: the
        BSS list's order) - the two one array, the second's name a place in it, when they are arrays of one
        element type; -> whether they were"""
        bss = getattr(self, "bss", {}) or {}
        names = list(bss)
        joined = getattr(self, "bss_joined", {})
        head = next((h for h, rest in joined.items() if var in rest), var)
        chain = [head] + joined.get(head, [])
        if chain[-1] not in names or names.index(chain[-1]) + 1 >= len(names):
            return False
        nxt = names[names.index(chain[-1]) + 1]
        decls = self.bss_decls()
        a, b = decls.get(head), decls.get(nxt)

        def shape(d):                                       # (*v[64] and *w[64]: one shape)
            return re.sub(r"\[[^\]]*\]", "[]", re.sub(r"[A-Za-z_]\w*", "@", d[1], count=1))
        if not a or not b or a[0] != b[0] or shape(a) != shape(b) or nxt in getattr(self, "bss_struct", {}):
            return False
        self.bss_joined = joined
        joined[head] = joined.get(head, []) + [nxt] + joined.pop(nxt, [])
        self.bss_definitions()                              # (the places' names now: their externs dropped)
        n_ = re.search(r"\[(\d+)\]", self.aliases[nxt][1])
        if n_:                                              # (and the array's own externs of its new size)
            self.decl_rules.append((r"(\bextern\b[^;\n]*?\b%s\s*\[)\s*\d*\s*\]" % re.escape(head),
                                    r"\g<1>%s]" % n_.group(1)))
        return True

    def bss_definitions(self):
        """the definitions of the BSS list's variables no file of the program defines: the type its extern says,
        an array of as many elements as its old bytes hold"""
        bss = getattr(self, "bss", {}) or {}
        if not bss:
            return []
        old = self.old_layouts()
        texts = [blank(t) for n, t in self.originals.items() if n.endswith((".c", ".h"))]
        defined, decl = set(), {}
        for t in texts:
            ds = depths(t.split("\n"))
            for i, l in enumerate(t.split("\n")):
                st_ = l.strip()
                if ds[i] or not st_.endswith(";") or "(" in st_.replace("(*", ""):
                    continue
                ext = st_.startswith("extern")
                r = declarators(re.sub(r"=.*;$", ";", re.sub(r"^(?:extern|static)\s+", "", st_)).rstrip(";"))
                if not r:
                    continue
                for nm, dcl in r[1]:
                    if nm in bss:
                        if ext:                     # (extern int g in one file, char *g in another: the pointer)
                            was = decl.get(nm)
                            if was is None or (re.fullmatch(r"(?:unsigned\s+|signed\s+)?(?:int|short|unsigned)",
                                                            was[0]) and was[1] == nm and dcl != nm):
                                decl[nm] = (r[0].strip(), dcl)
                        else:
                            defined.add(nm)
        out = []
        members_ = set()
        for var, (st_, nms) in getattr(self, "bss_struct", {}).items():
            out.append("%s %s;\t/* (%s: one place, as the old linker kept them) */" % (st_, var, ", ".join(nms)))
            members_ |= set(nms)
        for nm, size in bss.items():
            if nm in defined or nm in self.profile.get("runtime_internals", ()) or nm.startswith("_") or \
                    nm in members_:
                continue
            joined = getattr(self, "bss_joined", {})
            if any(nm in rest for rest in joined.values()):
                continue                                    # (a place in the array before it: joined)
            parts = [nm] + joined.get(nm, [])
            size = sum(bss.get(p_, 0) for p_ in parts)
            base, dcl = decl.get(nm, ("char", "%s[]" % nm))
            if "[" not in dcl:
                out.append("%s %s;" % (base, dcl))
                continue
            el = re.sub(r"\[[^\]]*\]", "", dcl, count=1).replace(nm, "")
            inner = re.findall(r"\[\s*(\d+)\s*\]", dcl)[1:]
            esz = 2 if "*" in el else old[base]["@size"] if base in old else OLD_SCALARS.get(base, 2)
            for d_ in inner:
                esz *= int(d_)
            if len(parts) > 1:                              # (the names after it: places in it)
                at_ = 0
                full = "%s %s;" % (base, re.sub(r"\[\s*\]|\[[^\]]*\]", "[%d]" % max(1, size // max(1, esz)), dcl,
                                                count=1))
                for p_ in parts:
                    if p_ != nm:
                        self.aliases[p_] = ("(&%s[%d])" % (nm, at_ // max(1, esz)), full)
                    at_ += bss.get(p_, 0)
            room = 16 if base == "char" and esz == 1 else 0   # (bytes: a block's head may have grown)
            out.append("%s %s;\t/* (%d bytes then%s) */" % (base, re.sub(r"\[\s*\]|\[[^\]]*\]", "[%d]" % (max(
                1, size // max(1, esz)) + room), dcl, count=1), size, ", and room" if room else ""))
        return out

    def old_elem_sizes(self):
        """{global array: the bytes of one of its elements in the original} (its far pointers 4, near ones 2)"""
        if not hasattr(self, "_old_elems"):
            old, out = self.old_layouts(), {}
            for t in self.originals.values():
                code = blank(t)
                ds = depths(code.split("\n"))
                for i, l in enumerate(code.split("\n")):
                    st_ = l.strip()
                    if ds[i] or "[" not in st_ or not st_.endswith(";") or "(" in st_.replace("(*", ""):
                        continue
                    isfar = bool(re.search(r"\b(far|huge)\b", st_))
                    r = declarators(re.sub(r"\b(far|near|huge)\b", "", re.sub(r"=.*;$", ";", st_)).rstrip(";"))
                    if not r:
                        continue
                    base = re.sub(r"\s+", " ", re.sub(r"\b(extern|static|register|const)\b", "", r[0])).strip()
                    for nm, dcl in r[1]:
                        el = re.sub(r"\[[^\]]*\]", "", dcl, count=1)
                        if "*" in el.replace(nm, ""):
                            out.setdefault(nm, 4 if isfar else 2)
                        elif base.startswith(("struct ", "union ")) and base in old:
                            out.setdefault(nm, old[base]["@size"])
                        elif base in OLD_SCALARS:
                            out.setdefault(nm, OLD_SCALARS[base])
            self._old_elems = out
        return self._old_elems

    def old_var_sizes(self):
        """{global: its bytes in the original} of the program's arrays (far pointers 4 bytes, structures as laid out)"""
        if not hasattr(self, "_old_vars"):
            old, out = self.old_layouts(), {}
            for t in self.originals.values():
                code = blank(t)
                ds = depths(code.split("\n"))
                for i, l in enumerate(code.split("\n")):
                    st_ = l.strip()
                    if ds[i] or "[" not in st_ or "(" in st_ or not st_.endswith(";"):
                        continue
                    isfar = bool(re.search(r"\b(far|huge)\b", st_))
                    r = declarators(re.sub(r"\b(far|near|huge)\b", "", re.sub(r"=.*;$", ";", st_)).rstrip(";"))
                    if not r:
                        continue
                    for nm, dcl in r[1]:
                        if not re.search(r"\[\s*\d+\s*\]", dcl):
                            continue
                        sz = size_of(re.sub(r"\b(extern|static)\b", "", r[0]).strip(), dcl, nm,
                                     4 if isfar else 2, old)
                        if sz is None:
                            base = re.sub(r"\s+", " ", re.sub(r"\b(extern|static|register|const)\b", "", r[0])).strip()
                            unit = OLD_SCALARS.get(base)
                            cnt = 1
                            for x in re.findall(r"\[\s*(\d+)\s*\]", dcl):
                                cnt *= int(x)
                            sz = unit * cnt if unit and "*" not in dcl else None
                        if sz:
                            out.setdefault(nm, sz)
            self._old_vars = out
        return dict(self._old_vars, **getattr(self, "asm_sizes", {}))   # (and what the assembler's data placed)

    def old_layouts(self):
        """the structures as the old compiler laid them out (16-bit pointers, far ones 4 bytes)"""
        if not hasattr(self, "_old"):
            from codebreaker.port import asm16
            self._old = asm16.CTypes._layouts(list(self.originals.values()),
                                              self.profile.get("widths", {}).get("pointer", 16) // 8, 4)
        return self._old

    def old_layouts_for(self, fname):
        """the old layouts as a file sees them: the definitions of the headers it includes (in their order) before
        any other file's - two headers that define one tag two ways: the one it includes"""
        if not hasattr(self, "_twice_tags"):
            seen, twice = {}, set()
            for n, t in self.originals.items():
                for m in re.finditer(r"\b((?:struct|union)\s+\w+)\s*\{", blank(t)):
                    k_ = re.sub(r"\s+", " ", m.group(1))
                    if seen.setdefault(k_, n) != n:
                        twice.add(k_)
            self._twice_tags, self._lay_cache = twice, {}
        if not self._twice_tags:
            return self.old_layouts()
        order, todo = [], [fname]
        while todo:                                         # (its headers, depth first, in the order included)
            n = todo.pop(0)
            if n in order or n not in self.originals:
                continue
            order.append(n)
            todo = re.findall(r'#\s*include\s*"([^"]+)"', self.originals[n]) + todo
        key = tuple(order)
        if key not in self._lay_cache:
            from codebreaker.port import asm16
            texts = [self.originals[n] for n in reversed(order)] + \
                [t for n, t in self.originals.items() if n not in order]
            self._lay_cache[key] = asm16.CTypes._layouts(texts, self.profile.get("widths", {}).get("pointer", 16) // 8, 4)
        return self._lay_cache[key]

    def finish(self, fname, text):
        """the loop's rules: names that meet the C library's renamed, declarations given their real types, lines
        mended"""
        if self.renames:
            rx = re.compile(r"\b(%s)\b" % "|".join(map(re.escape, sorted(self.renames))))
            text = sub_code(text, rx, lambda m: self.renames[m.group(1)]
                            if not re.match(r"[ \t]*#\s*include\b", text[text.rfind("\n", 0, m.start()) + 1:m.start()])
                            else m.group(0))       # (#include <setjmp.h>: the header's own name)
        for rx, new in self.decl_rules:
            text = re.sub(rx, new, text)
        text = apply_local_retypes(text, fname, self.local_retypes)
        for rule_ in getattr(self, "np_rules", ()):         # (p[0]->op, p[0] a small number at times, or l->op, l
            f_, var_, typ_ = rule_[:3]                      #  a NULL: read through the null segment there, as on DOS)
            if f_ != fname:
                continue
            if rule_[3:] == ("var",):
                text = sub_code(text, re.compile(r"(?<![\w.>])(%s)(?=\s*->)" % re.escape(var_)),
                                lambda m: "((%s)dos_np(%s))" % (typ_, m.group(1)))
            else:
                text = sub_code(text, re.compile(r"(?<![\w.>])(%s\s*\[[^\[\]]*\])(?=\s*->)" % re.escape(var_)),
                                lambda m: "((%s)dos_np(%s))" % (typ_, m.group(1)))
        sizes = getattr(self, "array_sizes", {})
        if sizes:                                           # (extern T a[]; - its size, where it is defined)
            text = re.sub(r"(?m)^(\s*extern\b[^;(){}]*?\b(%s))\s*\[\s*\]" % "|".join(map(re.escape, sorted(sizes))),
                          lambda m: "%s[%d]" % (m.group(1), sizes[m.group(2)]), text)
        out, seen, origin = [], collections.Counter(), {}
        for i, l in enumerate(text.split("\n")):         # (a rule for the n-th line of that text: two alike
            k = seen[l]                                     #  lines in one file are two places)
            seen[l] += 1
            out.append(self.line_fixes.get((fname, l, k), self.line_fixes.get((fname, l), l)))
            origin[i + 1] = (l, k)
        self.line_origin[fname] = origin
        return "\n".join(out)

    def _preprocessor(self, text, fname):
        """#include of the old library's headers -> port.h; the program's own macros for what the C library
        has (putc...) dropped"""
        lib = set(self.profile["libc"])
        out, first = [], True
        for i, l in enumerate(text.split("\n")):
            m = re.match(r'\s*#\s*include\s*["<]([^">]+)[">]', l)
            if m and (m.group(1) in self.left_out or m.group(1).lower() in self.profile["runtime_headers"]):
                first = False                               # (port.h first of all, below: the program's own
                                                            #  headers, before the library's, need its types)
                self.notes["#include of the old library's header -> port.h"].append("%s:%d" % (fname, i + 1))
                continue
            m = re.match(r"\s*#\s*(?:undef|define)\s+(\w+)", l)
            if m and (m.group(1) in lib or m.group(1) == "FILE"):
                self.notes["macro of the old library's dropped (the C library of today's stands in)"].append(
                    "%s:%d" % (fname, i + 1))
                continue
            out.append(l)
        if fname.endswith(".c") and '#include "port.h"' not in out:
            out.insert(0, '#include "port.h"')
        return "\n".join(out)

    def _definitions(self, text, fname):
        lines = text.split("\n")
        code = blank(text).split("\n")
        defs = definitions(lines, code)
        for f, s, j, k in reversed(defs):
            g = self.fns.get(f.name, f)
            g.params, g.static = list(f.params), f.static
            g.implicit = getattr(f, "implicit", set())
            g.ret = g.ret if (getattr(g, "keep_int", False) or getattr(g, "ret_set", False)) and g.ret else f.ret
            for (fn_, pk), t in self.param_types.items():   # (what the loop found the parameters to be)
                if fn_ == f.name and pk < len(g.params):
                    g.params[pk] = (g.params[pk][0], t)
            body = "\n".join(code[j:k + 1])
            last = g.params[-1] if len(g.params) >= 2 else None
            if last and re.search(r"[(,]\s*%s&\s*%s\s*[,)]" % (ARGCAST, re.escape(last[0])), body) and \
                    re.fullmatch(r"(?:register\s+)?(?:int|short|unsigned|int16_t|intptr_t)\s+%s" % re.escape(last[0]),
                                 last[1].strip()) and re.search(r"\bchar\s*\*", g.params[-2][1]):
                # (the old way of a function of any number of arguments: the place of the first of them given on
                #  - they followed it on the stack, a word each; here they are put in words of their own)
                g.varargs = (last[0], g.params[-2][0])
                g.loose = False
                g.params = g.params[:-1]
                for x in range(j, k + 1):
                    rx_ = r"([(,]\s*%s)&\s*%s\b(?=\s*[,)])" % (ARGCAST, re.escape(last[0]))   # (an argument only)
                    if re.search(rx_, code[x]):
                        lines[x] = re.sub(rx_, lambda mm: mm.group(1) + last[0], lines[x])
                lines[j] = lines[j].rstrip() + " DOS_VA_WORDS(%s, %s);" % g.varargs
                self.notes["function of any number of arguments the old way (the place of the first given on): "
                           "..., its arguments put in words (DOS_VA_WORDS, by its format)"].append(
                    "%s:%d (%s)" % (fname, s + 1, f.name))
                body = "\n".join(blank("\n".join(lines)).split("\n")[j:k + 1])
            g.body = body
            valued = re.search(r"\breturn\b\s*[^\s;]", body)
            bare = re.findall(r"\breturn\s*;", body)
            if f.name == "main":
                g.ret = g.ret or "int"
            g.void = not valued and not g.ret and f.name != "main" and not getattr(g, "keep_int", False)
            if not valued and not bare and (g.ret or getattr(g, "keep_int", False)) and \
                    (g.ret or "int").strip() not in ("void", "static void") and f.name != "main":
                # (a function that gives a value, falling off its end: what the register held - the value of
                # its last assignment of a call's, when nothing after it gives the register another)
                stmts = [x for x in range(j + 1, k) if code[x].strip()]
                last = None
                tail = re.fullmatch(r"(\w+)\s*\([^;]*\)\s*;", code[stmts[-1]].strip()) if stmts else None
                callee = self.fns.get(tail.group(1)) if tail else None
                if callee and (callee.ret or "int").strip() not in ("", "void") and not callee.void and \
                        depths(code)[stmts[-1]] == depths(code)[j] + 1:
                    x = stmts[-1]                           # (its last statement a call of one that gives a value:
                    ind = re.match(r"\s*", lines[x]).group(0)   #  what that gave was in the register - so given)
                    lines[x] = ind + "return " + lines[x].lstrip()
                    self.notes["function giving a value that fell off its end after a call (the value in the "
                               "register): the call's value returned"].append("%s:%d (%s)" % (fname, x + 1, f.name))
                    stmts = []
                for x in reversed(stmts):
                    s_ = code[x].strip()
                    a_ = re.fullmatch(r"(\w+)\s*=\s*\w+\s*\([^;]*\)\s*;", s_)
                    if a_:
                        last = (x, a_.group(1))
                        break
                    if not re.fullmatch(r"(?:\+\+|--)?\s*[\w.\->\[\]]+\s*(?:\+\+|--)?\s*;|[\w.\->\[\]]+\s*=\s*"
                                        r"(?:\d+|0x[0-9a-fA-F]+)\s*;", s_):
                        break
                if last and re.search(r"\b%s\b" % last[1], "\n".join(code[j:last[0]])):
                    ind = re.match(r"\s*", lines[last[0]]).group(0)
                    lines[k] = ind + "return %s;\t\t/* (what the register held: the old function fell off its " \
                        "end) */\n" % last[1] + lines[k]
                    self.notes["function giving a value that fell off its end (the value in the register): its "
                               "last value returned"].append("%s:%d (%s)" % (fname, k + 1, f.name))
            if valued and (g.ret or "int").strip() not in ("void", "static void") and f.name != "main":
                # (a function that gives a value but whose last statement, an if-else, has a branch falling off
                # its end after x = f(...): what the register held then was that value - given)
                dep, at_ = 0, []
                for x in range(j, k + 1):
                    at_.append(dep)
                    dep += code[x].count("{") - code[x].count("}")
                D = at_[1] if len(at_) > 1 else 1           # (the body's depth)
                body_ = [x for x in range(j + 1, k) if code[x].strip()]
                closes = [x for x in body_ if code[x].strip().startswith("}") and at_[x - j] == D + 1 and
                          re.fullmatch(r"\}\s*(?:else\b.*)?", code[x].strip())]
                if closes and closes[-1] == body_[-1] and \
                        re.match(r"\s*(?:if|else)\b", "\n".join(code[x] for x in body_ if at_[x - j] == D)
                                 .split("\n")[-1] if any(at_[x - j] == D for x in body_) else "") and \
                        any(re.search(r"\belse\b", code[c_]) for c_ in closes):
                    for c_ in closes:
                        prev = max((x for x in body_ if x < c_ and code[x].strip()), default=None)
                        if prev is None or at_[prev - j] != D + 1:
                            continue
                        s_ = code[prev].strip()
                        if re.fullmatch(r"[\w.\->\[\]]+\s*=\s*\w+\s*\([^;]*\)\s*;", s_) and \
                                not s_.startswith("return"):
                            ind = re.match(r"\s*", lines[prev]).group(0)
                            lines[prev] = ind + "return " + lines[prev].lstrip()
                            self.notes["function giving a value whose branch fell off its end after an assignment "
                                       "of a call (the value in the register): that value returned"].append(
                                "%s:%d (%s)" % (fname, prev + 1, f.name))
            if bare and (valued or not g.void) and (g.ret or "int").strip() != "void":   # (or an int to its callers)
                g.bare = len(bare)
                for x in range(j, k + 1):
                    if re.search(r"\breturn\s*;", code[x]):
                        lines[x] = re.sub(r"\breturn\s*;", "return 0;", lines[x])
                        self.notes["bare return in a function that gives a value elsewhere: return 0 (what the "
                                   "register held is not known)"].append("%s:%d" % (fname, x + 1))
            if g.loose:                                     # (the old-style definition kept - its calls unchecked:
                if not g.ret:                               #  what it gives as its callers say - an int when it
                    g.ret = "void" if g.void else "int"     #  never said - and its integers
                if g.ret:
                    head = code[s] if s == j - 1 - (j - s - 1) else code[s]   # whole enough for a pointer)
                    nm = re.search(r"\b%s\s*\(" % re.escape(g.name), lines[s])
                    if nm:
                        lines[s] = ("static " if g.static else "") + g.ret + \
                            ("" if g.ret.endswith("*") else " ") + lines[s][nm.start():]
                    elif s + 1 < len(lines) and re.search(r"\b%s\s*\(" % re.escape(g.name), lines[s + 1]):
                        nm2 = re.search(r"\b%s\s*\(" % re.escape(g.name), lines[s + 1])
                        lines[s] = ""                       # (the type on the line before: the head on one line)
                        lines[s + 1] = ("static " if g.static else "") + g.ret + \
                            ("" if g.ret.endswith("*") else " ") + lines[s + 1][nm2.start():]
                for x in range(s + 1, j):
                    if lines[x].strip().endswith(";"):
                        lines[x] = re.sub(r"\b(?:unsigned\s+int|unsigned(?!\s+(?:char|long|short))|short|int)\b"
                                          r"(?!\s*\*)", "intptr_t", lines[x])
                undecl = [p_ for p_, t_ in g.params if p_ in getattr(g, "implicit", set())]
                if undecl and not lines[j].lstrip().startswith("intptr_t"):   # (never declared: old C's int -
                    lines[j] = "intptr_t %s; %s" % (", ".join(undecl), lines[j].lstrip())   # here intptr_t)
                self.notes["old-style definition kept: its integer parameters intptr_t (the callers may give "
                           "pointers)"].append("%s:%d (%s)" % (fname, s + 1, g.name))
                continue
            self.notes["old-style definition -> prototype"].append("%s:%d" % (fname, s + 1))
            lines[s:j] = [g.proto()]
        statics = [self.fns[f.name] for f, s, j, k in defs if f.static]
        if statics:                                         # (the file's own functions: declared at its head)
            at = max([i for i, l in enumerate(lines) if l.startswith("#include")] + [-1]) + 1
            lines[at:at] = [g.proto() + ";" for g in statics]
        return "\n".join(lines)

    def headers(self):
        """port.h, proto.h and data.c"""
        tags = sorted({t for f in self.fns.values() for n, p in f.params for t in re.findall(r"\b(?:struct|union)\s+\w+", p)} |
                      {t for f in self.fns.values() for t in re.findall(r"\b(?:struct|union)\s+\w+", f.ret or "")})
        whole = set()                                       # (T (*a)[3]: an array of T - T must be complete)
        for f in self.fns.values():
            for n, p in f.params:
                for x in re.finditer(r"\b((?:struct|union)\s+\w+)\s*\(\s*\*\s*\w+\s*\)\s*\[", array_param(p, n)):
                    whole.add(re.sub(r"\s+", " ", x.group(1)))
        incs = []
        for t in sorted(whole):
            h = next((h for h, ht in self.files.items() if h.endswith(".h") and
                      re.search(r"\b%s\s*\{" % re.escape(t).replace(r"\ ", r"\s+"), blank(ht))), None)
            if h and h not in incs:
                incs.append(h)
        proto = ["/* proto.h -- every function of the program, declared (made by CodeBreaker's port) */",
                 "#ifndef PROTO_H", "#define PROTO_H"] + ['#include "%s"' % h for h in incs] + \
            ["%s;" % t for t in tags] + [""]
        for f in sorted(self.fns.values(), key=lambda f: (f.file, f.line)):
            if not f.static:
                proto.append(re.sub(r"\bregister\s+", "", f.proto()) + ";")
        if self.asm_protos:                                 # (the program's own assembly routines, called old-style:
            proto += ["", "/* the program's routines in assembly language (their *64.asm) */"]   # their arguments
            proto += [v for k, v in sorted(self.asm_protos.items())]                          # pushed as they were)
        proto += ["", "#endif"]
        ptext = "\n".join(proto)
        ptext = widths(ptext, blank(ptext), collections.defaultdict(list), "proto.h", self.int16)[0]
        port = ["/* port.h -- what every file of the port includes first (made by CodeBreaker's port) */",
                "#ifndef PORT_H", "#define PORT_H", "#include <stdio.h>", "#include <stdlib.h>",
                "#include <string.h>", "#include <stdint.h>", "#include <ctype.h>", "#include <signal.h>",
                "#include <errno.h>", "#include <time.h>",
                '#include "%s"' % self.profile["platform"]["header"]]
        align = self.profile.get("struct_align")
        if align:                                           # (the program's structures laid out as its compiler
            port += ["/* the program's structures laid out as its compiler did: members on %d-byte boundaries "
                     "at most */" % align, "#pragma pack(push, %d)" % align]   # did - the system's headers above)
        for h in sorted(getattr(self, "view_headers", ())):  # (a structure some names are members of: known here)
            port.append('#include "%s"' % h)
        if self.aliases:                                    # (variables the old linker gave one place: the names of
            owners = {}                                     #  its elements are the elements)
            for al, (where, od) in sorted(self.aliases.items()):
                owners.setdefault(re.search(r"[A-Za-z_]\w*", where).group(0), od)
            port += ["", "/* the names the old linker gave the places of an array's elements */"]
            port += [widths("extern %s" % od, blank("extern %s" % od), collections.defaultdict(list), "port.h",
                            self.int16)[0] for o, od in sorted(owners.items()) if od]
            port += ["#define %s\t%s" % (al, where) for al, (where, od) in sorted(self.aliases.items(),
                                                                                  key=lambda x: x[1][0])]
        port += ['#include "proto.h"', "#endif"]
        def tags_of(h, seen=None):                          # (the tags a header defines, itself and through its
            seen = set() if seen is None else seen          #  includes)
            if h in seen or h not in self.files:
                return set()
            seen.add(h)
            t = self.files[h]
            out_ = set(re.findall(r"\b((?:struct|union)\s+\w+)\s*\{", blank(t)))
            for i_ in re.findall(r'#\s*include\s*"([^"]+)"', t):
                out_ |= tags_of(i_, seen)
            return {re.sub(r"\s+", " ", x) for x in out_}
        groups = []                                         # (headers that define one tag two ways: not in one
        for h in sorted({h for h, d in self.data if h.endswith(".h")}):   # file - a file each such way)
            g_ = next((g_ for g_ in groups if not (tags_of(h) & set().union(*[tags_of(x) for x in g_]) -
                                                  set().union(*[{t_ for t_ in tags_of(x) if t_ in tags_of(h) and
                                                                 self._same_def(t_, x, h)} for x in g_]))), None)
            if g_ is None:
                groups.append([h])
            else:
                g_.append(h)
        if self.asm_data or not groups:
            groups = [sorted({h for h, d in self.data if h.endswith(".h")} |
                             ({h for h in self.files if h.endswith(".h")} if self.asm_data else set()))]
        datas = []
        for k_, g_ in enumerate(groups):
            name = "data.c" if k_ == 0 else "data%d.c" % (k_ + 1)
            datas.append((name, ["/* %s -- the variables the headers share, defined once (made by CodeBreaker's "
                                 "port) */" % name, '#include "port.h"'] + ['#include "%s"' % h for h in g_] + [""] +
                          [d for h, d in self.data if h in g_ or (k_ == 0 and h not in set().union(*groups))] +
                          (["", "/* the variables an assembler module of data alone placed */"] +
                           [d for h, d in self.asm_data] if self.asm_data and k_ == 0 else [])))
        data = datas[0][1]
        bss = self.bss_definitions()

        def defining(u):                                    # (the header that defines a structure - of two that
            hs_ = [h for h, t in self.files.items() if h.endswith(".h") and   # do, the one of its name)
                   re.search(r"\b%s\s*\{" % re.escape(u).replace(r"\ ", r"\s+"), blank(t))]
            return next((h for h in hs_ if os.path.splitext(h)[0] == u.split()[-1]), hs_[0] if hs_ else None)

        def add(h, seen):                                   # (and first what its members' structures need: by
            if h in seen:                                   #  value - a complete type)
                return
            seen.add(h)
            body = blank(self.files[h])
            for u in re.findall(r"\b((?:struct|union)\s+\w+)\s+\w+\s*(?:\[[^\]]*\])?\s*;", body):
                d_ = defining(re.sub(r"\s+", " ", u))
                if d_ and d_ != h:
                    add(d_, seen)
            bdata.append('#include "%s"' % h)
        bdata, seen_ = ["/* bss.c -- the variables the old build defined beginning without a value (its BSS list), "
                        "as the program declares them (made by CodeBreaker's port) */", '#include "port.h"'], set()
        for u in sorted(set(re.findall(r"\b((?:struct|union)\s+\w+)\s+\w+\s*[\[;]", "\n".join(bss)))):
            h = defining(re.sub(r"\s+", " ", u))           # (a structure of its own, not a pointer: its header)
            if h:
                add(h, seen_)
        btext = "\n".join(bdata + [""] + bss) + "\n"
        btext = widths(btext, blank(btext), collections.defaultdict(list), "bss.c", self.int16)[0]
        res = {"port.h": self.finish("port.h", "\n".join(port) + "\n"), "proto.h": self.finish("proto.h", ptext + "\n"),
               "bss.c": self.finish("bss.c", btext) if bss else None}
        for name, lines_ in datas:
            dtext = "\n".join(lines_) + "\n"
            dtext = widths(dtext, blank(dtext), collections.defaultdict(list), name, self.int16)[0]
            res[name] = self.finish(name, dtext) if self.data or self.asm_data else None
        return res

    def _same_def(self, tag, h1, h2):
        """whether two headers define a tag alike (one header including the other: the same definition)"""
        def where(h, seen=None):
            seen = set() if seen is None else seen
            if h in seen or h not in self.files:
                return None
            seen.add(h)
            if re.search(r"\b%s\s*\{" % re.escape(tag).replace(r"\ ", r"\s+"), blank(self.files[h])):
                return h
            for i_ in re.findall(r'#\s*include\s*"([^"]+)"', self.files[h]):
                w_ = where(i_, seen)
                if w_:
                    return w_
            return None
        return where(h1) == where(h2)


# ---------------------------------------------------------------- the frame, where the program counts on it

SIZES = {"char": 1, "signed char": 1, "unsigned char": 1, "int16_t": 2, "uint16_t": 2, "int32_t": 4, "uint32_t": 4,
         "int": 2, "unsigned": 2, "short": 2, "long": 4, "float": 4, "double": 8, "unsigned int": 2,
         "unsigned short": 2, "short int": 2, "unsigned long": 4, "long int": 4, "unsigned long int": 4,
         "signed int": 2, "signed long": 4, "int8_t": 1, "uint8_t": 1}


OLD_SCALARS = {"char": 1, "signed char": 1, "unsigned char": 1, "short": 2, "short int": 2, "unsigned short": 2,
               "int": 2, "unsigned": 2, "unsigned int": 2, "signed": 2, "long": 4, "long int": 4, "unsigned long": 4,
               "unsigned long int": 4, "float": 4, "double": 8}


def split_view_lists(text):
    """in a union's structures: char kind, flags; - one member a line (each can be given its own new place)"""
    code = blank(text)
    out, last = [], 0
    for um in re.finditer(r"\bunion\s*\w*\s*\{", code):
        d, k = 0, um.end() - 1
        while k < len(code):
            d += {"{": 1, "}": -1}.get(code[k], 0)
            if d == 0:
                break
            k += 1
        for vm in re.finditer(r"\bstruct\s*\{([^{}]*)\}", code[um.end():k]):
            a0 = um.end() + vm.start(1)
            for st in re.finditer(r"[^;]*;", code[a0:um.end() + vm.end(1)]):
                r = declarators(st.group(0).strip().rstrip(";"))
                if not r or len(r[1]) < 2 or "(" in st.group(0):
                    continue
                a, b = a0 + st.start(), a0 + st.end()
                lead = text[a:b][:len(text[a:b]) - len(text[a:b].lstrip())]
                ind = lead.split("\n")[-1] if "\n" in lead else " " * 8
                out.append((a, b, lead + ("\n" + ind).join("%s %s;" % (r[0].strip(), dc) for nm, dc in r[1])))
    for a, b, new in sorted(out, reverse=True):
        text = text[:a] + new + text[b:]
    return text


def union_views(text, notes, fname, sizes=None):
    """a union whose views (its structures) meet at the same old places - prc.len where lab.scope was, the bytes
    of a char f1c[6] over another's members: the views laid out again, each old place one new place in all of
    them (padding where a view has nothing there), so that what one view writes another reads where it did; its
    plain members (a pointer, a long) views of their own - they say where the next old place may be now"""
    text = split_view_lists(text)
    sizes = sizes or {}
    code = blank(text)
    edits = []
    for um in re.finditer(r"\bunion\s*\w*\s*\{", code):
        d, k = 0, um.end() - 1
        while k < len(code):
            d += {"{": 1, "}": -1}.get(code[k], 0)
            if d == 0:
                break
            k += 1
        body_at = um.end()
        views = []                                          # ([member: (decl start, decl end, old off, old size,
        for vm in re.finditer(r"\bstruct\s*\{", code[body_at:k]):   # new size, name, is a byte array)])
            vs = body_at + vm.end()
            d2, e = 1, vs
            while e < k and d2:
                d2 += {"{": 1, "}": -1}.get(code[e], 0)
                e += 1
            members, off = [], 0
            pos = vs
            for stmt in re.finditer(r"[^;{}]*;", code[vs:e - 1]):
                s = stmt.group(0)
                if not s.strip(";").strip():
                    continue
                isfar = bool(re.search(r"\b(far|huge)\b", s))
                r = declarators(re.sub(r"\b(far|near|huge)\b", "", s).strip())
                if not r or len(r[1]) != 1:
                    members = None
                    break
                n, dcl = r[1][0]
                old_sz = size_of(r[0], dcl, n, 4 if isfar else 2, {})
                if old_sz is None:
                    unit = OLD_SCALARS.get(re.sub(r"\s+", " ", re.sub(r"\b(register|const|volatile)\b", "",
                                                                         r[0])).strip())
                    cnt_ = 1
                    for x in re.findall(r"\[\s*(\d+)\s*\]", dcl):
                        cnt_ *= int(x)
                    old_sz = unit * cnt_ if unit else None
                if old_sz is None:
                    members = None
                    break
                ptr = "*" in dcl.replace(n, "", 1)
                cnt = 1
                for x in re.findall(r"\[\s*(\d+)\s*\]", dcl):
                    cnt *= int(x)
                new_sz = 8 * cnt if ptr else old_sz
                if old_sz > 1 and off % 2:
                    off += 1
                bytes_ = bool(re.fullmatch(r"(?:unsigned\s+|signed\s+)?char", r[0].strip()) and "[" in dcl and
                              not ptr)
                members.append([vs + stmt.start(), vs + stmt.end(), off, old_sz, new_sz, n, bytes_])
                off += old_sz
            if members is None:
                views = None
                break
            views.append((vs, e - 1, members))
        if views:                                           # (its plain members: one-member views at its start)
            inner = code[body_at:k]
            flat = re.sub(r"\bstruct\s*\w*\s*\{[^{}]*\}[^;]*;", lambda x: " " * len(x.group(0)), inner)
            for st in re.finditer(r"[^;{}]*;", flat):
                r = declarators(st.group(0).strip().rstrip(";"))
                if not r or len(r[1]) != 1 or not st.group(0).strip().rstrip(";").strip():
                    continue
                n, dcl = r[1][0]
                base = re.sub(r"\s+", " ", r[0]).strip()
                ptr = "*" in dcl.replace(n, "", 1)
                if ptr:
                    old_sz, new_sz = (4 if re.search(r"\b(far|huge)\b", st.group(0)) else 2), 8
                elif base in sizes:
                    old_sz, new_sz = sizes[base]
                else:
                    old_sz = OLD_SCALARS.get(base)
                    new_sz = old_sz
                if not old_sz or "[" in dcl:
                    continue
                views.append((None, None, [[None, None, 0, old_sz, new_sz, n, False]]))
        if not views or len(views) < 2 or not any(m[4] != m[3] for v in views for m in v[2]):
            continue
        bounds = sorted({0} | {m[2] for v in views for m in v[2]} | {m[2] + m[3] for v in views for m in v[2]})
        new, prev = {}, None
        for b in bounds:                                    # (each old place: the least new one all views allow)
            n_ = 0 if prev is None else new[prev] + (b - prev)
            for v in views:
                for m in v[2]:
                    if m[2] + m[3] == b and not m[6]:
                        n_ = max(n_, new[m[2]] + m[4])
            new[b] = n_ + (n_ % 2 if b % 2 == 0 else 0)
            prev = b
        changed = False
        for vs, ve, members in views:
            if vs is None:                                  # (a plain member: at the start, as it is)
                continue
            cur, pad = 0, 0
            for m in members:
                at = new[m[2]]
                ds = m[0] + (len(code[m[0]:m[1]]) - len(code[m[0]:m[1]].lstrip()))   # (the declaration itself)
                ls = text.rfind("\n", 0, ds) + 1
                indent = text[ls:ds] if not text[ls:ds].strip() else " " * 12
                if at > cur:                                # (a line of its own before it: the room)
                    edits.append((ls, ls, "%schar _old%d[%d];\t\t/* (where the other views have members) */\n" % (
                        indent, pad, at - cur)))
                    pad += 1
                    changed = True
                if m[6]:                                    # (a byte array over other views' members: as long)
                    size = new[m[2] + m[3]] - at
                    stmt = text[ds:m[1]]
                    new_stmt = re.sub(r"\[\s*\d+\s*\]", "[%d]" % size, stmt, count=1)
                    if new_stmt != stmt:
                        edits.append((ds, m[1], new_stmt))
                        changed = True
                    cur = at + size
                else:
                    cur = at + m[4]
        if changed:
            notes["a union whose views meet at the same old places: laid out again, each old place one new place "
                  "in all of them"].append(fname)
    for a, b, new_text in sorted(edits, reverse=True):
        text = text[:a] + new_text + text[b:]
    return text


def c_string_bytes(lit):
    """the bytes of a C string literal ("a\\0b" "c"): as the compiler made them, the 0 after left out"""
    out = b""
    for part in re.findall(r'"((?:[^"\\]|\\.)*)"', lit):
        out += part.encode("latin1").decode("unicode_escape").encode("latin1")
    return out


def size_of(base, decl, name, ptr, structs):
    """the size a declaration had in the original (its pointers ptr bytes) - or None"""
    d = decl.replace(name, "", 1)
    n = 1
    for x in re.findall(r"\[\s*(\d+)\s*\]", d):
        n *= int(x)
    if "*" in d or "(" in d:
        return ptr * n
    b = re.sub(r"\s+", " ", re.sub(r"\b(?:register|static|extern|auto|const|volatile)\s+", "", base)).strip()
    if b in SIZES:
        return SIZES[b] * n
    if b in structs:
        return structs[b]["@size"] * n
    return None


def layouts(texts, ptr=2):
    """{"struct S": {member: (offset, size), "@size": n}} of every structure, laid out as the 8086 compilers did
    (a word on an even offset; a nested struct or union one member)"""
    from codebreaker.port import asm16
    full = asm16.CTypes._layouts(texts, ptr, ptr)
    return {k: {m: (v if m == "@size" else v[:2]) for m, v in lay.items()} for k, lay in full.items()}

def frames(port, out):
    """a local array the program reads past the end of - given to a function that takes its pointer for a
    structure's (a name buffer read as a struct whose next member is the frame's next variable): the locals it
    reaches, in a structure of their own, side by side as the original's frame had them"""
    lay = layouts(out.values(), port.profile.get("widths", {}).get("pointer", 2) // 8 or 2)
    owner = collections.defaultdict(list)
    for s, ms in lay.items():
        for m in ms:
            if m != "@size":
                owner[m].append(s)
    need = {}                                               # ((function, parameter index): bytes it reaches)
    for f in port.fns.values():
        body = getattr(f, "body", "")
        for i, (p, decl) in enumerate(f.params):
            if "*" not in decl:
                continue
            own = re.search(r"\b(struct \w+)\s*\*", decl)
            e = 0
            for m in re.finditer(r"(?:\(\s*\(\s*(struct \w+)\s*\*\s*\)\s*%s\s*\)|\b%s)\s*->\s*(\w+)" % (p, p), body):
                s = m.group(1) or (own.group(1) if own and own.group(1) in lay else None) or \
                    (owner[m.group(2)][0] if owner.get(m.group(2)) else None)
                if s in lay and m.group(2) in lay[s]:
                    o, sz = lay[s][m.group(2)]
                    e = max(e, o + sz)
            if e:
                need[(f.name, i)] = e
    for _ in range(8):                                      # (through the functions that hand the pointer on)
        grew = False
        for f in port.fns.values():
            body = getattr(f, "body", "")
            for i, (p, decl) in enumerate(f.params):
                for (g, k), e in list(need.items()):
                    for spans in _calls(body, g):
                        if k < len(spans) and body[spans[k][0]:spans[k][1]].strip() == p and need.get((f.name, i), 0) < e:
                            need[(f.name, i)] = e
                            grew = True
        if not grew:
            break
    if not need:
        return
    for fname, text in list(out.items()):
        if not fname.endswith(".c"):
            continue
        lines = text.split("\n")
        code = blank(text).split("\n")
        for fn, s, j, k in reversed(definitions_any(lines, code)):
            body = "\n".join(code[j:k + 1])
            locs = _locals(lines, code, j, k)
            arrays = {n: (sz, at) for n, (base, dcl, sz, at, reg) in locs.items() if "[" in dcl and sz}
            for (g, idx), e in need.items():
                for spans in _calls(body, g):
                    if idx >= len(spans):
                        continue
                    a = body[spans[idx][0]:spans[idx][1]].strip()
                    if a not in arrays or arrays[a][0] >= e:
                        continue
                    _overlay(port, fname, lines, code, j, k, locs, a, e, g)
                    code = blank("\n".join(lines)).split("\n")
                    body = "\n".join(code[j:k + 1])
                    locs = _locals(lines, code, j, k)
                    arrays = {n: (sz, at) for n, (base, dcl, sz, at, reg) in locs.items() if "[" in dcl and sz}
        out[fname] = "\n".join(lines)


def _calls(body, g):
    out, i = [], 0
    for m in re.finditer(r"\b%s\s*\(" % re.escape(g), body):
        i, d, spans, start = m.end(), 1, [], m.end()
        while i < len(body) and d:
            c = body[i]
            d += (c in "([") - (c in ")]")
            if (c == "," and d == 1) or d == 0:
                spans.append((start, i))
                start = i + 1
            i += 1
        out.append(spans)
    return out


def definitions_any(lines, code):
    """the function definitions of a file, old style or new: (name, first line, '{' line, last line)"""
    out, ds, i = [], depths(code), 0
    while i < len(lines):
        m = re.match(r"^[\w\s*]*?\b([A-Za-z_]\w*)\s*\([^;]*\)\s*$", code[i]) if ds[i] == 0 else None
        if m and m.group(1) not in KEYWORDS and i + 1 < len(lines) and code[i + 1].strip().startswith("{"):
            j, d, k = i + 1, 0, i + 1
            while k < len(lines):
                d += code[k].count("{") - code[k].count("}")
                if d == 0:
                    break
                k += 1
            out.append((m.group(1), i, j, k))
            i = k + 1
        else:
            i += 1
    return out


def _locals(lines, code, j, k, ptr=2):
    """{name: (base, declarator, size, line, register)} of the declarations heading a body, in their order"""
    out = collections.OrderedDict()
    for x in range(j + 1, k):
        s = code[x].strip()
        if not s:
            continue
        r = declarators(s) if s.endswith(";") and "=" not in s and "(" not in s.replace("(*", "") else None
        if not r or not r[0] or not set(re.sub(r"\b(?:struct|union)\s+\w+", "", r[0]).split()) <= TYPEWORDS:
            break
        for n, dcl in r[1]:
            out[n] = (r[0], dcl, size_of(r[0], dcl, n, ptr, {}), x, "register" in r[0])
    return out


def _overlay(port, fname, lines, code, j, k, locs, a, e, g):
    names = list(locs)
    i = names.index(a)
    base, dcl, size, at, reg = locs[a]
    members, off = [(a, base, dcl)], size
    for n in reversed(names[:i]):                           # (the frame grows down: those declared before it lie
        b2, d2, s2, at2, r2 = locs[n]                       #  above, the nearest first)
        if r2:
            continue
        if s2 is None:
            return
        if s2 > 1 and off % 2:
            off += 1
        members.append((n, b2, d2))
        off += s2
        if off >= e:
            break
    if off < e:
        return
    gone = {n for n, b, d in members}
    for x in sorted({locs[n][3] for n in gone}, reverse=True):   # (the declarations: the rest of each line kept)
        r = declarators(code[x].strip())
        keep = [d for n, d in r[1] if n not in gone]
        ind = lines[x][:len(lines[x]) - len(lines[x].lstrip())]
        lines[x] = "%s%s %s;" % (ind, r[0], ", ".join(keep)) if keep else None
    ind = "\t"
    decl = [ind + "struct {\t\t\t/* side by side, as the original's frame had them: %s reads past %s */" % (g, a)]
    decl += [ind + "\t%s %s;" % (re.sub(r"\bregister\s+", "", b), d) for n, b, d in members]
    decl += [ind + "} frame;"]
    first = at                                              # (where the array was declared)
    lines[first] = "\n".join(([lines[first]] if lines[first] is not None else []) + decl)
    rx = re.compile(r"(?<![.>\w])(%s)\b" % "|".join(map(re.escape, gone)))
    for x in range(first + 1, k + 1):
        if lines[x] is not None:
            lines[x] = sub_code(lines[x], rx, lambda m: "frame." + m.group(1))
    lines[:] = "\n".join(l for l in lines if l is not None).split("\n")
    port.notes["locals the program reaches past an array (as the original's frame had them): kept side by side "
               "in a structure"].append("%s (%s: %s)" % (fname, g, ", ".join(n for n, b, d in members)))


# ---------------------------------------------------------------- the compiler's complaints, mended

def members(texts):
    """{member: [the structures that have it]} of every structure the program defines"""
    out = collections.defaultdict(list)
    for t in texts:
        code = blank(t)
        for m in re.finditer(r"\b(struct|union)\s+(\w+)\s*\{", code):
            d, k = 0, m.end() - 1
            while k < len(code):
                d += {"{": 1, "}": -1}.get(code[k], 0)
                if d == 0:
                    break
                k += 1
            for s in code[m.end():k].split(";"):
                r = declarators(s) if s.strip() else None
                if r:
                    for n, dcl in r[1]:
                        out[n].append("%s %s" % (m.group(1), m.group(2)))
    return out


def object_before(line, at):
    """the start of the postfix expression that ends just before `at` (the -> of a member)"""
    i = at
    while True:
        while i and line[i - 1] in " \t":
            i -= 1
        if i and line[i - 1] in ")]":
            close, open_ = line[i - 1], "(" if line[i - 1] == ")" else "["
            d, i = 0, i - 1
            while i >= 0:
                d += {close: 1, open_: -1}.get(line[i], 0)
                if d == 0:
                    break
                i -= 1
            if open_ == "(" and not (i and re.match(r"\w", line[i - 1])):
                return i                                    # (a parenthesized object: the whole)
            continue
        m = re.search(r"[A-Za-z_]\w*$", line[:i])
        if not m:
            return i
        i = m.start()
        if line[:i].endswith("->") or line[:i].endswith("."):
            i -= 2 if line[:i].endswith("->") else 1
            continue
        return i


def abstract(decl, name):
    """'register char *name' -> 'char *' (the type alone, for a cast)"""
    t = re.sub(r"(?<!struct )(?<!union )(?<!enum )\b%s\b" % re.escape(name), "",   # (struct bcd *bcd: the name, not
               re.sub(r"\bregister\s+", "", decl), count=1).strip()                #  the tag)
    t = re.sub(r"\s*\[\s*\d*\s*\]$", " *", t)
    return re.sub(r"\s+\*", " *", t)


def call_args(line, fn, partial=False):
    """[(start, end) of each argument] of the first call of fn on the line, or None (partial: of a call that goes
    on to the next line, the arguments whole on this one)"""
    m = re.search(r"\b%s\s*\(" % re.escape(fn), line)
    if not m:
        return None
    i, d, spans, start = m.end(), 1, [], m.end()
    while i < len(line) and d:
        c = line[i]
        if c in "([{":
            d += 1
        elif c in ")]}":
            d -= 1
        if (c == "," and d == 1) or d == 0:
            spans.append((start, i))
            start = i + 1
        i += 1
    return spans if d == 0 or (partial and d >= 1 and spans) else None


def truthy(body, name):
    """whether a parameter is only ever tested (if (p), !p, p && q): a pointer given for it may be p != 0"""
    uses = list(re.finditer(r"\b%s\b" % re.escape(name), body))
    return bool(uses) and all(re.search(r"(?:(?:^|[^\w\s\])]|\b(?:if|while|for|return))\s*\(|!|&&|\|\|)\s*$",
                                        body[:u.start()]) and   # (a call's argument is no test)
                              re.match(r"\s*(?:\)|&&|\|\|)", body[u.end():]) for u in uses)




# what the compilers say, by the kind of complaint: gcc's words, clang's, MSVC's
SAYS = {
    "libname": [r"conflicting types for '(?P<fn>\w+)'", r"'(?P<fn>\w+)': redefinition; different"],
    "argtype": [r"passing argument (?P<n>\d+) of '(?P<fn>\w+)' (?P<how>from incompatible pointer type|makes integer "
                r"from pointer without a cast|makes pointer from integer without a cast)",
                r"incompatible (?P<how>pointer types|pointer to integer conversion|integer to pointer conversion) "
                r"passing '(?P<frm>[^']*)'(?: \(aka '[^']*'\))? to parameter of type '(?P<to>[^']*)'",
                r"'function': incompatible types - from '(?P<frm>[^']*)' to '(?P<to>[^']*)'",
                r"'function': '(?P<to>[^']*)' differs in levels of indirection from '(?P<frm>[^']*)'"],
    "fnptr": [r"(?:assignment to|initialization of) '(?P<to>[^']*\(\*\)\([^']*\))'(?: \{aka '[^']*'\})? from "
              r"incompatible pointer type "
              r"'(?P<frm>[^']*\(\*\)\([^']*\))'",
              r"incompatible function pointer types assigning to '(?P<to>[^']*)'(?: \(aka '[^']*'\))? from "
              r"'(?P<frm>[^']*)'",
              r"'=': incompatible types - from '(?P<frm>[^']*\*\)\([^']*\))' to '(?P<to>[^']*)'"],
    "intptr": [r"assignment to '(?P<to>[^']*)'(?: \{aka '[^']*'\})? from '(?P<frm>[^']*\*)'(?: \{aka '[^']*'\})? makes "
               r"integer from pointer",
               r"incompatible pointer to integer conversion assigning to '(?P<to>[^']*)'(?: \(aka '[^']*'\))? from "
               r"'(?P<frm>[^']*\*)'",
               r"'=': '(?P<to>[^'*]*)' differs in levels of indirection from '(?P<frm>[^']*\*)'"],
    "assign": [r"(?:assignment to|initialization of) '(?P<to>[^']*\*)'(?: \{aka '[^']*'\})? from incompatible pointer "
               r"type '(?P<frm>[^']*)'",
               r"incompatible pointer types (?:assigning to|initializing) '(?P<to>[^']*\*)'(?: \(aka '[^']*'\))? "
               r"(?:from|with an expression of type) '(?P<frm>[^']*)'",
               r"'(?:=|initializing)': incompatible types - from '(?P<frm>[^']*)' to '(?P<to>[^']*\*)'"],
    "retptr": [r"returning '(?P<frm>[^']*\*)'(?: \{aka '[^']*'\})? from a function with return type "
               r"'(?P<to>[^']*)'(?: \{aka '[^']*'\})? makes integer from pointer",
               r"incompatible pointer to integer conversion returning '(?P<frm>[^']*\*)'"],
    "retint": [r"returning '(?P<frm>[^'*]*)'(?: \{aka '[^']*'\})? from a function with return type "
               r"'(?P<to>[^']*\*)'(?: \{aka '[^']*'\})? makes pointer from integer",
               r"incompatible integer to pointer conversion returning '(?P<frm>[^'*]*)'"],
    "retmix": [r"returning '(?P<frm>[^']*\*)'(?: \{aka '[^']*'\})? from a function with incompatible return type "
               r"'(?P<to>[^']*\*)'",
               r"incompatible pointer types returning '(?P<frm>[^']*\*)'"],
    "callptr": [r"(?:assignment to|initialization of) '(?P<to>[^']*\*)'(?: \{aka '[^']*'\})? from "
                r"'(?P<frm>[^'*]*)'(?: \{aka '[^']*'\})? makes pointer from integer",
                r"incompatible integer to pointer conversion (?:assigning to|initializing) '(?P<to>[^']*\*)'"],
    "neg": [r"comparison is always (?:false|true) due to limited range of data type",
            r"result of comparison of constant -\d+ with expression of type '(?P<t>[^']*)'.* is always",
            r"'(?:==|!=)': signed/unsigned mismatch"],
    "member": [r"request for member '(?P<m>\w+)' in something not a structure or union",
               r"'(?:struct|union) \w+' has no member named '(?P<m>\w+)'", r"no member named '(?P<m>\w+)' in",
               r"member reference base type '[^']*' is not a structure or union",
               r"left of '->(?P<m>\w+)' must point to struct", r"invalid type argument of '->'", r"'(?P<m>\w+)' : is not a member of"],
    "void": [r"void value not ignored", r"requires expression of scalar type \('void' invalid\)",
             r"incompatible type 'void'", r"illegal use of type 'void'", r"'void' illegal", r"void expression",
             r"controlling expression has type 'void'"],
    "ptrcast": [r"cast from pointer to integer of different size", r"cast to smaller integer type '[^']*' from '[^']*\*'"],
    "incomplete": [r"type of formal parameter (?P<n>\d+) is incomplete",
                   r"argument type '(?P<to>(?:union|struct) \w+)' is incomplete"],
    "unionset": [r"incompatible types when assigning to type '(?P<to>union \w+)' from type '(?P<frm>[^']*)'",
                 r"assigning to '(?P<to>union \w+)' from incompatible type '(?P<frm>[^']*)'"],
    "unionarg": [r"incompatible type for argument (?P<n>\d+) of '(?P<fn>\w+)'",
                 r"passing '(?P<frm>[^']*)'(?: \(aka '[^']*'\))? to parameter of incompatible type '(?P<to>union \w+)'"],
    "seqpt": [r"operation on '(?P<v>\w+)' may be undefined", r"unsequenced modification and access to '(?P<v>\w+)'"],
    "intcast": [r"cast to pointer from integer of different size", r"cast to '[^']*' from smaller integer type",
                r"conversion from '[^']*' to '[^']*' of greater size"],
    "arity": [r"too (?:many|few) arguments to function (?:'(?P<fn>\w+)'|call)", r"too (?:many|few) actual parameters"],
}


CAST = r"\(\s*(?:(?:struct|union)\s+\w+|\w+)(?:\s+\w+)*\s*\**\s*\)|\(\s*[\w\s]+\**\s*\(\s*\*\s*\)\s*\([^()]*\)\s*\)"


def said(text):
    """(the kind of a complaint, the match) - or (None, None)"""
    for kind, pats in SAYS.items():
        for p in pats:
            m = re.search(p, text)
            if m:
                return kind, m
    return None, None


def call_at(line, col):
    """(function, argument index, (start, end)) of the innermost call with an argument at col (0-based) - or None"""
    best = None
    for m in re.finditer(r"\b([A-Za-z_]\w*)\s*\(", line):
        if m.group(1) in KEYWORDS:
            continue
        spans = call_args(line[m.start():], m.group(1), partial=True)
        if not spans:
            continue
        for k, (a, b) in enumerate(spans):
            a, b = a + m.start(), b + m.start()
            if a <= col <= b and (best is None or b - a < best[2][1] - best[2][0]):
                best = (m.group(1), k, (a, b))
    return best


def ctype(t, tags=()):
    """a type as a compiler wrote it, for a cast or a declaration of the port's (MSVC leaves out the word struct)"""
    t = re.sub(r"\s*\(aka '[^']*'\)", "", t.replace("__cdecl ", "").replace("__cdecl", ""))
    t = re.sub(r"(?<!struct )(?<!union )\b(\w+)\b", lambda m: "struct " + m.group(1) if m.group(1) in tags else
               m.group(1), t)
    return re.sub(r"\s+", " ", t).strip()


def block_heads(t, notes, fname, ptr2):
    """(char *)p->base + 4 - a block chained by its first word (char **), its bytes after a head of 4: the link
    is a pointer of today's, the head DOS_HEAD(4) (what follows the link as it was); the block's allocation
    (malloc(size + 4)) and its first one's place (g + 4 beside (char **)g) the same"""
    code = blank(t)
    edits, heads = [], set()
    ref = r"(?:\((?:[^()]|\([^()]*\))*\)|[\w.\->]+)"
    for m in re.finditer(r"\(\s*char\s*\*\s*\)\s*(%s)\s*\+\s*(\d+)\b" % ref, code):
        opd, n = m.group(1), int(m.group(2))
        last = re.findall(r"\w+", re.sub(r"\(\s*char\s*\*\s*\*\s*\)", "", opd))
        if n < 2 or n % 2 or not ("char **" in re.sub(r"\s+", " ", opd).replace("( char", "(char") or
                                  (last and last[-1] in ptr2) or re.search(r"\(\s*char\s*\*\s*\*\s*\)", opd)):
            continue
        edits.append((m.start(2), m.end(2), "DOS_HEAD(%d)" % n))
        heads.add(n)
    for m in re.finditer(r"\{([^{}]*)\}", code):           # ({ (char **)g, 1016, g + 4, ... }: a pool's first block)
        row = m.group(1)
        for c in re.finditer(r"\(\s*char\s*\*\s*\*\s*\)\s*(\w+)", row):
            for x in re.finditer(r"\b%s\s*\+\s*(\d+)\b" % re.escape(c.group(1)), row):
                n = int(x.group(1))
                if n >= 2 and n % 2 == 0:
                    edits.append((m.start(1) + x.start(1), m.start(1) + x.end(1), "DOS_HEAD(%d)" % n))
                    heads.add(n)
    for n in heads:                                         # (and the blocks given: their heads as long)
        for m in re.finditer(r"\b(?:malloc|calloc|alloc|getmem|sbrk)\s*\(([^;()]*\S)\s*\+\s*(%d)\s*\)" % n, code):
            edits.append((m.start(2), m.end(2), "DOS_HEAD(%d)" % n))
    seen, out = set(), []
    for e in sorted(edits, reverse=True):
        if e[0] in seen:
            continue
        seen.add(e[0])
        t = t[:e[0]] + e[2] + t[e[1]:]
        out.append(e)
    if out:
        notes["a block chained by its first word, its bytes after a head of the old size: the head DOS_HEAD "
              "(the link a pointer of today's)"].append("%s (%d)" % (fname, len(out)))
    return t


def whole_word_stores(t, notes, fname):
    """*(int *)p = v; with v an intptr_t (the callers give it pointers): the word it stored was either - stored
    whole now"""
    lines, code = t.split("\n"), blank(t).split("\n")
    for name, s_, j_, k_ in definitions_any(lines, code):
        body = "\n".join(code[s_:k_ + 1])
        wide = set(re.findall(r"\bintptr_t\s+(\w+)\s*[,)=]", body))
        for dl in re.findall(r"\bintptr_t\s+([^;(){}]*);", body):   # (intptr_t n, v, pool;)
            wide |= {re.match(r"\s*\**\s*(\w*)", x).group(1) for x in dl.split(",")}
        if not wide:
            continue
        rx = re.compile(r"\*\s*\(\s*(?:u?int16_t|int|short|unsigned)\s*\*\s*\)\s*(\w+)\s*=\s*(\w+)\s*;")
        for x in range(j_, k_ + 1):
            m = rx.search(code[x])
            if m and m.group(2) in wide:
                lines[x] = lines[x][:m.start()] + "*(intptr_t *)%s = %s;" % (m.group(1), m.group(2)) + \
                    lines[x][m.end():]
                notes["a word stored through (int *) of a value that may be a pointer (intptr_t): stored "
                      "whole"].append("%s:%d" % (fname, x + 1))
    return "\n".join(lines)


def file_internals(t, notes, fname):
    """fp->_file, fp->_flag read - the old library's FILE opened up (today's keeps its own to itself): fileno(fp),
    and the old flags of end of file (10h) and error (20h) from feof and ferror"""
    code = blank(t)
    edits = []
    for m in re.finditer(r"([A-Za-z_][\w.]*(?:->\w+|\[[^\]]*\])*)\s*->\s*_(file|flag)\b(?!\s*(?:[-+*/|&^]?=(?!=)|\+\+|--))", code):
        edits.append((m.start(), m.end(), ("fileno(%s)" if m.group(2) == "file" else "dos_fflag(%s)") % m.group(1)))
    for a, b, new in reversed(edits):
        t = t[:a] + new + t[b:]
    if edits:
        notes["the old library's FILE read inside (today's keeps it to itself): fileno, its end-of-file and "
              "error flags (dos_fflag)"].append("%s (%d)" % (fname, len(edits)))
    return t


def stdio_inits(t, notes, fname):
    """FILE *f = stdout; at the top of a file - a constant to the old C, a call of the library's now: the
    variable set by a routine run before main (DOS_INIT)"""
    if not fname.endswith(".c"):
        return t
    sets = []

    def one(m):
        sets.append((m.group(3), m.group(4)))
        notes["a variable given stdin, stdout or stderr where it is defined (not a constant now): set before "
              "main"].append("%s (%s)" % (fname, m.group(3)))
        return "%sFILE *%s;%s" % (m.group(1), m.group(3), m.group(5))
    t = re.sub(r"(?m)^((?:static\s+)?)(FILE\s*\*\s*)(\w+)\s*=\s*(stdin|stdout|stderr)\s*;(.*)$", one, t)
    for v, w in sets:
        t = t.rstrip("\n") + "\nDOS_INIT(dos_init_%s) { %s = %s; }\n" % (v, v, w)
    return t


def word_copies(t, notes, fname, copied):
    """union tokval { int16_t i; char *s; struct sym *sym; ... } and x.val.i = y.val.i somewhere: the int member
    the word was copied by - what it carried, a pointer as well (16 bits all then) - intptr_t, so it carries the
    whole of either"""
    if not copied:
        return t
    code = blank(t)
    edits = []
    for um in re.finditer(r"\bunion\s*\w*\s*\{", code):
        d, k = 0, um.end() - 1
        while k < len(code):
            d += {"{": 1, "}": -1}.get(code[k], 0)
            if d == 0:
                break
            k += 1
        body = code[um.end():k]
        flat = re.sub(r"\b(?:struct|union)\s*\w*\s*\{[^{}]*\}", lambda x: " " * len(x.group(0)), body)
        if not re.search(r"\*\s*\w+\s*;", flat):
            continue                                        # (no pointer in it: a number is a number)
        for st in re.finditer(r"(?m)^([ \t]*)(?:u?int16_t|short|int|unsigned)\s+(\w+)\s*;", flat):
            if st.group(2) in copied:
                a = um.end() + st.start()
                b = um.end() + st.end()
                edits.append((a, b, "%sintptr_t %s;" % (st.group(1), st.group(2))))
                notes["an int of a union of pointers, copied from one to another (the word it was: a pointer "
                      "too): intptr_t"].append("%s (%s)" % (fname, st.group(2)))
    for a, b, new in sorted(edits, reverse=True):
        t = t[:a] + new + t[b:]
    return t


def word_unions(t, notes, fname, olds=()):
    """union { int16_t val; char *name; } - a word that was a number or a pointer (16 bits both): the number as
    wide as the pointer, so either is read whole through either"""
    def one(m):
        body = m.group(2)
        decls = [d.strip() for d in body.split(";") if d.strip()]
        ptrs = [d for d in decls if "*" in d]
        ints = [d for d in decls if re.fullmatch(r"(?:u?int16_t|short|int|unsigned)\s+\w+", d)]
        if not ptrs or len(ints) != 1 or len(ints) + len(ptrs) != len(decls) or \
                (m.group(1) or "").strip() in olds:
            return m.group(0)
        nm = ints[0].split()[-1]
        notes["a union of a number and pointers (one 16-bit word then): the number intptr_t"].append(
            "%s (%s)" % (fname, nm))
        return m.group(0).replace(ints[0], "intptr_t " + nm, 1)
    return re.sub(r"\bunion(\s+\w+)?\s*\{([^{}]*)\}", one, t)


def stored_wide(port, out):
    """push(n) intptr_t n; { s->v = n; } - pop() { int v; v = s->v; return v; }: an intptr_t parameter (a pointer
    it carries) kept in a member, and taken out again by a function giving it back - the member, the local and the
    function's value intptr_t too (16 bits all then); -> whether any was"""
    wide = set()                                            # ((tag, member)) given an intptr_t parameter
    for f, t in out.items():
        if not f.endswith(".c"):
            continue
        lines, code = t.split("\n"), blank(t).split("\n")
        for name, s_, j_, k_ in definitions_any(lines, code):
            me = port.fns.get(name)
            if not me:
                continue
            head = " ".join(code[s_:j_ + 1])               # (the definition's own say, as the port has it now)
            ps = [pn for pn, pt in me.params if re.fullmatch(r"intptr_t\s+\w+", pt.strip())] + \
                re.findall(r"\bintptr_t\s+(\w+)\s*[,);]", head)
            body = "\n".join(code[j_:k_ + 1])
            for pn in ps:
                for m in re.finditer(r"\b(\w+)\s*->\s*(\w+)\s*=\s*%s\s*;" % re.escape(pn), body):
                    tags = set(re.findall(r"\bstruct\s+(\w+)\s*\*\s*%s\s*[;,=)]" % re.escape(m.group(1)),
                                          blank(t)))
                    if len(tags) == 1:
                        wide.add((tags.pop(), m.group(2)))
    changed = False
    done = set()
    for tag, mem in sorted(wide):                          # (in the port's own text: each round is made from it)
        for f, t in list(port.files.items()):
            if not t:
                continue
            m = re.search(r"\bstruct\s+%s\s*\{[^{}]*?\b((?:u?int16_t|int|short|unsigned))\s+%s\s*;" %
                          (re.escape(tag), re.escape(mem)), blank(t))
            if m and "(a handle" not in t[m.end(1):t.find("\n", m.end(1))]:
                port.files[f] = t[:m.start(1)] + "intptr_t" + t[m.end(1):]
                if f in out and out[f]:
                    m2 = re.search(r"\bstruct\s+%s\s*\{[^{}]*?\b((?:u?int16_t|int|short|unsigned))\s+%s\s*;" %
                                   (re.escape(tag), re.escape(mem)), blank(out[f]))
                    if m2:
                        out[f] = out[f][:m2.start(1)] + "intptr_t" + out[f][m2.end(1):]
                port.mended["a member given an intptr_t parameter (the pointer it carried kept there): "
                            "intptr_t"].append("%s (struct %s: %s)" % (f, tag, mem))
                changed = True
                done.add(mem)
    mems = {m for _, m in wide}
    for f, t in list(out.items()):                          # (and taken out again: v = s->v; return v;)
        if not f.endswith(".c") or not mems:
            continue
        lines, code = t.split("\n"), blank(t).split("\n")
        for name, s_, j_, k_ in sorted(definitions_any(lines, code), key=lambda d: -d[1]):
            me = port.fns.get(name)
            if not me or (me.ret or "int").strip() not in ("int", "int16_t", "short", "unsigned", "uint16_t"):
                continue
            body = "\n".join(code[j_:k_ + 1])
            rets = set(re.findall(r"\breturn\s+(\w+)\s*;", body))
            if len(rets) != 1 or re.search(r"\breturn\s+[^\w\s;]|\breturn\s+\w+\s*[^\w\s;]", body):
                continue
            v = rets.pop()
            if not re.search(r"\b%s\s*=\s*\w+\s*->\s*(?:%s)\s*;" % (re.escape(v), "|".join(map(re.escape, mems))),
                             body):
                continue
            for x in range(j_, k_ + 1):
                new = re.sub(r"^(\s*)(?:u?int16_t|int|short|unsigned)(\s+%s\s*;)" % re.escape(v), r"\1intptr_t\2",
                             lines[x])
                if new != lines[x]:
                    lines[x] = new
            me.ret, me.ret_set = "intptr_t", True
            port.local_retypes[(f, name, v)] = "intptr_t"
            for x in range(s_, j_ + 1):
                lines[x] = re.sub(r"^(\s*(?:static\s+)?)(?:u?int16_t|int|short|unsigned)(\s+%s\s*\()" %
                                  re.escape(name), r"\1intptr_t\2", lines[x])
            out[f] = "\n".join(lines)
            for g, tg in list(out.items()):                 # (its declarations)
                if tg and g != f:
                    ng = re.sub(r"(?m)^(\s*(?:extern\s+|static\s+)?)(?:u?int16_t|int|short|unsigned)(\s+%s\s*\()" %
                                re.escape(name), r"\1intptr_t\2", tg)
                    if ng != tg:
                        out[g] = ng
            lines = out[f].split("\n")
            port.mended["a function giving back what a member kept of an intptr_t parameter: intptr_t"].append(
                "%s (%s: %s)" % (f, name, v))
            changed = True
    return changed


def passed_on(port, out):
    """an integer parameter a function gives on, as it is, to a parameter that holds a pointer whole (intptr_t):
    it carried the pointer too (16 bits both) - intptr_t; -> whether any was"""
    wide = {n: {k for k, (pn, pt) in enumerate(f.params) if re.fullmatch(r"intptr_t\s+\w+", pt.strip())}
            for n, f in port.fns.items()}
    wide = {n: ks for n, ks in wide.items() if ks}
    if not wide:
        return False
    pat = re.compile(r"\b(%s)\s*\(" % "|".join(map(re.escape, sorted(wide))))
    changed = False
    for f, t in out.items():
        if not f.endswith(".c"):
            continue
        lines, code = t.split("\n"), blank(t).split("\n")
        for name, s_, j_, k_ in definitions_any(lines, code):
            me = port.fns.get(name)
            if not me:
                continue
            mine = {pn: i for i, (pn, pt) in enumerate(me.params)
                    if re.fullmatch(r"(?:register\s+)?(?:u?int16_t|int|unsigned|short)\s+%s" % re.escape(pn), pt.strip())}
            if not mine:
                continue
            for x in range(j_, k_ + 1):
                for m in pat.finditer(code[x]):
                    spans = call_args(code[x][m.start():], m.group(1)) or []
                    for k, (a, b) in enumerate(spans):
                        arg = code[x][m.start() + a:m.start() + b].strip()
                        if k in wide[m.group(1)] and arg in mine:
                            i = mine.pop(arg)
                            me.params[i] = (arg, "intptr_t %s" % arg)
                            port.param_types[(me.name, i)] = me.params[i][1]
                            port.mended["integer parameter given on to one that holds a pointer (it carried one): "
                                        "intptr_t"].append("%s:%d (%s, %s)" % (f, x + 1, me.name, arg))
                            changed = True
    return changed


def union_member_for(out, union, typ):
    """the member of a union (its definition in the port's text) that a value of the type is: of the type itself,
    or the like kind (a double the floating one, a pointer the first pointer, a number the integer of its size)"""
    tag = union.split()[-1]
    body = None
    for t in out.values():
        m = re.search(r"\bunion\s+%s\s*\{" % re.escape(tag), blank(t or ""))
        if m:
            d, k = 1, m.end()
            c = blank(t)
            while k < len(c) and d:
                d += {"{": 1, "}": -1}.get(c[k], 0)
                k += 1
            body = c[m.end():k - 1]
            break
    if body is None:
        return None
    flat = re.sub(r"\{[^{}]*\}", "", body)                  # (its own members: not those of a structure in it)
    mems = []
    for decl in flat.split(";"):
        r = declarators(decl.strip()) if decl.strip() else None
        if r and len(r[1]) == 1 and r[0].strip() and (not r[0].strip().startswith(("struct", "union")) or
                                                       re.match(r"(?:struct|union)\s+\w+$", r[0].strip()) or
                                                       "*" in r[1][0][1]):
            nm, dcl = r[1][0]
            mems.append((nm, (re.sub(r"\s+", " ", r[0]).strip() + " " + dcl.replace(nm, "")).replace(" ", "")))
    t = typ.replace(" ", "").replace("const", "")
    t = {"long": "int32_t", "longint": "int32_t", "int": "int32_t", "unsignedint": "uint32_t", "unsigned": "uint32_t",
         "short": "int16_t", "shortint": "int16_t", "shortunsignedint": "uint16_t", "unsignedshort": "uint16_t",
         "float": "double", "unsignedlong": "uint32_t"}.get(t, t)   # (the compiler's words: today's int is 32 bits)
    for nm, mt in mems:
        if mt == t:
            return nm
    if "*" in t:
        return next((nm for nm, mt in mems if mt == "char*"), next((nm for nm, mt in mems if "*" in mt), None))
    if t in ("double", "float", "longdouble"):
        return next((nm for nm, mt in mems if mt in ("double", "float")), None)
    if t in ("int32_t", "uint32_t", "longlong", "int64_t", "intptr_t"):
        return next((nm for nm, mt in mems if mt in ("int32_t", "uint32_t")), None)
    return next((nm for nm, mt in mems if mt in ("int16_t", "uint16_t", "char", "unsignedchar")), None)


def visible_tags(out, f):
    """the struct and union tags a file sees: its own and its headers' (theirs, in turn)"""
    seen, todo, tags = set(), [f], set()
    while todo:
        n = todo.pop()
        if n in seen or n not in out:
            continue
        seen.add(n)
        t = blank(out[n])
        tags |= set(re.findall(r"\b(?:struct|union)\s+(\w+)\s*\{", t))
        todo += re.findall(r'#\s*include\s*"([^"]+)"', out[n])
    return tags


def mend(port, out, diags):
    """one round of mending from the compiler's complaints; -> whether anything was changed"""
    _all = []

    def all_code():                                         # (every file's code, as one text: once a round)
        if not _all:
            _all.append("\n".join(blank(t_) for t_ in out.values() if t_))
        return _all[0]
    _vis = {}

    def visible(f_):                                        # (the tags a file sees: once a file a round)
        if f_ not in _vis:
            _vis[f_] = visible_tags(out, f_)
        return _vis[f_]
    changed_ = passed_on(port, out)
    if stored_wide(port, out):
        changed_ = True
    notes = port.mended
    mem = members(list(out.values()))
    tags = {s.split()[1] for v in mem.values() for s in v}
    changed = False
    by_line = collections.defaultdict(list)
    for d in diags:
        by_line[(d["file"], d["line"])].append(d)
    for (f, ln), ds in by_line.items():
        if f not in out:
            continue
        lines = out[f].split("\n")
        if ln > len(lines):
            continue
        l = orig = lines[ln - 1]
        for d in sorted(ds, key=lambda d_: -d_.get("col", 0)):   # (right to left: a mend moves what follows it)
            kind, m = said(d["text"])
            if not kind and "incompatible" in d["text"]:
                kind, m = "other", re.match("", "")         # (only the a = b = 0 below may mend it)
            if not kind:
                continue
            col = max(d.get("col", 1) - 1, 0)

            def g(k, m=m, text=d["text"]):
                v = m.groupdict().get(k)
                if v and re.search(r"\b__\w+", v):          # (a name of one C library's own - __p_sig_fn_t: what
                    aka = re.search(r"'%s' \{aka '([^']*)'\}" % re.escape(v), text)   # it is, every compiler's)
                    if aka:
                        return aka.group(1)
                return v
            if kind == "libname":
                fn = g("fn")
                if f == "proto.h" and fn in port.fns and fn not in port.renames:
                    port.renames[fn] = fn + "_"             # (proto.h agrees with the definitions: what it meets is
                    notes["function named as one of the C library of today: renamed with _ after"].append(  # the
                        "%s (%s)" % (port.fns[fn].file, fn))                                                # library)
                    changed = True
            elif kind == "argtype":
                at = call_at(l, col)
                if g("fn"):                                 # (gcc names the function and the argument)
                    spans = call_args(l, g("fn"), partial=True)
                    k = int(g("n")) - 1
                    at = (g("fn"), k, spans[k]) if spans and k < len(spans) else at
                fn = port.fns.get(at[0]) if at else None
                if at and "pointer from integer" in d["text"] + (g("how") or "") or \
                        at and "integer to pointer" in d["text"]:
                    a, b = at[2]                            # (a word of 16 bits laid over two bytes - *(int *)&s->sa -
                    arg = l[a:b].strip()                    #  given as a pointer: it holds a handle; the pointer)
                    if re.match(r"\(?\s*\*\s*\(\s*(?:u?int16_t|int|short|unsigned)\s*\*\s*\)", arg):
                        lead = l[a:b][:len(l[a:b]) - len(l[a:b].lstrip())]
                        l = l[:a] + lead + "dos_hp(%s)" % arg + l[b:]
                        notes["a word of two bytes given as a pointer (a pointer kept there in two bytes): the handle "
                              "it holds, the pointer"].append("%s:%d" % (f, ln))
                        changed = True
                        continue
                want = next((re.search(r"expected '(?P<to>[^']*)'(?: \{aka '[^']*'\})? but argument is of type", x) for x in
                             d.get("notes", []) if "expected '" in x), None)
                if not fn and at and want:                  # (a parameter of the C library's: what it takes)
                    t = re.sub(r"\bconst\s+", "", ctype(want.group("to"), tags))   # (the variable is written too)
                    if re.search(r"\b__\w+", t):                # (the library's own name for it: what it is)
                        aka = next((re.search(r"expected '[^']*' \{aka '([^']*)'\}", x) for x in d.get("notes", [])
                                    if "aka" in x), None)
                        if aka:
                            t = ctype(aka.group(1), tags)
                    a, b = at[2]
                    arg = l[a:b].strip()
                    v = re.fullmatch(r"(?:[\w\]\[]+(?:->|\.))*(\w+)", arg)
                    if v and (v.group(1) in port.fns or v.group(1) in port.asm_protos or "(*)" in t):
                        v = None                            # (a function, or a function's pointer: a cast)
                    fn_here = enclosing(out.get(f, ""), ln)
                    if v and (not any(re.search(r"\*\s*%s\s*[;,)=\[]" % re.escape(v.group(1)), blank(t_))
                                      for t_ in out.values()) or
                              (fn_here and declares_local(out[f], fn_here, v.group(1)))):
                        v = None                            # (an int that held a pointer, there: a cast)
                    if v and "*" in t and not re.match(CAST, arg) and not re.match(r"\s*(?:const\s+)?void\s*\*", t):
                        # (a variable or member the recovered source typed loosely - the bytes did not say: it is
                        # what the library takes, so it is declared one)
                        name = v.group(1)
                        if name not in port.retyped:
                            port.retyped[name] = t
                            port.decl_rules.append((r"(?m)^(\s*(?:static\s+|extern\s+|register\s+)?)(?:const\s+)?"
                                                    r"(?:u?int16_t|int|unsigned|char|void|u?int32_t)\s*\*\s*%s\s*;" %
                                                    re.escape(name), r"\g<1>%s%s%s;" % (
                                                        t, "" if t.endswith("*") else " ", name)))
                            notes["variable or member given to the C library as its type (the recovered source "
                                  "had it loosely typed): declared so"].append("%s:%d (%s, %s)" % (f, ln, name, t))
                            changed = True
                    elif not re.match(CAST, arg):
                        l = l[:a] + (" " if l[a:a + 1] == " " else "") + "(%s)%s" % (t, arg) + l[b:]
                        notes["argument of another type than the C library's parameter: a cast"].append(
                            "%s:%d (%s)" % (f, ln, at[0]))
                        changed = True
                    continue
                if not fn or at[1] >= len(fn.params):
                    continue
                n, decl = fn.params[at[1]]
                a, b = at[2]
                arg = l[a:b].strip()
                if arg.startswith("(intptr_t)") or arg.endswith("!= 0"):
                    continue                                # (mended already)
                how = g("how") or ""
                derefs = re.search(r"\*\s*\+*\s*%s\b|\b%s\s*(?:\+\+|--)?\s*(?:\[|->)" % (re.escape(n), re.escape(n)),
                                   getattr(fn, "body", ""))
                if "incompatible pointer" in how and (fn.name, at[1]) in getattr(port, "given_params", set()) and \
                        "void *" not in decl and not derefs:   # (callers give it pointers of more than one type, as
                    k = at[1]                               #  the int it was took them all: any pointer)
                    fn.params[k] = (n, "void *%s" % n)
                    port.param_types[(fn.name, k)] = fn.params[k][1]
                    notes["parameter the old definition left an int, given pointers of several types: void *"].append(
                        "%s:%d (%s, %s)" % (f, ln, fn.name, n))
                    changed = True
                    continue
                given_t = g("frm") or next((x_.group("t") for x_ in (re.search(
                    r"but argument is of type '(?P<t>[^']*)'", y) for y in d.get("notes", [])) if x_), "")
                word_out = re.fullmatch(r"(?:register\s+)?(?:u?int16_t|int|unsigned|short)\s*\*\s*%s" % re.escape(n),
                                        decl.strip())
                if "incompatible pointer" in how and word_out and (
                        re.search(r"\*\s*\*\s*$", given_t) or re.fullmatch(r"intptr_t\s*\*", given_t.strip())):
                    k = at[1]                               # (an int * the old function stored a pointer through -
                    fn.params[k] = (n, "intptr_t *%s" % n)  #  a word was either: a place that holds either whole)
                    port.param_types[(fn.name, k)] = fn.params[k][1]
                    notes["int * parameter given the place of a pointer (it stores one there): intptr_t *"].append(
                        "%s:%d (%s, %s)" % (f, ln, fn.name, n))
                    changed = True
                    continue
                if "incompatible pointer" in how and re.fullmatch(r"intptr_t\s*\*\s*%s" % re.escape(n), decl.strip()):
                    if re.search(r"\*\s*\*\s*$", given_t):
                        continue                            # (a pointer's place: an intptr_t's, the same 8 bytes)
                    v_ = re.fullmatch(r"&\s*(\w+)", arg)
                    fn_at = enclosing(out.get(f, ""), ln)
                    if v_ and fn_at and re.fullmatch(r"(?:u?int16_t|short|int|unsigned)\s*\*", given_t.strip()) and \
                            declares_local(out[f], fn_at, v_.group(1)):
                        port.local_retypes[(f, fn_at[0], v_.group(1))] = "intptr_t"   # (the word it gives: whole)
                        notes["a local's place given for one a pointer is stored in: the local intptr_t"].append(
                            "%s:%d (%s, %s)" % (f, ln, fn_at[0], v_.group(1)))
                        changed = True
                        continue
                if "from pointer" in how or "pointer to integer" in how or \
                   (g("frm") and "*" in g("frm") and "*" not in (g("to") or "*")):
                    given = next((re.search(r"but argument is of type '(?P<t>[^']*\*)'", x) for x in d.get("notes", [])
                                  if "but argument is of type" in x), None)
                    as_ptr = re.search(r"\*\s*%s\b|\b%s\s*(?:\[|->)|\b(?:%s)\s*\(\s*%s\s*[,)]" % (
                        re.escape(n), re.escape(n), "|".join(sorted(port.profile["libc"])), re.escape(n)),
                        getattr(fn, "body", ""))            # (used as a pointer there: deref, index, a library's)
                    if given and not used_as_number(getattr(fn, "body", ""), n) and \
                            (n in getattr(fn, "implicit", set()) or (as_ptr and re.fullmatch(
                            r"(?:register\s+)?(?:int|unsigned|short|int16_t|uint16_t)\s+%s" % re.escape(n),
                            decl.strip()))) and not getattr(fn, "loose", False):
                        k = at[1]                           # (a parameter the old definition never declared - an
                        t_ = ctype(given.group("t"), tags)  #  int by default: what the callers give it)
                        fn.params[k] = (n, "%s%s%s" % (t_, "" if t_.endswith("*") else " ", n))
                        port.param_types[(fn.name, k)] = fn.params[k][1]
                        port.given_params = getattr(port, "given_params", set()) | {(fn.name, k)}
                        notes["parameter the old definition left undeclared (an int): the pointer its callers give"
                              ].append("%s:%d (%s, %s: %s)" % (f, ln, fn.name, n, t_))
                        changed = True
                        continue
                    if truthy(getattr(fn, "body", ""), n):
                        new, why = "%s != 0" % arg, "pointer given for a parameter that is only tested: p != 0"
                    elif re.search(r"\bintptr_t\b", decl):   # (the parameter holds either: the pointer given whole)
                        new, why = "(intptr_t)(%s)" % arg, "pointer given for a parameter that holds either: (intptr_t)"
                    else:                                   # (an int that carried a pointer - 16 bits both, then:
                        k = at[1]                           #  the parameter holds either whole now)
                        fn.params[k] = (n, re.sub(r"^(?:register\s+)?(?:u?int16_t|int|unsigned|short)\b",
                                                  lambda mm: mm.group(0).replace(mm.group(0).split()[-1],
                                                                                 "intptr_t"), decl))
                        port.param_types[(fn.name, k)] = fn.params[k][1]
                        notes["integer parameter given a pointer (both were 16 bits): intptr_t, to hold either"].append(
                            "%s:%d (%s, %s)" % (f, ln, fn.name, n))
                        changed = True
                        continue
                else:
                    t = ctype(g("to"), tags) if g("to") else abstract(decl, n)
                    t = widths(t, blank(t), collections.defaultdict(list), f, port.int16)[0]
                    if arg.replace(" ", "").startswith("(%s)" % t.replace(" ", "")):
                        continue                            # (cast so already: the complaint is another's)
                    new = "(%s)%s" % (t, arg if re.fullmatch(r"[\w.\->\[\]]+|\w+\([^()]*\)|&?[\w.\->\[\]]+", arg)
                                      else "(%s)" % arg)
                    why = "argument of another type than the parameter (as old C took it): a cast"
                l = l[:a] + (" " if l[a:a + 1] == " " else "") + new + l[b:]
                notes[why].append("%s:%d (%s, argument %d)" % (f, ln, fn.name, at[1] + 1))
                changed = True
            elif kind == "fnptr":
                x = re.match(r"\s*(\w+)\s*=\s*(\w+)\s*;", l)
                at = re.match(r"\w+", l[col:])
                fn = port.fns.get(at.group()) if at else None
                if fn and fn.void and not ctype(g("to")).startswith("void"):
                    fn.void, fn.keep_int = False, True      # (kept with functions that give an int: so does it)
                    notes["function put where functions giving an int are (a table, a pointer): int"].append(
                        "%s:%d (%s)" % (f, ln, fn.name))
                    changed = True
                elif at and at.group() in port.fns and not x and g("to") and "initialization" in d["text"]:
                    t = ctype(g("to"), tags)                # (an element of a table of them: as the table's)
                    l = l[:col] + "(%s)" % t + l[col:]
                    notes["function put in a table of functions of another prototype: a cast"].append(
                        "%s:%d (%s)" % (f, ln, at.group()))
                    changed = True
                elif x and x.group(2) in port.fns and x.group(1) in getattr(port, "fptr_mixed", set()) and g("to"):
                    t = ctype(g("to"), tags)                # (given functions of several prototypes: each made the
                    l = l[:x.start(2)] + "(%s)" % t + l[x.start(2):]   # pointer's own type)
                    notes["pointer given functions of several prototypes: each cast to the pointer's type"].append(
                        "%s:%d (%s)" % (f, ln, x.group(2)))
                    changed = True
                elif x and x.group(2) in port.fns:
                    t = ctype(g("frm"), tags)
                    if "*)" not in t:
                        t = t.replace("(", "(*)(", 1)       # (clang names the function's type, not its pointer's)
                    t = re.sub(r"\(\s*\*\s*\)", "(*)", t)
                    port.fptr.setdefault(x.group(1), set()).add(t)
                    changed = True
                else:
                    kind = "assign"                         # (no function of the program given: a cast)
            chain = re.fullmatch(r"(\s*)((?:[\w.\->\[\]]+\s*=\s*){2,})(0|\(\w[\w\s*]*\)\s*0)\s*;(.*)", l)
            if chain and (kind in ("assign", "intptr", "callptr", "fnptr") or "incompatible" in d["text"]):
                lhs = [x.strip() for x in chain.group(2).split("=") if x.strip()]
                l = chain.group(1) + ", ".join("%s = 0" % x for x in lhs) + ";" + chain.group(4)
                notes["0 given to several of different types at once (a = b = 0): to each"].append("%s:%d" % (f, ln))
                changed = True
                kind = None
            if kind == "assign":
                eq = [x for x in re.finditer(r"(?<![=!<>+\-*/%&|^])=(?!=)", l)]
                eq.sort(key=lambda x: abs(x.start() - col))
                if eq:
                    a = eq[0].end()
                    b = a
                    d, inq = 0, None
                    while b < len(l) and not (d == 0 and l[b] in ";,"):
                        d += (l[b] in "([{") - (l[b] in ")]}")    # (a member of a compound literal: to its })
                        if d < 0:
                            break
                        b += 1
                    while b > a and l[b - 1] == " ":                # (the value: not the space before its end)
                        b -= 1
                    rhs = l[a:b].strip()
                    t = ctype(g("to"), tags)
                    c = re.match(CAST, rhs)
                    if c and c.end() < len(rhs) and rhs[:c.end()].replace(" ", "") != "(%s)" % t.replace(" ", ""):
                        rhs = rhs[c.end():].strip()         # (a cast of an earlier round, to another type: replaced)
                        c = None
                    if rhs and not c:
                        l = l[:a] + " (%s)%s" % (t, rhs if re.fullmatch(r"[\w.\->\[\]]+|\w+\(.*\)", rhs) else
                                                  "(%s)" % rhs) + l[b:]
                        notes["pointer given to a pointer of another type (as old C took it): a cast"].append(
                            "%s:%d" % (f, ln))
                        changed = True
            elif kind == "retptr":                          # (a function of old C's int that gives a pointer)
                above = out[f].split("\n")[:ln]
                for x in reversed(above):
                    dm = re.match(r"^[\w\s*]*?\b(\w+)\s*\([^;]*\)\s*$", x)
                    if dm and dm.group(1) in port.fns:
                        fn = port.fns[dm.group(1)]
                        t = ctype(g("frm"), tags)
                        if not fn.ret or fn.ret in ("int", "int16_t"):
                            fn.ret, fn.void, fn.keep_int = t, False, True
                            notes["function of old C's int that gives a pointer: declared to give it"].append(
                                "%s (%s, %s)" % (f, fn.name, t))
                            changed = True
                        break
            elif kind in ("retint", "retmix"):              # (a function given a pointer type by its callers that
                fn_at = enclosing(out.get(f, ""), ln)       #  gives an int - an offset; or pointers of more than
                fn = port.fns.get(fn_at[0]) if fn_at else None   # one type: as the int it was gave them all)
                if fn and kind == "retint" and not getattr(fn, "int_ret", False):
                    fn.ret, fn.void, fn.keep_int, fn.int_ret = "int", False, True, True
                    fn.ret_set = True
                    notes["function that gives an integer though its callers take it for a pointer (16 bits both "
                          "then): int, the callers cast"].append("%s (%s)" % (f, fn.name))
                    changed = True
                elif fn and kind == "retmix" and fn.ret != "void *":
                    fn.ret, fn.void, fn.keep_int, fn.ret_set = "void *", False, True, True
                    notes["function that gives pointers of several types (old C's int): void *"].append(
                        "%s (%s)" % (f, fn.name))
                    changed = True
            elif kind == "callptr":                         # (a pointer from a function declared int)
                near = sorted(re.finditer(r"(?<![=!<>+\-*/%&|^])=(?!=)", l), key=lambda x: abs(x.start() - col))
                cm = re.match(r"\s*\(*\s*(\w+)\s*\(", l[near[0].end():]) if near else None   # (the call right after
                name = cm.group(1) if cm else None                                          #  the = complained of)
                t = ctype(g("to"), tags)
                if name in port.fns and getattr(port.fns[name], "int_ret", False):
                    a = near[0].end()                       # (it gives an integer: what the old pointer was)
                    b, dd = a, 0
                    while b < len(l) and not (dd == 0 and l[b] in ";,"):
                        dd += (l[b] in "([") - (l[b] in ")]")
                        if dd < 0:
                            break
                        b += 1
                    rhs = l[a:b].strip()
                    if rhs and not re.match(CAST, rhs):
                        l = l[:a] + " (%s)(intptr_t)(%s)" % (t, rhs) + l[b:]
                        notes["integer given to a pointer (as old C took it): a cast"].append("%s:%d" % (f, ln))
                        changed = True
                elif name in port.fns and (not port.fns[name].ret or port.fns[name].ret in ("int", "int16_t")):
                    fn = port.fns[name]
                    fn.ret, fn.void, fn.keep_int = t, False, True
                    notes["function of old C's int that gives a pointer: declared to give it"].append(
                        "%s:%d (%s, %s)" % (f, ln, name, t))
                    changed = True
                elif not name or (name not in port.fns and name not in port.asm_protos):
                    eq = sorted(re.finditer(r"(?<![=!<>+\-*/%&|^])=(?!=)", l), key=lambda x: abs(x.start() - col))
                    if eq:                                  # (no call: the value cast, as old C took it)
                        a = eq[0].end()
                        b, dd = a, 0
                        while b < len(l) and not (dd == 0 and l[b] in ";,"):
                            dd += (l[b] in "([") - (l[b] in ")]")
                            if dd < 0:
                                break
                            b += 1
                        rhs = l[a:b].strip()
                        if rhs and not re.match(CAST, rhs):
                            l = l[:a] + " (%s)(%s)" % (t, rhs) + l[b:]
                            notes["integer given to a pointer (as old C took it): a cast"].append("%s:%d" % (f, ln))
                            changed = True
                elif name in port.asm_protos and not port.asm_protos[name].startswith(t):
                    port.asm_protos[name] = "%s%s%s();" % (t, "" if t.endswith("*") else " ", name)
                    notes["routine of the assembly language that gives a pointer: declared to give it"].append(
                        "%s:%d (%s, %s)" % (f, ln, name, t))
                    changed = True
            elif kind == "neg":
                # x == -1 of an unsigned: 16 bits, the 8086 compared it unsigned (0xFFFF); here the uint16_t is made
                # an int first and the test fails - the constant given the operand's type keeps the meaning
                t = ctype(g("t")) if g("t") else None
                best = None
                for x in re.finditer(r"(?P<a>[\w\]\)]+)\s*(?P<op>[=!<>]=|[<>])\s*(?P<k>-\s*\d+)\b|"
                                     r"(?P<k2>-\s*\d+)\s*(?P<op2>[=!<>]=|[<>])\s*(?P<b>[\w\[(]+)", l):
                    if best is None or abs(x.start() - col) < abs(best.start() - col):
                        best = x
                if best:
                    opnd = best.group("a") or best.group("b")
                    name = re.sub(r"\[.*", "", opnd).strip("()")
                    decl = re.search(r"\b(u?int16_t|uint8_t|unsigned char)\b[^;]*\b%s\b" % re.escape(name), out[f])
                    t = t or (decl.group(1) if decl else "uint16_t")   # (a byte's test was always false there too)
                    if t in ("uint16_t", "unsigned short"):
                        k = "k" if best.group("k") else "k2"
                        a, b = best.span(k)
                        l = l[:a] + "(uint16_t)%s" % best.group(k).replace(" ", "") + l[b:]
                        notes["negative constant compared with a 16-bit unsigned (unsigned there, an int here): "
                              "(uint16_t)-1"].append("%s:%d" % (f, ln))
                        changed = True
            elif kind == "intptr":
                eq_ = l.find("=", max(0, col - 1))
                if eq_ > 0 and l[eq_ + 1:eq_ + 2] != "=" and \
                        re.search(r"\*\s*\(\s*(?:u?int16_t|int|short|unsigned)\s*\*\s*\)\s*&[^=]*$", l[:eq_]):
                    end_ = l.find(";", eq_)                 # (*(int *)&s->sa = str: a pointer kept in a word laid
                    rhs_ = l[eq_ + 1:end_].strip() if end_ > eq_ else ""   #  over two bytes - its handle there)
                    if rhs_ and not rhs_.startswith("dos_ph("):
                        l = l[:eq_ + 1] + " dos_ph(%s)" % rhs_ + l[end_:]
                        notes["a pointer kept in a word of two bytes laid over others (*(int *)&s->sa): its handle "
                              "(dos_ph)"].append("%s:%d" % (f, ln))
                        changed = True
                        continue
                x = re.match(r"\s*(\w+)\s*=", l)
                t = ctype(g("frm"), tags)
                if not x:                                   # (if ((fp = fopen(..)) == 0): the name before the =)
                    eqat = l.find("=", max(0, col - 1))
                    vm = re.search(r"(?<![.>])\b(\w+)\s*$", l[:eqat]) if eqat > 0 else None
                    fn_at = enclosing(out.get(f, ""), ln)
                    if vm and fn_at and declares_local(out[f], fn_at, vm.group(1)):
                        x = vm
                if not x:                                   # (a member given a pointer's value - a = p = 0: the
                    eq = l.find("=", max(0, col - 1))       #  number it was, the pointer's bits)
                    mm = re.search(r"(?<![\w.>])(\w+)\s*(?:->|\.)\s*(\w+)\s*$", l[:eq]) if eq > 0 else None
                    if mm and l[eq + 1:eq + 2] != "=":      # (x->node = n, int node: the member a pointer's width)
                        tg = re.findall(r"\bstruct\s+(\w+)\s*\*?\s*%s\s*[;,=)\[]" % re.escape(mm.group(1)),
                                        blank(out.get(f, "")))
                        done_ = False
                        for h_, ht in (list(port.files.items()) if len(set(tg)) == 1 else []):
                            sm = re.search(r"\bstruct\s+%s\s*\{[^{}]*?\b((?:u?int16_t|int|short|unsigned))\s+%s\s*;" %
                                           (re.escape(tg[0]), re.escape(mm.group(2))), blank(ht))
                            if sm:
                                port.files[h_] = ht[:sm.start(1)] + "intptr_t" + ht[sm.end(1):]
                                if out.get(h_):
                                    s2 = re.search(r"\bstruct\s+%s\s*\{[^{}]*?\b((?:u?int16_t|int|short|unsigned))"
                                                   r"\s+%s\s*;" % (re.escape(tg[0]), re.escape(mm.group(2))),
                                                   blank(out[h_]))
                                    if s2:
                                        out[h_] = out[h_][:s2.start(1)] + "intptr_t" + out[h_][s2.end(1):]
                                notes["a member given a pointer (an int then, a pointer's width): intptr_t"].append(
                                    "%s (struct %s: %s)" % (h_, tg[0], mm.group(2)))
                                changed = done_ = True
                        if done_:
                            continue
                    end = l.find(";", eq)
                    rhs = l[eq + 1:end].strip() if eq >= 0 and end > eq else ""
                    if rhs and not rhs.startswith("(intptr_t)") and l[eq + 1:eq + 2] != "=":
                        l = l[:eq + 1] + " (intptr_t)(%s)" % rhs + l[end:]
                        notes["a pointer's value given to an integer (both 16 bits then): (intptr_t)"].append(
                            "%s:%d" % (f, ln))
                        changed = True
                    continue
                var = x.group(1)
                fn_at = enclosing(out.get(f, ""), ln)       # (the function the complaint is in, and whether the
                local = fn_at and declares_local(out[f], fn_at, var)   #  variable is one of its own)
                key_ = (f, fn_at[0], var) if local else var
                wide_ = "intptr_t" in (g("to") or "") or "long long" in (d["text"] or "")
                if key_ in port.retyped and not wide_:
                    continue
                scope = "\n".join(blank(out[f]).split("\n")[fn_at[1]:fn_at[3] + 1]) if local else \
                    all_code()
                if wide_ or used_as_number(scope, var):     # (a number too: the pointer's bits given to it, a cast)
                    eqat = l.find("=", max(0, col - 1))
                    a, dd = eqat + 1, 0
                    b = a
                    while b < len(l) and not (dd == 0 and l[b] in ";,") and dd >= 0:
                        dd += (l[b] in "([") - (l[b] in ")]")
                        if dd < 0:
                            break
                        b += 1
                    rhs = l[a:b].strip()
                    if eqat > 0 and rhs and not rhs.startswith("(intptr_t)"):
                        l = l[:a] + " (intptr_t)(%s)" % rhs + l[b:]
                        notes["a pointer's value given to an integer (both 16 bits then): (intptr_t)"].append(
                            "%s:%d" % (f, ln))
                        changed = True
                    continue
                if not local and not any(re.search(r"(?m)^(?:extern\s+|static\s+)?(?:u?int16_t|int|unsigned)\s+"
                                                   r"(?:[^;(){}\n]*,\s*)?%s\s*[;,=\[]" % re.escape(var), blank(t_))
                                         for t_ in out.values()):
                    continue                                # (no variable of the program's by that name: not a guess)
                port.retyped[key_] = t                      # (the variable holds a pointer: it is declared one)
                if local:                                   # (there alone: another function's may be a number)
                    port.local_retypes[(f, fn_at[0], var)] = t
                else:
                    v = re.escape(var)
                    port.decl_rules.append((r"(?m)^(\s*(?:static\s+|extern\s+|register\s+)?)(?:u?int16_t|int|unsigned)"
                                            r"\s+%s(\s*=\s*0)?\s*;" % v,
                                            r"\g<1>%s%s%s\g<2>;" % (t, "" if t.endswith("*") else " ", var)))
                    port.decl_rules.append((GLOBAL_DECL % v, split_decl(var, t)))   # (extern int a, v, b;)
                notes["integer variable that holds a pointer (the 8086's were both 16 bits): declared a "
                      "pointer"].append("%s:%d (%s, %s)" % (f, ln, var, t))
                changed = True
            elif kind == "ptrcast":                         # ((int)p: a pointer as a word, 16 bits both then - the
                x = re.match(r"\(\s*(?:u?int16_t|int|short|unsigned|unsigned\s+int)\s*\)", l[col:])   # whole of it
                if not x:                                   #  now)
                    x_ = list(re.finditer(r"\(\s*(?:u?int16_t|int|short|unsigned|unsigned\s+int)\s*\)", l[:col + 1]))
                    x = x_[-1] if x_ else None
                    at_ = x.start() if x else None
                else:
                    at_ = col
                if x is None:
                    continue
                l = l[:at_] + "(intptr_t)" + l[at_ + x.end() - x.start():]
                notes["a pointer cast to an int (both 16 bits then): to intptr_t, the whole of it"].append(
                    "%s:%d" % (f, ln))
                changed = True
            elif kind == "incomplete":                      # (a structure given by value, its header not here)
                tag = g("to")
                if not tag:
                    at = call_at(l, col)
                    fn = port.fns.get(at[0]) if at else None
                    if fn and at[1] < len(fn.params):
                        x = re.search(r"\b(?:struct|union)\s+\w+", fn.params[at[1]][1])
                        tag = x.group(0) if x else None
                if not tag:
                    continue
                hs = [h for h, t_ in out.items() if h.endswith(".h") and
                      re.search(r"\b%s\s*\{" % re.escape(tag).replace(r"\ ", r"\s+"), blank(t_))]
                if len(hs) == 1 and hs[0] not in port.need_headers.get(f, set()):
                    port.need_headers.setdefault(f, set()).add(hs[0])
                    notes["a structure given by value where its header was not included: included"].append(
                        "%s:%d (%s)" % (f, ln, hs[0]))
                    changed = True
            elif kind == "unionset":                        # (u = a member's value: that member of it set)
                to, frm = g("to"), g("frm")
                eq = l.find("=", max(0, col - 1))
                if eq < 0 or l[eq + 1:eq + 2] == "=":
                    continue
                a, dd = eq + 1, 0
                b = a
                while b < len(l) and not (dd == 0 and l[b] in ";,") and dd >= 0:
                    dd += (l[b] in "([{") - (l[b] in ")]}")
                    if dd < 0:
                        break
                    b += 1
                rhs = l[a:b].strip()
                mname = union_member_for(out, to, frm)
                if not rhs or not mname or rhs.startswith("(%s)" % to):
                    continue
                l = l[:a] + " (%s){ .%s = %s }" % (to, mname, rhs) + l[b:]
                notes["a union given a member's value (old C copied the bytes): the union, that member set"].append(
                    "%s:%d (.%s)" % (f, ln, mname))
                changed = True
            elif kind == "unionarg":                        # (a union given by value: old C pushed a member's
                to = g("to") or next((x_.group(1) for x_ in (re.search(r"expected '(union \w+)'", y)   # value, its
                                                             for y in d.get("notes", [])) if x_), None)   # bytes)
                frm = g("frm") or next((x_.group(1) for x_ in (re.search(r"argument is of type '([^']*)'", y)
                                                               for y in d.get("notes", [])) if x_), None)
                if not to or not frm:
                    continue
                at = call_at(l, col)
                if g("fn"):
                    spans = call_args(l, g("fn"), partial=True)
                    k = int(g("n")) - 1
                    at = (g("fn"), k, spans[k]) if spans and k < len(spans) else at
                if not at:
                    continue
                a, b = at[2]
                arg = l[a:b].strip()
                if arg.startswith("(%s)" % to):
                    continue
                mname = union_member_for(out, to, frm)
                zero = re.fullmatch(r"0[lL]?", arg) is not None
                if not mname and not zero:
                    continue
                lead = l[a:b][:len(l[a:b]) - len(l[a:b].lstrip())]
                l = l[:a] + lead + ("(%s){ 0 }" % to if zero and not mname or zero and frm in ("int",) else
                                    "(%s){ .%s = %s }" % (to, mname, arg)) + l[b:]
                if to.split()[-1] not in visible(f):        # (the union, complete where it is made)
                    h_ = next((h for h, t_ in out.items() if h.endswith(".h") and
                               re.search(r"\bunion\s+%s\s*\{" % re.escape(to.split()[-1]), blank(t_))), None)
                    if h_:
                        port.need_headers.setdefault(f, set()).add(h_)
                notes["a union given by value as one of its members' values (old C pushed the bytes): the union, "
                      "that member set"].append("%s:%d (%s, .%s)" % (f, ln, at[0], mname))
                changed = True
            elif kind == "seqpt":                           # (f(op, (n -= 2) == 0, n): the old compiler's order
                v = g("v")                                  #  of the arguments, written out)
                order = port.profile.get("arg_order", "right")
                x = re.search(r"\(\s*(%s\s*(?:[-+*/%%&|^]|<<|>>)=\s*[^()=,;]+|(?:\+\+|--)\s*%s)\s*\)" % (v, v), l)
                st_ = re.match(r"(\s*)(return\s+)?", l)
                if not x or order != "left" or l.rstrip().endswith(("{", ")")) or \
                        re.match(r"\s*(?:if|while|for|switch)\b", l) or not l.rstrip().endswith(";"):
                    continue
                if re.search(r"\b%s\b" % v, l[st_.end():x.start()]):
                    continue                                # (read before it: an order of its own)
                eff = x.group(1)
                body = l[:x.start()] + v + l[x.end():]
                ind, ret = st_.group(1), st_.group(2) or ""
                rest = body[st_.end():].rstrip()[:-1]
                l = "%s%s(%s, %s);" % (ind, ret, eff, rest) if ret else "%s%s, %s;" % (ind, eff, rest)
                notes["an argument changes a variable another reads (their order unsaid in C): the old compiler's "
                      "order, left to right, written out"].append("%s:%d (%s)" % (f, ln, v))
                changed = True
            elif kind == "intcast":                         # (an int made a pointer: it held one - 16 bits both)
                x = re.match(r"\(\s*(?:(?:struct|union)\s+\w+|\w+)[\w\s]*\*+\s*\)\s*(?:\(\s*(\w+)\s*\)|(\w+))", l[col:])
                if not x or re.match(r"\s*(?:\(|->|\.|\[)", l[col + x.end():]):
                    continue
                var = x.group(1) or x.group(2)
                fn_at = enclosing(out.get(f, ""), ln)
                fn = port.fns.get(fn_at[0]) if fn_at else None
                k = next((i for i, (pn, pt) in enumerate(fn.params) if pn == var), None) if fn else None
                if k is not None:
                    if re.fullmatch(r"(?:register\s+)?(?:u?int16_t|int|unsigned|short)\s+%s" % re.escape(var),
                                    fn.params[k][1].strip()):
                        fn.params[k] = (var, "intptr_t %s" % var)
                        port.param_types[(fn.name, k)] = fn.params[k][1]
                        notes["integer parameter made a pointer (it held one: both were 16 bits): intptr_t"].append(
                            "%s:%d (%s, %s)" % (f, ln, fn.name, var))
                        changed = True
                    continue
                local = fn_at and declares_local(out[f], fn_at, var)
                key_ = (f, fn_at[0], var) if local else var
                if key_ in port.retyped:
                    continue
                if local:
                    port.retyped[key_] = "intptr_t"
                    port.local_retypes[(f, fn_at[0], var)] = "intptr_t"
                elif any(re.search(r"(?m)^(?:extern\s+|static\s+)?(?:u?int16_t|int|unsigned)\s+%s\s*;" % re.escape(var),
                                   blank(t_)) for t_ in out.values()):
                    port.retyped[key_] = "intptr_t"
                    port.decl_rules.append((r"(?m)^(\s*(?:static\s+|extern\s+|register\s+)?)(?:u?int16_t|int|unsigned)"
                                            r"\s+%s\s*;" % re.escape(var), r"\g<1>intptr_t %s;" % var))
                else:
                    continue
                notes["integer variable made a pointer (it held one: both were 16 bits): intptr_t"].append(
                    "%s:%d (%s)" % (f, ln, var))
                changed = True
            elif kind == "member":
                name = g("m")
                xs = [x for x in re.finditer(r"->\s*(\w+)", l) if (not name or x.group(1) == name)]
                xs.sort(key=lambda x: abs(x.start() - col))
                seen_ = visible(f)                          # (of the structures the file sees: the one that has it)
                for x in xs[:1]:
                    cands = mem.get(x.group(1), [None])
                    in_h = {tg for h_, t_ in out.items() if h_.endswith(".h")
                            for tg in re.findall(r"\b(?:struct|union)\s+(\w+)\s*\{", blank(t_ or ""))}
                    s = next((c for c in cands if c and c.split()[-1] in seen_),
                             next((c for c in cands if c and c.split()[-1] in in_h), None))   # (a header's: it
                    if not s:                                                             #  can be included)
                        continue
                    o = object_before(l, x.start())
                    obj = l[o:x.start()].strip()
                    if not s or obj.startswith("((%s *)" % s):
                        continue
                    l = l[:o] + "((%s *)%s)" % (s, obj) + l[x.start():]
                    notes["member of a structure used on another pointer (as old C allowed): a cast to the "
                          "structure that has it"].append("%s:%d (%s)" % (f, ln, x.group(1)))
                    changed = True
            elif kind == "void":
                for x in re.finditer(r"\b(\w+)\s*\(", l):
                    fn = port.fns.get(x.group(1))
                    if fn and fn.void:
                        fn.void, fn.keep_int = False, True
                        notes["function whose value is used after all: int (it was what the register held)"].append(
                            "%s:%d (%s)" % (f, ln, fn.name))
                        changed = True
            elif kind == "arity":
                names = [g("fn")] if g("fn") else [x.group(1) for x in re.finditer(r"\b(\w+)\s*\(", l)]
                for n in names:
                    fn = port.fns.get(n)
                    sp = call_args(l, n) if fn else None
                    if fn and not fn.loose and not getattr(fn, "varargs", None) and sp is not None and \
                       len([s for s in sp if l[s[0]:s[1]].strip()]) != len(fn.params):
                        fn.loose = True
                        notes["function called with other numbers of arguments than it has: old-style definition "
                              "kept (its calls unchecked, as they were)"].append("%s:%d (%s)" % (f, ln, n))
                        changed = True
        if l != orig:                                       # (kept as a rule: the next round's text has the line)
            o = port.line_origin.get(f, {}).get(ln)
            port.line_fixes[(f, o[0], o[1]) if o else (f, orig)] = l
    for var, types in port.fptr.items():                    # (a pointer to functions of one prototype: given it)
        if len(types) > 1 and var not in getattr(port, "fptr_mixed", set()):
            port.fptr_mixed = getattr(port, "fptr_mixed", set()) | {var}
            changed = True
        if len(types) == 1 and var not in port.fptr_done:
            t = next(iter(types))
            port.decl_rules.append((r"\b(?:unsigned\s+|signed\s+)?\w+\s*\**\s*\(\s*\*\s*%s\s*\)\s*\(\s*\)" % var,
                                    t.replace("(*)", "(*%s)" % var, 1)))
            port.fptr_done.add(var)
            notes["pointer to functions declared with the prototype of those it is given"].append("%s (%s)" % (var, t))
            changed = True
    return changed or changed_
