// codebreaker/recompile/xenon.hpp -- compat shim for cb/recompile/xenon.hpp (docs/CPP_DESIGN.md 6.3). Conversion and
// forwarding; deleted with its last old-layer consumer (recompile/x360project).
//
// xenon.py -- XenonRecomp, the Xbox 360 recompiler CodeBreaker's projects are generated with.
//
//     xenon::build();                       // native/xenonrecomp -> build/XenonRecomp/XenonRecomp.exe
//     xenon::recompile(config_toml);        // runs it on a project's configuration (config.py writes one)
//     xenon::recompile_synced(config_toml); // the same into a clean folder, then only the changed files into the
//                                           // configuration's output folder (and the stale ones out)
//     xenon::instruction_tests();           // Xenia's PowerPC tests through it: {tested, skipped, mismatches}
//
// The old module's locations (ROOT, SOURCE, BUILD, RECOMP, ANALYSE, TESTS) are plain variables here, set from the
// checkout root as the import did; every call makes a cb::recompile::xenon::Xenon from them and the settings
// (visual_studio, ppc_binutils, state). The Python's subprocess helpers (_run, _run_shell) and its mtime scan are the
// new module's own and are not repeated.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/recompile/xenon.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::recompile::xenon {

// The module's locations. In Python they are fixed at import from the module's own place in the checkout (ROOT is
// three folders above xenon.py); set_root derives them from a checkout root, and each is a plain variable so a caller
// (or a test) can point any one of them elsewhere. This header sits six folders below the checkout.
inline py::str ROOT;
inline py::str SOURCE;
inline py::str BUILD;
inline py::str RECOMP;
inline py::str ANALYSE;
inline py::str TESTS;
// a host-side helper, and a test that needs a switch-table configuration test mode has not
inline const std::tuple<py::str, py::str> SKIP_TESTS{py::str("ppc_testing_native_thunks"), py::str("seq_jumptable_constants")};

// Derive ROOT, SOURCE, BUILD, RECOMP, ANALYSE and TESTS from a checkout root (what the import does).
inline void set_root(const py::str& root) {
    ROOT = root;
    SOURCE = py::os::path::join(ROOT, py::str("native"), py::str("xenonrecomp"));
    BUILD = py::os::path::join(SOURCE, py::str("build"));
    RECOMP = py::os::path::join(BUILD, py::str("XenonRecomp"), py::str("XenonRecomp.exe"));
    ANALYSE = py::os::path::join(BUILD, py::str("XenonAnalyse"), py::str("XenonAnalyse.exe"));
    TESTS = py::os::path::join(SOURCE, py::str("ppc_tests"));
}

namespace detail {

inline py::str default_root() {
    py::str here = py::os::path::abspath(py::str(__FILE__));
    for (int i = 0; i < 6; ++i) here = py::os::path::dirname(here);
    return here;
}
inline const bool root_set = (set_root(default_root()), true);

// the new module's object, from the current locations and the settings
inline cb::recompile::xenon::Xenon make() {
    cb::recompile::xenon::Config c;
    c.checkout_root = cb::fs::from_text(ROOT.utf8());
    c.state_dir = cb::fs::from_text(py::settings::get_str(py::str("state")).utf8());
    const std::string vs = py::settings::get_str(py::str("visual_studio")).utf8();
    if (!vs.empty()) c.visual_studio = cb::fs::from_text(vs);
    const std::string binutils = py::settings::get_str(py::str("ppc_binutils")).utf8();
    if (!binutils.empty()) c.ppc_binutils = cb::fs::from_text(binutils);
    return cb::recompile::xenon::Xenon(std::move(c));
}

inline py::list<py::str> to_py_list(const std::vector<std::string>& v) {
    py::list<py::str> out;
    for (const std::string& s : v) out.append(cb::compat::to_py(s));
    return out;
}

}  // namespace detail

// The Visual Studio install that has clang-cl.
inline py::str visual_studio() {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::fs::to_text(detail::make().visual_studio())); });
}

inline py::str clang_cl() {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::fs::to_text(detail::make().clang_cl())); });
}

// The environment vcvars64.bat sets up (the linker, the SDK's headers and libraries), cached after the first use.
// (the module's `_env`; nullopt until msvc_env first runs)
inline std::optional<py::dict<py::str, py::str>> _env;

