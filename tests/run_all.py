#!/usr/bin/env python3
"""Run the test suites and exit non-zero if any of them fails.

    codebreaker.exe tests/run_all.py                    # all suites
    codebreaker.exe tests/run_all.py --quick            # skip the slower end-to-end suites (the merge gate)
    codebreaker.exe tests/run_all.py --changed [BASE]   # only the suites that depend on what changed since BASE
    codebreaker.exe tests/run_all.py --plan             # with --changed: say what would run, run nothing
    codebreaker.exe tests/run_all.py --jobs N           # N jobs side by side (default: settings.json "workers")
    codebreaker.exe tests/run_all.py --no-cache         # run the compilers instead of asking the compile cache
    codebreaker.exe tests/run_all.py --no-build         # run the codebreaker.exe there is, even if it is stale

Use this as the gate before committing:  codebreaker.exe tests/run_all.py && git commit ...
(Piping a single suite through `tail` hides its exit code; this script does not.)

The program under test is codebreaker.exe (state/cxx/codebreaker/build, or CB_EXE): every suite runs in it,
whatever runs this script, and there is no python fallback. It is built from the sources, the tests among
them, so a codebreaker.exe older than any of them is rebuilt first (tools/build_codebreaker_exe.cmd,
incremental) unless --no-build.

Side by side.  Suites run in parallel, up to the `workers` setting (CB_JOBS, --jobs override it); every job
gets CB_JOBS=1, so `workers` processes are busy in all. tests/translate and tests/java (pytest folders) are
one job per test file, longest first (times of the last run: state/test-speed/times.json). Suites with a
group name in SUITES share something (a state folder, the DOSBox-X emulator, a service's port) and run one
after another within it.

--changed [BASE].  BASE is a git revision (default: the merge base with master). The files that differ from it
(committed, edited, untracked) go to native/testkit's `plan`, which places each on the dependency map and says
which suites reach it: the import graph tools/check_imports.py walks, plus what the last good run of each
suite loaded (state/test-speed/deps.json, written by every run), plus the data files a suite reads; a change
the map cannot place (or no testkit.exe) runs everything. --changed composes with --quick.

Compile cache.  run_all.py is also the pytest plugin (-p run_all) that, for g++ and javac, asks
`testkit lookup` whether the run was done before (same compiler bytes, flags, preprocessed source / sources
and class path) and puts its outputs in place instead of compiling; a run that succeeded and printed
nothing is stored (`testkit store`). state/test-cache, pruned to CB_TEST_CACHE_MB (4096); CB_TEST_CACHE=0
or --no-cache turns it off. The full --quick run stays the merge gate."""
import concurrent.futures as cf
import json
import os
import re
import subprocess
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
KIT = os.path.join(ROOT, "native", "testkit", "build", "bin", "testkit.exe")
EXE = os.environ.get("CB_EXE") or os.path.join(ROOT, "state", "cxx", "codebreaker", "build", "codebreaker.exe")
BUILT_FROM = ("codebreaker", "tests", "tools", os.path.join("native", "typed"), os.path.join("native", "cbexe"))

