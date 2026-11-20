// exact/ledger.hpp -- every one-function source of a project compiled by its toolchain and put against the program.
//
//     codebreaker exact PROJECT ledger [--learn] [-v] [--write]
//
// Each src/fADDR.c is compiled on its own; its first function is held to the program at ADDR, the others at their
// places in SYMBOLS. A project with several toolchains has each file compiled by each; a function is identical when
// one of them gives its bytes, and the ledger says which did.
//
//     --learn   the places identical functions show are added to SYMBOLS (a name is only learnt from a file all of
//               whose placed functions are identical under one toolchain)
//     --write   src/LEDGER is written (otherwise only the counts are printed)
//     -v        a line for each function
//
// What the module asks of its surroundings -- the project, its toolchains, the decoder -- it asks through the
// interface below; exact/project fills it.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "exact/compare.hpp"
#include "exact/modules.hpp"
#include "exact/pascal_join.hpp"
#include "exact/symbols.hpp"
#include "exact/x86.hpp"

namespace cb::exact::ledger {

using Toolchain = modules::Toolchain;
using Verdict = pascal_join::Verdict;   // identical | differs | no place | does not compile (| equivalent, for a Pascal program)

// A toolchain of a project, with the label the ledger says it by.
struct LabelledToolchain {
    std::string label;
    std::shared_ptr<const Toolchain> toolchain;
};

// One source file of a project: its place in the program (none: the file is placed by the names SYMBOLS knows; a
// negative place says the same and that the file is not named by an address).
struct SourceFile {
    std::optional<std::int64_t> at;
    std::filesystem::path path;
};

// What the ledger reads of an exact project.
class Project {
public:
    virtual ~Project() = default;
    virtual std::filesystem::path dir() const = 0;                        // the project's folder
    virtual std::filesystem::path src() const = 0;                        // this set's source folder
    virtual std::string exe() const = 0;                                  // the program's file name, as the project gives it
    virtual std::filesystem::path path(std::string_view name) const = 0;   // a file of the project's (src/LEDGER)
    virtual std::optional<std::string> config_text(std::string_view key) const = 0;   // a string entry of project.json: "named_by", "toolchain"
    // A Pascal program kept in pieces ("compilands" in project.json): its view for exact/pascal_join, else null.
    virtual const pascal_join::Project* compilands() const = 0;
    virtual std::map<std::string, Bytes> headers() const = 0;
    virtual std::vector<std::string> include() const = 0;
    virtual std::optional<std::string> switches() const = 0;
    virtual std::vector<std::uint32_t> starts() const = 0;                 // the code starts of src/MODULES, ascending (empty: none)
    virtual bool whole(const std::filesystem::path& file) const = 0;       // a whole source file: its text lies in one piece
    virtual ByteView image() const = 0;                                    // the program's image
    virtual std::uint32_t code_end() const = 0;                            // where the code ends (0: not given)
    virtual std::vector<LabelledToolchain> toolchains() const = 0;
    virtual SymbolTable symbols() const = 0;
    virtual void save_symbols(const SymbolTable& symbols) const = 0;
    virtual std::vector<SourceFile> sources() const = 0;
    // The project's source sets: the project itself, then those of "also". Each has its own folder and toolchains.
    virtual std::vector<std::shared_ptr<const Project>> sets() const = 0;
    // What compare::function takes of the program.
    virtual compare::Program program() const = 0;
    virtual const x86::Decoder16& decoder() const { return x86::capstone_decoder(); }
};

// One function of a file (or the file, when it does not compile: no function, and its last console line as the note).
struct Row {
    std::string file;                        // the source file's name (a Pascal procedure: "COMPILAND pFIRST.pas")
    std::optional<std::string> function;
    std::optional<std::uint32_t> address;    // where the program has it (none: no place)
    std::uint32_t length = 0;
    Verdict verdict = Verdict::NoPlace;
    std::vector<std::string> notes;          // what is wrong ("? " first: a warning only)
    std::vector<std::string> by;             // the labels of the toolchains that give the function's bytes
};

// One source file compiled by one toolchain and held to the program at `at`. `symbols` is the file's own copy of
// SYMBOLS (the names its functions meet are added to it); `learnt` gets the places of the functions found identical
// (when all the file's placed functions are), and of the names they bind. IoError when the file cannot be read;
// FormatError when the compiler's object is not an OMF module.
std::vector<Row> check_file(const Project& project, const Toolchain& toolchain, const std::filesystem::path& file,
                            std::optional<std::int64_t> at, SymbolTable symbols, SymbolTable& learnt);

// After a line /* SAMEFILE */: empty stand-ins of functions of the program's same file, which a far function calls
// another way than one of another file (push cs, call near) - they are compiled and not compared. The names are
// upper-case without their leading underscores.
std::set<std::string> stubs(std::string_view source);

// The function at `at` of `length` bytes goes on to the next code start (or to a pad of zeros before it: a segment's
// end), where `starts` are the code starts (ascending) and `code_end` where the code ends (0: not given).
bool reaches(std::span<const std::uint32_t> starts, std::uint32_t code_end, ByteView image, std::uint32_t at, std::uint32_t length);

// One file's rows under one toolchain.
struct Attempt {
    std::string label;
    std::vector<Row> rows;
};
// One file's rows under each toolchain -> its rows, a function identical when one gave it (the row of the first that
// did; else of the first that compiled; `by` names all that did).
std::vector<Row> merge(const std::vector<Attempt>& per);

// Every source of the project (a Pascal program in pieces: its compilands) under each of its toolchains. `learn`
// adds the places identical functions show to SYMBOLS. `verbose` prints a line each.
std::vector<Row> run(const Project& project, bool learn = false, bool verbose = false);

// src/LEDGER: a line a function: file, function, bytes, verdict, and the toolchains that give it (when there are
// several).
void write(const Project& project, const std::vector<Row>& rows);

// `exact PROJECT ledger [--learn] [-v] [--write]`: the counts go to stdout. Returns the exit code (0).
int command(const Project& project, const std::vector<std::string>& args);

}  // namespace cb::exact::ledger
