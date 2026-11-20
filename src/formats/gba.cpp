#include "formats/gba.hpp"

#include <algorithm>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::gba {

const std::array<std::uint8_t, 156> kLogo = {
    0x24, 0xFF, 0xAE, 0x51, 0x69, 0x9A, 0xA2, 0x21, 0x3D, 0x84, 0x82, 0x0A, 0x84, 0xE4, 0x09, 0xAD, 0x11, 0x24, 0x8B, 0x98, 0xC0, 0x81, 0x7F, 0x21,
    0xA3, 0x52, 0xBE, 0x19, 0x93, 0x09, 0xCE, 0x20, 0x10, 0x46, 0x4A, 0x4A, 0xF8, 0x27, 0x31, 0xEC, 0x58, 0xC7, 0xE8, 0x33, 0x82, 0xE3, 0xCE, 0xBF,
    0x85, 0xF4, 0xDF, 0x94, 0xCE, 0x4B, 0x09, 0xC1, 0x94, 0x56, 0x8A, 0xC0, 0x13, 0x72, 0xA7, 0xFC, 0x9F, 0x84, 0x4D, 0x73, 0xA3, 0xCA, 0x9A, 0x61,
    0x58, 0x97, 0xA3, 0x27, 0xFC, 0x03, 0x98, 0x76, 0x23, 0x1D, 0xC7, 0x61, 0x03, 0x04, 0xAE, 0x56, 0xBF, 0x38, 0x84, 0x00, 0x40, 0xA7, 0x0E, 0xFD,
    0xFF, 0x52, 0xFE, 0x03, 0x6F, 0x95, 0x30, 0xF1, 0x97, 0xFB, 0xC0, 0x85, 0x60, 0xD6, 0x80, 0x25, 0xA9, 0x63, 0xBE, 0x03, 0x01, 0x4E, 0x38, 0xE2,
    0xF9, 0xA2, 0x34, 0xFF, 0xBB, 0x3E, 0x03, 0x44, 0x78, 0x00, 0x90, 0xCB, 0x88, 0x11, 0x3A, 0x94, 0x65, 0xC0, 0x7C, 0x63, 0x87, 0xF0, 0x3C, 0xAF,
    0xD6, 0x25, 0xE4, 0x8B, 0x38, 0x0A, 0xAC, 0x72, 0x21, 0xD4, 0xF8, 0x07};

namespace {

constexpr std::string_view kReader = "gba";
constexpr std::size_t kFixedAt = 0xB2;

std::uint32_t word_at(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | static_cast<std::uint32_t>(b[at + 1]) << 8 | static_cast<std::uint32_t>(b[at + 2]) << 16 |
           static_cast<std::uint32_t>(b[at + 3]) << 24;
}

// An ARM branch (B or BL, any condition but the never-condition): bits 27-25 are 101.
bool is_branch(std::uint32_t w) { return (w & 0x0E000000) == 0x0A000000 && (w >> 28) != 0xF; }

// A text field of the header, trailing zeros removed.
std::string text_field(const engine::Node& header, std::string_view name) {
    std::string s = header.get(name).text;
    while (!s.empty() && s.back() == '\0') s.pop_back();
    return s;
}

}  // namespace

std::uint8_t header_complement(ByteView header) {
    unsigned sum = 0x19;
    for (std::size_t i = 0xA0; i <= 0xBC; ++i) sum += header[i];
    return static_cast<std::uint8_t>(-static_cast<int>(sum));
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    std::set<std::uint32_t> starts = {0};   // (the header's own branch: the first instruction)
    if (header.entry && *header.entry >= kRomBase && *header.entry < end()) starts.insert(*header.entry - kRomBase);
    const std::vector<std::uint32_t> found = find_functions(data, std::move(starts), decode, {.begin = 0, .end = static_cast<std::uint32_t>(data.size())});
    std::vector<std::uint32_t> out;
    for (const std::uint32_t a : found) out.push_back(a + kRomBase);
    return out;
}

Rom parse(ByteView data) {
    const engine::Tree tree = engine::read_builtin(kReader, data);   // config/formats/gba.fmt
    const engine::Node& d = tree.root.get("header");
    Rom rom;
    rom.data.assign(data.begin(), data.end());
    Header& h = rom.header;
    h.entry_word = static_cast<std::uint32_t>(d.u("entry_word"));
    if (d.u("is_branch")) h.entry = static_cast<std::uint32_t>(d.u("entry"));
    h.logo_ok = equal_bytes(engine::bytes_of(tree, d.get("logo")), kLogo);
    h.title = text_field(d, "title");
    h.game_code = text_field(d, "game_code");
    h.maker_code = text_field(d, "maker_code");
    h.unit_code = static_cast<std::uint8_t>(d.u("unit_code"));
    h.device_type = static_cast<std::uint8_t>(d.u("device_type"));
    h.version = static_cast<std::uint8_t>(d.u("version"));
    h.complement = static_cast<std::uint8_t>(d.u("complement"));
    h.computed_complement = static_cast<std::uint8_t>(d.u("computed_complement"));
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, kHeaderSize);
    if (!head || head->size() < kHeaderSize) return false;
    if ((*head)[kFixedAt] != kFixedValue) return false;
    return is_branch(word_at(*head, 0));
}

}  // namespace cb::formats::gba
