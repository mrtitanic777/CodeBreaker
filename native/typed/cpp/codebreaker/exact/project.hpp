// codebreaker/exact/project.hpp -- typed C++ of codebreaker.exact.project
//
// project.py -- one program being rebuilt: a directory and its plain-text descriptions.
//
//     project.json   {"image": the original program (a path from the project's directory),
//                     "toolchain": profile name, "variant": the compiler's variant,
//                     "toolchains": [{"label", "toolchain", "variant"}] instead, when the program's
//                         compiler is not in hand and several come near it (a function counts when one
//                         of them gives its bytes),
//                     "switches": the switches most files had,
//                     "dgroup": image offset of the data segment (DGROUP),
//                     "code_segments": [image offsets where the code segments begin] (default [0]),
//                     "loadseg": the paragraph the image's segment words were made for (default 0),
//                     "starts": a file (from the project's directory) listing where every function
//                         begins, one hex address a line: a function is then only identical when it
//                         reaches the next (it is not shorter than the program's),
//                     "code_end": where the code ends (with "starts"),
//                     "src": the sources' folder (default "src"),
//                     "include": [folders of headers searched before the kit's]}
//     src/SYMBOLS    name  C|D|S  address        (C code: image offset; D data: DS offset; S a segment)
//     src/fADDR.c    one function each (its first function is at ADDR)
//     src/LEDGER     what each function file came to
//
//     auto p = project::load(folder);
//     p->image  p->relocs()  p->symbols()  p->save_symbols(s)  p->toolchain()  p->sources()
//
// Typing notes (how the Python's duck-typed `prj` is typed here):
//   * `Project` is the object `prj`. Its members are the Python's: dir, exe, src, include, dgroup, code_segments, loadseg,
//     code_end, switches, file, header, image, packed, packed_file, and the methods. `cfg` (the project.json dict) is the
//     struct Config: every key the exact package reads has its field (a key the file lacks is the Python's default); the
//     whole dict (with the override of an "also" set merged) is kept as JSON text in Config::text, for a key no field
//     covers (cfg_json(key) reads one back), and is what save_project writes with a changed entry.
//   * A number the file gives as an int or as text ("0x61F0": int(x, 0)) is a Num; where the Python converts it
//     (dgroup, code_segments, loadseg, code_end in __init__; the rest where they are read) so does this.
//   * `override` of Project(folder, override) (a set of the project's sources: its own folder, pattern, toolchain ...) is
//     a JSON object as text: the keys it names replace the project's, "also" is dropped.
//   * `relocs()` returns a reference to the set the project keeps. `packed` is the exepack::Packed (null when the
//     project is not packed), `header` the 14 words of the MZ header (empty: not an MZ program: the Python's None).
//   * toolchain().p of the Python (the profile dict) is read for the parts the exact package asks of it: ProfileParts
//     (carve, link, communal, headers, libraries, objects, kit, inverse, data order).  profile_parts(tc) reads them from
//     the profile's JSON file (toolchains/NAME.json).
//
// Stand-ins this type serves (the interfaces the typed exact.* ports were written against).  Each is an adapter object
// made by a function below; it reads the Project it is made from (it must outlive the adapter), and the answer to each
// of its members is the Project's member of the same meaning:
//
//   modules::Project   (as_modules)   src, path, code_end, own_end, switches, cfg_str(key) = the cfg's string entry
//                      "first_segment", "named_by", "toolchain", "variant", "sources" ... (any key's string value; None
//                      for a key that is not a string), code_segments, toolchain (modules::Toolchain: the typed
//                      toolchain), headers, include, also_sets = sets()[1:], image, symbols, dgroup, loadseg,
//                      code_base, relocs
//   library::Project   (as_library)   as modules::Project, + model, profile_libraries/profile_objects/profile_kit from
//                      ProfileParts, project_libraries = cfg.libraries, first_objects, library_gap, library_start,
//                      data_sizes, data_pads, dir; `_library` is kept in the adapter, which the Project keeps (as_library
//                      always hands out the same one: prj._library of the Python)
//   data::Project      (as_data)      as modules::Project, + data_order = profile "data"."order"
//   ledger::Project    (as_ledger)    src, dir, exe, path, cfg_str, has_compilands, pascal_project, headers, include, switches,
//                      starts, whole, image, code_end, toolchains, symbols, save_symbols, sources, sets (ledger views of
//                      sets()), dgroup, loadseg, code_base, relocs, decoder (the 16-bit decoder installed with set_decoder16)
//   identify::Project  (as_identify)  as ledger::Project + set_switches (prj.switches = ...)
//   standin::Project   (as_standin)   image, symbols, loadseg, dgroup, code_base, relocs, packed_relocs = packed.relocs
//   recut::Project     (as_recut)     symbols, packed_relocs, header, file
//   linkwhole::Project (as_linkwhole) dir, src, sources, toolchain, headers, include, switches, sets, link = cfg.link,
//                      image, file, exe, symbols
//   pascal_join::Project (pascal_project) dir, cfg (the compilands part of cfg), image, toolchain (the MSPascal toolchain),
//                      symbols, compare (dgroup, loadseg, code_base, relocs), decode
//   compare::Project   (compare_project)  dgroup, loadseg, code_base, relocs
//   batch::Project     (batch_project)    headers, include, switches
//   readable::Project (as_readable)   dir, src, symbols, sources, ledger_run, with_src
//   draft::Project     (as_draft)     toolchain_name, inverse_model, locate (library::locate), symbols, image,
//                      source_addresses, path, functions (carve::functions); prove_texts is exact.draft.prove, which has no
//                      typed port: it is the function given to set_prove_texts
//   forms::Prj of exact.draft.forms/candidates/prove: kit_dir, library_places, compile_listing: toolchain().p's "kit" and
//                      kit_dir(kit), locate(prj) (as_library), toolchain()->compile_listing
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/exact/batch.hpp"
#include "codebreaker/exact/compare.hpp"
#include "codebreaker/exact/data.hpp"
#include "codebreaker/exact/draft/package.hpp"
#include "codebreaker/exact/exepack.hpp"
#include "codebreaker/exact/identify.hpp"
#include "codebreaker/exact/ledger.hpp"
#include "codebreaker/exact/library.hpp"
#include "codebreaker/exact/linkwhole.hpp"
#include "codebreaker/exact/modules.hpp"
#include "codebreaker/exact/pascal_join.hpp"
#include "codebreaker/exact/readable.hpp"
#include "codebreaker/exact/recut.hpp"
#include "codebreaker/exact/standin.hpp"
#include "codebreaker/exact/toolchain.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::project {

