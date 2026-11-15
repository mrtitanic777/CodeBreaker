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

### 3. Per-function reasons are in (module_pack.cpp), and the class behind row 3

With the evidence recorded, the 44 small-module functions are (KEYBOARD, VGA, SYSTEM, SOUND, VER, LZEXPAND, KRNL386):
"no room" in a fixed or self-loading segment 23 (13 KRNL386 below the cut, 7 + 3 in fixed segments of SYSTEM, SOUND,
KEYBOARD), reads ES as entered 4, calls pass the caller's CS 3, reads a variable nothing sets 5, calls leave DS changed 3,
returns in the flags 1, C that does not compile 3 (vga DEVICEBITMAPBITS, ver GETFILERESOURCE, sub_3_1200). So the biggest class
is "no room".

Cause for the stubs in it (SOUND `SYNCALLVOICES`, 3 bytes `xor ax,ax / retf`): the original has no Windows prologue, but
OpenWatcom writes one for a far function (`push ds / pop ax / nop / push bp / mov bp,sp / push ds / mov ds,ax ...`, 14 bytes), so the C
does not fit its place; `-zW` leaves it out (measured: 3 bytes).

Fix (module_build16.cpp, `refit_without_prologue`, called from the fixed-segment packing in `decide_resident`): a function that
finds no room, whose original has no Windows far prologue (`windows_far_prologue`), is compiled once more with `-zW`; the
object is kept if smaller and the packing is tried again; else the function is given up as before.

Proof: `regress all15 --runs 2`: PASS, PASS (7m49s, screens state/phase3/shots/win95track-refit-all15-*.png).
SOUND C 2 -> 3 (`SYNCALLVOICES` now C, gate passes it); USER C 547 -> 549; nothing got worse. Setup's programs are all assembly
and are not touched by this (not rerun).

