# MS C 4.0 — Original Source-File Map (from embedded `@(#)` markers)

Every C 4.0 tool embeds SCCS `@(#)what` version strings — one per translation unit — carrying the
**original `.c`/`.y` filename and its SCCS revision**. Harvested directly from the unpacked
binaries (`workspace/scripts/carve.py` string output + a `@(#)` scan). This is ground-truth
attribution: it tells us, per binary, exactly which authored source modules were linked in, so
reconstruction proceeds **per module**, and a module shared across tools need only be solved once.

Build stamp found in MSC/CL: **`4.02 Mon Jun 23 10:13:47 PDT 1986`**; banner
**`Microsoft (R) C Compiler Version 4.00`**, `Copyright (C) Microsoft Corp 1984, 1985, 1986`.

## The compiler pipeline (what each EXE is)

```
  MSC.EXE / CL.EXE   driver ("cc")  ── spawns ──▶  c1 ─▶ c2 ─▶ c3  (─▶ masm/link)
       │                                          front    optim   codegen/OMF
   cc_main.c                                       end
```
The driver strings reference `c0.exe c1.exe c2.exe c3.exe masm.exe link.exe` and pass an
**intermediate language (IL)** between passes (`Prep_il`, `Init_il` temp files; the shared
`il.c`/`initil.c`/`sil.c`/`dbil.c` modules).

| EXE | Role | main unit | # units |
|---|---|---|---:|
| MSC / CL | driver (`cc`) | `cc_main.c:1.28` | 7 |
| C1 | front end: preprocess + lex + parse + IL emit | `main1p.c:1.18` | 40 |
| C2 | optimizer + global codegen | `main.c:1.30` | 48 |
| C3 / C3L | final codegen + peephole + OMF `.obj` writer | `main.c:1.24` | 35 |
| LINK | segmented linker | `main.c:1.1` (rev 3.x lineage) | 24 |
| LIB | OMF library manager | `libomp.c:1.10` | 6 |
| MAKE | build tool | `make.c:1.23` | 3 |
| EXEPACK | the packer | `exepack.c:2.23` | 4 |
| EXEMOD | header modifier | `exemod.c:1.8` | 1 |
| SETENV | env sizer | `setenv.c:1.2` | 2 |
| LINK4 | Win1.x linker | `main.c:1.1` | 5 |
| CV/CVR | CodeView | (no `@(#)` markers — stripped) | — |

## Per-EXE module lists

**Driver — MSC:** cc_main.c·msc_msdos.c·err_msg.c·error.c·flags.c·getflags.c·strings.c
**Driver — CL:** cc_main.c·cc_msdos.c·err_msg.c·error.c·flags.c·getflags.c·strings.c
> MSC and CL are the **same driver** (`cc_main.c:1.28`) differing only in the DOS-glue unit
> (`msc_msdos.c` vs `cc_msdos.c`). Reconstruct once, diff the glue.

**C1 (front end, 40):** const·dbil·debug·err_msg·error·esu·fatal_msg·float·getflags·**grammar.y**·
gramut·hash·il·initil·io·ioin·keywords·lex·ltoa·main1p·memallo·p0expand·p0expr·p0human·p0io·
p0prepro·p0scanner·p1_debug·p1_init·pragma·strings·symbols·to_dbil·toil·toinil·tokens·trees·types·
version·warn_msg
> `p0*` = preprocessor; `lex/keywords/tokens` = scanner; **`grammar.y:1.35`** = the **yacc grammar**
> + `gramut`; `trees/types/symbols/hash` = the AST/semantic core; `toil/toinil/to_dbil/il/dbil` =
> IL emission (incl. CodeView debug IL).

**C2 (optimizer + codegen, 48):** MD·MDmisc·ail·andor·bitfields·canonical·**codegen.x**·confold·cse·
csize·ctypes·debug·emit·err_msg·error·expand·exphelp·fatal_msg·gencode·genswitch·getattrib·
getflags·grammar·il·initil·interpret·io·ioin·ltoa·main·memallo·memsize·opMD·optimize·optstring·
p2pragma·p2symtab·pragma·pril·regMD·sil·sortnode·su·suMD·switchMD·tables·trees·warn_msg
> `optimize/cse/confold/expand/canonical/sortnode` = the optimizer; `regMD/suMD/switchMD/opMD/MD` =
> the machine-dependent (8086) layer; `emit/gencode/genswitch` = code emission;
> **`codegen.x:1.57`** (`.x` = a generated table file).

**C3 / C3L (final pass, 35 each — identical set; C3L = large-model build):** MD·ail·check_mode·
code·csize·dbil·dbread·debug·disasm·err_msg·error·fatal_msg·getflags·initil·io·ioin·labels·ltoa·
main·memallo·**omf**·**omf8086**·opMD·p3debug·p3io·p3symtab·pgo·pgoMD·pragma·sil·srclist·tables·
type·warn_msg
> **`omf.c:1.16`/`omf8086.c:1.41`** = the OMF `.obj` writer; `pgo/pgoMD` = peephole;
> `disasm.c:1.23` = the built-in disassembler (for listings); `srclist` = source listing.

**LINK (24):** alloc·fprintf·main·newadr·newcmd·newdeb·newin2·newini·newlib·newmap·newout·newpar·
newpri·newrel·newsta·newsym·newtp1·newtp2·newtrn·newupd·newutl·pack·stdio20·vm
**LIB (6):** liblup·libomp·newcomm·newcrf·newmas·newvmm
**MAKE (3):** make·mamsbnr·syserr **· EXEPACK (4):** bufio·epmsbnr·exepack·syserr
**EXEMOD (1):** exemod **· SETENV (2):** semsbnr·setenv **· LINK4 (5):** alloc·fprintf·main·stdio20·version

## Shared modules → cross-tool identification

These units appear in multiple EXEs and compile to (near-)identical code — solve once, then
**identify** the twins everywhere else (cross-pass libsig):

| Module | Appears in | Note |
|---|---|---|
| `error.c:1.4` | MSC CL C1 C2 C3 | error core |
| `err_msg.c` | all | 1.8 in drivers, 1.10 in passes (different rev → different bytes) |
| `fatal_msg.c:1.5`, `warn_msg.c:1.14` | C1 C2 C3 | message tables |
| `getflags.c:1.30` | MSC CL C1 C2 C3 | option parser (same rev everywhere) |
| `strings.c:1.1`, `ltoa.c:1.1` | many | tiny helpers |
| `io.c:1.2`, `ioin.c:1.1` | C1 C2 C3 | I/O layer |
| `il.c:1.6`, `initil.c:1.1`, `sil.c:1.3`, `dbil.c:1.6` | C1/C2/C3 subsets | the IL passed between passes |
| `MD.c:1.1`, `opMD.c:1.1`, `csize.c:1.4`, `ail.c:1.4`, `tables.c:1.4` | C2 C3 | machine-dependent shared |
| `memallo.c:1.6`, `debug.c:1.8`, `pragma.c:1.6` | C1 C2 C3 | runtime |
| `alloc.c`, `fprintf.c`, `stdio20.c:1.5` | LINK LINK4 | linker runtime |
| `syserr.c:1.1` | MAKE EXEPACK | |

**Reconstruction consequence:** cluster carved functions by owning module (the `@(#)` string is
usually emitted near its module's data), reconstruct a module once, and propagate the identification
to every tool that links it — a large multiplier on coverage.
