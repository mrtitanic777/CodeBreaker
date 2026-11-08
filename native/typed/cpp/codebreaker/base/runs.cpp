// codebreaker/base/runs.cpp -- C++ from codebreaker.base.runs.

#include "codebreaker/base/runs.hpp"

#include <ctime>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::base::runs {

const int64_t AGE = 24 * 3600;   // a run folder older than this is a killed run's

namespace {

// settings.state(*parts): a path inside the state folder, which is created if it is not there.
py::str state_(const py::str& a, const py::str& b, const py::str& c) {
    py::str path = py::os::path::join(py::settings::get_str(py::str("state")), a, b, c);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

py::str runs_(const py::str& tool) {
    return state_(tool, py::str("runs"), py::str(""));
}

// os.getpid()
int64_t getpid_() {
#ifdef _WIN32
    return static_cast<int64_t>(_getpid());
#else
    return static_cast<int64_t>(getpid());
#endif
}

// time.strftime("%Y%m%d-%H%M%S"): the local time now
py::str stamp_() {
    std::time_t t = std::time(nullptr);
    std::tm lt{};
#ifdef _WIN32
    localtime_s(&lt, &t);
#else
    localtime_r(&t, &lt);
#endif
    char buf[64];
    std::strftime(buf, sizeof buf, "%Y%m%d-%H%M%S", &lt);
    return py::str(buf);
}

// re.sub(r"[^A-Za-z0-9.-]+", "-", s): each run of other characters becomes one "-"
// (written out, so this file needs no regex engine to link).
py::str tidy_(const py::str& s) {
    std::u32string out;
    bool in_run = false;
    for (char32_t c : s.u32()) {
        bool keep = (c >= U'A' && c <= U'Z') || (c >= U'a' && c <= U'z') || (c >= U'0' && c <= U'9') || c == U'.' || c == U'-';
        if (keep) {
            out += c;
            in_run = false;
        } else if (!in_run) {
            out += U'-';
            in_run = true;
        }
    }
    return py::str(out);
}

}  // namespace

// The tool's one folder of what its runs leave (made if it is not there).
py::str results(const py::str& tool, const py::str& name) {
    return state_(tool, name, py::str(""));
}

// Delete a run folder; a machine just stopped lets go of its files a moment later.
bool remove(const py::str& path, int64_t tries) {
    for (int64_t _ = 0; _ < tries; ++_) {
        // shutil.rmtree(path, ignore_errors=True) only ever removes a real folder: on a file or a
        // link it fails, and the error is ignored (the runtime's rmtree would delete the file).
        if (py::os::path::isdir(path) && !py::os::path::islink(path)) {
            py::shutil::rmtree(path, true);
        }
        if (!py::os::path::exists(path)) {
            return true;
        }
        py::time::sleep(0.5);
    }
    return false;
}

// Delete the tool's run folders older than `age` seconds (the killed runs'). -> those deleted
py::list<py::str> sweep(const py::str& tool, double age) {
    py::str base = runs_(tool);
    py::list<py::str> gone;
    double now = py::time::time();
    for (const py::str& n : py::os::listdir(base)) {
        py::str p = py::os::path::join(base, n);
        try {
            if (py::os::path::isdir(p) && now - py::os::path::getmtime(p) > age) {
                if (remove(p, 1)) {
                    gone.append(p);
                }
            }
        } catch (const py::OSError&) {
            // a folder that vanished or cannot be read is left alone
        }
    }
    return gone;
}

// A new run folder of the tool's (the killed runs' swept first). -> its path; the caller
// deletes it with remove() when the run ends (Folder does both).
py::str start(const py::str& tool, const py::str& tag) {
    sweep(tool);
    py::str clean = tidy_(tag).slice(0, 40);
    if (clean.empty()) {
        clean = py::str("run");
    }
    py::str name = clean + py::str("_") + stamp_() + py::str("_") + py::to_str(getpid_());
    py::str path = py::os::path::join(runs_(tool), name);
    int64_t n = 0;
    while (py::os::path::exists(n ? path + py::str("_") + py::to_str(n) : path)) {
        n += 1;
    }
    if (n) {
        path = path + py::str("_") + py::to_str(n);
    }
    py::os::makedirs(path);
    return path;
}

Folder::Folder(const py::str& tool, const py::str& tag) : path_(start(tool, tag)) {}

Folder::~Folder() {
    try {
        remove(path_);
    } catch (...) {
    }
}

}  // namespace codebreaker::base::runs
