#!/usr/bin/env python3
"""used_modules.py -- which modules of the package a real run actually loads.

    python tools/used_modules.py

Reconstructs one small 32-bit and one small 16-bit module from scratch (into a throwaway
store) and reports every package module that was never imported while doing so. Those are
reachable on paper -- some function somewhere imports them -- but the working path does not
touch them: candidates for removal, or for being wired in properly.
"""
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)


def main():
    os.environ["CB_FUNCTIONS_DB"] = os.path.join(tempfile.mkdtemp(), "functions.sqlite")
    os.environ["CB_JOBS"] = "1"                              # in this process, so imports are seen
    from codebreaker.base import corpus
    from codebreaker.source import module_writer
    out = tempfile.mkdtemp()
    for name in ("version.dll", "lzexpand.dll"):
        if corpus.retail(name):
            module_writer.write_module(name, out_dir=out, log=lambda s: None)
    loaded = {m for m in sys.modules if m.startswith("codebreaker")}
    unused, total = [], 0
    for base, dirs, files in os.walk(os.path.join(ROOT, "codebreaker")):
        dirs[:] = [d for d in dirs if d != "__pycache__"]
        for f in sorted(files):
            if not f.endswith(".py") or f.startswith("__"):
                continue
            p = os.path.join(base, f)
            mod = os.path.relpath(p, ROOT)[:-3].replace(os.sep, ".")
            n = sum(1 for _ in open(p, encoding="utf-8", errors="replace"))
            total += n
            if mod not in loaded:
                unused.append((n, mod))
    for n, mod in sorted(unused, reverse=True):
        print("%5d  %s" % (n, mod))
    print("%d of the package's modules (%d of %d lines) were never loaded" % (
        len(unused), sum(n for n, _m in unused), total))
    return 0


if __name__ == "__main__":
    sys.exit(main())
