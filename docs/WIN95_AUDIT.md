# Windows 95 from authored source: the audit (ROADMAP track 1, step 1)

Audit of 2026-11-27. Read-only: nothing in the Windows95 repository (`C:\Users\mrtit\Documents\GitHub\Windows95`, HEAD `7573d480`)
was changed. The finish line it measures against (docs/ROADMAP.md, track 1): every function of every rebuilt module back as
source in the language it was written in, no gated functions, no stand-ins, `regress all15`, `regress setup` and
`regress install` passing. The premise behind "source" (Logan, restated): readable, maintainable, modifiable, integral
(behaves as the original), compilable with OpenWatcom (C) and MASM; not byte-exact.

This extends `docs/WIN95_COMPLETION.md` (the 2026-11-14 audit of the 22 boot modules and its conversion log). That document is
about the 22 modules `regress install` rebuilds. This one covers every module placed in the tree (1,172 folders) and adds what
the first left out: what the committed tree holds, whether it compiles, how readable it is, and what has been verified.

The per-module table is `docs/WIN95_AUDIT_MODULES.tsv` (1,172 rows, one per module folder, columns explained at the end).

## 1. Headline

| | |
|---|---:|
| module folders placed in the Windows95 tree | 1,172 (1,126 mapped to a corpus binary) |
| of them vendored original source (msdos/, the MS-DOS 6.0 drop rebased to 7.10), not reconstruction | 21 |
| ring-0 drivers as DDK-macro MASM (vmm32/) | 222 |
| C modules with function bodies in the tree | 620 (30,649 function bodies) |
| C modules that are **shells**: a `.c` of globals and cross-reference comments, no function body | 303 |
| curated by hand (RECOVERY.md, readable, builds): lz32, lzexpand | 2 |
| folders with no source at all | 6 |
| the 22 modules `regress install` rebuilds and boots (15 MINI.CAB + Setup's 7 programs) | 8,727 functions: **808 C (9%)**, 4,433 assembly (51%, by the build's own classification), 3,486 compiled code kept as instructions (40%) |
| stand-ins (a function replaced by `return 0`) in those 22 | 0 |
| tree C files that compile with OpenWatcom as they stand | 1,608 of 3,353; **518 of the 2,092 files that hold function bodies** (131 of 620 modules) |
| ring-0 drivers rebuilt and booted to the desktop | 13 of 222 (the 12 tested plus VMM) |

The one-line state: **Windows 95 boots from 22 modules of CodeBreaker's own authored form, and that form is not in the tree.**
The other 1,150 modules are in the tree as older decompiler output, most of it not self-contained, mostly unnamed, and not
rebuilt or run as part of anything.

## 2. What exists, in three layers

The Windows95 tree holds source from three pipelines, which do not agree with each other.

1. **The component tree** (`base/ comm/ core/ drivers/ mapi/ mmedia/ ole/ print/ setup/ shell/ unsorted/ vmm32/`, committed).
   Placed in September 2026 by the Python pipeline (README: "reconstructed source, not yet build-curated"). 3,365 `.c`,
   1,347 `.asm`. This is what the module table counts.
2. **The authored layer** (`design/recon/authored/`, committed, 2026-10-23): `c/<module>.c` per module, `tree/` split by
   source file, and the ledgers `AUTHORED`, `BUILDLINK`, `EXPORTPARITY`, `EXPORTGAPS`, `BEHAVIOR`, `VERIFY` (all .csv). The
   real-mode DOS base (IO.SYS, COMMAND.COM and the rest of the boot chain) is annotated MASM in `design/recon/*`.
3. **The module builds of today's CodeBreaker** (`design/codebreaker/build/<module>/`, `design/boot/mini/src/`): per-function
   L2 records (`module.l2.json`: a C function, or a machine fragment with the recorded reason), the C and assembly written from
   them, the `.def`, the link. Built 2026-11-21 to 2026-11-23 for 25 modules. **Gitignored** (`/design/codebreaker/`,
   `/design/boot/`): this is the form that boots in `regress`, and it is regenerated, not stored.

The committed tree's own `core/user`, `core/gdi`, `core/kernel` and `setup/SETUPX` are layer 1 shells: `core/user/user_seg5.c`
is 6 lines (a header and `#include "user.h"`), its `STATUS.md` says "VERIFIED 0, CANDIDATE 0", and the 30 `user_seg*.c`
total 8,337 bytes for a module whose rebuilt form has 576 C functions.

