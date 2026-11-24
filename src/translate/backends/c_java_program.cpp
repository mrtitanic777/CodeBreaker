// translate/backends/c_java_program.cpp -- the program of the C back end for Java: the names and C types of its classes, their
// structs, function tables and interface tables, and the whole translation unit (see c_java.hpp).

#include <algorithm>
#include <cctype>
#include <format>
#include <map>
#include <set>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "translate/backends/c_java_internal.hpp"
#include "translate/desugar.hpp"

namespace cb::translate::backends::c_java::detail {

namespace {

// `{a}` ... with the Object receiver first: the parameter list of a table slot.
std::vector<std::string> with_self(const std::vector<std::string>& ps) {
    std::vector<std::string> all{std::string(kObj)};
    all.insert(all.end(), ps.begin(), ps.end());
    return all;
}

// The parameter list of a virtual-call helper and its argument list: `jobject* o, T a0, ...` and `o, a0, ...`.
void helper_params(const std::vector<std::string>& ps, std::string& params, std::string& args) {
    std::vector<std::string> a{"o"}, p{"jobject* o"};
    for (std::size_t i = 0; i < ps.size(); ++i) {
        a.push_back(std::format("a{}", i));
        p.push_back(std::format("{} a{}", ps[i], i));
    }
    params = join_commas(p);
    args = join_commas(a);
}

// The name Java gives the class at run time: a member class is Outer$Inner.
std::string class_text_name(const ClassInfo& cls) {
    if (cls.outer == nullptr || cls.local || cls.anon || !cls.qname.starts_with(cls.outer->qname + ".")) return std::string(text::trim(cls.qname, "<>"));
    return class_text_name(*cls.outer) + "$" + cls.qname.substr(cls.outer->qname.size() + 1);
}

bool names_object_method(const std::optional<std::string>& name) {
    return name && (*name == "toString" || *name == "equals" || *name == "hashCode");
}

}  // namespace

// ---------------------------------------------------------------------------------------------- the program
Program::Program(Arena& arena, std::span<ir::Node* const> units, std::optional<std::string> main) : main_(std::move(main)) {
    desugar::run(arena, units);
    model_ = std::make_unique<jmodel::Model>(units);
    model_->name_anonymous();
    // a method name the program declares with more than one parameter list is written with its parameter codes
    std::map<std::string, std::set<std::string>> signatures;
    std::map<std::string, std::map<std::string, std::set<std::string>>> exact;   // name: its codes: the parameter classes behind them
    for (const auto& [qname, cls] : model_->classes()) {
        by_cname_.emplace(cls->cname, cls);
        for (const ir::Node* x : cls->members()) {
            if (const auto* f = ir::get<ir::FieldDecl>(x); f != nullptr && f->sym && f->sym->is_static) static_names_.insert(f->sym->name);
            if (const auto* e = ir::get<ir::EnumConst>(x); e != nullptr && e->sym) static_names_.insert(e->sym->name);
        }
        for (const ir::Node* mn : cls->methods()) {
            const ir::MethodDecl& m = method_of(mn);
            if (m.kind != "method") continue;
            std::vector<std::string> codes;
            for (const ir::Node* pn : m.params) codes.push_back(type_code(jmodel::erase(param_of(pn).type)));
            signatures[m.name.value_or("")].insert(join_commas(codes));
            std::string classes;
            for (const ir::Node* pn : m.params) classes += exact_code(jmodel::erase(param_of(pn).type)) + ",";
            exact[m.name.value_or("")][join_commas(codes)].insert(std::move(classes));
        }
    }
    for (const auto& [name, sigs] : signatures) {
        if (sigs.size() > 1) overloaded_.insert(name);
    }
    for (const auto& [name, by_code] : exact) {   // foo(Item) and foo(Block) have one code: those are told apart by their classes
        if (std::any_of(by_code.begin(), by_code.end(), [](const auto& entry) { return entry.second.size() > 1; })) {
            overloaded_.insert(name);
            by_class_.insert(name);
        }
    }
}

// A parameter's code, naming its class where the code alone says only "an object".
std::string Program::exact_code(const ir::TypeP& type) const {
    std::string code = type_code(type);
    ir::TypeP inner = type;
    while (inner && kind_of(inner) == "array") inner = elem_of(inner);
    if ((code == "O" || code == "OA") && inner && kind_of(inner) == "class") code += ident(after_dot(class_name(inner)));
    return code;
}

// ---- names and types
std::string Program::cname(std::string_view qname) const {
    const ClassInfo* cls = model_->info(qname);
    if (cls == nullptr) throw Decline("not a class of the program: " + std::string(qname));
    return cls->cname;
}

std::string Program::mname(std::string_view name, std::span<const ir::TypeP> params) const {
    std::string base = ident(name);
    if (overloaded_.contains(std::string(name))) {
        base += '_';
        const bool classes = by_class_.contains(std::string(name));
        for (const ir::TypeP& p : params) base += classes ? exact_code(jmodel::erase(p)) : type_code(jmodel::erase(p));
    } else if (name == "init" || name == "new" || name == "finit" || name == "clinit" || static_names_.contains(std::string(name))) {   // Java keeps fields and methods apart, and a method may be named as the functions C makes for a class
        base += "_m";
    }
    return base;
}

std::string Program::ct(const ir::TypeP& type) const {
    if (!type) return "void";
    const std::string_view kind = type->kind();
    if (const std::string_view prim = prim_ctype(kind); !prim.empty()) return std::string(prim);
    if (kind == "null") return "nullptr_t";
    if (kind == "typevar" || kind == "wildcard" || kind == "any") return std::string(kObj);
    if (kind == "array") return std::string(kArr);
    if (kind == "class") {
        const std::string_view name = class_name(type);
        if (name == "java.lang.String") return std::string(kStr);
        if (name == "java.lang.StringBuilder" || name == "java.lang.StringBuffer") return std::string(kSb);
        if (const ClassInfo* cls = model_->info(name)) return cls->kind == "interface" ? std::string(kObj) : cls->cname + "*";
        if (name == "java.io.PrintStream") return "void*";
        return std::string(kObj);
    }
    throw Decline("type " + ir::to_text(*type));
}

std::string Program::class_ref(std::string_view qname) const {
    if (const ClassInfo* cls = model_->info(qname)) return std::format("&{}_class", cls->cname);
    if (qname == "java.lang.String") return "&jrt_String_class";
    const std::optional<std::string_view> name = jmodel::runtime_name(qname);
    if ((!name || *name == "Object") && qname != jmodel::kObject) {
        if (qname == "java.lang.CharSequence") return "&jrt_CharSequence_class";
        throw Decline("runtime class " + std::string(qname));
    }
    if (qname == "java.util.ArrayList" || qname == "java.util.List" || qname == "java.util.LinkedList") return "&jrt_ArrayList_class";
    if (qname == "java.util.HashMap" || qname == "java.util.Map" || qname == "java.util.LinkedHashMap" || qname == "java.util.TreeMap")
        return "&jrt_HashMap_class";
    if (qname == "java.util.Map.Entry") return "&jrt_node_class";
    return std::format("&jrt_{}_class", name.value_or("Object"));
}

Sig Program::sig(const ClassInfo& cls, const ir::Node* method) const {
    const ir::MethodDecl& m = method_of(method);
    const std::optional<jmodel::Override> root = model_->overrides(cls, method);
    std::vector<ir::TypeP> params;
    if (root) {
        params = root->params;
    } else {
        for (const ir::Node* pn : m.params) params.push_back(param_of(pn).type);
    }
    const ir::TypeP& ret = root ? root->result : m.ret;
    Sig out;
    for (const ir::TypeP& p : params) {
        out.ps.push_back(ct(jmodel::erase(p)));
        out.eps.push_back(jmodel::erase(p));
    }
    out.r = m.kind == "method" ? ct(jmodel::erase(ret)) : "void";
    return out;
}

std::optional<std::string> Program::base_struct(const ClassInfo& cls) const {
    if (cls.kind == "enum") return "jrt_Enum";
    if (cls.base && !cls.base->empty() && *cls.base != jmodel::kObject && *cls.base != "java.lang.Number") {   // (a Number is held as an Object)
        if (const ClassInfo* b = model_->info(*cls.base)) return b->cname;
        if (is_library_throwable(*cls.base)) return "jrt_Throwable";
        if (*cls.base == "java.lang.Enum") return "jrt_Enum";
        if (const auto runtime = runtime_collection_base(*cls.base)) {   // its object begins with the runtime's; what the runtime calls through its own tables it cannot see overridden
            for (const ir::Node* mn : cls.methods()) {
                const ir::MethodDecl& m = method_of(mn);
                if (m.kind == "method" && !is_static(m) && !is_object_method(m)) throw Decline("a class overriding a method of the runtime class " + *cls.base + ": " + m.name.value_or(""));
            }
            return std::string(runtime->cstruct);
        }
        throw Decline("extends the runtime class " + *cls.base);
    }
    return std::nullopt;
}

std::string Program::base_class_ref(const ClassInfo& cls) const {
    if (cls.kind == "enum") return "&jrt_Enum_class";
    if (cls.base && !cls.base->empty() && *cls.base != jmodel::kObject && *cls.base != "java.lang.Number") return class_ref(*cls.base);
    return "&jrt_Object_class";
}

bool Program::is_throwable(const ClassInfo& cls) const {
    std::optional<std::string> q = cls.base;
    while (q && !q->empty()) {
        if (is_library_throwable(*q)) return true;
        const ClassInfo* b = model_->info(*q);
        q = b != nullptr ? b->base : std::nullopt;
    }
    return false;
}

std::string Program::lit(std::string_view text) {
    const std::string key(text);
    if (!lits_.contains(key)) lits_.insert_or_assign(key, std::format("lit_{}", lits_.size() + 1));
    lit_units_[lits_.at(key)].insert(unit_);
    const Utf16Literal u = utf16_literal(text);
    return std::format("jrt_lit(&{}, {}, {})", lits_.at(key), u.text, u.length);
}

bool Program::needs_clinit(const ClassInfo& cls) const {
    if (cls.base) {   // making or using a class initialises the classes it extends first
        if (const ClassInfo* base = model_->info(*cls.base); base != nullptr && needs_clinit(*base)) return true;
    }
    for (const ir::Node* x : cls.members()) {
        if (const auto* m = ir::get<ir::MethodDecl>(x); m != nullptr && m->kind == "static_init") return true;
        if (const auto* f = ir::get<ir::FieldDecl>(x); f != nullptr && f->sym && f->sym->is_static && f->init != nullptr &&
                                                      !jmodel::constant_init(*f))
            return true;
        if (ir::is<ir::EnumConst>(x)) return true;
    }
    return false;
}

ClassInfo* Program::class_of_ctype(std::string_view ctype) const {
    if (!ctype.ends_with('*')) return nullptr;
    const auto it = by_cname_.find(std::string(ctype.substr(0, ctype.size() - 1)));
    return it == by_cname_.end() ? nullptr : it->second;
}

void Program::touch(std::string_view ctype) {
    if (const ClassInfo* cls = class_of_ctype(ctype)) used_[unit_].insert(cls);
}

bool Program::has_ctor(const ClassInfo& cls) const {
    for (const ir::Node* m : cls.methods()) {
        if (method_of(m).kind == "ctor") return true;
    }
    return false;
}

// ---- the table of virtual slots
const std::vector<Slot>& Program::slots(const ClassInfo& cls) {
    if (const auto cached = slots_.find(&cls); cached != slots_.end()) return cached->second;
    std::vector<Slot> out;
    const ClassInfo* base = cls.base ? model_->info(*cls.base) : nullptr;
    if (base != nullptr && base->kind != "interface") {
        out = slots(*base);   // a copy of each
    } else if (cls.kind != "interface" && is_throwable(cls)) {
        out.push_back(Slot{"getMessage", {}, std::string(kStr), "jrt_Throwable_getMessage"});
    }
    for (const ir::Node* mn : cls.methods()) {
        const ir::MethodDecl& m = method_of(mn);
        if (m.kind != "method" || is_static(m) || is_private(m)) continue;
        if (is_object_method(m)) continue;   // Object's slots, in the jobject_vt part
        const Sig s = sig(cls, mn);
        const std::string name = mname(m.name.value_or(""), s.eps);
        const std::string impl = has_body(m) ? std::format("{}_{}", cls.cname, name) : "0";
        auto found = std::find_if(out.begin(), out.end(), [&](const Slot& x) { return x.name == name; });
        if (found != out.end()) {
            if (has_body(m)) found->impl = impl;
        } else {
            out.push_back(Slot{name, s.ps, s.r, impl});
        }
    }
    return slots_.emplace(&cls, std::move(out)).first->second;
}

std::array<std::string, 3> Program::object_slots(const ClassInfo& cls) const {
    std::array<std::string, 3> impl = {"jrt_Object_toString", "jrt_Object_equals", "jrt_Object_hashCode"};   // toString equals hashCode
    std::vector<const ClassInfo*> chain;
    std::optional<std::string> q = cls.qname;
    while (q && !q->empty()) {
        const ClassInfo* c = model_->info(*q);
        if (c == nullptr) {
            if (is_library_throwable(*q)) impl[0] = "jrt_Throwable_toString";
            if (const auto runtime = runtime_collection_base(*q)) {
                for (std::size_t i = 0; i < 3; ++i) impl[i] = std::format("jrt_{}_{}", runtime->objects, i == 0 ? "toString" : i == 1 ? "equals" : "hashCode");
            }
            if (*q == "java.lang.Enum") impl[0] = "jrt_Enum_toString";
            break;
        }
        chain.push_back(c);
        q = c->kind != "enum" ? c->base : std::optional<std::string>("java.lang.Enum");
    }
    for (auto it = chain.rbegin(); it != chain.rend(); ++it) {   // the most derived override wins
        const ClassInfo& c = **it;
        for (const ir::Node* mn : c.methods()) {
            const ir::MethodDecl& m = method_of(mn);
            if (!is_object_method(m) || !has_body(m) || m.kind != "method") continue;
            const std::size_t slot = *m.name == "toString" ? 0 : *m.name == "equals" ? 1 : 2;
            impl[slot] = std::format("{}_{}", c.cname, *m.name);
        }
    }
    return impl;
}

std::vector<IMethod> Program::iface_methods(std::string_view qname) const {
    const ClassInfo* cls = model_->info(qname);
    if (cls == nullptr) {
        if (const jmodel::Signature* sam = jmodel::library_functional(qname)) {
            std::vector<std::string> cps;
            for (const ir::TypeP& p : sam->params) cps.push_back(ct(jmodel::erase(p)));
            return {IMethod{sam->name, cps, ct(jmodel::erase(sam->result)), sam->name, sam->params}};
        }
        if (qname == "java.lang.Iterable") return {IMethod{"iterator", {}, std::string(kObj), "iterator", {}}};
        if (qname == "java.util.Iterator")
            return {IMethod{"hasNext", {}, "bool", "hasNext", {}}, IMethod{"next", {}, std::string(kObj), "next", {}}};
        if (qname == "java.lang.Comparable")
            return {IMethod{"compareTo", {std::string(kObj)}, "int32_t", "compareTo", {ir::typevar_type("T")}}};
        if (qname == "java.lang.CharSequence" || qname == "java.lang.AutoCloseable" || qname == "java.io.Closeable") return {};
        throw Decline("interface " + std::string(qname));
    }
    std::vector<IMethod> out;
    const auto known = [&](const std::string& n) {
        return std::any_of(out.begin(), out.end(), [&](const IMethod& y) { return y.name == n; });
    };
    for (const std::string& q : cls->ifaces) {
        for (IMethod& x : iface_methods(q)) {
            if (!known(x.name)) out.push_back(std::move(x));
        }
    }
    for (const ir::Node* mn : cls->methods()) {
        const ir::MethodDecl& m = method_of(mn);
        if (m.kind != "method" || is_static(m)) continue;
        if (names_object_method(m.name)) continue;
        const Sig s = sig(*cls, mn);
        const std::string name = mname(m.name.value_or(""), s.eps);
        if (!known(name)) out.push_back(IMethod{name, s.ps, s.r, m.name.value_or(""), s.eps});
    }
    return out;
}

std::vector<std::string> Program::all_ifaces(const ClassInfo& cls) const {
    std::vector<std::string> out;
    const std::vector<std::string> ancestors = model_->ancestors(cls.qname);
    for (std::size_t i = 1; i < ancestors.size(); ++i) {
        const std::string& q = ancestors[i];
        if (!model_->is_interface(q) || std::find(out.begin(), out.end(), q) != out.end()) continue;
        if (model_->info(q) != nullptr || jmodel::library_functional(q) != nullptr || q == "java.lang.Iterable" ||
            q == "java.util.Iterator" || q == "java.lang.Comparable")
            out.push_back(q);
    }
    return out;
}

std::optional<std::string> Program::find_impl(const ClassInfo& cls, std::string_view java_name, std::size_t parameters, const std::string* slot) const {
    if (slot != nullptr) {
        if (auto exact = find_impl_of(cls, java_name, parameters, slot)) return exact;
    }
    return find_impl_of(cls, java_name, parameters, nullptr);
}

std::optional<std::string> Program::find_impl_of(const ClassInfo& cls, std::string_view java_name, std::size_t parameters, const std::string* slot) const {
    const auto implementing = [&](const ClassInfo& c) -> std::optional<std::string> {
        for (const ir::Node* mn : c.methods()) {
            const ir::MethodDecl& m = method_of(mn);
            if (m.kind == "method" && m.name == java_name && has_body(m) && m.params.size() == parameters && !is_static(m)) {
                const Sig s = sig(c, mn);
                const std::string name = mname(*m.name, s.eps);
                if (slot == nullptr || name == *slot) return std::format("{}_{}", c.cname, name);
            }
        }
        return std::nullopt;
    };
    std::optional<std::string> q = cls.qname;
    while (q && !q->empty()) {
        const ClassInfo* c = model_->info(*q);
        if (c == nullptr) break;
        if (auto found = implementing(*c)) return found;
        q = c->base;
    }
    for (const std::string& qa : model_->ancestors(cls.qname)) {   // a default method of an interface
        const ClassInfo* c = model_->info(qa);
        if (c == nullptr || c->kind != "interface") continue;
        if (auto found = implementing(*c)) return found;
    }
    return std::nullopt;
}

// The interface tables of a lambda: its function in the one abstract slot, each default method in its own, for the interface and those it extends.
Lines Program::lambda_tables(const std::string& name, const ClassInfo& iface, std::string_view java_name, std::size_t parameters) const {
    Lines out;
    std::vector<std::string> all{iface.qname}, itabs;
    for (std::string& q : all_ifaces(iface)) all.push_back(std::move(q));
    int number = 0;
    for (const std::string& q : all) {
        std::string impls;
        for (const IMethod& x : iface_methods(q)) {
            std::string impl = name + "_fn";
            if (x.java != java_name || x.eps.size() != parameters) {
                std::optional<std::string> found = find_impl(iface, x.java, x.eps.size(), &x.name);
                if (!found) throw Decline(std::format("a lambda of {} leaves {} without a body", iface.qname, x.java));
                impl = std::move(*found);
            }
            impls += std::format(", ({} (*)({})){}", x.r, join_commas(with_self(x.ps)), impl);
        }
        const ClassInfo* qi = model_->info(q);
        const std::string vt = qi != nullptr ? std::format("struct {}_vt", qi->cname) : std::format("jrt_{}_vt", after_dot(q));
        const std::string table = std::format("{}_vt{}", name, number++);
        out.push_back(std::format("static const {} {} = {{{{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}}{}}};", vt, table, impls));
        itabs.push_back(std::format("{{{}, &{}}}", class_ref(q), table));
    }
    out.push_back(std::format("static const jitab {}_itabs[] = {{{}}};", name, join_commas(itabs)));
    return out;
}

// ---- constructors
std::string Program::ctor_name(const ClassInfo& cls, const ir::Node* ctor) const {
    std::size_t n = 0;
    for (const ir::Node* x : cls.methods()) n += method_of(x).kind == "ctor";
    if (n <= 1) return cls.cname + "_init";
    CB_ASSERT(ctor != nullptr, "a class with several constructors names the one it means");
    const auto codes_of = [&](const ir::Node* c, bool classes) {
        std::string text;
        for (const ir::Node* pn : method_of(c).params) text += classes ? exact_code(jmodel::erase(param_of(pn).type)) : type_code(jmodel::erase(param_of(pn).type));
        return text;
    };
    bool classes = false;   // (two constructors with one code are told apart by their parameters' classes)
    for (const ir::Node* x : cls.methods()) {
        classes = classes || (x != ctor && method_of(x).kind == "ctor" && method_of(x).params.size() == method_of(ctor).params.size() && codes_of(x, false) == codes_of(ctor, false));
    }
    std::string codes = codes_of(ctor, classes);
    return std::format("{}_init_{}", cls.cname, codes.empty() ? "V" : codes);
}

std::vector<std::string> Program::ctor_params(const ClassInfo& cls, const ir::MethodDecl& ctor) const {
    std::vector<std::string> ps{cls.cname + "* self"};
    if (cls.kind == "enum") {
        ps.emplace_back("jstring* name_in");
        ps.emplace_back("int32_t ordinal_in");
    }
    for (const ir::Node* pn : ctor.params) {
        const ir::Param& p = param_of(pn);
        ps.push_back(std::format("{} {}", ct(p.type), ident(need_sym(p.sym, "a parameter").name)));
    }
    return ps;
}

std::vector<std::string> Program::new_params(const ClassInfo& cls, const ir::MethodDecl* ctor) const {
    std::vector<std::string> ps;
    if (cls.needs_outer) ps.push_back(std::format("{} outer_in", ct(ir::class_type(cls.outer->qname))));
    for (const jmodel::Capture& c : cls.captures) ps.push_back(std::format("{} cap_{}", ct(c.type), ident(c.name)));
    if (cls.kind == "enum") {
        ps.emplace_back("jstring* name_in");
        ps.emplace_back("int32_t ordinal_in");
    }
    if (ctor != nullptr) {
        for (const ir::Node* pn : ctor->params) {
            const ir::Param& p = param_of(pn);
            ps.push_back(std::format("{} {}", ct(p.type), ident(need_sym(p.sym, "a parameter").name)));
        }
    }
    return ps;
}

// ---- the program
std::vector<ClassInfo*> Program::ordered() const {
    // supertypes before their subclasses; classes in the order of their lines, ties by C name
    std::vector<ClassInfo*> out;
    std::unordered_set<std::string> seen;
    std::function<void(ClassInfo*)> visit = [&](ClassInfo* cls) {
        if (!seen.insert(cls->qname).second) return;
        std::vector<std::string> supers;
        if (cls->base && !cls->base->empty()) supers.push_back(*cls->base);
        supers.insert(supers.end(), cls->ifaces.begin(), cls->ifaces.end());
        for (const std::string& q : supers) {
            if (ClassInfo* b = model_->info(q)) visit(b);
        }
        out.push_back(cls);
    };
    std::vector<ClassInfo*> all;
    for (const auto& [qname, cls] : model_->classes()) all.push_back(cls);
    std::stable_sort(all.begin(), all.end(), [](const ClassInfo* a, const ClassInfo* b) {
        if (a->node->line != b->node->line) return a->node->line < b->node->line;
        return a->cname < b->cname;
    });
    for (ClassInfo* cls : all) visit(cls);
    return out;
}

// The class a file is named for: the outermost one a class sits in (nested, local and anonymous classes stay with it, as in Java).
static const ClassInfo* top_of(const ClassInfo* cls) {
    while (cls->outer != nullptr) cls = cls->outer;
    return cls;
}

// The classes whose C names a piece of C text mentions: a name is a class's own, or one that begins with it up to an underscore
// (`Foo_class`, `Foo_bar_v`, `struct Foo_vt`).
static void mentioned(std::string_view text, const std::unordered_map<std::string_view, const ClassInfo*>& names, std::set<const ClassInfo*>& out) {
    const auto word = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; };
    for (std::size_t i = 0; i < text.size();) {
        if (!word(text[i])) {
            ++i;
            continue;
        }
        std::size_t end = i;
        while (end < text.size() && word(text[end])) ++end;
        const std::string_view token = text.substr(i, end - i);
        for (std::size_t cut = 1; cut <= token.size(); ++cut) {
            if (cut != token.size() && token[cut] != '_') continue;
            if (const auto found = names.find(token.substr(0, cut)); found != names.end()) out.insert(found->second);
        }
        i = end;
    }
}

