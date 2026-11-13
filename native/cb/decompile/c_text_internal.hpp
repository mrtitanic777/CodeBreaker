// cb/decompile/c_text_internal.hpp -- what the parts of c_text (c_text_text.cpp, c_text_flat.cpp, c_text_win16.cpp,
// c_text_code_data.cpp) share. Not an interface: the module's is c_text.hpp.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/json_fwd.hpp"
#include "cb/decompile/c_text.hpp"

namespace cb::decompile::c_text::internal {

// Is `part` in `s`?
inline bool has(std::string_view s, std::string_view part) { return s.find(part) != std::string_view::npos; }

// ---- the C text of the headers (c_text_text.cpp) -------------------------------------------------------------------------
std::string_view types_text();      // the 32-bit module's types
std::string_view helpers_text();    // ... and the helpers it defines
std::string_view powerpc_text();    // ... and what the PowerPC path adds
// The joins, pieces and widenings the core writes for every pair of sizes up to 8 bytes beyond the ones in helpers_text, and the
// processor's floating-point operations it writes by name.
const std::string& x86_helpers();
// The C library functions the x86 helpers declare (a module of its own with a function of such a name drops the declaration).
std::span<const std::string_view> libm_names();

// The C type `uint<size>` for 1, 2, 4 and 8 bytes; `fallback` for another size.
std::string by_size(std::int64_t size, std::string_view fallback = "uint4");
bool is_by_size_type(std::string_view type);

// The structures declared: every name first (`typedef struct s_f_p s_f_p;`, so one may point at another), then each at its exact
// layout -- its fields at their offsets with filler between, the whole its full size, packed (the 32-bit header packs everything;
// a 16-bit structure says so itself). A field type `header` does not know is the bytes it takes.
std::vector<std::string> struct_decls(const std::vector<StructNote>& structs, std::string_view header, bool far16 = false);
// {structure name: its bytes}; the first of a name is kept.
std::map<std::string, std::int64_t> struct_sizes(const std::vector<StructNote>& structs);
// Each text followed by a line break.
std::string lines_of(const std::vector<std::string>& v);

// `<prefix>Ram<8 hex digits>`, whole: (prefix, digits).
std::optional<std::pair<std::string, std::string>> parse_ram_name(std::string_view name);
// `_<prefix>Ram<8 hex digits>`: the core's name for the same place read at another width.
bool is_underscored_ram_name(std::string_view name);
// The C type of a 32-bit/PowerPC global from the decompiler's name prefix (p = pointer to what follows).
std::string ram_type(std::string_view prefix, std::int64_t size, bool called);
// A global type for a 16-bit module.
std::string global_type16(std::string_view prefix, std::int64_t size, std::optional<std::int64_t> elem);
// `(.+)_(\d+)`, whole: an import symbol and its ordinal.
std::optional<std::pair<std::string, std::int64_t>> split_import_symbol(std::string_view symbol);

// ---- the library's documents (c_text_flat.cpp) ---------------------------------------------------------------------------
// A function's L2 document as the library takes it: exactly as written when the notes keep the text (a constant past 64 bits),
// else the tree as JSON.
Json document_json(const FunctionNotes& notes);

// ---- 16-bit (c_text_code_data.cpp, c_text_win16.cpp) --------------------------------------------------------------------
struct CodeData {
    std::vector<std::string> decls;
    std::map<std::int64_t, std::string> defs;
    std::map<std::int64_t, CodeSegmentReport> report;
    std::vector<std::pair<std::string, std::string>> plan;   // {function: JSON text of {"code_places": {node: "(uint2)member"}}}
};
// Each number in the C that is a place in a code segment's data, read through CS, written as that place in the segment's
// codebreaker_code_<n>: from what writing each function found (`reports` {function: the back end's code_reads facts, JSON text},
// in address order; `where` {function: (segment, offset)}).
CodeData code_segment_data(const std::vector<std::pair<std::string, std::optional<std::string>>>& reports,
                           const std::map<std::string, std::optional<SegOff>>& where, const describe::NeDescription& d,
                           const std::filesystem::path& module);
// {segment: [(offset, segment whose selector the loader puts there)]}: the module's own selector fix-ups (a selector, or the
// selector half of a far pointer), chains followed.
std::map<std::int64_t, std::vector<std::pair<std::int64_t, std::int64_t>>> selector_sites(const std::filesystem::path& module);
// The offsets from BP of the argument words the function at [start, end) of `code` loads as far pointers (`les bx, [bp+6]`).
std::set<std::int64_t> far_loads16(ByteView code, std::int64_t start, std::int64_t end);
// Whether the function at `off` loads DS with the data segment `dgroup` in its prologue.
bool loads_dgroup16(ByteView code, std::int64_t off, std::int64_t dgroup);

}  // namespace cb::decompile::c_text::internal