# (file, slow, group). dosbox: suites that start DOSBox-X (one emulator at a time); service: servers on a
# port. (Suites with a state folder of their own, in a temporary folder, need no group.)
SUITES = [("../tools/check_imports.py", False, ""), ("../tools/tidy.py", False, ""),
          ("test_plumbing.py", False, ""), ("test_ui.py", False, "service"), ("test_store.py", False, ""),
          ("test_mcp.py", False, "service"), ("test_service.py", False, "service"), ("test_symbols.py", False, ""),
          ("test_dos.py", False, "dosbox"), ("test_analysis.py", False, ""), ("test_drivers.py", False, "dosbox"),
          ("test_versions.py", False, ""), ("test_powerpc.py", False, ""), ("test_xex.py", False, ""),
          ("test_vmx128.py", False, ""), ("test_match_powerpc.py", False, ""), ("test_xenos.py", False, ""),
          ("test_platforms.py", False, ""), ("test_nes.py", False, ""), ("test_z80.py", False, ""),
          ("test_m68k.py", False, ""), ("test_xbe.py", False, ""), ("test_recompile.py", False, ""),
          ("test_assembler.py", False, ""), ("test_media.py", False, ""), ("test_xbox_formats.py", False, ""),
          ("test_xbox_crypto.py", False, ""), ("test_xex_edit.py", False, ""), ("test_ppc_calls.py", False, ""),
          ("test_xna.py", False, ""), ("test_xbox_effect.py", False, ""), ("test_whole_module.py", False, ""),
          ("test_invert.py", False, "dosbox"), ("test_readable.py", False, ""), ("test_java.py", False, ""),
          ("test_translate.py", False, ""), ("test_concolic.py", False, ""), ("test_run_record.py", False, ""),
          ("test_native_trace.py", False, ""), ("test_native_engine.py", False, ""),
          ("test_frozen.py", False, ""), ("test_port.py", False, ""), ("test_converted.py", False, ""),
          ("test_exepack.py", False, ""), ("test_pklite.py", False, ""), ("test_x360.py", True, ""),
          ("test_frozen_slow.py", True, ""), ("test_verification.py", True, "")]
SHARDED = {"test_java.py": "java", "test_translate.py": "translate"}      # pytest folders run a file at a time
READS_EVERY_PY = ("../tools/check_imports.py", "../tools/tidy.py")


def state_dir(*parts):
    return os.path.join(os.environ.get("CB_STATE") or os.path.join(ROOT, "state"), *parts)


def read_json(path):
    try:
        with open(path, encoding="utf-8") as f:
            return json.load(f)
    except (OSError, ValueError):
        return {}


def write_json(path, value):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    tmp = "%s.%d.tmp" % (path, os.getpid())
    with open(tmp, "w", encoding="utf-8") as f:
        json.dump(value, f, sort_keys=True)
    os.replace(tmp, path)


def job_count(argv=()):
    if "--jobs" in argv:
        return max(1, int(argv[argv.index("--jobs") + 1]))
    sys.path.insert(0, ROOT)
    from codebreaker.base import parallel
    return max(1, parallel.workers())


# ---------------------------------------------------------------------------------------------------
# Jobs: a command, the suite it belongs to, the group it must not share a moment with.
# ---------------------------------------------------------------------------------------------------
def launch_pytest(target, tmp):
    os.makedirs(os.path.dirname(tmp), exist_ok=True)               # (pytest makes the basetemp, not its parent)
    return [EXE, "-m", "pytest", "-q", "-p", "no:cacheprovider", "-p", "run_all", "--basetemp=" + tmp, target]


def child_env(argv=()):
    env = dict(os.environ, CB_NO_FP_CACHE="1", CB_JOBS="1",           # verdicts from the current code; one worker a job
               PYTHONPATH=os.pathsep.join(p for p in (HERE, os.environ.get("PYTHONPATH")) if p))
    if "--no-cache" in argv:
        env["CB_TEST_CACHE"] = "0"
    return env


def trace_path(label):
    return state_dir("test-speed", "trace", re.sub(r"[^\w.-]", "_", label) + ".json")


def shard_jobs(name, folder, times, n):
    """The test files of tests/<folder> in 4n bins of about equal time (times of the last run; file size the
    first time), longest first. A bin is pytest over the whole folder with CB_TEST_ONLY naming its files: the
    plugin deselects the rest, so every module is still imported (some tests rely on what another module
    registers at import) but only the bin's tests run. Each pytest run has a basetemp of its own."""
    files = sorted(f for f in os.listdir(os.path.join(HERE, folder)) if f.startswith("test_") and f.endswith(".py"))
    weight = lambda f: times.get("%s/%s" % (folder, f), os.path.getsize(os.path.join(HERE, folder, f)) / 4000.0)
    bins = [[] for _ in range(max(2, min(len(files), 4 * n)))]
    load = [0.0] * len(bins)
    for f in sorted(files, key=weight, reverse=True):
        i = load.index(min(load))
        bins[i].append(f)
        load[i] += weight(f)
    return [dict(suite=name, label="%s/bin%d" % (folder, i), group="", est=load[i], trace=trace_path("%s/bin%d" % (folder, i)),
                 env={"CB_TEST_ONLY": ",".join(b)}, cmd=launch_pytest(os.path.join("tests", folder), state_dir("test-tmp", name, "bin%d" % i)))
            for i, b in sorted(enumerate(bins), key=lambda x: -load[x[0]]) if b]


