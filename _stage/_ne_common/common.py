"""common.py -- shared by the harnesses of source.ne_asm.{reachability,recon,idiom,sweep} (and the VxD tool-lane ones).

* the ORIGINAL Python is imported from the worktree named by CB_WT (recheck.sh exports it; unset: the main checkout), with
  the settings of the MAIN checkout (settings.json is not in a worktree);
* the real NE inputs: state/mini and state/setup (the Windows 95 Setup's own modules);
* the Cases writer: cases.txt (what test.cpp reads) and expected.txt ("<case>\\t=>\\t<json>").
"""
import hashlib
import json
import os
import random
import sys

MAIN = r"C:\Users\mrtit\Documents\GitHub\CodeBreaker"
WT = os.environ.get("CB_WT") or MAIN
for p in (MAIN, WT):
    if p in sys.path:
        sys.path.remove(p)
    sys.path.insert(0, p)                 # (WT first)
from codebreaker.base import settings  # noqa: E402

settings.ROOT = MAIN                      # (settings.json, state/, the corpus: the main checkout's)
settings.FILE = os.path.join(MAIN, "settings.json")
settings._loaded = None
os.environ["CB_CORPUS_ROOT"] = (os.path.normpath(os.path.join(MAIN, "..", "Windows95", "design", "corpus", "files")))

MINI = MAIN + "/state/mini/original/"
SETUP = MAIN + "/state/setup/original/"

# (path, [segment numbers]) -- real NE code segments, small ones (every one of the small modules, one or two of the big)
SMALL = [("COMM.DRV", (1, 2, 3)), ("KEYBOARD.DRV", (1, 2, 3, 4, 5, 6, 7, 8)), ("LZEXPAND.DLL", (1, 2, 3, 4, 5, 6, 7, 8)),
         ("MINIKBD.DLL", (1,)), ("NOMOUSE.DRV", (1,)), ("SOUND.DRV", (1,)), ("SYSTEM.DRV", (1,)), ("VER.DLL", (1, 2, 3, 4)),
         ("LMOUSE31.DRV", (1,)), ("MSMOUS31.DRV", (1,)), ("KRNL386.EXE", (3,)), ("GDI.EXE", (4, 5, 6, 8)), ("USER.EXE", (4, 5))]
SMALL_SETUP = [("SETUP.EXE", (1,)), ("W95INF16.DLL", (1,)), ("SUEXPAND.DLL", (1, 3, 4, 5, 6)), ("NETDI.DLL", (4,))]


def segs(extra=()):
    out = [(MINI + n, s) for n, ss in SMALL for s in ss] + [(SETUP + n, s) for n, ss in SMALL_SETUP for s in ss]
    return out + list(extra)


def sha1(b):
    if isinstance(b, str):
        b = b.encode("utf-8")
    return hashlib.sha1(bytes(b)).hexdigest()


def dump(v):
    return json.dumps(v, separators=(",", ":"), ensure_ascii=True)


def err(e):
    t = type(e).__name__
    return ["ERR", t, str(e) if t == "RuntimeError" else ""]


def tryit(f):
    try:
        return f()
    except Exception as e:  # noqa: BLE001
        return err(e)


def make_ordinals(path="ORDINALS.md"):
    """the file load_ordinals reads from the working folder (seeded)"""
    R = random.Random(20261111)
    lines = ["# Ordinals", "", "| Ordinal | Name |", "|---|---|"]
    for m in ("KERNEL", "USER", "GDI", "KEYBOARD", "SOUND", "MOUSE", "COMM", "LZEXPAND", "VER", "SYSTEM", "WIN87EM", "SETUPX", "SUWIN"):
        for n in range(1, 400):
            if R.random() < 0.8:
                lines.append("| %s.%d | `%s_%d` | x |" % (m, n, m[:3].capitalize(), n))
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines) + "\n")


class Cases:
    def __init__(self, folder):
        self.cases = open(os.path.join(folder, "cases.txt"), "w", newline="\n", encoding="ascii")
        self.expected = open(os.path.join(folder, "expected.txt"), "w", newline="\n", encoding="ascii")
        self.n = 0
        self.times = {}
        self.cwd = os.getcwd()

    def add(self, case, f):
        import time
        os.chdir(self.cwd)                         # (a case may change the working folder)
        t0 = time.time()
        res = tryit(f)
        self.times[case.split(chr(9))[0]] = self.times.get(case.split(chr(9))[0], 0.0) + time.time() - t0
        self.cases.write(case + "\n")
        self.expected.write("%s\t=>\t%s\n" % (case, dump(res)))
        self.n += 1

    def close(self):
        for k, v in sorted(self.times.items()):
            print('  %s: %.2f s' % (k, v), file=sys.stderr)
        self.cases.close()
        self.expected.close()


def hx(s):
    """a text field of a case: hex of its UTF-8 ('~' for empty)"""
    b = s.encode("utf-8")
    return b.hex() if b else "~"


def agg(items):
    """[count, sha1 of the lines]"""
    xs = [str(x) for x in items]
    return [len(xs), sha1("\n".join(xs))]