What the fix does not reach: the other stubs of this class (SOUND `WEP`, `SETSOUNDNOISE`, `SETVOICEENVELOPE`, SYSTEM `WEP`,
`A20_PROC`, KEYBOARD's three) take arguments, and OpenWatcom gives a `__stdcall` function with parameters a BP frame whatever
the flags (`push bp / mov bp,sp / mov ax,1 / pop bp / retf 2`, 10 bytes against the original's 6: `-zW -of- -ox` measured, no
change). Next step for them: write those unused-parameter stubs so the compiler makes no frame (a calling-convention pragma
for the stub, or the decompiler's stub text without the parameter names), then the same refit applies.

### 4. Row 2, read against the original (not converted)

- `BUGSTOP`: the C is `return int03_ax(param_1)`; the gate's "leaves 0000=0, 0012=1 in the data segment where the original
  leaves nothing" comes from the `int03_ax` helper (the interrupt-routine stand-ins the build adds), not from the function: it
  needs the helper's data writes kept out of the comparison, or the INT 3 written inline.
- `CPLAPPLET`: the C keeps SI, DI, DS where the original changes DI (it calls through a far pointer at DS:4A and the callee's
  register contract is not modelled); it needs that callee's register effects in the description.
- `DISABLE`, `POWEREVENTPROC`: as the work list says (call through DS:0018 answered by the gate with INQUIRE): a gate change,
  to leave out arguments of a pointer call whose callee has no signature; not done, because it weakens the gate and needs its own
  proof of teeth (`codebreaker teeth`).

### 5. Parameter stubs: not possible with OpenWatcom flags (measured, no change made)

Tried on `int __far __pascal B(int a1){ return 1; }` (original: `mov ax,1 / retf 2`, 6 bytes): with `-zW`, `-bt=dos`,
`-os/-ot/-ox/-oh/-oi/-ol`, `-0`/`-3`, no `-zu`, `__pascal`, `__stdcall`, `__cdecl`, an unnamed or `(void)`-used parameter, and
`#pragma aux ... parm routine / caller []` the code is always `push bp / mov bp,sp / mov ax,1 / pop bp / retf 2` (10 bytes);
near functions too. OpenWatcom builds a BP frame for any function with stack parameters; the only switches that change it
(`-od`, `-of+`) make it larger. Only a parameterless stub is frame-free (`xor ax,ax / retf`, what item 3 handles). So the ten stubs
that take parameters (SOUND WEP, SETSOUNDNOISE, SETVOICEENVELOPE; SYSTEM WEP, A20_PROC; KEYBOARD SCREENSWITCHENABLE,
ENABLEKBSYSREQ, INQUIRE; ...) need either a compiler change (an OpenWatcom option to omit the frame when no parameter is read:
the toolchain is ours to extend) or stay assembly. Their size in the original is exactly the frame-less form, so this is the
smallest well-defined compiler item on the list.

### 6. Recorded reasons for GDI and USER (after items 1 and 3), biggest class first

| class | GDI | USER | what it needs |
|---|---:|---:|---|
| no room in fixed segment 1 (the C is bigger than the original's code; the segment must keep the original's size, see the comment in decide_resident) | 2 (+12 gate-diff ones that also found no room: `GETDCSTATE`...) | 73 (+8 with `GETDESKTOPHWND`...) | smaller code from the compiler (the prologue trick of item 3 is tried first and does not reach these), or a measured budget for how far a fixed segment may grow: USER 1:BA2E grown to C63C still ran Setup, C778 did not, so there is room for roughly 1.5 KB, which `regress install` (not all15: it stops at the License page) would have to decide |
| reads a variable nothing sets (a register live in at entry: in_CX, extraout_DX, unaff_BP ...) | 10 | 43 | register live-in in the lifter (x86_lift.cc): the keystone item of the backlog plan; one fix converts the most functions of any single rule |
| calls a function that leaves DS changed | 3 | 20 | the callee's DS effect in the description, so the C reloads DS after the call |
| passes AX/CL to a register-argument helper | 10 | 25 (16+5+4) | the helper's register convention as a `__fastcall`-style pragma in the header |
| reads ES as entered | 4 | 8 | ES as a parameter in the description |
| gate diff (calls differ) | 1 | 12 | per function, as for DISABLE below |
| returns DX:AX where the C returns a word | 0 | 5 | result width from the callers |
| caller's CS passed | 1 | 5 | the far-call argument read |

Totals now: GDI 53 and USER 250 functions of origin C are still kept as instructions (GDI 18 and USER 42 of them by the gate).

### 7. Caller results on by default (describe.cpp, `Policy::caller_results`)

The "reads a variable nothing sets" class is not, in the cases read (GDI sub_1_5FC1 ... in_DX), a register live-in: it is
the second word (DX) of a callee's DX:AX result that the callee's description gives as a word. The decompiler already has the
rule (`results()`: a callee whose DX its callers read, and that hands back a DX of its own on every way, is four bytes), off by
default since USER's C once stopped Setup ("not enough memory": since explained by the fixed-segment size, and kept by the size
rule). (There is no x86_lift.cc in the tree: the lifter is the Ghidra-derived decompiler core plus describe_ne_facts.cpp.)
Turned on (`CB_CALLER_RESULTS=0` turns it off).

Proof: `regress all15 --runs 2` with it on: PASS, PASS (7m04s): USER C 549 -> 553, VER C 23 -> 24. Installed exe, default
policy: `regress install --runs 1`: **PASS**, 1h13m, Setup to the desktop on all 22 modules (screen
state/phase3/shots/win95track-install-install-1.png). GDI is unchanged (its in_DX cases are a shared-body entry,
sub_1_1260/1268 `mov cx,..; mov dx,..` into one body, which "every way" does not accept: next).

### 8. OpenWatcom: no BP frame for a routine whose stack parameters are never read

The source is beside the rel tree (experiments/premise-proof/open-watcom-v2/bld, built in place by its own makefiles).
Cause: `NeedBPProlog()` (bld/cg/intel/c/x86proc.c) answered "yes" whenever `parms.size != 0`.
Change: `ScanInstructions()` (which already walks every instruction of the routine before the prologue) records whether any
instruction other than `OP_PARM_DEF` names a `STACK_PARM` temp (as operand, result, or the base/index of an indexed operand);
`NeedBPProlog()` asks for BP for parameters only when one is referenced. Windows far prologues (`CHAIN_FRAME`), locals,
debug info, address-taken parameters keep their frame as before.

Build (no Python, nothing else touched): `cmd` with `OWROOT=<tree>`, `OWTOOLS=VISUALC`, vcvars64, `cmnvars.bat`, then `wmake` in
`bld/cg/intel/i86/ntx64.dll` (cgi86.lib) and `bld/cc/i86/ntx64.dll` (wccd.dll, wcc.exe). The result was put in a copy of rel
(CB_OPENWATCOM) so the shared rel was not changed while other runs were using it.

Measured, `-bt=windows -mm -zq -w0 -s -os -nc=MCODE`:

| function | before | after |
|---|---|---|
| `int __far __pascal B(int a1){return 1;}` with `-zW` | `55 89 E5 B8 01 00 5D CA 02 00` (10) | `B8 01 00 CA 02 00` (6, the original's) |
| `return a1;` | frame | frame (unchanged) |
| `return g(&a1);` | frame | frame (unchanged) |

Proof: `CB_OPENWATCOM=<copy> codebreaker regress all15 --runs 2`: PASS, PASS. SOUND C 3 -> 6, KRNL386 18 -> 20, USER 553 -> 556
(gated 42 -> 41). `regress install` with the rebuilt compiler was started and stopped by me at 12:42 AM: it sat for 20 minutes "waiting for a free slot - decompile" (other jobs held the slots) and never reached a boot, so **the rebuilt compiler is not install-proven and rel/ is unchanged**. To install it: `cp bld/cc/i86/ntx64.dll/binbuild/{wccd.dll,wcc.exe}` over rel/binnt64 after an install passes with `CB_OPENWATCOM` set to a copy of rel holding them. The ten parameter stubs: three of SOUND fit and are C now (WEP, SETSOUNDNOISE, SETVOICEENVELOPE); the other
stubs (SYSTEM WEP/A20_PROC, KEYBOARD's three) did not change: see "Next".

Open (found at the end, not solved): SYSTEM `WEP` and `A20_PROC` and KEYBOARD's `SCREENSWITCHENABLE`, `ENABLEKBSYSREQ`,
`INQUIRE` still say "finds no room in fixed segment" although their objects are now the original's size (SYSTEM f0004.obj
`B8 02 00 CA 02 00`, f0005.obj `B8 01 00 CA 02 00`: 6 bytes each, in holes of 6). SOUND's identical stubs were accepted, so the
packing in `decide_resident` (pack_holes, run once per fixed segment) gives them up for another reason in these two modules;
next: print the holes and sizes it sees for SYSTEM segment 1.

### 9. Why the install "waited for a free slot - decompile" (not stuck)

`Slots` (base/parallel.cpp) are lock files state/slots/slot-0..3: four decompile slots for every session on the machine. A
decompile of more than one chunk of functions asks for slots, waits up to 600 s for one, then runs on one regardless. After an
exe change (describe.cpp's policy, other sessions' builds) the decompile of the big modules is a cache miss; with another
session's decompiles holding the four slots the install's builds queued, in 10-minute waits, behind them. No deadlock, no
leak: the slot files are re-taken by whoever runs. Retrying is right; to shorten it, start the install when no other decompile
runs. (The ~5 GB codebreaker.exe was a decompile of USER/NETDI chunks, not a stuck process.)

### 10. The packing pad (module_pack.cpp, module.hpp `byte_aligned_code`, module_build16.cpp)

SYSTEM `WEP` and `A20_PROC` (6-byte objects in 6-byte holes) were refused because `pack_holes` put one pad byte before an
object whose hole starts at an odd offset ("OpenWatcom aligns its code to a word"). The objects of this build are compiled
`-os`, and OpenWatcom then gives their code segment byte alignment (SEGDEF attribute 28h, A=1, measured in f0005.obj), so no pad
is wanted. `pack_holes` takes `word_aligned` (default true: its tests are unchanged); the 16-bit build passes false.

Proof (the rebuilt compiler of item 8 and this build; unit tests `decompile.describe` 19/19 and `build.module` 19/19 pass):
`regress all15 --runs 2`: PASS, PASS. KRNL386 C 18 -> 21, GDI 123 -> 130 (gated 23 -> 16), USER 542 -> 561, SOUND 2 -> 6,
SYSTEM 0 -> 2, VER 23 -> 24; since the start of the session +37 functions in C, none lost.
`regress install --runs 1` with the fork's rebuilt compiler (CB_OPENWATCOM = a copy of rel with its wccd.dll and wcc.exe; the installed
exe, so without the pad change): **PASS**, 1h05m (28m for the install itself), Setup to the desktop on all 22 modules (USER C 556,
GDI 128, SOUND 6, KRNL386 20), screen state/phase3/shots/win95track-install-ow2-install-1.png. So the compiler change is
install-proven and rel/binnt64 may take its wccd.dll and wcc.exe. The pad change was then proven by `regress install --runs 1` too (a private copy of the shared build's exe with the change and
the installed compiler; `win95track-install-pad`): **PASS**, 1h26m, Setup to the desktop on all 22 modules (USER C 563 / gated 41,
GDI 130 / 16, SYSTEM 2, SOUND 6); screen state/phase3/shots/win95track-install-pad-install-1.png.

### 11. The compiler is in the real toolchain

rel/binnt64/wccd.dll and wcc.exe replaced by the fork's builds (the ones the install above passed with; 02:09 AM, Nov 15), the
originals kept in rel/binnt64/backup-20261115/. Built from the fork commit `7a2406dc` (branch `codebreaker` of
github.com/mrtitanic777/open-watcom-v2, bld/cg/intel/c/x86proc.c: the frame is omitted for a routine whose stack parameters no
instruction names), with the VISUALC host and `wmake` in bld/cg/intel/i86/ntx64.dll and bld/cc/i86/ntx64.dll (see item 8).

### 12. "Reads a variable nothing sets", the biggest group, read (USER 43, GDI 10): what it is and where it belongs

Counted by the variable (USER / GDI): `in_AX` 15 / 0, `in_DX` 3+1 / 6, `extraout_DX|AX|AH` 8 / 0, `unaff_BP|SI|DI` 4 / 3,
stack temporaries 8 / 1, the rest singles.

- GDI `in_DX` (6): a second result word of a callee whose body is shared by several entries (`sub_1_1260` / `sub_1_1268`:
  `mov cx,..; mov dx,..; jmp` into one body that returns DX:AX). The `results()` rule (item 7) does not take a callee whose DX is set
  on only some ways; here DX is set on every way but the entry code of the *other* entry sets it before the jump into the body, which
  `DxReturns::every_way` does not follow across the jump. **Tried and found not to be the cause**: resolving a jump into another
  function's middle to that function (`handed_on` in `DxReturns::every_way`) changed nothing in GDI (C 130, never-set 10, same
  as before), and the change was taken out again. The body of 1:1268 is not a plain function: it leaves through `jmp 0x52D6` /
  `jmp 0x5281` into other functions' code and calls a helper through `push cs / call near`; it needs reading by hand (or the
  next step: which of its ways `every_way` refuses, printed), not a rule.
