// cb/recompile/title.hpp -- an Xbox 360 title as it comes, ready for recompile-project.
//
//     std::optional<Title> t = title::locate(path, find_volumes);   nothing when `path` holds no title
//     Bytes xex = t->read_xex();                                    default.xex's bytes
//     std::size_t n = t->unpack(folder);                            every file of the title under `folder` (once; a marker records it)
//
// A title comes as a LIVE/CON/PIRS package (Xbox LIVE Arcade, installs), a disc image (ISO, CSO), a Games-on-Demand folder, a folder of
// its files, or a folder holding any of those (a console's content folders: <title id>/000D0000/<package>). The title is the volume
// whose top folder holds default.xex; packages beside it without one (title updates, DLC) are left alone.
//
// The Xbox containers (STFS, XDVDFS, GoD, CSO / CCI) are read as volumes by formats/xbox/volumes: `locate(path)` uses it. A caller that
// has its own way to open containers (a test with a fake one) hands it in as `FindVolumes`.
#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/media.hpp"

namespace cb::recompile::title {

using Volumes = std::vector<std::unique_ptr<formats::media::Volume>>;

// The volumes of `path` when it is an Xbox container, nothing when it is some other kind of file. A damaged container may throw a
// cb::Error: the scan of a folder takes it as "not a title" and goes on.
using FindVolumes = std::function<std::optional<Volumes>(const std::filesystem::path&)>;

// What an unpack writes into its marker: a marker of another format is unpacked again.
inline constexpr int kMarkerFormat = 2;
// The marker file an unpack leaves in its folder.
inline constexpr const char* kMarkerName = ".codebreaker-unpacked";

class Title {
public:
    // What the title was found in: the package or disc image, or the folder of its files.
    const std::filesystem::path& source() const { return source_; }
    // A name for the project: the volume's display name or label, or the folder's or file's own name, ASCII only ("title" when nothing
    // is left).
    const std::string& name() const { return name_; }

    // default.xex. NotFoundError when the title has none (a folder is a title only if it has one, so it was removed since).
    Bytes read_xex() const;

    // Writes the title's files under `folder` unless they are there from the same source (a marker in the folder says so), and
    // answers how many it wrote (0 when already unpacked). A name inside a container that would leave the folder is made safe.
    std::size_t unpack(const std::filesystem::path& folder) const;

private:
    friend std::optional<Title> locate(const std::filesystem::path&, const FindVolumes&);
    friend std::optional<Title> from_volumes(const std::filesystem::path&, std::optional<Volumes>);

    Title(std::filesystem::path source, std::string name) : source_(std::move(source)), name_(std::move(name)) {}

    std::filesystem::path source_;
    std::string name_;
    std::unique_ptr<formats::media::Volume> volume_;   // a container's volume; none for a folder of files
    std::optional<std::size_t> xex_;                   // index of default.xex in the volume's files
    std::optional<std::filesystem::path> folder_;      // a folder of files
};

// The size of a file, or of every file under a folder: what the unpack marker records of its source.
std::uint64_t size_of(const std::filesystem::path& path);

// The name of the file in `folder` that is default.xex (some copies name a LIVE package default.xex): matched by name without regard
// to case, and by its first four bytes, "XEX2".
std::optional<std::string> default_xex(const std::filesystem::path& folder);

// The title among `volumes` (the first whose top folder holds default.xex) found in `path`, or nothing.
std::optional<Title> from_volumes(const std::filesystem::path& path, std::optional<Volumes> volumes);

// The title in `path` (a package, disc image, GoD folder, folder of files, or a folder holding any of those), or nothing. Folders are
// searched depth first in name order, a folder's own files before its subfolders.
std::optional<Title> locate(const std::filesystem::path& path, const FindVolumes& find_volumes);
// The same, the containers read by formats/xbox/volumes.
std::optional<Title> locate(const std::filesystem::path& path);

}  // namespace cb::recompile::title
