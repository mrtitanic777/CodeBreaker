// build/module.hpp -- build a whole module from its reconstruction, and check what came out.
//
//     ModuleBuilder builder(services);
//     Built b = builder.build("lzexpand.dll");        // b.image is the linked file
//     cb::out("{}\n", b.report());
//
// The per-function builds (build/functions) make a throw-away DLL to judge functions in: it has no data, no resources, no
// entry table of its own. This makes the module: the same decompiled functions, compiled the same way, plus the module's
// data as C (build/data), its resources compiled from `.rc` (build/resources) and its exports and imports
// (build/definitions), linked by OpenWatcom into a 16-bit NE or 32-bit PE image. Then the image is checked against the
// original:
//
//     format      the same kind of file, library or program as the original
//     exports     every original export present, under the same ordinal and name
//     imports     nothing imported that the original does not import
//     resources   the same resources (build/resources compare)
//     runs        loaded in the emulator, each exported function is called with zero arguments and what it did is set
//                 beside what the original does
//
// What "built" means here, plainly: a function the compiler rejected is in the image as a stand-in that returns 0, and so
// is anything called that has no reconstruction; a module can link, export everything and still do little. The report
// counts both. The check is of the container, not of behaviour -- behaviour is what verify/ judges, one function at a time.
//
// Toolchain facts this relies on (each cost a failed build once):
//
//   * OpenWatcom's tools are started by full path: Windows searches the caller's PATH, not the environment handed to the tool.
//   * 16-bit: the C library to link is lib286/win/clib?.lib. lib286/clibl.lib one level up is an empty stub, and it is found
//     first if lib286 is on the library path before lib286/win.
//   * 16-bit: floating point needs -fpi87 (the emulator library lacks the fix-up symbols -fpi asks for, and -fpc does not
//     work in this build).
//   * 16-bit: a module is a library because the linker is told `format windows dll` -- that sets the header's library flag
//     without the stock start-up object, which wants a LIBMAIN. The header flag, not the file name, says what the original was.
//   * 16-bit: one segment holds 64 KB, and a near call cannot leave its segment, so each function is compiled into a code
//     segment named for the original segment it came from (-nt=). The compiler's own helpers (32-bit multiply and divide,
//     ...) then cannot be near either: the functions are compiled for the medium model (-mm: far code, near data) and linked
//     with the medium-model library.
//   * 16-bit: the data object is linked first, so it starts at offset 0 of the automatic data segment and the code's
//     offsets into it stay right.
//   * Linker symbols holding `@`, `?` or `$` are quoted (unquoted, `@` means "read a file").
//   * Exports are tied to the function at the export's address, and the entry point to the function at the original's
//     entry address; a library with no entry point is not loadable as one, so one whose entry was not reconstructed gets a
//     stand-in that reports success.
//
// What a build needs from its surroundings is given, not looked up (`Services`): where the corpus and the state are, the
// tools, the function builder and its cache, and the pieces of CodeBreaker that make a module's facts (the decompiled and
// made-compilable module, its interface, the origin of each function, the emulators and the gate that judges a rebuilt
// function). A test gives recorded answers.
#pragma once

// <windows.h> defines `near` and `far` as empty macros; this header's neighbours use them as names.
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "analyze/origin.hpp"
#include "decompile/data.hpp"
#include "build/definitions.hpp"
#include "build/functions.hpp"
#include "build/resources.hpp"
#include "toolchain/watcom.hpp"
#include "core/arena.hpp"
#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "core/ordered_map.hpp"
#include "core16_types.hpp"
#include "decompile/c_text.hpp"
#include "source/assembly.hpp"

