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
| Icarus | ZX Spectrum (tape) | MmmPT/icarus-zx-spectrum | see its ASSET-LICENSE | 95 | rebuilds identical, verified |

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
