#include "formats/iso9660.hpp"

#include <algorithm>
#include <format>
#include <optional>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/random_file.hpp"
#include "core/text.hpp"
#include "formats/disc_image.hpp"
#include "formats/format_builtin.hpp"

namespace cb::formats::iso9660 {

namespace eng = cb::formats::engine;

namespace {

constexpr std::string_view kReader = "iso9660";
constexpr std::size_t kFirstDescriptor = 16;   // sectors 0..15 are the system area
constexpr std::size_t kMaxDescriptors = 32;    // a volume has a handful; this stops a damaged one
constexpr std::size_t kRootRecordAt = 156;     // in a volume descriptor
constexpr std::size_t kVolumeSizeAt = 80;      // in a volume descriptor (both-endian, 4 bytes)
constexpr std::uint8_t kFlagDirectory = 0x02;
constexpr std::uint8_t kFlagMultiExtent = 0x80;

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
    std::array<std::uint8_t, 7> recorded{};
    std::string name;            // decoded, without ";1" and without a trailing "."
    bool directory() const { return (flags & kFlagDirectory) != 0; }
};

// One tree of the volume: the primary one, or the Joliet one.
struct Tree {
    bool joliet = false;
    std::uint64_t descriptor_at = 0;   // where its volume descriptor is
    std::string system, volume, created;   // its descriptor's fields as stored (the volume identifier is UCS-2 in a Joliet one)
    std::uint32_t block_size = 0;
    Record root;
};

// A descriptor's sector, or a directory's bytes, as iso9660.fmt reads it.
eng::Tree iso_tree(ByteView bytes, std::string_view root) {
    eng::ReadOptions options;
    options.root = std::string(root);
    options.keep_source = false;   // only read from
    return eng::builtin().read("iso9660", bytes, options);
}

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
    // A cooked image file is read (and written) directly; a raw one, a .cue sheet's or one in memory through its sectors.
    std::optional<fs::RandomFile> file;
    std::optional<disc::Sectors> disc;
    std::vector<Tree> trees;   // the primary tree first, then the Joliet one if there is one
    std::uint32_t sectors = 0;
    std::optional<std::uint32_t> boot_catalog;   // the El Torito boot catalog's sector

    Impl(const std::filesystem::path& p, Access a) : path(p), access(a) {
        if (a == Access::Read) {
            disc::Sectors s = disc::Sectors::open(p);
            if (s.raw() || s.file() != p) disc = std::move(s);
        }
        if (!disc) file.emplace(p, a == Access::Read ? fs::FileMode::Read : fs::FileMode::ReadWrite);
    }
    explicit Impl(disc::Sectors s) : access(Access::Read), disc(std::move(s)) { path = disc->file(); }

    Bytes bytes_at(std::uint64_t offset, std::size_t count) const { return disc ? disc->read_at(offset, count) : file->read_at(offset, count); }

    [[noreturn]] void bad(std::uint64_t offset, const std::string& what) const { throw FormatError(kReader, offset, what); }

    Bytes read_at(std::uint64_t offset, std::size_t count) const {
        Bytes out = bytes_at(offset, count);
        if (out.size() != count)
            throw TruncatedError(kReader, offset, count, out.size());
        return out;
    }

