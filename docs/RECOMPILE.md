# Recompiling: a whole program as C++ that runs on a PC

CodeBreaker's main deliverable is source to read: functions decompiled, rebuilt and judged
one by one. This lane produces something else: the whole program, ahead of time, as C++ that
builds and runs on another machine. The output is not readable source. It keeps the original's
behaviour at the level of registers and memory. Today it covers Xbox 360 titles, in
`codebreaker/recompile/`.

```bash
python -m codebreaker recompile default.xex --all -o game.cpp          # every function
python -m codebreaker recompile default.xex --at 0x820FBC58 --size 0x140 --check
python -m codebreaker recompile-project default.xex -o C:\Games\TetrisPC
```

## The method

This is the method XenonRecomp introduced and the rexglue SDK carries out. Its code generator is
the source of `recompile/builders/` (BSD-3, see NOTICE). It is an ahead-of-time translation, not
an emulator:

- **Functions.** Each guest function becomes `void sub_XXXXXXXX(PPCContext& ctx, uint8_t* base)`.
  Functions take no arguments of their own: guest arguments are read from `ctx.r3`... So any
  branch target can become a function, and a branch to it a tail call.
- **Statements.** There is one C++ statement per guest instruction, for example
  `add rD,rA,rB` → `ctx.rD.u64 = ctx.rA.u64 + ctx.rB.u64;` (powerpc.py lists the forms).
  Record forms (`add.`) append the CR0 update.
- **Memory.** Guest memory is one flat 4 GB allocation, so a guest address is `base + addr`.
  Every access of two bytes or more is byte-swapped, and volatile, so the compiler keeps all of
  the guest's memory traffic.
- **Calls.** Direct calls are C++ calls. Indirect calls (`bctrl`, virtual methods) go through
  a table of function pointers keyed by guest address. A `bctr` through a jump table becomes a
  `switch` (jumptable.py).
- **Imports.** Kernel and system-library calls become host functions. Each is one of:
  - the rexglue kernel core's own implementation of the export (`__imp__<name>`);
  - a host implementation of CodeBreaker's;
  - the native GPU subsystem, for `Vd*`;
  - a stub that logs once.
- **Untranslated instructions.** An instruction no builder translates becomes
  `PPC_UNIMPLEMENTED("...")`. It is a trap at run time, never a wrong translation.
- **VMX128.** The Xenon's 128-register vector set is decoded first, by
  `codebreaker/decompile/vmx128.py`. Capstone reads those words as later instructions the
  Xenon doesn't have.

## Lessons from running real titles

The per-instruction emitter is mechanical. Getting real titles to run turned up the following,
much of it the hard way:

- **Finding every function is the hard part.** A function that isn't found is a call that does
  nothing at run time. In one title a `this`-adjusting destructor thunk was missed, so `free()`
  ran on a pointer 4 bytes off and the process crashed. `discover.py` therefore takes more than
  `.pdata` and calls:
  - every code address stored as a pointer in the data sections, where a function's first
    instruction plausibly stands (virtual tables, function tables);
  - addresses seeded from a run.

  `recompile_image` then also turns every address the generated code calls but that isn't yet
  a function into one. Tail jumps go to such places, and the register-restore runs are entered
  that way, with one entry per register count.
- **Cold blocks and shared epilogues reached by a jump become functions.** That is safe, because
  every function takes the whole context. A switch table's last case belongs to the switch
  (size overrides).
- **`setjmp`/`longjmp` must be the host's.** Recompiled as ordinary functions, they restore
  registers and return normally, which corrupts `r1`/`r31`. In one title that zeroed `this` in an
  image loader. The runtime has to intercept their addresses: `ppc_setjmp`/`ppc_longjmp`, and
  the rexglue context's jump-buffer map. Wiring those addresses into the emitter is still
  open (below).
- **A literal zero is not `r0`.** In X-form loads and stores, Capstone writes the base `rA = 0`
  as `0`, and that means the literal zero, not register r0. The builders read it that way.
  (Before this was fixed, every `stfiwx f0, 0, r11` was left untranslated.)

## Where it stands (2026-10-21)

Tetris (`default_unc.xex`):

- **Discovery.** 9,262 functions found: 6,683 from `.pdata`, the calls, and 685 more through
  pointers. Recompiling adds 104 tail-jump entries.
- **Translation.** 99.8% of 881,184 instructions. Every real instruction is translated; what is
  left is zero padding between functions.
- **Compiling.** Samples of the output (600 random functions and 200 vector-heavy ones) compile
  against the kernel core with g++ 15 (C++23).
- **Running.** `tests/test_recompile.py` compiles C with the Xbox 360 compiler, recompiles it,
  builds it, runs it and compares the result with the C computed natively. It covers:
  - integer arithmetic, division, shifts and conditionals;
  - floating point;
  - a loop over big-endian guest memory;
  - dispatch through the indirect-call table.

**The generated Tetris project builds and links with MinGW g++ 15 and Ninja**, producing a
23 MB `tetris.exe`. The pieces are the title's 44 translation units, the kernel core and the
DXBC translator. Earlier copies of the kernel never linked. To get here, the kernel needed:

- the SDK's base library (strings, threads, memory, clocks, the exception handler), now in
  `native/rexglue/kernel/src/core`;
- the headers and sources its subset lacked: `src/core`, `virtual_file_system.cpp`, the
  host-path and null filesystem entries, CLI11, picosha2;
- three GCC adaptations, marked `CodeBreaker:`: SEH thread naming, `__assume`, and
  `roundevenf`, which MinGW's libm lacks;
- in the DXBC translator, its stand-ins for base-library functions left out when it links
  beside the kernel;
- every executable section counted as code. A title's linked-in libraries live in `.embsec_`
  sections past `.text`, which the image reader had never read.

**Running it.** The D3D12 device comes up, the kernel loads the XEX, creates the title's main
thread and runs recompiled code: the entry point `sub_820FBC58`, then `sub_820FC340`, then
`sub_820FC268`. That third function faults at its first kernel-variable import. It reads a
pointer from an import slot (`lwz r11, 0x8a4(0x820008A4 slot)`) that the loader has not
filled with the variable's address, then dereferences it.

To run a whole title, these remain:

- **Variable imports.** The kernel's exported variables must be placed in guest memory and
  their import slots filled at load. This is the first thing the title does.
- **Host implementations** of the imports the kernel core does not implement. These are stubs
  that log once today.
- **`setjmp`/`longjmp`** addresses wired into the emitter.
- **The GPU bridge.** The PM4 ring, register file and D3D12 backend are there. The three Xenos
  translators are still to fill: shaders to DXBC (the rexglue DXBC translator plugs in), the
  EDRAM render-target cache, and texture untiling.
- **Thread and heap setup** at boot.
