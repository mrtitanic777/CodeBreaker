"""direct.py <suffix>...: direct importers (modules, tests, tools, native generators) of each module, from a fresh graph.json."""
import json, sys
P = "state/work/typed-port/python_reach/"
G = json.load(open(P + "graph.json"))
g, ext = G["graph"], G["ext"]
g["codebreaker.__main__"] = []
for s in sys.argv[1:]:
    t = "codebreaker." + s
    print("%-34s modules=%s files=%s" % (s, sorted(x[11:] for x in g if t in g[x]), sorted(ext.get(t, []))))
