import json, os, re, collections, sys
P = "state/work/typed-port/python_reach/"
d = json.load(open(P + "graph.json"))
graph, ext, files = d["graph"], d["ext"], d["files"]
DL = json.load(open(P + "deleted_lines.json")) if os.path.exists(P + "deleted_lines.json") else {}
deleted = set(json.load(open(P + "deleted.json"))) if os.path.exists(P + "deleted.json") else set()
main_src = open("codebreaker/__main__.py", encoding="utf-8").read()
cmd_mod = dict(re.findall(r'"([\w-]+)": \("(codebreaker\.[\w.]+)"', main_src))
native_names = set(re.findall(r'"([\w-]+)"', re.search(r"NATIVE = \((.*?)\)", main_src, re.S).group(1)))
# batch 3 (2026-11-13): every command but `ui` is native, so no command module is a root; the roots are the UI, __main__ (corpus.take_option only)
# and the tools/*.py still used
live_cmds = []


# `codebreaker.__main__` names every command's module as a string, but in the exe the NATIVE commands never reach their Python:
# keep only the edges of the commands that still run Python there.
graph["codebreaker.__main__"] = ["codebreaker.base.corpus"]
# corpus.take_option is all `__main__` runs; the lazy imports of corpus (formats.detect, platforms) belong to its other callers
graph["codebreaker.base.corpus"] = ["codebreaker", "codebreaker.base", "codebreaker.base.settings"]


def closure(rs):
    seen, st = set(), list(rs)
    while st:
        m = st.pop()
        if m in seen or m not in graph:
            continue
        seen.add(m)
        st.extend(graph.get(m, []))
    return seen


base_roots = {"codebreaker", "codebreaker.base.corpus", "codebreaker.base.settings"}
plat = {m for m in graph if m.startswith("codebreaker.platforms")}
cmd_cl = {c: closure({cmd_mod[c]} | base_roots | plat) for c in live_cmds}
ui_cl = closure({"codebreaker.ui", "codebreaker.ui.terminal", "codebreaker.ui.client"} | base_roots)
boot_roots = {m for m in graph if m.startswith("codebreaker.verify.gate16")} | {"codebreaker.source.module_writer"}
boot_cl = closure(boot_roots)
plat_cl = closure(plat)

# text mentions in native sources / data (comments included)
blobs = []
for top in ("codebreaker", "native/cb", "native/cbexe"):
    for dp, dn, fs in os.walk(top):
        dn[:] = [x for x in dn if x != "build"]
        for f in fs:
            if f.endswith((".json", ".txt", ".cpp", ".hpp", ".h", ".cc", ".cmake", ".cmd")):
                try:
                    blobs.append(open(os.path.join(dp, f), encoding="utf-8", errors="replace").read())
                except Exception:
                    pass
blob = "\n".join(blobs)

# native files by dir
native = collections.defaultdict(list)
for dp, dn, fs in os.walk("native/cb"):
    dn[:] = [x for x in dn if x not in ("build", "testdata", "compat")]
    for f in fs:
        if f.endswith((".cpp", ".hpp")):
            native[dp.replace("\\", "/")].append(f)


def norm(s):
    return re.sub(r"[^a-z0-9]", "", s.lower())


def native_for(mod):
    parts = mod.split(".")[1:]
    if not parts:
        return None, False
    leaf = parts[-1]
    dirp = "native/cb/" + "/".join(parts[:-1]) if len(parts) > 1 else "native/cb"
    isinit = files[mod].endswith("__init__.py")
    if isinit:
        dirp, leaf = dirp + "/" + leaf, leaf
    nl = norm(leaf)
    cands, tests = [], []
    for f in sorted(native.get(dirp, [])):
        stem = f.rsplit(".", 1)[0]
        n = norm(stem)
        if stem.endswith("_test"):
            if norm(stem[:-5]).startswith(nl) or (isinit and True):
                tests.append(f)
        elif n == nl or (not isinit and n.startswith(nl)) or (nl.startswith(n) and len(n) > 4 and not isinit):
            cands.append(f)
    if isinit and not cands and os.path.isdir(dirp):
        cands = ["(%s/ package)" % dirp.replace("native/cb/", "")]
        dd = [f for f in native.get(dirp, [])]
    rep = None
    if cands:
        rep = dirp.replace("native/cb/", "cb/") + "/" + cands[0]
        if len(cands) > 1:
            rep += " +%d" % (len(cands) - 1)
    return rep, bool(tests)


gen_text = {}
for dp, dn, fs in os.walk("native/cb"):
    dn[:] = [x for x in dn if x != "build"]
    for f in fs:
        if f.endswith(".py"):
            gen_text[os.path.join(dp, f).replace("\\", "/")] = 1


_roots = ui_cl | closure({"codebreaker.__main__"})
_tool_roots = {m for m, fs in ext.items() if any(f.startswith("tools/") for f in fs)}
# edges no AST sees: plugins named in toolchain JSON, the java project packages loaded by manifest name, the lanes platforms/__init__ names by
# concatenation ("codebreaker.verify.lanes." + name)
_dyn = set()
for _f in os.listdir("data/exact/toolchains"):
    for _x in re.findall(r'"(codebreaker\.[\w.]+):\w+"', open("data/exact/toolchains/" + _f, encoding="utf-8").read()):
        _dyn.add(_x)
