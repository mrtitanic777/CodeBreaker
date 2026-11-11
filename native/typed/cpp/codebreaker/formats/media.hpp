// codebreaker/formats/media.hpp -- compat shim over cb/formats/media.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/media.{hpp,cpp}: FAT12/16/32 volumes, ISO 9660 discs (plain and raw sectors, Joliet
// names, El Torito), partition tables, module bundles, put() and extract(). This header keeps the old py:: surface for the
// ports that have not been refactored yet (formats/xbox/volumes, coverage/reach, recompile/title, formats/xbox/package ...)
// and is deleted with the last of them. Conversion and forwarding only, except what must see the old Xbox volumes:
//   COMPAT(py): volumes / put / extract / main  -- media.cpp: they put formats::xbox::volumes first (an Xbox 360 disc also
//       carries an ISO 9660 video part) and xbox/volumes.hpp includes this header; remove when formats.xbox.volumes is
//       refactored
//   COMPAT(py): BundleVolume  -- built from the old formats::ne shim; remove when its last consumer is refactored
//
// A volume's duck type (kind, label, where, boot, files(), read(entry)) is the abstract class Volume; volumes() hands out
// std::shared_ptr<Volume>. An Entry's `extents` exist only for ISO entries (std::optional).
#pragma once

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/formats/media.hpp"
#include "codebreaker/formats/ne.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::media {

namespace ne = codebreaker::formats::ne;
namespace cm = cb::formats::media;

// {field: value} of a boot sector / volume descriptor: text or numbers
using BootValue = std::variant<py::str, int64_t>;
using Boot = py::dict<py::str, BootValue>;

// One file or folder of a volume.
struct Entry {
    py::str path;
    int64_t size = 0;
    bool is_dir = false;
    int64_t start = 0;
    py::str when, short_;
    int64_t attributes = 0;
    std::optional<py::list<std::tuple<int64_t, int64_t>>> extents;      // ISO entries: [(block, size)]

    Entry() = default;
    Entry(const py::str& path_, int64_t size_, bool is_dir_, int64_t start_, const py::str& when_ = py::str(""),
          const py::str& short__ = py::str(""), int64_t attributes_ = 0)
        : path(path_), size(size_), is_dir(is_dir_), start(start_), when(when_), short_(short__.empty() ? path_ : short__), attributes(attributes_) {}
    py::str repr() const { return py::percent(py::str("Entry(%r, %d)"), path, size); }
};

namespace detail {

inline Entry entry_of(const cm::Entry& e) {
    Entry out(cb::compat::to_py(e.path), static_cast<int64_t>(e.size), e.is_dir, static_cast<int64_t>(e.start), cb::compat::to_py(e.when),
              cb::compat::to_py(e.short_name), e.attributes);
    if (!e.extents.empty()) {   // only ISO entries have extents (each has at least one)
        py::list<std::tuple<int64_t, int64_t>> runs;
        for (const cm::Extent& x : e.extents) runs.append(std::make_tuple(static_cast<int64_t>(x.block), static_cast<int64_t>(x.size)));
        out.extents = runs;
    }
    return out;
}

inline cm::Entry entry_to(const Entry& e) {
    cm::Entry out;
    out.path = cb::compat::from_py(e.path);
    out.size = static_cast<std::uint64_t>(e.size);
    out.is_dir = e.is_dir;
    out.start = static_cast<std::uint64_t>(e.start);
    if (e.extents)
        for (const auto& x : *e.extents) out.extents.push_back({static_cast<std::uint64_t>(std::get<0>(x)), static_cast<std::uint64_t>(std::get<1>(x))});
    return out;
}

inline Boot boot_of(std::span<const cm::BootField> fields) {
    Boot out;
    for (const cm::BootField& f : fields)
        out.set(cb::compat::to_py(f.name), f.number ? BootValue(*f.number) : BootValue(cb::compat::to_py(f.text)));
    return out;
}

}  // namespace detail

// what every kind of volume gives
class Volume {
  public:
    py::str kind, label, where;
    Boot boot;
    virtual ~Volume() = default;
    // Every file and folder (FAT: folders before what is in them).
    virtual py::list<Entry> files() = 0;
    virtual py::bytes read(const Entry& entry) = 0;
};

// Bytes of an image file from an offset on, read as asked for.
class _Source {
  public:
    py::str path;
    int64_t offset = 0;
    _Source() = default;
    explicit _Source(const py::str& path_, int64_t offset_ = 0) : path(path_), offset(offset_) {}
    py::bytes read(int64_t at, int64_t n) const {
        return cb::compat::guard([&] {
            if (offset + at < 0) throw py::OSError("[Errno 22] Invalid argument");
            return cb::compat::to_py(cb::ByteView(cm::Source(std::filesystem::path(cb::compat::from_py(path)), static_cast<std::uint64_t>(offset))
                                                      .read(static_cast<std::uint64_t>(at), static_cast<std::uint64_t>(n < 0 ? 0 : n))));
        });
    }
};

