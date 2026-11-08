// codebreaker/java/overlays.hpp -- typed C++ of codebreaker.java.overlays
//
// Per-project code overlays -- re-apply hand-fixes on top of freshly generated output.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::overlays {

// Stable per-project overlay key. Uses the output project's basename (e.g. 'MinecraftBeta').
// (out_dir: a str or None; java_dir is not used.)
py::str project_key(const std::optional<py::str>& out_dir, const std::optional<py::str>& java_dir = std::nullopt);

// The per-project hand-fix folder: settings.json `java_overlays`, else state/java/overlays.
py::str overlays_root();

// Pick the overlay dir: explicit arg wins; else <converter>/overlays/<project_key>.
std::optional<py::str> resolve_overlay_dir(const std::optional<py::str>& out_dir,
                                           const std::optional<py::str>& java_dir = std::nullopt,
                                           const std::optional<py::str>& explicit_ = std::nullopt);

// Copy every file under overlay_dir over out_dir (same relative path). Returns the list of applied
// relpaths. Skips MANIFEST.md. Creates missing subdirs. No-op if overlay_dir is absent/empty.
// `prog` is the progress callback (None in the Python: a no-op).
py::list<py::str> apply_overlays(const py::str& out_dir, const std::optional<py::str>& overlay_dir,
                                 const std::function<void(const py::str&)>& prog = nullptr);

}  // namespace codebreaker::java::overlays