// `net/minecraft/Foo` -> `NET_MINECRAFT_FOO`
static std::string include_guard(std::string_view path) {
    std::string guard;
    for (const char c : path) guard += std::isalnum(static_cast<unsigned char>(c)) != 0 ? static_cast<char>(std::toupper(static_cast<unsigned char>(c))) : '_';
    return guard;
}

namespace {

// What one class contributes to the files it sits in.
struct Piece {
    Lines header;    // structs, table types, declarations of its static data and functions, the virtual-call helpers
    Lines data;      // its static data
    Lines lambdas;   // the closure records of its lambdas
    Lines defs;      // its functions
    Lines tables;    // its function table, interface tables and class object
};

// One class file: Foo.h and Foo.c for the top-level class Foo and the classes nested in it.
struct Unit {
    const ClassInfo* top = nullptr;
    std::string stem;                     // `Foo`
    std::string dir;                      // `net/minecraft/`, or empty
    std::vector<std::size_t> members;     // indexes into the program's class order
    std::set<std::size_t> bases;          // the other units whose structs these classes embed
    std::string path() const { return dir + stem; }
};

}  // namespace

std::vector<File> Program::write() {
    const std::vector<ClassInfo*> order = ordered();
    std::vector<Piece> pieces(order.size());
    std::vector<Unit> units;
    std::unordered_map<const ClassInfo*, std::size_t> unit_of_top, unit_of;   // unit_of: class -> its unit
    std::vector<std::size_t> unit_at(order.size());
    for (std::size_t i = 0; i < order.size(); ++i) {
        const ClassInfo* top = top_of(order[i]);
        auto [at, fresh] = unit_of_top.emplace(top, units.size());
        if (fresh) {
            Unit unit;
            unit.top = top;
            unit.stem = top->cname;
            unit.dir = text::replace_all(top->package, ".", "/");
            if (!unit.dir.empty()) unit.dir += '/';
            units.push_back(std::move(unit));
        }
        unit_at[i] = at->second;
        unit_of[order[i]] = at->second;
        units[at->second].members.push_back(i);
    }
    const auto move_tail = [](Lines& from, std::size_t keep) {
        Lines tail(std::make_move_iterator(from.begin() + static_cast<std::ptrdiff_t>(keep)), std::make_move_iterator(from.end()));
        from.resize(keep);
        return tail;
    };
    // the phases run over all classes in turn (what a program declines is found in the order it always was)
    for (std::size_t i = 0; i < order.size(); ++i) {
        unit_ = unit_at[i];
        pieces[i].header = struct_of(*order[i], pieces[i].data);
    }
    for (std::size_t i = 0; i < order.size(); ++i) {
        unit_ = unit_at[i];
        const std::size_t before = vfuncs.size();
        const Lines protos = prototypes(*order[i]);
        pieces[i].header.push_back(std::format("extern const jclass {}_class;", order[i]->cname));
        pieces[i].header.insert(pieces[i].header.end(), protos.begin(), protos.end());
        const Lines helpers = move_tail(vfuncs, before);
        pieces[i].header.insert(pieces[i].header.end(), helpers.begin(), helpers.end());
        pieces[i].header.emplace_back("");
    }
    for (std::size_t i = 0; i < order.size(); ++i) {
        unit_ = unit_at[i];
        const std::size_t before = lambdas.size();
        pieces[i].defs = definitions(*order[i]);
        pieces[i].lambdas = move_tail(lambdas, before);
    }
    for (std::size_t i = 0; i < order.size(); ++i) {
        unit_ = unit_at[i];
        pieces[i].tables = tables(*order[i]);
    }

    // the struct a class embeds is in the header of its base's file
    for (std::size_t i = 0; i < order.size(); ++i) {
        const ClassInfo& cls = *order[i];
        if (cls.kind == "interface" || cls.kind == "enum" || !cls.base) continue;
        const ClassInfo* base = model_->info(*cls.base);
        if (base == nullptr) continue;
        if (const std::size_t from = unit_at[i], to = unit_of.at(base); from != to) units[from].bases.insert(to);
    }
    {   // one file's header cannot wait on another's that waits on it
        std::vector<int> state(units.size(), 0);
        std::function<void(std::size_t)> visit = [&](std::size_t u) {
            if (state[u] == 2) return;
            if (state[u] == 1) throw Decline("the classes of " + units[u].stem + " extend classes that extend back into them");
            state[u] = 1;
            for (const std::size_t b : units[u].bases) visit(b);
            state[u] = 2;
        };
        for (std::size_t u = 0; u < units.size(); ++u) visit(u);
    }

    std::unordered_map<std::string_view, const ClassInfo*> names;
    for (const ClassInfo* cls : order) names.emplace(cls->cname, cls);
    const auto include = [](std::string_view path) { return std::format("#include \"{}.h\"", path); };

    // string literals: one used by a single file is that file's own, the others are shared
    std::vector<Lines> own_literals(units.size());
    Lines shared_literals, shared_externs;
    for (const auto& entry : lits_) {
        const std::string& slot = entry.second;
        const std::set<std::size_t>& users = lit_units_[slot];
        if (users.size() == 1) {
            own_literals[*users.begin()].push_back(std::format("static jstring* {};", slot));
        } else {
            shared_externs.push_back(std::format("extern jstring* {};", slot));
            shared_literals.push_back(std::format("jstring* {};", slot));
        }
    }

    std::vector<File> classes;
    std::string all_text;   // (to find what the files ask of the runtime's helpers)
    for (std::size_t u = 0; u < units.size(); ++u) {
        const Unit& unit = units[u];
        const std::string guard = include_guard(unit.path()) + "_H";
        const std::string nested = unit.members.size() > 1 ? " and the classes nested in it" : "";
        Lines header = {std::format("/* {}.h -- {}{}: its structs and function tables, and what the other files of the program call. */", unit.path(), unit.stem, nested),
                        "#ifndef " + guard, "#define " + guard, "", "#include \"jprogram.h\""};
        for (const std::size_t b : unit.bases) header.push_back(include(units[b].path()));
        header.emplace_back("");
        for (const std::size_t i : unit.members) header.insert(header.end(), pieces[i].header.begin(), pieces[i].header.end());
        header.emplace_back("#endif");
        classes.push_back({unit.path() + ".h", text::join(header, "\n") + "\n"});

        Lines body = own_literals[u];
        if (!body.empty()) body.emplace_back("");
        for (const std::size_t i : unit.members) body.insert(body.end(), pieces[i].data.begin(), pieces[i].data.end());
        body.emplace_back("");
        for (const std::size_t i : unit.members) {
            body.insert(body.end(), pieces[i].lambdas.begin(), pieces[i].lambdas.end());
            body.insert(body.end(), pieces[i].defs.begin(), pieces[i].defs.end());
        }
        for (const std::size_t i : unit.members) body.insert(body.end(), pieces[i].tables.begin(), pieces[i].tables.end());
        const std::string body_text = text::join(body, "\n");
        std::set<const ClassInfo*> used;
        mentioned(body_text, names, used);
        if (const auto handled = used_.find(u); handled != used_.end()) used.insert(handled->second.begin(), handled->second.end());
        std::set<std::size_t> others;   // (the files in a fixed order, whatever the order of the classes' addresses)
        for (const ClassInfo* cls : used) {
            if (const std::size_t other = unit_of.at(cls); other != u) others.insert(other);
        }
        Lines source = {std::format("/* {}.c -- the code and static data of {}{}. */", unit.path(), unit.stem, nested), include(unit.path())};
        for (const std::size_t other : others) source.push_back(include(units[other].path()));
        source.emplace_back("");
        classes.push_back({unit.path() + ".c", text::join(source, "\n") + "\n" + body_text + "\n"});
        all_text += body_text;
    }

    // the entry point
    std::vector<const ClassInfo*> mains;
    for (const ClassInfo* cls : order) {
        for (const ir::Node* mn : cls->methods()) {
            const ir::MethodDecl& m = method_of(mn);
            if (m.name == "main" && is_static(m) && m.params.size() == 1) {
                mains.push_back(cls);
                break;
            }
        }
    }
    Lines entry = {"/* jmain.c -- the program's entry point: JRT_MAIN names the class whose main runs, else the program's own. */", "#include \"jprogram.h\""};
    std::set<std::size_t> main_units;
    for (const ClassInfo* cls : mains) main_units.insert(unit_of.at(cls));
    for (const std::size_t u : main_units) entry.push_back(include(units[u].path()));
    entry.insert(entry.end(), {"", "int main(int argc, char** argv) {"});
    if (!mains.empty()) {
        entry.emplace_back("\tconst char* which = getenv(\"JRT_MAIN\");");
        for (const ClassInfo* cls : mains) {
            entry.push_back(std::format("\tif (which && strcmp(which, \"{}\") == 0) return jrt_run_main({}_main, argc, argv);",
                                        after_dot(cls->qname), cls->cname));
        }
        entry.push_back(std::format("\treturn jrt_run_main({}_main, argc, argv);", main_ && !main_->empty() ? *main_ : mains[0]->cname));
    } else {
        entry.emplace_back("\treturn 0;");
    }
    entry.emplace_back("}");

    // what every file shares: the runtime, a typedef for each class, the string literals more than one file uses
    Lines shared = {"/* written by CodeBreaker's C back end for Java (cb/translate/backends/c_java) from",
                    "   the program's L2; it runs on jrt.h / jrt.c, the Java library in C. */",
                    "#ifndef JPROGRAM_H", "#define JPROGRAM_H", "", "#include \"jrt.h\"", ""};
    for (const ClassInfo* cls : order) {
        if (cls->kind != "interface") shared.push_back(std::format("typedef struct {} {};", cls->cname, cls->cname));
    }
    shared.emplace_back("");
    if (!shared_externs.empty()) {
        shared.insert(shared.end(), shared_externs.begin(), shared_externs.end());
        shared.emplace_back("");
    }
    Lines shared_source = {"/* jprogram.c -- what the files of the program share. */", "#include \"jprogram.h\"", ""};
    shared_source.insert(shared_source.end(), shared_literals.begin(), shared_literals.end());
    // Java's Unicode categories are a large table: only a program that asks for them carries it
    if (all_text.find("jrt_Character_getType_C(") != std::string::npos) {
        const std::string table = fs::read_text(default_runtime_dir().parent_path() / "java_char_types.inc");
        shared.emplace_back("int32_t jrt_Character_getType_C(char16_t c);");
        shared.emplace_back("");
        shared_source.push_back("static const uint16_t jrt_char_types[] = {\n" + table + "};");
        shared_source.push_back("int32_t jrt_Character_getType_C(char16_t c) {");
        shared_source.push_back("\tsize_t low = 0, high = sizeof(jrt_char_types) / sizeof(jrt_char_types[0]) / 2;");
        shared_source.push_back("\twhile (high - low > 1) {");
        shared_source.push_back("\t\tconst size_t middle = low + (high - low) / 2;");
        shared_source.push_back("\t\tif (jrt_char_types[middle * 2] <= c) low = middle; else high = middle;");
        shared_source.push_back("\t}");
        shared_source.push_back("\treturn jrt_char_types[low * 2 + 1];");
        shared_source.push_back("}");
    }
    shared.emplace_back("#endif");
    std::vector<File> files;
    files.push_back({"jprogram.h", text::join(shared, "\n") + "\n"});
    if (shared_source.size() > 3) files.push_back({"jprogram.c", text::join(shared_source, "\n") + "\n"});
    for (File& f : classes) files.push_back(std::move(f));
    files.push_back({"jmain.c", text::join(entry, "\n") + "\n"});
    return files;
}

