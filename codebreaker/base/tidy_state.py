"""tidy_state.py -- how big state/ is, part by part, and what of it is stale, gone.

    python -m codebreaker tidy-state              the sizes, and the stale removed
    python -m codebreaker tidy-state --dry        the sizes, and what would be removed
    python -m codebreaker tidy-state --stray      also remove what no part of CodeBreaker writes
    python -m codebreaker tidy-state --keep-builds    leave Converted/*/build/ alone

state/ is CodeBreaker's to fill, and once filled 70 GB: fingerprints under comparer revisions
long gone, millions of cache files, the disk and CD copies of killed boot runs. What fills it now
looks after itself -- the cache (base/cache.py) holds its generation of now under a cap, and every
run that copies a disk works in a run folder deleted when it ends (base/runs.py). This is the
look over all of it, and the clean-up of what came before or slipped through. The rules:

    cache          older generations of every kind dropped, the cap held (cache --prune)
    old caches     the fingerprints' own file of before (state/fingerprints.sqlite), now in the cache
    runs           run folders older than a day (killed runs), of every tool that has them, and
                   those of before run folders were (phase3/walk_*, setup16_*, t_*; vxdboot/run_*;
                   record16's memory dumps)
    kept disks     an installed disk a Setup walk kept to compare (record16/walk/<tag>/hd.img),
                   a week old
    builds         the object files of a Converted/<name>/build/ not built for two weeks (the
                   programs and their symbols stay; the next build compiles them again)
    stray          anything at the top of state/ no part of CodeBreaker writes -- listed, removed
                   only with --stray (it may be someone's hand work)

What a run of the code would make again is all that goes; the stores (results.sqlite,
functions.sqlite), hand work (java/overlays, xna/overlays) and the tools' outputs are left.
"""
import glob
import os
import re
import shutil
import sys
import time

DAY = 24 * 3600
RUN_TOOLS = ("phase3", "vxdboot", "record16")
# what the code names by a value, not by a literal: the period compilers' work folders, the
# setup ISO's folder (the setting win95_setup_iso). (The 16-bit API tables, api16*.json, the
# headers' prototypes.json and xdk/ are kept in the knowledge base now, state/db/knowledge.sqlite:
# copies left over from before are not known, and go.)
NAMED_ELSEWHERE = ("msc6", "msvc152", "win95", "products", "golden")
OLD_CACHES = ("fingerprints.sqlite",)     # (the fingerprints' own file, before they were in the cache)


def _size(path):
    if os.path.isfile(path):
        return os.path.getsize(path)
    total = 0
    for d, _dirs, names in os.walk(path):
        for n in names:
            try:
                total += os.path.getsize(os.path.join(d, n))
            except OSError:
                pass
    return total


def _gb(n):
    return "%.2f GB" % (n / float(1 << 30)) if n >= 1 << 30 else "%.1f MB" % (n / float(1 << 20))


def known():
    """The names at the top of state/ that CodeBreaker's code writes: read from the code itself
    (settings.state("<name>" ..., runs.<call>("<name>" ..., "state", "<name>"), so a new tool's
    folder is known the day it is written."""
    from codebreaker.base import settings
    names = set(NAMED_ELSEWHERE) | {"cache"}
    pattern = re.compile(r"""(?:\bstate\(\s*|\bruns\.\w+\(\s*|["']state["']\s*,\s*)["']([^"']+)["']""")
    for top in ("codebreaker", "tools"):
        for d, dirs, files in os.walk(os.path.join(settings.ROOT, top)):
            dirs[:] = [x for x in dirs if x != "__pycache__"]
            for f in files:
                if f.endswith(".py"):
                    with open(os.path.join(d, f), encoding="utf-8", errors="replace") as fh:
                        names.update(m.split("/")[0] for m in pattern.findall(fh.read()))
    return names


def _is_known(name, names):
    base = re.sub(r"-(wal|shm|journal)$", "", name)
    return base in names or base in OLD_CACHES


