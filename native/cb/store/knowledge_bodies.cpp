// cb/store/knowledge_bodies.cpp -- see knowledge_bodies.hpp.
#include "cb/store/knowledge_bodies.hpp"

#include <vector>

namespace cb::store {

void KnowledgeBodies::set_bodies(const ModuleRef& module, const std::map<std::string, Body>& by_key) {
    if (!module.sha.empty()) store_.add_module(module.sha, module.name, module.kind);
    std::vector<knowledge::BodyRow> rows;
    rows.reserve(by_key.size());
    for (const auto& [key, body] : by_key) rows.push_back({key, body.digest, body.name, body.source});
    store_.set_bodies(module.sha, rows);
}

std::map<std::string, Body> KnowledgeBodies::bodies(std::string_view module) {
    std::map<std::string, Body> out;
    for (knowledge::BodyRow& row : store_.bodies(module)) out[row.addr] = Body{std::move(row.digest), std::move(row.name), std::move(row.source)};
    return out;
}

void KnowledgeBodies::name_body(std::string_view digest, std::string_view name, std::string_view source) {
    store_.name_body(digest, name, source);
}

std::map<std::string, SharedName> KnowledgeBodies::names_from_bodies(std::string_view module) {
    std::map<std::string, SharedName> out;
    for (auto& [key, name] : store_.names_from_bodies(module)) out[key] = SharedName{std::move(name.name), std::move(name.source)};
    return out;
}

BodyCounts KnowledgeBodies::shared_bodies() {
    const knowledge::show::SharedBodies shared = store_.shared_bodies();
    return BodyCounts{.recorded = shared.recorded, .distinct = shared.distinct, .in_several = shared.in_several_modules, .copies = shared.copies};
}

}  // namespace cb::store
