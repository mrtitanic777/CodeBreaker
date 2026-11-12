// cb/translate/backends/java.cpp -- the Java back end: C types and Java text, the pointer analysis, the program, and the
// faithful tier's file (java_fn.cpp holds the function writer).
#include "cb/translate/backends/java.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <format>

#include "cb/core/text.hpp"

namespace cb::translate::backends::java {

// ---- C types ------------------------------------------------------------------------------------------------------------

namespace {

struct JvKindVisitor {
    CKind operator()(const ir::PrimType& t) const {
        switch (t.prim) {
            case ir::Prim::Bool: return CKind::Bool;
            case ir::Prim::I8: return CKind::I8;
            case ir::Prim::I16: return CKind::I16;
            case ir::Prim::I32: return CKind::I32;
            case ir::Prim::I64: return CKind::I64;
            case ir::Prim::U8: return CKind::U8;
            case ir::Prim::U16: return CKind::U16;
            case ir::Prim::U32: return CKind::U32;
            case ir::Prim::U64: return CKind::U64;
            case ir::Prim::F32: return CKind::F32;
            case ir::Prim::F64: return CKind::F64;
            case ir::Prim::F80: return CKind::F80;
            case ir::Prim::Char: return t.is_signed.value_or(true) ? CKind::I8 : CKind::U8;   // plain char: by its sign
        }
        return CKind::Other;
    }
    CKind operator()(const ir::VoidType&) const { return CKind::Void; }
    CKind operator()(const ir::NullType&) const { return CKind::Null; }
    CKind operator()(const ir::ClassType&) const { return CKind::Class; }
    CKind operator()(const ir::ArrayType&) const { return CKind::Array; }
    CKind operator()(const ir::PtrType&) const { return CKind::Ptr; }
    CKind operator()(const ir::FnType&) const { return CKind::Fn; }
    CKind operator()(const ir::FnPtrType&) const { return CKind::FnPtr; }
    CKind operator()(const ir::RecordType&) const { return CKind::Record; }
    CKind operator()(const auto&) const { return CKind::Other; }
};

}  // namespace

CKind kind_of(const ir::TypeP& type) {
    if (!type) return CKind::Void;
    return std::visit(JvKindVisitor{}, type->body);
}

int bits_of(CKind kind) {
    switch (kind) {
        case CKind::Bool: return 1;
        case CKind::I8: case CKind::U8: return 8;
        case CKind::I16: case CKind::U16: return 16;
        case CKind::I32: case CKind::U32: return 32;
        case CKind::I64: case CKind::U64: return 64;
        default: return 0;
    }
}

bool is_int(const ir::TypeP& type) {
    const CKind k = kind_of(type);
    return bits_of(k) != 0 && k != CKind::Bool;
}
bool is_unsigned(const ir::TypeP& type) {
    const CKind k = kind_of(type);
    return k == CKind::U8 || k == CKind::U16 || k == CKind::U32 || k == CKind::U64;
}
bool is_float(const ir::TypeP& type) {
    const CKind k = kind_of(type);
    return k == CKind::F32 || k == CKind::F64 || k == CKind::F80;
}
bool is_ptr(const ir::TypeP& type) {
    const CKind k = kind_of(type);
    return k == CKind::Ptr || k == CKind::FnPtr || k == CKind::Null;
}
bool is_record(const ir::TypeP& type) {
    const CKind k = kind_of(type);
    return k == CKind::Class || k == CKind::Record;
}

std::string_view java_primitive(CKind kind) {
    switch (kind) {
        case CKind::Bool: return "boolean";
        case CKind::I8: case CKind::U8: return "byte";
        case CKind::I16: return "short";
        case CKind::U16: return "char";
        case CKind::I32: case CKind::U32: return "int";
        case CKind::I64: case CKind::U64: return "long";
        case CKind::F32: return "float";
        case CKind::F64: case CKind::F80: return "double";
        default: return {};
    }
}

std::string spell(const ir::TypeP& type) {
    if (!type) return "?";
    if (const auto* c = std::get_if<ir::ClassType>(&type->body)) {
        std::string out = c->name;
        if (!c->args.empty()) {
            out += '<';
            for (std::size_t i = 0; i < c->args.size(); ++i) out += (i != 0 ? ", " : "") + spell(c->args[i]);
            out += '>';
        }
        return out;
    }
    if (const auto* p = std::get_if<ir::PtrType>(&type->body)) return spell(p->to) + (p->ref ? "&" : "*");
    if (const auto* a = std::get_if<ir::ArrayType>(&type->body)) {
        return spell(a->elem) + "[" + (a->count ? std::to_string(*a->count) : std::string()) + "]";
    }
    if (const auto* fp = std::get_if<ir::FnPtrType>(&type->body)) return spell(fp->fn) + "*";
    if (const auto* f = std::get_if<ir::FnType>(&type->body)) {
        std::string out = spell(f->result.type) + "(";
        for (std::size_t i = 0; i < f->params.size(); ++i) out += (i != 0 ? "," : "") + spell(f->params[i].type);
        return out + ")";
    }
    return std::string(type->kind());
}

ir::Node* strip(ir::Node* expr) {
    while (const auto* p = ir::get<ir::Paren>(expr)) expr = p->x;
    return expr;
}

bool is_null_constant(ir::Node* expr) {
    expr = strip(expr);
    while (const auto* c = ir::get<ir::Cast>(expr)) expr = strip(c->x);
    const ir::Lit* lit = ir::get<ir::Lit>(expr);
    if (lit == nullptr) return false;
    struct Visitor {
        bool operator()(std::monostate) const { return true; }
        bool operator()(bool v) const { return !v; }
        bool operator()(std::int64_t v) const { return v == 0; }
        bool operator()(std::uint64_t v) const { return v == 0; }
        bool operator()(double) const { return false; }
        bool operator()(const std::string&) const { return false; }
    };
    return std::visit(Visitor{}, lit->value);
}

bool from_alloc(ir::Node* expr) {
    const ir::Call* c = ir::get<ir::Call>(strip(expr));
    return c != nullptr && c->target == nullptr && c->sym != nullptr &&
           (c->sym->name == "malloc" || c->sym->name == "calloc" || c->sym->name == "realloc");
}

const ir::TypeP& pointee(const ir::TypeP& type) {
    static const ir::TypeP none;
    const auto* p = type ? std::get_if<ir::PtrType>(&type->body) : nullptr;
    return p != nullptr ? p->to : none;
}

// ---- text ---------------------------------------------------------------------------------------------------------------

namespace {

// The code points of UTF-8 text (a byte that is not well-formed stands for itself). Literals reach this module as the UTF-8 of
// the front end's text, whose characters are the bytes of a C string (or the units of a wide one).
std::vector<std::uint32_t> jv_code_points(std::string_view s) {
    std::vector<std::uint32_t> out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size();) {
        const auto b0 = static_cast<unsigned char>(s[i]);
        std::size_t n = 1;
        std::uint32_t cp = b0;
        if (b0 >= 0xC0 && b0 < 0xE0) { n = 2; cp = b0 & 0x1Fu; }
        else if (b0 >= 0xE0 && b0 < 0xF0) { n = 3; cp = b0 & 0x0Fu; }
        else if (b0 >= 0xF0 && b0 < 0xF8) { n = 4; cp = b0 & 0x07u; }
        bool good = n > 1 && i + n <= s.size();
        for (std::size_t k = 1; good && k < n; ++k) {
            const auto b = static_cast<unsigned char>(s[i + k]);
            if ((b & 0xC0) != 0x80) good = false;
            else cp = (cp << 6) | (b & 0x3Fu);
        }
        if (!good) { n = 1; cp = b0; }
        out.push_back(cp);
        i += n;
    }
    return out;
}

const std::unordered_set<std::string_view>& jv_keywords() {
    static const std::unordered_set<std::string_view> words = {
        "abstract", "assert", "boolean", "break", "byte", "case", "catch", "char", "class", "const", "continue", "default", "do",
        "double", "else", "enum", "extends", "final", "finally", "float", "for", "goto", "if", "implements", "import", "instanceof",
        "int", "interface", "long", "native", "new", "package", "private", "protected", "public", "return", "short", "static",
        "strictfp", "super", "switch", "synchronized", "this", "throw", "throws", "transient", "try", "void", "volatile", "while",
        "true", "false", "null", "var", "record", "yield", "sealed", "permits",
        // names the runtime owns
        "Mem", "LibC", "Fn", "String", "Object", "Math", "System", "Integer", "Long", "Double", "Float", "Character", "Byte", "Short",
        "Boolean", "frame"};
    return words;
}

// The text of a double as Python's repr wrote it (the shortest digits that read back, in fixed notation from 1e-4 up to 1e16 and
// in scientific notation outside), which is what the Java literal reads.
std::string jv_double_text(double v) {
    if (v == 0.0) return std::signbit(v) ? "-0.0" : "0.0";
    char buf[64];
    const auto r = std::to_chars(buf, buf + sizeof buf, std::fabs(v), std::chars_format::scientific);
    const std::string sci(buf, r.ptr);                       // d[.ddd]e[+-]xx
    const std::size_t e = sci.find('e');
    std::string digits = sci.substr(0, e);
    digits.erase(std::remove(digits.begin(), digits.end(), '.'), digits.end());
    const int exp10 = std::stoi(sci.substr(e + 1));
    const int n = static_cast<int>(digits.size());
    std::string out = v < 0 ? "-" : "";
    if (exp10 >= -4 && exp10 <= 15) {
        if (exp10 >= 0) {
            if (n <= exp10 + 1) out += digits + std::string(static_cast<std::size_t>(exp10 + 1 - n), '0') + ".0";
            else out += digits.substr(0, static_cast<std::size_t>(exp10 + 1)) + "." + digits.substr(static_cast<std::size_t>(exp10 + 1));
        } else {
            out += "0." + std::string(static_cast<std::size_t>(-exp10 - 1), '0') + digits;
        }
        return out;
    }
    // scientific: a mantissa that Java reads as a double needs its point (`1.0e+16`)
    out += digits.substr(0, 1) + "." + (n > 1 ? digits.substr(1) : std::string("0"));
    out += std::format("e{}{:02}", exp10 < 0 ? '-' : '+', std::abs(exp10));
    return out;
}

}  // namespace

