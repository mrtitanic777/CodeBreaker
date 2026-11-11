// cb/exact/habits/cmpp.hpp -- a Pascal procedure compiled by the original compiler, compared with the executable.
//
//     cmpp M.EXE 0x00001 m100/p00001.pas [--proc NAME] [--asm] [--cod]
//
// The file is a small complete Pascal program: the declarations the procedure needs, stubs for the procedures it calls,
// and the procedure itself, named P<address> (P00001 for the one at image 0x00001) unless --proc says otherwise. It is
// compiled with the MS-Pascal compiler of 1981 (tools/pcc: the `PascalCompiler` below) and the procedure's bytes are
// compared with the executable's. Not compared, because they depend on where things are placed: the bytes a linker
// fills in (addresses of variables and constants, far call targets) and the displacement of a near call to another
// procedure. The report says IDENTICAL, or the instructions that differ. --asm prints the original procedure and
// compiles nothing; --cod prints the compiler's own listing of the compiled procedure.
//
// `walk` and `loose_bytes` are also the instruction-level view the newer comparers (newcomp, pascal_join) build on.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/exact/x86.hpp"

namespace cb::exact::habits::cmpp {

// ---- the code of a procedure, instruction by instruction ------------------------------------------------------------
// What follows a procedure's call of the entry helper: the compiler's frame data, which is not code. The 1981 compiler
// writes a byte and a word behind the call; MASM-era Pascal 3.x only the frame size word.
enum class EntryData { ByteAndWord, Word };

struct Options {
    EntryData entry_data = EntryData::ByteAndWord;
    const x86::Decoder16* decoder = nullptr;   // null: the Capstone 16-bit decoder
};

// One row of a listing. The entry data are a row of their own (mnemonic "hdr", text "db 0x.. ; dw 0x...." or "dw 0x....");
// a byte that decodes to no instruction is "db 0x.." (mnemonic "db").
struct Row {
    std::uint32_t offset = 0;
    std::uint32_t size = 0;
    std::string text;
    std::string mnemonic;
    friend bool operator==(const Row&, const Row&) = default;
};

// The rows of `code`, decoded as if it began at `base`. With `skip_header`, a procedure that begins with the entry
// helper's call (E8) has its entry data (at offset 3) kept out of the instruction stream; if the data are not all there
// the bytes are simply decoded.
std::vector<Row> walk(ByteView code, std::uint32_t base = 0, bool skip_header = true, const Options& options = {});

// The offsets of `code` not compared: those of `fixups` (what the linker fills in), the displacement of a near call to
// outside the procedure, and the four bytes of a far call or jump.
std::set<std::uint32_t> loose_bytes(ByteView code, const std::set<std::uint32_t>& fixups = {}, const Options& options = {});

// ---- the compiler, and the comparison ---------------------------------------------------------------------------------
// What the compiler made of the file.
struct Compiled {
    std::optional<Bytes> code;   // the code segment; none: the compiler produced none
    struct Span {
        std::uint32_t offset = 0, size = 0;
    };
    std::unordered_map<std::string, Span> procedures;   // by upper-case name: where each lies in `code`
    std::vector<std::uint32_t> fixups;                  // the offsets in `code` of the words the linker fills in
    std::string listing, console, cod;                  // the compiler's listing, what it printed, its code listing
};

// The tool driver (tools/pcc): the original executable's image, and a file compiled with the period compiler. The seam
// is real: the compiler runs in an emulator or DOSBox, a test supplies a fake.
class PascalCompiler {
public:
    virtual ~PascalCompiler() = default;
    virtual Bytes image(const std::string& exe) = 0;
    virtual Compiled compile(const std::filesystem::path& source) = 0;
};

struct Request {
    std::string exe;                 // M.EXE: the ledger of its procedures is <ledger dir>/<EXE>.json
    std::uint32_t address = 0;       // the procedure's address in the image
    std::filesystem::path source;
    std::optional<std::string> procedure;   // --proc: its name in the file (default P<address>)
    bool original_only = false;      // --asm
    bool show_listing = false;       // --cod
};
// The command line as the tool takes it (the program's own name not included). UsageError for a malformed one.
Request parse_request(std::span<const std::string> args);

struct Difference {   // an instruction both listings have, of the same size, that differs
    std::uint32_t address = 0;
    std::string original, compiled;
};
struct StepRow {
    std::string original, compiled;   // the compiled text starts with "*" when any of its bytes is not compared
};
struct OutOfStep {   // where the two listings stop lining up, and the next instructions of each
    std::uint32_t original_address = 0;
    std::uint32_t compiled_offset = 0;
    std::vector<StepRow> rows;
};

struct Comparison {
    enum class Verdict { Identical, Differs, NoCode, OriginalOnly };
    Verdict verdict = Verdict::Differs;
    std::string procedure;           // its name in the compiled file
    std::uint32_t address = 0;
    std::string exe;
    std::uint32_t original_size = 0;
    std::uint32_t compiled_size = 0;
    std::size_t not_compared = 0;    // bytes of the compiled procedure left out of the comparison
    std::vector<Row> original_rows;  // OriginalOnly: the original procedure's listing (offsets within it) ...
    Bytes original_code;             // ... and its bytes
    std::vector<Difference> differences;   // Differs: the first twelve
    std::optional<OutOfStep> out_of_step;
    std::vector<std::string> diagnostics;  // NoCode: the compiler's lines that name a problem, else the end of its console
    std::string compiler_listing;    // --cod: the compiler's own listing of the procedure
};

// Compare. `ledger_dir` holds <EXE>.json: {"0x00001": {"size": N, ...}, ...} for the procedures of each executable.
// NotFoundError/ParseError/FormatError for a missing or damaged ledger, an address it does not list, or an image that
// does not hold the procedure.
Comparison compare(const Request& request, PascalCompiler& compiler, const std::filesystem::path& ledger_dir,
                   const Options& options = {});

// The report as the tool prints it, and the exit status that goes with it (0 identical or listed, 1 differs, 2 no code).
std::string render(const Comparison& comparison);
int exit_status(const Comparison& comparison);

}  // namespace cb::exact::habits::cmpp
