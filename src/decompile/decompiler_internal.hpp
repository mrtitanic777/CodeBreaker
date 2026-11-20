// decompile/decompiler_internal.hpp -- what the files of decompiler*.cpp share and nothing outside them uses.
#pragma once

#include <cstdint>
#include <format>
#include <string>
#include <string_view>
#include <vector>

#include "core/json_fwd.hpp"
#include "decompile/decompiler.hpp"

namespace cb::decompile::decompiler::internal {

// Lower-case hexadecimal, no padding (`%llx`); 8 digits (`%08llx`); upper-case padded to `width` (`%0*llX`).
inline std::string hex(std::int64_t v) { return std::format("{:x}", static_cast<std::uint64_t>(v)); }
inline std::string hex8(std::int64_t v) { return std::format("{:08x}", static_cast<std::uint64_t>(v)); }
inline std::string upper_hex(std::int64_t v, int width) { return std::format("{:0{}X}", static_cast<std::uint64_t>(v), width); }

inline bool is_word_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
inline bool is_lower_hex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }

// Whether `text` holds `name` as a whole word: not part of a longer one.
bool has_word(std::string_view text, std::string_view name);

// The last `n` bytes of `text` (not starting inside a UTF-8 sequence).
std::string tail_bytes(std::string_view text, std::size_t n);

// Every `func_0x<8 hex>` of `text` that stands as a word of its own: the position of the `f`, and the 8 digits.
struct Placeholder {
    std::size_t at = 0;
    std::string digits;
};
std::vector<Placeholder> find_placeholders(std::string_view text);

// The facts a function is written with, in the library's tagged JSON, and what the library left in them.
Json facts_json(const c_text::ModuleFacts& facts);
// The same facts as the text the library reads (facts_json(facts) written compactly, ASCII only). A 32-bit module's shared parts
// (ModuleFacts::shared) are written once and reused.
std::string facts_text(const c_text::ModuleFacts& facts);
// What the back end reported of a function (in the facts it was given) put into the module's facts. The facts are the library's
// own form: tuples as {"$t": [...]}, sets as {"$s": [...]}, dicts with keys that are not text as {"$d": [[key, value], ...]}.
void apply_left(c_text::ModuleFacts& facts, const Json& left);

}  // namespace cb::decompile::decompiler::internal
