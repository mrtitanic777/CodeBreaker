# The compiler middle on L2 (ENGINE_UNIFICATION.md item 6)

One optimizer and one code generator under every language, from L2 down. L2 is `translate::ir` (src/translate/ir.hpp), the
typed tree the decompiler, the translators' front ends and the .NET and Java lanes emit. The first back end is x86cc
(src/build/x86cc): OpenWatcom's C compilers wcc386 (32-bit) and wcc (16-bit), compiled from their own source into CodeBreaker
and run in-process. x64 stays with MSVC and ml64 (the 2026-11-26 exception: OpenWatcom has no Win64 code generator), reached
as it is today, through C text.

## What x86cc is made of

A compile is four parts, each with its own run state (run/cbrun.hpp): the front end (`FrontRun`: scanner, preprocessor,
declarations, expressions, statements, pragmas), the code generator (`BackRun`), the inline assembler and the runtime support.
Between the scanner and the object there are three representations:

1. **The front end's tree.** The parser does not build a syntax tree and then type it: it calls semantic actions as it reads
   (`AddOp`, `BinOp`, `RelOp`, `IntOp`, `ShiftOp`, `AsgnOp`, `CnvOp` in expr/cmath.cpp; the leaves `VarLeaf`, `IntLeaf` in
   expr/cexpr.cpp), and each statement becomes a typed expression tree (`TREEPTR`, opcodes `OPR_*`) appended to the function's
   list by `AddStmt` (stmt/cstmt.cpp): `OPR_FUNCTION`, `OPR_LABELCOUNT`, `OPR_RETURN`, `OPR_JUMPFALSE`, `OPR_LABEL`,
   `OPR_FUNCEND` ... The usual arithmetic conversions, implicit casts, constant folding (expr/cfold.cpp) and the symbol weights
   the register allocator later reads (`IncSymWeight`: a use inside a loop counts 16 times more) are all done here.
2. **The cg interface.** output/cgen.cpp walks those lists and calls the code generator: `CGProcDecl`, `CGParmDecl`,
   `CGAutoDecl`, `CGFEName`, `CGInteger`, `CGBinary`, `CGUnary`, `CGAssign`, `CGCall`, `CGCompare`, `CGFlow`, `CGControl`,
   `CGReturn`, the data calls `DG*` and the segment calls `BE*` (codegen/interface/cgprotos.h). This is OpenWatcom's real
   middle: its C, C++ and FORTRAN front ends all drive the same code generator through it. The code generator asks the front end
   back through the callbacks of codegen/interface/cgfertns.h: `FEName`, `FEExtName`, `FEAttr`, `FEAuxInfo` (calling
   conventions, registers saved, pragmas), `FESegID`, `FEBack`, `FEParmType`, `FEDbgType` ... (front/driver/cfeinfo.cpp).
3. **The code generator's instructions.** codegen/tree folds and addresses the cg calls' trees, and codegen/interface/generate.cpp
   lowers each routine into blocks of instructions and runs the passes below, then the x86 encoder writes OMF.

## The passes there are, and where they run

| Where | Pass | Source |
|---|---|---|
| front end's tree | constant folding (`FoldExprTree`, `DoConstFold` on every `AddStmt`) | front/expr/cfold.cpp |
| front end's tree | inline expansion of small functions (`-oe`), pruning unreferenced static functions | front/output/cgen.cpp |
| cg calls' trees | folding at construction, address folding, the address modes | codegen/tree |
| instructions | `FixReturns`, `BlockTrim`, `TailRecursion`, `InsDead`, `FixMemRefs` | codegen/ir, codegen/optimize |
| instructions | `PreOptimize`: common subexpressions, propagation, load/store, multiply by constants | codegen/optimize |
| instructions | `PropNullInfo`, conflicts, `SplitVars` and the loop optimizations (`-ol`: induction variables, invariants, unrolling) | codegen/optimize, codegen/analysis |
| instructions | `AxeDeadCode`, `FixIndex`, `FixSegments`, x87 and integer register allocation, `PostOptimize` | codegen/regalloc, x86/x87 |
| instructions | the stack map, block order, prologue and epilogue, instruction scheduling (`-or`), the peephole optimizer | codegen/schedule, codegen/peephole |

What runs on L2: nothing, to begin with. Everything after the cg interface is machine work and stays in the code generator.
The front end's passes are language rules (C's conversions, C's folding), so they stay where C semantics are applied. A pass
moves up to L2 only when a second back end needs it (the C++17 or C# writers, the 16-bit target, a native compile of Java or
C#) and only when moving it is proven: the front end's copy switched off and L2's switched on give the same objects on the
corpus. The candidates, in the order they are likely to be needed: constant folding with L2's stated rules (`Overflow`,
`Binary::is_signed`), unreferenced-function pruning, tail-call marking, inlining decisions.

## How L2 reaches it: two seams, in order

