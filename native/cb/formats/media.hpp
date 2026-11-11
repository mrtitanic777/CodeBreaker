// cb/formats/media.hpp -- floppy, hard-disk and CD images, and bundles of system modules, read as volumes of files.
//
//     for (auto& v : media::volumes(path)) {       every volume inside an image file
//         v->kind(), v->label(), v->where()        what it is, its name, where in the image it is
//         v->boot()                                the fields of its boot sector / volume descriptor
//         for (const media::Entry& e : v->files()) every file and folder: e.path  e.size  e.is_dir  e.when
//             Bytes data = v->read(e);
//     media::extract(path, folder, names)          write the files out
//     media::put(path, files)                      write files into an image's FAT volume in place
//
// One image file can hold several volumes: a floppy image is one FAT volume; a hard-disk image (also a "flat" virtual
// disk) is a partition table and a FAT volume per partition, logical drives included; a CD image is an ISO 9660 volume
// and, when it is bootable by floppy emulation (El Torito), the boot floppy inside it as a FAT volume of its own; and a
// Windows 1.x boot image (WIN100.BIN with its .OVL) is a bundle of system modules, each written out as an ordinary
// one-file executable (cb/formats/ne.hpp does the taking apart). The Xbox and Xbox 360 containers (STFS, discs, GoD,
// FATX ...) are volumes of their own in the xbox modules.
//
// FAT: 12-, 16- and 32-bit tables told apart by the number of clusters, as DOS does; long file names are read, and put()
// writes by the 8.3 ones; floppies from before DOS 2.0 carry no parameter block, so their layout is taken from the media
// byte and the image size. ISO 9660: the primary names, or the long (Joliet) names when the disc has them; images with
// raw 2352-byte sectors are read too. Files are read on demand, never the whole image.
//
// A volume keeps its image file open and is not safe to use from two threads at once.
#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/random_file.hpp"
#include "cb/formats/ne.hpp"

namespace cb::formats::media {

// Bytes of an image file from an offset on, read as asked for. Copies share the open file.
class Source {
public:
    // Opens the image for reading; NotFoundError / AccessError when it cannot be.
    explicit Source(const std::filesystem::path& file, std::uint64_t offset = 0);
    // The same file, `more` bytes further in.
    Source shifted(std::uint64_t more) const;

    const std::filesystem::path& path() const { return file_->path(); }
    std::uint64_t offset() const { return offset_; }
    // The size of the whole image file.
    std::uint64_t file_size() const { return file_size_; }
    // Up to `count` bytes at `at` (relative to the offset): fewer if the file ends first, none beyond it.
    Bytes read(std::uint64_t at, std::uint64_t count) const;

private:
    std::shared_ptr<fs::RandomFile> file_;
    std::uint64_t offset_ = 0;
    std::uint64_t file_size_ = 0;
};

enum class VolumeKind { Fat12, Fat16, Fat32, Iso9660, Bundle };
// "fat12" | "fat16" | "fat32" | "iso9660" | "bundle"
std::string_view kind_name(VolumeKind kind);

// One extent of a file on an ISO 9660 disc: where it starts (a block) and how long it is.
struct Extent {
    std::uint64_t block = 0, size = 0;
};

// One file or folder of a volume.
struct Entry {
    std::string path;                 // folders separated by '/'
    std::uint64_t size = 0;
    bool is_dir = false;
    // FAT: the first cluster. ISO 9660: the first block. Bundle: where the module's header is.
    std::uint64_t start = 0;
    std::string when;                 // "YYYY-MM-DD HH:MM", empty when the volume does not say
    std::string short_name;           // the 8.3 / primary name's path (what --plain shows); the path when there is none
    std::uint8_t attributes = 0;      // FAT attributes
    std::vector<Extent> extents;      // ISO 9660: the runs the data is stored in
};

// One line of a volume's boot-sector or volume-descriptor facts, in the order they are shown. `text` is what is shown;
// `number` is present for the fields that are counts.
struct BootField {
    std::string name;
    std::string text;
    std::optional<std::int64_t> number;
};

// What every kind of volume gives.
class Volume {
public:
    virtual ~Volume() = default;
    virtual VolumeKind kind() const = 0;
    // The volume's name. A FAT volume with no name in its boot sector takes the one of the label entry in its root folder
    // when `files()` has read it.
    const std::string& label() const { return label_; }
    // Where in the image file the volume is, in words ("partition at sector 63, type 06"); empty for the whole file.
    const std::string& where() const { return where_; }
    std::span<const BootField> boot() const { return boot_; }
    // Every file and folder, folders before what is in them. Read once, then kept.
    virtual const std::vector<Entry>& files() = 0;
    // The contents of a file (empty for a folder).
    virtual Bytes read(const Entry& entry) = 0;

protected:
    std::string label_, where_;
    std::vector<BootField> boot_;
};

// ---- FAT ---------------------------------------------------------------------------------------------------------------

struct FatGeometry {
    bool old = false;                        // a floppy from before DOS 2.0: no parameter block, the layout is inferred
    std::uint32_t bytes_per_sector = 0, sectors_per_cluster = 0, root_entries = 0, fats = 0, sectors_per_fat = 0;
    std::uint64_t fat_at = 0, root_at = 0, data_at = 0;   // byte offsets from the volume's start
    std::uint32_t clusters = 0;
    int bits = 12;                           // 12, 16 or 32
    std::uint32_t root_cluster = 0;          // FAT32's root folder
};

class FatVolume final : public Volume {
public:
    // The volume at the start of `source`; `size` is the room it has (a partition's size), when known -- it decides
    // whether a boot sector with no parameter block is a pre-DOS-2.0 floppy. nullopt when what is there is no FAT volume.
    static std::optional<FatVolume> try_open(Source source, std::string where = {},
                                             std::optional<std::uint64_t> size = std::nullopt);
    // The same; FormatError when it is not a FAT volume.
    static FatVolume open(Source source, std::string where = {}, std::optional<std::uint64_t> size = std::nullopt);

