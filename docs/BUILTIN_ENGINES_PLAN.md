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
| Fernflower (inside RetroMCP's jar) and javac (the JDK) | Java classes decompiled to Java source, and Java compiled back to classes | decompile/jar.cpp, java/tools.cpp |

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

## 5. The Java engines: the decompiler and the compiler

Logan (2026-11-20): what CodeBreaker needs from RetroMCP is not RetroMCP (it only prepares a Minecraft version to be
worked on: downloads, mappings, patches) but the two engines it drives: **Fernflower**, which decompiles the classes to
Java, and **javac**, the JDK's compiler, which compiles the Java back to classes. Those two are ported to CodeBreaker C++;
RetroMCP's own steps are not. Each engine is proven against the original on the same input before it replaces it.

1. **Class files** (done: `java/classfile`): a class read into a tree and written back byte for byte; a method's code
   decoded into instructions and encoded back. Both engines stand on it. Proven on every class of the jars in the tree.
2. **The decompiler**: Fernflower, the build RetroMCP uses (github.com/Lassebq/Fernflower, master; package
   `de.fernflower`, Apache-2.0 with the JetBrains headers; 211 files, about 42,000 lines; every class in RetroMCP's jar is
   in it). The reference is that source built with the JDK (`javac --release 8`, `ConsoleDecompiler <jar> <out>`): the
   C++ port writes the same Java for the same classes, a method the original cannot write included. In this order, each
   layer proven before the next:
   - the class structure (struct/, struct/attr, consts, gen, generics: about 4,400 lines) over `java/classfile`;
   - the instructions and the control-flow graph (code/, code/optinstructions, code/cfg, code/interpreter: about 5,300);
   - statements from the graph (modules/decompiler/decompose, stats, DomHelper, the loop, if, finally and label helpers);
   - expressions and the stack (exps, ExprProcessor, StackVarsProcessor, sforms: SSA);
   - variables and their types (vars), the simplify passes, the renamer;
   - the class writer and the main driver (main/, rels, collectors, util), with RetroMCP's decompile options.
3. **The compiler**: Java source to class files (parse, attribute, flow, lower, generate), checked by compiling with
   both and comparing what the classes do (and, where javac is deterministic, their bytes).

## Order

1. x86asm (in progress). 2. The decompiler in-process. 3. x86link, and wasm's jobs moved to x86asm. 4. The DOS executive
for everything DOSBox-X does. 5. The C compiler, by the option chosen.
6. The Java decompiler, then the Java compiler.
