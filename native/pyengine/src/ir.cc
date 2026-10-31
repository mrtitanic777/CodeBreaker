// ir.cc -- the L2 nodes and their JSON (see ir.hpp).
#include "ir.hpp"

#include <functional>

namespace pyengine {

static const char* PRIMS[] = {"bool", "i8", "i16", "i32", "i64", "u8", "u16", "u32", "u64", "f32", "f64", "f80", "char"};

bool ty_eq(const TyP& a, const TyP& b) {
    if (a == b) return true;
    if (!a || !b) return false;
    if (a->kind != b->kind || a->name != b->name || a->args.size() != b->args.size()) return false;
    if (a->has_ref != b->has_ref || a->ref != b->ref || a->record != b->record || a->exception != b->exception) return false;
    for (size_t i = 0; i < a->args.size(); i++)
        if (!ty_eq(a->args[i], b->args[i])) return false;
    return true;
}

std::string ty_repr(const TyP& t) {
    if (!t) return "None";
    if (t->kind == "class") {
        std::string s = t->name;
        if (!t->args.empty()) {
            s += "<";
            for (size_t i = 0; i < t->args.size(); i++) s += (i ? ", " : "") + ty_repr(t->args[i]);
            s += ">";
        }
        return s;
    }
    return t->kind;
}

TyP prim(const char* kind) {
    auto t = std::make_shared<IType>();
    t->kind = kind;
    return t;
}
TyP cls(const std::string& name, std::vector<TyP> args) {
    auto t = std::make_shared<IType>();
    t->kind = "class";
    t->name = name;
    t->args = std::move(args);
    return t;
}
TyP cls_ref(const std::string& name, bool ref) {
    auto t = std::make_shared<IType>();
    t->kind = "class";
    t->name = name;
    t->has_ref = true;
    t->ref = ref;
    return t;
}
TyP cls_flag(const std::string& name, const char* flag) {
    auto t = std::make_shared<IType>();
    t->kind = "class";
    t->name = name;
    std::string f = flag;
    if (f == "ref") {
        t->has_ref = true;
        t->ref = true;
    } else if (f == "exception") t->exception = true;
    else if (f == "record") t->record = true;
    return t;
}
TyP record_ty(const std::string& name, bool ref) {
    auto t = std::make_shared<IType>();
    t->kind = "class";
    t->name = name;
    t->has_ref = true;
    t->ref = ref;
    t->record = true;
    return t;
}

SymP sym(const std::string& kind, const std::string& name, std::optional<std::string> owner, bool stat, TyP type, std::vector<TyP> params,
         TyP ret) {
    auto s = std::make_shared<ISym>();
    s->kind = kind;
    s->name = name;
    s->owner = std::move(owner);
    s->stat = stat;
    s->type = std::move(type);
    s->params = std::move(params);
    s->ret = std::move(ret);
    return s;
}

IRP node(const char* k, int line) {
    auto n = std::make_shared<IR>();
    n->k = k;
    n->line = line;
    return n;
}
IRP mk_lit(py::V v, TyP t, std::optional<std::string> text) {
    auto n = node("Lit");
    n->has_value = !v.is_none();
    n->lit_value = std::move(v);
    n->type = std::move(t);
    n->text = std::move(text);
    return n;
}
IRP mk_lit_none(TyP t) { return mk_lit(py::V(), std::move(t)); }
IRP mk_pyfn(const std::string& name, std::vector<IRP> args, TyP t, IRP target, int line, std::vector<py::V> typeargs) {
    auto n = node("Call", line);
    n->target = std::move(target);
    n->sym = sym("function", name, std::string("py"));
    n->args = std::move(args);
    n->typeargs = std::move(typeargs);
    n->type = std::move(t);
    return n;
}
IRP mk_name(SymP s, TyP t) {
    auto n = node("Name");
    n->sym = std::move(s);
    n->type = std::move(t);
    return n;
}

std::vector<IRP> ir_children(const IRP& n) {
    std::vector<IRP> out;
    auto one = [&](const IRP& x) {
        if (x) out.push_back(x);
    };
    auto many = [&](const std::vector<IRP>& v) {
        for (auto& x : v) one(x);
    };
    const std::string& k = n->k;
    if (k == "Unit") many(n->members);   // (types)
    else if (k == "ClassDecl") many(n->members);
    else if (k == "FieldDecl") one(n->init);
    else if (k == "MethodDecl") {
        many(n->params);
        one(n->body);
    } else if (k == "Block") many(n->stmts);
    else if (k == "Local") one(n->init);
    else if (k == "ExprStmt") one(n->expr);
    else if (k == "If") {
        one(n->cond);
        one(n->then);
        one(n->orelse);
    } else if (k == "While") {
        one(n->cond);
        one(n->body);
    } else if (k == "For") {
        many(n->inits);
        one(n->cond);
        many(n->update);
        one(n->body);
    } else if (k == "ForEach") {
        one(n->var);
        one(n->iterable);
        one(n->body);
    } else if (k == "Return" || k == "Throw") one(n->expr);
    else if (k == "Try") {
        many(n->resources);
        one(n->body);
        many(n->catches);
        one(n->final_);
    } else if (k == "Catch") one(n->body);
    else if (k == "Select") one(n->obj);
    else if (k == "Call") {
        one(n->target);
        many(n->args);
    } else if (k == "New") many(n->args);
    else if (k == "Index") {
        one(n->arr);
        one(n->index);
    } else if (k == "Unary" || k == "Cast" || k == "Paren") one(n->x);
    else if (k == "Binary") {
        one(n->left);
        one(n->right);
    } else if (k == "Concat") many(n->parts);
    else if (k == "Assign") {
        one(n->target);
        one(n->value);
    } else if (k == "Cond") {
        one(n->cond);
        one(n->then);
        one(n->orelse);
    } else if (k == "Lambda") {
        many(n->params);
        one(n->body);
    } else if (k == "Labeled") {
        one(n->body);
    }
    return out;
}

void ir_walk(const IRP& n, const std::function<void(const IRP&)>& f) {
    std::vector<IRP> stack = {n};
    while (!stack.empty()) {
        IRP x = stack.back();
        stack.pop_back();
        f(x);
        auto kids = ir_children(x);
        for (auto it = kids.rbegin(); it != kids.rend(); ++it) stack.push_back(*it);
    }
}

// ------------------------------------------------------------------------------------ JSON
py::V type_json(const TyP& t) {
    if (!t) return py::V();
    py::V d = py::V::dict();
    for (const char* p : PRIMS)
        if (t->kind == p) {
            d.set("kind", t->kind);
            return d;
        }
    if (t->kind == "void" || t->kind == "null" || t->kind == "any" || t->kind == "dynamic") {
        d.set("kind", t->kind);
        return d;
    }
    d.set("kind", "class");
    d.set("name", t->name);
    if (!t->args.empty()) {
        py::V a = py::V::list();
        for (auto& x : t->args) a.append(type_json(x));
        d.set("args", a);
    }
    return d;
}

py::V sym_json(const SymP& s) {
    if (!s) return py::V();
    py::V d = py::V::dict();
    d.set("kind", s->kind);
    d.set("name", s->name);
    if (s->owner) d.set("owner", *s->owner);
    if (s->stat) d.set("static", true);
    if (s->type) d.set("type", type_json(s->type));
    if (!s->params.empty() || s->kind == "method" || s->kind == "ctor" || s->kind == "function") {
        py::V p = py::V::list();
        for (auto& x : s->params) p.append(type_json(x));
        d.set("params", p);
    }
    if (s->ret) d.set("ret", type_json(s->ret));
    return d;
}

static py::V nodes(const std::vector<IRP>& v) {
    py::V l = py::V::list();
    for (auto& x : v) l.append(to_json(x));
    return l;
}
static py::V strs(const std::vector<std::string>& v) {
    py::V l = py::V::list();
    for (auto& x : v) l.append(py::V(x));
    return l;
}

py::V to_json(const IRP& n) {
    py::V d = py::V::dict();
    d.set("node", n->k);
    if (n->line) d.set("line", n->line);
    const std::string& k = n->k;
    // slot helpers: a child slot keeps [], a non-child slot leaves None and [] out
    auto node_slot = [&](const char* name, const IRP& x) {
        if (x) d.set(name, to_json(x));
    };
    auto field_list = [&](const char* name, const std::vector<IRP>& v) { d.set(name, nodes(v)); };
    auto plain_list = [&](const char* name, const std::vector<std::string>& v) {
        if (!v.empty()) d.set(name, strs(v));
    };
    auto ty = [&](const char* name, const TyP& t) {
        if (t) d.set(name, type_json(t));
    };
    auto sy = [&](const SymP& s) {
        if (s) d.set("sym", sym_json(s));
    };
    auto str_opt = [&](const char* name, const std::optional<std::string>& s) {
        if (s) d.set(name, *s);
    };
    bool expr = k == "Lit" || k == "Name" || k == "Select" || k == "TypeName" || k == "This" || k == "Call" || k == "New" || k == "Index" ||
                k == "Unary" || k == "Binary" || k == "Concat" || k == "Assign" || k == "Cond" || k == "Cast" || k == "Lambda" || k == "Paren";
    if (expr) ty("type", n->type);
    if (k == "Unit") {
        str_opt("path", n->path);
        str_opt("package", n->package);
        plain_list("imports", n->imports);
        field_list("types", n->members);
    } else if (k == "ClassDecl") {
        d.set("name", n->name);
        d.set("qname", n->qname);
        d.set("kind", n->dkind);
        plain_list("mods", n->mods);
        ty("extends", n->extends);
        field_list("members", n->members);
    } else if (k == "FieldDecl") {
        sy(n->sym);
        ty("type", n->type);
        node_slot("init", n->init);
        plain_list("mods", n->mods);
    } else if (k == "MethodDecl") {
        sy(n->sym);
        d.set("name", n->name);
        d.set("kind", n->dkind);
        field_list("params", n->params);
        ty("ret", n->ret);
        node_slot("body", n->body);
        plain_list("mods", n->mods);
        plain_list("comments", n->comments);
    } else if (k == "Param") {
        sy(n->sym);
        ty("type", n->type);
        plain_list("mods", n->mods);
    } else if (k == "Block") {
        field_list("stmts", n->stmts);
    } else if (k == "Local") {
        sy(n->sym);
        ty("type", n->type);
        node_slot("init", n->init);
        plain_list("mods", n->mods);
    } else if (k == "ExprStmt") {
        node_slot("expr", n->expr);
    } else if (k == "If") {
        node_slot("cond", n->cond);
        node_slot("then", n->then);
        node_slot("orelse", n->orelse);
    } else if (k == "While") {
        node_slot("cond", n->cond);
        node_slot("body", n->body);
    } else if (k == "For") {
        field_list("init", n->inits);
        node_slot("cond", n->cond);
        field_list("update", n->update);
        node_slot("body", n->body);
    } else if (k == "ForEach") {
        node_slot("var", n->var);
        node_slot("iterable", n->iterable);
        node_slot("body", n->body);
    } else if (k == "Break" || k == "Continue") {
    } else if (k == "Return" || k == "Throw") {
        node_slot("expr", n->expr);
    } else if (k == "Try") {
        field_list("resources", n->resources);
        node_slot("body", n->body);
        field_list("catches", n->catches);
        node_slot("final", n->final_);
    } else if (k == "Catch") {
        sy(n->sym);
        if (!n->types.empty()) {
            py::V l = py::V::list();
            for (auto& t : n->types) l.append(type_json(t));
            d.set("types", l);
        }
        node_slot("body", n->body);
    } else if (k == "Comment") {
        str_opt("text", n->text);
    } else if (k == "Lit") {
        if (n->has_value) d.set("value", n->lit_value);
        str_opt("text", n->text);
    } else if (k == "Name") {
        sy(n->sym);
    } else if (k == "Select") {
        node_slot("obj", n->obj);
        sy(n->sym);
    } else if (k == "TypeName" || k == "This") {
    } else if (k == "Call") {
        node_slot("target", n->target);
        sy(n->sym);
        field_list("args", n->args);
        if (!n->typeargs.empty()) {
            py::V l = py::V::list();
            for (auto& x : n->typeargs) l.append(x);
            d.set("typeargs", l);
        }
    } else if (k == "New") {
        sy(n->sym);
        field_list("args", n->args);
        str_opt("memory", n->memory);
    } else if (k == "Index") {
        node_slot("arr", n->arr);
        node_slot("index", n->index);
    } else if (k == "Unary") {
        str_opt("op", n->op);
        node_slot("x", n->x);
        d.set("postfix", n->postfix);
    } else if (k == "Binary") {
        str_opt("op", n->op);
        node_slot("left", n->left);
        node_slot("right", n->right);
    } else if (k == "Concat") {
        field_list("parts", n->parts);
    } else if (k == "Assign") {
        str_opt("op", n->op);
        node_slot("target", n->target);
        node_slot("value", n->value);
    } else if (k == "Cond") {
        node_slot("cond", n->cond);
        node_slot("then", n->then);
        node_slot("orelse", n->orelse);
    } else if (k == "Cast") {
        node_slot("x", n->x);
    } else if (k == "Lambda") {
        field_list("params", n->params);
        node_slot("body", n->body);
    } else if (k == "Paren") {
        node_slot("x", n->x);
    } else if (k == "Labeled") {
        str_opt("label", n->text);
        node_slot("body", n->body);
    } else if (k == "Goto") {
        str_opt("label", n->text);
    }
    for (auto& [key, v] : n->extra) d.set(key.c_str(), v);
    return d;
}

}  // namespace pyengine
