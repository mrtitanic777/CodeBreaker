// codebreaker/java/ledger.hpp -- typed C++ port of codebreaker.java.ledger.
//
// ledger.py -- the structured record of everything the converter does to a file.
//
// The whole point of the restructure is that the converter must TELL YOU what it
// did. Every observable action becomes an Event with a Category, a Severity, a
// location (file + line), and a human message. A Ledger is just a queryable bag of
// Events plus the roll-ups the reports need (per-file grouping, risk scoring,
// totals).
//
// Nothing here knows *how* events are produced -- analyzers (post-hoc, Phase 1) and
// eventually the generator/rules (instrumented, later phases) both just call
// ledger.add(...). That separation is the contract that lets us swap the monolith
// out underneath the reporting without touching the reports.
//
// Events are shared objects in Python (extend() puts the very same Event in two ledgers), so a
// ledger holds them as py::ref<Event>.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::java::ledger {

// ---------------------------------------------------------------------------
// Categories -- WHAT kind of thing happened to the code.
// Ordered roughly by how much it should worry you, worst first.
// ---------------------------------------------------------------------------
struct Category {
    static const py::str ERROR;       // compiler said no (from a build log) -- definitely broken
    static const py::str GUESSED;     // converter made a heuristic call that could be wrong (value/pointer,
                                      //   inferred element type) -- THE category most likely to be a latent bug
    static const py::str STUBBED;     // replaced with a placeholder; needs a hand implementation
    static const py::str BLOCKED;     // explicitly could-not-port (PORT-BLOCKED)
    static const py::str RISK;        // matches a known crash/leak pattern (from converter_lint)
    static const py::str WARNING;     // suspicious; verify by hand (PORT-WARNING, TODO/XXX/FIXME)
    static const py::str OVERRIDDEN;  // a hand-written file replaced generated output for this class
    static const py::str ANNOTATED;   // an explanatory note about a decision already taken
    static const py::str PORTED;      // translated literally and cleanly (the boring, good case)
    static const py::str RULE;        // a named fix/rule fired (before -> after)

    static const py::list<py::str> ALL;
};

// ---------------------------------------------------------------------------
// Severities -- HOW MUCH you should care. Drives the per-file risk score.
// ---------------------------------------------------------------------------
struct Severity {
    static const py::str HIGH;
    static const py::str MED;
    static const py::str LOW;

    static const py::dict<py::str, int64_t> WEIGHT;
    static const py::dict<py::str, int64_t> ORDER;
};

// Default severity for each category (an analyzer may override per-event).
extern const py::dict<py::str, py::str> _DEFAULT_SEVERITY;

// Event.as_dict(): a dict with these fixed keys, in this order.
struct AsDict {
    py::str file;
    int64_t line = 0;
    py::str category;
    py::str severity;
    py::str source;
    py::str rule;
    py::str message;
    py::str snippet;
};
template <class F> void for_each_field(const AsDict& r, F&& f) {
    f("file", r.file);
    f("line", r.line);
    f("category", r.category);
    f("severity", r.severity);
    f("source", r.source);
    f("rule", r.rule);
    f("message", r.message);
    f("snippet", r.snippet);
}
template <class F> void for_each_field_mut(AsDict& r, F&& f) {
    f("file", r.file, true);
    f("line", r.line, true);
    f("category", r.category, true);
    f("severity", r.severity, true);
    f("source", r.source, true);
    f("rule", r.rule, true);
    f("message", r.message, true);
    f("snippet", r.snippet, true);
}

// One thing that happened to one place in the code.
class Event : public py::object {
public:
    py::str file;
    int64_t line = 0;
    py::str category;
    py::str severity;
    py::str source;    // which analyzer/stage produced this ('marker', 'lint', ...)
    py::str rule;      // rule id / error code / marker keyword
    py::str message;
    py::str snippet;   // the offending source line, stripped

    // (the Python's line=0 / severity=None / source='' / rule='' / snippet='' defaults)
    Event(const py::str& file, const py::str& category, const py::str& message, int64_t line = 0,
          const std::optional<py::str>& severity = std::nullopt, const py::str& source = py::str(""),
          const py::str& rule = py::str(""), const py::str& snippet = py::str(""));

    AsDict as_dict() const;
};

// [(file, risk, {category: count}, {severity: count}), ...]
using FileRisk = std::tuple<py::str, int64_t, py::dict<py::str, int64_t>, py::dict<py::str, int64_t>>;

// A queryable collection of Events, plus the roll-ups the reports need.
class Ledger : public py::object {
public:
    py::list<py::ref<Event>> events;

    Ledger();

    // -- recording -----------------------------------------------------------
    void add(const py::str& file, const py::str& category, const py::str& message, int64_t line = 0,
             const std::optional<py::str>& severity = std::nullopt, const py::str& source = py::str(""),
             const py::str& rule = py::str(""), const py::str& snippet = py::str(""));
    void extend(const py::list<py::ref<Event>>& more);
    int64_t size() const;   // len(ledger)

    // -- queries -------------------------------------------------------------
    py::list<py::str> files() const;
    py::dict<py::str, py::list<py::ref<Event>>> by_file() const;
    py::dict<py::str, int64_t> totals_by_category() const;
    py::dict<py::str, int64_t> totals_by_severity() const;
    // Higher = inspect sooner. Weighted by severity.
    int64_t risk_score(const py::list<py::ref<Event>>& file_events) const;
    // [(file, risk, {category: count}, {severity: count}), ...] sorted worst-first.
    py::list<FileRisk> file_risk_table() const;

    // -- serialization -------------------------------------------------------
    py::str to_json() const;   // json.dumps(..., indent=2)
};

}  // namespace codebreaker::java::ledger
