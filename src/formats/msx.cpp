#include "formats/msx.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::msx {

namespace {

constexpr std::string_view kReader = "msx";
constexpr std::size_t kPage = 0x4000;
constexpr std::size_t kMaxUnmapped = 0xC000;

bool ab_at(ByteView data, std::size_t at) {
    return data.size() >= at + 2 && data[at] == 'A' && data[at + 1] == 'B';
}

}  // namespace

Program parse(ByteView data) {
    if (data.size() > kMaxUnmapped) {
        throw UnsupportedError(kReader, 0,
                               std::format("a {} KB cartridge switches banks (a mapper), not read yet", data.size() / 1024));
    }
    // The header is at the start of page 1 ($4000): the image's first byte when the cartridge starts there, or its
    // byte $4000 when it fills pages 0-2 from $0000 (48 KB: msx-wrally).
    const engine::Tree tree = engine::read_builtin(kReader, data);   // config/formats/msx.fmt finds the "AB" at $0000 or $4000
    const engine::Node& h = tree.root.get("header");
    const std::size_t at = h.offset;
    std::uint32_t origin = at == 0 ? 0x4000 : 0x0000;
    Program p;
    p.header.init = static_cast<std::uint16_t>(h.u("init"));
    p.header.statement = static_cast<std::uint16_t>(h.u("statement"));
    p.header.device = static_cast<std::uint16_t>(h.u("device"));
    p.header.text = static_cast<std::uint16_t>(h.u("text"));
    if (at == 0 && p.header.init != 0 && p.header.init < 0x4000) {
        origin = 0x0000;
    } else if (at == 0 && p.header.init >= 0x8000 && data.size() <= kPage) {
        origin = 0x8000;
    }
    p.origin = origin;
    p.image.assign(data.begin(), data.end());
    const std::pair<std::string_view, std::uint16_t> named[] = {
        {"init", p.header.init}, {"statement", p.header.statement}, {"device", p.header.device}};
    for (const auto& [name, address] : named) {
        const bool inside = address != 0 && origin <= address && address < origin + data.size();
        const bool taken = std::any_of(p.entries.begin(), p.entries.end(),
                                       [&](const Entry& e) { return e.address == address; });
        if (inside && !taken) p.entries.push_back({address, name});
    }
    return p;
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".rom", ".mx1", ".mx2"})) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, 2);
    if (!head) return false;
    if (ab_at(*head, 0)) return true;
    const std::optional<Bytes> at_page = read_prefix(path, kPage, 2);
    return at_page && ab_at(*at_page, 0);
}

}  // namespace cb::formats::msx
