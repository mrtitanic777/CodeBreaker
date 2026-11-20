// translate/dotnet/cs2cpp_stmt.cpp -- C# into the C++03 back end: statements. Each is rewritten in place (the node it answers is the one it
// was given, or a new one in the same arena); what has no form on the C++03 path is refused (see cs2cpp.hpp).

#include <format>

#include "translate/dotnet/cs2cpp_internal.hpp"

namespace cb::translate::dotnet::cs2cpp {

using namespace impl;

namespace {
namespace stmt2_impl {

// An expression with no effects that may be evaluated more than once: a name, `this`, or a member of one.
bool is_plain(const ir::Node* e) {
    if (ir::is<ir::Name>(e) || ir::is<ir::This>(e)) return true;
    const auto* sel = ir::get<ir::Select>(e);
    return sel != nullptr && (ir::is<ir::Name>(sel->obj) || ir::is<ir::This>(sel->obj) || ir::is<ir::TypeName>(sel->obj));
}

const ir::ClassType* class_of_type(const ir::TypeP& t) { return t ? std::get_if<ir::ClassType>(&t->body) : nullptr; }

}  // namespace stmt2_impl
}  // namespace

ir::Node* ToJava::block(ir::Node* bn) {
    auto* b = ir::get<ir::Block>(bn);
    if (b == nullptr) throw Refuse("block", "a body that is not a block");
    std::vector<ir::Node*> ss;
    for (ir::Node* s : b->stmts) ss.push_back(stmt(s));
    return ir::make(arena_, ir::Block{.stmts = std::move(ss)}, {}, bn->line, bn->col);
}

ir::Node* ToJava::stmt_slot(const ir::Suite& suite) {
    if (std::holds_alternative<ir::Node*>(suite)) return stmt(std::get<ir::Node*>(suite));
    throw Refuse("list", "the statement list on the C++03 path");
}

ir::Node* ToJava::stmt(ir::Node* s) {
    if (s == nullptr) throw Refuse("NoneType", "a missing statement");
    const std::string k(ir::kind_name(*s));
    if (ir::is<ir::Block>(s)) return block(s);
    if (auto* l = ir::get<ir::Local>(s)) {
        l->sym = sym(l->sym);
        l->type = ty(l->type);
        if (l->init != nullptr) l->init = expr(l->init);
        return s;
    }
    if (auto* es = ir::get<ir::ExprStmt>(s)) {
        ir::Node* e = es->expr;
        const auto* asg = ir::get<ir::Assign>(e);
        if (asg != nullptr && property_sym(asg->target) != nullptr)
            return make(ir::ExprStmt{.expr = set_prop(e)}, {}, s);
        const auto* un = ir::get<ir::Unary>(e);
        if (un != nullptr && (un->op == "++" || un->op == "--") && property_sym(un->x) != nullptr) {   // p++ as a statement: p += 1
            ir::Node* one = make(ir::Lit{.value = std::int64_t(1)}, ir::i32_type());
            ir::Node* compound = make(ir::Assign{.op = un->op.substr(0, 1), .target = un->x, .value = one}, e->type);
            return make(ir::ExprStmt{.expr = set_prop(compound)}, {}, s);
        }
        if (asg != nullptr && !asg->op) {
            const auto* ix = ir::get<ir::Index>(asg->target);
            if (ix != nullptr && ix->sym) return make(ir::ExprStmt{.expr = set_index(e)}, {}, s);
        }
        es->expr = expr(e);
        return s;
    }
    if (auto* i = ir::get<ir::If>(s)) {
        ir::Node* c = expr(i->cond);
        ir::Node* t = stmt_slot(i->then);
        i->cond = c;
        i->then = t;
        i->orelse = !body_absent(i->orelse) ? stmt_slot(i->orelse) : nullptr;
        return s;
    }
    if (auto* w = ir::get<ir::While>(s)) {
        ir::Node* c = expr(w->cond);
        ir::Node* b = stmt_slot(w->body);
        w->cond = c;
        w->body = b;
        return s;
    }
    if (auto* d = ir::get<ir::DoWhile>(s)) {
        ir::Node* b = stmt_slot(d->body);
        ir::Node* c = expr(d->cond);
        d->body = b;
        d->cond = c;
        return s;
    }
    if (auto* f = ir::get<ir::For>(s)) {
        for (ir::Node*& x : f->init) x = stmt(x);
        f->cond = f->cond != nullptr ? expr(f->cond) : nullptr;
        for (ir::Node*& x : f->update) x = expr(x);
        f->body = stmt_slot(f->body);
        return s;
    }
    if (ir::is<ir::ForEach>(s)) return foreach_(s);
    if (auto* sw = ir::get<ir::Switch>(s)) {
        const auto* et = stmt2_impl::class_of_type(sw->expr != nullptr ? sw->expr->type : ir::TypeP());
        if (et != nullptr && et->name == "System.String") return string_switch(s);
        sw->expr = expr(sw->expr);
        for (ir::Node* cn : sw->cases) {
            auto* c = ir::get<ir::Case>(cn);
            if (c == nullptr) throw Refuse("switch", "a switch holding something that is no case");
            for (ir::Node*& x : c->labels) x = expr(x);
            for (ir::Node*& x : c->body) x = stmt(x);
        }
        return s;
    }
    if (const auto* b = ir::get<ir::Break>(s)) {
        if (b->label && !b->label->empty()) throw Refuse("label", "a labelled jump");
        return s;
    }
    if (const auto* c = ir::get<ir::Continue>(s)) {
        if (c->label && !c->label->empty()) throw Refuse("label", "a labelled jump");
        return s;
    }
    if (ir::is<ir::Empty>(s) || ir::is<ir::Comment>(s)) return s;
    if (auto* r = ir::get<ir::Return>(s)) {
        r->expr = r->expr != nullptr ? expr(r->expr) : nullptr;
        return s;
    }
    if (auto* t = ir::get<ir::Throw>(s)) {
        if (t->expr == nullptr) throw Refuse("rethrow", "`throw;`");
        t->expr = expr(t->expr);
        return s;
    }
    if (auto* t = ir::get<ir::Try>(s)) {
        if (!t->resources.empty()) throw Refuse("using", "using (IDisposable): the C++03 runtime has no Dispose to call");
        t->body = block(t->body);
        for (ir::Node* cn : t->catches) {
            auto* c = ir::get<ir::Catch>(cn);
            if (c == nullptr) throw Refuse("try", "a try holding something that is no catch");
            c->sym = sym(c->sym);
            for (ir::TypeP& x : c->types) x = ty(x);
            c->body = block(c->body);
        }
        t->finally_body = t->finally_body != nullptr ? block(t->finally_body) : nullptr;
        return s;
    }
    if (auto* cc = ir::get<ir::CtorCall>(s)) {
        cc->sym = sym(cc->sym);
        for (ir::Node*& a : cc->args) a = expr(a);
        return s;
    }
    throw Refuse(k, std::format("the statement {} on the C++03 path", k));
}

ir::Node* ToJava::foreach_(ir::Node* sn) {
    auto* s = ir::get<ir::ForEach>(sn);
    ir::Node* varn = stmt(s->var);
    auto* var = ir::get<ir::Local>(varn);
    if (var == nullptr) throw Refuse("foreach", "a for-each variable that is no Local");
    ir::Node* coll = expr(s->iterable);
    if (!stmt2_impl::is_plain(coll)) throw Refuse("foreach", "foreach over an expression with effects");
    ++loops_;
    if (!var->sym) throw Refuse("foreach", "a for-each variable with no symbol");
    const ir::SymP i = ir::make_sym(ir::SymKind::Local, std::format("_i{}", loops_), var->sym->owner, false, ir::i32_type());
    const auto iv = [&] { return make(ir::Name{.sym = i}, ir::i32_type()); };   // (a node of its own at every place it is used)
    const auto again = [&] { return ir::clone(arena_, coll); };
    const ir::TypeP ct = coll->type;
    const auto* cls = stmt2_impl::class_of_type(ct);
    ir::Node* n = nullptr;
    ir::Node* item = nullptr;
    if (ct && ct->kind() == "array") {
        n = make(ir::Length{.arr = coll}, ir::i32_type());
        item = make(ir::Index{.arr = again(), .index = iv()}, var->type);
    } else if (cls != nullptr && cls->name == "java.util.ArrayList") {
        n = call(coll, method_sym("size", "java.util.ArrayList", {}, ir::i32_type()), {}, ir::i32_type());
        item = call(again(), method_sym("get", "java.util.ArrayList", {ir::i32_type()}, ir::typevar_type("E")), {iv()}, var->type);
    } else if (cls != nullptr && cls->name == "java.lang.String") {
        n = call(coll, method_sym("length", "java.lang.String", {}, ir::i32_type()), {}, ir::i32_type());
        item = call(again(), method_sym("charAt", "java.lang.String", {ir::i32_type()}, type_of_kind("u16")), {iv()}, type_of_kind("u16"));
    } else {
        throw Refuse("foreach", "foreach over a " + (ct ? ir::to_text(*ct) : std::string("None")));
    }
    var->init = item;
    ir::Node* body = stmt(s->body);
    std::vector<ir::Node*> stmts{varn};
    if (const auto* bb = ir::get<ir::Block>(body)) stmts.insert(stmts.end(), bb->stmts.begin(), bb->stmts.end());
    else stmts.push_back(body);
    ir::Node* zero = make(ir::Lit{.value = std::int64_t(0)}, ir::i32_type());
    ir::Node* init = make(ir::Local{.sym = i, .type = ir::i32_type(), .init = zero});
    ir::Node* cond = make(ir::Binary{.op = "<", .left = iv(), .right = n}, ir::bool_type());
    ir::Node* update = make(ir::Unary{.op = "++", .x = iv()}, ir::i32_type());
    ir::Node* fb = make(ir::Block{.stmts = std::move(stmts)});
    return make(ir::For{.init = {init}, .cond = cond, .update = {update}, .body = fb}, {}, sn);
}

ir::Node* ToJava::string_switch(ir::Node* sn) {
    auto* s = ir::get<ir::Switch>(sn);
    ir::Node* v = expr(s->expr);
    if (!stmt2_impl::is_plain(v)) throw Refuse("switch", "a switch on a string expression with effects");
    struct Link {
        ir::Node* test;
        std::vector<ir::Node*> body;
    };
    std::vector<Link> chain;
    std::optional<std::vector<ir::Node*>> default_body;
    std::vector<ir::Node*> pending;   // the labels of the sections that share the next body
    for (ir::Node* cn : s->cases) {
        auto* c = ir::get<ir::Case>(cn);
        if (c == nullptr) throw Refuse("switch", "a switch holding something that is no case");
        pending.insert(pending.end(), c->labels.begin(), c->labels.end());
        if (c->body.empty()) {
            if (c->is_default) throw Refuse("switch", "a default label sharing a section");
            continue;
        }
        std::vector<ir::Node*> body = c->body;
        if (const auto* last = ir::get<ir::Break>(body.back()); last != nullptr && !(last->label && !last->label->empty())) body.pop_back();
        for (ir::Node* x : body) {
            for (const ir::Node* n : ir::walk(x)) {
                if (ir::is<ir::Break>(n) || ir::is<ir::Continue>(n)) throw Refuse("switch", "a break or continue inside a string switch section");
            }
        }
        for (ir::Node*& x : body) x = stmt(x);
        if (c->is_default) {
            default_body = std::move(body);
        } else {
            ir::Node* tests = nullptr;
            for (ir::Node* label : pending) {
                ir::Node* lx = expr(label);
                ir::Node* eq = call(ir::clone(arena_, v), method_sym("equals", "java.lang.String", {object_type()}, ir::bool_type()), {lx}, ir::bool_type());
                tests = tests == nullptr ? eq : make(ir::Binary{.op = "||", .left = tests, .right = eq}, ir::bool_type());
            }
            chain.push_back(Link{tests, std::move(body)});
        }
        pending.clear();
    }
    ir::Node* out = nullptr;
    if (default_body) out = make(ir::Block{.stmts = std::move(*default_body)});
    for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
        ir::Node* then = make(ir::Block{.stmts = std::move(it->body)});
        out = make(ir::If{.cond = it->test, .then = then, .orelse = out}, {}, sn);
    }
    return out != nullptr ? out : make(ir::Empty{});
}

