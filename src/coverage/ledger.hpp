// coverage/ledger.hpp -- the coverage ledger: how much of every module of the corpus is accounted for.
//
//     codebreaker coverage                  by kind of module, then every module the store holds
//     codebreaker coverage --all            list the modules nothing has been done to as well
//     codebreaker coverage --module NAME    one module: its counts, its bytes, why functions fail
//     codebreaker coverage --csv FILE       one row per corpus module, for a spreadsheet
//     codebreaker coverage --json           the whole ledger as JSON
//
//     store::FunctionStore store(index, folder, /*readonly*/ true);
//     ledger::Ledger ledger(services, &store);
//     ledger::Report report = ledger.measure();             // {profiles, kinds, total, modules}
//     for (const std::string& line : ledger::render(report)) out("{}", line);
//
// Two sources and nothing else: the corpus (base/corpus) says which modules exist and how much code each holds; the
// function store (engine/function_store) says what has been made of each function. A row -- for a module, a kind of
// module, or everything -- counts functions / decompiled / compiled, the store's classes (verified, weak, unverified,
// not-judged, no-compile, no-source: they add up to `functions`) and bytes (code_bytes, bytes_known, bytes_verified,
// bytes_weak). The store is opened read-only: measuring coverage never changes a result and can run beside a sweep.
//
// Kinds are 32-bit (PE), 16-bit (NE), driver (LE and the W3 bundle), DOS and Java. Only the first two can be reconstructed
// and judged today, so the others show what is waiting.
//
// A Ledger remembers each module's counts until the store's summary of it changes (its stamp), so a second measure of an
// unchanged store reads nothing but the summaries. The code size of each corpus file is remembered in
// <state>/coverage_corpus.json until the file changes.
//
// What the ledger needs of the rest of the program -- the corpus, where the state folder is, which verifier profile
// each kind of module is judged under -- comes in through `Services`, so the module owns no global.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "base/corpus.hpp"
#include "core/cli.hpp"
#include "core/json_fwd.hpp"
#include "store/function_store.hpp"
#include "store/verdicts.hpp"

namespace cb::coverage::ledger {

// The kinds of module, in the order they are listed.
struct Kind {
    std::string_view id, title;
};
inline constexpr std::array<Kind, 5> kKinds = {{{"pe", "32-bit"}, {"ne", "16-bit"}, {"le", "driver"}, {"dos", "DOS"}, {"java", "Java"}}};
// The title of a kind ("32-bit"); "" for a name that is not one.
std::string_view kind_title(std::string_view kind);

// The numbers of a row.
struct Counts {
    std::int64_t functions = 0, decompiled = 0, compiled = 0;
    store::ClassCount classes;                     // {class: functions} for every class of store::kClassOrder
    std::int64_t code_bytes = 0, bytes_known = 0, bytes_verified = 0, bytes_weak = 0;
    std::int64_t judged = 0;                        // functions with a result under the settings in force
    Counts();
};

// One module of the corpus: its code size comes from the inspector.
struct CorpusModule {
    std::string name, path, kind;
    std::int64_t size = 0, code_bytes = 0;
};

// A module row of the ledger.
struct Row : Counts {
    std::string name, path, kind;
    std::int64_t size = 0;
    bool in_store = false, measured = false;        // measured: every known function has a result under the current settings
    std::optional<std::string> sha;                 // set when the store holds the module
};

// A kind row (or the total): the rows' sums, and how many modules / how many in the store / how many measured.
struct Summary : Counts {
    std::string kind, name;
    std::int64_t modules = 0, in_store = 0, measured = 0;
};

struct Report {
    std::map<std::string, std::optional<std::string>> profiles;   // {kind: digest of the verifier settings}
    std::vector<Summary> kinds;                                   // in the order of kKinds
    Summary total;
    std::vector<Row> modules;                                     // by name
    std::string generated, corpus;
};

// What the ledger is built from.
struct Services {
    const base::Corpus* corpus = nullptr;           // required
    std::filesystem::path state_dir;                // coverage_corpus.json is kept here
    // The verifier settings in force now, per kind of module (a kind that cannot be worked out on this machine maps to
    // nullopt: the settings the store used last stand in). Default: none known.
    std::function<std::map<std::string, std::optional<std::string>>()> verifier_profiles;
    // The time of day written in the report, "2026-11-12 14:03:00". Default: the local clock.
    std::function<std::string()> now_text;
    // Where the function store is, for the `coverage` command (opened read-only).
    std::filesystem::path store_index, store_folder;
};

class Ledger {
public:
    // `store` is not owned and may be null (no store: the corpus is listed alone); it should be read-only.
    Ledger(Services services, store::FunctionStore* store);

