// java/frontend.cpp -- see frontend.hpp.
#include "java/frontend.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include <format>
#include <limits>
#include <set>

#include <nlohmann/json.hpp>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/process.hpp"
#include "core/text.hpp"

namespace cb::java::frontend {

namespace {

namespace ir = translate::ir;
namespace stdfs = std::filesystem;
using ir::Node;
using ir::SymP;
using ir::TypeP;

constexpr std::string_view kReader = "javac tree dump";

// ---- reading the dump -------------------------------------------------------------------------------------------------------

// A member that may be absent: null when it is, or when the dump wrote null.
const Json* jfe_opt(const Json& j, const char* key) {
    if (!j.is_object()) return nullptr;
    const auto it = j.find(key);
    return it == j.end() || it->is_null() ? nullptr : &*it;
}

[[noreturn]] void jfe_bad(const Json& at, const std::string& what) {
    std::uint32_t line = 0;
    if (at.is_object()) {
        if (const Json* l = jfe_opt(at, "l"); l != nullptr && l->is_number_unsigned()) line = l->get<std::uint32_t>();
    }
    throw ParseError(kReader, line, 0, what);
}

const Json& jfe_req(const Json& j, const char* key) {
    const Json* v = jfe_opt(j, key);
    if (v == nullptr) jfe_bad(j, std::format("a node lacks \"{}\"", key));
    return *v;
}

std::string jfe_text(const Json& j, const char* key) {
    const Json& v = jfe_req(j, key);
    if (!v.is_string()) jfe_bad(j, std::format("\"{}\" is not text", key));
    return v.get<std::string>();
}

// A member holding text, if it does.
std::optional<std::string> jfe_text_opt(const Json& j, const char* key) {
    const Json* v = jfe_opt(j, key);
    if (v == nullptr || !v->is_string()) return std::nullopt;
    return v->get<std::string>();
}

// A node's kind.
std::string jfe_kind(const Json& n) { return jfe_text(n, "k"); }

// The children of a list member (an absent one has none).
const Json& jfe_list(const Json& j, const char* key) {
    static const Json kEmpty = Json::array();
    const Json* v = jfe_opt(j, key);
    return v != nullptr && v->is_array() ? *v : kEmpty;
}

// ---- types -------------------------------------------------------------------------------------------------------------------

std::optional<ir::Prim> primitive(std::string_view name) {
    if (name == "boolean") return ir::Prim::Bool;
    if (name == "byte") return ir::Prim::I8;
    if (name == "short") return ir::Prim::I16;
    if (name == "char") return ir::Prim::U16;
    if (name == "int") return ir::Prim::I32;
    if (name == "long") return ir::Prim::I64;
    if (name == "float") return ir::Prim::F32;
    if (name == "double") return ir::Prim::F64;
    return std::nullopt;
}

bool jfe_space(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

// A type at `i` of `s`; the index after it.
std::pair<TypeP, std::size_t> parse_at(std::string_view s, std::size_t i) {
    while (i < s.size() && s[i] == ' ') ++i;
    if (s.substr(i).starts_with("<nulltype>") || s.substr(i).starts_with("<nullType>")) return {ir::null_type(), i + 10};
    if (s.substr(i).starts_with("<any>")) return {ir::any_type(), i + 5};
    if (i < s.size() && s[i] == '?') {
        ++i;
        for (const auto& [word, mode] : {std::pair{std::string_view(" extends "), ir::BoundMode::Extends}, std::pair{std::string_view(" super "), ir::BoundMode::Super}}) {
            if (s.substr(i).starts_with(word)) {
                auto [bound, next] = parse_at(s, i + word.size());
                return {ir::make_type(ir::WildcardType{ir::WildcardBound{mode, bound}}), next};
            }
        }
        return {ir::make_type(ir::WildcardType{}), i};
    }
    std::size_t j = i;
    while (j < s.size() && std::string_view("<>,[]&").find(s[j]) == std::string_view::npos) ++j;
    std::string name(text::trim(s.substr(i, j - i)));
    if (name.starts_with("capture#")) name = "java.lang.Object";   // a captured wildcard: its erasure
    std::vector<TypeP> args;
    if (j < s.size() && s[j] == '<') {
        ++j;
        for (;;) {
            auto [arg, next] = parse_at(s, j);
            args.push_back(arg);
            j = next;
            while (j < s.size() && s[j] == ' ') ++j;
            if (j < s.size() && s[j] == ',') {
                ++j;
                continue;
            }
            ++j;   // '>'
            break;
        }
    }
    TypeP t;
    if (const auto prim = primitive(name)) {
        t = ir::prim_type(*prim);
    } else if (name == "void") {
        t = ir::void_type();
    } else if ((name.find('.') == std::string::npos && name.size() <= 2 && !name.empty() && name[0] >= 'A' && name[0] <= 'Z' && args.empty())) {
        t = ir::typevar_type(name);
    } else {
        t = ir::class_type(name, args);
    }
    while (s.substr(std::min(j, s.size())).starts_with("[]")) {
        t = ir::array_of(t);
        j += 2;
    }
    if (j < s.size() && s[j] == '&') {   // an intersection: its first bound
        j = parse_at(s, j + 1).second;
    }
    return {t, j};
}

SymP jfe_sym(const Json* d) {
    if (d == nullptr || !d->is_object() || d->empty()) return nullptr;
    const std::string k = jfe_text(*d, "kind");
    ir::SymKind kind;
    if (k == "LOCAL_VARIABLE" || k == "EXCEPTION_PARAMETER" || k == "RESOURCE_VARIABLE" || k == "BINDING_VARIABLE") kind = ir::SymKind::Local;
    else if (k == "PARAMETER") kind = ir::SymKind::Param;
    else if (k == "FIELD") kind = ir::SymKind::Field;
    else if (k == "ENUM_CONSTANT") kind = ir::SymKind::EnumConst;
    else if (k == "METHOD") kind = ir::SymKind::Method;
    else if (k == "CONSTRUCTOR") kind = ir::SymKind::Ctor;
    else if (k == "CLASS" || k == "INTERFACE" || k == "ENUM" || k == "RECORD" || k == "ANNOTATION_TYPE") kind = ir::SymKind::Class;
    else if (k == "TYPE_PARAMETER") kind = ir::SymKind::TypeVar;
    else if (k == "PACKAGE") kind = ir::SymKind::Package;
    else kind = ir::SymKind::Global;   // a kind of element the lowering has no use for
    const std::optional<std::string> owner = jfe_text_opt(*d, "owner");
    const bool is_static = jfe_opt(*d, "static") != nullptr && jfe_opt(*d, "static")->get<bool>();
    if (jfe_opt(*d, "params") != nullptr) {
        SymP s = ir::make_sym(kind, jfe_text(*d, "name"), owner, is_static);
        for (const Json& p : jfe_list(*d, "params")) s->params.push_back(parse_type(p.get<std::string>()));
        const std::optional<std::string> ret = jfe_text_opt(*d, "ret");
        s->ret = parse_type(ret ? std::optional<std::string_view>(*ret) : std::nullopt);
        s->varargs = jfe_opt(*d, "varargs") != nullptr && jfe_opt(*d, "varargs")->get<bool>();
        return s;
    }
    const std::optional<std::string> qname = jfe_text_opt(*d, "qname");
    std::string name = qname && !qname->empty() ? *qname : jfe_text(*d, "name");
    const std::optional<std::string> type = jfe_text_opt(*d, "type");
    return ir::make_sym(kind, std::move(name), owner, is_static, parse_type(type ? std::optional<std::string_view>(*type) : std::nullopt));
}

// ---- the lowering ---------------------------------------------------------------------------------------------------------------

bool is_type_kind(std::string_view k) { return k == "CLASS" || k == "INTERFACE" || k == "ENUM" || k == "RECORD" || k == "ANNOTATION_TYPE"; }

std::string_view class_kind_of(std::string_view k) {
    if (k == "CLASS") return "class";
    if (k == "INTERFACE") return "interface";
    if (k == "ENUM") return "enum";
    if (k == "RECORD") return "record";
    return "annotation";
}

bool is_literal(std::string_view k) { return k.ends_with("_LITERAL"); }

class Lowering {
public:
    explicit Lowering(Arena& arena) : arena_(arena) {}

    Node* unit(const Json& n, std::string_view path) {
        ir::Unit u;
        u.path = std::string(path);
        u.package = jfe_opt(n, "packageName") != nullptr ? qualified(jfe_opt(n, "packageName")) : std::string();
        for (const Json& i : jfe_list(n, "imports")) u.imports.push_back(qualified(jfe_opt(i, "qualifiedIdentifier")));
        for (const Json& t : jfe_list(n, "typeDecls"))
            if (is_type_kind(jfe_kind(t))) u.types.push_back(decl(t, std::nullopt));
        return make(std::move(u), {}, n);
    }

private:
    Arena& arena_;

    template <class K>
    Node* make(K kind, TypeP type, const Json& at) {
        std::uint32_t line = 0, col = 0;
        if (const Json* l = jfe_opt(at, "l"); l != nullptr && l->is_number()) line = l->get<std::uint32_t>();
        if (const Json* c = jfe_opt(at, "c"); c != nullptr && c->is_number()) col = c->get<std::uint32_t>();
        return ir::make(arena_, std::move(kind), std::move(type), line, col);
    }
    template <class K>
    Node* stmt_node(K kind, const Json& at) { return make(std::move(kind), {}, at); }

    TypeP ty(const Json& n) {
        const std::optional<std::string> t = jfe_text_opt(n, "$type");
        return parse_type(t ? std::optional<std::string_view>(*t) : std::nullopt);
    }

    std::string qualified(const Json* n) {
        if (n == nullptr) return {};
        const std::string k = jfe_kind(*n);
        if (k == "IDENTIFIER") return jfe_text(*n, "name");
        if (k == "MEMBER_SELECT") return qualified(&jfe_req(*n, "expression")) + "." + jfe_text(*n, "identifier");
        return {};
    }

    std::vector<std::string> mods(const Json* n) {
        std::vector<std::string> out;
        if (n == nullptr) return out;
        for (const Json& f : jfe_list(*n, "flags")) out.push_back(f.get<std::string>());
        return out;
    }

    // ---- declarations
    Node* decl(const Json& n, const std::optional<std::string>& outer) {
        const SymP s = jfe_sym(jfe_opt(n, "$sym"));
        ir::ClassDecl c;
        c.name = jfe_text_opt(n, "simpleName");
        c.qname = s ? std::optional<std::string>(s->name) : c.name;
        c.kind = std::string(class_kind_of(jfe_kind(n)));
        c.mods = mods(jfe_opt(n, "modifiers"));
        for (const Json& tp : jfe_list(n, "typeParameters")) c.typeparams.push_back(jfe_text_opt(tp, "name").value_or(""));
        if (const Json* e = jfe_opt(n, "extendsClause")) c.extends = ty(*e);
        for (const Json& i : jfe_list(n, "implementsClause")) c.implements.push_back(ty(i));
        c.outer = outer;
        const std::string qname = c.qname.value_or("");
        const bool is_record = *c.kind == "record";
        Node* node = make(std::move(c), {}, n);
        ir::ClassDecl* cd = ir::get<ir::ClassDecl>(node);
        for (const Json& m : jfe_list(n, "members")) cd->members.push_back(member(m, qname, n));
        if (is_record) record_members(node);
        return node;
    }

    Node* member(const Json& m, const std::string& cls_qname, const Json& raw_cls) {
        const std::string k = jfe_kind(m);
        if (is_type_kind(k)) return decl(m, cls_qname);
        if (k == "VARIABLE") {
            const SymP s = jfe_sym(jfe_opt(m, "$sym"));
            if (s && s->kind == ir::SymKind::EnumConst) {
                ir::EnumConst e;
                e.sym = s;
                const Json* init = jfe_opt(m, "initializer");
                if (init != nullptr) {
                    for (const Json& a : jfe_list(*init, "arguments")) e.args.push_back(expr(a));
                    if (const Json* body = jfe_opt(*init, "classBody")) e.body = decl(*body, cls_qname);
                    e.ctor = jfe_sym(jfe_opt(*init, "$sym"));
                }
                return make(std::move(e), {}, m);
            }
            ir::FieldDecl f;
            f.sym = s;
            f.type = ty(m);
            if (const Json* i = jfe_opt(m, "initializer")) f.init = expr(*i);
            f.mods = mods(jfe_opt(m, "modifiers"));
            return make(std::move(f), {}, m);
        }
        if (k == "METHOD") return method(m, raw_cls);
        if (k == "BLOCK") {   // an initialiser block
            ir::MethodDecl d;
            const bool is_static = jfe_opt(m, "static") != nullptr && jfe_opt(m, "static")->get<bool>();
            d.name = is_static ? "<clinit>" : "<init-block>";
            d.kind = is_static ? "static_init" : "init";
            d.ret = ir::void_type();
            d.body = block(m, false);
            return make(std::move(d), {}, m);
        }
        return make(ir::Unsupported{k, std::nullopt}, ir::any_type(), m);
    }

    Node* method(const Json& m, const Json& raw_cls) {
        ir::MethodDecl d;
        d.sym = jfe_sym(jfe_opt(m, "$sym"));
        const std::optional<std::string> name = jfe_text_opt(m, "name");
        const bool ctor = name && *name == "<init>";
        d.name = name;
        d.kind = ctor ? "ctor" : "method";
        for (const Json& p : jfe_list(m, "parameters")) {
            ir::Param param;
            param.sym = jfe_sym(jfe_opt(p, "$sym"));
            param.type = ty(p);
            param.mods = mods(jfe_opt(p, "modifiers"));
            d.params.push_back(make(std::move(param), {}, p));
        }
        if (const Json* body = jfe_opt(m, "body")) d.body = block(*body, ctor);
        d.mods = mods(jfe_opt(m, "modifiers"));
        // javac adds a default constructor to a class that declares none, at the class's own position
        if (ctor && jfe_opt(m, "l") != nullptr && jfe_opt(raw_cls, "l") != nullptr && *jfe_opt(m, "l") == *jfe_opt(raw_cls, "l") &&
            jfe_opt(m, "c") != nullptr && jfe_opt(raw_cls, "c") != nullptr && *jfe_opt(m, "c") == *jfe_opt(raw_cls, "c"))
            d.mods.push_back("IMPLICIT");
        d.ret = d.sym && d.sym->ret ? d.sym->ret : ir::void_type();
        for (const Json& t : jfe_list(m, "throws")) d.throws.push_back(ty(t));
        for (const Json& tp : jfe_list(m, "typeParameters")) d.typeparams.push_back(jfe_text_opt(tp, "name").value_or(""));
        return make(std::move(d), {}, m);
    }

    // ---- statements
    Node* block(const Json& n, bool ctor_body) {
        ir::Block b;
        for (const Json& st : jfe_list(n, "statements")) {
            Node* s = stmt(st);
            if (ctor_body) {
                if (auto* call = ir::get<ir::CtorCall>(s)) {
                    const Json* sl = jfe_opt(st, "l");
                    const Json* sc = jfe_opt(st, "c");
                    const Json* nl = jfe_opt(n, "l");
                    const Json* nc = jfe_opt(n, "c");
                    if (sl != nullptr && nl != nullptr && sc != nullptr && nc != nullptr && *sl == *nl && *sc == *nc)
                        call->kind = call->kind.value_or("") + ":implicit";   // javac's own super() at the body's position
                }
            }
            b.stmts.push_back(s);
        }
        return make(std::move(b), {}, n);
    }

    ir::Suite one(Node* n) { return n; }

    Node* local_of(const Json& v, bool with_init) {
        ir::Local l;
        l.sym = jfe_sym(jfe_opt(v, "$sym"));
        l.type = ty(v);
        if (with_init) {
            if (const Json* i = jfe_opt(v, "initializer")) l.init = expr(*i);
        }
        l.mods = mods(jfe_opt(v, "modifiers"));
        return make(std::move(l), {}, v);
    }

    Node* stmt(const Json& n) {
        const std::string k = jfe_kind(n);
        if (k == "BLOCK") return block(n, false);
        if (k == "VARIABLE") return local_of(n, true);
        if (k == "EXPRESSION_STATEMENT") {
            const Json& e = jfe_req(n, "expression");
            const Json* sel = jfe_kind(e) == "METHOD_INVOCATION" ? jfe_opt(e, "methodSelect") : nullptr;
            if (sel != nullptr && jfe_kind(*sel) == "IDENTIFIER" && (jfe_text(*sel, "name") == "this" || jfe_text(*sel, "name") == "super")) {
                ir::CtorCall c;
                c.kind = jfe_text(*sel, "name");
                c.sym = jfe_sym(jfe_opt(e, "$sym"));
                for (const Json& a : jfe_list(e, "arguments")) c.args.push_back(expr(a));
                return stmt_node(std::move(c), n);
            }
            if (sel != nullptr && jfe_kind(*sel) == "MEMBER_SELECT" && (jfe_text(*sel, "identifier") == "this" || jfe_text(*sel, "identifier") == "super")) {
                const Json* ssym = jfe_opt(*sel, "$sym");
                if (ssym != nullptr && jfe_text_opt(*ssym, "kind") == std::optional<std::string>("CONSTRUCTOR")) {
                    ir::CtorCall c;
                    c.kind = jfe_text(*sel, "identifier");
                    c.sym = jfe_sym(jfe_opt(e, "$sym"));
                    for (const Json& a : jfe_list(e, "arguments")) c.args.push_back(expr(a));
                    c.target = expr(jfe_req(*sel, "expression"));
                    return stmt_node(std::move(c), n);
                }
            }
            return stmt_node(ir::ExprStmt{expr(e)}, n);
        }
        if (k == "IF") {
            ir::If i;
            i.cond = expr(jfe_req(n, "condition"));
            i.then = stmt(jfe_req(n, "thenStatement"));
            if (const Json* e = jfe_opt(n, "elseStatement")) i.orelse = stmt(*e);
            else i.orelse = static_cast<Node*>(nullptr);
            return stmt_node(std::move(i), n);
        }
        if (k == "WHILE_LOOP") {
            ir::While w;
            w.cond = expr(jfe_req(n, "condition"));
            w.body = stmt(jfe_req(n, "statement"));
            return stmt_node(std::move(w), n);
        }
        if (k == "DO_WHILE_LOOP") {
            ir::DoWhile w;
            w.body = stmt(jfe_req(n, "statement"));
            w.cond = expr(jfe_req(n, "condition"));
            return stmt_node(std::move(w), n);
        }
        if (k == "FOR_LOOP") {
            ir::For f;
            for (const Json& s : jfe_list(n, "initializer")) f.init.push_back(stmt(s));
            if (const Json* c = jfe_opt(n, "condition")) f.cond = expr(*c);
            for (const Json& s : jfe_list(n, "update")) f.update.push_back(expr(jfe_req(s, "expression")));
            f.body = stmt(jfe_req(n, "statement"));
            return stmt_node(std::move(f), n);
        }
        if (k == "ENHANCED_FOR_LOOP") {
            ir::ForEach f;
            f.var = local_of(jfe_req(n, "variable"), false);
            f.iterable = expr(jfe_req(n, "expression"));
            f.body = stmt(jfe_req(n, "statement"));
            return stmt_node(std::move(f), n);
        }
        if (k == "SWITCH") {
            ir::Switch s;
            s.expr = expr(jfe_req(n, "expression"));
            for (const Json& c : jfe_list(n, "cases")) s.cases.push_back(case_of(c, false));
            return stmt_node(std::move(s), n);
        }
        if (k == "BREAK") return stmt_node(ir::Break{jfe_text_opt(n, "label")}, n);
        if (k == "CONTINUE") return stmt_node(ir::Continue{jfe_text_opt(n, "label")}, n);
        if (k == "RETURN") {
            ir::Return r;
            if (const Json* e = jfe_opt(n, "expression")) r.expr = expr(*e);
            return stmt_node(std::move(r), n);
        }
        if (k == "THROW") return stmt_node(ir::Throw{expr(jfe_req(n, "expression"))}, n);
        if (k == "TRY") {
            ir::Try t;
            for (const Json& r : jfe_list(n, "resources")) t.resources.push_back(jfe_kind(r) == "VARIABLE" ? stmt(r) : stmt_node(ir::ExprStmt{expr(r)}, r));
            t.body = block(jfe_req(n, "block"), false);
            for (const Json& c : jfe_list(n, "catches")) {
                const Json& v = jfe_req(c, "parameter");
                ir::Catch catch_;
                catch_.sym = jfe_sym(jfe_opt(v, "$sym"));
                const Json* tt = jfe_opt(v, "type");
                if (tt != nullptr && jfe_kind(*tt) == "UNION_TYPE") {
                    for (const Json& a : jfe_list(*tt, "typeAlternatives")) catch_.types.push_back(ty(a));
                } else {
                    catch_.types.push_back(ty(v));
                }
                catch_.body = block(jfe_req(c, "block"), false);
                t.catches.push_back(make(std::move(catch_), {}, c));
            }
            if (const Json* f = jfe_opt(n, "finallyBlock")) t.finally_body = block(*f, false);
            return stmt_node(std::move(t), n);
        }
        if (k == "LABELED_STATEMENT") {
            ir::Labeled l;
            l.label = jfe_text_opt(n, "label");
            l.body = stmt(jfe_req(n, "statement"));
            return stmt_node(std::move(l), n);
        }
        if (k == "SYNCHRONIZED") {
            ir::Sync s;
            s.lock = expr(jfe_req(n, "expression"));
            s.body = block(jfe_req(n, "block"), false);
            return stmt_node(std::move(s), n);
        }
        if (k == "EMPTY_STATEMENT") return stmt_node(ir::Empty{}, n);
        if (is_type_kind(k)) return stmt_node(ir::LocalClass{decl(n, std::nullopt)}, n);
        if (k == "ASSERT") {
            ir::Assert a;
            a.cond = expr(jfe_req(n, "condition"));
            if (const Json* d = jfe_opt(n, "detail")) a.detail = expr(*d);
            return stmt_node(std::move(a), n);
        }
        if (k == "YIELD") return stmt_node(ir::Yield{expr(jfe_req(n, "value"))}, n);
        return stmt_node(ir::ExprStmt{make(ir::Unsupported{k, std::nullopt}, ir::any_type(), n)}, n);
    }

    Node* case_of(const Json& c, bool in_expr) {
        ir::Case out;
        const Json& labels = jfe_list(c, "labels");
        for (const Json& lab : labels) {
            const std::string lk = jfe_kind(lab);
            if (lk == "DEFAULT_CASE_LABEL") out.is_default = true;
            else if (lk == "CONSTANT_CASE_LABEL") out.labels.push_back(expr(jfe_req(lab, "constantExpression")));
            else out.labels.push_back(make(ir::Unsupported{lk, std::nullopt}, ir::any_type(), lab));
        }
        if (labels.empty()) {   // older javac: expression(s) or none
            std::vector<const Json*> exprs;
            if (const Json* es = jfe_opt(c, "expressions"); es != nullptr && es->is_array() && !es->empty()) {
                for (const Json& e : *es) exprs.push_back(&e);
            } else if (const Json* e = jfe_opt(c, "expression")) {
                exprs.push_back(e);
            }
            for (const Json* e : exprs) out.labels.push_back(expr(*e));
            out.is_default = exprs.empty();
        }
        if (jfe_text_opt(c, "caseKind") == std::optional<std::string>("RULE")) {   // an arrow arm: a value, a block or a throw
            const Json* b = jfe_opt(c, "body");
            if (b == nullptr) {
                // no body
            } else {
                const std::string bk = jfe_kind(*b);
                if (bk == "BLOCK" || bk == "THROW" || bk == "EXPRESSION_STATEMENT") out.body.push_back(stmt(*b));
                else if (in_expr) out.body.push_back(stmt_node(ir::Yield{expr(*b)}, *b));
                else out.body.push_back(stmt_node(ir::ExprStmt{expr(*b)}, *b));
            }
            if (!in_expr) out.body.push_back(stmt_node(ir::Break{std::nullopt}, c));   // an arrow arm does not fall through
        } else {
            for (const Json& s : jfe_list(c, "statements")) out.body.push_back(stmt(s));
        }
        return make(std::move(out), {}, c);
    }

    // ---- expressions
    Node* expr(const Json& n) {
        const std::string k = jfe_kind(n);
        const TypeP t = ty(n);
        if (is_literal(k)) return literal(n, k, t);
        if (k == "PARENTHESIZED") return make(ir::Paren{expr(jfe_req(n, "expression"))}, t, n);
        if (k == "IDENTIFIER") return identifier(n, t);
        if (k == "MEMBER_SELECT") return member_select(n, t);
        if (k == "METHOD_INVOCATION") return call(n, t);
        if (k == "NEW_CLASS") {
            ir::New nw;
            nw.sym = jfe_sym(jfe_opt(n, "$sym"));
            for (const Json& a : jfe_list(n, "arguments")) nw.args.push_back(expr(a));
            if (const Json* body = jfe_opt(n, "classBody")) {
                Json as_class = *body;
                as_class["k"] = "CLASS";
                Node* decl_node = decl(as_class, std::nullopt);
                ir::ClassDecl* cd = ir::get<ir::ClassDecl>(decl_node);
                if (!cd->extends) {
                    if (const Json* id = jfe_opt(n, "identifier")) cd->extends = ty(*id);   // what an anonymous class extends or implements
                }
                nw.body = decl_node;
            }
            if (const Json* outer = jfe_opt(n, "enclosingExpression")) nw.outer = expr(*outer);
            return make(std::move(nw), t, n);
        }
        if (k == "NEW_ARRAY") {
            ir::NewArray a;
            a.elem = t && std::holds_alternative<ir::ArrayType>(t->body) ? ir::dims(t).second : ir::any_type();
            for (const Json& d : jfe_list(n, "dimensions")) a.dims.push_back(expr(d));
            for (const Json& e : jfe_list(n, "initializers")) a.init.push_back(expr(e));
            return make(std::move(a), t, n);
        }
        if (k == "ARRAY_ACCESS") {
            ir::Index i;
            i.arr = expr(jfe_req(n, "expression"));
            i.index = expr(jfe_req(n, "index"));
            return make(std::move(i), t, n);
        }
        if (const auto op = ir::binary_operator(k)) {
            Node* l = expr(jfe_req(n, "leftOperand"));
            Node* r = expr(jfe_req(n, "rightOperand"));
            if (k == "PLUS" && t && std::holds_alternative<ir::ClassType>(t->body) && std::get<ir::ClassType>(t->body).name == "java.lang.String") {
                ir::Concat c;
                for (Node* x : {l, r}) {
                    if (auto* inner = ir::get<ir::Concat>(x)) c.parts.insert(c.parts.end(), inner->parts.begin(), inner->parts.end());
                    else c.parts.push_back(x);
                }
                return make(std::move(c), t, n);
            }
            return make(ir::Binary{std::string(*op), l, r, std::nullopt, std::nullopt}, t, n);
        }
        if (const auto op = ir::unary_operator(k)) return make(ir::Unary{std::string(op->op), expr(jfe_req(n, "expression")), op->postfix, std::nullopt}, t, n);
        if (k == "ASSIGNMENT") return make(ir::Assign{std::nullopt, expr(jfe_req(n, "variable")), expr(jfe_req(n, "expression")), std::nullopt}, t, n);
        if (k.ends_with("_ASSIGNMENT")) {
            if (const auto op = ir::binary_operator(k.substr(0, k.size() - std::string_view("_ASSIGNMENT").size())))
                return make(ir::Assign{std::string(*op), expr(jfe_req(n, "variable")), expr(jfe_req(n, "expression")), std::nullopt}, t, n);
        }
        if (k == "CONDITIONAL_EXPRESSION")
            return make(ir::Cond{expr(jfe_req(n, "condition")), expr(jfe_req(n, "trueExpression")), expr(jfe_req(n, "falseExpression"))}, t, n);
        if (k == "TYPE_CAST") return make(ir::Cast{expr(jfe_req(n, "expression")), std::nullopt, false}, t, n);
        if (k == "INSTANCE_OF") return instance_of(n, t);
        if (k == "SWITCH_EXPRESSION") {
            ir::SwitchExpr s;
            s.expr = expr(jfe_req(n, "expression"));
            for (const Json& c : jfe_list(n, "cases")) s.cases.push_back(case_of(c, true));
            return make(std::move(s), t, n);
        }
        if (k == "LAMBDA_EXPRESSION") {
            ir::Lambda l;
            for (const Json& v : jfe_list(n, "parameters")) {
                ir::Param p;
                p.sym = jfe_sym(jfe_opt(v, "$sym"));
                p.type = ty(v);
                l.params.push_back(make(std::move(p), {}, v));
            }
            const Json& b = jfe_req(n, "body");
            l.body = jfe_kind(b) == "BLOCK" ? block(b, false) : expr(b);
            return make(std::move(l), t, n);
        }
        if (k == "MEMBER_REFERENCE") {
            ir::MethodRef r;
            r.target = expr(jfe_req(n, "qualifierExpression"));
            r.sym = jfe_sym(jfe_opt(n, "$sym"));
            r.mode = jfe_text_opt(n, "mode") == std::optional<std::string>("NEW") ? "new" : "invoke";
            return make(std::move(r), t, n);
        }
        return make(ir::Unsupported{k, std::nullopt}, t, n);
    }

    Node* instance_of(const Json& n, const TypeP& t) {
        const Json* pat = jfe_opt(n, "pattern");
        const Json* test = jfe_opt(n, "type");
        if (test == nullptr && pat != nullptr) test = jfe_opt(*pat, "variable");
        if (test == nullptr && pat != nullptr) test = jfe_opt(*pat, "var");
        Node* binding = nullptr;
        if (pat != nullptr && jfe_text_opt(*pat, "k") == std::optional<std::string>("BINDING_PATTERN")) {   // `x instanceof T v`: v is bound where it matched
            const Json* v = jfe_opt(*pat, "variable");
            if (v == nullptr) v = &jfe_req(*pat, "var");
            binding = local_of(*v, false);
            if (test == nullptr) test = v;
        } else if (pat != nullptr && !pat->empty()) {   // a record pattern (deconstruction): not yet
            return make(ir::Unsupported{"INSTANCE_OF:" + jfe_text_opt(*pat, "k").value_or("?"), std::nullopt}, t, n);
        }
        return make(ir::InstanceOf{expr(jfe_req(n, "expression")), test != nullptr ? ty(*test) : ir::any_type(), binding}, t, n);
    }

    Node* literal(const Json& n, const std::string& k, const TypeP& t) {
        ir::Lit lit;
        const Json* v = jfe_opt(n, "value");
        if (k == "NULL_LITERAL") {
            // null
        } else if (k == "FLOAT_LITERAL" || k == "DOUBLE_LITERAL") {
            lit.value = floating(v);
        } else if (k == "INT_LITERAL" || k == "LONG_LITERAL") {
            lit.value = integer(v);
        } else if (k == "BOOLEAN_LITERAL") {
            lit.value = v != nullptr && (v->is_boolean() ? v->get<bool>() : v->is_string() ? !v->get<std::string>().empty() : true);
        } else if (v != nullptr) {
            if (v->is_string()) lit.value = v->get<std::string>();
            else if (v->is_number_integer()) lit.value = v->get<std::int64_t>();
        }
        return make(std::move(lit), t, n);
    }

    static ir::LitValue integer(const Json* v) {
        if (v == nullptr) return std::int64_t{0};
        if (v->is_number_unsigned() && v->get<std::uint64_t>() > static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())) return v->get<std::uint64_t>();
        if (v->is_number_integer()) return v->get<std::int64_t>();
        if (v->is_string()) {
            const std::string& s = v->get_ref<const std::string&>();
            std::int64_t value = 0;
            if (std::from_chars(s.data(), s.data() + s.size(), value).ec == std::errc()) return value;
        }
        return std::int64_t{0};
    }

    static ir::LitValue floating(const Json* v) {
        if (v == nullptr) return 0.0;
        if (v->is_number()) return v->get<double>();
        if (v->is_string()) {
            const std::string& s = v->get_ref<const std::string&>();
            if (s == "NaN") return std::numeric_limits<double>::quiet_NaN();
            if (s == "Infinity") return std::numeric_limits<double>::infinity();
            if (s == "-Infinity") return -std::numeric_limits<double>::infinity();
            double value = 0;
            if (std::from_chars(s.data(), s.data() + s.size(), value).ec == std::errc()) return value;
        }
        return 0.0;
    }

    Node* identifier(const Json& n, const TypeP& t) {
        const std::optional<std::string> name = jfe_text_opt(n, "name");
        if (name == std::optional<std::string>("this")) return make(ir::This{}, t, n);
        if (name == std::optional<std::string>("super")) return make(ir::Super{}, t, n);
        const SymP s = jfe_sym(jfe_opt(n, "$sym"));
        if (!s) return make(ir::Unsupported{"IDENTIFIER?", name}, t, n);
        if (s->kind == ir::SymKind::Local || s->kind == ir::SymKind::Param) return make(ir::Name{s}, t, n);
        if (s->kind == ir::SymKind::Field || s->kind == ir::SymKind::EnumConst) {   // a bare field name: on this, or on its class
            const TypeP owner = ir::class_type(s->owner.value_or(""));
            Node* obj = s->is_static ? make(ir::TypeName{}, owner, n) : make(ir::This{}, owner, n);
            return make(ir::Select{obj, s}, t, n);
        }
        if (s->kind == ir::SymKind::Class || s->kind == ir::SymKind::TypeVar || s->kind == ir::SymKind::Package)
            return make(ir::TypeName{}, t && !std::holds_alternative<ir::AnyType>(t->body) ? t : ir::class_type(s->name), n);
        return make(ir::Unsupported{"IDENTIFIER:" + std::string(ir::to_text(s->kind)), name}, t, n);
    }

    Node* member_select(const Json& n, const TypeP& t) {
        const std::string ident = jfe_text(n, "identifier");
        const SymP s = jfe_sym(jfe_opt(n, "$sym"));
        const Json& inner = jfe_req(n, "expression");
        if (ident == "class") return make(ir::ClassLit{ty(inner)}, t, n);
        if (ident == "length" && s && s->owner == std::optional<std::string>("Array")) return make(ir::Length{expr(inner)}, t, n);
        if (ident == "this") return make(ir::This{ty(inner)}, t, n);
        if (ident == "super") return make(ir::Super{ty(inner)}, t, n);
        if (!s) return make(ir::Unsupported{"MEMBER_SELECT?", ident}, t, n);
        if (s->kind == ir::SymKind::Class || s->kind == ir::SymKind::Package || s->kind == ir::SymKind::TypeVar)
            return make(ir::TypeName{}, t && !std::holds_alternative<ir::AnyType>(t->body) ? t : ir::class_type(s->name), n);
        if (s->kind == ir::SymKind::Field || s->kind == ir::SymKind::EnumConst) return make(ir::Select{expr(inner), s}, t, n);
        return make(ir::Unsupported{"MEMBER_SELECT:" + std::string(ir::to_text(s->kind)), ident}, t, n);
    }

    Node* call(const Json& n, const TypeP& t) {
        ir::Call c;
        c.sym = jfe_sym(jfe_opt(n, "$sym"));
        const Json& sel = jfe_req(n, "methodSelect");
        for (const Json& a : jfe_list(n, "arguments")) c.args.push_back(expr(a));
        const std::string sk = jfe_kind(sel);
        if (sk == "IDENTIFIER") {
            if (c.sym && c.sym->kind == ir::SymKind::Method && c.sym->is_static)
                c.target = make(ir::TypeName{}, ir::class_type(c.sym->owner.value_or("")), sel);
            // else: this class's own method, implicit this
        } else if (sk == "MEMBER_SELECT") {
            c.target = expr(jfe_req(sel, "expression"));
        } else {
            return make(ir::Unsupported{"METHOD_INVOCATION:" + sk, std::nullopt}, t, n);
        }
        return make(std::move(c), t, n);
    }

    // ---- records
    // What javac writes for a record only when it lowers it: the canonical constructor's field assignments, an accessor per component,
    // equals, hashCode and toString.
    void record_members(Node* cls_node) {
        ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cls_node);
        const std::uint32_t line = cls_node->line, col = cls_node->col;
        const std::string qname = c.qname.value_or("");
        std::vector<Node*> comps;
        for (Node* m : c.members) {
            const auto* f = ir::get<ir::FieldDecl>(m);
            if (f != nullptr && f->sym && !f->sym->is_static) comps.push_back(m);
        }
        const TypeP me = ir::class_type(qname);
        const auto mk = [&](auto kind, TypeP type = {}) { return ir::make(arena_, std::move(kind), std::move(type), line, col); };
        const auto lit = [&](ir::LitValue v, const TypeP& type) { return mk(ir::Lit{std::move(v), std::nullopt}, type); };
        const auto this_field = [&](Node* f) {
            const auto* fd = ir::get<ir::FieldDecl>(f);
            return mk(ir::Select{mk(ir::This{}, me), fd->sym}, fd->type);
        };
        const auto static_call = [&](const std::string& owner, const std::string& name, std::vector<TypeP> params, const TypeP& ret, std::vector<Node*> args) {
            SymP s = ir::make_sym(ir::SymKind::Method, name, owner, true);
            s->params = std::move(params);
            s->ret = ret;
            ir::Call call;
            call.target = mk(ir::TypeName{}, ir::class_type(owner));
            call.sym = s;
            call.args = std::move(args);
            return mk(std::move(call), ret);
        };
        const auto add_method = [&](const std::string& name, std::vector<Node*> params, const TypeP& ret, std::vector<Node*> stmts) {
            SymP s = ir::make_sym(ir::SymKind::Method, name, qname, false);
            for (Node* p : params) s->params.push_back(ir::get<ir::Param>(p)->type);
            s->ret = ret;
            ir::MethodDecl d;
            d.sym = s;
            d.name = name;
            d.kind = "method";
            d.params = std::move(params);
            d.ret = ret;
            d.body = mk(ir::Block{std::move(stmts)});
            d.mods = {"public", "GENERATED"};
            c.members.push_back(mk(std::move(d)));
        };
        const auto declared = [&](std::string_view name, std::size_t nparams) {
            for (Node* m : c.members) {
                const auto* d = ir::get<ir::MethodDecl>(m);
                if (d != nullptr && d->name == std::optional<std::string>(std::string(name)) && d->params.size() == nparams) return true;
            }
            return false;
        };
        const TypeP string_t = ir::class_type("java.lang.String"), object_t = ir::class_type("java.lang.Object");
        const auto hash_owner = [](const ir::PrimType& p) -> std::optional<std::string> {
            switch (p.prim) {
            case ir::Prim::I32: return "java.lang.Integer";
            case ir::Prim::I64: return "java.lang.Long";
            case ir::Prim::I16: return "java.lang.Short";
            case ir::Prim::I8: return "java.lang.Byte";
            case ir::Prim::U16: return "java.lang.Character";
            case ir::Prim::Bool: return "java.lang.Boolean";
            case ir::Prim::F32: return "java.lang.Float";
            case ir::Prim::F64: return "java.lang.Double";
            default: return std::nullopt;
            }
        };

        // the canonical constructor (javac's own, or a compact one) assigns each component from its parameter
        std::vector<TypeP> want;
        for (Node* f : comps) want.push_back(ir::get<ir::FieldDecl>(f)->type);
        for (Node* m : c.members) {
            auto* d = ir::get<ir::MethodDecl>(m);
            if (d == nullptr || d->kind != std::optional<std::string>("ctor") || d->params.size() != want.size()) continue;
            bool same = true;
            for (std::size_t i = 0; i < want.size(); ++i) same = same && ir::get<ir::Param>(d->params[i])->type == want[i];
            if (!same) continue;
            auto* body = std::holds_alternative<Node*>(d->body) ? ir::get<ir::Block>(std::get<Node*>(d->body)) : nullptr;
            if (body == nullptr) continue;
            bool assigned = false;
            for (Node* x : ir::walk(std::get<Node*>(d->body))) {
                const auto* a = ir::get<ir::Assign>(x);
                const auto* sel = a != nullptr ? ir::get<ir::Select>(a->target) : nullptr;
                if (sel != nullptr && ir::is<ir::This>(sel->obj)) assigned = true;
            }
            if (!assigned) {
                for (std::size_t i = 0; i < comps.size(); ++i) {
                    const auto* f = ir::get<ir::FieldDecl>(comps[i]);
                    const auto* p = ir::get<ir::Param>(d->params[i]);
                    Node* value = mk(ir::Name{p->sym}, p->type);
                    body->stmts.push_back(mk(ir::ExprStmt{mk(ir::Assign{std::nullopt, this_field(comps[i]), value, std::nullopt}, f->type)}));
                }
            }
        }
        for (Node* f : comps) {   // x() for each component x
            const auto* fd = ir::get<ir::FieldDecl>(f);
            if (!declared(fd->sym->name, 0)) add_method(fd->sym->name, {}, fd->type, {mk(ir::Return{this_field(f)})});
        }
        if (!declared("toString", 0)) {   // Point[x=1, y=2]
            ir::Concat parts;
            parts.parts.push_back(lit(c.name.value_or("") + "[", string_t));
            for (std::size_t i = 0; i < comps.size(); ++i) {
                parts.parts.push_back(lit((i == 0 ? "" : ", ") + ir::get<ir::FieldDecl>(comps[i])->sym->name + "=", string_t));
                parts.parts.push_back(this_field(comps[i]));
            }
            parts.parts.push_back(lit(std::string("]"), string_t));
            add_method("toString", {}, string_t, {mk(ir::Return{mk(std::move(parts), string_t)})});
        }
        if (!declared("hashCode", 0)) {   // h = 31 * h + hash(component), from 0
            const TypeP i32 = ir::i32_type();
            SymP h = ir::make_sym(ir::SymKind::Local, "h", qname + ".hashCode", false, i32);
            std::vector<Node*> stmts;
            ir::Local init;
            init.sym = h;
            init.type = i32;
            init.init = lit(std::int64_t{0}, i32);
            stmts.push_back(mk(std::move(init)));
            for (Node* f : comps) {
                const auto* fd = ir::get<ir::FieldDecl>(f);
                const ir::PrimType* prim = fd->type && std::holds_alternative<ir::PrimType>(fd->type->body) ? &std::get<ir::PrimType>(fd->type->body) : nullptr;
                const std::optional<std::string> owner = prim != nullptr ? hash_owner(*prim) : std::nullopt;
                Node* call = static_call(owner ? *owner : "java.util.Objects", "hashCode", {owner ? fd->type : object_t}, i32, {this_field(f)});
                Node* times = mk(ir::Binary{"*", lit(std::int64_t{31}, i32), mk(ir::Name{h}, i32), std::nullopt, std::nullopt}, i32);
                Node* sum = mk(ir::Binary{"+", times, call, std::nullopt, std::nullopt}, i32);
                stmts.push_back(mk(ir::ExprStmt{mk(ir::Assign{std::nullopt, mk(ir::Name{h}, i32), sum, std::nullopt}, i32)}));
            }
            stmts.push_back(mk(ir::Return{mk(ir::Name{h}, i32)}));
            add_method("hashCode", {}, i32, std::move(stmts));
        }
        if (!declared("equals", 1)) {   // the same record type, every component equal
            const TypeP boolean = ir::bool_type();
            SymP o = ir::make_sym(ir::SymKind::Param, "o", qname + ".equals", false, object_t);
            SymP that = ir::make_sym(ir::SymKind::Local, "that", qname + ".equals", false, me);
            Node* isit = mk(ir::InstanceOf{mk(ir::Name{o}, object_t), me, nullptr}, boolean);
            std::vector<Node*> stmts;
            ir::If test;
            test.cond = mk(ir::Unary{"!", mk(ir::Paren{isit}, boolean), false, std::nullopt}, boolean);
            test.then = mk(ir::Return{lit(false, boolean)});
            test.orelse = static_cast<Node*>(nullptr);
            stmts.push_back(mk(std::move(test)));
            ir::Local cast;
            cast.sym = that;
            cast.type = me;
            cast.init = mk(ir::Cast{mk(ir::Name{o}, object_t), std::nullopt, false}, me);
            stmts.push_back(mk(std::move(cast)));
            Node* cond = nullptr;
            for (Node* f : comps) {
                const auto* fd = ir::get<ir::FieldDecl>(f);
                Node* mine = this_field(f);
                Node* theirs = mk(ir::Select{mk(ir::Name{that}, me), fd->sym}, fd->type);
                const ir::PrimType* prim = fd->type && std::holds_alternative<ir::PrimType>(fd->type->body) ? &std::get<ir::PrimType>(fd->type->body) : nullptr;
                Node* eq;
                if (prim != nullptr && (prim->prim == ir::Prim::F32 || prim->prim == ir::Prim::F64)) {   // Float.compare / Double.compare == 0 (NaN equals NaN)
                    Node* compare = static_call(*hash_owner(*prim), "compare", {fd->type, fd->type}, ir::i32_type(), {mine, theirs});
                    eq = mk(ir::Binary{"==", compare, lit(std::int64_t{0}, ir::i32_type()), std::nullopt, std::nullopt}, boolean);
                } else if (prim != nullptr) {
                    eq = mk(ir::Binary{"==", mine, theirs, std::nullopt, std::nullopt}, boolean);
                } else {
                    eq = static_call("java.util.Objects", "equals", {object_t, object_t}, boolean, {mine, theirs});
                }
                cond = cond == nullptr ? eq : mk(ir::Binary{"&&", cond, eq, std::nullopt, std::nullopt}, boolean);
            }
            stmts.push_back(mk(ir::Return{cond != nullptr ? cond : lit(true, boolean)}));
            ir::Param param;
            param.sym = o;
            param.type = object_t;
            add_method("equals", {mk(std::move(param))}, boolean, std::move(stmts));
        }
    }
};

}  // namespace

