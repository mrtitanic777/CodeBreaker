# One recompiler

Unification item 5 (docs/ENGINE_UNIFICATION.md): `recompile/x360`, `recompile/xenonrecomp` and `recompile/rexglue` become one
static recompiler on the shared p-code / L2 core, with each console's runtime beside it. Tetris, Minecraft and Up must still play
at every step. Once that is done, the recompiler takes any CPU that has a SLEIGH description.

This file says what is there today, what the one design is, and the order of the steps, each with its proof. Each step gets its
status at the end as it lands.

## What there is today (2026-11-28)

The lane is one command, `codebreaker recompile-project <title> [--check]` (docs/RECOMPILE.md). Behind it, five engines do
work that overlaps.

| Piece | Where | What it does | Who uses it |
|---|---|---|---|
| XenonRecomp | `src/recompile/xenonrecomp/XenonRecomp` (3,600 lines; hedge-dev's, MIT, with our changes in CHANGES.md) | Translates each guest instruction into one C++ statement on a `PPCContext`. It decodes with its own copy of binutils' `ppc-dis.c` and reads a TOML configuration. | Every title `recompile-project` makes, the Dashboard's modules, and the `xenon-recompile` command |
| XenonAnalyse | `src/recompile/xenonrecomp/XenonAnalyse` | Finds switch tables by matching fixed instruction sequences. | `recompile-project`, to add the tables our own finder misses |
| CodeBreaker's analysis | `src/recompile/discover`, `switches`, `config`, `jumptable` | Finds every function (`.pdata`, calls, data pointers, built addresses, leaf functions), the save/restore runs, `setjmp`/`longjmp` and the switch tables, then writes XenonRecomp's configuration. It decodes with Capstone plus `decompile/vmx128`. | `recompile-project` |
| CodeBreaker's translator | `src/recompile/powerpc`, `builders/` (3,400 lines, derived from rexglue's code generator) | The same per-instruction C++ form, one function at a time or a whole image. | The `recompile` command, its tests, and `recompile-project --runtime rexglue` |
| rexglue | `src/recompile/rexglue/kernel` (rexglue's kernel core) and `dxbc` (its shader translator to DXBC) | A second runtime, Xenia-shaped. It is copied into a `--runtime rexglue` project. Its headers are the include path for `recompile --check`. | Only the `--runtime rexglue` project form and `recompile --check`. No title plays on it. |
| The x360 runtime | `src/recompile/x360` (runtime, system UI, server) | The Xbox 360 as native code: the kernel and XAM export by export, Xenos on Direct3D 12, XMA, input, files, the Guide. | Every title that plays |

Beside these, the shared core already has the Xbox 360's CPU as a description:

- `decompile/engine/languages/src/ppc/ppc_64_xenon_be.slaspec` is the Power ISA with AltiVec and VMX128.
- The decompiler lifts from it.
- `sleigh_reader` reads instructions from it.
- The one emulator (`decompile/engine/pcode_emulator`, the PowerPC adapter in `emulate/cpu/pcode_cpu_ppc`) runs it. The vector
  and system user operations it needs are implemented once, in `emulate/cpu/pcode_ops_ppc*`.

So today three PowerPC decoders read the same instructions: binutils' in XenonRecomp, Capstone with vmx128 in our analysis,
and SLEIGH in the core. Two translators write the same C++ form, and two runtimes exist where one runs anything.

### What they share

- **The output form.** XenonRecomp and the `builders` both write `void sub_X(PPCContext& ctx, uint8_t* base)`, one statement per
  instruction, with memory through `PPC_LOAD_*` and `PPC_STORE_*`, calls by name and indirect calls through one table.
  The runtime depends only on this form: `PPCContext`'s layout (`ppc_context.h`, set by `ppc_config.h`), the macros, the
  `__imp__sub_X` / weak `sub_X` hook pair, and `PPCFuncMappings`.
- **The facts about a title.** The functions and their sizes, the switch tables, the save/restore helpers, `setjmp`/`longjmp`,
  and the register settings. Our analysis already decides all of them. XenonRecomp only reads them from the TOML it is given,
  except the switch tables XenonAnalyse adds.
- **The instruction semantics.** These are the same three times over: XenonRecomp's `switch` on `PPC_INST_*`, the builders,
  and the SLEIGH description with `pcode_ops_ppc`. Xenia's instruction tests (`xenonrecomp/ppc_tests`, 162 files with the
  console's results) check the first.

## The one design

```
title (XEX, package, disc)                      formats/xex, recompile/title
  -> image + facts                              recompile/discover, switches, config  (the one analysis)
  -> per instruction: p-code                    decompile/engine sleigh, ppc_64_xenon_be.sla  (the one decoder)
  -> per function: C++ on the context           recompile/emit  (the one translator)
  -> project                                    recompile/x360project
  -> program = generated C++ + console runtime  recompile/x360  (the Xbox 360's runtime, beside the core)
```

1. **One decoder: SLEIGH.** The recompiler reads instructions the way the decompiler and the emulator do. Binutils' `ppc-dis.c`,
   and Capstone for PowerPC, retire from this lane.
2. **One analysis: ours.** `discover`, `switches` and `config` decide every fact. XenonAnalyse retires once `switches` finds
   every table it adds (the projects' logs count them: `SwitchTables::added`).
3. **One translator, from p-code.** Each instruction's p-code (its varnodes in the register, unique, constant and ram spaces) is
   written as C++ on the context:
   - a register varnode is the context field SLEIGH's register maps to (a table from the description's register names to
     `PPCContext` fields);
   - a unique varnode is a local;
   - a LOAD or STORE goes through `PPC_LOAD_*` / `PPC_STORE_*`;
   - BRANCH, CBRANCH, CALL, CALLIND and RETURN become the same gotos, calls, switches and returns the current output uses;
   - a CALLOTHER (VMX128's vector operations, `mftb`, `sync`, the traps) calls the same implementation the emulator binds in
     `pcode_ops_ppc`. That code moves into a header both include, so the emulator and the recompiled program cannot disagree.

   The output keeps today's form (the `PPCContext` layout, the macros, the hook pair, the mappings), so **the runtime does not
   change** when the translator does. The C++ compiler folds the p-code's temporaries. "Never slower" is measured on the
   generated programs' frame times, not only on the time it takes to generate them.
4. **On L2.** A recompiled function is a list of statements on machine state, which is L2's `MachineFragment` and `Goto` /
   `Labeled` form. Once the p-code emitter is proven, its output is built as L2 and written by L2's C++ back end, the same back
   end every other front end uses. The emitter's knowledge (the register table, the memory macros, the call forms) becomes that
   back end's rule set `machine`. Nothing about the Xbox 360 sits in the back end: the register table comes from the description,
   and the macros and context come from the runtime's contract.
5. **One runtime per console, beside the core.** `recompile/x360` is the Xbox 360's runtime. rexglue's kernel and shader
   translator retire with the `--runtime rexglue` project form. `recompile --check` compiles against the x360 runtime's
   context header instead of rexglue's.
6. **Any CPU.** A second console is its SLEIGH description, its register table and a runtime of its own. The decoder,
   translator, project and build stay the same. The first after the Xbox 360 is chosen with Logan. The PS3's Cell PPU, on
   `ppc_64_isa_altivec_be.sla`, is the nearest.

What does not change: the `recompile-project` command and its project layout, the runtime's switches, and the `xenon-recompile`
command's interface (a configuration in, `ppc/` out), which the Dashboard builds with. That command is served by the one
translator once it is proven, so the Dashboard does not have to change at the same time.

## The steps

Each step lands on its own with its proof. "Plays" means what the slow regression tier checks (`tests/regress/slow.json`:
`recompile-project <title> --check` exits 0, meaning the title built, presented frames for the whole run, called no
unimplemented import and did not stop). Where a step leaves the generated C++ byte-identical, the titles play by construction,
and the proof is the byte comparison. A step that changes the generated C++ is proven by running the titles. Every step that
changes behaviour also needs a person to play each title for a while on a controller, because a headless run does not press
buttons.

0. **Baseline.** From this branch, recompile, build and check Minecraft and Up. Record frames and stops, and a digest of every
   generated file (`src/ppc/*`) as the golden for the byte-identical steps.
   - Tetris: its package is no longer on this machine. `C:/Nexia360/Games` holds 18 entries, Tetris Splash is not among them,
     and the settings have no `regress_tetris`. It needs Logan to put it back before any step can be proven on it.
1. **The p-code of one instruction, as data.** The core gains a lift API: the p-code ops of the instruction at an address,
   with their varnodes (space, offset, size) and the user operations' names. The emulator already decodes this internally, but
   nothing outside the core can see it. Proof: unit tests on known PowerPC words (`add`, `lwz`, `stwu`, `bl`, `bctr`, a VMX128
   op), checked against the emulator's results on the same words. No title is touched.
   What step 1 shows about the description, which shapes step 2:
   - Integer, load, store and branch instructions are a few operations each, on the description's own registers. For example,
     `lwz` is `INT_ADD`, `LOAD`, `INT_ZEXT`. `cmpwi` reads r3's low word as a part of r3 (4 bytes at r3's offset + 4).
   - A condition field is one packed byte, `cr6 = lt<<3 | gt<<2 | eq<<1 | so`, built from shifts and ORs. The context keeps four
     bytes per field (`ctx.cr6.eq`), so the register table says how each description register is read and written in the context
     (packed and unpacked for CR, XER's bits, the FPSCR).
   - Every floating-point operation also updates about 25 single-bit FPSCR registers (`fp_fx`, `fp_vxsnan`, ...). XenonRecomp
     does not model them and no title has needed them. The register table marks such registers as not kept: their writes are
     dropped and their reads are an error, so dropping them is never silent.
   - `bl` also copies r2 to a description register, `r2Save`, that the context does not have. It is marked as not kept too.
   - VMX128 operations are user operations (`vmaddfp128` is `CALLOTHER vectorMultiplyAddFloatingPoint` on `vs32`), served by
     `pcode_ops_ppc_vector`: the code step 2 shares with the emulator.
2. **The p-code translator, beside XenonRecomp.** `recompile/emit` writes a function from p-code in today's output form.
   Proof: Xenia's instruction tests and ours (`ppc_tests`, 162 files with the console's results) through the new translator,
   0 mismatches. The 8 tests XenonRecomp skips are counted, since the description may cover them. No title is touched, because
   XenonRecomp still writes their code.
3. **The titles on the new translator, behind a switch.** `recompile-project --translator pcode`. Proof: Minecraft and Up (and
   Tetris once it is back) `--check` as in the baseline, and frame times no worse than the baseline's. Then a person plays each
   title. Where a title differs, the debugging method in docs/RECOMPILE.md applies: swap function bodies between the two
   translations in bulk and bisect.
4. **The new translator is the default. XenonRecomp's translator, XenonAnalyse and binutils' decoder retire.** Before this:
   `switches` finds every table XenonAnalyse added on the three titles, and `xenon-recompile` is served by the new translator,
   with the Dashboard's 7 modules giving the same `ppc/` as the step 3 build. Proof: the step 3 checks, run again, and Logan's
   go-ahead for the Dashboard. The memory note *xenonrecomp-one-home* says to delete nothing of the Dashboard's line until it
   builds from here and Logan agrees.
5. **rexglue and the builders retire.** The `recompile` command (one function, `--all`, `--check`) runs the one translator.
   `--runtime rexglue` goes away, and with it `src/recompile/rexglue` and `src/recompile/builders`. Proof: the `recompile` unit
   tests rewritten on the one translator, and the three titles' generated C++ byte-identical to step 4's, since this step does
   not touch the project path.
6. **On L2.** The emitter builds L2 (`MachineFragment`, `Goto`, `Labeled`) and L2's C++ back end writes it, with the rule set
   `machine`. Proof: the three titles' generated C++ byte-identical to step 5's (or each difference reviewed and the titles
   checked again), and the instruction tests at 0 mismatches.
7. **A second CPU.** Its description, register table and runtime. The CPU is chosen with Logan. Proof: a program for it
   recompiled and run with the same result as on the emulator.

## Status

- 2026-11-28: design written.
- 2026-11-28: step 0, Minecraft: `recompile-project "C:/Nexia360/Games/Minecraft Xbox 360 Edition" --check 120` from this branch
  found 43,128 functions, 213 switch tables and no unimplemented instruction. It built, presented 6,600 frames in 120 s (the
  ceiling) with no stop, and exited 0. The whole run took 689 s. The digests of its 175 generated files are kept with the run.
- 2026-11-28: step 0, Up: `recompile-project "C:/Nexia360/Games/Disney-Pixar Up (USA).iso" --check 120` found 29,618 functions,
  461 switch tables and no unimplemented instruction. It built, presented 6,600 frames in 120 s with no stop, and exited 0 (649 s
  in all; the digests of its 123 generated files are kept). Tetris cannot be checked: its package is gone from the machine. Step 0
  is done for two of the three titles.
- 2026-11-28: step 1: `sleigh::Reader::pcode` gives an instruction's operations as data (`Op`, `Varnode`, `PcodeOp`, `Lifted`), with
  `register_name`, `register_named`, `user_op_name` and `space_name`. Proof: `recompile.pcode.*`, 5 tests and 35 checks, on
  `ppc_64_xenon_be.sla`: add, lwz, stwu, cmpwi, bl, blr, bctr, beq, vmaddfp128, sync, a word that is not an instruction, and
  add, lwz and stwu run on the emulator with the results the operations name. No title is touched: nothing on the project
  path calls the new API.
- 2026-11-28: step 2: the p-code translator (`recompile/pcode_emit`) beside XenonRecomp. XenonRecomp keeps the function around each
  instruction (labels, branches, calls, switch tables, the flush mode, its locals) and asks the translator first for every instruction
  (`XenonRecomp/instruction_translator.h`); the translator writes the instruction from its p-code or declines it (a way out of it, a
  user operation such as VMX128's arithmetic, a register the context lacks, a fused multiply-add, a store before `eieio`), and
  XenonRecomp writes what it declines. The description is `ppc_64_xenonexact_be.sla`: `ppc_64_xenon_be.slaspec` with `XENON_EXACT`,
  the console's 32-bit mode for record forms and carries, `divw`, `mulhw` and `lvebx`/`lvehx`/`lvewx` as the console's results
  (and XenonRecomp's own tests) give them (docs/GHIDRA_FORK.md L12); the decompiler's descriptions are unchanged but for line
  numbers. Proof: `recompile.x360.xenias_instruction_tests_through_the_pcode_translator`: every test file, 0 mismatches, 2,767
  instructions written by the translator and 1,608 by XenonRecomp (1,279 of them `blr`). On the way: the instruction tests had not
  built since a rename reached the harness's generated `main.cpp` (`xenon_log::println`), and a build failure was reported as a
  skip, so the test passed with nothing checked; both fixed, and the XenonRecomp-only run is 0 mismatches again.