- USER `in_AX` (15), sample `sub_1_441D`: `if (flag) { for (p = a2->next; p; p = p->next) if (p->f_1a || p->f_25 & 0x10) return in_AX; }
  return sub_1_444C(a1, a2);`. On the early return the original leaves AX as it came in: the source function has a path with no value
  (the C author's `return;` in an `int` function), so the callers cannot depend on it, and the C here reads the register it never
  set. Not a live-in parameter in the `__fastcall` sense (`register_arguments` reads only the straight-line start): it is
  "AX unspecified on this path". Where it belongs: the C back end (c_text), the value of a read of an entry register that only feeds
  a `return` is the result left unspecified, to be written as the `AX` the function last computed (or 0) -- then the gate
  decides, per function, whether callers see a difference. Not done: it needs the gate run on the 15 and a choice of what to return.
- `extraout_*` (8 USER): the same as GDI's `in_DX` at call sites that read the second result of a callee the description gives one word.
- `unaff_BP/SI/DI` (7): frame registers read without being set (a function that works in its caller's frame or keeps
  SI/DI for its caller): `origin` calls most of these assembly already; they stay assembly.

### 13. USER `in_AX`: the unspecified result written as 0 (module_build16.cpp `unspecified_entry_result`)

As decided: where the only uses of `in_AX` in a function's text are its declaration and `return in_AX;` paths (never set, never read
otherwise), each such return is written `return 0; /* the original left AX as it came in: no caller reads it */` and the
declaration is dropped; no parameter is invented. Done in `compile_functions`, before the text is compiled, so the compiler, the
never-set check and the gate all see the final text; the gate then decides per function (a function whose verdict changed
goes back to assembly by the gate's own rule).

Proof: direct `build` of USER: never-set 43 -> 28 (15 converted from "kept": `in_AX` all went). `regress all15 --runs 2`:
PASS, PASS (8m20s): USER C 563 -> 569 / gated 41 (the gate sent back the ones it found different; the net gain is 6),
every other module unchanged. (A first all15 run of the same exe failed with USER "NOT LINKED ... No such file": a
collision of two builds in the shared folder state/build/user.exe, mine and another session's, and passed on the rerun;
builds into state/build/<module> are not isolated between sessions.) `regress install` started with the same exe: result below.

### 14. USER's 20 "calls a function leaving DS changed" (also GDI 3, KEYBOARD 1): read, not converted

The rule is `scan` in build/module_scan.cpp (a caller of any function whose instructions load DS and do not put it back is kept as
its instructions: KERNEL's sub_2_2437 `push ds; call 0170 ... pop ds` faulted with the C, which kept the callee's DS). It is a
blanket: it does not look at what the caller does after the call. A caller whose original restores DS itself (`pop ds`, or `mov ds, ..`
before any DS access) is correct as C if the C reloads DS after the call; one that goes on with the callee's DS cannot be written
in C at all. So the rule needs two halves: (1) classify each such call site from the caller's instructions after it (restored
before the next DS use / used with the new DS); (2) for the restored sites the C gets a DS reload after the call (a
`codebreaker_ds()`-style statement the build already has for `__loadds` functions). Not done: (2) changes the C of every caller of
such a callee, and each needs the gate.

Not converted (the GDI `in_DX` shared-body entries; they need `DxReturns` read by hand, see item 12):


Next (item 3 of the lead's list, not started): GDI's remaining recorded reasons (16 gated, 10 never-set shared-body entries,
6 CL-to-helper) and USER's (73 no-room, 43 never-set, 20 DS-leaving callees); and KEYBOARD's three stubs (27 to 63 bytes:
`SCREENSWITCHENABLE`, `ENABLEKBSYSREQ`, `INQUIRE`), which still find no room.

### Where this stops

Converted and proved this session: `WEP` (item 1, +11 functions in all) and the prologue refit (item 3, +3). `DISABLE` and
`POWEREVENTPROC`: the gate was not changed, because `codebreaker teeth` was not run on a changed gate. Next, by size of
class: register live-in in the lifter (43 + 10 + 5 here, plus the 5 small-module ones), then the fixed-segment size budget
(73 + 2 + 10 small), then DS-leaving callees (23).
