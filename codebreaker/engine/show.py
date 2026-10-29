"""show.py -- what a binary's database holds, and why.

    python -m codebreaker facts <binary>                  every kind: how many facts, their size, who made them
    python -m codebreaker facts <binary> <kind>           each fact of the kind: its key, hash, size, when made,
                                                          whether its step's version is the version of now
    python -m codebreaker facts <binary> <kind> <key>     one fact: its value (in short) and what it was made from
    python -m codebreaker facts <binary> <kind> [key] --make      made current first (the engine runs)
    python -m codebreaker facts --databases               every database, its size, and the cap (db_max_gb)

<binary> is a path or a corpus name. Nothing is made unless --make is given.
"""
import os
import time

from codebreaker.engine import engine, facts


def _mb(n):
    return "%.1f MB" % (n / 1048576.0) if n >= 1 << 20 else "%.1f KB" % (n / 1024.0)


def _short(v, width=100):
    """A value in a line or a few."""
    if isinstance(v, (bytes, bytearray)):
        return "%d bytes" % len(v)
    if isinstance(v, str):
        lines = v.splitlines() or [""]
        head = "\n".join("      " + ln[:width] for ln in lines[:12])
        return "%d lines\n%s%s" % (len(lines), head, "\n      ..." if len(lines) > 12 else "")
    if isinstance(v, dict):
        return "{" + ", ".join("%s: %s" % (k, _short(x, 40).split("\n")[0]) for k, x in list(v.items())[:12]) + \
            (", ..." if len(v) > 12 else "") + "}"
    if isinstance(v, (list, tuple)):
        return "[%d] " % len(v) + ", ".join(_short(x, 40).split("\n")[0] for x in list(v)[:6]) + \
            (", ..." if len(v) > 6 else "")
    text = repr(v)
    return text[:width] + ("..." if len(text) > width else "")


def _when(t):
    return time.strftime("%Y-%m-%d %I:%M:%S %p", time.localtime(t)) if t else "-"


def main(argv):
    if "--databases" in argv:
        total = 0
        for p, size, t in facts.databases():
            total += size
            print("  %-70s %10s  opened %s" % (os.path.basename(p), _mb(size), _when(t)))
        print("  all %s (cap %s: setting db_max_gb)" % (_mb(total), _mb(facts.max_bytes())))
        return 0
    make = "--make" in argv
    args = [a for a in argv if not a.startswith("--")]
    if not args:
        print(__doc__)
        return 2
    from codebreaker.base import corpus
    path = corpus.retail(args[0])
    if not path:
        print("no binary %r" % args[0])
        return 2
    db = engine.open(path)
    if make and len(args) > 1:
        t = time.time()
        if len(args) > 2:
            engine.get(db, args[1], args[2], log=print)
        else:
            engine.get(db, args[1], "", log=print)
        print("made in %.1fs: %s" % (time.time() - t, engine.last_run(db)))
    print("%s  (%s)" % (path, db.path))
    if len(args) == 1:
        print("  %-20s %8s %10s  %s" % ("kind", "facts", "size", "made by"))
        for kind, n, size, steps in db.kinds():
            print("  %-20s %8d %10s  %s" % (kind, n, _mb(size or 0), steps))
        return 0
    kind = args[1]
    try:
        now = engine._step_of(kind).version()
    except LookupError:
        now = None
    if len(args) == 2:
        rows = db.rows(kind)
        for r in rows:
            state = "given" if r.given else ("version of now" if r.version == now else "OLD VERSION")
            print("  %-14s %s  %10s  %s  %6.2fs  %s" % (r.key or '""', r.hash[:12], _mb(r.size), _when(r.made),
                                                       r.took or 0, state))
        print("  %d facts of %s" % (len(rows), kind))
        return 0
    key = args[2]
    w = engine.why(db, kind, key)
    if w is None:
        print("  no fact %s %s (--make to make it)" % (kind, key))
        return 1
    print("  %s %s  hash %s, %s" % (kind, key, w["hash"], _mb(w["size"])))
    print("  made by    %s, version %s%s" % (w["step"], w["version"],
                                             "" if w["version"] == w["version_now"] else
                                             " (the version of now is %s: stale)" % w["version_now"]))
    print("  made       %s, in %.2fs" % (_when(w["made"]), w["took"] or 0))
    print("  from       %d facts" % len(w["inputs"]))
    for k, key2, h, same in w["inputs"][:40]:
        print("    %-20s %-14s %s  %s" % (k, key2 or '""', h[:12], "the same now" if same else "CHANGED"))
    if len(w["inputs"]) > 40:
        print("    ... %d more" % (len(w["inputs"]) - 40))
    print("  value      %s" % _short(db.row(kind, key).value()))
    return 0