std::string Ex::at(int min_prec) const { return prec >= min_prec ? code : "(" + code + ")"; }

std::optional<int> binary_precedence(std::string_view op) {
    static const std::unordered_map<std::string_view, int> table = {
        {"*", kMul}, {"/", kMul}, {"%", kMul}, {"+", kAdd}, {"-", kAdd}, {"<<", kShift}, {">>", kShift}, {">>>", kShift},
        {"<", kRel}, {"<=", kRel}, {">", kRel}, {">=", kRel}, {"==", kEq}, {"!=", kEq}, {"&", kBitAnd}, {"^", kXor},
        {"|", kBitOr}, {"&&", kAnd}, {"||", kOr}};
    const auto it = table.find(op);
    if (it == table.end()) return std::nullopt;
    return it->second;
}

std::string jname(std::string_view name) {
    std::string out = name.empty() ? std::string("x") : text::sanitize_identifier(name);
    if (jv_keywords().contains(out) || (!out.empty() && out[0] >= '0' && out[0] <= '9')) out += '_';
    return out;
}

std::string java_string(std::string_view text) {
    std::string out = "\"";
    for (const std::uint32_t o : jv_code_points(text)) {
        if (o == '"') out += "\\\"";
        else if (o == '\\') out += "\\\\";
        else if (o == '\n') out += "\\n";
        else if (o == '\t') out += "\\t";
        else if (o == '\r') out += "\\r";
        else if (o >= 32 && o < 127) out += static_cast<char>(o);
        else out += std::format("\\{:03o}", o & 0xFFu);
    }
    return out + "\"";
}

std::string java_wstring(std::string_view text) {
    std::string out = "\"";
    for (const std::uint32_t o : jv_code_points(text)) {
        if (o == '"' || o == '\\') { out += '\\'; out += static_cast<char>(o); }
        else if (o == '\n') out += "\\n";
        else if (o == '\r') out += "\\r";
        else if (o == '\t') out += "\\t";
        else if (o >= 32 && o < 127) out += static_cast<char>(o);
        else if (o < 256) out += std::format("\\{:03o}", o);
        else out += std::format("\\u{:04x}", o);
    }
    return out + "\"";
}

std::size_t char_count(std::string_view literal_text) { return jv_code_points(literal_text).size(); }

bool is_wide(const ir::Node& lit) {
    const auto* a = lit.type ? std::get_if<ir::ArrayType>(&lit.type->body) : nullptr;
    if (a == nullptr || !a->elem) return false;
    const CKind k = kind_of(a->elem);
    return k == CKind::U16 || k == CKind::I16;
}

