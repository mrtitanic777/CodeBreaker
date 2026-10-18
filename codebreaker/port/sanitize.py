"""sanitize.py -- what the port does wrong only when it runs: found by AddressSanitizer on the verify cases, mended.

An old program may write past the end of an array where its own linker put the next variable - harmless there
(what it overwrote it did not read again, or read as it meant to), memory of something else here. The compiler
cannot see it; a run can. The port is built with -fsanitize=address, every verify case run, and each overflow the
sanitizer reports becomes a rule of the port:

  a global array written (or read) past its end   the array given room for what the original reached
  a table read before its start (t[k - 0x101],    what the old linker had put there, by the program's data
  k at the end of the input: -1)                  segment's places (SYMBOLS): that variable read instead
"""
import glob
import math
import os
import re

GLOBAL = re.compile(r"AddressSanitizer: global-buffer-overflow.*?\n(?P<rw>READ|WRITE) of size (?P<sz>\d+).*?"
                    r"is located (?P<after>\d+) bytes after global variable '(?P<var>\w+)'.*?of size (?P<size>\d+)", re.S)


BEFORE = re.compile(r"AddressSanitizer: global-buffer-overflow.*?\n(?P<rw>READ) of size (?P<sz>\d+).*?\n\s*#0 0x[0-9a-fA-F]+ "
                    r"in \w+ (?P<path>[^\n]*?):(?P<line>\d+)", re.S)
BEFORE_VAR = re.compile(r"is located (?P<before>\d+) bytes before global variable '(?P<var>\w+)'.*?of size (?P<size>\d+)")

C_SIZES = {"char": (1, 1), "signed char": (1, 1), "unsigned char": (1, 1), "int16_t": (2, 2), "uint16_t": (2, 2),
           "short": (2, 2), "int32_t": (4, 4), "uint32_t": (4, 4), "int": (4, 4), "unsigned": (4, 4), "long": (4, 4),
           "float": (4, 4), "double": (8, 8), "intptr_t": (8, 8), "int64_t": (8, 8)}


def c_layout(body):
    """{member: (offset, size)} and the size of a structure of today's C (x64: pointers 8, natural alignment) - of
    its members' text (the port's: int16_t and the like); None when a member is not known"""
    from codebreaker.port import early_c
    off, out, align = 0, {}, 1
    for d in early_c.blank(body).split(";"):
        r = early_c.declarators(d.strip()) if d.strip() else None
        if not r:
            continue
        base = re.sub(r"\s+", " ", re.sub(r"\b(register|const|volatile|signed)\b(?= \w)", "", r[0])).strip()
        for nm, dcl in r[1]:
            n = 1
            for x in re.findall(r"\[\s*(\d+)\s*\]", dcl):
                n *= int(x)
            if "*" in dcl.replace(nm, "", 1):
                sz, al = 8, 8
            elif base in C_SIZES:
                sz, al = C_SIZES[base]
            else:
                return None, None
            off = (off + al - 1) // al * al
            out[nm] = (off, sz * n)
            off += sz * n
            align = max(align, al)
    return out, (off + align - 1) // align * align


INDEX = re.compile(r"(?P<path>[^\s:]+(?::[\\/][^\s:]+)?):(?P<line>\d+):(?P<col>\d+): runtime error: index (?P<idx>-\d+) "
                   r"out of bounds for type '(?P<type>[^']*)'")