namespace cb::build::module {

// Exported functions called when a built module is run.
inline constexpr std::size_t kRunLimit = 40;

// Where, in the original, a function is: a virtual address (32-bit) or segment and offset (16-bit).
using Address = std::variant<std::uint64_t, core16::SegOff>;

// ---- what a build found out ---------------------------------------------------------------------------------------------

// One exported function called with zero arguments in the built module and in the original.
struct RanExport {
    std::string name;
    std::string rebuilt, original;   // how each ended
    bool same = false;               // the same end and the same result
};

// The original's bytes the C of a code segment reads through CS, put in that segment, and the functions that read them.
struct CodeDataSegment {
    std::int64_t segment = 0;
    std::vector<std::string> readers;
};

// A function whose callers contradict how it is described: who removes its arguments.
struct Dispute {
    std::string function, who;
};

// A hole of a frame (the place of a function rebuilt from C) and what the function needed there.
struct FrameFit {
    std::string function;
    std::int64_t hole = 0, size = 0;
};

// The symbol a function's object defines and the object, relative to the build folder.
struct PublicSymbol {
    std::string symbol, object;
};

// A build and what was found out about it.
struct Built {
    std::filesystem::path original;
    std::string name;                                  // the file's name, lower case
    std::optional<std::string> kind;                   // the corpus's lane of the original: "pe", "ne" ...
    std::optional<std::filesystem::path> folder;       // where the build was made
    std::optional<std::filesystem::path> image;        // the linked file (nullopt: not linked)
    std::string error;
    std::size_t functions = 0;                         // in the module
    std::size_t with_text = 0;                         // given C by the decompiler
    OrderedMap<std::string, std::string> rejected;     // {function: why the compiler rejected it} -- stand-ins
    std::vector<std::string> absent;                   // called, but nothing reconstructed: stand-ins too
    OrderedMap<std::string, std::string> assembled;    // 16-bit: {function: why} rejected C, written as its own instructions
    OrderedMap<std::string, std::string> assembly_refused;   // 16-bit: why a rejected function could not be
    std::vector<std::string> undefined;                // left unresolved in the image
    std::shared_ptr<const decompile::data::Data> data;
    std::string data_error;
    std::size_t lost_addresses = 0;                    // address constants in functions that could not be placed
    std::optional<resources::Rebuilt> resources;       // of writing and compiling them
    std::string entry;
    bool format_same = false;
    std::size_t exports_total = 0, exports_same = 0, exports_on_stand_ins = 0;
    std::vector<definitions::MissingExport> exports_missing;
    std::vector<definitions::Import> imports_extra;    // not in the original, sorted by their text
    std::size_t imports_original = 0, imports_built = 0;
    std::optional<bool> resources_same;                // nullopt: the original has none
    std::string resources_line;
    std::vector<RanExport> ran;
    std::set<Address> stand_in_addresses;              // where, in the original, the rejected functions are

    // What the builders add while they work.
    OrderedMap<std::string, std::string> gated;        // {function: how its C differs from the original}: rebuilt from instructions
    std::vector<CodeDataSegment> code_data;
    std::vector<Dispute> disputes;
    std::optional<std::string> contract_error;         // the call-contract check failed
    std::optional<std::string> gate_error;             // the gate could not run some functions
    std::vector<core16::SegOff> hidden;                // functions the description does not have
    std::vector<std::string> lead_ins;                 // 16-bit: object stems linked first into their segment, in order
    std::set<std::string> placed_c;                    // 16-bit: functions kept as C (a frame's pieces fit)
    std::set<std::string> moved_c;                     // 16-bit: functions moved to the segment beside theirs
    OrderedMap<std::string, std::string> resident_overflow;   // {function: why its C finds no room below where a segment is cut}
    std::vector<FrameFit> frame_fit;
    std::string frame_why;                             // why a frame's pieces did not assemble back
    std::map<std::string, std::string> elsewhere_why;  // {function: why the compiler rejected its far copy}
    std::set<std::string> near_names;                  // near functions (a near one stays in its segment)
    std::map<std::int64_t, std::int64_t> resident_limit;   // {segment: where it is cut after start-up}
    bool byte_aligned_code = false;                         // the functions' objects are byte-aligned (-os): no pad byte before one at an odd place
    std::map<std::string, PublicSymbol> publics;       // {function: the symbol the linker sees}
    std::map<std::string, analyze::origin::Origin> origins;   // by function name
    std::optional<std::string> layout;                 // "segments renumbered: ..." when the segments differ
    std::map<std::int64_t, Bytes> carried_segments;    // 16-bit: the data segments the build carried that the module did not
    std::shared_ptr<Arena> l2_arena;                   // owns `l2`
    translate::ir::Node* l2 = nullptr;                 // the module as one L2 tree
    std::optional<std::string> carried_report;         // a build made in another process: its report as that process wrote it

