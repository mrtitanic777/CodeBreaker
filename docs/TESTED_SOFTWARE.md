# Software CodeBreaker has been tested on

A running list: every real program put through a CodeBreaker lane, what was done with it and
what came out. Add a line whenever a new program goes through; change a line when a result
changes. Hand-written test programs (tests/data/) are not listed here: this is real software.

Results, for the source lanes: **rebuilds identical** = the source written from the program
assembles or compiles back into it byte for byte; **verified** = `measure` judged every
function equivalent (for an identical rebuild that is the rebuild itself; edited source is
judged function by function in the platform's model). For recompiled titles: **plays** (seen
playing), **runs** (runs headless for a minute without stopping; screens not all checked),
**stops at** (the first thing the runtime does not do yet).

## 8-bit and 16-bit homebrew (source lanes)

Open-source homebrew with published source, downloaded 2026-10-23 from the authors' own
repositories (kept outside the repository, in `GitHub/homebrew-samples/`).

| Program | Platform | Author / source | License | Functions | Result |
|---|---|---|---|---|---|
| HelloWorld | Master System | PeterLemon/SMS (VDP/HelloWorld) | none stated | 3 | rebuilds identical, verified |
| Sprite | Master System | PeterLemon/SMS (VDP/Sprite) | none stated | 3 | rebuilds identical |
| SMSAudio | Master System | PeterLemon/SMS (PSG/SMSAudio) | none stated | 3 | rebuilds identical, verified |
| µCity 1.3 | Game Boy Color | AntonioND/ucity | GPL-3.0 | 25 | rebuilds identical, verified |
| 2048 | Genesis / Mega Drive | atamurad/sega-2048 | MIT | 38 | rebuilds identical, verified (found: indexed addressing with a long index register) |
| Retail Clerk '89 | Genesis / Mega Drive | huguesjohnson/RetailClerk89 (v1.0.1) | see its LICENSE | 82 | rebuilds identical, verified |
| World Rally (demo) | MSX (48 KB) | theNestruo/msx-wrally | GPL-3.0 | 358 | rebuilds identical, verified (found: header at $4000, lone DD/FD prefixes, NEG mirrors) |
| nohzdyve | MSX (8 KB) | plainspooky/nohzdyve (v1.0) | see its repository | 99 | rebuilds identical, verified |
| 240p Test Suite | ColecoVision (32 KB) | nanochess/240pTestSuite_colecovision (suitecv.rom) | GPL-2.0 | 74 | rebuilds identical, verified |
| 240p Test Suite | SG-1000 (32 KB, .sg) | nanochess/240pTestSuite_colecovision (suitesg.rom) | GPL-2.0 | 63 | rebuilds identical, verified (the Master System lane, no mapper) |
| brk, demo01, demo02, irq_test, raw (BLL demos) | Atari Lynx (BS93 objects) | 42Bastian/new_bll (demos/) | Apache-2.0 | 2, 3, 11, 2, 19 | rebuilds identical, verified |
| boing, fire, 256col, drawtest, mines2 (BLL demos) | Atari Lynx (BS93 objects) | 42Bastian/new_bll (demos/) | Apache-2.0 | 17, 22, 16, 14, 27 | rebuilds identical, verified (65SC02: BRA, STZ, TRB/TSB, INC A; Suzy and Mikey registers by name) |
| Overlay Test (overlay.lnx) | Atari Lynx (.lnx cartridge) | 42Bastian/new_bll (demos/overlay) | Apache-2.0 | 16 | rebuilds identical, verified (the first stage decrypted, the loaders run to find the 3 segments they copy) |
| new_bll.lnx (uBLL loader) | Atari Lynx (.lnx, 256 KB) | 42Bastian/new_bll (uBLL) | Apache-2.0 | 1 | rebuilds identical, verified |
| Refrain 0.0.2.43 (202-p1.p1, 1 MB P-ROM, GCC, words swapped) | Neo Geo | defalt45/refrain (builds/) | GPL-3.0 | 13 | rebuilds identical, verified (13 functions run in the 68000 with the Neo Geo's memory map and each differs when its first instruction is changed); most of the game is reached through function pointers (`JSR (A3)`), which the flow does not follow: 5.4 KB of 1 MB is code |
| DATlib demo (dev_p1.rom, 398 KB) | Neo Geo | kl3mousse/seafighter (include/DATlib/sample/DATdemo) | GPL-3.0 | 11 | rebuilds identical, verified |
| Mezz-Estate audio driver 1.1.1b (driver.m1, 24 KB Z80 M-ROM) | Neo Geo (sound) | neogeo-mzs/Mezz-Estate-NeoGeo-Audio-Driver (release) | MIT | 56 | rebuilds identical (as Z80 source with the YM2610 and 68000-link ports named; not judged: no Neo Geo sound model in the Z80 lanes) |
| minimalcd.iso (96 KB) | Sega CD | LukeUsher/ProjectCD (release minimal-cd) | BSD-2-Clause | 5 | rebuilds identical, verified (the system area: EUR security code, the IP that starts the sub CPU, an SP with an empty jump table) |
| bootsect.bin (32 KB system area) | Sega CD | LukeUsher/ProjectCD | BSD-2-Clause | 8 | rebuilds identical, verified |
| loader.lnx | Atari Lynx (.lnx, 256 KB) | 42Bastian/new_bll | Apache-2.0 | 0 | rebuilds identical (as data: its loaders end in the boot ROM at $FE4A, which is not modelled) |
| Kibalchish (NTSC, PAL, PAL60) | Atari 2600 (4 KB, dasm) | ivanivorontsov-sudo/atari-kibalchish (build/) | MIT | 16 each | rebuilds identical, verified (all 12 judged functions diverge when their first byte is turned over; TIA and RIOT registers named) |
| Legendary Spear (NTSC) | Atari 2600 (2 KB) | DChristianson/legendary-spear (v1.1 release) | MIT | 1 | rebuilds identical, verified (a 2 KB cartridge: the window at $F800, mirrors folded) |
| FlappyBoot (boot.bin) | Atari 2600 (4 KB) | cleak/FlappyBoot | Unlicense | (counted in the test output) | rebuilds identical, verified |
| Pipes 2600 (pipe2600.bin) | Atari 2600 (8 KB, F8, batari Basic) | albf/pipes-2600 | MIT | 2 (one reset per bank) | rebuilds identical, verified (code in the second bank is reached only through bank-switch trampolines, so most of the cartridge is data) |
| (none yet) | Atari 7800 | no open-source homebrew with a permissive licence and a published binary was found (Drone Patrol, Side Bounce and Black Forest state no licence; hotdog and cc7800 are GPL source only) | | | the lane is tested with synthetic A78 files built in the tests (a linear 16 KB ROM and a 128 KB SuperGame): rebuild identical, judged |
| 8K Copier (8KLEVELC.BIN, the best binary on the disk) | Amstrad CPC (.dsk, extended) | muckypaws/AmstradCPC (8KCopier.dsk) | LGPL-3.0 | 18 | rebuilds identical, verified |
| ROMDump (ROMDUMP.BIN) | Amstrad CPC (.dsk, standard) | llopis/amstrad-diagnostics (ROMDump.dsk) | MIT | 1 | rebuilds identical, verified (load address $0000) |
| Icarus | ZX Spectrum (tape) | MmmPT/icarus-zx-spectrum | see its ASSET-LICENSE | 95 | rebuilds identical, verified |
| PCE Mouse Test (pce_mouse.pce, 6 banks behind a 512-byte header) | PC Engine / TurboGrafx-16 | dshadoff/PCE_Mouse_Test (src/) | MIT | 66 | rebuilds identical, verified (12 judged functions run in the HuC6280 with the MPR map: 11 diverge when their first byte is turned over) |
| BALL, GAMELIFE, MML, SAMPLE1 (demo projects, 3 banks behind a 512-byte header each) | PC Engine / TurboGrafx-16 | langel/pce-dev-kit (demo projects/) | Unlicense | 25, 34, 24, 25 | rebuilds identical, verified (11 of 12 judged functions of each diverge when their first byte is turned over) |
| gothicvania.sfc (512 KB), hello-world-region.sfc (256 KB) | Super Nintendo (LoROM, PVSnesLib C) | donth77/snes-homebrew (release v1.0.0) | MIT | 80, 41 | rebuilds identical, verified (all 12 judged functions of each diverge when their first byte is turned over) |
| the 65 example cartridges of OpenSNES 0.55.0 (aim_target ... world_sprites: 256 KB LoROM, hirom_demo: 512 KB HiROM; some use SuperFX, SA-1, DSP-1, the Super Scope) | Super Nintendo (OpenSNES C) | k0b3n4irb/opensnes (release v0.55.0, opensnes-examples zip) | MIT | 2,220 in all (283,879 of 17,301,504 bytes as code: the ROMs are mostly padding, graphics and sound) | all 65 rebuild identical; 9 judged (simple_sprite 19 functions, controller 9, blocks 75, breakout 31, hirom_demo 24, mode7_racing 31, rpg 63, shmup 31, hdma_wave 33): 100 of the 105 judged functions diverge when their first byte is turned over. The coprocessors' code is data, as is the SPC700's |
| Curse of the Lost Miner (culomin5200.a52, `bin/main.c.rom`, 16 KB; cc65 C) | Atari 5200 | 8bitworkshop/culomin5200 | CC0-1.0 | 100 | rebuilds identical, verified (12 judged functions all diverge when their first byte is turned over; `reach --judge`: 100 of 100 verified) |
| jag_ball.cof (4 KB COFF) and jag_ball.j64 (1 MB cartridge) | Atari Jaguar | 42Bastian/JaguarDemos (jag_ball/) | Unlicense (public domain) | 20 each | rebuilds identical, verified (the sections' bytes / the cartridge; 12 judged functions, 12 and 10 diverge when their first byte is turned over; the GPU code is data) |
| yarc.cof (128 KB COFF) and yarc.j64 (1 MB cartridge) | Atari Jaguar | 42Bastian/JaguarDemos (yarc/) | Unlicense (public domain) | 8 each | rebuilds identical, verified (8 judged, 8 and 7 diverge when their first byte is turned over) |
| (none) | WonderSwan / WonderSwan Color | no open-source WonderSwan program with a stated licence and a published binary was found: Wondersnake (tslanina/Retro-WonderSwanColor-Wondersnake, GPL-3.0) is source only and needs Borland TASM; the Wonderful toolchain's examples (CC0) ship no ROMs | | | the lane is tested with cartridges built in the tests (the footer layout is checked against what Wondersnake's `com2ws` writes); the rebuilt source needs NASM |
| CDI_FROG (the CD-RTOS module of the game, 20,642 bytes, built from the C and assembly in `src/`) | Philips CD-i (a module file) | gameblabla/superquartet-cdi (src/Disk/) | MIT | 4 | rebuilds identical, verified (all 4 judged functions diverge when their first byte is turned over; `TRAP #0` system calls named and logged; the header read as the format says: sync, size, name, type, parity, execution offset) |
| E-SRG 1.1.1 (e-srg-1.1.1e.bin, e-srg-1.1.1v.bin; 32 KB each) | Vectrex | emmulator/vectrex-e-srg (v1.1.1 release) | GPL-3.0 | 45 each | rebuilds identical, verified (12 judged functions each, all diverge when their first byte is turned over; BIOS calls and VIA registers by name) |
| bouncer6.bin, dt2.bin (tutorial cartridges) | Vectrex | JoakimLarsson/VectrexTutorial (bin/) | GPL-3.0 | 7, 1 | rebuilds identical, verified (all judged functions diverge) |
| Capitan Sevilla 3D Public Demo.vb (2 MB), SPONG Demo (2019-06-20).vb (2 MB), Flash.Cart.Test.Program.HyperBoy.vb (512 KB), Multi Color Demo.vb (512 KB) | Virtual Boy | VUEngine release builds (the projects' released ROMs, as downloaded to homebrew-samples/vb; repository URLs still to be filled in) | MIT (licences checked when downloaded) | 167, 130, 121, 104 | rebuilds identical, verified (12 judged functions each, all diverge when their first byte is turned over; VIP, VSU and hardware registers by name) |
| spaceversus.rom (32 KB, three segments) | Intellivision | marcomarrero/SpaceVersus-IntyBASIC | MIT | 54 | rebuilds identical, verified (12 judged functions, all diverge when their first word is changed; STIC and PSG registers by name; the segment CRCs are the computed ones) |
| asteroids-neo.ngp (26 KB) | Neo Geo Pocket Color | underscore42/ngpc-asteroids-neo | MIT | 60 | rebuilds identical, verified (12 judged functions, all diverge when their first byte is turned over; hardware registers, BIOS variables and interrupt vectors by name; 4,434 instructions, none undecodable) |
| (hand-made test cartridges, tests/ only) | Odyssey 2, Channel F | written for the tests | - | 2, 2 | rebuild identical, verified; no open-source homebrew ROM for either console was found to add here |
| (none) | 3DO (Opera disc, AIF LaunchMe) | no open-source 3DO program with a stated licence and a published binary was found (trapexit/3do-devkit states none; gameblabla/exp3DOsdk's ISOs link the 3DO SDK's libraries, which are not under its MIT licence) | | | the lane is tested with discs and AIF executables built in the tests; the AIF header layout (big-endian words, entry as the BL at word 3) was checked against the first 384 bytes of a published LaunchMe, read and not kept |

Not done for these four: the real WonderSwan and 3DO programs above are missing (nothing to put through `reach --judge` but the tests' own cartridges); the Jaguar's
GPU and DSP RISC code stays data and the 6 MB banked cartridges are not modelled; the 5200's 40 KB banked cartridges are not modelled.

## PlayStation homebrew (the PS1 lane)

Open-source PS-X EXEs with published source, downloaded 2026-11-14 as binaries from the authors' own repositories (kept outside
the repository, in `GitHub/homebrew-samples/ps1/`): JaCzekanski/ps1-tests (MIT; the `tests.zip` of release build-158: C++ built with
GCC and PSn00bSDK) and PeterLemon/PSX (no licence stated; hand-written MIPS assembly with its data). Nothing downloaded was run.
Each is written as GNU as source by `source/ps1_writer` and built again by CodeBreaker's own MIPS assembler; `reach --judge` then judged every function (rebuilt identical, so each is the original: 58 programs, 6107 functions, 100% verified). *Functions* are the
function starts found (the entry point, what it calls, what begins after another function's end and reads as a function, what a `lui`/`addiu` pair or a table points at).

| Program | Platform | Author / source | License | Functions | Result |
|---|---|---|---|---|---|
| CPUADD.exe | PlayStation | PeterLemon/PSX (CPUTest/CPU/ADD) | none stated | 50 | rebuilds identical, verified |
| CPUDIV.exe | PlayStation | PeterLemon/PSX (CPUTest/CPU/DIV) | none stated | 71 | rebuilds identical, verified |
| CPULW.exe | PlayStation | PeterLemon/PSX (CPUTest/CPU/LOADSTORE/LW) | none stated | 116 | rebuilds identical, verified |
| CPUMULT.exe | PlayStation | PeterLemon/PSX (CPUTest/CPU/MULT) | none stated | 71 | rebuilds identical, verified |
| Cube.exe | PlayStation | PeterLemon/PSX (Cube) | none stated | 1 | rebuilds identical, verified |
| GTEAVSZ.exe | PlayStation | PeterLemon/PSX (CPUTest/GTE/AVSZ) | none stated | 31 | rebuilds identical, verified |
| GTENCLIP.exe | PlayStation | PeterLemon/PSX (CPUTest/GTE/NCLIP) | none stated | 31 | rebuilds identical, verified |
| LZ77Decode.exe | PlayStation | PeterLemon/PSX (Compress/LZ77/LZ77Decode) | none stated | 3 | rebuilds identical, verified |
| PRINTGPU.exe | PlayStation | PeterLemon/PSX (Demo/printgpu) | none stated | 23 | rebuilds identical, verified |
| VBLANK.exe | PlayStation | PeterLemon/PSX (Demo/vblank) | none stated | 8 | rebuilds identical, verified |
| disc-swap.exe | PlayStation | JaCzekanski/ps1-tests (cdrom/disc-swap) | MIT | 101 | rebuilds identical, verified |
| getloc.exe | PlayStation | JaCzekanski/ps1-tests (cdrom/getloc) | MIT | 129 | rebuilds identical, verified |
| terminal.exe | PlayStation | JaCzekanski/ps1-tests (cdrom/terminal) | MIT | 435 | rebuilds identical, verified |
| timing.exe | PlayStation | JaCzekanski/ps1-tests (cdrom/timing) | MIT | 102 | rebuilds identical, verified |
| access-time.exe | PlayStation | JaCzekanski/ps1-tests (cpu/access-time) | MIT | 88 | rebuilds identical, verified |
| code-in-io.exe | PlayStation | JaCzekanski/ps1-tests (cpu/code-in-io) | MIT | 107 | rebuilds identical, verified |
| cop.exe | PlayStation | JaCzekanski/ps1-tests (cpu/cop) | MIT | 92 | rebuilds identical, verified |
| io-access-bitwidth.exe | PlayStation | JaCzekanski/ps1-tests (cpu/io-access-bitwidth) | MIT | 178 | rebuilds identical, verified |
| chain-looping.exe | PlayStation | JaCzekanski/ps1-tests (dma/chain-looping) | MIT | 87 | rebuilds identical, verified |
| chopping.exe | PlayStation | JaCzekanski/ps1-tests (dma/chopping) | MIT | 88 | rebuilds identical, verified |
| dpcr.exe | PlayStation | JaCzekanski/ps1-tests (dma/dpcr) | MIT | 103 | rebuilds identical, verified |
| otc-test.exe | PlayStation | JaCzekanski/ps1-tests (dma/otc-test) | MIT | 145 | rebuilds identical, verified |
| animated-triangle.exe | PlayStation | JaCzekanski/ps1-tests (gpu/animated-triangle) | MIT | 114 | rebuilds identical, verified |
| bandwidth.exe | PlayStation | JaCzekanski/ps1-tests (gpu/bandwidth) | MIT | 94 | rebuilds identical, verified |
| benchmark.exe | PlayStation | JaCzekanski/ps1-tests (gpu/benchmark) | MIT | 309 | rebuilds identical, verified |
| clipping.exe | PlayStation | JaCzekanski/ps1-tests (gpu/clipping) | MIT | 74 | rebuilds identical, verified |
| clut-cache.exe | PlayStation | JaCzekanski/ps1-tests (gpu/clut-cache) | MIT | 86 | rebuilds identical, verified |
| gp0-e1.exe | PlayStation | JaCzekanski/ps1-tests (gpu/gp0-e1) | MIT | 94 | rebuilds identical, verified |
| lines.exe | PlayStation | JaCzekanski/ps1-tests (gpu/lines) | MIT | 71 | rebuilds identical, verified |
| mask-bit.exe | PlayStation | JaCzekanski/ps1-tests (gpu/mask-bit) | MIT | 87 | rebuilds identical, verified |
| quad.exe | PlayStation | JaCzekanski/ps1-tests (gpu/quad) | MIT | 69 | rebuilds identical, verified |
| rectangles.exe | PlayStation | JaCzekanski/ps1-tests (gpu/rectangles) | MIT | 75 | rebuilds identical, verified |
| texture-flip.exe | PlayStation | JaCzekanski/ps1-tests (gpu/texture-flip) | MIT | 71 | rebuilds identical, verified |
| texture-overflow.exe | PlayStation | JaCzekanski/ps1-tests (gpu/texture-overflow) | MIT | 72 | rebuilds identical, verified |
| transparency.exe | PlayStation | JaCzekanski/ps1-tests (gpu/transparency) | MIT | 69 | rebuilds identical, verified |
| triangle.exe | PlayStation | JaCzekanski/ps1-tests (gpu/triangle) | MIT | 68 | rebuilds identical, verified |
| uv-interpolation.exe | PlayStation | JaCzekanski/ps1-tests (gpu/uv-interpolation) | MIT | 71 | rebuilds identical, verified |
| version-detect.exe | PlayStation | JaCzekanski/ps1-tests (gpu/version-detect) | MIT | 65 | rebuilds identical, verified |
| vram-to-vram-overlap.exe | PlayStation | JaCzekanski/ps1-tests (gpu/vram-to-vram-overlap) | MIT | 280 | rebuilds identical, verified |
| gte-fuzz.exe | PlayStation | JaCzekanski/ps1-tests (gte-fuzz) | MIT | 305 | rebuilds identical, verified |
| test-all.exe | PlayStation | JaCzekanski/ps1-tests (gte/test-all) | MIT | 203 | rebuilds identical, verified |
| pad.exe | PlayStation | JaCzekanski/ps1-tests (input/pad) | MIT | 64 | rebuilds identical, verified |
| 4bit.exe | PlayStation | JaCzekanski/ps1-tests (mdec/4bit) | MIT | 88 | rebuilds identical, verified |
| 8bit.exe | PlayStation | JaCzekanski/ps1-tests (mdec/8bit) | MIT | 88 | rebuilds identical, verified |
| frame-15bit-dma.exe | PlayStation | JaCzekanski/ps1-tests (mdec/frame) | MIT | 85 | rebuilds identical, verified |
| frame-15bit.exe | PlayStation | JaCzekanski/ps1-tests (mdec/frame) | MIT | 86 | rebuilds identical, verified |
| frame-24bit-dma.exe | PlayStation | JaCzekanski/ps1-tests (mdec/frame) | MIT | 85 | rebuilds identical, verified |
| frame-24bit.exe | PlayStation | JaCzekanski/ps1-tests (mdec/frame) | MIT | 86 | rebuilds identical, verified |
| movie-15bit.exe | PlayStation | JaCzekanski/ps1-tests (mdec/movie) | MIT | 90 | rebuilds identical, verified |
| movie-24bit.exe | PlayStation | JaCzekanski/ps1-tests (mdec/movie) | MIT | 90 | rebuilds identical, verified |
| step-by-step-log.exe | PlayStation | JaCzekanski/ps1-tests (mdec/step-by-step-log) | MIT | 95 | rebuilds identical, verified |
| memory-transfer.exe | PlayStation | JaCzekanski/ps1-tests (spu/memory-transfer) | MIT | 188 | rebuilds identical, verified |
| ram-sandbox.exe | PlayStation | JaCzekanski/ps1-tests (spu/ram-sandbox) | MIT | 125 | rebuilds identical, verified |
| stereo.exe | PlayStation | JaCzekanski/ps1-tests (spu/stereo) | MIT | 72 | rebuilds identical, verified |
| test.exe | PlayStation | JaCzekanski/ps1-tests (spu/test) | MIT | 69 | rebuilds identical, verified |
| toolbox.exe | PlayStation | JaCzekanski/ps1-tests (spu/toolbox) | MIT | 294 | rebuilds identical, verified |
| timer-dump.exe | PlayStation | JaCzekanski/ps1-tests (timer-dump) | MIT | 76 | rebuilds identical, verified |
| timers.exe | PlayStation | JaCzekanski/ps1-tests (timers) | MIT | 89 | rebuilds identical, verified |

On a disc (2026-11-26): CPUADD.exe put on a PlayStation disc as SLUS_000.01, with a SYSTEM.CNF that boots it (`work/ps1disc/make.py`), as a cooked `.iso`, a raw mode 2 `.bin` and its `.cue`. `inspect` says "PlayStation disc, boots SLUS_000.01". `convert` of the `.cue` writes SLUS_000.01.s, the same source as CPUADD.exe's own. `check` of the `.bin` verifies 50 of 50. No PlayStation disc with published source was found, so the disc was made here around a program that has one.

## Game Boy Advance homebrew (the GBA lane)

Open-source GBA ROMs with published source, downloaded 2026-11-15 as binaries from the authors' own repositories (kept outside the repository, in
`GitHub/homebrew-samples/gba/`): jsmolka/gba-tests (MIT; hand-written ARM and Thumb conformance suites), nba-emu/hw-test (BSD-3-Clause;
hardware tests), ZeroDayArcade/Pong-Homebrew-GBA (MIT; C), djedditt/gba-rumble-example (MIT; C), HorstBaerbel/GBA-mem (MIT; C, a memory test).
Each is written as GNU as source by `source/gba_writer` and built again by CodeBreaker's own ARM/Thumb assembler; `reach --judge --per-platform 100` then judged every
function (rebuilt identical, so each is the original: 41 programs, 4075 functions, 100% verified). *Functions* are the starts the writer found, ARM and Thumb.

| Program | Platform | Source | Licence | Functions | Result |
|---|---|---|---|---|---|
| MemTestGBA.gba | Game Boy Advance | HorstBaerbel/GBA-mem | MIT | 37 | rebuilds identical, verified |
| Pong-Homebrew-GBA.gba | Game Boy Advance | ZeroDayArcade/Pong-Homebrew-GBA | MIT | 45 | rebuilds identical, verified |
| archive-irq-cancel-irq-ie-cancel-irq-ie.gba | Game Boy Advance | nba-emu/hw-test (archive/irq-cancel-irq-ie-cancel-irq-ie.gba) | BSD-3-Clause | 176 | rebuilds identical, verified |
| archive-irq-cancel-irq-if-cancel-irq-if.gba | Game Boy Advance | nba-emu/hw-test (archive/irq-cancel-irq-if-cancel-irq-if.gba) | BSD-3-Clause | 176 | rebuilds identical, verified |
| archive-irq-cancel-irq-ime-cancel-irq-ime.gba | Game Boy Advance | nba-emu/hw-test (archive/irq-cancel-irq-ime-cancel-irq-ime.gba) | BSD-3-Clause | 176 | rebuilds identical, verified |
| archive-ppu-basic-timing-basic-timing.gba | Game Boy Advance | nba-emu/hw-test (archive/ppu-basic-timing-basic-timing.gba) | BSD-3-Clause | 177 | rebuilds identical, verified |
| archive-ppu-exact-timing-exact-timing.gba | Game Boy Advance | nba-emu/hw-test (archive/ppu-exact-timing-exact-timing.gba) | BSD-3-Clause | 170 | rebuilds identical, verified |
| archive-ppu-mode2-mode2.gba | Game Boy Advance | nba-emu/hw-test (archive/ppu-mode2-mode2.gba) | BSD-3-Clause | 14 | rebuilds identical, verified |
| archive-ppu-mode3-mode3.gba | Game Boy Advance | nba-emu/hw-test (archive/ppu-mode3-mode3.gba) | BSD-3-Clause | 14 | rebuilds identical, verified |
| archive-ppu-mode4-mode4.gba | Game Boy Advance | nba-emu/hw-test (archive/ppu-mode4-mode4.gba) | BSD-3-Clause | 14 | rebuilds identical, verified |
| arm-arm.gba | Game Boy Advance | jsmolka/gba-tests (arm-arm) | MIT | 11 | rebuilds identical, verified |
| bios-bios.gba | Game Boy Advance | jsmolka/gba-tests (bios-bios) | MIT | 7 | rebuilds identical, verified |
| bus-128kb-boundary-128kb-boundary.gba | Game Boy Advance | nba-emu/hw-test (bus-128kb-boundary-128kb-boundary) | BSD-3-Clause | 252 | rebuilds identical, verified |
| dma-burst-into-tears-burst-into-tears.gba | Game Boy Advance | nba-emu/hw-test (dma-burst-into-tears-burst-into-tears) | BSD-3-Clause | 253 | rebuilds identical, verified |
| dma-force-nseq-access-force-nseq-access.gba | Game Boy Advance | nba-emu/hw-test (dma-force-nseq-access-force-nseq-access) | BSD-3-Clause | 257 | rebuilds identical, verified |
| dma-latch-latch.gba | Game Boy Advance | nba-emu/hw-test (dma-latch-latch) | BSD-3-Clause | 256 | rebuilds identical, verified |
| dma-start-delay-start-delay.gba | Game Boy Advance | nba-emu/hw-test (dma-start-delay-start-delay) | BSD-3-Clause | 253 | rebuilds identical, verified |
| gba-rumble-example.gba | Game Boy Advance | djedditt/gba-rumble-example | MIT | 290 | rebuilds identical, verified |
| haltcnt-haltcnt.gba | Game Boy Advance | nba-emu/hw-test (haltcnt-haltcnt) | BSD-3-Clause | 183 | rebuilds identical, verified |
| irq-irq-delay-irq-delay.gba | Game Boy Advance | nba-emu/hw-test (irq-irq-delay-irq-delay) | BSD-3-Clause | 183 | rebuilds identical, verified |
| memory-memory.gba | Game Boy Advance | jsmolka/gba-tests (memory-memory) | MIT | 7 | rebuilds identical, verified |
| nes-nes.gba | Game Boy Advance | jsmolka/gba-tests (nes-nes) | MIT | 2 | rebuilds identical, verified |
| ppu-bgpd-bgpd.gba | Game Boy Advance | nba-emu/hw-test (ppu-bgpd-bgpd) | BSD-3-Clause | 15 | rebuilds identical, verified |
| ppu-bgx-bgx.gba | Game Boy Advance | nba-emu/hw-test (ppu-bgx-bgx) | BSD-3-Clause | 15 | rebuilds identical, verified |
| ppu-dispcnt-latch-dispcnt-latch.gba | Game Boy Advance | nba-emu/hw-test (ppu-dispcnt-latch-dispcnt-latch) | BSD-3-Clause | 103 | rebuilds identical, verified |
| ppu-greenswap-greenswap.gba | Game Boy Advance | nba-emu/hw-test (ppu-greenswap-greenswap) | BSD-3-Clause | 22 | rebuilds identical, verified |
| ppu-hello.gba | Game Boy Advance | jsmolka/gba-tests (ppu-hello) | MIT | 7 | rebuilds identical, verified |
| ppu-ram-access-timing-ram-access-timing.gba | Game Boy Advance | nba-emu/hw-test (ppu-ram-access-timing-ram-access-timing) | BSD-3-Clause | 187 | rebuilds identical, verified |
| ppu-shades.gba | Game Boy Advance | jsmolka/gba-tests (ppu-shades) | MIT | 2 | rebuilds identical, verified |
| ppu-sprite-hmosaic-sprite-hmosaic.gba | Game Boy Advance | nba-emu/hw-test (ppu-sprite-hmosaic-sprite-hmosaic) | BSD-3-Clause | 24 | rebuilds identical, verified |
| ppu-status-irq-dma-status-irq-dma.gba | Game Boy Advance | nba-emu/hw-test (ppu-status-irq-dma-status-irq-dma) | BSD-3-Clause | 193 | rebuilds identical, verified |
| ppu-stripes.gba | Game Boy Advance | jsmolka/gba-tests (ppu-stripes) | MIT | 2 | rebuilds identical, verified |
| ppu-vram-mirror-vram-mirror.gba | Game Boy Advance | nba-emu/hw-test (ppu-vram-mirror-vram-mirror) | BSD-3-Clause | 171 | rebuilds identical, verified |
| save-flash128.gba | Game Boy Advance | jsmolka/gba-tests (save-flash128) | MIT | 7 | rebuilds identical, verified |
| save-flash64.gba | Game Boy Advance | jsmolka/gba-tests (save-flash64) | MIT | 7 | rebuilds identical, verified |
| save-none.gba | Game Boy Advance | jsmolka/gba-tests (save-none) | MIT | 7 | rebuilds identical, verified |
| save-sram.gba | Game Boy Advance | jsmolka/gba-tests (save-sram) | MIT | 7 | rebuilds identical, verified |
| thumb-thumb.gba | Game Boy Advance | jsmolka/gba-tests (thumb-thumb) | MIT | 10 | rebuilds identical, verified |
| timer-reload-reload.gba | Game Boy Advance | nba-emu/hw-test (timer-reload-reload) | BSD-3-Clause | 163 | rebuilds identical, verified |
| timer-start-stop-start-stop.gba | Game Boy Advance | nba-emu/hw-test (timer-start-stop-start-stop) | BSD-3-Clause | 172 | rebuilds identical, verified |
| unsafe-unsafe.gba | Game Boy Advance | jsmolka/gba-tests (unsafe-unsafe) | MIT | 8 | rebuilds identical, verified |

## Xbox 360 titles (recompile lane)

Logan's own copies (`C:\Nexia360\Games`), through `recompile-project <title> --check`. All are in
the Xbox 360 Dashboard's games section.

| Title | Kind | Result |
|---|---|---|
| Tetris Splash | XBLA | plays |
| Minecraft: Xbox 360 Edition (TU0) | disc | plays |
| Disney·Pixar Up | disc | plays |
| PAC-MAN Championship Edition | XBLA | plays (untuned: the first title through one command) |
| Ms. Pac-Man | XBLA | runs (its default.xex is a renamed LIVE package) |
| GALAGA | XBLA | runs, menu seen |
| DIG-DUG | XBLA | runs |
| Bomberman LIVE | XBLA | runs |
| UNO Rush | XBLA | runs |
| Doritos Crash Course | XBLA | runs (needed small system blocks from the kernel pool) |
| Ecco the Dolphin | XBLA (Sega Vintage Collection) | runs (needed listen / accept / recv) |
| Zuma | XBLA | runs (needed the XEX load address for a 2005 basefile) |
| Trials HD | XBLA | runs, slowly (CPU-bound, about 12 frames/s) |
| TRON | XBLA (Digital Eclipse emulation) | runs (needed tessellation and achievement strings) |

## Windows 95 (source lanes)

The Windows 95 corpus (the Windows95 repository): every executable class through the native
decompiler -- 363 NE, 636 PE, 223 VxD and 92 DOS modules. Rebuilt and running: the 15 MINI.CAB
modules (KRNL386, USER, GDI, ...) boot together to Setup's Wizard and install Windows to the
desktop (2026-10-30, master af5ffb910: rebuilt from source, Setup's License page reached twice
each with the caller-results rule off, on, and on with fixed segments allowed to grow; the
milestone sweep of that day -- the smallest 95 32-bit and 90 each of 16-bit, VxD and DOS modules
-- is in docs/history/STATUS.md); the 7 Setup programs; 212 of 215 VxDs build, 40 boot-set VxDs load in the running system.
`vxd boot` (2026-10-28, docs/VXD.md): VMM32.VXD (build_w3) and DOSMGR, IFSMGR, SHELL, VDMAD, VFAT,
VMPOLL, VPICD, VTD, VTDAPI, VXDLDR (build_le), each loaded and started, boot an installed Windows 95
to the desktop; DRVSPACX and VDHCP rebuilt too but not loaded on that machine.

## Open-source C (ground truth for the C lanes)

Compiled with OpenWatcom and taken back to C by the OSS benchmark, their own source the
answer: cJSON, zlib, sds, tiny-AES-c, inih, Lua, miniz.

## XNA / C# (.NET lane)

Since 2026-11-27 the .NET lane's decompiler is CodeBreaker's own port of ILSpy 11 (src/dotnet/decompiler, with ilspycmd's command
line): no ilspycmd and no .NET are needed. Its C# is ilspycmd 11.0's, byte for byte, on what is below.

| Program | Platform | Result |
|---|---|---|
| ILSpy's own test suite (TestCases/Pretty: 112 programs built with Roslyn 4, .NET 9) | .NET 9 (C# 1-13) | C# identical to ilspycmd 11 on all 112, byte for byte; the ILAst after each of ILSpy's 39 IL transforms identical too |
| System.Linq, System.Text.Json, System.Private.Xml (.NET 9, ReadyToRun x64); System.Core, Microsoft.VisualBasic (.NET Framework 4) | .NET 9, .NET Framework 4 | C# identical to ilspycmd 11 (System.Private.Xml: 6.8 MB of C#) |
| System.Private.CoreLib 9, mscorlib 2.0 and 4.0, System.Windows.Forms 4, Microsoft.Xna.Framework 3.1 | .NET 9, .NET Framework 2 and 4, XNA 3.1 | metadata, IL text, type system and ILAst identical to ILSpy's (CoreLib: 958 000 ILAst lines, 2.4 s against ILSpy's 4.5 s); XNA's C# differs from ilspycmd's in the order of two `using` lines (being fixed) |
| ISymWrapper, CustomMarshalers (.NET Framework 4, C++/CLI mixed mode, x64); an ARM64 build | .NET Framework 4, .NET 9 | C# identical to ilspycmd 11 |
| Arcadecraft (Firebase Industries, 2011) | Xbox 360 Indie Game (XNA 4.0) | `xna`: 6 assemblies to C# (583 files, 125 000 lines: the game, its two own libraries, SunBurn, DPSF, EasyStorage); 610 content files -- 434 textures and 6 fonts as PNG, pixel for pixel as Arcadecraft's own pipeline made them, 170 other types catalogued; all 106 XACT cues resolved to their waves (as the hand port's own map, 97 of 97). `xna port` with its overlay (the deobfuscated SunBurn, the reconstructed shaders, its port runtime: ../Arcadecraft) builds and runs to the title screen through the real SunBurn renderer. Without the overlay, the generic lane now deobfuscates SunBurn (de4dot finds Spices.Net) and the port compiles with nothing written by hand; it runs until SunBurn's own licence check ("Product not activated"), which CodeBreaker does not bypass -- the overlay is the way past it. Its shaders exist only as Xbox microcode |
| Block World (Hazzah Games) | Xbox 360 Indie Game (XNA 4.0) | `xna`: 16 assemblies to C# (the game and its Hazzah engine); 210 textures, 3 fonts, 150 sound effects and the XACT sounds (XMA2 decoded), 6 songs, and all 20 effects recovered as HLSL from their Xbox microcode (19 .xnb and the copy of XNA's stock BasicEffect it reads as a file: one technique per ShaderIndex). 105 files of the package are empty (an earlier extraction filled them with the next file's blocks: fixed). `xna port`, nothing written by hand: builds (the compile loop's MonoGame differences, effect bytes passed through the runtime, the controller read with the keyboard too) and plays its menus: title, loading, the main menu and the world customizer, the 3D block world rendering behind them through the recovered shaders and byte-swapped models. `--check-content`: 461 of 463 .xnb load (all 41 models with plausible buffers, 19 effects, 210 textures, the cube map, 150 sound effects, 6 songs, 22 avatar animations, 8 particle effects); the 2 left are models whose textures the game never shipped. Entering a world not yet driven by a scripted run (its menu needs a person) |

## Java

Minecraft (alpha, PC): its classes as C++03 through the Java lane, built and running.

Minecraft Alpha (decompiled source), as C#: NoiseGenerator, NoiseGeneratorPerlin, NoiseGeneratorOctaves and
MathHelper through the javac front end and the C# back end (translator phase 7), judged beside the JVM with a
driver of seeded world-generation calls: the same output, java.util.Random and Java's text for doubles included
(the C# kit's tests under tests/translate).

## The universal translator (C# and Python inputs)

| Input | Licence | Result |
|---|---|---|
| tests/translate/corpus/csharp (10), csharp_cpp (2), csharp_features (11) | written for the kit | C# -> C++17: 23 of 23 lower, write, compile and agree with g++, clang and MSVC; the 11 feature programs also round-trip through the C# back end |
| CPython standard library: bisect, heapq, colorsys, keyword, operator, fnmatch, genericpath, posixpath, shlex, textwrap, reprlib (read in place from the installed Python's Lib, not copied) | PSF-2.0 | Python front end census (`translate.python.package.the_standard_library_census`): 11 modules, 184 functions, 2 translate, 182 refused (untyped 91, stdlib 36, dynamic 30, conflict 8, unresolved 6, syntax 4); the reasons are the next work |
| C# from Reko (Win16-Research/reko) and ExtremeLauncher on this machine | GPL | not used: not permissive |

## Nintendo DS homebrew (the DS lane)

Open-source DS ROMs with published source, downloaded 2026-11-15 as binaries from the authors' own release pages (kept outside the repository, in
`GitHub/homebrew-samples/nds/`). Each is written as GNU as source by `source/nds_writer` (both processors in one file) and built again by CodeBreaker's own
assembler; every function is then run in the interpreter of its own processor in the original and in the rebuilt ROM
(`cb_tests --filter machine_nds.homebrew`; it skips programs that are not on the machine).

| Program | Platform | Source | Licence | Functions | Result |
|---|---|---|---|---|---|
| DinoDS.nds | Nintendo DS | BotRandomness/DinoDS v1.0.0 (C, libnds) | MIT | 794 (ARM9 585, ARM7 209; 753 Thumb) | rebuilds identical; 3176 of 3176 runs (4 states) the same in the rebuilt ROM: 1692 return, 906 reach the step limit, 526 stop at a bus error (hardware not modelled), 52 at bytes that are no instruction |
| dldi_benchmark_blocksds.nds | Nintendo DS | asiekierka/dldi-driver-benchmark v0.4.3 (BlocksDS) | MIT | not recorded | run did not finish in the session: not yet verified |
| dldi_benchmark_dkA.nds | Nintendo DS | asiekierka/dldi-driver-benchmark v0.4.3 (devkitARM) | MIT | not recorded | run did not finish in the session: not yet verified |
| ButtonBoot.nds | Nintendo DS | FlameKat53/ButtonBoot v3.0.0 | MIT | - | refused: its header has no DS logo checksum ($CF56 at $15C), so it is not read as a DS ROM |

## Nintendo 64 homebrew (the N64 lane)

Open-source ROMs with published source, downloaded 2026-11-15 as binaries from the authors' own repositories (kept outside the
repository, in `GitHub/homebrew-samples/n64/`): PeterLemon/N64 (Unlicense; hand-written MIPS assembly for the R4300i, the
`.N64`/`.n64` files are big-endian `.z64` data, 1 MB programs under the standard CIC-NUS-6102 boot code: 78 of the repository's 408 ROMs,
taken across its groups) and vrgl117-games/sblobber64 (MIT; a libdragon game in C built with GCC, the `.z64` of the release
`n64brew-jam-1`). Nothing downloaded was run. Each is written as GNU as source by `source/n64_writer` and built again by CodeBreaker's
own big-endian MIPS assembler (`reach --judge`: every function of the original verified, which for an identical rebuild is the rebuild
itself; the unit test `engine.machine_n64.homebrew` also runs every function of the original and of the rebuilt ROM in `emulate/r4300` from
two states, and runs the whole program from its entry point for three million instructions). Since 2026-11-26 a function that starts the
RSP has its microcode run to its BREAK in `emulate/rsp`, and every display list handed to the RDP is compared as its commands (`emulate/rdp`;
the RDP is not run, so there are no pixels). RSP DCT FastDCTBlockDecode: its task runs to its BREAK and the ROM still rebuilds identical.
RDP AlphaCompare: 3,696 RDP commands read (`engine.machine_n64.rsp_tasks_run_and_rdp_lists_are_read`). The microcode of the three RSP DCT
ROMs reads back word for word through the RSP decoder (`decompile.mips.rsp.peterlemon_microcode_reads_back`).

| Programs | Platform | Author / source | License | Functions | Result |
|---|---|---|---|---|---|
| CPUTest/CPU: 22 ROMs (ADDU, DADD, DDIV, DIV, DMULT, DSUB, the shifts DSLL32 DSRA DSRAV DSRL32 SLL SRA SRL, LB LH LW SD SW, MULTU, OR, SUB, TIMINGNTSC ...) | Nintendo 64 | PeterLemon/N64 (CPUTest/CPU) | Unlicense | 2,600 | rebuild identical, verified |
| CPUTest/CP1: 12 ROMs (ABS ADD CEIL CVT DIV FLOOR MUL NEG ROUND SQRT SUB TRUNC) | Nintendo 64 | PeterLemon/N64 (CPUTest/CP1) | Unlicense | 1,294 | rebuild identical, verified |
| CPUTest/CP0 (2), CPUTest/Exceptions (8: Compare, Syscall, TLB) | Nintendo 64 | PeterLemon/N64 (CPUTest) | Unlicense | 113, 383 | rebuild identical, verified |
| HelloWorld (4), FrameBuffer (2), SetScreen NTSC / PAL, Initialize, Input (3), Interrupt (3) | Nintendo 64 | PeterLemon/N64 | Unlicense | 1 to 6 each | rebuild identical, verified |
| Compress: HUFFMAN (3), LZ77 (3) | Nintendo 64 | PeterLemon/N64 (Compress) | Unlicense | 2 to 3 each | rebuild identical, verified |
| RCP (2), RDP (AlphaCompare, AlphaCoverage, RDPModeInput), RDRAMTest, RSP DCT (3), CP1 Fractal (4) | Nintendo 64 | PeterLemon/N64 | Unlicense | 1 to 169 each | rebuild identical, verified |
| sblobber64.z64 (5.8 MB: a libdragon game; its code ends before the first megabyte, the rest is data) | Nintendo 64 (libdragon, GCC) | vrgl117-games/sblobber64 (release n64brew-jam-1) | MIT | 3,445 | rebuilds identical, verified; from its entry point it runs three million instructions of libdragon's startup (stack from the memory size the boot code leaves, the code copied from the cartridge, PI DMA of its data, PI_STATUS polled) |

78 ROMs of PeterLemon/N64: 4,669 functions in all; with sblobber64, 79 ROMs and 8,114 functions. Every ROM's header checksums are the ones the
CIC-NUS-6102 algorithm computes (`formats.n64.the_checksums_of_real_roms_are_the_ones_the_boot_code_computes`).

## Sega 32X and Saturn homebrew (the SH-2 lanes)

Open-source programs with published source, downloaded 2026-11-15 as binaries from the authors' release pages (kept outside the repository, in
`GitHub/homebrew-samples/sega32x/` and `saturn/`). Nothing downloaded was run. Each is written as GNU as source by `source/sega32x_writer` or
`source/saturn_writer` and built again by CodeBreaker's own SH-2 assembler; `reach --judge` then judged every function (rebuilt identical, so each
is the original: 3 programs on 32X or Saturn disc images, 1,460 functions, 100% verified), and a copy with each of 12 functions' first instruction
changed is run beside the original in the SH-2 interpreter and diverges (`source.sega32x.homebrew`, `source.saturn.homebrew`).

