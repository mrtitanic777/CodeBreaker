# Java to C# through the one C# back end (stage 15)

Stage 15 of docs/DOTNET_DECOMPILER_PLAN.md, item 4 of docs/ENGINE_UNIFICATION.md. It has three parts:

- Java reaches L2 from Fernflower's own result, not by printing Java text first.
- ILSpy's C# back end writes C# from that L2. Where an IL fact is missing it uses the plain form.
- `translate/backends/csharp` (`csharp.cpp`'s Writer) and `csharp_java` then retire.

## Where each part lives

```
 Fernflower (java/decompiler)            javac front end (java/frontend)       C# front end (translate/frontends/csharp)
   ClassWrapper, Statement, Exprent         TreeDump JSON                          Roslyn's tree
        |  java/decompiler/lower_l2 (new)         |  lower_unit                           |
        +------------------------+----------------+---------------------------------------+
                                 v
                 L2 units (translate::ir), Unit::lang "java" or "csharp"
                                 |
                                 |  rule set java (new, an L2 -> L2 pass): Java's library and semantics
                                 |  restated as .NET's. This is csharp_java's knowledge.
                                 v
                 C#-shaped L2: .NET types and members, Java semantics made explicit
                                 |
          +----------------------+------------------------------------+
          | CIL units (stage 14)                                      | every other unit
          | the builders read L2 + IL facts                           | the plain lowering, L2SyntaxBuilder (new):
          | (ExpressionBuilder, StatementBuilder, CallBuilder)        | L2 -> ILSpy's C# syntax tree, no type system
          +----------------------+------------------------------------+
                                 v
               ILSpy's C# syntax tree (dotnet/decompiler/csharp/syntax)
                                 |  the C# transforms that read only the tree (plain units: those that need no resolve result)
                                 v
               CSharpOutputVisitor + InsertParenthesesVisitor, with the back end's formatting options
                                 v
                               .cs files, ledger
```

The plain lowering is the "L2 -> C# syntax tree lowering in front of them" that ENGINE_UNIFICATION item 4 names. The builders
cannot do this job yet. Each fact they read is an IL fact: an `IType` of the type system, an `il::ILInstruction` the tree is
annotated with, or a pattern matched against the ILAst. A Java unit has none of these. Once a builder's cases read only L2 (stage 14
moved the facts, not the skeleton), they fold into the plain lowering. In the end there is one lowering, and it takes an IL fact
where the L2 has one.

## What each Java construct is in L2, and who writes it

The L2 shape is the one the javac front end gives today (`java/frontend.hpp`). Fernflower's lowering must give the same shape, so the
rule set and the back end read one form. "Rules" is the rule set java. "Plain" is the plain lowering. "Visitor" is the output visitor.

| Java | L2 | Back-end path |
|---|---|---|
| package, imports | `Unit{package, imports}` | plain: `NamespaceDeclaration`, `UsingDeclaration`s from the types the tree names |
| class, interface, enum, record, annotation | `ClassDecl{kind}` | rules: record -> sealed class with javac's members; enum of objects -> class. plain: `TypeDeclaration` |
| nested / inner / local / anonymous class | `ClassDecl{outer}`, `LocalClass`, `New{body}` | rules: hoisted to members of the outer class, with `outer_` and `cap_x` fields. plain: nested `TypeDeclaration` |
| field, initialiser, `static {}` | `FieldDecl{init}`, `MethodDecl{kind static_init / init}` | rules: Java's order (instance initialisers moved after the base call, statics in one static constructor). plain: `FieldDeclaration`, `ConstructorDeclaration` |
| method, constructor, `this(...)` / `super(...)` | `MethodDecl`, `CtorCall` | rules: `virtual` / `override` / `sealed`, `main` -> `Main`, Object's methods -> `ToString` / `Equals` / `GetHashCode`. plain: `MethodDeclaration`, `ConstructorInitializer` |
| generics, bounds, wildcards | `ClassType{args}`, `TypeParam`, `WildcardType` | rules: wildcards erased to their bound. plain: `SimpleType` / `MemberType` + `Constraint` |
| primitive and boxed types | `PrimType`, `ClassType java.lang.Integer` | rules: `Integer` -> `int`, `int?` where a null can reach it (boxed-null flow), `JLib.Unbox` reads. plain: `PrimitiveType`, `ComposedType{nullable}` |
| `String`, `List`, `Map` ... and their calls | `ClassType`, `Call{sym owner}` | rules: api_maps/java.csharp.json. Each template is read into L2 once, `{t}` `{0}` as holes, so the back end gets a call and never text. plain: the call |
| string `+` | `Concat` | rules: each part Java turns into text differently goes through `JavaRt.Str`. plain: `+` chain, or an interpolated string where the C# writer made one |
| arithmetic, shifts, `>>>`, integer division by zero | `Binary{op, overflow}` | rules: `>>>` -> unsigned shift, constant overflow -> `unchecked`. plain: `BinaryOperatorExpression`, `UncheckedExpression` |
| `instanceof`, pattern `instanceof` | `InstanceOf{binding}` | plain: `is` / `is T x` |
| switch on int / enum / String, arrow switch, switch expression | `Switch`, `SwitchExpr`, `Yield` | rules: fall-through -> `goto case`; an enum of objects switches on its ordinal. plain: `SwitchStatement`, `SwitchExpression` |
| labelled `break` / `continue` | `Labeled`, `Break{label}` | rules: `goto` a label after or inside the loop. plain: `GotoStatement`, `LabelStatement` |
| try / catch / finally, multi-catch, try-with-resources | `Try{resources}`, `Catch{types}` | rules: a multi-catch -> `catch (Exception e) when (e is A \|\| e is B)`. plain: `TryCatchStatement`, `UsingStatement` |
| `synchronized` | `Sync` | plain: `LockStatement` |
| lambda, method reference | `Lambda`, `MethodRef` | rules: the functional interface -> `Func` / `Action` / a `JFn` class for an interface of the program; a reference into Java's library -> the lambda it stands for. plain: `LambdaExpression`, method group |
| `assert` | `Assert` | rules: `System.Diagnostics.Debug.Assert` (Java's `-ea` off: dropped, as javac's run does) |
| arrays, `.length`, `new T[n][]` | `NewArray`, `Length`, `Index` | rules: a negative size throws Java's exception where the program catches it. plain: `ArrayCreateExpression`, `.Length` |
| `X.class`, reflection | `ClassLit` | rules: `typeof(X)` and the reflection shims |

