// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/formats/dreamcast.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::dreamcast {

namespace {

constexpr std::string_view kReader = "dreamcast";
constexpr std::string_view kHardwareId = "SEGA SEGAKATANA ";
constexpr std::size_t kRawHeader = 16;
constexpr std::array<std::uint8_t, 12> kSync = {0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0};
constexpr std::size_t kSlice = 32;                        // the unit the scramble shuffles
constexpr std::size_t kChunk = 2048 * 1024;               // and the chunk it shuffles within

std::uint32_t le32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at + 3]) << 24 | static_cast<std::uint32_t>(b[at + 2]) << 16 | static_cast<std::uint32_t>(b[at + 1]) << 8 | b[at];
}

bool raw_sync(ByteView d) { return d.size() >= kSync.size() && std::memcmp(d.data(), kSync.data(), kSync.size()) == 0; }
bool has_id(ByteView d, std::size_t at) { return d.size() >= at + kHardwareId.size() && std::memcmp(d.data() + at, kHardwareId.data(), kHardwareId.size()) == 0; }

std::string field(ByteView b, std::size_t at, std::size_t n) {
    std::string s = text::from_latin1(b.subspan(at, n));
    const std::size_t nul = s.find('\0');
    if (nul != std::string::npos) s.resize(nul);
    return std::string(text::trim(s));
}

// The sectors of a track: where the user data of sector `n` lies in the bytes.
struct Sectors {
    ByteView image;
    std::size_t stride = kSector;
    std::size_t skip = 0;

    bool has(std::size_t n) const { return (n + 1) * stride <= image.size(); }
    ByteView user(std::size_t n) const { return image.subspan(n * stride + skip, kSector); }
    Bytes read(std::size_t n, std::size_t length) const {
        Bytes out;
        for (std::size_t s = n; out.size() < length && has(s); ++s) {
            const ByteView u = user(s);
            out.insert(out.end(), u.begin(), u.begin() + std::min(kSector, length - out.size()));
        }
        return out;
    }
};

// An ISO 9660 volume descriptor at sector 16 of these sectors?
bool has_volume(const Sectors& s) {
    if (!s.has(16)) return false;
    const ByteView pvd = s.user(16);
    return pvd[0] == 1 && std::memcmp(pvd.data() + 1, "CD001", 5) == 0;
}

// The track that starts at byte `start` of `image`, if its first sector is IP.BIN's and it holds a volume: in 2048-byte sectors, raw ones (mode 1
// or mode 2 form 1) or 2336-byte ones.
std::optional<Sectors> sectors_at(ByteView image, std::size_t start) {
    const ByteView rest = image.subspan(start);
    const auto holds = [&](std::size_t stride, std::size_t skip) -> std::optional<Sectors> {
        if (rest.size() < stride) return std::nullopt;
        if (!has_id(rest, skip)) return std::nullopt;
        if (stride == kRawSector && !raw_sync(rest)) return std::nullopt;
        Sectors s{rest, stride, skip};
        if (!has_volume(s)) return std::nullopt;
        return s;
    };
    for (const auto& [stride, skip] : {std::pair<std::size_t, std::size_t>{kSector, 0}, {kRawSector, 16}, {kRawSector, 24}, {2336, 8}})
        if (const auto s = holds(stride, skip)) return s;
    return std::nullopt;
}

// Where the boot track is in an image: at its start, or (a .cdi) anywhere a sector holding IP.BIN's hardware ID has an ISO volume behind it; the
// last such place is the one (an earlier session's boot track is the one a later session builds on).
std::optional<Sectors> find_track(ByteView image, bool scan) {
    if (const auto at_start = sectors_at(image, 0)) return at_start;
    if (!scan) return std::nullopt;
    std::optional<Sectors> found;
    const std::string_view needle = kHardwareId;
    std::size_t from = 0;
    while (from + needle.size() <= image.size()) {
        const auto it = std::search(image.begin() + static_cast<std::ptrdiff_t>(from), image.end(), needle.begin(), needle.end(),
                                    [](std::uint8_t a, char b) { return a == static_cast<std::uint8_t>(b); });
        if (it == image.end()) break;
        const std::size_t at = static_cast<std::size_t>(it - image.begin());
        for (const std::size_t skip : {std::size_t{0}, std::size_t{8}, std::size_t{16}, std::size_t{24}}) {
            if (at < skip) continue;
            if (const auto s = sectors_at(image, at - skip)) {
                found = s;
                break;
            }
        }
        from = at + 1;
    }
    return found;
}

