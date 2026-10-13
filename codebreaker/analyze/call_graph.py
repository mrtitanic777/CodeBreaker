"""call_graph.py -- who calls whom, what refers to what, and what the exports reach.

    from codebreaker.analyze import image, call_graph
    g = call_graph.build(image.load("comctl32.dll"))
    g.calls[f]          [(site, kind, destination)]  kind: "call" | "import" | "tail" | "indirect"
    g.callers[f]        the functions that call f
    g.refs              [(function, instruction, address, how)]   how: "r" | "w" | "rw" | "a"
                        every reference from code to an address in the module ("a": its address
                        is taken -- pushed, loaded or stored as a value)
    g.taken[f]          where function f's address is taken (code sites and data addresses)
    g.users[name]       the functions that call import `name`
    g.order             every function, callees before callers (cycles in one piece)
    g.reachable         the functions execution can reach from the exports and the entry point
    g.reachable_with_tables   the same, also entering at every function whose address the
                        module's data holds (tables of handlers, virtual tables)

An operand counts as an address only when the file says so, where it does: a 32-bit module with
relocations marks every address it holds, so a constant that merely looks like one is not taken
for one. A module without them (the system's fixed-base DLLs) falls back to "lands inside the
module". In 16-bit code the data segment's variables are the operands with a fixed offset, and
the addresses are the places relocation writes.

The counts also say how many bytes of the code sections no function reaches: tables, strings and
padding kept among the code, and code whose start nothing gives away.

Reached means: called, jumped to, or having its address taken by something reached. A function
nothing refers to at all is reported as unreferenced -- dead, or entered in a way the code does
not show (a table index, an ordinal computed at run time).
"""

from codebreaker.analyze import image

READS = ("cmp", "test", "push", "bt", "call", "jmp", "lcall", "ljmp", "fld", "fild", "fcomp", "fcom",
          "fadd", "fsub", "fmul", "fdiv", "ficom", "ficomp", "bound", "div", "idiv", "mul")
PURE_WRITES = ("mov", "pop", "fstp", "fst", "fistp", "fist", "lea", "movzx", "movsx", "sidt", "sgdt",
                "fstcw", "fnstcw", "fnstsw", "fstsw")


class Graph(object):
    def __init__(self):
        self.calls, self.callers, self.refs, self.taken, self.users = {}, {}, [], {}, {}
        self.order, self.cycles, self.reachable, self.unreferenced = [], [], set(), []
        self.reachable_with_tables = set()


def _relocated(img, ins):
    """The addresses the loader writes into this instruction: {value}; None when the file does
    not say where addresses are."""
    if img.pointers is None:
        return None
    out = set()
    for a in range(ins.addr + 1, ins.addr + ins.size - (3 if img.bits == 32 else 1)):
        p = img.pointers.get(a)
        if p is not None and p[1] is not None:
            out.add(p[1])
    return out


def _references(img, fn, ins):
    """(address, how) for every operand of one instruction that refers to a place in the module."""
    known = _relocated(img, ins)
    if known is not None and not known and img.bits == 32:
        return
    for k, op in enumerate(ins.ops):
        if op[0] == "i":
            if ins.flow or ins.target is not None:
                continue
            if img.bits == 32:
                if (op[1] in known if known is not None else (op[2] == 4 and img.is_data(op[1]))):
                    yield op[1], "a"
        elif op[0] == "m" and op[1] is None:
            if img.bits == 32:
                va = op[4]
                if not (va in known if known is not None else img.span(va) is not None):
                    continue
            else:
                if op[5] not in (None, "ds") or not img.data_segment or op[2] is not None:
                    continue
                va = img.data_segment + (op[4] & 0xFFFF)
                if img.span(va) is None:
                    continue
            if ins.mn == "lea":
                yield va, "a"
            elif k or ins.mn in READS:
                yield va, "r"
            else:
                yield va, "w" if ins.mn in PURE_WRITES or ins.mn.startswith("set") else "rw"
        elif op[0] == "m" and img.bits == 32:                    # [reg + ADDRESS]: a table indexed from a register
            va = op[4] & 0xFFFFFFFF
            if (va in known if known is not None else (va >= 0x10000 and img.span(va) is not None)):
                yield va, "a" if ins.mn == "lea" else "r" if (k or ins.mn in READS) else "rw"
    if img.bits == 16 and known:                                 # a far address or a segment written into the code
        for va in known:
            if ins.flow is None or ins.target is None:
                yield va, "a"


def _strongly_connected(nodes, edges):
    """Tarjan without recursion. -> components, callees first."""
    index, low, on, stack, out, counter = {}, {}, set(), [], [], 0
    for root in nodes:
        if root in index:
            continue
        work = [(root, iter(edges.get(root, ())))]
        index[root] = low[root] = counter
        counter += 1
        stack.append(root)
        on.add(root)
        while work:
            v, it = work[-1]
            for w in it:
                if w not in index:
                    index[w] = low[w] = counter
                    counter += 1
                    stack.append(w)
                    on.add(w)
                    work.append((w, iter(edges.get(w, ()))))
                    break
                if w in on:
                    low[v] = min(low[v], index[w])
            else:
                work.pop()
                if work:
                    low[work[-1][0]] = min(low[work[-1][0]], low[v])
                if low[v] == index[v]:
                    comp = []
                    while True:
                        w = stack.pop()
                        on.discard(w)
                        comp.append(w)
                        if w == v:
                            break
                    out.append(comp)
    return out