## 3. Summary, counts per state

State of each module folder, by what the committed tree holds (`state` column):

| state | modules | meaning |
|---|---:|---|
| `vxd-asm` | 222 | ring-0 driver as DDK-macro MASM: `<name>.asm` (Declare_Virtual_Device, Begin_Control_Dispatch, VMMcall/VxDcall) plus byte-exact per-segment `.asm` |
| `curated` | 2 | lz32, lzexpand: RECOVERY.md, named, headers, `owbuild.json`; lzexpand's 11 files compile with OpenWatcom |
| `raw-compiles` | 130 | function bodies, every `.c` compiles with OpenWatcom as it stands |
| `raw-no-compile` | 488 | function bodies; at least one file does not compile |
| `shell` | 303 | `.c` with globals and cross-reference comments only (0 function bodies); includes core/kernel, gdi, user, setup/SETUPX, SUWIN |
| `vendored-original` | 21 | msdos/: authentic MS-DOS 6.0 source (assembly 1,225 `.asm`, 486 `.c`), rebased to 7.10; not reconstruction |
| `no-source` | 6 | comm/ras, drivers/_root, print/HPVRES, shell/MORICONS, vmm32/_root, vnet/_root |

By component:

| component | modules | vxd-asm | curated | raw, compiles | raw, no compile | shell | none / vendored | function bodies in tree | functions the store holds for them |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| base | 13 | 0 | 2 | 0 | 10 | 1 | 0 | 2,708 | 2,968 |
| core | 8 | 0 | 0 | 0 | 3 | 5 | 0 | 196 | 8,578 |
| drivers | 107 | 0 | 0 | 0 | 105 | 1 | 1 | 2,592 | 10,151 |
| vmm32 | 253 | 222 | 0 | 0 | 0 | 30 | 1 | 0 | 1,262 |
| comm | 44 | 0 | 0 | 3 | 13 | 27 | 1 | 573 | 10,550 |
| mmedia | 34 | 0 | 0 | 7 | 11 | 16 | 0 | 839 | 4,775 |
| ole | 25 | 0 | 0 | 4 | 13 | 8 | 0 | 7,297 | 21,332 |
| print | 58 | 0 | 0 | 12 | 30 | 15 | 1 | 816 | 6,914 |
| setup | 30 | 0 | 0 | 6 | 12 | 12 | 0 | 196 | 5,988 |
| shell | 29 | 0 | 0 | 0 | 4 | 24 | 1 | 1,218 | 5,879 |
| mapi | 9 | 0 | 0 | 2 | 1 | 6 | 0 | 86 | 2,073 |
| games | 5 | 0 | 0 | 1 | 0 | 4 | 0 | 1 | 349 |
| unsorted | 535 | 0 | 0 | 95 | 286 | 154 | 0 | 14,127 | 121,592 |
| msdos | 21 | 0 | 0 | 0 | 0 | 0 | 21 | (vendored) | 1,493 |
| vnet | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 |

For the 620 modules with bodies the tree holds 30,649 function bodies; the store knows 150,645 functions in those modules, so the
tree carries about a fifth of what the current engine can already produce. The 535 `unsorted` folders (46% of the tree) are
modules whose component was never decided (placement confidence `low`: 487 modules; `none`: 48).

The function counts in the last column are the store's (a different count from the L2 build's for the same module; for
example KRNL386 is 1,166 in the store and 679 in its L2). They are used only to size things, not added to the table above.

### 3.1 The 22 boot modules, from their L2 records

`codebreaker regress all15|setup|install` rebuild these. Source: each module's `module.l2.json` of 2026-11-23 (so today's
numbers; they match WIN95_COMPLETION.md's end-of-run state: all15 C about 800). Columns: C = authored C function; hand asm =
"written in assembly" by the build's origin classification; kept = compiled code (origin says C) shipped as instructions, with the
reason class read from the recorded reason text; named = C functions with a name that is not `sub_<seg>_<off>`.

