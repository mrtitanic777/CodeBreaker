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

## PowerPC (2026-10-19; `tools/sleigh_verify_ppc.py`, report `state/sleigh_verify_ppc.txt`)

The specification the PowerPC lane decompiles with (`ppc_64_isa_altivec_be.sla`,
`PowerPC:BE:64:A2ALT-32addr`), on every instruction form the Xbox 360 SDK's `libcmt.lib`,
`d3d9.lib` and `xapilib.lib` use (compiler output and hand-written runtime), 3 encodings x 8
states each. `pcodecheck` mode `ppc-be-64`: big-endian memory, addresses cut to 32 bits (as the
language truncates `ram`), code not given decodes as `nop`.

The reference is Unicorn's 64-bit PowerPC (the 970 model). It cannot have its machine state
register set (a write, `mtmsr` and `mtmsrd` all leave it 0), so it runs in 32-bit mode with
floating point off. Its XER SO/OV/CA are set by running `mtxer` and read back by running `mfxer`
(a direct register write or read misses them). Registers hold sign-extended 32-bit values, the
state of code built for 32-bit addresses, and from those the two modes agree except where a
result does not fit 32 bits. There the LT/GT/EQ of a record form and OV are not compared, nor
the high word of an effective address (an update form's base register, a branch target).
Forms Unicorn will not run in that mode (64-bit instructions: `ld`, `std`, `rldicl`, `extsw`; and
floating point) run instead on CodeBreaker's own interpreter (`emulate/powerpc.py`), the one the
PowerPC lane verifies with. The report marks them "(interp)". SO and OV are not compared there.

| | Forms |
|---|---|
| Agree before the fixes | 226 |
| Agree after the fixes | **233** |
| Disagree, explained (below) | 4 |
| Untested (VMX vector ops, which neither reference runs; cache hints; `sync`; traps; `mtmsrd`; the time base) | 42 |

Fixed in the specification (docs/GHIDRA_FORK.md, L6):

| Instruction | Was | Now |
|---|---|---|
| `mtxer`, `mfxer` | moved the raw XER only: SO, OV and CA (registers of their own, `xer_so` ..., which the arithmetic sets and reads) were left as they were / not read | moved too |
| `stwcx.`, `stdcx.` | stored a user operation's result (`*EA = storeWordConditionalIndexed(S, ...)`); without the reservation cr0 was left as it was | store the register; without the reservation, store nothing and clear LT/GT/EQ; the reservation is dropped either way |
| `fctid`, `fctiw` (and their record forms) | truncated toward zero | rounded, as the default rounding mode does (the `z` forms still truncate) |

Found on the way: `pcodecheck` never gave the translator its float formats, so every floating-point
p-code operation was "unimplemented". It now does, for x86's x87 too. Each case also gets a
context of its own, because PowerPC's context variables are locked after the first case. The
interpreter's `stwcx.`/`stdcx.` always succeeded; it now keeps the reservation `lwarx`/`ldarx`
make, which changes nothing for real code, where one always comes first.

Disagreements left:
- `divwo.`: Unicorn keeps OV set after a division that does not overflow; the ISA says OE=1 clears
  it. A reference bug; the specification is right.
- `fctid`: an exact tie (`-0.5`). The processor rounds to even; p-code's `round` rounds away from zero.
- `lwzx`, `stwbrx`: an access that wraps past 0xffffffff, which only random states produce.
