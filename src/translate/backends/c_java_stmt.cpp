// translate/backends/c_java_stmt.cpp -- statements in the C back end for Java: blocks, locals, the loops, labelled jumps, switch,
// and try/catch/finally over the runtime's handler chain (see c_java.hpp).

#include <array>
#include <format>

#include "core/text.hpp"
#include "translate/backends/c_java_internal.hpp"

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

bool MethodWriter::must_finish(std::size_t depth) const {
    for (std::size_t i = depth; i < tries_.size(); ++i) {
        if (tries_[i].finally_body != nullptr || !tries_[i].close.empty()) return true;
    }
    return false;
}

void MethodWriter::unwind(std::size_t depth, Lines& out, std::string_view ind) {
    const std::vector<Handler> open = tries_;
    while (tries_.size() > depth) {   // what runs here is outside the handler it belongs to: a throw from it goes further out
        const Handler h = tries_.back();
        tries_.pop_back();
        out.push_back(std::format("{}jrt_pop(&{});", ind, h.name));
        if (!h.close.empty()) out.push_back(std::format("{}{}", ind, h.close));
        if (h.finally_body != nullptr) {
            out.push_back(std::format("{}{{", ind));
            block(h.finally_body, out, ind);
            out.push_back(std::format("{}}}", ind));
        }
    }
    tries_ = open;
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
    label_depth_[label] = tries_.size();
    stmt(body, out, ind);
    if (broken) out.push_back(std::format("{}{}_brk:;", ind, label));
}

void MethodWriter::emit(const ir::Node&, const ir::Return& k, Lines& out, std::string_view ind) {
    const std::string popped = pops(0);
    if (must_finish(0)) {   // the value is worked out first; the finallys and closes run after it, and may not see their own names change it
        const std::string inner = std::string(ind) + "\t";
        out.push_back(std::format("{}{{", ind));
        std::string result;
        if (k.expr != nullptr) {
            const Code value = expr(k.expr);
            result = std::format("jrt_r{}", ++temps_);
            out.push_back(std::format("{}{} {} = {};", inner, ret_, result, conv(value.code, value.ctype, ret_)));
        }
        unwind(0, out, inner);
        out.push_back(result.empty() ? inner + "return;" : std::format("{}return {};", inner, result));
        out.push_back(std::format("{}}}", ind));
        return;
    }
    if (k.expr == nullptr) {
        out.push_back(std::format("{}{}return;", ind, popped));
        return;
    }
    const Code value = expr(k.expr);
    const std::string v = conv(value.code, value.ctype, ret_);
    if (!popped.empty()) {
        out.push_back(std::format("{}{{ {} r_ = {}; {}return r_; }}", ind, ret_, v, popped));   // the value survives the handlers' pops
    } else {
        out.push_back(std::format("{}return {};", ind, v));
    }
}

