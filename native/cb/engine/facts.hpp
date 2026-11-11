// cb/engine/facts.hpp -- the program database: one SQLite file per binary, every fact about it with where it came from.
//
//     Databases dbs({.root = state / "db", .max_bytes = 4 * kGiB});
//     auto db = dbs.open_for(binary_path);                 // <root>/<sha256>.sqlite, the binary's own
//     std::optional<Row> row = db->row("function.c", "1:7cd4");
//     db->write("function.c", "1:7cd4", Value{text}, "function.c", version, {{kind, key, hash}});
//     db->rows("function.gate");                           // every fact of a kind
//     dbs.hold();                                          // every database back under the cap
//
// A fact is (kind, key, value): `kind` says what it is ("function.object"), `key` which one within the binary (a
// function's address "1:7cd4", or "" for one of the whole binary). With it is kept how it was made: the step that made
// it, that step's version (a digest of its code), and the hash of every fact it was made from. A value is kept as
//
//     bytes        as they are                        (tag B)
//     text         UTF-8                              (tag S)
//     a document   compact JSON, keys sorted         (tag J)
//
// and its hash is the SHA-1 of the tag and those bytes: two facts are the same exactly when their hashes are. A small
// value is kept in its row; a large one (past kBlobAt bytes) once in the blob table, by its hash, however many facts
// hold it. (The tag P of the databases the Python wrote -- a pickle -- is Python's: such a fact reads as an
// UnsupportedError and is made again.)
//
// The file layout is the one native/engine (cbengine.dll, which the Python still calls) writes, so a database is read
// by either.
//
// The size of it all is looked after the way base/cache looks after the cache's: the facts a step made under another
// version are dropped (drop_other_versions, once per step per process), and all the databases together are held under
// a cap -- the ones opened longest ago go first, whole, never one open in this process; a database that holds the
// function store's tables is not deleted, its facts go. Facts a person gives are made by the step "given" and are never
// dropped by either rule.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/knowledge/value.hpp"

namespace cb::engine::facts {

inline constexpr std::size_t kBlobAt = 4096;                  // values larger than this are kept once, in the blob table
inline constexpr std::uint64_t kGiB = std::uint64_t{1} << 30;
inline constexpr std::uint64_t kDefaultMaxBytes = 4 * kGiB;   // the setting `db_max_gb`'s default
inline constexpr std::string_view kGiven = "given";           // the step of a fact a person (or the engine's caller) set

// What a fact holds: bytes, text, or a document.
using Value = std::variant<Bytes, std::string, knowledge::Value>;

struct Encoded {
    char tag = 'B';   // B bytes, S text, J JSON
    Bytes data;
};
// How a value is kept. A document holding a number JSON cannot say (NaN, infinity) throws UnsupportedError.
Encoded encode(const Value& value);
// Throws FormatError for malformed JSON or text, UnsupportedError for the tag P (a Python pickle) or an unknown tag.
Value decode(char tag, ByteView data);

// One fact a fact was made from.
struct Input {
    std::string kind, key, hash;
    friend bool operator==(const Input&, const Input&) = default;
};

class Database;

// One fact as it is kept: its value is read only when asked for.
class Row {
public:
    std::string kind, key, hash;
    char tag = 'B';
    std::uint64_t size = 0;
    std::string step, version;
    std::vector<Input> inputs;
    double made = 0, took = 0;   // when (seconds since 1970) and how long (seconds)

    bool given() const { return step == kGiven; }
    Bytes data() const;          // the value's bytes, as kept
    Value value() const;

private:
    friend class Database;
    const Database* db_ = nullptr;
    std::optional<Bytes> inline_;      // the value when it is small
    std::optional<std::string> blob_;  // else the hash it is kept under
};

struct KindSummary {
    std::string kind;
    std::uint64_t count = 0, bytes = 0;
    std::string steps;   // the steps that made them, comma separated
};

// The facts of one binary. Thread-safe: one mutex serialises the statements.
class Database {
public:
    // Open (creating the file and its folder), and note the time: the cap goes by when a database was last opened.
    static std::shared_ptr<Database> open(const std::filesystem::path& file);
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    const std::filesystem::path& path() const { return path_; }
    std::string sha;   // the binary's SHA-256 (set by Databases::open_for)

