// codebreaker/java/overlays.hpp -- SHIM over cb/java/overlays.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
#pragma once

#include <functional>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/overlays.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::java::overlays {

// The per-project hand-fix folder: settings.json `java_overlays`, else state/java/overlays (created).
inline py::str overlays_root() {
    py::str set = py::settings::get_str(py::str("java_overlays"));
    if (set.size() > 0) return set;
    py::str path = py::os::path::join(py::settings::get_str(py::str("state")), py::str("java"), py::str("overlays"));
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

// java_dir is not used.
inline py::str project_key(const std::optional<py::str>& out_dir, const std::optional<py::str>& = std::nullopt) {
    return cb::compat::to_py(cb::java::overlays::project_key(
        cb::fs::from_text(out_dir ? cb::compat::from_py(*out_dir) : std::string())));
}

inline std::optional<py::str> resolve_overlay_dir(const std::optional<py::str>& out_dir,
                                                  const std::optional<py::str>& = std::nullopt,
                                                  const std::optional<py::str>& explicit_ = std::nullopt) {
    return cb::compat::guard([&]() -> std::optional<py::str> {
        std::optional<cb::fs::stdfs::path> ex;
        if (explicit_ && explicit_->size() > 0) ex = cb::fs::from_text(cb::compat::from_py(*explicit_));
        const std::string out = out_dir ? cb::compat::from_py(*out_dir) : std::string();
        auto dir = cb::java::overlays::resolve_overlay_dir(cb::fs::from_text(out),
                                                           cb::fs::from_text(cb::compat::from_py(overlays_root())), ex);
        return dir ? std::optional<py::str>(cb::compat::to_py(cb::fs::to_text(*dir))) : std::nullopt;
    });
}

inline py::list<py::str> apply_overlays(const py::str& out_dir, const std::optional<py::str>& overlay_dir,
                                        const std::function<void(const py::str&)>& prog = nullptr) {
    return cb::compat::guard([&] {
        std::optional<cb::fs::stdfs::path> ov;
        if (overlay_dir && overlay_dir->size() > 0) ov = cb::fs::from_text(cb::compat::from_py(*overlay_dir));
        cb::java::overlays::Progress progress;
        if (prog) progress = [&](std::string_view line) { prog(cb::compat::to_py(line)); };
        return cb::compat::to_py(cb::java::overlays::apply_overlays(cb::fs::from_text(cb::compat::from_py(out_dir)), ov, progress));
    });
}

}  // namespace codebreaker::java::overlays
