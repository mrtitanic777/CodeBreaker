# The translator: any language in, any language out

The request (2026-10-19): "I want to be able to convert all kinds of code", then: "I wanna be able to
convert back-and-forth universally. C to Java, Java to C, C++ to Java, Java to C++, etc." Every
suggestion of the first draft was accepted: a garbage collector on the PC, modern Java, C#, the
project layer, more consoles. **Dynamic languages are in scope too**, and no language or pair is
preferred: "I wanna complete universal transpiler for all programming languages."

Today `codebreaker/java` (CodeBreaker) converts one kind of code well: decompiled 2010-2011 Minecraft
Java into C++03 for the Xbox 360. Judged beside the JVM, Alpha has 161 classes alike and none
different; Beta has 145 alike and one different, on non-ASCII text only. The Alpha 360 build is clean.
This document is the plan to turn it into a universal translator without losing any of that. Nothing
here is built yet.

## What "universal" means

N languages translated pairwise would need N x (N-1) translators. With a hub they need N front ends
(language -> IR) and N back ends (IR -> language). Any front end then pairs with any back end:
C -> Java, Java -> C, Python -> C++, C# -> Python, and so on.

Every pair is possible. Not every pair is equally natural, and the plan says so explicitly. The
output comes in two tiers:

- **Faithful (always):** the output does what the input does, checked by running both. Where the
  target cannot say something directly, the output carries a small emulation in its runtime.
- **Idiomatic (where the languages line up):** the output reads like code a person would write in the
  target. A pass chooses the idiomatic form wherever it is provably equivalent, and falls back to the
  faithful form elsewhere.

Where the languages differ in ways that matter, translation means lowering or emulating:

