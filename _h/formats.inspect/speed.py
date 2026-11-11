"""speed.py -- the C++ harness's time by kind of call (compare with expected.py's stderr line).

Run from this folder after build.cmd:  python speed.py
"""
import os
import subprocess
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
env = dict(os.environ, CB_CHECKOUT_ROOT=os.path.abspath(os.path.join(HERE, "..", "..", "..", "..")))
by = {}
for line in open(os.path.join(HERE, "cases.txt"), encoding="utf-8"):
    by.setdefault(line.rstrip("\n").split("\t", 1)[0], []).append(line)
total = 0.0
parts = []
for op, lines in sorted(by.items()):
    p = os.path.join(HERE, "only_%s.txt" % op)
    open(p, "w", encoding="utf-8", newline="\n").writelines(lines)
    t = time.perf_counter()
    subprocess.run([os.path.join(HERE, "test.exe"), p], stdout=subprocess.DEVNULL, env=env, cwd=HERE)
    dt = time.perf_counter() - t
    total += dt
    parts.append("%s %.2f" % (op, dt))
    os.remove(p)
sys.stdout.write("C++ seconds by call (each run is its own process): " + ", ".join(parts) + " (total %.2f)\n" % total)