What a construct can't be written as is declined, never guessed: the rule set throws `Decline` with a reason, and the method becomes
`throw new System.NotSupportedException("CB-GAP: ...")` with a ledger line, as the old writer does.

## Fernflower's result in L2 (java/decompiler/lower_l2)

Fernflower's final tree, after `ClassWrapper` and the class processors (enum, initializer, nested class, lambda, assert, finally),
lowers directly:

- `ClassStruct` / `StructMethod` / `StructField` + their signatures -> `ClassDecl`, `MethodDecl`, `FieldDecl` (types from `VarType`
  and the generic signatures).
- `Statement` kinds -> statements: Sequence -> statements, If -> `If`, Do (while / do-while / for / for-each / infinite) -> `While` /
  `DoWhile` / `For` / `ForEach`, Switch -> `Switch`, Catch / CatchAll -> `Try`, Synchronized -> `Sync`, a labelled statement ->
  `Labeled`.
- `Exprent` kinds -> expressions: Assignment -> `Assign`, Const -> `Lit`, Var -> `Name`, Field -> `Select`, Invocation -> `Call` /
  `New`, Function (the operators, casts, instanceof, array length) -> `Binary` / `Unary` / `Cast` / `InstanceOf` / `Length`, Array ->
  `Index`, New -> `New` / `NewArray` (with an anonymous body), Exit -> `Return` / `Throw`, Monitor -> `Sync`, Switch -> `SwitchExpr`.
- Each node keeps its bytecode span (`OriginSpan`), as the CIL lane does.

The proof is to lower each Fernflower class and compare it with the javac front end's lowering of Fernflower's own text for the
same class: the same tree, apart from source positions. Then the C# from both is the same.

## The plain lowering (dotnet/decompiler/csharp/l2_syntax_builder)

- **Declarations**: a `SyntaxTree` per unit, with `UsingDeclaration`s, a `NamespaceDeclaration` per package and a `TypeDeclaration`
  per class. Its members are `FieldDeclaration`, `MethodDeclaration`, `ConstructorDeclaration` (with its `ConstructorInitializer`),
  `PropertyDeclaration`, `EnumMemberDeclaration` and `OperatorDeclaration`. A type is an `AstType` made from the L2 type's names:
  `PrimitiveType` for the C# keywords, `SimpleType` / `MemberType` for classes, `ComposedType` for arrays and nullables.
