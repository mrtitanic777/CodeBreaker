#include "cb/formats/xbox/volumes.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <unordered_map>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/random_file.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/xbox/ciso.hpp"
#include "cb/formats/xbox/device.hpp"
#include "cb/formats/xbox/fatx.hpp"
#include "cb/formats/xbox/gdf.hpp"
#include "cb/formats/xbox/heavyiron.hpp"
#include "cb/formats/xbox/nand.hpp"
#include "cb/formats/xbox/pck.hpp"
#include "cb/formats/xbox/stfs.hpp"
#include "cb/formats/xbox/xzp.hpp"
#include "cb/formats/xbox/zar.hpp"

namespace cb::formats::xbox::volumes {

namespace volumes_impl {

namespace stdfs = std::filesystem;
using media::BootField;
using media::VolumeKind;

BootField text_field(std::string name, std::string value) { return {std::move(name), std::move(value), std::nullopt}; }
BootField count_field(std::string name, std::uint64_t value) {
    return {std::move(name), std::to_string(value), static_cast<std::int64_t>(value)};
}
std::string hex_text(std::int64_t v) { return std::format("0x{:X}", v); }   // (a negative base reads "0x-5")

std::string leaf(const stdfs::path& p) { return fs::to_text(p.filename()); }

// A volume whose files are a list of (path, size) and a way to read one by its path.
class ListedVolume final : public media::Volume {
public:
    using Reader = std::function<Bytes(const std::string& path)>;
    ListedVolume(VolumeKind kind, std::string label, std::string where, std::vector<BootField> boot,
                 const std::vector<std::pair<std::string, std::uint64_t>>& listing, Reader reader)
        : kind_(kind), reader_(std::move(reader)) {
        label_ = std::move(label);
        where_ = std::move(where);
        boot_ = std::move(boot);
        for (const auto& [path, size] : listing) {
            media::Entry e;
            e.path = path;
            e.short_name = path;
            e.size = size;
            entries_.push_back(std::move(e));
        }
    }
    VolumeKind kind() const override { return kind_; }
    const std::vector<media::Entry>& files() override { return entries_; }
    Bytes read(const media::Entry& entry) override { return entry.is_dir ? Bytes{} : reader_(entry.path); }

private:
    VolumeKind kind_;
    Reader reader_;
    std::vector<media::Entry> entries_;
};

using Listing = std::vector<std::pair<std::string, std::uint64_t>>;

// A name met twice is listed once, at its first place, with the later data (what a name-keyed table does).
template <class V>
void put_by_name(std::vector<std::pair<std::string, V>>& table, std::unordered_map<std::string, std::size_t>& index, std::string name, V value) {
    const auto [it, added] = index.emplace(name, table.size());
    if (added) {
        table.emplace_back(std::move(name), std::move(value));
    } else {
        table[it->second].second = std::move(value);
    }
}

// ---- STFS --------------------------------------------------------------------------------------------------------------------

Volumes stfs_volume(const stdfs::path& path) {
    auto package = std::make_shared<stfs::Package>(stfs::Package::parse(fs::read_file(path)));
    std::string magic(stfs::to_string(package->magic()));
    magic = std::string(text::trim(magic));
    std::vector<BootField> boot = {text_field("magic", magic), text_field("title id", package->title_id()),
                                   text_field("content type", std::format("0x{:X}", package->content_type())),
                                   text_field("display name", package->display_name())};
    Listing listing;
    for (const stfs::FileEntry& f : package->files()) listing.emplace_back(f.path, f.size);
    Volumes out;
    out.push_back(std::make_unique<ListedVolume>(
        VolumeKind::Stfs, package->title_name().empty() ? leaf(path) : package->title_name(), "", std::move(boot), listing,
        [package](const std::string& p) { return package->read_file(p); }));
    return out;
}

// ---- XDVDFS ------------------------------------------------------------------------------------------------------------------

std::unique_ptr<media::Volume> xdvdfs_volume(std::shared_ptr<Device> device, std::string label, std::string where, VolumeKind kind,
                                             std::optional<std::int64_t> base = std::nullopt,
                                             std::optional<std::int64_t> descriptor = std::nullopt) {
    auto disc = std::make_shared<gdf::Disc>(gdf::Disc::open(std::move(device), base, descriptor));
    Listing listing;
    for (const gdf::Entry& e : disc->files()) listing.emplace_back(e.path, e.size);
    return std::make_unique<ListedVolume>(kind, std::move(label), std::move(where), std::vector<BootField>{text_field("volume at", hex_text(disc->base()))},
                                          listing, [disc](const std::string& p) { return disc->read(p); });
}

// The Data#### parts: any file whose name starts with "data".
bool has_parts(const stdfs::path& folder) {
    std::error_code ec;
    for (const auto& e : stdfs::directory_iterator(folder, ec)) {
        std::error_code status;
        if (text::lower(fs::to_text(e.path().filename())).starts_with("data") && e.is_regular_file(status)) return true;
    }
    return false;
}

std::vector<stdfs::path> subfolders(const stdfs::path& folder) {
    std::vector<stdfs::path> out;
    std::error_code ec;
    for (const auto& e : stdfs::directory_iterator(folder, ec)) {
        std::error_code status;
        if (e.is_directory(status)) out.push_back(e.path());
    }
    std::ranges::sort(out, [](const stdfs::path& a, const stdfs::path& b) { return text::lower(leaf(a)) < text::lower(leaf(b)); });
    return out;
}

// The `<media id>.data` folder in or under `folder`: the folder itself when it holds the parts, else the first `.data` folder with
// parts that a walk from the top finds (each folder's own subfolders before the deeper ones).
std::optional<stdfs::path> find_data_folder(const stdfs::path& folder) {
    if (has_parts(folder)) return folder;
    const std::vector<stdfs::path> below = subfolders(folder);
    for (const stdfs::path& d : below) {
        if (text::lower(leaf(d)).ends_with(".data") && has_parts(d)) return d;
    }
    for (const stdfs::path& d : below) {
        if (std::optional<stdfs::path> hit = find_data_folder(d)) return hit;
    }
    return std::nullopt;
}

// A Games-on-Demand folder: the `.data` folder of Data#### parts, or the folder of the whole package (the one that holds it). With its
// `header` (the package file beside the .data folder), the volume is placed as the header's SVOD descriptor says: the disc sectors before
// its first data block are not in the parts, and in the "enhanced GDF" layout the volume descriptor comes first, the volume's sectors a
// 4 KB block after it.
std::optional<Volumes> god_volume(const stdfs::path& folder, const std::optional<stdfs::path>& header = std::nullopt) {
    const std::optional<stdfs::path> data = find_data_folder(folder);
    if (!data) return std::nullopt;
    std::optional<std::int64_t> base, descriptor;
    if (header) {
        const Bytes d = fs::RandomFile(*header, fs::FileMode::Read).read_at(0x391, 7);   // the SVOD descriptor: features, blocks, first block
        if (d.size() == 7) {
            const std::int64_t first = d[4] | (d[5] << 8) | (d[6] << 16);   // (24-bit, low byte first)
            const bool enhanced = (d[0] & 0x40) != 0;
            base = (enhanced ? 0x1000 : 0) - first * 0x1000;
            descriptor = 0;
        }
    }
    const std::string label = leaf(data->parent_path()).empty() ? "GoD" : leaf(data->parent_path());
    Volumes out;
    out.push_back(xdvdfs_volume(SvodDevice::open(*data), label, "Games on Demand", VolumeKind::God, base, descriptor));
    return out;
}

// The Data#### files of a package's .data folder, as they are.
Volumes parts_volume(const stdfs::path& folder) {
    std::vector<std::string> names;
    std::error_code ec;
    for (const auto& e : stdfs::directory_iterator(folder, ec)) {
        std::error_code status;
        if (e.is_regular_file(status)) names.push_back(leaf(e.path()));
    }
    std::ranges::sort(names);
    Listing listing;
    for (const std::string& n : names) listing.emplace_back(n, std::filesystem::file_size(folder / fs::from_text(n)));
    Volumes out;
    out.push_back(std::make_unique<ListedVolume>(VolumeKind::Parts, leaf(folder), "", std::vector<BootField>{count_field("parts", names.size())},
                                                 listing, [folder](const std::string& n) { return fs::read_file(folder / fs::from_text(n)); }));
    return out;
}

// ---- FATX --------------------------------------------------------------------------------------------------------------------

class FatxPartition final : public media::Volume {
public:
    FatxPartition(std::shared_ptr<fatx::Volume> reader, fatx::Partition partition) : reader_(std::move(reader)), partition_(std::move(partition)) {
        label_ = partition_.name;
        where_ = std::format("partition at 0x{:X}", partition_.offset);
        boot_ = {text_field("cluster", std::format("0x{:X} bytes", partition_.bytes_per_cluster)),
                 count_field("clusters", partition_.max_clusters),
                 text_field("byte order", partition_.order == fatx::ByteOrder::Big ? "big (Xbox 360)" : "little (original Xbox)")};
    }
    VolumeKind kind() const override { return VolumeKind::Fatx; }

