// codebreaker/java/emit/header.cpp -- typed C++ port of codebreaker.java.emit.header (see header.hpp).

#include "codebreaker/java/emit/header.hpp"

#include <cstdio>
#include <functional>

#include "codebreaker/java/emit/naming.hpp"
#include "codebreaker/java/emit/ownership.hpp"
#include "codebreaker/java/emit/std_transforms.hpp"
#include "codebreaker/java/typemap.hpp"

namespace codebreaker::java::emit::header {

namespace {

using py::str;
using ledger::Category;
using ledger::Severity;

str S(const char* s) { return str(s); }

// apply_std_transforms(text, cpp_std): the transform pass reads only the standard's year.
str apply_transforms(const str& text, const std::optional<CppStd>& cpp_std) {
    std::optional<std_transforms::CppStandard> std_;
    if (cpp_std) {
        std_ = std_transforms::CppStandard{cpp_std->year};
    }
    return std_transforms::apply_std_transforms(text, std_);
}

}  // namespace

CppStd::CppStd(int64_t year_) : year(year_) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "C++%02lld", static_cast<long long>(py::mod(year_, 100)));
    label = str(buf);
}

str _banner(const str& cls_name, const std::optional<CppStd>& cpp_std, const std::optional<str>& platform_label) {
    str std_label = cpp_std ? cpp_std->label : S("C++03");
    str plat = (platform_label && !platform_label->empty()) ? *platform_label : S("unknown");
    return S("// Auto-generated from ") + cls_name + ".java\n// Target: " + plat + " / " + std_label;
}

std::optional<str> _std_header_for(const str& cpp_base) {
    // Object -> JObject (the universal boxed value type) is a name-changing mapping, so it lands here
    // rather than as a user X.h. JObject lives in the runtime prelude; a header that takes/returns it
    // BY VALUE (e.g. `equals(JObject)`) needs the full definition, so pull in JavaRT.h. Without this the
    // header has neither an include nor a forward-decl for JObject and fails to compile standalone (the
    // only reason it ever worked was the .cpp happening to #include JavaRT.h before the class header).
    if (cpp_base.contains(S("JObject"))) {
        return S("\"JavaRT.h\"");
    }
    if (cpp_base.contains(S("std::string"))) {
        return S("<string>");
    }
    if (cpp_base.contains(S("std::vector"))) {
        return S("<vector>");
    }
    if (cpp_base.contains(S("JMap"))) {
        return std::nullopt;   // JMap.h is always included (like JList.h/JSet.h)
    }
    if (cpp_base.contains(S("std::map"))) {
        return S("<map>");
    }
    if (cpp_base.contains(S("std::set"))) {
        return S("<set>");
    }
    return std::nullopt;
}

str _base_type_spelling(const str& parent) {
    // C++ base-class spelling for a Java class that EXTENDS a container (e.g. `extends ArrayList`).
    // ArrayList/List -> JList<void* > (JList.h is always included); HashMap/Map -> std::map<JObject,
    // JObject>; HashSet/Set/TreeSet -> std::set<void* >. Non-container parents are returned unchanged.
    if (!typemap::CONTAINERS.contains(parent)) {
        return parent;
    }
    str base = typemap::JAVA_TO_CPP.get(parent, parent);
    return base + typemap::RAW_CONTAINER_DEFAULT.get(base, S(""));
}