    Built() = default;
    // `kind_of` is the corpus's lane name of the original.
    Built(std::filesystem::path original, std::optional<std::string> kind_of);

    bool linked() const { return image && !image->empty(); }
    // What was found, as lines of text.
    std::string report() const;
};

// ---- the module as one L2 tree ------------------------------------------------------------------------------------------

// A 16-bit module as one L2 tree: a Module whose units are its functions in address order, each as it came back -- one
// rebuilt from C as its L2 document (its MethodDecl; else its name and place alone), one kept as its instructions as a
// MachineFragment: its place, length, why, and the assembly the build wrote for it (a run of them assembled as one file:
// the text on the first, the others `within` it). `assembled`: {name: why} (Built::assembled); `stems`: {name: object stem};
// `lengths`: {segment: bytes}, for the last function of each. The nodes are made in `arena`.
translate::ir::Node* module_document(Arena& arena, std::string_view module, const decompile::c_text::Module16& m,
                                     const OrderedMap<std::string, std::string>& assembled,
                                     const std::map<std::string, std::string>& stems, const std::filesystem::path& folder,
                                     const std::map<std::int64_t, std::int64_t>& lengths = {});
// A 32-bit module as one L2 tree: its functions in address order, each its L2 document placed by its address; one with no
// L2 (the decompiler could not make it) its name and place alone.
translate::ir::Node* module_document32(Arena& arena, std::string_view module, const decompile::c_text::Compilable32& m);

// The stem of an object from its C stem (`cstem` {name: stem}): the name that has it.
std::optional<std::string> name_of_stem(std::string_view stem, const std::map<std::string, std::string>& cstem);

// {name: why} for the functions of `module` that kept_as_asm.txt keeps as their instructions: C the gate passes that a run of
// the whole module showed wrong. The list is for those the decompiler cannot yet write right; each line goes when it can.
// `text` is the file's: lines of `module function why`, `#` starting a comment.
OrderedMap<std::string, std::string> kept_as_asm(std::string_view text, std::string_view module);

// A far pointer to one of the module's functions that the decompiler left as two plain stores -- a window class built on the stack, `lpfnWndProc` as
// `xStack_1a = 0x1587; xStack_18 = 0x1048;` (the offset and the selector the decompiler gives the function's segment) -- written as the function's address:
// `xStack_1a = (uint2)(uint4)(void __far *)&BEAR306; xStack_18 = (uint2)((uint4)(void __far *)&BEAR306 >> 16);`. As numbers they keep the function's
// ORIGINAL offset (the selector alone is fixed up), so a function rebuilt from C that lands elsewhere is registered at an address with other code, or the
// filler, behind it: BEAR306 and sub_25_0429 of USER, registered as window procedures that way, stopped the boot for that reason and no other.
// `lookup` gives the name of the far function whose address (selector << 16 | offset) is the value, or nothing. The two stores are consecutive lines, in either order.
std::string name_function_pointer_stores(std::string_view text, const std::function<std::optional<std::string>(std::uint64_t)>& lookup);
// The same where the selector is already written as the segment of a function (the decompiler does that for a loader fix-up):
// `a = 0x1587; b = ((uint2)((uint4)(void __far *)&sub_10_009C >> 16));` -- the offset is then the offset of a function in sub_10_009C's segment, and is written
// as that function's address. `lookup(anchor, offset)` gives the far function at that offset of the anchor's segment, or nothing.
std::string name_function_offsets_beside_selectors(std::string_view text,
                                                   const std::function<std::optional<std::string>(const std::string& anchor, std::uint64_t offset)>& lookup);

// An original whose MZ header has DOS load the whole file (KRNL386: its real-mode start reads the NE header and the segments
// out of memory): ours says the same of ours.
void whole_file_image(const std::filesystem::path& original, const std::filesystem::path& image);

// ---- where a build asks the rest of CodeBreaker -------------------------------------------------------------------------

// A module made compilable, as the lane of its kind gives it.
struct Prepared {
    std::size_t functions = 0;                           // every function the module has
    std::shared_ptr<decompile::c_text::Module16> win16;  // a 16-bit Windows module
    std::shared_ptr<decompile::c_text::Compilable32> win32;
};

// How a call of an exported function in the emulator ended.
struct CallOutcome {
    std::string ended;                 // "ret", or what stopped it
    std::optional<std::int64_t> result;
};

// What the gate says of one function of a rebuilt module run beside the original's.
struct GateResult {
    enum class Verdict { Same, Divergent, Inconclusive };
    std::string name;
    Verdict verdict = Verdict::Same;
    std::string why;
};

struct Services {
    // ---- the tools
    functions::FunctionBuilder* builder = nullptr;       // compiles, and keeps what compiles made
    functions::BuildCache* cache = nullptr;              // the shared build cache (may be null: nothing is kept)
    toolchain::watcom::OpenWatcom open_watcom{std::filesystem::path()};
    source::assembly::Prober* prober = nullptr;          // which instruction texts the assembler turns back into the original's bytes

