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
builds into state/build/<module> are not isolated between sessions.) `regress install --runs 1` with the same exe (`win95track-install-inax`): **PASS**, 1h30m, Setup to the desktop on all 22 modules, USER C 569 / gated 41; screen state/phase3/shots/win95track-install-inax-install-1.png.

### 14. USER's 20 "calls a function leaving DS changed" (also GDI 3, KEYBOARD 1): read, not converted

The rule is `scan` in build/module_scan.cpp (a caller of any function whose instructions load DS and do not put it back is kept as
its instructions: KERNEL's sub_2_2437 `push ds; call 0170 ... pop ds` faulted with the C, which kept the callee's DS). It is a
blanket: it does not look at what the caller does after the call. A caller whose original restores DS itself (`pop ds`, or `mov ds, ..`
before any DS access) is correct as C if the C reloads DS after the call; one that goes on with the callee's DS cannot be written
in C at all. So the rule needs two halves: (1) classify each such call site from the caller's instructions after it (restored
before the next DS use / used with the new DS); (2) for the restored sites the C gets a DS reload after the call (a
`codebreaker_ds()`-style statement the build already has for `__loadds` functions). Not done: (2) changes the C of every caller of
such a callee, and each needs the gate.

### 15. The DS classifier (module_scan.cpp): on by default; two callers kept as assembly

Built in build/module_scan.cpp, tested in build/module_test.cpp (21 tests pass): `leaves_ds(ins, own_data_loads)` (a `mov ds, ax` whose AX is a
`mov ax, imm` the loader fixes to the module's own automatic data segment changes nothing: the fix-ups are found from the segment's
relocation records, following the chain of a non-additive one through the stored words) and `loads_own_ds_before(ins, k, own_data_loads)`
(the caller loaded the same segment itself before the call at instruction k). A caller of a callee that leaves only that segment (USER's
sub_1_5D4F: 20 callers) is no longer forced to assembly when it had DGROUP loaded before the call.

First tried on all of them: USER C 569 -> 576 and `regress all15` FAILED (`FAIL(dos)`, twice); the same exe with the 13 callers forced back
to assembly passed. Bisection by `CB_ASM_ONLY` subsets is not reliable (forcing a subset moves other functions and fails differently), so the rule
was put behind a switch (`CB_DS_OWN_CALLERS=<one name>`, default off) and run one function at a time, all else as in the passing state
(one `regress all15 --runs 1` each; two earlier series ran on a stale exe copy and were discarded):

| caller | all15 | USER C |
|---|---|---:|
| sub_1_6CA0 | PASS | 571 |
| sub_6_028C | PASS | 570 |
| sub_15_147C | PASS | 570 |
| sub_15_187A | PASS | 570 |
| **BEAR306** | **FAIL(dos)** | 570 |
| **sub_25_0429** | **FAIL(error box before Welcome)** | 570 |

So the DS reading is right for four and the two others break for a reason of their own (their C, unlocked, is wrong in a way the gate
does not see; not found). They are listed in data/build/kept_as_asm.txt (the whole-run-showed-wrong list). The rule is then on by default
(`CB_NO_DS_OWN=1` turns it off). `regress all15 --runs 2` with the switch on for all 13 callers and the two kept: PASS, PASS, USER C 574 / gated 40;
default (no switch), rebuilt exe: PASS, PASS (5m23s), USER C 574 / gated 40 (+5 over 569), GDI unchanged. `regress install --runs 1` of this exe (`win95track-install-ds`): **PASS**, 1h27m, Setup to the desktop on all 22 modules; screen state/phase3/shots/win95track-install-ds-install-1.png.

Also noted: regress has no option for a private state folder (its builds go to state/build/<module> and state/work/regress), so concurrent runs
by two sessions can collide; run them one at a time.

### 16. The GDI shared-body `in_DX` entries (1:1260 / 1:1268): read by hand, still refused, switch left default off

