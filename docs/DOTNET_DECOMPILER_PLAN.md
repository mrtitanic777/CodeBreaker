# The .NET decompiler: ILSpy ported to C++

CodeBreaker turns a .NET program (any PE with a CLR header, 32- or 64-bit, an .exe or a .dll) back into C# with a
decompiler of its own: ILSpy's engine, `ICSharpCode.Decompiler` (MIT), ported whole to typed C++20 the way Fernflower
was ported to `src/java/decompiler`. Today the Convert route `dotnet` and the XNA lane start the installed `ilspycmd`,
which needs .NET; when this is done neither is needed.

## What is ported, and from where

- Upstream: github.com/icsharpcode/ILSpy, tag **v11.0** (commit cdeae656f), the same version as the `ilspycmd`
  11.0.0.9375 on this computer, so the installed tool is the oracle for every stage. The source is kept for reference
  in `work/ilspy-port/upstream/` (not in the repository).
- Where it goes: `src/dotnet/` (a new area, `cb_area(dotnet)`, namespace `cb::dotnet`), in two parts, as Java is
  (`java/classfile` reads, `java/decompiler` is the port):
  - `dotnet/metadata`: the reader of ECMA-335 metadata. ILSpy does not read metadata itself; it uses .NET's
    `System.Reflection.Metadata`, which is part of .NET, not of ILSpy. This part is CodeBreaker's own, written from
    the ECMA-335 standard on top of `formats/pe` (which already finds the CLR header), with the same names as
    System.Reflection.Metadata where the port calls it (MetadataReader, handles, BlobReader, SignatureDecoder) so the
    ported code reads like the original.
  - `dotnet/decompiler`: the port. A `NOTICE` keeps ILSpy's copyright and MIT licence; every header says which
    upstream classes (namespace and file) it ports, as `java/decompiler/*.hpp` do.
