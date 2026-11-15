# Lanes: what a platform provides, and how to add one

A **lane** is the path one kind of program takes from its file to judged source: written back as source, rebuilt, and
each function run beside the original's. A **platform** is what a program was made for. This document says what a
platform provides today, file by file, checked against the code (2026-11-14); then how to add one (a checklist and a
worked walk-through of the Game Boy lane); then the registration object (section 6): a lane is one plug-in, registered in one
place, and the shared lists (platform enum, probes, `info` text, extensions, lane kit chains) iterate the registry.

Paths are under `native/cb/` unless they start with `docs/`. See also `docs/PLATFORM_DESCRIPTOR.md` (why the platform
is one record) and `docs/PLATFORMS.md`.

## 1. The pieces

Two kinds of lane exist, and the difference decides how much a new platform has to write.

* **Machine lanes** (`nes`, `sms`, `gb`, `genesis`, `ps1`, `zx`, `msx`, `coleco`, `cpc`): programs written in assembly come back as
  assembly. One shared class, `verify::lanes::ImageLane`, is the lane; a platform gives it a *spec*. This is the path
  for a new console or home computer, and the rest of this document is about it.
* **Bespoke lanes** (`pe`, `ne`, `le`, `powerpc`, `dos`, `java`): each is its own `verify::lanes::Kind` subclass
  (`verify/lanes/win32.hpp`, `win16.hpp`, `driver.hpp`, `powerpc.hpp`, `dos.hpp`, `java.hpp`) wired in
  `engine/lane_kit*.cpp` with compilers, a decompiler and tools. A new compiled-language platform writes a `Kind`.

What a machine platform provides:

