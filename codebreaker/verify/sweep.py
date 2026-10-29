"""sweep.py -- reconstruct and judge every module of the corpus, smallest first, resumably.

    python -m codebreaker sweep                    every 32-bit module not yet measured
    python -m codebreaker sweep --only a.dll,b.dll these
    python -m codebreaker sweep --only @list.txt   one module name per line
    python -m codebreaker sweep --16               the 16-bit modules; --drivers the drivers;
                                                   --dos real-mode programs; --all every kind
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


def status(fs, paths, log=print):
    """Report what has been measured of `paths` under the settings in force for each one's kind.
    -> {path: module hash} of those with results."""
    from codebreaker.base import corpus
    from codebreaker.store import functions as F
    from codebreaker.verify import reconstruct
    digests = reconstruct.digests()
    stored = {}
    for digest in set(digests.values()) - {None}:
        stored.update({sha: digest for sha, _name in fs.modules(digest)})
    done, tot = {}, dict.fromkeys(F.SCORED, 0)
    for path in paths:
        sha = corpus.sha(path)
        if stored.get(sha) != digests.get(reconstruct.kind_name(path)) or sha not in stored:
            continue
        done[path] = sha
        for k, n in fs.scores(stored[sha], sha).items():
            tot[k] += n
    log("%d of %d modules measured under the current verifier settings; %d functions: %s" % (
        len(done), len(paths), sum(tot.values()), ", ".join("%s %d" % (F.TITLE[k], tot[k]) for k in F.SCORED)))
    return done


def main(argv):
    from codebreaker.base import corpus
    from codebreaker.store import functions as F
    from codebreaker.verify import reconstruct
    from codebreaker import platforms
    if not platforms.of("pe").compiler.tool():
        raise SystemExit("OpenWatcom is not set up (settings.json: openwatcom): nothing could be built")
    fs = F.open()
    kinds = ("pe", "ne", "le", "dos") if "--all" in argv else ("ne",) if "--16" in argv else \
        ("le",) if "--drivers" in argv else ("dos",) if "--dos" in argv else ("pe",)
    paths = []
    for k in kinds:
        if k == "le":
            paths += corpus.modules("le", exts=(".vxd", ".386"))
        elif k == "dos":
            from codebreaker.formats import dos
            paths += sorted(dos.programs(), key=os.path.getsize)
        else:
            paths += corpus.modules(k)
    only = _wanted(argv)
    if only is not None:
        paths = [p for p in paths if os.path.basename(p).lower() in only]
    done = status(fs, paths)
    if "--status" in argv:
        return 0
    failed = 0
    for i, path in enumerate(paths):
        name = os.path.basename(path).lower()
        sha, digest = reconstruct.identity(path)
        if "--again" not in argv and path in done:
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
        c = fs.scores(digest, sha)
        print("[%d/%d] %s: %s  (%.0fs)" % (i + 1, len(paths), name, ", ".join(
            "%s %d" % (F.TITLE[k], c[k]) for k in F.SCORED if c[k]), time.time() - t0), flush=True)
    status(fs, paths)
    return 1 if failed else 0
