# Lanes: what a platform provides, and how to add one

A **lane** is the path one kind of program takes from its file to judged source: written back as source, rebuilt, and
each function run beside the original's. A **platform** is what a program was made for. This document says what a
platform provides today, file by file, checked against the code (2026-11-14); then how to add one (a checklist and a
worked walk-through of the Game Boy lane); then the design for adding a platform without editing the core, and what of
that exists now.

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
| Lane wiring | `engine/lane_kit_machines.cpp` | per machine: its starting states, its `effects` function (load, run, collect), and its entry in the if-chain of `LaneKit::machine_lane` |

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

Two closed lists must grow by one when a platform is added, both in `platforms/package.hpp` (`enum class Id`,
`kOrder`) and `platforms/package.cpp` (`kIdNames`, and the `switch` in `Registry::recognises`, which calls a probe by
id: `case Id::Gb: return need(pr.is_gb, "is_gb")(path)`), and `Probes` gains an `is_<name>` member. Platforms are tried
in `kOrder`; the first that recognises a file is its platform (`Registry::identify`).

Who builds the registry: `coverage/reach_command.cpp`, `standard_registry()` fills `Probes` (one lambda per format:
`probes.is_gb = ... formats::gb::probe`) and calls `registry->add(platforms::gb())` for every platform. That is the
only production construction (also used by `ui/mcp_symbols.cpp`).

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

1. `Registry::identify(path)` tries the platforms in `kOrder` (`platforms/package.cpp`); `identify_with(registry)`
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
   `kit_detail::machine_of_file(p)` (`lane_kit_machines.cpp`), a chain of `formats::<name>::probe` in the order
   nes, sms, gb, genesis, ps1, zx, msx.
3. `Kinds::find(name)` gives the lane; `prepare` writes the source, `build` assembles it, `judge` per function; the
   candidates go to the function store (`engine/function_store`).

Judging also runs in worker processes (`lane_kit_workers.cpp`): a worker makes its own `LaneKit` and asks
`LaneKit::lane_named(name)`, so a lane must be reachable through `lane_named` (machine lanes are, via `machine_lane`).

Other places that list the machine kinds by name (all are in the checklist): `source/pipeline_module.cpp`
(`written_as_assembly`: the module is written as an assembly file), `ui/mcp_standard.cpp` (`verifier_profiles`: the
kinds the ledger reports settings digests for), `engine/lane_kit.cpp` (`add_lanes`: the list of machine names),
`engine/lane_kit_internal.hpp` (comments and the `machines_` map).

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
| 8 | `platforms/package.hpp` | `Id::Gb`, `kOrder`, `Probes::is_gb` | edit |
| 9 | `platforms/package.cpp` | `kIdNames`, `case Id::Gb` in `recognises` | edit |
| 10 | `coverage/reach_command.cpp` | include, `probes.is_gb`, `registry->add(platforms::gb())` | edit |
| 11 | `engine/lane_kit_machines.cpp` | `gb_state`, `gb_effects`, the `name == "gb"` spec branch, `machine_of_file` line | edit |
| 12 | `engine/lane_kit.cpp` | `"gb"` in the machine list of `add_lanes` | edit |
| 13 | `engine/lane_kit_internal.hpp`, `lane_kit.hpp` | the name in comments (the `machines_` map is keyed by string) | edit |
| 14 | `source/pipeline_module.cpp` | `written_as_assembly` | edit |
| 15 | `ui/mcp_standard.cpp` | `verifier_profiles` kind list | edit |
| 16 | `engine/lane_kit_machines_test.cpp`, `source/writers_homebrew_test.cpp` | the lane's and a real game's tests | edit |

`CMakeLists.txt` needs no change (the files are globbed into existing areas). Ten of the sixteen rows are edits to shared
files; rows 8 to 12, 14 and 15 are the "registration points" of section 6.

The flow for `codebreaker measure game.gb`: `kind_of_file` -> `machine_of_file` -> `formats::gb::probe` (row 3) gives
"gb"; `Kinds::find("gb")` is the `ImageLane` made from rows 11's spec; `prepare` calls `gb_writer::reconstruct`
(row 6), which reads the image (3), finds function starts with the SM83 decoder (2, 1), writes assembly, and
assembles with `sm83_asm::build` (4); `build` records whether the bytes are the original; `judge(name)` is either
"identical" or runs the function in both programs with `gb_effects` over `emulate::sm83` (5) from the four
starting states and compares end, A, flags, BC, DE, HL, RAM and the I/O log.