// The class's structs and the declarations of its static data (for its header); `data` gets the definitions of that data (for its source).
Lines Program::struct_of(const ClassInfo& cls, Lines& data) {
    Lines out;
    if (cls.kind == "interface") {
        out.push_back(std::format("struct {}_vt {{", cls.cname));
        out.emplace_back("\tjobject_vt o_;");
        for (const IMethod& x : iface_methods(cls.qname)) {
            out.push_back(std::format("\t{} (*{})({});", x.r, x.name, join_commas(with_self(x.ps))));
        }
        out.emplace_back("};");
        out.emplace_back("");
        return out;
    }
    const std::optional<std::string> base = base_struct(cls);
    out.push_back(std::format("struct {} {{", cls.cname));
    out.push_back(base ? std::format("\t{} super_;", *base) : "\tjobject hdr_;");
    if (cls.needs_outer) out.push_back(std::format("\t{} outer_;", ct(ir::class_type(cls.outer->qname))));
    for (const jmodel::Capture& c : cls.captures) out.push_back(std::format("\t{} cap_{};", ct(c.type), ident(c.name)));
    for (const ir::Node* fn : cls.fields()) {
        const ir::FieldDecl& f = field_of(fn);
        if (f.sym && !f.sym->is_static) out.push_back(std::format("\t{} {};", ct(f.type), ident(f.sym->name)));
    }
    out.emplace_back("};");
    // the table type: Object's slots, then every slot of the class and its bases, in order
    out.push_back(std::format("struct {}_vt {{", cls.cname));
    out.emplace_back("\tjobject_vt o_;");
    for (const Slot& s : slots(cls)) out.push_back(std::format("\t{} (*{})({});", s.r, s.name, join_commas(with_self(s.ps))));
    out.emplace_back("};");
    for (const ir::Node* fn : cls.fields()) {
        const ir::FieldDecl& f = field_of(fn);
        if (!f.sym || !f.sym->is_static) continue;
        std::string init;
        if (jmodel::constant_init(f)) {
            init = MethodWriter(*this, &cls).constant(f);
        } else {
            init = (f.type && f.type->is_prim()) ? "0" : "NULL";
        }
        out.push_back(std::format("extern {} {}_{};", ct(f.type), cls.cname, ident(f.sym->name)));
        data.push_back(std::format("{} {}_{} = {};", ct(f.type), cls.cname, ident(f.sym->name), init));
    }
    for (const ir::Node* x : cls.members()) {
        if (const auto* ec = ir::get<ir::EnumConst>(x)) {
            const std::string name = ident(need_sym(ec->sym, "an enum constant").name);
            out.push_back(std::format("extern {}* {}_{};", cls.cname, cls.cname, name));
            data.push_back(std::format("{}* {}_{};", cls.cname, cls.cname, name));
        }
    }
    if (cls.kind == "enum") data.push_back(std::format("static jarray* {}_values_;", cls.cname));
    if (needs_clinit(cls)) data.push_back(std::format("static bool {}_inited_;", cls.cname));
    out.emplace_back("");
    return out;
}

