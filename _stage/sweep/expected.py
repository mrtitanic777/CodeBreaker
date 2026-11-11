"""expected.py -- the ORIGINAL codebreaker.source.ne_asm.sweep on the Windows 95 corpus (its NE modules) and real NE modules of state/mini
and state/setup.

Run as `python expected.py` from this folder (CB_WT names the worktree whose Python is imported; unset: the main checkout).
Writes ORDINALS.md, names_*.json, cases.txt (for test.cpp) and expected.txt.  TEMP is tmp/ and the state folder state_py/
(state_cpp/ for test.exe), both next to this file; what the sweep emits goes under out_sw/ (emptied before the C++ run).
Cases (tab separated; text fields are hex of UTF-8, '~' empty):
  consts                         ROOT is the checkout root
  setup                          _setup(): the working folder is ROOT afterwards
  ne_modules                     ne_modules(): [count, sha1 of "name|path" lines (the corpus folder as <C>), first names]; the working folder is ROOT
  resolve NAME                   _resolve(name)
  module NAME PATH BACKEND EMIT  sweep_module(name, path, backend, emit): the record, and the files emitted (path, sha1)
  sweep START LIMIT EMIT BACKEND sweep(limit, start, emit, backend): [sha1 of what it printed, the summary, the ledger file's sha1, the files emitted]
  main ARG...                    main() with sys.argv = ["ne_sweep.py"] + ARG: [status, sha1 of stdout (when the status is 0)]
"""
import contextlib
import io
import os
import shutil
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
os.chdir(HERE)
os.environ["TEMP"] = os.path.join(HERE, "tmp")
os.environ["TMP"] = os.path.join(HERE, "tmp")
shutil.rmtree(os.environ["TEMP"], ignore_errors=True)
os.makedirs(os.environ["TEMP"], exist_ok=True)
os.environ["CB_STATE"] = os.path.join(HERE, "state_py")
shutil.rmtree(os.environ["CB_STATE"], ignore_errors=True)
os.makedirs(os.environ["CB_STATE"], exist_ok=True)
sys.path.insert(0, os.path.join(os.path.dirname(HERE), "_ne_common"))
import common as C  # noqa: E402
from common import hx, sha1  # noqa: E402

from codebreaker.base import corpus  # noqa: E402
from codebreaker.source.ne_asm import sweep as S  # noqa: E402

C.make_ordinals()
shutil.rmtree("out_sw", ignore_errors=True)
os.makedirs("out_sw", exist_ok=True)
for tag in ("nomouse", "keyboard"):
    os.makedirs(os.path.join(os.environ["CB_STATE"], "idiomatic", tag), exist_ok=True)
with open("names_nomouse.json", "w") as f:
    f.write('{"1:0": "RenamedStart", "1:7": "9x y", "2:0": "OtherSeg"}')
with open("names_keyboard.json", "w") as f:
    f.write("{bad")
shutil.copy("names_nomouse.json", os.path.join(os.environ["CB_STATE"], "idiomatic", "nomouse", "names.json"))
shutil.copy("names_keyboard.json", os.path.join(os.environ["CB_STATE"], "idiomatic", "keyboard", "names.json"))
K = C.Cases(HERE)
CROOT = corpus.root()


def files_d(folder):
    """[(relative path, sha1)] of every file under folder, sorted"""
    out = []
    for dp, _dn, fn in os.walk(folder):
        for n in fn:
            p = os.path.join(dp, n)
            out.append([os.path.relpath(p, folder).replace("\\", "/"), sha1(open(p, "rb").read())])
    return sorted(out)


def rec_j(r):
    return r


def summary_j(summary):
    return summary


K.add("consts", lambda: [os.path.normcase(S.ROOT) == os.path.normcase(C.MAIN)])


def setup():
    S._setup()
    return os.path.normcase(os.getcwd()) == os.path.normcase(S.ROOT)


K.add("setup", setup)
MODS = S.ne_modules()
os.chdir(HERE)


def nem():
    mods = S.ne_modules()
    ok = os.path.normcase(os.getcwd()) == os.path.normcase(S.ROOT)
    lines = ["%s|%s" % (n, p.replace(CROOT, "<C>")) for n, p in mods]
    return [len(mods), sha1("\n".join(lines)), [n for n, _p in mods[:6]], ok]


K.add("ne_modules", nem)
for nm in (C.MINI + "NOMOUSE.DRV", "NOMOUSE.DRV", "nosuchmodule.drv", "ver.dll", "kernel32.dll", "", "krnl386.exe"):
    K.add("resolve\t" + hx(nm), lambda nm=nm: ("same" if S._resolve(nm) == nm else os.path.basename(S._resolve(nm)).lower()))

# sweep_module on small real modules: the record and the emitted files
n_case = [0]


