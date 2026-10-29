"""l2c.py -- the hub's C back end held to the decompiler's own C, function by function.

    python tools/l2c.py [NAME ...] [--golden FOLDER] [--show N] [--diff NAME]

Runs the native decompiler with CB_L2=1 on each golden module's frozen description (tools/golden.py)
and, for every function, writes its `/*@L2 {...} */` document back as C with
codebreaker.translate.backends.c and compares that with the C the decompiler printed beside it,
character for character: its comments and where it broke a long line too (c_text's passes read
spacing and lines).
Per module: functions written back the same, the same tokens but laid out otherwise (spacing, line
breaks, comments), different, declined (a node the back end does not write).
docs/IR_UNIFICATION.md step 2: the back end replaces the printed C where they are the same.

--diff NAME prints one function's two texts where they part.

    python tools/l2c.py --made MODULE ... [--show N] [--record]

the whole way to compilable C (c_text) for corpus modules, once from the printed C and once from
the back end's (CB_C_FROM_L2), each function's final text compared: the gate for moving a c_text
pass onto L2 (docs/IR_UNIFICATION.md step 3).

--record keeps the printed side's texts (state/l2c/<module>.<sha256>.pickle) as the reference:
once a pass's c_text version is deleted the printed path cannot make them again, and --made then
holds the C from L2 to that record (the printed path's last word, made before the deletion)."""
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
    """{name: (L2 document or None, the printed C)} from the decompiler's output: the C after its
    notes, as PrintC wrote it (docFunction: its first line break left out, and the empty line
    after it)."""
    found = {}
    heads = list(HEAD.finditer(out))
    for i, h in enumerate(heads):
        chunk = out[h.start():heads[i + 1].start() if i + 1 < len(heads) else len(out)]
        note = NOTE.search(chunk)
        printed = chunk[note.end() + 2:] if note and chunk[note.end():note.end() + 2] == "\n\n" else chunk
        if printed.endswith("\n\n"):
            printed = printed[:-1]
        found[h.group(1)] = (json.loads(note.group(1)) if note else None, printed)
    return found


def compare(doc, printed):
    """('same' | 'same tokens' | 'different' | 'declined: why' | 'no L2', the back end's C or '')
    'same': character for character as printed; 'same tokens': laid out otherwise."""
    if doc is None:
        return "no L2", ""
    try:
        mine = cback.function(doc)
    except cback.Decline as e:
        return "declined: %s" % e, ""
    if tokens(mine) != tokens(printed):
        return "different", mine
    return ("same" if mine == printed else "same tokens"), mine


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
                k = next((i for i, (x, y) in enumerate(zip(mine, printed)) if x != y), min(len(mine), len(printed)))
                p = "layout: %r  |  %r" % (mine[max(0, k - 30):k + 15], printed[max(0, k - 30):k + 15])
                partings[p] += 1
                examples.setdefault(p, fn)
            elif verdict == "different":
                a, b = first_parting(mine, printed, 3)
                partings["%s  |  %s" % (a, b)] += 1
                examples.setdefault("%s  |  %s" % (a, b), fn)
            if diff == fn:
                print(mine)
                print("----- printed")
                print(printed)
                print("----- parting:\n  %s\n  %s" % first_parting(mine, printed, 12))
    total = sum(tally.values())
    print("%-10s %5d functions  %5d same  %5d same tokens  %5d different  %5d declined  %4d no L2  (%.0fs)" % (
        name, total, tally["same"], tally["same tokens"], tally["different"], tally["declined"], tally["no L2"],
        time.time() - t0),
        flush=True)
    for p, k in partings.most_common(show):
        print("      %4d  %s   [%s]" % (k, p, examples.get(p, "")))
    return tally


