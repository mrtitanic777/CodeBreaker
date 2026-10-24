# XenonRecomp in CodeBreaker: what is changed from upstream

This is hedge-dev's XenonRecomp (MIT, upstream at `ddd128b`, "Update outdated info in README") with the changes
CodeBreaker's Xbox 360 projects and the Xbox 360 Dashboard need. It is the one XenonRecomp: the dashboard builds from
it (its own copy in `Dashboard/Shared-unified/tools/XenonRecomp`, branch `dashboard`, was merged here on 2026-10-23 at
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
