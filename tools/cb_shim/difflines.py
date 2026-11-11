"""difflines.py ACTUAL EXPECTED [N] [--soft] -- compare two line files ignoring CRLF/LF; print the first N differing lines.

--soft: two lines that are both errors (contain ERR, EXC or "error") count as equal (the category of an error is not
compared across the Python and C++ implementations, docs/CPP_DESIGN.md 7.1); the number of such lines is reported."""
import sys

args = [x for x in sys.argv[1:] if not x.startswith("--")]
soft = "--soft" in sys.argv
a = open(args[0], "rb").read().replace(b"\r\n", b"\n").split(b"\n")
b = open(args[1], "rb").read().replace(b"\r\n", b"\n").split(b"\n")
n = int(args[2]) if len(args) > 2 else 10
is_err = lambda s: b"ERR" in s or b"EXC" in s or b'"error"' in s
bad = soft_equal = 0
for i in range(max(len(a), len(b))):
    x = a[i] if i < len(a) else b"<missing>"
    y = b[i] if i < len(b) else b"<missing>"
    if x != y:
        if soft and is_err(x) and is_err(y):
            soft_equal += 1
            continue
        bad += 1
        if bad <= n:
            print("line %d\n  actual:   %s\n  expected: %s" % (i + 1, x[:300].decode("latin1"), y[:300].decode("latin1")))
print("%d lines, %d differ%s" % (max(len(a), len(b)), bad, ", %d differ only in the kind of error" % soft_equal if soft else ""))