`DxReturns::every_way` now has two default-off extensions (`Policy::dx_shared_bodies`, `CB_DX_SHARED=1`; unit tests of the policy pass): it follows a
`jmp` out of the function's range into the code it lands in (1:1268 jumps to 1:52D6 and 1:5281, which `xchg` DX and return DX:AX), and a call to a
near function that never writes DX (1:522D) is transparent. With `CB_DX_SHARED=1` GDI's build did not change (C 130, 10 never-set): by
hand, 1:1268 has a path that sets no DX of its own -- `mov ax, cx / push bx / push ax / call far 1:..24FD (sub_20_24FD) / pop bx / cmp .. / jmp L_12CA /
pop di / pop ds / leave / retf 6`: the far callee returns a word, so DX is whatever it left. That is an error path whose DX no caller can
depend on (a caller stores it through `lpPoint` only when AX is non-zero). So `every_way`'s "DX set on every way" is the right strictness for
a *defined* DX:AX; what these entries need is the same decision as the `in_AX` one (item 13): a four-byte result whose DX is *unspecified* on
the paths that set none, written as 0 by the back end, the gate then deciding per function. Not done: it widens `caller_results`
(which once stopped Setup) and needs the all15 and install proofs as that one had.

### 17. The unspecified-DX rule for GDI (default off: `CB_DX_SHARED=1 CB_DX_UNSPECIFIED=1`)

Three pieces, all behind the two switches (`Policy::dx_shared_bodies`, `Policy::dx_unspecified`; defaults unit-tested):

1. `DxReturns::every_way` follows a `jmp` that leaves a function into the code it lands in, and treats a call to a near function that never writes
   DX as transparent (item 16).
2. `DxReturns::every_way(.., any_way)`: a callee whose callers read DX is four bytes when at least one way sets DX, the other ways leaving it
   unspecified (the same decision as `in_AX`, item 13).
3. `callers_read_dx` goes through a conditional jump, on the way that falls on, when looking for the caller's read of DX (GDI's 1:5FC1:
   `call 1268 / cmp / je / les / mov es:[bx], ax / mov es:[bx+2], dx` reads it on one way only).

