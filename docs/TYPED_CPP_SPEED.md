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
  call), `\b` ASCII path, a greedy single-character loop followed by a plain ASCII character skips the backtrack positions
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

## cb refactor, batch W0-B1 (compare, emulate): before and after, 2026-11-11

The py-layer port (before) against the idiomatic native/cb code (after), same machine, release build, best of N.

| What | Before | After |
|---|---|---|
| compare::image::first_difference, 4 MB equal until the last bytes | 2.35 ms | 0.21 ms (memcmp, eight bytes at a time) |
| compare::mask::same, 4 MB, every 64th byte masked | 3.77 ms | 0.93 ms |
| compare::mask::distance, 4 MB, one mask | 4.89 ms | 0.96 ms |
| the 32-bit recorder (cbtrace.dll, C interface; 900,002 instructions of a counting loop, one hook per instruction) | 26.4 / 27.4 / 27.5 ns per instruction | 27.0 / 27.4 / 28.7 ns per instruction |

Notes:
* The recorder's code is the same code moved (native/tracer -> native/cb/emulate/tracer); only the Unicorn and Capstone
  tables changed (`ucl()` is one acquire load). A first version took a mutex per call and was 6% slower; the atomic fix
  brought it to noise. Runs on this machine vary by a factor of two with other load, so the pairs above were run back to back.
* pcode's cost is the pcodecheck pipe (unchanged); ports16 and symbolic construct a few objects per run (not measured: far
  from any hot loop of the judge).
* Harness: state/work/cpp-refactor/hot-b1/ (the recorder), state/work/cpp-refactor/compare/ (mask, image).

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

### native/cb/build (W0-B5): the old harness cases replayed

Same inputs (the harness `cases.txt` of each module, parsing and hex decoding included), old typed port `test.exe` against the new module.

| Module | py:: port | native/cb | Speedup |
|---|---|---|---|
| build/mzfile (158 cases) | 260 ms | 53 ms | 4.9x |
| build/exepack (5,385 cases) | 297 ms | 127 ms | 2.3x |
| build/stubjoin (979 cases, several with multi-GB stub places) | 7,859 ms | 4,462 ms | 1.8x |
| build/vxdfile (329 cases) | 120 ms | 17 ms | 7x |

The assemblers, definitions, watcom, msvc152, period16 and dos_sessions have no CPU-bound work of their own (they drive other programs); not timed.
## cb refactor: recompile batch W0-B24 (2026-11-11)

Real inputs: the Xbox 360 images of Minecraft in C:/Nexia360/Games (Xbox 360 Edition 20 MB, Indev, the test build, the two
pre-release builds). Old = the original Python module (the typed port runs the same algorithm; where its harness could be built
it is timed too); new = `native/cb/recompile`, Release /O2, best of 5 runs after one warm-up, on a busy shared machine. The
image is already read in every row (reading it is formats/powerpc_image).

