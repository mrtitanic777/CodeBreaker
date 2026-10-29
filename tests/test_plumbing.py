#!/usr/bin/env python3
"""Tests for the shared plumbing: tool resolution, the retail corpus resolver, and the isolated
sweep runner. Run:  python tests/test_plumbing.py     (exit code 0 = all passed)

Checks that need the OpenWatcom tree, the built decompilers or the retail corpus are SKIPPED
when those are not present, so the suite still runs on a bare checkout."""
import os
import sys
import json
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from codebreaker.base import corpus
from codebreaker.base import parallel
from codebreaker.build import watcom as toolchain   # noqa: E402
from codebreaker.jobs import pool   # noqa: E402

RESULTS = []


class Skip(Exception):
    pass


def check(fn):
    try:
        fn()
        RESULTS.append((fn.__name__, "PASS", ""))
    except Skip as s:
        RESULTS.append((fn.__name__, "SKIP", str(s)))
    except Exception as e:
        RESULTS.append((fn.__name__, "FAIL", "%s: %s" % (type(e).__name__, e)))


# --- toolchain -------------------------------------------------------------------------------
def test_decompiler_is_built():
    from codebreaker.decompile import decompiler
    if not decompiler.available():
        raise Skip("native/decompiler is not built")
    assert os.path.isfile(decompiler._exe()) and os.path.isdir(decompiler._languages())


def test_openwatcom():
    assert toolchain.ow_include("ne")[1].endswith("win")
    assert toolchain.ow_include("pe")[1].endswith("nt")
    env = toolchain.ow_env("ne")
    assert env["WATCOM"] == toolchain.openwatcom_root()
    assert env["INCLUDE"].split(os.pathsep) == toolchain.ow_include("ne")
    if not toolchain.ow_tool("wlink"):
        raise Skip("OpenWatcom not built")
    assert os.path.basename(toolchain.ow_cc("ne")).lower() == "wcc.exe"
    assert os.path.basename(toolchain.ow_cc("pe")).lower() == "wcc386.exe"
    assert os.path.isfile(os.path.join(toolchain.ow_lib("ne"), "libentry.obj"))


# --- corpus ----------------------------------------------------------------------------------
def _need_corpus():
    if not os.path.isdir(corpus.root()) or not corpus.index():
        raise Skip("retail corpus not present")


def test_corpus_rank_prefers_installed_copy():
    a = os.path.join("x", "setup", "inf", "foo.dll")
    b = os.path.join("x", "WINDOWS", "SYSTEM", "foo.dll")
    c = os.path.join("x", "WINDOWS", "foo.dll")
    assert sorted([a, c, b], key=corpus._rank) == [b, c, a]


def test_corpus_retail():
    _need_corpus()
    k = corpus.retail("kernel32")
    assert k and os.path.isfile(k), k
    assert corpus.retail("kernel32.dll") == k and corpus.retail("KERNEL32.DLL") == k
    assert corpus.retail(k) == k                           # a path passes through
    assert "system" in k.lower().replace("\\", "/").split("/"), k
    assert corpus.lane(k) == "pe"
    assert corpus.retail("no_such_module_zz.dll") is None
    assert corpus.candidates("kernel32.dll")[0] == k


def test_corpus_modules():
    _need_corpus()
    pe, ne = corpus.modules("pe"), corpus.modules("ne")
    assert pe and ne
    assert all(corpus.lane(p) == "pe" for p in pe[:25])
    names = [os.path.basename(p).lower() for p in pe]
    assert len(names) == len(set(names))                   # one path per module name
    assert not set(names) & {os.path.basename(p).lower() for p in ne}
    sizes = [os.path.getsize(p) for p in pe]
    assert sizes == sorted(sizes)


# --- format detection ------------------------------------------------------------------------
def test_format_detection():
    import struct
    from codebreaker.formats import detect
    d = tempfile.mkdtemp()

    def mk(name, sig, off=0x80):
        p = os.path.join(d, name)
        with open(p, "wb") as f:
            f.write(b"MZ" + b"\0" * 0x3A + struct.pack("<I", off) + b"\0" * (off - 0x40) + sig + b"\0\0")
        return p
    assert detect.format_of(mk("pe", b"PE")) == "PE" and detect.lane(mk("pe2", b"PE")) == "pe"
    assert detect.format_of(mk("ne", b"NE")) == "NE" and detect.lane(mk("ne2", b"NE")) == "ne"
    assert detect.format_of(mk("lx", b"LX")) == "LX" and detect.lane(mk("lx2", b"LX")) == "le"
    assert detect.format_of(mk("dos", b"ZZ")) == "MZ" and detect.lane(mk("dos2", b"ZZ")) == "dos"
    # a DOS stub of 64 KB or more: e_lfanew is 4 bytes (reading 2 mis-located the header)
    assert detect.format_of(mk("bigstub", b"PE", off=0x12000)) == "PE"
    with open(os.path.join(d, "text"), "wb") as f:
        f.write(b"not an executable")
    assert detect.format_of(os.path.join(d, "text")) is None
    assert detect.lane(os.path.join(d, "missing")) is None
    assert corpus.lane(mk("viacorpus", b"NE")) == "ne"     # the corpus resolver delegates here


