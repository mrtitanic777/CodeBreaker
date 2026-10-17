# The decompiler core: deepen it, then build its successor

The engine is Ghidra's decompiler core (C++, ~190,000 lines, `native/decompiler/core`) driven by
`native/decompiler/decompiler.cc`. It was built for flat memory. CodeBreaker's executables are
mostly not flat -- Win16 NE modules and DOS programs are segmented, VxDs call the system through
`int 20h` -- and today the gap is closed *after* the core: 32 passes in
`codebreaker/decompile/c_text.py` rewrite its C with regular expressions, and much of the
module's structure is decided outside it in `describe.py`. Most of the bugs of phase 2 were born
in those passes (docs/WIN16_LESSONS.md).

Logan (2026-10-17): "I think the Ghidra core needs to be massively updated and by design so will
CodeBreaker be." Two tracks:

- **Track 1 -- evolve the Ghidra core.** The main line. Teach it what the repair passes do, make it
  verified, parallel and better at types and structure. Its rules and structuring are decades of
  correctness work and stay.
- **Track 2 -- a new core, long horizon.** A Rust engine built one subsystem at a time beside the
  Ghidra core, until it matches or beats it on every golden function. Starts after phase 2 lands.

## The rules every change follows

1. **The golden comparison.** Nothing in a core changes without `tools/golden.py`: the C of every
   function of a fixed corpus, before and after, diffed function by function. A change lands only
   when each difference is one it meant to make -- then the full build + gate + Setup regression.
   Corpus: 11 modules, 6,813 functions -- MINI.CAB's NE modules, PE (kernel32, gdi32), LE (vkd),
   DOS (COMMAND.COM).
2. **Behaviour, not text.** Where output must change, it is checked by emulation (the per-function
   gate, and the VM) as well as by eye.
3. **The machine stays up.** The governor (memory budget, OS ceiling, time limit) is a hard
   backstop and stays; bounded analysis fails a bad function early. Heavy work runs as one heavy
   process at below-normal priority. (2026-10-17: two blue screens -- cfosspeed6.sys, since
   uninstalled, and commit exhaustion: decompiler.exe at 27.7 GB against a 4 GB page file.)

## Track 1 -- evolve the Ghidra core

### Native support for every executable class (phases)

| Phase | In the core | Passes it retires | Formats |
|---|---|---|---|
| 0 | the golden harness; profile of a full build | -- | all |
| 1 | **bounded analysis**: a bad start fails early and cheap; the governor as backstop | -- | all |
| 2 | **segmented addresses**: a far pointer is a type (selector:offset); DS and SS are known values per function; reads through ES/DS print as such; a selector loaded from a fix-up is a segment name | `_far`, `_far_loads`, `_far_arguments`, `_far_addresses`, `_far_reads`, `_near_to_far`, `_far_locals`, `_segment_registers`, `_loads_dgroup`, `_selector_sites`, `_stack_segment_stores`, `_ram_type`, `_global_type` | NE, DOS |
| 3 | **frames as structures**: a function's stack frame keeps the original's layout; a buffer whose address escapes is a field of it | `_buffer_fields`, `_frame_block`, `_keep_frame`, `_entry_sp`, `_stack_arguments`, `_homed_arguments`, `_passed_on` | NE, DOS, PE, LE |
| 4 | **Win16 conventions**: Pascal/C, near/far, arguments removed by `retf N` or by hand, register arguments, DX:AX results | `_open_arguments`, `_fit_calls`, `_joined_arguments`, `_arguments_at`, `_pieces` | NE, DOS |
| 5 | **system calls as functions**: INT 21h/2Fh/31h by AH/AX, VxD `int 20h` services (VMM/VxD service tables, register conventions from the DDK) | `_interrupts` | DOS, NE, LE |
| 6 | **real mode**: segment arithmetic (`seg*16+off`) for DOS programs and boot code | -- | DOS |
| 7 | **function discovery in the core**: far-pointer targets, prologues after an end, jump tables, the hidden-function guard, moved from `describe.py` | (describe.py) | all |

### Making the core better, not only broader

| Item | What | Measured by |
|---|---|---|
| A | **Verified SLEIGH**: every instruction's p-code checked against an emulator (x86-16, x86-32, PowerPC) over sampled states; spec bugs fixed; a per-instruction report | instructions agreeing / disagreeing / untested |
| B | **Parallel analysis in one process**: functions across threads over one shared, read-only program image, instead of a process per job; results cached by input hash | wall time, RAM per function, before and after |
| C | **Constraint-based type recovery** (Retypd/TIE style) in place of heuristic propagation -- first the struct and far-pointer cases of phases 2-3 | golden diffs; types agreeing with SDK headers |
| D | **Goto-free structuring** where the core still emits goto | gotos per 1,000 lines in the corpus |
| E | *Later:* ML/LLM naming and type suggestions, layered on verified output and never changing its behaviour | -- |

### Order

1. **Now:** phase 1 follow-ups (jump-table forms the core mis-sizes, GDI32's failures, COMMAND.COM's
   speed) -- bounded analysis is in.
2. **Phase 2** (segmented addresses) -- the largest share of the repair passes and of the bugs.
3. **A** (verified SLEIGH) alongside phase 2: it finds wrong semantics that every later phase would
   otherwise build on. Starts with x86-16, the instructions the corpus uses most.
4. **B** (parallel in one process) -- the speed and memory lever, once the governor's per-function
   accounting is per thread.
5. **Phase 3** with **C** (frames and their types belong together), then phase 4, 5, 6, 7.
6. **D** once types settle; **E** last.

## Track 2 -- a new core (after phase 2)

A Rust engine, built to replace the Ghidra core only where it is proven better:

- one SSA IR end to end (no separate rebuild between analysis and decompilation)
- query-based, incremental recomputation (Salsa-like): change a type, recompute only what depends
- parallel by default; storage memory-mapped (LMDB/SQLite)
- a headless server; any UI a client (Ghidra's Java UI included)

Built one subsystem at a time -- p-code lift, data flow, types, structuring -- each running beside
the Ghidra core on the golden corpus until it matches or beats it on every function.

Further ideas, each to be weighed when its turn comes: Datalog for points-to and data flow; e-graphs
for simplification; CRDT multi-user editing; sandboxed WASM plugins; first-class Python; an
integrated time-travel debugger; SLEIGH generated from vendor formal specifications (ARM ASL,
RISC-V Sail, Intel XED).

## Status

- Governor (memory budget, OS ceiling, time limit) -- built into `decompiler.cc`, 2026-10-17.
- Phase 0 -- the golden corpus frozen (tools/golden.py; state/golden): 11 modules, 6,813 functions,
  deterministic. Baseline: 59 failures (GDI32 38); LZEXPAND had 2 runaway functions; COMMAND.COM
  takes 116 s for 536 functions (~10x the NE rate).
- Phase 1 -- bounded analysis in the core (budget checked between actions, between rule
  applications, whenever an op is made; the function fails in the same process). Root causes found
  and fixed: a MULTIEQUAL behind another op kept an input for a removed edge (5 functions now
  decompile); jump tables the core sized from a wrong range -- the describer now gives each 16-bit
  `jmp cs:[reg+table]` its exact targets (`J` lines) and the core's switch-variable search no
  longer takes an unrelated MULT for the index's scale (USER sub_1_032E: 0.37 s, was a runaway).
  Open: other jump-table forms (GDI 2, COMM 2 "flow exceeded"), GDI32's 38 failures, COMMAND.COM.
