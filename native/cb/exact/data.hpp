// cb/exact/data.hpp -- a module's initialized data written as C: where it stands, and definitions proposed from the
// bytes.
//
//     codebreaker exact PROJECT data MODULE [--draft] [--write]
//
// The module's functions are compiled alone (no data files): their literals, each with the function that names it
// first, are lined up with the program's data segment from the module's data start (src/MODULES). What lies between
// them is the module's own: its variables, and the strings their values point at. Which declarations those are, and
// where they stood in the source, the toolchain profile's "data" rule says:
//
//     "order": "after_next_function"   (Microsoft C 3.00) a file's _DATA follows the source: an initializer's strings
//         where the declaration stands, a function's literals with the function, and a variable's bytes after the
//         literals of the next function after its declaration - so a run after F's literals is variables declared
//         before the function after F (src/dNEXT.c), and the strings before such a run are their values; in a file
//         without functions, all literals come first, then the variables
//
// The definitions are typed by the functions' extern declarations (int where none says), their values by the
// program's bytes: a word that points at one of the module's strings is that string, at a known place its name.
// What the bytes cannot tell is marked /* ? */.
//
// Source text here is bytes, each one character (Latin-1), as in exact/modules.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/exact/modules.hpp"

namespace cb::exact::data {

// The data cannot be told: the module's functions do not compile alone, the program has no data for it, the
// toolchain has no rule for where its data stood.
class DataError : public Error {
public:
    explicit DataError(const std::string& what) : Error(what) {}
    CB_ERROR_BOILERPLATE(DataError)
};

// What the command needs of a project beyond exact/modules (exact/project implements it).
class Project : public modules::Project {
public:
    // The toolchain profile's "data" rule ("after_next_function" when the profile gives none).
    virtual std::string data_order() const = 0;
};

// A byte string as a C string literal: \n \t \r and the quote and backslash escaped, printable ASCII as it is, every
// other byte as three octal digits.
std::string c_string(ByteView bytes);

// ---- where the module's data stands ----------------------------------------------------------------------------

// A string of the data the module's functions name: its place in the functions' own literals, its bytes with the
// NUL, and the function that names it (a literal nothing names takes the function before it).
struct Literal {
    std::uint32_t offset = 0;
    Bytes bytes;
    std::optional<std::string> function;
};

inline constexpr std::string_view kStart = "(start)";   // "after": before the module's first function

// A stretch of the data the literals leave over, with the function whose literals come before it.
enum class RunKind { Strings, Vars };
struct Run {
    std::uint32_t lo = 0, hi = 0;
    std::string after;   // a function's name, or kStart
    RunKind kind = RunKind::Vars;
};

struct FunctionPlace {
    std::string name;
    std::uint32_t address = 0;   // of its file's function in the program
};

struct Walk {
    std::vector<Run> runs;
    std::vector<Literal> literals;
    std::uint32_t data_start = 0, data_end = 0;   // the module's data in the data segment: up to the next module's
    std::vector<FunctionPlace> functions;         // as the functions compiled alone name them, with their addresses
};

// DataError when the row has no data start, the program has no data segment there, or the functions do not compile.
Walk walk(const modules::Project& project, const modules::Row& row);
Walk walk(const modules::Project& project, const modules::Row& row, const modules::FunctionTypes& types);

// ---- C declarations --------------------------------------------------------------------------------------------

struct Declaration {
    std::string type;         // "int", "unsigned long", "struct node"
    std::string declarator;   // "tab[4]", "*p", "(*fn[2])()"
    friend auto operator<=>(const Declaration&, const Declaration&) = default;
};
// How the functions of the files declare the data they name (extern lines), by name.
using Declarations = std::map<std::string, std::set<Declaration>>;
// The files' extern declarations; without `files`, those of every src/f*.c.
Declarations externs(const std::filesystem::path& src, const std::optional<std::vector<std::filesystem::path>>& files = std::nullopt);

struct Field {
    std::string base;                  // "char", "unsigned int", "struct node"
    unsigned stars = 0;
    std::vector<std::uint32_t> dims;   // the dimensions that are numbers
    std::string name;
};
using Structs = std::map<std::string, std::vector<Field>>;   // struct name -> its fields in order (lookup only)
struct StructTable {
    Structs structs;
    std::map<std::string, std::string> home;   // struct name -> the first header that defines it
};
// The structs the project's headers (its own, then the *.h of its include folders by name) define.
StructTable parse_structs(const modules::Project& project);

// The bytes a declaration takes when it says (a scalar, a pointer, an array of a known count, a struct). Of a name
// declared more than one way, the first declaration by (type, declarator) that says.
std::optional<std::uint32_t> declared_size(const std::set<Declaration>& declarations, const Structs& structs);

// ---- proposing the definitions ---------------------------------------------------------------------------------

struct DataFile {
    std::string name;   // dADDR.c, eADDR.c or MODULE-data.c
    std::string text;
};
// The module's data files proposed, in the order the runs lie. DataError when the toolchain's rule is not known.
std::vector<DataFile> draft(const Project& project, const modules::Row& row);
std::vector<DataFile> draft(const Project& project, const modules::Row& row, const modules::FunctionTypes& types);

// `exact PROJECT data [MODULE..] [--draft] [--write]`: each module's data runs, or with --draft/--write its proposed
// files (--write keeps a file already there). Returns the command's exit code.
int run(const Project& project, const std::vector<std::string>& args);

}  // namespace cb::exact::data
