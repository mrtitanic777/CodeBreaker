// translate/backends/cpp03_ctor.cpp -- constructors, field initialisers, initialiser blocks and method bodies from the IR: the entry
// points the body source calls (see cpp03.hpp).

#include <algorithm>
#include <format>

#include "core/text.hpp"
#include "translate/backends/cpp03_internal.hpp"

namespace cb::translate::backends::cpp03 {

using detail::block_stmts;
using detail::need_sym;
using detail::single;

namespace {
namespace ctor_impl {

std::string param_key(const ir::TypeP& type) {
    const jir::JType jt = jtype(type);
    std::string key = jt.name;
    for (std::uint32_t i = 0; i < jt.array_dims; ++i) key += "[]";
    return key;
}

// The constructor this(...) calls: (its IR constructor, the converter's parsed one).
struct Delegate {
    const ir::Node* method;
    const jir::Method* parsed;
};

Delegate delegate_target(Context& ctx, const ir::ClassDecl& cls, const ir::CtorCall& call) {
    if (!call.sym) throw Decline("unresolved this(...)");
    std::vector<std::string> want;
    for (const ir::TypeP& p : call.sym->params) want.push_back(param_key(p));
    for (const ir::Node* node : cls.members) {
        const auto* m = ir::get<ir::MethodDecl>(node);
        if (m == nullptr || !m->kind || *m->kind != "ctor" || m->params.size() != want.size()) continue;
        std::vector<std::string> got;
        std::vector<ir::TypeP> types;
        for (const ir::Node* p : m->params) {
            const auto* param = ir::get<ir::Param>(p);
            if (param == nullptr) throw Decline("a constructor parameter that is no Param");
            got.push_back(param_key(param->type));
            types.push_back(param->type);
        }
        if (got != want) continue;
        const jir::Method* parsed = ctx.parsed_ctor(cls.name.value_or("None"), types);
        if (parsed == nullptr) throw Decline("delegate target not parsed");
        return {node, parsed};
    }
    throw Decline("delegate target not found");
}

// A constructor's first statement if it is a this(...) / super(...) call, and the statements after it.
struct SplitCtor {
    const ir::CtorCall* first = nullptr;
    std::vector<const ir::Node*> rest;
};

SplitCtor split_ctor(const ir::MethodDecl& m) {
    SplitCtor out;
    if (std::holds_alternative<ir::Node*>(m.body) && std::get<ir::Node*>(m.body) == nullptr) return out;
    const auto& stmts = block_stmts(single(m.body));
    out.rest.assign(stmts.begin(), stmts.end());
    if (!out.rest.empty()) out.first = ir::get<ir::CtorCall>(out.rest[0]);
    if (out.first != nullptr) out.rest.erase(out.rest.begin());
    return out;
}

bool kind_starts(const ir::CtorCall& call, std::string_view prefix) { return call.kind.value_or("").starts_with(prefix); }
bool kind_ends(const ir::CtorCall& call, std::string_view suffix) { return call.kind.value_or("").ends_with(suffix); }

// (kind, arguments) of the base initialiser the delegation chain ends in, written with each target's parameters replaced by the
// (parenthesised) arguments it was called with.
std::pair<std::optional<std::string>, std::optional<std::string>> delegated_super(Dialect dialect, Context& ctx, const ir::Node& cls_node,
                                                                                  Writer& w, const ir::CtorCall& call, int depth) {
    if (depth > 8) throw Decline("delegation too deep");
    const ir::ClassDecl& cls = *ir::get<ir::ClassDecl>(&cls_node);
    const Delegate target = delegate_target(ctx, cls, call);
    const auto tw = make_writer(dialect, ctx, cls_node, target.method, target.parsed);
    const auto& tparams = ir::get<ir::MethodDecl>(target.method)->params;
    const std::size_t n = std::min(tparams.size(), call.args.size());
    for (std::size_t i = 0; i < n; ++i) {
        const auto* p = ir::get<ir::Param>(tparams[i]);
        tw->alias[tw->name(need_sym(p->sym, "parameter").name)] = "(" + w.expr(call.args[i]).code + ")";
    }
    const SplitCtor split = split_ctor(*ir::get<ir::MethodDecl>(target.method));
    if (split.first != nullptr && kind_starts(*split.first, "this")) return delegated_super(dialect, ctx, cls_node, *tw, *split.first, depth + 1);
    if (split.first != nullptr && !kind_ends(*split.first, ":implicit")) {
        if (split.first->target != nullptr) throw Decline("qualified super call");
        std::vector<std::string> parts;
        for (const ir::Node* a : split.first->args) parts.push_back(tw->expr(a).code);
        return {"super", detail::join_commas(parts)};
    }
    return {std::nullopt, std::nullopt};
}

// The delegated-to constructor's body, in a block: its parameters as locals holding the arguments.
Lines delegated_body(Dialect dialect, Context& ctx, const ir::Node& cls_node, Writer& w, const ir::CtorCall& call, int depth) {
    if (depth > 8) throw Decline("delegation too deep");
    const ir::ClassDecl& cls = *ir::get<ir::ClassDecl>(&cls_node);
    const Delegate target = delegate_target(ctx, cls, call);
    const auto tw = make_writer(dialect, ctx, cls_node, target.method, target.parsed);
    Lines lines{"{"};
    const auto& tparams = ir::get<ir::MethodDecl>(target.method)->params;
    const std::size_t n = std::min(tparams.size(), call.args.size());
    for (std::size_t i = 0; i < n; ++i) {
        const auto* p = ir::get<ir::Param>(tparams[i]);
        const std::string name = tw->name(need_sym(p->sym, "parameter").name);
        const auto declared = tw->vars.find(name);
        if (declared == tw->vars.end()) throw Decline("delegate parameter " + name + " not declared");
        const std::string temp = std::format("j2xD{}_{}", depth, name);
        const Code arg = w.expr(call.args[i]);
        const std::string local = detail::strip_const_ref(declared->second);
        lines.push_back(std::format("\t{} {} = {};", local, temp, unbox(coerce(arg.code, local), arg.ctype, local)));
        tw->vars[temp] = local;
        tw->alias[name] = temp;
    }
    const SplitCtor split = split_ctor(*ir::get<ir::MethodDecl>(target.method));
    if (split.first != nullptr && kind_starts(*split.first, "this")) {
        for (const std::string& x : delegated_body(dialect, ctx, cls_node, *tw, *split.first, depth + 1)) lines.push_back("\t" + x);
    }
    for (const ir::Node* st : split.rest) tw->stmt(st, lines, "\t");
    lines.push_back("}");
    return lines;
}

}  // namespace ctor_impl
}  // namespace

CtorParts ctor_parts(Dialect dialect, Context& ctx, const ir::Node& cls_node, const ir::Node& method, const jir::Method* parsed,
                     bool two_phase) {
    const auto* m = ir::get<ir::MethodDecl>(&method);
    const auto* cls = ir::get<ir::ClassDecl>(&cls_node);
    CB_ASSERT(m != nullptr && cls != nullptr, "a constructor is a MethodDecl of a ClassDecl");
    if (has_unsupported(&method)) throw Decline("unsupported construct");
    const auto w = make_writer(dialect, ctx, cls_node, &method, parsed);
    ctor_impl::SplitCtor split = ctor_impl::split_ctor(*m);
    const ir::CtorCall* first = split.first;
    std::optional<std::string> args;
    if (first != nullptr && !ctor_impl::kind_ends(*first, ":implicit")) {
        if (first->target != nullptr) throw Decline("qualified super call");
        std::vector<std::string> parts;
        for (const ir::Node* a : first->args) parts.push_back(w->expr(a).code);
        args = detail::join_commas(parts);
    }
    Lines out;
    if (two_phase) {
        w->two_phase = true;
        w->ret = cls->name.value_or("None") + "*";
        if (first != nullptr && ctor_impl::kind_starts(*first, "this")) {
            out.push_back(std::format("{}::j2xInit({});", cls->name.value_or("None"), args ? *args : std::string()));
            args = std::nullopt;
        }
    } else if (first != nullptr && ctor_impl::kind_starts(*first, "this")) {
        // C++03 has no delegation: the other constructor's base initialiser comes here, with its parameters replaced by this call's
        // arguments, and its body runs first, in a block of its own
        const auto [kind, delegated_args] = ctor_impl::delegated_super(dialect, ctx, cls_node, *w, *first, 0);
        for (const std::string& x : ctor_impl::delegated_body(dialect, ctx, cls_node, *w, *first, 0)) out.push_back(x);
        for (const ir::Node* s : split.rest) w->stmt(s, out, "");
        return CtorParts{kind, delegated_args, std::move(out)};
    }
    for (const ir::Node* s : split.rest) w->stmt(s, out, "");
    std::optional<std::string> kind;
    if (first != nullptr) { const std::string first_kind = first->kind.value_or(""); kind = std::string(text::split(first_kind, ':')[0]); }
    return CtorParts{kind, args, std::move(out)};
}

std::string field_init(Dialect dialect, Context& ctx, const ir::Node& cls_node, const ir::Node& field) {
    const auto* f = ir::get<ir::FieldDecl>(&field);
    const auto* cls = ir::get<ir::ClassDecl>(&cls_node);
    CB_ASSERT(f != nullptr && cls != nullptr, "a field initialiser is a FieldDecl of a ClassDecl");
    if (f->init == nullptr || has_unsupported(&field)) throw Decline("field initialiser");
    const auto w = make_writer(dialect, ctx, cls_node, nullptr, nullptr);
    const Code written = w->expr(f->init);
    const std::string owner = cls->name.value_or("None");
    const std::string& field_name = need_sym(f->sym, "field").name;
    const std::optional<std::string> target = ctx.field_ctype(owner, field_name);
    if (!target) throw Decline("field " + owner + "." + field_name);
    return unbox(coerce(written.code, *target), written.ctype, *target);
}

Lines block_lines(Dialect dialect, Context& ctx, const ir::Node& cls_node, const ir::Node& method) {
    const auto* m = ir::get<ir::MethodDecl>(&method);
    CB_ASSERT(m != nullptr, "an initialiser block is a MethodDecl");
    if (has_unsupported(&method)) throw Decline("unsupported construct");
    const auto w = make_writer(dialect, ctx, cls_node, nullptr, nullptr);
    Lines out;
    for (const ir::Node* s : block_stmts(single(m->body))) w->stmt(s, out, "");
    return out;
}

Lines method_body(Dialect dialect, Context& ctx, const ir::Node& cls_node, const ir::Node& method, const jir::Method* parsed) {
    if (has_unsupported(&method)) throw Decline("unsupported construct");
    return make_writer(dialect, ctx, cls_node, &method, parsed)->body();
}

}  // namespace cb::translate::backends::cpp03
