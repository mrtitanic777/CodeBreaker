"""struct_text.py -- which field accesses are fields: the module's decision, for the C back end.

    plan = struct_text.plan(module_path, docs, by_va, declared)   # {function: {param: (structure, {offset: type})}}
    header += struct_text.declarations(plan, used)

The decompiler writes what the machine does: `*(int4 *)(a1 + 0x48)`. A person wrote `p->width`.
Which pointers are which structure is worked out from the whole module's code
(analyze/structures.py: an argument handed on is the same structure in the function that
receives it); here that is decided for the module from its functions' trees, and the C back end
writes it (translate/backends/c._struct_fields). For every argument known to point at a
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

_SIZE = {"int1": 1, "uint1": 1, "xunknown1": 1, "undefined1": 1, "undefined": 1, "bool": 1, "char": 1,
         "int2": 2, "uint2": 2, "xunknown2": 2, "undefined2": 2,
         "int4": 4, "uint4": 4, "xunknown4": 4, "undefined4": 4, "float4": 4, "int": 4,
         "int8": 8, "uint8": 8, "xunknown8": 8, "undefined8": 8, "float8": 8}


def kind(base, stars):
    """(the field's type as written, its size) for an access `*(base stars)...`."""
    if len(stars) > 1:
        return base + " " + "*" * (len(stars) - 1), 4
    return (base, _SIZE[base]) if base in _SIZE else (None, 0)


def plan(module_path, docs, by_va, declared):
    """{function: {parameter: (structure, {offset: (type, size)})}} -- the accesses through each
    function's parameters that are a structure's fields, decided over the module: `docs`
    {function: its L2 document}, `declared` {function: its heading as written}. On any trouble
    in the analysis, nothing: this is for reading, never for meaning."""
    from codebreaker.translate.backends import c as cback
    try:
        from codebreaker.analyze import image, structures
        found = structures.recover(image.load(module_path))
    except Exception:
        return {}
    name_at = dict(by_va)
    owner = {}                                           # (function name, parameter name) -> structure
    for s in found:
        for root in s["roots"]:
            if root[0] == "arg" and root[1] in name_at and name_at[root[1]] in docs:
                fn = name_at[root[1]]
                params = cback.parameter_names(docs[fn])
                head = declared.get(fn, "")
                # (a function that takes `this` in a register has it first in its list, before
                # the arguments on the stack that the analysis counts from 0 -- as its heading is
                # written, where the module's C leaves the convention's word out)
                shift = 1 if "__thiscall" in head or "__fastcall" in head else 0
                if 0 <= root[2] + shift < len(params):
                    owner.setdefault((fn, params[root[2] + shift]), s["name"])
            elif root[0] == "this" and root[1] in name_at and name_at[root[1]] in docs:
                # the object a member function is called on: its first parameter, in ECX
                fn = name_at[root[1]]
                head = declared.get(fn, "")
                params = cback.parameter_names(docs[fn])
                if params and ("__thiscall" in head or "__fastcall" in head):
                    owner.setdefault((fn, params[0]), s["name"])
    if not owner:
        return {}
    # what type each field is read as, over the whole module
    votes = {}
    for (fn, param), sname in owner.items():
        for base, stars, off in cback.struct_accesses(docs[fn], param):
            k, size = kind(base, stars)
            if k:
                c = votes.setdefault(sname, {}).setdefault(off, {})
                c[(k, size)] = c.get((k, size), 0) + 1
    fields = {}                                          # structure -> {offset: (type, size)}
    for sname, by_off in votes.items():
        at, chosen = 0, {}
        for off in sorted(by_off):
            k, size = max(by_off[off].items(), key=lambda kv: (kv[1], kv[0]))[0]
            if off < at:
                continue                                 # inside the field before it
            chosen[off] = (k, size)
            at = off + size
        if len(chosen) >= 2:
            fields[sname] = chosen
    out = {}
    for (fn, param), sname in owner.items():
        if sname in fields:
            out.setdefault(fn, {})[param] = (sname, fields[sname])
    return out


def declarations(planned, used):
    """The header's declarations of the structures `used` (the ones a function was written with a
    field of), each field at its offset, gaps as padding (a line's end after them, whether or not
    there are any: the header the module had is followed by it wherever a plan was made)."""
    fields = {sname: f for by_param in planned.values() for sname, f in by_param.values()}
    decls = []
    for sname in sorted(used, key=lambda n: int(n.rsplit("_", 1)[1]) if n.rsplit("_", 1)[1].isdigit() else 0):
        lines, at = ["typedef struct %s {" % sname], 0
        for off, (k, size) in sorted(fields[sname].items()):
            if off > at:
                lines.append("  uint1 _gap_%x[%d];" % (at, off - at))
            star = k.count("*")
            lines.append("  %s %sf_%x;" % (k.rstrip("* "), "*" * star, off))
            at = off + size
        lines.append("} %s;" % sname)
        decls.append("\n".join(lines))
    return "\n".join(decls) + "\n"
