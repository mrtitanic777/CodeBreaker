#include "cb/java/emit/source.hpp"

#include <algorithm>
#include <format>
#include <map>

#include "cb/core/text.hpp"
#include "cb/java/emit/naming.hpp"
#include "cb/java/emit/ownership.hpp"
#include "cb/java/emit/std_transforms.hpp"
#include "cb/java/typemap.hpp"

namespace cb::java::emit::source {

namespace {

using ledger::Category;
using ledger::Severity;
using Lines = std::vector<std::string>;

bool src_is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }

void src_append(Lines& out, const Lines& more) { out.insert(out.end(), more.begin(), more.end()); }

Lines src_indented(const Lines& lines, std::string_view indent = "\t") {
    Lines out;
    out.reserve(lines.size());
    for (const std::string& line : lines) out.push_back(std::string(indent) + line);
    return out;
}

std::string src_joined(const Lines& lines) { return text::join(lines, "\n"); }

// Record one event against `<class>.cpp`.
void src_note(ledger::Ledger* ledger, const ir::JClass& cls, Category category, std::string message, std::optional<Severity> severity,
              std::string_view rule, std::uint32_t line = 0) {
    if (ledger == nullptr) return;
    ledger->add(cls.name + ".cpp", category, std::move(message),
                {.line = line, .severity = severity, .source = "emit", .rule = std::string(rule)});
}

// What one rendering reads: the class, the resolver, the options.
struct Writer {
    const ir::JClass& cls;
    const types::TypeResolver& resolver;
    const Options& options;

    BodySource* bodies() const { return options.bodies; }
    bool is_two_phase(std::string_view name) const { return options.two_phase.contains(name); }

    // What the back end could not write: said in the code and the ledger, and thrown if it runs -- never a guess.
    Lines gap(const std::string& what, std::uint32_t line = 0) const {
        src_note(options.ledger, cls, Category::Stubbed,
                 "untranslated " + what + ": the back end declined it -- the body throws UnsupportedOperationException (J2X-GAP)",
                 Severity::High, "untranslated", line);
        return {"// J2X-GAP: " + what + " was not translated (see the ledger)", "throw new UnsupportedOperationException();"};
    }

    Lines init_list() const;
    Lines instance_init_lines() const;
    Lines static_block_lines() const;
    std::pair<Lines, Lines> statics() const;
    Lines ctor(const ir::Method& m) const;
    Lines two_phase_ctors() const;
    Lines dtor() const;
    Lines method(const ir::Method& m) const;
    Lines mi_forward(const ir::Method& interface_method, const std::string& base) const;
    Lines typed_enum_source() const;
};

// The C++ element spelling of a T[] field: the JArray<...> template argument. It must MATCH types::cpp_type's array handling
// exactly, or the aggregate data array (`static elem _data[]`) and the JArray<elem> field type disagree -- byte[] is
// JArray<unsigned char> (a buffer) even though a SCALAR byte is signed char.
std::string src_array_elem_cpp(const ir::JType& type, const types::TypeResolver& resolver) {
    const ir::JType element = type.element();
    if (element.name == "byte" && !element.is_array()) return "unsigned char";
    std::string elem = resolver.cpp_type(element, types::Use::Element);
    if (resolver.ownership(element) == types::Ownership::Pointer && !elem.ends_with('*')) elem += '*';
    return elem;
}

std::string src_banner(const ir::JClass& cls, const Options& options) { return header::banner(cls.name, options.cpp_std, options.platform_label); }

// `Class::name = new Class...` : the field a self-singleton stores its instance in.
std::optional<std::string> src_detect_self_singleton(std::string_view class_name, const Lines& static_init_lines) {
    for (const std::string& raw : static_init_lines) {
        std::string_view line = text::trim(raw);
        if (!line.starts_with(class_name) || !line.substr(class_name.size()).starts_with("::")) continue;
        std::size_t i = class_name.size() + 2;
        const std::size_t field_begin = i;
        while (i < line.size() && src_is_word(line[i])) ++i;
        if (i == field_begin) continue;
        const std::string_view field = line.substr(field_begin, i - field_begin);
        while (i < line.size() && text::is_space(line[i])) ++i;
        if (i >= line.size() || line[i] != '=') continue;
        ++i;
        while (i < line.size() && text::is_space(line[i])) ++i;
        if (!line.substr(i).starts_with("new")) continue;
        i += 3;
        const std::size_t spaces = i;
        while (i < line.size() && text::is_space(line[i])) ++i;
        if (i == spaces || !line.substr(i).starts_with(class_name)) continue;
        i += class_name.size();
        if (i < line.size() && src_is_word(line[i])) continue;   // `\b` after the class name
        return std::string(field);
    }
    return std::nullopt;
}

// Java initialises a class LAZILY on first static access, so a self-singleton getter called mid-init (StatList's
// func_25157_c -> CraftingManager::getInstance(), whose instance is built in a LATER-ordered initStatics()) returned NULL and
// crashed. A linear j2x_initAll cannot reproduce lazy init for a true cycle; emulate it: a `X::s_foo = new X(...)` singleton
// makes its pure getter (`return s_foo;`) build on demand, and initStatics() is idempotent. This guard is put before the
// return of a method that is a PURE getter of the singleton -- one statement, `return field;` -- and never touches a method
// that does other work.
Lines src_inject_lazy_singleton(Lines lines, std::string_view class_name, std::string_view field) {
    const auto open = std::ranges::find(lines, "{");
    if (open == lines.end()) return lines;
    std::vector<std::size_t> statements;
    for (std::size_t i = static_cast<std::size_t>(open - lines.begin()) + 1; i < lines.size(); ++i) {
        const std::string_view t = text::trim(lines[i]);
        if (!t.empty() && t != "}") statements.push_back(i);
    }
    if (statements.size() != 1) return lines;
    const std::size_t i = statements.front();
    const std::string_view statement = text::trim(lines[i]);
    if (statement == std::format("return {};", field) || statement == std::format("return {}::{};", class_name, field)) {
        const std::string indent = lines[i].substr(0, lines[i].size() - std::string(text::ltrim(lines[i])).size());
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(i),
                     std::format("{}if ({} == NULL) initStatics();   // J2X: lazy singleton (Java <clinit> on static access)", indent, field));
    }
    return lines;
}

