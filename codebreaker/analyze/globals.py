"""globals.py -- a module's global variables and its initialised tables of addresses.

    from codebreaker.analyze import image, globals as global_variables
    img = image.load("comctl32.dll")
    global_variables.variables(img)        {address: fact}
    global_variables.pointer_tables(img)   [table]

A variable is an address the code reads or writes by name -- in 32-bit code an operand that is
an address inside the module, in 16-bit code a fixed offset into the data segment. For each:

    size        the widest access to it
    type        what its uses say it is (see _type): a string, a function pointer (called
                through, or initialised with a function's address), a pointer (what it holds is
                dereferenced), a far pointer, a float or double, a counter, the result of a named
                system call (a handle), an array (indexed, with its element size), else an
                integer of its size
    readers, writers    the functions that do each;  taken: its address is passed around
    initial     the value the file gives it, or "uninitialised"
    values      the constants the code stores in it or compares it with (a small set is a state
                or a mode)
    extent      bytes to the next thing known at a higher address -- room for an array or a
                structure that starts here

A table of addresses is a run of places where the file says the loader writes an address
(relocation records), outside the instructions: tables of handlers, virtual tables, arrays of
string pointers; and, as "records", addresses repeating at a fixed stride (an array of structures
with a pointer in it). A module without relocation records gets only the runs whose every entry
is a function or a string.
"""
import bisect

from codebreaker.analyze import call_graph, image, strings
from codebreaker.analyze.image import family

_WORD = {1: "unsigned char", 2: "unsigned short", 4: "unsigned long", 8: "double", 10: "long double"}


def _operand(img, ins, va):
    """The memory operand of `ins` that names `va`: (operand, position), or (None, None)."""
    for k, op in enumerate(ins.ops):
        if op[0] == "m" and op[1] is None:
            if (op[4] if img.bits == 32 else img.data_segment + (op[4] & 0xFFFF)) == va:
                return op, k
    return None, None


def _dereferenced(fn, i, reg):
    """Is register `reg`, loaded at fn.insns[i], used as an address before it is overwritten?"""
    fam = family(reg)
    for b in fn.insns[i + 1:i + 9]:
        if any(op[0] == "m" and ((op[1] and family(op[1]) == fam) or (op[2] and family(op[2]) == fam)) for op in b.ops):
            return True
        if fam in b.wr or b.flow in ("jmp", "ret") or (b.flow == "call" and fam in ("eax", "ecx", "edx")):
            return False
    return False


