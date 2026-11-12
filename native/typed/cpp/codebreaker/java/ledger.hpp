// codebreaker/java/ledger.hpp -- SHIM over cb/java/ledger.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
//
// The old Event/Ledger are shared, mutable objects (callers append events to `ledger->events` and read event
// fields directly), so the shim keeps them as data; every query goes through the new Ledger by conversion, and the
// results are mapped back to the old events.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/ledger.hpp"

namespace codebreaker::java::ledger {

namespace cbl = cb::java::ledger;

struct Category {
    static inline const py::str ERROR = "ERROR";
    static inline const py::str GUESSED = "GUESSED";
    static inline const py::str STUBBED = "STUBBED";
    static inline const py::str BLOCKED = "BLOCKED";
    static inline const py::str RISK = "RISK";
    static inline const py::str WARNING = "WARNING";
    static inline const py::str OVERRIDDEN = "OVERRIDDEN";
    static inline const py::str ANNOTATED = "ANNOTATED";
    static inline const py::str PORTED = "PORTED";
    static inline const py::str RULE = "RULE";

    static inline const py::list<py::str> ALL = {ERROR, GUESSED, STUBBED, BLOCKED, RISK,
                                                 WARNING, OVERRIDDEN, ANNOTATED, PORTED, RULE};
};

struct Severity {
    static inline const py::str HIGH = "HIGH";
    static inline const py::str MED = "MED";
    static inline const py::str LOW = "LOW";

    static inline const py::dict<py::str, int64_t> WEIGHT = {{py::str("HIGH"), 5}, {py::str("MED"), 2}, {py::str("LOW"), 0}};
    static inline const py::dict<py::str, int64_t> ORDER = {{py::str("HIGH"), 0}, {py::str("MED"), 1}, {py::str("LOW"), 2}};
};

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

namespace conv_ {
inline cbl::Category category(const py::str& s) {
    auto c = cbl::parse_category(cb::compat::from_py(s));
    if (!c) throw py::ValueError("not a ledger category: " + cb::compat::from_py(s));
    return *c;
}
inline cbl::Severity severity(const py::str& s) {
    auto v = cbl::parse_severity(cb::compat::from_py(s));
    if (!v) throw py::ValueError("not a ledger severity: " + cb::compat::from_py(s));
    return *v;
}
}  // namespace conv_

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

    // (the Python's line=0 / severity=None / source='' / rule='' / snippet='' defaults; '' severity counts as not given)
    Event(const py::str& file, const py::str& category, const py::str& message, int64_t line = 0,
          const std::optional<py::str>& severity = std::nullopt, const py::str& source = py::str(""),
          const py::str& rule = py::str(""), const py::str& snippet = py::str(""))
        : file(file), line(line), category(category),
          severity(severity && severity->size() > 0
                       ? *severity
                       : cb::compat::to_py(cbl::name(cbl::default_severity(conv_::category(category))))),
          source(source), rule(rule), message(message), snippet(snippet) {}

    AsDict as_dict() const { return AsDict{file, line, category, severity, source, rule, message, snippet}; }

    cbl::Event to_new() const {
        cbl::Event e;
        e.file = cb::compat::from_py(file);
        e.line = cb::compat::int_from_py<std::uint32_t>(line);
        e.category = conv_::category(category);
        e.severity = conv_::severity(severity);
        e.source = cb::compat::from_py(source);
        e.rule = cb::compat::from_py(rule);
        e.message = cb::compat::from_py(message);
        e.snippet = cb::compat::from_py(snippet);
        return e;
    }
};

using FileRisk = std::tuple<py::str, int64_t, py::dict<py::str, int64_t>, py::dict<py::str, int64_t>>;

class Ledger : public py::object {
public:
    py::list<py::ref<Event>> events;

    Ledger() {}

    // -- recording -----------------------------------------------------------
    void add(const py::str& file, const py::str& category, const py::str& message, int64_t line = 0,
             const std::optional<py::str>& severity = std::nullopt, const py::str& source = py::str(""),
             const py::str& rule = py::str(""), const py::str& snippet = py::str("")) {
        events.append(std::make_shared<Event>(file, category, message, line, severity, source, rule, snippet));
    }
    void extend(const py::list<py::ref<Event>>& more) { events.extend(more); }
    int64_t size() const { return events.size(); }   // len(ledger)

