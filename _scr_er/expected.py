"""expected.py: runs the ORIGINAL codebreaker.exact.readable on many inputs; writes cases.txt (for test.cpp)
and expected.jsonl.  Strings in cases.txt are hex of UTF-8 ('-' = empty)."""
import io, json, os, random, re, shutil, sys, contextlib
sys.path.insert(0, r"C:/Users/mrtit/Documents/GitHub/CodeBreaker")
from codebreaker.exact import readable, ledger, project as projmod

HERE = os.path.dirname(os.path.abspath(__file__))
SCEN = os.path.join(HERE, "scen")
shutil.rmtree(SCEN, ignore_errors=True)
os.makedirs(SCEN)


def hx(s):
    b = s.encode("utf-8")
    return b.hex() if b else "-"


def js(o):
    return json.dumps(o, ensure_ascii=True, separators=(",", ":"))


cases, outs = [], []


def call(f):
    try:
        return f()
    except Exception as e:
        return {"exc": type(e).__name__}


def do_rename(text, names):
    cases.append("R %s %d %s" % (hx(text), len(names), " ".join("%s %d %s" % (k, a, hx(n)) for (k, a), n in names.items())))
    outs.append(js(call(lambda: readable.rename(text, names))))


def do_mapping(syms):
    cases.append("M %d %s" % (len(syms), " ".join("%s %s %d" % (hx(n), t, a) for n, (t, a) in syms.items())))

    def f():
        m = readable.mapping(type("P", (), {"symbols": lambda self: syms})())
        return [[k, a, n] for (k, a), n in m.items()]
    outs.append(js(call(f)))


class Fake:
    def __init__(self, d, override=None):
        self.dir = d
        self.src = (override or {}).get("src", os.path.join(d, "src"))

    def symbols(self):
        s = {}
        p = os.path.join(self.src, "SYMBOLS")
        if os.path.exists(p):
            for l in open(p):
                c = l.split()
                if l.startswith("#") or len(c) < 2:
                    continue
                if len(c) >= 3 and c[1] in ("C", "D", "S"):
                    s[c[0]] = (c[1], int(c[2], 16))
                else:
                    s[c[0]] = ("?", int(c[1], 16))
        return s

    def sources(self):
        out = []
        for f in sorted(os.listdir(self.src)):
            m = re.fullmatch(r"f([0-9a-f]{5})\.c", f)
            if m:
                out.append((int(m.group(1), 16), os.path.join(self.src, f)))
        return out


def fake_run(prj):
    rows = []
    for at, p in prj.sources():
        t = open(p, encoding="latin1", newline="").read()
        rows.append((os.path.basename(p), "fn%05x" % at, at, 10, "differs" if "BREAK" in t else "identical", [], []))
    return rows


readable.ledger.run = fake_run
readable.projmod.Project = lambda d, override=None: Fake(d, override)

nscen = [0]


def do_main(symlines, files, argv):
    n = nscen[0]
    nscen[0] += 1
    dirs = []
    for who in ("py", "cpp"):
        d = os.path.join(SCEN, "%s_%d" % (who, n))
        os.makedirs(os.path.join(d, "src"))
        if symlines is not None:
            open(os.path.join(d, "src", "SYMBOLS"), "w", newline="").write("\n".join(symlines) + "\n")
        for fn, t in files.items():
            open(os.path.join(d, "src", fn), "wb").write(t.encode("latin1"))
        dirs.append(d)
    cases.append("P %s %d %s" % (hx(dirs[1].replace("\\", "/")), len(argv), " ".join(hx(a) for a in argv)))
    buf = io.StringIO()

    def f():
        with contextlib.redirect_stdout(buf):
            return readable.main(Fake(dirs[0]), argv)
    rc = call(f)
    after = {fn: open(os.path.join(dirs[0], "src", fn), "rb").read().decode("latin1") for fn in sorted(os.listdir(os.path.join(dirs[0], "src")))}
    outs.append(js({"rc": rc, "out": buf.getvalue(), "files": after}))


# ---- rename
N = {("f", 0x1234a): "Init", ("f", 0x2): "x", ("g", 0x10): "counter", ("g", 0xabc): "tbl", ("f", 0x00b00): "Other"}
for t in [
    "", "int f1234a(void);", "call f1234a(); f1234a();", "/* f1234a */ f1234a", "\"f1234a\" f1234a '\\'' f1234a",
    "_f1234a g_10 g_0010 g_00010 f1234ab xf1234a f1234a_", "int Init; f1234a();", "g_abc g_ABC f00b00",
    "/* unterminated f1234a", "\"unterminated f1234a\nf1234a", "'a' g_10 'b", "caf\xe9f1234a f1234a \xe9",
    "f1234a\r\nf1234a\r\n/* c\nf1234a */\r\nf1234a", "f00b00 Other", "f00b00 x f00002", "\"a\\\"f1234a\" f1234a",
    "g_10 counter g_abc", "tbl g_abc", "\u0141 f1234a \u4e2d",
]:
    do_rename(t, N)
    do_rename(t, {})
do_rename("f1234a f1234b", {("f", 0x1234a): "A", ("f", 0x1234b): "A"})
do_rename("f1234a f1234a", {("f", 0x1234a): "A"})
do_rename("A f1234a f1234b", {("f", 0x1234a): "A", ("f", 0x1234b): "B"})
do_rename("f1234a", {("g", 0x1234a): "A"})