**The Game Boy Advance lane (ARM7TDMI)** is the third (2026-11-15): `formats/gba`, `decompile/arm`, `build/arm_asm`, `emulate/arm`, `source/gba_writer`, `platforms/gba` and a `gba` branch in `lane_kit_machines.cpp`. What is new against the others: the processor has two instruction sets, so an address carries the state in its lowest bit (odd: Thumb) through the format's function finder (`source/isa`: `arm_decoder`), the writer's flow (a `bx` is followed when the register was just loaded from a literal pool or made from the pc) and the assembler (`.arm`/`.thumb`; the encoder is given the state as the address's bit 0); a function's key in the result and in the store is odd for Thumb. The writer checks each instruction's text against the assembler and keeps the bits (`.hword`/`.word`) of those it would write differently.

**The Nintendo DS lane (ARM946E-S + ARM7TDMI)** reuses the GBA's ARM code (2026-11-15) and is the example of a lane added through `engine/machine_lanes` (no edit of the lane kit: `engine/machine_nds`, called from `LaneKit::register_worker_kinds`). Two processors in one image: the writer (`source/nds_writer`) analyses each program on its own (`Arch::V5TE` for the ARM9, V4T for the ARM7, each at its RAM address, with its own names `arm9_...` and `arm7_...`) and writes ONE source with a part for each, because the ROM is one file whose header, tables and file system name the two by offset, address and size: the header's words are label expressions (`.word arm9_end - arm9`), `.org` puts each program at its RAM address and back to the file position, and `.cpu` switches the assembler's instruction set. The assembler takes a second encoder (`Assembler(v4t, &v5te)`). The lane picks the interpreter's `System` from the function's address.

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
8. **Platform registry** (core edit): `Id`, `kOrder` (and the array size), `kIdNames`, `Probes::is_<name>`, the
   `recognises` case in `platforms/package.{hpp,cpp}`; the `probes.is_<name>` lambda and `registry->add` in
   `coverage/reach_command.cpp`; `platforms/package_test.cpp`'s completeness check follows `kOrder.size()`.
9. **Lane** (`engine/lane_kit_machines.cpp`): `<name>_state`, `<name>_effects`, and the spec: `kind`, `rev`, `states`
   (at least three, one all-zero, with different `fill` seeds), `effects`, `what`, `start`, `writer`, `key_digits`.
   Add the `machine_of_file` probe line and the name to `add_lanes`'s list in `lane_kit.cpp`. **Or, with the registry
   of section 6, one `machine_lanes().add({...})` and none of these three edits.**
10. **Name lists**: `written_as_assembly` in `source/pipeline_module.cpp`, `verifier_profiles` in
    `ui/mcp_standard.cpp` (skip both if you registered through `machine_lanes()`: they ask the registry).
11. **Lane test**: in `engine/lane_kit_machines_test.cpp`, a `Sample` of an open-source program (a rebuild-identical
    case and an edited-function case); record it in `docs/TESTED_SOFTWARE.md`.
12. **Verify**: `cb_tests.exe --filter <area>.<name>` for each area; then
    `codebreaker reach <sample> --judge` and `codebreaker measure <sample>`.

What the judge guarantees depends on step 9: an `effects` function that does not show a place (a memory region, a
device) cannot catch a difference there. `ImageLane` says "identical" or compares exactly what `effects` returns.

## 6. Loadable without editing the core: design

### 6.1 The registration points that need an edit today

| # | Point | Why it is a closed list |
|---|---|---|
| A | `platforms/package.hpp` `Id`, `kOrder` (a `std::array<Id, 14>`), `package.cpp` `kIdNames` (array of 14), `Registry::recognises` switch | an enum with exhaustive switches; the array lengths are literals |
| B | `Probes` (`is_nes`, `is_sms`, ...) and `reach_command.cpp` `standard_registry()` | one member and one lambda per format; one `add` per platform |
| C | `engine/lane_kit_machines.cpp` `LaneKit::machine_lane` | an `if (name == "gb") ... else if ...` chain that builds the spec |
| D | `engine/lane_kit_machines.cpp` `kit_detail::machine_of_file` | a chain of probes in a fixed order |
| E | `engine/lane_kit.cpp` `LaneKit::add_lanes` | a brace list of names |
| F | `source/pipeline_module.cpp` `written_as_assembly`, `ui/mcp_standard.cpp` `verifier_profiles` | string comparisons / a list of names |
| G | the CPU, format, assembler, interpreter and writer | not registration: new files in existing areas (globbed by CMake) |

G is already pluginless in the sense that matters: a new file in an existing area compiles into it. The rest is a
registry problem: a lane is made of a *name*, a *probe* (is this file mine?) and a *spec* (what `ImageLane` is
built from) -- C, D, E, F are the same three facts written in four places.

### 6.2 Proposal

1. **A table of machine lanes** (`engine/machine_lanes.hpp`): `MachineLane{name, probe, make_spec}` and a
   process-wide `MachineLaneRegistry`. `LaneKit::machine_lane`, `machine_of_file`, `add_lanes`,
   `written_as_assembly` and `verifier_profiles` ask it. **Implemented now (6.3)**, additive: the kit's seven machines
   are still built in; a registered one is consulted after them.
2. **Migrate the seven built-ins into the table** (mechanical: each `if` branch of `machine_lane` becomes a
   `MachineLane{...}` in a `register_builtin_machines()`), then delete the chains (C, D, E, F) and `built_in_machine`.
   Not done yet only because the PS1 lane is being added to the same function by another change; do it after it lands.
3. **One folder per platform**: `native/cb/lanes/<name>/` with `lane.cpp` (the `MachineLane` and its `register_<name>()`)
   and the platform's own files, instead of the files spread over seven areas. Because `cb_area` globs, a
   `cb_area(lanes)` is one CMake line, with a generated `lanes_registry.cpp` (CMake `file(GLOB lanes/*/lane.cpp)` writes a
   call to each `register_<name>()`), so adding a platform adds a folder and nothing else. A static-library
   linker drops an object file nobody refers to, so self-registering static initialisers are not used: the generated
   file is the one reference.
4. **The platform record without the enum** (point A, B): `Platform` gets an optional
   `std::function<bool(const std::filesystem::path&)> recognise` and `Id::Custom`, so a platform needs no `Id`,
   `Probes` member or `switch` case; `Registry` tries the built-ins in `kOrder`, then the custom ones in the order
   added. This is what makes `reach` list a registered lane's files as "has a lane" (today `reach` reads only
   `Registry`). It touches `package.{hpp,cpp}`, which the MIPS lane is also editing, so it is left as a proposal.
