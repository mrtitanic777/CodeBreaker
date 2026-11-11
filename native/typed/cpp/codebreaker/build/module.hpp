// codebreaker/build/module.hpp -- interface of codebreaker.build.module (typed C++; implementation pending).
//
// module.py -- build a whole module from its reconstruction, and check what came out.
//
//     build(module)            -> Built; b.image is the linked file; b.report() says what was found
//     check(b), run(b)         compare the linked image with the original / call its exports beside the original's
//     module_document(...)     the whole module as one L2 tree (docs/IR_UNIFICATION.md step 4)
//
// The per-function builds (build/functions) make a throw-away DLL to judge functions in; this makes the module:
// the same decompiled functions plus the module's data (build/data), resources (build/resources), exports and
// imports (build/definitions), linked by OpenWatcom into a 16-bit NE or 32-bit PE image.
//
// Typing notes:
//   * The decompiled-and-made-compilable module the builders read (the Python's `m`, a translate.l2.Made carrying
//     the lane's facts as attributes) is decompile::c_text::Module16 / Module32 (c_text.hpp): the 16-bit and 32-bit
//     builders each take their kind's. What the lanes add to it (key, all_keys, sig; for 32-bit `va`, the inverse of
//     by_va, which _build32 makes from by_va itself) is verify::reconstruct::Prepared / the lane's own extras.
//     build() gets the module from the kind's prepare() as reconstruct::Prepared and takes Prepared::lane back
//     to its Module16 / Module32 (a std::shared_ptr<void> that the kind's lane fills with it).
//   * Built grows attributes during a build (the Python adds them with `b.x = ...` / getattr(b, "x", default)):
//     all of them are members here with the Python's absent-means-default as an empty value or std::nullopt.
//   * An L2 document (the Python's dict {"node": "Module", "units": [...]}) is translate::ir::Dict, as translate/l2.hpp
//     and ir.hpp hold them.
//   * An address of an export is build::definitions::Export::address: a virtual address (32-bit) or (seg, off) (16-bit).
//   * log=print is std::function<void(const py::str&)>; the Python's `print` is the default (writes the line).
//   * SystemExit (raised for an unknown module / unset platform) is py::SystemExit.
//   * build_many() runs each module in a Python subprocess of its own in the original (pickle over a pipe); the C++
//     implementation builds in worker threads or processes as it sees fit: the contract is "{module: Built} in order".
//   * Private helpers other than those listed are not declared (they are used by build.module itself only).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <vector>
#include <set>
#include <tuple>
#include <variant>

#include "codebreaker/build/data.hpp"
#include "codebreaker/build/definitions.hpp"
#include "codebreaker/build/resources.hpp"
#include "codebreaker/core16_types.hpp"
#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::analyze::origin {
struct Origin;
}
namespace codebreaker::decompile::describe {
class Description;   // decompile/describe.hpp
}

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
    py::list<build::definitions::Import> imports_extra;     // (MODULE, name or ordinal), sorted by str()
    int64_t imports_original = 0, imports_built = 0;
    std::optional<bool> resources_same;    // None: the original has none
    py::str resources_line;
    py::list<std::tuple<py::str, py::str, py::str, bool>> ran;   // (export, rebuilt exit, original exit, same result)
    py::set<Address> stand_in_addresses;   // where, in the original, the rejected functions are

    // --- members the builders add while they work (getattr(b, "x", default) in the Python) ---
    py::dict<py::str, py::str> gated;               // {function: how its C differs from the original} (rebuilt from instructions)
    py::list<std::tuple<int64_t, py::list<py::str>>> code_data;   // (segment, the functions that read the original's data through CS)
    py::list<std::tuple<py::str, py::str>> disputes;   // (function, who the callers say removes the arguments)
    std::optional<py::str> contract_error;          // the call-contract check failed: "<Type>: <message>"
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
    py::dict<py::str, std::tuple<py::str, py::str>> _publics;   // {function: (symbol, ...)} the linker sees
    py::dict<py::str, std::shared_ptr<analyze::origin::Origin>> origins;   // analyze.origin.module16(...).functions
    std::optional<py::str> layout;                  // "segments renumbered: ..." when the segments differ
    translate::ir::Dict l2;                         // the module as one L2 tree (module_document / module_document32)
    std::optional<py::str> carried_report;          // a build made in a worker process (build_many): its report as the worker wrote it

    Built() = default;   // (so that py::dict<py::str, Built> can hold one)
    explicit Built(const py::str& original);

    bool linked() const;                   // @property: bool(self.image)
    py::str report() const;
};

