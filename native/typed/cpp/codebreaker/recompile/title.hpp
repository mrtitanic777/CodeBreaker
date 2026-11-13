// codebreaker/recompile/title.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/recompile/title.{hpp,cpp}; this
// header keeps the old py:: surface (Title, locate) for recompile/project and the dynamic exe. Conversion and forwarding only: the
// containers are read by native/cb/formats/xbox/volumes.
//
// Not carried over: the internals the module's own port exposed (_STFS, _DISCS, _MARKER, _FORMAT, _size, _default_xex, _from_volumes,
// _container, Title::_xex_entry).
#pragma once

#include <memory>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/recompile/title.hpp"
#include "py.hpp"

namespace codebreaker::recompile::title {

class Title {
  public:
    py::str source;
    std::optional<py::str> name;

    explicit Title(cb::recompile::title::Title&& made)
        : source(cb::compat::to_py(cb::fs::to_text(made.source()))),
          name(cb::compat::to_py(made.name())),
          made_(std::make_shared<cb::recompile::title::Title>(std::move(made))) {}

    py::bytes read_xex() {
        return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(made_->read_xex())); });
    }
    // -> files written (0 when already unpacked)
    int64_t unpack(const py::str& folder) {
        return cb::compat::guard([&] { return static_cast<int64_t>(made_->unpack(cb::fs::from_text(cb::compat::from_py(folder)))); });
    }

  private:
    std::shared_ptr<cb::recompile::title::Title> made_;
};

// The title in `path` (a package, disc image, GoD folder, folder of files, or a folder holding any of those), or None.
inline std::optional<Title> locate(const py::str& path) {
    return cb::compat::guard([&]() -> std::optional<Title> {
        std::optional<cb::recompile::title::Title> made = cb::recompile::title::locate(cb::fs::from_text(cb::compat::from_py(path)));
        if (!made) return std::nullopt;
        return Title(std::move(*made));
    });
}

}  // namespace codebreaker::recompile::title
