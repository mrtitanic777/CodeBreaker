# The translator: many languages in, many targets out

The request (2026-10-19): "I want to be able to convert all kinds of code." Asked which kinds: other
languages too, not only Java. Asked which targets: the Xbox 360 (C++03), the PC (modern C++), and the
other consoles.

Today `codebreaker/java` (J2CTool) converts one kind of code well: decompiled 2010-2011 Minecraft.
Judged beside the JVM, Alpha has 161 classes alike and none different; Beta has 145 alike and one
different, on non-ASCII text only. The Alpha 360 build is clean. This document is the plan to turn it
into a general translator without losing any of that. Nothing here is built yet. It is for review
before code moves.

## Where it is today

| Part | Size | What it is |
|---|---|---|
| `parse.py` | 437 lines | A brace-and-paren scan, not a grammar. Works because decompiler output is regular: tabs, one declarator per line, `var1` locals. |
| `ir.py` | 225 lines | Declarations only: classes, fields, method signatures. **Method bodies stay raw text lines.** |
| `emit/statements/` | 3,286 lines | Turns that text into C++ statement by statement: about 100 regular expressions plus scans for types and scopes. |
| `emit/` (rest), `types.py`, `typemap.py`, `twophase.py` | ~2,500 lines | Declarations, ownership, naming, pointer-or-value decisions. |
| `runtime.py` + `platform/canonical/` | 1,759 + 2,570 lines, 42 headers | The Java library in C++: real where the game needed it (strings, collections, IO, regex, MD5), stubs elsewhere. |
| `console/`, `profiles/` | 5,394 lines | The 360 project, the XDK, and fixes for Minecraft's own code. |
| `probe.py` | 858 lines | The judge: runs each class beside the JVM and compares results. Already language-neutral in shape. |

Two lessons from the work so far shape the plan.

1. **Text rewriting is where the bugs were born.** Nearly every fix of the last week was in the
   regex statement layer or in a runtime stub: `->length` vs `.length`, `finally` copies, null
   checks, byte casts, discarded `new`. The same lesson came out of the decompiler
   (docs/CORE_ROADMAP.md, WIN16_LESSONS.md). A translator that has to accept code it was never tuned
   on needs to work on a tree, not on lines.
2. **The judge is what made it trustworthy.** Every fix was found or confirmed by running the
   original beside the C++. Every new language gets the same.

## The rules every change follows

1. **The Minecraft regression gate.** Alpha and Beta are judged in full before and after every
   change: no class may go from alike to different or to not compiling, and the Alpha 360 build stays
   at 0 warnings and 0 errors. Today: Alpha 161 alike, Beta 145 alike.
2. **Behaviour, not text.** Emitted C++ may change shape freely. What it does is checked by the judge.
3. **One IR.** Front ends produce it, back ends consume it, and language-neutral work happens on it
   once. No back end reads source text, and no front end knows a target.
4. **The core names no game.** Anything that knows `Block`, `GuiMainMenu` or `splashes.txt` lives in a
   project profile (see "Project layer" below).

## The shape

```
  Java ─┐                                           ┌─ C++03 back end ─ 360 / other consoles
  C#   ─┼─► front end ─► IR ─► passes ─► IR ─► back end ┤
  ...  ─┘  (per language)       (shared)               └─ C++17 back end ─ PC
                                                          + runtime (per source language)
                                                          + platform layer (per target)
                 judge: the original's own runtime beside the C++ (JVM, .NET, ...)
```

### Front ends: let each language's own compiler parse it

The hard part of a front end is not syntax but meaning: overloads, generics, inference, implicit
conversions, which `x` a name refers to. Rewriting that for each language is years of work, and the
languages' own compilers already do it. So each front end is a small program in that language's own
toolchain that compiles the source and dumps its **typed** syntax tree as JSON, which Python lowers
into the IR.