| Source has | Target lacks it (e.g.) | Faithful form | Idiomatic form when possible |
|---|---|---|---|
| Raw memory: pointer arithmetic, unions, type punning (C, C++) | Java, C#, Python, JS, Lua | an emulated linear memory (a byte array plus addresses), as Emscripten and CheerpJ do | ordinary objects, when a pointer analysis proves the code only uses whole structs and arrays |
| Garbage collection (Java, C#, Python, JS, Lua) | C, C++ | a collector in the runtime (Boehm GC) | ownership: owned fields, scoped objects, as CodeBreaker does for the 360 |
| Destructors / RAII (C++) | Java, C#, Python, JS, Lua | try/finally at each scope exit | `try-with-resources` / `using` / `with` |
| Templates (C++), generics (Java, C#) | C, Python, JS, Lua | one copy per instantiation, or erasure to a common base | generics in Java/C#; plain duck-typed code in dynamic targets |
| Multiple inheritance (C++, Python) | Java, C# | interfaces plus delegation | interfaces where the bases hold no state |
| Dynamic typing (Python, JS, Lua) | C, C++, Java, C# | a `Value` type (number / string / table / function / object) plus a small runtime | concrete types wherever type inference proves them |
| Closures, generators, coroutines | C, C++03 | closure records and state machines | lambdas (C++11+, Java 8+, C#), iterators, `yield` |
| Exceptions (most) | C | error returns with `setjmp`/`longjmp` underneath | error codes where the throw sites are local |
| Unsigned and fixed-width integers (C) | Java (no unsigned), JS and Lua (doubles) | masked arithmetic (`x & 0xFFFFFFFF`), helper calls | native types where the range analysis allows |
| `eval`, `exec`, runtime code loading (dynamic) | static targets | the translator ships an interpreter for the unsupported part, and says so in the ledger | none |

Some programs depend on the source machine itself: inline assembly, a particular OS API, hardware
registers. They translate as far as the code is portable, and the rest goes to the ledger with the
reason. That is the same "honest gap" rule as the decompiler's.

## Where it is today

*Update 2026-10-19: phase 1 is done and the old statement layer is deleted.* Every body, constructor
and initialiser of Alpha, Beta and Indev comes from javac's typed tree through the IR and the C++03
back end (`codebreaker/translate/`), judged equal or better than the old path class by class. What
the back end declines is an explicit, ledgered `J2X-GAP` (the body throws). The table below is the
starting point the plan was written against.

| Part | Size | What it is |
|---|---|---|
| `parse.py` | 437 lines | A brace-and-paren scan, not a grammar. Works because decompiler output is regular. |
| `ir.py` | 225 lines | Declarations only. **Method bodies stay raw text lines.** |
| `emit/statements/` | 3,286 lines | Turns that text into C++ statement by statement: about 100 regular expressions plus scans. |
| `emit/` (rest), `types.py`, `typemap.py`, `twophase.py` | ~2,500 lines | Declarations, ownership, naming, pointer-or-value decisions. |
| `runtime.py` + `platform/canonical/` | 1,759 + 2,570 lines, 42 headers | The Java library in C++: real where Minecraft needed it, stubs elsewhere. |
| `console/`, `profiles/` | 5,394 lines | The 360 project, the XDK, and fixes for Minecraft's own code. |
| `probe.py` | 858 lines | The judge: runs each class beside the JVM and compares. |

Two lessons from the work so far shape the plan:

1. **Text rewriting is where the bugs were born.** Nearly every fix of the last week was in the regex
   statement layer or in a runtime stub. The decompiler taught the same lesson
   (docs/CORE_ROADMAP.md, WIN16_LESSONS.md). A universal translator works on trees, never on lines.
2. **The judge is what made it trustworthy.** Every fix was found or confirmed by running the original
   beside the translation. Every language pair gets the same.

## The rules every change follows

1. **The regression gate.** Minecraft Alpha and Beta are judged in full before and after every change:
   no class may go from alike to different or to not compiling, and the Alpha 360 build stays at 0
   warnings and 0 errors. As each new language arrives, its corpus joins the gate.
2. **Behaviour, not text.** Output may change shape freely. What it does is checked by the judge.
3. **One IR.** Front ends produce it, back ends consume it, passes work on it. No back end reads
   source text, and no front end knows a target.
4. **The core names no program.** Anything that knows `Block` or `splashes.txt` lives in a project
   profile.
5. **Honest gaps.** Whatever cannot be translated faithfully is listed in the ledger with its reason.
   It is never silently dropped.

## The shape

```
 C ──────┐                                               ┌──► C
 C++ ────┤                                               ├──► C++03  (360, consoles)
 Java ───┤                                               ├──► C++17  (PC)
 C# ─────┼─► front end ─► IR ─► passes ─► IR ─► back end ┼──► Java
 Python ─┤   (per lang)          (shared)                ├──► C#
 JS/TS ──┤                                               ├──► Python
 Lua ────┤                                               ├──► JavaScript
 binary ─┘ (the decompiler emits the IR directly)        └──► Lua
                 runtime per (source semantics x target language)
                 platform layer per target machine
                 judge: the source run in its own runtime beside the output in its own
```

### Front ends: let each language's own compiler parse it

The hard part of a front end is meaning, not syntax: overloads, inference, implicit conversions,
which `x` a name refers to. Each language's own toolchain already knows that. So each front end is a
small program in that toolchain that dumps a **typed** syntax tree as JSON, and Python lowers it to
the IR. Every one of these toolchains is installed on this machine except Lua's.

| Language | Front end | Installed |
|---|---|---|
| C, C++ | clang: `-Xclang -ast-dump=json` (types, overloads and template instantiations resolved) | clang 22.1 |
| Java | a small `javac` plug-in on the compiler tree API (`com.sun.source.tree`, `Trees`) | JDK 21 |
| C# | a small Roslyn program (semantic model) | .NET 9 |
| Python | CPython's own `ast` module, plus the IR's type inference | Python 3.14 |
| JavaScript / TypeScript | the TypeScript compiler API (it parses plain JS too, with inferred types) | Node 24 (the `typescript` package is a download) |
| Lua | a Lua parser library (LPeg-based or `lua-parser`), or a small parser of our own: Lua's grammar is small | not installed (a download) |
| Binaries | CodeBreaker's decompiler already produces C; the C front end takes it from there | in the repo |

Source that a toolchain will not accept (half-decompiled code, missing libraries) keeps a text front
end per file where one exists, as Minecraft does today.

### One IR family, in three levels (docs/IR_UNIFICATION.md)

Decided 2026-10-19: CodeBreaker has one IR family, not one for binaries and another for source.

| Level | What it is | Produced by | Consumed by |
|---|---|---|---|
| L0 machine | P-code: exact instruction semantics | SLEIGH | the lifter |
| L1 low | SSA over typed values: memory spaces and segments, calls with conventions | the lifter (Ghidra core today, the Rust core later) | data flow, types, structuring |
| **L2 hub** | the typed tree below | every source front end, **and the decompiler** | every pass and back end |

The decompiler is one more front end: it emits L2 after structuring, and C comes from the hub's C
back end like any other target. There is no binary -> C text -> clang round trip. **L2 is defined in
JSON Schema** (`codebreaker/translate/l2.schema.json`), so Python (`ir.py`) and Rust (serde) implement
the same thing; a binary encoding of the schema is added later only if size or speed demands it.
Phase 2's segment and pointer types are built once, as L2 types, together with the "Windows 95"
work that owns phase 2 of the decompiler.

### L2: one typed tree that can hold every source language and every binary

Today's `ir.py` grows from declarations into whole programs. Everything a back end might need to
know is a node or a flag, and lowering is left to the back end or a pass.

- **Declarations:** modules and namespaces, types (class, struct, union, interface, enum, record,
  trait), generic and template parameters, fields, functions and methods, properties, operators,
  constructors and destructors, static initialisers, nested and local types, globals.
- **Statements:** block, local, expression, if, loops (while, do, for, for-each, for-in over keys),
  switch and match, labelled break and continue, goto (C), return, throw, try/catch/finally, scoped
  resources (RAII, `using`, `with`), lock, yield and await.
- **Expressions:** literals, resolved names, member access, calls (resolved overload, or dynamic),
  `new` and allocation (managed, `malloc`, placement), arrays, casts by kind (numeric, reference,
  boxing, reinterpret), operators with exact semantics (width, signedness, overflow, division by zero,
  `>>>`), pointer arithmetic, address-of and dereference, conditional, lambdas and closures with their
  captures, method references, type tests, string operations, comprehensions (Python), spread and
  destructuring (JS), multiple assignment and returns (Lua, Python).
- **Types:** exact-width integers (signed and unsigned), floats, booleans, chars with an encoding,
  strings with an encoding (UTF-16 for Java, C# and JS; bytes for C; Unicode code points for Python),
  pointers and references, arrays (fixed, dynamic, jagged), class references, generic instances,
  function types, nullable, and **`dynamic`** for whatever inference cannot pin down.
- **Memory model:** each allocation carries how it is managed (GC, owned, borrowed, stack, manual,
  emulated) as decided by the source language and refined by the passes. *Emulated* is the linear
  memory for pointers the analysis cannot discipline, from C sources and binaries alike.
- **For binaries** (docs/IR_UNIFICATION.md, items 1-7):
  1. pointers carry an **address space**: flat, near, far (selector:offset), huge, real-mode
     (`seg*16+off`), I/O port;
  2. structures can have an **explicit layout**: each field's byte offset, the packing, the total size;
  3. function types carry a **calling convention**: cdecl, pascal, stdcall, fastcall, register
     arguments, where the result is returned, who pops how many bytes;
  4. **exact integer semantics** are in the operators (16-bit int, wrap or saturate), not in a back end;
  5. the **emulated memory** model above;
  6. an **opaque machine fragment** node: what C cannot carry stays assembly, printed as inline asm by
     the C back end and reported in the ledger by the others;
  7. **provenance**: next to source file, line and column, a node can carry *module + address*.
- A small **core** node set; everything else is a flagged extension with a lowering pass into the core,
  so a back end only has to know the core.
- Every node carries its provenance, so ledgers and the judge point at the original.

### Passes: work done once for every language

- **Type inference** for dynamic sources: concrete types where provable, `dynamic` elsewhere.
- **Pointer and memory analysis** for C and C++ sources: which pointers are disciplined (they become
  references and objects in managed targets) and which need the emulated memory.
- **Null model:** which references can be null; value types that need a null state.
- **Construction:** two-phase construction for virtual calls in constructors (`twophase.py`),
  constructor delegation.
- **Ownership and escape:** what may be freed where, for targets without a collector.
- **Static initialisation order.**
- **Exceptions:** `finally`, unwinding into targets without exceptions.
- **Reflection:** registries into factories (`reflection_factory.py`).
- **Numeric semantics:** saturating casts, signed bytes, unsigned emulation, integer division.
- **Idioms:** recognise faithful forms with an idiomatic equivalent (a counted loop over an array ->
  for-each; try/finally around a close -> `with`; getter/setter pairs -> properties) and use it when
  the target has one.

### Back ends

| Back end | Targets | Memory from a GC source | Notes |
|---|---|---|---|
| C++03 | Xbox 360, older consoles | ownership inference, reference counting where undecidable | today's path |
| C++17 | PC (Windows, Linux) | Boehm GC | templates, lambdas, CMake |
| C (C99) | anything with a C compiler | Boehm GC | classes as structs and function tables, exceptions as `setjmp` |
| Java | JVM | native | pointer-heavy C/C++ through the emulated memory |
| C# | .NET | native | structs and unsafe code available for C sources |
| Python | CPython | native | typed source -> type hints |
| JavaScript / TypeScript | Node, browsers | native | integers are doubles: 64-bit through `BigInt`, 32-bit by masking |
| Lua | Lua 5.4, LuaJIT | native | tables for objects, 1-based arrays adjusted |

### Runtimes: one per (source semantics, target language)

A translation needs the source language's library in the target language: Java's `java.util` in
C++, C's `libc` in Java, Python's built-ins in C#. Writing each by hand for every pair is N x N, so:

- **The core of each source library is written once in the IR** (or in a language with a front end)
  and translated by the translator itself into every target. That is how `java.util.HashMap` reaches
  Lua.
- **Native pieces** (files, sockets, threads, time, graphics) are a thin hand-written layer per target
  language, the way `platform/reference/` is for the 360.
- **Where the target already has the same thing,** the runtime maps to it (`java.util.ArrayList` ->
  `List<T>` in C#, `std::vector` in C++) when the semantics match exactly, and the judge checks that
  they do.
- Today's `platform/canonical/` (the Java library in C++03) is the first such runtime and stays.

### Memory

- **GC source -> C/C++ on the PC:** Boehm GC. Converted code allocates through it and never deletes.
- **GC source -> C/C++ on consoles:** today's ownership inference, plus reference counting where the
  pass cannot decide.
- **Manual-memory source (C, C++) -> managed target:** disciplined pointers become references;
  everything else lives in the emulated memory. `free` becomes a no-op in managed targets, or a real
  release in the emulated memory.
- **Manual -> manual (C <-> C++):** kept as is.

### Project layer

Everything that knows a particular program sits behind a **project profile**:

- the console passes (`sound_fix`, `render_fix`, `controller_nav`, `applet_ping`, `achievements`,
  `xact`);
- overlays and hand fixes;
- name lists such as `typemap.KNOWN_NULLABLE_GETTERS`;
- the probe stand-ins for libraries a program shipped in modified form.

A program with no profile converts with the core alone.

### The judge, for every pair

`probe.py` runs a class's methods in the original's runtime and in the translation's, then compares
results, objects and fields. It becomes language-neutral with an adapter per runtime: the JVM, .NET,
CPython, Node, Lua, and a native compiler for C and C++. The harness is generated from the IR, so the
same cases run on both sides of any pair. Round trips are judged too: A -> B -> A must still match A.

**Corpora:**
- **Regression:** Minecraft Alpha and Beta.
- **Per source language:** two or three real projects chosen for their features rather than their
  size: heavy generics and collections; lambdas, closures and iterators; real IO; for C, pointer-heavy
  code; for dynamic languages, code that changes types at run time.
- Your own programs, whenever you have some: real targets beat sample projects.

## Every language: the language kit

The goal is "a complete universal transpiler for all programming languages", with no pair preferred
over another. There are thousands of languages and new ones keep appearing, so "all" is reached by
making each addition mechanical rather than by a fixed list. A language is added by supplying a
**kit** to a fixed contract:

1. **Front end:** source -> IR, through the language's own toolchain where one can dump a typed tree,
   else through a parser of our own (`tree-sitter` grammars exist for almost every language and give
   a uniform syntax tree to start from).
2. **Back end:** IR -> source, with its idiom rules.
3. **Library:** the language's standard library, written mostly in the IR so the translator carries
   it into every target, with a thin native layer per target.
4. **Judge adapter:** how to run the language's own code beside a translation and read results.
5. **Corpus:** real programs in the language, which join the regression gate.

A finished kit pairs with every language already present, in both directions.

**The order is set by dependency, not preference:** each step is the one that the most later work
stands on.

| Wave | Languages | Why here |
|---|---|---|
| Foundation | Java -> C++ on the new IR | the one path already proven; the gate checks the rebuild |
| 1 | C (in and out) | the lowest common target: every runtime can bottom out in it, and binaries arrive as C through the decompiler |
| 2 | Java (in and out, modern), C++ (in, C++03 and C++17 out) | the static object model, generics and templates, RAII |
| 3 | C# | completes the static, garbage-collected family; checks the IR is not Java-shaped |
| 4 | Python, JavaScript/TypeScript, Lua | the dynamic machinery (`dynamic`, inference, closures, tables) once, three languages on it |
| 5 | Go, Rust, Kotlin, Swift, Objective-C, D, Zig, Dart, Scala | mostly kits over machinery that exists by then; Rust as a target uses `Rc`/`RefCell` in the faithful tier |
| 6 | Ruby, PHP, Perl, Pascal/Delphi, BASIC dialects, Fortran, COBOL | more dynamic and older procedural languages |
| 7 | Haskell and ML family, Lisp/Scheme/Clojure, Erlang/Elixir, Prolog | need runtime models of their own: laziness, macros, actors, logic search |
| 8 | Assembly languages | through the decompiler, as binaries are |

Each wave's languages can be built side by side once the wave before them is in.

## Phases

Each phase ends with the regression gate passing, plus its own exit check. The first two are the
foundation; after them, front ends and back ends follow the waves above, and each one multiplies the
number of working pairs.

1. **Bodies into the IR, beside the old path.** Statement and expression nodes; the javac front end;
   a C++03 back end that emits from the tree. Per class, the new path is used when the judge rates it
   at least as well as the old one. *Exit:* every Alpha and Beta class through the new path, results
   equal or better.
2. **Passes off the text.** Null, construction, ownership, static order, exceptions and reflection as
   IR passes; `emit/statements/` deleted (done 2026-10-19: the registry null guard, the leak fix, the
   reflection tag and the no-GC ownership cleanups are decided on the IR in the back end; the 360 fixes
   to the game's own logic are IR patches, codebreaker/translate/rewrite.py. Text rewriting left on
   purpose: two-phase construction and the GL->D3D9 rename, token renames that must also reach the
   hand-written platform layer and overlays). *Exit:* the gate passes and the emitted C++ is diffed class
   by class against the last old-path output, every difference explained.
3. **Project layer.** Everything game-specific behind project profiles. *Exit:* a non-Minecraft Java
   program converts with no Minecraft code loaded. (Done 2026-10-19: codebreaker/java/projects -- a project is found by
   its manifest.json, as data, and only then imported; Minecraft's profiles, edition detection, console
   fixes and reports live in projects/minecraft; the default is generic. tests/java/test_projects.py.)
4. **Modern Java and the C++17 back end.** Generics, lambdas, streams, records; Boehm GC; CMake; the
   PC platform layer (SDL2 and OpenGL). *Exit:* a Java corpus judged on both C++ back ends; Minecraft
   Alpha builds for the PC.
   (Progress 2026-10-20: the PC platform -- `--platform pc_opengl`: LWJGL on SDL2 + real OpenGL
   (codebreaker/java/platform/pc), a generated PC entry and CMake project. Minecraft Alpha converts,
   builds with g++ -std=gnu++17 and plays: menus, world generation, rendering. Sound (paulscode
   SoundSystem on SDL2 audio + libvorbisfile), real threads, an offline network layer, settings saving
   and PNG screenshots too. Java's left-to-right operand order is kept where C++'s differs (the back end
   sequences the operands into temporaries); without it every world differed from Java's for the same
   seed, on the 360 as well. Still to do: the C++17 back end proper, Boehm GC, modern Java front-end
   features.)
   (Progress 2026-10-31: **the C++17 back end proper for Java**, `translate/backends/cpp17_java.py` (beside the Python front end's `cpp17.py`, on its own runtime: to be one back end), two ways in.
   *Inside the transpiler* (`convert(..., cpp_std='C++17')`): cpp17.Bodies, cpp03's Writer with the C++17
   forms -- a for-each over a container as a range-for, a switch expression as an immediately invoked
   lambda, `auto` where the initialiser spells the type, nullptr against pointers (the runtime's value
   types keep their C++03 null: `JavaString != nullptr` has no operator) -- cpp03's faithful form
   everywhere else. *Whole programs from L2* (`cpp17.Program`, on `translate/runtime/cpp17/jrt.hpp`):
   declarations and bodies; references are `std::shared_ptr` (reference counting instead of Boehm,
   which is a download: the classes whose fields can form a cycle are listed at the top of the output,
   the place a collector is needed); records and final classes that never hand `this` out get real
   constructors (`std::make_shared<Point>(x, y)`), the rest two-phase construction as the JVM does it;
   lambdas and method references are C++ lambdas held by the interface (`jrt::fn<I>`, `[=]` captures,
   `this` held strongly); for-each a range-for (`jrt::each<T>`); `for (Map.Entry e : m.entrySet())`
   that only reads the entry a structured binding; switch expressions immediately invoked lambdas;
   static fields `static inline` with Java's lazy class initialisation; erased generics with checked
   casts back; java.util.Optional on std::optional; lazy streams; Java's own HashMap table so iteration
   order is Java's (the judge found HashMap.merge inserting at the bin's head). *Modern Java in the
   front end*: records (their implicit members, as ObjectMethods defines them), instanceof patterns,
   the anonymous class's supertype; `translate/desugar.py` lowers for-each, switch expressions,
   switches on strings and patterns for the back ends that cannot say them (cpp03, C). *Judged*
   (`translate/javajudge.py`): Minecraft Alpha through the transpiler on both back ends, 161 classes
   alike and 0 different on each, the same classes (297 run nothing comparable); five modern programs
   written for this (tests/translate/fixtures/java_modern: records, sealed interfaces, switch
   expressions, patterns, generics, lambdas, method references, streams, collectors, Optional,
   exceptions, maps), each with a probe harness generated from L2 and translated with the program:
   all five alike as whole C++17 programs (37 methods, 635 cases, every main's output); through the
   transpiler all five are refused on both back ends -- its declaration front end (parse.py) and the
   2010 runtime do not read records, bounded generics or the newer library. Minecraft's MathHelper and
   MCHashTable as whole programs: alike. Still to do: the transpiler's declarations from L2, Boehm.)
5. **C and C++ in (clang), Java out.** The pointer analysis and the emulated memory; the Java back
   end; the C library in Java. *Exit:* a C corpus and a C++ corpus judged as Java; Java -> C++ -> Java
   round trips judged.
   (Done 2026-10-31, see "Phase 5 as built" below: C corpus 184 of 187 judged programs equivalent --
   161 idiomatic, 23 faithful -- and 3 refused, none divergent; C++ corpus 7 of 9 equivalent, the 2
   refused the ones meant to be; the Java -> C++ -> Java round trip equivalent.)
6. **C out.** The C back end. *Exit:* the Java corpus judged as C.
   (Progress 2026-10-31: `translate/backends/c_java.py`, beside the decompiler's `backends/c.py`
   (untouched): a whole Java program as C11 on `translate/runtime/c` (jrt.h, jrt.c). A class is a
   struct starting with its base's; virtual methods a table whose prefix is each base's; interfaces
   through per-class itabs; lambdas and method references closure records; X_init/X_new two-phase
   construction; setjmp/longjmp exceptions (a return or jump out of a try pops its handler, locals
   volatile); Java's HashMap order. Memory: nothing is freed while the program runs -- a collector or
   reference counting is still to do here. Compiled with clang, judged beside the JVM: Shapes,
   Generics, Bank and TextStats alike (29 methods, 510 cases, every main); Streams refused (no streams
   in the C runtime yet); Minecraft's MathHelper and MCHashTable alike.)
7. **C# in and out.** Roslyn front end, C# back end, the .NET adapter in the judge.
   (Progress 2026-10-31: the kit is in. Front end `translate/frontends/csharp`: CsTreeDump.cs, built
   by the installed SDK's own csc.dll against its Microsoft.CodeAnalysis assemblies by path -- no
   project, no restore, nothing downloaded -- dumps declarations and Roslyn's IOperation tree;
   lower.py makes L2. Back end `translate/backends/csharp.py` (+ `csharp_java.py`, Java's library and
   semantics in C#, through `api_maps/java.csharp.json` and the runtime `dotnet/runtime/JavaRt.cs`).
   Judge `translate/dotnet/judge.py` (csc + the installed runtime; javac + the JVM; the host C++
   compiler), pairs in `dotnet/pairs.py`. Judged: C# -> C# 10 of 10 equivalent; Java -> C# 4 of 4
   (the Java lane's Calc/Base/Sub and fixtures, a modern-Java program, and real Minecraft Alpha
   NoiseGeneratorPerlin/Octaves + MathHelper through java.util.Random to the same doubles); C# ->
   C++03 through `dotnet/cs2cpp.py`: Shapes equivalent, the rest refused with reasons. Where L2 was
   Java-shaped, and what was added (additive): a Sym could not name a property or say a parameter is
   ref/out (kinds property/event, Sym.refs, Sym.flags); nothing said whose semantics a unit's string
   conversions and library calls carry (Unit.lang); no property or indexer (Property, Index.sym); type
   parameters lost their bounds (TypeParam); an enum could not be a named integer (EnumConst.value);
   u16 meant Java's char, but C# has both char and ushort (Prim.enc); no nullable reference, no
   rectangular array (Class.nullable, Array.rank); no `??` (Binary op). Beyond the schema: grouping
   was carried only by javac's Paren nodes (the C++03 back end prints no precedence), so the C# front
   end puts back the parentheses C# needs; and the C++03 back end is L2 for bodies only -- its
   declarations still come from Java source text, its runtime is Java's library (System.out a no-op),
   and it writes no for-each -- which `cs2cpp.py` crosses with a Java declaration skeleton, a library
   rebinding and C#'s properties lowered to methods.)
8. **Python in and out.** The `ast` front end, type inference, the `dynamic` runtime, the Python back
   end.
9. **JavaScript / TypeScript in and out.** The TypeScript compiler front end, the JS back end.
10. **Lua in and out.**
11. **Binaries in.** A `PrintIR` beside the Ghidra core's PrintC writes each structured function as L2
    JSON. The hub's C back end must reproduce today's C (`tools/golden.py`, the emulator judge) before
    the C text path is retired; `c_text.py`'s remaining passes become L2 passes or C idiom rules. The
    Rust core (CORE_ROADMAP Track 2) targets L1 and L2 and has no emitter of its own.
12. **More consoles,** each a back end setting plus a platform layer.

Phases 1 and 2 cannot move: everything else stands on them. The L2 schema is written during phase 1,
with the binary-facing items, so the decompiler work and the translator converge on one IR. After phase 11 the waves continue
language by language (waves 5-8), each one a kit.

## Phase 5 as built (2026-10-31): C and C++ in, Java out

**Front end** (`codebreaker/translate/frontends/clang/`). clang parses and attributes each file
(`-Xclang -ast-dump=json`, target `x86_64-pc-windows-msvc`: the data model the judge's own clang build
runs with). `lower.py` lowers the declarations of the program's own files (the file and the headers
beside it) into one `ir.Unit`; `ctypes.py` reads clang's type spellings back through C's declarator
grammar (typedefs, tags, C++ names, `__attribute__`s and conventions dropped) and lays structs out.
What C says implicitly is explicit in L2: every conversion is a `Cast` (marked `implicit`), sizeof is a
`Lit` of the target's value (its spelling in `text`), `p->f` is `Select` on `*p`, a C++ reference is a
`ptr` with `ref` whose reads and bindings are `Unary '*'/'&'`, `this` is a pointer, C enum constants are
int globals, a string literal's value is its bytes. C++: classes (fields, methods, constructors with the
base initialiser as `CtorCall super`, destructors, static members defined outside the class), single
inheritance, virtual/pure methods, operators as calls, templates by instance (`Box<f64, 2>`), new/delete,
`enum class`, range-for over arrays. L2 additions, all optional: `Ptr.ref`, `Cast.implicit`,
`ClassDecl.layout` (a `record`), `Unit.target`. A document per file is cached (`clang.l2_file`).

**Back end** (`codebreaker/translate/backends/java.py`, `java_objects.py`, `java_libc.py`, runtime
`java_runtime/crt/`). The pointer analysis (`java.Analysis`) picks the tier per program:
- *idiomatic* when every pointer is disciplined -- structs and C++ classes are Java classes, a struct
  pointer a reference, an array (and a pointer that only indexes) a Java array, a scalar whose address is
  taken a one-element array (`int[] n = {5}`: C's out-parameters and C++'s `int &`), struct values copied as
  C copies them (copy constructors, `set`), malloc `new`, free the collector's, function pointers functional
  interfaces;
- *faithful* otherwise -- unions, pointer arithmetic, punning, raw memory functions, varargs definitions:
  everything reachable through a pointer lives in `crt.Mem` (a little-endian byte array; an address is an
  int; a struct its address, its fields at offsets named in a class per struct; frames on an emulated
  stack; MSVC's own va_list model for `...`), variables whose address is never taken stay Java locals.
Both share C's arithmetic on Java types (unsigned division/comparison/widening written out), structure
`goto` where it nests (labeled `break`/`continue`), and C's library: `crt.LibC` (printf with C's exact
formatting, string/memory/ctype/stdlib, MSVC's rand and ctype masks), chosen through the API map's new
C rows (`codebreaker/java/api_maps/libc.json`). A function it cannot write is an explicit gap (its body
throws, the Java lane's J2X-GAP); a program whose run reaches a gap is refused with that gap's reason.

**Judge** (`codebreaker/translate/judge_native_jvm.py`, `python tools/judge_c_java.py [c|cpp|roundtrip]`):
the original built with clang `-O0` and run; the translation compiled with javac and run; stdout, stderr
and exit code compared ("\r\n" as "\n"). Verdicts: equivalent, divergent, refused, no-compile,
no-original. Corpus: `frontends/clang/corpus/manifest.json` -- Logan's CodingTeacher C course, the MS C
compiler project's K&R test programs (printf, fp, diff, diff300; bdos output as putchar), the MSC 3.00
verify inputs, tiny-AES-c's test, zlib's `enough.c`, Reko's C subjects; for C++, Reko's
LocalStackVariables and eight classic programs written for the corpus (`corpus/cpp/`, two of them meant to
be refused: iostream/vector and RAII).

**Round trip:** `judge_native_jvm.roundtrip` -- the Java lane's fixture classes (Vec3D, MathHelper,
AxisAlignedBB), the C++03 back end's test classes (Base, Sub) and a driver go javac -> IR -> C++03
(`codebreaker/java`), then clang -> L2, where `frontends/clang/j2x.py` maps the Java runtime in C++
(JavaString, JArray, JCat, System::out, j2xToInt, s_ statics, initStatics, the sequencing temporaries)
back onto Java's library, then the Java back end: equivalent to the original on the JVM.

**Not yet:** bit-fields; files (fopen); setjmp/longjmp; RAII (stack objects with destructors: try/finally);
the C++ standard library (iostream, string, containers); multiple inheritance; lambdas; exceptions in the
faithful tier; a C++ program that needs the faithful tier; argument evaluation order follows Java's
left-to-right where C leaves it open.

## Still open

0. **L2's segment and pointer types** are agreed with the "Windows 95" session (decompiler phase 2)
   before either side builds them.

1. **Downloads:** the TypeScript compiler (`typescript`, from npm), a Lua interpreter (5.4),
   `tree-sitter` grammars, and later each wave's toolchains. Each is asked about when its phase comes,
   with name, source and size.
2. **Corpora:** any programs, in any language, that you want converted.
3. **Which console after the 360?**
