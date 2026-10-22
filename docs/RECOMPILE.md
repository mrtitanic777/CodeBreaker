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
`toolchains/binutils-ppc`), recompiled, built and run. Today: 160 tests, 0 mismatches; 8 are
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

Next: a long play session on a real controller; the first shader of a scene takes seconds to
compile with FXC the first time (the shader cache keeps it); a second title through
`recompile-project`, to find what is Tetris-specific.
