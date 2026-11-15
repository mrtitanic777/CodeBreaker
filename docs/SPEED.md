# Speed: where a measure's time goes

Measured 2026-11-14 with `codebreaker measure <module>` in a fresh state folder (cold), then again at once (warm).
The machine was shared with other work (16 threads, about 60% busy), so times are good to roughly +-20%.
`measure` already prints the stage split: `(Ns: decompile, build, judge)`. `CB_PROFILE=1` adds a table of the
32-bit judge's stages on stderr (native/cb/verify/stage_timer.hpp; off by default, one cached bool when off).
A judging worker process prints its own table.

## Workloads

| Workload | Functions | Cold wall | decompile | build | judge | Warm wall | Warm judged |
|---|---|---|---|---|---|---|---|
| lz32.dll (32-bit thunk, 39 fns) | 39 | 3 s | 1 | 1 | 2 | 1.1 s | 0 |
| mpr.dll (32-bit, 40 KB) | 227 | 42-47 s | 2 | 3 | 42 | 1.2 s | 0 |
| msacm32.dll (32-bit, 92 KB) | 273 | 53 s | 9 | 3 | 41 | 2.5 s | 0 |
| lzexpand.dll (16-bit) | 46 | 4.9 s | 1 | 1 | 3 | 0.3 s | 0 |
| commdlg.dll (16-bit, 98 KB) | 207 | 26 s | 4 | 5 | 17 | 1.7 s | 0 |
| PS1 peterlemon/CPUADD.exe | 50 | 0.19 s | 0 | 0 | 0 | 0.11 s | 0 |
| PS1 movie-24bit.exe / test-all.exe | 90 / 203 | 0.57 / 0.36 s | 0 | 0 | 0 | 0.48 / 0.32 s | 0 |

Findings:
* The incremental engine works: a warm re-run judges nothing; what remains is decompiling again (1-2.5 s).
* Judging is already parallel: the 32-bit lane shares names between the parent and up to 4 worker processes
  (engine/lane_kit_workers.cpp), the 16-bit lane uses judge_threads. Per-function results are kept by signature
  and by object-code digest; original fingerprints are cached across runs.
* PS1 is not a speed problem (sub-second).
* The 32-bit judge is the cost. mpr.dll, `CB_PROFILE=1`, per process (about 35-50 functions each, five processes):
  judge.function 34 s = run.original 15-18 s (1,900-3,100 uncached runs) + run.rebuilt 6-9 s (800-1,000)
  + the explorer's observed runs 9-12 s (1,300-2,400); judge.directed (11-14 s) and judge.explore (10-14 s) are
  mostly those runs. About 5,000-7,000 emulator runs per process, 145 per function.
* Inside a run (temporary counters, since removed): 95% of a run's time is in Unicorn's start call, and the
  setup, hook-recording, collection and page restore total under 5%. Runs split in two kinds:
  - 16% of runs hit the 20,000 instruction cap: 0.3 us per instruction, 7 ms each, about 2/3 of all instructions
    but 20% of the time.
  - the other 84% run about 700 instructions and still take 4-5 ms: about 6 us per instruction, a fixed cost
    that does not scale with the instructions run. A plain Unicorn run costs 4 us here (micro-benchmark), and
    translating a block costs 3.3 us; so each run pays for something that is not emulation, most likely code
    translated again for every run (translation blocks dropped, or hooks changing them). Adding or removing a
    CODE hook flushes the translation cache (measured: 5 us -> 900 us for a 500-instruction function); the branch-logged runs
    do that, and a run that must stop exactly at the cap does it explicitly (UC_CTL_TB_FLUSH in
    native_trace.cpp). The cause for the ordinary runs is not yet pinned down.

## Opportunities, by expected gain

1. Find and remove the fixed 4-5 ms per ordinary run (about 70% of 32-bit judge time). Expected: judging 2-3x
   faster. First step: count translation-block builds per run (UC_CTL_TB_REQUEST_CACHE at entry), then keep one
   hook set per laid-out machine across runs (hooks keyed by the machine, the per-run state behind a stable
   pointer) so translated blocks survive from run to run. The verdicts must not change: the same runs, the same
   fingerprints. Lives in emulate/ (tracer), not core.
2. Branch-logged and shadow runs (observed): the CODE hook on the function's range is added per run, which
   flushes translated code each time. A full-range hook that ignores addresses outside the range, left attached,
   removes it. 15-20% of judge time.
3. Cache the decompile of an unchanged module (key: module hash and decompiler revision). Warm re-runs spend
   1-2.5 s of their 1.1-2.5 s in decompile; across a sweep of 566 modules that is 10-20 minutes of nothing.
4. Fewer runs: the original's runs for the explorer are re-made by `run.observed` after `run.original` has made
   the same inputs (original fingerprint and branch log of one input are two runs). One run could give both.
   Needs the branch log in the fingerprint run (hook cost) -- measure first.
5. Start the judging workers while the build runs (they wait on the parent's build today: `Prepared` and the
   build are serial, 5-10 s of a 50 s measure on larger modules), and share the platform-independent
   context set-up across them.

Not worth doing: the truncated runs (already 0.3 us per instruction, and the cap is part of the verdict);
PS1; the 16-bit lane's 17 s on commdlg (0.09 s per function, already threaded).

## Status

Phase 1 (this file, `CB_PROFILE` stage timer) done. No optimization has been made yet: the tracer fix (1) needs a
build-test loop on the shared build folder, which was blocked for long stretches today. No verdict can have
changed: only the stage timer was added.
