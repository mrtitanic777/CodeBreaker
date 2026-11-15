#include "cb/formats/segacd.hpp"

#include <algorithm>
#include <cstring>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::segacd {

namespace {

constexpr std::string_view kReader = "segacd";
constexpr std::size_t kMemorySize = 0x1000000;
constexpr std::string_view kDiscId = "SEGADISCSYSTEM";
constexpr std::array<std::uint8_t, 12> kSync = {0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0};

bool has_id(ByteView d, std::size_t at) { return d.size() >= at + kDiscId.size() && std::memcmp(d.data() + at, kDiscId.data(), kDiscId.size()) == 0; }
bool raw_sync(ByteView d) { return d.size() >= kSync.size() && std::memcmp(d.data(), kSync.data(), kSync.size()) == 0; }

std::uint32_t be32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) << 24 | static_cast<std::uint32_t>(b[at + 1]) << 16 | static_cast<std::uint32_t>(b[at + 2]) << 8 | b[at + 3];
}
std::uint16_t be16(ByteView b, std::size_t at) { return static_cast<std::uint16_t>(b[at] << 8 | b[at + 1]); }

std::string field(ByteView b, std::size_t at, std::size_t n) {
    std::string s = text::from_latin1(b.subspan(at, n));
    const std::size_t nul = s.find('\0');
    if (nul != std::string::npos) s.resize(nul);
    return std::string(text::trim(s));
}

Program program_at(ByteView boot, std::size_t at) { return {be32(boot, at), be32(boot, at + 4), be32(boot, at + 8), be32(boot, at + 12)}; }

}  // namespace

bool is_disc_start(ByteView head) { return has_id(head, 0) || (raw_sync(head) && has_id(head, kRawHeader)); }

Bytes Disc::main_memory() const {
    Bytes mem(kMemorySize, 0);
    std::copy(boot.begin() + main_begin, boot.begin() + main_end, mem.begin() + kMainBase);
    return mem;
}

Bytes Disc::sub_memory() const {
    Bytes mem(kMemorySize, 0);
    if (has_sub()) std::copy(boot.begin() + sub_begin, boot.begin() + sub_end, mem.begin() + kSubBase);
    return mem;
}

std::array<std::uint32_t, 4> Disc::sub_entries() const {
    std::array<std::uint32_t, 4> out{};
    if (!has_sub() || sub_end - sub_begin < 0x20) return out;
    const std::uint32_t length = sub_end - sub_begin;
    const std::uint32_t table = be32(boot, sub_begin + 0x18);   // the jump table's offset in the SP
    if (table % 2 != 0 || table + 8 > length) return out;
    for (std::size_t i = 0; i < out.size(); ++i) {
        const std::uint16_t word = be16(boot, sub_begin + table + 2 * i);   // an offset from the table's start
        const std::uint32_t target = table + word;
        if (word != 0 && target < length && target % 2 == 0) out[i] = kSubBase + target;
    }
    return out;
}

std::vector<std::uint32_t> Disc::functions(const Decoder& decode) const {
    std::set<std::uint32_t> all;
    {   // the main CPU's program
        std::set<std::uint32_t> starts = {main_address(main_begin)};
        const std::uint32_t ip_entry = header.ip.start + header.ip.entry;
        if (ip_entry >= main_begin && ip_entry < main_end && ip_entry % 2 == 0) starts.insert(main_address(ip_entry));
        const Bytes mem = main_memory();
        for (const std::uint32_t a : find_functions(mem, std::move(starts), decode, {.begin = kMainBase, .end = main_address(main_end), .even_targets_only = true}))
            all.insert(a);
    }
    if (has_sub()) {
        std::set<std::uint32_t> starts;
        for (const std::uint32_t e : sub_entries()) {
            if (e != 0) starts.insert(e);
        }
        const Bytes mem = sub_memory();
        for (const std::uint32_t a : find_functions(mem, std::move(starts), decode, {.begin = kSubBase, .end = sub_address(sub_end), .even_targets_only = true}))
            all.insert(a);
    }
    return {all.begin(), all.end()};
}

Disc parse(ByteView data) {
    Disc disc;
    std::size_t user = 0;
    if (has_id(data, 0)) {
        disc.sector_size = kSector;
    } else if (raw_sync(data) && has_id(data, kRawHeader)) {
        disc.sector_size = kRawSector;
        user = kRawHeader;
    } else {
        throw FormatError(kReader, 0, "no \"SEGADISCSYSTEM\" at the start of the disc");
    }
    for (std::size_t s = 0; s < kBootBytes / kSector; ++s) {
        const std::size_t at = s * disc.sector_size + user;
        if (at + kSector > data.size()) break;
        disc.boot.insert(disc.boot.end(), data.begin() + static_cast<std::ptrdiff_t>(at), data.begin() + static_cast<std::ptrdiff_t>(at + kSector));
    }
    const ByteView boot = disc.boot;
    if (boot.size() < kSecurityAt) throw FormatError(kReader, 0, "too short for the boot header");

    Header& h = disc.header;
    h.disc_id = field(boot, 0, 16);
    h.volume = field(boot, 0x10, 11);
    h.system = field(boot, 0x20, 11);
    h.ip = program_at(boot, 0x30);
    h.sp = program_at(boot, 0x40);
    h.system_type = field(boot, 0x100, 16);
    h.copyright = field(boot, 0x110, 16);
    h.domestic_name = field(boot, 0x120, 48);
    h.overseas_name = field(boot, 0x150, 48);
    h.serial = field(boot, 0x180, 14);
    h.region = field(boot, 0x1F0, 3);

    // The main program: from the security code to the IP's end, when the header's IP lies in the system area and after the security code.
    const std::uint64_t ip_end = std::uint64_t{h.ip.start} + h.ip.size;
    disc.main_begin = kSecurityAt;
    disc.main_end = h.ip.start >= kSecurityAt && h.ip.size != 0 && ip_end <= boot.size() ? static_cast<std::uint32_t>(ip_end)
                                                                                         : static_cast<std::uint32_t>(std::min<std::size_t>(boot.size(), 0x800));
    // The sub program: the SP's range clipped to the system area, when it lies after the main program.
    if (h.sp.size != 0 && h.sp.start >= disc.main_end && h.sp.start % 2 == 0 && h.sp.start < boot.size()) {
        disc.sub_begin = h.sp.start;
        disc.sub_end = static_cast<std::uint32_t>(std::min<std::uint64_t>(std::uint64_t{h.sp.start} + h.sp.size, boot.size()));
    }
    return disc;
}

Disc read(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, kRawSector * 16);   // (the system area: a few KB are read, not the disc)
    if (!head) throw IoError(path, 0, "cannot be read");
    return parse(*head);
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, kRawHeader + 16);
    return head && is_disc_start(*head);
}

}  // namespace cb::formats::segacd
