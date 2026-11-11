"""sample_text_cases.py OUT -- a stratified sample of the exact.draft.text harness (cases.txt + expected.txt, the results the
Python module gave) written as native/cb/exact/testdata/draft_text.cases: "case<TAB>=><TAB>result json" per line.

Left out on purpose (documented DIVERGES of the C++ module): a line index that is negative or past the end (Python
wraps or raises IndexError), text with non-ASCII characters (Python's word class is Unicode), and a register name that
is a regular expression."""
import re
import sys

H = "C:/Users/mrtit/Documents/GitHub/CodeBreaker/state/work/typed-port/exact.draft.text/"
OUT = sys.argv[1]
PER_OP = 14

cases = open(H + "cases.txt", "rb").read().decode("utf-8").replace("\r\n", "\n").split("\n")
expected = open(H + "expected.txt", "rb").read().decode("utf-8").replace("\r\n", "\n").split("\n")
meta = re.compile(r"[().|*+?\[\]\\]")
eligible = []
for c, e in zip(cases, expected):
    if not c.strip():
        continue
    op = c.split("\t")[0]
    if op == "changed" or '"error"' in e:
        continue
    if any(ord(ch) > 127 for ch in c):
        continue
    fields = c.split("\t")[1:]
    if op in ("opener", "openers", "blockend", "ifparts"):
        lines = fields[0][1:].split("\x1f") if fields[0][1:] else []
        k = int(fields[1][1:])
        if k < 0 or k >= len(lines):
            continue
    if op.startswith("liveafter") and (meta.search(fields[2][1:]) or int(fields[1][1:]) < -1):
        continue
    eligible.append((op, c + "\t=>\t" + e))
count = {}
for op, _ in eligible:
    count[op] = count.get(op, 0) + 1
seen, kept = {}, []
for op, line in eligible:
    n = seen[op] = seen.get(op, 0) + 1
    stride = max(1, count[op] // PER_OP)
    if n % stride == 1 or stride == 1:
        kept.append(line)
open(OUT, "w", encoding="utf-8", newline="\n").write("\n".join(kept) + "\n")
print(len(kept), "cases", count)
