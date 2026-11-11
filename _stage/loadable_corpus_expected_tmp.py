"""expected.py -- the ORIGINAL codebreaker.source.vxd.loadable_corpus on a small corpus of REAL VxDs of the Windows 95 corpus (copied into inputs/mini/:
three drivers under .vxd / .386 / .pdr, one named as an extra file, a truncated one that will not open, a real-mode file that is no LE).

Run as `python expected.py` from this folder (CB_WT names the worktree whose Python is imported; unset: the main checkout).
TEMP is tmp/ and the state folder state_py/ (state_cpp/ for test.exe); the results files go under runs/ (emptied before the C++ run).
Cases (tab separated; a text field is hex of its UTF-8, '~' empty):
  consts                           _VXD_EXTS, _EXTRA_FILES, ROOT is the checkout root
  is_le FILE                       _is_le_vxd(FILE)
  one SPEC                         _one_module(name, v): the record (its seconds as 0)
  run SCEN                         run(inputs/mini, runs/SCEN.json, redo, workers=1): [stdout, the file's text, the results]  (times normalised)
                                   SCEN: fresh | resume | redo | seeded | seeded_redo   (resume/redo/seeded_redo continue the file `fresh`/`seeded` wrote)
  save                             _save / _summary on a seeded results dict
  main ARG...                      the script block (runpy, as __main__): [status, stdout, the file's text]
"""
import contextlib
import io
import json
import os
import re
import runpy
import shutil
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
os.chdir(HERE)
sys.path.insert(0, os.path.join(os.path.dirname(HERE), "_ne_common"))
import vxdcommon as X  # noqa: E402
from vxdcommon import hexs, sha1  # noqa: E402

X.isolate_env(HERE)
shutil.rmtree("runs", ignore_errors=True)
os.makedirs("runs", exist_ok=True)
shutil.rmtree("inputs", ignore_errors=True)
os.makedirs("inputs/mini", exist_ok=True)

from codebreaker.formats import le  # noqa: E402
from codebreaker.source.vxd import loadable_corpus as C  # noqa: E402

K = X.VC.Cases(HERE)
open("capstone.txt", "w").write(X.VC.CAPSTONE_DLL)                  # the capstone.dll test.exe loads
drivers = X.small(6)
for (p, i, name, v), dst in zip(drivers, ["splitter.vxd", "srammtd.vxd", "vtdi.386", "vsd.pdr", "csmapper.sys", "diskmgmt.mpd"]):
    shutil.copy(p, os.path.join("inputs/mini", dst))
data = open(drivers[0][0], "rb").read()
with open("inputs/mini/trunc.vxd", "wb") as f:                     # an LE that will not open
    f.write(data[:0x100] + b"\0" * 0x10)
with open("inputs/mini/notle.vxd", "wb") as f:                     # a real-mode file: skipped
    f.write(b"MZ" + b"\0" * 100)
with open("inputs/mini/readme.txt", "w") as f:
    f.write("not a driver\n")
MINI = os.path.join(HERE, "inputs", "mini")


def norm_text(s):
    s = re.sub(r'"seconds": [0-9.]+', '"seconds": 0', s)
    s = re.sub(r"\[\d+s\]", "[Ns]", s)
    s = re.sub(r"\d+s ==", "Ns ==", s)
    return s.replace(os.environ["CB_STATE"], "<STATE>").replace(HERE, "<H>")


def file_text(p):
    return norm_text(open(p, "rb").read().decode("latin-1")) if os.path.exists(p) else None


K.add("consts", lambda: [os.path.normcase(C.ROOT) == os.path.normcase(X.MAIN)])
def norm_rec(r):
    if "seconds" in r:
        r = dict(r)
        r["seconds"] = 0
    return r


for p, i, name, v in drivers[:5]:
    K.add("one\t%s" % X.VC.vspec(p, i), lambda v=v, name=name: norm_rec(C._one(name, v, X.MAIN)))