| Program | Platform | Author / source | License | Functions | Result |
|---|---|---|---|---|---|
| 240p Test Suite 1.0 (`240pSuite_32X_1.0.32x`, 1.9 MB; GCC-built SH-2 code, graphics and tables) | Sega 32X | Dasutin/32X240pTestSuite (release v1.0) | GPL-2.0 | 85 SH-2 + 4 68000 | rebuilds identical, verified (the SH-2 program is 126,704 bytes at ROM offset $1BE4B0: 9.5 KB of it is reached as code, the rest data; 12 of 12 edited functions diverge) |
| RoQ Player (`roq32X.32x`, 3.5 MB; hand-written SH-2 assembly) | Sega 32X | viciious/roq32x (release 2.0) | MIT | 63 SH-2 + 4 68000 | rebuilds identical, verified (11 of 12 edited functions diverge) |
| Save Game Copier 3.7.1, 3.7.0 and 3.6.18 (`game.iso`, 824 KB; Jo Engine, GCC) | Sega Saturn | slinga-homebrew/Save-Game-Copier (releases) | GPL-3.0 | 1,304 / 1,304 / 1,222 | rebuild identical, verified (the system area: IP.BIN of 6,144 bytes with its four area-code entries; the first program 0.BIN of 203,856 bytes at $06004000, 94 KB of it reached as code; 11 of 12 edited functions diverge in each) |

