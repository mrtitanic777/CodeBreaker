// cb/knowledge/show.hpp -- what the knowledge base holds, and where each piece came from.
//
//     codebreaker knowledge                          every platform's kinds
//     codebreaker knowledge <platform>               ... of one platform
//     codebreaker knowledge <platform> <kind>        its keys (the first 200)
//     codebreaker knowledge <platform> <kind> <key>  one piece: its value, source, origin
//     codebreaker knowledge --load                   load every authored file first
//
// The command is a pure function of what the base answers: `run` asks a `Base` (the knowledge store implements it over
// its database) and returns the text for stdout and the exit status, so it is tested without a database or a terminal.
// Exit status: 0 on success, 1 when the asked-for piece is unknown ("nothing known").
#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/knowledge/value.hpp"

namespace cb::knowledge::show {

inline constexpr std::size_t kMaxValueBytes = 20000;   // how much of one value the command shows
inline constexpr std::size_t kKeyListLimit = 200;       // how many keys of a kind it lists
inline constexpr std::size_t kSourceColumns = 72;       // how much of a kind's source the listing shows

// One kind of one platform, as the listing shows it.
struct KindRow {
    std::string platform;
    std::string kind;
    std::int64_t keys = 0;                         // how many keys the kind has
    std::int64_t bytes = 0;                        // the size of its values
    std::map<std::string, std::int64_t> origins;   // {origin: how many keys came that way}, by origin name
    std::string source;                            // what the kind was read or worked out from
};

// The body store: function bodies seen in any module.
struct SharedBodies {
    std::int64_t recorded = 0;
    std::int64_t distinct = 0;
    std::int64_t in_several_modules = 0;
};

// Where a piece came from.
struct Provenance {
    std::string source;
    std::string origin;                  // authored, derived, learned or given
    std::optional<double> made_at;       // seconds since the epoch; unknown reads as now
    std::string hash;
};

// A key's value: bytes, text or a structured value (what the base keeps a fact as).
struct Stored {
    enum class Kind { Bytes, Text, Structured } kind = Kind::Structured;
    Bytes bytes;
    std::string text;
    Value structured;
};

// What `run` asks of the knowledge base.
class Base {
public:
    virtual ~Base() = default;
    virtual std::string path() const = 0;                                   // where the base lives
    virtual std::string platform(std::string_view name) const = 0;          // the base's name for a lane's (ne -> win16)
    virtual void load_authored() = 0;                                       // load every authored file into the base
    virtual std::vector<KindRow> kinds(std::optional<std::string_view> platform) = 0;
    virtual SharedBodies shared_bodies() = 0;
    // The first `limit` keys of a kind (in the kind's order), and how many it has.
    virtual std::vector<std::string> first_keys(std::string_view platform, std::string_view kind, std::size_t limit,
                                                std::size_t& total) = 0;
    // The value of a key (authored tables first, then the base); nullopt when there is none.
    virtual std::optional<Stored> value(std::string_view platform, std::string_view kind, std::string_view key) = 0;
    virtual std::optional<Provenance> about(std::string_view platform, std::string_view kind, std::string_view key) = 0;
};

struct Outcome {
    std::string text;   // for stdout, ending in a newline
    int exit_code = 0;
};

// The command line after `knowledge` (see the top of this file).
Outcome run(Base& base, std::span<const std::string> args);

// A stored value as the command shows it: JSON indented by one space (bytes as a short hex preview). `run` shows at most
// kMaxValueBytes of it, cut at a character.
std::string dump(const Stored& value);

// `2026-11-12 03:04 AM`: the local time of seconds since the epoch.
std::string local_time_text(double epoch_seconds);

}  // namespace cb::knowledge::show
