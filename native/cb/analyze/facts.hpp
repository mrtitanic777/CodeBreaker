// cb/analyze/facts.hpp -- the fact table: what the analyses found, kept beside the function store.
//
//     FactStore store = FactStore::open(facts::default_path(config));   // the function store's own sqlite file
//     store.put(sha, "conventions", rows);                              // replaces that module's facts of that analysis
//     store.get(sha, {.analysis = "conventions", .addr = "bfee83ad"});
//     store.get(sha, {.kind = "structure"});
//     store.tally(sha);                                                 // {analysis: {kind: count}}
//
// A fact is one thing an analysis established: about a function (`addr` is that function's key in the function store
// -- the address in hex for a 32-bit module or a driver, "segment:offset" for a 16-bit one) or about the module as a
// whole (`addr` empty). `kind` says what it is ("convention", "structure", "global", "class", "switch", ...) and
// `value` holds the rest as a document (knowledge::Value: null, bool, integer, double, text, list, ordered map).
//
// The table lives in the same file as the function store, so a function's reconstruction, its verdicts and what is
// known about it are one query apart; it is created here, on first use, and nothing in the store depends on it.
// A value is stored as JSON text with its object keys sorted, so equal facts are equal text; an object comes back with
// its keys in that sorted order.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/analyze/native.hpp"
#include "cb/base/config.hpp"
#include "cb/base/sqlite.hpp"
#include "cb/knowledge/value.hpp"

namespace cb::analyze::facts {

// The function store's key for an address of a module of this kind: "segment:offset" ("%d:%04x") for a 16-bit
// module, the address in hex otherwise; "" for no address (a fact about the module as a whole).
std::string key(ImageKind kind, std::optional<Va> va);

// One row of what an analysis has to say about a module.
struct Row {
    std::string addr;               // the function's key (see key()), or "" for the module
    std::string kind;
    knowledge::Value value;
};

// A stored fact.
struct Fact {
    std::string analysis;
    std::string addr;
    std::string kind;
    knowledge::Value value;
};

// What get() narrows the module's facts to (an empty member does not narrow).
struct Filter {
    std::optional<std::string> analysis;
    std::optional<std::string> addr;
    std::optional<std::string> kind;
};

// {analysis: {kind: how many}}: sorted, so a report is stable.
using Tally = std::map<std::string, std::map<std::string, std::int64_t>>;

// The function store's file: the environment's CB_FUNCTIONS_DB, else functions.sqlite in the state folder.
std::filesystem::path default_path(const config::Config& config);

class FactStore {
public:
    // Open (creating the file, its folder and the table). IoError when the file cannot be opened.
    static FactStore open(const std::filesystem::path& file);

    const std::filesystem::path& path() const { return db_.path(); }

    // Everything `analysis` has to say about `module`, replacing what it said before: one transaction, so a failed
    // put leaves the old facts.
    void put(std::string_view module, std::string_view analysis, std::span<const Row> rows);

    // The module's facts, narrowed by the filter, in the order they were put.
    std::vector<Fact> get(std::string_view module, const Filter& filter = {});

    Tally tally(std::string_view module);

private:
    explicit FactStore(base::sqlite::Database db) : db_(std::move(db)) {}
    base::sqlite::Database db_;
};

// The text a value is stored as, and its reading back (FormatError / ParseError for text that is not a document).
std::string to_text(const knowledge::Value& value);
knowledge::Value from_text(std::string_view text);

}  // namespace cb::analyze::facts
