"""The data of an assembler module, as C (part of the port; see port/asm16.py for its code).

A module of data alone is the old linker's business: its bytes were placed, and the names in them were offsets.
Here they are C - each table given the C program's types where it declares them (and where a pointer of a
declared one points: that type too), its words that held an offset a pointer now, initialized as the bytes were:

  image(text)        the module's segments: their bytes, which words were the offset of a name (relocations), the
                     labels' places, each segment's class
  messages(text)     a table of numbered messages for the C library's __NMSG_TEXT / __FMSG_TEXT (segments of class
                     MSG / FAR_MSG: a word, the number; the text; a 0), as C: the table and the two lookups
  typed(text, ct)    the tables of a data module with pointers in them, as C definitions with initializers
"""
import re

from codebreaker.port import asm16


class Seg:
    def __init__(self, name, cls):
        self.name, self.cls = name, cls
        self.data = bytearray()
        self.reloc = {}                                     # (offset of a word: (symbol, displacement))
        self.labels = {}                                    # (name: offset)
        self.items = []                                     # ((offset, size) of each DB/DW/DD item, in order)


def _value(v, equ):
    """a DW/DD item: (number, None) or (displacement, symbol)"""
    v = re.sub(r"(?i)^offset\s+(?:\w+:)?", "", v.strip())
    n = asm16.number(v)
    if n is not None:
        return n, None
    if v in equ and asm16.number(equ[v]) is not None:
        return asm16.number(equ[v]), None
    m = re.fullmatch(r"([A-Za-z_$@?][\w$@?]*)\s*(?:([+-])\s*(\w+))?", v)
    if m:
        d = asm16.number(m.group(3)) if m.group(3) else 0
        return (-d if m.group(2) == "-" else d), m.group(1)
    raise asm16.Asm16Error("a data item not understood: %r" % v)


def _bytes(item):
    """a DB item: its bytes"""
    item = item.strip()
    if len(item) >= 2 and item[0] == item[-1] and item[0] in "'\"":
        q = item[0]
        return item[1:-1].replace(q + q, q).encode("latin1")
    n = asm16.number(item)
    if n is None:
        raise asm16.Asm16Error("a byte not understood: %r" % item)
    return bytes([n & 0xFF])


def image(text):
    """{segment name: Seg} of a module's data"""
    segs, cur, equ = {}, None, {}
    for l in asm16.parse(text):
        d = l.directive
        if l.label and cur:
            cur.labels[l.label] = len(cur.data)
        if not d:
            continue
        kind, name, rest = d
        rest = asm16.strip_comment(rest).strip()
        if kind == "SEGMENT":
            m = re.search(r"'([^']*)'", rest)
            cur = segs.setdefault(name, Seg(name, m.group(1) if m else None))
            if m and not cur.cls:
                cur.cls = m.group(1)
            continue
        if kind == "ENDS":
            cur = None
            continue
        if kind in ("EQU", "="):
            equ[name] = rest
            continue
        if not cur:
            continue
        if kind == "LABEL":
            cur.labels[name] = len(cur.data)
            continue
        if kind in ("DB", "DW", "DD"):
            if name:
                cur.labels[name] = len(cur.data)
            unit = {"DB": 1, "DW": 2, "DD": 4}[kind]
            m = re.match(r"(?i)\s*(\w+)\s+DUP\s*\((.*)\)\s*$", rest)
            if m:
                n = asm16.number(m.group(1))
                v = m.group(2).strip()
                fill = 0 if v == "?" else asm16.number(v) or 0
                at = len(cur.data)
                cur.data += (fill & ((1 << (8 * unit)) - 1)).to_bytes(unit, "little") * n
                cur.items.append((at, unit * n))
                continue
            for it in asm16.split_ops(rest):
                at = len(cur.data)
                if unit == 1:
                    cur.data += _bytes(it)
                else:
                    v, sym = _value(it, equ)
                    if sym:
                        cur.reloc[at] = (sym, v)
                        v = 0
                    cur.data += (v & ((1 << (8 * unit)) - 1)).to_bytes(unit, "little")
                cur.items.append((at, len(cur.data) - at))
    return segs


def c_string(b):
    """bytes as a C string literal (its 0 left to C)"""
    out = []
    for c in b:
        ch = chr(c)
        if ch == "\\":
            out.append("\\\\")
        elif ch == '"':
            out.append('\\"')
        elif ch == "\n":
            out.append("\\n")
        elif ch == "\t":
            out.append("\\t")
        elif ch == "\r":
            out.append("\\r")
        elif 32 <= c < 127:
            out.append(ch)
        else:
            out.append("\\%03o" % c)
    s = "".join(out)
    return '"%s"' % re.sub(r'(\\[0-7]{3})(?=[0-7])', r'\1""', s)


