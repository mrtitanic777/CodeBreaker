// cb/build/module_internal.hpp -- what the pieces of the module builder (module_*.cpp) share. Not part of the API.
//
// The module builder is split by concern: the linker and the assembler (module_tools), the passes that read the original's
// instructions to decide which functions cannot be C (module_scan), packing rebuilt functions into the holes of a frame
// (module_pack), assembling the run of functions that stay assembly (module_asm16), the two builds (module_build16,
// module_build32), the checks and the gate (module_check, module_gate), the report and the L2 document.
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/build/asm16.hpp"
#include "cb/build/module.hpp"
#include "cb/decompile/describe.hpp"
#include "cb/formats/ne.hpp"

namespace cb::build::module::internal {

namespace stdfs = std::filesystem;
using Env = functions::Env;
using Place = core16::SegOff;
using Places = std::map<Place, std::string>;                  // the function that starts at each place of a 16-bit module
using Segments = std::map<std::uint32_t, formats::ne::Segment>;   // by number, from 1

// ---- small things ---------------------------------------------------------------------------------------------------------

// The last `n` bytes of `s` (all of it when it is shorter).
std::string tail_of(std::string_view s, std::size_t n);
// `s` without white space at its ends, then its last `n` bytes.
std::string trimmed_tail(std::string_view s, std::size_t n);
// What follows the first "{" of a function's text (all of it when there is none): the body.
std::string_view after_brace(std::string_view text);
// Whether the environment variable is set and not empty; its value ("" when it is not set).
bool env_is_set(const char* name);
std::string env_text(const char* name);
std::optional<std::string> env_value(const char* name);
// `'symbol'`: a linker symbol that holds `@`, `?` or `$` is quoted (unquoted, `@` means "read a file").
std::string quote_symbol(std::string_view symbol);
// A file's text, '\n' line ends, bytes that are not UTF-8 replaced.
std::string read_text_lossy(const stdfs::path& file);
// Whether `text` has `word` as a whole identifier (not part of a longer one).
bool has_identifier(std::string_view text, std::string_view word);
// `codebreaker_code_<segment>` as a whole identifier in `text`: the C reads its segment's own bytes through CS.
bool reads_code_data(std::string_view text, std::int64_t segment);

// ---- running the tools ----------------------------------------------------------------------------------------------------

struct ToolRun {
    std::uint32_t code = 0;
    std::string out, err;
    std::string said() const { return out + err; }
};
ToolRun run_tool(const stdfs::path& exe, std::vector<std::string> args, const stdfs::path& cwd, const Env& env,
                 std::chrono::milliseconds timeout = std::chrono::minutes(30));

// Run the linker on `lines` (written to m.lnk in `folder`). -> the m.dll it made (nullopt: none), everything it said.
struct Linked {
    std::optional<stdfs::path> image;
    std::string log;
};
Linked link_image(const stdfs::path& linker, const Env& env, const stdfs::path& folder, const std::vector<std::string>& lines);

// Link; give whatever is undefined a stand-in and link again; whatever is still undefined after that is left unresolved (and
// reported). `stand_in(symbol)` gives C text or nothing; `compile_extra(stem, text)` gives "" or why it does not compile.
std::optional<stdfs::path> link_resolving(Built& b, const stdfs::path& linker, const Env& env, const stdfs::path& folder,
                                          std::vector<std::string> lines,
                                          const std::function<std::optional<std::string>(const std::string&)>& stand_in,
                                          const std::function<std::string(const std::string&, const std::string&)>& compile_extra);

// A stand-in for something the link could not find: the header's declaration of it with an empty body, or -- when it was never
// declared -- the function a call to an undeclared name assumes.
std::optional<std::string> stand_in_from_header(std::string_view header, std::string_view symbol);

// The original's DOS part (the bytes before its NE header) as a stub for the linker: its MZ header made to say just that much.
// -> the file's name in `folder`, or nullopt.
std::optional<std::string> write_dos_stub(const stdfs::path& original, const stdfs::path& folder);

// The assembler, with what it made of a text kept in the build cache: a piece the same as in an earlier build, or earlier in this
// one, is not assembled again.
class Assembler {
public:
    Assembler(const Services& services, stdfs::path exe, Env env, stdfs::path folder)
        : services_(services), exe_(std::move(exe)), env_(std::move(env)), folder_(std::move(folder)) {}
    bool available() const { return !exe_.empty(); }
    const stdfs::path& exe() const { return exe_; }
    const stdfs::path& folder() const { return folder_; }
    const Env& env() const { return env_; }
    struct Result {
        std::uint32_t code = 0;   // 0, or the assembler's exit code
        std::string message;
    };
    // Write `text` as <stem>.asm in the folder and assemble it into <stem>.obj.
    Result assemble(const std::string& stem, std::string_view text,
                    const std::vector<std::string>& flags = {"-q", "-zcm"}) const;
    // What the assembler is: its bytes, the files it may read, the options it reads (a cache key).
    std::string tool_inputs() const;

private:
    const Services& services_;
    stdfs::path exe_;
    Env env_;
    stdfs::path folder_;
};

// An object of `n` bytes of INT 3 in code segment `seg`: a place kept where it was. -> its stem.
std::string write_filler(const Assembler& assembler, std::uint32_t seg, std::string_view stem, std::uint32_t n);

// {function name: the symbol its object defines and the object, relative to `folder`}: as the compiler decorates a C name -- _name
// (__cdecl, or __stdcall with an open argument list), _name@N (__stdcall), name_ (register arguments).
std::map<std::string, PublicSymbol> publics_of(const Services& services, const stdfs::path& folder,
                                               const std::map<std::string, std::string>& stems);

// ---- passes over the original's instructions (16-bit) ---------------------------------------------------------------------

// Where a function's instructions put a value in AX that it returns with: walking back from each return through its straight run of
// instructions, the first one writing AX (or a call: what the callee left there goes back to the caller as it came) -- one past the
// prologue's (`mov ax, ds` / `push ds; pop ax` / `mov ax, DGROUP` ahead of `mov ds, ax`), and not a `pop ax` (its own AX given back).
std::optional<std::uint32_t> result_in_ax(ByteView code, std::uint32_t start, std::uint32_t end);
// Whether every fix-up the original makes to a place of its own is by the segment's number -- none through the entry table (a
// movable segment's), none a far pointer.
bool segment_number_fixups(const formats::ne::Module& ne);
// Whether a function's instructions load DS and do not put it back (`own_data_loads`: the `mov ax, imm` instructions that are fixed to the
// module's own data segment, whose `mov ds, ax` changes nothing for a caller that has it already).
bool leaves_ds(const std::vector<decompile::describe::Insn>& ins, const std::set<std::int64_t>& own_data_loads = {});
// Whether DS was loaded from the module's own data segment before instruction `k` (see leaves_ds).
bool loads_own_ds_before(const std::vector<decompile::describe::Insn>& ins, std::size_t k, const std::set<std::int64_t>& own_data_loads);
// Where a self-loading KERNEL cuts its own code segment after start-up: GlobalReAlloc of CS to `mov cx, size`.
std::optional<std::uint32_t> resident_limit_of(ByteView code);
// Functions whose C cannot say what the original passes or returns in a register: {name: why}.
std::map<std::string, std::string> registers_c_cannot_carry(const formats::ne::Module& ne, const Segments& segs, const Places& at,
                                                            const std::map<std::string, std::string>& texts);
// The functions whose addresses sit in the automatic data segment as near pointers (a table of handlers the code calls through).
std::set<std::string> near_pointer_tables(const formats::ne::Module& ne, const Places& at);
// The functions whose original loads AX from its frame just before a far call through a pointer in its frame (a VxD's API entry).
std::set<std::string> register_argument_calls(const formats::ne::Module& ne, const Places& at);

// ---- packing the functions rebuilt from C into the holes of a frame -----------------------------------------------------------

struct HoleSpec {
    std::uint32_t lo = 0, hi = 0;
    std::optional<std::string> name;   // the function rebuilt from C whose place it is
};
struct Bin {
    std::uint32_t lo = 0, hi = 0;
    std::vector<std::pair<std::uint32_t, std::string>> objects;   // (pad, name)
    std::uint32_t used = 0;
};
struct Packed {
    std::vector<Bin> bins;
    std::vector<std::string> no_room_far, no_room_near;
};
// Each function in its own hole first, when it fits; the rest packed into what room is left (near ones first, then the biggest).
// `size`: bytes of code of each; `aside`: far ones sent elsewhere first; `limit`: where the segment's resident part ends -- a
// function from below it goes only into a hole that ends below it too (KERNEL keeps only the first A940h bytes of its segment 1).
Packed pack_holes(const std::vector<HoleSpec>& holes, const OrderedMap<std::string, std::uint32_t>& size,
                  const std::set<std::string>& near, const std::set<std::string>& aside, std::optional<std::uint32_t> limit,
                  bool word_aligned = true);

// ---- assembly in a 16-bit module -----------------------------------------------------------------------------------------------

// A piece of assembly and the instructions kept as text for it.
struct Piece {
    std::unique_ptr<asm16::Function> f;
    std::set<std::uint32_t> good;
    std::optional<std::string> assembled;   // "" when it assembled back to the original's bytes, else why not
};

using ElsewhereFn = std::function<std::optional<std::string>(const std::string&)>;   // the stem of a far function's copy beside its segment
using PieceAssembler = std::function<std::string(const std::string& stem, Piece&, std::uint32_t lo, std::uint32_t hi)>;

// The frame of a segment with assembly in it in the order the linker is to lay it out. -> the object stems, or nullopt.
std::optional<std::vector<std::string>> frame_pieces(Built& b, Piece& piece, std::uint32_t seg, std::map<std::string, std::string>& stems,
                                                     const stdfs::path& folder, const Assembler& assembler,
                                                     const PieceAssembler& assemble_piece, const ElsewhereFn& elsewhere);

// Record a function as built from assembly (`stem`).
void note_assembled(Built& b, const std::string& name, std::map<std::string, std::string>& stems, const std::string& stem);

struct AssemblyJob {
    const Services& services;
    Built& b;
    const formats::ne::Module& ne;
    const Places& at;
    const Segments& segs;
    std::map<std::string, std::string>& stems;
    const std::map<std::string, PublicSymbol>& publics;
    const stdfs::path& folder;
    const Assembler& assembler;
    const std::map<std::int64_t, Bytes>& carried;
    const std::set<std::string>& forced;
    const ElsewhereFn& elsewhere;
};
// The assembly of the module's own instructions where C does not do (build/asm16): each code segment's lead-in -- what is before its
// first function, often a variable the code reads through CS -- linked first into the segment so that it stays where it was; and each
// rejected function, instead of a stand-in, when it can be written so and assembles back to the original's bytes.
// -> {import symbol without its underscore: the import} they need.
std::map<std::string, asm16::Import> assemble16(AssemblyJob& job);

// ---- the gate -----------------------------------------------------------------------------------------------------------------

// The keys the gate's verdicts are kept by: what each is made of (the original module, the checker's tools, the header, the function's
// text, what the description says of it and what it calls).
std::map<std::string, std::string> gate_keys(const Services& services, const stdfs::path& module, const std::vector<std::string>& names,
                                             const decompile::c_text::Module16& m);

}  // namespace cb::build::module::internal
