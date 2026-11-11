// codebreaker/store/functions.hpp -- typed C++ port of codebreaker.store.functions.
//
// functions.py -- the function store: one place that knows every function and what was made of it.
//
// Everything CodeBreaker does to a function ends up here, and everything that reports reads from here: the
// module writer, the scoreboard, the tools that explain a failure.
//
//     module       one original binary (by content hash)
//     function     one function of a module (by address)
//     candidate    one decompiler's reconstruction of a function: its source, or why it has none
//     verdict      what the verifier found for a candidate, under one set of verifier settings
//     text         something a decompiler's functions share, such as its header of types
//     identified   a function recognised as a runtime library's (its name there, and the library)
//
// Where it is kept. A module's functions, candidates, verdicts, texts, identified functions, built
// signatures and byte-match variants are tables of that binary's program database (state/db/<sha>.sqlite,
// engine/facts.hpp: the same file the engine keeps the binary's facts in). The store's own file
// (state/functions.sqlite, CB_FUNCTIONS_DB) is the index of them: which modules there are (sha, name, path,
// kind), the verifier profiles, and a summary per module kept as each module is written. A store given a
// path of its own keeps its databases in the folder `db` beside it (a store not named functions.sqlite: in
// <its name>.db/), as the knowledge base beside it is found.
//
// A verdict is kept per verifier profile (a digest of every setting a verdict depends on), so changing the
// verifier adds new verdicts beside the old ones instead of erasing them.
//
//     auto fs = functions::open();                   // state/functions.sqlite and state/db/
//     fs->add_module(sha, "comctl32.dll", path, "pe");
//     fs->record(sha, "bfee83ad", "core", rev, sig, "sub_0183AD", source, profile, "equivalent", "coverage=100");
//     fs->best(sha, profile);                        // {address: the best candidate's row}
//     fs->tally(profile, sha);                       // {class: count}
//     fs->module_rows();                             // [(sha, name, path, kind)] (the index)
//
// The store is C++ (native/engine: engine/native_engine.hpp's FunctionStore).
//
// Notes on the port:
//   * ORDER, TITLE, WEAK, IDENTIFIED, SCORED and classify are the verdict vocabulary's
//     (verify/verdicts.hpp), named here too, as the store's readers know them.
//   * _ro(path) opens a sqlite3 connection to a file read-only: the sqlite3 module is Python's, so it is a
//     small function that gives the URI Python opens (the connection itself is not ported).
#pragma once

#include <memory>
#include <optional>

#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py.hpp"

namespace codebreaker::store::functions {

// How good a result is, best first. A candidate's class is what the module writer prints.
inline const py::list<py::str>& ORDER() { return verify::verdicts::ORDER(); }
inline const py::dict<py::str, py::str>& TITLE() { return verify::verdicts::TITLE(); }
inline const py::dict<py::str, py::str>& WEAK() { return verify::verdicts::WEAK_LIMITS(); }
// A function recognised as a runtime library's is counted on its own, not as a failure of reconstruction --
// unless it was also verified, which is the stronger statement and stays.
inline const py::str& IDENTIFIED() { return verify::verdicts::IDENTIFIED; }
inline const py::list<py::str>& SCORED() { return verify::verdicts::SCORED(); }
inline std::pair<py::str, py::list<py::str>> classify(const std::optional<py::str>& word,
                                                      const std::optional<py::str>& limits = std::nullopt) {
    return verify::verdicts::classify(word, limits);
}

// The store at a glance, quickly (one file: the index's summaries): per module {sha, name, kind, path,
// functions, identified, latest: {class: count}}, by each function's LATEST verdict under any judge.
py::list<engine::native_engine::Overview> overview(const std::optional<py::str>& path = std::nullopt);

// {addr: class} by each function's latest verdict under any judge (see overview).
py::dict<py::str, engine::native_engine::Cell> latest(const py::str& module_sha,
                                                      const std::optional<py::str>& path = std::nullopt);

py::str default_path();

// Where a store keeps its modules' databases: the engine's (state/db) for the store of the state folder; for
// a store given a path of its own, the folder `db` beside it (or <name>.db/ for a store not called
// functions.sqlite). `own` unset: whether a path was given or CB_FUNCTIONS_DB is set.
py::str databases_folder(const std::optional<py::str>& path = std::nullopt,
                         const std::optional<bool>& own = std::nullopt);

// The URI sqlite3.connect opens `path` read-only by.
py::str _ro(const py::str& path);

// The function store (see the file's text): the C++ store's.
std::shared_ptr<engine::native_engine::FunctionStore> FunctionStore(const std::optional<py::str>& path = std::nullopt,
                                                                    bool readonly = false);

// The function store. Read-only: None when there is no store yet.
std::shared_ptr<engine::native_engine::FunctionStore> open(const std::optional<py::str>& path = std::nullopt,
                                                           bool readonly = false);

}  // namespace codebreaker::store::functions