Not done: no open-source Saturn or 32X program with a published binary and a permissive licence was found beyond these (Jo Engine's own samples
and libyaul's examples are source only, and building them needs a downloaded toolchain, which is not run). Dreamcast (SH-4) is below.

## GameCube, Wii and Wii U homebrew (the gamecube, wii and wiiu lanes)

Open-source homebrew with published source and a permissive licence, binaries only, downloaded 2026-11-15 from the authors' own
GitHub releases (kept outside the repository, in `GitHub/homebrew-samples/{gamecube,wii,wiiu}/`; never run on this machine). Functions are
the ones `reach` finds; "runs" is how many of the first 60 functions, each from two states, the original and the rebuilt program ran the
same in the PowerPC interpreter (an identical rebuild always does).

| Program | Console | Author / source | License | Functions | Result |
|---|---|---|---|---|---|
| Wii-donut.c (`Wii-donut.c-gc.dol`, r9) | GameCube | AndrewPiroli/Wii-donut.c | Unlicense | 385 | rebuilds identical, verified |
| BEANZ GameCube Monitor v1.1 (`.dol` and `.elf` with its symbol table) | GameCube | nxs8739/BEANZ-GameCube-Monitor | MIT | 354 (each) | both rebuild identical, verified |
| SuDokuL v1.5 (gamecube) | GameCube | Mode8fx/SuDokuL | MIT | 4,703 | rebuilds identical, verified |
| GCMM-EX v1.0.1 (`gcmm_ex_GC.dol` / `.elf`), gcradio v3.2.0 (`.dol`, `-gui.dol`), cubemenu v0.3.0 | GameCube | alexishida/gcmm-ex, RyoSaeba89/gcradio, hudsonbrendon/cubemenu | MIT | in `homebrew-samples`; run with `CB_GEKKO_ALL=1` | larger programs, not in the unit test |
| Wii-donut.c (Wii `boot.dol`, r9) | Wii | AndrewPiroli/Wii-donut.c | Unlicense | 738 | rebuilds identical, verified |
| doodle-jump-wii v1.0.0 (`doodle-wii.dol`) | Wii | TechAlberto/doodle-jump-wii | MIT | 838 | rebuilds identical, verified |
| Scuffedcraft 0.0.1 (`.dol`, `.elf`), GCMM-EX (Wii build), SuDokuL v1.5 (wii) | Wii | pnyaa/Scuffedcraft, alexishida/gcmm-ex, Mode8fx/SuDokuL | MIT | in `homebrew-samples`; `CB_GEKKO_ALL=1` | larger programs, not in the unit test |
| MultiDRCSpaceDemo v0.1 (`.rpx`, compressed sections) | Wii U | GaryOderNichts/MultiDRCSpaceDemo | MIT | 1,979 | rebuilds identical (the decompressed file), verified |
| gamepadtopro 1.1.1 (`.wps`, a Wii U plugin: an RPL) | Wii U | capitalistspz/gamepadtopro | MIT | 488 | rebuilds identical (decompressed), verified |
| SuDokuL v1.5 (wiiu `.rpx`) | Wii U | Mode8fx/SuDokuL | MIT | in `homebrew-samples`; `CB_GEKKO_ALL=1` | larger program, not in the unit test |

## Sega Dreamcast homebrew (the SH-4 lane)

Open-source program with published source, downloaded 2026-11-15 as a binary from the author's release page (kept outside the repository, in
`GitHub/homebrew-samples/dreamcast/`). Nothing downloaded was run. The release ships `1ST_READ.BIN` without a disc, so `source.dreamcast.homebrew`
puts it in a disc of its own (the test IP.BIN of `source/dreamcast_test_programs.hpp`, the program at $8C010000), writes it as GNU as source with
`source/dreamcast_writer`, builds the source again with CodeBreaker's own SH-4 assembler (identical), and runs a copy with each of 12 functions' first
instruction changed beside the original in the SH-4 interpreter.