Result with both on: GDI C 130 -> 132, never-set 10 -> 8; `regress all15 --runs 2`: PASS, PASS (USER 574 / gated 40, GDI 132 / 16, others
unchanged). `regress install --runs 1` with both on (`win95track-install-dx`, a private exe copy): **PASS**, 1h52m (34 min for the install itself; the rest waiting for shared decompile slots), Setup to the desktop on all 22 modules, GDI C 132 / gated 16, USER C 574 / gated 40; screen state/phase3/shots/win95track-install-dx-install-1.png. Proven; the switches are still default off in the tree until the next edit window (two one-line defaults in describe.cpp and the policy test).
The description of sub_1_1268 itself still says a two-byte result (it is the callee's own: rebuilt from instructions); what changed is the
callers that read its DX.

### 18. BEAR306 and sub_25_0429: what the gate does not see (read, not fixed)

Both are USER window procedures (`a4` = message, a switch of ~30 cases). Each passes the gate and each breaks the boot once it is C.
The gate (verify/gate16, isolated16) runs the function on its own beside the original and answers every call out alike on both sides: the callees
are stand-ins. Two things follow that a window procedure falls through:

- A callee's *side effect through a pointer argument* is not run. BEAR306's WM_PAINT case passes the address of a local array to
  `sub_1_0110` (BeginPaint) and `sub_1_76C3` (EndPaint) -- `(int2)aiStack_26, unaff_SS` -- and reads fields of what they fill; with the callees stubbed the
  filled buffer is never there, so C and original compare equal on an input that does not exercise the real thing.
- Which locals sit where. The decompiler turned BEAR306's frame into one array `aiStack_26[18]` with the pointer local at `(uint1 *)aiStack_26 + 0x20`
  (a 32-byte PAINTSTRUCT then a word): C that names a local through another local's address depends on the compiler laying the frame out as
  the original did, which OpenWatcom does not promise.

The change that would close the gate itself, not the two functions: run the callees that take a pointer to the function's own frame as
their own model (a table of the Windows calls that fill a structure: BeginPaint, GetClientRect, GetWindowRect, GetMessage, ... which are known from
the SDK headers) so the buffer is filled on both sides; and refuse (keep as instructions) any C that addresses a local at an offset from another local's
address past that local's own size. Neither is built; the two functions stay in data/build/kept_as_asm.txt.

### 19. A model for the Windows calls that fill a structure (built, default off: `CB_FILL=1`)

`data/knowledge/models/win16.json` gains a `fill` for 13 calls (BEGINPAINT 32 bytes, GETCLIENTRECT / GETWINDOWRECT / GETUPDATERECT / GETCLIPBOX 8,
GETCURSORPOS / GETCARETPOS 4, GETMESSAGE / PEEKMESSAGE 18, GETWINDOWPLACEMENT 22, GETTEXTMETRICS 31, GETKEYBOARDSTATE 256, GETSCROLLRANGE two words):
`"fill": "<word>:<bytes>[,...]"`, the far pointer being the arguments' words `<word>` (segment) and `<word>+1` (offset) as pushed. `StubModel::fills`
(emulate/windows16.hpp), read by `models_of`; `Runner::probe` writes the same fixed bytes (0x40 + 5i mod 0x3B, never zero) through that pointer on
both sides of a comparison when `CB_FILL=1`. A call that is only a fill still answers as an unmodelled one (the path explorer keeps varying its
result), and the gate's cache key includes the mode (module_gate.cpp). Unit tests: `emulate.windows16` 12 of 12.

Does it move existing verdicts? No: with `CB_FILL=1` the gate re-ran (USER 594 functions, GDI 134) and the sets it sends back are **identical** to
the baseline's (USER 40 / GDI 16, `diff` empty on the names).

Does it explain BEAR306 and sub_25_0429? No. `sub_25_0429` as C (forced out of its DS rule) passes the gate with and without the fill (it calls no
structure-filling API: ISWINDOW, its own helpers); BEAR306 is held by data/build/kept_as_asm.txt, which `CB_FORCE_C` does not override, so the fill
could not be tried on it without editing that file. So the gate gap for these two is not the callee-fill. What remains as candidates, unproven:
stack-frame layout dependence of the decompiled C (item 18), and state in the data segment that callees (stubbed) would have changed between
two messages (a window procedure is correct per call and wrong in sequence: the gate runs one call at a time).

### 20. A sequence-aware gate (built, default off: `CB_SEQ=1`; it does NOT catch BEAR306 or sub_25_0429)

**Design** (`judge_sequences` in verify/gate16/isolated16.cpp, run after the one-call inputs agree, for a function that removes 10 argument bytes, i.e. a window
procedure's pwnd, message, wParam, lParam -- the last five words pushed). Three scripts, each a message run: WM_NCCREATE, WM_CREATE, NCCALCSIZE, SIZE, MOVE, SHOWWINDOW,
ACTIVATE, SETFOCUS, ERASEBKGND, PAINT, then every message constant the function compares with (the directed inputs'), then DESTROY and NCDESTROY; the whole list twice per script.
The scripts differ in wParam / lParam, in what an unmodelled Windows call returns, and in the data segment (as loaded, or the patterned one). `pwnd` is a fresh, zeroed 0x60-byte
window structure in the data segment's zero tail. After every step both sides' prints are compared (calls, writes, result, SI/DI/DS) and so is the data segment each side has
accumulated: the writes of every step are carried to the next as `DataOffset` memory (`mem_init`), each side its own. Modes of the value: `1` callees answered alike (as the one-call
gate); `chain` the module's other functions run for real on both sides (a window word set by one callee and read by the next message's, carried too); `trace` prints how far each
function got (stderr). The cache key carries the value (module_gate.cpp). Also new: `CB_FORCE_C=name` now overrides kept_as_asm.txt (module_build16.cpp), so a kept function can be
tried as C without editing the file.

**Result (USER of MINI.CAB, 1613 functions, private `CB_STATE`; both functions forced to C)**:
- Both sequences runs (callees alike; and with `CB_FILL=1` as well) pass BEAR306 and sub_25_0429: sequence 1 and 3 run all the steps to the end on both sides with the same prints and
  the same data segment; sequence 2 of BEAR306 faults at step 25 (WM_USER+3) on both sides alike. Gated set: identical to the baseline (40, `diff` of the names empty).
  So **state kept in the data segment between messages is not what the gate misses**, at least with a window structure this simple, and filling BeginPaint's structure does not change it.
- `chain` mode: gated 40 -> 41; the one new refusal is `sub_35_187A` ("sequence 1 step 2, WM_CREATE after NCCREATE: returns original f83c, ours 0"). It is a three-line window
  procedure that calls sub_2_0B27 (assembly) then sub_35_085C or sub_35_08E1 (both C): the difference is most likely in one of those callees' C that the one-call gate cannot see
  (their callees are stand-ins), not in sub_35_187A itself; not traced further. `chain` also stops BEAR306 at its WM_PAINT (both sides fault in the real callees), so it says nothing of it.
  `chain` is therefore a lead, not a gate: a refusal blames the caller for a callee.
- Cost: about +10 s on USER's 4 minute build (the sequences run for the functions that remove 10 bytes only).

**What this leaves open for BEAR306 / sub_25_0429** (the doc's two remaining hypotheses were: data-segment state between messages -- refuted above; stack-frame layout dependence -- untried).
BEAR306's C (MINI's, 10:1587) reads its per-window structure through `sub_2_0B27(0, a5)` (assembly, AX in) and creates it in WM_CREATE through `LocalAlloc` + `sub_2_0B54`, so the state a
later message reads goes through callees the one-call gate stubs; `chain` would carry that but faults at WM_PAINT in the emulator. Next: make `chain` survive BeginPaint/EndPaint
(models for the fills in a chain run, or make the callee's own structure fill run), or trace the all-15 boot with the function as C and as assembly (the isolated16 recorder / QEMU)
and compare the calls from the first divergence. Nothing here is on by default; the proofs (`regress all15 --runs 2`, `regress install --runs 1`) were not run because nothing that
changes what is built is on: with the switch off the build is unchanged (the only default-path edit is the kept_as_asm override that applies only when `CB_FORCE_C` is set).

### 21. sub_35_187A explained; the boot-trace attempt for BEAR306 (2026-11-18)

**sub_35_187A is a chain-mode artifact, not a defect.** Original: `mov ds, 1160h`, `si = sub_2_0B27(0, a5)` (assembly, near pointer in AX), then `sub_35_085C` or `sub_35_08E1`
by bit 20h of `[si+2Ch]`; neither callee defines AX when its first tests skip the body (`test [si+2Ch], 8 / je end`: AX stays what the caller left, the near pointer), and
sub_35_187A returns what they leave. The original therefore returns the window pointer (f83c in the run), the C returns 0: an unspecified result inherited through callees,
the class of items 13 and 16. In `CB_SEQ=chain` mode the result is therefore no longer compared (calls and the carried data segment still are); the rerun of USER's MINI.CAB
gives gated 40 (the baseline), no sequence refusals.

**BEAR306 boot trace: not done; one approach ruled out.** Tried (default off, now removed) and found not to work: `CB_TRACE_WNDPROC=<name>` (module_build16.cpp for the C text,
build/asm16.cpp for the kept-assembly text: write 0B3h and the message, wParam and window words to port 0E9h at the function's entry) and `CB_DEBUGCON_FILE=<path>`
(setup16.cpp: QEMU `-debugcon`). A boot with either variant (BEAR306 as kept assembly with the probe; as C with the probe) is FAIL(dos) in 5m31s, while the same private
state with nothing set is PASS in 1m15s: the guest cannot execute `out` there (ring 3, or no I/O permission), so the probe itself stops Setup and the debugcon file stays empty.
A log has to go through memory, not a port: e.g. a ring buffer written in a block the probe allocates (GlobalAlloc through KERNEL) with a signature per record, and the VM's
memory saved with the monitor's `pmemsave` after the failure and scanned for the records; or QEMU's gdb stub with a breakpoint at the function's linear address (code selector
base from the LDT). Another way that needs no tracing: bisect by message -- keep BEAR306 as the original assembly under another name and have the C answer only a chosen
set of messages itself, the rest delegated; each boot is 5 minutes, about 5 boots for 28 cases.
**Correction:** there is no upper-case module-name trap. `kept_as_asm()` compares with `iequals` (unit-tested with `USER.EXE`), `Built::name` is lower-cased, and a clean rebuild
of MINI's USER from the upper-case path applied the kept lines (BEAR306 and sub_25_0429 came out as kept assembly). The "lines did not apply" runs were my own probe edits and
stale runs. The two probes (`CB_TRACE_WNDPROC`, `CB_DEBUGCON_FILE`) were removed.

**Static reading** (original assembly against the decompiled C and against the compiled C variant): every case of BEAR306 matches in argument order, DX:AX result, SI/DI/DS
saves and the 0x403 ES reads; the C is right as read. Two things it depends on without saying so: after `sub_2_0B27` the C copies `mov dx, ax` and answers 0x4c9 with `xor dx, ax`
(0 only because DX equals AX there). So the failure is not in an obvious case; it is a message-level or placement effect.

**Bisect by message** (`CB_BISECT=<function>:<hex>,<hex>[:<default>]`, default off; module_build16.cpp, module_build.cpp): the function's C answers only the listed messages (its
fourth word); every other goes to `sub_1_5D4F` with the same arguments (the function's own default path); the function is left out of the gate (it is meant to differ). Used with
`CB_FORCE_C=BEAR306`; `regress all15 --only USER.EXE --runs 1` on a private state, one boot at a time. Results below.

Not converted (superseded by item 16):


Next (item 3 of the lead's list, not started): GDI's remaining recorded reasons (16 gated, 10 never-set shared-body entries,
6 CL-to-helper) and USER's (73 no-room, 43 never-set, 20 DS-leaving callees); and KEYBOARD's three stubs (27 to 63 bytes:
`SCREENSWITCHENABLE`, `ENABLEKBSYSREQ`, `INQUIRE`), which still find no room.

### Where this stops

Converted and proved this session: `WEP` (item 1, +11 functions in all) and the prologue refit (item 3, +3). `DISABLE` and
`POWEREVENTPROC`: the gate was not changed, because `codebreaker teeth` was not run on a changed gate. Next, by size of
class: register live-in in the lifter (43 + 10 + 5 here, plus the 5 small-module ones), then the fixed-segment size budget
(73 + 2 + 10 small), then DS-leaving callees (23).

## State at the end of this run (2026-11-15) and the next step

Proven and on (snapshot b4547db78): the register-save rule (`WEP`), the prologue-free refit of frame-less stubs, the word-alignment-free packing for `-os`
objects, the compiler change in the OpenWatcom fork (no BP frame for a routine that never reads its stack parameters; rel/binnt64 holds it, originals in
backup-20261115), caller results (DX:AX), the unspecified-result rule for `in_AX`, the DS classifier (USER's callers of sub_1_5D4F), and the unspecified-DX rule
for GDI (`CB_DX_SHARED`, `CB_DX_UNSPECIFIED`, both on, `=0` turns each off). Each passed `regress all15 --runs 2` and `regress install --runs 1`.
Today's counts (`regress all15`): KRNL386 C 21, GDI C 132 / gated 16, USER C 574 / gated 40, SOUND 6, SYSTEM 2, VER 24; every other module as at the start.
Since the audit's start: all15 C 753 -> about 800 functions, none lost.

Built, off by default: the Windows-call structure fill (`CB_FILL=1`, item 19). It moves no verdict (identical gated sets) and does not explain the two
failures below.

**Next step: a sequence-aware gate, for `BEAR306` and `sub_25_0429`** (both in data/build/kept_as_asm.txt; their C passes the gate and breaks the all-15 boot
once it is used: FAIL(dos) and "error box before Welcome"). What is known (items 15, 18, 19): the DS reading of their call of sub_1_5D4F is right; the gate runs
one call at a time with callees stubbed; filling the structures Windows calls write does not change either verdict; `CB_FORCE_C` does not override kept_as_asm.txt
(to try BEAR306 as C remove its line temporarily, or add that override). The untried hypothesis: a window procedure is right per call and wrong in
sequence -- the data segment (and the per-window struct a window's near handle points at) left by one message is what the next one reads. So: let the gate
run a short script of messages through the same function and data segment on both sides (WM_NCCREATE, WM_CREATE, WM_SIZE, WM_PAINT ... taken from the message
constants the function compares with, which the "directed" inputs already hold), carrying the data between the calls, and compare after each. Prove
it first does not move existing verdicts (as item 19 did: compare the gated name sets before/after for USER and GDI), then try the two functions; a verdict that
changes for a third function goes back to assembly. Alternatives if that finds nothing: trace the all-15 boot with the function as C and as assembly (QEMU or the
isolated16 recorder) and compare the calls it makes from its first divergence; or bisect inside the function by keeping one `case` of its switch as instructions.

Tools and habits learned this run (for the next agent): `bash state/work/locked.sh` for shared builds; `state/work/build_tests.cmd` (cb_tests) and
`cmake --build state\cxx\cb --target codebreaker` (the exe); run regress on a private copy of the exe (`cp state/cxx/cb/bin/codebreaker.exe *.dll` into your
scratch folder) so another session's build cannot replace it; never run two series scripts at once (a stopped task leaves its child running); wait for a free
decompile slot (state/slots, four for the whole machine: a regress install may queue behind other sessions' decompiles); a regress run prints each module's
C / asm / gated; `state/build/<m>/module.l2.json` carries each function's recorded reason; `CB_ASM_ONLY` subsets move other functions and are not a reliable bisection.