| module | functions | C | hand asm | kept as asm | of which no C compiled | C named |
|---|---:|---:|---:|---:|---:|---:|
| KRNL386.EXE | 679 | 21 | 639 | 19 | 0 | 8 |
| GDI.EXE | 883 | 132 | 702 | 47 | 2 | 40 |
| USER.EXE | 1,613 | 576 | 814 | 222 | 1 | 54 |
| KEYBOARD.DRV | 34 | 3 | 26 | 5 | 0 | 2 |
| VGA.DRV | 197 | 3 | 191 | 2 | 1 | 3 |
| SYSTEM.DRV | 13 | 2 | 11 | 0 | 0 | 2 |
| MINIKBD.DLL | 2 | 0 | 2 | 0 | 0 | 0 |
| MSMOUS31.DRV | 50 | 2 | 46 | 2 | 0 | 2 |
| NOMOUSE.DRV | 4 | 0 | 4 | 0 | 0 | 0 |
| LMOUSE31.DRV | 59 | 3 | 52 | 4 | 0 | 1 |
| SOUND.DRV | 37 | 6 | 29 | 2 | 0 | 4 |
| COMM.DRV | 84 | 2 | 82 | 0 | 0 | 2 |
| VER.DLL | 31 | 24 | 3 | 2 | 2 | 7 |
| LZEXPAND.DLL | 46 | 32 | 13 | 1 | 0 | 12 |
| WIN87EM.DLL | 39 | 2 | 37 | 0 | 0 | 1 |
| **all15** | **3,771** | **808 (21%)** | **2,651 (70%)** | **306 + 6** | | **138 (17% of the C)** |
| SUWIN.EXE | 779 | 0 | 130 | 649 | 26 | n/a |
| SETUPX.DLL | 1,466 | 0 | 299 | 1,167 | 40 | n/a |
| SYSDETMG.DLL | 1,170 | 0 | 331 | 839 | 9 | n/a |
| NETDI.DLL | 1,476 | 0 | 1,000 | 476 | 27 | n/a |
| SUEXPAND.DLL | 46 | 0 | 13 | 33 | 0 | n/a |
| W95INF16.DLL | 12 | 0 | 5 | 7 | 0 | n/a |
| SETUP.EXE | 7 | 0 | 4 | 3 | 0 | n/a |
| **setup7** | **4,956** | **0** | **1,782 (36%)** | **3,174** | **102** | |
| **install (22)** | **8,727** | **808 (9%)** | **4,433 (51%)** | **3,486 (40%)** | | |

(The setup7 modules are built all-assembly on purpose, as `regress setup` builds them, because every C version crashes; the
521 + 978 + 652 + 377 + 32 + 7 + 3 reasons that read "its C is not used: " with nothing after it are that mode, not evidence
about the function.)

Why the 306 compiled functions of all15 are still instructions (read from the recorded reason; one reason per function):