// Is `name(` called unqualified in the text -- not as a member of another object? A call on `this` counts. The call must
// stand where an identifier begins: not after a word character, `.` or `>`.
bool src_calls_unqualified(std::string_view text, std::string_view name) {
    const auto stands_alone = [&](std::size_t at) {
        if (at == 0) return true;
        const char c = text[at - 1];
        return !(src_is_word(c) || c == '.' || c == '>');
    };
    for (std::size_t at = text.find(name); at != std::string_view::npos; at = text.find(name, at + 1)) {
        std::size_t after = at + name.size();
        while (after < text.size() && text::is_space(text[after])) ++after;
        if (after >= text.size() || text[after] != '(') continue;
        if (stands_alone(at)) return true;
        // `this.name(` or `this->name(`, the `this` standing alone
        std::size_t k = at;
        while (k > 0 && text::is_space(text[k - 1])) --k;
        if (k >= 1 && text[k - 1] == '.') --k;
        else if (k >= 2 && text[k - 1] == '>' && text[k - 2] == '-') k -= 2;
        else continue;
        while (k > 0 && text::is_space(text[k - 1])) --k;
        if (k >= 4 && text.substr(k - 4, 4) == "this" && stands_alone(k - 4)) return true;
    }
    return false;
}

}  // namespace

bool is_multi_dimensional_aggregate(std::string_view s) {
    if (!s.starts_with("new")) return false;
    std::size_t i = 3;
    const std::size_t spaces = i;
    while (i < s.size() && text::is_space(s[i])) ++i;
    if (i == spaces) return false;
    const std::size_t type_begin = i;
    while (i < s.size() && (src_is_word(s[i]) || s[i] == '.')) ++i;
    if (i == type_begin) return false;
    while (i < s.size() && text::is_space(s[i])) ++i;
    std::size_t dims = 0;
    while (i < s.size() && s[i] == '[') {
        ++i;
        while (i < s.size() && text::is_space(s[i])) ++i;
        if (i >= s.size() || s[i] != ']') return false;
        ++i;
        while (i < s.size() && text::is_space(s[i])) ++i;
        ++dims;
    }
    return dims >= 2 && i < s.size() && s[i] == '{';
}

