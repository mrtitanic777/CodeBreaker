# Speed: where a measure's time goes

Measured 2026-11-14 with `codebreaker measure <module>` in a fresh state folder (cold), then again at once (warm).
The machine was shared with other work (16 threads, about 60% busy), so times are good to roughly +-20%.
`measure` already prints the stage split: `(Ns: decompile, build, judge)`. `CB_PROFILE=1` adds a table of the
32-bit judge's stages on stderr (src/verify/stage_timer.hpp; off by default, one cached bool when off).
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

## Phase 2: what was changed, and what it gave

Cause found (micro-benchmarks against Unicorn alone, then counters in the tracer): a run on a kept machine paid about
4 ms that was not emulation, because the translated code was thrown away again and again.
* Unicorn drops its translated code when a code hook is added, and a run that is ended by `uc_emu_stop` costs the
  next ones their translation too (measured: 28 us for a 100-block function run to its end address, 830 us when the same
  run is ended by a stop from a hook).
* Every run put its hooks on and took them off; every run that has to go one instruction at a time (a block the
  recorder cannot take whole, a call into an import's stand-in, the instruction cap) stopped the engine, added the
  instruction hook, flushed all translated code and started again. In msacm32.dll 40% of the runs were such runs
  (most of them calls into nothing that stop after about 30 instructions), each costing 4-8 ms.

Changes (all in src/emulate, none in core):
1. `native_trace::HookHost` (kept by each machine): the hooks stay on the engine between runs while the runs ask for
   the same set; the per-run state is the host's one object, made new for each run. Plain runs: 4-5 ms -> 0.15-0.6 ms.
2. The instruction hook stays on too, unarmed (`code_armed`), and a run that has to go one instruction at a time arms
   it and goes on where it is: no stop, no flush, no second start. (Without a host, the old way is unchanged.)
3. Machines are kept apart by whether they log branches (a machine of each: the branch hook is on a function's range
   and changes with the function), up to 6 kept machines (was 4).
4. Because translated code now outlives a run: `forget_code` throws away what was translated from a page the host
   wrote (memory the run was given, pages put back after a run, pages unmapped again). Unicorn does not drop code for
   host writes: without this a call into scratch memory ran stale code and one function in msacm32.dll (7db6188c)
   changed verdict from run to run. A test covers the mix (emulator_test: a_kept_machine_keeps_its_hooks...).
5. Within a run, the recorder looks a straight run up in a small per-run table before it takes the cache's lock.

Timings (`codebreaker measure`, fresh state folder, cold; the machine was shared and loaded 60-70%, so ranges):

| Module | Before | After | Judge CPU (CB_PROFILE judge.function, per process) |
|---|---|---|---|
| lz32.dll | 3.2 s | 2.4-3.2 s | -- |
| mpr.dll | 42-59 s | 25-35 s | 34 s -> about 15 s |
| msacm32.dll | 53-65 s | 38-48 s | 35 s -> about 16 s |
| lzexpand.dll, commdlg.dll (16-bit) | 4.4 / 23.6 s | same (the 16-bit lane does not use this tracer) | -- |

What is left on msacm32.dll's 38-48 s: decompile 11 s and build 4 s, both serial and before any judging starts; the
judging itself is now about 16 s.

Proof that verdicts are unchanged: for each of lz32, mpr, msacm32, lzexpand and commdlg a fresh state folder was measured
before and after, and the program database was dumped (every function: address, signature, verdict, reason, limits,
detail, carried). The `measure` text is identical (apart from times) and the rows are identical, with one
normalisation: the `paths=N` count in limits, which varies from run to run on the unchanged code (the explorer's
path count, e.g. mpr 7fd05a67: 29 / 26 / 29 on three runs of one binary). The 32-bit modules match the pre-change dump
row for row, three runs of msacm32 included. commdlg (16-bit) differs from the pre-change dump in 3 rows only because
the NE decompiler was changed by other work in between (variable names in the source text); it is identical between
the runs before and after this tracer change.

Not done: opportunity 3 (cache the decompile of an unchanged module). The decompile depends on the knowledge base
(names and prototypes that propagate), so a cache key of the module hash and the decompiler revision is not enough,
and a key that is enough needs the knowledge base's own version; left until that exists. Opportunities 4 and 5 are
open. The 1-2.5 s warm re-run (decompile only) is the remaining cost of an unchanged module.

## Phase 3: the console-lane source writers (2026-11-15)

`codebreaker reach --judge` on a console program is `codebreaker measure` of it, and for the console lanes that is the source writer:
discover the code, write the assembly, assemble it again, compare with the original. `CB_PROFILE=1` now times the stages of the 3DS,
Switch and MIPS (PSP, PS2) writers (`n3ds.*`, `switch.*`, `mips.*` in src/source). The proof harness is
`cb_tests_source --filter source.writers_speed` with `CB_WRITER_SPEED_FILES=a;b` and `CB_WRITER_SPEED_OUT=<folder>`: one line per
file with the writer's seconds and the SHA-256 of the source and of the rebuilt file, to diff before and after.

Hot spots found, each in shared code:

1. The first pass of the ARM, MIPS and 68000 assemblers (`size_guess`) copied the whole label table (an unordered_map of every label of
   the program) for every instruction that names a label defined further on: quadratic in the program. Now
   `assemble_with_guesses` (build/asm_text.hpp) adds the guesses to the table and takes them out again; same answer, no copy. This
   was 98% of the writer time for the 3DS (arm), the PSP (mips, DOSBox 185 s of 185 s) and is the same code for the DS, GBA, N64, PS1,
   PS2, Genesis, CD-i, Jaguar, Neo Geo and Sega CD writers.
2. The Switch writer's search for functions nothing calls walked, for each candidate, everything the candidate reaches including all
   code already found (511 candidates re-walked 2.4 million instructions in ftpd.nro; 24 s of NX-Shell's 44 s). `image_writer::walk_beyond`
   walks only code not yet found and stops following found code once the six instructions the test asks for are counted: the same
   accept/reject decision and the same code, targets and starts (a unit test checks it against `walk` for every entry of a toy program).
3. The per-instruction "does this text assemble back to its word" check of the Switch and 3DS writers is independent per instruction:
   it now runs on `parallel::parallel_for` (workers(): CB_JOBS and low_load apply) and is filed in address order.
4. Memory: the Switch writer held a 48-byte tree node per byte of the asset section and per byte inside an instruction;
   `image_writer::ByteMarks` is one bit per byte.

Timings, writer wall seconds, cold, one process, CB_JOBS=4, the machine shared with other agents (so good to about +-20%).
Rebuilt files and sources are byte-identical before and after (same SHA-256 for every file below).

| Program | Before | After | Peak memory before / after |
|---|---|---|---|
| PSP dosbox/EBOOT.PBP (3 MB, 14,975 fns) | 185 s (reported 25+ min earlier) | 3.4 s | 208 / 203 MB |
| 3DS Universal-Updater.3dsx (3 MB, 5,629 fns) | 102 s | 3.2 s | 308 / 319 MB |
| 3DS FBI.3dsx (1.2 MB, 4,221 fns) | 25 s | 1.5 s | 158 / 163 MB |
| Switch NX-Shell.nro (8.4 MB, 14,580 fns) | 44 s | 10.4 s | 1,297 / 1,342 MB (before ByteMarks) |
| Switch ftpd.nro (1.5 MB, 2,357 fns) | 3.0 s | 1.5 s | -- |
| Wii U SuDokuL.rpx (3.5 MB, 6,645 fns; `measure`) | 528 s | 5.5 s | -- / 374 MB |

`reach --judge --per-platform 10 --max-size 16384` over homebrew-samples, one fresh state folder per column, wall time per platform
(the Wii U / GameCube / Wii writer assembles with the MIPS assembler's code, so it had the same copy): Switch (3 files) 26 s -> 14 s,
3DS (4 files, one an ELF of 5 MB) 358 s -> 12 s, Wii U (3 files) 697 s -> 10 s, Saturn (3 files) 1 s -> 1 s. The four result tables
(`state/reach/r_<platform>.json` as printed) are identical before and after, line for line. (The `after` run used the exe built before the
parallel text check and ByteMarks, the two smaller changes.)

Then: the AArch64 assembler's second pass assembles the instructions side by side (a serial walk finds each line's address, the bytes are put in
place in line order, the first fault in line order is the one reported). NX-Shell's `switch.assemble` 5.6 s -> 2.9 s, the whole writer 10.4 s ->
6.8 s, peak memory 1,297 -> 1,136 MB (with ByteMarks); the same SHA-256 for source and rebuilt file. Unit tests rerun after these edits:
cb_tests_build `asm` (141), cb_tests_source `writer` (266) and `homebrew` (16), `image_writer` (21): all pass.

What is left in the writers (NX-Shell, 6.8 s): the a64 assembler's first pass and second-pass assembling 2.9 s, text check 1.4 s, listing 1.3 s.
The ARM and MIPS assemblers' passes are still serial (their first pass sizes instructions that name later labels).

Not done: item 3 of the first list (decompile cache). Its key problem (the knowledge base's version) is unchanged and this phase did not touch it.