std::string string_lit(const ir::Node& lit) {
    const auto* l = ir::get<ir::Lit>(&lit);
    const std::string* text = l != nullptr ? std::get_if<std::string>(&l->value) : nullptr;
    if (text == nullptr) throw Refused("a string literal that holds no text");
    return is_wide(lit) ? java_wstring(*text) : java_string(*text);
}

Ex int_lit(std::int64_t value, std::string_view jt) {
    int bits = 32;
    if (jt == "byte") bits = 8;
    else if (jt == "short" || jt == "char") bits = 16;
    else if (jt == "long") bits = 64;
    std::uint64_t v = static_cast<std::uint64_t>(value);
    if (bits < 64) v &= (std::uint64_t{1} << bits) - 1;
    if (jt == "char") {
        // 127 (DEL) is a cast too: no Java escape writes it
        const bool plain = v >= 32 && v <= 126 && v != '\'' && v != '\\';
        if (!plain) return {"(char) " + std::to_string(v), kUnary, "char"};
        return {std::string("'") + static_cast<char>(v) + "'", kAtom, "char"};
    }
    std::int64_t sv = static_cast<std::int64_t>(v);
    if (bits < 64 && v >= (std::uint64_t{1} << (bits - 1))) sv = static_cast<std::int64_t>(v) - (std::int64_t{1} << bits);
    if (jt == "long") {
        if (sv < -(std::int64_t{1} << 40) || sv > (std::int64_t{1} << 40)) return {std::format("0x{:X}L", v), kAtom, "long"};
        return {std::to_string(sv) + "L", sv >= 0 ? kAtom : kUnary, "long"};
    }
    if (jt == "byte" || jt == "short") {
        if (sv == 0) return {"0", kAtom, "int"};
        return {"(" + std::string(jt) + ") " + std::to_string(sv), kUnary, std::string(jt)};
    }
    if (sv < -(std::int64_t{1} << 24) && sv != -(std::int64_t{1} << 31)) return {std::format("0x{:X}", v & 0xFFFFFFFFu), kAtom, "int"};
    return {std::to_string(sv), sv >= 0 ? kAtom : kUnary, "int"};
}

Ex float_lit(double value, std::string_view jt) {
    const std::string cls = jt == "double" ? "Double" : "Float";
    if (std::isnan(value)) return {cls + ".NaN", kAtom, std::string(jt)};
    if (std::isinf(value)) return {cls + (value > 0 ? ".POSITIVE_INFINITY" : ".NEGATIVE_INFINITY"), kAtom, std::string(jt)};
    std::string text = jv_double_text(value);
    const std::size_t e = text.find('e');
    if (e != std::string::npos && text.substr(0, e).find('.') == std::string::npos) text.insert(e, ".0");
    const bool negative = text.starts_with('-');
    if (jt == "float") text += 'f';
    return {text, negative ? kUnary : kAtom, std::string(jt)};
}

bool java_char_text(std::string_view text) {
    if (text.size() < 3 || text.front() != '\'' || text.back() != '\'') return false;
    const std::string_view body = text.substr(1, text.size() - 2);
    if (body.front() != '\\') {
        // exactly one character other than a quote, a backslash or a line break
        if (body.front() == '\'' || body.front() == '\n') return false;
        return jv_code_points(body).size() == 1;
    }
    const std::string_view esc = body.substr(1);
    if (esc.size() == 1 && std::string_view("btnfr\"'\\").find(esc[0]) != std::string_view::npos) return true;
    if (esc.empty() || esc.size() > 3) return false;
    for (const char c : esc) {
        if (c < '0' || c > '7') return false;
    }
    return esc.size() < 3 || esc[0] <= '3';
}

bool is_int_text(std::string_view code) {
    std::size_t i = 0;
    if (i < code.size() && code[i] == '(') ++i;
    if (i < code.size() && code[i] == '-') ++i;
    const std::size_t digits = i;
    while (i < code.size() && code[i] >= '0' && code[i] <= '9') ++i;
    if (i == digits) return false;
    if (i < code.size() && code[i] == ')') ++i;
    return i == code.size();
}

std::string promoted(std::string_view jt) {
    return (jt == "byte" || jt == "short" || jt == "char") ? "int" : std::string(jt);
}

std::string promoted2(std::string_view a, std::string_view b) {
    static constexpr std::string_view order[] = {"int", "long", "float", "double"};
    const std::string pa = promoted(a);
    const std::string pb = promoted(b);
    const auto ia = std::find(std::begin(order), std::end(order), pa);
    const auto ib = std::find(std::begin(order), std::end(order), pb);
    if (ia == std::end(order) || ib == std::end(order)) return pa;
    return std::string(*std::max(ia, ib));
}

ir::TypeP promote_type(const ir::TypeP& type) {
    switch (kind_of(type)) {
        case CKind::I8: case CKind::U8: case CKind::I16: case CKind::U16: case CKind::Bool: return ir::i32_type();
        default: return type;
    }
}

// ---- analysis -----------------------------------------------------------------------------------------------------------

namespace {

bool jv_is_raw_memory_call(std::string_view name) {
    static const std::unordered_set<std::string_view> raw = {
        "memset", "memcpy", "memmove", "memcmp", "memchr", "qsort", "bsearch", "realloc", "vprintf", "vfprintf", "vsprintf",
        "vsnprintf", "strchr", "strrchr", "strstr", "strpbrk", "strtol", "strtoul", "strtoll", "strtoull", "strtod", "modf",
        "frexp", "time", "strdup", "_strdup", "fgets"};
    return raw.contains(name);
}

const std::vector<std::string>& jv_mods_of(const ir::Node* member) {
    static const std::vector<std::string> none;
    if (const auto* f = ir::get<ir::FieldDecl>(member)) return f->mods;
    if (const auto* m = ir::get<ir::MethodDecl>(member)) return m->mods;
    return none;
}

bool jv_has(const std::vector<std::string>& mods, std::string_view m) { return std::find(mods.begin(), mods.end(), m) != mods.end(); }

}  // namespace

