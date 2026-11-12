// cb/recompile/config.hpp -- the XenonRecomp configuration of a title, from what CodeBreaker finds in it.
//
//     Title title{.image = image, .run_names = names};                   // an image, as discover reads it, and its save/restore runs
//     std::string toml = make(title, dis, "game.xex", {.out_dir = "ppc"});
//
// XenonRecomp takes a title's functions from its .pdata and follows the calls itself; what the configuration adds is what it
// cannot find:
//
//   * the compiler's register save/restore helpers (__savegprlr_14, __restgprlr_14, __savefpr_14, __restfpr_14,
//     __savevmx_14/_64, __restvmx_14/_64): each run is recompiled as one function with an entry per register count;
//   * every function .pdata leaves out (recompile/discover: leaf functions, those reached only through a pointer in the data
//     or an address the code builds), with its length;
//   * the switch tables (XenonAnalyse's, written to their own file);
//   * the register settings, which must match the runtime's ppc_config.h (native/x360) since they change PPCContext's
//     layout;
//   * the title's hooks: `midasm_hook` tables from a hooks file, copied as they are.
//
// The image is given as `Title`: what recompile/discover reads of it, plus the names of its save/restore runs. Making one
// from an .xex belongs to the formats modules (powerpc_image, xex); this module has no file format in it except XenonAnalyse's
// and the switch finder's table file, a TOML of `[[switch]]` tables, read here by a reader of its own (no TOML library).
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/recompile/disassemble.hpp"
#include "cb/recompile/discover.hpp"
#include "cb/recompile/switches.hpp"

