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

Tetris (Tetris Splash, XBLA): `recompile-project` makes the project in 19 seconds — 14,861
functions, 112 switch tables, no unimplemented instruction — and it builds with clang-cl.
`tetris.exe` loads the title, binds its 217 imports (36 still stubs: networking, the camera,
some XAM UI), runs its main thread, two worker threads and the system threads, reads its data
(`bin.tort`) through asynchronous reads with completion routines, and draws: frame 300 is the
Xbox LIVE Arcade splash. It then stops at the first texture in a format the renderer does not
support yet, `k_4_4_4_4`.

Next:

- **Texture formats**: the renderer supports `k_8`, `k_8_8`, `k_8_8_8_8` and `k_16_16`;
  Tetris needs `k_4_4_4_4` and, most likely, the DXT formats.
- **The stubbed imports** a title reaches (Tetris: `NetDll_recvfrom` is polled).
- **A second title** through `recompile-project`, to find what is Tetris-specific.
