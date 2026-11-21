// cb-unity: alone
#include "formats/ngp.hpp"
#include "knowledge/package.hpp"

#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::ngp {

namespace {

constexpr std::string_view kReader = "ngp";

bool has_licence(ByteView d) {
    if (d.size() < kHeaderSize) return false;
    const std::string_view text = as_text(d.first(28));
    return text == "COPYRIGHT BY SNK CORPORATION" || text == " LICENSED BY SNK CORPORATION";
}

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    // config/knowledge/ngp/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("ngp", "hardware");
    return table;
}

Rom parse(ByteView d) {
    const engine::Tree tree = engine::read_builtin(kReader, d);   // config/formats/ngp.fmt
    const engine::Node& n = tree.root.get("header");
    Rom r;
    r.data.assign(d.begin(), d.end());
    Header& h = r.header;
    h.licensed = n.u("licensed") != 0;
    h.start = static_cast<std::uint32_t>(n.u("start"));
    h.catalog = static_cast<std::uint16_t>(n.u("catalog"));
    h.sub_catalog = static_cast<std::uint8_t>(n.u("sub_catalog"));
    h.colour = n.u("colour") != 0;
    std::string title = n.get("title").text;
    while (!title.empty() && (title.back() == ' ' || title.back() == '\0')) title.pop_back();
    h.title = title;
    return r;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".ngp", ".ngc", ".npc"})) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, kHeaderSize);
    return head && has_licence(*head);
}

}  // namespace cb::formats::ngp
