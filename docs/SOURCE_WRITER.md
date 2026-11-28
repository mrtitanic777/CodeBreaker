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
src/source/console_cart6502.cpp               the cart6502 shape (6502 cartridges: read and write register names, data at the end, a file header)
src/source/console_segmented.cpp              the segmented shape (ca65 segments an ld65 configuration lays out in file order)
src/source/console_pce.cpp, console_snes.cpp  the flow the segmented shape cannot do alone (a SegmentCode: bank mapping, register widths)
src/source/console_vectored.cpp               the vectored shape (a 68000 image from its vectors or header, each piece at an `.org`)
src/source/console_mips.cpp                   the vectored shape's MIPS processor (the R3000A and the R4300: delay slots, %hi/%lo pairs)
src/source/console_arm.cpp                    the vectored shape's ARM7TDMI processor (ARM and Thumb states, literal pools)
src/source/console_risc.hpp                   what the RISC processors write alike: data rows, a header's fields
src/source/console_v810.cpp                   the vectored shape's V810 processor (movhi pairs as hi()/lo())
src/source/console_executable.cpp             the executable shape (a loader-format executable, written by its container's engine)
src/source/console_ps3.cpp, console_gekko.cpp, console_vita.cpp, console_n3ds.cpp, console_switch.cpp, console_nds.cpp
                                              the executable shape's containers that are engines of their own (PS3; GameCube, Wii, Wii U;
                                              Vita; 3DS; Switch; DS); `mips-elf` and `sh2` are source/mips_elf_writer and source/sh2_writer
src/source/console_segacd.cpp, console_sega32x.cpp, console_threedo.cpp, console_cdi.cpp, console_orbis.cpp, console_xboxone.cpp
                                              the executable shape's other containers (Sega CD; 32X; 3DO; CD-i; PS4 and PS5; Xbox One)
src/source/console_flat8.cpp, console_tlcs900.cpp, console_m6809.cpp, console_cp1610.cpp, console_v30.cpp
                                              the flat shape's processors (8048 and F8 in one engine; TLCS-900/H; 6809; CP1610; V30MZ)
src/source/console_platforms.cpp              per-platform code where it is truly needed: the loader (file -> image + values)
```

`console_writer::reconstruct("coleco", file, folder)` reads the description, asks the platform's loader for the program (its image, its
origin, its entry points and the named values of its header), and writes it through the **shape** the description names. A shape is
one way a program sits in memory, shared by every platform that sits that way:

| shape | how the image lies | platforms (today's writers) |
|---|---|---|
| `z80-block` | one block at one address, flow from the entries | ZX Spectrum, MSX, ColecoVision, Amstrad CPC (and the Neo Geo's sound CPU) |
| `banked` | fixed-size banks, the first N mapped at power-on, a header range that is data, vectors at fixed places | Game Boy, Master System / Game Gear / SG-1000 |
| `cart6502` | a 6502 cartridge's banks, each seen in a window; its last bytes data; the chips' read and write names | Atari 2600, 5200, 7800 |
| `segmented` | the file cut into pieces, each a ca65 segment in an ld65 memory area: code linked where it is seen, the rest bytes by role | NES, Atari Lynx, PC Engine, SNES |
| `vectored` | a 68000 image with a vector table or entry header, its pieces each at an `.org` | Genesis (and the 32X's 68000 side), Jaguar, Neo Geo, PlayStation, Nintendo 64, Game Boy Advance, Virtual Boy |
| `flat` | a cartridge ROM written whole by its processor's engine (`flat.cpu`) | Odyssey 2, Channel F, Neo Geo Pocket, Vectrex, Intellivision, WonderSwan |
| `executable` | a loader-format executable (ELF, 3DSX, NSO, SELF, DOL, PE, a disc's boot program) with sections, written by its container's engine | PS2, PSP, PS3, PS4, PS5, Vita, 3DS, DS, Switch, GameCube, Wii, Wii U, Xbox One, Saturn, Dreamcast, Sega CD, 32X, 3DO, CD-i |

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

The `segmented` shape adds: `cpu` (`6502`, `65sc02`), the L2 `assembler`, the `built` file's name (a template), `prologue` lines, `whole`, the
knowledge section of the `registers`, `data_floor`, a `tail` (`{at, size, lines}`: the vectors) and the `roles` a loader gives its pieces
(`{as: rows|row|incbin, file, note}`). The loader cuts the file into pieces (`Piece`: segment, memory area, role, offset, size, the address code is
linked at) and says which image offset each address shows (`offset_of`): an instruction or a label is written only in the piece the address shows.
Where flow keeps a state the plain flow does not, the loader gives a `SegmentCode` (console_writer.hpp) that follows the code and writes the
code pieces, and may decide where each is linked: the PC Engine's flow tracks the MPRs (`LDA #n` / `TAM`) and links each bank at the page it
was first found mapped at; the SNES's tracks the register widths and emulation mode (`REP`, `SEP`, `CLC; XCE`) and writes `.a16` / `.i8`
where the assembler's widths must change. The shape keeps the rest: the head, the names, the other pieces' roles (a copier header's rows,
`what` and `neutral`), the ld65 configuration (`address_digits`) and the build.