Analysis& Analysis::run() {
    for (ir::Node* un : units) {
        const ir::Unit* u = ir::get<ir::Unit>(un);
        if (u == nullptr) continue;
        for (ir::Node* t : u->types) {
            if (const ir::ClassDecl* c = ir::get<ir::ClassDecl>(t)) {
                const std::string name = c->name.value_or("");
                if (c->kind == "union") why.push_back("union " + name);
                bool has_method = false;
                bool bitfield = false;
                for (const ir::Node* m : c->members) {
                    has_method = has_method || ir::is<ir::MethodDecl>(m);
                    bitfield = bitfield || jv_has(jv_mods_of(m), "bitfield");
                }
                if (c->kind == "class" || has_method || c->extends) cpp_classes = true;
                if ((c->kind == "struct" || c->kind == "union") && bitfield) why.push_back("bit-fields");
            }
            if (const ir::MethodDecl* m = ir::get<ir::MethodDecl>(t)) {
                if (m->sym != nullptr && m->sym->varargs && ir::present(m->body)) {
                    why.push_back("a variadic function (" + m->name.value_or("") + ")");
                }
            }
            exempt.clear();
            for (ir::Node* n : ir::walk(t)) {
                // free's argument and C's NULL ((void *) 0) are pointers to void that name nothing
                if (const ir::Call* call = ir::get<ir::Call>(n); call != nullptr && call->target == nullptr && call->sym != nullptr && call->sym->name == "free") {
                    for (ir::Node* a : call->args) exempt.insert(strip(a));
                }
                if (const ir::Cast* cast = ir::get<ir::Cast>(n); cast != nullptr && is_null_constant(cast->x)) {
                    exempt.insert(n);
                    exempt.insert(strip(cast->x));
                }
                if (const ir::Cast* cast = ir::get<ir::Cast>(n); cast != nullptr && exempt.contains(n)) {
                    exempt.insert(strip(cast->x));
                    continue;
                }
                visit(n);
            }
        }
    }
    return *this;
}

void Analysis::visit(ir::Node* n) {
    if (const ir::Unary* u = ir::get<ir::Unary>(n); u != nullptr && u->op == "&") {
        ir::Node* x = strip(u->x);
        if (const ir::Name* name = ir::get<ir::Name>(x); name != nullptr && !is_record(x->type) && kind_of(x->type) != CKind::Array) {
            if (name->sym != nullptr) addressed.insert(name->sym->name);   // (a scalar whose address is taken: boxed in the idiomatic tier)
        } else if (ir::is<ir::Index>(x) || (ir::is<ir::Select>(x) && !is_record(x->type))) {
            why.push_back("address of an element or field");
        }
        return;
    }
    if (const ir::Call* c = ir::get<ir::Call>(n); c != nullptr) {
        if (c->target == nullptr && c->sym != nullptr && jv_is_raw_memory_call(c->sym->name)) {
            why.push_back("raw memory (" + c->sym->name + ")");
            return;
        }
        if (c->target != nullptr && c->target->type) {
            const ir::Type& tt = *c->target->type;
            const ir::FnType* fn = nullptr;
            if (const auto* fp = std::get_if<ir::FnPtrType>(&tt.body)) fn = fp->fn ? std::get_if<ir::FnType>(&fp->fn->body) : nullptr;
            else fn = std::get_if<ir::FnType>(&tt.body);
            const bool is_fn_like = std::holds_alternative<ir::FnPtrType>(tt.body) || std::holds_alternative<ir::FnType>(tt.body);
            if (is_fn_like && (fn == nullptr ? 0 : fn->params.size()) != c->args.size()) {
                why.push_back("a K&R function pointer (no prototype)");
                return;
            }
        }
        return;
    }
    if (const ir::Binary* b = ir::get<ir::Binary>(n); b != nullptr) {
        if ((b->op == "<" || b->op == "<=" || b->op == ">" || b->op == ">=") && is_ptr(b->left->type)) {
            why.push_back("pointer comparison");
        } else if ((b->op == "+" || b->op == "-") && (is_ptr(b->left->type) || is_ptr(b->right->type))) {
            why.push_back("pointer arithmetic");
        }
        return;
    }
    if (const ir::Index* ix = ir::get<ir::Index>(n); ix != nullptr) {
        if (ix->arr != nullptr && kind_of(ix->arr->type) == CKind::Ptr && kind_of(pointee(ix->arr->type)) == CKind::Void &&
            pointee(ix->arr->type)) {
            why.push_back("a void pointer");
        }
        return;
    }
    if (const ir::Unary* u = ir::get<ir::Unary>(n); u != nullptr) {
        if ((u->op == "++" || u->op == "--") && is_ptr(n->type)) why.push_back("pointer arithmetic");
        return;
    }
    if (const ir::Assign* a = ir::get<ir::Assign>(n); a != nullptr) {
        if (a->op && (*a->op == "+" || *a->op == "-") && is_ptr(a->target->type)) why.push_back("pointer arithmetic");
        return;
    }
    if (const ir::Cast* cast = ir::get<ir::Cast>(n); cast != nullptr && is_ptr(n->type) && cast->x != nullptr && cast->x->type) {
        const ir::TypeP& a = n->type;
        const ir::TypeP& b = cast->x->type;
        if (is_ptr(b)) {
            const ir::TypeP& ae = pointee(a);
            const ir::TypeP& be = pointee(b);
            const bool both_ptr = kind_of(a) == CKind::Ptr && kind_of(b) == CKind::Ptr;
            const bool a_void = kind_of(ae) == CKind::Void && ae;
            const bool b_void = kind_of(be) == CKind::Void && be;
            if (both_ptr && ae && be && kind_of(ae) != kind_of(be) && !(a_void || b_void) && !cast->implicit) {
                why.push_back("a cast between pointer types");
            } else if (both_ptr && a_void != b_void) {
                if (!from_alloc(cast->x)) why.push_back("a void pointer");
            }
        } else if (is_int(b)) {
            why.push_back("an integer used as a pointer");
        }
        return;
    }
    if (const ir::Cast* cast = ir::get<ir::Cast>(n); cast != nullptr && is_int(n->type) && cast->x != nullptr && is_ptr(cast->x->type)) {
        why.push_back("a pointer used as an integer");
    }
}

// ---- the program --------------------------------------------------------------------------------------------------------------

std::string_view to_text(Tier tier) {
    switch (tier) {
        case Tier::Auto: return "auto";
        case Tier::Idiomatic: return "idiomatic";
        case Tier::Faithful: return "faithful";
    }
    return "auto";
}

namespace {

std::string jv_join(const std::vector<std::string>& parts, std::string_view sep) { return text::join(parts, sep); }

}  // namespace

