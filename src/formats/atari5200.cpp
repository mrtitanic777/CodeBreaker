// cb-unity: alone
#include "formats/atari5200.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/probe.hpp"

namespace cb::formats::atari5200 {

namespace {

constexpr std::string_view kReader = "atari5200";

bool valid_size(std::uint64_t size) { return size == 0x1000 || size == 0x2000 || size == 0x4000 || size == 0x8000; }

// The start address in the last two bytes of a ROM of `size` bytes, and whether it lies in the window the ROM fills.
bool start_inside(std::uint64_t size, std::uint16_t start) { return start >= 0xC000 - size && start < 0xC000; }

}  // namespace

Bytes Rom::memory() const {
    Bytes mem(0x10000, 0);
    std::copy(data.begin(), data.end(), mem.begin() + base);
    return mem;
}

std::optional<std::uint32_t> Rom::offset(std::uint32_t address) const {
    if (address < base || address >= base + data.size()) return std::nullopt;
    return address - base;
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    std::set<std::uint32_t> starts;
    if (offset(start)) starts.insert(start);
    return find_functions(memory(), std::move(starts), decode, {.begin = base, .end = base + static_cast<std::uint32_t>(data.size())});
}

Rom parse(ByteView data) {
    if (!valid_size(data.size())) throw FormatError(kReader, 0, std::format("{} bytes is not an Atari 5200 ROM (4, 8, 16 or 32 KB)", data.size()));
    Rom rom;
    rom.data.assign(data.begin(), data.end());
    rom.base = static_cast<std::uint32_t>(0xC000 - data.size());
    const std::size_t end = data.size();
    rom.flag_a = data[end - 4];
    rom.flag_b = data[end - 3];
    rom.start = static_cast<std::uint16_t>(data[end - 2] | data[end - 1] << 8);
    if (!start_inside(data.size(), rom.start)) throw FormatError(kReader, end - 2, std::format("the start address ${:04X} is outside the ROM", rom.start));
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const bool a52 = has_extension(path, {".a52"});
    if (!a52 && !has_extension(path, {".bin", ".rom"})) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size || !valid_size(*size)) return false;
    if (a52) return true;
    const std::optional<Bytes> tail = read_prefix(path, *size - 2, 2);
    if (!tail || tail->size() != 2) return false;
    const std::uint16_t start = static_cast<std::uint16_t>((*tail)[0] | (*tail)[1] << 8);
    if (!start_inside(*size, start)) return false;
    // a raw dump is not told by its extension: the start address must hold something that is not erased ROM
    const std::optional<Bytes> first = read_prefix(path, start - (0xC000 - *size), 1);
    return first && first->size() == 1 && (*first)[0] != 0x00 && (*first)[0] != 0xFF;
}

}  // namespace cb::formats::atari5200