| Program | Platform | Author / source | License | Functions | Result |
|---|---|---|---|---|---|
| openMenu 1.3.0 (`1ST_READ.BIN`, 311,220 bytes; KallistiOS / GCC SH-4 code with FPU) | Sega Dreamcast | sbstnc/openmenu (release v1.3.0; openMenu, the open-source GDEMU frontend) | BSD-3-Clause | 1,497 | rebuilds identical; 9 of 12 edited functions diverge |

Not done: no open-source Dreamcast program shipped as a `.cdi` / `.gdi` with a permissive licence was found (the KallistiOS examples and most homebrew are
source only, and building them needs a downloaded toolchain), so `.cdi`, `.gdi` and a scrambled `1ST_READ.BIN` are tested on discs the tests build, not on a real
image; `reach --judge` was not run on a real disc.

## PlayStation 2 and PSP homebrew (the ps2 and psp lanes)

Open-source programs with published source and a stated licence, downloaded 2026-11-15 as binaries from the authors' own release pages (kept outside the repository, in
`GitHub/homebrew-samples/ps2/` and `/psp/`; never run on this machine). Each is written as GNU as source by `source/mips_elf_writer` and built again by CodeBreaker's own
assembler; its functions (the first 150, or `CB_PS2_PSP_FUNCTIONS`) are then run in the interpreter, in the original and in the rebuilt file
(`cb_tests --filter machine_ps2.homebrew` / `machine_psp.homebrew`; they skip programs that are not on the machine). The ps2-packer builds of Open PS2 Loader and
wLaunchELF (AFL-3.0, AFL-2.0) are not listed: a packed ELF is a decompression stub and a compressed payload with no R5900 flags, so nothing of the program is code to read.

