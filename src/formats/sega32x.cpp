#include "formats/sega32x.hpp"

#include <algorithm>
#include <cstring>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/probe.hpp"

namespace cb::formats::sega32x {

namespace {

constexpr std::string_view kReader = "sega32x";
constexpr std::size_t kVectorCount = 256;

std::uint32_t be32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) << 24 | static_cast<std::uint32_t>(b[at + 1]) << 16 | static_cast<std::uint32_t>(b[at + 2]) << 8 | b[at + 3];
}

std::string field(ByteView b, std::size_t at, std::size_t n) {
    std::string s = text::from_latin1(b.subspan(at, n));
    const std::size_t nul = s.find('\0');
    if (nul != std::string::npos) s.resize(nul);
    return std::string(text::trim(s));
}

// The address of the cartridge window or of SDRAM (any cache view: the top three bits are the cache's)?
bool is_start_address(std::uint32_t a) {
    const std::uint32_t p = a & 0x1FFFFFFF;
    return a % 2 == 0 && ((p >= kSdram && p < kSdram + kSdramSize) || (p >= kCartWindow && p < kCartWindow + kCartWindowSize));
}

}  // namespace

bool is_rom_start(ByteView head, bool dot_32x) {
    if (head.size() < kMinSize || std::memcmp(head.data() + 0x100, "SEGA", 4) != 0) return false;
    const bool says_32x = field(head, 0x100, 16).find("32X") != std::string::npos;
    return (says_32x || dot_32x) && is_start_address(be32(head, 0x3E0)) && is_start_address(be32(head, 0x3E4));
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, kMinSize);
    return head && is_rom_start(*head, has_extension(path, {".32x"}));
}

ByteView Rom::sh2_program() const {
    if (header.sh2_offset >= data.size()) return {};
    const std::size_t length = std::min<std::size_t>({header.sh2_size, data.size() - header.sh2_offset, kSdramSize});
    return ByteView(data).subspan(header.sh2_offset, length);
}

bool Rom::has_sh2_program() const {
    return header.sh2_size != 0 && header.sh2_size <= kSdramSize && header.sh2_offset < data.size() && header.sh2_offset + header.sh2_size <= data.size();
}

bool Rom::in_program(std::uint32_t address) const {
    const std::uint32_t p = address & 0x1FFFFFFF;
    return has_sh2_program() && p >= kSdram && p - kSdram < sh2_program().size();
}

Bytes Rom::sdram() const {
    Bytes mem(kSdramSize, 0);
    const ByteView program = sh2_program();
    std::copy(program.begin(), program.end(), mem.begin());
    return mem;
}

std::vector<std::uint32_t> Rom::vector_targets(const Decoder& decode) const {
    std::set<std::uint32_t> out;
    const ByteView program = sh2_program();
    for (const std::uint32_t vbr : {header.master_vbr, header.slave_vbr}) {
        const std::uint32_t p = vbr & 0x1FFFFFFF;
        if (p < kSdram || p - kSdram + 4 > program.size()) continue;
        for (std::size_t n = 0; n < kVectorCount && p - kSdram + 4 * n + 4 <= program.size(); ++n) {
            if (n == 1 || n == 3) continue;   // (the stack pointers)
            const std::uint32_t v = be32(program, p - kSdram + 4 * n);
            if (!in_program(v) || v % 2 != 0) continue;
            if (decode(program, (v & 0x1FFFFFFF) - kSdram)) out.insert(v & 0x1FFFFFFF);
        }
    }
    return {out.begin(), out.end()};
}

std::vector<std::uint32_t> Rom::functions(const Decoder& decode) const {
    const ByteView program = sh2_program();
    std::set<std::uint32_t> starts;   // as offsets in the program: the decoder works on the program's bytes
    const auto add = [&](std::uint32_t address) {
        if (in_program(address) && (address & 1) == 0) starts.insert((address & 0x1FFFFFFF) - kSdram);
    };
    add(header.master_start);
    add(header.slave_start);
    for (const std::uint32_t v : vector_targets(decode)) add(v);
    const std::vector<std::uint32_t> found = find_functions(program, std::move(starts), decode, {.begin = 0, .end = static_cast<std::uint32_t>(program.size())});
    std::vector<std::uint32_t> out;
    for (const std::uint32_t off : found)
        if (off < program.size() && off % 2 == 0) out.push_back(address_of(off));
    return out;
}

Rom parse(ByteView data) {
    if (data.size() < kMinSize) throw FormatError(kReader, 0, "shorter than the 32X header");
    Rom rom;
    rom.data.assign(data.begin(), data.end());
    const ByteView h = rom.data;
    Header& x = rom.header;
    x.system = field(h, 0x100, 16);
    x.domestic_name = field(h, 0x120, 48);
    x.overseas_name = field(h, 0x150, 48);
    x.serial = field(h, 0x180, 14);
    x.region = field(h, 0x1F0, 3);
    x.module_name = field(h, 0x3C0, 16);
    x.reserved_3d0 = be32(h, 0x3D0);
    x.sh2_offset = be32(h, 0x3D4);
    x.reserved_3d8 = be32(h, 0x3D8);
    x.sh2_size = be32(h, 0x3DC);
    x.master_start = be32(h, 0x3E0);
    x.slave_start = be32(h, 0x3E4);
    x.master_vbr = be32(h, 0x3E8);
    x.slave_vbr = be32(h, 0x3EC);
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

}  // namespace cb::formats::sega32x