bool is_deferrable_static_init(std::string_view init, std::string_view field_type) {
    // a `const` member cannot be reassigned in initStatics(): it stays eager
    const std::string_view head = field_type.substr(0, field_type.find('<'));
    for (const std::string_view word : text::split_ws(head)) {
        if (word == "const") return false;
    }
    if (init.find("::") != std::string_view::npos || init.find("->") != std::string_view::npos) return true;
    for (std::size_t at = init.find("new"); at != std::string_view::npos; at = init.find("new", at + 1)) {   // the word `new`
        const bool starts = at == 0 || !src_is_word(init[at - 1]);
        const bool ends = at + 3 >= init.size() || !src_is_word(init[at + 3]);
        if (starts && ends) return true;
    }
    // a call to a lowercase-initial function name (not a PascalCase constructor)
    for (std::size_t i = 0; i < init.size(); ++i) {
        if (init[i] < 'a' || init[i] > 'z') continue;
        if (i > 0 && (src_is_word(init[i - 1]) || init[i - 1] == ':' || init[i - 1] == '.')) continue;
        std::size_t j = i + 1;
        while (j < init.size() && src_is_word(init[j])) ++j;
        while (j < init.size() && text::is_space(init[j])) ++j;
        if (j < init.size() && init[j] == '(') return true;
    }
    return false;
}

std::optional<std::string> default_value(std::string_view cpp_type) {
    static const std::set<std::string, std::less<>> primitives = {"int",   "short", "long long", "char",  "unsigned char",
                                                                   "signed char", "float", "double",    "bool",  "long"};
    if (cpp_type.ends_with('*')) return "NULL";
    if (primitives.contains(cpp_type)) return "0";
    return std::nullopt;
}

std::vector<std::string> referenced_types(const ir::JClass& cls, const types::TypeResolver& resolver, std::string_view body_text) {
    std::set<std::string, std::less<>> names;
    const auto add = [&names](const auto& self, const ir::JType& type) -> void {
        names.insert(type.name);
        for (const ir::JType& arg : type.args) self(self, arg);
    };

    if (cls.parent) names.insert(*cls.parent);
    names.insert(cls.interfaces.begin(), cls.interfaces.end());
    for (const ir::Field& f : cls.fields) add(add, f.type);
    for (const ir::Field* f : cls.inherited_fields) add(add, f->type);
    for (const ir::Method& m : cls.methods) {
        if (m.return_type) add(add, *m.return_type);
        for (const ir::Param& p : m.params) add(add, p.type);
    }
    // capitalised identifiers of the body: new X / X:: / casts / locals
    for (std::size_t i = 0; i < body_text.size();) {
        if (!src_is_word(body_text[i])) { ++i; continue; }
        std::size_t j = i;
        while (j < body_text.size() && src_is_word(body_text[j])) ++j;
        if (body_text[i] >= 'A' && body_text[i] <= 'Z') names.insert(std::string(body_text.substr(i, j - i)));
        i = j;
    }
    if (body_text.find("j2xClassOf") != std::string_view::npos || body_text.find("typeid") != std::string_view::npos) {
        names.insert("Class");   // `.class` / getClass() needs Class.h
    }
    // The transitive CLOSURE: the statement translator emits arbitrarily deep chains, so a .cpp needs the FULL definition of every
    // type reachable through field and method links, not one hop. Iterate to a fixpoint over the resolved member environment.
    std::set<std::string, std::less<>> seen;
    std::vector<std::string> frontier(names.begin(), names.end());
    while (!frontier.empty()) {
        const std::string n = std::move(frontier.back());
        frontier.pop_back();
        if (!seen.insert(n).second) continue;
        const auto env = resolver.data().members.find(n);
        if (env == resolver.data().members.end()) continue;
        for (const auto& [field, type] : env->second.fields) add(add, type);
        for (const auto& [method, type] : env->second.methods) add(add, type);
        for (const std::string& name : names) {
            if (!seen.contains(name)) frontier.push_back(name);
        }
    }
    std::vector<std::string> out;
    for (const std::string& name : names) {
        if (name == cls.name) continue;
        if (resolver.data().known.contains(name) || typemap::runtime_headers().contains(name)) out.push_back(name);
    }
    return out;   // sorted: a std::set
}

// ---------------------------------------------------------------------------------------------- the pieces of a class

Lines Writer::init_list() const {
    Lines items;
    for (const ir::Field* f : cls.instance_fields()) {
        if (const auto dv = default_value(resolver.field_type(f->type))) items.push_back(std::format("{}({})", naming::field_name(f->name, f->is_static), *dv));
    }
    return items;
}