| reason class | functions | where |
|---|---:|---|
| no room in a fixed-size segment (KERNEL/GDI/USER segment 1 must stay the original's size) | 104 | USER 78, GDI 12, KRNL386 10 |
| the function's own return register or a result register the callers read (AX/DX/ES/flags) | 50 | USER 45, GDI 5 |
| its C differs when run beside the original (the gate's verdict) | 43 | USER 32, GDI 6, LMOUSE31 3, MSMOUS31 2 (WIN87EM's WEP is fixed) |
| reads a variable the C never sets (register live-in) | 43 | USER 29, GDI 8, KRNL386 3 |
| calls a function that leaves DS changed | 20 | USER 14, GDI 3, KRNL386 2 |
| reads ES as entered | 19 | USER 10, GDI 5, KRNL386 4 |
| passes the caller's CS | 9 | |
| other (flags return, contract) | 17 | |
| the OpenWatcom compile of the decompiled C fails | 6 | VER 2, GDI 2, VGA 1, USER 1 |

(The class counts add to 306 + 6 within rounding of how a reason that names two causes is assigned; GDI's and USER's gated
counts in WIN95_COMPLETION.md's end state are 16 and 40, which this table splits between "no room" and "gated" by which cause is
printed first.)

Built the same way but outside the 22, so also in authored form today: REGEDIT.EXE (152 C functions, no assembly),
XCOPY32.EXE (40 C), UNINSTAL.EXE (5 C, 4 asm, 1 gated).

## 4. Language, per module

"Language" has three answers and they are different questions.

| | answer |
|---|---|
| what the original was written in | `origin_lang` column (design/recon/authored/AUTHORED.csv, 2026-10-23): C/PE 577, C/NE 326, asm/LE 218, asm/MZ 16, C/MZ 26, unclassified 28 (of 1,191 binaries). For the 22 boot modules the build classifies each function (section 3.1): 51% hand assembly, which is a count of instruction shapes (argument in CX, JCXZ, IN, a function that reads a register it never sets), not a recovered fact; USER at 814 of 1,613 and GDI at 702 of 883 are worth a second look. |
| what the committed tree holds | `tree_lang`: C 923 modules, asm 222, vendored 21, none 6. No mixed C-and-asm module exists in the component tree (the boot modules' hand assembly lives only in layer 3). |
| what the form that boots is | C functions plus MachineFragments (`wasm` syntax, `.386p`, `db` for encodings the assembler would not reproduce), per function, in `module.l2.json`. |

The 222 VxDs are the right language (assembly, DDK macros). The 30 IOS miniport drivers (`.mpd`, PE format) in `vmm32/` are
shells.

## 5. Builds with the period toolchain (OpenWatcom and MASM)

Measured: every tracked `.c`/`.cpp` of the component tree (3,353 files) compiled with CodeBreaker's own `codebreaker compile`
(OpenWatcom `wcc`/`wcc386` in this program; 16-bit `-bt=windows -ml` for NE modules, 32-bit `-bt=nt` for PE, OpenWatcom's own headers,
no module-specific include path, 4 at a time, output outside the tree).

| set | files | compile | |
|---|---:|---:|---|
| modules with function bodies | 2,092 | 518 | 25% |
| shell modules | 1,261 | 1,090 | (nothing in them to compile; 171 still fail) |
| all | 3,353 | 1,608 | 48% |

27,332 compile errors in all. The first error of each of the 1,574 failing files that hold function bodies, by class: an
undeclared symbol, almost all of them the decompiler's own temporaries (`r24` alone is 550 files; `r20` to `r27`, `res`,
`result`, `register0x...`: 1,030 files), a type the file never declares because it includes no SDK header (`WORD`, MFC's
`ON_MESSAGE`, `DISP_FUNCTION`: 320), a function returning a value it is declared not to (102), other (107), no finish in 120 s
(15, among them inetmib1, wininet, ole32). In the shells, the first error is usually a header the file names that is not there
(`afxext.h`, `print.h`). The curated pair is the exception: lz32 and lzexpand's main files compile.

Build recipes in the tree: all 186 `build.mk` files are Microsoft C flags (`/AM /G2 /Gsw /Ox`) with include and library paths into
`C:\Users\mrtit\...\Windows95\recon\toolchain\...`, a folder that does not exist; none names OpenWatcom. Two modules have an
`owbuild.json` (lz32, lzexpand). `tools/` in the tree is Python (`build16.py`, `build_all.py`, `build_cd.py`, `mkiso.py`,
`place_map.py`, `place_source.py`), retired since 2026-11-14.

Other ledgers (design/ledgers, 2026-11-20 and earlier, built by the Python pipeline): BUILDALL.csv, 566 DLLs: 284 compile and
link, 277 compile, 3 resource-only, 2 curated; 121 fail to link. design/recon/authored/BUILDLINK.csv, 922 modules: 917 link,
with **4,519 stubs** across 879 modules; the linked image's size is a median 46% of the retail file's. EXPORTPARITY.csv (923):
739 export every retail name, 53 are short (3,157 exports have no recovered function: 2,791 "unresolved", 333 "no function
recovered"), 130 do not build.

Ring-0 (design/docs/VXD_COVERAGE.md and docs/VXD.md): all 222 re-assemble byte-exact from the MASM (903 of 903 objects), and the
from-source loadable build covers 222; `vxd boot` boots 12 rebuilt drivers plus VMM32 to the desktop (the other 209 never
booted rebuilt). I did not rebuild a driver for this audit (a `build --driver` run of `vtd` timed out without output in 3 minutes);
these are the ledgers' numbers.

The 22 boot modules build with OpenWatcom and the native linker inside `codebreaker build`/`regress`; that is the only place a
module is built, linked, put on the CD and booted.

## 6. Verified to behave as the original

| evidence | what it covers | result |
|---|---|---|
| `regress all15` / `setup` / `install` (docs/WIN95_COMPLETION.md) | the 22 modules, Setup to the desktop | last recorded passes 2026-11-15 to 2026-11-18 (all15 PASS twice; setup PASS 1h11m; install PASS 1h27m, 1h52m); shots through 2026-11-23. **Not re-run for this audit** (an hour or more of the machine); the L2 records of 2026-11-23 match the end-of-run counts. |
| the function store (`design/codebreaker/functions.sqlite`, judged 2026-11-20/21, 1,149 modules) | the current engine's C for every function, run beside the original | 203,904 functions: verified 83,773 (41%), weak 50,409, unverified 47,441, no-compile 7,779, not-judged or no-source 14,502. These are verdicts on the engine's candidates, not on the tree's committed C. |
| the store, for the boot modules | | USER 505 of 2,317 verified (22%), GDI 293 of 1,657 (18%), KRNL386 154 of 1,166 (13%), SETUPX 1,018 of 2,932 (35%), SUWIN 638 of 1,558 (41%), SYSDETMG 372 of 2,340 (16%), LZEXPAND 54 of 92, VER 30 of 62 |
| BEHAVIOR.csv (Oct 2026) | whole exports run against the original, NE modules only | 799 modules; 4,414 exports tested, 174 equivalent (3.9%); PE and MZ modules: 0 tested |
| `vxd boot` | rebuilt drivers on a real boot | 13 of 222 |

"Verified" in the store was re-based on a verifier with teeth after the 2026-10-12 finding that the old one passed 41% of
bodies it should have rejected; the store's numbers are on the current profile (a809807d25b3), not the old sweep's 81,649.

Nothing is verified by running the **tree's** committed C: no tree module other than the boot set is built and run.

## 7. Readability

Two measurements, on different source.

**The form that boots (22 modules):** the C in `design/codebreaker/build/<m>/` is decompiler output with a typed skeleton.
VER's `GETFILERESOURCESIZE`, for example, takes `(a1,a3,a4,a5,a6,a7,a8)` in K&R style, keeps its locals in a
`_Packed struct codebreaker_frame`, indexes it with casts (`*(xunknown4 *)((uint1 *)codebreaker_frame.axStack_50 + 0x40)`), passes
`unaff_SS` by hand, and calls KERNEL by ordinal (`KERNEL_18`, `KERNEL_176`, `LZEXPAND_6`). 138 of 808 C functions (17%) have
a real name; every parameter in KRNL386, GDI, USER, VER, LZEXPAND is `a<N>` or `param_<N>` (498 of 498 in GDI, 1,521 of 1,521 in
USER); types are `int2`, `int4`, `xunknown1`; there are no comments except the decompiler's own `WARNING:` lines (17 functions in the 15 modules).
The assembly fragments carry no names but the function's, and `db` runs for encodings.

**The committed tree (620 modules with bodies; heuristic: references to `sub_<hex>` per function body, under 1 = named, 1 to 4 =
partly named, 4 and over = raw):** raw 289 modules, partly named 261, named 70. By function bodies: 11,379 in raw modules, 17,274
in partly named, 1,996 in named. The curated pair (lz32: 13 bodies, lzexpand: 90) are the only modules written to the bar. The
older pipeline also left junk: `design/recon/authored/c/mciole.c` begins with `extern int the(); extern int no(); extern int not();`,
declarations made from words of the decompiler's own comments.

**The VxDs:** the control dispatch, service table and `VMMcall`/`VxDcall` sites are DDK idiom (13,386 symbolic service calls,
19,097 `BeginProc`); internal routines are `sub_<object>_<offset>` (18,243 distinct) unless a `.SYM` named them; 125,608 `loc_`
labels; 9% of the code lines are `db` for encodings `ML` would not reproduce; no source comments.

## 8. What is missing

- The 22 modules' authored form is not committed anywhere (section 2, layer 3).
- Everything outside those 22 has no run, no boot, no rebuild into the install.
- The Setup chain is only part ours: SUUTIL and W95INF32 (32-bit) and the WIN95_xx cabinets are still the originals
  (memory, 2026-10-20; not re-checked here), and the other ~1,286 of the 1,308 binaries Windows 95 installs are the retail ones.
- Resources, fonts (20 excluded), data files (1,067) and the 3 code-less resource DLLs have no place in the tree.
- A `RECOVERY.md` (evidence and inferences, per the Reconstruction Standard) exists for 2 modules; `STATUS.md` exists in 187 folders, mostly the generated kind that says "VERIFIED 0".

## 9. The gaps that block "Windows 95 from authored source", ranked

Ranked by how much each blocks (modules or functions) and by whether others wait on it. Each group is a unit of work.

### Group A: the form that boots must be the form that is kept

**A1. Commit the authored form of the 22 modules into the tree (22 modules, 8,727 functions).** Today `core/kernel`, `core/gdi`,
`core/user`, `setup/SETUPX` and the rest are shells and the real form is regenerated into gitignored folders. Make `build` write
each module's source (C per function, assembly fragments with their reasons, `.def`, resources, a build recipe that names
OpenWatcom and MASM) into its folder, make `codebreaker build <folder>` rebuild from that folder alone and `regress` run
from the folders; commit them. Everything else in this list is unmeasurable until this exists, because a change to the
decompiler changes the "source". Smallest, unblocks all of B and C.

**A2. Replace the old placement with the current engine's output (about 1,150 modules, 150,645 functions).** 303 shell modules
hold nothing; 488 raw modules hold a fifth of their functions in code that mostly does not compile (30,649 of 150,645). The
engine's `convert` can write each module; `unsorted/` (535 folders, 46%) needs its component decided first (487 are `low`
confidence). Order by size: shells that already built (the 130 `raw-compiles` modules and the 22 above) first.

