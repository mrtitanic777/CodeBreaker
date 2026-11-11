#include "cb/base/cache.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <cstring>
#include <format>
#include <tuple>

#include <intrin.h>
#include <nmmintrin.h>
#include <process.h>

#include "cb/base/sqlite.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::base::cache {

namespace stdfs = std::filesystem;
using sqlite::Database;
using sqlite::Row;
using sqlite::Value;

namespace {

constexpr std::uint32_t kEnvelopeMagic = 0x31564243;               // "CBV1"
constexpr std::size_t kEnvelopeBytes = 16;                         // magic, schema, length, crc32
constexpr std::uint64_t kCheckEvery = std::uint64_t{32} << 20;     // bytes a process writes between looks at the cap
constexpr std::int64_t kTouchSeconds = 3600;                       // a result's time of use is renewed at most this often
constexpr const char* kDropped = "_dropped";                       // where what is going waits for its deletion
constexpr const char* kDbExtension = ".cbc";
constexpr std::size_t kOldestPerFile = 4000;
constexpr int kHoldRounds = 16;

// The kinds the assemblers and compilers fill: pure functions of inputs their keys name by content, kept in the
// build cache every checkout shares.
constexpr std::string_view kSharedKinds[] = {"wasm16", "objects", "never_set", "masm", "assembled16", "omf_publics"};

std::int64_t now_seconds() {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::int64_t mtime_seconds(const stdfs::path& p, std::error_code& ec) {
    const auto t = stdfs::last_write_time(p, ec);
    if (ec) return 0;
    return std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::clock_cast<std::chrono::system_clock>(t).time_since_epoch())
        .count();
}

std::string lower_name(const stdfs::path& p) { return text::lower(fs::to_text(p.filename())); }

bool is_sidecar(const std::string& name) {
    return name.ends_with("-wal") || name.ends_with("-shm") || name.ends_with("-journal");
}

// "g123abc.cbc" -> "g123abc": a generation's name is what comes before the first dot.
std::string generation_stem(const std::string& name) { return name.substr(0, name.find('.')); }

std::string format_size(std::uint64_t n) {
    if (n >= kGiB) return std::format("{:.2f} GB", static_cast<double>(n) / static_cast<double>(kGiB));
    return std::format("{:.1f} MB", static_cast<double>(n) / static_cast<double>(1 << 20));
}

// Size of a file (with its write-ahead log and shared-memory sidecars) or of a whole tree; unreadable parts count 0.
std::uint64_t size_of(const stdfs::path& p) {
    std::error_code ec;
    if (stdfs::is_directory(p, ec)) {
        std::uint64_t total = 0;
        for (stdfs::recursive_directory_iterator it(p, stdfs::directory_options::skip_permission_denied, ec), end;
             !ec && it != end; it.increment(ec)) {
            std::error_code e2;
            if (it->is_regular_file(e2)) {
                const std::uintmax_t n = it->file_size(e2);
                if (!e2) total += n;
            }
        }
        return total;
    }
    std::uint64_t total = 0;
    for (const char* suffix : {"", "-wal", "-shm"}) {
        stdfs::path q = p;
        q += suffix;
        const std::uintmax_t n = stdfs::file_size(q, ec);
        if (!ec) total += n;
        ec.clear();
    }
    return total;
}

void remove_quietly(const stdfs::path& p) {
    try {
        fs::remove_tree(p);
    } catch (const IoError&) {
        // in use by another process: it goes the next time
    }
}

// Every generation of a kind's folder but `generation`: moved aside at once (another process opening the kind finds
// them gone), then deleted.
void drop_other_generations(const stdfs::path& state_dir, const stdfs::path& kind_dir, const std::string& generation) {
    static std::atomic<std::uint64_t> counter{0};
    std::error_code ec;
    std::vector<stdfs::path> entries;
    for (const auto& e : stdfs::directory_iterator(kind_dir, ec)) entries.push_back(e.path());
    for (const stdfs::path& entry : entries) {
        if (generation_stem(fs::to_text(entry.filename())) == generation) continue;
        const stdfs::path aside =
            state_dir / kDropped / std::format("{}-{}-{}", _getpid(), now_seconds(), counter.fetch_add(1));
        std::error_code e2;
        stdfs::create_directories(aside.parent_path(), e2);
        stdfs::rename(entry, aside, e2);
        if (e2) continue;   // in use by a process of other code: next time
        remove_quietly(aside);
    }
}

}  // namespace

