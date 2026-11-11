"""subset.py <harness folder> <regex of the case line> [--max N] -- runs the harness' test.exe on the cases whose line matches, and compares with expected.txt
(a development aid: prints the number of cases, identical ones, the first differing one, and the time).  Run with the run.cmd environment set."""
import os
import re
import subprocess
import sys
import time

folder, rx = sys.argv[1], re.compile(sys.argv[2])
mx = int(sys.argv[sys.argv.index("--max") + 1]) if "--max" in sys.argv else 10 ** 9
os.chdir(folder)
cases = [ln.rstrip("\n") for ln in open("cases.txt", encoding="ascii") if rx.search(ln)][:mx]
exp = {}
for ln in open("expected.txt", encoding="ascii"):
    k, _, v = ln.rstrip("\n").partition("\t=>\t")
    exp[k] = v
open("c_sub.txt", "w", newline="\n").write("\n".join(cases) + "\n")
t = time.time()
extra = [sys.argv[sys.argv.index("--arg") + 1]] if "--arg" in sys.argv else []
p = subprocess.run([os.path.abspath("test.exe"), "c_sub.txt"] + extra, capture_output=True, text=True)
dt = time.time() - t
got = {}
for ln in p.stdout.splitlines():
    k, _, v = ln.partition("\t=>\t")
    got[k] = v
same = sum(1 for c in cases if got.get(c) == exp.get(c))
print("%d cases, %d identical, exit %d, %.1f s" % (len(cases), same, p.returncode, dt))
print(p.stderr[-500:])
shown = 0
for c in cases:
    if got.get(c) != exp.get(c):
        print("DIFF", c[:160])
        print("  exp", (exp.get(c) or "")[:220])
        print("  got", (got.get(c) or "")[:220])
        shown += 1
        if shown >= 2:
            break