### Group B: functions still kept as instructions in the boot set (3,486)

**B1. Setup's seven programs: 3,174 compiled functions, 0 as C (SUWIN 649, SYSDETMG 839, SETUPX 1,167, NETDI 476, SUEXPAND 33,
W95INF16 7, SETUP 3).** Every C version crashes: SUWIN's reaches the License page and then faults (3 functions forced to
assembly get it to Windows Components), SETUPX's faults at start with 14 functions bisected and cause unknown
(`config/build/kept_as_asm.txt`), NETDI refuses assembly in 5 segments (a short `ja` out of a function; 110 fixups to a
selector with no symbol) and falls back to C, and its build took 30 to 100 minutes. This is roadmap item 3 and the largest
single block of the finish line. Method that worked before: first ring-3 fault, then the live-VM gdb tools; smallest module first.

**B2. The all15 C-origin functions kept as instructions (306, plus 6 whose C does not compile).** USER 222, GDI 49, KRNL386 19.
By class (section 3.1): fixed-size segments leave no room for the C (104: a packing change, not a decompiler one; the memory
note says to pack into holes and the resident part); a result register or ES/DS/CS contract the C cannot express (98); a register
live-in the lifter does not model (43; the "register live-in in x86_lift.cc" keystone); the gate's verdicts (43, of which two,
BEAR306 and sub_25_0429, are fixed behind `CB_FN_PTR_STORES`, default off). Each class fixed once moves many functions.

