"""Prove native cache / tidy-state against the Python on a COPY of the main checkout's state/cache.

Usage: python tools/cache_tidy_proof.py setup | run  (needs cb_tests_base: toolsbuild_cb.cmd area base)
Everything lives under state/work/cachetidy/ of this worktree; the main checkout is only read.
"""
import hashlib
import json
import os
import shutil
import sqlite3
import subprocess
import sys
import time

WT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))                    # this checkout
MAIN_STATE = os.environ.get("CB_PROOF_MAIN_STATE") or os.path.join(WT, "state")   # the state whose cache is copied (only read)
HERE = os.path.join(WT, "state", "work", "cachetidy")
BASE = os.path.join(HERE, "base")               # base/state, base/Converted, base/build_cache
RUN = os.path.join(HERE, "run")                 # the working copy, the same path for Python and native
EXE = os.path.join(WT, "state", "cxx", "cb", "bin", "cb_tests_base.exe")
DAY = 86400


def old(path, days):
    t = time.time() - days * DAY
    os.utime(path, (t, t))


def copy_small_or_stub(src, dst, limit=40 << 20):
    os.makedirs(dst, exist_ok=True)
    for name in os.listdir(src):
        if name == "cache":
            continue
        s, d = os.path.join(src, name), os.path.join(dst, name)
        try:
            if os.path.isdir(s):
                size = sum(os.path.getsize(os.path.join(r, f)) for r, _d, fs in os.walk(s) for f in fs
                           if os.path.exists(os.path.join(r, f)))
                if size < limit:
                    shutil.copytree(s, d, copy_function=shutil.copy2, ignore_dangling_symlinks=True, dirs_exist_ok=True)
                else:
                    os.makedirs(d, exist_ok=True)
                    with open(os.path.join(d, "stub.bin"), "wb") as f:
                        f.write(b"\0" * (size // 5000 + 1))
            else:
                size = os.path.getsize(s)
                if size < limit:
                    shutil.copy2(s, d)
                else:
                    with open(d, "wb") as f:
                        f.write(b"\0" * (size // 5000 + 1))
        except OSError as e:
            print("skipped", name, e)


def setup():
    if os.path.exists(BASE):
        shutil.rmtree(BASE)
    state = os.path.join(BASE, "state")
    copy_small_or_stub(MAIN_STATE, state)
    shutil.copytree(os.path.join(MAIN_STATE, "cache"), os.path.join(state, "cache"), copy_function=shutil.copy2)
    shutil.copytree(os.path.join(MAIN_STATE, "build_cache"), os.path.join(BASE, "build_cache"), copy_function=shutil.copy2)
    # the Python's own files of now, made by the Python in a scratch state
    scratch = os.path.join(HERE, "scratch")
    if os.path.exists(scratch):
        shutil.rmtree(scratch)
    env = dict(os.environ, CB_STATE=scratch, CB_BUILD_CACHE=os.path.join(scratch, "bc"))
    code = ("from codebreaker.base import cache\n"
            "for kind in ('fingerprints','decompiled','decompiled_fn','gate16_tables','compilable16','exact','newkind'):\n"
            "    for i in range(5):\n"
            "        cache.put(kind, cache.key(kind, i), bytes([i]) * (3000 + i))\n"
            "p = cache.path('port-objects', cache.key('p'))\n"
            "import os; os.makedirs(os.path.dirname(p), exist_ok=True); open(p, 'wb').write(b'x' * 500)\n"
            "cache._close()\n")
    subprocess.run([sys.executable, "-c", code], cwd=WT, env=env, check=True)
    cache = os.path.join(state, "cache")
    for kind in os.listdir(os.path.join(scratch, "cache")):
        if kind in ("exact", "port-objects"):
            continue
        for n in os.listdir(os.path.join(scratch, "cache", kind)):
            if n.endswith(".sqlite"):
                os.makedirs(os.path.join(cache, kind), exist_ok=True)
                shutil.copy2(os.path.join(scratch, "cache", kind, n), os.path.join(cache, kind, n))
    # a kind that is only the Python's, with one old generation planted, and the generations of before the code moved on
    for kind, gens in (("decompiled", ["g203d43271953", "gdead00000001"]), ("decompiled_fn", ["g203d43271953"]),
                       ("gate16_tables", ["g203d43271953"]), ("compilable16", ["g5552a81e3e52"]),
                       ("fingerprints", ["g777777777777"])):
        for g in gens:
            src = None
            for n in os.listdir(os.path.join(cache, kind)) if os.path.isdir(os.path.join(cache, kind)) else ():
                if n.startswith(g):
                    src = n
            f = os.path.join(cache, kind, g + ".sqlite")
            if not os.path.exists(f):
                shutil.copy2([os.path.join(cache, kind, n) for n in os.listdir(os.path.join(cache, kind))
                              if n.endswith(".sqlite")][0], f)
            for sfx in ("", "-wal", "-shm"):
                if os.path.exists(f + sfx):
                    old(f + sfx, 5 if g != "gdead00000001" else 9)
    # stale parts of state/ and Converted/
    st = state
    for rel, days in (("phase3/runs/old1/hd.img", 3), ("phase3/walk_old/x.bin", 3), ("record16/walk/tag1/hd.img", 10),
                      ("record16/rec_mem_1.bin", 3), ("vxdboot/run_old/a", 3), ("fingerprints.sqlite", 0)):
        p = os.path.join(st, *rel.split("/"))
        os.makedirs(os.path.dirname(p), exist_ok=True)
        with open(p, "wb") as f:
            f.write(b"z" * 1234)
        if days:
            old(p, days)
            if rel.count("/") >= 2 and not rel.startswith("record16/walk"):
                old(os.path.dirname(p), days)
    os.makedirs(os.path.join(st, "phase3", "runs", "fresh1"), exist_ok=True)
    open(os.path.join(st, "phase3", "runs", "fresh1", "hd.img"), "wb").write(b"f" * 77)
    os.makedirs(os.path.join(st, "my_notes"), exist_ok=True)
    open(os.path.join(st, "my_notes", "n.txt"), "w").write("hand work")
    cv = os.path.join(BASE, "Converted")
    for prog, days in (("progA", 20), ("progB", 0)):
        for f in ("a.obj", "b.o", "prog.exe"):
            p = os.path.join(cv, prog, "build", f)
            os.makedirs(os.path.dirname(p), exist_ok=True)
            open(p, "wb").write(b"o" * 999)
            if days:
                old(p, days)
    print("base ready:", sum(os.path.getsize(os.path.join(r, f)) for r, _d, fs in os.walk(BASE) for f in fs) >> 20, "MB")


def listing(root):
    """{relative path: description}, a database by its rows (the bytes of a database depend on its history)."""
    out = {}
    for r, _d, fs in os.walk(root):
        for f in fs:
            p = os.path.join(r, f)
            rel = os.path.relpath(p, root).replace("\\", "/")
            if f.endswith(("-wal", "-shm")):
                out[rel] = "sidecar"
                continue
            if f.endswith(".sqlite") and "/cache/" in "/" + rel and not f.startswith(("functions", "results")):
                try:
                    db = sqlite3.connect("file:" + p.replace("\\", "/") + "?immutable=1", uri=True)
                    ks = [row[0] for row in db.execute("SELECT k FROM entry ORDER BY k")]
                    db.close()
                    out[rel] = "db rows=%d keys=%s" % (len(ks), hashlib.sha1("|".join(ks).encode()).hexdigest()[:10])
                except sqlite3.Error as e:
                    out[rel] = "db unreadable"
                continue
            try:
                out[rel] = "file %d" % os.path.getsize(p)
            except OSError:
                out[rel] = "gone"
    return out


def fresh():
    if os.path.exists(RUN):
        shutil.rmtree(RUN, ignore_errors=True)
    shutil.copytree(BASE, RUN, copy_function=shutil.copy2, dirs_exist_ok=True)


def env_for(cap):
    e = dict(os.environ, CB_STATE=os.path.join(RUN, "state"), CB_BUILD_CACHE=os.path.join(RUN, "build_cache"),
             CB_CONVERTED=os.path.join(RUN, "Converted"))
    e.pop("CB_NO_CACHE", None)
    if cap:
        e["CB_CACHE_MAX_GB"] = str(cap)
    else:
        e.pop("CB_CACHE_MAX_GB", None)
    return e


def run_python(args, cap):
    t = time.time()
    p = subprocess.run([sys.executable, "-m", "codebreaker"] + args, cwd=WT, env=env_for(cap), capture_output=True, text=True)
    return p.stdout, p.stderr, time.time() - t, p.returncode


def run_native(args, cap):
    e = env_for(cap)
    e["CB_PROOF_ARGS"] = " ".join(args)
    e["CB_PROOF_ROOT"] = WT
    e["CB_PROOF_OUT"] = os.path.join(HERE, "native_out.txt")
    if os.path.exists(e["CB_PROOF_OUT"]):
        os.remove(e["CB_PROOF_OUT"])
    t = time.time()
    p = subprocess.run([EXE, "--filter", "base.cache_proof"], cwd=WT, env=e, capture_output=True, text=True)
    dt = time.time() - t
    out = open(e["CB_PROOF_OUT"], encoding="utf-8").read() if os.path.exists(e["CB_PROOF_OUT"]) else "(no output) " + p.stdout[-400:]
    return out, p.stderr, dt, p.returncode


def norm(text):
    return text.replace("\r\n", "\n")


SCENARIOS = [
    ("cache (stats)", ["cache"], None),
    ("cache --prune", ["cache", "--prune"], None),
    ("cache --prune, cap 0.3 GB", ["cache", "--prune"], 0.3),
    ("cache --prune, cap 0.004 GB", ["cache", "--prune"], 0.004),
    ("tidy-state, cap 0.004 GB", ["tidy-state"], 0.004),
    ("cache --clear", ["cache", "--clear"], None),
    ("tidy-state --dry", ["tidy-state", "--dry"], None),
    ("tidy-state", ["tidy-state"], None),
    ("tidy-state --stray --keep-builds", ["tidy-state", "--stray", "--keep-builds"], None),
]


def main():
    results = []
    for name, args, cap in SCENARIOS:
        row = {"scenario": name}
        snaps = {}
        for who, fn in (("python", run_python), ("native", run_native)):
            fresh()
            before = listing(RUN)
            out, err, dt, rc = fn(args, cap)
            after = listing(RUN)
            snaps[who] = (norm(out), after, before, dt, rc, err)
            row[who + "_s"] = round(dt, 2)
        py, nat = snaps["python"], snaps["native"]
        os.makedirs(os.path.join(HERE, "out"), exist_ok=True)
        tag = name.replace(" ", "_").replace("(", "").replace(")", "").replace(",", "").replace("--", "")
        for who, s in snaps.items():
            open(os.path.join(HERE, "out", "%s.%s.txt" % (tag, who)), "w", encoding="utf-8").write(s[0] + "\nrc=%s\n%s" % (s[4], s[5][-500:]))
        row["output_same"] = py[0] == nat[0]
        row["rc"] = (py[4], nat[4])
        diff = sorted(k for k in set(py[1]) | set(nat[1]) if py[1].get(k) != nat[1].get(k) and not (py[1].get(k) == "sidecar" or nat[1].get(k) == "sidecar"))
        row["files_same"] = not diff
        row["file_diffs"] = [(k, py[1].get(k), nat[1].get(k)) for k in diff][:12]
        row["changed_by_python"] = sum(1 for k in py[1] if py[1][k] != py[2].get(k))
        results.append(row)
        print(json.dumps(row, indent=1))
    json.dump(results, open(os.path.join(HERE, "results.json"), "w"), indent=1)


if __name__ == "__main__":
    {"setup": setup, "run": main}[sys.argv[1]]()