TypeP parse_type(std::optional<std::string_view> text) {
    if (!text) return ir::any_type();
    return parse_at(text::trim(*text), 0).first;
}

Node* lower_unit(Arena& arena, const Json& tree, std::string_view path) { return Lowering(arena).unit(tree, path); }

// ---- running javac -----------------------------------------------------------------------------------------------------------

std::vector<stdfs::path> java_files(const stdfs::path& java_dir) {
    std::vector<stdfs::path> out;
    for (const auto& entry : stdfs::recursive_directory_iterator(java_dir))
        if (entry.is_regular_file() && entry.path().extension() == ".java") out.push_back(entry.path());
    std::sort(out.begin(), out.end());
    return out;
}

namespace {

// The folder holding the compiled TreeDump, compiled when its source or the JDK changed.
stdfs::path treedump_folder(const DumpTools& tools) {
    const Bytes source = fs::read_file(tools.treedump_source);
    const process::ProcessResult version = process::run({.argv = {fs::to_text(tools.javac), "-version"}, .err = process::Capture::Merge});
    Bytes key_bytes = source;
    key_bytes.insert(key_bytes.end(), version.out.begin(), version.out.end());
    key_bytes.push_back(0);
    const std::string javac_text = fs::to_text(tools.javac);
    key_bytes.insert(key_bytes.end(), javac_text.begin(), javac_text.end());
    const std::string key = crypto::to_hex(crypto::sha1(key_bytes)).substr(0, 12);
    const stdfs::path folder = tools.state / "translate" / ("treedump-" + key);
    std::error_code ec;
    if (stdfs::is_regular_file(folder / "TreeDump.class", ec)) return folder;
    fs::make_dirs(folder);
    const process::ProcessResult r = process::run({.argv = {fs::to_text(tools.javac), "-nowarn", "-encoding", "UTF-8", "-d", fs::to_text(folder), fs::to_text(tools.treedump_source)},
                                                   .err = process::Capture::Merge,
                                                   .timeout = std::chrono::minutes(5)});
    if (r.exit_code != 0) {
        stdfs::remove_all(folder, ec);
        const std::string message(r.out.begin(), r.out.end());
        throw ToolError("javac", r.exit_code, "TreeDump did not compile: " + message.substr(0, 400));
    }
    for (const auto& old : stdfs::directory_iterator(folder.parent_path(), ec)) {   // (those of another source or JDK: never used again)
        const std::string name = fs::to_text(old.path().filename());
        if (name.starts_with("treedump-") && old.path() != folder) stdfs::remove_all(old.path(), ec);
    }
    return folder;
}

bool names_word(std::string_view text, std::string_view word) {
    for (std::size_t at = text.find(word); at != std::string_view::npos; at = text.find(word, at + 1)) {
        const auto part = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; };
        if ((at == 0 || !part(text[at - 1])) && (at + word.size() >= text.size() || !part(text[at + word.size()]))) return true;
    }
    return false;
}

