# The x86 SLEIGH specification, checked instruction by instruction

docs/CORE_ROADMAP.md, Track 1 item A. Every instruction the decompiler reads is turned into
p-code by the SLEIGH specification (`native/decompiler/languages/x86.sla`); if the specification
is wrong, every later analysis is built on the wrong meaning. This checks it against a real
processor's emulator.

## How

- `native/decompiler/pcodecheck` (target `pcodecheck`): runs one instruction's p-code with the
  core's own emulator (`emulate.cc`) from a given state -- registers, flags, memory -- in 16-bit
  real mode (`segment()` = `base * 16 + offset`, as `x86-16-real.pspec` defines it), and reports
  what it left. A user operation the emulator has no meaning for (`in`, `out`, `int` ...) makes
  the case untested.
- `tools/sleigh_verify.py`: every distinct instruction form (mnemonic, operand kinds and sizes,
  prefixes) the golden corpus's 16-bit modules use -- USER, GDI, KRNL386, VGA, KEYBOARD, COMM,
  LZEXPAND -- up to 3 encodings each, from 8 random states each, run in Unicorn and through the
  p-code; compared: the general and segment registers, the flags (except those Intel leaves
  undefined for that instruction and count), the bytes of memory written, the next instruction.
  The report is written to `state/sleigh_verify.txt`.
- `native/decompiler/languages/src/x86/` holds the specification's source (Ghidra 12.1.3, Apache
  2.0); `build/bin/sleigh` (target `sleigh`, the core's own SLEIGH compiler) makes `x86.sla` from
  it. Unchanged, it compiles byte for byte to the `x86.sla` the decompiler shipped with -- once
  the compiler could read a 64-bit constant on Windows (docs/GHIDRA_FORK.md, C10).

## Result (2026-10-18; 476 forms, 8,320 cases)

| | Forms |
|---|---|
| Agree before the fixes | 395 |
| Agree after the fixes | **399** |
| Disagree, on purpose (below) | 9 |
| Untested (`in`, `out`, `int`, `hlt`, privileged in real mode) | 68 |

Fixed in the specification (docs/GHIDRA_FORK.md, L5):

| Instruction | Was | Now |
|---|---|---|
| `pushf` | FLAGS bit 1 pushed as 0 | 1, as the processor always has it |
| `iret`, `iretd` | the flags popped were not unpacked: CF, ZF ... kept their old values | unpacked, as `popf` does |
| `aad`, `aam` | SF, ZF, PF from AX | from AL (Intel SDM) |
| `shr`, `shrd` | OF's cases swapped: 0 after a shift by 1, overwritten after any other count | a shift by 1 sets OF (the operand's top bit; for `shrd` whether it changed); other counts leave it |

Left on purpose: AF is not modelled (126 forms; nothing in C reads it); far `call`/`jmp`/`retf`
do not write CS (the decompiler keeps CS per function and resolves far transfers itself); an
address-size prefix in 16-bit code widens a push's stack pointer to ESP (only in data read as
code).

Golden (tools/golden.py, 11 modules, 6,853 functions): 65 functions print differently, every one
the `pushf` fix -- the flags word they compute now has bit 1 set (`... | 4` -> `... | 6`, or
`| 2` added), a few with locals numbered differently; nothing else changed.

## x86-32 (2026-10-18; `--bits 32`, report `state/sleigh_verify32.txt`)

The same harness on the instructions KERNEL32 and GDI32 use, run as a flat 32-bit program --
every segment based at 0, as the decompiler's `x86:LE:32` sees Windows (`pcodecheck`'s mode
`x86-32-flat`; Unicorn maps a page, filled with the pattern, wherever an instruction reaches).
Forms with an FS or GS prefix are left out (their base is the thread's own block).

| | Forms |
|---|---|
| Agree | **247** |
| Disagree, on purpose | 1 |
| Untested | 29 |

No specification bug found. The one on purpose is the address-size prefix again (`67 push`: the
specification takes SP for the stack). Untested besides `in`/`out`/`int`: what depends on the
descriptor tables, which a flat program does not model -- `lar`, `lsl`, `verr`, `verw`, `iret`/`iretd`
(a popped CS of another privilege also pops SS:ESP), and a segment register's push (Intel lets
it write the selector or its zero extension). AF is not modelled (70 forms), as in 16-bit.

## Next

PowerPC against the interpreter in `codebreaker/emulate/powerpc.py`.