BASE_SIZES = {"char": 1, "signed char": 1, "unsigned char": 1, "short": 2, "int": 2, "unsigned": 2,
              "unsigned int": 2, "unsigned short": 2, "int16_t": 2, "uint16_t": 2, "long": 4, "unsigned long": 4,
              "int32_t": 4, "uint32_t": 4, "int8_t": 1, "uint8_t": 1}


def publics(text):
    out = set()
    for l in asm16.parse(text):
        if l.directive and l.directive[0] == "PUBLIC":
            out |= {x.strip() for x in l.directive[2].split(",") if x.strip()}
    return out


def typed(text, ct):
    """the C of a data module whose words hold addresses (a table of chains, of names): each table typed - by the
    C program's declaration of its name, or by what points to it - and initialized as its bytes were. None when
    the module has no addresses in it"""
    segs = image(text)
    if not any(s.reloc for s in segs.values()):
        return None
    where = {}
    for s in segs.values():
        for n, o in s.labels.items():
            where[n] = (s, o)
    pubs = publics(text)
    cname = lambda n: n[1:] if n.startswith("_") and n in pubs else n

    def osize(t):
        t = re.sub(r"\s+", " ", t.strip())
        if t.endswith("*"):
            return 2
        if t.startswith(("struct ", "union ")):
            lay = ct.old.get(t)
            return lay["@size"] if lay else None
        return BASE_SIZES.get(re.sub(r"^(signed|const) ", "", t))

    types, queue, notes = {}, [], []                        # (label: the type of its elements)

    def assign(sym, t, why):
        t = re.sub(r"\s+", " ", t.strip())
        if sym not in where:
            notes.append("%s: not in the module (%s)" % (sym, why))
            return
        if sym in types:
            if types[sym] != t:
                notes.append("%s: %s and %s (%s)" % (sym, types[sym], t, why))
            return
        types[sym] = t
        queue.append(sym)

    for p in sorted(pubs):
        v = ct.var(cname(p))
        if v and p in where:
            stars = re.sub(r"\[[^\]]*\]", "", v[1]).replace(cname(p), "", 1).count("*")
            assign(p, v[0].strip() + (" " + "*" * stars if stars else ""), "declared")

    def extent(sym):
        s, o = where[sym]
        nxt = min([x for x in s.labels.values() if x > o] + [len(s.data)])
        return s, o, nxt

    def walk(t, s, at):
        """what the element of type t at s+at points to: typed"""
        if t.endswith("*"):
            r = s.reloc.get(at)
            if r:
                assign(r[0], t[:-1], "pointed to")
            return
        lay = ct.old.get(t)
        if not lay:
            return
        for name, v in lay.items():
            if name == "@size" or not isinstance(v, tuple):
                continue
            fo, fs, base, dcl, isptr = v
            if isptr and isinstance(dcl, str):
                stars = dcl.replace(name, "", 1).count("*")
                r = s.reloc.get(at + fo)
                if r:
                    assign(r[0], base + (" " + "*" * (stars - 1) if stars > 1 else ""), "%s.%s" % (t, name))

    while queue:
        sym = queue.pop()
        t = types[sym]
        size = osize(t)
        if not size:
            notes.append("%s: no size for %s" % (sym, t))
            continue
        s, o, end = extent(sym)
        for k in range(max(1, (end - o) // size)):
            walk(t, s, o + k * size)

    def word(s, at, n):
        return int.from_bytes(bytes(s.data[at:at + n]), "little")

    def ref(r, t):
        sym, disp = r
        if sym not in types:
            return None
        u = types[sym]
        us = osize(u) or 1
        name = cname(sym)
        if disp % us == 0:
            return "&%s[%d]" % (name, disp // us) if disp else ("&%s[0]" % name)
        return "(%s)((char *)%s + %d)" % (t, name, disp)    # (into the middle of an element)

    def value(t, s, at):
        if t.endswith("*"):
            r = s.reloc.get(at)
            if r:
                v = ref(r, t)
                if v:
                    return v
                notes.append("%s+%d: %s, not typed" % (s.name, at, r[0]))
                return "0"
            n = word(s, at, 2)
            return "0" if n == 0 else "(%s)%d" % (t, n)
        lay = ct.old.get(t)
        if lay:
            vals = []
            for name, v in lay.items():
                if name == "@size" or not isinstance(v, tuple):
                    continue
                fo, fs, base, dcl, isptr = v
                if isptr:
                    stars = dcl.replace(name, "", 1).count("*") if isinstance(dcl, str) else 1
                    vals.append(value(base + " " + "*" * stars, s, at + fo))
                elif isinstance(dcl, str) and "[" in dcl:
                    n = osize(base) or 1
                    vals.append("{ %s }" % ", ".join(str(word(s, at + fo + j * n, n)) for j in range(fs // n)))
                else:
                    n = word(s, at + fo, fs)
                    if fs in (1, 2, 4) and not base.startswith("unsigned") and n >= 1 << (8 * fs - 1):
                        n -= 1 << (8 * fs)
                    vals.append(str(n))
            return "{ %s }" % ", ".join(vals)
        n = osize(t) or 1
        v = word(s, at, n)
        if n in (1, 2, 4) and not t.startswith("unsigned") and v >= 1 << (8 * n - 1):
            v -= 1 << (8 * n)
        return str(v)

    order = sorted(types, key=lambda x: (where[x][0].name, where[x][1]))
    decls, defs = [], []
    for sym in order:
        t = types[sym]
        s, o, end = extent(sym)
        size = osize(t)
        count = max(1, (end - o) // size)
        name = cname(sym)
        v = ct.var(name) if sym in pubs else None
        array = not v or "[" in v[1]
        static = "" if sym in pubs else "static "
        sep = "" if t.endswith("*") else " "
        dim = "[%d]" % count if array else ""
        decls.append("%s%s%s%s%s;" % (static, t, sep, name, dim))
        if t == "char" and array:
            b = bytes(s.data[o:o + count])
            text_ = b[:-1] if b.endswith(b"\0") else b
            if b"\0" not in text_:
                defs.append("%s%s %s%s = %s;" % (static, t, name, dim, c_string(text_)))
                continue
        if array:
            items = [value(t, s, o + k * size) for k in range(count)]
            defs.append("%s%s%s%s%s = {\n\t%s\n};" % (static, t, sep, name, dim, ",\n\t".join(items)))
        else:
            defs.append("%s%s%s%s = %s;" % (static, t, sep, name, value(t, s, o)))
    head = ["/* the tables of an assembler module, with the addresses in them (made by CodeBreaker's port: typed "
            "by the C program's declarations and what points where) */"]
    head += ["/* (not typed: %s) */" % x for x in notes[:20]]
    typed.notes = notes
    return "\n".join(head + decls + [""] + defs)


def messages(text):
    """the C of a module of numbered messages (segments of class MSG and FAR_MSG), or None: a table of each - the
    near ones __NMSG_TEXT's, the far ones __FMSG_TEXT's (their numbers are their own: the same number in both is
    two messages)"""
    segs = image(text)
    tables = {"MSG": [], "FAR_MSG": []}
    for s in segs.values():
        if s.cls in tables and s.data:
            tables[s.cls].append(s)
    if not any(tables.values()):
        return None
    out = ["/* the numbered messages of the C library's __NMSG_TEXT (near) and __FMSG_TEXT (far) */"]
    for cls, name in (("MSG", "dos_nmsgs"), ("FAR_MSG", "dos_fmsgs")):
        out.append("static const struct { int16_t num; const char *text; } %s[] = {" % name)
        for sg in tables[cls]:
            b, i = bytes(sg.data), 0
            while i + 2 <= len(b):
                num = int.from_bytes(b[i:i + 2], "little")
                if num == 0xFFFF:
                    break
                j = b.index(b"\0", i + 2) if b"\0" in b[i + 2:] else len(b)
                out.append("\t{ %d, %s }," % (num, c_string(b[i + 2:j])))
                i = j + 1
        out += ["\t{ -1, 0 }", "};", ""]
    for fn, name in (("__NMSG_TEXT", "dos_nmsgs"), ("__FMSG_TEXT", "dos_fmsgs")):
        out += ["char *%s(int32_t n)\t\t/* (an int: as the callers give it) */" % fn,
                "{",
                "\tint32_t i;",
                "\tfor (i = 0; %s[i].text; i++)" % name,
                "\t\tif (%s[i].num == n) return (char *)%s[i].text;" % (name, name),
                "\treturn (char *)\"\";",
                "}",
                ""]
    return "\n".join(out)
