# recompile95 — make our derived C compile, then link into an OS

`il.py` (the CodeBreaker IR engine) lifts the byte-exact reconstructed Win95 `.asm`
into readable "register-machine" C at `win95/src/generated/` (the external win95 corpus)
(~225,000 functions). That C is faithful to the assembly — register-globals (`eax`/`ecx`),
`al`/`ah` lanes, `__idiv`/`CF` intrinsics, computed `goto*`, offset-named `sub_<hex>` calls —
and so does **not** compile or link as-is.

This project takes that C the rest of the way, **without touching `il.py`**, one rung at a time.

## The three rungs

1. **It compiles** — GCC (gnu89) + a shared prelude. Byte-exactness deferred. **DONE (100%).**
2. **It links & runs** — every module's symbols resolve across the whole OS. **DONE for linking**
   (PE + NE + LE/VxD); "runs" is the remaining frontier.
3. **It's byte-exact** — recompiles to the *identical* bytes. This stays a **separate track**
   (`matchsearch` + the MSVC-2.0 oracle); GCC will never reproduce MSVC's exact codegen, and
   the readable lift is deliberately not the byte-exact source.

Rung 1 is "as close to the ASM as we reconstructed, with the smallest tolerance that still
compiles" — the goal set for this project. Compiler is **GCC in gnu89 mode**: it accepts the
lift's computed `goto*` and old-style implicit calls. (MSVC 2.0 cannot compile the readable
lift at all — that's only ever the byte-exact raiser's job.)

## Where it stands

| Rung | Result | How |
|------|--------|-----|
| **1 — compiles** | **1115 / 1115 modules, 0 errors** (PE 773, LE/VxD 215, NE 127) | prelude + ~12 adapter rules |
| **2a — object files** | **1115 / 1115 → machine code** (~213,500 fns) | `adapt(link=True)` |
| **2b — links** | **whole OS: PE + NE + LE**, ~305,000 defined symbols across ~1,173 modules | `link95.py` |

The rung-1 trajectory across the classify→fix-a-class→re-classify loop was
7.6 → 14.8 → 35.7 → 53.9 → 97.8 → 99.5 → **100.0%** — reached with two layers only, il.py untouched.

## The two layers (rung 1 / 2a)