    // ---- the corpus and the state
    std::function<std::optional<std::filesystem::path>(std::string_view name)> retail;        // a corpus name to its file
    std::function<std::optional<std::string>(const std::filesystem::path&)> kind_of;          // "ne", "pe" ...
    std::function<std::filesystem::path(std::string_view name)> build_folder;                 // <state>/build/<name>
    std::filesystem::path kept_as_asm_file;                                                  // build/kept_as_asm.txt

    // ---- what is known of a module
    // The module made compilable (decompiled, its C written, its facts), as the lane of its kind gives it.
    std::function<Prepared(const std::filesystem::path& module, const std::filesystem::path& work)> prepare;
    // A module's interface: its exports, imports, the names it calls itself.
    std::function<definitions::Interface(const std::filesystem::path&)> interface_of;
    // The origin (C or assembly, and why) of each function of a 16-bit module, by address; `by_va` names them.
    std::function<std::map<std::int64_t, analyze::origin::Origin>(const std::filesystem::path& module,
                                                                  const std::map<std::int64_t, std::string>& by_va)>
        origins16;
    // The starts of the functions the lane carves from a 32-bit module (for the address constants of its code).
    std::function<std::vector<std::uint64_t>(const std::filesystem::path&)> functions32;
    // The link lines (import statements) an object file of a 32-bit module needs for the imports its code calls.
    std::function<std::vector<std::string>(ByteView object, const std::filesystem::path& module, const std::set<std::string>& used)>
        import_lines32;

    // ---- the checks
    // An exported function of a module loaded in the emulator, called with zero arguments.
    std::function<CallOutcome(const std::filesystem::path& module, std::string_view kind, const definitions::Export&)> call_export;
    // The gate: the named functions of a 16-bit build run beside the original's, every call out answered alike on both sides.
    std::function<std::vector<GateResult>(const std::filesystem::path& module, const std::filesystem::path& folder,
                                          const std::vector<std::string>& names)>
        gate_check;
    // The calls of the functions of a built image beside the original's: {function: how its calls differ}. Throws when it cannot say.
    std::function<std::map<std::string, std::string>(const std::filesystem::path& module, const std::filesystem::path& folder,
                                                     const std::filesystem::path& image, const std::vector<std::string>& names)>
        gate_contract;
    // Everything the gate's verdicts depend on besides the module and the function (the checker, the emulators, the explorers
    // and their limits), as text: a verdict is kept by it.
    std::function<std::string()> gate_stamp;

