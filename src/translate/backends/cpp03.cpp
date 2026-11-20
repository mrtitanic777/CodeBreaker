// translate/backends/cpp03.cpp -- the C++03 back end's context, its type bridge and the tables its writers share (see cpp03.hpp).

#include "translate/backends/cpp03.hpp"

#include <algorithm>

#include "core/text.hpp"
#include "java/emit/ownership.hpp"
#include "translate/backends/cpp03_internal.hpp"

namespace cb::translate::backends::cpp03 {

namespace ownership = cb::java::emit::ownership;
namespace typemap = cb::java::typemap;
using cb::java::types::Use;

namespace detail {

const ir::Node* single(const ir::Suite& suite) {
    if (std::holds_alternative<std::vector<ir::Node*>>(suite)) throw Decline("statement list");
    return std::get<ir::Node*>(suite);
}

ir::TypeP type_of(const ir::Node* node) {
    if (node == nullptr) return {};
    if (const auto* local = ir::get<ir::Local>(node)) return local->type;
    return node->type;
}

std::string_view kind_of(const ir::TypeP& type) {
    if (!type) throw Decline("untyped expression");
    return type->kind();
}
std::string_view kind_of(const ir::Node& node) { return kind_of(type_of(&node)); }

const ir::ClassType* class_type(const ir::TypeP& type) {
    return type ? std::get_if<ir::ClassType>(&type->body) : nullptr;
}

const ir::Sym& need_sym(const ir::SymP& sym, std::string_view what) {
    if (!sym) throw Decline("unresolved " + std::string(what));
    return *sym;
}

const std::vector<ir::Node*>& block_stmts(const ir::Node* block) {
    const auto* b = ir::get<ir::Block>(block);
    if (b == nullptr) throw Decline("a body that is not a block");
    return b->stmts;
}

std::string_view after_dot(std::string_view s) {
    const std::size_t at = s.rfind('.');
    return at == std::string_view::npos ? s : s.substr(at + 1);
}

std::string join_commas(const std::vector<std::string>& parts) { return text::join(parts, ", "); }

std::string strip_const_ref(std::string_view type) {
    std::string_view t = type;
    if (t.starts_with("const") && t.size() > 5 && text::is_space(t[5])) t = text::ltrim(t.substr(5));
    while (t.ends_with('&')) t.remove_suffix(1);
    return std::string(text::trim(t));
}

bool starts_with_any_collection(std::string_view ctype) {
    return ctype.starts_with("JList<") || ctype.starts_with("JMap<") || ctype.starts_with("JSet<");
}

static bool word_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
static bool ident_start(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

bool is_simple_chain(std::string_view code) {
    std::size_t i = 0;
    while (true) {   // one identifier, then `->` or `.` and the next
        if (i >= code.size() || !ident_start(code[i])) return false;
        while (i < code.size() && word_char(code[i])) ++i;
        if (i == code.size()) return true;
        if (code.substr(i).starts_with("->")) i += 2;
        else if (code[i] == '.') i += 1;
        else return false;
    }
}

std::string format_value(const ir::LitValue& value) {
    return std::visit(
        [](const auto& x) -> std::string {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::monostate>) return "null";
            else if constexpr (std::is_same_v<T, bool>) return x ? "true" : "false";
            else if constexpr (std::is_same_v<T, std::string>) return "'" + x + "'";
            else if constexpr (std::is_same_v<T, double>) return text::format_double(x);
            else return std::to_string(x);
        },
        value);
}

std::string percent_d(std::int64_t value) { return std::to_string(value); }

const std::unordered_set<std::string_view>& cpp_keywords() {
    static const std::unordered_set<std::string_view> set = [] {
        std::unordered_set<std::string_view> s;
        for (std::string_view w : text::split_ws(
                 "alignas alignof and and_eq asm auto bitand bitor bool break case catch char char16_t char32_t class compl const "
                 "constexpr const_cast continue decltype default delete do double dynamic_cast else enum explicit export extern false "
                 "float for friend goto if inline int long mutable namespace new noexcept not not_eq nullptr operator or or_eq private "
                 "protected public register reinterpret_cast return short signed sizeof static static_assert static_cast struct switch "
                 "template this thread_local throw true try typedef typeid typename union unsigned using virtual void volatile wchar_t "
                 "while xor xor_eq NULL min max"))
            s.insert(w);
        return s;
    }();
    return set;
}

// java.lang.Math as C: the converter's table, but each function at Java's own precision (double), where the converter has the
// float forms (floorf, powf, ...) for the 360's float pipeline.
const std::map<std::string, std::string, std::less<>>& math_functions() {
    static const std::map<std::string, std::string, std::less<>> table = [] {
        std::map<std::string, std::string, std::less<>> m = typemap::math_api_emit();
        m["floor"] = "floor";
        m["ceil"] = "ceil";
        m["pow"] = "pow";
        m["exp"] = "exp";
        m["log"] = "log";
        m.erase("PI");
        return m;
    }();
    return table;
}

const std::set<std::string, std::less<>>& bit_operators() {
    static const std::set<std::string, std::less<>> s = {"<<", ">>", "&", "|", "^"};
    return s;
}
const std::map<std::string, std::string, std::less<>>& saturating_casts() {
    static const std::map<std::string, std::string, std::less<>> m = {{"int", "j2xToInt"}, {"long long", "j2xToLong"}};
    return m;
}
const std::set<std::string, std::less<>>& reflection_types() {
    static const std::set<std::string, std::less<>> s = {"Class*", "Constructor*"};   // left as written: the reflection pass rewrites them
    return s;
}
const std::set<std::string, std::less<>>& reflective_methods() {
    static const std::set<std::string, std::less<>> s = {"newInstance", "forName", "getConstructor", "getDeclaredConstructor"};
    return s;
}
const std::map<std::string, std::string, std::less<>>& boxed_tests() {
    static const std::map<std::string, std::string, std::less<>> m = {
        {"String", "isString"}, {"Integer", "isInteger"}, {"Character", "isCharacter"}, {"Boolean", "isBoolean"}};
    return m;
}
// X.class for a boxed or built-in type: the C++ type it is (as the converter's statement layer has it)
const std::map<std::string, std::string, std::less<>>& class_literal_types() {
    static const std::map<std::string, std::string, std::less<>> m = {
        {"Byte", "signed char"}, {"Short", "short"},    {"Integer", "int"},      {"Long", "long long"}, {"Float", "float"},
        {"Double", "double"},    {"Character", "char"}, {"Boolean", "bool"},     {"String", "JavaString"}, {"Object", "JObject"}};
    return m;
}
const std::set<std::string, std::less<>>& npe_catchers() {
    static const std::set<std::string, std::less<>> s = {"java.lang.Exception", "java.lang.RuntimeException", "java.lang.Throwable",
                                                        "java.lang.NullPointerException"};
    return s;
}
// the library classes whose calls change no state (lookups)
const std::set<std::string, std::less<>>& pure_owners() {
    static const std::set<std::string, std::less<>> s = {
        "java.lang.String", "java.lang.Math",      "java.lang.Integer",   "java.lang.Float",
        "java.lang.Double", "java.lang.Long",      "java.lang.Character", "java.lang.Boolean",
        "java.lang.Class",  "java.lang.reflect.Constructor"};
    return s;
}

}  // namespace detail

using detail::after_dot;

// ---------------------------------------------------------------------------------------------- types
jir::JType jtype(const ir::TypeP& type) {
    if (!type) return jir::JType{"void", 0, {}};
    const auto [dims, base] = ir::dims(type);
    static const std::map<std::string, std::string, std::less<>> kPrimitives = {
        {"bool", "boolean"}, {"i8", "byte"},  {"i16", "short"}, {"u16", "char"}, {"i32", "int"},
        {"i64", "long"},     {"f32", "float"}, {"f64", "double"}, {"void", "void"}};   // IR kinds -> the Java names the resolver keys on
    const std::string_view kind = base->kind();
    std::string name;
    if (const auto it = kPrimitives.find(kind); it != kPrimitives.end()) name = it->second;
    else if (kind == "class") name = base->simple();
    else if (kind == "typevar" || kind == "wildcard" || kind == "null") name = "Object";
    else throw Decline("type " + ir::to_text(*type));
    std::vector<jir::JType> args;
    if (const auto* cls = std::get_if<ir::ClassType>(&base->body)) {
        for (const ir::TypeP& a : cls->args) args.push_back(jtype(a));
    }
    return jir::JType{std::move(name), static_cast<std::uint32_t>(dims), std::move(args)};
}

std::vector<std::pair<std::string, std::string>> split_params(std::string_view params) {
    std::vector<std::string> pieces;
    int depth = 0;
    std::string cur;
    for (const char ch : params) {
        if (ch == ',' && depth == 0) {
            pieces.push_back(cur);
            cur.clear();
            continue;
        }
        depth += ch == '<';
        depth -= ch == '>';
        cur.push_back(ch);
    }
    if (!text::trim(cur).empty()) pieces.push_back(cur);
    std::vector<std::pair<std::string, std::string>> out;
    for (const std::string& piece : pieces) {   // the last identifier is the name, what is before it the type
        const std::string_view p = text::trim(piece);
        std::size_t start = p.size();
        while (start > 0 && (std::isalnum(static_cast<unsigned char>(p[start - 1])) != 0 || p[start - 1] == '_')) --start;
        while (start < p.size() && (p[start] >= '0' && p[start] <= '9')) ++start;   // an identifier does not begin with a digit
        if (start == p.size()) throw Decline("parameter without a name: " + std::string(p));
        out.emplace_back(std::string(text::trim(p.substr(0, start))), std::string(p.substr(start)));
    }
    return out;
}

bool is_pointer(std::string_view ctype) { return text::rtrim(ctype).ends_with('*'); }

std::string array_element(std::string_view ctype) {
    const std::string_view t = text::trim(ctype);
    if (!t.starts_with("JArray<") || !t.ends_with('>')) throw Decline("index into " + std::string(ctype));
    return std::string(text::trim(t.substr(7, t.size() - 8)));
}

std::optional<std::string> pointee_class(std::string_view ctype) {
    std::string_view t = text::trim(ctype);
    if (!t.ends_with('*')) return std::nullopt;
    t = text::rtrim(t.substr(0, t.size() - 1));
    if (!text::is_identifier(t)) return std::nullopt;
    return std::string(t);
}

// ---------------------------------------------------------------------------------------------- Context
Context::Context(const cb::java::types::TypeResolver& resolver_, const cb::java::typemap::TypeProfile& profile_,
                 const cb::java::runtime::Runtime& runtime_, std::span<const jir::JClass> classes)
    : resolver(resolver_), profile(profile_), runtime(runtime_) {
    for (const jir::JClass& c : classes) classes_[c.name] = &c;
}

void Context::set_program_classes(std::set<std::string, std::less<>> qualified_names) { program_ = std::move(qualified_names); }

bool Context::is_game(const std::optional<std::string>& qualified) const {
    if (!program_.empty()) return qualified && program_.contains(*qualified);
    return classes_.contains(after_dot(qualified ? std::string_view(*qualified) : std::string_view()));
}

const jir::JClass* Context::parsed_class(std::string_view simple) const {
    const auto it = classes_.find(simple);
    return it == classes_.end() ? nullptr : it->second;
}

static std::string param_key(const jir::JType& type) {
    std::string key = type.name;
    for (std::uint32_t i = 0; i < type.array_dims; ++i) key += "[]";
    return key;
}

const jir::Method* Context::parsed_ctor(std::string_view simple, std::span<const ir::TypeP> params) const {
    const jir::JClass* c = parsed_class(simple);
    if (c == nullptr) return nullptr;
    std::vector<std::string> want;
    for (const ir::TypeP& p : params) want.push_back(param_key(jtype(p)));
    for (const jir::Method* m : c->constructors()) {
        std::vector<std::string> got;
        for (const jir::Param& p : m->params) got.push_back(param_key(p.type));
        if (got == want) return m;
    }
    return nullptr;
}

const jir::Method* Context::parsed_method(std::string_view owner_in, std::string_view name, std::span<const ir::TypeP> params) const {
    std::set<std::string, std::less<>> seen;
    std::optional<std::string> owner = std::string(owner_in);
    std::vector<std::string> want;
    for (const ir::TypeP& p : params) want.push_back(param_key(jtype(p)));
    while (owner && !owner->empty() && !seen.contains(*owner)) {
        seen.insert(*owner);
        const jir::JClass* c = parsed_class(*owner);
        if (c == nullptr) return nullptr;
        for (const jir::Method& m : c->methods) {
            if (m.name == name && !m.is_ctor && m.params.size() == params.size()) {
                std::vector<std::string> got;
                for (const jir::Param& p : m.params) got.push_back(param_key(p.type));
                if (got == want) return &m;
            }
        }
        for (const jir::Method& m : c->methods) {   // erased generics / raw types: by arity
            if (m.name == name && !m.is_ctor && m.params.size() == params.size()) return &m;
        }
        owner = c->parent;
    }
    return nullptr;
}

std::optional<std::string> Context::field_ctype(std::string_view owner, std::string_view name) const {
    const jir::JType* jt = resolver.member_field_type(owner, name);
    if (jt == nullptr) return std::nullopt;
    if (jt->name == "byte" && !jt->is_array() &&
        profile.unsigned_byte_scalar_fields.contains(typemap::ClassField(std::string(owner), std::string(name))))
        return std::string("unsigned char");
    return resolver.field_type(*jt);
}

// ---------------------------------------------------------------------------------------------- Writer
Writer::Writer(Context& ctx, const ir::Node& cls, const ir::Node* method, const jir::Method* parsed)
    : ctx_(ctx),
      r_(ctx.resolver),
      cls_(*ir::get<ir::ClassDecl>(&cls)),
      m_(ir::get<ir::MethodDecl>(method)),
      parsed_(parsed) {
    CB_ASSERT(ir::get<ir::ClassDecl>(&cls) != nullptr, "a writer is made for a class");
    cname_ = cls_.name ? *cls_.name : "None";
    if (method != nullptr) method_line_ = method->line;
    if (parsed_ != nullptr) {
        for (const auto& [type, name] : split_params(ownership::params_str(*parsed_, r_))) vars[name] = type;
    }
    if (cls_.extends) parent_ = cls_.extends->simple();
    ret = parsed_ != nullptr ? ownership::return_cpp_type(*parsed_, r_) : "void";
    // the no-GC ownership cleanups run in methods (as the converter's did), not constructors
    owning_ = parsed_ != nullptr && m_ != nullptr && m_->kind && *m_->kind == "method";
}

std::string Writer::ctype(const ir::TypeP& type, Use use) const {
    const jir::JType jt = jtype(type);
    if (!jt.is_array() && r_.data().values.contains(jt.name)) throw Decline("value type " + jt.name);
    return r_.cpp_type(jt, use);
}

std::string Writer::name(std::string_view n) const {
    return detail::cpp_keywords().contains(n) ? std::string(n) + "_" : std::string(n);
}

}  // namespace cb::translate::backends::cpp03
