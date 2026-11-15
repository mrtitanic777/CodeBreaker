#include "cb/build/disc_image.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <format>
#include <fstream>
#include <map>
#include <memory>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/random_file.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/media.hpp"

namespace cb::build::disc_image {

namespace {

namespace stdfs = std::filesystem;

constexpr std::size_t kMaxDepth = 8;                // folder levels ISO-9660 allows
constexpr std::size_t kMaxFolders = 0xFFFF;         // the path table numbers a parent in 16 bits
constexpr std::uint64_t kMaxFileSize = 1ull << 32;  // a record states a size in 32 bits
constexpr std::size_t kCopyPiece = 1u << 20;
// El Torito media type by boot image size: 1.2 MB, 1.44 MB and 2.88 MB floppies are emulated; any other size is loaded as it is ("no
// emulation").
struct FloppySize {
    std::uint64_t size;
    std::uint8_t media;
};
constexpr FloppySize kFloppySizes[] = {{1228800, 1}, {1474560, 2}, {2949120, 3}};

using Stamp = std::array<std::uint8_t, 7>;   // a directory record's date: year-1900, month, day, hour, minute, second, zone

struct Node {
    stdfs::path source;
    std::pair<std::string, std::string> key;   // the level-1 (base, extension); a folder's extension is empty
    std::string ident;                         // "README.;1" for a file, "SETUP" for a folder
    bool is_dir = false;
    std::vector<Node*> children;
    Node* parent = nullptr;
    std::uint32_t number = 0;                  // a folder's place in the path table (1-based)
    std::uint32_t lba = 0;
    std::uint64_t size = 0;
    Stamp stamp{};
};

Stamp stamp_of(const stdfs::path& path) {
    using namespace std::chrono;
    std::error_code ec;
    const auto modified = cb::fs::last_write_time(path, ec);
    if (ec) throw IoError(path, ec.value(), "cannot read its time");
    const auto seconds = floor<std::chrono::seconds>(clock_cast<system_clock>(modified));
    const auto days = floor<std::chrono::days>(seconds);
    const year_month_day date{days};
    const hh_mm_ss<std::chrono::seconds> time{seconds - days};
    return Stamp{static_cast<std::uint8_t>(std::clamp(static_cast<int>(date.year()) - 1900, 0, 255)), static_cast<std::uint8_t>(static_cast<unsigned>(date.month())),
                 static_cast<std::uint8_t>(static_cast<unsigned>(date.day())), static_cast<std::uint8_t>(time.hours().count()),
                 static_cast<std::uint8_t>(time.minutes().count()), static_cast<std::uint8_t>(time.seconds().count()), 0};
}

// The two forms ISO 9660 stores a number in: little-endian then big-endian.
void both16(Bytes& out, std::uint16_t v) {
    out.insert(out.end(), {static_cast<std::uint8_t>(v), static_cast<std::uint8_t>(v >> 8), static_cast<std::uint8_t>(v >> 8), static_cast<std::uint8_t>(v)});
}
void both32(Bytes& out, std::uint32_t v) {
    for (int k = 0; k < 4; ++k) out.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
    for (int k = 3; k >= 0; --k) out.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
}
void put_le32(Bytes& out, std::uint32_t v) {
    for (int k = 0; k < 4; ++k) out.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
}
void put_be32(Bytes& out, std::uint32_t v) {
    for (int k = 3; k >= 0; --k) out.push_back(static_cast<std::uint8_t>(v >> (8 * k)));
}

std::uint64_t sectors(std::uint64_t size) { return (size + kSectorSize - 1) / kSectorSize; }

// A name's characters in the set ISO 9660 level 1 allows, each character that is not one becoming '_', cut to `limit` characters.
std::string clean(std::string_view name, std::size_t limit) {
    std::string out;
    std::size_t count = 0;
    for (std::size_t i = 0; i < name.size() && count < limit; ++count) {
        const auto lead = static_cast<unsigned char>(name[i]);
        const std::size_t length = lead < 0x80 ? 1 : (lead >> 5) == 6 ? 2 : (lead >> 4) == 14 ? 3 : (lead >> 3) == 30 ? 4 : 1;
        const char c = static_cast<char>(std::toupper(lead));
        out += (length == 1 && ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')) ? c : '_';
        i += length;
    }
    return out;
}

// One directory record: length, the extent, the size, the date, the flags and the identifier, padded to an even length.
Bytes record(std::string_view ident, std::uint32_t lba, std::uint32_t size, bool is_dir, const Stamp& stamp) {
    const std::size_t length = 33 + ident.size() + (1 - ident.size() % 2);
    Bytes r;
    r.push_back(static_cast<std::uint8_t>(length));
    r.push_back(0);
    both32(r, lba);
    both32(r, size);
    r.insert(r.end(), stamp.begin(), stamp.end());
    r.push_back(is_dir ? 2 : 0);
    r.insert(r.end(), {0, 0});   // file unit size and interleave gap
    both16(r, 1);                // volume sequence number
    r.push_back(static_cast<std::uint8_t>(ident.size()));
    r.insert(r.end(), ident.begin(), ident.end());
    r.resize(length, 0);
    return r;
}

// The bytes of one folder's directory: "." and "..", then its children. A record never crosses a sector boundary; the whole is padded to
// full sectors.
Bytes directory_bytes(const Node& node) {
    const auto rec = [](std::string_view ident, const Node& n) { return record(ident, n.lba, static_cast<std::uint32_t>(n.size), n.is_dir, n.stamp); };
    Bytes out = rec(std::string_view("\0", 1), node);
    append(out, rec(std::string_view("\1", 1), *node.parent));
    for (const Node* child : node.children) {
        const Bytes r = rec(child->ident, *child);
        if (out.size() % kSectorSize + r.size() > kSectorSize) out.resize(out.size() + (kSectorSize - out.size() % kSectorSize), 0);
        append(out, r);
    }
    out.resize((out.size() + kSectorSize - 1) / kSectorSize * kSectorSize, 0);
    return out;
}

Bytes path_table(const std::vector<Node*>& folders, bool big_endian) {
    Bytes out;
    for (const Node* node : folders) {
        const std::string ident = node->ident.empty() ? std::string("\0", 1) : node->ident;
        out.push_back(static_cast<std::uint8_t>(ident.size()));
        out.push_back(0);
        if (big_endian) {
            put_be32(out, node->lba);
            out.push_back(static_cast<std::uint8_t>(node->parent->number >> 8));
            out.push_back(static_cast<std::uint8_t>(node->parent->number));
        } else {
            put_le32(out, node->lba);
            out.push_back(static_cast<std::uint8_t>(node->parent->number));
            out.push_back(static_cast<std::uint8_t>(node->parent->number >> 8));
        }
        out.insert(out.end(), ident.begin(), ident.end());
        if (ident.size() % 2 != 0) out.push_back(0);
    }
    return out;
}

// The El Torito boot catalog: a validation entry (its 16-bit words sum to zero) and the default entry naming the boot image.
Bytes boot_catalog(std::uint32_t image_lba, std::uint64_t image_size) {
    Bytes check(32, 0);
    check[0] = 1;   // header; platform 0 = x86
    constexpr std::string_view id = "CODEBREAKER";
    std::copy(id.begin(), id.end(), check.begin() + 4);
    check[30] = 0x55;
    check[31] = 0xAA;
    std::uint32_t total = 0;
    for (std::size_t i = 0; i < 32; i += 2) total += check[i] | (check[i + 1] << 8);
    const auto fix = static_cast<std::uint16_t>(0u - total);
    check[28] = static_cast<std::uint8_t>(fix);
    check[29] = static_cast<std::uint8_t>(fix >> 8);
    std::uint8_t media = 0;
    for (const FloppySize& floppy : kFloppySizes)
        if (floppy.size == image_size) media = floppy.media;
    const std::uint32_t count = media != 0 ? 1 : static_cast<std::uint32_t>(std::min<std::uint64_t>(0xFFFF, (image_size + 511) / 512));   // 512-byte sectors to load
    Bytes entry = {0x88, media, 0, 0, 0, 0, static_cast<std::uint8_t>(count), static_cast<std::uint8_t>(count >> 8)};
    put_le32(entry, image_lba);
    Bytes out = check;
    append(out, entry);
    out.resize(kSectorSize, 0);
    return out;
}

Bytes descriptor(std::uint8_t kind, ByteView body = {}) {
    Bytes out = {kind, 'C', 'D', '0', '0', '1', 1};
    append(out, body);
    out.resize(kSectorSize, 0);
    return out;
}

// Reads the tree to write; the children of every folder are in the order the standard asks of directory records (by name, then
// extension). The clashes found are described in `clashes`.
class Scan {
public:
    Node* root(const stdfs::path& folder) {
        Node* node = make(folder, {"", ""}, true, nullptr);
        node->parent = node;
        fill(*node, 1);
        return node;
    }
    std::vector<std::string> clashes;

private:
    std::vector<std::unique_ptr<Node>> nodes_;

    Node* make(const stdfs::path& source, std::pair<std::string, std::string> key, bool is_dir, Node* parent) {
        auto node = std::make_unique<Node>();
        node->source = source;
        node->key = std::move(key);
        node->is_dir = is_dir;
        node->parent = parent;
        node->ident = is_dir ? node->key.first : node->key.first + "." + node->key.second + ";1";
        std::error_code ec;
        node->size = is_dir ? 0 : stdfs::file_size(source, ec);
        if (ec) throw IoError(source, ec.value(), "cannot read its size");
        node->stamp = stamp_of(source);
        nodes_.push_back(std::move(node));
        return nodes_.back().get();
    }

    void fill(Node& node, std::size_t depth) {
        if (depth > kMaxDepth) throw DiscError(std::format("{}: folders nest deeper than the {} levels ISO-9660 allows", fs::to_text(node.source), kMaxDepth));
        struct Entry {
            std::string name;
            stdfs::path path;
            bool is_dir;
        };
        std::vector<Entry> entries;
        for (const auto& item : stdfs::directory_iterator(node.source)) {
            std::error_code ec;
            const bool is_dir = item.is_directory(ec);
            if (!is_dir && !item.is_regular_file(ec)) continue;
            entries.push_back({fs::to_text(item.path().filename()), item.path(), is_dir});
        }
        std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) { return a.name < b.name; });

        // the names that end up the same: (is a folder, level-1 key) -> the names, in the order the keys were first met
        std::vector<std::pair<std::pair<bool, std::pair<std::string, std::string>>, std::vector<std::string>>> seen;
        for (const Entry& entry : entries) {
            auto key = level1_name(entry.name, entry.is_dir);
            const auto id = std::make_pair(entry.is_dir, key);
            const auto found = std::find_if(seen.begin(), seen.end(), [&](const auto& s) { return s.first == id; });
            if (found == seen.end()) seen.push_back({id, {entry.name}});
            else found->second.push_back(entry.name);
            node.children.push_back(make(entry.path, std::move(key), entry.is_dir, &node));
        }
        for (const auto& [id, names] : seen) {
            if (names.size() < 2) continue;
            clashes.push_back(std::format("{}: {} all become {}", fs::to_text(node.source), text::join(names, ", "),
                                          id.first ? id.second.first : id.second.first + "." + id.second.second));
        }
        std::stable_sort(node.children.begin(), node.children.end(), [](const Node* a, const Node* b) { return a->key < b->key; });
        for (Node* child : node.children)
            if (child->is_dir) fill(*child, depth + 1);
    }
};

// The folders in path-table order -- by level, then by the parent's number, then by name -- each given its 1-based number.
std::vector<Node*> numbered(Node* root) {
    std::vector<Node*> order;
    std::vector<Node*> level = {root};
    while (!level.empty()) {
        order.insert(order.end(), level.begin(), level.end());
        std::vector<Node*> next;
        for (Node* node : level)
            for (Node* child : node->children)
                if (child->is_dir) next.push_back(child);
        level = std::move(next);
    }
    if (order.size() > kMaxFolders) throw DiscError("more than 65535 folders");
    for (std::size_t i = 0; i < order.size(); ++i) order[i]->number = static_cast<std::uint32_t>(i + 1);
    return order;
}

// "YYYYMMDDHHMMSS00" and a zero offset: when the image was written.
Bytes volume_time() {
    using namespace std::chrono;
    const auto seconds = floor<std::chrono::seconds>(system_clock::now());
    const auto days = floor<std::chrono::days>(seconds);
    const year_month_day date{days};
    const hh_mm_ss<std::chrono::seconds> time{seconds - days};
    const std::string text = std::format("{:04}{:02}{:02}{:02}{:02}{:02}00", static_cast<int>(date.year()), static_cast<unsigned>(date.month()),
                                         static_cast<unsigned>(date.day()), time.hours().count(), time.minutes().count(), time.seconds().count());
    Bytes out(text.begin(), text.end());
    out.push_back(0);
    return out;
}

// A file to put on the disc: where it is and where it goes.
struct Placed {
    stdfs::path source;
    std::uint64_t size = 0;
    std::uint32_t lba = 0;
    Node* node = nullptr;   // the directory entry that states the file's extent (none for the boot image, which has no entry)
};

}  // namespace

