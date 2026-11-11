// codebreaker/source/placement.hpp -- SHIM over cb/source/placement.hpp (docs/CPP_DESIGN.md 6.3): where a module's source goes in the
// source tree.
//
//     placement::resolve("comctl32.dll", path)      ("shell/comctl32", "known")
//     placement::folder("comctl32.dll", path)       the folder itself (created on demand)
//
// The shim holds the one Placement per (tree, product) the old callers share -- the old module cached the tables and the folders it
// found for the process -- and wires it to the old corpus shim (which product, which tree, what lane a file is). The tables are read
// from codebreaker/source/placement.json under the checkout root, as before. Deleted with its last py:: consumer.
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/placement.hpp"
#include "codebreaker/base/corpus.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::placement {

namespace shim_ {

inline const cb::source::placement::Placement& current() {
    static std::mutex lock;
    static std::map<std::string, std::unique_ptr<cb::source::placement::Placement>> held;   // by tree and product; lookup only
    const std::lock_guard<std::mutex> guard(lock);
    auto& corpus = base::corpus::detail::instance();
    const std::filesystem::path tree = corpus.design_dir().parent_path();
    const std::string product = corpus.current();
    auto& slot = held[cb::fs::to_text(tree) + "|" + product];
    if (!slot) {
        const std::filesystem::path file = cb::fs::from_text(cb::compat::from_py(py::settings::checkout_root())) / "codebreaker" / "source" / "placement.json";
        slot = std::make_unique<cb::source::placement::Placement>(tree, cb::source::placement::load_table(file, product));
    }
    return *slot;
}

}  // namespace shim_

// The source tree of the selected product.
inline py::str tree_root() { return cb::compat::to_py(cb::fs::to_text(shim_::current().tree_root())); }

// {module stem: folder relative to the tree} for every module folder already there.
inline py::dict<py::str, py::str> existing(const std::optional<py::str>& = std::nullopt) {
    py::dict<py::str, py::str> out;
    for (const auto& [stem, folder] : shim_::current().existing()) out.set(cb::compat::to_py(stem), cb::compat::to_py(folder));
    return out;
}

// -> (folder relative to the tree, how it was decided).
inline std::tuple<py::str, py::str> resolve(const py::str& module, const py::str& path = py::str("")) {
    return cb::compat::guard([&] {
        const std::string file = cb::compat::from_py(path);
        std::optional<cb::base::Lane> lane;
        if (!file.empty()) lane = base::corpus::detail::instance().lane(cb::fs::from_text(file));
        const auto d = shim_::current().resolve(cb::compat::from_py(module), cb::fs::from_text(file), lane);
        return std::make_tuple(cb::compat::to_py(d.folder), cb::compat::to_py(cb::source::placement::how_name(d.how)));
    });
}

// The folder a module's source goes in.
inline py::str folder(const py::str& module, const py::str& path = py::str(""), bool create = true) {
    return cb::compat::guard([&] {
        const std::string file = cb::compat::from_py(path);
        std::optional<cb::base::Lane> lane;
        if (!file.empty()) lane = base::corpus::detail::instance().lane(cb::fs::from_text(file));
        return cb::compat::to_py(cb::fs::to_text(shim_::current().folder(cb::compat::from_py(module), cb::fs::from_text(file), lane, create)));
    });
}

// python -m codebreaker place <module> ... | --all
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&]() -> int64_t {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        return cb::source::placement::place_command(shim_::current(), base::corpus::detail::instance(), args);
    });
}

}  // namespace codebreaker::source::placement
