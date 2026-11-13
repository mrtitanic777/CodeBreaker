"""gen_win16.py <outfile> <module.dll> -- run from the repo root with the Python package and the native DLLs built (the oracle for
win16_test.cpp; goes with the Python).

Runs the steps of the live Python engine/win16.py on a real 16-bit module and records what they gave: the decompiled module as the
lane's prepare made it (the data the steps read: header, description, address maps, keys, texts, notes ...), and the facts made of it
-- the segment table, the functions, the description's lines, each function's C, notes, L2 document, source, contract and, where the
compiler is set up, object and flow check.
"""
import hashlib
import json
import os
import sys

sys.path.insert(0, ".")
from codebreaker.base import corpus  # noqa: E402
from codebreaker.engine import engine, win16  # noqa: E402

out, name = sys.argv[1], sys.argv[2]
path = corpus.retail(name)


def plain(o):
    if isinstance(o, (set, frozenset)):
        return sorted(o, key=repr)
    if isinstance(o, tuple):
        return list(o)
    if isinstance(o, bytes):
        return {"$bytes": hashlib.sha1(o).hexdigest()}
    return repr(o)


db = engine.open(path)
m, lines = engine.get(db, "ne.module")
doc = {"module": {
    "header": m.header, "description": list(m.description.lines), "by_va": [[k, v] for k, v in sorted(m.by_va.items())],
    "where": [[k, list(v)] for k, v in sorted(m.where.items())], "key": dict(m.key), "near": sorted(m.near),
    "texts": dict(m.texts), "order": list(m.order), "all_keys": list(m.all_keys), "sig": m.sig,
    "notes": {k: v for k, v in list((getattr(m, "notes", None) or {}).items())[:2]}, "wide": sorted(m.wide)}}
facts = {}
for kind in ("ne.segments", "ne.description", "ne.functions", "ne.header", "ne.lines"):
    facts[kind] = engine.get(db, kind)
funcs = facts["ne.functions"]
keys = sorted(funcs)[:12]            # (the golden keeps the first dozen functions' facts: the notes are large)
note_keys = [k for k in sorted(funcs) if funcs[k]["name"] in doc["module"]["notes"]]   # (the notes kept above)
for kind in ("function.c", "function.source", "function.contract"):
    facts[kind] = {k: engine.get(db, kind, k) for k in keys}
for kind in ("function.notes",):   # (function.l2 is the notes' "l2": the test reads it there)
    facts[kind] = {k: engine.get(db, kind, k) for k in note_keys}
    facts[kind].update({k: engine.get(db, kind, k) for k in keys if k not in facts[kind] and funcs[k]["name"] not in (m.notes or {})})
compiled = {}
try:
    for k in keys[:6]:
        o = engine.get(db, "function.object", k)
        n = engine.get(db, "function.never_set", k)
        compiled[k] = {"object": None if o is None else {x: (hashlib.sha1(y).hexdigest() if isinstance(y, bytes) else y) for x, y in o.items()},
                       "never_set": n}
except Exception as e:                                   # (no compiler here: the steps that need it are not recorded)
    compiled = {"error": "%s: %s" % (type(e).__name__, e)}
doc["facts"] = facts
doc["compiled"] = compiled
doc["cflags"] = win16.CFLAGS
doc["binary"] = os.path.basename(path)
with open(out, "w") as fh:
    json.dump(doc, fh, sort_keys=True, default=plain, separators=(",", ":"))
