"""expected.py -- the ORIGINAL codebreaker.coverage.reach on a tree of real programs (corpus, tests/data) and made-up gaps.

Builds seed/ (a function store with results for some of the programs) and inputs/ (the tree, a disk image, results of an
earlier run), runs the original on a copy of seed (state/), and writes cases.txt (op, label, arguments), knowledge.txt (what
platforms.identify and reconstruct.identity answered: test.cpp's stand-ins for those two) and expected.txt.
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
import subprocess
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOTDIR = "C:/Users/mrtit/Documents/GitHub/CodeBreaker"
sys.path.insert(0, ROOTDIR)
os.environ["COLUMNS"] = "80"
os.environ["PATH"] = os.path.dirname(os.path.abspath(__file__)) + os.pathsep + os.environ["PATH"]      # (the git.exe of this folder first)
SEED = os.path.join(HERE, "seed")
STATE = os.path.join(HERE, "state")
INPUTS = os.path.join(HERE, "inputs")
for d in (SEED, STATE, INPUTS):
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)
os.environ["CB_STATE"] = SEED
os.environ["CB_FUNCTIONS_DB"] = os.path.join(SEED, "functions.sqlite")

# a git that answers like a repository with one revision (git.exe in this folder: found before any other)
GIT = os.path.join(HERE, "git.exe")
if not os.path.isfile(GIT):
    open(os.path.join(HERE, "fakegit.c"), "w").write('#include <stdio.h>\n#include <stdlib.h>\nint main(void) { if (getenv("FAKE_GIT_FAIL")) return 1; puts("abc1234"); return 0; }\n')
    subprocess.run('cmd /c ""C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat" >nul 2>&1 && cd /d "%s" && cl /nologo /O1 fakegit.c /Fe:git.exe /Fo:fakegit.obj >nul"' % HERE, shell=True, check=True)
os.chdir(HERE)

from codebreaker import platforms                          # noqa: E402
from codebreaker.base import corpus                        # noqa: E402
from codebreaker.coverage import reach                     # noqa: E402
from codebreaker.decompile import describe as dec          # noqa: E402
from codebreaker.formats import pe_functions               # noqa: E402
from codebreaker.formats.ne import NEFile                  # noqa: E402
from codebreaker.store import functions as F               # noqa: E402
from codebreaker.verify import reconstruct                 # noqa: E402

EXTS = (".dll", ".exe", ".drv", ".ocx", ".cpl", ".vxd", ".386", ".sys", ".com")


def pick(kind, n, lo=3000, hi=120000):
    mods = sorted(corpus.modules(kind, exts=EXTS), key=lambda p: (os.path.getsize(p), p))
    mods = [m for m in mods if lo < os.path.getsize(m) < hi]
    return [mods[i * (len(mods) - 1) // (n - 1)] for i in range(n)]


# ---- the tree
TREE = os.path.join(INPUTS, "tree")
for sub in ("", "sub", "sub/deeper", ".hid", "node_modules", "__pycache__", "emptydir", "games"):
    os.makedirs(os.path.join(TREE, sub), exist_ok=True)
pes, nes, les, doses = pick("pe", 4), pick("ne", 3), pick("le", 2, 3000, 400000), pick("dos", 2, 3000, 200000)
placed = []
for i, f in enumerate(pes + nes + les + doses):
    dest = [TREE, os.path.join(TREE, "sub"), os.path.join(TREE, "sub", "deeper")][i % 3]
    shutil.copy(f, os.path.join(dest, os.path.basename(f)))
    placed.append(os.path.join(dest, os.path.basename(f)))
shutil.copy(pes[0], os.path.join(TREE, ".hid", "hidden.exe"))
shutil.copy(pes[0], os.path.join(TREE, "node_modules", "mod.dll"))
shutil.copy(pes[0], os.path.join(TREE, "__pycache__", "cache.dll"))
shutil.copy(pes[0], os.path.join(TREE, "sub", "samecontent.exe"))                  # (the same program under another name)
DATA = os.path.join(ROOTDIR, "tests", "data")
for kind in ("nes", "gb", "genesis", "msx", "sms", "zx"):
    for name in sorted(os.listdir(os.path.join(DATA, kind)))[:2]:
        p = os.path.join(DATA, kind, name)
        if os.path.isfile(p) and os.path.getsize(p) < 600000:
            shutil.copy(p, os.path.join(TREE, "games", kind + "_" + name))
elf_hdr = lambda machine, little=True: b"\x7fELF\x02" + (b"\x01" if little else b"\x02") + b"\x01" + bytes(11) + (machine.to_bytes(2, "little" if little else "big")) + bytes(300)  # noqa: E731
blobs = {
    "psx.exe": b"PS-X EXE" + bytes(2000),
    "n64.z64": b"\x80\x37\x12\x40" + bytes(4000),
    "n64.v64": b"\x37\x80\x40\x12" + bytes(4000),
    "n64.n64": b"\x40\x12\x37\x80" + bytes(4000),
    "gba.gba": bytes(3) + b"\xea" + bytes(0xB2 - 4) + b"\x96" + bytes(600),
    "ds.nds": bytes(2000),
    "snes.sfc": bytes(70000),
    "snes.smc": bytes(70000),
    "mips.elf": elf_hdr(0x08), "ppc.elf": elf_hdr(0x14, False), "ppc64.elf": elf_hdr(0x15, False), "arm.elf": elf_hdr(0x28), "x64.elf": elf_hdr(0x3E),
    "x86.elf": elf_hdr(0x03), "aarch64.elf": elf_hdr(0xB7), "sh.elf": elf_hdr(0x2A), "spu.elf": elf_hdr(0x17), "odd.elf": elf_hdr(0x1234),
    "short.elf": b"\x7fELF\x01\x01\x01" + bytes(8),
    "app.apk": b"PK\x03\x04" + bytes(500), "notapk.zip": b"PK\x03\x04" + bytes(500),
    "text.txt": b"just some text\r\n" * 20, "empty.bin": b"", "random.dat": bytes((i * 37 + 11) % 256 for i in range(5000)),
    "gbahead.bin": bytes(0xB2) + b"\x96",
}
for name, blob in blobs.items():
    open(os.path.join(TREE, "games", name), "wb").write(blob)
IMG = os.path.join(INPUTS, "boot.img")
shutil.copy(os.path.join(ROOTDIR, "..", "Windows95", "design", "corpus", "files", "setup", "media", "boot.img"), IMG)
shutil.copy(IMG, os.path.join(TREE, "disk.img"))
open(os.path.join(INPUTS, "not_an_image.img"), "wb").write(bytes(4096))
shutil.copy(os.path.join(INPUTS, "not_an_image.img"), os.path.join(TREE, "bad.img"))

# ---- the store: results for the PE and NE programs of the tree, under the digests the identity says
DIGESTS = reconstruct.digests()
CYCLE = [("equivalent", ""), ("equivalent", "low-coverage"), ("divergent", ""), ("inconclusive", ""), ("no-compile", ""), ("equivalent", ""),
         ("consistent", ""), ("divergent", "")]


def addresses(path, kind, n=30):
    if kind == "pe":
        return ["%x" % va for va in sorted(pe_functions.function_spans(path))][:n]
    return ["%d:%04x" % so for so in sorted(dec.functions(NEFile(path)))][:n]


fs = F.FunctionStore()
for path, kind in [(placed[0], "pe"), (placed[1], "pe"), (placed[4], "ne")]:
    sha, digest = reconstruct.identity(path)
    fs.add_module(sha, os.path.basename(path).lower(), path, kind)
    fs.add_profile(digest, "{}")
    addrs = addresses(path, kind)
    fs.add_functions(sha, addrs)
    rows = []
    for i, a in enumerate(addrs):
        v, limits = CYCLE[i % len(CYCLE)]
        rows.append(dict(addr=a, sig="s%d" % i, name="f_%d" % i, source="int f(void){return %d;}" % i, verdict=v, limits=limits, reason="r" if v == "divergent" else ""))
    fs.record_many(sha, "decompiler", "1", digest, rows)
fs.close()
shutil.copytree(SEED, STATE, dirs_exist_ok=True)
os.environ["CB_STATE"] = STATE
os.environ["CB_FUNCTIONS_DB"] = os.path.join(STATE, "functions.sqlite")

# ---- what the lanes' registry and the reconstruct module answer are recorded as they are asked
KNOW_P, KNOW_I = {}, {}
_identify = platforms.identify


def recording_identify(path):
    p = _identify(path)
    KNOW_P[path] = None if p is None else (p.title, p.name, 1 if p.lane else 0, p.family, p.container)
    return p


platforms.identify = recording_identify
_identity = reconstruct.identity


def recording_identity(path):
    try:
        r = _identity(path)
    except Exception as e:                                  # noqa: BLE001
        KNOW_I[path] = ("ERR", type(e).__name__, str(e).replace("\n", " ").replace("\t", " "))
        raise
    KNOW_I[path] = ("OK", r[0], r[1])
    return r


reconstruct.identity = recording_identity

# ---- the measuring child: a command that behaves as the case needs (the real one is `python -m codebreaker measure`)
COMMANDS = {
    "ok": ["cmd", "/c", "echo", "hello"],
    "fail": ["cmd", "/c", "echo line1 & echo boom here & exit 3"],
    "failsilent": ["cmd", "/c", "exit 5"],
    "failbig": ["cmd", "/c", "echo " + "z" * 300 + " & exit 9"],
    "hang": ["ping", "-n", "30", "127.0.0.1"],
}


def child_for(path):
    base = os.path.basename(path).lower()
    for key in COMMANDS:
        if base.startswith(key + "_") or base == key:
            return COMMANDS[key]
    return COMMANDS["ok"]


_Popen = subprocess.Popen


class FakePopen(_Popen):
    def __init__(self, args, *a, **k):
        if isinstance(args, list) and args[1:3] == ["-m", "codebreaker"]:
            args = child_for(args[4])
        super().__init__(args, *a, **k)


subprocess.Popen = FakePopen
time.time = lambda: 1000.0

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


for _n in ("describe", "programs", "_unpack", "run", "report", "_sha", "spread", "_tools"):
    setattr(reach, _n, _timed(_n, getattr(reach, _n)))

cases = []
out = open(os.path.join(HERE, "expected.txt"), "w", encoding="utf-8", newline="\n")


def rec(op, label, args, text):
    cases.append("\t".join([op, label] + [str(a) for a in args]))
    out.write("== %s\n%s\n" % (label, text))


def jd(v):
    return json.dumps(v, sort_keys=True)


def sha256(path):
    return hashlib.sha256(open(path, "rb").read()).hexdigest() if os.path.isfile(path) else "(no file)"


def capture(argv):
    o, e = io.StringIO(), io.StringIO()
    code = 0
    with contextlib.redirect_stdout(o), contextlib.redirect_stderr(e):
        try:
            code = reach.main(list(argv))
        except SystemExit as x:
            code = "SystemExit(%s)" % x.code
        except Exception as x:                               # noqa: BLE001
            code = "%s: %s" % (type(x).__name__, x)
    return "exit %s\n-- stdout\n%s-- stderr\n%s" % (code, o.getvalue(), e.getvalue())


def results_text(name):
    """The results file with its time removed, as JSON; and the report beside it."""
    p = os.path.join(STATE, "reach", name + ".json")
    if not os.path.isfile(p):
        return "(no results file)"
    d = json.load(open(p, encoding="utf-8"))
    d.pop("when", None)
    return jd(d) + "\n" + sha256(os.path.join(STATE, "reach", name + ".md"))


def md_text(name):
    p = os.path.join(STATE, "reach", name + ".md")
    return open(p, "rb").read().decode("utf-8").replace("\r\n", "\n") if os.path.isfile(p) else "(no report)"


# ---- describe, _gap, _sha over every file of the tree (and the ones that are not there)
all_files = sorted(os.path.join(d, n) for d, _dirs, names in os.walk(TREE) for n in names)
all_files += [IMG, os.path.join(INPUTS, "not_an_image.img"), os.path.join(INPUTS, "no_such_file.exe")]
for f in all_files:
    try:
        r = reach.describe(f)
    except Exception as e:                                   # noqa: BLE001
        r = "%s" % type(e).__name__
    rec("describe", "describe %s" % os.path.relpath(f, HERE), [f], jd(r))
    head = open(f, "rb").read(0x200) if os.path.isfile(f) else b""
    rec("gap", "gap %s" % os.path.relpath(f, HERE), [f], jd(reach._gap(f, head)))
    if os.path.isfile(f):
        rec("sha", "sha %s" % os.path.relpath(f, HERE), [f], reach._sha(f))
rec("sha", "sha missing", [os.path.join(INPUTS, "no_such_file.exe")], "")
for ext, head in ((".apk", b"PK"), (".APK", b"PK"), (".zip", b"PK"), (".nds", b""), (".SFC", b""), (".exe", b"\x7fELF\x01\x02\x01" + bytes(11) + b"\x00\x28"), (".bin", b"PS-X EXE")):
    p = os.path.join(INPUTS, "x" + ext)
    rec("gaphead", "gaphead %s %s" % (ext, head.hex()), [p, head.hex()], jd(reach._gap(p, head)))

# ---- the programs
for inputs, containers in (([TREE], 20), ([TREE], 3), ([TREE], 0), ([os.path.join(TREE, "sub"), os.path.join(TREE, "games", "psx.exe")], 5),
                           ([IMG], 20), ([IMG], 2), ([os.path.join(INPUTS, "not_an_image.img")], 20), ([os.path.join(INPUTS, "no_such_dir")], 20),
                           ([TREE, TREE], 1), ([os.path.join(TREE, "emptydir")], 20), ([], 20)):
    logs = []
    got = reach.programs(inputs, containers, logs.append)
    rec("programs", "programs %s %d" % (";".join(os.path.relpath(i, HERE) for i in inputs), containers), [containers] + [";".join(inputs)], jd([[os.path.relpath(p, HERE) if False else p, o] for p, o in got]) + "\n" + jd(logs))
# unpacking
for path, limit in ((IMG, 20), (IMG, 3), (IMG, 0), (IMG, 100), (os.path.join(INPUTS, "not_an_image.img"), 20), (os.path.join(INPUTS, "no_such_file.img"), 5)):
    logs = []
    got = reach._unpack(path, limit, logs.append)
    rec("unpack", "unpack %s %d" % (os.path.basename(path), limit), [path, limit], jd([got, logs, [sha256(p) for p in got]]))
rec("env", "no failure of git", ["FAKE_GIT_FAIL", ""], "")
rec("tools", "tools", [], reach._tools())
os.environ["FAKE_GIT_FAIL"] = "1"
rec("env", "git fails", ["FAKE_GIT_FAIL", "1"], "")
rec("tools", "tools git fails", [], reach._tools())
del os.environ["FAKE_GIT_FAIL"]
rec("env", "git again", ["FAKE_GIT_FAIL", ""], "")

# ---- spread
for sizes, n, max_size in (("5,1,9,3,7", 3, 100), ("5,1,9,3,7", 2, 100), ("5,1,9,3,7", 1, 100), ("5,1,9,3,7", 0, 100), ("5,1,9,3,7", 5, 100), ("5,1,9,3,7", 9, 100),
                           ("5,1,9,3,7", 3, 6), ("10,20,30,40,50,60,70,80,90,100,110", 4, 1000), ("10,20,30,40,50,60,70,80,90,100,110", 5, 1000),
                           ("10,20,30,40,50,60,70,80,90,100,110", 7, 85), ("", 3, 10), ("4,4,4,4", 2, 10), ("1,2,3,4,5,6,7,8,9,10,11,12,13", 6, 99), ("2", -1, 5)):
    rows = [{"size": int(s), "path": "p%d_%d" % (int(s), i)} for i, s in enumerate(sizes.split(",")) if s]
    rec("spread", "spread %s %d %d" % (sizes, n, max_size), [sizes, n, max_size], jd([r["path"] for r in reach.spread(rows, n, max_size)]))

# ---- the report, from rows of every shape
def row(platform, lane, family="windows", found="PE executable", path="x.exe", judged=None, **kw):
    r = {"found": found, "platform": platform, "lane": lane, "family": family, "path": path, "origin": "", "size": 10, "sha": path}
    if judged is not None:
        r["judged"] = judged
    r.update(kw)
    return r


def j(tally, stopped="", seconds=1.5):
    return {"tally": tally, "seconds": seconds, "stopped": stopped}


T1 = {"verified": 6, "weak": 2, "unverified": 1, "no-compile": 3, "not-judged": 4}
REPORTS = {
    "empty": [],
    "plain": [row("Windows 32-bit", "pe", path="a.exe", judged=j(T1)), row("Windows 32-bit", "pe", path="b.exe", judged=j({"verified": 1})), row("Windows 32-bit", "pe", path="c.exe")],
    "stopped": [row("Windows 16-bit", "ne", path="a.exe", judged=j({"verified": 5}, "timed out after 300 s")), row("Windows 16-bit", "ne", path="b.exe", judged=j({"verified": 2, "weak": 1}, "boom: it stopped")),
                row("Windows 16-bit", "ne", path="c.exe", judged=j({}))],
    "gap": [row("Game Boy Advance (ARM7TDMI)", None, "gap", "unknown", "g.gba"), row("Nintendo 64 (MIPS R4300)", None, "gap", "unknown", "n.z64"), row("Windows 32-bit", "pe", path="a.exe", judged=j(T1))],
    "other": [row("", None, "", "unknown", "t%d.dat" % i) for i in range(12)] + [row("", None, "", "text", "x.txt")] * 3 + [row("", None, "source", "source / text: C", "a.c")],
    "mixed": [row("Z", None, "gap", path="z.bin"), row("A", "alane", path="a.bin", judged=j({"verified": 3, "unverified": 1})), row("B", "blane", path="b.bin"), row("A", "alane", path="a2.bin", judged=j({"weak": 7}, "x")),
              row("", None, "", "unknown", "q.dat")],
    "divisions": [row("P", "pl", path="p%d" % i, judged=j({"verified": 1, "weak": 2, "unverified": 3, "no-compile": 0})) for i in range(3)],
    "judged zero": [row("P", "pl", path="p", judged=j({"verified": 0, "weak": 0}))],
}
for name, rows in REPORTS.items():
    p = os.path.join(INPUTS, "rows_%s.json" % name.replace(" ", "_"))
    json.dump({"programs": rows}, open(p, "w", encoding="utf-8"))
    rec("report", "report %s" % name, [p], reach.report(rows))
rec("report", "report big", [os.path.join(INPUTS, "rows_mixed.json")], reach.report(REPORTS["mixed"] * 2))

# ---- judge: the children
sc = os.path.join(INPUTS, "kids")
os.makedirs(sc, exist_ok=True)
pe_known = placed[0]
for kid, timeout in (("ok_a.exe", 20), ("fail_a.exe", 20), ("failsilent_a.exe", 20), ("failbig_a.exe", 20), ("hang_a.exe", 1)):
    p = os.path.join(sc, kid)
    shutil.copy(placed[0], p)
    sha, digest = reconstruct.identity(p)            # (recorded)
    rec("judge", "judge %s %d" % (kid, timeout), [p, timeout], jd(reach.judge(p, timeout)))
p = os.path.join(sc, "ok_notally.dat")
open(p, "wb").write(b"data that nothing reconstructs")
rec("judge", "judge no tally", [p, 20], jd(reach.judge(p, 20)))
# tally_of
for f in (placed[0], placed[1], placed[4], placed[3], os.path.join(TREE, "games", "text.txt"), os.path.join(INPUTS, "no_such_file.exe"), pe_known):
    rec("tally", "tally %s" % os.path.relpath(f, HERE), [f], jd(reach.tally_of(f)))

# ---- run
old_rows = [
    {"found": "PE executable", "platform": "Windows 32-bit", "lane": "pe", "family": "windows", "path": placed[0], "origin": TREE, "size": 1, "sha": hashlib.sha256(open(placed[0], "rb").read()).hexdigest(), "picked": True,
     "judged": {"tally": {"verified": 9}, "seconds": 2.5, "stopped": ""}, "tools": "abc1234", "note": {"x": [1, 2, None]}},
    {"found": "NES ROM", "platform": "Nintendo NES", "lane": "nes", "family": "nes", "path": "gone.nes", "origin": "", "size": 5, "sha": "OLD2", "picked": True,
     "judged": {"tally": {"weak": 1}, "seconds": 0.1, "stopped": "timed out after 5 s"}, "tools": "zzz"},
    {"found": "unknown", "platform": "", "lane": None, "family": "", "path": "gone.dat", "origin": "", "size": 5, "sha": "OLD3"},
]
os.makedirs(os.path.join(STATE, "reach"), exist_ok=True)
runs = [
    # (name, inputs, per_platform, do_judge, timeout, containers, again, max_size)
    ("r_plain", [TREE], 5, False, 5, 20, False, 4 << 20),
    ("r_plain", [TREE], 5, False, 5, 20, False, 4 << 20),                         # (again: the file is there)
    ("r_few", [TREE], 2, False, 5, 2, False, 60000),
    ("r_judge", [TREE], 3, True, 3, 5, False, 4 << 20),
    ("r_judge", [TREE], 3, True, 3, 5, False, 4 << 20),                           # (cached: nothing judged again)
    ("r_judge", [TREE], 3, True, 3, 5, True, 4 << 20),                            # (--again)
    ("r_old", [TREE], 3, True, 3, 5, False, 4 << 20),                             # (with the earlier run's results)
    ("r_img", [IMG, os.path.join(INPUTS, "not_an_image.img")], 5, False, 5, 6, False, 4 << 20),
    ("r_none", [os.path.join(INPUTS, "no_such_dir")], 5, False, 5, 20, False, 4 << 20),
    ("r_zero", [TREE], 0, True, 3, 20, False, 4 << 20),
    ("r_tiny", [TREE], 5, True, 3, 20, False, 1),
]
for name, inputs, per, do_judge, timeout, containers, again, max_size in runs:
    if name == "r_old":
        old_file = os.path.join(INPUTS, "old_results.json")
        json.dump({"when": "x", "inputs": [], "tools": "abc1234", "programs": old_rows}, open(old_file, "w", encoding="utf-8"), indent=1)
        shutil.copy(old_file, os.path.join(STATE, "reach", "r_old.json"))
        rec("put_old", "put the earlier results", [old_file], "")
    logs = []
    rows, text = reach.run(inputs, per, do_judge, timeout, containers, name, again, max_size, logs.append)
    rec("run", "run %s %d %d %d %d %d %d" % (name, per, int(do_judge), timeout, containers, int(again), max_size),
        [name, per, int(do_judge), timeout, containers, int(again), max_size, ";".join(inputs)],
        jd([rows, text, logs]) + "\n" + results_text(name) + "\n" + md_text(name))

# ---- the command
mains = [[], ["-h"], ["--help"], ["--report"], ["--report", "r_plain"], ["--report", "no_such_run"], ["--report=r_judge"], ["--rep", "r_few"], ["--report="],
         [TREE, "--name", "m_a"], ["--name", "m_b", TREE], [TREE, "--judge", "--timeout", "2", "--name", "m_c", "--per-platform", "1"], [TREE, "--name=m_d", "--containers=1"],
         ["--max-size", "30", TREE, "--name", "m_e"], ["--per-platform"], ["--per-platform", "x"], ["--timeout", "0x10", TREE], ["--bogus"], [TREE, "--judge", TREE],
         ["--name", "--judge"], ["--judge=1"], [TREE, "--again", "--ju", "--na", "m_f", "--ti", "3", "--per", "1"], ["--per-platform", " 2 ", TREE, "--name", "m_g"],
         ["--", TREE, "--judge"], ["--max-size", "-5", TREE, "--name", "m_h"], ["-x"], ["-hx"], ["--per-platform=5", "--per-platform=2", TREE, "--name", "m_i"],
         [TREE, "--bogus", "other", "--name", "m_j"], ["--containers", "abc"], ["--again", "--judge", "--name", "m_k", os.path.join(INPUTS, "no_such_dir")]]
for argv in mains:
    rec("main", "main %s" % " ".join(a if os.sep not in a else os.path.relpath(a, HERE) for a in argv), argv, capture(argv))
out.close()
open(os.path.join(HERE, "cases.txt"), "w", encoding="utf-8", newline="\n").write("\n".join(cases) + "\n")
with open(os.path.join(HERE, "knowledge.txt"), "w", encoding="utf-8", newline="\n") as f:
    for path, v in KNOW_P.items():
        f.write("\t".join(["P", path] + (["NONE"] if v is None else [v[0], v[1], str(v[2]), v[3], v[4]])) + "\n")
    for path, v in KNOW_I.items():
        f.write("\t".join(["I", "" if path is None else path] + list(v)) + "\n")
print("%d cases" % len(cases))
sys.stderr.write("python seconds by call: " + ", ".join("%s %.2f" % kv for kv in sorted(TIMES.items())) + "\n")
