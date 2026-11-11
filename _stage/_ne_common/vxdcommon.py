"""vxdcommon.py -- what the VxD tool-lane harnesses (source.vxd.{tree,owbuild,from_source,corpus,loadable_corpus,boot}) share, on top of
_ne_common/common.py (the Python under test is the worktree CB_WT's; the settings are the main checkout's) and ../_vxd_common/common.py."""
import os
import random
import shutil
import sys

HERE_COMMON = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE_COMMON)
import common as C  # noqa: E402,F401  (sets the Python and the settings)

import importlib.util  # noqa: E402

_spec = importlib.util.spec_from_file_location("vxd_common_mod", os.path.join(os.path.dirname(HERE_COMMON), "_vxd_common", "common.py"))
VC = importlib.util.module_from_spec(_spec)         # (../_vxd_common/common.py: the same module name as ours, so loaded by its path)
_spec.loader.exec_module(VC)
from codebreaker.formats import le  # noqa: E402

MAIN = C.MAIN
sha1 = C.sha1
dump = C.dump


def sane(v2):
    """every field the code sizes an allocation or a loop from stays small (a damaged header can announce gigabytes; see
    source.vxd.le_writer's expected.py)"""
    return (0 < v2.mpages <= 3000 and 0 < v2.pagesize <= 65536 and v2.mpages * v2.pagesize <= (16 << 20)
            and len(v2.objects) <= 64 and sum(o["vsize"] for o in v2.objects) <= (4 << 20)
            and sum(o["pcnt"] for o in v2.objects) <= 4096 and all(0 <= o["pmidx"] <= 4096 for o in v2.objects))


def isolate_env(here, state=True):
    """TEMP and the state folder next to the harness (what the tools keep goes there)"""
    os.environ["TEMP"] = os.path.join(here, "tmp")
    os.environ["TMP"] = os.path.join(here, "tmp")
    shutil.rmtree(os.environ["TEMP"], ignore_errors=True)
    os.makedirs(os.environ["TEMP"], exist_ok=True)
    if not state:               # (LINK fails with a long variable such as CB_STATE in its environment: the linking harnesses keep the default state folder)
        return
    os.environ["CB_STATE"] = os.path.join(here, "state_py")
    shutil.rmtree(os.environ["CB_STATE"], ignore_errors=True)
    os.makedirs(os.environ["CB_STATE"], exist_ok=True)


_CORPUS = []


def corpus():
    """[(size, path, member index, member name, LEFile)] of every driver of the Win95 corpus, smallest first"""
    if not _CORPUS:
        for p in le.corpus_files():
            for i, (n, v) in enumerate(le.members(p)):
                _CORPUS.append((len(v.d), p, i, n, v))
        _CORPUS.sort(key=lambda r: (r[0], r[1], r[2]))
    return _CORPUS


def small(n, pagesize=None, skip=0, maxsize=60000):
    """the n smallest standalone drivers (a file of one LE), optionally of one page size"""
    out = []
    for sz, p, i, name, v in corpus():
        if sz > maxsize:
            break
        if pagesize and v.pagesize != pagesize:
            continue
        if i != 0 or len(le.members(p)) != 1:
            continue
        out.append((p, i, name, v))
    return out[skip:skip + n]


def damaged_copies(path, v, k, seed0, nflips=4):
    """k seeded damaged copies of the LE's bytes whose header fields stay sane: [(file name, bytes)]"""
    out = []
    for var in range(k):
        for attempt in range(40):
            data = VC.damaged(v.d, seed0 + var * 7 + 100000 * attempt, nflips=nflips + 2 * var)
            try:
                v2 = le.LEFile.from_buffer(data)
                ok = sane(v2)
            except Exception:  # noqa: BLE001
                ok = True
            if ok:
                out.append(data)
                break
    return out


def hexs(s):
    b = s.encode("utf-8")
    return b.hex() if b else "~"
