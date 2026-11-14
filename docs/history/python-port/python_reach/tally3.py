import json, collections
P = "docs/history/python-port/python_reach/"
ok = json.load(open(P + "solve3.json")) + ["codebreaker.build.ppc_asm", "codebreaker.formats.xex_edit", "codebreaker.base.tidy_state", "codebreaker.build.period16"]
rows = {r["m"]: r for r in json.load(open(P + "rows.json"))}
c = collections.defaultdict(lambda: [0, 0])
for m in ok:
    k = ".".join(m.split(".")[1:3]) if m.split(".")[1] in ("source", "ui", "recompile", "symbols", "build", "formats", "base", "engine", "knowledge", "analyze", "compare", "coverage", "invert") else m
    if m.split(".")[1] in ("invert", "coverage"):
        k = m.split(".")[1]
    if m.startswith("codebreaker.source.vxd"):
        k = "source.vxd"
    if m.startswith("codebreaker.source.ne_asm"):
        k = "source.ne_asm"
    c[k][0] += 1
    c[k][1] += rows[m]["lines"]
for k, v in sorted(c.items()):
    print(k, v)
print(len(ok), sum(rows[m]["lines"] for m in ok))
