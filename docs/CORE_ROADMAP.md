# Deepening the decompiler core: every Windows 95 executable class, natively

The engine is Ghidra's decompiler core (C++, ~190,000 lines, `native/decompiler/core`) driven by
`native/decompiler/decompiler.cc`. It was built for flat memory. CodeBreaker's executables are
mostly not flat -- Win16 NE modules and DOS programs are segmented, VxDs call the system through
`int 20h` -- and today the gap is closed *after* the core: 32 passes in
`codebreaker/decompile/c_text.py` rewrite its C with regular expressions, and much of the
module's structure is decided outside it in `describe.py`. Most of the bugs of phase 2 were born
in those passes (docs/WIN16_LESSONS.md). The plan: teach the core what those passes do, one
subsystem at a time, until they can be deleted.

Not a rewrite: the core's simplification rules, type propagation and structuring are decades of
correctness work and stay. What is added is what Win16/DOS/VxD need and flat code never did.

## The rule every change follows

Nothing in the core changes without the **golden comparison**: the C of every function of a fixed
corpus, before and after, diffed function by function (`tools/golden.py`). A change lands only when
each difference is one it meant to make, and then the full build + gate + Setup regression passes.

Corpus: all 15 MINI.CAB modules (NE + DOSX), plus samples of each class -- PE (kernel32, a
DLL), LE (vmm32 drivers), DOS/MZ (IO.SYS, COMMAND.COM).

## What the core lacks, and the passes that stand in for it

| Phase | In the core | Passes it retires | Formats |
|---|---|---|---|
| 0 | the golden harness; profile of a full build | -- | all |
| 1 | **bounded analysis**: a bad start fails early and cheap (flow, block, varnode limits), replacing the 200,000-instruction retry; the memory/time governor stays as the backstop | -- | all |
| 2 | **segmented addresses**: a far pointer is a type (selector:offset); DS and SS are known values per function; reads through ES/DS print as such; a selector loaded from a fix-up is a segment name | `_far`, `_far_loads`, `_far_arguments`, `_far_addresses`, `_far_reads`, `_near_to_far`, `_far_locals`, `_segment_registers`, `_loads_dgroup`, `_selector_sites`, `_stack_segment_stores`, `_ram_type`, `_global_type` | NE, DOS |
| 3 | **frames as structures**: a function's stack frame keeps the original's layout; a buffer whose address escapes is a field of it | `_buffer_fields`, `_frame_block`, `_keep_frame`, `_entry_sp`, `_stack_arguments`, `_homed_arguments`, `_passed_on` | NE, DOS, PE, LE |
| 4 | **Win16 conventions**: Pascal/C, near/far, arguments removed by `retf N` or by hand, register arguments, DX:AX results | `_open_arguments`, `_fit_calls`, `_joined_arguments`, `_arguments_at`, `_pieces` | NE, DOS |
| 5 | **system calls as functions**: INT 21h/2Fh/31h by AH/AX, VxD `int 20h` services (VMM/VxD service tables, register conventions from the DDK) | `_interrupts` | DOS, NE, LE |
| 6 | **real mode**: segment arithmetic (`seg*16+off`) for DOS programs and boot code | -- | DOS |
| 7 | **function discovery in the core**: far-pointer targets, prologues after an end, the hidden-function guard, moved from `describe.py` | (describe.py) | all |

PE (flat 32-bit) is already the core's home ground; it gains from phases 1, 3, 5 (its import
thunks) and 7.

## Status

- Governor (memory budget, OS ceiling, time limit) -- built into `decompiler.cc`, 2026-10-17.
- Phase 0 -- next.
