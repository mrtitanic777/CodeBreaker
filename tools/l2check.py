"""l2check.py -- the decompiler's L2 for every function of the golden corpus, held to the schema.

    python tools/l2check.py [NAME ...] [--golden FOLDER] [--show N]

Runs the native decompiler with CB_L2=1 on each golden module's frozen description (tools/golden.py),
takes each function's `/*@L2 {...} */` note, and asks codebreaker.translate.l2.validate of it. Per
module: functions, documents valid, documents with a body, and the Unsupported nodes left in the
bodies (what PrintL2 met that has no L2 node yet), by kind. docs/IR_UNIFICATION.md step 1 is done
when every function gives a valid document."""
import collections
import json
import os
import re
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)

from codebreaker.decompile import decompiler                 # noqa: E402
from codebreaker.translate import l2                         # noqa: E402

NOTE = re.compile(r"^/\*@L2 (\{.*\}) \*/$", re.M)


def _walk(node, found):
    if isinstance(node, dict):
        if node.get("node") == "Unsupported":
            found[(node.get("kind"), node.get("raw"))] += 1
        for v in node.values():
            _walk(v, found)
    elif isinstance(node, list):
        for v in node:
            _walk(v, found)


def check(name, folder, show):
    snap = json.load(open(os.path.join(folder, name, "golden.json")))
    import jsonschema
    validator = jsonschema.Draft202012Validator(l2.schema())
    total = valid = bodies = clean = written = 0
    unsupported = collections.Counter()
    errors = collections.Counter()
    t0 = time.time()
    os.environ["CB_L2"] = "1"
    for member, m in snap.items():
        out, _err, _whole = decompiler._run_resuming(m["description"], (), 3600)
        # every function the golden run wrote C for (a crash shows here); not those the core itself
        # could not decompile, which have no C either
        written += sum("decompile failed" not in text for text in m["functions"].values())
        for note in NOTE.finditer(out):
            total += 1
            try:
                doc = json.loads(note.group(1))
            except ValueError as e:
                errors["not JSON: %s" % e] += 1
                continue
            problem = next(iter(validator.iter_errors(doc)), None)
            if problem is not None:
                errors["%s at %s" % (problem.message[:80], "/".join(str(p) for p in problem.absolute_path)[:60])] += 1
                continue
            valid += 1
            if doc.get("body"):
                bodies += 1
            found = collections.Counter()
            _walk(doc.get("body"), found)
            unsupported.update(found)
            clean += not found
    print("%-10s %5d functions  %5d valid  %5d with a body  %5d without Unsupported  (%.0fs)" % (
        name, written, valid, bodies, clean, time.time() - t0), flush=True)
    if total < written:
        errors["%d functions gave no L2 (the decompiler stopped on them)" % (written - total)] += 1
    for e, n in errors.most_common(show):
        print("      invalid %4d  %s" % (n, e))
    for (kind, raw), n in unsupported.most_common(show):
        print("      unsupported %4d  %s %r" % (n, kind, raw))
    return written, valid, unsupported


def main(argv):
    folder = argv[argv.index("--golden") + 1] if "--golden" in argv else os.path.join(ROOT, "state", "golden")
    show = int(argv[argv.index("--show") + 1]) if "--show" in argv else 8
    skip = {folder, str(show)}
    names = [a for a in argv if not a.startswith("--") and a not in skip] or sorted(
        n for n in os.listdir(folder) if os.path.isfile(os.path.join(folder, n, "golden.json")))
    total = valid = 0
    kinds = collections.Counter()
    for name in names:
        t, v, u = check(name, folder, show)
        total, valid = total + t, valid + v
        for (kind, _raw), n in u.items():
            kinds[kind] += n
    print("all: %d functions, %d valid L2 (%.1f%%); Unsupported nodes by kind: %s" % (
        total, valid, 100.0 * valid / max(total, 1), dict(kinds.most_common())))
    return 0 if total and valid == total else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
