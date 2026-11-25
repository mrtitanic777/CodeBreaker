// translate/backends/go_java_program.cpp -- reading the tree, the names and types of the Go back end for Java, and the program's classes: what a class
// becomes (see go_java.hpp for the model).

#include <algorithm>
#include <charconv>
#include <cmath>
#include <format>
#include <map>

#include "translate/backends/go_java_internal.hpp"
#include "translate/desugar.hpp"

namespace cb::translate::backends::go_java::detail {

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
    return false;
}

// ---- names ------------------------------------------------------------------------------------------------------------------

namespace {
const std::set<std::string_view>& go_keywords() {
    static const std::set<std::string_view> list = {"break", "default", "func", "interface", "select", "case", "defer", "go", "map", "struct", "chan", "else", "goto",
                                                    "package", "switch", "const", "fallthrough", "if", "range", "type", "continue", "for", "import", "return", "var"};
    return list;
}
}  // namespace

std::string field_ident(std::string_view name) {
    std::string out;
    for (const char c : name) out.push_back(c == '$' ? '_' : c);
    if (go_keywords().contains(out) || out == "self" || out == "_") out.push_back('_');
    return out;
}

std::string ident(std::string_view name) {
    static const std::set<std::string_view> reserved = {"any", "bool", "byte", "cap", "clear", "close", "complex", "copy", "delete", "error", "false", "float32", "float64", "imag",
                                                        "int", "int8", "int16", "int32", "int64", "iota", "len", "make", "max", "min", "new", "nil", "panic", "print", "println",
                                                        "real", "recover", "rune", "string", "true", "uint", "uint8", "uint16", "uint32", "uint64", "uintptr", "append",
                                                        "jrt", "math", "this", "self", "main", "init", "flow", "retv", "ex", "ok", "_"};
    std::string out = field_ident(name);
    if (reserved.contains(out)) out.push_back('_');
    return out;
}

std::string capital(std::string_view name) {
    std::string out = field_ident(name);
    if (!out.empty() && out[0] >= 'a' && out[0] <= 'z') out[0] = static_cast<char>(out[0] - 'a' + 'A');
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

std::string indent_of(std::size_t depth) { return std::string(depth, '\t'); }

std::string short_code(const ir::TypeP& type) {
    const std::string_view k = kind_of(type);
    if (is_prim_kind(k)) return std::string(k == "u16" ? "char" : k);
    if (k == "array") return short_code(elem_of(type)) + "_arr";
    if (k == "class") {
        const std::string_view n = class_name(type);
        if (n == "java.lang.String") return "str";
        const std::size_t dot = n.rfind('.');
        std::string s(dot == std::string_view::npos ? n : n.substr(dot + 1));
        for (char& c : s)
            if (c == '$') c = '_';
        return s;
    }
    return std::string(k);
}

std::string go_string(std::string_view s) {
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
                if (cp < 0x20 || cp == 0x7F) out += std::format("\\x{:02x}", cp);
                else out.append(s.substr(i - len, len));
        }
    }
    out += '"';
    return out;
}

std::string float_text(double value, bool single) {
    char buf[64];
    const auto res = single ? std::to_chars(buf, buf + sizeof buf, static_cast<float>(value)) : std::to_chars(buf, buf + sizeof buf, value);
    std::string s(buf, res.ptr);
    if (s.find('.') == std::string::npos && s.find('e') == std::string::npos) s += ".0";
    return s;
}

std::string typed_text(const Code& c) {
    if (!c.bare) return c.text;
    const std::string_view k = kind_of(c.type);
    std::string t = k == "u16" ? "uint16" : k == "bool" ? "bool" : std::string(k == "i32" ? "int32" : k == "i64" ? "int64" : k == "i16" ? "int16" : k == "i8" ? "int8" : k == "f32" ? "float32" : "float64");
    return t + "(" + c.text + ")";
}

// ---- the program ------------------------------------------------------------------------------------------------------------

namespace {

bool is_abstract(const ClassInfo& cls) { return cls.kind == "interface" || has_mod(cls.decl->mods, "abstract"); }

std::string binary_name(const ClassInfo& cls) {
    if (cls.outer == nullptr || !cls.qname.starts_with(cls.outer->qname + ".")) return cls.qname;
    return binary_name(*cls.outer) + "$" + cls.qname.substr(cls.outer->qname.size() + 1);
}

bool is_field_static(const ir::FieldDecl& f) { return has_mod(f.mods, "static") || (f.sym && f.sym->is_static); }

}  // namespace

