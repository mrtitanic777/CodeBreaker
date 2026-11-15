# Windows 95 in authored source: the completion audit (ROADMAP track 1)

Audit of 2026-11-14 (phase 1), with the conversion log (phase 2) at the end. Every number below is from the tool's own
output; the commands are listed so the table can be made again.

## How the numbers were made

```
codebreaker regress all15 --runs 1                 # PASS in 3m21s; prints, per module: functions / C / asm / stand-ins / gated
codebreaker build state/mini/original/<M> --no-run # the build report of each of the 15 (gated lines, stand-ins, unresolved)
CB_ASM_KEEP_C=codebreaker_none CB_NO_GATE=1 \
codebreaker build state/setup/original/<M> --no-run # the 7 Setup programs, as `regress setup` builds them (all assembly, no gate)
codebreaker origin <M>                             # which functions were written in assembly (analyze/origin)
state/build/<m>/module.l2.json                     # the build's own per-function record: a C function, or a MachineFragment
                                                   #   with its reason ("written in assembly: ..." / "its C does not compile: ...")
```

The three sets of the roadmap are two sets of modules: **all15** (the 15 MINI.CAB modules), **setup7** (Setup's seven 16-bit
programs), and **install** = all15 + setup7 = 22 modules (`regress install` builds exactly these two lists, see
`all15_names()` / `setup_names()` in native/cb/verify/regress.cpp).

Columns (from `Built`, native/cb/build/module.hpp, and `note_assembled`, module_pack.cpp):

| column | meaning | source |
|---|---|---|
| functions | functions in the module | build report |
| authored C | built from decompiled C and trusted: in the image as C | regress "C", = L2 units that are not fragments |
| hand asm | written in assembly by the original's author (origin says so): stays assembly, correct | L2 fragments "written in assembly" |
| C-origin kept as asm | origin says C (compiled), but the image has its instructions: C not trusted | L2 fragments "its C ..." |
| of which gated | the gate ran the C beside the original and it differed | build report "gated N" (a subset of the previous column) |
| stand-ins | compiler rejected the C, or called but never reconstructed: a function that returns 0 | build report |

"Compiled-but-still-assembly" is the column "C-origin kept as asm" (gated included). Stand-ins are **0 in every module**.

## Headline

| set | functions | authored C | hand asm (correct as assembly) | C-origin kept as asm | of which gated | stand-ins |
|---|---:|---:|---:|---:|---:|---:|
| all15 | 3771 | 753 (20%) | 2651 (70%) | 367 (10%) | 72 | 0 |
| setup7 | 4956 | 0 | 1782 (36%) | 3174 (64%) | n/a (no gate) | 0 |
| install (22) | 8727 | 753 (9%) | 4433 (51%) | 3541 (41%) | 72 | 0 |

So the work that is left is the 3541 functions that were compiled code and are shipped as instructions: 367 in the 15
boot modules (72 of them by the gate's verdict) and 3174 in Setup's programs (every C version of which crashes, so all are
kept as assembly on purpose). Today's numbers differ from the ones quoted for the milestone (STATUS.md: USER C 509 / gated 61,
GDI C 118 / 24): the run reports USER C 542 / gated 43 and GDI C 123 / gated 23 and still boots (see "Baseline").

Counting caveat: `origin` counts 1 to 10 functions per module differently from the build (it finds 882 functions in GDI,
the build 883; origin says 694 assembly, the build's L2 has 702 "written in assembly"); totals above use the build's L2.
NETDI.DLL's row is from its L2 of 12:38 today (a fresh `build` of it did not finish in 10 minutes on this machine, see
the work list).

## Per module

all15 (`regress all15`; its own build lines in brackets are the regress "C / asm / gated" columns)

