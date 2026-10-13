"""structures.py -- structures recovered from how pointers are used.

    from codebreaker.analyze import image, structures
    found = structures.recover(image.load("comctl32.dll"))
    for s in found:
        s["name"], s["size"], s["exact"]      # exact: the size is an allocation's, not a lower bound
        s["fields"]                           # {offset: {size, reads, writes, notes, ...}}
        s["roots"]                            # the pointers that were unified into this structure

A pointer is followed from where it comes into view:

    an argument          `mov esi, [ebp+8]`             (16-bit: `mov bx, [bp+6]`, `les bx, [bp+6]`)
    `this`               ecx on entry, when the function reads it before writing it
    a global pointer     `mov eax, [g]`
    an allocation        the result of LocalAlloc / GlobalAlloc / HeapAlloc / malloc / operator new
                         with a constant size -- which gives the structure's exact size
    a pointer field      `mov eax, [esi+10h]` and then eax is used as an address

through register copies, `lea`, and the frame slots unoptimised code keeps it in. Every access
`[pointer + offset]` is a field: its offset, its width, whether it is read or written, whether
what it holds is itself used as an address (a pointer field), is indexed (an array), is loaded
into the FPU (a float), or is given the address of a table of functions (a virtual table --
classes.py turns those into classes).

Two pointers are the same structure when one is handed on as the other: a function passing its
argument 0 as argument 1 of another, or as `this`. All such pointers are unified, so a structure
gathers its fields from every function that touches it, not only the one a caller happens to look
at. A structure is reported when it has two fields or more, or one field and a known size.

Running the code (observe.py) then says which of these fields a real run touches.
"""
from codebreaker.analyze import call_graph, conventions, image
from codebreaker.analyze.image import family

_ALLOCATORS = {"LocalAlloc": 1, "GlobalAlloc": 1, "HeapAlloc": 2, "malloc": 0, "calloc": None, "??2@YAPAXI@Z": 0,
               "LOCALALLOC": 0, "GLOBALALLOC": None, "CoTaskMemAlloc": 0, "SHAlloc": 0, "VirtualAlloc": 1}
MAX_OFFSET = 0x4000


GENERIC = 8                             # a pointer handed over from more places than this is anybody's


def _field(fields, root, off, size):
    f = fields.setdefault(root, {}).get(off)
    if f is None:
        f = fields[root][off] = {"size": 0, "reads": 0, "writes": 0, "notes": set(), "stores": set(), "users": set()}
    f["size"] = max(f["size"], size)
    return f


