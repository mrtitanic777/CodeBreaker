"""gen_units.py <outdir> -- cases for strings.conversions and strings._assemble from the live Python and DLL."""
import json
import os
import random
import sys

sys.path.insert(0, ".")
from codebreaker.analyze import strings  # noqa: E402

out = sys.argv[1]
os.makedirs(out, exist_ok=True)

formats = ["", "%", "%%", "%d", "%5.2f", "%-8s|", "%*d", "%*.*s", "%.*f", "%ld %lu %lld %I64d %I64u %hd %hu %hhd",
           "%s %S %ls %ws %hs", "%c %C", "%p %n", "%e %E %g %G", "%x %X %o %i %u", "%08lx", "%#x", "%+d % d", "%.f",
           "%5.d", "%L", "abc%", "%z", "%I32d", "%hhx", "%5%", "100%% done %d", "%1$d", "File %s, line %d: %s",
           "%s = %ld", "Error %lu in %hs (%ws)", "%-*.*lf%%", "%0*d", "% 5.3s", "%.10hs", "%hc %lc %wc", "%lf %Lf",
           "%.0f%s", "%lld%%", "%I64x", "%hhhd", "%lls", "%hS %lS", "x%%%d%%y", "%%%%", "%*%", "%-+ 0#5d", "%.*%d"]
cases = []
for f in formats:
    for wide in (False, True):
        cases.append({"format": f, "wide": wide, "types": strings.conversions(f, wide)})
with open(os.path.join(out, "conversions.json"), "w") as fh:
    json.dump(cases, fh, indent=0)

rnd = random.Random(20261111)
words = ["Hello", "Error", "kernel32", "%s", "OK", "ab", "A", "File:", "xYz", "Wait", "1234", "a1", "..", "Quit now"]


def random_stores():
    stores = {}
    n = 0
    for _ in range(rnd.randint(1, 5)):
        base = rnd.choice(["ebp", "esp"])
        off = rnd.randint(-0x80, 0x40)
        for _ in range(rnd.randint(1, 3)):
            kind = rnd.random()
            word = rnd.choice(words)
            if kind < 0.25:                         # 16-bit characters
                data = b"".join(bytes([ord(c), 0]) for c in word) + b"\x00\x00"
            elif kind < 0.9:
                data = word.encode() + (b"\x00" if rnd.random() < 0.7 else b"")
            else:
                data = bytes(rnd.randrange(256) for _ in range(rnd.randint(2, 8)))
            i = 0
            while i < len(data):
                width = rnd.choice([1, 2, 4])           # how the program stored them: dword, word, byte stores
                n += 1
                for k in range(min(width, len(data) - i)):
                    stores[(base, off + i + k)] = (data[i + k], n)
                i += width
            off += len(data) + rnd.choice([0, 0, 1, 3])
    return stores


cases = []
for _ in range(300):
    stores = random_stores()
    least = rnd.choice([4, 4, 4, 3, 5, 2])
    got = strings._assemble(stores, least)
    cases.append({"stores": [[b, o, v, n] for (b, o), (v, n) in stores.items()], "least": least,
                  "pieces": [[b, o, t, w] for b, o, t, w in got]})
with open(os.path.join(out, "assemble.json"), "w") as fh:
    json.dump(cases, fh, separators=(",", ":"))
print("conversions", len(formats) * 2, "assemble", len(cases), "with pieces", sum(1 for c in cases if c["pieces"]))