def before_start(port, out, case, text):
    """a read before a table's start: what the original read there (the variable its linker put there) - a rule
    for the line; -> why, or None"""
    ds = getattr(port, "ds", {}) or {}
    ix = INDEX.search(text)
    given = None
    if ix:                                                  # (the bounds check's report: the row itself)
        f = os.path.basename(ix.group("path"))
        ln = int(ix.group("line"))
        if f not in out:
            return None
        line = out[f].split("\n")[ln - 1]
        names = [x for x in re.finditer(r"\b(\w+)\s*\[", line) if x.group(1) in ds]
        if not names:
            return None
        col = int(ix.group("col")) - 1
        var = min(names, key=lambda x: abs(x.start() - col)).group(1)
        given = int(ix.group("idx"))
        before = 0
    else:
        m = BEFORE.search(text)
        v = BEFORE_VAR.search(text)
        if not m or not v or v.group("var") not in ds:
            return None
        var, before = v.group("var"), int(v.group("before"))
        f = os.path.basename(m.group("path"))
        ln = int(m.group("line"))
        if f not in out:
            return None
        line = out[f].split("\n")[ln - 1]
    x = re.search(r"\b%s\s*\[([^\[\]]+)\]\s*\.\s*(\w+)" % re.escape(var), line)
    if not x or re.search(r"\+\+|--|[^=!<>]=[^=]", x.group(1)):
        return None
    field = x.group(2)
    from codebreaker.port import early_c
    decl = None
    for t in out.values():                                  # (the table's structure and its rows)
        d = re.search(r"\b(struct\s+\w+)\s+%s\s*\[[^\]]*\]\s*=\s*\{" % re.escape(var), early_c.blank(t))
        if d:
            decl = (d.group(1), t, d.end())
            break
    if not decl:
        for t in out.values():
            d = re.search(r"\b(struct\s+\w+)\s+%s\s*\[" % re.escape(var), early_c.blank(t))
            if d:
                decl = (d.group(1), t, None)
                break
    if not decl:
        return None
    st = re.sub(r"\s+", " ", decl[0])
    body = None
    for t in out.values():
        b = re.search(r"\b%s\s*\{([^{}]*)\}" % re.escape(st).replace(r"\ ", r"\s+"), early_c.blank(t))
        if b:
            body = t[b.start(1):b.end(1)]
            break
    lay, esz = c_layout(body) if body else (None, None)
    old = port.old_layouts_for(f) if hasattr(port, "old_layouts_for") else port.old_layouts()
    olay = old.get(st)
    if not lay or field not in lay or not olay or field not in olay:
        return None
    if given is not None:
        idx = given
    else:
        pos = -before                                       # (the place read, from the table's start, today)
        idx = (pos - lay[field][0]) // esz                  # (the row: below the first)
        if idx * esz + lay[field][0] != pos:
            return None                                     # (not that member of a row: another read)
    addr = ds[var] + idx * olay["@size"] + olay[field][0]   # (that row's member in the old data segment)
    owner = max((a, n) for n, a in ds.items() if a <= addr)
    o = addr - owner[0]
    w = owner[1]
    wexpr = None
    for t in out.values():                                  # (what is there: of the variable's own type)
        d = re.search(r"\b(struct\s+\w+)\s+%s\s*\[" % re.escape(w), early_c.blank(t))
        if d:
            wst = re.sub(r"\s+", " ", d.group(1))           # (declared where it is read now, its header too)
            port.extra_decls.setdefault(f, set()).add("extern %s %s[];" % (wst, w))
            hs = [h for h, ht in out.items() if h.endswith(".h") and
                  re.search(r"\b%s\s*\{" % re.escape(wst).replace(r"\ ", r"\s+"), early_c.blank(ht))]
            if hs:
                port.need_headers.setdefault(f, set()).add(hs[0])
            wl = old.get(re.sub(r"\s+", " ", d.group(1)))
            if wl:
                e, rem = divmod(o, wl["@size"])
                mem = next((k for k, val in wl.items() if k != "@size" and val[0] == rem), None)
                if mem:
                    wexpr = "%s[%d].%s" % (w, e, mem)
            break
    if wexpr is None and o == 0:
        wexpr = w
    if wexpr is None:
        return None
    o_ = port.line_origin.get(f, {}).get(ln)
    new = line[:x.start()] + "((%s) == %d ? %s : %s)" % (x.group(1).strip(), idx, wexpr, x.group(0)) + line[x.end():]
    port.line_fixes[(f, o_[0], o_[1]) if o_ else (f, line)] = new
    why = "%s:%d %s[%d].%s (case '%s'): before the table - the original read %s there" % (
        f, ln, var, idx, field, case, wexpr)
    port.mended["a table read before its start (as the original did: its linker's variable there): that "
                "variable read"].append(why)
    return why