# ---------------------------------------------------------------------------------------------- --made
# The whole way to compilable C: a 16-bit module through c_text.compilable16, a 32-bit one through
# compilable32 -- once from the C the decompiler printed, once from the back end's (CB_C_FROM_L2) --
# each in a process of its own, so nothing kept in memory crosses from one to the other.
_MADE = r"""
import os, pickle, sys, tempfile
sys.path.insert(0, %r)
from codebreaker.base import corpus
from codebreaker.decompile import c_text
from codebreaker.verify import reconstruct
path = corpus.retail(%r) or %r
work = tempfile.mkdtemp(prefix="l2made_")
if reconstruct.kind_of(path).KIND == "ne":
    m = c_text.compilable16(path, work)
    texts = dict(m.texts)
else:
    from codebreaker.verify.lanes import win32
    texts = c_text.compilable32(path, win32.LANES["pe"].functions(path), work)[1]
pickle.dump(texts, open(%r, "wb"))
import shutil
shutil.rmtree(work, ignore_errors=True)
"""


def made(module, l2):
    """{function: its compilable C}, from the printed C or (l2) from the back end's."""
    import pickle
    import subprocess
    import tempfile
    import shutil
    folder = tempfile.mkdtemp(prefix="l2made_")
    out = os.path.join(folder, "texts.pickle")
    env = dict(os.environ)
    env.pop("CB_C_FROM_L2", None)
    if l2:
        env["CB_C_FROM_L2"] = "1"
    try:
        r = subprocess.run([sys.executable, "-c", _MADE % (ROOT, module, module, out)], env=env,
                           capture_output=True, text=True, errors="replace")
        if r.returncode:
            raise RuntimeError("%s: %s" % (module, (r.stderr or r.stdout).strip().splitlines()[-1:]))
        with open(out, "rb") as f:
            return pickle.load(f)
    finally:
        shutil.rmtree(folder, ignore_errors=True)       # (nothing of a run is left behind)


def _record_path(module):
    from codebreaker.base import corpus, settings
    import hashlib
    path = corpus.retail(module) or module
    with open(path, "rb") as f:
        sha = hashlib.sha256(f.read()).hexdigest()
    return settings.state("l2c", "%s.%s.pickle" % (os.path.basename(module).lower(), sha[:16]))


def reference(module, record=False):
    """The printed side: kept (--record makes it now, from the printed path, and keeps it)."""
    import pickle
    where = _record_path(module)
    if not record and os.path.isfile(where):
        with open(where, "rb") as f:
            return pickle.load(f)
    texts = made(module, False)
    if record:
        with open(where, "wb") as f:
            pickle.dump(texts, f)
    return texts


def check_made(module, show, record=False):
    """Functions whose compilable C is the same either way, and the first lines that differ."""
    a, b = reference(module, record), made(module, True)

    def flat(t):                                         # (its tokens: comments, and where a line was broken, aside)
        return " ".join(tokens(t))
    same = [n for n in a if n in b and a[n] == b[n]]
    spaced = [n for n in a if n in b and a[n] != b[n] and flat(a[n]) == flat(b[n])]
    differ = [n for n in a if n in b and flat(a[n]) != flat(b[n])]
    print("%-14s %5d functions  %5d the same  %4d but for comments and line breaks  %4d different  %d only one way" % (
        module, len(a), len(same), len(spaced), len(differ), len(set(a) ^ set(b))), flush=True)
    for n in differ[:show]:
        ta, tb = flat(a[n]), flat(b[n])
        k = next((i for i, (x, y) in enumerate(zip(ta, tb)) if x != y), min(len(ta), len(tb)))
        print("      %s:\n        printed  ...%s\n        from L2  ...%s" % (n, ta[max(0, k - 60):k + 60], tb[max(0, k - 60):k + 60]))
    return len(a), len(same) + len(spaced)


def main(argv):
    if "--made" in argv:
        show = int(argv[argv.index("--show") + 1]) if "--show" in argv else 6
        names = [a for a in argv if not a.startswith("--") and a != str(show)]
        total = same = 0
        for name in names:
            t, s = check_made(name, show, "--record" in argv)
            total, same = total + t, same + s
        print("all: %d functions, %d made the same either way" % (total, same))
        return 0 if total and same == total else 1
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
