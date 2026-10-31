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
-- is in docs/STATUS.md); the 7 Setup programs; 212 of 215 VxDs build, 40 boot-set VxDs load in the running system.
`vxd boot` (2026-10-28, docs/VXD.md): VMM32.VXD (build_w3) and DOSMGR, IFSMGR, SHELL, VDMAD, VFAT,
VMPOLL, VPICD, VTD, VTDAPI, VXDLDR (build_le), each loaded and started, boot an installed Windows 95
to the desktop; DRVSPACX and VDHCP rebuilt too but not loaded on that machine.

## Open-source C (ground truth for the C lanes)

Compiled with OpenWatcom and taken back to C by `tools/oss_bench.py`, their own source the
answer: cJSON, zlib, sds, tiny-AES-c, inih, Lua, miniz.

## XNA / C# (.NET lane)

| Program | Platform | Result |
|---|---|---|
| Arcadecraft (Firebase Industries, 2011) | Xbox 360 Indie Game (XNA 4.0) | `xna`: 6 assemblies to C# (583 files, 125 000 lines: the game, its two own libraries, SunBurn, DPSF, EasyStorage); 610 content files -- 434 textures and 6 fonts as PNG, pixel for pixel as Arcadecraft's own pipeline made them, 170 other types catalogued; all 106 XACT cues resolved to their waves (as the hand port's own map, 97 of 97). `xna port` with its overlay (the deobfuscated SunBurn, the reconstructed shaders, its port runtime: ../Arcadecraft) builds and runs to the title screen through the real SunBurn renderer. Without the overlay, the generic lane now deobfuscates SunBurn (de4dot finds Spices.Net) and the port compiles with nothing written by hand; it runs until SunBurn's own licence check ("Product not activated"), which CodeBreaker does not bypass -- the overlay is the way past it. Its shaders exist only as Xbox microcode |
| Block World (Hazzah Games) | Xbox 360 Indie Game (XNA 4.0) | `xna`: 16 assemblies to C# (the game and its Hazzah engine); 210 textures, 3 fonts, 150 sound effects and the XACT sounds (XMA2 decoded), 6 songs, and all 20 effects recovered as HLSL from their Xbox microcode (19 .xnb and the copy of XNA's stock BasicEffect it reads as a file: one technique per ShaderIndex). 105 files of the package are empty (an earlier extraction filled them with the next file's blocks: fixed). `xna port`, nothing written by hand: builds (the compile loop's MonoGame differences, effect bytes passed through the runtime, the controller read with the keyboard too) and plays its menus: title, loading, the main menu and the world customizer, the 3D block world rendering behind them through the recovered shaders and byte-swapped models. `--check-content`: 461 of 463 .xnb load (all 41 models with plausible buffers, 19 effects, 210 textures, the cube map, 150 sound effects, 6 songs, 22 avatar animations, 8 particle effects); the 2 left are models whose textures the game never shipped. Entering a world not yet driven by a scripted run (its menu needs a person) |

## Java

Minecraft (alpha, PC): its classes as C++03 through the Java lane, built and running.
