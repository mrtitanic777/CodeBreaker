"""gen_switch_samples.py -- how switch_samples.txt was made (kept for provenance; not run by the tests).

Reads the Xbox 360 image of Minecraft (C:/Nexia360/Games/Minecraft Xbox 360 Edition/default.xex) through the
original Python (codebreaker.recompile.switches and .jumptable), picks real switch tables of every kind the scanner
knows plus the bctrs it leaves unresolved or calls pointer calls, and writes for each a *window* of the image (the
code around the bctr, the table's data) together with what the original produced on exactly that window. The C++
tests rebuild an image from the windows and must give the same answers.

    PYTHONDONTWRITEBYTECODE=1 python gen_switch_samples.py <CodeBreaker checkout> <out file>

The expected values were reviewed by hand against the counts the whole-image scan gives (Minecraft: 213 tables,
98+58+48 guarded byte/short/absolute, 4+2+3 unguarded, 4 unresolved, 12 pointer calls).
"""
import sys

ROOT, OUT = sys.argv[1], sys.argv[2]
sys.path.insert(0, ROOT)
from codebreaker.recompile import jumptable as J          # noqa: E402
from codebreaker.recompile import switches as S           # noqa: E402
from codebreaker.formats import powerpc_image, xex        # noqa: E402

XEX = r"C:\Nexia360\Games\Minecraft Xbox 360 Edition\default.xex"
BEFORE, AFTER = 0x300, 0x100     # bytes of code kept before / after the bctr
CODE_BEFORE = 0x100              # the scanned code range starts here: nothing scanned reads before the window


def hexs(b):
    return b.hex()


image = powerpc_image.read(xex.basefile(XEX) if xex.is_xex(XEX) else XEX)
lo, hi = image.code
data = image.bytes_at(image.base, hi - image.base)
img = S.Image(data, image.base)
funcs = sorted((b, b + n) for b, (n, _p, _h) in image.extents.items())
full = S.Scanner(img, lo, hi)
tables, report = S.find(XEX)


def shape_of(t):
    """(kind, size, table, base, shift) of a guarded table, by the module's own steps."""
    guard = full.find_guard(t["bctr"])
    mt = t["bctr"] - 4
    while full.insn(mt)[0] != "mtctr":
        mt -= 4
    return S.table_shape(full.eval_reg(full.insn(mt)[1], mt, guard))


def pick(pred, n):
    return [t for t in tables if pred(t)][:n]


chosen = []
for kind in ("absolute", "byte", "short"):
    chosen += [(f"guarded_{kind}_{i}", t) for i, t in enumerate(pick(lambda t, k=kind: t["kind"] == k and t["guard"] is not None, 3))]
    chosen += [(f"unguarded_{kind}_{i}", t) for i, t in enumerate(pick(lambda t, k=kind: t["kind"] == k and t["guard"] is None and not t.get("inline"), 2))]
chosen += [(f"inline_{i}", t) for i, t in enumerate(pick(lambda t: t.get("inline"), 2))]
others = [(f"unresolved_{i}", a) for i, (a, _m) in enumerate(report["unresolved"][:3])]
others += [(f"pointer_call_{i}", a) for i, a in enumerate(report["pointer_calls"][:2])]

SIZE = {"absolute": 4, "byte": 1, "short": 2}