| Program | Platform | Source | Licence | Functions | Result |
|---|---|---|---|---|---|
| NTPS2-UNC.ELF (NTP client, 2.1 MB, symbols) | PlayStation 2 | ShyavanS/NTPS2 (v1.2.1) | GPL-3.0 | 1,033 | rebuilds identical; 300 of 300 judged runs equal |
| quake.elf (Quake port, 1.4 MB) | PlayStation 2 | glampert/quake-ps2 (v1.1.0) | GPL-2.0 | 2,783 | rebuilds identical; the first 129 functions judged equal (running them found the scratchpad bounds bug) |
| ee_core.elf (neutrino's EE core, 25 KB) | PlayStation 2 | rickgaiser/neutrino (v1.8.0) | AFL-3.0 | 124 | rebuilds identical; 248 of 248 runs equal |
| psplink.prx, usbhostfs.prx, usbgdb.prx, remotejoy.prx, debugmenu.prx, scrkprintf.prx, siokprintf.prx, usbkprintf.prx, psplink_user.prx | PSP (unencrypted PRX) | pspdev/psplinkusb (v3.2.1) | BSD-3-Clause | 556, 86, 81, 53, 39, 15, 13, 14, 6 | rebuild identical; every judged run equal (imports named by NID) |
| EBOOT.PBP (psplink) | PSP | pspdev/psplinkusb (v3.2.1) | BSD-3-Clause | 322 | rebuilds identical (the whole package); 120 of 120 runs equal |
| EBOOT.PBP (llama2psp) | PSP | sixf0ur/llama2psp (1.0) | MIT | 528 | rebuilds identical; 120 of 120 runs equal |
| EBOOT.PBP (DOSBox 0.72 for the PSP, 3 MB) | PSP | PSP-Archive/dosbox_psp (0.72-psp-0.0.5) | GPL-2.0 | not finished | the writer's analysis of this 3 MB stripped module takes over 25 minutes (backlog: the function search re-reads every pointer-like word); skipped by the test unless `CB_PS2_PSP_ALL` is set |

## Nintendo 3DS homebrew (the n3ds lane)

Open-source homebrew with published source, binaries only, kept in `GitHub/homebrew-samples/3ds/` (never run on this machine, never committed). Each is written as GNU as
source by `source/n3ds_writer` and built again by CodeBreaker's own assembler; its functions (the first 400, in order) are then run in the interpreter, in the original and
in the rebuilt file (`cb_tests --filter machine_n3ds.homebrew`; it skips programs that are not on the machine).

| Program | Console | Source | Licence | Functions | Result |
|---|---|---|---|---|---|
| FBI.3dsx (1.2 MB) | 3DS | Steveice10/FBI (2.6.1) | MIT | 4221 | rebuilds identical (write and build 21 s); 800 of 800 runs equal |
| Anemone3DS.3dsx (1.7 MB) | 3DS | astronautlevel2/Anemone3DS (v3.0.1) | GPL-3.0 | 4399 | rebuilds identical (32 s); 800 of 800 runs equal |
| Anemone3DS.elf (5.3 MB, with debug information) | 3DS (ELF) | astronautlevel2/Anemone3DS (v3.0.1) | GPL-3.0 | 3686 | rebuilds identical (57 s); 800 of 800 runs equal |
| Universal-Updater.3dsx (3 MB) | 3DS | Universal-Team/Universal-Updater (v3.4.1) | GPL-3.0 | 5629 | rebuilds identical (136 s); 800 of 800 runs equal |

`codebreaker reach --judge` on the folder: 14249 functions of the three `.3dsx` files, all verified.

## PlayStation Vita homebrew (the vita lane)

Open-source homebrew with published source, binaries only, kept in `GitHub/homebrew-samples/vita/` (never run on this machine, never committed): the SELF files with
unencrypted, zlib-compressed segments that VitaSDK's tools make. Each is read by `formats/vita` (the segments inflated), written as GNU as Thumb source by
`source/vita_writer` and built again by CodeBreaker's own assembler; its functions (the first 400) are then run in the interpreter, in the original and in the rebuilt file
(`cb_tests --filter machine_vita.homebrew`; it skips programs that are not on the machine).

| Program | Console | Source | Licence | Functions | Result |
|---|---|---|---|---|---|
| eboot_safe.bin (lpp-vita, 3.7 MB) | Vita (fixed-address SELF) | Rinnegatamante/lpp-vita | GPL-3.0 | 84 | rebuilds identical; 168 of 168 runs equal |
| eboot.bin of VitaShell.vpk (1.8 MB, relocatable) | Vita | TheOfficialFloW/VitaShell | GPL-3.0 | 439 | rebuilds identical; 800 of 800 runs equal |

Only the functions the flow reaches by calls are found (callbacks and functions called through pointers are data for now), and many runs end at a 32-bit Thumb-2 instruction the interpreter does not run (ldrd, strd, pld, tbb, NEON).

## PlayStation 3 (the ps3 lane)

No open-source PS3 homebrew binary with readable segments was found: the releases that were looked at (bucanero/ps3loadx v1.1 `EBOOT.BIN`, MIT; jjolano/openps3ftp `openps3ftp.sprx`,
GPL-3.0) are SELF files whose segments are compressed and encrypted, which CodeBreaker refuses (it does not decrypt), and the projects that publish PSL1GHT output publish it
signed. The lane is therefore proved on PowerPC64 ELF files that `engine/machine_ps3_test` builds word by word (the PSL1GHT layout: text at $10000, a function descriptor
`{code, TOC, 0}` in the data at $10000000): they rebuild identical, a descriptor finds its function, an unedited copy judges equal and an edited one diverges. When a plain `.elf`
from PSL1GHT's `make` or an unencrypted SELF of an open-source project is put in `GitHub/homebrew-samples/ps3/` the same writer and lane take it; nothing is claimed for real
programs yet.

## Nintendo Switch homebrew (the switch lane)

Open-source homebrew with published source, binaries only (GitHub releases), kept in `GitHub/homebrew-samples/switch/` (never run on this machine, never committed). Each is read by `formats/nro`, written as GNU as source by `source/switch_writer` and built again by CodeBreaker's own assembler; the first 300 functions are run in the interpreter, in the original and in the rebuilt file (`cb_tests --filter machine_switch.homebrew`; it skips programs that are not on the machine), and `codebreaker reach --judge` verifies them all.

| Program | Console | Source | Licence | Functions | Result |
|---|---|---|---|---|---|
| ftpd-classic.nro (v3.2.1, 519 KB) | Switch | https://github.com/mtheall/ftpd | GPL-3.0 | 956 | rebuilds identical; 956 verified; 600 of 600 runs equal |
| ftpd.nro (v3.2.1, 1.5 MB) | Switch | https://github.com/mtheall/ftpd | GPL-3.0 | 2,357 | rebuilds identical; 2,357 verified; 600 of 600 runs equal |
| NX-Shell.nro (4.01, 8.4 MB) | Switch | https://github.com/joel16/NX-Shell | GPL-2.0 | 14,580 | rebuilds identical (36 s); 14,580 verified; 600 of 600 runs equal |

Only what flow reaches (calls, branches, relocation targets, and stretches that begin with a function prologue) is code; jump tables are not followed. Most sampled runs end at a data abort, because a function is called with made-up pointers.

## Windows x64 (64-bit PE32+, the pe64 writer)

64-bit Windows programs from this machine's own Windows 11 (build 26300) `C:\Windows\System32`, read in place (never copied into the repository),
written as NASM source by `source/pe64_writer` (over `source/x64_writer`) with `codebreaker pe-source` and assembled again by x86asm
(2026-11-26). No C yet (phase 2); the functions are the ones flow reaches from the entry, the exports and `.pdata`.

| Program | Functions | Result |
|---|---|---|
| notepad.exe | 466 | rebuilds identical (8 s) |
| calc.exe | 31 | rebuilds identical (1 s) |
| cmd.exe | 883 | rebuilds identical (9 s) |
| kernel32.dll | 3,233 | rebuilds identical (26 s) |
| user32.dll | not counted | rebuilds identical (39 s) |
| ntdll.dll | 6,313 | rebuilds identical (2 min 13 s) |

### As C, built with Visual C++ x64 and judged (the win64 lane, `codebreaker check`)

The same programs as C (2026-11-26): the Microsoft runtime's functions identified by the x64 signatures (Visual C++ 14.16-14.51,
the Universal CRT) and left to the original, every other function decompiled, compiled on its own with Visual C++ 14.51's x64 `cl`,
linked into one DLL beside the original, and run beside the original's in Unicorn's x64 one function at a time.
Weak: equivalent with a limit (low coverage, a stand-in callee) or only ever faulting alike. Teeth (`check --teeth`): of the
functions deliberately broken, the share caught among those it had called verified, among the weak ones, and in all.

