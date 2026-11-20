// build/disc_image.hpp -- write an ISO 9660 CD image from a folder.
//
//     disc_image::write(folder, "out.iso", {.volume_id = "CODEBREAKER", .boot_image = "boot.img"});   // -> the image's size
//
// The writer produces a plain level-1 image (8.3 upper-case names, no Joliet or Rock Ridge extensions), optionally bootable through an
// El Torito boot record with floppy emulation. File contents are copied through in pieces, never held in memory whole. Names are
// made to fit: upper-cased, characters other than A-Z 0-9 _ become _ (one for each character of the name), cut to 8 characters plus a
// 3-character extension; if two names in one folder end up the same the write is refused and the clashes are listed (DiscError),
// instead of producing an image with two files of one name.
//
// Reading an image back (and floppy and hard-disk images) is cb/formats/media.hpp; the --list and --extract forms of the command go
// there:
//
//     disc <folder> <out.iso> [--volume NAME] [--boot IMG]
//     disc --list <image.iso>
//     disc --extract <image.iso> <outdir>
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "core/cli.hpp"
#include "core/error.hpp"

namespace cb::build::disc_image {

inline constexpr std::size_t kSectorSize = 2048;

// The folder cannot be written as an ISO 9660 image: names that clash once cut to 8.3, folders nested deeper than the 8 levels the
// standard allows, more than 65535 folders, a file of 4 GB or more, or a path that is not a folder.
class DiscError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(DiscError)
};

struct Options {
    std::string volume_id = "CODEBREAKER";                // upper-cased and cut like a name; at most 32 characters
    std::optional<std::filesystem::path> boot_image;      // a floppy image (normally 1474560 bytes) makes the disc bootable
};

// The name a file or folder gets on the disc: (base, extension). A folder has no extension.
std::pair<std::string, std::string> level1_name(std::string_view name, bool is_dir);

// Writes the contents of `folder` as an ISO 9660 image at `out` and returns the image's size in bytes. DiscError for the reasons above;
// IoError when a file cannot be read or the image cannot be written.
std::uint64_t write(const std::filesystem::path& folder, const std::filesystem::path& out, const Options& options = {});

// The volume name a folder's image gets: its name in capitals, a character ISO 9660 does not allow as '_', at most 32 (CODEBREAKER when empty).
std::string volume_name(std::string_view folder_name);

struct Environment {};

// `disc <folder> <out.iso> [--volume NAME] [--boot IMG]` writes and prints "wrote <out>: N bytes"; `disc --list <image>` lists the
// image's volumes with their boot fields; `disc --extract <image> <outdir>` writes its files and prints how many. A refusal (DiscError)
// or an unreadable file is "disc: <message>" on stderr and exit 1. `disc <folder>` alone writes <folder>.iso beside it; `disc <image>` alone extracts it
// into a folder beside it named after it.
cli::Command command(const Environment& environment = {});

}  // namespace cb::build::disc_image
