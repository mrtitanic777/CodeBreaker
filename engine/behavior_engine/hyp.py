#!/usr/bin/env python3
r"""hyp.py -- `wt behavior hyp`: show the accumulated hypotheses for a function.

    wt behavior hyp <module> [func] [--min C] [--evidence]

    <module>       the traced binary, e.g. gdi32.dll
    [func]         VA or section offset (hex/decimal); omitted -> lowest-VA function with a store.
    --min C        only show hypotheses with confidence >= C (e.g. 0.66).
    --evidence     print the supporting/contradicting evidence events per hypothesis.

Reads the derived hypothesis store (built by `wt behavior experiment`). Competing claims for
the same subject are shown together, highest confidence first, so uncertainty stays visible.
"""
import os
import sys

from engine.behavior_engine import corpus, hypothesis


from engine.cli.common import opt as _opt


def _subject_label(s):
    if s["kind"] == "arg":
        return "arg%d" % s["index"]
    if s["kind"] == "function":
        return "function"
    return s.get("kind", "?")


def _resolve_va(module, func):
    d = os.path.join(corpus.derived_dir(), "hypotheses", os.path.basename(module))
    if not os.path.isdir(d):
        return None
    files = sorted(f for f in os.listdir(d) if f.endswith(".json"))
    if not files:
        return None
    if func is None:
        return int(files[0][:-5], 16)
    val = int(func, 0) if isinstance(func, str) else int(func)
    if ("0x%x.json" % val) in files:
        return val
    # maybe a section offset -> match the stored func_off
    import json
    for f in files:
        with open(os.path.join(d, f), encoding="utf-8") as fh:
            st = json.load(fh)
        va = int(f[:-5], 16)
        # no func_off stored in the hyp file; accept VA match only
        if va == val:
            return va
    return val


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    func = None
    for a in argv[1:]:
        if not a.startswith("-"):
            func = a; break
    min_c = float(_opt(argv, "--min", 0.0))
    show_ev = "--evidence" in argv

    va = _resolve_va(module, func)
    if va is None:
        print("no hypothesis store for %s -- run `wt behavior experiment %s %s` first"
              % (module, module, func or "")); sys.exit(1)
    store = hypothesis.load(module, va)
    hyps = [h for h in store["hypotheses"] if h["confidence"] >= min_c]
    if not hyps:
        print("no hypotheses (>= %.2f) for %s 0x%x" % (min_c, module, va)); return

    print("=== hypotheses: %s  func VA 0x%x  (%d claims) ===" %
          (store["module"], store["func_va"], len(store["hypotheses"])))
    # group by subject, then confidence desc
    hyps.sort(key=lambda h: (str(h["subject"]), -h["confidence"]))
    last = None
    for h in hyps:
        lab = _subject_label(h["subject"])
        if lab != last:
            print("\n%s:" % lab); last = lab
        bar = "#" * int(round(h["confidence"] * 20))
        print("  [%-20s] %.3f  %s   (support=%d contradict=%d)" %
              (bar, h["confidence"], h["statement"], h["support"], h["contradict"]))
        if show_ev:
            for e in h["evidence"]:
                print("        - %-10s %s  [%s]" % (e["kind"], e["detail"], e["ref"]))


if __name__ == "__main__":
    main()
