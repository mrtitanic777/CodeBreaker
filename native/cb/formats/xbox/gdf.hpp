// cb/formats/xbox/gdf.hpp -- XDVDFS ("GDF", the Game Disc Format): the file system of Xbox and Xbox 360 discs, and of what is inside
// Games-on-Demand and installed content.
//
//     auto disc = gdf::Disc::open(std::make_shared<FileDevice>("game.iso"));
//     for (const gdf::Entry& e : disc.entries()) ...        every file and folder
//     Bytes xex = disc.read("default.xex");                  a file by path, case ignored
//     Bytes image = gdf::build({{"default.xex", xex_bytes}});   a minimal disc volume to wrap a title
//
// A volume has a descriptor at its sector 32 (2048-byte sectors) naming the root directory; a directory is a binary tree of entries
// (each gives the places, in 4-byte units, of the entries that sort before and after it, 0xFFFF for none), and files are runs of
// sectors. Where the volume starts in the device depends on the disc layout: the usual XGD offsets are tried, or the caller says (a
// Games-on-Demand package's volume sits at a base its header gives, and its descriptor may be elsewhere than the volume's sector 32).
//
// Names are Latin-1 text, decoded once to UTF-8. `entries()` lists every file and folder sorted by the lower-cased path (Latin-1
// case), a folder before what is in it.
//
// Errors: FormatError when the device holds no volume; NotFoundError from `read` for a path that is not a file.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/formats/xbox/device.hpp"

namespace cb::formats::xbox::gdf {

inline constexpr std::uint32_t kSector = 0x800;
inline constexpr std::string_view kMagic = "MICROSOFT*XBOX*MEDIA";

struct Entry {
    std::string path;           // folders joined with '/'
    bool is_dir = false;
    std::uint32_t start = 0;    // the first sector, from the volume's start
    std::uint32_t size = 0;
};

class Disc {
public:
    // The volume on `device`. `base` is where the volume starts (bytes, may be negative: a package whose first sectors are not in
    // its parts); without it the usual layouts are tried in turn. `descriptor` is where its descriptor is (bytes), by default
    // the volume's sector 32. FormatError when there is no volume.
    static Disc open(std::shared_ptr<Device> device, std::optional<std::int64_t> base = std::nullopt,
                     std::optional<std::int64_t> descriptor = std::nullopt);

    std::int64_t base() const { return base_; }
    std::uint32_t root_sector() const { return root_sector_; }
    std::uint32_t root_size() const { return root_size_; }

    // Every file and folder of the volume (read once, then kept).
    const std::vector<Entry>& entries();
    // The files only, in the same order.
    std::vector<Entry> files();
    // The bytes of a file (a folder is none); nullopt when there is no such file. Paths compare without regard to Latin-1 case.
    std::optional<Bytes> try_read(std::string_view path);
    // The same; NotFoundError for a path that is not a file.
    Bytes read(std::string_view path);
    // The bytes of the file `entry` names.
    Bytes read(const Entry& entry);

private:
    Disc() = default;
    std::vector<Entry> walk(std::uint32_t sector, std::uint32_t size, const std::string& prefix, std::unordered_set<std::uint32_t>& seen);

    std::shared_ptr<Device> device_;
    std::int64_t base_ = 0;
    std::uint32_t root_sector_ = 0, root_size_ = 0;
    std::optional<std::vector<Entry>> entries_;
};

// A file to place on a disc built by `build`.
struct SourceFile {
    std::string name;   // Latin-1 characters only; no folders
    Bytes data;
};

// A minimal XDVDFS volume holding `files` in a flat root folder: the descriptor at sector 32, the root directory, then the file data.
// Enough to wrap a default.xex into a title. The directory is ordered by upper-cased name. UsageError for a name outside Latin-1 or
// empty or longer than 255 characters.
Bytes build(std::span<const SourceFile> files);

// ---- the command ---------------------------------------------------------------------------------------------------------------

std::string_view usage();
// `gdf info|list|extract <image.iso|svod-dir> [path] [out]`: the report on stdout, the exit code (0; 2 with the usage for a command
// line it does not understand). Failures to read are thrown.
int command(std::span<const std::string> args);

}  // namespace cb::formats::xbox::gdf