std::pair<std::string, std::string> level1_name(std::string_view name, bool is_dir) {
    if (is_dir) {
        std::string base = clean(name, 8);
        return {base.empty() ? "_" : base, ""};
    }
    const std::size_t dot = name.rfind('.');
    const std::string_view base = dot == std::string_view::npos ? name : name.substr(0, dot);
    const std::string_view extension = dot == std::string_view::npos ? std::string_view() : name.substr(dot + 1);
    std::string cleaned = clean(base, 8);
    return {cleaned.empty() ? "_" : cleaned, clean(extension, 3)};
}

std::uint64_t write(const stdfs::path& folder, const stdfs::path& out, const Options& options) {
    std::error_code ec;
    if (!stdfs::is_directory(folder, ec)) throw DiscError(fs::to_text(folder) + " is not a folder");
    Scan scan;
    Node* root = scan.root(folder);
    if (!scan.clashes.empty()) throw DiscError("names that clash once cut to 8.3:\n  " + text::join(scan.clashes, "\n  "));
    const std::vector<Node*> folders = numbered(root);
    std::vector<Placed> files;
    std::vector<std::string> too_big;
    for (Node* node : folders)
        for (Node* child : node->children)
            if (!child->is_dir) {
                files.push_back({child->source, child->size, 0, child});
                if (child->size >= kMaxFileSize) too_big.push_back(fs::to_text(child->source));
            }
    if (!too_big.empty()) throw DiscError("too large for one ISO-9660 file (4 GB): " + text::join(too_big, ", "));

    // layout: 16 system sectors, descriptors, both path tables, boot, directories, files
    const bool bootable = options.boot_image.has_value() && !options.boot_image->empty();
    std::uint64_t boot_size = 0;
    if (bootable) {
        boot_size = stdfs::file_size(*options.boot_image, ec);
        if (ec) throw IoError(*options.boot_image, ec.value(), "cannot read its size");
    }
    std::uint64_t lba = 17 + (bootable ? 1 : 0) + 1;
    const std::uint64_t table_size = path_table(folders, false).size();
    const std::uint64_t little_at = lba;
    const std::uint64_t big_at = lba + sectors(table_size);
    lba = big_at + sectors(table_size);
    const std::uint64_t catalog_at = lba;
    const std::uint64_t boot_at = lba + 1;
    if (bootable) lba += 1 + sectors(boot_size);
    for (Node* node : folders) {
        node->size = directory_bytes(*node).size();
        node->lba = static_cast<std::uint32_t>(lba);
        lba += node->size / kSectorSize;
    }
    for (Placed& file : files) {
        file.lba = static_cast<std::uint32_t>(lba);
        file.node->lba = file.lba;
        lba += sectors(file.size);
    }
    const std::uint64_t total = lba;

    const Bytes now = volume_time();
    Bytes unset(16, '0');
    unset.push_back(0);
    Bytes volume;   // the primary volume descriptor's body: what follows the seven bytes of type, identifier and version
    volume.push_back(0);
    volume.insert(volume.end(), 32, ' ');   // system identifier
    std::string id = clean(options.volume_id, 32);
    if (id.empty()) id = "CODEBREAKER";
    id.resize(32, ' ');
    volume.insert(volume.end(), id.begin(), id.end());
    volume.resize(73, 0);
    both32(volume, static_cast<std::uint32_t>(total));
    volume.resize(113, 0);
    both16(volume, 1);   // volume set size
    both16(volume, 1);   // volume sequence number
    both16(volume, static_cast<std::uint16_t>(kSectorSize));
    both32(volume, static_cast<std::uint32_t>(table_size));
    put_le32(volume, static_cast<std::uint32_t>(little_at));
    volume.resize(141, 0);
    put_be32(volume, static_cast<std::uint32_t>(big_at));
    volume.resize(149, 0);
    append(volume, record(std::string_view("\0", 1), root->lba, static_cast<std::uint32_t>(root->size), true, root->stamp));
    volume.insert(volume.end(), 623, ' ');   // set, publisher, preparer, application, copyright, abstract and bibliographic names
    append(volume, now);
    append(volume, now);
    append(volume, unset);
    append(volume, unset);
    volume.push_back(1);   // file structure version

    fs::RandomFile image(out, fs::FileMode::Create);
    const auto put = [&](std::uint64_t at_sector, ByteView data) { image.write_at(at_sector * kSectorSize, data); };
    put(16, descriptor(1, volume));
    if (bootable) {
        Bytes body(64, 0);
        constexpr std::string_view torito = "EL TORITO SPECIFICATION";
        std::copy(torito.begin(), torito.end(), body.begin());
        put_le32(body, static_cast<std::uint32_t>(catalog_at));
        put(17, descriptor(0, body));
        put(catalog_at, boot_catalog(static_cast<std::uint32_t>(boot_at), boot_size));
        files.insert(files.begin(), Placed{*options.boot_image, boot_size, static_cast<std::uint32_t>(boot_at), nullptr});
    }
    put(17 + (bootable ? 1 : 0), descriptor(255));
    put(little_at, path_table(folders, false));
    put(big_at, path_table(folders, true));
    for (const Node* node : folders) put(node->lba, directory_bytes(*node));
    for (const Placed& file : files) {
        std::ifstream in(file.source, std::ios::binary);
        if (!in) throw IoError(file.source, 0, "cannot be opened");
        Bytes piece(kCopyPiece);
        std::uint64_t at = std::uint64_t{file.lba} * kSectorSize, left = file.size;
        while (left != 0) {
            in.read(reinterpret_cast<char*>(piece.data()), static_cast<std::streamsize>(std::min<std::uint64_t>(left, kCopyPiece)));
            const auto got = static_cast<std::size_t>(in.gcount());
            if (got == 0) throw DiscError(fs::to_text(file.source) + " shrank while it was being copied");
            image.write_at(at, ByteView(piece).first(got));
            at += got;
            left -= got;
        }
    }
    if (image.size() < total * kSectorSize) image.write_at(total * kSectorSize - 1, Bytes{0});
    image.flush();
    return total * kSectorSize;
}

