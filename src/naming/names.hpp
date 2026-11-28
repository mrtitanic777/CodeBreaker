// naming/names.hpp -- the best name for every function and import of a module, and where it came from.
//
//     codebreaker names comctl32.dll                         every function and import, named
//     codebreaker names comctl32.dll --set 7fe0183a StrChrHelper
//     codebreaker names --shared                             how many bodies the indexed modules share
//     codebreaker names --ordinals FILE...                   remake the table of names for numbered exports
//     codebreaker names lz32.dll --propose | --packets | --proposals | --accept ADDR | --reject ADDR | --suggest ADDR NAME
//                                                            proposed names, kept apart from names until accepted (proposals.hpp, docs/NAMING.md)
//
//     Namer namer(services);
//     ModuleNames n = namer.of_module(path);                 {functions: [address, key, name, source], imports: [module, ordinal, name, source]}
//     namer.bodies(path)                                     {address: digest of the body}
//     namer.index(path)                                      record the module's bodies, and the names it gives them, in the body store
//     namer.set_name(path, address, "Name")                  name a body: it holds in every module with that body
//
// A function's name is taken from the first of these that has one:
//
//     export          the module's own export table
//     run-time types  (PowerPC) a virtual function of a class its run-time type information names, as Class_vfN; the image's entry point as `entry`
//     debug symbols   CodeView records in the file, or the module's .SYM file (Microsoft's symbol files for the system modules, when the corpus's
//                     reference folder holds them) -- used only where the file's exports say the symbols are for this build
//     ordinal table   the name its maker's definition file gives an export the module leaves unnamed
//     named body      a name given to this body by hand
//     runtime library the library function its bytes are (symbols/runtime_library)
//     same body       the name another module gives the same body
//     (none)          sub_<address>: a placeholder, not a name
//
// The same function is linked into many modules -- the C runtime, helper libraries, code shared between the 16-bit and 32-bit halves of a
// component. Its body is recognised by a digest of its bytes with everything the linker fills in left out (addresses inside the module, calls and
// jumps out of the function), so the digest is the same wherever the code sits. The body store (store/bodies.hpp) keeps one row per body per
// module; a name given to a body is kept once and read back for every module that has it.
//
// What this module reads of the rest of the program comes in through `Services`: the corpus (which kind of file a path is, its hash, the
// reference folder), the table of numbered exports, the body store, and what the readers of each kind of file know (PowerPC images, the
// console's import names, which functions are a runtime library's, the compiler, remaking the ordinal table). Where one is not given the
// module goes without: no PowerPC names, no console imports, no runtime-library names, no "built with" line.
//
// The program fills them in workbench.hpp: the knowledge base's body store (store/knowledge_bodies.hpp), the PowerPC reader (formats/powerpc_image), the
// console's ordinals (the knowledge base's), identify.hpp for the runtime library and the compiler, ordinal_collect.hpp for the table. The
// command the program registers is `command(Environment)` below: what it works over is made when it runs.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "base/corpus.hpp"
#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "core16_types.hpp"
#include "formats/ne.hpp"
#include "formats/pe_functions.hpp"
#include "platforms/decoder.hpp"
#include "store/bodies.hpp"
#include "store/proposals.hpp"
#include "symbols/environment.hpp"
#include "symbols/ordinals.hpp"
#include "symbols/name_data.hpp"
#include "symbols/runtime_library.hpp"

