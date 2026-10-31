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
   The C is unchanged: `tools/golden.py check` gives the same differences as master's build. (Each statement's addresses as `origin` {"at": [...]}: ExprStmt, Return and If since 2026-10-28, every other statement since 2026-10-30 -- step 4. Comments: carried since 2026-10-28, step 3.)
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
   by CB_C_FROM_L2 and handed back the printed C. Keyed now.) Today's path stays the default.
   (Comments and where PrintC breaks a long line: carried since 2026-10-28, step 3.)
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
   with SS (KEYBOARD's NEWTABLEEX; `c_text._frame_through_ss`, not yet a tree pass).
   **Comments and line breaks carried 2026-10-28.** PrintL2 writes PrintC's comments into L2 where
   PrintC writes them (it sorts them with the core's CommentSorter and catches emitLineComment): the
   function's warnings as `MethodDecl.comments`, the others as `Comment` statements at their places
   (a schema addition). The one place L2 cannot hold one is a loop's comma list (`while (a, b)`),
   where the core can sort a comment between two parts: that becomes Unsupported, and the printed C
   stays; there is none in the golden corpus or the seven `--made` modules.
   The back end lays its text out as PrintC does, by the rule rather than by carried break points:
   it makes the same calls to the core's emitter in the same order -- each expression and type
   through a reverse-Polish stack as PrintLanguage::pushOp/pushAtom place it -- and runs them
   through a port of EmitPrettyPrint (`backends/c._Pretty`: lines of 100, broken where the groups
   allow, a comment's broken line filled). What a moved pass adds (`__far`, `MK_FP(...)`) is given
   no room, as c_text adds it to the printed C after the layout. `tools/l2c.py` now holds the back
   end to the printed C character for character, comments and line breaks included: 6,808 of
   6,808. `--made` on the seven modules: 5,336 functions, all the same, none "but for comments and
   line breaks". Next: the c_text passes after the pointer ones, in L2_BINARY_TYPES.md's order.
   **The passes after them, 2026-10-29** (in the order c_text runs them, L2_BINARY_TYPES.md giving
   none past the pointers): `frame_through_ss` (a frame buffer reached through its address as a
   number gets SS), `pieces` (`x._2_2_`; signed beside an ordering comparison -- L2 Binary.op says
   so, not the text around it), `segment_registers` (`unaff_SS = codebreaker_ss()`), `null_is_zero`,
   `global_pointer_steps` (`(uint1 *)pRam.. + n`; done on the name the text ends with before the
   `+`, as the regex saw it, including the `&x + 2` a piece is written with), `one_name_for_globals`
   (`_iRam..` is `iRam..`) and `far_result` (`T __far *` for a far result; a far pointer to a
   pointer declines, keeping the printed C). Each is a rule over the tree (`backends/c._PASSES`,
   run before the function is written), its text placed as c_text places its rewrite: the
   expression as one token given the room of what PrintC printed (`_Stack.whole`), so the lines
   break where they did. `pieces` and `null_is_zero` are done for the 32-bit path too
   (`backends/c.FLAT`; compilable32 leaves them out for C from L2). Gate (`--made`): KRNL386 1,166, GDI 1,657, LZEXPAND 46, KEYBOARD 70, COMM 41 and LZ32 39 functions, all the same. USER has not been gated yet: with a newly built decompiler its first full decompile ran past two hours, so it was stopped. Holding the gate meant copying one of c_text's blind spots onto the tree: `(int4)*(int2 *)((int2)&xStack_4 + i)` is parsed as a product (int4 is no C type name), so c_text never gives that frame read SS. GDI's sub_35_0669, sub_35_0722 and sub_36_0000 read their frame through DS in both paths. **Fixed in both paths, 2026-10-29** (c_text._behind_cast in _frame_through_ss and _far_reads; the back end no longer skips a dereference behind a cast): those three now read their frame through SS, and four more GDI functions of the same blind spot (sub_5_1B0A, sub_5_3DAD, sub_37_0714, sub_37_07A5, a far place read behind `(int8)` / `(uint4)` after a product or a quotient) through a far cast; KRNL386, LZEXPAND, KEYBOARD, COMM and LZ32 unchanged.
   **The printed path retired, 2026-10-29.** The C from L2 is now the only 16-bit and 32-bit x86
   path (decompile16 and compilable32 ask run() for it whatever CB_C_FROM_L2 says), and each pass
   it does is gone from c_text: `_far`, `_far_from_stack`, `_frame_through_ss`,
   `_segment_registers`, `_near_calls`, `_global_pointer_steps`, `_one_name_for_globals`,
   `_far_result` (and `moved_to_l2`). `_pieces` and `_null_is_zero` stay, for the 32-bit C the
   back end declines and for PowerPC. Those that read the module moved too, as the passes
   `backends/c.IN_MODULE`: the back end writes a 16-bit function's C once the module is whole
   (`c_text._write16`, among workers), given what the module says of it (`c_text.module_facts16`:
   its model in the description, how wide its instructions read through each far argument, which
   argument words they load as far pointers, the export prologue, a DGROUP load) -- the model
   word, `_joined_arguments`, `_far_arguments`, `void` -> `xunknown2`, a byte result made a word,
   `__export` and `__loadds`; and the module's tables m.near, m.wide, m.novalue and
   m.exported_prologue come from the document and those facts (`backends/c.flags`), not from the
   heading's text. c_text's `_joined_arguments`, `_far_arguments`, `_byte_result`, `_before_name`
   and `_far_before_name` are deleted. Two rules kept as c_text had them, each a change of its
   own to try: a heading c_text's parser lost (`int2 __fastcall16far f(void)`: tree-sitter reads
   `__fastcall` as Microsoft's word) gets no `__export`, `__loadds` or word result
   (`backends/c._lost`: KRNL386's sub_2_7805, sub_2_79DB and sub_2_7BA8 load DGROUP and stay
   without __loadds); `far` anywhere in the heading as c_text saw it decides where the arguments
   start for `_far_arguments`.
   A 16-bit function the back end declines (an Unsupported node: the core's PrintL2 has no node
   for some statements yet) is now one the decompiler could not make (m.failed, its body a
   stand-in the gate sends back to its instructions): GDI's sub_22_0B96, the one such function of
   the seven modules. The gate holds the C to the printed path's texts as they were last made,
   recorded before the deletions (`tools/l2c.py --made`; `--record` only for an intended
   change): LZEXPAND 46, KEYBOARD 70, COMM 41, LZ32 39, KRNL386 1,166 functions the same, GDI
   1,656 of 1,657 (sub_22_0B96) -- and the module's tables (near, wide, novalue,
   exported_prologue, interrupts, selector imports, prototypes, the header) the same as the
   printed path's.
   **Left in c_text, and why.** Per 16-bit function (`_one16`), in the order they run:
   `_selector_structs` and `_struct_gaps` (a selector's structure is the module's when the
   selector is a global: its fields gathered over every function, then renamed in their texts);
   `_own_segment_calls`; `_open_arguments` (after `_caller_frame_slots`, which rewrites a helper's
   heading as text: the two move together); `_interrupts`, `_far_addresses`, `_far_casts`,
   `_entry_sp`, `_stack_arguments`, `_far_locals`, `_far_concat_casts`, `_buffer_fields`,
   `_far_reads`, `_near_to_far` (each reads what the ones before it wrote, so they move in that
   order); `_stack_segment_stores` (the code bytes: a fact for `module_facts16` next) and
   `_callee_pops` (last: it rewrites casts the others write). Module-wide, in `_compilable16`:
   `_caller_frame_slots` (a helper and its callers rewritten together), an import's value used
   bare, the module's own segments as values and far pointers passed whole, `_code_segment_data`,
   the globals' macros (from each function's facts: decompiler.split_notes), `prototypes` and
   `declaration` (which still read each heading from its text) and the header. The 32-bit path
   (`compilable32`: the frame, comparisons as numbers, code pointers, structure gaps, thiscall
   and fastcall, the globals) and PowerPC's (`compilable_powerpc`) have not begun. decompile16's
   rounds still find the calls to places not yet known (`func_0x...`) and count each call's
   arguments in the printed C; the L2 documents' Call nodes say the same, and that is the next
   to move off text.
   **More of c_text's 16-bit passes on the tree, 2026-10-30** (backends/c, `MOVED` and
   `IN_MODULE`, run as `_write16` writes each function with the module's facts):
   `_selector_structs` and `_struct_gaps` (each structure's declaration, and a global selector's
   fields, go to the facts -- "selector_decls", "selector_shared" -- which c_text gathers in
   address order and declares once with `_shared_selector_structs`, as before);
   `_own_segment_calls` (the function's own made-up selector is a fact, "selector");
   `_interrupts` (the interrupts taken are a fact, "interrupts"; an interrupt result's local goes:
   the back end can now leave a declaration out, `_now['drop']`); the definition in the older
   style (`_old_style`, c_text's `_open_arguments`: `_caller_frame_slots` now writes the helper's
   heading in that style itself; `_open_arguments` stays for PowerPC); `_entry_sp` and
   `_stack_arguments`. The last four moved ahead of `_far_addresses` and `_far_casts`, which do not
   read or write what they do -- but for one thing each: the selector structures are named by the
   selector as written before the interrupts' results are renamed (KEYBOARD's
   `seg_sub_2_031F_int_DS_00`), so those two moved together; and `_far_addresses` takes away a
   local that only holds a far pointer's high word, which `_entry_sp` then never saw as a local of
   the frame (`backends/c._unused_high_halves`: five USER and GDI functions). The tree passes read
   the text as c_text's parser read it: tree-sitter does not know the decompiler's type names, and
   takes `(int2)&x` for a bitwise and unless what follows binds tighter (`(int2)&p->f + 2` is a
   cast), `(int2)*p` for a product (`backends/c._unary_as_read`). `_struct_gaps` stays in c_text
   for the 32-bit and PowerPC paths. decompile16's rounds read the calls, their arguments and
   each function's own arguments and convention off the L2 documents (`decompiler._l2_calls`,
   `_own_arguments`; a function without one from its text). Gate: `l2c --made` now also holds
   each module's header and tables (near, wide, novalue, exported_prologue, interrupts,
   prototyped, selector imports, failed) to the reference; the references were recorded again,
   with them, from master as it was (e93d9f5d7): LZEXPAND 55, KEYBOARD 79, COMM 50, LZ32 39,
   KRNL386 1,175, GDI 1,666 and USER 2,326 entries (functions and nine for the module), all the
   same; where they differ only in line breaks (KRNL386 73, GDI 194, USER 75, LZEXPAND 1,
   KEYBOARD 1) a heading in the older style fits on one line where PrintC broke the longer one,
   or a rewrite is laid out by its own text. c_text.py is 3,026 lines (3,284 before).
   **The far-pointer chain begun, 2026-10-30:** `_far_addresses` (with `_whole_far_arguments`:
   MK_FP as a far pointer to what the offset names, a far pointer's halves as its own
   arithmetic or passed whole, an unused high-half local and its statement gone -- the back end
   can now leave a statement out, `_now['gone']`) and `_far_casts` (a cast before a far name
   made far, CONCAT22 of its halves `(int4)p`, its low word moved on as its arithmetic) are the
   back end's. The MK_FP texts the earlier passes write (a far pointer from the frame, a data
   address, a frame buffer through its address, `_caller_frame_slots`' arguments) take the
   far-address form where it is moved. Their rewrites are given no room in the layout (as long as
   c_text made them: given the room of what PrintC printed, a `;` or `)(...)` was left on a line
   of its own), so more functions differ from the reference in line breaks alone (LZEXPAND 9,
   KEYBOARD 6, COMM 12, KRNL386 212, GDI 509, USER 427); every token, and each module's header
   and tables, is the same. c_text.py is 2,863 lines.
   **Every 16-bit per-function pass on the tree, 2026-10-30** (backends/c, `MOVED`, `IN_MODULE`,
   `_PASSES`; one commit each, c_text's function, frozen fixture and freeze_tests entry deleted,
   its cases ported to tests/translate/test_c_backend.py): `_far_locals` (a four-byte local read
   through declared `T __far *`, its casts far; `_p` beside a near p the far pointer, p its offset
   where nothing sets p; a pointer set from one typed otherwise cast to its own type),
   `_far_concat_casts`, `_buffer_fields` with `_frame_block` (runs of words made one array,
   buffers laid out to the next one passed on or the saved BP, never-set locals inside one read
   as its fields, two buffers or more one packed `codebreaker_frame`), `_far_reads`,
   `_near_to_far`, `_stack_segment_stores` with `_far_record` (where the function stores SS into
   its frame is a module fact now, "ss_stores": c_text._ss_store_places) and `_callee_pops` (a
   fact too, "callee_pops", worked out before the C is written; the last pass). c_text's `_one16`
   only takes the core's notes off each text.
   What the back end gained for them: a local declared otherwise (`_now['decls']`); the locals'
   declarations rewritten as a text (`_now['locals_text']`, written in their place -- c_text's
   line edits, blank lines included, on that text alone); a local's uses renamed
   (`_now['ren']`, read lazily, so a buffer's later frame name reaches every text written from
   it, the entry-SP pass's included: `_local_text`); a statement put before or after another
   (`_now['before']`, `_now['after']`; "before" is the statement C's grammar has the call in:
   `_statement_keys`, an else-if's first if, a case's last label); and the text as it is written
   now (`_seen`: one layout on one line, recording each node shown, each name's place and what is
   before it -- through a moved pass's rewrite to the text around it -- so "passed on" and "its
   address taken" read as c_text's parser read them, `(char *)&x` inside a piece of x included).
   Quirks kept as c_text had them: `x->f` is no read through `*x` (L2's Select of *x);
   `(int4)*p` is an indirection behind casts with no `*`, after a product or a quotient only
   behind one cast, never after a prefix operator (`_star_as_read`); `T **` made far is written
   `T * __far*`; a structure gap's far byte pointer gets `_near_to_far`'s `(uint1 *)` where its
   brackets hold a number (`_gap_cast`). One change of order: c_text ran these passes after
   `_caller_frame_slots` (module-wide, on the text); they now run before it, as the function is
   written -- no gated module shows a difference, and moving `_caller_frame_slots` onto the
   tree next removes the question.
   Gate: the references were recorded again from master 2d2a195fc (the old ones were lost with
   their worktree; a copy in state/work/item3c/refs/), and after every pass `l2c --made` gives
   LZEXPAND 55, KEYBOARD 79, COMM 50, LZ32 39, KRNL386 1,175, GDI 1,666 and USER 2,326 entries
   (functions, the header and the tables) the same as the reference character for character --
   none different, none in line breaks only. c_text.py is 2,178 lines (2,863 before).
   **The module-wide passes on the tree, 2026-10-30** (one commit each). What the module decides
   from every function is now decided from what writing each one found, and the functions it
   names are written again with the decision: `_write16` writes every function once (each pass
   reporting into the facts what the module needs: `frame`, `selector_imports`, `code_reads`,
   `names`, `heading`), `_compilable16` makes the module's plans from those reports and writes the
   functions they name a second time with them (merged, one second writing each), and builds the
   header from the reports -- no function's text is read back. In order: `_caller_frame_slots`
   (backends/c._caller_frame_slots + `caller_frame_plan`: the helper's heading in the older style
   with far-pointer parameters, each caller's plain calls given its locals' addresses, a local it
   never declared declared first, `_now['declare_first']`; the per-function passes and this one
   now run in the order they are written in); an import's value used bare (`_selector_imports`,
   given the module's `selector_values`); the module's own segments as values and far pointers
   passed whole (`_segment_values` + `segment_plan`, after the first writing says which functions
   are far; GDI's sub_42_231C taught it to read a call's arguments as written whatever rewrite
   holds them); `_code_segment_data` (`_code_segment_reads` finds every number a read through CS
   may come from, by its node -- the selector structures' `SEG_PTR(name, CS)` among the far
   pointers passed as (offset, CS), as c_text's parser read that text, and a local by the name it
   is written as: KRNL386's int_DI -- and c_text keeps only the layout over the segment's data,
   the second writing given `code_places`); the globals' macros, the imports, the segments carried,
   the intrinsics (`_names_used`: the tree's names and calls as printed, and those in the text the
   passes wrote); `prototypes` / `declaration` (`_heading_facts` + backends/c.`prototypes`, with
   `arguments_at`, `argument_words`, `words`, `parameters`; one quirk kept: a heading in the newer
   style is none c_text's pattern read -- the blank line before its `{` -- so it gets no
   prototype); a global selector's structure that is each function's own (`selector_renames`, 52
   of KRNL386's functions); and `_one16` (the core's notes left out by the back end:
   `without_notes`). c_text's helpers nothing called any more went with them.
   **c_text's Module classes folded,** into `translate/l2.Made`: the module made compilable as one
   L2 Module document (`m.doc`; each function a MethodDecl with its C as `text` and its place as
   `origin` {module, va}, the header the Module's), the lane's tables beside it as attributes;
   `texts`, `order`, `by_va`, `header`, `functions` are views of the document, and setting texts
   or header makes a new document (a `copy.copy` of the module keeps its own).
   **The 32-bit path begun:** compilable32 keeps the L2 documents (`decompiler.raw(write=False)`)
   and writes each function with the module's facts, so the back end does the passes that are for
   the module's C alone (backends/c.`FLAT`, `FLAT_ONLY`): a `void` result `int`; `__thiscall` /
   `__fastcall` left out of the heading and reported (`registers`: the module's in_ecx / in_edx);
   the structure gaps; the code pointers (`functions`); a pointer compared with a number as one.
   compilable32 keeps its text rules for C the back end declined.
   Gate: after every pass `l2c --made` gives LZEXPAND 55, KEYBOARD 79, COMM 50, LZ32 39, KRNL386
   1,175, GDI 1,666 and USER 2,326 entries the same as the references (master 2d2a195fc),
   character for character, and the 16-bit modules' code data, code reads, imports and segments
   carried the same as master's; the 32-bit and PowerPC passes are held by state/work/item3d/gate.py
   to references recorded from master 1821606f9 before any change (a copy in
   state/work/item3d/refs/): every text, the header and the tables of LZ32 44, VERSION 49, IMM32 78,
   TAPI32 191, WINSPOOL 87, MPR 231 entries; with structures recovered (struct_recovery 32) MPR,
   VERSION, TAPI32, WINSPOOL and COMDLG32 239; PowerPC: the Xbox 360 HUD module 670 -- all the same.
   No output changed. c_text.py is 1,860 lines (2,412 before).
   **Left in c_text.** The 16-bit path's module assembly is no text pass any more, but it is still
   in c_text: `_compilable16` (the orchestration and the header), the module's facts from the
   bytes (`module_facts16`, `_far_argument_widths`, `_far_loads`, `_loads_dgroup`,
   `_ss_store_places`, `_selector_sites`, `_keeps_frame`, `_code_bytes`, `_data_runs`,
   `_code_segment_data`'s layout), `HEADER`, `interrupt_helpers`, `interrupt_routines`,
   `INTRINSICS16`, `_global_type`; and `declaration(text)`, which build/module still reads each
   function's heading with. The 32-bit path's text passes on C from L2: `_keep_frame` (the frame
   structure `_f` where a local's address escapes), the globals read through as pointers
   (`through`), a global used at another width (its alias), pointer globals compared with a cast
   number (`same_cast`), `_fit_calls`, the odd-size typedefs, struct_text.apply (`p->field`); and,
   for C the back end declines, `_pieces`, `_null_is_zero`, `_compare_as_numbers`,
   `_code_pointers`, `_struct_gaps`. The PowerPC path is not begun: it decompiles without L2
   (`decompiler.raw` with no `l2`), so every pass there (`_WIDE_ADDRESS`, `_struct_gaps`,
   `_pieces` with PIECE, `_compare_as_numbers`, `_code_pointers`, `_homed_arguments`, void ->
   uint8, `_open_arguments`, the globals, `call_counts`) is still on text; it needs PrintL2's
   PowerPC documents held to the printed C first (tools/l2c.py has no PowerPC corpus).
   **The 32-bit and PowerPC paths on the tree, and c_text without text passes, 2026-10-30** (one
   commit each). 32-bit: `_keep_frame` (backends/c._keep_frame: the frame structure `_f` where a
   frame local's address escapes, the other declarations' lines left empty as c_text left them,
   the names written after the layout; the structure sizes are the facts' `struct_sizes`); C the
   back end declines is now a function the decompiler could not make (no C, as a failed decompile:
   measured on LZ32, VERSION, IMM32, TAPI32, WINSPOOL, MPR, COMDLG32, KERNEL32, USER32, GDI32,
   ADVAPI32, COMCTL32 and SHELL32 -- 7,291 functions -- the back end declines 2, COMCTL32's
   sub_02C195 and SHELL32's sub_0252EC, each for an expression PrintL2 has no node for), so the
   fallback's text rules went (`_null_is_zero`, `_keep_frame`, and the 32-bit uses of `_pieces`,
   `_compare_as_numbers`, `_code_pointers`, `_struct_gaps`); the global rules -- what compilable32
   decides over the module is decided from every function's tree before any is written
   (backends/c.`module_names32`: the names each writes, its calls, the globals it calls through)
   and each function written with its part (`_globals_through`, `_global_now` for the width
   aliases, `_same_cast`), the imports used from the trees' calls, each function declared with its
   heading as the back end writes it (the facts' `declared`), the comments before a function left
   out by the back end (`without_notes`); `_fit_calls` (`takes32` from each heading,
   `_fit_calls`); the odd-size typedefs (`type_names`: the types each tree writes); and
   struct_text: `struct_text.plan` decides the fields from the trees and the headings and the
   functions it names are written again with theirs (`_struct_fields`), `struct_text.apply` and
   its patterns gone. One output change, a fix: the pattern over the text took `*(int4 *)a1->f_1c`
   for `*(int4 *)a1` and wrote `((struct_7 *)a1)->f_0->f_1c` (another place, and no C: f_0 is an
   int4); COMDLG32 (structures recovered) sub_00BF00, and 9 COMCTL32 and 9 SHELL32 functions
   (and the headers) change so, four COMCTL32 structures whose only second field was that misreading no longer
   declared. PowerPC: the native decompiler writes an L2 document for every processor but 16-bit
   x86 (Description.flat), and a constant PrintC made up with no variable of its own no longer
   crashes PrintL2 (PrintL2::readType: two HUD functions died there with the notes on); on the Xbox
   360 HUD module the back end's C is the printed C for all 665 functions, character for character
   (state/work/item3e/ppc_l2.py). compilable_powerpc writes each function from L2 with the passes
   `backends/c.POWERPC` (the facts' `lane`): `_wide_address`, `_struct_gaps`, `_pieces` (spelled
   `PIECE(x, off, T)`), `_compare_as_numbers`, `_code_pointers`, `_homed_arguments` (the slot the
   caller keeps for a register argument is the argument), `uint8_result` (void is uint8, the facts'
   `novalue`), `open_arguments` (the older style); and the module's globals, the calls through
   them, the `func_0x` addresses, the imports, the declarations and how many arguments each callee
   is called with (`Made.calls`, which the PowerPC lane reads too) come from the trees. c_text's
   `_pieces`, `_compare_as_numbers`, `_code_pointers`, `_struct_gaps`, `_open_arguments`,
   `_homed_arguments`, the text `call_counts` and the text readers (`_called`, `_called_through`,
   `_globals`, `_identifiers`, `_without_notes`, `_declaration_lines`) are deleted, with their
   frozen fixtures; c_text no longer imports ctree. 16-bit: build/module declares each export
   with the declaration the back end wrote from the function's L2 heading (`Made.declarations`),
   not `declaration(text)`, which is deleted.
   Gate (state/work/item3e, references from fcf307c52 re-verified before the first change): after
   each pass the seven 16-bit modules' 5,390 entries and tables the same; the 32-bit references
   (LZ32 44, VERSION 49, IMM32 78, TAPI32 191, WINSPOOL 87, MPR 231; with structures recovered MPR,
   VERSION, TAPI32, WINSPOOL, COMDLG32 239) and more recorded from fcf307c52 for rules the first set
   barely reaches (KERNEL32 1,871, USER32 550, GDI32 832, ADVAPI32 177, COMCTL32 1,105, SHELL32
   1,902) the same but for the two changes above (recorded again for them); PowerPC: the HUD
   module's 670 entries the same.
   **Left in c_text** (no text pass): the three paths' module assembly -- `compilable32` (the
   module's decisions and header), `_compilable16` / `_write16` and the facts from the code bytes
   (`module_facts16`, `_far_argument_widths`, `_far_loads`, `_loads_dgroup`, `_ss_store_places`,
   `_selector_sites`, `_code_bytes`, `_code_segment_data`'s layout), `compilable_powerpc`, the
   header texts and `_struct_decls`, `interrupt_helpers` / `interrupt_routines`, `call_counts` (on
   the trees' calls). (The 32-bit lane's build/module read a prototype from each text,
   `functions.prototype`; since 2026-10-30 it declares each export from L2, `Made.declarations`:
   step 4.) c_text.py is 1,420 lines (1,860 before).
   **The 16-bit frame and shape passes, on the tree, 2026-10-30** (the Windows 95 session's
   `wip/frame16`, ported; off unless `CB_FRAME16=1`). `decompile/frame16.py` lays a 16-bit
   function's locals out as the original's frame, so that C 8 places them where the original had
   them: the places every `[bp-k]` reaches (one whose address is taken a buffer, up to the next
   place), each local's place from its L2 Local `at` or the instruction that assigns it, the
   pieces of a buffer made one byte array read at their offsets, a bool kept in a word an int2,
   the locals declared nearest the frame pointer first. `decompile/shape16.py` writes `x++` /
   `x--` where the instruction is an inc / dec and the statement is x's own step of one (or a
   place in the frame set to a number: the WIP rewrote any statement of an inc / dec, which
   changed what `iVar6 = iVar6 + 3` or `bVar8 = pxVar5 == 0` do), and turns an if-return round
   where the original has the other return first. Both are passes on the function's L2 tree before the C back end
   writes it, beside asm_blocks (`decompiler.c_from_l2`, given the code when `tree_images()`;
   once per function, `facts["placed"]`), so the module's L2 tree is made of the trees so laid
   out. The decompiler gives each Local its place (`at`, a Location; a schema addition) only
   with CB_FRAME16=1 (decompiler.cc `g_frame16`), and the switch is in the decompiler's and
   compilable16's cache keys; the back end writes a postfix `++` / `--` (PrintC's
   post_increment, which the decompiler never needs). Gate with the switch off: every function,
   header and table of the seven `--made` modules the same (5,390), and the 32-bit and PowerPC
   paths too. With it on, the functions it changes are the WIP's but for the wrong increments
   (LZEXPAND 7 of 9, KEYBOARD 9 of 12, COMM 4, SUWIN 365 of 376), no function is declined, and
   OpenWatcom gives the same errors as without it. Test: tests/translate/test_frame16.py.
4. **Opaque machine fragments**: the assembly lanes' functions (source/image_writer.py, the NASM
   writers) as L2 `asm` nodes, so one tree holds a whole module whatever each part came back as.
   **Begun 2026-10-29, for the 16-bit module build.** The node is L2's `MachineFragment`
   (L2_BINARY_TYPES.md section 7), with three fields added to the schema and ir.py: `name` (the
   function it stands for), `syntax` (the assembler: wasm here) and `within` (a run of functions
   assembled as one file: the text is on the first, the others name it). `build/module.py`'s
   `_build16` writes the module as one Module (`module_document`, `b.l2`, `module.l2.json` in the
   build folder): its functions in address order, each one kept as its instructions a
   MachineFragment (origin, length, why, the .asm the build assembled), each one rebuilt from C
   its L2 document (decompiler.run keeps them with CB_C_FROM_L2: `c_text.Module16.l2`), and one with
   no L2 its name and place. `backends/c.module` writes the whole tree back: the C of each
   function, a note in each fragment's place, and the assembly beside it, which the lane assembles
   on its own as today. Not yet: a fragment inside a function (a block) outside the _asm-block
   pass. Test: tests/translate/test_c_backend.py.
   **The assembly writers, 2026-10-30.** source/image_writer.py (the six console and home-computer
   writers), the NASM writers (ne_writer, pe_writer, le_writer) and the MASM writers (driver_writer,
   dos_writer) build their file as one Module of MachineFragments as they write it (`l2.Assembly`:
   a section, segment, object, bank or function each), and the text is the assembly back end's
   (`translate/backends/asm.py`); identical output on every module gated (docs/ARCHITECTURE.md,
   "Built: one Module model, the writers as back ends"). Test: tests/translate/test_module_model.py.
   **L2 everywhere, 2026-10-30** (one commit each). *The 32-bit and VxD module builds*: `_build32`
   writes its module as one Module too (`module_document32`, `b.l2`, module.l2.json): each function
   in address order its L2 document (compilable32 keeps them: `Made.l2`) placed by its address, one
   with no L2 its name and place; driver_writer (the VxD lane's build) gives every function a
   MachineFragment of its own (`Writer.function_starts`: the entry points and every place a call
   lands) and writes its tree as module.l2.json beside the source. *Prototypes from L2*: `_build32`
   declares each export with the back end's heading (`Made.declarations`: compilable32's, as last
   written -- after the structure fields' second writing), not `functions.prototype` of its text;
   checked first on every function of LZ32, VERSION, IMM32, TAPI32, WINSPOOL, MPR, the structures-on
   five, KERNEL32, USER32, GDI32, ADVAPI32, COMCTL32 and SHELL32 (7,829 functions): they agree on
   every function with C (the two the back end declines have neither). *Statement provenance*: PrintL2 gives every statement its instructions as
   `origin` -- While, DoWhile, For and Switch their test's (as If), each part of a for's init and
   update its own, a Goto, Break or Continue the branch it is (none for a break the structure adds),
   a label and a case where their block begins, a comment the instruction it is attached to.
   On the golden corpus (6,808 functions, all valid L2) every ExprStmt, Return, If, While, For, Switch, Case, Labeled and Comment has an origin; DoWhile 2,091 of 2,314, Goto 3,625 of 5,040 and Break 1,525 of 1,948 (the rest made by no instruction: an endless loop, a fall-through goto, a case's break); a Block has none. The C is unchanged. *A fragment per function in the NASM writers*: ne_writer (entries, CS:IP, near calls, far
   calls through a relocation into the module), pe_writer (the entry, pe_functions' starts, near
   calls) and le_writer (the entry points, services and calls) cut their trees at each function as
   driver_writer does; nothing is decoded differently. Gate: the C of every gate set the same as the
   references (the seven `--made` modules, the 32-bit six, the structures-on five, refs_extra's six,
   the PowerPC HUD), the module builds of lz32.dll and version.dll and the driver builds of vkd.vxd and
   splitter.vxd the same but for timestamps (module.l2.json added), the NASM writers' 21 files of
   seven modules byte for byte.
5. **One verdict vocabulary** for binaries and source pairs (item 5 above), read by the module
   writer from L2 provenance (module + address).
   **Done 2026-10-30** (codebreaker/verify/verdicts.py; docs/ARCHITECTURE.md, "Built: one verdict
   vocabulary"). Three sets of words, one each: what a judge says of a pair (equivalent,
   consistent, divergent, inconclusive, no-compile, no-source -- the emulator lanes, the 16-bit
   gate, the port's cases, Java, the whole-image lanes), what it amounts to (verified, weak,
   identified, unverified, not-judged, no-compile, no-source, with one TITLE and one LABEL each),
   and what a byte comparison says (identical, same size, different, no original extent,
   no-compile). Older spellings map on read (no-synth, same, differs, not run, does not compile,
   run-time library ...). The module writer builds its tree with each function's origin
   {module, va} and reads its verdict by that (`V.Index`). Left: exact/'s byte ledger keeps its own
   words (its "equivalent" is "equal up to another compiler state's habits", not the judge's).
