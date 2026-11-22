// translate/backends/c_java_program.cpp -- the program of the C back end for Java: the names and C types of its classes, their
// structs, function tables and interface tables, and the whole translation unit (see c_java.hpp).

#include <algorithm>
#include <format>
#include <map>
#include <set>

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

std::string class_text_name(const ClassInfo& cls) { return std::string(text::trim(cls.qname, "<>")); }

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
        }
    }
    for (const auto& [name, sigs] : signatures) {
        if (sigs.size() > 1) overloaded_.insert(name);
    }
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
        for (const ir::TypeP& p : params) base += type_code(jmodel::erase(p));
    } else if (static_names_.contains(std::string(name))) {   // Java keeps fields and methods apart; C has one name for both
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
    if (cls.base && !cls.base->empty() && *cls.base != jmodel::kObject) {
        if (const ClassInfo* b = model_->info(*cls.base)) return b->cname;
        if (is_library_throwable(*cls.base)) return "jrt_Throwable";
        if (*cls.base == "java.lang.Enum") return "jrt_Enum";
        throw Decline("extends the runtime class " + *cls.base);
    }
    return std::nullopt;
}

std::string Program::base_class_ref(const ClassInfo& cls) const {
    if (cls.kind == "enum") return "&jrt_Enum_class";
    if (cls.base && !cls.base->empty() && *cls.base != jmodel::kObject) return class_ref(*cls.base);
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
    const Utf16Literal u = utf16_literal(text);
    return std::format("jrt_lit(&{}, {}, {})", lits_.at(key), u.text, u.length);
}