def _scan(img, fn, fact, graph, fields, edges, sizes, passed):
    """Follow pointers through one function. Adds to `fields` {root: {offset: field}} and to
    `edges` [(pointer, the pointer it is handed on as)]."""
    word = 4 if img.bits == 32 else 2
    far = (fact.get("convention") or "").endswith("far")
    first = 2 * word if img.bits == 32 else (6 if far else 4)
    framed = fact.get("frame")
    labels = {i.target for i in fn.insns if i.flow in ("jcc", "jmp") and i.target is not None}
    holds, slots, pushed = {}, {}, []          # register -> (root, delta); frame slot -> the same
    if "ecx" in (fact.get("reg_args") or ()) and img.bits == 32:
        holds["ecx"] = (("this", fn.start), 0)
    calls = {site: (kind, dest) for site, kind, dest in graph.calls[fn.start]}
    depth = 0 if not framed else None          # bytes pushed so far, while it can be followed
    pending = {}                               # register -> (root, offset) of the field it was loaded from

    def bound(op):
        """The (root, delta) a memory operand's base register holds."""
        return holds.get(family(op[1])) if op[1] else None

    def argument(op):
        """Which argument slot a memory operand is, or None."""
        if op[1] is None or op[2] is not None:
            return None
        fam = family(op[1])
        if framed and fam == "ebp" and op[4] >= first:
            return (op[4] - first) // word
        if depth is not None and fam == "esp" and img.bits == 32 and op[4] >= depth + word:
            return (op[4] - depth - word) // word
        return None

    for i, ins in enumerate(fn.insns):
        if ins.addr in labels:
            for r in ("eax", "ecx", "edx"):
                if not (r == "ecx" and i == 0):
                    holds.pop(r, None)
            pending.clear()
            depth = None if depth else depth
        mn, ops = ins.mn, ins.ops
        # every access through a followed pointer is a field
        for k, op in enumerate(ops):
            if op[0] != "m":
                continue
            if op[1] and family(op[1]) in pending and mn != "lea":       # a loaded field used as an address
                root, off = pending[family(op[1])]
                _field(fields, root, off, word)["notes"].add("pointer")
                child = ("field", root, off)
                holds[family(op[1])] = (child, 0)
                del pending[family(op[1])]
            b = bound(op)
            if b is None:
                continue
            off = b[1] + op[4]
            if not 0 <= off < MAX_OFFSET:
                continue
            if mn == "lea":
                if off:
                    _field(fields, b[0], off, 0)["notes"].add("address taken")
                continue
            f = _field(fields, b[0], off, op[6])
            f["users"].add(fn.start)
            write = k == 0 and mn not in call_graph.READS
            f["writes" if write else "reads"] += 1
            if write and mn not in call_graph.PURE_WRITES:
                f["reads"] += 1
            if op[2] is not None:
                f["notes"].add("array")
                f["element"] = max(f.get("element", 0), op[3] if op[3] > 1 else op[6])
            if mn[0] == "f" and mn != "fwait":
                f["notes"].add("float")
            if ins.flow == "call":
                f["notes"].add("called")
            if write and mn == "mov" and len(ops) == 2 and ops[1][0] == "i" and ops[1][2] == word:
                f["stores"].add(ops[1][1])
            if write and mn == "mov" and len(ops) == 2 and ops[1][0] == "r" and op[6] in (word, 4):
                src = holds.get(family(ops[1][1]))               # a followed pointer stored in a field
                if src and src[1] == 0:
                    edges.append((src[0], ("field", b[0], off)))
        # calls: what is handed on, and what comes back
        if ins.flow == "call":
            kind, dest = calls.get(ins.addr, ("indirect", None))
            if kind == "call" and dest is not None:
                for j, b in enumerate(reversed(pushed[-8:])):
                    if b and b[1] == 0:
                        edges.append((b[0], ("arg", dest, j)))
                if "ecx" in holds and holds["ecx"][1] == 0 and img.bits == 32:
                    passed.append((holds["ecx"][0], dest))
            elif kind == "import":
                api = img.imports[dest].split("!")[-1]
                for j, b in enumerate(reversed(pushed[-8:])):
                    if b and b[1] == 0:
                        fields.setdefault(b[0], {})
                        sizes.setdefault(("uses", b[0]), set()).add("%s argument %d" % (api, j))
            for r in ("eax", "ecx", "edx"):
                holds.pop(r, None)
                pending.pop(r, None)
            if kind == "import":
                api = img.imports[dest].split("!")[-1]
                stem = api[:-1] if api[-1:] in "AW" and api[:-1] in _ALLOCATORS else api
                if stem in _ALLOCATORS and _ALLOCATORS[stem] is not None:
                    args = image.arguments(fn, i, _ALLOCATORS[stem] + 1)
                    k = _ALLOCATORS[stem]
                    if k < len(args) and args[k][0] == "i" and 0 < args[k][1] < MAX_OFFSET:
                        root = ("alloc", ins.addr)
                        holds["eax"] = (root, 0)
                        sizes[root] = args[k][1]
                        fields.setdefault(root, {})
            if kind == "call" and dest is not None:
                holds["eax"] = (("returns", dest), 0)
            elif kind == "import" and "eax" not in holds:
                holds["eax"] = (("result", ins.addr), 0)
            pushed = []
            continue
        if ins.flow == "ret":
            b = holds.get("eax")
            if b and b[1] == 0 and b[0] != ("returns", fn.start):
                edges.append((b[0], ("returns", fn.start)))
            continue
        if mn == "push":
            b = None
            if ops and ops[0][0] == "r":
                b = holds.get(family(ops[0][1]))
            elif ops and ops[0][0] == "m":
                a = argument(ops[0])
                b = (("arg", fn.start, a), 0) if a is not None else slots.get((family(ops[0][1] or ""), ops[0][4]))
            pushed.append(b)
            if depth is not None:
                depth += word
            continue
        if depth is not None:
            d = conventions.stack_delta(ins)
            depth = None if d is None else depth + d
        if not ops or ops[0][0] not in ("r", "m"):
            continue
        # where pointers go
        if ops[0][0] == "r":
            dst = family(ops[0][1])
            new = None
            if mn in ("mov", "les", "lds") and len(ops) == 2:
                src = ops[1]
                if src[0] == "r":
                    new = holds.get(family(src[1]))
                    if new is None and family(src[1]) in pending and dst != family(src[1]):
                        pending[dst] = pending[family(src[1])]
                        holds.pop(dst, None)
                        continue
                elif src[0] == "m":
                    a = argument(src)
                    if a is not None:
                        new = (("arg", fn.start, a), 0)
                    elif src[1] is None and src[2] is None:
                        va = src[4] if img.bits == 32 else img.data_segment + (src[4] & 0xFFFF)
                        if img.span(va) is not None and va not in img.imports and src[6] in (word, 4):
                            new = (("global", va), 0)
                    elif src[1] and family(src[1]) in ("ebp", "esp") and (family(src[1]), src[4]) in slots:
                        new = slots[(family(src[1]), src[4])]
                    elif bound(src) is not None and src[6] in (word, 4) and src[2] is None:
                        b = bound(src)
                        if 0 <= b[1] + src[4] < MAX_OFFSET:
                            pending[dst] = (b[0], b[1] + src[4])
                            holds.pop(dst, None)
                            continue
            elif mn == "lea" and len(ops) == 2 and ops[1][0] == "m" and ops[1][2] is None and bound(ops[1]) is not None:
                b = bound(ops[1])
                new = (b[0], b[1] + ops[1][4])
            if dst in ins.wr or new is not None:
                pending.pop(dst, None)
                if new is not None:
                    holds[dst] = new
                else:
                    holds.pop(dst, None)
            for r in ins.wr:
                if r != dst and r in holds and r not in ("esp", "ebp"):
                    holds.pop(r, None)
        elif mn == "mov" and ops[0][1] and family(ops[0][1]) in ("ebp", "esp") and ops[0][2] is None and len(ops) == 2:
            key = (family(ops[0][1]), ops[0][4])
            b = holds.get(family(ops[1][1])) if ops[1][0] == "r" else None
            if b is not None:
                slots[key] = b
            else:
                slots.pop(key, None)
        elif mn == "mov" and ops[0][1] is None and ops[0][2] is None and len(ops) == 2 and ops[1][0] == "r":
            b = holds.get(family(ops[1][1]))                     # a followed pointer stored in a global
            va = ops[0][4] if img.bits == 32 else img.data_segment + (ops[0][4] & 0xFFFF)
            if b and b[1] == 0 and img.span(va) is not None and ops[0][5] in (None, "ds"):
                edges.append((b[0], ("global", va)))
        else:
            for r in ins.wr:
                if r in holds and r not in ("esp", "ebp"):
                    holds.pop(r, None)
                pending.pop(r, None)


