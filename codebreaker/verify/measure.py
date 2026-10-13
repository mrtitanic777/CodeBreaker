"""measure.py -- judge one decompiler's reconstruction of one module and print the tally.

    python -m codebreaker measure <module> [core|export]

Nothing is written to the source tree: this is the number to watch while changing a decompiler
or the verifier. `core` is the embedded Ghidra core (default), `export` Ghidra run headless.
"""
import collections
import sys
import time


def main(argv):
    if not argv:
        print(__doc__)
        return 2
    from codebreaker.base import corpus
    from codebreaker.source import module_writer as W
    from codebreaker.verify import pipeline, store, whole_module
    pipeline._arm()
    path = corpus.retail(argv[0])
    if not path:
        raise SystemExit("no module %r in the corpus" % argv[0])
    engine = argv[1] if len(argv) > 1 else "core"
    t = time.time()
    results, _preamble = whole_module.verify(path, store=store.open(), engine=engine)
    tally = collections.Counter(W.classify(r)[0] for r in results.values())
    print("%s (%s) in %.0fs: %s" % (argv[0], engine, time.time() - t,
                                    ", ".join("%s %d" % (W.TITLE[k], tally[k]) for k in W.ORDER)))
    why = collections.Counter(r.get("reason") for r in results.values() if r["verdict"] == "divergent")
    if why:
        print("unverified by cause: " + ", ".join("%s %d" % kv for kv in why.most_common()))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
