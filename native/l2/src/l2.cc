#include "l2.hpp"

#include <cmath>
#include <map>

#include "pystr.hpp"

namespace l2 {

using py::V;

// ---------------------------------------------------------------------------------------------- the node classes (ir.py)
static std::vector<NodeClass> make_classes() {
    std::vector<NodeClass> c;
    auto decl = [&](const char* n, std::vector<std::string> slots, std::vector<std::string> fields) {
        c.push_back({n, slots, fields, false, false});
    };
    auto stmt = [&](const char* n, std::vector<std::string> slots, std::vector<std::string> fields) {
        c.push_back({n, slots, fields, false, true});
    };
    auto stmt1 = [&](const char* n, std::vector<std::string> slots) { stmt(n, slots, slots); };
    auto expr = [&](const char* n, std::vector<std::string> slots, std::vector<std::string> fields) {
        std::vector<std::string> all{"type"};
        all.insert(all.end(), slots.begin(), slots.end());
        c.push_back({n, all, fields, true, false});
    };
    auto expr1 = [&](const char* n, std::vector<std::string> slots) { expr(n, slots, slots); };
    // -- declarations
    decl("Module", {"units", "header"}, {"units"});
    decl("Unit", {"path", "package", "imports", "types", "lang", "target"}, {"types"});
    decl("ClassDecl", {"name", "qname", "kind", "mods", "typeparams", "extends", "implements", "members", "outer",
                       "tparams", "layout"}, {"members"});
    decl("FieldDecl", {"sym", "type", "init", "mods"}, {"init"});
    decl("MethodDecl", {"sym", "name", "kind", "params", "ret", "body", "mods", "throws", "typeparams", "comments",
                        "text", "tparams"}, {"params", "body"});
    decl("Param", {"sym", "type", "mods"}, {});
    decl("EnumConst", {"sym", "args", "body", "ctor", "value"}, {"args", "body"});
    decl("Property", {"sym", "type", "mods", "params", "accessors", "init"}, {"params", "accessors", "init"});
    decl("TypeParam", {"name", "bounds", "flags", "variance"}, {});
    // -- statements
    stmt1("Block", {"stmts"});
    stmt("Local", {"sym", "type", "init", "mods"}, {"init"});
    stmt1("ExprStmt", {"expr"});
    stmt1("If", {"cond", "then", "orelse"});
    stmt1("While", {"cond", "body"});
    stmt1("DoWhile", {"body", "cond"});
    stmt1("For", {"init", "cond", "update", "body"});
    stmt1("ForEach", {"var", "iterable", "body"});
    stmt1("Switch", {"expr", "cases"});
    stmt("Case", {"labels", "default", "body"}, {"labels", "body"});
    stmt("Break", {"label"}, {});
    stmt("Continue", {"label"}, {});
    stmt1("Return", {"expr"});
    stmt1("Throw", {"expr"});
    stmt1("Try", {"resources", "body", "catches", "final"});
    stmt("Catch", {"sym", "types", "body"}, {"body"});
    stmt("Labeled", {"label", "body"}, {"body"});
    stmt("Goto", {"label"}, {});
    stmt("MachineFragment", {"length", "reason", "why", "asm", "name", "syntax", "within"}, {});
    stmt1("Sync", {"lock", "body"});
    stmt("Empty", {}, {});
    stmt("Comment", {"text"}, {});
    stmt1("LocalClass", {"decl"});
    stmt1("Assert", {"cond", "detail"});
    stmt("CtorCall", {"kind", "sym", "args", "target"}, {"args", "target"});
    stmt1("Yield", {"expr"});
    // -- expressions
    expr("Lit", {"value", "text"}, {});
    expr("Name", {"sym"}, {});
    expr("Select", {"obj", "sym"}, {"obj"});
    expr("TypeName", {}, {});
    expr("This", {"qualifier"}, {});
    expr("Super", {"qualifier"}, {});
    expr("Call", {"target", "sym", "args", "typeargs"}, {"target", "args"});
    expr("New", {"sym", "args", "body", "outer", "memory"}, {"outer", "args", "body"});
    expr("NewArray", {"elem", "dims", "init"}, {"dims", "init"});
    expr("Index", {"arr", "index", "sym"}, {"arr", "index"});
    expr1("Length", {"arr"});
    expr("Unary", {"op", "x", "postfix", "overflow"}, {"x"});
    expr("Binary", {"op", "left", "right", "overflow", "signed"}, {"left", "right"});
    expr1("Concat", {"parts"});
    expr("Assign", {"op", "target", "value", "overflow"}, {"target", "value"});
    expr1("Cond", {"cond", "then", "orelse"});
    expr("Cast", {"x", "overflow", "implicit"}, {"x"});
    expr("InstanceOf", {"x", "test", "binding"}, {"x"});
    expr("ClassLit", {"of"}, {});
    expr1("Lambda", {"params", "body"});
    expr("MethodRef", {"target", "sym", "mode"}, {"target"});
    expr1("SwitchExpr", {"expr", "cases"});
    expr("AddressOf", {"sym"}, {});
    expr1("FarAddress", {"selector", "offset"});
    expr("Piece", {"x", "byte", "width"}, {"x"});
    expr1("Join", {"hi", "lo"});
    expr1("Paren", {"x"});
    expr("Unsupported", {"kind", "raw"}, {});
    return c;
}

const std::vector<NodeClass>& node_classes() {
    static const std::vector<NodeClass> c = make_classes();
    return c;
}

const NodeClass* node_class(const std::string& name) {
    static const std::map<std::string, const NodeClass*> by = [] {
        std::map<std::string, const NodeClass*> m;
        for (auto& c : node_classes()) m[c.name] = &c;
        return m;
    }();
    auto it = by.find(name);
    return it == by.end() ? nullptr : it->second;
}

int NodeClass::index(const std::string& slot) const {
    for (size_t n = 0; n < slots.size(); n++)
        if (slots[n] == slot) return (int)n;
    return -1;
}
bool NodeClass::is_field(const std::string& slot) const {
    for (auto& f : fields)
        if (f == slot) return true;
    return false;
}

Item& Node::slot(const std::string& name) {
    int k = cls->index(name);
    if (k < 0) py::raise("AttributeError", std::string(cls->name) + " has no slot " + name);
    return slots[(size_t)k];
}
const Item* Node::find(const std::string& name) const {
    int k = cls->index(name);
    return k < 0 ? nullptr : &slots[(size_t)k];
}

// ---------------------------------------------------------------------------------------------- types
static const char* PRIMS[] = {"bool", "i8", "i16", "i32", "i64", "u8", "u16", "u32", "u64", "f32", "f64", "f80", "char"};
static bool is_prim(const std::string& k) {
    for (auto p : PRIMS)
        if (k == p) return true;
    return false;
}

static void keep(const V& d, Type& t, std::initializer_list<const char*> keys) {
    for (auto k : keys)
        if (d.has(k)) t.info.set(k, d.get(k));
}

static TypedEntry typed_entry(const V& p) {
    // dict(p, type=type_from(p['type'])): p's keys in order, `type` read as a Type
    TypedEntry e;
    e.dict = p;
    e.type = type_from(p.at("type"));
    return e;
}

TypeP type_from(const V& d) {
    if (d.is_none()) return nullptr;
    auto t = std::make_shared<Type>();
    const V& kv = d.at("kind");
    t->kind = kv.str();
    const std::string& k = t->kind;
    if (is_prim(k) || k == "void") {
        keep(d, *t, {"signed", "enc"});
    } else if (k == "null" || k == "any" || k == "dynamic") {
    } else if (k == "class") {
        t->name = d.at("name");
        if (d.has("args"))
            for (auto& a : d.at("args").seq()) t->args.push_back(type_from(a));
        if (py::truthy(d.get("nullable"))) t->info.set("nullable", V(true));
    } else if (k == "typevar") {
        t->name = d.at("name");
    } else if (k == "wildcard") {
        if (d.has("bound")) {
            t->has_bound = true;
            t->bound = d.at("bound");
            t->bound_type = type_from(d.at("type"));
        }
    } else if (k == "array") {
        t->elem = type_from(d.at("elem"));
        keep(d, *t, {"count", "rank"});
    } else if (k == "ptr") {
        t->elem = type_from(d.at("to"));
        t->info.set("space", d.at("space"));
        for (auto key : {"seg", "ref"}) {
            const V& v = d.get(key);
            if (!v.is_none() && !(v.is_bool() && !v.b)) t->info.set(key, v);
        }
    } else if (k == "selector") {
        keep(d, *t, {"of"});
    } else if (k == "fn") {
        keep(d, *t, {"conv", "distance", "pops", "loadds", "preserves", "variadic"});
        for (auto& p : d.at("params").seq()) t->params.push_back(typed_entry(p));
        t->result = py::copy_dict(d.at("result"));
        if (t->result.has("type")) {
            t->result_has_type = true;
            t->result_type = type_from(t->result.get("type"));
        }
    } else if (k == "fnptr") {
        t->info.set("space", d.at("space"));
        t->fn = type_from(d.at("fn"));
    } else if (k == "record") {
        keep(d, *t, {"size", "role", "packed"});
        for (auto& f : d.at("fields").seq()) t->fields.push_back(typed_entry(f));
        t->name = d.get("name");
    } else {
        py::raise("ValueError", "no IR form for L2 type kind " + py::str_repr(k));
    }
    return t;
}

static V entry_json(const TypedEntry& e) {
    V d = py::copy_dict(e.dict);
    d.set("type", type_json(e.type));
    return d;
}

static const V& info_or(const Type& t, const char* key, const V& dflt) {
    return t.info.has(key) ? t.info.get(key) : dflt;
}

V type_json(const TypeP& tp) {
    if (!tp) return V();
    const Type& t = *tp;
    const std::string& k = t.kind;
    V d = V::dict();
    d.set("kind", V(k));
    if (is_prim(k) || k == "void") {
        for (auto key : {"signed", "enc"})
            if (t.info.has(key)) d.set(key, t.info.get(key));
        return d;
    }
    if (k == "null" || k == "any" || k == "dynamic") return d;
    if (k == "class") {
        d.set("name", t.name);
        if (!t.args.empty()) {
            V a = V::list();
            for (auto& x : t.args) a.append(type_json(x));
            d.set("args", a);
        }
        if (py::truthy(t.info.get("nullable"))) d.set("nullable", V(true));
        return d;
    }
    if (k == "typevar") {
        d.set("name", t.name);
        return d;
    }
    if (k == "wildcard") {
        if (t.has_bound) {
            d.set("bound", t.bound);
            d.set("type", type_json(t.bound_type));
        }
        return d;
    }
    if (k == "array") {
        d.set("elem", type_json(t.elem));
        if (t.info.has("count")) d.set("count", t.info.get("count"));
        if (t.info.has("rank") && !py::eq(t.info.get("rank"), V(1))) d.set("rank", t.info.get("rank"));
        return d;
    }
    if (k == "ptr") {
        d.set("space", info_or(t, "space", V("flat")));
        d.set("to", type_json(t.elem));
        if (t.info.has("seg")) d.set("seg", t.info.get("seg"));
        if (py::truthy(t.info.get("ref"))) d.set("ref", V(true));
        return d;
    }
    if (k == "selector") {
        if (t.info.has("of")) d.set("of", t.info.get("of"));
        return d;
    }
    if (k == "fn") {
        d.set("conv", info_or(t, "conv", V("cdecl")));
        V ps = V::list();
        for (auto& p : t.params) ps.append(entry_json(p));
        d.set("params", ps);
        V res = t.result.is_none() ? V::dict() : py::copy_dict(t.result);
        if (t.result.is_none()) res.set("at", V("none"));
        if (res.has("type")) res.set("type", type_json(t.result_type));
        d.set("result", res);
        for (auto key : {"distance", "pops", "loadds", "preserves", "variadic"})
            if (t.info.has(key)) d.set(key, t.info.get(key));
        return d;
    }
    if (k == "fnptr") {
        d.set("space", info_or(t, "space", V("flat")));
        d.set("fn", type_json(t.fn));
        return d;
    }
    if (k == "record") {
        d.set("size", t.info.at("size"));
        V fs = V::list();
        for (auto& f : t.fields) fs.append(entry_json(f));
        d.set("fields", fs);
        if (py::truthy(t.name)) d.set("name", t.name);
        for (auto key : {"role", "packed"})
            if (t.info.has(key)) d.set(key, t.info.get(key));
        return d;
    }
    py::raise("ValueError", "no L2 form for type kind " + py::str_repr(k));
}

// ---------------------------------------------------------------------------------------------- symbols
static std::vector<V> as_tuple(const V& v) {
    if (v.is_none()) py::raise("TypeError", "'NoneType' object is not iterable");
    if (v.is_str()) {
        std::vector<V> out;
        const std::string& s = v.str();
        size_t p = 0;
        while (p < s.size()) {
            size_t st = p;
            py::decode_cp(s, p);
            out.push_back(V(s.substr(st, p - st)));
        }
        return out;
    }
    if (v.is_dict()) {
        std::vector<V> out;
        for (auto& kv : v.items()) out.push_back(kv.first);
        return out;
    }
    return v.seq();
}

SymP sym_from(const V& d) {
    if (d.is_none()) return nullptr;
    auto s = std::make_shared<Sym>();
    s->kind = d.at("kind");
    s->name = d.at("name");
    s->owner = d.get("owner");
    if (d.has("static")) s->stat = d.get("static");
    s->type = type_from(d.get("type"));
    if (d.has("params"))
        for (auto& p : as_tuple(d.get("params"))) s->params.push_back(type_from(p));
    s->ret = type_from(d.get("ret"));
    if (d.has("varargs")) s->varargs = d.get("varargs");
    if (d.has("refs")) s->refs = as_tuple(d.get("refs"));
    if (d.has("flags")) s->flags = as_tuple(d.get("flags"));
    return s;
}

V sym_json(const SymP& sp) {
    if (!sp) return V();
    const Sym& s = *sp;
    V d = V::dict();
    d.set("kind", s.kind);
    d.set("name", s.name);
    if (!s.owner.is_none()) d.set("owner", s.owner);
    if (py::truthy(s.stat)) d.set("static", V(true));
    if (s.type) d.set("type", type_json(s.type));
    if (!s.params.empty() || s.kind == "method" || s.kind == "ctor" || s.kind == "function") {
        V ps = V::list();
        for (auto& p : s.params) ps.append(type_json(p));
        d.set("params", ps);
    }
    if (s.ret) d.set("ret", type_json(s.ret));
    if (py::truthy(s.varargs)) d.set("varargs", V(true));
    bool any = false;
    for (auto& r : s.refs) any = any || py::truthy(r);
    if (any) d.set("refs", V::list(s.refs));
    if (!s.flags.empty()) d.set("flags", V::list(s.flags));
    return d;
}

// ---------------------------------------------------------------------------------------------- nodes
static bool is_node_dict(const V& v) { return v.is_dict() && v.has("node"); }
static bool is_type_dict(const V& v) { return v.is_dict() && v.has("kind"); }

static bool typed_slot(const std::string& s) {
    return s == "type" || s == "extends" || s == "ret" || s == "elem" || s == "test" || s == "of" || s == "qualifier" ||
           s == "layout";
}
static bool type_list_slot(const std::string& s) {
    return s == "implements" || s == "throws" || s == "types" || s == "typeargs" || s == "bounds";
}

NodeP from_json(const V& d) {
    const V& name = d.at("node");
    const NodeClass* cls = name.is_str() ? node_class(name.str()) : nullptr;
    if (!cls) py::raise("KeyError", py::repr(name));
    auto n = std::make_shared<Node>();
    n->cls = cls;
    n->line = d.has("line") ? d.get("line") : V(0);
    n->col = d.has("col") ? d.get("col") : V(0);
    n->origin = d.get("origin");
    n->slots.resize(cls->slots.size());
    for (size_t k = 0; k < cls->slots.size(); k++) {
        const std::string& slot = cls->slots[k];
        if (!d.has(slot.c_str())) continue;
        const V& v = d.get(slot.c_str());
        Item& it = n->slots[k];
        if (slot == "sym" || slot == "ctor") {
            it.kind = Item::SymK;
            it.sym = sym_from(v);
            if (!it.sym) it = Item();
        } else if (is_node_dict(v)) {
            it.kind = Item::NodeK;
            it.node = from_json(v);
        } else if (v.is_list() && v.size() > 0 && [&] {
                       for (auto& x : v.seq())
                           if (!is_node_dict(x)) return false;
                       return true;
                   }()) {
            it.kind = Item::Nodes;
            for (auto& x : v.seq()) it.nodes.push_back(from_json(x));
        } else if (typed_slot(slot) && is_type_dict(v)) {
            it.kind = Item::TypeK;
            it.type = type_from(v);
        } else if (type_list_slot(slot) && v.is_list() && [&] {
                       for (auto& x : v.seq())
                           if (!is_type_dict(x)) return false;
                       return true;
                   }()) {
            it.kind = Item::Types;
            for (auto& x : v.seq()) it.types.push_back(type_from(x));
        } else {
            it.kind = Item::Raw;
            it.raw = v;
        }
    }
    return n;
}

static V raw_value(const V& v) {
    // _value of a value that is no node, type or symbol: lists element by element, a float that is not
    // finite as its repr, anything else as it is (a dict untouched)
    if (v.is_list() || v.is_tuple()) {
        V out = V::list();
        for (auto& x : v.seq()) out.append(raw_value(x));
        return out;
    }
    if (v.is_float() && !std::isfinite(v.f)) return V(py::float_repr(v.f));
    return v;
}

static V item_value(const Item& it) {
    switch (it.kind) {
        case Item::Raw: return raw_value(it.raw);
        case Item::NodeK: return to_json(*it.node);
        case Item::TypeK: return type_json(it.type);
        case Item::SymK: return sym_json(it.sym);
        case Item::Nodes: {
            V out = V::list();
            for (auto& x : it.nodes) out.append(to_json(*x));
            return out;
        }
        case Item::Types: {
            V out = V::list();
            for (auto& x : it.types) out.append(type_json(x));
            return out;
        }
    }
    return V();
}

static bool is_none_item(const Item& it) {
    switch (it.kind) {
        case Item::Raw: return it.raw.is_none();
        case Item::NodeK: return !it.node;
        case Item::TypeK: return !it.type;
        case Item::SymK: return !it.sym;
        default: return false;
    }
}
static bool is_empty_list(const Item& it) {
    if (it.kind == Item::Raw) return it.raw.is_list() && it.raw.size() == 0;
    if (it.kind == Item::Nodes) return it.nodes.empty();
    if (it.kind == Item::Types) return it.types.empty();
    return false;
}

V to_json(const Node& n) {
    V d = V::dict();
    d.set("node", V(n.cls->name));
    if (py::truthy(n.line)) d.set("line", n.line);
    if (py::truthy(n.col)) d.set("col", n.col);
    if (!n.origin.is_none()) d.set("origin", n.origin);
    for (size_t k = 0; k < n.cls->slots.size(); k++) {
        const std::string& slot = n.cls->slots[k];
        const Item& it = n.slots[k];
        if (is_none_item(it)) continue;
        if (!n.cls->is_field(slot) && is_empty_list(it) && slot != "params" && slot != "args") continue;
        d.set(V(slot), item_value(it));
    }
    return d;
}

V roundtrip(const V& d) { return to_json(*from_json(d)); }

// ---------------------------------------------------------------------------------------------- the schema
Schema::Schema(const V& schema) : root_(schema) {}

const V& Schema::resolve(const std::string& ref) const {
    if (ref == "#") return root_;
    if (!py::startswith(ref, "#/")) py::raise("ValueError", "schema: unsupported $ref " + ref);
    const V* cur = &root_;
    for (auto& part : py::split(ref.substr(2), "/")) cur = &cur->at(part.c_str());
    return *cur;
}

static bool same(const V& a, const V& b) {
    // jsonschema's equal: a bool is not a number
    if (a.is_bool() != b.is_bool()) return false;
    if (a.is_list() && b.is_list()) {
        if (a.size() != b.size()) return false;
        for (size_t n = 0; n < a.size(); n++)
            if (!same(a[n], b[n])) return false;
        return true;
    }
    if (a.is_dict() && b.is_dict()) {
        if (a.size() != b.size()) return false;
        for (auto& kv : a.items())
            if (!b.has(kv.first) || !same(kv.second, b.get(kv.first))) return false;
        return true;
    }
    return py::eq(a, b);
}

static bool type_is(const std::string& t, const V& v) {
    if (t == "object") return v.is_dict();
    if (t == "array") return v.is_list();
    if (t == "string") return v.is_str();
    if (t == "boolean") return v.is_bool();
    if (t == "null") return v.is_none();
    if (t == "integer") return v.is_int() || (v.is_float() && std::isfinite(v.f) && v.f == std::floor(v.f));
    if (t == "number") return v.is_int() || v.is_float();
    return false;
}

bool Schema::check(const V& s, const V& v, std::string& why, const std::string& at) const {
    if (s.is_bool()) {
        if (!s.b) why = at + ": not allowed";
        return s.b;
    }
    for (auto& kv : s.items()) {
        const std::string& key = kv.first.str();
        const V& w = kv.second;
        if (key == "$ref") {
            if (!check(resolve(w.str()), v, why, at)) return false;
        } else if (key == "type") {
            bool ok = false;
            if (w.is_str()) ok = type_is(w.str(), v);
            else
                for (auto& t : w.seq()) ok = ok || type_is(t.str(), v);
            if (!ok) {
                why = at + ": " + py::repr(v).substr(0, 80) + " is not of type " + py::repr(w);
                return false;
            }
        } else if (key == "const") {
            if (!same(w, v)) {
                why = at + ": " + py::repr(w) + " was expected";
                return false;
            }
        } else if (key == "enum") {
            bool ok = false;
            for (auto& e : w.seq()) ok = ok || same(e, v);
            if (!ok) {
                why = at + ": " + py::repr(v).substr(0, 80) + " is not one of " + py::repr(w).substr(0, 120);
                return false;
            }
        } else if (key == "anyOf" || key == "oneOf" || key == "allOf") {
            int good = 0;
            std::string sub;
            for (auto& alt : w.seq()) {
                std::string w2;
                if (check(alt, v, w2, at)) good++;
                else if (sub.empty()) sub = w2;
                else if (key == "allOf") {
                }
                if (key == "allOf" && good == 0 && !w2.empty()) {
                    why = w2;
                    return false;
                }
            }
            bool ok = key == "anyOf" ? good > 0 : key == "oneOf" ? good == 1 : good == (int)w.size();
            if (!ok) {
                why = at + ": " + py::repr(v).substr(0, 80) + " is not valid under " + key + (sub.empty() ? "" : " (" + sub + ")");
                return false;
            }
        } else if (key == "if") {
            std::string w2;
            bool cond = check(w, v, w2, at);
            const V& then = s.get(cond ? "then" : "else");
            if (!then.is_none() && !check(then, v, why, at)) return false;
        } else if (key == "properties") {
            if (!v.is_dict()) continue;
            for (auto& p : w.items())
                if (v.has(p.first) && !check(p.second, v.get(p.first), why, at + "/" + p.first.str())) return false;
        } else if (key == "additionalProperties") {
            if (!v.is_dict()) continue;
            const V& props = s.get("properties");
            for (auto& p : v.items()) {
                if (props.is_dict() && props.has(p.first)) continue;
                if (w.is_bool() && !w.b) {
                    why = at + ": additional property " + py::repr(p.first) + " not allowed";
                    return false;
                }
                if (!w.is_bool() && !check(w, p.second, why, at + "/" + p.first.str())) return false;
            }
        } else if (key == "required") {
            if (!v.is_dict()) continue;
            for (auto& r : w.seq())
                if (!v.has(r)) {
                    why = at + ": " + py::repr(r) + " is a required property";
                    return false;
                }
        } else if (key == "items") {
            if (!v.is_list()) continue;
            size_t n = 0;
            for (auto& x : v.seq())
                if (!check(w, x, why, at + "/" + std::to_string(n++))) return false;
        } else if (key == "minItems") {
            if (v.is_list() && (int64_t)v.size() < w.as_int()) {
                why = at + ": too short";
                return false;
            }
        } else if (key == "minimum") {
            if ((v.is_int() || v.is_float()) && v.as_float() < w.as_float()) {
                why = at + ": below the minimum";
                return false;
            }
        }
        // (description, title, default, $schema, $id, $defs, then, else: no assertion)
    }
    return true;
}

bool Schema::valid(const V& doc, std::string* why) const {
    std::string w;
    bool ok = check(root_, doc, w, "");
    if (why) *why = w;
    return ok;
}

}  // namespace l2
