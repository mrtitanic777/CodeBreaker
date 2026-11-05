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

_SPEC = re.compile(r"%([-+ 0#]*)(\*|\d+)?(?:\.(\*|\d+))?(hh|h|ll|l|L|w|I64|I32)?([diouxXeEfgGcCsSpn%])")


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


def _ask(img, what):
    """A strings question to the DLL, with the image's names as they are now."""
    h = img._handle()
    h.names(img)
    return h.ask("strings." + what)


def _table(img):
    return {va: (text, kind, referenced) for va, text, kind, referenced in _ask(img, "table")}


def table(img):
    return img.memo("strings", _table)


def _uses(img):
    tab = table(img)
    out = {}
    for f, rows in _ask(img, "uses"):
        out[f] = [tuple(r) for r in rows]
        if img.bits == 16:                              # a use from 16-bit code makes the string referenced
            for _site, va, _text in rows:
                tab[va] = (tab[va][0], tab[va][1], True)
    return out


def uses(img):
    return img.memo("strings.uses", _uses)


def stack_strings(img):
    """[(function, "ebp-0x20", text, wide)] -- strings built by storing immediates into the frame."""
    return [tuple(r) for r in _ask(img, "stack")]


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
    table(img)
    got = _ask(img, "formats")
    out = [{"addr": f, "kind": "format", "site": site, "function": api, "format": text,
            "arguments": conversions(text, wide) if listed else None}
           for f, site, api, text, wide, listed in got["rows"]]
    return out, got["sites"], {dest: k for dest, k in got["own"]}


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
