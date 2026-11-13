"""gen_lanes.py <outfile> -- run from the repo root with the Python package and cbengine.dll built (the oracle for lanes_test.cpp;
goes with the Python).

Runs the scenario of tests/test_engine.py::test_a_lane_on_the_lanes_steps (a made-up lane judged in this process: prepare reads the
binary's words, build "compiles" each function to the digits of its body, judge says equivalent for an even key and divergent for an
odd one) through the live Python engine/lanes.py, and records after each stage what the steps gave -- the rows, the texts, the
build's facts, and which functions were judged -- as the C++ steps must give them for the same lane.
"""
import json
import os
import sys

sys.path.insert(0, ".")
sys.path.insert(0, "tests")
import test_engine as T  # noqa: E402
from codebreaker.engine import engine  # noqa: E402

out = sys.argv[1]
lane = T._declared()
p = T.Place("2 7 4 9")
doc = {"stages": []}


def stage(name, **extra):
    got = engine.get(p.db, "lane.tfake.rows")
    rec = {"stage": name, "rows": got, "judged": sorted(lane.JUDGED), "build": engine.last_run(p.db).get("lane.tfake.build"),
           "texts": engine.get(p.db, "lane.tfake.texts"), "keys": engine.get(p.db, "lane.tfake.keys"),
           "built": {k: engine.get(p.db, "lane.tfake.built", k) for k in ("0000", "0001", "0002", "0003")}}
    rec.update(extra)
    doc["stages"].append(rec)
    lane.JUDGED[:] = []


try:
    lane.JUDGED[:] = []
    stage("first")
    f = engine.get(p.db, "lane.tfake.function", "0002")
    doc["function_0002"] = f
    stage("again")
    engine.put(p.db, "lane.tfake.function", "0002", [f[0], f[1] + " /* by hand */"])
    stage("comment")
    engine.put(p.db, "lane.tfake.function", "0002", [f[0], f[1].replace("4", "6")])
    engine.put(p.db, "lane.tfake.function", "0003", ["f3", "int f3(void) { return x; }"])
    stage("code")
    engine.forget(p.db, "lane.tfake.function", "0002")
    engine.forget(p.db, "lane.tfake.function", "0003")
    stage("forgotten")
finally:
    p.close()
with open(out, "w") as fh:
    json.dump(doc, fh, sort_keys=True, indent=1)
