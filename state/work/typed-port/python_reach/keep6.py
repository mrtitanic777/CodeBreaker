"""keep6.py: closure of the kept-out set (boot closure + live roots); what remains of a prefix outside it."""
import json, sys
P = "state/work/typed-port/python_reach/"
d = json.load(open(P + "pool6.json"))
g, lines, live = d["graph"], d["lines"], set(d["live"])
KEEP = ("verify.authored", "verify.gate16", "verify.lanes.powerpc", "source.module_writer", "ui")
ext = json.load(open(P + "graph.json"))["ext"]
RET = {"bench_re","cache_tidy_proof","cb_order","chart","gate_teeth16","ghidra_fork","goto_report","l2check","oss_bench","sample","sleigh_verify_ppc","sleigh_verify","struct_report","trace16","verdict_diff","record","replay","copy_walk","memmap16","gdbrsp","sample_shapes_cases","sample_text_cases","causes","failures"}
toolroots = {m for m in g if any(f.startswith("tools/") and f.split("/")[-1][:-3] not in RET for f in ext.get(m, []))}
roots = {m for m in g if any(m == "codebreaker." + k or m.startswith("codebreaker." + k + ".") for k in KEEP)} | live | toolroots
seen, st = set(), list(roots)
while st:
    m = st.pop()
    if m in seen or m not in g:
        continue
    seen.add(m)
    st.extend(g[m])
print("kept closure:", len(seen), sum(lines[m] for m in seen))
json.dump(sorted(seen), open(P + "keep6.json", "w"))
for pre in sys.argv[1:]:
    inn = sorted(m for m in seen if m.startswith("codebreaker." + pre))
    out = sorted(m for m in g if m.startswith("codebreaker." + pre) and m not in seen)
    print(pre, "kept:", len(inn), [m[11:] for m in inn])
    print(pre, "free:", len(out), sum(lines[m] for m in out))
