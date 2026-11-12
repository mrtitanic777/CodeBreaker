#include "cb/translate/frontends/python/package.hpp"

#include <algorithm>
#include <concepts>
#include <format>
#include <map>
#include <variant>

#include "cb/core/text.hpp"
#include "cb/translate/frontends/python/methods.hpp"
#include "cb/translate/frontends/python/pyast.hpp"

namespace cb::translate::frontends::python::package {

namespace syntax = ::pyfront;
using ir::Node;
using program::Class;
using program::Fn;
using program::Module;
using program::Program;

std::unique_ptr<Program> program_of_tree(const std::filesystem::path& root, const std::string& package) {
    auto program = std::make_unique<Program>();
    program->add_tree(root, package);
    program->index();
    return program;
}

std::unique_ptr<Program> program_of_sources(const std::vector<std::pair<std::string, std::string>>& sources) {
    auto program = std::make_unique<Program>();
    for (const auto& [name, source] : sources)
        program->add_module(name, source);
    program->index();
    return program;
}

namespace {

using Lowered = lower::Lowered;
using LoweredMap = OrderedMap<std::string, Lowered>;
using GlobalKey = std::pair<std::string, std::string>;   // (module, name)
using GlobalDecls = std::map<GlobalKey, Node*>;          // sorted by (module, name): the order they are lowered in

// ---- types ----------------------------------------------------------------------------------------------------------------

void add_types_of(const ir::TypeP& type, std::set<std::string>& out) {
    if (!type)
        return;
    const auto* cls = std::get_if<ir::ClassType>(&type->body);
    if (!cls)
        return;
    if (!cls->name.starts_with("py."))
        out.insert(cls->name);
    for (const ir::TypeP& a : cls->args)
        add_types_of(a, out);
}

// The L2 type of an inference type that may be missing (an attribute the program has no type for).
ir::TypeP type_or_refuse(lower::Context& ctx, const std::optional<pytypes::Type>& t, std::uint32_t line, std::string_view what) {
    if (!t)
        throw lower::Refused(lower::RefusalClass::Untyped, std::string(what), line);
    return ctx.type_of(*t, line, what);
}

// The first segment of a qualified name that is a module of the program: the name itself, else what precedes its last dot.
std::optional<std::string> owning_module(Program& prog, const std::string& qualified) {
    if (prog.modules.contains(qualified))
        return qualified;
    const std::size_t dot = qualified.rfind('.');
    if (dot == std::string::npos)
        return std::nullopt;
    const std::string owner = qualified.substr(0, dot);
    return prog.modules.contains(owner) ? std::optional<std::string>(owner) : std::nullopt;
}

// The records and classes the lowered functions and the module names use, and -- to a fixpoint -- the types of those records'
// fields and classes' attributes and the classes' bases.
std::set<std::string> used_types(Program& prog, lower::Context& ctx, const LoweredMap& lowered, const GlobalDecls& global_decls) {
    std::set<std::string> used;
    for (const auto& [q, l] : lowered)
        if (l.decl && !l.is_refused() && l.lowering)
            for (const std::string& t : types_in(l.decl))
                used.insert(t);
    for (const auto& [key, decl] : global_decls)
        if (const auto* field = ir::get<ir::FieldDecl>(decl))
            add_types_of(field->type, used);

    std::map<std::string, program::SiteId> records;   // "module.Name" -> its site
    for (const auto& [root, name] : ctx.records().names())
        records[ctx.qualified_record(root)] = root;

    std::vector<std::string> todo(used.begin(), used.end());
    while (!todo.empty()) {
        const std::string q = todo.back();
        todo.pop_back();
        std::set<std::string> more;
        if (const auto record = records.find(q); record != records.end()) {
            for (const lower::RecordField& field : ctx.records().fields(record->second)) {
                try {
                    add_types_of(ctx.type_of(field.type, 0, "field " + field.key), more);
                } catch (const lower::Refused&) {   // a field with no type adds nothing: the unit says so where it declares it
                }
            }
        } else if (const auto cls = prog.classes.find(q)) {
            if (const auto attrs = prog.attrs.find(q); attrs != prog.attrs.end()) {
                for (const auto& [attr, ignored] : attrs->second) {
                    try {
                        add_types_of(type_or_refuse(ctx, prog.attr_type(q, attr), 0, "attribute " + attr), more);
                    } catch (const lower::Refused&) {
                    }
                }
            }
            for (const std::string& base : (*cls)->bases)
                more.insert(base);
        }
        for (const std::string& x : more)
            if (used.insert(x).second)
                todo.push_back(x);
    }
    return used;
}

// A record whose type appears inside another used type (a field of a record that is used).
bool record_inside_used(lower::Context& ctx, program::SiteId root, const std::set<std::string>& used) {
    const std::string q = ctx.qualified_record(root);
    for (const auto& [other, name] : ctx.records().names()) {
        if (other == root || !used.contains(ctx.qualified_record(other)))
            continue;
        for (const lower::RecordField& field : ctx.records().fields(other)) {
            try {
                std::set<std::string> inside;
                add_types_of(ctx.type_of(field.type, 0), inside);
                if (inside.contains(q))
                    return true;
            } catch (const lower::Refused&) {
            }
        }
    }
    return false;
}

// The attributes of a class in the order its methods first assign them, `__init__` first.
std::vector<std::string> attribute_order(const Class& c) {
    std::vector<const Fn*> methods;
    for (const auto& [name, f] : c.methods)
        methods.push_back(f);
    std::stable_sort(methods.begin(), methods.end(), [](const Fn* a, const Fn* b) {
        return std::pair(a->name != "__init__", a->node->pos.lineno) < std::pair(b->name != "__init__", b->node->pos.lineno);
    });
    std::vector<std::string> order;
    for (const Fn* f : methods) {
        for (const pyast::AnyNode& node : pyast::walk(pyast::AnyNode(static_cast<const syntax::Stmt*>(f->node)))) {
            const auto* attribute = node.expr() ? syntax::as<syntax::Attribute>(node.expr()) : nullptr;
            if (attribute && attribute->ctx == syntax::ExprCtx::Store && std::ranges::find(order, attribute->attr) == order.end())
                order.push_back(attribute->attr);
        }
    }
    return order;
}

// ---- declarations ---------------------------------------------------------------------------------------------------------------

Node* comment(lower::Context& ctx, std::string text) { return ir::make(ctx.arena(), ir::Comment{.text = std::move(text)}); }

// The literal of a class-level constant, as the value of an attribute of type `type`; nullptr for a value L2 has no literal for.
Node* constant_literal(lower::Context& ctx, const syntax::Constant& c, const ir::TypeP& type) {
    using K = syntax::ConstValue::Kind;
    ir::LitValue value;
    switch (c.value.kind) {
    case K::None:
        break;
    case K::True:
    case K::False:
        value = c.value.kind == K::True;
        break;
    case K::Int:
        if (c.value.fits_i64) {
            value = c.value.i64;
        } else if (const auto big = Wide::from_decimal(c.value.text); big && big->to_u64()) {
            value = *big->to_u64();
        } else {
            return nullptr;
        }
        break;
    case K::Float:
        value = c.value.f;
        break;
    case K::Str:
        value = c.value.text;
        break;
    default:
        return nullptr;
    }
    return ir::make(ctx.arena(), ir::Lit{.value = std::move(value)}, type);
}

// A class-level constant that makes a class's attribute the constant of its type (an instance attribute of that name keeps it
// out: then the value is the instance's).
bool plain_constant(const syntax::Expr* e) {
    const auto* c = syntax::as<syntax::Constant>(e);
    return c && c->value.kind != syntax::ConstValue::Kind::None && c->value.kind != syntax::ConstValue::Kind::Bytes;
}

template <class K>
concept HasSym = requires(K& k) { { k.sym } -> std::convertible_to<ir::SymP>; };
template <class K>
concept HasType = requires(K& k) { { k.type } -> std::convertible_to<ir::TypeP>; };

// The symbol a node names or declares, if it has one.
ir::SymP sym_of(Node& node) {
    return ir::visit(node, [](auto& k) -> ir::SymP {
        if constexpr (HasSym<std::remove_cvref_t<decltype(k)>>)
            return k.sym;
        else
            return ir::SymP();
    });
}

// The type a declaration (a field, a local, a parameter) holds in its own slot, if it has one.
ir::TypeP own_type_of(Node& node) {
    return ir::visit(node, [](auto& k) -> ir::TypeP {
        if constexpr (HasType<std::remove_cvref_t<decltype(k)>>)
            return k.type;
        else
            return ir::TypeP();
    });
}

// The module `mod` as a unit: its records, classes, names and functions, and the modules they name.
lower::ModuleResult build_unit(Program& prog, lower::Context& ctx, const std::string& mod, const LoweredMap& lowered,
                               const GlobalDecls& global_decls, const std::set<std::string>& used_everywhere) {
    Module& m = **prog.modules.find(mod);
    lower::ModuleResult r;
    r.name = mod;
    for (const auto& [q, f] : prog.fns) {
        if (f->module != &m)
            continue;
        const Lowered* got = lowered.find(q);
        if (!got)
            continue;
        r.lines.insert_or_assign(q, f->lines);
        if (got->is_refused()) {
            r.refused.insert_or_assign(q, *got->refused);
        } else {
            r.translated.insert_or_assign(q, got->decl);
            if (got->lowering) {
                r.calls.insert_or_assign(q, got->lowering->calls());
                r.made.insert_or_assign(q, got->lowering->classes_made());
                r.globals.insert_or_assign(q, got->lowering->globals_used());
            }
        }
    }

    // what the translations here use
    std::set<std::string> used;
    for (const auto& [q, md] : r.translated)
        for (const std::string& t : types_in(md))
            used.insert(t);
    for (const auto& [key, decl] : global_decls)
        if (key.first == mod)
            if (const auto* field = ir::get<ir::FieldDecl>(decl))
                add_types_of(field->type, used);

    std::vector<Node*> types;
    // records declared here that the translation uses
    for (const auto& [root, name] : ctx.records().names()) {
        const std::string q = ctx.qualified_record(root);
        if (ctx.records().module(root) != mod || !(used_everywhere.contains(q) || used.contains(q) || record_inside_used(ctx, root, used)))
            continue;
        std::vector<Node*> members;
        try {
            for (const lower::RecordField& field : ctx.records().fields(root)) {
                const ir::TypeP t = ctx.type_of(field.type, 0, "field " + field.key);
                members.push_back(ir::make(ctx.arena(), ir::FieldDecl{.sym = ir::make_sym(ir::SymKind::Field, field.key, q, false, t),
                                                                      .type = t,
                                                                      .mods = field.required ? std::vector<std::string>{} : std::vector<std::string>{"optional"}}));
            }
        } catch (const lower::Refused& e) {   // the record cannot be declared: the unit says why in its place
            members = {comment(ctx, "a field has no type: " + e.refusal().reason())};
        }
        types.push_back(ir::make(ctx.arena(), ir::ClassDecl{.name = name,
                                                            .qname = q,
                                                            .kind = "record",
                                                            .mods = ctx.records().is_ref(root) ? std::vector<std::string>{"ref"} : std::vector<std::string>{},
                                                            .members = std::move(members)}));
    }

    // classes
    for (const auto& [class_name, cptr] : m.classes) {
        const Class& c = *cptr;
        std::vector<Node*> methods;
        for (const auto& [name, f] : c.methods)
            if (const auto* translated = r.translated.find(f->qname))
                methods.push_back(*translated);
        if (methods.empty() && !used.contains(c.qname) && !used_everywhere.contains(c.qname))
            continue;

        // the attributes: those the methods assign, and the class-level values an instance assigns too
        OrderedMap<std::string, pytypes::Type> attrs;
        if (const auto known = prog.attrs.find(c.qname); known != prog.attrs.end())
            for (const auto& [attr, t] : known->second)
                attrs.insert_or_assign(attr, t);
        const program::StringSet assigned = prog.assigned_attrs_any(c.qname);
        for (const auto& [name, value] : c.class_attrs) {
            if (!assigned.contains(name) || attrs.contains(name))
                continue;
            const auto at = prog.class_attr_types.find({c.qname, name});
            attrs.insert_or_assign(name, at == prog.class_attr_types.end() ? pytypes::bottom() : at->second);
        }
        const std::vector<std::string> order = attribute_order(c);
        std::vector<std::string> attr_names;
        for (const auto& [attr, t] : attrs)
            attr_names.push_back(attr);
        const auto position = [&](const std::string& a) {
            const auto it = std::ranges::find(order, a);
            return static_cast<std::size_t>(it - order.begin());   // an attribute no method assigns comes after those that are
        };
        std::stable_sort(attr_names.begin(), attr_names.end(), [&](const std::string& a, const std::string& b) { return position(a) < position(b); });

        std::vector<Node*> members;
        for (const std::string& attr : attr_names) {
            ir::TypeP it;
            try {
                it = type_or_refuse(ctx, prog.attr_type(c.qname, attr), 0, "attribute " + attr);
            } catch (const lower::Refused& e) {
                members.push_back(comment(ctx, std::format("{}: not translated ({})", attr, e.refusal().reason())));
                continue;
            }
            Node* init = nullptr;
            if (const syntax::Expr* const* value = c.class_attrs.find(attr); value && plain_constant(*value))
                init = constant_literal(ctx, *syntax::as<syntax::Constant>(*value), it);
            members.push_back(ir::make(ctx.arena(), ir::FieldDecl{.sym = ir::make_sym(ir::SymKind::Field, attr, c.qname, false, it), .type = it, .init = init}));
        }
        for (const auto& [name, value] : c.class_attrs) {   // class-level constants no instance assigns: static const members
            if (attrs.contains(name) || prog.attr_owner(c.qname, name) != c.qname)
                continue;
            const auto at = prog.class_attr_types.find({c.qname, name});
            const auto* constant = syntax::as<syntax::Constant>(value);
            if (at == prog.class_attr_types.end() || !constant)
                continue;
            try {
                const ir::TypeP it = ctx.type_of(at->second, 0, "class attribute " + name);
                members.push_back(ir::make(ctx.arena(), ir::FieldDecl{.sym = ir::make_sym(ir::SymKind::Field, name, c.qname, true, it),
                                                                      .type = it,
                                                                      .init = constant_literal(ctx, *constant, it),
                                                                      .mods = {"static", "const"}}));
            } catch (const lower::Refused&) {   // a constant with no type is left out
            }
        }
        members.insert(members.end(), methods.begin(), methods.end());

        std::vector<std::string> mods;
        if (c.exception)
            mods.push_back("exception:" + *c.exception);
        for (const auto& [name, f] : c.methods)
            if (const lower::Refusal* refused = r.refused.find(f->qname))
                members.push_back(comment(ctx, std::format("not translated: {} -- {}", f->name, refused->reason())));
        types.push_back(ir::make(ctx.arena(), ir::ClassDecl{.name = c.name,
                                                            .qname = c.qname,
                                                            .kind = "class",
                                                            .mods = std::move(mods),
                                                            .extends = c.bases.empty() ? ir::TypeP() : ir::class_type(c.bases.front()),
                                                            .members = std::move(members)}));
    }

    // the module's names and functions
    std::vector<Node*> members;
    std::vector<Node*> names_here;
    for (const auto& [key, decl] : global_decls)
        if (key.first == mod)
            names_here.push_back(decl);
    std::stable_sort(names_here.begin(), names_here.end(), [](const Node* a, const Node* b) { return a->line < b->line; });
    members.insert(members.end(), names_here.begin(), names_here.end());
    for (const auto& [name, f] : m.functions) {
        if (const auto* translated = r.translated.find(f->qname))
            members.push_back(*translated);
        else if (const lower::Refusal* refused = r.refused.find(f->qname))
            members.push_back(comment(ctx, std::format("not translated: {} -- {}", f->name, refused->reason())));
    }
    const std::size_t dot = mod.rfind('.');
    types.push_back(ir::make(ctx.arena(), ir::ClassDecl{.name = dot == std::string::npos ? mod : mod.substr(dot + 1),
                                                        .qname = mod,
                                                        .kind = "module",
                                                        .members = std::move(members)}));

    // the modules this one needs: where the types it uses, its translations' symbols and its declarations' types are from
    std::set<std::string> deps;
    const auto depend = [&](const std::string& qualified) {
        if (const auto owner = owning_module(prog, qualified); owner && *owner != mod)
            deps.insert(*owner);
    };
    for (const std::string& q : used)
        depend(q);
    for (const auto& [q, md] : r.translated) {
        for (Node* x : ir::walk(md)) {
            const ir::SymP s = sym_of(*x);
            if (s && s->owner && *s->owner != "py")
                depend(*s->owner);
        }
    }
    for (Node* decl : types) {
        const auto* cls = ir::get<ir::ClassDecl>(decl);
        std::set<std::string> names;
        add_types_of(cls->extends, names);   // (the base class: its name, unless it is the support library's)
        for (Node* member : cls->members)
            if (const auto* field = ir::get<ir::FieldDecl>(member))
                add_types_of(field->type, names);
        for (const std::string& q : names)
            depend(q);
    }
    ir::Unit unit;
    unit.path = m.path.empty() ? "<" + mod + ">" : m.path.generic_string();
    unit.package = mod;
    unit.imports.assign(deps.begin(), deps.end());
    unit.types = std::move(types);
    unit.lang = "python";
    r.unit = ir::make(ctx.arena(), std::move(unit));
    for (const auto& [key, decl] : global_decls)
        if (key.first == mod)
            r.global_decls.push_back(decl);
    return r;
}

}  // namespace

std::set<std::string> types_of(const ir::TypeP& type) {
    std::set<std::string> out;
    add_types_of(type, out);
    return out;
}

std::set<std::string> types_in(Node* method) {
    std::set<std::string> out;
    for (Node* x : ir::walk(method)) {
        add_types_of(x->type, out);
        add_types_of(own_type_of(*x), out);
    }
    if (const auto* md = ir::get<ir::MethodDecl>(method))
        add_types_of(md->ret, out);
    return out;
}

Translation translate(Program& prog, const Options& options) {
    const infer::Library& library = options.library ? *options.library : methods::library();
    Translation out;
    out.context_ = std::make_unique<lower::Context>(prog, library, options.int_policy);
    lower::Context& ctx = *out.context_;

    std::vector<Fn*> todo;
    for (const auto& [q, f] : prog.fns)
        if (!options.functions || options.functions->contains(q))
            todo.push_back(f.get());
    LoweredMap lowered = lower::lower_functions(ctx, todo);

    // the module names the translations read: each lowered as its module's top level, or refused with what reads it
    std::set<GlobalKey> needed;
    for (const auto& [q, l] : lowered)
        if (l.lowering && !l.is_refused())
            needed.insert(l.lowering->globals_used().begin(), l.lowering->globals_used().end());

    std::map<GlobalKey, lower::Refusal> bad_globals;
    GlobalDecls global_decls;
    std::map<std::string, std::unique_ptr<infer::ModuleEval>> module_evals;   // kept: a module name's lowering reads its evaluation
    for (const GlobalKey& key : needed) {
        const auto& [mod, name] = key;
        Module& mm = **prog.modules.find(mod);
        const auto* values = mm.assigns.find(name);
        if (!values || values->size() != 1 || values->front() == nullptr) {
            bad_globals[key] = lower::Refusal{.cls = lower::RefusalClass::Dynamic,
                                              .detail = std::format("module name {} assigned {} times", name, values ? values->size() : 0)};
            continue;
        }
        auto& evaluation = module_evals[mod];
        if (!evaluation) {
            prog.evaluate([&] {
                evaluation = std::make_unique<infer::ModuleEval>(prog, &mm, true, library);
                evaluation->run();
                return 0;
            });
        }
        const syntax::Expr* value = values->front();
        const std::uint32_t line = static_cast<std::uint32_t>(value->pos.lineno);
        try {
            lower::FnLower names(ctx, mm, *evaluation);
            const ir::TypeP t = ctx.type_of(prog.global_type(mod, name), line, "module name " + name);
            Node* init = names.value_of(value, t);
            if (names.has_pending())
                throw lower::Refused(lower::RefusalClass::Syntax, std::format("module name {} needs statements", name), line);
            const auto* cls = std::get_if<ir::ClassType>(&t->body);
            const bool plain = std::holds_alternative<ir::PrimType>(t->body) || (cls && (cls->name == "py.str" || cls->name == "py.bytes" || cls->name == "py.tuple"));
            global_decls[key] = ir::make(ctx.arena(),
                                         ir::FieldDecl{.sym = ir::make_sym(ir::SymKind::Global, name, mod, true, t),
                                                       .type = t,
                                                       .init = init,
                                                       .mods = plain ? std::vector<std::string>{"static", "const"} : std::vector<std::string>{"static"}},
                                         {}, line);
            for (const GlobalKey& g : names.globals_used())
                if (!needed.contains(g) && g != key)
                    bad_globals[key] = lower::Refusal{.cls = lower::RefusalClass::Syntax, .detail = std::format("module name {} refers to another", name)};
        } catch (const lower::Refused& e) {   // this name could not be lowered: whatever reads it is refused
            bad_globals[key] = e.refusal();
            global_decls.erase(key);
        }
    }
    for (auto& [q, l] : lowered) {
        if (!l.lowering || l.is_refused())
            continue;
        for (const GlobalKey& g : l.lowering->globals_used()) {   // (sorted: the first that is bad names the reason)
            if (const auto bad = bad_globals.find(g); bad != bad_globals.end()) {
                l.refused = lower::Refusal{.cls = lower::RefusalClass::CallsRefused,
                                           .detail = std::format("module name {} ({})", g.second, bad->second.reason().substr(0, 60))};
                l.decl = nullptr;
                break;
            }
        }
    }
    lower::close_refusals(lowered);
    const std::set<std::string> used = used_types(prog, ctx, lowered, global_decls);

    std::set<std::string> wanted;
    if (options.modules) {
        wanted = *options.modules;
    } else {
        for (const auto& [name, m] : prog.modules)
            wanted.insert(name);
    }
    std::vector<std::string> queue;   // (and the modules whose declarations they need)
    for (const std::string& m : wanted)
        if (prog.modules.contains(m))
            queue.push_back(m);
    for (std::size_t at = 0; at < queue.size(); ++at) {
        const std::string mod = queue[at];
        if (out.modules_.contains(mod))
            continue;
        lower::ModuleResult r = build_unit(prog, ctx, mod, lowered, global_decls, used);
        for (const std::string& dep : ir::get<ir::Unit>(r.unit)->imports)
            if (!out.modules_.contains(dep) && std::ranges::find(queue, dep) == queue.end())
                queue.push_back(dep);
        out.modules_.insert_or_assign(mod, std::move(r));
    }
    return out;
}

}  // namespace cb::translate::frontends::python::package
