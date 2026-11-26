# Roadmap

Where CodeBreaker goes after 1.0.0. Ten tracks, each worked to its finish line. Each track lists what "done"
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
2. Close the gaps the report names in the Xbox 360 runtime (src/recompile/x360), title by title, starting with Minecraft.
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

Status: the store, the commands, the rules and the invariance proof are in ([NAMING.md](NAMING.md)). 2026-11-26 (phase 3):
the callback-role rule (window, dialog, thread, hook, timer and enum procedures, by window class and dialog title), the
assistant pass proven end to end on Notepad (every function named, the judge's verdicts and rows identical), and review
(`names MODULE --review`, `--accept-sure`, the Proposed names tool). Left: parameter and local names, body digests for
the other lanes, a ground truth with private names.

## 8. CodeBreaker on Linux

`codebreaker` builds and runs on Linux (x86-64; GCC and Clang) as it does on Windows: the same commands, the same
tests, the same results. About 87 source files call the Windows API directly today, 13 of them in `core`.

1. A portable layer in `src/core`: processes and pipes, files and paths, file locks, loading the engine
   libraries, environment, console text, time; a Windows and a POSIX implementation behind one interface.
2. Every other area moved onto that layer (no Windows calls outside `core` and the Windows-only lanes' tools).
3. The engines (L2, formats, DOS, XEX, decompiler) and the third-party libraries (Unicorn, Capstone, Z3)
   built as Linux shared libraries; `tools/` gets shell equivalents of the build and gate scripts.
4. CI builds and tests on Ubuntu; the gate's command cases run on Linux where their tools exist there.
5. The lanes that drive Windows-only tools (the period compilers on the built-in DOS executive, QEMU for the Windows 95 boot) work on
   Linux with those tools' Linux builds.

## 9. Every console

Every games console, from the first home consoles to today's: each one's programs come back as source (assembly where
they were written in assembly, C/C++ where they were compiled), rebuild, and are judged in a model of the machine.
A lane is a CPU family plus a machine model (docs/LANES.md), so each new CPU opens a group of consoles.

Done: NES, Master System / Game Gear, Game Boy / Color, Genesis, Neo Geo (AES / MVS), Sega CD, ZX Spectrum, MSX, PlayStation, Nintendo 64 (R4300i; the RSP and RDP are not run), Sega 32X, Sega Saturn, GameCube, Wii and Wii U (the PowerPC 750 family; the hardware is named and logged, not emulated), original Xbox, Xbox 360.
Order, by consoles opened per lane:

1. Game Boy Advance (ARM7TDMI), then Nintendo DS / DSi (ARM9 + ARM7).
2. The 65xx family: SNES (65816; the SPC700 sound program is still data) and TurboGrafx-16 / PC Engine (HuC6280) are done, with Atari 2600 / 5200 / 7800 / Lynx.
3. The rest of the Z80 and 68000 machines: ColecoVision, SG-1000, Amstrad CPC, Neo Geo CD, Neo Geo
   Pocket (TLCS-900).
PS2 and PSP (track 9): done 2026-11-15, see docs/LANES.md (the R5900 and Allegrex as variants of the shared MIPS decoder, `emulate/mips_le`, the ELF writer).
4. Nintendo 64 (MIPS R4300 + RSP), reusing the PlayStation's MIPS core. Done 2026-11-15: the CPU, ROM loader, source writer and `n64` lane; the RSP (its microcode as a second MIPS-like program) and the RDP are the open part.
5. SuperH: Sega 32X and Saturn (SH-2) and the Dreamcast (SH-4) done; see the 2026-11-15 Dreamcast entry.
6. PowerPC: GameCube (Gekko), Wii (Broadway), Wii U (Espresso), reusing the Xbox 360's PowerPC. Done 2026-11-15: see the Status entry.
7. PlayStation 2 (R5900 Emotion Engine + IOP) and PSP (Allegrex).
8. The oddities: Virtual Boy (V810, done 2026-11-15), Intellivision (CP1610, done 2026-11-15), Channel F (F8), Odyssey 2 (8048), (Vectrex, 6809, is done), Neo Geo Pocket (TLCS-900/H, done 2026-11-15),
   CD-i (68070) is done too. WonderSwan (V30), 3DO (ARM60), Atari Jaguar (68000 + Tom/Jerry) and Philips CD-i are done (2026-11-15).
9. Modern: 3DS (ARM11), PS Vita, Switch (ARM64), PS3 (Cell PPU + SPU), PS4 / PS5 and Xbox One / Series (x86-64) —
   for programs one may lawfully take apart (homebrew, decrypted dumps of one's own).

   Done (2026-11-15): Nintendo 3DS (ARM11: `.3dsx` and devkitARM ELF) PS Vita (Cortex-A9: unencrypted SELF and ELF) and PS3 (Cell PPU: ELF64 and unencrypted SELF, proved on test-built ELFs).

   Done (2026-11-15): PlayStation 4 and PlayStation 5 (x86-64: ELF, OpenOrbis fake SELF; real GPL homebrew rebuilds identical and is judged) and Xbox One / Series (the PE32+ of a dev-mode UWP package; a real GPL app rebuilds identical and is judged).

   Done (2026-11-15): Nintendo Switch (Cortex-A57: AArch64 core with its own decoder, assembler and interpreter; homebrew `.nro`, `.nso` and ELF; `reach --judge` verifies every function of three real NROs).

Each lane is proven like the PlayStation's: open-source homebrew for that console rebuilds identical and its functions
are judged, recorded in docs/TESTED_SOFTWARE.md.

## 10. .NET to C# with a decompiler of its own

A .NET program (any PE with a CLR header, 32- or 64-bit) comes back as C# from CodeBreaker's own decompiler: ILSpy's
engine (ICSharpCode.Decompiler v11.0, MIT) ported to C++ in `src/dotnet`, with an ECMA-335 metadata reader of its
own, so no installed `ilspycmd` and no .NET are needed. Plan, stages and sizes: docs/DOTNET_DECOMPILER_PLAN.md.

1. The metadata reader and ILSpy's metadata layer; then the IL disassembler, matching `ilspycmd -il` byte for byte.
2. The type system, the ILAst, control flow and the IL transforms, each matching ILSpy's own dumps.
3. The C# syntax tree, resolver, builders and transforms: `ilspycmd`'s C# byte for byte, then its projects.
4. Finish line: the Convert route `dotnet` and the XNA lane use it; tested programs in docs/TESTED_SOFTWARE.md.

## Status

- 2026-11-26: track 10 set (the ILSpy port, docs/DOTNET_DECOMPILER_PLAN.md); upstream v11.0 fetched for reference.
- 2026-11-26: track 10 stage 0: `dotnet/metadata`, CodeBreaker's own ECMA-335 reader (System.Reflection.Metadata's interface: row views, heaps, signatures, custom attribute values, method bodies); its dump matches .NET's own reader line for line on the test assemblies (PE32 and PE32+) and on System.Private.CoreLib 9.0, .NET Framework 2.0's mscorlib and System, WinForms 4 and XNA 3.1's mixed-mode Framework. Not yet tested: a #- (uncompressed) tables stream, which needs a hand-made input.
- 2026-11-26: track 10 stages 1 and 2: ILSpy's metadata layer (MetadataFile, PEFile, MetadataExtensions, SRMExtensions, assembly references) and IL disassembler (ReflectionDisassembler, MethodBodyDisassembler, ILStructure, the signature provider) ported to `dotnet/decompiler`. `show_il` writes what `ilspycmd -il` writes, byte for byte, for the test assemblies and for System.Linq, System.Text.Json, System.Private.Xml and System.Private.CoreLib 9 (1.39 million lines), .NET Framework 2.0's mscorlib, WinForms 4 and XNA 3.1's Framework and Framework.Game; CoreLib in 0.9 s (ilspycmd: 8.5 s). Not yet ported: the universal assembly resolver (needed only when attribute blobs are decoded) and debug information (sequence points).
- 2026-11-15: PowerPC 750 consoles landed: the GameCube (Gekko), Wii (Broadway) and Wii U (Espresso) as three lanes (registered through engine/machine_lanes) on `decompile/ppc`, `formats/{dol,ppc_elf,ppc_program,gekko_map}`, `source/gekko_writer` and the Xbox 360's PowerPC interpreter with paired singles, GQRs and logged hardware; homebrew DOLs, ELFs and RPXs rebuild identical. Left: the hardware is named and logged, not emulated (graphics, DSP, disc, controllers); interrupts and exceptions are not delivered; many functions stop as unsupported at a privileged or unmodelled instruction; the Wii U's imports (CafeOS libraries) are code stubs, not named; GameCube and Wii discs (.iso, .rvz, .wbfs) and Wii U packages are not read; relocations of RPL libraries are not applied.
- 2026-11-15: Nintendo 64 (R4300i) landed on the PlayStation's MIPS core: `.z64` / `.v64` / `.n64` loader (header, IPL3 as data, CIC, checksums, the load address), the R4300 variant of the MIPS decoder, assembler and (as `emulate/r4300`) interpreter, the `n64` writer and lane (registered through engine/machine_lanes); 79 open-source ROMs rebuild identical. Left: the RSP (microcode in IMEM, vector unit) and the RDP are not run (what is written to their registers and to DMEM is logged); interrupts and exceptions are not delivered; the boot code (IPL3) is data, not a program in its own right; ROMs larger than the first megabyte are data past it, so a program that loads code from the cartridge by PI DMA has that code as data.
- 2026-11-14: roadmap set; tracks 1 (audit) and 2 (MIPS/PS1) started.
- 2026-11-15: track 2 phase 3 (Nintendo DS, ARM946E-S + ARM7TDMI) landed: `.nds` loader, the ARMv5TE additions to the ARM decoder/assembler/interpreter, the DS memory maps and CP15, one source for both processors with the file system as data, and the `nds` lane (registered through engine/machine_lanes). Left: bank-switched video memory, the hardware divider and square-root unit, the 3D engine and the wireless registers are not modelled; overlays and compressed ARM9 binaries are data; DSi (ARM9 with ARMv5 extras, `.dsi`) not read.
- 2026-11-15: track 2 phase 2 (ARM7TDMI, Game Boy Advance) landed: `.gba` loader, ARM and Thumb decoder/disassembler and assembler (GNU as `.syntax unified`), own interpreter, source writer and the `gba` lane; see docs/TESTED_SOFTWARE.md for the ROMs that rebuild identical. Left: literal-pool assembly (`ldr r0, =x`), multiboot ROMs loaded to EWRAM, Thumb switch tables, interrupt delivery in the interpreter, then N64 and DS.
- 2026-11-14: track 2 phase 1 (MIPS R3000A, PlayStation) landed: PS-X EXE loader, decoder/disassembler (GNU as syntax, GTE decoded), own assembler, own interpreter (GTE registers and the division-free commands), source writer and the `ps1` lane; 58 open-source PS-X EXEs rebuild identical. Left: GTE RTPS/RTPT and the colour commands, PS-X EXEs inside disc images (BIN/CUE, ISO), then phase 2 (ARM7TDMI, Game Boy Advance).
- 2026-11-15: track 9 item 5 (SuperH) part 1: the SH-2 landed: decoder/assembler/interpreter of its own, the Sega 32X lane (`.32x`: the SH-2 program at $06000000 as GNU as source beside the Genesis writer's 68000 source) and the Sega Saturn lane (`.iso` / `.bin` + `.cue`: IP.BIN and the first program as one source), registered through engine/machine_sh2; 32X and Saturn homebrew rebuilds identical and is judged. Left: Dreamcast (SH-4: the FPU, pair moves, FTRV/FIPR, `.cdi`/`.gdi`/1ST_READ.BIN at $8C010000, Holly and AICA registers); the SH-2 interpreter does not model the 32X's PWM or DREQ FIFO, the Saturn's SCU DSP or DMA, or interrupts; jump tables built with `mova` + `braf` are not followed (their targets are found only as pointers), and Saturn/32X programs that are mostly data come back mostly as data.
- 2026-11-15: track 9 item 5 (SuperH) part 2: the Dreamcast (SH-4): the SH-4 is a variant of the SH-2 core (little-endian words, the FPU, SHAD/SHLD, PREF and the cache/TLB instructions, the extra control registers, banked R0-R7; every decodable word assembles back to itself), `formats/dreamcast` (`.gdi`, `.cdi`, `.iso`/`.bin`; IP.BIN field by field, 1ST_READ.BIN at $8C010000, CD-R scramble detected and undone), `source/dreamcast_writer` (GNU as source rebuilt byte-identical; the lane's disc scrambles the program again) and the `dreamcast` lane (Holly, Maple, G1/GD-ROM, G2, PowerVR2 and AICA registers named and logged, BIOS system calls stubbed and logged). Left: the PowerVR's tile accelerator, the AICA's ARM7 sound program and DSP, the GD-ROM command set, the MMU and caches as behaviour (they are logged state), FPU exception flags and rounding modes, interrupts and DMA; a `.cdi`'s descriptor is not read (its boot track is found by content), so an image whose boot track is not a plain ISO volume is not read; the SH-4's 64-bit double moves in big-endian mode (the Dreamcast does not use it) are not modelled.
- 2026-11-15: track 9 items 2 and 8 (part): four more consoles on cores CodeBreaker already had: the Atari 5200 (the 6502 core: raw `.a52` / `.bin`, GTIA / ANTIC / POKEY named and logged, dasm source), the Atari Jaguar (the 68000 core: `.j64` cartridges at $800000 and COFF executables, Tom and Jerry named and logged, the GPU and DSP programs stay data), the WonderSwan and WonderSwan Color (V30MZ: Capstone's 16-bit x86 for decoding, Unicorn's 16-bit x86 for running, NASM source found out in rounds; the I/O ports named) and the 3DO (ARM60: the ARM tables and interpreter, with a big-endian bus; the Opera filesystem's LaunchMe, an AIF). 5200 and Jaguar homebrew rebuild identical and are judged (CC0 and public-domain sources); no licensed WonderSwan or 3DO binary was found, so those two are tested on cartridges and discs the tests build. Left: Philips CD-i (68070: the CD-i disc and CD-RTOS modules); the Jaguar's GPU / DSP RISC cores and 6 MB banking; the 5200's banked 40 KB cartridge and a real BIOS; the WonderSwan's bank registers ($C0-$C3, ROMs over 896 KB), interrupts, sound and video, and far calls into other segments; the 3DO's 26-bit mode (flags in the pc), the folios as a model (the SWI numbers are logged only) and the Madam / Clio hardware beyond the named registers.
- 2026-11-15: track 9 item 8: the Philips CD-i (68070 = the 68000 core): `formats/cdi` finds the CD-RTOS (OS-9/68000) modules of a disc image by their header parity at sector starts (or reads a module file), `source/cdi_writer` writes each module's header, code and data as Motorola-syntax source with `TRAP #0` + call word as the system call by name, and the `cdi` lane runs functions with each call stubbed and logged. A real module (CDI_FROG, MIT) rebuilds identical. Left: the disc's filesystem (path table, directories) and the file-to-module mapping; CD-RTOS as a model (calls are only logged); the video chips' registers beyond a name or two; the rest of this batch (Vectrex, Virtual Boy and PC-FX, Intellivision, Neo Geo Pocket, Odyssey 2 and Channel F) each need a new CPU core and have not been started.
- 2026-11-15: track 9 item 8: the Vectrex (Motorola 6809, a new core): `decompile/m6809` (every opcode of the three pages: the decoder's text assembles back to the same bytes for every encoding, 59,000 checks), `build/m6809_asm`, `emulate/m6809`, `formats/vectrex`, `source/vectrex_writer` and the `vectrex` lane (VIA registers and BIOS entries named; the BIOS stays stubs). E-SRG and Joakim Larsson's tutorial cartridges rebuild identical and are judged. Left: banked cartridges (over 32 KB), the VIA's timers and shift register as behaviour, interrupts (SWI, CWAI and SYNC stop a run), the BIOS as a model (calls are only logged), the sound chip (AY-3-8912 behind the VIA), and a lwasm check of the written source.
- 2026-11-15: track 9 item 8: the Virtual Boy (NEC V810, a new core) and the Intellivision (GI CP1610, a new core): `decompile/v810` + `build/v810_asm` + `emulate/v810` + `formats/vb` + `source/vb_writer` + the `vb` lane; `decompile/cp1610` + `build/cp1610_asm` + `emulate/cp1610` + `formats/intv` + `source/intv_writer` + the `intv` lane. Four VUEngine demos and Space Versus rebuild identical and verify (`reach --judge`: 522 and 54 functions, 100%). PC-FX (same V810 core) not done: no licensed homebrew found. Neo Geo Pocket (TLCS-900/H), Odyssey 2 and Channel F next.
- 2026-11-15: track 9 item 8: Neo Geo Pocket, first step only: `formats/ngp` reads the `.ngp` / `.ngc` / `.npc` header (licence text, start address, catalogue, colour flag, title) and names the K1GE, timer and BIOS-variable registers; `info` recognises a cartridge (reported as a gap). The TLCS-900/H core (decoder, assembler, interpreter: a variable-length prefix-based ISA, the largest of the track-9 cores so far), the writer and the lane are not written. Toshiba reference disassembler (MIT): Tixul/NgpCraft_Disasm.
- 2026-11-15: track 9 item 8: Neo Geo Pocket finished: the TLCS-900/H core (`decompile/tlcs900` one table read three ways, `build/tlcs900_asm`, `emulate/tlcs900`), `source/ngp_writer`, the `ngp` lane. ASTEROIDS (MIT) rebuilds identical, 60 functions verified, all 12 judged diverge when their first byte is turned over. Left: the timers, interrupts and DMA as behaviour (a handler runs only when called), the Z80 sound CPU (its RAM is memory), the BIOS as a model (calls are logged), pointer tables of functions in data, `.ngc` files with a bank-switched second ROM chip at $800000.
- 2026-11-15: track 9 item 8: the Odyssey 2 (Intel 8048) and the Channel F (Fairchild F8), two more new cores: `decompile/i8048` + `emulate/i8048` + `formats/o2` + `source/o2_writer` + the `o2` lane; `decompile/f8` + `emulate/f8` + `formats/chf` + `source/chf_writer` + the `chf` lane; `build/byte_asm` is the shared directive layer of their assemblers. Hand-made cartridges rebuild identical and verify (`reach --judge`); no homebrew yet. Only the TLCS-900/H core of Neo Geo Pocket is left of item 8.
