"""expected.py -- the ORIGINAL codebreaker.compare.versions on real pairs of builds (Windows 3.1 / 95 / 98 modules).

Builds seed/ (a function store with reconstructions of an old build's functions), runs the original on a copy (state/), and
writes cases.txt (op, label, arguments: the same calls), knowledge.txt (what reconstruct.identity answered), capstone.txt (the
capstone DLL's path) and expected.txt.   Run from this folder:  python expected.py
The original's by_likeness is the corrected one (determinism, see _bugs/LIST.md): CB_VERSIONS_PY names the versions.py to use
until the fix is merged where the checkout is (default: the checkout's own).
"""
import contextlib
import functools
import hashlib
import importlib.util
import io
import json
import os
import re
import shutil
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOTDIR = "C:/Users/mrtit/Documents/GitHub/CodeBreaker"
sys.path.insert(0, ROOTDIR)
SEED = os.path.join(HERE, "seed")
STATE = os.path.join(HERE, "state")
INPUTS = os.path.join(HERE, "inputs")
for d in (SEED, STATE, INPUTS):
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)
os.environ["CB_STATE"] = SEED
os.environ["CB_FUNCTIONS_DB"] = os.path.join(SEED, "functions.sqlite")

import codebreaker.compare                                   # noqa: E402,F401
if os.environ.get("CB_VERSIONS_PY"):
    spec = importlib.util.spec_from_file_location("codebreaker.compare.versions", os.environ["CB_VERSIONS_PY"])
    mod = importlib.util.module_from_spec(spec)
    sys.modules["codebreaker.compare.versions"] = mod
    spec.loader.exec_module(mod)
    codebreaker.compare.versions = mod
import capstone                                              # noqa: E402
from codebreaker.base import corpus                          # noqa: E402
from codebreaker.compare import versions                     # noqa: E402
from codebreaker.store import functions as F                 # noqa: E402
from codebreaker.verify import reconstruct                   # noqa: E402

open(os.path.join(HERE, "capstone.txt"), "w").write(os.path.join(os.path.dirname(capstone.__file__), "lib", "capstone.dll"))

# ---- the modules
mods = {}
for n in ("win3.1", "win95", "win98"):
    with corpus.selected(n):
        mods[n] = {k: {os.path.basename(p).lower(): p for p in corpus.modules(k)} for k in ("pe", "ne")}


