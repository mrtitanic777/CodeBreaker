// translate/backends/c_java_method.cpp -- the members the C back end for Java writes: a method or constructor (and its X_new),
// the field initialisers of a class (X_finit), its static initialiser (X_clinit) (see c_java.hpp).

#include <algorithm>
#include <format>

#include "core/text.hpp"
#include "translate/backends/c_java_internal.hpp"

namespace cb::translate::backends::c_java::detail {

MethodWriter::MethodWriter(Program& program, const ClassInfo* cls, const ir::Node* method)
    : p_(program), ci_(cls), node_(method), m_(method != nullptr ? &method_of(method) : nullptr) {
    static_ = m_ != nullptr && is_static(*m_);
    if (ci_ != nullptr) {
        for (const jmodel::Capture& c : ci_->captures) captures_[c.name] = "self->cap_" + ident(c.name);
    }
}

// ---- members
Lines MethodWriter::define() {
    const ir::MethodDecl& m = *m_;
    vol_ = any_node(single(m.body), [](const ir::Node* n) { return ir::is<ir::Try>(n); });
    Lines out;
    if (m.kind == "ctor") {
        const std::string name = p_.ctor_name(*ci_, node_);
        out.push_back(std::format("static void {}({}) {{", name, join_commas(p_.ctor_params(*ci_, m))));
        for (const ir::Node* pn : m.params) {
            const ir::Param& param = param_of(pn);
            vars_[ident(need_sym(param.sym, "a parameter").name)] = p_.ct(param.type);
        }
        const std::vector<ir::Node*>& body = body_statements(m);
        const ir::CtorCall* first = body.empty() ? nullptr : ir::get<ir::CtorCall>(body[0]);
        const std::span<ir::Node* const> rest = first != nullptr ? std::span<ir::Node* const>(body).subspan(1) : std::span<ir::Node* const>(body);
        std::string first_kind;
        if (first != nullptr) {
            if (!first->kind) throw Decline("a constructor call of no kind");
            first_kind = *first->kind;
        }
        const bool delegated = first != nullptr && first_kind.starts_with("this");
        if (ci_->kind == "enum") {
            out.emplace_back("\t((jrt_Enum*)self)->name = name_in;");
            out.emplace_back("\t((jrt_Enum*)self)->ordinal = ordinal_in;");
        }
        if (first != nullptr && (!first_kind.ends_with(":implicit") || delegated)) {
            Lines call = ctor_call(*first, "\t");
            out.insert(out.end(), call.begin(), call.end());
        } else if (ci_->base && !ci_->base->empty() && *ci_->base != jmodel::kObject && ci_->kind != "enum") {
            if (const ClassInfo* b = p_.model().info(*ci_->base)) {
                const ir::Node* implicit = nullptr;   // the base's constructor with no parameters
                for (const ir::Node* x : b->methods()) {
                    if (method_of(x).kind == "ctor" && method_of(x).params.empty()) {
                        implicit = x;
                        break;
                    }
                }
                // javac's constructor of an anonymous class takes the arguments of its `new` and hands them to the base
                const ir::Node* chosen = nullptr;
                if (ci_->anon && !m.params.empty()) {
                    for (const ir::Node* x : b->methods()) {
                        const ir::MethodDecl& c = method_of(x);
                        if (c.kind != "ctor" || c.params.size() != m.params.size()) continue;
                        bool same = true;
                        for (std::size_t i = 0; i < c.params.size(); ++i) same = same && p_.ct(param_of(c.params[i]).type) == p_.ct(param_of(m.params[i]).type);
                        if (same) {
                            chosen = x;
                            break;
                        }
                    }
                    if (chosen == nullptr) throw Decline("the base constructor of an anonymous class");
                }
                if (chosen != nullptr) {
                    std::string forwarded;
                    for (const ir::Node* pn : m.params) forwarded += ", " + ident(need_sym(param_of(pn).sym, "a parameter").name);
                    out.push_back(std::format("\t{}(({}*)self{});", p_.ctor_name(*b, chosen), b->cname, forwarded));
                } else if (implicit != nullptr) {
                    out.push_back(std::format("\t{}(({}*)self);", p_.ctor_name(*b, implicit), b->cname));
                }
            }
        }
        if (!delegated) out.push_back(std::format("\t{}_finit(self);", ci_->cname));
        Lines statements;
        stmts(rest, statements, "\t");
        out.insert(out.end(), statements.begin(), statements.end());
        out.emplace_back("}");
        out.emplace_back("");
        // X_new: allocate, then run the constructor
        out.push_back(std::format("static {}* {}({}) {{", ci_->cname, text::replace_first(name, "_init", "_new"),
                                  decl_params(p_.new_params(*ci_, &m))));
        if (p_.needs_clinit(*ci_)) out.push_back(std::format("\t{}_clinit();", ci_->cname));
        out.push_back(std::format("\t{}* o = ({}*)jrt_alloc(&{}_class);", ci_->cname, ci_->cname, ci_->cname));
        if (ci_->needs_outer) out.emplace_back("\to->outer_ = outer_in;");
        for (const jmodel::Capture& c : ci_->captures) out.push_back(std::format("\to->cap_{} = cap_{};", ident(c.name), ident(c.name)));
        std::vector<std::string> call{"o"};
        if (ci_->kind == "enum") {
            call.emplace_back("name_in");
            call.emplace_back("ordinal_in");
        }
        for (const ir::Node* pn : m.params) call.push_back(ident(need_sym(param_of(pn).sym, "a parameter").name));
        out.push_back(std::format("\t{}({});", name, join_commas(call)));
        out.emplace_back("\treturn o;");
        out.emplace_back("}");
        out.emplace_back("");
        return out;
    }
    const Sig s = p_.sig(*ci_, node_);
    ret_ = s.r;
    std::vector<std::string> params, entry;
    if (!static_) {
        params.emplace_back("jobject* self_");
        entry.push_back(ci_->kind != "interface" ? std::format("\t{}* self = ({}*)self_;", ci_->cname, ci_->cname)
                                                 : std::string("\tjobject* self = self_;"));
    }
    const std::size_t n = std::min(s.ps.size(), m.params.size());
    for (std::size_t i = 0; i < n; ++i) {
        const std::string& declared = s.ps[i];   // the root's parameter type: what callers pass
        const ir::Param& param = param_of(m.params[i]);
        const std::string own = p_.ct(param.type);
        const std::string name = ident(need_sym(param.sym, "a parameter").name);
        vars_[name] = own;
        if (own != declared) {
            params.push_back(std::format("{} {}_in", declared, name));
            entry.push_back(std::format("\t{} {} = {};", own, name, conv(name + "_in", declared, own)));
        } else {
            params.push_back(std::format("{} {}", declared, name));
        }
    }
    const std::string fname = is_object_method(m) && !static_ ? std::format("{}_{}", ci_->cname, *m.name)
                                                              : std::format("{}_{}", ci_->cname, p_.mname(m.name.value_or(""), s.eps));
    out.push_back(std::format("static {} {}({}) {{", s.r, fname, decl_params(params)));
    if (static_ && p_.needs_clinit(*ci_)) out.push_back(std::format("\t{}_clinit();", ci_->cname));
    out.insert(out.end(), entry.begin(), entry.end());
    Lines statements;
    stmts(body_statements(m), statements, "\t");
    out.insert(out.end(), statements.begin(), statements.end());
    out.emplace_back("}");
    out.emplace_back("");
    return out;
}

Lines MethodWriter::ctor_call(const ir::CtorCall& call, std::string_view ind) {
    if (!call.kind) throw Decline("a constructor call of no kind");
    if (call.kind->starts_with("this")) {
        const auto target = call.sym ? p_.model().method_decl(*call.sym) : std::nullopt;
        if (!target) throw Decline("this(...) target");
        std::vector<std::string> parameter_types;
        for (const ir::Node* pn : method_of(target->method).params) parameter_types.push_back(p_.ct(param_of(pn).type));
        const std::string a = args(call.args, parameter_types, call.sym);
        std::vector<std::string> all{"self"};
        if (ci_->kind == "enum") {
            all.emplace_back("name_in");
            all.emplace_back("ordinal_in");
        }
        if (!a.empty()) all.push_back(a);
        return {std::format("{}{}({});", ind, p_.ctor_name(*ci_, target->method), join_commas(all))};
    }
    if (!ci_->base || ci_->base->empty() || *ci_->base == jmodel::kObject) return {};
    const ClassInfo* b = p_.model().info(*ci_->base);
    if (b == nullptr) {   // a runtime Throwable
        std::vector<std::string> as;
        for (std::size_t i = 0; i < call.args.size(); ++i) {
            const Code c = expr(call.args[i]);
            as.push_back(conv(c.code, c.ctype, i == 0 ? kStr : kObj));
        }
        const ir::TypeP t0 = as.size() == 1 ? type_of(call.args[0]) : ir::TypeP();
        if (as.size() == 1 && t0 && class_name(t0) != "java.lang.String")
            return {std::format("{}jrt_Throwable_init_cause((jobject*)self, NULL, {});", ind, as[0])};
        if (as.size() == 2) return {std::format("{}jrt_Throwable_init_cause((jobject*)self, {}, {});", ind, as[0], as[1])};
        return {std::format("{}jrt_Throwable_init((jobject*)self, {});", ind, !as.empty() ? as[0] : std::string("NULL"))};
    }
    const auto target = call.sym ? p_.model().method_decl(*call.sym) : std::nullopt;
    if (!target) throw Decline("super(...) target");
    std::vector<std::string> parameter_types;
    for (const ir::Node* pn : method_of(target->method).params) parameter_types.push_back(p_.ct(param_of(pn).type));
    const std::string a = args(call.args, parameter_types, call.sym);
    std::vector<std::string> all{std::format("({}*)self", b->cname)};
    if (!a.empty()) all.push_back(a);
    return {std::format("{}{}({});", ind, p_.ctor_name(*b, target->method), join_commas(all))};
}

Lines MethodWriter::anon_new() {
    const ClassInfo* b = ci_->base ? p_.model().info(*ci_->base) : nullptr;
    const auto* creation = ir::get<ir::New>(ci_->creation);
    std::vector<std::string> params = p_.new_params(*ci_, nullptr);
    std::optional<std::string> super_call;
    if (b != nullptr && b->kind != "interface" && creation != nullptr && creation->sym) {
        if (const auto target = p_.model().method_decl(*creation->sym)) {
            std::string tail;
            std::size_t i = 0;
            for (const ir::Node* pn : method_of(target->method).params) {
                params.push_back(std::format("{} a{}", p_.ct(param_of(pn).type), i));
                tail += std::format(", a{}", i);
                ++i;
            }
            super_call = std::format("\t{}(({}*)o{});", p_.ctor_name(*b, target->method), b->cname, tail);
        }
    }
    Lines out;
    out.push_back(std::format("static {}* {}_new({}) {{", ci_->cname, ci_->cname, decl_params(params)));
    if (p_.needs_clinit(*ci_)) out.push_back(std::format("\t{}_clinit();", ci_->cname));
    out.push_back(std::format("\t{}* o = ({}*)jrt_alloc(&{}_class);", ci_->cname, ci_->cname, ci_->cname));
    if (ci_->needs_outer) out.emplace_back("\to->outer_ = outer_in;");
    for (const jmodel::Capture& c : ci_->captures) out.push_back(std::format("\to->cap_{} = cap_{};", ident(c.name), ident(c.name)));
    if (super_call) out.push_back(*super_call);
    out.push_back(std::format("\t{}_finit(o);", ci_->cname));
    out.emplace_back("\treturn o;");
    out.emplace_back("}");
    out.emplace_back("");
    return out;
}

Lines MethodWriter::finit() {
    // the instance field initialisers and initialiser blocks, in the order the class has them
    Lines body;
    for (const ir::Node* x : ci_->members()) {
        if (const auto* f = ir::get<ir::FieldDecl>(x)) {
            if (f->init == nullptr || !f->sym || f->sym->is_static) continue;
            ir::Node* self = ir::make(scratch(), ir::This{});
            ir::Node* target = ir::make(scratch(), ir::Select{.obj = self, .sym = f->sym}, f->type);
            ir::Node* assign = ir::make(scratch(), ir::Assign{.target = target, .value = f->init}, f->type);
            stmt(ir::make(scratch(), ir::ExprStmt{.expr = assign}), body, "\t");
        } else if (const auto* m = ir::get<ir::MethodDecl>(x); m != nullptr && m->kind == "init") {
            stmts(body_statements(*m), body, "\t");
        }
    }
    Lines out = {std::format("static void {}_finit({}* self) {{", ci_->cname, ci_->cname)};
    out.insert(out.end(), body.begin(), body.end());
    out.emplace_back("}");
    out.emplace_back("");
    return out;
}

Lines MethodWriter::clinit() {
    static_ = true;
    Lines out = {std::format("static void {}_clinit(void) {{", ci_->cname), std::format("\tif ({}_inited_) return;", ci_->cname),
                 std::format("\t{}_inited_ = true;", ci_->cname)};
    const ClassInfo* base = ci_->base ? p_.model().info(*ci_->base) : nullptr;
    if (base != nullptr && p_.needs_clinit(*base)) out.push_back(std::format("\t{}_clinit();", base->cname));
    std::vector<const ir::EnumConst*> consts;
    for (const ir::Node* x : ci_->members()) {
        if (const auto* ec = ir::get<ir::EnumConst>(x)) consts.push_back(ec);
    }
    for (std::size_t k = 0; k < consts.size(); ++k) {
        const ir::EnumConst& ec = *consts[k];
        const ClassInfo* sub = ec.body != nullptr ? p_.model().of_decl(ec.body) : nullptr;   // a constant with a body is a subclass
        const ClassInfo* target = sub != nullptr ? sub : ci_;
        const ir::Node* ctor = nullptr;
        if (ec.ctor) {
            if (const auto match = p_.model().method_decl(*ec.ctor)) ctor = match->method;
        }
        std::vector<std::string> parameter_types;
        if (ctor != nullptr) {
            for (const ir::Node* pn : method_of(ctor).params) parameter_types.push_back(p_.ct(param_of(pn).type));
        }
        const std::string a = args(ec.args, parameter_types, ec.ctor);
        const std::string constant_name = need_sym(ec.sym, "an enum constant").name;
        std::vector<std::string> lead{p_.lit(constant_name), std::to_string(k)};
        if (!a.empty()) lead.push_back(a);
        if (sub != nullptr) {
            out.push_back(std::format("\t{}_{} = ({}*){}_new({});", ci_->cname, ident(constant_name), ci_->cname, target->cname, join_commas(lead)));
        } else {
            const std::string make = ctor != nullptr ? text::replace_first(p_.ctor_name(*ci_, ctor), "_init", "_new") : ci_->cname + "_new";
            out.push_back(std::format("\t{}_{} = {}({});", ci_->cname, ident(constant_name), make, join_commas(lead)));
        }
    }
    if (ci_->kind == "enum") {
        out.push_back(std::format("\t{}_values_ = jrt_new_array(sizeof(jobject*), {});", ci_->cname, consts.size()));
        for (std::size_t k = 0; k < consts.size(); ++k) {
            out.push_back(std::format("\tJA(jobject*, {}_values_, {}) = (jobject*){}_{};", ci_->cname, k, ci_->cname,
                                      ident(consts[k]->sym->name)));
        }
    }
    for (const ir::Node* x : ci_->members()) {
        if (const auto* f = ir::get<ir::FieldDecl>(x)) {
            if (!f->sym || !f->sym->is_static || f->init == nullptr || jmodel::constant_init(*f)) continue;
            ir::Node* type_name = ir::make(scratch(), ir::TypeName{}, ir::class_type(ci_->qname));
            ir::Node* target = ir::make(scratch(), ir::Select{.obj = type_name, .sym = f->sym}, f->type);
            ir::Node* assign = ir::make(scratch(), ir::Assign{.target = target, .value = f->init}, f->type);
            stmt(ir::make(scratch(), ir::ExprStmt{.expr = assign}), out, "\t");
        } else if (const auto* m = ir::get<ir::MethodDecl>(x); m != nullptr && m->kind == "static_init") {
            stmts(body_statements(*m), out, "\t");
        }
    }
    out.emplace_back("}");
    out.emplace_back("");
    return out;
}

std::string MethodWriter::constant(const ir::FieldDecl& field) {
    const Code c = expr(field.init);
    return conv_to(c, field.type, p_.ct(field.type));
}

}  // namespace cb::translate::backends::c_java::detail
