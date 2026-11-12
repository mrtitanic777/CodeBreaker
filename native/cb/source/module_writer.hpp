// cb/source/module_writer.hpp -- one source file per module, every function labelled with what is known.
//
//     module_writer::Prepared p = ...;                       the module as its preparation left it (below)
//     Written w = module_writer::write_module(p, store, out_dir, /*split=*/0, log);
//     w.file, w.counts, w.total
//
// Each function carries one status line:
//
//     VERIFIED            behaves as the original on every input tried, with most of it exercised
//     VERIFIED (weak)     matched, but the match is thin -- the reason is stated
//     UNVERIFIED          rebuilt and run; it behaves differently (what differed is stated)
//     NOT JUDGED          the original could not be observed in the test environment
//     DOES NOT COMPILE    the reconstructed text is not valid C yet
//     NO SOURCE           nothing usable was produced
//     IDENTIFIED (runtime library)
//                         its bytes are a function of a library that came with the compiler (the C runtime, a compiler helper): named,
//                         with the library. Not this module's own code, so not counted as a reconstruction that failed -- unless it was
//                         verified too, and then it is counted as VERIFIED
//
// Imports by number are written with their real names where the exporting module gives one (`GetDlgItem`, not `USER_91`); a C++ export is
// shown with the declaration its decorated name stands for.
//
// A driver or a real-mode program is written as its assembly file (write_assembly) with the same status counts at its head; a Java program
// as the C++ folder it transpiles to (source/java_writer). Where a module's files go in the source tree is source/placement's business.
//
// The files are written by a back end (translate/backends/module_file) from the status of each function and the store's facts for the head.
// Everything written here is read from the function store, so the file and the scoreboard cannot disagree with the results.
//
// What the writer is handed, and what it asks of others. The module's preparation -- finding it, reconstructing and judging it, naming what
// the binary names, the readable pass over its text -- is the caller's; the writer reads the facts it is given (`Prepared`), the function
// store, and the readable pass through `ReadablePass`, and writes the files. Nothing here is global.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core16_types.hpp"
#include "cb/engine/function_store.hpp"
#include "cb/source/authorship.hpp"
#include "cb/translate/backends/module_file.hpp"
#include "cb/translate/l2.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::source::module_writer {

using Log = std::function<void(std::string_view)>;

// ---- what a status says ---------------------------------------------------------------------------------------

// The words that say a verdict's reason ("returns a different value"), else the reason as it is.
std::string_view why(std::string_view reason);

// The words after the status for one function's best candidate: how much of a verified function was exercised, why a match is weak, what
// differed.
std::string describe(const engine::Best& best);

// ---- names --------------------------------------------------------------------------------------------------------

// {symbol: name} for a module's imports by number that can be written by their real name: the name is a C name found only once, and no
// function of the module has it (`used`) -- and it is not itself the name of an import by number. (A 32-bit module's are already named in the
// decompiled text.)
std::map<std::string, std::string> unique_import_names(const std::map<std::string, std::string>& found, const std::set<std::string>& used);

// An import of a PowerPC image: `import<library>_ord_<number>`, the library as the image's own table has it (0 the first).
struct ConsoleImport {
    std::size_t library = 0;
    std::string number;
};
// The library and number of a symbol of that spelling.
std::optional<ConsoleImport> parse_console_import(std::string_view symbol);

// {symbol: name} for a PowerPC image's imports. An image names what it imports by library and number only (`import0_ord_16`); the numbers of
// the console's system libraries are public knowledge. `libraries[i]` maps a number to its name for the image's i'th library; `symbols` are
// the image's imports. A name is used where it is a C name found only once and no function has it.
std::map<std::string, std::string> console_import_names(std::span<const std::string> symbols,
                                                         std::span<const std::map<std::string, std::string>> libraries,
                                                         const std::set<std::string>& used);

// `text` with each identifier that is a key of `names` written as its value. Whole identifiers only.
std::string spell(std::string_view text, const std::map<std::string, std::string>& names);

// The names a module's functions are written with: imports by number by their real names, and an internal function the decompiler left as
// `sub_...` by the name recovered for it elsewhere (a name it shares with a named function in another module, a runtime library's, or a
// debug symbol file's) -- where that name is free.
struct Names {
    std::map<std::string, std::string> rename;   // a placeholder name -> the recovered one
    std::map<std::string, std::string> all;      // rename and the imports: what is replaced in the text
};
// `candidate_name(address)` is the name the function has now, "" for none.
Names plan_names(std::span<const std::string> addresses, const std::function<std::string(const std::string&)>& candidate_name,
                 const std::map<std::string, std::string>& imports, const std::map<std::string, std::string>& exported,
                 const std::map<std::string, std::string>& recovered);

