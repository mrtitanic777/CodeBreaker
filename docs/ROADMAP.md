# Roadmap

Where CodeBreaker goes after 1.0.0. Nine tracks, each worked to its finish line. Each track lists what "done"
means and its phases; the status line is updated as phases land.

## 1. Windows 95 in authored source, all of it

Every function of every rebuilt Windows 95 module comes back as source in the language it was written in: C where it
was compiled, assembly where it was written by hand, with no gated functions and no stand-ins, and the milestone
regression (all15, setup, install) still passes.

1. Audit: per module (the 15 MINI.CAB modules, the 7 Setup programs, the full-install set), the functions still gated,
   still assembly where they were compiled, or still stand-ins; ranked by what each needs.
2. Convert, module by module, smallest first; each step gated by `codebreaker regress all15`.
3. The Setup programs' C versions (today they crash): find why, fix, prove with `regress setup`.
4. Finish line: `regress install` on every module in its authored form.

## 2. New console lanes: MIPS and ARM

The same path as the 8- and 16-bit lanes: an instruction decoder and assembler of CodeBreaker's own, a file loader, a
model of the machine for the judge, and source that rebuilds byte for byte.

1. MIPS R3000, PlayStation: the PS-X EXE loader, the decoder, the assembler (its source rebuilds identical), the
   interpreter (functions run and are judged), open-source PS1 homebrew as the test set.
2. ARM7TDMI (ARM and Thumb), Game Boy Advance: the cartridge loader, decoder, assembler, interpreter, homebrew.
3. Then N64 (MIPS R4300, big-endian, 64-bit) and DS (ARM9 + ARM7), reusing the two.

## 3. Recompiled games as a product

A title goes from disc or package to a PC build that plays, in one command, with input, audio and graphics working.

1. A playability report per title: what the headless check sees (frames, unimplemented imports, the first stop).
2. Close the gaps the report names in the Xbox 360 runtime (native/x360), title by title, starting with Minecraft.
3. Packaging: the built game as a folder that runs anywhere (its assets, its runtime), and the drop-in flow.

## 4. The universal translator, every front end to every back end

1. C# to C++17 and C, judged by running both (as Java to C is today).
2. Python to C++17 on real programs, with its runtime (py.hpp) completed for what those programs use.
3. C to C++ (a modernising pass) and C++ to C where the program allows it.

## 5. Speed

1. Measure: where a sweep's time goes today, per stage.
2. The judge in parallel across functions and modules, within the machine's limits.
3. Measure only what changed: the incremental engine re-judges a function only when its source or its inputs changed.

## 6. A plugin SDK for lanes

1. Document the lane interface as it is (engine/lane_kit, platforms/): what a platform provides.
2. A lane template and a worked example (a small CPU, end to end).
3. Lanes loadable without editing the core: a registration point, and the docs to add one.

## 7. Names, explained, and still proven

1. A naming pass: for each unnamed function, a proposed name and a one-line description from its code, its calls and
   its data (through the MCP tools), stored in the knowledge base with where it came from.
2. Every rename and comment is checked to change nothing the judge sees; names propagate corpus-wide as today.
3. Review: names below a confidence are shown for a person to accept.

Status: the store, the commands, the rules and the invariance proof are in ([NAMING.md](NAMING.md)); the assistant that
reads the evidence packets and proposes names for the other 95% is the next step.

## 8. CodeBreaker on Linux

`codebreaker` builds and runs on Linux (x86-64; GCC and Clang) as it does on Windows: the same commands, the same
tests, the same results. About 87 source files call the Windows API directly today, 13 of them in `core`.

1. A portable layer in `native/cb/core`: processes and pipes, files and paths, file locks, loading the engine
   libraries, environment, console text, time; a Windows and a POSIX implementation behind one interface.
2. Every other area moved onto that layer (no Windows calls outside `core` and the Windows-only lanes' tools).
3. The engines (L2, formats, DOS, XEX, decompiler, NASM) and the third-party libraries (Unicorn, Capstone, Z3)
   built as Linux shared libraries; `tools/` gets shell equivalents of the build and gate scripts.
4. CI builds and tests on Ubuntu; the gate's command cases run on Linux where their tools exist there.
5. The lanes that drive Windows-only tools (period compilers under DOSBox-X, QEMU for the Windows 95 boot) work on
   Linux with those tools' Linux builds.

## 9. Every console

Every games console, from the first home consoles to today's: each one's programs come back as source (assembly where
they were written in assembly, C/C++ where they were compiled), rebuild, and are judged in a model of the machine.
A lane is a CPU family plus a machine model (docs/LANES.md), so each new CPU opens a group of consoles.

Done: NES, Master System / Game Gear, Game Boy / Color, Genesis, ZX Spectrum, MSX, PlayStation, original Xbox, Xbox 360.
Order, by consoles opened per lane:

1. Game Boy Advance (ARM7TDMI), then Nintendo DS / DSi (ARM9 + ARM7).
2. The 65xx family: SNES (65816 + SPC700), TurboGrafx-16 / PC Engine (HuC6280), Atari 2600 / 5200 / 7800 / Lynx.
3. The rest of the Z80 and 68000 machines: ColecoVision, SG-1000, Amstrad CPC, Neo Geo AES / CD, Sega CD, Neo Geo
   Pocket (TLCS-900).
4. Nintendo 64 (MIPS R4300 + RSP), reusing the PlayStation's MIPS core.
5. SuperH: Sega 32X and Saturn (SH-2), Dreamcast (SH-4).
6. PowerPC: GameCube (Gekko), Wii (Broadway), Wii U (Espresso), reusing the Xbox 360's PowerPC.
7. PlayStation 2 (R5900 Emotion Engine + IOP) and PSP (Allegrex).
8. The oddities: Virtual Boy (V810), WonderSwan (V30), 3DO (ARM60), Intellivision (CP1610), Channel F (F8), Vectrex
   (6809), Odyssey 2 (8048), Atari Jaguar (68000 + Tom/Jerry), CD-i (68070).
9. Modern: 3DS (ARM11), PS Vita, Switch (ARM64), PS3 (Cell PPU + SPU), PS4 / PS5 and Xbox One / Series (x86-64) —
   for programs one may lawfully take apart (homebrew, decrypted dumps of one's own).

Each lane is proven like the PlayStation's: open-source homebrew for that console rebuilds identical and its functions
are judged, recorded in docs/TESTED_SOFTWARE.md.

## Status

- 2026-11-14: roadmap set; tracks 1 (audit) and 2 (MIPS/PS1) started.
- 2026-11-14: track 2 phase 1 (MIPS R3000A, PlayStation) landed: PS-X EXE loader, decoder/disassembler (GNU as syntax, GTE decoded), own assembler, own interpreter (GTE registers and the division-free commands), source writer and the `ps1` lane; 58 open-source PS-X EXEs rebuild identical. Left: GTE RTPS/RTPT and the colour commands, PS-X EXEs inside disc images (BIN/CUE, ISO), then phase 2 (ARM7TDMI, Game Boy Advance).