**B3. The 4,433 "hand assembly" functions are a classification, not a fact.** Kept as assembly by the rule "hand-written assembly
stays assembly", but the test is instruction shapes. 51% for the 22, 70% for all15 (USER 50%, GDI 80%) which is a lot for modules that Microsoft is
unlikely to have written mostly in assembly. If any are compiled C they belong in B2. A check against compiler idioms and the Win3.1/Win95
SDK samples settles it per segment.

### Group C: readable, maintainable, compilable (the premise)

**C1. Names, types and structure for the boot set's C (808 functions).** 83% of functions are `sub_...`, every parameter is `a<N>`,
locals live in `codebreaker_frame`, KERNEL/USER calls are by ordinal, types are `xunknown`. The pieces exist (the namestore,
docs/NAMING.md, the SDK typing of L2, the Win95 DDK/SDK symbols and `.SYM` files); the work is wiring them to the output of
`build` and measuring: named share, parameters with SDK types, no `xunknown`, no frame struct.

**C2. Make the tree's C self-contained and compilable (488 modules, 1,574 files).** The errors are few classes (section 5): temporaries
the decompiler used but did not declare (1,030 files, first error), types from headers the file never includes (320). Fix once in the writer, not per file: declare or eliminate temporaries, include the SDK header the module's imports
imply, one `m.h` per module. Target: `compile_ok = compile_total` in the TSV for every module with bodies.

**C3. Build recipes: one, OpenWatcom and MASM, no absolute paths.** Replace the 186 `build.mk` (Microsoft flags, paths into a
folder that is gone) and the Python `tools/` with the recipe `build` writes (A1). `owbuild.json` is the only recipe form that
builds today (2 modules).

**C4. Ring-0 source quality (222 drivers).** Builds and re-assembles byte-exact, but routines are `sub_<object>_<offset>`
(18,243), 125,608 anonymous `loc_` labels, 9% `db`, no comments. Names from the VMM/VxD service tables, `.SYM` where the
corpus has one, structure, plus rebuilt-and-booted beyond 13 (`vxd boot --bisect` over the rest).