using Symbols = compare::Symbols;
using Num = std::variant<int64_t, py::str>;          // an int, or a number written as text ("0x20": int(x, 0))

// int(x, 0) if it is text
int64_t num(const Num& x);

// one of "toolchains": the compilers that come near the program's
struct ToolchainSpec {
    std::optional<py::str> label;                    // (default: the toolchain's name)
    py::str toolchain;
    std::optional<py::str> variant;
};

// the project.json dict
struct Config {
    py::str image;                                   // the original program (KeyError when the file lacks it)
    py::str src = py::str("src");
    py::list<py::str> include;
    std::optional<Num> dgroup;                       // (None: "dgroup" is not in the file)
    bool has_code_segments = false;
    std::optional<py::str> code_segments_text;       // "code_segments": "relocs" (any other text: its characters are read as numbers, as the Python's iteration)
    py::list<Num> code_segments;                     // "code_segments": [...] (default [0] where the file has none)
    Num loadseg = int64_t(0);
    Num code_end = int64_t(0);
    Num code_start = int64_t(0);
    Num data_end = int64_t(0);
    py::str code = py::str("near");
    std::optional<py::str> switches;
    std::optional<py::str> starts;
    py::list<Num> starts_add;
    py::list<Num> starts_drop;
    std::optional<py::str> packed;
    py::str sources = py::str("f([0-9a-f]{5})\\.c");
    std::optional<py::str> place_by;
    std::optional<py::str> unplaced;
    std::optional<py::str> names;
    std::optional<py::str> named_by;
    std::optional<py::str> first_segment;
    std::optional<py::str> toolchain;
    std::optional<py::str> variant;
    bool has_toolchains = false;
    py::list<ToolchainSpec> toolchains;
    bool whole = false;
    bool recut = false;
    std::optional<py::str> link_order;
    py::str model = py::str("S");
    int64_t library_gap = 4096;
    std::optional<Num> library_start;
    py::list<library::LibRef> libraries;
    py::list<py::str> first_objects;
    py::list<py::str> after_library;
    py::dict<py::str, int64_t> data_sizes;
    py::dict<py::str, int64_t> data_pads;
    py::list<py::str> also;                          // each entry of "also" (an override) as JSON text
    linkwhole::LinkConfig link;                      // "link" ({} when the file has none)
    pascal_join::Config pascal_config;                       // "root", "over", "map", "kinds", "procedures", "units", "equivalent", "compilands"
    py::str text;                                    // the whole dict (the override merged) as JSON text, in the file's key order