namespace cb::naming::names {

using symbols::Environment;

// The names as data (Address, NameSource, Named, the store's keys, the body digests, debug symbols, PowerPC facts) are symbols/name_data.hpp.

class Namer {
public:
    struct Services {
        const base::Corpus& corpus;
        const symbols::ordinals::Ordinals& ordinals;
        store::BodyStore* bodies = nullptr;   // not owned; none: no body is recorded and no name is shared
        // Each of these is optional; see the top of this file for what a module goes without.
        std::function<std::optional<symbols::names::PowerPcNames>(const std::filesystem::path&)> powerpc = {};
        std::function<std::map<std::string, std::string>(const std::filesystem::path&)> console_import_names = {};   // {import symbol: name}
        std::function<std::map<symbols::names::Address, symbols::runtime_library::Identified>(const std::filesystem::path&)> runtime_library = {};
        std::function<symbols::runtime_library::CompilerInfo(const std::filesystem::path&)> compiler = {};
        // `names --ordinals FILE...`: make the table of names for numbered exports from definition files and import libraries (ordinals::collect and
        // where it is saved are the caller's: they need the whole corpus).
        std::function<void(std::span<const std::filesystem::path>)> collect_ordinals = {};
        // Proposed names (store/proposals.hpp): an accepted one names a function too small to have a body, and an undecided one is shown beside
        // the function it is for. None given: no function has either.
        store::ProposalStore* proposals = nullptr;   // not owned
        // `names MODULE --propose|--packets|--suggest|--proposals|--accept|--reject` (symbols/proposals.hpp); none given: those options say so.
        std::function<int(const cli::Args&)> proposals_command = {};
    };

    explicit Namer(Services services);
    Namer(const Namer&) = delete;
    Namer& operator=(const Namer&) = delete;
    ~Namer();

    // {address: digest} for the functions of a module that have a body worth telling apart. NotFoundError / FormatError for a file that cannot be read.
    std::map<symbols::names::Address, std::string> bodies(const std::filesystem::path& module);

    // {address: (name, where it is from)} from debug symbols that are shown to be for this very build: a symbol file is from some build of the
    // module, and offsets move between builds, so a segment's names are used only where the file's own exports sit at the offsets the symbols give
    // them (at least two, and two in three of those the symbol file names). Remembered per module for the life of the Namer.
    const symbols::names::Named& debug_names(const std::filesystem::path& module);
    // {address: name of the object file the function was linked from}, for the functions the module's .SYM file places in a segment named after an
    // object -- and only where that file is shown to be for this build (see debug_names).
    const std::map<symbols::names::Address, std::string>& source_files(const std::filesystem::path& module);

    // {address: (name, source)} from what the module and its symbol files say themselves: exports, debug symbols, the ordinal table.
    symbols::names::Named own_names(const std::filesystem::path& module);
    // For a PowerPC image: what names a function for certain is its run-time type information -- a virtual function of a named class -- and the entry
    // point. Empty without a PowerPC reader (Services::powerpc).
    symbols::names::Named powerpc_names(const std::filesystem::path& module);

    // Record a module's bodies in the body store, each with the name the module itself gives it. -> the number of bodies. UsageError without a body store.
    std::size_t index(const std::filesystem::path& module, const std::optional<symbols::names::Named>& own = std::nullopt);
    // Name the body of the function at `address`; every module holding that body then has the name. -> the body's digest, or nullopt when the function
    // has no body worth naming. UsageError without a body store.
    // `source` is where the name is from, as `names` shows it ("named body (<source>)"): by hand, or an accepted proposal.
    std::optional<std::string> set_name(const std::filesystem::path& module, symbols::names::Address address, std::string_view name, std::string_view source = "named by hand");
    // The module's content hash as the body store and the proposals know it ("" for a file the corpus does not know), and the store's key for a
    // function of it.
    std::string module_sha(const std::filesystem::path& module) const;
    std::string key_of(const std::filesystem::path& module, symbols::names::Address address) const;

    // Every function and import of a module with its best name and where the name is from.
    symbols::names::ModuleNames of_module(const std::filesystem::path& module);

    // `codebreaker names ...`: the command line after the command name. Returns the exit code.
    int run(std::span<const std::string> args);
    // What the command does with the arguments it was given (the parser's work is done): the exit code.
    int execute(const cli::Args& args);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// The declaration of the `names` command (its arguments and help); `run` is what it does with them.
cli::Command declare_command(std::function<int(const cli::Args&)> run);

// The command as the program registers it: the objects it works over (the knowledge base, the corpus's modules) are made when it runs.
// Defined in names_command.cpp.
cli::Command command(const symbols::Environment& environment);

}  // namespace cb::naming::names
