"""expected.py -- the ORIGINAL codebreaker.coverage.ledger over a function store built from real corpus modules.

Builds seed/ (a function store of real corpus modules with a spread of verdicts, and the corpus size memo), runs the
original on a copy of it (state/), and writes cases.txt (the same calls, tab separated: op, the label, the
arguments), digests.txt (what reconstruct.digests() answers: test.cpp's stand-in for the lanes) and expected.txt.
Run from this folder:  python expected.py
"""
import contextlib
import functools
import hashlib
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

from codebreaker.base import corpus                       # noqa: E402
from codebreaker.coverage import ledger                   # noqa: E402
from codebreaker.decompile import describe                # noqa: E402
from codebreaker.formats import pe_functions              # noqa: E402
from codebreaker.formats.ne import NEFile                 # noqa: E402
from codebreaker.store import functions as F              # noqa: E402
from codebreaker.verify import reconstruct                # noqa: E402

EXTS = (".dll", ".exe", ".drv", ".ocx", ".cpl", ".vxd", ".386", ".sys", ".com")
shutil.copy(os.path.join(ROOTDIR, "state", "coverage_corpus.json"), os.path.join(SEED, "coverage_corpus.json"))


def pick(kind, n, small=True):
    mods = sorted(corpus.modules(kind, exts=EXTS), key=lambda p: (os.path.getsize(p), p))
    if small:
        mods = [m for m in mods if 8000 < os.path.getsize(m) < 150000]
    return [mods[i * (len(mods) - 1) // (n - 1)] for i in range(n)]


DIGESTS = reconstruct.digests()
CYCLE = [("equivalent", "", ""), ("equivalent", "low-coverage", ""), ("equivalent", "fault-only,truncated", ""),
         ("consistent", "", ""), ("divergent", "", "ret-differs: 3 vs 4"), ("inconclusive", "", "judge: the emulator crashed"),
         ("no-compile", "", "gcc: error: unknown type"), ("no-source", "", ""), ("equivalent", "", ""),
         ("divergent", "", "writes-differ"), ("equivalent", "stub-callee", ""), ("divergent", "", "")]


def addresses(path, kind, n=70):
    if kind == "pe":
        return ["%x" % va for va in sorted(pe_functions.function_spans(path))][:n]
    if kind == "ne":
        return ["%d:%04x" % so for so in sorted(describe.functions(NEFile(path)))][:n]
    return ["1:%04x" % (0x10 * i) for i in range(30)]


def fill(fs, path, kind, profile, skip_every=0, bad=None, extra_engine=True):
    sha = corpus.sha(path)
    fs.add_module(sha, os.path.basename(path).lower(), path, kind)
    fs.add_profile(profile, "{}")
    addrs = addresses(path, kind)
    if bad:
        addrs = addrs + [bad]
    fs.add_functions(sha, addrs)
    rows, alt = [], []
    for i, a in enumerate(addrs):
        if skip_every and i % skip_every == 0:
            continue
        v, limits, reason = CYCLE[i % len(CYCLE)]
        src = None if v == "no-source" or i % 7 == 3 else "int f_%d(void) { return %d; }" % (i, i)
        rows.append(dict(addr=a, sig="s%d" % i, name="f_%d" % i, source=src, verdict=v, limits=limits, reason=reason))
        if extra_engine and i % 5 == 0:
            alt.append(dict(addr=a, sig="t%d" % i, name="g_%d" % i, source="int g(void) { return 1; }",
                            verdict="equivalent" if i % 10 else "divergent", limits="", reason="" if i % 10 else "alt differs"))
    fs.record_many(sha, "decompiler", "1", profile, rows)
    if alt:
        fs.record_many(sha, "alt", "2", profile, alt)
    return sha


fs = F.FunctionStore()
pes, nes, les, doses = pick("pe", 4), pick("ne", 4), pick("le", 2, small=False), pick("dos", 2, small=False)
shas = {}
shas[pes[0]] = fill(fs, pes[0], "pe", DIGESTS["pe"])                                  # measured: every function has a result
shas[pes[1]] = fill(fs, pes[1], "pe", DIGESTS["pe"], skip_every=4)                    # part measured
shas[pes[2]] = fill(fs, pes[2], "pe", DIGESTS["pe"], bad="xyz")                       # an address that is not one: no extents
shas[pes[3]] = fill(fs, pes[3], "pe", "0123456789ab")                                 # judged under another profile
shas[nes[0]] = fill(fs, nes[0], "ne", DIGESTS["ne"])
shas[nes[1]] = fill(fs, nes[1], "ne", DIGESTS["ne"], skip_every=3)
shas[nes[2]] = fill(fs, nes[2], "ne", DIGESTS["ne"], bad="1:2:3")
shas[les[0]] = fill(fs, les[0], "le", "1e1e1e1e1e1e")                                 # kind with no profile on this machine
shas[doses[0]] = fill(fs, doses[0], "dos", DIGESTS["dos"])
# a module in the store that is no module of the corpus, one whose file is gone, and a second file under a corpus name
extra = os.path.join(INPUTS, "extra_not_in_corpus.dll")
shutil.copy(pes[0], extra)
shas[extra] = fill(fs, extra, "pe", DIGESTS["pe"])
fs.add_module("ab" * 32, "ghost.dll", os.path.join(INPUTS, "ghost.dll"), "pe")
fs.add_functions("ab" * 32, ["401000", "401010"])
fs.record_many("ab" * 32, "decompiler", "1", DIGESTS["pe"], [dict(addr="401000", sig="x", name="n", source="int n(void){}", verdict="equivalent")])
fs.add_module("cd" * 32, os.path.basename(nes[0]).lower(), os.path.join(INPUTS, "older_copy.dll"), "ne")
fs.add_functions("cd" * 32, ["1:0000"])
fs.set_identified(shas[pes[0]], {a: ("_memset", "libc") for a in addresses(pes[0], "pe")[:2]})
fs.add_module("ef" * 32, "weirdkind.dll", extra, "bogus")
fs.add_functions("ef" * 32, ["401000"])
fs.close()
shutil.copytree(SEED, STATE, dirs_exist_ok=True)
os.environ["CB_STATE"] = STATE
os.environ["CB_FUNCTIONS_DB"] = os.path.join(STATE, "functions.sqlite")
MEMO = os.path.join(STATE, "coverage_corpus.json")

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


for _n in ("measure", "corpus_modules", "extents", "render", "main", "reasons", "write_csv", "_counts"):
    setattr(ledger, _n, _timed(_n, getattr(ledger, _n)))

with open(os.path.join(HERE, "digests.txt"), "w", encoding="utf-8", newline="\n") as f:
    for k, v in DIGESTS.items():
        f.write("%s\t%s\n" % (k, v if v is not None else "None"))

cases = []
out = open(os.path.join(HERE, "expected.txt"), "w", encoding="utf-8", newline="\n")


def rec(op, label, args, text):
    """One call: the case line (op, label, arguments) and what the original answered."""
    cases.append("\t".join([op, label] + [str(a) for a in args]))
    out.write("== %s\n%s\n" % (label, text))


def jd(v):
    return json.dumps(v, sort_keys=True)


def led_json(led):
    led = dict(led)
    led.pop("generated", None)
    return jd(led)


def mask(text):
    return re.sub(r'"generated": "[^"]*"', '"generated": "T"', text)


def capture(argv):
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        code = ledger.main(list(argv))
    return "exit %d\n%s" % (code, mask(buf.getvalue()))


def sha256(path):
    return hashlib.sha256(open(path, "rb").read()).hexdigest() if os.path.isfile(path) else "(no file)"


# ---- the corpus
rec("corpus_modules", "corpus_modules 0", [0], jd(ledger.corpus_modules()))
fs = ledger.open_store()
rec("profiles", "profiles store", [1], jd(ledger.profiles(fs)))
rec("profiles", "profiles none", [0], jd(ledger.profiles(None)))

# ---- extents
for path, kind in [(pes[0], "pe"), (pes[1], "pe"), (nes[0], "ne"), (nes[1], "ne"), (les[0], "le"), (doses[0], "dos"), (pes[2], "pe"),
                   (nes[2], "ne"), (os.path.join(INPUTS, "ghost.dll"), "pe")]:
    sha = shas.get(path)
    addrs = list(fs.addresses(sha)) if sha else ["401000"]
    rec("extents", "extents %s %s" % (path, kind), [path, kind, ",".join(addrs)], jd(ledger.extents(path, kind, addrs)))
for addrs in (["400000", "zz"], [], ["1:0000", "1:0010"], ["99:0001"], ["401000", "401000", "401010"]):
    rec("extents", "extents %s pe %s" % (pes[0], ",".join(addrs)), [pes[0], "pe", ",".join(addrs)], jd(ledger.extents(pes[0], "pe", addrs)))
for addrs in (["1:0000", "1:0010", "1:0010"], ["1:xyz"], ["0:0", "3:ffff"], ["a"], []):
    rec("extents", "extents %s ne %s" % (nes[0], ",".join(addrs)), [nes[0], "ne", ",".join(addrs)], jd(ledger.extents(nes[0], "ne", addrs)))
missing = os.path.join(INPUTS, "no_such_file.bin")
rec("extents", "extents missing", [missing, "pe", "401000"], jd(ledger.extents(missing, "pe", ["401000"])))

# ---- one module's classes and counts
for path, kind in ((pes[0], "pe"), (pes[1], "pe"), (nes[1], "ne"), (les[0], "le"), (doses[0], "dos")):
    sha = shas[path]
    digest = DIGESTS[kind] or "1e1e1e1e1e1e"
    cls, judged = ledger._classes(fs, sha, digest)
    rec("classes", "classes %s %s" % (sha, digest), [sha, digest], jd([cls, judged]))
    rec("classes", "classes %s None" % sha, [sha, "None"], jd(list(ledger._classes(fs, sha, None))))
    for use_stamp in (0, 1, 1):
        stamp = fs.stamps().get(sha) if use_stamp else None
        c = ledger._counts(fs, sha, digest, path, kind, stamp)
        rec("counts", "counts %s %s stamp=%d" % (sha, digest, use_stamp), [sha, digest, path, kind, use_stamp], jd(c))
rec("counts", "counts None-digest", [shas[pes[0]], "None", pes[0], "pe", 0], jd(ledger._counts(fs, shas[pes[0]], None, pes[0], "pe", None)))
ghost = os.path.join(INPUTS, "ghost.dll")
rec("counts", "counts missing path", [shas[pes[0]], DIGESTS["pe"], ghost, "pe", 0],
    jd(ledger._counts(fs, shas[pes[0]], DIGESTS["pe"], ghost, "pe", None)))
fs.close()

# ---- the ledger
for module in ("", "kernel32.dll", "kernel32", os.path.basename(pes[0]), os.path.basename(nes[1]).rsplit(".", 1)[0], "extra_not_in_corpus",
               "ghost.dll", "nosuchmodule", "weirdkind.dll", os.path.basename(les[0]), os.path.basename(doses[0])):
    led = ledger.measure(module=module or None)
    rec("measure", "measure %r" % module, [module], led_json(led))
    for every in (0, 1):
        rec("render", "render %r %d" % (module, every), [module, every], "\n".join(ledger.render(led, bool(every))))
    rec("states", "states %r" % module, [module], jd([ledger._state(r) for r in led["modules"]]))
    csv_path = os.path.join(INPUTS, "ledger.csv")
    n = ledger.write_csv(led, csv_path)
    rec("csv", "csv %r" % module, [module, csv_path], "%d %s" % (n, sha256(csv_path)))
fs = ledger.open_store()
rec("measure_store", "measure with an open store", [""], led_json(ledger.measure(fs)))
rec("measure_store", "measure with an open store, one", [os.path.basename(pes[1])], led_json(ledger.measure(fs, module=os.path.basename(pes[1]))))
for path, kind in ((pes[0], "pe"), (pes[1], "pe"), (nes[0], "ne"), (nes[1], "ne"), (les[0], "le"), (doses[0], "dos")):
    digest = DIGESTS[kind] or "1e1e1e1e1e1e"
    for most in (8, 2, 0):
        rec("reasons", "reasons %s %s %d" % (shas[path], digest, most), [shas[path], digest, most], jd(ledger.reasons(fs, shas[path], digest, most)))
fs.close()
for a, b in ((1, 3), (0, 0), (5, 5), (1, 7), (999, 1000), (3, 0)):
    rec("pct", "pct %d %d" % (a, b), [a, b], repr(ledger._pct(a, b)))
rec("table", "table", [], "\n".join(ledger._table(["a", "bb", "c"], [["xx", "1", "22"], ["y", "333", "4"]])))

# ---- the whole refresh (the corpus sizes read again; the memo written)
rec("rm_memo", "remove the memo", [], "")
os.remove(MEMO)
rec("corpus_modules", "corpus_modules 1", [1], jd(ledger.corpus_modules(True)))
rec("memo", "memo after refresh", [], sha256(MEMO))
rec("rm_memo", "remove the memo", [], "")
os.remove(MEMO)
rec("corpus_modules", "corpus_modules without memo", [0], jd(ledger.corpus_modules()))
rec("memo", "memo written", [], sha256(MEMO))
# a stale memo entry: a size that is no longer the file's, and one for a file that is gone
memo = json.load(open(MEMO, encoding="utf-8"))
first = sorted(memo)[0]
memo[first][0] += 1
memo["C:/no/such/file.dll"] = [1, 2, 3, "pe"]
memo["C:/odd/short.dll"] = [1, 2]
memo["C:/odd/text.dll"] = {"a": [True, None, "x"], "b": 5}
stale = os.path.join(INPUTS, "stale_memo.json")
json.dump(memo, open(stale, "w", encoding="utf-8"))
rec("copy_memo", "put a stale memo", [stale], "")
shutil.copy(stale, MEMO)
rec("corpus_modules", "corpus_modules stale memo", [0], jd(ledger.corpus_modules()))
rec("memo", "memo after a stale one", [], sha256(MEMO))
broken = os.path.join(INPUTS, "broken_memo.json")
open(broken, "w").write("not json")
rec("copy_memo", "put a broken memo", [broken], "")
shutil.copy(broken, MEMO)
rec("corpus_modules", "corpus_modules broken memo", [0], jd(ledger.corpus_modules()))
rec("memo", "memo after a broken one", [], sha256(MEMO))

# ---- the command
mains = [[], ["--all"], ["--module", "kernel32.dll"], ["--module", os.path.basename(pes[0])], ["--module", os.path.basename(pes[1])],
         ["--module", os.path.basename(nes[1])], ["--module", os.path.basename(les[0])], ["--module", "extra_not_in_corpus"],
         ["--module", "nosuchmodule"], ["--json"], ["--json", "--module", os.path.basename(pes[1])], ["--help"], ["-h"],
         ["--csv", os.path.join(INPUTS, "main.csv")], ["--csv", os.path.join(INPUTS, "main.csv"), "--json"],
         ["--module", "ghost.dll"], ["--refresh", "--module", os.path.basename(nes[0])], ["--all", "--module", "kernel32"]]
for argv in mains:
    rec("main", "main %s" % " ".join(argv), argv, capture(argv))
rec("memo", "memo after the commands", [], sha256(MEMO))
rec("file", "file main.csv", [os.path.join(INPUTS, "main.csv")], sha256(os.path.join(INPUTS, "main.csv")))
# no store at all
rec("nostore", "no store", [], "")
os.environ["CB_FUNCTIONS_DB"] = os.path.join(STATE, "none.sqlite")        # (a store that is not there)
for argv in ([], ["--module", "kernel32.dll"], ["--json"]):
    rec("main", "main (no store) %s" % " ".join(argv), argv, capture(argv))
rec("measure", "measure no store", [""], led_json(ledger.measure()))
out.close()
open(os.path.join(HERE, "cases.txt"), "w", encoding="utf-8", newline="\n").write("\n".join(cases) + "\n")
print("%d cases" % len(cases))
sys.stderr.write("python seconds by call: " + ", ".join("%s %.2f" % kv for kv in sorted(TIMES.items())) + " (total %.2f)\n" % sum(TIMES.values()))
