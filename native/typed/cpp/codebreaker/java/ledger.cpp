// codebreaker/java/ledger.cpp -- typed C++ port of codebreaker.java.ledger (see the header).

#include "codebreaker/java/ledger.hpp"

#include <algorithm>
#include <cstdio>
#include <string>

namespace codebreaker::java::ledger {

const py::str Category::ERROR = "ERROR";
const py::str Category::GUESSED = "GUESSED";
const py::str Category::STUBBED = "STUBBED";
const py::str Category::BLOCKED = "BLOCKED";
const py::str Category::RISK = "RISK";
const py::str Category::WARNING = "WARNING";
const py::str Category::OVERRIDDEN = "OVERRIDDEN";
const py::str Category::ANNOTATED = "ANNOTATED";
const py::str Category::PORTED = "PORTED";
const py::str Category::RULE = "RULE";
const py::list<py::str> Category::ALL = {Category::ERROR,      Category::GUESSED,   Category::STUBBED, Category::BLOCKED,
                                         Category::RISK,       Category::WARNING,   Category::OVERRIDDEN,
                                         Category::ANNOTATED,  Category::PORTED,    Category::RULE};

const py::str Severity::HIGH = "HIGH";
const py::str Severity::MED = "MED";
const py::str Severity::LOW = "LOW";
const py::dict<py::str, int64_t> Severity::WEIGHT = {{py::str("HIGH"), 5}, {py::str("MED"), 2}, {py::str("LOW"), 0}};
const py::dict<py::str, int64_t> Severity::ORDER = {{py::str("HIGH"), 0}, {py::str("MED"), 1}, {py::str("LOW"), 2}};

const py::dict<py::str, py::str> _DEFAULT_SEVERITY = {
    {Category::ERROR, Severity::HIGH},      {Category::GUESSED, Severity::HIGH}, {Category::STUBBED, Severity::HIGH},
    {Category::BLOCKED, Severity::HIGH},    {Category::RISK, Severity::MED},     {Category::WARNING, Severity::MED},
    {Category::OVERRIDDEN, Severity::MED},  {Category::ANNOTATED, Severity::LOW}, {Category::PORTED, Severity::LOW},
    {Category::RULE, Severity::LOW},
};

Event::Event(const py::str& file, const py::str& category, const py::str& message, int64_t line,
             const std::optional<py::str>& severity, const py::str& source, const py::str& rule,
             const py::str& snippet)
    : file(file),
      line(line),   // int(line) if line else 0
      category(category),
      // severity or _DEFAULT_SEVERITY.get(category, Severity.LOW)   ('' counts as not given)
      severity(severity && py::truthy(*severity) ? *severity : _DEFAULT_SEVERITY.get(category, Severity::LOW)),
      source(source),
      rule(rule),
      message(message),
      snippet(snippet) {}

AsDict Event::as_dict() const {
    return AsDict{file, line, category, severity, source, rule, message, snippet};
}

Ledger::Ledger() {}

void Ledger::add(const py::str& file, const py::str& category, const py::str& message, int64_t line,
                 const std::optional<py::str>& severity, const py::str& source, const py::str& rule,
                 const py::str& snippet) {
    events.append(std::make_shared<Event>(file, category, message, line, severity, source, rule, snippet));
}

void Ledger::extend(const py::list<py::ref<Event>>& more) {
    events.extend(more);
}

int64_t Ledger::size() const {
    return py::len(events);
}

py::list<py::str> Ledger::files() const {
    py::set<py::str> seen;
    for (const py::ref<Event>& e : events) {
        seen.add(e->file);
    }
    return py::sorted(seen);
}

py::dict<py::str, py::list<py::ref<Event>>> Ledger::by_file() const {
    py::dict<py::str, py::list<py::ref<Event>>> out;
    for (const py::ref<Event>& e : events) {
        out.setdefault(e->file, py::list<py::ref<Event>>()).append(e);
    }
    for (const py::list<py::ref<Event>>& evs : out.values()) {
        // evs.sort(key=lambda e: (e.line, Severity.ORDER.get(e.severity, 9)))
        evs.sort([](const py::ref<Event>& e) {
            return std::make_tuple(e->line, Severity::ORDER.get(e->severity, int64_t(9)));
        });
    }
    return out;
}

py::dict<py::str, int64_t> Ledger::totals_by_category() const {
    py::dict<py::str, int64_t> out;
    for (const py::str& c : Category::ALL) {
        out.set(c, 0);
    }
    for (const py::ref<Event>& e : events) {
        out.set(e->category, out.get(e->category, int64_t(0)) + 1);
    }
    return out;
}

py::dict<py::str, int64_t> Ledger::totals_by_severity() const {
    py::dict<py::str, int64_t> out = {{Severity::HIGH, 0}, {Severity::MED, 0}, {Severity::LOW, 0}};
    for (const py::ref<Event>& e : events) {
        out.set(e->severity, out.get(e->severity, int64_t(0)) + 1);
    }
    return out;
}

int64_t Ledger::risk_score(const py::list<py::ref<Event>>& file_events) const {
    int64_t total = 0;
    for (const py::ref<Event>& e : file_events) {
        total += Severity::WEIGHT.get(e->severity, int64_t(0));
    }
    return total;
}

py::list<FileRisk> Ledger::file_risk_table() const {
    py::list<FileRisk> rows;
    for (const auto& kv : by_file().items()) {
        const py::str& f = std::get<0>(kv);
        const py::list<py::ref<Event>>& evs = std::get<1>(kv);
        py::dict<py::str, int64_t> cats;
        py::dict<py::str, int64_t> sevs;
        for (const py::ref<Event>& e : evs) {
            cats.set(e->category, cats.get(e->category, int64_t(0)) + 1);
            sevs.set(e->severity, sevs.get(e->severity, int64_t(0)) + 1);
        }
        rows.append(FileRisk{f, risk_score(evs), cats, sevs});
    }
    // rows.sort(key=lambda r: (-r[1], r[0]))
    rows.sort([](const FileRisk& r) { return std::make_tuple(-std::get<1>(r), std::get<0>(r)); });
    return rows;
}

// -- json.dumps(..., indent=2), ensure_ascii: just the pieces to_json writes ----------------------
namespace {

void put_str(std::string& o, const py::str& s) {
    o += '"';
    for (char32_t c : s.u32()) {
        char b[16];
        if (c == '"') o += "\\\"";
        else if (c == '\\') o += "\\\\";
        else if (c == '\n') o += "\\n";
        else if (c == '\r') o += "\\r";
        else if (c == '\t') o += "\\t";
        else if (c == '\b') o += "\\b";
        else if (c == '\f') o += "\\f";
        else if (c < 0x20 || c == 0x7f) { std::snprintf(b, sizeof b, "\\u%04x", static_cast<unsigned>(c)); o += b; }
        else if (c < 0x7f) o += static_cast<char>(c);   // (json.dumps escapes everything outside space..~)
        else if (c < 0x10000) { std::snprintf(b, sizeof b, "\\u%04x", static_cast<unsigned>(c)); o += b; }
        else {
            unsigned v = static_cast<unsigned>(c) - 0x10000;
            std::snprintf(b, sizeof b, "\\u%04x\\u%04x", 0xD800 + (v >> 10), 0xDC00 + (v & 0x3FF));
            o += b;
        }
    }
    o += '"';
}

void put_counts(std::string& o, const py::dict<py::str, int64_t>& d, const std::string& ind) {
    if (d.empty()) {
        o += "{}";
        return;
    }
    o += "{";
    bool first = true;
    for (const auto& kv : d.items()) {
        o += first ? "\n" : ",\n";
        first = false;
        o += ind + "  ";
        put_str(o, std::get<0>(kv));
        o += ": " + std::to_string(std::get<1>(kv));
    }
    o += "\n" + ind + "}";
}

}  // namespace

py::str Ledger::to_json() const {
    std::string o = "{\n  \"events\": ";
    if (events.empty()) {
        o += "[]";
    } else {
        o += "[";
        bool first = true;
        for (const py::ref<Event>& e : events) {
            o += first ? "\n" : ",\n";
            first = false;
            AsDict d = e->as_dict();
            o += "    {\n";
            o += "      \"file\": ";     put_str(o, d.file);     o += ",\n";
            o += "      \"line\": " + std::to_string(d.line) + ",\n";
            o += "      \"category\": "; put_str(o, d.category); o += ",\n";
            o += "      \"severity\": "; put_str(o, d.severity); o += ",\n";
            o += "      \"source\": ";   put_str(o, d.source);   o += ",\n";
            o += "      \"rule\": ";     put_str(o, d.rule);     o += ",\n";
            o += "      \"message\": ";  put_str(o, d.message);  o += ",\n";
            o += "      \"snippet\": ";  put_str(o, d.snippet);  o += "\n";
            o += "    }";
        }
        o += "\n  ]";
    }
    o += ",\n  \"totals_by_category\": ";
    put_counts(o, totals_by_category(), "  ");
    o += ",\n  \"totals_by_severity\": ";
    put_counts(o, totals_by_severity(), "  ");
    o += "\n}";
    return py::str(o);
}

}  // namespace codebreaker::java::ledger
