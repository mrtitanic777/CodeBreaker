# wasm VxD port — assembling the Win95 ring-0 drivers with a from-source toolchain

Step 2 of the premise, for the **ring-0 VxD class**: take the reconstructed VxD
source in `../../../Windows95/vmm32/<name>/<name>.asm` (idiomatic DDK-macro form)
and assemble it with a **maintainable, non-Microsoft toolchain we control** —
OpenWatcom's `wasm`, built from source at `open-watcom-v2/` — instead of the
period Microsoft `ML.EXE`.

Each driver is one `.asm` that `include`s the stock DDK `INC32` headers
(`vmm.inc`, plus one per foreign device it calls) and uses the MASM 6.11 macro
set (`Declare_Virtual_Device`, `Begin_Control_Dispatch` / `Control_Dispatch` /
`End_Control_Dispatch`, `Begin_Service_Table`, `BeginProc`/`EndProc`,
`VxD_*_SEG`, `VMMcall`/`VxDcall`). `wasm` did not support several of those macro
behaviours, and rejected some instruction encodings MASM accepts. This document
lists what we changed and where things stand.

## Status

**222 / 222 drivers assemble with 0 errors** through the from-source `wasm`.
All 24 minimal repros green; the `wasm` tree builds clean.

> Assemble 32-bit VxDs with **`-dIS_32=1`** (the standard DDK 32-bit-VxD define,
> normally on the `ML` command line). Without it, headers whose service tables
> are `ifdef IS_32`-guarded (e.g. `vcond.inc`) skip the table, so a driver that
> `VxDcall`s a service from such a table fails with `E074 Constant operand is
> expected` on the missing `@@<service>` ordinal (this was `shell`'s last error;
> it is a build-flag issue, not a `wasm` bug).

Notably this is **ahead of stock `ML.EXE` 6.13**, which cannot even parse the DDK
headers (it dies in `vmm.inc` at a nested-macro `&`); the authentic byte-exact
build used the 1996 DDK's `ML 6.11`. So the from-source path is now the more
capable one for this codebase.

Build/run:

- Rebuild `wasm` after patching its C source:
  `cd open-watcom-v2 && . ./setvars.sh && cd bld/wasm && builder rel`
  (recompiles changed `c/*.c`, copies `nt386/wasm.exe` → `rel/binnt/wasm.exe`).
  Touching the token tables (`h/asminsd.h`, `inlnops.tok`) shifts the token enum —
  do `rm nt386/*.obj nt386/*.gh` first.
- Assemble a driver:
  `wasm -i=<space-free INC32 dir> -fo=out.obj <name>.asm`
  (`-i=` with spaces in the path crashes `wasm`; copy `INC32` to a space-free dir.)
- Batch harness (pass/fail + ranked error signatures): see the temp
  `vxd_batch/run.sh` used during the port.

## Toolchain notes