Program::Program(Arena& arena, std::span<ir::Node* const> units, std::string main) : arena_(arena), main_name_(std::move(main)), units_(units.begin(), units.end()) {
    const desugar::Form keep[] = {desugar::Form::ForEach};
    desugar::run(arena, units, keep);
    model_ = std::make_unique<jmodel::Model>(units);
    model_->name_anonymous();
    check_supported();
    std::map<std::string, std::set<std::string>> signatures;   // a method name the program declares with more than one parameter list is written with its parameter codes
    for (const auto& [qname, cls] : model_->classes()) {
        for (const ir::Node* mn : cls->methods()) {
            const ir::MethodDecl& m = method_of(mn);
            if (m.kind != "method") continue;
            std::vector<std::string> codes;
            for (const ir::Node* pn : m.params) codes.push_back(short_code(jmodel::erase(param_of(pn).type)));
            signatures[m.name.value_or("")].insert(join(codes));
        }
    }
    for (const char* object_method : {"toString", "hashCode"}) signatures[object_method].insert("");
    signatures["equals"].insert("object");
    for (const auto& [name, sigs] : signatures)
        if (sigs.size() > 1) overloaded_.insert(name);
    // a class with nothing but static members has no instances: it is only a prefix for its functions and variables
    for (const auto& [qname, cls] : model_->classes()) {
        if (cls->kind != "class" || cls->base.value_or("java.lang.Object") != "java.lang.Object" || !cls->ifaces.empty() || is_abstract(*cls)) continue;
        if (model_->subclassed(qname)) continue;
        bool plain = true;
        for (const ir::Node* member : cls->members()) {
            if (const auto* f = ir::get<ir::FieldDecl>(member); f != nullptr && !is_field_static(*f)) plain = false;
            if (const auto* m = ir::get<ir::MethodDecl>(member)) {
                if (m->kind == "method" && !is_static(*m)) plain = false;
                if (m->kind == "init") plain = false;
                if (m->kind == "ctor" && !m->params.empty()) plain = false;
            }
        }
        if (!plain) continue;
        const auto mentions = [&](const ir::TypeP& t) {
            std::vector<const ir::Type*> todo;
            if (t) todo.push_back(t.get());
            while (!todo.empty()) {
                const ir::Type* x = todo.back();
                todo.pop_back();
                if (const auto* c = std::get_if<ir::ClassType>(&x->body)) {
                    if (c->name == qname) return true;
                    for (const ir::TypeP& a : c->args) todo.push_back(a.get());
                } else if (const auto* a = std::get_if<ir::ArrayType>(&x->body)) {
                    if (a->elem) todo.push_back(a->elem.get());
                }
            }
            return false;
        };
        for (ir::Node* unit : units) {
            for_each_node(unit, [&](const ir::Node* n) {
                if (!plain || ir::is<ir::TypeName>(n)) return;
                if (const auto* nw = ir::get<ir::New>(n); nw != nullptr && nw->sym && nw->sym->owner == qname) plain = false;
                if (mentions(n->type)) plain = false;
                if (const auto* l = ir::get<ir::Local>(n); l != nullptr && mentions(l->type)) plain = false;
                if (const auto* p = ir::get<ir::Param>(n); p != nullptr && mentions(p->type)) plain = false;
                if (const auto* f = ir::get<ir::FieldDecl>(n); f != nullptr && mentions(f->type)) plain = false;
                if (const auto* m = ir::get<ir::MethodDecl>(n); m != nullptr && mentions(m->ret)) plain = false;
            });
        }
        if (plain) static_only_.insert(qname);
    }
    find_main();
}

void Program::check_supported() {
    for (const auto& [qname, cls] : model_->classes()) {
        if (qname.starts_with("java.")) throw Decline("the library class " + qname + " (a getClass() or a type the runtime has no word for)");
        if (cls->local || cls->anon) throw Decline("a local or anonymous class (" + qname + ")");
        if (cls->kind == "record" || cls->kind == "annotation") throw Decline("a " + cls->kind + " (" + qname + ")");
        for (const ir::Node* member : cls->members())
            if (const auto* ec = ir::get<ir::EnumConst>(member); ec != nullptr && ec->body != nullptr) throw Decline("an enum constant with a body of its own (" + qname + ")");
        if (cls->inner || cls->needs_outer) throw Decline("an inner class that holds its outer instance (" + qname + ")");
        if (cls->kind != "enum" && cls->base && *cls->base != "java.lang.Object" && model_->info(*cls->base) == nullptr && !is_lib_throwable(*cls->base))
            throw Decline("a class extending the library class " + *cls->base);
        for (const std::string& i : cls->ifaces)
            if (model_->info(i) == nullptr) throw Decline("a class implementing the library interface " + i);
        for (const ir::Node* mn : cls->methods()) {
            const ir::MethodDecl& m = method_of(mn);
            const std::string kind = m.kind.value_or("method");
            if (kind != "method" && kind != "ctor" && kind != "static_init" && kind != "init") throw Decline("a member of kind " + kind);
        }
        for (const ir::Node* member : cls->members())
            if (ir::is<ir::Property>(member)) throw Decline("a property");
    }
}

