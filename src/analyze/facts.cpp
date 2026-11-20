#include "analyze/facts.hpp"

#include <algorithm>
#include <chrono>
#include <format>

#include <nlohmann/json.hpp>

#include "analyze/image.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"

namespace cb::analyze::facts {

namespace {

namespace sqlite = base::sqlite;
using sqlite::Database;
using knowledge::Value;

constexpr const char* kSchema[] = {
    "CREATE TABLE IF NOT EXISTS fact("
    "module TEXT NOT NULL, analysis TEXT NOT NULL, addr TEXT NOT NULL DEFAULT '', kind TEXT NOT NULL, "
    "value TEXT, ts REAL)",
    "CREATE INDEX IF NOT EXISTS fact_module ON fact(module, analysis)",
    "CREATE INDEX IF NOT EXISTS fact_addr ON fact(module, addr)",
    "CREATE INDEX IF NOT EXISTS fact_kind ON fact(module, kind)",
};

// The database waits this long for another connection's lock: several analyses write to the store at once.
constexpr auto kBusyTimeout = std::chrono::seconds(60);

// A value as a JSON document whose object keys are sorted.
Json to_json_sorted(const Value& value) {
    switch (value.kind()) {
        case Value::Kind::Null: return Json(nullptr);
        case Value::Kind::Bool: return Json(*value.boolean());
        case Value::Kind::Int: return Json(*value.integer());
        case Value::Kind::Double: return Json(*value.number());
        case Value::Kind::Text: return Json(*value.text());
        case Value::Kind::List: {
            Json out = Json::array();
            for (const Value& item : *value.list()) out.push_back(to_json_sorted(item));
            return out;
        }
        case Value::Kind::Map: {
            std::vector<const Value::Map::value_type*> entries;
            for (const auto& entry : *value.map()) entries.push_back(&entry);
            std::sort(entries.begin(), entries.end(), [](const auto* a, const auto* b) { return a->first < b->first; });
            Json out = Json::object();
            for (const auto* entry : entries) out[entry->first] = to_json_sorted(entry->second);
            return out;
        }
    }
    return Json(nullptr);
}

std::string text_of(const sqlite::Value& cell) {
    const std::string* s = std::get_if<std::string>(&cell);
    return s != nullptr ? *s : std::string();
}

}  // namespace

std::string key(ImageKind kind, std::optional<Va> va) {
    if (!va) return {};
    if (kind == ImageKind::Ne) {
        // (segment, offset) as the decompiler lays a 16-bit module out: selector = kFirstSelector + 8 * (segment - 1)
        const std::int64_t segment = ((*va >> 16) - image::kFirstSelector) / 8 + 1;
        return std::format("{}:{:04x}", segment, *va & 0xFFFF);
    }
    return std::format("{:x}", *va);
}

std::filesystem::path default_path(const config::Config& config) {
    if (auto env = fs::get_env("CB_FUNCTIONS_DB"); env && !env->empty()) return fs::from_text(*env);
    return config.state("functions.sqlite");
}

FactStore FactStore::open(const std::filesystem::path& file) {
    Database db = Database::open(file, kBusyTimeout);
    db.exec("PRAGMA journal_mode=WAL");
    db.exec("PRAGMA synchronous=NORMAL");
    for (const char* statement : kSchema) db.exec(statement);
    return FactStore(std::move(db));
}

void FactStore::put(std::string_view module, std::string_view analysis, std::span<const Row> rows) {
    const double now = std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::vector<std::vector<sqlite::Value>> inserts;
    inserts.reserve(rows.size());
    for (const Row& row : rows)   // (rendered before the transaction opens: a value that cannot be written leaves the old facts)
        inserts.push_back({std::string(module), std::string(analysis), row.addr, row.kind, to_text(row.value), now});
    Database::Transaction transaction(db_);
    db_.exec("DELETE FROM fact WHERE module=? AND analysis=?", {std::string(module), std::string(analysis)});
    for (const auto& insert : inserts)
        db_.exec("INSERT INTO fact(module, analysis, addr, kind, value, ts) VALUES(?,?,?,?,?,?)", insert);
    transaction.commit();
}

std::vector<Fact> FactStore::get(std::string_view module, const Filter& filter) {
    std::string query = "SELECT analysis, addr, kind, value FROM fact WHERE module=?";
    std::vector<sqlite::Value> args{std::string(module)};
    const std::pair<const char*, const std::optional<std::string>*> narrowing[] = {
        {"analysis", &filter.analysis}, {"addr", &filter.addr}, {"kind", &filter.kind}};
    for (const auto& [column, wanted] : narrowing) {
        if (!wanted->has_value()) continue;
        query += std::format(" AND {}=?", column);
        args.push_back(**wanted);
    }
    query += " ORDER BY rowid";
    std::vector<Fact> out;
    for (const base::sqlite::Row& row : db_.query(query, args))
        out.push_back({text_of(row[0]), text_of(row[1]), text_of(row[2]), from_text(text_of(row[3]))});
    return out;
}

Tally FactStore::tally(std::string_view module) {
    Tally out;
    for (const base::sqlite::Row& row :
         db_.query("SELECT analysis, kind, COUNT(*) FROM fact WHERE module=? GROUP BY analysis, kind", {std::string(module)}))
        out[text_of(row[0])][text_of(row[1])] = std::get<std::int64_t>(row[2]);
    return out;
}

std::string to_text(const Value& value) { return json::dump(to_json_sorted(value), json::Style::PythonCompatible); }

Value from_text(std::string_view text) { return Value::from_json_text(text, "fact value"); }

}  // namespace cb::analyze::facts
