// codebreaker/build/module.hpp -- SHIM over cb/build/module.hpp (docs/CPP_DESIGN.md 6.3): the old types in, the new builder called, the
// old types out. Deleted with its last py:: consumer (engine/win16, engine/lanes, jobs/workers and the writers that build a module whole).
// The shim's translation unit (module.cpp) holds the one ModuleBuilder the old callers share and wires it to the old layer: the old cache,
// corpus and settings ports, the decompiled module the lanes prepare (Module16 / Module32 and what they hold, made into the new ones), the
// readers of a module's interface, the emulators and the gate that judges a rebuilt function.
//
// module.py -- build a whole module from its reconstruction, and check what came out (see cb/build/module.hpp):
//
//     build(module)            -> Built; b.image is the linked file; b.report() says what was found
//     check(b), run(b)         compare the linked image with the original / call its exports beside the original's
//     _build16, _build32       the two linkers, for the engine's own build path
//
// A Built here keeps its old shape (py:: containers); what the new Built holds that no old caller reads (the origins of the functions, the
// frame's fit, the module as an L2 tree) is not carried back.
//
// Typing notes:
//   * The decompiled-and-made-compilable module the builders read is decompile::c_text::Module16 / Module32 (c_text.hpp), as before; the
//     shim makes the new Module16 / Compilable32 of it for each build.
//   * An address of an export is build::definitions::Export::address: a virtual address (32-bit) or (seg, off) (16-bit).
//   * SystemExit (raised for an unknown module) is py::SystemExit.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <tuple>
#include <variant>
#include <vector>

#include "codebreaker/build/data.hpp"
#include "codebreaker/build/definitions.hpp"
#include "codebreaker/build/resources.hpp"
#include "codebreaker/core16_types.hpp"
#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::module {

// the (virtual address) or (segment, offset) a function sits at, as definitions::Export::address holds it
using Address = std::variant<uint64_t, std::tuple<int64_t, int64_t>>;

inline constexpr int64_t RUN_LIMIT = 40;   // exported functions called when a built module is run

extern const py::re::Pattern _BY_ADDRESS;  // \bfunc_0x([0-9a-f]{8})\b   (used by engine.win16)

// where kept_as_asm.txt is (beside module.py)
extern const py::str KEPT_AS_ASM;

// A build and what was found out about it.
class Built {
  public:
    py::str original;                      // the original's path
    py::str name;                          // its file name, lower case
    std::optional<py::str> kind;           // corpus.lane(original): "pe" | "ne" | "le" | "dos" | None
    std::optional<py::str> folder;         // where the build was made
    std::optional<py::str> image;          // the linked file (None: not linked)
    py::str error;
    int64_t functions = 0;                 // in the module
    int64_t with_text = 0;                 // given C by the decompiler
    py::dict<py::str, py::str> rejected;   // {function: why the compiler rejected it} -- stand-ins
    py::list<py::str> absent;              // called, but nothing reconstructed: stand-ins too
    py::dict<py::str, py::str> assembled;  // 16-bit: rejected C, written as its own instructions instead
    py::dict<py::str, py::str> assembly_refused;   // 16-bit: why a rejected function could not be
    py::list<py::str> undefined;           // left unresolved in the image
    std::shared_ptr<build::data::Data> data;        // build.data.Data (None: no data carried)
    py::str data_error;
    int64_t lost_addresses = 0;            // address constants in functions that could not be placed
    std::shared_ptr<build::resources::Result> resources;   // of writing and compiling them (None: not made)
    py::str entry;
    bool format_same = false;
    int64_t exports_total = 0, exports_same = 0, exports_on_stand_ins = 0;
    py::list<std::tuple<build::definitions::Export, py::str>> exports_missing;   // (Export, why)
    py::list<build::definitions::Import> imports_extra;     // (MODULE, name or ordinal), sorted
    int64_t imports_original = 0, imports_built = 0;
    std::optional<bool> resources_same;    // None: the original has none
    py::str resources_line;
    py::list<std::tuple<py::str, py::str, py::str, bool>> ran;   // (export, rebuilt exit, original exit, same result)
    py::set<Address> stand_in_addresses;   // where, in the original, the rejected functions are

