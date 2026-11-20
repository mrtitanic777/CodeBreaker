#include "formats/genesis.hpp"

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
#include "formats/sega32x.hpp"
#include "formats/segacd.hpp"

namespace cb::formats::genesis {

namespace {

constexpr std::string_view kReader = "genesis";
constexpr std::size_t kSmdHeader = 512;
constexpr std::size_t kSmdBlock = 0x4000;
constexpr std::size_t kMemorySize = 0x1000000;
constexpr std::uint32_t kFirstProgramAddress = 0x200;

bool is_sega(ByteView data, std::size_t at) {
    return data.size() >= at + 4 && std::memcmp(data.data() + at, "SEGA", 4) == 0;
}

std::string field(const engine::Node& header, std::string_view name) {
    return std::string(text::trim(text::from_latin1(as_bytes(header.get(name).text))));
}

std::uint32_t word(const engine::Node& header, std::string_view name) { return static_cast<std::uint32_t>(header.u(name)); }

}  // namespace

// The vector numbers (the long at 4 * n) that name a handler; the rest are reserved or unused.
std::string vector_name(unsigned n) {
    switch (n) {
        case 2: return "bus_error";
        case 3: return "address_error";
        case 4: return "illegal_instruction";
        case 5: return "zero_divide";
        case 6: return "chk";
        case 7: return "trapv";
        case 8: return "privilege";
        case 9: return "trace";
        case 10: return "line_a";
        case 11: return "line_f";
        case 24: return "spurious";
        case 25: return "level1";
        case 26: return "external";
        case 27: return "level3";
        case 28: return "hblank";
        case 29: return "level5";
        case 30: return "vblank";
        case 31: return "level7";
        default: break;
    }
    if (n >= 32 && n < 48) return std::format("trap{}", n - 32);
    return {};
}

Bytes deinterleave_smd(ByteView data) {
    if (data.size() < kSmdHeader) throw FormatError(kReader, 0, "too short for an .smd header");
    const ByteView body = data.subspan(kSmdHeader);
    Bytes out(body.size());
    for (std::size_t b = 0; b < body.size(); b += kSmdBlock) {
        const ByteView block = body.subspan(b, std::min(kSmdBlock, body.size() - b));
        const std::size_t half = block.size() / 2;
        for (std::size_t i = 0; i < block.size(); ++i) {
            // The first half of the block holds the odd bytes of the image, the second half the even ones.
            if (i < half) {
                out[b + 2 * i + 1] = block[i];
            } else {
                out[b + 2 * (i - half)] = block[i];
            }
        }
    }
    return out;
}

Bytes Rom::memory() const {
    Bytes mem(kMemorySize, 0);
    std::copy_n(data.begin(), std::min(data.size(), kRomWindow), mem.begin());
    return mem;
}

std::uint16_t Rom::computed_checksum() const {
    std::uint32_t sum = 0;
    std::size_t at = kFirstProgramAddress;
    for (; at + 1 < data.size(); at += 2) sum += static_cast<std::uint32_t>(data[at] << 8 | data[at + 1]);
    if (at < data.size()) sum += static_cast<std::uint32_t>(data[at] << 8);   // an odd final byte pairs with a zero
    return static_cast<std::uint16_t>(sum & 0xFFFF);
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    const std::uint32_t limit = static_cast<std::uint32_t>(data.size());
    std::set<std::uint32_t> starts;
    auto add = [&](std::uint32_t v) {
        if (v < limit && v % 2 == 0) starts.insert(v);
    };
    add(reset);
    for (const Vector& v : vectors) add(v.address);
    return find_functions(data, std::move(starts), decode, {.begin = 0, .end = limit, .even_targets_only = true});
}

Rom parse(ByteView data, std::string_view extension) {
    Rom rom;
    const bool smd_size = data.size() % kSmdBlock == kSmdHeader;
    rom.smd = smd_size && ((!is_sega(data, kHeaderAt) && data.size() > 9 && data[8] == 0xAA && data[9] == 0xBB) ||
                           text::iequals(extension, ".smd"));
    if (rom.smd) rom.data = deinterleave_smd(data);
    else rom.data.assign(data.begin(), data.end());

    const engine::Tree tree = engine::read_builtin(kReader, rom.data);   // config/formats/genesis.fmt
    const engine::Node& h = tree.root.get("header");
    rom.header = {
        .system = field(h, "system"), .copyright = field(h, "copyright"), .domestic_name = field(h, "domestic_name"),
        .overseas_name = field(h, "overseas_name"), .serial = field(h, "serial"),
        .checksum = static_cast<std::uint16_t>(h.u("checksum")), .devices = field(h, "devices"), .rom_start = word(h, "rom_start"), .rom_end = word(h, "rom_end"),
        .ram_start = word(h, "ram_start"), .ram_end = word(h, "ram_end"), .region = field(h, "region")};

    const engine::Node& longs = tree.root.get("vectors");
    rom.initial_ssp = static_cast<std::uint32_t>(longs.children[0].value);
    rom.reset = static_cast<std::uint32_t>(longs.children[1].value);
    for (unsigned n = 0; n < longs.children.size(); ++n) {
        std::string name = vector_name(n);
        const std::uint32_t v = static_cast<std::uint32_t>(longs.children[n].value);
        if (!name.empty() && v < rom.data.size() && v % 2 == 0 && v >= kFirstProgramAddress) {
            rom.vectors.push_back({std::move(name), v});
        }
    }
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view(), extension_of(path));
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, 0x400);
    if (!head) return false;
    if (segacd::is_disc_start(*head)) return false;   // a Sega CD disc carries the same header at $100, after its own ID
    if (sega32x::is_rom_start(*head, has_extension(path, {".32x"}))) return false;   // a 32X cartridge is a Genesis ROM with the add-on's header: its own lane
    if (is_sega(*head, 0x100) || is_sega(*head, 0x101)) return true;
    return has_extension(path, {".md", ".gen", ".smd"}) && head->size() >= 0x200;
}

}  // namespace cb::formats::genesis
