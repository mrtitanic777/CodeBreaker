#include "cb/decompile/l2tree.hpp"

#include <algorithm>

#include <nlohmann/json.hpp>

namespace cb::decompile::l2tree {

const Json* member(const Json* node, std::string_view key) {
    if (!node || !node->is_object()) return nullptr;
    auto it = node->find(std::string(key));
    if (it == node->end() || it->is_null()) return nullptr;
    return &*it;
}

Json* member(Json* node, std::string_view key) {
    return const_cast<Json*>(member(static_cast<const Json*>(node), key));
}

std::string_view text(const Json* node, std::string_view key) {
    const Json* m = member(node, key);
    if (!m || !m->is_string()) return {};
    return m->get_ref<const std::string&>();
}

std::optional<std::int64_t> integer(const Json* node, std::string_view key) {
    const Json* m = member(node, key);
    if (!m || !m->is_number_integer()) return std::nullopt;
    return m->get<std::int64_t>();
}

bool is_kind(const Json* node, std::string_view kind) { return node && node->is_object() && text(node, "node") == kind; }

bool present(const Json* node) {
    if (!node || node->is_null()) return false;
    if (node->is_boolean()) return node->get<bool>();
    if (node->is_number_integer()) return node->get<std::int64_t>() != 0;
    if (node->is_number_float()) return node->get<double>() != 0.0;
    if (node->is_string()) return !node->get_ref<const std::string&>().empty();
    return !node->empty();
}

void walk(Json& root, const std::function<void(Json&)>& f) {
    if (root.is_object()) {
        f(root);
        if (!root.is_object()) return;   // (f may have replaced the node by something else)
        for (auto& entry : root.items()) walk(entry.value(), f);
    } else if (root.is_array()) {
        for (std::size_t i = 0; i < root.size(); ++i) walk(root[i], f);
    }
}

void walk(const Json& root, const std::function<void(const Json&)>& f) {
    if (root.is_object()) {
        f(root);
        for (const auto& entry : root.items()) walk(entry.value(), f);
    } else if (root.is_array()) {
        for (const Json& x : root) walk(x, f);
    }
}

std::vector<const Json*> nodes(const Json& root) {
    std::vector<const Json*> out;
    walk(root, [&](const Json& n) { out.push_back(&n); });
    return out;
}

namespace {

bool is_statement_list_key(const std::string& key) {
    return key == "body" || key == "then" || key == "orelse" || key == "stmts";
}

void lists_in(Json& n, const std::function<void(Json&)>& f) {
    if (n.is_object()) {
        for (auto& entry : n.items()) {
            if (entry.value().is_array() && is_statement_list_key(entry.key())) f(entry.value());
            lists_in(entry.value(), f);
        }
    } else if (n.is_array()) {
        for (std::size_t i = 0; i < n.size(); ++i) lists_in(n[i], f);
    }
}

}  // namespace

void for_each_statement_list(Json& root, const std::function<void(Json&)>& f) { lists_in(root, f); }

std::vector<std::uint32_t> origin_offsets(const Json& stmt, bool keep_repeats) {
    std::vector<std::uint32_t> out;
    const Json* origin = member(&stmt, "origin");
    const Json* at = member(origin, "at");
    if (at && at->is_array()) {
        for (const Json& a : *at) {
            if (a.is_number_integer()) out.push_back(static_cast<std::uint32_t>(a.get<std::int64_t>() & 0xFFFF));
        }
    }
    std::sort(out.begin(), out.end());
    if (!keep_repeats) out.erase(std::unique(out.begin(), out.end()), out.end());
    return out;
}

}  // namespace cb::decompile::l2tree
