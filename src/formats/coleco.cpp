#include "formats/coleco.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::coleco {

namespace {

constexpr std::string_view kReader = "coleco";
constexpr std::size_t kVectors = 0x0C;       // the first vector's offset: RST $08
constexpr std::size_t kNmi = 0x21;
constexpr std::size_t kTitle = 0x24;


// Does a vector hold something the BIOS may jump to: a JMP, a RET, RETI or RETN?
bool used(ByteView d, std::size_t at) {
    if (at + 3 > d.size()) return false;
    if (d[at] == 0xC3 || d[at] == 0xC9) return true;
    return d[at] == 0xED && (d[at + 1] == 0x4D || d[at + 1] == 0x45);
}

}  // namespace

Program parse(ByteView data) {
    if (data.size() > kMaxSize) {
        throw UnsupportedError(kReader, 0, std::format("a {} KB cartridge switches banks (a mapper), not read yet", data.size() / 1024));
    }
    const engine::Tree tree = engine::read_builtin(kReader, data);   // config/formats/coleco.fmt
    const engine::Node& h = tree.root.get("header");
    Program p;
    p.header.title_screen = h.u("title_screen") != 0;
    p.header.sprite_names = static_cast<std::uint16_t>(h.u("sprite_names"));
    p.header.sprite_order = static_cast<std::uint16_t>(h.u("sprite_order"));
    p.header.work_buffer = static_cast<std::uint16_t>(h.u("work_buffer"));
    p.header.controllers = static_cast<std::uint16_t>(h.u("controllers"));
    p.header.start = static_cast<std::uint16_t>(h.u("start"));
    for (std::size_t i = kTitle; i < data.size() && i < kTitle + 0x40 && data[i] >= 0x20 && data[i] < 0x7F; ++i) p.header.title.push_back(static_cast<char>(data[i]));
    p.image.assign(data.begin(), data.end());
    const std::uint32_t end = kOrigin + static_cast<std::uint32_t>(data.size());
    if (p.header.start >= kOrigin + kTitle && p.header.start < end) p.entries.push_back({p.header.start, "start"});
    static constexpr const char* kNames[] = {"rst08", "rst10", "rst18", "rst20", "rst28", "rst30", "irq"};
    for (std::size_t i = 0; i < 7; ++i) {
        const std::size_t at = kVectors + 3 * i;
        if (used(data, at)) p.entries.push_back({static_cast<std::uint16_t>(kOrigin + at), kNames[i]});
    }
    if (used(data, kNmi)) p.entries.push_back({static_cast<std::uint16_t>(kOrigin + kNmi), "nmi"});
    return p;
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const bool col = has_extension(path, {".col"});
    if (!col && !has_extension(path, {".rom", ".bin"})) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size || *size < kTitle || (!col && *size > kMaxSize)) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, 2);
    return head && head->size() == 2 && ((*head)[0] == 0xAA || (*head)[0] == 0x55) && ((*head)[0] ^ (*head)[1]) == 0xFF;
}

const std::map<std::uint32_t, std::string>& bios_names() {
    // config/knowledge/coleco/registers.json
    static const std::map<std::uint32_t, std::string> kNames = knowledge::standard().table_names("coleco", "bios");
    return kNames;
}

}  // namespace cb::formats::coleco