// FAT ------------------------------------------------------------------------------------------
// A FAT12/16/32 volume (its attributes: the boot sector's fields, `boot`, the table `fat`).
class FatVolume : public Volume {
  public:
    _Source src;
    bool old = false;
    int64_t bps = 0, spc = 0, roots = 0, fats = 0, spf = 0, fat_at = 0, root_at = 0, data_at = 0, clusters = 0, bits = 0, root_cluster = 0;
    py::bytes fat;

    FatVolume(const _Source& source, const py::str& where_ = py::str(""), std::optional<int64_t> size = std::nullopt) : src(source) {
        take_(std::make_shared<cm::FatVolume>(cb::compat::guard([&] {
            return cm::FatVolume::open(cm::Source(std::filesystem::path(cb::compat::from_py(src.path)), static_cast<std::uint64_t>(src.offset)),
                                       cb::compat::from_py(where_), size ? std::optional<std::uint64_t>(static_cast<std::uint64_t>(*size)) : std::nullopt);
        })));
    }
    // a volume the new reader opened
    FatVolume(const _Source& source, std::shared_ptr<cm::FatVolume> opened) : src(source) { take_(std::move(opened)); }

    py::list<Entry> files() override {
        py::list<Entry> out;
        for (const cm::Entry& e : cb::compat::guard([&]() -> const std::vector<cm::Entry>& { return impl_->files(); })) out.append(detail::entry_of(e));
        label = cb::compat::to_py(impl_->label());     // the volume label of the root folder, once it was read
        return out;
    }
    int64_t _next(int64_t c) { return impl_->next_cluster(static_cast<std::uint32_t>(c)); }
    // The bytes of a cluster chain (all of it, or the first `size` bytes).
    py::bytes _chain(int64_t cluster, std::optional<int64_t> size = std::nullopt) {
        return cb::compat::to_py(cb::ByteView(impl_->read_chain(static_cast<std::uint32_t>(cluster),
                                                               size ? std::optional<std::uint64_t>(static_cast<std::uint64_t>(std::max<int64_t>(*size, 0))) : std::nullopt)));
    }
    py::bytes read(const Entry& entry) override {
        return entry.is_dir ? py::bytes() : _chain(entry.start, entry.size);
    }

  private:
    std::shared_ptr<cm::FatVolume> impl_;
    void take_(std::shared_ptr<cm::FatVolume> v) {
        impl_ = std::move(v);
        const cm::FatGeometry& g = impl_->geometry();
        where = cb::compat::to_py(impl_->where());
        old = g.old;
        bps = g.bytes_per_sector, spc = g.sectors_per_cluster, roots = g.root_entries, fats = g.fats, spf = g.sectors_per_fat;
        fat_at = static_cast<int64_t>(g.fat_at), root_at = static_cast<int64_t>(g.root_at), data_at = static_cast<int64_t>(g.data_at);
        clusters = g.clusters, bits = g.bits, root_cluster = g.root_cluster;
        kind = cb::compat::to_py(cm::kind_name(impl_->kind()));
        label = cb::compat::to_py(impl_->label());
        boot = detail::boot_of(impl_->boot());
        fat = cb::compat::to_py(impl_->fat());
    }
};

// ISO 9660 ---------------------------------------------------------------------------------------
class IsoVolume : public Volume {
  public:
    _Source src;
    int64_t sector = 2048, skip = 0, block = 0;
    std::optional<int64_t> boot_catalog;
    bool long_names = true;
    std::tuple<int64_t, int64_t> root;

    // `sector`/`skip`: 2048/0 for a plain image; 2352/16 or 2352/24 for raw sectors.
    explicit IsoVolume(const _Source& source, int64_t sector_ = 2048, int64_t skip_ = 0, bool long_names_ = true) : src(source) {
        take_(std::make_shared<cm::IsoVolume>(cb::compat::guard([&] {
            return cm::IsoVolume::open(cm::Source(std::filesystem::path(cb::compat::from_py(src.path)), static_cast<std::uint64_t>(src.offset)),
                                       static_cast<std::uint32_t>(sector_), static_cast<std::uint32_t>(skip_), long_names_);
        })));
    }
    // a volume the new reader opened
    IsoVolume(const _Source& source, std::shared_ptr<cm::IsoVolume> opened) : src(source) { take_(std::move(opened)); }

