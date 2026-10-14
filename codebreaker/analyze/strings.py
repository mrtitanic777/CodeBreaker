"""strings.py -- a module's strings: where they are, who uses them, and what they say.

    from codebreaker.analyze import image, strings
    img = image.load("comctl32.dll")
    strings.table(img)            {address: (text, "A" | "W", referenced)}
    strings.at(img, va)           (text, "A" | "W") of the string at an address, or None
    strings.uses(img)             {function: [(site, address, text)]}
    strings.stack_strings(img)    [(function, where, text)]  strings assembled by immediate stores
    strings.formats(img)          [call sites of printf-like functions with their format string
                                   and the argument types it implies]

A string is a run of printable characters ending in a zero byte (or of 16-bit characters ending
in a zero word). One the code refers to is believed from two characters up; one nothing refers
to is listed only from five, and only outside the code (early linkers put constants in the code
section, where an unreferenced "string" is usually instructions).

A format string says what follows it in the argument list: `"%s = %ld"` is a string pointer and
a long. That types the unnamed arguments of every call to wsprintf and its relatives -- and
finds the module's own printf-like functions: a function that is handed a literal with
conversions in it, at the same position, from several places.
"""
import re

from codebreaker.analyze import call_graph, image
from codebreaker.analyze.image import family

_SPEC = re.compile(r"%([-+ 0#]*)(\*|\d+)?(?:\.(\*|\d+))?(hh|h|ll|l|L|w|I64|I32)?([diouxXeEfgGcCsSpn%])")
# printf-like imports: position of the format argument
_FORMATTERS = {"wsprintf": 1, "wvsprintf": 1, "sprintf": 1, "swprintf": 1, "vsprintf": 1, "printf": 0,
               "wprintf": 0, "fprintf": 1, "fwprintf": 1, "_snprintf": 2, "_snwprintf": 2, "sscanf": 1,
               "swscanf": 1, "fscanf": 1, "_vsnprintf": 2, "wnsprintf": 2, "_wsprintf": 1, "_cprintf": 0}
_NO_LIST = ("wvsprintf", "vsprintf", "_vsnprintf")          # the arguments are elsewhere (a va_list)


def _ascii(data, start, least):
    end = start
    n = len(data)
    while end < n and (0x20 <= data[end] < 0x7F or data[end] in (9, 10, 13)):
        end += 1
    if end - start >= least and end < n and data[end] == 0:
        return data[start:end].decode("latin1")
    return None


def _wide(data, start, least):
    end = start
    n = len(data)
    while end + 1 < n and data[end + 1] == 0 and (0x20 <= data[end] < 0x7F or data[end] in (9, 10, 13)):
        end += 2
    if (end - start) // 2 >= least and end + 1 < n and data[end] == 0 and data[end + 1] == 0:
        return data[start:end:2].decode("latin1")
    return None


def at(img, va, least=2):
    """The string at exactly `va`: (text, "A" | "W"), or None."""
    s = img.span(va)
    if not s or va - s[0] >= s[5]:
        return None
    off = va - s[0]
    data = s[1]
    if img.bits == 32:
        w = _wide(data, off, least)
        if w is not None:
            return w, "W"
    a = _ascii(data, off, least)
    if a is not None and not (len(a) < 4 and off and 0x20 <= data[off - 1] < 0x7F):    # not the tail of a longer one
        return a, "A"
    return None


def _table(img):
    graph = call_graph.build(img)
    out = {}
    referenced = {va for _f, _i, va, how in graph.refs if how == "a"}
    if img.pointers:
        referenced |= {t for site, (_k, t) in img.pointers.items() if t is not None and not img.is_code(site)}
    for va in referenced:
        if va in img.names:
            continue
        s = at(img, va)
        if s:
            out[va] = (s[0], s[1], True)
    for base, data, executable, _w, _n, held in img.spans:          # what nothing refers to
        if executable or _n in (".rsrc", ".reloc", ".edata", ".idata"):
            continue
        off = 0
        while off < held:
            if 0x20 <= data[off] < 0x7F:
                found = _ascii(data, off, 5 if img.bits == 32 else 4)
                size = 1
                if found is None and img.bits == 32 and off + 1 < held and data[off + 1] == 0:
                    found, size = _wide(data, off, 5), 2
                if found is not None:
                    out.setdefault(base + off, (found, "A" if size == 1 else "W", False))
                    off += (len(found) + 1) * size
                    continue
                while off < held and 0x20 <= data[off] < 0x7F:      # skip the rest of a run that is not one
                    off += 1
            off += 1
    return out


def table(img):
    return img.memo("strings", _table)


