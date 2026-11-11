#include "cb/formats/gb.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::gb {

const std::array<std::uint8_t, 48> kLogo = {
    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
    0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
    0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E};

namespace {

constexpr std::string_view kReader = "gb";
constexpr std::size_t kLogoAt = 0x104;
constexpr std::size_t kTitleAt = 0x134;
constexpr std::size_t kMinProbeSize = 0x8000;
constexpr std::size_t kPowerOnRom = 0x8000;   // banks 0 and 1

std::string_view mbc_name(std::uint8_t type) {
    switch (type) {
        case 0x00: return "ROM";
        case 0x01: return "MBC1";
        case 0x02: return "MBC1+RAM";
        case 0x03: return "MBC1+RAM+BATTERY";
        case 0x05: return "MBC2";
        case 0x06: return "MBC2+BATTERY";
        case 0x08: return "ROM+RAM";
        case 0x09: return "ROM+RAM+BATTERY";
        case 0x0F: return "MBC3+TIMER+BATTERY";
        case 0x10: return "MBC3+TIMER+RAM+BATTERY";
        case 0x11: return "MBC3";
        case 0x12: return "MBC3+RAM";
        case 0x13: return "MBC3+RAM+BATTERY";
        case 0x19: return "MBC5";
        case 0x1A: return "MBC5+RAM";
        case 0x1B: return "MBC5+RAM+BATTERY";
        case 0x1C: return "MBC5+RUMBLE";
        case 0x1D: return "MBC5+RUMBLE+RAM";
        case 0x1E: return "MBC5+RUMBLE+RAM+BATTERY";
        case 0x20: return "MBC6";
        case 0x22: return "MBC7";
        case 0xFC: return "POCKET CAMERA";
        case 0xFE: return "HuC3";
        case 0xFF: return "HuC1";
        default: return {};
    }
}

bool logo_at(ByteView data) {
    return data.size() >= kLogoAt + kLogo.size() && std::equal(kLogo.begin(), kLogo.end(), data.begin() + kLogoAt);
}

}  // namespace

std::string Rom::mbc() const {
    const std::string_view name = mbc_name(cartridge_type);
    return name.empty() ? std::format("type ${:02X}", cartridge_type) : std::string(name);
}

ByteView Rom::bank(std::size_t index) const {
    CB_ASSERT(index < bank_count(), "Game Boy bank index out of range");
    return ByteView(data).subspan(index * kBank, std::min(kBank, data.size() - index * kBank));
}

Bytes Rom::memory() const {
    Bytes mem(0x10000, 0);
    std::copy_n(data.begin(), std::min(data.size(), kPowerOnRom), mem.begin());
    return mem;
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    const Bytes mem = memory();
    const std::uint32_t limit = static_cast<std::uint32_t>(std::min(data.size(), kPowerOnRom));
    std::set<std::uint32_t> starts{kEntry};
    for (const InterruptVector& v : kInterruptVectors) {
        if (mem[v.address] != 0xFF && mem[v.address] != 0x00 && decode(mem, v.address)) starts.insert(v.address);
    }
    // The header ($0104-$014F) is data.
    return find_functions(mem, std::move(starts), decode,
                          {.begin = 0, .end = limit, .data_begin = static_cast<std::uint32_t>(kLogoAt),
                           .data_end = static_cast<std::uint32_t>(kHeaderEnd)});
}

Rom parse(ByteView data) {
    if (data.size() < kHeaderEnd) throw FormatError(kReader, 0, "too short for a Game Boy cartridge");
    Rom rom;
    rom.data.assign(data.begin(), data.end());
    const Bytes& h = rom.data;
    rom.logo_ok = logo_at(h);
    switch (h[0x143]) {
        case 0x80: rom.color = ColorSupport::Also; break;
        case 0xC0: rom.color = ColorSupport::Only; break;
        default: rom.color = ColorSupport::None; break;
    }
    // The title field is 16 bytes at $0134; a Color cartridge takes its last bytes for the manufacturer code and
    // the flag, which leaves 11.
    const std::size_t title_end = rom.color == ColorSupport::None ? 0x143 : 0x13F;
    ByteView field = ByteView(h).subspan(kTitleAt, title_end - kTitleAt);
    const auto nul = std::find(field.begin(), field.end(), std::uint8_t{0});
    rom.title = std::string(text::trim(text::from_latin1(field.first(static_cast<std::size_t>(nul - field.begin())))));
    rom.sgb = h[0x146] == 0x03;
    rom.cartridge_type = h[0x147];
    rom.header_checksum = h[0x14D];
    std::uint8_t x = 0;
    for (std::size_t i = kTitleAt; i < 0x14D; ++i) x = static_cast<std::uint8_t>(x - h[i] - 1);
    rom.header_checksum_ok = x == h[0x14D];
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".gb", ".gbc"})) {
        const std::optional<Bytes> head = read_prefix(path, kLogoAt, kLogo.size());
        return head && head->size() == kLogo.size() && std::equal(kLogo.begin(), kLogo.end(), head->begin());
    }
    const std::optional<std::uint64_t> size = size_of(path);
    return size && *size >= kMinProbeSize;
}

}  // namespace cb::formats::gb
