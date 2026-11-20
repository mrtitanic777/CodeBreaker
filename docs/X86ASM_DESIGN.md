# x86asm — CodeBreaker's own x86 assembler

CodeBreaker assembles the x86 sources its writers produce with an engine of its own, `native/cb/build/x86asm/`, compiled
into `codebreaker.exe`. It replaces the separate `nasm.exe` (the `native/nasm` fork and the `nasm` setting), which
CodeBreaker used to start as a child process. When x86asm is complete, `native/nasm` and the setting are deleted.

## What "done" means

**Functionally identical to NASM 3.02 with CodeBreaker's MASM mode** (`native/nasm`, docs/MASM-COMPATIBILITY.md):
for every input, the same bytes out — the same object or image, the same errors refused. Not "valid machine code":
the writers spell an instruction as a mnemonic only where NASM's encoding is the original's bytes, so the engine must
make every choice NASM makes (shortest form, operand-size and ModRM choices, the multi-pass sizing of jumps, the order
of records in an OMF or COFF object).

The proof is differential, never by eye:

- **The capture corpus.** With `CB_NASM_CAPTURE=<folder>` set, every NASM run CodeBreaker makes saves its input
  source, its options and the reference `nasm.exe`'s output (and exit code, and messages) as one case. A gate run and
  a corpus sweep give the real inputs: the PE/NE/LE/x64/WonderSwan writers' flat images, the boot floppy, the Win16
  idioms (`--masm -f obj`), the VxD sources (`--masm -f win32`).
- **NASM's own MASM-mode tests** (`native/nasm/test/masm`).
- `cb_tests` area `x86asm` runs every case through x86asm and compares bytes with the recorded output. A case that
  differs is a bug in x86asm; nothing is ever "close enough".

## Architecture

NASM's seams are kept, as C++ modules (idiomatic typed C++, docs/CPP rules): each module is a port of NASM's file of
the same job, with NASM's behaviour, not a re-imagining of it.

| Module (`native/cb/build/x86asm/`) | NASM source it ports | Job |
|---|---|---|
| `types.hpp` | include/nasm.h, insns.h, tables.h, labels.h | tokens, expressions, operands, the instruction, locations, output records |
| `assembly.hpp/.cpp` | the globals of nasm.c, error.c | `Assembly`: ALL state of one run (options, pass, location, labels, warnings, diagnostics) |
| `scan`, `eval`, `labels`, `floats`, `strfunc`, `quote` | stdscan.c, eval.c, exprlib.c, labels.c, floats.c, strfunc.c, quote.c, rdstrnum.c | tokens and expressions |
| `insns` (+ generated table) | x86/insns.dat, x86/regs.dat, insnsb.c, insnsd.c, regflags.c, regvals.c | the instruction and register tables, as C++ data |
| `encode` | assemble.c | instruction size and bytes (`insn_size`, `assemble`) |
| `parse` | parser.c | one line into an `Insn` |
| `preproc` | preproc.c, pptok, macros.c, the standard macros | the preprocessor and MASM mode |
| `directives` | directiv.c, pragma.c, segalloc.c | directives and pragmas |
| `format_bin`, `format_obj`, `format_coff` | output/outbin.c, outobj.c, outcoff.c, outlib.c | the three output formats CodeBreaker uses |
| `listing` | listing.c | `-l` listings |
| `driver` | nasm.c | the pass loop and the command line |

### Rules

1. **No global or static mutable state.** Everything NASM keeps in globals lives in the `Assembly` object of the run.
   Two assemblies run at once in one process without touching each other.
2. **No `exit()`, no `abort()`.** A fatal error is an exception (`AsmError`, build/asm_text.hpp) the caller catches;
   errors and warnings are collected in the `Assembly` with NASM's exact text and line.
3. **No files unless asked.** The API is in memory: `assemble(Request{source, includes, format, options}) ->
   Result{bytes, listing, diagnostics, exit}`. `%include` reads through a callback (files, or a map of names to text).
4. **The command line too.** `codebreaker nasm <NASM's arguments>` behaves as `nasm.exe` did, for people and tests.
5. **Speed.** Faster than starting `nasm.exe`: no process, no disk. Measured on the capture corpus.
6. **Tests.** Each module has `*_test.cpp` in area `x86asm`; the differential test is the acceptance.

## Phases

1. Types, `Assembly`, scanner, expressions, labels, floats.
2. Tables and the encoder (16/32/64-bit, all of NASM's instructions).
3. Parser, directives, driver, the bin format → the flat-image writers and the floppy switch to x86asm.
4. Preprocessor (NASM language), then MASM mode; obj and coff formats → the Win16 idioms and the VxD builds switch.
5. `native/nasm`, the `nasm` setting and every child-process path deleted.