- Size: ILSpy's engine at v11.0 is 86,000 lines of C# code (blank lines, comments and lone braces not counted; 164,000
  with them). The port is about the same in C++: **about 85,000 to 95,000 lines**, plus the metadata reader. (The
  first estimate, 24,500, was too small by four: it is the size of the IL-to-C# core alone, without the type system,
  the C# syntax tree and the resolver, which ILSpy cannot run without.)

## Stages, in order

Each stage ends with something that runs and its area tests (`tools\build_cb.cmd area dotnet`). Sizes are upstream
C# code lines and the expected C++.

| # | Stage | Upstream | C# | C++ (est.) | What runs at its end |
|---|-------|----------|---:|---:|---|
| 0 | ECMA-335 metadata reader | (System.Reflection.Metadata, our own) | — | 6,000 | every table, heap and signature of a test assembly, dumped and compared with the oracle's dump |
| 1 | ILSpy's metadata layer | `Metadata/` (MetadataFile, PEFile, assembly resolver, signature providers, `SRMExtensions`, `SRMHacks`) | 3,800 | 4,000 | assemblies and their references found and opened as ILSpy opens them |
| 2 | IL disassembler | `Disassembler/`, `Output/` (ITextOutput, PlainTextOutput) | 3,200 | 3,300 | **first slice:** `codebreaker dotnet il <asm>` writes the same IL text as `ilspycmd -il`, byte for byte |
| 3 | Type system | `TypeSystem/` (+ Implementation), `Semantics/`, `Util/` | 13,450 | 13,000 | types, members, generics and substitution of an assembly and its references; a dump compared with the oracle |
| 4 | IL AST | `IL/` root (ILReader, BlockBuilder, ILVariable, the instruction classes that `Instructions.tt` generates) + `IL/Instructions/` | 12,400 | 11,000 | each method read into ILSpy's ILAst; the ILAst text compared with the oracle's |
| 5 | Control flow | `IL/ControlFlow/`, `FlowAnalysis/` (dominance, loops, conditions, switches, exit points, yield and async state machines, definite assignment) | 6,300 | 6,500 | structured ILAst (blocks, loops, if, switch) as the oracle's after the same transforms |
| 6a | IL transforms, core | `IL/Transforms/`: inlining, split variables, copy propagation, expression transforms, assignments, dead code, the pipeline | 6,500 | 6,500 | the ILAst after the core pipeline matches the oracle's |
| 6b | IL transforms, C# features | `IL/Transforms/`: delegates and lambdas, local functions, object and collection initializers, null propagation, string interpolation, using/lock/foreach patterns, tuples, ranges | 6,700 | 6,800 | every transform's ILAst matches |
| 7 | C# syntax tree and writer | `CSharp/Syntax/`, `CSharp/OutputVisitor/`, `CSharp/TypeSystem/`, TypeSystemAstBuilder | 10,500 | 10,500 | declarations of every type (signatures, attributes, no bodies) written as ILSpy writes them |
| 8 | C# resolver | `CSharp/Resolver/` (CSharpResolver, conversions, overload resolution, type inference) | 5,350 | 5,500 | casts and overloads chosen as ILSpy chooses them |
| 9 | C# from ILAst | `CSharp/` root: CSharpDecompiler, ExpressionBuilder, StatementBuilder, CallBuilder, RecordDecompiler, TranslatedExpression, `DecompilerSettings` | 9,200 | 9,500 | **second slice:** simple methods as C#, then whole types |
| 10 | C# transforms | `CSharp/Transforms/` (declare variables, pattern statements, using declarations, operators, prettify) | 3,900 | 4,000 | `ilspycmd`'s C# for a whole assembly, byte for byte |
| 11 | Project output | `CSharp/ProjectDecompiler/`, resources, `Humanizer/`, `Documentation/` (XML doc comments) | 2,550 | 2,700 | `ilspycmd -p` (a .csproj and a .cs per type) |
| 12 | The switch | `src/app/convert_backends.cpp` (route `dotnet`), `src/xna/run.cpp` | — | small | both call `cb::dotnet::decompile`; no `ilspycmd`, no .NET; TESTED_SOFTWARE.md updated |

Total: about 83,000 C# lines, about 89,000 C++ lines with the metadata reader.

Not ported, as nothing CodeBreaker does needs them: PDB writing (`DebugInfo/`), the solution writer (`Solution/`),
`Instrumentation/`, `DebugSteps/`, single-file bundles (`SingleFileBundle.cs`) and WebCil. Any can follow later.

## Status

- Stage 0 (done 2026-11-26): `dotnet/metadata`; its dump matches System.Reflection.Metadata's line for line on the test assemblies, CoreLib 9,
  Framework 2.0, WinForms and XNA 3.1.
- Stages 1 and 2 (done 2026-11-26, except the universal assembly resolver and debug information): `dotnet/decompiler/{metadata,
  disassembler,il,output,typesystem,util}`, about 3,100 lines (and .NET's Unicode category table); `show_il` is `ilspycmd -il` byte for byte on the test assemblies and seven
  real ones up to CoreLib's 1.39 million lines, ten times as fast. .NET's own text rules it needs (Unicode categories, the "R" number
  format) are copied from .NET by `testdata/oracle/NetText` and proved on 16,000 values.
- Stage 3 (done 2026-11-26): the type system, `dotnet/decompiler/typesystem` (about 12,000 lines): ILSpy's interfaces, the made types
  (arrays, generic instances, tuples, function pointers, nullability ...) kept once each in a per-compilation arena, specialized and fake
  members, the Metadata* entities, MetadataModule's resolution of references, attributes (custom ones decoded when read, the ones C#
  writes from metadata flags made up), the compilation (SimpleCompilation, MinimalCorlib, merged namespaces) and DecompilerTypeSystem,
  with the universal assembly resolver (`decompiler/metadata/universal_assembly_resolver`) and System.Reflection.Metadata's TypeName
  parser (`dotnet/metadata/type_name`). `testdata/oracle/TypeSystemDump` writes ILSpy's own type system as text; ours matches it line
  for line on the test assemblies (the checked-in golden, no references) and, with references resolved as ILSpy resolves them, on
  System.Linq, System.Text.Json, System.Private.Xml, CoreLib 9 (190,000 lines), mscorlib 2.0 and 4.0, System.Core, System.Xml,
  WinForms, WPF's PresentationFramework, Microsoft.VisualBasic, XNA 3.1's Framework and Game, Roslyn's Microsoft.CodeAnalysis.CSharp
  and ICSharpCode.Decompiler itself. Found on the way: our SRM ordered child namespaces differently from SRM's NamespaceCache (fixed).
  Not yet ported: ExtensionInfo (C# 14 extension blocks), the PDB's extra type information, DecompilerSettings.

## How each stage is proved

- **The oracle.** A small C# program, `src/dotnet/testdata/oracle/` (like Java's `GraphDump.java`), built against
  ICSharpCode.Decompiler 11.0 from NuGet, dumps what each stage makes (metadata tables, type system, ILAst after a
  named transform, C# text). Its dumps are checked in as goldens, so the tests never need .NET; only refreshing a
  golden does.
- **The inputs.** ILSpy's own test cases (`ICSharpCode.Decompiler.Tests/TestCases/Pretty`, 115 files, MIT), compiled
  once with Roslyn and kept as small .dll test inputs: for these the expected C# is the test case itself. Then real
  programs: the XNA games already in docs/TESTED_SOFTWARE.md (Arcadecraft, Block World), compared with what
  `ilspycmd` writes for them.
- **The finish line.** For every test assembly, the C# the port writes is the C# `ilspycmd` 11.0 writes, byte for
  byte; then the Convert route and the XNA lane switch over and the `ilspycmd` setting goes.

## Rules for the port (as for Fernflower)

- Typed, idiomatic C++20: classes for ILSpy's classes, `std::unique_ptr` / arenas for the trees (the ILAst and the
  syntax tree are owned trees with parent links), `std::variant` or virtual visitors where C# uses visitors,
  `std::string` (UTF-8) for text, never slower than it needs to be. Names in CodeBreaker's style; each header lists
  the upstream classes it ports.
- Upstream's order of iteration is kept wherever output depends on it (C# `Dictionary` keeps insertion order in
  practice; the port uses ordered containers there), so the text matches byte for byte.
- `Instructions.cs` is generated upstream from `Instructions.tt`; the port writes the instruction classes by hand
  from the generated file, as readable C++, not a generator.
