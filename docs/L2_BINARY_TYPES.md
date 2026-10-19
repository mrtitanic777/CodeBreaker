# L2 types for binaries: address spaces, layout, conventions

Proposal, 2026-10-19, from the Windows 95 session, for agreement with the J2CTool session, which owns
`codebreaker/translate/ir.py`. Implements items 1, 2, 3, 6 and 7 of IR_UNIFICATION.md. The schema is in
`l2_types.schema.json` beside this note. Nothing is built yet.

Every type here exists because the Win16 rebuild (USER/GDI/KRNL, 2026-10-16 to 19) needed it and
`c_text.py` had to fake it with a regex. The pass each one replaces is named, so the list doubles
as the retirement plan for those passes.

## 1. Pointers carry an address space

`{"kind": "ptr", "space": ..., "to": <Type>}`

| space | meaning | C back end prints | replaces |
|---|---|---|---|
| `flat` | one linear space (32-bit, Java and the rest) | `T *` | (today's default) |
| `near` | 16-bit offset in a segment named by `seg` | `T __near *` | `_near_calls` |
| `far` | selector:offset, 32 bits | `T __far *` | `_far`, `_far_locals`, `_far_from_stack` |
| `huge` | far, normalised across 64K by `__AHINCR` | `T __huge *` | (open) |
| `real` | real-mode `seg*16+off` | `T __far *` + a real-mode note | IO.SYS / DOS lane |
| `io` | an I/O port number | `inp`/`outp` calls | interrupt helpers |

`seg` on a near pointer names *which* segment it is relative to:

- `ds`: the automatic data segment;
- `ss`: the stack;
- `cs`: its own code segment;
- `{"module_segment": n}`: one of the module's own segments;
- `es`: whatever ES holds.

Picking the wrong segment is the bug class that made far pointers into the frame need SS (adab678).

## 2. A selector is a value with a known origin

`{"kind": "selector", "of": ...}`, 16 bits. `of` records what the build must reproduce:

- `{"ds": true}`
- `{"ss": true}`
- `{"module_segment": n}`: the C back end prints the selector of a function in it, or of the bytes the build carries for that segment (`codebreaker_segment_n`, c6a55e2);
- `{"import": "KERNEL.__0040H", "part": "segment"}`
- `{"unknown": true}`

The decompiler's made-up selectors (0x1000 + 8·(n−1)) never reach L2. That makes these fixes structural instead of textual:
- 1158h for segment 44 (c6a55e2);
- `push 1000h` turning into segment 1 (30f2674);
- `-0x62f1` not recognised as a function's offset (4852150).

A **far pointer value** is the expression `FarAddress(selector, offset)`, typed `ptr far`. MK_FP disappears.

## 3. Code pointers name the function

`{"kind": "fnptr", "space": "near"|"far", "fn": <FunctionType>}`. A constant code pointer is
`AddressOf(Sym)`, never a number, because the function moves in a rebuild (user-c-passes-moved-offsets, 4852150).

## 4. Records have an explicit layout

`{"kind": "record", "name": ..., "size": n, "packed": true, "fields": [{"name", "offset", "type"}]}`.
Offsets are fixed, not computed, and gaps are explicit `pad` fields. This one type serves three uses:

- the core's `/*@S` structures;
- frames: a function's locals at their original offsets (`codebreaker_frame`, 33d67af: two `char[26]` arrays that are really one 52-byte buffer, so every dialog came out the wrong width);
- data segments.

A field whose type is a `record` is nested in place.

## 5. Function types carry their convention

```
{"kind": "fn", "conv": "cdecl"|"pascal"|"stdcall"|"fastcall"|"register"|"interrupt",
 "distance": "near"|"far", "pops": n|null, "loadds": bool,
 "params": [{"type", "at": {"stack": off} | {"reg": "AX"} | {"regs": ["DX","AX"]}}],
 "result": {"type", "at": {"reg": "AX"} | {"regs": ["DX","AX"]} | {"reg": "AL"} | {"flag": "CF"} |
            {"regs": ["ES","BX"]} | "none"},
 "preserves": ["SI","DI","DS"], "variadic": bool}
```

What it covers:
- `register` means arguments in registers. C cannot carry that, so the C back end keeps such a function as a machine fragment; this is `_registers_c_cannot_carry`.
- A result in CF is a flags-result callee.
- `result: none` is a function that sets no AX (CopyRect), as the Win16 gate now distinguishes.
- OpenWatcom's default is register-based, so the C back end must always print the convention (watcom-default-register-convention).

## 6. Integers carry their width and signedness on the operation

The hub already has `i16`/`u16`. The binary-facing additions:

- **Comparisons:** `Compare` carries `signed: bool`, from the instruction (`jl` versus `jb`), not from the operand types. This is 35b2efa: a piece in an ordering comparison is signed.
- **Partial values:** `Piece(value, byte, width)` and `Join(hi, lo)`. These are `_pieces` and `_joined_arguments`.
- **char:** signed or unsigned per target, stated, not assumed (the `-j` flag).

## 7. What stays assembly is a node

`MachineFragment {module, segment, offset, length, reason, asm}` can stand as a function or as a block:

- `reason` says why: hand-written, register arguments, DS left changed, a fault table, and so on;
- the C back end prints it as a separate .asm, as the build does today;
- other back ends list it in the ledger.

## 8. Provenance on every node

Next to `line` and `col`, there is an optional `origin: {"module", "segment", "offset"}` (or `{"va"}` for flat binaries). The gate, the ledger and the workbench point at the original bytes.

## Order of work

1. **J2CTool** (owner): adopt or adjust this schema, and add the types to `ir.py`. For the Java and C front ends, `ptr` with `space: flat` and `record` are useful immediately.
2. **Windows 95 session:** `PrintIR` in decompiler.cc, starting with function types (§5) and pointers (§1–3), which the core already knows from the description.
3. **C back end:** prints these. It is judged first against today's C by tools/golden.py, then by the Win16 gate and the VM.

## Open questions for J2CTool

- Should `selector`/`far` live in the core node set, or in an extension that lowers to `flat` + emulated memory for managed targets? My vote is an extension: Java never needs them.
- Should `record`'s explicit layout replace `class` for C structs, or sit beside it? My vote is beside it: a Java class has no layout.
- Are field names in L2 the decompiler's (`field_0x8`) until a namer pass runs, or optional?
