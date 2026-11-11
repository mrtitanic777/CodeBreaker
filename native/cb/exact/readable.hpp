// cb/exact/readable.hpp -- the sources with the names SYMBOLS has put in for the engine's address-names, proven to
// change nothing.
//
//     codebreaker exact PROJECT readable [--write]
//
// The sources the engine writes name a function fXXXXX and a variable g_XXXX after where they are. Where src/SYMBOLS
// has a name for that place (the names stage carries them from another version; a person may write one), each source
// file gets it in place of the address-name: a name of C code `_name` is `name` in C. The files are compiled again,
// every file into the ledger, and the change is kept only if no function that was identical stops being so (the names
// of functions and variables are the linker's; a local's name, which can move it in the frame, is left). Without
// --write the renaming is only tried and reported.
//
// The source files are handled as bytes (they are Latin-1 on disk and written back as they were): only ASCII tokens
// are looked at and changed.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/exact/symbols.hpp"

namespace cb::exact::readable {

// The place an address-name stands for: a function at an image address (fXXXXX) or a variable at a data offset
// (g_XXXX).
struct Place {
    enum class Kind { Function, Variable };
    Kind kind = Kind::Function;
    std::uint32_t address = 0;
    friend auto operator<=>(const Place&, const Place&) = default;
};

// What the address-name `word` (the whole of it: f0123a, _f0123a or g_12ab) stands for.
std::optional<Place> parse_address_name(std::string_view word);

using NameMap = std::map<Place, std::string>;

// {place: name} from SYMBOLS, for the places it has a real name for: a name of C code `_name` is `name`; names that
// are address-names themselves, or that carry ':' or '@', are not names of places. The first name given a place wins.
NameMap mapping(const SymbolTable& symbols);

// The text with each address-name that has a name renamed - unless the name is in the text already, as something
// else - outside comments, strings and character constants (another program's place, or text).
std::string rename(std::string_view text, const NameMap& names);

// ---- the command ---------------------------------------------------------------------------------------------

struct SourceFile {
    std::uint32_t address = 0;          // the function's image address
    std::filesystem::path path;
};

struct LedgerRow {
    std::string file;
    std::string function;
    std::string verdict;                // "identical" or what is wrong with it
};

inline constexpr std::string_view kIdentical = "identical";

// What the command needs of a project (exact/project and exact/ledger, refactored later, implement it).
class Project {
public:
    virtual ~Project() = default;
    virtual const SymbolTable& symbols() const = 0;
    virtual std::vector<SourceFile> sources() const = 0;
    virtual const std::filesystem::path& source_dir() const = 0;
    virtual std::vector<LedgerRow> ledger() const = 0;     // every file compiled again, every function held to the program
    // The same project with its sources taken from `source_dir` instead.
    virtual std::unique_ptr<Project> with_source_dir(const std::filesystem::path& source_dir) const = 0;
};

struct Options {
    bool write = false;
};

// The command: 0 when the renaming is kept (or there is nothing to do), 1 when it is not kept or SYMBOLS has no names.
// The report goes to stdout.
int run(const Project& project, const Options& options);

}  // namespace cb::exact::readable