Dependencies _collect_dependencies(const ClassRef& cls, const TypeResolver& resolver) {
    // Return (system_includes, quoted_includes, forward_decls).
    py::set<str> sys_inc, inc, fwd;

    std::function<void(const JTypeRef&)> note = [&](const JTypeRef& jtype) {
        str name = jtype->name;
        if (typemap::PRIMITIVES.contains(name)) {
            return;
        }
        str cpp_base = resolver.base_cpp(name);
        if (cpp_base != name) {   // std-mapped (string/vector/...)
            std::optional<str> h = _std_header_for(cpp_base);
            if (h && !h->empty()) {
                sys_inc.add(*h);
            }
            for (const JTypeRef& a : jtype->args) {   // generic element types still need decls
                note(a);
            }
            return;
        }
        // user type with its own X.h
        if (resolver.is_value(jtype) && !jtype->is_array()) {
            inc.add(name);   // by value -> need full definition
        } else {
            fwd.add(name);   // pointer -> forward declare
        }
        for (const JTypeRef& a : jtype->args) {
            note(a);
        }
    };

    if (cls->parent && !cls->parent->empty()) {
        if (typemap::CONTAINERS.contains(*cls->parent)) {   // class extends ArrayList/HashMap/... -> STL base
            std::optional<str> h = _std_header_for(_base_type_spelling(*cls->parent));
            if (h && !h->empty()) {
                sys_inc.add(*h);
            }
        } else {
            inc.add(*cls->parent);
        }
    }
    for (const str& iface : cls->interfaces) {
        inc.add(iface);
    }
    for (const FieldRef& f : cls->fields) {
        note(f->jtype);
    }
    for (const MethodRef& m : cls->methods) {
        if (m->return_type) {
            note(m->return_type);
        }
        for (const py::ref<ir::Param>& p : m->params) {
            note(p->jtype);
        }
    }
    for (const auto& im_base : cls->mi_forwards) {   // synthesized forwarding overrides
        const MethodRef& im = std::get<0>(im_base);
        if (im->return_type) {
            note(im->return_type);
        }
        for (const py::ref<ir::Param>& p : im->params) {
            note(p->jtype);
        }
    }

    inc.discard(cls->name);
    fwd.discard(cls->name);
    fwd = fwd - inc;   // if we #include it, don't also forward-declare
    return {sys_inc, inc, fwd};
}

str _method_decl(const ClassRef& cls, const MethodRef& m, const TypeResolver& resolver) {
    str params = ownership::params_str(m, resolver);
    if (m->is_ctor) {
        return cls->name + "(" + params + ");";
    }
    str ret = ownership::return_cpp_type(m, resolver);
    str prefix = m->is_static ? S("static ") : S("virtual ");
    str suffix = m->is_abstract ? S(" = 0") : S("");
    str name = resolver.renamed_method(cls->name, m->name);   // field/method name-collision -> name_
    return prefix + ret + " " + name + "(" + params + ")" + suffix + ";";
}

str _field_type(const ClassRef& cls, const FieldRef& f, const TypeResolver& resolver) {
    // C++ type for an instance field. Scalar Java `byte` -> `signed char` (Java-faithful; resolver
    // handles it) and `byte[]` -> unsigned-char buffer -- see the signed-byte policy in typemap. The
    // only override is the (empty by default) UNSIGNED_BYTE_SCALAR_FIELDS escape hatch.
    if (f->jtype->name == S("byte") && !f->jtype->is_array() &&
        typemap::UNSIGNED_BYTE_SCALAR_FIELDS.contains(std::make_tuple(cls->name, f->name))) {
        return S("unsigned char");
    }
    return resolver.field_type(f->jtype);
}