// Java instance field initialisers and instance `{ }` blocks, in declaration order. Java runs these at the top of every
// constructor that does NOT delegate via this(...); dropping them was the null-deref / uninitialised-field crash class
// (EntityPlayer.inventory).
Lines Writer::instance_init_lines() const {
    if (bodies() != nullptr) {
        if (auto tree = bodies()->instance_inits(cls)) return std::move(*tree);
    }
    const auto fields = cls.instance_fields();
    const bool any_initializer = std::ranges::any_of(fields, [](const ir::Field* f) { return f->initializer.has_value(); });
    if (!any_initializer && cls.inst_inits.empty()) return {};
    return gap("instance initialisers of " + cls.name);
}

Lines Writer::static_block_lines() const {
    if (cls.static_inits.empty()) return {};
    if (bodies() != nullptr) {
        const auto tree = bodies()->static_blocks(cls);
        if (tree && tree->size() == cls.static_inits.size()) {
            Lines out;
            for (const Lines& block : *tree) src_append(out, block);
            return out;
        }
    }
    return gap("static {} blocks of " + cls.name);
}

// The static field DEFINITIONS (zero-initialised for the deferrable ones) and the DEFERRED initialiser statements that
// initStatics() must run in dependency order.
std::pair<Lines, Lines> Writer::statics() const {
    Lines defs, inits;
    for (const ir::Field* f : cls.static_fields()) {
        const std::string name = naming::field_name(f->name, f->is_static);
        const std::string ft = resolver.field_type(f->type);
        const std::string_view init_raw = f->initializer ? text::trim(*f->initializer) : std::string_view();
        std::optional<std::string> tree;
        if (bodies() != nullptr && !init_raw.empty()) tree = bodies()->field_init(cls, *f);
        std::optional<Lines> items;
        if (bodies() != nullptr && ownership::aggregate_array_body(*f)) items = bodies()->array_items(cls, *f);
        const bool multi = is_multi_dimensional_aggregate(init_raw);
        // a multi-dimensional aggregate is written from the back end's expression: without it there is nothing to write
        if (!init_raw.empty() && !tree && (!items || multi)) {
            defs.push_back(std::format("{} {}::{};", ft, cls.name, name));
            src_append(inits, gap(std::format("static field {}.{}", cls.name, f->name), f->line));
            continue;
        }
        if (multi) {   // new T[][]{...} -> nested JArrayInit (C++03), built in initStatics(): it allocates, so it is no constant
            defs.push_back(std::format("{} {}::{};", ft, cls.name, name));   // zero/default now (order-safe)
            inits.push_back(std::format("{}::{} = {};", cls.name, name, *tree));
            src_note(options.ledger, cls, Category::Risk,
                     "multi-dim static array new T[][]{...} -> nested JArrayInit (C++03 has no braced new[][]; each level length-tracked)",
                     Severity::Low, "static-aggregate-array-nd", f->line);
            continue;
        }
        if (items) {   // new T[]{a,b,c} -> a file-static data array and a JArray over it
            const std::string data = std::format("_{}_{}_data", cls.name, name);
            defs.push_back(std::format("static {} {}[] = {{ {} }};", src_array_elem_cpp(f->type, resolver), data, text::join(*items, ", ")));
            defs.push_back(std::format("{} {}::{}({}, {});", ft, cls.name, name, data, items->size()));
            src_note(options.ledger, cls, Category::Risk,
                     "static array new T[]{...} -> JArray over a file-static data array (C++03 has no braced new[]; length tracked)",
                     Severity::Low, "static-aggregate-array", f->line);
            continue;
        }
        // a forward-declared static (no initialiser) is NULL / 0 / default
        const std::optional<std::string> init = !init_raw.empty() ? tree : default_value(ft);
        if (!init) {
            defs.push_back(std::format("{} {}::{};", ft, cls.name, name));
        } else if (is_deferrable_static_init(*init, ft)) {
            defs.push_back(std::format("{} {}::{};", ft, cls.name, name));            // zero-init now (order-safe)
            inits.push_back(std::format("{}::{} = {};", cls.name, name, *init));      // initialise in the ordered initStatics()
        } else {
            defs.push_back(std::format("{} {}::{} = {};", ft, cls.name, name, *init));   // a compile-time constant: eager
        }
    }
    return {std::move(defs), std::move(inits)};
}

