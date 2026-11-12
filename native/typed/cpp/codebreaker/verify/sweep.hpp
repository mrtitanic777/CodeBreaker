// codebreaker/verify/sweep.hpp -- COMPAT SHIM over cb/verify/sweep.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/sweep.{hpp,cpp} now (`options_of`, `status`, `run` over `Services`). This header keeps the old py:: API:
// `_wanted`, `status` and `main` over the old function store, the old corpus and the old lane registry (verify/reconstruct.hpp's shim).
//
//     python -m codebreaker sweep                    every 32-bit module not yet measured
//     python -m codebreaker sweep --only a.dll,b.dll these;  --only @list.txt  one module name per line
//     python -m codebreaker sweep --16 | --drivers | --dos | --all     the kind of module
//     python -m codebreaker sweep --status           what has been measured; no work
//     python -m codebreaker sweep --again            measure modules again even if already done
#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>

#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/verify/sweep.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/formats/dos.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::verify::sweep {

namespace detail {

// The new sweep's services over the old world: the lanes registered with the reconstruct shim, the old corpus and the store `store` (the files `fs` is).
struct Sweeping {
    cb::engine::FunctionStore store;
    cb::verify::reconstruct::Reconstructor rec;
    cb::verify::sweep::Services services;

    Sweeping(const std::shared_ptr<engine::native_engine::FunctionStore>& fs, const std::function<void(const py::str&)>& log)
        : store(cb::compat::legacy::path_of(fs->path), cb::compat::legacy::path_of(fs->folder)),
          rec(reconstruct::detail::services_over(store, [](const py::str&) {}, [](const std::filesystem::path& p) -> std::optional<std::string> {
              const auto s = base::corpus::sha(cb::compat::to_py(p.generic_string()));
              return s ? std::optional<std::string>(s->utf8()) : std::nullopt;
          })),
          services{.reconstruct = rec, .store = store} {
        services.modules = [](std::string_view kind) {
            std::vector<std::filesystem::path> out;
            const py::str k{std::string(kind)};
            if (kind == "le") {
                for (const py::str& p : base::corpus::modules(std::optional<py::str>(k), py::list<py::str>{py::str(".vxd"), py::str(".386")}))
                    out.push_back(cb::compat::legacy::path_of(p));
            } else if (kind == "dos") {
                std::vector<py::str> programs = formats::dos::programs().vec();
                std::stable_sort(programs.begin(), programs.end(),
                                 [](const py::str& a, const py::str& b) { return py::os::path::getsize(a) < py::os::path::getsize(b); });
                for (const py::str& p : programs) out.push_back(cb::compat::legacy::path_of(p));
            } else {
                for (const py::str& p : base::corpus::modules(std::optional<py::str>(k))) out.push_back(cb::compat::legacy::path_of(p));
            }
            return out;
        };
        services.out = [log](std::string_view line) { log ? log(cb::compat::to_py(line)) : py::print(cb::compat::to_py(line)); };
        services.err = [](std::string_view line) { py::print(cb::compat::to_py(line)); };
    }
};

}  // namespace detail

inline const py::str DOC = py::str("sweep.py -- reconstruct and judge every module of the corpus, smallest first, resumably.\n"
                                   "(see cb/verify/sweep.hpp)\n");

inline std::optional<py::set<py::str>> _wanted(const py::list<py::str>& argv) {
    if (!argv.contains(py::str("--only"))) return std::nullopt;
    int64_t at = 0;
    for (int64_t i = 0; i < static_cast<int64_t>(argv.size()); ++i)
        if (argv[i] == py::str("--only")) {
            at = i;
            break;
        }
    if (at + 1 >= static_cast<int64_t>(argv.size())) throw py::IndexError("list index out of range");
    return cb::compat::guard([&] {
        py::set<py::str> out;
        for (const std::string& n : cb::verify::sweep::wanted_names(argv[at + 1].utf8())) out.add(py::str(n));
        return out;
    });
}

// Report what has been measured of `paths` under the settings in force for each one's kind. -> {path: module hash} of those with results.
inline py::dict<py::str, py::str> status(const std::shared_ptr<engine::native_engine::FunctionStore>& fs, const py::list<py::str>& paths,
                                         const std::function<void(const py::str&)>& log = nullptr) {
    return cb::compat::guard([&] {
        detail::Sweeping s(fs, log);
        std::vector<std::filesystem::path> wanted;
        for (const py::str& p : paths) wanted.push_back(cb::compat::legacy::path_of(p));
        py::dict<py::str, py::str> out;
        for (const auto& [path, sha] : cb::verify::sweep::status(s.services, wanted)) out.set(cb::compat::legacy::native(path), py::str(sha));
        return out;
    });
}

inline int64_t main(const py::list<py::str>& argv) {
    if (build::watcom::ow_tool(py::str("wcc386")).empty())
        throw py::SystemExit("OpenWatcom is not set up (settings.json: openwatcom): nothing could be built");
    auto fs = store::functions::open();
    return cb::compat::guard([&] {
        cb::verify::sweep::Options o;
        if (argv.contains(py::str("--all"))) o.kinds = {"pe", "ne", "le", "dos"};
        else if (argv.contains(py::str("--16"))) o.kinds = {"ne"};
        else if (argv.contains(py::str("--drivers"))) o.kinds = {"le"};
        else if (argv.contains(py::str("--dos"))) o.kinds = {"dos"};
        if (const auto only = _wanted(argv)) {
            o.only = std::set<std::string>();
            for (const py::str& n : *only) o.only->insert(n.utf8());
        }
        o.status_only = argv.contains(py::str("--status"));
        o.again = argv.contains(py::str("--again"));
        detail::Sweeping s(fs, nullptr);
        return static_cast<int64_t>(cb::verify::sweep::run(o, s.services));
    });
}

}  // namespace codebreaker::verify::sweep