**Seam 1, the front end's semantic actions (now).** L2 is lowered into the front end's tree by calling the same semantic
actions the parser calls, in the same order, from the typed tree instead of from tokens. The interface is
`x86cc::BodyBuilder` (build/x86cc/l2body.hpp), which knows neither OpenWatcom's types nor translate::ir's: `name`, `integer`,
`binary`, `return_value` ... ; the front end implements it (front/stmt/cl2body.cpp) and the L2 side drives it (build/l2cc). A
compile on this seam is a normal compile of a C unit whose function bodies are empty (`int add(int a, int b) {}`): the
declarations, types, prototypes and pragmas go through the front end's declaration parser as text (the text L2 already
carries for a C unit: `Module::header`, the prototypes the C back end writes), and when the parser reaches a body that L2 gives,
`Statement` is replaced by the builder's walk of the tree (`L2Statement`). From the front end's tree down nothing changes, so
an L2 function whose tree matches the C text's gives the same object by construction, and the proof is a byte compare.

Why this seam first: identical objects are the strongest proof there is, and they are only reachable if every detail the C
parser applies (conversions, folding, the symbol weights that steer register allocation, label numbering, the end-of-function
label and dead-code flags) is applied by the same code. A lowering straight into the cg interface would have to re-create all
of them before its first identical object.

**Seam 2, the cg interface (later).** L2 is lowered straight into the cg calls, with the front-end callbacks answered from L2's
symbols and types (an L2 `FrontRun` beside the C one: names, attributes, calling conventions from `FnType`, segments from
`SegmentRef`). This is the seam for L2 that did not come from C: a Java, C# or Python program compiled natively with no C
semantics in between, where L2's own rules (Java's wrapping `int`, C#'s checked arithmetic) are stated in the tree. Its proof is
behaviour (the equivalence judges), and for L2 that did come from C, identical objects against seam 1.

The readable source CodeBreaker hands over stays C text (the premise: readable, compiles, behaves as the original). The text
path keeps proving that the deliverable compiles; the L2 path is proven to give the same object as the text path, so either
proves the other, and the L2 path saves writing and re-scanning the bodies.

## The targets

1. wcc386, 32-bit flat (Win32, Win95 PE, DOS extenders): first.
2. wcc, 16-bit segmented (Win16 NE, DOS): the same front end source, so the same `L2Statement` and builder compiled into
   cb_wcc_i86; near, far and huge pointers come from L2's `PtrType::space` and `SegmentRef`, the memory model from the unit's
   `CompilerTarget` and the compile's switches.
3. x64: MSVC and ml64 through C text, as today. A Win64 code generator for OpenWatcom is not part of this item.

## The proof

1. **Identical objects (seam 1).** For every function of a corpus, compile it both ways in the same folder, from a file of the
   same name and time (the object's module name and dependency record carry them), with the same switches: today's path (L2 to
   C text by translate/backends/c, then x86cc) and the L2 path (declarations as text, the body from the tree). Report, per
   corpus, functions the same, declined (a construct not lowered yet: today's path is used) and different (a bug; never
   accepted). The corpora: the decompiler's L2 documents of lz32.dll (93 functions) and the 16-bit lzexpand.dll (46)
   (`translate.l2.decompiler_documents_lose_nothing`), then the captured compiles of test-inputs/x86cc/gate (177), then the
   Win95 module builds.
2. **Identical behaviour**, where objects differ on purpose (a pass moved to L2 with a different result, seam 2, languages other
   than C): the per-function equivalence judge on the Unicorn oracle, and at milestones the MINI.CAB boot to Setup.
3. **Nothing else moves.** x86cc.replay (every captured compile the same in-process) stays 177 of 177: with no L2 bodies given the
   front end runs exactly as before.
4. **Never slower.** Compile time both ways over each corpus, in the report of each step.

## Step order

1. This document.
2. The first vertical slice: `BodyBuilder` with names, integer literals, the binary arithmetic, bitwise, shift and comparison
   operators and `return`; `L2Statement`; `l2cc::compile` for one `MethodDecl`; one small function the same object both ways.
3. Statements: locals, assignments and compound assignments, `if`/`else`, the loops, `break`/`continue`, labels and `goto`,
   `switch`.
4. Expressions: calls (prototyped), casts, unary operators, pointers, indexing, fields, `&&`/`||`, `?:`, the comma.
5. The corpus harness on lz32 and lzexpand's L2 documents: the same / declined / different, and the times.
6. wcc (16-bit): the builder in cb_wcc_i86; far and near pointers, segments; the 16-bit corpus.
7. Declarations from the tree (types, prototypes, globals, struct layouts through the front end's declaration actions), so no
   text is scanned at all.
8. Seam 2: L2 into the cg interface, the callbacks from L2; a Java or C# program compiled natively, judged by behaviour.
9. The build lanes compile through L2 by default wherever the corpus shows the same objects; the text path stays the check of
   the deliverable.

## Status

- 2026-11-28: this plan.