Lines Writer::ctor(const ir::Method& m) const {
    const std::string params = ownership::params_str(m, resolver);
    Lines items, body;
    std::optional<CtorBody> tree;
    if (bodies() != nullptr) tree = bodies()->ctor(cls, &m, false);
    if (tree) {
        if (tree->super_args && cls.parent) items.push_back(std::format("{}({})", *cls.parent, *tree->super_args));
        src_append(items, init_list());
        body = instance_init_lines();
        src_append(body, tree->lines);
    } else {
        items = init_list();
        body = gap(std::format("constructor {}({})", cls.name, params), m.line);
    }
    Lines out = {std::format("{}::{}({})", cls.name, cls.name, params)};
    if (!items.empty()) out.push_back("\t: " + text::join(items, "\n\t, "));
    out.emplace_back("{");
    src_append(out, src_indented(body));
    out.emplace_back("}");
    return out;
}

// A two-phase class (twophase.hpp): `C::C()` zeroes the fields and each Java constructor becomes `C* C::j2xInit(params)` -- the
// parent's j2xInit (Java's super(...)), the instance initialisers, the body, `return this`. this(args) is a call to the other
// j2xInit, which does the parent and the initialisers.
Lines Writer::two_phase_ctors() const {
    const bool parent_two_phase = cls.parent && is_two_phase(*cls.parent);
    const Lines items = init_list();
    Lines out = {std::format("{}::{}()", cls.name, cls.name)};
    if (!items.empty()) out.push_back("\t: " + text::join(items, "\n\t, "));
    src_append(out, {"{", "}", ""});
    std::vector<const ir::Method*> ctors = cls.constructors();
    if (ctors.empty()) ctors.push_back(nullptr);
    for (const ir::Method* m : ctors) {
        const std::string params = m != nullptr ? ownership::params_str(*m, resolver) : std::string();
        std::optional<CtorBody> tree;
        if (bodies() != nullptr) tree = bodies()->ctor(cls, m, true);
        out.push_back(std::format("{}* {}::j2xInit({})", cls.name, cls.name, params));
        out.emplace_back("{");
        if (tree) {
            Lines lines;
            if (tree->call != CtorCall::This && parent_two_phase) {
                lines.push_back(std::format("{}::j2xInit({});", *cls.parent, tree->super_args.value_or("")));   // super(...), or Java's implicit one
            }
            if (tree->call != CtorCall::This) src_append(lines, instance_init_lines());
            src_append(lines, tree->lines);
            src_append(out, src_indented(lines));
            src_append(out, {"\treturn this;", "}", ""});
        } else {
            src_append(out, src_indented(gap(std::format("constructor {}({})", cls.name, params), m != nullptr ? m->line : 0)));
            src_append(out, {"}", ""});
        }
    }
    src_note(options.ledger, cls, Category::Rule,
             "two-phase construction: constructors are j2xInit(...) so their virtual calls reach the override, as in Java", Severity::Low,
             "two-phase-ctor");
    out.pop_back();
    return out;
}

// The destructor of a class that allocates heap in its constructors (the Rule of Three): it frees what the class owns.
Lines Writer::dtor() const {
    const auto owned = ownership::owned_pointers(cls, resolver);
    const auto containers = ownership::owned_containers(cls, resolver);
    // container ELEMENTS first (an element may reference a scalar-owned sibling), then scalar pointers; each by name
    const std::map<std::string, std::string> container_by_name(containers.begin(), containers.end());
    const std::map<std::string, bool> owned_by_name(owned.begin(), owned.end());
    Lines out = {std::format("{}::~{}()", cls.name, cls.name), "{"};
    for (const auto& [name, suffix] : container_by_name) {
        out.push_back(std::format("\tfor (int _i = 0; _i < {}{}; ++_i) {{ delete {}[_i]; }}", name, suffix, name));
    }
    for (const auto& [name, is_array] : owned_by_name) {
        out.push_back(std::format("\tdelete{} {};", is_array ? "[]" : "", name));
        out.push_back(std::format("\t{} = NULL;", name));
    }
    out.emplace_back("}");
    Lines freed;
    for (const auto& [name, unused] : owned_by_name) freed.push_back(name);
    for (const auto& [name, unused] : container_by_name) freed.push_back(name);
    std::ranges::sort(freed);
    if (!freed.empty()) {
        src_note(options.ledger, cls, Category::Risk,
                 std::format("Rule-of-Three: synthesized destructor frees {} heap-owning field(s): {}", freed.size(), text::join(freed, ", ")),
                 Severity::Med, "rule-of-three-dtor");
    }
    return out;
}

