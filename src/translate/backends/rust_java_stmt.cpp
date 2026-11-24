// translate/backends/rust_java_stmt.cpp -- statements of the Rust back end for Java: blocks, locals, branches, loops and the jumps out of them,
// switches, return and throw, try/catch/finally (see rust_java.hpp).

#include <algorithm>
#include <format>

#include "translate/backends/rust_java_internal.hpp"

namespace cb::translate::backends::rust_java::detail {

namespace {

Code mk_code(std::string text, ir::TypeP type) {
    Code c;
    c.text = std::move(text);
    c.type = std::move(type);
    return c;
}

bool is_loop(const ir::Node* s) { return ir::is<ir::While>(s) || ir::is<ir::DoWhile>(s) || ir::is<ir::For>(s); }

// A statement that always leaves the case it ends.
bool leaves(const ir::Node* s) {
    s = strip_paren(s);
    if (s == nullptr) return false;
    if (ir::is<ir::Break>(s) || ir::is<ir::Continue>(s) || ir::is<ir::Return>(s) || ir::is<ir::Throw>(s)) return true;
    if (const auto* b = ir::get<ir::Block>(s)) return !b->stmts.empty() && leaves(b->stmts.back());
    if (const auto* i = ir::get<ir::If>(s)) {
        const std::vector<ir::Node*> a = ir::statements(i->then), b = ir::statements(i->orelse);
        return !a.empty() && !b.empty() && leaves(a.back()) && leaves(b.back());
    }
    return false;
}

// A break with no label that ends the statement list's own switch: not inside a loop or another switch.
bool has_plain_break(const ir::Node* s) {
    if (s == nullptr) return false;
    if (const auto* b = ir::get<ir::Break>(s)) return !b->label;
    if (is_loop(s) || ir::is<ir::Switch>(s) || ir::is<ir::Lambda>(s) || ir::is<ir::ClassDecl>(s)) return false;
    for (const ir::Node* c : ir::children(*s))
        if (has_plain_break(c)) return true;
    return false;
}

// The statements a Block, or a lone statement, holds.
std::vector<ir::Node*> stmts_of(const ir::Node* n) {
    if (const auto* b = ir::get<ir::Block>(n)) return b->stmts;
    if (n == nullptr) return {};
    return {const_cast<ir::Node*>(n)};
}

}  // namespace

Writer::Writer(Program& p, const ClassInfo* cls, const ir::MethodDecl* m) : p_(p), ci_(cls), m_(m) {}

void Writer::note_assigned(std::span<ir::Node* const> stmts) {
    for (const ir::Node* root : stmts) {
        for_each_node(root, [&](const ir::Node* n) {
            if (const auto* a = ir::get<ir::Assign>(n)) {
                if (const auto* name = ir::get<ir::Name>(strip_paren(a->target)); name != nullptr && name->sym) assigned_.insert(ident(name->sym->name));
            } else if (const auto* u = ir::get<ir::Unary>(n); u != nullptr && (u->op == "++" || u->op == "--")) {
                if (const auto* name = ir::get<ir::Name>(strip_paren(u->x)); name != nullptr && name->sym) assigned_.insert(ident(name->sym->name));
            } else if (ir::is<ir::Try>(n)) {
                has_try_ = true;
            }
        });
    }
}

Lines Writer::body(std::span<ir::Node* const> stmts, const ir::TypeP& ret, std::size_t depth) {
    ret_ = ret;
    Lines out;
    const bool value = ret && kind_of(ret) != "void";
    for (std::size_t i = 0; i < stmts.size(); ++i) {
        const ir::Node* s = stmts[i];
        const bool last = i + 1 == stmts.size();
        if (last && ir::is<ir::Return>(s)) {
            const auto* r = ir::get<ir::Return>(s);
            if (r->expr == nullptr) break;   // a trailing `return;` says nothing
            out.push_back(indent_of(depth) + value_of(r->expr, ret));   // the function's value
            return out;
        }
        stmt(s, out, depth);
        if (last && value && !leaves(s)) out.push_back(indent_of(depth) + "unreachable!()");
    }
    return out;
}

std::string Writer::value_of(const ir::Node* e, const ir::TypeP& want) {
    if (const auto* na = ir::get<ir::NewArray>(strip_paren(e)); na != nullptr && !e->type) array_hint_[e] = want;
    return conv(expr(e), want).text;
}

std::string Writer::as_arg(const ir::Node* e, const ir::TypeP& param) { return value_of(e, param); }

void Writer::stmts(std::span<ir::Node* const> list, Lines& out, std::size_t depth) {
    for (const ir::Node* s : list) stmt(s, out, depth);
}

void Writer::suite(const ir::Suite& s, Lines& out, std::size_t depth) {
    const std::vector<ir::Node*> list = ir::statements(s);
    if (list.size() == 1) {
        if (const auto* b = ir::get<ir::Block>(list[0])) {
            stmts(b->stmts, out, depth);
            return;
        }
    }
    stmts(list, out, depth);
}

std::string Writer::cond_text(const ir::Node* e) {
    const Code c = conv(expr(e), ir::bool_type());
    return c.text;
}

void Writer::stmt(const ir::Node* s, Lines& out, std::size_t depth) {
    if (s == nullptr) return;
    const std::string ind = indent_of(depth);
    if (const auto* b = ir::get<ir::Block>(s)) {
        out.push_back(ind + "{");
        stmts(b->stmts, out, depth + 1);
        out.push_back(ind + "}");
    } else if (const auto* l = ir::get<ir::Local>(s)) {
        emit_local(*s, *l, out, depth);
    } else if (const auto* e = ir::get<ir::ExprStmt>(s)) {
        emit_expr_stmt(e->expr, out, depth);
    } else if (const auto* i = ir::get<ir::If>(s)) {
        emit_if(*i, out, depth);
    } else if (const auto* w = ir::get<ir::While>(s)) {
        emit_while(*w, std::nullopt, out, depth);
    } else if (const auto* d = ir::get<ir::DoWhile>(s)) {
        emit_do(*d, std::nullopt, out, depth);
    } else if (const auto* f = ir::get<ir::For>(s)) {
        emit_for(*f, std::nullopt, out, depth);
    } else if (const auto* lab = ir::get<ir::Labeled>(s)) {
        const std::vector<ir::Node*> inner = ir::statements(lab->body);
        const ir::Node* target = inner.size() == 1 ? inner[0] : nullptr;
        if (const auto* lw = ir::get<ir::While>(target)) emit_while(*lw, lab->label, out, depth);
        else if (const auto* ld = ir::get<ir::DoWhile>(target)) emit_do(*ld, lab->label, out, depth);
        else if (const auto* lf = ir::get<ir::For>(target)) emit_for(*lf, lab->label, out, depth);
        else {   // a labeled block: `break label` leaves it
            Target t;
            t.kind = Target::Kind::Block;
            t.java = lab->label;
            t.labeled_block = true;
            t.tries = tries_;
            t.label = "l" + std::to_string(++labels_);
            targets_.push_back(t);
            Lines inner_lines;
            stmts(inner, inner_lines, depth + 1);
            targets_.pop_back();
            out.push_back(ind + "'" + t.label + ": {");
            out.insert(out.end(), inner_lines.begin(), inner_lines.end());
            out.push_back(ind + "}");
        }
    } else if (const auto* sw = ir::get<ir::Switch>(s)) {
        emit_switch(*sw, out, depth);
    } else if (const auto* r = ir::get<ir::Return>(s)) {
        emit_return(*r, out, depth);
    } else if (const auto* br = ir::get<ir::Break>(s)) {
        emit_jump(true, br->label, out, depth);
    } else if (const auto* co = ir::get<ir::Continue>(s)) {
        emit_jump(false, co->label, out, depth);
    } else if (const auto* th = ir::get<ir::Throw>(s)) {
        const Code c = conv(expr(th->expr), ir::class_type("java.lang.Throwable"));
        out.push_back(ind + "throw(" + c.text + ");");
    } else if (const auto* t = ir::get<ir::Try>(s)) {
        emit_try(*t, out, depth);
    } else if (ir::is<ir::Empty>(s) || ir::is<ir::Comment>(s) || ir::is<ir::Assert>(s)) {
        // (an assert is off unless the JVM is given -ea)
    } else if (const auto* sy = ir::get<ir::Sync>(s)) {
        stmt(sy->body, out, depth);
    } else if (ir::is<ir::CtorCall>(s)) {
        throw Decline("a constructor call that is not the constructor's first statement");
    } else if (ir::is<ir::LocalClass>(s)) {
        throw Decline("a local class");
    } else {
        throw Decline("the statement " + std::string(ir::kind_name(*s)));
    }
}

void Writer::emit_local(const ir::Node&, const ir::Local& k, Lines& out, std::size_t depth) {
    const ir::Sym& sym = need_sym(k.sym, "a local");
    const std::string name = ident(sym.name);
    const ir::TypeP type = k.type ? k.type : sym.type;
    const std::string rt = p_.rtype(type);
    const bool mut = assigned_.contains(name);
    std::string line = indent_of(depth) + "let " + (mut ? "mut " : "") + name + ": " + rt;
    if (k.init != nullptr) line += " = " + value_of(k.init, type);
    else if (has_try_) line += " = Default::default()";
    out.push_back(line + ";");
}

void Writer::emit_expr_stmt(const ir::Node* e, Lines& out, std::size_t depth) {
    const std::string ind = indent_of(depth);
    e = strip_paren(e);
    if (const auto* a = ir::get<ir::Assign>(e)) {
        out.push_back(ind + assign_code(a->target, a->value, a->op, false, type_of(e)).text + ";");
    } else if (const auto* u = ir::get<ir::Unary>(e); u != nullptr && (u->op == "++" || u->op == "--")) {
        out.push_back(ind + incdec(*u, false).text + ";");
    } else if (ir::is<ir::Call>(e)) {
        const Code c = expr(e);
        out.push_back(ind + c.text + ";");
    } else {
        out.push_back(ind + "let _ = " + expr(e).text + ";");
    }
}

void Writer::emit_if(const ir::If& k, Lines& out, std::size_t depth) {
    const std::string ind = indent_of(depth);
    out.push_back(ind + "if " + cond_text(k.cond) + " {");
    suite(k.then, out, depth + 1);
    const ir::If* chain = &k;
    while (ir::present(chain->orelse)) {
        const std::vector<ir::Node*> rest = ir::statements(chain->orelse);
        if (rest.size() == 1 && ir::is<ir::If>(rest[0])) {
            chain = ir::get<ir::If>(rest[0]);
            out.push_back(ind + "} else if " + cond_text(chain->cond) + " {");
            suite(chain->then, out, depth + 1);
            continue;
        }
        out.push_back(ind + "} else {");
        suite(chain->orelse, out, depth + 1);
        break;
    }
    out.push_back(ind + "}");
}

// Whether a `continue` in the body ends this loop's turn (an unlabeled one not inside an inner loop, or one naming the loop's label).
bool Writer::body_continues(const ir::Suite& body, const std::optional<std::string>& label) const {
    const std::function<bool(const ir::Node*, bool)> scan = [&](const ir::Node* n, bool inner) -> bool {
        if (n == nullptr) return false;
        if (const auto* c = ir::get<ir::Continue>(n)) return c->label ? (label && *c->label == *label) : !inner;
        if (ir::is<ir::Lambda>(n) || ir::is<ir::ClassDecl>(n)) return false;
        const bool nested = inner || is_loop(n);
        for (const ir::Node* child : ir::children(*n))
            if (scan(child, nested)) return true;
        return false;
    };
    for (const ir::Node* s : ir::statements(body))
        if (scan(s, false)) return true;
    return false;
}

// A loop's body is written inside the Target (so jumps find it); `header` is the loop line without its label or brace.
void Writer::looped(Target t, const ir::Suite& body, std::string_view header, bool wrap_cont, Lines& out, std::size_t depth,
                    const std::function<void(Lines&, std::size_t)>& after_body) {
    t.kind = Target::Kind::Loop;
    t.tries = tries_;
    t.label = "l" + std::to_string(++labels_);
    if (t.java) t.label = *t.java;
    t.has_cont = wrap_cont;
    t.cont = "c" + std::to_string(labels_);
    t.labeled_block = false;
    targets_.push_back(t);
    const std::size_t at = targets_.size() - 1;
    Lines inner;
    if (wrap_cont) {
        inner.push_back(indent_of(depth + 1) + "'" + t.cont + ": {");
        suite(body, inner, depth + 2);
        inner.push_back(indent_of(depth + 1) + "}");
    } else {
        suite(body, inner, depth + 1);
    }
    if (after_body) after_body(inner, depth + 1);
    const Target done = targets_[at];
    targets_.pop_back();
    out.push_back(indent_of(depth) + (done.label_used ? "'" + done.label + ": " : std::string()) + std::string(header) + " {");
    out.insert(out.end(), inner.begin(), inner.end());
    out.push_back(indent_of(depth) + "}");
}

void Writer::emit_while(const ir::While& k, std::optional<std::string> label, Lines& out, std::size_t depth) {
    Target t;
    t.java = label;
    const auto* lit = ir::get<ir::Lit>(strip_paren(k.cond));
    const bool forever = lit != nullptr && std::holds_alternative<bool>(lit->value) && std::get<bool>(lit->value);
    looped(t, k.body, forever ? "loop" : "while " + cond_text(k.cond), false, out, depth);
}

void Writer::emit_do(const ir::DoWhile& k, std::optional<std::string> label, Lines& out, std::size_t depth) {
    Target t;
    t.java = label;
    const bool cont = body_continues(k.body, label);
    const std::string cond = cond_text(k.cond);
    looped(t, k.body, "loop", cont, out, depth, [&](Lines& lines, std::size_t d) {
        lines.push_back(indent_of(d) + "if !(" + cond + ") {");
        lines.push_back(indent_of(d + 1) + "break;");
        lines.push_back(indent_of(d) + "}");
    });
}

// for (int i = a; i < b; i++) as `for i in a..b`: when nothing in the body changes i or b, so the bound is the same every turn.
bool Writer::emit_range_for(const ir::For& k, std::optional<std::string> label, Lines& out, std::size_t depth) {
    if (k.init.size() != 1 || k.update.size() != 1 || k.cond == nullptr) return false;
    const auto* local = ir::get<ir::Local>(k.init[0]);
    if (local == nullptr || local->init == nullptr || !local->sym) return false;
    const ir::TypeP type = local->type ? local->type : local->sym->type;
    const std::string_view kind = kind_of(type);
    if (kind != "i32" && kind != "i64") return false;
    const auto* cond = ir::get<ir::Binary>(strip_paren(k.cond));
    if (cond == nullptr || cond->op != "<") return false;
    const auto* lhs = ir::get<ir::Name>(strip_paren(cond->left));
    if (lhs == nullptr || !lhs->sym || lhs->sym->name != local->sym->name) return false;
    // the update is i++, ++i or i += 1
    const ir::Node* upd = strip_paren(ir::get<ir::ExprStmt>(k.update[0]) != nullptr ? ir::get<ir::ExprStmt>(k.update[0])->expr : k.update[0]);
    bool steps = false;
    if (const auto* u = ir::get<ir::Unary>(upd); u != nullptr && u->op == "++") {
        const auto* n = ir::get<ir::Name>(strip_paren(u->x));
        steps = n != nullptr && n->sym && n->sym->name == local->sym->name;
    } else if (const auto* a = ir::get<ir::Assign>(upd); a != nullptr && a->op == "+") {
        const auto* n = ir::get<ir::Name>(strip_paren(a->target));
        const auto* one = ir::get<ir::Lit>(strip_paren(a->value));
        steps = n != nullptr && n->sym && n->sym->name == local->sym->name && one != nullptr && std::holds_alternative<std::int64_t>(one->value) &&
                std::get<std::int64_t>(one->value) == 1;
    }
    if (!steps) return false;
    // the bound: a literal, or a local (or an array's length through one) the body leaves alone
    const ir::Node* bound = strip_paren(cond->right);
    const ir::Node* bound_name = bound;
    if (const auto* len = ir::get<ir::Length>(bound)) bound_name = strip_paren(len->arr);
    const bool literal_bound = ir::is<ir::Lit>(bound);
    const auto* bn = ir::get<ir::Name>(bound_name);
    if (!literal_bound && (bn == nullptr || !bn->sym)) return false;
    const std::string_view bound_kind = kind_of(type_of(bound));
    if (!is_integral_kind(bound_kind) || (kind == "i32" && bound_kind == "i64")) return false;
    bool disturbed = false;
    for (const ir::Node* s : ir::statements(k.body)) {
        for_each_node(s, [&](const ir::Node* n) {
            const ir::Node* assigned = nullptr;
            if (const auto* a = ir::get<ir::Assign>(n)) assigned = strip_paren(a->target);
            else if (const auto* u = ir::get<ir::Unary>(n); u != nullptr && (u->op == "++" || u->op == "--")) assigned = strip_paren(u->x);
            if (const auto* an = ir::get<ir::Name>(assigned); an != nullptr && an->sym) {
                if (an->sym->name == local->sym->name || (bn != nullptr && bn->sym && an->sym->name == bn->sym->name)) disturbed = true;
            }
            if (ir::is<ir::Lambda>(n) || ir::is<ir::ClassDecl>(n)) disturbed = true;
        });
    }
    if (disturbed) return false;
    const std::string name = ident(local->sym->name);
    const Code first = conv(expr(local->init), type);
    const std::string start = first.bare ? first.text + std::string(kind) : wrap(first, kShift + 1);
    const Code end = conv(expr(cond->right), type);
    Target t;
    t.java = label;
    looped(t, k.body, "for " + name + " in " + start + ".." + wrap(end, kShift + 1), false, out, depth);
    return true;
}

void Writer::emit_for(const ir::For& k, std::optional<std::string> label, Lines& out, std::size_t depth) {
    if (emit_range_for(k, label, out, depth)) return;
    const std::string ind = indent_of(depth);
    const bool scoped = !k.init.empty();
    if (scoped) out.push_back(ind + "{");
    const std::size_t d = scoped ? depth + 1 : depth;
    for (const ir::Node* i : k.init) stmt(i, out, d);
    const bool has_update = !k.update.empty();
    const bool cont = has_update && body_continues(k.body, label);
    Target t;
    t.java = label;
    std::string header = "loop";
    if (k.cond != nullptr) {
        const auto* lit = ir::get<ir::Lit>(strip_paren(k.cond));
        if (!(lit != nullptr && std::holds_alternative<bool>(lit->value) && std::get<bool>(lit->value))) header = "while " + cond_text(k.cond);
    }
    looped(t, k.body, header, cont, out, d, [&](Lines& lines, std::size_t dd) {
        for (const ir::Node* u : k.update) {
            const auto* es = ir::get<ir::ExprStmt>(u);
            emit_expr_stmt(es != nullptr ? es->expr : u, lines, dd);
        }
    });
    if (scoped) out.push_back(ind + "}");
}

void Writer::emit_jump(bool is_break, const std::optional<std::string>& label, Lines& out, std::size_t depth) {
    int idx = -1;
    for (std::size_t i = targets_.size(); i-- > 0;) {
        const Target& t = targets_[i];
        if (label) {
            if (t.java == label) {
                idx = static_cast<int>(i);
                break;
            }
        } else if (is_break) {
            if (t.kind == Target::Kind::Loop || t.kind == Target::Kind::Switch) {
                idx = static_cast<int>(i);
                break;
            }
        } else if (t.kind == Target::Kind::Loop) {
            idx = static_cast<int>(i);
            break;
        }
    }
    if (idx < 0) throw Decline("a break or continue with nothing to leave");
    Target& t = targets_[idx];
    if (!try_ctx_.empty() && static_cast<std::size_t>(idx) < try_ctx_.back().base) {   // out of a try closure: it ends with the jump to be made after it
        TryCtx& ctx = try_ctx_.back();
        out.push_back(indent_of(depth) + "return Flow::" + (is_break ? "Break" : "Continue") + "(" + std::to_string(ctx.jumps.size()) + ");");
        ctx.jumps.emplace_back(is_break, label);
        return;
    }
    bool between = false;   // something between the jump and its target that a bare break or continue would stop at instead
    for (std::size_t j = idx + 1; j < targets_.size(); ++j)
        if (targets_[j].kind == Target::Kind::Loop || targets_[j].labeled_block || targets_[j].has_cont) between = true;
    std::string text;
    if (t.kind == Target::Kind::Switch || t.kind == Target::Kind::Block) {
        t.label_used = true;
        text = "break '" + t.label + ";";
    } else if (is_break) {
        if (between || t.has_cont) {
            t.label_used = true;
            text = "break '" + t.label + ";";
        } else {
            text = "break;";
        }
    } else if (t.has_cont) {
        t.cont_used = true;
        text = "break '" + t.cont + ";";
    } else if (between) {
        t.label_used = true;
        text = "continue '" + t.label + ";";
    } else {
        text = "continue;";
    }
    out.push_back(indent_of(depth) + text);
}

void Writer::emit_return(const ir::Return& k, Lines& out, std::size_t depth) {
    const bool value = k.expr != nullptr;
    const std::string v = value ? value_of(k.expr, ret_) : "";
    if (tries_ > 0) out.push_back(indent_of(depth) + "return Flow::Return(" + (value ? v : "()") + ");");
    else out.push_back(indent_of(depth) + (value ? "return " + v + ";" : "return;"));
}

// ---------------------------------------------------------------------------------------------------------------- switch

std::string Writer::case_label(const ir::Node* label, const ir::TypeP& scrutinee) {
    label = strip_paren(label);
    if (const auto* lit = ir::get<ir::Lit>(label)) {
        std::int64_t v = 0;
        if (const auto* s = std::get_if<std::string>(&lit->value)) {
            unsigned char c = s->empty() ? 0 : static_cast<unsigned char>((*s)[0]);
            if (c >= 0x80) throw Decline("a non-ASCII char case label");
            v = c;
        } else if (const auto* i = std::get_if<std::int64_t>(&lit->value)) {
            v = *i;
        } else {
            throw Decline("a case label that is not a number or char");
        }
        return std::to_string(v);
    }
    if (const auto* u = ir::get<ir::Unary>(label); u != nullptr && u->op == "-")
        if (ir::is<ir::Lit>(u->x)) return "-" + case_label(u->x, scrutinee);
    if (ir::is<ir::Select>(label) || ir::is<ir::Name>(label)) return expr(label).text;   // a constant
    throw Decline("a case label that is not a constant");
}

bool Writer::terminates(const ir::Node* s) const { return leaves(s); }

// An enum case label is its constant's ordinal.
std::string Writer::enum_case_label(const ir::Node* label, const ClassInfo& cls) {
    label = strip_paren(label);
    const ir::Sym* sym = nullptr;
    if (const auto* sel = ir::get<ir::Select>(label)) sym = sel->sym.get();
    else if (const auto* name = ir::get<ir::Name>(label)) sym = name->sym.get();
    std::size_t ordinal = 0;
    for (const ir::Node* member : cls.members()) {
        if (const auto* ec = ir::get<ir::EnumConst>(member)) {
            if (sym != nullptr && ec->sym && ec->sym->name == sym->name) return std::to_string(ordinal);
            ++ordinal;
        }
    }
    throw Decline("an enum case label that names no constant");
}

void Writer::emit_switch(const ir::Switch& k, Lines& out, std::size_t depth) {
    const std::string ind = indent_of(depth);
    const ir::TypeP stype = type_of(k.expr);
    const std::string_view sk = kind_of(stype);
    const ClassInfo* enum_class = p_.program_class(stype);
    const bool on_enum = enum_class != nullptr && enum_class->kind == "enum";
    if (!on_enum && sk != "i32" && sk != "i16" && sk != "i8" && sk != "u16") throw Decline("a switch on " + std::string(sk == "class" ? std::string(class_name(stype)) : std::string(sk)));
    struct Arm {
        std::vector<std::string> patterns;
        bool is_default = false;
        std::vector<ir::Node*> body;
    };
    std::vector<Arm> arms;
    Arm pending;
    for (std::size_t i = 0; i < k.cases.size(); ++i) {
        const auto* c = ir::get<ir::Case>(k.cases[i]);
        if (c == nullptr) throw Decline("a switch member that is not a case");
        for (const ir::Node* label : c->labels) pending.patterns.push_back(on_enum ? enum_case_label(label, *enum_class) : case_label(label, stype));
        if (c->is_default) pending.is_default = true;
        const bool last = i + 1 == k.cases.size();
        if (c->body.empty() && !last) continue;   // labels sharing the next body
        pending.body = c->body;
        if (!pending.body.empty() && !last && !terminates(pending.body.back())) throw Decline("a switch case that falls through into the next");
        // the final break is the end of the arm
        if (!pending.body.empty()) {
            if (const auto* br = ir::get<ir::Break>(pending.body.back()); br != nullptr && !br->label) pending.body.pop_back();
        }
        arms.push_back(std::move(pending));
        pending = Arm();
    }
    bool block = false;   // a break that is not the arm's last statement needs somewhere to go
    for (const Arm& a : arms)
        for (const ir::Node* s : a.body)
            if (has_plain_break(s)) block = true;
    Target t;
    t.kind = Target::Kind::Switch;
    t.tries = tries_;
    t.label = "l" + std::to_string(++labels_);
    t.labeled_block = block;
    targets_.push_back(t);
    Lines inner;
    const std::size_t d = block ? depth + 1 : depth;
    Code scrut = expr(k.expr);
    if (on_enum) scrut = mk_code(wrap(receiver(k.expr), kAtom) + "." + p_.fields_fn(*enum_class) + "().enum_ordinal.get()", ir::i32_type());
    inner.push_back(indent_of(d) + "match " + wrap(scrut, kLow) + " {");
    const Arm* default_arm = nullptr;
    const auto write_arm = [&](const Arm& a) {
        const std::string pat = a.is_default ? "_" : join(a.patterns, " | ");
        if (a.body.empty()) {
            inner.push_back(indent_of(d + 1) + pat + " => {}");
            return;
        }
        inner.push_back(indent_of(d + 1) + pat + " => {");
        stmts(a.body, inner, d + 2);
        inner.push_back(indent_of(d + 1) + "}");
    };
    for (const Arm& a : arms) {
        if (a.is_default) default_arm = &a;
        else write_arm(a);
    }
    if (default_arm != nullptr) write_arm(*default_arm);
    else inner.push_back(indent_of(d + 1) + "_ => {}");
    inner.push_back(indent_of(d) + "}");
    targets_.pop_back();
    if (block) {
        out.push_back(ind + "'" + t.label + ": {");
        out.insert(out.end(), inner.begin(), inner.end());
        out.push_back(ind + "}");
    } else {
        out.insert(out.end(), inner.begin(), inner.end());
    }
}

// ---------------------------------------------------------------------------------------------------------------- try

// Whether a statement holds a break or continue that leaves it (one whose loop, switch or label is not inside it).
static bool jumps_out(const ir::Node* n, bool in_loop, bool in_switch, std::vector<std::string>& labels) {
    if (n == nullptr) return false;
    if (const auto* b = ir::get<ir::Break>(n)) {
        if (b->label) return std::find(labels.begin(), labels.end(), *b->label) == labels.end();
        return !in_loop && !in_switch;
    }
    if (const auto* c = ir::get<ir::Continue>(n)) {
        if (c->label) return std::find(labels.begin(), labels.end(), *c->label) == labels.end();
        return !in_loop;
    }
    if (ir::is<ir::Lambda>(n) || ir::is<ir::ClassDecl>(n)) return false;
    bool loop = in_loop, sw = in_switch;
    std::size_t pushed = 0;
    if (is_loop(n)) loop = true;
    if (ir::is<ir::Switch>(n)) sw = true;
    if (const auto* l = ir::get<ir::Labeled>(n); l != nullptr && l->label) {
        labels.push_back(*l->label);
        pushed = 1;
    }
    bool out = false;
    for (const ir::Node* child : ir::children(*n))
        if (!out && jumps_out(child, loop, sw, labels)) out = true;
    labels.resize(labels.size() - pushed);
    return out;
}

void Writer::emit_try(const ir::Try& k, Lines& out, std::size_t depth) {
    if (!k.resources.empty()) throw Decline("try-with-resources");
    const std::string ind = indent_of(depth);
    const auto leaves_in = [](const ir::Node* n) {
        std::vector<std::string> labels;
        return any_node(n, [](const ir::Node* x) { return ir::is<ir::Return>(x); }) || jumps_out(n, false, false, labels);
    };
    bool flow = leaves_in(k.body);
    for (const ir::Node* c : k.catches) flow = flow || leaves_in(c);
    const std::string ret_type = (ret_ && kind_of(ret_) != "void") ? p_.rtype(ret_) : "()";
    const std::string closure = flow ? "|| -> Flow<" + ret_type + "> {" : "|| {";

    // what a closure's `return Flow::...` become once it has ended: the function's return, and the jumps it could not make inside
    const auto outcome_arms = [&](const TryCtx& ctx, Lines& lines, std::size_t d) {
        lines.push_back(indent_of(d) + "Ok(Flow::Return(v)) => return " + (tries_ > 0 ? "Flow::Return(v)" : "v") + ",");
        for (std::size_t id = 0; id < ctx.jumps.size(); ++id) {
            lines.push_back(indent_of(d) + "Ok(Flow::" + (ctx.jumps[id].first ? "Break" : "Continue") + "(" + std::to_string(id) + ")) => {");
            emit_jump(ctx.jumps[id].first, ctx.jumps[id].second, lines, d + 1);
            lines.push_back(indent_of(d) + "}");
        }
        lines.push_back(indent_of(d) + "Ok(_) => {}");
    };
    // a closure body: its statements one level in, with the jumps that leave it recorded
    const auto closure_body = [&](const std::vector<ir::Node*>& body, Lines& lines, std::size_t d) {
        try_ctx_.push_back(TryCtx{targets_.size(), {}});
        ++tries_;
        stmts(body, lines, d);
        if (flow) lines.push_back(indent_of(d) + "Flow::Next");
        --tries_;
        TryCtx ctx = std::move(try_ctx_.back());
        try_ctx_.pop_back();
        return ctx;
    };

    // the try and its catch clauses, as statements (the whole of a try with no finally)
    const auto try_catch_part = [&](Lines& lines, std::size_t d) {
        const std::string i1 = indent_of(d);
        if (k.catches.empty()) {   // (a finally only: the body is run as it is)
            stmts(stmts_of(k.body), lines, d);
            return;
        }
        lines.push_back(i1 + (flow ? "match try_catch(" : "if let Err(ex) = try_catch(") + closure);
        const TryCtx ctx = closure_body(stmts_of(k.body), lines, d + 1);
        lines.push_back(i1 + "}) {");
        if (flow) {
            outcome_arms(ctx, lines, d + 1);
            lines.push_back(indent_of(d + 1) + "Err(ex) => {");
        }
        const std::size_t dc = flow ? d + 2 : d + 1;
        bool first = true;
        for (const ir::Node* cn : k.catches) {
            const auto* c = ir::get<ir::Catch>(cn);
            if (c == nullptr) throw Decline("a catch clause that is not one");
            std::vector<std::string> tests;
            std::string var_type = "Ref<dyn Throwable>";
            const ClassInfo* user = nullptr;
            for (const ir::TypeP& t : c->types) {
                tests.push_back("instance_of(Some(ex), " + rs_string(std::string(class_name(t))) + ")");
                if (const ClassInfo* pc = p_.program_class(t)) user = pc;
            }
            std::string bind = "Some(ex)";
            if (user != nullptr && c->types.size() == 1) {
                var_type = p_.rtype(c->types[0]);
                p_.need_caster(*user);
                bind = "cast_" + user->cname + "(Some(ex))";
            }
            lines.push_back(indent_of(dc) + (first ? "if " : "} else if ") + join(tests, " || ") + " {");
            lines.push_back(indent_of(dc + 1) + "let " + (assigned_.contains(ident(need_sym(c->sym, "a catch variable").name)) ? "mut " : "") +
                            ident(c->sym->name) + ": " + var_type + " = " + bind + ";");
            stmts(stmts_of(c->body), lines, dc + 1);
            first = false;
        }
        lines.push_back(indent_of(dc) + "} else {");
        lines.push_back(indent_of(dc + 1) + "throw(Some(ex));");
        lines.push_back(indent_of(dc) + "}");
        if (flow) lines.push_back(indent_of(d + 1) + "}");
        lines.push_back(i1 + "}");
    };

    if (k.finally_body == nullptr) {
        try_catch_part(out, depth);
        return;
    }
    // with a finally: everything runs under catch_all, the finally follows it whatever happened, then the outcome is carried on
    out.push_back(ind + "let outcome = catch_all(" + closure);
    try_ctx_.push_back(TryCtx{targets_.size(), {}});
    ++tries_;
    try_catch_part(out, depth + 1);
    if (flow) out.push_back(indent_of(depth + 1) + "Flow::Next");
    --tries_;
    const TryCtx ctx = std::move(try_ctx_.back());
    try_ctx_.pop_back();
    out.push_back(ind + "});");
    stmts(stmts_of(k.finally_body), out, depth);
    out.push_back(ind + "match outcome {");
    if (flow) outcome_arms(ctx, out, depth + 1);
    else out.push_back(indent_of(depth + 1) + "Ok(_) => {}");
    out.push_back(indent_of(depth + 1) + "Err(payload) => rethrow(payload),");
    out.push_back(ind + "}");
}

Lines Writer::ctor_call(const ir::CtorCall& call) {
    const ir::Sym& sym = need_sym(call.sym, "a constructor call");
    const std::string owner = sym.owner.value_or("");
    const bool is_this = call.kind && call.kind->starts_with("this");
    if (const ClassInfo* target = p_.model().info(owner)) {
        const auto match = p_.model().method_decl(sym);
        if (!match) throw Decline("a constructor of " + owner + " the program does not declare");
        std::vector<ir::TypeP> params;
        for (const ir::Node* pn : method_of(match->method).params) params.push_back(param_of(pn).type);
        std::vector<std::string> args = {"this"};
        for (const std::string& a : call_args(call.args, params, false)) args.push_back(a);
        return {p_.holder(*match->cls) + "::init" + p_.ctor_suffix(*match->cls, match->method) + "(" + join(args) + ");"};
        (void)target;
        (void)is_this;
    }
    if (owner == "java.lang.Object" || owner == "java.lang.Enum") return {};
    if (p_.is_lib_throwable(owner)) {
        std::vector<std::string> args = {"this"};
        std::string fn;
        const std::size_t n = call.args.size();
        if (n == 0) fn = "init";
        else if (n == 1 && class_name(type_of(call.args[0])) == "java.lang.String") fn = "init_message";
        else if (n == 1) fn = "init_cause";
        else if (n == 2) fn = "init_message_cause";
        else throw Decline("a library exception constructor with " + std::to_string(n) + " arguments");
        const std::vector<ir::TypeP> params = sym.params;
        for (const std::string& a : call_args(call.args, params, false)) args.push_back(a);
        return {"ThrowableData::" + fn + "(" + join(args) + ");"};
    }
    throw Decline("a constructor of the library class " + owner);
}

}  // namespace cb::translate::backends::rust_java::detail