void Program::find_main() {
    std::vector<const ClassInfo*> found;
    for (const auto& [qname, cls] : model_->classes()) {
        for (const ir::Node* mn : cls->methods("main")) {
            const ir::MethodDecl& m = method_of(mn);
            if (!is_static(m) || m.params.size() != 1) continue;
            const ir::TypeP t = param_of(m.params[0]).type;
            if (kind_of(t) == "array" && class_name(elem_of(t)) == "java.lang.String") found.push_back(cls);
        }
    }
    if (!main_name_.empty()) {
        for (const ClassInfo* c : found) {
            if (c->qname == main_name_ || c->cname == main_name_ || (c->qname.size() > main_name_.size() && c->qname.ends_with("." + main_name_))) {
                main_cls_ = c;
                return;
            }
        }
        throw Decline("no main method in the class " + main_name_);
    }
    if (found.size() != 1) throw Decline(found.empty() ? "no class has a main method" : "several classes have a main method");
    main_cls_ = found[0];
}

// ---- types ---------------------------------------------------------------------------------------------------------------------

bool Program::is_lib_throwable(std::string_view qname) const {
    if (model_->info(qname) != nullptr) return false;
    return qname == "java.lang.Throwable" || model_->is_sub(qname, "java.lang.Throwable");
}
bool Program::is_throwable_type(std::string_view qname) const { return qname == "java.lang.Throwable" || model_->is_sub(qname, "java.lang.Throwable"); }
const ClassInfo* Program::program_class(const ir::TypeP& type) const {
    if (kind_of(type) != "class") return nullptr;
    return model_->info(class_name(type));
}

std::string Program::gtype(const ir::TypeP& type) const {
    const std::string_view k = kind_of(type);
    if (k == "bool") return "bool";
    if (k == "i8") return "int8";
    if (k == "i16") return "int16";
    if (k == "i32") return "int32";
    if (k == "i64") return "int64";
    if (k == "f32") return "float32";
    if (k == "f64") return "float64";
    if (k == "u16") return "uint16";
    if (k == "void" || k.empty()) return "";
    if (k == "array") return "[]" + gtype(elem_of(type));
    if (k == "typevar" || k == "wildcard") return "any";
    if (k == "null") return "any";
    if (k == "class") {
        const std::string& n = std::get<ir::ClassType>(type->body).name;
        if (n == "java.lang.String") return "jrt.String";
        if (n == "java.lang.Object" || n == "java.lang.Number" || n == "java.lang.Comparable" || n == "java.lang.CharSequence") return "any";
        if (!boxed_kind(type).empty()) return "any";
        if (n == "java.lang.StringBuilder") return "*jrt.StringBuilder";
        if (n == "java.io.PrintStream") return "*jrt.PrintStream";
        if (model_->info(n) == nullptr) {
            if (n == "java.util.List" || n == "java.util.ArrayList" || n == "java.util.LinkedList") return "jrt.List";
            if (n == "java.util.Collection" || n == "java.lang.Iterable") return "jrt.Collection";
            if (n == "java.util.Set" || n == "java.util.HashSet" || n == "java.util.LinkedHashSet") return "jrt.Set";
            if (n == "java.util.Map" || n == "java.util.HashMap" || n == "java.util.LinkedHashMap") return "jrt.Map";
            if (n == "java.util.Iterator") return "jrt.Iterator";
            if (n == "java.util.Map.Entry") return "jrt.Entry";
            if (is_lib_throwable(n)) return "jrt.Throwable";
            throw Decline("the library type " + n);
        }
        const ClassInfo* cls = model_->info(n);
        if (static_only(*cls)) throw Decline("a type of the class " + n + ", which has only static members");
        return iface(*cls);
    }
    throw Decline("a type of kind " + std::string(k));
}

// ---- classes -------------------------------------------------------------------------------------------------------------------