inline py::dict<py::str, py::str> msvc_env() {
    if (!_env) {
        cb::compat::guard([&] {
            cb::recompile::xenon::Xenon x = detail::make();
            py::dict<py::str, py::str> env;
            for (const auto& [k, v] : x.msvc_env()) env.set(cb::compat::to_py(k), cb::compat::to_py(v));
            _env = env;
        });
    }
    return *_env;
}

// Build XenonRecomp and XenonAnalyse (only when a source is newer than the executables).
inline py::str build(bool force = false) {
    return cb::compat::guard([&] {
        cb::recompile::xenon::Xenon x = detail::make();
        return cb::compat::to_py(cb::fs::to_text(x.build(force)));
    });
}

// Run XenonRecomp on a configuration file. -> its output (warnings, the unimplemented).
inline py::str recompile(const py::str& config) {
    return cb::compat::guard([&] {
        cb::recompile::xenon::Xenon x = detail::make();
        return cb::compat::to_py(x.recompile(cb::fs::from_text(config.utf8())));
    });
}

// recompile_synced's result: {"generated", "changed", "stale": [names], "log": [its warning lines]}
struct SyncResult {
    int64_t generated = 0;
    int64_t changed = 0;
    py::list<py::str> stale;
    py::list<py::str> log;
};
template <class F> void for_each_field_mut(SyncResult& r, F&& f) {
    f("generated", r.generated, true);
    f("changed", r.changed, true);
    f("stale", r.stale, true);
    f("log", r.log, true);
}

// XenonRecomp on `config` into a clean folder, then into its out_directory_path only the files that changed (the
// others keep their times, so builds stay incremental) and out the files this run did not make. `report` takes each
// line to show (default: print it).
inline SyncResult recompile_synced(const py::str& config, const std::function<void(const py::str&)>& report = nullptr) {
    return cb::compat::guard([&] {
        cb::recompile::xenon::Xenon x = detail::make();
        const auto r = x.recompile_synced(cb::fs::from_text(config.utf8()), [&](std::string_view line) {
            const py::str s = cb::compat::to_py(line);
            if (report) report(s);
            else py::print(s);
        });
        SyncResult out;
        out.generated = static_cast<int64_t>(r.generated);
        out.changed = static_cast<int64_t>(r.changed);
        out.stale = detail::to_py_list(r.stale);
        out.log = detail::to_py_list(r.log);
        return out;
    });
}

// The command line without the program name (sys.argv[1:]); prints to stdout. -> the exit code
inline int64_t main(const py::list<py::str>& argv) {
    if (argv.size() != 1) {
        py::print(py::str("usage: python -m codebreaker xenon-recompile <config.toml>"));
        return 2;
    }
    try {
        recompile_synced(argv[0]);
    } catch (const py::RuntimeError& e) {
        py::print(py::str(e.what()));
        return 1;
    } catch (const py::ValueError& e) {
        py::print(py::str(e.what()));
        return 1;
    }
    return 0;
}

// instruction_tests' result: {"tested": n, "skipped": [...], "mismatches": [...], "failed": [...]}
struct InstructionTestsResult {
    int64_t tested = 0;
    py::list<py::str> skipped;
    py::list<py::str> mismatches;
    py::list<py::str> failed;
};
template <class F> void for_each_field_mut(InstructionTestsResult& r, F&& f) {
    f("tested", r.tested, true);
    f("skipped", r.skipped, true);
    f("mismatches", r.mismatches, true);
    f("failed", r.failed, true);
}

// Xenia's PowerPC instruction tests (and ours) through XenonRecomp. `names` limits the tests (empty or none: all);
// `work` is the folder they are built in (empty or none: the state folder's xenonrecomp/ppc_tests).
inline InstructionTestsResult instruction_tests(const std::optional<py::list<py::str>>& names = std::nullopt,
                                                const std::optional<py::str>& work = std::nullopt,
                                                const py::str& march = py::str("haswell")) {
    return cb::compat::guard([&] {
        cb::recompile::xenon::Xenon x = detail::make();
        cb::recompile::xenon::InstructionTestOptions options;
        if (names) {
            for (const py::str& n : *names) options.names.push_back(n.utf8());
        }
        if (work && !work->empty()) options.work = cb::fs::from_text(work->utf8());
        options.march = march.utf8();
        const auto r = x.instruction_tests(options);
        InstructionTestsResult out;
        out.tested = static_cast<int64_t>(r.tested);
        out.skipped = detail::to_py_list(r.skipped);
        out.mismatches = detail::to_py_list(r.mismatches);
        out.failed = detail::to_py_list(r.failed);
        return out;
    });
}

}  // namespace codebreaker::recompile::xenon
