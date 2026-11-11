// codebreaker/base/converted.hpp -- SHIM over cb/base/converted.hpp (docs/CPP_DESIGN.md 6.3): the old types in, the new functions
// called, the old types out. No logic; deleted with its last py:: consumer.
//
// Where CodeBreaker puts what it makes of a binary: one project folder per binary (see cb/base/converted.hpp).
//
//     Project p = converted::project(path);                    // Converted/<name>/, made (original copied in) once
//     py::str out = os::path::join(p.src, "tetris.c");         // ... write the outputs into its folders ...
//     p.record("decompile", {out}, "...");                     // project.json and README.txt say what was done
//
// The old Converted folder is the setting `converted` (settings.json, or the CB_CONVERTED environment variable), else
// Converted/ at the top of the checkout. Its Project has the folders as `py::str` members, as the old callers use them.
//
//   * Not carried over (no old caller reads them): ProjectData's `order`/`other` (the keys a person added to project.json are
//     kept in the file by the new module, but are not shown here), `_GENERIC`, and the Python's TypeError/NotImplementedError
//     corners of a hand-damaged project.json.
//   * A cb::Error reaches the old code as the py:: exception for its meaning (cb::compat::guard).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "cb/base/converted.hpp"
#include "cb/compat/legacy_settings.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::base::converted {

namespace nc = ::cb::base::converted;

inline const py::list<py::str> FOLDERS = py::list<py::str>{py::str("original"), py::str("src"), py::str("assets"), py::str("build"),
                                                           py::str("reports")};

// The current time as datetime.datetime.now().isoformat(timespec="seconds") writes it (a test hook).
inline std::function<py::str()>& _clock() {
    static std::function<py::str()> clock = [] { return py::str(nc::local_time_now()); };
    return clock;
}

// The Converted folder (created on demand).
inline py::str root() {
    const py::str configured = py::settings::get_str(py::str("converted"));
    const std::filesystem::path folder = !configured.empty() ? ::cb::compat::legacy::path_of(configured)
                                                             : ::cb::compat::legacy::path_of(py::settings::checkout_root()) / "Converted";
    return ::cb::compat::guard([&] { return ::cb::compat::legacy::native(nc::Store(folder).root()); });
}

inline py::str _sha1(const py::str& path) {
    return ::cb::compat::guard([&] { return py::str(nc::sha1_of(::cb::compat::legacy::path_of(path))); });
}
inline py::str _clean(const py::str& name) { return py::str(nc::clean_name(name.utf8())); }
inline py::str default_name(const py::str& path) { return py::str(nc::default_name(::cb::compat::legacy::path_of(path))); }
inline py::str _kind(const py::str& path) { return py::str(nc::kind_of(::cb::compat::legacy::path_of(path))); }

// One step of project.json: a command, when it ran, what it made, notes.
struct Step {
    std::optional<py::str> command, when;
    std::optional<py::list<py::str>> outputs;
    std::optional<py::str> notes;
};

// project.json's dict
struct ProjectData {
    std::optional<py::str> name, file, source, sha1, kind;
    std::optional<int64_t> size;
    std::optional<py::list<Step>> steps;
};

class Project {
  public:
    explicit Project(const py::str& folder) : impl_(::cb::compat::legacy::path_of(folder), clock()) { fill(); }
    py::str root, original, src, assets, build, reports, manifest;

    // project.json's contents ({} -- no key set -- when it is missing or damaged)
    ProjectData data() const {
        const nc::Manifest m = impl_.manifest();
        ProjectData d;
        if (m.name) d.name = py::str(*m.name);
        if (m.file) d.file = py::str(*m.file);
        if (m.source) d.source = py::str(*m.source);
        if (m.sha1) d.sha1 = py::str(*m.sha1);
        if (m.kind) d.kind = py::str(*m.kind);
        if (m.size) d.size = static_cast<int64_t>(*m.size);
        if (!m.steps.empty()) {
            py::list<Step> steps;
            for (const nc::Step& s : m.steps) {
                Step o;
                o.command = py::str(s.command);
                o.when = py::str(s.when);
                o.outputs = ::cb::compat::to_py(s.outputs);
                if (!s.notes.empty()) o.notes = py::str(s.notes);
                steps.append(o);
            }
            d.steps = steps;
        }
        return d;
    }
    py::str name() const { return py::str(impl_.name()); }
    // The copy of the binary in original/.
    std::optional<py::str> original_file() const {
        const auto f = impl_.original_file();
        if (!f) return std::nullopt;
        return ::cb::compat::legacy::native(*f);
    }
    // Note a step: the command, the files or folders it made, and anything a reader needs (how to build or run them).
    // Rewrites project.json and README.txt.
    void record(const py::str& command, const py::list<py::str>& outputs = py::list<py::str>(), const py::str& notes = py::str()) const {
        std::vector<std::filesystem::path> paths;
        for (const py::str& o : outputs.vec()) paths.push_back(::cb::compat::legacy::path_of(o));
        ::cb::compat::guard([&] { impl_.record(command.utf8(), paths, notes.utf8()); });
    }

  private:
    friend Project project(const py::str&, const std::optional<py::str>&, const std::optional<py::str>&);
    Project(nc::Project impl) : impl_(std::move(impl)) { fill(); }   // NOLINT
    static nc::Clock clock() { return [] { return _clock()().utf8(); }; }
    void fill() {
        root = ::cb::compat::legacy::native(impl_.root());
        original = ::cb::compat::legacy::native(impl_.original());
        src = ::cb::compat::legacy::native(impl_.src());
        assets = ::cb::compat::legacy::native(impl_.assets());
        build = ::cb::compat::legacy::native(impl_.build());
        reports = ::cb::compat::legacy::native(impl_.reports());
        manifest = ::cb::compat::legacy::native(impl_.manifest_path());
    }
    nc::Project impl_;
};

// The project folder of a binary: found again by its SHA-1, else made (its original copied in).
// `base`: another folder to keep it in (default: the Converted folder).
inline Project project(const py::str& path, const std::optional<py::str>& name = std::nullopt,
                       const std::optional<py::str>& base = std::nullopt) {
    return ::cb::compat::guard([&] {
        const nc::Store store(::cb::compat::legacy::path_of(root()), Project::clock());
        std::optional<std::filesystem::path> base_folder;
        if (base) base_folder = ::cb::compat::legacy::path_of(*base);
        return Project(store.project(::cb::compat::legacy::path_of(path), name ? std::optional<std::string>(name->utf8()) : std::nullopt,
                                     base_folder));
    });
}

}  // namespace codebreaker::base::converted