| module | functions | authored C | hand asm | C-origin kept as asm | gated | stand-ins | regress line |
|---|---:|---:|---:|---:|---:|---:|---|
| KRNL386.EXE | 679 | 18 | 639 | 22 | 0 | 0 | C 18 asm 661 |
| GDI.EXE | 883 | 123 | 702 | 58 | 23 | 0 | C 123 asm 760 gated 23 |
| USER.EXE | 1613 | 542 | 814 | 257 | 43 | 0 | C 542 asm 1071 gated 43 |
| KEYBOARD.DRV | 34 | 3 | 26 | 5 | 0 | 0 | C 3 asm 31 |
| VGA.DRV | 197 | 3 | 191 | 3 | 0 | 0 | C 3 asm 194 |
| SYSTEM.DRV | 13 | 0 | 11 | 2 | 0 | 0 | C 0 asm 13 |
| MINIKBD.DLL | 2 | 0 | 2 | 0 | 0 | 0 | C 0 asm 2 |
| MSMOUS31.DRV | 50 | 2 | 46 | 2 | 2 | 0 | C 2 asm 48 gated 2 |
| NOMOUSE.DRV | 4 | 0 | 4 | 0 | 0 | 0 | C 0 asm 4 |
| LMOUSE31.DRV | 59 | 2 | 52 | 5 | 3 | 0 | C 2 asm 57 gated 3 |
| SOUND.DRV | 37 | 2 | 29 | 6 | 0 | 0 | C 2 asm 35 |
| COMM.DRV | 84 | 2 | 82 | 0 | 0 | 0 | C 2 asm 82 |
| VER.DLL | 31 | 23 | 3 | 5 | 0 | 0 | C 23 asm 8 |
| LZEXPAND.DLL | 46 | 32 | 13 | 1 | 0 | 0 | C 32 asm 14 |
| WIN87EM.DLL | 39 | 1 | 37 | 1 | 1 | 0 | C 1 asm 38 gated 1 |

setup7 (`regress setup` builds them all-assembly: CB_ASM_KEEP_C names no function, CB_NO_GATE=1)

| module | functions | authored C | hand asm | C-origin kept as asm | stand-ins |
|---|---:|---:|---:|---:|---:|
| SUWIN.EXE | 779 | 0 | 130 | 649 | 0 |
| SETUPX.DLL | 1466 | 0 | 299 | 1167 | 0 |
| SYSDETMG.DLL | 1170 | 0 | 331 | 839 | 0 |
| NETDI.DLL | 1476 | 0 | 1000 | 476 | 0 |
| SUEXPAND.DLL | 46 | 0 | 13 | 33 | 0 |
| W95INF16.DLL | 12 | 0 | 5 | 7 | 0 |
| SETUP.EXE | 7 | 0 | 4 | 3 | 0 |

The gated functions the build report names (it prints the first 12 of each module):

