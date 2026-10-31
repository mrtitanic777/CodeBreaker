"""measure.py -- reconstruct and judge one module and print the tally. Nothing is written to the
source tree.

    python -m codebreaker measure <module> [--no-engine]

The decompile, build and judge are steps of the engine (engine/lanes.py) by default: only what
changed since the last measure is made again. --no-engine (or CB_ENGINE=0) runs the old direct path.

This is the number to watch while changing the decompiler or the verifier.
"""
import collections
import os
from codebreaker.verify import verdicts as V


def main(argv):
    if "--engine" in argv:                                   # (the engine is the default; kept for scripts)
        os.environ["CB_ENGINE"] = "1"
    if "--no-engine" in argv:                                # (the old direct path)
        os.environ["CB_ENGINE"] = "0"
    argv = [a for a in argv if a not in ("--engine", "--no-engine")]
    if not argv:
        print(__doc__)
        return 2
    from codebreaker.base import corpus
    from codebreaker.store import functions as F
    from codebreaker.verify import reconstruct
    path = argv[0] if os.path.isdir(argv[0]) else corpus.retail(argv[0])   # a folder: a Java program
    if not path:
        raise SystemExit("no module %r in the corpus" % argv[0])
    fs = F.open()
    sha, digest = reconstruct.run(path, fs=fs)
    rows = fs.candidates(sha, digest)
    why = collections.Counter(r["reason"] for r in rows if r["verdict"] == V.DIVERGENT)
    if why:
        print("unverified by cause: " + ", ".join("%s %d" % kv for kv in why.most_common()))
    bad = collections.Counter((r["reason"] or "")[:60] for r in rows if r["verdict"] == V.NO_COMPILE)
    if bad:
        print("does not compile: " + "; ".join("%s (%d)" % kv for kv in bad.most_common(4)))
    return 0
