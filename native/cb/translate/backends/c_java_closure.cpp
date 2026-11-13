// cb/translate/backends/c_java_closure.cpp -- lambdas and method references in the C back end for Java: a closure record (a struct
// holding the captured values) implementing the functional interface through its own itab, and `X_new` to make one (see c_java.hpp).

#include <algorithm>
#include <format>
#include <unordered_set>

#include "cb/core/text.hpp"
#include "cb/translate/backends/c_java_internal.hpp"

namespace cb::translate::backends::c_java::detail {

Code MethodWriter::closure(const ir::Node* e) {
    if (const auto* lambda = ir::get<ir::Lambda>(e)) return on(*e, *lambda);
    return on(*e, *ir::get<ir::MethodRef>(e));
}

// The functional interface a lambda or reference of type `type` implements: its one method as the runtime calls it.
SamInfo MethodWriter::sam(const ir::TypeP& type) {
    if (kind_of(type) != "class") throw Decline("lambda without a functional interface type");
    const std::string name(class_name(type));
    const std::optional<jmodel::Signature> single_method = p_.model().functional(name);
    if (!single_method) throw Decline("not a functional interface: " + name);
    if (p_.model().info(name) != nullptr) {   // a program's own interface: its slot for that method
        std::vector<IMethod> ms = p_.iface_methods(name);
        for (const IMethod& x : ms) {
            if (x.java == single_method->name) return SamInfo{name, x.name, x.ps, x.r, ms};
        }
        throw Decline("a functional interface whose method the table does not have: " + name);
    }
    std::vector<std::string> cps;
    for (const ir::TypeP& x : single_method->params) cps.push_back(p_.ct(jmodel::erase(x)));
    const std::string cr = p_.ct(jmodel::erase(single_method->result));
    return SamInfo{name, single_method->name, cps, cr, {IMethod{single_method->name, cps, cr, single_method->name, single_method->params}}};
}

Code MethodWriter::on(const ir::Node& n, const ir::Lambda& k) {
    const SamInfo si = sam(n.type);
    if (si.ms.size() != 1) throw Decline("a lambda for an interface with default methods");
    const std::string name = std::format("Lambda{}", ++p_.lambda_count);

    // what the body reads from outside: locals and parameters of the enclosing code, and `this`
    std::unordered_set<std::string> inner;
    for (const ir::Node* x : k.params) inner.insert(ident(need_sym(param_of(x).sym, "a lambda parameter").name));
    std::vector<const ir::Node*> walked;
    for_each_node(k.body, [&](const ir::Node* x) { walked.push_back(x); });
    for (const ir::Node* x : walked) {
        if (const auto* local = ir::get<ir::Local>(x); local != nullptr && local->sym) inner.insert(ident(local->sym->name));
    }
    std::vector<Cap> caps;
    std::unordered_set<std::string> seen;
    bool uses_this = false;
    for (const ir::Node* x : walked) {
        if (const auto* nm = ir::get<ir::Name>(x); nm != nullptr && nm->sym) {
            const std::string key = ident(nm->sym->name);
            if (!inner.contains(key) && !seen.contains(key)) {
                const Code c = on(*x, *nm);
                caps.push_back(Cap{key, c.code, c.ctype});
                seen.insert(key);
            }
        }
        if (ir::is<ir::This>(x) || ir::is<ir::Super>(x)) uses_this = true;
        if (const auto* call = ir::get<ir::Call>(x); call != nullptr && call->target == nullptr && call->sym && !call->sym->is_static) uses_this = true;
        if (const auto* sel = ir::get<ir::Select>(x); sel != nullptr && ir::is<ir::This>(sel->obj)) uses_this = true;
    }
    Code this_code;
    if (uses_this) this_code = this_ref();

    Lines fields = {"\tjobject hdr_;"};
    for (const Cap& c : caps) fields.push_back(std::format("\t{} cap_{};", c.ct, c.name));
    if (uses_this) fields.push_back(std::format("\t{} cap_this;", this_code.ctype));

    // the body is written as a function of its own, reading what it captured from the record
    MethodWriter body(p_, ci_, nullptr);
    std::unordered_map<std::string, Code> captured;
    for (const Cap& c : caps) captured[c.name] = Code{"self->cap_" + c.name, c.ct};
    if (uses_this) captured["#this"] = Code{"self->cap_this", this_code.ctype};
    body.lam_ = std::move(captured);
    body.static_ = static_;
    body.ret_ = si.r;
    std::vector<std::string> params{"jobject* self_"};
    Lines entry = {std::format("\tstruct {}* self = (struct {}*)self_;", name, name), "\t(void)self;"};
    for (std::size_t i = 0; i < k.params.size() && i < si.ps.size(); ++i) {
        const ir::Param& param = param_of(k.params[i]);
        const std::string& want = si.ps[i];
        const std::string own = p_.ct(param.type);
        const std::string pname = ident(need_sym(param.sym, "a lambda parameter").name);
        body.vars_[pname] = own;
        if (own != want) {
            params.push_back(std::format("{} {}_in", want, pname));
            entry.push_back(std::format("\t{} {} = {};", own, pname, body.conv(pname + "_in", want, own)));
        } else {
            params.push_back(std::format("{} {}", want, pname));
        }
    }
    Lines out;
    if (const auto* block = ir::get<ir::Block>(k.body)) {
        body.vol_ = any_node(k.body, [](const ir::Node* x) { return ir::is<ir::Try>(x); });
        body.stmts(block->stmts, out, "\t");
    } else if (si.r != "void") {
        body.stmt(ir::make(scratch(), ir::Return{.expr = k.body}), out, "\t");
    } else {
        body.stmt(ir::make(scratch(), ir::ExprStmt{.expr = k.body}), out, "\t");
    }

    const ClassInfo* qi = p_.model().info(si.q);
    const std::string vt = qi != nullptr ? std::format("struct {}_vt", qi->cname) : std::format("jrt_{}_vt", after_dot(si.q));
    const std::string cls_ref = p_.class_ref(si.q);
    Lines unit_text = {std::format("struct {} {{", name)};
    unit_text.insert(unit_text.end(), fields.begin(), fields.end());
    unit_text.emplace_back("};");
    unit_text.push_back(std::format("static {} {}_fn({}) {{", si.r, name, join_commas(params)));
    unit_text.insert(unit_text.end(), entry.begin(), entry.end());
    unit_text.insert(unit_text.end(), out.begin(), out.end());
    unit_text.emplace_back("}");
    unit_text.push_back(std::format("static const {} {}_vt = {{{{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}}, {}_fn}};", vt, name, name));
    unit_text.push_back(std::format("static const jitab {}_itabs[] = {{{{{}, &{}_vt}}}};", name, cls_ref, name));
    unit_text.push_back(std::format("static const jclass {}_class = {{\"{}$$Lambda\", &jrt_Object_class, sizeof(struct {}), &jrt_Object_vt, JRT_ITABS({}_itabs), 0}};",
                               name, text::trim(ci_->qname, "<>"), name, name));
    std::vector<std::string> ctor_params;
    for (const Cap& c : caps) ctor_params.push_back(std::format("{} c_{}", c.ct, c.name));
    if (uses_this) ctor_params.push_back(std::format("{} c_this", this_code.ctype));
    unit_text.push_back(std::format("static jobject* {}_new({}) {{", name, decl_params(ctor_params)));
    unit_text.push_back(std::format("\tstruct {}* o = (struct {}*)jrt_alloc(&{}_class);", name, name, name));
    for (const Cap& c : caps) unit_text.push_back(std::format("\to->cap_{} = c_{};", c.name, c.name));
    if (uses_this) unit_text.emplace_back("\to->cap_this = c_this;");
    unit_text.emplace_back("\treturn (jobject*)o;");
    unit_text.emplace_back("}");
    unit_text.emplace_back("");
    p_.lambdas.insert(p_.lambdas.end(), unit_text.begin(), unit_text.end());

    std::vector<std::string> as;
    for (const Cap& c : caps) as.push_back(c.code);
    if (uses_this) as.push_back(this_code.code);
    return {std::format("{}_new({})", name, join_commas(as)), std::string(kObj)};
}

// The arguments of a method reference's call: each of the lambda's parameters, cast where the target's parameter type differs.
std::vector<ir::Node*> MethodWriter::ref_args(std::span<ir::Node* const> names, std::span<const ir::TypeP> param_types) {
    std::vector<ir::Node*> out;
    const std::size_t n = std::min(names.size(), param_types.size());
    for (std::size_t i = 0; i < n; ++i) {
        ir::Node* name = names[i];
        const ir::TypeP& t = param_types[i];
        if (t && kind_of(t) != "typevar" && kind_of(t) != "wildcard" && type_of(name) != t) {
            out.push_back(ir::make(scratch(), ir::Cast{.x = name}, t));
        } else {
            out.push_back(name);
        }
    }
    return out;
}

// A method reference is the lambda it stands for, written as one.
Code MethodWriter::on(const ir::Node& n, const ir::MethodRef& k) {
    if (!k.sym) throw Decline("unresolved method reference");
    const ir::SymP& s = k.sym;
    const SamInfo si = sam(n.type);
    const std::optional<jmodel::Signature> single_method = p_.model().functional(si.q);
    std::vector<ir::TypeP> sam_params;
    if (single_method) sam_params = single_method->params;
    std::vector<ir::Node*> params, names;   // the lambda's parameters a0 a1 ... and the names that read them
    for (std::size_t i = 0; i < si.ps.size(); ++i) {
        const ir::TypeP t = jmodel::erase(i < sam_params.size() ? sam_params[i] : ir::class_type(std::string(jmodel::kObject)));
        const ir::SymP sym = ir::make_sym(ir::SymKind::Param, std::format("a{}", i), std::nullopt, false, t);
        params.push_back(ir::make(scratch(), ir::Param{.sym = sym, .type = t}));
        names.push_back(ir::make(scratch(), ir::Name{.sym = sym}, t));
    }
    const auto make_lambda = [&](ir::Node* body) { return ir::make(scratch(), ir::Lambda{.params = params, .body = body}, n.type); };
    const auto make_call = [&](ir::Node* target, std::vector<ir::Node*> as) {
        return ir::make(scratch(), ir::Call{.target = target, .sym = s, .args = std::move(as)}, s->ret);
    };
    ir::Node* body = nullptr;
    if (k.mode == "new") {
        const ir::TypeP t = type_of(k.target);
        if (kind_of(t) == "array") {
            body = ir::make(scratch(), ir::NewArray{.elem = elem_of(t), .dims = {names.at(0)}}, t);
        } else {
            body = ir::make(scratch(), ir::New{.sym = s, .args = ref_args(names, s->params)}, t);
        }
    } else if (s->is_static) {
        ir::Node* type_name = ir::make(scratch(), ir::TypeName{}, ir::class_type(s->owner.value_or("")));
        body = make_call(type_name, ref_args(names, s->params));
    } else if (ir::is<ir::TypeName>(k.target)) {   // Type::method: the first parameter is the receiver
        ir::Node* recv = ir::make(scratch(), ir::Cast{.x = names.at(0)}, ir::class_type(s->owner.value_or("")));
        body = make_call(recv, ref_args(std::span<ir::Node* const>(names).subspan(1), s->params));
    } else {
        // the receiver is evaluated where the reference is made: held in a local the lambda captures
        const Code rc = (!ir::is<ir::This>(k.target) && !ir::is<ir::Super>(k.target)) ? expr(k.target) : this_ref();
        const std::string rname = std::format("mr{}", ++temps_);
        pending_.push_back(decl(rc.ctype, rname));
        vars_[rname] = rc.ctype;
        const ir::TypeP rt = type_of(k.target);
        ir::Node* receiver_name = ir::make(scratch(), ir::Name{.sym = ir::make_sym(ir::SymKind::Local, rname, std::nullopt, false, rt)}, rt);
        ir::Node* lambda = make_lambda(make_call(receiver_name, ref_args(names, s->params)));
        const Code got = on(*lambda, *ir::get<ir::Lambda>(lambda));
        return {std::format("({} = {}, {})", rname, rc.code, got.code), got.ctype};
    }
    ir::Node* lambda = make_lambda(body);
    return on(*lambda, *ir::get<ir::Lambda>(lambda));
}

}  // namespace cb::translate::backends::c_java::detail
