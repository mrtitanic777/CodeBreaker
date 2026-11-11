"""expected.py -- the ORIGINAL codebreaker.source.ne_asm.recon on REAL NE modules (state/mini, state/setup) and seeded inputs.

Run as `python expected.py` from this folder (CB_WT names the worktree whose Python is imported; unset: the main checkout).
Writes ORDINALS.md, cases.txt (for test.cpp) and expected.txt.  TEMP is the tmp/ folder next to this file.
Cases (tab separated; text fields are hex of UTF-8, '~' empty):
  consts                         JMPCC, RETF_MACRO, the constants
  hex INT                        masm_hex(INT)
  tomasm TEXT MODE               to_masm(text, is_target)   MODE 0: no label, 1: a label for an even target
  tomasm_rows PATH SEG MODE      to_masm over every row of a real segment: [n, sha1]
  dbline HEXBYTES OFF NOTE       _dbline;  emit_db / emit_data (kind) likewise on the bytes
  sds PATH SEG LO HI             _safe_data_spans(Seg, lo, hi)
  emit PATH SEG LO HI MODE       emit_function(Seg, lo, hi, force_db, spans): MODE 0 none, 1 force pattern, 2 pattern + safe spans
  wrap N                         wrap of N seeded lines
  div SEED                       _divergers on a seeded listing, bytes and masks
  recon PATH SEG LO HI [PASSES]  reconcile(Seg, lo, hi, set()[, PASSES]) with the DDK-era MASM 4.00 on the native DOS executive
  pick                           _pick()
  main ARG...                    main() with sys.argv = ["asmrecon.py"] + ARG (args as hex): [status, stdout] (stdout only where
                                 the status is 0; the stderr texts are not compared)
"""
import contextlib
import io
import os
import random
import shutil
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
os.chdir(HERE)
os.environ["TEMP"] = os.path.join(HERE, "tmp")
os.environ["TMP"] = os.path.join(HERE, "tmp")
shutil.rmtree(os.environ["TEMP"], ignore_errors=True)
os.makedirs(os.environ["TEMP"], exist_ok=True)
os.environ["CB_STATE"] = os.path.join(HERE, "state_py")      # (what the tools keep -- caches, work files -- stays in this folder)
shutil.rmtree(os.environ["CB_STATE"], ignore_errors=True)
os.makedirs(os.environ["CB_STATE"], exist_ok=True)
sys.path.insert(0, os.path.join(os.path.dirname(HERE), "_ne_common"))
import common as C  # noqa: E402
from common import hx, sha1  # noqa: E402

from codebreaker.formats import ne as ne_mod  # noqa: E402
from codebreaker.source.ne_asm import recon as R  # noqa: E402
from codebreaker.source.ne_asm import segment as trace  # noqa: E402
from codebreaker.source.ne_asm import segdiff  # noqa: E402

C.make_ordinals()
ORD = trace.load_ordinals("ORDINALS.md")
K = C.Cases(HERE)
rnd = random.Random(1111)
_segs = {}


def seg(path, si):
    k = (path, si)
    if k not in _segs:
        _segs[k] = trace.Seg(ne_mod.NEFile(path), si, ORD)
    return _segs[k]


def lines_d(lines):
    return [len(lines), sha1("\n".join(lines))]


def force_of(off):
    return (off * 37 + 11) % 5 == 0


def target_of(mode):
    return (lambda o: ("L_%04x" % o) if o % 2 == 0 else None) if mode else (lambda o: None)


def res_d(res):
    if res is None:
        return None
    return [res["exact"], res["length_match"], res["our_len"], res["shipped_len"], res["n_diff"], res["first_diff"], res["masked"]]


K.add("consts", lambda: [sorted(R.JMPCC), R.RETF_MACRO])
for v in (0, 1, 9, 10, 11, 15, 16, 0xab, 0xff, 0x100, 0xabc, 0x1234, 0xa000, 0xffff, 0xdeadbeef, 0x7fffffff, 0xfffffffff):
    K.add("hex\t%d" % v, lambda v=v: R.masm_hex(v))

