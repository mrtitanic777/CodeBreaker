# Fewer gotos: the structuring plan

docs/CORE_ROADMAP.md, Track 1 item D. C13-C18 (docs/GHIDRA_FORK.md) took the golden corpus from
6,923 gotos to 5,034. About 1,980 remain outside COMMAND.COM (hand-written assembly: its C is never
the deliverable). They are no longer near-misses of a shape the structurer knows. They are flows
C cannot write without a goto unless the code is reshaped first.

## Where gotos come from

`CollapseStructure` (native/decompiler/core/cpp/blockaction.cc) folds the flow graph with its
shape rules -- ruleBlockCat, ruleBlockOr, ruleBlockProperIf, ruleBlockIfElse, ruleBlockWhileDo,
ruleBlockDoWhile, ruleBlockInfLoop, ruleBlockSwitch, ruleCaseFallthru -- until one block is left.
When no rule fits, `selectGoto()` marks an edge as a goto and folding goes on. Every goto is born
there.

## Approach: extend, do not replace

The rules, the printing and C13-C18 stay. Where `selectGoto()` would give up, a restructuring step
reshapes the region that is stuck so the rules can fold it; only if that fails is a goto marked.
Each step is behind a switch for A/B runs, and is its own core change (C19, C20 ...).

## Phases

0. **Measurement.** Gotos counted by class (`tools/goto_report.py --classes`). A fixed regression set
   of real functions per class from the golden corpus. A synthetic set: small C programs written to
   have each shape, compiled, decompiled, recompiled and run against the original.
1. **Loop exits past code** (about 450). C18 generalised: the code a loop's normal exit runs before
   the place a goto from inside also reaches goes into the loop on that exit path. Tried on a copy
   of the graph and kept only when the function's gotos drop (a region version traded gotos).
2. **Out of nested blocks, into a sibling** (about 890). For an acyclic region that will not fold:
   copy a small join block per way in (within a code-growth budget), or nest it under the combined
   condition (`if (a && !b)`, BlockCondition) when that condition can be evaluated again safely; else
   the goto stays. No new flag variables yet: making variables this late is the riskiest change.
3. **Loops with a second entry** (about 590). Node splitting: copy the part of the loop from the
   second entry to the head, so the loop has one entry (C17's region copy, made general for a region
   that flows on). A size limit: above it the goto stays.
4. **Authentic gotos.** `goto cleanup` -- one label reached only from error paths -- is kept: the
   source is to read as written.

## Each step is shown safe

Gotos by class, no module worse; golden: no new failures, one thread and four agree; behaviour
(`measure` + tools/verdict_diff.py) on KERNEL32, GDI32, USER; the synthetic set; for 16-bit, the
Win16 gate and the VM. The PowerPC and DOS lanes share the core and gain too.

## Expectation

Four to six weeks, phase by phase. A first target: half of the ~1,980. The last few hundred are
likely the ones where a goto is the clearest C.
