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
import re
import subprocess

KEYWORDS = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern",
            "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static",
            "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"}
TYPEWORDS = {"int", "char", "long", "short", "unsigned", "signed", "float", "double", "void", "struct", "union",
             "enum", "register", "static", "extern", "auto", "const", "volatile", "FILE", "int16_t", "uint16_t",
             "int32_t", "uint32_t", "int8_t", "uint8_t"}
PRINTF = {"printf", "fprintf", "sprintf"}


# ---------------------------------------------------------------- reading the text

def blank(text):
    """the text with what is inside its comments, strings and character constants made spaces (newlines and
    the quotes kept, every offset the same): what is code"""
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

class Fn:
    def __init__(self, name, ret, params, static, line):
        self.name, self.ret, self.params, self.static, self.line = name, ret, params, static, line
        self.void = False
        self.loose = False                                  # (calls it does not match: old-style kept)
        self.bare = 0                                       # (bare returns given a value)

    def proto(self, names=True):
        ps = ", ".join(t if names else t for n, t in self.params) or "void"
        ret = "void" if self.void else (self.ret or "int")
        sep = "" if ret.endswith("*") else " "
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
                        r"union\s+\w+)\s*)+\**\s*", code[i - 1]) and code[i - 1].strip():
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
ALLOC = re.compile(r"\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*(\w*alloc\w*)\s*\(")


MEMFNS = ("memset", "memcpy", "memmove", "memcmp", "movbytes", "fillb", "far_memcpy", "_fmemcpy", "_fmemset")


LOCAL_DECL = r"(?m)^(\s*(?:register\s+)?)(u?int16_t|int|unsigned)\s+((?:\w+\s*,\s*)*)%s\s*((?:,\s*\w+\s*)*);"


def used_as_number(code, var):
    """whether a variable is computed with as a number somewhere (n /= 2, n * v, n << 1, n & 7, n % k)"""
    v = re.escape(var)
    return bool(re.search(r"\b%s\s*(?:[*/%%^|]=|<<=|>>=|[/%%]|<<|>>|\*(?!\s*\))|&(?!&)|\|(?!\|))" % v, code) or
                re.search(r"(?:[\w)\]]\s*(?:[*/%%]|<<|>>)|[*/%%]=|<<=|>>=)\s*%s\b" % v, code) or
                re.search(r"\(?\s*\b%s\s*(?:[+\-]=?[^;)]*)?\)?\s*(?:[<>]=?)\s*-?\s*(?:[1-9]|0\b)" % v, code) or
                re.search(r"\b%s\s*[+\-]\s*\w+\s*\)\s*[&|]" % v, code) or   # (compared with a number; (off + n) & ~n)
                re.search(r"\[\s*(?:\+\+|--)?\s*%s\s*(?:\+\+|--)?\s*\]" % v, code))   # (an index)


def enclosing(text, ln):
    """(name, first line, '{' line, last line) of the function line ln (1-based) is in - None"""
    code = blank(text)
    for d in definitions_any(text.split("\n"), code.split("\n")):
        if d[1] <= ln - 1 <= d[3]:
            return d
    for f, s, j, k in definitions(text.split("\n"), code.split("\n")):   # (an old-style one, kept so)
        if s <= ln - 1 <= k:
            return (f.name, s, j, k)
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
        return out + "%s%s%s;" % (self.typ, "" if self.typ.endswith("*") else " ", self.name)


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


