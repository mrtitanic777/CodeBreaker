// knowledge/store.hpp -- the knowledge base's database: one SQLite file, every platform's knowledge keyed by platform.
//
//     Store kb(Store::default_path(config.state()), DataFolders::from_environment());
//     kb.get("win32", "prototype", "GetDlgItem");                          // a Value, or nullopt
//     kb.put("win32", "argument_bytes.code", "USER32.FindWindowA", 8, "user32.dll's code", Origin::Learned);
//     kb.table("win16", "prototype");                                      // {key: value} of a kind, in the order written
//     kb.ensure({"win32", "prototype"}, stamp, make, "the headers", Origin::Derived);   // a kind made again when its stamp changes
//
// A piece of knowledge is (platform, kind, key, value), kept the way the program database keeps a fact (engine/facts.hpp:
// the value is a `Value`, with the SHA-1 of its encoding), and with where it came from:
//
//     source     what it was read or worked out from: a file of the checkout, the headers, a module's code, a person
//     origin     Authored   loaded from a file people write (ordinal_names.json, driver_services.json, ...): the file is
//                           the knowledge, the rows its copy, made again when the file changes
//                Derived    worked out from inputs outside the checkout (the compiler's headers): made again when their
//                           stamp changes
//                Learned    found once by a step and kept (the bytes of arguments an export removes, read from its
//                           code): written once, seen by every later ask
//                Given      given by hand (a body named); never dropped
//
// A whole kind that is authored or derived is made in one go by ensure(): the stamp it was made under (the file's hash;
// the header folders' sizes and times) is kept in `loaded`, and when it differs the kind's authored and derived rows are
// made again -- learned and given rows stay.
//
// Bodies (the name store) are kept here too: `body` -- a function's body digest in a module, with the name the module
// itself gives it -- and `body_name` -- a name given to a body, which holds in every module with that body.
//
// The Store is the knowledge of `KindStore` (what loading an authored kind asks), of `show::Base` (what the `knowledge`
// command lists), and gives `ReadDigests` its answers (`digest`). It is thread-safe: one mutex serialises the connection.
// The file is this module's own (`knowledge.v2.sqlite`; an older base's file is left alone and its contents are made again).
#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "knowledge/reads.hpp"
#include "knowledge/show.hpp"
#include "knowledge/sources.hpp"
#include "knowledge/value.hpp"

namespace cb::knowledge {

// Where a piece came from (see the top of this file).
enum class Origin { Authored, Derived, Learned, Given };

std::string_view origin_name(Origin origin);
std::optional<Origin> parse_origin(std::string_view name);

// A body of a module, as the name store keeps it.
struct BodyRow {
    std::string addr;
    std::string digest;
    std::optional<std::string> name;     // the name the module itself gives it
    std::optional<std::string> source;
};

// A body's name and where it is from ("named by hand", "the same body in lz32.dll").
struct BodyName {
    std::string name;
    std::string source;
    friend bool operator==(const BodyName&, const BodyName&) = default;
};

class Store : public KindStore, public show::Base {
public:
    // Open (creating the file and its folder). `folders`: where the authored files are.
    Store(const std::filesystem::path& file, DataFolders folders);
    ~Store() override;
    Store(const Store&) = delete;
    Store& operator=(const Store&) = delete;

    // `$CB_KNOWLEDGE_DB` when set, else <state>/db/knowledge.v2.sqlite.
    static std::filesystem::path default_path(const std::filesystem::path& state_dir);
    // The knowledge base of the state folder a function store is in (a test's own store in a folder of its own has a
    // knowledge base of its own).
    static std::filesystem::path beside(const std::filesystem::path& function_store_file);

    const std::filesystem::path& file() const { return file_; }
    AuthoredSources& sources() { return sources_; }