def pairs_of(kind, a, b, count, lo, hi):
    common = sorted(set(mods[a][kind]) & set(mods[b][kind]), key=lambda n: (os.path.getsize(mods[a][kind][n]), n))
    common = [n for n in common if lo <= os.path.getsize(mods[a][kind][n]) <= hi and lo <= os.path.getsize(mods[b][kind][n]) <= hi * 2]
    step = max(1, len(common) // count)
    return [(mods[a][kind][n], mods[b][kind][n]) for n in common[::step][:count]]


PE = pairs_of("pe", "win95", "win98", 7, 9000, 30000)
NE = pairs_of("ne", "win95", "win98", 6, 5000, 20000) + pairs_of("ne", "win3.1", "win95", 4, 10000, 40000)
SAME = [(PE[0][0], PE[0][0]), (NE[0][1], NE[0][1])]                                  # a build against itself
CROSS = [(PE[0][0], PE[1][1]), (NE[0][0], NE[3][1])]                                 # two different modules
dos_files = []
for k in ("dos",):
    with corpus.selected("win95"):
        dos_files = sorted(corpus.modules("dos", exts=(".exe", ".com", ".sys")), key=lambda p: (os.path.getsize(p), p))
dos_files = [p for p in dos_files if 2000 < os.path.getsize(p) < 60000][:6]
DOS = [(dos_files[0], dos_files[0]), (dos_files[1], dos_files[2]), (dos_files[3], dos_files[3])]
BAD = os.path.join(INPUTS, "text.txt")
open(BAD, "w").write("not a program")
NOFILE = os.path.join(INPUTS, "no_such.exe")

# ---- make the store: reconstructions of the old build of one pair
os.environ["CB_STATE"] = SEED
CARRY_PAIR = None
for a, b in PE + NE:
    c = versions.compare(a, b)
    ident = [p for p in c.pairs if p["status"] == "identical"]
    if len(ident) >= 3:
        CARRY_PAIR = (a, b)
        break
assert CARRY_PAIR, "no pair with identical functions"
fs = F.FunctionStore()
a, b = CARRY_PAIR
c = versions.compare(a, b)
sha_a, digest_a = reconstruct.identity(a)
sha_b, digest_b = reconstruct.identity(b)
fs.add_module(sha_a, os.path.basename(a).lower(), a, c.a.kind)
fs.add_profile(digest_a, "{}")
fs.add_functions(sha_a, list(c.a.order))
rows = []
for i, p in enumerate(c.pairs):
    if p["status"] == "identical":
        v, lim = [("equivalent", ""), ("equivalent", "low-coverage"), ("divergent", ""), ("consistent", "")][i % 4]
        rows.append(dict(addr=p["from"], sig="s%d" % i, name="f_%d" % i, source="int f_%d(void) { return %d; }" % (i, i) if i % 5 else None, verdict=v, limits=lim, reason=""))
fs.record_many(sha_a, "decompiler", "1", digest_a, rows)
fs.add_module(sha_b, os.path.basename(b).lower(), b, c.b.kind)
fs.add_functions(sha_b, list(c.b.order))
fs.set_bodies(sha_b, {p["to"]: ("body%d" % i, None, None) for i, p in enumerate([p for p in c.pairs if p["status"] == "identical"][:3])})
fs.close()
shutil.copytree(SEED, STATE, dirs_exist_ok=True)
os.environ["CB_STATE"] = STATE
os.environ["CB_FUNCTIONS_DB"] = os.path.join(STATE, "functions.sqlite")

# ---- identity is recorded as it is asked
KNOW_I = {}
_identity = reconstruct.identity


def recording_identity(path):
    try:
        r = _identity(path)
    except Exception as e:                                   # noqa: BLE001
        KNOW_I[path] = ("ERR", type(e).__name__, str(e).replace("\n", " ").replace("\t", " "))
        raise
    KNOW_I[path] = ("OK", r[0], r[1])
    return r


reconstruct.identity = recording_identity

# ---- the 16-bit function starts, recorded as asked (the order the original's dict gave them)
from codebreaker.decompile import describe as dec16          # noqa: E402

KNOW_D = {}
_functions = dec16.functions


def recording_functions(nef):
    r = _functions(nef)
    KNOW_D[nef.path] = [(seg, off, name) for (seg, off), name in r.items()]
    return r


dec16.functions = recording_functions
for p in (a, b):
    recording_identity(p)
time.time = lambda: 100.0

TIMES = {}


def _timed(name, fn):
    @functools.wraps(fn)
    def run(*a_, **k):
        t = time.perf_counter()
        try:
            return fn(*a_, **k)
        finally:
            TIMES[name] = TIMES.get(name, 0.0) + time.perf_counter() - t
    return run


for _n in ("load", "match", "compare", "shared", "product", "main"):
    setattr(versions, _n, _timed(_n, getattr(versions, _n)))

cases = []
out = open(os.path.join(HERE, "expected.txt"), "w", encoding="utf-8", newline="\n")


def rec(op, label, args, text):
    cases.append("\t".join([op, label] + [str(x) for x in args]))
    out.write("== %s\n%s\n" % (label, text))


def jd(v):
    return json.dumps(v, sort_keys=True)


def sha256(path):
    return hashlib.sha256(open(path, "rb").read()).hexdigest() if os.path.isfile(path) else "(no file)"


def short(p):
    return os.path.relpath(p, os.path.dirname(os.path.dirname(HERE))) if False else os.path.basename(p)


def side_dict(s):
    return {"kind": s.kind, "name": s.name, "code": s.code, "order": s.order,
            "callers": {k: sorted(v) for k, v in s.callers.items()},
            "functions": {k: {"key": f.key, "name": f.name, "named": f.named, "at": f.at, "size": f.size, "rows": [[a_, t] for a_, t in f.rows], "text": f.text,
                              "shape": f.shape, "ids": f.ids, "calls": [list(c_) for c_ in f.calls], "marks": sorted(f.marks), "jumps": f.jumps,
                              "count": f.count} for k, f in s.functions.items()}}


def capture(argv):
    o = io.StringIO()
    code = 0
    with contextlib.redirect_stdout(o):
        try:
            code = versions.main(list(argv))
        except SystemExit as x:
            code = "SystemExit(%s)" % x.code
        except Exception as x:                               # noqa: BLE001
            code = "%s: %s" % (type(x).__name__, x)
    return "exit %s\n%s" % (code, o.getvalue())


# ---- load
for p in sorted({x for pr in PE + NE + SAME + CROSS + DOS for x in pr})[:40]:
    try:
        text = jd(side_dict(versions.load(p)))
    except Exception as e:                                   # noqa: BLE001
        text = "EXC %s: %s" % (type(e).__name__, e)
    rec("load", "load %s" % os.path.basename(p), [p], text)
for p in (BAD, NOFILE):
    try:
        text = jd(side_dict(versions.load(p)))
    except Exception as e:                                   # noqa: BLE001
        text = "EXC %s: %s" % (type(e).__name__, e)
    rec("load", "load %s" % os.path.basename(p), [p], text)

# ---- match / compare
ALL = PE + NE + SAME + CROSS + DOS
for a_, b_ in ALL:
    for least in (0.6, 0.85):
        sa, sb = versions.load(a_), versions.load(b_)
        m = versions.match(sa, sb, least)
        rec("match", "match %s %s %s" % (os.path.basename(a_), os.path.basename(b_), least), [a_, b_, least], jd([[k, v[0], v[1]] for k, v in m.items()]))
    c = versions.compare(a_, b_)
    rec("compare", "compare %s %s" % (os.path.basename(a_), os.path.basename(b_)), [a_, b_, 0.6, 1],
        jd(c.as_json()) + "\n" + c.report() + "\n" + jd([c.counts(), c.code(), c.names()]))
    rec("compare", "compare no listings %s %s" % (os.path.basename(a_), os.path.basename(b_)), [a_, b_, 0.6, 0],
        jd(c.as_json(listings=False)))
for a_, b_ in (PE[0], NE[0]):
    c = versions.compare(a_, b_, 0.3)
    rec("compare", "compare least 0.3 %s" % os.path.basename(a_), [a_, b_, 0.3, 1], jd(c.as_json()) + "\n" + c.report() + "\n" + jd([c.counts(), c.code(), c.names()]))
for p in (BAD, NOFILE):
    for pr in ((p, PE[0][1]), (PE[0][0], p)):
        try:
            versions.compare(*pr)
            text = "no error"
        except Exception as e:                               # noqa: BLE001
            text = "EXC %s: %s" % (type(e).__name__, e)
        rec("compare", "compare error %s" % os.path.basename(p), [pr[0], pr[1], 0.6, 1], text)

# ---- likeness
a_, b_ = PE[1]
sa, sb = versions.load(a_), versions.load(b_)
keys_a, keys_b = list(sa.functions)[:8], list(sb.functions)[:8]
for ka in keys_a[:5]:
    for kb in keys_b[:5]:
        rec("likeness", "likeness %s %s %s %s" % (os.path.basename(a_), os.path.basename(b_), ka, kb), [a_, b_, ka, kb],
            repr(versions.likeness(sa.functions[ka], sb.functions[kb])))

# ---- shared
for paths, level, least in (([PE[0][0], PE[0][1]], "text", 8), ([PE[0][0], PE[0][1], PE[1][0], PE[1][1]], "shape", 8), ([PE[2][0], PE[2][1]], "bytes", 3),
                            ([NE[0][0], NE[0][1], NE[1][0]], "text", 4), ([DOS[0][0], DOS[1][0], DOS[1][1]], "shape", 5), ([PE[0][0]], "text", 8)):
    rec("shared", "shared %s %s" % (level, least), [level, least, ";".join(paths)], jd(versions.shared(paths, level, least)))

# ---- carried names and reconstructions
fs = F.open()
c = versions.compare(a, b)
rec("carry", "reconstructions", [a, b], jd(c.reconstructions(fs)))
rec("carry", "carry_names", [a, b], jd(c.carry_names(fs)))
rec("carry", "carry", [a, b], jd(c.carry(fs)))
rec("carry", "store after", [a, b], jd({"best": {k: [v.get("class"), v.get("engine"), v.get("name"), v.get("source"), v.get("verdict"), v.get("reason")] for k, v in fs.best(sha_b, digest_b).items()},
                                        "names": {k: [x for x in v] for k, v in fs.names_from_bodies(sha_b).items()}}))
rec("carry", "carry again", [a, b], jd(c.carry(fs)))
rec("carry", "carry_names again", [a, b], jd(c.carry_names(fs)))
fs.close()
c2 = versions.compare(PE[2][0], PE[2][1])
rec("carry", "no store results", [PE[2][0], PE[2][1]], jd([c2.reconstructions(), c2.carry_names(), c2.carry()]))

# ---- products
for sample, jobs in ((3, 1), (5, 1), (7, 1)):
    logs = []
    r = versions.product("win95", "win98", sample, jobs, 0.6, logs.append)
    rec("product", "product win95 win98 %d %d" % (sample, jobs), [sample, jobs, 0.6], jd([r, logs]))
logs = []
r = versions.product("win95", "win98", 5, 1, 0.6, logs.append)
rec("product", "product2 win95 win98 5", [5, 2, 0.6], jd([r, logs]))
logs = []
r = versions.product("win3.1", "win95", 4, 1, 0.6, logs.append)
rec("product", "product win3.1 win95 4 1", [4, 1, 0.6], jd([r, logs]))

# ---- the command
JSON1, JSON2, JSON3, JSON4 = [os.path.join(INPUTS, "out%d.json" % i) for i in range(1, 5)]
mains = [[], [PE[0][0], PE[0][1]], [PE[0][0], PE[0][1], "--json", JSON1], [PE[0][0], PE[0][1], "--least", "0.3"], [os.path.basename(PE[0][0]), "--from", "win95", "--to", "win98"],
         [os.path.basename(NE[0][0]), "--from", "win95", "--to", "win98", "--json", JSON2], ["nosuchmodule.dll", "--from", "win95", "--to", "win98"],
         [PE[0][0], PE[0][1], "--carry"], [a, b, "--carry"],
         ["--shared", PE[0][0], PE[0][1], "--json", JSON3], ["--shared", PE[0][0], PE[0][1], PE[1][0], "--level", "shape"], ["--shared", PE[0][0]], ["--shared", PE[0][0], PE[0][1], "--level", "bogus"],
         ["--all", "--from", "win95", "--to", "win98", "--sample", "3", "--json", JSON4], ["--all", "--from", "win3.1", "--to", "win95", "--sample", "4", "--jobs", "1"],
         [PE[0][0], PE[0][1], "--from"], [BAD, PE[0][1]], ["--from", "win95"], [PE[0][0], PE[0][1], "--sample", "x"]]
for argv in mains:
    rec("main", "main %s" % " ".join(os.path.basename(x) if os.sep in x else x for x in argv), argv, capture(argv))
for j in (JSON1, JSON2, JSON3, JSON4):
    rec("file", "file %s" % os.path.basename(j), [j], sha256(j))
out.close()
open(os.path.join(HERE, "cases.txt"), "w", encoding="utf-8", newline="\n").write("\n".join(cases) + "\n")
with open(os.path.join(HERE, "knowledge.txt"), "w", encoding="utf-8", newline="\n") as f:
    for path, v in KNOW_I.items():
        f.write("\t".join(["I", path] + list(v)) + "\n")
    for path, rows in KNOW_D.items():
        for seg, off, name in rows:
            f.write("\t".join(["D", path, str(seg), str(off), str(name).replace("\t", " ").replace("\n", " ")]) + "\n")
print("%d cases" % len(cases))
sys.stderr.write("python seconds by call: " + ", ".join("%s %.2f" % kv for kv in sorted(TIMES.items())) + "\n")
