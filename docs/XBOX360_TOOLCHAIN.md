# The Xbox 360 toolchain, as the PowerPC lane relies on it

How the Xbox 360 SDK turns C and C++ into a title, stage by stage, and which of those facts
CodeBreaker's PowerPC lane depends on. It summarizes a reverse-engineering study of the SDK's
own tools: the compiler driver (`cl.exe`), the front-ends (`c1.dll`, `c1xx.dll`), the back-end
(`c2.dll`), the linker (`link.exe`) and the packager (`imagexex.exe`). The study read their
decompiled code and checked its findings against what the tools emit. The tools and their
decompiles are Microsoft's: they stay on the machine that has the SDK and are never committed
(docs/NOT_IN_REPO.md). This file keeps only the findings.

## The chain

```
source.c / .cpp
  cl.exe        reads the options and loads each phase DLL in-process (_InvokeCompilerPassW@16)
  c1 / c1xx     preprocess, parse, semantics; writes the program as an IL byte-stream
  c2            reads the IL; optimizes; allocates registers; selects PowerPC/VMX128; writes .obj
  link.exe      resolves symbols; /OPT:REF then /OPT:ICF; lays out a big-endian PowerPC PE
  imagexex      packs the PE as a XEX2 (compressed, page-hashed, signed)
```

CodeBreaker works this chain backwards. `native/xex` unpacks the XEX2 to the PE ("basefile").
`formats/powerpc_image.py` reads the PE. The decompiler turns the PE's functions back into C.
`build/functions.py` compiles that C with the same `cl.exe` and links it with the same
`link.exe`, and `verify/lanes/powerpc.py` runs both images side by side.

## What the lane takes from each stage

**The driver's defaults.** Every compile gets these from `cl.exe` before the user's own
options: `-D_M_PPCBE -D_XBOX_VER=200 -QVMX128 -Zp8 -fp:fast -Gd -Ob0 -Ot -ML`. It is VC10
(16.00.11886). So a title's code is `__cdecl`, packs structures to 8 bytes, does not inline
unless told to, and has the 128-register vector file switched on. CodeBreaker compiles through
the same driver, so the defaults come along without being named.
`verify/authored/match_powerpc.py --try` pins down the switches a module was built with. On the SDK's
`hud` sample, `/O1` matches 46 functions exactly and `/O2` matches 45.

**VMX128.** The Xenon has 128 vector registers. AltiVec's encodings (primary opcode 4, 5-bit
register fields) can name only 32 of them. So each vector operation also has a `*128` form
in primary opcodes 5 and 6, with each 7-bit register number split across several fields. In
the compiler's machine description every AltiVec operation is followed by its `*128` twin,
which has its own operand-format class (`0x56`-`0x6F`) and encoding word. The back-end picks
the twin whenever the register allocator puts a value in `v32`-`v127`, and the allocator
colors the whole 128-register file. That is why most vector code in a title is `*128` code
(Minecraft: 33,163 of its 38,074 vector words).

- Decode table: `codebreaker/decompile/vmx128.py` (77 rows). Its base encodings agree with the
  compiler's own table: `vperm128 14000000`, `vor128 140002D0`, `vand128 14000210`,
  `vxor128 14000310`.
- SLEIGH: generated from that table (`PowerPC:BE:64:Xenon`).
- `vsel128` has a format class of its own because its destination is also the selector. The
  table writes it as `(vA & ~vD) | (vB & vD)`.
- The compiler also lists two names the decoder spells differently:
  - `vmr128`, a move: `vor128` with both sources the same;
  - `dcbz128`, the 128-byte cache-line clear, which the decoder prints by its POWER name
    `dcbzl`.

**Register allocation.** `c2` colors with a graph-coloring allocator, run once per register
class: 32 general registers, 32 floating-point, 128 vector. It saves callee-saved registers
through shared helper runs (`__savegprlr`/`__restgprlr` and the `stfd`/`lfd` runs). A
function calls into those runs instead of saving inline, and often leaves by jumping into the
restore run instead of ending in `blr`. `formats/powerpc_image.py` treats these runs as part
of their callers, and `verify/authored/match_powerpc.py` accepts that tail jump as a function's end.

**Instruction selection.** `c2` routes each of 174 IL operations through 41 handlers to its
PowerPC form. Strength reductions apply: `a * 4` becomes `slwi`. This is the gradient
`repair` searches along. The C that compiles back to the original bytes is the C whose
operations select the same instructions.

**Objects.** `c2` writes COFF with big-endian code and little-endian headers. Every place
the linker fills in is a relocation of one of the PowerPC types:

| Relocation type | What the linker fills in |
|---|---|
| `REL24`/`ADDR24` | the 24-bit distance of `b`/`bl` |
| `REL14`/`ADDR14` | the 14-bit field of `bc` |
| `REFHI`/`REFLO`, `ADDR16`, `TOCREL*`, `SECREL16` | a 16-bit half of an address (`lis`/`addi`, loads and stores) |
| `ADDR32`, `SECREL` | a whole word |

`verify/authored/match_powerpc.py` masks exactly those bits and keeps comparing the opcode and
registers of a relocated instruction. Labels inside a function (`$M...`, storage class 6) are
not function symbols.

**The linker.** `/OPT:REF` runs first and drops what nothing references. `/OPT:ICF` then
folds sections with identical contents into one, keyed by a content hash. So one body in a
title can stand for several functions of the source, and those functions can no longer be
told apart: the image keeps one address for all of them. The `.pdata` record of a function
gives its length, the length of its prologue and whether it has an exception handler. A
record's length can take in the padding after the function and a small leaf function that
has no record of its own. `match_powerpc` therefore ends a function at the first zero word.

**C++.** Run-time type information sits beside each virtual table: the locator (signature 0)
before the table points at the type descriptor, which holds the decorated class name. Where
that chain survives, `powerpc_image.virtual_methods` names every slot (`Class_vfN`). C++ names
use Microsoft's decoration scheme, the one `symbols/demangle.py` reads.

**The packager.** The XEX2 container (zero-omitted pages, LZX, AES, the page-hash chain, the
signature) is `native/xex`'s subject; see docs/XEX.md.

## The other tools of the SDK

The same study covers the tools around the build chain:

- **The shader compiler** (`fxc`, `vsa`, `psa`): HLSL to Xenos GPU microcode. The microcode is
  the console's second instruction set: 96-bit ALU words that pair a vector and a scalar
  operation, plus fetch and control-flow words.
- **The patch builder** (`deltaxex`): a title update is an LZX delta over the decompressed
  base image.
- **The audio encoder** (`xma2encode`): the XMA2 container. See `formats/xbox/xma.py`.
- **The UI compiler** (`XuiTool`): `.xui` scenes to XUR. See `formats/xbox/xur.py`.
