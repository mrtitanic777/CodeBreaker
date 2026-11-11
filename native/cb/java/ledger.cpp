#include "cb/java/ledger.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>

#include "cb/core/json.hpp"

namespace cb::java::ledger {

std::string_view name(Category c) {
    switch (c) {
        case Category::Error: return "ERROR";
        case Category::Guessed: return "GUESSED";
        case Category::Stubbed: return "STUBBED";
        case Category::Blocked: return "BLOCKED";
        case Category::Risk: return "RISK";
        case Category::Warning: return "WARNING";
        case Category::Overridden: return "OVERRIDDEN";
        case Category::Annotated: return "ANNOTATED";
        case Category::Ported: return "PORTED";
        case Category::Rule: return "RULE";
    }
    return "ANNOTATED";
}

std::string_view name(Severity s) {
    switch (s) {
        case Severity::High: return "HIGH";
        case Severity::Med: return "MED";
        case Severity::Low: return "LOW";
    }
    return "LOW";
}

const std::array<Category, kCategoryCount>& all_categories() {
    static const std::array<Category, kCategoryCount> kAll = {
        Category::Error,      Category::Guessed,   Category::Stubbed,    Category::Blocked, Category::Risk,
        Category::Warning,    Category::Overridden, Category::Annotated, Category::Ported,  Category::Rule};
    return kAll;
}

std::optional<Category> parse_category(std::string_view text) {
    for (Category c : all_categories()) {
        if (name(c) == text) return c;
    }
    return std::nullopt;
}

std::optional<Severity> parse_severity(std::string_view text) {
    for (Severity s : {Severity::High, Severity::Med, Severity::Low}) {
        if (name(s) == text) return s;
    }
    return std::nullopt;
}

int weight(Severity s) {
    switch (s) {
        case Severity::High: return 5;
        case Severity::Med: return 2;
        case Severity::Low: return 0;
    }
    return 0;
}

Severity default_severity(Category c) {
    switch (c) {
        case Category::Error:
        case Category::Guessed:
        case Category::Stubbed:
        case Category::Blocked: return Severity::High;
        case Category::Risk:
        case Category::Warning:
        case Category::Overridden: return Severity::Med;
        case Category::Annotated:
        case Category::Ported:
        case Category::Rule: return Severity::Low;
    }
    return Severity::Low;
}

// ---- Ledger -------------------------------------------------------------------------------------------

void Ledger::add(std::string file, Category category, std::string message, EventDetail detail) {
    Event e;
    e.file = std::move(file);
    e.line = detail.line;
    e.category = category;
    e.severity = detail.severity.value_or(default_severity(category));
    e.source = std::move(detail.source);
    e.rule = std::move(detail.rule);
    e.message = std::move(message);
    e.snippet = std::move(detail.snippet);
    events_.push_back(std::move(e));
}

void Ledger::extend(const std::vector<Event>& events) { events_.insert(events_.end(), events.begin(), events.end()); }

std::vector<std::string> Ledger::files() const {
    std::vector<std::string> out;
    for (const Event& e : events_) out.push_back(e.file);
    std::sort(out.begin(), out.end());
    out.erase(std::unique(out.begin(), out.end()), out.end());
    return out;
}

std::map<std::string, std::vector<std::size_t>> Ledger::by_file_index() const {
    std::map<std::string, std::vector<std::size_t>> out;
    for (std::size_t i = 0; i < events_.size(); ++i) out[events_[i].file].push_back(i);
    for (auto& [file, indices] : out) {
        std::stable_sort(indices.begin(), indices.end(), [this](std::size_t a, std::size_t b) {
            if (events_[a].line != events_[b].line) return events_[a].line < events_[b].line;
            return events_[a].severity < events_[b].severity;
        });
    }
    return out;
}

std::map<std::string, std::vector<const Event*>> Ledger::by_file() const {
    std::map<std::string, std::vector<const Event*>> out;
    for (const auto& [file, indices] : by_file_index()) {
        for (std::size_t i : indices) out[file].push_back(&events_[i]);
    }
    return out;
}
CategoryCounts Ledger::totals_by_category() const {
    CategoryCounts out{};
    for (const Event& e : events_) ++out[static_cast<std::size_t>(e.category)];
    return out;
}

SeverityCounts Ledger::totals_by_severity() const {
    SeverityCounts out{};
    for (const Event& e : events_) ++out[static_cast<std::size_t>(e.severity)];
    return out;
}

int Ledger::risk_score(const std::vector<const Event*>& file_events) {
    int score = 0;
    for (const Event* e : file_events) score += weight(e->severity);
    return score;
}

std::vector<FileRisk> Ledger::file_risk_table() const {
    std::vector<FileRisk> rows;
    for (const auto& [file, events] : by_file()) {
        FileRisk row;
        row.file = file;
        row.risk = risk_score(events);
        for (const Event* e : events) {
            ++row.categories[static_cast<std::size_t>(e->category)];
            ++row.severities[static_cast<std::size_t>(e->severity)];
        }
        rows.push_back(std::move(row));
    }
    std::stable_sort(rows.begin(), rows.end(), [](const FileRisk& a, const FileRisk& b) {
        if (a.risk != b.risk) return a.risk > b.risk;
        return a.file < b.file;
    });
    return rows;
}

std::string Ledger::to_json() const {
    Json doc = Json::object();
    Json events = Json::array();
    for (const Event& e : events_) {
        Json j = Json::object();
        j["file"] = e.file;
        j["line"] = e.line;
        j["category"] = std::string(name(e.category));
        j["severity"] = std::string(name(e.severity));
        j["source"] = e.source;
        j["rule"] = e.rule;
        j["message"] = e.message;
        j["snippet"] = e.snippet;
        events.push_back(std::move(j));
    }
    doc["events"] = std::move(events);

    Json by_category = Json::object();
    const CategoryCounts cats = totals_by_category();
    for (Category c : all_categories()) by_category[std::string(name(c))] = cats[static_cast<std::size_t>(c)];
    doc["totals_by_category"] = std::move(by_category);

    Json by_severity = Json::object();
    const SeverityCounts sevs = totals_by_severity();
    for (Severity s : {Severity::High, Severity::Med, Severity::Low}) {
        by_severity[std::string(name(s))] = sevs[static_cast<std::size_t>(s)];
    }
    doc["totals_by_severity"] = std::move(by_severity);
    return json::dump(doc, json::Style::Pretty);
}

}  // namespace cb::java::ledger