Program program(std::span<ir::Node* const> units, std::string_view name, const Options& options) {
    Analysis first(units);
    first.run();
    const std::set<std::string> sorted_why(first.why.begin(), first.why.end());
    const std::vector<std::string> why(sorted_why.begin(), sorted_why.end());
    if (options.tier == Tier::Faithful && first.cpp_classes) throw Refused("C++ classes have no faithful (emulated memory) form yet");
    std::optional<std::string> declined;
    if (options.tier == Tier::Idiomatic || (options.tier == Tier::Auto && (why.empty() || first.cpp_classes))) {
        if (!why.empty()) throw Refused("C++ code the object model cannot carry: " + jv_join(why, ", "));
        try {
            if (!options.idiomatic) throw Refused("the idiomatic tier is not available");
            std::unique_ptr<TierWriter> writer = options.idiomatic(units, name, first);
            std::string source = writer->write();
            if (writer->gaps().empty() || first.cpp_classes || options.tier == Tier::Idiomatic) {
                return {std::string(name), std::move(source), Tier::Idiomatic, writer->notes()};
            }
            declined = "gaps in the idiomatic tier: " + jv_join(writer->gaps(), "; ");
        } catch (const Refused& refused) {
            if (first.cpp_classes || options.tier == Tier::Idiomatic) throw;
            declined = refused.what();
        }
    }
    Analysis second(units);   // (afresh: the idiomatic writer may have changed its facts)
    second.run();
    Faithful writer(units, std::string(name), second, options.library);
    writer.init();
    std::string source = writer.write();
    std::vector<std::string> notes = writer.notes;
    if (declined) {
        notes.push_back("faithful tier: the idiomatic tier declined (" + *declined + ")");
    } else {
        const std::set<std::string> reasons(second.why.begin(), second.why.end());
        const std::vector<std::string> listed(reasons.begin(), reasons.end());
        notes.push_back("faithful tier: " + (listed.empty() ? std::string("chosen") : jv_join(listed, ", ")));
    }
    return {std::string(name), std::move(source), Tier::Faithful, std::move(notes)};
}

// ---- the faithful tier ----------------------------------------------------------------------------------------------------------

VarKey key_of(const ir::Sym& sym) {
    if (sym.kind == ir::SymKind::Global || sym.kind == ir::SymKind::Function) return {true, "", sym.name};
    return {false, sym.owner.value_or(""), sym.name};
}

const ir::RecordType& layout_of(const ir::TypeP& layout) {
    const auto* r = layout ? std::get_if<ir::RecordType>(&layout->body) : nullptr;
    if (r == nullptr) throw Refused("a record whose layout is not an explicit one");
    return *r;
}

namespace {

// The type of a parameter node (its own, else its symbol's).
ir::TypeP jv_param_type(const ir::Node* param) {
    const ir::Param* p = ir::get<ir::Param>(param);
    if (p == nullptr) return {};
    if (p->type) return p->type;
    return p->sym != nullptr ? p->sym->type : ir::TypeP{};
}

std::string jv_param_name(const ir::Node* param) {
    const ir::Param* p = ir::get<ir::Param>(param);
    return p != nullptr && p->sym != nullptr ? p->sym->name : std::string();
}

std::string jv_first_unsupported(ir::Node* node) {
    const std::vector<ir::Node*> found = ir::unsupported(node);
    if (found.empty()) return "?";
    const ir::Unsupported* u = ir::get<ir::Unsupported>(found.front());
    return u->kind.value_or("?") + " (line " + std::to_string(found.front()->line) + ")";
}

std::string jv_upper_first(std::string s) {
    if (!s.empty() && s[0] >= 'a' && s[0] <= 'z') s[0] = static_cast<char>(s[0] - 'a' + 'A');
    return s;
}

}  // namespace

// A C type as its declaration reads, around a name (the comment beside a global or a local in memory).
std::string ctype_text(const ir::TypeP& t, const std::string& name) {
    if (!t) return "? " + name;
    if (const auto* a = std::get_if<ir::ArrayType>(&t->body)) {
        return ctype_text(a->elem, name + "[" + (a->count ? std::to_string(*a->count) : std::string()) + "]");
    }
    if (const auto* p = std::get_if<ir::PtrType>(&t->body)) return ctype_text(p->to, "*" + name);
    if (const auto* c = std::get_if<ir::ClassType>(&t->body)) return "struct " + c->name + " " + name;
    const std::string_view k = t->kind();
    static const std::unordered_map<std::string_view, std::string_view> words = {
        {"i8", "signed char"}, {"u8", "unsigned char"}, {"i16", "short"}, {"u16", "unsigned short"}, {"i32", "int"},
        {"u32", "unsigned"}, {"i64", "long long"}, {"u64", "unsigned long long"}, {"f32", "float"}, {"f64", "double"},
        {"bool", "_Bool"}, {"fnptr", "function pointer"}};
    const auto it = words.find(k);
    return std::string(it != words.end() ? it->second : k) + " " + name;
}

namespace {

std::string jv_access_for(std::string_view jt) {
    if (jt == "byte") return "Byte";
    if (jt == "short") return "Short";
    if (jt == "char") return "Char";
    if (jt == "int") return "Int";
    if (jt == "long") return "Long";
    if (jt == "float") return "Float";
    if (jt == "double") return "Double";
    if (jt == "boolean") return "Bool";
    return {};
}

}  // namespace

Faithful::Faithful(std::span<ir::Node* const> unit_nodes, std::string program_name, const Analysis& a, const LibraryCalls* lib)
    : units(unit_nodes.begin(), unit_nodes.end()), name(std::move(program_name)), analysis(a), library(lib) {}

Faithful::~Faithful() = default;

void Faithful::init() {
    if (!units.empty()) {
        const ir::Unit* first = ir::get<ir::Unit>(units.front());
        if (first != nullptr && first->target && first->target->pointer) ptr_size = *first->target->pointer;
    }
    for (ir::Node* un : units) {
        const ir::Unit* u = ir::get<ir::Unit>(un);
        if (u == nullptr) continue;
        const bool is_cpp = u->target && u->target->language == "c++";
        for (ir::Node* t : u->types) {
            if (ir::ClassDecl* c = ir::get<ir::ClassDecl>(t)) {
                records.insert_or_assign(c->qname.value_or(c->name.value_or("")), t);
            } else if (ir::MethodDecl* m = ir::get<ir::MethodDecl>(t)) {
                if (jv_has(m->mods, "unsupported")) throw Refused("a declaration the front end did not lower: " + jv_first_unsupported(t));
                if (ir::present(m->body) && m->sym != nullptr) {
                    const std::string fname = m->name.value_or(m->sym->name);
                    if (ir::MethodDecl** earlier = functions.find(fname)) {
                        if (!is_cpp) throw Refused("two definitions of " + fname);
                        // C++ overloads (a function template's instances among them): Java overloads
                        std::vector<ir::MethodDecl*>& all = overloads[fname];
                        if (all.empty()) all.push_back(*earlier);
                        all.push_back(m);
                        continue;
                    }
                    functions.insert_or_assign(fname, m);
                }
            } else if (ir::FieldDecl* f = ir::get<ir::FieldDecl>(t)) {
                global_decls.push_back(f);
            }
        }
    }
    check_records();
    make_names();
    find_addressed();
}

