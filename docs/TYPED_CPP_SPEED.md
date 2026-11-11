# Python vs typed C++: speed tally

A running record of how fast each typed C++ port runs against the original Python on the same proof harness
(`state/work/typed-port/<module>/`: the same real inputs on both sides, outputs compared IDENTICAL). Times are
whole-harness wall times as the porting agent or reviewer measured them on Logan's machine, usually while other
work was running, so treat single numbers as approximate (±20–40%). "Python" includes the interpreter start.

Updated as ports land. Ratio = Python time / C++ time (above 1 = C++ faster).

## Summary (2026-11-10)

| | Count |
|---|---|
| Port harnesses timed (rows below) | 65 |
| C++ faster (ratio > 1.1) | 43 |
| About the same (0.9–1.1) | 8 |
| C++ slower (< 0.9) | 14 |
| Median ratio | ~1.5x faster |

Slower ports are bugs to fix (speed is a product requirement); each has a stated cause below. Several of the 14
were timed before the runtime speed-ups now on staging (regex prefilters, str UTF-8 cache, dict, JSON, RSA,
sha1, makedirs) and are marked "re-time".

## Re-timing after the runtime work (2026-11-11)

Measured on a shared, busy machine (other agents' builds running): wall seconds, two runs each, Python = `python expected.py`,
C++ = `run.cmd` of a clean build (every harness IDENTICAL). Where the machine load made the two runs differ widely, the
CPU time of the test exe is given as well.

| Module | Python (2 runs) | C++ (2 runs) | Now | Cause and fix |
|---|---|---|---|---|
| port.pre_ansi part 5 | 34 / 41 s (CPU 25.5) | CPU 39-42 s (wall 93 / 109 under load) | ~1.6x slower (was 9x: 215 s) | stale runtime objects in the harness (old regex), then: small-block allocator (-25% CPU), py::re ASCII fast paths and inline backtrack stack (-18%) |
| port.pre_ansi part 3 | 38 / 37 s | 9 / 11 s | 3.5x faster | |
| port.asm16 | 8.9 / 8.7 s | 5.3 / 5.3 s | 1.7x faster | |
| source.ne_asm.datacode | 4 / 7 s | 6.7 / 7 s | ~1x (was 0.57x) | allocator |
| source.ne_asm.segment | 204 / 95 s | 375 / 212 s under load; 92 s on a quiet run | ~1x | 70% of the run is waiting for `objdump` (same process on both sides); the typed part is ~25 s |
| symbols.debug_build_estimate | 31 / 67 s | 18.5 / 25.5 s | faster (was 0.7x) | allocator |
| knowledge.show | 11.5 / 11.8 s | 11.9 / 13 s | ~1x (was 0.64x) | allocator |
| exact.toolchain | 9.7 / 51.6 s | 20.5 / 8.8 s | ~1x (noise: real compiles) | |
| base.converted | 0.7 / 1.1 s | 1.2 / 1.0 s | ~1x | process start dominates |
| translate.backends.csharp | 25.3 / 19.7 s | 19.9 / 27.3 s | ~1x (was 0.82x) | allocator; the str-hash cache attempt overflowed the stack here (found by this harness) |
| port.arith16 (real run) | link msc-3.00-p3 modern, 270 files: 186 s | 242 s (clang is 54% of it, same on both) | 1.3x slower (was 10x on the reviewer's run) | JSON bytes parsed directly (no UTF-32 round trip); the rest was the old runtime |
| build.asm16, symbols.prototypes, source.readable | | | not timed | their expected.py needs formats.dll built in the checkout (not built in this worktree) |

Runtime changes behind these (all output-neutral):
- `operator new/delete` (native/l2/src/pystr.cc): per-thread free lists of 16-byte size classes up to 1 KiB, carved from 64 KiB
  chunks of one reserved address range, so delete finds the class from the address (no header, unsized delete works). The
  typed ports make a `std::u32string` per `py::str`; the system heap was ~25% of their CPU. p5: 52 -> 39 s CPU.
- py::re (native/l2/src/pyre.cc): every one-character instruction tests an ASCII byte by its own table (no UTF-8 decode, no
  call), `` ASCII path, a greedy single-character loop followed by a plain ASCII character skips the backtrack positions
  where that character is not (sre's literal-tail check), the backtrack stack is an inline array. Micro-benchmark
  (tools/bench_re.cc vs tools/bench_re.py on 295 C files, 18 patterns): 704 -> 290 ms against Python's 525 ms.
- ctext.blank's cache looks up by hash without copying the (large) text.
- tools/prof_sampler.cpp: an in-process sampling profiler (link into a harness, /Zi); `PROF_FOCUS=<fn>` lists callers.
  `-DCB_RE_PROF` on pyre.cc prints per-pattern regex time.
- A cached hash in `py::str` was tried and dropped: it grew every str by 16 bytes and deep recursive ports (csharp back end)
  overflowed the stack.

Harness notes: `recheck.sh` is broken for harnesses whose expected.py re-executes itself for PYTHONHASHSEED (on Windows
`os.execv` returns at once and memcap kills the child): set PYTHONHASHSEED=1 in the environment first. Bare `test.exe` in a
run.cmd resolves to the MSYS `test` where NoDefaultCurrentDirectoryInExePath is set: use `.	est.exe`. p5's run.cmd passed
`"%~dp0"` (a trailing backslash escapes the quote): use `"%~dp0."`.

## Per port

| Module | Python | C++ | Ratio | Note |
|---|---|---|---|---|
| decompile.describe | 340 s | 70 s | 4.9x | |
| emulate.native_shadow | 4.55 s | 1.3 s | 3.5x | |
| emulate.shadow | 9.9 s | 1.6 s | 6.2x | Python side spawns the p-code lifter |
| emulate.windows16 | 5.1 s | 2.5 s | 2.0x | |
| emulate.emulator | 3.95 s | 3.75 s | 1.05x | both use the native recorder |
| emulate.pcode | 3.2 s | 2.6 s | 1.2x | |
| verify.gate16.contract16 | 60 s | 25 s | 2.4x | |
| analyze.image (+4 analyses) | 4.3 s | 2.0 s | 2.2x | |
| symbols api_arguments/ordinals/runtime_library/names/classes_writer | 21 s | 6 s | 3.5x | |
| build.msc6 | 83 s | 39 s | 2.1x | |
| build.asm16 | 8 s | 23.7 s | **0.34x** | not profiled yet |
| verify.lanes.win16 | 23.3 s | 12 s | 1.9x | |
| verify.gate16.isolated16 | 8.9 s | 4.5 s | 2.0x | |
| verify.reconstruct | 59.8 s | ~20 s | 3.0x | |
| decompile.c_text / decompiler / build.functions | ~3.6 s | ~1.6 s | ~2.2x | per-step table in the G4 report |
| java.transpile / probe / lanes.java | 1020 s | 202 s | 5.0x | Alpha convert 66–78 s vs 20 s |
| translate.backends.java (trio) | 3.2–3.8 s | 1.1 s | ~3.2x | program() only |
| translate.backends.c_java | 45.2 s | 35.1 s | 1.3x | |
| translate.backends.csharp | 37.4 s | 45.7 s | **0.82x** | 18 s of it is JSON tree loading (since sped up); back end alone faster |
| translate.backends.cpp03 / desugar | ~25 s | 22–25 s | ~1.05x | |
| translate.native_l2 / c / cpp17 | 61.7 s | 54.2 s | 1.14x | program() runs inside the native library on both |
| translate.frontends.python (program/infer/methods) | 97 s | 69 s | 1.4x | |
| translate.frontends.python.lower | — | — | — | lowering 0.09 s; rest is infer |
| translate.dotnet.cs2cpp | 2.27 s | 0.55 s | 4.1x | |
| translate.dotnet.judge | 83.4 s | 69 s | 1.2x | javac/JVM/g++ dominate |
| exact.library | 14.1 s | 8.5 s | 1.7x | |
| exact.data | 34 s | 22 s | 1.5x | 15 s in modules.combine |
| exact.toolchain | 9.0 s | 10.8–15.5 s | ~0.75x | real compiles under the same C++ executive on both sides; noisy |
| verify.authored.search16 | 0.71 s | 0.44 s | 1.6x | |
| recompile.powerpc | 90 s | 33 s | 2.7x | |
| recompile.config | 30–49 s | 2.1 s | ~19x | |
| recompile.switches | 11.9 s | 1.7 s | 7.0x | |
| analyze.ppc_calls | 130 s | 43 s | 3.0x | |
| analyze.dos_functions | 128 s | 16 s | 8.0x | |
| decompile.vmx128 | 29–34 s | 5–6 s | ~5.7x | |
| exact.modules | 0.6 s | 0.5 s | 1.2x | |
| exact.standin | 2.7 s | 1.9 s | 1.4x | |
| port.asm16 | 10.7 s | 7.2 s | 1.5x | |
| port.pre_ansi (part 3) | 60 s | 41 s | 1.5x | |
| port.pre_ansi (part 5) | 23 s | 215 s | **0.11x** | branch predates the py::str/regex speed-ups; re-time on staging |
| source.vxd.disassembly | 504 s | 84 s | 6.0x | |
| source.vxd.le_writer | 491 s | 61 s | 8.0x | |
| source.vxd.coalesce | 14 s | 16 s | 0.88x | |
| formats.le | 47.8 s | 33.9 s | 1.4x | |
| symbols.driver_services | 6.0 s | 5.85 s | 1.0x | |
| base.converted | 2.1 s | 3.2 s | **0.66x** | makedirs/sha1 (since fixed in the runtime); re-time |
| jobs.pool | 262 s | 110 s | 2.4x | Python dominated by process start |
| build.native_dos | 9.5 s | 6.3 s | 1.5x | |
| build.masm / build.dos_tools | 35 s | 35 s | 1.0x | DOSBox boot dominates |
| build.driver | 50 s | 45 s | 1.1x | wasm/wlink dominate |
| verify.compare | 1–3.6 s | 1 s | ~2x | |
| exact.emu.dos | 10.8 s | 10.9 s | 1.0x | same C++ executive underneath; vs Python executive 34.8 s = 3.2x |
| formats.xbox.crypto.xsm3 | 2.0 s | 0.26 s | 7.7x | |
| formats.xbox.crypto signers (5) | 4–33 s | 20–32 s | **~0.2x** | RSA powmod (since fixed: 1024-bit powmod 0.97 ms vs CPython 4.07 ms); re-time |
| formats.xbox.crypto.shadowboot | 28 s | 457 s | **0.06x** | same RSA cause; re-time |
| verify.lanes.genesis | 43 s | 15.3 s | 2.8x | |
| verify.lanes.nes / sms / gb / z80_flat | 13–24 s | 6.6–17 s | ~1.6x | |
| exact.plm86 | 43.8 s | 18 s | 2.4x | |
| platforms.dos | 3.7 s | 1.6 s | 2.3x | |
| symbols.debug_build_estimate | 38.9 s | 50–58 s | **0.7x** | objdump per segment in ne_asm.segment / debug_build |
| build.period16 | 49.6 s | 47.1 s | 1.05x | DOSBox-X dominates |
| emulate.driver | 5.1 s | 4.6 s | 1.1x | |
| knowledge.show | 23 s | 36 s | **0.64x** | not found yet |
| source.ne_asm.datacode | 13 s | 23 s | **0.57x** | ne_asm.segment Seg slow on big segments |
| source.ne_asm.segment | 195 s | ~3300 s → ~375 s | slower | 55 min before the regex fix; re-time on staging |
| symbols.cpp_classes | 31 s | 6.6 s | 4.7x | |
| build.definitions | 5.3 s | 3.8 s | 1.4x | |
| symbols.prototypes | 2.8 s | 8.1 s | **0.35x** | JSON + regex (both since sped up); re-time |
| source.readable | 7.2 s | 9.0 s | **0.8x** | multiline regex (since fixed: 450 ms -> 15-20 ms); re-time |

## Runtime micro-benchmarks (C++ runtime vs CPython)

| Operation | Before | After | CPython |
|---|---|---|---|
| finditer `^#define` (re.M) over 14.6 MB | 454 ms | 21 ms | 283 ms |
| 500 searches `^#define` (re.M) | 33 s | 0.3 ms | 3.1 ms |
| dict: 50k str keys set+get ×20 | 3.09 s | 1.28 s | 1.56 s |
| JSON parse 8 MB | — | 129–151 ms | 143–178 ms |
| JSON dump 8 MB | — | 124–142 ms | 114–128 ms |
| sha1 2 MB | 19 ms | 1.0 ms | 1.0 ms |
| sha512 2 MB | — | 7.7 ms | 3.5 ms |
| RSA 1024-bit powmod | ~295 ms | 0.97 ms | 4.07 ms |
| makedirs (new leaf) | 4.2 ms | 0.80 ms | 0.92 ms |
| split("\n") into 310k str | — | 190–450 ms | 58 ms (UTF-32 str storage) |

## Still slower and why (to fix)

- `port.pre_ansi`, `source.ne_asm.segment`, `datacode`, `debug_build_estimate`: big-text regex/str work and per-segment `objdump` processes; re-time on staging first.
- Crypto signers/shadowboot: RSA; fixed in MemCrypto on staging, re-time.
- `build.asm16`, `knowledge.show`: not profiled yet.
- Runtime: `py::str` stores UTF-32 (4x memory, slow huge splits); sha512/md5 portable code vs OpenSSL asm; JSON dump ~10% slower.

The whole-codebase comparison (the full `tests/run_all.py` under Python vs `codebreaker.exe`) comes once ports are wired into the exe.
## core16 integration (2026-11-11): the 33-module cycle linked for real, end to end

All 33 modules (decompiler, emulators, verify lanes, module builder) in one program, no stubs, no /FORCE:UNRESOLVED
(`state/work/typed-port/core16.integration/`). Each case is run on the Python and on the C++ with the same inputs and the
outputs diffed IDENTICAL. Real OpenWatcom tools, real Unicorn; builds are cold (`CB_NO_CACHE=1`: no shared build cache; the
decompiler's own cache is warm on both sides). Wall seconds, one run each, machine shared with other work (single numbers
are +-30%).

| Case | Python | C++ | Ratio |
|---|---|---|---|
| decompile (kind.prepare), 6 modules (lzexpand, ver, keyboard, comm, lz32, version) | 0.5-1.4 | 0.6-5.6 | ~0.5x (warm decompiler cache; process start and DLL loads dominate) |
| judge lzexpand.dll (win16 lane, 47 items) | 28.7 | 16.0 | 1.8x |
| judge ver.dll | 16.4 | 6.9 | 2.4x |
| judge lz32.dll / version.dll (win32 lane) | 7.3 / 16.2 | 7.3 / 16.2 | 1.0x |
| build lzexpand.dll (direct path, gate included) | 13.2 | 6.9 | 1.9x |
| build ver.dll | 15.9 | 12.2 | 1.3x |
| build comm.drv | 11.2 | 9.8 | 1.1x |
| build keyboard.drv | 18.1 | 12.7 | 1.4x |
| build lz32.dll / version.dll (_build32) | 2.9 / 3.4 | 3.2 / 2.4 | 0.9x / 1.4x |
| isolated16.check, ver.dll (21 functions) | 22.6 | 20.8 | 1.1x |
| reconstruct.run lzexpand.dll (fresh store) | 30.3 | 19.7 | 1.5x |
| reconstruct.run lz32.dll / version.dll | 9.4 / 19.8 | 7.4 / 17.5 | 1.3x / 1.1x |
| build_many (lzexpand, ver, lz32) | 31.8 | 34.1 | 0.9x (one at a time here, see below) |
| main (build ver.dll --out) | 38.0 | 22.8 | 1.7x |
| build on the engine (default path), fresh facts: lzexpand / ver / comm / keyboard / lz32 / version | 17.4 / 11.8 / 13.1 / 17.3 / 3.3 / 4.4 | 37.5 / 40.2 / 8.5 / 19.5 / 2.4 / 2.3 | slow on lzexpand and ver (see below) |
| build on the engine, facts kept (second run), ver.dll | 1.0 | 0.3 | 3.3x |

### After worker processes, the module copy and the full engine steps (same day, later)

The gate (`isolated16::check`), the judge (`reconstruct::run`) and `build_many` run in worker PROCESSES of the program itself again
(`jobs/workers`: `core16 --cb-worker gate|judge|build`; a host opts in with `workers::dispatch` + `workers::enable`), so Unicorn and the tracer keep
per-process state as in the Python. `ne.image` copies the module with its own `segments` instead of preparing it again, and the
engine now has function.notes, .l2, .source, .object and .never_set (the link is served the engine's objects). Same cases, outputs
IDENTICAL, machine busier than in the table above (the Python took 1.5-3x its earlier times in some rows).

| Case | Python | C++ | Ratio |
|---|---|---|---|
| isolated16.check, ver.dll | 23.4 | 14.6 | 1.6x |
| reconstruct.run lzexpand.dll (judge in processes) | 87.1 | 9.9 | 8.8x |
| reconstruct.run ver.dll | 34.3 | 3.7 | 9.3x |
| build_many (lzexpand, ver, lz32), one process each | 22.9 | 14.1 | 1.6x |
| build ver.dll / comm.drv / keyboard.drv / lzexpand.dll (direct path, gate in processes) | 30.6 / 21.1 / 27.0 / 21.7 | 21.4 / 13.1 / 23.0 / 12.6 | 1.4x / 1.6x / 1.2x / 1.7x |
| build on the engine, cold facts: lzexpand / ver / lz32 | 17.4 / 18.9 / 3.4 | 9.5 / 11.3 / 3.3 | 1.8x / 1.7x / 1.0x |
| (before the module copy and the served objects: lzexpand / ver) | 17.4 / 11.8 | 37.5 / 40.2 | 0.5x / 0.3x |
| main ver.dll --out | 23.7 | 18.1 | 1.3x |

Notes:
* One thing in the Python is order-dependent: its engine-path `run` judged lz32.dll's GetExpandedNameW/LZOpenFileW `exit-differs`
  when another module had been run in the same state folder first, `stack-differs` (as its direct path and the C++) when lz32.dll
  was first. Compared with lz32.dll first in a fresh state: identical.
* Engine facts kept (second run): ver.dll 0.3 s (C++) vs 1.0 s (Python).

## Idiomatic C++ refactor (native/cb), against the py:: ports

| Module | py:: port | native/cb | Speedup |
|---|---|---|---|
| knowledge/sources (all 14 kinds, 16,896 rows) | 139-145 ms | 45-48 ms | ~3x |
| verify/verdicts classify() (12.8M calls: 8 words x 4 limits texts) | 296-306 ns | 19 ns from text, 1.2 ns from enums (the judge's own use) | 15x / 250x (the live engine/verdicts.cc: 80 ns) |
| verify/teeth breaks() (60,000 C texts of a few lines) | 1.17 s | 0.46 s (including the harness's JSON in and out) | 2.5x |
| verify/guided (74 synthetic programs x100, behind the old-record shim: every call converts the records both ways) | 0.66 s | 0.32 s | 2.1x |
### cb refactor W0-B16 (java: apimap, reflection_factory, platforms, ledger, ir, debug, bugfix_ledger, typemap, stubs, overlays, cmake, textutil, _paths -> native/cb/java)

The new code is compared with the Python module on the same inputs (the old typed ports run through the py:: layer and were not timed
separately; they were slower than the Python on string work). Release build, one process, repeated runs on a loaded machine (ms):

| work (real or synthetic input) | Python | cb C++ |
|---|---|---|
| apimap: load the 12 shipped api_maps/*.json, 20 loads | 101 | 84-150 |
| apimap: extract_symbols over 3.5 MB of Java-like text | 154 | 7-12 |
| bugfix_ledger: collect over codebreaker/java (134 markers), 3 runs | 84 | 49-69 |
| ledger: add 200k events / file_risk_table / to_json | 159 / 92 / 218 | 40 / 45 / 75 |
| reflection_factory.process over an 800 KB .cpp with 400 registrations | 21 | 10-12 |

Notes:
* apimap load is the one at parity, not faster: 100 KB of JSON is 2.5 ms of the 4-7 ms per load (the ordered JSON the file order needs).
* ledger.to_json first went through a JSON document (947 ms for 38 MB); it writes the fixed layout directly now (75 ms).
### cb refactor, wave 0 batch W0-B6 (decompile: m6502, xenos, struct_text, jar, ctree, inline_asm, z80), 2026-11-11

The old per-module harness run end to end (process start included), the typed port (old) against the refactored module reached
through its shim (new), same cases. Wall time in ms, mean of four runs; the cases are the harness's, so the work per run is the
old port's own cases (m6502 9,342, xenos 6,699, struct_text 261, ctree 3,822 outputs, inline_asm 3,810, z80 25,854 cases of which the
new driver runs the ones that still exist as API: decode, text, assemble, value).

| Module | old | new | Ratio |
|---|---|---|---|
| decompile/m6502 | 152 | 118 | 1.3x |
| decompile/xenos | 88 | 64 | 1.4x |
| decompile/struct_text | 42 | 32 | 1.3x |
| decompile/ctree | 44 | 31 | 1.4x |
| decompile/inline_asm | 122 | 64 | 1.9x |
| decompile/z80 | 366 | 129 | 2.8x |
| decompile/jar | (process-bound: a fake `java` is started per case) | same | 1.0x |

Notes:
* The "new" side still pays the shim's conversions to `py::str`/`py::list` (the old callers' types); against the module's own
  API the gap is wider. z80 `decode` no longer builds a `py::dict` row per instruction.

## Refactor to idiomatic C++, wave 0 batch 4 (base + build), 2026-11-11

Old = the typed port on the py:: layer (built from the pre-refactor sources, /O2), new = `native/cb` (/O2, static CRT), same inputs, one thread, best of three.

| Work | old (ms) | new (ms) |
|---|---|---|
| pklite `read` of EXTRACT.EXE's load module (46 KB, 86 KB expanded, 33,011 tokens) | 1.39 | 0.73 |
| pklite `parse` of the 86 KB image (PKLITE's choices) | 68.6 | 40.8 |
| pklite `pack` (parse + encode + the safe-memory pass) | 66.7 | 41.2 |
| ctext `blank` x5 of a 207 KB C text (first sight, no cache hit) | 7.09 | 1.41 |
| ctext `blank(whole)` x5 of the same | 6.33 | 1.14 |
| corpus `index` of the Win95 corpus (2,562 files, 2,534 names; walk, lowercase, rank, sort) | 10.2 - 11.8 | 4.6 - 5.5 |
| corpus `retail("kernel32")` after the index | (cached) | 0.17 (cached) |

Notes: the old `ctext.blank` kept an LRU of 16,384 results; the new one has none (a scan costs about what hashing the text did).
The old Python `pklite.parse` takes 7.8 s on the same image. `functions`, `runs`, `parallel`, `tidy_state` are process / file-system
bound (a compile is a process spawn; both ports use CreateProcess): not timed apart from the real OpenWatcom 16-bit build below.
A 16-bit build through the real OpenWatcom (3 functions, one rejected, cache warm in a second folder): the objects come out of the
cache rewritten for the new folder (`omf_here`) and the linked DLL is byte-identical to the first.
## cb/base/cache and cb/analyze (refactor, 2026-11-11)

Idiomatic C++ in `native/cb/` against what it replaces, same inputs, best of 3, on a busy shared machine (single numbers
are +-30%).

| What | Old | New | Ratio |
|---|---|---|---|
| analyze engine (cbanalyze.dll through JSON) vs `cb/analyze` called directly: comctl32.dll, 1133 functions (discover + call graph + strings + resources) | 0.40 s | 0.16 s | 2.5x faster |
| the same, user.exe (NE), 2319 functions | 0.67 s | 0.30 s | 2.2x faster |
| mfc30.dll, 3481 functions | 0.42 s | 0.29 s | 1.5x faster |
| kernel32.dll, 1867 functions | 0.41 s | 0.19-0.25 s | 1.7-2.1x faster |
| shell32.dll, 2398 functions | 0.74 s | 0.42-0.64 s | 1.2-1.8x faster |
| of which the strings + resources passes alone, kernel32 / shell32 | 0.024 / 0.034 s | 0.018-0.025 / 0.030-0.043 s | about the same |
| cache: put 5000 x 200 B | 0.30 s | 0.40 s | 0.75x (~20 us a put more: the second table) |
| cache: get 15000 x 200 B | 0.10 s | 0.10 s | same |
| cache: put 300 x 100 KB / get 900 x 100 KB | 0.24 / 0.07 s | 0.22 / 0.07 s | same |

Why the analyses are faster: the engine used to answer through JSON (every decoded instruction of every function written
as text and read back) and now returns typed structs. Why the small cache put is slower: the time of use is a table of its
own (so renewing it never rewrites a result: that fixed a 100 KB-per-touch WAL write), and a trigger writes it, one more
b-tree insert per put. It is immaterial next to the work the cache fronts (a compile takes 100 ms or more). Speed-ups that
were needed to get here: prepared statements kept per connection, text and blobs bound without a copy, the payload taken
out of the row without a second copy, a hardware CRC-32C for the entry checksum (with the extra copy, the checksum of the core's
bytewise CRC-32 made a 100 KB get 3x slower than the old port's: 0.25 s -> 0.07 s per 900 gets).
## cb refactor: exact batch W0-B7 (2026-11-11)

Old = the typed port on the py:: layer (pre-refactor commit, C++17 /O1, built as its harness build.cmd did); new = the
refactored module reached through its compat shim from the same harness (so the figure includes the shim's conversions),
C++20 /O1. Best of three runs of the harness's whole case file, process start included.

| module (harness work) | old ms | new ms |
|---|---|---|
| exact/draft/text (54,961 cases: tests, blocks, live registers, declarations) | 2047 | 1469 |
| exact/compare (4,000 functions against an image, Capstone decode of each) | 512 | 510 |

The other modules of the batch (aout, exepack, hooks_lattice, x86, linkemu, batch, readable, draft/package) have
harnesses of a few hundred cases that run in the process start; they were checked for the same behaviour, not timed.
## Executable-format readers (cb/formats: pe, ne, le, dos, omf, detect), 2026-11-11

Old = the native C++ readers (native/formats) behind the typed shells; new = native/cb/formats. Both run over the same inputs by a
differential harness (state/work/cpp-refactor/formats/diff.cpp: each side dumps every fact its readers give for a file to text; the
dumps are compared), on the 1,248 files of the Win95 corpus (586 PE, 373 NE, 218 LE among them) and the OMF objects/libraries of the
period toolchains and the old harness (1,738), with seeded damage (truncations, bit flips, damaged headers) as extra inputs
(8,736 .. 38,688 per lane). Times are the whole dump per lane (parse + every reader + text), best of three on a busy machine:

| Lane (inputs) | old | new |
|---|---|---|
| PE (1,248) | 0.29 s | 0.30 s |
| NE (1,248) | 0.22 s | 0.19 s |
| LE (1,248) | 0.63 s | 0.68 s (equal within noise; 0.39 / 0.39 on a quiet run) |
| DOS (1,248, packed programs unpacked statically) | 0.08 s | 0.08 s |
| DOS with damage (26,208; most run the emulated unpacker to its limit) | 30.8 s | 35.5 s before caching the Unicorn table in the hook, equal after |
| OMF (1,738) | 3.08 s | 3.17 s |

Differences found by the harness and kept on purpose are listed in the `DIVERGES` comment at the top of each `*_test.cpp` and in
state/work/typed-port/_bugs/LIST.md: strict reads of a field cut by the end of the file (OMF 51 of 12,094 inputs, all truncations),
index 0 as "no target" instead of "the last entry" (OMF 178 fix-up lines, all in the hand-made synthetic objects), a trailing odd byte
of a UTF-16 resource name dropped instead of replaced (2 of 26,208 damaged PEs). Everything else matched (0 mismatches in 8,736 PE,
NE, LE and DOS inputs each, 38,688 LE, 26,208 NE and DOS).

## W0-B25: recompile (x360project, disassemble, builders/floating) and source (assembly, placement, ne_asm/segdiff, ne_asm/run)

These are I/O- and tool-bound (they start XenonRecomp, OpenWatcom, the corpus walk) except `disassemble`, the only one on a hot path
(the recompiler lists every word of a function). New code, Release, one thread, 4,096 words of Minecraft Xbox 360 code (0x822B6E00):
the listing with the VMX128 callback, Capstone for the rest, plus the flow classification from two entries and the data runs, the three
decodes of the two `Disassembler`s the test builds included: the whole `recompile.disassemble` suite (7 tests, 78 checks) runs in 47 ms
wall including process start. The old port decoded each word with one `cs_disasm` call per word; the new one does the same through
`platforms::Decoder::each` (also one `cs_disasm` per word), so the per-word cost is Capstone's in both and the new code adds no work. The old
exe could not be run here (no Python-engine build in this worktree), so no side-by-side number is claimed.
`floating::emit` is a table lookup over 32 rules and one string fill per instruction (the old one was an `if` chain of 32 string
compares and `py::str` formatting). Nothing else in the batch has a loop worth timing.