// A file in the root directory.
struct Entry {
    std::string name;
    std::uint32_t lba = 0, size = 0;   // the sector as the volume numbers it
};

struct Volume {
    std::vector<Entry> files;
    std::uint32_t base = 0;   // the volume's sector number of this track's first sector
};

Volume root_directory(const Sectors& sectors) {
    Volume out;
    const ByteView pvd = sectors.user(16);
    const std::uint32_t root_lba = le32(pvd, 158), root_size = le32(pvd, 166);
    // the offset between the volume's sector numbers and this track's: where the sector holding the root directory (its first record, ".", names
    // that sector) lies
    for (std::size_t idx = 17; idx < 64 && sectors.has(idx); ++idx) {
        const ByteView d = sectors.user(idx);
        if (d[0] >= 34 && d[25] == 2 && d[32] == 1 && d[33] == 0 && le32(d, 2) == root_lba) {
            out.base = root_lba - static_cast<std::uint32_t>(idx);
            break;
        }
    }
    if (root_lba < out.base) return out;
    const Bytes dir = sectors.read(root_lba - out.base, std::min<std::uint32_t>(root_size, 1u << 20));
    std::size_t at = 0;
    while (at + 34 <= dir.size()) {
        const std::size_t length = dir[at];
        if (length == 0) {   // (records do not cross a sector: the rest of this one is padding)
            at = (at / kSector + 1) * kSector;
            continue;
        }
        if (at + length > dir.size() || length < 34) break;
        const std::size_t name_length = dir[at + 32];
        if (33 + name_length <= length && (dir[at + 25] & 2) == 0) {
            std::string name(reinterpret_cast<const char*>(dir.data()) + at + 33, name_length);
            if (const std::size_t semi = name.find(';'); semi != std::string::npos) name.resize(semi);
            out.files.push_back({std::move(name), le32(dir, at + 2), le32(dir, at + 10)});
        }
        at += length;
    }
    return out;
}

Disc build_disc(const Sectors& sectors, Container container) {
    if (!sectors.has(kSystemSectors - 1)) throw FormatError(kReader, 0, "no system area");
    Disc disc;
    disc.container = container;
    disc.system = sectors.read(0, kSystemBytes);
    const ByteView h = disc.system;
    Header& x = disc.header;
    x.hardware_id = field(h, 0, 16);
    x.maker = field(h, 0x10, 16);
    x.device = field(h, 0x20, 16);
    x.areas = field(h, 0x30, 8);
    x.peripherals = field(h, 0x38, 8);
    x.product = field(h, 0x40, 10);
    x.version = field(h, 0x4A, 6);
    x.release_date = field(h, 0x50, 16);
    x.boot_file = field(h, 0x60, 16);
    x.software_maker = field(h, 0x70, 16);
    x.title = field(h, 0x80, 128);

    // the first program: the file the header names, else the usual 1ST_READ.BIN
    const Volume volume = root_directory(sectors);
    const Entry* chosen = nullptr;
    for (const std::string_view want : {std::string_view(x.boot_file), std::string_view("1ST_READ.BIN")}) {
        if (want.empty()) continue;
        for (const Entry& e : volume.files)
            if (text::iequals(e.name, want) && e.size != 0 && e.lba >= volume.base) {
                chosen = &e;
                break;
            }
        if (chosen) break;
    }
    if (chosen) {
        disc.first_read_name = chosen->name;
        disc.first_read = sectors.read(chosen->lba - volume.base, chosen->size);
        if (disc.first_read.size() < chosen->size) disc.first_read.clear();   // (a file cut short by the image is not read)
    }
    disc.program = disc.first_read;
    if (looks_scrambled(disc.first_read)) {
        disc.scrambled = true;
        disc.program = descramble(disc.first_read);
    }
    return disc;
}

