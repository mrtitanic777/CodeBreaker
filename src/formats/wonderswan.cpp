// cb-unity: alone
#include "formats/wonderswan.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/probe.hpp"

namespace cb::formats::wonderswan {

namespace {

constexpr std::string_view kReader = "wonderswan";

bool valid_size(std::uint64_t size) { return size >= 0x10000 && size <= 0x1000000 && (size & (size - 1)) == 0; }

// The footer's far jump: EA, then the offset and the segment.
bool has_far_jump(ByteView footer) { return footer.size() >= kFooter && footer[0] == 0xEA; }

}  // namespace

Bytes Rom::memory() const {
    Bytes mem(kEnd, 0);
    std::copy(data.end() - mapped(), data.end(), mem.begin() + base());
    return mem;
}

std::optional<std::uint32_t> Rom::offset(std::uint32_t address) const {
    if (address < base() || address >= kEnd) return std::nullopt;
    return static_cast<std::uint32_t>(data.size() - mapped()) + (address - base());
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    std::set<std::uint32_t> starts;
    if (offset(entry())) starts.insert(entry());
    const std::uint32_t lo = std::max(code_base(), base());
    const std::uint32_t hi = static_cast<std::uint32_t>(std::min<std::uint64_t>(static_cast<std::uint64_t>(code_base()) + 0x10000, kEnd));
    return find_functions(memory(), std::move(starts), decode, {.begin = lo, .end = hi});
}

Rom parse(ByteView data) {
    if (!valid_size(data.size())) throw FormatError(kReader, 0, std::format("{} bytes is not a WonderSwan ROM (a power of two, 64 KB to 16 MB)", data.size()));
    const ByteView footer = data.last(kFooter);
    if (!has_far_jump(footer)) throw FormatError(kReader, data.size() - kFooter, "the footer does not start with the far jump (EA)");
    Rom r;
    r.data.assign(data.begin(), data.end());
    r.reset_offset = static_cast<std::uint16_t>(footer[1] | footer[2] << 8);
    r.reset_segment = static_cast<std::uint16_t>(footer[3] | footer[4] << 8);
    r.maintenance = footer[5];
    r.developer = footer[6];
    r.color = footer[7];
    r.game_id = footer[8];
    r.version = footer[9];
    r.size_code = footer[10];
    r.save = footer[11];
    r.flags = footer[12];
    r.rtc = footer[13];
    r.checksum = static_cast<std::uint16_t>(footer[14] | footer[15] << 8);
    std::uint32_t sum = 0;
    for (std::size_t i = 0; i + 2 < data.size(); ++i) sum += data[i];
    r.checksum_ok = (sum & 0xFFFF) == r.checksum;
    if (!r.offset(r.entry())) throw FormatError(kReader, data.size() - kFooter + 1, std::format("the start address {:04X}:{:04X} is not in the ROM", r.reset_segment, r.reset_offset));
    return r;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".ws", ".wsc"})) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size || !valid_size(*size)) return false;
    const std::optional<Bytes> footer = read_prefix(path, *size - kFooter, kFooter);
    return footer && has_far_jump(*footer);
}

}  // namespace cb::formats::wonderswan
