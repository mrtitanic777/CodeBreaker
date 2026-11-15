#include "cb/formats/atari2600.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::atari2600 {

namespace {

constexpr std::string_view kReader = "atari2600";
constexpr std::size_t kMaxSize = 0x8000;

std::optional<Scheme> scheme_of_size(std::size_t size) {
    switch (size) {
        case 0x800:
        case 0x1000: return Scheme::None;
        case 0x2000: return Scheme::F8;
        case 0x4000: return Scheme::F6;
        case 0x8000: return Scheme::F4;
        default: return std::nullopt;
    }
}

std::uint16_t word_at(ByteView data, std::size_t at) { return static_cast<std::uint16_t>(data[at] | data[at + 1] << 8); }

// The opcodes a program's first instruction usually is (the reset code of a cartridge starts with SEI / CLD / LDX # and the like): a bare
// `.bin` is taken as a cartridge only if its reset points at one.
bool plausible_first_opcode(std::uint8_t op) {
    static constexpr std::uint8_t kLikely[] = {0x78, 0xD8, 0xA2, 0xA9, 0xA0, 0x18, 0x38, 0x4C, 0x20, 0xEA, 0x58, 0x8A, 0x98, 0xAA, 0xA8, 0x85, 0x86,
                                               0x84, 0x8D, 0x8E, 0x8C, 0xA5, 0xA6, 0xA4, 0xAD, 0xAE, 0xAC, 0x4A, 0x0A, 0x48, 0xC8, 0xE8, 0x88, 0xCA, 0x9A, 0xBA};
    return std::find(std::begin(kLikely), std::end(kLikely), op) != std::end(kLikely);
}

}  // namespace

std::string_view to_string(Scheme scheme) {
    switch (scheme) {
        case Scheme::None: return "none";
        case Scheme::F8: return "F8";
        case Scheme::F6: return "F6";
        case Scheme::F4: return "F4";
    }
    return "none";
}

ByteView Rom::bank(std::size_t index) const {
    CB_ASSERT(index < bank_count(), "Atari 2600 bank index out of range");
    return ByteView(data).subspan(index * bank_size, bank_size);
}

std::uint32_t Rom::first_hotspot() const {
    switch (scheme) {
        case Scheme::F8: return 0xFF8;
        case Scheme::F6: return 0xFF6;
        case Scheme::F4: return 0xFF4;
        case Scheme::None: break;
    }
    return 0;
}

std::optional<std::size_t> Rom::bank_of_hotspot(std::uint32_t address) const {
    if (scheme == Scheme::None || !(address & 0x1000)) return std::nullopt;
    const std::uint32_t low = address & 0xFFF;
    const std::uint32_t first = first_hotspot();
    if (low < first || low >= first + bank_count()) return std::nullopt;
    return low - first;
}

std::optional<std::uint32_t> Rom::fold(std::uint32_t address) const {
    if (!(address & 0x1000) || address > 0xFFFF) return std::nullopt;
    return window | (address & (bank_size - 1));
}

Bytes Rom::memory(std::size_t index) const {
    Bytes mem(0x10000, 0);
    const ByteView b = bank(index);
    for (std::uint32_t base = 0x1000; base < 0x10000; base += 0x1000) {
        for (std::uint32_t at = 0; at < 0x1000; at += bank_size) std::copy(b.begin(), b.end(), mem.begin() + base + at);
    }
    return mem;
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    std::set<std::uint32_t> all;
    for (std::size_t b = 0; b < bank_count(); ++b) {
        const Bytes mem = memory(b);
        std::set<std::uint32_t> starts;
        for (const std::uint32_t v : {std::uint32_t{vectors[b].reset}, std::uint32_t{vectors[b].irq}}) {
            if (const auto folded = fold(v)) starts.insert(*folded);
        }
        for (const std::uint32_t start : find_functions(mem, std::move(starts), decode, {.begin = window, .end = window + bank_size})) {
            all.insert(static_cast<std::uint32_t>(b) << 16 | start);
        }
    }
    return {all.begin(), all.end()};
}

Rom parse(ByteView data) {
    if (data.size() > kMaxSize) throw FormatError(kReader, 0, std::format("{} bytes: more than the largest cartridge handled (32 KB)", data.size()));
    const std::optional<Scheme> scheme = scheme_of_size(data.size());
    if (!scheme) throw FormatError(kReader, 0, std::format("{} bytes is not the size of a cartridge (2, 4, 8, 16 or 32 KB)", data.size()));
    Rom rom;
    rom.data.assign(data.begin(), data.end());
    rom.scheme = *scheme;
    rom.bank_size = data.size() == 0x800 ? 0x800 : 0x1000;
    for (std::size_t b = 0; b < rom.bank_count(); ++b) {
        const std::size_t end = (b + 1) * rom.bank_size;
        rom.vectors.push_back({word_at(data, end - 4), word_at(data, end - 2)});
    }
    const std::uint16_t reset = rom.vectors.back().reset;
    if (!(reset & 0x1000)) throw FormatError(kReader, rom.data.size() - 4, std::format("the reset vector {:04X} is not in the cartridge's window", reset));
    const std::uint32_t base = (reset & 0xF000) == 0x1000 ? 0x1000 : 0xF000;
    rom.window = rom.bank_size == 0x800 ? base + 0x800 : base;
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const bool a26 = has_extension(path, {".a26"});
    if (!a26 && !has_extension(path, {".bin"})) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size || *size > kMaxSize || !scheme_of_size(static_cast<std::size_t>(*size))) return false;
    const std::optional<Bytes> data = read_prefix(path, 0, static_cast<std::size_t>(*size));
    if (!data || data->size() != *size) return false;
    try {
        const Rom rom = parse(*data);
        if (a26) return true;
        // a bare .bin: every bank's reset must point at something that looks like start-up code
        for (std::size_t b = 0; b < rom.bank_count(); ++b) {
            const std::optional<std::uint32_t> at = rom.fold(rom.vectors[b].reset);
            if (!at || !plausible_first_opcode(rom.bank(b)[rom.offset(0, *at)])) return false;
        }
        return true;
    } catch (const FormatError&) {
        return false;
    }
}

}  // namespace cb::formats::atari2600
