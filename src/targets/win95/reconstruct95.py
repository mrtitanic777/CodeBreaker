#!/usr/bin/env python3
r"""reconstruct95.py -- full Windows 95 reconstruction, placed DIRECTLY into the authored tree.

One pass over the corpus: reconstruct each module (PE behavioral readable-C + C++/MFC/typing
preamble; NE 16-bit deep via recon16), then write the finished, named, beautified source straight
into its component folder in the ONE source tree (the repo root) via placement.place -- which archives
any existing code in that folder into `legacy/` first. NO staging directories.

  * PE  -> compose.reconstruct_module (fused) -> <stem>.c
  * NE  -> recon16.reconstruct_module (deep)  -> <stem>.c
  * LE/VxD are reconstructed as MASM by the separate VxD engine, which already targets vmm32/.

Module-level PARALLEL (independent per module) and RESUMABLE (skips a module whose placed <stem>.c
already exists, so re-running continues; --force re-places, archiving-to-legacy once).

    wt --target win95 reconstruct95 [--per 80] [--jobs 8] [--mods a.dll,b.dll] [--force] [--dry]
"""
import os
import sys
import collections

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import target
import placement


def _render_pe(module, per):
    import compose
    rs = compose.reconstruct_module(module, per, fuse=True, behav_limit=per)
    out = ["/* %s -- behavioral reconstruction (compose): readable C where recoverable,\n"
           "   validated against the binary oracle. */\n" % module]
    try:
        pre = compose.module_preamble(module)
    except Exception:
        pre = ""
    if pre:
        out.append(pre + "\n")
    n = nr = 0
    for r in rs:
        n += 1
        if getattr(r, "source", None):
            nr += 1
            out.append(compose._fn_header(r) + "%s\n" % r.source.rstrip())
    return "\n".join(out) + "\n", n, nr


def _render_ne(path, module):
    from src.engines.behavior import recon16
    rs = recon16.reconstruct_module(path, limit=None)
    out = ["/* %s -- 16-bit (Win16 NE) deep reconstruction (recon16 on tracer16):\n"
           "   readable C where recoverable, validated against the binary oracle. */\n" % module]
    n = nr = 0
    for r in rs:
        n += 1
        if not r.get("reconstructed"):
            continue
        c = r.get("emitted_c") or r.get("source") or ""
        if not c.strip():
            continue
        nr += 1
        label = r.get("label") or ("sub_%x_%x" % (r.get("seg", 0), r.get("offset", 0)))
        out.append("/* %s  [%s] */\n%s\n" % (label, r.get("method", ""), c.rstrip()))
    return "\n".join(out) + "\n", n, nr


def _one(module, per, force, dry):
    """Reconstruct + place ONE module. Returns (status, nfuncs, nrecon, placedir)."""
    try:
        path = target.resolve(module)
        fmt = target.format_of(path)
    except Exception:
        return ("resolveerr", 0, 0, "")
    stem = module.rsplit(".", 1)[0]
    comp, _conf = placement.resolve(module, fmt=fmt, path=path)
    target_dir = os.path.join(placement.tree_root(), *comp.split("/"))
    placed_c = os.path.join(target_dir, stem + ".c")
    if not force and os.path.exists(placed_c):
        return ("skip", 0, 0, comp)
    try:
        if fmt == "PE":
            text, n, nr = _render_pe(module, per)
        elif fmt == "NE":
            text, n, nr = _render_ne(path, module)
        else:
            return ("notsupported:" + fmt, 0, 0, comp)     # LE/VxD via the MASM engine
    except Exception as e:
        return ("ERR:" + str(e)[:48], 0, 0, comp)
    placement.place(module, {stem + ".c": text}, fmt=fmt, path=path,
                    lane=("pe-deep" if fmt == "PE" else "ne-deep"), dry=dry)
    return ("ok", n, nr, comp)


def _modules():
    try:
        allp = {os.path.basename(p).lower(): p for p in target.all_modules()}
    except Exception:
        allp = {}
    mods = []
    for name, p in allp.items():
        if name.endswith(".fon"):                           # fonts are data, not source
            continue
        try:
            if target.format_of(p) in ("PE", "NE"):         # LE/VxD handled by the VxD engine
                mods.append(name)
        except Exception:
            pass
    return sorted(set(mods))


