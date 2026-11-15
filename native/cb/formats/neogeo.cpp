#include "cb/formats/neogeo.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::neogeo {

namespace {

constexpr std::string_view kReader = "neogeo";
constexpr std::size_t kMemorySize = 0x1000000;
constexpr std::array<std::uint8_t, 4> kNeoMagic = {'N', 'E', 'O', 1};

bool plain_header(ByteView d, std::size_t at) { return d.size() >= at + 8 && std::memcmp(d.data() + at, "NEO-GEO", 7) == 0; }
// "NEO-GEO\0" with each pair of bytes reversed.
bool swapped_header(ByteView d, std::size_t at) {
    return d.size() >= at + 8 && std::memcmp(d.data() + at, "EN-OEG", 6) == 0 && d[at + 6] == 0 && d[at + 7] == 'O';
}

bool neo_magic(ByteView d) { return d.size() >= 4 && std::memcmp(d.data(), kNeoMagic.data(), 4) == 0; }

std::string c_string(ByteView d, std::size_t at, std::size_t n) {
    std::string s(as_text(d.subspan(at, n)));
    const std::size_t nul = s.find('\0');
    if (nul != std::string::npos) s.resize(nul);
    return std::string(text::trim(s));
}

}  // namespace

std::string_view layout_text(Layout layout) {
    switch (layout) {
        case Layout::Plain: return "P-ROM";
        case Layout::WordSwapped: return "P-ROM, words swapped";
        case Layout::Neo: return ".neo container";
        case Layout::Halves: return "P-ROM, from even and odd halves";
    }
    return "";
}

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
        case 25: return "vblank";
        case 26: return "timer";
        case 27: return "reset_button";
        default: break;
    }
    if (n >= 32 && n < 48) return std::format("trap{}", n - 32);
    return {};
}

Bytes swap_words(ByteView data) {
    Bytes out(data.begin(), data.end());
    for (std::size_t i = 0; i + 1 < out.size(); i += 2) std::swap(out[i], out[i + 1]);
    return out;
}

Bytes join_halves(ByteView even, ByteView odd) {
    if (even.size() != odd.size()) throw FormatError(kReader, 0, "the even and odd halves differ in size");
    Bytes out(even.size() * 2);
    for (std::size_t i = 0; i < even.size(); ++i) {
        out[2 * i] = even[i];
        out[2 * i + 1] = odd[i];
    }
    return out;
}

std::uint32_t Rom::address_of(std::size_t offset) {
    return offset < kRomWindow ? static_cast<std::uint32_t>(offset) : kBankAt + static_cast<std::uint32_t>(offset - kRomWindow);
}

std::size_t Rom::offset_of(std::uint32_t address) const {
    if (address < std::min(data.size(), kRomWindow)) return address;
    if (address >= kBankAt && address < kBankAt + kRomWindow && kRomWindow + (address - kBankAt) < data.size()) return kRomWindow + (address - kBankAt);
    return data.size();
}

Bytes Rom::memory() const {
    Bytes mem(kMemorySize, 0);
    std::copy_n(data.begin(), std::min(data.size(), kRomWindow), mem.begin());
    if (data.size() > kRomWindow) std::copy_n(data.begin() + kRomWindow, std::min(data.size() - kRomWindow, kRomWindow), mem.begin() + kBankAt);
    return mem;
}

std::vector<std::uint32_t> Rom::callback_jumps() const {
    std::vector<std::uint32_t> out;
    for (std::size_t i = 0; i < header.callbacks.size(); ++i) {
        if (header.callbacks[i] != 0) out.push_back(kFirstCode + 6 * static_cast<std::uint32_t>(i));
    }
    return out;
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    const std::uint32_t limit = static_cast<std::uint32_t>(std::min(data.size(), kRomWindow));
    std::set<std::uint32_t> starts;
    auto add = [&](std::uint32_t v) {
        if (v >= kFirstCode && v < limit && v % 2 == 0) starts.insert(v);
    };
    add(reset);
    for (const Vector& v : vectors) add(v.address);
    for (const std::uint32_t callback : header.callbacks) add(callback);
    add(header.security);
    return find_functions(ByteView(data).first(limit), std::move(starts), decode, {.begin = kFirstCode, .end = limit, .even_targets_only = true});
}