# to_masm: hand-picked texts (objdump Intel syntax as segment.py makes them) and the real rows
texts = ["", " ", "nop", "ret", "retf", "retf 0x4", "push bp", "mov bp,sp", "mov    ax,WORD PTR [bp+0x6]", "mov    BYTE PTR [bx+si],al",
         "mov    DWORD PTR [bx],eax", "lcall  FWORD PTR [bx]", "rep movs WORD PTR es:[di],WORD PTR ds:[si]", "rep stos BYTE PTR es:[di],al",
         "repz cmps BYTE PTR ds:[si],BYTE PTR es:[di]", "repne scas al,BYTE PTR es:[di]", "lock xchg ax,bx", "lock", "rep", "repz",
         "lods   al,BYTE PTR ds:[si]", "lods   ax,WORD PTR ds:[si]", "stos   WORD PTR es:[di],ax", "movs   BYTE PTR es:[di],BYTE PTR ds:[si]",
         "jmp    0x1234", "jmp 0x1234", "call   0x20", "je     0xa000", "loop   0xff", "jcxz   0x10", "jmp    ax", "jmp    WORD PTR [bx]",
         "mov    ax,0xffff", "mov    ax,0xabc", "mov    ax,0x0", "add    sp,0xa", "cmp    ax,0x9", "int    0x21", "in     al,0x60", "mov    es:0x1234,ax",
         "jmp    0xZZ", "call   0x", "JMP 0x10", "mov  ax,  0x1f  ", "é 0x1f", "mov ax,0x123456789abcdef"]
for t in texts:
    for mode in (0, 1):
        K.add("tomasm\t%s\t%d" % (hx(t), mode), lambda t=t, mode=mode: R.to_masm(t, target_of(mode)))

