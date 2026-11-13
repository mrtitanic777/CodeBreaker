// codebreaker/formats/xbox/volumes.hpp -- COMPAT SHIM over cb/formats/xbox/volumes.hpp (docs/CPP_DESIGN.md 6.3), for the old media port.
// Deleted with its last py:: consumer.
//
// The Xbox and Xbox 360 containers as media volumes are native/cb/formats/xbox/volumes.{hpp,cpp}. The old surface kept is
// `volumes(path)` -> [volume, ...] or nothing, the volumes being the old media::Volume (media::NewVolume over the new ones). The
// module's helpers (_Listed, _stfs, _xdvdfs, _god, _parts, FatxVolume, _fatx, _nand, _zar, _payloads, the ciso / xzp hooks) were its
// own internals and are not carried over. Conversion and forwarding only.
#pragma once

#include <memory>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/xbox/volumes.hpp"
#include "codebreaker/formats/media.hpp"
#include "py.hpp"

namespace codebreaker::formats::xbox::volumes {

using VolumeList = py::list<std::shared_ptr<media::Volume>>;

// The volumes of `path` when it is an Xbox container, else None (it is then some other kind of image, or not one).
// `long_names` is media's option; Xbox names are always long.
inline std::optional<VolumeList> volumes(const py::str& path, bool long_names = true) {
    (void)long_names;
    return ::cb::compat::guard([&]() -> std::optional<VolumeList> {
        std::optional<::cb::formats::xbox::volumes::Volumes> found = ::cb::formats::xbox::volumes::open(::cb::fs::from_text(::cb::compat::from_py(path)));
        if (!found) return std::nullopt;
        VolumeList out;
        for (auto& v : *found) out.append(std::make_shared<media::NewVolume>(std::shared_ptr<::cb::formats::media::Volume>(std::move(v))));
        return out;
    });
}

}  // namespace codebreaker::formats::xbox::volumes