bool Program::static_only(const ClassInfo& cls) const { return static_only_.contains(cls.qname); }
const ClassInfo* Program::base_class(const ClassInfo& cls) const { return cls.base ? model_->info(*cls.base) : nullptr; }
std::vector<const ClassInfo*> Program::ancestors(const ClassInfo& cls) const {
    std::vector<const ClassInfo*> out;
    for (const std::string& q : model_->ancestors(cls.qname)) {
        const ClassInfo* c = model_->info(q);
        if (c != nullptr && c != &cls) out.push_back(c);
    }
    return out;
}
bool Program::is_concrete(const ClassInfo& cls) const { return (cls.kind == "class" || cls.kind == "enum") && !is_abstract(cls) && !static_only(cls); }

const ClassInfo* Program::declaring_class(const ClassInfo& cls, std::string_view name) const {
    const auto declares = [&](const ClassInfo& c) {
        for (const ir::Node* m : c.members())
            if (const auto* f = ir::get<ir::FieldDecl>(m); f != nullptr && f->sym && f->sym->name == name) return true;
        return false;
    };
    if (declares(cls)) return &cls;
    for (const ClassInfo* a : ancestors(cls))
        if (declares(*a)) return a;
    return nullptr;
}

// ---- methods -------------------------------------------------------------------------------------------------------------------

bool Program::is_object_method(const ir::MethodDecl& m) const {
    const std::string n = m.name.value_or("");
    if (n == "toString" || n == "hashCode") return m.params.empty();
    if (n == "equals") return m.params.size() == 1 && class_name(param_of(m.params[0]).type) == "java.lang.Object";
    return false;
}

std::string Program::mname(const ir::MethodDecl& m) const {
    std::string base = capital(m.name.value_or(""));
    if (is_object_method(m) || m.params.empty() || !overloaded_.contains(m.name.value_or(""))) return base;
    std::vector<std::string> codes;
    for (const ir::Node* pn : m.params) codes.push_back(short_code(jmodel::erase(param_of(pn).type)));
    return base + "_" + join(codes, "_");
}

bool Program::introduces(const ClassInfo& cls, const ir::MethodDecl& m) const {
    if (is_static(m) || is_private(m) || m.kind != "method" || is_object_method(m)) return false;
    const std::string rn = mname(m);
    for (const ClassInfo* a : ancestors(cls)) {
        for (const ir::Node* other : a->methods()) {
            const ir::MethodDecl& o = method_of(other);
            if (o.kind == "method" && !is_static(o) && !is_private(o) && !is_object_method(o) && mname(o) == rn) return false;
        }
    }
    return true;
}

jmodel::Override Program::signature(const ClassInfo& cls, const ir::Node* method) const {
    if (const auto ov = model_->overrides(cls, method)) return *ov;
    const ir::MethodDecl& m = method_of(method);
    jmodel::Override sig;
    for (const ir::Node* pn : m.params) sig.params.push_back(jmodel::erase(param_of(pn).type));
    sig.result = jmodel::erase(m.ret);
    return sig;
}

std::string Program::result_text(const jmodel::Override& sig) const {
    const std::string r = gtype(sig.result);
    return r.empty() ? "" : " " + r;
}

// The parameter list of a method: `a int32, b jrt.String` (or just the types).
std::string Program::param_list(const ClassInfo& cls, const ir::Node* method, bool names) const {
    const ir::MethodDecl& m = method_of(method);
    const jmodel::Override sig = signature(cls, method);
    std::vector<std::string> out;
    for (std::size_t i = 0; i < m.params.size(); ++i) {
        const ir::Param& p = param_of(m.params[i]);
        const std::string root = gtype(sig.params.at(i));
        if (!names) out.push_back(root);
        else out.push_back(ident(need_sym(p.sym, "a parameter").name) + (root != gtype(p.type) ? "_" : "") + " " + root);
    }
    return join(out);
}

std::string Program::ctor_suffix(const ClassInfo& cls, const ir::Node* ctor) const {
    std::vector<const ir::Node*> ctors;
    for (const ir::Node* mn : cls.methods())
        if (method_of(mn).kind == "ctor") ctors.push_back(mn);
    if (ctors.size() <= 1) return "";
    for (std::size_t i = 0; i < ctors.size(); ++i)
        if (ctors[i] == ctor) return std::to_string(i);
    throw Decline("a constructor of " + cls.qname + " that is not one of its own");
}

bool Program::is_const_field(const ir::FieldDecl& f) const {
    if (!is_field_static(f) || !has_mod(f.mods, "final") || f.init == nullptr || !jmodel::constant_init(f)) return false;
    const std::string_view k = kind_of(f.type);
    return is_prim_kind(k) || class_name(f.type) == "java.lang.String";
}

