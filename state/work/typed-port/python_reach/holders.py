"""holders.py <suffix>...: after fixpoint's cut, which kept files / b6 modules import each directly."""
import json, collections, sys
exec(open("state/work/typed-port/python_reach/fixpoint.py").read().split("print(\"iterations\"")[0])
for s in sys.argv[1:]:
    t = "codebreaker." + s
    kept_files = [f for f, ms in fileimp.items() if f not in cut and t in ms]
    mods = [x[11:] for x in g if t in g[x] and x in alive]
    print(s, "alive" if t in alive else "dead", "| kept files:", kept_files, "| alive importers:", mods)