# --- the job runner: threads, and a subprocess per item -----------------------------------------------------------------
_CHILD = ("import sys,json,time\n"
          "a=sys.argv[1]\n"
          "if a=='crash': raise SystemExit(3)\n"
          "if a=='slow': time.sleep(30)\n"
          "if a=='err': print(json.dumps({'error':'boom'})); raise SystemExit(0)\n"
          "print('noise'); print(json.dumps({'item':a,'n':len(a)}))\n")


def _argv(item):
    return [sys.executable, "-c", _CHILD, item]


def test_isolated_separates_failures_and_resumes():
    state = os.path.join(tempfile.mkdtemp(), "state.json")
    seen = []
    out = pool.isolated(_argv, ["ok1", "ok22", "crash", "err", "slow"], jobs=5, timeout=5,
                           state_path=state, on_result=lambda it, r, ok: seen.append((it, ok)))
    assert set(out["done"]) == {"ok1", "ok22"}, out
    assert out["done"]["ok22"] == {"item": "ok22", "n": 4}
    assert out["failed"]["crash"]["error"] == "no-output"
    assert out["failed"]["err"]["error"] == "boom"
    assert out["failed"]["slow"]["error"] == "timeout"     # a hang is bounded, and NOT "done"
    assert sorted(seen) == [("crash", False), ("err", False), ("ok1", True), ("ok22", True),
                            ("slow", False)]
    assert set(json.load(open(state))["done"]) == {"ok1", "ok22"}

    # resume: completed items are skipped, failed ones are retried (max_tries=2)
    out2 = pool.isolated(_argv, ["ok1", "ok22", "crash", "err"], jobs=4, timeout=5,
                            state_path=state)
    assert out2["ran"] == 2 and out2["skipped"] == 2, out2
    assert out2["failed"]["crash"]["tries"] == 2
    # third run: tries exhausted -> skipped, unless retry_failed
    out3 = pool.isolated(_argv, ["crash", "err"], jobs=2, timeout=5, state_path=state)
    assert out3["ran"] == 0 and out3["skipped"] == 2, out3
    out4 = pool.isolated(_argv, ["crash"], jobs=1, timeout=5, state_path=state,
                            retry_failed=True)
    assert out4["ran"] == 1 and out4["failed"]["crash"]["tries"] == 3


def test_threads_keep_order_and_raise():
    assert pool.threads(lambda n: n * n, range(20), jobs=4) == [n * n for n in range(20)]

    def bad(n):
        if n == 3:
            raise ValueError("three")
        return n
    try:
        pool.threads(bad, range(6), jobs=2)
        assert False, "the error was swallowed"
    except ValueError:
        pass
    got = dict(pool.as_done(bad, range(6), jobs=3, failed=lambda item, e: "failed: %s" % e))
    assert got == {0: 0, 1: 1, 2: 2, 3: "failed: three", 4: 4, 5: 5}


def test_throttle_is_safe():
    assert parallel.throttle(cores=None, below_normal=False) in (True, False)


# --- state that looks after its own size -----------------------------------------------------
_TEST_REV = 1                   # the generation of the kind "test-gen" (cache.GENERATIONS, below)


class _State:
    """A state folder of its own for one test (CB_STATE), the cache's open files let go after."""
    def __enter__(self):
        from codebreaker.base import cache
        self.was = os.environ.get("CB_STATE")
        self.path = tempfile.mkdtemp(prefix="cbstate_")
        os.environ["CB_STATE"] = self.path
        cache._close()
        return self.path

    def __exit__(self, *exc):
        import shutil
        from codebreaker.base import cache
        cache._close()
        if self.was is None:
            os.environ.pop("CB_STATE", None)
        else:
            os.environ["CB_STATE"] = self.was
        shutil.rmtree(self.path, ignore_errors=True)


def test_cache_drops_older_generations():
    global _TEST_REV
    from codebreaker.base import cache
    cache.GENERATIONS["test-gen"] = (__name__, "_TEST_REV")
    try:
        with _State():
            _TEST_REV = 1
            cache.put("test-gen", "a", b"made by revision 1")
            assert cache.get("test-gen", "a") == b"made by revision 1"
            kind = os.path.join(cache.root(), "test-gen")
            os.makedirs(os.path.join(kind, "ab"))              # (the layout of before: a file per result)
            old = [n for n in os.listdir(kind) if n.endswith(".sqlite")]
            assert old == [cache.generation("test-gen") + ".sqlite"], old
            _TEST_REV = 2                                       # the code moves on
            cache._close()                                      # (a new process)
            assert cache.get("test-gen", "a") is None
            assert sorted(n for n in os.listdir(kind) if not n.endswith(("-wal", "-shm"))) == \
                [cache.generation("test-gen") + ".sqlite"], os.listdir(kind)
            cache.put("test-gen", "a", b"made by revision 2")
            assert cache.get("test-gen", "a") == b"made by revision 2"
            assert [k for k, _s, _r, _g, _n in cache.stats()] == ["test-gen"]
    finally:
        del cache.GENERATIONS["test-gen"]
        _TEST_REV = 1


