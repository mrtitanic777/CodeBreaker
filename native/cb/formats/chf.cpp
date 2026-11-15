// cb-unity: alone
#include "cb/formats/chf.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::chf {

namespace {

constexpr std::string_view kReader = "chf";

bool size_ok(std::uint64_t size) { return size >= 256 && size <= kMaxSize && size % 256 == 0; }

}  // namespace

const std::map<std::uint32_t, std::string>& bios() {
    static const std::map<std::uint32_t, std::string> table = {{0x008F, "BIOS_DELAY"}, {0x00D0, "BIOS_CLEAR_SCREEN"}, {0x0107, "BIOS_PUSHK"}, {0x011E, "BIOS_POPK"}, {0x0679, "BIOS_DRAWCHAR"}};
    return table;
}

Rom parse(ByteView d) {
    if (!size_ok(d.size())) throw FormatError(kReader, 0, std::format("{} bytes is not a Channel F cartridge (whole 256-byte pages up to 62 KB)", d.size()));
    Rom r;
    r.data.assign(d.begin(), d.end());
    return r;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size || !size_ok(*size)) return false;
    if (has_extension(path, {".chf"})) return true;
    if (!has_extension(path, {".bin"})) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, 2);
    return head && head->size() == 2 && (*head)[0] == 0x55 && (*head)[1] == 0x2B;
}

}  // namespace cb::formats::chf