def _variables(img):
    graph = call_graph.build(img)
    text = strings.table(img)
    out = {}
    for f, ins, va, how in graph.refs:
        if va in img.names or va in img.imports:
            continue
        s = img.span(va)
        if s is None or (s[2] and img.covering(va) is not None):
            continue                                            # an instruction, not data
        g = out.get(va)
        if g is None:
            g = out[va] = {"addr": None, "kind": "global", "at": va, "size": 0, "readers": set(), "writers": set(),
                           "taken": 0, "notes": set(), "values": set(), "writable": s[3], "element": 0}
        op, k = _operand(img, ins, va)
        if how == "a":
            g["taken"] += 1
            continue
        if op is None:                                          # [reg + ADDRESS]: indexed from a register
            g["notes"].add("array")
            for o in ins.ops:
                if o[0] == "m" and o[1] is not None:
                    g["element"] = max(g["element"], o[3] if o[2] else o[6])
            (g["readers"] if how == "r" else g["writers"]).add(f)
            continue
        g["size"] = max(g["size"], op[6])
        if "r" in how:
            g["readers"].add(f)
        if "w" in how:
            g["writers"].add(f)
        if op[2] is not None:
            g["notes"].add("array")
            g["element"] = max(g["element"], op[3] if op[3] > 1 else op[6])
        mn, fn = ins.mn, img.function(f)
        i = fn.at(ins.addr)
        if ins.flow == "call":
            g["notes"].add("called")
        elif mn[0] == "f" and mn != "fwait":
            g["notes"].add("integer in the FPU" if mn.startswith("fi") else "float")
        elif mn in ("les", "lds"):
            g["notes"].add("far pointer")
        elif mn in ("inc", "dec") or (mn in ("add", "sub") and len(ins.ops) == 2 and ins.ops[1][:2] == ("i", 1) and k == 0):
            g["notes"].add("counter")
        elif mn in ("mov", "cmp", "test", "or", "and") and len(ins.ops) == 2 and ins.ops[1][0] == "i" and k == 0:
            if not (img.pointers and img.pointers.get(ins.addr + ins.size - op[6])):
                g["values"].add(ins.ops[1][1])
            if mn in ("or", "and", "test"):
                g["notes"].add("flags")
        if mn == "mov" and k == 1 and ins.ops[0][0] == "r" and _dereferenced(fn, i, ins.ops[0][1]):
            g["notes"].add("pointer")
        if mn == "mov" and k == 0 and ins.ops[1][0] == "r" and family(ins.ops[1][1]) == "eax":
            src = image.definition(fn, i, ins.ops[1][1])
            if src and src[0] == "call" and src[1] in img.imports:
                g["notes"].add("result of " + img.imports[src[1]].split("!")[-1])
    known = sorted(set(out) | set(text))
    for va, g in out.items():
        s = img.span(va)
        k = bisect.bisect_right(known, va)
        end = s[0] + len(s[1])
        g["extent"] = min((known[k] if k < len(known) and known[k] < end else end) - va, 0x10000)
        size = g["size"] or g["element"] or (4 if img.bits == 32 else 2)
        if not img.initialised(va):
            g["initial"] = "uninitialised"
            held = None
        else:
            held = int.from_bytes(img.read(va, min(size, 8)), "little")
            g["initial"] = held
        target = (img.pointers or {}).get(va)
        if target and target[1] is not None:
            g["initial"] = "&" + img.label(target[1])
            if target[1] in img.names:
                g["notes"].add("called")
            elif target[1] in img.imports:
                g["notes"].add("called")
            elif target[1] in text:
                g["notes"].add("string pointer")
            else:
                g["notes"].add("pointer")
        g["type"] = _type(img, g, text.get(va), size)
        g["size"] = size
    return out


def _type(img, g, string, size):
    notes = g["notes"]
    if string and not g["writers"]:
        return "%s[%d]" % ("wchar_t" if string[1] == "W" else "char", len(string[0]) + 1)
    if "called" in notes:
        return "function pointer"
    if "far pointer" in notes:
        return "far pointer"
    if "string pointer" in notes:
        return "char *"
    if "float" in notes:
        return {4: "float", 8: "double", 10: "long double"}.get(size, "float")
    api = sorted(n for n in notes if n.startswith("result of "))
    if api:
        return api[0]
    if "array" in notes:
        return "%s[]" % _WORD.get(g["element"] or size, "unsigned char")
    if "pointer" in notes:
        return "pointer"
    if "counter" in notes:
        return "counter (%d bytes)" % size
    if "flags" in notes and len(g["values"]) >= 2:
        return "flags (%d bytes)" % size
    if g["taken"] and not g["readers"] and not g["writers"]:
        return "object passed by address (up to %d bytes)" % g["extent"]
    return _WORD.get(size, "%d bytes" % size)


def variables(img):
    return img.memo("globals", _variables)


def _slot_kind(img, text, target):
    if target in img.names:
        return "function"
    if target in img.imports:
        return "import"
    if target in text:
        return "string"
    if img.is_code(target):
        return "code"
    return "data"


def _pointer_tables(img):
    text = strings.table(img)
    word = 4
    if img.pointers is not None:
        sites = {}
        for site, (kind, target) in img.pointers.items():
            if target is None or kind not in ("abs32", "far") or (img.is_code(site) and (
                    img.kind == "ne" or img.covering(site) is not None)):
                continue
            sites[site] = target
    else:
        sites = {}
        for base, data, executable, _w, _n, held in img.spans:
            if executable:
                continue
            for off in range(0, held - 3, 4):
                v = int.from_bytes(data[off:off + 4], "little")
                if v in img.names or (v in text and text[v][2]):
                    sites[base + off] = v
    order = sorted(sites)
    tables, used = [], set()
    i = 0
    while i < len(order):
        j = i
        while j + 1 < len(order) and order[j + 1] == order[j] + word:
            j += 1
        if j > i:
            entries = [sites[a] for a in order[i:j + 1]]
            kinds = [_slot_kind(img, text, t) for t in entries]
            if not all(k == "code" for k in kinds):               # all-code runs are jump tables (see dispatch.py)
                tables.append({"at": order[i], "count": len(entries), "stride": word, "targets": entries,
                               "kinds": kinds, "shape": "run"})
            used.update(order[i:j + 1])
        i = j + 1
    singles = [a for a in order if a not in used]
    i = 0
    while i + 2 < len(singles):                                   # the same field of an array of structures
        stride = singles[i + 1] - singles[i]
        j = i + 1
        while 8 <= stride <= 128 and j + 1 < len(singles) and singles[j + 1] - singles[j] == stride:
            j += 1
        if j - i >= 2 and 8 <= stride <= 128:
            entries = [sites[a] for a in singles[i:j + 1]]
            tables.append({"at": singles[i], "count": len(entries), "stride": stride, "targets": entries,
                           "kinds": [_slot_kind(img, text, t) for t in entries], "shape": "records"})
            i = j + 1
        else:
            i += 1
    return sorted(tables, key=lambda t: t["at"])


