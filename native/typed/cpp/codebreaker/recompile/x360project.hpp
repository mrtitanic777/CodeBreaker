// codebreaker/recompile/x360project.hpp -- SHIM over cb/recompile/x360project.hpp (docs/CPP_DESIGN.md 6.3): an Xbox 360 title as a PC
// project on CodeBreaker's Xbox 360 runtime (see the new header for the layout of the project folder).
//
//     auto info = x360project::scaffold(xex_path, folder, name, title_files);
//
// The new module is given XenonRecomp's tool chain as a `Recompiler`; this shim fills it from the old ports (recompile/xenon, config,
// discover, formats/xex), which have not been refactored yet. Deleted with its last py:: consumer (recompile/project).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/recompile/x360project.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/recompile/config.hpp"
#include "codebreaker/recompile/discover.hpp"
#include "codebreaker/recompile/xenon.hpp"
#include "py_os.hpp"

namespace codebreaker::recompile::x360project {

// The checkout root (three folders above the Python module); a plain variable a test may point elsewhere.
inline py::str ROOT = py::str("");
inline void set_root(const py::str& root) { ROOT = root; }

// the program's, target's and scripts' name from a .xex path: its stem, lower case, letters and digits kept
inline py::str _name(const py::str& xex_path) { return cb::compat::to_py(cb::recompile::x360project::default_name(cb::fs::from_text(cb::compat::from_py(xex_path)))); }

// Make dst hold src's files, rewriting only those that changed and removing the rest.
inline void _sync(const py::str& src, const py::str& dst) {
    cb::compat::guard([&] { cb::recompile::x360project::sync_folder(cb::fs::from_text(cb::compat::from_py(src)), cb::fs::from_text(cb::compat::from_py(dst))); });
}

// scaffold's result: {"name", "dir", "functions", "switches", "unimplemented"}
struct ScaffoldInfo {
    py::str name;
    py::str dir;
    int64_t functions = 0;
    int64_t switches = 0;
    int64_t unimplemented = 0;
};
template <class F> void for_each_field_mut(ScaffoldInfo& r, F&& f) {
    f("name", r.name, true);
    f("dir", r.dir, true);
    f("functions", r.functions, true);
    f("switches", r.switches, true);
    f("unimplemented", r.unimplemented, true);
}

namespace shim_ {

// XenonRecomp's tool chain as the old ports know it.
inline cb::recompile::x360project::Recompiler recompiler() {
    namespace cbx = cb::recompile::x360project;
    const auto path = [](const py::str& s) { return cb::fs::from_text(cb::compat::from_py(s)); };
    cbx::Recompiler rc;
    rc.analyse = path(xenon::ANALYSE);
    rc.recomp = path(xenon::RECOMP);
    rc.source = path(xenon::SOURCE);
    rc.visual_studio = path(xenon::visual_studio());
    rc.runtime_cmake = path(py::os::path::join(ROOT, py::str("native"), py::str("x360"), py::str("x360.cmake")));
    rc.ensure_built = [] { cb::compat::guard([] { xenon::build(); }); };
    rc.write_basic = [](const std::filesystem::path& xex, const std::filesystem::path& out) {
        cb::compat::guard([&] { formats::xex::basic(cb::compat::to_py(cb::fs::to_text(xex)), cb::compat::to_py(cb::fs::to_text(out))); });
    };
    rc.write_switch_tables = [](const std::filesystem::path& file, const std::filesystem::path& basic, const std::optional<std::string>& analysed) {
        return cb::compat::guard([&] {
            const auto [found, added] = config::write_switch_tables(cb::compat::to_py(cb::fs::to_text(file)), cb::compat::to_py(cb::fs::to_text(basic)),
                                                                    analysed ? std::optional<py::str>(cb::compat::to_py(*analysed)) : std::nullopt);
            return cbx::Recompiler::SwitchTables{static_cast<std::size_t>(found), static_cast<std::size_t>(added)};
        });
    };
    rc.make_config = [](const std::filesystem::path& basic, std::string_view out_dir, std::string_view tables, std::string_view file_path) {
        return cb::compat::guard([&] {
            return cb::compat::from_py(config::make(cb::compat::to_py(cb::fs::to_text(basic)), cb::compat::to_py(out_dir), cb::compat::to_py(tables),
                                                    std::nullopt, std::optional<py::str>(cb::compat::to_py(file_path))));
        });
    };
    rc.savegprlr_14_address = [](const std::filesystem::path& basic) {
        return cb::compat::guard([&] {
            const auto helpers = config::helpers(*discover::_image(cb::compat::to_py(cb::fs::to_text(basic))));
            return static_cast<std::uint32_t>(helpers.get(py::str("savegprlr_14_address"), 0));
        });
    };
    return rc;
}

}  // namespace shim_

// `name` and `title_files` are None by default (the name is taken from the .xex; no title folder is copied).
inline ScaffoldInfo scaffold(const py::str& xex_path, const py::str& out_dir, const std::optional<py::str>& name = std::nullopt,
                             const std::optional<py::str>& title_files = std::nullopt) {
    return cb::compat::guard([&] {
        const auto info = cb::recompile::x360project::scaffold(
            shim_::recompiler(), cb::fs::from_text(cb::compat::from_py(xex_path)), cb::fs::from_text(cb::compat::from_py(out_dir)),
            name ? cb::compat::from_py(*name) : std::string(),
            title_files ? std::optional<std::filesystem::path>(cb::fs::from_text(cb::compat::from_py(*title_files))) : std::nullopt);
        return ScaffoldInfo{cb::compat::to_py(info.name), cb::compat::to_py(cb::fs::to_text(info.dir)), static_cast<int64_t>(info.functions),
                            static_cast<int64_t>(info.switches), static_cast<int64_t>(info.unimplemented)};
    });
}

// Runs Build-<name>.bat with at most `jobs` compiles at once. -> (ok, the last lines of its output)
inline std::tuple<bool, py::list<py::str>> build(const py::str& out_dir, const py::str& name, int64_t jobs = 4) {
    return cb::compat::guard([&] {
        const auto r = cb::recompile::x360project::build(cb::fs::from_text(cb::compat::from_py(out_dir)), cb::compat::from_py(name), static_cast<unsigned>(jobs));
        return std::make_tuple(r.ok, cb::compat::to_py(r.tail));
    });
}

// check's result: {"frames", "unimplemented": {import: calls}, "fatal": message or "", "exit", "log"}
struct CheckResult {
    int64_t frames = 0;
    py::dict<py::str, int64_t> unimplemented;
    py::str fatal;
    std::optional<int64_t> exit;   // the program's exit code (empty when it had to be ended)
    py::str log;
};
template <class F> void for_each_field_mut(CheckResult& r, F&& f) {
    f("frames", r.frames, true);
    f("unimplemented", r.unimplemented, true);
    f("fatal", r.fatal, true);
    f("exit", r.exit, true);
    f("log", r.log, true);
}

// Runs the built title headless for `seconds` and reads its log.
inline CheckResult check(const py::str& out_dir, const py::str& name, int64_t seconds = 60) {
    return cb::compat::guard([&] {
        const auto r = cb::recompile::x360project::check(cb::fs::from_text(cb::compat::from_py(out_dir)), cb::compat::from_py(name), static_cast<unsigned>(seconds));
        CheckResult out;
        out.frames = static_cast<int64_t>(r.frames);
        for (const auto& [import, calls] : r.unimplemented) out.unimplemented.set(cb::compat::to_py(import), static_cast<int64_t>(calls));
        out.fatal = cb::compat::to_py(r.fatal);
        if (r.exit) out.exit = static_cast<int64_t>(*r.exit);
        out.log = cb::compat::to_py(cb::fs::to_text(r.log));
        return out;
    });
}

}  // namespace codebreaker::recompile::x360project
