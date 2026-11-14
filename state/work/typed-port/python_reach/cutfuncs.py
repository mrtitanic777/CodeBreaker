"""cutfuncs.py FILE name,name,...: remove those top-level functions (with their blank-line separation) and their names from `for fn in (...)` lists."""
import ast, re, sys
path, names = sys.argv[1], set(sys.argv[2].split(","))
src = open(path, encoding="utf-8").read()
lines = src.split("\n")
tree = ast.parse(src)
spans = []
for n in tree.body:
    if isinstance(n, ast.FunctionDef) and n.name in names:
        start = (n.decorator_list[0].lineno if n.decorator_list else n.lineno) - 1
        spans.append((start, n.end_lineno))
missing = names - {n.name for n in tree.body if isinstance(n, ast.FunctionDef)}
if missing:
    print("not found:", missing)
for s, e in sorted(spans, reverse=True):
    # drop the function and the blank lines after it
    while e < len(lines) and lines[e].strip() == "":
        e += 1
    del lines[s:e]
out = "\n".join(lines)
for nm in names:
    out = re.sub(r"\b%s,\s*" % nm, "", out)
    out = re.sub(r",\s*\b%s\b" % nm, "", out)
open(path, "w", encoding="utf-8", newline="\n").write(out)
print("cut", len(spans), "functions from", path)
