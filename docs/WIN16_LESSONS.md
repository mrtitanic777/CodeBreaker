# Win16 rebuild: what went wrong, and what the tool must do on its own

Phase 2 (2026-10-14 .. 10-17) rebuilt Setup's MINI.CAB mini-Windows (KRNL386, GDI, USER and
13 drivers/DLLs) from our source until Setup ran through Continue to its License page with
every module ours -- USER with all 397 functions it has in C. This is the record of each
problem met on the way: what it looked like, what it really was, what CodeBreaker now does
about it by itself, and what is still left for a person (or an LLM) to notice. The last
column is the backlog: each "still manual" entry is something the tool should learn so the
next module does not cost the same days.

Status words: **automatic** -- the build handles it with no one looking; **detected** -- the
build notices and reports it; **manual** -- found only by someone booting and digging.

## 1. The rebuilt code was wrong

| # | Symptom | Real cause | Now | Still manual / next |
|---|---|---|---|---|
| 1.1 | GDI crash "Invalid DPMI return" at boot | OpenWatcom's default convention is its own register one: an unnamed `typedef int code();` passed every indirect call's arguments in registers | **automatic**: every emitted prototype/typedef names `__cdecl`/`__stdcall` (`c_text.py`) | a lint that refuses C text with an unnamed convention |
| 1.2 | Caller's stack off after a call | how many argument bytes a callee removes was guessed from the C, not the instructions | **automatic**: `describe.py` reads `retf N`, hand-popped arguments (pop return, pop args, push return; also through a helper `call; jmp bx`), argument-inserting thunks (`forwards`), `callers_say` consensus; `contract16` compares the caller's cleanup after each call with the original's | -- |
| 1.3 | Results cut to 16 bits | DX:AX results where DX is set from a segment register before AX | **automatic**: `describe._segment_in_dx`, result sizes locked from the instructions | -- |
| 1.4 | Arguments shifted in far calls | callers decompiled before a callee was found | **automatic**: callers re-decompiled when new functions appear | -- |
| 1.5 | Wrong far reads | `*MK_FP(seg, p+2)` typed by a near cast | **automatic**: typed by the pointer's element type (`_far_addresses`) | -- |
| 1.6 | Stack records (CBT_CREATEWND, WNDCLASS, BITMAP, PAINTSTRUCT) split or half-filled | a structure passed by address was seen as separate locals; Ghidra's alias cut-off at level 0; `mov [bp-x], ss` stores dropped | **automatic**: `_buffer_fields` folds an escaped array up to the next one; `varmap.cc` skips the cut-off at alias level 0; `_stack_segment_stores` puts the SS store back | -- |
| 1.7 | Boot dies inside KERNEL's instruction decoder | functions with a fault-range table (`start,end,resume` words naming inner instruction offsets: IsBadStringPtr's `__GP` chain) | **automatic**: `origin.py` keeps such functions in assembly | -- |
| 1.8 | Random DS after a call | "DS keepers": `push ds .. call .. pop ds` without loading DS -- the callee may change DS, C cannot promise to keep it | **automatic**: kept in assembly (`origin.py`) | -- |
| 1.9 | Registers clobbered across calls | Watcom used FS/GS as scratch | **automatic**: `-zfp -zgp` | -- |
| 1.10 | `__loadds` too late | the DS load came after the frame setup | **automatic** | -- |
| 1.11 | "Call to Undefined Dynalink" in KRNL386 | a self-loading module (KRNL386: its MZ part covers the whole file) got extra `CODE<n>X` segments | **automatic**: none for self-loaders; stub verbatim, file alignment, chained selector fix-ups (`STUBVERBATIM`, `SELECTORFIXUPS`) | -- |
| 1.12 | Hand-written assembly turned into wrong C | origin not checked first | **automatic**: hand-pop, call-and-return stubs and the above stay assembly | origin triage still misses kinds we have not met |

## 2. The build put good code in the wrong place

These were the last USER failures, and none of them was the C. A function rebuilt from C is
placed after its segment's assembly frame; its old place becomes `int 3` filler. Anything that
still reached the old place ran the filler (each `int 3` is handled and goes on) into the
next function.