def _build(img):
    g = Graph()
    funcs = set(img.functions)
    direct = {}
    for fn in img.each():
        calls = g.calls[fn.start] = []
        edges = direct[fn.start] = set()
        for ins in fn.insns:
            if ins.flow in ("call", "jmp"):
                dest = image.destination(img, ins)
                if dest in img.imports:
                    calls.append((ins.addr, "import" if ins.flow == "call" else "tail", dest))
                    g.users.setdefault(img.imports[dest], set()).add(fn.start)
                elif dest is None:
                    if ins.flow == "call" or not fn.tables or all(t["site"] != ins.addr for t in fn.tables):
                        calls.append((ins.addr, "indirect", None))
                elif dest in funcs and (ins.flow == "call" or dest != fn.start and not fn.start < dest < fn.limit):
                    calls.append((ins.addr, "call" if ins.flow == "call" else "tail", dest))
                    edges.add(dest)
                    g.callers.setdefault(dest, set()).add(fn.start)
            for va, how in _references(img, fn, ins):
                if va in img.imports:
                    continue
                g.refs.append((fn.start, ins, va, how))
                if va in funcs and how == "a":
                    g.taken.setdefault(va, []).append(("code", fn.start, ins.addr))
    # function addresses held in data
    if img.pointers is not None:
        for site, (_kind, target) in img.pointers.items():
            if target in funcs and (not img.is_code(site) or img.owner(site) is None):
                g.taken.setdefault(target, []).append(("data", None, site))
    else:
        for va, data, executable, _w, _name, held in img.spans:
            if executable:
                continue
            for off in range(0, held - 3, 4):
                v = int.from_bytes(data[off:off + 4], "little")
                if v in funcs:
                    g.taken.setdefault(v, []).append(("data", None, va + off))
    g.callers = {k: sorted(v) for k, v in g.callers.items()}
    g.users = {k: sorted(v) for k, v in g.users.items()}
    comps = _strongly_connected(img.functions, direct)
    g.order = [f for comp in comps for f in comp]
    g.cycles = [sorted(c) for c in comps if len(c) > 1 or c[0] in direct[c[0]]]
    # what the exports reach: calls, tail jumps, and addresses taken by reached code
    takes = {}
    for f, _ins, va, how in g.refs:
        if how == "a" and va in funcs:
            takes.setdefault(f, set()).add(va)
    def reach(roots):
        work = [r for r in roots if r in funcs]
        seen = set(work)
        while work:
            f = work.pop()
            for t in direct.get(f, ()) | takes.get(f, set()):
                if t not in seen:
                    seen.add(t)
                    work.append(t)
        return seen
    g.reachable = reach(img.roots)
    g.reachable_with_tables = reach(list(img.roots) + [f for f, w in g.taken.items() if any(x[0] == "data" for x in w)])
    g.unreferenced = [f for f in img.functions if f not in g.callers and f not in g.taken and f not in set(img.roots)]
    return g


def build(img):
    return img.memo("call_graph", _build)


def analyse(img):
    g = build(img)
    kinds = {"call": 0, "import": 0, "tail": 0, "indirect": 0}
    edges = 0
    facts = []
    for f in img.functions:
        callees, imports, indirect = set(), set(), 0
        for _site, kind, dest in g.calls[f]:
            kinds[kind] += 1
            if dest is None:
                indirect += 1
            elif dest in img.imports:
                imports.add(img.imports[dest])
            else:
                callees.add(dest)
        edges += len(callees)
        facts.append({"addr": f, "kind": "calls", "callees": [img.label(c) for c in sorted(callees)],
                      "imports": sorted(imports), "indirect": indirect,
                      "callers": [img.label(c) for c in g.callers.get(f, ())],
                      "address_taken": len(g.taken.get(f, ())), "reached": f in g.reachable})
    leaves = sum(1 for f in img.functions if not g.calls[f])
    reached = sum(i.size for fn in img.each() for i in fn.insns)
    held = sum(s[5] for s in img.spans if s[2])
    counts = {"functions": len(img.functions), "functions_found_from_calls": len(img.discovered),
              "edges": edges, "call_sites": kinds["call"], "import_call_sites": kinds["import"],
              "tail_jumps": kinds["tail"], "indirect_call_sites": kinds["indirect"],
              "imports_used": len(g.users), "leaf_functions": leaves, "recursive_groups": len(g.cycles),
              "address_taken_functions": len(g.taken), "data_references": len(g.refs),
              "roots": len(img.roots), "reached_from_roots": len(g.reachable),
              "reached_with_data_tables": len(g.reachable_with_tables),
              "unreferenced_functions": len(g.unreferenced),
              "code_bytes_decoded": reached, "code_section_bytes_not_reached": max(held - reached, 0)}
    top = sorted(g.callers.items(), key=lambda kv: -len(kv[1]))[:15]
    used = sorted(g.users.items(), key=lambda kv: -len(kv[1]))[:15]
    text = ["most called:"] + ["  %-28s %d callers" % (img.label(f), len(c)) for f, c in top]
    text += ["most used imports:"] + ["  %-36s %d functions" % (n, len(c)) for n, c in used]
    if g.cycles:
        text += ["recursive groups: " + "; ".join(",".join(img.label(f) for f in c[:4]) + ("..." if len(c) > 4 else "")
                                                   for c in g.cycles[:8])]
    if img.discovered:
        text += ["function starts found from calls (not in the decompiler's description): %d, e.g. %s"
                 % (len(img.discovered), ", ".join(img.label(f) for f in sorted(img.discovered)[:6]))]
    return {"counts": counts, "facts": facts, "text": text}
