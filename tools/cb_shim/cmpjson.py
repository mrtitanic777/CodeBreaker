"""cmpjson.py ACTUAL EXPECTED -- compare the compare-harness output field by field: [id, [bad, notes, learn, syms, ident]]."""
import json
import re
import sys

a = open(sys.argv[1], "rb").read().decode("latin1").replace("\r\n", "\n").split("\n")
b = open(sys.argv[2], "rb").read().decode("latin1").replace("\r\n", "\n").split("\n")
parts = {"bad": 0, "notes": 0, "learn": 0, "syms": 0, "ident": 0, "error": 0}
note_diff = {}
shown = 0
for i, (x, y) in enumerate(zip(a, b)):
    if x == y or not x.strip():
        continue
    try:
        jx, jy = json.loads(x), json.loads(y)
    except Exception:
        parts["error"] += 1
        continue
    rx, ry = jx[1], jy[1]
    if not isinstance(rx, list) or not isinstance(ry, list):
        parts["error"] += 1
        continue
    for k, name in enumerate(("bad", "notes", "learn", "syms", "ident")):
        if rx[k] != ry[k]:
            parts[name] += 1
            if name == "notes":
                for nx, ny in zip(rx[k], ry[k]):
                    if nx != ny:
                        key = re.sub(r"[0-9a-f]{2,}", "#", re.sub(r"'[^']*'", "S", ny))[:50]
                        note_diff.setdefault(key, (nx, ny))
            elif shown < 6:
                shown += 1
                print("line", i + 1, name, "\n  actual:  ", json.dumps(rx[k])[:200], "\n  expected:", json.dumps(ry[k])[:200])
print(parts)
for key, (nx, ny) in list(note_diff.items())[:12]:
    print("note  actual:  ", nx[:150], "\n      expected:", ny[:150])
