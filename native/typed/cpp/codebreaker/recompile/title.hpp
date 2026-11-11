// codebreaker/recompile/title.hpp -- typed C++ from codebreaker.recompile.title.
//
// title.py -- an Xbox 360 title as it comes, ready for recompile-project.
//
//     auto t = title::locate(path);       // -> std::optional<Title>: None when `path` holds no title
//     t->read_xex();                      // default.xex's bytes
//     t->unpack(folder);                  // every file of the title under `folder` (once; a marker records it)
//
// A title comes as a LIVE/CON/PIRS package (Xbox LIVE Arcade, installs), a disc image (ISO, CSO), a
// Games-on-Demand folder, a folder of its files, or a folder holding any of those (a console's
// content folders: <title id>/000D0000/<package>). The title is the volume whose top folder holds
// default.xex; packages beside it without one (title updates, DLC) are left alone.
//
// Typing notes:
//   * `volume=None` / `folder=None` are a null std::shared_ptr<Volume> / std::optional<py::str>.
//   * `Title.name` is None only until _from_volumes sets it; it is a std::optional<py::str>.
//   * a volume's `boot` is a {str: str | int} dict (media.hpp); a "display name" that is a non-zero int would be
//     `.lower()`-ed in the Python (AttributeError), and is here too.
//   * shutil.copy2 keeps the file's times; the runtime's copy2 does not, so it is written here.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "codebreaker/formats/media.hpp"
#include "codebreaker/formats/xbox/volumes.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::recompile::title {

namespace media = codebreaker::formats::media;

extern const py::list<py::bytes> _STFS;       // (b"CON ", b"LIVE", b"PIRS")
extern const py::list<py::str> _DISCS;        // (".iso", ".xiso", ".cso", ".cci")
extern const py::str _MARKER;                 // ".codebreaker-unpacked"
extern const int64_t _FORMAT;                 // what an unpack writes; a marker of another format is unpacked again

class Title {
  public:
    py::str source;
    std::optional<py::str> name;

    Title(const py::str& source, const std::optional<py::str>& name, std::shared_ptr<media::Volume> volume = nullptr,
          const std::optional<py::str>& folder = std::nullopt);

    std::optional<media::Entry> _xex_entry();
    py::bytes read_xex();
    // Writes the title's files under `folder` unless they are there from the same source.
    // -> files written (0 when already unpacked).
    int64_t unpack(const py::str& folder);

  private:
    std::shared_ptr<media::Volume> _volume;
    std::optional<py::str> _folder;
};

int64_t _size(const py::str& path);
// The folder's default.xex if it is one (some copies name a LIVE package default.xex).
std::optional<py::str> _default_xex(const py::str& folder);
std::optional<Title> _from_volumes(const py::str& path, const std::optional<formats::xbox::volumes::VolumeList>& found);
// A Title when the file `path` is a package or disc image holding one.
std::optional<Title> _container(const py::str& path);
// The title in `path` (a package, disc image, GoD folder, folder of files, or a folder holding
// any of those), or None.
std::optional<Title> locate(const py::str& path);

}  // namespace codebreaker::recompile::title
