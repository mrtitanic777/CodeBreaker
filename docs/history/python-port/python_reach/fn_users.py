"""fn_users.py FILE word,word: top-level functions whose body mentions any word (imports or attribute use)."""
import ast, sys
path, words = sys.argv[1], sys.argv[2].split(",")
src = open(path, encoding="utf-8").read()
t = ast.parse(src)
lines = src.split("\n")
hit = []
for n in t.body:
    if isinstance(n, ast.FunctionDef):
        body = "\n".join(lines[n.lineno - 1:n.end_lineno])
        if any(w in body for w in words):
            hit.append(n.name)
print(",".join(hit))