| Piece | Where | What it is |
|---|---|---|
| Format loader | `formats/<name>.{hpp,cpp}` | `probe(path)`, `read(path)`, the 64 KB (or RAM) image the CPU sees at power-on (`memory()`), function starts found by following code (`functions(Decoder)`) |
| Platform description | `platforms/<name>.{hpp,cpp}` | one `Platform` record: name, title, CPU facts, calling convention, emulator, toolchain (built from a `MachineSpec` by `platforms::machine`) |
| CPU decoder | `decompile/<cpu>.{hpp,cpp}` | the instruction table: `decode`, `text`, `assemble`, flow of each instruction |
| Flow decoder | `source/isa.{hpp,cpp}` | adapts the CPU table to `formats::Decoder` (size, how control leaves, where it goes) for the format's function finder and the writer |
| Assembler | `build/<cpu>_asm.{hpp,cpp}` | text to bytes: `build(source, size, folder)` for the Game Boy (a directive layer over the table's `assemble`) |
| Machine model / interpreter | `emulate/<cpu>.{hpp,cpp}` | `Bus` with device ranges that log accesses, `Cpu`, `run(bus, entry, options)` to a `RunResult` |
| Source writer | `source/<name>_writer.{hpp,cpp}` | the image as assembly source that builds back into it; reports `image_writer::Written` |
| Lane registration | `engine/machine_<name>.cpp` (`engine/machine_builtin.cpp` for the lane kit's first ten) | ONE `register_lane({...})`: the platform, the probe, the `ImageLaneSpec` (starting states, `effects`: load, run, collect), the `info` text, the extensions `reach` takes (section 6); named once in `engine/lane_list.cpp` |

The code lives in the area libraries `cb_formats`, `cb_platforms`, `cb_decompile`, `cb_build`, `cb_emulate`,
`cb_source`, `cb_engine` (and `cb_verify` for `ImageLane`); see `CMakeLists.txt` for the links (the Game Boy assembler
sits in `cb_build` and links `cb_decompile` for its table, comment on line 140).

### 1.1 Format loader (`formats/`)

`formats/gb.hpp` is the model. It declares `parse(ByteView)`, `read(path)` returning a `Rom` (data, header facts,
`memory()`, `bank(i)`, `functions(const Decoder&)`) and `probe(path)`. `probe` must say false for anything that is not
the format, never throw for an unreadable file (the platform registry also catches `IoError` and `FormatError`:
`platforms/package.cpp`, `Registry::recognises`). `functions` follows code from the entry points with
`formats::find_functions` (`formats/code_scan.hpp`): the format knows the vectors and the address layout
(`CodeSpace`), the caller supplies the CPU as a `formats::Decoder` (`std::function<optional<DecodedInsn>(ByteView, pc)>`,
flow is one of `Next, Branch, Jump, Call, Return, Indirect, Halt`).

### 1.2 Platform description (`platforms/`)

`platforms/package.hpp`: a `Platform` is data (id, `name`, `title`, `family`, `source`, `lane`, `Cpu`, `Loader`,
`Convention`, `Emulator`, `Compiler`). A machine fills a `MachineSpec` and calls `platforms::machine(spec)`
(`package.cpp`), which sets `name = id_text(id)`, `source = Asm`, `lane = name`, `steps = Lanes`, a register
convention (`InRegisters`, caller pops) and `emulator = {OwnInterpreter, true}`. `name` is what results are filed
under in the function store, and it is the same string as the lane's `Kind::name()`.

A machine has no entry in `enum class Id`: its descriptor says `Id::Machine` (`MachineSpec` has a `name` and no id), and the
registry tells machines apart by name. Only the bespoke platforms (Java, Xbox 360, the original Xbox, Win32, Win16, VxD, DOS) are
in the enum, with their probes in `Probes` (`platforms/package.hpp`). A machine's probe is `Platform::recognise`, which its
registration object sets (section 6). Platforms are tried in the order they were added -- the order of the list in
`engine/lane_list.cpp`, all machines after the Xbox and before Win32 -- and the first that recognises a file is its platform
(`Registry::identify`).

Who builds the registry: `coverage/reach_command.cpp`, `standard_registry()` fills `Probes` for the bespoke platforms, adds
their descriptors, then adds `engine::lane_platform(lane)` for every registered lane. That is the only production
construction (also used by `ui/mcp_symbols.cpp`).

### 1.3 CPU decoder (`decompile/`)

`decompile/sm83.hpp` is the table for the Game Boy's CPU, generated from the opcode fields:
`pattern_of(Page, opcode)`, `decode(code, at, pc) -> optional<Insn>`, `text(insn, labels)` (RGBDS syntax),
`assemble(line, pc, labels) -> Bytes`, `flow_of(pattern)`. One table serves the disassembler, the assembler and the
interpreter, so they cannot disagree. Capstone (`platforms/decoder.hpp`, `Arch::X86` and `Arch::Ppc` only) is for
processors that have an engine; the console CPUs use CodeBreaker's own tables.

`source/isa.hpp` is "the one place" that turns a table into `formats::Decoder` (`sm83_decoder()`) and into a
`build::Encoder` (`z80_encoder()`, `m68k_encoder()`, `mips_encoder()`; the Game Boy and 6502 have their own layers).

### 1.4 Assembler (`build/`)

`build/sm83_asm.hpp`: `Bytes build(source, size, folder)`, two passes, RGBDS directives (`SECTION`, `db`, `dw`, `ds`,
`INCBIN`, `DEF`, labels). It throws `AsmError` with the line. The writer must emit text this assembler reads; a
program that assembles back to the original bytes is the strongest evidence the writer is right.

### 1.5 Machine model and interpreter (`emulate/`)

`emulate/sm83.hpp`: `Bus` (the Z80 module's: 64 KB plus mapped device ranges, every access to a range logged in
order), `Cpu`, `RunOptions` (registers, `max_steps`), `RunResult` (registers, `io` log, `exit()` = "return" |
"stopped: why" | "limit"), `run(bus, entry, options)`: pushes a return address nothing returns to, runs to the
function's `RET`. A run belongs to one thread. `record(RunResult)` gives the `emulate::Run` record the judge of the
bespoke lanes compares; the machine lanes compare their own `effects` (below).

### 1.6 Source writer (`source/`)

`source/gb_writer.hpp`: `reconstruct(rom_file, folder)` and `write(rom, name, folder)` return
`image_writer::Written`. `source/image_writer.hpp` is the shared machinery; a writer implements two interfaces for
its instruction type and calls the shared steps:

* `CodeModel<Insn>`: `decode`, `size`, `target`, `inside`, `fits`, `follows`, `stops` (`GbFlow` in `gb_writer.cpp`);
* `Listing<Insn>`: how one stretch of the image is written (instruction text, data lines, the header's named
  fields) (`GbListing`);
* then `walk` (what flow reaches), `name_functions`, `make_labels`, `emit` (one `translate::l2::Assembly`: each
  function a `MachineFragment`), `finish`, and `assembled(r, build_lambda, original, built_path)`, which assembles
  and sets `r.same` when the bytes equal the original.

Code is what flow reaches; everything else is data. Function names are the vector's (`entry`, `vblank`) or
`sub_<addr>`; I/O registers get the hardware's own names (`registers()` in `gb_writer`, from Pan Docs).

### 1.7 The lane: `ImageLane` and what a machine says to it

`verify/lanes/kind.hpp` defines the contract every lane meets (`Kind`: `name()`, `profile(path)`,
`prepare(path, work)`, `build(prepared, path, work)` returning `Built{items, judge, missing}`, with `judge(name)`
thread-safe). `verify/lanes/image_lane.hpp` implements it for every machine; a machine supplies an `ImageLaneSpec`:

| Field | Meaning |
|---|---|
| `kind` | the name results are filed under (= `Platform::name`) |
| `rev` | bump when what a verdict depends on changes (it is part of the profile digest) |
| `states` | the starting states of each run (`State` = map of register name to value, plus `fill`, a seed for RAM) |
| `effects(subject, entry, state)` | load the program, run the function at `entry`, return what it showed (`Effect` = number, text, bytes, or lines) |
| `what` | a name for each place in the effects, so a divergence says which ("RAM", "flags") |
| `start(state)` | the state as a verdict's detail says it |
| `writer` | the platform's `Writer`: `(module, out_dir) -> lanes::Written` |
| `subjects` | optional: the two programs `effects` runs (default: original path and the built file) |
| `key_digits` | hex digits of an address in the store's keys (4 default; `genesis` 6, `ps1` 8) |

Judging (`ImageLane::judge_function`): if the rebuilt program is byte-identical, every function is Equivalent
("the cartridge built from this source is the original"); else each function runs in both programs from every state,
and the first differing place makes the verdict Divergent with that place's name.

`engine/lane_kit_machines.cpp` holds the per-machine pieces. For the Game Boy: `gb_state(...)`, `gb_effects(...)`,
the `name == "gb"` branch of `LaneKit::machine_lane`. `gb_effects` reads `formats::gb::read(path).memory()`, seeds work
RAM from `fill` (`machine_fill`), builds an `emulate::sm83::Bus` with the machine's devices (LY reads as in
vblank, bank-select writes logged), runs `emulate::sm83::run`, and returns end, A, F, BC, DE, HL, RAM and the I/O log.
What a *machine* is, in the judge's terms, is exactly this: which memory is compared, which hardware answers, which
accesses are logged.

## 2. How a file finds its lane

All of it is deterministic and in these places.

**`codebreaker reach <path>`** (`coverage/reach.{hpp,cpp}`, `coverage/reach_command.cpp`)

1. `Registry::identify(path)` tries the platforms in the order they were added (`engine/lane_list.cpp` for the machines); `identify_with(registry)`
   (`reach.cpp`) turns the `Platform` into `PlatformInfo{title, name, family, container, has_lane}` where
   `has_lane = platform.lane.has_value()`. A file no platform takes is "unknown"; a file `gap_of()` names (GBA, N64,
   NDS ...) is a counted gap, a platform with no lane yet.
2. With `--judge`, each picked program is measured by a process of its own: the argv is `codebreaker measure <path>`
   (`reach.cpp`, near line 112), stopped after `--timeout`. The tally comes from the function store.

**`codebreaker measure <module>`** (`verify/measure.cpp`, `source/pipeline.cpp`)

1. `World::reconstructor().run(module)`; `World::kinds()` calls `LaneKit::add_lanes(kinds)`
   (`engine/lane_kit.cpp`), which adds every lane that can be made on this machine (the machines need no external
   tools).
2. `Reconstructor::kind_of(path)` asks `Services::kind_of_file`, set by `LaneKit::reconstruct_services`
   (`lane_kit.cpp`): `corpus.lane(p)` for PE/NE/LE/DOS; otherwise Java; otherwise
   `kit_detail::machine_of_file(p)` (`lane_kit_machines.cpp`): the first registered lane whose probe takes the file,
   in the order of `engine/lane_list.cpp`.
3. `Kinds::find(name)` gives the lane; `prepare` writes the source, `build` assembles it, `judge` per function; the
   candidates go to the function store (`engine/function_store`).

Judging also runs in worker processes (`lane_kit_workers.cpp`): a worker makes its own `LaneKit` and asks
`LaneKit::lane_named(name)`, so a lane must be reachable through `lane_named` (machine lanes are, via `machine_lane`).

The places that used to list the machine kinds by name ask the registry now (`engine/lane_registry.hpp`):
`source/pipeline_module.cpp` (`written_as_assembly`: the module is written as an assembly file), `ui/mcp_standard.cpp`
(`verifier_profiles`: the kinds the ledger reports settings digests for), `engine/lane_kit.cpp` (`add_lanes`),
`engine/lane_kit_machines.cpp` (`machine_of_file`) and `coverage/reach.cpp` (the extensions `reach` looks for).

## 3. Tests: how they are added

Tests are in-tree (`core/testing.hpp`): `CB_TEST("area.module.case", [] { CB_CHECK(...); })` in a `<name>_test.cpp`
beside the code. `CMakeLists.txt` `cb_area(<dir>)` globs `<dir>/*.cpp`, excluding `*_test.cpp` and `*_main.cpp`, into
the static library `cb_<dir>`, and links every `*_test.cpp` into the one `cb_tests` program. So a new file in an
existing area needs no CMake edit; a new area is one `cb_area(dir)` line plus its `target_link_libraries`. Run an area
or a module with `cb_tests.exe --filter formats.gb` (a substring of the test name), from the repo root; an area alone
builds as `cb_tests_<area>` (`tools\build_cb.cmd area <name>`). Inputs that are not in the repository
(commercial or large homebrew ROMs) are found with `cb::testing::local_input(...)` / `beside_checkout(...)` and the
test says it skipped when they are missing (the machines' lane tests use `homebrew-samples`,
`docs/TESTED_SOFTWARE.md`).

A machine lane's tests, as the Game Boy has them: `formats/gb_test.cpp`, `decompile/sm83_test.cpp`,
`build/sm83_asm_test.cpp`, `emulate/sm83_test.cpp`, `source/gb_writer_test.cpp`, `platforms/machines_test.cpp`
(`platforms.gb.descriptor`), `engine/lane_kit_machines_test.cpp` (the lane: a cartridge comes back identical; one
with an edited function is run in both), `source/writers_homebrew_test.cpp` (a real game, µCity, rebuilds identical).
`verify/lanes/image_lane_test.cpp` tests `ImageLane` itself with a fake machine and is the cheapest model of a lane test.

## 4. Worked walk-through: the Game Boy / SM83 lane

Every file the lane touches, in the order the data flows. **New** means the platform had to write it; **edit** means it
is a line in a file that also serves other platforms.

| # | File | Role | |
|---|---|---|---|
| 1 | `decompile/sm83.{hpp,cpp}` + `decompile/sm83_test.cpp` | the CPU table: decode, text, assemble, flow | new |
| 2 | `source/isa.{hpp,cpp}` | `sm83_decoder()` adapts the table to `formats::Decoder` | edit |
| 3 | `formats/gb.{hpp,cpp}` + `formats/gb_test.cpp` | header, logo/checksum, `memory()`, `functions()`, `probe()` | new |
| 4 | `build/sm83_asm.{hpp,cpp}` + `build/sm83_asm_test.cpp` | RGBDS-subset assembler | new |
| 5 | `emulate/sm83.{hpp,cpp}` + `emulate/sm83_test.cpp` | the CPU's interpreter over the Z80 module's `Bus` | new |
| 6 | `source/gb_writer.{hpp,cpp}` + `source/gb_writer_test.cpp` | the cartridge as `.asm`, assembled and compared | new |
| 7 | `platforms/gb.{hpp,cpp}` + `platforms/machines_test.cpp` | the `Platform` descriptor | new |
| 8 | `engine/machine_builtin.cpp` | `register_gb_lane()`: the one registration object (platform, probe, `info` text, extensions) | new |
| 9 | `engine/lane_list.cpp` | `register_gb_lane();` and its include: the one list of lanes | edit |
| 10 | `engine/lane_kit_machines.cpp` | `gb_state`, `gb_effects`, the `name == "gb"` spec branch (a lane after the first ten puts these in its own `engine/machine_<name>.cpp` and gives `make_spec`) | edit |
| 11 | `engine/lane_kit_machines_test.cpp`, `source/writers_homebrew_test.cpp` | the lane's and a real game's tests | edit |

`CMakeLists.txt` needs no change (the files are globbed into existing areas). Since the registration object (section 6) a lane edits one
shared file, `engine/lane_list.cpp`, by one line; the Game Boy's spec still sits in the lane kit's own `machine_lane`, which a new lane does not touch.

The flow for `codebreaker measure game.gb`: `kind_of_file` -> `machine_of_file` -> `formats::gb::probe` (row 3) gives
"gb"; `Kinds::find("gb")` is the `ImageLane` made from row 10's spec; `prepare` calls `gb_writer::reconstruct`
(row 6), which reads the image (3), finds function starts with the SM83 decoder (2, 1), writes assembly, and
assembles with `sm83_asm::build` (4); `build` records whether the bytes are the original; `judge(name)` is either
"identical" or runs the function in both programs with `gb_effects` over `emulate::sm83` (5) from the four
starting states and compares end, A, flags, BC, DE, HL, RAM and the I/O log.

**The Game Boy Advance lane (ARM7TDMI)** is the third (2026-11-15): `formats/gba`, `decompile/arm`, `build/arm_asm`, `emulate/arm`, `source/gba_writer`, `platforms/gba` and a `gba` branch in `lane_kit_machines.cpp`. What is new against the others: the processor has two instruction sets, so an address carries the state in its lowest bit (odd: Thumb) through the format's function finder (`source/isa`: `arm_decoder`), the writer's flow (a `bx` is followed when the register was just loaded from a literal pool or made from the pc) and the assembler (`.arm`/`.thumb`; the encoder is given the state as the address's bit 0); a function's key in the result and in the store is odd for Thumb. The writer checks each instruction's text against the assembler and keeps the bits (`.hword`/`.word`) of those it would write differently.

**The Nintendo DS lane (ARM946E-S + ARM7TDMI)** reuses the GBA's ARM code (2026-11-15) and is the example of a lane added through `engine/machine_lanes` (no edit of the lane kit: `engine/machine_nds`, called from `LaneKit::register_worker_kinds`). Two processors in one image: the writer (`source/nds_writer`) analyses each program on its own (`Arch::V5TE` for the ARM9, V4T for the ARM7, each at its RAM address, with its own names `arm9_...` and `arm7_...`) and writes ONE source with a part for each, because the ROM is one file whose header, tables and file system name the two by offset, address and size: the header's words are label expressions (`.word arm9_end - arm9`), `.org` puts each program at its RAM address and back to the file position, and `.cpu` switches the assembler's instruction set. The assembler takes a second encoder (`Assembler(v4t, &v5te)`). The lane picks the interpreter's `System` from the function's address.

**The Nintendo 64 lane (MIPS R4300i)** (2026-11-15) extends the PlayStation's MIPS code, as a lane added through `engine/machine_lanes` (`engine/machine_n64`; the edits to shared files are the platform registry's `Id::N64` (`platforms/package`), the reach probe, the detector, and the one `register_n64_lane()` call in `LaneKit::register_worker_kinds`). `formats/n64` reads `.z64`, `.v64` and `.n64` as one ROM (held big-endian; `in_order()` gives the file back in its own order) with the header field by field, the boot code (IPL3, `$40-$FFF`) as data, its CIC chip from the CRC-32 of that code, the entry point IPL3 copies the first megabyte of the program to (the 6103 and 6106 boot codes move it), and the two header checksums recomputed from the program (a mismatch is said in the source, not repaired: the CRCs stay data, so the rebuild stays the ROM it was). The R4300 is a `Variant` of `decompile/mips` and of its assembler (`decode(..., Variant::R4300)`, `assemble(..., Variant::R4300)`: big-endian words, MIPS III's 64-bit, branch-likely, trap, TLB and FPU instructions, no GTE, no RFE; the R3000A's table and bytes are unchanged), `build/mips_asm` takes a `ByteOrder` for its data directives, `source/isa_r4300` is the flow decoder and encoder, and `source/n64_writer` is the writer: the same shape as the PlayStation's, with the header, the boot code as data, the program at its load address, and what is past the first megabyte as data at its cartridge address. The interpreter is `emulate/r4300` (not a widening of the PlayStation's, which is 32-bit and has the load delay slot the R4300 lacks): 64-bit registers, 8 MB of RDRAM, the cartridge, a PI DMA, the TLB, the FPU; the RCP's register blocks are logged (SP, DP, MI, VI, AI, PI, RI, SI), the RSP and the RDP are not run. Where it is approximate (exceptions end a run instead of being delivered, Count counts instructions, FPU arithmetic is round-to-nearest) the header of `emulate/r4300.hpp` says so.

