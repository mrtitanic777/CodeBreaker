#!/usr/bin/env python3
"""causes.py -- across every judged module of one kind: why functions are not verified.

    python tools/causes.py 16|32 [--each N]

Compile rejections by message and behavioural differences by what differed, counted over all
modules judged under the current settings, largest first. Where tools/failures.py explains one
module, this says which defect is worth fixing first for the whole corpus.
"""
import collections
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def main(argv):
    if not argv or argv[0] not in ("16", "32"):
        print(__doc__)
        return 2
    from codebreaker.store import functions as F
    from codebreaker.verify import reconstruct
    kind = "ne" if argv[0] == "16" else "pe"
    each = int(argv[argv.index("--each") + 1]) if "--each" in argv else 2
    fs = F.open()
    compile_, differ, tally = collections.Counter(), collections.Counter(), collections.Counter()
    ex = collections.defaultdict(list)
    n_mod = 0
    for sha, name, path in fs.db.execute("SELECT sha, name, path FROM module WHERE kind=?", (kind,)):
        if not os.path.isfile(path or ""):
            continue
        _sha, digest = reconstruct.identity(path)
        # (a runtime-library function is identified, not the module's own: not counted here)
        library = fs.identified(sha)
        rows = [r for r in fs.candidates(sha, digest) if r["verdict"] and r["addr"] not in library]
        if not rows:
            continue
        n_mod += 1
        for r in rows:
            cls = F.classify(r["verdict"], r["limits"])[0] if r["verdict"] else "no-source"
            tally[cls] += 1
            if r["verdict"] == "no-compile":
                k = re.sub(r"'[^']*'", "'..'", r["reason"] or "")[:70]
                compile_[k] += 1
                if len(ex[k]) < each:
                    ex[k].append("%s %s: %s" % (name, r["name"], (r["reason"] or "")[:90]))
            elif r["verdict"] == "divergent":
                d = (r["detail"] or "").split("  [")[0]
                k = r["reason"] + "  " + re.sub(r"\b[0-9a-f]{3,}\b", "N", re.sub(r"#\d+", "#k", d))[:60]
                differ[k] += 1
                if len(ex[k]) < each:
                    ex[k].append("%s %s %s: %s" % (name, r["addr"], r["name"], (r["detail"] or "")[:100]))
            elif cls == "weak":
                for c in (r["limits"] or "").split(","):
                    if c in F.WEAK:
                        tally["weak: " + c] += 1
    total = sum(tally[k] for k in F.ORDER)
    print("%d modules, %d functions: %s" % (n_mod, total, ", ".join(
        "%s %d (%.0f%%)" % (F.TITLE[k], tally[k], 100.0 * tally[k] / max(1, total)) for k in F.ORDER if tally[k])))
    print("why weak: " + ", ".join("%s %d" % (k[6:], n) for k, n in tally.most_common() if k.startswith("weak: ")))
    for title, counter in (("does not compile", compile_), ("behaves differently", differ)):
        print("\n%s:" % title)
        for k, n in counter.most_common(14):
            print("%5d  %s" % (n, k))
            for e in ex[k]:
                print("          " + e)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
