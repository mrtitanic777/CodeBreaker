// translate/backends/cpp03_internal.hpp -- what the modules of the C++03 back end share and nobody else needs.
#pragma once

#include <functional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_set>

#include "translate/backends/cpp03.hpp"

namespace cb::translate::backends::cpp03::detail {

// ---- reading the tree
// The statement or expression a slot holds. A bare list of statements is not something the Java front end makes: a Decline.
const ir::Node* single(const ir::Suite& suite);
// The type of an expression (a Local's declared type for a Local); empty for a node that has none.
ir::TypeP type_of(const ir::Node* node);
// The kind of an expression's type ("i32", "class", ...); a Decline for an untyped expression.
std::string_view kind_of(const ir::Node& node);
std::string_view kind_of(const ir::TypeP& type);
// The class type an expression or type has, or null.
const ir::ClassType* class_type(const ir::TypeP& type);
// The symbol of a node, which must have one.
const ir::Sym& need_sym(const ir::SymP& sym, std::string_view what);
// The statements of a Block node; a Decline for anything else.
const std::vector<ir::Node*>& block_stmts(const ir::Node* block);

template <class F> void for_each_node(const ir::Node* root, F&& f) {   // pre-order, source order; null: nothing
    if (root == nullptr) return;
    f(root);
    for (const ir::Node* child : ir::children(*root)) for_each_node(child, f);
}
template <class P> bool any_node(const ir::Node* root, P&& pred) {
    if (root == nullptr) return false;
    if (pred(root)) return true;
    for (const ir::Node* child : ir::children(*root)) {
        if (any_node(child, pred)) return true;
    }
    return false;
}

// ---- text
// Whether `haystack` holds `needle` (the standard library has this from C++23).
inline bool has(std::string_view haystack, std::string_view needle) { return haystack.find(needle) != std::string_view::npos; }
// `s.rsplit('.', 1)[-1]`
std::string_view after_dot(std::string_view s);
std::string join_commas(const std::vector<std::string>& parts);
// A C++ type without its leading `const` and its trailing `&`.
std::string strip_const_ref(std::string_view type);
bool starts_with_any_collection(std::string_view ctype);   // JList< JMap< JSet<
// A name or member chain with no grouping: `a`, `this->x.y`.
bool is_simple_chain(std::string_view code);
std::string format_value(const ir::LitValue& value);       // for a Decline's message
std::string percent_d(std::int64_t value);
// The code points of UTF-8 text (a byte that is no valid UTF-8 reads as itself).
std::vector<std::uint32_t> code_points(std::string_view s);

// ---- tables (each built once)
const std::unordered_set<std::string_view>& cpp_keywords();
const std::map<std::string, std::string, std::less<>>& math_functions();
const std::set<std::string, std::less<>>& bit_operators();
const std::map<std::string, std::string, std::less<>>& saturating_casts();
const std::set<std::string, std::less<>>& reflection_types();
const std::set<std::string, std::less<>>& reflective_methods();
const std::map<std::string, std::string, std::less<>>& boxed_tests();
const std::map<std::string, std::string, std::less<>>& class_literal_types();
const std::set<std::string, std::less<>>& npe_catchers();
const std::set<std::string, std::less<>>& pure_owners();

}  // namespace cb::translate::backends::cpp03::detail
