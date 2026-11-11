// cb/java/ledger.hpp -- the structured record of everything the converter does to a file.
//
// The whole point of the restructure is that the converter must TELL YOU what it did. Every observable action
// becomes an Event with a Category, a Severity, a location (file + line), and a human message. A Ledger is just a
// queryable bag of Events plus the roll-ups the reports need (per-file grouping, risk scoring, totals).
//
// Nothing here knows *how* events are produced -- analyzers (post-hoc) and the generator/rules (instrumented) both
// just call ledger.add(...). That separation is the contract that lets us swap the monolith out underneath the
// reporting without touching the reports.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::ledger {

// WHAT kind of thing happened to the code. Ordered roughly by how much it should worry you, worst first.
enum class Category {
    Error,        // compiler said no (from a build log) -- definitely broken
    Guessed,      // converter made a heuristic call that could be wrong (value/pointer, inferred element type) --
                  //   THE category most likely to be a latent bug
    Stubbed,      // replaced with a placeholder; needs a hand implementation
    Blocked,      // explicitly could-not-port (PORT-BLOCKED)
    Risk,         // matches a known crash/leak pattern (from converter_lint)
    Warning,      // suspicious; verify by hand (PORT-WARNING, TODO/XXX/FIXME)
    Overridden,   // a hand-written file replaced generated output for this class
    Annotated,    // an explanatory note about a decision already taken
    Ported,       // translated literally and cleanly (the boring, good case)
    Rule,         // a named fix/rule fired (before -> after)
};
inline constexpr std::size_t kCategoryCount = 10;

// HOW MUCH you should care. Drives the per-file risk score. Ordered most important first.
enum class Severity { High, Med, Low };
inline constexpr std::size_t kSeverityCount = 3;

std::string_view name(Category c);   // "ERROR", "GUESSED", ... (what reports print)
std::string_view name(Severity s);   // "HIGH", "MED", "LOW"
std::optional<Category> parse_category(std::string_view text);
std::optional<Severity> parse_severity(std::string_view text);

// Every category, in the order above.
const std::array<Category, kCategoryCount>& all_categories();

// The weight of a severity in a file's risk score: High 5, Med 2, Low 0.
int weight(Severity s);

// The severity an event of this category gets unless the producer says otherwise.
Severity default_severity(Category c);

// One thing that happened to one place in the code.
struct Event {
    std::string file;
    std::uint32_t line = 0;
    Category category = Category::Annotated;
    Severity severity = Severity::Low;
    std::string source;    // which analyzer/stage produced this ("marker", "lint", ...)
    std::string rule;      // rule id / error code / marker keyword
    std::string message;
    std::string snippet;   // the offending source line, stripped
};

// What an event carries beyond its file, category and message.
struct EventDetail {
    std::uint32_t line = 0;
    std::optional<Severity> severity;   // default: the category's
    std::string source;
    std::string rule;
    std::string snippet;
};

using CategoryCounts = std::array<std::size_t, kCategoryCount>;   // indexed by Category
using SeverityCounts = std::array<std::size_t, kSeverityCount>;   // indexed by Severity

struct FileRisk {
    std::string file;
    int risk = 0;
    CategoryCounts categories{};
    SeverityCounts severities{};
};

// A queryable collection of Events, plus the roll-ups the reports need.
class Ledger {
public:
    // -- recording -----------------------------------------------------------
    void add(std::string file, Category category, std::string message, EventDetail detail = {});
    void add(Event event) { events_.push_back(std::move(event)); }
    void extend(const std::vector<Event>& events);

    std::size_t size() const { return events_.size(); }
    const std::vector<Event>& events() const { return events_; }

    // -- queries -------------------------------------------------------------
    // The files that have events, sorted.
    std::vector<std::string> files() const;
    // The events of each file, ordered by (line, severity) with the producer's order kept among equals.
    std::map<std::string, std::vector<const Event*>> by_file() const;
    // The same grouping as indices into events() (for callers that hold the events elsewhere).
    std::map<std::string, std::vector<std::size_t>> by_file_index() const;
    CategoryCounts totals_by_category() const;
    SeverityCounts totals_by_severity() const;
    // Higher = inspect sooner. Weighted by severity.
    static int risk_score(const std::vector<const Event*>& file_events);
    // One row per file with events, worst first (ties by file name).
    std::vector<FileRisk> file_risk_table() const;

    // -- serialisation -------------------------------------------------------
    // {"events": [...], "totals_by_category": {...}, "totals_by_severity": {...}} as indented JSON; every category
    // and severity appears in the totals, zero or not.
    std::string to_json() const;

private:
    std::vector<Event> events_;
};

}  // namespace cb::java::ledger

