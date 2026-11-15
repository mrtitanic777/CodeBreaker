// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/formats/n64.hpp"

#include <algorithm>
#include <bit>
#include <set>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::n64 {

namespace {

constexpr std::string_view kReader = "n64";

std::uint32_t be32(ByteView d, std::size_t at) {
    return static_cast<std::uint32_t>(d[at]) << 24 | static_cast<std::uint32_t>(d[at + 1]) << 16 | static_cast<std::uint32_t>(d[at + 2]) << 8 | d[at + 3];
}

std::uint32_t swap_pairs(std::uint32_t w) { return (w & 0xFF00FF00u) >> 8 | (w & 0x00FF00FFu) << 8; }
std::uint32_t reverse_bytes(std::uint32_t w) { return swap_pairs(w) << 16 | swap_pairs(w) >> 16; }

std::string trimmed(ByteView d, std::size_t at, std::size_t n) {
    std::string s(reinterpret_cast<const char*>(d.data()) + at, n);
    while (!s.empty() && (s.back() == ' ' || s.back() == '\0')) s.pop_back();
    return s;
}

struct CicEntry {
    std::uint32_t crc32;
    Cic cic;
};
constexpr std::array<CicEntry, 5> kCics = {{{0x6170A4A1, Cic::C6101}, {0x90BB6CB5, Cic::C6102}, {0x0B050EE0, Cic::C6103}, {0x98BC2C86, Cic::C6105}, {0xACC8580A, Cic::C6106}}};

}  // namespace

std::string_view order_text(ByteOrder order) {
    switch (order) {
        case ByteOrder::Z64: return ".z64";
        case ByteOrder::V64: return ".v64";
        case ByteOrder::N64: return ".n64";
    }
    return ".z64";
}

std::string_view cic_text(Cic cic) {
    switch (cic) {
        case Cic::C6101: return "CIC-NUS-6101";
        case Cic::C6102: return "CIC-NUS-6102";
        case Cic::C6103: return "CIC-NUS-6103";
        case Cic::C6105: return "CIC-NUS-6105";
        case Cic::C6106: return "CIC-NUS-6106";
        case Cic::Unknown: break;
    }
    return "unknown boot code";
}

std::optional<ByteOrder> order_of(ByteView head) {
    if (head.size() < 4) return std::nullopt;
    const std::uint32_t w = be32(head, 0);
    if (w == kPiConfig) return ByteOrder::Z64;
    if (w == swap_pairs(kPiConfig)) return ByteOrder::V64;
    if (w == reverse_bytes(kPiConfig)) return ByteOrder::N64;
    return std::nullopt;
}

Bytes Rom::convert(ByteView bytes, ByteOrder from, ByteOrder to) {
    Bytes out(bytes.begin(), bytes.end());
    if (from == to) return out;
    // each swap is its own inverse: undo `from`'s, then apply `to`'s
    const auto pairs = [&] {
        for (std::size_t i = 0; i + 1 < out.size(); i += 2) std::swap(out[i], out[i + 1]);
    };
    const auto words = [&] {
        for (std::size_t i = 0; i + 3 < out.size(); i += 4) std::reverse(out.begin() + static_cast<std::ptrdiff_t>(i), out.begin() + static_cast<std::ptrdiff_t>(i) + 4);
    };
    if (from == ByteOrder::V64) pairs();
    if (from == ByteOrder::N64) words();
    if (to == ByteOrder::V64) pairs();
    if (to == ByteOrder::N64) words();
    return out;
}

ByteView Rom::program() const {
    const ByteView all(data);
    if (all.size() <= kBootSize) return {};
    return all.subspan(kBootSize, std::min(kLoadSize, all.size() - kBootSize));
}

std::uint32_t Rom::load_address() const {
    switch (cic) {
        case Cic::C6103: return header.entry - 0x100000;
        case Cic::C6106: return header.entry - 0x200000;
        default: return header.entry;
    }
}

Bytes Rom::memory() const {
    Bytes ram(kRdramSize, 0);
    const ByteView program_bytes = program();
    const std::size_t at = load_address() & 0x1FFFFFFF;
    if (at < ram.size()) std::copy_n(program_bytes.begin(), std::min(program_bytes.size(), ram.size() - at), ram.begin() + static_cast<std::ptrdiff_t>(at));
    // the words of the OS's reserved area the boot code and the PIF leave: the TV type, the ROM base, how much RDRAM there is (a program's startup
    // code sets its stack from it)
    const auto put = [&](std::size_t where, std::uint32_t value) {
        for (unsigned i = 0; i < 4; ++i) ram[where + i] = static_cast<std::uint8_t>(value >> (24 - 8 * i));
    };
    const std::string_view pal = "PDFSIHXY";
    put(0x300, header.country == 'B' ? 2u : pal.find(header.country) != std::string_view::npos && header.country != 0 ? 0u : 1u);   // osTvType: NTSC 1, PAL 0, MPAL 2
    put(0x308, 0xB0000000u);                                                                                                  // osRomBase
    put(0x318, static_cast<std::uint32_t>(kRdramSize));                                                                      // osMemSize
    return ram;
}