def run_case(name, bctr, table_window):
    windows = [(bctr - BEFORE, data[bctr - BEFORE - image.base: bctr + AFTER - image.base])]
    if table_window:
        a, n = table_window
        windows.append((a, data[a - image.base: a - image.base + n]))
    fn = S.containing(funcs, bctr)
    base = min(a for a, _ in windows)
    end = max(a + len(b) for a, b in windows)
    buf = bytearray(end - base)
    for a, b in windows:
        buf[a - base: a - base + len(b)] = b
    code = (bctr - CODE_BEFORE, bctr + 4)      # the scanned code ends at the bctr under test
    import bisect
    k = bisect.bisect_right(funcs, (bctr, 0xFFFFFFFF))
    wfuncs = funcs[max(0, k - 2): k + 2]     # the functions around the bctr, as the real scan saw them
    # the original, over exactly this window
    wimg = S.Image(bytes(buf), base)
    sc = S.Scanner(wimg, code[0], code[1])
    found, unresolved, pointers = [], [], []
    for a in range(code[0], code[1], 4):
        if wimg.u32(a) != 0x4E800420:
            continue
        try:
            t = sc.analyze(a)
        except S.Unresolved as e:
            unresolved.append(a)
            continue
        if t is None:
            try:
                t = sc.analyze_unguarded(a, S.extent(wfuncs, a, code[0], code[1]))
            except S.Unresolved:
                return None      # the original loses the whole scan here (an unguarded table running off the window)
            if t is None:
                if S.indexed_load_before(wimg, a, code[0]):
                    pointers.append(a)
                continue
        t["func"] = S.containing(wfuncs, a)
        found.append(t)
    outside = [t["bctr"] for t in found if t["func"] and any(not (t["func"][0] <= l <= t["func"][1]) for l in t["labels"])]
    # the recompiler's recogniser, over the same bytes
    mem = lambda size: (lambda addr: None if addr < base or addr + size > end or not any(a <= addr and addr + size <= a + len(b) for a, b in windows)
                        else int.from_bytes(buf[addr - base: addr - base + size], "big"))
    exec_lo, exec_hi = (fn if fn else (code[0], code[1]))
    insns = [(a, wimg.u32(a), "", "") for a in range(code[0], code[1], 4)]
    jt = J.detect_jump_tables(insns, exec_lo, exec_hi, mem(1), mem(2), mem(4), lambda a: exec_lo <= a < exec_hi)
    lines = [f"case {name}"]
    for a, b in windows:
        lines.append(f"window {a:X} {b.hex()}")
    lines.append(f"code {code[0]:X} {code[1]:X}")
    for f in wfuncs:
        lines.append(f"func {f[0]:X} {f[1]:X}")
    lines.append(f"exec {exec_lo:X} {exec_hi:X}")
    for t in found:
        fnr = f"{t['func'][0]:X}-{t['func'][1]:X}" if t["func"] else "-"
        g = f"{t['guard']:X}" if t["guard"] is not None else "-"
        lines.append("table %X r=%d kind=%s default=%X guard=%s table=%X inline=%d func=%s labels=%s" % (
            t["bctr"], t["r"], t["kind"], t["default"], g, t.get("table") or 0, 1 if t.get("inline") else 0, fnr,
            ",".join("%X" % l for l in t["labels"])))
    lines.append("report unresolved=%s outside=%s pointers=%s" % (
        ",".join("%X" % a for a in unresolved) or "-", ",".join("%X" % a for a in outside) or "-",
        ",".join("%X" % a for a in pointers) or "-"))
    for a, (reg, targets) in sorted(jt.items()):
        lines.append("jt %X reg=%d targets=%s" % (a, reg, ",".join("%X" % t for t in targets)))
    lines.append("end")
    return lines


out = ["# Real switch tables of Minecraft (Xbox 360 Edition) with what the original Python found in each window.",
       "# Made by gen_switch_samples.py; see there. Addresses and bytes in hex.", ""]
def case_for(name, t):
    if t["guard"] is None:
        size = SIZE[t["kind"]]
        tw = (t["table"], size * (len(t["labels"]) + 16))
    else:
        kind, size, table, _b, _s = shape_of(t)
        tw = (table, size * (len(t["labels"]) + 16))
    if t.get("inline"):
        tw = None      # the data follows the bctr: inside the code window
    return run_case(name, t["bctr"], tw)


kept = 0
for name, t in chosen:
    lines = case_for(name, t)
    if lines is None:
        print("skipped", name)
        continue
    out += lines + [""]
    kept += 1
for name, a in others:
    lines = run_case(name, a, None)
    if lines is None:
        print("skipped", name)
        continue
    out += lines + [""]
    kept += 1
open(OUT, "w", newline="\n").write("\n".join(out))
print(kept, "cases")
