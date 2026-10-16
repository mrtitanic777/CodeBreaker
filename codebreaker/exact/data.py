"""data.py -- a module's initialized data written as C: where it stands, and definitions proposed from the bytes.

    python -m codebreaker exact PROJECT data MODULE [--draft] [--write]

The module's functions are compiled alone (no data files): their literals, each with the function
that names it first, are lined up with the program's data segment from the module's data start
(src/MODULES). What lies between them is the module's own: its variables, and the strings their
values point at. Which declarations those are, and where they stood in the source, the toolchain
profile's "data" rule says:

    "order": "after_next_function"   (Microsoft C 3.00) a file's _DATA follows the source: an
        initializer's strings where the declaration stands, a function's literals with the function,
        and a variable's bytes after the literals of the next function after its declaration - so a
        run after F's literals is variables declared before the function after F (src/dNEXT.c), and
        the strings before such a run are their values; in a file without functions, all literals
        come first, then the variables

The definitions are typed by the functions' extern declarations (int where none says), their
values by the program's bytes: a word that points at one of the module's strings is that string, at a
known place its name. What the bytes cannot tell is marked /* ? */.

    data.walk(project, row) -> (runs [(lo, hi, after function, kind)], strings, dstart, dend)
    data.draft(project, row) -> {file name: text}
"""
import collections
import glob
import os
import re
import struct

from codebreaker.exact import modules, omf

BS = chr(92)


def cstr(b):
    o = ""
    for c in b:
        ch = chr(c)
        if ch == "\n":
            o += BS + "n"
        elif ch == "\t":
            o += BS + "t"
        elif ch == "\r":
            o += BS + "r"
        elif ch in '"' + BS:
            o += BS + ch
        elif 32 <= c < 127:
            o += ch
        else:
            o += BS + "%03o" % c
    return '"' + o + '"'


def _functions_only(prj, row):
    mod, a, e, d, x = row
    paths = [q for q in modules.files_of(prj.src, a, e) if os.path.basename(q).startswith("f")] if a is not None else []
    if not paths:
        return None
    tc = prj.toolchain()
    default = prj.switches if prj.switches is not None else tc.p.get("switches", "")
    text, flags = modules.combine(paths, default, tc.p.get("file_switches"))
    fl = x or (sorted(flags)[0] if flags else default)
    if tc.p.get("file_switches") and fl:
        text = "/* %s: %s */\n" % (tc.p["file_switches"], fl) + text
    raw, con = tc.compile(text.encode("latin1"), headers=prj.headers(), include=prj.include, switches=prj.switches)
    if raw is None:
        raise SystemExit("%s's functions do not compile alone:\n%s" % (mod, con[-1500:]))
    return omf.read(raw), [int(os.path.basename(q)[1:6], 16) for q in paths]