str _render_typed_enum_header(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger,
                              const std::optional<CppStd>& cpp_std, const std::optional<str>& platform_label) {
    // A TYPED enum (constants carry ctor args / the enum has instance fields or methods) becomes a
    // real class: instance fields + ctor + methods are preserved, and each constant is a singleton
    // `static EnumX* NAME` built in initStatics(). All statics use BARE names (no s_), because a typed
    // enum stays in the holder set so `EnumX.NAME` resolves to `EnumX::NAME` (see types.py).
    str guard = naming::guard_macro(cls->name);
    py::list<str> L = {S("#ifndef ") + guard, S("#define ") + guard, S(""),
                       _banner(cls->name, cpp_std, platform_label), S(""),
                       S("#include \"JavaString.h\""), S("#include \"JArray.h\""), S("#include \"JList.h\""),
                       S("#include \"JSet.h\""), S("#include \"JMap.h\"")};
    auto [sys_inc, inc, fwd] = _collect_dependencies(cls, resolver);
    for (const str& h : py::sorted(sys_inc)) {
        L.append(S("#include ") + h);
    }
    for (const str& h : py::sorted(inc)) {
        L.append(S("#include \"") + h + ".h\"");
    }
    for (const str& name : py::sorted(fwd)) {
        L.append(S("class ") + name + ";");
    }
    L.append(S(""));
    L.append(S("class ") + cls->name + " {");
    L.append(S("public:"));
    for (const MethodRef& m : cls->constructors()) {
        L.append(S("\t") + _method_decl(cls, m, resolver));
    }
    L.append(S("\tvirtual ~") + cls->name + "();");
    L.append(S(""));
    L.append(S("\tint __ordinal;   // Java enum ordinal (0-based, declaration order)"));
    for (const FieldRef& f : cls->instance_fields()) {
        L.append(S("\t") + _field_type(cls, f, resolver) + " " + f->name + ";");   // bare instance name
    }
    L.append(S(""));
    for (const py::ref<ir::EnumConst>& ec : cls->enum_consts) {   // the singleton constants
        L.append(S("\tstatic ") + cls->name + "* " + naming::enum_member(ec->name) + ";");
    }
    for (const FieldRef& f : cls->static_fields()) {   // real enum statics -> BARE name (holder access)
        L.append(S("\tstatic ") + resolver.field_type(f->jtype) + " " + f->name + ";");
    }
    L.append(S(""));
    for (const MethodRef& m : cls->methods) {
        if (!m->is_ctor) {
            L.append(S("\t") + _method_decl(cls, m, resolver));
        }
    }
    L.append(S("\tint ordinal();"));
    L.append(S("\tstatic JArray<") + cls->name + "* > _j2x_values;");   // backing table (defined in the .cpp)
    L.append(S("\tstatic JArray<") + cls->name + "* >& values();");
    L.append(S("\tstatic void initStatics();"));
    L.append(S("};"));
    L.append(S(""));
    L.append(S("#endif // ") + guard);
    if (ledger != nullptr) {
        ledger->add(cls->name + ".h", Category::PORTED,
                    S("typed enum ") + cls->name + " -> class with " + py::to_str(py::len(cls->enum_consts)) +
                        " singleton constants + preserved fields/methods",
                    0, std::nullopt, S("emit"), S("typed-enum"));
    }
    return apply_transforms(py::join(S("\n"), L), cpp_std);
}

