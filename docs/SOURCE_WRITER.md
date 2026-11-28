# One source writer for every console

Unification item 7 (ENGINE_UNIFICATION.md). Today `src/source/*_writer.cpp` is 56 files, about 18,000 lines. Since fde43c858 every
console writer reads its code through the one disassembler and builds through the one assembler (`described_source`, `z80_source`,
`m6502_source`, `risc_source`) in its period dialect (`config/dialects/*.json`), and its hardware names come from the knowledge base
(`config/knowledge/<platform>/registers.json`). What is still written once per console is the rest: what the file is, where code
starts, the head comment, the header fields, the vectors, which hardware tables apply, the banks and how each is introduced, the build
recipe. That part is a description, not code.

## The shape

```
config/platforms/<platform>.json             what the platform is, as data (the machine's facts and its writer's)
src/source/console_writer.{hpp,cpp}           the one writer: reads the description, runs the shape it names
src/source/console_banked.cpp                 the banked shape (the processor a CPU trait: Z80, SM83)
src/source/console_platforms.cpp              per-platform code where it is truly needed: the loader (file -> image + values)
```

`console_writer::reconstruct("coleco", file, folder)` reads the description, asks the platform's loader for the program (its image, its
origin, its entry points and the named values of its header), and writes it through the **shape** the description names. A shape is
one way a program sits in memory, shared by every platform that sits that way:

| shape | how the image lies | platforms (today's writers) |
|---|---|---|
| `z80-block` | one block at one address, flow from the entries | ZX Spectrum, MSX, ColecoVision, Amstrad CPC (and the Neo Geo's sound CPU) |
| `banked` | fixed-size banks, the first N mapped at power-on, a header range that is data, vectors at fixed places | Game Boy, Master System / Game Gear / SG-1000, NES, PC Engine, Atari 2600/5200/7800, Lynx, SNES |
| `vectored` (later) | a 68000/ARM/MIPS image with a vector table or entry header | Genesis, Sega CD, 32X, Neo Geo, Jaguar, GBA, PS1, N64, Virtual Boy |
| `executable` (later) | a loader-format executable (ELF, 3DSX, NSO, SELF, XBE, DOL) with sections | PS2, PSP, PS3, Vita, 3DS, DS, Switch, GameCube/Wii/Wii U, Dreamcast, Saturn |

The CPU is not the shape: a shape is written once over `image_writer`'s `CodeModel` / `Listing` and given the processor through its
source module (`z80_source`, `described_source` with a `processors::Processor`, `m6502_source`, `risc_source`).

## What a description says

```json
{
  "platform": "coleco",                    // the knowledge base's platform name (config/knowledge/coleco)
  "title": "ColecoVision cartridge",
  "shape": "z80-block",
  "dialect": "sjasmplus",                  // config/dialects/sjasmplus.json
  "ports": "ports",                        // knowledge hardware section named for IN/OUT ports
  "fixed": "bios",                         // knowledge table of fixed addresses outside the image (BIOS entry points, system RAM)
  "head": [                                // the head comment, one template per line
    "ColecoVision cartridge {file}; {kb} KB at $8000; {title_screen?$AA55: the BIOS shows its title screen first|$55AA: the game starts at once}",
    "sprite names ${sprite_names:04X} ...",
    {"line": "title: {title}", "when": "title"},
    {"line": "disk file: {it}", "each": "files"}
  ]
}
```

Templates: `{name}` a value, `{name:04X}` a number in `std::format`'s spec, `{name?yes|no}` a choice on whether the value is set
(non-zero, non-empty); the branches are templates themselves. A line object may say `when` (written only when that value is set) or
`each` (once per element of a list, the element as `{it}`). The writer gives every program `file` (the file's name), `size` and `kb`
(the image's), `origin`; the loader gives the header's values by name.

The `banked` shape adds (one per platform, all data): `bank_size`, `power_on_banks`, the dialect `blocks` that open a bank (first and
rest), `header_fields` `[{at, size, what}]` (written as data rows with their names; the `.fmt` descriptions in `config/formats` say
where the fields are, the description says what each is called in the source), `vectors` `[{at, name}]` (a function at a vector is
named for it), `data_floor` (below it an operand is not a data pointer), the hardware tables as `{section, digits, as: port|constant}`
and the build (`assembler`, the extension of the source, what the built image is compared with).

## What stays code

* The loaders: reading a file is the format's (`formats/<x>`); the loader only says which bytes are the image, where it sits, the entry
  points and the header's values. A dozen lines each.
* A rule that really is the hardware's and appears once (the SG-1000 has no memory control port; a Game Boy write into ROM space goes to
  the bank controller) is a named rule the shape offers and a description switches on, not a branch on the platform's name.
* DOS, the drivers, NE, LE, PE, x64, Java and the class writers are not console writers and keep their code.

## Moving a writer

One at a time, smallest first. Before the move the writer's output on every sample (its tests' programs and the homebrew samples,
`source.writers.homebrew.*`, run with CB_HOMEBREW_SAMPLES so none is skipped) is kept; after it the same files must come out byte for
byte, and the area's tests (`source.<x>_writer.*`, `engine.machine_<x>`) pass. The old writer's files are then deleted, its callers
call `console_writer::reconstruct`, and its tests move to `source/console_writer_test.cpp` or stay as `source.<x>_writer.*` over the
engine.

## Status

- 2026-11-27: design; `z80-block` shape and the description reader; ZX Spectrum, MSX, ColecoVision and Amstrad CPC moved.
- 2026-11-27: the descriptions move to `config/platforms/` (one file per platform). `banked` shape: Game Boy (SM83, RGBDS: the header
  fields, the interrupt vectors, a write into ROM space is the bank controller's) and Master System / Game Gear / SG-1000 (Z80, WLA-DX:
  the Game Gear's ports and the SG-1000's missing mapper as `variants` by system). Their goldens and the five homebrew samples are
  byte-identical.
