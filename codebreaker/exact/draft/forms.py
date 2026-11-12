"""forms.py -- the forms of values and places, read back: registers folded into expressions, steps and stores
joined, pointers and their widths, the library's macros, the frame's locals, and the variants a beam tries."""
import os
import re

from codebreaker.exact.draft.text import (_balanced, _block_end, _conj, _declare_pointer, _else_ifs, _HEADERS,
                                          _if_parts, _LIBDATA, _live_after, _not, _same_arm, _strip, _TABLES,
                                          _top_commas, _top_split, _truth, _unlabelled, ARRAYS, BYTEPTR, FILEPTR,
                                          PTRSIZE, SIZES, split_body, STRUCTARR)
from codebreaker.exact.draft.shapes import flat_returns


def frame_names(text, rule, regs_last=False):
    """the locals renamed so that the compiler gives each the frame slot the program shows. rule (the toolchain
    profile's "frame_order"): {"mod": M} - the compiler gives the locals of a function (register ones too) their
    slots in the order of their names' buckets in its table of names, (the sum of the name's characters) mod M,
    ascending, the first next to BP. An auto the model names vN is at BP-N; a filler _pN after vN; a register
    local takes a free slot. A name is kept where its bucket falls in order, else a letter is put after it."""
    mod = rule.get("mod")
    if not mod or "{" not in text:
        return None
    head, _sep, body = split_body(text)
    lines = body.split("\n")
    decls = []
    for k, l in enumerate(lines[1:], 1):
        m = re.match(r"\s+(register\s+)?((?:unsigned\s+)?(?:int|char|long|short)\b|struct\s+\w+)\s*(\**)\s*(\w+)\s*(?:\[(\d+)\])?;\s*$", l)
        if not m:
            if l.strip():
                break
            continue
        reg, ty, star, name, cnt = m.groups()
        if ty.startswith("struct") and not star:
            return None                                     # (a struct's size the text does not say)
        size = 2 if star else SIZES.get(ty.split()[-1], 2) * int(cnt or 1)
        decls.append(dict(name=name, reg=bool(reg), size=max(size, 2) if not cnt else size + (size & 1)))
    autos = sorted((d for d in decls if not d["reg"] and re.fullmatch(r"v\d+", d["name"])), key=lambda d: int(d["name"][1:]))
    if not autos:
        return None
    fill = {d["name"]: d for d in decls if re.fullmatch(r"_p\d+", d["name"])}
    regs = [d for d in decls if d["reg"]]
    order = []
    drop = set()
    resize = {}
    added = []
    cum = 0
    for d in autos:                                         # (each auto at its place; what lies between: register
        n = int(d["name"][1:])                              #  locals, then a filler)
        gap = n - d["size"] - cum
        took = 0
        while gap >= 2 and regs and not regs_last:
            order.append(regs.pop(0))
            gap -= 2
            took += 1
        if "_p%d" % cum in fill:
            if gap <= 0:                                    # (the register locals take the room: no filler)
                drop.add("_p%d" % cum)
                fill.pop("_p%d" % cum)
            else:
                order.append(fill["_p%d" % cum])
                if took:                                    # (the filler what they leave of it)
                    resize["_p%d" % cum] = gap
        elif gap > 0 and REGSLOTS[0]:                       # (room no local or register takes: a filler there -
            f_ = dict(name="_p%d" % cum, reg=False, size=gap)   #  an unused local of the source's)
            order.append(f_)
            added.append(f_)
        order.append(d)
        cum = n
    order += regs + [fill[f] for f in fill if fill[f] not in order]
    if len(order) > mod:
        return None
    taken = set(re.findall(r"\b[A-Za-z_]\w*\b", text))
    bucket = lambda s: sum(map(ord, s)) % mod
    new = {}
    prev = -1
    for k, d in enumerate(order):
        room = mod - (len(order) - k)                       # (the last bucket this one may take)
        nm = d["name"]
        if not (prev < bucket(nm) <= room):
            want = prev + 1
            nm = next((d["name"] + c for c in "abcdefghijklmnopqrstuvwxyz" if bucket(d["name"] + c) == want and d["name"] + c not in taken), None)
            if nm is None:
                return None
            new[d["name"]] = nm
        prev = bucket(nm)
    if not new and not drop and not resize and not added:
        return None
    if added:                                               # (the fillers made: declared after the others)
        bl = body.split("\n")
        j = 1
        while j < len(bl) and (not bl[j].strip() or re.fullmatch(r"\s+(?:register\s+)?[\w\s*]+?\w+(?:\[\d+\])?;", bl[j])):
            j += 1
        ind_ = re.match(r"\s*", bl[1]).group(0) if len(bl) > 1 and bl[1].strip() else "    "
        bl[j:j] = ["%schar %s[%d];" % (ind_, f_["name"], f_["size"]) for f_ in added]
        body = "\n".join(bl)
    for f in drop:
        body = re.sub(r"(?m)^[ \t]*char %s\[\d+\];\n" % f, "", body)
    for f, z in resize.items():
        body = re.sub(r"(?m)^([ \t]*char %s)\[\d+\];$" % f, lambda m: "%s[%d];" % (m.group(1), z), body)
    for a, b in new.items():
        body = re.sub(r"\b%s\b" % a, b, body)
    return head + "{" + body


