"""expected.py -- the ORIGINAL codebreaker.ui.progress over function stores built from real corpus modules.

Builds seed/ (two stores: st_a, results arriving now; st_b, results from ten days ago; each with the corpus size memo),
copies it to work/ and runs the original there with the clock fixed (time.time() = NOW).  Writes cases.txt (op, label,
arguments: the same calls), digests.txt (what reconstruct.digests() answers: test.cpp's stand-in for the lanes) and
expected.txt.   Run from this folder:  python expected.py
"""
import contextlib
import functools
import io
import json
import os
import re
import shutil
import sqlite3
import sys
import tempfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOTDIR = "C:/Users/mrtit/Documents/GitHub/CodeBreaker"
sys.path.insert(0, ROOTDIR)
SEED = os.path.join(HERE, "seed")
WORK = os.path.join(HERE, "work")
INPUTS = os.path.join(HERE, "inputs")
for d in (SEED, WORK, INPUTS):
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)
NOW = 1800000000.0
TMP_WORKERS = os.path.join(INPUTS, "tmp_workers")
TMP_NONE = os.path.join(INPUTS, "tmp_none")


def use(state, tmp):
    os.environ["CB_STATE"] = state
    os.environ["CB_FUNCTIONS_DB"] = os.path.join(state, "functions.sqlite")
    os.environ["TEMP"] = os.environ["TMP"] = os.environ["TMPDIR"] = tmp
    tempfile.tempdir = None


use(os.path.join(SEED, "st_a"), TMP_NONE)
os.makedirs(TMP_NONE)
os.makedirs(os.path.join(SEED, "st_a"))
os.makedirs(os.path.join(SEED, "st_b"))

from codebreaker.base import corpus                       # noqa: E402
from codebreaker.decompile import describe                # noqa: E402
from codebreaker.formats import pe_functions              # noqa: E402
from codebreaker.formats.ne import NEFile                 # noqa: E402
from codebreaker.store import functions as F              # noqa: E402
from codebreaker.ui import progress                       # noqa: E402
from codebreaker.verify import reconstruct                # noqa: E402

EXTS = (".dll", ".exe", ".drv", ".ocx", ".cpl")
for st in ("st_a", "st_b"):
    shutil.copy(os.path.join(ROOTDIR, "state", "coverage_corpus.json"), os.path.join(SEED, st, "coverage_corpus.json"))