    py::list<Entry> files() override {
        py::list<Entry> out;
        for (const cm::Entry& e : cb::compat::guard([&]() -> const std::vector<cm::Entry>& { return impl_->files(); })) out.append(detail::entry_of(e));
        return out;
    }
    py::bytes read(const Entry& entry) override {
        if (entry.is_dir) return py::bytes();
        if (!entry.extents) throw py::AttributeError("'Entry' object has no attribute 'extents'");
        return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(impl_->read(detail::entry_to(entry)))); });
    }
    // (offset in the image file, size, emulated media) of the El Torito boot image: media 0 none (a loader), 1 / 2 / 3 a
    // 1.2 / 1.44 / 2.88 MB floppy, 4 a hard disk. None when the disc is not bootable.
    std::optional<std::tuple<int64_t, int64_t, int64_t>> boot_image() {
        auto b = cb::compat::guard([&] { return impl_->boot_image(); });
        if (!b) return std::nullopt;
        return std::make_tuple(static_cast<int64_t>(b->offset), static_cast<int64_t>(b->size), static_cast<int64_t>(b->media));
    }

  private:
    std::shared_ptr<cm::IsoVolume> impl_;
    void take_(std::shared_ptr<cm::IsoVolume> v) {
        impl_ = std::move(v);
        const cm::IsoLayout& l = impl_->layout();
        sector = l.sector, skip = l.skip, block = l.block, long_names = l.long_names;
        if (l.boot_catalog) boot_catalog = *l.boot_catalog;
        root = std::make_tuple<int64_t, int64_t>(l.root_block, l.root_size);
        where = py::str("");
        kind = py::str("iso9660");
        label = cb::compat::to_py(impl_->label());
        boot = detail::boot_of(impl_->boot());
    }
};

// a bundle of system modules ---------------------------------------------------------------------
// A Windows 1.x boot image: system modules stored together (see formats.ne.open_boot_image).
class BundleVolume : public Volume {
  public:
    py::dict<py::str, ne::BootImageModule> modules;

    // ValueError when the file is not a bundle of at least two modules
    explicit BundleVolume(const py::str& path) {
        kind = py::str("bundle");
        where = py::str("");
        modules = ne::open_boot_image(path);
        if (modules.size() < 2) throw py::ValueError("not a bundle of modules");
        label = py::os::path::basename(path);
        boot.set(py::str("modules"), BootValue(static_cast<int64_t>(modules.size())));
    }

    py::list<Entry> files() override {
        py::list<Entry> out;
        for (const auto& kv : modules.items()) {
            const py::str& name = std::get<0>(kv);
            out.append(Entry(name + py::str(".EXE"), _bytes(name).size(), false, std::get<1>(kv).ne_off, py::str(""), name + py::str(".EXE")));
        }
        return out;
    }
    py::bytes read(const Entry& entry) override { return _bytes(entry.path.slice(0, entry.path.size() - 4)); }

  private:
    py::dict<py::str, py::bytes> _built;
    py::bytes _bytes(const py::str& name) {
        if (!_built.contains(name)) _built.set(name, ne::standalone(modules.at(name)));
        return _built.at(name);
    }
};

// any image ------------------------------------------------------------------------------------
// Every volume inside an image file. ValueError when nothing in it is recognised.
py::list<std::shared_ptr<Volume>> volumes(const py::str& path, bool long_names = true);

// Write `files` {"FOLDER/NAME.EXT": bytes} into FAT volume number `volume` of the image at `path`, in place. -> [(path, "replaced" | "added")]
py::list<std::tuple<py::str, py::str>> put(const py::str& path, const py::dict<py::str, py::bytes>& files, int64_t volume = 0);

inline py::str _safe(const py::str& path) { return cb::compat::guard([&] { return cb::compat::to_py(cb::formats::media::safe_relative_path(cb::compat::from_py(path)).string()); }); }

// Write the files of an image under `folder` (each volume after the first in a folder of its own). `names` limits it to
// matching names or paths (wildcards allowed). -> files written
int64_t extract(const py::str& path, const py::str& folder, const py::list<py::str>& names = py::list<py::str>(), bool long_names = true);

// fnmatch.fnmatch(name, pat): lower case, "/" is "\", then the wildcard match
inline bool fnmatch(const py::str& name, const py::str& pat) { return cm::wildcard_match(cb::compat::from_py(pat), cb::compat::from_py(name)); }

// main's docstring: the usage text
extern const py::str MAIN_DOC;

// python -m codebreaker media list|extract ...; `argv` is sys.argv[1:]. -> the exit status.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::media
