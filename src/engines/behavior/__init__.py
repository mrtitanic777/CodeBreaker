#!/usr/bin/env python3
r"""behavior_engine -- the BEHAVIORAL RECONSTRUCTION engine (dynamic evidence).

Where the rest of CodeBreaker reconstructs a binary *statically* (bytes -> IR -> C ->
recompile -> byte-diff), this engine reconstructs it *experimentally*: it runs the real
machine code as an oracle, observes what it actually does, and preserves those
observations as structured, queryable evidence. It is the dynamic half of the
"use the binary itself as the oracle" idea -- a generalization of the pass/fail Unicorn
harness in recompile95/tools/difftest.py into full behavioral recording.

v1 (this slice) delivers the foundation everything else reads from:

    tracer.py   run one function in Unicorn with instruction + memory hooks, producing
                one structured TraceRecord (the execution-trace schema): the path taken,
                every memory read/write, detected calls, faults, and the return value.
                Unlike difftest (which dies on a pointer deref, so it only covers PURE
                LEAF functions), the tracer LAZILY MAPS a zeroed page on an unmapped
                access and records the fault as evidence -- so it can trace the
                memory-touching majority too.
    corpus.py   persist TraceRecords into a durable, append-only evidence corpus under
                <data>/behavior/ (one latest file per function + a running JSONL log),
                so experiments accumulate indefinitely.

Later slices grow on top of this corpus: an execution/dependency graph, competing
hypotheses with confidence, and active experimentation (choosing inputs that
disambiguate hypotheses). They read evidence; they do not re-run the loader.

    wt behavior trace <module> [func]      # record a trace into the corpus
    wt behavior show   <module> [func]     # pretty-print a recorded trace
"""
