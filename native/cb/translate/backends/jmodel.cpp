#include "cb/translate/backends/jmodel.hpp"

#include <algorithm>
#include <deque>
#include <format>
#include <set>

namespace cb::translate::backends::jmodel {

namespace {

using ir::TypeP;

// ---- the library tables -------------------------------------------------------------------------------------------------

TypeP object_type() { return ir::class_type(std::string(kObject)); }

struct Library {
    std::unordered_map<std::string, std::vector<std::string>> supers;
    std::vector<std::string> throwables;
    std::vector<std::pair<std::string, Signature>> functional_list;   // in the table's order
    std::unordered_map<std::string, std::size_t> functional_index;
    std::unordered_map<std::string, std::vector<Signature>> methods;
    std::unordered_map<std::string, std::string> runtime;
    std::vector<std::string> none;
    std::vector<Signature> no_methods;

    Library() {
        const TypeP T = ir::typevar_type("T");
        const TypeP str = ir::class_type("java.lang.String");
        const TypeP obj = object_type();
        const TypeP i = ir::i32_type();
        const TypeP b = ir::bool_type();
        const TypeP v = ir::void_type();
        const TypeP i64 = ir::prim_type(ir::Prim::I64);
        const TypeP f64 = ir::prim_type(ir::Prim::F64);
        const TypeP u16 = ir::prim_type(ir::Prim::U16);

        const std::vector<std::pair<std::string, std::vector<std::string>>> supers_table = {
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
        for (const auto& [name, list] : supers_table) {
            supers[name] = list;
            if (name.ends_with("Exception") || name.ends_with("Error")) throwables.push_back(name);
        }

        auto sam = [&](const std::string& q, std::string method, std::vector<TypeP> params, TypeP result) {
            functional_index[q] = functional_list.size();
            functional_list.push_back({q, Signature{std::move(method), std::move(params), std::move(result)}});
        };
        sam("java.lang.Runnable", "run", {}, v);
        sam("java.util.function.Function", "apply", {T}, T);
        sam("java.util.function.BiFunction", "apply", {T, T}, T);
        sam("java.util.function.UnaryOperator", "apply", {T}, T);
        sam("java.util.function.BinaryOperator", "apply", {T, T}, T);
        sam("java.util.function.Supplier", "get", {}, T);
        sam("java.util.function.Consumer", "accept", {T}, v);
        sam("java.util.function.BiConsumer", "accept", {T, T}, v);
        sam("java.util.function.Predicate", "test", {T}, b);
        sam("java.util.function.BiPredicate", "test", {T, T}, b);
        sam("java.util.function.IntBinaryOperator", "applyAsInt", {i, i}, i);
        sam("java.util.function.IntUnaryOperator", "applyAsInt", {i}, i);
        sam("java.util.function.IntPredicate", "test", {i}, b);
        sam("java.util.function.IntFunction", "apply", {i}, T);
        sam("java.util.function.IntConsumer", "accept", {i}, v);
        sam("java.util.function.IntSupplier", "getAsInt", {}, i);
        sam("java.util.function.ToIntFunction", "applyAsInt", {T}, i);
        sam("java.util.function.ToLongFunction", "applyAsLong", {T}, i64);
        sam("java.util.function.ToDoubleFunction", "applyAsDouble", {T}, f64);
        sam("java.util.function.IntToDoubleFunction", "applyAsDouble", {i}, f64);
        sam("java.util.function.IntToLongFunction", "applyAsLong", {i}, i64);
        sam("java.util.function.DoubleUnaryOperator", "applyAsDouble", {f64}, f64);
        sam("java.util.function.DoubleBinaryOperator", "applyAsDouble", {f64, f64}, f64);
        sam("java.util.function.LongBinaryOperator", "applyAsLong", {i64, i64}, i64);
        sam("java.util.function.LongUnaryOperator", "applyAsLong", {i64}, i64);
        sam("java.util.Comparator", "compare", {T, T}, i);

        methods[std::string(kObject)] = {{"toString", {}, str}, {"equals", {obj}, b}, {"hashCode", {}, i}};
        methods["java.lang.Comparable"] = {{"compareTo", {T}, i}};
        methods["java.lang.Iterable"] = {{"iterator", {}, ir::class_type("java.util.Iterator")}, {"forEach", {T}, v}};
        methods["java.util.Iterator"] = {{"hasNext", {}, b}, {"next", {}, T}, {"remove", {}, v}};
        methods["java.lang.Throwable"] = {{"getMessage", {}, str}};
        methods["java.lang.CharSequence"] = {{"length", {}, i}, {"charAt", {i}, u16}};
        for (const auto& [q, s] : functional_list) methods[q].push_back(s);

        // library classes -> the runtime's class names; the first library class of a runtime name is the one it stands for
        const std::vector<std::pair<std::string, std::string>> runtime_table = {
            {"java.lang.Object", "Object"}, {"java.lang.String", "String"}, {"java.lang.CharSequence", "Object"},
            {"java.lang.Integer", "Integer"}, {"java.lang.Long", "Long"}, {"java.lang.Short", "Short"},
            {"java.lang.Byte", "Byte"}, {"java.lang.Character", "Character"}, {"java.lang.Boolean", "Boolean"},
            {"java.lang.Float", "Float"}, {"java.lang.Double", "Double"}, {"java.lang.Number", "Object"},
            {"java.lang.StringBuilder", "StringBuilder"}, {"java.lang.StringBuffer", "StringBuilder"},
            {"java.lang.AbstractStringBuilder", "StringBuilder"}, {"java.lang.Math", "Math"},
            {"java.lang.StrictMath", "Math"}, {"java.lang.System", "System"}, {"java.lang.Comparable", "Comparable"},
            {"java.lang.Iterable", "Iterable"}, {"java.lang.Runnable", "Runnable"}, {"java.lang.Enum", "Enum"},
            {"java.lang.Record", "Object"}, {"java.lang.Throwable", "Throwable"}, {"java.io.PrintStream", "PrintStream"},
            {"java.util.List", "List"}, {"java.util.ArrayList", "ArrayList"}, {"java.util.LinkedList", "LinkedList"},
            {"java.util.Collection", "Collection"}, {"java.util.Iterator", "Iterator"}, {"java.util.Map", "Map"},
            {"java.util.HashMap", "HashMap"}, {"java.util.LinkedHashMap", "LinkedHashMap"}, {"java.util.TreeMap", "TreeMap"},
            {"java.util.Map.Entry", "MapEntry"}, {"java.util.Set", "Set"}, {"java.util.HashSet", "HashSet"},
            {"java.util.LinkedHashSet", "LinkedHashSet"}, {"java.util.TreeSet", "TreeSet"}, {"java.util.Deque", "LinkedList"},
            {"java.util.Queue", "LinkedList"}, {"java.util.ArrayDeque", "ArrayDeque"}, {"java.util.Arrays", "Arrays"},
            {"java.util.Collections", "Collections"}, {"java.util.Objects", "Objects"}, {"java.util.Optional", "Optional"},
            {"java.util.OptionalInt", "OptionalInt"}, {"java.util.OptionalDouble", "OptionalDouble"},
            {"java.util.Comparator", "Comparator"}, {"java.util.stream.Stream", "Stream"},
            {"java.util.stream.IntStream", "IntStream"}, {"java.util.stream.Collectors", "Collectors"},
            {"java.util.stream.Collector", "Collector"},
        };
        for (const auto& [q, name] : runtime_table) runtime.emplace(q, name);
        auto simple = [](const std::string& q) { return q.substr(q.rfind('.') + 1); };
        for (const auto& entry : functional_list) runtime.emplace(entry.first, simple(entry.first));
        for (const auto& [q, ignored] : supers_table) {
            (void)ignored;
            if (q.ends_with("Exception") || q.ends_with("Error")) runtime.emplace(q, simple(q));
        }
    }
};

const Library& library() {
    static const Library table;
    return table;
}

// The name of a class type; none for another kind of type.
const ir::ClassType* as_class(const TypeP& t) { return t ? std::get_if<ir::ClassType>(&t->body) : nullptr; }

bool is_variable(const TypeP& t) {
    return t && (std::holds_alternative<ir::TypeVarType>(t->body) || std::holds_alternative<ir::WildcardType>(t->body));
}

// A parameter of an overridden method (declared as `super_t`) and of the overriding one (`t`).
bool matches(const TypeP& super_t, const TypeP& t) {
    if (!super_t || !t) return false;
    if (is_variable(super_t)) return t->is_ref();
    return erase(super_t) == erase(t);
}

// The class names a field type can hold a reference to.
void collect_refs(const TypeP& t, std::vector<std::string>& out) {
    if (!t) return;
    if (const auto* array = std::get_if<ir::ArrayType>(&t->body)) return collect_refs(array->elem, out);
    if (const auto* cls = as_class(t)) {
        out.push_back(cls->name);
        for (const TypeP& a : cls->args) collect_refs(a, out);
    }
    if (is_variable(t)) out.emplace_back(kObject);
}

// Library types a field of which may hold anything at all (it may hold this class again).
bool holds_anything(const std::string& q) {
    static const std::set<std::string, std::less<>> names = {
        "java.lang.Object", "java.util.List", "java.util.ArrayList", "java.util.Map", "java.util.HashMap", "java.util.Set",
        "java.util.HashSet", "java.util.function.Function", "java.util.function.Supplier", "java.lang.Runnable"};
    return names.contains(q);
}

bool has_mod(const std::vector<std::string>& mods, std::string_view mod) {
    return std::find(mods.begin(), mods.end(), mod) != mods.end();
}

bool is_static_method(const ir::MethodDecl& m) { return m.sym && m.sym->is_static; }

}  // namespace

// ---- the library --------------------------------------------------------------------------------------------------------

const std::vector<std::string>& library_supers(std::string_view qname) {
    const Library& lib = library();
    const auto it = lib.supers.find(std::string(qname));
    return it == lib.supers.end() ? lib.none : it->second;
}

const std::vector<std::string>& library_throwables() { return library().throwables; }

const Signature* library_functional(std::string_view qname) {
    const Library& lib = library();
    const auto it = lib.functional_index.find(std::string(qname));
    return it == lib.functional_index.end() ? nullptr : &lib.functional_list[it->second].second;
}

const std::vector<Signature>& library_methods(std::string_view qname) {
    const Library& lib = library();
    const auto it = lib.methods.find(std::string(qname));
    return it == lib.methods.end() ? lib.no_methods : it->second;
}

std::optional<std::string_view> runtime_name(std::string_view qname) {
    const Library& lib = library();
    const auto it = lib.runtime.find(std::string(qname));
    if (it == lib.runtime.end()) return std::nullopt;
    return std::string_view(it->second);
}

// ---- types ----------------------------------------------------------------------------------------------------------------

TypeP erase(const TypeP& type) {
    if (!type) return ir::void_type();
    if (is_variable(type) || std::holds_alternative<ir::AnyType>(type->body)) return object_type();
    if (const auto* array = std::get_if<ir::ArrayType>(&type->body)) {
        ir::ArrayType erased = *array;
        erased.elem = erase(array->elem);
        return ir::make_type(std::move(erased));
    }
    if (const auto* cls = as_class(type); cls != nullptr && !cls->args.empty()) return ir::class_type(cls->name);
    return type;
}

// ---- ClassInfo ----------------------------------------------------------------------------------------------------------

std::vector<ir::Node*> ClassInfo::methods(std::optional<std::string_view> name) const {
    std::vector<ir::Node*> out;
    for (ir::Node* m : decl->members) {
        const auto* candidate = ir::get<ir::MethodDecl>(m);
        if (candidate == nullptr || !candidate->kind || (*candidate->kind != "method" && *candidate->kind != "ctor")) continue;
        if (name && candidate->name != *name) continue;
        out.push_back(m);
    }
    return out;
}

std::vector<ir::Node*> ClassInfo::fields() const {
    std::vector<ir::Node*> out;
    for (ir::Node* m : decl->members)
        if (ir::is<ir::FieldDecl>(m)) out.push_back(m);
    return out;
}

// ---- Model: the classes -------------------------------------------------------------------------------------------------

Model::Model(std::span<ir::Node* const> units) {
    for (ir::Node* u : units) {
        const auto* unit = ir::get<ir::Unit>(u);
        if (unit == nullptr) continue;
        if (unit->package && !unit->package->empty() &&
            std::find(packages_.begin(), packages_.end(), *unit->package) == packages_.end())
            packages_.push_back(*unit->package);
    }
    for (ir::Node* u : units) {
        auto* unit = ir::get<ir::Unit>(u);
        if (unit == nullptr) continue;
        for (ir::Node* type : unit->types) add(type, nullptr, unit->package.value_or(""));
    }
    std::vector<ClassInfo*> all;
    for (const auto& entry : classes_) all.push_back(entry.second);
    for (ClassInfo* c : all) find_supers(*c);
    for (ClassInfo* c : all)
        if (c->local || c->anon) find_captures(*c);
}

ClassInfo& Model::add(ir::Node* class_decl, ClassInfo* outer, const std::string& package, bool local, bool anon,
                      ir::Node* in_method) {
    auto* decl = ir::get<ir::ClassDecl>(class_decl);
    const std::string qname = decl->qname.value_or("");
    std::string cname, key;
    if (anon || qname.empty() || local) {
        ++anonymous_;
        const std::string base = outer != nullptr ? outer->cname : "Local";
        const std::string name = decl->name && !decl->name->empty() ? *decl->name : "local";
        cname = std::format("{}_{}{}", base, anon ? "anon" : name, anonymous_);
        // a local class with a name keeps it as its key; an anonymous one is known by its C name
        key = !qname.empty() && !anon ? qname : "<" + cname + ">";
    } else {
        std::string q = qname;
        if (!package.empty() && q.starts_with(package + ".")) q = q.substr(package.size() + 1);
        std::replace(q.begin(), q.end(), '.', '_');
        std::replace(q.begin(), q.end(), '$', '_');
        cname = std::move(q);
        key = qname;
    }
    auto owned = std::make_unique<ClassInfo>();
    ClassInfo& ci = *owned;
    owned_.push_back(std::move(owned));
    ci.node = class_decl;
    ci.decl = decl;
    ci.qname = key;
    ci.cname = cname;
    ci.outer = outer;
    ci.local = local;
    ci.anon = anon;
    ci.kind = decl->kind.value_or("");
    const bool is_static = has_mod(decl->mods, "static") || ci.kind == "interface" || ci.kind == "enum" ||
                           ci.kind == "record" || ci.kind == "annotation";
    ci.inner = outer != nullptr && !is_static && !local && !anon && outer->kind != "interface";
    ci.needs_outer = ci.inner;
    ci.package = package;
    ci.method = in_method;
    if ((local || anon) && in_method != nullptr && outer != nullptr) {
        const auto* m = ir::get<ir::MethodDecl>(in_method);
        ci.needs_outer = !is_static_method(*m) && m->kind != "static_init";
    }
    classes_.insert_or_assign(key, &ci);
    by_decl_[class_decl] = &ci;
    for (ir::Node* member : decl->members) {
        if (ir::is<ir::ClassDecl>(member)) {
            add(member, &ci, package);
        } else if (const auto* constant = ir::get<ir::EnumConst>(member); constant != nullptr && constant->body != nullptr) {
            ClassInfo& sub = add(constant->body, &ci, package, false, true);
            sub.enum_const = member;
            sub.needs_outer = false;
            sub.base = ci.qname;
        } else if (ir::is<ir::MethodDecl>(member) || ir::is<ir::FieldDecl>(member)) {
            nested_in(member, ci, package);
        }
    }
    return ci;
}

void Model::nested_in(ir::Node* member, ClassInfo& cls, const std::string& package) {
    ir::Node* method = nullptr;
    std::vector<ir::Node*> stack;
    if (auto* m = ir::get<ir::MethodDecl>(member)) {
        method = member;
        stack = ir::statements(m->body);
    } else if (auto* f = ir::get<ir::FieldDecl>(member)) {
        if (f->init != nullptr) stack.push_back(f->init);
    }
    // depth first, the last child first: the order the anonymous classes are numbered in
    while (!stack.empty()) {
        ir::Node* n = stack.back();
        stack.pop_back();
        if (const auto* local = ir::get<ir::LocalClass>(n)) {
            add(local->decl, &cls, package, true, false, method);
            continue;
        }
        if (auto* created = ir::get<ir::New>(n); created != nullptr && created->body != nullptr) {
            for (ir::Node* a : created->args) stack.push_back(a);
            if (created->outer != nullptr) stack.push_back(created->outer);
            ClassInfo& sub = add(created->body, &cls, package, false, true, method);
            sub.creation = n;
            if (method == nullptr) {   // a field initialiser: an instance one has `this`
                const auto* f = ir::get<ir::FieldDecl>(member);
                sub.needs_outer = !(f->sym && f->sym->is_static);
            }
            continue;
        }
        for (ir::Node* child : ir::children(*n)) stack.push_back(child);
    }
}

void Model::find_supers(ClassInfo& ci) {
    const ir::ClassDecl& d = *ci.decl;
    if (ci.anon && ci.creation != nullptr) {
        const TypeP& t = d.extends ? d.extends : ci.creation->type;
        const auto* named = as_class(t);
        const std::string target = named != nullptr ? named->name : std::string(kObject);
        const ClassInfo* known = info(target);
        const bool library_interface =
            known == nullptr && (library_functional(target) != nullptr ||
                                 (!library_methods(target).empty() && target != kObject && target != "java.lang.Throwable"));
        if ((known != nullptr && known->kind == "interface") || library_interface) {
            ci.base = std::string(kObject);
            ci.ifaces = {target};
        } else {
            ci.base = target;
        }
        return;
    }
    if (ci.enum_const != nullptr) return;
    const auto* extended = as_class(d.extends);
    if (ci.kind == "enum") {
        ci.base = "java.lang.Enum";
    } else if (ci.kind == "record") {
        ci.base = std::string(kObject);
    } else if (extended != nullptr) {
        ci.base = extended->name;
    } else if (ci.kind != "interface") {
        ci.base = std::string(kObject);
    }
    for (const TypeP& t : d.implements)
        if (const auto* named = as_class(t)) ci.ifaces.push_back(named->name);
    if (ci.kind == "interface" && extended != nullptr) ci.ifaces.insert(ci.ifaces.begin(), extended->name);
}

void Model::find_captures(ClassInfo& ci) {
    std::set<std::string, std::less<>> declared;
    const std::vector<ir::Node*> nodes = ir::walk(ci.node);
    for (const ir::Node* n : nodes) {
        if (const auto* local = ir::get<ir::Local>(n); local != nullptr && local->sym) declared.insert(local->sym->name);
        if (const auto* param = ir::get<ir::Param>(n); param != nullptr && param->sym) declared.insert(param->sym->name);
    }
    for (const ir::Node* n : nodes) {
        const auto* name = ir::get<ir::Name>(n);
        if (name == nullptr || !name->sym || declared.contains(name->sym->name)) continue;
        const bool seen = std::any_of(ci.captures.begin(), ci.captures.end(),
                                      [&](const Capture& c) { return c.name == name->sym->name; });
        if (!seen) ci.captures.push_back({name->sym->name, n->type});
    }
}

void Model::name_anonymous() {
    for (const auto& entry : classes_) {
        ClassInfo& ci = *entry.second;
        if (!(ci.anon || ci.local)) continue;
        std::vector<ir::Node*> stack(ci.decl->members.begin(), ci.decl->members.end());
        while (!stack.empty()) {
            ir::Node* n = stack.back();
            stack.pop_back();
            if (ir::is<ir::ClassDecl>(n) && n != ci.node) continue;   // its own class does its members
            ir::visit(*n, [&](auto& k) {
                if constexpr (requires { k.sym; }) {
                    ir::Sym* s = k.sym.get();
                    if (s != nullptr && s->owner && s->owner->empty() &&
                        (s->kind == ir::SymKind::Field || s->kind == ir::SymKind::Method || s->kind == ir::SymKind::Ctor))
                        s->owner = ci.qname;
                }
            });
            if (ir::is<ir::This>(n)) {
                const auto* t = as_class(n->type);
                if (t != nullptr && t->name.empty()) n->type = ir::class_type(ci.qname);
            }
            if (const auto* m = ir::get<ir::MethodDecl>(n); m != nullptr && ir::present(m->body)) {
                for (ir::Node* s : ir::statements(m->body)) stack.push_back(s);
            } else if (const auto* f = ir::get<ir::FieldDecl>(n); f != nullptr && f->init != nullptr) {
                stack.push_back(f->init);
            } else if (const auto* created = ir::get<ir::New>(n); created != nullptr && created->body != nullptr) {
                for (ir::Node* a : created->args) stack.push_back(a);
                if (created->outer != nullptr) stack.push_back(created->outer);
            } else {
                for (ir::Node* child : ir::children(*n)) stack.push_back(child);
            }
        }
    }
}

// ---- Model: lookups -----------------------------------------------------------------------------------------------------

ClassInfo* Model::info(std::string_view qname) const {
    if (qname.empty()) return nullptr;
    const auto* found = classes_.find(std::string(qname));
    return found != nullptr ? *found : nullptr;
}

ClassInfo* Model::of_decl(const ir::Node* class_decl) const {
    const auto it = by_decl_.find(class_decl);
    return it == by_decl_.end() ? nullptr : it->second;
}

ClassInfo* Model::of_type(const TypeP& type) const {
    const auto* named = as_class(type);
    return named != nullptr ? info(named->name) : nullptr;
}

std::vector<std::string> Model::supers(std::string_view qname) const {
    if (const ClassInfo* ci = info(qname)) {
        std::vector<std::string> out;
        if (ci->base) out.push_back(*ci->base);
        out.insert(out.end(), ci->ifaces.begin(), ci->ifaces.end());
        return out;
    }
    std::vector<std::string> out = library_supers(qname);
    if (qname != kObject) out.emplace_back(kObject);
    return out;
}

std::vector<std::string> Model::ancestors(std::string_view qname) const {
    std::vector<std::string> out;
    std::deque<std::string> todo = {std::string(qname)};
    std::set<std::string, std::less<>> seen;
    while (!todo.empty()) {
        std::string q = std::move(todo.front());
        todo.pop_front();
        if (!seen.insert(q).second) continue;
        for (std::string& s : supers(q)) todo.push_back(std::move(s));
        out.push_back(std::move(q));
    }
    return out;
}

bool Model::is_sub(std::string_view a, std::string_view b) const {
    if (b == kObject) return true;
    const auto all = ancestors(a);
    return std::find(all.begin(), all.end(), b) != all.end();
}

bool Model::is_interface(std::string_view qname) const {
    if (const ClassInfo* ci = info(qname)) return ci->kind == "interface";
    static const std::set<std::string, std::less<>> interfaces = {
        "java.lang.Comparable", "java.lang.Iterable", "java.util.Iterator", "java.lang.CharSequence",
        "java.util.List", "java.util.Map", "java.util.Set", "java.util.Collection"};
    return library_functional(qname) != nullptr || interfaces.contains(qname);
}

bool Model::subclassed(std::string_view qname) const {
    return std::any_of(classes_.begin(), classes_.end(), [&](const auto& e) { return e.second->base == qname; });
}

std::optional<MethodMatch> Model::method_decl(const ir::Sym& sym) const {
    if (!sym.owner) return std::nullopt;
    const bool ctor = sym.kind != ir::SymKind::Method;
    for (const std::string& q : ancestors(*sym.owner)) {
        ClassInfo* ci = info(q);
        if (ci == nullptr) continue;
        for (ir::Node* m : ci->methods(ctor ? std::string_view("<init>") : std::string_view(sym.name))) {
            const auto* decl = ir::get<ir::MethodDecl>(m);
            if (decl->params.size() != sym.params.size()) continue;
            bool same = true;
            for (std::size_t k = 0; k < sym.params.size() && same; ++k) {
                const auto* p = ir::get<ir::Param>(decl->params[k]);
                same = erase(p != nullptr ? p->type : TypeP()) == erase(sym.params[k]);
            }
            if (same) return MethodMatch{ci, m};
        }
        if (ctor) break;
    }
    return std::nullopt;
}

std::optional<Override> Model::overrides(const ClassInfo& cls, const ir::Node* method) const {
    const auto* m = ir::get<ir::MethodDecl>(method);
    if (m == nullptr || m->kind != "method" || is_static_method(*m) || has_mod(m->mods, "private")) return std::nullopt;
    std::vector<TypeP> own;   // the types of m's parameters
    for (const ir::Node* p : m->params) {
        const auto* param = ir::get<ir::Param>(p);
        own.push_back(param != nullptr ? param->type : TypeP());
    }
    std::optional<Override> found;
    std::set<std::string, std::less<>> seen;
    std::deque<std::string> todo;
    for (std::string& s : supers(cls.qname)) todo.push_back(std::move(s));
    while (!todo.empty()) {
        const std::string q = std::move(todo.front());
        todo.pop_front();
        if (!seen.insert(q).second) continue;
        if (const ClassInfo* sci = info(q)) {
            for (const ir::Node* sn : sci->methods(m->name ? std::optional<std::string_view>(*m->name) : std::nullopt)) {
                const auto* sm = ir::get<ir::MethodDecl>(sn);
                if (sm->kind != "method" || sm->params.size() != own.size() || is_static_method(*sm) || has_mod(sm->mods, "private"))
                    continue;
                std::vector<TypeP> declared;
                bool same = true;
                for (std::size_t k = 0; k < own.size() && same; ++k) {
                    const auto* sp = ir::get<ir::Param>(sm->params[k]);
                    declared.push_back(sp != nullptr ? sp->type : TypeP());
                    same = matches(declared.back(), own[k]);
                }
                if (same) found = Override{std::move(declared), sm->ret};
            }
        } else {
            for (const Signature& sig : library_methods(q)) {
                if (!m->name || sig.name != *m->name || sig.params.size() != own.size()) continue;
                bool same = true;
                for (std::size_t k = 0; k < own.size() && same; ++k) same = matches(sig.params[k], own[k]);
                if (same) found = Override{sig.params, sig.result};
            }
        }
        for (std::string& s : supers(q)) todo.push_back(std::move(s));
    }
    return found;
}

std::optional<Signature> Model::functional(std::string_view qname) const {
    if (const Signature* lib = library_functional(qname)) return *lib;
    const ClassInfo* ci = info(qname);
    if (ci == nullptr || ci->kind != "interface") return std::nullopt;
    std::vector<Signature> abstract;
    for (const std::string& q : ancestors(qname)) {
        const ClassInfo* sci = info(q);
        if (sci == nullptr) {
            if (const Signature* lib = library_functional(q)) abstract.push_back(*lib);
            continue;
        }
        if (sci->kind != "interface") continue;
        for (const ir::Node* mn : sci->methods()) {
            const auto* m = ir::get<ir::MethodDecl>(mn);
            const std::string name = m->name.value_or("");
            if (ir::present(m->body) || is_static_method(*m) || name == "toString" || name == "equals" || name == "hashCode") continue;
            const bool known = std::any_of(abstract.begin(), abstract.end(), [&](const Signature& a) {
                return a.name == name && a.params.size() == m->params.size();
            });
            if (known) continue;
            std::vector<TypeP> params;
            for (const ir::Node* p : m->params) {
                const auto* param = ir::get<ir::Param>(p);
                params.push_back(param != nullptr ? param->type : TypeP());
            }
            abstract.push_back({name, std::move(params), m->ret});
        }
    }
    if (abstract.size() != 1) return std::nullopt;
    return std::move(abstract.front());
}

std::vector<std::string> Model::cycles() const {
    // (the graph is only searched: the result is sorted below, so its hash order never shows)
    // what each class's instance fields can reach ("*": anything at all, it may hold this class again)
    std::unordered_map<std::string, std::set<std::string>> graph;
    std::unordered_map<std::string, std::vector<std::string>> subclasses;   // base -> the keys of its subclasses
    for (const auto& [key, ci] : classes_) {
        if (ci->base) subclasses[*ci->base].push_back(key);
        std::set<std::string>& out = graph[ci->qname];
        for (const ir::Node* fn : ci->fields()) {
            const auto* f = ir::get<ir::FieldDecl>(fn);
            if (!f->sym || f->sym->is_static) continue;
            std::vector<std::string> refs;
            collect_refs(f->type, refs);
            for (const std::string& q : refs) {
                if (classes_.contains(q)) out.insert(q);
                else if (holds_anything(q)) out.insert("*");
            }
        }
        if (ci->needs_outer && ci->outer != nullptr) out.insert(ci->outer->qname);
    }
    std::vector<std::string> result;
    for (const auto& [q, edges] : graph) {
        std::set<std::string> seen;
        std::vector<std::string> todo(edges.begin(), edges.end());
        bool hit = false;
        while (!todo.empty() && !hit) {
            const std::string x = std::move(todo.back());
            todo.pop_back();
            if (x == q || x == "*") {
                hit = true;
                break;
            }
            if (!seen.insert(x).second) continue;
            if (const auto g = graph.find(x); g != graph.end()) todo.insert(todo.end(), g->second.begin(), g->second.end());
            // a field of type X may hold a subclass of X
            if (const auto s = subclasses.find(x); s != subclasses.end())
                for (const std::string& sub : s->second)
                    if (!seen.contains(sub)) todo.push_back(sub);
        }
        if (hit) result.push_back(classes_.at(q)->cname);
    }
    std::sort(result.begin(), result.end());
    return result;
}

// ---- literals ---------------------------------------------------------------------------------------------------------

std::string char_literal(std::uint32_t code) {
    if (code >= 32 && code < 127 && code != '\\' && code != '\'') return std::format("u'{}'", static_cast<char>(code));
    return std::format("(char16_t){}", code);
}

bool constant_init(const ir::FieldDecl& field) {
    const ir::Node* init = field.init;
    while (const auto* paren = ir::get<ir::Paren>(init)) init = paren->x;
    if (const auto* negated = ir::get<ir::Unary>(init); negated != nullptr && negated->op == "-" && ir::is<ir::Lit>(negated->x))
        init = negated->x;
    const auto* lit = ir::get<ir::Lit>(init);
    return lit != nullptr && !std::holds_alternative<std::monostate>(lit->value) &&
           !std::holds_alternative<std::string>(lit->value) && field.type && field.type->is_prim();
}

}  // namespace cb::translate::backends::jmodel
