"""sample_shapes_cases.py OUT -- a stratified sample of the exact.draft.shapes harness (the draft bodies of texts.txt and the
operations of cases.txt), run through the ORIGINAL Python module (codebreaker/exact/draft/shapes.py), written as
native/cb/exact/testdata/draft_shapes.cases: one case per line,

    op <TAB> S<text> [<TAB> S<extra> ...] <TAB> => <TAB> {"r": result} | {"same": true}  [with ,"t":{tables} inside]

The text escapes \\ \\t \\n \\r. Per operation: up to CHANGED draft bodies the pass rewrites and one it leaves alone (short
bodies only, so the file stays small), plus the whole ternary / pair_long families of the harness. `t` is what the pass
recorded of the function (UCHAR, PTRSIZE, LONGINFO) as expected.py wrote it.

Run from the checkout whose codebreaker/ package is the Python to sample (the harness folder is read-only input).
"""
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", ".."))
sys.path.insert(0, ROOT)
from codebreaker.exact.draft import shapes as S  # noqa: E402
from codebreaker.exact.draft import text as T  # noqa: E402

H = "C:/Users/mrtit/Documents/GitHub/CodeBreaker/state/work/typed-port/exact.draft.shapes/"
OUT = sys.argv[1]
CHANGED = 5
MAX_LEN = 900
BS, TB, NL, CR = chr(92), chr(9), chr(10), chr(13)


def esc(s):
    return s.replace(BS, BS + BS).replace(TB, BS + "t").replace(NL, BS + "n").replace(CR, BS + "r")


def unesc(s):
    out, i = [], 0
    while i < len(s):
        if s[i] == BS and i + 1 < len(s):
            out.append({"t": TB, "n": NL, "r": CR}.get(s[i + 1], s[i + 1]))
            i += 2
        else:
            out.append(s[i])
            i += 1
    return "".join(out)


texts = [unesc(l) for l in open(H + "texts.txt", "rb").read().decode("utf-8").replace("\r\n", "\n").split("\n")]
cases = [c for c in open(H + "cases.txt", "rb").read().decode("utf-8").replace("\r\n", "\n").split("\n") if c.strip()]

RANGE_F = {(lo, lo + d, False) for lo in range(6) for d in range(5)}
RANGE_ALL = RANGE_F | {(lo, lo + d, True) for lo in range(6) for d in range(5)}
OPD = {}
for name in dir(S):
    if not name.startswith("__") and callable(getattr(S, name)):
        OPD[name] = (getattr(S, name), 0)
OPD["range_switches_f"] = (S.range_switches, 1)
OPD["range_switches_all"] = (S.range_switches, 2)
OPD["goto_tails1"] = (lambda t: S.goto_tails(t, 1), 0)
OPD["unsigned_fields1"] = (lambda t: S.unsigned_fields(t, 1), 0)
OPD["adjacent_swaps1"] = (lambda t: S.adjacent_swaps(t, 1), 0)


def tables():
    t = {}
    if len(T.UCHAR):
        t["u"] = sorted(json.dumps(list(x) if isinstance(x, tuple) else x, separators=(",", ":")) for x in T.UCHAR)
    if len(T.PTRSIZE):
        t["p"] = sorted([k, v] for k, v in T.PTRSIZE.items())
    li = {}
    if "fields" in T.LONGINFO:
        li["f"] = sorted(T.LONGINFO["fields"])
    if "globals" in T.LONGINFO:
        li["g"] = sorted(T.LONGINFO["globals"])
    if li:
        t["l"] = li
    return t


def clear():
    T.UCHAR.clear()
    T.PTRSIZE.clear()
    T.LONGINFO.clear()
    T.RANGES.clear()


def tojson(o):
    return json.dumps(o, separators=(",", ":"), ensure_ascii=True)


def run(op, text, extra):
    clear()
    if op == "ternary":
        r = S._ternary(*extra)
    elif op == "pairlong":
        r = S._pair_long(*extra)
    else:
        fn, rk = OPD[op]
        if rk == 1:
            T.RANGES.update(RANGE_F)
        elif rk == 2:
            T.RANGES.update(RANGE_ALL)
        r = fn(text)
    body = '{"same":true}' if isinstance(r, str) and text is not None and r == text else tojson({"r": r})
    tb = tables()
    if tb:
        body = body[:-1] + ',"t":' + tojson(tb) + "}"
    return body, r


by_op = {}
for c in cases:
    f = c.split(TB)
    by_op.setdefault(f[0], []).append((int(f[1]), [unesc(x) for x in f[2:]]))

lines = []
for op, items in by_op.items():
    if op in ("ternary", "pairlong"):
        step = 1 if op == "pairlong" else max(1, len(items) // 40)
        for idx, extra in items[::step]:
            body, _ = run(op, None, extra)
            lines.append(TB.join([op] + ["S" + esc(x) for x in extra] + ["=>", body]))
        continue
    changed, same = [], None
    seen = set()
    for idx, extra in items:
        t = texts[idx]
        if len(t) > MAX_LEN or t in seen:
            continue
        seen.add(t)
        try:
            body, r = run(op, t, extra)
        except Exception:      # (a draft the pass cannot read: no case)
            continue
        if body.startswith('{"same"') and not body.endswith('"t"'):
            if same is None:
                same = (t, body)
        elif r not in (None, []):
            changed.append((t, body))
        if len(changed) >= CHANGED * 6 and same:
            break
    pick = changed[::max(1, len(changed) // CHANGED)][:CHANGED]
    for t, body in pick + ([same] if same else []):
        lines.append(TB.join([op, "S" + esc(t), "=>", body]))
open(OUT, "w", encoding="utf-8", newline="\n").write("\n".join(lines) + "\n")
print(len(lines), "cases")