def run(per=80, mods=None, jobs=1, force=False, dry=False, progress=None):
    mods = mods or _modules()
    if jobs and jobs > 1:
        return _run_parallel(per, mods, jobs, force, dry, progress)
    tot = collections.Counter()
    total = len(mods)
    for i, m in enumerate(mods, 1):
        if progress:
            progress(i, total, m, tot)
        st, n, nr, comp = _one(m, per, force, dry)
        if st == "ok":
            tot["mods"] += 1; tot["funcs"] += n; tot["readable"] += nr
            print("  %-18s -> %-24s %d/%d" % (m, comp, nr, n), flush=True)
        elif st.startswith("ERR"):
            print("  SKIP %-14s (%s)" % (m, st), flush=True)
    _summary(tot)


_PW = {}


def _winit(corpus, target_dir, per, force, dry):
    import os as _os
    _os.environ["WT_DATA_ROOT"] = corpus
    try:
        _os.chdir(corpus)
        from src.core import cores, config
        config.apply_config(); cores.set_target(target_dir)
    except Exception:
        pass
    _PW.update(per=per, force=force, dry=dry)


def _wtask(m):
    try:
        return (m,) + _one(m, _PW["per"], _PW["force"], _PW["dry"])
    except Exception as e:
        return (m, "ERR:" + str(e)[:48], 0, 0, "")


def _run_parallel(per, mods, jobs, force, dry, progress):
    from src.core import paths
    from concurrent.futures import ProcessPoolExecutor, as_completed
    cb = paths.repo_root()
    corpus = os.environ.get("WT_DATA_ROOT", os.getcwd())
    target_dir = os.path.join(cb, "src", "targets", "win95")
    os.environ["PYTHONPATH"] = os.pathsep.join(
        [cb, target_dir] + ([os.environ["PYTHONPATH"]] if os.environ.get("PYTHONPATH") else []))
    tot = collections.Counter()
    total = len(mods); done = 0
    print("PARALLEL full reconstruction -> authored tree: %d modules, %d workers" % (total, jobs), flush=True)
    with ProcessPoolExecutor(max_workers=jobs, initializer=_winit,
                             initargs=(corpus, target_dir, per, force, dry)) as ex:
        futs = {ex.submit(_wtask, m): m for m in mods}
        for fu in as_completed(futs):
            m = futs[fu]
            try:
                _m, st, n, nr, comp = fu.result()
            except Exception as e:
                st, n, nr, comp = "ERR:" + str(e)[:48], 0, 0, ""
            done += 1
            if st == "ok":
                tot["mods"] += 1; tot["funcs"] += n; tot["readable"] += nr
            if progress:
                progress(done, total, m, tot)
            if st == "ok" or st.startswith("ERR"):
                print("  [%d/%d] %-18s -> %-22s %s" % (done, total, m, comp,
                      ("%d/%d" % (nr, n) if st == "ok" else st)), flush=True)
    _summary(tot)


def _summary(tot):
    print("=" * 64)
    print("RECONSTRUCTION -> TREE: %d modules placed, %d funcs, %d readable (%.0f%%)" %
          (tot["mods"], tot["funcs"], tot["readable"], 100.0 * tot["readable"] / (tot["funcs"] or 1)))


from src.cli.common import opt as _opt


def main():
    argv = sys.argv[1:]
    if argv and argv[0] in ("-h", "--help"):
        print(__doc__); return
    from src.core import config
    config.apply_config()
    per = int(_opt(argv, "--per") or 80)
    jobs = int(_opt(argv, "--jobs") or 1)
    force = "--force" in argv
    dry = "--dry" in argv
    mods = _opt(argv, "--mods"); mods = mods.split(",") if mods else None
    print("full reconstruction -> authored tree (per=%d jobs=%d force=%s dry=%s) root=%s"
          % (per, jobs, force, dry, placement.tree_root()), flush=True)
    run(per=per, mods=mods, jobs=jobs, force=force, dry=dry)


if __name__ == "__main__":
    main()
