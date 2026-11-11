// cb/exact/pascal_join.hpp -- a Pascal program written in pieces, joined into its compilands, compiled, compared.
//
// A compiland of an MS-Pascal program is one source (a PROGRAM or a MODULE) holding all its procedures; the
// reconstruction keeps it in pieces so that each procedure can be written and checked alone:
//
//     <dir>/GLOBALS.pas    types and variables          <dir>/EXTERNS.pas   procedures of other compilands
//     <dir>/FORWARDS.pas   forward declarations         <dir>/MAIN.pas      the compiland's own block
//     <dir>/pADDR.pas      the procedure P<ADDR> (ADDR: its place in the program); one nested in it is
//                          <dir>/pADDR.d/pNEST.pas, pulled in by a line {+pNEST}
//     a heading marked {STUB} (something to call) or {TODO} (not written yet) is not compared
//
// joined in address order, compiled by the project's toolchain, each procedure held to the program at its place.
// The compiler of 1981 runs out of memory on a large compiland: it is compiled in "parts" runs, each with the
// procedures of one run in full and the others as empty bodies (their headings: something to call).
//
// Source text is bytes, each one character (Latin-1), read, joined and handed to the compiler undecoded.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
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

namespace cb::exact::pascal_join {

enum class CompilandKind { Program, Module };

// A compiland's entry of project.json's "compilands".
struct CompilandConfig {
    std::string dir;
    CompilandKind kind = CompilandKind::Program;
    std::uint32_t main_at = 0;                              // where its own block is in the program
    int parts = 1;                                          // compile runs
    std::optional<std::vector<std::string>> over;           // overlay folders (else the project's)
    std::optional<std::vector<std::string>> dirs;           // else {dir}
    std::optional<std::int64_t> unit;
    bool stackck = false;
    std::optional<std::string> equivalent;                  // the name of an equivalence comparer
};

// The keys of project.json that pascal_join reads.
struct Config {
    std::string root = ".";
    std::vector<std::string> over;
    std::optional<std::string> map;          // a JSON file {COMPILAND: {PIECE: "address"}}: where a piece of another program's is here
    std::optional<std::string> kinds;        // a JSON file {"address": {"kind": "check" | ...}}: stack check or none
    std::string procedures;                  // a JSON file {"0xADDR": {"size", "unit"}}: every procedure of the program
    std::map<std::string, std::int64_t> units;   // compiland -> its unit number (lookup)
    std::optional<std::string> equivalent;
    OrderedMap<std::string, CompilandConfig> compilands;   // in project.json's order: they are run in it
};

struct CompileResult {
    std::optional<Bytes> object;
    std::string console;
};

// exact/mspascal, as pascal_join reads it.
class Toolchain {
public:
    virtual ~Toolchain() = default;
    virtual CompileResult compile(ByteView source, std::string_view file_name) const = 0;
    // The bytes after the entry helper's call (what an equivalence comparer needs to know of the compiler).
    virtual std::uint32_t helper_data() const { return 3; }
};

// What an equivalence comparer says of two spellings of one procedure.
struct Equivalence {
    bool same = false;
    std::vector<std::string> why;
};
// (what the program has, what was compiled, the offsets the linker fills, the toolchain's helper_data)
using Equivalent = std::function<Equivalence(ByteView, ByteView, const std::set<std::uint32_t>&, std::uint32_t)>;

struct Project {
    std::filesystem::path dir;
    Config config;
    ByteView image;
    const Toolchain* toolchain = nullptr;
    SymbolTable symbols;
    compare::Program program;                            // data segment (it may be below the image), load segment, code bases
    std::map<std::string, Equivalent> equivalents;       // by the name a config gives (lookup)
    const x86::Decoder16* decoder = nullptr;             // null: Capstone's
};

class Compiland {
public:
    Compiland(const Project& project, std::string name, const CompilandConfig& config);

    std::uint32_t address_of(std::string_view piece_hex) const;   // a piece's place in this program (its map's, else the number)
    std::vector<std::string> pieces() const;                      // pADDR.pas files, in address order
    // The runs the compiland is compiled in: the pieces of each, MAIN.pas last of the last.
    std::vector<std::vector<std::string>> parts() const;
    // The source of one run: the others' procedures as empty stubs where something calls them.
    std::string source(const std::vector<std::string>& part) const;
    // The procedures of a run that are compared (hex addresses; "MAIN" for the block).
    std::vector<std::string> names_of(const std::vector<std::string>& part) const;

    const std::string& name() const { return name_; }
    const CompilandConfig& config() const { return config_; }
    bool has_map() const { return !map_.empty(); }
    bool maps(std::string_view piece_hex) const;
    const std::vector<std::filesystem::path>& directories() const { return dirs_; }   // the overlays, then the compiland's own folder(s)

private:
    std::filesystem::path where(std::string_view file) const;
    std::string check(std::string_view piece) const;
    std::string expand(std::string_view text, const std::string& dir, const std::optional<std::string>& own) const;
    std::string piece(std::string_view file) const;
    std::string read(std::string_view file) const;
    std::map<std::string, std::string> result_types() const;
    std::string stub(std::string_view file, const std::map<std::string, std::string>& types) const;

    const Project& project_;
    std::string name_;
    CompilandConfig config_;
    std::vector<std::filesystem::path> dirs_;
    std::filesystem::path src_;
    std::string headline_, empty_;
    std::map<std::string, std::string> map_;            // piece (upper-case hex) -> its place (lookup)
    bool over_ = false;
    struct Edit { std::string file, old_text, new_text; };
    std::vector<Edit> edits_;                           // lowest overlay first
    std::set<std::string> removed_;
    std::map<std::uint32_t, std::string> kinds_;        // procedure address -> "check" or other (lookup)
};

enum class Verdict { DoesNotCompile, NoPlace, Identical, Equivalent, Differs };
std::string_view verdict_text(Verdict v);   // "does not compile", "no place", "identical", "equivalent", "differs"

// One compared procedure (or the console of a compile that failed).
struct Row {
    std::string label;                          // "COMPILAND pFIRST.pas"
    std::optional<std::string> procedure;       // P<ADDR>, or the compiland's name for its block
    std::optional<std::uint32_t> address;
    std::uint32_t length = 0;
    Verdict verdict = Verdict::NoPlace;
    std::vector<std::string> notes;             // what is wrong ("? " first: a warning)
};

struct Procedure {
    std::optional<std::uint32_t> size;
    std::optional<std::int64_t> unit;
};
// The program's procedures by address, from the project's "procedures" file.
std::map<std::uint32_t, Procedure> procedures(const Project& project);

// Compile every compiland (each run of it) and hold its procedures to the program. `verbose` prints a line each.
std::vector<Row> run(const Project& project, bool verbose = false);

}  // namespace cb::exact::pascal_join
