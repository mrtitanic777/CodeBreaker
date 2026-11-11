#include "cb/formats/iso9660.hpp"

#include <algorithm>
#include <format>
#include <optional>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/random_file.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::iso9660 {
namespace {

constexpr std::string_view kReader = "iso9660";
constexpr std::size_t kFirstDescriptor = 16;   // sectors 0..15 are the system area
constexpr std::size_t kMaxDescriptors = 32;    // a volume has a handful; this stops a damaged one
constexpr std::size_t kRootRecordAt = 156;     // in a volume descriptor
constexpr std::size_t kVolumeSizeAt = 80;      // in a volume descriptor (both-endian, 4 bytes)
constexpr std::uint8_t kFlagDirectory = 0x02;
constexpr std::uint8_t kFlagMultiExtent = 0x80;

std::uint32_t le32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | static_cast<std::uint32_t>(b[at + 1]) << 8 |
           static_cast<std::uint32_t>(b[at + 2]) << 16 | static_cast<std::uint32_t>(b[at + 3]) << 24;
}

void put_both32(ByteSpan b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) {
        b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));       // little-endian half
        b[at + 4 + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));   // big-endian half
    }
}

// One directory record.
struct Record {
    std::uint64_t at = 0;        // where in the image the record is (to patch it)
    std::uint32_t extent = 0;    // first sector of its data
    std::uint32_t size = 0;
    std::uint8_t flags = 0;
    std::string name;            // decoded, without ";1" and without a trailing "."
    bool directory() const { return (flags & kFlagDirectory) != 0; }
};

// One tree of the volume: the primary one, or the Joliet one.
struct Tree {
    bool joliet = false;
    std::uint64_t descriptor_at = 0;   // where its volume descriptor is
    Record root;
};

// A name as the volume stores it, as text: Joliet names are UCS-2, big-endian.
std::string decode_name(ByteView raw, bool joliet) {
    std::string name;
    if (joliet) {
        for (std::size_t i = 0; i + 1 < raw.size(); i += 2)
            text::append_utf8(name, static_cast<char32_t>(raw[i] << 8 | raw[i + 1]));
    } else {
        name.assign(as_text(raw));
    }
    if (const std::size_t version = name.rfind(';'); version != std::string::npos)
        name.resize(version);
    if (name.size() > 1 && name.back() == '.')
        name.pop_back();   // (a file with no extension is written "NAME.;1")
    return name;
}

// A path component as asked for: without a ";1" version, as a record's name is.
std::string_view without_version(std::string_view part) {
    if (const std::size_t v = part.rfind(';'); v != std::string_view::npos)
        part = part.substr(0, v);
    return part;
}

}  // namespace

struct Volume::Impl {
    std::filesystem::path path;
    Access access;
    fs::RandomFile file;
    std::vector<Tree> trees;   // the primary tree first, then the Joliet one if there is one
    std::uint32_t sectors = 0;

    Impl(const std::filesystem::path& p, Access a)
        : path(p), access(a), file(p, a == Access::Read ? fs::FileMode::Read : fs::FileMode::ReadWrite) {}

    [[noreturn]] void bad(std::uint64_t offset, const std::string& what) const { throw FormatError(kReader, offset, what); }

    Bytes read_at(std::uint64_t offset, std::size_t count) const {
        Bytes out = file.read_at(offset, count);
        if (out.size() != count)
            throw TruncatedError(kReader, offset, count, out.size());
        return out;
    }

    // The record that starts at `at` in a directory's bytes (`dir_at`: where those bytes are in the image).
    std::optional<Record> record_in(ByteView dir, std::size_t at, std::uint64_t dir_at, bool joliet) const {
        const std::size_t length = dir[at];
        if (length == 0)
            return std::nullopt;
        if (length < 34 || at + length > dir.size() || 33u + dir[at + 32] > length)
            bad(dir_at + at, "a directory record that does not fit");
        Record r;
        r.at = dir_at + at;
        r.extent = le32(dir, at + 2);
        r.size = le32(dir, at + 10);
        r.flags = dir[at + 25];
        const ByteView raw = dir.subspan(at + 33, dir[at + 32]);
        if (raw.size() == 1 && raw[0] <= 1) {   // "." and ".."
            r.name = raw[0] == 0 ? "." : "..";
            return r;
        }
        r.name = decode_name(raw, joliet);
        return r;
    }

    // The records of the directory `dir`, without "." and "..". A record never crosses a sector; a zero length byte
    // pads the sector out.
    std::vector<Record> children(const Tree& tree, const Record& dir) const {
        if (!dir.directory())
            bad(dir.at, std::format("{} is not a directory", dir.name));
        const std::uint64_t at = std::uint64_t{dir.extent} * kSectorSize;
        const Bytes bytes = read_at(at, dir.size);
        std::vector<Record> out;
        std::size_t pos = 0;
        while (pos < bytes.size()) {
            const std::optional<Record> r = record_in(bytes, pos, at, tree.joliet);
            if (!r) {
                pos = (pos / kSectorSize + 1) * kSectorSize;
                continue;
            }
            pos += bytes[pos];
            if (r->name != "." && r->name != "..")
                out.push_back(*r);
        }
        return out;
    }

    // The record of `path` in a tree; nullopt if there is none.
    std::optional<Record> find(const Tree& tree, std::string_view where) const {
        Record at = tree.root;
        for (const std::string_view part : text::split(where, '/')) {
            if (part.empty())
                continue;
            std::optional<Record> next;
            for (const Record& c : children(tree, at))
                if (text::iequals(c.name, without_version(part))) {
                    next = c;
                    break;
                }
            if (!next)
                return std::nullopt;
            at = *next;
        }
        return at;
    }

