// exact/project.hpp -- one program being rebuilt: a directory and its plain-text descriptions.
//
// A project is a folder holding project.json (what the program is and how it was built), src/SYMBOLS (the names the
// program's places have), the source files (src/fADDR.c, one function each; or whole files placed by src/ORDER) and
// what the stages write (src/LEDGER, src/MODULES, src/CRTLIB). This module reads all of it once and is what the stages
// are driven from: each stage (modules, library, data, ledger, identify, standin, recut, linkwhole, readable, draft,
// compare, batch, pascal_join) declares the small interface it needs of "a project", and Project answers each of them
// through an adapter it hands out (`as_modules()`, `as_ledger()` ...). No stage knows another's view, and none knows
// this class.
//
//     project.json   {"image": the original program (a path from the project's directory),
//                     "toolchain": profile name, "variant": the compiler's variant,
//                     "toolchains": [{"label", "toolchain", "variant"}] instead, when the program's
//                         compiler is not in hand and several come near it (a function counts when one
//                         of them gives its bytes),
//                     "switches": the switches most files had,
//                     "dgroup": image offset of the data segment (DGROUP; numbers may be written as text: "0x61F0"),
//                     "code_segments": [image offsets where the code segments begin] (default [0]), or "relocs": the
//                         segments the relocation table points at,
//                     "loadseg": the paragraph the image's segment words were made for (default 0),
//                     "starts": a file (from the project's directory) listing where every function
//                         begins, one hex address a line: a function is then only identical when it
//                         reaches the next (it is not shorter than the program's),
//                     "code_end": where the code ends (with "starts"),
//                     "src": the sources' folder (default "src"),
//                     "include": [folders of headers searched before the kit's],
//                     "also": [{...}] more sets of sources: each an override of the keys above (its own "src",
//                         "sources" pattern, "toolchain" ...), an assembler's modules beside a compiler's files}
//     src/SYMBOLS    name  C|D|S  address        (C code: image offset; D data: DS offset; S a segment)
//     src/fADDR.c    one function each (its first function is at ADDR)
//     src/LEDGER     what each function file came to
//
//     auto services = std::make_shared<project::Services>();
//     services->toolchains = &toolchains;                    // exact/toolchain's Toolchains
//     auto p = project::Project::open(folder, services);
//     p->image();  p->symbols();  p->sources();  p->toolchain();
//     modules::run(*p->as_modules(), args);                  // a stage driven from the project
//
// What the project cannot do itself comes in through `Services`: the compilers (exact/toolchain), the function finder
// (carve), the inverse models and the prover of the draft stage. A member that
// needs one says so with a UsageError when it is not given.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "core/bytes.hpp"
#include "exact/batch.hpp"
#include "exact/compare.hpp"
#include "exact/data.hpp"
#include "exact/draft/package.hpp"
#include "formats/exepack.hpp"
#include "exact/identify.hpp"
#include "exact/ledger.hpp"
#include "exact/library.hpp"
#include "exact/linkwhole.hpp"
#include "exact/modules.hpp"
#include "exact/pascal_join.hpp"
#include "exact/readable.hpp"
#include "exact/recut.hpp"
#include "exact/standin.hpp"
#include "exact/symbols.hpp"
#include "exact/toolchain.hpp"
#include "exact/x86.hpp"

