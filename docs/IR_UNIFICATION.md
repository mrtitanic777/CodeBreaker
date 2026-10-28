# One IR, in layers: the decompiler and the translator share a hub

Design note, 2026-10-19. Applies to docs/TRANSLATOR_DESIGN.md (branch `design-multilang`) and
docs/CORE_ROADMAP.md Track 2. Nothing here is built; it is a decision to make **before translator
phase 1 fixes the IR's shape**.

## The problem

Two intermediate representations are forming independently:

- **The translator's hub IR** (`codebreaker/translate/ir.py`): a typed tree holding every source
  language (declarations, statements, expressions, types, memory model).
- **The planned Rust core's SSA IR** (CORE_ROADMAP Track 2): values, ops, memory spaces, for
  analysis and decompilation of binaries.

And the translator's binary path is planned as:

    binary -> decompiler -> C text -> clang front end -> hub IR

That is a lossy round trip. The decompiler *knows* each value's type, segment, convention and
structure layout, prints it as text, and clang then has to re-derive it — or fails on
half-decompiled code, which is why a text front end is kept as a fallback. It is the
`c_text.py` regex problem again, one level up. Two IRs would also mean two type systems, two sets
of passes (numeric semantics, structures, pointer discipline) and two places to fix every bug.

## The decision: one IR family, three levels

| Level | What it is | Produced by | Consumed by |
|---|---|---|---|
| **L0 machine** | P-code: exact instruction semantics | SLEIGH (verified, SLEIGH_VERIFY.md) | the lifter, `pcodecheck` |
| **L1 low** | SSA over typed values: memory spaces and segments, calls with conventions, no structure yet | the lifter (Ghidra core today, Rust core later) | data flow, types, structures, structuring |
| **L2 hub** | the translator's typed tree: declarations, statements, expressions | **every front end — and the decompiler** | every pass and back end |

The decompiler becomes **one more front end**: it emits L2 directly, after structuring. C text is
then produced by the hub's C back end, the same as for any other source. So:

    binary -> L0 -> L1 -> structuring -> L2 -> back end -> C / C++ / Java / Rust / ...

A decompiled function can go straight to any target, judged by the same verdict store.

## What L2 must hold so that binaries fit (add these now, while it is small)

1. **Pointers with an address space**: near, far (selector:offset), huge, real-mode `seg*16+off`,
   and I/O ports. Phase 2's segment types *are* L2 types; build them once, here.
2. **Explicit layout**: a structure field's byte offset, its packing, and the size of the whole,
   fixed rather than computed (as `struct_align` does). Needed for `/*@S` structures and for frames.
3. **Calling conventions on function types**: cdecl, pascal, stdcall, register arguments, results in
   DX:AX, callee-pops-N.
4. **Exact integer semantics everywhere**: 16-bit int, signed char, wrap versus saturate — the
   `arith16` rules belong in the IR's operators, not in the emitter.
5. **Emulated memory** as a memory model alongside GC, owned, borrowed and stack, for pointers the
   analysis cannot discipline (C sources and binaries alike).
