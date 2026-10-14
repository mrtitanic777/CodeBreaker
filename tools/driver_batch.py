#!/usr/bin/env python3
"""driver_batch.py -- write, build and check the source of many drivers, one at a time.

    python tools/driver_batch.py [N] [--from K]      the N smallest corpus drivers (default 20)

One line per driver: instructions, share written as bytes, rounds, and which structural checks
came back the same. Ends with totals. Runs in one process; nothing is judged here (that is
`sweep --drivers`)."""
import os
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
from codebreaker.base import settings                                   # noqa: E402
from codebreaker.formats import le                                      # noqa: E402
from codebreaker.source import driver_writer                            # noqa: E402


def main(argv):
    args = [a for a in argv if not a.startswith("--")]
    n = int(args[0]) if args else 20
    start = int(argv[argv.index("--from") + 1]) if "--from" in argv else 0
    files = sorted(le.corpus_files(), key=os.path.getsize)[start:start + n]
    ok = built = 0
    for path in files:
        name = os.path.basename(path).lower()
        t0 = time.time()
        try:
            if len(le.members(path)) != 1:
                print("%-14s a container; skipped" % name, flush=True)
                continue
            r = driver_writer.reconstruct(path, settings.state("driver_source", os.path.splitext(name)[0]))
        except Exception as e:
            print("%-14s FAILED %s: %s" % (name, type(e).__name__, str(e)[:90]), flush=True)
            continue
        checks = r.get("checks") or {}
        same = bool(checks) and all(checks.values())
        ok += same
        built += bool(r.get("linked"))
        ins = max(1, r.get("instructions", 0))
        print("%-14s %5d instructions, %2d%% as bytes, %2d rounds, %-9s %s  (%.0fs)" % (
            name, r.get("instructions", 0), 100 * r.get("raw", 0) // ins, r["rounds"],
            "ALL SAME" if same else "built" if r.get("linked") else "NO BUILD",
            "" if same else "; ".join((r.get("notes") or [])[:1])[:90], time.time() - t0), flush=True)
    print("%d drivers: %d built, %d with every check the same" % (len(files), built, ok))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
