# Recompiling: a whole program as C++ that runs on a PC

CodeBreaker's main deliverable is source to read: functions decompiled, rebuilt and judged
one by one. This lane produces something else: the whole program, ahead of time, as C++ that
builds and runs on another machine. The output is not readable source. It keeps the original's
behaviour at the level of registers and memory. Today it covers Xbox 360 titles.

```bash
python -m codebreaker recompile-project default.xex -o C:\Games\Tetris --name tetris --title-files <the title's folder>
C:\Games\Tetris\Build-tetris.bat
C:\Games\Tetris\build\tetris.exe C:\Games\Tetris\game
```

## The pieces

Two tools were merged, each giving what it does best:

- **XenonRecomp** (`native/xenonrecomp`, hedge-dev's, MIT, with the Xbox 360 ports' changes)
  translates. Each guest instruction becomes one C++ statement on a `PPCContext`; guest memory
  is one flat big-endian 4 GB space; every function takes the whole context, so any branch
  target can be a function and a branch to it a tail call; a `bctr` through a table becomes a
  `switch`. `codebreaker/recompile/xenon.py` builds it (clang-cl, Ninja; setting
  `visual_studio`) and runs it.
- **CodeBreaker's analysis** configures it (`codebreaker/recompile/config.py`): what
  XenonRecomp cannot find by itself.
  - Every function `.pdata` leaves out, sized (`discover.py`): call targets, code addresses
    stored in the data, addresses the code builds (`lis` + `addi`/`ori`), and leaf functions that
    start right after another one ends. A switch's case labels are never starts (an absolute
    table is data holding code addresses), and the function holding a switch runs to the end of
    its last case.
  - The register save/restore runs (`__savegprlr_14` ... `__restvmx_64`, found by their code)
    and the CRT's `setjmp`/`longjmp` (found by the jump buffer they use).
  - The switch tables (`switches.py`, which walks back from every `bctr` and evaluates CTR
    symbolically; XenonAnalyse's tables are added for any it leaves unresolved).
  - The register settings: which registers are locals of each function. The non-volatile ones
    stay in the context: a compiler-generated `__finally` block is called with `bl` and reads its
    parent's `r30`/`r31`.
- **The runtime** (`native/x360`, CodeBreaker's Xbox 360 runtime, from the Xbox 360 ports) runs
  the result: the kernel and XAM reimplemented export by export, the Xenos GPU on Direct3D 12,
  XMA audio, input, the file system (the title's folder is `game:`), the system UI.
  `x360.cmake` builds a title's program; `native/x360/README.md` lists its switches.
- **Hooks**: XenonRecomp emits every function as `__imp__sub_X` plus a weak `sub_X`, so a host
  function defined as `sub_X` replaces it; `midasm_hook` tables in a title's configuration put
  host code at single instructions.
- **XEX**: the runtime and XenonRecomp read only unencrypted, basic-compressed XEX files;
  `codebreaker.formats.xex.basic` rewrites any XEX that way.

The rexglue SDK's kernel core (`native/rexglue`) and CodeBreaker's own per-instruction
translator (`powerpc.py`, its builders derived from rexglue's code generator) remain:
`recompile` translates functions on their own (and `tests/test_recompile.py` runs them), and
`recompile-project --runtime rexglue` still makes the earlier project form.

## Checking the translation

`xenon.instruction_tests()` runs Xenia's PowerPC instruction tests (inputs and the outputs
measured on the console; `native/xenonrecomp/ppc_tests`) through XenonRecomp: each test is
assembled with PowerPC binutils that know VMX128 (setting `ppc_binutils`, default
`toolchains/binutils-ppc`), recompiled, built and run, with CodeBreaker's own tests beside them
(`ppc_tests/own`: divides, `frsqrte`, the dashboard's VMX). Today: 162 test files, 0 mismatches; 8 are
left out because XenonRecomp does not implement their instructions (`lvexx`, `mulhdu`, `rlwnm`,
`subfme`, `td`, `tdi`, `tw`, `twi`; a title's own recompile reports any it uses).

## Lessons from running real titles

- **Finding every function is the hard part.** A function that isn't found is an indirect call
  that lands nowhere. Tetris needed functions reached only by an address the code builds (the
  CRT's `FlsAlloc` stand-in, the callbacks it registers) and leaf functions after a `blr` (D3D's
  device setters).
- **Switch tables split functions** when their labels are taken for starts: 126 cases "outside
  their function" until labels were excluded.
- **`setjmp`/`longjmp` must be the host's.** As ordinary functions, `longjmp` returns, and its
  caller goes on with registers it never restored: in Tetris, `this` was null in D3D's device
  setup.
- **A function that never returns** (`bl` to `longjmp` then padding) shows up as a callee that
  returns with `r31` changed. The fault report's host stack (the recompiled functions are the
  frames) finds it.
- **Imports through pointers.** The CRT calls `KeTlsGetValue` through a pointer `TlsGetValue`
  hands back, so import stubs belong in the indirect-call table.
- **A literal zero is not `r0`.** In X-form loads and stores, `rA = 0` is the literal zero.
- **A function split in two must still run into its second half.** Discovery declares functions
  inside others (a shared tail several functions branch to); the piece before one falls through
  into it on the console. XenonRecomp ended such a piece without continuing: in Up, 191 functions
  stopped short, among them the shader compiler's register check, which returned its pointer
  argument as "in use" ("error X3526: register 16 already in use"). It now calls the next function.
- **For the same reason, CR, CTR and XER live in the context.** A piece entered by a branch tests
  the flags or counts down the CTR its caller set (`cmplwi cr6,r11,1; b 0x8212E5F4`, which starts
  with `bne cr6`); as locals they were lost. The runtime is built with the title's settings, and
  regenerating with other settings re-reads them (the context layouts must agree).
- **Word divides write the whole register.** `divw`/`divwu` wrote only the low word, keeping a stale
  high word for the 64-bit shifts that follow; all divides by zero faulted the host. The quotient
  is now zero-extended and a zero divisor gives 0.
- **An inline jump table ends at the first case after it,** not at its lowest case: one of Up's
  cases jumps back before the table, which cut it to 2 of its 11 entries.
- **Comparing with another recompiler finds the rest.** Diffing our C++ against the reference
  port's (rexglue, the same output style) function by function found the divides; swapping its
  bodies into our build in bulk showed the shader-compiler fault was not in the translation of any
  matching function, which pointed at the boundaries.

## Where it stands (2026-10-22)

Tetris (Tetris Splash, XBLA) plays. `recompile-project` makes the project in about 20 seconds
(14,895 functions, 114 switch tables, no unimplemented instruction); it builds with clang-cl and
`Play-tetris.bat` starts it. It runs with no unimplemented import or message reached: the XBLA
splash, the Tetris Online logo, the title screen, the menus, a Marathon game with its tutorial,
with sound (the earlier rexglue-based port never had any). The title folder holds the disc's
files only: the `devData` folder of loose files an earlier port unpacked from `bin.tort` sends
the game down a developer path that ends in "File not found".

What it took, beyond the plumbing: texture formats (the packed 16-bit ones, DXT1/2_3/4_5, the
8-bit aliases), cube textures, fetch constants that are not textures read as zero, the gamer
picture into a texture (`XamReadTileToTextureEx`, looked up by ordinal), sockets with no link,
XSessions offline, and two recompiler fixes: tail-jump targets declared as functions, and
XenonRecomp no longer dropping a branch to an unknown address (it wrote a comment; XACT's
sound-bank sizing then came out one byte short).

Up (Disney/Pixar, Heavy Iron) plays: 44,283 functions, 461 switch tables. It runs through the legal
screen, the studio logos, the title screen, the save-slot menu, the in-engine cutscene and into the first
biplane level at 60 frames a second. What it took: the recompiler fixes above, `frsqrte`,
`XamInputSetState` (rumble on a host controller), `XMPGetPlaybackController`, and in the renderer
primitive reset (strips and fans cut by the reset index), `k_24_8` depth textures, 32-bit float
textures and `getTextureGradients`. Open: dark speckle on lit surfaces in the level. The same fixes
applied to Tetris (137 functions had stopped short) and Minecraft (3,393), which still play.

Next: a long play session on a real controller; the first shader of a scene takes seconds to
compile with FXC the first time (the shader cache keeps it); a second title through
`recompile-project`, to find what is Tetris-specific.
