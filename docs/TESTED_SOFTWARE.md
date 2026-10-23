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
desktop; the 7 Setup programs; 212 of 215 VxDs build, 40 boot-set VxDs load in the running system.

## Open-source C (ground truth for the C lanes)

Compiled with OpenWatcom and taken back to C by `tools/oss_bench.py`, their own source the
answer: cJSON, zlib, sds, tiny-AES-c, inih, Lua, miniz.

## Java

Minecraft (alpha, PC): its classes as C++03 through the Java lane, built and running.
