# XenonRecomp in CodeBreaker: what is changed from upstream

This is hedge-dev's XenonRecomp (MIT, upstream at `ddd128b`, "Update outdated info in README") with the changes
CodeBreaker's Xbox 360 projects and the Xbox 360 Dashboard need. It is the one XenonRecomp: the dashboard builds from
it (its own copy in `Dashboard/Shared/tools/XenonRecomp`, branch `dashboard`, was merged here on 2026-10-23 at
its `b904d22`).

The instruction tests are `ppc_tests` (Xenia's, and ours in `ppc_tests/own`), run by `codebreaker/recompile/xenon.py`:
162 tests, 0 mismatches.

## From the Xbox 360 ports (HijackAssassin)

- Instructions used by Minecraft TU46; `bdnzf` honours BI.
- `vpkd3d128` / `vupkd3d128` in full, `vcmpbfp` (and 128), `blrl`.
- `mftb` reads a 50 MHz time base (`PPC_QUERY_TIMEBASE`).
- Switches use the index's low 32 bits and jump through CTR for unlisted cases.
- Guest memory goes through `PPC_HOST` (the 0xE0000000 window offset).
- Function analysis: `bdzlr` / `bdnzlr` are conditional; a branch to the next instruction.
- Fused `fmadd` / `fmsub` / `fnmadd` / `fnmsub` (one rounding, PowerPC NaN rules); VMX `vmaddfp` stays unfused.

## From the Xbox 360 Dashboard

- Module symbol suffix (`_xstart`, the save/restore helpers, `PPCFuncMappings`) and an overridable `PPC_LOOKUP_FUNC`,
  so several recompiled modules link into one program; `suffix_function_names` puts the suffix on `sub_<address>`.
- Import blocks walked by size and named by index; the import library name index is its low byte (early devkit
  dashboards).
- Instructions of dash.xex (2.0.6719 to 17559) and the 17559 system modules: `rlwimi`'s high word and record form,
  `rldicl.` sets CR0, `vrlw`, `vsubsbs`, `vcmpgtub.` / `vcmpgtuh.` set CR6.
- `ppc_context.h`: fused multiply-add on builds below x86-64-v3.
- Non-volatile registers as locals, but kept in the context for SEH parents, their funclets and their callers.
- Tail calls: a count-down branch out of its function, and a conditional branch out of the region.
- Entries for callbacks that only data points at, and for SEH filter funclets inside other functions.
- Section sizes clamped to the image (XAM 8495 and later).
- `relocatable_code`: layout-dependent values read from per-function arrays (`PPC_RI` / `PPC_RL`), labels as offsets,
  direct calls through the indirect-call table, a `//@R` line per function (the unified dashboard DLLs need it).
- `db16cyc` is `PPC_SPIN_HINT()`, a no-op unless the runtime defines it.

## From CodeBreaker (Tetris, Minecraft TU0, Up)

- A function that ends on an instruction the CPU steps past (anything but `b`, `bctr`, `blr`) continues into the
  function after it (Up: 191 functions stopped short).
- An entry XenonRecomp finds itself is analysed only up to the next known function; a switch's case labels are never
  entries.
- A branch to an address where no function is known calls through the indirect-call table (it was a comment, and the
  call vanished).
- Divides never fault the host: a zero divisor (or the most negative value over -1) gives 0; a word divide
  zero-extends its quotient.
- `frsqrte` / `frsqrtes`.
- The data-pointer scan stops at the image's end.
- A missing configuration file is reported.
- Under `relocatable_code`, the two calls above (into the next function, to an unknown address) go through
  `PPC_LOOKUP_FUNC` / the indirect-call table like every other call.

CR, CTR and XER in the context (Up's functions entered by a branch read the flags and CTR their caller set) is a
configuration choice (`cr_as_local = false` and the rest, written by `codebreaker/recompile/config.py`), not a code
change: the dashboard's modules keep theirs as locals.

## Built into CodeBreaker (2026-11-20)

CodeBreaker links XenonRecomp and XenonAnalyse into `codebreaker.exe` (MSVC, clang-cl or GCC) and calls them in-process
(`src/recompile/xenon_engine`); the programs still build on their own.

- Builds with MSVC: `_byteswap_*` where there is no `__builtin_bswap*` (`XenonUtils/byteswap.h`).
- Builds with GCC: `_XXOVERLAPPED`'s `Error` and `Length` (`XenonUtils/xbox.h`) and `XDBFTitleID`'s `u16` (`XenonUtils/xdbf.h`)
  are plain words; GCC refuses a member with a constructor (`be<>`) in an anonymous struct, and no tool reads them.
- The same code whichever compiler builds it: the alias form (`XENON_RECOMP_USE_ALIAS`) is chosen for the compiler of
  the code it writes (clang), not for its own; upstream switched it off when XenonRecomp itself was not built with clang.
  MSVC and clang-cl builds write byte-identical output (397 files of 7 Dashboard modules, `title_zune` and `xam` among them).
- The programs are libraries too: `LibXenonRecomp` (`xenon_recomp_main`, `XenonRecomp/xenon_recomp_main.h`) and
  `LibXenonAnalyseMain` (`xenon_analyse_main`, `XenonAnalyse/xenon_analyse_main.h`); the executables are the same code.
- Messages go through `XenonUtils/xenon_log.h`: stdout, or a string the caller collects them in (per thread).
- A missing configuration is an error the caller gets back (an exception, exit code 1), not `std::exit`.
- An input that is not an XEX or ELF image (missing, empty) is refused with an error instead of read past its end
  (`Image::ParseImage`, both programs).
- XenonAnalyse's output buffer is cleared at the start of each run (it is a file-level static).
- Inside another build the programs (and xxhsum) build on request only.

## The one recompiler, step 2 (2026-11-28)

- An instruction translator the caller sets (`XenonRecomp/instruction_translator.h`, for the runs of one thread) is asked
  for every instruction first, with the names the configuration gives each register. Where it writes the instruction,
  its statements stand in for the `switch`'s (the flush mode set first when it computes with the FPU). Where it declines,
  or when no translator is set or the code is `relocatable_code`, the output is what it was. CodeBreaker's p-code
  translator (`src/recompile/pcode_emit`) is the one it is given (docs/RECOMPILER.md).
- The instruction tests' harness printed its mismatches with `xenon_log::println`, which the generated `main.cpp` does not
  have (a rename of fmt's `println` had reached the text it writes), so the tests no longer built: it is `fmt::println` again.
