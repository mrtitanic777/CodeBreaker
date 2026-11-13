"""gen_driver_lane.py <worktree> <outfile> -- run from the repo root with the Python package built:

    python -B native/cb/verify/lanes/testdata/gen_driver_lane.py . <outfile>

Runs the live Python of codebreaker/verify/lanes/driver.py on three real Windows 95 drivers (BIOS, SPLITTER, SRAMMTD: native/cb/formats/testdata)
and records what it answered, as verify/lanes/driver.cpp must: the number of inputs of each entry point, the fingerprints of the first input of each
entry point (how it ended, EAX, the calls in order, the number of instructions that ran), the verdict of every entry point of each against itself,
and the verdicts of copies of each with one executed instruction of an entry point broken (which instruction: its place in the file and its text).
"""
import json
import os
import sys

sys.path.insert(0, ".")
from codebreaker.base import settings as _settings  # noqa: E402
MAIN = os.environ.get("CB_MAIN_CHECKOUT", "C:/Users/mrtit/Documents/GitHub/CodeBreaker")
_settings.ROOT = MAIN                                    # (settings.json, the kits and the state folder are the main checkout's)
_settings.FILE = os.path.join(MAIN, "settings.json")
_settings._loaded = None
from codebreaker.verify.lanes import driver  # noqa: E402

wt, out = sys.argv[1], sys.argv[2]
work = os.path.join(wt, "state", "work", "w67", "driver-breaks")
os.makedirs(work, exist_ok=True)
doc = {"files": {}}
for name in ("bios.vxd", "splitter.vxd", "srammtd.vxd"):
    path = wt + "/native/cb/formats/testdata/" + name
    image = driver.load(path)
    rec = {"entries": list(image.entries), "inputs": {}, "first": {}, "self": {}, "broken": {}}
    for label in image.entries:
        first, more = driver.inputs(label)
        rec["inputs"][label] = [len(first), len(more)]
        f = driver.fingerprint(image, image.entries[label], first[min(1, len(first) - 1)])
        rec["first"][label] = {"exit": f.get("exit"), "ret": f.get("ret"), "deps": list(f.get("deps") or ()),
                               "pop": f.get("pop"), "cov": len(f.get("cov") or ()), "memory": (f.get("memory") or [None])[0],
                               "result": [str(x) for x in (f.get("result") or ())]}
        v, reason, limits, detail = driver.judge((path, path), (label, label))
        rec["self"][label] = [v, reason, list(limits), detail]
        if v != "equivalent":
            continue
        for nth in range(3):
            copy = os.path.join(work, "broken_%s_%s_%d.vxd" % (name, label, nth))
            changed = driver.break_instruction(path, copy, label, nth)
            if not changed:
                break
            driver._IMAGES.pop(copy, None)
            bv, breason, blimits, bdetail = driver.judge((path, copy), (label, label))
            rec["broken"]["%s %d" % (label, nth)] = [changed[0], changed[1], bv, breason, list(blimits), bdetail]
            os.remove(copy)
    doc["files"][name] = rec
json.dump(doc, open(out, "w"), indent=1)
print("written", out, sum(len(r["broken"]) for r in doc["files"].values()), "broken judged")
