// cb/translate/backends/c_java_names.cpp -- what the C back end for Java says in C: names, the C type of a Java type, the runtime's
// letters, string literals, and the small questions it asks of a tree (see c_java.hpp).

#include <array>
#include <format>

#include "cb/core/text.hpp"
#include "cb/translate/backends/c_java_internal.hpp"

namespace cb::translate::backends::c_java::detail {

// ---------------------------------------------------------------------------------------------- reading the tree
ir::TypeP type_of(const ir::Node* node) {
    if (node == nullptr) return {};
    if (const auto* local = ir::get<ir::Local>(node)) return local->type;
    if (const auto* param = ir::get<ir::Param>(node)) return param->type;
    return node->type;
}

std::string_view kind_of(const ir::TypeP& type) { return type ? type->kind() : std::string_view(); }

std::string_view class_name(const ir::TypeP& type) {
    if (!type) return {};
    const auto* cls = std::get_if<ir::ClassType>(&type->body);
    return cls != nullptr ? std::string_view(cls->name) : std::string_view();
}

ir::TypeP elem_of(const ir::TypeP& type) {
    if (!type) return {};
    const auto* array = std::get_if<ir::ArrayType>(&type->body);
    return array != nullptr ? array->elem : ir::TypeP();
}

ir::MethodDecl& method_of(const ir::Node* node) {
    auto* method = ir::get<ir::MethodDecl>(const_cast<ir::Node*>(node));
    if (method == nullptr) throw Decline("a member that is not a method");
    return *method;
}

ir::FieldDecl& field_of(const ir::Node* node) {
    auto* field = ir::get<ir::FieldDecl>(const_cast<ir::Node*>(node));
    if (field == nullptr) throw Decline("a member that is not a field");
    return *field;
}

const ir::Param& param_of(const ir::Node* node) {
    const auto* param = ir::get<ir::Param>(node);
    if (param == nullptr) throw Decline("a parameter that is not a Param");
    return *param;
}

ir::Sym& need_sym(const ir::SymP& sym, std::string_view what) {
    if (!sym) throw Decline(std::format("{} has no symbol", what));
    return *sym;
}

ir::Node* single(const ir::Suite& suite) {
    if (const auto* one = std::get_if<ir::Node*>(&suite)) return *one;
    const auto& list = std::get<std::vector<ir::Node*>>(suite);
    if (list.empty()) return nullptr;
    throw Decline("statement list");
}

const std::vector<ir::Node*>& body_statements(const ir::MethodDecl& method) {
    const auto* block = ir::get<ir::Block>(single(method.body));
    if (block == nullptr) throw Decline("a method body that is not a block");
    return block->stmts;
}

bool has_body(const ir::MethodDecl& method) { return ir::present(method.body); }

bool is_static(const ir::MethodDecl& method) { return method.sym != nullptr && method.sym->is_static; }

bool is_private(const ir::MethodDecl& method) {
    for (const std::string& mod : method.mods) {
        if (mod == "private") return true;
    }
    return false;
}

bool is_object_method(const ir::MethodDecl& method) {
    if (!method.name) return false;
    const std::string& name = *method.name;
    if (name != "toString" && name != "equals" && name != "hashCode") return false;
    return method.params.size() == (name == "equals" ? 1u : 0u);
}

// ---------------------------------------------------------------------------------------------- names
namespace {

bool is_c_keyword(std::string_view name) {
    static const std::unordered_set<std::string_view> words = {
        "auto",     "break",  "case",   "char",     "const",   "continue", "default", "do",    "double", "else",   "enum",
        "extern",   "float",  "for",    "goto",     "if",      "inline",   "int",     "long",  "register", "restrict",
        "return",   "short",  "signed", "sizeof",   "static",  "struct",   "switch",  "typedef", "union", "unsigned",
        "void",     "volatile", "while", "bool",    "true",    "false",    "self",    "self_", "NULL",   "errno",  "stdin",
        "stdout",   "stderr", "EOF",    "jrt"};
    return words.contains(name);
}

}  // namespace

std::string ident(std::string_view name) {
    if (is_c_keyword(name)) return std::string(name) + "_";
    std::string out(name);
    for (char& c : out) {
        if (c == '$') c = '_';
    }
    return out;
}

std::string_view after_dot(std::string_view qname) {
    const std::size_t at = qname.rfind('.');
    return at == std::string_view::npos ? qname : qname.substr(at + 1);
}

std::string join_commas(const std::vector<std::string>& parts) { return text::join(parts, ", "); }

std::string decl_params(const std::vector<std::string>& params) {
    std::string joined = join_commas(params);
    return joined.empty() ? "void" : joined;
}

std::optional<std::uint32_t> first_code_point(std::string_view s) {
    if (s.empty()) return std::nullopt;
    const auto b0 = static_cast<unsigned char>(s[0]);
    std::size_t n = 1;
    std::uint32_t cp = b0;
    if (b0 >= 0xF0) {
        n = 4;
        cp = b0 & 0x07u;
    } else if (b0 >= 0xE0) {
        n = 3;
        cp = b0 & 0x0Fu;
    } else if (b0 >= 0xC0) {
        n = 2;
        cp = b0 & 0x1Fu;
    } else if (b0 >= 0x80) {
        return std::nullopt;
    }
    if (s.size() < n) return std::nullopt;
    for (std::size_t i = 1; i < n; ++i) {
        const auto b = static_cast<unsigned char>(s[i]);
        if ((b & 0xC0u) != 0x80u) return std::nullopt;
        cp = (cp << 6) | (b & 0x3Fu);
    }
    return cp;
}

Utf16Literal utf16_literal(std::string_view utf8) {
    // Java strings are UTF-16: a character above U+FFFF is two units, and the length is the number of units.
    std::vector<char16_t> units;
    units.reserve(utf8.size());
    for (std::size_t i = 0; i < utf8.size();) {
        std::optional<std::uint32_t> cp = first_code_point(utf8.substr(i));
        std::size_t width = 1;
        if (!cp) {
            cp = static_cast<unsigned char>(utf8[i]);   // a byte that is no UTF-8 reads as itself
        } else {
            width = *cp < 0x80 ? 1 : *cp < 0x800 ? 2 : *cp < 0x10000 ? 3 : 4;
        }
        i += width;
        if (*cp >= 0x10000) {
            const std::uint32_t v = *cp - 0x10000;
            units.push_back(static_cast<char16_t>(0xD800 + (v >> 10)));
            units.push_back(static_cast<char16_t>(0xDC00 + (v & 0x3FF)));
        } else {
            units.push_back(static_cast<char16_t>(*cp));
        }
    }
    std::string body;
    bool prev_hex = false;   // after a \x escape a hex digit would be read as part of it: close and reopen the literal
    for (const char16_t u : units) {
        if (u == '\\' || u == '"') {
            body += '\\';
            body += static_cast<char>(u);
            prev_hex = false;
        } else if (u == '\n') {
            body += "\\n";
            prev_hex = false;
        } else if (u >= 32 && u < 127) {
            const bool hex_digit = (u >= '0' && u <= '9') || (u >= 'a' && u <= 'f') || (u >= 'A' && u <= 'F');
            if (prev_hex && hex_digit) body += "\" u\"";
            body += static_cast<char>(u);
            prev_hex = false;
        } else {
            body += std::format("\\x{:04x}", static_cast<unsigned>(u));
            prev_hex = true;
        }
    }
    return {std::format("u\"{}\"", body), units.size()};
}

// ---------------------------------------------------------------------------------------------- the tables
namespace {

struct Pair {
    std::string_view key, value;
};

std::string_view lookup(std::span<const Pair> table, std::string_view key) {
    for (const Pair& p : table) {
        if (p.key == key) return p.value;
    }
    return {};
}

constexpr Pair kPrim[] = {{"bool", "bool"},    {"i8", "int8_t"},   {"i16", "int16_t"}, {"u16", "char16_t"}, {"i32", "int32_t"},
                          {"i64", "int64_t"},  {"f32", "float"},   {"f64", "double"},  {"void", "void"}};
constexpr Pair kLetter[] = {{"bool", "Z"}, {"i8", "B"},  {"i16", "S"}, {"u16", "C"},
                            {"i32", "I"},  {"i64", "J"}, {"f32", "F"}, {"f64", "D"}};
constexpr Pair kBox[] = {{"java.lang.Integer", "I"},   {"java.lang.Long", "J"},      {"java.lang.Short", "S"},
                         {"java.lang.Byte", "B"},      {"java.lang.Character", "C"}, {"java.lang.Boolean", "Z"},
                         {"java.lang.Float", "F"},     {"java.lang.Double", "D"}};
constexpr Pair kLetterCtype[] = {{"Z", "bool"},    {"B", "int8_t"},  {"S", "int16_t"}, {"C", "char16_t"},
                                 {"I", "int32_t"}, {"J", "int64_t"}, {"F", "float"},   {"D", "double"}};

}  // namespace

std::string_view prim_ctype(std::string_view kind) { return lookup(kPrim, kind); }
std::string_view kind_letter(std::string_view kind) { return lookup(kLetter, kind); }
std::string_view box_letter(std::string_view qname) { return lookup(kBox, qname); }
std::string_view letter_ctype(std::string_view letter) { return lookup(kLetterCtype, letter); }

std::string_view ctype_letter(std::string_view ctype) {
    for (const Pair& p : kLetterCtype) {
        if (p.value == ctype) return p.key;
    }
    return {};
}

bool is_numeric(std::string_view c) {
    return c == "int8_t" || c == "int16_t" || c == "char16_t" || c == "int32_t" || c == "int64_t" || c == "float" || c == "double";
}
bool is_integral(std::string_view c) {
    return c == "int8_t" || c == "int16_t" || c == "char16_t" || c == "int32_t" || c == "int64_t";
}
bool is_prim_c(std::string_view c) { return is_numeric(c) || c == "bool"; }
bool is_pointer_ctype(std::string_view c) { return c.ends_with('*'); }

std::string type_code(const ir::TypeP& type) {
    if (!type) return "";
    const std::string_view kind = type->kind();
    if (const std::string_view letter = kind_letter(kind); !letter.empty()) return std::string(letter);
    if (kind == "array") {
        const ir::TypeP elem = elem_of(type);
        const std::string_view letter = elem ? kind_letter(elem->kind()) : std::string_view();
        return std::string(letter.empty() ? std::string_view("O") : letter) + "A";
    }
    if (class_name(type) == "java.lang.String") return "Str";
    return "O";
}

const Code* library_static(std::string_view owner, std::string_view name) {
    struct Entry {
        std::string_view owner, name;
        Code code;
    };
    static const std::array<Entry, 11> table = {{
        {"java.lang.Integer", "MAX_VALUE", {"JRT_INT_MAX", "int32_t"}},
        {"java.lang.Integer", "MIN_VALUE", {"JRT_INT_MIN", "int32_t"}},
        {"java.lang.Long", "MAX_VALUE", {"JRT_LONG_MAX", "int64_t"}},
        {"java.lang.Long", "MIN_VALUE", {"JRT_LONG_MIN", "int64_t"}},
        {"java.lang.Math", "PI", {"JRT_PI", "double"}},
        {"java.lang.Math", "E", {"JRT_E", "double"}},
        {"java.lang.Double", "NaN", {"NAN", "double"}},
        {"java.lang.Double", "POSITIVE_INFINITY", {"INFINITY", "double"}},
        {"java.lang.Double", "NEGATIVE_INFINITY", {"(-INFINITY)", "double"}},
        {"java.lang.System", "out", {"jrt_System_out", "void*"}},
        {"java.lang.System", "err", {"jrt_System_err", "void*"}},
    }};
    for (const Entry& e : table) {
        if (e.owner == owner && e.name == name) return &e.code;
    }
    return nullptr;
}

bool is_library_throwable(std::string_view qname) {
    static const std::unordered_set<std::string_view> names = [] {
        std::unordered_set<std::string_view> out;
        for (const std::string& q : jmodel::library_throwables()) out.insert(q);
        return out;
    }();
    return qname == "java.lang.Throwable" || names.contains(qname);
}

std::string array_store(std::string_view element_ctype) {
    if (is_pointer_ctype(element_ctype) && element_ctype != kStr && element_ctype != kArr && element_ctype != kSb) return "jobject*";
    return std::string(element_ctype);
}

}  // namespace cb::translate::backends::c_java::detail
