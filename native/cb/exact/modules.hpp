// cb/exact/modules.hpp -- the program's source files: their functions and data put together, compiled whole, checked.
//
//     codebreaker exact PROJECT modules [MODULE..] [-v]
//
// The functions are written one to a file (src/fADDR.c) so that each can be compiled and compared alone. The
// original had them in modules; src/MODULES gives, one a line:
//
//     name   code-start | -   data-start   [switches]
//         name.c   a source file: its code from code-start to the next module's, its data from data-start
//         NAME.OBJ an object of the kit, linked as it is
//         -        a file of data only (src/NAME-data.c)
//
// A module's data is in src/dADDR.c (what the file defines before the function at ADDR) and src/eADDR.c (after it,
// at the file's end); its functions are the src/fADDR.c of its code. They are put together in order - the includes
// once, each extern once, the functions of other files that return something else than an int declared - compiled
// once, and every function is compared with the program, and the file's initialized data (_DATA) with the data
// segment, word for word where the linker fills a word.
//
// Source text here is bytes, each one character (Latin-1): the files are read, joined and handed to the compiler
// without being decoded.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/exact/compare.hpp"
#include "cb/exact/symbols.hpp"
#include "cb/exact/x86.hpp"

namespace cb::exact::modules {

// One line of src/MODULES.
struct Row {
    std::string name;
    std::optional<std::uint32_t> code_start;   // none: a file of data only ("-")
    std::optional<std::uint32_t> code_end;     // where the next module's code (or the library's, or the program's) begins
    std::optional<std::uint32_t> data_start;
    std::optional<std::string> switches;
    friend bool operator==(const Row&, const Row&) = default;
};

// What the compiler is given besides the source.
struct CompileOptions {
    std::map<std::string, Bytes> headers;      // the project's headers by name (lookup: the toolchain writes them out)
    std::vector<std::string> include;          // include folders
    std::optional<std::string> switches;
};

struct CompileResult {
    std::optional<Bytes> object;               // the object module; none when the compiler rejected the source
    std::string console;                       // what the compiler said
};

// exact/toolchain, as modules reads it (that module is refactored later and implements this).
class Toolchain {
public:
    virtual ~Toolchain() = default;
    // An entry of the toolchain's profile ("switches", "file_switches", "segment_switch"), or none.
    virtual std::optional<std::string> profile(std::string_view key) const = 0;
    virtual CompileResult compile(ByteView source, const CompileOptions& options) const = 0;
};

// exact/project, as modules reads it (that module is refactored later and implements this).
class Project {
public:
    virtual ~Project() = default;
    virtual std::filesystem::path src() const = 0;                       // the folder of the source files
    virtual std::filesystem::path path(std::string_view name) const = 0;   // a file of the project's (src/MODULES, src/CRTLIB)
    virtual std::uint32_t code_end() const = 0;                          // where the code ends (0: not given)
    virtual std::uint32_t own_end() const = 0;                           // where the program's own code ends
    virtual std::optional<std::string> switches() const = 0;
    virtual std::optional<std::string> config_text(std::string_view key) const = 0;   // a string entry of project.json
    virtual std::vector<std::uint32_t> code_segments() const = 0;        // image offsets where the code segments begin
    virtual std::shared_ptr<const Toolchain> toolchain() const = 0;
    virtual std::map<std::string, Bytes> headers() const = 0;
    virtual std::vector<std::string> include() const = 0;
    // The sets of sources after the project's own (an "also" set has its own pattern of files, headers, toolchain).
    virtual std::vector<std::shared_ptr<const Project>> also_sets() const = 0;
    // what check reads of the program
    virtual ByteView image() const = 0;
    virtual SymbolTable symbols() const = 0;
    virtual compare::Program program() const = 0;
};

// ---- src/MODULES and src/CRTLIB ------------------------------------------------------------------------------------

// The rows of src/MODULES with their ends worked out; a row of a part kept in a code segment of its own ("@SEG")
// is not one. Empty when the project has no src/MODULES. Throws ParseError for a line that is not one.
std::vector<Row> rows(const Project& project);

// Where the library's part begins: the first code place src/CRTLIB names ("code ADDR", "code=ADDR", or a line's note
// that begins with its place).
std::optional<std::uint32_t> lib_start(const Project& project);

// The functions of the file that #pragma alloc_text puts in another code segment (a row of src/MODULES:
// name start @SEGMENT).
struct Part {
    std::uint32_t start = 0, end = 0;
    std::string segment;
    friend bool operator==(const Part&, const Part&) = default;
};
std::vector<Part> parts(const Project& project, std::string_view module);

// The code starts src/MODULES gives, ascending.
std::vector<std::uint32_t> starts(const Project& project);

// The src/dADDR.c, fADDR.c and eADDR.c files whose address is in [start, end), in address order (d, f, e at one).
std::vector<std::filesystem::path> files_of(const std::filesystem::path& src, std::uint32_t start, std::uint32_t end);

// ---- putting a module together -------------------------------------------------------------------------------------

// The program's functions (the fADDR of src) that return something else than an int, with the type they return
// ("char *", "struct node *", "unsigned long" ...): a module calling one of another file declares it.
class FunctionTypes {
public:
    static FunctionTypes read(const std::filesystem::path& src);
    const OrderedMap<std::string, std::string>& all() const { return types_; }

private:
    OrderedMap<std::string, std::string> types_;   // in the order the files give them (the declarations are written so)
};

struct Combined {
    std::string text;                    // the files' texts as one source
    std::set<std::string> switches;      // what the functions' files ask for (their header comments, else the default)
};

// `before` puts a text (a pragma) before the body of a path. `header_tag` is the profile's "file_switches": the word
// in a file's first-line comment `/* TAG: switches */` that says how it is compiled (none: files carry none).
Combined combine(const std::vector<std::filesystem::path>& paths, std::string_view default_switches,
                 std::optional<std::string_view> header_tag, const std::map<std::filesystem::path, std::string>& before,
                 const FunctionTypes& types);

struct Source {
    std::string text;                       // the module's whole text
    std::vector<std::uint32_t> addresses;   // of its functions, in order (those of another code segment are not here)
    std::set<std::string> switches;
};

// The module's source; none for an object of the kit.
std::optional<Source> source(const Project& project, const Row& row, const FunctionTypes& types);

// The source set whose pattern of files names this file (an "also" set's, else the project's: `own` null), with its
// toolchain.
struct SetOf {
    std::shared_ptr<const Project> own;
    std::shared_ptr<const Toolchain> toolchain;
};
SetOf set_of(const Project& project, std::string_view module);

// The name the project gives the first code segment ("first_segment"), for a module that lies in it.
std::optional<std::string> first_segment(const Project& project, std::optional<std::uint32_t> start);

struct Compiled {
    std::optional<Bytes> object;
    std::string console;
    std::vector<std::uint32_t> addresses;   // of the module's functions
};
// Compile the module (an object of the kit gives an empty result). `toolchain` overrides the project's.
Compiled compile_module(const Project& project, const Row& row, const FunctionTypes& types,
                        const Toolchain* toolchain = nullptr);

// ---- checking ------------------------------------------------------------------------------------------------------

enum class Verdict { Same, Differs, NoPlace };

struct FunctionResult {
    std::string name;
    std::optional<std::uint32_t> address;   // none: the program has no place for it (a name that is not fADDR or in SYMBOLS)
    std::uint32_t length = 0;
    Verdict verdict = Verdict::NoPlace;
    std::size_t differing_bytes = 0;        // for Differs
};

struct CheckResult {
    bool kit_object = false;                // an object of the kit: nothing to check
    std::optional<std::string> error;       // the compiler's console when the module did not compile
    std::vector<FunctionResult> functions;
    std::uint32_t data_length = 0;
    // the initialized data's differences from the program's, one line each ("0042: 12, the program's 13")
    std::vector<std::string> data_differences;
    std::optional<Bytes> object;
};

// Compile (unless `object` is given) and compare the module's functions and data with the program.
CheckResult check(const Project& project, const Row& row, const FunctionTypes& types,
                  const std::optional<Bytes>& object = std::nullopt,
                  const x86::Decoder16& decoder = x86::capstone_decoder());

// `exact PROJECT modules [MODULE..] [-v]`: a line for each module, and with -v the functions and data that differ.
// Returns the command's exit code.
int run(const Project& project, const std::vector<std::string>& args, const x86::Decoder16& decoder = x86::capstone_decoder());

}  // namespace cb::exact::modules
