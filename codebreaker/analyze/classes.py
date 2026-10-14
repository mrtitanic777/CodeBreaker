"""classes.py -- C++ in a module: virtual tables, classes and their hierarchy, MFC's class
records and message maps, and exception-handling tables.

    from codebreaker.analyze import image, classes
    img = image.load("mfc40.dll")
    classes.vtables(img)          [{at, slots, installed_by, com}]
    classes.runtime_classes(img)  MFC's CRuntimeClass records: name, object size, base
    classes.message_maps(img)     MFC message maps: the class, and every entry as its macro
    classes.hierarchy(img)        [{name, vtable, bases, size, methods, evidence}]
    classes.exception_tables(img) C++ try/catch tables and __try/__except scope tables

Where each fact comes from:

  * A virtual table is a table of function addresses whose own address the code stores into an
    object (`mov dword ptr [esi], offset table`); the functions that do so are the class's
    constructors and destructors. It ends where the next table the code names begins. An object
    kept in the data whose first word points at such a table counts too. A table whose first
    three functions remove 12, 4 and 4 bytes of arguments has the shape of a COM interface
    (QueryInterface, AddRef, Release); four more removing 8, 16, 24 and 36 make it IDispatch.
  * A class's NAME is in the binary in two cases: run-time type information (the word before
    the table leads to the type's decorated name and its bases -- exact), and MFC (the first
    virtual function returns the class's CRuntimeClass record, which names it, gives its object
    size and links to its base).
  * Without either, inheritance is inferred and said to be: a table whose leading slots repeat
    a shorter table's is probably derived from it; a function that installs two tables in one
    object installs the base's first (a constructor) or last (a destructor).
  * A message map is found as MFC lays it out (24-byte entries ending in a zero entry) and tied
    to its class through GetMessageMap, the virtual function that returns it.
  * The instance layout of a class is the structure (structures.py) whose first field is given
    the table's address.

32-bit modules. A 16-bit module's virtual tables appear in globals.py as tables of far pointers;
nothing here names their classes.
"""
import re

from codebreaker.analyze import call_graph, conventions, dispatch, image, strings, structures
from codebreaker.analyze.image import family

_CXX_MAGIC = (0x19930520, 0x19930521, 0x19930522)
_IDENT = re.compile(r"[A-Za-z_][A-Za-z0-9_]{1,63}$")


# ---- virtual tables ---------------------------------------------------------------------------
def _vtables(img):
    if img.bits != 32:
        return []
    graph = call_graph.build(img)
    facts = conventions.functions(img)
    funcs = set(img.functions)
    named = {va for _f, _i, va, how in graph.refs if how == "a"}        # addresses the code names
    stores, static = {}, {}                                              # table -> who installs it
    for f, ins, va, how in graph.refs:
        if how != "a" or ins.mn != "mov" or len(ins.ops) != 2 or ins.ops[1][0] != "i" or ins.ops[1][1] != va \
                or img.u32(va) not in funcs:
            continue
        if ins.ops[0][0] == "m" and ins.ops[0][1] is not None:           # mov [reg+N], offset table
            stores.setdefault(va, []).append((f, ins.addr, ins.ops[0][4]))
        elif ins.ops[0][0] == "r":                                       # mov reg, offset table / mov [reg2+N], reg
            fn, reg = img.function(f), ins.ops[0][1]
            for b in fn.insns[fn.at(ins.addr) + 1:fn.at(ins.addr) + 4]:
                if b.mn == "mov" and len(b.ops) == 2 and b.ops[0][0] == "m" and b.ops[0][1] is not None \
                        and b.ops[1][:2] == ("r", reg):
                    stores.setdefault(va, []).append((f, b.addr, b.ops[0][4]))
                    break
                if family(reg) in b.wr or b.flow:
                    break
    if img.pointers:                                                     # an object in the data that starts with one
        for site, (_k, t) in img.pointers.items():
            if t is not None and not img.is_code(site) and img.u32(t) in funcs and t in img.pointers \
                    and img.u32(t + 4) in funcs and img.u32(t + 8) in funcs and site != t - 4 and site != t + 4:
                static.setdefault(t, []).append(site)
    out = []
    for va in sorted(set(stores) | set(static)):
        slots, at = [], va
        while True:
            t = img.u32(at)
            if t not in funcs or (slots and (at in named or at in static)) \
                    or (img.pointers is not None and at not in img.pointers):
                break
            slots.append(t)
            at += 4
        if not slots:
            continue
        if va not in stores and (len(slots) < 3 or any(a in static or a in stores for a in range(va + 4, at, 4))):
            continue
        removes = [facts[s].get("removes") for s in slots[:7]]
        com = removes[:3] == [12, 4, 4]                  # QueryInterface, AddRef, Release
        dual = com and removes[3:7] == [8, 16, 24, 36]   # ... and IDispatch
        out.append({"at": va, "slots": slots, "installed_by": sorted({f for f, _s, _o in stores.get(va, ())}),
                    "at_offsets": sorted({o for _f, _s, o in stores.get(va, ())}),
                    "static_objects": sorted(static.get(va, ())), "com": bool(com), "dispatch": bool(dual)})
    return out