def _describe(img, root):
    if root[0] == "arg":
        return "argument %d of %s" % (root[2], img.label(root[1]))
    if root[0] == "this":
        return "this of %s" % img.label(root[1])
    if root[0] == "global":
        return "what global %x points at" % root[1]
    if root[0] == "alloc":
        return "allocated at %x" % root[1]
    if root[0] == "returns":
        return "what %s returns" % img.label(root[1])
    if root[0] == "result":
        return "result of the call at %x" % root[1]
    return "field +%x of (%s)" % (root[2], _describe(img, root[1]))


def _recover(img):
    graph = call_graph.build(img)
    facts = conventions.functions(img)
    fields, edges, sizes, passed = {}, [], {}, []
    for fn in img.each():
        if fn.insns:
            _scan(img, fn, facts[fn.start], graph, fields, edges, sizes, passed)
    for root, dest in passed:                           # `this` handed on in ecx
        if "ecx" in (facts[dest].get("reg_args") or ()):
            edges.append((root, ("this", dest)))
    # Which pointers are one structure. A pointer handed on is the pointer received -- unless
    # the receiver takes pointers from many places (a helper for anything: free, a list
    # insert), or leads to no field at all, or the two are known to differ in size.
    edges = sorted(set(e for e in edges if e[0] != e[1]), key=repr)
    sources, onward = {}, {}
    for a, b in edges:
        sources.setdefault(b, set()).add(a)
        onward.setdefault(a, set()).add(b)
    useful = {r for r, f in fields.items() if f}
    grew = True
    while grew:
        grew = False
        for a, b in edges:
            if b in useful and a not in useful:
                useful.add(a)
                grew = True
    parent = {}

    def find(x):
        p = parent.setdefault(x, x)
        while p != parent[p]:
            parent[p] = parent[parent[p]]
            p = parent[p]
        parent[x] = p
        return p
    extent = {}                                         # set -> [exact size or None, end of its last field]
    for r, fs in fields.items():
        extent[r] = [sizes.get(r), max((o + f["size"] for o, f in fs.items()), default=0)]
    for a, b in edges:
        if len(sources[b]) > GENERIC or b not in useful or a not in useful:
            continue
        ra, rb = find(a), find(b)
        if ra == rb:
            continue
        ea, eb = extent.get(ra, [None, 0]), extent.get(rb, [None, 0])
        if ea[0] and eb[0] and ea[0] != eb[0]:
            continue
        exact = ea[0] or eb[0]
        if exact and max(ea[1], eb[1]) > exact:
            continue
        keep, gone = (ra, rb) if repr(ra) < repr(rb) else (rb, ra)
        parent[gone] = keep
        extent[keep] = [exact, max(ea[1], eb[1])]
    groups = {}
    for root in list(fields):
        groups.setdefault(find(root), []).append(root)
    out = []
    for rep, roots in groups.items():
        merged = {}
        for r in roots:
            for off, f in fields.get(r, {}).items():
                m = merged.setdefault(off, {"size": 0, "reads": 0, "writes": 0, "notes": set(), "stores": set(),
                                            "users": set(), "element": 0})
                m["size"] = max(m["size"], f["size"])
                m["reads"] += f["reads"]
                m["writes"] += f["writes"]
                m["notes"] |= f["notes"]
                m["stores"] |= f["stores"]
                m["users"] |= f["users"]
                m["element"] = max(m["element"], f.get("element", 0))
        merged = {o: f for o, f in merged.items() if f["size"] or f["notes"]}
        exact = [sizes[r] for r in roots if r in sizes]
        if len(merged) < 2 and not (merged and exact):
            continue
        low = max((o + max(f["size"], 1) for o, f in merged.items()), default=0)
        uses = sorted(set().union(*[sizes.get(("uses", r), set()) for r in roots]))
        out.append({"roots": sorted(roots, key=repr), "fields": merged, "exact": bool(exact),
                    "size": max(exact) if exact else low, "passed_to": uses,
                    "functions": sorted(set().union(*[f["users"] for f in merged.values()]))})
    out.sort(key=lambda s: (-len(s["fields"]), repr(s["roots"][0])))
    for n, s in enumerate(out):
        s["name"] = "struct_%d" % (n + 1)
    return out