// ---- the readable pass ----------------------------------------------------------------------------------------------

// Names in place of numbers and placeholders (SDK constants, handles and parameters named for what Windows takes them as, function pointers
// for what they point at), over the module as the store holds it.
class ReadablePass {
public:
    virtual ~ReadablePass() = default;
    // The C of the function at `address`, as the pass reads it.
    virtual std::string function(const std::string& address) = 0;
    // Does no caller in the module use what the function called `name` returns?
    virtual bool results_unused(const std::string& name) const = 0;
    // A hint at what a function called by placeholder does, from its text ("" for none).
    virtual std::string calls_hint(std::string_view text) const = 0;
    // The header with the globals as the functions now call them.
    virtual std::string rename_globals(const std::string& header) const = 0;
    virtual bool constants_used() const = 0;
    // The SDK constants written into the module, with the values the headers give them.
    virtual std::string definitions() const = 0;
    virtual std::string struct_definitions() const = 0;
};

// Make the pass for a module: `stored` is the module as the store holds it, each function's C (after the names are spelled) by address and its
// header.
using ReadableFactory = std::function<std::unique_ptr<ReadablePass>(const translate::l2::Made& stored)>;

// ---- a module as its preparation left it ------------------------------------------------------------------------------

struct Prepared {
    std::string name;                     // the file's name, lower case: "comctl32.dll"
    std::string stem;
    std::string module;                   // the store's key for it (a content hash)
    std::string profile;                  // the verifier settings the verdicts are under (a digest)
    std::string date;                     // when the files are written, "2026-11-12"
    std::string source_engine;            // the engine whose text the store holds as the module's header
    translate::backends::module_file::Lane lane = translate::backends::module_file::Lane::Pe;
    bool flat = false;                    // a 32-bit module (its functions can be only a jump through an import)

    std::map<std::string, std::string> exported;        // address -> decorated name, for a C++ export
    std::map<std::string, std::string> found_imports;   // symbol -> real name, before the choice of those that can be written
    std::map<std::string, std::string> recovered;       // address -> a name found elsewhere for an internal function
    std::function<std::string(const std::string&)> globals_note;   // a note on the module's globals, given its header ("" for none)
    std::function<std::string(core16::Va)> address_key;            // the store's key of an address: how the store spells it
    std::vector<core16::ClassRecord> classes;                       // the C++ classes found in it
    std::function<std::optional<std::string>(core16::Va)> import_behind;   // the import a function only jumps to
    std::optional<translate::backends::module_file::Whole> whole;   // how the whole module built, when it was tried
    ReadableFactory readable;
};

struct Written {
    std::filesystem::path file;           // the first file written
    engine::ClassCount counts;            // by status, every class of verify::kClassScored a key
    std::size_t total = 0;                // the functions the module is known to have
};

// The module's C (<stem>.c, or parts when `split` functions at most go to a file), its header <stem>.h and its C++ classes
// (<stem>_classes.hpp), into `out_dir`.
Written write_module(const Prepared& module, engine::FunctionStore& store, const std::filesystem::path& out_dir, std::size_t split = 0, const Log& log = nullptr);

// A driver's or a real-mode program's source: its one assembly file <stem>.asm (CRLF, Latin-1), the status at its head.
Written write_assembly(const Prepared& module, engine::FunctionStore& store, const std::filesystem::path& out_dir, const Log& log = nullptr);

// ---- the scoreboard --------------------------------------------------------------------------------------------------

struct ScoreRow {
    std::string name;
    std::string kind;
    engine::ClassCount counts;   // every class of verify::kClassScored a key
};
struct ScoreboardInput {
    std::string generated;                 // "2026-11-12 09:30"
    std::string settings;                  // the verifier settings of each kind, as the heading says them
    std::vector<ScoreRow> modules;         // judged under the current settings, by name
    std::size_t corpus_32 = 0;             // the corpus's 32-bit modules
    std::size_t corpus_16 = 0;             // and its 16-bit ones
};
// SCOREBOARD.md: only modules measured on the current verifier are listed. Nothing is counted as progress except what the labels say.
std::string scoreboard(const ScoreboardInput& in);

}  // namespace cb::source::module_writer