def _near_ds(fn, i):
    """Is the data segment pushed or stored right beside instruction i (a far pointer being built)?"""
    for b in fn.insns[max(0, i - 2):i + 3]:
        if any(op[0] == "r" and op[1] == "ds" for op in b.ops):
            return True
    return False


def _uses(img):
    tab = table(img)
    out = {}
    if img.bits == 32:
        for f, ins, va, how in call_graph.build(img).refs:
            if how == "a" and va in tab:
                out.setdefault(f, []).append((ins.addr, va, tab[va][0]))
        return out
    lo = img.data_segment
    for fn in img.each():                               # 16-bit: an offset into the data segment, beside DS
        for i, ins in enumerate(fn.insns):
            if ins.mn not in ("push", "mov", "lea") or ins.flow:
                continue
            for op in ins.ops[-1:]:
                va = None
                if op[0] == "i" and op[2] == 2 and op[1] >= 0x10:
                    va = lo + op[1]
                elif op[0] == "m" and ins.mn == "lea" and op[1] is None and op[2] is None:
                    va = lo + (op[4] & 0xFFFF)
                if va is not None and lo and va in tab and (ins.mn == "lea" or _near_ds(fn, i)):
                    out.setdefault(fn.start, []).append((ins.addr, va, tab[va][0]))
                    tab[va] = (tab[va][0], tab[va][1], True)
    return out


def uses(img):
    return img.memo("strings.uses", _uses)


def _assemble(stores, least=4):
    """Printable runs in {(base, offset): (byte, store number)} -> [(base, offset, text, wide)]"""
    out = []
    keys = sorted(stores)
    i = 0
    while i < len(keys):
        base, off = keys[i]
        j = i
        while j + 1 < len(keys) and keys[j + 1] == (base, keys[j][1] + 1):
            j += 1
        run = [stores[k] for k in keys[i:j + 1]]
        start = 0
        for k in range(len(run) + 1):
            if k == len(run) or not 0x20 <= run[k][0] < 0x7F:
                seg = run[start:k]
                text = "".join(chr(b) for b, _n in seg)
                if len(seg) >= least and len({n for _b, n in seg}) >= 2 and re.search("[A-Za-z]{2}", text):
                    out.append((base, off + start, text, False))
                start = k + 1
        # 16-bit characters: letter, 0, letter, 0
        k = 0
        while k + 1 < len(run):
            m = k
            while m + 1 < len(run) and 0x20 <= run[m][0] < 0x7F and run[m + 1][0] == 0:
                m += 2
            if (m - k) // 2 >= 3:
                text = "".join(chr(run[x][0]) for x in range(k, m, 2))
                if re.search("[A-Za-z]{2}", text):
                    out.append((base, off + k, text, True))
            k = max(m, k + 1)
        i = j + 1
    return out


def stack_strings(img):
    """[(function, "ebp-0x20", text, wide)] -- strings built by storing immediates into the frame."""
    out = []
    for fn in img.each():
        stores, n = {}, 0
        for ins in fn.insns:
            ops = ins.ops
            if ins.mn == "mov" and len(ops) == 2 and ops[0][0] == "m" and ops[1][0] == "i" and ops[0][1] \
                    and family(ops[0][1]) in ("ebp", "esp") and ops[0][2] is None:
                n += 1
                for k in range(ops[0][6]):
                    stores[(family(ops[0][1]), ops[0][4] + k)] = ((ops[1][1] >> (8 * k)) & 0xFF, n)
        if len(stores) >= 4:
            for base, off, text, wide in _assemble(stores):
                out.append((fn.start, "%s%+#x" % (base, off), text, wide))
    return out


def conversions(fmt, wide=False):
    """The C types of the arguments a format string consumes, in order."""
    types = []
    for _flags, width, prec, length, conv in _SPEC.findall(fmt):
        if conv == "%":
            continue
        types += ["int"] * ((width == "*") + (prec == "*"))
        if conv in "sS":
            types.append("wchar_t *" if (length in ("l", "w")) or ((conv == "S") != wide) else "char *")
        elif conv in "cC":
            types.append("wchar_t" if (conv == "C") != wide else "char")
        elif conv == "p":
            types.append("void *")
        elif conv == "n":
            types.append("int *")
        elif conv in "eEfgG":
            types.append("double")
        else:
            signed = conv in "di"
            if length in ("ll", "I64"):
                types.append("__int64" if signed else "unsigned __int64")
            elif length == "l":
                types.append("long" if signed else "unsigned long")
            elif length == "h":
                types.append("short" if signed else "unsigned short")
            else:
                types.append("int" if signed else "unsigned")
    return types


def _formatter(name):
    """(position of the format argument, takes a wide format) for a printf-like import, or None."""
    bare = name.split("!")[-1]
    stem = bare[:-1] if bare[-1:] in "AW" and bare[:-1].lower() in _FORMATTERS else bare
    k = _FORMATTERS.get(stem.lower())
    if k is None:
        return None
    return k, bare.endswith("W") or "w" in stem.lower().replace("wsprintf", "").replace("wvsprintf", "").replace("wnsprintf", "")


