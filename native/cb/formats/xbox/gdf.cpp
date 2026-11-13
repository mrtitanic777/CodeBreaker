#include "cb/formats/xbox/gdf.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <iterator>

#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::xbox::gdf {

namespace {

constexpr std::string_view kReader = "xdvdfs";
// where the volume starts on the discs seen: a plain image, XGD1, XGD2, XGD3, redump, and one 64 KB in
constexpr std::int64_t kBases[] = {0, 0xFD90000, 0x2080000, 0xFB20000, 0x18300000, 0x10000};
constexpr std::uint32_t kMaxDirectory = 1u << 20;   // a folder is at most 1 MB (a bigger size is damage)
constexpr std::size_t kEntryHeader = 0x0E;
constexpr std::uint8_t kDirectoryAttribute = 0x10;
constexpr std::uint32_t kNoEntry = 0xFFFF;

std::uint32_t le32(const Bytes& b, std::size_t at) {
    return b[at] | (b[at + 1] << 8) | (b[at + 2] << 16) | (static_cast<std::uint32_t>(b[at + 3]) << 24);
}
std::uint32_t le16(const Bytes& b, std::size_t at) { return b[at] | (b[at + 1] << 8); }

Bytes read_signed(Device& device, std::int64_t at, std::size_t length) {
    if (at < 0) throw FormatError(kReader, 0, std::format("a read at {} is before the start of the device", at));
    return device.read_at(static_cast<std::uint64_t>(at), length);
}

// The path with ASCII and Latin-1 capitals made small (the UTF-8 forms: A-Z, and U+00C0..U+00DE but U+00D7 are C3 80..C3 9E but C3 97),
// the key paths compare by. Byte order of the result is code point order.
std::string lower_key(std::string_view path) {
    std::string out(path);
    for (std::size_t i = 0; i < out.size(); ++i) {
        const auto c = static_cast<unsigned char>(out[i]);
        if (c >= 'A' && c <= 'Z') {
            out[i] = static_cast<char>(c + 32);
        } else if (c == 0xC3 && i + 1 < out.size()) {
            const auto d = static_cast<unsigned char>(out[i + 1]);
            if (d >= 0x80 && d <= 0x9E && d != 0x97) out[i + 1] = static_cast<char>(d + 0x20);
            ++i;
        }
    }
    return out;
}

// The key of the sort of `build`: names in capitals, the way the console's own authoring tools order a folder (Latin-1 bytes).
std::vector<std::uint32_t> upper_key(const Bytes& latin1) {
    std::vector<std::uint32_t> out;
    for (const std::uint8_t c : latin1) {
        if (c >= 'a' && c <= 'z') {
            out.push_back(c - 32u);
        } else if (c == 0xDF) {
            out.push_back('S');   // sharp s is SS
            out.push_back('S');
        } else if (c == 0xB5) {
            out.push_back(0x39C);   // micro sign is capital Greek mu
        } else if (c == 0xFF) {
            out.push_back(0x178);
        } else if (c >= 0xE0 && c <= 0xFE && c != 0xF7) {
            out.push_back(c - 32u);
        } else {
            out.push_back(c);
        }
    }
    return out;
}

}  // namespace

Disc Disc::open(std::shared_ptr<Device> device, std::optional<std::int64_t> base, std::optional<std::int64_t> descriptor) {
    Disc d;
    d.device_ = std::move(device);
    bool found = base.has_value();
    std::int64_t at = base.value_or(0);
    if (!found) {
        for (const std::int64_t candidate : kBases) {
            if (as_text(read_signed(*d.device_, candidate + 32 * std::int64_t{kSector}, kMagic.size())) == kMagic) {
                at = candidate;
                found = true;
                break;
            }
        }
    }
    if (!found) throw FormatError(kReader, 0, "no XDVDFS volume found");
    d.base_ = at;
    const std::int64_t where = descriptor.value_or(at + 32 * std::int64_t{kSector});
    const Bytes vd = read_signed(*d.device_, where, kSector);
    if (vd.size() < 0x1C || as_text(ByteView(vd).subspan(0, kMagic.size())) != kMagic) {
        throw FormatError(kReader, static_cast<std::uint64_t>(std::max<std::int64_t>(where, 0)), "the volume descriptor does not carry the XDVDFS magic");
    }
    d.root_sector_ = le32(vd, 0x14);
    d.root_size_ = le32(vd, 0x18);
    return d;
}