**The PlayStation 2 and PSP lanes (MIPS R5900, MIPS Allegrex; little-endian ELF)** (2026-11-15) extend the same MIPS code once more. `decompile/mips` gains `Variant::R5900` (MIPS III without the 64-bit multiplies, MOVZ/MOVN/PREF, LQ/SQ, three-operand MULT and the second multiplier, the multimedia instructions of opcode 0x1C, a single-precision FPU with its accumulator, EI/DI, VU0 moves) and `Variant::Allegrex` (MIPS32 plus CLZ CLO MADD MSUB MAX MIN, ROTR, SEB SEH WSBH WSBW BITREV, a single-precision FPU); what a table line cannot say (VU0 macro mode's first function group, LQC2/SQC2, EXT/INS, the VFPU's loads, stores, arithmetic and prefixes) is `decompile/mips_ext`, written and read back to the same word. One interpreter serves both (`emulate/mips_le`: 64-bit registers plus the EE's upper halves, MMI, FPU, with a `Port` the machine implements), and `emulate/paged_memory` makes a 32 MB RAM cheap (shared loaded regions, 4 KB pages made when written). `emulate/ee` and `emulate/allegrex` are the machines: the EE's unit registers (timers, GIF, VIF0/1, DMAC, INTC, GS privileged) and the PSP's hardware blocks are named and logged, the EE kernel's system calls and the PSP's imported functions (by name, from the NID) are logged, and the ones that end a program end the run. `formats/mips_elf` reads the ELF, `formats/ps2` (ELF and an ISO's BOOT2 file) and `formats/psp` (ELF, PRX, EBOOT.PBP; `~PSP` is recorded as not handled; imports and exports by NID, `psp_nids.cpp` holds the function names of pspsdk's lists, each NID computed from its name's SHA-1). One writer, `source/mips_elf_writer`, writes any such file whole -- the ELF header and tables a row to a line, each section as code or data at its address, everything else (symbols, relocations, a PBP's other files) as data -- so the rebuilt file is the original; `ps2_writer` and `psp_writer` say the processor, the hardware names and the function names. Edits to shared files: `platforms/package.*` (`Id::Ps2`, `Id::Psp`), the reach probes and registry (`coverage/reach_command`), the detector, `lane_kit_workers` (the two registrations) and the lists in the tests. Not run: the VU0 macro arithmetic and the VFPU (both programs stop at it the same way), the multimedia multiplies and divides (PMULT*, PDIV*, PMADD*, PMFHL ...), the TLB, the GS, the GE, the DMA controllers and the IOP. The `mips_ext` tables are written from the published instruction layouts without a reference implementation to compare with: the VFPU's registers use CodeBreaker's own digit syntax (`C000`, `M100`) and only the instruction families listed in `mips_ext.cpp` decode; the rest of the VFPU and VU0's second function group stay `.word`.

**The PC Engine and Super Nintendo lanes (HuC6280, 65C816)** (2026-11-15) are the 65xx family's two larger members, both added through `engine/machine_lanes` (`engine/machine_pce`, `engine/machine_snes`, called from `LaneKit::register_worker_kinds`) and written in ca65 syntax over the 65xx core the NES, Atari and Lynx lanes share: `build/m6502_asm` (the directive layer and the ld65 configuration reader; it now hands the 65816's `.a16` / `.i16` to the encoder, `Encoder6502::assemble_with`, and asks the encoder how long a branch not yet resolved is, `branch_length`, because BBRn and BRL are three bytes), `source/isa_65xx` (`huc6280_encoder`, `w65816_encoder`) and `emulate/m6502` (the HuC6280 reuses the 65C02 core, with `Cpu::set_pages` moving page zero and the stack). What is new: (1) the decoder needs state. An instruction's length depends on the 65816's M and X flags, and a HuC6280 address means nothing without the mapping registers, so both writers keep the state a path leaves in a memo beside the walk (`SnesFlow::pending_`, `PceFlow::pending_`, first path to an address wins; `iw::walk` is untouched: `decode` reads the memo, `target` and `decode` write it) and put the state, where the judge needs it, in the function's address (the SNES: the widths and the mode above bit 24; the PC Engine: the bank above bit 16). (2) An address has several spellings (the SNES's $00 and $80 mirrors, the PC Engine's page a bank is mapped at), so a label is used only where the operand's own value is the label's. (3) The writers check each instruction: its text is assembled and compared with its bytes, and one that does not match is written as data, so the source is the file whatever the analysis guessed. (4) The reachable code is found from the vectors by following the state; a bank only mapping code selects is data until something reached names it.