    // the dict read from JSON text (KeyError when "image" is missing)
    static Config from_json(const py::str& json_text);
};

// ---- the parts of a toolchain profile the exact package reads (tc.p.get(...))
struct CarveSpec {                                   // "carve"
    py::list<py::str> prologues;
    std::optional<py::str> prologue_call;
    bool frames = true;
    bool calls = true;
    bool emulator_ints = true;
};
struct LinkSpec {                                    // "link"
    std::optional<py::str> kit;
    std::optional<py::str> run;                      // (KeyError where the profile has none and a link is made)
    std::optional<py::str> dir;
    py::dict<py::str, py::list<py::str>> libs;
    py::list<py::str> copy;
    std::optional<py::str> exemod;
    Num psp = py::str("0x1000");
    py::list<py::str> names = py::list<py::str>({py::str("_edata"), py::str("_end")});
};
struct Communal {                                    // "communal"
    py::str segment;
    bool para = false;                               // "align": "para"
};
struct ProfileParts {
    py::str name;
    std::optional<py::str> kit;                      // p["kit"]
    CarveSpec carve;                                 // p.get("carve", {}) (the defaults when none)
    std::optional<LinkSpec> link;                    // p.get("link"): none when absent or empty
    std::optional<Communal> communal;                // p.get("communal"): none when absent or empty
    py::list<py::str> headers = py::list<py::str>({py::str(".h")});    // p.get("headers", [".h"])
    py::dict<py::str, py::list<library::LibRef>> libraries;            // p.get("libraries", {})
    py::dict<py::str, py::list<py::str>> objects;                      // p.get("objects", {})
    std::optional<py::str> inverse;                  // p.get("inverse")
    py::str data_order = py::str("after_next_function");               // p.get("data", {}).get("order", ...)
};
// the parts of the profile the toolchain was made from (read from toolchains/NAME.json, once)
const ProfileParts& profile_parts(const toolchain::Toolchain& tc);

// the 16-bit decoder the adapters give where an interface asks for one (platforms.decoder(16).disasm)
void set_decoder16(const compare::Decoder& d);
const compare::Decoder& decoder16();

// exact.draft.prove.prove_texts, which has no typed port: given what as_draft() hands out for it
using ProveTexts = std::function<std::tuple<py::str, py::str>(draft::InverseModel&, const py::list<std::tuple<py::str, py::dict<py::str, int64_t>>>&,
                                                              int64_t a, int64_t ln, const py::dict<int64_t, py::str>& names)>;
void set_prove_texts(const ProveTexts& f);

