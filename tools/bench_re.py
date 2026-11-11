"""bench_re.py <patterns.txt> <files...>: the Python re timing and checksum that tools/bench_re.cc compares with."""
import re
import sys
import time

pats = []
for line in open(sys.argv[1], encoding="utf-8").read().split("\n"):
    if "\t" in line:
        f, p = line.split("\t", 1)
        pats.append((int(f), p))
texts = [open(p, encoding="utf-8", errors="replace", newline="").read() for p in sys.argv[2:]]
total = 0.0
for f, p in pats:
    rx = re.compile(p, f)
    n = span = 0
    t0 = time.perf_counter()
    for rep in range(3):
        for tx in texts:
            ms = list(rx.finditer(tx))
            if rep == 0:
                n += len(ms)
                for m in ms:
                    span += m.end() - m.start() + m.start() % 7
    ms_ = (time.perf_counter() - t0) * 1000 / 3
    total += ms_
    print("%8.1f ms  n=%d span=%d  %s" % (ms_, n, span, p[:60]))
print("%8.1f ms total" % total)