namespace cb::exact::project {

// ---- project.json ----------------------------------------------------------------------------------------------------

// One of "toolchains": a compiler that comes near the program's.
struct ToolchainSpec {
    std::string label;                       // default: the toolchain's name
    std::string toolchain;
    std::optional<std::string> variant;
};

// A library the project links: its tag in src/CRTLIB and its path (from the project's folder, or "KIT:path").
struct LibraryRef {
    std::string tag;
    std::string path;
};

// project.json as a struct: every key the exact stages read has its field, a key the file lacks has the default the
// stages assume. Numbers the file writes as text ("0x20") are read as numbers.
struct Config {
    std::string image;                                   // the original program (required)
    std::string src = "src";
    std::vector<std::string> include;
    std::optional<std::int64_t> dgroup;                  // none: not in the file (a small program's relocations show it)
    std::optional<std::vector<std::uint32_t>> code_segments;
    bool code_segments_from_relocs = false;              // "code_segments": "relocs"
    std::uint32_t loadseg = 0;
    std::uint32_t code_end = 0;
    std::uint32_t code_start = 0;
    std::uint32_t data_end = 0;
    std::string code = "near";                           // "near" | "far"
    std::optional<std::string> switches;
    std::optional<std::string> starts;                   // a file listing where every function begins
    std::vector<std::uint32_t> starts_add, starts_drop;  // functions carve finds wrongly, mended
    std::optional<std::string> packed;                   // the original as EXEPACK left it
    std::string sources = R"(f([0-9a-f]{5})\.c)";        // the pattern of the one-function files' names; group 1 is the address
    std::optional<std::string> place_by;
    std::optional<std::string> unplaced;
    std::optional<std::string> names;
    std::optional<std::string> named_by;
    std::optional<std::string> first_segment;
    std::optional<std::string> toolchain;
    std::optional<std::string> variant;
    std::vector<ToolchainSpec> toolchains;
    bool whole = false;
    bool recut = false;
    std::optional<std::string> link_order;
    std::string model = "S";                             // the library's memory model
    std::uint32_t library_gap = 4096;
    std::optional<std::uint32_t> library_start;
    std::vector<LibraryRef> libraries;
    std::vector<std::string> first_objects;
    std::vector<std::string> after_library;
    std::map<std::string, std::uint32_t> data_sizes;
    std::vector<library::Project::DataPad> data_pads;    // "data_pads": {DS offset (hex): length}, by offset
    std::vector<std::string> also;                       // each entry of "also" (an override), as JSON text
    linkwhole::LinkConfig link;                          // "link" (the default when the file has none)
    pascal_join::Config pascal;                          // "root", "over", "map", "kinds", "procedures", "units", "equivalent", "compilands"
    std::map<std::string, std::string> strings;          // every top-level entry that is a string (config_text reads these)
};

// The config in `json_text` (a project.json). ParseError for text that is not one, FormatError for a missing "image".
Config parse_config(std::string_view json_text);

// ---- the toolchain profile's parts the project reads --------------------------------------------------------------

struct CarveSpec {                                       // "carve"
    std::vector<std::string> prologues;
    std::optional<std::string> prologue_call;
    bool frames = true;
    bool calls = true;
    bool emulator_ints = true;
};
struct LinkSpec {                                        // "link"
    std::optional<std::string> kit, run, dir;
    std::map<std::string, std::vector<std::string>> libs;
    std::vector<std::string> copy;
    std::optional<std::string> exemod;
    std::int64_t psp = 0x1000;
    std::vector<std::string> names = {"_edata", "_end"};
};
struct Communal {                                        // "communal"
    std::string segment;
    bool para = false;                                   // "align": "para"
};
struct ProfileParts {
    std::string name;
    std::optional<std::string> kit;
    CarveSpec carve;
    std::optional<LinkSpec> link;
    std::optional<Communal> communal;
    std::vector<std::string> headers = {".h"};           // the extensions of the headers beside the sources
    std::map<std::string, std::vector<LibraryRef>> libraries;   // by memory model
    std::map<std::string, std::vector<std::string>> objects;    // by memory model
    std::optional<std::string> inverse;                  // the inverse model the draft stage reads the code with
    std::string data_order = "after_next_function";      // where a module's data stood
};
// The parts of the profile `name` (profiles/NAME.json of the locations). NotFoundError / ParseError.
ProfileParts read_profile_parts(const toolchain::Locations& locations, std::string_view name);

// ---- what the project is given --------------------------------------------------------------------------------------

class Project;

// One function carve finds in the program.
struct CodeFunction {
    std::uint32_t address = 0, length = 0;
};

// The prover of the draft stage, bound to its project: the texts the model gave, held to the program.
using ProveTexts = std::function<draft::ProveOutcome(draft::InverseModel&, const draft::ModelTexts&, std::uint32_t address,
                                                     std::uint32_t length, const std::map<std::uint32_t, std::string>& names)>;

// What reaches outside the project. Shared by a project and its sets; everything is optional but what a call needs.
struct Services {
    toolchain::Toolchains* toolchains = nullptr;                       // the compilers (not owned)
    const x86::Decoder16* decoder = nullptr;                           // the 16-bit decoder; none: Capstone's
    // The functions of the program (exact/carve), in address order.
    std::function<std::vector<CodeFunction>(Project&)> carve;
    // The inverse model a toolchain profile names ("inverse"), for the draft stage.
    std::function<std::shared_ptr<draft::InverseModel>(std::string_view name)> inverse_model;
    ProveTexts prove_texts;
    // The prover made for one project (draft/bind): used when `prove_texts` is not given. It holds what a project's proofs keep
    // between functions (the compiler's options, the candidates' caches, the beam's history).
    std::function<ProveTexts(const std::shared_ptr<Project>&)> prover_for;
    // The equivalence comparers a Pascal project's "equivalent" names (habits).
    std::map<std::string, pascal_join::Equivalent> equivalents;
};

// A toolchain of the project and the label the ledger says it by.
struct NamedToolchain {
    std::string label;
    std::shared_ptr<toolchain::Toolchain> toolchain;
};

// A source file of the project: the address of its first function, or -1 for a file with none in its name.
struct Source {
    std::int64_t at = 0;
    std::filesystem::path path;
};

// The 14 words of an MZ header (e_magic ... e_lfarlc, e_ovno).
using MzHeader = std::array<std::uint16_t, 14>;

// What standin reads of the project: the object it rebuilds is held to this. `program()` is valid as long as this lives.
struct StandinInput {
    SymbolTable symbols;
    ByteView image;
    compare::Program compare;
    std::vector<std::uint32_t> relocations;
    standin::Program program() const { return standin::Program{image, symbols, compare, relocations}; }
};

// What recut reads of the project. `original()` is valid as long as this lives.
struct RecutInput {
    SymbolTable symbols;
    std::optional<std::vector<std::uint32_t>> packed_relocs;
    std::optional<recut::RelocTable> table;
    ByteView file;
    recut::Original original() const { return recut::Original{symbols, packed_relocs, table, file}; }
};

// ---- the project --------------------------------------------------------------------------------------------------------

class Project : public std::enable_shared_from_this<Project> {
public:
    // Open the project in `folder`. `override_json` (a JSON object as text) is what a set of the project's sources
    // replaces of project.json: the keys it names, "also" dropped. Throws NotFoundError (no project.json, no program),
    // ParseError (project.json is not one), FormatError / TruncatedError (a damaged program header).
    static std::shared_ptr<Project> open(const std::filesystem::path& folder, std::shared_ptr<const Services> services = nullptr,
                                         const std::optional<std::string>& override_json = std::nullopt);

