// translate/backends/rust_java_program.cpp -- the program's classes in Rust: what a class becomes, and the tables that tie a trait's methods to the
// bodies that run them (see rust_java.hpp for the model).

#include <algorithm>
#include <format>
#include <map>

#include "translate/backends/rust_java_internal.hpp"
#include "translate/desugar.hpp"

namespace cb::translate::backends::rust_java::detail {

namespace {

// The names the runtime defines: a program class of the same name would hide them.
bool names_runtime_type(std::string_view name) {
    static const std::set<std::string_view> list = {"Str",    "Ref",       "Arr",   "System",  "Math",       "StringBuilder", "Object", "Throwable", "PrintStream",
                                                     "Flow",   "Cat",       "Nn",    "Thrown",  "LibException", "JArray",      "ArrOps", "JInt",      "FArg",
                                                     "FmtArg", "ThrowableData", "Cell", "Box",    "Vec",        "String",        "Option", "Some",      "None"};
    return list.contains(name);
}

std::string_view lower_kind(std::string_view kind) { return kind; }

// A library interface whose methods the program writes as its own (a class that is Comparable has a compareTo), so that nothing of the runtime needs it.
bool is_marker_interface(const std::string& qname) {
    return qname == "java.lang.Comparable" || qname == "java.lang.AutoCloseable" || qname == "java.io.Closeable" || qname == "java.io.Serializable" ||
           qname == "java.lang.Cloneable";
}

bool is_abstract(const ClassInfo& cls) { return cls.kind == "interface" || has_mod(cls.decl->mods, "abstract"); }

// A method without a body that a class must have implemented.
bool is_abstract_method(const ClassInfo& cls, const ir::MethodDecl& m) {
    return !ir::present(m.body) && !is_static(m) && !has_mod(m.mods, "native") && (cls.kind == "interface" || has_mod(m.mods, "abstract"));
}

}  // namespace

// ---------------------------------------------------------------------------------------------------------------- the program

Program::Program(Arena& arena, std::span<ir::Node* const> units, std::string main) : main_name_(std::move(main)), units_(units.begin(), units.end()) {
    desugar::run(arena, units);
    model_ = std::make_unique<jmodel::Model>(units);
    model_->name_anonymous();
    check_supported();
    // a method name the program declares with more than one parameter list is written with its parameter codes
    std::map<std::string, std::set<std::string>> signatures;
    for (const auto& [qname, cls] : model_->classes()) {
        for (const ir::Node* mn : cls->methods()) {
            const ir::MethodDecl& m = method_of(mn);
            if (m.kind != "method") continue;
            std::vector<std::string> codes;
            for (const ir::Node* pn : m.params) codes.push_back(short_code(jmodel::erase(param_of(pn).type)));
            signatures[m.name.value_or("")].insert(join(codes));
        }
    }
    for (const char* object_method : {"toString", "hashCode"}) signatures[object_method].insert("");
    signatures["equals"].insert("object");
    for (const auto& [name, sigs] : signatures)
        if (sigs.size() > 1) overloaded_.insert(name);
    // a class with nothing but static members is a unit struct: no instances, no subclasses, no type of its own in use
    for (const auto& [qname, cls] : model_->classes()) {
        if (cls->kind != "class" || cls->base.value_or("java.lang.Object") != "java.lang.Object" || !cls->ifaces.empty() || is_abstract(*cls)) continue;
        if (model_->subclassed(qname)) continue;
        bool plain = true;
        for (const ir::Node* member : cls->members()) {
            if (const auto* f = ir::get<ir::FieldDecl>(member); f != nullptr && !(f->sym && f->sym->is_static) && !has_mod(f->mods, "static")) plain = false;
            if (const auto* m = ir::get<ir::MethodDecl>(member)) {
                if (m->kind == "method" && !is_static(*m)) plain = false;
                if (m->kind == "init") plain = false;
                if (m->kind == "ctor" && !m->params.empty()) plain = false;
            }
        }
        if (!plain) continue;
        const auto mentions = [&](const ir::TypeP& t) {
            std::vector<const ir::Type*> todo;
            if (t) todo.push_back(t.get());
            while (!todo.empty()) {
                const ir::Type* x = todo.back();
                todo.pop_back();
                if (const auto* c = std::get_if<ir::ClassType>(&x->body)) {
                    if (c->name == qname) return true;
                    for (const ir::TypeP& a : c->args) todo.push_back(a.get());
                } else if (const auto* a = std::get_if<ir::ArrayType>(&x->body)) {
                    if (a->elem) todo.push_back(a->elem.get());
                }
            }
            return false;
        };
        for (ir::Node* unit : units) {
            for_each_node(unit, [&](const ir::Node* n) {
                if (!plain || ir::is<ir::TypeName>(n)) return;
                if (const auto* nw = ir::get<ir::New>(n); nw != nullptr && nw->sym && nw->sym->owner == qname) plain = false;
                if (mentions(n->type)) plain = false;
                if (const auto* l = ir::get<ir::Local>(n); l != nullptr && mentions(l->type)) plain = false;
                if (const auto* p = ir::get<ir::Param>(n); p != nullptr && mentions(p->type)) plain = false;
                if (const auto* f = ir::get<ir::FieldDecl>(n); f != nullptr && mentions(f->type)) plain = false;
                if (const auto* m = ir::get<ir::MethodDecl>(n); m != nullptr && mentions(m->ret)) plain = false;
            });
        }
        if (plain) static_only_.insert(qname);
    }
    find_main();
}

void Program::check_supported() {
    std::vector<std::string> libraries;   // classes of the library whose source the program reads (java.io's streams ...): the runtime has no word for them
    std::set<std::string> reasons;   // (what the classes need that is not written, all of it: a program is declined with the list)
    for (const auto& [qname, cls] : model_->classes()) {
        try {
            if (qname.starts_with("java.")) {
                libraries.push_back(qname);
                continue;
            }
            if (cls->local || cls->anon) throw Decline("a local or anonymous class (" + qname + ")");
            if (cls->kind == "annotation") throw Decline("an annotation (" + qname + ")");
            for (const ir::Node* member : cls->members())
                if (const auto* ec = ir::get<ir::EnumConst>(member); ec != nullptr && ec->body != nullptr) throw Decline("an enum constant with a body of its own (" + qname + ")");
            if (cls->inner || cls->needs_outer) throw Decline("an inner class that holds its outer instance (" + qname + ")");
            if (!cls->decl->typeparams.empty() || !cls->decl->tparams.empty()) throw Decline("a generic class (" + qname + ")");
            if (names_runtime_type(cls->cname)) throw Decline("a class named like a runtime type (" + qname + ")");
            if (cls->kind != "enum" && cls->kind != "record" && cls->base && *cls->base != "java.lang.Object" && model_->info(*cls->base) == nullptr && !is_lib_throwable(*cls->base))
                throw Decline("a class extending the library class " + *cls->base);
            for (const std::string& i : cls->ifaces)
                if (model_->info(i) == nullptr && !is_marker_interface(i)) throw Decline("a class implementing the library interface " + i);
            for (const ir::Node* mn : cls->methods()) {
                const ir::MethodDecl& m = method_of(mn);
                if (!m.typeparams.empty() || !m.tparams.empty()) throw Decline("a generic method (" + m.name.value_or("") + ")");
                const std::string kind = m.kind.value_or("method");
                if (kind != "method" && kind != "ctor" && kind != "static_init" && kind != "init") throw Decline("a member of kind " + kind);
            }
            for (const ir::Node* member : cls->members())
                if (ir::is<ir::ClassDecl>(member) || ir::is<ir::Property>(member)) {
                    if (ir::is<ir::Property>(member)) throw Decline("a property");
                }
        } catch (const Decline& d) {
            reasons.insert(d.what());
        }
    }
    if (!libraries.empty()) reasons.insert("the program uses " + std::to_string(libraries.size()) + " library classes the runtime has no word for (" + libraries[0] + " ...)");
    if (!reasons.empty()) {
        std::vector<std::string> list(reasons.begin(), reasons.end());
        if (list.size() > 12) list.resize(12);
        throw Decline(join(list, "; "));
    }
}

void Program::find_main() {
    std::vector<const ClassInfo*> found;
    for (const auto& [qname, cls] : model_->classes()) {
        for (const ir::Node* mn : cls->methods("main")) {
            const ir::MethodDecl& m = method_of(mn);
            if (!is_static(m) || m.params.size() != 1) continue;
            const ir::TypeP t = param_of(m.params[0]).type;
            if (kind_of(t) == "array" && class_name(elem_of(t)) == "java.lang.String") found.push_back(cls);
        }
    }
    if (!main_name_.empty()) {
        for (const ClassInfo* c : found) {
            if (c->qname == main_name_ || c->cname == main_name_ || (c->qname.size() > main_name_.size() && c->qname.ends_with("." + main_name_))) {
                main_cls_ = c;
                return;
            }
        }
        throw Decline("no main method in the class " + main_name_);
    }
    if (found.size() != 1) throw Decline(found.empty() ? "no class has a main method" : "several classes have a main method");
    main_cls_ = found[0];
}

// ---------------------------------------------------------------------------------------------------------------- types

bool Program::is_lib_throwable(std::string_view qname) const {
    if (model_->info(qname) != nullptr) return false;
    return qname == "java.lang.Throwable" || model_->is_sub(qname, "java.lang.Throwable");
}
bool Program::is_throwable_type(std::string_view qname) const { return qname == "java.lang.Throwable" || model_->is_sub(qname, "java.lang.Throwable"); }

const ClassInfo* Program::program_class(const ir::TypeP& type) const {
    if (kind_of(type) != "class") return nullptr;
    return model_->info(class_name(type));
}

std::string Program::rtype(const ir::TypeP& type) const {
    const std::string_view k = kind_of(type);
    if (k == "bool" || k == "i8" || k == "i16" || k == "i32" || k == "i64" || k == "f32" || k == "f64" || k == "u16") return std::string(k);
    if (k == "void" || k.empty()) return "()";
    if (k == "array") return "Arr<" + rtype(elem_of(type)) + ">";
    if (k == "class") {
        const auto& c = std::get<ir::ClassType>(type->body);
        const std::string& n = c.name;
        if (n == "java.lang.String") return "Str";
        if (n == "java.lang.Object") return "Ref<dyn Object>";
        if (n == "java.lang.StringBuilder") return "Ref<StringBuilder>";
        if (n == "java.io.PrintStream") return "PrintStream";
        if (const std::string_view box = boxed_kind(type); !box.empty()) return "Option<" + std::string(box) + ">";
        if (n.starts_with("java.util.") && model_->info(n) == nullptr) {
            const auto arg = [&](std::size_t i) { return rtype(c.args.at(i)); };
            const auto need = [&](std::size_t count) {
                if (c.args.size() != count) throw Decline("the library type " + n + " without its type arguments");
            };
            if (n == "java.util.List" || n == "java.util.ArrayList" || n == "java.util.Collection") return need(1), "Ref<ArrayList<" + arg(0) + ">>";
            if (n == "java.util.Set" || n == "java.util.HashSet" || n == "java.util.LinkedHashSet") return need(1), "Ref<HashSet<" + arg(0) + ">>";
            if (n == "java.util.Map" || n == "java.util.HashMap" || n == "java.util.LinkedHashMap") return need(2), "Ref<HashMap<" + arg(0) + ", " + arg(1) + ">>";
            if (n == "java.util.Iterator") return need(1), "Ref<JIter<" + arg(0) + ">>";
            if (n == "java.util.Map.Entry") return need(2), "Ref<MapEntry<" + arg(0) + ", " + arg(1) + ">>";
        }
        if (const ClassInfo* cls = model_->info(n)) {
            if (!c.args.empty()) throw Decline("the generic type " + ir::to_text(*type));
            if (static_only(*cls)) throw Decline("a type of the class " + n + ", which has only static members");
            return "Ref<dyn " + trait_name(*cls) + ">";
        }
        if (is_lib_throwable(n)) return "Ref<dyn Throwable>";
        throw Decline("the library type " + n);
    }
    if (k == "typevar") throw Decline("a type variable");
    throw Decline("a type of kind " + std::string(k));
}

// ---------------------------------------------------------------------------------------------------------------- classes

bool Program::static_only(const ClassInfo& cls) const { return static_only_.contains(cls.qname); }
std::string Program::holder(const ClassInfo& cls) const { return static_only(cls) ? cls.cname : cls.cname + "Data"; }

const ClassInfo* Program::base_class(const ClassInfo& cls) const { return cls.base ? model_->info(*cls.base) : nullptr; }

std::vector<const ClassInfo*> Program::ancestors(const ClassInfo& cls) const {
    std::vector<const ClassInfo*> out;
    for (const std::string& q : model_->ancestors(cls.qname)) {
        const ClassInfo* c = model_->info(q);
        if (c != nullptr && c != &cls) out.push_back(c);
    }
    return out;
}

bool Program::is_concrete(const ClassInfo& cls) const { return (cls.kind == "class" || cls.kind == "enum" || cls.kind == "record") && !is_abstract(cls) && !static_only(cls); }

std::vector<const ClassInfo*> Program::subclasses_of(const ClassInfo& cls) const {
    std::vector<const ClassInfo*> out;
    for (const auto& [qname, c] : model_->classes())
        if (is_concrete(*c) && (c->qname == cls.qname || model_->is_sub(c->qname, cls.qname))) out.push_back(c);
    return out;
}

// ---------------------------------------------------------------------------------------------------------------- methods

bool Program::is_object_method(const ir::MethodDecl& m) const {
    const std::string n = m.name.value_or("");
    if (n == "toString" || n == "hashCode") return m.params.empty();
    if (n == "equals") return m.params.size() == 1 && class_name(param_of(m.params[0]).type) == "java.lang.Object";
    return false;
}

std::string Program::mname(const ir::MethodDecl& m) const {
    std::string base = member_ident(m.name.value_or(""));
    if (is_object_method(m) || m.params.empty() || !overloaded_.contains(m.name.value_or(""))) return base;
    std::vector<std::string> codes;
    for (const ir::Node* pn : m.params) codes.push_back(short_code(jmodel::erase(param_of(pn).type)));
    return base + "_" + join(codes, "_");
}

bool Program::introduces(const ClassInfo& cls, const ir::MethodDecl& m) const {
    if (is_static(m) || is_private(m) || m.kind != "method" || is_object_method(m)) return false;
    const std::string rn = mname(m);
    for (const ClassInfo* a : ancestors(cls)) {
        for (const ir::Node* other : a->methods()) {
            const ir::MethodDecl& o = method_of(other);
            if (o.kind == "method" && !is_static(o) && !is_private(o) && !is_object_method(o) && mname(o) == rn) return false;
        }
    }
    return true;
}

std::optional<MethodRef> Program::find_impl(const ClassInfo& cls, const std::string& rn) const {
    const auto has = [&](const ClassInfo& c) -> const ir::Node* {
        for (const ir::Node* mn : c.methods()) {
            const ir::MethodDecl& m = method_of(mn);
            if (m.kind == "method" && !is_static(m) && ir::present(m.body) && mname(m) == rn) return mn;
        }
        return nullptr;
    };
    for (const ClassInfo* c = &cls; c != nullptr; c = base_class(*c))
        if (const ir::Node* mn = has(*c)) return MethodRef{c, mn};
    for (const ClassInfo* a : ancestors(cls))
        if (a->kind == "interface")
            if (const ir::Node* mn = has(*a)) return MethodRef{a, mn};
    return std::nullopt;
}

std::string Program::ctor_suffix(const ClassInfo& cls, const ir::Node* ctor) const {
    std::vector<const ir::Node*> ctors;
    for (const ir::Node* mn : cls.methods())
        if (method_of(mn).kind == "ctor") ctors.push_back(mn);
    if (ctors.size() <= 1) return "";
    for (std::size_t i = 0; i < ctors.size(); ++i)
        if (ctors[i] == ctor) return "_" + std::to_string(i);
    throw Decline("a constructor of " + cls.qname + " that is not one of its own");
}

bool Program::is_const_field(const ir::FieldDecl& f) const {
    const bool is_static_field = has_mod(f.mods, "static") || (f.sym && f.sym->is_static);
    if (!is_static_field || !has_mod(f.mods, "final") || f.init == nullptr || !jmodel::constant_init(f)) return false;
    const std::string_view k = kind_of(f.type);
    return is_prim_kind(k);
}

// ---------------------------------------------------------------------------------------------------------------- writing

namespace {

std::string binary_name(const ClassInfo& cls) {
    if (cls.outer == nullptr || !cls.qname.starts_with(cls.outer->qname + ".")) return cls.qname;
    return binary_name(*cls.outer) + "$" + cls.qname.substr(cls.outer->qname.size() + 1);
}

}  // namespace

std::string Program::write() {
    std::string out;
    const auto put = [&](const Lines& lines) {
        for (const std::string& l : lines) out += l + "\n";
    };
    std::vector<Lines> classes;
    for (const auto& [qname, cls] : model_->classes()) classes.push_back(class_text(*cls));
    std::string name = main_cls_->cname;
    out += "// " + name + ".rs -- the Rust translation of the Java program whose main is " + main_cls_->qname + " (CodeBreaker, backends/rust_java).\n";
    out += "// It runs on runtime/jrt.rs: references are `&'static` handles, a class is a trait, an exception is an unwind. See that file's header.\n";
    out += "#![allow(unused, non_snake_case, non_camel_case_types, non_upper_case_globals, unused_mut, unused_parens, unreachable_code, unused_variables, dead_code, unused_assignments, unused_braces)]\n";
    out += "#[macro_use]\n#[path = \"runtime/jrt.rs\"]\nmod jrt;\nuse jrt::*;\nuse std::any::Any;\nuse std::cell::Cell;\n\n";
    for (const Lines& c : classes) {
        put(c);
        out += "\n";
    }
    for (const std::string& q : casters_) {
        put(caster_text(*model_->info(q)));
        out += "\n";
    }
    // the entry: the main class's static initialisers (its statics run before main), then main
    std::string call = main_cls_->cname;
    const std::string holder_name = holder(*main_cls_);
    out += "fn main() {\n    run(|| {\n";
    out += "        " + holder_name + "::clinit();\n";
    out += "        " + holder_name + "::" + std::string([&] {
        for (const ir::Node* mn : main_cls_->methods("main")) {
            const ir::MethodDecl& m = method_of(mn);
            if (is_static(m) && m.params.size() == 1) return mname(m);
        }
        return std::string("main");
    }()) + "(new_array::<Str>(0));\n";
    out += "    });\n}\n";
    return out;
}

Lines Program::class_text(const ClassInfo& cls) {
    if (static_only(cls)) return static_only_text(cls);
    Lines out;
    const auto add = [&](const Lines& l) { out.insert(out.end(), l.begin(), l.end()); };
    add(data_struct(cls));
    out.push_back("");
    add(trait_text(cls));
    out.push_back("");
    add(holder_text(cls));
    add(statics_text(cls));
    if (is_concrete(cls)) {
        out.push_back("");
        add(concrete_text(cls));
    }
    return out;
}

Lines Program::data_struct(const ClassInfo& cls) {
    Lines out;
    if (cls.kind == "interface") {
        out.push_back("pub struct " + holder(cls) + ";");
        return out;
    }
    out.push_back("#[derive(Default)]");
    out.push_back("pub struct " + holder(cls) + " {");
    if (cls.kind == "enum") {
        out.push_back("    pub enum_name: Cell<Str>,");
        out.push_back("    pub enum_ordinal: Cell<i32>,");
    }
    for (const ir::Node* member : cls.members()) {
        const auto* f = ir::get<ir::FieldDecl>(member);
        if (f == nullptr || has_mod(f->mods, "static") || (f->sym && f->sym->is_static)) continue;
        out.push_back("    pub " + member_ident(need_sym(f->sym, "a field").name) + ": Cell<" + rtype(f->type) + ">,");
    }
    out.push_back("}");
    return out;
}

std::string Program::trait_signature(const ir::MethodDecl& m) const {
    std::vector<std::string> ps = {"&'static self"};
    for (const ir::Node* pn : m.params) ps.push_back(ident(need_sym(param_of(pn).sym, "a parameter").name) + ": " + rtype(param_of(pn).type));
    std::string s = "fn " + mname(m) + "(" + join(ps) + ")";
    if (kind_of(m.ret) != "void" && m.ret) s += " -> " + rtype(m.ret);
    return s;
}

Lines Program::trait_text(const ClassInfo& cls) {
    std::vector<std::string> supers;
    if (const ClassInfo* base = base_class(cls)) supers.push_back(trait_name(*base));
    else if (cls.base && is_lib_throwable(*cls.base)) supers.push_back("Throwable");
    for (const std::string& i : cls.ifaces)
        if (model_->info(i) != nullptr) supers.push_back(trait_name(*model_->info(i)));
    if (supers.empty()) supers.push_back("Object");
    // two unrelated supertraits that declare one method make a call through this trait ambiguous in Rust
    std::map<std::string, std::string> declared;
    for (const ClassInfo* a : ancestors(cls)) {
        for (const ir::Node* mn : a->methods()) {
            const ir::MethodDecl& m = method_of(mn);
            if (m.kind != "method" || is_static(m) || is_private(m) || is_object_method(m) || !introduces(*a, m)) continue;
            const auto [it, fresh] = declared.emplace(mname(m), a->qname);
            if (!fresh && it->second != a->qname && !model_->is_sub(a->qname, it->second) && !model_->is_sub(it->second, a->qname))
                throw Decline("the method " + mname(m) + " reaches " + cls.qname + " from both " + it->second + " and " + a->qname);
        }
    }
    Lines out;
    out.push_back("pub trait " + trait_name(cls) + ": " + join(supers, " + ") + " {");
    if (cls.kind != "interface") out.push_back("    fn " + fields_fn(cls) + "(&'static self) -> &'static " + holder(cls) + ";");
    for (const ir::Node* mn : cls.methods()) {
        const ir::MethodDecl& m = method_of(mn);
        if (m.kind == "method" && introduces(cls, m)) out.push_back("    " + trait_signature(m) + ";");
    }
    out.push_back("}");
    return out;
}

Lines Program::method_text(const ClassInfo& cls, const ir::Node* mn) {
    const ir::MethodDecl& m = method_of(mn);
    if (!ir::present(m.body)) return {};   // abstract: the trait says it
    Writer w(*this, &cls, &m);
    const std::vector<ir::Node*> stmts = body_of(m);
    w.note_assigned(stmts);
    std::vector<std::string> ps;
    const bool stat = is_static(m);
    if (!stat) ps.push_back("this: &'static dyn " + trait_name(cls));
    for (const ir::Node* pn : m.params) {
        const std::string n = ident(need_sym(param_of(pn).sym, "a parameter").name);
        ps.push_back((w.is_assigned(n) ? "mut " : "") + n + ": " + rtype(param_of(pn).type));
    }
    if (stat) w.make_static();
    std::string head = "    pub fn " + mname(m) + "(" + join(ps) + ")";
    if (m.ret && kind_of(m.ret) != "void") head += " -> " + rtype(m.ret);
    Lines out = {head + " {"};
    const Lines body = w.body(stmts, m.ret, 2);
    out.insert(out.end(), body.begin(), body.end());
    out.push_back("    }");
    return out;
}

Lines Program::ctor_text(const ClassInfo& cls, const ir::Node* cn) {
    const ir::MethodDecl& ctor = method_of(cn);
    Writer w(*this, &cls, &ctor);
    std::vector<ir::Node*> stmts = body_of(ctor);
    w.note_assigned(stmts);
    std::vector<std::string> ps = {"this: &'static dyn " + trait_name(cls)};
    for (const ir::Node* pn : ctor.params) {
        const std::string n = ident(need_sym(param_of(pn).sym, "a parameter").name);
        ps.push_back((w.is_assigned(n) ? "mut " : "") + n + ": " + rtype(param_of(pn).type));
    }
    Lines out = {"    pub fn init" + ctor_suffix(cls, cn) + "(" + join(ps) + ") {"};
    bool runs_fields = true;
    std::span<ir::Node* const> rest(stmts);
    if (!stmts.empty()) {
        if (const auto* call = ir::get<ir::CtorCall>(stmts[0])) {
            const Lines lines = w.ctor_call(*call);
            for (const std::string& l : lines) out.push_back("        " + l);
            runs_fields = !(call->kind && call->kind->starts_with("this"));
            rest = rest.subspan(1);
        }
    }
    if (runs_fields) {   // the field initialisers and initialiser blocks, in the order they are written
        for (const ir::Node* member : cls.members()) {
            if (const auto* f = ir::get<ir::FieldDecl>(member); f != nullptr && f->init != nullptr && !has_mod(f->mods, "static") && !(f->sym && f->sym->is_static)) {
                Writer fw(*this, &cls, &ctor);
                out.push_back("        this." + fields_fn(cls) + "()." + member_ident(f->sym->name) + ".set(" + fw.value_of(f->init, f->type) + ");");
            } else if (const auto* init = ir::get<ir::MethodDecl>(member); init != nullptr && init->kind == "init" && ir::present(init->body)) {
                Writer iw(*this, &cls, init);
                const std::vector<ir::Node*> body = body_of(*init);
                iw.note_assigned(body);
                out.push_back("        {");
                const Lines lines = iw.body(body, ir::void_type(), 3);
                out.insert(out.end(), lines.begin(), lines.end());
                out.push_back("        }");
            }
        }
    }
    const Lines lines = w.body(rest, ir::void_type(), 2);
    out.insert(out.end(), lines.begin(), lines.end());
    out.push_back("    }");
    return out;
}

Lines Program::holder_text(const ClassInfo& cls) {
    Lines out = {"impl " + holder(cls) + " {"};
    const auto add = [&](const Lines& l) { out.insert(out.end(), l.begin(), l.end()); };
    for (const ir::Node* member : cls.members()) {
        if (const auto* f = ir::get<ir::FieldDecl>(member); f != nullptr && is_const_field(*f)) {
            Writer w(*this, &cls, nullptr);
            w.make_static();
            out.push_back("    pub const " + member_ident(f->sym->name) + ": " + rtype(f->type) + " = " + w.value_of(f->init, f->type) + ";");
        }
    }
    for (const ir::Node* mn : cls.methods()) {
        const ir::MethodDecl& m = method_of(mn);
        if (m.kind == "method") add(method_text(cls, mn));
        else if (m.kind == "ctor" && cls.kind != "interface" && !static_only(cls)) add(ctor_text(cls, mn));
    }
    if (cls.kind == "enum") {
        std::vector<std::string> all;
        for (const ir::Node* member : cls.members())
            if (const auto* ec = ir::get<ir::EnumConst>(member)) all.push_back(static_field_name(cls, ec->sym->name) + "::get()");
        out.push_back("    pub fn values() -> Arr<Ref<dyn " + trait_name(cls) + ">> {");
        out.push_back("        array_of(&[" + join(all) + "])");
        out.push_back("    }");
        out.push_back("    pub fn valueOf(name: Str) -> Ref<dyn " + trait_name(cls) + "> {");
        for (const ir::Node* member : cls.members())
            if (const auto* ec = ir::get<ir::EnumConst>(member))
                out.push_back("        if name.equals(lit(" + rs_string(ec->sym->name) + ")) { return " + static_field_name(cls, ec->sym->name) + "::get(); }");
        out.push_back("        throw_new(\"java.lang.IllegalArgumentException\", Some(format!(\"No enum constant " + cls.qname + ".{}\", name.to_rs())))");
        out.push_back("    }");
    }
    // the static initialisers of the main class run before main
    std::vector<std::string> inits;
    if (&cls == main_cls_) {
        for (const ir::Node* member : cls.members()) {
            if (const auto* f = ir::get<ir::FieldDecl>(member); f != nullptr && f->init != nullptr && (has_mod(f->mods, "static") || (f->sym && f->sym->is_static)) && !is_const_field(*f))
                inits.push_back("let _ = " + static_field_name(cls, f->sym->name) + "::get();");
            else if (const auto* init = ir::get<ir::MethodDecl>(member); init != nullptr && init->kind == "static_init" && ir::present(init->body)) {
                Writer w(*this, &cls, init);
                w.make_static();
                const std::vector<ir::Node*> body = body_of(*init);
                w.note_assigned(body);
                inits.push_back("{");
                for (const std::string& l : w.body(body, ir::void_type(), 3)) inits.push_back(l);
                inits.push_back("}");
            }
        }
    } else {
        for (const ir::Node* member : cls.members())
            if (const auto* init = ir::get<ir::MethodDecl>(member); init != nullptr && init->kind == "static_init" && ir::present(init->body))
                throw Decline("a static initialiser block outside the main class (" + cls.qname + ")");
    }
    if (&cls == main_cls_) {
        out.push_back("    pub fn clinit() {");
        for (const std::string& l : inits) out.push_back(l.starts_with(" ") ? l : "        " + l);
        out.push_back("    }");
    }
    out.push_back("}");
    return out;
}

Lines Program::statics_text(const ClassInfo& cls) {
    Lines out;
    std::int32_t ordinal = 0;
    for (const ir::Node* member : cls.members()) {   // an enum's constants: made when first used, as their class is
        const auto* ec = ir::get<ir::EnumConst>(member);
        if (ec == nullptr) continue;
        Writer w(*this, &cls, nullptr);
        w.make_static();
        const auto match = model_->method_decl(need_sym(ec->ctor, "an enum constructor"));
        if (!match) throw Decline("the constructor of the enum constant " + ec->sym->name);
        std::vector<ir::TypeP> params;
        for (const ir::Node* pn : method_of(match->method).params) params.push_back(param_of(pn).type);
        std::vector<std::string> args = {"lit(" + rs_string(ec->sym->name) + ")", std::to_string(ordinal++)};
        for (const std::string& a : w.call_args(ec->args, params, false)) args.push_back(a);
        out.push_back("jstatic!(" + static_field_name(cls, ec->sym->name) + ": Ref<dyn " + trait_name(cls) + "> = " + obj_name(cls) + "::new" +
                      ctor_suffix(cls, match->method) + "(" + join(args) + "));");
    }
    for (const ir::Node* member : cls.members()) {
        const auto* f = ir::get<ir::FieldDecl>(member);
        if (f == nullptr || !(has_mod(f->mods, "static") || (f->sym && f->sym->is_static)) || is_const_field(*f)) continue;
        Writer w(*this, &cls, nullptr);
        w.make_static();
        const std::string init = f->init != nullptr ? w.value_of(f->init, f->type) : "Default::default()";
        out.push_back("jstatic!(" + static_field_name(cls, f->sym->name) + ": " + rtype(f->type) + " = " + init + ");");
    }
    return out;
}

Lines Program::static_only_text(const ClassInfo& cls) {
    Lines out = {"pub struct " + cls.cname + ";"};
    const Lines holder_lines = holder_text(cls);
    out.insert(out.end(), holder_lines.begin(), holder_lines.end());
    const Lines statics = statics_text(cls);
    out.insert(out.end(), statics.begin(), statics.end());
    return out;
}

Lines Program::concrete_text(const ClassInfo& cls) {
    Lines out;
    const std::string obj = obj_name(cls);
    // the data of this class and of every class above it
    std::vector<const ClassInfo*> chain = {&cls};
    for (const ClassInfo* a : ancestors(cls)) chain.push_back(a);
    const bool throwable = is_throwable(cls);
    out.push_back("#[derive(Default)]");
    out.push_back("pub struct " + obj + " {");
    if (throwable) out.push_back("    throwable: ThrowableData,");
    for (const ClassInfo* c : chain)
        if (c->kind != "interface") out.push_back("    d_" + c->cname + ": " + holder(*c) + ",");
    out.push_back("}");
    out.push_back("");
    // Object: the three methods a class may override, its name and what it is
    out.push_back("impl Object for " + obj + " {");
    for (const char* object_method : {"toString", "equals", "hashCode"}) {
        const std::optional<MethodRef> impl = find_impl(cls, object_method);
        // (the Object methods are found by name: an overload of equals has its own name)
        std::optional<MethodRef> own;
        for (const ClassInfo* c = &cls; c != nullptr && !own; c = base_class(*c))
            for (const ir::Node* mn : c->methods(object_method)) {
                const ir::MethodDecl& m = method_of(mn);
                if (m.kind == "method" && !is_static(m) && ir::present(m.body) && is_object_method(m)) own = MethodRef{c, mn};
            }
        const std::string_view name = object_method;
        if (own) {
            const ir::MethodDecl& m = method_of(own->node);
            if (name == "toString") out.push_back("    fn toString(&'static self) -> Str { " + holder(*own->cls) + "::toString(self) }");
            else if (name == "equals") out.push_back("    fn equals(&'static self, other: Ref<dyn Object>) -> bool { " + holder(*own->cls) + "::equals(self, other) }");
            else out.push_back("    fn hashCode(&'static self) -> i32 { " + holder(*own->cls) + "::hashCode(self) }");
            (void)m;
        } else if (name == "toString" && cls.kind == "enum") {
            out.push_back("    fn toString(&'static self) -> Str { self." + fields_fn(cls) + "().enum_name.get() }");
        } else if (name == "toString" && throwable) {
            out.push_back("    fn toString(&'static self) -> Str { throwable_to_string(self) }");
        }
        (void)impl;
    }
    out.push_back("    fn class_name(&'static self) -> &'static str { " + rs_string(binary_name(cls)) + " }");
    std::vector<std::string> chain_names;
    for (const std::string& q : model_->ancestors(cls.qname)) chain_names.push_back(rs_string(q));
    chain_names.push_back(rs_string("java.lang.Object"));
    chain_names.erase(std::unique(chain_names.begin(), chain_names.end()), chain_names.end());
    std::sort(chain_names.begin() + 1, chain_names.end());
    chain_names.erase(std::unique(chain_names.begin(), chain_names.end()), chain_names.end());
    out.push_back("    fn class_chain(&'static self) -> &'static [&'static str] { &[" + join(chain_names) + "] }");
    out.push_back("    fn as_any(&'static self) -> &'static dyn Any { self }");
    out.push_back("    fn as_object(&'static self) -> &'static dyn Object { self }");
    out.push_back("}");
    if (throwable) {
        out.push_back("impl Throwable for " + obj + " {");
        out.push_back("    fn Throwable_fields(&'static self) -> &'static ThrowableData { &self.throwable }");
        out.push_back("}");
    }
    // each trait of the class and of everything above it
    for (const ClassInfo* c : chain) {
        out.push_back("impl " + trait_name(*c) + " for " + obj + " {");
        if (c->kind != "interface") out.push_back("    fn " + fields_fn(*c) + "(&'static self) -> &'static " + holder(*c) + " { &self.d_" + c->cname + " }");
        for (const ir::Node* mn : c->methods()) {
            const ir::MethodDecl& m = method_of(mn);
            if (m.kind != "method" || !introduces(*c, m)) continue;
            const std::string rn = mname(m);
            const std::optional<MethodRef> impl = find_impl(cls, rn);
            if (!impl) throw Decline(cls.qname + " does not implement " + c->qname + "." + rn);
            std::vector<std::string> args = {"self"};
            for (const ir::Node* pn : m.params) args.push_back(ident(param_of(pn).sym->name));
            const bool unit = !m.ret || kind_of(m.ret) == "void";
            out.push_back("    " + trait_signature(m) + " { " + holder(*impl->cls) + "::" + mname(method_of(impl->node)) + "(" + join(args) + ") }");
            (void)unit;
        }
        out.push_back("}");
    }
    // construction: allocate (every field zero), then run the constructor
    out.push_back("impl " + obj + " {");
    for (const ir::Node* mn : cls.methods()) {
        const ir::MethodDecl& m = method_of(mn);
        if (m.kind != "ctor") continue;
        std::vector<std::string> ps, args = {"this"};
        if (cls.kind == "enum") ps = {"name: Str", "ordinal: i32"};
        for (const ir::Node* pn : m.params) {
            const std::string n = ident(param_of(pn).sym->name);
            ps.push_back(n + ": " + rtype(param_of(pn).type));
            args.push_back(n);
        }
        const std::string suffix = ctor_suffix(cls, mn);
        out.push_back("    pub fn new" + suffix + "(" + join(ps) + ") -> Ref<dyn " + trait_name(cls) + "> {");
        out.push_back("        let this: &'static " + obj + " = gc_new(" + obj + "::default());");
        if (cls.kind == "enum") {
            out.push_back("        this." + fields_fn(cls) + "().enum_name.set(name);");
            out.push_back("        this." + fields_fn(cls) + "().enum_ordinal.set(ordinal);");
        }
        out.push_back("        " + holder(cls) + "::init" + suffix + "(" + join(args) + ");");
        out.push_back("        Some(this)");
        out.push_back("    }");
    }
    out.push_back("}");
    return out;
}

Lines Program::caster_text(const ClassInfo& cls) {
    Lines out;
    out.push_back("fn cast_" + cls.cname + "(o: Ref<dyn Object>) -> Ref<dyn " + trait_name(cls) + "> {");
    out.push_back("    let r = match o {");
    out.push_back("        None => return None,");
    out.push_back("        Some(r) => r,");
    out.push_back("    };");
    out.push_back("    let any = r.as_any();");
    for (const ClassInfo* c : subclasses_of(cls)) {
        out.push_back("    if let Some(c) = any.downcast_ref::<" + obj_name(*c) + ">() {");
        out.push_back("        return Some(c);");
        out.push_back("    }");
    }
    out.push_back("    throw_new(\"java.lang.ClassCastException\", Some(format!(\"class {} cannot be cast to class " + binary_name(cls) + "\", r.class_name())))");
    out.push_back("}");
    return out;
}

}  // namespace cb::translate::backends::rust_java::detail
