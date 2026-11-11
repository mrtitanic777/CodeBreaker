// codebreaker/formats/media.hpp -- typed C++ from codebreaker.formats.media.
//
// media.py -- read media and containers: floppy, hard-disk and CD images, module bundles, and the Xbox / Xbox 360
// containers (formats/xbox/volumes.py: STFS, discs, GoD, FATX, ZAR, XZP, PCK).
//
//     for (auto& v : media::volumes(path)) {   every volume inside an image file (or GoD folder)
//         v->kind                              "fat12" | "fat16" | "fat32" | "iso9660" | "bundle" |
//                                              "stfs" | "xdvdfs" | "god" | "fatx" | "zar" | "xzp" | "pck"
//         v->label, v->where                   its name; where in the image it is
//         v->boot                              {field: value} of its boot sector / volume descriptor
//         for (auto& e : v->files())           every file and folder: e.path  e.size  e.is_dir  e.when
//             data = v->read(e)
//     media::extract(path, folder, names)      write the files out; -> number written
//     media::put(path, {{"WINDOWS/SYSTEM/X.VXD", data}})   write files into an image's FAT volume in place
//                                              (replaced or added; folders made as needed)
//
// One image file can hold several volumes: a floppy image is one FAT volume; a hard-disk image (also a "flat"
// virtual disk) is a partition table and a FAT volume per partition, logical drives included; a CD image is an
// ISO-9660 volume and, when it is bootable by floppy emulation (El Torito), the boot floppy inside it as a FAT
// volume of its own; and a Windows 1.x boot image (WIN100.BIN with its .OVL) is a bundle of system modules, each
// written out as an ordinary one-file executable (codebreaker.formats.ne does the taking apart).
//
// FAT: 12-, 16- and 32-bit tables told apart by the number of clusters, as DOS does; long file names are read
// (`--plain` shows the 8.3 names) and put() writes by the 8.3 ones; floppies from before DOS 2.0 carry no parameter
// block, so their layout is taken from the media byte and the image size. ISO-9660: the primary names, or the long
// (Joliet) names when the disc has them (`--plain` for the primary ones); images with raw 2352-byte sectors are read
// too. Files are read on demand, never the whole image. The FAT and ISO-9660 readers, the partition table and put()
// are formats.dll's (native/formats/media, docs/NATIVE.md step 4).
//
// Typing and runtime notes:
//   * The Python asked formats.dll through native.call; here the same questions go to the reader linked in
//     (native/formats/media/media.cc, through cbf_call, answered in typed values by py::native). The attribute dicts it
//     answers ("_attrs", whose keys the Python copied into the volume object's __dict__) are the records FatAttrs and
//     IsoAttrs; the boot-sector dict, whose values are text or numbers, is `Boot`: {str: variant<str, int>}.
//   * A volume's duck type (kind, label, where, boot, files(), read(entry)) is the abstract class Volume; volumes() hands
//     out std::shared_ptr<Volume> (FatVolume, IsoVolume, BundleVolume here; formats.xbox.volumes' kinds there).
//   * An Entry's `extents` attribute exists only for ISO entries: std::optional (reading it from another kind of entry is
//     the AttributeError the Python raises).
//   * `short` is a C++ keyword: Entry::short_.
//   * fnmatch (not in the runtime) is written below after Python's fnmatch.translate.
//   * main(argv) takes sys.argv[1:] and prints with py::print.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/formats/ne.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::media {

namespace ne = codebreaker::formats::ne;


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
    Entry(const py::str& path, int64_t size, bool is_dir, int64_t start, const py::str& when = py::str(""),
          const py::str& short_ = py::str(""), int64_t attributes = 0);
    py::str repr() const;       // "Entry(%r, %d)"
};

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
    explicit _Source(const py::str& path, int64_t offset = 0) : path(path), offset(offset) {}
    py::bytes read(int64_t at, int64_t n) const;
};

// the attribute dict formats.dll answers for a FAT volume (media.fat)
struct FatAttrs {
    py::str where;
    bool old = false;
    int64_t bps = 0, spc = 0, roots = 0, fats = 0, spf = 0, fat_at = 0, root_at = 0, data_at = 0, clusters = 0, bits = 0;
    py::str kind;
    int64_t root_cluster = 0;
    Boot boot;
    py::bytes fat;
    py::str label;
};
// ... and for an ISO-9660 volume (media.iso)
struct IsoAttrs {
    int64_t sector = 0, skip = 0;
    py::str where;
    std::optional<int64_t> boot_catalog;
    bool long_names = false;
    int64_t block = 0;
    py::str label;
    std::tuple<int64_t, int64_t> root;
    Boot boot;
};

