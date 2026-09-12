# Static recompilation — how it works, and our reimplementation

This documents **exactly** how the RexGlue / XenonRecomp static recompiler turns
Xbox 360 PowerPC into runnable PC C++, and how XboxTranspiler reimplements the
method in its own code (`ppcrecomp.py`). Grounded in the rexglue-sdk source
(`…\Xbox 360\Code\rexglue-sdk`), its generated output (the TetrisPC project), and
the real-world failure modes recorded in the MinecraftPC project.

## One-line summary

It is an **AOT transpiler**, not an emulator and not an IR/LLVM pass: it
disassembles PowerPC, builds a function/basic-block graph, then emits **one C++
statement per guest instruction** operating on a `PPCContext` register-machine
struct plus a flat `uint8_t* base` guest-memory pointer. Each guest function
becomes one C++ function `void sub_XXXX(PPCContext& ctx, uint8_t* base)`. Output
is plain text (no IR). This is XenonRecomp's method.

> **Truth class.** The output *runs* and reproduces behaviour; it is **not**
> readable/idiomatic source and **not** byte-exact to the original. It is the
> "runnable" truth (Pipeline B), distinct from the byte-exact permuter (A) and
> the readable-hypothesis layer (C). Kept separate on purpose.

## The runtime model (what the generated code targets)

- **CPU state** — `PPCContext`: 32 GPRs, 32 FPRs, 128 VMX128 vector regs, LR,
  CTR, XER, and CR as **eight small structs** (`cr0..cr7`, each `lt/gt/eq/so`
  bytes). Every scalar register is a **type-punning union** (`.u32/.s64/.f64/…`);
  the emitter picks the field per instruction — there is no separate 32-bit file.
- **Memory** — one flat 4 GB host allocation; a guest 32-bit address is just
  `base + (uint32_t)addr`. Big-endian is handled by wrapping **every** ≥2-byte
  access in `__builtin_bswap*`, `volatile`-qualified so the compiler can't elide
  guest traffic:
  ```cpp
  #define PPC_LOAD_U32(x)   __builtin_bswap32(*(volatile uint32_t*)(base + (uint32_t)(x)))
  #define PPC_STORE_U32(x,y) (*(volatile uint32_t*)(base + (uint32_t)(x)) = __builtin_bswap32(y))
  ```
  MMIO (GPU) addresses divert to an `MMIOHandler` via a parallel `PPC_MM_*` family.
- **Function ABI** — `void f(PPCContext& ctx, uint8_t* base)`. **No per-argument
  params**; guest args are read from `ctx.r3…` inside. Because *every* function
  takes the full context, any branch target can be promoted to a function and the
  branch becomes a semantically-correct tail-call — the load-bearing trick.

## Per-instruction translation (the table)

| guest | emitted C++ |
|---|---|
| `add rD,rA,rB` | `ctx.rD.u64 = ctx.rA.u64 + ctx.rB.u64;` |
| `addi rD,rA,i` | `ctx.rD.s64 = ctx.rA.s64 + i;` |
| `lis rD,i` | `ctx.rD.s64 = (int32_t)(i<<16);` |
| `lwz rD,d(rA)` | `ctx.rD.u64 = PPC_LOAD_U32(ctx.rA.u32 + d);` |
| `stwu rS,d(rA)` | `ea = ctx.rA.u32 + d; PPC_STORE_U32(ea, ctx.rS.u32); ctx.rA.u32 = ea;` |
| `cmpwi cr0,rA,i` | `ctx.cr0.compare<int32_t>(ctx.rA.s32, i, ctx.xer);` |
| `beq cr0,loc` | `if (ctx.cr0.eq) goto loc_XXXX;` |
| `bl target` | `ctx.lr = ret; sub_TARGET(ctx, base);` |
| `bctrl` | `ctx.lr = ret; PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);` |
| `blr` | `return;` |

Record forms (`add.`) append `ctx.cr0.compare<int32_t>(result, 0, ctx.xer);`.
Unknown opcodes emit `PPC_UNIMPLEMENTED(...)` — an honest trap, never a silent
wrong translation.

## Control flow & the indirect-call table

- Intra-function branches → `goto loc_X;`. Direct `bl` → a direct C++ call.
- **Indirect** calls (`bctr`/`bctrl`, virtual dispatch) → `PPC_CALL_INDIRECT_FUNC`
  which indexes an **address-keyed function-pointer table** (`PPCFuncMappings[]`,
  one `PPCFunc*` slot per guest code address). Null slots log "missing" and no-op
  rather than crash.
- **Switch/jump tables** → a real C++ `switch` with `case` → `goto`/tail-call and
  a `default:` indirect fallback.

## Imports / kernel / CRT

Calls into xboxkrnl / XAM / the CRT are **not recompiled** — they're replaced by
hand-written host C++. A guest import becomes `__imp__Name(ctx, base)`, a
trampoline that **marshals guest registers into typed host arguments**
(`r3,r4,… → params`), calls the native handler, and writes the result back to
`ctx.r3`. Unimplemented imports get warning stubs.

## The driver

`rexglue codegen <config.toml>` → load TOML + XEX → **Analyze** (function
discovery, block splitting, jump-table detection) → **Write** (per-function
`emitCpp`, split into N `*_recomp.i.cpp` shards, plus the `PPCFuncMappings`
table). Content-hashed for incremental rebuilds.