def struct_sizes(text, old, notes, fname):
    """(struct S *)malloc(n + 29): the recovered source gave a structure's size as the number it was (the bytes the
    same) - now sizeof(struct S) and the rest: a structure of today's layout is longer"""
    code = blank(text)
    edits = []
    for m in ALLOC.finditer(code):
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
        if m.group(2) not in ("malloc", "calloc", "realloc", "_fmalloc") and n == size:
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
        for n in sorted(self.files):                        # (unions whose views meet at the old places: so still)
            self.files[n] = union_views(self.files[n], self.union_notes, n)
        casts = set()                                       # (struct rfld *)&sym->u.rfl: a structure of its own laid
        for t in self.files.values():                       #  over a view of the union - laid out as the view is
            for m in re.finditer(r"\(\s*struct\s+(\w+)\s*(?:far\s*|near\s*|huge\s*)?\*\s*\)\s*&[^;,)]*?\bu\.(\w+)",
                                 blank(t)):
                casts.add((m.group(1), m.group(2)))
        for tag, view in sorted(casts):
            vbody = None
            for t in self.files.values():
                vm = re.search(r"\bstruct\s*\{([^{}]*)\}\s*%s\s*;" % re.escape(view), t)
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
            rx = re.compile(r"\b(?:%s)\b[ \t]*" % "|".join(map(re.escape, list(kw) + sorted(mac))))
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
        self.agree_notes = []
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
        defn = re.compile(r"^(?P<base>(?:unsigned\s+|signed\s+)?(?:char|short|int|long|unsigned))\s+(?P<name>\w+)"
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
            code = blank(t)
            t = old_declarations(t, code, known, self.notes, n,
                                 list(self.profile["platform"]["calls"]) + list(self.aliases) +
                                 [re.search(r"[A-Za-z_]\w*", w).group(0) for w, d in self.aliases.values()])
            if n.endswith(".h"):
                t, defs = header_data(t, blank(t), n, self.notes)
                self.data += [(n, d) for d in defs]
            out[n] = t
        tags = {}                                           # (a structure: the header that defines it)
        for h, t in out.items():
            if h.endswith(".h"):
                for m in re.finditer(r"\b(struct|union)\s+(\w+)\s*\{", blank(t)):
                    tags.setdefault("%s %s" % (m.group(1), m.group(2)), h)
        for n, t in list(out.items()):
            if not n.endswith(".c"):
                continue
            code = blank(t)
            own = {"%s %s" % (m.group(1), m.group(2)) for m in re.finditer(r"\b(struct|union)\s+(\w+)\s*\{", code)}
            inc = set(re.findall(r'#\s*include\s*"([^"]+)"', t))
            need = sorted({tags[u] for u in set(re.findall(r"\b(?:struct|union)\s+\w+", code)) - own
                           if u in tags and tags[u] not in inc})
            if need:                                        # (the old build gave it the header some other way)
                t = t.replace('#include "port.h"', '#include "port.h"\n' + "\n".join('#include "%s"' % h
                                                                                   for h in need), 1)
                out[n] = t
                self.notes["header of the structures a file uses, which it did not include: included"].append(
                    "%s (%s)" % (n, ", ".join(need)))
        for n, t in out.items():
            if n.endswith(".c"):
                out[n] = self._definitions(t, n)
        for n, t in out.items():
            code = blank(t)
            t, code = widths(t, code, self.notes, n, self.int16)
            t = word_unions(t, self.notes, n, set(getattr(self, "old_records", ())))
            t = formats(t, code, self.notes, n, self.int16, self.profile.get("printf_hex_upper", False))
            t = struct_sizes(t, self.old_layouts(), self.notes, n)
            t = header_offsets(t, self.old_layouts(), self.notes, n)
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

    def finish(self, fname, text):
        """the loop's rules: names that meet the C library's renamed, declarations given their real types, lines
        mended"""
        if self.renames:
            rx = re.compile(r"\b(%s)\b" % "|".join(map(re.escape, sorted(self.renames))))
            text = sub_code(text, rx, lambda m: self.renames[m.group(1)])
        for rx, new in self.decl_rules:
            text = re.sub(rx, new, text)
        text = apply_local_retypes(text, fname, self.local_retypes)
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
            g.body = body
            valued = re.search(r"\breturn\b\s*[^\s;]", body)
            bare = re.findall(r"\breturn\s*;", body)
            if f.name == "main":
                g.ret = g.ret or "int"
            g.void = not valued and not g.ret and f.name != "main" and not getattr(g, "keep_int", False)
            if not valued and not bare and g.ret and g.ret.strip() not in ("void", "static void") and \
                    f.name != "main":
                # (a function that gives a value, falling off its end: what the register held - the value of
                # its last assignment of a call's, when nothing after it gives the register another)
                stmts = [x for x in range(j + 1, k) if code[x].strip()]
                last = None
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
                        lines[s] = ("static " if g.static else "") + g.ret
                for x in range(s + 1, j):
                    if lines[x].strip().endswith(";"):
                        lines[x] = re.sub(r"\b(?:unsigned\s+int|unsigned|short|int)\b(?!\s*\*)", "intptr_t", lines[x])
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
        proto = ["/* proto.h -- every function of the program, declared (made by CodeBreaker's port) */",
                 "#ifndef PROTO_H", "#define PROTO_H"] + ["%s;" % t for t in tags] + [""]
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
        hs = sorted({h for h, d in self.data if h.endswith(".h")} |
                    ({h for h in self.files if h.endswith(".h")} if self.asm_data else set()))
        data = ["/* data.c -- the variables the headers share, defined once (made by CodeBreaker's port) */",
                '#include "port.h"'] + ['#include "%s"' % h for h in hs] + [""] + [d for h, d in self.data] + \
            (["", "/* the variables an assembler module of data alone placed */"] + [d for h, d in self.asm_data]
             if self.asm_data else [])
        dtext = "\n".join(data) + "\n"
        dtext = widths(dtext, blank(dtext), collections.defaultdict(list), "data.c", self.int16)[0]
        return {"port.h": self.finish("port.h", "\n".join(port) + "\n"), "proto.h": self.finish("proto.h", ptext + "\n"),
                "data.c": self.finish("data.c", dtext) if self.data or self.asm_data else None}


# ---------------------------------------------------------------- the frame, where the program counts on it

SIZES = {"char": 1, "signed char": 1, "unsigned char": 1, "int16_t": 2, "uint16_t": 2, "int32_t": 4, "uint32_t": 4,
         "int": 2, "unsigned": 2, "short": 2, "long": 4, "float": 4, "double": 8, "unsigned int": 2,
         "unsigned short": 2, "short int": 2, "unsigned long": 4, "long int": 4, "unsigned long int": 4,
         "signed int": 2, "signed long": 4, "int8_t": 1, "uint8_t": 1}


OLD_SCALARS = {"char": 1, "signed char": 1, "unsigned char": 1, "short": 2, "short int": 2, "unsigned short": 2,
               "int": 2, "unsigned": 2, "unsigned int": 2, "signed": 2, "long": 4, "long int": 4, "unsigned long": 4,
               "unsigned long int": 4, "float": 4, "double": 8}


def union_views(text, notes, fname):
    """a union whose views (its structures) meet at the same old places - prc.len where lab.scope was, the bytes
    of a char f1c[6] over another's members: the views laid out again, each old place one new place in all of
    them (padding where a view has nothing there), so that what one view writes another reads where it did"""
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


def call_args(line, fn):
    """[(start, end) of each argument] of the first call of fn on the line, or None"""
    m = re.search(r"\b%s\s*\(" % re.escape(fn), line)
    if not m:
        return None
    i, d, spans, start = m.end(), 1, [], m.end()
    while i < len(line) and d:
        c = line[i]
        if c in "([":
            d += 1
        elif c in ")]":
            d -= 1
        if (c == "," and d == 1) or d == 0:
            spans.append((start, i))
            start = i + 1
        i += 1
    return spans if d == 0 else None


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
        spans = call_args(line[m.start():], m.group(1))
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


def mend(port, out, diags):
    """one round of mending from the compiler's complaints; -> whether anything was changed"""
    if passed_on(port, out):
        changed_ = True
    else:
        changed_ = False
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
        for d in ds:
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
                    spans = call_args(l, g("fn"))
                    k = int(g("n")) - 1
                    at = (g("fn"), k, spans[k]) if spans and k < len(spans) else at
                fn = port.fns.get(at[0]) if at else None
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
                        d += (l[b] in "([") - (l[b] in ")]")
                        if d < 0:
                            break
                        b += 1
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
                if key_ in port.retyped:
                    continue
                scope = "\n".join(blank(out[f]).split("\n")[fn_at[1]:fn_at[3] + 1]) if local else \
                    "\n".join(blank(t_) for t_ in out.values() if t_)
                if used_as_number(scope, var):              # (a number too: the pointer's bits given to it, a cast)
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
                                            r"\s+%s\s*;" % v, r"\g<1>%s%s%s;" % (t, "" if t.endswith("*") else " ", var)))
                notes["integer variable that holds a pointer (the 8086's were both 16 bits): declared a "
                      "pointer"].append("%s:%d (%s, %s)" % (f, ln, var, t))
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
                for x in xs[:1]:
                    s = mem.get(x.group(1), [None])[0]
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
                    if fn and not fn.loose and sp is not None and \
                       len([s for s in sp if l[s[0]:s[1]].strip()]) != len(fn.params):
                        fn.loose = True
                        notes["function called with other numbers of arguments than it has: old-style definition "
                              "kept (its calls unchecked, as they were)"].append("%s:%d (%s)" % (f, ln, n))
                        changed = True
        if l != orig:                                       # (kept as a rule: the next round's text has the line)
            o = port.line_origin.get(f, {}).get(ln)
            port.line_fixes[(f, o[0], o[1]) if o else (f, orig)] = l
    for var, types in port.fptr.items():                    # (a pointer to functions of one prototype: given it)
        if len(types) == 1 and var not in port.fptr_done:
            t = next(iter(types))
            port.decl_rules.append((r"\b(?:unsigned\s+|signed\s+)?\w+\s*\**\s*\(\s*\*\s*%s\s*\)\s*\(\s*\)" % var,
                                    t.replace("(*)", "(*%s)" % var, 1)))
            port.fptr_done.add(var)
            notes["pointer to functions declared with the prototype of those it is given"].append("%s (%s)" % (var, t))
            changed = True
    return changed or changed_
