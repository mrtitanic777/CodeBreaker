"""gen_isolated16.py <worktree> <outfile> -- run from the repo root with the Python package built:

    python -B src/verify/gate16/testdata/gen_isolated16.py . <outfile>

Runs the live Python of codebreaker/verify/gate16/isolated16.py (check()) on a real 16-bit module (LZEXPAND.DLL: src/formats/testdata) as its own
"rebuild": the functions of its entry table as f0, f1 ... with a made-up table of how each is called (the bytes of arguments it removes, from a run),
and a build folder whose linker map (m.map) puts each name where the original has it (every function equals itself), or where the next function is
(a rebuild that does something else); with chained functions (callees run for real), with the inputs given, and with a name the tables do not know.
What it answered is what verify/gate16/isolated16.cpp must answer: the verdicts, and the first thing that differs (the input and the kind of
difference).
"""
import json
import os
import shutil
import sys

sys.path.insert(0, ".")
from codebreaker.base import settings as _settings  # noqa: E402
MAIN = os.environ.get("CB_MAIN_CHECKOUT", "C:/Users/mrtit/Documents/GitHub/CodeBreaker")
_settings.ROOT = MAIN
_settings.FILE = os.path.join(MAIN, "settings.json")
_settings._loaded = None
from codebreaker.verify.gate16 import isolated16  # noqa: E402
from codebreaker.verify.lanes import win16  # noqa: E402

wt, out = sys.argv[1], sys.argv[2]
path = os.path.abspath(wt + "/src/formats/testdata/lzexpand.dll")
nef = win16.P.loader.open(path)
places = []
for e in nef.entries():
    if e.get("seg") and (e["seg"], e["offset"]) not in places:
        places.append((e["seg"], e["offset"]))
places = places[:10]
names = ["f%d" % i for i in range(len(places))]
vectors = win16.vectors()
funcs = {}
for n, (seg, off) in zip(names, places):
    pop = win16.fingerprint(path, seg, off, False, False, vectors[0]).get("pop")
    funcs[n] = [(pop - 4) if pop else -1, True]
tables = {"funcs": funcs, "wide": [], "place": {n: list(p) for n, p in zip(names, places)}}
doc = {"names": names, "places": places, "funcs": funcs}
work = os.path.join(wt, "state", "work", "w67", "gate-folders")
shutil.rmtree(work, ignore_errors=True)


def folder(label, where):
    d = os.path.join(work, label)
    os.makedirs(d)
    with open(os.path.join(d, "m.map"), "w") as f:
        f.write("Segment Offset Symbol\n")
        for n, (seg, off) in zip(names, where):
            f.write("%04x:%08x  _%s\n" % (seg, off, n))
        f.write("0001:00000000+ _first_wins\n0001:00000010  _first_wins\n")
    return d


own = folder("own", places)
swapped = folder("swapped", places[1:] + places[:1])
doc["swapped_places"] = places[1:] + places[:1]
cases = {}


def run(label, folder_, names_, **kw):
    t = dict(tables)
    got = list(isolated16.check(path, folder_, names_, image=path, workers=1, tables=t, **kw))
    cases[label] = [[n, v, why] for n, v, why in got]


run("own", own, names)
run("swapped", swapped, names)
run("own_chain", own, names[:4], chain=True)
run("swapped_chain", swapped, names[:4], chain=True)
run("swapped_given", swapped, names[:5], inputs=[vectors[0], vectors[3], vectors[8]])
run("unknown", own, ["nope", names[0]])
doc["cases"] = cases
json.dump(doc, open(out, "w"), indent=1)
print("written", out, {k: len(v) for k, v in cases.items()})
