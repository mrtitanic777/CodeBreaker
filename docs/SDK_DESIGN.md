# The CodeBreaker SDK

CodeBreaker as a library: a small, stable, documented public API over its engines, plus the samples, docs and tools that make it
easy to learn and hard to misuse. Modelled on what the Xbox 360 XDK gets right: headers that are a promise, libraries you link
and go, a sample for every feature, a reference entry for every API, and tools that check your code against it.

Two goals, equally weighted (Logan, 2026-11-27):

1. **People use CodeBreaker as a library.** A program of their own opens a binary, decompiles, emulates, translates or judges,
   through `#include <cb/...>` and one library, without knowing how the source tree is laid out.
2. **The code base is easy to understand.** Clean, structured and efficient: every area has one job, one public header set and
   one owner of each concept; nothing internal leaks; a newcomer finds anything in two steps (the area, then the header).

The SDK is the forcing function for goal 2. A public API can only be small if the engines behind it are single: the unification
work (docs/ENGINE_UNIFICATION.md) is the prerequisite, and this document is where that work is heading.

## What there is today (2026-11-27)

- About 1.7 million lines in 28 areas under `src/`. Each area is one static library (`cb_<area>`) built from every `.cpp` in it.
- Every header is internal. Any area may include any other area's headers. The `codebreaker` program, the window, the MCP server and
  the tests are the only clients, and they reach into the areas directly.
- Largest areas: build (310k lines), recompile (294k), decompile (247k), dotnet (155k), translate (101k), java (90k), formats (77k),
  source (61k), engine (59k), exact (55k), emulate (42k).
- What already acts like an SDK: the `codebreaker` command line and its MCP server (32 tools), the `.fmt` file-format descriptions,
  the SLEIGH processor descriptions, `config/` as data, and the converted programs' runtime (`config/java/platform`, JArray, Gpu).

## The shape

```
include/cb/            the public headers: the only ones a client sees
  cb.h                 version, init, errors, the handle types
  binary.h             open any file: format, sections, symbols, functions, imports, resources
  disasm.h             instructions of any CPU that has a description
  decompile.h          a function or a module to C (or the lane's language), with its verdict
  l2.h                 the IR: read, build, walk, write L2 documents
  translate.h          any front end to any back end through L2
  emulate.h            a CPU from its description: memory, registers, hooks, run, trace
  judge.h              original against rebuilt: run both, compare, verdict
  build.h              compile and link authored source with the lane's toolchain
  format.h             describe a new file format (.fmt) and register it
  lane.h               describe a new platform: CPU, loader, machine, writer
  cb.hpp               C++ wrappers: RAII handles, std::string_view, std::span, ranges
lib/                   cbsdk.dll + cbsdk.lib (Release, Debug), one version
samples/               one small program per feature, all on samples/framework
docs/sdk/              the reference (generated from include/cb) and the concept guides
```

- **A C ABI underneath, C++ on top.** `cb.h` and friends are plain C: opaque handles (`cb_binary*`, `cb_function*`, `cb_l2_doc*`),
  `cb_status` results, caller-owned buffers or `cb_string` values with one free function. Any language binds to it (Python, C#,
  Rust). `cb.hpp` gives C++ users RAII and modern types with no extra cost.
- **One DLL, one version.** `cbsdk.dll` exports only the `cb_*` functions. Its version (`CB_SDK_VERSION`, `cb_version()`) follows
  semantic versioning: a minor release only adds; a major release may remove, and the docs list what moved.
- **Everything else is internal.** The areas under `src/` stay as they are, but their headers stop being reachable from outside;
  `include/cb` is implemented in `src/sdk/`, a thin layer over the engines.

## The rules (for the SDK and for the code base)

1. **One concept, one owner.** Each idea (a binary, a function, an L2 node, a CPU, a verdict) has exactly one type and one area. A
   second copy is a bug (see ENGINE_UNIFICATION.md).
2. **Areas have a public face.** Each area keeps its outward headers in one place (`src/<area>/api/` or a single `<area>.hpp`)
   and everything else is private to it. Other areas include only the public face. A checker enforces it (phase 1).
3. **Layers point one way.** core, base -> formats -> decompile, emulate -> translate, verify -> engine, lanes -> app, ui, sdk. An
   include that points up is a build error.
4. **Data, not code, where it's data.** Tables (NIDs, opcodes, formats, compiler versions, platform descriptions) live in `config/`
   as text and are embedded at build time.
5. **Every public function has**: a reference entry (what, parameters, ownership, errors, thread safety, availability), a sample or
   a test that calls it, and a stable name in one style (`cb_<noun>_<verb>`: `cb_binary_open`, `cb_function_decompile`).