void MethodWriter::jump(bool is_break, const std::optional<std::string>& label, Lines& out, std::string_view ind) {
    const bool labelled = label && !label->empty();
    std::size_t outside = 0;   // the handlers that stay open: those around the loop, or around the label
    if (!labelled) {
        for (const Handler& h : tries_) outside += h.loops < loops_;
    } else if (const auto at = label_depth_.find(*label); at != label_depth_.end()) {
        outside = at->second;
    }
    const std::string word = labelled ? std::format("goto {}_{};", *label, is_break ? "brk" : "cont") : (is_break ? "break;" : "continue;");
    if (must_finish(outside)) {
        out.push_back(std::format("{}{{", ind));
        unwind(outside, out, std::string(ind) + "\t");
        out.push_back(std::format("{}\t{}", ind, word));
        out.push_back(std::format("{}}}", ind));
        return;
    }
    out.push_back(std::format("{}{}{}", ind, pops(outside), word));
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
    const bool on_string = class_name(type) == "java.lang.String";
    std::size_t string_case = 0;   // a switch on a String is on the place of the label it equals
    if (on_string) {
        std::vector<std::string> labels;
        for (const ir::Node* cn : k.cases) {
            const auto* c = ir::get<ir::Case>(cn);
            if (c == nullptr) throw Decline("a switch member that is not a case");
            for (const ir::Node* label : c->labels) {
                const Code text = expr(cpp03::strip_paren(label));
                labels.push_back(conv(text.code, text.ctype, kStr));
            }
        }
        x = labels.empty() ? std::format("jrt_string_case({}, 0, NULL)", x)
                           : std::format("jrt_string_case({}, {}, (jstring*[]){{{}}})", x, labels.size(), join_commas(labels));
        enum_class = nullptr;
    } else if (enum_class != nullptr && enum_class->kind == "enum") {
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
            for (const ir::Node* label : c->labels)
                out.push_back(std::format("{}case {}:", ind, on_string ? std::to_string(string_case++) : case_label(label, enum_class)));
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
    // A finally is written once for each way out, so a C label in it would be defined twice; and written at a jump inside a deeper loop,
    // a `break` of its own would leave the wrong loop.
    const std::array<const ir::Node*, 1> finally_only{k.finally_body};
    if (any_node(k.finally_body, [](const ir::Node* x) { return ir::is<ir::Labeled>(x); })) throw Decline("a label in a finally");
    if (cpp03::leaves(bodies) && cpp03::leaves(finally_only)) throw Decline("a finally that jumps, around a body that jumps");
    // the finally runs after the body and its catches, or, when something escapes them, before the exception goes on
    const std::string f = std::format("f{}", ++temps_);
    out.push_back(std::format("{}{{", ind));
    out.push_back(std::format("{}\tjrt_try {};", ind, f));
    out.push_back(std::format("{}\tjrt_push(&{});", ind, f));
    out.push_back(std::format("{}\tif (setjmp({}.jb) == 0) {{", ind, f));
    tries_.push_back(Handler{f, loops_, k.finally_body, {}});
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

// `r.close()` for a resource: through the table of the program's class or interface that declares it.
std::string MethodWriter::close_call(const std::string& resource, const ir::TypeP& type) {
    for (const std::string& q : p_.model().ancestors(std::string(class_name(type)))) {
        const ClassInfo* c = p_.model().info(q);
        if (c == nullptr) continue;
        for (const ir::Node* mn : c->methods("close")) {
            const ir::MethodDecl& m = method_of(mn);
            if (!m.params.empty() || is_static(m)) continue;
            const std::string name = p_.mname("close", p_.sig(*c, mn).eps);
            if (c->kind == "interface") return std::format("{}_{}_i((jobject*){})", c->cname, name, resource);
            return std::format("{}_{}_v((jobject*){})", slot_owner(c, name)->cname, name, resource);
        }
    }
    throw Decline("a resource of a library class");
}

// Each resource is open around the ones after it and the body. Leaving normally or by a jump closes it; an exception closes it too, and
// what that close throws is added to the exception as suppressed. A null resource is not closed.
void MethodWriter::try_body(const ir::Try& s, std::size_t at, Lines& out, std::string_view ind) {
    if (at == s.resources.size()) {
        block(s.body, out, ind);
        return;
    }
    const auto saved = vars_;
    const std::string i1 = std::string(ind) + "\t", i2 = i1 + "\t", i3 = i2 + "\t";
    out.push_back(std::format("{}{{", i1));
    const ir::Node* r = s.resources[at];
    std::string name;
    ir::TypeP type;
    if (const auto* local = ir::get<ir::Local>(r)) {
        stmt(r, out, i2);
        name = ident(need_sym(local->sym, "a resource").name);
        type = local->type;
    } else {   // `try (existing)`
        if (const auto* written = ir::get<ir::ExprStmt>(r)) r = written->expr;
        const Code value = expr(r);
        type = type_of(r);
        name = std::format("res{}", ++temps_);
        const std::string ct = p_.ct(type);
        out.push_back(i2 + decl(ct, name, conv(value.code, value.ctype, ct)));
    }
    const std::string call = close_call(name, type);
    const std::string h = std::format("h{}", ++temps_), c = std::format("c{}", ++temps_);
    out.push_back(std::format("{}jrt_try {};", i2, h));
    out.push_back(std::format("{}jrt_push(&{});", i2, h));
    out.push_back(std::format("{}if (setjmp({}.jb) == 0) {{", i2, h));
    tries_.push_back(Handler{h, loops_, nullptr, std::format("if ({} != NULL) {};", name, call)});
    try_body(s, at + 1, out, i2);
    tries_.pop_back();
    out.push_back(std::format("{}jrt_pop(&{});", i3, h));
    out.push_back(std::format("{}}} else {{", i2));
    out.push_back(std::format("{}jrt_pop(&{});", i3, h));
    out.push_back(std::format("{}if ({} != NULL) {{", i3, name));
    out.push_back(std::format("{}\tjrt_try {};", i3, c));
    out.push_back(std::format("{}\tjrt_push(&{});", i3, c));
    out.push_back(std::format("{}\tif (setjmp({}.jb) == 0) {{ {}; jrt_pop(&{}); }}", i3, c, call, c));
    out.push_back(std::format("{}\telse {{ jrt_pop(&{}); jrt_Throwable_addSuppressed_O({}.ex, {}.ex); }}", i3, c, h, c));
    out.push_back(std::format("{}}}", i3));
    out.push_back(std::format("{}jrt_throw({}.ex);", i3, h));
    out.push_back(std::format("{}}}", i2));
    out.push_back(std::format("{}if ({} != NULL) {};", i2, name, call));
    out.push_back(std::format("{}}}", i1));
    vars_ = saved;
}

void MethodWriter::try_catch(const ir::Try& s, Lines& out, std::string_view ind) {
    if (s.catches.empty()) {
        out.push_back(std::format("{}{{", ind));
        try_body(s, 0, out, ind);
        out.push_back(std::format("{}}}", ind));
        return;
    }
    const std::string t = std::format("t{}", ++temps_);
    out.push_back(std::format("{}{{", ind));
    out.push_back(std::format("{}\tjrt_try {};", ind, t));
    out.push_back(std::format("{}\tjrt_push(&{});", ind, t));
    out.push_back(std::format("{}\tif (setjmp({}.jb) == 0) {{", ind, t));
    tries_.push_back(Handler{t, loops_, nullptr, {}});
    try_body(s, 0, out, std::string(ind) + "\t");
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
