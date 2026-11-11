#!/usr/bin/env python3
"""cb_order.py: bottom-up refactor order for the typed ports (docs/CPP_DESIGN.md 6.2).

Computes the include graph of native/typed/cpp/codebreaker/**, condenses strongly
connected components (cycles are refactored together), and writes docs/cpp_order.txt:
waves 0..N of units (a unit = one module or one cycle) with code-line counts,
in-tree dependencies, area batches and the native/ C++ that moves in with each unit.
A module = a path stem (x.hpp + x.cpp); .inc files belong to the module that includes them.
One-off tool: deleted with the Python.   Usage: python tools/cb_order.py            -> docs/cpp_order.txt  (every #include, .cpp too)
       python tools/cb_order.py --headers-only -> docs/cpp_order_api.txt (only .hpp/.inc edges: the
       public-API graph; it has no cycles, so it shows the order if .cpp back-edges are resolved
       header-first, which is how a cycle in the full graph is meant to be split)
"""
import os, re, sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CPP = os.path.join(ROOT, "native", "typed", "cpp")
TREE = os.path.join(CPP, "codebreaker")
HDR_ONLY = "--headers-only" in sys.argv  # edges from .hpp/.inc only (public-API graph)
OUT = os.path.join(ROOT, "docs", "cpp_order_api.txt" if HDR_ONLY else "cpp_order.txt")
EXTS = (".hpp", ".cpp", ".inc")
INC_RE = re.compile(r'^\s*#\s*include\s*([<"])([^>"]+)[>"]', re.M)


def code_lines(text):
    text = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), text, flags=re.S)
    n = 0
    for ln in text.splitlines():
        s = ln.strip()
        if s and not s.startswith("//"):
            n += 1
    return n


files = {}  # rel path (posix, from CPP) -> text
for dp, _, fns in os.walk(TREE):
    for fn in fns:
        if fn.endswith(EXTS):
            p = os.path.join(dp, fn)
            files[os.path.relpath(p, CPP).replace(os.sep, "/")] = open(p, encoding="utf-8", errors="replace").read()

by_base = defaultdict(list)
for f in files:
    by_base[os.path.basename(f)].append(f)


def resolve(src, inc):
    d = os.path.dirname(src)
    for cand in (os.path.normpath(os.path.join(d, inc)).replace(os.sep, "/"), inc):
        if cand in files:
            return cand
    b = by_base.get(os.path.basename(inc), [])
    return b[0] if len(b) == 1 else None


def stem(f):
    return f.rsplit(".", 1)[0]


# module of each file; .inc attaches to its includer's module
mod_of = {f: stem(f) for f in files if not f.endswith(".inc")}
for f in files:
    if f.endswith(".inc"):
        for g, t in files.items():
            if not g.endswith(".inc"):
                for _, inc in INC_RE.findall(t):
                    if resolve(g, inc) == f:
                        mod_of[f] = stem(g)
        mod_of.setdefault(f, stem(f))

mods = defaultdict(list)
for f, m in mod_of.items():
    mods[m].append(f)
lines = {m: sum(code_lines(files[f]) for f in fs) for m, fs in mods.items()}

deps = {m: set() for m in mods}
for f, t in files.items():
    if HDR_ONLY and f.endswith(".cpp"):
        continue
    for _, inc in INC_RE.findall(t):
        r = resolve(f, inc)
        if r and mod_of[r] != mod_of[f]:
            deps[mod_of[f]].add(mod_of[r])

# Tarjan SCC (iterative)
index, low, onst, st, idx = {}, {}, set(), [], [0]
sccs = []


def strong(v0):
    work = [(v0, iter(sorted(deps[v0])))]
    index[v0] = low[v0] = idx[0]; idx[0] += 1; st.append(v0); onst.add(v0)
    while work:
        v, it = work[-1]
        for w in it:
            if w not in index:
                index[w] = low[w] = idx[0]; idx[0] += 1; st.append(w); onst.add(w)
                work.append((w, iter(sorted(deps[w]))))
                break
            elif w in onst:
                low[v] = min(low[v], index[w])
        else:
            work.pop()
            if work:
                low[work[-1][0]] = min(low[work[-1][0]], low[v])
            if low[v] == index[v]:
                c = []
                while True:
                    w = st.pop(); onst.discard(w); c.append(w)
                    if w == v:
                        break
                sccs.append(sorted(c))


for m in sorted(mods):
    if m not in index:
        strong(m)

uid = {}
for i, c in enumerate(sccs):
    for m in c:
        uid[m] = i
udeps = [set() for _ in sccs]
for m, ds in deps.items():
    for d in ds:
        if uid[d] != uid[m]:
            udeps[uid[m]].add(uid[d])
level = {}


def lvl(u):
    stack = [u]
    while stack:
        x = stack[-1]
        pend = [d for d in udeps[x] if d not in level]
        if pend:
            stack.extend(pend)
            continue
        level[x] = 1 + max((level[d] for d in udeps[x]), default=-1)
        stack.pop()


for u in range(len(sccs)):
    lvl(u)


def short(m):
    return m.replace("codebreaker/", "", 1)


def uname(u):
    c = sccs[u]
    return short(c[0]) if len(c) == 1 else "CYCLE{" + ",".join(short(m) for m in c) + "}"


def ulines(u):
    return sum(lines[m] for m in sccs[u])


def area(u):
    parts = short(sccs[u][0]).split("/")
    return "/".join(parts[:-1][:2]) or "(root)"