    Project(const Project&) = delete;
    Project& operator=(const Project&) = delete;

    const std::filesystem::path& dir() const { return dir_; }          // the project's folder, absolute
    const std::filesystem::path& exe() const { return exe_; }          // the original program's file
    const std::filesystem::path& src() const { return src_; }          // the sources' folder
    const std::vector<std::filesystem::path>& include() const { return include_; }
    const Config& config() const { return config_; }
    const Services& services() const { return *services_; }
    std::shared_ptr<const Services> services_ptr() const { return services_; }

    std::int64_t dgroup() const { return dgroup_; }                    // image offset of the data segment
    const std::vector<std::uint32_t>& code_segments() const { return code_segments_; }   // ascending
    std::uint32_t loadseg() const { return loadseg_; }
    std::uint32_t code_end() const { return code_end_; }               // 0: not given
    const std::optional<std::string>& switches() const { return switches_; }
    void set_switches(const std::optional<std::string>& s) { switches_ = s; }   // identify tries candidates this way
    ByteView file() const { return file_; }                            // the program's file
    ByteView image() const { return image_; }                          // the load image
    const std::optional<MzHeader>& header() const { return header_; }  // none: not an MZ program
    const std::optional<formats::exepack::Original>& packed() const { return packed_; }   // the original as EXEPACK left it
    // The places in the load image where the loader adds the load segment.
    const std::unordered_set<std::uint32_t>& relocs() const { return relocs_; }
    // The image offset of the code segment `a` is in.
    std::uint32_t code_base(std::uint32_t a) const;

