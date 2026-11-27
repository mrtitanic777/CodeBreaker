#include "formats/saturn.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/disc_image.hpp"
#include "formats/iso9660.hpp"
#include "formats/probe.hpp"

namespace cb::formats::saturn {

namespace {

constexpr std::string_view kReader = "saturn";
constexpr std::string_view kHardwareId = "SEGA SEGASATURN ";
constexpr std::size_t kRawHeader = disc::kRawHeader;
using disc::raw_sync;
constexpr std::uint32_t kDefaultLoad = 0x06004000;

std::uint32_t be32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) << 24 | static_cast<std::uint32_t>(b[at + 1]) << 16 | static_cast<std::uint32_t>(b[at + 2]) << 8 | b[at + 3];
}

bool has_id(ByteView d, std::size_t at) { return d.size() >= at + kHardwareId.size() && std::memcmp(d.data() + at, kHardwareId.data(), kHardwareId.size()) == 0; }

std::string field(ByteView b, std::size_t at, std::size_t n) {
    std::string s = text::from_latin1(b.subspan(at, n));
    const std::size_t nul = s.find('\0');
    if (nul != std::string::npos) s.resize(nul);
    return std::string(text::trim(s));
}

// The files of the root directory of the ISO 9660 volume at sector 16 (empty when there is none).
std::vector<iso9660::Entry> root_directory(const disc::Sectors& sectors) {
    try {
        std::vector<iso9660::Entry> out;
        for (iso9660::Entry& e : iso9660::Volume(sectors).list("/"))
            if (!e.directory) out.push_back(std::move(e));
        return out;
    } catch (const Error&) {
        return {};
    }
}

}  // namespace

bool is_disc_start(ByteView head) { return has_id(head, 0) || (raw_sync(head) && has_id(head, kRawHeader)) || (raw_sync(head) && has_id(head, kRawHeader + 8)); }

std::uint32_t Disc::ip_length() const {
    const std::uint32_t length = std::clamp<std::uint32_t>(header.ip_size, 0x100, static_cast<std::uint32_t>(system.size()));
    return length & ~1u;
}

std::uint32_t Disc::load_address() const { return header.first_read_address != 0 ? header.first_read_address : kDefaultLoad; }

Bytes Disc::memory() const {
    Bytes mem(kHighRamSize, 0);
    const auto put = [&](std::uint32_t address, ByteView data) {
        if (address < kHighRam || address - kHighRam >= mem.size()) return;
        const std::size_t room = mem.size() - (address - kHighRam);
        std::copy_n(data.begin(), std::min(room, data.size()), mem.begin() + (address - kHighRam));
    };
    put(kIpBase, ByteView(system).subspan(0, ip_length()));
    put(load_address(), first_read);
    return mem;
}

std::vector<std::uint32_t> Disc::functions(const Decoder& decode) const {
    const Bytes mem = memory();
    std::set<std::uint32_t> all;
    const auto follow = [&](std::set<std::uint32_t> starts, std::uint32_t begin, std::uint32_t end) {
        for (const std::uint32_t a : find_functions(mem, std::move(starts), decode, {.begin = begin, .end = end}))
            if (a >= begin && a < end && a % 2 == 0) all.insert(a + kHighRam);
    };
    {   // IP.BIN: the area-code entries (each a `bra`) and the initial program they run into
        std::set<std::uint32_t> starts;
        const std::uint32_t begin = kIpBase - kHighRam;
        const std::uint32_t end = begin + ip_length();
        for (std::uint32_t entry = kAreaEntries; entry < kInitAt; entry += 0x20)
            if (entry + 2 <= ip_length() && decode(mem, begin + entry)) starts.insert(begin + entry);
        if (kInitAt + 2 <= ip_length() && decode(mem, begin + kInitAt)) starts.insert(begin + kInitAt);
        if (!starts.empty()) follow(std::move(starts), begin, end);
    }
    if (!first_read.empty() && load_address() >= kHighRam && load_address() - kHighRam + 2 <= mem.size()) {
        const std::uint32_t begin = load_address() - kHighRam;
        const std::uint32_t end = static_cast<std::uint32_t>(std::min<std::size_t>(begin + first_read.size(), mem.size()));
        if (decode(mem, begin)) follow({begin}, begin, end);
    }
    return {all.begin(), all.end()};
}

Disc parse(ByteView image, std::size_t sector_size) {
    const disc::Sectors sectors = disc::Sectors::of(image, sector_size);
    if (!sectors.has(kSystemSectors - 1) && !sectors.has(0)) throw FormatError(kReader, 0, "no system area");
    const Bytes first = sectors.sector(0);
    if (!has_id(first, 0)) throw FormatError(kReader, 0, "not a Saturn disc (no \"SEGA SEGASATURN\" at the start of the first sector)");
    Disc disc;
    disc.sector_size = kSector;
    disc.system = sectors.read(0, kSystemBytes);
    const ByteView h = disc.system;
    if (h.size() < 0x100) throw FormatError(kReader, 0, "shorter than the 256-byte header");
    Header& x = disc.header;
    x.hardware_id = field(h, 0, 16);
    x.maker = field(h, 0x10, 16);
    x.product = field(h, 0x20, 10);
    x.version = field(h, 0x2A, 6);
    x.release_date = field(h, 0x30, 8);
    x.device = field(h, 0x38, 8);
    x.areas = field(h, 0x40, 10);
    x.peripherals = field(h, 0x50, 16);
    x.title = field(h, 0x60, 112);
    x.ip_size = be32(h, 0xE0);
    x.master_stack = be32(h, 0xE8);
    x.slave_stack = be32(h, 0xEC);
    x.first_read_address = be32(h, 0xF0);
    x.first_read_size = be32(h, 0xF4);

    // the first program: the root directory's file with the lowest sector that is not a licence text
    const std::vector<iso9660::Entry> files = root_directory(sectors);
    const iso9660::Entry* chosen = nullptr;
    for (const iso9660::Entry& e : files) {
        if (text::iequals(e.name, "ABS.TXT") || text::iequals(e.name, "BIB.TXT") || text::iequals(e.name, "CPY.TXT") || e.size == 0) continue;
        if (!chosen || e.extent < chosen->extent) chosen = &e;
    }
    if (chosen) {
        disc.first_read_name = chosen->name;
        disc.first_read = sectors.read(chosen->extent, chosen->size);
        if (disc.first_read.size() < chosen->size) disc.first_read.clear();   // (a file cut short by the image is not read)
    }
    return disc;
}

Disc read(const std::filesystem::path& path) {
    std::filesystem::path image = path;
    std::size_t sector_size = 0;
    if (has_extension(path, {".cue"})) {
        const std::optional<disc::Cue> cue = disc::read_cue(path);
        if (!cue) throw FormatError(kReader, 0, "a .cue sheet that names no file");
        image = cue->file;
        sector_size = cue->sector_size;
    }
    const fs::MappedFile file(image);
    return parse(file.view(), sector_size);
}

bool probe(const std::filesystem::path& path) {
    std::filesystem::path image = path;
    if (has_extension(path, {".cue"})) {
        const std::optional<disc::Cue> cue = disc::read_cue(path);
        if (!cue) return false;
        image = cue->file;
    }
    const std::optional<Bytes> head = read_prefix(image, 0, kRawHeader + 8 + kHardwareId.size());
    return head && is_disc_start(*head);
}

}  // namespace cb::formats::saturn
