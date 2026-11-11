"""expected.py -- the ORIGINAL codebreaker.source.ne_asm.idiom on REAL NE modules (state/mini, state/setup) and seeded inputs.

Run as `python expected.py` from this folder (CB_WT names the worktree whose Python is imported; unset: the main checkout).
Writes ORDINALS.md, names_*.json, inputs/, cases.txt (for test.cpp) and expected.txt.  TEMP is tmp/ and the state folder
state_py/ (state_cpp/ for test.exe), both next to this file.
Cases (tab separated; text fields are hex of UTF-8, '~' empty):
  consts                         _JMP, _JCC, _CALL, _PREAMBLE, _NLST
  branch TEXT                    _branch(text)
  procname PATH SEG OFF MODE     _procname(Seg, off); MODE 1: a Seg with name_override (odd names on its first functions)
  emit PATH SEG LO HI FMODE MODE _emit(Seg, lo, hi, funcs, 1, force_db, data_off): FMODE 0 funcs=None, 1 [], 2 the range's functions
                                 and one more, 3 with a duplicate; MODE 0 plain, 1 force_db by a pattern, 2 + data_off of the safe spans
  stage                          _stage_cmacros(".") (the inc folder: names and sizes)
  asm PATH SEG LO HI             assemble(text of _emit) with the DDK ML.EXE + CMACROS.INC on the native DOS executive
  nasm PATH SEG LO HI            assemble_nasm(text of _emit)  (the nasm-masm fork)
  nasm_text HEX                  assemble_nasm(text) on given text (errors); nasm_missing HEX: with a nasm that is not there
  div SEED                       _nasm_divergers on a seeded listing
  rec BACKEND PATH SEG LO HI [F] reconcile_range;   ver BACKEND PATH SEG LO HI  verify_range
  main SAVE ARG...               the script block of the module (runpy, as __main__), sys.argv = [file] + ARG, NE_SAVE = SAVE:
                                 [status, stdout (state folder as <STATE>)] (an error: its type)
"""
import contextlib
import io
import os
import random
import re
import runpy
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

from codebreaker.formats import ne as ne_mod  # noqa: E402
from codebreaker.source.ne_asm import idiom as I  # noqa: E402
from codebreaker.source.ne_asm import segment as trace  # noqa: E402

C.make_ordinals()
ORD = trace.load_ordinals("ORDINALS.md")
K = C.Cases(HERE)
rnd = random.Random(1112)
_segs = {}
_ovsegs = {}


def seg(path, si):
    k = (path, si)
    if k not in _segs:
        _segs[k] = trace.Seg(ne_mod.NEFile(path), si, ORD)
    return _segs[k]


ODD = ["9bad-name", "", "ok$?@_1", "né w", "a b", "_x"]


def ovseg(path, si):
    k = (path, si)
    if k not in _ovsegs:
        base = seg(path, si)
        ov = {f: ODD[i % len(ODD)] for i, f in enumerate(sorted(base.funcs)[:8])}
        _ovsegs[k] = trace.Seg(ne_mod.NEFile(path), si, ORD, name_override=ov)
    return _ovsegs[k]


def force_of(off):
    return (off * 37 + 11) % 5 == 0


def lines_d(text):
    ls = text.split("\n")
    return [len(ls), sha1(text)]


def set_d(s):
    return [len(s), sha1(",".join("%d" % x for x in sorted(s)))]


def res_d(res):
    if res is None:
        return None
    return [res["exact"], res["length_match"], res["our_len"], res["shipped_len"], res["n_diff"], res["first_diff"], res["masked"]]


def norm_asm_out(text):
    """a listing / log without what changes from run to run (the assembler's banner with the date, the temp folder names)"""
    out = [ln for ln in text.splitlines() if "Macro Assembler" not in ln and "Page" not in ln]
    return re.sub(r"ne_nasm_\w{8}", "ne_nasm_X", "\n".join(out))


def asm_d(r):
    o = r["obj"]
    if o is not None:
        o = re.sub(rb"ne_nasm_[0-9a-z_]{8}", b"ne_nasm_XXXXXXXX", o)      # (nasm's object names its source: the temp folder, whose name is random)
    return [None if o is None else [len(o), sha1(o)], sha1(norm_asm_out(r["lst"])), sha1(norm_asm_out(r["log"]))]


K.add("consts", lambda: [sorted(I._JMP), sorted(I._JCC), sorted(I._CALL), I._PREAMBLE, I._NLST.pattern])
texts = ["", "jmp    0x1234", "jmp 0x1234", "jmp  0x1234 ", "call   0x20", "je     0xa000", "loop   0xff", "jcxz   0x10", "jecxz  0x10", "jmp    ax",
         "jmp    WORD PTR [bx]", "mov    ax,0x10", "nop", "JMP 0x10", "jmpx   0x10", "ja     0xZZ", "call   0x", "jns    0x7fffffff", "lcall  0x10",
         "jmp\t0x1f", "call 0x0", "retf", "ljmp   0x1:0x2", "jne    0x00ff"]