def vtables(img):
    return img.memo("classes.vtables", _vtables)


# ---- run-time type information ----------------------------------------------------------------
def _decorated(name):
    """`.?AVCFoo@@` -> "CFoo";  `.?AVInner@Outer@@` -> "Outer::Inner"."""
    if not name.startswith(".?A") or len(name) < 6:
        return None
    parts = [p for p in name[4:].split("@") if p]
    return "::".join(reversed(parts)) if parts and all(re.match(r"[\w?$<>,]+$", p) for p in parts) else name[4:]


def _type_name(img, descriptor):
    s = strings.at(img, descriptor + 8, least=4) if descriptor and img.span(descriptor) else None
    return _decorated(s[0]) if s and s[0].startswith(".?A") else None


def _rtti(img, table):
    """(class name, [base names]) from the type information before a virtual table, or None."""
    locator = img.u32(table - 4)
    if not locator or not img.span(locator) or img.is_code(locator) and img.owner(locator) is not None:
        return None
    signature, descriptor, hierarchy = img.u32(locator), img.u32(locator + 12), img.u32(locator + 16)
    if signature != 0 or descriptor is None:
        return None
    name = _type_name(img, descriptor)
    if not name:
        return None
    bases = []
    count, array = (img.u32(hierarchy + 8), img.u32(hierarchy + 12)) if hierarchy and img.span(hierarchy) else (0, 0)
    for k in range(1, min(count or 0, 64)):                            # entry 0 is the class itself
        base = _type_name(img, img.u32(img.u32(array + 4 * k) or 0) or 0) if array else None
        if base:
            bases.append(base)
    return name, bases


# ---- MFC ---------------------------------------------------------------------------------------
def _returned(img, va):
    """The address the code at `va` returns, when it is just `mov eax, offset X / ret` (or
    `mov eax, [import] / ret`); else None."""
    b = img.read(va, 6)
    if len(b) == 6 and b[0] in (0xB8, 0xA1) and b[5] == 0xC3:
        x = int.from_bytes(b[1:5], "little")
        if (b[0] == 0xB8 and img.span(x)) or (b[0] == 0xA1 and x in img.imports):
            return x
    return None


def _returns_address(img):
    """{function: address} for the functions that just return an address."""
    return img.memo("classes.getters", lambda i: {f: v for f in i.functions for v in [_returned(i, f)] if v is not None})


def _class_of_import(name):
    """The class an imported CRuntimeClass record belongs to: ?classCWnd@CWnd@@2UCRuntimeClass@@B."""
    m = re.search(r"\?class(\w+?)@", name)
    return m.group(1) if m else name.split("!")[-1]


def _aligned_pointers(img):
    """(address, value) for every aligned word of the module's data that holds an address."""
    lo, hi = img.spans[0][0], img.spans[-1][0] + len(img.spans[-1][1])
    for base, data, executable, _w, name, held in img.spans:
        if executable or name in (".rsrc", ".reloc", ".edata", ".idata"):
            continue
        for off in range(0, held - 3, 4):
            v = int.from_bytes(data[off:off + 4], "little")
            if lo <= v < hi:
                yield base + off, v