def suite_jobs(name, group, times, n):
    if name in SHARDED:
        return shard_jobs(name, SHARDED[name], times, n)
    path = os.path.join(HERE, name)
    with open(path, encoding="utf-8") as f:                      # a file of pytest tests only: run by pytest
        own = "__main__" in f.read()
    # a script reports what it imported through the interpreter (-X importtime), a pytest run through the plugin
    cmd = [EXE, "-X", "importtime", path] if own else launch_pytest(path, state_dir("test-tmp", name))
    return [dict(suite=name, label=name, group=group, est=times.get(name, 5.0), cmd=cmd, trace=None if own else trace_path(name))]


def run_job(job, env):
    t0 = time.time()
    if job["trace"]:
        os.makedirs(os.path.dirname(job["trace"]), exist_ok=True)
        if os.path.exists(job["trace"]):
            os.remove(job["trace"])
        env = dict(env, CB_TEST_TRACE=job["trace"])
    env = dict(env, **job.get("env", {}))
    r = subprocess.run(job["cmd"], capture_output=True, text=True, errors="replace", env=env, cwd=ROOT)
    job["file_secs"] = read_json(job["trace"]).get("secs", {}) if job["trace"] else {}
    err = (r.stderr or "").splitlines()
    if job["trace"]:
        job["loaded"] = read_json(job["trace"]).get("files")
    else:                                                         # -X importtime: "import time: self | cum | name"
        job["loaded"] = [ln.rsplit("|", 1)[-1].strip() for ln in err if ln.startswith("import time:")]
        r.stderr = "\n".join(ln for ln in err if not ln.startswith("import time:"))
    return r, time.time() - t0


def run_jobs(jobs, n, env, done):
    """The jobs, n at a time, a group's jobs one after another; done(job, result, seconds) as each ends."""
    pending = sorted(jobs, key=lambda j: -j["est"])
    running, busy = {}, set()
    with cf.ThreadPoolExecutor(max_workers=n) as pool:
        while pending or running:
            for j in list(pending):
                if len(running) >= n:
                    break
                if j["group"] and j["group"] in busy:
                    continue
                pending.remove(j)
                busy.add(j["group"])
                running[pool.submit(run_job, j, env)] = j
            finished, _ = cf.wait(running, return_when=cf.FIRST_COMPLETED)
            for fut in finished:
                j = running.pop(fut)
                busy.discard(j["group"])
                done(j, *fut.result())


def summarize_pytest(results):
    """(passed, skipped, failed, [failing lines]) over finished pytest jobs."""
    count = lambda word, line: int((re.search(r"(\d+) %s" % word, line) or [0, 0])[1])
    passed = skipped = failed = 0
    bad = []
    for r in results:
        out = (r.stdout or "") + (r.stderr or "")
        last = out.strip().splitlines()[-1] if out.strip() else ""
        passed, skipped = passed + count("passed", last), skipped + count("skipped", last)
        n = count("failed", last) + count("error", last)
        failed += n
        if r.returncode and not n:                                # exited badly without a count (collection broke)
            failed += 1
            bad.append("  FAIL " + (last or "pytest exited %s" % r.returncode)[:200])
        bad += ["  FAIL " + ln.split(" ", 1)[1] for ln in out.splitlines() if ln.startswith(("FAILED", "ERROR")) and " " in ln]
    return passed, skipped, failed, bad