    // {kind: digest} -- the verifier settings in force now.
    std::map<std::string, std::optional<std::string>> profiles() const;

    // Every module of the corpus, one per name, by name. `refresh`: read every file's code size again.
    std::vector<CorpusModule> corpus_modules(bool refresh = false);

    // One module's counts under `digest` (none: nothing can have been judged). `path` and `kind` say how far each known
    // function reaches. The module's counts are kept until its summary in the store changes.
    Counts counts(const std::string& sha, const std::optional<std::string>& digest, const std::filesystem::path& path,
                  const std::string& kind);

    // The ledger. `module` limits it to one name (with or without its extension).
    Report measure(std::optional<std::string_view> module = std::nullopt, bool refresh = false);

    // {class: [(reason, functions)]} -- why the functions of a module that are not verified are not, commonest first, at
    // most `most` each; classes in the order of store::kClassOrder.
    using Reasons = std::vector<std::pair<store::Class, std::vector<std::pair<std::string, std::int64_t>>>>;
    Reasons reasons(const std::string& sha, const std::string& digest, std::size_t most = 8) const;

    store::FunctionStore* store() const { return store_; }

private:
    struct Stamp {
        std::int64_t functions = 0, judged = 0;
        std::optional<double> last;
        friend bool operator==(const Stamp&, const Stamp&) = default;
    };

    Services services_;
    store::FunctionStore* store_;
    // (sha, digest) -> (the summary it was counted at, the counts): looked up
    std::map<std::pair<std::string, std::string>, std::pair<Stamp, Counts>> kept_;
    std::map<std::string, Stamp> stamps_;   // the summaries of the last measure
};

// {address: bytes} -- how far each known function reaches: to the next known function of its section or segment, or to
// that section's end. Addresses are the store's: hexadecimal for a 32-bit module, "segment:offset" for a 16-bit one. A
// module that cannot be read has none; an address that does not parse or lies outside the code is left out.
std::map<std::string, std::int64_t> extents(const std::filesystem::path& path, std::string_view kind, const std::vector<std::string>& addrs);

// The sums of rows.
Summary total_of(const std::vector<Row>& rows);

// ---- as text
// "  4.5%" of a over b; "     -" when b is zero.
std::string percent(std::int64_t a, std::int64_t b);
// Rows of columns, the first left-aligned and the rest right-aligned, trailing blanks cut; the first row is the head.
std::vector<std::string> table(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& rows);
std::vector<std::string> render(const Report& report, bool every = false);
// "measured", "part measured", "no code" (in the store, nothing in it to measure), "not measured now" or "untouched".
std::string_view state_of(const Row& row);
// One row per module, CRLF line ends; -> how many.
std::size_t write_csv(const Report& report, const std::filesystem::path& path);
// The report as the `--json` document.
Json to_json(const Report& report);
// What `--module` prints of one row (the counts, the bytes, why functions fail).
std::vector<std::string> describe_row(const Row& row, const Report& report, const Ledger& ledger);

// `coverage [--module NAME] [--all] [--csv FILE] [--json] [--refresh]`.
cli::Command command(const Services& services);

// The services over the corpus and the function store of a state folder (store/functions.hpp: its index file, CB_FUNCTIONS_DB, and the folder
// of its modules' databases). `verifier_profiles`: the verifier settings in force now per kind of module (what verify's Reconstructor::digests
// says); without it none are known and the settings the store used last stand in. The corpus must outlive the services.
Services services_over(const base::Corpus& corpus, const std::filesystem::path& state_dir,
                       std::function<std::map<std::string, std::optional<std::string>>()> verifier_profiles = {});

}  // namespace cb::coverage::ledger
