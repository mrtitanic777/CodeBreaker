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
   The C is unchanged: `tools/golden.py check` gives the same differences as master's build. (Not yet carried: the address of each statement as `origin`. Comments: carried since 2026-10-28, step 3.)
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
   **Left in c_text, 2026-10-30.** Per 16-bit function (`_one16`), in order: `_far_locals`,
   `_far_concat_casts`, `_buffer_fields`, `_far_reads`, `_near_to_far` (the rest of the
   far-pointer chain: each reads what the ones before wrote; `_far_locals` changes locals'
   declared types, so it is the next and the largest),
   `_stack_segment_stores` (the code bytes: a fact for `module_facts16` next), `_callee_pops`
   (last: it rewrites casts the chain writes). Module-wide in `_compilable16`, as listed above
   (`_caller_frame_slots`, the selector values, own segments, far pointers passed whole,
   `_code_segment_data`, the globals' macros, `prototypes` / `declaration`, the header), and
   c_text's `Module16` / `Module` classes (not yet folded into the L2 Module); the 32-bit and
   PowerPC paths not begun.
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
   on its own as today. Not yet: the 32-bit and VxD lanes' module builds; a fragment inside a
   function (a block). Test: tests/translate/test_c_backend.py.
   **The assembly writers, 2026-10-30.** source/image_writer.py (the six console and home-computer
   writers), the NASM writers (ne_writer, pe_writer, le_writer) and the MASM writers (driver_writer,
   dos_writer) build their file as one Module of MachineFragments as they write it (`l2.Assembly`:
   a section, segment, object, bank or function each), and the text is the assembly back end's
   (`translate/backends/asm.py`); identical output on every module gated (docs/ARCHITECTURE.md,
   "Built: one Module model, the writers as back ends"). Test: tests/translate/test_module_model.py.
5. **One verdict vocabulary** for binaries and source pairs (item 5 above), read by the module
   writer from L2 provenance (module + address).
