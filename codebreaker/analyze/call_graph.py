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

The graph is worked out in C++ (native/analyze, call_graph.cc) over the image cbanalyze.dll
holds; this module makes its Graph from the answer and reports it.

The counts also say how many bytes of the code sections no function reaches: tables, strings and
padding kept among the code, and code whose start nothing gives away.

Reached means: called, jumped to, or having its address taken by something reached. A function
nothing refers to at all is reported as unreferenced -- dead, or entered in a way the code does
not show (a table index, an ordinal computed at run time).
"""

READS = ("cmp", "test", "push", "bt", "call", "jmp", "lcall", "ljmp", "fld", "fild", "fcomp", "fcom",
          "fadd", "fsub", "fmul", "fdiv", "ficom", "ficomp", "bound", "div", "idiv", "mul")
PURE_WRITES = ("mov", "pop", "fstp", "fst", "fistp", "fist", "lea", "movzx", "movsx", "sidt", "sgdt",
                "fstcw", "fnstcw", "fnstsw", "fstsw")


class Graph(object):
    def __init__(self):
        self.calls, self.callers, self.refs, self.taken, self.users = {}, {}, [], {}, {}
        self.order, self.cycles, self.reachable, self.unreferenced = [], [], set(), []
        self.reachable_with_tables = set()


def _build(img):
    """The graph, worked out in C++ (native/analyze, call_graph.cc) over the image the DLL holds."""
    got = img._handle().ask("call_graph.build")
    g = Graph()
    g.calls = {f: [tuple(c) for c in cs] for f, cs in got["calls"]}
    g.callers = {f: fs for f, fs in got["callers"]}
    for addr, fs in got["users"]:                     # by name: two addresses may stand for one import
        g.users.setdefault(img.imports[addr], set()).update(fs)
    g.users = {k: sorted(v) for k, v in g.users.items()}
    g.refs = [(f, img.function(f).insns[img.function(f).index[at]], va, how) for f, at, va, how in got["refs"]]
    g.taken = {va: [tuple(t) for t in ts] for va, ts in got["taken"]}
    g.order, g.cycles = got["order"], got["cycles"]
    g.reachable, g.reachable_with_tables = set(got["reachable"]), set(got["reachable_with_tables"])
    g.unreferenced = got["unreferenced"]
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
