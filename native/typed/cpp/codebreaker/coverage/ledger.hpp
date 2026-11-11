// codebreaker/coverage/ledger.hpp -- typed C++ of codebreaker.coverage.ledger (see ledger.cpp).
//
// ledger.py -- the coverage ledger: how much of every module of the corpus is accounted for.
//
//     python -m codebreaker coverage                  by kind of module, then every module the store holds
//     python -m codebreaker coverage --all            list the modules nothing has been done to as well
//     python -m codebreaker coverage --module NAME    one module: its counts, its bytes, why functions fail
//     python -m codebreaker coverage --csv FILE       one row per corpus module, for a spreadsheet
//     python -m codebreaker coverage --json           the whole ledger as JSON
//
//     Ledger led = ledger::measure();            // {profiles, kinds, total, modules}
//     auto fs = ledger::open_store();            // the function store, opened read-only
//
// Two sources and nothing else: the corpus (base/corpus) says which modules exist and how much code each holds; the
// function store (store/functions) says what has been made of each function. A row -- for a module, a kind of module,
// or everything -- counts functions / decompiled / compiled, the store's classes (verified, weak, unverified,
// not-judged, no-compile, no-source: they add up to `functions`) and bytes (code_bytes, bytes_known, bytes_verified,
// bytes_weak).  The store is opened read-only: measuring coverage never changes a result.
//
// Typing notes (the Python's dicts, as structs):
//   * a module row is `Row`; a kind row, and the total, is `Summary`; both hold `Counts` (the numbers COUNTS names:
//     the store's classes are the dict `classes`, keyed as verdicts::ORDER, in that order).  `to_value(Ledger)` is
//     the whole dict with the keys in the order the Python's dicts had them (the --json output).
//   * a store is `engine::native_engine::FunctionStore` (None: a null pointer), as in store::functions.
//   * what reconstruct.digests() answers -- the verifier profile per kind of module -- comes from the lanes registered
//     in verify::reconstruct (register_kind), as in the Python the lanes' own modules.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/formats/inspect.hpp"
#include "py.hpp"

namespace codebreaker::coverage::ledger {

using Store = std::shared_ptr<engine::native_engine::FunctionStore>;

// KINDS: (kind, title)
const py::list<std::tuple<py::str, py::str>>& KINDS();
const py::dict<py::str, py::str>& TITLE();
// COUNTS: ("functions", "decompiled", "compiled") + ORDER + ("code_bytes", "bytes_known", "bytes_verified", "bytes_weak")
const py::list<py::str>& COUNTS();

// The numbers of a row (COUNTS and "judged").
struct Counts {
    int64_t functions = 0, decompiled = 0, compiled = 0;
    py::dict<py::str, int64_t> classes;           // {class: functions} for every class of verdicts::ORDER
    int64_t code_bytes = 0, bytes_known = 0, bytes_verified = 0, bytes_weak = 0;
    int64_t judged = 0;
    Counts();
    Counts(const Counts& o);                       // (a dict is shared on copy: the copy has a dict of its own)
    Counts& operator=(const Counts& o);
    // r[key] for a key of COUNTS or "judged"
    int64_t at(const py::str& key) const;
};

// One module of the corpus: its code size comes from the inspector and is remembered in state/coverage_corpus.json
struct CorpusModule {
    py::str name, path, kind;
    int64_t size = 0, code_bytes = 0;
};

// A module row of the ledger.
struct Row : Counts {
    py::str name, path, kind;
    int64_t size = 0;
    bool in_store = false, measured = false;
    std::optional<py::str> sha;                   // set when the store holds the module
};

// A kind row (or the total): the rows' sums, and how many modules / how many in the store / how many measured.
struct Summary : Counts {
    py::str kind, name;
    int64_t modules = 0, in_store = 0, measured = 0;
};

struct Ledger {
    py::dict<py::str, std::optional<py::str>> profiles;   // {kind: digest}
    py::list<Summary> kinds;
    Summary total;
    py::list<Row> modules;
    py::str generated, corpus;
};

Store open_store(const std::optional<py::str>& path = std::nullopt);

// {kind: digest} -- the verifier settings in force now, per kind of module (the settings most recently used in the
// store stand in for one that cannot be worked out on this machine).
py::dict<py::str, std::optional<py::str>> profiles(const Store& fs = nullptr);

py::list<CorpusModule> corpus_modules(bool refresh = false);

// {address: bytes} -- how far each known function reaches (see ledger.py)
py::dict<py::str, int64_t> extents(const py::str& path, const py::str& kind, const py::list<py::str>& addrs);

// ({address: class}, how many have a result under these settings)
std::tuple<py::dict<py::str, py::str>, int64_t> _classes(const Store& fs, const py::str& sha, const std::optional<py::str>& digest);

// one module's counts; `stamp` (what changes when its results change) lets an unchanged module be served from memory
Counts _counts(const Store& fs, const py::str& sha, const std::optional<py::str>& digest, const py::str& path, const py::str& kind,
               const std::optional<std::tuple<int64_t, int64_t, engine::native_engine::Cell>>& stamp = std::nullopt);

// The ledger. `fs` null: the store is opened here (and closed).  `module` limits it to one name.
Ledger measure(const Store& fs = nullptr, const std::optional<py::str>& module = std::nullopt, bool refresh = false);

Summary _total(const py::list<Row>& rows);

// {class: [(reason, functions)]} -- why the functions of a module that are not verified are not, commonest first.
py::dict<py::str, py::list<std::tuple<py::str, int64_t>>> reasons(const Store& fs, const py::str& sha, const py::str& digest,
                                                                  int64_t most = 8);

// ---- as text
py::str _pct(int64_t a, int64_t b);
py::list<py::str> _table(const py::list<py::str>& head, const py::list<py::list<py::str>>& rows);
py::list<py::str> render(const Ledger& led, bool every = false);
py::str _state(const Row& r);
int64_t write_csv(const Ledger& led, const py::str& path);

// The whole ledger as the Python's dict (the --json output): keys in the Python's order.
formats::inspect::Value to_value(const Ledger& led);

extern const char* DOC;     // the module's text, printed by `coverage --help`

// python -m codebreaker coverage [--module NAME] [--all] [--csv FILE] [--json] [--refresh]
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::coverage::ledger
