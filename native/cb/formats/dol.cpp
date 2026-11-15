// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/formats/dol.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::dol {

namespace {

constexpr std::string_view kReader = "dol";
constexpr std::uint32_t kMem1Base = 0x80000000, kMem1End = 0x81800000;   // 24 MB of main memory, cached
constexpr std::uint32_t kMem2Base = 0x90000000, kMem2End = 0x94000000;   // the Wii's second memory (64 MB)

std::uint32_t be32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) << 24 | static_cast<std::uint32_t>(b[at + 1]) << 16 | static_cast<std::uint32_t>(b[at + 2]) << 8 | b[at + 3];
}

std::string section_name(std::size_t i) { return i < kTextSections ? std::format("text{}", i) : std::format("data{}", i - kTextSections); }

bool console_address(std::uint32_t a) { return (a >= kMem1Base && a < kMem1End) || (a >= kMem2Base && a < kMem2End); }

// Fill a Dol from the header; FormatError for one that is not a DOL's.
Dol parse_header(ByteView data, std::uint64_t file_size) {
    if (data.size() < kHeaderSize) throw FormatError(kReader, 0, "shorter than the 256-byte header");
    Dol dol;
    for (std::size_t i = 0; i < kSections; ++i) {
        dol.offsets[i] = be32(data, 4 * i);
        dol.addresses[i] = be32(data, 0x48 + 4 * i);
        dol.sizes[i] = be32(data, 0x90 + 4 * i);
    }
    dol.bss_address = be32(data, 0xD8);
    dol.bss_size = be32(data, 0xDC);
    dol.entry = be32(data, 0xE0);
    bool any = false;
    bool entry_in_code = false;
    for (std::size_t i = 0; i < kSections; ++i) {
        if (dol.sizes[i] == 0) continue;
        any = true;
        const std::uint64_t end = static_cast<std::uint64_t>(dol.offsets[i]) + dol.sizes[i];
        if (dol.offsets[i] < kHeaderSize || end > file_size) throw FormatError(kReader, 4 * i, std::format("{} lies outside the file", section_name(i)));
        if (dol.addresses[i] % 4 != 0 || !console_address(dol.addresses[i]) || !console_address(dol.addresses[i] + dol.sizes[i] - 1))
            throw FormatError(kReader, 0x48 + 4 * i, std::format("{} is not loaded into the console's memory", section_name(i)));
        if (i < kTextSections && dol.entry >= dol.addresses[i] && dol.entry - dol.addresses[i] < dol.sizes[i]) entry_in_code = true;
    }
    if (!any) throw FormatError(kReader, 0, "no section is used");
    if (!entry_in_code || dol.entry % 4 != 0) throw FormatError(kReader, 0xE0, "the entry point is not in a code section");
    // sections must not overlap in the file, or the source could not give each its own bytes
    std::vector<std::pair<std::uint32_t, std::uint32_t>> spans;
    for (std::size_t i = 0; i < kSections; ++i)
        if (dol.sizes[i] != 0) spans.emplace_back(dol.offsets[i], dol.sizes[i]);
    std::ranges::sort(spans);
    for (std::size_t i = 1; i < spans.size(); ++i)
        if (spans[i].first < spans[i - 1].first + spans[i - 1].second) throw FormatError(kReader, 0, "two sections overlap in the file");
    return dol;
}

}  // namespace

std::string_view console_text(Console console) { return console == Console::Wii ? "Wii" : "GameCube"; }

std::vector<ppc_segment::Segment> Dol::segments() const {
    std::vector<ppc_segment::Segment> out;
    for (std::size_t i = 0; i < kSections; ++i) {
        if (sizes[i] == 0) continue;
        out.push_back({section_name(i), addresses[i], offsets[i], sizes[i], sizes[i], i < kTextSections});
    }
    return out;
}

const ppc_segment::Segment* Dol::segment_at(const std::vector<ppc_segment::Segment>& segments, std::uint32_t address) const {
    for (const ppc_segment::Segment& s : segments)
        if (address >= s.address && address - s.address < s.size) return &s;
    return nullptr;
}

Console Dol::console() const {
    for (std::size_t i = 0; i < kSections; ++i) {
        if (sizes[i] != 0 && addresses[i] >= kMem2Base) return Console::Wii;
    }
    if (entry >= kMem2Base) return Console::Wii;
    // `lis rD, 0xCDxx`: the Hollywood registers (0xCD000000)
    for (std::size_t i = 0; i < kTextSections; ++i) {
        if (sizes[i] == 0) continue;
        const ByteView code = ByteView(data).subspan(offsets[i], sizes[i]);
        for (std::size_t at = 0; at + 4 <= code.size(); at += 4) {
            const std::uint32_t w = be32(code, at);
            if ((w & 0xFC1F0000) != 0x3C000000) continue;   // addis rD,0,imm
            const std::uint32_t imm = w & 0xFFFF;
            if (imm >= 0xCD00 && imm <= 0xCDFF) return Console::Wii;
        }
    }
    return Console::GameCube;
}

Dol parse(ByteView data) {
    Dol dol = parse_header(data, data.size());
    dol.data.assign(data.begin(), data.end());
    return dol;
}

Dol read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size || *size < kHeaderSize || *size > (std::uint64_t{64} << 20)) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, kHeaderSize);
    if (!head || head->size() < kHeaderSize) return false;
    try {
        parse_header(*head, *size);   // (the header alone, with the file's size for the bounds)
        return true;
    } catch (const Error&) {
        return false;
    }
}

}  // namespace cb::formats::dol
