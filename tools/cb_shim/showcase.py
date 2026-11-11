"""showcase.py CASES ACTUAL EXPECTED OP [SKIP] -- decode the first differing case of OP (after SKIP of them)."""
import sys


def unesc(s):
    out, i = [], 0
    while i < len(s):
        if s[i] == "\\" and i + 1 < len(s):
            i += 1
            out.append({"t": "\t", "n": "\n", "r": "\r"}.get(s[i], s[i]))
        else:
            out.append(s[i])
        i += 1
    return "".join(out)


def arg(f):
    if f == "N":
        return None
    if f[0] == "S":
        return unesc(f[1:])
    body = f[1:]
    if not body:
        return []
    return [None if it == "N" else unesc(it[1:]) for it in body.split("\x1f")]


cases = open(sys.argv[1], "rb").read().decode("latin1").replace("\r\n", "\n").split("\n")
a = open(sys.argv[2], "rb").read().decode("latin1").replace("\r\n", "\n").split("\n")
b = open(sys.argv[3], "rb").read().decode("latin1").replace("\r\n", "\n").split("\n")
op = sys.argv[4]
skip = int(sys.argv[5]) if len(sys.argv) > 5 else 0
for i, c in enumerate(cases):
    if not c.startswith(op + "\t"):
        continue
    if a[i] != b[i] and not ("error" in a[i] and "error" in b[i]):
        if skip:
            skip -= 1
            continue
        fields = [arg(f) for f in c.split("\t")[1:]]
        print("case", i + 1, "actual", a[i], "expected", b[i])
        for f in fields:
            if isinstance(f, list):
                for k, l in enumerate(f):
                    print("   %2d| %s" % (k, l))
            else:
                print("  arg:", repr(f))
        break
