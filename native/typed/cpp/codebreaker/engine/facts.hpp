// codebreaker/engine/facts.hpp -- typed C++ port of codebreaker.engine.facts.
//
// facts.py -- the program database: one SQLite file per binary, every fact about it with where it came from.
//
//     auto db = facts::open_for(path);               // state/db/<sha>.sqlite, the binary's own
//     auto row = db->row("function.c", "1:7cd4");    // a Row, or None
//     db->write("function.c", "1:7cd4", value, "function.c", version, inputs);
//     db->rows("function.gate");                     // every fact of a kind
//     facts::hold();                                 // every database back under the setting db_max_gb
//
// A fact is (kind, key, value): `kind` says what it is ("function.object"), `key` which one within the
// binary (a function's address "1:7cd4", or "" for one of the whole binary). With it is kept how it was
// made: the step that made it, that step's version, and the hash of every fact it was made from. A value is
// kept as
//
//     bytes        as they are                    (tag B)
//     text         UTF-8                          (tag S)
//     JSON         what comes back from JSON the same as it went in (tag J)
//     a pickle     anything else                  (tag P)
//
// and its hash is the SHA-1 of the tag and those bytes. A small value is kept in its row; a large one (past
// BLOB_AT bytes) once in the blob table, by its hash.
//
// The fact store is C++ (native/engine, here linked in: engine/native_engine.hpp): Database is its.
//
// Notes on the port:
//   * A value is `Value`, the three kinds a typed program can hold: bytes, str, or a JSON value (the
//     knowledge::sources::Json tagged union -- the one place the values genuinely mix types). A Python
//     value that only a pickle can keep has no typed form: decode() of tag P raises NotImplementedError,
//     encode() never makes one (a JSON value holding a NaN, which Python's check refuses and pickles,
//     raises the same).
//   * The cycle facts <-> native_engine <-> engine is broken with forward declarations: Row and the
//     functions here name native_engine::Database; its class is in native_engine.hpp.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/symbols/native.hpp"
#include "py.hpp"

namespace codebreaker::engine::native_engine {
class Database;
}

namespace codebreaker::engine::facts {

using Json = knowledge::sources::Json;
// What a fact holds: bytes, text, or JSON.
using Value = std::variant<py::bytes, py::str, Json>;
// ("kind", "key", "hash") -- one fact a fact was made from
using Input = std::tuple<py::str, py::str, py::str>;

constexpr int64_t BLOB_AT = 4096;      // values larger than this are kept once, in the blob table
constexpr int64_t DEFAULT_MAX_GB = 4;
extern const py::str GIVEN;            // the step of a fact a person (or the engine's caller) set

// -> (tag, bytes) of a value as it is kept.
std::tuple<py::str, py::bytes> encode(const Value& value);
Value decode(const py::str& tag, const py::bytes& data);

// json.dumps(value, sort_keys=True, separators=(",", ":")) of a JSON value.
py::str json_dumps(const Json& value);
// A JSON value as symbols::native::ask / loads give it, as a facts value.
Json from_native(const symbols::native::Json& j);
// json.loads(text) as a JSON value.
Json json_loads(const py::str& text);

// One fact as it is kept: its value is read only when asked for.
class Row {
  public:
    Row(native_engine::Database* db, const py::str& kind, const py::str& key, const py::str& hash,
        const py::str& tag, const std::optional<py::bytes>& value, const std::optional<py::str>& blob,
        int64_t size, const py::str& step, const py::str& version, const std::optional<py::str>& inputs,
        double made, double took);

    native_engine::Database* db;
    py::str kind, key, hash, tag;
    int64_t size;
    py::str step, version;
    py::list<Input> inputs;
    double made, took;

    // The value's bytes, as kept.
    py::bytes data() const;
    Value value() const;
    bool given() const;

  private:
    std::optional<py::bytes> value_;
    std::optional<py::str> blob_;
};

// The facts of one binary (see the file's text): the C++ store's.
std::shared_ptr<native_engine::Database> Database(const py::str& path);

// --- where the databases are ---
// state/db, where every binary's database is.
py::str root();

// The binary's SHA-256 (kept by path, size and time, in the C++).
py::str sha(const py::str& path);

// The database in the file `path` (one connection per process).
std::shared_ptr<native_engine::Database> open_at(const py::str& path);

// The database of the binary at `binary` (its SHA-256 names it): state/db/<sha>.sqlite.
std::shared_ptr<native_engine::Database> open_for(const py::str& binary,
                                                  const std::optional<py::str>& folder = std::nullopt);

// A folder for the files a step writes as it works (a build's objects): beside the database, <sha>.work/.
py::str work(native_engine::Database& db, const py::list<py::str>& parts = py::list<py::str>());

int64_t max_bytes();

// [(path, bytes, last opened)] of every database, the one opened longest ago first. (Not the knowledge
// base beside them, knowledge.sqlite.)
py::list<std::tuple<py::str, int64_t, double>> databases(const std::optional<py::str>& folder = std::nullopt);

// All the databases back under `cap` bytes (default the setting db_max_gb): past it, those opened
// longest ago go, whole, with their work folders, until four fifths of the cap is used; never one open
// in this process. -> [path gone or emptied of facts]
py::list<py::str> hold(const std::optional<int64_t>& cap = std::nullopt,
                       const std::optional<py::str>& folder = std::nullopt);

// The process's open databases (path -> Database), and its lock: native_engine's Database::close() takes
// itself out of them.
void forget_open(const py::str& path);

}  // namespace codebreaker::engine::facts