- **[`include/win95c.h`](include/win95c.h)** — *the prelude.* Declares the shared vocabulary
  once, so a "syntax error" that recurs 200,000 times is fixed in one place:
  typedefs `u8..u128` + `u80` (x87); register / segment / MMX / SSE (`xmm`) / AVX (`ymm`) globals;
  byte/word **lane** macros (`al`/`ah`/`ax` are memory views of `eax`, so writing a lane leaves
  the rest of the parent unchanged, exactly as on hardware); every x86 intrinsic as a **variadic**
  function-like macro → `__rc_*` (arity varies per instruction); flags; `fnptr`; and an `__asm(...)`
  neutralizer (a macro beats GCC's `__asm` keyword because the preprocessor runs first).
- **[`tools/rc.py`](tools/rc.py)** — *the adapter + classifier.* `adapt()` applies mechanical
  whole-class source rewrites the prelude can't express; `--classify` compiles every module
  (GCC, gnu89, multithreaded) and buckets the residual errors, so the remaining blockers are
  always an explicit worklist. `adapt(link=True)` is the rung-2 mode: it names each definition
  `sub_<offset>` to match the `sub_<hex>(...)` call sites so intra-module calls resolve, and
  routes every call through an unprototyped fn-ptr cast so the lift's call/def arity disagreements
  don't block codegen (that cast marks where real signature recovery is owed — rung 2c).

The ~12 adapter rules each killed a whole error *class* (self-OR idiom, computed-goto cast,
flag-temp decls, unique/`sub_`-matched function names, **struct-arg elimination**
`x->fHEX` → `*(u32*)((char*)x+0xHEX)`, `void *argN` → `u32`, `&literal` fix, unresolved-goto drop,
`__jmpd` assign-to-literal drop). The lesson: "225k don't compile" was really "225k share ~12
error classes" — the classify histogram *is* the worklist.

## The linker (rung 2b)

- **[`tools/link95.py`](tools/link95.py)** — takes the rung-2a compiling C for a set of modules
  and makes them **link together** across module boundaries, then partial-links (`ld -r`) them and
  reports how many cross-module symbols resolve. Three moves, dispatched per executable format:
  1. **Namespace** every module's `sub_<off>` → `<PREFIX>_sub_<off>` (globally unique).
  2. **Export-alias** — for each export (name/ordinal → offset) emit an `__attribute__((alias))`
     symbol on the def at that offset. Uncarved exports get a marked `--stub` placeholder.
  3. **Import-rewrite** — each import call site is rewritten to reference the exporter's symbol.

  Format coverage:
  - **PE** (32-bit) — exports/imports straight from the PE tables (section-aware RVA→offset).
  - **NE** (16-bit) — exports from the NE entry table + name tables; far-call **imports recovered
    from the disassembly comment** the byte-exact asm already carries
    (`… -> USER.POSTAPPMESSAGE [USER.116]`), which `il.py`'s `_call_target()` now parses into a
    named `MODULE_NAME` symbol instead of an opaque indirect call.
  - **LE / VxD** (32-bit flat) — objects (not segments) via `le.py`; VxD service imports resolve
    through the **DDB service table** (see below).

Run it from the win95 data dir (so the format parsers find the binaries):

```bash
python tools/link95.py kernel32.dll user32.dll gdi32.dll   # a few modules
python tools/link95.py --all --stub                        # the whole OS
```

## The VxD service-table work (what unblocked LE)

VxD service entry points are reached **only** via `VxDCall (Device_ID<<16)|index` — invisible to
the prologue-scan and call-sweep signals the carve uses, so the carve otherwise swallowed them
mid-function and no def existed for a `NDIS_*` import to resolve to. Two pieces fixed this
end-to-end, in the CodeBreaker engine (not in this folder):

- **A real binary DDB parser** in [`engine/targets/win95/le.py`](../targets/win95/le.py):
  it walks the LE fixup tables, finds the Device Descriptor Block, and resolves its service table
  (an array of fixups) to `(index, obj, offset)` triples — `ddb_services()`.
- **Carve integration** in [`engine/targets/win95/le_seg.py`](../targets/win95/le_seg.py):
  `_carve()` adds those service offsets as function starts. The existing `off2idx` filter drops
  any offset that isn't a real instruction boundary, so a split there is **byte-exact-neutral by
  construction** — it only ever refines boundaries, never changes bytes. (ndis.vxd: 595 → 685
  carved functions; re-verified byte-exact.)

## How it fits together

```
byte-exact .asm  ──il.py (CodeBreaker)──▶  readable C   win95/src/generated/*.c
                                                   │
                              rc.adapt() + win95c.h │  (rung 1: compiles;  rung 2a: → .o)
                                                   ▼
                                    link95.py  (rung 2b: namespace + export-alias + import-rewrite)
                                                   │  PE tables · NE disasm comments · LE DDB
                                                   ▼
                                    ld -r  ──▶  one linked body (~305k defined symbols)
```

Depends on the CodeBreaker engine (`il.py`, `decompile.py`) and the win95 format parsers
(`pe.py`, `ne.py`, `le.py`, `le_seg.py`); consumes the generated C under
`win95/src/generated/` (the external win95 corpus).

## What's next

- **Carve coverage, not linking, is the wall now.** The remaining unresolved symbols are all
  carve gaps: mid-function call targets and uncarved INT 3F MOVEABLE thunks (`ADDATOM`/`FINDATOM`
  are 3-byte `cd 3f` stubs the carve treats as data). Both need `il.py`/carve splitting, not link
  work. The DDB→carve pattern above is the template for closing them.
- **Rung 2c** — recover real function arities and drop the fn-ptr cast shims.
- **Rung 2d** — target 32-bit PE output instead of the host x86-64.
- **"Runs"** — the second half of rung 2, once linking is airtight.

Byte-exactness (rung 3) stays the separate `matchsearch` + MSVC-2.0 track — see the byte-exact
notes in CodeBreaker, not here.