    // -- queries -------------------------------------------------------------
    py::list<py::str> files() const { return cb::compat::guard([&] { return cb::compat::to_py(to_new().files()); }); }

    py::dict<py::str, py::list<py::ref<Event>>> by_file() const {
        return cb::compat::guard([&] {
            py::dict<py::str, py::list<py::ref<Event>>> out;
            for (const auto& [file, indices] : to_new().by_file_index()) {
                py::list<py::ref<Event>> evs;
                for (std::size_t i : indices) evs.append(events[static_cast<int64_t>(i)]);
                out.set(cb::compat::to_py(file), evs);
            }
            return out;
        });
    }

    py::dict<py::str, int64_t> totals_by_category() const {
        return cb::compat::guard([&] {
            const cbl::CategoryCounts counts = to_new().totals_by_category();
            py::dict<py::str, int64_t> out;
            for (cbl::Category c : cbl::all_categories()) {
                out.set(cb::compat::to_py(cbl::name(c)), static_cast<int64_t>(counts[static_cast<std::size_t>(c)]));
            }
            return out;
        });
    }

    py::dict<py::str, int64_t> totals_by_severity() const {
        return cb::compat::guard([&] {
            const cbl::SeverityCounts counts = to_new().totals_by_severity();
            py::dict<py::str, int64_t> out;
            for (cbl::Severity s : {cbl::Severity::High, cbl::Severity::Med, cbl::Severity::Low}) {
                out.set(cb::compat::to_py(cbl::name(s)), static_cast<int64_t>(counts[static_cast<std::size_t>(s)]));
            }
            return out;
        });
    }

    // Higher = inspect sooner. Weighted by severity.
    int64_t risk_score(const py::list<py::ref<Event>>& file_events) const {
        return cb::compat::guard([&] {
            std::vector<cbl::Event> converted;
            for (const py::ref<Event>& e : file_events) converted.push_back(e->to_new());
            std::vector<const cbl::Event*> view;
            for (const cbl::Event& e : converted) view.push_back(&e);
            return static_cast<int64_t>(cbl::Ledger::risk_score(view));
        });
    }

    // [(file, risk, {category: count}, {severity: count}), ...] sorted worst-first. The count dicts hold the
    // categories/severities the file has, in category/severity order.
    py::list<FileRisk> file_risk_table() const {
        return cb::compat::guard([&] {
            py::list<FileRisk> rows;
            for (const cbl::FileRisk& r : to_new().file_risk_table()) {
                py::dict<py::str, int64_t> cats, sevs;
                for (cbl::Category c : cbl::all_categories()) {
                    if (r.categories[static_cast<std::size_t>(c)] != 0)
                        cats.set(cb::compat::to_py(cbl::name(c)), static_cast<int64_t>(r.categories[static_cast<std::size_t>(c)]));
                }
                for (cbl::Severity s : {cbl::Severity::High, cbl::Severity::Med, cbl::Severity::Low}) {
                    if (r.severities[static_cast<std::size_t>(s)] != 0)
                        sevs.set(cb::compat::to_py(cbl::name(s)), static_cast<int64_t>(r.severities[static_cast<std::size_t>(s)]));
                }
                rows.append(FileRisk{cb::compat::to_py(r.file), static_cast<int64_t>(r.risk), cats, sevs});
            }
            return rows;
        });
    }

    // -- serialization -------------------------------------------------------
    py::str to_json() const { return cb::compat::guard([&] { return cb::compat::to_py(to_new().to_json()); }); }

    cbl::Ledger to_new() const {
        cbl::Ledger out;
        for (const py::ref<Event>& e : events) out.add(e->to_new());
        return out;
    }
};

// The old event for a new one (the shims of the analyzers append what the new code finds to an old ledger).
inline py::ref<Event> from_new(const cbl::Event& e) {
    return std::make_shared<Event>(cb::compat::to_py(e.file), cb::compat::to_py(cbl::name(e.category)), cb::compat::to_py(e.message),
                                   static_cast<int64_t>(e.line), cb::compat::to_py(cbl::name(e.severity)), cb::compat::to_py(e.source),
                                   cb::compat::to_py(e.rule), cb::compat::to_py(e.snippet));
}

}  // namespace codebreaker::java::ledger
