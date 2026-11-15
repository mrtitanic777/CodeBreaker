# Any software, any platform

CodeBreaker's aim: take **any** program -- from 8-bit computers and arcade boards to every games
console, PC and phone of today -- back to source that is readable, compiles, and behaves as the
original did. One tool, one pipeline, one way of judging; a platform is what plugs into it.

![Platforms and lanes](architecture/06-platforms-and-lanes.svg)

## The layers

Everything after the CPU front end is shared. A new platform adds pieces to the layers below the
line and nothing above it.

| Layer | What it is | Per platform? |
|---|---|---|
| Unpack | containers: tapes, disks, cartridge and arcade ROM sets, discs, XEX, ELF, PE, NE, LE, APK, IPA, Mach-O | yes: a reader in `formats/` |
| Identify | which CPU, OS, SDK, compiler and runtime made it -- and, per function, whether it was written in assembly or compiled | yes: signatures in `symbols/` |
| CPU front end | the instruction set as SLEIGH, lifted to p-code | yes: a processor spec in `native/decompiler` (upstream Ghidra already has most: 6502, Z80, 68000, MIPS, SuperH, ARM, AArch64, 8051, AVR, PIC, Dalvik -- the work is compiling them in and checking them, not writing them) |
| ---------------- | ---------------- | ---------------- |
| Shared form | p-code, one intermediate code for every CPU | no |
| Analyze and name | functions, types, structures, calls into the SDK and hardware | no (fed by the platform's names) |
| Back to source | in the language it was written in: assembly source for hand-written code (most 8-bit, arcade and early console code), C/C++ for compiled code, source for bytecode | no |
| Rebuild | the platform's own compilers and SDKs, or modern ones | yes: a build profile in `build/` |
| Judge | run beside the original on a model of the platform | yes: a platform model in `emulate/` |

## Where it stands

| Family | CPUs | Platforms | Unpack | CPU spec | Platform model | Judged |
|---|---|---|---|---|---|---|
| x86 | 8086 - x86-64 | DOS, Windows 3.x/95, drivers | yes | yes (x86-64 spec present, unused) | Unicorn: DOS, Win16, Win32, VxD | yes |
| PowerPC | 32/64-bit, VMX128 | Xbox 360, PowerPC images | yes (XEX, STFS, discs, GoD, FATX) | yes (Xenon: VMX128) | own interpreter | yes |
| Bytecode | JVM | Java source to C++03 | n/a | n/a | the JVM | yes |
| Bytecode | .NET CIL | XNA games (Xbox 360 Indie / Arcade, PC): `xna`, then `xna port` | yes (STFS, folders; XNB content, XACT banks) | n/a (ILSpy decompiles CIL to C#) | MonoGame (xna/runtime: content incl. Xbox byte order for buffers and textures, effects recovered as HLSL from Xenos microcode and compiled by mgfxc, players, storage, XACT, the controller with the keyboard; generated XNA API stubs; de4dot for obfuscated assemblies) | builds and runs (no per-function judge) |
| 8-bit | 6502 (2A03) | NES | yes (iNES, NES 2.0) | yes (checked: 151/151) | own 6502 interpreter | yes |
| 8-bit | 6502 (6507) | Atari 2600 | yes (raw .a26 / .bin: 2 KB, 4 KB, and 8 / 16 / 32 KB with F8 / F6 / F4 bank switching by the usual hotspots; the 13-bit address mirrors folded) | own table (the NMOS 6502) | own 6502 interpreter over the 6507's map: 13 address lines, 128 bytes of RAM with the stack in its mirror, TIA and RIOT registers named and logged, bank switches logged | yes (dasm source: `ORG`/`RORG` per bank, TIA and RIOT names, `.w` for absolute zero-page forms; CodeBreaker's own dasm-syntax assembler rebuilds it) |
| 8-bit | 6502 (Sally) | Atari 7800 | yes (.a78 header v1-4; linear ROMs of 4-48 KB at the top of memory, SuperGame banks of 16 KB; Activision, absolute, banked-RAM, POKEY and YM2151 boards not modelled, 2600 mode not modelled) | own table (the NMOS 6502) | own 6502 interpreter over the 7800's map: TIA, MARIA and RIOT registers named and logged, 4 KB RAM with its zero-page and stack windows, SuperGame bank select logged | yes (dasm source, the header as data; the 6502C's HALT stalls are not modelled) |
| 8-bit | 65SC02 | Atari Lynx | yes (.lnx cartridges, BS93 `.o` objects; the cartridge's encrypted first stage is decrypted and the loaders run to find what they load) | own table (the NMOS set and the 65SC02's BRA, STZ, TRB/TSB, PHX/PHY/PLX/PLY, INC A/DEC A, `(zp)`, `JMP (abs,X)`) | own 65SC02 interpreter (Suzy and Mikey registers named and logged) | yes (ca65 source, `ld65` configuration) |
| 8-bit | Z80 | Master System, Game Gear, SG-1000, SC-3000 | yes (.sms .gg .sg .sc; Sega mapper on the Master System and Game Gear, none on the SG-1000 / SC-3000) | own table (every opcode, undocumented included) | own Z80 interpreter | yes (WLA-DX source) |
| 8-bit | Z80 | ZX Spectrum, MSX | yes (.sna .z80 .tap; .rom up to 48 KB) | own table | own Z80 interpreter | yes (sjasmplus / pasmo source) |
| 8-bit | Z80 | ColecoVision | yes (.col, or .rom/.bin with the $AA55 header; up to 32 KB, larger mappers refused) | own table | own Z80 interpreter with the console's memory map (BIOS stubbed and its calls logged by name, RAM at $6000, VDP and sound ports logged) | yes (sjasmplus / pasmo source) |
| 8-bit | Z80 | Amstrad CPC | yes (.dsk standard and extended: the AMSDOS directory, the best binary; .sna 64 KB snapshots; .bin with an AMSDOS header; tapes not yet) | own table | own Z80 interpreter with the firmware jumpblock stubbed and its calls logged by name, gate array / CRTC / PPI ports logged with all 16 bits | yes (sjasmplus / pasmo source) |
| 8-bit | SM83 | Game Boy, Game Boy Color | yes (.gb .gbc; header, MBC) | own table | own SM83 interpreter | yes (RGBDS source) |
| 16/32-bit | 68000 | Genesis / Mega Drive | yes (.md .bin .gen .smd; header, vectors) | own table (every 68000 instruction) | own 68000 interpreter | yes (Motorola-syntax source) |
| 16/32-bit | 68000 (+ Z80 sound) | Neo Geo (AES / MVS) | yes (the P-ROM: plain, word-swapped, or the even and odd halves joined; `.neo` containers, whose M-ROM is kept; "NEO-GEO" header at $100, the four callbacks of its jump table, vectors; the second megabyte in the $200000 bank window) | own table (the Genesis's) | own 68000 interpreter with the Neo Geo's memory map: work and palette RAM, the system ports and memory card logged, the LSPC's VRAM modelled, the BIOS stubbed with each call logged (SYSTEM_IO, FIX_CLEAR ...) | yes (Motorola-syntax source; the Z80 M-ROM comes back as sjasmplus / pasmo source, not judged) |
| 16/32-bit | 68000 x 2 | Sega CD / Mega-CD | yes (.iso 2048- and .bin 2352-byte sectors; the boot header, the main CPU's security code and IP at $FF0000, the sub CPU's SP at $6000 with its jump table; only the 32 KB system area) | own table (the Genesis's) | own 68000 interpreter with each CPU's memory map: the Genesis's with the gate array and word RAM for the main CPU; PRG-RAM, word RAM, PCM and the gate array for the sub CPU, whose CD BIOS calls are logged | yes (Motorola-syntax source, one file holding both programs at their own addresses) |
| 8/16-bit | 6502, 65816, Z80, 68000, SPC700 | Atari 5200 and 8-bit, SNES, C64, Apple II, arcade | no | 6502, Z80, 68000 yes | 6502, Z80, 68000 yes | no |
| x86, console | Pentium III | original Xbox (XBE) | yes (as a PE image; kernel imports named) | yes | Unicorn (the 32-bit lane) | yes |
| 32-bit | MIPS R3000A (MIPS I, little-endian; GTE decoded) | PlayStation | yes (PS-X EXE; header, load address, entry) | own table (every MIPS I instruction, COP0, the GTE's commands) | own R3000A interpreter (delay slots, load delay, BIOS calls and devices logged; the GTE's registers, not its commands) | yes (GNU as source) |
| MIPS, SuperH | R4300, R5900, SH-2, SH-4 | N64, PS2, Saturn, Dreamcast, arcade | no | no | no | no |
| PowerPC, Cell | Gekko, Broadway, Cell PPU/SPU | GameCube, Wii, PS3 (the Xbox 360 is the PowerPC row above) | no | PPU yes, SPU no | no | no |
| 32-bit | ARM7TDMI (ARMv4T: ARM and Thumb state) | Game Boy Advance | yes (.gba; header, logo, checksum, entry branch, ROM at $08000000) | own table (every ARMv4T ARM and Thumb instruction) | own ARM7TDMI interpreter (both states, banked modes, memory map, BIOS calls logged) | yes (GNU as `.syntax unified` source) |
| 32-bit | ARM946E-S (ARMv5TE: ARM and Thumb state, BLX, CLZ, saturating and 16-bit multiplies, LDRD/STRD, PLD, coprocessor 15) + ARM7TDMI | Nintendo DS | yes (.nds; header, logo and CRC-16s, both programs at their RAM addresses, FNT, FAT, overlay tables, banner) | own table (the ARMv4T set and the ARMv5TE additions, selected by an `Arch` parameter) | own ARM interpreter, `System::Nds9` and `System::Nds7` (ARMv5 interworking loads, CP15 and the TCMs, main RAM, shared WRAM, I/O registers and BIOS calls logged) | yes (GNU as `.syntax unified` source, one file with a part for each processor) |
| x86-64, ARM | x86-64, ARM9, ARMv7, AArch64 | PC 64-bit, DS, 3DS, Xbox One, PS4/5, phones, Switch | no | no | no | no |
| Firmware | 8051, AVR, PIC, ARM Cortex-M, Xtensa | microcontrollers, BIOS/UEFI, device firmware | no | no | no | no |
| Bytecode | Dalvik, .NET IL, scripts | Android, .NET, Lua and others | no | no | no | no |

## Order of work

1. **Platform-neutral seams first.** Today the platform is often implied (x86, Windows). Make it
   explicit: a platform descriptor (CPU spec, container reader, OS/SDK names, build profile,
   platform model) that every stage asks, so the next platform adds a descriptor, not branches.
2. **One small 8-bit platform end to end** -- the proof that the seams hold: 6502 (NES or Apple II,
   with homebrew and open-source programs as ground truth, as the OSS benchmark does for C).
   Its source is assembly that rebuilds (ca65), as for DOS and drivers; C only for cc65-built
   programs. Unicorn has no 6502, Z80 or SuperH, so the platform model comes from an open-source
   emulator core, driven the way `emulate/` drives Unicorn.
3. **Arcade**: ROM sets (MAME's set descriptions say which CPU and where each ROM loads) on the
   same 8/16-bit CPUs; Z80 and 68000.
4. **Xbox and Xbox 360 completed**: XEX (native/xex, docs/XEX.md) + the PowerPC already here; the
   original Xbox is x86 already handled -- only the XBE reader and the kernel's imports are new.
5. **64-bit and ARM**: x86-64 PE/ELF, AArch64 ELF/Mach-O; then Android (Dalvik) and iOS.
6. **The rest of the consoles**, family by family: MIPS (PS1 done: R3000A; N64, PS2), SuperH (Saturn,
   Dreamcast), Cell (PS3), x86-64 consoles.

Each step is judged the same way as everything else: rebuilt, run beside the original, the same
behaviour -- with open-source and homebrew programs, whose authors' source we have, as the
ground truth for how close the source comes back.