    // This project, then the other sets of sources project.json lists ("also").
    std::vector<std::shared_ptr<Project>> sets();

    // Where the program's own code ends (the last of the functions carve finds; 0 when it finds none). UsageError
    // when the services have no carve.
    std::uint32_t own_end();
    // The functions carve finds. UsageError when the services have no carve.
    std::vector<CodeFunction> functions();
    // Where every function begins, from the "starts" file, ascending; none when the project has none.
    std::optional<std::vector<std::uint32_t>> starts() const;

    // The compilers. UsageError when the services have no Toolchains or the project names none.
    std::vector<NamedToolchain> toolchains() const;
    std::shared_ptr<toolchain::Toolchain> toolchain() const;           // the first
    // The parts of the first toolchain's profile (read once).
    std::shared_ptr<const ProfileParts> profile_parts() const;
    std::shared_ptr<const ProfileParts> profile_parts(std::string_view toolchain_name) const;

    std::filesystem::path path(std::string_view name) const { return src_ / std::string(name); }   // a file of src/
    SymbolTable symbols() const;                                       // src/SYMBOLS (empty when there is none)
    void save_symbols(const SymbolTable& symbols) const;
    // The one-function files and the whole files src/ORDER lists, by address.
    std::vector<Source> sources() const;
    // Does the file's code lie in one piece (a file src/ORDER lists; any file of a project that says "whole")?
    bool whole(const std::filesystem::path& file) const;
    // The headers of the sources' folder, by name: the files with one of the toolchain profile's header extensions.
    std::map<std::string, Bytes> headers() const;
    std::map<std::string, Bytes> headers_with(const std::vector<std::string>& extensions) const;
    modules::CompileOptions compile_options() const;                   // headers, include, switches
    batch::CompileOptions batch_options() const;

    // project.json with entry[key][sub] = value set (the object created when it is not there) and written back; this
    // project's config is read again. What the link's fit does to "data_sizes" and "data_pads".
    void set_entry(std::string_view key, std::string_view sub, std::int64_t value);

    // What comparing a function reads of the program (valid while the project lives).
    compare::Program program() const;

    // ---- the stages' views of the project (each valid while its shared_ptr lives; they keep the project alive) ----
    std::shared_ptr<const modules::Project> as_modules();
    std::shared_ptr<const library::Project> as_library();
    std::shared_ptr<const data::Project> as_data();
    std::shared_ptr<const ledger::Project> as_ledger();             // identify takes the same view
    std::shared_ptr<const linkwhole::Project> as_linkwhole();
    std::shared_ptr<const readable::Project> as_readable();
    std::shared_ptr<draft::Project> as_draft();
    // identify: what it needs besides the ledger's view (the locations, the toolchain loader, the switches setter).
    identify::Services identify_services();
    StandinInput standin_input() const;
    RecutInput recut_input() const;
    // pascal_join's view; UsageError when the project has no "compilands" or its toolchain does not compile Pascal.
    pascal_join::Project pascal_project() const;

private:
    Project(std::filesystem::path folder, std::shared_ptr<const Services> services, std::optional<std::string> override_json);
    void read_config();
    void read_program();
    void derive_code_segments();

    std::filesystem::path dir_, exe_, src_;
    std::vector<std::filesystem::path> include_;
    std::shared_ptr<const Services> services_;
    std::optional<std::string> override_;
    Config config_;
    std::int64_t dgroup_ = 0;
    std::vector<std::uint32_t> code_segments_;
    std::uint32_t loadseg_ = 0, code_end_ = 0;
    std::optional<std::string> switches_;
    Bytes file_;
    ByteView image_;
    std::optional<MzHeader> header_;
    std::optional<formats::exepack::Original> packed_;
    std::unordered_set<std::uint32_t> relocs_;
    std::optional<std::uint32_t> own_end_;
    mutable std::mutex parts_mutex_;
    mutable std::map<std::string, std::shared_ptr<const ProfileParts>> parts_;
};

}  // namespace cb::exact::project