def recover(img):
    """Every structure the image's code shows, largest first."""
    return img.memo("structures", _recover)


def field_type(f, word=4):
    """A C type for one recovered field."""
    notes = f["notes"]
    base = {1: "unsigned char", 2: "unsigned short", 4: "unsigned long", 8: "double", 10: "long double"}.get(f["size"], "unsigned char")
    if "called" in notes:
        return "function pointer"
    if "float" in notes:
        return "float" if f["size"] == 4 else "double"
    if "pointer" in notes:
        return "pointer"
    if "array" in notes:
        return "%s[]" % {1: "unsigned char", 2: "unsigned short", 4: "unsigned long"}.get(f.get("element") or f["size"], "unsigned char")
    if "address taken" in notes and not f["size"]:
        return "embedded object"
    return base


def layout(img, s):
    """The structure as C text, gaps as padding."""
    word = 4 if img.bits == 32 else 2
    lines, at = ["struct %s {%s" % (s["name"], "    /* %d bytes */" % s["size"] if s["exact"] else "")], 0
    for off in sorted(s["fields"]):
        f = s["fields"][off]
        if off > at:
            lines.append("    unsigned char gap_%x[%d];" % (at, off - at))
        if off < at:
            continue                                    # bytes of the field before it, accessed singly
        if not f["size"] and "address taken" in f["notes"] and any(off < o < off + 4 for o in s["fields"]):
            continue
        t = field_type(f, word)
        size = f["size"] or 1
        name = "f_%x" % off
        decl = {"function pointer": "void (*%s)()" % name, "pointer": "void *%s" % name,
                "embedded object": "unsigned char %s[1]" % name}.get(t)
        if decl is None:
            decl = ("%s %s[1]" % (t[:-2], name)) if t.endswith("[]") else "%s %s" % (t, name)
        lines.append("    %-34s /* +%x  read %d, written %d%s */" % (
            decl + ";", off, f["reads"], f["writes"],
            (", " + ", ".join(sorted(n for n in f["notes"] if n not in ("pointer", "called", "float")))) if f["notes"] - {"pointer", "called", "float"} else ""))
        at = off + size
    lines.append("};")
    return "\n".join(lines)


