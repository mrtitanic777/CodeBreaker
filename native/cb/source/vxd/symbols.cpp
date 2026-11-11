// cb/source/vxd/symbols.cpp -- control messages and debug symbols of the Win95 DDK (see symbols.hpp, ddk.hpp).
#include "cb/source/vxd/symbols.hpp"

#include <algorithm>

#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::source::vxd {
namespace {

using std::string;
using std::string_view;

bool is_word_start(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
bool is_word(char c) { return is_word_start(c) || (c >= '0' && c <= '9'); }
bool is_hex_digit(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

// A line of VMM.INC of the form `  NAME   EQU   0AH` (EQU and H in capitals): (name, value). A value too large for 32
// bits is given as the largest, which is what the caller needs of it (it only asks whether the value is under 40h).
struct Equate {
    string_view name;
    std::uint32_t value = 0;
};

std::optional<Equate> equate_of(string_view line) {
    size_t i = 0;
    const auto skip_space = [&] {
        while (i < line.size() && text::is_space(line[i])) ++i;
    };
    skip_space();
    if (i >= line.size() || !is_word_start(line[i])) return std::nullopt;
    const size_t name_start = i;
    while (i < line.size() && is_word(line[i])) ++i;
    const string_view name = line.substr(name_start, i - name_start);
    const size_t gap = i;
    skip_space();
    if (i == gap || line.substr(i, 3) != "EQU") return std::nullopt;
    i += 3;
    const size_t gap2 = i;
    skip_space();
    if (i == gap2) return std::nullopt;
    const size_t digits = i;
    while (i < line.size() && is_hex_digit(line[i])) ++i;
    if (i == digits || i >= line.size() || line[i] != 'H') return std::nullopt;
    const string_view hex = line.substr(digits, i - digits);
    ++i;
    skip_space();
    if (i != line.size()) return std::nullopt;
    return Equate{name, text::parse_int<std::uint32_t>(hex, 16).value_or(UINT32_MAX)};
}

bool is_name_byte(std::uint8_t c) { return c == '_' || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }

}  // namespace

std::map<std::uint32_t, std::string> parse_control_messages(std::string_view vmm_inc_text) {
    // text mode: a lone \r ends a line as well
    string text(vmm_inc_text);
    text = text::replace_all(text, "\r\n", "\n");
    std::replace(text.begin(), text.end(), '\r', '\n');

    std::map<std::uint32_t, string> out;
    bool started = false;
    for (const string_view line : text::lines(text)) {
        const auto equate = equate_of(line);
        if (!equate) continue;
        const string name(equate->name);
        const string upper = text::upper(name);
        if (upper == "SYS_CRITICAL_INIT") started = true;
        if (!started || equate->value >= 0x40) continue;
        if (upper.ends_with("_BIT") || upper.ends_with("_FLAG") || upper.ends_with("_ID") || upper.starts_with("DIOC")) continue;
        out.try_emplace(equate->value, name);
        if (upper == "SYSTEM_EXIT2") break;   // tail of the message block
    }
    return out;
}

std::map<std::uint32_t, std::string> control_messages(const DdkFolders& ddk) {
    const auto vmm = DdkFolders::first_existing(ddk.inc, "VMM.INC");
    if (!vmm) return {};
    return parse_control_messages(fs::read_text(*vmm));
}

OrderedMap<std::uint16_t, std::string> parse_sym_names(ByteView sym_file) {
    OrderedMap<std::uint16_t, string> found;
    const size_t size = sym_file.size();
    size_t p = 0;
    while (p + 3 < size) {
        const std::uint16_t offset = static_cast<std::uint16_t>(sym_file[p] | (sym_file[p + 1] << 8));
        const size_t length = sym_file[p + 2];
        // (a name cut off by the end of the file is taken as far as it goes)
        const ByteView name = sym_file.subspan(p + 3, std::min(length, size - (p + 3)));
        const bool ok = length >= 3 && length <= 40 && std::all_of(name.begin(), name.end(), is_name_byte);
        if (ok) {
            found.try_emplace(offset, text::from_latin1(name));
            p += 3 + length;
        } else {
            p += 1;
        }
    }
    // the segment and group markers (a leading underscore and no other) are not functions
    OrderedMap<std::uint16_t, string> kept;
    for (const auto& [offset, name] : found) {
        if (!name.starts_with('_') || std::count(name.begin(), name.end(), '_') > 1) kept.try_emplace(offset, name);
    }
    return kept;
}

OrderedMap<std::uint16_t, std::string> sym_names(const DdkFolders& ddk, std::string_view device) {
    const auto dir = DdkFolders::first_existing(ddk.debug);
    if (!dir) return {};
    const string wanted = text::upper(device) + ".SYM";
    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(*dir, ec)) {
        if (text::iequals(fs::to_text(entry.path().filename()), wanted)) return parse_sym_names(fs::read_file(entry.path()));
    }
    return {};
}

}  // namespace cb::source::vxd
