// cb/symbols/body_store.cpp -- see body_store.hpp.
#include "cb/symbols/body_store.hpp"

#include <vector>

#include "cb/base/sqlite.hpp"

namespace cb::symbols {

void KnowledgeBodies::set_bodies(const store::ModuleRef& module, const std::map<std::string, store::Body>& by_key) {
    std::vector<knowledge::BodyRow> rows;
    rows.reserve(by_key.size());
    for (const auto& [key, body] : by_key) rows.push_back({key, body.digest, body.name, body.source});
    knowledge_.add_module(module.sha, module.name, module.kind);
    knowledge_.set_bodies(module.sha, rows);
}

std::map<std::string, store::Body> KnowledgeBodies::bodies(std::string_view module) {
    std::map<std::string, store::Body> out;
    for (knowledge::BodyRow& row : knowledge_.bodies(module)) out[row.addr] = store::Body{std::move(row.digest), std::move(row.name), std::move(row.source)};
    return out;
}

void KnowledgeBodies::name_body(std::string_view digest, std::string_view name, std::string_view source) {
    knowledge_.name_body(digest, name, source);
}

std::map<std::string, store::SharedName> KnowledgeBodies::names_from_bodies(std::string_view module) {
    std::map<std::string, store::SharedName> out;
    for (auto& [addr, name] : knowledge_.names_from_bodies(module)) out[addr] = store::SharedName{std::move(name.name), std::move(name.source)};
    return out;
}

store::BodyCounts KnowledgeBodies::shared_bodies() {
    const knowledge::show::SharedBodies counts = knowledge_.shared_bodies();
    store::BodyCounts out;
    out.recorded = counts.recorded;
    out.distinct = counts.distinct;
    out.in_several = counts.in_several_modules;
    namespace sql = base::sqlite;
    sql::Database db = sql::Database::open(knowledge_.file());
    const std::vector<sql::Row> rows = db.query("SELECT COALESCE(SUM(n), 0) FROM (SELECT COUNT(*) n FROM body GROUP BY hash HAVING COUNT(DISTINCT module) > 1)");
    if (!rows.empty())
        if (const auto* n = std::get_if<std::int64_t>(&rows[0][0])) out.copies = *n;
    return out;
}

}  // namespace cb::symbols