def walk(prj, row):
    mod, a, e, dstart, x = row
    rows = modules.rows(prj)
    nxt = sorted(r[3] for r in rows if r[3] > dstart)
    ds = prj.image[prj.dgroup:]
    dend = nxt[0] if nxt else len(ds)
    got = _functions_only(prj, row)
    if got is None:
        return [(dstart, dend, "(start)", "vars")], [], dstart, dend, []
    o, addrs = got
    text = omf.code_segments(o)[0]
    fns = omf.functions(o, text)
    lit = o.seg.get("_DATA", b"")[:o.filled.get("_DATA", 0)]
    owner = {}
    for seg, fo, kind, tgt, disp in o.fix:
        if seg == text and kind == "off" and tgt == ("seg", "_DATA"):
            tw = (int.from_bytes(o.seg[text][fo:fo + 2], "little") + disp) & 0xFFFF
            f = [n for n, off, ln in fns if off <= fo < off + ln]
            if f and tw not in owner:
                owner[tw] = f[0]
    strs = []
    k = 0
    while k < len(lit):
        e2 = lit.index(b"\0", k) if b"\0" in lit[k:] else len(lit)
        strs.append((k, lit[k:e2 + 1], owner.get(k)))
        k = e2 + 1
    cur = None
    for i, (k, s, f) in enumerate(strs):
        if f is None:
            strs[i] = (k, s, cur)
        else:
            cur = f
    places = _align(ds, [s for k, s, f in strs], dstart, dend)
    runs = []
    pos = dstart
    last = None
    missing = []
    for (k, s, f), at in zip(strs, places):
        if at is None:
            missing.append((s, f))
            continue
        if at != pos:
            runs.append((pos, at, last, "?"))
        pos = at + len(s)
        last = f
    if pos < dend:
        runs.append((pos, dend, last, "?"))
    out = []
    for lo, hi, after, kind in runs:
        seg = ds[lo:hi]
        txt = all(32 <= c < 127 or c in (0, 9, 10, 13) for c in seg) and seg.count(0) <= max(1, len(seg) // 6)
        out.append((lo, hi, after or "(start)", "strings" if txt else "vars"))
    names = [n for n, off, ln in fns]
    return out, strs, dstart, dend, list(zip(names, addrs))


def _align(ds, lits, lo, hi):
    """the places of the literals, in order, in ds[lo:hi]: each at an occurrence after the one before such that all
    the rest can still be found (an initializer's string can equal a function's literal and stand before it)"""
    memo = {}

    def fits(i, pos):
        if i == len(lits):
            return True
        key = (i, pos)
        if key not in memo:
            at = ds.find(lits[i], pos, hi)
            memo[key] = at >= 0 and fits(i + 1, at + len(lits[i]))
        return memo[key]
    out = []
    pos = lo
    for i, s in enumerate(lits):                            # (the latest that lets the rest fit: a declaration's
        best = None                                         #  initializer strings stand before the next function's
        at = ds.find(s, pos, hi)                            #  literals, and may equal them)
        while at >= 0:
            if fits(i + 1, at + len(s)):
                best = at
            at = ds.find(s, at + 1, hi)
        if best is None:
            out.append(None)
            continue
        out.append(best)
        pos = best + len(s)
    return out


def externs(src, files=None):
    """{name: {(type, declarator)}} - how the functions declare the data they name (files: only those)"""
    decl = collections.defaultdict(set)
    for p in files if files is not None else glob.glob(os.path.join(src, "f*.c")):
        for l in open(p, encoding="latin1"):
            if not l.startswith("extern"):
                continue
            l = re.sub(r"/\*.*?\*/", "", l).strip().rstrip(";")
            m = re.match(r"extern\s+(unsigned\s+\w+|unsigned|struct\s+\w+|union\s+\w+|\w+)\s+(.*)", l)
            if not m or "(" in m.group(2):
                continue
            for it in m.group(2).split(","):
                n = re.search(r"\b(\w+)\s*(?:\[|$)", it.strip().lstrip("*"))
                if n:
                    decl[n.group(1)].add((m.group(1).strip(), it.strip()))
    return decl


def draft(prj, row):
    """{file name: text} - the module's data files proposed (the profile's rule says where each run goes)"""
    rule = prj.toolchain().p.get("data", {}).get("order", "after_next_function")
    if rule != "after_next_function":
        raise SystemExit("no data rule %r yet" % rule)
    rs, strs_, dstart, dend, fnaddr = walk(prj, row)
    ds = prj.image[prj.dgroup:]
    W = lambda o: struct.unpack_from("<H", ds, o)[0]
    syms = {}
    for n, (t, a) in prj.symbols().items():
        if t == "D" and ":" not in n and n.startswith("_"):
            syms.setdefault(a, n[1:])
    decl = externs(prj.src)
    mine = [q for q in modules.files_of(prj.src, row[1], row[2]) if os.path.basename(q).startswith("f")] if row[1] is not None else []
    local = set(externs(prj.src, mine)) if mine else set()
    for nm in decl:                                         # (the engine's names of places: g_ADDR)
        m = re.fullmatch(r"g_([0-9a-f]{4})", nm)
        if m and dstart <= int(m.group(1), 16) < dend and int(m.group(1), 16) not in syms:
            syms[int(m.group(1), 16)] = nm
    strs = {}
    k = dstart
    while k < dend:
        if ds[k] and 32 <= ds[k] < 127 or ds[k] in (9, 10):
            e = ds.index(b"\0", k)
            if all(32 <= c < 127 or c in (9, 10, 13) for c in ds[k:e]):
                strs[k] = ds[k:e]
                k = e + 1
                continue
        k += 1
    targets = {W(q) for q in range(dstart, dend - 1)}
    for v in targets:                                       # (a 0 pointed at, no variable's place: an empty string)
        if dstart <= v < dend and ds[v] == 0 and v not in syms and v not in strs and (v == dstart or ds[v - 1] == 0 or v - 1 in
                                                                                       {s + len(t) for s, t in strs.items()}):
            strs[v] = b""
    structs = parse_structs(prj)
    order = [n.lstrip("_") for n, a in fnaddr]
    file_of = {n.lstrip("_"): "f%05x" % a for n, a in fnaddr}
    files = collections.OrderedDict()
    varruns = []
    for lo, hi, after, kind in rs:
        if kind == "strings":
            continue
        if after == "(start)":
            q = lo                                          # (the file's start: its initializers' strings first)
            while q < hi:                                   # (a string nothing points at is a char array of its own)
                if q in strs and q in targets:
                    q += len(strs[q]) + 1
                elif ds[q] == 0 and q & 1:
                    q += 1
                else:
                    break
            lo = q
            fn = order[0] if order else None
        else:
            q = hi                                          # (after a function's literals: variables, then the
            while True:                                     #  strings of the initializers before the next one)
                cand = [s for s in strs if lo <= s < q and s + len(strs[s]) + 1 in (q, q - 1) and s in targets]
                if not cand:
                    break
                q = max(cand)
            hi = q
            fn = after.lstrip("_")                          # (its variables were declared before that function)
        if lo >= hi:
            continue
        key = ("d" + file_of[fn][1:] + ".c") if fn in file_of else (os.path.splitext(row[0])[0] + "-data.c" if row[1] is None else "e%05x.c" % fnaddr[-1][1])
        lo2 = lo + 1 if lo & 1 and ds[lo] == 0 else lo
        varruns.append((key, lo2, hi))
    # inside the variables' runs nothing is a literal: what looks like a string there is a variable's bytes, and a
    # word that points into a run points at a variable (one of its own, named after its place)
    for key, lo2, hi in varruns:
        for s in [s for s in strs if lo2 <= s < hi]:
            del strs[s]
    for key, lo2, hi in varruns:
        for t in targets:
            if lo2 < t < hi and t not in syms and t % 2 == 0:
                syms[t] = "g_%04x" % t
    for key, lo2, hi in varruns:
        lines = files.setdefault(key, [])
        places = sorted(set([lo2] + [a for a in syms if lo2 <= a < hi]))
        for a, nx in zip(places, places[1:] + [hi]):
            lines.append(_definition(ds, a, nx, syms, decl, strs, structs, local=local))
    defined = set()
    for t in files.values():
        defined |= set(re.findall(r"\b(\w+)(?=(?:\[\d*\])?\s*=)", "\n".join(t)))
    known = set(syms.values())
    for key, lines in files.items():                        # (the places named that are not the file's: externs)
        used = set()
        for l in lines:
            body = l.partition("=")[2]
            used |= {w for w in re.findall(r"\b(\w+)\b", re.sub(r'"(?:[^"\\]|\\.)*"', "", body)) if w in known}
        refs = sorted(used - defined)
        lines[:0] = ["extern %s %s;" % sorted(decl.get(r, {("int", r)}))[0] for r in refs]
    return {k: "\n".join(v) + "\n" for k, v in files.items()}


SIZES = {"char": 1, "unsigned char": 1, "int": 2, "unsigned": 2, "unsigned int": 2, "short": 2, "unsigned short": 2,
         "long": 4, "unsigned long": 4, "float": 4, "double": 8}


def parse_structs(prj):
    """{struct name: [(base type, stars, [dimensions], field name)]} of the project's headers"""
    texts = list(prj.headers().values())
    for d in prj.include:
        for f in glob.glob(os.path.join(d, "*.h")):
            texts.append(open(f, "rb").read())
    out = {}
    for t in texts:
        t = re.sub(r"/\*.*?\*/", " ", t.decode("latin1"), flags=re.S)
        for m in re.finditer(r"struct\s+(\w+)\s*\{([^{}]*)\}\s*;", t):
            fields = []
            for decl in m.group(2).split(";"):
                decl = " ".join(decl.split())
                if not decl:
                    continue
                dm = re.match(r"((?:unsigned\s+)?(?:struct\s+\w+|union\s+\w+|\w+))\s+(.*)", decl)
                if not dm:
                    continue
                for it in dm.group(2).split(","):
                    it = it.strip()
                    stars = len(it) - len(it.lstrip("*"))
                    nm = re.match(r"\**\s*(\w+)", it)
                    dims = [int(x, 0) for x in re.findall(r"\[(\w+)\]", it) if re.fullmatch(r"\d+|0x[0-9a-fA-F]+", x)]
                    if nm:
                        fields.append((dm.group(1), stars, dims, nm.group(1)))
            out[m.group(1)] = fields
    return out


def _sizeof(base, stars, dims, structs, align):
    if stars:
        n, al = 2, 2
    elif base.startswith("struct "):
        n, al = _struct_size(base[7:], structs, align)
    else:
        n = SIZES.get(base, 2)
        al = min(n, align)
    for d in dims:
        n *= d
    return n, al


def _struct_size(name, structs, align):
    pos = 0
    top = 1
    for base, stars, dims, f in structs.get(name, []):
        n, al = _sizeof(base, stars, dims, structs, align)
        pos += -pos % al
        pos += n
        top = max(top, al)
    return pos + (-pos % top), top


def _definition(ds, a, nx, syms, decl, strs, structs=None, align=2, local=None):
    W = lambda o: struct.unpack_from("<H", ds, o)[0]
    n = nx - a
    nm = syms.get(a, "g_%04x" % a)
    ty = decl.get(nm, set())
    words = [W(a + 2 * q) for q in range(n // 2)]

    def val(v):
        if v in strs:
            return cstr(strs[v])
        if v in syms and v != a:
            return ("&" if not any(i.endswith("]") for t, i in decl.get(syms[v], ())) else "") + syms[v]
        return None
    isarr = any(i.endswith("]") for t, i in ty)
    isptr = any("*" in i for t, i in ty)
    kinds = sorted(t for t, i in ty)
    base = next((k for k in kinds if k.startswith("struct ")), kinds[0] if kinds else None)
    ptrs = sum(1 for v in words if val(v) is not None)
    if ty and local is not None and nm not in local and ptrs >= 2 and (base in ("char", "int", "unsigned") or not isarr and not isptr):
        ty, isarr, isptr, base = set(), False, False, None     # (another module's type, and the bytes are a table of
                                                            #  places: the bytes say what it is)
    k = 2 if isptr and not isarr and not any("(" in i for t, i in ty) else SIZES.get(base) if not isarr and not isptr else None
    if ty and k and k < n and n - k >= 2:                   # (a variable, and after it one nothing names)
        return _definition(ds, a, a + k, syms, decl, strs, structs, align, local) + "\n" + \
            _definition(ds, a + k, nx, syms, decl, strs, structs, align, local)
    note = "                 /* (nothing names it) */" if not ty and a not in syms else ""
    if structs and base and base.startswith("struct ") and base[7:] in structs and not isptr:
        size, al = _struct_size(base[7:], structs, align)
        if size and n % size == 0:
            elems = [_struct_value(ds, a + k * size, base[7:], structs, align, val) for k in range(n // size)]
            if isarr or len(elems) > 1:
                return "%s %s[] = {\n    %s,\n};" % (base, nm, ",\n    ".join(elems))
            return "%s %s = %s;" % (base, nm, elems[0])
    if base and base not in ("char", "int", "unsigned", "long") and isptr:
        cast = "(%s *)" % base
        vals = [val(v) or ("0" if v == 0 else "(%s *)0x%04x /* ? */" % (base, v)) for v in words]
        vals = [(cast + x) if x.startswith("&") else x for x in vals]
        if isarr:
            return "%s *%s[%d] = { %s };" % (base, nm, n // 2, ", ".join(vals) if any(words) else "0")
        return "%s *%s = %s;" % (base, nm, vals[0])
    if base == "char" and isarr and not isptr:
        body = ds[a:nx]
        s = body.rstrip(b"\0")
        return "char %s[%d] = %s;%s" % (nm, n, cstr(s) if s and b"\0" not in s else "{ %s }" % ", ".join(str(c) for c in body) if s else '""', note)
    if n == 2:
        v = words[0]
        pv = val(v)
        if pv is not None and pv.startswith('"@(#)') and a not in syms:
            return "static char *sccsid = %s;" % pv
        if pv is not None and pv.startswith('"'):
            return "char *%s = %s;%s" % (nm, pv, note)
        if pv is not None:
            return "char *%s = (char *)%s;%s" % (nm, pv, note)
        if isptr:
            return "char *%s = %s;%s" % (nm, "0" if v == 0 else "(char *)0x%04x /* ? */" % v, note)
        return "%s %s = %d;%s" % (base if base in ("int", "unsigned") else "int", nm, v if base == "unsigned" else (v ^ 0x8000) - 0x8000, note)
    if n == 4 and base == "long":
        return "long %s = %dL;%s" % (nm, struct.unpack_from("<i", ds, a)[0], note)
    if n % 2 == 0 and (isptr or any(val(v) for v in words)):
        vs = [val(v) or ("0" if v == 0 else "(char *)0x%04x /* ? */" % v) for v in words]
        return "char *%s[%d] = { %s };%s" % (nm, n // 2, ", ".join(("(char *)" + x) if x.startswith("&") else x for x in vs), note)
    if n % 2 == 0:
        return "int %s[%d] = { %s };%s" % (nm, n // 2, ", ".join(str((v ^ 0x8000) - 0x8000) for v in words), note)
    return "char %s[%d] = { %s };%s" % (nm, n, ", ".join(str(c) for c in ds[a:nx]), note)


def _struct_value(ds, at, name, structs, align, val):
    W = lambda o: struct.unpack_from("<H", ds, o)[0]
    parts = []
    pos = 0
    for base, stars, dims, f in structs[name]:
        n, al = _sizeof(base, stars, dims, structs, align)
        pos += -pos % al
        p = at + pos
        if stars:
            v = W(p)
            x = val(v)
            if x is None:
                x = "0" if v == 0 else "(%s %s)0x%04x /* ? */" % (base, "*" * stars, v)
            elif x.startswith("&") or not x.startswith('"'):
                x = "(%s %s)%s" % (base, "*" * stars, x)
            parts.append(x)
        elif base.startswith("struct ") and not dims:
            parts.append(_struct_value(ds, p, base[7:], structs, align, val))
        elif base in ("char", "unsigned char") and dims:
            body = ds[p:p + n]
            s = body.rstrip(b"\0")
            parts.append(cstr(s) if b"\0" not in s else "{ %s }" % ", ".join(str(c) for c in body))
        elif SIZES.get(base, 2) == 1 and not dims:
            c = ds[p]
            parts.append(("'%s'" % chr(c)) if 32 < c < 127 and chr(c) not in "'" + BS else ("0x%02x" % c if c > 9 else str(c)))
        elif SIZES.get(base, 2) == 4 and not dims:
            parts.append("%dL" % struct.unpack_from("<i" if not base.startswith("unsigned") else "<I", ds, p)[0])
        elif not dims:
            v = W(p)
            parts.append(str(v if base.startswith("unsigned") else (v ^ 0x8000) - 0x8000))
        else:
            k = SIZES.get(base, 2)
            parts.append("{ %s }" % ", ".join(str(int.from_bytes(ds[p + j:p + j + k], "little")) for j in range(0, n, k)))
        pos += n
    while len(parts) > 1 and parts[-1] in ("0", "0L"):
        parts.pop()                                         # (the rest of an element left to the compiler's zeros)
    return "{ %s }" % ", ".join(parts)


def main(prj, argv):
    want = [a for a in argv if not a.startswith("-")]
    for row in modules.rows(prj):
        if want and row[0] not in want and os.path.splitext(row[0])[0] not in want:
            continue
        if row[0].upper().endswith(".OBJ"):
            continue
        if "--draft" in argv or "--write" in argv:
            for fn, text in draft(prj, row).items():
                print("== %s" % fn)
                print(text)
                p = os.path.join(prj.src, fn)
                if "--write" in argv:
                    if os.path.exists(p):
                        print("   (exists: not written)")
                    else:
                        open(p, "w", newline="\n").write(text)
            continue
        rs, strs, dstart, dend, fnaddr = walk(prj, row)
        print("%s: data %04x..%04x, %d literals of its functions" % (row[0], dstart, dend, len(strs)))
        for lo, hi, after, kind in rs:
            print("   %04x..%04x %4d bytes after %-10s %s" % (lo, hi, hi - lo, after, kind))
    return 0