| What | Old | New | Ratio |
|---|---|---|---|
| switches.find, Minecraft default.xex (213 tables, 4 unresolved, 12 pointer calls) | 0.69 s (Python) | 0.003 s | ~230x |
| the same, Indev / test build / pre-release 1 / pre-release 2 | 0.39 / 0.21 / 0.55 / 0.59 s | 0.001 / 0.001 / 0.002 / 0.002 s | 200-400x |
| jumptable over every bctr of Minecraft's code (1232 bctrs, 214 tables) | 6.4 s (Python) | 0.0026 s | ~2400x |
| the same, Indev (no runaway table; 422 bctrs, 153 tables) | 0.41 s (Python) | 0.0015 s | ~270x |
| jumptable, the old harness's 1,158 detect cases (fuzzed images), typed port vs new through its shim | 0.54 s | 0.41-0.46 s | 1.2-1.3x |
| xenon.build() when up to date (the check of 4 source folders' times) | 4.5 ms (Python) | 1.7-2.8 ms | 2x |
| xenon.msvc_env() (the real cmd.exe running vcvars64.bat) | 2.1 s | 2.1-2.7 s | same (it is cmd.exe) |
| xenon.instruction_tests, 5 of Xenia's tests through the real binutils, XenonRecomp and clang-cl | 14.2 s (Python) | 14.9 s | same (clang-cl compiles for 12 s) |
| gpu.gpu_files / design_md | text constants | text constants | nothing to time |

The scan modules are faster because the Python read each word of a 20 MB image through `struct.unpack_from` and each
instruction through a tuple decode; the C++ reads bytes and decodes fields. The 6.4 s of the jumptable row is mostly one
real table, an unbounded byte table at 0x824A8E4C of Minecraft, which the Python read for 9.5 million entries until the image
ended (and the pre-release builds' similar one for 6.9 million); the C++ stops at 65,536 (`kMaxTableEntries`, a DIVERGES
in jumptable_test). Everything else it finds in the five images is identical to the original (whole-image comparison,
below). The xenon module is bound by the programs it runs; its own work (the environment of vcvars64, the scan for a newer
source, the compare of the generated files) is a few milliseconds.

Equivalence on the real images (not timed): `switches.find` on the five images gives the same tables (213/150/91/148/148),
labels, guards, defaults, functions, and the same unresolved / outside / pointer-call lists, byte for byte as text; the
recompiler's jumptable gives the same index registers and targets for every table of the five images except the cut at 65,536
in the three runaway tables named above. The old typed harness's cases (1,158 detect cases) agree except the same 16 tables.
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

## W0-B23: port/arith16 and port/verify (native/cb/port)

Both are bound by the programs they start (clang for arith16; the emulator and the port for verify), so the interesting numbers are
what they do with what those programs give. Release, one thread, this machine.

| piece | old (Python original) | new (cb::port) | note |
|---|---|---|---|
| arith16: read clang's tree of a.c + hdr.h (662 KB of JSON, 671 nodes of the file, ~12,000 of the headers') | json.loads + mark_files 4.7 ms | streaming parser into typed nodes 2.5 ms | the Python builds a document of dicts then walks it again; the C++ never makes the document |
| arith16: work out the casts from the tree (58 casts) | edits_of 2.4 ms | edits_of 0.03 ms | the Python walks the whole dict tree, the C++ the typed nodes (each expression's type is a field) |
| arith16 per file, end to end | clang 0.1-0.4 s + 7 ms | clang 0.1-0.4 s + 2.6 ms | clang is the cost, as before; a result kept in the cache costs a hash of the file and its headers |
| verify: console diff, 2,000 / 20,000 / 20,000 lines with 20 / 200 / 2,000 changed | difflib 2.3 / 115 / 166 ms (autojunk gave up aligning the repeated lines: 138 / 34,173 / 35,642 lines out) | Myers 0.8 / 9.5 / 138 ms (138 / 1,366 / 10,924 lines out) | the old diff of 200 changes was 34,173 lines wide (it replaced nearly everything); the new one is the changes and their context |
| verify: the cases of cfg_all (17 cases, the real DOS emulator, no cache) | not run (needs the Python emulator host) | 1.1 s | 15 of the 17 cases start a native port and a DOS program; the compare itself is microseconds |

Equivalence on the old harnesses: verify's cfg_all / cfg_vol / cfg_empty / cfg_latin through the real emulator (cbdos.dll) give the
same table rows and the same console differences as the Python's expected report, byte for byte (the path is written with `/`, the
trailing blank line is gone, and `unknown-program` is an error). arith16: the C++ and the fixed Python give the same text on 80 random
files of 40 statements (clang's own trees), and on the harness's a.c/b.c/bad.c/empty.c/nohdr.c/hdr.h.

## exact/emu and exact/habits (wave 0, batch B8)

Whole old harness run (process start, parsing the cases, the module) against the new module's golden test over the same
cases (process start, parsing, the module), milliseconds, release build:

| work | old port | cb |
|---|---|---|
| exact/habits/oldcomp (1,000 pairs) | 44 | 36 |
| exact/habits/boolx (2,337 cases) | 255 | 29 |
| exact/habits/symx (10,625 cases) | 551 | 45 |
| exact/emu/isis, 20,000,000 8080 instructions of a loop | 517 | 281 |

exact/emu/isis and exact/emu/xenix golden tests copy a folder per case, which dominates their time (isis 91 runs 2.4 s,
xenix 67 runs 6.0 s; the old harness's 317 and 91 runs took 1.6 s and 4.7 s with fewer copies); xenix's work is Unicorn's
either way and was not timed apart.

## cb/java/console (W0-B17), 2026-11-11

The old side is the original Python (the live implementation of these features; the typed ports ran over the same
algorithms). Real inputs, the new side a driver over `cb_java` (Release, C++20 /O2); best of three, process start excluded.
Outputs were compared first: all of them identical to the Python's.

| work | Python | native/cb | note |
|---|---|---|---|
| xdk_scan.detect, the installed XDK (2,514 entry points in its headers) | 0.125-0.162 s | 0.055 s | 2.3-3x; same 2,514 names and header for each |
| assets.run, Minecraft's `minecraft.jar` (346 entries, 42 assets, 3 PNGs converted) | 0.054 s | 0.050-0.059 s | same: 42 files identical, every PNG pixel for pixel |
| of which: reading the jar / converting the 3 PNGs / writing the 42 files | - | 0.7 ms / 0.5 ms / 65-115 ms | the writes are the file system; the Python does the same ones |
| vcxproj.run, a generated 5,202-file project (the real xbox360 profile's 55 rules) | 4.2-4.7 s | 2.3-3.6 s | 1.3-1.8x; .vcxproj and .filters byte-identical to the Python's |

The vcxproj figure is dominated by classifying every class against the profile's rules through the generated project's very
deep inheritance chains (up to 208 ancestors, each tried against 55 patterns); a converted game's chains are 3-6 deep, where the
whole run is a few milliseconds. Not memoised: the classification of a class and its ancestors is repeated per file.
## Java lane text passes (cb/java: emit/std_transforms, projects/minecraft/{raycast, content, keybind_mark, ui_mark}), 2026-11-11

Old = the typed ports over the py:: layer, run by the old harnesses (state/work/typed-port/<module>/test.exe over its cases.txt,
whole process); new = the same inputs through native/cb/java in one process, best of 20 (state/work/cpp-refactor/w0-b18/bench.cpp).
The old process start alone is 10 ms, so the old work is the total minus that:

| Work | old (total - start) | new |
|---|---|---|
| raycast.convert_gl_pick_to_raycast, 261 recorded cases | 54 - 11 = 43 ms | 3.6 ms |
| std_transforms (apply, sanitize, annotate, literal-masked sub), 544 cases | 53 - 10 = 43 ms (all four functions) | 0.36 ms for the 256 `apply` cases alone |
| content.scan_assets over the harness corpus (33 files) | not run (the old harness writes into its folders) | 3.4 ms |
| content.render_content_md (c1) | | 1.6 ms |
| keybind_mark.mark / ui_mark.mark of the harness tree (dry run) | | 1.0 ms / 1.7 ms |

No old figure for the tree passes: their harnesses are written to rebuild and copy their fixture folders for every case. They
were compared for behaviour instead: 81 keybind_mark, 85 ui_mark, 143 controller_nav and 261 raycast recorded cases, the 544
std_transforms cases, 57 content calls, the whole platform_equivalents and minecraft_xbox360 tables: no difference that is not
listed in a DIVERGES block (raycast overlap and comment rules, std_transforms empty pattern, run() on a missing folder, the
controller_nav half-patch return).
## Quad files, the MC2 inverse and the Java tool table (cb/invert, cb/java/run, format_key), 2026-11-11

`cb::invert::early_c` is a typed API over the same compiler model (native/invert, linked as cb_invert_model), so its speed is the
model's: the old harness's 808 recover/recover_cfg calls (each followed by two to_c, detect_copies, a ladder fold and two
local_decls, and the text written out) take 0.19-0.22 s in the old process; the new golden test runs 397 recoveries in about
0.06 s of its 0.10-0.14 s (the rest is the process start and loading the DLLs): about 0.15 ms a recovery against 0.25 ms,
with the model identical, so the gain is the lost output formatting. The quad readers (quad_dump, quad_align on the 10 KB FXU.Q
and the Lattice objects), early_c_frame, grammars, known_differences (seven patterns, compiled once per table) and format_key
run in microseconds on their real inputs; they have no hot path and were checked for the same behaviour, not timed. The Java
tool table parses a command line in microseconds; the tools it runs are other modules' work.
## port/pre_ansi part 4 (cb/port: c_source, old_sizes, struct_layout, union_views, frames, original_program, bss, rewrite, headers, complaints), 2026-11-11

Old = the typed port (pre_ansi_p4.cpp, 1,870 lines over the py:: layer); new = native/cb/port. Both run the old part-4 harness
(state/work/typed-port/port.pre_ansi.p4: every call of the part on the three source trees lib 2.20, link 2.44 and MS C 3.00 p0,
2,940 answers). The old exe also runs the Port constructor (parts 1-3, not yet refactored) and could not be rebuilt without it
(its objects are older than master's header shims), so the two times are not of equal scope:

| Whole harness (3 trees, 2,940 calls) | old | new |
|---|---|---|
| old test.exe (constructor of parts 1-3 + part 4) | 2.86 s | |
| new driver (part 4 only; the constructor's state is read from a dump) | | 0.20 s |

The Python original runs the same constructor in 0.5 s for the three trees, so the old constructor is a fraction of the 2.86 s; the
new part 4 is not slower. The new answers equal the Python's on all 2,940 calls (compare.py, by meaning; 0 differences).

### native/cb/port (W0-B21: pre_ansi PART 5, the widening passes and the mending)

The old typed port's harness (state/work/typed-port/port.pre_ansi.p5) runs all five parts of pre_ansi over four whole programs, so the
mending alone cannot be timed there (the port's own record: 34 s Python, 39-42 s CPU the typed port, 1.6x slower than the Python). The
same work is timed here on the cases of the golden files (native/cb/port/testdata/pre_ansi_p5): the Python replays them with
state/work/cpp-refactor/pre_ansi_p5/replay.py, the C++ is the golden test (`CB_P5_TIMING=1 cb_tests --filter port.pre_ansi_p5`).
Same inputs, results compared (the Python's own, recorded). Seconds on a busy machine, the better of two runs.

| Work | Python | native/cb/port | Speedup |
|---|---|---|---|
| lib-2.20: text passes over every file | 0.014 | 0.003 | 4.7x |
| lib-2.20: mend, 6 complaints of each kind one by one, then all together for two rounds | 0.406 | 0.219 | 1.9x |
| lib-2.20: old_c_ways, numbered_places, agree_wide, stored_wide, passed_on | 0.021 (with a deepcopy of the Port) | 0.008 | 2.6x |
| msc-3.00-p0: mend, the same | 0.639 | 0.159 | 4.0x |
| msc-3.00-p0: whole-program passes | 0.051 (with a deepcopy) | 0.016 | 3.2x |
| msc-3.00-p3 (304 files): mend, the same | 2.800 | 0.887 | 3.2x |
| msc-3.00-p3: whole-program passes | 0.198 (with a deepcopy) | 0.096 | 2.1x |
| link-2.44: mend, the same | 0.660 | 0.248 | 2.7x |
| call_at over 3 columns of every line with a call (lib-2.20) | 0.027 | <0.001 | >25x |

The new code is faster than the Python on every row; the old typed port was timed slower than the Python (1.6x), so it is faster than that too (inferred, not run side by side). What made the mending
faster was not the regular-expression engine (the engine is slower than CPython's `re` on a single search): the Python recomputed the
blanked text of every file per complaint (`any(re.search(...) for t in out.values())`) and parsed each file's functions once per
complaint (an lru_cache on the text, but the key is hashed on every lookup); the C++ keeps one reading of each file for the round.
## cb refactor: symbols and vxd batch W0-B26 (2026-11-11)

Old = the engine or Python module the new code replaces, same inputs, best of 3-5, shared machine (+-30%).

| What | Old | New |
|---|---|---|
| prototypes: read the 10,166 declarations of OpenWatcom's h/nt+h/win and Microsoft's SDK/DDK headers (engine native/symbols/prototypes.cc, process start and JSON out included) | 600 ms | 560 ms (the reader alone) |
| demangle: 5,727 real names through the old engine (parse + type name) vs the new reader (parse + type name + plain + readable + stack_bytes, i.e. three parses a name) | 73 ms | 105 ms |
| sdk_structs: read the 8,514 structures and 6,526 pointer types of the three 32-bit header folders | Python 400 ms | 200 ms |
| xdk_libraries: search dash.xex (2.0M words) and a game's default.xex (1.8M words) for 1,308 library signatures | Python 2.9 s / 1.9 s (whole `match`, image read included) | 220 / 200 ms (the search alone; a sorted pair index replaced a table of lists: 1.1 s before) |
| xdk_libraries: make the 1,308 signatures from 2,277 functions | Python 300 ms | 8 ms |
| ordinals: read all 597 .lib and .def files of the reference folder | Python 570 ms (process start included) | 91 ms |


## cb refactor W0-B22: port/pre_ansi part 2 (the data passes of the port of old C), 2026-11-11

Inputs: real source trees of `Microsoft-Assemblers-Compilers/ports` (lib-2.20, link-2.44, masm-4.00, msc-1.04-mc2, msc-3.00-p0,
msc-3.00-p3 for the whole-port runs; msc-3.00-p3, -p0, -p2 and masm-4.00 for the passes alone): the twelve passes of `Port`, each on
a fresh Port and as the chain `Port.modernize` runs, and `Port.modernize` itself. The passes moved from the typed port (`py::str`
text, `py::re`) to `native/cb/port/old_c_*.cpp` (`std::string`, `cb::Regex`, token scans); the old file is a shim over them.

| What | Python | old typed C++ | new (`cb::port`) | Ratio vs Python |
|---|---|---|---|---|
| the passes alone (no `Port.__init__`, no conversion), 4 trees: p3 / p0 / p2 / masm | 4.83 / 2.60 / 11.49 / 8.88 s = 27.8 s | not separated | 1.46 / 0.28 / 2.64 / 0.92 s = 5.3 s | ~5x faster |
| 6 trees, 24 pass runs + `modernize` each, whole harness (includes the old `Port.__init__` of parts 1, 3, 5) | ~90 s passes + 44.5 s `modernize` | 200 s (busy machine; 102 s for the passes before `modernize` was timed) | 27.7 s (the shim over the new code) | ~5x faster than the old typed port, ~5x than Python |

The harness answers: `modernize` of all six trees and every per-pass record are identical to the old typed port's files (45
directories, byte for byte) and, once the CRs the C++ harness reader keeps in CRLF files and the two names `agree_wide` (part 5)
picks by the order of a set are put aside, equal to the Python's. The passes are not slower than the port they replace on any tree.
## cb refactor: xna batch W0-B30 (2026-11-11)

Old = the typed port on the py:: layer (xna/mgfx.cpp, xna/fixups.cpp, built /O2), same inputs, best of 3, shared machine (+-30%).

| What | Old | New |
|---|---|---|
| fixups: `apply` over 20 decompiled C# files of 136 KB each (2.7 MB: ref casts, constructors, data holders; the new run also does the placements the old one got wrong, so it makes slightly different edits) | 12.9 s | 0.52 s |
| mgfx: `add_parameters` of 4 parameters (scalar, vector, matrix, texture) to a compiled effect (619 bytes), 20,000 calls | 139 ms | 43 ms |

## cb refactor: exact/draft/forms, batch W1-B11 (2026-11-11)

Old = the typed port on the py:: layer (exact/draft/forms.cpp, /O2 in the harness `state/work/typed-port/exact.draft.forms`), same inputs, one run each, shared machine (+-30%).

| What | Old | New |
|---|---|---|
| the harness: 482,355 pass runs over 8,475 draft bodies (every pass of the module, library passes against a fake kit), texts and cases read from the files, one JSON line written per case | 11.1 s | 5.6 s |

The harness answers: the Python original (fixed: three bugs, see _bugs/LIST.md) and the C++ module give the same result for all 482,355 cases, the tables the passes fill included. The dynamic-pattern work of the old port (a regex built per name per call) is code now (`\b`-bounded word scans); the 129 patterns that are left are compiled once.
| contract16: `_calls` over 250 functions (474 calls) of four Win95 16-bit modules | Python 199 ms (the old typed port could not be isolated: its harness times describe16 with it) | 42 ms (the whole test: parse the dump, decode, compare; process start 55 ms not counted) |
| setup16 make_cd: the CD is patched in place (copy + two records) instead of written again by pycdlib; screen shrink 640x480 to 160x120 is the same Pillow filter | not timed (the old port had no ISO writer) | fixture CD in a few ms; the cost is the copy of the image |
## cb refactor: symbols/libraries, x86, wine16 and translate/ir, batch W0-B27 (2026-11-11)

Old = the live Python module (the exe's translated Python runs it), best of 3, shared machine (+-30%). Results compared first: the same functions
(kind, count, and a digest of every name, byte, mask and offset) for all 391 real libraries of the Win95 DDK/SDK and the Xbox 360 XDK (under 30 MB).

| What | Old | New |
|---|---|---|
| symbols/libraries: read all 391 libraries (277 Win95 reference, 114 XDK; 16-bit OMF, 32-bit COFF and PowerPC COFF), file read included | Python 27.0 s | 1.9 s (14x) |
| ... the largest: xavatar2d.lib (5,513 functions) | Python 2.32 s | 0.050 s (46x) |
| ... libcMT.lib (10 MB, 2,139 PowerPC functions) | Python 0.107 s | 0.016 s (6.7x) |
| ... KERNEL32.LIB (the SDK's import library, 631 thunks) | Python 0.013 s | 0.0027 s (4.8x) |
| libraries::Index.find (the pair index of xdk_libraries, now shared) | the same code, moved | no change (xdk match 200-220 ms, see batch W0-B26) |
| wine16, x86 | not timed: a few hundred lines of text; a decoder wrapper that does no work of its own | |
| translate/ir: build 1M nodes (500k `a + n`), then walk them and read each one's kind | old py:: port 0.051 s build, 0.249 s walk | build 0.087 s, walk 0.041 s |

translate/ir: building a node is 1.7x slower than the old port (87 ns against 46: the arena registers every node's destructor, and a node is
136 bytes; the first version, with every node as big as a ClassDecl, was 424 bytes and took 0.19 s, so the big declarations are now boxed), walking
is 6x faster (no shared_ptr counts, no `py::list` of `py::ref`, the kind by a visit rather than a virtual `class_name()` that builds a `py::str`),
and a build with a walk is 2.3x faster. A tree is built once and walked by every pass.

## cb refactor W1-B1: emulate/powerpc, z80, m6502, native_driver, 2026-11-11

Inputs: a 3000-iteration PowerPC loop (store, load, add, `bdnz`: 15,000 instructions a run, 200 runs), a Z80 `DEC BC` loop (32,770
steps a run), a 6502 `INX`/`BNE` loop (200,000 steps a run); one thread, Release (`state/work/cpp-refactor/w1b1/bench.cpp`, `bench.py`).
The old typed ports ran on `py::` containers and per-step dictionaries and cannot be built apart from the Python runtime here, so the
comparison is with the Python they were ported from (the live implementation until the exe retires it).

| What | Python | new (`cb::emulate`) | Ratio |
|---|---|---|---|
| PowerPC interpreter | 0.20 Msteps/s | 29.5 Msteps/s | ~145x |
| Z80 interpreter | 0.43 Msteps/s | 44.4 Msteps/s | ~100x |
| 6502 interpreter | 0.70 Msteps/s | 87.9 Msteps/s | ~125x |

Correctness against the Python: 1,825 6502 single steps (every opcode, decimal mode, memory CRC) are identical; 53,419 of 53,424 PowerPC
single steps (every primary and extended opcode, random words, random machine states, memory CRC of every written page) are identical, and
the other five are signalling-NaN cases the old harness recorded unstably (the current Python gives what the C++ gives).

## cb::decompile m68k, xenos_hlsl, vmx128 (W1-B6)

Old harnesses replayed (state/work/typed-port/decompile.m68k and decompile.xenos_hlsl, the old port's `test.exe`) against the same recorded cases replayed through `cb_tests` (Release; CB_M68K_CORPUS / CB_XENOS_CORPUS, which also parse the case text and the Python's JSON answers).

| What | old typed C++ | new (`cb::decompile`) |
|---|---|---|
| m68k: the whole old harness (133,147 decodes with text, 65,536 text-and-assemble round trips, 6,600 assembler lines, the private helpers' cases, the opcode-shape table) | 5.0 s | 1.6 s for the decodes, round trips and assembler lines (the helpers' cases are gone with the helpers) |
| xenos_hlsl: 7,872 shaders translated (plus 442 containers and the small functions in the old run) | 1.5 s | 1.2 s (with the JSON of every answer parsed) |
| vmx128: decode and the SLEIGH generation | not timed separately (the old harness was dominated by reading four game images) | the 77 rows plus 200 free-bit words decode and the 22 KB SLEIGH is written inside the test's 10 ms |

Nothing is slower than the port it replaces.

## cb::build::flat_thunk and cb::base::converted (wave 1, W1-B3, 2026-11-11)

Measured on this machine (an SSD with Defender on), the same real inputs through the live Python and through `native/cb`. The real work of both is
small; what is measured is mostly file creation, and that is where the C++ was first slower (`std::filesystem::create_directories` costs 2.6 ms for a
folder that is already there and 3 ms for a new one in the MSVC library, and every `cb::fs::write_file` asked it first: `cb::fs::make_dirs` now looks
first and creates with `CreateDirectoryW`).

| What | Python | new (`cb::build::flat_thunk` / `cb::base::converted`) |
|---|---|---|
| `recover` of the 35 flat-thunk modules of the Win95 corpus with the real 8,292-prototype SDK table (4 files each; stub reading, SMapLS scan, text) | 0.25-0.45 s | 0.10-0.12 s (0.025 s of it is not file creation) |
| `Store.project` + two `record` + the second `project` (found again) for the same 35 modules, fresh Converted folder | 0.97 s | 0.57 s |

The old typed port of flat_thunk was not timed (the typed ports are being replaced). The 35 modules x 5 files `recover` makes are byte-identical to the
Python's (175 files); the OpenWatcom builds of recovered sources of lz32, dciman32 and version are byte-identical DLLs (PE timestamp aside), and the period
build (thunk.exe, ML, CL, LINK) of the curated lz32 source differs only in the export and resource timestamps (8 bytes).
## cb refactor: build/msc6 and build/asm16, batch W1-B4 (2026-11-11)

Old = the live Python module (the exe's translated Python runs it) and, where it could be run, the old typed port on the py:: layer (its harness
binaries of state/work/typed-port, built /O2). Same inputs, shared machine (+-30%). Results compared first, on meaning: asm16 over the 699
functions of the Win95 16-bit modules (every attribute, the source in six forms, check / check_piece / differences): 699 of 699 equal; msc6 over 199 decompiled
texts of four modules and 150 mutated ones through every text pass, 400 fuzzed object files, Wine typing and the SDK passes: all equal but the 13
fuzzed (damaged) objects (see DIVERGES in msc6_test.cpp); compile_many against the REAL compilers (Microsoft C 6.0a under DOSBox-X, Visual C++ 1.52c on Windows):
single, named, batched, together, partial, a refused file among good ones, the cache read back: all equal.

| What | Old | New |
|---|---|---|
| asm16: 699 functions made, then each one's mnemonics, candidates, raw calls, the source in six forms, three checks (no file output) | Python 5.5 s | 1.6-2.0 s (3x), the module files read included |
| ... the same with the harness's 4,400 output files and JSON lines | old typed port 37.3 s | 30.1 s (the files dominate both: cb::fs::write_file costs about 6 ms a small file on this machine) |
| msc6 text passes over 345 texts (pascal_order, fold_temporaries, join_far_parameters, split_frame, ds_globals, sdk_calls) | Python 0.43 s | 0.085 s (5x): fold_temporaries 0.286 -> 0.040, pascal_order 0.049 -> 0.017, sdk_calls 0.046 -> 0.010 |
| msc6: sdk_prototypes of the 2,000-line WINDOWS.H | Python 10 ms | 3 ms |
| msc6: the whole harness (400 fuzzed objects, every pass, Wine typing of four headers, 150 mutated texts), no file output | | 0.5-0.7 s |
| msc6 / asm16 compile_many | the compiler dominates (DOSBox-X starts in about 2 s; a native Visual C++ 1.52c compile 0.1 s a file): equal within noise | |

## cb refactor: build batch W1-B5 (2026-11-11)

Old = the Python that is still the live implementation (build/data.py, build/driver.py, over formats.dll), same inputs, shared machine
(+-30%); the typed ports on the py:: layer were not built in this worktree. Not timed: build/m68k_asm and build/native_dos (their cost is the
instruction encoder and the DOS executive behind them, which are other modules' work; the new directive layer is a single linear pass over a
source's lines).

| What | Old | New |
|---|---|---|
| `data32` over 60 Win95 PE modules (relocated words, pointers, the C text of every data section; file read and image parsed in the timing) + `data16` and `far_segments` over 60 NE modules | 1.41 s (Python) | 0.50 s (outputs identical for all 120, byte for byte) |
| driver `compare` of two small VxDs (splitter, srammtd, bios; 9 pairs x 20) | 3.57 ms per pair | 0.29 ms per pair |

## cb refactor: decompile batch W1-B7 (2026-11-11)

Old = the typed port on the py:: layer (`decompile/asm_blocks.cpp`, `decompile/sm83.cpp`; the old harness binaries of
`state/work/typed-port/`), same inputs, shared machine (+-30%). New = `native/cb/decompile/{asm_blocks,frame16,shape16,sm83,l2tree}`
(`cb::Json` L2 documents, `std::string`, no py::).

| What | Python | old typed C++ | new |
|---|---|---|---|
| asm_blocks: the whole old harness (1,848 `place` cases on recorded decoders, plus 5,800 calls of its helpers; parse, run, print) | not timed | 1,264 ms | 440-540 ms (the same cases, parse and compare included) |
| sm83: the whole old harness (14,700 decode / text / assemble / table cases) | not timed | 430 ms | 191 ms (through the compat shim, so with the py:: conversions the shim does) |
| frame16 + shape16 x2 over 400 random documents (~8 KB of L2 each: lay_out, then reshape of its result and of the original), documents already parsed | 433 ms | no harness runs (the old port needs describe's decoder) | 83 ms |

The first version of shape16 re-opened Capstone for every statement it looked at (460 ms, no faster than the Python); a
`inline_asm::Decoder16` (one open handle, asked many times) opened by the pass made it 5x faster than the Python.

## cb refactor: exact batches W1-B9 and W1-B12 (2026-11-11)

Old = the Python module on the same inputs (the Python is the live implementation of both), one run, shared machine (+-30%). New = Release `cb_tests` build.

| What | Python | New |
|---|---|---|
| `exact/recut`: 300 `recut` of a 400-byte segment with 150 fix-ups | 239 ms | 72 ms |
| `exact/recut`: 300 `recut` of a 60-byte segment (3 data records) | 48 ms | 11 ms |
| `exact/recut`: 300 `recut` of a 3000-byte segment (3 data records of 1 KB) | 63 ms | 3 ms |
| `exact/habits/cmpp`: `walk` of 408 listings (random procedures, damaged and short ones; 65,000 rows) x 20 | 579 ms | 141 ms |

`exact/emu/dpmi` has no Python run to time against on its own (it runs only inside the DOS executive's emulation loop); each INT 31h it serves is
a handful of register reads and one or two memory writes.

## formats/inspect (cb::formats::inspect)

The old harness's 2,180 cases (88 files of the Win95 corpus and damaged copies: describe, render, markdown, strings, hexdump, resource_body), the calls that do the module's work, `main` left out:

| | Python (native readers behind it) | cb::formats::inspect |
|---|---|---|
| describe / render / markdown / strings / hexdump / resource_body, all cases | 0.97 s (describe 0.28, strings 0.47, hexdump 0.14, render 0.05, resource_body 0.03) | 0.35 s (describe 0.05, strings 0.18, hexdump 0.05, render 0.03, resource_body 0.02) |
| the 101 `info` command runs, output captured | not timed (a process each) | 0.85 s in one process |

About 2.8x faster on the same calls; nothing is slower. Measured on a busy machine (other builds running).
## formats.dll answers from cb::formats (pe, ne, le, dos, omf, detect), 2026-11-11

Old = master's formats.dll (its own readers in native/formats); new = formats.dll whose questions call native/cb/formats.
Every question codebreaker/formats/{pe,ne,le,dos,omf,detect}.py asks (130 of the 137 offered; never asked by the
workload: dos.pklite_head, dos.unpack_emulated, dos.unpack_exepack, le.compress, le.expand, ne._read_name_table,
ne._rsrc_string -- the last two are covered by native/formats/gate.py's samples), driven through the Python API over the
1,248 Win95 corpus files and the 1,738 OMF files (toolchains + seeded damage); both DLLs in one process, time summed
inside cbf_call (harness: state/work/fmtdiff_a39/diff.py), on a machine running a test gate beside it:

| Lane (calls) | old | new |
|---|---|---|
| PE (40,110) | 1.35 s | 1.08 s |
| NE (10,931) | 0.21 s | 0.48 s (the module is parsed again, file copied, per question) |
| LE (43,513) | 2.13 s | 1.24 s |
| DOS (17,698) | 1.35 s | 1.02 s |
| detect (9,984) | 2.65 s | 2.38 s |
| OMF (76,409) | 3.97 s | 3.96 s |
| all (198,645) | 11.66 s | 10.16 s |

Answers: 0 differences for PE, NE, LE, DOS and detect. OMF: only on damaged or non-OMF input -- the exception's kind
(old IndexError/KeyError from Python-style indexing, new ValueError/struct.error from FormatError/TruncatedError; 2,267
calls) or message, strict reads of a field cut by the end of the file (42 calls), index 0 as "no target" (102 calls), as
listed in the formats DIVERGES notes. Fixed on the way: omf rename_externs on an EXTDEF without a type index (26 MSVC
1.52 MFC objects), omf patched with a non-ASCII segment name, ne.u16/u32 with a negative offset.

## cb refactor W1-B10: exact/draft/shapes (2026-11-11)

Old = the typed port on the py:: layer (exact/draft/shapes.cpp, py::re), new = cb::exact::draft::shapes (cb::Regex, hand scans), same inputs, one run each, shared machine (+-30%).

| What | Old | New |
|---|---|---|
| the old harness: 853,952 pass runs (74 passes over 11,900 generated draft bodies of up to a few KB, results printed as JSON) | 30.2 s | 13.2 s (incl. JSON output and reading the 25 MB of cases) |

The harness answers: 853,921 of the 853,952 results equal the Python module's (expected.txt: result, or its crc32 and length, and the tables the passes recorded); the other 31 are store_joins drafts on which the Python raised IndexError (fixed in the Python, which now answers as the C++ does).
## formats/media and formats/pe_functions in native/cb/formats, 2026-11-11 (W1-B14)

Old = the live implementation of the same algorithms (formats.dll: native/formats/media/media.cc and
native/formats/win/pe_functions.cc, asked through native.call from Python; the typed ports ran this same code); new =
cb::formats::media and cb::formats::pe_functions called directly. Each timing is the mean of 3-5 runs after a warm-up, on
a machine running other builds. The old figures leave out opening the PE and the Python object work between calls, so the
comparison favours the old side; the new figures include reading and parsing the file.

| Work | old | new |
|---|---|---|
| kernel32.dll sweep + starts + extents (PeCarve) | 220 ms | 43 ms |
| kernel32.dll function_spans | 77 ms | 33 ms |
| gdi32.dll sweep (PeCarve / function_spans) | 67 / 30 ms | 14 / 13 ms |
| user32.dll sweep (PeCarve / function_spans) | 8.5 / 5.2 ms | 3.2 / 2.2 ms |
| kernel32.dll PeCarve + data_refs (every instruction decoded again) | 1,226 ms (Python) | 82 ms |
| win95.iso volumes + file list (217 files) | 0.7 ms | 0.17 ms |
| hd.img / floppy.img / parts.img volumes + list | 0.5 / 0.5 / 0.7 ms | 0.07 / 0.06 / 0.11 ms |
| floppy.img, all 31 files read | 5.8 ms | 3.6 ms |
| win95.iso, three files over 100 KB read | 3.3 ms | 1.3 ms |

The sweep is faster because it steps through Capstone's iterator (one reused instruction record) instead of asking for an
array of records per 64 KB window; the file readers keep the image open instead of opening it for every read.

Answers: pe_functions -- funcs, call targets, starts, every decoded instruction with its operands (as Capstone gave them),
resolved calls, calls_in, disasm_func, data_refs and function_spans on lz32, version, notepad, user32, kernel32 and gdi32
equal the old harness's recorded values (60 checks), disasm_whole32 on its 11 blobs (noise included) at three bases (33
checks), PeSeg of lz32 (rows, functions, names, relocation mask). media -- the volumes, boot facts, entries (with extents)
and every file's bytes of 11 images (floppy, hard disk, partitioned disk, El Torito disc, raw 2352-byte discs, Windows 95
Setup disc, a pre-DOS-2.0 floppy, two Windows 1.0 boot images), long and plain names, equal Python's line for line (966
lines); put() on copies of three of them gives images byte for byte equal to Python's.

## cb refactor W1-B2: verify/compare, verify/concolic, emulate/native_win16, analyze/globals, analyze/ppc_calls, analyze/image (2026-11-11)

Old = the engine or typed port the new code replaces (same inputs, best of 3, shared machine, +-30%). These modules are small passes; the
numbers that matter are that nothing got slower and that the recorder, the solver and the decoder (the real costs) are the same code.

| What | Old | New |
|---|---|---|
| analyze/globals: cbanalyze.dll's globals.variables + globals.tables on one loaded module (lz32 / dpwsock / password.cpl / vswks / pa3dmxd.drv; the DLL's answer alone, JSON out, from Python) | 0.5 / 0.5 / 0.9 / 0.9 / 2.6 ms | not isolated: the golden test decodes, builds the call graph and the strings table and then runs the pass for all 15 modules in 1.5 s wall (0.19 s process start); the pass is the same loop over the same references without the JSON |
| analyze/image: load_pe of 3 real PE modules, functions carved and discovered (formats::pe + Capstone) | not timed (the old loader needs the Python, formats.dll and the DLL) | 1.1 s for all three, decode included |
| verify/concolic: the 17 explorer tests, each a real Z3 session (own context) | the z3++ binding over py:: lists | 1.8 s for 72 checks, 0.19 s of it process start |
| emulate/native_win16: the recorder is the same tracer code (w16_rec.cpp); the 16 callback trampolines became one context callback (cbt_w16_set_context) | 16 fixed slots, each call through a closure | one indirect call per event |
## W1-B15: formats/xbox heavyiron and XeCrypt (2026-11-12)

New idiomatic C++ (native/cb/formats/xbox) against the original Python on the same inputs, release build, one machine, the work
in a loop of N (ms for the whole loop). Python = the original `heavyiron.py` / `XeCrypt.py` (its RSA is `pow()` on Python ints,
keygen is OpenSSL's through `cryptography`); the old typed ports are not timed (they were never faster than the Python for the
big-number parts: they carried their own BigInt).

| Work | Python | C++ |
|---|---|---|
| heavyiron parse of BULK.ho (7.8 MB, 1283 assets) x20 | 31 | 1.0 |
| XeCrypt RSA-2048 bootloader signature (sign) x50 | 302 | 185 |
| XeCrypt RSA-2048 verify x50 | 47 | 32 |
| RSA-2048 key generation x3 | 97 | 86 (CNG; with a seeded RandomSource the primes are searched here: ~270 ms each) |
| rot_sum_sha over 8 MB x5 | 3097 | 81 |
| RC4 over 8 MB x5 | 52 | 52 (state and indices in locals; about 4 cycles a byte, the dependency-chain limit of RC4, as OpenSSL's) |

## cb refactor W1-B20: port/asm16 (2026-11-11)

Old = the typed port on the py:: layer (its harness, state/work/typed-port/port.asm16, rc_run.log); New = native/cb/port/asm16*.cpp. The same work: the five Microsoft tools (lib 2.20, link 2.44, masm 4.00, msc 1.04, masm 5.10), 40 assembly modules each ported twice (plain; then with argument types, far routines, a data map, a listing and aliases), one run on the shared machine (+-30%). Results first compared on meaning: the text emitted, the kinds before every line, each routine's exit and writes, the arguments found, the C types (36,798 comparisons, port.asm16.oracle) -- all equal to the Python's.

| What | Old (typed port) | New |
|---|---|---|
| CTypes of each tree (layouts, variables, far pointers) | 0.36 s | 0.03 s |
| c_functions of each tree | 0.23 s | 0.03 s |
| Port() of every module, both variants (reading, labels, code, data) | 0.35 s | 0.03 s |
| run() of every module, both variants (the kinds, to a fixed point) | 4.05 s | 0.39 s |
| emit() of every module, both variants | 0.09 s | 0.01 s |
| all of it | about 5.1 s (the Python: about 8.9 s) | 0.5 s |

The gains are the registers (an array, not a map of strings), the operands (parsed once per line, not at every step of the fixed point), the states (origin sets shared and copied on write) and the text work (scans, not backtracking regular expressions).
## W1-B22: recompile/discover and recompile/builders (arith, logical, memory, system, vector)

Old = the typed port on the py:: layer (`py::list<py::str>` statements, helpers as `std::function` over `py::str`), new =
`native/cb/recompile/builders` (tables of statement text, `std::format`, an `Operands` interface), both Release /O2 in one test
executable, one thread, every case of the golden file (the statements of the instruction, operand tokens parsed in each call),
repeated; times for the whole loop.

| Work | Old | New |
|---|---|---|
| arith, 544 cases x60 | 39 ms | 12 ms |
| logical, 930 cases x60 | 114 ms | 26 ms |
| memory, 1,068 cases x60 | 147 ms | 43 ms |
| system, 1,195 cases x30 | 66 ms | 21 ms |
| vector, 1,137 cases x30 | 61 ms | 38 ms |

`discover` over the 16 KB Minecraft slice (4,096 words, a data section of 57 pointers, 37 functions): 0.04 ms a run with the pointer
scan (50 runs in 2 ms). The old port could not be timed beside it in the test executable (it reads its image through the old PowerPC
PE reader, which the new tests cannot link).

## cb refactor W1-B18: java/console platform_config + platform_refs, java/projects/minecraft fix passes, edition, platform_coverage, profiles (2026-11-11)

Old = the live Python (the old typed port was not timed: it ran the same regexes through the py:: layer). Converted-looking C++ text made by repeating the test fixtures; best of one, shared machine.

| What | Python (0.3 MB / 100 repeats) | New (3.2 MB / 1000 repeats) |
|---|---|---|
| applet_ping rewrite | 2 ms | 6 ms |
| sound_fix rewrite (200 SFX sites: Python replaces the whole file once per site) | 6112 ms (quadratic; did not finish in 4 min at 1000 repeats) | 19 ms (a local routed once, then skipped) |
| render_fix rewrite | 2 ms | 10 ms |
| platform_coverage provided_symbols / used_holder_symbols | 31 / 8 ms | 5 / 0 ms |

Per MB the applet and render passes run at about 2 and 3 ms; the Python's were 7 and 7 ms per MB, so nothing is slower. The platform_config, platform_refs and edition modules are a few file writes and string builds (microseconds); not timed.

## cb refactor W1-B23: source/ne_asm (segment, fixups) and source/vxd (disassembly, coalesce) (2026-11-11)

Old = the typed ports on the py:: layer, timed by their own harnesses (state/work/typed-port/source.vxd.disassembly and source.vxd.coalesce, rc_run.log); New = native/cb/source/{vxd/disassembly*.cpp, vxd/coalesce.cpp, ne_asm/*.cpp}, Release, one thread, the same machine. The new code was first compared on meaning with the Python module's own results over the same inputs, each with an environment variable that names the harness folder (the tests skip without it): `CB_VXD_DISASSEMBLY_HARNESS` -- 225 real Windows 95 drivers, instructions, code bytes, labels, jump tables, service table and device name all equal; `CB_VXD_COALESCE_HARNESS` -- 1,855 cases (coalesce, the three restores, finalize, over 54 sampled drivers and their split-record, padded-name, short-name and no-version variants), byte for byte (SHA-1) equal except 4 cases on 2 deliberately damaged copies, where the Python read past the end of a page; `CB_NE_SEGMENT_HARNESS` -- 172 segments of the Windows 3.x and Setup modules, the whole digest equal (rows, fix-ups, exports, function starts and spans, arguments, calls, window-procedure dispatch, handler calls, a checksum of the annotated rows, the call graph).

| What | Old (typed port) | New |
|---|---|---|
| vxd/disassembly: the 273 `disasm` cases (open the module, recursive descent, summary hashes) | 19.4 s | 1.5 s for the recovery of the 225 drivers that have an answer (the whole test, with opening and comparing: about 2.2 s) |
| vxd/disassembly: the listing of each driver (`emit`) | 14.6 s | 0.87 s (225 listings, 49 MB of text) |
| vxd/coalesce: the five operations of the harness (coalesce 2.46 s, finalize 0.57, restore_entry_point 0.27, restore_metadata 0.25, restore_version_resource 0.20) | 3.75 s | 0.4 s for 1,855 cases, with reading each file, opening the oracle and the SHA-1 of every answer |
| ne_asm/segment and fixups: 172 segments, the whole digest | not run here | 11 s, of which nearly all is starting objdump once or twice per segment (the module's own work is the text of its listing) |

The gains of the disassembly are the per-byte states (an array of three bits per byte of an object, where the Python and the port kept a set of (object, offset) places for every byte of code, of data and of entry points), the places kept in ordered maps instead of dict handles, the decoder opened once per width (the port looked one up for every instruction), and the listing built by appends. coalesce reads each fix-up record once into a record and writes the grouped ones from a sorted index; its time is the copy of the file. The segment tools run objdump, an external program, and read its text: nothing in them is on a hot path.
## W1-B24: symbols/runtime_library, api_arguments, cpp_classes, driver_services

The old ports (py:: layer) cannot be linked into the test executable, and a full exe build is not allowed on this drive, so they
are not timed here. The new code is timed against the ORIGINAL Python on real inputs, with the Python's answers as the oracle:
every answer below was compared one for one and was the same (the oracle ran as a temporary test, not kept: it reads the Win95
corpus and C:/Nexia360 and the Python's dumps). Release /O2, one thread, C++ times include building the module's inputs from the
dump (hex-decoding the code of a DLL) and exclude finding the function starts (the formats reader's carve), which the Python's
`identify` includes. The typed ports were measured at 3.5x (api_arguments, ordinals, runtime_library, names) and 4.7x
(cpp_classes) over the Python above; the new code is faster than that in every row.

| Work | Python | New C++ |
|---|---|---|
| api_arguments: the stack bytes of every export of 14 Win95 DLLs (kernel32, user32, gdi32, advapi32, comctl32, shell32, lz32, crtdll, mfc40, mnmcpi32, ole32, comdlg32, wsock32, winmm: 12,131 exports, all equal) | 26.2 s | 0.37 s |
| cpp_classes: Minecraft.Client.xex (115 classes), 545107DE default.xex (1,274 classes, 3,418 virtual functions, 657 found by layout) | 0.13 s, 0.27 s | 0.0024 s, 0.021 s |
| driver_services: read the 87 include files of the driver kit (108 devices, 37 service tables, 51 messages) | 0.036 s | 0.017 s |
| runtime_library: identify crtdll.dll (578 functions) / mfc30.dll (1,759) / mfc40.dll / vbrun300.dll (72, 16-bit) / user.exe (16-bit) | 0.13 / 0.65 / 0.71 / 2.3 / 2.6 s | 0.014 / 0.35 / 0.073 / 0.042 / 0.029 s |
| runtime_library: shell32.dll, user32.dll (the two modules where most of the time is the matching) | 0.67 / 0.064 s | 0.68 / 0.073 s |

The shell32 and user32 rows are level with the Python, whose time there also includes the formats reader's function carve that the
C++ row leaves out: what is left is the matching itself, one SHA-1 per candidate row of the table whose leading bytes fit (a start
that begins `push ebp / mov ebp, esp` fits hundreds), which both do the same number of times; a table that grouped the rows of one
size and mask so that each was digested once per start is the next step if this ever shows in a profile. The gains elsewhere are the
table (a hash of leading bytes over byte strings, not a dict of `py::bytes` lists), the walk (a flat decoder record, no regular
expression per instruction) and the search (sets over words, no `py::dict` of dicts).
## W1-B21: recompile/powerpc (and config, port/oldlayout)

Old = the Python (the old typed port could not be linked into the test executable: it reads its image through the old PowerPC PE
reader), new = `native/cb/recompile/powerpc.cpp`, Release /O2, one thread, the whole 16 KB Minecraft slice (4,096 instructions) as one
function with its jump-table recovery, per round.

| Work | Old (Python) | New |
|---|---|---|
| powerpc.recompile_function, 4,096 instructions | 48.9 ms | 10.8 ms |

What remains is mostly Capstone's decode. config (one `config.make` of a 2 KB program: 0.5 ms in the Python) and oldlayout (clang
dominates) were not timed separately.

## translate/backends/jmodel and module_file (W1-B28)

Both are one-shot, per-program work (the object model of a Java program; the head and parts of one source file), a few
microseconds to a millisecond; neither has a hot path, so no old-versus-new timing was taken. The cycle search of jmodel
is now linear in the classes plus edges (the Python rescanned every class for each subclass lookup); module_file builds each
file with one pass over its parts instead of repeated list concatenation.

## translate/frontends/python/program (W1-B29)

`Program` is the symbol tables and facts of the Python front end; the work that is not a one-off is loading a package and building
its symbol tables (`add_tree` + `index`). Timed on the 552 modules of `codebreaker/` (7,374 functions, 456 classes), best of three,
a heavily loaded machine (other builds running), Release:

| step | Python (program.py) | C++ (cb) |
|---|---|---|
| add_tree: read and parse 552 modules | 2,980 ms | 846 ms |
| index: symbol tables, bases, `none_aware`, `index_params` | 1,951 ms | 133 ms |

The old typed port (`native/typed/cpp/.../program.cpp`) links only into the translated executable, so it was not timed on its own; it
does the same work over `py::str` and `py::dict`. Parsing is pyfront's (unchanged); `index` was the Python's ast walks and is now one
walk per function over a flat child list.
