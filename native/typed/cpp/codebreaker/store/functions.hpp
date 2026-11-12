// codebreaker/store/functions.hpp -- SHIM over cb/store/functions.hpp (docs/CPP_DESIGN.md 6.3): where the function store is and how it is opened.
// The old types in (py::str paths), the new location rules called, the old store (engine::native_engine::FunctionStore, which the old callers hold)
// out. Deleted with its last py:: consumer.
//
// The verdict vocabulary's names (ORDER, TITLE, WEAK, IDENTIFIED, SCORED, classify) are the old verify/verdicts shim's, forwarded. `default_path()`
// no longer makes the state folder (the store makes its folders when it is opened for writing).
#pragma once

#include <memory>
#include <optional>

#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/store/functions.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py.hpp"

namespace codebreaker::store::functions {

// How good a result is, best first. A candidate's class is what the module writer prints.
inline const py::list<py::str>& ORDER() { return verify::verdicts::ORDER(); }
inline const py::dict<py::str, py::str>& TITLE() { return verify::verdicts::TITLE(); }
inline const py::dict<py::str, py::str>& WEAK() { return verify::verdicts::WEAK_LIMITS(); }
// A function recognised as a runtime library's is counted on its own, not as a failure of reconstruction -- unless it was also verified.
inline const py::str& IDENTIFIED() { return verify::verdicts::IDENTIFIED; }
inline const py::list<py::str>& SCORED() { return verify::verdicts::SCORED(); }
inline std::pair<py::str, py::list<py::str>> classify(const std::optional<py::str>& word, const std::optional<py::str>& limits = std::nullopt) {
    return verify::verdicts::classify(word, limits);
}

// Where a store keeps its modules' databases: the engine's (state/db) for the store of the state folder; for a store given a path of its own, the
// folder `db` beside it (or <name>.db/ for a store not called functions.sqlite). `own` unset: whether a path was given or CB_FUNCTIONS_DB is set.
inline py::str databases_folder(const std::optional<py::str>& path = std::nullopt, const std::optional<bool>& own = std::nullopt) {
    return cb::compat::guard([&] {
        std::optional<std::filesystem::path> given;
        if (path && !path->empty()) given = cb::compat::legacy::path_of(*path);
        return cb::compat::legacy::native(cb::store::functions::databases_folder(cb::compat::legacy::state_dir(), given, own));
    });
}

inline py::str default_path() {
    return cb::compat::guard([&] { return cb::compat::legacy::native(cb::store::functions::default_path(cb::compat::legacy::state_dir())); });
}

// The URI sqlite3.connect opens `path` read-only by.
inline py::str _ro(const py::str& path) {
    return py::str("file:") + path.replace(py::str("\\"), py::str("/")).replace(py::str("?"), py::str("%3f")).replace(py::str("#"), py::str("%23")) +
           py::str("?mode=ro");
}

// The function store (see cb/store/functions.hpp): the C++ store's.
inline std::shared_ptr<engine::native_engine::FunctionStore> FunctionStore(const std::optional<py::str>& path = std::nullopt, bool readonly = false) {
    return std::make_shared<engine::native_engine::FunctionStore>(path, readonly);
}

// The function store. Read-only: None when there is no store yet.
inline std::shared_ptr<engine::native_engine::FunctionStore> open(const std::optional<py::str>& path = std::nullopt, bool readonly = false) {
    if (readonly && !py::os::path::isfile((path && !path->empty()) ? *path : default_path())) return nullptr;
    return FunctionStore(path, readonly);
}

// The store at a glance, quickly (one file: the index's summaries): per module {sha, name, kind, path, functions, identified, latest: {class:
// count}}, by each function's LATEST verdict under any judge.
inline py::list<engine::native_engine::Overview> overview(const std::optional<py::str>& path = std::nullopt) {
    auto fs = open(path, true);
    if (!fs) return py::list<engine::native_engine::Overview>();
    auto result = fs->overview();
    fs->close();
    return result;
}

// {addr: class} by each function's latest verdict under any judge (see overview).
inline py::dict<py::str, engine::native_engine::Cell> latest(const py::str& module_sha, const std::optional<py::str>& path = std::nullopt) {
    auto fs = open(path, true);
    if (!fs) return py::dict<py::str, engine::native_engine::Cell>();
    auto result = fs->latest(module_sha);
    fs->close();
    return result;
}

}  // namespace codebreaker::store::functions