The `vectored` shape adds: `cpu` (`m68k`), `immediate_floor` (a long immediate below it names data only when it is code),
`calls_start_functions`, `org_digits`, `blank_after_piece`, `neutral_rows`, the header `fields` (`{at, size, what, as: text|rows|long|pointer,
when}`) and, for a `vectors` piece, the loader's list of what each long is (`vector_names`) and how many lead longs stay numbers. The loader
gives the pieces (each with or without an `.org`), the code ranges, the entries by name, and, where the file is not the assembled image
(a Jaguar COFF), how the image is put back into it (`rebuilt`). For MIPS (`cpu`: `mips`, `r4300`) the header `fields` say each field's
kind (`ascii`, `zeros`, `word`, `address`, `bytes`, `byte`, `items`), and the description gives the word order (`endian`), the `prologue`,
which view of a hardware register is bare (`bare_region`, `other_suffix`), the RAM and other ranges that are variables, and `built_what`;
the loader may name the program (`file`, `stem`: a disc's boot file) and give what the rebuilt file is compared with (`original`: the
N64's file order).

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
- 2026-11-27: `cart6502` shape (a 6502 cartridge of banks seen in a window, flow addresses bank << 16 | address, its last bytes data as
  templates, each chip's read and write register names): Atari 2600 and 5200 moved, their platform facts into their files too. The 5200
  writes its source in the neutral text and says it in DASM at the end (`whole`); the 2600 says each line as it goes. The two are not
  the same text (data rows break differently around strings), so `whole` stays a description field until one way is chosen.
- 2026-11-27: the Atari 7800 on `cart6502` (the A78 header written as data ahead of the banks, a SuperGame board's banks with the last
  fixed at $C000, the vectors in the last bank only); the loader says where a branch goes and which operand is an address of the program.
- 2026-11-28: `segmented` shape (ca65 segments, the ld65 configuration written beside the source): the NES (the header and a trainer one row
  each, the PRG a segment a bank, the CHR beside the source as `.incbin`, the vectors as words) and the Atari Lynx (the whole file in order, the
  stretches the loaders copy linked at their RAM addresses, the encrypted first stage and the headers noted). The Lynx's boot ROM is the
  emulator's: the caller plays it and hands the program to `load_lynx`. Their machine facts moved into their files.
- 2026-11-28: the PC Engine and the SNES on `segmented`, each with its loader's `SegmentCode` (console_pce.cpp, console_snes.cpp). The
  source, the linker configuration, the functions and the code bytes are byte-identical to the old writers' on the 5 PC Engine and 67 SNES
  homebrew programs here and on their tests' programs.
- 2026-11-28: `vectored` shape (68000): the Genesis (the vector table, the header's fields; the 32X writes its 68000 side through
  `load_genesis`) and the Jaguar (a cartridge's boot signature and header, a COFF's sections put back into the file). Byte-identical on the
  Genesis, Jaguar, 32X and Sega CD homebrew programs and the Genesis golden.
- 2026-11-28: the vectored shape's MIPS processor (console_mips.cpp): the PlayStation and the Nintendo 64, their two near-duplicate
  writers one; byte-identical on all 58 PlayStation and 79 Nintendo 64 homebrew programs.
- 2026-11-28: the vectored shape's ARM processor (console_arm.cpp): the Game Boy Advance, its header as fields (`comment`, `byte_rows`,
  `padded`, `byte`, `zeros`); the data rows and header fields the RISC processors share (console_risc.hpp). Byte-identical on all 41 GBA
  homebrew programs, and the PlayStation's and Nintendo 64's 137 again.
- 2026-11-28: the Neo Geo on the vectored shape (68000): the header's `ascii` and `number` fields, the second megabyte as a piece at the bank
  window, the function starts the loader gives apart from flow's starts, `equ_width`; a loader's `after` (what is still to do once the program
  is written) writes a .neo container's Z80 sound program through the `neogeo_sound` platform (`z80-block`, the M-ROM's own description).
  Byte-identical on the two P-ROMs and the M-ROM here and the Neo Geo golden.