Lines Writer::method(const ir::Method& m) const {
    const std::string ret = ownership::return_cpp_type(m, resolver);
    const std::string params = ownership::params_str(m, resolver);
    const std::string name = resolver.renamed_method(cls.name, m.name);   // a field/method name collision -> name_
    Lines out = {std::format("{} {}::{}({})", ret, cls.name, name, params), "{"};
    std::optional<Lines> tree;
    if (bodies() != nullptr) tree = bodies()->body(cls, m);
    src_append(out, src_indented(tree ? *tree : gap("method " + m.name, m.line)));
    out.emplace_back("}");
    return out;
}

// A synthesized MI-diamond forwarding override: `R Cls::m(args) { [return] Base::m(args); }`, so an interface pure-virtual
// already implemented by a base class is satisfied here too.
Lines Writer::mi_forward(const ir::Method& interface_method, const std::string& base) const {
    const std::string ret = ownership::return_cpp_type(interface_method, resolver);
    const std::string params = ownership::params_str(interface_method, resolver);
    Lines names;
    for (const ir::Param& p : interface_method.params) names.push_back(p.name);
    src_note(options.ledger, cls, Category::Rule,
             std::format("MI-diamond: synthesized {}() forwarding to {}:: (interface decl + base impl unified into one vtable override)",
                         interface_method.name, base),
             Severity::Low, "mi-forward");
    return {std::format("{} {}::{}({})", ret, cls.name, interface_method.name, params), "{",
            std::format("\t{}{}::{}({});", ret == "void" ? "" : "return ", base, interface_method.name, text::join(names, ", ")), "}"};
}

// The .cpp of a typed enum: the static const/instance definitions, the constructor(s), destructor and methods through the
// normal renderers, and an initStatics() that builds the singleton instance table (in constructor-body dependency order via
// j2x_initAll), assigns ordinals and fills values().
Lines Writer::typed_enum_source() const {
    Lines body;
    for (const ir::EnumConst& ec : cls.enum_consts) body.push_back(std::format("{}* {}::{};", cls.name, cls.name, naming::enum_member(ec.name)));
    for (const ir::Field* f : cls.static_fields()) body.push_back(std::format("{} {}::{};", resolver.field_type(f->type), cls.name, f->name));
    body.push_back(std::format("JArray<{}* > {}::_j2x_values;", cls.name, cls.name));
    body.emplace_back();

    for (const ir::Method* m : cls.constructors()) {
        src_append(body, ctor(*m));
        body.emplace_back();
    }
    src_append(body, dtor());
    body.emplace_back();
    for (const ir::Method& m : cls.methods) {
        if (m.is_ctor || m.is_abstract) continue;
        src_append(body, method(m));
        body.emplace_back();
    }
    src_append(body, {std::format("int {}::ordinal()", cls.name), "{", "\treturn this->__ordinal;", "}", ""});
    src_append(body, {std::format("JArray<{}* >& {}::values()", cls.name, cls.name), "{", "\treturn _j2x_values;", "}", ""});

    // initStatics(): construct each singleton, set its ordinal, run the real static initialisers, fill values()
    Lines init;
    for (std::size_t i = 0; i < cls.enum_consts.size(); ++i) {
        const ir::EnumConst& ec = cls.enum_consts[i];
        std::optional<Lines> args;
        if (bodies() != nullptr) args = bodies()->enum_args(cls, ec);
        if (!args) {
            src_append(init, gap(std::format("enum constant {}.{}", cls.name, ec.name)));
            continue;
        }
        const std::string member = naming::enum_member(ec.name);
        init.push_back(std::format("{} = new {}({});", member, cls.name, text::join(*args, ", ")));
        init.push_back(std::format("{}->__ordinal = {};", member, i));
    }
    for (const ir::Field* f : cls.static_fields()) {
        if (!f->initializer) continue;
        std::optional<std::string> value;
        if (bodies() != nullptr) value = bodies()->field_init(cls, *f);
        if (!value) src_append(init, gap(std::format("static field {}.{}", cls.name, f->name)));
        else init.push_back(std::format("{} = {};", f->name, *value));
    }
    src_append(init, static_block_lines());
    init.push_back(std::format("_j2x_values = JArray<{}* >({});", cls.name, cls.enum_consts.size()));
    for (std::size_t i = 0; i < cls.enum_consts.size(); ++i) init.push_back(std::format("_j2x_values[{}] = {};", i, naming::enum_member(cls.enum_consts[i].name)));
    src_append(body, {std::format("void {}::initStatics()", cls.name), "{"});
    src_append(body, src_indented(init));
    src_append(body, {"}", ""});

    const std::vector<std::string> used = referenced_types(cls, resolver, src_joined(body));
    Lines out = {"#include \"stdafx.h\"", "#include \"JavaRT.h\"", "", src_banner(cls, options), "", "#include \"" + cls.name + ".h\""};
    for (const std::string& n : used) out.push_back("#include \"" + n + ".h\"");
    out.emplace_back();
    src_append(out, body);
    src_note(options.ledger, cls, Category::Ported, std::format("typed enum source: {} singletons built in initStatics()", cls.enum_consts.size()),
             std::nullopt, "typed-enum");
    return out;
}

