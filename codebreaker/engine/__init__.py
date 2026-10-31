"""engine -- the program database and the incremental engine (docs/ARCHITECTURE.md, parts 1 and 2).

    facts.py     one SQLite database per binary: facts, how each was made, the size rule
    engine.py    steps (what each reads and makes), and get(): a fact current, made again only
                 when what it was made from changed, the stale ones side by side
    win16.py     the 16-bit module build as steps: describe, decompile, compile, gate, link
    show.py      python -m codebreaker facts <binary> [kind [key]]: what a database holds and why

The engine is the default path (reconstruct.run, build/module.build, measure, sweep). CB_ENGINE=0
falls back to the old direct paths, kept until the next milestone (docs/NATIVE.md's four moves).
"""
import os


def enabled():
    """Is the engine path on? The default; CB_ENGINE=0 falls back to the old direct path."""
    return os.environ.get("CB_ENGINE", "1") != "0"
