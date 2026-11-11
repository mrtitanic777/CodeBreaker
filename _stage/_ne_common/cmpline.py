"""cmpline.py <harness folder> [N] -- the first N lines of expected.txt / actual.txt that differ: the case and the text around the first difference."""
import sys

folder = sys.argv[1]
n = int(sys.argv[2]) if len(sys.argv) > 2 else 3
a = open(folder + "/expected.txt", encoding="ascii", newline="").read().replace("\r\n", "\n").split("\n")
b = open(folder + "/actual.txt", encoding="ascii", newline="").read().replace("\r\n", "\n").split("\n")
shown = 0
print("lines: expected %d actual %d" % (len(a), len(b)))
for i in range(max(len(a), len(b))):
    x = a[i] if i < len(a) else None
    y = b[i] if i < len(b) else None
    if x != y:
        print("line", i + 1, (x or y or "")[:120].replace("\t", " | "))
        if x is None or y is None:
            print("  missing in", "expected" if x is None else "actual")
        else:
            k = next((j for j in range(min(len(x), len(y))) if x[j] != y[j]), min(len(x), len(y)))
            print("  at char", k)
            print("  exp:", x[max(0, k - 70):k + 110])
            print("  act:", y[max(0, k - 70):k + 110])
        shown += 1
        if shown >= n:
            break
