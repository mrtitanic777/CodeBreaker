// cb/formats/xbox/volumes.hpp -- the Xbox and Xbox 360 containers as media volumes.
//
//     if (auto found = volumes::open(path)) for (auto& v : *found) { v->kind(); for (auto& e : v->files()) v->read(e); }
//
// Each volume has the shape cb/formats/media.hpp gives every volume (kind, label, where, boot fields, files, read), so `media list`
// and `media extract` read these as they read a floppy or a CD:
//
//   stfs       an STFS package (CON / LIVE / PIRS): saves, DLC, title updates, installs
//   xdvdfs     an Xbox / Xbox 360 disc image (XISO, redump XGD), plain or CSO / CCI compressed
//   god        a Games-on-Demand folder (its Data#### parts are the disc, de-interleaved), or its header beside a `.data` folder
//   parts      the Data#### files of a package that is not a disc (a video download is its .wmv cut into them), as they are
//   fatx       each FATX partition of a hard-disk or memory-unit image
//   nand       the flash file system of a console's NAND dump
//   heavyiron  a Heavy Iron GoodEngine archive (.ho): each asset by layer, id and type
//   zar        a ZArchive (.zar)
//   xzp        a XUI package (XUIZ)
//   pck        a Minecraft Legacy Console Edition .pck
//
// Errors: a damaged container is a cb::Error (FormatError, TruncatedError, IoError); a file that is none of these is `nullopt`.
#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <vector>

#include "cb/formats/media.hpp"

namespace cb::formats::xbox::volumes {

using Volumes = std::vector<std::unique_ptr<media::Volume>>;

// The volumes of `path` (a file, or a Games-on-Demand folder) when it is an Xbox container, nullopt when it is some other kind of
// file. Xbox names are always long, so there is no long-names option.
std::optional<Volumes> open(const std::filesystem::path& path);

}  // namespace cb::formats::xbox::volumes
