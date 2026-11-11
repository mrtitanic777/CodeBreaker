"""expected.py -- the ORIGINAL codebreaker.source.vxd.tree on REAL VxDs of the Windows 95 corpus (the smallest standalone drivers) and
seeded damaged copies of them (every size field kept sane: see _ne_common/vxdcommon.py).

Run as `python expected.py` from this folder (CB_WT names the worktree whose Python is imported; unset: the main checkout).
TEMP is tmp/ and the state folder state_py/ (state_cpp/ for test.exe); the trees go under trees/ (emptied before the C++ run).
Cases (tab separated; a text field is hex of its UTF-8, '~' empty):
  seg_class FLAGS                        seg_class(flags)
  entrytab SPEC                          _entrytab_ddb(v)   (SPEC: V:path|member of the corpus, or B:file of a damaged copy)
  nprobe FILE / wprobe FILE              the probes of nasm / wasm on a file of inputs/: [return code, error text, pattern]
  probe_missing KIND                     a probe whose assembler is not there
  conv SPEC OBJ BACKEND MAXIT NAME       _converge_buildable on the whole object: [lines, sha1]
  tree SPEC MODE MAXINS DIR NAME         emit_tree(v, name, DIR, MAXINS, ...): the files written and their sha1; MODE plain | force | nasm | wasm
  main OUT ARG...                        the script block (runpy, as __main__): [status, stdout, the files under OUT]
"""
import contextlib
import io
import os
import random
import runpy
import shutil
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
os.chdir(HERE)
sys.path.insert(0, os.path.join(os.path.dirname(HERE), "_ne_common"))
import vxdcommon as X  # noqa: E402
from vxdcommon import hexs, sha1  # noqa: E402

X.isolate_env(HERE)
shutil.rmtree("trees", ignore_errors=True)
os.makedirs("trees", exist_ok=True)
os.makedirs("inputs", exist_ok=True)

from codebreaker.base import settings  # noqa: E402
from codebreaker.formats import le  # noqa: E402
from codebreaker.source.vxd import disassembly as dis  # noqa: E402
from codebreaker.source.vxd import masm as vxd_masm  # noqa: E402
from codebreaker.source.vxd import tree as T  # noqa: E402

open("capstone.txt", "w").write(X.VC.CAPSTONE_DLL)                  # the capstone.dll test.exe loads
K = X.VC.Cases(HERE)
NASM = settings.get("nasm")
WASM = os.path.join(os.path.dirname(settings.get("openwatcom") or ""), "bld", "wasm", "nt386", "wasm.exe")
rnd = random.Random(1113)


def lines_d(ls):
    return [len(ls), sha1("\n".join(ls))]


def files_d(folder):
    out = []
    for dp, _dn, fn in os.walk(folder):
        for n in fn:
            p = os.path.join(dp, n)
            out.append([os.path.relpath(p, folder).replace("\\", "/"), sha1(open(p, "rb").read())])
    return sorted(out)


# ---- the pure functions
flags = sorted(set([0, 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x8000, 0x14, 0x44, 0x54, 0x8004, 0x8014, 0x8044, 0x1000, 0xffff, 0x7fff, 0x8000 | 0x4]
                   + [rnd.randrange(0, 0x10000) for _ in range(60)]))
for fl in flags:
    K.add("seg_class\t%d" % fl, lambda fl=fl: list(T.seg_class(fl)))

drivers = X.small(14)
specs = []
for p, i, name, v in drivers:
    specs.append((X.VC.vspec(p, i), name, v))
for k, (sp, name, v) in enumerate(specs[:6]):               # seeded damaged copies of the first few
    for var, data in enumerate(X.damaged_copies(sp, v, 2, 5000 + k * 31)):
        fn = "inputs/dmg_%d_%d.bin" % (k, var)
        with open(fn, "wb") as f:
            f.write(data)
        try:
            specs.append(("B:" + fn, "DMG", le.LEFile.from_buffer(data)))
        except Exception:  # noqa: BLE001   (a damaged copy that is no LE any more: not a case)
            os.remove(fn)
for sz, p, i, name, v in X.corpus():                          # every driver of the corpus: the entry-table query
    K.add("entrytab\t%s" % X.VC.vspec(p, i), lambda v=v: T._entrytab_ddb(v))
for sp, name, v in specs:
    K.add("entrytab\t%s" % sp, lambda v=v: T._entrytab_ddb(v))

# ---- the probes
texts = ["bits 32\nmov eax, ebx\n", "nosuchop eax\n", "mov eax,\nmov ebx, ecx\nbadop\n", "        .386p\n_TEXT segment dword public use32 'CODE'\n        nop\n_TEXT ends\n        end\n",
         "foo bar baz\n", "", "mov eax, ebx\r\nbadop2\r\n"]
