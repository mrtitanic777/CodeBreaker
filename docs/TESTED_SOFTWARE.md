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

| Program | Platform | Result |
|---|---|---|
| Arcadecraft (Firebase Industries, 2011) | Xbox 360 Indie Game (XNA 4.0) | `xna`: 6 assemblies to C# (583 files, 125 000 lines: the game, its two own libraries, SunBurn, DPSF, EasyStorage); 610 content files -- 434 textures and 6 fonts as PNG, pixel for pixel as Arcadecraft's own pipeline made them, 170 other types catalogued; all 106 XACT cues resolved to their waves (as the hand port's own map, 97 of 97). `xna port` with its overlay (the deobfuscated SunBurn, the reconstructed shaders, its port runtime: ../Arcadecraft) builds and runs to the title screen through the real SunBurn renderer. Without the overlay, the generic lane now deobfuscates SunBurn (de4dot finds Spices.Net) and the port compiles with nothing written by hand; it runs until SunBurn's own licence check ("Product not activated"), which CodeBreaker does not bypass -- the overlay is the way past it. Its shaders exist only as Xbox microcode |
| Block World (Hazzah Games) | Xbox 360 Indie Game (XNA 4.0) | `xna`: 16 assemblies to C# (the game and its Hazzah engine); 210 textures, 3 fonts, 150 sound effects and the XACT sounds (XMA2 decoded), 6 songs, and all 20 effects recovered as HLSL from their Xbox microcode (19 .xnb and the copy of XNA's stock BasicEffect it reads as a file: one technique per ShaderIndex). 105 files of the package are empty (an earlier extraction filled them with the next file's blocks: fixed). `xna port`, nothing written by hand: builds (the compile loop's MonoGame differences, effect bytes passed through the runtime, the controller read with the keyboard too) and plays its menus: title, loading, the main menu and the world customizer, the 3D block world rendering behind them through the recovered shaders and byte-swapped models. `--check-content`: 461 of 463 .xnb load (all 41 models with plausible buffers, 19 effects, 210 textures, the cube map, 150 sound effects, 6 songs, 22 avatar animations, 8 particle effects); the 2 left are models whose textures the game never shipped. Entering a world not yet driven by a scripted run (its menu needs a person) |

## Java

Minecraft (alpha, PC): its classes as C++03 through the Java lane, built and running.

Minecraft Alpha (decompiled source), as C#: NoiseGenerator, NoiseGeneratorPerlin, NoiseGeneratorOctaves and
MathHelper through the javac front end and the C# back end (translator phase 7), judged beside the JVM with a
driver of seeded world-generation calls: the same output, java.util.Random and Java's text for doubles included
(the C# kit's tests under tests/translate).

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
two states, and runs the whole program from its entry point for three million instructions). The RSP and the RDP are not run: the programs
that draw with them do so with their registers and display lists as logged writes.

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
