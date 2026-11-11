// cb/formats/iso9660.hpp -- an ISO 9660 volume (with its Joliet names) read, and a file in it replaced in place.
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

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::iso9660 {

inline constexpr std::size_t kSectorSize = 2048;

enum class Access { Read, ReadWrite };

// Which tree of the volume names are read in: the ISO 9660 one (upper case, 8.3 at level 1) or Joliet's (long names).
// A path is found in a tree by its names; both trees point at the same data.
enum class Names { Primary, Joliet };

// What a directory holds.
struct Entry {
    std::string name;            // as the volume writes it, without ";1"
    bool directory = false;
    std::uint32_t size = 0;      // bytes (a directory's extent)
};

class Volume {
public:
    // Opens the image. FormatError if it has no primary volume descriptor.
    explicit Volume(const std::filesystem::path& path, Access access = Access::Read);
    Volume(Volume&&) noexcept;
    Volume& operator=(Volume&&) noexcept;
    ~Volume();

    // Whether the image has a Joliet tree beside the primary one (long names, mixed case).
    bool has_joliet() const;
    // The size of the volume in sectors, as its descriptors say.
    std::uint32_t sector_count() const;

    // What is in a directory, in the order the directory holds it ("/" is the root). `tree`: whose names to read.
    std::vector<Entry> list(std::string_view directory, Names tree = Names::Primary) const;
    // The contents of a file.
    Bytes read(std::string_view file, Names tree = Names::Primary) const;
    // Replace a file's contents (the volume must be open ReadWrite; UsageError otherwise). See the top of this file.
    void replace(std::string_view file, ByteView data);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::formats::iso9660