6. **Fast by default.** No API forces a copy, a JSON hop or a re-parse; handles point into the engine's own structures; long work
   reports progress and can be cancelled.
7. **Errors say what to do.** Each `cb_status` has a plain-words message naming the input and the next step.

## The API, first cut

| Header | Core calls | Backed by |
|---|---|---|
| `cb.h` | `cb_version`, `cb_init(options)`, `cb_last_error`, `cb_string_free`, `cb_cancel` | base, core |
| `binary.h` | `cb_binary_open(path|bytes)`, `_format`, `_arch`, `_sections`, `_symbols`, `_imports`, `_exports`, `_functions`, `_resources` | formats (the .fmt engine and its readers) |
| `disasm.h` | `cb_disasm(arch, bytes, address)` -> instructions with text, length, operands, branch targets | decompile/engine (SLEIGH) |
| `decompile.h` | `cb_function_decompile(fn, options)` -> source text, language, warnings; `cb_module_decompile`; `cb_function_verdict` | decompile, the lanes |
| `l2.h` | `cb_l2_read/write`, `cb_l2_root`, node kind, children, types, symbols, `cb_l2_build_*` | translate/ir |
| `translate.h` | `cb_translate(input, from, to, options)` -> files; `cb_languages()` | translate front and back ends |
| `emulate.h` | `cb_cpu_create(arch)`, `_map`, `_reg_get/set`, `_step`, `_run(until)`, `_hook`, `_trace` | the one p-code emulator |
| `judge.h` | `cb_judge(original, rebuilt, options)` -> verdict, first difference | verify, engine |
| `build.h` | `cb_build(project, toolchain)` -> output, diagnostics | build |
| `format.h`, `lane.h` | register a `.fmt` description or a lane description at run time | formats, engine |

The first cut is deliberately small. A call goes in when a sample needs it, not before.

## Samples (`samples/`)

Each is one folder, one `main.c` or `main.cpp`, a CMakeLists that finds the installed SDK (`find_package(CodeBreaker)`), and a
README that says what it shows. They share `samples/framework` (argument parsing, printing, timing) the way the ATG samples share
theirs.

1. `HelloBinary`: open a file, print its format, sections and imports.
2. `HelloDecompile`: decompile one function to C.
3. `DisasmAnyCpu`: disassemble bytes for any described CPU.
4. `Emulate`: load a small ROM and step it, printing registers.
5. `WalkL2`: decompile to L2 and walk the tree.
6. `Translate`: Python to C++17 through L2, then build and run both.
7. `JudgeTwoBuilds`: run an original and a rebuilt program and print the verdicts.
8. `NewFileFormat`: a `.fmt` description that `HelloBinary` then reads.
9. `NewCpu`: a toy CPU from a `.sinc`: disassemble, emulate and decompile it.
10. `FromPython`: the C ABI from Python (ctypes), to show the ABI is language-neutral.

Every sample builds and runs in the gate, so a sample that rots is a failing test.

## Docs and tools

- **Reference:** generated from `include/cb` comments (one page per header and per function: prototype, parameters, ownership,
  errors, thread safety, since-version, the samples that call it). Concept guides: binaries, L2, lanes, verdicts, the emulator,
  translation.
- **`cb` MCP doc tools**, like the `xdk_*` tools: `cb_lookup` (any SDK function: prototype, doc, header, since), `cb_examples` (which
  samples call it), `cb_verify` (compile a snippet against the SDK headers and report misuse).
- **Trace viewer:** the behavior engine's flight recorder rendered as a timeline: CB's PIX.

## The runtime SDK (second track)