- 2026-11-28: the vectored shape's V810 processor (console_v810.cpp): the Virtual Boy, its header's fields relative to the loader's
  `header_at`. Byte-identical on its 4 homebrew programs. The `vectored` row of the table is done; next the `executable` shape.
- 2026-11-28: `executable` shape (console_executable.cpp): the container's engine (`mips-elf`: source/mips_elf_writer) given the console's
  facts as data -- the processor and its variant, what the program is (a template), the rebuilt file's extension, the views of memory
  (`views`: a mask or an offset to the physical address) and the RAM ranges; the loader gives the ELF, the names the container gives
  (a PSP module's exports and imports' stubs) and the format's hardware table. The PlayStation 2 and the PSP moved; byte-identical on
  their 15 homebrew programs.
- 2026-11-28: the rest of the `executable` shape. Container `sh2` (source/sh2_writer): the Saturn and the Dreamcast, a disc's IP.BIN and
  first program in one source; the description holds the header's fields (`text` or a big-endian `long`), the RAM ranges (`ram`, less where
  the loader says the program is loaded: `own`), the knowledge section of the registers (with the SH7604's own: `chip_registers`), `sh4`, the
  rebuilt file's name (`built`) and the head; the loader gives the parts, the entries and their names, and `after` (the disc the lane runs,
  the Dreamcast's program scrambled again: console_writer::dreamcast_disc). The writers that were engines of their own moved whole into
  the console writer as containers (`ps3`, `gekko` for the GameCube, Wii and Wii U, `vita`, `n3ds`, `switch`, `nds`): their heads, the
  lines after them (`prologue`) and the rebuilt file's name are the description's, a head may name what flow found (`functions`, the DS's
  `arm9_functions`), the loader gives the program and the values. Byte-identical (source, functions, code bytes, rebuild) on the 34
  homebrew programs of those consoles, the same refusals on the 5 they do not take.
- 2026-11-28: the last twelve, and item 7 is done. `flat` (console_executable.cpp: write_flat) is a cartridge ROM written whole by its
  processor's engine, the description's `flat` section naming it (`cpu`) with the head and the rebuilt file's name; the Odyssey 2's 8048
  and the Channel F's F8 are one engine (console_flat8.cpp, the BIOS's routines from the description's `fixed`). The Sega CD, the 32X, the
  3DO, the CD-i, the PS4/PS5 and the Xbox One are containers of the executable shape. A loader hands a container its format's program in
  `Contained::program` (std::any; console_shapes.hpp: contained_as<T>). Byte-identical on 48 dumps of homebrew programs and the writers'
  own synthetic images.