// ---- writing -------------------------------------------------------------------------------------------------------------------

std::string Program::write() {
    std::string out;
    const auto put = [&](const Lines& lines) {
        for (const std::string& l : lines) out += l + "\n";
        out += "\n";
    };
    Lines statics;   // package-level variables
    std::vector<Lines> classes;
    for (const auto& [qname, cls] : model_->classes()) classes.push_back(class_text(*cls));
    Lines clinit = {"func clinit() {"};
    for (const auto& [qname, cls] : model_->classes()) static_init_text(*cls, clinit);
    clinit.push_back("}");
    std::string main_call;
    for (const ir::Node* mn : main_cls_->methods("main")) {
        const ir::MethodDecl& m = method_of(mn);
        if (is_static(m) && m.params.size() == 1) main_call = static_name(*main_cls_, mname(m));
    }
    out += "// " + main_cls_->cname + ".go -- the Go translation of the Java program whose main is " + main_cls_->qname + " (CodeBreaker, backends/go_java).\n";
    out += "// It runs on jrt, the Java runtime in jrt/: see the header of jrt/jrt.go for how Java's behaviour is kept.\n";
    out += "package main\n\nimport (\n";
    if (math_) out += "\t\"math\"\n\n";
    out += "\t\"javaprog/jrt\"\n)\n\n";
    for (const Lines& c : classes)
        if (!c.empty()) put(c);
    put(clinit);
    out += "func main() {\n\tjrt.Run(func() {\n\t\tclinit()\n\t\t" + main_call + "([]jrt.String{})\n\t})\n}\n";
    return out;
}

Lines Program::class_text(const ClassInfo& cls) {
    Lines out;
    const auto add = [&](const Lines& l) {
        if (l.empty()) return;
        if (!out.empty()) out.push_back("");
        out.insert(out.end(), l.begin(), l.end());
    };
    // static fields
    Lines vars;
    for (const ir::Node* member : cls.members()) {
        const auto* f = ir::get<ir::FieldDecl>(member);
        if (f == nullptr || !is_field_static(*f)) continue;
        std::string line = "var " + static_name(cls, field_ident(f->sym->name)) + " " + gtype(f->type);
        if (is_const_field(*f)) {
            Writer w(*this, &cls, nullptr);
            w.make_static();
            line += " = " + w.value_of(f->init, f->type);
        }
        vars.push_back(line);
    }
    if (cls.kind == "enum") {
        for (const ir::Node* member : cls.members())
            if (const auto* ec = ir::get<ir::EnumConst>(member)) vars.push_back("var " + static_name(cls, field_ident(ec->sym->name)) + " " + iface(cls));
    }
    add(vars);
    if (static_only(cls)) {
        for (const ir::Node* mn : cls.methods())
            if (method_of(mn).kind == "method") add(method_text(cls, mn));
        return out;
    }
    add(interface_text(cls));
    if (cls.kind == "interface") {
        add(default_method_text(cls));
        for (const ir::Node* mn : cls.methods())
            if (method_of(mn).kind == "method" && is_static(method_of(mn))) add(method_text(cls, mn));
        return out;
    }
    add(struct_text(cls));
    for (const ir::Node* mn : cls.methods()) {
        const ir::MethodDecl& m = method_of(mn);
        if (m.kind == "method") add(method_text(cls, mn));
        else if (m.kind == "ctor") add(ctor_text(cls, mn));
    }
    if (cls.kind == "enum") add(enum_text(cls));
    for (const ir::Node* mn : cls.methods()) {
        if (method_of(mn).kind == "ctor" && is_concrete(cls)) add(new_text(cls, mn));
    }
    // default methods of the interfaces it implements, for the ones nothing above supplies
    Lines defaults;
    for (const std::string& iname : cls.ifaces) {
        const ClassInfo* ic = model_->info(iname);
        if (ic == nullptr) continue;
        for (const ClassInfo* base = base_class(cls); base != nullptr && ic != nullptr; base = base_class(*base))
            if (model_->is_sub(base->qname, ic->qname)) ic = nullptr;
        if (ic == nullptr) continue;
        std::vector<const ClassInfo*> chain = {ic};
        for (const ClassInfo* a : ancestors(*ic)) chain.push_back(a);
        for (const ClassInfo* d : chain) {
            if (d->kind != "interface") continue;
            for (const ir::Node* mn : d->methods()) {
                const ir::MethodDecl& m = method_of(mn);
                if (m.kind != "method" || is_static(m) || !ir::present(m.body)) continue;
                bool own = false;
                for (const ClassInfo* c = &cls; c != nullptr && !own; c = base_class(*c))
                    for (const ir::Node* other : c->methods())
                        if (method_of(other).kind == "method" && !is_static(method_of(other)) && ir::present(method_of(other).body) && mname(method_of(other)) == mname(m)) own = true;
                if (own) continue;
                const jmodel::Override sig = signature(*d, mn);
                std::vector<std::string> args = {"this.self"};
                std::vector<std::string> ps;
                for (std::size_t i = 0; i < m.params.size(); ++i) {
                    const std::string pn = "a" + std::to_string(i);
                    ps.push_back(pn + " " + gtype(sig.params.at(i)));
                    args.push_back(pn);
                }
                const bool unit = gtype(sig.result).empty();
                defaults.push_back("func (this *" + obj(cls) + ") " + mname(m) + "(" + join(ps) + ")" + result_text(sig) + " { " + (unit ? "" : "return ") + static_name(*d, mname(m)) + "_default(" +
                                   join(args) + ") }");
            }
        }
    }
    add(defaults);
    return out;
}