// A directory's entries and, under each folder, its own. The entries of a directory are a binary tree (each names the places of the
// entries before and after it in name order), so it is walked as one, from the first entry: a folder of a large title once "held"
// itself, without end, when its sectors were read in a row. `seen` holds the directories met, so a folder that leads back to one is
// not entered again.
std::vector<Entry> Disc::walk(std::uint32_t sector, std::uint32_t size, const std::string& prefix, std::unordered_set<std::uint32_t>& seen) {
    std::vector<Entry> out;
    if (seen.contains(sector) || size == 0 || size > kMaxDirectory) return out;
    seen.insert(sector);
    const std::size_t padded = (size + kSector - 1) / kSector * kSector;
    const Bytes data = read_signed(*device_, base_ + std::int64_t{sector} * kSector, padded);

    std::vector<std::size_t> todo{0};
    std::unordered_set<std::size_t> visited;   // (lookup only)
    while (!todo.empty()) {
        const std::size_t pos = todo.back();
        todo.pop_back();
        if (visited.contains(pos) || pos + kEntryHeader > data.size()) continue;
        visited.insert(pos);
        const std::uint32_t left = le16(data, pos), right = le16(data, pos + 2);
        const std::uint32_t start = le32(data, pos + 4), file_size = le32(data, pos + 8);
        const std::uint8_t attributes = data[pos + 0x0C], name_length = data[pos + 0x0D];
        if (name_length == 0 || name_length == 0xFF || pos + kEntryHeader + name_length > data.size()) continue;
        std::string name = text::from_latin1(ByteView(data).subspan(pos + kEntryHeader, name_length));
        std::string full = prefix.empty() ? name : prefix + "/" + name;
        const bool is_dir = (attributes & kDirectoryAttribute) != 0;
        out.push_back({full, is_dir, start, file_size});
        if (is_dir) {
            std::vector<Entry> below = walk(start, file_size, full, seen);
            out.insert(out.end(), std::make_move_iterator(below.begin()), std::make_move_iterator(below.end()));
        }
        for (const std::uint32_t next : {left, right}) {
            if (next != kNoEntry) todo.push_back(std::size_t{next} * 4);
        }
    }
    // by lower-cased path, equal paths keeping the order met
    std::vector<std::pair<std::string, std::size_t>> keys;
    keys.reserve(out.size());
    for (std::size_t i = 0; i < out.size(); ++i) keys.emplace_back(lower_key(out[i].path), i);
    std::stable_sort(keys.begin(), keys.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    std::vector<Entry> sorted;
    sorted.reserve(out.size());
    for (const auto& k : keys) sorted.push_back(std::move(out[k.second]));
    return sorted;
}

const std::vector<Entry>& Disc::entries() {
    if (!entries_) {
        std::unordered_set<std::uint32_t> seen;
        entries_ = walk(root_sector_, root_size_, {}, seen);
    }
    return *entries_;
}

std::vector<Entry> Disc::files() {
    std::vector<Entry> out;
    for (const Entry& e : entries()) {
        if (!e.is_dir) out.push_back(e);
    }
    return out;
}

Bytes Disc::read(const Entry& entry) {
    return read_signed(*device_, base_ + std::int64_t{entry.start} * kSector, entry.size);
}

std::optional<Bytes> Disc::try_read(std::string_view path) {
    const std::string want = lower_key(path);
    for (const Entry& e : entries()) {
        if (!e.is_dir && lower_key(e.path) == want) return read(e);
    }
    return std::nullopt;
}

Bytes Disc::read(std::string_view path) {
    std::optional<Bytes> data = try_read(path);
    if (!data) throw NotFoundError(std::filesystem::path(std::string(path)), 0, "no such file on the disc");
    return std::move(*data);
}

// ---- building ------------------------------------------------------------------------------------------------------------------

Bytes build(std::span<const SourceFile> files) {
    struct Placed {
        Bytes name;                  // Latin-1
        std::vector<std::uint32_t> key;
        const SourceFile* file;
    };
    std::vector<Placed> order;
    for (const SourceFile& f : files) {
        Bytes latin1 = text::to_latin1(f.name);
        if (f.name.empty() || latin1.size() > 255 || text::from_latin1(latin1) != f.name) {
            throw UsageError(std::format("gdf::build: '{}' is not a name of 1 to 255 Latin-1 characters", f.name));
        }
        order.push_back({latin1, upper_key(latin1), &f});
    }
    std::stable_sort(order.begin(), order.end(), [](const Placed& a, const Placed& b) { return a.key < b.key; });

    const std::int64_t n = static_cast<std::int64_t>(order.size());
    std::int64_t directory_bytes = 0;
    for (const Placed& p : order) directory_bytes += 0x0E + static_cast<std::int64_t>(p.name.size()) + 3;
    constexpr std::int64_t kRootSector = 33;
    const std::int64_t data_sector = 34 + (directory_bytes + kSector - 1) / kSector;

    Bytes entries;
    std::int64_t sector = data_sector;
    std::vector<std::pair<std::int64_t, const Bytes*>> placed;
    for (std::int64_t i = 0; i < n; ++i) {
        const Placed& p = order[static_cast<std::size_t>(i)];
        const Bytes& data = p.file->data;
        const std::int64_t sectors = (static_cast<std::int64_t>(data.size()) + kSector - 1) / kSector;
        const std::int64_t entry_size = (0x0E + static_cast<std::int64_t>(p.name.size()) + 3) & ~std::int64_t{3};
        const auto used = static_cast<std::int64_t>(entries.size());
        if (used / kSector != (used + entry_size - 1) / kSector) {   // an entry does not straddle a sector: 0xFF to the next one
            entries.insert(entries.end(), static_cast<std::size_t>((kSector - used % kSector) % kSector), 0xFF);
        }
        const std::int64_t here = static_cast<std::int64_t>(entries.size());
        const std::int64_t right = i == n - 1 ? 0xFFFF : (here + entry_size) / 4;
        io::ByteWriter e;
        e.u16(0xFFFF);
        e.u16(static_cast<std::uint16_t>(right));
        e.u32(static_cast<std::uint32_t>(sector));
        e.u32(static_cast<std::uint32_t>(data.size()));
        e.u8(0x20);
        e.u8(static_cast<std::uint8_t>(p.name.size()));
        e.bytes(p.name);
        while (e.size() % 4 != 0) e.u8(0xFF);
        append(entries, e.data());
        placed.emplace_back(sector, &data);
        sector += sectors;
    }
    const auto root_size = static_cast<std::uint32_t>(entries.size());
    Bytes volume(static_cast<std::size_t>(sector * kSector), 0);
    const std::size_t vd = 32 * kSector;
    std::memcpy(&volume[vd], kMagic.data(), kMagic.size());
    for (int k = 0; k < 4; ++k) {
        volume[vd + 0x14 + k] = static_cast<std::uint8_t>(kRootSector >> (8 * k));
        volume[vd + 0x18 + k] = static_cast<std::uint8_t>(root_size >> (8 * k));
    }
    std::memcpy(&volume[vd + 0x7EC], kMagic.data(), kMagic.size());   // the magic again at the end of the descriptor
    if (!entries.empty()) std::memcpy(&volume[kRootSector * kSector], entries.data(), entries.size());
    for (const auto& [at, data] : placed) {
        if (!data->empty()) std::memcpy(&volume[static_cast<std::size_t>(at * kSector)], data->data(), data->size());
    }
    return volume;
}

// ---- the command ---------------------------------------------------------------------------------------------------------------

std::string_view usage() { return "usage: gdf {info|list|extract} <image.iso|svod-dir> [path] [out]\n"; }

namespace {

Disc open_image_or_folder(const std::string& path) {
    const std::filesystem::path p = fs::from_text(path);
    std::error_code ec;
    if (std::filesystem::is_directory(p, ec)) {   // a Games-on-Demand .data folder: the de-interleaving device, the volume at 0
        return Disc::open(SvodDevice::open(p), 0);
    }
    return Disc::open(std::make_shared<FileDevice>(p));
}

}  // namespace

int command(std::span<const std::string> args) {
    if (args.empty() || args[0] == "-h" || args[0] == "--help") {
        out_text(usage());
        return 0;
    }
    const std::string& op = args[0];
    if (args.size() < 2 || (op != "info" && op != "list" && op != "extract")) {
        out_text(op == "info" || op == "list" || op == "extract" ? std::format("usage: gdf {} <image|svod-dir>\n", op) : std::format("unknown op '{}'\n", op));
        return 2;
    }
    Disc disc = open_image_or_folder(args[1]);
    if (op == "info") {
        const std::vector<Entry> files = disc.files();
        std::uint64_t total = 0;
        for (const Entry& e : files) total += e.size;
        out("volume base : 0x{:X}\n", disc.base());
        out("root sector : {}  size 0x{:X}\n", disc.root_sector(), disc.root_size());
        out("files       : {}, {} bytes total\n", files.size(), total);
    } else if (op == "list") {
        const std::vector<Entry> files = disc.files();
        for (const Entry& e : files) out("  {:>12}  {}\n", e.size, e.path);
        out("total: {} files\n", files.size());
    } else {
        if (args.size() < 3) {
            out_text("usage: gdf extract <image> <path-on-disc> [out]\n");
            return 2;
        }
        const Bytes data = disc.read(args[2]);
        const std::string leaf = args[2].substr(args[2].find_last_of('/') == std::string::npos ? 0 : args[2].find_last_of('/') + 1);
        const std::string dest = args.size() > 3 ? args[3] : leaf;
        fs::write_file(fs::from_text(dest), data);
        out("extracted {} ({} bytes) -> {}\n", args[2], data.size(), dest);
    }
    return 0;
}

}  // namespace cb::formats::xbox::gdf
