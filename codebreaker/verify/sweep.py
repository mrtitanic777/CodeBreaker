"""sweep.py -- reconstruct and judge every module of the corpus, smallest first, resumably.

    python -m codebreaker sweep                    every 32-bit module not yet measured
    python -m codebreaker sweep --only a.dll,b.dll these
    python -m codebreaker sweep --only @list.txt   one module name per line
    python -m codebreaker sweep --16               the 16-bit modules; --all for both
    python -m codebreaker sweep --status           what has been measured; no work
    python -m codebreaker sweep --again            measure modules again even if already done

A module counts as measured when every one of its functions has a result under the current
verifier settings, so an interrupted sweep picks up at the module it stopped in, and changing
the verifier makes the whole corpus due again without anyone having to say so. One module that
fails outright is reported and the sweep moves on.
"""
import os
import time


def _wanted(argv):
    if "--only" not in argv:
        return None
    arg = argv[argv.index("--only") + 1]
    if arg.startswith("@"):
        with open(arg[1:], encoding="utf-8") as f:
            return {ln.strip().lower() for ln in f if ln.strip()}
    return {x.strip().lower() for x in arg.split(",") if x.strip()}


def status(fs, digest, paths, log=print):
    from codebreaker.store import functions as F
    done = {name: sha for sha, name in fs.modules(digest)}
    tot = dict.fromkeys(F.ORDER, 0)
    for name, sha in sorted(done.items()):
        for k, n in fs.tally(digest, sha).items():
            tot[k] += n
    n = sum(tot.values())
    log("%d of %d modules measured under verifier settings %s; %d functions: %s" % (
        len(done), len(paths), digest, n, ", ".join("%s %d" % (F.TITLE[k], tot[k]) for k in F.ORDER)))
    return done


def main(argv):
    from codebreaker.base import corpus
    from codebreaker.store import functions as F
    from codebreaker.verify import win32 as pipeline, reconstruct
    from codebreaker.build import watcom as period_tools
    if not period_tools.ow_tool("wcc386"):
        raise SystemExit("OpenWatcom is not set up (settings.json: openwatcom): nothing could be built")
    fs = F.open()
    digest = pipeline.settings_profile("pe")[0]
    kinds = ("pe", "ne") if "--all" in argv else ("ne",) if "--16" in argv else ("pe",)
    paths = [p for k in kinds for p in corpus.modules(k)]
    only = _wanted(argv)
    if only is not None:
        paths = [p for p in paths if os.path.basename(p).lower() in only]
    done = status(fs, digest, paths)
    if "--status" in argv:
        return 0
    failed = 0
    for i, path in enumerate(paths):
        name = os.path.basename(path).lower()
        sha, _d = reconstruct.identity(path)
        if "--again" not in argv and name in done:
            judged = fs.best(sha, digest)
            if all(a in judged for a in fs.addresses(sha)):
                continue
        t0 = time.time()
        try:
            reconstruct.run(path, fs=fs, log=lambda s: None)
        except KeyboardInterrupt:
            raise
        except Exception as e:                              # one module must not end the sweep
            failed += 1
            print("[%d/%d] %s FAILED: %s: %s" % (i + 1, len(paths), name, type(e).__name__, str(e)[:120]), flush=True)
            continue
        c = fs.tally(digest, sha)
        print("[%d/%d] %s: %s  (%.0fs)" % (i + 1, len(paths), name, ", ".join(
            "%s %d" % (F.TITLE[k], c[k]) for k in F.ORDER if c[k]), time.time() - t0), flush=True)
    status(fs, digest, paths)
    return 1 if failed else 0