void Faithful::check_records() {
    for (const auto& [qname, node] : records) {
        const ir::ClassDecl* c = ir::get<ir::ClassDecl>(node);
        const std::string kind = c->kind.value_or("");
        if (kind != "struct" && kind != "union") throw Refused(kind + " " + c->name.value_or(""));
        if (!c->layout) throw Refused("struct " + c->name.value_or("") + " has no layout (bit-fields?)");
    }
}

void Faithful::make_names() {
    std::unordered_set<std::string> taken;
    for (const auto& [fname, fn] : functions) taken.insert(jname(fname));
    for (const ir::FieldDecl* f : global_decls) taken.insert(jname(f->sym->name));
    taken.insert(name);
    for (const auto& [qname, node] : records) {
        const ir::ClassDecl* c = ir::get<ir::ClassDecl>(node);
        std::string base = jname(text::replace_all(c->name.value_or(""), "__unnamed_", "Anon"));
        std::string candidate = jv_upper_first(base);
        while (taken.contains(candidate)) candidate += "_t";
        taken.insert(candidate);
        layout_names.insert_or_assign(qname, candidate);
    }
    used_names = std::move(taken);
}

// Every variable whose address is taken (by & or by decay of an array): it lives in memory.
void Faithful::find_addressed() {
    addr_taken.clear();
    for (ir::Node* u : units) {
        for (ir::Node* n : ir::walk(u)) {
            const ir::Unary* un = ir::get<ir::Unary>(n);
            if (un == nullptr || un->op != "&") continue;
            const ir::Name* nm = ir::get<ir::Name>(strip(un->x));
            if (nm != nullptr && nm->sym != nullptr) addr_taken.insert(key_of(*nm->sym));
        }
    }
}

std::vector<ir::MethodDecl*> Faithful::all_functions() const {
    std::vector<ir::MethodDecl*> out;
    for (const auto& [fname, f] : functions) {
        const auto it = overloads.find(fname);
        if (it == overloads.end() || it->second.empty()) out.push_back(f);
        else out.insert(out.end(), it->second.begin(), it->second.end());
    }
    return out;
}

ir::MethodDecl* Faithful::function_for(const ir::Sym& sym) const {
    ir::MethodDecl* const* only = functions.find(sym.name);
    if (only == nullptr) return nullptr;
    const auto it = overloads.find(sym.name);
    if (it == overloads.end() || it->second.empty()) return *only;
    const std::vector<ir::MethodDecl*>& candidates = it->second;
    std::vector<std::string> want;
    for (const ir::TypeP& p : sym.params) want.push_back(spell(p));
    for (ir::MethodDecl* f : candidates) {
        std::vector<std::string> have;
        for (const ir::Node* p : f->params) have.push_back(spell(jv_param_type(p)));
        if (have == want) return f;
    }
    for (ir::MethodDecl* f : candidates) {
        if (f->params.size() == want.size()) return f;
    }
    return candidates.front();
}

// ---- types

std::string Faithful::jt(const ir::TypeP& type) {
    const CKind k = kind_of(type);
    if (const std::string_view prim = java_primitive(k); !prim.empty()) return std::string(prim);
    switch (k) {
        case CKind::Void: return "void";
        case CKind::Ptr: case CKind::FnPtr: case CKind::Array: case CKind::Class: case CKind::Record: case CKind::Null: return "int";
        default: throw Refused("type " + spell(type));
    }
}

const ir::RecordType& Faithful::layout(const ir::TypeP& type) {
    ir::Node* const* c = records.find(rec_name(type));
    if (c == nullptr) throw Refused("struct " + spell(type) + ": a library type the C runtime for Java does not have");
    return layout_of(ir::get<ir::ClassDecl>(*c)->layout);
}

std::string Faithful::rec_name(const ir::TypeP& type) const {
    if (type && std::holds_alternative<ir::ClassType>(type->body)) return spell(type);
    if (const auto* r = type ? std::get_if<ir::RecordType>(&type->body) : nullptr) return r->name.value_or("");
    return "";
}

std::int64_t Faithful::size(const ir::TypeP& type) {
    const CKind k = kind_of(type);
    if (const int bits = bits_of(k); bits != 0) return k == CKind::Bool ? 1 : bits / 8;
    switch (k) {
        case CKind::F32: return 4;
        case CKind::F64: case CKind::F80: return 8;
        case CKind::Ptr: case CKind::FnPtr: case CKind::Null: return ptr_size;
        case CKind::Array: {
            const auto& a = std::get<ir::ArrayType>(type->body);
            if (!a.count) throw Refused("an array of unknown size");
            return size(a.elem) * static_cast<std::int64_t>(*a.count);
        }
        case CKind::Class: case CKind::Record: return static_cast<std::int64_t>(layout(type).size);
        case CKind::Void: return 1;
        default: throw Refused("size of " + spell(type));
    }
}

std::int64_t Faithful::align(const ir::TypeP& type) {
    const CKind k = kind_of(type);
    if (k == CKind::Array) return align(std::get<ir::ArrayType>(type->body).elem);
    if (k == CKind::Class || k == CKind::Record) {
        std::int64_t most = 1;
        for (const ir::RecordField& f : layout(type).fields) most = std::max(most, align(f.type));
        return most;
    }
    return std::min<std::int64_t>(size(type), 8);
}

std::string Faithful::access(const ir::TypeP& type) {
    const CKind k = kind_of(type);
    if (k == CKind::Ptr || k == CKind::FnPtr || k == CKind::Null) return "Ptr";
    std::string a = jv_access_for(java_primitive(k));
    if (a.empty()) throw Refused("no memory accessor for " + spell(type));
    return a;
}

