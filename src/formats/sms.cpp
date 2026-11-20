#include "formats/sms.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/text.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::sms {

namespace {

constexpr std::string_view kReader = "sms";
constexpr std::size_t kCopierHeader = 512;
constexpr std::size_t kSlotsSize = 0xC000;   // pages 0-2; $C000 up is RAM

struct Region {
    System system;
    std::string_view name;
};

// The region nibble of the header's last byte.
std::optional<Region> region_of(unsigned code) {
    switch (code) {
        case 3: return Region{System::MasterSystem, "Japan"};
        case 4: return Region{System::MasterSystem, "export"};
        case 5: return Region{System::GameGear, "Japan"};
        case 6: return Region{System::GameGear, "export"};
        case 7: return Region{System::GameGear, "international"};
        default: return std::nullopt;
    }
}

// The ROM size nibble.
std::optional<std::uint32_t> size_of_code(unsigned code) {
    switch (code) {
        case 0xA: return 0x2000;
        case 0xB: return 0x4000;
        case 0xC: return 0x8000;
        case 0xD: return 0xC000;
        case 0xE: return 0x10000;
        case 0xF: return 0x20000;
        case 0x0: return 0x40000;
        case 0x1: return 0x80000;
        case 0x2: return 0x100000;
        default: return std::nullopt;
    }
}

// The header the description found ($7FF0, $3FF0 or $1FF0: config/formats/sms.fmt), if any.
std::optional<Header> find_header(const Bytes& data) {
    const engine::Tree tree = engine::read_builtin(kReader, data);
    const engine::Node* n = tree.root.find("header");
    if (n == nullptr) return std::nullopt;
    Header out;
    out.at = static_cast<std::uint32_t>(n->offset);
    out.checksum = static_cast<std::uint16_t>(n->u("checksum"));
    // The product code: BCD, low byte first, its top digit in the high nibble of byte 14.
    const std::string code = std::format("{:X}{:02X}{:02X}", n->u("product2") >> 4, n->u("product1"), n->u("product0"));
    const std::size_t first = code.find_first_not_of('0');
    out.product = first == std::string::npos ? "0" : code.substr(first);
    out.version = static_cast<std::uint8_t>(n->u("version"));
    const std::optional<Region> r = region_of(static_cast<unsigned>(n->u("region")));
    out.region = r ? std::string(r->name) : "?";
    out.size = size_of_code(static_cast<unsigned>(n->u("size_code")));
    return out;
}

}  // namespace

std::string_view to_string(System s) {
    switch (s) {
        case System::MasterSystem: return "Master System";
        case System::GameGear: return "Game Gear";
        case System::Sg1000: return "SG-1000";
    }
    return "Master System";
}

ByteView Rom::page(std::size_t index) const {
    CB_ASSERT(index < page_count(), "SMS page index out of range");
    return ByteView(data).subspan(index * kPage, std::min(kPage, data.size() - index * kPage));
}

Bytes Rom::memory() const {
    Bytes mem(0x10000, 0);
    const std::size_t n = std::min(data.size(), kSlotsSize);
    std::copy_n(data.begin(), n, mem.begin());
    return mem;
}

std::optional<std::uint32_t> Rom::mapped(std::uint32_t address) const {
    if (address >= 0xC000 || address >= data.size()) return std::nullopt;
    return address;
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    const Bytes mem = memory();
    const std::uint32_t limit = static_cast<std::uint32_t>(std::min(data.size(), kSlotsSize));
    std::set<std::uint32_t> starts{0};
    // An interrupt vector the program does not use is often filled with $FF (RST $38).
    for (std::uint32_t v : {kResetVector, kIrqVector, kNmiVector}) {
        if (v < limit && mem[v] != 0xFF) starts.insert(v);
    }
    return find_functions(mem, std::move(starts), decode, {.begin = 0, .end = limit});
}

Rom parse(ByteView data, std::string_view extension) {
    if (data.size() % kPage == kCopierHeader) data = data.subspan(kCopierHeader);   // a copier's header in front
    Rom rom;
    rom.data.assign(data.begin(), data.end());
    rom.header = find_header(rom.data);
    if (rom.header) {
        const std::optional<Region> r = region_of(rom.data[rom.header->at + 15] >> 4);
        rom.system = r ? r->system : System::MasterSystem;
    } else if (text::iequals(extension, ".gg")) {
        rom.system = System::GameGear;
    } else if (text::iequals(extension, ".sg") || text::iequals(extension, ".sc")) {
        rom.system = System::Sg1000;
    }
    rom.has_mapper = rom.system != System::Sg1000 && rom.data.size() > kSlotsSize;
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view(), extension_of(path));
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".sms", ".gg", ".sg", ".sc"})) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    return size && *size >= 0x2000;
}

}  // namespace cb::formats::sms
