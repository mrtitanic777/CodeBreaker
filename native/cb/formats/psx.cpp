#include "cb/formats/psx.hpp"

#include <algorithm>
#include <cstring>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::psx {

namespace {

constexpr std::string_view kReader = "psx";
constexpr std::size_t kMarkerAt = 0x4C;

bool has_magic(ByteView data) { return data.size() >= kMagic.size() && std::memcmp(data.data(), kMagic.data(), kMagic.size()) == 0; }

}  // namespace

Bytes Exe::memory() const {
    Bytes ram(kRamSize, 0);
    const ByteView program = text();
    const std::size_t at = header.t_addr & kRamMask;
    if (at < ram.size()) std::copy_n(program.begin(), std::min(program.size(), ram.size() - at), ram.begin() + static_cast<std::ptrdiff_t>(at));
    return ram;
}

std::vector<std::uint32_t> Exe::functions(const Decoder& decode) const {
    const Bytes ram = memory();
    const std::uint32_t begin = header.t_addr & kRamMask;
    const std::uint32_t end = std::min<std::uint32_t>(begin + static_cast<std::uint32_t>(text().size()), kRamSize);
    std::set<std::uint32_t> starts;
    const std::uint32_t entry = header.pc0 & kRamMask;
    if (entry >= begin && entry < end && entry % 4 == 0) starts.insert(entry);
    const std::vector<std::uint32_t> found = find_functions(ram, std::move(starts), decode, {.begin = begin, .end = end});
    std::vector<std::uint32_t> out;
    for (const std::uint32_t a : found)
        if (a >= begin && a < end && a % 4 == 0) out.push_back(a | region());
    return out;
}

Exe parse(ByteView data) {
    if (!has_magic(data)) throw FormatError(kReader, 0, "not a PS-X EXE (no \"PS-X EXE\" at the start)");
    if (data.size() < kHeaderSize) throw FormatError(kReader, 0, "shorter than the 2048-byte header");
    Exe exe;
    exe.data.assign(data.begin(), data.end());
    io::ByteReader r(exe.data, kReader);
    r.seek(0x10);
    Header& h = exe.header;
    h.pc0 = r.u32();
    h.gp0 = r.u32();
    h.t_addr = r.u32();
    h.t_size = r.u32();
    h.data_addr = r.u32();
    h.data_size = r.u32();
    h.bss_addr = r.u32();
    h.bss_size = r.u32();
    h.sp_base = r.u32();
    h.sp_offset = r.u32();
    std::size_t n = kMarkerAt;
    while (n < kHeaderSize && exe.data[n] != 0) ++n;
    h.marker.assign(reinterpret_cast<const char*>(exe.data.data()) + kMarkerAt, n - kMarkerAt);
    return exe;
}

Exe read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, kMagic.size());
    return head && has_magic(*head);
}

}  // namespace cb::formats::psx