## The hard part is function discovery, not translation (MinecraftPC lessons)

The per-instruction emitter is mechanical. What actually took iterative
crash-debugging on a real title:

- **`setjmp`/`longjmp` must be host-intercepted.** Recompiled as ordinary
  functions they do a register-restore + plain return that corrupts `r1`/`r31`
  (observed: `sub_8279F9A8 ZEROED r31` → null `this` → AV in the JPEG/PNG loader).
  Declare `setjmp_address`/`longjmp_address` → emit host `ppc_setjmp`/`ppc_longjmp`.
- **Missed function entries crash the game.** Three classes:
  - *vtable/function-table thunks* the static scan skipped (e.g. a `this`-adjusting
    destructor thunk → silent no-op → `free()` on a `+4` pointer → AV). Recover by
    scanning the image for code addresses referenced as pointers.
  - *out-of-line blocks* (cold paths, shared epilogues, jump-table case bodies)
    not sealed as entries → `UnresolvedCall`. Promote to functions (safe: full
    context passed → tail-call).
  - *switch-table tail truncation* → "undeclared label"; fix with a size override
    to the next real function.

So a full port needs, beyond the emitter: a **linear-sweep + entry-sealing +
pointer/vtable-scan function discoverer**, **switch-table tail inclusion**, and a
**runtime** (kernel/CRT hosts, the indirect-call table, MMIO→GPU, setjmp/longjmp).

## Our reimplementation status

`ppcrecomp.py` is a **working proof-of-concept** of the emitter: it recompiles a
real Tetris function to the exact C++ shape above and the output **compiles with
g++** (`--check`). It ships a minimal own-code runtime header (`rex_ppc_min.h`:
`PPCContext`, the bswap load/store macros, the `PPC_FUNC` ABI). On the Tetris
entry function: 78/80 instructions translated, the rest honestly trapped.

```bash
XboxTranspiler ppcrecomp default.xex --vaddr 0x820FBC58 --size 0x140 -o func.cpp --check
XboxTranspiler ppcrecomp --emit-runtime rex_ppc_min.h
```

**What a full port adds** (roadmap, in order): (1) ~~the function discoverer~~
— **done** (`ppcdiscover.py`): `.pdata` RUNTIME_FUNCTION parse (exact extents) +
`bl`-descent recovers **8,853 functions on Tetris, 0 false positives** vs the
reference's table (87.5% recall; the missing 12.5% are vtable/pointer-referenced
methods + imports); (2) the instruction table — floating-point, record forms,
rotates and a few integer ops added, now **97.4% instruction coverage** across
300 real functions (remaining gap: VMX128 + rarer FP/integer forms); (3) the
runtime (indirect-call table, kernel/CRT host shims, setjmp/longjmp interception,
MMIO→GPU bridge) — **not yet**; (4) ~~whole-image emission + a `PPCFuncMappings`
table~~ — **done** (`ppcrecomp --all`: discovers, recompiles every function, emits
the mapping table, and g++ compiles the result).

```bash
XboxTranspiler ppcdiscover default.xex                 # function boundaries
XboxTranspiler ppcrecomp default.xex --all --limit 300 -o img.cpp --check
```

**Recompiled code RUNS correctly (proven end-to-end).** `ppcrun.py` closes and
self-checks the whole loop with no emulator oracle: it compiles a C function with
the **XDK**, recompiles the resulting real PowerPC through our tools, links it
with g++, **executes** it, and compares the result to the C function computed
natively. All cases pass (arithmetic, multiply, shift, bitwise) — so
XDK-compile → our-disasm → our-recompile → host-compile → run is correct on real
instructions.

```bash
XboxTranspiler ppcrun --selftest          # recompiled PowerPC runs, matches native
```

**Discovery tail + indirect-call table — done.** Vtable/pointer scanning
(`ppcdiscover`, scan data for code-address pointers with a prologue check, reject
addresses interior to known functions) lifts recall to **93.6%** on Tetris at 17
false positives (0.18%). The **indirect-call table** (address-keyed
`PPCFuncMapping` + `ppc_call_indirect`) is proven: `ppcrun` recompiles two
functions at distinct guest addresses, links them, and dispatches **indirectly**
through the table — `inc(dbl(5))` runs correctly. So multi-function linking and
`bctrl`/vtable dispatch work.

**Runtime scaffold stood up (`ppcboot.py`).** The load-bearing runtime pieces are
in place and proven: **guest memory + flat image load** — recompiled code reads
the real Tetris image through the byte-swapping `PPC_LOAD` (`deref(0x82080000)` →
`0x7D8802A6`, the `mflr` prologue; `deref(0x82000000)` → `0x4D5A9000`, `MZ`);
plus a **host-shim registry** (`__imp__`/`ppc_lookup` import trampolines) and
**`ppc_setjmp`/`ppc_longjmp`** host helpers (the MinecraftPC crash fix), which
compile. `rex_ppc_min.h` + the `ppcboot` scaffold is our own runtime header.

Next real work (whole-program, later): fill the scaffold with real
**xboxkrnl/XAM/CRT host implementations**, wire `setjmp`/`longjmp` address
declarations into the emitter, add the **MMIO→GPU bridge** and thread/heap setup,
so an *entire* recompiled game links and runs. Plus the **VMX128** instruction
builders and resolving object-side `bl` relocations. Each is mechanical given the
proven model.