    // ---- running
    // A build made in a process of its own (several modules at once): the Built it made, or nullopt when processes cannot be started.
    std::function<std::optional<Built>(const std::string& module, const std::optional<std::filesystem::path>& outdir, bool call)> build_apart;
    // Where a module built in a project goes (Converted/<name>/build) and how the project is told of it.
    std::function<std::filesystem::path(const std::filesystem::path& module)> project_build_folder;
    std::function<void(const std::filesystem::path& module, const std::string& note)> record_build;
    // How many modules are built at once (the budget of the machine).
    std::function<int()> workers;
    // Where progress lines go (default: standard output).
    std::function<void(std::string_view)> log;
};

// Options of one build.
struct BuildOptions {
    std::optional<std::filesystem::path> outdir;         // where the built image is put, alone (the working files are under Services::build_folder)
    bool call = true;                                   // run the built module's exports beside the original's
    std::function<void(std::string_view)> log;           // default: Services::log
    // The functions already known to differ from the original ({function: how}); with them given the gate does not run
    // unless `gate` says so (it is how a build is repeated).
    std::optional<OrderedMap<std::string, std::string>> gated;
    bool gate = false;
};

// The builder. It holds no state of its own beyond the services: two builds may run at once.
class ModuleBuilder {
public:
    explicit ModuleBuilder(Services services) : services_(std::move(services)) {}
    const Services& services() const { return services_; }

    // Build a module whole. A 16-bit module's functions rebuilt from C are then each run beside the original's (the gate:
    // on their own, every call out answered alike on both sides); one that does not do what the original does is rebuilt from
    // its own instructions, and the module built again (Built::gated). CB_NO_GATE=1 leaves them. UsageError for a module that is
    // not in the corpus or is of a kind that is not linked whole here.
    Built build(std::string_view module, const BuildOptions& options = {}) const;

    // Several modules built side by side, as many at once as the machine's budget, the largest first, in the order given.
    std::vector<std::pair<std::string, Built>> build_many(const std::vector<std::string>& modules,
                                                          const std::function<std::optional<std::filesystem::path>(const std::string&)>& outdir_of = {},
                                                          bool call = true) const;

    // The two linkers: link `m` (compiled with the platform's tools) into a file in `folder`; the path, or nullopt when nothing was
    // linked (`b.error` says why). They fill `b`.
    std::optional<std::filesystem::path> build16(Built& b, const std::filesystem::path& original, const decompile::c_text::Module16& m,
                                                 const std::filesystem::path& folder) const;
    std::optional<std::filesystem::path> build32(Built& b, const std::filesystem::path& original, const decompile::c_text::Compilable32& m,
                                                 const std::filesystem::path& folder) const;

    // Compare the linked image with the original: format, exports, imports, resources.
    void check(Built& b) const;
    // Call the built module's exported functions in the emulator, and the original's beside.
    void run(Built& b, std::size_t limit = kRunLimit) const;

    // {function: how it differs} when the gate's verdict is kept for every function the module has as C text (the last build's
    // set, recorded with it), else nullopt.
    std::optional<OrderedMap<std::string, std::string>> gate_known(const std::filesystem::path& module,
                                                                   const decompile::c_text::Module16& m) const;
    // {function: how it differs} for the named functions rebuilt from C, each run beside the original's. A function's verdict is kept
    // by what makes it: the original module, the function's C and the header it compiles with, the compiler and its options, and the
    // checker; a function the same as in an earlier build is not run again. `not_run` takes the ones the gate gave no verdict for.
    OrderedMap<std::string, std::string> gate(const std::filesystem::path& module, const std::filesystem::path& folder,
                                              const std::vector<std::string>& names, const decompile::c_text::Module16& m,
                                              std::vector<std::pair<std::string, std::string>>* not_run = nullptr) const;
    // The functions the description says otherwise of than their callers do, who removes the arguments: a report, never an error
    // (a failure is one entry "(not checked)").
    std::vector<Dispute> disputes(const decompile::describe::NeDescription& d) const;

    // `build <module>... [--out DIR] [--no-run]`
    cli::Command command() const;

private:
    Services services_;
};

}  // namespace cb::build::module