def pointer_tables(img):
    return img.memo("globals.tables", _pointer_tables)


def analyse(img):
    var, tables = variables(img), pointer_tables(img)
    facts = []
    by_type = {}
    for va in sorted(var):
        g = dict(var[va])
        g["readers"], g["writers"] = len(g["readers"]), len(g["writers"])
        g["notes"] = sorted(g["notes"])
        g["values"] = sorted(g["values"])[:24]
        del g["element"]
        facts.append(g)
        key = g["type"].split("[")[0].split(" (")[0] if not g["type"].startswith("result of") else "result of a system call"
        key = "string" if g["type"].endswith("]") and g["type"][0] in "cw" else key
        by_type[key] = by_type.get(key, 0) + 1
    for t in tables:
        kinds = {}
        for k in t["kinds"]:
            kinds[k] = kinds.get(k, 0) + 1
        facts.append({"addr": None, "kind": "pointer_table", "at": t["at"], "count": t["count"], "stride": t["stride"],
                      "shape": t["shape"], "holds": kinds, "first": [img.label(x) for x in t["targets"][:8]]})
    plain = ("unsigned char", "unsigned short", "unsigned long")
    counts = {"globals": len(var),
              "typed_beyond_size": sum(n for k, n in by_type.items() if k not in plain and "bytes" not in k),
              "pointers": by_type.get("pointer", 0) + by_type.get("char *", 0) + by_type.get("far pointer", 0),
              "function_pointers": by_type.get("function pointer", 0),
              "strings": by_type.get("string", 0), "arrays": sum(n for k, n in by_type.items() if k.endswith("]") or "[]" in k),
              "counters": by_type.get("counter", 0), "flags": by_type.get("flags", 0),
              "results_of_system_calls": by_type.get("result of a system call", 0),
              "floats": by_type.get("float", 0) + by_type.get("double", 0),
              "uninitialised": sum(1 for g in var.values() if g["initial"] == "uninitialised"),
              "written_never_read": sum(1 for g in var.values() if g["writers"] and not g["readers"] and not g["taken"]),
              "read_never_written": sum(1 for g in var.values() if g["readers"] and not g["writers"] and not g["taken"]),
              "pointer_tables": sum(1 for t in tables if t["shape"] == "run"),
              "record_tables": sum(1 for t in tables if t["shape"] == "records"),
              "table_entries": sum(t["count"] for t in tables),
              "function_table_entries": sum(t["kinds"].count("function") for t in tables)}
    text = ["types: " + ", ".join("%s %d" % kv for kv in sorted(by_type.items(), key=lambda kv: -kv[1]))]
    for g in sorted(var.values(), key=lambda g: -(len(g["readers"]) + len(g["writers"])))[:12]:
        text.append("  %x  %-28s read by %d, written by %d%s" % (
            g["at"], g["type"], len(g["readers"]), len(g["writers"]),
            ("  values " + ",".join("%x" % v for v in sorted(g["values"])[:8])) if g["values"] else ""))
    for t in tables[:8]:
        text.append("  table %x: %d %s, e.g. %s" % (t["at"], t["count"], "/".join(sorted(set(t["kinds"]))),
                                                     ", ".join(img.label(x) for x in t["targets"][:4])))
    return {"counts": counts, "facts": facts, "text": text}