Lane revision 7 (2026-11-26) adds guided inputs (`Win64Lane::guided_inputs`): each run of the original is traced (every
conditional jump, setcc and cmovcc of the function with the comparison before it, and where a compared register was loaded
from), and verify/guided turns each comparison the other way, in an argument, in what imports return, or in the memory the value
was read from (a field behind a pointer argument, a global). Where a run faults on an address it loaded, the place that address
came from gets a pointer of its own, until the run gets past. Every case of a switch (a bound compared before an indirect jump) is
tried. The fixed inputs gain scratch memory of zeros (null pointers, zero counts) and imports that hand back a pointer.

| Program | Functions | Runtime library | Verified | Weak | Unverified | Does not compile | No source | Teeth: verified / weak / all |
|---|---|---|---|---|---|---|---|---|
| notepad.exe, revision 6 | 386 | 24 (Visual C++ 14.38) | 117 | 136 | 65 | 31 | 13 | 62 of 88 (70%) / 32 of 54 (59%) / 94 of 142 (66%) |
| notepad.exe, revision 7 | 386 | 24 | 141 | 90 | 103 | 31 | 13 | 86 of 105 (81%) / 24 of 34 (70%) / 110 of 139 (79%) |
| cmd.exe, revision 6 | 665 | 23 | 143 | 266 | 148 | 65 | 20 | 54 of 88 (61%) / 22 of 47 (46%) / 76 of 135 (56%) |
| cmd.exe, revision 7 | 665 | 23 | 143 | 230 | 185 | 65 | 20 | 61 of 86 (71%) / 18 of 32 (56%) / 79 of 118 (67%) |
| notepad.exe, revision 7 + the C that compiles (2026-11-27) | 386 | 24 | 164 | 94 | 107 | 0 | 13 | |
| cmd.exe, revision 7 + the C that compiles (2026-11-27) | 665 | 23 | 249 | 198 | 176 | 0 | 20 | |
| calc.exe, revision 6 | 22 | 4 | 5 | 5 | 5 | 2 | 1 | |

