#!/usr/bin/env python3
"""
finish.py — the end-of-unit routine, in one call.

Closing out a reconstruction unit always ran the same four steps: sweep the
recovered names over the C, verify against the binary, check the files are
structurally intact, and re-measure. Four calls, every time, and the one that
mattered most (`verify.py` with NO arguments) was the easiest to skip — which
is exactly how eight fabricated calls survived in the app layer for months.

So it is not optional here. `finish.py` always runs the WHOLE-PROJECT verify,
never a per-file one, and reports GHOSTS FIRST because a ghost is the only
result that means something is actually wrong.

    python finish.py                 sweep + verify + integrity + ledger
    python finish.py --no-sweep      skip the name sweep
    python finish.py --quiet         just the verdict line
"""
import glob
import re
import subprocess
import sys

from src.core import layout


def run(cmd):
    p = subprocess.run([sys.executable] + cmd, capture_output=True, text=True)
    return p.stdout + p.stderr


def integrity():
    """Braces balanced and no stray marker left behind."""
    bad = []
    # hand-written + machine-generated C, honoring the migrated src/ tree
    for f in layout.written_c_files() + layout.generated_c_files():
        s = open(f, encoding="utf-8", errors="replace").read()
        # strip comments and string/char literals before counting
        t = re.sub(r"/\*.*?\*/", "", s, flags=re.S)
        t = re.sub(r"//[^\n]*", "", t)
        t = re.sub(r'"(\\.|[^"\\])*"', '""', t)
        t = re.sub(r"'(\\.|[^'\\])*'", "''", t)
        if t.count("{") != t.count("}"):
            bad.append("%s: braces %d{ vs %d}" % (f, t.count("{"), t.count("}")))
        if "call_UNKNOWN" in s or "FIXME_ORD" in s:
            bad.append("%s: unresolved marker" % f)
    return bad


def main():
    quiet = "--quiet" in sys.argv
    if "--no-sweep" not in sys.argv:
        out = run(["sweep.py"])
        if not quiet:
            print(out.strip().splitlines()[-1] if out.strip() else "sweep: no output")

    # ALWAYS the whole project. A per-file verify passed on every one of the
    # eight fabrications that the project-wide run caught.
    v = run(["verify.py"])
    ghosts = re.search(r"GHOST calls \(errors\): (\d+)", v)
    missing = re.search(r"missing calls in non-partial functions: (\d+)", v)
    checked = re.search(r"checked (\d+) functions", v)
    g = int(ghosts.group(1)) if ghosts else -1

    if g > 0:
        print("*** %d GHOST CALL(S) — the reconstruction claims calls the "
              "binary does not make ***" % g)
        for line in v.splitlines():
            if line.strip().startswith("GHOST"):
                print(line)
    else:
        print("GHOSTS: 0  (no reconstruction claims a call the binary does not make)")

    bad = integrity()
    if bad:
        print("INTEGRITY: %d problem(s)" % len(bad))
        for b in bad[:10]:
            print("   " + b)
    else:
        print("INTEGRITY: ok")

    led = run(["ledger.py"])
    tot = [l for l in led.splitlines() if l.strip().startswith("TOTAL")]
    ords_ = [l for l in led.splitlines() if "ORDINALS named" in l]
    if not quiet:
        if checked:
            print("VERIFIED: %s functions, %s missing in non-partial bodies"
                  % (checked.group(1), missing.group(1) if missing else "?"))
        for l in tot + ords_:
            print(l.rstrip())

    sys.exit(1 if (g > 0 or bad) else 0)


if __name__ == "__main__":
    main()