| # | Symptom | Real cause | Now | Still manual / next |
|---|---|---|---|---|
| 2.1 | Raw near calls into a moved function | `E8/E9` in bytes the frame keeps raw (code reached only through a table) | **automatic**: `raw_calls_into_holes` writes them by name | -- |
| 2.2 | BEAR306 alone in C: Windows dies, `pop ds` takes garbage in CreateWindowEx's internals | a window procedure registered as a far pointer built in code: `mov [bp-18h], 1587h` + a selector fix-up; only the selector moved | **automatic**: `asm16._far_pointer_offsets` writes the number as `offset SYMBOL`, paired with the selector fix-up beside it (a frame may hold selectors of several segments whose functions share an offset: USER 12:0FF1 / 35:0FF1) | -- |
| 2.3 | 085E+092B+0A92 together in C: Setup quits cleanly | a `call far 12:085E` in raw bytes: offset a number, selector a fix-up; ran the filler into the next function, whose garbage arguments made GDI divide by zero; Setup's divide handler exits | **automatic**: `asm16._raw_far_pointers` (offset before the selector, or `push seg / push off`) | -- |
| 2.4 | A helper renumbers segments | the runtime's long division was linked before the original's second data segment: 44 → 46, and every selector after it | **automatic**: link order keeps the original's segment numbers, additions last (`module.py`) | -- |
| 2.5 | Far calls landing in `int 3` filler (USER 35:035A → 43:0234, 38:07A2 → 36:0043) | a second entry point Ghidra merged into the function before it; another segment calls it far from raw bytes; once the outer function is C the entry is gone | **automatic**: `asm16` reports a raw `call far`/`jmp far`/`push seg, push off` with no function at its target as an entry; the build adds it as a label or keeps the containing function in assembly (`raw_far_entries` → `wanted`) | -- |
| 2.6 | (latent) | a moved function's offset stored in a **data** segment with no fix-up beside it (a near pointer table) | **manual** | scan data segments for words equal to moved functions' offsets referenced by the code; report them |

**The rule to keep:** when a function moves, every reference to its old offset must be
symbolic. The build should *prove* it: after linking, look for any word in the image (code
or data) equal to a moved function's old offset that sits next to a selector fix-up for its
segment, or is used as a call/jump target -- and fail the build if one is left.

## 3. The checks lied

