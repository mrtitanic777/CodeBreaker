#!/usr/bin/env python3
"""Run the test suites and exit non-zero if any of them fails.

    codebreaker.exe tests/run_all.py                    # all suites
    codebreaker.exe tests/run_all.py --quick            # skip the slower end-to-end suites (the merge gate)
    codebreaker.exe tests/run_all.py --changed [BASE]   # only the suites that depend on what changed since BASE
    codebreaker.exe tests/run_all.py --plan             # with --changed: say what would run, run nothing
    codebreaker.exe tests/run_all.py --jobs N           # N jobs side by side (default: settings.json "workers")
    codebreaker.exe tests/run_all.py --no-build         # run the codebreaker.exe there is, even if it is stale

Use this as the gate before committing:  codebreaker.exe tests/run_all.py && git commit ...
(Piping a single suite through `tail` hides its exit code; this script does not.)

The program under test is codebreaker.exe (state/cxx/codebreaker/build, or CB_EXE): every suite runs in it,
whatever runs this script, and there is no python fallback. It is built from the sources, the tests among
them, so a codebreaker.exe older than any of them is rebuilt first (tools/build_codebreaker_exe.cmd,
incremental) unless --no-build.

Side by side.  Suites run in parallel, up to the `workers` setting (CB_JOBS, --jobs override it); every job
gets CB_JOBS=1, so `workers` processes are busy in all. Suites with a group name in SUITES share something
(a state folder, a service's port) and run one after another within it. What the suites check is little
now: the rest of the Python is gone (its tests are native: `tools/gate.cmd` runs cb_tests and the command
cases), and the terminal interface's two suites need a real python with Textual, so they skip here.

--changed [BASE].  BASE is a git revision (default: the merge base with master). The files that differ from it
(committed, edited, untracked) go to native/testkit's `plan`, which places each on the dependency map and says
which suites reach it: the import graph tools/check_imports.py walks, plus what the last good run of each
suite loaded (state/test-speed/deps.json, written by every run), plus the data files a suite reads; a change
the map cannot place (or no testkit.exe) runs everything. --changed composes with --quick.

The full --quick run stays the merge gate."""
import concurrent.futures as cf
import json
import os
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
          ("test_ui.py", False, "service"), ("test_ui_thin.py", False, "service")]
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
    if os.environ.get("CB_JOBS", "").isdigit():
        return max(1, int(os.environ["CB_JOBS"]))
    workers = read_json(os.path.join(ROOT, "settings.json")).get("workers")           # (settings.json: "workers", a number)
    return max(1, int(workers)) if isinstance(workers, int) else max(1, (os.cpu_count() or 2) // 2)


# ---------------------------------------------------------------------------------------------------
# Jobs: a command, the suite it belongs to, the group it must not share a moment with.
# ---------------------------------------------------------------------------------------------------
def child_env(argv=()):
    env = dict(os.environ, CB_NO_FP_CACHE="1", CB_JOBS="1",           # verdicts from the current code; one worker a job
               PYTHONPATH=os.pathsep.join(p for p in (HERE, os.environ.get("PYTHONPATH")) if p))
    return env


def suite_jobs(name, group, times, n):
    path = os.path.join(HERE, name)
    # a script reports what it imported through the interpreter (-X importtime)
    return [dict(suite=name, label=name, group=group, est=times.get(name, 5.0), cmd=[EXE, "-X", "importtime", path])]


def run_job(job, env):
    t0 = time.time()
    env = dict(env, **job.get("env", {}))
    r = subprocess.run(job["cmd"], capture_output=True, text=True, errors="replace", env=env, cwd=ROOT)
    err = (r.stderr or "").splitlines()
    job["loaded"] = [ln.rsplit("|", 1)[-1].strip() for ln in err if ln.startswith("import time:")]    # "import time: self | cum | name"
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
        name = job["suite"]
        finished.setdefault(name, []).append((job, r, secs))
        if len(finished[name]) < count[name]:
            return
        results = [r for _j, r, _s in finished[name]]
        total = sum(s for _j, _r, s in finished[name])
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
    print("ALL SUITES PASSED" if not failed else "FAILED: " + ", ".join(sorted(failed, key=[n for n, _g in suites].index)))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
