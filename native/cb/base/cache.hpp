// cb/base/cache.hpp -- never do the same work twice: results kept by what they were made from.
//
//     cache::Cache cache(config);
//     cache.declare_kind("masm", {.scope = cache::Scope::Shared, .generation = kMasmCacheVersion});
//     const std::string k = cache::key({cache.tool_bytes_id(masm_exe), flags, source});   // all the result depends on
//     if (auto hit = cache.get("masm", k)) return *hit;                                   // bytes, or a miss
//     ... make the result ...
//     cache.put("masm", k, result);
//     cache.keep("checks", k, verdict);                                                    // a typed value (cache_codec.hpp)
//     std::optional<Verdict> again = cache.value<Verdict>("checks", k);
//
// A compiler given the same source and flags gives the same object file; the decompiler given the same bytes gives
// the same C. Both take most of the time of every run, and most of what a run is given is what the run before was
// given. So the result is filed under a hash of everything it depends on -- the tool (its name, size and date, or
// its bytes), its options, its input -- and looked up before the tool is started. Nothing is ever stale: a
// different input is a different name.
//
// But a name no one asks for again is never asked for again, and kept for ever it is only weight. So the cache
// looks after its own size, by two rules:
//
//   GENERATIONS  every kind of result is made by some version of the code, and the owner of the kind says which
//                (KindSpec::generation: any text that changes when the code does -- a revision constant, a build
//                id). Results of different generations are kept in different files, and the first time a kind is
//                used in a process the files of every other generation go: the code of now can never ask for them.
//   THE CAP      all the caches of one scope together are held under `Config::max_bytes`. As results are written,
//                past the cap the ones used longest ago go first, from any kind, until four fifths of it is used.
//
// SCOPES       a State kind lives in the state folder (state/cache). A Shared kind -- what an assembler or a
//              compiler made of its inputs, named by the bytes of the tools and files that went in (tool_bytes_id,
//              files_bytes_id), never by a path or a date -- lives in the build cache every checkout shares. Shared
//              kinds are never dropped by generation (another checkout may run other code), only by the cap.
//
// FORMAT       a kind is one SQLite file, <root>/<kind>/<generation>.cbc: a table entry(k, v) whose values are
//              wrapped in a small envelope (magic, the writer's schema number, length, CRC-32C), a table usage(k, used)
//              holding when each was last used (apart, so that renewing it never rewrites a result; triggers keep it in step
//              with entry), and
//              PRAGMA user_version = kFormatVersion. An entry whose envelope is wrong, whose schema is not the
//              reader's, or whose checksum fails is simply a miss; a file of another format version is replaced.
//              A kind whose results are files a tool writes itself (path()) has a folder,
//              <root>/<kind>/<generation>/<first two key characters>/<key>, instead. Nothing older than this format
//              is read: a cache the Python wrote is a cache that is rebuilt. Deleting the folder costs time and
//              nothing else. `Config::disabled` (CB_NO_CACHE=1) turns every lookup into a miss and keeps nothing.
//
// One Cache is thread-safe and meant to be shared by every worker of a process.
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <initializer_list>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "cb/base/cache_codec.hpp"
#include "cb/core/bytes.hpp"
#include "cb/core/io.hpp"

namespace cb::base::cache {

inline constexpr std::uint32_t kFormatVersion = 1;                        // PRAGMA user_version of a cache file
inline constexpr std::uint64_t kGiB = std::uint64_t{1} << 30;
inline constexpr std::uint64_t kDefaultMaxBytes = 2 * kGiB;               // state caches together
inline constexpr std::uint64_t kDefaultSharedMaxBytes = 6 * kGiB;         // the shared build cache

// ---- the envelope's checksum ---------------------------------------------------------------------------------

// CRC-32C of an entry's payload (the value of "123456789" is 0xE3069283). entry_checksum uses the CPU's crc32
// instruction where there is one; the portable form is the table-driven one and gives the same value.
std::uint32_t entry_checksum(ByteView data);
std::uint32_t entry_checksum_portable(ByteView data);

// ---- keys ----------------------------------------------------------------------------------------------------

// One ingredient of a key. Text and bytes with the same content are the same part, as are an integer and its
// decimal text.
class KeyPart {
public:
    KeyPart(std::string_view text) : bytes_(text) {}                                          // NOLINT: implicit
    KeyPart(const char* text) : bytes_(text) {}                                               // NOLINT
    KeyPart(const std::string& text) : bytes_(text) {}                                        // NOLINT
    KeyPart(ByteView bytes) : bytes_(as_text(bytes)) {}                                       // NOLINT
    KeyPart(const Bytes& bytes) : bytes_(as_text(bytes)) {}                                   // NOLINT
    template <std::integral T>
    KeyPart(T v) : bytes_(integer_text(v)) {}                                                 // NOLINT
    KeyPart(double v);                                                                        // shortest round-trip text
    KeyPart(const std::vector<std::string>& list);                                            // joined with 0x1f
    const std::string& bytes() const { return bytes_; }

private:
    template <class T>
    static std::string integer_text(T v) {
        if constexpr (std::is_same_v<T, bool>) return v ? "true" : "false";
        else return std::to_string(v);
    }
    std::string bytes_;
};

// The SHA-1 (lowercase hex) of the parts, each length-prefixed so that ("ab") and ("a", "b") differ.
std::string key(std::initializer_list<KeyPart> parts);
std::string key(std::span<const KeyPart> parts);

// What identifies a program by its name, size and modification time (seconds): a newer build is a different tool.
std::string tool_id(const std::filesystem::path& tool);

// A folder of files a tool reads, by each file's name, size and modification time.
std::string folder_id(const std::filesystem::path& folder);

// ---- configuration and kinds ---------------------------------------------------------------------------------

enum class Scope { State, Shared };

struct Config {
    std::filesystem::path state_dir;                          // where State kinds are kept (state/cache)
    std::filesystem::path shared_dir;                         // the shared build cache
    std::uint64_t max_bytes = kDefaultMaxBytes;               // cap on everything under state_dir
    std::uint64_t shared_max_bytes = kDefaultSharedMaxBytes;  // cap on everything under shared_dir
    bool disabled = false;                                    // every lookup misses, nothing is kept

