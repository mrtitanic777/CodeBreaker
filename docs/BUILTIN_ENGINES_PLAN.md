# Built-in engines: what CodeBreaker still runs as separate programs, and how each becomes its own

The rule (Logan, 2026-11-19): CodeBreaker does its work with engines in its own codebase, compiled into `codebreaker.exe`
— never by starting a separate tool beside it. x86asm (docs/X86ASM_DESIGN.md) replaces NASM. This is the plan for the
rest. Nothing here is built yet.

## What is still separate

| Separate program | What CodeBreaker uses it for | Called from |
|---|---|---|
| `decompiler.exe`, built beside `codebreaker.exe` in `build` (+ `pcodecheck.exe`, `sleigh.exe`) | every decompilation to C: a module's description in, each function's C and notes out | decompile/decompiler.cpp, emulate (pcodecheck) |
| OpenWatcom `wcc` / `wcc386` | compiling the C CodeBreaker writes (16-bit and 32-bit), the build and check lanes | build/functions, module_build16/32, engine/lane_kit*, engine/win16, verify/gate16 (23 files) |
| OpenWatcom `wasm` | DOS programs' and VxDs' MASM-style assembly | source/dos_writer, source/vxd |
| OpenWatcom `wlink` | linking DOS, Win16, Win32 and VxD builds | the same lanes |
| DOSBox-X | running period DOS tools (Microsoft C, MASM) and DOS programs | build/dos_tools, scripts |
| javac (the JDK) | the Java translator's front end (TreeDump: parse and attribute) | java/frontend.cpp |

## 1. The decompiler — first, and the smallest step

It is already ours in all but packaging: `src/decompile/engine` is `decompiler.cc` (ours) around a decompiler core
(`src/decompile/engine/core`, about 190,000 lines of C++, Apache-2.0) built as a library. Making it built-in means linking
that library into `codebreaker.exe` and calling it in memory: the description CodeBreaker writes today becomes a
structure passed directly, and each function's C comes back as data. No process, no description files on disk.

What has to be solved:
- **Crash isolation.** Today a function that crashes the decompiler kills only `decompiler.exe`; in-process it would kill
  CodeBreaker. The engine runs each batch on a worker thread with structured-exception and C++ exception guards, and a
  function that faults is reported as not decompiled — the same outcome as now.
- **Speed.** Parallel decompilation keeps working: the core's state is per-architecture object, so several run at once.
- **The core's origin.** The core is CodeBreaker's fork of the Ghidra C++ decompiler (docs/GHIDRA_FORK.md): our code,
  its licence and notice kept, not a Ghidra dependency. Logan confirmed (2026-11-19): it is absorbed into
  `codebreaker.exe` as a built-in engine.

## 2. OpenWatcom's assembler and linker (wasm, wlink)

Same shape as x86asm: port the job into C++ engines with differential proof against the current tools (capture every
run, replay, same bytes). `wasm`'s MASM dialect largely overlaps x86asm's MASM mode, so after x86asm phase 4 the DOS and
VxD sources can move to x86asm and `wasm` drops out without its own port. `wlink` becomes `x86link`: OMF and COFF in,
MZ / NE / PE / LE out — the formats CodeBreaker already reads and writes (formats/).

## 3. OpenWatcom's C compilers (wcc, wcc386) — the largest

A full C compiler with its optimizing code generator: hundreds of thousands of lines (the C front end and the code
generator together). Options, smallest first:
1. **Port OpenWatcom's front end and code generator** into CodeBreaker as an engine, with the same differential proof.
   Faithful, very large.
2. **Write CodeBreaker's own C compiler** for the C CodeBreaker writes (a known subset), over the L2 hub, with its own
   16- and 32-bit x86 back end. Smaller, but its code would differ from OpenWatcom's: the proof becomes "behaves the same"
   (the oracle judges both), not "same bytes".
Recommended: decide after x86asm and the decompiler are built in; option 2 fits the L2 architecture
(docs/ARCHITECTURE.md), option 1 keeps every build's bytes as they are today.

## 4. DOSBox-X

Already has an in-house replacement under way: CodeBreaker's own DOS executive on its emulator (build/native_dos,
exact/emu/dos), proven byte-identical for MASM and ML. Extending it to the Microsoft C passes and to running DOS
programs for behaviour checks retires DOSBox-X.

## 5. The Java engines -- done: RetroMCP is retired

Logan (2026-11-20): CodeBreaker does natively what RetroMCP did for Minecraft, and RetroMCP is deleted. javac is not
ported ("a major headache": about 120,000 lines); the JDK stays only for the translator's front end (TreeDump).

1. **Class files** (`java/classfile`): a class read and written back byte for byte, a method's code decoded and encoded.
2. **The decompiler** (`java/decompiler`): Fernflower (Lassebq's build, Apache-2.0) ported whole. With Minecraft's
   options (nco=1 rbr=0 asc=1 ovr=0 dgs=1, a tab indent) every class of the Gradle wrapper (31) and of RetroMCP's own jar
   (585) is written byte for byte as the original writes it, its failures included (decompiler_test.cpp). Memory: a bump
   region per class (`Arena::ClassRegion`), 890 MB at peak instead of 11 GB; a stack of its own (512 MB reserved). Java's
   collection orders are Java's, sized as the JDK since 19 sizes a copied HashSet/HashMap (the reference runs on JDK 21).
   `codebreaker java fernflower <jar> <dir>` runs it on any jar.
3. **The class tree** (`java/bytecode`): ASM's tree model (ClassNode, MethodNode, InsnList, Type ...), read with
   java/classfile, written with the pool built again and max stack and locals computed (ASM's COMPUTE_MAXS).
4. **The Minecraft preparation** (`java/projects/minecraft/prep`): RetroMCP's setup (the version list, the version JSON,
   the jars, libraries, natives and assets downloaded with core/http), its RDI library (the mappings read, the classes
   remapped and fixed: decompose variables, common constants, the GL constants, source files, inner classes, implicit
   constructors, generics, exceptions, access, merge), the decompile with the native Fernflower and DiffPatch's patch
   application, in TaskDecompile's order (prepare.hpp). Each part proven against the original's own step (an oracle of
   RetroMCP's code, work/retire-retromcp/oracle) on six versions (1.5.2, 1.2.5, b1.7.3, c0.30, inf-20100618, a1.2.6):
   setup 1436 of 1436 files byte for byte; remap, fixes and constants: every class; patches: every hunk. End to end
   (`codebreaker java decompile <version>`): 6,545 of 6,551 source files are RetroMCP's own text; the 6 others are where
   RetroMCP's Fernflower iterates sets ordered by Java's identity hash (its own output there depends on what the JVM ran
   before), and in each ours is the original's ordered build's text byte for byte. Not ported, as nothing uses them:
   RetroMCP's IDE projects and its recompile-and-MD5 stage (javac, for reobfuscation).

## Order

1. x86asm (in progress). 2. The decompiler in-process. 3. x86link, and wasm's jobs moved to x86asm. 4. The DOS executive
for everything DOSBox-X does. 5. The C compiler, by the option chosen.
6. The Java engines (done: RetroMCP retired; javac not ported).
