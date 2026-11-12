// cb/translate/frontends/python/infer_util.hpp -- the small functions the files of the inference share: types made from
// a name or a container site, constant readers, the slice of a tuple type.
#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/translate/frontends/python/infer.hpp"

namespace cb::translate::frontends::python::infer {

// A type of a kind that is told apart by a name: inst, cls, fn, module, extern.
inline Type named_type(Kind kind, std::string name) { return Type{.kind = kind, .name = std::move(name)}; }
inline Type inst_type(std::string class_name) { return named_type(Kind::Inst, std::move(class_name)); }
// A list, set or dict of a container site.
inline Type site_type(Kind kind, program::SiteId site) { return Type{.kind = kind, .site = site}; }

// "int", "list" ...: a type's kind, for messages.
inline std::string kind_text(const Type& t) { return std::string(P::kind_word(t.kind)); }

// The part of a qualified name after its last dot.
inline std::string last_part(std::string_view qualified) {
    const std::size_t dot = qualified.rfind('.');
    return std::string(dot == std::string_view::npos ? qualified : qualified.substr(dot + 1));
}

// `name` up to its first dot (the library an `extern` name is of).
inline std::string first_part(std::string_view dotted) { return std::string(dotted.substr(0, dotted.find('.'))); }

inline bool is_name(const syntax::Expr* e, std::string_view id) {
    const auto* n = syntax::as<syntax::Name>(e);
    return n != nullptr && n->id == id;
}

inline bool word_in(std::string_view word, std::initializer_list<std::string_view> words) {
    return std::ranges::find(words, word) != words.end();
}

// The integer a literal (or a negated literal) is.
std::optional<Wide> int_constant(const syntax::Expr* e);

// args[a:b] of a tuple's element types, for constant bounds (as in Python: a negative bound counts from the end, a bound
// past either end stops there).
std::vector<Type> tuple_slice(const std::vector<Type>& items, const std::optional<Wide>& from, const std::optional<Wide>& to);

// An integer's text in a message: "None" for an unbounded end.
inline std::string bound_text(const Bound& b) { return b ? b->to_string() : std::string("None"); }

}  // namespace cb::translate::frontends::python::infer