- **Java:** a small `javac` plug-in using the compiler tree API (`com.sun.source.tree`, `Trees`): every
  expression with its resolved type, every name with its declaration, every call with its chosen
  overload. Any Java version the installed JDK reads (8-21+). The JDK is already a requirement for the
  judge.
- **C#:** a small Roslyn program (the .NET 9 SDK is installed). Same output shape.
- **Fallback:** source that does not compile (half-decompiled code, missing libraries) keeps today's
  text front end, per class, until it does. The Minecraft corpora compile as-is now that `java_libs`
  and the probe stand-ins exist.

### The IR: a typed tree, keeping the source's intent

Today's `ir.py` grows from declarations into whole programs:

- **Declarations:** modules, types (class, interface, struct, enum, record), generic parameters,
  fields, methods, properties and events (C#), constructors, static initialisers, nested and local
  types.
- **Statements:** block, local declaration, expression statement, if, while, do, for, for-each,
  switch (on ints, strings and enums, pattern form later), break and continue with labels, return,
  throw, try/catch/finally, using/try-with-resources, synchronized/lock, yield.
- **Expressions:** literals, names (resolved to a declaration), field access, calls (resolved to an
  overload), `new`, arrays, casts with kind (numeric, reference, boxing), unary and binary operators
  with their exact semantics (Java `>>>`, integer division by zero, overflow), conditional, lambda,
  method reference, instanceof/`is`, string concatenation, assignment and compound assignment.
- **Types:** primitives with exact widths and signedness, class references, arrays, generic
  parameters and instantiations, function types, nullable value types (C#).
- Every node carries its source file, line and column, so ledgers and the judge point back at the
  original.

The IR keeps intent and leaves lowering to the back end. A lambda stays a lambda: C++17 emits a
lambda, C++03 emits a functor class. A for-each stays a for-each.

### Passes: the work done once for every language

What is spread through the regex layer today becomes passes over the IR. Each one exists already as
text logic, so each port is checked against today's output.

- **Null model:** which references can be null, and value types that need a null state (the
  JavaString/JList/JMap work, now general).
- **Construction:** two-phase construction for virtual calls in constructors (`twophase.py`), and
  constructor delegation.
- **Ownership and escape:** what may be freed where (leak temporaries, scoped deletes, objects that
  hand out `this`) for targets without a collector.
- **Static initialisation order:** today's `j2x_initAll` dependency order.
- **Exceptions:** `finally`, null-pointer handlers, catch-by-pointer.
- **Reflection:** class registries into factories (`reflection_factory.py`).
- **Numeric semantics:** saturating float-to-int casts, signed bytes, unsigned shifts.

### Back ends

| | C++03 back end | C++17 back end |
|---|---|---|
| For | Xbox 360, other old consoles (`platforms.py`) | PC (Windows, Linux) |
| Generics | templates, or erased to a base pointer where today's code does | templates |
| Lambdas | generated functor classes | lambdas |
| Memory | today's ownership inference: owned fields, scoped temporaries, leak ledger | a collector (Boehm GC) or reference counting (see "Memory") |
| Strings | the runtime's string type | the same type, or `std::u16string` underneath |
| Projects | `.vcxproj` for the XDK (`console/vcxproj.py`) | CMake |

### Runtimes: one per source language, one platform layer per target

- **Java runtime:** today's `platform/canonical/` grows into a library in its own right:
  `java.lang`, `java.util` (with Java's own semantics: HashMap iteration order, `Integer` caching),
  `java.io`, `java.nio`, `java.util.regex`, `java.time`, `java.util.concurrent`. It is built in the
  order corpora need it, never speculatively, and each class is judged like game code.
- **.NET runtime:** `System`, `System.Collections.Generic`, `System.Text`, `System.IO`, LINQ, in the
  same style.
- **Shared core under both:** UTF-16 strings (the open difference: `length()` counts bytes today),
  boxing, exceptions, the memory model.
- **Platform layers:** what `platform/reference/` is for the 360 (Direct3D, XInput, XACT, storage),
  one per target. PC: SDL2 plus OpenGL. Others from the `platforms.py` profiles.

### Memory

This is the biggest design decision. Java and C# collect garbage, and C++ does not.

- **PC:** link a conservative collector (Boehm GC). Converted code allocates through it and never
  deletes. It is the most faithful option and the simplest to get right; the cost is a pause per
  collection.
- **Consoles:** keep today's ownership inference: owned fields freed in destructors, scoped
  temporaries, an explicit leak ledger. Add reference counting where the ownership pass cannot
  decide.
- The IR's ownership pass decides per allocation, and the back end chooses the mechanism.

### Project layer: everything that knows a particular program

Profiles today (`profiles/minecraft_*.py`) choose the era. Everything game-specific moves behind one
interface, a **project profile**:

- the console passes (`console/sound_fix.py`, `render_fix.py`, `controller_nav.py`, `applet_ping.py`,
  `achievements.py`, `xact.py`);
- overlays and hand fixes (`overlays.py`, `java_overlays`);
- name lists (`typemap.KNOWN_NULLABLE_GETTERS`, Minecraft class names in rules);
- the probe stand-ins (`probe_shims/`) for libraries a game shipped in modified form.

A program with no profile converts with the core alone. Minecraft keeps working through its profile.

### The judge, for every language

`probe.py` already runs a class's methods in the original's runtime and in C++, and compares results,
objects and fields. It becomes language-neutral with one adapter per source runtime: the JVM today,
`dotnet` for C#. The harness is generated from the IR rather than parsed from source.

**Corpora:**
- **Regression:** Minecraft Alpha and Beta, as above.
- **Java:** two or three open-source projects chosen for their language features rather than their
  size: one with heavy generics and collections, one with lambdas and streams, one that does real IO.
- **C#:** one XNA-era game or library and one ordinary library.

Each corpus's result (alike / different / does not compile / not judged) is tracked like Alpha's.

## Phases

Each phase ends with the Minecraft regression gate passing, plus its own exit check.

1. **Bodies into the IR, beside the old path.** Statement and expression nodes in `ir.py`; the javac
   front end; a C++03 back end that emits from the tree. Per class, the new path is used when the
   judge rates it at least as well as the old one. *Exit:* every Alpha and Beta class through the new
   path, the regex statement layer unused, results equal or better.
2. **Passes off the text.** The null, construction, ownership, static-order, exception and reflection
   logic moved onto the IR; `emit/statements/` deleted. *Exit:* the gate passes and the emitted C++ is
   diffed class by class against the last old-path output, every difference explained.
3. **Project layer.** Everything game-specific moved behind project profiles. *Exit:* a non-Minecraft
   Java program converts with no Minecraft code loaded.
4. **Modern Java.** Generics, lambdas, streams, records and switch expressions; the Java corpus
   above. *Exit:* each corpus converted, compiled and judged, and its numbers recorded.
5. **The C++17 back end and the PC platform layer.** Boehm GC, CMake, SDL2 and OpenGL. *Exit:* the
   Java corpus judged on the C++17 back end too, and Minecraft Alpha builds for the PC.
6. **C#.** The Roslyn front end, the .NET runtime, `dotnet` in the judge. *Exit:* the C# corpus
   converted and judged.
7. **More consoles.** Back-end and platform-layer work per console, starting with whichever one you
   pick.

Phases 1 and 2 are the largest. They are also where the risk is lowest, because the Minecraft gate
checks every step.

## Open questions for you

1. **Memory on the PC:** a garbage collector (most faithful, simplest) or reference counting
   (deterministic, slower to get right)? The recommendation is a GC.
2. **The corpora:** do you have Java or C# programs you want converted? Real targets beat sample
   projects as a corpus.
3. **Languages after C#:** C++ from Java and C# is natural because both are statically typed and
   garbage-collected. Dynamic languages (Python, JavaScript, Lua) need run-time typing in the output.
   That is a different project, and the plan leaves it out unless you want it.
4. **Which console after the 360?**
