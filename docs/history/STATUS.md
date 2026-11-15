# CodeBreaker — where it stands

Written 2026-10-24 from a `reach` run over the programs on the owner's PC (the commands below
regenerate every number). The previous version of this page (2026-10-13) quoted figures from the
first verifier, which later turned out to pass broken functions; they are not repeated here.

## How to read a number here

A function counts as **verified** when it was decompiled, rebuilt, run beside the original on the
same inputs and did the same things (calls, arguments, results, memory written). **Weak** means it
matched with a stated limit (most often: under half of its instructions ran on any input).
**Unverified** means it did something different, or did not compile. For the assembly lanes (DOS,
drivers, 8-bit and 16-bit consoles) the whole program is also rebuilt from its source and compared
byte for byte.

How far "verified" can be trusted is itself measured: `teeth` breaks functions the judge passed and
counts how many breaks it catches (README, "What a verdict means"). Measured 2026-10-27 on 13
modules (9 32-bit Windows, 3 16-bit Windows, 1 Xbox 360; the reach sample below plus six earlier
ones), three rounds each, `state/teeth/2026-10-27/`:

| verdict | breaks caught |
|---|---|
| VERIFIED | 800 of 1,005 (80%) -- from 66% (calc.exe, wfm0203.acv) to 95% (nec24pin.drv) |
| VERIFIED (weak) | 175 of 261 (67%) |

By kind of break: a comparison turned round 95%, two arguments swapped 92%, a constant one higher
68%, a plus made a minus 58%, a store left out 56%. So a VERIFIED function is right about four times
in five against one small wrong change; read the verified shares below with that margin. Two thirds
of the breaks that survived (131 of 199) were in functions whose every instruction ran: the inputs
reached the broken line and the judge did not see its effect -- a store to memory it does not compare,
a constant whose +1 the inputs do not distinguish. That is the next work on the judge, ahead of more
inputs.