def analyse(img):
    found = recover(img)
    facts = []
    for s in found:
        facts.append({"addr": None, "kind": "structure", "name": s["name"], "size": s["size"], "exact_size": s["exact"],
                      "fields": {("+%x" % o): {"size": f["size"], "type": field_type(f), "reads": f["reads"],
                                               "writes": f["writes"], "notes": sorted(f["notes"])}
                                 for o, f in sorted(s["fields"].items())},
                      "seen_as": [_describe(img, r) for r in s["roots"][:12]], "pointers_unified": len(s["roots"]),
                      "functions": [img.label(f) for f in s["functions"][:40]], "passed_to": s["passed_to"][:12]})
    nf = sum(len(s["fields"]) for s in found)
    counts = {"structures": len(found), "fields": nf,
              "with_exact_size": sum(1 for s in found if s["exact"]),
              "pointer_fields": sum(1 for s in found for f in s["fields"].values() if "pointer" in f["notes"]),
              "function_pointer_fields": sum(1 for s in found for f in s["fields"].values() if "called" in f["notes"]),
              "array_fields": sum(1 for s in found for f in s["fields"].values() if "array" in f["notes"]),
              "seen_through_this": sum(1 for s in found if any(r[0] == "this" for r in s["roots"])),
              "seen_through_global_pointer": sum(1 for s in found if any(r[0] == "global" for r in s["roots"])),
              "unified_across_functions": sum(1 for s in found if len(s["functions"]) > 1),
              "largest_fields": max((len(s["fields"]) for s in found), default=0),
              "functions_touching_a_structure": len(set().union(*[set(s["functions"]) for s in found])) if found else 0}
    text = []
    for s in found[:3]:
        text.append("%s: %d fields, %d bytes%s, used by %d functions, seen as %s" % (
            s["name"], len(s["fields"]), s["size"], "" if s["exact"] else " at least", len(s["functions"]),
            "; ".join(_describe(img, r) for r in s["roots"][:3])))
    if found:
        text += layout(img, found[0]).split("\n")[:24]
    return {"counts": counts, "facts": facts, "text": text}