std::optional<std::pair<std::uint32_t, std::uint32_t>> Rom::checksums() const {
    if (data.size() < kBootSize + kLoadSize) return std::nullopt;
    std::uint32_t seed = 0;
    switch (cic) {
        case Cic::C6101: case Cic::C6102: case Cic::Unknown: seed = 0xF8CA4DDC; break;   // (a homebrew boot code is checksummed as the 6102, the commonest)
        case Cic::C6103: seed = 0xA3886759; break;
        case Cic::C6105: seed = 0xDF26F436; break;
        case Cic::C6106: seed = 0x1FEA617A; break;
    }
    std::uint32_t t1 = seed, t2 = seed, t3 = seed, t4 = seed, t5 = seed, t6 = seed;
    const ByteView d(data);
    for (std::size_t i = kBootSize; i < kBootSize + kLoadSize; i += 4) {
        const std::uint32_t c = be32(d, i);
        const std::uint32_t sum = t6 + c;
        if (sum < t6) ++t4;
        t6 = sum;
        t3 ^= c;
        const std::uint32_t k = std::rotl(c, static_cast<int>(c & 0x1F));
        t5 += k;
        t2 ^= t2 > c ? k : t6 ^ c;
        t1 += cic == Cic::C6105 ? be32(d, kHeaderSize + 0x0710 + (i & 0xFF)) ^ c : t5 ^ c;
    }
    if (cic == Cic::C6103) return std::pair{(t6 ^ t4) + t3, (t5 ^ t2) + t1};
    if (cic == Cic::C6106) return std::pair{t6 * t4 + t3, t5 * t2 + t1};
    return std::pair{t6 ^ t4 ^ t3, t5 ^ t2 ^ t1};
}

bool Rom::checksums_match() const {
    const auto sums = checksums();
    return sums && sums->first == header.crc1 && sums->second == header.crc2;
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    const Bytes ram = memory();
    const std::uint32_t load = load_address();
    const std::uint32_t begin = load & 0x1FFFFFFF;
    const std::uint32_t end = std::min<std::uint32_t>(begin + static_cast<std::uint32_t>(program().size()), kRdramSize);
    std::set<std::uint32_t> starts;
    const std::uint32_t entry = begin;   // IPL3 jumps to where it copied the program
    if (entry >= begin && entry < end && entry % 4 == 0) starts.insert(entry);
    const std::vector<std::uint32_t> found = find_functions(ram, std::move(starts), decode, {.begin = begin, .end = end});
    std::vector<std::uint32_t> out;
    for (const std::uint32_t a : found)
        if (a >= begin && a < end && a % 4 == 0) out.push_back(a | (load & ~0x1FFFFFFFu));
    return out;
}

Rom parse(ByteView file) {
    const std::optional<ByteOrder> order = order_of(file);
    if (!order) throw FormatError(kReader, 0, "not a Nintendo 64 ROM (no $80371240 at the start in any byte order)");
    if (file.size() < kBootSize) throw FormatError(kReader, 0, "shorter than the boot code ($1000 bytes)");
    Rom rom;
    rom.order = *order;
    rom.data = Rom::convert(file, *order, ByteOrder::Z64);
    const ByteView d(rom.data);
    Header& h = rom.header;
    h.pi_config = be32(d, 0);
    h.clock_rate = be32(d, 4);
    h.entry = be32(d, 8);
    h.release = be32(d, 0xC);
    h.crc1 = be32(d, 0x10);
    h.crc2 = be32(d, 0x14);
    h.name = trimmed(d, 0x20, 20);
    h.category = static_cast<char>(d[0x3B]);
    h.game_code = std::string(reinterpret_cast<const char*>(d.data()) + 0x3C, 2);
    h.country = static_cast<char>(d[0x3E]);
    h.version = d[0x3F];
    rom.boot_crc32 = crypto::crc32(rom.boot_code());
    for (const CicEntry& c : kCics)
        if (c.crc32 == rom.boot_crc32) rom.cic = c.cic;
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, 4);
    if (!head || !order_of(*head)) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    return size && *size >= kBootSize;
}

}  // namespace cb::formats::n64
