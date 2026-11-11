// codebreaker/verify/measure.hpp -- COMPAT SHIM over cb/verify/measure.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/measure.{hpp,cpp} now: the tally and the declared `measure` command, with the corpus
// and the reconstruct-and-judge passed in as services. This header supplies those services from the old ports (the
// corpus, the function store, the lanes) and keeps `main(argv)` -- the arguments after the command name -- for callers
// that still run it through the old command table. No logic of measure's own.
//
// The lanes must be registered before main() is called (engine/lanes_wiring.hpp).
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/verify/measure.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py_os.hpp"

namespace codebreaker::verify::measure {

inline int64_t main(const py::list<py::str>& argv) {
    namespace nv = cb::verify::measure;
    namespace eng = codebreaker::engine::native_engine;
    nv::Services services;
    services.find_module = [](std::string_view name) -> std::optional<std::filesystem::path> {
        const py::str given = cb::compat::to_py(name);
        std::optional<py::str> path = py::os::path::isdir(given) ? std::optional<py::str>(given) : base::corpus::retail(given);   // a folder: a Java program
        if (!path || path->empty()) return std::nullopt;
        return std::filesystem::path(path->utf8());
    };
    services.judge = [](const std::filesystem::path& path, nv::Engine engine) {
        if (engine == nv::Engine::On) py::os::environ_::set(py::str("CB_ENGINE"), py::str("1"));   // (the old path's switch)
        if (engine == nv::Engine::Off) py::os::environ_::set(py::str("CB_ENGINE"), py::str("0"));
        const py::str module = cb::compat::to_py(path.generic_string());
        auto fs = store::functions::open();
        auto [sha, digest] = reconstruct::run(module, fs);
        std::vector<nv::Row> rows;
        for (const auto& r : fs->candidates(sha, digest)) {
            nv::Row row;
            if (std::holds_alternative<py::str>(r.verdict)) row.verdict = cb::verify::parse_verdict(std::get<py::str>(r.verdict).utf8());
            if (std::holds_alternative<py::str>(r.reason)) row.reason = std::get<py::str>(r.reason).utf8();
            rows.push_back(std::move(row));
        }
        return rows;
    };
    std::vector<std::string> args = {"measure"};
    for (const py::str& a : argv) args.push_back(a.utf8());
    return cb::compat::guard([&] { return cb::cli::Cli("codebreaker", {nv::command(services)}).run(args); });
}

}  // namespace codebreaker::verify::measure