Lines Program::prototypes(const ClassInfo& cls) {
    Lines out;
    if (cls.kind == "interface") {
        for (const ir::Node* mn : cls.methods()) {
            const ir::MethodDecl& m = method_of(mn);
            if (!has_body(m)) continue;   // static and default methods
            const Sig s = sig(cls, mn);
            std::vector<std::string> all;
            if (!is_static(m)) all.emplace_back(kObj);
            all.insert(all.end(), s.ps.begin(), s.ps.end());
            out.push_back(std::format("{} {}_{}({});", s.r, cls.cname, mname(m.name.value_or(""), s.eps), decl_params(all)));
        }
        for (const IMethod& x : iface_methods(cls.qname)) {
            std::string params, args;
            helper_params(x.ps, params, args);
            vfuncs.push_back(std::format("static inline {} {}_{}_i({}) {{ return ((const struct {}_vt*)jrt_ivt(o, &{}_class))->{}({}); }}",
                                         x.r, cls.cname, x.name, params, cls.cname, cls.cname, x.name, args));
        }
        return out;
    }
    for (const ir::Node* mn : cls.methods()) {
        const ir::MethodDecl& m = method_of(mn);
        if (m.kind == "ctor") {
            const std::string init = ctor_name(cls, mn);
            out.push_back(std::format("void {}({});", init, join_commas(ctor_params(cls, m))));
            out.push_back(std::format("{}* {}({});", cls.cname, text::replace_first(init, "_init", "_new"),
                                      decl_params(new_params(cls, &m))));
            continue;
        }
        if (!has_body(m)) continue;
        const Sig s = sig(cls, mn);
        const std::string name = std::format("{}_{}", cls.cname, mname(m.name.value_or(""), s.eps));
        if (is_static(m)) {
            out.push_back(std::format("{} {}({});", s.r, name, decl_params(s.ps)));
        } else if (is_object_method(m)) {
            const std::string_view r = *m.name == "toString" ? kStr : *m.name == "equals" ? std::string_view("bool") : std::string_view("int32_t");
            out.push_back(std::format("{} {}_{}({});", r, cls.cname, *m.name, join_commas(with_self(s.ps))));
        } else {
            out.push_back(std::format("{} {}({});", s.r, name, join_commas(with_self(s.ps))));
        }
    }
    if (!has_ctor(cls)) out.push_back(std::format("{}* {}_new({});", cls.cname, cls.cname, decl_params(new_params(cls, nullptr))));
    out.push_back(std::format("void {}_finit({}* self);", cls.cname, cls.cname));
    if (needs_clinit(cls)) out.push_back(std::format("void {}_clinit(void);", cls.cname));
    if (cls.kind == "enum") out.push_back(std::format("jarray* {}_values(void);", cls.cname));
    const ClassInfo* base = cls.base ? model_->info(*cls.base) : nullptr;
    for (const Slot& s : slots(cls)) {
        if (base != nullptr && base->kind != "interface") {
            const std::vector<Slot>& base_slots = slots(*base);
            if (std::any_of(base_slots.begin(), base_slots.end(), [&](const Slot& b) { return b.name == s.name; })) continue;   // the base's helper serves
        }
        std::string params, args;
        helper_params(s.ps, params, args);
        vfuncs.push_back(std::format("static inline {} {}_{}_v({}) {{ return ((const struct {}_vt*)jrt_vt(o))->{}({}); }}", s.r,
                                     cls.cname, s.name, params, cls.cname, s.name, args));
    }
    return out;
}

