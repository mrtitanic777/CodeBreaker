// cb-unity: alone (helpers named for what they do here)
#include "cb/java/emit/header.hpp"

#include <algorithm>
#include <format>
#include <vector>

#include "cb/core/text.hpp"
#include "cb/java/emit/naming.hpp"
#include "cb/java/emit/ownership.hpp"
#include "cb/java/emit/std_transforms.hpp"

namespace cb::java::emit::header {

namespace {

using Lines = std::vector<std::string>;

// The includes every header starts with: strings, arrays and the collections are cheap to include and idempotent, and a
// header that mentions any of them in a field, param or return needs the full definition.
constexpr std::string_view kRuntimeIncludes[] = {"JavaString.h", "JArray.h", "JList.h", "JSet.h", "JMap.h"};

// The type of an instance field. A scalar Java `byte` is `signed char` (Java-faithful: the resolver does that) and
// `byte[]` an unsigned-char buffer -- the signed-byte policy of the type map; the one override is the (empty by
// default) set of scalar bytes forced back to unsigned.
std::string field_type_of(const ir::JClass& cls, const ir::Field& f, const types::TypeResolver& resolver, const Options& options) {
    if (f.type.name == "byte" && !f.type.is_array() && options.unsigned_byte_scalar_fields.contains({cls.name, f.name})) {
        return "unsigned char";
    }
    return resolver.field_type(f.type);
}

void add_runtime_includes(Lines& out) {
    for (const std::string_view h : kRuntimeIncludes) out.push_back(std::format("#include \"{}\"", h));
}

void add_dependency_lines(Lines& out, const Dependencies& deps) {
    for (const std::string& h : deps.system_includes) out.push_back("#include " + h);
    for (const std::string& h : deps.includes) out.push_back(std::format("#include \"{}.h\"", h));
}

// A constant-only enum: the constants as scoped static const ints so `EnumOS::linux` resolves; the enum TYPE itself maps
// to int (see the resolver), so variables and switches are plain ints.
std::string render_constant_enum(const ir::JClass& cls, const Options& options) {
    const std::string guard = naming::guard_macro(cls.name);
    Lines out{"#ifndef " + guard, "#define " + guard, "", banner(cls.name, options.cpp_std, options.platform_label), "",
              std::format("struct {} {{", cls.name)};
    for (std::size_t i = 0; i < cls.enum_consts.size(); ++i) {
        out.push_back(std::format("\tstatic const int {} = {};", naming::enum_member(cls.enum_consts[i].name), i));
    }
    // EnumX.values().length -> EnumX::_J2X_COUNT (the member value IS its ordinal, 0-based, so `EnumX::MEMBER.ordinal()`
    // is the identity -- handled by the back end).
    out.push_back(std::format("\tstatic const int _J2X_COUNT = {};", cls.enum_consts.size()));
    out.push_back("};");
    out.push_back("");
    out.push_back("#endif // " + guard);
    if (options.ledger != nullptr) {
        options.ledger->add(cls.name + ".h", ledger::Category::Ported, std::format("enum {} -> struct of static const int constants", cls.name),
                            {.source = "emit", .rule = "enum"});
    }
    return text::join(out, "\n");
}

// A TYPED enum (constants carry ctor args, or the enum has instance fields or methods) becomes a real class: instance
// fields, constructor and methods are preserved, and each constant is a singleton `static EnumX* NAME` built in
// initStatics(). All statics use BARE names (no s_), because a typed enum stays in the holder set so `EnumX.NAME`
// resolves to `EnumX::NAME`.
std::string render_typed_enum(const ir::JClass& cls, const types::TypeResolver& resolver, const Options& options) {
    const std::string guard = naming::guard_macro(cls.name);
    Lines out{"#ifndef " + guard, "#define " + guard, "", banner(cls.name, options.cpp_std, options.platform_label), ""};
    add_runtime_includes(out);
    const Dependencies deps = collect_dependencies(cls, resolver);
    add_dependency_lines(out, deps);
    for (const std::string& name : deps.forward_decls) out.push_back(std::format("class {};", name));
    out.push_back("");
    out.push_back(std::format("class {} {{", cls.name));
    out.push_back("public:");
    for (const ir::Method* m : cls.constructors()) out.push_back("\t" + method_decl(cls, *m, resolver));
    out.push_back(std::format("\tvirtual ~{}();", cls.name));
    out.push_back("");
    out.push_back("\tint __ordinal;   // Java enum ordinal (0-based, declaration order)");
    for (const ir::Field* f : cls.instance_fields()) {
        out.push_back(std::format("\t{} {};", field_type_of(cls, *f, resolver, options), f->name));   // bare instance name
    }
    out.push_back("");
    for (const ir::EnumConst& ec : cls.enum_consts) {   // the singleton constants
        out.push_back(std::format("\tstatic {}* {};", cls.name, naming::enum_member(ec.name)));
    }
    for (const ir::Field* f : cls.static_fields()) {   // real enum statics -> BARE name (holder access)
        out.push_back(std::format("\tstatic {} {};", resolver.field_type(f->type), f->name));
    }
    out.push_back("");
    for (const ir::Method& m : cls.methods) {
        if (!m.is_ctor) out.push_back("\t" + method_decl(cls, m, resolver));
    }
    out.push_back("\tint ordinal();");
    out.push_back(std::format("\tstatic JArray<{}* > _j2x_values;", cls.name));   // backing table (defined in the .cpp)
    out.push_back(std::format("\tstatic JArray<{}* >& values();", cls.name));
    out.push_back("\tstatic void initStatics();");
    out.push_back("};");
    out.push_back("");
    out.push_back("#endif // " + guard);
    if (options.ledger != nullptr) {
        options.ledger->add(cls.name + ".h", ledger::Category::Ported,
                            std::format("typed enum {} -> class with {} singleton constants + preserved fields/methods", cls.name,
                                        cls.enum_consts.size()),
                            {.source = "emit", .rule = "typed-enum"});
    }
    return text::join(out, "\n");
}

// The constructors and the destructor of an ordinary class.
void add_constructors(Lines& out, const ir::JClass& cls, const types::TypeResolver& resolver, const Options& options) {
    const std::vector<const ir::Method*> ctors = cls.constructors();
    const bool two_phase = options.two_phase.contains(cls.name);
    if (two_phase) {
        // two-phase: C() only zeroes the fields; each Java constructor is j2xInit(...)
        out.push_back(std::format("\t{}();", cls.name));
        for (const ir::Method* m : ctors) out.push_back(std::format("\t{}* j2xInit({});", cls.name, ownership::params_str(*m, resolver)));
        if (ctors.empty()) out.push_back(std::format("\t{}* j2xInit();", cls.name));
    } else {
        for (const ir::Method* m : ctors) out.push_back("\t" + method_decl(cls, *m, resolver));
    }
    if (two_phase) {
        // nothing more
    } else if (resolver.data().values.contains(cls.name) && !ctors.empty() &&
               std::none_of(ctors.begin(), ctors.end(), [](const ir::Method* c) { return c->arity() == 0; })) {
        // A value-type class held by value as a field must be default-constructible; if it only has parameterized
        // constructors, synthesize a (zero-initializing) default one.
        out.push_back(std::format("\t{}();", cls.name));
    } else if (ctors.empty() && (!cls.instance_fields().empty() || !cls.inst_inits.empty())) {
        // A class with NO explicit constructor relies on Java's implicit default one, which zero-inits ALL fields (and
        // runs any field initializers and init-blocks). C++'s compiler-default constructor leaves primitives and pointers
        // as GARBAGE -- the uninitialized-field crash class (GuiScreen had no constructor, so allowUserInput was
        // garbage, the menu dereferenced a NULL player, IP 0x0). So a zero-init default constructor is declared whenever
        // such a class has ANY instance fields (or init-blocks); the source emitter defines it.
        out.push_back(std::format("\t{}();", cls.name));
    }
    if (!cls.is_interface) out.push_back(std::format("\tvirtual ~{}();", cls.name));
}

std::string render_class(const ir::JClass& cls, const types::TypeResolver& resolver, const Options& options) {
    const std::string guard = naming::guard_macro(cls.name);
    Lines out{"#ifndef " + guard, "#define " + guard, "", banner(cls.name, options.cpp_std, options.platform_label), ""};
    add_runtime_includes(out);
    const Dependencies deps = collect_dependencies(cls, resolver);
    add_dependency_lines(out, deps);
    if (!deps.system_includes.empty() || !deps.includes.empty()) out.push_back("");
    for (const std::string& name : deps.forward_decls) out.push_back(std::format("class {};", name));
    if (!deps.forward_decls.empty()) out.push_back("");

    // class declaration
    std::vector<std::string> bases;
    if (cls.parent && !cls.parent->empty()) bases.push_back("public " + base_type_spelling(*cls.parent));
    for (const std::string& i : cls.interfaces) bases.push_back("public " + i);
    out.push_back(std::format("class {}{} {{", cls.name, bases.empty() ? "" : " : " + text::join(bases, ", ")));
    out.push_back("public:");

    add_constructors(out, cls, resolver, options);

    // fields (instance, then static)
    const std::vector<const ir::Field*> inst = cls.instance_fields();
    const std::vector<const ir::Field*> stat = cls.static_fields();
    if (!inst.empty() || !stat.empty()) out.push_back("");
    for (const ir::Field* f : inst) {
        out.push_back(std::format("\t{} {};", field_type_of(cls, *f, resolver, options), naming::field_name(f->name, f->is_static)));
    }
    for (const ir::Field* f : stat) {   // arrays are JArray<T>: a normal member declaration
        out.push_back(std::format("\tstatic {} {};", resolver.field_type(f->type), naming::field_name(f->name, f->is_static)));
    }

    // methods (constructors are already out)
    bool any_method = false;
    for (const ir::Method& m : cls.methods) any_method = any_method || !m.is_ctor;
    if (any_method) out.push_back("");
    for (const ir::Method& m : cls.methods) {
        if (!m.is_ctor) out.push_back("\t" + method_decl(cls, m, resolver));
    }

    // java.lang.Object.toString() is callable through any reference, but a generated INTERFACE has no Object base to
    // inherit it from -- so a `functor->toString()` call would not compile. An interface that does not declare its own
    // toString gets a default one (argo's J_Functor).
    if (cls.is_interface &&
        std::none_of(cls.methods.begin(), cls.methods.end(), [](const ir::Method& m) { return m.name == "toString" && m.params.empty(); })) {
        out.push_back("\tvirtual JavaString toString() { return JavaString(); }");
    }

    // Ordered static init: deferred static field initializers and Java static{} blocks run here, called in dependency
    // order by the generated driver j2x_initAll(). Always declared (empty when nothing is deferred) so the driver can
    // call it on any concrete class.
    if (!cls.is_interface) {
        out.push_back("");
        out.push_back("\tstatic void initStatics();");
    }

    // MI-diamond unification: forwarding overrides so an interface's pure-virtual that a base class already implements
    // concretely is satisfied in THIS class (concrete: NOT '= 0' like the interface's).
    if (!cls.mi_forwards.empty()) {
        out.push_back("");
        for (const ir::MiForward& f : cls.mi_forwards) {
            const ir::Method& im = *f.interface_method;
            out.push_back(std::format("\tvirtual {} {}({});", ownership::return_cpp_type(im, resolver), im.name, ownership::params_str(im, resolver)));
        }
    }

    // Rule of Three: a class that owns heap (freed in its destructor) must forbid copying, else a by-value copy
    // double-frees the pointer -> heap corruption.
    if (ownership::owns_heap(cls, resolver)) {
        out.push_back("");
        out.push_back("private:");
        out.push_back("\t// owns heap; copying disabled so a copy cannot double-free (Rule of Three)");
        out.push_back(std::format("\t{}(const {}&);", cls.name, cls.name));
        out.push_back(std::format("\t{}& operator=(const {}&);", cls.name, cls.name));
        if (options.ledger != nullptr) {
            options.ledger->add(cls.name + ".h", ledger::Category::Risk,
                                "Rule-of-Three: copy ctor + assignment disabled (owns heap) to prevent double-free heap corruption",
                                {.severity = ledger::Severity::Med, .source = "emit", .rule = "rule-of-three-nocopy"});
        }
    }
    out.push_back("};");
    out.push_back("");
    out.push_back("#endif // " + guard);
    return text::join(out, "\n");
}

// What the report shows about a rendered header: it was generated, and each lossy field mapping once per type.
void record_header(const ir::JClass& cls, const types::TypeResolver& resolver, ledger::Ledger& ledger) {
    const std::string file = cls.name + ".h";
    ledger.add(file, ledger::Category::Ported, std::format("generated header for {}", cls.name), {.source = "emit", .rule = "header"});
    std::set<std::string> seen;
    for (const ir::Field& f : cls.fields) {
        const std::optional<std::string_view> note = resolver.port_note(f.type);
        if (note && seen.insert(f.type.name).second) {
            ledger.add(file, ledger::Category::Annotated,
                       std::format("field type {} -> {} ({})", f.type.name, resolver.field_type(f.type), *note),
                       {.line = f.line, .severity = ledger::Severity::Low, .source = "emit", .rule = "type-note"});
        }
    }
}

}  // namespace

CppStd CppStd::of_year(int year) { return {year, std::format("C++{:02}", year % 100)}; }

std::string banner(std::string_view class_name, const std::optional<CppStd>& cpp_std, std::string_view platform_label) {
    const std::string_view std_label = cpp_std ? std::string_view(cpp_std->label) : "C++03";
    const std::string_view platform = platform_label.empty() ? "unknown" : platform_label;
    return std::format("// Auto-generated from {}.java\n// Target: {} / {}", class_name, platform, std_label);
}

std::optional<std::string> std_header_for(std::string_view cpp_base) {
    if (cpp_base.find("JObject") != std::string_view::npos) return "\"JavaRT.h\"";
    if (cpp_base.find("std::string") != std::string_view::npos) return "<string>";
    if (cpp_base.find("std::vector") != std::string_view::npos) return "<vector>";
    if (cpp_base.find("JMap") != std::string_view::npos) return std::nullopt;   // JMap.h is always included (like JList.h and JSet.h)
    if (cpp_base.find("std::map") != std::string_view::npos) return "<map>";
    if (cpp_base.find("std::set") != std::string_view::npos) return "<set>";
    return std::nullopt;
}

std::string base_type_spelling(std::string_view parent) {
    if (!typemap::containers().contains(parent)) return std::string(parent);
    const typemap::NameMap& to_cpp = typemap::java_to_cpp();
    const auto mapped = to_cpp.find(parent);
    const std::string base = mapped == to_cpp.end() ? std::string(parent) : mapped->second;
    const typemap::NameMap& defaults = typemap::raw_container_default();
    const auto suffix = defaults.find(base);
    return suffix == defaults.end() ? base : base + suffix->second;
}

Dependencies collect_dependencies(const ir::JClass& cls, const types::TypeResolver& resolver) {
    Dependencies deps;

    auto note = [&](auto&& self, const ir::JType& t) -> void {
        if (typemap::primitives().contains(t.name)) return;
        const std::string cpp_base = resolver.base_cpp(t.name);
        if (cpp_base != t.name) {   // std-mapped (string/vector/...)
            if (const std::optional<std::string> h = std_header_for(cpp_base)) deps.system_includes.insert(*h);
            for (const ir::JType& a : t.args) self(self, a);   // generic element types still need declarations
            return;
        }
        // a user type with its own X.h
        if (resolver.is_value(t) && !t.is_array()) {
            deps.includes.insert(t.name);   // by value: the full definition
        } else {
            deps.forward_decls.insert(t.name);   // a pointer: forward declare
        }
        for (const ir::JType& a : t.args) self(self, a);
    };
    auto note_type = [&](const ir::JType& t) { note(note, t); };

    if (cls.parent && !cls.parent->empty()) {
        if (typemap::containers().contains(*cls.parent)) {   // class extends ArrayList/HashMap/...: an STL base
            if (const std::optional<std::string> h = std_header_for(base_type_spelling(*cls.parent))) deps.system_includes.insert(*h);
        } else {
            deps.includes.insert(*cls.parent);
        }
    }
    for (const std::string& iface : cls.interfaces) deps.includes.insert(iface);
    for (const ir::Field& f : cls.fields) note_type(f.type);
    for (const ir::Method& m : cls.methods) {
        if (m.return_type) note_type(*m.return_type);
        for (const ir::Param& p : m.params) note_type(p.type);
    }
    for (const ir::MiForward& f : cls.mi_forwards) {   // synthesized forwarding overrides
        const ir::Method& im = *f.interface_method;
        if (im.return_type) note_type(*im.return_type);
        for (const ir::Param& p : im.params) note_type(p.type);
    }

    deps.includes.erase(cls.name);
    deps.forward_decls.erase(cls.name);
    for (const std::string& inc : deps.includes) deps.forward_decls.erase(inc);   // included: no forward declaration too
    return deps;
}

std::string method_decl(const ir::JClass& cls, const ir::Method& method, const types::TypeResolver& resolver) {
    const std::string params = ownership::params_str(method, resolver);
    if (method.is_ctor) return std::format("{}({});", cls.name, params);
    const std::string ret = ownership::return_cpp_type(method, resolver);
    const std::string name = resolver.renamed_method(cls.name, method.name);   // a field/method name collision -> name_
    return std::format("{}{} {}({}){};", method.is_static ? "static " : "virtual ", ret, name, params, method.is_abstract ? " = 0" : "");
}

std::string render_header(const ir::JClass& cls, const types::TypeResolver& resolver, const Options& options) {
    std::string text;
    if (cls.is_enum && resolver.data().typed_enums.contains(cls.name)) {
        text = render_typed_enum(cls, resolver, options);
    } else if (cls.is_enum) {
        text = render_constant_enum(cls, options);
    } else {
        text = render_class(cls, resolver, options);
        if (options.ledger != nullptr) record_header(cls, resolver, *options.ledger);
    }
    return options.cpp_std ? std_transforms::apply_std_transforms(text, options.cpp_std->year) : text;
}

}  // namespace cb::java::emit::header
