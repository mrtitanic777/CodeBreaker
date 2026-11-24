// translate/backends/rust_java_names.cpp -- reading the tree, and the names, literals and types of the Rust back end for Java (see rust_java.hpp).

#include <charconv>
#include <cmath>
#include <format>

#include "translate/backends/rust_java_internal.hpp"

namespace cb::translate::backends::rust_java::detail {

// ---- reading the tree ----------------------------------------------------------------------------------------------------

ir::TypeP type_of(const ir::Node* node) {
    if (node == nullptr) return {};
    if (node->type) return node->type;
    if (const auto* name = ir::get<ir::Name>(node); name != nullptr && name->sym) return name->sym->type;
    if (const auto* local = ir::get<ir::Local>(node)) return local->type ? local->type : (local->sym ? local->sym->type : ir::TypeP());
    if (const auto* param = ir::get<ir::Param>(node)) return param->type ? param->type : (param->sym ? param->sym->type : ir::TypeP());
    return {};
}

std::string_view kind_of(const ir::TypeP& type) { return type ? type->kind() : std::string_view(); }

std::string_view class_name(const ir::TypeP& type) {
    if (!type) return {};
    const auto* c = std::get_if<ir::ClassType>(&type->body);
    return c != nullptr ? std::string_view(c->name) : std::string_view();
}

ir::TypeP elem_of(const ir::TypeP& type) {
    if (!type) return {};
    const auto* a = std::get_if<ir::ArrayType>(&type->body);
    return a != nullptr ? a->elem : ir::TypeP();
}

const ir::Node* strip_paren(const ir::Node* node) {
    while (const auto* p = ir::get<ir::Paren>(node)) node = p->x;
    return node;
}

ir::MethodDecl& method_of(const ir::Node* node) {
    auto* m = ir::get<ir::MethodDecl>(const_cast<ir::Node*>(node));
    if (m == nullptr) throw Decline("a member that is not a method where one was expected");
    return *m;
}
ir::FieldDecl& field_of(const ir::Node* node) {
    auto* f = ir::get<ir::FieldDecl>(const_cast<ir::Node*>(node));
    if (f == nullptr) throw Decline("a member that is not a field where one was expected");
    return *f;
}
const ir::Param& param_of(const ir::Node* node) {
    const auto* p = ir::get<ir::Param>(node);
    if (p == nullptr) throw Decline("a parameter that is not a Param");
    return *p;
}
ir::Sym& need_sym(const ir::SymP& sym, std::string_view what) {
    if (!sym) throw Decline(std::string(what) + " the front end did not resolve");
    return *sym;
}

std::vector<ir::Node*> body_of(const ir::MethodDecl& method) {
    std::vector<ir::Node*> out = ir::statements(method.body);
    if (out.size() == 1) {
        if (const auto* block = ir::get<ir::Block>(out[0])) return block->stmts;
    }
    return out;
}

bool has_mod(const std::vector<std::string>& mods, std::string_view mod) {
    for (const std::string& m : mods)
        if (m == mod) return true;
    return false;
}
bool is_static(const ir::MethodDecl& method) { return has_mod(method.mods, "static") || (method.sym && method.sym->is_static); }
bool is_private(const ir::MethodDecl& method) { return has_mod(method.mods, "private"); }

bool is_prim_kind(std::string_view k) { return k == "bool" || k == "i8" || k == "i16" || k == "u16" || k == "i32" || k == "i64" || k == "f32" || k == "f64"; }
bool is_integral_kind(std::string_view k) { return k == "i8" || k == "i16" || k == "u16" || k == "i32" || k == "i64"; }
bool is_float_kind(std::string_view k) { return k == "f32" || k == "f64"; }
bool is_numeric_kind(std::string_view k) { return is_integral_kind(k) || is_float_kind(k); }

std::string_view boxed_kind(const ir::TypeP& type) {
    const std::string_view n = class_name(type);
    if (n == "java.lang.Integer") return "i32";
    if (n == "java.lang.Long") return "i64";
    if (n == "java.lang.Double") return "f64";
    if (n == "java.lang.Float") return "f32";
    if (n == "java.lang.Short") return "i16";
    if (n == "java.lang.Byte") return "i8";
    if (n == "java.lang.Boolean") return "bool";
    if (n == "java.lang.Character") return "u16";
    return {};
}

ir::TypeP prim_by_kind(std::string_view k) {
    if (k == "i32") return ir::i32_type();
    if (k == "bool") return ir::bool_type();
    if (k == "i64") return ir::prim_type(ir::Prim::I64);
    if (k == "f64") return ir::prim_type(ir::Prim::F64);
    if (k == "f32") return ir::prim_type(ir::Prim::F32);
    if (k == "i16") return ir::prim_type(ir::Prim::I16);
    if (k == "i8") return ir::prim_type(ir::Prim::I8);
    if (k == "u16") return ir::prim_type(ir::Prim::U16);
    throw Decline("the primitive " + std::string(k));
}

ir::TypeP promoted(const ir::TypeP& a, const ir::TypeP& b) {
    const std::string_view x = kind_of(a), y = kind_of(b);
    if (x == "f64" || y == "f64") return ir::prim_type(ir::Prim::F64);
    if (x == "f32" || y == "f32") return ir::prim_type(ir::Prim::F32);
    if (x == "i64" || y == "i64") return ir::prim_type(ir::Prim::I64);
    return ir::i32_type();
}
ir::TypeP unary_promoted(const ir::TypeP& a) {
    const std::string_view x = kind_of(a);
    if (x == "i64" || x == "f32" || x == "f64") return a;
    return ir::i32_type();
}

bool is_pure(const ir::Node* node) {
    node = strip_paren(node);
    if (node == nullptr) return true;
    if (ir::is<ir::Lit>(node) || ir::is<ir::Name>(node) || ir::is<ir::This>(node) || ir::is<ir::TypeName>(node)) return true;
    if (const auto* s = ir::get<ir::Select>(node)) return is_pure(s->obj);
    if (const auto* l = ir::get<ir::Length>(node)) return is_pure(l->arr);
    return false;
}

// ---- names ------------------------------------------------------------------------------------------------------------------

namespace {
const std::set<std::string_view>& keywords() {
    static const std::set<std::string_view> list = {
        "as", "break", "const", "continue", "crate", "else", "enum", "extern", "false", "fn", "for", "if", "impl", "in", "let", "loop", "match", "mod", "move",
        "mut", "pub", "ref", "return", "self", "Self", "static", "struct", "super", "trait", "true", "type", "unsafe", "use", "where", "while", "async", "await",
        "dyn", "abstract", "become", "box", "do", "final", "macro", "override", "priv", "typeof", "unsized", "virtual", "yield", "try", "gen", "union"};
    return list;
}
}  // namespace

std::string member_ident(std::string_view name) {
    std::string out;
    for (const char c : name) out.push_back(c == '$' ? '_' : c);
    if (keywords().contains(out)) out.push_back('_');
    return out;
}

std::string ident(std::string_view name) {
    static const std::set<std::string_view> reserved = {
        // what the translation's own text uses bare
        "lit", "cat", "up", "same", "throw", "run", "fa", "new_array", "array_of", "new_array2", "new_array3", "instance_of", "try_catch", "catch_all",
        "rethrow", "jformat", "check_cast", "Str", "Ref", "Arr", "System", "Math", "StringBuilder", "Object", "Throwable", "Some", "None", "Ok", "Err", "Box",
        "Vec", "String", "Option", "Cell", "Flow", "Default", "main", "this", "flow", "ex"};
    std::string out;
    for (const char c : name) out.push_back(c == '$' ? '_' : c);
    if (reserved.contains(out) || keywords().contains(out)) out.push_back('_');
    return out;
}

std::string join(const std::vector<std::string>& parts, std::string_view sep) {
    std::string out;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i > 0) out += sep;
        out += parts[i];
    }
    return out;
}