bool Program::needs_clinit(const ClassInfo& cls) const {
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

std::optional<std::string> Program::find_impl(const ClassInfo& cls, std::string_view java_name, std::size_t parameters) const {
    const auto implementing = [&](const ClassInfo& c) -> std::optional<std::string> {
        for (const ir::Node* mn : c.methods()) {
            const ir::MethodDecl& m = method_of(mn);
            if (m.kind == "method" && m.name == java_name && has_body(m) && m.params.size() == parameters && !is_static(m)) {
                const Sig s = sig(c, mn);
                return std::format("{}_{}", c.cname, mname(*m.name, s.eps));
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

// ---- constructors
std::string Program::ctor_name(const ClassInfo& cls, const ir::Node* ctor) const {
    std::size_t n = 0;
    for (const ir::Node* x : cls.methods()) n += method_of(x).kind == "ctor";
    if (n <= 1) return cls.cname + "_init";
    CB_ASSERT(ctor != nullptr, "a class with several constructors names the one it means");
    std::string codes;
    for (const ir::Node* pn : method_of(ctor).params) codes += type_code(jmodel::erase(param_of(pn).type));
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

std::string Program::write() {
    const std::vector<ClassInfo*> order = ordered();
    Lines structs, protos, tables_, defs;
    const auto append = [](Lines& to, Lines&& from) { to.insert(to.end(), std::make_move_iterator(from.begin()), std::make_move_iterator(from.end())); };
    for (const ClassInfo* cls : order) append(structs, struct_of(*cls));
    for (const ClassInfo* cls : order) append(protos, prototypes(*cls));
    for (const ClassInfo* cls : order) append(defs, definitions(*cls));
    for (const ClassInfo* cls : order) append(tables_, tables(*cls));
    Lines head = {"/* written by CodeBreaker's C back end for Java (cb/translate/backends/c_java) from",
                  "   the program's L2; it runs on jrt.h / jrt.c, the Java library in C. */", "#include \"jrt.h\"", ""};
    for (const ClassInfo* cls : order) {
        if (cls->kind != "interface") head.push_back(std::format("typedef struct {} {};", cls->cname, cls->cname));
    }
    for (const ClassInfo* cls : order) head.push_back(std::format("extern const jclass {}_class;", cls->cname));
    head.emplace_back("");
    Lines literals;
    for (const auto& entry : lits_) literals.push_back(std::format("static jstring* {};", entry.second));
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
    Lines tail = {"", "int main(int argc, char** argv) {"};
    if (!mains.empty()) {
        tail.emplace_back("\tconst char* which = getenv(\"JRT_MAIN\");");
        for (const ClassInfo* cls : mains) {
            tail.push_back(std::format("\tif (which && strcmp(which, \"{}\") == 0) return jrt_run_main({}_main, argc, argv);",
                                       after_dot(cls->qname), cls->cname));
        }
        tail.push_back(std::format("\treturn jrt_run_main({}_main, argc, argv);", main_ && !main_->empty() ? *main_ : mains[0]->cname));
    } else {
        tail.emplace_back("\treturn 0;");
    }
    tail.emplace_back("}");
    Lines all;
    for (const Lines* part : {&head, &structs, &literals}) all.insert(all.end(), part->begin(), part->end());
    all.emplace_back("");
    for (const Lines* part : {&protos, &vfuncs, &lambdas, &defs, &tables_, &tail}) all.insert(all.end(), part->begin(), part->end());
    return text::join(all, "\n") + "\n";
}

Lines Program::struct_of(const ClassInfo& cls) {
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
        out.push_back(std::format("static {} {}_{} = {};", ct(f.type), cls.cname, ident(f.sym->name), init));
    }
    for (const ir::Node* x : cls.members()) {
        if (const auto* ec = ir::get<ir::EnumConst>(x)) {
            out.push_back(std::format("static {}* {}_{};", cls.cname, cls.cname, ident(need_sym(ec->sym, "an enum constant").name)));
        }
    }
    if (cls.kind == "enum") out.push_back(std::format("static jarray* {}_values_;", cls.cname));
    if (needs_clinit(cls)) out.push_back(std::format("static bool {}_inited_;", cls.cname));
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
            out.push_back(std::format("static {} {}_{}({});", s.r, cls.cname, mname(m.name.value_or(""), s.eps), decl_params(all)));
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
            out.push_back(std::format("static void {}({});", init, join_commas(ctor_params(cls, m))));
            out.push_back(std::format("static {}* {}({});", cls.cname, text::replace_first(init, "_init", "_new"),
                                      decl_params(new_params(cls, &m))));
            continue;
        }
        if (!has_body(m)) continue;
        const Sig s = sig(cls, mn);
        const std::string name = std::format("{}_{}", cls.cname, mname(m.name.value_or(""), s.eps));
        if (is_static(m)) {
            out.push_back(std::format("static {} {}({});", s.r, name, decl_params(s.ps)));
        } else if (is_object_method(m)) {
            const std::string_view r = *m.name == "toString" ? kStr : *m.name == "equals" ? std::string_view("bool") : std::string_view("int32_t");
            out.push_back(std::format("static {} {}_{}({});", r, cls.cname, *m.name, join_commas(with_self(s.ps))));
        } else {
            out.push_back(std::format("static {} {}({});", s.r, name, join_commas(with_self(s.ps))));
        }
    }
    if (!has_ctor(cls)) out.push_back(std::format("static {}* {}_new({});", cls.cname, cls.cname, decl_params(new_params(cls, nullptr))));
    out.push_back(std::format("static void {}_finit({}* self);", cls.cname, cls.cname));
    if (needs_clinit(cls)) out.push_back(std::format("static void {}_clinit(void);", cls.cname));
    if (cls.kind == "enum") out.push_back(std::format("static jarray* {}_values(void);", cls.cname));
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
        out.push_back(std::format("static jarray* {}_values(void) {{", cls.cname));
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
    for (const std::string& q : all_ifaces(cls)) {
        const std::vector<IMethod> ms = iface_methods(q);
        if (q == "java.lang.Comparable" && !find_impl(cls, "compareTo", 1)) continue;   // an enum's: jrt_natural compares by ordinal
        std::string impls;
        for (const IMethod& x : ms) {
            std::optional<std::string> impl = find_impl(cls, x.java, x.eps.size());
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
