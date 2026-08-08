#!/usr/bin/env python3
r"""cli.py -- CodeBreaker Diff command line (invoked as `wt diff <cmd>`).

    wt diff versions                       # the registered OS snapshots
    wt diff init                           # create/upgrade the corpus DB
    wt diff ingest win95_gold [--limit N] [--modules a,b] [--force]
    wt diff stats                          # per-version row + fingerprint coverage
    wt diff match win310 win95_gold [--threshold 0.75]
    wt diff census win310 win95_gold       # similarity census report
    wt diff propagate win95_gold win310 [--min 0.85] [--apply]

Cross-version, so it lives in the shared engine and takes no --target. The DB
defaults to engine/diff/corpus.db (override with --db).
"""
import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import corpus
import matcher
import propagator
import enrich as enrich_mod


def _fmt(n):
    return "{:,}".format(n)


def cmd_versions(args):
    print("registered OS snapshots:\n")
    for vid in sorted(corpus.VERSIONS):
        v = corpus.VERSIONS[vid]
        root = os.path.join(corpus.REPO, v["asm_root"])
        exists = "OK " if os.path.isdir(root) else "-- "
        print("  [%s] %-11s %-13s %s" % (exists, vid, v["architecture"], v["asm_root"]))


def cmd_init(args):
    conn = corpus.open_db(args.db)
    corpus.init_db(conn)
    print("initialised corpus DB: %s" % args.db)


def cmd_ingest(args):
    conn = corpus.open_db(args.db)
    corpus.init_db(conn)
    mods = args.modules.split(",") if args.modules else None

    def prog(a, s, e):
        sys.stderr.write("\r  ingesting %s: +%s added, %s skipped, %s err "
                         % (args.version, _fmt(a), _fmt(s), _fmt(e)))
        sys.stderr.flush()

    added, skipped, errored = corpus.ingest(
        conn, args.version, limit=args.limit, modules=mods, force=args.force,
        progress=prog)
    sys.stderr.write("\r" + " " * 70 + "\r")
    print("ingested %s: %s added, %s skipped, %s errored"
          % (args.version, _fmt(added), _fmt(skipped), _fmt(errored)))


def cmd_stats(args):
    conn = corpus.open_db(args.db)
    corpus.init_db(conn)
    rows = conn.execute(
        "SELECT version_id, COUNT(*) n, "
        " SUM(CASE WHEN cfg_node_count>0 THEN 1 ELSE 0 END) coded, "
        " SUM(CASE WHEN call_count>0 THEN 1 ELSE 0 END) withcalls, "
        " COUNT(DISTINCT module_name) mods "
        " FROM functions GROUP BY version_id ORDER BY version_id").fetchall()
    if not rows:
        print("corpus empty -- run `wt diff ingest <version>` first"); return
    print("%-12s %10s %10s %10s %8s" % ("version", "functions", "with-code",
                                        "with-calls", "modules"))
    for r in rows:
        print("%-12s %10s %10s %10s %8s" % (
            r["version_id"], _fmt(r["n"]), _fmt(r["coded"] or 0),
            _fmt(r["withcalls"] or 0), _fmt(r["mods"] or 0)))


def cmd_match(args):
    conn = corpus.open_db(args.db)
    ms = matcher.match_versions(conn, args.source, args.target,
                                threshold=args.threshold, store=True)
    c = matcher.census(conn, args.source, args.target, ms)
    _print_census(c)
    named = [m for m in ms if not m["target_name"].startswith("sub_")
             or not m["source_name"].startswith("sub_")]
    for m in sorted(ms, key=lambda x: -x["score"])[:args.show]:
        print("  %.2f  %-8s  %s/%s  <->  %s/%s"
              % (m["score"], m["match_type"], m["source_module"], m["source_name"],
                 m["target_module"], m["target_name"]))
    if named:
        print("\n  (%d matches carry a recovered name on at least one side)" % len(named))


def cmd_census(args):
    conn = corpus.open_db(args.db)
    _print_census(matcher.census(conn, args.source, args.target))


def _print_census(c):
    print("\nSimilarity Census: %s -> %s" % (c["source"], c["target"]))
    print("  source functions : %s" % _fmt(c["source_functions"]))
    print("  matched          : %s (%.1f%%)" % (
        _fmt(c["matched"]),
        100.0 * c["matched"] / c["source_functions"] if c["source_functions"] else 0))
    for t, n in sorted(c["by_type"].items(), key=lambda x: -x[1]):
        print("    %-11s %s" % (t, _fmt(n)))
    print()


def cmd_enrich(args):
    conn = corpus.open_db(args.db)
    named, done, missing = enrich_mod.enrich(conn, args.version, apply=True)
    print("enriched %s: named %s function(s) across %d module(s); %d module(s)"
          " had no reachable named binary" % (args.version, _fmt(named), done, missing))


def cmd_propagate(args):
    conn = corpus.open_db(args.db)
    n = propagator.propagate_signatures(conn, args.source, args.target,
                                        min_score=args.min, apply=args.apply)
    verb = "propagated" if args.apply else "would propagate (dry-run; pass --apply)"
    print("%s %d name(s) %s -> %s" % (verb, n, args.source, args.target))


def cmd_ui(args):
    import server
    server.DB = args.db
    server.serve(port=args.port, open_browser=not args.no_open)


def build_parser():
    p = argparse.ArgumentParser(prog="wt diff", description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--db", default=corpus.DEFAULT_DB, help="corpus DB path")
    sub = p.add_subparsers(dest="cmd", required=True)

    sub.add_parser("versions").set_defaults(func=cmd_versions)
    sub.add_parser("init").set_defaults(func=cmd_init)
    sub.add_parser("stats").set_defaults(func=cmd_stats)

    ing = sub.add_parser("ingest")
    ing.add_argument("version")
    ing.add_argument("--limit", type=int)
    ing.add_argument("--modules")
    ing.add_argument("--force", action="store_true")
    ing.set_defaults(func=cmd_ingest)

    mt = sub.add_parser("match")
    mt.add_argument("source"); mt.add_argument("target")
    mt.add_argument("--threshold", type=float, default=0.75)
    mt.add_argument("--show", type=int, default=15)
    mt.set_defaults(func=cmd_match)

    cs = sub.add_parser("census")
    cs.add_argument("source"); cs.add_argument("target")
    cs.set_defaults(func=cmd_census)

    en = sub.add_parser("enrich")
    en.add_argument("version")
    en.set_defaults(func=cmd_enrich)

    pr = sub.add_parser("propagate")
    pr.add_argument("source"); pr.add_argument("target")
    pr.add_argument("--min", type=float, default=0.85)
    pr.add_argument("--apply", action="store_true")
    pr.set_defaults(func=cmd_propagate)

    ui = sub.add_parser("ui")
    ui.add_argument("--port", type=int, default=8790)
    ui.add_argument("--no-open", action="store_true")
    ui.set_defaults(func=cmd_ui)
    return p


def main(argv=None):
    args = build_parser().parse_args(argv)
    args.func(args)


if __name__ == "__main__":
    main()
