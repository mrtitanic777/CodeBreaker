// formats/xbox/disc_command.hpp -- the `xbox-disc` command: the files of an Xbox 360 disc image, listed, read one at a time or unpacked.
//
//     xbox-disc files  <image>                 every file: its size, a tab, its path on the disc
//     xbox-disc read   <image> <path> <out>    one file of the disc (exit 3 when the disc has none)
//     xbox-disc unpack <image> <folder>        every file, streamed in 8 MiB pieces; a file already there at its full size is kept (an
//                                              interrupted unpack goes on); `PROGRESS <bytes done> <bytes in all>` lines as it goes
//
// An image is a disc image (.iso) or the data folder of a Games on Demand package (the SVOD parts: `Data0000`, ...). It is read with
// gdf.hpp (XDVDFS); nothing is held in memory but the piece being copied. Every file is written whole or not at all: to `<name>.part`,
// then renamed. The folders of a disc are made even when empty.
//
// `files` lists the disc's files by their lower-cased path, a folder's files before the next folder's (gdf.hpp's order).
//
// Errors: FormatError for an image that holds no volume, NotFoundError for one that is not there.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <vector>

#include "core/cli.hpp"
#include "formats/xbox/device.hpp"
#include "formats/xbox/gdf.hpp"

namespace cb::formats::xbox::disc_command {

inline constexpr std::size_t kPiece = 8 << 20;   // the largest read of one file

// A disc image opened: where its bytes come from, and its volume.
struct Image {
    std::shared_ptr<Device> device;
    gdf::Disc disc;
};

// A file (.iso) or a folder (a GoD package's data) as a disc.
Image open_image(const std::filesystem::path& path);

// A file written whole or not at all (its folder made, `<path>.part` renamed).
void write_whole(const std::filesystem::path& path, ByteView data);

// Every file of the disc under `folder`, as `unpack` writes them. `progress` is told (bytes done, bytes in all) when the whole percentage
// changes and at the end. A file already at its full size is not read again. Answers the bytes in all the files.
std::uint64_t unpack(Image& image, const std::filesystem::path& folder, const std::function<void(std::uint64_t done, std::uint64_t total)>& progress);

cli::Command command();

}  // namespace cb::formats::xbox::disc_command
