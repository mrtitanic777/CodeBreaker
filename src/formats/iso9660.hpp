// formats/iso9660.hpp -- an ISO 9660 volume (with its Joliet names) read, and a file in it replaced in place.
//
// What the harnesses that boot an operating system from a CD need: find a file by its path, read it, and swap it for
// another -- Windows 95 Setup's CD with one of its cabinets rebuilt. The replacement is made where the file's data
// stands: the new data is appended at the end of the image, the file's directory records (in the primary tree and in
// the Joliet one, which name the same extent) are pointed at it, and the volume's size is raised. Nothing else in the
// image moves, so every other file, the boot catalog, the path tables and the volume's layout stay exactly as they
// were; the old data stays where it was, unreferenced.
//
//     iso9660::Volume cd(path, iso9660::Access::ReadWrite);
//     for (const iso9660::Entry& e : cd.list("/WIN95")) ...      // "MINI.CAB", "PRECOPY1.CAB", ...
//     Bytes cab = cd.read("/WIN95/MINI.CAB");
//     cd.replace("/WIN95/MINI.CAB", new_cab);
//
// Names are compared without regard to case, and the ";1" version of a name is not part of it. A volume that is not
// ISO 9660 is a FormatError; a path the volume does not hold is a NotFoundError; a file that is stored in several
// extents is an UnsupportedError.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "formats/disc_image.hpp"

namespace cb::formats::iso9660 {

inline constexpr std::size_t kSectorSize = 2048;

enum class Access { Read, ReadWrite };

// Which tree of the volume names are read in: the ISO 9660 one (upper case, 8.3 at level 1) or Joliet's (long names).
// A path is found in a tree by its names; both trees point at the same data.
enum class Names { Primary, Joliet };

// A run of a file's data: `size` bytes from the start of sector `sector`.
struct Extent {
    std::uint32_t sector = 0;
    std::uint32_t size = 0;
};

// What a directory holds.
struct Entry {
    std::string name;            // as the volume writes it, without ";1"
    bool directory = false;
    std::uint32_t size = 0;      // bytes (a directory's extent; all of a file's extents)
    std::uint32_t extent = 0;    // the first sector of its data (past the extended attribute record)
    std::array<std::uint8_t, 7> recorded{};   // the recording date: years since 1900, month, day, hour, minute, second, zone
    std::vector<Extent> extents; // a file in several extents (consecutive records of one name) lists each
};

// What the volume descriptors say.
struct Info {
    std::string system, label, created;   // the primary descriptor's system and volume identifiers and creation date (blank-trimmed)
    std::string tree_label;               // the volume identifier of the tree asked for (Joliet's is UCS-2)
    std::uint32_t blocks = 0;             // the volume's size in logical blocks
    std::uint32_t block_size = 2048;
    bool has_joliet = false;
    std::uint32_t root_sector = 0, root_size = 0;   // the root directory of the tree asked for
    std::optional<std::uint32_t> boot_catalog;      // the El Torito boot catalog's sector
};

class Volume {
public:
    // Opens the image: a cooked one (.iso, 2048 bytes a sector), or, read only, a raw one (.bin, 2352) or a .cue sheet's
    // (formats/disc_image). FormatError if it has no primary volume descriptor.
    explicit Volume(const std::filesystem::path& path, Access access = Access::Read);
    // The volume of sectors already open (an image in memory, or a file), read only.
    explicit Volume(disc::Sectors sectors);
    Volume(Volume&&) noexcept;
    Volume& operator=(Volume&&) noexcept;
    ~Volume();

    // Whether the image has a Joliet tree beside the primary one (long names, mixed case).
    bool has_joliet() const;
    // The size of the volume in sectors, as its descriptors say.
    std::uint32_t sector_count() const;

    // What is in a directory, in the order the directory holds it ("/" is the root). `tree`: whose names to read.
    std::vector<Entry> list(std::string_view directory, Names tree = Names::Primary) const;
    // What is in the directory an Entry of `list` (or `root`) names: the same, read where the directory is, without walking a path.
    std::vector<Entry> list(const Entry& directory, Names tree = Names::Primary) const;
    // The root directory as an entry.
    Entry root(Names tree = Names::Primary) const;
    // The contents of a file (all of its extents).
    Bytes read(std::string_view file, Names tree = Names::Primary) const;
    // The contents of a file an Entry of `list` names.
    Bytes read(const Entry& entry) const;
    // What the descriptors say of the volume, with the root and label of `tree`.
    Info info(Names tree = Names::Primary) const;
    // `count` sectors of data from sector `first` on (fewer when the image ends first).
    Bytes read_sectors(std::uint64_t first, std::uint64_t count) const;
    // Replace a file's contents (the volume must be open ReadWrite; UsageError otherwise). See the top of this file.
    void replace(std::string_view file, ByteView data);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::formats::iso9660