def test_cache_holds_its_cap_least_recently_used_first():
    import time
    from codebreaker.base import cache
    with _State():
        blob = os.urandom(100 << 10)
        now = int(time.time())
        for n in range(40):                                     # 4 MB, used one after another
            cache.put("test-lru", "k%d" % n, blob)
        db = cache._db("test-lru")
        for n in range(40):
            db.execute("UPDATE entry SET used=? WHERE k=?", (now - 100000 + n, "k%d" % n))
        db.commit()
        assert cache.get("test-lru", "k0") == blob              # the oldest, used again: kept
        p = cache.path("test-files", "f" * 40)                  # a tool's own file, older than all
        os.makedirs(os.path.dirname(p))
        with open(p, "wb") as f:
            f.write(blob)
        os.utime(p, (now - 200000, now - 200000))
        cap = 2 << 20
        cache.hold(cap)
        assert sum(cache.sizes().values()) <= cap, cache.sizes()
        assert not os.path.exists(p)                            # the longest unused went first
        assert cache.get("test-lru", "k0") == blob and cache.get("test-lru", "k39") == blob
        assert cache.get("test-lru", "k1") is None              # the oldest not used again: gone
        # and as results are written: never far past the cap (setting cache_max_gb)
        was = cache.CHECK_EVERY, os.environ.get("CB_CACHE_MAX_GB")
        cache.CHECK_EVERY, os.environ["CB_CACHE_MAX_GB"] = 1 << 20, "0.004"
        try:
            for n in range(100):                                # 10 MB written
                cache.put("test-lru", "w%d" % n, blob)
            assert sum(cache.sizes().values()) <= (4 << 20) + (2 << 20), cache.sizes()
            assert cache.get("test-lru", "w99") == blob          # the newest kept
        finally:
            cache.CHECK_EVERY = was[0]
            if was[1] is None:
                os.environ.pop("CB_CACHE_MAX_GB", None)
            else:
                os.environ["CB_CACHE_MAX_GB"] = was[1]


def test_fingerprints_are_kept_in_the_cache():
    from codebreaker.base import cache
    from codebreaker.verify import compare
    with _State() as state:
        k = compare.cache_key("test", 1, "input")
        assert compare.cache_get(k) is None
        compare.cache_put(k, {"exit": "ret", "deps": ()})
        assert compare.cache_get(k) == {"exit": "ret", "deps": ()}
        assert os.listdir(os.path.join(cache.root(), "fingerprints"))
        assert not os.path.exists(os.path.join(state, "fingerprints.sqlite"))


def test_run_folders_are_deleted():
    import time
    from codebreaker.base import runs
    with _State() as state:
        killed = runs.start("test-tool", "killed")             # a run killed two days ago
        open(os.path.join(killed, "hd.img"), "wb").close()
        os.utime(killed, (time.time() - 2 * 86400,) * 2)
        going = runs.start("test-tool", "going")               # one still going
        with runs.folder("test-tool", "mine") as work:
            assert os.path.isdir(work) and work.startswith(os.path.join(state, "test-tool", "runs"))
            open(os.path.join(work, "cd.iso"), "wb").close()
        assert not os.path.exists(work)                         # deleted at its end
        assert not os.path.exists(killed) and os.path.isdir(going)
        try:
            with runs.folder("test-tool", "fails") as work:
                raise RuntimeError("the machine hung")
        except RuntimeError:
            pass
        assert not os.path.exists(work)                         # deleted however it ends
        assert runs.results("test-tool").startswith(os.path.join(state, "test-tool", "shots"))


def test_tidy_state_finds_the_stale():
    import time
    from codebreaker.base import tidy_state
    with _State() as state:
        for p in ("fingerprints.sqlite", os.path.join("phase3", "walk_x", "hd.img"),
                  os.path.join("zz_scratch", "log.txt"), "results.sqlite"):
            os.makedirs(os.path.dirname(os.path.join(state, p)), exist_ok=True)
            open(os.path.join(state, p), "wb").close()
        os.utime(os.path.join(state, "phase3", "walk_x"), (time.time() - 2 * 86400,) * 2)
        found = {os.path.relpath(p, state).replace(os.sep, "/"): why for p, _s, why in tidy_state.stale()}
        assert found.get("fingerprints.sqlite") == "old caches" and found.get("phase3/walk_x") == "runs", found
        strays = [os.path.basename(p) for p, _s in tidy_state.stray()]
        assert strays == ["zz_scratch"], strays                 # (results.sqlite, phase3: CodeBreaker's)


def main():
    for fn in (test_decompiler_is_built, test_openwatcom,
               test_corpus_rank_prefers_installed_copy, test_corpus_retail, test_corpus_modules,
               test_format_detection,
               test_isolated_separates_failures_and_resumes, test_threads_keep_order_and_raise,
               test_throttle_is_safe, test_cache_drops_older_generations,
               test_cache_holds_its_cap_least_recently_used_first, test_fingerprints_are_kept_in_the_cache,
               test_run_folders_are_deleted, test_tidy_state_finds_the_stale):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