### Group D: proof

**D1. Run the milestone regression on the audited state (22 modules).** `regress all15 --runs 2`, `setup`, `install` once, to date
the numbers in sections 1 and 3.1 (last proofs 2026-11-15 to 2026-11-18; the L2 records are of 2026-11-23). Not done for this audit.

**D2. Behaviour evidence for the rest.** The only module-wide evidence outside the 22 is the function store (41% verified on
the current profile, on the engine's candidates) and BEHAVIOR.csv (3.9% of NE exports equivalent, no PE module tested). To build
to "Windows 95 from authored source" a module needs to be put in the install and the install run, so the second ranked
verification is the install gate over more modules: add the next components in size order (the 15 MINI.CAB modules' neighbours:
KERNEL32, USER32, GDI32, SHELL32, the VxDs the boot loads) and run `regress install` with them.

### Group E: the edges

**E1. The Setup chain's remaining originals** (SUUTIL, W95INF32 32-bit; the WIN95_xx cabinets): rebuild or place.
**E2. No-source folders and non-code files** (6 folders; the 3 resource-only DLLs; fonts; 1,067 data files): resources are
rebuilt verbatim by the resource lane but have no home in the tree.
**E3. Documentation of inference.** `RECOVERY.md` for each module (evidence and what is inferred) exists for 2 of 1,172; the
Reconstruction Standard asks for one per module. Generate it from the L2 record's reasons and the store's verdicts once A1 exists.

Suggested order: A1, then B2 and B3 (all15 is small enough to finish and each fix is proved by `regress all15`), B1 smallest
program first, C1 in step with them, then A2/C2 for the next modules into the install (D2). The ROADMAP's four phases are the same
list: phase 2 = A1 + B2/B3, phase 3 = B1, phase 4 = D1 on everything.

## 10. How this was made, and what is not covered

- Tree inventory: `git ls-files` of the Windows95 repository, one record per `<component>/<module>` folder (legacy/ and _build/
  excluded); function bodies and statements counted by a brace-depth scan of each `.c`/`.cpp` (comments stripped), assembly lines
  by directive; placement and corpus names from `build/place_map.json` and each `PLACEMENT.md`.
- Compile sweep: `codebreaker compile FILE --target i86|i386 -bt=windows|nt -zq -w0 -fo=<outside the tree>` per file; the target
  from the module's format in place_map. OpenWatcom's headers only: a module that needs the Chicago SDK's headers fails here
  and would pass with its own include path, which no module has; 15 files timed out at 120 s.
- Store: `design/codebreaker/functions.sqlite` read-only (the `summary` table's latest profile per module).
- Authored layer: the six `design/recon/authored/*.csv` ledgers, joined by module name.
- Boot set: the 25 `design/codebreaker/build/<m>/module.l2.json` read as data; reason classes by text match on the recorded
  reason.
- Not done: no regression run, no rebuild of a VxD (see section 5), no run of any tree C. Counts of "hand asm" and "readable"
  are heuristics as stated. The store's function counts differ from the L2's for the same module. The `converted/` folder
  (untracked, one module, VGA) is not counted. Audit scripts and intermediate files: `work/track1-audit/` (not committed).

## Columns of WIN95_AUDIT_MODULES.tsv

`component`, `module`: the folder. `corpus`, `fmt`: the binaries it holds (place_map) and their format. `origin_lang`: the
authored layer's classification. `tree_lang`: C, asm, vendored, none. `state`: section 3. `readability`: shell, raw,
partly-named, named, ddk-macro-asm, original, none. `files`, `c_files`, `asm_files`, `loc`. `bodies`, `statements`,
`sub_refs`, `gotos`, `comments`: from the C. `db_lines`, `asm_code_lines`: from the assembly. `store_functions`, `v_verified`,
`v_weak`, `v_unverified`, `v_nocompile`, `v_other`: the store's latest verdicts for the module's corpus binaries.
`compile_ok`, `compile_total`, `first_error`: the OpenWatcom sweep. `authored_build`, `authored_size_pct`, `authored_stubs`,
`export_parity`, `exports_missing`, `behav_tested`, `behav_equiv`: the authored layer's ledgers. `buildall`: BUILDALL.csv
(status/build). `placement_conf`, `curated`, `boot_set` (all15, setup7, or built), `legacy_files`.