// The whole module as one L2 tree: a Module whose units are its functions in address order, each as it came back
// -- one rebuilt from C as its L2 document (its MethodDecl), one kept as its instructions as a MachineFragment
// (its place, length, why, and the assembly the build wrote for it). `assembled`: {name: why} (Built.assembled);
// `stems`: {name: object stem}; `lengths`: {segment: bytes}, for the last function of each.
translate::ir::Dict module_document(const py::str& module, const decompile::c_text::Module16& m,
                                    const py::dict<py::str, py::str>& assembled,
                                    const py::dict<py::str, py::str>& stems, const py::str& folder,
                                    const std::optional<py::dict<int64_t, int64_t>>& lengths = std::nullopt);

// A 32-bit module as one L2 tree: its functions in address order, each its L2 document placed by its address
// (origin {module, va}); one with no L2 its name and place alone.
translate::ir::Dict module_document32(const py::str& module, const decompile::c_text::Module32& m);

// The stem of an object from its C stem (`cstem` {name: stem}): the name that has it.
py::str name_of_stem(const py::str& stem, const py::dict<py::str, py::str>& cstem);

// {name: why} for the functions of `module` kept as their instructions by kept_as_asm.txt.
py::dict<py::str, py::str> kept_as_asm(const py::str& module);

// An original whose MZ header has DOS load the whole file (KRNL386): ours says the same of ours.
void _whole_file_image(const py::str& original, const py::str& image);

// [(address, bytes)] for everything of a 32-bit image that is not code (the emulator maps the code itself).
py::list<std::tuple<int64_t, py::bytes>> _sections(const py::str& path);

// The two linkers: link `m` (compiled with the platform's tools) into a file in `folder`; -> its path, or None
// when nothing was linked (b.error says why). Platform slots platforms.of("ne"/"pe").packager.link.
std::optional<py::str> _build16(Built& b, const py::str& path, const decompile::c_text::Module16& m, const py::str& folder);
std::optional<py::str> _build32(Built& b, const py::str& path, const decompile::c_text::Module32& m, const py::str& folder);

// Compare the linked image with the original: format, exports, imports, resources.
Built& check(Built& b);

// Call the built module's exported functions in the emulator, and the original's beside.
Built& run(Built& b, int64_t limit = RUN_LIMIT);

// [(name, who the callers say removes the arguments)] for each function the description says otherwise of.
// (A report, not the build: never raises; a failure is one pair ("(not checked)", message).)
py::list<std::tuple<py::str, py::str>> _disputes(const decompile::describe::Description& d);

// {name: how it differs} when the gate's verdict is kept for every function the module has as C text, else None.
std::optional<py::dict<py::str, py::str>> _gate16_known(const py::str& path, const decompile::c_text::Module16& m);

// {name: how it differs} for the named functions rebuilt from C, each run beside the original's (isolated16).
py::dict<py::str, py::str> _gate16(const py::str& path, const py::str& folder, const py::list<py::str>& names,
                                   const decompile::c_text::Module16& m);

using Log = std::function<void(const py::str&)>;

// Build a module whole. -> Built.  A 16-bit module's functions rebuilt from C are then each run beside the original's
// (verify/gate16/isolated16); one that does not do what the original does is rebuilt from its own instructions,
// and the module built again (b.gated). CB_NO_GATE=1 leaves them.
// Raises py::SystemExit for a module not in the corpus.
Built build(const py::str& module, const std::optional<py::str>& outdir = std::nullopt, bool call = true,
            const Log& log = nullptr, const std::optional<py::dict<py::str, py::str>>& gated = std::nullopt,
            bool _gate = false);

// Several modules built side by side -> {module: Built}, in the order given. `outdir_of(module)` names each folder.
py::dict<py::str, Built> build_many(const py::list<py::str>& modules,
                                    const std::function<std::optional<py::str>(const py::str&)>& outdir_of = nullptr,
                                    bool call = true, const Log& log = nullptr, const std::optional<int64_t>& jobs = std::nullopt);

// A build worker process's job (jobs/workers): args = module, outdir or "-", call ("1"/"0"), answer file. Writes what a Built is read by.
int64_t build_worker(const std::vector<std::string>& args);

// python -m codebreaker build <module> [--out DIR] [--no-run] [--engine] [--no-engine]; -> exit status
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::build::module
