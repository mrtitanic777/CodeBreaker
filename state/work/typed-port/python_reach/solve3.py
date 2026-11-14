"""Which pool modules can go: no importer outside the deleted set, every test holder mapped natively (half 1 of test_port_map.md)."""
import json, sys, collections
P = "state/work/typed-port/python_reach/"
pool = json.load(open(P + "pool3.json"))
dead = set(pool["dead"])
rows = {r["m"]: r for r in json.load(open(P + "rows.json"))}
g = json.load(open(P + "live3.json"))["graph"]
MAPPED = set(sys.argv[1].split(","))
ALSO_BLOCK = set()
ALLOW_NOREP = ("codebreaker.invert.", "codebreaker.build.disc_image", "codebreaker.build.floppy")
BLOCK = ("codebreaker.platforms.", "codebreaker.verify.lanes.", "codebreaker.java.projects", "codebreaker.exact.", "codebreaker.translate.rewrite", "codebreaker.java.console.xdk_scan", "codebreaker.engine.win16", "codebreaker.source.genesis_writer", "codebreaker.source.nes_writer")
imp = collections.defaultdict(set)
for m in dead:
    for x in g[m]:
        imp[x].add(m)
ok = set()
for m in dead:
    r = rows[m]
    if r["boot"]:
        continue
    if any(t[5:-3] not in MAPPED for t in r["tests"]):
        continue
    if m.startswith(ALLOW_NOREP):
        pass
    elif m in ALSO_BLOCK or any(m.startswith(b) for b in BLOCK) or not r["rep"]:
        continue
    ok.add(m)
changed = True
while changed:
    changed = False
    for m in list(ok):
        # importers must all be in ok (dead importers) -- a live importer cannot exist (pool = not live)
        if any(i not in ok for i in imp[m] if i != m):
            ok.discard(m)
            changed = True
print("deletable", len(ok), sum(rows[m]["lines"] for m in ok))
pk = collections.defaultdict(list)
for m in sorted(ok):
    pk[".".join(m.split(".")[1:3])].append(m)
for k, v in sorted(pk.items()):
    ts = sorted({t[5:-3] for m in v for t in rows[m]["tests"]})
    print("%-28s %3d %6d tests=%s" % (k, len(v), sum(rows[m]["lines"] for m in v), ts))
json.dump(sorted(ok), open(P + "solve3.json", "w"))