def run_pytest_folder(folder):
    """What tests/test_java.py and tests/test_translate.py run: a pytest job per test file, `workers` at a time."""
    results, seen = [], {}
    run_jobs(shard_jobs("test_%s.py" % folder, folder, read_json(state_dir("test-speed", "times.json")), job_count()), job_count(),
             child_env(sys.argv), lambda job, r, secs: (results.append(r), seen.update({k.replace("tests/", "", 1): v for k, v in job["file_secs"].items()})))
    write_json(state_dir("test-speed", "times.json"), {**read_json(state_dir("test-speed", "times.json")), **seen})
    passed, skipped, failed, bad = summarize_pytest(results)
    print("\n".join(bad + ["%d passed, %d skipped, %d failed" % (passed, skipped, failed)]))
    return 1 if failed else 0


# ---------------------------------------------------------------------------------------------------
# The compile cache, as a pytest plugin: native/testkit does the keying and the storing.
# ---------------------------------------------------------------------------------------------------
_REAL = None
_COMPILERS = {"g++", "gcc", "c++", "cc", "clang", "clang++", "javac"}


def cache_dir():
    return os.environ.get("CB_TEST_CACHE_DIR") or state_dir("test-cache")


def _cached_run(*popenargs, **kw):
    args = popenargs[0] if popenargs else kw.get("args")
    name = os.path.splitext(os.path.basename(str(args[0])))[0].lower() if isinstance(args, (list, tuple)) and args else ""
    if name not in _COMPILERS or kw.get("input") is not None or kw.get("shell"):
        return _REAL(*popenargs, **kw)
    ask = [KIT, "lookup", "--cache", cache_dir(), "--cwd", os.path.abspath(kw.get("cwd") or os.getcwd()), "--"] + [str(a) for a in args]
    p = _REAL(ask, capture_output=True, text=True)
    word = p.stdout.split()
    with open(os.path.join(cache_dir(), "stats.log"), "a") as f:
        f.write((word[0] if word else "SKIP") + "\n")
    if p.returncode == 0:                                         # a hit: the outputs are in place
        text = kw.get("text") or kw.get("universal_newlines") or kw.get("encoding") or kw.get("errors")
        out = lambda s: ("" if text else b"") if kw.get("capture_output") or kw.get(s) == subprocess.PIPE else None
        return subprocess.CompletedProcess(args, 0, out("stdout"), out("stderr"))
    r = _REAL(*popenargs, **kw)
    if p.returncode == 1 and r.returncode == 0 and not r.stdout and not r.stderr:
        _REAL([KIT, "store"] + ask[2:6] + ["--key", word[1]] + (["--dkey", word[2]] if len(word) > 2 else []) + ask[6:], capture_output=True)
    return r


def pytest_configure(config):
    global _REAL
    if os.environ.get("CB_TEST_CACHE") != "0" and os.path.isfile(KIT) and subprocess.run is not _cached_run:
        os.makedirs(cache_dir(), exist_ok=True)
        _REAL, subprocess.run = subprocess.run, _cached_run


def pytest_collection_modifyitems(config, items):
    only = os.environ.get("CB_TEST_ONLY")                         # a bin of a big folder: run only these files' tests
    if only:
        keep = set(only.split(","))
        gone = [i for i in items if os.path.basename(str(i.fspath)) not in keep]
        items[:] = [i for i in items if os.path.basename(str(i.fspath)) in keep]
        config.hook.pytest_deselected(items=gone)


_SECS = {}


def pytest_runtest_logreport(report):
    f = report.nodeid.split("::")[0].replace("\\", "/")
    _SECS[f] = _SECS.get(f, 0.0) + report.duration


def pytest_sessionfinish(session, exitstatus):
    if os.environ.get("CB_TEST_TRACE"):                           # which of the repo's files this run loaded
        root = os.path.normcase(ROOT) + os.sep
        write_json(os.environ["CB_TEST_TRACE"], {"files": sorted(
            os.path.relpath(m.__file__, ROOT).replace(os.sep, "/") for m in list(sys.modules.values())
            if os.path.normcase(str(getattr(m, "__file__", None) or "")).startswith(root) and m.__file__.endswith(".py")),
            "secs": {k: round(v, 1) for k, v in _SECS.items()}})