// ---- the .gdi sheet ---------------------------------------------------------------------------------------------

struct GdiTrack {
    unsigned number = 0;
    std::uint32_t lba = 0;
    unsigned type = 0;           // 4: data, 0: audio
    std::size_t sector_size = 0;
    std::string file;
    std::uint64_t offset = 0;
};

// The words of a line, a quoted one being a word.
std::vector<std::string> gdi_words(std::string_view line) {
    std::vector<std::string> out;
    std::size_t i = 0;
    while (i < line.size()) {
        while (i < line.size() && text::is_space(line[i])) ++i;
        if (i >= line.size()) break;
        std::string word;
        if (line[i] == '"') {
            const std::size_t close = line.find('"', i + 1);
            word = std::string(line.substr(i + 1, close == std::string_view::npos ? std::string_view::npos : close - i - 1));
            i = close == std::string_view::npos ? line.size() : close + 1;
        } else {
            while (i < line.size() && !text::is_space(line[i])) word.push_back(line[i++]);
        }
        out.push_back(std::move(word));
    }
    return out;
}

std::optional<std::vector<GdiTrack>> read_gdi(const std::filesystem::path& path) {
    std::string sheet;
    try {
        sheet = fs::read_text(path);
    } catch (const Error&) {
        return std::nullopt;
    }
    std::vector<GdiTrack> tracks;
    bool first = true;
    for (const std::string_view raw : text::lines(sheet)) {
        const std::string_view line = text::trim(raw);
        if (line.empty()) continue;
        const std::vector<std::string> w = gdi_words(line);
        if (first) {   // the track count
            first = false;
            if (w.size() != 1 || !text::parse_int<unsigned>(w[0])) return std::nullopt;
            continue;
        }
        if (w.size() < 5) return std::nullopt;
        GdiTrack t;
        const auto number = text::parse_int<unsigned>(w[0]);
        const auto lba = text::parse_int<std::uint32_t>(w[1]);
        const auto type = text::parse_int<unsigned>(w[2]);
        const auto size = text::parse_int<std::size_t>(w[3]);
        if (!number || !lba || !type || !size) return std::nullopt;
        t.number = *number;
        t.lba = *lba;
        t.type = *type;
        t.sector_size = *size;
        t.file = w[4];
        if (w.size() > 5) t.offset = text::parse_int<std::uint64_t>(w[5]).value_or(0);
        tracks.push_back(std::move(t));
    }
    if (tracks.empty()) return std::nullopt;
    return tracks;
}

}  // namespace

// ---- the scramble -----------------------------------------------------------------------------------------------