The "C that compiles" rows (2026-11-27): every decompiled function's C now builds with x64 cl. The C writer (decompile/c_text_x64)
gives an undeclared field's address as the address it is, a callee whose text starts with warnings its result, imports split over
lines and functions passed as values their macros, odd-width partial accesses (3, 5, 6, 7 bytes) and 128-bit products their
helpers, a whole-array assignment as a byte copy, the stack pointer after __chkstk as _AddressOfReturnAddress, a self-call with
other arguments an unprototyped call; the functions that now compile are judged (most verify).

The revision 7 rows were made with a newer decompiler (the tree of 2026-11-26, 11:41 PM), which writes 16 more functions of
notepad and one more of cmd than revision 6's did (the classes add up to 402 and 666: the decompiler's functions beyond
the ones the module's unwind table states). The same build with the guided inputs turned off (`CB_NO_GUIDED=1`) gives
notepad 128 verified, 134 weak, 72 unverified (teeth 78% / 47% / 67%) and cmd 145, 253, 160 (69% / 43% / 61%): the guided inputs
turn 13 of notepad's weak functions into verified ones, and on both programs they mostly find differences no input reached before
(31 more unverified on notepad, 25 on cmd).

### As C++, its classes from the run-time type information (2026-11-26)

A C++ program comes back as C++: `symbols/cpp_classes_x64` reads Visual C++'s x64 RTTI (complete object locators, type descriptors,
hierarchy descriptors, base class arrays) and the virtual function tables after them, and `decompile/cpp_text_x64` declares each of
the program's own classes (not the C++ library's, WRL's, WIL's, ATL's or a template instance's) with its virtual functions in table
order, its data members where its methods read them through `this`, and writes as members the functions of its own table and its
constructors and destructors (a function that stores the class's table into its first argument). The methods are compiled as C++
by the x64 `cl` (/TP), the other functions stay C, and the win64 lane judges both (as in the tree of 2026-11-26, 11:45 PM).
`decompile` writes such a program as one `.cpp` file. The functions the tables hold are decompiled too (a leaf function has no
unwind entry): the 16 more of notepad above.

| Program | Functions | Classes | Methods | Methods verified | Verified | Weak | Unverified | Does not compile | No source |
|---|---|---|---|---|---|---|---|---|---|
| shapes.exe (the test program, `src/symbols/testdata/x64_classes`) | 46 | 5 | 20 | 18 | 14 | 21 | 4 | 0 | 2 |
| SecurityHealthHost.exe (130 KB) | 295 | 6 | 13 | 10 (8, 2 weak) | 131 | 75 | 60 | 24 | 7 |

SecurityHealthHost's classes: CClassFactory and CSurrogate (each on WRL's RuntimeClass, which is not declared), IRefCounted and
ILockable (interfaces of pure virtual functions), CommonUtil::CHResultException, and CError : CHResultException. Of its 13 methods
one diverges (deps-differ) and two do not compile (one a decompiler error its C has too, one a pointer conversion C allows and C++
does not).

