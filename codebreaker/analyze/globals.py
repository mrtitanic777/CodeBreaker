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

Both are worked out in C++ (native/analyze, globals.cc) over the image cbanalyze.dll holds.
"""
from codebreaker.analyze import strings


def _ask(img, what):
    """A globals question to the DLL (over the strings table), with the image's names as they are now."""
    strings.table(img)
    h = img._handle()
    h.names(img)
    return h.ask("globals." + what)


def _variables(img):
    out = {}
    for g in _ask(img, "variables"):
        out[g["at"]] = {"addr": None, "kind": "global", "at": g["at"], "size": g["size"], "readers": set(g["readers"]),
                        "writers": set(g["writers"]), "taken": g["taken"], "notes": set(g["notes"]),
                        "values": set(g["values"]), "writable": g["writable"], "element": g["element"],
                        "extent": g["extent"], "initial": g["initial"], "type": g["type"]}
    return out


def variables(img):
    return img.memo("globals", _variables)


def pointer_tables(img):
    return img.memo("globals.tables", lambda im: _ask(im, "tables"))


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
