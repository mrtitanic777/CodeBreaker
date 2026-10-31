// cpp17_jmodel.cc -- the Java object model (cpp17_jmodel.hpp; jmodel.py).
#include <algorithm>
#include <deque>
#include <functional>
#include <set>

#include "cpp17_jmodel.hpp"

namespace cpp17 {
namespace jm {

const char* const OBJECT = "java.lang.Object";

namespace {
TypeP T_() { return mk_type("typevar", "T"); }
TypeP P(const char* k) { return mk_type(k); }
TypeP C(const char* n) { return mk_type("class", n); }
}  // namespace

const std::vector<std::pair<std::string, std::vector<std::string>>>& lib_supers() {
    static const std::vector<std::pair<std::string, std::vector<std::string>>> v = {
        {"java.lang.String", {"java.lang.CharSequence", "java.lang.Comparable"}},
        {"java.util.ArrayList", {"java.util.List"}},
        {"java.util.LinkedList", {"java.util.List", "java.util.Deque"}},
        {"java.util.List", {"java.util.Collection"}},
        {"java.util.Set", {"java.util.Collection"}},
        {"java.util.HashSet", {"java.util.Set"}},
        {"java.util.LinkedHashSet", {"java.util.Set"}},
        {"java.util.TreeSet", {"java.util.Set"}},
        {"java.util.Collection", {"java.lang.Iterable"}},
        {"java.util.Deque", {"java.util.Queue"}},
        {"java.util.Queue", {"java.util.Collection"}},
        {"java.util.ArrayDeque", {"java.util.Deque"}},
        {"java.util.HashMap", {"java.util.Map"}},
        {"java.util.LinkedHashMap", {"java.util.HashMap"}},
        {"java.util.TreeMap", {"java.util.Map"}},
        {"java.lang.Exception", {"java.lang.Throwable"}},
        {"java.lang.Error", {"java.lang.Throwable"}},
        {"java.lang.RuntimeException", {"java.lang.Exception"}},
        {"java.lang.IllegalArgumentException", {"java.lang.RuntimeException"}},
        {"java.lang.IllegalStateException", {"java.lang.RuntimeException"}},
        {"java.lang.ArithmeticException", {"java.lang.RuntimeException"}},
        {"java.lang.NullPointerException", {"java.lang.RuntimeException"}},
        {"java.lang.ClassCastException", {"java.lang.RuntimeException"}},
        {"java.lang.UnsupportedOperationException", {"java.lang.RuntimeException"}},
        {"java.lang.IndexOutOfBoundsException", {"java.lang.RuntimeException"}},
        {"java.lang.ArrayIndexOutOfBoundsException", {"java.lang.IndexOutOfBoundsException"}},
        {"java.lang.StringIndexOutOfBoundsException", {"java.lang.IndexOutOfBoundsException"}},
        {"java.lang.NumberFormatException", {"java.lang.IllegalArgumentException"}},
        {"java.util.NoSuchElementException", {"java.lang.RuntimeException"}},
        {"java.io.IOException", {"java.lang.Exception"}},
        {"java.lang.Integer", {"java.lang.Number", "java.lang.Comparable"}},
        {"java.lang.Long", {"java.lang.Number", "java.lang.Comparable"}},
        {"java.lang.Double", {"java.lang.Number", "java.lang.Comparable"}},
        {"java.lang.Enum", {"java.lang.Comparable"}},
        {"java.lang.Record", {}},
        {"java.util.function.UnaryOperator", {"java.util.function.Function"}},
        {"java.util.function.BinaryOperator", {"java.util.function.BiFunction"}},
    };
    return v;
}

const std::vector<std::string>* lib_supers_of(const std::string& q) {
    for (auto& p : lib_supers())
        if (p.first == q) return &p.second;
    return nullptr;
}

const std::vector<std::pair<std::string, Sam>>& sams() {
    static const std::vector<std::pair<std::string, Sam>> v = [] {
        TypeP T = T_(), I = P("i32"), B = P("bool"), V = P("void"), L = P("i64"), D = P("f64");
        return std::vector<std::pair<std::string, Sam>>{
            {"java.lang.Runnable", {"run", {}, V}},
            {"java.util.function.Function", {"apply", {T}, T}},
            {"java.util.function.BiFunction", {"apply", {T, T}, T}},
            {"java.util.function.UnaryOperator", {"apply", {T}, T}},
            {"java.util.function.BinaryOperator", {"apply", {T, T}, T}},
            {"java.util.function.Supplier", {"get", {}, T}},
            {"java.util.function.Consumer", {"accept", {T}, V}},
            {"java.util.function.BiConsumer", {"accept", {T, T}, V}},
            {"java.util.function.Predicate", {"test", {T}, B}},
            {"java.util.function.BiPredicate", {"test", {T, T}, B}},
            {"java.util.function.IntBinaryOperator", {"applyAsInt", {I, I}, I}},
            {"java.util.function.IntUnaryOperator", {"applyAsInt", {I}, I}},
            {"java.util.function.IntPredicate", {"test", {I}, B}},
            {"java.util.function.IntFunction", {"apply", {I}, T}},
            {"java.util.function.IntConsumer", {"accept", {I}, V}},
            {"java.util.function.IntSupplier", {"getAsInt", {}, I}},
            {"java.util.function.ToIntFunction", {"applyAsInt", {T}, I}},
            {"java.util.function.ToLongFunction", {"applyAsLong", {T}, L}},
            {"java.util.function.ToDoubleFunction", {"applyAsDouble", {T}, D}},
            {"java.util.function.IntToDoubleFunction", {"applyAsDouble", {I}, D}},
            {"java.util.function.IntToLongFunction", {"applyAsLong", {I}, L}},
            {"java.util.function.DoubleUnaryOperator", {"applyAsDouble", {D}, D}},
            {"java.util.function.DoubleBinaryOperator", {"applyAsDouble", {D, D}, D}},
            {"java.util.function.LongBinaryOperator", {"applyAsLong", {L, L}, L}},
            {"java.util.function.LongUnaryOperator", {"applyAsLong", {L}, L}},
            {"java.util.Comparator", {"compare", {T, T}, I}},
        };
    }();
    return v;
}

const Sam* sam_of_lib(const std::string& q) {
    for (auto& p : sams())
        if (p.first == q) return &p.second;
    return nullptr;
}

const std::vector<Sam>* lib_methods_of(const std::string& q) {
    static const std::vector<std::pair<std::string, std::vector<Sam>>> v = [] {
        TypeP T = T_(), I = P("i32"), B = P("bool"), V = P("void");
        std::vector<std::pair<std::string, std::vector<Sam>>> m = {
            {OBJECT, {{"toString", {}, C("java.lang.String")}, {"equals", {C(OBJECT)}, B}, {"hashCode", {}, I}}},
            {"java.lang.Comparable", {{"compareTo", {T}, I}}},
            {"java.lang.Iterable", {{"iterator", {}, C("java.util.Iterator")}, {"forEach", {T}, V}}},
            {"java.util.Iterator", {{"hasNext", {}, B}, {"next", {}, T}, {"remove", {}, V}}},
            {"java.lang.Throwable", {{"getMessage", {}, C("java.lang.String")}}},
            {"java.lang.CharSequence", {{"length", {}, I}, {"charAt", {I}, P("u16")}}},
        };
        for (auto& s : sams()) {
            bool found = false;
            for (auto& e : m)
                if (e.first == s.first) {
                    e.second.push_back(s.second);
                    found = true;
                }
            if (!found) m.push_back({s.first, {s.second}});
        }
        return m;
    }();
    for (auto& p : v)
        if (p.first == q) return &p.second;
    return nullptr;
}

// ---------------------------------------------------------------------------------------------- classes
std::vector<NodeP> ClassInfo::methods(const std::string* name) const {
    std::vector<NodeP> out;
    for (auto& m : members()) {
        if (!is(m, "MethodDecl")) continue;
        std::string k = str(raw(m, "kind"));
        if ((k == "method" || k == "ctor") && (!name || str(raw(m, "name")) == *name)) out.push_back(m);
    }
    return out;
}

std::vector<NodeP> ClassInfo::fields() const {
    std::vector<NodeP> out;
    for (auto& m : members())
        if (is(m, "FieldDecl")) out.push_back(m);
    return out;
}

TypeP erase(const TypeP& t) {
    if (!t) return mk_type("void");
    if (t->kind == "typevar" || t->kind == "wildcard" || t->kind == "any") return mk_type("class", OBJECT);
    if (t->kind == "array") return mk_type("array", "", {}, erase(t->elem));
    if (t->kind == "class" && !t->args.empty()) return mk_type("class", tname(t));
    return t;
}

Model::Model(const std::vector<NodeP>& units) {
    for (auto& u : units)
        for (auto& c : nodes(u, "types")) add(c, nullptr, str(raw(u, "package")));
    std::vector<ClassInfo*> snapshot = order_;
    for (auto* ci : snapshot) supers_of(ci);
    snapshot = order_;
    for (auto* ci : snapshot)
        if (ci->local || ci->anon) capture(ci);
}

std::string Model::cname_of(const std::string& qname, const std::string& package) const {
    std::string q = qname;
    if (!package.empty() && starts(q, package + ".")) q = q.substr(package.size() + 1);
    return py::replace(py::replace(q, ".", "_"), "$", "_");
}

ClassInfo* Model::add(const NodeP& c, ClassInfo* outer, const std::string& package, bool local, bool anon,
                      const NodeP& method) {
    std::string qname = str(raw(c, "qname"));
    std::string cname, key;
    if (anon || qname.empty() || local) {
        anon_++;
        std::string base = outer ? outer->cname : "Local";
        std::string nm = str(raw(c, "name"));
        cname = base + "_" + (anon ? std::string("anon") : (nm.empty() ? std::string("local") : nm)) + std::to_string(anon_);
        key = (!qname.empty() && !anon) ? qname : "<" + cname + ">";
    } else {
        cname = cname_of(qname, package);
        key = qname;
    }
    owned_.emplace_back(new ClassInfo());
    ClassInfo* ci = owned_.back().get();
    ci->decl = c;
    ci->qname = key;
    ci->cname = cname;
    ci->outer = outer;
    ci->local = local;
    ci->anon = anon;
    ci->kind = str(raw(c, "kind"));
    bool stat = has_mod(c, "static") || ci->kind == "interface" || ci->kind == "enum" || ci->kind == "record" ||
                ci->kind == "annotation";
    ci->inner = outer && !stat && !local && !anon && outer->kind != "interface";
    ci->needs_outer = ci->inner;
    ci->package = package;
    ci->method = method;
    if ((local || anon) && method && outer)
        ci->needs_outer = !sstatic(sym(method)) && str(raw(method, "kind")) != "static_init";
    auto it = index_.find(key);
    if (it != index_.end()) order_[it->second] = ci;
    else {
        index_[key] = order_.size();
        order_.push_back(ci);
    }
    by_decl_[c.get()] = ci;
    for (auto& m : nodes(c, "members")) {
        if (is(m, "ClassDecl")) {
            add(m, ci, package);
        } else if (is(m, "EnumConst") && node(m, "body")) {
            ClassInfo* sub = add(node(m, "body"), ci, package, false, true);
            sub->enum_const = m;
            sub->needs_outer = false;
            sub->base = ci->qname;
        } else if (is(m, "MethodDecl") || is(m, "FieldDecl")) {
            nested_in(m, ci, package);
        }
    }
    return ci;
}

void Model::nested_in(const NodeP& m, ClassInfo* ci, const std::string& package) {
    NodeP method = is(m, "MethodDecl") ? m : nullptr;
    NodeP root = is(m, "MethodDecl") ? node(m, "body") : node(m, "init");
    if (!root) return;
    std::vector<NodeP> stack{root};
    while (!stack.empty()) {
        NodeP n = stack.back();
        stack.pop_back();
        if (is(n, "LocalClass")) {
            add(node(n, "decl"), ci, package, true, false, method);
            continue;
        }
        if (is(n, "New") && node(n, "body")) {
            for (auto& a : nodes(n, "args")) stack.push_back(a);
            if (node(n, "outer")) stack.push_back(node(n, "outer"));
            ClassInfo* sub = add(node(n, "body"), ci, package, false, true, method);
            sub->new_ = n;
            if (!method) sub->needs_outer = !sstatic(sym(m));       // a field initialiser: an instance one has `this`
            continue;
        }
        for (auto& c : children(n)) stack.push_back(c);
    }
}

void Model::supers_of(ClassInfo* ci) {
    const NodeP& d = ci->decl;
    TypeP ext = type(d, "extends");
    if (ci->anon && ci->new_) {
        TypeP t = ext ? ext : type(ci->new_);
        std::string target = t && t->kind == "class" ? tname(t) : OBJECT;
        ClassInfo* tci = info(target);
        bool lib_m = lib_methods_of(target) != nullptr;
        if ((tci && tci->kind == "interface") || (!tci && sam_of_lib(target)) ||
            (!tci && lib_m && target != OBJECT && target != "java.lang.Throwable")) {
            ci->base = OBJECT;
            ci->ifaces = {target};
        } else {
            ci->base = target;
        }
        return;
    }
    if (ci->enum_const) return;
    std::string k = str(raw(d, "kind"));
    if (k == "enum") ci->base = "java.lang.Enum";
    else if (k == "record") ci->base = OBJECT;
    else if (ext && ext->kind == "class") ci->base = tname(ext);
    else ci->base = k != "interface" ? OBJECT : "";
    ci->ifaces.clear();
    for (auto& t : types(d, "implements"))
        if (t && t->kind == "class") ci->ifaces.push_back(tname(t));
    if (k == "interface" && ext) ci->ifaces.insert(ci->ifaces.begin(), tname(ext));
}

void Model::capture(ClassInfo* ci) {
    std::set<std::string> declared;
    auto all = walk(ci->decl);
    for (auto& n : all)
        if ((is(n, "Local") || is(n, "Param")) && sym(n)) declared.insert(sname(sym(n)));
    std::vector<std::pair<std::string, TypeP>> used;
    for (auto& n : all) {
        if (is(n, "Name") && sym(n) && !declared.count(sname(sym(n)))) {
            bool have = false;
            for (auto& u : used) have = have || u.first == sname(sym(n));
            if (!have) used.push_back({sname(sym(n)), type(n)});
        }
    }
    ci->captures = used;
}

void Model::name_anonymous() {
    // javac names an anonymous (or local) class's members as owned by '' -- give them the class's key,
    // so a field or method of one is found like any other's
    for (auto* ci : order_) {
        if (!(ci->anon || ci->local)) continue;
        std::vector<NodeP> stack = ci->members();
        while (!stack.empty()) {
            NodeP n = stack.back();
            stack.pop_back();
            if (is(n, "ClassDecl") && n.get() != ci->decl.get()) continue;
            SymP s = sym(n);
            if (s && s->owner.is_str() && s->owner.str().empty()) {
                std::string k = skind(s);
                if (k == "field" || k == "method" || k == "ctor") s->owner = V(ci->qname);
            }
            if (is(n, "This")) {
                TypeP t = type(n);
                if (t && t->kind == "class" && tname(t).empty()) set_type(n, mk_type("class", ci->qname));
            }
            if (is(n, "MethodDecl") && node(n, "body")) stack.push_back(node(n, "body"));
            else if (is(n, "FieldDecl") && node(n, "init")) stack.push_back(node(n, "init"));
            else if (is(n, "New") && node(n, "body")) {
                for (auto& a : nodes(n, "args")) stack.push_back(a);
                if (node(n, "outer")) stack.push_back(node(n, "outer"));
            } else {
                for (auto& c : children(n)) stack.push_back(c);
            }
        }
    }
}

// ---------------------------------------------------------------------------------------------- lookups
ClassInfo* Model::info(const std::string& q) const {
    if (q.empty()) return nullptr;
    auto it = index_.find(q);
    return it == index_.end() ? nullptr : order_[it->second];
}

ClassInfo* Model::of_type(const TypeP& t) const { return t && t->kind == "class" ? info(tname(t)) : nullptr; }

ClassInfo* Model::by_decl(const l2::Node* n) const {
    auto it = by_decl_.find(n);
    return it == by_decl_.end() ? nullptr : it->second;
}

std::vector<std::string> Model::supers(const std::string& q) const {
    ClassInfo* ci = info(q);
    if (ci) {
        std::vector<std::string> out;
        if (!ci->base.empty()) out.push_back(ci->base);
        out.insert(out.end(), ci->ifaces.begin(), ci->ifaces.end());
        return out;
    }
    std::vector<std::string> out;
    if (auto ls = lib_supers_of(q)) out = *ls;
    if (q != OBJECT) out.push_back(OBJECT);
    return out;
}

std::vector<std::string> Model::ancestors(const std::string& q) const {
    std::vector<std::string> out;
    std::deque<std::string> todo{q};
    while (!todo.empty()) {
        std::string x = todo.front();
        todo.pop_front();
        if (std::find(out.begin(), out.end(), x) != out.end()) continue;
        out.push_back(x);
        for (auto& s : supers(x)) todo.push_back(s);
    }
    return out;
}

bool Model::is_sub(const std::string& a, const std::string& b) const {
    if (b == OBJECT) return true;
    auto an = ancestors(a);
    return std::find(an.begin(), an.end(), b) != an.end();
}

bool Model::is_interface(const std::string& q) const {
    ClassInfo* ci = info(q);
    if (ci) return ci->kind == "interface";
    static const std::set<std::string> ifs = {"java.lang.Comparable", "java.lang.Iterable", "java.util.Iterator",
                                              "java.lang.CharSequence", "java.util.List", "java.util.Map",
                                              "java.util.Set", "java.util.Collection"};
    return sam_of_lib(q) || ifs.count(q);
}

bool Model::subclassed(const std::string& q) const {
    for (auto* ci : order_)
        if (ci->base == q) return true;
    return false;
}

std::pair<ClassInfo*, NodeP> Model::method_decl(const SymP& s) const {
    if (!s) return {nullptr, nullptr};
    std::string want = skind(s) == "method" ? sname(s) : std::string("<init>");
    for (auto& q : ancestors(sowner(s))) {
        ClassInfo* ci = info(q);
        if (!ci) continue;
        for (auto& m : ci->methods(&want)) {
            auto& ps = nodes(m, "params");
            if (ps.size() != s->params.size()) continue;
            bool ok = true;
            for (size_t i = 0; i < ps.size() && ok; i++) ok = type_eq(erase(type(ps[i])), erase(s->params[i]));
            if (ok) return {ci, m};
        }
        if (skind(s) == "ctor") break;
    }
    return {nullptr, nullptr};
}

static bool matches(const TypeP& super_t, const TypeP& t) {
    if (!super_t || !t) return false;
    if (super_t->kind == "typevar" || super_t->kind == "wildcard") return is_ref_type(t);
    return type_eq(erase(super_t), erase(t));
}

bool Model::overrides(ClassInfo* ci, const NodeP& m, std::vector<TypeP>& params, TypeP& ret) const {
    if (str(raw(m, "kind")) != "method" || sstatic(sym(m)) || has_mod(m, "private")) return false;
    bool found = false;
    std::set<std::string> seen;
    std::deque<std::string> todo;
    for (auto& s : supers(ci->qname)) todo.push_back(s);
    auto& mps = nodes(m, "params");
    std::string mname = str(raw(m, "name"));
    while (!todo.empty()) {
        std::string q = todo.front();
        todo.pop_front();
        if (seen.count(q)) continue;
        seen.insert(q);
        ClassInfo* sci = info(q);
        if (sci) {
            for (auto& sm : sci->methods(&mname)) {
                auto& sps = nodes(sm, "params");
                if (str(raw(sm, "kind")) != "method" || sps.size() != mps.size()) continue;
                bool ok = true;
                for (size_t i = 0; i < sps.size() && ok; i++) ok = matches(type(sps[i]), type(mps[i]));
                if (ok && !sstatic(sym(sm)) && !has_mod(sm, "private")) {
                    params.clear();
                    for (auto& sp : sps) params.push_back(type(sp));
                    ret = type(sm, "ret");
                    found = true;
                }
            }
        } else if (auto lm = lib_methods_of(q)) {
            for (auto& s : *lm) {
                if (s.name != mname || s.params.size() != mps.size()) continue;
                bool ok = true;
                for (size_t i = 0; i < s.params.size() && ok; i++) ok = matches(s.params[i], type(mps[i]));
                if (ok) {
                    params = s.params;
                    ret = s.ret;
                    found = true;
                }
            }
        }
        for (auto& s : supers(q)) todo.push_back(s);
    }
    return found;
}

std::optional<Sam> Model::sam(const std::string& q) const {
    if (auto s = sam_of_lib(q)) return *s;
    ClassInfo* ci = info(q);
    if (!ci || ci->kind != "interface") return std::nullopt;
    std::vector<Sam> abstract;
    for (auto& a : ancestors(q)) {
        ClassInfo* sci = info(a);
        if (!sci) {
            if (auto s = sam_of_lib(a)) abstract.push_back(*s);
            continue;
        }
        if (sci->kind != "interface") continue;
        for (auto& m : sci->methods()) {
            std::string n = str(raw(m, "name"));
            if (!node(m, "body") && !sstatic(sym(m)) && n != "toString" && n != "equals" && n != "hashCode") {
                bool dup = false;
                for (auto& x : abstract) dup = dup || (x.name == n && x.params.size() == nodes(m, "params").size());
                if (!dup) {
                    Sam s;
                    s.name = n;
                    for (auto& p : nodes(m, "params")) s.params.push_back(type(p));
                    s.ret = type(m, "ret");
                    abstract.push_back(s);
                }
            }
        }
    }
    if (abstract.size() == 1) return abstract[0];
    return std::nullopt;
}

static void refs(const TypeP& t, std::vector<std::string>& out) {
    if (!t) return;
    if (t->kind == "array") return refs(t->elem, out);
    if (t->kind == "class") {
        out.push_back(tname(t));
        for (auto& a : t->args) refs(a, out);
    }
    if (t->kind == "typevar" || t->kind == "wildcard") out.push_back(OBJECT);
}

std::vector<std::string> Model::cycles() const {
    static const std::set<std::string> anything = {
        "java.lang.Object", "java.util.List", "java.util.ArrayList", "java.util.Map", "java.util.HashMap",
        "java.util.Set", "java.util.HashSet", "java.util.function.Function", "java.util.function.Supplier",
        "java.lang.Runnable"};
    std::vector<std::pair<std::string, std::set<std::string>>> graph;
    std::unordered_map<std::string, size_t> gi;
    for (auto* ci : order_) {
        std::set<std::string> out;
        for (auto& f : ci->fields()) {
            SymP s = sym(f);
            if (!s || sstatic(s)) continue;
            std::vector<std::string> rs;
            refs(type(f), rs);
            for (auto& q : rs) {
                if (info(q)) out.insert(q);
                else if (anything.count(q)) out.insert("*");
            }
        }
        if (ci->needs_outer && ci->outer) out.insert(ci->outer->qname);
        if (gi.count(ci->qname)) graph[gi[ci->qname]].second = out;
        else {
            gi[ci->qname] = graph.size();
            graph.push_back({ci->qname, out});
        }
    }
    std::vector<std::string> result;
    for (auto& g : graph) {
        const std::string& q = g.first;
        std::set<std::string> seen;
        std::vector<std::string> todo(g.second.begin(), g.second.end());
        // (Python iterates a set: the order a cycle is found in does not change whether one is)
        bool hit = false;
        while (!todo.empty()) {
            std::string x = todo.back();
            todo.pop_back();
            if (x == q || x == "*") {
                hit = true;
                break;
            }
            if (seen.count(x)) continue;
            seen.insert(x);
            auto it = gi.find(x);
            if (it != gi.end())
                for (auto& y : graph[it->second].second) todo.push_back(y);
            for (auto* ci : order_)
                if (ci->base == x && !seen.count(ci->qname)) todo.push_back(ci->qname);
        }
        if (hit) result.push_back(info(q)->cname);
    }
    std::sort(result.begin(), result.end());
    return result;
}

}  // namespace jm
}  // namespace cpp17
