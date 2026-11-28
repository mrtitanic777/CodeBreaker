# One engine per role: the six unification items

Approved by Logan on 2026-11-26 ("Let's do ALL of these!"), under the standing rule of one engine per role: one core plus descriptions,
and every language through L2. The items are done one at a time, ahead of track 1 of docs/ROADMAP.md. Items 4 and 1 come first,
because they decide the shape of the ILSpy port while it's still being built. This file says what each item replaces, how it's done,
and what proves it. Each item's section gets its status as it lands.

## What there is today (2026-11-26)

| Role | Engines today | Lines |
|---|---|---|
| Bytecode decompiler | `src/java/decompiler` (the Fernflower port: Java text), `src/dotnet/decompiler` (the ILSpy port, stages 0-7 so far: C# text) | 24,500 + 23,000 |
| The translator's tree | `translate::ir` (typed nodes, `ir.hpp`; L2's JSON is `l2.hpp`), **and** `cb_l2`'s own tree (`translate/l2/src/l2.hpp`: Python-shaped values, `py::V`), joined by a JSON call protocol (`native_l2`) | |
| Translator back ends | `cpp03` (Java), `cpp17` (shim) + `cb_l2`'s `cpp17_core` with rules `cpp17_java`, `cpp17_python`, `csharp_cpp17` (C#), `c` + `c_java` (Java) + `cb_l2`'s C back end (binaries), `csharp` + `csharp_java`, `java`, `go_java`, `rust_java`, `asm` | 28,700 + 15,200 |
| Emulators | `emulate/pcode` (SLEIGH p-code) and 25 hand-written: a64, allegrex, arm, cp1610, ee, f8, huc6280, i8048, m6502, m6809, m68k, mips, powerpc (+fpu, gekko), r4300, rsp (2026-11-26), sh2, sh4, sm83, tlcs900, v810, w65816, z80; Unicorn as the x86 oracle | |
| Assemblers | `decompile/engine/sleigh_assembler.cc` and seven of their own: a64, arm, cp1610, mips, ppc, sh, sm83 (`src/build/*_asm`), with x86asm (MASM-compatible) | |
| Static recompilers | `recompile/x360`, `recompile/xenonrecomp`, `recompile/rexglue` | |
| Compilers | the OpenWatcom fork (wcc/wcc386), x86cc, each with its own middle | |

The finding that shapes item 1: the "pair" back ends already read L2. `translate::ir` is L2's tree in memory, and `l2.hpp` is only
its JSON form. A back end like `go_java` is called that because Java is the only source with rules for it, not because it reads a
second IR. So most of item 1 is two things. First, put one tree in place of two: `cb_l2` keeps its own Python-shaped copy of L2 and
a JSON protocol to reach it. Second, split each back end into its target's emitter plus one rule set per source language, as `cpp17`
already is (`cpp17_core` + `cpp17_java` + `cpp17_python`).

## 4. One decompiler core (with the ILSpy port session)

JVM and CIL are both stack bytecode. Both decompilers end in L2, and the Java and C# text comes from L2's back ends.

- **The front halves stay**: Fernflower's control flow, variables and expression rebuilding, and ILSpy's ILAst, control flow, the 39
  transforms and the type system (stages 4-7, ported and matching ILSpy). Each lowers its result into `translate::ir`.
- **The text halves become back ends**: Fernflower's writer (`class_writer`, the exprents' `to_java`) becomes the Java back end's
  layout. ILSpy's C# syntax tree, output visitor and C# transforms (stages 7 and 10) become L2's one C# back end (an L2 → C# syntax
  tree lowering in front of them), and `backends/csharp`'s own emitter retires.
- **Everything through L2, no trade-offs (Logan, 2026-11-26)**: .NET → C# goes through L2 too. ILSpy's stages 4-6 are the one CIL
  analysis. After stage 6 the ILAst is lowered to L2, and that lowering loses nothing ILSpy's builders read: stack types,
  signedness and lifting, the chosen overload and call form, conversions and their kind, the type system's entities, and IL offsets.
  What L2 has no slot for is added to L2 (`ir.hpp`, the schema), never kept on the side. ILSpy's builders (stage 9), resolver (stage
  8), C# transforms (stage 10) and output visitor (stage 7) are the C# back end, reading L2. The same back end writes C# for every
  other front end, taking the plain form where an IL fact is absent. ilspycmd is ported too (its command line and project output,
  stages 11 and 12), so nothing needs .NET. The installed ilspycmd is only the oracle: its output byte for byte, through L2.