Rom parse(ByteView data, std::string_view extension) {
    Rom rom;
    ByteView program = data;
    if (neo_magic(data) && (text::iequals(extension, ".neo") || data.size() >= kNeoHeader)) {
        io::ByteReader r(data, kReader);
        r.seek(4);
        Neo neo;
        neo.p_size = r.u32();
        neo.s_size = r.u32();
        neo.m_size = r.u32();
        neo.v1_size = r.u32();
        neo.v2_size = r.u32();
        neo.c_size = r.u32();
        neo.year = r.u32();
        neo.genre = r.u32();
        r.u32();   // the screenshot kind
        neo.ngh = r.u32();
        if (data.size() < kNeoHeader) throw FormatError(kReader, 0, "too short for a .neo header");
        neo.name = c_string(data, 44, 33);
        neo.manufacturer = c_string(data, 77, 17);
        program = io::checked_range(data, kNeoHeader, neo.p_size);
        const std::uint64_t m_at = std::uint64_t{kNeoHeader} + neo.p_size + neo.s_size;
        if (neo.m_size != 0) {
            const ByteView m = io::checked_range(data, m_at, neo.m_size);
            rom.sound.assign(m.begin(), m.end());
        }
        rom.layout = Layout::Neo;
        rom.neo = std::move(neo);
    }
    if (program.size() < kMinSize) throw FormatError(kReader, 0, "too short for a Neo Geo P-ROM");
    if (swapped_header(program, kHeaderAt)) {
        rom.data = swap_words(program);
        if (rom.layout == Layout::Plain) rom.layout = Layout::WordSwapped;
    } else if (plain_header(program, kHeaderAt)) {
        rom.data.assign(program.begin(), program.end());
    } else {
        throw FormatError(kReader, kHeaderAt, "no \"NEO-GEO\" at $100");
    }

    const ByteView h = rom.data;
    io::ByteReader r(h, kReader);
    r.seek(0x107);
    rom.header.system_version = r.u8();
    rom.header.ngh = r.u16_be();
    rom.header.program_size = r.u32_be();
    rom.header.backup_ram = r.u32_be();
    rom.header.save_size = r.u16_be();
    rom.header.eye_catcher = r.u8();
    rom.header.eye_catcher_bank = r.u8();
    for (std::uint32_t& s : rom.header.settings) s = r.u32_be();
    for (std::size_t i = 0; i < rom.header.callbacks.size(); ++i) {
        const std::size_t at = kFirstCode + 6 * i;   // JMP (xxx).L
        if (h[at] == 0x4E && h[at + 1] == 0xF9) {
            rom.header.callbacks[i] = static_cast<std::uint32_t>(h[at + 2]) << 24 | static_cast<std::uint32_t>(h[at + 3]) << 16 |
                                      static_cast<std::uint32_t>(h[at + 4]) << 8 | h[at + 5];
        }
    }

    r.seek(0x182);
    rom.header.security = r.u32_be();
    r.seek(0);
    std::array<std::uint32_t, 64> longs{};
    for (std::uint32_t& l : longs) l = r.u32_be();
    rom.initial_ssp = longs[0];
    rom.reset = longs[1];
    const std::size_t limit = std::min(rom.data.size(), kRomWindow);
    for (unsigned n = 0; n < longs.size(); ++n) {
        std::string name = vector_name(n);
        const std::uint32_t v = longs[n];
        if (!name.empty() && v < limit && v % 2 == 0 && v >= kFirstCode) rom.vectors.push_back({std::move(name), v});
    }
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view(), extension_of(path));
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, 0x108);
    if (!head) return false;
    if (plain_header(*head, kHeaderAt) || swapped_header(*head, kHeaderAt)) return true;
    return neo_magic(*head) && has_extension(path, {".neo"});
}

}  // namespace cb::formats::neogeo