// ---- the project
class Project : public std::enable_shared_from_this<Project> {
  public:
    py::str dir;                                     // os.path.abspath(folder)
    Config cfg;
    py::str exe;                                     // the original program's file
    py::str src;                                     // the sources' folder
    py::list<py::str> include;
    int64_t dgroup = 0;
    py::list<int64_t> code_segments;
    int64_t loadseg = 0;
    int64_t code_end = 0;
    std::optional<py::str> switches;
    py::bytes file;                                  // the program's file
    py::list<int64_t> header;                        // the 14 words of the MZ header (empty: not an MZ file)
    py::bytes image;                                 // the load image
    py::ref<exepack::Packed> packed;                 // (the original as EXEPACK left it, when the image is the unpacked program)
    py::bytes packed_file;

    // Project(folder, override=None): override is a JSON object as text
    explicit Project(const py::str& folder, const std::optional<py::str>& override_json = std::nullopt);

    // the places in the load image where the loader adds the load segment
    const py::set<int64_t>& relocs() const;
    // [Project] - this one, and the other sets of sources project.json lists
    py::list<std::shared_ptr<Project>> sets();
    // the image offset of the code segment `a` is in
    int64_t code_base(int64_t a) const;
    // where the program's own code ends (the last of its functions, exact/carve.py)
    int64_t own_end();
    // [image offsets where functions begin], or none
    std::optional<py::list<int64_t>> starts() const;
    std::shared_ptr<toolchain::Toolchain> toolchain() const;
    // [(label, Toolchain)]
    py::list<std::tuple<py::str, std::shared_ptr<toolchain::Toolchain>>> toolchains() const;
    py::str path(const py::str& name) const;
    Symbols symbols() const;
    void save_symbols(const Symbols& s) const;
    // [(address, path)] of the one-function files, and of the whole files src/ORDER lists
    py::list<std::tuple<int64_t, py::str>> sources() const;
    // does the file's code lie in one piece?
    bool whole(const py::str& path) const;
    // {name: bytes} the headers of the sources' folder
    py::dict<py::str, py::bytes> headers() const;
    // a key of project.json as the JSON text it holds, or none
    std::optional<py::str> cfg_json(const py::str& key) const;
    // the project.json of the folder read again, cfg[key][sub] = value (setdefault(key, {})), written back
    // (json.dump(cfg, f, indent=1)) and kept as the project's cfg: what link.fit does to "data_sizes" and "data_pads"
    void set_project_entry(const py::str& key, const py::str& sub, int64_t value);

    // prj._library: the library stage's answer is kept in the library adapter of the project (as_library); forget it
    // (`prj._library = None`)
    void forget_library();

    // prj._own_end
    std::optional<int64_t> own_end_;

    // (the adapter that keeps prj._library)
    std::shared_ptr<library::Project> library_view_;

  private:
    mutable std::optional<py::set<int64_t>> relocs_;
};

// project.load(folder)
std::shared_ptr<Project> load(const py::str& folder);

// ---- the adapters (see the top of this file)
std::shared_ptr<modules::Project> as_modules(Project& p);
std::shared_ptr<library::Project> as_library(Project& p);        // the project's one (prj._library lives in it)
std::shared_ptr<data::Project> as_data(Project& p);
std::shared_ptr<ledger::Project> as_ledger(Project& p);
std::shared_ptr<identify::Project> as_identify(Project& p);
std::shared_ptr<standin::Project> as_standin(Project& p);
std::shared_ptr<recut::Project> as_recut(Project& p);
std::shared_ptr<linkwhole::Project> as_linkwhole(Project& p);
pascal_join::Project pascal_project(Project& p);
compare::Project compare_project(Project& p);
batch::Project batch_project(Project& p);
std::shared_ptr<readable::Project> as_readable(Project& p);
std::shared_ptr<draft::Project> as_draft(Project& p);

}  // namespace codebreaker::exact::project