def pick(kind, n):
    mods = sorted(corpus.modules(kind, exts=EXTS), key=lambda p: (os.path.getsize(p), p))
    mods = [m for m in mods if 8000 < os.path.getsize(m) < 150000]
    return [mods[i * (len(mods) - 1) // (n - 1)] for i in range(n)]


DIGESTS = reconstruct.digests()
CYCLE = [("equivalent", ""), ("equivalent", "low-coverage"), ("divergent", ""), ("inconclusive", ""), ("no-compile", ""), ("equivalent", ""),
         ("consistent", ""), ("divergent", "")]


def addresses(path, kind, n=40):
    if kind == "pe":
        return ["%x" % va for va in sorted(pe_functions.function_spans(path))][:n]
    return ["%d:%04x" % so for so in sorted(describe.functions(NEFile(path)))][:n]


pes, nes = pick("pe", 3), pick("ne", 2)
plan = [(pes[0], "pe", 0), (pes[1], "pe", 3), (nes[0], "ne", 0), (pes[2], "pe", 5)]
for st, base_last, age in (("st_a", NOW, 0), ("st_b", NOW - 864000, 864000)):
    use(os.path.join(SEED, st), TMP_NONE)
    fs = F.FunctionStore()
    meta = []
    for k, (path, kind, skip) in enumerate(plan):
        sha = corpus.sha(path)
        fs.add_module(sha, os.path.basename(path).lower(), path, kind)
        fs.add_profile(DIGESTS[kind], "{}")
        addrs = addresses(path, kind)
        fs.add_functions(sha, addrs)
        rows = []
        for i, a in enumerate(addrs):
            if skip and i % skip == 0:
                continue
            v, limits = CYCLE[i % len(CYCLE)]
            rows.append(dict(addr=a, sig="s%d" % i, name="f_%d" % i, source="int f(void){return %d;}" % i, verdict=v, limits=limits,
                             reason="r" if v == "divergent" else ""))
        fs.record_many(sha, "decompiler", "1", DIGESTS[kind], rows)
        meta.append((sha, fs.database_path(sha)))
    fs.close()
    # the times: the index's `last` per module, and every verdict's ts (a third of them within the last ten minutes in st_a)
    idx = sqlite3.connect(os.path.join(SEED, st, "functions.sqlite"))
    for k, (sha, _db) in enumerate(meta):
        idx.execute("UPDATE summary SET last=? WHERE module=?", (base_last - (10 if k == 2 else 40 * k + 5) - (2000 if k == 1 else 0), sha))
    idx.commit()
    idx.close()
    for k, (sha, db) in enumerate(meta):
        c = sqlite3.connect(db)
        for n, (rowid,) in enumerate(c.execute("SELECT rowid FROM verdict ORDER BY rowid").fetchall()):
            ts = NOW - age - (30 + 17 * n if n % 3 == 0 and k != 3 else 4000 + n)
            c.execute("UPDATE verdict SET ts=? WHERE rowid=?", (ts, rowid))
        c.commit()
        c.close()
shutil.copytree(SEED, WORK, dirs_exist_ok=True)

# the job runner's notes in a temporary folder
for sub, name, content, age in (("cbjobs1/w1", "a.cur", "x", 100), ("cbjobs1/w1", "b.cur", "", 100), ("cbjobs1/w2", "c.cur", "y", 5000),
                                ("cbjobs1/w2", "d.cur", "y", 3599 - 0), ("CBJOBS7/w3", "e.CUR", "z", 10), ("cbjobs1/.hid", "f.cur", "z", 1),
                                ("cbjobs1/w1", ".g.cur", "z", 1), ("other/w9", "h.cur", "z", 1), ("cbjobs2/w4", "i.txt", "z", 1)):
    p = os.path.join(TMP_WORKERS, sub)
    os.makedirs(p, exist_ok=True)
    f = os.path.join(p, name)
    open(f, "w").write(content)
    os.utime(f, (NOW - age, NOW - age))
open(os.path.join(TMP_WORKERS, "cbjobs3"), "w").write("a file, not a folder")
for d in (os.path.join(INPUTS, "log_empty.txt"), ):
    open(d, "w").close()
logs = {
    "log_empty.txt": b"",
    "log_blank.txt": b"\n\n   \n",
    "log_one.txt": b"only line",
    "log_many.txt": b"first\nsecond\n  third line with spaces   \n\n\n",
    "log_cr.txt": b"a\rb\rlast\r",
    "log_long.txt": b"head\n" + b"x" * 6000 + b"\nthe tail line\n",
    "log_bytes.txt": b"ok\n\xff\xfe broken \x80 bytes\n",
    "log_one_long.txt": b"y" * 5000,
    "log_unicode.txt": "caf\u00e9 \u2603 done\n".encode("utf-8"),
    "log_u2028.txt": "one\u2028two\u2029three".encode("utf-8"),
}
for name, data in logs.items():
    open(os.path.join(INPUTS, name), "wb").write(data)

time.time = lambda: NOW
TIMES = {}


def _timed(name, fn):
    @functools.wraps(fn)
    def run(*a, **k):
        t = time.perf_counter()
        try:
            return fn(*a, **k)
        finally:
            TIMES[name] = TIMES.get(name, 0.0) + time.perf_counter() - t
    return run


for _n in ("snapshot", "line", "_busy_workers", "_last_line", "main"):
    setattr(progress, _n, _timed(_n, getattr(progress, _n)))
with open(os.path.join(HERE, "digests.txt"), "w", encoding="utf-8", newline="\n") as f:
    for k, v in DIGESTS.items():
        f.write("%s\t%s\n" % (k, v if v is not None else "None"))

cases = []
out = open(os.path.join(HERE, "expected.txt"), "w", encoding="utf-8", newline="\n")


def rec(op, label, args, text):
    cases.append("\t".join([op, label] + [str(a) for a in args]))
    out.write("== %s\n%s\n" % (label, text))


def jd(v):
    return json.dumps(v, sort_keys=True)


def maskt(text):
    return re.sub(r"^\[\d\d:\d\d:\d\d\]", "[T]", text, flags=re.M)


def capture(argv):
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        code = progress.main(list(argv))
    return "exit %d\n%s" % (code, maskt(buf.getvalue()))


def setup(state, tmp, label):
    s, t = (os.path.join(WORK, state) if state != "none" else os.path.join(WORK, "no_store")), (TMP_WORKERS if tmp == "workers" else TMP_NONE)
    use(s, t)
    rec("use", label, [state, tmp], "")


LOG = os.path.join(INPUTS, "log_many.txt")
snaps = {}
for state in ("st_a", "st_b", "none"):
    for tmp in ("none", "workers"):
        setup(state, tmp, "use %s %s" % (state, tmp))
        for log in ("", LOG):
            s = progress.snapshot(log or None)
            snaps[(state, tmp, log)] = s
            shown = dict(s)
            shown.pop("time")
            rec("snapshot", "snapshot %s %s %s" % (state, tmp, os.path.basename(log) if log else "-"), [log], jd(shown))
            s["time"] = "T"
            rec("line", "line %s %s %s" % (state, tmp, os.path.basename(log) if log else "-"), [log], progress.line(s))
        setup(state, tmp, "use %s %s" % (state, tmp))
# the line of a made-up reading
base = dict(snaps[("st_a", "workers", LOG)])
base["time"] = "T"
variants = [["workers=0"], ["workers=2", "eta=3700"], ["eta=59"], ["eta=0"], ["eta=3600"], ["eta=None"], ["rate=0.0", "active=0"], ["rate=0.0", "active=1"],
            ["rate=12.5"], ["rate=0.1"], ["log="], ["log=" + "z" * 100], ["recent=0"], ["kinds=0"], ["kinds=0", "recent=0", "log=", "workers=0", "eta=None", "rate=0.0", "active=0"],
            ["idle=None"], ["classes=0"]]
for v in variants:
    s = json.loads(json.dumps(base))
    for kv in v:
        k, val = kv.split("=", 1)
        if k == "workers":
            s["workers"] = int(val)
        elif k == "eta":
            s["eta"] = None if val == "None" else int(val)
        elif k == "rate":
            s["rate"] = float(val)
        elif k == "active":
            s["active"] = bool(int(val))
        elif k == "log":
            s["log"] = val
        elif k == "recent":
            s["recent"] = []
        elif k == "kinds":
            s["kinds"] = []
        elif k == "idle":
            s["idle"] = None
        elif k == "classes":
            for kk in s["kinds"]:
                for c in F.ORDER:
                    kk[c] = 0
    rec("line_variant", "line_variant %s" % " ".join(v), v, progress.line(s))
# the pieces
for name in list(logs) + ["no_such_log.txt"]:
    p = os.path.join(INPUTS, name)
    rec("last_line", "last_line %s" % name, [p], jd(progress._last_line(p)))
for t in (None, 0, 1, 59, 60, 61, 3599, 3600, 3661, 7325, 86400, 359999):
    rec("clock", "clock %s" % t, ["None" if t is None else t], progress._clock(t))
for tmp in ("none", "workers"):
    setup("st_a", tmp, "use st_a %s" % tmp)
    rec("busy", "busy %s" % tmp, [], str(progress._busy_workers()))
# the command (one reading; a loop that never ends is not run)
for state in ("st_a", "st_b", "none"):
    for tmp in ("none", "workers"):
        setup(state, tmp, "use %s %s" % (state, tmp))
        for argv in (["--once"], ["--once", "--log", LOG], ["--once", "--interval", "0.2"], ["--once", "--log", os.path.join(INPUTS, "no_such_log.txt")]):
            rec("main", "main %s %s %s" % (state, tmp, " ".join(a if not os.sep in a else os.path.basename(a) for a in argv)), argv, capture(argv))
out.close()
open(os.path.join(HERE, "cases.txt"), "w", encoding="utf-8", newline="\n").write("\n".join(cases) + "\n")
print("%d cases" % len(cases))
sys.stderr.write("python seconds by call: " + ", ".join("%s %.2f" % kv for kv in sorted(TIMES.items())) + " (total %.2f)\n" % sum(TIMES.values()))