def _runtime_classes(img):
    if img.bits != 32:
        return {}
    found = {}
    for at, v in _aligned_pointers(img):
        s = strings.at(img, v, least=2)
        if not s or s[1] != "A" or not _IDENT.match(s[0]):
            continue
        size, schema, create, base = (img.u32(at + o) for o in (4, 8, 12, 16))
        if None in (size, schema, create, base) or not 4 <= size <= 0x10000 or not (schema <= 0x1000 or schema == 0xFFFF):
            continue
        if create and not img.is_code(create):
            continue
        found[at] = {"at": at, "name": s[0], "size": size, "schema": None if schema == 0xFFFF else schema,
                     "creatable": bool(create), "_base": base}
    for c in found.values():
        b = c.pop("_base")
        if b and img.is_code(b):                        # the DLL build: a function returns the base's record
            b = _returned(img, b) or b
        c["base"] = found[b]["name"] if b in found else _class_of_import(img.imports[b]) if b in img.imports else None
        c["_linked"] = b == 0 or c["base"] is not None
    bases = {c["base"] for c in found.values() if c["base"]}
    return {at: {k: v for k, v in c.items() if k != "_linked"} for at, c in found.items()
            if c["_linked"] and (c["base"] or c["name"] in bases)}


def runtime_classes(img):
    return img.memo("classes.runtime", _runtime_classes)


def _macro(img, e):
    msg, code, ident, last, sig, fn = e
    handler = img.label(fn)
    if msg == 0x111:
        if code == 0xFFFFFFFF or code == 0xFFFF:
            return "ON_UPDATE_COMMAND_UI%s(%s, %s)" % ("_RANGE" if last != ident else "", _ids(ident, last), handler)
        if code == 0:
            return "ON_COMMAND%s(%s, %s)" % ("_RANGE" if last != ident else "", _ids(ident, last), handler)
        return "ON_CONTROL%s(%#x, %s, %s)" % ("_RANGE" if last != ident else "", code, _ids(ident, last), handler)
    if msg == 0x4E:
        return "ON_NOTIFY%s(%d, %s, %s)" % ("_RANGE" if last != ident else "", code - (1 << 32) if code >> 31 else code,
                                            _ids(ident, last), handler)
    if msg == 0xC000:
        return "ON_REGISTERED_MESSAGE(*(UINT *)%#x, %s)" % (sig if sig > 0x10000 else ident, handler)
    name = dispatch.MESSAGES.get(msg)
    if name:
        return "ON_%s() -> %s" % (name, handler) if sig not in (10, 11, 12) else "ON_MESSAGE(%s, %s)" % (name, handler)
    return "ON_MESSAGE(%#06x, %s)" % (msg, handler)


def _ids(first, last):
    return "%d" % first if last == first else "%d, %d" % (first, last)


def _message_maps(img):
    if img.bits != 32:
        return []
    funcs = set(img.functions)
    getters = _returns_address(img)
    holders = {}                                         # address a word of data holds -> where
    for at, v in _aligned_pointers(img):
        holders.setdefault(v, []).append(at)
    by_table = {}
    for vt in vtables(img):
        for f in vt["slots"]:
            if f in getters:
                by_table.setdefault(getters[f], []).append(vt["at"])
    out, skip = [], -1
    for base, data, executable, _w, name, held in img.spans:
        if executable or name in (".rsrc", ".reloc", ".edata", ".idata"):
            continue
        off = 0
        while off + 48 <= held:
            if base + off < skip or int.from_bytes(data[off + 20:off + 24], "little") not in funcs:
                off += 4
                continue
            entries, p = [], off
            while p + 24 <= held:
                e = [int.from_bytes(data[p + 4 * k:p + 4 * k + 4], "little") for k in range(6)]
                if e[0] == 0 and e[5] == 0 and e[4] == 0:
                    break
                if e[5] not in funcs or e[0] > 0xFFFF or not (e[4] <= 0x80 or img.span(e[4])) or e[2] > 0xFFFF \
                        or e[3] > 0xFFFF or not (e[1] <= 0xFFFF or e[1] >= 0xFFFFF000):
                    entries = []
                    break
                entries.append(tuple(e))
                p += 24
            else:
                entries = []
            at = base + off
            owners = [h - 4 for h in holders.get(at, ()) if h - 4 >= base]       # AFX_MSGMAP {base, entries}
            if entries and (owners or len(entries) >= 3):
                rec = {"at": at, "entries": entries, "map": owners[0] if owners else None, "class_tables": [], "base_map": None}
                if owners:
                    rec["class_tables"] = sorted(by_table.get(owners[0], ()))
                    b = img.u32(owners[0])
                    rec["base_map"] = getters.get(b, b) or None
                out.append(rec)
                skip = base + (p - off) + off + 24
                off = p + 24 - ((p + 24) % 4)
                continue
            off += 4
    return out


def message_maps(img):
    return img.memo("classes.message_maps", _message_maps)