def stale(now=None):
    """[(path, bytes, rule)] of what the rules say can go (the cache apart: it prunes itself)."""
    from codebreaker.base import runs, settings
    now = now or time.time()
    state = settings.get("state")
    out = []

    def old(p, age):
        try:
            return now - os.path.getmtime(p) > age
        except OSError:
            return False

    for name in OLD_CACHES:
        for suffix in ("", "-wal", "-shm"):
            p = os.path.join(state, name + suffix)
            if os.path.exists(p):
                out.append((p, _size(p), "old caches"))
    for tool in RUN_TOOLS:
        for p in glob.glob(os.path.join(state, tool, "runs", "*")):
            if old(p, runs.AGE):
                out.append((p, _size(p), "runs"))
    for pattern in ("phase3/walk_*", "phase3/setup16_*", "phase3/t_*", "vxdboot/run_*",
                    "record16/rec_mem_*.bin", "record16/rec_hit_*.bin"):
        for p in glob.glob(os.path.join(state, *pattern.split("/"))):
            if old(p, DAY):
                out.append((p, _size(p), "runs"))
    for p in glob.glob(os.path.join(state, "record16", "walk", "*", "hd.img")):
        if old(p, 7 * DAY):
            out.append((p, _size(p), "kept disks"))
    for build in _builds():
        objs, latest = [], 0
        for d, _dirs, names in os.walk(build):
            for n in names:
                p = os.path.join(d, n)
                try:
                    latest = max(latest, os.path.getmtime(p))
                except OSError:
                    continue
                if n.lower().endswith((".obj", ".o")):
                    objs.append(p)
        if objs and now - latest > 14 * DAY:
            out += [(p, _size(p), "builds") for p in objs]
    return out


def _builds():
    """Every Converted/<name>/build/ folder."""
    from codebreaker.base import settings
    croot = settings.get("converted") or os.path.join(settings.ROOT, "Converted")
    return [b for b in glob.glob(os.path.join(croot, "*", "build")) if os.path.isdir(b)]


def stray():
    """[(path, bytes)] at the top of state/ that no part of CodeBreaker writes."""
    from codebreaker.base import settings
    state = settings.get("state")
    names = known()
    return [(os.path.join(state, n), _size(os.path.join(state, n))) for n in sorted(os.listdir(state))
            if not _is_known(n, names)]


def parts():
    """[(name, bytes)] of state/'s top-level parts, the biggest first."""
    from codebreaker.base import settings
    state = settings.get("state")
    return sorted(((n, _size(os.path.join(state, n))) for n in os.listdir(state)), key=lambda x: -x[1])


def _delete(p):
    if os.path.isdir(p):
        shutil.rmtree(p, ignore_errors=True)
    else:
        try:
            os.remove(p)
        except OSError:
            pass
    return not os.path.exists(p)


def main(argv):
    """python -m codebreaker tidy-state [--dry] [--stray] [--keep-builds]"""
    from codebreaker.base import cache, settings
    if argv and argv[0] in ("-h", "--help"):
        print(__doc__)
        return 0
    dry = "--dry" in argv
    state = settings.get("state")
    os.makedirs(state, exist_ok=True)
    before = parts()
    print("state/ (%s): %s" % (state, _gb(sum(s for _n, s in before))))
    for n, s in before[:25]:
        print("  %-28s %10s" % (n, _gb(s)))
    builds = _builds()
    if builds:
        print("Converted/*/build/ (%d): %s" % (len(builds), _gb(sum(_size(b) for b in builds))))
    gone = [(p, s, why) for p, s, why in stale() if not ("--keep-builds" in argv and why == "builds")]
    strays = stray()
    if strays:
        print("not written by CodeBreaker (%s):" % ("removed" if "--stray" in argv and not dry else
                                                     "kept; --stray removes them"))
        for p, s in strays:
            print("  %-28s %10s" % (os.path.basename(p), _gb(s)))
        if "--stray" in argv:
            gone += [(p, s, "stray") for p, s in strays]
    by_rule = {}
    for p, s, why in gone:
        if dry or _delete(p):
            n, b = by_rule.get(why, (0, 0))
            by_rule[why] = (n + 1, b + s)
    for why, (n, b) in sorted(by_rule.items()):
        print("%s %-12s %5d item(s)  %10s" % ("would remove" if dry else "removed", why, n, _gb(b)))
    if not dry:
        for kind, names in cache.prune():
            print("removed cache %s: old generation(s) %s" % (kind, ", ".join(names)))
        after = sum(s for _n, s in parts())
        print("state/ now %s (the cache's cap: %s, setting cache_max_gb)" % (_gb(after), _gb(cache.max_bytes())))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