    // ---- pieces ---------------------------------------------------------------------------------------------------
    std::optional<Value> get(std::string_view platform, std::string_view kind, std::string_view key);
    bool has(std::string_view platform, std::string_view kind, std::string_view key);
    void put(std::string_view platform, std::string_view kind, std::string_view key, const Value& value,
             std::string_view source = {}, Origin origin = Origin::Learned);
    void put_many(std::string_view platform, std::string_view kind, const Rows& values, std::string_view source = {},
                  Origin origin = Origin::Learned);
    void forget(std::string_view platform, std::string_view kind, std::string_view key);
    // Where a piece came from, or nullopt.
    std::optional<show::Provenance> provenance(std::string_view platform, std::string_view kind, std::string_view key);
    // {key: value} of a kind: authored and derived rows in the order they were made, then the others in the order
    // they came.
    Rows table(std::string_view platform, std::string_view kind);
    // The kind's authored file loaded again first when it changed, then the rows.
    Rows authored_table(const KindId& kind);

    // The stamp a whole kind was made under, or nullopt.
    std::optional<std::string> stamp_of(std::string_view platform, std::string_view kind);
    // A kind made again when `stamp` differs from the one it was made under; the rows as they are now. The rows an
    // earlier make wrote as authored or derived are replaced, learned and given ones stay.
    Rows ensure(const KindId& kind, std::string_view stamp, const std::function<Rows()>& make, std::string_view source,
                Origin origin);
    Rows ensure(const KindId& kind, std::string_view stamp, const std::function<Rows()>& make,
                std::string_view source) override {
        return ensure(kind, stamp, make, source, Origin::Authored);
    }
    void forget_ensured(const KindId& kind) override;

    // What a step reading a piece has as its input: the piece's hash, "absent" when there is none; a whole kind: a hash of
    // every piece's; for the name store ("bodies"): a hash of what it answers.
    std::string digest(const Read& read);
    // A small note kept by name (what was copied from where, last time).
    std::optional<std::string> carried(std::string_view what);
    void set_carried(std::string_view what, std::string_view mark);

    // ---- bodies: the name store ------------------------------------------------------------------------------------
    void add_module(std::string_view sha, std::string_view name, std::string_view platform);
    // The bodies of a module, replacing the ones it had.
    void set_bodies(std::string_view module, std::span<const BodyRow> bodies);
    std::vector<BodyRow> bodies(std::string_view module);
    // Give a body a name: it holds in every module with that body. Never dropped.
    void name_body(std::string_view digest, std::string_view name, std::string_view source = "named by hand");
    // A body's name: the one given to it, else the one every module naming it agrees on.
    std::optional<BodyName> name(std::string_view digest);
    // The names a module's bodies have from other modules or by hand, by address.
    std::vector<std::pair<std::string, BodyName>> names_from_bodies(std::string_view module);
    std::size_t modules_with_body(std::string_view digest);

    // ---- show::Base ------------------------------------------------------------------------------------------------
    std::string path() const override { return file_.string(); }
    std::string platform(std::string_view name) const override;
    void load_authored() override;
    std::vector<show::KindRow> kinds(std::optional<std::string_view> platform) override;
    show::SharedBodies shared_bodies() override;
    std::vector<std::string> first_keys(std::string_view platform, std::string_view kind, std::size_t limit,
                                        std::size_t& total) override;
    std::optional<show::Stored> value(std::string_view platform, std::string_view kind, std::string_view key) override;
    std::optional<show::Provenance> about(std::string_view platform, std::string_view kind, std::string_view key) override;

private:
    struct Db;
    std::string whole_kind_digest_locked(std::string_view platform, std::string_view kind);
    Rows table_locked(std::string_view platform, std::string_view kind);

    std::filesystem::path file_;
    AuthoredSources sources_;
    std::unique_ptr<Db> db_;
    std::recursive_mutex mutex_;
};

// The `knowledge` command's environment over the knowledge store of a state folder: the store is opened when the command runs.
show::Environment environment(const std::filesystem::path& state_dir, DataFolders folders = DataFolders::from_environment());

// The base asked as the engine's `ReadDigests` asks.
inline ReadDigests::DigestOf digests_of(Store& store) {
    return [&store](const Read& read) { return store.digest(read); };
}

}  // namespace cb::knowledge