HOT = [  # per CPP_DESIGN 6.2: judge tracer, describe, c_text, compare, the emulators
    (re.compile(r"emulate/(native_|shadow|driver|run|emulator|capstone|unicorn|windows16|ports16|package)"), "judge tracer / emulators"),
    (re.compile(r"emulate/"), "emulators"),
    (re.compile(r"verify/(compare|verdicts|lane_common)|^codebreaker/compare/"), "compare"),
    (re.compile(r"decompile/describe"), "describe"),
    (re.compile(r"decompile/c_text"), "c_text"),
]


def hot(u):
    for m in sccs[u]:
        for rx, why in HOT:
            if rx.search(m):
                return why
    return None


def natives(u):
    out = set()
    for m in sccs[u]:
        s = short(m)
        txt = "\n".join(files[f] for f in mods[m])
        if s.startswith("formats/"):
            out.add("native/formats")
        if s.startswith("engine/") or "cbengine" in txt:
            out.add("native/engine")
        if re.match(r"translate/(ir|l2|native_l2|rewrite|desugar)$", s):
            out.add("native/l2")
        if re.search(r"cbtrace|native/tracer", txt) or re.match(r"emulate/(native_trace|native_shadow|native_driver)$", s):
            out.add("native/tracer")
        if re.search(r"dosexec|cbdos", txt):
            out.add("native/dosexec")
    return sorted(out)


waves = defaultdict(list)
for u in range(len(sccs)):
    waves[level[u]].append(u)
NW = max(waves) + 1


def batches(us):
    byarea = defaultdict(list)
    for u in us:
        byarea[area(u)].append(u)
    order = sorted(byarea, key=lambda a: (0 if any(hot(u) for u in byarea[a]) else 1, a))
    res = []
    st_ = {"cur": [], "n": 0, "areas": []}

    def flush():
        if st_["cur"]:
            res.append((st_["areas"], st_["cur"], st_["n"]))
        st_["cur"], st_["n"], st_["areas"] = [], 0, []

    for a in order:
        for u in sorted(byarea[a], key=lambda u: (0 if hot(u) else 1, -ulines(u), uname(u))):
            if st_["cur"] and st_["n"] + ulines(u) > 3000:
                flush()
            if st_["cur"] and st_["areas"][-1] != a and st_["n"] >= 1500:
                flush()
            st_["cur"].append(u)
            st_["n"] += ulines(u)
            if a not in st_["areas"]:
                st_["areas"].append(a)
        if st_["n"] >= 1500:
            flush()
    flush()
    return res


o = []
w = o.append
w("cpp_order.txt -- bottom-up refactor order of native/typed/cpp/codebreaker/** (generated by tools/cb_order.py)")
w("")
w("Unit = one module (x.hpp + x.cpp, .inc files folded in) or one include cycle (SCC), refactored together.")
w("Waves are topological levels of the condensed include graph: a unit's dependencies are all in earlier waves.")
w("Numbering: wave 0 here = leaves of the typed tree. (The design doc's own 'wave 0', cb/core, is already built.)")
w("Lines = non-blank, non-comment lines of the unit's .hpp/.cpp/.inc. 'deps' = in-tree units it includes (none = leaf).")
w("[HOT] = hot path per the doc (judge tracer, describe, c_text, compare, emulators): take first within a wave.")
w("natives = native/ C++ the doc says moves in with the unit (formats, engine, l2, tracer, dosexec). The doc names")
w("the groups; the per-unit mapping is by area name and by which unit includes the native header.")
if HDR_ONLY:
    w("MODE: headers only. Edges come from .hpp/.inc includes; .cpp includes are ignored. A module's header can be")
    w("refactored once the headers it includes are; its .cpp may need modules of a later wave (see cpp_order.txt).")
    w("")
tot_l = sum(ulines(u) for u in range(len(sccs)))
w(f"Totals: {len(mods)} modules, {len(sccs)} units, {tot_l} code lines, {NW} waves.")
w("")
w("SUMMARY")
w("wave  units  lines  batches")
for k in range(NW):
    us = waves[k]
    w(f"{k:>4}  {len(us):>5}  {sum(ulines(u) for u in us):>5}  {len(batches(us)):>5}")
cyc = [u for u in range(len(sccs)) if len(sccs[u]) > 1]
w("")
w("CYCLES (units with more than one module)")
for u in sorted(cyc, key=lambda u: -ulines(u)):
    w(f"  wave {level[u]}  {ulines(u):>6} lines  {len(sccs[u])} modules" + ("  ** over 3,000 lines **" if ulines(u) > 3000 else ""))
    w("    " + ", ".join(short(m) for m in sccs[u]))
w("")
for k in range(NW):
    us = waves[k]
    w("=" * 78)
    w(f"WAVE {k}: {len(us)} units, {sum(ulines(u) for u in us)} lines")
    w("=" * 78)
    nat = defaultdict(list)
    for u in us:
        for n in natives(u):
            nat[n].append(uname(u))
    if nat:
        w("native/ moving in this wave:")
        for n in sorted(nat):
            w(f"  {n}  <- {', '.join(nat[n])}")
    w("batches (ready to assign):")
    for i, (areas, bu, bl) in enumerate(batchs := batches(us), 1):
        w(f"  W{k}-B{i}: {', '.join(areas)} -- {len(bu)} units, {bl} lines")
        for u in bu:
            h = " [HOT: " + hot(u) + "]" if hot(u) else ""
            d = sorted(uname(x) for x in udeps[u])
            n = natives(u)
            w(f"    {uname(u)}  {ulines(u)}{h}")
            w(f"        deps: {', '.join(d) if d else '(none)'}")
            if n:
                w(f"        natives: {', '.join(n)}")
    w("")
os.makedirs(os.path.dirname(OUT), exist_ok=True)
open(OUT, "w", encoding="utf-8", newline="\n").write("\n".join(o) + "\n")
print("wrote", OUT)