Lines Program::interface_text(const ClassInfo& cls) {
    Lines out = {"type " + iface(cls) + " interface {"};
    if (const ClassInfo* base = base_class(cls)) out.push_back("\t" + iface(*base));
    else if (cls.base && is_lib_throwable(*cls.base)) out.push_back("\tjrt.Throwable");
    for (const std::string& i : cls.ifaces) out.push_back("\t" + iface(*model_->info(i)));
    if (cls.kind == "interface") out.push_back("\t" + marker(cls) + "()");
    else out.push_back("\t" + accessor(cls) + "() *" + obj(cls));
    if (cls.kind == "enum") {
        out.push_back("\tName() jrt.String");
        out.push_back("\tOrdinal() int32");
        out.push_back("\tCompareTo(o any) int32");
    }
    std::set<std::string> seen;
    for (const ir::Node* mn : cls.methods()) {
        const ir::MethodDecl& m = method_of(mn);
        if (m.kind != "method" || !introduces(cls, m) || !seen.insert(mname(m)).second) continue;
        const jmodel::Override sig = signature(cls, mn);
        out.push_back("\t" + mname(m) + "(" + param_list(cls, mn, false) + ")" + result_text(sig));
    }
    out.push_back("}");
    return out;
}

Lines Program::default_method_text(const ClassInfo& cls) {
    Lines out;
    for (const ir::Node* mn : cls.methods()) {
        const ir::MethodDecl& m = method_of(mn);
        if (m.kind != "method" || is_static(m) || !ir::present(m.body)) continue;
        Writer w(*this, &cls, &m);
        const std::vector<ir::Node*> stmts = body_of(m);
        w.find_reads(stmts);
        const jmodel::Override sig = signature(cls, mn);
        std::vector<std::string> ps = {"this " + iface(cls)};
        for (std::size_t i = 0; i < m.params.size(); ++i) ps.push_back(ident(param_of(m.params[i]).sym->name) + " " + gtype(sig.params.at(i)));
        out.push_back("func " + static_name(cls, mname(m)) + "_default(" + join(ps) + ")" + result_text(sig) + " {");
        const Lines body = w.body(stmts, m.ret, 1);
        out.insert(out.end(), body.begin(), body.end());
        out.push_back("}");
        out.push_back("");
    }
    if (!out.empty()) out.pop_back();
    return out;
}

Lines Program::struct_text(const ClassInfo& cls) {
    Lines out = {"type " + obj(cls) + " struct {"};
    if (const ClassInfo* base = base_class(cls)) out.push_back("\t" + obj(*base));
    else if (cls.base && is_lib_throwable(*cls.base)) out.push_back("\tjrt.ThrowableBase");
    out.push_back("\tself " + iface(cls));
    if (cls.kind == "enum") {
        out.push_back("\tname_ jrt.String");
        out.push_back("\tordinal_ int32");
    }
    for (const ir::Node* member : cls.members()) {
        const auto* f = ir::get<ir::FieldDecl>(member);
        if (f == nullptr || is_field_static(*f)) continue;
        out.push_back("\t" + field_ident(need_sym(f->sym, "a field").name) + " " + gtype(f->type));
    }
    out.push_back("}");
    out.push_back("");
    out.push_back("func (this *" + obj(cls) + ") " + accessor(cls) + "() *" + obj(cls) + " { return this }");
    {   // the marker of each interface it implements that no class above it already does
        std::set<std::string> done;
        for (const ClassInfo* a : ancestors(cls)) {
            if (a->kind != "interface" || !done.insert(a->qname).second) continue;
            const ClassInfo* base = base_class(cls);
            if (base != nullptr && model_->is_sub(base->qname, a->qname)) continue;
            out.push_back("func (this *" + obj(cls) + ") " + marker(*a) + "() {}");
        }
    }
    out.push_back("func (this *" + obj(cls) + ") ClassName() string { return " + go_string(binary_name(cls)) + " }");
    return out;
}