ir::Node* ToJava::set_prop(ir::Node* en) {
    auto* e = ir::get<ir::Assign>(en);
    const auto* t = ir::get<ir::Select>(e->target);
    const ir::Sym& property = *t->sym;
    const std::optional<Accessors> acc = prop(property.owner, property.name);
    const auto field_it = auto_.find(PropKey(property.owner.value_or(""), property.name));
    const ir::SymP field = field_it != auto_.end() ? field_it->second : ir::SymP();
    if (acc && !acc->setter && field && !e->op) {   // a get-only auto-property set in its constructor: its field
        ir::Node* sel = make(ir::Select{.obj = expr(t->obj), .sym = field}, field->type);
        ir::Node* value = expr(e->value);
        return make(ir::Assign{.target = sel, .value = value}, field->type);
    }
    if (!acc || !acc->setter) throw Refuse("property", std::format("setting {}.{}", or_none(property.owner), property.name));
    const ir::SymP getter = acc->getter;
    const ir::SymP setter = acc->setter;
    ir::Node* obj = expr(t->obj);
    ir::Node* value = expr(e->value);
    if (e->op) {   // p += v: p = p + v, the current value read through the getter
        if (!(ir::is<ir::This>(obj) || ir::is<ir::TypeName>(obj) || ir::is<ir::Name>(obj)))
            throw Refuse("property", "a compound assignment to a property of an expression");
        ir::Node* current = make(ir::Call{.target = ir::clone(arena_, obj), .sym = getter}, getter->ret);
        value = make(ir::Binary{.op = *e->op, .left = current, .right = value}, getter->ret);
        if (getter->ret && one_of(getter->ret->kind(), {"i8", "i16", "u16"})) value = make(ir::Cast{.x = value}, getter->ret);
    }
    return call(obj, setter, {value}, ir::void_type());
}

ir::Node* ToJava::set_index(ir::Node* en) {
    auto* e = ir::get<ir::Assign>(en);
    const auto* t = ir::get<ir::Index>(e->target);
    const std::optional<std::string> owner = t->sym->owner;
    if (is_str(owner, "System.Collections.Generic.List")) {
        const ir::SymP setter = method_sym("set", "java.util.ArrayList", {ir::i32_type(), ir::typevar_type("E")}, ir::typevar_type("E"));
        ir::Node* target = expr(t->arr);
        if (!std::holds_alternative<ir::Node*>(t->index)) throw Refuse("list", "the expression list on the C++03 path");
        ir::Node* idx = expr(std::get<ir::Node*>(t->index));
        ir::Node* val = expr(e->value);
        return call(target, setter, {idx, val}, ty(e->target->type));
    }
    throw Refuse("indexer", "an indexer of " + or_none(owner));
}

}  // namespace cb::translate::dotnet::cs2cpp