def module_case(name, path, backend, emit):
    n_case[0] += 1
    folder = "out_sw/m%d" % n_case[0] if emit else None
    case = "module\t%s\t%s\t%s\t%s" % (hx(name), path, backend, folder or "~")

    def f():
        r = S.sweep_module(name, path, backend=backend, emit=folder)
        return [r, files_d(folder) if folder and os.path.isdir(folder) else None]
    K.add(case, f)


for name, path in (("NOMOUSE.DRV", C.MINI + "NOMOUSE.DRV"), ("KEYBOARD.DRV", C.MINI + "KEYBOARD.DRV"), ("SOUND.DRV", C.MINI + "SOUND.DRV"),
                   ("system.drv", C.MINI + "SYSTEM.DRV"), ("MINIKBD.DLL", C.MINI + "MINIKBD.DLL"), ("VER.DLL", C.MINI + "VER.DLL"),
                   ("LZEXPAND.DLL", C.MINI + "LZEXPAND.DLL"), ("SETUP.EXE", C.SETUP + "SETUP.EXE"), ("W95INF16.DLL", C.SETUP + "W95INF16.DLL"),
                   ("COMM.DRV", C.MINI + "COMM.DRV"), ("LMOUSE31.DRV", C.MINI + "LMOUSE31.DRV"), ("SERIFE.FON", C.MINI + "SERIFE.FON")):
    module_case(name, path, "nasm", True)
module_case("NOMOUSE.DRV", C.MINI + "NOMOUSE.DRV", "nasm", False)
module_case("NOMOUSE.DRV", C.MINI + "NOMOUSE.DRV", "ml", True)
module_case("VER.DLL", C.MINI + "VER.DLL", "ml", False)
module_case("nosuch.drv", HERE + "/no/such/file.drv", "nasm", False)
module_case("NOMOUSE.DRV", C.MINI + "WIN.INI", "nasm", False)                          # not an NE file
os.makedirs("out_sw/blocked", exist_ok=True)
open("out_sw/blocked/nomouse", "w").close()                                            # (a file where the module's folder goes)
K.add("module\t%s\t%s\tnasm\tout_sw/blocked" % (hx("NOMOUSE.DRV"), C.MINI + "NOMOUSE.DRV"),
      lambda: S.sweep_module("NOMOUSE.DRV", C.MINI + "NOMOUSE.DRV", backend="nasm", emit="out_sw/blocked"))

# sweep() over a few small modules of the corpus (picked by their place in the list)
small = [i for i, (n, p) in enumerate(MODS) if os.path.getsize(p) < 6000][:4]
big_first = [i for i, (n, p) in enumerate(MODS) if os.path.getsize(p) < 1500]
sweeps = [(small[0], 1, "~", "nasm"), (small[1], 1, "out_sw/s1", "nasm"), (small[0], 2, "out_sw/s2", "nasm"), (small[-1] if small else 0, 1, "out_sw/s3", "nasm"),
          (len(MODS) - 1, None, "~", "nasm"), (len(MODS) + 5, 3, "~", "nasm"), (len(MODS) - 2, -1, "~", "nasm")]
if big_first:
    sweeps.append((big_first[0], 1, "out_sw/s4", "ml"))
for start, limit, emit, backend in sweeps:
    def f(start=start, limit=limit, emit=emit, backend=backend):
        out = io.StringIO()
        folder = None if emit == "~" else emit
        with contextlib.redirect_stdout(out):
            summ = S.sweep(limit, start, folder, backend)
        led = None
        if folder and os.path.exists(os.path.join(folder, "_ne_idiom_summary.json")):
            led = sha1(open(os.path.join(folder, "_ne_idiom_summary.json"), "rb").read())
        return [sha1(out.getvalue()), summ, led, files_d(folder) if folder and os.path.isdir(folder) else None]
    K.add("sweep\t%d\t%s\t%s\t%s" % (start, "-" if limit is None else limit, emit, backend), f)


def run_main(args):
    old = sys.argv
    sys.argv = ["ne_sweep.py"] + args
    out, status = io.StringIO(), 0
    try:
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(io.StringIO()):
            S.main()
    except SystemExit as e:
        status = e.code if isinstance(e.code, int) else (1 if e.code else 0)
    finally:
        sys.argv = old
    return [status, sha1(out.getvalue()) if status == 0 else None]


for args in (["--list"], ["--limit", "1", "--start", str(small[0]), "--emit", "out_sw/mm1"], ["--limit=1", "--start=%d" % small[1], "--backend", "nasm"],
             ["--backend", "zz"], ["--limit", "x"], ["--start"], ["--nosuch"], ["--list", "extra"], ["--limit", "1", "--start", "-1"],
             ["--limit", "-1", "--start", str(len(MODS) - 3)]):
    K.add("\t".join(["main"] + [hx(a) for a in args]), lambda args=args: run_main(args))
K.close()
print(K.n, "cases")