std::string Faithful::size_code(const ir::TypeP& type) {
    if (is_record(type)) {
        const std::string* n = layout_names.find(rec_name(type));
        if (n == nullptr) throw Refused("struct " + spell(type) + ": a library type the C runtime for Java does not have");
        return *n + ".SIZE";
    }
    return std::to_string(size(type));
}

const ir::RecordField& Faithful::field_off(const ir::TypeP& record, std::string_view field) {
    for (const ir::RecordField& f : layout(record).fields) {
        if (f.name == field) return f;
    }
    throw Refused("no field " + std::string(field) + " in " + spell(record));
}

// ---- the file

std::string Faithful::write() {
    static_init.clear();
    fields_out.clear();
    for (const ir::FieldDecl* g : global_decls) declare_global(*g);
    std::vector<std::vector<std::string>> funcs;
    for (ir::MethodDecl* f : all_functions()) {
        funcs.push_back(function_or_gap(*f, [&] { return FnWriter(*this, f).write(); }));
    }
    for (const ir::FieldDecl* g : global_decls) init_global(*g);
    std::vector<std::string> out = {"// " + name + ".java -- translated from C by CodeBreaker (faithful tier: C memory emulated in crt.Mem).",
                                    "import crt.*;", "", "import static crt.LibC.*;", "", "public final class " + name + " {"};
    std::vector<std::string> record_names;
    for (const auto& [qname, node] : records) record_names.push_back(qname);
    std::sort(record_names.begin(), record_names.end(), [&](const std::string& a, const std::string& b) {
        const std::uint32_t la = records.at(a)->line;
        const std::uint32_t lb = records.at(b)->line;
        return la != lb ? la < lb : a < b;
    });
    for (const std::string& qn : record_names) {
        const std::vector<std::string> lines = layout_class(*ir::get<ir::ClassDecl>(records.at(qn)));
        out.insert(out.end(), lines.begin(), lines.end());
    }
    for (const auto& [fname, constant] : fn_consts) iface(fn_sig(*functions.at(fname)->sym));   // (each registered function's interface, before they are written)
    for (const auto& [sig, iname] : fn_ifaces) {
        std::string ps;
        for (std::size_t i = 0; i < sig.params.size(); ++i) ps += (i != 0 ? ", " : "") + sig.params[i] + " a" + std::to_string(i);
        out.push_back("");
        out.push_back("    interface " + iname + " { " + sig.ret + " call(" + ps + "); }");
    }
    if (!fields_out.empty() || !static_init.empty()) out.push_back("");
    for (const std::string& f : fields_out) out.push_back("    " + f);
    for (const auto& [fname, constant] : fn_consts) {
        const Sig sig = fn_sig(*functions.at(fname)->sym);
        out.push_back("    static final int " + constant + " = Fn.register((" + iface(sig) + ") " + name + "::" + jname(fname) + ");");
    }
    if (!static_init.empty()) {
        out.push_back("");
        out.push_back("    static {");
        for (const std::string& s : static_init) out.push_back("        " + s);
        out.push_back("    }");
    }
    for (const std::vector<std::string>& f : funcs) {
        out.push_back("");
        out.insert(out.end(), f.begin(), f.end());
    }
    const std::vector<std::string> tail = entry();
    out.insert(out.end(), tail.begin(), tail.end());
    out.push_back("}");
    for (const std::string& g : gaps) notes.push_back("gap: " + g);
    return text::join(out, "\n") + "\n";
}

std::vector<std::string> Faithful::function_or_gap(const ir::MethodDecl& f, const std::function<std::vector<std::string>()>& write_fn,
                                                   const std::string& indent, const std::optional<std::string>& owner) {
    try {
        return write_fn();
    } catch (const Refused& refused) {
        const std::string reason = refused.what();
        const std::string fname = f.name.value_or("");
        if (fname == "main" && !owner) throw;   // (a program whose main is a gap is refused)
        const std::optional<std::string> sig = gap_signature(f, owner);
        if (!sig) throw;
        const std::string what = (owner ? *owner + "::" : std::string()) + fname;
        gaps.push_back(what + " (" + reason + ")");
        const std::string message = java_string("CodeBreaker gap: " + what + ": " + reason);
        return {indent + *sig + " {", indent + "    // " + text::replace_all(reason, "\n", " "),
                indent + "    throw new UnsupportedOperationException(" + message + ");", indent + "}"};
    }
}

std::optional<std::string> Faithful::gap_signature(const ir::MethodDecl& f, const std::optional<std::string>&) {
    Sig sig;
    try {
        sig = fn_sig(*f.sym);
    } catch (const Refused&) {
        return std::nullopt;
    }
    std::vector<std::string> names;
    if (sig.params.size() > f.params.size()) names.push_back("result");
    for (const ir::Node* p : f.params) names.push_back(jname(jv_param_name(p)));
    std::string ps;
    for (std::size_t i = 0; i < std::min(sig.params.size(), names.size()); ++i) ps += (i != 0 ? ", " : "") + sig.params[i] + " " + names[i];
    if (f.sym->varargs) ps += (ps.empty() ? "" : ", ") + std::string("Object... va");
    return "static " + sig.ret + " " + jname(f.name.value_or("")) + "(" + ps + ")";
}

std::vector<std::string> Faithful::layout_class(ir::ClassDecl& c) {
    const ir::RecordType& lay = layout_of(c.layout);
    const std::string n = layout_names.at(c.qname.value_or(""));
    const char* what = c.kind == "union" ? "union" : "struct";
    std::string parts = "SIZE = " + std::to_string(lay.size);
    for (const ir::RecordField& f : lay.fields) {
        parts += ", " + (f.name != "SIZE" ? jname(f.name) : std::string("SIZE_")) + " = " + std::to_string(f.offset);
    }
    return {"",
            std::string("    /** ") + what + " " + text::replace_all(c.name.value_or(""), "__unnamed_", "(anonymous) ") +
                ": its size and its fields' offsets in memory. */",
            "    static final class " + n + " {", "        static final int " + parts + ";", "    }"};
}

std::vector<std::string> Faithful::entry() {
    ir::MethodDecl* const* found = functions.find("main");
    if (found == nullptr) throw Refused("no main function");
    const ir::MethodDecl& m = **found;
    std::vector<std::string> out = {"", "    public static void main(String[] args) {"};
    std::string call = "main()";
    if (m.params.size() >= 2) call = "main(args.length + 1, Mem.argv(System.getProperty(\"c.argv0\", \"" + name + "\"), args))";
    if (!m.ret || kind_of(m.ret) == CKind::Void) {
        out.push_back("        " + call + ";");
        out.push_back("        exit(0);");
    } else {
        out.push_back("        exit(" + call + ");");
    }
    out.push_back("    }");
    return out;
}

