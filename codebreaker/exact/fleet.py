"""fleet.py -- the bench of several projects at once, a function at a time over a pool of worker processes.

    python -m codebreaker.exact.fleet PROJECT.. --out PREFIX [--jobs N] [--since PREFIX] [--keep FILE]

Every function each project has a source for is drafted and proven as `bench` does, the largest first so the
pool stays busy to the end. Each worker has a stage of its own (CB_STAGE_TAG); the compile cache is shared.

--since PREFIX re-benches only what a run before did not get identical (PREFIX_NAME.txt, one line a function),
carrying its identical ones over as they were; --keep FILE (lines `NAME ADDR` or a regression list) benches
those again too, whatever they were - the guard that what was identical still is; --only benches the --keep list
alone (a regression run). Writes PREFIX_NAME.txt for each
project (`ADDR verdict`) and PREFIX.txt, the tally of each and of all.
"""
import collections
import multiprocessing
import os
import re
import sys
import time

_PRJ = {}


def _init(tags):
    with tags.get_lock():                                   # (a stage of the worker's own, numbered from 1)
        tags.value += 1
        os.environ["CB_STAGE_TAG"] = "w%d" % tags.value


def _one(task):
    path, a = task
    from codebreaker.exact import project, bench
    if path not in _PRJ:
        _PRJ[path] = project.load(path)
    t0 = time.time()
    try:
        rows = bench.run(_PRJ[path], [a])
        v = rows[0][1] if rows else "no source"
    except Exception as e:                                  # (a function that breaks a worker's run: noted)
        v = "failed: %s: %s" % (type(e).__name__, str(e)[:80])
    return path, a, v, time.time() - t0


def _read(f):
    out = {}
    if os.path.exists(f):
        for l in open(f):
            a, _, v = l.rstrip("\n").partition(" ")
            if a:
                out[int(a, 16)] = v
    return out


def main(argv):
    from codebreaker.exact import project, carve
    out = argv[argv.index("--out") + 1]
    jobs = int(argv[argv.index("--jobs") + 1]) if "--jobs" in argv else max(1, (os.cpu_count() or 2) - 2)
    since = argv[argv.index("--since") + 1] if "--since" in argv else None
    keep = set()
    if "--keep" in argv:                                    # (NAME ADDR lines, or `echo "NAME: ... bench A B ..."`)
        for l in open(argv[argv.index("--keep") + 1]):
            m = re.match(r'\s*echo "(\w+):.*\bbench ((?:[0-9a-f]{5} ?)+)', l)
            if m:
                keep |= {(m.group(1), int(x, 16)) for x in m.group(2).split()}
            elif re.fullmatch(r"\s*(\w+) ([0-9a-f]{5})\s*", l):
                n, x = l.split()
                keep.add((n, int(x, 16)))
    only = "--only" in argv                                 # (the --keep list alone: a regression run)
    opts = {"--out", "--jobs", "--since", "--keep"}
    paths = [p for i, p in enumerate(argv) if not p.startswith("--") and (i == 0 or argv[i - 1] not in opts)]
    name = lambda p: os.path.basename(os.path.normpath(p))
    tasks, carried = [], {}
    for p in paths:
        prj = project.load(p)
        fs = {a: ln for a, ln, w in carve.functions(prj)}
        have = sorted(a for a, f in prj.sources() if a in fs)
        before = _read("%s_%s.txt" % (since, name(p))) if since else {}
        carried[p] = {}
        for a in have:
            if only and (name(p), a) not in keep:
                continue
            if since and before.get(a, "").startswith("identical") and (name(p), a) not in keep:
                carried[p][a] = before[a]
            else:
                tasks.append((fs[a], p, a))
    tasks.sort(reverse=True)                                # (the largest first: the pool ends together)
    print("%d functions to bench (%d carried over), %d workers" % (len(tasks), sum(len(c) for c in carried.values()), jobs))
    sys.stdout.flush()
    tags = multiprocessing.Value("i", 0)
    res = {p: dict(carried[p]) for p in paths}
    t0 = time.time()
    done = 0
    with multiprocessing.Pool(jobs, initializer=_init, initargs=(tags,)) as pool:
        for path, a, v, dt in pool.imap_unordered(_one, [(p, a) for ln, p, a in tasks]):
            res[path][a] = v
            done += 1
            if done % 25 == 0 or done == len(tasks):
                print("%d/%d done, %.0fs" % (done, len(tasks), time.time() - t0))
                sys.stdout.flush()
    tally_all = collections.Counter()
    lines = []
    for p in paths:
        with open("%s_%s.txt" % (out, name(p)), "w") as f:
            for a in sorted(res[p]):
                f.write("%05x %s\n" % (a, res[p][a]))
        t = collections.Counter(v.split(":")[0] for v in res[p].values())
        tally_all += t
        lines.append("== %s: %d functions: %s" % (name(p), len(res[p]), ", ".join("%d %s" % (n, k) for k, n in t.most_common())))
    lines.append("== all: %d functions: %s (%.0fs)" % (sum(tally_all.values()), ", ".join("%d %s" % (n, k) for k, n in tally_all.most_common()),
                                                       time.time() - t0))
    open(out + ".txt", "w").write("\n".join(lines) + "\n")
    print("\n".join(lines))
    return 0


if __name__ == "__main__":
    multiprocessing.freeze_support()
    sys.exit(main(sys.argv[1:]))
