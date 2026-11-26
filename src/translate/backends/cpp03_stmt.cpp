// translate/backends/cpp03_stmt.cpp -- the C++03 back end's statements: blocks, locals, loops, switch, try / catch / finally (see
// cpp03.hpp). Each writes lines of C++ into `out`, indented by `ind` (a run of tabs).

#include <format>

#include "core/text.hpp"
#include "translate/backends/cpp03_internal.hpp"

namespace cb::translate::backends::cpp03 {

namespace typemap = cb::java::typemap;

using cb::java::ledger::Category;
using cb::java::ledger::Severity;
using detail::block_stmts;
using detail::need_sym;
using detail::single;

namespace {
namespace stmt_impl {

bool is_loop(const ir::Node* n) {
    return ir::is<ir::While>(n) || ir::is<ir::DoWhile>(n) || ir::is<ir::For>(n) || ir::is<ir::ForEach>(n);
}

}  // namespace stmt_impl
}  // namespace

// ------------------------------------------------------------------ the statements of a body
Lines Writer::body() {
    if (m_ == nullptr) throw Decline("no method");
    if (std::holds_alternative<ir::Node*>(m_->body) && std::get<ir::Node*>(m_->body) == nullptr) throw Decline("no body");
    // A boxed local (Integer, Byte ...) the method compares with null keeps the box (JObject): written as its primitive, `null`
    // became 0 and a real 0 read as null -- Beta's DataWatcher took Byte's type id 0 for "Unknown data type" and shut the game down.
    nullable_boxed_.clear();
    const auto is_null = [](const ir::Node* e) {
        const auto* lit = ir::get<ir::Lit>(strip_paren(e));
        return lit != nullptr && std::holds_alternative<std::monostate>(lit->value);
    };
    for (ir::Node* n : ir::walk(const_cast<ir::Node*>(single(m_->body)))) {
        const auto* b = ir::get<ir::Binary>(n);
        if (b == nullptr || (b->op != "==" && b->op != "!=") || is_null(b->left) == is_null(b->right)) continue;
        const ir::Node* other = strip_paren(is_null(b->left) ? b->right : b->left);
        const auto* nm = ir::get<ir::Name>(other);
        if (nm == nullptr || !nm->sym || !other->type || other->type->kind() != "class") continue;
        if (typemap::boxed_value_types().contains(other->type->simple())) nullable_boxed_.insert(name(nm->sym->name));
    }
    Lines out;
    stmts(block_stmts(single(m_->body)), out, "");
    return out;
}

void Writer::stmts(std::span<const ir::Node* const> list, Lines& out, std::string_view ind) {
    const auto saved = after_;
    for (std::size_t i = 0; i < list.size(); ++i) {
        after_ = list.subspan(i + 1);
        try {
            stmt(list[i], out, ind);
        } catch (...) {
            after_ = saved;
            throw;
        }
    }
    after_ = saved;
}

void Writer::block(const ir::Node* statement, Lines& out, std::string_view ind) {
    const auto saved = vars;   // what the block declares goes out of scope with it
    const std::string inner = std::string(ind) + "\t";
    if (const auto* b = ir::get<ir::Block>(statement)) stmts(b->stmts, out, inner);
    else stmt(statement, out, inner);
    vars = saved;
}

void Writer::block(const ir::Suite& suite, Lines& out, std::string_view ind) { block(single(suite), out, ind); }

void Writer::loop_body(const ir::Node* body, Lines& out, std::string_view ind) {
    const std::optional<std::string> label = std::exchange(cont_next_, std::nullopt);
    if (!label) {
        block(body, out, ind);
        return;
    }
    out.push_back(std::string(ind) + "\t{");
    block(body, out, std::string(ind) + "\t");
    out.push_back(std::string(ind) + "\t}");
    out.push_back(std::string(ind) + "\t" + *label + "_cont:;");
}

void Writer::loop_body(const ir::Suite& body, Lines& out, std::string_view ind) { loop_body(single(body), out, ind); }

void Writer::stmt(const ir::Suite& suite, Lines& out, std::string_view ind) { stmt(single(suite), out, ind); }

void Writer::stmt(const ir::Node* s, Lines& out, std::string_view ind) {
    const bool loop = stmt_impl::is_loop(s) || ir::is<ir::Switch>(s);
    loops_ += loop;
    Lines saved_pending = std::move(pending_);
    pending_.clear();
    const bool saved_order = order_;
    order_ = true;
    const auto saved_subst = subst_;   // temporaries belong to this rendering of the statement (a finally body is written twice)
    Lines sub;
    const auto restore = [&](Lines& declarations) {
        loops_ -= loop;
        declarations = std::move(pending_);
        pending_ = std::move(saved_pending);
        order_ = saved_order;
        subst_ = saved_subst;
    };
    Lines decls;
    try {
        write_statement(s, sub, ind);
    } catch (...) {
        restore(decls);
        throw;
    }
    restore(decls);
    for (const std::string& d : decls) out.push_back(std::string(ind) + d);   // the statement's ordered-operand temporaries
    out.insert(out.end(), sub.begin(), sub.end());
}

void Writer::write_statement(const ir::Node* s, Lines& out, std::string_view ind) {
    const auto emit = [&](std::string line) { out.push_back(std::string(ind) + line); };
    if (ir::is<ir::Block>(s)) {
        emit("{");
        block(s, out, ind);
        emit("}");
    } else if (const auto* lb = ir::get<ir::Labeled>(s)) {
        write_labeled(*lb, out, ind);
    } else if (const auto* sy = ir::get<ir::Sync>(s)) {   // as the text path: the lock is dropped, and marked
        emit(std::format("/* J2X-REVIEW[synchronized-dropped]: was synchronized({}) -- add a lock if this state is touched by another "
                         "thread (network/pool) */ {{",
                         comment_safe(expr(sy->lock).code)));
        block(sy->body, out, ind);
        emit("}");
    } else if (const auto* lo = ir::get<ir::Local>(s)) {
        write_local(*lo, out, ind);
    } else if (const auto* es = ir::get<ir::ExprStmt>(s)) {
        review(es->expr, out, ind);
        hoist_owned_args(es->expr, out, ind);
        free_owned_reassign(es->expr, out, ind);
        const std::optional<std::string> kept = kept_call(es->expr);
        if (kept && !kept->empty()) emit(*kept);
        else emit(expr(es->expr).code + ";");
    } else if (const auto* iff = ir::get<ir::If>(s)) {
        emit(std::format("if({}) {{", cond(iff->cond)));
        block(iff->then, out, ind);
        const ir::Suite* e = &iff->orelse;
        while (std::holds_alternative<ir::Node*>(*e) && ir::is<ir::If>(std::get<ir::Node*>(*e))) {
            const auto* ei = ir::get<ir::If>(std::get<ir::Node*>(*e));
            emit(std::format("}} else if({}) {{", cond(ei->cond)));
            block(ei->then, out, ind);
            e = &ei->orelse;
        }
        if (std::holds_alternative<std::vector<ir::Node*>>(*e) || std::get<ir::Node*>(*e) != nullptr) {
            emit("} else {");
            block(*e, out, ind);
        }
        emit("}");
    } else if (const auto* wh = ir::get<ir::While>(s)) {
        emit(std::format("while({}) {{", cond(wh->cond)));
        loop_body(wh->body, out, ind);
        emit("}");
    } else if (const auto* dw = ir::get<ir::DoWhile>(s)) {
        emit("do {");
        loop_body(dw->body, out, ind);
        emit(std::format("}} while({});", cond(dw->cond)));
    } else if (const auto* fo = ir::get<ir::For>(s)) {
        write_for(*fo, out, ind);
    } else if (const auto* rt = ir::get<ir::Return>(s); rt != nullptr && !fin_.empty()) {   // out through the finally
        const int n = fin_.back().first;
        if (rt->expr != nullptr) {
            const auto [code, it] = expr(rt->expr);
            emit(std::format("j2xRet{} = {};", n, unbox(coerce(code, ret), it, ret)));
        }
        emit(std::format("j2xFin{} = 1; goto j2xFinally{};", n, n));
    } else if ((ir::is<ir::Break>(s) || ir::is<ir::Continue>(s)) && !fin_.empty() && loops_ == fin_.back().second &&
               [&] {
                   const auto& label = ir::is<ir::Break>(s) ? ir::get<ir::Break>(s)->label : ir::get<ir::Continue>(s)->label;
                   return !label || label->empty();   // an unlabelled jump
               }()) {
        const int n = fin_.back().first;
        emit(std::format("j2xFin{} = {}; goto j2xFinally{};", n, ir::is<ir::Break>(s) ? 2 : 3, n));
    } else if (const auto* ret_s = ir::get<ir::Return>(s)) {
        if (ret_s->expr != nullptr) {
            review(ret_s->expr, out, ind);
            hoist_owned_args(ret_s->expr, out, ind);
            const auto [code, it] = expr(ret_s->expr);
            emit(std::format("return {};", unbox(coerce(code, ret), it, ret)));
        } else {
            emit(two_phase ? "return this;" : "return;");
        }
    } else if (const auto* br = ir::get<ir::Break>(s)) {
        if (br->label && !br->label->empty()) emit("goto " + *br->label + "_brk;");
        else emit("break;");
    } else if (const auto* co = ir::get<ir::Continue>(s)) {
        if (co->label && !co->label->empty()) emit("goto " + *co->label + "_cont;");
        else emit("continue;");
    } else if (const auto* th = ir::get<ir::Throw>(s)) {
        emit(std::format("throw {};", expr(th->expr).code));
    } else if (const auto* sw = ir::get<ir::Switch>(s)) {
        write_switch(*sw, out, ind);
    } else if (const auto* tr = ir::get<ir::Try>(s)) {
        write_try(*tr, out, ind);
    } else if (ir::is<ir::Empty>(s)) {
        emit(";");
    } else if (ir::is<ir::CtorCall>(s)) {
        throw Decline("constructor call not first");
    } else {
        throw Decline("statement " + (s != nullptr ? std::string(ir::kind_name(*s)) : std::string("NoneType")));
    }
}

void Writer::write_labeled(const ir::Labeled& s, Lines& out, std::string_view ind) {
    const ir::Node* body = single(s.body);
    bool any_continue = false;
    bool any_break = false;
    detail::for_each_node(body, [&](const ir::Node* n) {
        if (const auto* c = ir::get<ir::Continue>(n)) any_continue = any_continue || c->label == s.label;
        else if (const auto* b = ir::get<ir::Break>(n)) any_break = any_break || b->label == s.label;
    });
    if (any_continue) {
        if (!stmt_impl::is_loop(body)) throw Decline("continue to a label that is not a loop");
        cont_next_ = s.label;
    }
    stmt(s.body, out, ind);
    if (any_break) out.push_back(std::format("{}{}_brk:;", ind, s.label ? *s.label : std::string("None")));
}

void Writer::write_local(const ir::Local& s, Lines& out, std::string_view ind) {
    const std::string nm = name(need_sym(s.sym, "local").name);
    if (vars.contains(nm)) throw Decline("shadowed local " + nm);
    const std::string ct = nullable_boxed_.contains(nm) ? std::string("JObject") : ctype(s.type);
    vars[nm] = ct;
    if (s.init != nullptr) {
        review(s.init, out, ind);
        hoist_owned_args(s.init, out, ind);
        const auto [code, it] = expr(s.init);
        out.push_back(std::format("{}{} {} = {};", ind, ct, nm, unbox(coerce(code, ct), it, ct)));
        free_owned_return(s, ct, nm, out, ind);
    } else {
        out.push_back(std::format("{}{} {};", ind, ct, nm));
    }
}

void Writer::write_for(const ir::For& s, Lines& out, std::string_view ind) {
    const auto& inits = s.init;
    if (inits.size() > 1) {
        for (const ir::Node* i : inits) {
            if (!ir::is<ir::Local>(i)) throw Decline("for init");
        }
    }
    std::string init;
    if (!inits.empty()) {
        std::vector<std::string> parts;
        if (const auto* first = ir::get<ir::Local>(inits[0])) {
            const std::string ct = ctype(first->type);
            for (const ir::Node* node : inits) {
                const auto* l = ir::get<ir::Local>(node);
                const std::string n = name(need_sym(l->sym, "loop variable").name);
                if (vars.contains(n)) throw Decline("shadowed loop variable " + n);
                vars[n] = ct;
                if (l->init != nullptr) parts.push_back(n + " = " + expr(l->init).code);
                else parts.push_back(n);
            }
            init = ct + " " + detail::join_commas(parts);
        } else {
            for (const ir::Node* node : inits) {
                const auto* x = ir::get<ir::ExprStmt>(node);
                if (x == nullptr) throw Decline("for init");
                parts.push_back(expr(x->expr).code);
            }
            init = detail::join_commas(parts);
        }
    }
    const std::string c = s.cond != nullptr ? cond(s.cond) : "";
    std::vector<std::string> ups;
    for (const ir::Node* u : s.update) ups.push_back(expr(u).code);
    out.push_back(std::format("{}for ({}; {}; {}) {{", ind, init, c, detail::join_commas(ups)));
    loop_body(s.body, out, ind);
    out.push_back(std::string(ind) + "}");
    for (const ir::Node* l : inits) {   // the loop variable's scope ends with the loop
        if (const auto* ll = ir::get<ir::Local>(l)) vars.erase(name(need_sym(ll->sym, "loop variable").name));
    }
}

void Writer::write_switch(const ir::Switch& s, Lines& out, std::string_view ind) {
    const ir::Node* selector = s.expr;
    if (const auto* p = ir::get<ir::Paren>(selector)) selector = p->x;
    const auto [x, xt] = expr(selector);
    if (detail::has(xt, "JavaString")) throw Decline("switch on a string");
    out.push_back(std::format("{}switch({}) {{", ind, x));
    for (const ir::Node* node : s.cases) {
        const auto* c = ir::get<ir::Case>(node);
        if (c == nullptr) throw Decline("a switch holding something that is no case");
        for (const ir::Node* label : c->labels) out.push_back(std::format("{}case {}:", ind, expr(label).code));
        if (c->is_default) out.push_back(std::string(ind) + "default:");
        out.push_back(std::string(ind) + "{");
        for (const ir::Node* b : c->body) stmt(b, out, std::string(ind) + "\t");
        out.push_back(std::string(ind) + "}");
    }
    out.push_back(std::string(ind) + "}");
}

void Writer::write_try(const ir::Try& s, Lines& out, std::string_view ind) {
    if (!s.resources.empty()) throw Decline("try-with-resources");
    if (s.finally_body == nullptr) {
        try_catch(s, out, ind);
        return;
    }
    // finally: run on the way out by an exception (catch (...), then rethrow) and on the way out normally (after the try). A return,
    // break or continue out of the try would skip the second copy, so such a try goes through the jump variable instead.
    std::vector<const ir::Node*> parts{s.body};
    for (const ir::Node* c : s.catches) {
        const auto* ct = ir::get<ir::Catch>(c);
        if (ct == nullptr) throw Decline("a try holding something that is no catch");
        parts.push_back(ct->body);
    }
    if (leaves(parts)) {
        finally_with_jumps(s, out, ind);
        return;
    }
    out.push_back(std::string(ind) + "try {");
    try_catch(s, out, std::string(ind) + "\t");
    out.push_back(std::string(ind) + "} catch (...) {");
    block(s.finally_body, out, ind);
    out.push_back(std::string(ind) + "\tthrow;");
    out.push_back(std::string(ind) + "}");
    out.push_back(std::string(ind) + "{");
    block(s.finally_body, out, ind);
    out.push_back(std::string(ind) + "}");
}

void Writer::finally_with_jumps(const ir::Try& s, Lines& out, std::string_view ind) {
    if (!fin_.empty()) throw Decline("a finally with jumps inside another");
    std::vector<const ir::Node*> bodies{s.body};
    for (const ir::Node* c : s.catches) {
        const auto* ct = ir::get<ir::Catch>(c);
        if (ct == nullptr) throw Decline("a try holding something that is no catch");
        bodies.push_back(ct->body);
    }
    for (const ir::Node* b : bodies) {
        detail::for_each_node(b, [](const ir::Node* x) {
            if (const auto* bk = ir::get<ir::Break>(x)) {
                if (bk->label && !bk->label->empty()) throw Decline("a labelled jump out of a finally");
            } else if (const auto* ct = ir::get<ir::Continue>(x)) {
                if (ct->label && !ct->label->empty()) throw Decline("a labelled jump out of a finally");
            }
        });
    }
    const int n = ++fin_count_;
    const std::string pad(ind);
    out.push_back(pad + "{");
    out.push_back(std::format("{}\tint j2xFin{} = 0;", ind, n));
    if (ret != "void" && !two_phase) out.push_back(std::format("{}\t{} j2xRet{};", ind, detail::strip_const_ref(ret), n));
    fin_.emplace_back(n, loops_);
    try {
        out.push_back(pad + "\ttry {");
        try_catch(s, out, pad + "\t\t");
        out.push_back(pad + "\t} catch (...) {");
        block(s.finally_body, out, pad + "\t");
        out.push_back(pad + "\t\tthrow;");
        out.push_back(pad + "\t}");
    } catch (...) {
        fin_.pop_back();
        throw;
    }
    fin_.pop_back();
    out.push_back(std::format("{}\tj2xFinally{}:;", ind, n));
    out.push_back(pad + "\t{");
    block(s.finally_body, out, pad + "\t");
    out.push_back(pad + "\t}");
    // when the try and every catch end in a return or throw, the finally is always left by the return: say so without a test, or the
    // compiler sees a path that returns nothing (MSVC C4715)
    bool always = is_abrupt(s.body);
    for (const ir::Node* c : s.catches) {
        if (always && !is_abrupt(ir::get<ir::Catch>(c)->body)) always = false;
    }
    const std::string test = always ? "" : std::format("if (j2xFin{} == 1) ", n);
    if (two_phase) out.push_back(std::format("{}\t{}return this;", ind, test));
    else if (ret == "void") out.push_back(std::format("{}\t{}return;", ind, test));
    else out.push_back(std::format("{}\t{}return j2xRet{};", ind, test, n));
    if (loops_ != 0) {
        out.push_back(std::format("{}\tif (j2xFin{} == 2) break;", ind, n));
        out.push_back(std::format("{}\tif (j2xFin{} == 3) continue;", ind, n));
    }
    out.push_back(pad + "}");
}

void Writer::try_catch(const ir::Try& s, Lines& out, std::string_view ind) {
    if (s.catches.empty()) {
        out.push_back(std::string(ind) + "{");
        block(s.body, out, ind);
        out.push_back(std::string(ind) + "}");
        return;
    }
    bool npe_here = false;
    for (const ir::Node* node : s.catches) {
        const auto* c = ir::get<ir::Catch>(node);
        if (c == nullptr) throw Decline("a try holding something that is no catch");
        for (const ir::TypeP& t : c->types) {
            const auto* cls = detail::class_type(t);
            if (cls != nullptr && detail::npe_catchers().contains(cls->name)) npe_here = true;
        }
    }
    out.push_back(std::string(ind) + "try {");
    npe_ += npe_here;
    try {
        block(s.body, out, ind);
    } catch (...) {
        npe_ -= npe_here;
        throw;
    }
    npe_ -= npe_here;
    for (const ir::Node* node : s.catches) {
        const auto* c = ir::get<ir::Catch>(node);
        if (c->types.size() != 1) throw Decline("multi-catch");
        const std::string ct = ctype(c->types[0]);
        const std::string nm = name(need_sym(c->sym, "catch variable").name);
        if (vars.contains(nm)) throw Decline("shadowed catch variable");
        vars[nm] = ct;
        const bool used = detail::any_node(c->body, [&](const ir::Node* n) {
            const auto* nn = ir::get<ir::Name>(n);
            return nn != nullptr && nn->sym && nn->sym->name == c->sym->name;
        });
        out.push_back(std::string(ind) + (used ? std::format("}} catch ({} {}) {{", ct, nm) : std::format("}} catch ({}) {{", ct)));   // C4101
        block(c->body, out, ind);
        vars.erase(nm);
    }
    out.push_back(std::string(ind) + "}");
}

void Writer::note(Category category, Severity severity, std::string_view rule, std::string text) {
    if (ctx_.ledger == nullptr) return;
    cb::java::ledger::EventDetail detail;
    detail.line = method_line_;   // a static block or field initialiser has no method
    detail.severity = severity;
    detail.source = "emit";
    detail.rule = std::string(rule);
    ctx_.ledger->add(cname_ + ".cpp", category, std::move(text), std::move(detail));
}

std::optional<std::string> Writer::kept_call(const ir::Node* e_) {
    const ir::Node* e = strip_paren(e_);
    const auto* call = ir::get<ir::Call>(e);
    if (npe_ != 0 || call == nullptr || call->target == nullptr || !call->sym) return std::nullopt;
    const ir::Node* t = strip_paren(call->target);
    if (const auto* nw = ir::get<ir::New>(t)) {
        const auto* created = detail::class_type(t->type);
        if (nw->body == nullptr && nw->outer == nullptr && created != nullptr && ctx_.is_game(created->name) &&
            call->sym->name != "start" && call->sym->name != "run") {
            const std::string name = jtype(t->type).name;
            const auto& data = r_.data();
            if (data.thread_like.contains(name) || data.values.contains(name) || r_.extends_value_container(name) ||
                !data.known.contains(name))
                return std::nullopt;
            // the call as written, for its method and arguments. Writing it may hold operands in sequenced temporaries
            // (`(j2xo1 = new X(..), j2xo2 = .., j2xo1->m(j2xo2))`): when the form is not the plain one, the writer is put back as
            // it was, so the statement's own writing evaluates them again. FIXED BUG kept-call-lost-temps: the temporaries stayed
            // registered and the statement wrote `j2xo1->m(j2xo2)` with j2xo1 still NULL (every rail placed: BlockRail's
            // (new RailLogic(..)).func_792_a(world.isBlockIndirectlyGettingPowered(..), b); RenderMinecart's chest).
            const auto saved_subst = subst_;
            const Lines saved_pending = pending_;
            const int saved_temps = temps_;
            const std::string code = expr(e).code;
            static const Regex written(R"(^\(?new ([^(]+)\((.*?)\)\)?->(\w+)\((.*)\)$)");
            const auto m = written.match(code);
            if (!m || m->group(1) != name) {
                subst_ = saved_subst;
                pending_ = saved_pending;
                temps_ = saved_temps;
                return std::nullopt;
            }
            const std::string tmp = std::format("_j2xleak{}", leaks_++);
            const std::string decl = !m->group(2).empty() ? std::format("{} {}({});", name, tmp, m->group(2)) : std::format("{} {};", name, tmp);
            note(Category::Rule, Severity::Med, "leak-temp-stackified",
                 std::format("leak fixed: throwaway `(new {}(...))->{}(...)` -> scoped stack object (Java GC temp leaked every call)", name,
                             m->group(3)));
            return std::format("{{ {} {}.{}({}); }}", decl, tmp, m->group(3), m->group(4));
        }
    }
    if (ir::is<ir::Index>(t)) {
        const auto [recv, rt] = expr(t);
        if (!is_pointer(rt)) return std::nullopt;
        const std::string code = expr(e).code;
        note(Category::Risk, Severity::Med, "null-deref-guard",
             "null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be a virtual call through NULL -- "
             "undefined behaviour in C++)");
        return std::format("if ({} != NULL) {{ {}; }}", recv, code);
    }
    return std::nullopt;
}

}  // namespace cb::translate::backends::cpp03