def run_case(scen):
    out_json = "runs/%s.json" % scen
    base = {"resume": "fresh", "redo": "fresh", "seeded_redo": "seeded"}.get(scen)
    if base:
        shutil.copy("runs/%s.json" % base, out_json)
    if scen.startswith("seeded"):
        if scen == "seeded":
            seed = {"splitter": {"ok": True, "stage": None, "seconds": 1.5, "objects": 1, "device_id": 5, "device_id_ok": True, "identity": True, "resource_len": 7, "vxd_len": 100},
                    "vsd": {"ok": False, "stage": "exception", "log": "x", "trace": "tb", "seconds": 0.5}, "trunc": {"ok": False, "stage": "open", "log": "bad"},
                    "failed1": {"ok": False, "stage": "link", "seconds": 1.0, "log": "L\nM"}, "failed2": {"ok": False, "stage": None, "seconds": 2.0, "log": ""}}
            with open(out_json, "w") as f:
                json.dump(seed, f, indent=1)
            shutil.copy(out_json, "inputs/seeded.json")
    redo = {"redo": ["splitter"], "seeded_redo": ["splitter", "failed1", "vsd"]}.get(scen)
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        res = C.run(MINI, out_json, redo=redo, workers=1, root=X.MAIN)
    for k, r in res.items():
        if "seconds" in r:
            r["seconds"] = 0
    return [norm_text(buf.getvalue()), file_text(out_json), res]


for scen in ("fresh", "resume", "redo", "seeded", "seeded_redo"):
    K.add("run\t" + scen, lambda scen=scen: run_case(scen))


def save_case():
    res = {"a": {"byte_exact": True, "objects": [{"obj": 1, "match": True, "ok": True, "len": 10, "symbolic": 2, "forced_raw": 0, "raw_fallback": False, "diffs": 0, "err": None}],
                 "n_obj": 1, "n_exact": 1, "symbolic": 2, "forced_raw": 0, "raw_fallback_objs": 0, "seconds": 0.25},
           "bé": {"error": "é中", "trace": "line1\nline2"}, "c": {"byte_exact": False, "objects": [], "n_obj": 0, "n_exact": 0, "symbolic": 0, "forced_raw": 1,
                                                                            "raw_fallback_objs": 1, "seconds": 3.0}}
    C._save(res, "runs/save.json")
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        C._summary(res, 12.4)
        C._summary({}, 0.0)
    return [file_text("runs/save.json"), norm_text(buf.getvalue()), os.path.exists("runs/save.json.tmp")]


K.add("save", save_case)
CORPUS = os.path.join(os.path.dirname(C.__file__), "corpus.py")


def run_main(args):
    old = sys.argv
    sys.argv = [CORPUS] + args
    buf, status = io.StringIO(), 0
    try:
        with contextlib.redirect_stdout(buf):
            runpy.run_path(CORPUS, run_name="__main__")
    except SystemExit as e:
        status = e.code if isinstance(e.code, int) else (1 if e.code else 0)
    finally:
        sys.argv = old
    outs = [a for a in args if a.endswith(".json")]
    return [status, norm_text(buf.getvalue()), file_text(outs[0]) if outs else None]


for args in (["inputs/mini", "runs/m1.json", "--workers", "1"], ["--workers", "1", "inputs/mini", "runs/m2.json"], ["inputs/mini", "runs/m3.json", "--redo", "splitter,vsd", "--workers", "1"],
             ["--redo", "vtdi", "inputs/mini", "runs/m4b.json", "--workers", "1"], ["--workers"], ["--workers", "x", "inputs/mini", "runs/m4.json"], ["inputs/mini", "runs/m5.json", "--workers", "1", "--redo", "~"]):
    K.add("\t".join(["main"] + [hexs(a) for a in args]), lambda args=args: run_main(args))
K.close()
print(K.n, "cases")