// CRC-32C (Castagnoli) of the payload: the checksum of the envelope. The CPU's crc32 instruction does eight bytes a step
// where it exists (every x64 machine of the last decade); the table-driven fallback gives the same value.
std::uint32_t entry_checksum_portable(ByteView data) {
    static const std::array<std::uint32_t, 256> table = [] {
        std::array<std::uint32_t, 256> t{};
        for (std::uint32_t i = 0; i < 256; ++i) {
            std::uint32_t c = i;
            for (int k = 0; k < 8; ++k) c = (c & 1) != 0 ? (c >> 1) ^ 0x82F63B78u : c >> 1;
            t[i] = c;
        }
        return t;
    }();
    std::uint32_t crc = 0xFFFFFFFFu;
    for (std::uint8_t b : data) crc = table[(crc ^ b) & 0xFF] ^ (crc >> 8);
    return ~crc;
}

std::uint32_t entry_checksum(ByteView data) {
    static const bool hardware = [] {
        int info[4];
        __cpuid(info, 1);
        return (info[2] & (1 << 20)) != 0;   // SSE4.2
    }();
    if (!hardware) return entry_checksum_portable(data);
    std::uint64_t crc = 0xFFFFFFFFu;
    std::size_t i = 0;
    for (; i + 8 <= data.size(); i += 8) {
        std::uint64_t word;
        std::memcpy(&word, data.data() + i, 8);
        crc = _mm_crc32_u64(crc, word);
    }
    for (; i < data.size(); ++i) crc = _mm_crc32_u8(static_cast<std::uint32_t>(crc), data[i]);
    return ~static_cast<std::uint32_t>(crc);
}

namespace {

// A cache file's contents as the envelope the writer made of them: nullopt if it is not ours, is of another schema,
// or is damaged.
std::optional<Bytes> open_envelope(Bytes&& stored, std::uint32_t schema) {
    if (stored.size() < kEnvelopeBytes) return std::nullopt;
    {
        io::ByteReader r(stored, "cache entry");
        if (r.u32() != kEnvelopeMagic) return std::nullopt;
        if (r.u32() != schema) return std::nullopt;
        const std::uint32_t length = r.u32();
        const std::uint32_t crc = r.u32();
        if (length != r.remaining()) return std::nullopt;
        if (entry_checksum(r.bytes(length)) != crc) return std::nullopt;
    }
    stored.erase(stored.begin(), stored.begin() + kEnvelopeBytes);   // the payload is what is left (no second copy)
    return std::move(stored);
}

Bytes seal_envelope(ByteView payload, std::uint32_t schema) {
    io::ByteWriter w;
    w.u32(kEnvelopeMagic);
    w.u32(schema);
    w.u32(static_cast<std::uint32_t>(payload.size()));
    w.u32(entry_checksum(payload));
    w.bytes(payload);
    return w.take();
}

std::int64_t as_int(const Value& v) {
    const auto* p = std::get_if<std::int64_t>(&v);
    return p != nullptr ? *p : 0;
}

}  // namespace

// ---- keys ----------------------------------------------------------------------------------------------------

KeyPart::KeyPart(double v) : bytes_(text::format_double(v)) {}

KeyPart::KeyPart(const std::vector<std::string>& list) : bytes_(text::join(list, "\x1f")) {}

std::string key(std::span<const KeyPart> parts) {
    std::string buffer;
    for (const KeyPart& p : parts) {
        buffer += std::to_string(p.bytes().size());
        buffer += ':';
        buffer += p.bytes();
    }
    return crypto::to_hex(crypto::sha1(as_bytes(buffer)));
}

std::string key(std::initializer_list<KeyPart> parts) { return key(std::span<const KeyPart>(parts.begin(), parts.size())); }

std::string tool_id(const stdfs::path& tool) {
    const std::string name = lower_name(tool);
    std::error_code ec;
    if (!stdfs::exists(tool, ec)) return name + ":missing";
    const std::uintmax_t size = stdfs::is_regular_file(tool, ec) ? stdfs::file_size(tool, ec) : 0;
    const std::int64_t mtime = mtime_seconds(tool, ec);
    if (ec) return name + ":missing";
    return std::format("{}:{}:{}", name, size, mtime);
}