Lines Program::method_text(const ClassInfo& cls, const ir::Node* mn) {
    const ir::MethodDecl& m = method_of(mn);
    if (!ir::present(m.body)) return {};
    Writer w(*this, &cls, &m);
    const std::vector<ir::Node*> stmts = body_of(m);
    w.find_reads(stmts);
    const bool stat = is_static(m);
    if (stat) w.make_static();
    const jmodel::Override sig = signature(cls, mn);
    std::string head = stat ? "func " + static_name(cls, mname(m)) : "func (this *" + obj(cls) + ") " + mname(m);
    head += "(" + param_list(cls, mn, true) + ")" + result_text(sig) + " {";
    Lines out = {head};
    for (std::size_t i = 0; i < m.params.size(); ++i) {   // a parameter the root declares as Object but this method types more closely
        const ir::Param& p = param_of(m.params[i]);
        const std::string root = gtype(sig.params.at(i)), own = gtype(p.type);
        if (root == own) continue;
        const std::string n = ident(p.sym->name);
        out.push_back("\t" + n + " := " + (own == "jrt.String" ? "jrt.AsString(" + n + "_)" : (is_prim_kind(kind_of(p.type)) ? n + "_.(" + own + ")" : "jrt.As[" + own + "](" + n + "_)")));
    }
    const Lines body = w.body(stmts, m.ret, 1);
    out.insert(out.end(), body.begin(), body.end());
    out.push_back("}");
    return out;
}

Lines Program::ctor_text(const ClassInfo& cls, const ir::Node* cn) {
    const ir::MethodDecl& ctor = method_of(cn);
    Writer w(*this, &cls, &ctor);
    std::vector<ir::Node*> stmts = body_of(ctor);
    w.find_reads(stmts);
    std::string ps;
    {
        std::vector<std::string> list;
        for (const ir::Node* pn : ctor.params) list.push_back(ident(need_sym(param_of(pn).sym, "a parameter").name) + " " + gtype(param_of(pn).type));
        ps = join(list);
    }
    Lines out = {"func (this *" + obj(cls) + ") ctor" + ctor_suffix(cls, cn) + "(" + ps + ") {"};
    bool runs_fields = true;
    std::span<ir::Node* const> rest(stmts);
    if (!stmts.empty()) {
        if (const auto* call = ir::get<ir::CtorCall>(stmts[0])) {
            for (const std::string& l : w.ctor_call(*call)) out.push_back("\t" + l);
            runs_fields = !(call->kind && call->kind->starts_with("this"));
            rest = rest.subspan(1);
        }
    }
    if (runs_fields) {   // the field initialisers and initialiser blocks, in the order they are written
        for (const ir::Node* member : cls.members()) {
            if (const auto* f = ir::get<ir::FieldDecl>(member); f != nullptr && f->init != nullptr && !is_field_static(*f)) {
                Writer fw(*this, &cls, &ctor);
                out.push_back("\tthis." + field_ident(f->sym->name) + " = " + fw.value_of(f->init, f->type));
            } else if (const auto* init = ir::get<ir::MethodDecl>(member); init != nullptr && init->kind == "init" && ir::present(init->body)) {
                Writer iw(*this, &cls, init);
                const std::vector<ir::Node*> body = body_of(*init);
                iw.find_reads(body);
                out.push_back("\t{");
                for (const std::string& l : iw.body(body, ir::void_type(), 2)) out.push_back(l);
                out.push_back("\t}");
            }
        }
    }
    const Lines lines = w.body(rest, ir::void_type(), 1);
    out.insert(out.end(), lines.begin(), lines.end());
    out.push_back("}");
    return out;
}

