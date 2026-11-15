#include "cb/formats/snes.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::snes {

namespace {

constexpr std::string_view kReader = "snes";
constexpr int kAccepted = 6;   // the lowest score a header place needs

std::size_t copier_size_of(std::uint64_t size) { return size % kLoRomBank == kCopierHeaderSize && size > kCopierHeaderSize ? kCopierHeaderSize : 0; }

std::uint16_t word_of(ByteView h, std::size_t at) { return static_cast<std::uint16_t>(h[at] | h[at + 1] << 8); }

// How well the 64 bytes at a header place read as an internal header for `mapping`.
int score(ByteView h, Mapping mapping) {
    int s = 0;
    if (static_cast<std::uint16_t>(word_of(h, 0x1C) + word_of(h, 0x1E)) == 0xFFFF) s += 4;
    const std::uint8_t map = h[0x15];
    if (((map & 1) != 0) == (mapping == Mapping::HiRom)) s += 2;
    if ((map & 0xE0) == 0x20) s += 1;
    int printable = 0;
    for (std::size_t i = 0; i < 21; ++i) printable += h[i] >= 0x20 && h[i] < 0x7F;
    if (printable == 21) s += 2;
    else if (printable >= 16) s += 1;
    const std::uint16_t reset = word_of(h, 0x3C);
    if (reset >= 0x8000 && reset != 0xFFFF) s += 2;
    if (h[0x17] <= 0x0D) s += 1;
    return s;
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
    if (file.size() < copier + kLoRomBank) throw FormatError(kReader, 0, std::format("{} bytes is smaller than a cartridge (32 KB)", file.size()));
    Rom rom;
    rom.copier_header.assign(file.begin(), file.begin() + static_cast<std::ptrdiff_t>(copier));
    rom.data.assign(file.begin() + static_cast<std::ptrdiff_t>(copier), file.end());
    int best = -1;
    for (const auto& [place, mapping] : {std::pair{kLoRomHeader, Mapping::LoRom}, std::pair{kHiRomHeader, Mapping::HiRom}}) {
        if (place + kInternalHeaderSize > rom.data.size()) continue;
        const int s = score(ByteView(rom.data).subspan(place, kInternalHeaderSize), mapping);
        if (s > best) {
            best = s;
            rom.mapping = mapping;
            rom.header_offset = place;
        }
    }
    if (best < kAccepted) throw FormatError(kReader, copier + kLoRomHeader, "no internal header: neither $7FC0 nor $FFC0 reads as one");
    const ByteView h = ByteView(rom.data).subspan(rom.header_offset, kInternalHeaderSize);
    rom.title.assign(reinterpret_cast<const char*>(h.data()), 21);
    while (!rom.title.empty() && (rom.title.back() == ' ' || rom.title.back() == '\0')) rom.title.pop_back();
    rom.map_mode = h[0x15];
    rom.chipset = h[0x16];
    rom.rom_size_code = h[0x17];
    rom.ram_size_code = h[0x18];
    rom.country = h[0x19];
    rom.developer = h[0x1A];
    rom.version = h[0x1B];
    rom.complement = word_of(h, 0x1C);
    rom.checksum = word_of(h, 0x1E);
    rom.checksum_ok = static_cast<std::uint16_t>(rom.complement + rom.checksum) == 0xFFFF;
    rom.native = {word_of(h, 0x24), word_of(h, 0x26), word_of(h, 0x28), word_of(h, 0x2A), word_of(h, 0x2C), word_of(h, 0x2E)};
    rom.emulation = {word_of(h, 0x34), word_of(h, 0x36), word_of(h, 0x38), word_of(h, 0x3A), word_of(h, 0x3C), word_of(h, 0x3E)};
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".sfc", ".smc", ".swc", ".fig"})) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size) return false;
    const std::size_t copier = copier_size_of(*size);
    if (*size < copier + kLoRomBank) return false;
    for (const auto& [place, mapping] : {std::pair{kLoRomHeader, Mapping::LoRom}, std::pair{kHiRomHeader, Mapping::HiRom}}) {
        if (copier + place + kInternalHeaderSize > *size) continue;
        const std::optional<Bytes> h = read_prefix(path, copier + place, kInternalHeaderSize);
        if (h && h->size() == kInternalHeaderSize && score(*h, mapping) >= kAccepted) return true;
    }
    return false;
}

