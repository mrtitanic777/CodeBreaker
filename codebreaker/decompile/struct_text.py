"""struct_text.py -- write field accesses as fields.

    header, texts = struct_text.apply(module_path, header, texts, by_va)

The decompiler writes what the machine does: `*(int4 *)(a1 + 0x48)`. A person wrote `p->width`.
Which pointers are which structure is worked out from the whole module's code
(analyze/structures.py: an argument handed on is the same structure in the function that
receives it); here that is put into the source. For every argument known to point at a
structure, an access through it at a constant offset becomes a field of that structure:

    *(int4 *)(a1 + 0x48)        ->   ((struct_7 *)a1)->f_48

and the structure is declared in the header with each field at its offset, gaps as padding.
The field's type is the type most of the module's accesses to that offset use; an access with
another type (one byte of a word, a signed read of an unsigned field) is left as it was, so
every rewritten access means exactly what it meant: the same bytes, read as the same type.
Nothing else changes -- the argument stays the number the decompiler made it.

Fields have no names in a binary; `f_48` says where the field is. Names are for a person (or a
header that documents the structure) to give.
"""
import re

_SIZE = {"int1": 1, "uint1": 1, "xunknown1": 1, "undefined1": 1, "undefined": 1, "bool": 1, "char": 1,
         "int2": 2, "uint2": 2, "xunknown2": 2, "undefined2": 2,
         "int4": 4, "uint4": 4, "xunknown4": 4, "undefined4": 4, "float4": 4, "int": 4,
         "int8": 8, "uint8": 8, "xunknown8": 8, "undefined8": 8, "float8": 8}
_NUMBER = r"(?:0x[0-9a-fA-F]+|\d+)"


def _access(param):
    """Matches `*(T *)(param + N)` and `*(T *)param` -- T a plain type, or pointers to one."""
    p = re.escape(param)
    return re.compile(r"\*\((\w+) ?(\*+)\)(?:\(%s \+ (%s)\)|%s(?![\w(]))" % (p, _NUMBER, p))


def _parameters(text):
    head = text.split("{", 1)[0]
    m = re.search(r"\(([^()]*)\)\s*$", head.strip())
    if not m or m.group(1).strip() in ("", "void"):
        return []
    return [x.strip().split()[-1].lstrip("*") for x in m.group(1).split(",")]


def _kind(base, stars):
    """(the field's type as written, its size) for an access `*(base stars)...`."""
    if len(stars) > 1:
        return base + " " + "*" * (len(stars) - 1), 4
    return (base, _SIZE[base]) if base in _SIZE else (None, 0)


def apply(module_path, header, texts, by_va):
    """-> (header, texts) with the accesses that are fields written as fields. On any trouble in
    the analysis the source is returned as it was: this is for reading, never for meaning."""
    try:
        from codebreaker.analyze import image, structures
        found = structures.recover(image.load(module_path))
    except Exception:
        return header, texts
    name_at = dict(by_va)
    owner = {}                                           # (function name, parameter name) -> structure
    for s in found:
        for root in s["roots"]:
            if root[0] == "arg" and root[1] in name_at:
                fn = name_at[root[1]]
                params = _parameters(texts.get(fn, ""))
                head = texts.get(fn, "").split("{", 1)[0]
                # (a function that takes `this` in a register has it first in its list, before
                # the arguments on the stack that the analysis counts from 0)
                shift = 1 if "__thiscall" in head or "__fastcall" in head else 0
                if 0 <= root[2] + shift < len(params):
                    owner.setdefault((fn, params[root[2] + shift]), s["name"])
            elif root[0] == "this" and root[1] in name_at:
                # the object a member function is called on: its first parameter, in ECX
                fn = name_at[root[1]]
                head = texts.get(fn, "").split("{", 1)[0]
                params = _parameters(texts.get(fn, ""))
                if params and ("__thiscall" in head or "__fastcall" in head):
                    owner.setdefault((fn, params[0]), s["name"])
    if not owner:
        return header, texts
    # what type each field is read as, over the whole module
    votes = {}
    for (fn, param), sname in owner.items():
        body = texts[fn].partition("{")[2]
        for m in _access(param).finditer(body):
            kind, size = _kind(m.group(1), m.group(2))
            if kind:
                off = int(m.group(3), 0) if m.group(3) else 0
                c = votes.setdefault(sname, {}).setdefault(off, {})
                c[(kind, size)] = c.get((kind, size), 0) + 1
    fields = {}                                          # structure -> {offset: (type, size)}
    for sname, by_off in votes.items():
        at, chosen = 0, {}
        for off in sorted(by_off):
            kind, size = max(by_off[off].items(), key=lambda kv: (kv[1], kv[0]))[0]
            if off < at:
                continue                                 # inside the field before it
            chosen[off] = (kind, size)
            at = off + size
        if len(chosen) >= 2:
            fields[sname] = chosen
    if not fields:
        return header, texts
    out, used = dict(texts), set()
    for (fn, param), sname in owner.items():
        if sname not in fields:
            continue
        head, brace, body = out[fn].partition("{")

        def field(m, sname=sname, param=param):
            kind, _size = _kind(m.group(1), m.group(2))
            off = int(m.group(3), 0) if m.group(3) else 0
            if kind is None or fields[sname].get(off, (None,))[0] != kind:
                return m.group(0)
            used.add(sname)
            return "((%s *)%s)->f_%x" % (sname, param, off)
        out[fn] = head + brace + _access(param).sub(field, body)
    decls = []
    for sname in sorted(used, key=lambda n: int(n.rsplit("_", 1)[1]) if n.rsplit("_", 1)[1].isdigit() else 0):
        lines, at = ["typedef struct %s {" % sname], 0
        for off, (kind, size) in sorted(fields[sname].items()):
            if off > at:
                lines.append("  uint1 _gap_%x[%d];" % (at, off - at))
            star = kind.count("*")
            lines.append("  %s %sf_%x;" % (kind.rstrip("* "), "*" * star, off))
            at = off + size
        lines.append("} %s;" % sname)
        decls.append("\n".join(lines))
    return header + "\n".join(decls) + "\n", out