// ---------------------------------------------------------------------------------------------- render_source

namespace {

// '\n'-joined lines, trailing whitespace dropped, one final newline, and the standard's rewrites.
std::string src_finish(const Lines& lines, const Options& options) {
    std::string text = src_joined(lines);
    while (!text.empty() && text::is_space(text.back())) text.pop_back();
    text += '\n';
    return options.cpp_std ? std_transforms::apply_std_transforms(text, options.cpp_std->year) : text;
}

}  // namespace

Rendered render_source(const ir::JClass& cls, const types::TypeResolver& resolver, const Options& options) {
    const Writer w{cls, resolver, options};
    Rendered result;
    if (cls.is_enum && resolver.data().typed_enums.contains(cls.name)) {
        result.text = src_finish(w.typed_enum_source(), options);
        result.has_init_statics = true;   // always: the constant table must be built
        return result;
    }
    if (cls.is_interface || cls.is_enum) return result;

    Lines body;
    auto [static_defs, static_init_lines] = w.statics();
    const std::optional<std::string> singleton_field = src_detect_self_singleton(cls.name, static_init_lines);   // lazy-init emulation
    if (!static_defs.empty()) {
        src_append(body, static_defs);
        body.emplace_back();
    }
    const bool two_phase = w.is_two_phase(cls.name);
    const std::vector<const ir::Method*> ctors = cls.constructors();
    if (two_phase) {
        src_append(body, w.two_phase_ctors());
        body.emplace_back();
    } else {
        for (const ir::Method* m : ctors) {
            src_append(body, w.ctor(*m));
            body.emplace_back();
        }
    }
    const auto no_arg_ctor = std::ranges::any_of(ctors, [](const ir::Method* c) { return c->arity() == 0; });
    if (!two_phase && resolver.data().values.contains(cls.name) && !ctors.empty() && !no_arg_ctor) {
        // a zero-initialising default constructor for a value type that lacks one, so it can be held by value as a field; it
        // matches the declaration header.cpp writes
        const Lines items = w.init_list();
        Lines ctor = {std::format("{}::{}()", cls.name, cls.name)};
        if (!items.empty()) ctor.push_back("\t: " + text::join(items, "\n\t, "));
        src_append(ctor, {"{", "}"});
        src_append(body, ctor);
        body.emplace_back();
    } else if (!two_phase && ctors.empty() && (!cls.instance_fields().empty() || !cls.inst_inits.empty())) {
        // A class with NO explicit constructor relies on Java's implicit default one, which zero-initialises ALL fields and runs
        // any initialisers and init blocks. C++'s implicit constructor leaves primitives and pointers as garbage (the
        // uninitialised-field crash class), so a zero-initialising default constructor is written whenever the class has any
        // instance fields or init blocks. It matches the declaration header.cpp writes.
        const Lines items = w.init_list();
        const Lines inits = w.instance_init_lines();
        Lines ctor = {std::format("{}::{}()", cls.name, cls.name)};
        if (!items.empty()) ctor.push_back("\t: " + text::join(items, "\n\t, "));
        ctor.emplace_back("{");
        src_append(ctor, src_indented(inits));
        ctor.emplace_back("}");
        src_append(body, ctor);
        body.emplace_back();
    }
    src_append(body, w.dtor());
    body.emplace_back();
    for (const ir::Method& m : cls.methods) {
        if (m.is_ctor || m.is_abstract) continue;
        Lines lines = w.method(m);
        if (singleton_field) lines = src_inject_lazy_singleton(std::move(lines), cls.name, *singleton_field);
        src_append(body, lines);
        body.emplace_back();
    }
    // An abstract method CALLED from a constructor (Entity's constructor calls the abstract entityInit()) needs an out-of-line
    // definition: from the base constructor the call resolves NON-virtually to THIS class's version, so a pure `= 0` with no body
    // is an unresolved external. A pure virtual may have a body; an empty one is written (subclasses still override it). Java would
    // dispatch to the override here, but C++ calls the base during base construction -- the standard virtual-call-in-constructor
    // divergence.
    std::string ctor_text;
    for (const ir::Method* c : ctors) {
        for (const ir::SourceLine& line : c->body) ctor_text += line.text + "\n";
    }
    for (const ir::Method& m : cls.methods) {
        if (!m.is_abstract || !src_calls_unqualified(ctor_text, m.name)) continue;
        const std::string rt = ownership::return_cpp_type(m, resolver);
        const std::string signature = std::format("{} {}::{}({})", rt, cls.name, m.name, ownership::params_str(m, resolver));
        if (rt == "void") {
            src_append(body, {signature, "{", "}", ""});
        } else {
            const std::optional<std::string> dv = default_value(rt);
            src_append(body, {signature, "{", std::format("\treturn {};", dv ? *dv : rt + "()"), "}", ""});
        }
        src_note(options.ledger, cls, Category::Risk,
                 std::format("abstract {}() is called from a constructor -- emitted an empty base definition (C++ calls the base, not the "
                             "override, during base construction)", m.name),
                 Severity::Med, "abstract-called-in-ctor", m.line);
    }
    for (const ir::MiForward& forward : cls.mi_forwards) {
        src_append(body, w.mi_forward(*forward.interface_method, forward.base_class));
        body.emplace_back();
    }

    // ORDERED STATIC INIT: the deferred static field initialisers and the Java static{} blocks are run by a single
    // `void <Class>::initStatics()`, which the generated driver (j2x_init.cpp / j2x_initAll()) calls in dependency order -- NOT by
    // a file-local trigger, whose cross-translation-unit order is undefined (the static-init-order crash class: Block initialising
    // before Material -> a null vtable). The static field DEFINITIONS above are zero-initialised; initStatics() assigns the values.
    Lines init_body = static_init_lines;           // the deferred static FIELD initialisers first
    src_append(init_body, w.static_block_lines());   // then the Java static{} block bodies
    // initStatics() is ALWAYS written (empty when nothing is deferred), so the driver can call it on any class without a
    // header/source mismatch; header.cpp declares it for every concrete class to match.
    result.has_init_statics = !init_body.empty();
    src_append(body, {std::format("void {}::initStatics()", cls.name), "{"});
    if (singleton_field && !init_body.empty()) {   // idempotent: a lazy getter may have already built the singleton
        body.push_back(std::format("\tif ({} != NULL) return;   // J2X: idempotent (may be lazily inited via a singleton getter)", *singleton_field));
    }
    src_append(body, src_indented(init_body));
    src_append(body, {"}", ""});
    if (!init_body.empty()) {
        src_note(options.ledger, cls, Category::Risk,
                 "static field inits + static{} blocks deferred into initStatics(), run in dependency order by j2x_initAll() (kills the "
                 "static-init-order crash class)",
                 Severity::Med, "static-init-ordered");
    }

    // A .cpp needs the FULL definition of every user type it touches (member access, new/cast/static), not just the forward
    // declarations in its own header: the class's signatures catch types reached only through lowercase variables (`entity->posX`
    // never spells `Entity`), and the capitalised tokens of the body catch the rest.
    const std::vector<std::string> used = referenced_types(cls, resolver, src_joined(body));
    Lines out = {"#include \"stdafx.h\"", "#include \"JavaRT.h\"", "", src_banner(cls, options), "", "#include \"" + cls.name + ".h\""};
    for (const std::string& n : used) out.push_back("#include \"" + n + ".h\"");
    out.emplace_back();
    src_append(out, body);

    src_note(options.ledger, cls, Category::Ported, "generated source for " + cls.name, std::nullopt, "source");
    result.text = src_finish(out, options);
    return result;
}

}  // namespace cb::java::emit::source