// ---- globals

void Faithful::declare_global(const ir::FieldDecl& g) {
    const ir::Sym& s = *g.sym;
    const std::string jn = jname(s.name);
    if (jv_has(g.mods, "const") && std::any_of(g.mods.begin(), g.mods.end(), [](const std::string& m) { return m.starts_with("enum:"); })) {
        const ir::Lit* lit = ir::get<ir::Lit>(g.init);
        const auto* number = lit != nullptr ? std::get_if<std::int64_t>(&lit->value) : nullptr;
        if (number == nullptr) throw Refused("an enum constant that is not a number: " + s.name);
        globals[s.name] = Var{jn, false, ir::i32_type(), VarKind::Enum};
        fields_out.push_back("static final int " + jn + " = " + std::to_string(*number) + ";");
        return;
    }
    const ir::TypeP& t = g.type;
    const CKind k = kind_of(t);
    const bool mem = k == CKind::Array || k == CKind::Class || k == CKind::Record || addr_taken.contains(VarKey{true, "", s.name});
    if (const auto earlier = globals.find(s.name); earlier != globals.end() && earlier->second.kind != VarKind::Enum) return;   // (a tentative definition again)
    globals[s.name] = Var{jn, mem, t, VarKind::Global};
    if (mem) {
        fields_out.push_back("static final int " + jn + " = Mem.global(" + size_code(t) + ", " + std::to_string(align(t)) + ");  // " +
                             ctype_text(t, s.name));
        return;
    }
    const std::string jtype = jt(t);
    const std::optional<std::string> constant = g.init != nullptr ? const_value(g.init, t) : std::nullopt;
    if (g.init == nullptr || !constant) fields_out.push_back("static " + jtype + " " + jn + ";");
    else fields_out.push_back("static " + jtype + " " + jn + " = " + *constant + ";");
}

std::optional<std::string> Faithful::const_value(ir::Node* expr, const ir::TypeP& type) {
    expr = strip(expr);
    const ir::Lit* lit = ir::get<ir::Lit>(expr);
    if (const ir::Cast* cast = ir::get<ir::Cast>(expr); cast != nullptr && cast->implicit) {
        // an implicit conversion of a number is that number as the declaration's type
        const ir::Lit* inner = ir::get<ir::Lit>(strip(cast->x));
        const bool number = inner != nullptr && (std::holds_alternative<std::int64_t>(inner->value) ||
                                                 std::holds_alternative<std::uint64_t>(inner->value) || std::holds_alternative<double>(inner->value));
        lit = number ? inner : nullptr;
    }
    if (lit == nullptr) return std::nullopt;
    if (const bool* b = std::get_if<bool>(&lit->value)) return *b ? "true" : "false";
    const std::string jtype = is_int(type) || is_float(type) ? jt(type) : std::string();
    if (const auto* i = std::get_if<std::int64_t>(&lit->value); i != nullptr && (is_int(type) || is_float(type))) {
        return is_float(type) ? float_lit(static_cast<double>(*i), jtype).code : int_lit(*i, jtype).code;
    }
    if (const auto* u = std::get_if<std::uint64_t>(&lit->value); u != nullptr && (is_int(type) || is_float(type))) {
        return is_float(type) ? float_lit(static_cast<double>(*u), jtype).code : int_lit(static_cast<std::int64_t>(*u), jtype).code;
    }
    if (const auto* d = std::get_if<double>(&lit->value); d != nullptr && is_float(type)) return float_lit(*d, jtype).code;
    if (std::holds_alternative<std::monostate>(lit->value) && is_ptr(type)) return "0";
    return std::nullopt;
}

void Faithful::init_global(const ir::FieldDecl& g) {
    const auto found = globals.find(g.sym->name);
    if (found == globals.end() || found->second.kind == VarKind::Enum || g.init == nullptr) return;
    const Var v = found->second;
    if (!v.mem && const_value(g.init, g.type)) return;
    FnWriter w(*this, nullptr);
    if (v.mem) {
        const std::vector<std::string> lines = w.init_place(v.java, g.type, g.init);
        static_init.insert(static_init.end(), lines.begin(), lines.end());
    } else {
        static_init.push_back(v.java + " = " + w.conv(w.value(g.init), g.init->type, g.type).code + ";");
    }
}

// ---- functions

Sig Faithful::fn_sig(const ir::Sym& sym) {
    Sig sig;
    for (const ir::TypeP& p : sym.params) sig.params.push_back(jt(p));
    sig.ret = sym.ret ? jt(sym.ret) : "void";
    if (sym.ret && is_record(sym.ret)) sig.params.insert(sig.params.begin(), "int");
    return sig;
}

Sig Faithful::fn_sig_of_type(const ir::TypeP& fn_or_fnptr) {
    const ir::TypeP* fn_type = &fn_or_fnptr;
    if (const auto* fp = fn_or_fnptr ? std::get_if<ir::FnPtrType>(&fn_or_fnptr->body) : nullptr) fn_type = &fp->fn;
    const auto* f = *fn_type ? std::get_if<ir::FnType>(&(*fn_type)->body) : nullptr;
    if (f == nullptr) throw Refused("a function type was expected: " + spell(fn_or_fnptr));
    Sig sig;
    for (const ir::FnParam& p : f->params) sig.params.push_back(jt(p.type));
    const ir::TypeP& rt = f->result.type;
    sig.ret = rt ? jt(rt) : "void";
    if (rt && is_record(rt)) sig.params.insert(sig.params.begin(), "int");
    return sig;
}

std::string Faithful::iface(const Sig& sig) {
    for (const auto& [known, iname] : fn_ifaces) {
        if (known == sig) return iname;
    }
    std::string n;
    for (const std::string& p : sig.params) n += jv_upper_first(p);
    n += "To" + jv_upper_first(sig.ret);
    fn_ifaces.emplace_back(sig, n);
    return n;
}

std::string Faithful::fn_const(const std::string& function) {
    if (!functions.contains(function)) throw Refused("the address of library function " + function);
    if (!fn_consts.contains(function)) fn_consts.insert_or_assign(function, "FN_" + jname(function));
    return fn_consts.at(function);
}

}  // namespace cb::translate::backends::java