    const std::vector<media::Entry>& files() override {
        if (!entries_) load();
        return *entries_;
    }

    Bytes read(const media::Entry& entry) override {
        if (!entries_) load();
        const auto it = by_path_.find(entry.path);
        if (it == by_path_.end()) throw NotFoundError(std::filesystem::path(entry.path), 0, "no such file in the partition");
        const fatx::Entry& e = *it->second;
        Bytes out;
        std::uint64_t remaining = e.file_size;
        for (const std::uint32_t cluster : reader_->cluster_chain(partition_, e.first_cluster)) {
            if (remaining == 0) break;
            Bytes chunk = reader_->read_cluster(partition_, cluster);
            chunk.resize(static_cast<std::size_t>(std::min<std::uint64_t>(chunk.size(), std::min<std::uint64_t>(remaining, partition_.bytes_per_cluster))));
            remaining -= chunk.size();
            append(out, chunk);
        }
        return out;
    }

private:
    // every entry, a folder before what is in it; a path met twice is listed once, at its first place, with the later entry
    void flatten(const std::vector<fatx::Entry>& level, std::vector<const fatx::Entry*>& flat) const {
        for (const fatx::Entry& e : level) {
            flat.push_back(&e);
            flatten(e.children, flat);
        }
    }
    void load() {
        tree_ = reader_->list_directory(partition_);
        std::vector<const fatx::Entry*> flat;
        flatten(tree_, flat);
        std::vector<media::Entry> entries;
        for (const fatx::Entry* e : flat) {
            media::Entry m;
            m.path = e->path;
            m.short_name = e->path;
            m.size = e->file_size;
            m.is_dir = e->is_directory;
            m.start = e->first_cluster;
            m.attributes = e->attributes;
            m.when = e->modified ? e->modified->to_string() : std::string();
            const auto [it, added] = by_path_.emplace(e->path, e);
            if (added) {
                index_.emplace(e->path, entries.size());
                entries.push_back(std::move(m));
            } else {
                it->second = e;
                entries[index_.at(e->path)] = std::move(m);
            }
        }
        entries_ = std::move(entries);
    }