# ---- the hierarchy ----------------------------------------------------------------------------
def _hierarchy(img):
    tables = vtables(img)
    if not tables:
        return []
    runtime, getters = runtime_classes(img), _returns_address(img)
    by_at = {t["at"]: t for t in tables}
    layouts = {}
    for s in structures.recover(img):
        for v in s["fields"].get(0, {}).get("stores", ()):
            if v in by_at:
                layouts.setdefault(v, s)
    maps = {}
    for m in message_maps(img):
        for t in m["class_tables"]:
            maps[t] = m
    out = {}
    for t in tables:
        c = {"vtable": t["at"], "slots": len(t["slots"]), "methods": t["slots"], "name": None, "bases": [],
             "evidence": [], "size": None, "installed_by": t["installed_by"], "com": t["com"]}
        info = _rtti(img, t["at"])
        if info:
            c["name"], c["bases"] = info
            c["evidence"].append("run-time type information")
        elif t["slots"][0] in getters and getters[t["slots"][0]] in runtime:
            rc = runtime[getters[t["slots"][0]]]
            c["name"], c["size"] = rc["name"], rc["size"]
            chain, seen = [], {rc["name"]}
            by_name = {r["name"]: r for r in runtime.values()}
            b = rc["base"]
            while b and b not in seen:
                chain.append(b)
                seen.add(b)
                b = by_name[b]["base"] if b in by_name else None
            c["bases"] = chain
            c["evidence"].append("MFC class record at %x" % rc["at"])
        s = layouts.get(t["at"])
        if s:
            c["layout"] = s["name"]
            c["fields"] = len(s["fields"])
            if c["size"] is None:
                c["size"], c["size_exact"] = s["size"], s["exact"]
        if t["at"] in maps:
            c["message_map"] = maps[t["at"]]["at"]
        out[t["at"]] = c
    # inheritance the binary does not state: shared leading slots, and the order tables are installed
    installs = {}
    for t in tables:
        for f in t["installed_by"]:
            installs.setdefault(f, []).append(t["at"])
    for t in tables:
        c = out[t["at"]]
        if c["bases"]:
            continue
        best = None
        for u in tables:
            if u is t or len(u["slots"]) >= len(t["slots"]) or len(u["slots"]) < 2:
                continue
            same = sum(1 for a, b in zip(u["slots"], t["slots"]) if a == b)
            if same >= max(2, len(u["slots"]) // 2) and (best is None or (same, len(u["slots"])) > best[0]):
                best = ((same, len(u["slots"])), u["at"])
        together = {o for f in t["installed_by"] for o in installs[f] if o != t["at"] and len(by_at[o]["slots"]) <= len(t["slots"])}
        if best:
            c["bases"] = [out[best[1]]["name"] or "class_%x" % best[1]]
            c["evidence"].append("probably: %d of its first %d virtual functions are those of the table at %x"
                                 % (best[0][0], best[0][1], best[1]))
        elif len(together) == 1:
            o = next(iter(together))
            c["bases"] = [out[o]["name"] or "class_%x" % o]
            c["evidence"].append("probably: installed in the same object, by the same function, as the table at %x" % o)
    for c in out.values():
        if c["name"] is None:
            c["name"] = "class_%x" % c["vtable"]
    return sorted(out.values(), key=lambda c: c["vtable"])


def hierarchy(img):
    return img.memo("classes.hierarchy", _hierarchy)


# ---- exception handling -----------------------------------------------------------------------
def _cxx_tables(img):
    """C++ exception tables: [{at, states, cleanups, tries: [{low, high, catches: [(type, handler)]}]}]"""
    out = []
    lo, hi = img.spans[0][0], img.spans[-1][0] + len(img.spans[-1][1])
    inside = lambda v: lo <= v < hi                                      # noqa: E731
    for base, data, _x, _w, name, held in img.spans:
        if name in (".rsrc", ".reloc"):
            continue
        for magic in _CXX_MAGIC:
            pat, pos = magic.to_bytes(4, "little"), 0
            while True:
                pos = data.find(pat, pos, held)
                if pos < 0:
                    break
                at = base + pos
                pos += 4
                states, unwind, ntry, trymap = (img.u32(at + o) for o in (4, 8, 12, 16))
                if None in (states, unwind, ntry, trymap) or states > 4096 or ntry > 256 \
                        or (states and not inside(unwind)) or (ntry and not inside(trymap)) or not (states or ntry):
                    continue
                cleanups = sum(1 for k in range(states) if img.u32(unwind + 8 * k + 4))
                tries = []
                for k in range(ntry):
                    low, high, _ch, ncatch, handlers = (img.u32(trymap + 20 * k + o) or 0 for o in (0, 4, 8, 12, 16))
                    catches = []
                    for c in range(min(ncatch, 64)):
                        ptype, handler = img.u32(handlers + 16 * c + 4), img.u32(handlers + 16 * c + 12)
                        if handler is None or not img.is_code(handler):
                            catches = None
                            break
                        catches.append((_type_name(img, ptype) or ("..." if not ptype else "?"), handler))
                    if catches is None:
                        tries = None
                        break
                    tries.append({"low": low, "high": high, "catches": catches})
                if tries is not None:
                    out.append({"at": at, "states": states, "cleanups": cleanups, "tries": tries})
    return out


def _scope_table(img, at):
    """The entries of a __try scope table: [(enclosing level, filter, handler)]."""
    rows = []
    for k in range(64):
        level, flt, handler = (img.u32(at + 12 * k + o) for o in (0, 4, 8))
        if handler is None or not img.is_code(handler) or not (level == 0xFFFFFFFF or level < k) \
                or not (flt == 0 or img.is_code(flt)):
            break
        rows.append((None if level == 0xFFFFFFFF else level, flt or None, handler))
    return rows


def _exception_tables(img):
    if img.bits != 32:
        return {"cxx": [], "frames": []}
    graph = call_graph.build(img)
    cxx = _cxx_tables(img)
    by_table = {t["at"]: t for t in cxx}
    thunks = {}                                          # the stub that hands the table to the run-time
    for f, ins, va, how in graph.refs:
        if how == "a" and va in by_table and ins.mn == "mov":
            thunks[f] = va
            thunks[ins.addr] = va                        # the stub may sit inside its function's bytes
    helpers = set()                                      # functions that set a frame up for their caller
    for fn in img.each():
        if len(fn.insns) <= 14 and any(i.mn == "mov" and i.ops[0][0] == "m" and i.ops[0][5] == "fs" and i.ops[0][4] == 0
                                       for i in fn.insns if len(i.ops) == 2) \
                and any(i.mn == "push" and i.ops and i.ops[0][:2] == ("r", "eax") for i in fn.insns):
            helpers.add(fn.start)
        elif len(fn.insns) == 1 and fn.insns[0].flow == "jmp" and "EH_prolog" in img.imports.get(
                image.destination(img, fn.insns[0]), ""):
            helpers.add(fn.start)                        # the compiler's own, imported from its run-time
    helpers |= {slot for slot, name in img.imports.items() if "EH_prolog" in name}
    frames = []
    for fn in img.each():
        if fn.start in helpers:
            continue
        head = fn.insns[:3]
        if len(head) >= 2 and head[0].mn == "mov" and head[0].ops[0][:2] == ("r", "eax") and head[0].ops[1][0] == "i" \
                and head[1].flow == "call" and image.destination(img, head[1]) in helpers:
            frame = {"function": fn.start, "handler": head[0].ops[1][1]}
            if frame["handler"] in thunks:
                frame["cxx"] = thunks[frame["handler"]]
            frames.append(frame)
            continue
        pushes = []
        for i, ins in enumerate(fn.insns[:24]):
            if ins.mn == "push" and ins.ops and ins.ops[0][0] == "i":
                pushes.append(ins.ops[0][1])
            if any(op[0] == "m" and op[5] == "fs" and op[4] == 0 and op[1] is None for op in ins.ops) \
                    and ins.ops[0][0] == "m" and ins.mn == "mov":
                handler = next((p for p in reversed(pushes) if img.is_code(p) or p in img.imports), None)
                if handler is None:
                    break
                frame = {"function": fn.start, "handler": handler}
                if handler in thunks:
                    frame["cxx"] = thunks[handler]
                else:
                    data = [p for p in pushes if p != handler and img.span(p) and not (img.is_code(p) and p in img.names)]
                    rows = _scope_table(img, data[-1]) if data else []
                    if rows:
                        frame["scope_table"] = data[-1]
                        frame["scopes"] = rows
                frames.append(frame)
                break
            if ins.flow in ("jmp", "ret"):
                break
    return {"cxx": cxx, "frames": frames}


def exception_tables(img):
    return img.memo("classes.exceptions", _exception_tables)


def analyse(img):
    tables, tree = vtables(img), hierarchy(img)
    runtime, maps, eh = runtime_classes(img), message_maps(img), exception_tables(img)
    thiscall = sum(1 for f in conventions.functions(img).values() if f.get("convention") == "thiscall")
    facts = []
    for c in tree:
        row = {"addr": None, "kind": "class", "name": c["name"], "vtable": c["vtable"], "virtual_functions": c["slots"],
               "methods": [img.label(m) for m in c["methods"][:64]], "bases": c["bases"], "evidence": c["evidence"],
               "size": c["size"], "constructors_or_destructors": [img.label(f) for f in c["installed_by"]],
               "com_interface": c["com"]}
        for k in ("layout", "fields", "message_map", "size_exact"):
            if k in c:
                row[k] = c[k]
        facts.append(row)
    for rc in runtime.values():
        facts.append(dict(rc, addr=None, kind="mfc_class"))
    for m in maps:
        names = [c["name"] for c in tree if c["vtable"] in m["class_tables"]]
        facts.append({"addr": None, "kind": "message_map", "at": m["at"], "class": names[0] if names else None,
                      "entries": [_macro(img, e) for e in m["entries"]], "base_map": m["base_map"]})
    for t in eh["cxx"]:
        owners = [fr["function"] for fr in eh["frames"] if fr.get("cxx") == t["at"]]
        facts.append({"addr": owners[0] if owners else None, "kind": "cxx_exception_table", "at": t["at"],
                      "states": t["states"], "cleanups": t["cleanups"],
                      "try_blocks": [{"states": "%d..%d" % (b["low"], b["high"]),
                                      "catches": ["catch (%s) at %x" % c for c in b["catches"]]} for b in t["tries"]]})
    for fr in eh["frames"]:
        if "cxx" in fr:
            continue
        row = {"addr": fr["function"], "kind": "exception_frame", "handler": img.label(fr["handler"])}
        if fr.get("scopes"):
            row["scopes"] = [{"enclosing": lvl, "kind": "__except" if flt else "__finally",
                              "filter": flt, "handler": h} for lvl, flt, h in fr["scopes"]]
        facts.append(row)
    named = [c for c in tree if c["evidence"] and not c["evidence"][0].startswith("probably")]
    counts = {"virtual_tables": len(tables), "virtual_functions": sum(len(t["slots"]) for t in tables),
              "com_shaped_tables": sum(1 for t in tables if t["com"]),
              "constructors_or_destructors": len({f for t in tables for f in t["installed_by"]}),
              "classes_named_by_the_binary": len(named),
              "classes_with_stated_bases": sum(1 for c in named if c["bases"]),
              "classes_with_inferred_base": sum(1 for c in tree if c["evidence"] and c["evidence"][0].startswith("probably")),
              "classes_with_instance_layout": sum(1 for c in tree if "layout" in c),
              "mfc_class_records": len(runtime), "message_maps": len(maps),
              "message_map_entries": sum(len(m["entries"]) for m in maps),
              "message_maps_tied_to_a_class": sum(1 for m in maps if m["class_tables"]),
              "this_in_ecx_functions": thiscall,
              "cxx_exception_tables": len(eh["cxx"]), "try_blocks": sum(len(t["tries"]) for t in eh["cxx"]),
              "catch_handlers": sum(len(b["catches"]) for t in eh["cxx"] for b in t["tries"]),
              "exception_frames": len(eh["frames"]),
              "frames_with_scope_table": sum(1 for fr in eh["frames"] if fr.get("scopes")),
              "except_or_finally_scopes": sum(len(fr.get("scopes", ())) for fr in eh["frames"])}
    text = []
    for c in tree[:10]:
        text.append("class %s%s  table %x, %d virtual, %s%s" % (
            c["name"], (" : " + ", ".join(c["bases"][:3])) if c["bases"] else "", c["vtable"], c["slots"],
            ("%d bytes" % c["size"]) if c["size"] else "size unknown", ("  [%s]" % c["evidence"][0]) if c["evidence"] else ""))
    for m in maps[:3]:
        names = [c["name"] for c in tree if c["vtable"] in m["class_tables"]]
        text.append("message map %x%s:" % (m["at"], " of " + names[0] if names else ""))
        text += ["    " + _macro(img, e) for e in m["entries"][:8]]
    return {"counts": counts, "facts": facts, "text": text}
