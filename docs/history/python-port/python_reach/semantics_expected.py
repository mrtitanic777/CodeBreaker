"""semantics_expected.py OUT: what CPython gives for every run_* function of tests/translate/test_python_frontend.py's SEMANTICS, one JSON line each ("name<TAB>json")."""
import json
import re
import sys

sys.path.insert(0, "tests/translate")
sys.path.insert(0, ".")
import test_python_frontend as T

names = sorted(re.findall(r"^def (run_\w+)\(", T.SEMANTICS, re.M))
space = {}
exec(compile(T.SEMANTICS, "snip", "exec"), space)
out = []
for n in names:
    try:
        v = space[n]()
        j = json.loads(json.dumps(v, default=lambda b: b.decode("latin1")))
    except Exception as e:
        j = {"error": type(e).__name__}
    out.append("%s\t%s" % (n, json.dumps(j, ensure_ascii=True, separators=(",", ":"))))
open(sys.argv[1], "w", encoding="utf-8", newline="\n").write("\n".join(out) + "\n")
print(len(out), "functions")
