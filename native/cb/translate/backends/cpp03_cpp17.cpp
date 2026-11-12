// cb/translate/backends/cpp03_cpp17.cpp -- the C++17 dialect of the C++03 back end: the same writer with the forms C++17 has a word for
// (see cpp03.hpp).

#include <format>

#include "cb/core/arena.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/backends/cpp03_internal.hpp"

namespace cb::translate::backends::cpp03 {

using detail::need_sym;

std::unique_ptr<Writer> make_writer(Dialect dialect, Context& ctx, const ir::Node& cls, const ir::Node* method,
                                    const jir::Method* parsed) {
    if (dialect == Dialect::Cpp17) return std::make_unique<Writer17>(ctx, cls, method, parsed);
    return std::make_unique<Writer>(ctx, cls, method, parsed);
}

namespace {
namespace cpp17_impl {

bool is_null_literal(const ir::Node* e) {
    const auto* lit = ir::get<ir::Lit>(strip_paren(e));
    return lit != nullptr && std::holds_alternative<std::monostate>(lit->value);
}

bool starts_with_value_type(const std::string& ctype) {
    return ctype.starts_with("JavaString") || ctype.starts_with("JList") || ctype.starts_with("JMap") || ctype.starts_with("JSet");
}

}  // namespace cpp17_impl
}  // namespace

void Writer17::write_statement(const ir::Node* s, Lines& out, std::string_view ind) {
    if (const auto* fe = ir::get<ir::ForEach>(s)) return write_foreach(*fe, out, ind);
    if (const auto* lo = ir::get<ir::Local>(s)) {
        if (cpp17_impl::is_null_literal(lo->init)) {
            const std::string ct = ctype(lo->type);
            if (is_pointer(ct)) {   // a pointer starts as nullptr
                const std::string nm = name(need_sym(lo->sym, "local").name);
                if (vars.contains(nm)) throw Decline("shadowed local " + nm);
                vars[nm] = ct;
                out.push_back(std::format("{}{} {} = nullptr;", ind, ct, nm));
                return;
            }
        }
        if (lo->init != nullptr && spelled(*s, *lo)) return auto_local(*s, *lo, out, ind);
    }
    Writer::write_statement(s, out, ind);
}

Code Writer17::write_binary(const ir::Node& e, const ir::Binary& b) {
    if ((b.op == "==" || b.op == "!=") && (cpp17_impl::is_null_literal(b.left) || cpp17_impl::is_null_literal(b.right))) {
        const ir::Node* other = cpp17_impl::is_null_literal(b.left) ? b.right : b.left;
        bool pointer = false;
        try {
            const ir::TypeP ot = detail::type_of(other);
            pointer = ot && ot->is_ref() && is_pointer(ctype(ot));
        } catch (const Decline&) {   // a type C++03 cannot name is no pointer here
            pointer = false;
        }
        if (pointer) {
            const Code o = expr(other);
            if (is_pointer(o.ctype)) return {o.code + " " + b.op + " nullptr", "bool"};
        }
    }
    return Writer::write_binary(e, b);
}

bool Writer17::spelled(const ir::Node& node, const ir::Local& s) const {
    (void)node;
    const ir::Node* init = strip_paren(s.init);
    if (!(ir::is<ir::New>(init) || ir::is<ir::Cast>(init)) || !s.type || !init->type) return false;
    const auto* nw = ir::get<ir::New>(init);
    return *init->type == *s.type && s.type->kind() == "class" && !(nw != nullptr && nw->body != nullptr);
}

void Writer17::auto_local(const ir::Node& node, const ir::Local& s, Lines& out, std::string_view ind) {
    Lines lines;
    Writer::write_statement(&node, lines, ind);
    const auto declared = vars.find(name(need_sym(s.sym, "local").name));
    const std::string ct = declared != vars.end() ? declared->second : "";
    const std::string prefix = std::string(ind) + ct + " ";
    if (lines.size() == 1 && !ct.empty() && lines[0].starts_with(prefix)) {
        const std::string star = is_pointer(ct) ? "*" : "";
        if (!star.empty() || cpp17_impl::starts_with_value_type(ct)) lines[0] = std::string(ind) + "auto" + star + " " + lines[0].substr(prefix.size());
    }
    out.insert(out.end(), lines.begin(), lines.end());
}

void Writer17::write_foreach(const ir::ForEach& s, Lines& out, std::string_view ind) {
    const Code coll = expr(s.iterable);
    const auto* var = ir::get<ir::Local>(s.var);
    if (var == nullptr) throw Decline("a for-each variable that is no Local");
    const std::string nm = name(need_sym(var->sym, "loop variable").name);
    const std::string vt = ctype(var->type);
    if (vars.contains(nm)) throw Decline("shadowed loop variable " + nm);
    if (coll.ctype.starts_with("JList<") || coll.ctype.starts_with("const JList<")) {
        vars[nm] = vt;
        out.push_back(std::format("{}for ({} {} : {}) {{", ind, vt, nm, coll.code));
        loop_body(s.body, out, ind);
        out.push_back(std::string(ind) + "}");
        vars.erase(nm);
        return;
    }
    if (coll.ctype.starts_with("JArray<")) {
        const int n = ++temps_;
        const std::string arr = std::format("j2x_a{}", n);
        const std::string idx = std::format("j2x_i{}", n);
        out.push_back(std::string(ind) + "{");
        out.push_back(std::format("{}\t{} {} = {};", ind, coll.ctype, arr, coll.code));
        out.push_back(std::format("{}\tfor (int {} = 0; {} < {}.length; {}++) {{", ind, idx, idx, arr, idx));
        vars[nm] = vt;
        out.push_back(std::format("{}\t\t{} {} = {}[{}];", ind, vt, nm, arr, idx));
        loop_body(s.body, out, std::string(ind) + "\t");
        out.push_back(std::string(ind) + "\t}");
        out.push_back(std::string(ind) + "}");
        vars.erase(nm);
        return;
    }
    throw Decline("for-each over " + coll.ctype);
}

Code Writer17::write_switch_expr(const ir::Node& e, const ir::SwitchExpr& s) {
    const std::string t = ctype(e.type);
    const ir::Node* selector = s.expr;
    if (const auto* p = ir::get<ir::Paren>(selector)) selector = p->x;
    const Code x = expr(selector);
    if (detail::has(x.ctype, "JavaString")) throw Decline("switch on a string");
    Lines lines{std::format("[&]() -> {} {{", t), std::format("\tswitch({}) {{", x.code)};
    const std::string saved_ret = ret;
    const bool saved_two_phase = two_phase;
    ret = t;
    two_phase = false;
    const auto restore = [&] {
        ret = saved_ret;
        two_phase = saved_two_phase;
    };
    Arena scratch;   // the `return`s a yield becomes: they live in this call only and nothing in the tree points to them
    try {
        for (const ir::Node* node : s.cases) {
            const auto* c = ir::get<ir::Case>(node);
            if (c == nullptr) throw Decline("a switch holding something that is no case");
            for (const ir::Node* label : c->labels) lines.push_back(std::format("\tcase {}:", expr(label).code));
            if (c->is_default) lines.push_back("\tdefault:");
            std::vector<const ir::Node*> body;
            for (const ir::Node* b : c->body) {
                if (const auto* y = ir::get<ir::Yield>(b)) body.push_back(ir::make(scratch, ir::Return{.expr = y->expr}, {}, b->line, b->col));
                else body.push_back(b);
            }
            for (const ir::Node* b : body) {
                detail::for_each_node(b, [](const ir::Node* n) {
                    if (ir::is<ir::Yield>(n)) throw Decline("a yield inside a nested statement");
                });
            }
            lines.push_back("\t{");
            for (const ir::Node* b : body) stmt(b, lines, "\t\t");
            lines.push_back("\t}");
        }
    } catch (...) {
        restore();
        throw;
    }
    restore();
    lines.push_back("\t}");
    lines.push_back("\tthrow 0;");
    lines.push_back("}()");
    return {text::join(lines, "\n"), t};
}

bool Writer17::handles(const ir::Node* e) const { return Writer::handles(e) || ir::is<ir::SwitchExpr>(e); }

Code Writer17::dispatch(const ir::Node* e) {
    if (const auto* x = ir::get<ir::SwitchExpr>(e)) return write_switch_expr(*e, *x);
    return Writer::dispatch(e);
}

}  // namespace cb::translate::backends::cpp03