    std::shared_ptr<fatx::Volume> reader_;
    fatx::Partition partition_;
    std::vector<fatx::Entry> tree_;
    std::optional<std::vector<media::Entry>> entries_;
    std::unordered_map<std::string, const fatx::Entry*> by_path_;   // lookup only
    std::unordered_map<std::string, std::size_t> index_;            // lookup only
};

std::optional<Volumes> fatx_volumes(const stdfs::path& path) {
    auto reader = std::make_shared<fatx::Volume>(path);
    Volumes out;
    for (fatx::Partition& p : reader->detect_partitions()) out.push_back(std::make_unique<FatxPartition>(reader, std::move(p)));
    if (out.empty()) return std::nullopt;
    return out;
}

// ---- the rest ----------------------------------------------------------------------------------------------------------------

Volumes nand_volume(const stdfs::path& path) {
    auto flash = std::make_shared<nand::Flash>(nand::Flash::parse(fs::read_file(path)));
    Listing listing;
    for (const nand::File& f : flash->files()) listing.emplace_back(f.name, f.size);
    Volumes out;
    out.push_back(std::make_unique<ListedVolume>(VolumeKind::Nand, "flash", std::format("root block {:#x}", flash->root_block()),
                                                 std::vector<BootField>{count_field("files", flash->files().size())}, listing,
                                                 [flash](const std::string& n) { return flash->read(n); }));
    return out;
}

// A container whose payloads are all in one buffer: (name, bytes) by name.
Volumes payload_volume(VolumeKind kind, const stdfs::path& path, std::vector<std::pair<std::string, Bytes>> found) {
    std::vector<std::pair<std::string, Bytes>> blobs;
    std::unordered_map<std::string, std::size_t> index;
    for (auto& [name, data] : found) put_by_name(blobs, index, name, std::move(data));
    Listing listing;
    for (const auto& [name, data] : blobs) listing.emplace_back(name, data.size());
    auto table = std::make_shared<std::vector<std::pair<std::string, Bytes>>>(std::move(blobs));
    auto lookup = std::make_shared<std::unordered_map<std::string, std::size_t>>(std::move(index));
    Volumes out;
    out.push_back(std::make_unique<ListedVolume>(kind, leaf(path), "", std::vector<BootField>{count_field("files", table->size())}, listing,
                                                 [table, lookup](const std::string& n) { return (*table)[lookup->at(n)].second; }));
    return out;
}

Volumes zar_volume(const stdfs::path& path) {
    auto archive = std::make_shared<zar::Reader>(zar::Reader::open(path));
    Listing listing;
    for (const zar::FileInfo& f : archive->files()) listing.emplace_back(f.path, f.size);
    Volumes out;
    out.push_back(std::make_unique<ListedVolume>(VolumeKind::Zar, leaf(path), "", std::vector<BootField>{count_field("files", listing.size())}, listing,
                                                 [archive](const std::string& n) { return archive->read(n); }));
    return out;
}

Volumes heavyiron_volume(const stdfs::path& path) {
    auto data = std::make_shared<Bytes>(fs::read_file(path));
    auto archive = std::make_shared<heavyiron::Archive>(heavyiron::Archive::parse(*data));
    using Located = std::pair<const heavyiron::Asset*, std::uint32_t>;   // the asset and its size
    std::vector<std::pair<std::string, Located>> by_name;
    std::unordered_map<std::string, std::size_t> index;
    for (const heavyiron::VolumeEntry& e : heavyiron::volume_entries(*archive)) put_by_name<Located>(by_name, index, e.name, {e.asset, e.size});
    Listing listing;
    for (const auto& [name, located] : by_name) listing.emplace_back(name, located.second);
    auto table = std::make_shared<decltype(by_name)>(std::move(by_name));
    auto lookup = std::make_shared<decltype(index)>(std::move(index));
    Volumes out;
    out.push_back(std::make_unique<ListedVolume>(VolumeKind::HeavyIron, leaf(path), "", std::vector<BootField>{count_field("assets", table->size())}, listing,
                                                 [data, archive, table, lookup](const std::string& n) {
                                                     const ByteView view = heavyiron::asset_bytes(*data, *(*table)[lookup->at(n)].second.first);
                                                     return Bytes(view.begin(), view.end());
                                                 }));
    return out;
}

bool name_ends(const stdfs::path& path, std::string_view suffix) { return text::lower(fs::to_text(path)).ends_with(suffix); }

}  // namespace volumes_impl

std::optional<Volumes> open(const std::filesystem::path& path) {
    using namespace volumes_impl;
    std::error_code ec;
    if (stdfs::is_directory(path, ec)) return god_volume(path);

    const Bytes head = fs::RandomFile(path, fs::FileMode::Read).read_at(0, 16);
    const std::string_view magic = as_text(ByteView(head).subspan(0, std::min<std::size_t>(4, head.size())));
    if (magic == "CON " || magic == "LIVE" || magic == "PIRS") {
        // A package whose contents are the Data#### parts in the `<header>.data` folder beside it: a Games-on-Demand title when its
        // volume is a disc (SVOD, volume type 1 at 0x3A9), else the parts themselves -- a video download is its .wmv cut into them.
        const stdfs::path data_folder = fs::from_text(fs::to_text(path) + ".data");
        if (stdfs::is_directory(data_folder, ec)) {
            const Bytes type = fs::RandomFile(path, fs::FileMode::Read).read_at(0x3A9, 4);
            const bool svod = type.size() == 4 && type[0] == 0 && type[1] == 0 && type[2] == 0 && type[3] == 1;
            return svod ? god_volume(data_folder, path) : std::optional<Volumes>(parts_volume(data_folder));
        }
        return stfs_volume(path);
    }
    if (magic == "CISO" || magic == "CCIM") {
        Volumes out;
        out.push_back(xdvdfs_volume(std::make_shared<ciso::Image>(ciso::Image::open(path)), leaf(path),
                                    std::string(text::trim(magic)) + " compressed disc", VolumeKind::Xdvdfs));
        return out;
    }
    if (nand::is_nand(path)) return nand_volume(path);
    if (name_ends(path, ".ho") && heavyiron::is_archive(path)) return heavyiron_volume(path);
    if (magic == "XUIZ") {
        const Bytes file = fs::read_file(path);
        std::vector<std::pair<std::string, Bytes>> found;
        for (const xzp::Payload& p : xzp::payloads(file)) found.emplace_back(p.path, Bytes(file.begin() + static_cast<std::ptrdiff_t>(p.offset), file.begin() + static_cast<std::ptrdiff_t>(p.offset + p.size)));
        return payload_volume(VolumeKind::Xzp, path, std::move(found));
    }
    if (name_ends(path, ".zar")) return zar_volume(path);
    if (name_ends(path, ".pck")) {
        const Bytes file = fs::read_file(path);
        std::vector<std::pair<std::string, Bytes>> found;
        for (const pck::Payload& p : pck::payloads(file).found) found.emplace_back(p.name, Bytes(file.begin() + static_cast<std::ptrdiff_t>(p.offset), file.begin() + static_cast<std::ptrdiff_t>(p.offset + p.size)));
        return payload_volume(VolumeKind::Pck, path, std::move(found));
    }
    try {   // a plain disc image
        Volumes out;
        out.push_back(xdvdfs_volume(std::make_shared<FileDevice>(path), leaf(path), "disc image", VolumeKind::Xdvdfs));
        return out;
    } catch (const FormatError&) {   // not XDVDFS: a hard-disk or memory-unit image, or nothing of ours
    }
    if (magic == "FATX" || fs::RandomFile(path, fs::FileMode::Read).size() >= 0x80000) {
        try {
            return fatx_volumes(path);
        } catch (const FormatError&) {   // (not FATX, or a damaged one: it is some other kind of image)
            return std::nullopt;
        } catch (const IoError&) {
            return std::nullopt;
        }
    }
    return std::nullopt;
}

}  // namespace cb::formats::xbox::volumes
