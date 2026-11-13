"""gen_observe.py <outfile> <module>:<limit>... -- run from the repo root with the Python package and the native DLLs built
(the oracle for observe_test.cpp's golden test; goes with the Python).

For each module (a PE file path, `crtdll.dll:120`) the live Python (codebreaker.analyze.observe) runs its functions in the emulator
with one process (so that the two patches below hold) and the file records:

  argc         what the import-argument resolver answered for every import the runs met (the C++ test answers the same table)
  candidates   the functions run, in the order the Python chose them: [address, argument count, pointer arguments, battery]
  seen         the observation of each function
  report       observe.analyse of them: counts, facts, text

Patched, so that the run does not depend on the machine: the emulator's headers-derived "gives something back through a
pointer" table is empty (as the Python itself has it when there are no headers); everything else is the Python as it is.
"""
import json
import sys

sys.path.insert(0, ".")
from codebreaker.analyze import image, observe  # noqa: E402
from codebreaker.emulate import emulator  # noqa: E402
from codebreaker.jobs import pool  # noqa: E402

out = sys.argv[1]
doc = {}
argc = {}
real_argc = observe._argc


def recording_argc(dll, name):
    r = real_argc(dll, name)
    argc["%s!%s" % (dll, name)] = r
    return r


observe._argc = recording_argc
emulator._outs = lambda name: []
captured = {}
real_run = pool.run


def capturing_run(*a, **kw):
    captured["items"] = list(a[1])
    return real_run(*a, **kw)


pool.run = capturing_run


def obs(o):
    if o is None:
        return None
    return {"fields": {str(k): {str(off): c for off, c in v.items()} for k, v in o["fields"].items()},
            "nested": {"%d,%d" % k: {str(off): c for off, c in v.items()} for k, v in o["nested"].items()},
            "indirect": [list(x) for x in o["indirect"]], "passes": [list(x) for x in o["passes"]],
            "exit": o["exit"], "removed": o["removed"], "returns": list(o["returns"]) if o["returns"] else None,
            "idiom": o["idiom"]}


for spec in sys.argv[2:]:
    path, _, limit = spec.rpartition(":")
    battery_only = path.endswith("@battery")       # `crtdll.dll@battery:400`: only what the idiom battery answered
    path = path.replace("@battery", "")
    limit = int(limit)
    img = image.load(path)
    seen = observe.run(img, limit, 1)
    items = captured["items"]
    if battery_only:
        doc[path.replace("\\", "/").rsplit("/", 1)[-1] + "@battery"] = {
            "battery": [[f, a, seen[f]["idiom"]] for f, a, p, b in items if b]}
        continue
    report = observe.analyse(img, limit, 1)
    facts = []
    for f in report["facts"]:
        f = dict(f)
        if f.get("returns"):
            f["returns"] = list(f["returns"])
        facts.append(f)
    doc[path.replace("\\", "/").rsplit("/", 1)[-1]] = {
        "limit": limit,
        "candidates": [[f, a, list(p), bool(b)] for f, a, p, b in items],
        "seen": {str(f): obs(o) for f, o in seen.items()},
        "report": {"counts": report["counts"], "facts": facts, "text": report["text"]}}
doc["argc"] = argc
with open(out, "w") as fh:
    json.dump(doc, fh, sort_keys=True, separators=(",", ":"))
