"""byop.py CASES ACTUAL EXPECTED [OP [N]] -- the differing lines of a case-per-line harness, counted by operation (the first
tab-separated field of the case), with N examples of OP."""
import sys

cases = open(sys.argv[1], "rb").read().replace(b"\r\n", b"\n").split(b"\n")
a = open(sys.argv[2], "rb").read().replace(b"\r\n", b"\n").split(b"\n")
b = open(sys.argv[3], "rb").read().replace(b"\r\n", b"\n").split(b"\n")
want = sys.argv[4].encode() if len(sys.argv) > 4 else None
n = int(sys.argv[5]) if len(sys.argv) > 5 else 5
is_err = lambda s: b"error" in s or b"ERR" in s
counts, errs, shown = {}, {}, 0
for i, c in enumerate(cases):
    if not c.strip():
        continue
    op = c.split(b"\t")[0]
    x = a[i] if i < len(a) else b"<missing>"
    y = b[i] if i < len(b) else b"<missing>"
    if x == y:
        continue
    if is_err(x) and is_err(y):
        errs[op] = errs.get(op, 0) + 1
        continue
    counts[op] = counts.get(op, 0) + 1
    if want == op and shown < n:
        shown += 1
        print("case:    ", c[:260].decode("latin1"))
        print("  actual:  ", x[:200].decode("latin1"))
        print("  expected:", y[:200].decode("latin1"))
print("differing by op:", {k.decode(): v for k, v in sorted(counts.items())})
print("error kind only:", {k.decode(): v for k, v in sorted(errs.items())})