    Record find_or_throw(const Tree& tree, std::string_view where) const {
        if (const std::optional<Record> r = find(tree, where))
            return *r;
        throw NotFoundError(path, 2, std::format("{} is not in the volume", where));
    }

    void read_descriptors() {
        for (std::size_t i = 0; i < kMaxDescriptors; ++i) {
            const std::uint64_t at = (kFirstDescriptor + i) * kSectorSize;
            const Bytes d = file.read_at(at, kSectorSize);
            if (d.size() < kSectorSize || d[1] != 'C' || d[2] != 'D' || d[3] != '0' || d[4] != '0' || d[5] != '1')
                break;
            if (d[0] == 255)
                break;   // the terminator
            const bool primary = d[0] == 1;
            // a supplementary descriptor is Joliet's when its escape sequence is one of UCS-2's three levels
            const bool joliet = d[0] == 2 && d[88] == '%' && d[89] == '/' && (d[90] == '@' || d[90] == 'C' || d[90] == 'E');
            if (!primary && !joliet)
                continue;
            Tree t;
            t.joliet = joliet;
            t.descriptor_at = at;
            const std::optional<Record> root = record_in(d, kRootRecordAt, at, joliet);
            if (!root || !root->directory())
                bad(at + kRootRecordAt, "no root directory");
            t.root = *root;
            t.root.at = at + kRootRecordAt;
            if (primary) {
                trees.insert(trees.begin(), t);
                sectors = le32(d, kVolumeSizeAt);
            } else {
                trees.push_back(t);
            }
        }
        if (trees.empty() || trees.front().joliet)
            bad(kFirstDescriptor * kSectorSize, "not an ISO-9660 volume");
    }
};

Volume::Volume(const std::filesystem::path& path, Access access) : impl_(std::make_unique<Impl>(path, access)) {
    impl_->read_descriptors();
}
Volume::Volume(Volume&&) noexcept = default;
Volume& Volume::operator=(Volume&&) noexcept = default;
Volume::~Volume() = default;

bool Volume::has_joliet() const { return impl_->trees.size() > 1; }
std::uint32_t Volume::sector_count() const { return impl_->sectors; }

namespace {
const Tree& tree_of(const std::vector<Tree>& trees, Names which) {
    if (which == Names::Joliet) {
        if (trees.size() < 2)
            throw UsageError("the volume has no Joliet tree");
        return trees[1];
    }
    return trees.front();
}
}  // namespace

std::vector<Entry> Volume::list(std::string_view directory, Names tree) const {
    const Tree& t = tree_of(impl_->trees, tree);
    std::vector<Entry> out;
    for (const Record& r : impl_->children(t, impl_->find_or_throw(t, directory)))
        out.push_back({r.name, r.directory(), r.size});
    return out;
}

Bytes Volume::read(std::string_view file, Names tree) const {
    const Record r = impl_->find_or_throw(tree_of(impl_->trees, tree), file);
    if (r.directory())
        throw UsageError(std::format("{} is a directory", file));
    if (r.flags & kFlagMultiExtent)
        throw UnsupportedError(kReader, r.at, "a file in several extents");
    return impl_->read_at(std::uint64_t{r.extent} * kSectorSize, r.size);
}

void Volume::replace(std::string_view file, ByteView data) {
    Impl& v = *impl_;
    if (v.access != Access::ReadWrite)
        throw UsageError("the volume is open read-only");
    if (data.size() > 0xFFFFFFFFu - kSectorSize)
        throw UnsupportedError(kReader, 0, "a file of 4 GB or more");

    // The file's record in every tree, found before anything is written: a volume that lacks the file in one of its
    // trees is left as it was.
    std::vector<Record> records;
    for (const Tree& t : v.trees) {
        const Record r = v.find_or_throw(t, file);
        if (r.directory())
            throw UsageError(std::format("{} is a directory", file));
        if (r.flags & kFlagMultiExtent)
            throw UnsupportedError(kReader, r.at, "a file in several extents");
        records.push_back(r);
    }

    // The data goes after the last sector of the image (the image is padded out to whole sectors first).
    const std::uint64_t end = v.file.size();
    const std::uint64_t sectors_now = (end + kSectorSize - 1) / kSectorSize;
    const std::uint64_t data_sectors = (data.size() + kSectorSize - 1) / kSectorSize;
    const std::uint64_t total = sectors_now + data_sectors;
    if (total > 0xFFFFFFFFu)
        throw UnsupportedError(kReader, 0, "a volume of 2^32 sectors");
    Bytes padded(data.begin(), data.end());
    padded.resize(static_cast<std::size_t>(data_sectors * kSectorSize), 0);
    if (end != sectors_now * kSectorSize)
        v.file.write_at(sectors_now * kSectorSize - 1, Bytes{0});   // (grows the file to a whole sector)
    v.file.write_at(sectors_now * kSectorSize, padded);

    // The records and the descriptors, rewritten whole.
    for (const Record& r : records) {
        Bytes rec = v.read_at(r.at, 34);
        put_both32(rec, 2, static_cast<std::uint32_t>(sectors_now));
        put_both32(rec, 10, static_cast<std::uint32_t>(data.size()));
        v.file.write_at(r.at, rec);
    }
    for (const Tree& t : v.trees) {
        Bytes size(8);
        put_both32(size, 0, static_cast<std::uint32_t>(total));
        v.file.write_at(t.descriptor_at + kVolumeSizeAt, size);
    }
    v.file.flush();
    v.sectors = static_cast<std::uint32_t>(total);
}

}  // namespace cb::formats::iso9660