    // CB_NO_CACHE is set to anything non-empty.
    static bool disabled_by_environment();
};

struct KindSpec {
    Scope scope = Scope::State;
    // Text that changes whenever the code that makes this kind's results changes; empty: one generation.
    std::string generation;
};

// ---- seeing it -----------------------------------------------------------------------------------------------

struct HitCount {
    std::uint64_t found = 0;
    std::uint64_t missed = 0;
};

struct KindStats {
    std::string kind;
    Scope scope = Scope::State;
    std::uint64_t bytes = 0;                    // on disk, every generation
    std::uint64_t results = 0;
    std::vector<std::string> generations;       // on disk
    std::optional<std::string> current;         // the generation of now, if the kind is declared
};

struct Dropped {
    std::string kind;
    std::vector<std::string> generations;
};

// The text of `codebreaker cache --stats`: one row per kind and a total against the cap.
std::string describe(const std::vector<KindStats>& stats, std::uint64_t cap);

// ---- the cache -----------------------------------------------------------------------------------------------

class Cache {
public:
    explicit Cache(Config config);
    Cache(const Cache&) = delete;
    Cache& operator=(const Cache&) = delete;
    ~Cache();

    const Config& config() const { return config_; }

    // Say who makes a kind and how it is kept. Kinds never declared are State kinds with one generation. Must come
    // before the kind's first use in this process (a later declaration of a kind already in use is ignored).
    // The kinds the compilers and assemblers fill are Shared by default: see Cache's constructor.
    void declare_kind(std::string_view kind, KindSpec spec);

    // The result under `key`, or nothing. `schema` is the layout number the writer used (0 for raw bytes).
    std::optional<Bytes> get(std::string_view kind, std::string_view key, std::uint32_t schema = 0);
    // File a result. A failure to write (the lock is lost, the disk is full) loses only the caching.
    void put(std::string_view kind, std::string_view key, ByteView data, std::uint32_t schema = 0);

    // A typed value (cache_codec.hpp). value() is a miss for an entry written under another layout.
    template <class T>
    std::optional<T> value(std::string_view kind, std::string_view key) {
        std::optional<Bytes> data = get(kind, key, Codec<T>::kSchema);
        if (!data) return std::nullopt;
        try {
            io::ByteReader r(*data, "cache value");
            return Codec<T>::read(r);
        } catch (const FormatError&) {   // damaged payload under a valid envelope: treat it as absent
            return std::nullopt;
        }
    }
    template <class T>
    void keep(std::string_view kind, std::string_view key, const T& value) {
        io::ByteWriter w;
        Codec<T>::write(w, value);
        put(kind, key, w.data(), Codec<T>::kSchema);
    }

    // Where the result filed under `key` is, for a tool that writes its own file (the folder is made when the file
    // is). Such files count toward the cap too; call hold() after writing.
    std::filesystem::path path(std::string_view kind, std::string_view key);

    // What identifies a program, or a folder of files a tool reads, by content: for keys in a Shared kind. The
    // digest of a file is remembered by (path, size, modification time), so asking again is cheap.
    std::string tool_bytes_id(const std::filesystem::path& tool);
    std::string files_bytes_id(std::span<const std::filesystem::path> folders);

    // Bring each scope back under its cap: past it, the results used longest ago go first, from any kind, until
    // four fifths of it is used. Returns the bytes freed (as the entries counted them). `state_cap` replaces the
    // state folder's cap for this call.
    std::uint64_t hold(std::optional<std::uint64_t> state_cap = std::nullopt);

    // Found/missed lookups per kind in this process, for a run's summary.
    std::map<std::string, HitCount> counts() const;
    // What is on disk now, kind by kind (State scope, then Shared).
    std::vector<KindStats> stats();
    // Every declared State kind back to its generation of now, and the cap held. Returns what was dropped.
    std::vector<Dropped> prune();
    // Everything kept in the state folder, gone. Returns bytes freed.
    std::uint64_t clear();
    // Close the open files (and forget the folders of path()).
    void close();

private:
    struct Kind;
    struct Impl;
    Kind& open_kind(std::string_view kind);
    Config config_;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::base::cache
