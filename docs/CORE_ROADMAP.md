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
- A -- verified SLEIGH (2026-10-18): every instruction form the golden 16-bit corpus uses checked
  against Unicorn (tools/sleigh_verify.py; docs/SLEIGH_VERIFY.md): 476 forms, 399 agree, 9 differ
  on purpose, 68 untested; four spec bugs fixed (pushf bit 1, iret flags, aad/aam flags, shr/shrd
  OF). x86-32 (KERNEL32, GDI32, flat): 277 forms, 247 agree, 1 differs on purpose, 29 untested;
  no bug found. PowerPC (2026-10-19; tools/sleigh_verify_ppc.py): the Xbox 360 SDK's instructions
  against Unicorn (32-bit mode) and the PowerPC interpreter: 279 forms, 226 -> 233 agree, 4
  explained, 42 untested (VMX); fixed mtxer/mfxer, stwcx./stdcx., fctid/fctiw (L6).
- B -- parallel analysis in one process (2026-10-18): `CB_THREADS=N` (setting `threads`), each
  thread its own Architecture over one shared read-only image (core change C11). Output identical
  to one thread on all 11 modules. Golden corpus: 868 s -> 366 s at 4 threads (2.4x); peak memory
  ~3x (USER 72 -> 216 MB, KERNEL32 89 -> 261 MB). Found on the way: a function's C depended on
  the functions decompiled before it in the same process (the globals an analysis named, and their
  types, stayed for the next) -- each function now starts from the description alone; 418 golden
  functions changed, every one in a global's name, size or type. Results were already cached per
  function by input hash (codebreaker/decompile/decompiler.py), which this makes sound.
- C -- structures from the code's own accesses (2026-10-18), `CB_STRUCTS=1`; on by default for 32-bit
  x86 since 2026-10-19 (setting `struct_recovery`: off / 32 / all -- 16-bit waits on the Win16 gate and
  the VM; decompiler.cc, "Structures"; core change C12). Every load
  and store through `v + c` is a constraint on v (a field of that size at c), followed back through
  constant additions, copies and a near segment; a variable whose constraints agree (two or more
  fields, none overlapping, never indexed at run time; reads only probed or written back do not
  count) gets a structure with exactly those fields and the function is analysed again with it.
  Golden (tools/struct_report.py): 2,349 structures, 9,096 fields in 1,701 functions;
  `*(T *)(p + n)` 17,302 -> 7,347 (-58%); no new failures (COMMAND.COM's two are its time limit
  under load, with or without). SDK: of 25 exported parameters the header declares a pointer to an
  SDK structure, 24 agree field for field (85 of 91 fields; the other is CHARSETINFO, copied a
  DWORD at a time). Found: OpenWatcom's wingdi.h declares FONTSIGNATURE `fsCsb[4]`; Microsoft's
  SDK and GDI32 itself have `fsCsb[2]` (24 bytes). One thread and four agree byte for byte.
  Next: far pointers (phase 2's segment types), structures passed between functions.
  Turned on for 32-bit (2026-10-19): c_text declares each `/*@S` structure (packed, at its exact
  layout, filler between fields) before the prototypes; an offset with no field (`&p->field_0x4`,
  `&p[1].field_0x8`) is the bytes there; the structures' sizes keep a frame's locals at their
  offsets; a pointer -- parameter or cast -- compared with a number is compared as one; and on the
  second analysis the globals keep the first one's types (a structure is the function's own: a
  global copied from a variable given one took it over). The setting is part of the cache key.
  Behaviour (measure + tools/verdict_diff.py, off vs on): KERNEL32 1,867 and GDI32 865 functions,
  no verdict better or worse; 790 read differently (fields for `*(T *)(p + n)`). The first try was
  111 not compiling (from 33) and 18 worse. Far pointers (16-bit, `les bx, p; es:[bx+8]`) are
  recognized too, not measured yet; 16-bit stays off.
- D -- fewer gotos (2026-10-18; tools/goto_report.py): a short return tail that paths reach by
  `goto` is copied onto each path (core change C13). Golden: gotos 6,923 -> 6,449, 20.2 -> 17.9
  per 1,000 lines; to a bare `return` 225 -> 26; 203 functions changed, no new failures. What is
  left is mostly loops the structurer could not make one (2,453 backward) and crossing forward
  jumps (3,266): those need restructuring proper (condition-based refinement, "no more gotos"),
  the next step. Found on the way: the 7B governor could stop a function at once, "over the time
  limit", when the one before it on its thread had run long (fixed, 067b32e).
  Then (C14) tails holding calls: the core's block copier now copies calls and their side
  effects, so `goto L; ... L: f(); return x;` becomes the call and the return in place. Gotos
  6,449 -> 5,701 (18.8 -> 15.7 per 1,000 lines; KERNEL32 -25%, GDI32 -29%, USER -20%); 350
  functions changed, no new failures, one thread and four agree. Left: 2,341 backward, 3,096
  forward, mostly COMMAND.COM (hand-written assembly; 3,090 of them) -- loops with several
  entries and crossing jumps, for restructuring proper.
  Then (C15) a goto target that flows into the shared return is given its own copy of it and
  joined to it, so it too can be split: 5,701 -> 5,417 (14.9 per 1,000 lines); 136 functions
  changed, no new failures. Since the start of 7D: 6,923 -> 5,417 (-22%).
  Then (C16) a goto that jumps over a small join block (`if (a) { ...; goto L; } x = 0; L:`):
  the block is copied for each way in -- the core can now split a block that flows on, patching
  its successor's MULTIEQUALs -- 5,417 -> 5,361, no module worse. Since the start: -23%.
