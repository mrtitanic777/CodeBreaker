#!/usr/bin/env python3
"""Tests for the shared plumbing: tool resolution, the retail corpus resolver, and the isolated
sweep runner. Run:  python test/test_plumbing.py     (exit code 0 = all passed)

Checks that need the OpenWatcom tree, the built decompilers or the retail corpus are SKIPPED
when those are not present, so the suite still runs on a bare checkout."""
import os
import sys
import json
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT not in sys.path:
    sys.path.insert(0, ROOT)

from src.core import corpus, parallel, toolchain   # noqa: E402

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
def test_decompiler_resolution():
    assert toolchain.decompiler("pe") == toolchain.decompiler("win95pedec.exe")
    assert toolchain.decompiler("win95pedec") == toolchain.decompiler("pe")
    assert toolchain.decompiler("no-such-lane") == ""
    if not toolchain.decompiler("pe"):
        raise Skip("decompilers not built")
    assert os.path.isfile(toolchain.decompiler("pe"))
    assert len(toolchain.decompiler_build_id()) == 12


def test_decompiler_dir_override():
    old = os.environ.get("CB_DECOMPILER_BIN")
    d = tempfile.mkdtemp()
    try:
        os.environ["CB_DECOMPILER_BIN"] = d
        assert toolchain.decompiler_dir() == d
        assert toolchain.decompiler("pe") == ""            # nothing built there
        open(os.path.join(d, "win95pedec.exe"), "wb").close()
        assert toolchain.decompiler("pe") == os.path.join(d, "win95pedec.exe")
    finally:
        if old is None:
            os.environ.pop("CB_DECOMPILER_BIN", None)
        else:
            os.environ["CB_DECOMPILER_BIN"] = old


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


# --- isolated sweep runner -------------------------------------------------------------------
_CHILD = ("import sys,json,time\n"
          "a=sys.argv[1]\n"
          "if a=='crash': raise SystemExit(3)\n"
          "if a=='slow': time.sleep(30)\n"
          "if a=='err': print(json.dumps({'error':'boom'})); raise SystemExit(0)\n"
          "print('noise'); print(json.dumps({'item':a,'n':len(a)}))\n")


def _argv(item):
    return [sys.executable, "-c", _CHILD, item]


def test_isolated_map_separates_failures_and_resumes():
    state = os.path.join(tempfile.mkdtemp(), "state.json")
    seen = []
    out = parallel.isolated_map(_argv, ["ok1", "ok22", "crash", "err", "slow"], jobs=5, timeout=5,
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
    out2 = parallel.isolated_map(_argv, ["ok1", "ok22", "crash", "err"], jobs=4, timeout=5,
                                 state_path=state)
    assert out2["ran"] == 2 and out2["skipped"] == 2, out2
    assert out2["failed"]["crash"]["tries"] == 2
    # third run: tries exhausted -> skipped, unless retry_failed
    out3 = parallel.isolated_map(_argv, ["crash", "err"], jobs=2, timeout=5, state_path=state)
    assert out3["ran"] == 0 and out3["skipped"] == 2, out3
    out4 = parallel.isolated_map(_argv, ["crash"], jobs=1, timeout=5, state_path=state,
                                 retry_failed=True)
    assert out4["ran"] == 1 and out4["failed"]["crash"]["tries"] == 3


def test_throttle_is_safe():
    assert parallel.throttle(cores=None, below_normal=False) in (True, False)


def main():
    for fn in (test_decompiler_resolution, test_decompiler_dir_override, test_openwatcom,
               test_corpus_rank_prefers_installed_copy, test_corpus_retail, test_corpus_modules,
               test_isolated_map_separates_failures_and_resumes, test_throttle_is_safe):
        check(fn)
    for name, status, detail in RESULTS:
        print("  %-4s %s%s" % (status, name, ("  -- " + detail) if detail else ""))
    fails = sum(1 for _n, s, _d in RESULTS if s == "FAIL")
    print("%d passed, %d skipped, %d failed" % (
        sum(1 for r in RESULTS if r[1] == "PASS"), sum(1 for r in RESULTS if r[1] == "SKIP"), fails))
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