**The Sega 32X and Saturn lanes (SH-2, big-endian, 16-bit instructions)** are the first SuperH lanes (2026-11-15) and share one writer: `decompile/sh2` (the table, with the SH-1's and SH-2's instructions in GNU as syntax), `build/sh_asm` (the directive layer: `.long` and `.word` big-endian, `.align n` as 2^n, `!` comments), `emulate/sh2` (a `Bus` of RAM regions and logged device ranges, the delay slot, MAC and the divide unit on the chip), `source/sh2_writer` (the one writer: an `Image` is parts that tile the bytes, each at its own address, code or data) and, per machine, `formats/sega32x` + `source/sega32x_writer` and `formats/saturn` + `source/saturn_writer`, with `engine/machine_sh2` registering both lanes. What is new against the others: the SH-2 loads its 32-bit constants with `mov.l @(disp,pc),rn` from a pool the compiler leaves after the function, so the writer follows each load to its constant, makes the constant a label (`K_<address>`) and writes it by what it holds (a label of the program, a register of the machine, a variable in RAM, a number), and finds functions that are only called through a pointer from those constants; a branch's delay slot is a line of its own and the flow ends after the slot of an unconditional branch. A 32X cartridge is two programs in one ROM: the 68000 side is written by the Genesis writer (`<stem>.asm`), the SH-2 program (the bytes at the ROM offset the header at $3D4 gives, loaded at $06000000) as `<stem>.sh2.s`, and the lane's rebuilt ROM is the two put together; a function's address says which interpreter runs it. A Saturn disc's compared image is the system area (IP.BIN: the header field by field, the four area-code entries, the initial program) followed by the first program (the root directory's first file that is not a licence text, at the load address IP.BIN names): one source with an `.org` per part, and the lane runs a disc of its own made from the rebuilt bytes (`<stem>.built.iso`).

**The Dreamcast lane (SH-4, little-endian)** extends the SH-2 core rather than adding a second one: `decompile/sh2` takes a `Variant` (`Sh2` big-endian, `Sh4` little-endian with the FPU and system instructions in a second table; the text of an FPU word is the single-precision one whatever FPSCR holds, and the assembler also takes `drN`, `xdN`, `fmov.d` for the same words), `build/sh_asm` writes `.long`/`.word` low byte first for it, `emulate/sh2` runs it (`Bus(Variant::Sh4)`: little-endian memory, P4 registers held and logged, the store queues; `emulate/sh4.cpp`: the FPU, banks, RTE, SHAD/SHLD, cache operations as bus-log entries), and `source/sh2_writer` writes it (`Image::sh4`: bases are physical addresses, a pointer's cache view is `label + 0x80000000`). `formats/dreamcast` reads the disc (`.gdi`, `.cdi` searched for its boot track, `.iso`, `.bin`), `source/dreamcast_writer` writes IP.BIN (header field by field) and 1ST_READ.BIN as one source and names the machine's registers, and `engine/machine_dreamcast` is the lane: main RAM 16 MB with the BIOS's system-call vectors at $8C0000B0 pointing at stubs that return 0 and log each entry, the Holly/PowerVR/AICA/Maple/G1/G2 ranges logged, PVR_SPG_STATUS and the system interrupt status answering as mid-frame. Results compared: R0 R1 FR0, R8-R15 FR12-FR15, PR GBR MACH MACL SR FPSCR FPUL, memory, hardware accesses.

**The GameCube, Wii and Wii U lanes (PowerPC 750: Gekko, Broadway, Espresso)** (2026-11-15) are three lanes on one CPU, one writer and one interpreter, added through `engine/machine_lanes` (`engine/machine_gekko`, which registers `gamecube`, `wii` and `wiiu`; the only edits to shared files are the platform registry's three ids, the reach probes and the detector). What is new: (1) **one table for three consoles.** `decompile/ppc` is a table of every 750 instruction read both ways (the opcode's fixed bits and an operand list per row; the decoder takes the first row a word matches, so the simplified mnemonics `li lis mr not mflr mtlr blr bctr beq bne bdnz cmpwi cr7,...` come first and every row builds its own assembler), with Gekko's paired singles; the three consoles' CPUs execute the same instructions, so what differs between the lanes is the machine around the CPU. (2) **Three container formats, one program.** `formats/dol` (the 256-byte header: 18 section offsets, addresses and sizes, the bss area, the entry point), `formats/ppc_elf` (32-bit big-endian PowerPC ELF; a Wii U RPX has OS/ABI 0xCA, type 0xFE01 and compressed sections, which it inflates and lays out again: the lane's program is the decompressed file, since the compressed bytes cannot be reproduced) and `formats/ppc_program`, which gives either one as the file tiled in regions (header, code, data, the rest, each with the address it is loaded at) and decides the console: a Wii U program by its ELF type, the Wii by a section in MEM2 or a `lis` of the Hollywood register block (0xCDxx) in its code, otherwise the GameCube. (3) **The writer** (`source/gekko_writer`) writes the file in file order, each region at its load address: a DOL's header as words whose addresses are the labels of its sections and whose sizes are the difference of two labels, an ELF's header and its program and section header tables a row to a line; code is what flow reaches from the entry (no delay slots, four-byte instructions), the data and the rest `.long` / `.asciz` / `.space` lines with a word that holds an address of the program written as its label, an ELF's symbol table names functions and data, and an address the code builds with `lis` and `addi`, `ori` or a load's offset is `name@ha` / `name@l` over a label, a hardware register (`VI_DPV`, `DI_STATUS`, `IPC_PPCCTRL` ... from `formats/gekko_map`) or an `.equ var_<address>`. Each instruction's text is assembled again and compared with its word, and one that would not come back is written `.long`. The assembler is `build/mips_asm`'s directive layer (big-endian, GNU as syntax) over `decompile/ppc` (`source/isa_ppc`). (4) **The interpreter is the Xbox 360's** (`emulate/powerpc`) with `paired_singles` on (opcode 4 stays VMX128 for the Xbox 360), the quantization registers (SPR 912-919), a second half for each floating register, `r2` and `r13` set the way the start-up code sets them (the small data area), and `devices`: address ranges whose accesses are logged instead of read and written (a read answers how many times the register has been read, so a polling loop ends). The lane compares how a function ended, r3 and r4, f1 and its second half, r1 and r14-r31, the memory it changed and every hardware access in order.

**The PlayStation lane (MIPS)** is the second example, in place since 2026-11-14: `formats/psx`, `decompile/mips`,
`build/mips_asm`, `emulate/mips`, `source/ps1_writer`, `platforms/ps1` and a `ps1` branch in `lane_kit_machines.cpp`
(a 32-bit address space with `key_digits = 8`, BIOS calls in its effects). It will be added here when it is merged.

## 5. Checklist to add a machine lane

1. **CPU**: `decompile/<cpu>.{hpp,cpp}` (table: decode, text, assemble, flow) and its test. If it is a Z80 relative,
   start from `decompile/sm83` (the smallest) or `z80`.
2. **Flow decoder**: `source/isa.{hpp,cpp}`: `<cpu>_decoder()`.
3. **Format**: `formats/<name>.{hpp,cpp}`: `probe`, `read`, `memory()`, `functions(decoder)`; test with a synthetic image
   built in the test (see `formats/gb_test.cpp`) so it runs without ROMs.
4. **Assembler**: `build/<cpu>_asm.{hpp,cpp}` and its test (hand-worked expected bytes).
5. **Interpreter**: `emulate/<cpu>.{hpp,cpp}`: `Bus`, `run`, `RunResult`, and its test (work expected values out by
   hand from the CPU manual, not from this code).
6. **Writer**: `source/<name>_writer.{hpp,cpp}`: `CodeModel`, `Listing`, `reconstruct(file, folder)`; test that a
   synthetic image comes back as source that builds the same bytes (`r.same`).
7. **Platform**: `platforms/<name>.{hpp,cpp}`: `Platform <name>()` from a `MachineSpec`; a descriptor test beside
   `platforms.gb.descriptor` in `platforms/machines_test.cpp`.
8. **The lane** (`engine/machine_<name>.{hpp,cpp}`, new): `<name>_lane_spec()` (`kind`, `rev`, `states`: at least three, one
   all-zero, with different `fill` seeds, `effects`, `what`, `start`, `writer`, `key_digits`), the `info` text as a
   `formats::detect::Detector` if the format is recognisable from its bytes, and `register_<name>_lane()`: ONE `register_lane({...})`
   with the name, the platform (step 7), the probe (step 3), `make_spec`, the detector and the extensions `reach` takes.
9. **The list**: one include and one line `register_<name>_lane();` in `engine/lane_list.cpp`, after the lanes whose files this one
   must not take (the order is the order the platforms are tried in). Nothing else is edited: not the platform enum, `reach`'s
   probes or extensions, `info`'s detection, the lane kit's lists, the tests' name lists.
10. **Lane test**: in `engine/lane_kit_machines_test.cpp`, a `Sample` of an open-source program (a rebuild-identical
    case and an edited-function case); record it in `docs/TESTED_SOFTWARE.md`. `engine/lane_registry_test.cpp` already checks the
    properties every registration has (a name, a platform of that name, a probe, a writer, the states) for every lane in the list.
11. **Verify**: `cb_tests.exe --filter <area>.<name>` for each area; then
    `codebreaker info <sample>`, `codebreaker reach <sample> --judge` and `codebreaker measure <sample>`.

What the judge guarantees depends on step 8: an `effects` function that does not show a place (a memory region, a
device) cannot catch a difference there. `ImageLane` says "identical" or compares exactly what `effects` returns.

## 6. The registration object: a lane is one plug-in

What a platform says to the rest of CodeBreaker used to be written in about a dozen shared lists (the platform enum and its probes, `reach`'s
probes and file extensions, `info`'s detection text, the lane kit's machine chain and name lists, and the tests' counts), and five agents adding
lanes at once collided on them. A lane is now ONE registration object, defined in the lane's own file, and the shared places iterate the registry.
(2026-11-15; `engine/lane_registry.{hpp,cpp}`, `engine/lane_list.cpp`, tests in `engine/lane_registry_test.cpp`.)

### 6.1 The shape

```cpp
void register_toy_lane() {                                           // engine/machine_toy.cpp, the lane's own file
    register_lane({
        .name = "toy",                                               // what results are filed under: the platform's name, the lane's kind
        .platform = platforms::toy,                                  // the descriptor (platforms/toy.cpp): CPU, convention, emulator, toolchain
        .probe = [](const std::filesystem::path& p) { return formats::toy::probe(p); },   // is this file one of the lane's?
        .make_spec = toy_lane_spec,                                  // the decoder, writer and machine model: what ImageLane runs (empty: the lane kit builds it)
        .detectors = {{.priority = 500, .detect = info_toy}},        // the text `codebreaker info` prints for one of its files
        .program_extensions = {".toy"},                              // the extensions `reach` takes for programs inside a container
    });
}
```

and `engine/lane_list.cpp` names `register_toy_lane` once, in the order the platforms are tried in. The list is explicit, not
self-registering: an object file of a static library that nothing refers to is dropped by the linker (MSVC, clang-cl and GCC alike), and a static
initialiser in it never runs. `register_all_lanes()` runs the list once (`std::call_once`); the host's main hook, the lane kit, `reach` and the
MCP server's profiles call it, so the host and its worker processes make the same registrations, and the unit tests register before they run.

What the registry fills, and who asks it:

| Place | Filled from the registration | Asked by |
|---|---|---|
| platform registry (`platforms::Registry`) | `lane_platform(lane)`: the descriptor with `recognise` = the probe; kept in list order | `coverage/reach_command.cpp` `standard_registry()` (`reach`, the MCP server) |
| `formats::detect::identify` | `detectors`, in the order of their `priority` (the existing ones hold 10, 20, 30 ... in the order `info` has always tried them; a new lane keeps the default 1000, after them) | `info`, `reach`, `inspect` |
| machine lane table (`machine_lanes()`) | `make_spec`, with the probe | `LaneKit::machine_lane`, `lane_named` (worker processes), `add_lanes`, `machine_of_file` |
| `reach`'s program extensions | `program_extensions` | `reach::Services::program_extensions` |
| verifier profiles, `written_as_assembly` | the name | `ui/mcp_standard.cpp`, `source/pipeline_module.cpp` |

A detector is a function `std::optional<std::vector<Detected>>(const DetectInput&)` (`formats/detect.hpp`): `DetectInput` is the mapped file, its
first 0x400 bytes, its size and its name; the helpers `one`, `name_ends_with` and `be32_or_zero` build the answer. It returns nullopt for a file that
is not the lane's. A detector may answer for a file format its lane shares with another (the PlayStation 2's covers the PSP's ELF; the
PlayStation 4's covers the PlayStation 5's). Each detector is written in its lane's own file inside `namespace cb::formats::detect { namespace { ... } }`,
so it reads exactly as it did when it was a block of `detect.cpp`.

The lane kit's own ten machines (NES, Master System, Game Boy, Genesis, PlayStation, Game Boy Advance, ZX Spectrum, MSX, ColecoVision, Amstrad CPC)
are registered too (`engine/machine_builtin.cpp`), without a `make_spec`: the lane kit still builds their lanes (`LaneKit::machine_lane`), so the machine lane table
takes no `make_spec` for those names (`built_in_machine`). Moving their specs into `engine/machine_<name>.cpp` files is mechanical and not done.

### 6.2 Properties checked for every lane

`engine/lane_registry_test.cpp` takes the registry as it is and checks, for each lane, that: its name is unique and well formed; its platform
descriptor has that name as `name` and `lane`, a title, a family, and runs code; its probe is set; a lane the table builds has a spec whose `kind` is
the name, with starting states, `effects`, `what` and a `writer`; a lane without a spec is one the lane kit builds itself; its detectors and
extensions are well formed; the platform registry built from the lanes keeps their order. `platforms/package_test.cpp` tests `Registry` itself with
stand-in machines (ordering, `named`, duplicates, probe failures); `engine/lane_kit_test.cpp` checks that every registered lane can be named by the kit.
No test counts the lanes or lists their names, so adding a lane edits no test list.

### 6.3 What is not done

* A third party's lane in another binary: the registration is a struct of `std::function`s and strings, so a DLL exporting
  `extern "C" void cb_register_lanes()` that calls `register_lane` is practical only when built with the same compiler and runtime; a
  C interface (`decode`, `run`, `assemble` as function pointers) would be the contract otherwise. Not started.
* Bespoke lanes (`pe`, `ne`, `le`, `powerpc`, `dos`, `java`) are not registrations: each is a `Kind` subclass wired in `engine/lane_kit*.cpp` with the
  kit's decompiler, builder and cache.
* The ten built-in machines' specs stay in `lane_kit_machines.cpp` (6.1).
* `reach`'s `--max-size` is one setting for all lanes; a per-lane default would be a field of the registration.

## 6b. Four lanes added on cores that were already here (Atari 5200, Jaguar, WonderSwan, 3DO)

Each is a file in `formats/`, a platform record, a writer, a machine in `engine/machine_<name>.cpp` and a registration (section 6).
(The per-lane paragraphs of sections 4 and 6b were written before the registration object: where they name edits to `platforms/package.{hpp,cpp}`,
`coverage/reach_command.cpp`, `formats/detect.cpp` or `LaneKit::register_worker_kinds`, those are now the lane's own `register_<name>_lane()` and
its line in `engine/lane_list.cpp`.)
What was particular: the **Jaguar** writer's source starts at address 0 (the assembler's image begins there), so a cartridge's `org $800000` leaves a
9 MB image and `rebuilt_file` takes the sections out of it again (for a COFF, back into a copy of the file); the **3DO** is big-endian, so the writer
works on the image with each word reversed (`Program::native()`), data is `.word`, and `emulate::arm::Bus::set_big_endian` makes a byte access go to
byte `address ^ 3` of a reversed word; the **WonderSwan** is x86, so its writer uses Capstone and NASM's rounds (`source/flat_rounds`, `x86_flat`) rather than
a table of its own: each instruction is text unless NASM does not give its bytes back (`differing` reports an instruction of another length first, since one
shifts the offsets of the branches after it), and the lane runs on Unicorn's 16-bit x86 with `in` / `out` hooks; the **5200** is the 7800's lane without the
header and the banks.

**The Nintendo 3DS lane (ARM11, ARMv6K)** (2026-11-15) extends the ARM core the GBA, DS and 3DO share, as a lane added through `engine/machine_lanes` (`engine/machine_n3ds`; edits to shared files: the platform registry's `Id::N3ds`, the reach probe, the detector, `lane_kit_workers`). `decompile/arm` gains `Arch::V6K` and `V7A` (an ordered enum; `>=` tests replace `== V5TE`) and `Op::Ext`: `decompile/arm_ext` is a table of rows (the bits a row fixes, a mnemonic, operand tokens) read both ways, covering the ARMv6 integer additions (byte reversal, sign and zero extension with rotate, LDREX/STREX, SSAT/USAT, PKH, SMMUL, SMLAD, UMAAL, USAD8, the parallel add and subtract families) and the VFP (data processing, VLDR/VSTR, VPUSH/VPOP/VLDM/VSTM, core moves, VMRS/VMSR, VCMP, VCVT); a round trip over a spread of the word space checks that every matching word assembles back. `formats/n3ds` reads a `.3dsx` (the segments, the absolute relocation tables as the list of pointer words) or an ARM ELF (`formats/mips_elf::parse(data, machine)`), `source/n3ds_writer` writes the file whole with address = file offset (a relocated word is `label + difference`, an SVC is followed by its Horizon name), and the interpreter's `System::N3ds` is paged memory (`emulate/paged_memory`) with logged hardware windows; `RunOptions::return_address` and `ending` let a lane choose where a run returns and which SVCs end it. Not done: Thumb and Thumb-2 for the ARM11, relative relocations are not applied by the loader model, the SVC IPC (service handles, command buffers) is not interpreted, GPU command lists are data, NEON-free VFP only (the ARM11 has no NEON); `.cia` / `.3ds` are encrypted and refused.

**The Virtual Boy and Intellivision lanes** (2026-11-15) each bring a CPU core of their own (V810: 32-bit RISC with 16- and 32-bit instructions; CP1610: 10-bit decles) through `engine/machine_vb` and `engine/machine_intv`, added to the machine lane table with `register_vb_lane()` / `register_intv_lane()` (shared files: the platform registry's `Id::Vb` and `Id::Intv`, `OwnDecoder::V810` and `Cp1610`, the reach probes, `formats/detect.cpp`). Both writers write a whole-file source and verify each instruction's text by assembling it back (an instruction whose text would not give its words is a data line with the text in a comment). Particular to the **V810**: addresses are 32-bit ($07000000 + the file offset, the ROM repeated to fill 16 MB), the writer pairs a `movhi` with the `movea` / `addi` / load / store that completes a constant and writes `hi(label)` / `lo(label)` when that constant is ROM data or a named hardware register, and a `movhi / movea / jmp [reg]` with a constant is followed as a jump. Particular to the **CP1610**: memory is word-addressed and ROM words are 10 bits (a word's higher bits are ignored by the decoder and kept in the data line); `SDBD` is its own line and the next instruction is double-byte, which the walk tracks (`IntvFlow::after_sdbd_`) and the assembler tracks from the previous line; the `.rom` is rebuilt with `build/cp1610_asm`, which works out each segment's CRC-16; the Intellicart's tables after the segments are kept as `ROMTAIL` bytes. Both lanes bump `spec.rev` when their model changes (the reach store keys verdicts on it; `reach --again` re-measures).

**The Nintendo Switch lane (AArch64)** (2026-11-15) adds a fourth ARM-family core, written for A64's fixed 32-bit words: `decompile/a64` is one table of rows, each a 32-symbol bit pattern (`0` and `1` fixed, any other symbol a named field) and a list of operand tokens (`RSd,RSn,AIMM`, `MU3`, `SLIST`) with an `Op` for the interpreter and an optional predicate; aliases (`mov`, `cmp`, `ret`, `lsl #n`, `cset`, `ubfx`, `uxtl` ...) stand before the rows they specialise, so a word prints as its preferred alias, and the assembler tries a mnemonic's rows in the same order, rejecting a word that would not decode to the row. `engine/machine_switch` loads a NRO (`formats/nro`) as the homebrew loader leaves it (segments at $08000000, relative relocations applied, bss, stack, thread-local storage, the loader's configuration entries), runs a function in `emulate/a64` and compares how it ended, x0, x1, x19-x28, SP, memory and the Horizon calls; the first state is what `_start` is given (x0 the configuration entries, x1 = -1). Shared files: the platform registry (`Id::Switch`, `OwnDecoder::A64`), the reach probe, the detector, the lane kit's registration. A rebuilt NSO or ELF is the loaded image, so the lane runs it as one flat region.

**The Neo Geo Pocket lane (Toshiba TLCS-900/H)** (2026-11-15) is the largest of the new cores: `decompile/tlcs900` holds one table of rows (first-byte class: single byte, register, source memory, destination memory; operand sizes; the operation byte's pattern; the operation and operand tokens in text order) that the decoder reads forward and the assembler backward, so `decode`, `text` and `assemble` cannot disagree; a test sends every second byte of every group and 400,000 random byte strings through decode, text and assemble. An instruction can have several encodings (`ld A,(XWA)` short or extended, an address in a longer form): the assembler gives the shortest, and the writer keeps the original bytes as a `DB` line whenever the text does not give them back. `build/tlcs900_asm` is `build/byte_asm` with `Options::converge` (repeat the first pass until no label moves), needed because an absolute address and a jump target change size with their value; the instruction assembler also never takes a register name for a number in that first pass. `engine/machine_ngp` (shared files: `Id::Ngp`, `OwnDecoder::Tlcs900`, the reach probe, `lane_kit_workers`, `formats/detect.cpp`) runs functions over the 24-bit map: I/O and the K2GE registers as logged devices (the raster line and the status register alternate so a frame wait ends), video memory logged, work RAM, Z80 RAM, the cartridge, and a BIOS ROM of `ret`s with the vector table at $FFFE00; `swi` and table calls are logged by number. The writer finds the interrupt handlers a program installs (`ld XWA,<ROM address>` then a store into $6FB8-$6FFF) as functions. Not done: timers, interrupts and DMA as behaviour, the Z80, the second ROM chip.

**The PlayStation Vita lane (Cortex-A9, ARMv7-A)** (2026-11-15) extends the same ARM core a second time (`engine/machine_vita`; shared files: the platform registry's `Id::Vita`, the reach probe, the detector, `lane_kit_workers`, `source/isa`, `build/arm_asm`'s size guess for `.w` instructions). `Arch::V7A`; `decompile/arm_thumb2` decodes Thumb-2: the branches into the ordinary `Op::B` / `Bl` / `Blx` (so flow needs nothing new; encode and assemble know the wide forms and the full 24-bit call), IT, CBZ, the hints and the extensions as `Op::Ext`, the VFP's Thumb words as the ARM rows with their halves swapped, and every other 32-bit word for its length with Capstone's text (`cb/core/capstone_api`). `decompile/arm.cpp` decodes the common 32-bit integer instructions (data processing, loads and stores, LDM/STM, multiplies, shifts, MOVW/MOVT, bit fields) into the fields the interpreter already runs, with Capstone's text; the assembler has no encoding for those, so the writer writes them `.word` with the text as a comment. `formats/vita` reads the SELF (segment table at the offset the header gives, zlib inflated; encrypted refused) or the ELF, the module information `e_entry` names (start and stop functions, the import tables: NIDs named from `formats/vita_nids.cpp`, generated from vitasdk/vita-headers' `db/360`, MIT), and writes the image back as a plain ELF for comparison (`Program::to_elf`). `source/vita_writer` writes the image whole, address = image offset, Thumb only, IT blocks commented with the condition. The interpreter has `System::Vita` (paged memory), IT state, imports by stub address (`RunOptions::imports`, logged by name) and the VFP's moves and arithmetic. Not done: functions called through pointers (the scan for `push {..., lr}` in the gaps found mostly data and is off), ARM-state code, an assembler for the 32-bit integer forms, NEON, LDRD/STRD/PLD/TBB in the interpreter, relocation of a relocatable program, `.vpk` archives (the `eboot.bin` inside is read).

**The PlayStation 3 lane (Cell PPU, 64-bit PowerPC)** (2026-11-15) reuses the PowerPC the Xbox 360 and GameCube lanes share (`engine/machine_ps3`; shared files: the platform registry's `Id::Ps3`, the reach probe, the detector, `lane_kit_workers`, `source/isa_ppc`). `decompile/ppc` gains rows for the 64-bit instructions (DS-form `ld` `ldu` `lwa` `std` `stdu`, the MD-form rotates `rldicl` ... with the six-bit shift and mask operands, `sld` `srd` `srad` `sradi`, `cntlzd` `extsw`, `mulld` `mulhd` `divd`, `ldx` ..., `cmpd` `cmpdi`, `fcfid` `fctid`) marked `wide`: `decode`, `decode_word` and `assemble` take `wide = false`, so the 750 family reads those words as data as before and `isa::ppc64_encoder()` assembles them. `formats/ps3` reads an ELF64 (the entry is a function descriptor: code, then the TOC r2 is set to) or a SELF (version 2: the section-info table, zlib segments inflated, encrypted refused), the PRX parameter's import stubs (NIDs named from `formats/ps3_nids.cpp`, generated from PSL1GHT's `ppu/sprx/*/exports.h` and `ppu/include/lv2/syscalls.h`, MIT) and writes the image back as an ELF for comparison. `source/ps3_writer` writes the image whole, address = image offset, finds functions from the entry and from the function descriptors in the data (a doubleword that is the address of code followed by the program's TOC), annotates `sc` with the lv2 system call from the `li r11, N` before it, and writes VMX words as `.long` with Capstone's text. The lane runs in `emulate/powerpc` (r2 = the TOC). Not done: real homebrew (none with plain segments was found, so the lane is proved on test-built ELFs), VMX in the interpreter, imported calls (the descriptors are unpatched, so a function that calls out ends there), the PRX relocation of a `.sprx`, SPU programs (data), r13 and TLS.

**The PlayStation 4, PlayStation 5 and Xbox One / Series lanes (x86-64)** (2026-11-15) are the first lanes on the 64-bit side of the x86 family, and they add no second x86 core: the decoder is Capstone's 64-bit mode (`platforms/decoder`), the runner is Unicorn's x86-64 engine (`emulate/unicorn_uc`, the one the DOS, Win32 and original Xbox lanes run on), the assembler is NASM (the whole-file x86 writers' choice since the WonderSwan lane: `source/flat_rounds`, `source/x86_flat`, unchanged). New: `formats/orbis` (an x86-64 ELF, the Orbis ELF whose dynamic data lives in the `PT_SCE_DYNLIBDATA` segment with `DT_SCE_*` tags, and OpenOrbis's fake SELF, which is read back into the ELF it holds; imports named by hashing a list of function names with the system's NID salt -- SHA-1 of name plus salt, first eight bytes reversed, base 64 -- in `formats/orbis_nids.cpp`; `.pkg` and PS5 SELFs recognised and refused; the console told by the file name, since a payload ELF does not say), `formats/xboxone` (the PE32+ of a UWP app in its `.appx` / `.msix` / `.msixbundle` zip, zip64 included, or loose; an `.xvc` / `.msixvc` / `.eappx` recognised and refused; the PE itself read by `formats/pe`), `source/x64_writer` (one NASM source for the whole file: a section per run with `vstart`, rip-relative operands as `[rel label]` or a segment label plus an offset, the register-to-register forms NASM would encode the other way through `mov_`-style macros, strings and `times` runs as data, everything else found out in rounds) under `source/orbis_writer` (symbols, relocation pointers and initialisers as function starts, GOT slots as `got_<name>`, a PLT stub named for its import so a call reads `call puts`) and `source/xboxone_writer` (the exception directory's functions, `__imp_<name>` slots), and `engine/machine_x64` (the three lanes: relocations applied, each import's slot filled with a stub that logs `name(rdi, rsi, rdx)` and returns 0, `syscall` logged by FreeBSD name, the exit functions ending the run, a thread block for `fs:` / `gs:`; effects are RAX, RDX, the callee-saved registers, RSP, the memory changed and the calls). Shared files: the platform registry's `Id::Ps4`, `Id::Ps5` and `Id::XboxOne` (placed before `Id::Win32`, which would otherwise take every PE), the reach probes, the detector, `lane_kit_workers`. Not done: data pointers as labels (a vtable is `db` rows), Sony library calls with real behaviour (every import returns 0), SPRX modules, ELF programs above 4 GB of address space, and a PS5 SELF or a retail package (refused, by design).

**The Odyssey 2 and Channel F lanes** (2026-11-15), the two smallest cores: both are byte-addressed 8-bit machines whose cartridge file is a bare ROM with no header (so `.o2` and `.chf` decide, and a `.bin` only when it starts $55 $2B). Their assemblers share `build/byte_asm` (the directive layer: the CPU supplies `assemble` and an expression evaluator, `decompile/asm_value.hpp`, and says whether `DW` is big-endian). Particular to the **8048**: `JMP` / `CALL` carry 11 bits and the memory-bank flip-flop supplies the twelfth, which the table cannot know, so a jump is taken to stay in the bank of its own address; a conditional jump goes to the page of its second byte. Particular to the **F8**: a branch displacement counts from its own address; `PI` and `JMP` overwrite the accumulator with the address's high byte; a function that calls must save PC1 (`LR K,P` / `LR P,K`) to return. The model's guesses are named in `emulate/i8048.hpp` and `emulate/f8.hpp` (the 8048's timer, the F8's decimal adjust).

## 7. The flat Z80 machines with a model: ColecoVision and Amstrad CPC

`msx` and `zx` are flat Z80 lanes: one block at one address, ROM bytes read as RET. `coleco` and `cpc` use the same writer
(`source/z80_writer`) and the same lane (`flat_spec` in `engine/lane_kit_machines.cpp`) but give it a *model*
(`emulate/z80_machines.{hpp,cpp}`): which addresses are firmware (every byte RET, each call noted by its documented
name: `formats/coleco::bios_names()`, `formats/cpc::firmware_names()`), which RAM a run is handed and seeded, where the stack
is, what a port read answers, and every port access noted with all 16 bits (the CPC decodes by the high byte). The effects
compared are the registers, the memory changed, the ports touched and the firmware entries called. A new Z80 machine of
this kind is a format (`formats/<name>`), a writer that names the machine's ports and entries, a `Machine`, and one
branch in `LaneKit::machine_lane` (the SG-1000 / SC-3000 stay in the `sms` lane: `.sg` / `.sc` files have no mapper, so
`sms_effects` does not log $FFFC-$FFFF as mapper writes for them).
