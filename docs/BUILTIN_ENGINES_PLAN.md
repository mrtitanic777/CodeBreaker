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
| RetroMCP (`src/java/retromcp`, a jar run by Java) | a Minecraft version downloaded, remapped and decompiled to Java source, the first step of `java port` for a game jar | decompile/jar.cpp, java/tools.cpp |

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

## 5. RetroMCP: the Java decompile pipeline

Logan (2026-11-20): CodeBreaker does natively everything RetroMCP does, and the jar goes. A full C++ port in stages, each
proven against the jar's own output on real Minecraft versions before the next starts; the jar and its Gradle build are
deleted when the last stage matches (its LICENSE and MCP-CREDITS stay with the code that came from it). Compiling and
running the game keep using the installed JDK, as the Java front end already does.

1. **Class files** (done: `java/classfile`): a class read into a tree and written back byte for byte; a method's code
   decoded into instructions and encoded back. Proven on every class of the jars in the tree.
2. **Versions and downloads**: the version list, a version's JSON, the client and server jars, the libraries and the
   assets, kept in a cache keyed by their SHA-1; an HTTP client on WinHTTP (and libcurl's place on Linux).
3. **Mappings and remapping**: Tiny v2 mappings read, the obfuscated names of every class, field and method renamed
   through the constant pools and descriptors (the injector's remap), with the jar's own results as the oracle.
4. **The injector's fixes**: inner classes, implicit constructors, exceptions (.exc), access, decomposed variables,
   the common constants and the GL constants, string literals, generics guessed or stripped.
5. **The decompiler**: Fernflower's passes (control flow, variables, expressions, class writer) in C++, the largest
   stage; each pass checked on the output Fernflower gives for the same remapped classes.
6. **Patches and the source adapters**: the unified diffs applied, the source rewritten as RetroMCP does after them.

`java port` (java/port.cpp) keeps working through every stage: a stage that lands replaces the jar's part of the run.

## Order

1. x86asm (in progress). 2. The decompiler in-process. 3. x86link, and wasm's jobs moved to x86asm. 4. The DOS executive
for everything DOSBox-X does. 5. The C compiler, by the option chosen.
6. RetroMCP, by its stages above.