def pointer_flow(text):
    """the pointers a function's names are, from those it declares: a name compared with a pointer, given one's
    value, or giving its value to one is one too (a pointer compares without its sign: the code shows which)"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    decl = re.compile(r"(?m)^([ \t]*)(register\s+)?int\s+(\w+);$")
    ptr = set(re.findall(r"(?m)^[ \t]*(?:register\s+)?char\s*\*\s*(\w+);", text))
    if not ptr:
        return text
    ints = set(m.group(3) for m in decl.finditer(text))
    changed = True
    while changed:
        changed = False
        for x in list(ints - ptr):
            for p in ptr:
                st = r"(?:\+\+|--)?"
                if re.search(r"\(\(?%s%s (?:==|!=|<|>|<=|>=) %s%s\)|\(\(?%s%s (?:==|!=|<|>|<=|>=) %s%s\)" % (st, x, st, p, st, p, st, x), body) or \
                        re.search(r"(?m)^[ \t]*%s = %s;|^\s*%s = %s;" % (x, p, p, x), body):   # (the value itself:
                                                                                                  #  not a field of it)
                    ptr.add(x)
                    changed = True
                    break
    new = lambda m: "%s%schar *%s;" % (m.group(1), m.group(2) or "", m.group(3)) if m.group(3) in ptr else m.group(0)
    return decl.sub(new, head) + sep + decl.sub(new, body)


def char_param_variants(text, most=2):
    """the text with one of its int parameters a char (each, at most `most`): one compared with a byte, or given
    one, may be a char the caller widened (a variant: the compiler decides)"""
    head, sep, body = split_body(text)
    out = []
    for m in list(re.finditer(r"(?m)^([ \t]*)int ([\w, ]+);$", head))[:1]:
        names = [x.strip() for x in m.group(2).split(",")]
        for p in names[:most]:
            if not re.search(r"\b%s\b" % p, body):
                continue
            rest = [x for x in names if x != p]
            new = (m.group(1) + "int " + ", ".join(rest) + ";\n" if rest else "") + m.group(1) + "char %s;" % p
            out.append(head[:m.start()] + new + head[m.end():] + sep + body)
        used = [p for p in names if re.search(r"\b%s\b" % p, body)]
        if len(used) > 1:                                   # (and all of them chars)
            rest = [x for x in names if x not in used[:most]]
            new = (m.group(1) + "int " + ", ".join(rest) + ";\n" if rest else "") + m.group(1) + "char %s;" % ", ".join(used[:most])
            out.append(head[:m.start()] + new + head[m.end():] + sep + body)
    return out


def pointer_widths(text, most=2):
    """the text with its register pointers (`register char *p;`, at most `most` of them) made int * - each, then
    all: what the model read through them may be words (the text alone does not say)"""
    ps = re.findall(r"(?m)^[ \t]*register char \*(\w+);", text)[:most]
    out = []

    def words(t, p):                                        # (its steps in bytes now in words)
        t = re.sub(r"(?m)^([ \t]*)register char \*%s;" % p, r"\1register int *%s;" % p, t)
        return re.sub(r"\b%s = %s ([+-]) (\d+);" % (p, p), lambda m: "%s = %s %s %d;" % (p, p, m.group(1), int(m.group(2)) // 2)
                      if int(m.group(2)) % 2 == 0 else m.group(0), t)
    for p in ps:
        out.append(words(text, p))
    if len(ps) > 1:
        t = text
        for p in ps:
            t = words(t, p)
        out.append(t)
    return out


def pre_steps(text):
    """X--; (X++; X = X - 1;) then a test of X: the step in the test - if (--X ..) - a variant: the compiler
    tests the flags of the step there, not a compare"""
    lines = text.split("\n")
    changed = False
    k = 0
    while k + 1 < len(lines):
        m = re.match(r"\s*(\w+(?:->f\d+)?)(\+\+|--);\s*$", lines[k]) or re.match(r"\s*(\w+) = \1 ([+-]) [12];\s*$", lines[k])
        # (by 2: a pointer to words stepped once - declared so where the compiler asks)
        if m:
            x = m.group(1)
            op = m.group(2)
            op = "++" if op in ("++", "+") else "--"
            t = re.match(r"(\s*(?:if|while) \(\(?)(%s)\b(.*)$" % re.escape(x), lines[k + 1])
            if t:
                lines[k + 1] = t.group(1) + op + x + t.group(3)
                lines[k] = None
                changed = True
                k += 2
                continue
            nxt = lines[k + 1]                              # (or read once by the next statement: --x in place)
            if len(re.findall(r"\b%s\b" % re.escape(x), nxt)) == 1 and not re.match(r"\s*%s\s*=" % re.escape(x), nxt) \
                    and not re.match(r"\s*(?:\}|else\b|L_\w+:)", nxt):
                lines[k + 1] = re.sub(r"\b%s\b" % re.escape(x), op + x, nxt, count=1)
                lines[k] = None
                changed = True
                k += 2
                continue
        k += 1
    return "\n".join(l for l in lines if l is not None) if changed else None


def address_variants(text, dsize, most=2, lo=0x100):
    """a number a pointer is compared with, or given (`p < 14708`, `p = 14708`): the place of data there - an array
    at that offset, g_XXXX (a variant each: the compiler decides; a pointer compares without its sign)"""
    ptrs = set(re.findall(r"(?m)^[ \t]*(?:register\s+)?(?:char|int|struct \w+)\s*\*\s*(\w+);", text))
    if not ptrs:
        return []
    nums = []
    for m in re.finditer(r"\b(%s)\s*(?:[<>]=?|==|!=|=)\s*(-?\d{3,5})\b" % "|".join(map(re.escape, ptrs)), text):
        n = int(m.group(2)) & 0xFFFF                        # (a place above 32K the model writes negative)
        if lo <= n < dsize and (n, m.group(2)) not in nums:
            nums.append((n, m.group(2)))
    out = []
    for n, lit in nums[:most]:
        name = "g_%04x" % n
        out.append((name, re.sub(r"(?<![\w.])%s(?![\w.])" % re.escape(lit), name, text)))
    return out


def word_arrays(text, lo=0x40):
    """(E << 1)->fN, N a place in the data (not a field's small offset): an array of words there indexed by E -
    g_XXXX[E] (the model reads `mov bx,E / shl bx,1 / [bx+N]` as a field of a pointer)"""
    rx4 = r"\(\(([A-Za-z_]\w*(?:->f\d+)*) << 1\) << 1\)->f(\d+)\b|\(([A-Za-z_]\w*(?:->f\d+)*) << 2\)->f(\d+)\b"
    at4 = {}                                                # (the places read through an index times 4: the lines)
    ls_ = text.split("\n")
    for k_, l_ in enumerate(ls_):
        for m_ in re.finditer(rx4, l_):
            at4.setdefault(int(m_.group(2) or m_.group(4)), set()).add(k_)
    halves = lambda x, y: x == y or abs(x - y) == 1 and all(re.match(r"\s*(?:ax|bx|cx|dx) = ", ls_[z]) for z in (x, y))
    near = lambda a, b: any(halves(x, y) for x in at4.get(a, ()) for y in at4.get(b, ()))   # (a long's two words together)
    pair4 = {n for n in at4 if (n + 2 in at4 and not near(n, n + 2)) or (n - 2 in at4 and not near(n, n - 2))}

    def longs(m):                                           # (an index doubled twice: an array of longs there - two
        n = int(m.group(2))                                 #  words 2 apart read on their own: of 4-byte structs)
        if n < lo:
            return m.group(0)
        name = "g_%04x" % n
        ln_ = m.string[m.string.rfind("\n", 0, m.start()) + 1:m.string.find("\n", m.end()) if "\n" in m.string[m.end():] else len(m.string)]
        word = re.fullmatch(r"\s*(?:si|di|bx|cx) = %s;" % re.escape(m.group(0)), ln_) or \
            re.search(r"\w\((?:[^()]*, )?%s(?:, [^()]*)?\)" % re.escape(m.group(0)), ln_) and not re.search(r"\bLONG\(", ln_)
        if n in pair4 or word:                              # (one word of it loaded into a pointer register, or
                                                            #  pushed alone as an argument)
            STRUCTARR.setdefault(name, (4, "int"))
            ARRAYS[name] = "struct s_%s" % name
            return "%s[%s].f0" % (name, m.group(1))
        ARRAYS[name] = "long"
        return "%s[%s]" % (name, m.group(1))
    text = re.sub(r"\(\(([A-Za-z_]\w*(?:->f\d+)*) << 1\) << 1\)->f(\d+)\b", longs, text)
    text = re.sub(r"\(([A-Za-z_]\w*(?:->f\d+)*) << 2\)->f(\d+)\b", longs, text)
    rx = re.compile(r"\(((?:(?:\+\+|--)?[A-Za-z_]\w*(?:->f\d+)*(?:\+\+|--)?|\d+)(?:\s*[-+]\s*\d+)?) << 1\)->f(\d+)\b")

    def one(m):
        n = int(m.group(2))
        if n < lo:
            return m.group(0)
        name = "g_%04x" % n
        ARRAYS[name] = "int"
        return "%s[%s]" % (name, m.group(1))
    t = rx.sub(one, text)

    def paren(m):                                           # (an expression in parentheses the index: itself)
        n = int(m.group(2))
        if n < lo:
            return m.group(0)
        name = "g_%04x" % n
        ARRAYS[name] = "int"
        return "%s[%s]" % (name, m.group(1)[1:-1])
    t = re.sub(r"\((\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)) << 1\)->f(\d+)\b", paren, t)

    def byte_index(m):                                      # (a scratch register's value plus a place in the data:
        n = int(m.group(2))                                 #  an array of bytes there, the value its index)
        if n < 0x100:
            return m.group(0)
        name = "g_%04x" % n
        ARRAYS[name] = "char" if ("data", n) in BYTEPTR else "int"
        return "%s[%s]" % (name, m.group(1))
    t = re.sub(r"\b(ax|bx|cx|dx)->f(\d+)\b", byte_index, t)
    for g in set(re.findall(r"\bg_([0-9a-f]+)\[", t)):     # (an element of the data the model named: an array)
        if int(g, 16) >= 0x40:
            ARRAYS.setdefault("g_%04x" % int(g, 16), "char" if ("data", int(g, 16)) in BYTEPTR or ("*", int(g, 16)) in BYTEPTR else "int")
    t = re.sub(r"\(((?:\+\+|--)?[A-Za-z_]\w*(?:\+\+|--)?)\)->f(\d+)\b", byte_index, t)   # (a variable's value -
                                                                                       #  stepped too - the index)
    # (a register given K * E, then an element at a place in the data through it: an array of structs of K
    #  bytes - g_N[E].f0, the struct laid out in localize)
    def struct_index(m):
        ind, k, e, rest = m.group(1), int(m.group(2)), m.group(3), m.group(4)
        g = re.search(r"\b(g_[0-9a-f]{4})\[bx\]", rest)
        if not g or k < 2 or k > 0x400:
            return m.group(0)
        STRUCTARR[g.group(1)] = (k, ARRAYS.get(g.group(1), "int"))
        ARRAYS[g.group(1)] = "struct s_%s" % g.group(1)
        return "%s%s" % (ind, rest.replace("%s[bx]" % g.group(1), "%s[%s].f0" % (g.group(1), e), 1))
    t = re.sub(r"(?m)^([ \t]*)bx = \((\d+) \* (.+?)\);\n[ \t]*(.*\bg_[0-9a-f]{4}\[bx\].*)$", struct_index, t)

    def struct_place(m):                                    # (K * E + N, N a place in the data: the address of
        k, e, n = int(m.group(1)), m.group(2), int(m.group(3))   #  the element E of an array of K-byte structs)
        if n < 0x100 or k < 3 or k > 0x400:
            return m.group(0)
        name = "g_%04x" % n
        STRUCTARR.setdefault(name, (k, "int"))
        ARRAYS[name] = "struct s_%s" % name
        return "&%s[%s]" % (name, e)
    t = re.sub(r"\(\((\d+) \* ([\w\->.]+)\) \+ (\d+)\)(?!->)", struct_place, t)

    ls_ = t.split("\n")                                     # (a register given K * E read only at places in the
    for k_, l_ in enumerate(ls_):                           #  data through it until given again: the product there)
        m_ = re.fullmatch(r"([ \t]*)(bx|si|di) = \((\d+) \* ([A-Za-z_]\w*)\);", l_)
        if not m_ or int(m_.group(3)) < 2:
            continue
        r_, e_ = m_.group(2), m_.group(4)
        uses, j_ = [], k_ + 1
        while j_ < len(ls_):
            y_ = ls_[j_]
            if re.match(r"\s*(?:%s|%s) = |\s*(?:L_\w+:|\}|case\b|default:)" % (r_, re.escape(e_)), y_) or re.search(r"\b%s\s*(?:\+\+|--)" % re.escape(e_), y_):
                break
            if re.search(r"\b%s\b" % r_, y_):
                uses.append(j_)
            j_ += 1
        if r_ == "bx":                                      # (an element of words through it: the place there)
            uses_ = [re.sub(r"\bg_([0-9a-f]{4})\[bx\]", lambda q: "bx->f%d" % int(q.group(1), 16), ls_[u]) for u in uses]
            if all(not re.search(r"\bbx\b(?!->f(?:\d{3,}))", x) for x in uses_):
                for u, x in zip(uses, uses_):
                    ls_[u] = x
        if not uses or not all(not re.search(r"\b%s\b(?!->f(?:\d{3,}))" % r_, ls_[u]) for u in uses) or \
           any(int(n_) < 0x100 for u in uses for n_ in re.findall(r"\b%s->f(\d+)" % r_, ls_[u])) or \
           _live_after(ls_, uses[-1], r_):
            continue
        for u in uses:
            ls_[u] = re.sub(r"\b%s->f(\d+)" % r_, lambda q: "(%s * %s)->f%s" % (m_.group(3), e_, q.group(1)), ls_[u])
        ls_[k_] = None
    t = "\n".join(x for x in ls_ if x is not None)

    def struct_product(m):                                  # (the product folded in place: (K * E)->fN)
        k, e, n = int(m.group(1)), m.group(2), int(m.group(3))
        if n < 0x100 or k < 2 or k > 0x400:
            return m.group(0)
        name = "g_%04x" % n
        STRUCTARR.setdefault(name, (k, "char" if ("data", n) in BYTEPTR else "int"))
        ARRAYS[name] = "struct s_%s" % name
        return "%s[%s].f0" % (name, e)
    t = re.sub(r"\((\d+) \* ([A-Za-z_][\w\->]*)\)->f(\d+)\b", struct_product, t)
    # (the compiler keeps bx on: later elements through it, until it is given again - and the product itself
    #  read as a pointer to a field, (K * E)->fN, an element at N)
    lines = t.split("\n")
    for k, l in enumerate(lines):
        m = re.match(r"[ \t]*(\w+)\[(.+?)\]\.f0\b", l) if "].f0" in l else None
        prod = re.search(r"\b(g_[0-9a-f]{4})\[(.+?)\]\.f0", l)
        if not prod or prod.group(1) not in STRUCTARR:
            continue
        kk, e = STRUCTARR[prod.group(1)][0], prod.group(2)
        for j in range(k + 1, len(lines)):
            if re.match(r"[ \t]*bx = ", lines[j]) or re.search(r"\b%s\s*(?:=[^=]|\+\+|--)|(?:\+\+|--)\s*%s\b" % (re.escape(e), re.escape(e)), lines[j]):
                break
            def elem(m2):
                STRUCTARR.setdefault(m2.group(1), (kk, ARRAYS.get(m2.group(1), "int") if not str(ARRAYS.get(m2.group(1), "")).startswith("struct") else "int"))
                ARRAYS[m2.group(1)] = "struct s_%s" % m2.group(1)
                return "%s[%s].f0" % (m2.group(1), e)
            lines[j] = re.sub(r"\b(g_[0-9a-f]{4})\[bx\]", elem, lines[j])
            lines[j] = re.sub(r"\(%d \* %s\)->f(\d+)\b" % (kk, re.escape(e)), lambda m2: elem(type("M", (), {"group": lambda s, i: "g_%04x" % int(m2.group(1))})()), lines[j])
    t = "\n".join(lines)
    # (I << 1)[E]: the model's word index into the words at E - ((int *)(E))[I]
    def word_index(m):                                      # (an offset in bytes: through a char *)
        i, e = m.group(1), m.group(2)
        o = re.fullmatch(r"([A-Za-z_]\w*) \+ (\d+)", e)
        return "((int *)((char *)%s + %s))[%s]" % (o.group(1), o.group(2), i) if o else "((int *)(%s))[%s]" % (e, i)
    t = re.sub(r"\(((?:[A-Za-z_][\w\->]*|\d+|\((?:[^()]|\([^()]*\))*\))(?:\s*[-+]\s*\d+)?) << 1\)\[([^\[\]]+)\]", word_index, t)
    return t


def pre_increments(text):
    """X = X + n; then *X read once: *++X in its place - X a pointer to things of n bytes (a variant)"""
    lines = text.split("\n")
    changed = False
    for k in range(len(lines) - 1):
        m = re.match(r"\s*(\w+) = \1 \+ ([12]);\s*$", lines[k] or "")
        if not m:
            continue
        x, n = m.group(1), int(m.group(2))
        nxt = lines[k + 1]
        if nxt is None or len(re.findall(r"\b%s\b" % x, nxt)) != 1 or not re.search(r"\*%s\b(?!\s*(?:->|\[|\+\+|--))" % x, nxt):
            continue
        lines[k + 1] = re.sub(r"\*%s\b" % x, "*++%s" % x, nxt, count=1)
        lines[k] = None
        if n == 2:
            PTRSIZE[x] = 2
            lines = [re.sub(r"^(\s*register )char \*%s;" % x, r"\1int *%s;" % x, l) if l else l for l in lines]
        changed = True
    return "\n".join(l for l in lines if l is not None) if changed else None


def header_text(prj, header):
    """a header of the kit, as text ('' when the kit has none)"""
    from codebreaker.exact import toolchain as tcm
    try:
        root = tcm.kit_dir(prj.toolchain().p["kit"])
    except Exception:
        return ""
    for d, _, fs in os.walk(root):
        for f in fs:
            if f.lower() == header.lower():
                return open(os.path.join(d, f), encoding="latin1").read()
    return ""


def table_macros(prj, text, specs):
    """the macros of a header that read a table of the library - ctype.h's isdigit(c) .. ( (_ctype_+1)[c] & _DIGIT ) -
    read back where the model has the table's place: (c->fN & M), N the table's place + its offset, M a macro's
    mask (the profile's "table_macros": [{"header": "ctype.h"}]: the macros and masks are the kit's header's, the
    table's place the library stage's)"""
    for sp in specs or []:
        key = (prj.dir, sp["header"])
        if key not in _TABLES:
            h = header_text(prj, sp["header"])
            masks = {n: int(v, 0) for n, v in re.findall(r"#define\s+(_[A-Z]+)\s+(0x[0-9a-fA-F]+|\d+)", h)}
            macs = []
            for name, arg, tab, off, expr in re.findall(r"#define\s+(\w+)\((\w)\)\s*\(\s*\((\w+)\+(\d+)\)\[\2\]\s*&\s*([^)]*\)?)\s*\)", h):
                bits = re.findall(r"_[A-Z]+", expr)
                if bits and all(b in masks for b in bits):
                    m = 0
                    for b in bits:
                        m |= masks[b]
                    macs.append((name, tab, int(off), m))
            if prj.dir not in _LIBDATA:
                data_macros(prj, "", [{"base": "-", "size": 1, "names": []}])
            _TABLES[key] = macs
        hit = False
        for name, tab, off, mask in _TABLES[key]:
            base = _LIBDATA.get(prj.dir, {}).get("_" + tab) or _LIBDATA.get(prj.dir, {}).get(tab)
            if base is None:
                continue
            n = base + off
            for form, fmt in ((r"\(\((\w+)->f%d & %d\) != 0\)" % (n, mask), "%s(%%s)" % name),
                              (r"\(\((\w+)->f%d & %d\) == 0\)" % (n, mask), "!%s(%%s)" % name),
                              (r"\((\w+)->f%d & %d\)" % (n, mask), "%s(%%s)" % name)):
                new = re.sub(form, lambda m: fmt % m.group(1), text)
                if new != text:
                    hit, text = True, new
        if hit:
            FILEPTR.setdefault("#include", sp["header"])
            FILEPTR["#include2"] = sp["header"]
    return text


def header_names(prj, header):
    """the functions and macros a header of the kit declares (stdio.h's fopen, freopen, getc ..): those come from
    the header, the draft does not declare them again"""
    key = (prj.toolchain().name, header.lower())
    if key not in _HEADERS:
        from codebreaker.exact import toolchain as tcm
        names = set()
        try:
            root = tcm.kit_dir(prj.toolchain().p["kit"])
            for d, _, fs in os.walk(root):
                for f in fs:
                    if f.lower() == header.lower():
                        t = open(os.path.join(d, f), encoding="latin1").read()
                        names |= set(re.findall(r"\b([A-Za-z_]\w*)\s*\(\s*\)\s*[;,]", t))
                        names |= set(re.findall(r"#define\s+([A-Za-z_]\w*)\s*\(", t))
                        break
                if names:
                    break
        except Exception:
            pass
        _HEADERS[key] = names
    return _HEADERS[key]


def data_macros(prj, text, specs):
    """a library's data a macro names - stdio's streams: stdin, stdout, stderr .. the elements of _iob, fileno(f)
    a byte of one - read back where the model has their places (g_XXXX, or a number passed: a stream's place)
    (the profile's "data_macros": [{"base": "__iob", "size": 8, "names": [..], "fields": {"7": "fileno(%s)"},
    "include": "stdio.h"}]; the base's place from the library stage)"""
    if not specs:
        return text
    if prj.dir not in _LIBDATA:
        try:
            from codebreaker.exact import library
            mp = library.locate(prj)
            places = {n: a for n, (t, a) in mp.allrefs.items() if t == "D"}
            for p in mp.coded + mp.donly:                   # (and the placed modules' own data names: a table
                if p.data is None:                          #  the macros read in line, which no code calls)
                    continue
                for n, s, off in p.m.o.pubs:
                    if s and s.upper() in ("_DATA", "DATA", "CONST"):
                        places.setdefault(n, p.data + off)
            _LIBDATA[prj.dir] = places
        except Exception:
            _LIBDATA[prj.dir] = {}
    places = _LIBDATA[prj.dir]
    for sp in specs:
        base = places.get(sp["base"])
        if base is None:
            continue
        n, size = len(sp["names"]), sp["size"]
        hit = [False]

        def field(m):
            a = int(m.group(1), 16)
            k, off = divmod(a - base, size)
            if 0 <= k < n and str(off) in sp.get("fields", {}):
                hit[0] = True
                return sp["fields"][str(off)] % sp["names"][k]
            return m.group(0)
        text = re.sub(r"\bg_([0-9a-f]{4})\b", field, text)

        def stream(m):                                      # (a number passed that is a stream's place)
            a = int(m.group(2))
            k, off = divmod(a - base, size)
            if 0 <= k < n and off == 0:
                hit[0] = True
                return m.group(1) + sp["names"][k]
            return m.group(0)
        text = re.sub(r"([(,]\s*)(\d{3,5})(?=\s*[,)])", stream, text)
        if hit[0] and sp.get("include"):
            FILEPTR["#include"] = sp["include"]
    return text


def unused_locals(text):
    """the locals declared in the function's body that nothing in it names any more, left out"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    lines = body.split("\n")
    drop = set()
    for k, l in enumerate(lines):
        m = re.match(r"\s*(?:register\s+)?(?:unsigned\s+)?(?:int|char|long|short|struct \w+)\s*\**\s*(\w+)\s*(?:\[\d+\])?;\s*$", l)
        if m and not re.search(r"\b%s\b" % m.group(1), "\n".join(lines[:k] + lines[k + 1:])):
            drop.add(k)
    return head + sep + "\n".join(l for k, l in enumerate(lines) if k not in drop)


def library_macros(text, macros):
    """a library macro the compiler expanded in line - stdio's getc(f) and putc(c, f): the count of the stream's
    buffer stepped, then its pointer, else the routine that fills or empties the buffer (the profile's "macros":
    [{"call": routine, "as": "getc(%s)", "type": "FILE", "include": "stdio.h"}]) - read back as the macro: from
    the line that steps the stream's count to the routine's call, the region is the macro's value"""
    if not macros:
        return text
    for mac in macros:
        rx = re.compile(r"\b%s\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)" % re.escape(mac["call"]))
        while True:
            lines = text.split("\n")
            hit = next(((k, m) for k, l in enumerate(lines) for m in [rx.search(l)] if m), None)
            if not hit:
                break
            k1, m = hit
            args = [x.strip() for x in _top_commas(m.group(1))]
            f = args[-1]
            k0 = next((j for j in range(k1, -1, -1) if re.search(r"\b%s->f2(?:--| = %s->f2 - 1)" % (re.escape(f), re.escape(f)), lines[j])), None)
            if k0 is None:
                break
            for ix in mac.get("byte_args", []):             # (an argument the macro stores as a byte - putc's c:
                p_ = re.fullmatch(r"\*(\w+)", args[ix]) if ix < len(args) else None   # *p of an int *p read
                if p_ and re.search(r"(?m)^[ \t]*(?:register )?int \*%s;$" % p_.group(1), text):   # a byte at a time)
                    args[ix] = "*(char *)%s" % p_.group(1)
            val = mac["as"] % tuple(args)
            k2 = k1
            depth = sum(l.count("{") - l.count("}") for l in lines[k0:k2 + 1])
            while depth > 0 and k2 + 1 < len(lines):        # (to the region's own closing brace)
                k2 += 1
                depth += lines[k2].count("{") - lines[k2].count("}")
            ind = re.match(r"\s*", lines[k0]).group(0)
            r = re.match(r"\s*return\b", lines[k1])
            new = ["%sreturn %s;" % (ind, val)] if r else ["%sax = %s;" % (ind, val)]
            if depth < 0:
                new.append(ind + "}")
            lines[k0:k2 + 1] = new
            text = "\n".join(lines)
            FILEPTR[f] = mac.get("type", "FILE")
            if mac.get("include"):                          # (a macro without a header leaves the one an earlier gave)
                FILEPTR["#include"] = mac["include"]
    return text


def and_returns(text):
    """if (A) return E; return 0;  ->  return A && E;  (a variant: the compiler makes one test chain of the &&)"""
    rx = re.compile(r"(?m)^([ \t]*)if (\(.*\))(?: \{)?\n\s*return (.+);\n(?:\s*\}\n)?\s*return 0;$")

    def one(m):
        e = m.group(3)
        if re.search(r"\|\|", e) and not (e.startswith("(") and e.endswith(")")):
            e = "(%s)" % e
        return "%sreturn %s && %s;" % (m.group(1), _truth(m.group(2)), e)
    t = rx.sub(one, text)
    return t if t != text else None


def word_constants(text):
    """a word the model reads as 32768..65535: a decimal constant over 32767 is a long to a 16-bit compiler (the
    compare or the sum goes to 32 bits) - the int it was, negative (65535 is -1)"""
    return re.sub(r"(?<![\w.])(3276[89]|327[7-9]\d|32[89]\d\d|3[3-9]\d{3}|[4-5]\d{4}|6[0-4]\d{3}|65[0-4]\d\d|655[0-2]\d|6553[0-5])(?![\w.])",
                  lambda m: str(int(m.group(1)) - 65536), text)


def unsigned_globals(text, most=2):
    """the text with one of its int globals unsigned (each): a variable compared, the compiler's code says how"""
    out = []
    m = re.search(r"(?m)^extern int ([\w, ]+);$", text)
    if not m:
        return out
    names = [x.strip() for x in m.group(1).split(",")]
    for g in names[:most]:
        rest = [x for x in names if x != g]
        new = ("extern int %s;\n" % ", ".join(rest) if rest else "") + "extern unsigned %s;" % g
        out.append(text[:m.start()] + new + text[m.end():])
    return out


def assign_in_tests(text):
    """X = E; then a test of X: the assignment in the test - if ((X = E) == K) - a variant: the compiler
    compares the value it has, not X again"""
    lines = text.split("\n")
    changed = False
    for k in range(len(lines) - 1):
        m = re.match(r"(\s*)([A-Za-z_]\w*(?:\[[^\]]*\])?) = ([^;]+);\s*$", lines[k] or "")
        if not m or re.match(r"\s*(?:if|while|return)\b", lines[k]):
            continue
        x, e = m.group(2), m.group(3)
        t = re.match(r"(\s*if \(\(?)(%s)\b(.*)$" % re.escape(x), lines[k + 1] or "")   # (an if's: a loop's test
                                                                                         #  would give it again each time)
        if t and not re.search(r"\b%s\b" % re.escape(x), e):
            lines[k + 1] = "%s(%s = %s)%s" % (t.group(1), x, e, t.group(3))
            lines[k] = None
            changed = True
    return "\n".join(l for l in lines if l is not None) if changed else None


def char_params(text):
    """a parameter the model only reads into a byte register (al = b) is a char: declared so (K&R widens it
    in the call; the function reads its low byte)"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    params = re.findall(r"\b\w+\(([\w, ]*)\)", head)
    params = [p.strip() for p in (params[-1].split(",") if params else []) if p.strip()]
    chars = [p for p in params if re.search(r"\b(?:al|bl|cl|dl) = %s;" % p, body) and
             not re.search(r"\b(?:ax|bx|cx|dx|si|di) = %s;" % p, body) and not re.search(r"\b%s\s*(?:->|\[)" % p, body)]
    if not chars:
        return text
    lines = head.split("\n")
    for j, l in enumerate(lines):
        d = re.match(r"(\s*)int (.*);$", l)
        if d:
            names = [x.strip() for x in d.group(2).split(",")]
            keep = [x for x in names if x not in chars]
            lines[j] = (d.group(1) + "int " + ", ".join(keep) + ";\n" if keep else "") + \
                d.group(1) + "char " + ", ".join(x for x in names if x in chars) + ";" if any(x in chars for x in names) else l
    return "\n".join(lines) + sep + body


def loop_gotos(text):
    """the gotos back to a loop's top: a move shared at the label (`ax = E; L: for (;;) { r = ax;` - every
    `ax = V; goto L;` too) given in each place (r = E; r = V), a for (;;) left holding one while and a break that
    while, and a goto to the top of the loop it is in a continue"""
    for _ in range(4):
        lines = text.split("\n")
        done = False
        for k, l in enumerate(lines):
            m = re.match(r"([ \t]*)(L_\w+):$", l)
            if not m:
                continue
            lab = m.group(2)
            gos = [j for j, x in enumerate(lines) if x.strip() == "goto %s;" % lab]
            # (the move shared at the label: into each place that comes to it)
            j = k + 1
            if j < len(lines) and lines[j].strip() == "for (;;) {":
                j += 1
            mv = re.match(r"[ \t]*(\w+) = ax;$", lines[j]) if j < len(lines) else None
            if mv and k and re.match(r"[ \t]*ax = [^;]+;$", lines[k - 1]) and gos and \
                    all(g and re.match(r"[ \t]*ax = [^;]+;$", lines[g - 1]) for g in gos) and \
                    not _live_after(lines, j, "ax"):
                r = mv.group(1)
                for g in [k] + gos:
                    lines[g - 1] = re.sub(r"^([ \t]*)ax = ", r"\1%s = " % r, lines[g - 1])
                lines[j] = None
                text = "\n".join(x for x in lines if x is not None)
                done = True
                break
            # (a for (;;) holding one while and a break: the while)
            if j < len(lines) and lines[k + 1].strip() == "for (;;) {":
                e = _block_end(lines, k + 1)
                w = k + 2
                if e and re.match(r"[ \t]*while \(.*\) \{$", lines[w]):
                    we = _block_end(lines, w)
                    if we and we + 2 == e and lines[we + 1].strip() == "break;":
                        lines[w:we + 1] = [x[4:] if x.startswith("    ") else x for x in lines[w:we + 1]]
                        lines[k + 1] = None
                        lines[we + 1] = None
                        lines[e] = None
                        text = "\n".join(x for x in lines if x is not None)
                        done = True
                        break
            # (a goto to the top of the loop it is in: a continue)
            if k + 1 < len(lines) and re.match(r"[ \t]*(?:while \(.*\)|for \(.*\)) \{$", lines[k + 1]) and gos:
                e = _block_end(lines, k + 1)
                if e and all(k + 1 < g < e for g in gos):
                    ok = True
                    for g in gos:                           # (not inside a loop of its own)
                        for q in range(k + 2, g):
                            x = lines[q]
                            if re.match(r"[ \t]*(?:while \(.*\)|for \(.*\)|do) \{$", x):
                                qe = _block_end(lines, q)
                                if qe and q < g < qe:
                                    ok = False
                    if ok:
                        for g in gos:
                            lines[g] = lines[g].replace("goto %s;" % lab, "continue;")
                        lines[k] = None
                        text = "\n".join(x for x in lines if x is not None)
                        done = True
                        break
        if not done:
            break
    return text


def default_last(text):
    """a switch's default arm, written first, moved to the end (a variant: the compiler puts the default's code
    after the tests wherever it is written, so the code alone does not say where)"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.fullmatch(r"(\s*)switch \(.*\) \{", l)
        if not m or k + 1 >= len(lines) or lines[k + 1] != m.group(1) + "default:":
            continue
        ind = m.group(1)
        nxt = next((j for j in range(k + 2, len(lines)) if re.fullmatch(r"%s(?:case [^:]+|default):" % re.escape(ind), lines[j])), None)
        end = next((j for j in range(k + 1, len(lines)) if lines[j] == ind + "}"), None)
        if nxt is None or end is None or nxt > end:
            continue
        arm = lines[k + 1:nxt]
        if not re.match(r"\s*(?:break;|return\b|goto\b)", [x for x in arm if x.strip()][-1]):
            continue                                        # (it falls into the next: not movable)
        return "\n".join(lines[:k + 1] + lines[nxt:end] + arm + lines[end:])
    return None


def empty_cases_first(text):
    """a switch's arms that only break (case K: break;) moved to its top (a variant: such an arm has no code of
    its own - where it is written decides the layout of the others)"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.fullmatch(r"(\s*)switch \(.*\) \{", l)
        if not m:
            continue
        ind = m.group(1)
        end = next((j for j in range(k + 1, len(lines)) if lines[j] == ind + "}"), None)
        if end is None:
            continue
        moved, rest, j = [], [], k + 1
        while j < end:
            if re.fullmatch(r"%scase [^:]+:" % re.escape(ind), lines[j]) and j + 1 < end and lines[j + 1].strip() == "break;" and \
                    (j == k + 1 or re.match(r"\s*(?:break;|return\b|goto\b|continue;)", lines[j - 1]) or lines[j - 1].strip().endswith("}")) and \
                    j > k + 1:
                moved += lines[j:j + 2]
                j += 2
                continue
            rest.append(lines[j])
            j += 1
        if moved:
            return "\n".join(lines[:k + 1] + moved + rest + lines[end:])
    return None


def or_loops(text):
    """for (;;) { if (A) { L: S; continue; } if (B) { goto L; } else { break; } } - a test or-ed with the first:
    while (A || B) { S } (a variant)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        m = re.fullmatch(r"([ \t]*)for \(;;\) \{", lines[k])
        e = _block_end(lines, k) if m else None
        if e is None or lines[e].strip() != "}":
            continue
        ind = m.group(1)
        i4 = ind + "    "
        p1 = _if_parts(lines, k + 1)
        if not p1 or p1[2] is not None:
            continue
        arm = p1[1]
        lab = re.fullmatch(r"[ \t]*(L_\w+):", arm[0]) if arm else None
        if not lab or arm[-1].strip() != "continue;":
            continue
        p2 = _if_parts(lines, p1[3])
        if not p2 or p2[3] != e or [x.strip() for x in p2[1] if x.strip()] != ["goto %s;" % lab.group(1)] or \
                [x.strip() for x in (p2[2] or []) if x.strip()] != ["break;"]:
            continue
        if re.search(r"\bgoto %s;" % lab.group(1), "\n".join(lines[:k] + lines[e + 1:])):
            continue
        a, b = _truth("(%s)" % p1[0]), _truth("(%s)" % p2[0])
        body = [x[4:] if x.startswith(i4 + "    ") else x for x in arm[1:-1]]
        new = ["%swhile (%s || %s) {" % (ind, a, b)] + body + ["%s}" % ind]
        return "\n".join(lines[:k] + new + lines[e + 1:])
    return None


def compare_results(text):
    """((A op B) ? 1 : 0) - a comparison's value: (A op B) itself, ? 0 : 1 its negation, and a call's side on the
    right (B mirror-op A) when the call was A (a variant: the compiler calls first whichever side it is on)"""
    mirror = {"<": ">", ">": "<", "<=": ">=", ">=": "<=", "==": "==", "!=": "!="}
    neg = {"<": ">=", ">=": "<", ">": "<=", "<=": ">", "==": "!=", "!=": "=="}

    def one(m):
        a, op, b, t, f = m.group(1), m.group(2), m.group(3), m.group(4), m.group(5)
        if not (_balanced(a) and _balanced(b)) or t == f:
            return m.group(0)
        if t == "0":
            op = neg[op]
        if re.search(r"\w\(", a) and not re.search(r"\w\(", b):
            a, b, op = b, a, mirror[op]
        return "(%s %s %s)" % (a, op, b)
    t = re.sub(r"\(\(([^?:\n]+?) (==|!=|<=|>=|<|>) ([^?:\n]+?)\) \? ([01]) : ([01])\)", one, text)
    return t if t != text else None


def register_reads(text):
    """r = E; S(E->f0 ..) - the next statement reading E again where the compiler had it in r: through r (*r, r->fN)
    (a variant: the source read it through the variable, as switch ((k = n->left)->op))"""
    lines = text.split("\n")
    for k in range(len(lines) - 1):
        m = re.fullmatch(r"([ \t]*)(r_\w+) = (\w+(?:->f\d+)+);", lines[k])
        if not m:
            continue
        r, e = m.group(2), m.group(3)
        nxt = lines[k + 1]
        if not re.search(re.escape(e) + r"\b", nxt):
            continue
        st = re.search(r"(?m)^[ \t]*register struct \w+ \*%s;$" % r, text)

        def via(mm):
            if mm.group(1) is None:
                return r                                    # (the value itself: the register)
            n = int(mm.group(1))
            return "%s->f%d" % (r, n) if st else "*%s" % r if n == 0 else mm.group(0)
        ind = len(lines[k]) - len(lines[k].lstrip())
        base = re.match(r"\w+", e).group(0)
        changed = False
        for j in range(k + 1, len(lines)):                  # (on through the block: until r or E's pointer is given
            x = lines[j]                                    #  again, or a call could change E)
            if x.strip() and len(x) - len(x.lstrip()) < ind:
                break
            new = re.sub(re.escape(e) + r"(?:->f(\d+))?\b(?!->)", via, x)
            if new != x:
                lines[j] = new
                changed = True
            if re.match(r"[ \t]*(?:%s|%s) = " % (r, base), x):
                break
        if changed:
            return "\n".join(lines)
    return None


def switch_breaks(text):
    """a switch last in a loop's body whose arms go on with `continue;`: break (the same - the loop's end), and a
    default that only goes on gone (a variant: which the source wrote decides the layout)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        m = re.fullmatch(r"([ \t]*)(?:while \(.*\)|for \(.*\)) \{", lines[k])
        e = _block_end(lines, k) if m else None
        if e is None:
            continue
        i4 = m.group(1) + "    "
        sw = next((j for j in range(e - 1, k, -1) if lines[j].strip()), None)
        if sw is None or lines[sw] != i4 + "}":
            continue
        st = next((j for j in range(sw - 1, k, -1) if lines[j].startswith(i4 + "switch (") and lines[j].endswith("{")), None)
        if st is None or any(lines[j].strip() == "continue;" and len(lines[j]) - len(lines[j].lstrip()) > len(i4) + 4
                             for j in range(st, sw)):
            continue
        new = list(lines)
        changed = False
        for j in range(st + 1, sw):
            if new[j] == i4 + "    continue;":
                new[j] = i4 + "    break;"
                changed = True
        j = st + 1
        while j < sw:                                       # (a default that only breaks: none)
            if new[j] == i4 + "default:" and j + 1 < sw and new[j + 1] == i4 + "    break;" and \
                    (j + 2 >= sw or re.match(r"\s*(?:case [^:]+|default):$", new[j + 2])):
                new[j] = new[j + 1] = None
                changed = True
                j += 2
                continue
            j += 1
        if changed:
            return "\n".join(x for x in new if x is not None)
    return None


def default_after(text):
    """switch (..) { .. default: S; case K: return V; } - the default falling into a case that only returns: the
    case written first, S after the switch with its own return V (a variant: the source's layout of both)"""
    for m in re.finditer(r"(?m)^([ \t]*)switch \(.*\) \{\n", text):
        ind = m.group(1)
        end = next((j for j in re.finditer(r"(?m)^%s\}$" % re.escape(ind), text) if j.start() > m.end()), None)
        if end is None:
            continue
        body = text[m.end():end.start()]
        d = re.search(r"(?m)^%sdefault:\n((?:%s    [^\n]*\n)+)%s(case [^:\n]+:)\n%s    (return [^;\n]*;)\n\s*\Z" %
                      (re.escape(ind), re.escape(ind), re.escape(ind), re.escape(ind)), body)
        if d and not re.search(r"(?m)^%s    (?:break|return|goto)\b" % re.escape(ind), d.group(1)):
            s_ = "".join(x[4:] + "\n" for x in d.group(1).rstrip("\n").split("\n"))
            new_body = "%s%s\n%s    %s\n" % (ind, d.group(2), ind, d.group(3)) + body[:d.start()]
            return text[:m.end()] + new_body + "%s}\n" % ind + s_ + "%s%s" % (ind, d.group(3)) + text[end.end():]
    return None


def push_tails(text):
    """if (C) { ..; r = A; } else { ..; r = B; } S(r); - the statement after, taking what each arm leaves in r, in
    each arm with its value: the compiler's one copy of a tail both arms ended with (a variant)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        p = _if_parts(lines, k)
        if not p or p[2] is None or p[3] >= len(lines):
            continue
        def tail(arm):                                      # (the arm's last run of r = V; - {r: V}, its start)
            got, j = {}, len(arm) - 1
            while j >= 0 and not arm[j].strip():
                j -= 1
            while j >= 0:
                m_ = re.fullmatch(r"(\s*)(ax|bx|cx|dx|t\d+) = (.+);", arm[j])   # (or the model's temporary)
                if not m_ or m_.group(2) in got:
                    break
                got[m_.group(2)] = (m_.group(3), m_.group(1))
                j -= 1
            return got, j + 1
        ga, ja = tail(p[1])
        gb, jb = tail(p[2])
        nxt = lines[p[3]]
        regs = [r for r in ga if r in gb]
        if not regs or set(ga) != set(gb) or re.match(r"\s*(?:if|while|for|switch|\}|L_\w+:)", nxt) or \
                any(len(re.findall(r"\b%s\b" % r, nxt)) != 1 or _live_after(lines, p[3], r) for r in regs):
            continue
        pv = lambda v: v if re.fullmatch(r"[\w\->.]+", v) else "(%s)" % v

        def put(got):
            s_ = nxt.strip()
            for r in regs:
                s_ = re.sub(r"\b%s\b" % r, lambda _: pv(got[r][0]), s_, count=1)
            return got[regs[0]][1] + s_
        then = p[1][:ja] + [put(ga)]
        els = p[2][:jb] + [put(gb)]
        i0 = re.match(r"\s*", lines[k]).group(0)
        new = [lines[k]] + then + [i0 + "} else {"] + els + [i0 + "}"]
        t = "\n".join(lines[:k] + new + lines[p[3] + 1:])
        for r in regs:
            if r.startswith("t"):
                t = _drop_decl(t, r)
        return t
    return None


def split_ternaries(text):
    """return (C ? A : B); - as the two returns it may have been: if (C) return A; return B; (a variant: the
    compiler keeps one copy of a tail both share either way)"""
    def one(m):
        ind, inner = m.group(1), m.group(2)
        depth, q = 0, None
        for i, ch in enumerate(inner):                      # (the top-level ? and :)
            depth += (ch == "(") - (ch == ")")
            if ch == "?" and depth == 0 and q is None:
                q = i
            if ch == ":" and depth == 0 and q is not None:
                c, a, b = inner[:q].strip(), inner[q + 1:i].strip(), inner[i + 1:].strip()
                return "%sif (%s) {\n%s    return %s;\n%s}\n%sreturn %s;" % (ind, c if c.startswith("(") else "(%s)" % c, ind, a, ind, ind, b)
        return m.group(0)
    t = re.sub(r"(?m)^([ \t]*)return \((.*)\);$", one, text)
    return t if t != text else None


def store_reloads(text):
    """M = A; .. r = M; - a register the model reloads from the place a plain value was just stored in (its
    store-forwarding, written out before the pointer moves): the value itself, r = A"""
    lines = text.split("\n")
    for k in range(len(lines)):
        m = re.fullmatch(r"([ \t]*)(ax|dx|bx|cx) = (\*\w+|\w+->f\d+);", lines[k])
        if not m:
            continue
        place = m.group(3)
        base = re.match(r"\*?(\w+)", place).group(1)
        for j in range(k - 1, max(k - 6, -1), -1):          # (the store just before, nothing between it and
            s = re.fullmatch(r"[ \t]*(.+?) = ([A-Za-z_]\w*|-?\d+);", lines[j])   #  here giving its value or
            if not s:                                                         #  base another)
                break
            if s.group(1) == place:
                if s.group(2) != base and not any(re.match(r"[ \t]*(?:%s|%s) = " % (re.escape(s.group(2)), base), lines[q]) for q in range(j + 1, k)):
                    lines[k] = "%s%s = %s;" % (m.group(1), m.group(2), s.group(2))
                break
    return "\n".join(lines)


def fn_pointers(text):
    """(*x)(..) - x called through: a pointer to a function, int (*x)(); (a parameter's or a local's int)"""
    for x in sorted(set(re.findall(r"\(\*([A-Za-z_]\w*)\)\s*\(", text))):
        if re.search(r"(?m)^[ \t]*(?:register\s+)?int (?:\w+, )*%s\b" % x, text):
            t = _redeclare(text, x, "int (*%s)()" % x)
            text = t or text
    return text


def scaled_index(text):
    """bx = p + (i << 1); .. *bx - a word of p's at i: p[i], p an int's pointer (the scaled index the model left
    as an address; a copy of the doubled index the code made on the way, `ax = (i << 1);`, nothing)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        m = re.match(r"([ \t]*)bx = ([A-Za-z_]\w*) \+ \((\w+|\((?:[^()]|\([^()]*\))+\)) << 1\);$", lines[k] or "")
        if not m:
            continue
        p, i = m.group(2), m.group(3)
        if i.startswith("(") and _balanced(i[1:-1]):
            i = i[1:-1]                                     # (an expression the index: as it is)
        j = k + 1
        while j < len(lines) and re.fullmatch(r"[ \t]*ax = \(%s << 1\);" % re.escape(i), lines[j] or "") and not _live_after(lines, j, "ax"):
            j += 1
        if j >= len(lines) or len(re.findall(r"(?<![\w)])\*bx\b(?!\+\+|--)", lines[j])) != 1 or \
                len(re.findall(r"\bbx\b", lines[j])) != 1 or _live_after(lines, j, "bx") or \
                any(re.search(r"\b%s\s*=[^=]" % v, lines[j]) for v in re.findall(r"[A-Za-z_]\w*", i)):
            continue
        lines[j] = re.sub(r"(?<![\w)])\*bx\b", "%s[%s]" % (p, i), lines[j])
        lines[k + 1:j] = [None] * (j - k - 1)
        lines[k] = None
        t = "\n".join(x for x in lines if x is not None)
        return _declare_pointer(t, p, "int") or t
    return text


def shared_arms(text):
    """if (A) { if (B) { S } } else { S } - S reached both ways (the compiler's one copy): if (!A || B) { S }"""
    lines = text.split("\n")
    changed = True
    while changed:
        changed = False
        for k in range(len(lines)):
            p = _if_parts(lines, k)
            if not p or p[2] is None:
                continue
            th = [x for x in p[1] if x.strip()]
            q = _if_parts(th, 0) if th else None
            if not q or q[2] is not None or q[3] != len(th):
                continue
            inner = [x[4:] for x in q[1] if x.strip()]
            if inner != [x for x in p[2] if x.strip()]:
                continue
            ind = re.match(r"\s*", lines[k]).group(0)
            cond = _conj("||", _truth(_not("(%s)" % p[0])), _truth("(%s)" % q[0]))
            lines[k:p[3]] = ["%sif (%s) {" % (ind, cond)] + p[2] + [ind + "}"]
            changed = True
            break
    return "\n".join(lines)


def guard_returns(text):
    """if (C) { S } return X; last in the function: if (!C) return X; S return X; (a variant: a guard returning
    at once is laid out its own way)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        if not lines[k].startswith("    if (") or not lines[k].endswith("{"):
            continue
        p = _if_parts(lines, k)
        if not p or p[2] is not None:
            continue
        rest = [x for x in lines[p[3]:] if x.strip()]
        if len(rest) != 2 or not re.fullmatch(r"    return [^;]+;", rest[0]) or rest[1] != "}":
            continue
        neg = _not("(%s)" % p[0])
        new = ["    if %s %s" % (neg if neg.startswith("(") and _balanced(neg[1:-1]) else "(%s)" % neg, rest[0].strip())] + \
            [x[4:] if x.startswith("        ") else x for x in p[1]]
        return "\n".join(lines[:k] + new + lines[p[3]:])
    for k in range(len(lines)):                             # (if (C) { S } else { return x; } last: the same guard;
        if not lines[k].startswith("    if (") or not lines[k].endswith("{"):   #  then an arm ending in a return
            continue                                                         #  its else flattened after it)
        p = _if_parts(lines, k)
        if not p or p[2] is None or [x for x in lines[p[3]:] if x.strip()] != ["}"]:
            continue
        el = [x.strip() for x in p[2] if x.strip()]
        th = [x for x in p[1] if x.strip()]
        if len(el) == 1 and el[0].startswith("return"):
            neg = _not("(%s)" % p[0])
            new = ["    if %s %s" % (neg if neg.startswith("(") and _balanced(neg[1:-1]) else "(%s)" % neg, el[0])] + \
                [x[4:] if x.startswith("        ") else x for x in p[1]]
        elif th and re.match(r"\s*return\b", th[-1]) and th[-1].startswith("        return"):
            new = lines[k:k + 1] + p[1] + ["    }"] + [x[4:] if x.startswith("        ") else x for x in p[2]]
        else:
            continue
        t = "\n".join(lines[:k] + new + lines[p[3]:])
        return guard_returns(t) or t
    return None


def guard_flat(text):
    """the first guard guard_returns makes (if (C) { S } else { return x; } last: if (!C) return x; S), the
    returns after it only flattened - not made guards again (a variant)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        if not lines[k].startswith("    if (") or not lines[k].endswith("{"):
            continue
        p = _if_parts(lines, k)
        if not p or p[2] is None or [x for x in lines[p[3]:] if x.strip()] != ["}"]:
            continue
        el = [x.strip() for x in p[2] if x.strip()]
        if len(el) != 1 or not el[0].startswith("return"):
            continue
        neg = _not("(%s)" % p[0])
        new = ["    if %s %s" % (neg if neg.startswith("(") and _balanced(neg[1:-1]) else "(%s)" % neg, el[0])] + \
            [x[4:] if x.startswith("        ") else x for x in p[1]]
        t = "\n".join(lines[:k] + new + lines[p[3]:])
        return flat_returns(t) or t
    return None


def guard_shared(text):
    """if (C) { S } [else { }] last, the function falling off its end the other way - S returning K somewhere:
    if (!C) return K; S (a variant: the way out the compiler shares with S's return of K, the model reading the
    jump to it as the end)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        if not lines[k].startswith("    if (") or not lines[k].endswith("{"):
            continue
        p = _if_parts(lines, k)
        if not p or (p[2] is not None and any(x.strip() for x in p[2])) or [x for x in lines[p[3]:] if x.strip()] != ["}"]:
            continue
        ks = [m.group(1) for m in (re.match(r"\s*return (-?\d+);$", x) for x in p[1]) if m]
        if not ks:
            continue
        neg = _not("(%s)" % p[0])
        new = ["    if %s return %s;" % (neg if neg.startswith("(") and _balanced(neg[1:-1]) else "(%s)" % neg, ks[0])] + \
            [x[4:] if x.startswith("        ") else x for x in p[1]]
        t = "\n".join(lines[:k] + new + lines[p[3]:])
        return flat_returns(t) or t
    return None


def bool_trees(text):
    """an if-tree whose every way ends giving r 1 or 0 (a goto to a labelled part of it the same part again):
    if (E) { r = 1; } else { r = 0; } - E the && / || of its tests, a part both ways share factored out
    (if (c) { X && T } else { T }: (!c || X) && T) - the compiler's one copy of the tests read back"""
    lines = _else_ifs(text.split("\n"))
    labels = {}

    def stmts(block):                                       # (the statements of a block: [(kind, data)])
        out, j = [], 0
        while j < len(block):
            l = block[j]
            if not l.strip():
                j += 1
                continue
            m = re.fullmatch(r"[ \t]*(L_\w+):", l)
            if m:
                out.append(("label", m.group(1)))
                j += 1
                continue
            p = _if_parts(block, j)
            if p:
                out.append(("if", p))
                j = p[3]
                continue
            out.append(("line", l.strip()))
            j += 1
        return out

    def node(block, reg):                                   # (the expression a block gives reg, or None)
        ss = stmts(block)
        lab = None
        if ss and ss[0][0] == "label":
            lab = ss[0][1]
            ss = ss[1:]
        if len(ss) != 1:
            return None
        kind, d = ss[0]
        if kind == "label":                                 # (a label and then only a label: no statement, no expression)
            return None
        if kind == "line":
            m = re.fullmatch(r"%s = ([01]);" % reg, d)
            if m:
                e = m.group(1)
            else:
                g = re.fullmatch(r"goto (L_\w+);", d)
                if not g:
                    return None
                e = ("goto", g.group(1))
        else:
            c, th, el = d[0], d[1], d[2]
            if el is None:
                return None
            a, b = node(th, reg), node(el, reg)
            if a is None or b is None:
                return None
            e = ("if", c, a, b)
        if lab:
            labels[lab] = e
        return e

    def text_of(e):
        if isinstance(e, str):
            return e
        if e[0] == "goto":
            return text_of(labels[e[1]]) if e[1] in labels and labels[e[1]] != e else None
        c, a, b = "(%s)" % _strip("(%s)" % e[1]) if not e[1].startswith("(") else e[1], e[2], e[3]
        a_, b_ = text_of(a), text_of(b)
        if a_ is None or b_ is None:
            return None
        t, f = _truth(c), _truth(_not(c))
        if b_ == "0":
            return t if a_ == "1" else "0" if a_ == "0" else _conj("&&", t, a_)
        if a_ == "1":
            return _conj("||", t, b_)
        if a_ == "0":
            return f if b_ == "1" else _conj("&&", f, b_)
        if b_ == "1":
            return _conj("||", f, a_)
        parts = [x.strip() for x in _top_split(a_, "&&")]
        tail = [x.strip() for x in _top_split(b_, "&&")]
        if len(parts) > len(tail) and parts[-len(tail):] == tail:   # (if (c) { X && T } else { T }: (!c || X) && T)
            x = " && ".join(parts[:-len(tail)])
            return _conj("&&", "(%s)" % _conj("||", f, x), b_)
        return None

    for k in range(len(lines)):
        p = _if_parts(lines, k)
        if not p or p[2] is None:
            continue
        nxt = next((x for x in lines[p[3]:] if x.strip()), "")
        r = re.fullmatch(r"[ \t]*return (\w+);", nxt)
        if not r:
            continue
        labels.clear()
        e = node(lines[k:p[3]], r.group(1))
        if e is None or isinstance(e, str) or e[0] != "if" or all(isinstance(x, str) for x in e[2:]):
            continue                                        # (one test alone: as written - its sense is the code's)
        x = text_of(e)
        x = text_of(e) if x is None else x                  # (a goto to a label met later: again)
        if x is None or x in ("0", "1") or "goto" in x:
            continue
        ind = re.match(r"\s*", lines[k]).group(0)
        reg = r.group(1)
        new = ["%sif %s {" % (ind, x if x.startswith("(") and x.endswith(")") and _balanced(x[1:-1]) else "(%s)" % x), "%s    %s = 1;" % (ind, reg),
               "%s} else {" % ind, "%s    %s = 0;" % (ind, reg), "%s}" % ind]
        return "\n".join(lines[:k] + new + lines[p[3]:])
    return text


def shared_tails(text):
    """if (A) { X; if (B) { T } } else { T } at the function's end - the compiler's one copy of T both ways
    reach: if (A) { X; if (!B) return; } T (a variant)"""
    lines = text.split("\n")
    for k in range(len(lines)):
        p = _if_parts(lines, k)
        if not p or p[2] is None or not p[1]:
            continue
        ind = re.match(r"\s*", lines[k]).group(0)
        rest = [x for x in lines[p[3]:] if x.strip()]
        if not rest or not (rest[0] == "}" or re.fullmatch(r"[ \t]*return( \w+)?;", rest[0]) and len(rest) > 1 and rest[1] == "}"):
            continue
        then = p[1]
        q = next((j for j in range(len(then)) if then[j].startswith(ind + "    if (") and then[j].endswith("{") and
                  len(then[j]) - len(then[j].lstrip()) == len(ind) + 4), None)
        while q is not None:                                # (the last if of the then-arm, nothing after it)
            pi = _if_parts(then, q)
            if pi and pi[2] is None and pi[3] == len(then):
                break
            q = next((j for j in range(q + 1, len(then)) if then[j].startswith(ind + "    if (") and then[j].endswith("{") and
                      len(then[j]) - len(then[j].lstrip()) == len(ind) + 4), None)
        if q is None:
            continue
        inner = [x[4:] for x in pi[1]]
        if [x.rstrip() for x in inner if x.strip()] != [x.rstrip() for x in p[2] if x.strip()]:
            continue
        neg = _not("(%s)" % pi[0])
        new = [lines[k]] + then[:q] + [ind + "    if (%s) return;" % (neg if neg.startswith("(") else "(%s)" % neg), ind + "}"] + \
            [x[4:] for x in p[2]]
        return "\n".join(lines[:k] + new + lines[p[3]:])
    return None


def scaled_places(text):
    """(i << 1) + K, K a place in the data: the address of a word there, &g_K[i] (a variant)"""
    def one(m):
        k = int(m.group(2)) & 0xFFFF
        if k < 0x100:
            return m.group(0)
        name = "g_%04x" % k
        ARRAYS.setdefault(name, "int")
        return "&%s[%s]" % (name, m.group(1))
    t = re.sub(r"\(\(([\w\->.]+) << 1\) \+ (-?\d+)\)", one, text)
    for g in sorted(set(re.findall(r"&(g_[0-9a-f]{4})\[", t))):   # (declared, when the text is a whole source)
        if re.search(r"(?m)^\w+\(.*\)$", t) and not re.search(r"(?m)^(?:extern|struct)[^\n]*\b%s\b" % g, t):
            t = "extern int %s[];\n" % g + t
    return t if t != text else None


def frame_more(text):
    """the frame one word bigger: the trailing filler two bytes more, or one added after the last local (a
    variant: a local the model's frame left out)"""
    head, sep, body = split_body(text)
    if not sep:
        return None
    decls = list(re.finditer(r"(?m)^[ \t]*(?!return\b)(?:register\s+)?(?:unsigned\s+)?(?:int|char|long|short|struct \w+)[ \t*]+\w+(?:\[\d+\])?;\n", body))
    decls = [d for d in decls if d.start() < (re.search(r"(?m)^[ \t]*(?!register|unsigned|int|char|long|short|struct)\S", body) or re.search(r"$", body)).start()]
    if not decls:
        return None
    last = decls[-1]
    f = re.fullmatch(r"([ \t]*)char (_p\w*)\[(\d+)\];\n", last.group(0))
    if f:
        new = "%schar %s[%d];\n" % (f.group(1), f.group(2), int(f.group(3)) + 2)
    else:
        new = last.group(0) + "    char _pm[2];\n"
    return head + sep + body[:last.start()] + new + body[last.end():]


def lead_filler(text):
    """the frame's first local vN (bp-N) with room between it and bp: a filler declared before it (a variant:
    unused locals the source declared first, the model's layout taking the first one for read)"""
    head, sep, body = split_body(text)
    if not sep:
        return None
    m = re.search(r"(?m)^([ \t]*)((?:unsigned\s+)?(int|char|long|short)|(?:\w+ )?\w+ \*) ?v(\d+);$", body)
    first = re.search(r"(?m)^[ \t]*(?!register\b)(?:unsigned\s+)?(?:int|char|long|short|struct \w+)[ \t*]+\w+(?:\[\d+\])?;$", body)
    if not m or not first or first.start() != m.start():
        return None
    size = 4 if m.group(3) == "long" else 1 if m.group(3) == "char" else 2
    gap = int(m.group(4)) - size
    if gap <= 0:
        return None
    fill = "".join("%sint _q%d;\n" % (m.group(1), 2 * j) for j in range(gap // 2)) + \
        ("%schar _q%d;\n" % (m.group(1), gap) if gap % 2 else "")      # (words: an array goes elsewhere in the frame)
    return head + sep + body[:m.start()] + fill + body[m.start():]


def register_copies(text):
    """r = x = E; with the register variable r given nothing else: x itself where r is read (a variant: the
    compiler went on through the value it still held - the register its own scratch, not a variable)"""
    for m in re.finditer(r"(?m)^([ \t]*)(r_\w+) = (\w+) = ([^;\n]+);$", text):
        r, x = m.group(2), m.group(3)
        rest = text[m.end():]
        if re.search(r"(?<![\w>.*])%s\s*(?:=[^=]|\+\+|--)|(?:\+\+|--)%s\b" % (r, r), rest) or \
                re.search(r"(?<![\w>.*])%s\s*(?:=[^=]|\+\+|--)" % x, rest):
            continue
        t = text[:m.start()] + "%s%s = %s;" % (m.group(1), x, m.group(4)) + re.sub(r"\b%s\b" % r, x, rest)
        d = re.search(r"(?m)^[ \t]*register [^;\n]*\b%s;\n" % r, t)
        if d:
            t = t[:d.start()] + t[d.end():]
        if (r[2:] + "w", 0) in BYTEPTR and (r[2:] + "*", 0) not in BYTEPTR:   # (words read through the register:
            t = _redeclare(t, x, "int *%s" % x) or t                        #  an int's pointer)
        return t
    return None


def unsigned_registers(text):
    """the text with its int register locals unsigned (a variant: compared, the compiler's jumps say which)"""
    t = re.sub(r"(?m)^([ \t]*)register int (r_\w+);$", r"\1register unsigned \2;", text)
    return t if t != text else None


def no_default(text):
    """switch (..) { .. default: return X; } - a switch with no default, the return after it (a variant: the
    compiler's default jump goes to the code after the switch either way, laid out after the cases)"""
    t = re.sub(r"(?m)^([ \t]*)default:\n[ \t]*(return [^;\n]*;)\n([ \t]*)\}$",
               lambda m: "%s}\n%s%s" % (m.group(3), m.group(3), m.group(2)) if len(m.group(1)) == len(m.group(3)) else m.group(0), text)
    return t if t != text else None


def unmasked(text):
    """(A == (B & 0xff)) - a byte the model masked compared with another: the bytes compared (a variant: the
    compiler compares bytes of two chars as they are)"""
    t = re.sub(r"(==|!=) \(((?:\*|\b)[A-Za-z_]\w*(?:->f\d+|\[[^\]]+\])?) & 0xff\)", r"\1 \2", text)
    t = re.sub(r"\(((?:\*|\b)[A-Za-z_]\w*(?:->f\d+|\[[^\]]+\])?) & 0xff\) (==|!=)", r"\1 \2", t)
    flip = {"==": "!=", "!=": "=="}
    t = re.sub(r"!\(([\w\->.*\[\]]+) (==|!=) ([\w\->.*\[\]]+)\)", lambda m: "(%s %s %s)" % (m.group(1), flip[m.group(2)], m.group(3)), t)
    u = re.sub(r"\b(\w+->f\d+|\*\w+) (==|!=) (12[89]|1[3-9]\d|2[0-4]\d|25[0-5])\b",     # (a byte compared with
               lambda m: "%s %s (char)0x%x" % (m.group(1), m.group(2), int(m.group(3))), t)   #  128..255: the char's)
    return u if u != text else None


def high_words(text):
    """f(a, b): a never read, b only masked (b & K) - a long's high word tested alone: long a; (a & 0xK0000L)
    (a variant: the compiler tests just the high word of a long masked so)"""
    h = re.search(r"(?m)^(\w+)\(([\w, ]+)\)\n", text)
    if not h:
        return None
    ps = [p.strip() for p in h.group(2).split(",")]
    body = text[h.end():]
    for p, q in zip(ps, ps[1:]):
        uses = re.findall(r"(?<![\w>.])%s(?![\w(])" % q, body)
        masks = re.findall(r"\(%s & (-?\d+|0x[0-9a-f]+)\)" % q, body)
        decl = len(re.findall(r"(?m)^[ \t]*(?:register )?\w+ [^;\n]*\b%s\b[^;\n]*;$" % q, body))
        if re.search(r"(?<![\w>.])%s(?![\w(])" % p, re.sub(r"(?m)^[ \t]*(?:register )?\w+ [\w, *]+;$", "", body)) or \
                not masks or len(uses) != len(masks) + decl:
            continue
        def mask(m):
            k = int(m.group(1), 0) & 0xffff
            return "(%s & 0x%x0000L)" % (p, k)
        t = re.sub(r"\(%s & (-?\d+|0x[0-9a-f]+)\)" % q, mask, body)
        t = _drop_decl(t, q)
        t = _redeclare(t, p, "long %s" % p)
        if t is None:
            return None
        return text[:h.start()] + "%s(%s)\n" % (h.group(1), ", ".join(x for x in ps if x != q)) + t
    return None


def assign_in_args(text):
    """X = E; f(..X..); - the value passed from the register it was stored from: f(..(X = E)..) (a variant)"""
    def one(m):
        ind, x, e, call = m.groups()
        if len(re.findall(r"(?<![\w>.])%s(?![\w(])" % re.escape(x), call)) != 1 or \
                re.search(r"&\s*%s(?![\w(])" % re.escape(x), call):   # (its address passed: no value there)
            return m.group(0)
        return ind + re.sub(r"(?<![\w>.])%s(?![\w(])" % re.escape(x), lambda _: "(%s = %s)" % (x, e), call, count=1)
    t = re.sub(r"(?m)^([ \t]*)([A-Za-z_][\w\->\[\]]*) = ([^;\n=]+);\n[ \t]*((?:return )?[A-Za-z_]\w*\(.*\);)$", one, text)
    return t if t != text else None


def return_sources(text):
    """X = Y; return X; - the value still in the register it was stored from: return Y (a variant: a reload of X
    is other code)"""
    t = re.sub(r"(?m)^([ \t]*)([^=;\n]+?) = ([A-Za-z_]\w*);\n([ \t]*)return \2;$", r"\1\2 = \3;\n\4return \3;", text)
    return t if t != text else None


def compound_assigns(text):
    """X = X op E; - the compiler's `op [X], reg` - as C's compound assignment X op= E (a variant: MS C gives the
    two different code)"""
    rx = re.compile(r"(?m)^([ \t]*)([A-Za-z_*][\w\->\[\]*]*) = \2 ([-+*/%&|^]|<<|>>) (.+);$")

    def one(m):
        ind, x, op, e = m.groups()
        if e.startswith("(") and e.endswith(")") and _strip("(" + e + ")") == e:
            e = e[1:-1]
        return "%s%s %s= %s;" % (ind, x, op, e)
    t = rx.sub(one, text)
    return t if t != text else None


def chain_assigns(text):
    """A = E; B = A; - the value given twice from the register it is in: B = A = E (a variant: a reload of A
    reads the same)"""
    rx = re.compile(r"(?m)^([ \t]*)([^\s=;][^=;]*?) = ([^;\n]+);\n\1([^\s=;][^=;]*?) = \2;$")

    def one(m):
        ind, a, e, b = m.groups()
        if b == a or re.search(r"\b%s\b" % re.escape(b), e) or re.search(r"\+\+|--|\w\(", a + b):
            return m.group(0)
        return "%s%s = %s = %s;" % (ind, b, a, e)
    t = rx.sub(one, text)
    return t if t != text else None


def guard_gotos(text):
    """a goto past statements under a test - the compiler's || of a test that skips them:
        if (C1) { if (C2) goto L; } S.. L:   ->   if (!C1 || !C2) { S.. }
        if (C) goto L; S.. L:                ->   if (!C) { S.. }
    (L then wanted by none else: dropped)"""
    lines = text.split("\n")
    changed = True
    while changed:
        changed = False
        for k in range(len(lines)):
            m = re.match(r"(\s*)if \((.*)\) \{$", lines[k])
            g = re.match(r"(\s*)if \((.*)\) goto (L_\w+);$", lines[k + 1]) if m and k + 2 < len(lines) else None
            one = re.match(r"(\s*)if \((.*)\) goto (L_\w+);$", lines[k])
            if m and g and lines[k + 2] == m.group(1) + "}":
                ind, lab = m.group(1), g.group(3)
                cond = "%s || %s" % (_truth(_not("(" + m.group(2) + ")")), _truth(_not("(" + g.group(2) + ")")))
                start = k + 3
            elif one:
                ind, lab = one.group(1), one.group(3)
                cond = _truth(_not("(" + one.group(2) + ")"))
                start = k + 1
            else:
                continue
            end = next((j for j in range(start, len(lines)) if lines[j] == ind + lab + ":"), None)
            if end is None or end == start:
                continue
            body = lines[start:end]
            if any(l.strip() and not l.startswith(ind) for l in body) or any(re.match(r"\s*L_\w+:", l) for l in body):
                continue
            depth = 0
            ok = True
            for l in body:                                  # (the statements between: whole ones)
                depth += l.count("{") - l.count("}")
                if depth < 0:
                    ok = False
            if not ok or depth:
                continue
            new = ["%sif (%s) {" % (ind, cond)] + ["    " + l if l.strip() else l for l in body] + [ind + "}"]
            rest = lines[:k] + new + lines[end + 1:]
            if re.search(r"\bgoto %s;" % lab, "\n".join(rest)):
                rest = lines[:k] + new + lines[end:]        # (wanted elsewhere: kept)
            lines = rest
            changed = True
            break
    return "\n".join(lines)


def merge_conditions(text):
    """ifs nested as the compiler's || and && leave them, joined again:
        if (A) { if (B) { P } else { Q } } else { Q }   ->  if (A && B) { P } else { Q }
        if (A) { if (B) { P } else { Q } } else { P }   ->  if (!A || B) { P } else { Q }
        if (A) { if (B) { P } }                         ->  if (A && B) { P }
    (the arms the model wrote twice, where both ways go)"""
    lines = _else_ifs(text.split("\n"))
    changed = True
    while changed:
        changed = False
        for k in range(len(lines)):
            p = _if_parts(lines, k)
            if not p:
                continue
            c1, then, els, end = p
            ind = re.match(r"\s*", lines[k]).group(0)
            body = [l for l in then if l.strip()]
            if not body:
                continue
            q = _if_parts(then, next(j for j, l in enumerate(then) if l.strip()))
            if not q or q[3] != len(then) and any(l.strip() for l in then[q[3]:]):
                continue
            c2, p2, q2, _ = q
            dd = lambda ls: [l[4:] if l.startswith("    ") else l for l in ls]
            new = None
            if els is None and q2 is None:
                new = ["%sif (%s) {" % (ind, _conj("&&", _truth("(" + c1 + ")"), _truth("(" + c2 + ")")))] + dd(p2) + [ind + "}"]
            elif els is not None and q2 is not None and _same_arm(els, dd(q2)):
                new = ["%sif (%s) {" % (ind, _conj("&&", _truth("(" + c1 + ")"), _truth("(" + c2 + ")")))] + dd(p2) + [ind + "} else {"] + _unlabelled(dd(q2), els) + [ind + "}"]
            elif els is not None and q2 is not None and _same_arm(els, dd(p2)):
                new = ["%sif (%s) {" % (ind, _conj("||", _truth(_not("(" + c1 + ")")), _truth("(" + c2 + ")")))] + _unlabelled(dd(p2), els) + [ind + "} else {"] + dd(q2) + [ind + "}"]
            if new:
                cand = lines[:k] + new + lines[end:]
                whole = "\n".join(cand)                     # (a label another goto still wants: kept, else not this way)
                if any(not re.search(r"(?m)^[ \t]*%s:$" % g, whole) for g in re.findall(r"\bgoto (L_\w+);", whole)):
                    continue
                lines = cand
                changed = True
                break
    return "\n".join(lines)


def register_temps(text):
    """a register variable given a call's value and read once, by the next statement, and never again: the
    compiler's own keeping of a value it computed first (strlen(b) of strlen(a) + strlen(b)) - the call in its
    place, the variable gone (a variant)"""
    head, sep, body = split_body(text)
    if not sep:
        return None
    lines = body.split("\n")
    changed = False
    for k, l in enumerate(lines):
        m = re.match(r"(\s*)(\w+) = (\w+\((?:[^()]|\([^()]*\))*\));$", l)
        if not m or not re.search(r"(?m)^[ \t]*register [\w ]+\**\s*\b%s;$" % m.group(2), body):   # (\b: the name, not the end of a longer one)
            continue
        x = m.group(2)
        rest = [j for j in range(len(lines)) if j != k and re.search(r"\b%s\b" % x, lines[j]) and not re.match(r"\s*register\b", lines[j])]
        if len(rest) != 1 or rest[0] != k + 1 or len(re.findall(r"\b%s\b" % x, lines[k + 1])) != 1:
            continue
        lines[k + 1] = re.sub(r"\b%s\b" % x, lambda _: m.group(3), lines[k + 1])
        lines[k] = None
        lines = [y for y in lines if y is None or not re.match(r"\s*register [\w ]+\**\s*\b%s;$" % x, y)]
        changed = True
        break
    if not changed:
        return None
    return head + sep + "\n".join(l for l in lines if l is not None)


def case_falls(text):
    """a case that goes on into the next (`goto L;` to the label the next case starts with): it falls into it -
    an arm's last `goto L;` is dropped, and `if (C) { X } else { goto L; } break;` is `if (C) { X break; }`"""
    lines = text.split("\n")
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)(L_\w+):$", lines[k])
        c = k
        while c > 0 and re.match(r"\s*(?:case [^:]+|default):$", lines[c - 1]):
            c -= 1
        if not m or c == k:
            k += 1
            continue
        lab, ind = m.group(2), re.match(r"\s*", lines[c]).group(0)
        i4 = ind + "    "
        if lines[c - 1] == i4 + "goto %s;" % lab:
            del lines[c - 1]
            k -= 1
        elif c >= 5 and lines[c - 4:c] == [i4 + "} else {", i4 + "    goto %s;" % lab, i4 + "}", i4 + "break;"] and \
                not re.match(r"\s*(?:return|break|continue|goto)\b", lines[c - 5]):
            lines[c - 4:c] = [i4 + "    break;", i4 + "}"]
            k -= 2
        else:
            k += 1
            continue
        if not re.search(r"\bgoto %s;" % lab, "\n".join(lines)):
            del lines[k]
        k += 1
    # (an arm's last goto to another case's start, not the next: the statements there to their break, copied -
    #  the compiler's one copy of a tail both cases end with)
    for _ in range(4):
        hit = False
        for j, l in enumerate(lines):
            g = re.fullmatch(r"(\s*)goto (L_\w+);", l)
            if not g or j + 1 >= len(lines) or not re.match(r"\s*(?:case [^:]+|default):$|\s*\}$", lines[j + 1]):
                continue
            at = next((i for i, x in enumerate(lines) if x.strip() == g.group(2) + ":"), None)
            if at is None or at <= j:
                continue
            end = next((i for i in range(at + 1, len(lines)) if re.match(r"\s*(?:break;|return\b)", lines[i])), None)
            if end is None or any(re.match(r"\s*(?:case [^:]+|default):$|\s*L_\w+:", x) for x in lines[at + 1:end]):
                continue
            src_ind = len(lines[at + 1]) - len(lines[at + 1].lstrip()) if at + 1 <= end else 0
            lines[j:j + 1] = [g.group(1) + x[src_ind:] if len(x) - len(x.lstrip()) >= src_ind else x for x in lines[at + 1:end + 1]]
            if not re.search(r"\bgoto %s;" % g.group(2), "\n".join(lines)):
                lines = [x for x in lines if x.strip() != g.group(2) + ":"]
            hit = True
            break
        if not hit:
            break
    return "\n".join(lines)


def inner_registers(text):
    """a register variable first given a value after other statements: declared in a block from there (to
    before the function's last return) - a variant: before the block the compiler has the register free for
    its own use"""
    head, sep, body = split_body(text)
    if not sep:
        return None
    lines = body.split("\n")
    decls = [(j, m) for j, l in enumerate(lines) for m in [re.match(r"    register ([\w ]+?\**) ?(\w+);$", l)] if m]
    for j, m in decls:
        x = m.group(2)
        first = next((k for k, l in enumerate(lines) if k > j and re.search(r"\b%s\b" % x, l)), None)
        if first is None or not re.match(r"    %s = " % x, lines[first]):
            continue
        stmts = [k for k in range(j + 1, first) if lines[k].strip() and not re.match(r"\s*(?:register |int |char |unsigned |long |struct )", lines[k])]
        if not stmts:
            continue
        end = max((k for k, l in enumerate(lines) if re.match(r"    return\b", l)), default=None)
        if end is None or end <= first:
            continue
        new = lines[:j] + lines[j + 1:first] + ["    {", "        register %s%s;" % (m.group(1) + " " if not m.group(1).endswith("*") else m.group(1), x)] + \
            ["    " + l if l.strip() else l for l in lines[first:end]] + ["    }"] + lines[end:]
        return head + sep + "\n".join(new)
    return None


def char_stores(text):
    """a byte register stored into a local (vN = al) is a char local; a test of the register at once is a test of
    the store - if ((vN = al) != K): the compiler compares what it has - and the register read later, before it is
    given another value, is the local's value. A constant 128..255 it is compared with: (char)0xNN, a byte's"""
    lines = text.split("\n")
    chars = set()
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)(v\d+) = ([abcd]l);\s*$", lines[k])
        if not m:
            k += 1
            continue
        v, r = m.group(2), m.group(3)
        w = r[0] + "x"
        nx = next((j for j in range(k + 1, len(lines)) if lines[j].strip()), None)
        if nx is None:
            break
        if re.match(r"\s*(?:if|while) \(.*\b%s\b" % r, lines[nx]):
            lines[nx] = re.sub(r"\b%s\b" % r, "(%s = %s)" % (v, r), lines[nx], count=1)
            del lines[k]
            nx -= 1
        else:
            k += 1
            nx = k
        chars.add(v)
        for j in range(nx + 1, len(lines)):                 # (read again before it changes: the local)
            if re.match(r"\s*L_\w+:", lines[j]) or re.search(r"\b(?:%s|%s) = |\w\(" % (r, w), lines[j]):
                break
            lines[j] = re.sub(r"\b%s\b" % r, v, lines[j])
    if not chars:
        return text
    text = "\n".join(lines)
    for v in chars:
        text = re.sub(r"(?m)^([ \t]*)int %s;" % v, r"\1char %s;" % v, text)
        text = re.sub(r"(\(%s = [abcd]l\)|\b%s) ([!=]=|[<>]=?) (1(?:2[89]|[3-9]\d)|2[0-4]\d|25[0-5])\b" % (v, v),
                      lambda m: "%s %s (char)0x%02x" % (m.group(1), m.group(2), int(m.group(3))), text)
    return text


def store_increments(text):
    """*p = X; p = p + 1; (X not reading p) - the store and the step as one: *p++ = X (a variant: the compiler
    makes other code of the two)"""
    lines = text.split("\n")
    out = []
    k = 0
    while k < len(lines):
        m = re.match(r"(\s*)\*(\w+) = (.*);\s*$", lines[k])
        if m and k + 1 < len(lines) and re.match(r"\s*%s = %s \+ 1;\s*$" % (m.group(2), m.group(2)), lines[k + 1]) \
                and not re.search(r"\b%s\b" % m.group(2), m.group(3)):
            out.append("%s*%s++ = %s;" % m.groups())
            k += 2
            continue
        out.append(lines[k])
        k += 1
    t = "\n".join(out)
    return t if t != text else None


def register_values(text):
    """r = E; (E a variable, a field, *p) read more than once after, nothing between changing E or calling: E at
    each read - the compiler kept E in the register and read it there (r a machine register left over)"""
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.fullmatch(r"([ \t]*)(ax|al|bx|bl|cx|cl|dx|dl) = ([A-Za-z_]\w*(?:->f\d+)*|\*[A-Za-z_]\w*);", l)
        if not m:
            continue
        reg, e = m.group(2), m.group(3)
        names = re.findall(r"[A-Za-z_]\w*", e)
        depth, uses, end = 0, [], len(lines)
        for j in range(k + 1, len(lines)):
            x = lines[j]
            depth += x.count("{") - x.count("}")
            if depth < 0 or re.match(r"[ \t]*%s = " % reg, x) or re.match(r"[ \t]*L_\w+:", x):
                end = j
                break
            if re.search(r"\w\s*\(", re.sub(r"\b(?:if|while|for|switch|return)\s*\(", "", x)) or \
                    any(re.search(r"(?<![\w>.])%s\s*(?:=[^=]|\+\+|--)" % re.escape(n), x) for n in names) or \
                    re.search(r"%s\s*=[^=]" % re.escape(e), x):
                end = None
                break
            if re.search(r"\b%s\b" % reg, x):
                uses.append(j)
        if end is None or len(uses) < 2 or _live_after(lines, (end - 1) if end else len(lines) - 1, reg):
            continue
        for j in uses:
            lines[j] = re.sub(r"\b%s\b" % reg, e, lines[j])
        lines[k] = None
        return "\n".join(x for x in lines if x is not None)
    return text


def shapes(text):
    """the shapes a compiler gives a statement, read back: a loop whose test it moved to the bottom (`goto L;
    do {..} while (c);` with no L - the jump goes to the test) is `while (c) {..}`; a step then a read one back
    (`p = p + 1; .. p[-1]`) is `*p++`"""
    lines = text.split("\n")
    out = []
    k = 0
    while k < len(lines):
        l = lines[k]
        m = re.match(r"(\s*)goto (L_\w+);\s*$", l)
        if m and k + 1 < len(lines) and lines[k + 1].strip() == "do {" and not re.search(r"^\s*%s:" % m.group(2), text, re.M):
            depth, close = 0, None
            for j in range(k + 1, len(lines)):              # (the do's own closing line)
                depth += lines[j].count("{") - lines[j].count("}")
                if depth == 0:
                    close = j
                    break
            mm = re.match(r"(\s*)\} while \((.*)\);\s*$", lines[close]) if close is not None else None
            if mm:
                out.append("%swhile (%s) {" % (m.group(1), mm.group(2)))
                out += lines[k + 2:close]
                out.append("%s}" % m.group(1))
                k = close + 1
                continue
        out.append(l)
        k += 1
    text = "\n".join(out)
    # after a return, what stands in the same block before its end (no label) is never reached
    lines = text.split("\n")
    keep = []
    dead = None
    for l in lines:
        ind = len(l) - len(l.lstrip(" "))
        if dead is not None:
            if ind < dead or re.match(r"\s*L_\w+:", l):    # (out of the return's block: a closing brace, or
                                                            #  the next statement after a braceless if's)
                dead = None
            else:
                continue
        keep.append(l)
        if re.match(r"\s*return\b.*;\s*$", l):
            dead = ind
    text = "\n".join(keep)
    # a step, then the value one back read: the post-increment (in a do's test, or the next statement)
    lines = text.split("\n")
    for k, l in enumerate(lines):
        m = re.match(r"\s*(\w+) = \1 \+ 1;\s*$", l)
        if not m:
            continue
        v = m.group(1)
        for j in range(k + 1, min(k + 4, len(lines))):
            if re.search(r"\b%s\[-1\]" % v, lines[j]):
                lines[j] = re.sub(r"\b%s\[-1\]" % v, "*%s++" % v, lines[j], count=1)
                lines[k] = None
                break
            if lines[j].strip() not in ("}", "") and not lines[j].strip().startswith("} while"):
                break
    return "\n".join(l for l in lines if l is not None)


def _drop_decl(t, x):
    """t without x in its `int a, x, b;` declaration (the line gone if x was all of it)"""
    def one(m):
        rest = (m.group(2) + m.group(3).lstrip(", ")).strip(", ")
        return "%sint %s;\n" % (m.group(1), rest) if rest else ""
    return re.sub(r"(?m)^([ \t]*)int ((?:\w+, )*)%s((?:, \w+)*);\n" % re.escape(x), one, t, count=1)


REGSLOTS = [False]                                          # (the toolchain gives a register local a frame slot)


def _redeclare(t, x, new):
    """t with x's declaration (`int a, x, b;` - a parameter's, a local's) as `new;`, or None"""
    d = re.search(r"(?m)^([ \t]*)(register\s+)?(struct \w+|\w+)\s*\*?\s*((?:\w+, )*)%s((?:, \w+)*);$" % re.escape(x), t)
    if not d or d.group(3) in ("return", "goto", "extern"):
        return None
    rest = (d.group(4) + d.group(5).lstrip(", ")).strip(", ")
    rep = ("%s%s%s %s;\n" % (d.group(1), d.group(2) or "", d.group(3), rest) if rest else "") + \
        "%s%s%s;" % (d.group(1), d.group(2) or "", new)
    return t[:d.start()] + rep + t[d.end():]