Converted programs (Java games and others) target a runtime: JArray, the object model, `GL11` / `Gpu` / the per-API graphics layers
in `config/java/platform`, audio, input. Documented and versioned as its own small SDK (`runtime/include`, samples such as "a
converted game on a new graphics back end"), it is what lets someone else port converted programs to a new platform, the way the
XDK defines the Xbox 360 for games. It follows the same rules: one owner per concept, public headers only, a sample per feature.

## Phases

Each phase ends in something that builds, passes the gate and is useful on its own.

0. **Inventory.** For every area: what other areas, the app, the window and the MCP server actually use from it (the de facto
   public surface), measured from the includes and calls. Output: `docs/sdk/inventory.md`, a table per area, and the upward
   includes that break rule 3.
1. **Public faces.** Each area gets its public header set (rule 2), callers move to it, and a checker in the gate fails on an
   include of another area's private header or an upward include. This is the cleanliness work; it pays off even before the SDK.
2. **`include/cb` + `src/sdk` + `cbsdk.dll`, first cut:** `cb.h`, `binary.h`, `disasm.h`, `decompile.h`, with samples 1-3, the
   reference generator, and the install layout (`find_package(CodeBreaker)`).
3. **L2, translate, emulate, judge** headers with samples 4-7, and the C ABI proven from Python (sample 10).
4. **Extending:** `format.h`, `lane.h`, samples 8-9.
5. **Tools:** the `cb` MCP doc tools, the trace viewer.
6. **The runtime SDK** for converted programs.
7. **Release:** SDK 1.0, with its version promise, and the `codebreaker` program itself rebuilt as a client of the SDK where that is
   no slower. The program becomes the first and biggest sample.

## Proof

- The gate builds every sample against the installed SDK (not the source tree) and runs it.
- An ABI check compares the exported `cb_*` symbols and the public headers to the last release; a removal outside a major version
  fails.
- The include checker (phase 1) runs in the gate.
- Speed: each SDK call is timed against the internal call it wraps; the SDK may not add measurable cost.

## Phase 0 findings (2026-11-27)

The inventory is [docs/sdk/inventory.md](sdk/inventory.md) (regenerate: `python tools/sdk/inventory.py src work/sdk-inventory/facts.json
&& python tools/sdk/report.py`). 27 areas, 7,716 files; 4,965 includes cross an area boundary in product code.

- **The layering is nearly right already.** Only 116 of those 4,965 includes point upward (34 area pairs). But they tie 22 of the 27
  areas into one cycle, so today no area can be built, tested or understood without the others. Fixing about a hundred includes
  breaks the cycle.
- **The upward includes have four causes:**
  1. **Commands live in low areas.** `store/analyze_command.cpp`, `store/versions_command.cpp`, `coverage/reach_command.cpp`,
     `symbols/cmds_main.cpp`, `verify/regress_main.cpp` and about a dozen more are command-line verbs. They reach up into engine,
     verify, java, recompile and xna because that is what a command does. They belong with the other commands in `app` (or a
     `commands` area at the top). Moving 18 files removes about half of the upward includes.
  2. **The L2 hub sits too high.** `decompile` includes `translate/l2.hpp`, `ir.hpp`, `native_l2.hpp` and `backends/c.hpp` (13
     includes): the decompiler emits L2, so L2 is below it, not above. `translate/ir` + `l2` become their own low area (`l2`,
     layer 1). The C back end the decompiler calls is the one real upward call and goes behind an interface `decompile` owns.
  3. **`platforms` is data, not a top layer.** It is 123 small files of console descriptions (memory maps, decoders) used by invert,
     coverage, symbols, analyze, compare, decompile and emulate. It belongs at layer 1, beside `formats`.
  4. **Test helpers in product folders.** `formats/*_test_image.hpp` build test ROMs with `source` and `build`; they move next to
     the tests that use them.
- **The messiest public faces** (headers used from outside): `formats` 98 headers from 20 areas, `source` 69, `platforms` 50,
  `emulate` 36, `decompile` 33, `core` 30 (used by every area: 3,277 includes, which is right for core), `build` 28, `java` 25.
- **The corrected layering**, lowest first:
  0. core, base
  1. formats, platforms, l2 (translate/ir + translate/l2), symbols, store, knowledge, jobs
  2. decompile, emulate, analyze, invert, exact, coverage, compare
  3. translate, source, build, dotnet, java, xna, recompile, port, verify
  4. engine
  5. app (with every command), ui, sdk

## Status

- 2026-11-27: design written (Logan: "use it as a library", "a really easy to understand code base that is not scattered all over
  the place and internal all over the place"; "cleanliness, structure, and efficiency"). Phase 0 started.
- 2026-11-27: phase 0 done: the inventory (docs/sdk/inventory.md, tools/sdk), its findings above. Next: phase 1, starting with the include checker and the four causes of the cycle.
- 2026-11-27: phase 1, step 1. The layering check is a test (app.layers.includes_point_down_except_the_listed_ones, every gate):
  upward includes are listed in tools/sdk/layer_baseline.txt, a new one fails, a fixed one must come off. Upward includes 116 -> 43:
  the commands analyze, versions, reach and ppc-calls moved into app (namespaces cb::app::...); the function store and the verdict
  vocabulary moved from engine and verify down into store (cb::store::FunctionStore, cb::store::Verdict, ...); harness programs
  (*_main.cpp) and test-image helpers (*_test_image.hpp) count as tests. Found on the way: verify/lane_common's Judgement duplicates
  store::Judgement (rule 1; next).
- 2026-11-27: phase 1, steps 2-5: one Judgement (27a462673); base reaches up no more: converted -> store, corpus-executables ->
  app (0812be3c3); the OpenWatcom driver -> base (0aee7d903); the comparison mask -> symbols (bdeaecb12). Upward includes 43 -> 32.
  Found: `symbols` is two areas in one. The symbol data (libraries, ordinals, the mask, names as data) is low and used by decompile,
  analyze, emulate, knowledge. The identification and naming pipeline (identify, names, names_bodies, workbench, debug_build,
  debug_build_estimate, ordinal_collect) uses decompile, analyze, build and source, and accounts for 14 of the 32. Next: split the
  pipeline into its own layer-3 area (`naming`), leaving `symbols` as the data.
- 2026-11-27: phase 1: symbols split. symbols/name_data.hpp holds the names as data (7811f5f69); the naming pipeline (the Namer,
  identify, the workbench, the debug build and its estimate, ordinal collection, the proposals) is its own area, naming (layer 3,
  cb::naming::...). symbols no longer includes anything above it. Upward includes 31 -> 19: what is left is decompile reaching
  translate's C back end (6) and build's data and Watcom helpers (2), exact's DOS runner (3), source and verify reaching the lane
  kit (7), store's versions ledger (1).
- 2026-11-27: phase 1: the L2 hub counts as the l2 layer, decompile/jar -> java, build/data -> decompile (d77067f32); the
  toolchain area (layer 1): the OpenWatcom, Microsoft C 6 and Visual C++ 1.52 drivers and the native DOS runner (cb::toolchain).
  Upward includes 11 -> 7, all of them source and verify reaching engine's lane kit (6) and store's versions ledger (1).
- 2026-11-27: phase 1 step: upward includes 0. The source pipeline is engine code (6e8a84aac), the versions ledger compare's
  (ee3aa0c5a), regress a command in app (bbfe805c5), and the one emulator's CPU adapters (engine/pcode_*, 110 files) are
  emulate/cpu (cb::emulate::pcode_*). Every include of product code now points down; app.layers keeps it so. Next: phase 1's
  public faces (one public header set per area).
- 2026-11-28: phase 2, the first cut. `include/cb`: `cb.h` (version, `cb_init`/`cb_shutdown`, `cb_status`, `cb_last_error`, `cb_string`, the
  handles), `binary.h` (open any file: format, processor, sections, imports, exports, the functions it names), `disasm.h` (the one
  disassembler: every processor of source/processors through its SLEIGH description, with length, flow and target), `decompile.h` (a
  function or a module as C), `version.h`, and `cb.hpp` (RAII, `std::string_view`, `std::span`, exceptions). 35 functions, all `cb_<noun>_<verb>`.
  `src/sdk` implements them over formats/inspect, source/processors (its pooled SLEIGH readers, now `read_instructions`), decompile/decompiler
  and engine/lane_kit, and includes nothing else of the engines (test sdk.includes.only_public_faces). Two pieces moved to their one owner on
  the way: the pooled instruction read (source/processors, which read_described now lists from) and cutting one function out of a module's C
  (decompile::decompiler::function_in, which the MCP server's decompile_function now calls too). `cbsdk.dll` + `cbsdk.lib` export only the
  `cb_*` names the headers declare (the export list is read from the headers at configure time; ELF: a version script) and carry the version
  (resource, `cb_version`). `cmake --install build --component sdk --prefix P`: `bin/` (cbsdk.dll, decompiler.exe, the engine DLLs),
  `include/cb`, `lib/`, `lib/cmake/CodeBreaker` (`find_package(CodeBreaker 0.1)`, target `CodeBreaker::cbsdk`), `share/codebreaker` (the data
  root cb_init finds beside bin/: config/ and the processors' compiled descriptions), `share/doc/codebreaker/reference`. Samples 1-3
  (HelloBinary in C, HelloDecompile in C++, DisasmAnyCpu in C) on `samples/framework`, built against the installed SDK and run against
  `tests/sdk/golden` in the gate (`tools\sdk\samples.cmd`, the gate's SDK step). The reference (`docs/sdk/reference`: an index, a page per
  header and per function, with the samples and tests that call it) is made from the headers' comments by `tools\sdk\reference.cmd`; the
  tests keep it current, keep every function called by a sample or a test, and keep every name of `tools/sdk/abi_baseline.txt` declared.
  Speed: `cb_sdk_bench` times each call against the one it wraps. Left for later: `cb_function_verdict` (with judge.h, phase 3: it needs the
  function store and a profile), `cb_cancel` and progress (no sample needs them yet), `cb_binary_open` from bytes (the readers take a path),
  resources and symbols in binary.h, functions found by analysis (the list is the entry point and the exports), a Debug cbsdk, the Linux
  build of cbsdk (written, not built).