| # | Symptom | Real cause | Now | Still manual / next |
|---|---|---|---|---|
| 3.1 | Hours of hunt results void | the VM verdict "screen is 640×480 graphical" counted Windows' error boxes as passes | **automatic**: `setup16`/`cont` pass only on the License page's grey pixels, early exit on DOS / error box | the verdict should read the screen text (or Setup's state), not pixels |
| 3.2 | "gated 0" while the gate crashed | worker processes crashed; "not run" was cached as "same" | **automatic**: worker stderr captured; "not run" never cached; errors in the report | -- |
| 3.3 | Stale gate verdicts | the cache key held the function's C but not the callee descriptions the checker fakes calls with | **automatic**: keys include callee description lines | audit every cache's inputs whenever its producer changes (see 5.2) |
| 3.4 | The gate passes functions that break Windows | `isolated16` runs one function with callees **faked**; GDI/KERNEL are stubs (BeginPaint never fills PAINTSTRUCT); it cannot see far pointers, init-time state, callee side effects | **detected only in the VM** | the real-modules gate: dump the running VM (all modules, real state), replay functions in Unicorn inside it; and the live-VM tools below, as a command |
| 3.5 | contract16 crashed on `add sp, word ptr` | unhandled operand form | **automatic** | -- |
| 3.7 | The regression passed a build that GP-faults (USER 1:007F) when nothing is typed | the harness pressed Enter during Setup's DOS phase, which Setup does not need; the keystrokes changed what Windows did and hid the fault; its "Welcome" check also took a white error box for the Welcome dialog | **automatic**: no keys before Welcome (opt-in `CONT_DOSKEYS`); an error box before Welcome is a failure (`setup16`, `cont`) | -- |
| 3.8 | "USER alone passes" while USER is broken | the original KERNEL's fault handler decodes and skips a faulting instruction (its IsBadPtr machinery, 1:A15E/1:A296): our USER's GP in the hook dispatcher (1:8093) was silently recovered; with our KERNEL as well it was not | **detected**: a verdict by the CPU, not the screen -- `kfault`: any ring-3 #GP/#DE/#UD outside KERNEL's own handlers in a `-d int` trace is a failure | make it the standard gate for every hunt |
| 3.9 | Pairs that fail only together (USER 7:03F8 + 39:05AC, one of which never runs) | the build put far C functions that did not fit their slot into a new segment ("beside"): a segment the original does not have renumbers the selectors of everything loaded after it | **automatic**: a function stays in its own segment (appended); a new one only past 64K. Left: the C runtime's helpers still make one extra segment at the end | put the runtime helpers into an existing segment |
| 3.6 | A "fix" blamed for a regression it did not cause | compared against the wrong baseline | **manual** | every A/B change runs old/new/control in the same harness (the buffer rule: 20/21/22 gated) |

## 4. Finding a VM-only bug (how the last ones were found)

Static reading did not find 2.2 or 2.3 -- the functions blamed were correct. What did:

1. **`qemu -d int`** on the failing run, then look for CPU exceptions at ring 3: `v=0d` (GP:
   the faulting CS:IP and selector), `v=00` (divide), and **runs of `v=03`** -- the filler of
   a moved function being executed.
2. **Name the code**: the CS base/limit from the log → the module segment (by length, then by
   its bytes in a memory dump); USER's selectors here are `81Fh + 8 × segment`.
3. **Break in the live VM** through QEMU's gdbstub: find the segment in the LDT by its bytes
   (mask fix-up sites), set breakpoints, log each call's arguments and AX/DX; diff ours vs the
   original. Attach **from power-on** -- USER loads its bitmaps/icons at start, before
   anything shows.
4. **Catch the fault** (a breakpoint on the faulting instruction with a condition, e.g. BX = 0
   at a `div bx`) and walk the BP frames (far frames: return CS at `[bp+4]`) to the caller.

Pitfalls met: QEMU's monitor socket takes one client at a time (connect per command, read to
the `(qemu)` prompt); an LDT copy read early is stale for selectors made later; an error box
can look like "Setup's graphical screen"; the VM runs without paging (CR0 = 11h), so linear =
physical.

**Next:** these are scratch scripts today (`gdbwatch`, `gdbdiv`, `walkbp`, `trymini` in the
session scratchpad). They belong in `codebreaker/verify/live16.py` as commands:
`watch`, `catch`, `trace`, plus the snapshot gate (3.4).

## 5. How we worked

| # | Problem | Rule now |
|---|---|---|
| 5.1 | A Continue test leaked 650 MB per run (hd + CD copies); ~230 runs filled C: while Logan was away | every harness deletes its per-run images in `finally`; check free space before long hunts; trim scratch constantly |
| 5.2 | Fixes to shared machinery broke modules that already passed | after any change to describe/origin/c_text/decompiler/build/cache: rebuild and run **all** MINI.CAB modules through Continue; say what was not re-verified |
| 5.3 | Cold rebuilds of USER cost minutes per hunt step | every expensive step is cached on its real inputs (decompiled_fn, gate16, assembled16, objects, wasm16, compilable16, origin16, omf_publics) |
| 5.4 | Hunts ran stale code while sources were being edited | stop hunts before shared changes |
| 5.5 | Six VMs at once strained the PC | at most ~3 VMs |
| 5.6 | Edits through shell heredocs broke on escaping | use the editor tools for code edits |
| 5.7 | A regex named `_LOCAL` in two places; the 16-bit one shadowed the 32-bit one | unique names; the 32-bit lane still to re-verify after that fix |

## Where each fix lives

- Function facts: `codebreaker/decompile/describe.py` (pops, thunks, result sizes, `callers_say`)
- Origin (asm or C): `codebreaker/analyze/origin.py`
- C text: `codebreaker/decompile/c_text.py`; Ghidra core: `native/decompiler/core/cpp/varmap.cc`
- Frames: `codebreaker/build/asm16.py`; link order, gates, caches: `codebreaker/build/module.py`
- Checks: `codebreaker/verify/gate16/isolated16.py`, `contract16.py`, `setup16.py`