namespace {

// The generator the console's own program uses: 16 bits of state, a multiplier of 2109 and an increment of 9273, the result 0xC000 past it.
class SliceRandom {
public:
    explicit SliceRandom(std::uint32_t seed) : seed_(seed & 0xFFFF) {}
    std::uint32_t next() {
        seed_ = (seed_ * 2109 + 9273) & 0x7FFF;
        return (seed_ + 0xC000) & 0xFFFF;
    }

private:
    std::uint32_t seed_;
};

// For a chunk of `slices` slices: which slice of the plain chunk each slice of the scrambled one is. The scrambled chunk is made going down from
// the last slice: slice i swaps places with a slice picked by the generator (from 0 up to i) and the one picked is the next of the scrambled chunk.
std::vector<std::uint32_t> chunk_order(SliceRandom& random, std::size_t slices) {
    std::vector<std::uint32_t> idx(slices);
    for (std::size_t i = 0; i < slices; ++i) idx[i] = static_cast<std::uint32_t>(i);
    std::vector<std::uint32_t> order;
    order.reserve(slices);
    for (std::size_t i = slices; i-- > 0;) {
        const std::size_t x = (static_cast<std::uint64_t>(random.next()) * i) >> 16;
        std::swap(idx[i], idx[x]);
        order.push_back(idx[i]);
    }
    return order;
}

// Both directions over a whole file: `forward` takes the plain file to the scrambled one.
Bytes shuffle(ByteView data, bool forward) {
    Bytes out(data.begin(), data.end());   // (the bytes after a chunk's last whole slice stay as they are)
    SliceRandom random(static_cast<std::uint32_t>(data.size()));
    std::size_t pos = 0;
    while (pos < data.size()) {
        const std::size_t length = std::min(kChunk, data.size() - pos);
        const std::vector<std::uint32_t> order = chunk_order(random, length / kSlice);
        for (std::size_t k = 0; k < order.size(); ++k) {
            const std::size_t scrambled_at = pos + k * kSlice, plain_at = pos + static_cast<std::size_t>(order[k]) * kSlice;
            if (forward) std::copy_n(data.begin() + static_cast<std::ptrdiff_t>(plain_at), kSlice, out.begin() + static_cast<std::ptrdiff_t>(scrambled_at));
            else std::copy_n(data.begin() + static_cast<std::ptrdiff_t>(scrambled_at), kSlice, out.begin() + static_cast<std::ptrdiff_t>(plain_at));
        }
        pos += length;
    }
    return out;
}

// What the SH-4's delay slot allows (read from the words' bits, so the format reader needs no decoder): 1 = an instruction that may stand there,
// 2 = one that may not (a branch without a delay slot, TRAPA, MOVA, a PC-relative load), 3 = a branch with a delay slot.
std::uint8_t slot_class(std::uint16_t w) {
    const unsigned top = w >> 12;
    if (top == 0xA || top == 0xB) return 3;                                // bra, bsr
    if ((w & 0xFF00) == 0x8D00 || (w & 0xFF00) == 0x8F00) return 3;        // bt/s, bf/s
    if ((w & 0xF0FF) == 0x0023 || (w & 0xF0FF) == 0x0003) return 3;        // braf, bsrf
    if ((w & 0xF0FF) == 0x402B || (w & 0xF0FF) == 0x400B) return 3;        // jmp, jsr
    if (w == 0x000B || w == 0x002B) return 3;                              // rts, rte
    if ((w & 0xFF00) == 0x8900 || (w & 0xFF00) == 0x8B00) return 2;        // bt, bf
    if ((w & 0xFF00) == 0xC300 || (w & 0xFF00) == 0xC700) return 2;        // trapa, mova
    if (top == 0x9 || top == 0xD) return 2;                                // mov.w and mov.l @(disp,pc)
    return 1;
}

// How many places in the first chunk have a branch whose delay slot cannot be one: code does not, so a count that falls when the slices are put back is
// a scrambled program's.
std::size_t slot_violations(ByteView data) {
    std::size_t bad = 0;
    for (std::size_t at = 0; at + 4 <= data.size(); at += 2) {
        const std::uint16_t w = static_cast<std::uint16_t>(data[at + 1] << 8 | data[at]);
        if (slot_class(w) != 3) continue;
        const std::uint16_t next = static_cast<std::uint16_t>(data[at + 3] << 8 | data[at + 2]);
        if (slot_class(next) != 1) ++bad;
    }
    return bad;
}

}  // namespace

Bytes descramble(ByteView data) { return shuffle(data, false); }
Bytes scramble(ByteView data) { return shuffle(data, true); }

bool looks_scrambled(ByteView data) {
    if (data.size() < 8 * kSlice) return false;
    // (the first chunk alone decides: its order is the generator's first outputs whatever follows)
    const ByteView first = data.first(std::min(kChunk, data.size()));
    const std::size_t as_is = slot_violations(first);
    if (as_is < 4) return false;
    SliceRandom random(static_cast<std::uint32_t>(data.size()));
    const std::vector<std::uint32_t> order = chunk_order(random, first.size() / kSlice);
    Bytes undone(first.begin(), first.end());
    for (std::size_t k = 0; k < order.size(); ++k)
        std::copy_n(first.begin() + static_cast<std::ptrdiff_t>(k * kSlice), kSlice, undone.begin() + static_cast<std::ptrdiff_t>(static_cast<std::size_t>(order[k]) * kSlice));
    const std::size_t after = slot_violations(undone);
    return as_is > after + std::max<std::size_t>(3, as_is / 8);
}

// ---- the disc ---------------------------------------------------------------------------------------------------

bool is_disc_start(ByteView head) { return has_id(head, 0) || (raw_sync(head) && has_id(head, kRawHeader)) || (raw_sync(head) && has_id(head, kRawHeader + 8)); }