def cache_report():
    log = os.path.join(cache_dir(), "stats.log")
    try:
        with open(log) as f:
            words = f.read().split()
        os.remove(log)
    except OSError:
        return ""
    return "compile cache: %d hits, %d compiled, %d not cacheable" % (words.count("HIT"), words.count("MISS"), words.count("SKIP"))


# ---------------------------------------------------------------------------------------------------
def changed_suites(base, names):
    """-> (the suites native/testkit says depend on what changed since base, None) or (None, why: run all)."""
    git = lambda *a: subprocess.run(["git", "-C", ROOT] + list(a), capture_output=True, text=True, errors="replace")
    if not base:
        base = next((r.stdout.strip() for r in (git("merge-base", "HEAD", ref) for ref in ("master", "origin/master")) if r.returncode == 0), "")
    diff = git("diff", "--name-status", "--no-renames", base)
    if not base or diff.returncode or not os.path.isfile(KIT):
        return None, "git or testkit.exe is not there"
    rows = [("D" if ln[0] == "D" else "M") + " " + ln.split("\t", 1)[1] for ln in diff.stdout.splitlines() if "\t" in ln]
    rows += ["M " + p for p in git("ls-files", "--others", "--exclude-standard").stdout.split("\n") if p.strip()]
    work = state_dir("test-speed")
    os.makedirs(work, exist_ok=True)
    for fname, text in (("changed.txt", rows), ("suites.txt", ["%s\t%s\t%s" % (n, os.path.relpath(os.path.join(HERE, n), ROOT).replace(os.sep, "/"),
                                                                      "py" if n in READS_EVERY_PY else "") for n in names])):
        with open(os.path.join(work, fname), "w", encoding="utf-8") as f:
            f.write("\n".join(text) + "\n")
    deps = read_json(state_dir("test-speed", "deps.json"))
    with open(os.path.join(work, "deps.txt"), "w", encoding="utf-8") as f:
        f.write("".join("%s\t%s\n" % (k, "\t".join(v)) for k, v in deps.items() if v))
    out = subprocess.run([KIT, "plan", "--root", ROOT, "--changed", os.path.join(work, "changed.txt"), "--suites", os.path.join(work, "suites.txt"),
                          "--deps", os.path.join(work, "deps.txt")], capture_output=True, text=True).stdout.splitlines()
    print("-- changed since %s: %d files" % (base[:10], len(rows)))
    if any(ln.startswith("ALL") for ln in out):
        return None, out[0][4:]
    return {ln[4:] for ln in out if ln.startswith("RUN ")}, None


def stale_source():
    """-> a source file newer than codebreaker.exe (None: it is current), or the exe's path if there is none."""
    if not os.path.isfile(EXE):
        return EXE
    built = os.path.getmtime(EXE)
    for top in BUILT_FROM:
        for d, dirs, files in os.walk(os.path.join(ROOT, top)):
            dirs[:] = [x for x in dirs if x not in ("__pycache__", "build")]
            for f in files:
                if f.endswith((".py", ".json", ".hpp", ".cpp", ".cc", ".h")) and os.path.getmtime(os.path.join(d, f)) > built:
                    return os.path.join(d, f)
    return None


def ensure_exe(argv):
    """codebreaker.exe current with the sources (built if not), or exit: the suites run nothing else."""
    why = stale_source()
    if why is None:
        return
    if "--no-build" in argv and os.path.isfile(EXE):
        print("-- codebreaker.exe is older than %s (--no-build: run anyway)" % os.path.relpath(why, ROOT))
        return
    print("-- building codebreaker.exe (%s)" % ("none yet" if why == EXE else "older than " + os.path.relpath(why, ROOT)), flush=True)
    r = subprocess.run(["cmd", "/c", os.path.join(ROOT, "tools", "build_codebreaker_exe.cmd")], cwd=ROOT,
                       capture_output=True, text=True, errors="replace")
    if r.returncode or not os.path.isfile(EXE):
        print("\n".join(((r.stdout or "") + (r.stderr or "")).strip().splitlines()[-30:]))
        print("FAILED: codebreaker.exe did not build")
        sys.exit(2)