- WIN87EM: `WEP` ("removes argument bytes: original 6, ours 4")
- MSMOUS31: `DISABLE`, `POWEREVENTPROC` ("calls: original INQUIRE(0,DS), ours INQUIRE(0,fff0)")
- LMOUSE31: `BUGSTOP` (leaves 0/1 in the data segment where the original writes nothing), `CPLAPPLET` (original changes DI, ours keeps it),
  `DISABLE` (as MSMOUS31's)
- GDI 23 and USER 43: see the module's build report (`codebreaker build state/mini/original/GDI.EXE`); 12 are printed.

## Why C-origin functions are kept as assembly (decide_forced, module_build16.cpp)

A function that origin calls compiled is rebuilt from its instructions when: its segment is 32-bit (GDI's 36 and 38, flat
thunks: use32 assembly written by the thunk compiler: correct as assembly); its C leaves no result in AX where the
instructions do; its decompiled calls pass the caller's CS; it reads ES as entered; a call passes fewer/more words than the
callee removes; it reads a variable nothing sets; its address is in a near-pointer table in the data segment; it loads AX
for a call through a pointer; the gate showed its C differs (gated); data/kept_as_asm.txt lists it (a whole-module run showed
it wrong: 14 SETUPX functions, found by bisection); or (setup7) the module is built all-assembly. The L2 record said only
"its C does not compile" for all of these; **the build now records the evidence** (module_pack.cpp `note_assembled`: "its C is
not used: <evidence>"), so the per-function reason is in module.l2.json after the next build (see the log).

## Ranked work list (smallest, most certain first)

| # | item | functions | what it needs | certainty |
|---|---|---:|---|---|
| 1 | WIN87EM `WEP` | 1 | `register_arguments` (decompile/describe_ne_facts.cpp) takes WEP's `push ax` (a save, popped back before `retf 2`) for a register argument, so WEP is described `__fastcall16far` (a plain far, callee removes nothing) instead of pascal with one word. A save that is popped back is not a read. One rule + test. | high |
| 2 | LMOUSE31 `CPLAPPLET`, `BUGSTOP`; MSMOUS31/LMOUSE31 `DISABLE`, `POWEREVENTPROC` | 5 | DISABLE and POWEREVENTPROC call through DS:0018 (`call word ptr [18h]`); the gate answers that pointer with INQUIRE and compares the stack words INQUIRE then reads, which are the saved registers of each side's own frame: a harness artefact for a callee that takes nothing. Either the gate drops arguments of pointer calls whose callee has no signature (a gate change: it must keep its teeth), or the C is written as the original's frame. BUGSTOP and CPLAPPLET are real (register contract, data writes): read each against the original. | medium |
| 3 | the other C-origin-as-asm of the small modules (KEYBOARD 5, VGA 3, SYSTEM 2, SOUND 6, VER 5, LZEXPAND 1, KRNL386 22) = 44 | 44 | the recorded evidence per function (this audit's tool change) says which rule kept each; most are "reads a variable nothing sets" or "result in AX": decompiler-side fixes, each proved by `regress all15` | medium |
| 4 | GDI 23 gated + 35 other, then USER 43 gated + 214 other | 315 | same, plus the gate's verdicts; the largest class by far is one decompiler pattern fixed once for many | medium |
| 5 | Setup's seven programs, smallest first: SETUP.EXE 3, W95INF16 7, SUEXPAND 33, SUWIN 649, SYSDETMG 839, SETUPX 1167 (14 bisected, cause unknown: kept_as_asm.txt), NETDI 476 | 3174 | find why their C crashes, class by class (the same rules as 3, run on these modules first); prove with `regress setup`. NETDI's fresh build did not finish in 10 minutes: its decompile or build needs a look before it can be audited again | low until the classes are known |
| 6 | the 4433 hand-written assembly functions | 4433 | nothing: assembly is the right source for them (rule: hand-written assembly stays assembly). Their reconstruction as cMacros source is `ne-asm`'s job | n/a |

Finish line (roadmap): `regress install` on every module in authored form. 3541 functions (rows 1 to 5) lie between here
and there.

## Baseline

`codebreaker regress all15 --runs 1 --tag win95track-base`: PASS in 3m21s (1 run; the regression's rule is two), the screen
`state/phase3/shots/win95track-base-all15-1.png`. State at the start of phase 2.

## Conversion log (phase 2)

### 1. `register_arguments`: a `push` popped back is a save (describe_ne_facts.cpp, test in describe_test.cpp)

Cause (WIN87EM `WEP`): `push ds / push ax / push si / push di ... pop di / pop si / pop ax / pop ds / retf 2` is a pascal far
function with one word argument that saves AX; `register_arguments` read the save as an argument in AX, so the description
said `__fastcall16far` (callee removes nothing) and the gate saw "removes argument bytes: original 6, ours 4". Fix: a `push R`
of a register that the function pops back before its return reads nothing. Unit test added beside the xor-ax case
(`CB_CHECK(!register_arguments(1E 50 56 57 5F 5E 58 1F CA 02 00))`).

Proof (the installed codebreaker.exe of 8:15 PM contains the patch; the lead's fs.cpp fix let another session's build install it;
my own rebuild at 8:0x PM failed only at link on other sessions' symbols in translate and in `fs::write_file`, so the unit test
was added but **not run by me**):

| | before (baseline) | after |
|---|---|---|
| WIN87EM | C 1 / gated 1 (WEP asm) | C 1 + WEP in C: gate passes it, gated 0 |
| GDI | C 123 / gated 23 | C 128 / gated 18 |
| USER | C 542 / gated 43 | C 547 / gated 42 |
| all other modules | unchanged | unchanged |

The same rule fixed 5 GDI functions and 1 USER function as well, each passing the gate (so 11 functions fewer
kept as instructions: gated 72 -> 66 in all15, C 753 -> 764).
`codebreaker regress all15 --runs 2`: PASS, PASS (8m14s). `codebreaker regress setup --runs 1`: PASS (1h11m, 22 modules, Setup's
seven all assembly). Screens: state/phase3/shots/win95track-wep-*.png.

### 2. Per-function reason for kept-as-assembly functions (module_pack.cpp `note_assembled`)

Written, not yet run: the installed exe of 8:15 PM may predate it. Check with `codebreaker build state/mini/original/WIN87EM.DLL`
and read `state/build/<m>/module.l2.json`: a function kept without a compiler rejection says "its C is not used: <evidence>".

### Where this stops

Rows 2 and 3 of the work list are not started: they need row 2's gate decision (DISABLE / POWEREVENTPROC) and the per-function
reasons of item 2 above, which need one more build of the tree. Next: build, read the reasons for the 44 small-module
functions, fix the largest class once.