cli::Command command(const Environment&) {
    cli::Command c;
    c.name = "disc";
    c.summary = "write an ISO 9660 CD image from a folder, or list or extract one";
    c.area = "Build";
    c.description =
        "  disc <folder> <out.iso> [--volume NAME] [--boot IMG]\n"
        "  disc --list <image.iso>\n"
        "  disc --extract <image.iso> <outdir>";
    c.params = {
        cli::Positional{"input", "the folder to write, or with --list or --extract the image"},
        cli::Positional{"output", "the image to write, or with --extract the folder to write the files to", false},
        cli::Option{"--volume", "", "the volume's name (default CODEBREAKER)", cli::Arg::Value, "NAME"},
        cli::Option{"--boot", "", "a floppy image to make the disc bootable with", cli::Arg::Value, "IMG"},
        cli::Option{"--list", "", "list the volumes of an image with their boot fields", cli::Arg::Flag},
        cli::Option{"--extract", "", "write the files of an image under a folder", cli::Arg::Flag},
    };
    c.handler = [](const cli::Args& a) -> int {
        const bool list = a.flag("list"), extract = a.flag("extract");
        const std::string first = a.required("input");
        const std::optional<std::string> second = a.value("output");
        try {
            if (list && !second && !extract) {
                for (const auto& volume : formats::media::volumes(fs::from_text(first))) out_text(formats::media::listing_text(*volume, true, true));
                return kExitOk;
            }
            if (extract && second && !list) {
                out("extracted {} files to {}\n", formats::media::extract(fs::from_text(first), fs::from_text(*second)), *second);
                return kExitOk;
            }
            if (second && !list && !extract) {
                Options options;
                options.volume_id = a.value_or("volume", "CODEBREAKER");
                if (const auto boot = a.value("boot")) options.boot_image = fs::from_text(*boot);
                const std::uint64_t size = write(fs::from_text(first), fs::from_text(*second), options);
                out("wrote {}: {} bytes{}\n", *second, size, a.value("boot").has_value() ? " (bootable)" : "");
                return kExitOk;
            }
        } catch (const DiscError& problem) {
            err("disc: {}\n", problem.what());
            return kExitFailed;
        } catch (const IoError& problem) {
            err("disc: {}\n", problem.what());
            return kExitFailed;
        }
        throw UsageError("disc: give <folder> <out.iso>, or --list <image>, or --extract <image> <outdir>");
    };
    return c;
}

}  // namespace cb::build::disc_image