5. **Out-of-tree**: with 1-4, a third party's lane is a static library (or a DLL exporting
   `extern "C" void cb_register_lanes(cb::engine::MachineLaneRegistry&)`, loaded by the host with `core/dynlib.hpp`
   from a `lanes/` folder next to `codebreaker.exe`). A DLL boundary needs a stable ABI for `ImageLaneSpec` (it
   holds `std::function`s and `std::string`s): practical only if the plugin is built with the same compiler and
   runtime; otherwise a C interface (`decode`, `run`, `assemble` as function pointers) is the contract. Not started.

### 6.3 What is implemented

`engine/machine_lanes.{hpp,cpp}` (cb_engine), tests in `engine/machine_lanes_test.cpp`.

```cpp
engine::machine_lanes().add({
    .name = "toy",
    .probe = [](const std::filesystem::path& p) { return p.extension() == ".toy"; },
    .make_spec = [] { verify::lanes::ImageLaneSpec s; s.kind = "toy"; /* states, effects, what, start, writer */ return s; }});
```

After that: `LaneKit::machine_lane("toy")` and `lane_named("toy")` (what a worker process asks), `add_lanes` (so `measure`
judges `.toy` files), the reconstruction path's `kind_of_file`, `written_as_assembly` and the MCP profile list all have
it. Behaviour of the seven built-in machines is unchanged: they are tried first; `add` refuses their names, an empty
or ill-formed name, a missing function and a duplicate (`UsageError`). A probe's `Error` is "no"; probes run outside the
registry's lock; `find` returns a `shared_ptr` that outlives a `remove`.

The host registers before it makes a `LaneKit`, at the top of `main` (as it calls `host_main_hook`); its worker
processes are the same program and register the same way. The registered lane is not in the platform registry (point A,
B above), so `codebreaker reach` identifies its files as "unknown" until item 4 of 6.2; `measure`, `module` and the
function store work.

### 6.4 What a third party still needs

* A place for their code: today, new files in the existing areas and a rebuild of CodeBreaker (items 3 and 5 of 6.2
  make that a folder, then a DLL).
* To make the `reach` report name their platform: the edits of point A and B (item 4).
* An `effects` function written against their own interpreter: nothing generates it; this is the part that defines
  what "behaves like the original" means for their machine.
* Registration at start-up: one call before the kit is made.
* For bespoke (compiled-language) lanes, the `Kind` subclass and its tools; those are not registrable yet (their
  construction is in `lane_kit.cpp` and takes the kit's decompiler, builder and cache).

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
