// cb/translate/dotnet/cs2cpp_decl.cpp -- C# into the C++03 back end: classes, members and the lowering of properties into fields and
// accessor methods (see cs2cpp.hpp).

#include <algorithm>
#include <format>

#include "cb/translate/dotnet/cs2cpp_internal.hpp"

namespace cb::translate::dotnet::cs2cpp {

using namespace impl;

namespace {
namespace decl_impl {

bool has_mod(const std::vector<std::string>& mods, std::string_view m) { return std::find(mods.begin(), mods.end(), m) != mods.end(); }

// The name a refusal gives a member: its own, else its symbol's, else `?`.
std::string member_name(const ir::Node* m) {
    if (const auto* md = ir::get<ir::MethodDecl>(m); md != nullptr && md->name && !md->name->empty()) return *md->name;
    ir::SymP sym;
    if (const auto* x = ir::get<ir::FieldDecl>(m)) sym = x->sym;
    else if (const auto* x2 = ir::get<ir::MethodDecl>(m)) sym = x2->sym;
    else if (const auto* x3 = ir::get<ir::Property>(m)) sym = x3->sym;
    else if (const auto* x4 = ir::get<ir::EnumConst>(m)) sym = x4->sym;
    return sym ? sym->name : "?";
}

}  // namespace decl_impl
}  // namespace

void ToJava::klass(ir::Node& cn) {
    auto& c = *ir::get<ir::ClassDecl>(&cn);
    {
        std::vector<std::string> kept;
        for (const std::string& m : c.mods) {
            if (one_of(m, {"public", "abstract"})) kept.push_back(m);
        }
        if (decl_impl::has_mod(c.mods, "sealed")) kept.push_back("final");
        c.mods = std::move(kept);
    }
    if (c.extends) c.extends = ty(c.extends);
    for (ir::TypeP& i : c.implements) i = ty(i);
    std::vector<ir::Node*> out;
    for (ir::Node* m : c.members) {
        try {
            const std::vector<ir::Node*> lowered = member(m, c);
            out.insert(out.end(), lowered.begin(), lowered.end());
        } catch (const Refuse& r) {
            refused_.push_back({std::format("{}.{}", or_none(c.name), decl_impl::member_name(m)), r.category(), r.why()});
            auto* md = ir::get<ir::MethodDecl>(m);
            if (md != nullptr && !body_absent(md->body)) {   // the method stays, its body says why it is not written
                ir::Node* un = make(ir::Unsupported{.kind = r.category(), .raw = r.why()}, ir::any_type());
                ir::Node* es = make(ir::ExprStmt{.expr = un});
                md->body = make(ir::Block{.stmts = {es}});
                out.push_back(m);
            }
        }
    }
    c.members = std::move(out);
}

std::vector<std::string> ToJava::mods(const std::vector<std::string>& in) const {
    std::vector<std::string> out;
    for (const std::string& m : in) {
        if (one_of(m, {"public", "private", "protected", "static", "abstract"})) {
            out.push_back(m);
        } else if (m == "const") {
            out.push_back("static");
            out.push_back("final");
        } else if (m == "sealed") {
            out.push_back("final");
        }
    }
    return out;
}

std::vector<ir::Node*> ToJava::member(ir::Node* m, const ir::ClassDecl& c) {
    if (auto* f = ir::get<ir::FieldDecl>(m)) {
        f->sym = sym(f->sym);
        f->type = ty(f->type);
        f->mods = mods(f->mods);
        if (f->init != nullptr) f->init = expr(f->init);
        return {m};
    }
    if (ir::is<ir::Property>(m)) return lower_prop(m, c);
    if (ir::is<ir::MethodDecl>(m)) return {method(m, c)};
    if (ir::is<ir::EnumConst>(m)) throw Refuse("enum", "an enum");
    throw Refuse("member", "a member " + std::string(ir::kind_name(*m)));
}

std::vector<ir::Node*> ToJava::lower_prop(ir::Node* pn, const ir::ClassDecl& c) {
    auto& p = *ir::get<ir::Property>(pn);
    if (!p.sym) throw Refuse("property", "a property with no symbol");
    const std::string owner = c.qname.value_or("");
    const auto found = props_.find(PropKey(owner, p.sym->name));
    if (found == props_.end())
        throw Refuse("property", std::format("the property {}.{} (its type has no form here)", or_none(c.name), p.sym->name));
    const Accessors acc = found->second;
    const ir::SymP g = acc.getter;
    const ir::SymP s = acc.setter;
    const bool is_static = acc.is_static;
    const ir::TypeP t = g->ret;
    std::vector<ir::Node*> out;
    std::vector<std::string> property_mods;
    for (const std::string& x : p.mods) {
        if (!one_of(x, {"override", "virtual"})) property_mods.push_back(x);
    }
    const std::vector<std::string> mods_ = mods(property_mods);
    const bool is_interface = c.kind && *c.kind == "interface";
    const bool abstract_ = decl_impl::has_mod(p.mods, "abstract") || is_interface;
    bool auto_p = !abstract_;
    if (auto_p) {
        for (const ir::Node* an : p.accessors) {
            const auto* a = ir::get<ir::MethodDecl>(an);
            if (a == nullptr) throw Refuse("property", "an accessor that is no method");
            if (!body_absent(a->body)) {
                auto_p = false;
                break;
            }
        }
    }
    ir::SymP field;
    if (auto_p) {   // the compiler's backing field
        field = ir::make_sym(ir::SymKind::Field, "_" + p.sym->name, c.qname, is_static, t);
        ir::Node* init = p.init != nullptr ? expr(p.init) : nullptr;
        std::vector<std::string> field_mods{"private"};
        if (is_static) field_mods.push_back("static");
        out.push_back(ir::make(arena_, ir::FieldDecl{.sym = field, .type = t, .init = init, .mods = std::move(field_mods)}, {}, pn->line, pn->col));
    }
    const ir::TypeP owner_type = ir::class_type(owner);
    for (ir::Node* an : p.accessors) {
        auto& a = *ir::get<ir::MethodDecl>(an);
        const bool is_get = a.kind && *a.kind == "get";
        const ir::SymP sy = is_get ? g : s;
        if (!sy) continue;
        std::vector<std::string> amods;
        for (const std::string& x : mods_) {
            if (!one_of(x, {"public", "private", "protected"})) amods.push_back(x);
        }
        if (!a.mods.empty()) {
            std::vector<std::string> own = mods(a.mods);
            own.insert(own.end(), amods.begin(), amods.end());
            amods = std::move(own);
        } else {
            amods = mods_;
        }
        ir::Node* body = nullptr;
        if (abstract_) {
            std::vector<std::string> keep;
            for (const std::string& x : amods) {
                if (x != "abstract") keep.push_back(x);
            }
            if (!is_interface) keep.push_back("abstract");
            amods = std::move(keep);
        } else if (body_absent(a.body)) {   // the compiler's own accessor
            ir::Node* qualifier = is_static ? make(ir::TypeName{}, owner_type) : make(ir::This{}, owner_type);
            ir::Node* backing = make(ir::Select{.obj = qualifier, .sym = field}, t);
            if (is_get) {
                body = make(ir::Block{.stmts = {make(ir::Return{.expr = backing})}});
            } else {
                ir::SymP value_sym = ir::make_sym(ir::SymKind::Param, "value", owner + ".set" + p.sym->name, false, t);
                ir::Node* value = make(ir::Name{.sym = value_sym}, t);
                ir::Node* assign = make(ir::Assign{.target = backing, .value = value}, t);
                body = make(ir::Block{.stmts = {make(ir::ExprStmt{.expr = assign})}});
            }
        } else {
            body = block(std::get<ir::Node*>(a.body));
        }
        std::vector<ir::Node*> params;
        if (!is_get) {
            ir::SymP value_sym = ir::make_sym(ir::SymKind::Param, "value", owner + ".set" + p.sym->name, false, t);
            params.push_back(make(ir::Param{.sym = value_sym, .type = t}));
        }
        out.push_back(ir::make(arena_,
                               ir::MethodDecl{.sym = sy, .name = sy->name, .kind = "method", .params = std::move(params), .ret = sy->ret,
                                              .body = body, .mods = std::move(amods)},
                               {}, an->line != 0 ? an->line : pn->line, an->col != 0 ? an->col : pn->col));
    }
    return out;
}

ir::Node* ToJava::method(ir::Node* mn, const ir::ClassDecl& c) {
    auto& m = *ir::get<ir::MethodDecl>(mn);
    std::optional<std::string> name = m.name;
    if (m.name) {
        if (const auto it = object_methods().find(*m.name); it != object_methods().end()) name = it->second;
    }
    const bool is_static = decl_impl::has_mod(m.mods, "static");
    const ir::TypeP string_array = ir::array_of(string_type());
    if (m.kind && *m.kind == "method" && is_str(m.name, "Main") && is_static) {
        name = "main";
        if (m.params.empty()) {
            ir::SymP args_sym = ir::make_sym(ir::SymKind::Param, "args", c.qname.value_or("") + ".main", false, string_array);
            m.params = {make(ir::Param{.sym = args_sym, .type = string_array})};
        }
    }
    if (decl_impl::has_mod(m.mods, "operator")) throw Refuse("operator", "an operator");
    const std::vector<std::string> original_mods = m.mods;   // (before the mapping below)
    m.name = name;
    m.sym = m.sym ? sym(m.sym) : ir::SymP();
    if (m.sym) {
        m.sym->name = name ? *name : "None";
        if (is_str(name, "main")) m.sym->params = {string_array};
    }
    for (ir::Node* pn : m.params) {
        auto* p = ir::get<ir::Param>(pn);
        if (p == nullptr) throw Refuse("member", "a parameter that is no Param");
        if (p->sym && std::any_of(p->sym->refs.begin(), p->sym->refs.end(), [](ir::PassMode r) { return r != ir::PassMode::ByValue; }))
            throw Refuse("ref", "a ref or out parameter");
        p->type = ty(p->type);
        p->sym = sym(p->sym);
    }
    m.ret = ty(m.ret);
    // (the C++03 path has no coroutine: a method that stops in the middle has no form here; the C++17 back end writes both)
    if (decl_impl::has_mod(original_mods, "iterator")) throw Refuse("iterator", "an iterator method (`yield`)");
    if (decl_impl::has_mod(original_mods, "async")) throw Refuse("async", "an async method");
    if (!(m.kind && *m.kind == "ctor")) {
        m.mods = mods(m.mods);
    } else {
        std::vector<std::string> v;
        for (const std::string& x : mods(m.mods)) {
            if (x != "static") v.push_back(x);
        }
        if (decl_impl::has_mod(m.mods, "IMPLICIT")) v.push_back("IMPLICIT");
        m.mods = std::move(v);
    }
    if (c.kind && *c.kind == "interface") {
        std::erase(m.mods, std::string("abstract"));
    }
    if (!body_absent(m.body)) {
        if (!std::holds_alternative<ir::Node*>(m.body)) throw Refuse("list", "the statement list on the C++03 path");
        m.body = block(std::get<ir::Node*>(m.body));
    }
    return mn;
}

}  // namespace cb::translate::dotnet::cs2cpp