Looking at those survivors line by line (`teeth` now records the line it changed): the memory the
inputs gave was too even. Every global read as zero (the emulator does not load a module's data), so
two globals swapped, or a loop that clears a table it no longer reaches, changed nothing; every
object was the same word all through, so a field read one byte off read the same value. Since
2026-10-27 (32-bit lane, PIPE_REV 45) the inputs also give every word of the original's writable
data a value of its own (a landing pad: called, it returns, and which was called is seen), each
global it reads its own value, and objects whose words are zero and non-zero by turns. On calc,
shlwapi and setup32 this caught no correct body (every verdict that changed was checked). Two
shlwapi "faults" it first showed were the judge's own: on those inputs the original itself runs
into the step limit, and the rebuild, whose C loop takes more steps than the original's `rep
stosd`, was cut off sooner. Since 2026-10-28 (PIPE_REV 46) an original cut off at the limit is
matched by the calls it made by then, the rebuild given room to make them, and a rebuild that ends
where the original was still going must have run further than it (calc.exe unchanged; setup32 and
shlwapi two bodies each from unverified to weak). The judge costs about 7% more time. Teeth after: calc.exe 67% -> 76%, wordpad.exe 79% -> 80%, comctl32.dll 84% -> 85%.
Most of what still survives at full coverage is Microsoft C's exception-frame bookkeeping (the
`fs:[0]` registration, the try level, handler addresses): seen only when an exception is raised,
which no input does. The rest is mostly a constant whose +1 changes nothing the inputs show.

## Reach: a sample of every platform on this PC

`python -m codebreaker reach ../homebrew-samples ../Windows95/design/corpus/files C:/Nexia360/Games --judge`
-- 5,225 files; up to 5 programs of each platform judged, spread across their sizes (the smallest,
the largest under 4 MB, and between), each stopped after 5 minutes.

| Platform | Programs found | Judged | Functions | Verified | Weak | Unverified | Stopped |
|---|---|---|---|---|---|---|---|
| 16-bit Windows | 362 | 5 | 274 | 49% | 14% | 36% | 1 (a font file crashes the lane) |
| 32-bit Windows | 638 | 25 (below) | 4,749 | 50% | 29% | 14% (+6% not judged) | |
| Xbox 360 (per function) | 177 | 5 | 241 | 47% | 19% | 32% | 2 (time limit: wininet.xex, a 3.9 MB default.xex) |
| DOS programs | 132 | 5 | 601 | 100% | | | |
| Device drivers (VxD) | 224 | 5 | 15 | 73% | 20% | | |
| Game Boy / Color, Genesis, MSX, Master System, ZX Spectrum | 9 | 9 | 706 | 100% | | | |
| .NET / XNA | 48 | not judged per function (the `xna` lane builds and runs whole games) | | | | | |
| Big-endian ELF, machine 0x3D32 (Windows 95's d3d.uc, win31.uc) | 2 | no lane | | | | | |

Five programs a platform is a sample, not a census: it shows the shape. The full Windows 95 corpus
is measured by `coverage` and `sweep`; `reach --per-platform N` takes more.

What the sample says:

- **Hand-written assembly comes back whole.** DOS, the drivers' code and every 8-bit / 16-bit console
  program judged rebuild identically and verify.
- **Compiled code (C) is about half verified**, on 16-bit and 32-bit Windows and the Xbox 360 alike,
  and the larger the program the larger the unverified share (setup25i.exe: 63 verified, 88 not).
  This is the main body of work: the causes are recorded per function (`tools/causes.py 16|32`).
- **Large Xbox 360 modules do not finish per-function judging in 5 minutes.** Whole Xbox 360 titles
  are proven the other way, recompiled to C++ that runs on a PC (`recompile-project`): see below.

## Whole programs that run

The real software put through each lane, and what came of it, is listed in
`docs/TESTED_SOFTWARE.md`. In short:

- **Windows 95**: the 15 MINI.CAB modules rebuilt from their reconstructed source (KRNL386, GDI and
  USER with their C) boot together and take Setup to the License page (re-checked 2026-10-30 on
  af5ffb910, with the caller-results rule off and on: see the milestone below); with the 7 Setup
  programs, Setup has installed Windows to the desktop on them.
- **Xbox 360**: Tetris Splash, Minecraft (TU0), Up and PAC-MAN CE play recompiled; nine more titles
  run.
- **XNA**: Arcadecraft and Block World build and run as MonoGame projects, their shaders recovered
  as HLSL from Xbox microcode.
- **Java**: Minecraft Alpha's classes as C++03, built and running.
- **8-bit and 16-bit homebrew**: nine programs rebuild byte for byte and verify.

## 32-bit Windows, a wider sample (2026-10-25)

25 of the corpus's 638 32-bit programs, spread across sizes (5 KB to 2 MB: system DLLs, drivers,
fax, network and Java components, Setup), judged with every fix of that night:

| | Verified | Weak | Unverified | Not judged | Does not compile |
|---|---|---|---|---|---|
| 25 programs, 4,749 functions of their own | 50% | 29% | 14% | 6% | 1% |

What moved it (each a general cause, found on one program and checked on the rest):

- the C runtime of Visual C++ 4.0 identified, not reconstructed (its signatures learned);
- the zero-filled part of a section (.bss) described, so its globals keep their sizes;
- C++ methods keep their object in ECX: the calling-convention analysis no longer stops on a
  function of padding (msjava.dll 626 -> 280 unverified);
- imports with stdcall or C++ decorated names are linked instead of falling through to the next
  instruction (ftpsvc2.dll 149 -> 38);
- a register pushed at entry is an argument when the callers load it (awfaxp32.dll 89 -> 43);
- the judge: a rebuild that runs out of steps where the original returned is given room, and a
  stubbed API gives back handles and counts through its pointer parameters, the same each run;
- a flat thunk's stubs (the thunk compiler's 32-to-16-bit veneers) identified, called in the
  original and entered like imports, so the module's own functions that call them are judged
  (mnmcpi32.dll: not judged 297 -> 1, verified 81 -> 151);
- the judge's stand-in for a callback, or for an import of unknown argument count (a driver's
  NDIS.SYS calls), removes what its caller pushed: the original's frame pointer had hidden the
  difference, and the rebuild, keeping none, returned to nowhere (bbsnav.nav 37 such -> 8).

Largest causes left: calls through a pointer whose argument count the decompiler does not know
(a selected allocator: `(*pcVar)()`; saying the pushes before each such call -- CB_POINTER_CALLS=1
-- helped no program and made msjava.dll worse, so it is off), and what is left of the rebuild
calling into nothing: 61 returns with the stack off (most in msjava.dll: one pointer chosen
between two functions of different argument counts -- CoTaskMemAlloc(size) or
CoTaskMemRealloc(p, size) -- each path pushing its own before one shared call, which one C call
cannot say), 57 calls through a pointer the test leaves empty, 14 jumps through one.

## Milestone regression, 2026-10-30 (master af5ffb910)

Run in a worktree of af5ffb910 with a state folder of its own (the main checkout's knowledge base
copied in; caches empty).

**Test suites** (`tests/run_all.py`, slow suites included, 4:46 to 5:13 PM): 40 of 46 suites
passed. Of the six that failed, two were bugs, fixed in commits of their own, and three were the
worktree's environment; one is stale frozen references:

| Suite | Cause | Now |
|---|---|---|
| tools/tidy.py | six undefined names: the C back end's inline `_asm` block (19fcc15da) wrote to an `out` and `pad` its function does not have -- a NameError on any function with a Microsoft C `_asm` block | fixed (548605574); seven unused imports still listed (c_text.py 444/647/675/709, describe.py 1117, windows16.py 343/854) |
| test_service.py | the service's port, worked out from the state folder's path, fell in one of Windows' excluded port ranges (Hyper-V/WinNAT, 39583-39882 here) and all twenty ports tried were refused | fixed (fe03f3c84): any free port then; 11 of 11 |
| test_mcp.py (test_what_was_made) | environment: TEMP set inside the project, so the "file outside" it reads was inside | not a fault |
| test_invert.py (test_16bit_assembly_source) | environment: source/vxd/masm.py copies DOSXNT.EXE from `toolchains/` relative to the working directory, and a worktree has none | passes with it (15 of 15) |
| test_java.py (does_not_compile_says_why) | environment: the long TEMP path cut the compiler's message before the name the test looks for | not a fault |
| test_frozen_slow.py | describe.ne_keyboard (sub_9_04B4 now `__fastcall16far`) and c_text_module.lzexpand_dll (header's new `#define __fastcall16far __far`): both the intended output of 5d4d60281, whose frozen cases were not recorded again | to record again (`tools/freeze_tests.py subjects` / `c_text`) |

**The all-15 Setup boot**: all 15 MINI.CAB modules rebuilt from source and Setup booted on them
(`setup16.continue_test`'s boot, 150 polls for the Welcome page). PASS = the License page, each
first result looked at, each run twice:

| Build | USER C / gated | GDI C / gated | USER fixed segment 1 | Boot 1 | Boot 2 |
|---|---|---|---|---|---|
| caller-results rule off (default) | 509 / 61 | 118 / 24 | BAC4h | PASS | PASS |
| rule on (`CB_CALLER_RESULTS=1`) | 514 / 60 | 118 / 24 | BAC4h | PASS | PASS |
| rule on, fixed segments may grow (`CB_FIXED_GROWS=1`) | 597 / 57 | 131 / 13 | C642h | PASS | PASS |

(The original's segment 1 is BA2Eh.) With the rule on, Setup reaches the License page: the old
"There is not enough memory to run Setup" did not come back. That the fixed-segment cap is what
cured it is consistent but not proven by this run: without the cap USER's segment 1 grows only to
C642h on this build, under the C778h at which 14a68ca2f saw the failure (C63Ch still ran), so the
uncapped control passed too.

**The sweep**: `sweep --all` is far longer than one sitting (1,232 modules, about ten an hour once
past the smallest 32-bit ones). Measured: the 95 smallest 32-bit modules, and the 90 smallest of
each of the 16-bit modules, drivers and DOS programs (`sweep --16|--drivers|--dos --only`). Against
each module's latest earlier results (the main checkout's store), 363 modules compared:

| | Verified | Weak | Identified | Unverified | Not judged | Does not compile |
|---|---|---|---|---|---|---|
| before | 10,033 | 742 | 347 | 869 | 21 | 131 |
| 2026-10-30 | 10,688 | 714 | 587 | 803 | 21 | 90 |

DOS programs 90 of 90 verified (7,678 functions); drivers 227 verified, 23 weak, 1 unverified. 40
modules have fewer verified+weak or more unverified+no-compile; most of the 16-bit ones gained
judged functions on the whole. Losses worth a look:

- 16-bit: system.drv 11 -> 6 good (GET80X87SAVESIZE `*(xunknown2 *)0xd` is now
  `*((int2 __far *)MK_FP(xRam10000000, 0xd))`, which faults in the judge), mouse.drv 22 -> 17
  (two no longer compile: `(uint2)codebreaker_code_1.at_11a4` passed to KERNEL_50 -- E1010 type
  mismatch, the code-segment data names of 19c0c7cdd; four `ret` values differ), rasapi16.dll
  6 -> 2 (thunks 3:002a.. now fault), scandskw.exe 3 -> 1, umdm16.dll 5 -> 3.
- 32-bit: rpcltc5.dll 23 -> 20 and rpclts5.dll 29 -> 27 (one function each "not in the build"
  now; others exit-differs where the C is unchanged -- the judge's later rules), awadpr32.exe,
  awcapi32.dll, msrle32.dll, rundll32.exe one each.

Not measured this time: the other 1,000-odd 32-bit modules, the rest of the 16-bit modules,
drivers and DOS programs, and the per-function teeth.

## Gaps found by this run

- `serife.fon` (a 16-bit font resource file) crashes the 16-bit lane: `IndexError: pop from empty list`.
- Seven Xbox 360 packages in the library do not open: five STFS packages ("index out of range")
  and two `.xenon.package` files of a format not yet known.
- `vmm32.vxd` (the drivers bundled into one W3 file) is filed under DOS programs.
- Platforms with no lane yet (named by `reach` when it meets them): Game Boy Advance, Nintendo DS,
  Nintendo 64, PlayStation, Super Nintendo, ELF by CPU (MIPS, ARM, SuperH, Cell), Android.
  `docs/PLATFORMS.md` is the whole map and the order of work.

## Keeping these numbers current

`reach` keeps its results in `state/reach/<name>.json` by each program's content and the tools'
revision, and writes `state/reach/<name>.md`. Run it again after a change to the decompiler,
builder or verifier: a program measured with the same tools is not measured again.