- The **authentic** byte-exact verification uses Microsoft's DDK tools
  (`ML 6.11` + DDK `LINK 2.60.5046` + `RC 3.11` + `ADRC2VXD`); that is the
  byte-diff check, not the deliverable. This `wasm` track is the
  toolchain-we-control build (the premise's "no MS tools" requirement).
- `ML.EXE 6.13` (`../../../Microsoft-Assemblers-Compilers/binaries/masm-6.13/`)
  is still useful as an **arbiter for isolated instruction/directive questions**
  — run it via a Windows-path invocation with `/c /Fl<lst> /Fo<obj>`; a
  `/`-style path makes it choke. Every leniency change below was checked against
  its listing to stay byte-exact.

## `wasm` source changes

All in `open-watcom-v2/bld/wasm/`. The OW tree is third-party / git-ignored, so
these live only as local patches; this doc is the record of them.

### Macro / pass-model (the DDK macro set)

The root cause behind most of these: **`wasm` persists symbols across passes**,
whereas MASM re-scans the source and rebuilds symbols on every pass. Three
surgical per-pass fixes bridge that instead of a full re-scan rewrite.

1. **Purged-macro restore** — `c/asmsym.c` (`AsmPurgeMacro`/`AsmRestorePurgedMacros`),
   `c/breakout.c` (PURGE), `c/write.c` (pass loop), `c/direct.c` (`dir_requeue`).
   `PURGE` now stashes the macro instead of freeing it and restores it at the
   start of the next pass, so a macro purged mid-pass (the DDK's
   `Control_Dispatch`, purged by `End_Control_Dispatch`) is present again when
   its guarded `include` is skipped on pass 2.
2. **`X EQU macroname` alias** — `c/macro.c` `ExpandMacro` (added `T_EQU`/`T_TEXTEQU`
   to the no-expand switch). Don't invoke a macro named on an `EQU` right-hand
   side; store it as a text alias (`VxD_ICODE_SEG equ VxD_INIT_CODE_SEG`).
3. **Include-guard underscore tokenization** — `c/condasm.c`
   `prep_line_for_conditional_assembly` (added `_` to the first-word scan). A
   symbol like `IF_BIT` was truncated to `IF` and matched as the `IF` directive,
   so it was not blanked inside a skipped block and executed anyway — breaking
   `ifndef _VMM_` double-include guards.
4. **Per-pass reset of main-file `EQU` constants** — `c/asmsym.c`
   (`AsmResetPassConstants`, walks the symbol hash), `c/asmexpnd.c`
   (`main_file` flag on `const_info`), `c/direct.c` (init). Drops top-level `EQU`
   constants at each pass start so source order re-establishes them, matching
   MASM. Fixes order-dependent `IFDEF`s such as `Create_<name>_Service_Table`,
   which otherwise persisted and made the external `Begin_Service_Table` take the
   create path on pass 2 (double-defining the service table).
5. **Macro redefinition across passes** — `c/macro.c` (`reset_macro_body`, called
   from `MacroDef` on pass > 0). A macro re-defined with a different body at a
   different source point (the DDK's `VKD_Service`, defined one way externally and
   another in the create path) now re-stores its body instead of keeping the
   stale one. Also removed a phase / non-convergence that stale bodies caused.
6. **Same-value `EQU` redefinition** — `c/asmexpnd.c` `createconstant`. MASM
   allows re-`EQU`ing a numeric to the **same** value (only a different value is
   an error). The DDK headers rely on it (`VCOND_DEVICE_ID EQU 38h` in both
   `vmm.inc` and `vcond.inc`). Permit an identical single-number redefine.

Earlier port work (prior sessions): `_FLAT` segment option, `&&` collapse,
`.xcref` name handling, MASM `UNION`, redefinable text equates, `.errb`/`.errnb`
text expansion, segment-nesting counter, `OFFSET32`, and the use-after-free crash
fix in `AsmTakeOut` (unlink from the `Tables[TAB_MACRO]` queue before freeing).

### Instruction encodings (`wasm` was stricter than MASM)

All verified byte-exact against `ML 6.13`'s listing.

7. **`BOUND reg32, qword ptr [..]`** — `c/asmins.c` (skip the generic same-size
   check for `T_BOUND`). BOUND's memory operand is legitimately twice the
   register width (`m32&32` = qword). Cleared 25 drivers.
8. **`LOCK` prefix leniency** — `c/asmmatch.c` (`T_LOCK` no longer errors on
   instructions whose form lacks `allowed_prefix == LOCK`). MASM emits `F0` for
   any `lock`; `wasm` rejected e.g. `lock and eax, [eax]`. Cleared 5.
9. **`RET`/`RETF` unsigned imm16 and shift unsigned imm8** — `c/asmins.c`
   `idata_nofixup`. RET/RETF stack-adjust is an unsigned `imm16` (not signed →
   not `I32`); a shift/rotate count is an unsigned `imm8` (never sign-extended,
   independent of destination size). Narrow ranges only, so `add eax,0FFh`
   sign-extension and the `shl eax,1` short form are untouched.

10. **`pushaw` / `popaw` mnemonics** — `c/asmscan.c` (after `get_instruction`).
    MASM has these 16-bit `PUSHA`/`POPA` spellings (`66 60` / `66 61` in a USE32
    segment); `wasm`'s table has only `pusha`/`popa`, which are *already* the
    `F_16` (16-bit, 66-prefixed) forms. When the lookup misses, alias `pushaw`→
    `pusha` and `popaw`→`popa`. Chosen over adding tokens `T_PUSHAW`/`T_POPAW`:
    new tokens must go in **both** `inlnops.tok` and `h/asminsd.h` in matching
    enum order or `mkopcode`'s consistency check fails (`Wrong data in
    asminsd.h`) — fragile for a 2-driver payoff. Fixed `declicl`, `el59x`.

Note: the operands in the BOUND / LOCK / shift / pushaw-popaw cases are often
garbage — data regions the disassembler decoded as instructions. They assemble
byte-exact so they do not block the build; the readable-source lane would have
the decoder emit `db` there instead. `shell`'s remaining error turned out to be
the missing `-dIS_32=1` build flag (see Status), not a `wasm` gap.

## LE-link lane

The from-source `wlink` (`rel/binnt/wlink.exe`) links a `wasm` OMF `.obj` into a
real Win95 **VxD LE** module. The recipe:

```
wlink format windows vxd [virtdevice]
      option quiet
      name <drv>.vxd
      export <NAME>_DDB          # the DDB from Declare_Virtual_Device <NAME>
      file  <drv>.obj
```

- `format windows vxd` selects the Windows-386/VxD LE (`os_type = 4`), not the
  OS/2 LE that `format os2 le` gives.
- `export <NAME>_DDB` — a VxD LE requires **exactly one export, the DDB**. wlink
  reads it (device id, DDK version) into the header and locates it for VMM; the
  entry point stays `0:0` (VMM finds the DDB via the export, like the retail
  `.vxd`s).
- Module type (LE module flags): `0x28000` = **static** (boot-loaded, the
  default) / `0x38000` = **dynamic** (VXDLDR-loadable, via `virtdevice`). This is
  a build-time property — it is **not** derivable from the DDB flags (verified:
  oracle `ati.vxd` is dynamic yet its DDB `DDB_DYNAMIC_VXD` bit is clear). The
  build picks it per driver from the retail `.vxd`'s own module flag.

Verified against retail VxDs (`Windows95/design/corpus/files/WINDOWS/SYSTEM/`):
`combuff` → `0x28000`, `ati` (+`virtdevice`) → `0x38000`, both matching. os_type,
entry `0:0`, cpu, page count all match.

**Batch link: 222 / 222 → VxD LE**, module type matches retail 190 / 190 (0
mismatches; static/dynamic picked per driver from the retail `.vxd`'s module
flag). `VKD` (the boot target) links to `0x28000` static, matching its retail
module.

The 3 that first failed (`drvspace`/`DSVXD`, `deccore`, `vnetsup`) were not a
linker problem: their idiomatic `.asm` in the repo were **stale** — generated
before the disassembler's `le.ddb()` gained a fallback for non-standard DDK
version stamps (`0x0300` DSVXD, `0x030b` VNetSup/DECCORE), so they carried no
`Declare_Virtual_Device` and thus no exportable DDB. Regenerating them with the
current engine (`vxd_idiom.emit_idiomatic_tree`) restores the DDB; then wasm +
`wlink format windows vxd / export <NAME>_DDB` produce valid VxD LE (DSVXD 4.9 KB,
VNetSup 16 KB, DECCORE 14 KB, all os_type=4). drvspace's oracle is
`setup/inf/drvspace.bin` — an MZ with an embedded LE whose DDB is `DSVXD`.

> Engine gotcha: `src/core/paths.py` `data_root()` defaults to the parent of the
> repo checkout, so the DDK `INC32` isn't found via the direct candidates and
> `vxd_services.load()` (run at import of `vxd_asm`) falls back to a recursive
> `**` glob over that whole folder — which crawls every sibling repo and appears
> to hang. Set `WT_REFERENCE_ROOT=<...>/Windows95/design` (and `WT_DATA_ROOT`)
> before running the engine. (Worth fixing the default / bounding that glob.)

### wlink source changes (also third-party / git-ignored — recorded here)

- `bld/wl/c/loadflat.c` `SetHeaderVxDInfo` / the VxD flags block: default the VxD
  module type to `VXD_DEVICE_DRIVER_STATIC` (0x28000) instead of the generic 3.x
  flags (0x8020); a DDB with `DDB_DYNAMIC_VXD` (bit 15) set → dynamic. (Values in
  `bld/watcom/h/exeflat.h`.)
- `bld/wl/c/cmdos2.c`: expose `VIRTdevice`/`PHYSdevice` under `format windows
  vxd` — they were `MK_OS2_FLAT`-only, and `ProcWindowsFormat` only ran the
  option loop for `MK_WIN_NE`; now also for `MK_WIN_VXD`. `virtdevice` → dynamic,
  `physdevice` → static.

### Still open (refinement, does not block loading)

Object-table grouping differs from retail: our link emits fewer objects, all
`exec+read` (`0x2045`), where retail separates an `INIT` (discardable, `0x2015`)
and a writable `DATA` (`0x2023`) object. VMM maps all VxD memory RWX at ring 0,
so this loads; matching it exactly needs segment→object ordering control. The
DDB entry, module type, and os_type — the parts VMM checks to load a VxD — are
correct.
