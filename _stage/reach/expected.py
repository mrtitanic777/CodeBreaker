"""expected.py -- the ORIGINAL codebreaker.source.ne_asm.reachability on REAL NE modules (state/mini, state/setup).

Run as `python expected.py` from this folder (CB_WT names the worktree whose Python is imported; unset: the main checkout).
Writes ORDINALS.md (what main() reads from the working folder), cases.txt (for test.cpp) and expected.txt.
Cases (tab separated):
  consts                         the module's constants
  resolve HEXNAME                _Corpus.resolve(name)  (an existing path, a corpus name, an unknown name)
  reach PATH SEG                 reachable(Seg)         -> [n, sha1 of the sorted offsets]
  bt PATH SEG                    branch_targets(Seg)
  br PATH SEG                    _branches(Seg)
  sdr PATH SEG                   safe_data_regions(Seg)
  main ARG...                    main() with sys.argv = ["reachability.py"] + ARG (args as hex): [status, stdout] (stdout only
                                 where the status is 0; the usage texts of argparse's errors are not reproduced)
"""
import contextlib
import io
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
os.chdir(HERE)
sys.path.insert(0, os.path.join(os.path.dirname(HERE), "_ne_common"))
import common as C  # noqa: E402
from common import hx, sha1  # noqa: E402

from codebreaker.formats import ne as ne_mod  # noqa: E402
from codebreaker.source.ne_asm import reachability as R  # noqa: E402
from codebreaker.source.ne_asm import segment as trace  # noqa: E402

C.make_ordinals()
ORD = trace.load_ordinals("ORDINALS.md")
K = C.Cases(HERE)
_segs = {}


def seg(path, si):
    k = (path, si)
    if k not in _segs:
        _segs[k] = trace.Seg(ne_mod.NEFile(path), si, ORD)
    return _segs[k]


def set_d(s):
    return [len(s), sha1(",".join("%d" % x for x in sorted(s)))]


def lst_d(xs):
    return [len(xs), sha1("\n".join(repr(tuple(x)) for x in xs))]


K.add("consts", lambda: [sorted(R.JMPCC), sorted(R.NOFALL)])
for nm in (C.MINI + "NOMOUSE.DRV", "NOMOUSE.DRV", "nosuchmodule.drv", "ver.dll", "kernel32.dll", ""):
    K.add("resolve\t" + hx(nm), lambda nm=nm: ("same" if R._target().resolve(nm) == nm else os.path.basename(R._target().resolve(nm)).lower()))

for path, si in C.segs():
    tag = "%s\t%d" % (path, si)
    K.add("reach\t" + tag, lambda: set_d(R.reachable(seg(path, si))))
    K.add("bt\t" + tag, lambda: set_d(R.branch_targets(seg(path, si))))
    K.add("br\t" + tag, lambda: lst_d(R._branches(seg(path, si))))
    K.add("sdr\t" + tag, lambda: lst_d(R.safe_data_regions(seg(path, si))))
# a data segment and a segment that does not exist (Seg raises)
K.add("reach\t%s\t4" % (C.MINI + "COMM.DRV"), lambda: set_d(R.reachable(seg(C.MINI + "COMM.DRV", 4))))
K.add("reach\t%s\t9" % (C.MINI + "COMM.DRV"), lambda: set_d(R.reachable(seg(C.MINI + "COMM.DRV", 9))))


def run_main(args):
    argv = ["reachability.py"] + args
    old = sys.argv
    sys.argv = argv
    out, status = io.StringIO(), 0
    try:
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(io.StringIO()):
            R.main()
    except SystemExit as e:
        status = e.code if isinstance(e.code, int) else 1
    finally:
        sys.argv = old
    return [status, out.getvalue() if status == 0 else ""]


P = C.MINI + "NOMOUSE.DRV"
for args in ([P], [P, "--seg", "1"], [P, "--seg=1"], ["--seg", "2", C.MINI + "COMM.DRV"], [C.MINI + "KEYBOARD.DRV", "--seg", "0x3"],
             [C.MINI + "SOUND.DRV", "--seg=1"], [C.SETUP + "SETUP.EXE"], [C.MINI + "COMM.DRV", "--seg", "x"], [], [P, "extra"],
             [P, "--seg"], [P, "--nosuch"], [C.MINI + "COMM.DRV", "--seg", "-1"], [C.MINI + "COMM.DRV", "--seg", "9"],
             ["nosuchmodule.drv"], [C.MINI + "LZEXPAND.DLL", "--seg", "3"], ["ver.dll"]):
    K.add("\t".join(["main"] + [hx(a) for a in args]), lambda args=args: run_main(args))
K.close()
print(K.n, "cases")