std::string folder_id(const stdfs::path& folder) {
    std::vector<std::string> parts;
    std::error_code ec;
    std::vector<stdfs::path> files;
    for (stdfs::recursive_directory_iterator it(folder, ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code e2;
        if (it->is_regular_file(e2)) files.push_back(it->path());
    }
    std::sort(files.begin(), files.end());
    for (const stdfs::path& f : files) {
        std::error_code e2;
        const std::uintmax_t size = stdfs::file_size(f, e2);
        const std::int64_t mtime = mtime_seconds(f, e2);
        if (e2) continue;   // vanished while we looked
        parts.push_back(std::format("{}:{}:{}", fs::to_text(f.filename()), size, mtime));
    }
    return key({KeyPart(parts)});
}

bool Config::disabled_by_environment() {
    const std::optional<std::string> v = fs::get_env("CB_NO_CACHE");
    return v.has_value() && !v->empty();
}

// ---- the cache -----------------------------------------------------------------------------------------------

struct Cache::Kind {
    KindSpec spec;
    bool opened = false;
    std::string generation;
    std::optional<Database> db;       // empty when the kind's file cannot be used
    stdfs::path folder;               // the generation folder of path() results
    bool folder_ready = false;
    HitCount hits;
};

struct Cache::Impl {
    std::mutex mutex;
    std::map<std::string, Kind, std::less<>> kinds;
    std::uint64_t written = 0;        // bytes written since the cap was looked at
    bool looked = false;              // has the cap been looked at in this process yet?
    // digests of files by (path, size, modification time): the bytes of a tool are read once
    std::map<std::tuple<std::string, std::uint64_t, std::int64_t>, std::string> digests;
};

Cache::Cache(Config config) : config_(std::move(config)), impl_(std::make_unique<Impl>()) {
    for (std::string_view kind : kSharedKinds) impl_->kinds[std::string(kind)].spec.scope = Scope::Shared;
}

Cache::~Cache() { close(); }

void Cache::declare_kind(std::string_view kind, KindSpec spec) {
    std::lock_guard lock(impl_->mutex);
    auto it = impl_->kinds.find(kind);
    if (it == impl_->kinds.end()) it = impl_->kinds.emplace(std::string(kind), Kind{}).first;
    if (it->second.opened) return;   // already in use in this process
    it->second.spec = std::move(spec);
}

namespace {

stdfs::path root_of(const Config& c, Scope s) { return s == Scope::Shared ? c.shared_dir : c.state_dir; }

// The generation's name: "g" and a short digest of the text the kind's owner gave, or "g0" for no text.
std::string generation_name(const std::string& text) {
    if (text.empty()) return "g0";
    return "g" + crypto::to_hex(crypto::sha1(as_bytes(text))).substr(0, 12);
}

// Open (creating if need be) a cache file and make it ours: the format version is checked, and a file of another
// format is replaced.
Database open_cache_file(const stdfs::path& file) {
    for (int attempt = 0; attempt < 2; ++attempt) {
        Database db = Database::open(file);
        const std::vector<Row> version = db.query("PRAGMA user_version");
        const std::int64_t v = version.empty() || version[0].empty() ? 0 : as_int(version[0][0]);
        if (v == kFormatVersion) {
            db.exec("PRAGMA synchronous=NORMAL");
            return db;
        }
        if (v == 0) {
            const std::vector<Row> tables = db.query("SELECT name FROM sqlite_master WHERE name='entry'");
            if (tables.empty()) {
                db.exec("PRAGMA auto_vacuum=INCREMENTAL");   // (a new file only: rows deleted give the space back)
                db.query("PRAGMA journal_mode=WAL");         // (workers read while one writes)
                db.exec("PRAGMA synchronous=NORMAL");
                // The time of use is a table of its own: renewing it must not rewrite a big result.
                db.exec("CREATE TABLE entry(k TEXT PRIMARY KEY, v BLOB)");
                db.exec("CREATE TABLE usage(k TEXT PRIMARY KEY, used INTEGER) WITHOUT ROWID");
                db.exec("CREATE INDEX usage_used ON usage(used)");
                // An entry written (or replaced) is used now; an entry deleted takes its time of use with it.
                db.exec("CREATE TRIGGER entry_written AFTER INSERT ON entry BEGIN "
                        "INSERT OR REPLACE INTO usage(k, used) VALUES(new.k, unixepoch()); END");
                db.exec("CREATE TRIGGER entry_deleted AFTER DELETE ON entry BEGIN DELETE FROM usage WHERE k = old.k; END");
                db.exec(std::format("PRAGMA user_version={}", kFormatVersion));
                return db;
            }
        }
        // Another format (or an older cache with the right table but no version): not ours to read.
        db = Database::open(":memory:");   // release the file
        remove_quietly(file);
        for (const char* suffix : {"-wal", "-shm"}) {
            stdfs::path sidecar = file;
            sidecar += suffix;
            remove_quietly(sidecar);
        }
    }
    throw IoError(file, 0, "cannot create a cache file");
}

}  // namespace

Cache::Kind& Cache::open_kind(std::string_view name) {
    auto it = impl_->kinds.find(name);
    if (it == impl_->kinds.end()) it = impl_->kinds.emplace(std::string(name), Kind{}).first;
    Kind& kind = it->second;
    if (kind.opened) return kind;
    kind.opened = true;
    kind.generation = generation_name(kind.spec.generation);
    const stdfs::path dir = root_of(config_, kind.spec.scope) / std::string(name);
    try {
        if (kind.spec.scope == Scope::State) drop_other_generations(config_.state_dir, dir, kind.generation);
        fs::make_dirs(dir);
        kind.db.emplace(open_cache_file(dir / (kind.generation + kDbExtension)));
    } catch (const Error& e) {
        log::debug(std::format("cache: kind {} unavailable: {}", name, e.what()));
        kind.db.reset();
    }
    return kind;
}

std::optional<Bytes> Cache::get(std::string_view kind_name, std::string_view key, std::uint32_t schema) {
    if (config_.disabled) return std::nullopt;
    std::lock_guard lock(impl_->mutex);
    Kind& kind = open_kind(kind_name);
    std::optional<Bytes> result;
    if (kind.db) {
        try {
            std::vector<Row> rows =
                kind.db->query("SELECT e.v, u.used FROM entry e LEFT JOIN usage u ON u.k = e.k WHERE e.k=?",
                               {Value(std::string(key))});
            if (!rows.empty()) {
                if (auto* stored = std::get_if<Bytes>(&rows[0][0])) result = open_envelope(std::move(*stored), schema);
                const std::int64_t now = now_seconds();
                if (result && as_int(rows[0][1]) < now - kTouchSeconds)   // (used again: the last to go under the cap)
                    kind.db->exec("INSERT OR REPLACE INTO usage(k, used) VALUES(?,?)", {Value(std::string(key)), Value(now)});
            }
        } catch (const IoError& e) {
            log::debug(std::format("cache: get {}: {}", kind_name, e.what()));
        }
    }
    (result ? kind.hits.found : kind.hits.missed) += 1;
    return result;
}

void Cache::put(std::string_view kind_name, std::string_view key, ByteView data, std::uint32_t schema) {
    if (config_.disabled) return;
    {
        std::lock_guard lock(impl_->mutex);
        Kind& kind = open_kind(kind_name);
        if (!kind.db) return;
        try {
            // one statement, one transaction: a trigger of the file's makes the entry's time of use
            kind.db->exec("INSERT OR REPLACE INTO entry(k, v) VALUES(?,?)",
                          {Value(std::string(key)), Value(seal_envelope(data, schema))});
        } catch (const IoError& e) {   // (one that loses the lock is made again next time)
            log::debug(std::format("cache: put {}: {}", kind_name, e.what()));
            return;
        }
        impl_->written += data.size() + key.size();
        if (impl_->written < kCheckEvery && impl_->looked) return;
        impl_->written = 0;
        impl_->looked = true;
    }
    hold();
}

stdfs::path Cache::path(std::string_view kind_name, std::string_view key) {
    std::lock_guard lock(impl_->mutex);
    auto it = impl_->kinds.find(kind_name);
    if (it == impl_->kinds.end()) it = impl_->kinds.emplace(std::string(kind_name), Kind{}).first;
    Kind& kind = it->second;
    if (!kind.folder_ready) {
        const std::string generation = generation_name(kind.spec.generation);
        const stdfs::path dir = root_of(config_, kind.spec.scope) / std::string(kind_name);
        if (kind.spec.scope == Scope::State) drop_other_generations(config_.state_dir, dir, generation);
        kind.folder = dir / generation;
        kind.folder_ready = true;
    }
    return kind.folder / std::string(key.substr(0, 2)) / std::string(key);
}

// ---- tools and folders by content ----------------------------------------------------------------------------

std::string Cache::tool_bytes_id(const stdfs::path& tool) {
    const std::string name = lower_name(tool);
    std::error_code ec;
    if (!stdfs::is_regular_file(tool, ec)) return name + ":missing";
    const std::uint64_t size = stdfs::file_size(tool, ec);
    const std::int64_t mtime = stdfs::last_write_time(tool, ec).time_since_epoch().count();
    if (ec) return name + ":missing";
    const auto memo_key = std::make_tuple(text::lower(fs::to_text(stdfs::absolute(tool).lexically_normal())), size, mtime);
    {
        std::lock_guard lock(impl_->mutex);
        const auto it = impl_->digests.find(memo_key);
        if (it != impl_->digests.end()) return it->second;
    }
    std::string digest;
    try {
        fs::MappedFile mapped(tool);
        digest = name + ":" + crypto::to_hex(crypto::sha1(mapped.view()));
    } catch (const IoError&) {
        return name + ":missing";
    }
    std::lock_guard lock(impl_->mutex);
    impl_->digests[memo_key] = digest;
    return digest;
}

std::string Cache::files_bytes_id(std::span<const stdfs::path> folders) {
    std::vector<std::string> parts;
    for (const stdfs::path& top : folders) {
        // Top-down, the files of a folder in name order, then its subfolders in name order.
        std::vector<stdfs::path> stack{top};
        std::vector<stdfs::path> order;
        auto walk = [&](auto&& self, const stdfs::path& dir) -> void {
            std::error_code ec;
            std::vector<stdfs::path> dirs, files;
            for (const auto& e : stdfs::directory_iterator(dir, ec)) {
                std::error_code e2;
                (e.is_directory(e2) ? dirs : files).push_back(e.path());
            }
            std::sort(dirs.begin(), dirs.end());
            std::sort(files.begin(), files.end());
            for (const stdfs::path& f : files) {
                const std::string rel = text::lower(fs::to_text(stdfs::relative(f, top, ec)));
                parts.push_back(rel + "=" + tool_bytes_id(f));
            }
            for (const stdfs::path& d : dirs) self(self, d);
        };
        walk(walk, top);
    }
    return key({KeyPart(parts)});
}

// ---- the cap -------------------------------------------------------------------------------------------------

namespace {

struct Stored {
    stdfs::path path;
    bool is_db = false;
};

// Everything kept on disk under `base`, any generation: kind -> its files and folders.
std::map<std::string, std::vector<Stored>> stored_under(const stdfs::path& base) {
    std::map<std::string, std::vector<Stored>> out;
    std::error_code ec;
    if (!stdfs::is_directory(base, ec)) return out;
    std::vector<stdfs::path> kinds;
    for (const auto& e : stdfs::directory_iterator(base, ec)) kinds.push_back(e.path());
    std::sort(kinds.begin(), kinds.end());
    for (const stdfs::path& kind_dir : kinds) {
        const std::string kind = fs::to_text(kind_dir.filename());
        if (kind == kDropped || !stdfs::is_directory(kind_dir, ec)) continue;
        std::vector<stdfs::path> entries;
        for (const auto& e : stdfs::directory_iterator(kind_dir, ec)) entries.push_back(e.path());
        std::sort(entries.begin(), entries.end());
        for (const stdfs::path& p : entries) {
            const std::string name = fs::to_text(p.filename());
            if (is_sidecar(name)) continue;
            out[kind].push_back({p, name.ends_with(kDbExtension)});
        }
        out[kind];   // a kind with nothing in it still shows
    }
    return out;
}

std::uint64_t total_size(const stdfs::path& base) {
    std::uint64_t total = 0;
    for (const auto& [kind, entries] : stored_under(base))
        for (const Stored& s : entries) total += size_of(s.path);
    std::error_code ec;
    if (stdfs::is_directory(base / kDropped, ec)) total += size_of(base / kDropped);
    return total;
}

struct OldEntry {
    std::int64_t used;
    std::uint64_t size;
    stdfs::path db;        // empty for a file
    std::int64_t row_id;   // the row of a database entry
    stdfs::path file;      // the file of a path() result
};

}  // namespace

std::uint64_t Cache::hold(std::optional<std::uint64_t> state_cap) {
    std::uint64_t freed = 0;
    const std::pair<stdfs::path, std::uint64_t> scopes[] = {{config_.state_dir, state_cap.value_or(config_.max_bytes)},
                                                           {config_.shared_dir, config_.shared_max_bytes}};
    std::lock_guard lock(impl_->mutex);
    // A connection to a cache file: this process's own when it has one open, else a new one.
    auto with_db = [&](const stdfs::path& file, auto&& fn) {
        for (auto& [name, kind] : impl_->kinds) {
            if (kind.db && fs::same_path(kind.db->path(), file)) return fn(*kind.db);
        }
        Database db = Database::open(file);
        return fn(db);
    };
    auto shrink = [&](const stdfs::path& file) {   // the free pages cut off the file, the log folded in and emptied
        try {
            with_db(file, [](Database& db) {
                db.query("PRAGMA incremental_vacuum");
                db.query("PRAGMA wal_checkpoint(TRUNCATE)");
                return 0;
            });
        } catch (const IoError&) {
        }
    };
    for (const auto& [base, cap] : scopes) {
        if (base.empty()) continue;
        bool shrunk = false;
        for (int round = 0; round < kHoldRounds; ++round) {
            const std::uint64_t total = total_size(base);
            if (total <= cap) break;
            const auto stores = stored_under(base);
            if (!shrunk) {   // (the logs folded in first: they may be most of it)
                for (const auto& [kind, entries] : stores)
                    for (const Stored& s : entries)
                        if (s.is_db) shrink(s.path);
                shrunk = true;
                continue;
            }
            // The results used longest ago.
            std::vector<OldEntry> olds;
            for (const auto& [kind, entries] : stores) {
                for (const Stored& s : entries) {
                    if (s.is_db) {
                        try {
                            with_db(s.path, [&](Database& db) {
                                for (const Row& r : db.query("SELECT e.rowid, u.used, length(e.k) + length(e.v) FROM usage u "
                                                             "JOIN entry e ON e.k = u.k ORDER BY u.used LIMIT ?",
                                                             {Value(static_cast<std::int64_t>(kOldestPerFile))}))
                                    olds.push_back({as_int(r[1]), static_cast<std::uint64_t>(as_int(r[2])), s.path, as_int(r[0]), {}});
                                return 0;
                            });
                        } catch (const IoError&) {
                        }
                    } else {
                        std::error_code ec;
                        for (stdfs::recursive_directory_iterator it(s.path, ec), end; !ec && it != end; it.increment(ec)) {
                            std::error_code e2;
                            if (!it->is_regular_file(e2)) continue;
                            const std::uintmax_t n = it->file_size(e2);
                            const std::int64_t t = mtime_seconds(it->path(), e2);
                            if (!e2) olds.push_back({t, n, {}, 0, it->path()});
                        }
                    }
                }
            }
            std::stable_sort(olds.begin(), olds.end(), [](const OldEntry& a, const OldEntry& b) { return a.used < b.used; });
            const std::uint64_t need = total - static_cast<std::uint64_t>(static_cast<double>(cap) * 0.8);
            std::uint64_t got = 0;
            std::map<stdfs::path, std::vector<std::int64_t>> rows_gone;
            std::vector<stdfs::path> files_gone;
            for (const OldEntry& o : olds) {
                if (got >= need) break;
                if (o.db.empty()) files_gone.push_back(o.file);
                else rows_gone[o.db].push_back(o.row_id);
                got += o.size;
            }
            if (got == 0) break;
            for (const stdfs::path& f : files_gone) remove_quietly(f);
            for (const auto& [file, ids] : rows_gone) {
                try {
                    with_db(file, [&](Database& db) {
                        Database::Transaction tx(db);
                        for (std::int64_t id : ids) {
                            db.exec("DELETE FROM entry WHERE rowid=?", {Value(id)});   // (its time of use goes by trigger)
                        }
                        tx.commit();
                        return 0;
                    });
                } catch (const IoError&) {
                    continue;
                }
                shrink(file);
            }
            freed += got;
        }
    }
    return freed;
}

// ---- seeing and emptying it ----------------------------------------------------------------------------------

std::map<std::string, HitCount> Cache::counts() const {
    std::lock_guard lock(impl_->mutex);
    std::map<std::string, HitCount> out;
    for (const auto& [name, kind] : impl_->kinds)
        if (kind.hits.found != 0 || kind.hits.missed != 0) out[name] = kind.hits;
    return out;
}

std::vector<KindStats> Cache::stats() {
    std::lock_guard lock(impl_->mutex);
    std::vector<KindStats> out;
    auto count_rows = [&](const stdfs::path& file) -> std::uint64_t {
        for (auto& [name, kind] : impl_->kinds)
            if (kind.db && fs::same_path(kind.db->path(), file)) {
                try {
                    return static_cast<std::uint64_t>(as_int(kind.db->query("SELECT count(*) FROM entry")[0][0]));
                } catch (const IoError&) {
                    return 0;
                }
            }
        try {
            Database db = Database::open(file);
            return static_cast<std::uint64_t>(as_int(db.query("SELECT count(*) FROM entry")[0][0]));
        } catch (const IoError&) {
            return 0;
        }
    };
    for (Scope scope : {Scope::State, Scope::Shared}) {
        const stdfs::path base = root_of(config_, scope);
        if (base.empty()) continue;
        for (const auto& [kind, entries] : stored_under(base)) {
            auto declared = impl_->kinds.find(kind);
            const bool mine = declared != impl_->kinds.end() ? declared->second.spec.scope == scope : scope == Scope::State;
            if (!mine) continue;
            KindStats s;
            s.kind = kind;
            s.scope = scope;
            for (const Stored& e : entries) {
                s.bytes += size_of(e.path);
                s.generations.push_back(generation_stem(fs::to_text(e.path.filename())));
                if (e.is_db) {
                    s.results += count_rows(e.path);
                } else {
                    std::error_code ec;
                    for (stdfs::recursive_directory_iterator it(e.path, ec), end; !ec && it != end; it.increment(ec)) {
                        std::error_code e2;
                        if (it->is_regular_file(e2)) s.results += 1;
                    }
                }
            }
            if (declared != impl_->kinds.end()) s.current = generation_name(declared->second.spec.generation);
            out.push_back(std::move(s));
        }
    }
    return out;
}

std::vector<Dropped> Cache::prune() {
    std::vector<Dropped> dropped;
    {
        std::lock_guard lock(impl_->mutex);
        for (const auto& [kind, entries] : stored_under(config_.state_dir)) {
            auto declared = impl_->kinds.find(kind);
            if (declared == impl_->kinds.end() || declared->second.spec.scope != Scope::State) continue;   // owner unknown
            const std::string generation = generation_name(declared->second.spec.generation);
            Dropped d{kind, {}};
            for (const Stored& s : entries) {
                const std::string name = fs::to_text(s.path.filename());
                if (generation_stem(name) != generation) d.generations.push_back(name);
            }
            if (d.generations.empty()) continue;
            Kind& k = declared->second;
            if (k.opened && k.db) {   // close before its files move
                k.db.reset();
                k.opened = false;
            }
            k.folder_ready = false;
            drop_other_generations(config_.state_dir, config_.state_dir / kind, generation);
            dropped.push_back(std::move(d));
        }
        remove_quietly(config_.state_dir / kDropped);
    }
    hold();
    return dropped;
}

void Cache::close() {
    std::lock_guard lock(impl_->mutex);
    for (auto& [name, kind] : impl_->kinds) {
        kind.db.reset();
        kind.opened = false;
        kind.folder_ready = false;
    }
}

std::uint64_t Cache::clear() {
    close();
    std::lock_guard lock(impl_->mutex);
    const std::uint64_t total = total_size(config_.state_dir);
    std::error_code ec;
    std::vector<stdfs::path> entries;
    for (const auto& e : stdfs::directory_iterator(config_.state_dir, ec)) entries.push_back(e.path());
    for (const stdfs::path& p : entries) remove_quietly(p);
    return total;
}

std::string describe(const std::vector<KindStats>& stats, std::uint64_t cap) {
    std::string out = std::format("{:<16} {:>10} {:>10}  {}\n", "kind", "size", "results", "generations (* = of now)");
    std::uint64_t total = 0;
    for (const KindStats& s : stats) {
        if (s.scope != Scope::State) continue;
        total += s.bytes;
        std::string gens;
        for (const std::string& g : s.generations) {
            if (!gens.empty()) gens += ' ';
            gens += g;
            if (s.current && g == *s.current) gens += '*';
        }
        out += std::format("{:<16} {:>10} {:>10}  {}\n", s.kind, format_size(s.bytes), s.results, gens);
    }
    out += std::format("{:<16} {:>10}   (cap {})\n", "all", format_size(total), format_size(cap));
    return out;
}

}  // namespace cb::base::cache
