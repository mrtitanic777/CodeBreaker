#include "cb/formats/nes.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::nes {

namespace {

constexpr std::string_view kReader = "nes";
constexpr std::uint8_t kMagic[4] = {'N', 'E', 'S', 0x1A};
constexpr std::uint32_t kRomBase = 0x8000;
constexpr std::uint32_t kLastAddress = 0xFFFF;

bool has_magic(ByteView data) { return data.size() >= 4 && std::memcmp(data.data(), kMagic, 4) == 0; }

Vectors read_vectors(const Bytes& mem) {
    io::ByteReader r(mem, kReader);
    r.seek(0xFFFA);
    Vectors v;
    v.nmi = r.u16();
    v.reset = r.u16();
    v.irq = r.u16();
    return v;
}

}  // namespace

std::string_view to_string(Mirroring m) {
    switch (m) {
        case Mirroring::Horizontal: return "horizontal";
        case Mirroring::Vertical: return "vertical";
        case Mirroring::FourScreen: return "four-screen";
    }
    return "horizontal";
}

ByteView Rom::bank(std::size_t index) const {
    CB_ASSERT(index < bank_count(), "NES bank index out of range");
    return ByteView(prg).subspan(index * kPrgUnit, kPrgUnit);
}

Bytes Rom::memory() const {
    Bytes mem(kAddressSpace, 0);
    auto place = [&](std::size_t at, ByteView src) { std::copy(src.begin(), src.end(), mem.begin() + at); };
    if (mapper == 0 || prg.size() <= 0x8000) {
        if (prg.size() == kPrgUnit) {
            // 16 KB: the same bank at $8000 and $C000.
            place(0x8000, prg);
            place(0xC000, prg);
        } else {
            place(0x8000, ByteView(prg).first(std::min<std::size_t>(prg.size(), 0x8000)));
        }
    } else {
        place(0x8000, bank(0));
        place(0xC000, bank(bank_count() - 1));
    }
    return mem;
}

std::optional<std::uint32_t> Rom::mapped(std::uint32_t address) const {
    if (address < kRomBase) return std::nullopt;
    if (prg.size() == kPrgUnit) return (address - kRomBase) & 0x3FFF;
    if (mapper == 0 || prg.size() <= 0x8000) return address - kRomBase;
    if (address < 0xC000) return address - kRomBase;
    return static_cast<std::uint32_t>(prg.size() - kPrgUnit) + (address - 0xC000);
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    const Bytes mem = memory();
    std::set<std::uint32_t> starts;
    for (std::uint32_t v : {std::uint32_t{vectors.nmi}, std::uint32_t{vectors.reset}, std::uint32_t{vectors.irq}}) {
        if (v >= kRomBase) starts.insert(v);
    }
    return find_functions(mem, std::move(starts), decode, {.begin = kRomBase, .end = kLastAddress + 1});
}

Rom parse(ByteView data) {
    if (!has_magic(data)) throw FormatError(kReader, 0, "not an iNES image");
    if (data.size() < kHeaderSize) throw FormatError(kReader, 0, std::format("only {} bytes: too short for an iNES header", data.size()));
    Rom rom;
    rom.header.assign(data.begin(), data.begin() + kHeaderSize);
    std::uint32_t prg_units = data[4];
    std::uint32_t chr_units = data[5];
    const std::uint8_t f6 = data[6];
    const std::uint8_t f7 = data[7];
    rom.nes2 = (f7 & 0x0C) == 0x08;
    rom.mapper = static_cast<std::uint16_t>((f6 >> 4) | (f7 & 0xF0));
    if (rom.nes2) {
        rom.mapper = static_cast<std::uint16_t>(rom.mapper | ((data[8] & 0x0F) << 8));
        rom.submapper = static_cast<std::uint8_t>(data[8] >> 4);
        prg_units |= static_cast<std::uint32_t>(data[9] & 0x0F) << 8;
        chr_units |= static_cast<std::uint32_t>(data[9] & 0xF0) << 4;
    }
    rom.mirroring = (f6 & 0x08) ? Mirroring::FourScreen : (f6 & 1) ? Mirroring::Vertical : Mirroring::Horizontal;
    rom.battery = (f6 & 0x02) != 0;

    const std::uint64_t trainer_size = (f6 & 0x04) ? kTrainerSize : 0;
    const std::uint64_t prg_size = std::uint64_t{prg_units} * kPrgUnit;
    const std::uint64_t chr_size = std::uint64_t{chr_units} * kChrUnit;
    const std::uint64_t body = data.size() - kHeaderSize;
    // The program must be all there. A dump may be trimmed in its CHR (or its trainer absent), and then
    // the CHR is what is left: tools that wrote such files exist, and the program is still readable.
    if (trainer_size + prg_size > body) throw FormatError(kReader, kHeaderSize,
                                  std::format("the PRG is cut short ({} of {} bytes)", body - std::min(body, trainer_size), prg_size));
    std::uint64_t at = kHeaderSize;
    auto take = [&](std::uint64_t length) {
        const std::uint64_t n = std::min<std::uint64_t>(length, data.size() - at);   // explicit clamp, see above
        Bytes part(data.begin() + static_cast<std::ptrdiff_t>(at), data.begin() + static_cast<std::ptrdiff_t>(at + n));
        at += n;
        return part;
    };
    rom.trainer = take(trainer_size);
    rom.prg = take(prg_size);
    rom.chr = take(chr_size);
    rom.extra.assign(data.begin() + static_cast<std::ptrdiff_t>(at), data.end());
    rom.code_at = rom.prg.size() == kPrgUnit ? 0xC000 : 0x8000;
    rom.vectors = read_vectors(rom.memory());
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, 4);
    return head && has_magic(*head);
}

}  // namespace cb::formats::nes