// The classes of CodeBreaker's Java library this program names, and those they name in turn.
std::vector<stdfs::path> library_used(const stdfs::path& library, const std::vector<stdfs::path>& program) {
    std::vector<stdfs::path> out;
    std::error_code ec;
    if (library.empty() || !stdfs::is_directory(library, ec)) return out;
    std::vector<std::pair<stdfs::path, std::string>> available;
    for (const stdfs::path& f : java_files(library)) available.emplace_back(f, fs::read_text(f));
    std::vector<std::string> open;
    for (const stdfs::path& f : program) open.push_back(fs::read_text(f));
    std::set<stdfs::path> taken;
    while (!open.empty()) {
        const std::string text = std::move(open.back());
        open.pop_back();
        for (const auto& [file, source] : available) {
            if (!taken.contains(file) && names_word(text, fs::to_text(file.stem()))) {
                taken.insert(file);
                open.push_back(source);
            }
        }
    }
    out.assign(taken.begin(), taken.end());
    return out;
}

}  // namespace

std::vector<Node*> lower_program(Arena& arena, const stdfs::path& java_dir, const DumpTools& tools) {
    if (tools.javac.empty() || tools.java.empty()) throw ToolError("javac", 0, "the Java front end needs a JDK (settings.json: java)");
    const stdfs::path folder = treedump_folder(tools);
    fs::ScopedTempDir out{"cb-treedump"};
    const stdfs::path listing = out.path() / "_files.txt";
    const std::vector<stdfs::path> program = java_files(java_dir);
    const std::vector<stdfs::path> library = library_used(tools.library, program);
    const auto dump = [&](bool with_library) {
        std::string names;
        for (const stdfs::path& f : program) names += (names.empty() ? "" : "\n") + fs::to_text(f);
        if (with_library) {
            for (const stdfs::path& f : library) names += "\n" + fs::to_text(f);
        }
        fs::write_text(listing, names);
        std::vector<std::string> argv = {fs::to_text(tools.java), "-Xss16m", "-cp", fs::to_text(folder), "TreeDump", "--out", fs::to_text(out.path() / "units"), "--sourcepath", fs::to_text(java_dir)};
        if (with_library) {
            argv.push_back("--patch");
            argv.push_back(fs::to_text(tools.library));
        }
        if (!tools.classpath.empty()) {
            std::string cp;
            for (const stdfs::path& p : tools.classpath) cp += (cp.empty() ? "" : ";") + fs::to_text(p);
            argv.push_back("--cp");
            argv.push_back(cp);
        }
        argv.push_back("@" + fs::to_text(listing));
        std::error_code ec;
        stdfs::remove_all(out.path() / "units", ec);
        fs::make_dirs(out.path() / "units");
        return process::run({.argv = argv, .timeout = std::chrono::minutes(30)});
    };
    process::ProcessResult r = dump(!library.empty());
    if (r.exit_code != 0 && !library.empty()) r = dump(false);   // (a member our library does not have: the program is then refused as using the JDK's)
    if (r.exit_code != 0) {
        const std::string err(r.err.begin(), r.err.end());
        throw ToolError("TreeDump", r.exit_code, "TreeDump failed: " + err.substr(err.size() > 600 ? err.size() - 600 : 0));
    }
    // {path relative to the dump without .json: tree}, in order of the path
    std::vector<std::pair<std::string, stdfs::path>> dumped;
    for (const auto& entry : stdfs::recursive_directory_iterator(out.path() / "units")) {
        if (!entry.is_regular_file() || entry.path().extension() != ".json") continue;
        const std::string name = fs::to_text(entry.path().filename());
        if (name.starts_with("_")) continue;
        std::string rel = stdfs::relative(entry.path(), out.path() / "units").generic_string();
        rel.resize(rel.size() - 5);
        dumped.emplace_back(std::move(rel), entry.path());
    }
    std::sort(dumped.begin(), dumped.end());
    std::vector<Node*> units;
    for (const auto& [rel, file] : dumped) units.push_back(lower_unit(arena, json::load(file), rel));
    return units;
}

}  // namespace cb::java::frontend