namespace cb::recompile::config {

// The registers kept as locals of each function (the runtime is built with the same settings: x360.cmake reads them back from
// the generated ppc_config.h). LR stays in the context (backtraces and unwinding read it), and so do the non-volatile
// registers: a compiler-generated __finally block is called with `bl` and reads its parent's r30/r31, which a local of the
// parent would hide. CR, CTR and XER stay there too: a function entered by a branch from another can test the flags or count
// down the CTR its caller set (Up: `cmplwi cr6,r11,1; b 0x8212E5F4`, which starts with `bne cr6`).
struct RegisterSetting {
    std::string_view key;
    bool value;
};
inline constexpr std::array<RegisterSetting, 8> kRegisterSettings = {{
    {"skip_lr", false},
    {"skip_msr", true},
    {"ctr_as_local", false},
    {"xer_as_local", false},
    {"reserved_as_local", true},
    {"cr_as_local", false},
    {"non_argument_as_local", false},
    {"non_volatile_as_local", false},
}};

// The save/restore runs of an image by their entry address: "__savegprlr_14", "__restfpr_14" ...
using RunNames = std::map<std::uint32_t, std::string>;

// What the configuration reads of a title.
struct Title {
    discover::Image image;       // the code, the .pdata functions, the imported ones
    RunNames run_names;
};

// {table base (the bctr): [case labels]}, in the order the file gives them; that order is how the function holding a switch
// grows to take in the next one whose case labels it reaches.
using SwitchTables = OrderedMap<std::uint32_t, std::vector<std::uint32_t>>;

// {config key: address}, keys in order.
using Addresses = std::map<std::string, std::uint32_t>;

// The save/restore helpers the image has, by their config key: savegprlr_14_address, restgprlr_14_address,
// savefpr_14_address, restfpr_14_address (from the run names), and savevmx_14_address, savevmx_64_address,
// restvmx_14_address, restvmx_64_address (found in the code: `li r11,-0x120` / `li r11,-0x400`, then stvx or lvx through r12).
// setjmp_address and longjmp_address are added when the image has them (see jump_buffers).
Addresses helpers(const Title& title, disassemble::Disassembler& dis);

// The first word after the zero padding (or blr) that comes before `address`: a function's start. `code` starts at `begin`.
std::uint32_t start_before(ByteView code, std::uint32_t begin, std::uint32_t address);

// The address after the first blr, bctr or plain branch at or after `address` (the end of the code when there is none).
std::uint32_t end_of_run(ByteView code, std::uint32_t begin, std::uint32_t address);

// The CRT's setjmp and longjmp (the Xbox 360 jmp_buf: r1 at +0x90, CR at +0x130, LR at +0x134): setjmp stores r1 at 0x90(r3);
// longjmp loads LR from 0x134 and r1 from 0x90 of one register. XenonRecomp turns them into the host's setjmp/longjmp; without
// them a longjmp is a call that returns, and its caller goes on with registers it never restored. {setjmp_address,
// longjmp_address}; a longjmp with no setjmp is not given (XenonRecomp would read the missing setjmp as 0 and the first longjmp
// would jump through zeros).
Addresses jump_buffers(const Title& title);

// ---- the switch-table file ----------------------------------------------------------------------------------------

// One `[[switch]]` table of XenonAnalyse's output or of the finder's.
struct SwitchEntry {
    std::uint32_t base = 0;                   // the bctr (the finder's) or the start of the sequence (XenonAnalyse's)
    std::optional<std::uint32_t> reg;         // r: the register holding the case index
    std::uint32_t default_target = 0;         // 0 when the file names none
    std::vector<std::uint32_t> labels;
};

// The `[[switch]]` tables of a TOML text, in order. `name` is the file's name, for the error. Other tables and keys are
// read and passed over. ParseError for text that is not TOML this reader knows (integers, strings, booleans, floats, arrays
// of those, tables and arrays of tables, comments), and for a `[[switch]]` with no `base` or no `labels`.
std::vector<SwitchEntry> parse_switch_entries(std::string_view toml, std::string_view name = "<toml>");

// {table base (the bctr): [case labels]} from a switch-table file. NotFoundError / IoError for the file, ParseError for its
// text.
SwitchTables read_switch_tables(const std::filesystem::path& path);
SwitchTables switch_tables_of(std::string_view toml, std::string_view name = "<toml>");

// The tables the finder found, with those XenonAnalyse's output (`analysed`, its file's text) has for a bctr the finder did
// not resolve added: XenonAnalyse's `base` is the sequence's start, so its bctr is the first `bctr` word at or after it.
// {the finder's count, XenonAnalyse's added}. An entry of XenonAnalyse's whose base is not in the code is passed over.
struct Merged {
    std::vector<switches::SwitchTable> tables;
    std::size_t found = 0;                    // the finder's
    std::size_t added = 0;                    // XenonAnalyse's
};
Merged merge_xenon_analyse(std::vector<switches::SwitchTable> found, const switches::CodeImage& image, std::string_view analysed,
                           std::string_view name = "<xenon analyse>");

// The title's switch-table file: the finder's tables (it walks back from every bctr), with XenonAnalyse's added for any bctr
// the finder did not resolve. {the finder's count, XenonAnalyse's added}.
struct WrittenTables {
    std::size_t found = 0;
    std::size_t added = 0;
};
WrittenTables write_switch_tables(const std::filesystem::path& path, const switches::CodeImage& image, const std::optional<std::string>& analysed = std::nullopt);

// ---- the functions .pdata leaves out ----------------------------------------------------------------------------------

struct ExtraFunction {
    std::uint32_t address = 0;
    std::uint32_t size = 0;
    friend constexpr bool operator==(const ExtraFunction&, const ExtraFunction&) = default;
};

// The functions .pdata leaves out, in address order, sized to the next known start (a run of save/restore entries, and the
// import thunks, count as starts). A switch's case labels are never starts (an absolute switch table is data holding code
// addresses, which the pointer scan would take for functions), and the function that holds a switch runs to the end of its
// last case. Every branch (b, bc; no link) out of a function is a tail call: its target is a function too, to a fixed point.
// `discovered` is what recompile/discover found (its function starts).
std::vector<ExtraFunction> extra_functions(const Title& title, const discover::Functions& discovered, disassemble::Disassembler& dis,
                                           const SwitchTables& tables = {});

// ---- the configuration ------------------------------------------------------------------------------------------------

struct MakeOptions {
    std::string out_dir = "ppc";
    std::string switch_tables = "switch_tables.toml";
    std::optional<std::string> hooks;                  // TOML text appended as it is (midasm_hook tables)
    std::optional<std::string> file_path;              // the .xex as the configuration names it (default: the xex_path given)
    std::optional<SwitchTables> switches;              // the switch tables; default: read from the file `switch_tables` beside the .xex, if there is one
};

// The configuration's text. `xex_path` is where the title is; discovery runs on `title.image`.
std::string make(const Title& title, disassemble::Disassembler& dis, const std::filesystem::path& xex_path, const MakeOptions& options = {});

}  // namespace cb::recompile::config