Lines Program::new_text(const ClassInfo& cls, const ir::Node* cn) {
    const ir::MethodDecl& ctor = method_of(cn);
    std::vector<std::string> ps, args;
    if (cls.kind == "enum") {
        ps = {"name jrt.String", "ordinal int32"};
    }
    for (const ir::Node* pn : ctor.params) {
        const std::string n = ident(param_of(pn).sym->name);
        ps.push_back(n + " " + gtype(param_of(pn).type));
        args.push_back(n);
    }
    const std::string suffix = ctor_suffix(cls, cn);
    Lines out = {"func New" + cls.cname + suffix + "(" + join(ps) + ") " + iface(cls) + " {", "\tthis := &" + obj(cls) + "{}", "\tthis.self = this"};
    for (const ClassInfo* a = base_class(cls); a != nullptr; a = base_class(*a)) out.push_back("\tthis." + obj(*a) + ".self = this");
    if (cls.kind == "enum") {
        out.push_back("\tthis.name_ = name");
        out.push_back("\tthis.ordinal_ = ordinal");
    }
    out.push_back("\tthis.ctor" + suffix + "(" + join(args) + ")");
    out.push_back("\treturn this");
    out.push_back("}");
    return out;
}

Lines Program::enum_text(const ClassInfo& cls) {
    Lines out;
    const std::string o = "func (this *" + obj(cls) + ") ";
    out.push_back(o + "Name() jrt.String { return this.name_ }");
    out.push_back(o + "Ordinal() int32 { return this.ordinal_ }");
    bool user_to_string = false;
    for (const ir::Node* mn : cls.methods("toString")) user_to_string = user_to_string || (method_of(mn).params.empty() && ir::present(method_of(mn).body));
    if (!user_to_string) out.push_back(o + "ToString() jrt.String { return this.name_ }");
    out.push_back(o + "CompareTo(o any) int32 { return this.ordinal_ - o.(" + iface(cls) + ").Ordinal() }");
    out.push_back("");
    std::vector<std::string> all;
    for (const ir::Node* member : cls.members())
        if (const auto* ec = ir::get<ir::EnumConst>(member)) all.push_back(static_name(cls, field_ident(ec->sym->name)));
    out.push_back("func " + static_name(cls, "values") + "() []" + iface(cls) + " { return []" + iface(cls) + "{" + join(all) + "} }");
    out.push_back("");
    out.push_back("func " + static_name(cls, "valueOf") + "(name jrt.String) " + iface(cls) + " {");
    out.push_back("\tfor _, c := range " + static_name(cls, "values") + "() {");
    out.push_back("\t\tif c.Name().Equals(name) {\n\t\t\treturn c\n\t\t}");
    out.push_back("\t}");
    out.push_back("\tjrt.ThrowMsg(\"java.lang.IllegalArgumentException\", \"No enum constant " + cls.qname + ".\"+name.GoString())");
    out.push_back("\treturn nil");
    out.push_back("}");
    return out;
}

// The static initialisation of a class: enum constants first, then static field initialisers and static blocks in the order written.
void Program::static_init_text(const ClassInfo& cls, Lines& out) {
    std::int32_t ordinal = 0;
    for (const ir::Node* member : cls.members()) {
        const auto* ec = ir::get<ir::EnumConst>(member);
        if (ec == nullptr) continue;
        Writer w(*this, &cls, nullptr);
        w.make_static();
        const auto match = model_->method_decl(need_sym(ec->ctor, "an enum constructor"));
        if (!match) throw Decline("the constructor of the enum constant " + ec->sym->name);
        std::vector<ir::TypeP> params;
        for (const ir::Node* pn : method_of(match->method).params) params.push_back(param_of(pn).type);
        std::vector<std::string> args = {"jrt.S(" + go_string(ec->sym->name) + ")", std::to_string(ordinal++)};
        for (const std::string& a : w.call_args(ec->args, params)) args.push_back(a);
        out.push_back("\t" + static_name(cls, field_ident(ec->sym->name)) + " = New" + cls.cname + ctor_suffix(cls, match->method) + "(" + join(args) + ")");
    }
    for (const ir::Node* member : cls.members()) {
        if (const auto* f = ir::get<ir::FieldDecl>(member); f != nullptr && f->init != nullptr && is_field_static(*f) && !is_const_field(*f)) {
            Writer w(*this, &cls, nullptr);
            w.make_static();
            out.push_back("\t" + static_name(cls, field_ident(f->sym->name)) + " = " + w.value_of(f->init, f->type));
        } else if (const auto* init = ir::get<ir::MethodDecl>(member); init != nullptr && init->kind == "static_init" && ir::present(init->body)) {
            Writer w(*this, &cls, init);
            w.make_static();
            const std::vector<ir::Node*> body = body_of(*init);
            w.find_reads(body);
            out.push_back("\t{");
            for (const std::string& l : w.body(body, ir::void_type(), 2)) out.push_back(l);
            out.push_back("\t}");
        }
    }
}

}  // namespace cb::translate::backends::go_java::detail