    VolumeKind kind() const override;
    const FatGeometry& geometry() const { return geometry_; }
    const Source& source() const { return source_; }
    // The table: the first copy of the FAT as stored.
    ByteView fat() const { return fat_; }
    // The cluster that follows `cluster` in its chain (an end-of-chain marker, 0xFF8 and up in FAT12, past the table too).
    std::uint32_t next_cluster(std::uint32_t cluster) const;
    // The bytes of a cluster chain: all of it, or the first `size` bytes.
    Bytes read_chain(std::uint32_t cluster, std::optional<std::uint64_t> size = std::nullopt) const;

    const std::vector<Entry>& files() override;
    Bytes read(const Entry& entry) override;

private:
    explicit FatVolume(Source source) : source_(std::move(source)) {}
    Source source_;
    FatGeometry geometry_;
    Bytes fat_;
    std::optional<std::vector<Entry>> entries_;
};

// ---- ISO 9660 ----------------------------------------------------------------------------------------------------------

// The El Torito boot image of a disc: where it is in the image file, how long, and the media it emulates
// (0 none: a loader; 1 / 2 / 3 a 1.2 / 1.44 / 2.88 MB floppy; 4 a hard disk).
struct BootImage {
    std::uint64_t offset = 0, size = 0;
    std::uint8_t media = 0;
};

struct IsoLayout {
    std::uint32_t sector = 2048;             // bytes per sector in the image file: 2048, or 2352 for raw sectors
    std::uint32_t skip = 0;                  // bytes before the 2048 of data in a raw sector (16 or 24)
    bool long_names = false;                 // the Joliet names are used
    bool has_joliet = false;
    std::uint32_t block = 2048;              // the volume's logical block size
    std::uint32_t root_block = 0, root_size = 0;
    std::optional<std::uint32_t> boot_catalog;   // the El Torito boot catalog's sector
};

class IsoVolume final : public Volume {
public:
    // `sector` / `skip`: 2048 / 0 for a plain image; 2352 / 16 or 2352 / 24 for raw sectors. `long_names`: use the
    // Joliet names when the disc has them. FormatError when there is no ISO 9660 volume.
    static IsoVolume open(Source source, std::uint32_t sector = 2048, std::uint32_t skip = 0, bool long_names = true);

