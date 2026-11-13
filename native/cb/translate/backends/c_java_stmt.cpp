// cb/translate/backends/c_java_stmt.cpp -- statements in the C back end for Java: blocks, locals, the loops, labelled jumps, switch,
// and try/catch/finally over the runtime's handler chain (see c_java.hpp).

#include <format>

#include "cb/core/text.hpp"
#include "cb/translate/backends/c_java_internal.hpp"

namespace cb::translate::backends::c_java::detail {

// ---- statements
void MethodWriter::stmts(std::span<ir::Node* const> list, Lines& out, std::string_view ind) {
    for (const ir::Node* s : list) stmt(s, out, ind);
}

void MethodWriter::block(const ir::Node* s, Lines& out, std::string_view ind) {
    const auto saved = vars_;
    const std::string inner = std::string(ind) + "\t";
    if (const auto* b = ir::get<ir::Block>(s)) {
        stmts(b->stmts, out, inner);
    } else if (s != nullptr) {
        stmt(s, out, inner);
    }
    vars_ = saved;
}

void MethodWriter::loop_body(const ir::Node* body, Lines& out, std::string_view ind) {
    const std::optional<std::string> label = std::exchange(cont_next_, std::nullopt);
    ++loops_;
    if (!label) {
        block(body, out, ind);
    } else {
        // `continue label` jumps to the end of the body: the body sits in its own block with `label_cont:` after it, so the goto
        // crosses no initialisation
        out.push_back(std::format("{}\t{{", ind));
        block(body, out, std::string(ind) + "\t");
        out.push_back(std::format("{}\t}}", ind));
        out.push_back(std::format("{}\t{}_cont:;", ind, *label));
    }
    --loops_;
}

void MethodWriter::stmt(const ir::Node* s, Lines& out, std::string_view ind) {
    // The temporaries a statement's operands need are declared before it: the statement has its own list, its own sequencing mode
    // and its own substitutions, put back when it is done. (A Decline ends the whole program, so nothing is restored for it.)
    Lines saved_pending = std::move(pending_);
    pending_.clear();
    const bool saved_order = std::exchange(order_, true);
    const auto saved_subst = subst_;
    if (s == nullptr) throw Decline("statement NoneType");
    Lines text;
    ir::visit(*s, [&](const auto& k) { emit(*s, k, text, ind); });
    Lines decls = std::move(pending_);
    pending_ = std::move(saved_pending);
    order_ = saved_order;
    subst_ = saved_subst;
    for (const std::string& d : decls) out.push_back(std::string(ind) + d);
    out.insert(out.end(), std::make_move_iterator(text.begin()), std::make_move_iterator(text.end()));
}

std::string MethodWriter::cond(const ir::Node* e) {
    const Code c = expr(cpp03::strip_paren(e));
    return conv(c.code, c.ctype, "bool");
}

std::string MethodWriter::decl(std::string_view ct, std::string_view name, const std::optional<std::string>& init) const {
    std::string q;
    if (vol_) {   // locals of a function with a try are volatile: their values survive the longjmp
        q = ct.ends_with('*') ? std::format("{} volatile", ct) : std::format("volatile {}", ct);
    } else {
        q = std::string(ct);
    }
    const std::string tail = init ? " = " + *init : (is_prim_c(ct) ? " = 0" : " = NULL");
    return std::format("{} {}{};", q, name, tail);
}

std::string MethodWriter::pops(std::size_t depth) const {
    std::string out;
    for (std::size_t i = tries_.size(); i > depth; --i) out += std::format("jrt_pop(&{}); ", tries_[i - 1].name);
    return out;
}

template <class K> void MethodWriter::emit(const ir::Node& n, const K&, Lines&, std::string_view) {
    throw Decline("statement " + std::string(ir::kind_name(n)));
}

void MethodWriter::emit(const ir::Node&, const ir::Block& k, Lines& out, std::string_view ind) {
    out.push_back(std::format("{}{{", ind));
    stmts(k.stmts, out, std::string(ind) + "\t");
    out.push_back(std::format("{}}}", ind));
}

void MethodWriter::emit(const ir::Node&, const ir::Local& k, Lines& out, std::string_view ind) {
    const std::string name = ident(need_sym(k.sym, "a local").name);
    const std::string ct = p_.ct(k.type);
    vars_[name] = ct;
    if (k.init == nullptr) {
        out.push_back(std::string(ind) + decl(ct, name));
        return;
    }
    const Code init = expr(k.init);
    out.push_back(std::string(ind) + decl(ct, name, conv(init.code, init.ctype, ct)));
}

void MethodWriter::emit(const ir::Node&, const ir::ExprStmt& k, Lines& out, std::string_view ind) {
    out.push_back(std::format("{}{};", ind, expr(k.expr).code));
}

void MethodWriter::emit(const ir::Node&, const ir::If& k, Lines& out, std::string_view ind) {
    out.push_back(std::format("{}if ({}) {{", ind, cond(k.cond)));
    block(single(k.then), out, ind);
    const ir::Node* rest = single(k.orelse);
    while (const auto* chained = ir::get<ir::If>(rest)) {
        out.push_back(std::format("{}}} else if ({}) {{", ind, cond(chained->cond)));
        block(single(chained->then), out, ind);
        rest = single(chained->orelse);
    }
    if (rest != nullptr) {
        out.push_back(std::format("{}}} else {{", ind));
        block(rest, out, ind);
    }
    out.push_back(std::format("{}}}", ind));
}

void MethodWriter::emit(const ir::Node&, const ir::While& k, Lines& out, std::string_view ind) {
    out.push_back(std::format("{}while ({}) {{", ind, cond(k.cond)));
    loop_body(single(k.body), out, ind);
    out.push_back(std::format("{}}}", ind));
}

void MethodWriter::emit(const ir::Node&, const ir::DoWhile& k, Lines& out, std::string_view ind) {
    out.push_back(std::format("{}do {{", ind));
    loop_body(single(k.body), out, ind);
    out.push_back(std::format("{}}} while ({});", ind, cond(k.cond)));
}

void MethodWriter::emit(const ir::Node&, const ir::For& k, Lines& out, std::string_view ind) {
    const auto saved = vars_;
    out.push_back(std::format("{}{{", ind));
    const std::string inner = std::string(ind) + "\t";
    for (const ir::Node* init : k.init) stmt(init, out, inner);
    const std::string test = k.cond != nullptr ? cond(k.cond) : "1";
    std::vector<std::string> updates;
    for (const ir::Node* u : k.update) updates.push_back(expr(u).code);
    out.push_back(std::format("{}for (; {}; {}) {{", inner, test, join_commas(updates)));
    loop_body(single(k.body), out, inner);
    out.push_back(std::format("{}}}", inner));
    out.push_back(std::format("{}}}", ind));
    vars_ = saved;
}

void MethodWriter::emit(const ir::Node&, const ir::Labeled& k, Lines& out, std::string_view ind) {
    const std::string label = k.label.value_or("");
    const ir::Node* body = single(k.body);
    // the jumps in the body that name this label: a `continue` makes the next loop's body end in `label_cont:`, a `break` leaves
    // `label_brk:` after the statement
    bool continued = false, broken = false;
    for_each_node(body, [&](const ir::Node* n) {
        if (const auto* b = ir::get<ir::Break>(n); b != nullptr && b->label == k.label) broken = true;
        if (const auto* c = ir::get<ir::Continue>(n); c != nullptr && c->label == k.label) continued = true;
    });
    if (continued) cont_next_ = label;
    stmt(body, out, ind);
    if (broken) out.push_back(std::format("{}{}_brk:;", ind, label));
}

void MethodWriter::emit(const ir::Node&, const ir::Return& k, Lines& out, std::string_view ind) {
    const std::string unwind = pops(0);
    if (k.expr == nullptr) {
        out.push_back(std::format("{}{}return;", ind, unwind));
        return;
    }
    const Code value = expr(k.expr);
    const std::string v = conv(value.code, value.ctype, ret_);
    if (!unwind.empty()) {
        out.push_back(std::format("{}{{ {} r_ = {}; {}return r_; }}", ind, ret_, v, unwind));   // the value survives the handlers' pops
    } else {
        out.push_back(std::format("{}return {};", ind, v));
    }
}

void MethodWriter::jump(bool is_break, const std::optional<std::string>& label, Lines& out, std::string_view ind) {
    const bool labelled = label && !label->empty();
    std::string unwind;
    if (!labelled) {   // the handlers opened inside the loop
        std::size_t outside = 0;
        for (const Handler& h : tries_) outside += h.loops < loops_;
        unwind = pops(outside);
    } else {
        unwind = !tries_.empty() ? pops(0) : "";
    }
    const std::string word = labelled ? std::format("goto {}_{};", *label, is_break ? "brk" : "cont") : (is_break ? "break;" : "continue;");
    out.push_back(std::format("{}{}{}", ind, unwind, word));
}

void MethodWriter::emit(const ir::Node&, const ir::Break& k, Lines& out, std::string_view ind) { jump(true, k.label, out, ind); }
void MethodWriter::emit(const ir::Node&, const ir::Continue& k, Lines& out, std::string_view ind) { jump(false, k.label, out, ind); }

void MethodWriter::emit(const ir::Node&, const ir::Throw& k, Lines& out, std::string_view ind) {
    out.push_back(std::format("{}jrt_throw((jobject*){});", ind, expr(k.expr).code));
}

void MethodWriter::emit(const ir::Node&, const ir::Empty&, Lines& out, std::string_view ind) { out.push_back(std::string(ind) + ";"); }
void MethodWriter::emit(const ir::Node&, const ir::Sync& k, Lines& out, std::string_view ind) { block(k.body, out, ind); }   // one thread: no lock
void MethodWriter::emit(const ir::Node&, const ir::Assert&, Lines&, std::string_view) {}   // assertions are off, as the JVM's default has them
void MethodWriter::emit(const ir::Node&, const ir::LocalClass&, Lines&, std::string_view) {}   // a local class is a class of the program
void MethodWriter::emit(const ir::Node&, const ir::CtorCall&, Lines&, std::string_view) { throw Decline("constructor call not first"); }

std::string MethodWriter::case_label(const ir::Node* label, const ClassInfo* enum_class) {
    label = cpp03::strip_paren(label);
    if (enum_class != nullptr) {   // an enum's cases are its constants' ordinals
        const ir::Sym* sym = nullptr;
        if (const auto* sel = ir::get<ir::Select>(label)) sym = sel->sym.get();
        else if (const auto* name = ir::get<ir::Name>(label)) sym = name->sym.get();
        std::size_t k = 0;
        for (const ir::Node* x : enum_class->members()) {
            if (const auto* ec = ir::get<ir::EnumConst>(x)) {
                if (sym != nullptr && need_sym(ec->sym, "an enum constant").name == sym->name) return std::to_string(k);
                ++k;
            }
        }
        throw Decline("enum case label");
    }
    if (ir::is<ir::Lit>(label)) return expr(label).code;
    if (const auto* u = ir::get<ir::Unary>(label); u != nullptr && u->op == "-" && ir::is<ir::Lit>(u->x)) return "-" + expr(u->x).code;
    if (const auto* sel = ir::get<ir::Select>(label); sel != nullptr && sel->sym) {   // a constant field of the program
        if (const ClassInfo* owner = p_.model().info(sel->sym->owner.value_or(""))) {
            for (const ir::Node* fn : owner->fields()) {
                const ir::FieldDecl& f = field_of(fn);
                if (f.sym && f.sym->name == sel->sym->name) {
                    if (f.init != nullptr) return case_label(f.init, nullptr);
                    break;
                }
            }
        }
    }
    throw Decline("case label");
}

void MethodWriter::emit(const ir::Node&, const ir::Switch& k, Lines& out, std::string_view ind) {
    const Code scrutinee = expr(cpp03::strip_paren(k.expr));
    std::string x = scrutinee.code;
    const ir::TypeP type = type_of(k.expr);
    const ClassInfo* enum_class = p_.model().of_type(type);
    if (enum_class != nullptr && enum_class->kind == "enum") {
        x = std::format("jrt_Enum_ordinal_({})", x);
    } else if (scrutinee.ctype == kObj) {   // a boxed value: switch on what it holds
        const std::string_view letter = box_letter(class_name(type));
        x = conv(x, scrutinee.ctype, letter_ctype(letter.empty() ? std::string_view("I") : letter));
        enum_class = nullptr;
    } else {
        enum_class = nullptr;
    }
    out.push_back(std::format("{}switch ({}) {{", ind, x));
    {
        ++loops_;   // a break leaves the switch
        for (const ir::Node* cn : k.cases) {
            const auto* c = ir::get<ir::Case>(cn);
            if (c == nullptr) throw Decline("a switch member that is not a case");
            for (const ir::Node* label : c->labels) out.push_back(std::format("{}case {}:", ind, case_label(label, enum_class)));
            if (c->is_default) out.push_back(std::format("{}default:", ind));
            out.push_back(std::format("{}{{", ind));
            const auto saved = vars_;
            for (const ir::Node* b : c->body) stmt(b, out, std::string(ind) + "\t");
            vars_ = saved;
            out.push_back(std::format("{}}}", ind));
        }
        --loops_;
    }
    out.push_back(std::format("{}}}", ind));
}

void MethodWriter::emit(const ir::Node&, const ir::Try& k, Lines& out, std::string_view ind) {
    if (!k.resources.empty()) throw Decline("try-with-resources");
    if (k.finally_body == nullptr) {
        try_catch(k, out, ind);
        return;
    }
    std::vector<const ir::Node*> bodies{k.body};
    for (const ir::Node* c : k.catches) {
        const auto* cc = ir::get<ir::Catch>(c);
        if (cc == nullptr) throw Decline("a catch that is not a Catch");
        bodies.push_back(cc->body);
    }
    if (cpp03::leaves(bodies)) throw Decline("a finally left by a jump");
    // the finally runs after the body and its catches, or, when something escapes them, before the exception goes on
    const std::string f = std::format("f{}", ++temps_);
    out.push_back(std::format("{}{{", ind));
    out.push_back(std::format("{}\tjrt_try {};", ind, f));
    out.push_back(std::format("{}\tjrt_push(&{});", ind, f));
    out.push_back(std::format("{}\tif (setjmp({}.jb) == 0) {{", ind, f));
    tries_.push_back(Handler{f, loops_});
    try_catch(k, out, std::string(ind) + "\t\t");
    tries_.pop_back();
    out.push_back(std::format("{}\t\tjrt_pop(&{});", ind, f));
    out.push_back(std::format("{}\t}} else {{", ind));
    out.push_back(std::format("{}\t\tjrt_pop(&{});", ind, f));
    block(k.finally_body, out, std::string(ind) + "\t");
    out.push_back(std::format("{}\t\tjrt_throw({}.ex);", ind, f));
    out.push_back(std::format("{}\t}}", ind));
    block(k.finally_body, out, ind);
    out.push_back(std::format("{}}}", ind));
}

void MethodWriter::try_catch(const ir::Try& s, Lines& out, std::string_view ind) {
    if (s.catches.empty()) {
        out.push_back(std::format("{}{{", ind));
        block(s.body, out, ind);
        out.push_back(std::format("{}}}", ind));
        return;
    }
    const std::string t = std::format("t{}", ++temps_);
    out.push_back(std::format("{}{{", ind));
    out.push_back(std::format("{}\tjrt_try {};", ind, t));
    out.push_back(std::format("{}\tjrt_push(&{});", ind, t));
    out.push_back(std::format("{}\tif (setjmp({}.jb) == 0) {{", ind, t));
    tries_.push_back(Handler{t, loops_});
    block(s.body, out, std::string(ind) + "\t");
    tries_.pop_back();
    out.push_back(std::format("{}\t\tjrt_pop(&{});", ind, t));
    out.push_back(std::format("{}\t}} else {{", ind));
    out.push_back(std::format("{}\t\tjrt_pop(&{});", ind, t));
    bool first = true;
    for (const ir::Node* cn : s.catches) {
        const auto* c = ir::get<ir::Catch>(cn);
        if (c == nullptr) throw Decline("a catch that is not a Catch");
        const std::string name = ident(need_sym(c->sym, "a catch parameter").name);
        std::vector<std::string> tests;
        for (const ir::TypeP& x : c->types) tests.push_back(std::format("jrt_instanceof({}.ex, {})", t, p_.class_ref(class_name(x))));
        std::string ct = c->types.size() == 1 ? p_.ct(c->types[0]) : "jrt_Throwable*";
        if (ct == kObj) ct = "jrt_Throwable*";
        out.push_back(std::format("{}\t\t{}if ({}) {{", ind, first ? "" : "} else ", text::join(tests, " || ")));
        first = false;
        const auto saved = vars_;
        vars_[name] = ct;
        out.push_back(std::format("{}\t\t\t{} {} = ({}){}.ex;", ind, ct, name, ct, t));
        block(c->body, out, std::string(ind) + "\t\t");
        vars_ = saved;
    }
    out.push_back(std::format("{}\t\t}} else {{", ind));
    out.push_back(std::format("{}\t\t\tjrt_throw({}.ex);", ind, t));
    out.push_back(std::format("{}\t\t}}", ind));
    out.push_back(std::format("{}\t}}", ind));
    out.push_back(std::format("{}}}", ind));
}

}  // namespace cb::translate::backends::c_java::detail