for path, si in C.segs():
    s = seg(path, si)
    tag = "%s\t%d" % (path, si)
    for mode in (0, 1):
        K.add("tomasm_rows\t%s\t%d" % (tag, mode), lambda: lines_d([R.to_masm(a, target_of(mode)) for _o, a in s.rows]))
    n = len(s.code)
    K.add("sds\t%s\t0\t%d" % (tag, n), lambda: R._safe_data_spans(s, 0, n))
    K.add("sds\t%s\t%d\t%d" % (tag, n // 3, n // 2), lambda: R._safe_data_spans(s, n // 3, n // 2))
    ranges = [(0, n)] + sorted((f["start"], f["end"]) for f in s.funcs.values())[:40]
    for lo, hi in ranges:
        for mode in (0, 1, 2):
            def f(lo=lo, hi=hi, mode=mode):
                force = {o for o, _ in s.rows if lo <= o < hi and force_of(o)} if mode else set()
                spans = R._safe_data_spans(s, lo, hi) if mode == 2 else ()
                return lines_d(R.emit_function(s, lo, hi, force, spans))
            K.add("emit\t%s\t%d\t%d\t%d" % (tag, lo, hi, mode), f)

# the db / data emitters on seeded bytes
for i in range(40):
    raw = bytes(rnd.randrange(256) for _ in range(rnd.choice([0, 1, 2, 5, 15, 16, 17, 33, 100])))
    if i % 4 == 0:                                   # text runs, with the quote and non-printables
        raw = bytes(rnd.choice(b"abc xyz'.\x00\x1f\x7fHELLO12") for _ in range(rnd.randrange(0, 120)))
    off = rnd.randrange(0, 0x10000)
    note = rnd.choice(["", "data", "retf", "mov ax,bx", "é"])
    K.add("dbline\t%s\t%d\t%s" % (raw.hex() or "~", off, hx(note)), lambda: R._dbline(raw, off, note))

    def lines():
        out = []
        R._emit_db(out, raw, off, note)
        return out
    K.add("emitdb\t%s\t%d\t%s" % (raw.hex() or "~", off, hx(note)), lines)
    for kind in ("str", "data"):
        def dl(kind=kind):
            out = []
            R._emit_data(out, raw, 0, len(raw), kind)
            return out
        K.add("emitdata\t%s\t%s" % (raw.hex() or "~", kind), dl)
for n in (0, 1, 3, 20):
    ls = ["        db      01h" + " " * i for i in range(n)]
    K.add("wrap\t%d" % n, lambda ls=ls: R.wrap(ls))


# _divergers on seeded listings
def listing(seed):
    r = random.Random(seed)
    n = r.randrange(0, 30)
    our = bytes(r.randrange(256) for _ in range(r.randrange(20, 200)))
    shipped = bytes(our[i] if r.random() < 0.85 else r.randrange(256) for i in range(len(our)))
    start = r.choice([0, 0x10, 0x100])
    lines, moff, spans, orig = [], 0, {}, start
    for _ in range(n):
        ln = r.randrange(1, 6)
        ooff = orig
        olen = ln if r.random() < 0.8 else r.randrange(1, 6)
        spans[ooff] = ooff + olen
        orig += olen
        cmt = " ; %04x" % ooff if r.random() < 0.9 else ""
        shape = r.randrange(5)
        if shape == 0:
            lines.append("%04X  %s\t\tnop%s" % (moff, " ".join("%02X" % r.randrange(256) for _ in range(ln)), cmt))
        elif shape == 1:
            lines.append("%04X  %s" % (moff, "L_%04x:" % ooff))                  # a label line: no bytefield
        elif shape == 2:
            lines.append("  %04X  %02X %s" % (moff, r.randrange(256), cmt))
        elif shape == 3:
            lines.append("%04X  %02X\t\tdb 01h," % (moff, 1))
            lines.append("      0001 ; %04x" % ooff)                              # the comment wrapped to the next line
        else:
            lines.append("%04X 90%s" % (moff, cmt))
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
    K.add("div\t%d" % seed, lambda dargs=dargs: [sorted(x) for x in R._divergers(*dargs)])

# reconcile (the tool lane: MASM 4.00 on the native DOS executive): small real functions and segments
small = [(C.MINI + "NOMOUSE.DRV", 1), (C.MINI + "KEYBOARD.DRV", 1), (C.MINI + "KEYBOARD.DRV", 7), (C.MINI + "SOUND.DRV", 1),
         (C.MINI + "COMM.DRV", 1), (C.MINI + "VER.DLL", 1), (C.MINI + "VER.DLL", 4), (C.MINI + "LZEXPAND.DLL", 3), (C.SETUP + "SETUP.EXE", 1)]
for path, si in small:
    s = seg(path, si)
    _sb, smask = segdiff.shipped_ne_segment(path, si)
    funcs = sorted(s.funcs.values(), key=lambda f: (f["end"] - f["start"], f["start"]))
    rs = [(0, len(s.code))] + [(f["start"], f["end"]) for f in funcs[:3] if f["end"] > f["start"]]
    for lo, hi in rs:
        def f(lo=lo, hi=hi):
            asm, res, force_db, n = R.reconcile(s, lo, hi, smask)
            return [sha1(asm), res_d(res), sorted(force_db), n]
        K.add("recon\t%s\t%d\t%d\t%d" % (path, si, lo, hi), f)
s = seg(C.MINI + "NOMOUSE.DRV", 1)
_sb, smask = segdiff.shipped_ne_segment(C.MINI + "NOMOUSE.DRV", 1)
for passes in (0, 1):
    def f(passes=passes):
        asm, res, force_db, n = R.reconcile(s, 0, len(s.code), smask, passes)
        return [sha1(asm), res_d(res), sorted(force_db), n]
    K.add("recon\t%s\t1\t0\t%d\t%d" % (C.MINI + "NOMOUSE.DRV", len(s.code), passes), f)
K.add("pick", lambda: list(R._pick()))


def run_main(args):
    old = sys.argv
    sys.argv = ["asmrecon.py"] + args
    out, status = io.StringIO(), 0
    try:
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(io.StringIO()):
            R.main()
    except SystemExit as e:
        status = e.code if isinstance(e.code, int) else (1 if e.code else 0)
    finally:
        sys.argv = old
    text = out.getvalue()
    extra = None
    for a in args:
        b = a.split("=")[-1]
        if b.startswith("out_") and os.path.exists(b):      # the --out file
            extra = sha1(open(b, "rb").read())
            os.remove(b)
    return [status, text if status == 0 else "", extra]


P = C.MINI + "NOMOUSE.DRV"
K1 = C.MINI + "KEYBOARD.DRV"
for args in ([P], [P, "--seg", "1"], [K1, "--seg", "1", "--func", "0x0"], [C.MINI + "KEYBOARD.DRV", "--seg=7", "--out", "out_1.asm"], [P, "--out=out_2.asm"],
             ["--selftest"], [], [P, "extra"], [P, "--seg"], [P, "--seg", "x"], [P, "--func", "zz"], [P, "--nosuch"], [P, "--func", "0x7777"],
             [C.MINI + "COMM.DRV", "--seg", "9"], ["nosuchmodule.drv"], [P, "--seg", "-1"], [K1, "--func", "-3"], [C.MINI + "VER.DLL", "--seg", "1"]):
    K.add("\t".join(["main"] + [hx(a) for a in args]), lambda args=args: run_main(args))
K.close()
print(K.n, "cases")