- **Statements and expressions**: one syntax node for each L2 node. Each decision the old writer makes is kept, because the goldens
  pin it: names renamed in inner scopes, constants that overflow made `unchecked`, a `foreach` over the source's iterable, `goto
  case` for a fall-through, interpolated strings, expression-bodied members.
- **Text**: ILSpy's `CSharpOutputVisitor` writes it, with `InsertParenthesesVisitor` for the parentheses and the back end's
  formatting options (four spaces, the old writer's line breaks). No text is built by hand.

## Order of work and proof

1. This note.
2. The plain lowering, for units with no library (Unit::lang "csharp"). Proof: the C# goldens (`translate.csharp.corpus.*`, the
   unit tests' expected text) through the new path. Each one is byte-identical, or its diff is reviewed and the C# compiles and runs
   the same (`judge-cs-cpp` / the C#-to-C# pairs).
3. The rule set java as an L2 -> L2 pass, moved from `csharp_java.cpp` hook by hook, with the api_maps templates read into L2.
   Proof: `translate.csharp_java.*` through rules + plain lowering. The Java kit programs are judged against the JVM, compiled with
   Roslyn and run (the `java_to_csharp` pair, translate_file's Java -> C# judge).
4. Fernflower's lowering (java/decompiler/lower_l2). Proof: the same L2 as javac's lowering of Fernflower's text on the
   decompiler's test classes, and on RetroMCP's jar as far as the javac route goes today.
5. The switch: translate_file, `pairs::java_to_csharp` and the registered back end call the new path. `csharp.cpp`, `csharp_java.cpp`
   and their headers are deleted. The C#-to-C# pair (`pairs::csharp_to_csharp`) and translate_file's C# route go to the new path
   in the same step. `csharp_cpp17` and `cs2cpp` (C# -> C++) don't use the writer, so they are not affected.
6. Always: the .NET path stays ilspycmd's byte for byte (the 121 inputs of work/ilspy-s14/cs14.sh: 117 SAME and 4 known DIFF, all
   121 the same as before). The plain lowering is a new path, so it changes nothing the builders do.

## Status

- 2026-11-27: this note.
- 2026-11-27: step 2, the plain lowering (`dotnet/decompiler/csharp/l2_syntax_builder`), for units of lang "csharp". It writes the
  ten C# corpus programs, and each one builds with Roslyn and prints the same as its original (work/stage15/judge_cs.sh). Its
  goldens are `dotnet/testdata/l2_csharp/*.cs.txt` (`dotnet.l2_csharp.corpus.*`). The diff against the old writer's goldens was
  reviewed and is layout only:
  - a blank line between fields;
  - a constructor's `: base(...)` on its own line;
  - `where` on the declaration's line;
  - array, object and collection initialisers with complex items, and switch expressions, one item per line;
  - a long call chain broken before its last call;
  - a block lambda indented with its statement;
  - `record R;` for an empty record;
  - no parentheses around `??` in an interpolation, and `(int)-d` for `(int)(-d)`;
  - `quantity` where the old writer wrote `this.quantity` without a local to hide it.

  Expression bodies (`=> expr;`) are kept. They come from one non-upstream flag on `EntityDeclaration`, which the output visitor
  reads and ILSpy's builders never set. Nothing calls the plain lowering yet: the old writer stays the one in use until step 5.
- 2026-11-28: step 3, first part: the rule set java, `translate/rules/java_csharp`. It is an L2 → L2 pass over a copy of the
  units: it moves Java's types onto .NET's and sets dispatch modifiers and names. It also carries over:
  - construction order;
  - library calls through the API-mapping registry, its 140 C# templates read into L2 by a small expression reader with the
    call's own target and arguments in the holes;
  - Java's text conversions and reference comparisons;
  - labelled break and continue as goto;
  - try-with-resources with Java's suppressed exceptions;
  - a finally that jumps.

  `L2CSharpProgram` runs it when its options give the registry, and writes the runtime files the units use. Proof:
  `dotnet.l2_csharp.java.*` (goldens in `dotnet/testdata/l2_csharp_java`). calc, fixtures, modern and features build with Roslyn
  and print what the old translation prints (work/stage15/judge_java.sh). calc, fixtures and modern also print what the JVM prints
  for their Java sources. calls, exception_cause and hash_order build. The diff against the old goldens is layout (as in step 2),
  plus `var` where the declared and made types are now the same .NET type, and plain concatenation where the old writer also
  wrote it for Java.

- 2026-11-28: step 3, second part: what the rule set declined by name is carried, each as the old writer does it, in L2:
  - java.util.stream, Optional, Collectors and a Comparator's combinators (the runtime's JStream ...), with String.format, Map's
    merge and compute, List.of, Set.of, Map.of and Arrays.asList;
  - boxes that may hold a null: the old writer's flow over the program finds them; each is `int?`, read through `JLib.Unbox`
    where Java reads the number;
  - classes declared in a method and anonymous classes, hoisted to members of the class around them with their captured locals
    (`cap_x`), and inner classes that reach their outer instance (`outer_`);
  - lambdas of the program's interfaces, through the interface's class `JFn`;
  - enums of objects, as classes of `JEnum`;
  - Comparator classes (an implicit conversion to Comparison) and Iterable classes (GetEnumerator);
  - switch-expression arms with statements, as a function called on the spot;
  - CodeBreaker's reflection classes (Class, Constructor), of no type in C#.

  Two changes to the plain lowering come with it: a class nested in a generic one has the outer's arguments on the outer's name
  (`Op<int>.JFn`), and a method named `Iface.Name` is an explicit implementation. Proof: every Java program translate.capability
  takes to C# (the kits, Shapes, Recs, Streams, Printable) goes through translate_file's Java to C# route on the new path and gives
  what the test wants of the old one: the JVM's output, or the refusal (AsListKit, Fixed, Early) or the divergence (TreeBin,
  Shuffled) it checks for.