- **L2 grows what the two need**: properties, events, indexers, operators, attributes, generic constraints, `yield`/`await` (after
  ILSpy's state-machine transforms), pattern matching, records, local functions, `using`/`lock`/`fixed`, checked arithmetic,
  `ref` locals and returns, and Java's anonymous classes, lambdas and switch-on-string after Fernflower's passes.
- **Proof**: `ilspycmd`'s C# byte for byte on the ILSpy port's test assemblies, through L2, the same as the port proves today. And
  the Java lane's tests: Fernflower's text byte-identical on RetroMCP's jar, and the Minecraft versions still converting and building.
- **The ILSpy port's stages for it** (agreed 2026-11-26), in this order, so each switch-over is checked against an output that is
  already exact:
  - **9a.** ILSpy's builders on the ILAst exactly as upstream, byte for byte against ilspycmd on the 112 Pretty cases and the real
    assemblies.
  - **10.** The C# transforms.
  - **11-12.** ilspycmd itself: its command line, `-p` project output, `-il`, `-t` and `-r`. Convert and the XNA lane then call ours.
  - **13.** L2 carries the structured ILAst losslessly. Added to `ir.hpp` and the schema, and reviewed against the other front ends so
    each new slot is general:
    - stack types and sign, lifted and checked flags;
    - call forms and conversion kinds;
    - address and ref semantics;
    - gotos and labels for what isn't structured;
    - `using`, `lock`, `yield`, `await` and pinned regions;
    - entities as type-system references;
    - variable kinds and IL ranges.

    The proof is a round trip: ILAst → L2 → ILAst gives the same ILAst text on all 122 assemblies.
  - **14.** The builders read L2, one node kind at a time. The proof is ilspycmd byte for byte through ILAst → L2 → C#, identical
    to 9a.
  - **15.** Java → C# through the same back end (Fernflower → L2 → ILSpy's C# back end); `backends/csharp`'s emitter retires.
- The ILSpy port session plans the C# side (asked 2026-11-26). This session does the Java side.

## 1. The translator hub: N front ends + M back ends

1. **One tree.** Port `cb_l2`'s C back end and the C++17 core with its rule sets from `l2::Node` / `py::V` onto `translate::ir`, and
   remove the JSON call protocol (`native_l2`'s `l2_call`). Proof: the C back end's and the C++17 goldens unchanged; `judge-py-cpp`
   and `judge-cs-cpp` agree as before.
2. **One back end per target language**, each an emitter plus rule sets chosen by `Unit.lang`:
   - C++: `cpp03`, `cpp17`, `csharp_cpp17` and `cb_l2`'s C++17 become one C++ back end with a standard level (03, 17) and rule sets
     java, csharp, python.
   - C: `c`, `c_java` and `cb_l2`'s C back end become one C back end with rule sets java and binary.
   - C#: `csharp` and `csharp_java` become one C# back end with rule sets java and cil (item 4).
   - Go and Rust: `go_java` and `rust_java` become the Go and Rust back ends, Java rules first.
   - Java: the Java back end, with rule sets c, cpp and bytecode (item 4).
3. **Delete the pair code** once its goldens and judges pass through the new back end: every `*_java` back end file and `csharp_cpp17`.

Proof for each step: the existing goldens (`translate.*`), the judges (`judge-c-java`, `judge-cs-cpp`, `judge-py-cpp`, the Java lane's
JVM comparisons), and the Minecraft versions converting and building as they do now.

## 2. One emulator

A translated-block cache for `emulate/pcode`: each basic block's p-code compiled once into a sequence of direct operations, with
constant-folded varnodes and precomputed space offsets, then cached by address and invalidated on a write to its bytes. The target is
to be at least as fast as the hand-written interpreter on each lane (68000, PS1, N64 and GBA were 1.1 to 2.7 times slower on
2026-11-21). Then each lane switches over one at a time. Its whole judging corpus must give the same verdicts. Then that lane's
emulator is deleted. The RSP (added 2026-11-26) is hand-written because there's no SLEIGH description of it: one is written as part
of this item, and the interpreter goes the same way. Unicorn stays as the x86 oracle unless the p-code emulator matches it.

## 3. One assembler

Each of a64, arm, cp1610, mips (with the RSP's vector unit), ppc, sh and sm83 folds into `sleigh_assembler.cc`. The text forms that
SLEIGH's syntax doesn't give (each lane's period dialect: GNU as, armips, rgbds) are kept by the dialect layer (`source/dialect`).
Before an assembler is deleted, all of its own test cases must rebuild byte for byte through SLEIGH. x86asm stays where MASM's
directives have no SLEIGH form. Its instruction encoding moves.

## 5. One recompiler

`recompile/x360`, `xenonrecomp` and `rexglue` become one static recompiler on the shared p-code / L2 core, with each console's
runtime beside it. Tetris, Minecraft and Up must still play on the Xbox 360 at every step. After that it takes any CPU with a
SLEIGH description.

## 6. The compiler middle on L2

One optimizer and code generator under every language, from L2 down: x86cc is the first back end, then OpenWatcom's code generator
for the 16- and 32-bit targets, and MSVC stays for x64 (the 2026-11-26 exception).

## Items 7-16 (Logan, 2026-11-27: "Yes, queue all")

These come from the Board session's survey of duplicates (`unification-survey-2026-11-27.md` in the session notes). They run after items 1-6,
one at a time, in this order. Each is proven by identical output on its existing tests before the old code is deleted, and each
tests only what it changes.

7. One source writer for every console, plus a description of each platform (~18,000 lines of per-console writers).
8. File detection from the `.fmt` descriptions.
9. The remaining hand-written readers onto the format engine: 7 ELF readers, MZ, NES, XEX, XBE, CAB, ISO, OMF and the Xbox
   containers.
10. One inverse compiler-model engine, with compiler versions as data (~28,000 lines).
11. Compiler-habit detectors as data.
12. The x86 32- and 64-bit pairs merged.
13. NID tables as data.
14. Small duplicates: ISO, XACT, ZIP, PNG, zlib, EXEPACK.
15. The Direct3D reference templates.
16. The Watcom linker's own loaders onto the shared readers.

## Status

- 2026-11-26: plan written; the inventory above.
- 2026-11-26: item 4: the ILSpy port's design for what L2 must carry for .NET (`ILAST_TO_L2.md`, in its work folder) is accepted
  as stage 13's plan, and its four decisions are answered: the size growth is allowed and measured, `is_signed` applies to every
  signed binary operator, decimal is the class `System.Decimal` with the literal's exact text, and `Intrinsic` has a binding review
  rule.
- 2026-11-26: item 1, step 1 started. The decompiler's own L2 documents lose nothing in `translate::ir`: every function of lz32.dll
  (93) and of the 16-bit lzexpand.dll (46) reads into the typed tree and writes back the same
  (`translate.l2.decompiler_documents_lose_nothing`). So `cb_l2`'s C back end can be moved onto the tree. Its size: `cback`,
  `cemit`, `cmodule`, `cpasses` and `cpasses2` are 5,600 lines over `py::V`, with `pyre` (Python-exact regex) and `pystr` kept as
  utilities. The C++17 core and its Java and Python rules are another 5,100. The proof is C byte for byte on the decompile corpus
  against today's `cb_l2`, then the C++17 goldens and judges.
- 2026-11-27: item 1, step 1, the C back end done. `translate/backends/c` runs on `translate::ir` and is the default (`CB_C_TREE=0`
  goes back to the library). A recording of 75,878 calls into `cb_l2` (15 MINI.CAB module builds and five 32-bit Win95 modules'
  checks) replays answer for answer, with about 15% less CPU time (144 MB peak against 43 MB: each document's tree is kept for
  later calls). Commits 8c7e2e3d3, c82c954a1, 60bd5828e. Next: the C++17 core onto the tree (part B), then `cb_l2` deleted.
- 2026-11-27: item 1, step 1, the C++17 back end done. `translate/backends/cpp17_tree` (be45e7701) is the default; `CB_CPP17_TREE=0`
  goes back to the library. Proof: every translate test group (40 groups, all passing) gives the same result with the switch on and
  off, with 6% less CPU time on (121.6 s against 128.9 s); 34 converted programs (C# and Python to C++17, each checked by running
  both) are byte-identical either way; the recordings replay answer for answer (223 of 223, and the conversions' own). Next: `cb_l2`
  deleted once its remaining users move: the L2 core entries (roundtrip, validate, ...) and the pyre, pystr and value helpers.
- 2026-11-27: item 1, step 1 done: `cb_l2` deleted (`src/translate/l2`, 16,600 lines with its two test programs). Its last entries
  answer in-process from `native_l2`: `roundtrip_text` through the typed tree (`l2::from_json`, `l2::to_json`), `set_schema` and
  `validate` by the schema check, now `src/translate/support/l2schema`. Python's value semantics the tree back ends use (`py::V`,
  `pystr`, `pyjson`, `pyre`) moved to `src/translate/support` (the library `cb_pysupport`), unchanged. The C entry point `l2_call`,
  `l2api.h` and the fallbacks `CB_C_TREE=0` and `CB_CPP17_TREE=0` are gone; `native_l2::Library` keeps its typed API (and a test's
  stand-in). The library's own test programs (built on request
  only) went with it: their C++17 goldens were stale (an older header), and what they held is in `translate.cpp17.*` and
  `translate.native_l2.cases.*`.
  Proof: the recordings replay answer for answer (75,878 of 75,878 C back end calls; 223 of 223 of the C++17 and core calls,
  `roundtrip_text`, `validate` and the unknown entry among them); every translate test group passes; the decompile area passes.
  Left of item 1's step 1: the back ends still take their arguments as JSON text in-process (`ctree::call`, `cpp17tree::call`);
  typed calls from `native_l2` would remove that last wire.
- 2026-11-27: items 2, 3, 13, 14 and 15 under way (one agent each): five emulator cores retired (F8, 8048, CP1610, HuC6280, 65C816),
  SuperH and PowerPC on the one assembler, the NID tables as data, one zlib/PNG/ZIP/XACT/EXEPACK/ISO 9660 reader, the Direct3D
  Displays as one template.
- 2026-11-27: item 2: the Z80 core retired. The emulator runs `z80exact.sla` (z80.slaspec built with EXACT: the undocumented
  flags, the block instructions' flags on every step, the ports' sixteen lines); `z80.sla` stays the decompiler's, unchanged
  apart from its recorded line numbers. Instructions with a byte in a bus device range are fetched through the bus (the
  emulator's `setFence`). Proof: 400,000 random instructions and the whole Z80 homebrew corpus (2,151 edited copies) the same
  on both cores; the random runs' digest is kept in `engine/pcode_cpu_z80_test`.
- 2026-11-27: item 2: the SM83 (Game Boy) core retired: `engine/pcode_cpu_sm83` gains `Sm83Cpu` for single steps, the old
  core's 25 unit tests run on it, and its random runs are kept as digests.
- 2026-11-27: item 2: the TLCS-900/H (Neo Geo Pocket) and V810 (Virtual Boy) cores retired, the last of the hybrids among the
  console lanes. The adapters handed about 90 TLCS-900 operations and the V810's floating point, bit strings, system registers,
  caxi, traps and divisions to copies of the old semantics (`engine/pcode_ops_tlcs900`, `pcode_ops_v810`, deleted); the
  descriptions now carry them under EXACT (`tlcs900exact.sla`: the four register banks switched by incf, decf, ldf and SR, every
  bit of SR and F', the DMA control registers, the flags as the processor sets them, the rotates, div, and daa, mirr, mula,
  bs1f/bs1b, minc/mdec, rld/rrd in p-code; `v810exact.sla`: the floating-point flags and conversions, the ten bit strings, the
  read-only system registers, jmp's low bit, the overflowing division). What ends a run (swi's BIOS stub, halt, trap, reti, a
  divide by zero) is a user operation the machine answers. The decompiler's `tlcs900.sla` and `v810.sla` are unchanged apart
  from two bugs (ldc's directions; caxi compared r30). Proof: 200,000 and 300,000 random instructions and programs per core and
  300,000 focused V810 runs, 0 differing; the NGP and Virtual Boy homebrew judged on both cores with no cap (1,746 edited
  copies, 0 verdicts disagreeing); the runs' digests are kept in `engine/pcode_cpu_tlcs900_test` and `pcode_cpu_v810_test`.
- 2026-11-27: item 7 started (docs/SOURCE_WRITER.md): one console writer (source/console_writer) and one file per platform
  (config/platforms/<name>.json). On it: ZX Spectrum, MSX, ColecoVision, Amstrad CPC (`z80-block`), Game Boy, Master System (`banked`),
  Atari 2600, 5200 and 7800 (`cart6502`); written source byte-identical on their tests, goldens and homebrew samples. Next: the Lynx, the NES, the PC Engine and the SNES, then `vectored`, then `executable`.
- 2026-11-28: item 7: the NES and the Atari Lynx on the `segmented` shape (ca65 segments placed by ld65); the source and the linker
  configuration byte-identical to the old writers' on every Lynx sample and the NES test programs. Next: the PC Engine and the SNES.
- 2026-11-28: item 7: the PC Engine and the SNES on `segmented`, the bank mapping and the register widths as the loader's flow (SegmentCode);
  byte-identical on 72 homebrew programs. Next: the `vectored` shape (Genesis first).
