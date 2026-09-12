# XboxTranspiler — roadmap

The north star, carried from WinTranspiler: **finish the reconstruction with
deterministic local tools — no LLM.** This fork brings that to the Xbox 360.

## Where we are (done, proven on real bytes)

- **XEX loader** (`xex.py`): XEX2/XEX1 header, optional-header directory,
  security info, original PE name, entry/base. Exact.
- **Decrypt + decompress front stage**: AES-128 (retail/devkit key), SHA-1
  block de-framing, LZX (`_lzx.py`) and basic compression. **Byte-exact against
  ground truth** (retail Tetris -> decompressed oracle, all PE sections match).
- **PowerPC disassembler** (`disasm_ppc.py`): capstone PPC big-endian,
  recursive-descent code/data. Validated on real game code.
- **Byte-exact reconstruction** (`asmrecon_ppc.py`): `.long`-backed, proven to
  re-parse to the exact section bytes; mnemonic-comment listing.
- **Pipeline wiring** (`target.py`): XEX is a first-class format —
  `format_of`/`open_module`/`code_bytes`/tiering all dispatch to it.
- **Matching-decompilation permuter** (`ppc_cc` + `ppc_obj` + `segdiff_ppc` +
  `permute --arch ppc`): compiles C with the **XDK PowerPC cl.exe** and searches
  to **byte-exact** against the shipped XEX bytes, with a bit-level relocation
  mask. Proven end-to-end with the real compiler.
- **VMX128 classifier**: the ~0.9% of words Capstone can't decode are labeled
  `vmx128` (by primary opcode 4/5/6) instead of mistaken for data; `ppcrecon`
  reports honest decode-coverage "% live" (~99% on Tetris).

## The work queue (ordered by the plan: permuter -> recompile -> readable)

1. **Static recompiler (Pipeline B — running C++).** Port the RexGlue/
   XenonRecomp method into our own code: translate each PPC function to
   compilable, runnable C++ (register-machine style), validated against the
   working Tetris port. This is the big "functioning recompiled code" win.
2. **Function boundaries + naming.** Port the recompiler-style discovery
   (save/restore GPR/FPR/VMX thunk signatures, switch/jump-table detection, the
   `.pdata` unwind table) so both the permuter and the recompiler operate on
   named functions, not one flat stream. Evidence only.
3. **Import table + thunks.** Parse the XEX import libraries (`0x000103FF`) so
   cross-module calls resolve to kernel/XAM ordinals.
4. **Readable-hypothesis layer (Pipeline C).** Reimplement the **XEXLoaderWV**
   loader logic in our own code and drive Ghidra headless for a *hypothesis*
   decompile — cross-checked against the byte-exact reconstruction, never
   blind-copied, always labeled as hypothesis (not byte-verifiable truth).
5. **Full VMX128 operand renderer.** Port the VMX128 opcode tables (`"cell 64"` /
   rexglue `opcode.h`) so vector ops get real mnemonics + operands, and (with a
   PowerPC assembler configured) `ppcrecon`'s reassemble-verified "% live".

**Note on Pipeline C (honesty):** "fully readable, rebuilt C++/C#" is not
deterministically achievable for an optimized binary — the readable layer is a
*hypothesis* (Ghidra-grade pseudo-C), while the byte-exact permuter (A) and the
runnable recompile (B) are the machine-checkable truths. Kept distinct on
purpose.

## Doctrine (unchanged)

Retail bytes are truth. "Done" = byte-exact, machine-checkable. Whatever resists
automation is flagged (`.long`, "unverified", "AMBIGUOUS"), never faked.
