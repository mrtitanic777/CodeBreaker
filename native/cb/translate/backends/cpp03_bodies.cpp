// cb/translate/backends/cpp03_bodies.cpp -- the C++03 back end as the converter's body source: it finds each member the source emitter asks
// for in the lowered units, writes it, and counts what it wrote and why it declined the rest (see cpp03.hpp).

#include <algorithm>
#include <format>

#include "cb/core/text.hpp"
#include "cb/translate/backends/cpp03_internal.hpp"
#include "cb/translate/desugar.hpp"

namespace cb::translate::backends::cpp03 {

using cb::java::emit::source::CtorBody;
using cb::java::emit::source::CtorCall;
using detail::need_sym;

namespace {
namespace bodies_impl {

std::string param_key(const jir::JType& type) {
    std::string key = type.name;
    for (std::uint32_t i = 0; i < type.array_dims; ++i) key += "[]";
    return key;
}

// The keys of an IR method's parameter types; nullopt where one is a type C++03 has no word for.
std::optional<std::vector<std::string>> ir_param_keys(const ir::MethodDecl& m) {
    std::vector<std::string> keys;
    try {
        for (const ir::Node* p : m.params) {
            const auto* param = ir::get<ir::Param>(p);
            if (param == nullptr) return std::nullopt;
            keys.push_back(param_key(jtype(param->type)));
        }
    } catch (const Decline&) {
        return std::nullopt;
    }
    return keys;
}

std::vector<std::string> parsed_param_keys(const jir::Method& m) {
    std::vector<std::string> keys;
    for (const jir::Param& p : m.params) keys.push_back(param_key(p.type));
    return keys;
}

// the first field named `name` of `c`, or null
const ir::Node* find_field(const ir::ClassDecl& c, std::string_view name) {
    for (const ir::Node* x : c.members) {
        const auto* f = ir::get<ir::FieldDecl>(x);
        if (f != nullptr && f->sym && f->sym->name == name) return x;
    }
    return nullptr;
}

std::string first_word(std::string_view why) { return std::string(why.substr(0, why.find(' '))); }

Lines keep_nulls(Lines lines) {
    for (std::string& line : lines) line = keep_null(line);
    return lines;
}

}  // namespace bodies_impl
}  // namespace

Bodies::Bodies(Context& ctx, Arena& arena, std::span<ir::Node* const> units, Dialect dialect) : ctx_(ctx), dialect_(dialect) {
    // The forms the C++17 writer has a word for stay; every other modern form is lowered to what C++ can say.
    static constexpr desugar::Form kKeep17[] = {desugar::Form::ForEach, desugar::Form::SwitchExpr};
    desugared_ = desugar::run(arena, units, dialect == Dialect::Cpp17 ? std::span<const desugar::Form>(kKeep17) : std::span<const desugar::Form>());
    for (const ir::Node* un : units) {
        const auto* u = ir::get<ir::Unit>(un);
        if (u == nullptr) throw Decline("a program holding something that is no Unit");
        for (const ir::Node* type : u->types) {
            const auto* c = ir::get<ir::ClassDecl>(type);
            if (c == nullptr) throw Decline("a unit holding something that is no class");
            decls_.emplace(c->name.value_or("None"), type);   // the first class of a name wins
        }
    }
    std::set<std::string, std::less<>> qualified;
    for (const auto& [name, node] : decls_) {
        const auto& q = ir::get<ir::ClassDecl>(node)->qname;
        if (q && !q->empty()) qualified.insert(*q);
    }
    ctx_.set_program_classes(std::move(qualified));
}

std::vector<std::string> Bodies::class_names() const {
    std::vector<std::string> names;
    for (const auto& [name, node] : decls_) names.push_back(name);
    std::sort(names.begin(), names.end());
    return names;
}

const ir::Node* Bodies::find_class(std::string_view name) const {
    const auto it = decls_.find(std::string(name));
    return it == decls_.end() ? nullptr : it->second;
}

const ir::Node* Bodies::find_method(const ir::Node& cls, const jir::Method& parsed) const {
    const std::vector<std::string> want = bodies_impl::parsed_param_keys(parsed);
    for (const ir::Node* node : ir::get<ir::ClassDecl>(&cls)->members) {
        const auto* m = ir::get<ir::MethodDecl>(node);
        if (m == nullptr || !m->kind || *m->kind != "method" || m->name != parsed.name || m->params.size() != parsed.params.size()) continue;
        const auto got = bodies_impl::ir_param_keys(*m);
        if (got && *got == want) return node;
    }
    return nullptr;
}

const ir::Node* Bodies::find_ctor(const ir::Node& cls, const jir::Method* parsed) const {
    std::vector<const ir::Node*> ctors;
    for (const ir::Node* node : ir::get<ir::ClassDecl>(&cls)->members) {
        const auto* m = ir::get<ir::MethodDecl>(node);
        if (m != nullptr && m->kind && *m->kind == "ctor") ctors.push_back(node);
    }
    if (parsed == nullptr) {   // the implicit default constructor
        for (const ir::Node* node : ctors) {
            if (ir::get<ir::MethodDecl>(node)->params.empty()) return node;
        }
        return nullptr;
    }
    const std::vector<std::string> want = bodies_impl::parsed_param_keys(*parsed);
    for (const ir::Node* node : ctors) {
        const auto* m = ir::get<ir::MethodDecl>(node);
        if (m->params.size() != parsed->params.size()) continue;
        const auto got = bodies_impl::ir_param_keys(*m);
        if (got && *got == want) return node;
    }
    return nullptr;
}

std::optional<CtorBody> Bodies::ctor(const jir::JClass& cls, const jir::Method* parsed, bool two_phase) {
    const ir::Node* c = find_class(cls.name);
    const ir::Node* m = c != nullptr ? find_ctor(*c, parsed) : nullptr;
    if (m == nullptr) {
        count(declined_, "ctor not found");
        return std::nullopt;
    }
    CtorParts parts;
    try {
        parts = ctor_parts(dialect_, ctx_, *c, *m, parsed, two_phase);
    } catch (const Decline& e) {
        const std::string why = e.what();
        count(declined_, "ctor " + bodies_impl::first_word(why));
        count(reasons_, "ctor: " + why);
        return std::nullopt;
    }
    ++written_ctors_;
    CtorBody out;
    if (parts.kind == "super") out.call = CtorCall::Super;
    else if (parts.kind == "this") out.call = CtorCall::This;
    if (parts.args) out.super_args = dialect_ == Dialect::Cpp17 ? keep_null(*parts.args) : *parts.args;
    out.lines = dialect_ == Dialect::Cpp17 ? bodies_impl::keep_nulls(std::move(parts.body)) : std::move(parts.body);
    return out;
}

std::optional<std::string> Bodies::field_init(const jir::JClass& cls, const jir::Field& field) {
    const ir::Node* c = find_class(cls.name);
    if (c == nullptr) return std::nullopt;
    const ir::Node* f = bodies_impl::find_field(*ir::get<ir::ClassDecl>(c), field.name);
    if (f == nullptr || ir::get<ir::FieldDecl>(f)->init == nullptr) return std::nullopt;
    try {
        std::string code = cpp03::field_init(dialect_, ctx_, *c, *f);
        return dialect_ == Dialect::Cpp17 ? keep_null(code) : code;
    } catch (const Decline& e) {
        count(reasons_, std::string("field init: ") + e.what());
        return std::nullopt;
    }
}

std::optional<std::vector<std::string>> Bodies::enum_args(const jir::JClass& cls, const jir::EnumConst& constant) {
    const ir::Node* c = find_class(cls.name);
    if (c == nullptr) return std::nullopt;
    const ir::EnumConst* e = nullptr;
    for (const ir::Node* x : ir::get<ir::ClassDecl>(c)->members) {
        const auto* ec = ir::get<ir::EnumConst>(x);
        if (ec != nullptr && ec->sym && ec->sym->name == constant.name) {
            e = ec;
            break;
        }
    }
    if (e == nullptr || e->body != nullptr) return std::nullopt;
    try {
        const auto w = make_writer(dialect_, ctx_, *c, nullptr, nullptr);
        Lines out;
        for (const ir::Node* a : w->unmarked(e->args)) out.push_back(w->arg(a));
        return dialect_ == Dialect::Cpp17 ? bodies_impl::keep_nulls(std::move(out)) : out;
    } catch (const Decline& ex) {
        count(reasons_, std::string("enum constant: ") + ex.what());
        return std::nullopt;
    }
}

std::optional<std::vector<std::string>> Bodies::array_items(const jir::JClass& cls, const jir::Field& field) {
    const ir::Node* c = find_class(cls.name);
    if (c == nullptr) return std::nullopt;
    const ir::Node* f = bodies_impl::find_field(*ir::get<ir::ClassDecl>(c), field.name);
    const ir::Node* init_node = f != nullptr ? ir::get<ir::FieldDecl>(f)->init : nullptr;
    const auto* init = ir::get<ir::NewArray>(init_node);
    if (init == nullptr || !init->dims.empty()) return std::nullopt;   // (a sized array has no items)
    try {
        const auto w = make_writer(dialect_, ctx_, *c, nullptr, nullptr);
        const std::optional<std::string> declared = ctx_.field_ctype(cls.name, field.name);
        const std::string elem = array_element(declared && !declared->empty() ? *declared : w->ctype(init_node->type));
        Lines out;
        for (const ir::Node* x : init->init) {
            const Code written = w->expr(x);
            out.push_back(unbox(coerce(written.code, elem), written.ctype, elem));
        }
        return dialect_ == Dialect::Cpp17 ? bodies_impl::keep_nulls(std::move(out)) : out;
    } catch (const Decline& ex) {
        count(reasons_, std::string("array items: ") + ex.what());
        return std::nullopt;
    }
}

std::optional<std::vector<std::string>> Bodies::instance_inits(const jir::JClass& cls) {
    const ir::Node* c = find_class(cls.name);
    if (c == nullptr) return std::nullopt;
    Lines out;
    try {
        for (const ir::Node* x : ir::get<ir::ClassDecl>(c)->members) {
            if (const auto* f = ir::get<ir::FieldDecl>(x); f != nullptr && f->init != nullptr && f->sym && !f->sym->is_static) {
                if (f->type && f->type->kind() == "array") {
                    const auto* lit = ir::get<ir::Lit>(f->init);
                    if (lit != nullptr && std::holds_alternative<std::monostate>(lit->value)) continue;   // an array field = null: JArray starts empty
                }
                out.push_back(std::format("this->{} = {};", f->sym->name, cpp03::field_init(dialect_, ctx_, *c, *x)));
            } else if (const auto* md = ir::get<ir::MethodDecl>(x); md != nullptr && md->kind && *md->kind == "init") {
                const Lines block = block_lines(dialect_, ctx_, *c, *x);
                out.insert(out.end(), block.begin(), block.end());
            }
        }
    } catch (const Decline& e) {
        count(reasons_, std::string("instance init: ") + e.what());
        return std::nullopt;
    }
    return dialect_ == Dialect::Cpp17 ? bodies_impl::keep_nulls(std::move(out)) : out;
}

std::optional<std::vector<std::vector<std::string>>> Bodies::static_blocks(const jir::JClass& cls) {
    const ir::Node* c = find_class(cls.name);
    if (c == nullptr) return std::nullopt;
    try {
        std::vector<Lines> out;
        for (const ir::Node* x : ir::get<ir::ClassDecl>(c)->members) {
            const auto* md = ir::get<ir::MethodDecl>(x);
            if (md == nullptr || !md->kind || *md->kind != "static_init") continue;
            Lines block = block_lines(dialect_, ctx_, *c, *x);
            out.push_back(dialect_ == Dialect::Cpp17 ? bodies_impl::keep_nulls(std::move(block)) : std::move(block));
        }
        return out;
    } catch (const Decline& e) {
        count(reasons_, std::string("static block: ") + e.what());
        return std::nullopt;
    }
}

std::optional<std::vector<std::string>> Bodies::body(const jir::JClass& cls, const jir::Method& method) {
    const ir::Node* c = find_class(cls.name);
    const ir::Node* m = c != nullptr ? find_method(*c, method) : nullptr;
    if (m == nullptr) {
        count(declined_, "not found");
        return std::nullopt;
    }
    Lines lines;
    try {
        lines = method_body(dialect_, ctx_, *c, *m, &method);
    } catch (const Decline& e) {
        const std::string why = e.what();
        count(declined_, why.empty() ? std::string("declined") : bodies_impl::first_word(why));
        count(reasons_, why);
        return std::nullopt;
    }
    ++written_;
    return dialect_ == Dialect::Cpp17 ? bodies_impl::keep_nulls(std::move(lines)) : lines;
}

}  // namespace cb::translate::backends::cpp03
