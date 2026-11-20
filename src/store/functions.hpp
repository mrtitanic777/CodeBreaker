// store/functions.hpp -- where the function store is, and opening it.
//
// The function store (engine/function_store.hpp) is the one place that knows every function and what was made of it: the module writer, the
// scoreboard and the tools that explain a failure read from it. Its tables live in two kinds of file: a module's own tables are in that binary's
// program database (<folder>/<sha>.sqlite), and the store's own file is the index of them (which modules there are, the verifier profiles, a
// summary per module). This module says where those files are, and gives the questions asked across the corpus.
//
//     auto store = functions::open(state);                      // state/functions.sqlite, state/db/      (nullptr: read-only, none yet)
//     auto glance = functions::overview(state);                 // per module, what its functions came to
//     auto classes = functions::latest(state, sha);             // {address: class} by each function's latest verdict
//
// Where. For the store of the state folder the index is <state>/functions.sqlite and the databases are <state>/db. A store given a path of its own
// (or CB_FUNCTIONS_DB, which names one) keeps its databases in the folder `db` beside it -- a store not called functions.sqlite in <its name>.db/ --
// as the knowledge base beside it is found.
//
// The verdict vocabulary the store's readers use (the classes, their titles, the limits that make a match weak) is verify/verdicts: it is not
// named again here.
//
// To carry a build's results to the next one (`codebreaker versions`) the store is read and written through VersionsLedger
// (store/versions_ledger.hpp), which implements compare::versions::Ledger.
#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "engine/function_store.hpp"
#include "verify/verdicts.hpp"

namespace cb::store::functions {

// The store's index file: CB_FUNCTIONS_DB when it is set and not empty, else functions.sqlite in the state folder.
std::filesystem::path default_path(const std::filesystem::path& state);

// Where a store keeps its modules' databases: the program databases' own folder (<state>/db) for the store of the state folder; for a store given a
// path of its own, the folder `db` beside it (or <name>.db/ beside a store not called functions.sqlite). `own` unset: whether a path was given or
// CB_FUNCTIONS_DB is set.
std::filesystem::path databases_folder(const std::filesystem::path& state, const std::optional<std::filesystem::path>& path = std::nullopt,
                                       std::optional<bool> own = std::nullopt);

// The function store at `path` (the state folder's when none is given). Opened for writing, its files and folders are made; read-only, nullptr when
// there is no store yet.
std::unique_ptr<engine::FunctionStore> open(const std::filesystem::path& state, const std::optional<std::filesystem::path>& path = std::nullopt,
                                            bool readonly = false);

// The store at a glance, quickly (the index's summaries only): per module its sha, name, kind and path, how many functions, how many are a
// runtime library's, and how many are of each class by each function's LATEST verdict under any judge -- what it last came to, not what the judge in
// force says (a change to the judge leaves every module unjudged until it is measured again). Empty when there is no store.
std::vector<engine::Overview> overview(const std::filesystem::path& state, const std::optional<std::filesystem::path>& path = std::nullopt);

// {address: class} by each function's latest verdict under any judge (see overview). Empty when there is no store.
std::map<std::string, verify::Class> latest(const std::filesystem::path& state, std::string_view module_sha,
                                            const std::optional<std::filesystem::path>& path = std::nullopt);

}  // namespace cb::store::functions