def main(argv):
    quick = "--quick" in argv
    suites = [(n, g) for n, slow, g in SUITES if not (quick and slow)]
    for n, slow, g in SUITES:
        if quick and slow:
            print("-- %-28s skipped (--quick)" % n)
    if "--changed" in argv:
        i = argv.index("--changed")
        only, why = changed_suites(argv[i + 1] if i + 1 < len(argv) and not argv[i + 1].startswith("--") else None, [n for n, _g in suites])
        if only is None:
            print("-- running everything (%s)" % why)
        else:
            print("-- depends on the change: %s\n-- not run: %s" % (", ".join(n for n, _g in suites if n in only) or "no suite",
                                                                   ", ".join(n for n, _g in suites if n not in only) or "none"))
            suites = [(n, g) for n, g in suites if n in only]
    if "--plan" in argv:
        return 0
    ensure_exe(argv)
    times = read_json(state_dir("test-speed", "times.json"))
    t0, n = time.time(), job_count(argv)
    jobs = [j for name, group in suites for j in suite_jobs(name, group, times, n)]
    count = {}
    for j in jobs:
        count[j["suite"]] = count.get(j["suite"], 0) + 1
    finished, failed, seen, traced = {}, [], {}, {}

    def done(job, r, secs):
        seen[job["label"]] = round(secs, 1)
        seen.update({k.replace("tests/", "", 1): v for k, v in job["file_secs"].items()})     # (java/test_x.py: seconds)
        name = job["suite"]
        finished.setdefault(name, []).append((job, r, secs))
        if len(finished[name]) < count[name]:
            return
        results = [r for _j, r, _s in finished[name]]
        total = sum(s for _j, _r, s in finished[name])
        if name in SHARDED:
            p, sk, fl, bad = summarize_pytest(results)
            summary, ok = "%d passed, %d skipped, %d failed" % (p, sk, fl), fl == 0
        else:
            r = results[0]
            lines = [ln for ln in (r.stdout or "").strip().splitlines() if ln.strip()]
            summary = next((ln.strip() for ln in reversed(lines) if "passed" in ln), "(no summary line)")
            ok = r.returncode == 0
            bad = ["     " + ln.strip()[:200] for ln in lines if "FAIL" in ln or ln.startswith("       ")]
            if not ok and (r.stderr or "").strip():
                bad.append("     " + r.stderr.strip().splitlines()[-1][:200])
        print("%s %-28s %s  (%.0fs)" % ("ok" if ok else "FAIL", name, summary, total), flush=True)
        if not ok:
            failed.append(name)
            print("\n".join(bad), flush=True)
        elif all(j["loaded"] for j, _r, _s in finished[name]):       # what a good run loaded: --changed's map
            traced[name] = sorted(set().union(*[j["loaded"] for j, _r, _s in finished[name]]))
    run_jobs(jobs, n, child_env(argv), done)
    if jobs:
        seen.update({k: round(sum(s for _j, _r, s in v), 1) for k, v in finished.items()})
        write_json(state_dir("test-speed", "times.json"), {**read_json(state_dir("test-speed", "times.json")), **seen})
    if traced:
        write_json(state_dir("test-speed", "deps.json"), {**read_json(state_dir("test-speed", "deps.json")), **traced})
    print("%d suites in %.0fs with %d side by side" % (len(suites), time.time() - t0, n))
    print(cache_report())
    if os.path.isfile(KIT):
        subprocess.run([KIT, "prune", "--cache", cache_dir(), "--mb", os.environ.get("CB_TEST_CACHE_MB", "4096")])
    print("ALL SUITES PASSED" if not failed else "FAILED: " + ", ".join(sorted(failed, key=[n for n, _g in suites].index)))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