Lines Program::definitions(const ClassInfo& cls) {
    Lines out;
    const auto append = [&](Lines&& lines) { out.insert(out.end(), std::make_move_iterator(lines.begin()), std::make_move_iterator(lines.end())); };
    if (cls.kind == "interface") {
        for (const ir::Node* mn : cls.methods()) {
            if (has_body(method_of(mn))) append(MethodWriter(*this, &cls, mn).define());
        }
        return out;
    }
    for (const ir::Node* mn : cls.methods()) {
        if (has_body(method_of(mn))) append(MethodWriter(*this, &cls, mn).define());
    }
    if (!has_ctor(cls)) append(MethodWriter(*this, &cls).anon_new());
    append(MethodWriter(*this, &cls).finit());
    if (needs_clinit(cls)) append(MethodWriter(*this, &cls).clinit());
    if (cls.kind == "enum") {
        out.push_back(std::format("jarray* {}_values(void) {{", cls.cname));
        out.push_back(std::format("\t{}_clinit();", cls.cname));
        out.push_back(std::format("\treturn jrt_array_clone({}_values_);", cls.cname));
        out.emplace_back("}");
        out.emplace_back("");
    }
    return out;
}

Lines Program::tables(const ClassInfo& cls) {
    Lines out;
    if (cls.kind == "interface") {
        out.push_back(std::format("const jclass {}_class = {{\"{}\", 0, 0, 0, 0, 0, 1}};", cls.cname, class_text_name(cls)));
        out.emplace_back("");
        return out;
    }
    const std::array<std::string, 3> obj = object_slots(cls);
    const std::string objs = std::format("{{{}, {}, {}}}", obj[0], obj[1], obj[2]);
    std::string slot_text;
    for (const Slot& s : slots(cls)) slot_text += std::format(", ({} (*)({})){}", s.r, join_commas(with_self(s.ps)), s.impl);
    out.push_back(std::format("static const struct {}_vt {}_vt_ = {{{}{}}};", cls.cname, cls.cname, objs, slot_text));
    std::vector<std::string> itabs;
    std::vector<std::string> from_runtime;   // the interfaces a runtime collection the class extends already answers
    for (std::optional<std::string> up = cls.base; up && !up->empty() && *up != jmodel::kObject;) {
        const ClassInfo* b = model_->info(*up);
        if (b == nullptr) {
            if (runtime_collection_base(*up)) from_runtime = model_->ancestors(*up);
            break;
        }
        up = b->base;
    }
    for (const std::string& q : all_ifaces(cls)) {
        if (std::find(from_runtime.begin(), from_runtime.end(), q) != from_runtime.end()) continue;
        const std::vector<IMethod> ms = iface_methods(q);
        if (q == "java.lang.Comparable" && !find_impl(cls, "compareTo", 1)) continue;   // an enum's: jrt_natural compares by ordinal
        std::string impls;
        for (const IMethod& x : ms) {
            std::optional<std::string> impl = find_impl(cls, x.java, x.eps.size(), &x.name);
            const bool is_abstract = std::find(cls.decl->mods.begin(), cls.decl->mods.end(), "abstract") != cls.decl->mods.end();
            if (!impl && is_abstract) impl = "0";   // never reached: an instance is of a subclass, whose own table is found first
            if (!impl) throw Decline(std::format("{} does not implement {}.{}", cls.cname, q, x.java));
            impls += std::format(", ({} (*)({})){}", x.r, join_commas(with_self(x.ps)), *impl);
        }
        const ClassInfo* qi = model_->info(q);
        const std::string vt = qi != nullptr ? std::format("struct {}_vt", qi->cname) : std::format("jrt_{}_vt", after_dot(q));
        std::string simple(after_dot(q));
        simple = text::replace_all(text::replace_all(simple, "<", ""), ">", "");
        const std::string name = std::format("{}_as_{}", cls.cname, simple);
        out.push_back(std::format("static const {} {} = {{{}{}}};", vt, name, objs, impls));
        itabs.push_back(std::format("{{{}, &{}}}", class_ref(q), name));
    }
    if (!itabs.empty()) out.push_back(std::format("static const jitab {}_itabs[] = {{{}}};", cls.cname, join_commas(itabs)));
    out.push_back(std::format("const jclass {}_class = {{\"{}\", {}, sizeof({}), &{}_vt_, {}, 0}};", cls.cname, class_text_name(cls),
                              base_class_ref(cls), cls.cname, cls.cname,
                              !itabs.empty() ? std::format("JRT_ITABS({}_itabs)", cls.cname) : std::string("0, 0")));
    out.emplace_back("");
    return out;
}

}  // namespace cb::translate::backends::c_java::detail
