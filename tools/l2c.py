"""l2c.py -- the hub's C back end held to the decompiler's own C, function by function.

    python tools/l2c.py [NAME ...] [--golden FOLDER] [--show N] [--diff NAME]

Runs the native decompiler with CB_L2=1 on each golden module's frozen description (tools/golden.py)
and, for every function, writes its `/*@L2 {...} */` document back as C with
codebreaker.translate.backends.c and compares that with the C the decompiler printed beside it,
token by token and in its spacing (comments aside, and where PrintC broke a long line: c_text's
passes read spacing).
Per module: functions written back the same, the same but for spacing, different, declined (a node
the back end does not write). docs/IR_UNIFICATION.md step 2: the back end replaces
the printed C where they are the same.

--diff NAME prints one function's two texts where they part."""
import collections
import json
import os
import re
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.decompile import decompiler                 # noqa: E402
from codebreaker.translate.backends import c as cback        # noqa: E402

HEAD = re.compile(r"^/\*@@ (\S+) ", re.M)
NOTE = re.compile(r"^/\*@L2 (\{.*\}) \*/$", re.M)
_TOKEN = re.compile(r"0x[0-9a-fA-F]+|\d+(?:\.\d+)?(?:e[-+]?\d+)?|\w+|'(?:\\.|[^'])*'|\"(?:\\.|[^\"])*\"|"
                    r"->|\+\+|--|<<=|>>=|<<|>>|<=|>=|==|!=|&&|\|\||[-+*/%&|^]=|\S")


def tokens(text):
    """The C's tokens, its comments and the decompiler's notes left out."""
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    return _TOKEN.findall(text)


def functions(out):
    """{name: (L2 document or None, the printed C)} from the decompiler's output."""
    found = {}
    heads = list(HEAD.finditer(out))
    for i, h in enumerate(heads):
        chunk = out[h.start():heads[i + 1].start() if i + 1 < len(heads) else len(out)]
        note = NOTE.search(chunk)
        found[h.group(1)] = (json.loads(note.group(1)) if note else None, chunk)
    return found


def _uncommented(text):
    return re.sub(r"/\*.*?\*/", " ", text, flags=re.S).strip()


def spaced_alike(mine, printed):
    """Is `mine` spaced as `printed` is, wherever printed is on one line? c_text's passes read
    spacing (`  xunknown1 axStack_4c [52];`), not only tokens. Where PrintC breaks a long line, the
    break stands either for a space (`a, b` in a condition's list) or for none (`f(a,b)`): either is
    taken there, and only there."""
    pieces = re.split(r"[ \t]*\n\s*", _uncommented(printed))
    pattern = " ?".join(re.escape(" ".join(p.split())) for p in pieces)
    return re.fullmatch(pattern, " ".join(_uncommented(mine).split())) is not None


def layout(text):
    """The C on one line, comments left out, every run of white space one space (for showing where
    two texts part)."""
    return " ".join(_uncommented(text).split())


def compare(doc, printed):
    """('same' | 'same tokens' | 'different' | 'declined: why' | 'no L2', the back end's C or '')
    'same': line for line as printed; 'same tokens': only the spacing differs."""
    if doc is None:
        return "no L2", ""
    try:
        mine = cback.function(doc)
    except cback.Decline as e:
        return "declined: %s" % e, ""
    if tokens(mine) != tokens(printed):
        return "different", mine
    return ("same" if spaced_alike(mine, printed) else "same tokens"), mine


def first_parting(a, b, around=8):
    ta, tb = tokens(a), tokens(b)
    i = 0
    while i < min(len(ta), len(tb)) and ta[i] == tb[i]:
        i += 1
    return " ".join(ta[max(0, i - around):i + around]), " ".join(tb[max(0, i - around):i + around])


def check(name, folder, show, diff=None):
    snap = json.load(open(os.path.join(folder, name, "golden.json")))
    os.environ["CB_L2"] = "1"
    t0 = time.time()
    tally, partings = collections.Counter(), collections.Counter()
    examples = {}
    for member, m in snap.items():
        out, _err, _whole = decompiler._run_resuming(m["description"], (), 3600)
        for fn, (doc, printed) in functions(out).items():
            if "decompile failed" in printed:
                continue
            verdict, mine = compare(doc, printed)
            tally[verdict.split(":")[0]] += 1
            if verdict.startswith("declined"):
                partings[verdict] += 1
            elif verdict == "same tokens":
                la, lb = layout(mine), layout(printed)
                k = next((i for i, (x, y) in enumerate(zip(la, lb)) if x != y), min(len(la), len(lb)))
                p = "spacing: %r  |  %r" % (la[max(0, k - 25):k + 15], lb[max(0, k - 25):k + 15])
                partings[p] += 1
                examples.setdefault(p, fn)
            elif verdict == "different":
                a, b = first_parting(mine, printed, 3)
                partings["%s  |  %s" % (a, b)] += 1
                examples.setdefault("%s  |  %s" % (a, b), fn)
            if diff == fn:
                print(mine)
                print("----- printed")
                print(re.sub(r"^/\*@L2 .*$", "", printed, flags=re.M))
                print("----- parting:\n  %s\n  %s" % first_parting(mine, printed, 12))
    total = sum(tally.values())
    print("%-10s %5d functions  %5d same  %5d same tokens  %5d different  %5d declined  %4d no L2  (%.0fs)" % (
        name, total, tally["same"], tally["same tokens"], tally["different"], tally["declined"], tally["no L2"],
        time.time() - t0),
        flush=True)
    for p, k in partings.most_common(show):
        print("      %4d  %s   [%s]" % (k, p, examples.get(p, "")))
    return tally


def main(argv):
    folder = argv[argv.index("--golden") + 1] if "--golden" in argv else os.path.join(ROOT, "state", "golden")
    show = int(argv[argv.index("--show") + 1]) if "--show" in argv else 8
    diff = argv[argv.index("--diff") + 1] if "--diff" in argv else None
    skip = {folder, str(show), diff}
    names = [a for a in argv if not a.startswith("--") and a not in skip] or sorted(
        n for n in os.listdir(folder) if os.path.isfile(os.path.join(folder, n, "golden.json")))
    total = collections.Counter()
    for name in names:
        total.update(check(name, folder, show, diff))
    n = sum(total.values())
    print("all: %d functions, %d written back the same (%.1f%%), %d with the same tokens, %d different, %d declined" % (
        n, total["same"], 100.0 * total["same"] / max(n, 1), total["same tokens"], total["different"], total["declined"]))
    return 0 if n and total["same"] == n else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