## PlayStation 4, PlayStation 5 and Xbox One / Series homebrew (the ps4, ps5 and xboxone lanes)

Open-source homebrew with published source and a stated licence, binaries only (GitHub releases), kept in `GitHub/homebrew-samples/{ps4,ps5,xboxone}/` (never run on this machine,
never committed). Each is read by `formats/orbis` or `formats/xboxone`, written as NASM source by `source/orbis_writer` or `source/xboxone_writer` (over `source/x64_writer`) and built
again with NASM; the functions are then run in Unicorn's x86-64 engine in the original and in a copy with its first byte turned over
(`cb_tests --filter engine.x64.homebrew`; it skips programs that are not on the machine). No retail game, firmware, key or decrypted file is used: encrypted formats are recognised and refused.

| Program | Console | Source | Licence | Functions | Result |
|---|---|---|---|---|---|
| hello_world.elf (OpenOrbis sample, 1.7 MB ELF, named imports) | PS4 | OpenOrbis/OpenOrbis-PS4-Toolchain (release v0.5.3, samples-elf-llvm-18) | GPL-3.0 | 4,916 | rebuilds identical; 10 of 10 judged agree with themselves and 8 diverge when their first byte is turned over |
| hello_world-eboot.bin (the same program through OpenOrbis's own create-fself: an Orbis ELF, imports by NID, in a fake SELF, 1.2 MB) | PS4 | OpenOrbis/create-fself (built from source) over the sample above | GPL-3.0 | 2,949 | rebuilds identical (the ELF the SELF holds); 10 judged, 8 diverge |
| ftpsrv-ps4.elf (v0.22, 147 KB payload) | PS4 | ps5-payload-dev/ftpsrv | GPL-3.0 | 185 | rebuilds identical; 10 judged, 10 diverge |
| ftpsrv-ps5.elf (v0.22, 192 KB payload) | PS5 | ps5-payload-dev/ftpsrv | GPL-3.0 | 488 | rebuilds identical; 10 judged, 9 diverge |
| klogsrv-ps5.elf (v0.9, 136 KB payload) | PS5 | ps5-payload-dev/klogsrv | GPL-3.0 | 387 | rebuilds identical; 10 judged, 9 diverge |
| elfldr-ps5.elf (v0.27, 395 KB payload) | PS5 | ps5-payload-dev/elfldr | GPL-3.0 | 78 | rebuilds identical; 10 judged, 10 diverge |
| moonlight-xbox-dx.exe (3.2 MB PE32+, from the x64 package of moonlight-xbox-dx_1.19.0.0_x64.msixbundle) | Xbox One / Series | TheElixZammuto/moonlight-xbox (release 1.19.0) | GPL-3.0 | 8,771 | rebuilds identical (1 min 40 s); 10 judged, 8 diverge |
| brotlidec.dll (51 KB PE32+, from the same package) | Xbox One / Series | TheElixZammuto/moonlight-xbox (package; Brotli is MIT) | GPL-3.0 package, MIT library | 140 | rebuilds identical; 10 judged, 10 diverge |

`codebreaker reach <folder> --judge` over the same files (`--max-size 20000` for the 13 MB package): every function of every program is verified, because the rebuild is the original (PS4: 12,029 functions of five programs and OELFs in `ps4/fself`, 185 of ftpsrv-ps4; PS5: 953 of three payloads; Xbox One / Series: 8,771 of the package's executable, 1 min 16 s).

A plain payload ELF does not say which console it is for (the PS5 payload SDK links one ELF for both and the PS5 runs PS4 programs), so the console is told by the file name; both
lanes are the same model. The fake SELFs were made on this machine with OpenOrbis's own `create-fself` (built from its GPL-3.0 source), from the sample ELFs above: no signed or
retail SELF is read. Only what flow reaches is code (calls, the entry, symbols, initialisers, and the code addresses relocations store in data); vtables are `db` rows, not labels.