rng = random.Random(1106)
pool = ["f1234a", "_f1234a", "g_10", "g_abc", "f00b00", "Init", "counter", "Other", "x", "tbl", "/*", "*/", "\"", "'", "\\", "\n", " ", "(", ")", ";", "int", "f1234", "g_", "\xe9", "f0000a", "g_1", "\r\n", "//", "caf\xe9"]
for i in range(300):
    t = "".join(rng.choice(pool) + rng.choice(["", " ", " "]) for _ in range(rng.randint(0, 25)))
    nm = {}
    for k in rng.sample(list(N), rng.randint(0, len(N))):
        nm[k] = rng.choice([N[k], "Init", "counter", "zz", "x"])
    do_rename(t, nm)

# ---- mapping
do_mapping({})
do_mapping({"_main": ("C", 0x100), "Foo": ("C", 0x200), "bar": ("D", 0x10), "baz": ("?", 0x20), "big": ("D", 0x10000),
            "bigq": ("?", 0x10000), "s": ("S", 5), "a:b": ("C", 1), "a@4": ("C", 2), "f1234a": ("C", 3), "_f1234a": ("C", 4),
            "g_10": ("D", 5), "_g_10": ("D", 6), "1bad": ("C", 7), "_": ("C", 8), "__x": ("C", 9), "Foo2": ("C", 0x200),
            "Dup": ("C", 0x300), "Dup2": ("C", 0x300), "q": ("?", 0xffff), "caf\xe9": ("C", 0x400), "": ("C", 1), "g_": ("C", 11),
            "f12345": ("D", 0x10), "_f1234": ("?", 0x41), "x y": ("C", 0x42), "\u00e9t": ("C", 0x43)})
for i in range(60):
    syms = {}
    for j in range(rng.randint(0, 12)):
        nm = rng.choice(["a", "_a", "__b", "f%05x" % rng.randint(0, 0xfffff), "_f%05x" % rng.randint(0, 0xfffff), "g_%x" % rng.randint(0, 0xffff),
                         "n%d" % rng.randint(0, 5), "x:%d" % j, "y@%d" % j, "9z", "ok_name"])
        syms[nm] = (rng.choice("CDS?"), rng.choice([0, 1, 0xffff, 0x10000, 0x12345, rng.randint(0, 0x2ffff)]))
    do_mapping(syms)


# ---- counts (the ledger made to give exactly these rows)
def do_counts(rows):
    cases.append("C %d %s" % (len(rows), " ".join("%s %s %s" % (hx(a), hx(b), hx(c)) for a, b, c in rows)))
    old = readable.ledger.run
    readable.ledger.run = lambda prj: [(a, b, 0, 1, c, [], []) for a, b, c in rows]
    try:
        r = call(lambda: [[a, b, c] for (a, b), c in readable.counts(None).items()])
    finally:
        readable.ledger.run = old
    outs.append(js(r))


do_counts([])
do_counts([("a.c", "f", "identical"), ("a.c", "g", "differs"), ("b.c", "f", "identical")])
do_counts([("a.c", "f", "identical"), ("a.c", "f", "differs")])
do_counts([("\u00e9.c", "f", "no place")])

# ---- main
sy = ["# name type address", "Init C 1234a", "counter D 10", "_Pub ? 00b00", "bad:name C 5", "x y"]
f1 = "int f1234a(void) { g_10 = 1; /* f1234a */ return f00b00(); }\r\nchar *s = \"f1234a\";\r\n"
f2 = "no names here\n"
f3 = "int f00b00() { return Init; } /* caf\xe9 */ \xe9 f1234a\n"
f4 = "BREAK f1234a\n"
do_main(None, {"f1234a.c": f1}, [])
do_main(["# nothing", "bad:name C 5"], {"f1234a.c": f1}, [])
do_main(sy, {"f1234a.c": f1, "f00b00.c": f2}, [])
do_main(sy, {"f1234a.c": f1, "f00b00.c": f2}, ["--write"])
do_main(sy, {"f00b00.c": f2}, ["--write"])
do_main(sy, {"f1234a.c": f1, "f00b00.c": f3, "f00001.c": f2}, ["--write", "x"])
do_main(sy, {"f1234a.c": f1, "f00b00.c": f3, "f00001.c": f2}, ["write"])
do_main(sy, {"f1234a.c": f4}, ["--write"])
do_main(sy, {"f1234a.c": f1, "f00b00.c": "int Init; f1234a Init\n"}, ["--write"])
do_main(["Init C 1234a", "BREAK C 00b00"], {"f1234a.c": f1, "f00b00.c": "f00b00 f1234a\n"}, ["--write"])
do_main(["Init C 1234a", "xBREAKx C 00b00"], {"f1234a.c": f1, "f00b00.c": "f00b00\n"}, ["--write"])
do_main(["Init C 1234a"], {"f1234a.c": "int f1234a;\n", "other.txt": "f1234a"}, ["--write"])

open(os.path.join(HERE, "cases.txt"), "w", newline="\n").write("\n".join(cases) + "\n")
open(os.path.join(HERE, "expected.jsonl"), "w", newline="\n").write("\n".join(outs) + "\n")
print(len(cases), "cases")