    VolumeKind kind() const override { return VolumeKind::Iso9660; }
    const IsoLayout& layout() const { return layout_; }
    const Source& source() const { return source_; }
    // The El Torito boot image, or nullopt when the disc is not bootable.
    std::optional<BootImage> boot_image() const;

    const std::vector<Entry>& files() override;
    Bytes read(const Entry& entry) override;

private:
    explicit IsoVolume(Source source) : source_(std::move(source)) {}
    Source source_;
    IsoLayout layout_;
    std::optional<std::vector<Entry>> entries_;
};

// ---- a bundle of system modules ----------------------------------------------------------------------------------------

// A Windows 1.x boot image: system modules stored together (see ne::open_boot_image). Each is a file "<NAME>.EXE",
// built as an ordinary executable.
class BundleVolume final : public Volume {
public:
    // nullopt when the file is not a bundle of at least two modules.
    static std::optional<BundleVolume> try_open(const std::filesystem::path& file);

    VolumeKind kind() const override { return VolumeKind::Bundle; }
    const std::vector<std::pair<std::string, ne::Module>>& modules() const { return modules_; }

    const std::vector<Entry>& files() override;
    Bytes read(const Entry& entry) override;

private:
    BundleVolume() = default;
    const Bytes& built(std::size_t module_index);
    std::vector<std::pair<std::string, ne::Module>> modules_;
    std::vector<std::optional<Bytes>> built_;
    std::optional<std::vector<Entry>> entries_;
};

// ---- any image ---------------------------------------------------------------------------------------------------------

// Every volume inside an image file: an ISO 9660 volume (and its boot floppy), a FAT volume, the FAT volumes of the
// partitions, or a module bundle, whichever the file is. `long_names`: the Joliet names of a disc rather than the primary
// ones. NotFoundError when the file is missing; FormatError when it is a folder or nothing in it is recognised.
std::vector<std::unique_ptr<Volume>> volumes(const std::filesystem::path& image, bool long_names = true);

// A file to write into a volume: its path ("FOLDER/NAME.EXT", 8.3 names in any case, '/' or '\' between folders) and data.
struct NewFile {
    std::string path;
    Bytes data;
};
enum class PutOutcome { Replaced, Added };
struct PutResult {
    std::string path;
    PutOutcome outcome = PutOutcome::Added;
};

// Writes `files` into FAT volume number `volume` of the image at `path`, in place. A file that is there is replaced --
// its clusters freed; its directory entry, attributes and long name kept, with the new start, size and time -- and one
// that is not is added, a missing folder made. Every copy of the FAT is written, also when a file fails. A file that
// cannot be written (a name that is not 8.3, a folder in the way, a full volume) is an error after the earlier files are
// in; UnsupportedError when the volume is not FAT with a parameter block, UsageError for a missing volume.
std::vector<PutResult> put(const std::filesystem::path& image, std::span<const NewFile> files, std::size_t volume = 0);

// Writes the files of an image under `folder` (each volume after the first in a folder of its own, "volume2" ...). `names`
// limits it to files whose name or path matches one of the wildcards (`*`, `?`, `[a-z]`, case ignored). Returns how many
// files were written. Names are made safe to write: no "..", no characters Windows refuses.
std::size_t extract(const std::filesystem::path& image, const std::filesystem::path& folder,
                    std::span<const std::string> names = {}, bool long_names = true);

// A path inside an image as a relative path that is safe to create: '/' and '\' separate folders, empty, "." and ".."
// parts are dropped, characters Windows refuses (< > : " | ? * and controls) become '_'. FormatError when nothing is left.
std::filesystem::path safe_relative_path(std::string_view path);

// Whether `name` matches the wildcard `pattern`: `*` any run, `?` any one character, `[abc]` / `[a-z]` / `[!abc]` a set;
// ASCII case is ignored and '/' equals '\'.
bool wildcard_match(std::string_view pattern, std::string_view name);

// The text `media list` prints for a volume: a summary line, with `boot_fields` the boot-sector facts, then one line per
// file and folder (`long_names` chooses the long path or the 8.3 one).
std::string listing_text(Volume& volume, bool long_names = true, bool boot_fields = false);

}  // namespace cb::formats::media