const std::map<std::uint32_t, std::string>& registers() {
    static const std::map<std::uint32_t, std::string> table = [] {
        std::map<std::uint32_t, std::string> t;
        constexpr std::string_view kPpu[] = {
            "INIDISP", "OBSEL",   "OAMADDL", "OAMADDH", "OAMDATA", "BGMODE",  "MOSAIC",  "BG1SC",   "BG2SC",   "BG3SC",  "BG4SC",  "BG12NBA", "BG34NBA", "BG1HOFS", "BG1VOFS",
            "BG2HOFS", "BG2VOFS", "BG3HOFS", "BG3VOFS", "BG4HOFS", "BG4VOFS", "VMAIN",   "VMADDL",  "VMADDH",  "VMDATAL", "VMDATAH", "M7SEL",  "M7A",    "M7B",    "M7C",
            "M7D",     "M7X",     "M7Y",     "CGADD",   "CGDATA",  "W12SEL",  "W34SEL",  "WOBJSEL", "WH0",     "WH1",    "WH2",    "WH3",     "WBGLOG",  "WOBJLOG", "TM",
            "TS",      "TMW",     "TSW",     "CGWSEL",  "CGADSUB", "COLDATA", "SETINI",  "MPYL",    "MPYM",    "MPYH",   "SLHV",   "OAMDATAREAD", "VMDATALREAD", "VMDATAHREAD",
            "CGDATAREAD", "OPHCT", "OPVCT", "STAT77", "STAT78"};
        for (std::size_t i = 0; i < std::size(kPpu); ++i) t[static_cast<std::uint32_t>(0x2100 + i)] = std::string(kPpu[i]);
        for (unsigned i = 0; i < 4; ++i) t[0x2140 + i] = std::format("APUIO{}", i);
        t[0x2180] = "WMDATA";
        t[0x2181] = "WMADDL";
        t[0x2182] = "WMADDM";
        t[0x2183] = "WMADDH";
        t[0x4016] = "JOYSER0";
        t[0x4017] = "JOYSER1";
        constexpr std::string_view kCpu[] = {"NMITIMEN", "WRIO",   "WRMPYA", "WRMPYB", "WRDIVL", "WRDIVH", "WRDIVB", "HTIMEL", "HTIMEH", "VTIMEL", "VTIMEH", "MDMAEN", "HDMAEN", "MEMSEL", "", "",
                                             "RDNMI",    "TIMEUP", "HVBJOY", "RDIO",   "RDDIVL", "RDDIVH", "RDMPYL", "RDMPYH", "JOY1L",  "JOY1H",  "JOY2L",  "JOY2H",  "JOY3L",  "JOY3H",  "JOY4L", "JOY4H"};
        for (std::size_t i = 0; i < std::size(kCpu); ++i) {
            if (!kCpu[i].empty()) t[static_cast<std::uint32_t>(0x4200 + i)] = std::string(kCpu[i]);
        }
        for (unsigned ch = 0; ch < 8; ++ch) {
            const std::uint32_t base = 0x4300 + ch * 0x10;
            t[base + 0] = std::format("DMAP{}", ch);
            t[base + 1] = std::format("BBAD{}", ch);
            t[base + 2] = std::format("A1T{}L", ch);
            t[base + 3] = std::format("A1T{}H", ch);
            t[base + 4] = std::format("A1B{}", ch);
            t[base + 5] = std::format("DAS{}L", ch);
            t[base + 6] = std::format("DAS{}H", ch);
            t[base + 7] = std::format("DASB{}", ch);
            t[base + 8] = std::format("A2A{}L", ch);
            t[base + 9] = std::format("A2A{}H", ch);
            t[base + 10] = std::format("NTRL{}", ch);
        }
        return t;
    }();
    return table;
}

std::string register_name(std::uint32_t address) {
    const auto it = registers().find(address & 0xFFFF);
    return it == registers().end() ? std::string() : it->second;
}

}  // namespace cb::formats::snes
