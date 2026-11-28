#include "formats/nes.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::nes {

namespace eng = cb::formats::engine;

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
    // The layout is ines.fmt's (the header's fields and what they mean, the trainer, PRG, CHR and the rest); what a
    // short file means stays here: the header and the PRG must be all there, a trimmed CHR is read as far as it goes.
    if (!has_magic(data)) throw FormatError(kReader, 0, "not an iNES image");
    if (data.size() < kHeaderSize) throw FormatError(kReader, 0, std::format("only {} bytes: too short for an iNES header", data.size()));
    eng::ReadOptions options;
    options.keep_source = false;   // only read from
    const eng::Tree tree = eng::builtin().read("ines", data, options);
    const eng::Node& h = tree.root.get("header");
    Rom rom;
    rom.header.assign(data.begin(), data.begin() + kHeaderSize);
    rom.nes2 = h.u("nes2") != 0;
    rom.mapper = static_cast<std::uint16_t>(h.u("mapper"));
    rom.submapper = static_cast<std::uint8_t>(h.u("submapper"));
    rom.mirroring = static_cast<Mirroring>(h.u("mirroring"));
    rom.battery = h.u("battery") != 0;
    auto part = [&](const eng::Node* n) { return n ? Bytes(data.begin() + static_cast<std::ptrdiff_t>(n->offset),
                                                           data.begin() + static_cast<std::ptrdiff_t>(n->offset + n->size)) : Bytes{}; };
    rom.trainer = part(tree.root.find("trainer"));
    rom.prg = part(&tree.root.get("prg"));
    rom.chr = part(&tree.root.get("chr"));
    rom.extra = part(&tree.root.get("extra"));
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