_dyn |= {m for m in graph if m.startswith(("codebreaker.java.projects.", "codebreaker.verify.lanes."))}
live_cl = closure(_roots | _tool_roots | _dyn)               # everything a live command, the UI, the platforms registry or a tool can reach
tool_only = closure(_tool_roots) - closure(_roots)    # kept alive by tools/*.py alone
json.dump({"live": sorted(live_cl), "cutgraph": {m: sorted(v) for m, v in graph.items()}}, open(P + "live.json", "w"))
rows = []
for m in sorted(graph):
    if m.startswith("codebreaker.ui") and m not in ("codebreaker.ui",):
        pass
    lines = DL[m] if m in DL else sum(1 for _ in open(files[m], encoding="utf-8", errors="replace"))
    cmds = [c for c in live_cmds if m in cmd_cl[c]]
    uiflag = m in ui_cl
    boot = m in boot_cl
    platflag = m in plat_cl and m not in {x for x in plat}
    fs_ = ext.get(m, [])
    tests = sorted({os.path.basename(f) for f in fs_ if f.startswith("tests/")})
    tools = sorted({os.path.basename(f) for f in fs_ if f.startswith("tools/")})
    gens = sorted({os.path.basename(f) for f in fs_ if f.startswith("native/")})
    mention = bool(re.search(re.escape(m) + r"(?![\w])", blob))
    rep, ntest = native_for(m)
    rows.append(dict(m=m, lines=lines, cmds=cmds, ui=uiflag, boot=boot, tests=tests, tools=tools, gens=gens,
                     mention=mention, rep=rep, ntest=ntest, plat=platflag))

json.dump(rows, open(P + "rows.json", "w"))


def reached(r):
    out = []
    if r["cmds"]:
        out.append("live cmds: " + ("all %d" % len(r["cmds"]) if len(r["cmds"]) > 3 else ",".join(r["cmds"])))
    if r["ui"]:
        out.append("UI")
    if r["plat"] and not r["cmds"]:
        out.append("platforms registry")
    if r["tests"]:
        out.append("tests: " + (",".join(t[5:-3] for t in r["tests"][:3]) + ("+%d" % (len(r["tests"]) - 3) if len(r["tests"]) > 3 else "")))
    if r["tools"]:
        out.append("tools: " + ",".join(t[:-3] for t in r["tools"][:3]))
    if r["gens"]:
        out.append("native gens: %d" % len(r["gens"]))
    if r["mention"] and not out:
        out.append("named in native text only")
    if not out and r["m"] in live_cl:
        out.append("closure (lazy imports of live modules)")
    return "; ".join(out) or "nothing"


def conds(r):
    c1 = "Y" if r["rep"] else "N"
    c2 = "Y" if r["ntest"] else "N"
    c3 = "G" if r["gens"] else "-"
    live = bool(r["cmds"] or r["ui"] or r["plat"] or r["tools"])
    c4 = "N" if live else "Y"
    c5 = "N" if r["boot"] else "Y"
    return c1 + c2 + c3 + c4 + c5


pk = collections.defaultdict(list)
for r in rows:
    parts = r["m"].split(".")
    key = ".".join(parts[:2]) if len(parts) > 1 else "(root)"
    pk[key].append(r)