str render_header(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger,
                  const std::optional<CppStd>& cpp_std, const std::optional<str>& platform_label,
                  const std::optional<py::set<str>>& two_phase_classes) {
    str guard = naming::guard_macro(cls->name);

    if (cls->is_enum && resolver.typed_enums.contains(cls->name)) {
        return _render_typed_enum_header(cls, resolver, ledger, cpp_std, platform_label);
    }

    // Constant-only enums: emit the constants as scoped static const ints so `EnumOS::linux` resolves;
    // the enum TYPE itself maps to int (see types.py), so variables/switches are plain ints.
    if (cls->is_enum) {
        py::list<str> L = {S("#ifndef ") + guard, S("#define ") + guard, S(""),
                           _banner(cls->name, cpp_std, platform_label), S(""), S("struct ") + cls->name + " {"};
        int64_t i = 0;
        for (const py::ref<ir::EnumConst>& ec : cls->enum_consts) {
            L.append(S("\tstatic const int ") + naming::enum_member(ec->name) + " = " + py::to_str(i) + ";");
            ++i;
        }
        // EnumX.values().length -> EnumX::_J2X_COUNT (the member value IS its ordinal, 0-based, so
        // `EnumX::MEMBER.ordinal()` is the identity -- handled by the back end's x_Call).
        L.append(S("\tstatic const int _J2X_COUNT = ") + py::to_str(py::len(cls->enum_consts)) + ";");
        L.append(S("};"));
        L.append(S(""));
        L.append(S("#endif // ") + guard);
        if (ledger != nullptr) {
            ledger->add(cls->name + ".h", Category::PORTED,
                        S("enum ") + cls->name + " -> struct of static const int constants", 0, std::nullopt,
                        S("emit"), S("enum"));
        }
        return apply_transforms(py::join(S("\n"), L), cpp_std);
    }

    py::list<str> L = {S("#ifndef ") + guard, S("#define ") + guard, S(""),
                       _banner(cls->name, cpp_std, platform_label), S("")};

    // String -> JavaString and T[] -> JArray<T>; a header that mentions either in a field/
    // param/return needs the full definition. Cheap and idempotent to always include.
    L.append(S("#include \"JavaString.h\""));
    L.append(S("#include \"JArray.h\""));
    L.append(S("#include \"JList.h\""));
    L.append(S("#include \"JSet.h\""));
    L.append(S("#include \"JMap.h\""));

    auto [sys_inc, inc, fwd] = _collect_dependencies(cls, resolver);
    for (const str& h : py::sorted(sys_inc)) {
        L.append(S("#include ") + h);
    }
    for (const str& h : py::sorted(inc)) {
        L.append(S("#include \"") + h + ".h\"");
    }
    if (sys_inc.size() != 0 || inc.size() != 0) {
        L.append(S(""));
    }
    for (const str& name : py::sorted(fwd)) {
        L.append(S("class ") + name + ";");
    }
    if (fwd.size() != 0) {
        L.append(S(""));
    }

    // class declaration
    py::list<str> bases;
    if (cls->parent && !cls->parent->empty()) {
        bases.append(S("public ") + _base_type_spelling(*cls->parent));
    }
    for (const str& i : cls->interfaces) {
        bases.append(S("public ") + i);
    }
    str decl = S("class ") + cls->name + (py::len(bases) != 0 ? S(" : ") + py::join(S(", "), bases) : S(""));
    L.append(decl + " {");
    L.append(S("public:"));

    // constructors + destructor
    bool two_phase = two_phase_classes && two_phase_classes->contains(cls->name);
    py::list<MethodRef> ctors = cls->constructors();
    if (two_phase) {
        // two-phase (see twophase.py): C() only zeroes the fields; each Java constructor is j2xInit(...)
        L.append(S("\t") + cls->name + "();");
        for (const MethodRef& m : ctors) {
            L.append(S("\t") + cls->name + "* j2xInit(" + ownership::params_str(m, resolver) + ");");
        }
        if (py::len(ctors) == 0) {
            L.append(S("\t") + cls->name + "* j2xInit();");
        }
    } else {
        for (const MethodRef& m : ctors) {
            L.append(S("\t") + _method_decl(cls, m, resolver));
        }
    }
    // a value-type class held by value as a field must be default-constructible; if it only
    // has parameterized ctors, synthesize a (zero-initializing) default ctor
    bool any_default_ctor = false;
    for (const MethodRef& c : ctors) {
        if (c->arity() == 0) {
            any_default_ctor = true;
        }
    }
    if (two_phase) {
        // (nothing: C() was declared above)
    } else if (resolver.values.contains(cls->name) && py::len(ctors) != 0 && !any_default_ctor) {
        L.append(S("\t") + cls->name + "();");
    }
    // A class with NO explicit ctor relies on Java's implicit default ctor, which zero-inits ALL fields
    // (and runs any field initializers / init-blocks). C++'s compiler-default ctor leaves primitives and
    // pointers as GARBAGE -- the uninitialized-field crash class (e.g. GuiScreen had no ctor, so
    // allowUserInput was garbage -> the menu deref'd a NULL player -> IP 0x0). So synthesize a zero-init
    // default ctor whenever such a class has ANY instance fields (or init-blocks). source.py emits the
    // matching definition via _init_list (primitives/pointers -> 0; objects default-construct).
    else if (py::len(ctors) == 0 && (py::len(cls->instance_fields()) != 0 || py::len(cls->inst_inits) != 0)) {
        L.append(S("\t") + cls->name + "();");
    }
    if (!cls->is_interface) {
        L.append(S("\tvirtual ~") + cls->name + "();");
    }

    // fields (instance, then static)
    py::list<FieldRef> inst = cls->instance_fields();
    py::list<FieldRef> stat = cls->static_fields();
    if (py::len(inst) != 0 || py::len(stat) != 0) {
        L.append(S(""));
    }
    for (const FieldRef& f : inst) {
        naming::Field nf{f->name, f->is_static};
        L.append(S("\t") + _field_type(cls, f, resolver) + " " + naming::field_name(nf) + ";");
    }
    for (const FieldRef& f : stat) {   // arrays are JArray<T> now -> normal member decl
        naming::Field nf{f->name, f->is_static};
        L.append(S("\tstatic ") + resolver.field_type(f->jtype) + " " + naming::field_name(nf) + ";");
    }

    // methods (skip ctors, already emitted)
    py::list<MethodRef> methods;
    for (const MethodRef& m : cls->methods) {
        if (!m->is_ctor) {
            methods.append(m);
        }
    }
    if (py::len(methods) != 0) {
        L.append(S(""));
    }
    for (const MethodRef& m : methods) {
        L.append(S("\t") + _method_decl(cls, m, resolver));
    }

    // java.lang.Object.toString() is callable through any reference, but a generated INTERFACE has no
    // Object base to inherit it from -- so a `functor->toString()` call won't compile. Give an interface
    // that doesn't declare its own toString a default one (argo's J_Functor).
    if (cls->is_interface) {
        bool declares_to_string = false;
        for (const MethodRef& mm : cls->methods) {
            if (mm->name == S("toString") && py::len(mm->params) == 0) {
                declares_to_string = true;
            }
        }
        if (!declares_to_string) {
            L.append(S("\tvirtual JavaString toString() { return JavaString(); }"));
        }
    }

    // Ordered static init: deferred static field initializers + Java static{} blocks run here,
    // called in dependency order by the generated driver j2x_initAll() (see source.py / j2x_init.cpp).
    // Always declared (empty when nothing to defer) so the driver can call it on any concrete class.
    if (!cls->is_interface) {
        L.append(S(""));
        L.append(S("\tstatic void initStatics();"));
    }

    // MI-diamond unification: forwarding overrides so an interface's pure-virtual that a base
    // class already implements concretely is satisfied in THIS class (see compute_mi_forwards).
    if (py::len(cls->mi_forwards) != 0) {
        L.append(S(""));
        for (const auto& im_base : cls->mi_forwards) {   // concrete override (NOT '= 0' like the interface)
            const MethodRef& im = std::get<0>(im_base);
            L.append(S("\tvirtual ") + ownership::return_cpp_type(im, resolver) + " " + im->name + "(" +
                     ownership::params_str(im, resolver) + ");");
        }
    }

    // Rule of Three: a class that owns heap (freed in its destructor) must forbid
    // copying, else a by-value copy double-frees the pointer -> heap corruption.
    if (ownership::owns_heap(cls, resolver)) {
        L.append(S(""));
        L.append(S("private:"));
        L.append(S("\t// owns heap; copying disabled so a copy cannot double-free (Rule of Three)"));
        L.append(S("\t") + cls->name + "(const " + cls->name + "&);");
        L.append(S("\t") + cls->name + "& operator=(const " + cls->name + "&);");
        if (ledger != nullptr) {
            ledger->add(cls->name + ".h", Category::RISK,
                        S("Rule-of-Three: copy ctor + assignment disabled (owns heap) to prevent "
                          "double-free heap corruption"),
                        0, Severity::MED, S("emit"), S("rule-of-three-nocopy"));
        }
    }

    L.append(S("};"));
    L.append(S(""));
    L.append(S("#endif // ") + guard);

    if (ledger != nullptr) {
        _report(cls, resolver, ledger);
    }
    return apply_transforms(py::join(S("\n"), L), cpp_std);
}

void _report(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger) {
    str fname = cls->name + ".h";
    ledger->add(fname, Category::PORTED, S("generated header for ") + cls->name, 0, std::nullopt, S("emit"),
                S("header"));
    py::set<str> seen;
    for (const FieldRef& f : cls->fields) {
        std::optional<str> note = resolver.port_note(f->jtype);
        if (note && !note->empty() && !seen.contains(f->jtype->name)) {
            seen.add(f->jtype->name);
            ledger->add(fname, Category::ANNOTATED,
                        S("field type ") + f->jtype->name + " -> " + resolver.field_type(f->jtype) + " (" + *note + ")",
                        f->line, Severity::LOW, S("emit"), S("type-note"));
        }
    }
}

}  // namespace codebreaker::java::emit::header
