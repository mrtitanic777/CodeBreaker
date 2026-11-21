// cb-unity: alone
#include "formats/o2.hpp"
#include "knowledge/package.hpp"

#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/probe.hpp"

namespace cb::formats::o2 {

namespace {

constexpr std::string_view kReader = "o2";

bool size_ok(std::uint64_t size) { return size >= 256 && size <= kMaxSize && size % 256 == 0; }

}  // namespace

const std::map<std::uint32_t, std::string>& vdc() {
    // config/knowledge/o2/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("o2", "vdc");
    return table;
}

Rom parse(ByteView d) {
    if (!size_ok(d.size())) throw FormatError(kReader, 0, std::format("{} bytes is not an Odyssey 2 cartridge (whole 256-byte pages up to 3 KB)", d.size()));
    Rom r;
    r.data.assign(d.begin(), d.end());
    return r;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".o2"})) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    return size && size_ok(*size);
}

}  // namespace cb::formats::o2
