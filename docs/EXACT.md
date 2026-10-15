# exact — rebuilding a compiled program byte for byte, from its source

Planned 2026-10-15. The engine for one kind of work: a DOS (or XENIX/ISIS) program built by a
compiler and linker of its time, rebuilt so that the period tools, run on reconstructed source,
make the original file again, byte for byte.

Fourteen programs were rebuilt this way by hand-written per-project scripts (Microsoft-Assemblers-
Compilers/research): MS C 1.0x and 2.03 (Lattice), MS C 3.00's six programs, MASM 1.00/1.10/3.00
(MS-Pascal), MASM 4.00 (MS C 3.00/4.00), MASM 5.10 (MS C 5.0), LINK 1.02-2.00 and LIB 1.02
(PL/M-86), LINK 2.40/2.44 and LIB 2.20/2.40 (XENIX C). The same ideas were written two to six
times over. `exact` is those ideas once, with what differs between toolchains moved into data.

## The shape

    toolchain profile   what a compiler/linker does, as data (+ named hooks where code is needed)
    project             what one program is: a directory of plain-text descriptions
    stages              generic engines that fill in the project, each checked by the bytes
    driver              python -m codebreaker exact <project> [stage]

A stage never writes what it cannot prove: everything it emits is compiled or linked by the
period tool and compared. What it cannot settle it leaves as a stand-in (the original's bytes in
an object of their own) and reports, so a program always relinks and the report says how much of
it is source.

## Toolchain profiles (codebreaker/exact/toolchains/*.json)

One per compiler/assembler/linker version. Facts that were scattered through scripts:

- how to run it: the kit's files, the emulator (in-process Unicorn DOS, XENIX-86, ISIS-II 8080),
  the command lines, switch sets; pass patches by name (e.g. MS C 3.00 release vs the build that
  made 3.00 itself) and pass hooks (the third pass's "room for one more merge?" answers)
- how its functions look: prologue/epilogue patterns, stack-check call, frame direction,
  register variables, the local-frame name hash (C 3.00: sum mod 11; C 5.0: (s+(s>>9))&15)
- how it lays out data: literal placement (with the function / pooled at the file's end /
  function-less files: literals first), variable order relative to functions, alignment,
  static vs communal, PL/M's words-then-bytes, far data segments named after the file
- calling convention (C, Pascal), symbol decoration
- for linkers: library module take order, communal placement hash (LINK 2.40 hash8),
  relocation table order, header quirks (LINK 1.08's load size), pad rules, EXEMOD/EXEPACK

## The project (one directory)

The msc-3.0 description files, made the common format:

    project.json   the original file, the toolchain profiles, model, default switches
    SYMBOLS        name kind address (learnt from identical functions)
    MODULES        name code-start data-start [switches]   (link order; - = data only)
    CRTLIB         library modules in image order; RAW name code=a-b data=c-d pub=n@o
    BSS            range / hole / name offset
    LINKOPT, FIRSTOBJ, EXEMOD
    src/           fADDR.c (one function each), dADDR.c / NAME-data.c, headers
    LEDGER         per function: identical / differs / stand-in

## Stages

1. **carve** — function starts and ends from the profile's prologues + call targets + CFG.
2. **library** — library modules found in the image (relocation-masked bytes against the kit's
   .LIBs, versions between releases noticed), their order, their data; unmatched runs become RAW
   stand-ins with public names taken from the references into them.
3. **names** — SYMBOLS seeded from the library and learnt from identical functions; sources ported
   from sibling programs and earlier versions (compile-compare over normalized bytes), renamed to
   this program's places; frame names fitted to the profile's hash.
4. **functions** — a first draft from the compiler's inverse model (CodeBreaker invert/: early_c,
   msc6, and models still to write for C 3.00-5.0), then a bounded search over meaning-preserving
   rewrites that the bytes decide: loop forms, case-group order, declaration order, statement
   grouping, the pass hooks. Stand-in when nothing fits.
5. **modules** — functions grouped into source files: what-strings, which data each function
   touches, assertion sccsid arguments, calls; link order from the data's order.
6. **data** — each file's initialized data from the image, placed by the profile's rules, typed by
   how the functions use it (tables of structs, biased bases, places inside objects rewritten as
   such, literals indexed directly, far data). Checked by compiling the file whole.
7. **bss** — uninitialized variables pinned at their places; library holes; stdio buffers.
8. **link** — the period linker under the emulator; post-link steps; whole-file compare with a
   diagnosis of the first divergence (code shift / data shift / BSS / header / relocation order /
   pad) that names the stage to fix.
9. **readable** — declarations made to agree, struct copies hoisted into headers, names.

## How it is proven

Each stage is built and validated on programs already rebuilt by hand, whose answers are known:
the engine, run on the original file with no hand edits, must reproduce what the hand work found.
Phase order and ground truth:

| Phase | Builds | Validated on |
|---|---|---|
| 1 | emulators in-process, toolchain profiles, one compile-compare runner, the project format | MS C 3.00 P0 ledger; MASM 4.00; LINK 2.40 (XENIX C) |
| 2 | carve, library, names | MS C 3.00 P0-P3, MASM 5.10 |
| 3 | modules, data, bss | MS C 3.00 P1, P2 (data identical) |
| 4 | link + diagnosis | MS C 3.00 MSC/CL/P0/P1/P2 relink file-identical; P3 finished by the engine |
| 5 | functions (drafts + search) | functions the hand work wrote, re-derived blind |
| 6 | readable | — |

Then the engine is pointed at programs nobody has rebuilt.