    std::optional<Row> row(std::string_view kind, std::string_view key = "") const;
    std::vector<Row> rows(std::optional<std::string_view> kind = std::nullopt) const;   // by key (within a kind: by kind, then key)
    std::vector<KindSummary> kinds() const;
    Bytes blob(std::string_view hash) const;   // throws NotFoundError... FormatError when the database lacks it

    std::optional<std::string> about(std::string_view name) const;
    void set_about(std::string_view name, std::string_view value);

    // Keep a fact; -> its hash.
    std::string write(std::string_view kind, std::string_view key, const Value& value, std::string_view step,
                      std::string_view version, const std::vector<Input>& inputs, double took = 0);
    // Keep a row as it is given (a fact whose hash is not its value's: the binary's own bytes by the digest the caller knows).
    void write_raw(std::string_view kind, std::string_view key, std::string_view hash, char tag, ByteView data,
                   std::string_view step, std::string_view version, std::string_view inputs_json, double took = 0);
    // A fact that is still right under a newer version of its step, or other inputs.
    void remake(std::string_view kind, std::string_view key, std::string_view version, const std::vector<Input>& inputs);
    // Drop one fact (key given) or every fact of a kind; -> how many.
    std::uint64_t forget(std::string_view kind, std::optional<std::string_view> key = std::nullopt);
    // The facts a step made under a version other than `version` can never be asked for again: gone (once per step per
    // process; never the step "given"). -> how many, and the blobs nothing holds go with them.
    std::uint64_t drop_other_versions(std::string_view step, std::string_view version);
    void collect();   // the blobs no fact holds, and the room they took
    bool cleaned(std::string_view step) const;
    void mark_cleaned(std::string_view step);

    struct Impl;

private:
    explicit Database(std::filesystem::path path);
    std::filesystem::path path_;
    std::unique_ptr<Impl> impl_;
    mutable std::mutex cleaned_mutex_;
    std::set<std::string, std::less<>> cleaned_;
};

struct Config {
    std::filesystem::path root;                     // where every binary's database is (state/db)
    std::uint64_t max_bytes = kDefaultMaxBytes;     // the cap on all of them together
};

struct DatabaseFile {
    std::filesystem::path path;
    std::uint64_t bytes = 0;   // with its -wal and -shm
    double opened = 0;         // last opened (seconds since 1970)
};

// The databases of a process: the one open per file, and the cap over all of them.
class Databases {
public:
    explicit Databases(Config config) : config_(std::move(config)) {}

    const Config& config() const { return config_; }
    // The database in `file` (one connection per process).
    std::shared_ptr<Database> open_at(const std::filesystem::path& file);
    // The database of the binary at `binary`: <root>/<sha256>.sqlite (or in `folder`). Notes the binary's name and path.
    std::shared_ptr<Database> open_for(const std::filesystem::path& binary, const std::optional<std::filesystem::path>& folder = std::nullopt);
    // The binary's SHA-256, lower-case hex (kept by path, size and time).
    std::string sha_of(const std::filesystem::path& binary);
    // A folder for the files a step writes as it works (a build's objects): beside the database, <sha>.work/.
    static std::filesystem::path work_folder(const Database& db, const std::filesystem::path& sub = {});
    // Every database of the folder, the one opened longest ago first. (Not knowledge.sqlite beside them: it holds what
    // was learned and given by hand, which nothing would make again.)
    std::vector<DatabaseFile> databases(const std::optional<std::filesystem::path>& folder = std::nullopt) const;
    // All the databases back under `cap` bytes (default the config's): past it, those opened longest ago go, whole,
    // with their work folders, until four fifths of the cap is used; never one open here (and of one holding the function
    // store's verdicts only its facts). -> the paths gone or emptied of facts.
    std::vector<std::filesystem::path> hold(std::optional<std::uint64_t> cap = std::nullopt, const std::optional<std::filesystem::path>& folder = std::nullopt);
    // Forget a closed database (so `hold` may remove it).
    void close(const std::filesystem::path& file);

private:
    Config config_;
    std::mutex mutex_;
    std::map<std::string, std::shared_ptr<Database>> open_;   // by absolute path: lookup (and `hold` asks which are open)
    std::map<std::tuple<std::string, std::uint64_t, std::int64_t>, std::string> shas_;   // (path, size, mtime) -> digest: lookup
};

}  // namespace cb::engine::facts