    // The record of a slot of a directory's bytes (`size` of them; `dir_at`: where they are in the image); nullopt for the zero that pads a sector.
    std::optional<Record> record_in(const eng::Node& slot, std::size_t size, std::uint64_t dir_at, bool joliet) const {
        const std::size_t at = slot.offset, length = slot.u("length");
        if (length == 0)
            return std::nullopt;
        const eng::Node* body = slot.child("body");
        if (length < 34 || at + length > size || !body || 33u + body->u("name_length") > length)
            bad(dir_at + at, "a directory record that does not fit");
        Record r;
        r.at = dir_at + at;
        r.extent = static_cast<std::uint32_t>(body->u("extent") + body->u("ext_attr"));   // past the extended attribute record
        r.size = static_cast<std::uint32_t>(body->u("size"));
        r.flags = static_cast<std::uint8_t>(body->u("flags"));
        const std::string& recorded = body->get("recorded").text;
        std::copy_n(recorded.begin(), r.recorded.size(), r.recorded.begin());
        const ByteView raw = as_bytes(body->get("name").text);
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
        const eng::Tree records = iso_tree(bytes, "Directory");
        std::vector<Record> out;
        for (const eng::Node& slot : records.root.get("slots").children) {
            const std::optional<Record> r = record_in(slot, bytes.size(), at, tree.joliet);
            if (r && r->name != "." && r->name != "..")
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
            const Bytes d = bytes_at(at, kSectorSize);
            if (d.size() < kSectorSize)
                break;
            const eng::Tree tree = iso_tree(d, "Descriptor");
            const eng::Node& v = tree.root;
            if (v.get("standard").text != "CD001")
                break;
            const std::uint64_t type = v.u("type");
            if (type == 255)
                break;   // the terminator
            if (type == 0 && v.get("boot_system").text == "EL TORITO SPECIFICATION") boot_catalog = static_cast<std::uint32_t>(v.u("boot_catalog"));
            const bool primary = type == 1;
            // a supplementary descriptor is Joliet's when its escape sequence is one of UCS-2's three levels
            const std::string& escapes = v.get("escapes").text;
            const bool joliet = type == 2 && escapes[0] == '%' && escapes[1] == '/' && (escapes[2] == '@' || escapes[2] == 'C' || escapes[2] == 'E');
            if (!primary && !joliet)
                continue;
            Tree t;
            t.joliet = joliet;
            t.descriptor_at = at;
            t.system = v.get("system").text;
            t.volume = v.get("volume").text;
            t.created = v.get("created").text;
            t.block_size = static_cast<std::uint32_t>(v.u("block_size"));
            const std::optional<Record> root = record_in(v.get("root"), kSectorSize, at, joliet);
            if (!root || !root->directory())
                bad(at + kRootRecordAt, "no root directory");
            t.root = *root;
            t.root.at = at + kRootRecordAt;
            if (primary) {
                sectors = static_cast<std::uint32_t>(v.u("blocks"));
                trees.insert(trees.begin(), std::move(t));
            } else {
                trees.push_back(std::move(t));
            }
        }
        if (trees.empty() || trees.front().joliet)
            bad(kFirstDescriptor * kSectorSize, "not an ISO-9660 volume");
    }
};

Volume::Volume(const std::filesystem::path& path, Access access) : impl_(std::make_unique<Impl>(path, access)) {
    impl_->read_descriptors();
}
Volume::Volume(disc::Sectors sectors) : impl_(std::make_unique<Impl>(std::move(sectors))) { impl_->read_descriptors(); }
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

namespace {
// The entries of a directory's records: a file in several extents (consecutive records of one name) is one entry listing each.
std::vector<Entry> entries_of(const std::vector<Record>& records) {
    std::vector<Entry> out;
    for (const Record& r : records) {
        if (!out.empty() && out.back().name == r.name && !r.directory() && !out.back().directory) {   // a further extent of the same file
            out.back().extents.push_back({r.extent, r.size});
            out.back().size += r.size;
            continue;
        }
        out.push_back({r.name, r.directory(), r.size, r.extent, r.recorded, {Extent{r.extent, r.size}}});
    }
    return out;
}
}  // namespace

std::vector<Entry> Volume::list(std::string_view directory, Names tree) const {
    const Tree& t = tree_of(impl_->trees, tree);
    return entries_of(impl_->children(t, impl_->find_or_throw(t, directory)));
}

std::vector<Entry> Volume::list(const Entry& directory, Names tree) const {
    Record r;
    r.name = directory.name;
    r.extent = directory.extent;
    r.size = directory.size;
    r.flags = kFlagDirectory;
    return entries_of(impl_->children(tree_of(impl_->trees, tree), r));
}

Entry Volume::root(Names tree) const {
    const Record& r = tree_of(impl_->trees, tree).root;
    return {"/", true, r.size, r.extent, r.recorded, {}};
}