6. **Opaque machine fragments**: a function or block that stays as assembly ("what C cannot carry
   stays asm") is a node, so the C back end prints inline asm and other back ends report it in the
   ledger instead of losing it.
7. **Provenance**: next to source file, line and column, a node can carry *module + address*, so
   the judge, the ledger and the workbench point at the original bytes.

`goto` is already in L2's statements. Keep it: structuring removes what it can (CORE_ROADMAP D).

## How to get there without stopping anything

1. **Write L2 down as a schema, not only as Python classes**: JSON Schema, Cap'n Proto or
   FlatBuffers. Python (`ir.py`) implements it now; the Rust core implements the same schema later
   and serialises across PyO3 or a file. This one step keeps the two worlds from drifting.
2. **Bridge from the Ghidra core first.** The core already emits marked-up tokens tied to its
   varnodes and ops (the `Emit`/`EmitMarkup` family behind PrintC). Add a `PrintIR` beside PrintC
   that writes the structured function as L2 JSON. Run it next to the C path on the golden corpus;
   the hub's C back end must reproduce today's C, gated by `tools/golden.py` and the emulator judge.
3. **Retire `c_text.py` into the C back end.** Each remaining pass is either an L2 pass (shared by
   all targets) or a C idiom rule. The 16 regexes left are the to-do list.
4. **Track 2 targets L1 and L2**, not a private IR: the Rust core's job is lifting, L1 analysis and
   structuring, ending in L2. It never needs its own emitter.
5. **One judge.** The emulator verdicts (binaries) and the runtime-beside verdicts (source pairs)
   already share the store; keep a single verdict vocabulary (alike, different, weak, not compiled,
   untested) across both.

## Risks

- **A kitchen-sink IR.** Keep a small *core* node set; everything else is a flagged extension with
  a lowering pass into the core, so a back end only has to know the core.
- **The C back end must match today's C first.** Until it does, the old path stays and the golden
  diff decides — the same rule the roadmap already uses.
- **Order.** L2's binary-facing types (items 1-4) overlap phase 2. Do them as one piece of work, not
  twice.

## Decisions (user, 2026-10-19)

1. **Adopted:** the three levels, with the decompiler as an L2 front end.
2. **Schema:** L2 is defined in **JSON Schema** first — readable, diffable in golden runs, and
   supported natively in Python and Rust (serde). A binary encoding of the same schema
   (FlatBuffers or Cap'n Proto) is added later only if size or speed demands it.
3. **Adopted:** phase 2's segment types are built directly as L2 types, once.

## Where it stands, and the order of work (2026-10-26)

Built: the L2 schema (`codebreaker/translate/l2.schema.json`) and its Python side
(`translate/l2.py`: to_json, from_json, validate); the Java front end and the C++03 back end use the
hub tree; the native decompiler's PrintIR, first stage (`CB_L2=1`): each function's signature and
types as an `/*@L2 ... */` note beside its C. Not built: function bodies in L2, a C back end, and
any c_text pass moved onto the tree. `decompile/c_text.py` is still 2,936 lines with 19 regular
expression rewrites (and `ctree.py`, a tree-sitter parse of the C, for the passes that moved off
regexes); `source/readable.py` has 10 more.

The order, each step gated by `tools/golden.py` and the emulator judge before the next:

1. **PrintIR bodies.** Statements and expressions of the structured function as L2, from the same
   markup PrintC walks, behind `CB_L2=1`. Done when every function of the golden corpus gives a
   document `l2.validate` accepts.
   **Done 2026-10-27.** `PrintL2` in `native/decompiler/decompiler.cc` is PrintC with its output
   discarded: its block emitters build If/While/For/DoWhile/Switch/Goto/Labeled statements, and each
   expression is rebuilt as a tree from the core's expression stack through two hooks (fork change
   C19, `placeOp`/`placeAtom`), so the tree has PrintC's casts, fields, indexing and negated
   conditions. `python tools/l2check.py`: all 6,808 golden functions the core decompiles give a valid
   document, with no Unsupported node left (45 more fail in the core itself and have no C either).
   The C is unchanged: `tools/golden.py check` gives the same differences as master's build. (Not yet carried: comments, and the address of each statement as `origin`.)
2. **The C back end** (`translate/backends/c.py`): L2 to C that compiles with OpenWatcom. Done when,
   on the golden corpus, it compiles everywhere today's C compiles and the judge gives the same
   verdicts. Today's path stays the default until then.
   **Done 2026-10-28.** `translate/backends/c.py` writes an L2 function document as C in the
   decompiler's own dialect, from PrintC's own rules: the core's operator table and its bracketing
   rule (`PrintLanguage::parentheses`: an operand of equal precedence is bracketed unless it is the
   same associative operator, `(a + b) - c`), the brackets of conditions the structure joined
   (`(a) && (b)`, none on the right side or in a loop's comma list), `if (c) goto x;` braceless.
   What only PrintC knew, PrintL2 now writes into L2: a literal's spelling (`Lit.text`: base, `U`,
   character escapes; a schema addition), the convention's word where it is not the default
   (`MethodDecl.mods`), the locals in PrintC's order and with PrintC's names for nameless types
   (`unkint3`), an if-goto's jump as one statement rather than a block, an `else` that only holds an
   `if` as a Block. `python tools/l2c.py` holds it to the printed C token for token and in its
   spacing, which c_text's passes read (`xunknown1 axStack_4c [52];`: written `a[52]`, the frame
   pass missed it and one calc.exe body went wrong): 6,807 of the 6,808 golden functions the same
   (the one left, VKD 1:0922, brackets across a hidden cast L2 does not show). With
   `CB_C_FROM_L2=1` the decompiler's C for each function is the back end's
   (decompiler.c_from_l2). `python tools/l2c.py --made MODULE ...` takes a module the whole way to
   compilable C both ways, each in a process of its own, and compares every function's final text:
   lzexpand, keyboard, comm, lz32 -- and USER, GDI and KRNL386 -- all the same, token for token.
   (A first check by the judge, 2026-10-28, gave the same verdicts on lzexpand, lz32, keyboard,
   comm and calc; for the 16-bit three it proved nothing: c_text.compilable16's cache was not keyed
   by CB_C_FROM_L2 and handed back the printed C. Keyed now.) Today's path stays the default; not
   carried yet: comments (PrintC's warnings), and where PrintC breaks a long line.
3. **Retire c_text into L2 passes,** one at a time, in L2_BINARY_TYPES.md's order (pointers with
   an address space first: `_far`, `_far_from_stack`, `_near_calls`). Each pass moved is one regex
   gone; the count above is the to-do list.
   **Begun 2026-10-28: `_far` and `_near_calls` moved** (`backends/c.MOVED`). The back end writes
   `__far` on a four-byte pointer's declaration and on the cast assigned to it, and `code __near *`
   on a call through a word; c_text leaves its own out for a function whose C the back end wrote
   (decompiler.FROM_L2 marks it; one it declined -- an Unsupported node -- keeps the printed C and
   every pass). Gate: `--made` on the seven modules above, 5,336 functions, all the same.
   **Pointer passes done 2026-10-28:** `_far_from_stack` moved too (`backends/c._far_from_stack`, a
   dataflow over the statements: a far pointer set from a near one into the frame gets SS). Which
   pointers are far now comes from L2's types alone, not the `/*@P name 4 */` notes: measured on
   every 16-bit function of the golden corpus (4,064) and of USER, GDI, KRNL386, LZEXPAND,
   KEYBOARD and COMM, the two sets are the same (the three USER/GDI functions that once differed
   no longer do). PrintL2 brackets a hidden function's operand as PrintC does (`(a & 0xcff) &
   0xffffff03` across a ZEXT), so `tools/l2c.py` gives 6,808 of 6,808. Two pointer bugs the
   stronger 16-bit judge found were fixed in both paths: a constant data address assigned to a far
   pointer stays near, so C makes it far with DS (LZEXPAND's LZDONE wrote through 0000:0032;
   `c_text.data_address`), and a frame buffer reached through its address as a number is written
   with SS (KEYBOARD's NEWTABLEEX; `c_text._frame_through_ss`, not yet a tree pass). Still not
   carried: comments (PrintC's warnings) and where PrintC breaks a long line -- the `--made` gate's
   "but for comments and line breaks". Next: the c_text passes after the pointer ones, in
   L2_BINARY_TYPES.md's order.
4. **Opaque machine fragments**: the assembly lanes' functions (source/image_writer.py, the NASM
   writers) as L2 `asm` nodes, so one tree holds a whole module whatever each part came back as.
5. **One verdict vocabulary** for binaries and source pairs (item 5 above), read by the module
   writer from L2 provenance (module + address).
