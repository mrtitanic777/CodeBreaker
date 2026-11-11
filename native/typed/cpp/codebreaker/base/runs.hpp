// codebreaker/base/runs.hpp -- SHIM over cb/base/runs.hpp (docs/CPP_DESIGN.md 6.3): the old types in, the new functions
// called, the old types out. No logic; deleted with its last py:: consumer.
//
// A run's own folder, gone with the run (see cb/base/runs.hpp). The state folder is the old settings' "state".
#pragma once

#include <chrono>

#include "cb/base/runs.hpp"
#include "cb/compat/legacy_settings.hpp"

namespace codebreaker::base::runs {

// a run folder older than this is a killed run's (seconds)
inline const int64_t AGE = 24 * 3600;

// The tool's one folder of what its runs leave (made if it is not there).
inline py::str results(const py::str& tool, const py::str& name = py::str("shots")) {
    return cb::compat::legacy::native(
        cb::base::runs::results(cb::compat::legacy::state_dir(), cb::compat::from_py(tool), cb::compat::from_py(name)));
}

// Delete a run folder; a machine just stopped lets go of its files a moment later.
inline bool remove(const py::str& path, int64_t tries = 20) {
    return cb::base::runs::remove(cb::compat::legacy::path_of(path), static_cast<int>(tries));
}

// Delete the tool's run folders older than `age` seconds (the killed runs'). -> those deleted
inline py::list<py::str> sweep(const py::str& tool, double age = 24.0 * 3600.0) {
    py::list<py::str> gone;
    for (const auto& p : cb::base::runs::sweep(cb::compat::legacy::state_dir(), cb::compat::from_py(tool),
                                               std::chrono::seconds(static_cast<std::int64_t>(age))))
        gone.append(cb::compat::legacy::native(p));
    return gone;
}

// A new run folder of the tool's (the killed runs' swept first). -> its path; the caller deletes it with remove() when the
// run ends (Folder does both).
inline py::str start(const py::str& tool, const py::str& tag = py::str("run")) {
    return cb::compat::legacy::native(
        cb::base::runs::start(cb::compat::legacy::state_dir(), cb::compat::from_py(tool), cb::compat::from_py(tag)));
}

// a run folder for the length of a scope, deleted at its end, however it ends
// (Python's `with runs.folder(tool, tag) as work:` is `runs::Folder work(tool, tag);`).
class Folder {
public:
    Folder(const py::str& tool, const py::str& tag = py::str("run"))
        : folder_(cb::compat::legacy::state_dir(), cb::compat::from_py(tool), cb::compat::from_py(tag)),
          path_(cb::compat::legacy::native(folder_.path())) {}
    Folder(const Folder&) = delete;
    Folder& operator=(const Folder&) = delete;
    const py::str& path() const { return path_; }

private:
    cb::base::runs::Folder folder_;
    py::str path_;
};

}  // namespace codebreaker::base::runs