Bytes Volume::read(const Entry& entry) const {
    Bytes out;
    if (entry.directory) return out;
    for (const Extent& x : entry.extents) {
        const Bytes part = impl_->bytes_at(std::uint64_t{x.sector} * kSectorSize, x.size);
        if (part.size() != x.size) throw TruncatedError(kReader, std::uint64_t{x.sector} * kSectorSize, x.size, part.size());
        append(out, part);
    }
    return out;
}

Bytes Volume::read(std::string_view file, Names tree) const {
    const Tree& t = tree_of(impl_->trees, tree);
    const std::string_view trimmed = file.size() > 1 && file.back() == '/' ? file.substr(0, file.size() - 1) : file;
    const std::size_t slash = trimmed.rfind('/');
    const std::string_view parent = slash == std::string_view::npos ? std::string_view("/") : trimmed.substr(0, slash == 0 ? 1 : slash);
    const std::string_view leaf = without_version(slash == std::string_view::npos ? trimmed : trimmed.substr(slash + 1));
    impl_->find_or_throw(t, file);   // NotFoundError for a path that is not there
    for (const Entry& e : list(parent, tree)) {
        if (!text::iequals(e.name, leaf)) continue;
        if (e.directory) throw UsageError(std::format("{} is a directory", file));
        return read(e);
    }
    throw NotFoundError(impl_->path, 2, std::format("{} is not in the volume", file));
}

Info Volume::info(Names tree) const {
    const Impl& v = *impl_;
    const Tree& t = tree_of(v.trees, tree);
    const Tree& p = v.trees.front();
    const auto text_of = [](std::string_view field) {
        std::string s(field);
        while (!s.empty() && (s.back() == ' ' || s.back() == 0)) s.pop_back();
        return s;
    };
    Info i;
    i.system = text_of(p.system);
    i.label = text_of(p.volume);
    i.created = text_of(p.created);
    if (t.joliet) {
        std::string label = decode_name(as_bytes(t.volume), true);
        while (!label.empty() && label.back() == ' ') label.pop_back();
        i.tree_label = label;
    } else {
        i.tree_label = i.label;
    }
    i.blocks = v.sectors;
    i.block_size = p.block_size != 0 ? p.block_size : 2048;
    i.has_joliet = v.trees.size() > 1;
    i.root_sector = t.root.extent;
    i.root_size = t.root.size;
    i.boot_catalog = v.boot_catalog;
    return i;
}

Bytes Volume::read_sectors(std::uint64_t first, std::uint64_t count) const {
    return impl_->bytes_at(first * kSectorSize, static_cast<std::size_t>(count * kSectorSize));
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
    const std::uint64_t end = v.file->size();
    const std::uint64_t sectors_now = (end + kSectorSize - 1) / kSectorSize;
    const std::uint64_t data_sectors = (data.size() + kSectorSize - 1) / kSectorSize;
    const std::uint64_t total = sectors_now + data_sectors;
    if (total > 0xFFFFFFFFu)
        throw UnsupportedError(kReader, 0, "a volume of 2^32 sectors");
    Bytes padded(data.begin(), data.end());
    padded.resize(static_cast<std::size_t>(data_sectors * kSectorSize), 0);
    if (end != sectors_now * kSectorSize)
        v.file->write_at(sectors_now * kSectorSize - 1, Bytes{0});   // (grows the file to a whole sector)
    v.file->write_at(sectors_now * kSectorSize, padded);

    // The records and the descriptors, rewritten whole.
    for (const Record& r : records) {
        Bytes rec = v.read_at(r.at, 34);
        rec[1] = 0;   // (the new data has no extended attribute record)
        put_both32(rec, 2, static_cast<std::uint32_t>(sectors_now));
        put_both32(rec, 10, static_cast<std::uint32_t>(data.size()));
        v.file->write_at(r.at, rec);
    }
    for (const Tree& t : v.trees) {
        Bytes size(8);
        put_both32(size, 0, static_cast<std::uint32_t>(total));
        v.file->write_at(t.descriptor_at + kVolumeSizeAt, size);
    }
    v.file->flush();
    v.sectors = static_cast<std::uint32_t>(total);
}

}  // namespace cb::formats::iso9660