    // --- members the builders add while they work ---
    py::dict<py::str, py::str> gated;               // {function: how its C differs from the original} (rebuilt from instructions)
    py::list<std::tuple<int64_t, py::list<py::str>>> code_data;   // (segment, the functions that read the original's data through CS)
    py::list<std::tuple<py::str, py::str>> disputes;   // (function, who the callers say removes the arguments)
    std::optional<py::str> contract_error;          // the call-contract check failed
    std::optional<py::str> gate_error;              // the gate could not run some functions
    py::list<std::tuple<int64_t, int64_t>> hidden; // (segment, offset) functions the description does not have
    py::list<py::str> lead_ins;                     // 16-bit: object stems placed first into their segment, in order
    py::set<py::str> placed_c;                      // 16-bit: functions kept as C (a frame's pieces fit)
    py::set<py::str> moved_c;                       // 16-bit: functions moved to the segment beside theirs
    py::dict<py::str, py::str> resident_overflow;   // {function: why its C finds no room below where a segment is cut}
    py::list<std::tuple<py::str, int64_t, int64_t>> frame_fit;   // (function, hole size, its size)
    py::str frame_why;                              // why a frame's pieces did not assemble back
    py::dict<py::str, py::str> elsewhere_why;       // {function: why the compiler rejected its far copy}
    py::set<py::str> near_names;                    // near functions (a near one stays in its segment)
    py::dict<int64_t, int64_t> resident_limit;      // {segment: where it is cut after start-up} (loads_itself modules)
    std::optional<py::str> layout;                  // "segments renumbered: ..." when the segments differ
    std::optional<py::str> carried_report;          // a build made in another process: its report as that process wrote it

    Built() = default;   // (so that py::dict<py::str, Built> can hold one)
    explicit Built(const py::str& original);

    bool linked() const;                   // @property: bool(self.image)
    py::str report() const;
};

// An original whose MZ header has DOS load the whole file (KRNL386): ours says the same of ours.
void _whole_file_image(const py::str& original, const py::str& image);

// The two linkers: link `m` (compiled with the platform's tools) into a file in `folder`; -> its path, or None when nothing was linked
// (b.error says why).
std::optional<py::str> _build16(Built& b, const py::str& path, const decompile::c_text::Module16& m, const py::str& folder);
std::optional<py::str> _build32(Built& b, const py::str& path, const decompile::c_text::Module32& m, const py::str& folder);

// Compare the linked image with the original: format, exports, imports, resources.
Built& check(Built& b);

// Call the built module's exported functions in the emulator, and the original's beside.
Built& run(Built& b, int64_t limit = RUN_LIMIT);

using Log = std::function<void(const py::str&)>;

// Build a module whole. -> Built.  A 16-bit module's functions rebuilt from C are then each run beside the original's (the gate); one that does
// not do what the original does is rebuilt from its own instructions, and the module built again (b.gated). CB_NO_GATE=1 leaves them.
// Raises py::SystemExit for a module not in the corpus.
Built build(const py::str& module, const std::optional<py::str>& outdir = std::nullopt, bool call = true, const Log& log = nullptr,
            const std::optional<py::dict<py::str, py::str>>& gated = std::nullopt, bool _gate = false);

// Several modules built side by side -> {module: Built}, in the order given. `outdir_of(module)` names each folder.
py::dict<py::str, Built> build_many(const py::list<py::str>& modules,
                                    const std::function<std::optional<py::str>(const py::str&)>& outdir_of = nullptr, bool call = true,
                                    const Log& log = nullptr, const std::optional<int64_t>& jobs = std::nullopt);

// python -m codebreaker build <module> [--out DIR] [--no-run] [--engine] [--no-engine]; -> exit status
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::build::module