// (the records' fields by name, for reading formats.dll's answers: py::native::read<FatAttrs>)
template <class F> void for_each_field_mut(FatAttrs& r, F&& f) {
    f("where", r.where, true);
    f("old", r.old, true);
    f("bps", r.bps, true);
    f("spc", r.spc, true);
    f("roots", r.roots, true);
    f("fats", r.fats, true);
    f("spf", r.spf, true);
    f("fat_at", r.fat_at, true);
    f("root_at", r.root_at, true);
    f("data_at", r.data_at, true);
    f("clusters", r.clusters, true);
    f("bits", r.bits, true);
    f("kind", r.kind, true);
    f("root_cluster", r.root_cluster, true);
    f("boot", r.boot, true);
    f("fat", r.fat, true);
    f("label", r.label, true);
}
template <class F> void for_each_field_mut(IsoAttrs& r, F&& f) {
    f("sector", r.sector, true);
    f("skip", r.skip, true);
    f("where", r.where, true);
    f("boot_catalog", r.boot_catalog, true);
    f("long_names", r.long_names, true);
    f("block", r.block, true);
    f("label", r.label, true);
    f("root", r.root, true);
    f("boot", r.boot, true);
}

// FAT ------------------------------------------------------------------------------------------
// A FAT12/16/32 volume (its attributes: the boot sector's fields, `boot`, the table `fat`).
class FatVolume : public Volume {
  public:
    _Source src;
    bool old = false;
    int64_t bps = 0, spc = 0, roots = 0, fats = 0, spf = 0, fat_at = 0, root_at = 0, data_at = 0, clusters = 0, bits = 0, root_cluster = 0;
    py::bytes fat;

    FatVolume(const _Source& source, const py::str& where = py::str(""), std::optional<int64_t> size = std::nullopt);
    FatVolume(const _Source& source, const FatAttrs& attrs);      // _attrs given

    py::list<Entry> files() override;
    int64_t _next(int64_t c);
    // The bytes of a cluster chain (all of it, or the first `size` bytes).
    py::bytes _chain(int64_t cluster, std::optional<int64_t> size = std::nullopt);
    py::bytes read(const Entry& entry) override;

  private:
    std::optional<py::list<Entry>> _entries;
    void take_(const FatAttrs& a);
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
    explicit IsoVolume(const _Source& source, int64_t sector = 2048, int64_t skip = 0, bool long_names = true);
    IsoVolume(const _Source& source, const IsoAttrs& attrs);      // _attrs given

    py::list<Entry> files() override;
    py::bytes read(const Entry& entry) override;
    // (offset in the image file, size, emulated media) of the El Torito boot image: media 0 none (a loader), 1 / 2 / 3 a
    // 1.2 / 1.44 / 2.88 MB floppy, 4 a hard disk. None when the disc is not bootable.
    std::optional<std::tuple<int64_t, int64_t, int64_t>> boot_image();

  private:
    std::optional<py::list<Entry>> _entries;
    void take_(const IsoAttrs& a);
};

// a bundle of system modules ---------------------------------------------------------------------
// A Windows 1.x boot image: system modules stored together (see formats.ne.open_boot_image).
class BundleVolume : public Volume {
  public:
    py::dict<py::str, ne::BootImageModule> modules;

    // ValueError when the file is not a bundle of at least two modules
    explicit BundleVolume(const py::str& path);

    py::list<Entry> files() override;
    py::bytes read(const Entry& entry) override;

  private:
    py::dict<py::str, py::bytes> _built;
    py::bytes _bytes(const py::str& name);
};

// any image ------------------------------------------------------------------------------------
// Every volume inside an image file. ValueError when nothing in it is recognised.
py::list<std::shared_ptr<Volume>> volumes(const py::str& path, bool long_names = true);

// Write `files` {"FOLDER/NAME.EXT": bytes} into FAT volume number `volume` of the image at `path`, in place. Paths are
// 8.3 names in any case, folder by folder. A file that is there is replaced -- its clusters freed; its directory entry,
// attributes and long name kept, with the new start, size and time -- and one that is not is added, a missing folder
// made. Every copy of the FAT is written. -> [(path, "replaced" | "added")]
py::list<std::tuple<py::str, py::str>> put(const py::str& path, const py::dict<py::str, py::bytes>& files, int64_t volume = 0);

py::str _safe(const py::str& path);

// Write the files of an image under `folder` (each volume after the first in a folder of its own). `names` limits it to
// matching names or paths (wildcards allowed). -> files written
int64_t extract(const py::str& path, const py::str& folder, const py::list<py::str>& names = py::list<py::str>(), bool long_names = true);

// fnmatch.fnmatch(name, pat): the platform's case rules (Windows: lower case, "/" is "\"), then fnmatchcase
bool fnmatch(const py::str& name, const py::str& pat);

// main's docstring: the usage text
extern const py::str MAIN_DOC;

// python -m codebreaker media list|extract ...; `argv` is sys.argv[1:]. -> the exit status.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::media