out = []
w = out.append
tot_l = sum(r["lines"] for r in rows)
del_l = sum(r["lines"] for r in rows if r["m"] in deleted)
w("# Python reach inventory: who still reaches each codebreaker/ module")
w("")
w("Made 2026-11-13, re-run after batch 3 (every command but `ui` native; batches 1, 2 and 3 deleted; the totals and rows below are the tree after batch 3). Every row is")
w("computed (state/work/typed-port/python_reach/graph.py and report.py: AST import graph of every .py, lazy imports and dotted-name")
w("strings included, plus tests/, tools/ and native/ generators); the batch-1 verdicts at the end were checked by hand; the batch-2 section corrects the batch-1 estimate of the deletable pool.")
w("")
w("%d modules / %d lines under codebreaker/ (ui is listed too, it is the kept terminal)." % (len(rows), tot_l))
w("")
w("## How to read it")
w("")
w("**Reached by** (what keeps a module alive today):")
w("- `live cmds`: reachable (static imports, lazy imports, dotted-name strings, the platforms registry) from the module of one of the 8")
w("  commands that still run Python in the exe: names knowledge coverage progress facts origin driver-services boot-floppy. `all N` means more than three of them.")
w("- `UI`: reachable from `ui/terminal.py` or `ui/client.py` (the thin client: stdlib + settings + verdicts only).")
w("- `platforms registry`: loaded by name from `codebreaker/platforms/*` (ORDER tuple, `Ref(\"module\", \"name\")`, `machine(...)` tuples).")
w("- `tests: x,y`: imported by `tests/test_x.py` (gate suites or pytest folders). `tools: x`: imported by `tools/x.py`.")
w("  `native gens: n`: imported by n golden generators under native/cb/**/testdata (the goldens they made are checked in).")
w("- `named in native text only`: a comment or test title in native/ or a data file mentions the dotted name; no Python caller.")
w("- The translated exe (pyengine --dynamic over codebreaker/, tests/, tools/) reaches exactly what those reach; the only")
w("  `native/typed/*.json` manifest is `codebreaker.compare.image` (live: `compare` is imported by 10 packages). The shims under")
w("  `native/cb/compat/` convert types and call the new native functions; none calls translated Python.")
w("")
w("**Conditions** (Logan's five; `feedback-delete-python-only-when-proven`), shown as five letters per module:")
w("1. native C++ for it is committed: Y when a file of the same name exists in native/cb at the same path (heuristic; N = none found).")
w("2. its own tests pass as C++: Y when a `<name>*_test.cpp` sits beside the native file (existence only; batch 1 was run).")
w("3. judge traces agree: G when native tests compare against goldens the Python made (a generator under testdata imports the")
w("   module); `-` when no such evidence is recorded. Per-body judge traces exist only for judged modules; for the rest the")
w("   Python-made golden IS the agreement evidence.")
w("4. every caller runs through codebreaker.exe: N when a live command, the UI, the platforms registry or a tool still imports it.")
w("5. the all-15 boot passes with the exe: N when the module is in the Python closure of the boot drivers (verify/gate16/*,")
w("   source/module_writer); Y means the boot never reaches it, so it cannot change the boot. There is no committed native driver for")
w("   the real boot run, so for N rows this condition cannot be shown until step 6 of docs/PYTHON_RETIREMENT.md.")
w("")
w("A module is deletable only with `YYGYY`, plus the by-hand checks in the batch section. The letters are heuristics: rows were computed on the")
w("tree before the batch-1 deletion; 9 rows read YYGYY (the port and crypto ones, and some translate ones) and the translate ones fail the by-hand")
w("check (their pytest files in tests/java and tests/translate have no confirmed native fate; `G` there comes only from a golden generator importing them).")
w("")
w("")
w(open(P + "batch1.md", encoding="utf-8").read())
w(open(P + "batch2.md", encoding="utf-8").read())
w(open(P + "batch3.md", encoding="utf-8").read())
w("### Unreached after batch 3 (computed: not in the closure of the UI, __main__ or a tool still used; tests are NOT roots here)")
w("")
for m in sorted(graph):
    if m not in live_cl:
        w("- `%s` (%d lines)" % (m[len("codebreaker."):], next(r["lines"] for r in rows if r["m"] == m)))
w("")
w("Kept alive by `tools/*.py` alone (%d modules, %d lines): %s" % (len(tool_only), sum(r["lines"] for r in rows if r["m"] in tool_only),
  ", ".join("`%s`" % m[len("codebreaker."):] for m in sorted(tool_only))))
w("")
w("## Totals by package")
w("")
w("| package | modules | lines | reached by a live cmd | UI | tests only | tools too | nothing/native-text only | YYGYY |")
w("|---|---|---|---|---|---|---|---|---|")
for k in sorted(pk):
    rs = pk[k]
    lc = sum(1 for r in rs if r["cmds"] or r["plat"])
    ui = sum(1 for r in rs if r["ui"])
    to = sum(1 for r in rs if not (r["cmds"] or r["plat"] or r["ui"]) and r["tests"] and not r["tools"])
    tl = sum(1 for r in rs if not (r["cmds"] or r["plat"] or r["ui"]) and r["tools"])
    no = sum(1 for r in rs if not (r["cmds"] or r["plat"] or r["ui"] or r["tests"] or r["tools"]))
    ok = sum(1 for r in rs if conds(r) == "YYGYY")
    w("| %s | %d | %d | %d | %d | %d | %d | %d | %d |" % (k, len(rs), sum(r["lines"] for r in rs), lc, ui, to, tl, no, ok))
w("")
w("## Every module")
w("")
for k in sorted(pk):
    w("### %s" % k)
    w("")
    w("| module | lines | reached by | native replacement | 1-5 |")
    w("|---|---|---|---|---|")
    for r in pk[k]:
        name = r["m"][len("codebreaker."):] if r["m"] != "codebreaker" else "(package)"
        tag = "DELETED batch 1" if r["m"] in deleted else ""
        w("| %s | %d | %s | %s | %s |" % (name, r["lines"], tag + (" was: " if tag else "") + reached(r) if tag else reached(r), r["rep"] or "none found", conds(r)))
    w("")
open("state/work/typed-port/python_reach.md", "w", encoding="utf-8").write("\n".join(out) + "\n")
print("wrote", len(out), "lines")
cnt = collections.Counter(conds(r) for r in rows)
print(cnt.most_common(12))
print("YYGYY:", [r["m"] for r in rows if conds(r) == "YYGYY"])