for t in texts:
    K.add("branch\t" + hx(t), lambda t=t: I._branch(t))

for path, si in C.segs():
    s = seg(path, si)
    tag = "%s\t%d" % (path, si)
    n = len(s.code)
    offs = sorted(set(list(s.funcs)[:12] + list(s.export_names)[:6] + [0, 1, n // 2, n, 0xffff]))
    for off in offs:
        for mode in (0, 1):
            K.add("procname\t%s\t%d\t%d" % (tag, off, mode), lambda off=off, mode=mode: I._procname(ovseg(path, si) if mode else seg(path, si), off))
    fl = sorted(f for f in s.funcs)
    ranges = [(0, n), (n // 3, n // 2)] + sorted((f["start"], f["end"]) for f in s.funcs.values())[:10]
    for lo, hi in ranges:
        for fmode in (0, 1, 2, 3):
            for mode in (0, 1, 2):
                if fmode > 1 and mode == 1:
                    continue

                def f(lo=lo, hi=hi, fmode=fmode, mode=mode):
                    inr = [x for x in fl if lo <= x < hi]
                    funcs = None if fmode == 0 else [] if fmode == 1 else (inr + [lo + 3] if fmode == 2 else (inr + inr[:1]))
                    force = {o for o, _ in s.rows if lo <= o < hi and force_of(o)} if mode == 1 else set()
                    dat = set()
                    if mode == 2:
                        from codebreaker.source.ne_asm import recon
                        for a, b, _k in recon._safe_data_spans(s, lo, hi):
                            dat |= {o for o, _ in s.rows if a <= o < b}
                    text, unres = I._emit(s, lo, hi, funcs, 1, force, dat)
                    return lines_d(text) + set_d(unres)
                K.add("emit\t%s\t%d\t%d\t%d\t%d" % (tag, lo, hi, fmode, mode), f)


def stage():
    tc = I._stage_cmacros(".")
    inc = os.path.join(tc, "inc")
    names = sorted(os.listdir(inc))
    return [os.path.relpath(tc, os.environ["TEMP"]).replace("\\", "/"), [[nm, os.path.getsize(os.path.join(inc, nm))] for nm in names]]


K.add("stage", stage)

# the assemblers on emitted text
small = [(C.MINI + "NOMOUSE.DRV", 1, 0, 45), (C.MINI + "KEYBOARD.DRV", 7, 0, 55), (C.MINI + "KEYBOARD.DRV", 1, 0, 126), (C.MINI + "SOUND.DRV", 1, 0, 600),
         (C.MINI + "VER.DLL", 1, 0, 23), (C.MINI + "LZEXPAND.DLL", 3, 0, 24), (C.SETUP + "SETUP.EXE", 1, 0, 300), (C.MINI + "COMM.DRV", 1, 0, 308)]
for path, si, lo, hi in small:
    s = seg(path, si)
    text = I._emit(s, lo, min(hi, len(s.code)))[0]
    K.add("asm\t%s\t%d\t%d\t%d" % (path, si, lo, hi), lambda text=text: asm_d(I.assemble(text)))
    K.add("nasm\t%s\t%d\t%d\t%d" % (path, si, lo, hi), lambda text=text: asm_d(I.assemble_nasm(text)))
bad = "OPTION M510\n?PLM = 1\ninclude cmacros.inc\nsBegin  CODE\n        nosuchop ax\n        mov ax,\nsEnd    CODE\n        end\n"
K.add("nasm_text\t" + hx(bad), lambda: asm_d(I.assemble_nasm(bad)))
K.add("nasm_text\t" + hx(""), lambda: asm_d(I.assemble_nasm("")))


def nasm_missing():
    old = os.environ.get("CB_NASM")
    os.environ["CB_NASM"] = os.path.join(HERE, "no", "such", "nasm.exe")
    try:
        return asm_d(I.assemble_nasm(bad))
    finally:
        if old is None:
            del os.environ["CB_NASM"]
        else:
            os.environ["CB_NASM"] = old


K.add("nasm_missing\t" + hx(bad), nasm_missing)
K.add("nasm_bin", lambda: [os.path.basename(I._nasm_bin()), os.path.basename(I._nasm_inc())])


# _nasm_divergers on seeded nasm listings
def listing(seed):
    r = random.Random(seed)
    n = r.randrange(0, 30)
    our = bytes(r.randrange(256) for _ in range(r.randrange(20, 200)))
    shipped = bytes(our[i] if r.random() < 0.85 else r.randrange(256) for i in range(len(our)))
    start = r.choice([0, 0x10, 0x100])
    lines, moff, spans, orig = [], 0, {}, start
    for k in range(n):
        ln = r.randrange(1, 6)
        ooff = orig
        olen = ln if r.random() < 0.8 else r.randrange(1, 6)
        spans[ooff] = ooff + olen
        orig += olen
        cmt = " ; %04x" % ooff if r.random() < 0.9 else ""
        shape = r.randrange(5)
        if shape == 0:
            lines.append("%4d %08X %s\t\tnop%s" % (k + 1, moff, "".join("%02X" % r.randrange(256) for _ in range(ln)), cmt))
        elif shape == 1:
            lines.append("%4d <1> %s" % (k + 1, "mov ax,1"))                      # a shim-expansion line: no offset
        elif shape == 2:
            lines.append("%d %08x %02x%s" % (k + 1, moff, r.randrange(256), cmt))
        elif shape == 3:
            lines.append("  %d  %08X  %02X %02X; no comment" % (k + 1, moff, 1, 2))
        else:
            lines.append("%4d                          sBegin CODE")
        moff += r.randrange(0, 6)
    omask = {x for x in range(len(our)) if r.random() < 0.1}
    smask = {x for x in range(len(shipped)) if r.random() < 0.1}
    return "\r\n".join(lines), our, omask, shipped, smask, spans, start


os.makedirs("inputs", exist_ok=True)
for seed in range(60):
    dargs = listing(seed)
    lst, our, omask, shipped, smask, spans, start = dargs
    with open("inputs/div_%d.lst" % seed, "wb") as f:
        f.write(lst.encode("ascii"))
    with open("inputs/div_%d.meta" % seed, "w", newline="\n") as f:
        f.write("%d\n%s\n%s\n%s\n%s\n%s\n" % (start, our.hex() or "~", shipped.hex() or "~", ",".join(map(str, sorted(omask))) or "~",
                                              ",".join(map(str, sorted(smask))) or "~", ",".join("%d:%d" % kv for kv in sorted(spans.items())) or "~"))
    K.add("div\t%d" % seed, lambda dargs=dargs: [sorted(x) for x in I._nasm_divergers(*dargs)])

# reconcile_range / verify_range
for path, si in [(C.MINI + "NOMOUSE.DRV", 1), (C.MINI + "KEYBOARD.DRV", 1), (C.MINI + "KEYBOARD.DRV", 3), (C.MINI + "KEYBOARD.DRV", 7), (C.MINI + "SOUND.DRV", 1),
                 (C.MINI + "COMM.DRV", 1), (C.MINI + "COMM.DRV", 2), (C.MINI + "VER.DLL", 3), (C.MINI + "VER.DLL", 4), (C.MINI + "LZEXPAND.DLL", 2),
                 (C.MINI + "LZEXPAND.DLL", 7), (C.MINI + "MINIKBD.DLL", 1), (C.MINI + "SYSTEM.DRV", 1), (C.MINI + "LMOUSE31.DRV", 1), (C.MINI + "GDI.EXE", 4),
                 (C.MINI + "GDI.EXE", 6), (C.MINI + "KRNL386.EXE", 3), (C.MINI + "USER.EXE", 5), (C.SETUP + "SETUP.EXE", 1), (C.SETUP + "W95INF16.DLL", 1),
                 (C.SETUP + "SUEXPAND.DLL", 3), (C.SETUP + "NETDI.DLL", 4)]:
    s = seg(path, si)
    n = len(s.code)
    funcs = sorted(s.funcs.values(), key=lambda f: (f["end"] - f["start"], f["start"]))
    rs = [(0, n)] + [(f["start"], f["end"]) for f in funcs[:2] if f["end"] > f["start"]]
    ml_ok = n <= 700
    for lo, hi in rs:
        for backend in ("nasm", "ml"):
            if backend == "ml" and not (ml_ok or hi - lo <= 150):
                continue

            def f(lo=lo, hi=hi, backend=backend):
                asm, res, force_db, nc = I.reconcile_range(s, lo, hi, backend=backend)
                return [sha1(asm), res_d(res), sorted(force_db), nc]
            K.add("rec\t%s\t%s\t%d\t%d\t%d" % (backend, path, si, lo, hi), f)

            def g(lo=lo, hi=hi, backend=backend):
                r = I.verify_range(s, lo, hi, backend=backend)
                return [r["ok"], r["n"], r["n_code"], r["db"], r["live"], sha1(r["asm"]), res_d(r.get("res"))]
            K.add("ver\t%s\t%s\t%d\t%d\t%d" % (backend, path, si, lo, hi), g)
s = seg(C.MINI + "KEYBOARD.DRV", 7)
K.add("rec\tnasm\t%s\t7\t0\t55\t1" % (C.MINI + "KEYBOARD.DRV"), lambda: [sha1(x) if isinstance(x, str) else res_d(x) if isinstance(x, dict) else sorted(x) if isinstance(x, set) else x
                                                                       for x in I.reconcile_range(s, 0, 55, funcs=[0, 20], backend="nasm")])
K.add("rec\tnasm\t%s\t7\t0\t55\t2" % (C.MINI + "KEYBOARD.DRV"), lambda: [sha1(x) if isinstance(x, str) else res_d(x) if isinstance(x, dict) else sorted(x) if isinstance(x, set) else x
                                                                       for x in I.reconcile_range(s, 0, 55, funcs=[], backend="nasm")])
K.add("rec\tnasm\t%s\t7\t0\t55\t3" % (C.MINI + "KEYBOARD.DRV"), lambda: [sha1(x) if isinstance(x, str) else res_d(x) if isinstance(x, dict) else sorted(x) if isinstance(x, set) else x
                                                                       for x in I.reconcile_range(s, 0, 55, backend="nasm", max_passes=1)])
K.add("rec\tnasm\t%s\t7\t0\t55\t4" % (C.MINI + "KEYBOARD.DRV"), lambda: [sha1(x) if isinstance(x, str) else res_d(x) if isinstance(x, dict) else sorted(x) if isinstance(x, set) else x
                                                                       for x in I.reconcile_range(s, 0, 55, backend="nasm", max_passes=0)])
# ranges that begin mid-instruction, run past the end of the code
n = len(seg(C.MINI + "NOMOUSE.DRV", 1).code)
for lo, hi in ((1, n), (3, 20), (0, n + 10), (n, n + 5), (10, 10), (20, 5)):
    s = seg(C.MINI + "NOMOUSE.DRV", 1)

    def g(lo=lo, hi=hi):
        r = I.verify_range(s, lo, hi, backend="nasm")
        return [r["ok"], r["n"], r["n_code"], r["db"], r["live"], sha1(r["asm"]), res_d(r.get("res"))]
    K.add("ver\tnasm\t%s\t1\t%d\t%d" % (C.MINI + "NOMOUSE.DRV", lo, hi), g)

# the script block
os.makedirs("out_save", exist_ok=True)
with open("names_nomouse.json", "w") as f:
    f.write('{"1:0": "RenamedStart", "1:7": "9x y", "2:0": "OtherSeg"}')
with open("names_keyboard.json", "w") as f:
    f.write("{bad")
for tag, fn in (("nomouse", "names_nomouse.json"), ("keyboard", "names_keyboard.json")):
    d = os.path.join(os.environ["CB_STATE"], "idiomatic", tag)
    os.makedirs(d, exist_ok=True)
    shutil.copy(fn, os.path.join(d, "names.json"))
IDIOM = os.path.join(os.path.dirname(I.__file__), "idiom.py")


def run_main(save, args):
    old = sys.argv, os.environ.get("NE_SAVE")
    sys.argv = [IDIOM] + args
    if save:
        os.environ["NE_SAVE"] = save
    else:
        os.environ.pop("NE_SAVE", None)
    out, status = io.StringIO(), 0
    try:
        with contextlib.redirect_stdout(out):
            runpy.run_path(IDIOM, run_name="__main__")
    except SystemExit as e:
        status = e.code if isinstance(e.code, int) else (1 if e.code else 0)
    finally:
        sys.argv = old[0]
        if old[1] is None:
            os.environ.pop("NE_SAVE", None)
        else:
            os.environ["NE_SAVE"] = old[1]
    text = out.getvalue().replace(os.environ["CB_STATE"], "<STATE>")
    extra = None
    if save and os.path.exists(save):
        extra = sha1(open(save, "rb").read())
        os.remove(save)
    return [status, text, extra]


P = C.MINI + "NOMOUSE.DRV"
for save, args in (("", []), ("", [P]), ("", [P, "1", "0", "whole"]), ("", [P, "1", "0", "0x10"]), ("out_save/a.asm", [C.MINI + "KEYBOARD.DRV", "7", "0", "whole"]),
                   ("out_save/b.asm", [C.MINI + "KEYBOARD.DRV", "1", "0x0", "0x40"]), ("", [C.MINI + "KEYBOARD.DRV", "1", "0", "whole"]),
                   ("", [C.MINI + "COMM.DRV", "4", "0", "10"]), ("", [C.MINI + "COMM.DRV", "9"]), ("", [P, "x"]), ("", [P, "1", "zz"]), ("", [P, "1", "0", "zz"]),
                   ("", ["nosuchmodule.drv"]), ("", [P, "1", "0x5", "0x20"]), ("", [C.MINI + "VER.DLL", "1", "0", "whole"])):
    K.add("\t".join(["main", hx(save)] + [hx(a) for a in args]), lambda save=save, args=args: run_main(save, args))
K.close()
print(K.n, "cases")
