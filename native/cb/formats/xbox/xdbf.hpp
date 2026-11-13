// cb/formats/xbox/xdbf.hpp -- XDBF / GPD, the Xbox 360's database format: gamer profiles, achievements, a title's own SPA.
//
//     xdbf::Database db = xdbf::Database::parse(bytes);
//     for (const xdbf::Achievement& a : db.achievements()) ...       name, descriptions, score, unlocked
//     db.unlock_achievement(1);  Bytes edited = db.build();         edit and rebuild (the heap is compacted)
//
// A database is a header, an entry table, a free-space table, then a heap of payloads. Entries are keyed by (namespace, id); the namespace
// says what the payload is (1 achievements, 2 images, 3 settings, 4 titles played, 5 strings, 6 and 7 avatar awards). Header and tables are
// big-endian: "XDBF", version, entry table capacity, entries used, free table capacity, free entries used; each entry is
// {u16 namespace, u64 id, u32 heap offset, u32 length}; the heap starts after both tables.
//
// An achievement record (namespace 1) is: size, id, image id, score, flags (0x20000: unlocked), unlock FILETIME, then the UTF-16BE name,
// unlocked description and locked description, each ended by a zero unit.
//
// A database owns its payloads: `parse` copies them out of the file, so edits (`set_payload`, `unlock_achievement`) are what `payload` and
// `achievements` then see, and `build` writes. Entry order is the file's.
//
// Errors: FormatError (TruncatedError) for bytes that are not a database, or whose entries lie outside the file.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"

namespace cb::formats::xbox::xdbf {

inline constexpr std::string_view kMagic = "XDBF";
inline constexpr std::uint32_t kAchievedFlag = 0x20000;
// The unlock time stamped on an achievement unlocked here: a fixed, plausible date (2015-06-15 12:00 UTC) as a FILETIME.
inline constexpr std::uint64_t kUnlockFiletime = 130788648000000000ull;

// The namespaces: "achievement", "image", "setting", "title", "string", "avatar-award", "avatar-award-1"; "ns<N>" for any other.
std::string namespace_name(std::uint16_t ns);

struct Entry {
    std::uint16_t ns = 0;
    std::uint64_t id = 0;
    Bytes payload;
};

struct Achievement {
    std::uint32_t id = 0, image = 0, score = 0, flags = 0;
    bool unlocked = false;
    std::string name, unlocked_desc, locked_desc;
};

// One achievement record, or nullopt for a payload too short to be one (under 0x1C bytes).
std::optional<Achievement> decode_achievement(ByteView payload);

class Database {
public:
    static Database parse(ByteView file);

    std::uint32_t version = 0;
    std::uint32_t entry_table_len = 0;   // the capacity the file declared; a rebuild never goes below it
    std::uint32_t free_table_len = 0;
    std::uint32_t free_count = 0;
    std::vector<Entry> entries;

    // Where the heap starts in the file this was read from.
    std::uint64_t data_start() const { return 0x18 + std::uint64_t{entry_table_len} * 0x12 + std::uint64_t{free_table_len} * 8; }

    // The first entry with this key, or null.
    const Entry* find(std::uint16_t ns, std::uint64_t id) const;
    std::optional<ByteView> payload(std::uint16_t ns, std::uint64_t id) const;
    // Entries per namespace, by namespace number.
    std::map<std::uint16_t, std::size_t> counts() const;
    // Every namespace-1 record that is long enough to decode, in entry order.
    std::vector<Achievement> achievements() const;

    // Replace the payload of an entry, or add the entry at the end.
    void set_payload(std::uint16_t ns, std::uint64_t id, Bytes payload);
    // Set (or clear) the unlocked flag and stamp (or zero) the unlock time on the first achievement record with this id. False when none.
    bool unlock_achievement(std::uint32_t id, bool unlock = true);
    // Unlock every achievement; how many were found to unlock.
    std::size_t unlock_all();

    // The database as a file: entries in order, the heap packed with no gaps, tables as large as the file declared (or as the entries need).
    Bytes build() const;
    // Every payload as <namespace>_<id>.bin in `folder`; how many.
    std::size_t extract_all(const std::filesystem::path& folder) const;
};

// `xdbf info|list|ach|extract|unlock|unlock-all|set <file.gpd> ...`: the module's command line.
cli::Command command();

}  // namespace cb::formats::xbox::xdbf
