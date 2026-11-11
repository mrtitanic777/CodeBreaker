// codebreaker/translate/frontends/python/package.cpp -- see package.hpp.

#include "codebreaker/translate/frontends/python/package.hpp"

#include <algorithm>
#include <set>

#include "../../../../../../pyengine/src/astutil.hpp"

namespace codebreaker::translate::frontends::python::pyfe {

namespace {

py::str ps(const char* s) { return py::str(s); }

int64_t lineno_of(const A::Expr* e) { return pyengine::lineno(pyengine::AnyNode(e)); }
int64_t lineno_of(const A::Stmt* s) { return pyengine::lineno(pyengine::AnyNode(s)); }

// q.rsplit('.', 1)[0]
py::str owner_of(const py::str& q) {
    int64_t i = q.rfind(ps("."));
    return i < 0 ? q : q.slice(0, i);
}

// the node's `type` and `sym` slots, whatever kind of node it is (getattr(x, 'type', None) / getattr(x, 'sym', None))
template <class F> void visit_slots(const ir::NodeP& n, F&& f);
template <class K, class F> bool try_kind(const ir::NodeP& n, F& f) {
    if (auto* p = dynamic_cast<K*>(n.get())) {
        p->each(f);
        return true;
    }
    return false;
}
template <class... K, class F> void visit_kinds(const ir::NodeP& n, F& f) { (void)(try_kind<K>(n, f) || ...); }
template <class F> void visit_slots(const ir::NodeP& n, F&& f) {
    visit_kinds<ir::Module, ir::Unit, ir::ClassDecl, ir::FieldDecl, ir::MethodDecl, ir::Param, ir::EnumConst, ir::Property, ir::TypeParam, ir::Block,
                ir::Local, ir::ExprStmt, ir::If, ir::While, ir::DoWhile, ir::For, ir::ForEach, ir::Switch, ir::Case, ir::Break, ir::Continue, ir::Return,
                ir::Throw, ir::Try, ir::Catch, ir::Labeled, ir::Goto, ir::MachineFragment, ir::Sync, ir::Empty, ir::Comment, ir::LocalClass, ir::Assert,
                ir::CtorCall, ir::Yield, ir::Lit, ir::Name, ir::Select, ir::TypeName, ir::This, ir::Super, ir::Call, ir::New, ir::NewArray, ir::Index,
                ir::Length, ir::Unary, ir::Binary, ir::Concat, ir::Assign, ir::Cond, ir::Cast, ir::InstanceOf, ir::ClassLit, ir::Lambda, ir::MethodRef,
                ir::SwitchExpr, ir::AddressOf, ir::FarAddress, ir::Piece, ir::Join, ir::Paren, ir::Unsupported>(n, f);
}

py::list<py::str> strs(std::initializer_list<const char*> xs) {
    py::list<py::str> out;
    for (const char* x : xs) out.append(ps(x));
    return out;
}

}  // namespace

ModFn::ModFn(Pr::Module& m) : qname(m.name + ps(".<module>")), module(&m), node(m.tree.get()) {}

std::unique_ptr<Pr::Program> program_of_tree(const py::str& root, const py::str& package) {
    auto prog = std::make_unique<Pr::Program>();
    prog->add_tree(root, package);
    prog->index();
    return prog;
}

std::unique_ptr<Pr::Program> program_of_sources(const py::dict<py::str, py::str>& sources) {
    auto prog = std::make_unique<Pr::Program>();
    for (const auto& kv : sources.items()) prog->add_module(std::get<0>(kv), ps("<") + std::get<0>(kv) + ps(">"), std::get<1>(kv));
    prog->index();
    return prog;
}

// ---- types in a translation ------------------------------------------------------------------------------------------------------
void _types_of(const ir::TypeP& t, py::set<py::str>& out) {
    if (!t) return;
    if (t->kind == ps("class")) {
        if (!t->name->startswith(ps("py."))) out.add(*t->name);
        for (const ir::TypeP& a : t->args) _types_of(a, out);
    }
}

py::set<py::str> _types_of(const ir::TypeP& t) {
    py::set<py::str> out;
    _types_of(t, out);
    return out;
}

py::set<py::str> _types_in(const py::ref<ir::MethodDecl>& md) {
    py::set<py::str> out;
    for (const ir::NodeP& x : ir::walk(md)) {
        visit_slots(x, [&](const char* name, auto& member, int) {
            using M = std::decay_t<decltype(member)>;
            if constexpr (std::is_same_v<M, ir::TypeP>) {
                if (std::string(name) == "type" && member) _types_of(member, out);
            }
        });
        // (a Local's or a Param's own type: the same slot, named again)
        if (auto* l = dynamic_cast<ir::Local*>(x.get())) _types_of(l->type, out);
        if (auto* p = dynamic_cast<ir::Param*>(x.get())) _types_of(p->type, out);
    }
    _types_of(md->ret, out);
    if (md->params)
        for (const ir::NodeP& p : *md->params) {
            if (auto* pp = dynamic_cast<ir::Param*>(p.get())) _types_of(pp->type, out);
        }
    return out;
}

int64_t _attr_order(Pr::Program&, Pr::Class& c, const py::str& attr) {
    std::vector<py::str> order;
    std::vector<Pr::Fn*> meths;
    for (Pr::Fn* f : c.methods.values()) meths.push_back(f);
    std::stable_sort(meths.begin(), meths.end(), [](Pr::Fn* a, Pr::Fn* b) {
        bool ai = !(a->name == ps("__init__")), bi = !(b->name == ps("__init__"));
        if (ai != bi) return !ai && bi;
        return lineno_of(static_cast<const A::Stmt*>(a->node)) < lineno_of(static_cast<const A::Stmt*>(b->node));
    });
    for (Pr::Fn* f : meths) {
        for (const pyengine::AnyNode& an : pyengine::walk(pyengine::AnyNode(static_cast<const A::Stmt*>(f->node)))) {
            const A::Expr* e = an.expr();
            const A::Attribute* at = A::as<A::Attribute>(e);
            if (at && at->ctx == A::ExprCtx::Store) {
                py::str name(at->attr);
                if (std::find(order.begin(), order.end(), name) == order.end()) order.push_back(name);
            }
        }
    }
    auto it = std::find(order.begin(), order.end(), attr);
    return it != order.end() ? static_cast<int64_t>(it - order.begin()) : static_cast<int64_t>(order.size());
}

bool _any_ref(Pr::Program&, Context& ctx, int64_t s, const py::set<py::str>& used) {
    py::dict<int64_t, py::str> names = ctx.records().names();
    py::str q = ctx.records().module(s) + ps(".") + names.at(s);
    for (const auto& kv : names.items()) {
        int64_t s2 = std::get<0>(kv);
        py::str q2 = ctx.records().module(s2) + ps(".") + std::get<1>(kv);
        if (used.contains(q2) && s2 != s) {
            for (const auto& f : ctx.records().fields(s2)) {
                try {
                    if (_types_of(ctx.ty(std::get<1>(f), 0)).contains(q)) return true;
                } catch (const Refused&) {
                }
            }
        }
    }
    return false;
}

py::set<py::str> used_types(Pr::Program& prog, Context& ctx, const LoweredMap& lowered,
                            const py::dict<std::tuple<py::str, py::str>, py::ref<ir::FieldDecl>>& global_decls) {
    py::set<py::str> used;
    for (const auto& kv : lowered.items()) {
        const Lowered& l = std::get<1>(kv);
        if (l.fl && std::holds_alternative<py::ref<ir::MethodDecl>>(l.m)) used.update(_types_in(std::get<py::ref<ir::MethodDecl>>(l.m)));
    }
    for (const auto& d : global_decls.values()) used.update(_types_of(d->type));
    py::dict<py::str, int64_t> recs;                                     // '%s.%s' % (module, name) -> root
    py::dict<int64_t, py::str> names = ctx.records().names();
    for (const auto& kv : names.items()) recs.set(ctx.records().module(std::get<0>(kv)) + ps(".") + std::get<1>(kv), std::get<0>(kv));
    std::vector<py::str> todo(used.begin(), used.end());
    while (!todo.empty()) {
        py::str q = todo.back();
        todo.pop_back();
        py::set<py::str> more;
        if (recs.contains(q)) {
            for (const auto& f : ctx.records().fields(recs.at(q))) {
                try {
                    more.update(_types_of(ctx.ty(std::get<1>(f), 0, ps("field ") + std::get<0>(f))));
                } catch (const Refused&) {
                }
            }
        } else if (prog.classes.contains(q)) {
            if (prog.attrs.contains(q)) {
                for (const py::str& attr : prog.attrs.at(q).keys()) {
                    try {
                        more.update(_types_of(ctx.ty(prog.attr_type(q, attr), 0, ps("attribute ") + attr)));
                    } catch (const Refused&) {
                    }
                }
            }
            for (const py::str& b : prog.classes.at(q)->bases) more.add(b);
        }
        for (const py::str& x : more) {
            if (!used.contains(x)) {
                used.add(x);
                todo.push_back(x);
            }
        }
    }
    return used;
}

std::shared_ptr<ModuleResult> build_unit(Pr::Program& prog, Context& ctx, Lowering& lowering, const py::str& mod, const LoweredMap& lowered,
                                         const py::dict<std::tuple<py::str, py::str>, py::ref<ir::FieldDecl>>& global_decls,
                                         const std::optional<py::set<py::str>>& used_all) {
    Pr::Module* m = prog.modules.at(mod);
    auto r = std::make_shared<ModuleResult>(mod);
    std::vector<Pr::Fn*> fns;
    for (Pr::Fn* f : prog.fns.values())
        if (f->module == m) fns.push_back(f);
    for (Pr::Fn* f : fns) {
        if (!lowered.contains(f->qname)) continue;
        const Lowered& got = lowered.at(f->qname);
        r->lines.set(f->qname, f->lines);
        if (std::holds_alternative<Refused>(got.m)) {
            r->refused.set(f->qname, std::get<Refused>(got.m).issue);
        } else {
            r->translated.set(f->qname, std::get<py::ref<ir::MethodDecl>>(got.m));
            r->calls.set(f->qname, got.fl->calls);
        }
    }
    py::list<ir::NodeP> types;
    // records declared here that the translation uses
    py::set<py::str> used;
    for (const auto& md : r->translated.values()) used.update(_types_in(md));
    for (const auto& kv : global_decls.items())
        if (std::get<0>(std::get<0>(kv)) == mod) used.update(_types_of(std::get<1>(kv)->type));
    const py::set<py::str>& everywhere = used_all ? *used_all : used;
    struct Rec {
        int64_t s;
        py::str q, name;
    };
    std::vector<Rec> recs;
    py::dict<int64_t, py::str> rnames = ctx.records().names();
    for (const auto& kv : rnames.items()) {
        int64_t s = std::get<0>(kv);
        py::str q = ctx.records().module(s) + ps(".") + std::get<1>(kv);
        if (ctx.records().module(s) == mod && (everywhere.contains(q) || used.contains(q) || _any_ref(prog, ctx, s, used))) recs.push_back({s, q, std::get<1>(kv)});
    }
    for (const Rec& rec : recs) {
        py::list<ir::NodeP> members;
        try {
            for (const auto& f : ctx.records().fields(rec.s)) {
                ir::TypeP t = ctx.ty(std::get<1>(f), 0, ps("field ") + std::get<0>(f));
                auto fd = std::make_shared<ir::FieldDecl>();
                fd->sym = std::make_shared<ir::Sym>(ps("field"), std::get<0>(f), rec.q, false, t);
                fd->type = t;
                fd->mods = std::get<2>(f) ? py::list<py::str>() : strs({"optional"});
                members.append(fd);
            }
        } catch (const Refused& e) {
            members = py::list<ir::NodeP>();
            auto c = std::make_shared<ir::Comment>();
            c->text = ps("a field has no type: ") + e.issue.reason;
            members.append(c);
        }
        auto cd = std::make_shared<ir::ClassDecl>();
        cd->name = rec.name;
        cd->qname = rec.q;
        cd->kind = ps("record");
        cd->mods = ctx.records().is_ref(rec.s) ? strs({"ref"}) : py::list<py::str>();
        cd->typeparams = py::list<py::str>();
        cd->implements = py::list<ir::TypeP>();
        cd->members = members;
        types.append(cd);
    }
    // classes
    for (Pr::Class* c : m->classes.values()) {
        py::list<ir::NodeP> methods;
        for (Pr::Fn* f : c->methods.values())
            if (r->translated.contains(f->qname)) methods.append(r->translated.at(f->qname));
        if (methods.empty() && !used.contains(c->qname) && !everywhere.contains(c->qname)) continue;
        py::list<ir::NodeP> members;
        py::dict<py::str, P::T> attrs = prog.attrs.contains(c->qname) ? prog.attrs.at(c->qname).copy() : py::dict<py::str, P::T>();
        StrSetLocal:;
        {
            Pr::StrSet any = prog.assigned_attrs_any(c->qname);
            for (const py::str& name : c->class_attrs.keys()) {
                if (any.count(name) && !attrs.contains(name)) {       // a class value an instance assigns is the instance's
                    std::tuple<py::str, py::str> key(c->qname, name);
                    attrs.set(name, prog.class_attr_types.contains(key) ? prog.class_attr_types.at(key) : P::BOTTOM());
                }
            }
        }
        std::vector<std::tuple<int64_t, py::str, P::T>> ordered;
        for (const auto& kv : attrs.items()) ordered.emplace_back(_attr_order(prog, *c, std::get<0>(kv)), std::get<0>(kv), std::get<1>(kv));
        std::stable_sort(ordered.begin(), ordered.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
        for (const auto& item : ordered) {
            const py::str& attr = std::get<1>(item);
            ir::TypeP it;
            try {
                it = ctx.ty(prog.attr_type(c->qname, attr), 0, ps("attribute ") + attr);
            } catch (const Refused& e) {
                auto cm = std::make_shared<ir::Comment>();
                cm->text = attr + ps(": not translated (") + e.issue.reason + ps(")");
                members.append(cm);
                continue;
            }
            ir::NodeP init;
            const A::Expr* v = c->class_attrs.contains(attr) ? c->class_attrs.at(attr) : nullptr;
            if (const A::Constant* k = A::as<A::Constant>(v))
                if (k->value.kind != A::ConstValue::Kind::None && k->value.kind != A::ConstValue::Kind::Bytes) init = lowering.lit(k->value, it);
            auto fd = std::make_shared<ir::FieldDecl>();
            fd->sym = std::make_shared<ir::Sym>(ps("field"), attr, c->qname, false, it);
            fd->type = it;
            fd->init = init;
            fd->mods = py::list<py::str>();
            members.append(fd);
        }
        for (const auto& kv : c->class_attrs.items()) {
            const py::str& name = std::get<0>(kv);
            const A::Expr* v = std::get<1>(kv);
            auto owner = prog.attr_owner(c->qname, name);
            if (attrs.contains(name) || !(owner && *owner == c->qname)) continue;
            std::tuple<py::str, py::str> key(c->qname, name);
            if (!prog.class_attr_types.contains(key) || !A::as<A::Constant>(v)) continue;
            ir::TypeP it;
            try {
                it = ctx.ty(prog.class_attr_types.at(key), 0, ps("class attribute ") + name);
            } catch (const Refused&) {
                continue;
            }
            auto fd = std::make_shared<ir::FieldDecl>();
            fd->sym = std::make_shared<ir::Sym>(ps("field"), name, c->qname, true, it);
            fd->type = it;
            fd->init = lowering.lit(A::as<A::Constant>(v)->value, it);
            fd->mods = strs({"static", "const"});
            members.append(fd);
        }
        members.extend(methods);
        py::str base_name = c->bases.empty() ? py::str("") : c->bases[0];
        ir::TypeP base = c->bases.empty() ? nullptr : std::make_shared<ir::Type>(ps("class"), base_name);
        py::list<py::str> mods;
        if (c->exception) mods.append(ps("exception:") + *c->exception);
        for (Pr::Fn* f : c->methods.values()) {
            if (r->refused.contains(f->qname)) {
                auto cm = std::make_shared<ir::Comment>();
                cm->text = ps("not translated: ") + f->name + ps(" -- ") + r->refused.at(f->qname).reason;
                members.append(cm);
            }
        }
        auto cd = std::make_shared<ir::ClassDecl>();
        cd->name = c->name;
        cd->qname = c->qname;
        cd->kind = ps("class");
        cd->mods = mods;
        cd->typeparams = py::list<py::str>();
        cd->extends = base;
        cd->implements = py::list<ir::TypeP>();
        cd->members = members;
        types.append(cd);
    }
    // the module's functions and names
    py::list<ir::NodeP> members;
    {
        std::vector<py::ref<ir::FieldDecl>> mine;
        for (const auto& kv : global_decls.items())
            if (std::get<0>(std::get<0>(kv)) == mod) mine.push_back(std::get<1>(kv));
        std::stable_sort(mine.begin(), mine.end(), [](const auto& a, const auto& b) { return a->line < b->line; });
        for (const auto& d : mine) members.append(d);
    }
    for (Pr::Fn* f : m->functions.values()) {
        if (r->translated.contains(f->qname)) {
            members.append(r->translated.at(f->qname));
        } else if (r->refused.contains(f->qname)) {
            auto cm = std::make_shared<ir::Comment>();
            cm->text = ps("not translated: ") + f->name + ps(" -- ") + r->refused.at(f->qname).reason;
            members.append(cm);
        }
    }
    {
        int64_t dot = mod.rfind(ps("."));
        auto cd = std::make_shared<ir::ClassDecl>();
        cd->name = dot < 0 ? mod : mod.slice(dot + 1, mod.size());
        cd->qname = mod;
        cd->kind = ps("module");
        cd->mods = py::list<py::str>();
        cd->typeparams = py::list<py::str>();
        cd->implements = py::list<ir::TypeP>();
        cd->members = members;
        types.append(cd);
    }
    std::set<std::u32string> deps;
    auto add_dep = [&](const py::str& q) {
        py::str owner = prog.modules.contains(q) ? q : owner_of(q);           // (a module's own names: the module)
        if (!(owner == mod) && prog.modules.contains(owner)) deps.insert(owner.u32());
    };
    for (const py::str& q : used) add_dep(q);
    for (const auto& md : r->translated.values()) {
        for (const ir::NodeP& x : ir::walk(md)) {
            visit_slots(x, [&](const char* name, auto& member, int) {
                using M = std::decay_t<decltype(member)>;
                if constexpr (std::is_same_v<M, ir::SymP>) {
                    if (std::string(name) == "sym" && member && member->owner && !(*member->owner == ps("")) && !(*member->owner == ps("py"))) {
                        const py::str& o = *member->owner;
                        py::str own = prog.modules.contains(o) ? o : owner_of(o);
                        if (prog.modules.contains(own) && !(own == mod)) deps.insert(own.u32());
                    }
                }
            });
        }
    }
    for (const ir::NodeP& decl : types) {              // what the declarations name: their modules too
        py::set<py::str> names;
        auto* cd = dynamic_cast<ir::ClassDecl*>(decl.get());
        if (cd->extends) names.add(*cd->extends->name);
        if (cd->members)
            for (const ir::NodeP& x : *cd->members)
                if (auto* fd = dynamic_cast<ir::FieldDecl*>(x.get())) names.update(_types_of(fd->type));
        for (const py::str& q : names) add_dep(q);
    }
    auto unit = std::make_shared<ir::Unit>();
    unit->path = m->path;
    unit->package = mod;
    py::list<py::str> imports;
    for (const auto& d : deps) imports.append(py::str(d));
    unit->imports = imports;
    unit->types = types;
    unit->lang = ps("python");
    r->unit = unit;
    return r;
}

py::dict<py::str, std::shared_ptr<ModuleResult>> translate(Pr::Program& prog, Lowering& lowering, const std::optional<py::set<py::str>>& modules,
                                                            const py::str& int_policy, const std::optional<py::set<py::str>>& fns) {
    std::shared_ptr<Context> ctx = lowering.context(prog, int_policy);
    std::set<std::u32string> wanted;
    if (modules) {
        for (const py::str& s : *modules) wanted.insert(s.u32());
    } else {
        for (const py::str& s : prog.modules.keys()) wanted.insert(s.u32());
    }
    std::vector<Pr::Fn*> todo;
    for (Pr::Fn* f : prog.fns.values())
        if (!fns || fns->contains(f->qname)) todo.push_back(f);
    LoweredMap lowered = lowering.lower_functions(prog, *ctx, todo);
    // module names the translations read: lowered as their modules' top level, or refused with them
    py::dict<std::tuple<py::str, py::str>, Refused> bad_globals;
    py::dict<std::tuple<py::str, py::str>, py::ref<ir::FieldDecl>> global_decls;
    std::set<std::tuple<std::u32string, std::u32string>> needed;
    for (const auto& kv : lowered.items()) {
        const Lowered& l = std::get<1>(kv);
        if (l.fl && !std::holds_alternative<Refused>(l.m))
            for (const auto& g : l.fl->globals_used) needed.insert({std::get<0>(g).u32(), std::get<1>(g).u32()});
    }
    py::dict<py::str, std::shared_ptr<infer::ModuleEval>> mevals;
    for (const auto& nk : needed) {
        py::str mod{std::get<0>(nk)}, name{std::get<1>(nk)};
        std::tuple<py::str, py::str> key(mod, name);
        Pr::Module* mm = prog.modules.at(mod);
        std::vector<const A::Expr*> values = mm->assigns.contains(name) ? mm->assigns.at(name) : std::vector<const A::Expr*>();
        if (values.size() != 1 || values[0] == nullptr) {
            bad_globals.set(key, Refused(lowering.issue(ps("dynamic"), py::percent(ps("module name %s assigned %d times"), name, static_cast<int64_t>(values.size())))));
            continue;
        }
        if (!mevals.contains(mod)) {
            auto me = std::make_shared<infer::ModuleEval>(prog, mm, true);
            me->run();
            mevals.set(mod, me);
        }
        ModFn modfn(*mm);
        std::shared_ptr<FnLower> fl = lowering.fn_lower(*ctx, modfn, *mevals.at(mod));
        try {
            int64_t line = lineno_of(values[0]);
            ir::TypeP t = ctx->ty(prog.global_type(mod, name), line, ps("module name ") + name);
            ir::NodeP init = fl->coerce(fl->expr(values[0]), t);
            if (fl->has_pre()) throw Refused(lowering.issue(ps("syntax"), py::percent(ps("module name %s needs statements"), name), line));
            auto fd = std::make_shared<ir::FieldDecl>();
            fd->sym = std::make_shared<ir::Sym>(ps("global"), name, mod, true, t);
            fd->type = t;
            fd->init = init;
            py::list<py::str> mods = strs({"static"});
            if (t->kind == ps("i64") || t->kind == ps("bool") || t->kind == ps("f64") ||
                (t->name && (*t->name == ps("py.str") || *t->name == ps("py.bytes") || *t->name == ps("py.tuple"))))
                mods.append(ps("const"));
            fd->mods = mods;
            fd->line = line;
            global_decls.set(key, fd);
            for (const auto& g : fl->globals_used) {
                std::tuple<std::u32string, std::u32string> gk{std::get<0>(g).u32(), std::get<1>(g).u32()};
                if (!needed.count(gk) && !(std::get<0>(g) == mod && std::get<1>(g) == name))
                    bad_globals.set(key, Refused(lowering.issue(ps("syntax"), py::percent(ps("module name %s refers to another"), name))));
            }
        } catch (const Refused& r) {
            bad_globals.set(key, r);
        }
    }
    for (const auto& kv : lowered.items()) {
        const py::str& q = std::get<0>(kv);
        const Lowered& l = std::get<1>(kv);
        if (!l.fl || std::holds_alternative<Refused>(l.m)) continue;
        std::vector<std::tuple<std::u32string, std::u32string>> sorted;
        for (const auto& g : l.fl->globals_used) sorted.push_back({std::get<0>(g).u32(), std::get<1>(g).u32()});
        std::sort(sorted.begin(), sorted.end());
        for (const auto& g : sorted) {
            std::tuple<py::str, py::str> key{py::str(std::get<0>(g)), py::str(std::get<1>(g))};
            if (bad_globals.contains(key)) {
                Lowered nl;
                nl.fl = l.fl;
                nl.m = Refused(lowering.issue(ps("calls-refused"), py::percent(ps("module name %s (%s)"), std::get<1>(key), bad_globals.at(key).issue.reason.slice(0, 60))));
                lowered.set(q, nl);
                break;
            }
        }
    }
    lowering.close_refusals(prog, lowered);
    py::set<py::str> used = used_types(prog, *ctx, lowered, global_decls);
    py::dict<py::str, std::shared_ptr<ModuleResult>> results;
    std::vector<py::str> pending;
    for (const auto& w : wanted)
        if (prog.modules.contains(py::str(w))) pending.push_back(py::str(w));
    while (!pending.empty()) {                                       // (and the modules whose declarations they need)
        py::str mod = pending.front();
        pending.erase(pending.begin());
        if (results.contains(mod)) continue;
        results.set(mod, build_unit(prog, *ctx, lowering, mod, lowered, global_decls, used));
        for (const py::str& dep : *results.at(mod)->unit->imports) {
            if (!results.contains(dep) && std::find(pending.begin(), pending.end(), dep) == pending.end()) pending.push_back(dep);
        }
    }
    return results;
}

}  // namespace codebreaker::translate::frontends::python::pyfe
