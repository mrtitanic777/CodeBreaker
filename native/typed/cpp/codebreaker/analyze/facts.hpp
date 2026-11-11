// codebreaker/analyze/facts.hpp -- typed C++ port of codebreaker.analyze.facts.
//
// facts.py -- the fact table: what the analyses found, kept beside the function store.
//
//     auto fs = facts::open();                            // the function store's own sqlite file
//     fs->put(sha, "conventions", rows);                  // replaces that module's facts of that analysis
//     fs->get(sha, "conventions", "bfee83ad");
//     fs->get(sha, std::nullopt, std::nullopt, "structure");
//     fs->tally(sha);                                     // {analysis: {kind: count}}
//
// A fact is one thing an analysis established: about a function (`addr` is that function's key
// in the function store -- the address in hex for a 32-bit module or a driver, "segment:offset"
// for a 16-bit one) or about the module as a whole (`addr` empty). `kind` says what it is
// ("convention", "structure", "global", "class", "switch", ...) and `value` holds the rest as JSON.
//
// The table lives in the same file as the function store (store/functions.py), so a function's
// reconstruction, its verdicts and what is known about it are one query apart; it is created
// here, on first use, and nothing in the store depends on it.
//
// Notes on the port:
//   * A fact's value is `Json` (knowledge::sources::Json, the tagged union that is what json.loads gives):
//     the one place the Python genuinely mixes types. It is stored as json.dumps(value, sort_keys=True,
//     default=_plain) writes it: a tuple (a List marked `tuple`) as a list; a set, or any other object the
//     analyses hold, is turned into JSON by _plain() before it is put (the overloads below), as json.dumps's
//     `default` hook does for the values it cannot write itself.
//   * key(img, va) takes the image's kind: the Python reads only `img.kind`; an overload takes the typed
//     analyze::image::Image.
//   * The sqlite3 module is base::cache::Db (the C++ port's connection over the sqlite3 amalgamation).
//     executescript(_SCHEMA) is its four statements run one after another; `with self.db:` is the
//     transaction put() commits (or rolls back when it fails).
//   * get() returns Fact records (the dicts {analysis, addr, kind, value}); tally() is {analysis: {kind: n}}.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/analyze/image.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "py.hpp"

namespace codebreaker::analyze::facts {

using Json = knowledge::sources::Json;

// The function store's key for an address of an image ('' for None).
py::str key(const py::str& kind, const std::optional<int64_t>& va);
py::str key(const image::Image& img, const std::optional<int64_t>& va);

// (addr key, kind, value): one row of what an analysis has to say about a module
using Row = std::tuple<py::str, py::str, Json>;

// {analysis, addr, kind, value} of one stored fact
struct Fact {
    py::str analysis;
    py::str addr;
    py::str kind;
    Json value;
};

class FactStore {
  public:
    explicit FactStore(const std::optional<py::str>& path = std::nullopt);
    FactStore(const FactStore&) = delete;
    FactStore& operator=(const FactStore&) = delete;

    py::str path;
    std::shared_ptr<base::cache::Db> db;

    void close();

    // rows: [(addr key, kind, value)] -- everything `analysis` has to say about `module`.
    void put(const py::str& module, const py::str& analysis, const py::list<Row>& rows);

    // [{analysis, addr, kind, value}] for a module, narrowed by whatever is given.
    py::list<Fact> get(const py::str& module, const std::optional<py::str>& analysis = std::nullopt,
                       const std::optional<py::str>& addr = std::nullopt, const std::optional<py::str>& kind = std::nullopt);

    py::dict<py::str, py::dict<py::str, int64_t>> tally(const py::str& module);
};

// json.dumps(value, sort_keys=True, default=_plain)
py::str dumps(const Json& value);
// json.loads(text)
Json loads(const py::str& text);

// JSON for what the analyses hold in sets and tuples: a set sorted by repr, as a list.
Json _plain(const py::set<int64_t>& x);
Json _plain(const py::set<py::str>& x);
// ... and str(x) for anything else.
Json _plain(const py::str& str_of_x);

std::shared_ptr<FactStore> open(const std::optional<py::str>& path = std::nullopt);   // as store.functions.open

}  // namespace codebreaker::analyze::facts
