// translate/frontends/clang/lower_decl.cpp -- the declarations of a unit: records and their members, enums, globals,
// functions and methods (defined in or out of their class), constructor initialisers.
#include <algorithm>
#include <format>

#include "core/text.hpp"
#include "translate/frontends/clang/lower_impl.hpp"

namespace cb::translate::frontends::clang {

namespace {

bool is_method_kind(std::string_view k) {
    return k == "CXXMethodDecl" || k == "CXXConstructorDecl" || k == "CXXDestructorDecl" || k == "CXXConversionDecl";
}

bool is_record_kind(std::string_view k) { return k == "CXXRecordDecl" || k == "ClassTemplateSpecializationDecl"; }

std::string lowered_name(const std::string& key) {
    const std::size_t dot = key.rfind('.');
    const std::string last = dot == std::string::npos ? key : key.substr(dot + 1);
    return last.substr(0, last.find('<'));
}

}  // namespace

ir::Node* Lowering::unit() {
    out_.clear();
    classes_.clear();
    decls(tree::inner(tree_), true);
    ir::Unit u;
    u.path = main_;
    u.package = "";
    u.types = out_;
    u.target = model_.target(language_, std_.value_or(""));
    return make_stmt(std::move(u), Pos{});
}

void Lowering::decls(const std::vector<Json>& nodes, bool top) {
    (void)top;
    for (const Json& n : nodes) {
        const std::string k = tree::kind(n);
        if (k == "NamespaceDecl" || k == "LinkageSpecDecl" || k == "ExternCContextDecl") {
            decls(tree::inner(n), false);
            continue;
        }
        if (!is_own(loc(n).file) || tree::flag(n, "isImplicit")) continue;
        try {
            decl(n);
        } catch (const ParseError& e) {   // one declaration the dump does not describe as clang writes it: that one is refused
            out_.push_back(unsupported_decl("decl:" + k, n, e.what()));
        } catch (const FormatError& e) {
            out_.push_back(unsupported_decl("decl:" + k, n, e.what()));
        }
    }
}

ir::Node* Lowering::unsupported_decl(const std::string& kind, const Json& n, const std::optional<std::string>& raw) {
    const Pos p = pos(n);
    ir::Block block;
    block.stmts.push_back(make_stmt(ir::ExprStmt{.expr = unsupported_node(kind, &n, raw)}, Pos{}));
    ir::MethodDecl m;
    m.name = tree::maybe_text(n, "name");
    m.kind = "function";
    m.ret = ir::void_type();
    m.body = make_stmt(std::move(block), Pos{});
    m.mods = {"unsupported"};
    return make_stmt(std::move(m), p);
}

void Lowering::decl(const Json& n) {
    const std::string k = tree::kind(n);
    if (k == "RecordDecl" || k == "CXXRecordDecl") {
        if (tree::flag(n, "completeDefinition")) record(n);
    } else if (k == "ClassTemplateDecl") {
        for (const Json& c : tree::inner(n)) {
            if (tree::kind(c) == "ClassTemplateSpecializationDecl" && tree::flag(c, "completeDefinition")) {
                const auto key = qnames_.find(tree::text(c, "id"));
                const Json* full = &c;
                if (key != qnames_.end()) {
                    if (const Json* const* hit = records_.find(key->second)) full = *hit;
                }
                record(*full);
            }
        }
    } else if (k == "ClassTemplateSpecializationDecl") {
        if (tree::flag(n, "completeDefinition")) record(n);
    } else if (k == "FunctionTemplateDecl") {
        for (const Json& c : tree::inner(n)) {
            const std::string ck = tree::kind(c);
            if ((ck == "FunctionDecl" || ck == "CXXMethodDecl") && tree::flag(c, "TemplateInstantiationPattern")) decl(c);
        }
    } else if (k == "FunctionDecl") {
        if (body(n) != nullptr) out_.push_back(function(n));
    } else if (is_method_kind(k)) {
        if (body(n) == nullptr) return;   // a member defined outside its class is lowered where its body is
        const Json* owner = nullptr;
        if (const std::string parent_id = tree::text(n, "parentDeclContextId"); !parent_id.empty()) {
            if (const auto it = decls_.find(parent_id); it != decls_.end()) owner = it->second;
        }
        if (owner == nullptr) {
            if (const auto it = parent_.find(tree::require(n, "id").get<std::string>()); it != parent_.end()) owner = it->second;
        }
        ir::Node* m = method(n, owner);
        ir::Node* cls_node = nullptr;
        if (owner != nullptr) {
            if (const auto key = qnames_.find(tree::text(*owner, "id")); key != qnames_.end()) {
                if (const auto c = classes_.find(key->second); c != classes_.end()) cls_node = c->second;
            }
        }
        ir::ClassDecl* cls = ir::get<ir::ClassDecl>(cls_node);
        ir::Node* prev = cls != nullptr ? find_method(*cls, n) : nullptr;
        if (prev != nullptr) {
            // (static, virtual, override are said in the class, not at the definition outside it)
            ir::MethodDecl* made = ir::get<ir::MethodDecl>(m);
            const ir::MethodDecl* before = ir::get<ir::MethodDecl>(prev);
            std::vector<std::string> mods = before->mods;
            for (const std::string& x : made->mods) {
                if (std::ranges::find(before->mods, x) == before->mods.end()) mods.push_back(x);
            }
            made->mods = std::move(mods);
            if (std::ranges::find(made->mods, "static") != made->mods.end() && made->sym) made->sym->is_static = true;
            std::ranges::replace(cls->members, prev, m);
        } else if (cls != nullptr) {
            cls->members.push_back(m);
        } else {
            out_.push_back(m);
        }
    } else if (k == "VarDecl") {
        if (tree::text(n, "storageClass") == "extern" && tree::inner(n).empty()) return;
        if (const Json* owner = member_owner(n)) {   // `int Shape::count = 0;`: the class's static member
            ir::ClassDecl* cls = nullptr;
            if (const auto key = qnames_.find(tree::text(*owner, "id")); key != qnames_.end()) {
                if (const auto c = classes_.find(key->second); c != classes_.end()) cls = ir::get<ir::ClassDecl>(c->second);
            }
            const Json* init = nullptr;
            for (const Json& c : tree::inner(n)) {
                if (!tree::ends_with_attr(tree::kind(c))) init = &c;
            }
            if (cls != nullptr && init != nullptr) {
                for (ir::Node* m : cls->members) {
                    auto* field = ir::get<ir::FieldDecl>(m);
                    if (field != nullptr && field->sym && field->sym->name == tree::require(n, "name").get<std::string>()) field->init = expr(*init);
                }
            }
            return;
        }
        out_.push_back(global_var(n));
    } else if (k == "EnumDecl") {
        enum_decl(n);
    } else if (k == "TypedefDecl" || k == "TypeAliasDecl" || k == "EmptyDecl" || k == "StaticAssertDecl" || k == "UsingDirectiveDecl" ||
               k == "UsingDecl" || k == "TypeAliasTemplateDecl" || k == "VarTemplateDecl" || k == "UsingShadowDecl") {
        // nothing to lower
    } else {
        out_.push_back(unsupported_decl("decl:" + k, n, std::nullopt));
    }
}

// A library symbol's owner: its C++ namespace ('std', 'System') where it has one, else the header that declares it
// ('stdio.h').
std::optional<std::string> Lowering::lib_owner(const Json& node) {
    std::vector<std::string> ns;
    const auto up = [this](const Json& d) -> const Json* {
        const auto it = parent_.find(tree::text(d, "id"));
        return it == parent_.end() ? nullptr : it->second;
    };
    for (const Json* cur = up(node); cur != nullptr; cur = up(*cur)) {
        if (tree::kind(*cur) == "NamespaceDecl") {
            const std::string name_text = tree::text(*cur, "name");
            ns.push_back(name_text.empty() ? "(anonymous)" : name_text);
        }
    }
    if (!ns.empty()) {
        std::ranges::reverse(ns);
        return text::join(ns, ".");
    }
    const Location& l = loc(node);
    if (l.file < 0) return std::nullopt;
    const std::string& f = files_[static_cast<std::size_t>(l.file)];
    const std::size_t sep = f.find_last_of("/\\");
    const std::string base = sep == std::string::npos ? f : f.substr(sep + 1);
    return base.empty() ? std::nullopt : std::optional<std::string>(base);
}

// The record a VarDecl is a static data member of (in its class or defined outside it), or nullptr.
const Json* Lowering::member_owner(const Json& n) {
    if (const auto it = parent_.find(tree::text(n, "id")); it != parent_.end() && is_record_kind(tree::kind(*it->second))) return it->second;
    if (const auto it = decls_.find(tree::text(n, "parentDeclContextId")); it != decls_.end() && is_record_kind(tree::kind(*it->second))) {
        return it->second;
    }
    return nullptr;
}

ir::Node* Lowering::find_method(ir::ClassDecl& cls, const Json& n) {
    const std::optional<std::string> name_text = tree::maybe_text(n, "name");
    const std::optional<std::string> qual = tree::maybe_text(tree::object(n, "type"), "qualType");
    for (ir::Node* m : cls.members) {
        const auto* md = ir::get<ir::MethodDecl>(m);
        if (md == nullptr) continue;
        const auto sig = sigs_.find(md);
        if (sig != sigs_.end() && sig->second.first == name_text && qual && sig->second.second == *qual) return m;
    }
    return nullptr;
}

const Json* Lowering::body(const Json& n) {
    for (const Json& c : tree::inner(n)) {
        const std::string k = tree::kind(c);
        if (k == "CompoundStmt" || k == "CXXTryStmt") return &c;
    }
    return nullptr;
}

// ---- records ---------------------------------------------------------------------------------------------------------

ir::Node* Lowering::record(const Json& n) {
    const auto key_it = qnames_.find(tree::require(n, "id").get<std::string>());
    if (key_it == qnames_.end()) return nullptr;
    const std::string key = key_it->second;
    if (const auto done = classes_.find(key); done != classes_.end()) return done->second;
    const ir::TypeP t = record_type(&n);
    const std::string tag = tree::maybe_text(n, "tagUsed").value_or("struct");
    const std::string kind = tag == "union" ? "union" : (tag == "class" ? "class" : "struct");
    std::vector<ir::TypeP> base_types;
    for (const Json& b : bases(n)) base_types.push_back(types_.parse(tree::text(tree::require(b, "type"), "qualType")));
    ir::ClassDecl cd;
    if (polymorphic(n)) cd.mods.push_back("polymorphic");
    if (tree::flag(tree::object(n, "definitionData"), "isAbstract")) cd.mods.push_back("abstract");
    cd.name = lowered_name(key);
    cd.qname = spell(t);
    cd.kind = kind;
    if (!base_types.empty()) cd.extends = base_types[0];
    if (base_types.size() > 1) cd.implements.assign(base_types.begin() + 1, base_types.end());
    cd.layout = record_layout(t);
    ir::Node* node = make_stmt(std::move(cd), pos(n));
    ir::ClassDecl* c = ir::get<ir::ClassDecl>(node);
    classes_[key] = node;
    out_.push_back(node);
    std::string access = tag == "class" ? "private" : "public";
    for (const Json& m : tree::inner(n)) {
        const std::string mk = tree::kind(m);
        if (mk == "AccessSpecDecl") {
            access = tree::maybe_text(m, "access").value_or(access);
            continue;
        }
        if (tree::flag(m, "isImplicit")) continue;
        if (mk == "FieldDecl") {
            const ir::TypeP ft = ty(m);
            std::vector<std::string> fmods = {access};
            if (tree::flag(m, "isBitfield")) fmods.push_back("bitfield");
            ir::Node* init = nullptr;
            for (const Json& x : tree::inner(m)) {
                const std::string xk = tree::kind(x);
                if (xk != "ConstantExpr" || !tree::flag(m, "isBitfield")) {
                    if (!tree::ends_with_attr(xk)) init = expr(x);
                }
            }
            c->members.push_back(make_stmt(ir::FieldDecl{.sym = sym(ir::SymKind::Field, tree::text(m, "name"), c->qname, false, ft), .type = ft, .init = init, .mods = fmods}, pos(m)));
        } else if (mk == "VarDecl") {   // a static data member
            const ir::TypeP ft = ty(m);
            ir::Node* init = nullptr;
            if (!tree::inner(m).empty() && !tree::ends_with_attr(tree::kind(tree::inner(m)[0]))) init = expr(tree::inner(m)[0]);
            std::vector<std::string> mods = {access, "static"};
            if (tree::text(tree::require(m, "type"), "qualType").find("const") != std::string::npos) mods.push_back("const");
            c->members.push_back(make_stmt(ir::FieldDecl{.sym = sym(ir::SymKind::Field, tree::require(m, "name").get<std::string>(), c->qname, true, ft), .type = ft, .init = init, .mods = mods}, pos(m)));
        } else if (is_method_kind(mk)) {
            if (tree::flag(m, "explicitlyDefaulted") || tree::flag(m, "explicitlyDeleted")) continue;
            c->members.push_back(method(m, &n, access));
        } else if ((mk == "CXXRecordDecl" || mk == "RecordDecl") && tree::flag(m, "completeDefinition")) {
            if (ir::Node* inner_node = record(m)) {
                if (auto* inner_class = ir::get<ir::ClassDecl>(inner_node)) inner_class->outer = c->qname;
            }
        } else if (mk == "EnumDecl") {
            enum_decl(m);
        } else if (mk == "FunctionTemplateDecl") {
            for (const Json& x : tree::inner(m)) {
                if (tree::kind(x) == "CXXMethodDecl" && tree::flag(x, "TemplateInstantiationPattern")) c->members.push_back(method(x, &n, access));
            }
        } else if (mk == "FriendDecl" || tree::ends_with_attr(mk) || mk == "TemplateArgument" || mk == "TypedefDecl" || mk == "TypeAliasDecl" ||
                   mk == "StaticAssertDecl" || mk == "UsingDecl") {
            continue;
        } else {
            c->members.push_back(make_stmt(ir::FieldDecl{.sym = sym(ir::SymKind::Field, "<" + mk + ">", c->qname), .type = ir::any_type(),
                                                         .init = unsupported_node("member:" + mk, &m), .mods = {"unsupported"}},
                                           Pos{}));
        }
    }
    return node;
}

void Lowering::enum_decl(const Json& n) {
    std::vector<const Json*> consts;
    for (const Json& c : tree::inner(n)) {
        if (tree::kind(c) == "EnumConstantDecl") consts.push_back(&c);
    }
    const std::string id = tree::require(n, "id").get<std::string>();
    const auto enum_value = [this](const Json& e) { return enum_values_.at(tree::require(e, "id").get<std::string>()); };
    const auto literal = [&](std::int64_t value) { return make(ir::Lit{.value = value}, ir::i32_type(), Pos{}); };
    if (const std::string* qn = scoped_enums_.find(id)) {
        ir::ClassDecl cd;
        cd.name = tree::require(n, "name").get<std::string>();
        cd.qname = *qn;
        cd.kind = "enum";
        cd.mods = {"scoped"};
        ir::Node* node = make_stmt(std::move(cd), pos(n));
        ir::ClassDecl* c = ir::get<ir::ClassDecl>(node);
        for (const Json* e : consts) {
            c->members.push_back(make_stmt(ir::FieldDecl{.sym = sym(ir::SymKind::Field, tree::require(*e, "name").get<std::string>(), *qn, true, ir::i32_type()),
                                                         .type = ir::i32_type(), .init = literal(enum_value(*e)), .mods = {"public", "static", "const"}},
                                           pos(*e)));
        }
        out_.push_back(node);
        return;
    }
    for (const Json* e : consts) {   // a C enum's constants are ints in the global scope
        out_.push_back(make_stmt(ir::FieldDecl{.sym = sym(ir::SymKind::Global, tree::require(*e, "name").get<std::string>(), unit_name_, true, ir::i32_type()),
                                               .type = ir::i32_type(), .init = literal(enum_value(*e)),
                                               .mods = {"static", "const", "enum:" + tree::text(n, "name")}},
                                 pos(*e)));
    }
}

ir::Node* Lowering::global_var(const Json& n) {
    ir::TypeP t = ty(n);
    ir::Node* init = nullptr;
    for (const Json& c : tree::inner(n)) {
        if (!tree::ends_with_attr(tree::kind(c))) init = expr(c);
    }
    t = complete_array(t, init);
    std::vector<std::string> mods = {"static"};
    if (tree::text(n, "storageClass") == "static") mods.push_back("internal");
    if (tree::text(tree::object(n, "type"), "qualType").starts_with("const ")) mods.push_back("const");
    return make_stmt(ir::FieldDecl{.sym = sym(ir::SymKind::Global, tree::require(n, "name").get<std::string>(), unit_name_, true, t), .type = t, .init = init, .mods = mods}, pos(n));
}

// `int a[] = {1, 2, 3}`: the array has the count its initialiser gave it.
ir::TypeP Lowering::complete_array(const ir::TypeP& t, const ir::Node* init) {
    const auto* array = t ? std::get_if<ir::ArrayType>(&t->body) : nullptr;
    if (array == nullptr || array->count || init == nullptr || !init->type) return t;
    const auto* from_init = std::get_if<ir::ArrayType>(&init->type->body);
    if (from_init != nullptr && from_init->count) return init->type;
    return t;
}

// ---- functions -------------------------------------------------------------------------------------------------------

// A FunctionDecl / CXXMethodDecl node (or a referencedDecl stub) -> its symbol.
ir::SymP Lowering::sym_of_function(const Json& d, const std::optional<std::string>& owner) {
    const Json* node = &d;
    if (const auto it = decls_.find(tree::text(d, "id")); it != decls_.end()) node = it->second;
    const ir::TypeP ft = ty(*node);
    std::vector<ir::TypeP> param_types;
    ir::TypeP result = ir::void_type();
    bool variadic = false;
    if (ft) {
        if (const auto* fn = std::get_if<ir::FnType>(&ft->body)) {
            for (const ir::FnParam& p : fn->params) param_types.push_back(p.type);
            if (fn->result.type) result = fn->result.type;
            variadic = fn->variadic;
        }
    }
    const std::string k = tree::maybe_text(*node, "kind").value_or(tree::kind(d));
    const std::string name_text = tree::maybe_text(*node, "name").value_or(tree::text(d, "name"));
    ir::SymP s;
    if (is_method_kind(k)) {
        const Json* par = nullptr;
        if (const auto it = decls_.find(tree::text(*node, "parentDeclContextId")); it != decls_.end()) par = it->second;
        if (par == nullptr) {
            if (const auto it = parent_.find(tree::text(*node, "id")); it != parent_.end()) par = it->second;
        }
        const ir::TypeP record = par != nullptr ? record_type(par) : ir::TypeP();
        const std::optional<std::string> qn = record ? std::optional<std::string>(spell(record)) : owner;
        s = sym(k == "CXXConstructorDecl" ? ir::SymKind::Ctor : ir::SymKind::Method, name_text, qn, tree::text(*node, "storageClass") == "static");
    } else {
        const std::optional<std::string> own = is_own(loc(*node).file) ? std::optional<std::string>(unit_name_) : lib_owner(*node);
        s = sym(ir::SymKind::Function, name_text, own, true);
    }
    s->params = std::move(param_types);
    s->ret = result;
    s->varargs = variadic;
    return s;
}

std::vector<ir::Node*> Lowering::params(const Json& n, Scope& scope) {
    std::vector<ir::Node*> out;
    for (const Json& p : tree::inner(n)) {
        if (tree::kind(p) != "ParmVarDecl") continue;
        const ir::TypeP t = ty(p);
        const std::string name_text = tree::flag(p, "name") ? local_name(p, scope) : scope.fresh("arg");
        ir::SymP s = sym(ir::SymKind::Param, name_text, scope.owner, false, t);
        scope.syms[tree::require(p, "id").get<std::string>()] = s;
        out.push_back(make_stmt(ir::Param{.sym = s, .type = t}, pos(p)));
    }
    return out;
}

std::string Lowering::local_name(const Json& d, Scope& scope) {
    std::string name_text = tree::text(d, "name");
    if (name_text.empty()) name_text = "tmp";
    if (scope.used.contains(name_text)) {
        int i = 2;
        while (scope.used.contains(std::format("{}_{}", name_text, i))) ++i;
        name_text = std::format("{}_{}", name_text, i);
    }
    scope.used.insert(name_text);
    return name_text;
}

ir::Node* Lowering::function(const Json& n) {
    auto scope = std::make_shared<Scope>(tree::maybe_text(n, "name"));
    ir::SymP s = sym_of_function(n);
    std::vector<ir::Node*> ps = params(n, *scope);
    scope_ = scope;
    ir::MethodDecl m;
    m.body = as_stmt(stmt(*body(n)));
    if (tree::text(n, "storageClass") == "static") m.mods.push_back("internal");
    if (tree::flag(n, "inline")) m.mods.push_back("inline");
    m.sym = s;
    m.name = tree::require(n, "name").get<std::string>();
    m.kind = "function";
    m.params = std::move(ps);
    m.ret = s->ret;
    return make_stmt(std::move(m), pos(n));
}

ir::Node* Lowering::method(const Json& n, const Json* owner, const std::string& access) {
    auto scope = std::make_shared<Scope>(tree::maybe_text(n, "name"));
    ir::SymP s = sym_of_function(n);
    std::vector<ir::Node*> ps = params(n, *scope);
    scope_ = scope;
    const std::string k = tree::require(n, "kind").get<std::string>();
    const std::string kind = k == "CXXConstructorDecl" ? "ctor" : (k == "CXXDestructorDecl" ? "dtor" : "method");
    std::vector<std::string> mods = {access};
    for (const char* flag : {"virtual", "pure", "inline"}) {
        if (tree::flag(n, flag)) mods.push_back(flag);
    }
    if (tree::text(n, "storageClass") == "static") mods.push_back("static");
    const std::string qual = tree::text(tree::object(n, "type"), "qualType");
    if (qual.ends_with(" const") || qual.find(") const") != std::string::npos) mods.push_back("const");
    for (const Json& c : tree::inner(n)) {
        if (tree::kind(c) == "OverrideAttr") {
            mods.push_back("override");
            break;
        }
    }
    ir::Node* body_node = nullptr;
    if (const Json* b = body(n)) {
        std::vector<ir::Node*> pre;
        for (const Json& c : tree::inner(n)) {
            if (tree::kind(c) == "CXXCtorInitializer") {
                std::vector<ir::Node*> init = ctor_init(c, owner);
                pre.insert(pre.end(), init.begin(), init.end());
            }
        }
        body_node = as_stmt(stmt(*b));
        if (!pre.empty()) {
            auto* block = ir::get<ir::Block>(body_node);
            if (block == nullptr) {   // (a function-try-block: the initialisers go in a block before it)
                ir::Block wrapped;
                wrapped.stmts = {body_node};
                body_node = make_stmt(std::move(wrapped), pos(*b));
                block = ir::get<ir::Block>(body_node);
            }
            pre.insert(pre.end(), block->stmts.begin(), block->stmts.end());
            block->stmts = std::move(pre);
        }
    }
    ir::MethodDecl m;
    m.sym = s;
    m.name = tree::maybe_text(n, "name");
    m.kind = kind;
    m.params = std::move(ps);
    m.ret = kind == "method" ? s->ret : ir::void_type();
    m.body = body_node;
    m.mods = std::move(mods);
    ir::Node* node = make_stmt(std::move(m), pos(n));
    sigs_[ir::get<ir::MethodDecl>(node)] = {tree::maybe_text(n, "name"), qual};
    return node;
}

std::vector<ir::Node*> Lowering::ctor_init(const Json& c, const Json* owner) {
    const Pos p = pos(c);
    const Json* val = tree::inner(c).empty() ? nullptr : &tree::inner(c)[0];
    if (tree::get(c, "baseInit") != nullptr) {
        const ir::TypeP bt = types_.parse(tree::text(tree::require(c, "baseInit"), "qualType"));
        ir::Node* e = val != nullptr ? expr(*val) : nullptr;
        const auto* made = ir::get<ir::New>(e);
        ir::CtorCall call_node;
        call_node.kind = "super";
        if (made != nullptr) {
            call_node.args = made->args;
            call_node.sym = made->sym;
        } else {
            if (e != nullptr) call_node.args = {e};
            const auto* cls = std::get_if<ir::ClassType>(&bt->body);
            call_node.sym = sym(ir::SymKind::Ctor, cls != nullptr ? cls->name : bt->simple(), spell(bt));
        }
        return {make_stmt(std::move(call_node), p)};
    }
    if (tree::get(c, "delegatingInit") != nullptr || tree::get(c, "delegateInit") != nullptr) {
        if (val == nullptr) tree::bad("a delegating initialiser has no call");
        ir::Node* e = expr(*val);
        const auto* made = ir::get<ir::New>(e);
        ir::CtorCall call_node;
        call_node.kind = "this";
        if (made != nullptr) {
            call_node.sym = made->sym;
            call_node.args = made->args;
        }
        return {make_stmt(std::move(call_node), p)};
    }
    const Json& field = tree::object(c, "anyInit");
    if (field.empty() || val == nullptr) return {};
    if (owner == nullptr) tree::bad("a constructor initialiser of no class");
    const ir::TypeP ot = record_type(owner);
    const ir::TypeP ft = types_.parse(tree::text(tree::require(field, "type"), "qualType"));
    ir::Node* self = make(ir::This{}, ir::pointer_to(ot), p);
    ir::Node* deref = make(ir::Unary{.op = "*", .x = self}, ot, p);
    ir::Node* target = make(ir::Select{.obj = deref, .sym = sym(ir::SymKind::Field, tree::require(field, "name").get<std::string>(), spell(ot), false, ft)}, ft, p);
    ir::Node* v = nullptr;
    const std::string vk = tree::kind(*val);
    const bool is_class = ft && std::holds_alternative<ir::ClassType>(ft->body);
    if (vk == "ImplicitValueInitExpr" || (vk == "CXXConstructExpr" && tree::inner(*val).empty() && !is_class)) {
        v = zero(ft, p);
    } else {
        v = expr(*val);
    }
    const auto* ptr = ft ? std::get_if<ir::PtrType>(&ft->body) : nullptr;
    if (ptr != nullptr && ptr->ref) v = make(ir::Unary{.op = "&", .x = v}, ft, p);
    ir::Node* assign = make(ir::Assign{.target = target, .value = v}, ft, p);
    return {make_stmt(ir::ExprStmt{.expr = assign}, p)};
}

}  // namespace cb::translate::frontends::clang