for n, t in enumerate(texts):
    fn = "inputs/probe_%d.asm" % n
    with open(fn, "w", newline="") as f:
        f.write(t)
    K.add("nprobe\t" + fn, lambda fn=fn: (lambda r: [r[0], r[1], r[2].pattern])(T._nasm_probe(NASM)(fn)))
    K.add("wprobe\t" + fn, lambda fn=fn: (lambda r: [r[0], r[1], r[2].pattern])(T._wasm_probe(WASM)(fn)))
K.add("probe_missing\tnasm", lambda: list(T._nasm_probe(os.path.join(HERE, "no", "such", "nasm.exe"))("inputs/probe_0.asm"))[:2])
K.add("probe_missing\twasm", lambda: list(T._wasm_probe(os.path.join(HERE, "no", "such", "wasm.exe"))("inputs/probe_0.asm"))[:2])


# ---- _converge_buildable and emit_tree
def head_tail(name, oi, seg, cls, fn="x.asm"):
    head = ["; %s -- %s object %d segment %s" % (fn, name, oi, seg), ".386p", "%s segment dword public use32 '%s'" % (seg, cls), ""]
    tail = ["%s ends" % seg, "        end", ""]
    return head, tail


def fresh(v):
    D = dis.VxDDisasm(v).run()
    D.v = v
    return D


for sp, name, v in specs[:5]:
    D = fresh(v)
    for o in v.objects[:2]:
        oi = o["idx"]
        cls, seg = T.seg_class(o["flags"])
        lo, hi = 0, len(D.code[oi])
        head, tail = head_tail(name, oi, seg, cls)
        db = D.ddb
        extra = {db["rva"]: "VXD_DDB"} if db and db["obj"] == oi else {}
        for backend, probe_arg, mk in (("nasm", NASM, T._nasm_probe), ("wasm", WASM, T._wasm_probe)):
            for maxit in (20, 1):
                def f(D=D, oi=oi, lo=lo, hi=hi, extra=extra, head=head, tail=tail, mk=mk, probe_arg=probe_arg, maxit=maxit):
                    return lines_d(T._converge_buildable(D, oi, lo, hi, extra, head, tail, mk(probe_arg), maxit))
                K.add("conv\t%s\t%d\t%s\t%d\t%s" % (sp, oi, backend, maxit, name), f)

n_tree = [0]


def tree_case(sp, v, name, mode, maxins):
    n_tree[0] += 1
    folder = "trees/t%d" % n_tree[0]

    def f():
        fr = None
        if mode == "force":
            D = fresh(v)
            fr = {}
            for (oi, a) in D.insns:
                if (a * 31 + 7) % 5 == 0:
                    fr.setdefault(oi, set()).add(a)
        files = T.emit_tree(v, name, folder, maxins, probe_nasm=NASM if mode == "nasm" else None, probe_wasm=WASM if mode == "wasm" else None,
                            force_raw_map=fr)
        return [files, files_d(folder)]
    K.add("tree\t%s\t%s\t%d\t%s\t%s" % (sp, mode, maxins, folder, name), f)


for sp, name, v in specs[:10]:
    tree_case(sp, v, name.split(".")[0].upper(), "plain", 6000)
for sp, name, v in specs[:6]:
    tree_case(sp, v, "T" + name.split(".")[0].upper(), "plain", 40)
    tree_case(sp, v, name.split(".")[0], "force", 6000)
for sp, name, v in specs[:4]:
    tree_case(sp, v, name.split(".")[0].upper(), "nasm", 6000)
    tree_case(sp, v, name.split(".")[0].upper(), "wasm", 6000)
for sp, name, v in specs[14:]:
    tree_case(sp, v, "DMG", "plain", 6000)
for sp, name, v in specs[10:12]:                                 # a big one: the whole of a driver split into many files
    tree_case(sp, v, name.split(".")[0].upper(), "plain", 25)

# ---- the script block
TREE = os.path.join(os.path.dirname(T.__file__), "tree.py")


def run_main(out, args):
    old = sys.argv
    sys.argv = [TREE] + args
    buf, status = io.StringIO(), 0
    try:
        with contextlib.redirect_stdout(buf):
            runpy.run_path(TREE, run_name="__main__")
    except SystemExit as e:
        status = e.code if isinstance(e.code, int) else (1 if e.code else 0)
    finally:
        sys.argv = old
    text = buf.getvalue().replace(os.environ["CB_STATE"], "<STATE>")
    return [status, text, files_d(out) if out and os.path.isdir(out) else None]


p0 = drivers[0][0]
p1 = drivers[1][0]
for out, args in (("trees/m1", [p0, "trees/m1"]), ("trees/m2", [p1, "trees/m2"]), ("", []), (os.path.join(os.environ["CB_STATE"], "vxd", "tree"), [p1]),
                  ("", [os.path.join(HERE, "nosuch.vxd")]), ("trees/m3", [os.path.join(HERE, "inputs", "probe_0.asm"), "trees/m3"])):
    out2 = out.replace(os.environ["CB_STATE"], "<STATE>") if out else ""
    K.add("\t".join(["main", hexs(out2)] + [hexs(a) for a in args]), lambda out=out, args=args: run_main(out, args))
K.close()
print(K.n, "cases")