std::string indent_of(std::size_t depth) { return std::string(depth * 4, ' '); }

std::string short_code(const ir::TypeP& type) {
    const std::string_view k = kind_of(type);
    if (is_prim_kind(k)) return std::string(k == "u16" ? "char" : k);
    if (k == "array") return short_code(elem_of(type)) + "_arr";
    if (k == "class") {
        const std::string_view n = class_name(type);
        if (n == "java.lang.String") return "str";
        const std::size_t dot = n.rfind('.');
        std::string s(dot == std::string_view::npos ? n : n.substr(dot + 1));
        for (char& c : s) {
            if (c == '$') c = '_';
        }
        return s;
    }
    return std::string(k);
}

std::string type_suffix(std::string_view kind) { return std::string(kind); }

// A Java string (as UTF-8 text) as the inside of a Rust string literal.
std::string rs_string(std::string_view s) {
    std::string out = "\"";
    std::size_t i = 0;
    while (i < s.size()) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        std::size_t len = c < 0x80 ? 1 : c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC2 ? 2 : 0;
        if (len == 0 || i + len > s.size()) throw Decline("a string that is not valid text");
        std::uint32_t cp = len == 1 ? c : c & (0xFFu >> (len + 1));
        for (std::size_t k = 1; k < len; ++k) {
            const unsigned char cc = static_cast<unsigned char>(s[i + k]);
            if ((cc & 0xC0) != 0x80) throw Decline("a string that is not valid text");
            cp = (cp << 6) | (cc & 0x3F);
        }
        if (cp >= 0xD800 && cp <= 0xDFFF) throw Decline("a string with a lone surrogate");
        i += len;
        switch (cp) {
            case '\\': out += "\\\\"; break;
            case '"': out += "\\\""; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (cp < 0x20 || cp == 0x7F) out += std::format("\\u{{{:x}}}", cp);
                else out.append(s.substr(i - len, len));
        }
    }
    out += '"';
    return out;
}

// The shortest text of a double that reads back, as a Rust literal: always with a point or an exponent.
std::string float_text(double value, bool single) {
    if (std::isnan(value) || std::isinf(value)) {
        const std::string t = std::isnan(value) ? "NAN" : value > 0 ? "INFINITY" : "NEG_INFINITY";
        return std::string(single ? "f32::" : "f64::") + t;
    }
    char buf[64];
    const auto res = single ? std::to_chars(buf, buf + sizeof buf, static_cast<float>(value)) : std::to_chars(buf, buf + sizeof buf, value);
    std::string s(buf, res.ptr);
    if (s.find('.') == std::string::npos && s.find('e') == std::string::npos) s += ".0";
    if (single) s += "f32";
    return s;
}

}  // namespace cb::translate::backends::rust_java::detail