def runtime_dir(cc):
    """where clang's sanitizer runtime is, beside the compiler (Windows: a DLL the program needs on its path)"""
    root = os.path.dirname(os.path.dirname(os.path.abspath(cc)))
    for p in sorted(glob.glob(os.path.join(root, "lib", "clang", "*", "lib", "windows")), reverse=True):
        if glob.glob(os.path.join(p, "clang_rt.asan_dynamic*.dll")):
            return p
    return None


def mend(port, out, reports):
    """the rules from the sanitizer's reports; -> what was mended ([(variable, why)])"""
    done = []
    for case, text in reports:
        found = False
        for ix in INDEX.finditer(text):                     # (each index the bounds check reported: one a read
            why = before_start(port, out, case, ix.group(0))   # before a table's start, perhaps)
            if why and why not in [w for c_, w in done]:
                done.append((case, why))
                found = True
        if found:
            continue
        if "bytes before global variable" in text and "bytes after global variable" not in \
                text.split("bytes before global variable")[0][-400:]:
            why = before_start(port, out, case, text)
            if why:
                done.append((case, why))
                continue
        for m in GLOBAL.finditer(text):
            var = m.group("var")
            reach = getattr(port, "reach", None)
            if reach is not None and var not in reach and hasattr(port, "joins_next") and port.joins_next(var):
                elems = re.search(r"(?m)^\s*(?:unsigned\s+|signed\s+)?(?:char|short|int|long|unsigned)\s+%s\s*\[[^\]]*\]"
                                  r"\s*=\s*\{([^}]*)\}" % re.escape(var), "\n".join(port.files.values()))
                n = len([x for x in elems.group(1).split(",") if x.strip()]) if elems else 0
                if n:                                       # (read into the variable after it: one table, as the
                    elem = int(m.group("size")) // n        #  original's linker made them - its elements)
                    reach[var] = (int(m.group("size")) + int(m.group("after")) + int(m.group("sz")) - 1) // elem
                    why = "%s[%d] %s (case '%s'): the table went on into the variables after it - one table" % (
                        var, reach[var], m.group("rw").lower(), case)
                    port.mended["a table read past its end into the variables after it (a run showed it): one "
                                "table"].append(why)
                    done.append((var, why))
                    continue
            decl = None
            for f, t in out.items():                        # (its definition: not a declaration of it, extern)
                d = re.search(r"(?m)^\s*(?!extern\b)(?:static\s+)?(?:[\w ]*?\b)?(?:char|short|int|long|unsigned|signed|"
                              r"float|double|struct\s+\w+|union\s+\w+|\w+_t)\b[\s*]+%s\s*\[\s*(\d+)\s*\]" %
                              re.escape(var), t)
                if d:
                    decl = int(d.group(1))
                    break
            sized = True
            if not decl:                                    # (char s[] = "...": its size the string's)
                for f, t in out.items():
                    if re.search(r"(?m)^\s*(?:static\s+)?(?:unsigned\s+|signed\s+)?char\s+%s\s*\[\s*\]\s*=" %
                                 re.escape(var), t):
                        decl, sized = int(m.group("size")), False
                        break
            if not decl or port.grown.get(var, 0) > decl:
                continue                                    # (grown already, this round)
            elem = int(m.group("size")) // decl
            extra = math.ceil((int(m.group("after")) + int(m.group("sz"))) / elem)
            port.grown[var] = decl + extra
            if sized:
                port.decl_rules.append((r"(?m)^(\s*(?:extern\s+|static\s+)?(?:[\w ]*?\b)?(?:char|short|int|long|"
                                        r"unsigned|signed|float|double|struct\s+\w+|union\s+\w+|\w+_t)\b[\s*]+%s"
                                        r"\s*\[)\s*%d\s*\]" % (re.escape(var), decl), r"\g<1>%d]" % (decl + extra)))
            else:
                port.decl_rules.append((r"(?m)^(\s*(?:static\s+)?(?:unsigned\s+|signed\s+)?char\s+%s\s*\[)\s*\]" %
                                        re.escape(var), r"\g<1>%d]" % (decl + extra)))
            why = "%s[%d] %s (case '%s'): the original reached into the variable its linker put after it" % (
                var, decl + extra - 1, "written" if m.group("rw") == "WRITE" else "read", case)
            port.mended["array reached past its end (as the original did, harmlessly there): room made for it"].append(why)
            done.append((var, why))
    return done
