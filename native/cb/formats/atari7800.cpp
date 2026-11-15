#include "cb/formats/atari7800.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::atari7800 {

namespace {

constexpr std::string_view kReader = "atari7800";
constexpr char kMagic[] = "ATARI7800";
constexpr std::size_t kMagicAt = 1;

bool has_magic(ByteView data) { return data.size() >= kHeaderSize && std::memcmp(data.data() + kMagicAt, kMagic, sizeof(kMagic) - 1) == 0; }

std::uint16_t word_of(const Bytes& mem, std::uint32_t at) { return static_cast<std::uint16_t>(mem[at] | mem[at + 1] << 8); }

}  // namespace

Bytes Rom::memory(std::size_t bank) const {
    Bytes mem(0x10000, 0);
    if (!supergame) {
        std::copy(data.begin(), data.end(), mem.begin() + base);
        return mem;
    }
    const std::size_t fixed = fixed_bank();
    const ByteView low = ByteView(data).subspan(std::min(bank, fixed) * kBankSize, kBankSize);
    std::copy(low.begin(), low.end(), mem.begin() + 0x8000);
    const ByteView high = ByteView(data).subspan(fixed * kBankSize, kBankSize);
    std::copy(high.begin(), high.end(), mem.begin() + 0xC000);
    return mem;
}

std::optional<std::uint32_t> Rom::offset(std::size_t bank, std::uint32_t address) const {
    if (address > 0xFFFF) return std::nullopt;
    if (!supergame) {
        if (address < base) return std::nullopt;
        return address - base;
    }
    if (address >= 0xC000) return static_cast<std::uint32_t>(fixed_bank() * kBankSize + (address - 0xC000));
    if (address >= 0x8000) return static_cast<std::uint32_t>(std::min(bank, fixed_bank()) * kBankSize + (address - 0x8000));
    return std::nullopt;
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    const Bytes mem = memory(0);
    std::set<std::uint32_t> starts;
    for (const std::uint32_t v : {std::uint32_t{vectors.nmi}, std::uint32_t{vectors.reset}, std::uint32_t{vectors.irq}}) {
        if (offset(0, v)) starts.insert(v);
    }
    const std::uint32_t begin = supergame ? 0x8000 : base;
    std::vector<std::uint32_t> all = find_functions(mem, std::move(starts), decode, {.begin = begin, .end = 0x10000});
    const std::uint32_t fixed = static_cast<std::uint32_t>(fixed_bank()) << 16;
    for (std::uint32_t& a : all) a = (supergame && a >= 0xC000 ? fixed : 0) | a;   // (the fixed bank's code is that bank's)
    return all;
}

Rom parse(ByteView data) {
    if (!has_magic(data)) throw FormatError(kReader, 0, "not an A78 header (no ATARI7800 at byte 1)");
    Rom rom;
    rom.header.assign(data.begin(), data.begin() + kHeaderSize);
    rom.version = data[0];
    rom.title.assign(reinterpret_cast<const char*>(data.data()) + 17, 32);
    while (!rom.title.empty() && (rom.title.back() == '\0' || rom.title.back() == ' ')) rom.title.pop_back();
    rom.declared_size = static_cast<std::uint32_t>(data[49] << 24 | data[50] << 16 | data[51] << 8 | data[52]);
    rom.cart_type = static_cast<std::uint16_t>(data[53] << 8 | data[54]);
    rom.controller1 = data[55];
    rom.controller2 = data[56];
    rom.tv = data[57];
    rom.save = data[58];
    rom.data.assign(data.begin() + kHeaderSize, data.end());
    const std::size_t size = rom.data.size();
    if (rom.declared_size > size) throw FormatError(kReader, kHeaderSize, std::format("the ROM is cut short ({} of {} bytes)", size, rom.declared_size));
    rom.supergame = (rom.cart_type & kTypeSuperGame) != 0 && size > 3 * kBankSize;
    if (rom.supergame) {
        if (size % kBankSize != 0 || size > 8 * kBankSize) throw FormatError(kReader, kHeaderSize, std::format("{} bytes is not 16 KB banks of a SuperGame board", size));
    } else {
        if (size < 0x1000 || size > 3 * kBankSize || size % 0x1000 != 0) {
            throw FormatError(kReader, kHeaderSize, std::format("{} bytes is not a ROM of 4 KB to 48 KB, or a SuperGame board's", size));
        }
        rom.base = static_cast<std::uint32_t>(0x10000 - size);
    }
    const Bytes mem = rom.memory(0);
    rom.vectors = {word_of(mem, 0xFFFA), word_of(mem, 0xFFFC), word_of(mem, 0xFFFE)};
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, kHeaderSize);
    return head && has_magic(*head);
}

}  // namespace cb::formats::atari7800
