"""gen_conventions.py <outdir> <module>... -- run from the repo root, with the Python package and cbanalyze.dll built (the oracle
for conventions_test.cpp; goes with the Python).

For each module writes <outdir>/<name>.conventions.json: what codebreaker.analyze.conventions answered on it --
  counts, text           analyse()'s counts and report lines
  digests                [[addr, crc32 of the fact as json.dumps(fact, sort_keys=True)]] for every function, in address order
  rows                   the rows of analyse()["facts"] that are not a function's (the duplicates and the bridge layer)
  full                   the whole of the facts of the first functions that say something (variadic, thiscall, ...), for reading
"""
import json
import os
import sys
import zlib

sys.path.insert(0, ".")
from codebreaker.analyze import conventions, image  # noqa: E402

out = sys.argv[1]
os.makedirs(out, exist_ok=True)

for module in sys.argv[2:]:
    img = image.load(module)
    facts = conventions.functions(img)
    report = conventions.analyse(img)
    digests = []
    for va in sorted(facts):
        text = json.dumps(facts[va], sort_keys=True)
        digests.append([va, zlib.crc32(text.encode("utf-8"))])
    rows = [f for f in report["facts"] if f.get("kind") != "convention"]
    interesting = [facts[va] for va in sorted(facts)
                   if facts[va].get("variadic") or facts[va].get("procedure") or facts[va].get("float_args")
                   or facts[va].get("convention") in ("thiscall", "fastcall", "register") or facts[va].get("entry")]
    doc = {"counts": report["counts"], "text": report["text"], "digests": digests, "rows": rows, "full": interesting[:6]}
    with open(os.path.join(out, "%s.conventions.json" % module), "w", newline="\n") as f:
        json.dump(doc, f, sort_keys=True)
    print(module, len(facts), len(interesting))
