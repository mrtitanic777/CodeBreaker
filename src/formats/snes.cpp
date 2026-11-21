#include "formats/snes.hpp"
#include "knowledge/package.hpp"

#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::snes {

namespace {

constexpr std::string_view kReader = "snes";

std::size_t copier_size_of(std::uint64_t size) { return size % kLoRomBank == kCopierHeaderSize && size > kCopierHeaderSize ? kCopierHeaderSize : 0; }

Vectors vectors_of(const engine::Node& v) {
    auto at = [&](std::string_view name) { return static_cast<std::uint16_t>(v.u(name)); };
    return {at("cop"), at("brk"), at("abort"), at("nmi"), at("reset"), at("irq")};
}

}  // namespace

std::string_view to_string(Mapping m) { return m == Mapping::LoRom ? "LoROM" : "HiROM"; }

std::optional<std::uint32_t> Rom::offset(std::uint32_t address) const {
    if (data.empty()) return std::nullopt;
    const std::uint32_t bank = address >> 16 & 0xFF, off = address & 0xFFFF;
    if (bank == 0x7E || bank == 0x7F) return std::nullopt;   // the console's RAM
    std::uint64_t at = 0;
    if (mapping == Mapping::LoRom) {
        if (off < 0x8000) return std::nullopt;
        at = static_cast<std::uint64_t>(bank & 0x7F) * kLoRomBank + (off & 0x7FFF);
    } else {
        const bool full = (bank >= 0x40 && bank < 0x7E) || bank >= 0xC0;
        if (!full && off < 0x8000) return std::nullopt;
        at = static_cast<std::uint64_t>(bank & 0x3F) << 16 | off;
    }
    return static_cast<std::uint32_t>(at % data.size());
}

std::uint32_t Rom::address_of(std::uint32_t offset, bool high) const {
    if (mapping == Mapping::LoRom) return ((offset / kLoRomBank | (high ? 0x80u : 0u)) << 16) | 0x8000u | (offset & 0x7FFF);
    return (0xC0u | (offset >> 16)) << 16 | (offset & 0xFFFF);
}

Rom parse(ByteView file) {
    const std::size_t copier = copier_size_of(file.size());
    Rom rom;
    rom.copier_header.assign(file.begin(), file.begin() + static_cast<std::ptrdiff_t>(copier));
    rom.data.assign(file.begin() + static_cast<std::ptrdiff_t>(copier), file.end());
    // config/formats/snes.fmt scores the two places the internal header can be and says which one wins (or fails).
    const engine::Tree tree = engine::read_builtin(kReader, rom.data);
    const bool hirom = tree.root.u("is_hirom") != 0;
    rom.mapping = hirom ? Mapping::HiRom : Mapping::LoRom;
    rom.header_offset = static_cast<std::uint32_t>(tree.root.u("header_offset"));
    const engine::Node& h = tree.root.get(hirom ? "hi" : "lo");
    rom.title = h.get("title").text;
    while (!rom.title.empty() && (rom.title.back() == ' ' || rom.title.back() == '\0')) rom.title.pop_back();
    rom.map_mode = static_cast<std::uint8_t>(h.u("map_mode"));
    rom.chipset = static_cast<std::uint8_t>(h.u("chipset"));
    rom.rom_size_code = static_cast<std::uint8_t>(h.u("rom_size_code"));
    rom.ram_size_code = static_cast<std::uint8_t>(h.u("ram_size_code"));
    rom.country = static_cast<std::uint8_t>(h.u("country"));
    rom.developer = static_cast<std::uint8_t>(h.u("developer"));
    rom.version = static_cast<std::uint8_t>(h.u("version"));
    rom.complement = static_cast<std::uint16_t>(h.u("complement"));
    rom.checksum = static_cast<std::uint16_t>(h.u("checksum"));
    rom.checksum_ok = static_cast<std::uint16_t>(rom.complement + rom.checksum) == 0xFFFF;
    rom.native = vectors_of(h.get("native"));
    rom.emulation = vectors_of(h.get("emulation"));
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".sfc", ".smc", ".swc", ".fig"})) return false;
    try {
        const fs::MappedFile file(path);
        (void)parse(file.view());   // a header the description scores well enough
        return true;
    } catch (const Error&) {
        return false;
    }
}

const std::map<std::uint32_t, std::string>& registers() {
    // config/knowledge/snes/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("snes", "registers");
    return table;
}

std::string register_name(std::uint32_t address) {
    const auto it = registers().find(address & 0xFFFF);
    return it == registers().end() ? std::string() : it->second;
}

}  // namespace cb::formats::snes