Bytes Disc::memory() const {
    Bytes mem(system.begin(), system.end());
    mem.resize(kSystemBytes, 0);
    mem.insert(mem.end(), program.begin(), program.end());
    return mem;
}

std::vector<std::uint32_t> Disc::functions(const Decoder& decode) const {
    const Bytes mem = memory();
    std::set<std::uint32_t> all;
    const auto follow = [&](std::set<std::uint32_t> starts, std::uint32_t begin, std::uint32_t end) {
        for (const std::uint32_t a : find_functions(mem, std::move(starts), decode, {.begin = begin, .end = end}))
            if (a >= begin && a < end && a % 2 == 0) all.insert(a + kIpBase);
    };
    if (kIpEntry + 2 <= ip_length() && decode(mem, kIpEntry)) follow({kIpEntry}, kIpEntry, static_cast<std::uint32_t>(kSystemBytes));
    if (!program.empty() && decode(mem, static_cast<std::uint32_t>(kSystemBytes))) follow({static_cast<std::uint32_t>(kSystemBytes)}, static_cast<std::uint32_t>(kSystemBytes), static_cast<std::uint32_t>(mem.size()));
    return {all.begin(), all.end()};
}

Disc parse(ByteView image, std::size_t sector_size) {
    (void)sector_size;   // (the layout is read from the data: the id, the sync and the volume descriptor agree on it)
    const std::optional<Sectors> track = find_track(image, true);
    if (!track) throw FormatError(kReader, 0, "not a Dreamcast disc (no \"SEGA SEGAKATANA\" at the start of a sector with an ISO 9660 volume 16 sectors on)");
    const std::size_t start = static_cast<std::size_t>(track->image.data() - image.data());
    return build_disc(*track, start != 0 ? Container::Cdi : track->stride == kSector ? Container::Iso : Container::Raw);
}

Disc read(const std::filesystem::path& path) {
    if (has_extension(path, {".gdi"})) {
        const std::optional<std::vector<GdiTrack>> tracks = read_gdi(path);
        if (!tracks) throw FormatError(kReader, 0, "not a .gdi track sheet");
        // IP.BIN is in the last data track that holds it
        for (auto it = tracks->rbegin(); it != tracks->rend(); ++it) {
            if (it->type != 4) continue;
            const std::filesystem::path file = path.parent_path() / fs::from_text(it->file);
            if (!std::filesystem::is_regular_file(file)) continue;
            const fs::MappedFile mapped(file);
            if (mapped.view().size() <= it->offset) continue;
            const std::optional<Sectors> s = sectors_at(mapped.view(), static_cast<std::size_t>(it->offset));
            if (!s) continue;
            Disc disc = build_disc(*s, Container::Gdi);
            disc.tracks = tracks->size();
            return disc;
        }
        throw FormatError(kReader, 0, "no data track of the .gdi holds IP.BIN");
    }
    const fs::MappedFile file(path);
    Disc disc = parse(file.view());
    if (has_extension(path, {".cdi"})) disc.container = Container::Cdi;
    return disc;
}

bool probe(const std::filesystem::path& path) {
    if (has_extension(path, {".gdi"})) {
        const std::optional<std::vector<GdiTrack>> tracks = read_gdi(path);
        if (!tracks) return false;
        for (auto it = tracks->rbegin(); it != tracks->rend(); ++it) {
            if (it->type != 4) continue;
            const std::optional<Bytes> head = read_prefix(path.parent_path() / fs::from_text(it->file), it->offset, kRawHeader + 8 + kHardwareId.size());
            if (head && is_disc_start(*head)) return true;
        }
        return false;
    }
    if (has_extension(path, {".cdi"})) {   // (the boot track is anywhere in the file: a scan)
        try {
            const fs::MappedFile file(path);
            return find_track(file.view(), true).has_value();
        } catch (const Error&) {
            return false;
        }
    }
    const std::optional<Bytes> head = read_prefix(path, 0, kRawHeader + 8 + kHardwareId.size());
    return head && is_disc_start(*head);
}

}  // namespace cb::formats::dreamcast