def literal(img, args, k):
    """The string a call's k-th argument points at: (address, text, kind), or None."""
    tab = table(img)
    if img.bits == 32:
        if k < len(args) and args[k][0] == "i" and args[k][1] in tab:
            return (args[k][1],) + tab[args[k][1]][:2]
        return None
    # 16-bit: a far pointer is two words, offset pushed last; the format is the k-th far pointer
    lo, hi = 2 * k, 2 * k + 1
    if hi < len(args) and args[lo][0] == "i":
        seg = args[hi]
        base = img.data_segment if seg[0] == "r" and seg[1] == "ds" else \
            (seg[1] << 16) if seg[0] == "i" and img.span(seg[1] << 16) else None
        if base is not None and base + args[lo][1] in tab:
            va = base + args[lo][1]
            return (va,) + tab[va][:2]
    return None


def _formats(img):
    graph = call_graph.build(img)
    out, seen_sites, candidates = [], 0, {}
    for f in img.functions:
        fn = img.function(f)
        for site, kind, dest in graph.calls[f]:
            if dest is None:
                continue
            i = fn.at(site)
            if dest in img.imports:
                fm = _formatter(img.imports[dest])
                if fm is None:
                    continue
                seen_sites += 1
                args = image.arguments(fn, i, 2 * (fm[0] + 1) if img.bits == 16 else fm[0] + 1)
                lit = literal(img, args, fm[0])
                if lit:
                    api = img.imports[dest]
                    listed = not any(api.split("!")[-1].lower().startswith(n) for n in _NO_LIST)
                    out.append({"addr": f, "kind": "format", "site": site, "function": api, "format": lit[1],
                                "arguments": conversions(lit[1], fm[1] or lit[2] == "W") if listed else None})
            elif kind == "call" and img.bits == 32:      # the module's own printf-like functions
                args = image.arguments(fn, i, 4)
                for k, a in enumerate(args):
                    if a[0] == "i" and a[1] in table(img) and _SPEC.search(table(img)[a[1]][0].replace("%%", "")):
                        candidates.setdefault((dest, k), []).append((f, site, a[1]))
    own = {}
    for (dest, k), sites in candidates.items():
        if len(sites) >= 2 and len({s[2] for s in sites}) >= 2:
            own[dest] = k
            for f, site, va in sites:
                text, kind = table(img)[va][:2]
                out.append({"addr": f, "kind": "format", "site": site, "function": img.label(dest), "format": text,
                            "arguments": conversions(text, kind == "W")})
    return out, seen_sites, own


def formats(img):
    return img.memo("strings.formats", _formats)


def analyse(img):
    tab, use = table(img), uses(img)
    stack = stack_strings(img)
    fmts, sites, own = formats(img)
    facts = [{"addr": None, "kind": "string", "at": va, "text": t, "wide": k == "W", "referenced": r}
             for va, (t, k, r) in sorted(tab.items())]
    for f, rows in sorted(use.items()):
        facts.append({"addr": f, "kind": "string_uses", "strings": sorted({t for _s, _v, t in rows})})
    facts += [{"addr": f, "kind": "stack_string", "where": w, "text": t, "wide": wide} for f, w, t, wide in stack]
    facts += fmts
    facts += [{"addr": f, "kind": "printf_like", "format_argument": k} for f, k in sorted(own.items())]
    counts = {"strings": len(tab), "referenced": sum(1 for v in tab.values() if v[2]),
              "wide": sum(1 for v in tab.values() if v[1] == "W"),
              "functions_using_strings": len(use), "string_uses": sum(len(r) for r in use.values()),
              "stack_strings": len(stack), "formatter_call_sites": sites,
              "format_strings_read": sum(1 for f in fmts if "!" in f["function"]),
              "own_printf_like_functions": len(own),
              "format_strings_at_own_functions": sum(1 for f in fmts if "!" not in f["function"]),
              "arguments_typed_by_formats": sum(len(f["arguments"] or ()) for f in fmts)}
    text = ["stack string in %s at %s: %r" % (img.label(f), w, t) for f, w, t, _x in stack[:8]]
    text += ["%s(%r) in %s -> %s" % (f["function"].split("!")[-1], f["format"][:50], img.label(f["addr"]),
                                     ", ".join(f["arguments"] or ["(argument list elsewhere)"])) for f in fmts[:10]]
    text += ["printf-like: %s, format is argument %d" % (img.label(f), k) for f, k in sorted(own.items())[:8]]
    return {"counts": counts, "facts": facts, "text": text}
